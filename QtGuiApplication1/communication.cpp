
#include<communication.h>
#pragma comment (lib, "ws2_32.lib")  //加载 ws2_32.dll


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
	memset(recvMsg, 0, sizeof(recvMsg));  //清空recvMsg
}


void Communication::sltDataAnalysis(QByteArray recvMsg)
{
	//要先把其中的一些量剥离出来
	
	QList<QByteArray> msgList = recvMsg.split(';');
	//msgList = recvMsg.split(';');


	vector<info_recv> InfoRecv;  //一个结构体数组,每经历五个空格算一组数
	int space_count = 0;//用来计空格数
	int order = 0; //当前是第几组struct
	vector<float> store_temp; //用来存储读出来的数
	int len;//用来计算队列长度
	int tmp = 0; //计算数是做暂时储存
	queue<int> q; //利用队列储存暂时量,FIFO
	
	//string buf = recvMsg.data();
	string buf = msgList[0].data();



//*************************************对队列进行分析********************************************
	
#if 0
	for (int i = 0; i<msgList[0].size(); i++)  //最后一位数后面没有空格,该怎么解决？,在发送端再加一个空格
	{
		if (buf[i] == ' ')//如果遇上空格，就传数并清除，也要防止出现好几个空格的情况（不需要
		{
			if (!q.empty())  //如果队列不为空，就将队列中的数经过整合传入store_tmp数组
			{
				len = q.size();//确认其中是几位数
				for (auto i = len; i>0; i--)
				{
					tmp += q.front() * pow(10, i - 1); //计算数是第几位然后代入
					q.pop();
				}
				store_temp.push_back(tmp);
				tmp = 0;
			}
		}
		else  //如果没有遇上空格，就将数压入队列
		{
			q.push(buf[i] - 48);
		}
	}
#endif
	int neg = 1; //正负号符号判断
	for (int i = 0; i<msgList[0].size(); i++)
	{
		if (buf[i] == '-')//遇到负号的情况
		{
			neg = -1;  //如果遇到负号就变-1.接下来乘完了之后下面又变成1
			continue;
		}
		if (buf[i] == '.')//如果遇上.，就传数并清除
		{
			if (!q.empty())  //如果队列不为空，就将队列中的数经过整合传入数组
			{
				len = q.size();//确认其中是几位数
				for (int i = len; i>0; i--)
				{
					tmp += q.front() * pow(10, i - 1); //计算数是第几位然后代入
					q.pop();
				}
				tmp = neg*tmp;
				neg = 1;
				store_temp.push_back(tmp);
				tmp = 0;
				i = i + 2; //到空格
			}
		}
		else  //如果没有遇上空格，就将数压入队列
		{
			q.push(buf[i] - 48);
		}
	}



	
	//test,打印一下得到的store_tmp数列
	int len_tmp = store_temp.size();
	qDebug() << "len_tmp = " << len_tmp << endl;
	/*
	for(int i=0;i<len_tmp;i++)
	{
	cout<<"store_temp ["<<i<<"]= "<<store_temp[i]<<endl;
	}
	*/

	/************************获得正经数列后专心处理数列，每5个数一组传入结构体****************************/
	//得到了一个数列：vector<int> store_temp;  每5个一组进行
	int count = (len_tmp + 1) / LEN_RECV_IMG; //一共几组数
	int count_tmp = 0;
	while (order<count)
	{       //x1<x2;y1<y2.左下,右上

		//2021/12/29发现重大问题，和原来预想的坐标轴不同，原来预想的是y轴向下，x轴向右。而经过测试发现坐标轴应该是：y轴向右，x轴向下，所以在赋值上需要转换过来，把图像坐标系向自己想象的坐标系转
		info_recv info_tmp = {};
		info_tmp.x_left = int(store_temp[count_tmp+1]);
		info_tmp.y_bottom = int(store_temp[count_tmp]);
		info_tmp.x_right = int(store_temp[count_tmp + 3]);
		info_tmp.y_top = int(store_temp[count_tmp + 2]);
		info_tmp.serial = int(store_temp[count_tmp + 4]);
		info_tmp.distance = store_temp[count_tmp + 5]*1.0/1000000.0;  //强制类型转换
		info_tmp.yaw_angle = store_temp[count_tmp + 6]*1.0/1000.0;
		
		InfoRecv.push_back(info_tmp);

		order++;
		count_tmp = (count_tmp + LEN_RECV_IMG);
	}
	count_tmp = 0;
	/*
	if(queue_infoRecv_data->IsEmpty()) //如果队列为空再压入，这样每次就能传一个完整的，队列要在模拟雷达函数中进行处理并清空。感觉也有点问题，如果在这边压入数据的时候被调用了呢？
	{
	for(int i=0;i<order;i++)
	{
	queue_infoRecv_data->Push(InfoRecv[i]);//将该结构体压入队列，牛的
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
	//释放store_temp
	store_temp.clear();

	sltSensorDataAnalysis(msgList[1]); //这个是传感器传过来的量

	return;
}

void Communication::sltSensorDataAnalysis(QByteArray recvMsg)
{
	SENSOR_DATA sensor_data;
	int space_count = 0;//用来计空格数
	vector<double> store_temp; //用来存储读出来的数
	int len;//用来计算队列长度
	int tmp = 0; //计算数是做暂时储存
	queue<int> q; //利用队列储存暂时量,FIFO
	int neg = 1; //正负号符号判断
	string buf = recvMsg.data();

	for (int i = 0; i<recvMsg.size(); i++)
	{
		if (buf[i] == '-')//遇到负号的情况
		{
			neg = -1;  //如果遇到负号就变-1.接下来乘完了之后下面又变成1
			continue;
		}
		if (buf[i] == '.')//如果遇上.，就传数并清除
		{
			if (!q.empty())  //如果队列不为空，就将队列中的数经过整合传入数组
			{
				len = q.size();//确认其中是几位数
				for (int i = len; i>0; i--)
				{
					tmp += q.front() * pow(10, i - 1); //计算数是第几位然后代入
					q.pop();
				}
				tmp = neg*tmp;
				neg = 1;
				store_temp.push_back(tmp);
				tmp = 0;
				i = i + 2; //到空格
			}
		}
		else  //如果没有遇上空格，就将数压入队列
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
	//接下来按顺序来
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
	sensor_data.heli_yaw = float(store_temp[9] * 1.0 / 1000.0); //角度

	//直升机水平方向速度：(这样算很费时间)
	//double velocity_ori = pow(pow(sensor_data.velocity_x, 2) + pow(sensor_data.velocity_y, 2), 0.5);
	emit sigForAlert(sensor_data.lat, sensor_data.lon, sensor_data.heli_yaw, sensor_data.alt_gps, sensor_data.velocity_x, sensor_data.velocity_y); //发送给告警线程
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
	// 初始化DLL
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	//使用QUdpSocket进行连接
	printf("Communication initiated successfully\n");
	QString msg_console = "Communication initiated successfully\n";
	emit sigCOMConsoleInfo(msg_console);

	qsocket = new QUdpSocket;


	//默认0为自动开启
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

	bool flagBind = qsocket->bind(localAddress, localPort);//绑定
	if (!flagBind)
	{
		qWarning() << QString::fromLocal8Bit("本机IP地址：") +
			localAddress.toString() + QString::fromLocal8Bit("绑定失败。");
	}

	QByteArray datagram;//报文
	msg_console = " Local Address:Port: 127.0.0.2:3322\n Server Address:Port: 127.0.0.1:2233\n";
	emit sigCOMConsoleInfo(msg_console);


	qDebug() << "waiting for message.... \n" << endl;
	//msg_console = "waiting for message.... \n";
	emit sigCOMConsoleInfo("CONNECTED... \n waiting for message.... ");

	/*
	报文格式：
	output_string = 32 13 319 493 1
	output_sensor = -680576.0 31153000.0 118234572.0 0.0 0.0 0.0 1.0 -1.0 1.0
	outputSum =16 57;32 13 319 493 1 ;-680576.0 31153000.0 118234572.0 0.0 0.0 0.0 1.0 -1.0 1.0
	*/
	qsocket->writeDatagram("halo,server", serverAddress, serverPort);
	while (1)
	{

		while (qsocket->hasPendingDatagrams())  //只要报文不为空就进入处理阶段
		{
			//msg_console = "Data received...\n";
			//emit(sigCOMConsoleInfo(msg_console));  //太占资源了

			qDebug() << datagram.data() << endl;
			datagram.resize(qsocket->pendingDatagramSize());
			QHostAddress sender;
			quint16 senderPort;
			//IMG_DATA
			qsocket->readDatagram(datagram.data(), datagram.size(), &serverAddress, &serverPort);
			sltDataAnalysis(datagram);
			qsocket->writeDatagram("halo,server_again", serverAddress, serverPort);  //发送以确保对面程序下次发送接下来的报文
			//qDebug() << datagram.size() << endl;
			datagram.clear(); //清空报文，为接收sensordata做准备




	// 		while (datagram.size() == -1 || datagram.size() == 0)//确保报文不为0，随时等待下一封电报的到来
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

