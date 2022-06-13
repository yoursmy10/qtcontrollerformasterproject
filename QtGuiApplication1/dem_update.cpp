#include<dem_update.h>
#include <Eigen/Dense>
#include<thread>
#include<numeric>
//#include <boost/math/distributions/normal.hpp>
#include <cmath>



using namespace std;
using namespace Eigen;





DEMUpdate::DEMUpdate(QObject *parent) :
	QObject(parent)
{

}

DEMUpdate::~DEMUpdate()
{
	myDB->deleteLater(); //删除database
	QMutexLocker m_lock(&m_mutex);

}





/*
时间：2021/12/14
创建人：吴旭峰
将主动探测得到的数据进行计算/处理，是主动探测数据进来的第一步
*/
void DEMUpdate::actDet2latlon(update_data updateData)
{
	//计算得到范围坐标及序列号
	double lat_temp = updateData.lat + updateData.dr * cos(updateData.theta*PI / 180) * ref_lat;
	//theta是相对于地面坐标系来说的，或者说是根据经度方向和雷达方向的夹角
	double lon_temp = updateData.lon + updateData.dr * sin(updateData.theta*PI / 180) * ref_lon;
	double delta_lat = updateData.dw / 2 * ref_lat;
	double delta_lon = updateData.dw / 2 * ref_lon;  //这里取了近似，默认建筑物比较方
	//double hgt_temp = updateData.hc + updateData.alt;
	double hgt_temp = updateData.hc;
	//double lat_left = lat_temp - delta_lat;
	double lat_bottom = lat_temp - delta_lat;
	//double lat_right = lat_temp + delta_lat;
	double lat_top = lat_temp + delta_lat;
	//double lon_bottom = lon_temp - delta_lon;
	double lon_left = lon_temp - delta_lon;
	//double lon_top = lon_temp + delta_lon;
	double lon_right = lon_temp + delta_lon;

	actDet tmp_actDet; //对其进行赋值
	tmp_actDet.lat_bottom = lat_bottom;
	tmp_actDet.lat_top = lat_top;
	tmp_actDet.serial = updateData.serial;
	tmp_actDet.lon_left = lon_left;
	tmp_actDet.lon_right = lon_right;
	tmp_actDet.hgt = hgt_temp;
	tmp_actDet.id = stu_id;
	stu_id++;


	emit sigAdd2DB(tmp_actDet);//添加到了student表中，就是未经过处理的最原始值

	//用于计算马氏距离需要的协方差矩阵
	double latmean = myDB->calcMean(QString::number(tmp_actDet.serial), "student", 10, 2);
	double lonmean = myDB->calcMean(QString::number(tmp_actDet.serial), "student", 10, 3);
	double density = 0.5;
	//计算当前值与前一个值之间的马氏距离,只有当student队列中满10个值时才开始计算
	if (latmean != -1 && lonmean != -1)
	{
		tmp_actDet = mahaDistance(tmp_actDet, lonmean, latmean, &density);
		//如果返回值为全0，那么就停止整个过程
		if(tmp_actDet.serial ==0 && tmp_actDet.hgt ==0)
		{
			qDebug() << "activate TRUNCTED_DENSITY!!!" << endl;
			qDebug() << "density = " << density << endl;
			emit sigDEMConsoleInfo("density = " + QString::number(density));//发送到控制台显示
			return;
		}
		
		//density = density / 2;
		
		qDebug() << "density = " << density << endl;
		emit sigDEMConsoleInfo("density = " + QString::number(density));//发送到控制台显示
		
		tmp_actDet = lowpassFilter(tmp_actDet, density, density, density);
		myDB->addMember_final(tmp_actDet);
		return;
	}

	else
	{
		tmp_actDet = lowpassFilter(tmp_actDet, 0.5, 0.5, 0.5);
		myDB->addMember_final(tmp_actDet);
		return;
	}
	//lowpass&&mahalanobis

	
	

}






/*
时间：2021/10/28
创建人：吴旭峰
用于更新地形高程数据库，
query：障碍物序号。从视觉来
dr：相对距离。
theta：方位角。弧度。
dw：障碍物宽。从视觉来
hc：障碍物相对于摄像机中心点的高度。从视觉来
lat：拍摄该图片时直升机纬度。
lon：拍摄该图片时直升机经度。
alt：拍摄该图片时直升机无线电高。
dem_base：地形高程数据库
*/
void DEMUpdate::database_update(actDet rec)
{

	
	qDebug() << "database_update is working... \n" << endl;
	QString msg_console = "database_update is working... \n";
	emit sigDEMConsoleInfo(msg_console);


	// 	double delta_lat = dw / 2 * ref_lat;
	// 	double delta_lon = dw / 2 * ref_lon;  
	//double lat_left = lat_temp - delta_lat;
	double lat_bottom = rec.lat_bottom;
	//double lat_right = lat_temp + delta_lat;
	double lat_top = rec.lat_top;
	//double lon_bottom = lon_temp - delta_lon;
	double lon_left = rec.lon_left;
	//double lon_top = lon_temp + delta_lon;
	double lon_right = rec.lon_right;
	int serial = rec.serial;
	double delta_lat = abs((lat_top - lat_bottom) / 2 * ref_lat);
	double delta_lon = abs((lon_right - lon_left) / 2 * ref_lon);//这里取了近似，默认建筑物比较方
	double hgt = rec.hgt;

	//显示计算出的障碍物坐标范围
	//res_display = rec;  //这个是用来显示的
	emit sigFinalResultDisplay(rec);//发送信号用于给主线程显示
	
	#if 0
	printf("serial = %d \n", serial);
	printf("lat_bottom = %f \n", lat_bottom);
	printf("lon_right =  %f \n", lon_right);
	printf("lat_top = %f \n", lat_top);
	printf("lon_left = %f \n", lon_left);
	#endif


	//m_mutex.lock();
	//查图：我只要知道左上角[0][0]点的经纬度，知道每一格之间的值差多少(least_distance)，我就能够直接计算出合适的区域
	double lat_init = dem_init[0][0].lat;  //数据库左上角经度
	double lon_init = dem_init[0][0].lon;  //数据库左上角纬度
	
										   //int rows = dem_base.size();
										   //int rows = 0; //行
										   //int rows = sizeof(dem_init)/sizeof(dem_init[0]);
	int rows = MAX_LEN;
	//int cols = dem_base[0].size();  //有点问题
	//int cols = 0;  //列
	//int cols = sizeof(dem_init)/sizeof(dem_init[0][0]);
	int cols = MAX_LEN;
	//判断左上是否出界，右下还没判定
#if 0
	if (lat_bottom < lat_init && lon_right > lon_init)
	{
		printf("out of range ! \n");
		return;
	}
#endif


	int m(0), n(0);  //计数
	m = abs(floor((lat_bottom - lat_init) / least_distance*ref_lat));  //得到左上点纵坐标
	n = abs(floor((lon_init - lon_right) / least_distance*ref_lon));   //得到左上点横坐标

	//计算右下角,这里是不完善的。边界情况考虑得不全，至少还有两种情况，一种是右下角出了数据库左上角，一种是左上角是否出了数据库右下角
	int len_col(0), len_row(0);
	len_row = ceil(delta_lat / least_distance*ref_lat);  //取大
	len_col = ceil(delta_lon / least_distance*ref_lon);

	//更新，更新的原则就是按照格子来，如果更新区域（lat,lon）角点落在格子中，则整个格子的高程更新为新区域的高程，这是一种冗余，但是这种冗余的量还有待观察，不清楚是不是要扩呢？
	//可不可以考虑一些高端一点的算法，让这个更新更加准确。（可以查点论文）
	
#if 0
	if (m + len_row > rows || n + len_col > cols)
	{
		qDebug() << ("右下角超出范围，将只更新包括在高程图的内信息 \n");
		for (int i = m; i < rows; i++)
		{
			for (int j = n; j < cols; j++)
			{
				dem_init[i][j].hgt = hgt;
				cout << "坐标为(" << dem_init[i][j].lat << "," << dem_init[i][j].lon << ")的点, 其高程被更新为" << dem_init[i][j].hgt << endl;
			}
		}
		qDebug() << ("序号为 %d 的障碍物在高程图中更新完成 \n", serial);
	}
#endif


	for (int i = m; i <= m + len_row; i++)  //找临近点更新
	{
		for (int j = n; j <= n + len_col; j++)
		{
			//qDebug() << "point(" << dem_init[i][j].lat << "," << dem_init[i][j].lon << "," << dem_init[i][j].hgt << ")";
			dem_init[i][j].hgt = hgt;
			//dem_init[i][j].hgt = 0;
			//qDebug() << "is updated, its hgt is now updated to:" << dem_init[i][j].hgt << endl;
		}
	}
	qDebug() << ("obstacle :serial: %d is updated in the DEM successfully\n", serial);
	msg_console = "obstacle :serial:" + QString::number(serial) + "  is updated in the DEM successfully\n";
	emit sigDEMConsoleInfo(msg_console);
	//m_mutex.unlock();
	return;
}



/*
time：2021/11/5
creator：吴旭峰
模拟雷达处理数据
这里暂时只考虑了平飞这一种情况，应该返回的是vector<update_data>update_data,然后一个一个输入到数据库更新算法中.
这个函数不应该叫模拟雷达函数，应该叫预处理函数
*/
vector<update_data> DEMUpdate::preProcessor()
{
	qDebug() << "preProcessor is working..." << endl;
	
	QString msg_console = "preProcessor is working...\n";
	emit sigDEMConsoleInfo(msg_console);
	
	vector<info_recv> recv_tmp;  //接收从通信线程传过来的数据
	vector<update_data> update_data_ret = {}; //要传入到更新算法中的数据dc

	//default_random_engine r;
	//uniform_real_distribution<double> distance_rand(50.0, 260.0); //随机距离
	//uniform_real_distribution<double> theta_rand(0, 45.0); //随机角度
	//uniform_real_distribution<double> alt_rand(100.0, 110.0); //随机高度，较稳定

	//传感器队列没有几组队列这个问题，因为他只有一组数据,如果数据队列中有超过一组数据，就要先把前面的去掉，留最新的
	while (queue_sensor_data.size() > 1)
	{
		queue_sensor_data.wait_and_pop();
	}
	SENSOR_DATA tmp_sensor;
	queue_sensor_data.wait_and_pop(tmp_sensor);
	//如果超过就重新接收

	//如果角速度过大，就停止更新
	if (tmp_sensor.angular_vel_z > MAX_ANGULAR_VEL || tmp_sensor.angular_vel_y > MAX_ANGULAR_VEL || tmp_sensor.angular_vel_z > MAX_ANGULAR_VEL)
	{
		qDebug() << "angular_vel out of range!" << endl;
		msg_console = "angular_vel out of range!\n";//角速度过大的情况		
		emit sigDEMConsoleInfo(msg_console);//发到界面上
		
		queue_infoRecv_data.clear();
		queue_sensor_data.clear();
		return update_data_ret;
	}

															  
	int len = queue_infoRecv_data.size();//获取队列的size，也就是有几组数据的问题
	//从通信函数产生的队列中接数据,按照逻辑来说这就是一幅图中的所有利用视觉探测到的障碍物
	for (int i = 0; i<len; i++)
	{
		//应该先用一个结构体来接,这个也是一个过渡结构体
		//recv_tmp.push_back(queue_infoRecv_data->wait_and_pop()); //用结构体接队列的数据
		info_recv tmp;
		queue_infoRecv_data.wait_and_pop(tmp);  //把队列中的值赋值给recv_tmp[i]
		recv_tmp.push_back(tmp);


		//recv_tmp.push_back(queue_infoRecv_data.wait_and_pop());
		/*
		serial[i] = queue_infoRecv_data->front().serial;
		x_left[i] = queue_infoRecv_data->front().x_left;
		x_right[i] = queue_infoRecv_data->front().x_right;
		y_top[i] = queue_infoRecv_data->front().y_top;
		y_bottom[i] = queue_infoRecv_data->front().y_bottom;
		queue_infoRecv_data->pop();
		*/
		//test
		/*
		cout<<"recv_tmp["<<i<<"] = " << recv_tmp[i].serial<<endl;
		*/
	}



	//先把参数random出来，先不管雷达的运作模式,要注意一些参数之间的逻辑关系
	//经纬度用南纬是负，北纬是正，东经是正，西经是负。
	for (int i = 0; i<len; i++)
	{
		update_data updateData;
		double judge = (max(recv_tmp[i].y_top, recv_tmp[i].y_bottom) - IMG_HEIGHT / 2);

		//double judge = 1;
		//如果障碍物还不到图片的中心点高，或者dr和最远雷达距离差不多，也就是说可能探测到了无限远的地方，那么判断这个建筑物和我是不会相撞的，我就不需要去更新它，减轻任务负担
		if (judge <= 0 || abs(recv_tmp[i].distance - MAX_RADAR_LEN) <= ERROR_BETWEEN_RADARLEN_DR) 
		{
			continue;
		}

		double pix2meters = abs(recv_tmp[i].distance*sin(recv_tmp[i].yaw_angle*PI/180) / abs((recv_tmp[i].x_right + recv_tmp[i].x_left)/2 - IMG_WIDTH/2));

		//updateData.dr = distance_rand(r); //random的
		updateData.dr = recv_tmp[i].distance;
		//updateData.theta = theta_rand(r); //random的
		updateData.theta = tmp_sensor.heli_yaw + recv_tmp[i].yaw_angle;//直升机方位角加上传感器方位角
		updateData.dw = (recv_tmp[i].x_right - recv_tmp[i].x_left)*pix2meters;
		updateData.alt = tmp_sensor.alt_gps; //GPS的值
		updateData.serial = recv_tmp[i].serial;
		updateData.lat = abs(tmp_sensor.lat); //GPS的值
		updateData.lon = abs(tmp_sensor.lon); //GPS的值
		//
		//20220322重写计算hgt算法
		//表示障碍物最下面那一横还在图里显示，而不是在图外,不一定对，先写着
		//*****************************************************************************
		/*
		if (recv_tmp[i].y_top != 0 && recv_tmp[i].y_bottom != 0 && recv_tmp[i].y_top < IMG_HEIGHT - 2 && recv_tmp[i].y_bottom < IMG_HEIGHT - 2)
		{
			updateData.hc = abs(recv_tmp[i].y_top - recv_tmp[i].y_bottom)*pix2meters;
		}
		else
		{
			updateData.hc = (judge - IMG_HEIGHT / 2)*pix2meters + updateData.alt;
		}
		*/
		//***********************************************************************

		updateData.hc = abs(recv_tmp[i].y_top - recv_tmp[i].y_bottom)*pix2meters;
		
		qDebug() << "hc = " << updateData.hc << endl;
		qDebug() << " recv_tmp[i].y_top" << recv_tmp[i].y_top << endl;
		qDebug() << " recv_tmp[i].y_bottom" << recv_tmp[i].y_bottom << endl;
		qDebug() << "pix2meter = " << pix2meters << endl;

		
		update_data_ret.push_back(updateData);
	}

	//这里就把需要的数据都获取完了，这里面有一张图中所有探测到的障碍物的数据，是一个结构体数组，然后一张一张传到地形更新算法中去
	emit sigDEMConsoleInfo("PreProcessor done...");
	myDB->addMember_helistate(tmp_sensor);//将直升机飞行状态记录到库中
	emit sig2plot(tmp_sensor); //主要是将直升机姿态和位置传到画图函数
	return update_data_ret;
}



/*
时间：2021/11/06
创建人：吴旭峰
初始化生成高程图
lat_init : 原点经度
lon_init : 原点纬度
least_distance : 相邻两点间距
MAX_LEN / least_distance的取值取决于你想覆盖多大的区域，测试用
默认初始点在图中间
*/
//TODO 可以用专门的稀疏矩阵进行存储，现在这样很占内存
void DEMUpdate::init_dem(double lat_init, double lon_init, double least_distance)
{
	for (int i = 0; i < MAX_LEN; i++)//i for lat, j for lon,生成默认地图，这里默认原点在最左下角。
	{
		for (int j = 0; j < MAX_LEN; j++)
		{
			dem_init[i][j].hgt = 0;
			dem_init[i][j].lon = lon_init + j*least_distance*ref_lon - 0.5*MAX_LEN*least_distance*ref_lon;
			dem_init[i][j].lat = lat_init + i*least_distance*ref_lat - 0.5*MAX_LEN*least_distance*ref_lat;
		}

	}
	qDebug() << ("dem initiated successfully");
	emit sigDEMConsoleInfo("dem map initiated successfully...");
	//return dem_init[MAX_LEN][MAX_LEN];
}



/*
时间：2021/12/15
创建人：吴旭峰
计算数据库传过来的达到阈值的数列，计算得到一个最合理值，输入到地形更新算法中,他现在的算法就是把表中，每5个平均值输出一遍，然后每加5个之后再把总和拿来输出一遍，求一个平均这样
*/
void DEMUpdate::calculator(vector<actDet> msg)
{
	qDebug() << "calculator is working ..." << endl;
	//QString msg_console = "calculator is working ...";
	emit sigDEMConsoleInfo("calculator is working ...");

	//msg = mahaDistance(msg); //对msg进行马氏距离计算，剔除离群点
	
	int len = msg.size(); //输入的长度
	actDet res;//返回值


	//加入一个判定看表中某项是否为空
	/*
	for (int i = 0; i < len;i++)
	{
		res = lowpassFilter(msg[i], 0.5, 0.5, 0.5);
		//这一步就是过滤数据，如果达到了收敛阈值那么这个值就在更新到地形高程图中时就一直是一个定值,并将这代表最终输入给地形更新算法的数据添加到finalotput表中
		res = myDB->calcMeanandCov(res);
		myDB->addMember_final(res);
	}
	*/
	//得到最后一项的res


	




	///////////////////////////////////////////添加遗忘系数220327
	///
	vector<double>forgottenIndex = forgotten_index(len);

	
	//先求个平均值凑合吧
	vector<double> lat_top_list;
	vector<double> lon_left_list;
	vector<double> hgt_list;
	vector<double> lat_bottom_list;
	vector<double> lon_right_list;

/*
	for (int i = 0; i < len; i++)
	{
		//每个值在进队列是就先乘上遗忘系数
		lat_top_list.push_back((msg[i].lat_top)*forgottenIndex[i]);
		lat_bottom_list.push_back((msg[i].lat_bottom)*forgottenIndex[i]);
		lon_left_list.push_back((msg[i].lon_left)*forgottenIndex[i]);
		lon_right_list.push_back((msg[i].lon_right)*forgottenIndex[i]);
		hgt_list.push_back((msg[i].hgt)*forgottenIndex[i]);
	}
*/


	for (int i = 0; i < len; i++)
	{
		//每个值在进队列是就先乘上遗忘系数
		lat_top_list.push_back((msg[i].lat_top));
		lat_bottom_list.push_back((msg[i].lat_bottom));
		lon_left_list.push_back((msg[i].lon_left));
		lon_right_list.push_back((msg[i].lon_right));
		hgt_list.push_back((msg[i].hgt));
	}
	
	//求均值,后期肯定要改用其他办法
	double sumValue = accumulate(begin(lat_top_list), end(lat_top_list), 0.0);
	double lat_top = sumValue / len;
	lat_top_list.clear();
	sumValue = accumulate(begin(lat_bottom_list), end(lat_bottom_list), 0.0);
	double lat_bottom = sumValue / len;
	lat_bottom_list.clear();
	sumValue = accumulate(begin(lon_left_list), end(lon_left_list), 0.0);
	double lon_left = sumValue / len;
	lon_left_list.clear();
	sumValue = accumulate(begin(lon_right_list), end(lon_right_list), 0.0);
	double lon_right = sumValue / len;
	lon_right_list.clear();
	sumValue = accumulate(begin(hgt_list), end(hgt_list), 0.0);
	double hgt = sumValue / len;
	hgt_list.clear();

	//新建返回值
	
	res.hgt = hgt;
	res.lat_bottom = lat_bottom;
	res.lat_top = lat_top;
	res.lon_left = lon_left;
	res.lon_right = lon_right;
	res.serial = msg[0].serial;
	res.id = msg[0].id;  //随便给的

	myDB->addMember_display(res);

	
	emit sigDEMUpdate(res); //发送信号，让地形更新算法工作，同时也将数据发送到二维图中更新障碍物位置信息
	emit sigFinalResultDisplay(res);//达到阈值的数列
	emit sigDEMConsoleInfo("Calculating Finished...");
	//return res;

}


//初始化地形更新线程
void DEMUpdate::initDEMupdate()
{
	myDB = new DataBase();
	myDB->sltRunDB();
	
	//TODO 初始化数据库，让它run起来
	connect(this, SIGNAL(sigAdd2DB(actDet)), myDB, SLOT(addMember(actDet)));  //act2latlon-->addmember
	connect(myDB, SIGNAL(startGet(QString)), myDB, SLOT(getClass(QString)));  //addmember --> getclass
	connect(myDB, SIGNAL(sigStartDEMUP(vector<actDet>)), this, SLOT(calculator(vector<actDet>))); //getclass-->calculator
	connect(this, SIGNAL(sigDEMUpdate(actDet)), this ,SLOT(database_update(actDet))); //calculator --> database_update
	connect(myDB, SIGNAL(sigDBinfo(QString)), this, SLOT(sltSendDBinfo(QString)));//发送数据库的信息给显示界面


	//下面两步都是在calculator中实现的
	connect(this, SIGNAL(sigAdd2DB_lowpass(actDet)), myDB, SLOT(addMember_lowpass(actDet)));  //lowpassfilter-->addmember_lowpass
	connect(this, SIGNAL(sigAdd2DB_mahala(actDet, double)), myDB, SLOT(addMember_maha(actDet, double)));//mahaladistance-->addmember_maha
	
	
	init_dem(lat_init,lon_init,least_distance); //用全局变量初始化地形高程图

	qDebug() << "task_dem_update initiated successfully" << endl;
	QString msg_console = "task_dem_update initiated successfully\n";
	emit(sigDEMConsoleInfo(msg_console));
	
	while (1) //循环，里面用信号量阻塞，或者是判断一下队列是否为空？，
	{
		if (!queue_infoRecv_data.empty())//如果队列非空，则对地形高程图执行更新操作. 2021/11/08/: 在双线程情况下这里会有问题，那就是就算队列只更新了一个，他这里判定也通过了，就会去执行。

		{
			qDebug()<<("queue is not empty, thread dem_update is working...\n");
			
			msg_console = "queue is not empty, thread dem_update is working...\n";
			emit(sigDEMConsoleInfo(msg_console));
			
			qDebug() << "queue's size = "<<queue_infoRecv_data.size()<<endl;
			vector<update_data> update_data;
			actDet finalData;
			update_data = preProcessor();
			
			//如果preProcessor返回值为空，也就是可能存在的角速度过大的情况下
			if (update_data.empty())
			{
				continue;
			}
			
			for (int i = 0; i < update_data.size(); i++)
			{	
				actDet2latlon(update_data[i]); //处理完后通过信号量发送出去
			}
		}
		else //如果没有，则进行下一轮循环
		{
			continue;
		}

	}
}



//发送数据库的信息给显示界面
void DEMUpdate::sltSendDBinfo(QString msg_console)
{
	emit sigDEMConsoleInfo(msg_console);
}



//当前点与前一个点之间的马氏距离计算
actDet DEMUpdate::mahaDistance(actDet msg, double xmean, double ymean, double *density)
{

	//搜集msg队列中和马氏距离计算有关的值
	vector<mahalanobisDistance> data;
	mahalanobisDistance tmp;
	double MDistance(-1);//-1是为了让他区别于正常的马氏距离，让初始值特殊一点

	double covxy = (msg.lon_left - xmean)*(msg.lat_top - ymean)/100;
	double x_use = msg.lat_top / 10;
	double y_use = msg.lon_right / 10;

	

	/*
	MatrixXd m(2, 1), mt(1, 2);
	vector<double> Xarr, Yarr;
	double sumX = 0;
	double sumY = 0;


	//计算协方差
	double covxx = 0;	
	covxx = (msg.lon_left - xmean)*(msg.lon_left - xmean);
	
	double covxy = 0;
	covxy = (msg.lon_left - xmean)*(msg.lat_top - ymean);
	
	//double covyy = 0;
	//covyy = (msg.lat_top - ymean)*(msg.lat_top - ymean);
	

	MatrixXd cov(2, 2);
	//double length = data.size() - 1;
	cov << covxx , covxy , covxy , covyy ;

	//应该要计算每两点之间的马氏距离
	MatrixXd mm(2, 1);
	double para1, para2;
	*/
	

	//******************************添加马氏距离点*************************************
	//第一个值，前面为空就需要先添加当下值
	if(myDB->tableIsEmplty(QString("%1").arg(msg.serial), "mahala") == true)
	{
		//myDB->addMember_maha(msg, MDistance);
		//emit sigAdd2DB_mahala(msg, MDistance);//将数据添加到马氏距离的表中
		myDB->addMember_mahanew(msg, MDistance);
	}

	else
	{
		tmp = myDB->getNewest_mahalanobis(QString("%1").arg(msg.serial));
		/*
		para1 = msg.lon_left - tmp.lon_left;
		para2 = msg.lat_top - tmp.lat_top;
		mm << para1, para2;
		MatrixXd temp(0, 0);
		temp = mm.transpose()*cov.inverse()*mm;
		MDistance = sqrt(sqrt(temp(0, 0)));
		*/
		//简化后的马氏距离
		MDistance = 2 * covxy*covxy*x_use*y_use;
		double maha_mean(0);
		
		maha_mean = myDB->calcMean(QString("%1").arg(msg.serial), "mahala", 10, 7);
		if(maha_mean != -1)
		{
			qDebug() << "Mdistance is working Mdistance = " + QString::number(MDistance) << endl;
			qDebug() << "maha_mean is working maha_mean = " + QString::number(maha_mean) << endl;
			double val = (MDistance / maha_mean)-1; //为了模拟标准正态分布
			//生成一个sigma与sd都为1的正态分布曲线
			//boost::math::normal_distribution<> norm(1, 1);
			//density = boost::math::pdf(norm, val);
			*density = normal_distribution(val);
			//如果达到截断概率密度就截断，并且返回全0值
			if(*density <= TRUNCTED_DENSITY)
			{
				actDet ret;
				ret.lon_left = 0;
				ret.lat_top = 0;
				ret.serial = 0;
				ret.hgt = 0;
				return ret;
			}

			
		}

		//MDistance要输入到数据库中

		//emit sigAdd2DB_mahala(msg, MDistance);//将数据添加到马氏距离的表中
		//myDB->addMember_maha(msg, MDistance);
		myDB->addMember_mahanew(msg, MDistance);

	}

	return msg;
	
}






/*
时间：2022/3/15
创建人：吴旭峰
低通滤波器
Parameters:
actDet msg: 输入的主动探测值
omega：低通滤波权重  (0,1)
*/
actDet DEMUpdate::lowpassFilter(actDet msg, float omega_lat, float omega_lon, float omega_hgt)
{
	
	//如果确实是空,就先将第一个直接添加进表
	if(myDB->tableIsEmplty(QString("%1").arg(msg.serial), "lowpass") == 1)
	{
		emit sigAdd2DB_lowpass(msg);
		return msg;
	}
	else
	{
		actDet tmp;//暂时储存值
		//从数据库中取出的该序号的第k-1个值，当前值msg为第k个值
		actDet last = myDB->getNewest_lowpass(QString("%1").arg(msg.serial));
		tmp.hgt = (1. - omega_hgt)*last.hgt + omega_hgt*msg.hgt;
		tmp.lat_bottom = (1. - omega_lat)*last.lat_bottom + omega_lat*msg.lat_bottom;
		tmp.lat_top = (1. - omega_lat)*last.lat_top + omega_lat*msg.lat_top;
		tmp.lon_left = (1. - omega_lon)*last.lon_left + omega_lat*msg.lon_left;
		tmp.lon_right = (1. - omega_lon)*last.lon_right + omega_lat*msg.lon_right;
		tmp.serial = msg.serial;
		tmp.id = msg.id;

		emit sigAdd2DB_lowpass(tmp);//发送给addmember_lowpass函数，将tmp值更新入lowpass表中，作为下一次运行时的k-1个值

		return tmp;
	}
	

	
}





double DEMUpdate::normal_distribution(double x)
{

	qDebug() << "normal distribution is working x =" + QString::number(x);
	// constants
	double a1 = 0.254829592;
	double a2 = -0.284496736;
	double a3 = 1.421413741;
	double a4 = -1.453152027;
	double a5 = 1.061405429;
	double p = 0.3275911;

	// Save the sign of x
	int sign = 1;
	if (x < 0)
		sign = -1;
	x = fabs(x) / sqrt(2.0);

	// A&S formula 7.1.26
	double t = 1.0 / (1.0 + p*x);
	double y = 1.0 - (((((a5*t + a4)*t) + a3)*t + a2)*t + a1)*t*exp(-x*x);

	return 0.5*(1.0 + sign*y);

}



vector<double> DEMUpdate::forgotten_index(int len)
{
	vector<double> ret;

		for (int i = 0; i < len;i++)
		{
			ret.push_back(2*(i + 1) / len);
		}
			
		return ret;
}

