#pragma once
#ifndef MULTIWINDOW_H_
#define MULTIWINDOW_H_
#define  WIN32_LEAN_AND_MEAN

#include <QtWidgets/QMainWindow>
#include<QObject>
#include <QMutex>

#include <QStandardItemModel>
#include <QDebug>
#include <QThread>
#include <QPushButton>
#include <QTimer>
#include"qpushbutton.h"
#include"qmessagebox.h"
#include <QtDataVisualization/q3dbars.h>
#include <QtDataVisualization/qbardataproxy.h>
#include <QtDataVisualization/qabstract3dseries.h>
#include <WinSock2.h>
#include <QFile>


#include "D:/backup/autonomous_landing/QtGuiApplication1/QtGuiApplication1/GeneratedFiles/ui_multiwindow.h"
#include "communication.h"
#include "dem_update.h"
#include "locdatabase.h"
#include "lookahead_alert.h"

//using namespace QtDataVisualization;


class multiwindow : public QMainWindow
{
	Q_OBJECT

public:
	multiwindow(QWidget *parent = Q_NULLPTR);

public slots:
	void pushButton_start_clicked();
	void tableviewUpdate(actDet recv);//�ڱ������ʾ
	//void sltStartDEM();
	void actDet_result_display(actDet); //���ڽ�����ʾ���ո��µ����ݿ��еľ������������,�����·���ʾ
	//void drawRect();//����άͼ�л���������(������)
	void updatePlot_heli(SENSOR_DATA tmp_sensor);
	void updatePlot_obst(actDet tmp_obst);
	void replot();//��ʱ���¶�άͼ
	//void updateYawFromTxt(double angle);//ͨ���޸ľ��봫������yawֵ������������ʵʱ�Կ��ܾͺܲ���
	void sltAppendConsole_DEM(QString msg_console);//������̨��Ϣ�������ʾ��
	void sltAppendConsole_COM(QString msg_console);//������̨��Ϣ�������ʾ��
	void sltUpdateHeliData(double lat, double lon, double yaw_heli, double heli_hgt, double velocity_x, double velocity_y); //����ʾ�и���ֱ������ֵ
	void sltChangeLight(int i);
	void sltUpdateVelandLADPlot();//�����ٶȼ�LAD����ͼ
	void sltRecLAD(double);//���ܸ澯�̷߳��͵�LAD����
signals:
	void sigInitDEM(); //�������θ����̣߳�ʹ������
	void sigInitCom(); //����ͨ���̣߳�ʹ������
	void sigRunAlert(double lat, double lon, double yaw_heli, double heli_hgt, double velocity_x, double velocity_y); //�澯�߳�
	void sigInitBars();
	void sigEditYaw(double);


private:
	Ui::multiwindowClass ui;
	//void initBars();//��ʼ��3d��ͼ��
	//QtDataVisualization::Q3DBars *mybars; //��ά��״ͼ
	//QtDataVisualization::Bars3DRenderer

	//ͨ��
	Communication *mycom;
	QThread *m_childThread_com;
	//���θ����㷨
	DEMUpdate *myDEMUpdate;
	QThread *m_childThread_DEMUpdate;
	//���ݿ��߳�
	DataBase *mydb;
	QThread *m_childThread_db;

	QStandardItemModel *mymodel;
	QCustomPlot *customPlot;  //��άͼ
	QCustomPlot *customPlot_velocity_LAD; //�ڶ�����άͼ��

	QTimer *timer;//��ʱ��
	QTimer *timer_alert;//���ڸ澯�㷨����
	QTimer *dataTimer; //���ڸ����ٶȺ�LAD��ά����ͼ
	//�澯�㷨
	LookAheadAlert *myalert;
	QThread *m_childThread_LookAheadAlert;
	//void showmultiboxes();
	void startDemUpdate();
	void endDemUpdate();
	void task_commu();
	void task_dem_update();
	void drawPlot();
	void drawPlot_velocity();//��ʼ���ٶ�����ϵ
	void setLED();//����led��

};

#endif