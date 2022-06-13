
#include<communication.h>
#pragma comment (lib, "ws2_32.lib")  //���� ws2_32.dll


using namespace std;



Communication::Communication(QObject *parent) :
	QObject(parent)
{

}


Communication::~Communication()
{
	if (sock_std != NULL)
	{
		sock_std = NULL;
	}
	memset(recvMsg, 0, sizeof(recvMsg));  //���recvMsg
}


void Communication::sltDataAnalysis(QByteArray recvMsg)
{
	//Ҫ�Ȱ����е�һЩ���������
	
	QList<QByteArray> msgList = recvMsg.split(';');
	//msgList = recvMsg.split(';');


	vector<info_recv> InfoRecv;  //һ���ṹ������,ÿ��������ո���һ����
	int space_count = 0;//�����ƿո���
	int order = 0; //��ǰ�ǵڼ���struct
	vector<float> store_temp; //�����洢����������
	int len;//����������г���
	int tmp = 0; //������������ʱ����
	queue<int> q; //���ö��д�����ʱ��,FIFO
	
	//string buf = recvMsg.data();
	string buf = msgList[0].data();



//*************************************�Զ��н��з���********************************************
	
#if 0
	for (int i = 0; i<msgList[0].size(); i++)  //���һλ������û�пո�,����ô�����,�ڷ��Ͷ��ټ�һ���ո�
	{
		if (buf[i] == ' ')//������Ͽո񣬾ʹ����������ҲҪ��ֹ���ֺü����ո�����������Ҫ
		{
			if (!q.empty())  //������в�Ϊ�գ��ͽ������е����������ϴ���store_tmp����
			{
				len = q.size();//ȷ�������Ǽ�λ��
				for (auto i = len; i>0; i--)
				{
					tmp += q.front() * pow(10, i - 1); //�������ǵڼ�λȻ�����
					q.pop();
				}
				store_temp.push_back(tmp);
				tmp = 0;
			}
		}
		else  //���û�����Ͽո񣬾ͽ���ѹ�����
		{
			q.push(buf[i] - 48);
		}
	}
#endif
	int neg = 1; //�����ŷ����ж�
	for (int i = 0; i<msgList[0].size(); i++)
	{
		if (buf[i] == '-')//�������ŵ����
		{
			neg = -1;  //����������žͱ�-1.������������֮�������ֱ��1
			continue;
		}
		if (buf[i] == '.')//�������.���ʹ��������
		{
			if (!q.empty())  //������в�Ϊ�գ��ͽ������е����������ϴ�������
			{
				len = q.size();//ȷ�������Ǽ�λ��
				for (int i = len; i>0; i--)
				{
					tmp += q.front() * pow(10, i - 1); //�������ǵڼ�λȻ�����
					q.pop();
				}
				tmp = neg*tmp;
				neg = 1;
				store_temp.push_back(tmp);
				tmp = 0;
				i = i + 2; //���ո�
			}
		}
		else  //���û�����Ͽո񣬾ͽ���ѹ�����
		{
			q.push(buf[i] - 48);
		}
	}



	
	//test,��ӡһ�µõ���store_tmp����
	int len_tmp = store_temp.size();
	qDebug() << "len_tmp = " << len_tmp << endl;
	/*
	for(int i=0;i<len_tmp;i++)
	{
	cout<<"store_temp ["<<i<<"]= "<<store_temp[i]<<endl;
	}
	*/

	/************************����������к�ר�Ĵ������У�ÿ5����һ�鴫��ṹ��****************************/
	//�õ���һ�����У�vector<int> store_temp;  ÿ5��һ�����
	int count = (len_tmp + 1) / LEN_RECV_IMG; //һ��������
	int count_tmp = 0;
	while (order<count)
	{       //x1<x2;y1<y2.����,����

		//2021/12/29�����ش����⣬��ԭ��Ԥ��������᲻ͬ��ԭ��Ԥ�����y�����£�x�����ҡ����������Է���������Ӧ���ǣ�y�����ң�x�����£������ڸ�ֵ����Ҫת����������ͼ������ϵ���Լ����������ϵת
		info_recv info_tmp = {};
		info_tmp.x_left = int(store_temp[count_tmp+1]);
		info_tmp.y_bottom = int(store_temp[count_tmp]);
		info_tmp.x_right = int(store_temp[count_tmp + 3]);
		info_tmp.y_top = int(store_temp[count_tmp + 2]);
		info_tmp.serial = int(store_temp[count_tmp + 4]);
		info_tmp.distance = store_temp[count_tmp + 5]*1.0/1000000.0;  //ǿ������ת��
		info_tmp.yaw_angle = store_temp[count_tmp + 6]*1.0/1000.0;
		
		InfoRecv.push_back(info_tmp);

		order++;
		count_tmp = (count_tmp + LEN_RECV_IMG);
	}
	count_tmp = 0;
	/*
	if(queue_infoRecv_data->IsEmpty()) //�������Ϊ����ѹ�룬����ÿ�ξ��ܴ�һ�������ģ�����Ҫ��ģ���״ﺯ���н��д�����ա��о�Ҳ�е����⣬��������ѹ�����ݵ�ʱ�򱻵������أ�
	{
	for(int i=0;i<order;i++)
	{
	queue_infoRecv_data->Push(InfoRecv[i]);//���ýṹ��ѹ����У�ţ��
	}
	}
	*/
	if (queue_infoRecv_data.empty())
	{
		for (int i = 0; i<order; i++)
		{
			queue_infoRecv_data.push(InfoRecv[i]);
		}
	}

	//test
	/*
	for(int i=0;i<order;i++)
	{
	cout<<"InfoRecv->serial =  "<<InfoRecv[i].serial<<endl;
	}
	*/
	//�ͷ�store_temp
	store_temp.clear();

	sltSensorDataAnalysis(msgList[1]); //����Ǵ���������������

	return;
}

void Communication::sltSensorDataAnalysis(QByteArray recvMsg)
{
	SENSOR_DATA sensor_data;
	int space_count = 0;//�����ƿո���
	vector<double> store_temp; //�����洢����������
	int len;//����������г���
	int tmp = 0; //������������ʱ����
	queue<int> q; //���ö��д�����ʱ��,FIFO
	int neg = 1; //�����ŷ����ж�
	string buf = recvMsg.data();

	for (int i = 0; i<recvMsg.size(); i++)
	{
		if (buf[i] == '-')//�������ŵ����
		{
			neg = -1;  //����������žͱ�-1.������������֮�������ֱ��1
			continue;
		}
		if (buf[i] == '.')//�������.���ʹ��������
		{
			if (!q.empty())  //������в�Ϊ�գ��ͽ������е����������ϴ�������
			{
				len = q.size();//ȷ�������Ǽ�λ��
				for (int i = len; i>0; i--)
				{
					tmp += q.front() * pow(10, i - 1); //�������ǵڼ�λȻ�����
					q.pop();
				}
				tmp = neg*tmp;
				neg = 1;
				store_temp.push_back(tmp);
				tmp = 0;
				i = i + 2; //���ո�
			}
		}
		else  //���û�����Ͽո񣬾ͽ���ѹ�����
		{
			q.push(buf[i] - 48);
		}
	}
	//test
	/*
	for(int i=0;i<9;i++)
	{
	cout<<store_temp[i]<<endl;
	}
	*/

	len = store_temp.size();
	//��������˳����
	sensor_data.alt_gps = float(store_temp[0] * 1.0 / 1000000.0);
	sensor_data.lat = store_temp[1] / 1000000.0;
	sensor_data.lon = store_temp[2] / 1000000.0;
	sensor_data.velocity_x = float(store_temp[3] * 1.0 / 1000.0);
	sensor_data.velocity_y = float(store_temp[4] * 1.0 / 1000.0);
	sensor_data.velocity_z = float(store_temp[5] * 1.0 / 1000.0);
	sensor_data.angular_vel_x = float(store_temp[6] * 1.0 / 1000.0);
	sensor_data.angular_vel_y = float(store_temp[7] * 1.0 / 1000.0);
	sensor_data.angular_vel_z = float(store_temp[8] * 1.0 / 1000.0);
	//sensor_data.distance = store_temp[9] * 1.0 / 1000.0;
	sensor_data.heli_yaw = float(store_temp[9] * 1.0 / 1000.0); //�Ƕ�

	//ֱ����ˮƽ�����ٶȣ�(������ܷ�ʱ��)
	//double velocity_ori = pow(pow(sensor_data.velocity_x, 2) + pow(sensor_data.velocity_y, 2), 0.5);
	emit sigForAlert(sensor_data.lat, sensor_data.lon, sensor_data.heli_yaw, sensor_data.alt_gps, sensor_data.velocity_x, sensor_data.velocity_y); //���͸��澯�߳�
	emit sigCOMConsoleInfo("Alert Info Transmitted...");
	store_temp.clear();

	if (queue_sensor_data.empty())
	{
		queue_sensor_data.push(sensor_data);
	}

	//test,display
	/*
	cout<<"sensor_data 0="<<setprecision(10)<<sensor_data.alt_gps<<endl;
	cout<<"sensor_data 1="<<sensor_data.lat<<endl;
	cout<<"sensor_data 2="<<sensor_data.lon<<endl;
	cout<<"sensor_data 3="<<sensor_data.velocity_x<<endl;
	cout<<"sensor_data 4="<<sensor_data.velocity_y<<endl;
	cout<<"sensor_data 5="<<sensor_data.velocity_z<<endl;
	cout<<"sensor_data 6="<<sensor_data.angular_vel_x<<endl;
	cout<<"sensor_data 7="<<sensor_data.angular_vel_y<<endl;
	cout<<"sensor_data 8="<<sensor_data.angular_vel_z<<endl;
	*/
	return;
}

void Communication::sltInitCommu()
{
	// ��ʼ��DLL
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	//ʹ��QUdpSocket��������
	printf("Communication initiated successfully\n");
	QString msg_console = "Communication initiated successfully\n";
	emit sigCOMConsoleInfo(msg_console);

	qsocket = new QUdpSocket;


	//Ĭ��0Ϊ�Զ�����
#if switch_socket == 1//for self test
	serverAddress.setAddress("127.0.0.1");
	serverPort = 2233;
	QHostAddress localAddress("127.0.0.2");
	localPort = 3322;
#endif


#if switch_socket == 0//for airsim
	serverAddress.setAddress("127.0.0.2");
	serverPort = 3322;
	QHostAddress localAddress("127.0.0.1");
	localPort = 2233;
#endif

	bool flagBind = qsocket->bind(localAddress, localPort);//��
	if (!flagBind)
	{
		qWarning() << QString::fromLocal8Bit("����IP��ַ��") +
			localAddress.toString() + QString::fromLocal8Bit("��ʧ�ܡ�");
	}

	QByteArray datagram;//����
	msg_console = " Local Address:Port: 127.0.0.2:3322\n Server Address:Port: 127.0.0.1:2233\n";
	emit sigCOMConsoleInfo(msg_console);


	qDebug() << "waiting for message.... \n" << endl;
	//msg_console = "waiting for message.... \n";
	emit sigCOMConsoleInfo("CONNECTED... \n waiting for message.... ");

	/*
	���ĸ�ʽ��
	output_string = 32 13 319 493 1
	output_sensor = -680576.0 31153000.0 118234572.0 0.0 0.0 0.0 1.0 -1.0 1.0
	outputSum =16 57;32 13 319 493 1 ;-680576.0 31153000.0 118234572.0 0.0 0.0 0.0 1.0 -1.0 1.0
	*/
	qsocket->writeDatagram("halo,server", serverAddress, serverPort);
	while (1)
	{

		while (qsocket->hasPendingDatagrams())  //ֻҪ���Ĳ�Ϊ�վͽ��봦��׶�
		{
			//msg_console = "Data received...\n";
			//emit(sigCOMConsoleInfo(msg_console));  //̫ռ��Դ��

			qDebug() << datagram.data() << endl;
			datagram.resize(qsocket->pendingDatagramSize());
			QHostAddress sender;
			quint16 senderPort;
			//IMG_DATA
			qsocket->readDatagram(datagram.data(), datagram.size(), &serverAddress, &serverPort);
			sltDataAnalysis(datagram);
			qsocket->writeDatagram("halo,server_again", serverAddress, serverPort);  //������ȷ����������´η��ͽ������ı���
			//qDebug() << datagram.size() << endl;
			datagram.clear(); //��ձ��ģ�Ϊ����sensordata��׼��




	// 		while (datagram.size() == -1 || datagram.size() == 0)//ȷ�����Ĳ�Ϊ0����ʱ�ȴ���һ��籨�ĵ���
	// 		{
	// 			datagram.resize(qsocket->pendingDatagramSize());
	// 		}
	// 		
	// 		//SENSOR_DATA
	// 		qsocket->readDatagram(datagram.data(), datagram.size(), &serverAddress, &serverPort);
	// 		sltSensorDataAnalysis(datagram);
	// 		datagram.clear();
		}

	}
}

