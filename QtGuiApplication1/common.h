#pragma once
#ifndef COMMON_H_
#define COMMON_H_


#include <QMutex>
#include <atomic>
#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <iostream>
#include <string>


using namespace std;

#define MAX 10000
//ͼƬ���س��͸�
#define IMG_WIDTH 512
#define IMG_HEIGHT 512
#define MAX_LEN 1500// �������ݿⳤ��
#define CAL_LEN 5 //���ݿ��������ͬserial�ĳ�Ա���ڵ���CAL_LEN������Ҫ������м��㣬�����������θ����㷨���и���
#define MAX_ANGULAR_VEL 10 //����ܽ��ܵĽ��ٶȣ��������������ٶȣ�����̽�⵽�����ݲ������
#define PI 3.1415926
#define RAD2DEGREE 57.3 //һ����Լ����57.3��
#define CALC_LEN 20 //����Ŀǰֵ��ǰ��CALC_LEN�����м������
#define THRESHOLD_LAT 1.5e-9 //�������ֵ��lat
#define THRESHOLD_LON 5.4e-8 //�������ֵ��lon ��ֵ�Ǽ���õ���
#define THRESHOLD_HGT 1e-7 //�ݶ�
#define TRUNCTED_DENSITY 0.15 //�ضϸ����ܶ�





/**********����������*********/
//Queue<info_recv> *queue_infoRecv_data;  //��ͨ���̲߳���,��������


//DEM�ṹ�壬�������θ߳����ݴ���ṹ
struct DEM
{
	double lat; //γ��
	double lon; //����
	double hgt; //�߳�
};


/*
ʱ�䣺2021/10/28
�����ˣ������
���ڸ��µ��θ߳����ݿ⣬
serial���ϰ�����š����Ӿ���
dr����Ծ��롣
theta����λ�ǡ��ǶȻ򻡶�
dw���ϰ�������Ӿ���
hc���ϰ����������������ĵ�ĸ߶ȡ����Ӿ���
lat�������ͼƬʱֱ����γ�ȡ�
lon�������ͼƬʱֱ�������ȡ�
alt�������ͼƬʱֱ����γ�ȡ�
dem_base�����θ߳����ݿ�
*/
struct update_data
{
	int serial;//�ϰ�����š����Ӿ���
	double dr;//��Ծ���,���״���
	double theta;//��λ�ǡ����״���
	double dw;//�ϰ����ͨ���״����ݼ���õ�������ʵ�ʳߴ�֮��
	double hc;//�ϰ����������������ĵ�ĸ߶ȡ��ں϶���
	double lat;//�����ͼƬʱֱ����γ��
	double lon;//�����ͼƬʱֱ��������
	double alt;//�����ͼƬʱֱ�������ߵ��
};



struct info_recv
{
	int serial; //���
	int x_left; //���ϵ�
	int y_top;
	int x_right;  //���µ�
	int y_bottom;
	float distance;  //ǿ�л�Ϊ�����ˣ��ǵó�Ŷ ��1000000
	float yaw_angle;	//ǿ�л�Ϊ�����ˣ��ǵó�Ŷ ��1000
};



/*
ʱ�䣺2021/10/28
�����ˣ������
��������Ϣ��������̽��˻�ȡ��
double lat;
double lon;
float alt_gps;//gps��ʾ�߶�
float velocity_x;//�ٶ�
float velocity_y;
float velocity_z;
float angular_vel_x; //���ٶȣ��и����󣬽��ٶ�һ�죬Ԥ�����Ͳ�׼ȷ�������ɸ����㷨�����ģ����ǿ��������ý��ٶ�����һЩ����
float angular_vel_y;
float angular_vel_z;
*/
struct SENSOR_DATA
{
	//GPS
	double lat; //γ��
	double lon; //����
	float alt_gps;//gps��ʾ�߶�
	float velocity_x;//�ٶ�
	float velocity_y;
	float velocity_z;

	//IMU
	float angular_vel_x; //���ٶȣ��и����󣬽��ٶ�һ�죬Ԥ�����Ͳ�׼ȷ�������ɸ����㷨�����ģ����ǿ��������ý��ٶ�����һЩ����
	float angular_vel_y;
	float angular_vel_z;
	//float distance;
	float heli_yaw; //ֱ����ƫ����
	//Barometer
	/*
	TODO ���뷽λ�Ǻ;��룬��Ȼ����ֻ�ܿ����� done
	*/
};

/*
ʱ�䣺2021/12/10
�����ˣ������
������������θ����㷨�ĸ�ʽ���Ǿ�γ�ȷ�Χֵ�Լ��߶�ֵ�������
*/
struct actDet
{
	double lat_bottom;
	double lat_top;
	double lon_left;
	double lon_right;
	double hgt;
	int serial;
	int id;
};



/*
ʱ�䣺2022/3/16
�����ˣ������
�����������Ͼ��������
*/
struct mahalanobisDistance
{
	double lon_left;
	double lat_top;
};








//��ȫ����
template<typename T>
class threadsafe_queue {
private:
	mutable std::mutex mut;
	std::queue<T> data_queue;
	std::condition_variable data_cond;
public:
	threadsafe_queue() {}
	threadsafe_queue(threadsafe_queue const& other) {
		std::lock_guard<std::mutex> lk(other.mut);
		data_queue = other.data_queue;
	}

	void push(T new_value) {
		std::lock_guard<std::mutex> lk(mut);
		data_queue.push(new_value);
		data_cond.notify_one();
	}

	void wait_and_pop(T& value) {
		std::unique_lock<std::mutex> lk(mut);
		data_cond.wait(lk, [this] {return !data_queue.empty(); });
		value = data_queue.front();  //���������
		data_queue.pop();
	}

	std::shared_ptr<T> wait_and_pop() {
		std::unique_lock<std::mutex> lk(mut);
		data_cond.wait(lk, [this] {return !data_queue.empty(); });
		std::shared_ptr<T> res(std::make_shared<T>(data_queue.front()));
		data_queue.pop();
		return res;
	}

	bool try_pop(T& value) {
		std::lock_guard<std::mutex> lk(mut);
		if (data_queue.empty())
			return false;
		value = data_queue.front();
		data_queue.pop();
		return true;
	}

	std::shared_ptr<T> try_pop() {
		std::lock_guard<std::mutex> lk(mut);
		if (data_queue.empty())
			return std::shared_ptr<T>();
		std::shared_ptr<T> res(std::make_shared<T>(data_queue.front()));
		data_queue.pop();
		return res;
	}

	bool empty() const {
		std::lock_guard<std::mutex> lk(mut);
		return data_queue.empty();
	}

	int size()  //���Լ��ӵ�
	{
		std::lock_guard<std::mutex> lk(mut);
		return data_queue.size();
	}

	void clear() {
		std::lock_guard<std::mutex> lk(mut);
		queue<T> empty;
		swap(empty, data_queue);
	}
};



//һЩȫ�ֱ���

extern threadsafe_queue<info_recv> queue_infoRecv_data;  //��ͨ���̲߳���,��������
extern threadsafe_queue<SENSOR_DATA> queue_sensor_data;  //���������ݶ��У���ʵÿ��ֻ����һ������
extern DEM dem_init[MAX_LEN][MAX_LEN]; ////��ȫ����Ϊ��,��������,��һά��Ӧlat,�ڶ�ά��Ӧlon
extern double least_distance;  //least_distance��ʱ��Ϊ10m����λΪ��
extern double ref_lat; //��ʱ����(1m����γ��)
extern double ref_lon;//ĳһȷ����γ�ȵص�ľ�γ�����׵Ļ���(1m���پ���)
//extern double pix2meters; //������ʵ�ʳߴ�֮��		
extern int stu_id;//id�ţ�actDET�ṹʹ�ã�Ҳ��������ʱ���Ⱥ�
extern double lat_init;
extern double lon_init;
extern int insert_order; //tableview��Ӧ�ò�������
extern double GRAVITY;//�������ٶ�
extern QMutex m_mutex;

extern vector<vector<int>> serialFrozen;

/*
extern bool isFrozen_lat = 0; //��serial��latֵ�Ƿ��Ѿ�������Ĭ�ϲ�����
extern bool isFrozen_lon = 0; //��serial��lonֵ�Ƿ��Ѿ�������Ĭ�ϲ�����
extern bool isFrozen_hgt = 0; //��serial��hgtֵ�Ƿ��Ѿ�������Ĭ�ϲ�����

*/




#endif