#pragma once
#ifndef LOCDATABASE_H_
#define LOCDATABASE_H_

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <common.h>



//���ݿ���
class DataBase : public QObject
{
	Q_OBJECT
public:
	explicit DataBase(QWidget *parent = 0);
	int queryRowCount(QSqlQuery query); //����ɸѡ������ݵ�����
	
	actDet getNewest_lowpass(QString serial);
	bool tableIsEmplty(QString serial, QString table);
	actDet calcMeanandCov(actDet msg); //����ÿ���ľ�ֵ��Э��������ж�ĳ��ֵ�Ƿ���Ҫ�������£����Ǳ�ɶ�ֵ
	double calcMean(QString serial, QString table, int len, int n);//�������ǰlen��ֵ�ľ�ֵ
	mahalanobisDistance getNewest_mahalanobis(QString serial);
	void addMember_final(actDet member);//��
	void addMember_helistate(SENSOR_DATA member);
	void addMember_display(actDet member);
	void addMember_maha(actDet msg, double maha_dist);//�������Ͼ�����
	void addMember_mahanew(actDet member, double maha);
	~DataBase();

signals:
	void sigCreateDB();
	void sigStartDEMUP(vector<actDet> result); //getclass�����������θ��º���
	void startGet(QString);
	void sigDBinfo(QString);
public slots:
	void sltRunDB();
	void getClass(QString serial);//���ͬһ���кŵĳ�Ա��������ֵ
	void createDB();//�������ݿ�
	void addMember(actDet member);//��
	void addMember_lowpass(actDet member);//���ӵ�ͨ�˲����
	


private:
	QSqlDatabase myDB;
	vector<int> history; //���ڼ����ù�ͬһ��serial�ù�����
	void initDB();//��ʼ�����ݿ�
	
	
	void delMember(actDet member);//ɾ
	void searchMember(actDet member);//��
	void updateMember(actDet member);//��
	void closeDB();//�ر����ݿ�
	void delDB();//ɾ�����ݿ�
	int count_num(vector<int>, int);
	double calcVariance(double mean, double sample);
	//��Ӹ�serial�����������ڣ���ʾ��������1��ֵ������
	void addFrozenMember(QString serial);
	//�ж���serial�µ�lat,lon,hgt�е�ĳһ���Ƿ񱻶���
	bool judgeFrozen(QString para, QString serial);
	//�޸�ĳserial�µ�ĳ�������Ķ���״̬
	void switchFrozenpara(QString para, QString serial);
	QString DBFilePath = "D:/backup/autonomous_landing/QtGuiApplication1/fusion_result.db";
	
};


#endif