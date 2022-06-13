
#pragma once
#ifndef LOOKAHEAD_ALERT_H_
#define LOOKAHEAD_ALERT_H_

#include <QObject>
#include <QMutexLocker>
#include <common.h>
#include <cmath>
#include <vector>



//定义用于边表ET和活动边表AET的通用类Edge
class Edge
{
public:
	int ymax;
	float x;
	float dx;
	Edge* next;
};


//定义用于表示像素点坐标的类Point
class Point
{
public:
	int x;
	int y;
	Point(int x, int y)
	{
		this->x = x;
		this->y = y;
	}
};



//前视告警类，主要是使用地形高程图进行地形扫描/威胁高度计算/前视距离和宽度确定
class LookAheadAlert : public QObject
{
	Q_OBJECT
public:
	explicit LookAheadAlert(QObject *parent = 0);	
	~LookAheadAlert();


signals:
	void sigColliAlert(int);//碰撞预警
	void sigLAD(double);//前视距离
	void sigConsoleInfo(QString);
	void sigEmitLAD(double);
public slots:
	void sltRunAlert(double lat, double lon, double yaw_heli, double heli_hgt, double velocity_x, double velocity_y); //运行整个告警线程
private:
	void terrain_scan(double lat, double lon, double yaw_heli, double LAD, double heli_hgt); //地形扫描
	double LAD_generate(double velocity); //前视距离生成
	int colli_pred(double safe_height, double dem_hgt); //碰撞威胁预测
	void polygon_scan(vector<Point> vertices, double safe_height);
	vector<int> findLoc(double lat, double lon);
	
	float delta_h = 0.9; //最小安全高度系数
	const double LAD_width = 50;//预测范围宽度，一般为210feet，或我默认的50m
	const double alpha = 10; //LAD计算用高度，一般为10°
	const float k_caution = 1;//系数1_caution
	const float k_warning = 0.5;//系数2_warning 现在还又是warning，没有caution，功能还没做 2022/01/17
	const float react_time = 1;//飞行员反应时间,默认1s
	vector<Point> vertices; //用来存放点
	//边表
	Edge *ET[MAX_LEN];
	//活动边表
	Edge *AET;
	//QMutexLocker m_lock;
	

};



#endif