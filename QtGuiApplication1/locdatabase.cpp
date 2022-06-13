#include "locdatabase.h"
#include <QDebug>
#include <QMessageBox>
#include <vector>
#include <QFile>
#include <float.h>




DataBase::DataBase(QWidget *parent) :QObject(parent)
{

}

DataBase::~DataBase()
{
	history.clear();
	myDB.close(); //关闭数据库
	QFile::remove(DBFilePath); //删除数据库，需要记录数据时可以注释掉

}


void DataBase::sltRunDB()  //初始化生成database,其实这里就是create了
{
	myDB = QSqlDatabase::addDatabase("QSQLITE");
	#if 1
	myDB.setDatabaseName(DBFilePath);//生成文件
	#endif
	#if 0
	myDB.setDatabaseName(":memory");  //在内存中生成
	#endif
	
	if (!myDB.open())
	{
		qDebug() << "database not open..." << endl;
		emit sigDBinfo("database not open...");
	}

	connect(this, SIGNAL(sigCreateDB()), this, SLOT(createDB()));//调用后自动createDB

	emit sigCreateDB();  //生成数据库
}



void DataBase::createDB()//给初始化完成的database创建各种表
{
	QSqlQuery query; //QSqlQuery类提供执行和操作的SQL语句的方法 query是一个句柄

	//创建班级表 包括：以序列号为id的序列号，班级总人数    exec()->执行   似乎不需要班级表了，直接用学生表就好
	//query.exec("create table class(serial int, sum int)"); 
	

	//创建学生表,包括，id 经纬度范围 高程以及班级号(serial)，这个serial是外键，用于关联班级和学生
	query.exec("create table student(id int, lat_bottom varchar, lat_top varchar, lon_left varchar, lon_right varchar, hgt varchar, serial varchar)");
	//创建一个低通滤波数据存储表
	query.exec("create table lowpass(id int, lat_bottom varchar, lat_top varchar, lon_left varchar, lon_right varchar, hgt varchar, serial varchar)");
	//创建一个马氏距离数据存储表
	query.exec("create table mahalanobis(serial varchar, mahalanobis_distance varchar, lon_left varchar, lat_top varchar)");
	
	//用来表示该序号的值是否已经冻结
	query.exec("create table serialFrozen(serial int, lat int, lon int, hgt int)");


	//创建地形更新算法输出的最终坐标,包括，id 经纬度范围 高程以及班级号(serial)，这个serial是外键，用于关联班级和学生
	query.exec("create table finaloutput(id int, lat_bottom varchar, lat_top varchar, lon_left varchar, lon_right varchar, hgt varchar, serial varchar)");
	query.exec("create table heliState(lat varchar, lon varchar, alt varchar, vx varchar, vy varchar, vz varchar)");

	query.exec("create table displayoutput(id int, lat_bottom varchar, lat_top varchar, lon_left varchar, lon_right varchar, hgt varchar, serial varchar)");
	query.exec("create table mahala(id int, lat_bottom varchar, lat_top varchar, lon_left varchar, lon_right varchar, hgt varchar, serial varchar, mahaDistance varchar)");

	
}



void DataBase::closeDB()
{
	myDB.close();
}




//增加新数据，根据member的数据更新班级和学生，如果没有班级就创建班级，然后再创建学生
void DataBase::addMember(actDet member)
{
	QString msg_console = "Database:addMember is working...";
	emit sigDBinfo(msg_console);
	
	//赋值
	QString stu_serial = QString("%1").arg(member.serial);
	//QString lat_bottom = QString("%1").arg(member.lat_bottom);
	QString lat_bottom = QString::number(member.lat_bottom, 10,10);
	QString lat_top = QString::number(member.lat_top, 10, 10);
	QString lon_left = QString::number(member.lon_left, 10, 10);
	QString lon_right = QString::number(member.lon_right,10,10);
	QString hgt = QString::number(member.hgt,10,10);
	QString stu_id = QString("%1").arg(member.id);
	

	//判断有无空信息
	if (stu_serial.isEmpty() || lat_top.isEmpty() || lat_bottom.isEmpty() || lon_right.isEmpty() || lon_left.isEmpty() || hgt.isEmpty()||stu_id.isEmpty())
	{
		//QMessageBox::information(this, "Warning! information not complete!");
		qDebug() << "Warning! Information incomplete!" << endl;
		emit sigDBinfo("Warning! Information incomplete!");
		return;
	}
	
	else
	{
		QSqlQuery query;
		//直接插入stu
		query.prepare("insert into student (id, lat_bottom, lat_top, lon_left, lon_right, hgt, serial) \
									values (:id, :lat_bottom, :lat_top, :lon_left, :lon_right, :hgt, :serial)");
		query.bindValue(":id",stu_id);
		query.bindValue(":lat_bottom", lat_bottom);
		query.bindValue(":lat_top", lat_top);
		query.bindValue(":lon_left", lon_left);
		query.bindValue(":lon_right", lon_right);
		query.bindValue(":hgt", hgt);
		query.bindValue(":serial", stu_serial);
		query.exec();

	}
	//emit startGet(stu_serial);

}






//有一种恶意堆砌的感觉
void DataBase::addMember_display(actDet member)
{
	QString msg_console = "Database:addMember_display is working...";
	emit sigDBinfo(msg_console);

	//赋值
	QString stu_serial = QString("%1").arg(member.serial);
	//QString lat_bottom = QString("%1").arg(member.lat_bottom);
	QString lat_bottom = QString::number(member.lat_bottom, 10, 10);
	QString lat_top = QString::number(member.lat_top, 10, 10);
	QString lon_left = QString::number(member.lon_left, 10, 10);
	QString lon_right = QString::number(member.lon_right, 10, 10);
	QString hgt = QString::number(member.hgt, 10, 10);
	QString stu_id = QString("%1").arg(member.id);


	//判断有无空信息
	if (stu_serial.isEmpty() || lat_top.isEmpty() || lat_bottom.isEmpty() || lon_right.isEmpty() || lon_left.isEmpty() || hgt.isEmpty() || stu_id.isEmpty())
	{
		//QMessageBox::information(this, "Warning! information not complete!");
		qDebug() << "Warning! Information incomplete!" << endl;
		emit sigDBinfo("Warning! Information incomplete!");
		return;
	}

	else
	{
		QSqlQuery query;
		//直接插入stu
		query.prepare("insert into displayoutput (id, lat_bottom, lat_top, lon_left, lon_right, hgt, serial) \
									values (:id, :lat_bottom, :lat_top, :lon_left, :lon_right, :hgt, :serial)");
		query.bindValue(":id", stu_id);
		query.bindValue(":lat_bottom", lat_bottom);
		query.bindValue(":lat_top", lat_top);
		query.bindValue(":lon_left", lon_left);
		query.bindValue(":lon_right", lon_right);
		query.bindValue(":hgt", hgt);
		query.bindValue(":serial", stu_serial);
		query.exec();

	}
	//emit startGet(stu_serial);

}






//有一种恶意堆砌的感觉
void DataBase::addMember_mahanew(actDet member, double maha)
{
	QString msg_console = "Database:addMember_display is working...";
	emit sigDBinfo(msg_console);


	
	//赋值
	QString stu_serial = QString("%1").arg(member.serial);
	//QString lat_bottom = QString("%1").arg(member.lat_bottom);
	QString lat_bottom = QString::number(member.lat_bottom, 10, 10);
	QString lat_top = QString::number(member.lat_top, 10, 10);
	QString lon_left = QString::number(member.lon_left, 10, 10);
	QString lon_right = QString::number(member.lon_right, 10, 10);
	QString hgt = QString::number(member.hgt, 10, 10);
	QString stu_id = QString("%1").arg(member.id);
	QString maha_dis = QString::number(maha, 10, 10);


	//判断有无空信息
	if (lat_top.isEmpty() || lat_bottom.isEmpty() || lon_right.isEmpty() || lon_left.isEmpty() || hgt.isEmpty() || stu_id.isEmpty()||std::isnan(maha))
	{
		//QMessageBox::information(this, "Warning! information not complete!");
		qDebug() << "Warning! Information incomplete!" << endl;
		emit sigDBinfo("Warning! Information incomplete!");
		return;
	}

	else
	{
		QSqlQuery query;
		//直接插入stu
		query.prepare("insert into mahala (id, lat_bottom, lat_top, lon_left, lon_right, hgt, serial, mahaDistance) \
									values (:id, :lat_bottom, :lat_top, :lon_left, :lon_right, :hgt, :serial, :mahaDistance)");
		query.bindValue(":id", stu_id);
		query.bindValue(":lat_bottom", lat_bottom);
		query.bindValue(":lat_top", lat_top);
		query.bindValue(":lon_left", lon_left);
		query.bindValue(":lon_right", lon_right);
		query.bindValue(":hgt", hgt);
		query.bindValue(":serial", stu_serial);
		query.bindValue(":mahaDistance", maha_dis);
		query.exec();

	}
	//emit startGet(stu_serial);

}











//增加新数据，直升机运动轨迹
void DataBase::addMember_helistate(SENSOR_DATA member)
{
	QString msg_console = "Database:HeliCopterState Record is working...";
	emit sigDBinfo(msg_console);

	//赋值
	//QString lat = QString("%1").arg(member.lat);
	//QString lat_bottom = QString("%1").arg(member.lat_bottom);
	QString lat = QString::number(member.lat, 10, 10);
	QString lon = QString::number(member.lon, 10, 10);
	QString alt = QString::number(member.alt_gps, 10, 10);
	QString vx = QString::number(member.velocity_x, 10, 10);
	QString vy = QString::number(member.velocity_y, 10, 10);
	QString vz = QString::number(member.velocity_z, 10, 10);
	


	//判断有无空信息
	if (lat.isEmpty() || lon.isEmpty() || alt.isEmpty() )
	{
		//QMessageBox::information(this, "Warning! information not complete!");
		qDebug() << "Warning! Information incomplete!" << endl;
		emit sigDBinfo("Warning! Information incomplete!");
		return;
	}

	else
	{
		QSqlQuery query;
		//直接插入stu
		query.prepare("insert into heliState (lat, lon, alt, vx, vy, vz) \
									values (:lat, :lon, :alt, :vx, :vy, :vz)");
		query.bindValue(":lat", lat);
		query.bindValue(":lon", lon);
		query.bindValue(":alt", alt);
		query.bindValue(":vx", vx);
		query.bindValue(":vy", vy);
		query.bindValue(":vz", vz);
		
		query.exec();

	}

}








//增加新数据，根据member的数据更新班级和学生，如果没有班级就创建班级，然后再创建学生
void DataBase::addMember_final(actDet member)
{
	QString msg_console = "Database:addMember is working...";
	emit sigDBinfo(msg_console);

	//赋值
	QString stu_serial = QString("%1").arg(member.serial);
	//QString lat_bottom = QString("%1").arg(member.lat_bottom);
	QString lat_bottom = QString::number(member.lat_bottom, 10, 10);
	QString lat_top = QString::number(member.lat_top, 10, 10);
	QString lon_left = QString::number(member.lon_left, 10, 10);
	QString lon_right = QString::number(member.lon_right, 10, 10);
	QString hgt = QString::number(member.hgt, 10, 10);
	QString stu_id = QString("%1").arg(member.id);


	//判断有无空信息
	if (stu_serial.isEmpty() || lat_top.isEmpty() || lat_bottom.isEmpty() || lon_right.isEmpty() || lon_left.isEmpty() || hgt.isEmpty() || stu_id.isEmpty())
	{
		//QMessageBox::information(this, "Warning! information not complete!");
		qDebug() << "Warning! Information incomplete!" << endl;
		emit sigDBinfo("Warning! Information incomplete!");
		return;
	}

	else
	{
		QSqlQuery query;
		//直接插入stu
		query.prepare("insert into finaloutput (id, lat_bottom, lat_top, lon_left, lon_right, hgt, serial) \
									values (:id, :lat_bottom, :lat_top, :lon_left, :lon_right, :hgt, :serial)");
		query.bindValue(":id", stu_id);
		query.bindValue(":lat_bottom", lat_bottom);
		query.bindValue(":lat_top", lat_top);
		query.bindValue(":lon_left", lon_left);
		query.bindValue(":lon_right", lon_right);
		query.bindValue(":hgt", hgt);
		query.bindValue(":serial", stu_serial);
		query.exec();

	}
	emit startGet(stu_serial);//发送这个sig意味着让其驱动getClass

}






//添加低通滤波器每次计算结束后的新数据到lowpass表中,actDet中的serial是主动探测中得到的id号就是
void DataBase::addMember_lowpass(actDet member)
{
	QString msg_console = "Database:addMember_lowpass is working...";
	emit sigDBinfo(msg_console);

	//赋值
	QString stu_serial = QString("%1").arg(member.serial);
	//QString lat_bottom = QString("%1").arg(member.lat_bottom);
	QString lat_bottom = QString::number(member.lat_bottom, 10, 10);
	QString lat_top = QString::number(member.lat_top, 10, 10);
	QString lon_left = QString::number(member.lon_left, 10, 10);
	QString lon_right = QString::number(member.lon_right, 10, 10);
	QString hgt = QString::number(member.hgt, 10, 10);
	QString stu_id = QString("%1").arg(member.id);


	//判断有无空信息
	if (lat_top.isEmpty() ||  lon_left.isEmpty() || hgt.isEmpty() || stu_id.isEmpty())
	{
		//QMessageBox::information(this, "Warning! information not complete!");
		qDebug() << "Warning! Information incomplete!" << endl;
		emit sigDBinfo("Warning! Information incomplete!");
		return;
	}

	else
	{
		QSqlQuery query;
		//直接插入lowpass表
		query.prepare("insert into lowpass (id, lat_bottom, lat_top, lon_left, lon_right, hgt, serial) \
									values (:id, :lat_bottom, :lat_top, :lon_left, :lon_right, :hgt, :serial)");
		query.bindValue(":id", stu_id);
		query.bindValue(":lat_bottom", lat_bottom);
		query.bindValue(":lat_top", lat_top);
		query.bindValue(":lon_left", lon_left);
		query.bindValue(":lon_right", lon_right);
		query.bindValue(":hgt", hgt);
		query.bindValue(":serial", stu_serial);
		query.exec();

	}
	//emit startGet(stu_serial);
}




//添加计算得到的马氏距离每次计算结束后的新数据到mahalanobis表中
void DataBase::addMember_maha(actDet msg, double maha_dist)
{
	QString msg_console = "Database:addMember_maha is working...";
	qDebug() << msg_console << endl;
	emit sigDBinfo(msg_console);

	//赋值

	QString maha_serial = QString("%1").arg(msg.serial);
	QString maha = QString::number(maha_dist, 10, 10);
	QString lat_top = QString::number(msg.lat_top, 10, 10);
	QString lon_left = QString::number(msg.lon_left, 10, 10);
	
	//QString maha = maha;


	//判断有无空信息
	if (maha_serial.isEmpty())
	{
		//QMessageBox::information(this, "Warning! information not complete!");
		qDebug() << "Warning! Information incomplete!" << endl;
		emit sigDBinfo("Warning! Information incomplete!");
		return;
	}

	else
	{
		QSqlQuery query;
		//直接插入lowpass表
		query.prepare("insert into mahalanobis (serial, mahalanobis_distance, lon_left, lat_top) \
									values (:serial,  :mahalanobis_distance, lon_left, lat_top)");
		query.bindValue(":serial", maha_serial);
		query.bindValue(":mahalanobis_distance", maha);
		query.bindValue(":lon_left", lon_left);
		query.bindValue(":lat_top", lat_top);
		//query.bindValue(":serial", stu_serial);
		query.exec();

	}
	//emit startGet(stu_serial);
}





//返回针对某序号的最新的低通滤波数据
actDet DataBase::getNewest_lowpass(QString serial)
{
	QSqlQuery query;//获得句柄
	QString msg_console = "Database:getNewest_lowpass is working...";
	qDebug() << msg_console;
	emit sigDBinfo(msg_console);


	query.prepare("SELECT * FROM lowpass WHERE serial = '" + serial + "'");  //这样能获得全部的包含serial的数据,应该是可以的，然后自然可以判断其长度.
																			 //query.prepare("SELECT * FROM student GROUP BY serial HAVING count(serial = '" + serial + "') > 20");  //这样能获得全部的包含serial的数据么？应该是可以的
	query.exec();
	int len_query = queryRowCount(query);//获取行数
										 //QSqlRecord record = query.record();
										 //int column = record.count(); //保存列数,第0列是列名
	qDebug() << "lowpass_len_query = " << len_query << endl;

	actDet tmp;
	//直接指向最新的该序列号的数据
	query.last();
	//query.previous();
	
	tmp.id = query.value(0).toInt();
	tmp.lat_bottom = query.value(1).toDouble();
	tmp.lat_top = query.value(2).toDouble();
	tmp.lon_left = query.value(3).toDouble();
	tmp.lon_right = query.value(4).toDouble();
	tmp.hgt = query.value(5).toDouble();
	tmp.serial = query.value(6).toInt();

	return tmp;//返回第k-1个值
	
	//qDebug() << "tmp.serial = " << tmp.serial << endl;
}



//判定lowpass表中某序号值行数是否为空
bool DataBase::tableIsEmplty(QString serial, QString table)
{
	QSqlQuery query;//获得句柄
	QString send;
	send = "SELECT * FROM " + table + " WHERE serial = '" + serial + "'";
	query.prepare(send);
	qDebug() << "tableisempty is working..." << endl;
	//query.prepare("SELECT * FROM lowpass WHERE serial = '" + serial + "'");
	query.exec();
	int len_query = queryRowCount(query);//获取行数

	if(len_query  == 0)
	{
		return 1; //一眼顶针，鉴定为空
	}
	else
	{
		return 0; //一眼顶针，鉴定为假
	}


}











//返回针对某序号的最新的马氏距离数据
mahalanobisDistance DataBase::getNewest_mahalanobis(QString serial)
{
	QSqlQuery query;//获得句柄
	QString msg_console = "Database:getNewest_mahalaobisn is working...";
	emit sigDBinfo(msg_console);


	query.prepare("SELECT * FROM mahala WHERE serial = '" + serial + "'");  //这样能获得全部的包含serial的数据,应该是可以的，然后自然可以判断其长度.
																			 //query.prepare("SELECT * FROM student GROUP BY serial HAVING count(serial = '" + serial + "') > 20");  //这样能获得全部的包含serial的数据么？应该是可以的
	query.exec();
	int len_query = queryRowCount(query);//获取行数
										 //QSqlRecord record = query.record();
										 //int column = record.count(); //保存列数,第0列是列名
	qDebug() << "len_query = " << len_query << endl;

	mahalanobisDistance res;
	//直接指向最新的该序列号的数据
	query.last();
	//query.previous();


	res.lon_left = query.value(3).toDouble();
	res.lat_top = query.value(2).toDouble();
	/*
	tmp.id = query.value(0).toInt();
	tmp.lat_bottom = query.value(1).toDouble();
	tmp.lat_top = query.value(2).toDouble();
	tmp.lon_left = query.value(3).toDouble();
	tmp.lon_right = query.value(4).toDouble();
	tmp.hgt = query.value(5).toDouble();
	tmp.serial = query.value(6).toInt();
	*/
	//qDebug() << "tmp.serial = " << tmp.serial << endl;

	return res;
}





//可以算最麻烦的一个了吧，要实时在线统计新增加的这个成员的序列号，如果满阈值了就要调用进行计算和输出，要定时输出
void DataBase::getClass(QString serial)
{
	QSqlQuery query;//获得句柄
	QString msg_console = "Database:getClass is working...";
	emit sigDBinfo(msg_console);
	
	//query.prepare("SELECT * FROM student WHERE serial = '" + serial + "'");  //这样能获得全部的包含serial的数据,应该是可以的，然后自然可以判断其长度.
	query.prepare("SELECT * FROM finaloutput WHERE serial = '"+ serial +"'");  //这样能获得全部的包含serial的数据,应该是可以的，然后自然可以判断其长度.
	//query.prepare("SELECT * FROM student GROUP BY serial HAVING count(serial = '" + serial + "') > 20");  //这样能获得全部的包含serial的数据么？应该是可以的
	query.exec();
	int len_query = queryRowCount(query);//获取行数
	//QSqlRecord record = query.record();
	//int column = record.count(); //保存列数,第0列是列名
	qDebug() << "len_query = " << len_query << endl; 
	vector<actDet> result = {};

	
	int count_order = 0;//之前输出过几次
	int initial_Pos = query.at(); //记录变化前位置
	query.next(); //point to the valid_value
	//count_order = std::count(history.begin(), history.end(), query.value(6).toInt()) + 1;  //之前输出过几次
	count_order = count_num(history, query.value(6).toInt()) + 1;
	

	
	//输出count_order看看
	emit sigDBinfo("count_order = "+QString::number(count_order)+"\n"+"query.value(6).toInt() =  " + QString::number(query.value(6).toInt()));
	query.seek(initial_Pos);//Point_Back

	
	if (len_query > CAL_LEN*count_order) //如果row大于阈值数量
	{		
		
		actDet tmp;
		//读取数据库中所有该serial的数据
		query.next();
		history.push_back(query.value(6).toInt()); //用于计数
		query.seek(initial_Pos);
		
		while (query.next())  //和链表很像
		{
			tmp.id = query.value(0).toInt();
			tmp.lat_bottom = query.value(1).toDouble();
			tmp.lat_top = query.value(2).toDouble();
			tmp.lon_left = query.value(3).toDouble();
			tmp.lon_right = query.value(4).toDouble();
			tmp.hgt = query.value(5).toDouble();
			tmp.serial = query.value(6).toInt();
			result.push_back(tmp);
			qDebug() << "tmp.serial = " << tmp.serial << endl;
			
			
		}
		
		//至此，就得到了达到阈值数量的serial的队列
	}
	else
	{
		
		return;
		//return了就不emitSIGNAL了
	}
	emit sigStartDEMUP(result);//发送数据

}


//计算筛选后的行数
int DataBase::queryRowCount(QSqlQuery query)
{
	int initialPos = query.at();
	// Very strange but for no records .at() returns -2
	int pos = 0;
	if (query.last()) {
		pos = query.at() + 1;
	}
	else {
		pos = 0;
	}
	// Important to restore initial pos
	query.seek(initialPos);
	return pos;
}





//返回[front, last]中包含serial的元素个数
int DataBase::count_num(vector<int> msg, int serial)
{
	int count = 0;
	for (int i = 0; i < msg.size();i++)
	{
		if (msg[i] == serial)
			count++;
	}
	return count;
}







/*
时间：2022/03/23
创建人：吴旭峰
计算均值
serial : serial
table : 表名
len : 需要计算的队列长度
n: 该数据在该表的第几列
*/
double DataBase::calcMean(QString serial, QString table, int len, int n)
{
	QSqlQuery query;//获得句柄
	QString send;
	send = "SELECT * FROM " + table + " WHERE serial = '" + serial + "'";
	query.prepare(send);
	qDebug() << "calcMEAN is working..." << endl;
	//query.prepare("SELECT * FROM lowpass WHERE serial = '" + serial + "'");
	query.exec();
	query.last();//指向最后一个

	
	int len_query = queryRowCount(query);//获取行数
	//如果长度不达标
	if(len_query < len)
	{
		return -1;
	}
	else
	{
		//计算均值
		double tmp_sum(0);
		//对MDistance来说，第一位的MDistance是没用的，所以要-1
		for (int i = 0; i < len-1;i++)
		{
			tmp_sum += query.value(n).toDouble();
			query.previous();//指向前一个
		}
		return tmp_sum / len;
		
	}



	
}














//计算每个的均值与协方差，并且判定某个值是否还需要正常更新，还是变成定值，如果要被冻结，则会将需要更新到地形更新算法的数据使用定值替换
actDet DataBase::calcMeanandCov(actDet msg)
{

	bool latIsFrozen(0), lonIsFrozen(0), hgtIsFrozen(0);
	
	
	QSqlQuery query;//获得句柄
	QString serial = QString::number(msg.serial);
	QString msg_console = "Database:CALCmeanANDvar is working...";
	emit sigDBinfo(msg_console);
	QString message = "serial = " + serial;


	query.prepare("SELECT * FROM student WHERE serial = '" + serial + "'");  //这样能获得全部的包含serial的数据,应该是可以的，然后自然可以判断其长度.
																			 //query.prepare("SELECT * FROM student GROUP BY serial HAVING count(serial = '" + serial + "') > 20");  //这样能获得全部的包含serial的数据么？应该是可以的
	query.exec();
	int len_query = queryRowCount(query);//获取行数
										 //QSqlRecord record = query.record();
										 //int column = record.count(); //保存列数,第0列是列名
	qDebug() << "calc_len_query = " << len_query << endl;
	vector<actDet> result = {};

	
	
	//使用一张表serialFrozen，0表示未冻结，1表示已冻结


	
	if (len_query > CALC_LEN ) //如果row大于阈值数量
	{
		//判定以下这个值是否已经添加了，如果没添加就在表中初始化这个值
		//addFrozenMember(QString::number(msg.serial));
		latIsFrozen = judgeFrozen("lat", serial);
		lonIsFrozen = judgeFrozen("lon", serial);
		hgtIsFrozen = judgeFrozen("hgt", serial);

		
		double sum_lat_top = 0;
		double sum_lon_left = 0;
		double sum_hgt = 0;
		double sum_lat_bottom = 0;
		double sum_lon_right = 0;
		double sum_lat_top_var(0), sum_lon_left_var(0), sum_hgt_var(0);

		double mean_lat_top(0), mean_lon_left(0), mean_hgt(0), mean_lat_bottom(0), mean_lon_right(0);
		query.last();//先指向最后一个
		

		//计算目前值往前倒20个的均值和方差,是从后往前倒
		int initial_Pos = query.at(); //记录变化前位置
		for (int i = 0; i < CALC_LEN;i++) 
		{
			if(latIsFrozen ==0)
			{
				sum_lat_bottom += query.value(1).toDouble();
			    sum_lat_top += query.value(2).toDouble();
			}

			if (lonIsFrozen == 0)
			{
				sum_lon_left += query.value(3).toDouble();
				sum_lon_right += query.value(4).toDouble();
			}
			
			if (hgtIsFrozen == 0)
			{
				sum_hgt = query.value(5).toDouble();
			}
			
			query.previous();
		}

		//计算均值
		if(latIsFrozen == 0)
		{
			mean_lat_top = sum_lat_top / CALC_LEN;
			mean_lat_bottom = sum_lat_bottom / CALC_LEN;
		}
		if (lonIsFrozen == 0)
		{
			mean_lon_left = sum_lon_left / CALC_LEN;
			mean_lon_right = sum_lon_right / CALC_LEN;			
		}
		
		if (hgtIsFrozen == 0)
		{
			mean_hgt = sum_hgt / CALC_LEN;
		}
		
		

		query.seek(initial_Pos);//Point_Back
		//计算方差    var = 1/n(sum[(sample - mean)*2])
		for (int i = 0; i < CALC_LEN;i++)
		{
			if (latIsFrozen == 0)
			{
				sum_lat_top_var += calcVariance(mean_lat_top, query.value(2).toDouble());
			}
			if (lonIsFrozen == 0)
			{
				sum_lon_left_var += calcVariance(mean_lon_left, query.value(3).toDouble());
			}
			if (hgtIsFrozen == 0)
			{
				sum_hgt_var += calcVariance(mean_hgt, query.value(5).toDouble());
			}
			
			query.previous();
		}

		
		//得到方差值后，需要用其来判断是否可以冻结计算值，并且把均值作为其输出值，并不再更新
		//不再更新，但是仍然记录探测得到的值，但是在输出给地形更新算法时只输出经过这里计算的值
		//设计的时候应该是在地形更新处加一个判定，来判定这个值到底还有没有更新，如果被冻结了的话就取冻结值，如果没有被冻结的话就取传进来的值(一个选择机制)
		if (latIsFrozen == 0 )
		{
			double lat_top_var = sum_lat_top_var / CALC_LEN;
			
				msg.lat_top = mean_lat_top;
				msg.lat_bottom = mean_lat_bottom;
				//switchFrozenpara("lat", serial);			
				//emit sigDBinfo(message + "lat is Frozen.........");
		}
		
		if (lonIsFrozen == 0)
		{
			double lon_left_var = sum_lon_left_var / CALC_LEN;
						
			msg.lon_left = mean_lon_left;
			msg.lon_right = mean_lon_right;
			//switchFrozenpara("lon", serial);
			//emit sigDBinfo(message + "lon is Frozen.........");
			
		}
		
		if (hgtIsFrozen == 0)
		{
			double hgt_var = sum_hgt_var / CALC_LEN;
					
			msg.hgt = mean_hgt;
			//switchFrozenpara("hgt", serial);
			//emit sigDBinfo(message + "hgt is Frozen.........");
			
		}
		
		//需要一个01变量来储存是否已经冻结,这个变量是该serial自带的一个标签，但是在设计的时候就没考虑设计这个标签
		//如果已经被冻结了，就可以直接跳过这个步骤了吧，可以通过serial判定?那就需要给每个actDet添加一个判定值,虽然有必要但是还有没有更偷懒的办法?用一个全局hash表储存已经被冻结的serial，然后每个和这个序号进行对比（用hash表么，最简单的话就是用一个vector，储存的时候按照数字大小储存）

		return msg;
				
	}
	else
	{
		emit sigDBinfo(message + " is not Frozen YET .........");
		return msg;
	}

	
	
	//emit sigStartDEMUP(result);//发送数据
}





//添加该serial，如果已经添加在内，表示其起码有1个值被冻结，则停止添加
void DataBase::addFrozenMember(QString serial)
{
	QSqlQuery query;//获得句柄
	query.prepare("SELECT * FROM serialFrozen WHERE serial = '" + serial + "'");  //这样能获得全部的包含serial的数据,应该是可以的，然后自然可以判断其长度.
	query.exec();


	//判定长度，如果长度超过1，就证明该serial已经存在，就不需要再添加了
	int len_query = queryRowCount(query);//获取行数
	if(len_query > 0 )
	{
		//表示已经存在，那就直接结束
		return;
	}

	else//添加该serial
	{
		stu_id = serial.toInt();
		query.prepare("insert into serialFrozen (serial, lat, lon, hgt) \
									values (:serial,  :lat, :lon, hgt)");
		query.bindValue(":serial", stu_id);
		query.bindValue(":lat", 0);
		query.bindValue(":lon", 0);
		query.bindValue(":hgt", 0);
		//query.bindValue(":serial", stu_serial);
		query.exec();
	}
	
}



//查询serial下的某个para为1或0
bool DataBase::judgeFrozen(QString para, QString serial)
{
	QSqlQuery query;//获得句柄
	query.prepare("SELECT * FROM serialFrozen WHERE serial = '" + serial + "'");  //这样能获得全部的包含serial的数据,应该是可以的，然后自然可以判断其长度.
	query.exec();

	query.last();
	//query.previous();

	int judge = 0;
	if(para == "lat")
	{
		judge = query.value(1).toInt();
	}
	if (para == "lon")
	{
		judge = query.value(2).toInt();
	}
	if (para == "hgt")
	{
		judge = query.value(3).toInt();
	}

	return judge;
}



//修改某serial下的某个参数的冻结状态
void DataBase::switchFrozenpara(QString para, QString serial)
{
	
	QSqlQuery query;//获得句柄
	QString input;
	int id = serial.toInt();
	input = "update serialFrozen set " + para + "=? where serial =?";
	query.prepare(input);  //这样能获得全部的包含serial的数据
	query.addBindValue(1);//1表示已经冻结
	query.addBindValue(id);	
	query.exec();
		
}








//计算均方差
double DataBase::calcVariance(double mean , double sample)
{
	return (sample - mean)*(sample - mean);
}



