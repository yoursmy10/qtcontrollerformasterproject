#include "multiwindow.h"

//һЩȫ�ֱ���

threadsafe_queue<info_recv> queue_infoRecv_data;  //��ͨ���̲߳���,��������
threadsafe_queue<SENSOR_DATA> queue_sensor_data;  //���������ݶ��У���ʵÿ��ֻ����һ������

DEM dem_init[MAX_LEN][MAX_LEN]{};//��ȫ����Ϊ��,��������,��һά��Ӧlat,�ڶ�ά��Ӧlon
QMutex m_mutex; //��dem_init����
double least_distance = 5;  //least_distance��ʱ��Ϊ10m����λΪ��
double ref_lat = 0.00000899; //��ʱ����(1m����γ��)
double ref_lon = 0.00001141;//ĳһȷ����γ�ȵص�ľ�γ�����׵Ļ���(1m���پ���)
							//double pix2meters = 0.1; //������ʵ�ʳߴ�֮��
int stu_id = 1;  //��ÿ�����ݵ�id
int insert_order = 1; //tableview��Ӧ�ò�������
double lat_init = 31.14300;  //�������ݿ��ʼ��γ��,���ĵ�
double lon_init = 118.224572; //�������ݿ��ʼ�����ȣ����ĵ�
double GRAVITY = 9.8;//�������ٶ�

vector<vector<int>> serialFrozen;
/*
bool isFrozen_lat = 0; //��serial��latֵ�Ƿ��Ѿ�������Ĭ�ϲ�����
bool isFrozen_lon = 0; //��serial��lonֵ�Ƿ��Ѿ�������Ĭ�ϲ�����
bool isFrozen_hgt = 0; //��serial��hgtֵ�Ƿ��Ѿ�������Ĭ�ϲ�����
*/






multiwindow::multiwindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	//qDebug() << QSqlDatabase::drivers();
	//mybars = new QtDataVisualization::Q3DBars;
	//emit sigInitBars();

	//ע��ṹ��
	qRegisterMetaType<SENSOR_DATA>("SENSOR_DATA");
	qRegisterMetaType<actDet>("actDet");
	qRegisterMetaType<update_data>("update_data");
	qRegisterMetaType<info_recv>("info_recv");

	setLED(); //��led�Ƴ�ʼ��Ϊ�̵�
	
	
	customPlot = ui.widget_2dgraph;
	customPlot_velocity_LAD = ui.widget_velocity_LAD;
	dataTimer = new QTimer(this);//��datatimerʵ����
	
	drawPlot();//�൱�ڳ�ʼ����άͼ��
	drawPlot_velocity();//��ʼ���ٶȺ�LAD����ʾ��ά����ͼ

	
	timer = new QTimer(this);
	timer->setInterval(300);
	connect(timer, SIGNAL(timeout()), this, SLOT(replot()));
	timer->start(); //��ʱ���趨300ms,���ڶ�ʱ����ֱ�����ڶ�άͼ�е�λ��


	

	//ͨ�����߳�
	m_childThread_com = new QThread;
	mycom = new Communication;
	mycom->moveToThread(m_childThread_com);
	m_childThread_com->start(QThread::LowPriority);//���ȼ�Ҳ�Ƿֵ�

	//���θ����㷨�߳�,12/17Ӧ��ҲҪ��ͬ���ݿ��߳�
	m_childThread_DEMUpdate = new QThread;
	myDEMUpdate = new DEMUpdate;
	myDEMUpdate->moveToThread(m_childThread_DEMUpdate);
	m_childThread_DEMUpdate->start(QThread::LowPriority);
	
	//�澯�߳�
	m_childThread_LookAheadAlert = new QThread;
	myalert = new LookAheadAlert;
	myalert->moveToThread(m_childThread_LookAheadAlert);
	m_childThread_LookAheadAlert->start(QThread::LowPriority);



	
	//һЩ�̶���UI��ʾ,��һЩ��Ҫʵʱ���µ�ui���ݾ���Ҫ�òۺ������ź������и���
	//ui.lineEdit_least_distance->setText(QString::number(least_distance, 'f', 2));
	ui.lineEdit_ref_lat->setText(QString::number(ref_lat, 'f', 7));
	ui.lineEdit_ref_lon->setText(QString::number(ref_lon, 'f', 7));
	//ui.lineEdit_pix2meter->setText(QString::number(pix2meters, 'f', 3)); //pix2meter��һ�����Ų�ͬ�ϰ�����仯��ֵ
	ui.lineEdit_lat_init->setText(QString::number(lat_init, 'f', 7));
	ui.lineEdit_lon_init->setText(QString::number(lon_init, 'f', 7));
	connect(myDEMUpdate, SIGNAL(sigFinalResultDisplay(actDet)), this, SLOT(actDet_result_display(actDet)));//tableview�е���ʾ

	//QStandardItemModel *model = new QStandardItemModel();

	//�Զ���ui��ı��
	mymodel = new QStandardItemModel(ui.tableView);
	mymodel->setItem(0, 0, new QStandardItem("serial"));
	mymodel->setItem(0, 1, new QStandardItem("lat_bottom"));
	mymodel->setItem(0, 2, new QStandardItem("lat_top"));
	mymodel->setItem(0, 3, new QStandardItem("lon_left"));
	mymodel->setItem(0, 4, new QStandardItem("lon_right"));
	mymodel->setItem(0, 5, new QStandardItem("hgt(m)"));
	mymodel->setItem(0, 6, new QStandardItem("id"));
	ui.tableView->setModel(mymodel);

	//һЩ����
	connect(ui.pushButton_start, &QPushButton::clicked, this, &multiwindow::pushButton_start_clicked); //��ʼ����
	connect(this, SIGNAL(sigInitCom()), mycom, SLOT(sltInitCommu())); //��sigInitCom��ͨ�ų�ʼ�������																	  
	connect(this, SIGNAL(sigInitDEM()), myDEMUpdate, SLOT(initDEMupdate()));
	connect(myDEMUpdate, SIGNAL(sigDEMConsoleInfo(QString)), this, SLOT(sltAppendConsole_DEM(QString)));//�����θ����㷨�źŸ��µ��ı�����																										
	connect(mycom, SIGNAL(sigCOMConsoleInfo(QString)), this, SLOT(sltAppendConsole_COM(QString)));
	connect(myDEMUpdate, SIGNAL(sigDEMUpdate(actDet)), this, SLOT(updatePlot_obst(actDet)));//ͼ������-�ϰ���
	connect(myDEMUpdate, SIGNAL(sig2plot(SENSOR_DATA)), this, SLOT(updatePlot_heli(SENSOR_DATA)));//ͼ������-ֱ����λ��
	connect(myDEMUpdate, SIGNAL(sigDEMUpdate(actDet)), this, SLOT(tableviewUpdate(actDet)));//ͼ������-�ϰ���-tableview
	connect(mycom, SIGNAL(sigForAlert(double, double, double, double, double, double)), myalert, SLOT(sltRunAlert(double, double, double, double, double, double)));//���и澯�߳�
	connect(myalert, SIGNAL(sigColliAlert(int)), this, SLOT(sltChangeLight(int)));//�ı�ƹ���ɫ
	connect(myalert, SIGNAL(sigConsoleInfo(QString)), this, SLOT(sltAppendConsole_DEM(QString)));//�澯��Ϣ��ʾ��DEM��
	connect(mycom, SIGNAL(sigForAlert(double, double, double, double, double, double)), this, SLOT(sltUpdateHeliData(double, double, double, double, double, double)));//������ʾ��ֱ������Ϣ
	connect(myalert, SIGNAL(sigEmitLAD(double)), this, SLOT(sltRecLAD(double)));
	
	
}


/*
��ʼ���θ��½��̰�ť
creator: WU Xufeng
create time: 2021/12/11
*/
void multiwindow::pushButton_start_clicked()
{
	emit sigInitCom(); //����ͨ���߳�
	emit sigInitDEM();//�������θ����߳�
	ui.textEdit_commu->append("Start_CommuEngine!");
	ui.textEdit_detail->append("Start_DEMEngine!");
	//ui.pushButton_start->setDisabled(1);  //�������ÿ�ʼ��ʧЧ
}




/*
����ʾ���в����������
creator: WU Xufeng
create time: 2021/12/17
*/
void multiwindow::actDet_result_display(actDet rec)
{
	QList<QStandardItem*> list;
	list << new QStandardItem(rec.serial) << new QStandardItem(rec.lat_bottom) << new QStandardItem(rec.lat_top)\
		<< new QStandardItem(rec.lon_left) << new QStandardItem(rec.lon_right) << new QStandardItem(rec.hgt);
	//mymodel->insertRow(0,list); //�ڵ�0�в���һ����¼
	mymodel->appendRow(list); //��ʾ
	ui.lineEdit_serial->setText(QString::number(rec.serial));
	ui.lineEdit_hgt->setText(QString::number(rec.hgt, 'f', 6));
	ui.lineEdit_lat_bottom->setText(QString::number(rec.lat_bottom, 'f', 6));
	ui.lineEdit_lat_top->setText(QString::number(rec.lat_top, 'f', 6));
	ui.lineEdit_lon_left->setText(QString::number(rec.lon_left, 'f', 6));
	ui.lineEdit_lon_right->setText(QString::number(rec.lon_right, 'f', 6));
}



//���²�����ֱ�����йصĲ�����ʾ
void multiwindow::sltUpdateHeliData(double lat, double lon, double yaw_heli, double heli_hgt, double velocity_x, double velocity_y)
{
	double velocity = std::sqrt(pow(velocity_x, velocity_x) + pow(velocity_y, velocity_y)); //�����xyƽ���ϵ��ٶ�
	ui.lineEdit_heli_lat->setText(QString::number(lat));
	ui.lineEdit_heli_lon->setText(QString::number(lon));
	
	ui.lineEdit_heli_velocity->setText(QString::number(velocity));
	ui.textEdit_velocity->append(QString::number(velocity));
	ui.lineEdit_heli_yaw->setText(QString::number(yaw_heli));
}

void multiwindow::sltRecLAD(double LAD)
{
	ui.lineEdit_LAD->setText(QString::number(LAD)); //����LAD��ʾ
}






/*
��tableview�в����������
creator: WU Xufeng
create time: 2021/12/28
*/
void multiwindow::tableviewUpdate(actDet rec)
{
	QList<QStandardItem*> add_items;
	//int actDet_len = 7;
	
	//����һ�����������
	add_items << new QStandardItem(QString::number(rec.serial));
	add_items << new QStandardItem(QString::number(rec.lat_bottom, 'f', 10));
	add_items << new QStandardItem(QString::number(rec.lat_top, 'f', 10));
	add_items << new QStandardItem(QString::number(rec.lon_left, 'f', 10));
	add_items << new QStandardItem(QString::number(rec.lon_right, 'f', 10));
	add_items << new QStandardItem(QString::number(rec.hgt, 'f', 10));
	add_items << new QStandardItem(QString::number(rec.id));

	mymodel->insertRow(insert_order, add_items);
	//mymodel->appendRow(add_items);
	insert_order = insert_order + 1;//ָ����һ��
	
}




/*
��ʼ���θ��½���
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
�رյ��θ��½���
creator: WU Xufeng
create time: 2021/12/11
2021/12/11 �Ȳ�д��Ӧ����event������д
*/
void multiwindow::endDemUpdate()
{

}


/*
������άͼ�������
creator: WU Xufeng
create time: 2021/12/19
*/
void multiwindow::drawPlot()//Ӧ������һ�����ݽӿ�
{
	
	customPlot->addGraph();
	customPlot->setInteraction(QCP::iRangeDrag, true); //��굥���϶�
	customPlot->setInteraction(QCP::iRangeZoom, true); //���ֻ�������
	QPen pen;
	//customPlot->graph(0)->setScatterStyle(QCPScatterStyle(QPixmap("./sun.png"))); //�ļ�λ��Ҫ��
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
	customPlot->graph(1)->setLineStyle(QCPGraph::lsNone);//ɾ����
	//customPlot->graph(1)->setLineStyle(QCPGraph::lsNone);
	customPlot->graph(0)->setLineStyle(QCPGraph::lsNone);  //��ֱ����λ������
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
	customPlot->xAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));     // ������(��Ӧ�̶�)����
	customPlot->yAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
	customPlot->xAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine)); // ��������(��Ӧ�ӿ̶�)����
	customPlot->yAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
	customPlot->xAxis->grid()->setSubGridVisible(true);     // ��ʾ��������
	customPlot->yAxis->grid()->setSubGridVisible(true);
	customPlot->setBackground(QColor(236, 236, 236));
	
	//connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange))); //����Ӧ
	//connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));
	// setup legend:
	customPlot->legend->setFont(QFont(font().family(), 7));
	customPlot->legend->setIconSize(30, 15);
	customPlot->legend->setVisible(true);
	customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft | Qt::AlignTop);
	
}




/*
���¶�άͼ��ֱ������λ��
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
���¶�άͼ���ϰ���λ��
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


//��ʾ���θ��¼��澯�߳���Ϣ
void multiwindow::sltAppendConsole_DEM(QString msg_console) 
{
	ui.textEdit_detail->append(msg_console);
}
//��ʾͨ���߳���Ϣ
void multiwindow::sltAppendConsole_COM(QString msg_console)
{
	ui.textEdit_commu->append(msg_console);
}


void multiwindow::replot()
{
	customPlot->replot();
}


//��ʼ���澯�ƹ�
void multiwindow::setLED()
{
	int size = 50;
	ui.LED->setText("");
	// �����þ��δ�С
	// ���ui�������õ�label��С����С��Ⱥ͸߶�С�����ν�������Ϊ��С��Ⱥ���С�߶ȣ�
	// ���ui�������õ�label��С����С��Ⱥ͸߶ȴ󣬾��ν�������Ϊ����Ⱥ����߶ȣ�
	QString min_width = QString("min-width: %1px;").arg(size);              // ��С��ȣ�size
	QString min_height = QString("min-height: %1px;").arg(size);            // ��С�߶ȣ�size
	QString max_width = QString("max-width: %1px;").arg(size);              // ��С��ȣ�size
	QString max_height = QString("max-height: %1px;").arg(size);            // ��С�߶ȣ�size
																			// �����ñ߽���״���߿�
	QString border_radius = QString("border-radius: %1px;").arg(size / 2);    // �߿���Բ�ǣ��뾶Ϊsize/2
	QString border = QString("border:2px solid black;");                    // �߿�Ϊ1px��ɫ
																			// ������ñ�����ɫ
	QString background = "background-color:";
	background += "#31F6BC";

	//��ʼ�����ָ澯
	ui.lineEdit_alert->setText("SAFE");
	ui.lineEdit_alert->setStyleSheet("color: #31F6BC");

#if 0 
	switch (color) {
	case 0:
		// ��ɫ
		background += "rgb(190,190,190)";
		break;
	case 1:
		// ��ɫ
		background += "rgb(255,0,0)";
		break;
	case 2:
		// ��ɫ
		background += "rgb(0,255,0)";
		break;
	case 3:
		// ��ɫ
		background += "rgb(255,255,0)";
		break;
	default:
		break;
	}
#endif
	const QString SheetStyle = min_width + min_height + max_width + max_height + border_radius + border + background;
	ui.LED->setStyleSheet(SheetStyle);
}


//�ı�澯�ƹ�
void multiwindow::sltChangeLight(int i)
{
	int size = 50;
	QString min_width = QString("min-width: %1px;").arg(size);              // ��С��ȣ�size
	QString min_height = QString("min-height: %1px;").arg(size);            // ��С�߶ȣ�size
	QString max_width = QString("max-width: %1px;").arg(size);              // ��С��ȣ�size
	QString max_height = QString("max-height: %1px;").arg(size);            // ��С�߶ȣ�size
																			// �����ñ߽���״���߿�
	QString border_radius = QString("border-radius: %1px;").arg(size / 2);    // �߿���Բ�ǣ��뾶Ϊsize/2
	QString border = QString("border:2px solid black;");                    // �߿�Ϊ1px��ɫ
																			// ������ñ�����ɫ
	QString background = "background-color:";
	//background += "#31F6BC";
	//QString default_color = "#31F6BC";
	//background = background + default_color;


	switch (i) {
	case 0:
		//��ɫ
		background += "#31F6BC";
		ui.lineEdit_alert->setText("SAFE");
		ui.lineEdit_alert->setStyleSheet("color: #31F6BC");
		break;
	case 1:
		// ��ɫ
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



//��ʼ���ٶȣ�LAD��ʱ��ͼ
void multiwindow::drawPlot_velocity()
{

	//����ʱ��Ϊ������
	QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
	timeTicker->setTimeFormat("%h:%m:%s");
	customPlot_velocity_LAD->xAxis->setTicker(timeTicker);
	customPlot_velocity_LAD->yAxis->setTickLength(20, 0);
	// setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
	connect(dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
	dataTimer->start(2); // Interval 0 means to refresh as fast as possible


	customPlot_velocity_LAD->addGraph();
	customPlot_velocity_LAD->setInteraction(QCP::iRangeDrag, true); //��굥���϶�
	customPlot_velocity_LAD->setInteraction(QCP::iRangeZoom, true); //���ֻ�������
	QPen pen;
	//customPlot->graph(0)->setScatterStyle(QCPScatterStyle(QPixmap("./sun.png"))); //�ļ�λ��Ҫ��
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

	customPlot_velocity_LAD->addGraph(customPlot_velocity_LAD->xAxis,customPlot_velocity_LAD->yAxis2);  //��׼�ұߵ�y��
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
	customPlot_velocity_LAD->xAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));     // ������(��Ӧ�̶�)����
	customPlot_velocity_LAD->yAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
	customPlot_velocity_LAD->xAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine)); // ��������(��Ӧ�ӿ̶�)����
	customPlot_velocity_LAD->yAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
	customPlot_velocity_LAD->xAxis->grid()->setSubGridVisible(true);     // ��ʾ��������
	customPlot_velocity_LAD->yAxis->grid()->setSubGridVisible(true);
	customPlot_velocity_LAD->setBackground(QColor(236, 236, 236));

	//connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange))); //����Ӧ
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
	double key = time.elapsed() / 1000.0; // ��ʼ�����ڵ�ʱ�䣬��λ��
	static double lastPointKey = 0;
	if (key - lastPointKey > 0.002) // ��Լ2ms���һ������
	{
		// ������ݵ�graph
		QString tmp = ui.lineEdit_heli_velocity->text();
		double val = tmp.toDouble();
		customPlot_velocity_LAD->graph(0)->addData(key,val); //0��velocity
		tmp = ui.lineEdit_LAD->text();
		val = tmp.toDouble();
		customPlot_velocity_LAD->graph(1)->addData(key,val); //1��LAD
		//��¼��ǰʱ��
		lastPointKey = key;
	}
	// �����ܶ������Ĺؼ�������趨x�᷶ΧΪ���8��ʱ��
	customPlot->xAxis->setRange(key, 12, Qt::AlignRight);
	//��ͼ
	customPlot->replot();
	
}
