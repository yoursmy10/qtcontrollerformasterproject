
#pragma once
#ifndef LOOKAHEAD_ALERT_H_
#define LOOKAHEAD_ALERT_H_

#include <QObject>
#include <QMutexLocker>
#include <common.h>
#include <cmath>
#include <vector>



//�������ڱ߱�ET�ͻ�߱�AET��ͨ����Edge
class Edge
{
public:
	int ymax;
	float x;
	float dx;
	Edge* next;
};


//�������ڱ�ʾ���ص��������Point
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



//ǰ�Ӹ澯�࣬��Ҫ��ʹ�õ��θ߳�ͼ���е���ɨ��/��в�߶ȼ���/ǰ�Ӿ���Ϳ��ȷ��
class LookAheadAlert : public QObject
{
	Q_OBJECT
public:
	explicit LookAheadAlert(QObject *parent = 0);	
	~LookAheadAlert();


signals:
	void sigColliAlert(int);//��ײԤ��
	void sigLAD(double);//ǰ�Ӿ���
	void sigConsoleInfo(QString);
	void sigEmitLAD(double);
public slots:
	void sltRunAlert(double lat, double lon, double yaw_heli, double heli_hgt, double velocity_x, double velocity_y); //���������澯�߳�
private:
	void terrain_scan(double lat, double lon, double yaw_heli, double LAD, double heli_hgt); //����ɨ��
	double LAD_generate(double velocity); //ǰ�Ӿ�������
	int colli_pred(double safe_height, double dem_hgt); //��ײ��вԤ��
	void polygon_scan(vector<Point> vertices, double safe_height);
	vector<int> findLoc(double lat, double lon);
	
	float delta_h = 0.9; //��С��ȫ�߶�ϵ��
	const double LAD_width = 50;//Ԥ�ⷶΧ��ȣ�һ��Ϊ210feet������Ĭ�ϵ�50m
	const double alpha = 10; //LAD�����ø߶ȣ�һ��Ϊ10��
	const float k_caution = 1;//ϵ��1_caution
	const float k_warning = 0.5;//ϵ��2_warning ���ڻ�����warning��û��caution�����ܻ�û�� 2022/01/17
	const float react_time = 1;//����Ա��Ӧʱ��,Ĭ��1s
	vector<Point> vertices; //������ŵ�
	//�߱�
	Edge *ET[MAX_LEN];
	//��߱�
	Edge *AET;
	//QMutexLocker m_lock;
	

};



#endif