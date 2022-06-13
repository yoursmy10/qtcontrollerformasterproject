#pragma once

#ifndef DEMUPDATE_H_
#define DEMUPDATE_H_


#include<common.h>
#include "locdatabase.h"
#include<random>
#include <QObject>
#include <thread>
#include<QDebug>
#include <QMutexLocker>


//DEM: DIGITAL ELEVATOR MAP
using namespace std;


#define MAX_RADAR_LEN 300 //雷达最远探测距离
#define ERROR_BETWEEN_RADARLEN_DR  2 //雷达最远探测距离和相对距离的最大接近值，如果小于这个值则可以认为雷达指向了无限远处，则可以剔除这个点




class DEMUpdate : public QObject
{
	Q_OBJECT

public:
	explicit DEMUpdate(QObject *parent = 0);
	~DEMUpdate();
	//整合之前的preProcessor 和 dem_update 
	QThread *myDBthread;
	DataBase *myDB; //数据库
	//actDet res_display;//用于显示最终输出到demupdate函数中的经过calculator处理的数据量

public slots:
	void initDEMupdate();//运行整个任务,需要被线程调用
	void calculator(vector<actDet>); //计算数据库传过来的达到阈值的数据，并输出一组最合理值
	void database_update(actDet recv);
	void sltSendDBinfo(QString);//发送数据库信息给显示界面
signals:
	//void sigInitDEMUpdate();
	void sigAdd2DB(actDet); //对数据库函数使用
	void sigAdd2DB_mahala(actDet, double); //将马氏距离数据更新入低通滤波的表中
	void sigAdd2DB_lowpass(actDet); //将低通滤波数据更新入低通滤波的表中
	//void sigAdd2DB_maha(actDet); //将马氏距离数据更新入马氏距离的表中
	void sigDEMUpdate(actDet);//发送给最终的数据库更新函数以及发送给画图函数
	void sigFinalResultDisplay(actDet);
	void sig2plot(SENSOR_DATA);//将直升机姿态和位置信息发送给画图函数
	void sigDEMConsoleInfo(QString);

private:

	vector<update_data> preProcessor();	
	void init_dem(double lat_init, double lon_init, double least_distance);	
	void actDet2latlon(update_data updateData);
	actDet mahaDistance(actDet msg, double xmean, double ymean, double *density);  //马氏距离
	actDet lowpassFilter(actDet msg, float omega_lat, float omega_lon, float omega_hgt); //低通滤波器,最终返回滤波完成的输入数据msg
	double normal_distribution(double x);
	vector<double> forgotten_index(int len);//输入总长度，返回一个包含各个数的遗忘系数的数列

};


#endif