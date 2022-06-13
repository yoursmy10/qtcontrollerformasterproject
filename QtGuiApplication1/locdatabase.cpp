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
	myDB.close(); //�ر����ݿ�
	QFile::remove(DBFilePath); //ɾ�����ݿ⣬��Ҫ��¼����ʱ����ע�͵�

}


void DataBase::sltRunDB()  //��ʼ������database,��ʵ�������create��
{
	myDB = QSqlDatabase::addDatabase("QSQLITE");
	#if 1
	myDB.setDatabaseName(DBFilePath);//�����ļ�
	#endif
	#if 0
	myDB.setDatabaseName(":memory");  //���ڴ�������
	#endif
	
	if (!myDB.open())
	{
		qDebug() << "database not open..." << endl;
		emit sigDBinfo("database not open...");
	}

	connect(this, SIGNAL(sigCreateDB()), this, SLOT(createDB()));//���ú��Զ�createDB

	emit sigCreateDB();  //�������ݿ�
}



void DataBase::createDB()//����ʼ����ɵ�database�������ֱ�
{
	QSqlQuery query; //QSqlQuery���ṩִ�кͲ�����SQL���ķ��� query��һ�����

	//�����༶�� �����������к�Ϊid�����кţ��༶������    exec()->ִ��   �ƺ�����Ҫ�༶���ˣ�ֱ����ѧ����ͺ�
	//query.exec("create table class(serial int, sum int)"); 
	

	//����ѧ����,������id ��γ�ȷ�Χ �߳��Լ��༶��(serial)�����serial����������ڹ����༶��ѧ��
	query.exec("create table student(id int, lat_bottom varchar, lat_top varchar, lon_left varchar, lon_right varchar, hgt varchar, serial varchar)");
	//����һ����ͨ�˲����ݴ洢��
	query.exec("create table lowpass(id int, lat_bottom varchar, lat_top varchar, lon_left varchar, lon_right varchar, hgt varchar, serial varchar)");
	//����һ�����Ͼ������ݴ洢��
	query.exec("create table mahalanobis(serial varchar, mahalanobis_distance varchar, lon_left varchar, lat_top varchar)");
	
	//������ʾ����ŵ�ֵ�Ƿ��Ѿ�����
	query.exec("create table serialFrozen(serial int, lat int, lon int, hgt int)");


	//�������θ����㷨�������������,������id ��γ�ȷ�Χ �߳��Լ��༶��(serial)�����serial����������ڹ����༶��ѧ��
	query.exec("create table finaloutput(id int, lat_bottom varchar, lat_top varchar, lon_left varchar, lon_right varchar, hgt varchar, serial varchar)");
	query.exec("create table heliState(lat varchar, lon varchar, alt varchar, vx varchar, vy varchar, vz varchar)");

	query.exec("create table displayoutput(id int, lat_bottom varchar, lat_top varchar, lon_left varchar, lon_right varchar, hgt varchar, serial varchar)");
	query.exec("create table mahala(id int, lat_bottom varchar, lat_top varchar, lon_left varchar, lon_right varchar, hgt varchar, serial varchar, mahaDistance varchar)");

	
}



void DataBase::closeDB()
{
	myDB.close();
}




//���������ݣ�����member�����ݸ��°༶��ѧ�������û�а༶�ʹ����༶��Ȼ���ٴ���ѧ��
void DataBase::addMember(actDet member)
{
	QString msg_console = "Database:addMember is working...";
	emit sigDBinfo(msg_console);
	
	//��ֵ
	QString stu_serial = QString("%1").arg(member.serial);
	//QString lat_bottom = QString("%1").arg(member.lat_bottom);
	QString lat_bottom = QString::number(member.lat_bottom, 10,10);
	QString lat_top = QString::number(member.lat_top, 10, 10);
	QString lon_left = QString::number(member.lon_left, 10, 10);
	QString lon_right = QString::number(member.lon_right,10,10);
	QString hgt = QString::number(member.hgt,10,10);
	QString stu_id = QString("%1").arg(member.id);
	

	//�ж����޿���Ϣ
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
		//ֱ�Ӳ���stu
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






//��һ�ֶ�������ĸо�
void DataBase::addMember_display(actDet member)
{
	QString msg_console = "Database:addMember_display is working...";
	emit sigDBinfo(msg_console);

	//��ֵ
	QString stu_serial = QString("%1").arg(member.serial);
	//QString lat_bottom = QString("%1").arg(member.lat_bottom);
	QString lat_bottom = QString::number(member.lat_bottom, 10, 10);
	QString lat_top = QString::number(member.lat_top, 10, 10);
	QString lon_left = QString::number(member.lon_left, 10, 10);
	QString lon_right = QString::number(member.lon_right, 10, 10);
	QString hgt = QString::number(member.hgt, 10, 10);
	QString stu_id = QString("%1").arg(member.id);


	//�ж����޿���Ϣ
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
		//ֱ�Ӳ���stu
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






//��һ�ֶ�������ĸо�
void DataBase::addMember_mahanew(actDet member, double maha)
{
	QString msg_console = "Database:addMember_display is working...";
	emit sigDBinfo(msg_console);


	
	//��ֵ
	QString stu_serial = QString("%1").arg(member.serial);
	//QString lat_bottom = QString("%1").arg(member.lat_bottom);
	QString lat_bottom = QString::number(member.lat_bottom, 10, 10);
	QString lat_top = QString::number(member.lat_top, 10, 10);
	QString lon_left = QString::number(member.lon_left, 10, 10);
	QString lon_right = QString::number(member.lon_right, 10, 10);
	QString hgt = QString::number(member.hgt, 10, 10);
	QString stu_id = QString("%1").arg(member.id);
	QString maha_dis = QString::number(maha, 10, 10);


	//�ж����޿���Ϣ
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
		//ֱ�Ӳ���stu
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











//���������ݣ�ֱ�����˶��켣
void DataBase::addMember_helistate(SENSOR_DATA member)
{
	QString msg_console = "Database:HeliCopterState Record is working...";
	emit sigDBinfo(msg_console);

	//��ֵ
	//QString lat = QString("%1").arg(member.lat);
	//QString lat_bottom = QString("%1").arg(member.lat_bottom);
	QString lat = QString::number(member.lat, 10, 10);
	QString lon = QString::number(member.lon, 10, 10);
	QString alt = QString::number(member.alt_gps, 10, 10);
	QString vx = QString::number(member.velocity_x, 10, 10);
	QString vy = QString::number(member.velocity_y, 10, 10);
	QString vz = QString::number(member.velocity_z, 10, 10);
	


	//�ж����޿���Ϣ
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
		//ֱ�Ӳ���stu
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








//���������ݣ�����member�����ݸ��°༶��ѧ�������û�а༶�ʹ����༶��Ȼ���ٴ���ѧ��
void DataBase::addMember_final(actDet member)
{
	QString msg_console = "Database:addMember is working...";
	emit sigDBinfo(msg_console);

	//��ֵ
	QString stu_serial = QString("%1").arg(member.serial);
	//QString lat_bottom = QString("%1").arg(member.lat_bottom);
	QString lat_bottom = QString::number(member.lat_bottom, 10, 10);
	QString lat_top = QString::number(member.lat_top, 10, 10);
	QString lon_left = QString::number(member.lon_left, 10, 10);
	QString lon_right = QString::number(member.lon_right, 10, 10);
	QString hgt = QString::number(member.hgt, 10, 10);
	QString stu_id = QString("%1").arg(member.id);


	//�ж����޿���Ϣ
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
		//ֱ�Ӳ���stu
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
	emit startGet(stu_serial);//�������sig��ζ����������getClass

}






//��ӵ�ͨ�˲���ÿ�μ��������������ݵ�lowpass����,actDet�е�serial������̽���еõ���id�ž���
void DataBase::addMember_lowpass(actDet member)
{
	QString msg_console = "Database:addMember_lowpass is working...";
	emit sigDBinfo(msg_console);

	//��ֵ
	QString stu_serial = QString("%1").arg(member.serial);
	//QString lat_bottom = QString("%1").arg(member.lat_bottom);
	QString lat_bottom = QString::number(member.lat_bottom, 10, 10);
	QString lat_top = QString::number(member.lat_top, 10, 10);
	QString lon_left = QString::number(member.lon_left, 10, 10);
	QString lon_right = QString::number(member.lon_right, 10, 10);
	QString hgt = QString::number(member.hgt, 10, 10);
	QString stu_id = QString("%1").arg(member.id);


	//�ж����޿���Ϣ
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
		//ֱ�Ӳ���lowpass��
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




//��Ӽ���õ������Ͼ���ÿ�μ��������������ݵ�mahalanobis����
void DataBase::addMember_maha(actDet msg, double maha_dist)
{
	QString msg_console = "Database:addMember_maha is working...";
	qDebug() << msg_console << endl;
	emit sigDBinfo(msg_console);

	//��ֵ

	QString maha_serial = QString("%1").arg(msg.serial);
	QString maha = QString::number(maha_dist, 10, 10);
	QString lat_top = QString::number(msg.lat_top, 10, 10);
	QString lon_left = QString::number(msg.lon_left, 10, 10);
	
	//QString maha = maha;


	//�ж����޿���Ϣ
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
		//ֱ�Ӳ���lowpass��
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





//�������ĳ��ŵ����µĵ�ͨ�˲�����
actDet DataBase::getNewest_lowpass(QString serial)
{
	QSqlQuery query;//��þ��
	QString msg_console = "Database:getNewest_lowpass is working...";
	qDebug() << msg_console;
	emit sigDBinfo(msg_console);


	query.prepare("SELECT * FROM lowpass WHERE serial = '" + serial + "'");  //�����ܻ��ȫ���İ���serial������,Ӧ���ǿ��Եģ�Ȼ����Ȼ�����ж��䳤��.
																			 //query.prepare("SELECT * FROM student GROUP BY serial HAVING count(serial = '" + serial + "') > 20");  //�����ܻ��ȫ���İ���serial������ô��Ӧ���ǿ��Ե�
	query.exec();
	int len_query = queryRowCount(query);//��ȡ����
										 //QSqlRecord record = query.record();
										 //int column = record.count(); //��������,��0��������
	qDebug() << "lowpass_len_query = " << len_query << endl;

	actDet tmp;
	//ֱ��ָ�����µĸ����кŵ�����
	query.last();
	//query.previous();
	
	tmp.id = query.value(0).toInt();
	tmp.lat_bottom = query.value(1).toDouble();
	tmp.lat_top = query.value(2).toDouble();
	tmp.lon_left = query.value(3).toDouble();
	tmp.lon_right = query.value(4).toDouble();
	tmp.hgt = query.value(5).toDouble();
	tmp.serial = query.value(6).toInt();

	return tmp;//���ص�k-1��ֵ
	
	//qDebug() << "tmp.serial = " << tmp.serial << endl;
}



//�ж�lowpass����ĳ���ֵ�����Ƿ�Ϊ��
bool DataBase::tableIsEmplty(QString serial, QString table)
{
	QSqlQuery query;//��þ��
	QString send;
	send = "SELECT * FROM " + table + " WHERE serial = '" + serial + "'";
	query.prepare(send);
	qDebug() << "tableisempty is working..." << endl;
	//query.prepare("SELECT * FROM lowpass WHERE serial = '" + serial + "'");
	query.exec();
	int len_query = queryRowCount(query);//��ȡ����

	if(len_query  == 0)
	{
		return 1; //һ�۶��룬����Ϊ��
	}
	else
	{
		return 0; //һ�۶��룬����Ϊ��
	}


}











//�������ĳ��ŵ����µ����Ͼ�������
mahalanobisDistance DataBase::getNewest_mahalanobis(QString serial)
{
	QSqlQuery query;//��þ��
	QString msg_console = "Database:getNewest_mahalaobisn is working...";
	emit sigDBinfo(msg_console);


	query.prepare("SELECT * FROM mahala WHERE serial = '" + serial + "'");  //�����ܻ��ȫ���İ���serial������,Ӧ���ǿ��Եģ�Ȼ����Ȼ�����ж��䳤��.
																			 //query.prepare("SELECT * FROM student GROUP BY serial HAVING count(serial = '" + serial + "') > 20");  //�����ܻ��ȫ���İ���serial������ô��Ӧ���ǿ��Ե�
	query.exec();
	int len_query = queryRowCount(query);//��ȡ����
										 //QSqlRecord record = query.record();
										 //int column = record.count(); //��������,��0��������
	qDebug() << "len_query = " << len_query << endl;

	mahalanobisDistance res;
	//ֱ��ָ�����µĸ����кŵ�����
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





//���������鷳��һ���˰ɣ�Ҫʵʱ����ͳ�������ӵ������Ա�����кţ��������ֵ�˾�Ҫ���ý��м���������Ҫ��ʱ���
void DataBase::getClass(QString serial)
{
	QSqlQuery query;//��þ��
	QString msg_console = "Database:getClass is working...";
	emit sigDBinfo(msg_console);
	
	//query.prepare("SELECT * FROM student WHERE serial = '" + serial + "'");  //�����ܻ��ȫ���İ���serial������,Ӧ���ǿ��Եģ�Ȼ����Ȼ�����ж��䳤��.
	query.prepare("SELECT * FROM finaloutput WHERE serial = '"+ serial +"'");  //�����ܻ��ȫ���İ���serial������,Ӧ���ǿ��Եģ�Ȼ����Ȼ�����ж��䳤��.
	//query.prepare("SELECT * FROM student GROUP BY serial HAVING count(serial = '" + serial + "') > 20");  //�����ܻ��ȫ���İ���serial������ô��Ӧ���ǿ��Ե�
	query.exec();
	int len_query = queryRowCount(query);//��ȡ����
	//QSqlRecord record = query.record();
	//int column = record.count(); //��������,��0��������
	qDebug() << "len_query = " << len_query << endl; 
	vector<actDet> result = {};

	
	int count_order = 0;//֮ǰ���������
	int initial_Pos = query.at(); //��¼�仯ǰλ��
	query.next(); //point to the valid_value
	//count_order = std::count(history.begin(), history.end(), query.value(6).toInt()) + 1;  //֮ǰ���������
	count_order = count_num(history, query.value(6).toInt()) + 1;
	

	
	//���count_order����
	emit sigDBinfo("count_order = "+QString::number(count_order)+"\n"+"query.value(6).toInt() =  " + QString::number(query.value(6).toInt()));
	query.seek(initial_Pos);//Point_Back

	
	if (len_query > CAL_LEN*count_order) //���row������ֵ����
	{		
		
		actDet tmp;
		//��ȡ���ݿ������и�serial������
		query.next();
		history.push_back(query.value(6).toInt()); //���ڼ���
		query.seek(initial_Pos);
		
		while (query.next())  //���������
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
		
		//���ˣ��͵õ��˴ﵽ��ֵ������serial�Ķ���
	}
	else
	{
		
		return;
		//return�˾Ͳ�emitSIGNAL��
	}
	emit sigStartDEMUP(result);//��������

}


//����ɸѡ�������
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





//����[front, last]�а���serial��Ԫ�ظ���
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
ʱ�䣺2022/03/23
�����ˣ������
�����ֵ
serial : serial
table : ����
len : ��Ҫ����Ķ��г���
n: �������ڸñ�ĵڼ���
*/
double DataBase::calcMean(QString serial, QString table, int len, int n)
{
	QSqlQuery query;//��þ��
	QString send;
	send = "SELECT * FROM " + table + " WHERE serial = '" + serial + "'";
	query.prepare(send);
	qDebug() << "calcMEAN is working..." << endl;
	//query.prepare("SELECT * FROM lowpass WHERE serial = '" + serial + "'");
	query.exec();
	query.last();//ָ�����һ��

	
	int len_query = queryRowCount(query);//��ȡ����
	//������Ȳ����
	if(len_query < len)
	{
		return -1;
	}
	else
	{
		//�����ֵ
		double tmp_sum(0);
		//��MDistance��˵����һλ��MDistance��û�õģ�����Ҫ-1
		for (int i = 0; i < len-1;i++)
		{
			tmp_sum += query.value(n).toDouble();
			query.previous();//ָ��ǰһ��
		}
		return tmp_sum / len;
		
	}



	
}














//����ÿ���ľ�ֵ��Э��������ж�ĳ��ֵ�Ƿ���Ҫ�������£����Ǳ�ɶ�ֵ�����Ҫ�����ᣬ��Ὣ��Ҫ���µ����θ����㷨������ʹ�ö�ֵ�滻
actDet DataBase::calcMeanandCov(actDet msg)
{

	bool latIsFrozen(0), lonIsFrozen(0), hgtIsFrozen(0);
	
	
	QSqlQuery query;//��þ��
	QString serial = QString::number(msg.serial);
	QString msg_console = "Database:CALCmeanANDvar is working...";
	emit sigDBinfo(msg_console);
	QString message = "serial = " + serial;


	query.prepare("SELECT * FROM student WHERE serial = '" + serial + "'");  //�����ܻ��ȫ���İ���serial������,Ӧ���ǿ��Եģ�Ȼ����Ȼ�����ж��䳤��.
																			 //query.prepare("SELECT * FROM student GROUP BY serial HAVING count(serial = '" + serial + "') > 20");  //�����ܻ��ȫ���İ���serial������ô��Ӧ���ǿ��Ե�
	query.exec();
	int len_query = queryRowCount(query);//��ȡ����
										 //QSqlRecord record = query.record();
										 //int column = record.count(); //��������,��0��������
	qDebug() << "calc_len_query = " << len_query << endl;
	vector<actDet> result = {};

	
	
	//ʹ��һ�ű�serialFrozen��0��ʾδ���ᣬ1��ʾ�Ѷ���


	
	if (len_query > CALC_LEN ) //���row������ֵ����
	{
		//�ж��������ֵ�Ƿ��Ѿ�����ˣ����û��Ӿ��ڱ��г�ʼ�����ֵ
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
		query.last();//��ָ�����һ��
		

		//����Ŀǰֵ��ǰ��20���ľ�ֵ�ͷ���,�ǴӺ���ǰ��
		int initial_Pos = query.at(); //��¼�仯ǰλ��
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

		//�����ֵ
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
		//���㷽��    var = 1/n(sum[(sample - mean)*2])
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

		
		//�õ�����ֵ����Ҫ�������ж��Ƿ���Զ������ֵ�����ҰѾ�ֵ��Ϊ�����ֵ�������ٸ���
		//���ٸ��£�������Ȼ��¼̽��õ���ֵ����������������θ����㷨ʱֻ���������������ֵ
		//��Ƶ�ʱ��Ӧ�����ڵ��θ��´���һ���ж������ж����ֵ���׻���û�и��£�����������˵Ļ���ȡ����ֵ�����û�б�����Ļ���ȡ��������ֵ(һ��ѡ�����)
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
		
		//��Ҫһ��01�����������Ƿ��Ѿ�����,��������Ǹ�serial�Դ���һ����ǩ����������Ƶ�ʱ���û������������ǩ
		//����Ѿ��������ˣ��Ϳ���ֱ��������������˰ɣ�����ͨ��serial�ж�?�Ǿ���Ҫ��ÿ��actDet���һ���ж�ֵ,��Ȼ�б�Ҫ���ǻ���û�и�͵���İ취?��һ��ȫ��hash�����Ѿ��������serial��Ȼ��ÿ���������Ž��жԱȣ���hash��ô����򵥵Ļ�������һ��vector�������ʱ�������ִ�С���棩

		return msg;
				
	}
	else
	{
		emit sigDBinfo(message + " is not Frozen YET .........");
		return msg;
	}

	
	
	//emit sigStartDEMUP(result);//��������
}





//��Ӹ�serial������Ѿ�������ڣ���ʾ��������1��ֵ�����ᣬ��ֹͣ���
void DataBase::addFrozenMember(QString serial)
{
	QSqlQuery query;//��þ��
	query.prepare("SELECT * FROM serialFrozen WHERE serial = '" + serial + "'");  //�����ܻ��ȫ���İ���serial������,Ӧ���ǿ��Եģ�Ȼ����Ȼ�����ж��䳤��.
	query.exec();


	//�ж����ȣ�������ȳ���1����֤����serial�Ѿ����ڣ��Ͳ���Ҫ�������
	int len_query = queryRowCount(query);//��ȡ����
	if(len_query > 0 )
	{
		//��ʾ�Ѿ����ڣ��Ǿ�ֱ�ӽ���
		return;
	}

	else//��Ӹ�serial
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



//��ѯserial�µ�ĳ��paraΪ1��0
bool DataBase::judgeFrozen(QString para, QString serial)
{
	QSqlQuery query;//��þ��
	query.prepare("SELECT * FROM serialFrozen WHERE serial = '" + serial + "'");  //�����ܻ��ȫ���İ���serial������,Ӧ���ǿ��Եģ�Ȼ����Ȼ�����ж��䳤��.
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



//�޸�ĳserial�µ�ĳ�������Ķ���״̬
void DataBase::switchFrozenpara(QString para, QString serial)
{
	
	QSqlQuery query;//��þ��
	QString input;
	int id = serial.toInt();
	input = "update serialFrozen set " + para + "=? where serial =?";
	query.prepare(input);  //�����ܻ��ȫ���İ���serial������
	query.addBindValue(1);//1��ʾ�Ѿ�����
	query.addBindValue(id);	
	query.exec();
		
}








//���������
double DataBase::calcVariance(double mean , double sample)
{
	return (sample - mean)*(sample - mean);
}



