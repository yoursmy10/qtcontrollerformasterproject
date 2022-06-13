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
	myDB->deleteLater(); //ɾ��database
	QMutexLocker m_lock(&m_mutex);

}





/*
ʱ�䣺2021/12/14
�����ˣ������
������̽��õ������ݽ��м���/����������̽�����ݽ����ĵ�һ��
*/
void DEMUpdate::actDet2latlon(update_data updateData)
{
	//����õ���Χ���꼰���к�
	double lat_temp = updateData.lat + updateData.dr * cos(updateData.theta*PI / 180) * ref_lat;
	//theta������ڵ�������ϵ��˵�ģ�����˵�Ǹ��ݾ��ȷ�����״﷽��ļн�
	double lon_temp = updateData.lon + updateData.dr * sin(updateData.theta*PI / 180) * ref_lon;
	double delta_lat = updateData.dw / 2 * ref_lat;
	double delta_lon = updateData.dw / 2 * ref_lon;  //����ȡ�˽��ƣ�Ĭ�Ͻ�����ȽϷ�
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

	actDet tmp_actDet; //������и�ֵ
	tmp_actDet.lat_bottom = lat_bottom;
	tmp_actDet.lat_top = lat_top;
	tmp_actDet.serial = updateData.serial;
	tmp_actDet.lon_left = lon_left;
	tmp_actDet.lon_right = lon_right;
	tmp_actDet.hgt = hgt_temp;
	tmp_actDet.id = stu_id;
	stu_id++;


	emit sigAdd2DB(tmp_actDet);//��ӵ���student���У�����δ�����������ԭʼֵ

	//���ڼ������Ͼ�����Ҫ��Э�������
	double latmean = myDB->calcMean(QString::number(tmp_actDet.serial), "student", 10, 2);
	double lonmean = myDB->calcMean(QString::number(tmp_actDet.serial), "student", 10, 3);
	double density = 0.5;
	//���㵱ǰֵ��ǰһ��ֵ֮������Ͼ���,ֻ�е�student��������10��ֵʱ�ſ�ʼ����
	if (latmean != -1 && lonmean != -1)
	{
		tmp_actDet = mahaDistance(tmp_actDet, lonmean, latmean, &density);
		//�������ֵΪȫ0����ô��ֹͣ��������
		if(tmp_actDet.serial ==0 && tmp_actDet.hgt ==0)
		{
			qDebug() << "activate TRUNCTED_DENSITY!!!" << endl;
			qDebug() << "density = " << density << endl;
			emit sigDEMConsoleInfo("density = " + QString::number(density));//���͵�����̨��ʾ
			return;
		}
		
		//density = density / 2;
		
		qDebug() << "density = " << density << endl;
		emit sigDEMConsoleInfo("density = " + QString::number(density));//���͵�����̨��ʾ
		
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
ʱ�䣺2021/10/28
�����ˣ������
���ڸ��µ��θ߳����ݿ⣬
query���ϰ�����š����Ӿ���
dr����Ծ��롣
theta����λ�ǡ����ȡ�
dw���ϰ�������Ӿ���
hc���ϰ����������������ĵ�ĸ߶ȡ����Ӿ���
lat�������ͼƬʱֱ����γ�ȡ�
lon�������ͼƬʱֱ�������ȡ�
alt�������ͼƬʱֱ�������ߵ�ߡ�
dem_base�����θ߳����ݿ�
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
	double delta_lon = abs((lon_right - lon_left) / 2 * ref_lon);//����ȡ�˽��ƣ�Ĭ�Ͻ�����ȽϷ�
	double hgt = rec.hgt;

	//��ʾ��������ϰ������귶Χ
	//res_display = rec;  //�����������ʾ��
	emit sigFinalResultDisplay(rec);//�����ź����ڸ����߳���ʾ
	
	#if 0
	printf("serial = %d \n", serial);
	printf("lat_bottom = %f \n", lat_bottom);
	printf("lon_right =  %f \n", lon_right);
	printf("lat_top = %f \n", lat_top);
	printf("lon_left = %f \n", lon_left);
	#endif


	//m_mutex.lock();
	//��ͼ����ֻҪ֪�����Ͻ�[0][0]��ľ�γ�ȣ�֪��ÿһ��֮���ֵ�����(least_distance)���Ҿ��ܹ�ֱ�Ӽ�������ʵ�����
	double lat_init = dem_init[0][0].lat;  //���ݿ����ϽǾ���
	double lon_init = dem_init[0][0].lon;  //���ݿ����Ͻ�γ��
	
										   //int rows = dem_base.size();
										   //int rows = 0; //��
										   //int rows = sizeof(dem_init)/sizeof(dem_init[0]);
	int rows = MAX_LEN;
	//int cols = dem_base[0].size();  //�е�����
	//int cols = 0;  //��
	//int cols = sizeof(dem_init)/sizeof(dem_init[0][0]);
	int cols = MAX_LEN;
	//�ж������Ƿ���磬���»�û�ж�
#if 0
	if (lat_bottom < lat_init && lon_right > lon_init)
	{
		printf("out of range ! \n");
		return;
	}
#endif


	int m(0), n(0);  //����
	m = abs(floor((lat_bottom - lat_init) / least_distance*ref_lat));  //�õ����ϵ�������
	n = abs(floor((lon_init - lon_right) / least_distance*ref_lon));   //�õ����ϵ������

	//�������½�,�����ǲ����Ƶġ��߽�������ǵò�ȫ�����ٻ������������һ�������½ǳ������ݿ����Ͻǣ�һ�������Ͻ��Ƿ�������ݿ����½�
	int len_col(0), len_row(0);
	len_row = ceil(delta_lat / least_distance*ref_lat);  //ȡ��
	len_col = ceil(delta_lon / least_distance*ref_lon);

	//���£����µ�ԭ����ǰ��ո������������������lat,lon���ǵ����ڸ����У����������ӵĸ̸߳���Ϊ������ĸ̣߳�����һ�����࣬������������������д��۲죬������ǲ���Ҫ���أ�
	//�ɲ����Կ���һЩ�߶�һ����㷨����������¸���׼ȷ�������Բ�����ģ�
	
#if 0
	if (m + len_row > rows || n + len_col > cols)
	{
		qDebug() << ("���½ǳ�����Χ����ֻ���°����ڸ߳�ͼ������Ϣ \n");
		for (int i = m; i < rows; i++)
		{
			for (int j = n; j < cols; j++)
			{
				dem_init[i][j].hgt = hgt;
				cout << "����Ϊ(" << dem_init[i][j].lat << "," << dem_init[i][j].lon << ")�ĵ�, ��̱߳�����Ϊ" << dem_init[i][j].hgt << endl;
			}
		}
		qDebug() << ("���Ϊ %d ���ϰ����ڸ߳�ͼ�и������ \n", serial);
	}
#endif


	for (int i = m; i <= m + len_row; i++)  //���ٽ������
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
time��2021/11/5
creator�������
ģ���״ﴦ������
������ʱֻ������ƽ����һ�������Ӧ�÷��ص���vector<update_data>update_data,Ȼ��һ��һ�����뵽���ݿ�����㷨��.
���������Ӧ�ý�ģ���״ﺯ����Ӧ�ý�Ԥ������
*/
vector<update_data> DEMUpdate::preProcessor()
{
	qDebug() << "preProcessor is working..." << endl;
	
	QString msg_console = "preProcessor is working...\n";
	emit sigDEMConsoleInfo(msg_console);
	
	vector<info_recv> recv_tmp;  //���մ�ͨ���̴߳�����������
	vector<update_data> update_data_ret = {}; //Ҫ���뵽�����㷨�е�����dc

	//default_random_engine r;
	//uniform_real_distribution<double> distance_rand(50.0, 260.0); //�������
	//uniform_real_distribution<double> theta_rand(0, 45.0); //����Ƕ�
	//uniform_real_distribution<double> alt_rand(100.0, 110.0); //����߶ȣ����ȶ�

	//����������û�м������������⣬��Ϊ��ֻ��һ������,������ݶ������г���һ�����ݣ���Ҫ�Ȱ�ǰ���ȥ���������µ�
	while (queue_sensor_data.size() > 1)
	{
		queue_sensor_data.wait_and_pop();
	}
	SENSOR_DATA tmp_sensor;
	queue_sensor_data.wait_and_pop(tmp_sensor);
	//������������½���

	//������ٶȹ��󣬾�ֹͣ����
	if (tmp_sensor.angular_vel_z > MAX_ANGULAR_VEL || tmp_sensor.angular_vel_y > MAX_ANGULAR_VEL || tmp_sensor.angular_vel_z > MAX_ANGULAR_VEL)
	{
		qDebug() << "angular_vel out of range!" << endl;
		msg_console = "angular_vel out of range!\n";//���ٶȹ�������		
		emit sigDEMConsoleInfo(msg_console);//����������
		
		queue_infoRecv_data.clear();
		queue_sensor_data.clear();
		return update_data_ret;
	}

															  
	int len = queue_infoRecv_data.size();//��ȡ���е�size��Ҳ�����м������ݵ�����
	//��ͨ�ź��������Ķ����н�����,�����߼���˵�����һ��ͼ�е����������Ӿ�̽�⵽���ϰ���
	for (int i = 0; i<len; i++)
	{
		//Ӧ������һ���ṹ������,���Ҳ��һ�����ɽṹ��
		//recv_tmp.push_back(queue_infoRecv_data->wait_and_pop()); //�ýṹ��Ӷ��е�����
		info_recv tmp;
		queue_infoRecv_data.wait_and_pop(tmp);  //�Ѷ����е�ֵ��ֵ��recv_tmp[i]
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



	//�ȰѲ���random�������Ȳ����״������ģʽ,Ҫע��һЩ����֮����߼���ϵ
	//��γ������γ�Ǹ�����γ���������������������Ǹ���
	for (int i = 0; i<len; i++)
	{
		update_data updateData;
		double judge = (max(recv_tmp[i].y_top, recv_tmp[i].y_bottom) - IMG_HEIGHT / 2);

		//double judge = 1;
		//����ϰ��ﻹ����ͼƬ�����ĵ�ߣ�����dr����Զ�״�����࣬Ҳ����˵����̽�⵽������Զ�ĵط�����ô�ж��������������ǲ�����ײ�ģ��ҾͲ���Ҫȥ���������������񸺵�
		if (judge <= 0 || abs(recv_tmp[i].distance - MAX_RADAR_LEN) <= ERROR_BETWEEN_RADARLEN_DR) 
		{
			continue;
		}

		double pix2meters = abs(recv_tmp[i].distance*sin(recv_tmp[i].yaw_angle*PI/180) / abs((recv_tmp[i].x_right + recv_tmp[i].x_left)/2 - IMG_WIDTH/2));

		//updateData.dr = distance_rand(r); //random��
		updateData.dr = recv_tmp[i].distance;
		//updateData.theta = theta_rand(r); //random��
		updateData.theta = tmp_sensor.heli_yaw + recv_tmp[i].yaw_angle;//ֱ������λ�Ǽ��ϴ�������λ��
		updateData.dw = (recv_tmp[i].x_right - recv_tmp[i].x_left)*pix2meters;
		updateData.alt = tmp_sensor.alt_gps; //GPS��ֵ
		updateData.serial = recv_tmp[i].serial;
		updateData.lat = abs(tmp_sensor.lat); //GPS��ֵ
		updateData.lon = abs(tmp_sensor.lon); //GPS��ֵ
		//
		//20220322��д����hgt�㷨
		//��ʾ�ϰ�����������һ�ỹ��ͼ����ʾ����������ͼ��,��һ���ԣ���д��
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

	//����Ͱ���Ҫ�����ݶ���ȡ���ˣ���������һ��ͼ������̽�⵽���ϰ�������ݣ���һ���ṹ�����飬Ȼ��һ��һ�Ŵ������θ����㷨��ȥ
	emit sigDEMConsoleInfo("PreProcessor done...");
	myDB->addMember_helistate(tmp_sensor);//��ֱ��������״̬��¼������
	emit sig2plot(tmp_sensor); //��Ҫ�ǽ�ֱ������̬��λ�ô�����ͼ����
	return update_data_ret;
}



/*
ʱ�䣺2021/11/06
�����ˣ������
��ʼ�����ɸ߳�ͼ
lat_init : ԭ�㾭��
lon_init : ԭ��γ��
least_distance : ����������
MAX_LEN / least_distance��ȡֵȡ�������븲�Ƕ������򣬲�����
Ĭ�ϳ�ʼ����ͼ�м�
*/
//TODO ������ר�ŵ�ϡ�������д洢������������ռ�ڴ�
void DEMUpdate::init_dem(double lat_init, double lon_init, double least_distance)
{
	for (int i = 0; i < MAX_LEN; i++)//i for lat, j for lon,����Ĭ�ϵ�ͼ������Ĭ��ԭ���������½ǡ�
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
ʱ�䣺2021/12/15
�����ˣ������
�������ݿ⴫�����Ĵﵽ��ֵ�����У�����õ�һ�������ֵ�����뵽���θ����㷨��,�����ڵ��㷨���ǰѱ��У�ÿ5��ƽ��ֵ���һ�飬Ȼ��ÿ��5��֮���ٰ��ܺ��������һ�飬��һ��ƽ������
*/
void DEMUpdate::calculator(vector<actDet> msg)
{
	qDebug() << "calculator is working ..." << endl;
	//QString msg_console = "calculator is working ...";
	emit sigDEMConsoleInfo("calculator is working ...");

	//msg = mahaDistance(msg); //��msg�������Ͼ�����㣬�޳���Ⱥ��
	
	int len = msg.size(); //����ĳ���
	actDet res;//����ֵ


	//����һ���ж�������ĳ���Ƿ�Ϊ��
	/*
	for (int i = 0; i < len;i++)
	{
		res = lowpassFilter(msg[i], 0.5, 0.5, 0.5);
		//��һ�����ǹ������ݣ�����ﵽ��������ֵ��ô���ֵ���ڸ��µ����θ߳�ͼ��ʱ��һֱ��һ����ֵ,���������������������θ����㷨��������ӵ�finalotput����
		res = myDB->calcMeanandCov(res);
		myDB->addMember_final(res);
	}
	*/
	//�õ����һ���res


	




	///////////////////////////////////////////�������ϵ��220327
	///
	vector<double>forgottenIndex = forgotten_index(len);

	
	//�����ƽ��ֵ�պϰ�
	vector<double> lat_top_list;
	vector<double> lon_left_list;
	vector<double> hgt_list;
	vector<double> lat_bottom_list;
	vector<double> lon_right_list;

/*
	for (int i = 0; i < len; i++)
	{
		//ÿ��ֵ�ڽ������Ǿ��ȳ�������ϵ��
		lat_top_list.push_back((msg[i].lat_top)*forgottenIndex[i]);
		lat_bottom_list.push_back((msg[i].lat_bottom)*forgottenIndex[i]);
		lon_left_list.push_back((msg[i].lon_left)*forgottenIndex[i]);
		lon_right_list.push_back((msg[i].lon_right)*forgottenIndex[i]);
		hgt_list.push_back((msg[i].hgt)*forgottenIndex[i]);
	}
*/


	for (int i = 0; i < len; i++)
	{
		//ÿ��ֵ�ڽ������Ǿ��ȳ�������ϵ��
		lat_top_list.push_back((msg[i].lat_top));
		lat_bottom_list.push_back((msg[i].lat_bottom));
		lon_left_list.push_back((msg[i].lon_left));
		lon_right_list.push_back((msg[i].lon_right));
		hgt_list.push_back((msg[i].hgt));
	}
	
	//���ֵ,���ڿ϶�Ҫ���������취
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

	//�½�����ֵ
	
	res.hgt = hgt;
	res.lat_bottom = lat_bottom;
	res.lat_top = lat_top;
	res.lon_left = lon_left;
	res.lon_right = lon_right;
	res.serial = msg[0].serial;
	res.id = msg[0].id;  //������

	myDB->addMember_display(res);

	
	emit sigDEMUpdate(res); //�����źţ��õ��θ����㷨������ͬʱҲ�����ݷ��͵���άͼ�и����ϰ���λ����Ϣ
	emit sigFinalResultDisplay(res);//�ﵽ��ֵ������
	emit sigDEMConsoleInfo("Calculating Finished...");
	//return res;

}


//��ʼ�����θ����߳�
void DEMUpdate::initDEMupdate()
{
	myDB = new DataBase();
	myDB->sltRunDB();
	
	//TODO ��ʼ�����ݿ⣬����run����
	connect(this, SIGNAL(sigAdd2DB(actDet)), myDB, SLOT(addMember(actDet)));  //act2latlon-->addmember
	connect(myDB, SIGNAL(startGet(QString)), myDB, SLOT(getClass(QString)));  //addmember --> getclass
	connect(myDB, SIGNAL(sigStartDEMUP(vector<actDet>)), this, SLOT(calculator(vector<actDet>))); //getclass-->calculator
	connect(this, SIGNAL(sigDEMUpdate(actDet)), this ,SLOT(database_update(actDet))); //calculator --> database_update
	connect(myDB, SIGNAL(sigDBinfo(QString)), this, SLOT(sltSendDBinfo(QString)));//�������ݿ����Ϣ����ʾ����


	//��������������calculator��ʵ�ֵ�
	connect(this, SIGNAL(sigAdd2DB_lowpass(actDet)), myDB, SLOT(addMember_lowpass(actDet)));  //lowpassfilter-->addmember_lowpass
	connect(this, SIGNAL(sigAdd2DB_mahala(actDet, double)), myDB, SLOT(addMember_maha(actDet, double)));//mahaladistance-->addmember_maha
	
	
	init_dem(lat_init,lon_init,least_distance); //��ȫ�ֱ�����ʼ�����θ߳�ͼ

	qDebug() << "task_dem_update initiated successfully" << endl;
	QString msg_console = "task_dem_update initiated successfully\n";
	emit(sigDEMConsoleInfo(msg_console));
	
	while (1) //ѭ�����������ź����������������ж�һ�¶����Ƿ�Ϊ�գ���
	{
		if (!queue_infoRecv_data.empty())//������зǿգ���Ե��θ߳�ͼִ�и��²���. 2021/11/08/: ��˫�߳����������������⣬�Ǿ��Ǿ������ֻ������һ�����������ж�Ҳͨ���ˣ��ͻ�ȥִ�С�

		{
			qDebug()<<("queue is not empty, thread dem_update is working...\n");
			
			msg_console = "queue is not empty, thread dem_update is working...\n";
			emit(sigDEMConsoleInfo(msg_console));
			
			qDebug() << "queue's size = "<<queue_infoRecv_data.size()<<endl;
			vector<update_data> update_data;
			actDet finalData;
			update_data = preProcessor();
			
			//���preProcessor����ֵΪ�գ�Ҳ���ǿ��ܴ��ڵĽ��ٶȹ���������
			if (update_data.empty())
			{
				continue;
			}
			
			for (int i = 0; i < update_data.size(); i++)
			{	
				actDet2latlon(update_data[i]); //�������ͨ���ź������ͳ�ȥ
			}
		}
		else //���û�У��������һ��ѭ��
		{
			continue;
		}

	}
}



//�������ݿ����Ϣ����ʾ����
void DEMUpdate::sltSendDBinfo(QString msg_console)
{
	emit sigDEMConsoleInfo(msg_console);
}



//��ǰ����ǰһ����֮������Ͼ������
actDet DEMUpdate::mahaDistance(actDet msg, double xmean, double ymean, double *density)
{

	//�Ѽ�msg�����к����Ͼ�������йص�ֵ
	vector<mahalanobisDistance> data;
	mahalanobisDistance tmp;
	double MDistance(-1);//-1��Ϊ���������������������Ͼ��룬�ó�ʼֵ����һ��

	double covxy = (msg.lon_left - xmean)*(msg.lat_top - ymean)/100;
	double x_use = msg.lat_top / 10;
	double y_use = msg.lon_right / 10;

	

	/*
	MatrixXd m(2, 1), mt(1, 2);
	vector<double> Xarr, Yarr;
	double sumX = 0;
	double sumY = 0;


	//����Э����
	double covxx = 0;	
	covxx = (msg.lon_left - xmean)*(msg.lon_left - xmean);
	
	double covxy = 0;
	covxy = (msg.lon_left - xmean)*(msg.lat_top - ymean);
	
	//double covyy = 0;
	//covyy = (msg.lat_top - ymean)*(msg.lat_top - ymean);
	

	MatrixXd cov(2, 2);
	//double length = data.size() - 1;
	cov << covxx , covxy , covxy , covyy ;

	//Ӧ��Ҫ����ÿ����֮������Ͼ���
	MatrixXd mm(2, 1);
	double para1, para2;
	*/
	

	//******************************������Ͼ����*************************************
	//��һ��ֵ��ǰ��Ϊ�վ���Ҫ����ӵ���ֵ
	if(myDB->tableIsEmplty(QString("%1").arg(msg.serial), "mahala") == true)
	{
		//myDB->addMember_maha(msg, MDistance);
		//emit sigAdd2DB_mahala(msg, MDistance);//��������ӵ����Ͼ���ı���
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
		//�򻯺�����Ͼ���
		MDistance = 2 * covxy*covxy*x_use*y_use;
		double maha_mean(0);
		
		maha_mean = myDB->calcMean(QString("%1").arg(msg.serial), "mahala", 10, 7);
		if(maha_mean != -1)
		{
			qDebug() << "Mdistance is working Mdistance = " + QString::number(MDistance) << endl;
			qDebug() << "maha_mean is working maha_mean = " + QString::number(maha_mean) << endl;
			double val = (MDistance / maha_mean)-1; //Ϊ��ģ���׼��̬�ֲ�
			//����һ��sigma��sd��Ϊ1����̬�ֲ�����
			//boost::math::normal_distribution<> norm(1, 1);
			//density = boost::math::pdf(norm, val);
			*density = normal_distribution(val);
			//����ﵽ�ضϸ����ܶȾͽضϣ����ҷ���ȫ0ֵ
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

		//MDistanceҪ���뵽���ݿ���

		//emit sigAdd2DB_mahala(msg, MDistance);//��������ӵ����Ͼ���ı���
		//myDB->addMember_maha(msg, MDistance);
		myDB->addMember_mahanew(msg, MDistance);

	}

	return msg;
	
}






/*
ʱ�䣺2022/3/15
�����ˣ������
��ͨ�˲���
Parameters:
actDet msg: ���������̽��ֵ
omega����ͨ�˲�Ȩ��  (0,1)
*/
actDet DEMUpdate::lowpassFilter(actDet msg, float omega_lat, float omega_lon, float omega_hgt)
{
	
	//���ȷʵ�ǿ�,���Ƚ���һ��ֱ����ӽ���
	if(myDB->tableIsEmplty(QString("%1").arg(msg.serial), "lowpass") == 1)
	{
		emit sigAdd2DB_lowpass(msg);
		return msg;
	}
	else
	{
		actDet tmp;//��ʱ����ֵ
		//�����ݿ���ȡ���ĸ���ŵĵ�k-1��ֵ����ǰֵmsgΪ��k��ֵ
		actDet last = myDB->getNewest_lowpass(QString("%1").arg(msg.serial));
		tmp.hgt = (1. - omega_hgt)*last.hgt + omega_hgt*msg.hgt;
		tmp.lat_bottom = (1. - omega_lat)*last.lat_bottom + omega_lat*msg.lat_bottom;
		tmp.lat_top = (1. - omega_lat)*last.lat_top + omega_lat*msg.lat_top;
		tmp.lon_left = (1. - omega_lon)*last.lon_left + omega_lat*msg.lon_left;
		tmp.lon_right = (1. - omega_lon)*last.lon_right + omega_lat*msg.lon_right;
		tmp.serial = msg.serial;
		tmp.id = msg.id;

		emit sigAdd2DB_lowpass(tmp);//���͸�addmember_lowpass��������tmpֵ������lowpass���У���Ϊ��һ������ʱ��k-1��ֵ

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

