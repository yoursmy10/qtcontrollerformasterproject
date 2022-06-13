#pragma once
#ifndef LOCDATABASE_H_
#define LOCDATABASE_H_

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <common.h>



//数据库类
class DataBase : public QObject
{
	Q_OBJECT
public:
	explicit DataBase(QWidget *parent = 0);
	int queryRowCount(QSqlQuery query); //返回筛选后的数据的行数
	
	actDet getNewest_lowpass(QString serial);
	bool tableIsEmplty(QString serial, QString table);
	actDet calcMeanandCov(actDet msg); //计算每个的均值与协方差，并且判定某个值是否还需要正常更新，还是变成定值
	double calcMean(QString serial, QString table, int len, int n);//计算表中前len个值的均值
	mahalanobisDistance getNewest_mahalanobis(QString serial);
	void addMember_final(actDet member);//增
	void addMember_helistate(SENSOR_DATA member);
	void addMember_display(actDet member);
	void addMember_maha(actDet msg, double maha_dist);//增加马氏距离结果
	void addMember_mahanew(actDet member, double maha);
	~DataBase();

signals:
	void sigCreateDB();
	void sigStartDEMUP(vector<actDet> result); //getclass发出，给地形更新函数
	void startGet(QString);
	void sigDBinfo(QString);
public slots:
	void sltRunDB();
	void getClass(QString serial);//获得同一序列号的成员数量及其值
	void createDB();//创建数据库
	void addMember(actDet member);//增
	void addMember_lowpass(actDet member);//增加低通滤波结果
	


private:
	QSqlDatabase myDB;
	vector<int> history; //用于计算用过同一个serial用过几次
	void initDB();//初始化数据库
	
	
	void delMember(actDet member);//删
	void searchMember(actDet member);//查
	void updateMember(actDet member);//改
	void closeDB();//关闭数据库
	void delDB();//删除数据库
	int count_num(vector<int>, int);
	double calcVariance(double mean, double sample);
	//添加该serial，如果添加在内，表示其起码有1个值被冻结
	void addFrozenMember(QString serial);
	//判定该serial下的lat,lon,hgt中的某一个是否被冻结
	bool judgeFrozen(QString para, QString serial);
	//修改某serial下的某个参数的冻结状态
	void switchFrozenpara(QString para, QString serial);
	QString DBFilePath = "D:/backup/autonomous_landing/QtGuiApplication1/fusion_result.db";
	
};


#endif