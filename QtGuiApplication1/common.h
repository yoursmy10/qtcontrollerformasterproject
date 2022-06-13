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
//图片像素长和高
#define IMG_WIDTH 512
#define IMG_HEIGHT 512
#define MAX_LEN 1500// 地形数据库长度
#define CAL_LEN 5 //数据库中如果相同serial的成员大于等于CAL_LEN，就需要对其进行计算，输出结果给地形更新算法进行更新
#define MAX_ANGULAR_VEL 10 //最大能接受的角速度，如果超过这个角速度，则其探测到的数据不予接收
#define PI 3.1415926
#define RAD2DEGREE 57.3 //一弧度约等于57.3°
#define CALC_LEN 20 //计算目前值往前倒CALC_LEN个这中间的总数
#define THRESHOLD_LAT 1.5e-9 //方差的阈值，lat
#define THRESHOLD_LON 5.4e-8 //方差的阈值，lon ，值是计算得到的
#define THRESHOLD_HGT 1e-7 //暂定
#define TRUNCTED_DENSITY 0.15 //截断概率密度





/**********缓冲区队列*********/
//Queue<info_recv> *queue_infoRecv_data;  //由通信线程产生,公共队列


//DEM结构体，单个地形高程数据储存结构
struct DEM
{
	double lat; //纬度
	double lon; //经度
	double hgt; //高程
};


/*
时间：2021/10/28
创建人：吴旭峰
用于更新地形高程数据库，
serial：障碍物序号。从视觉来
dr：相对距离。
theta：方位角。角度或弧度
dw：障碍物宽。从视觉来
hc：障碍物相对于摄像机中心点的高度。从视觉来
lat：拍摄该图片时直升机纬度。
lon：拍摄该图片时直升机经度。
alt：拍摄该图片时直升机纬度。
dem_base：地形高程数据库
*/
struct update_data
{
	int serial;//障碍物序号。从视觉来
	double dr;//相对距离,从雷达来
	double theta;//方位角。从雷达来
	double dw;//障碍物宽。通过雷达数据计算得到像素与实际尺寸之比
	double hc;//障碍物相对于摄像机中心点的高度。融合而来
	double lat;//拍摄该图片时直升机纬度
	double lon;//拍摄该图片时直升机经度
	double alt;//拍摄该图片时直升机无线电高
};



struct info_recv
{
	int serial; //序号
	int x_left; //左上点
	int y_top;
	int x_right;  //右下点
	int y_bottom;
	float distance;  //强行化为整数了，记得除哦 除1000000
	float yaw_angle;	//强行化为整数了，记得除哦 除1000
};



/*
时间：2021/10/28
创建人：吴旭峰
传感器信息，从主动探测端获取的
double lat;
double lon;
float alt_gps;//gps显示高度
float velocity_x;//速度
float velocity_y;
float velocity_z;
float angular_vel_x; //角速度，有个现象，角速度一快，预测结果就不准确，这是由跟踪算法决定的，但是可以试着用角速度来做一些限制
float angular_vel_y;
float angular_vel_z;
*/
struct SENSOR_DATA
{
	//GPS
	double lat; //纬度
	double lon; //经度
	float alt_gps;//gps显示高度
	float velocity_x;//速度
	float velocity_y;
	float velocity_z;

	//IMU
	float angular_vel_x; //角速度，有个现象，角速度一快，预测结果就不准确，这是由跟踪算法决定的，但是可以试着用角速度来做一些限制
	float angular_vel_y;
	float angular_vel_z;
	//float distance;
	float heli_yaw; //直升机偏航角
	//Barometer
	/*
	TODO 加入方位角和距离，不然还是只能靠假设 done
	*/
};

/*
时间：2021/12/10
创建人：吴旭峰
最终输入给地形更新算法的格式，是经纬度范围值以及高度值还有序号
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
时间：2022/3/16
创建人：吴旭峰
用来计算马氏距离的向量
*/
struct mahalanobisDistance
{
	double lon_left;
	double lat_top;
};








//安全队列
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
		value = data_queue.front();  //这句有问题
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

	int size()  //我自己加的
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



//一些全局变量

extern threadsafe_queue<info_recv> queue_infoRecv_data;  //由通信线程产生,公共队列
extern threadsafe_queue<SENSOR_DATA> queue_sensor_data;  //传感器数据队列，其实每次只接收一组数据
extern DEM dem_init[MAX_LEN][MAX_LEN]; ////先全部设为空,公共数组,第一维对应lat,第二维对应lon
extern double least_distance;  //least_distance暂时设为10m，单位为米
extern double ref_lat; //暂时给的(1m多少纬度)
extern double ref_lon;//某一确定经纬度地点的经纬度与米的换算(1m多少经度)
//extern double pix2meters; //像素与实际尺寸之比		
extern int stu_id;//id号，actDET结构使用，也用来表征时间先后
extern double lat_init;
extern double lon_init;
extern int insert_order; //tableview的应该插入行数
extern double GRAVITY;//重力加速度
extern QMutex m_mutex;

extern vector<vector<int>> serialFrozen;

/*
extern bool isFrozen_lat = 0; //该serial的lat值是否已经收敛，默认不收敛
extern bool isFrozen_lon = 0; //该serial的lon值是否已经收敛，默认不收敛
extern bool isFrozen_hgt = 0; //该serial的hgt值是否已经收敛，默认不收敛

*/




#endif