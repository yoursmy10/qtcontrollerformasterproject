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


#define MAX_RADAR_LEN 300 //�״���Զ̽�����
#define ERROR_BETWEEN_RADARLEN_DR  2 //�״���Զ̽��������Ծ�������ӽ�ֵ�����С�����ֵ�������Ϊ�״�ָ��������Զ����������޳������




class DEMUpdate : public QObject
{
	Q_OBJECT

public:
	explicit DEMUpdate(QObject *parent = 0);
	~DEMUpdate();
	//����֮ǰ��preProcessor �� dem_update 
	QThread *myDBthread;
	DataBase *myDB; //���ݿ�
	//actDet res_display;//������ʾ���������demupdate�����еľ���calculator�����������

public slots:
	void initDEMupdate();//������������,��Ҫ���̵߳���
	void calculator(vector<actDet>); //�������ݿ⴫�����Ĵﵽ��ֵ�����ݣ������һ�������ֵ
	void database_update(actDet recv);
	void sltSendDBinfo(QString);//�������ݿ���Ϣ����ʾ����
signals:
	//void sigInitDEMUpdate();
	void sigAdd2DB(actDet); //�����ݿ⺯��ʹ��
	void sigAdd2DB_mahala(actDet, double); //�����Ͼ������ݸ������ͨ�˲��ı���
	void sigAdd2DB_lowpass(actDet); //����ͨ�˲����ݸ������ͨ�˲��ı���
	//void sigAdd2DB_maha(actDet); //�����Ͼ������ݸ��������Ͼ���ı���
	void sigDEMUpdate(actDet);//���͸����յ����ݿ���º����Լ����͸���ͼ����
	void sigFinalResultDisplay(actDet);
	void sig2plot(SENSOR_DATA);//��ֱ������̬��λ����Ϣ���͸���ͼ����
	void sigDEMConsoleInfo(QString);

private:

	vector<update_data> preProcessor();	
	void init_dem(double lat_init, double lon_init, double least_distance);	
	void actDet2latlon(update_data updateData);
	actDet mahaDistance(actDet msg, double xmean, double ymean, double *density);  //���Ͼ���
	actDet lowpassFilter(actDet msg, float omega_lat, float omega_lon, float omega_hgt); //��ͨ�˲���,���շ����˲���ɵ���������msg
	double normal_distribution(double x);
	vector<double> forgotten_index(int len);//�����ܳ��ȣ�����һ������������������ϵ��������

};


#endif