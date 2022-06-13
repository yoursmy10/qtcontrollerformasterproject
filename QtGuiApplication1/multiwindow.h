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
	void tableviewUpdate(actDet recv);//在表格中显示
	//void sltStartDEM();
	void actDet_result_display(actDet); //用于接收显示最终更新到数据库中的经过处理的数据,在左下方显示
	//void drawRect();//在三维图中绘制立方体(建筑物)
	void updatePlot_heli(SENSOR_DATA tmp_sensor);
	void updatePlot_obst(actDet tmp_obst);
	void replot();//定时更新二维图
	//void updateYawFromTxt(double angle);//通过修改距离传感器的yaw值，但是这样做实时性可能就很差了
	void sltAppendConsole_DEM(QString msg_console);//将控制台信息输出到显示中
	void sltAppendConsole_COM(QString msg_console);//将控制台信息输出到显示中
	void sltUpdateHeliData(double lat, double lon, double yaw_heli, double heli_hgt, double velocity_x, double velocity_y); //在显示中更新直升机数值
	void sltChangeLight(int i);
	void sltUpdateVelandLADPlot();//更新速度及LAD折线图
	void sltRecLAD(double);//接受告警线程发送的LAD数据
signals:
	void sigInitDEM(); //触发地形更新线程，使其启动
	void sigInitCom(); //触发通信线程，使其启动
	void sigRunAlert(double lat, double lon, double yaw_heli, double heli_hgt, double velocity_x, double velocity_y); //告警线程
	void sigInitBars();
	void sigEditYaw(double);


private:
	Ui::multiwindowClass ui;
	//void initBars();//初始化3d的图标
	//QtDataVisualization::Q3DBars *mybars; //三维柱状图
	//QtDataVisualization::Bars3DRenderer

	//通信
	Communication *mycom;
	QThread *m_childThread_com;
	//地形更新算法
	DEMUpdate *myDEMUpdate;
	QThread *m_childThread_DEMUpdate;
	//数据库线程
	DataBase *mydb;
	QThread *m_childThread_db;

	QStandardItemModel *mymodel;
	QCustomPlot *customPlot;  //二维图
	QCustomPlot *customPlot_velocity_LAD; //第二个二维图表

	QTimer *timer;//计时器
	QTimer *timer_alert;//用于告警算法启动
	QTimer *dataTimer; //用于更新速度和LAD二维折线图
	//告警算法
	LookAheadAlert *myalert;
	QThread *m_childThread_LookAheadAlert;
	//void showmultiboxes();
	void startDemUpdate();
	void endDemUpdate();
	void task_commu();
	void task_dem_update();
	void drawPlot();
	void drawPlot_velocity();//初始化速度坐标系
	void setLED();//设置led灯

};

#endif