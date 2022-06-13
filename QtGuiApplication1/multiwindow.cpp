#include "multiwindow.h"

//一些全局变量

threadsafe_queue<info_recv> queue_infoRecv_data;  //由通信线程产生,公共队列
threadsafe_queue<SENSOR_DATA> queue_sensor_data;  //传感器数据队列，其实每次只接收一组数据

DEM dem_init[MAX_LEN][MAX_LEN]{};//先全部设为空,公共数组,第一维对应lat,第二维对应lon
QMutex m_mutex; //给dem_init上锁
double least_distance = 5;  //least_distance暂时设为10m，单位为米
double ref_lat = 0.00000899; //暂时给的(1m多少纬度)
double ref_lon = 0.00001141;//某一确定经纬度地点的经纬度与米的换算(1m多少经度)
							//double pix2meters = 0.1; //像素与实际尺寸之比
int stu_id = 1;  //给每个数据的id
int insert_order = 1; //tableview的应该插入行数
double lat_init = 31.14300;  //地形数据库初始化纬度,中心点
double lon_init = 118.224572; //地形数据库初始化经度，中心点
double GRAVITY = 9.8;//重力加速度

vector<vector<int>> serialFrozen;
/*
bool isFrozen_lat = 0; //该serial的lat值是否已经收敛，默认不收敛
bool isFrozen_lon = 0; //该serial的lon值是否已经收敛，默认不收敛
bool isFrozen_hgt = 0; //该serial的hgt值是否已经收敛，默认不收敛
*/






multiwindow::multiwindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	//qDebug() << QSqlDatabase::drivers();
	//mybars = new QtDataVisualization::Q3DBars;
	//emit sigInitBars();

	//注册结构体
	qRegisterMetaType<SENSOR_DATA>("SENSOR_DATA");
	qRegisterMetaType<actDet>("actDet");
	qRegisterMetaType<update_data>("update_data");
	qRegisterMetaType<info_recv>("info_recv");

	setLED(); //让led灯初始化为绿灯
	
	
	customPlot = ui.widget_2dgraph;
	customPlot_velocity_LAD = ui.widget_velocity_LAD;
	dataTimer = new QTimer(this);//讲datatimer实例化
	
	drawPlot();//相当于初始化二维图像
	drawPlot_velocity();//初始化速度和LAD的显示二维曲线图

	
	timer = new QTimer(this);
	timer->setInterval(300);
	connect(timer, SIGNAL(timeout()), this, SLOT(replot()));
	timer->start(); //计时器设定300ms,用于定时更新直升机在二维图中的位置


	

	//通信子线程
	m_childThread_com = new QThread;
	mycom = new Communication;
	mycom->moveToThread(m_childThread_com);
	m_childThread_com->start(QThread::LowPriority);//优先级也是分的

	//地形更新算法线程,12/17应该也要连同数据库线程
	m_childThread_DEMUpdate = new QThread;
	myDEMUpdate = new DEMUpdate;
	myDEMUpdate->moveToThread(m_childThread_DEMUpdate);
	m_childThread_DEMUpdate->start(QThread::LowPriority);
	
	//告警线程
	m_childThread_LookAheadAlert = new QThread;
	myalert = new LookAheadAlert;
	myalert->moveToThread(m_childThread_LookAheadAlert);
	m_childThread_LookAheadAlert->start(QThread::LowPriority);



	
	//一些固定的UI显示,另一些需要实时更新的ui内容就需要用槽函数和信号量进行更新
	//ui.lineEdit_least_distance->setText(QString::number(least_distance, 'f', 2));
	ui.lineEdit_ref_lat->setText(QString::number(ref_lat, 'f', 7));
	ui.lineEdit_ref_lon->setText(QString::number(ref_lon, 'f', 7));
	//ui.lineEdit_pix2meter->setText(QString::number(pix2meters, 'f', 3)); //pix2meter是一个随着不同障碍物而变化的值
	ui.lineEdit_lat_init->setText(QString::number(lat_init, 'f', 7));
	ui.lineEdit_lon_init->setText(QString::number(lon_init, 'f', 7));
	connect(myDEMUpdate, SIGNAL(sigFinalResultDisplay(actDet)), this, SLOT(actDet_result_display(actDet)));//tableview中的显示

	//QStandardItemModel *model = new QStandardItemModel();

	//自定义ui里的表格
	mymodel = new QStandardItemModel(ui.tableView);
	mymodel->setItem(0, 0, new QStandardItem("serial"));
	mymodel->setItem(0, 1, new QStandardItem("lat_bottom"));
	mymodel->setItem(0, 2, new QStandardItem("lat_top"));
	mymodel->setItem(0, 3, new QStandardItem("lon_left"));
	mymodel->setItem(0, 4, new QStandardItem("lon_right"));
	mymodel->setItem(0, 5, new QStandardItem("hgt(m)"));
	mymodel->setItem(0, 6, new QStandardItem("id"));
	ui.tableView->setModel(mymodel);

	//一些连接
	connect(ui.pushButton_start, &QPushButton::clicked, this, &multiwindow::pushButton_start_clicked); //开始工作
	connect(this, SIGNAL(sigInitCom()), mycom, SLOT(sltInitCommu())); //将sigInitCom与通信初始化程序绑定																	  
	connect(this, SIGNAL(sigInitDEM()), myDEMUpdate, SLOT(initDEMupdate()));
	connect(myDEMUpdate, SIGNAL(sigDEMConsoleInfo(QString)), this, SLOT(sltAppendConsole_DEM(QString)));//将地形更新算法信号更新到文本框中																										
	connect(mycom, SIGNAL(sigCOMConsoleInfo(QString)), this, SLOT(sltAppendConsole_COM(QString)));
	connect(myDEMUpdate, SIGNAL(sigDEMUpdate(actDet)), this, SLOT(updatePlot_obst(actDet)));//图画更新-障碍物
	connect(myDEMUpdate, SIGNAL(sig2plot(SENSOR_DATA)), this, SLOT(updatePlot_heli(SENSOR_DATA)));//图画更新-直升机位置
	connect(myDEMUpdate, SIGNAL(sigDEMUpdate(actDet)), this, SLOT(tableviewUpdate(actDet)));//图画更新-障碍物-tableview
	connect(mycom, SIGNAL(sigForAlert(double, double, double, double, double, double)), myalert, SLOT(sltRunAlert(double, double, double, double, double, double)));//运行告警线程
	connect(myalert, SIGNAL(sigColliAlert(int)), this, SLOT(sltChangeLight(int)));//改变灯光颜色
	connect(myalert, SIGNAL(sigConsoleInfo(QString)), this, SLOT(sltAppendConsole_DEM(QString)));//告警信息显示在DEM中
	connect(mycom, SIGNAL(sigForAlert(double, double, double, double, double, double)), this, SLOT(sltUpdateHeliData(double, double, double, double, double, double)));//更新显示的直升机信息
	connect(myalert, SIGNAL(sigEmitLAD(double)), this, SLOT(sltRecLAD(double)));
	
	
}


/*
开始地形更新进程按钮
creator: WU Xufeng
create time: 2021/12/11
*/
void multiwindow::pushButton_start_clicked()
{
	emit sigInitCom(); //启动通信线程
	emit sigInitDEM();//启动地形更新线程
	ui.textEdit_commu->append("Start_CommuEngine!");
	ui.textEdit_detail->append("Start_DEMEngine!");
	//ui.pushButton_start->setDisabled(1);  //开启后让开始键失效
}




/*
往显示表中插入更新数据
creator: WU Xufeng
create time: 2021/12/17
*/
void multiwindow::actDet_result_display(actDet rec)
{
	QList<QStandardItem*> list;
	list << new QStandardItem(rec.serial) << new QStandardItem(rec.lat_bottom) << new QStandardItem(rec.lat_top)\
		<< new QStandardItem(rec.lon_left) << new QStandardItem(rec.lon_right) << new QStandardItem(rec.hgt);
	//mymodel->insertRow(0,list); //在第0行插入一条记录
	mymodel->appendRow(list); //显示
	ui.lineEdit_serial->setText(QString::number(rec.serial));
	ui.lineEdit_hgt->setText(QString::number(rec.hgt, 'f', 6));
	ui.lineEdit_lat_bottom->setText(QString::number(rec.lat_bottom, 'f', 6));
	ui.lineEdit_lat_top->setText(QString::number(rec.lat_top, 'f', 6));
	ui.lineEdit_lon_left->setText(QString::number(rec.lon_left, 'f', 6));
	ui.lineEdit_lon_right->setText(QString::number(rec.lon_right, 'f', 6));
}



//更新部分与直升机有关的参数显示
void multiwindow::sltUpdateHeliData(double lat, double lon, double yaw_heli, double heli_hgt, double velocity_x, double velocity_y)
{
	double velocity = std::sqrt(pow(velocity_x, velocity_x) + pow(velocity_y, velocity_y)); //这个是xy平面上的速度
	ui.lineEdit_heli_lat->setText(QString::number(lat));
	ui.lineEdit_heli_lon->setText(QString::number(lon));
	
	ui.lineEdit_heli_velocity->setText(QString::number(velocity));
	ui.textEdit_velocity->append(QString::number(velocity));
	ui.lineEdit_heli_yaw->setText(QString::number(yaw_heli));
}

void multiwindow::sltRecLAD(double LAD)
{
	ui.lineEdit_LAD->setText(QString::number(LAD)); //更新LAD显示
}






/*
往tableview中插入更新数据
creator: WU Xufeng
create time: 2021/12/28
*/
void multiwindow::tableviewUpdate(actDet rec)
{
	QList<QStandardItem*> add_items;
	//int actDet_len = 7;
	
	//往新一行里添加数据
	add_items << new QStandardItem(QString::number(rec.serial));
	add_items << new QStandardItem(QString::number(rec.lat_bottom, 'f', 10));
	add_items << new QStandardItem(QString::number(rec.lat_top, 'f', 10));
	add_items << new QStandardItem(QString::number(rec.lon_left, 'f', 10));
	add_items << new QStandardItem(QString::number(rec.lon_right, 'f', 10));
	add_items << new QStandardItem(QString::number(rec.hgt, 'f', 10));
	add_items << new QStandardItem(QString::number(rec.id));

	mymodel->insertRow(insert_order, add_items);
	//mymodel->appendRow(add_items);
	insert_order = insert_order + 1;//指向下一行
	
}




/*
开始地形更新进程
creator: WU Xufeng
create time: 2021/12/11
*/
#if 0 
void multiwindow::startDemUpdate()
{
	ui.textEdit_detail->append("Start_Engine!");
	double lat_init = 31.14;
	double lon_init = 118.22;
	//mycom = new Communication();

	connect(this, SIGNAL(), mycom, SLOT(init_dem()));
}
#endif





/*
关闭地形更新进程
creator: WU Xufeng
create time: 2021/12/11
2021/12/11 先不写，应该用event进行描写
*/
void multiwindow::endDemUpdate()
{

}


/*
画出二维图表的主题
creator: WU Xufeng
create time: 2021/12/19
*/
void multiwindow::drawPlot()//应该留有一个数据接口
{
	
	customPlot->addGraph();
	customPlot->setInteraction(QCP::iRangeDrag, true); //鼠标单击拖动
	customPlot->setInteraction(QCP::iRangeZoom, true); //滚轮滑动缩放
	QPen pen;
	//customPlot->graph(0)->setScatterStyle(QCPScatterStyle(QPixmap("./sun.png"))); //文件位置要改
	customPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 5));
	customPlot->graph(0)->setPen(QPen(Qt::blue));
	// set graph name, will show up in legend next to icon:
	customPlot->graph(0)->setName("Helicopter Location");
#if 0
	QVector<double> lon, lat;
	QVector<double> lon_heli, lat_heli;  
	customPlot->graph(0)->setData(lon, lat);
#endif
	
	customPlot->addGraph();
	customPlot->graph(1)->setName("Obstacle Location");
	customPlot->graph(1)->setPen(QPen(Qt::red));
//	customPlot->graph(1)->setData(lon_heli, lat_heli);
	customPlot->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle,5));
	customPlot->graph(1)->setLineStyle(QCPGraph::lsNone);//删除线
	//customPlot->graph(1)->setLineStyle(QCPGraph::lsNone);
	customPlot->graph(0)->setLineStyle(QCPGraph::lsNone);  //给直升机位置连线
	// set title of plot:
	customPlot->plotLayout()->insertRow(0);
	customPlot->plotLayout()->addElement(0, 0, new QCPTextElement(customPlot, "DEM Database Update Real-time Display", QFont("JetBrain Mono", 12, QFont::Bold)));
	// axis configurations:

	//customPlot_velocity_LAD->yAxis2->setLabelFont(QFont("JetBrain Mono", 8, QFont::Thin));
	customPlot->xAxis->setLabel("Longitude");
	customPlot->yAxis->setLabel("Latitude");
	customPlot->xAxis->setLabelFont(QFont("JetBrain Mono", 8, QFont::Thin));
	customPlot->yAxis->setLabelFont(QFont("JetBrain Mono", 8, QFont::Thin));
	customPlot->xAxis2->setVisible(true);
	customPlot->yAxis2->setVisible(true);
	customPlot->xAxis2->setTickLabels(false);
	customPlot->yAxis2->setTickLabels(false);
	customPlot->xAxis2->setTicks(false);
	customPlot->yAxis2->setTicks(false);
	customPlot->xAxis2->setSubTicks(false);
	customPlot->yAxis2->setSubTicks(false);
	customPlot->xAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));     // 网格线(对应刻度)画笔
	customPlot->yAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
	customPlot->xAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine)); // 子网格线(对应子刻度)画笔
	customPlot->yAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
	customPlot->xAxis->grid()->setSubGridVisible(true);     // 显示子网格线
	customPlot->yAxis->grid()->setSubGridVisible(true);
	customPlot->setBackground(QColor(236, 236, 236));
	
	//connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange))); //自适应
	//connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));
	// setup legend:
	customPlot->legend->setFont(QFont(font().family(), 7));
	customPlot->legend->setIconSize(30, 15);
	customPlot->legend->setVisible(true);
	customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft | Qt::AlignTop);
	
}




/*
更新二维图中直升机的位置
creator: WU Xufeng
create time: 2021/12/20

*/
void multiwindow::updatePlot_heli(SENSOR_DATA tmp_sensor)
{
	qDebug() << "updatePLot_heli is working..." << endl;
	
	customPlot->graph(0)->addData(tmp_sensor.lat, tmp_sensor.lon);
	customPlot->rescaleAxes();
	customPlot->replot();
}


/*
更新二维图中障碍物位置
creator: WU Xufeng
create time: 2021/12/20
*/
void multiwindow::updatePlot_obst(actDet tmp_obst)
{
	qDebug() << "updatePLot_obst is working..." << endl;
	customPlot->graph(1)->addData(tmp_obst.lat_top, tmp_obst.lon_left);
	
	customPlot->replot();
	customPlot->graph(1)->addData(tmp_obst.lat_bottom, tmp_obst.lon_right);
	customPlot->rescaleAxes();
	customPlot->replot();
}


//显示地形更新及告警线程信息
void multiwindow::sltAppendConsole_DEM(QString msg_console) 
{
	ui.textEdit_detail->append(msg_console);
}
//显示通信线程信息
void multiwindow::sltAppendConsole_COM(QString msg_console)
{
	ui.textEdit_commu->append(msg_console);
}


void multiwindow::replot()
{
	customPlot->replot();
}


//初始化告警灯光
void multiwindow::setLED()
{
	int size = 50;
	ui.LED->setText("");
	// 先设置矩形大小
	// 如果ui界面设置的label大小比最小宽度和高度小，矩形将被设置为最小宽度和最小高度；
	// 如果ui界面设置的label大小比最小宽度和高度大，矩形将被设置为最大宽度和最大高度；
	QString min_width = QString("min-width: %1px;").arg(size);              // 最小宽度：size
	QString min_height = QString("min-height: %1px;").arg(size);            // 最小高度：size
	QString max_width = QString("max-width: %1px;").arg(size);              // 最小宽度：size
	QString max_height = QString("max-height: %1px;").arg(size);            // 最小高度：size
																			// 再设置边界形状及边框
	QString border_radius = QString("border-radius: %1px;").arg(size / 2);    // 边框是圆角，半径为size/2
	QString border = QString("border:2px solid black;");                    // 边框为1px黑色
																			// 最后设置背景颜色
	QString background = "background-color:";
	background += "#31F6BC";

	//初始化文字告警
	ui.lineEdit_alert->setText("SAFE");
	ui.lineEdit_alert->setStyleSheet("color: #31F6BC");

#if 0 
	switch (color) {
	case 0:
		// 灰色
		background += "rgb(190,190,190)";
		break;
	case 1:
		// 红色
		background += "rgb(255,0,0)";
		break;
	case 2:
		// 绿色
		background += "rgb(0,255,0)";
		break;
	case 3:
		// 黄色
		background += "rgb(255,255,0)";
		break;
	default:
		break;
	}
#endif
	const QString SheetStyle = min_width + min_height + max_width + max_height + border_radius + border + background;
	ui.LED->setStyleSheet(SheetStyle);
}


//改变告警灯光
void multiwindow::sltChangeLight(int i)
{
	int size = 50;
	QString min_width = QString("min-width: %1px;").arg(size);              // 最小宽度：size
	QString min_height = QString("min-height: %1px;").arg(size);            // 最小高度：size
	QString max_width = QString("max-width: %1px;").arg(size);              // 最小宽度：size
	QString max_height = QString("max-height: %1px;").arg(size);            // 最小高度：size
																			// 再设置边界形状及边框
	QString border_radius = QString("border-radius: %1px;").arg(size / 2);    // 边框是圆角，半径为size/2
	QString border = QString("border:2px solid black;");                    // 边框为1px黑色
																			// 最后设置背景颜色
	QString background = "background-color:";
	//background += "#31F6BC";
	//QString default_color = "#31F6BC";
	//background = background + default_color;


	switch (i) {
	case 0:
		//绿色
		background += "#31F6BC";
		ui.lineEdit_alert->setText("SAFE");
		ui.lineEdit_alert->setStyleSheet("color: #31F6BC");
		break;
	case 1:
		// 红色
		background += "rgb(255,0,0)";
		ui.lineEdit_alert->setText("WARNING");
		ui.lineEdit_alert->setStyleSheet("color: rgb(255, 0, 4)");
		break;

	default:
		break;
	}

	
	const QString SheetStyle = min_width + min_height + max_width + max_height + border_radius + border + background;
	ui.LED->setStyleSheet(SheetStyle);
}



//初始化速度，LAD与时间图
void multiwindow::drawPlot_velocity()
{

	//设置时间为横坐标
	QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
	timeTicker->setTimeFormat("%h:%m:%s");
	customPlot_velocity_LAD->xAxis->setTicker(timeTicker);
	customPlot_velocity_LAD->yAxis->setTickLength(20, 0);
	// setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
	connect(dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
	dataTimer->start(2); // Interval 0 means to refresh as fast as possible


	customPlot_velocity_LAD->addGraph();
	customPlot_velocity_LAD->setInteraction(QCP::iRangeDrag, true); //鼠标单击拖动
	customPlot_velocity_LAD->setInteraction(QCP::iRangeZoom, true); //滚轮滑动缩放
	QPen pen;
	//customPlot->graph(0)->setScatterStyle(QCPScatterStyle(QPixmap("./sun.png"))); //文件位置要改
	customPlot_velocity_LAD->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 5));
	customPlot_velocity_LAD->graph(0)->setPen(QPen(Qt::blue));
	// set graph name, will show up in legend next to icon:
	customPlot_velocity_LAD->graph(0)->setName("Helicopter Velocity");
	customPlot_velocity_LAD->graph(0)->setLineStyle(QCPGraph::lsLine);
#if 0
	QVector<double> lon, lat;
	QVector<double> lon_heli, lat_heli;
	customPlot->graph(0)->setData(lon, lat);
#endif

	customPlot_velocity_LAD->addGraph(customPlot_velocity_LAD->xAxis,customPlot_velocity_LAD->yAxis2);  //对准右边的y轴
	customPlot_velocity_LAD->graph(1)->setName("LAD");
	customPlot_velocity_LAD->graph(1)->setPen(QPen(Qt::red));
	//	customPlot->graph(1)->setData(lon_heli, lat_heli);
	customPlot_velocity_LAD->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));
	customPlot_velocity_LAD->graph(1)->setLineStyle(QCPGraph::lsLine);
	//customPlot->graph(1)->setLineStyle(QCPGraph::lsNone);
	
	// set title of plot:
	customPlot_velocity_LAD->plotLayout()->insertRow(0);
	//customPlot_velocity_LAD->plotLayout()->addElement(0, 0, new QCPTextElement(customPlot, "Title", QFont("JetBrain Monos", 8, QFont::Bold)));
	// axis configurations:
	customPlot_velocity_LAD->xAxis->setLabelFont(QFont("JetBrain Mono", 8, QFont::Thin));
	customPlot_velocity_LAD->yAxis->setLabelFont(QFont("JetBrain Mono", 8, QFont::Thin));
	customPlot_velocity_LAD->yAxis2->setLabelFont(QFont("JetBrain Mono", 8, QFont::Thin));
	customPlot_velocity_LAD->yAxis->setLabel("Velocity(m/s)");
	customPlot_velocity_LAD->xAxis->setLabel("Time(s)");
	customPlot_velocity_LAD->yAxis2->setLabel("LAD(m)");
	customPlot_velocity_LAD->xAxis2->setVisible(true);
	customPlot_velocity_LAD->yAxis2->setVisible(true);
	customPlot_velocity_LAD->xAxis2->setTickLabels(false);
	customPlot_velocity_LAD->yAxis2->setTickLabels(true);
	customPlot_velocity_LAD->xAxis2->setTicks(false);
	customPlot_velocity_LAD->yAxis2->setTicks(true);
	customPlot_velocity_LAD->xAxis2->setSubTicks(false);
	customPlot_velocity_LAD->yAxis2->setSubTicks(true);
	customPlot_velocity_LAD->xAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));     // 网格线(对应刻度)画笔
	customPlot_velocity_LAD->yAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
	customPlot_velocity_LAD->xAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine)); // 子网格线(对应子刻度)画笔
	customPlot_velocity_LAD->yAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
	customPlot_velocity_LAD->xAxis->grid()->setSubGridVisible(true);     // 显示子网格线
	customPlot_velocity_LAD->yAxis->grid()->setSubGridVisible(true);
	customPlot_velocity_LAD->setBackground(QColor(236, 236, 236));

	//connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange))); //自适应
	//connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));
	// setup legend:
	customPlot_velocity_LAD->legend->setFont(QFont(font().family(), 6));
	customPlot_velocity_LAD->legend->setIconSize(15, 8);
	customPlot_velocity_LAD->legend->setVisible(true);
	customPlot_velocity_LAD->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft | Qt::AlignTop);
}




void multiwindow::sltUpdateVelandLADPlot()
{
	static QTime time(QTime::currentTime());
	double key = time.elapsed() / 1000.0; // 开始到现在的时间，单位秒
	static double lastPointKey = 0;
	if (key - lastPointKey > 0.002) // 大约2ms添加一次数据
	{
		// 添加数据到graph
		QString tmp = ui.lineEdit_heli_velocity->text();
		double val = tmp.toDouble();
		customPlot_velocity_LAD->graph(0)->addData(key,val); //0是velocity
		tmp = ui.lineEdit_LAD->text();
		val = tmp.toDouble();
		customPlot_velocity_LAD->graph(1)->addData(key,val); //1是LAD
		//记录当前时刻
		lastPointKey = key;
	}
	// 曲线能动起来的关键在这里，设定x轴范围为最近8个时刻
	customPlot->xAxis->setRange(key, 12, Qt::AlignRight);
	//绘图
	customPlot->replot();
	
}
