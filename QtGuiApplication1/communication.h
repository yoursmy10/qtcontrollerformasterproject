#pragma once
#ifndef COMMUNICATION_H_
#define COMMUNICATION_H_
#include <QObject>
#include <QUdpSocket>
#include<stdio.h>
#include<WinSock2.h>
#include<time.h>
#include<vector>
#include<string>
#include<math.h>
#include "common.h"
#include<iomanip>
#include<QTimer>
#include<QNetworkDatagram>
#include <QHostAddress>



extern threadsafe_queue<info_recv> queue_infoRecv_data;
extern threadsafe_queue<SENSOR_DATA> queue_sensor_data;
#define BUF_SIZE 6*1024
#define LEN_RECV_IMG 7  //一组和图片中障碍物有关的数据有7个，分别是x1,y1,x2,y2,serial,distance,yaw


class Communication : public QObject
{
	Q_OBJECT

	public:
		explicit Communication(QObject *parent = 0);
		~Communication();
		//QThread *thread;
		QUdpSocket *qsocket;
		SOCKET *sock_std;  //原来打算用c++的方法来做，后来改成了QT的方法
		QTimer *timer_com; //定时器，暂时还没什么用 21/12/13
		
	signals:
		void sigInitCommu();
		void sigCOMConsoleInfo(QString);
		void sigForAlert(double lat, double lon, double yaw_heli, double heli_hgt, double velocity_x, double velocty_y);
	public slots:

		void sltInitCommu();  //通信函数

	private:
		
		void sltSensorDataAnalysis(QByteArray recvMsg);		//传感器数据解包函数，解析收到的信息，
		void sltDataAnalysis(QByteArray recvMsg);//数据解包函数，解析收到的信息，分析有几个障碍物，将每个障碍物的数据分配给一个info_recv结构体

// 		// 客户端
// 		sockaddr_in serverAddr;
		char recvMsg[BUF_SIZE];
		
// 		//本机地址信息
// 		sockaddr_in clientAddr;
// 		sockaddr fromAddr;
		// 客户端
		QHostAddress serverAddress;
		quint16 serverPort;
		// 本机
		QHostAddress localAddress;
		quint16 localPort;
		

};

#endif