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
#define LEN_RECV_IMG 7  //һ���ͼƬ���ϰ����йص�������7�����ֱ���x1,y1,x2,y2,serial,distance,yaw


class Communication : public QObject
{
	Q_OBJECT

	public:
		explicit Communication(QObject *parent = 0);
		~Communication();
		//QThread *thread;
		QUdpSocket *qsocket;
		SOCKET *sock_std;  //ԭ��������c++�ķ��������������ĳ���QT�ķ���
		QTimer *timer_com; //��ʱ������ʱ��ûʲô�� 21/12/13
		
	signals:
		void sigInitCommu();
		void sigCOMConsoleInfo(QString);
		void sigForAlert(double lat, double lon, double yaw_heli, double heli_hgt, double velocity_x, double velocty_y);
	public slots:

		void sltInitCommu();  //ͨ�ź���

	private:
		
		void sltSensorDataAnalysis(QByteArray recvMsg);		//���������ݽ�������������յ�����Ϣ��
		void sltDataAnalysis(QByteArray recvMsg);//���ݽ�������������յ�����Ϣ�������м����ϰ����ÿ���ϰ�������ݷ����һ��info_recv�ṹ��

// 		// �ͻ���
// 		sockaddr_in serverAddr;
		char recvMsg[BUF_SIZE];
		
// 		//������ַ��Ϣ
// 		sockaddr_in clientAddr;
// 		sockaddr fromAddr;
		// �ͻ���
		QHostAddress serverAddress;
		quint16 serverPort;
		// ����
		QHostAddress localAddress;
		quint16 localPort;
		

};

#endif