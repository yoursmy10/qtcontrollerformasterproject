#include "lookahead_alert.h"


LookAheadAlert::LookAheadAlert(QObject *parent):QObject(parent)
{
	
	
}

LookAheadAlert::~LookAheadAlert()
{	
	vertices.clear();
}




//运行告警线程,用定时器触发，固定频率执行一次
void LookAheadAlert::sltRunAlert(double lat, double lon, double yaw_heli, double heli_hgt, double velocity_x, double velocity_y)
{

	emit sigConsoleInfo("Look_Ahead ALERT is working...");
	//m_mutex.lock();

	//double velocity = std::sqrt(pow(velocity_x,velocity_x) + pow(velocity_y, velocity_y)); //这个是xy平面上的速度,同一个值在显示和这里都分别计算了一次，非常费资源
	double velocity = std::sqrt(velocity_x*velocity_x + velocity_y*velocity_y);
	double LAD_sim = LAD_generate(velocity); //根据前飞速度生成前视距离,velocity为xy平面上的和向量
	QString lad = QString("LAD = %1").arg(LAD_sim);
	//倒车的时候就不要告警了，添乱
	if(LAD_sim<=0)
	{
		emit sigConsoleInfo("LAD<0, alert modal muted...");
		return;
	}
	emit sigConsoleInfo(lad);
	
	terrain_scan(lat, lon, yaw_heli, LAD_sim, heli_hgt);
	emit sigConsoleInfo("terrain_scan done...");
	
	//m_mutex.unlock();
}







//生成前视距离
double LookAheadAlert::LAD_generate(double velocity)
{
	
	//根据公式：LAD = (V*V)/2gtan(alpha) + V*T
	double LAD_res = 0;
	emit sigConsoleInfo("LAD_generate is working...");
	//加绝对值，因为有速度是矢量有正负
	LAD_res = abs(((abs(velocity) * abs(velocity))) / (2 * GRAVITY*(std::tan(alpha / RAD2DEGREE))) + abs(velocity) * react_time);
	emit sigEmitLAD(LAD_res);//发送LAD给主界面
	return LAD_res;
}





//对直升机当前所处位置前方进行地形扫描，并通过高程进行对比，lat,lon:直升机当前位置,还要考虑机头朝向。考虑应用扫描线算法
//建议：原版terrainscan查询的是二维数组，这是浪费效率的，既然已经有了数据库，就应该直接用数据库中提取到的数据，那么应该更改的是findLOC函数
void LookAheadAlert::terrain_scan(double lat, double lon, double yaw_heli, double LAD, double heli_hgt)
{
	//1.-----------------根据当前位置找到直升机前方区域在地形高程图中的坐标------------------------------
	//调用地形高程图进行扫描，确定直升机当前位置O(2n)
	int m_init = 0; //m for lat
	int n_init = 0; //n for lon


	vector<int> tmp;
	tmp = findLoc(lat, lon);
	m_init = tmp[0]; //找不到位置
	
	n_init = tmp[1];
	//如果超出范围就直接返回
	if(m_init == -1 || n_init ==-1)
	{
		return;
	}
	tmp.clear();
	
	//qDebug()<<"helicopter location in dem map successfully updated..." << endl;
	//至此得到直升机在地形高程图中的初始坐标
	/*
	 
	 A-------------------B
	 |    SCANNING       |
	 |      AREA	     |
	 |                   |
	 C-------------------D
		  ORIENTATION
			 Heli
	*/

	//直升机朝向左下角点C
	double lon_lb = lon + ref_lon*LAD_width*sin(yaw_heli / RAD2DEGREE) / 2;
	double lat_lb = lat - ref_lat*LAD_width*cos(yaw_heli / RAD2DEGREE) / 2;
	tmp = findLoc(lat_lb, lon_lb);
	int m_lb = tmp[0];
	int n_lb = tmp[1];
	vertices.push_back(Point(m_lb, n_lb));
	tmp.clear();
	//直升机朝向右下角点D
	double lon_rb = lon - ref_lon*LAD_width*sin(yaw_heli / RAD2DEGREE) / 2;
	double lat_rb = lat + ref_lat*LAD_width*cos(yaw_heli / RAD2DEGREE) / 2;
	tmp = findLoc(lat_rb, lon_rb);
	int m_rb = tmp[0];
	int n_rb = tmp[1];
	vertices.push_back(Point(m_rb, n_rb));
	tmp.clear();
	//直升机朝向左上角点A
	double lon_lt = lon + ref_lon*LAD*cos(yaw_heli / RAD2DEGREE) + ref_lon*LAD_width*sin(yaw_heli / RAD2DEGREE) / 2;
	double lat_lt = lat + ref_lat*LAD*sin(yaw_heli / RAD2DEGREE) - ref_lat*LAD_width*cos(yaw_heli / RAD2DEGREE) / 2;
	tmp = findLoc(lat_lt, lon_lt);
	int m_lt = tmp[0];
	int n_lt = tmp[1];
	vertices.push_back(Point(m_lt, n_lt));
	tmp.clear();
	//直升机朝向右上角点B
	double lon_rt = lon + ref_lon*LAD*cos(yaw_heli / RAD2DEGREE) - ref_lon*LAD_width*sin(yaw_heli / RAD2DEGREE) / 2;
	double lat_rt = lat + ref_lat*LAD*sin(yaw_heli / RAD2DEGREE) + ref_lat*LAD_width*cos(yaw_heli / RAD2DEGREE) / 2;
	tmp = findLoc(lat_rt, lon_rt);
	int m_rt = tmp[0];
	int n_rt = tmp[1];
	vertices.push_back(Point(m_rt, n_rt));
	tmp.clear();

	double safe_alt = delta_h * heli_hgt; //障碍物安全高度
	//至此得到四个角点的经纬度以及DEM图中坐标
	//2.-----------------求解直升机前向需要扫描区域的四个角点坐标，并且建立边表（ET）及活动边表(AET)------------------
	//3.-----------------开始扫描，并对比每一格中的高程信息,如果遇到告警，输出告警信息------------------------------
	polygon_scan(vertices, safe_alt);
	vertices.clear();//每执行一次都要清除一次
}






//如果存在碰撞风险，就返回1，否则返回0
int LookAheadAlert::colli_pred(double safe_height, double dem_hgt)
{
	if(safe_height <= dem_hgt)
	{
		emit sigColliAlert(1);//发送信号给告警指示器
		return 1;
	}
	else
	{
		emit sigColliAlert(0);
		return 0;
	}
}





//返回相应经纬度在地形高程图中的坐标值
//error:数组索引超出界限
//不应该再使用二维数组了，因为对前方建筑物的原则应该是只要不在几个建筑物之间就好，直接上数据库，不在建筑物范围内的全部不算
//TODO 处理点在界外的情况,一般不会出现了
vector<int> LookAheadAlert::findLoc(double lat, double lon)
{
	int m_init = 0;
	vector<int> res;
	//判定出界的情况
	if(lat<dem_init[0][0].lat||lat>dem_init[MAX_LEN-1][0].lat)
	{
		emit sigConsoleInfo("heli_lat out of range...");
		return { -1,-1 };
	}
	
	if (lon<dem_init[0][0].lon || lon>dem_init[0][MAX_LEN - 1].lon)
	{
		emit sigConsoleInfo("heli_lon out of range...");
		return{ -1,-1 };
	}

	
	for (int i = 0; i < MAX_LEN; i++)
	{
		if (std::abs(dem_init[i][0].lat - lat) < least_distance*ref_lat)
		{
			res.push_back(i);
			m_init = i;
			break;
		}
	}
	for (int j = 0; j < MAX_LEN; j++)
	{
		if (std::abs(dem_init[m_init][j].lon - lon) < least_distance*ref_lon)
		{
			res.push_back(j);
			break;
		}

	}
	return res;
}



void LookAheadAlert::polygon_scan(vector<Point> vertices, double safe_height)
{
	//计算最高点的y坐标
	int maxY = 0;
	for (int i = 0; i<vertices.size(); i++)
	{
		if (vertices[i].y > maxY)
		{
			maxY = vertices[i].y;
		}
	}
	//初始化ET和AET
	Edge *pET[MAX_LEN];
	for (int i = 0; i<maxY; i++)
	{
		pET[i] = new Edge();
		pET[i]->next = nullptr;
	}
	AET = new Edge();
	AET->next = nullptr;


	//建立边表ET
	for (int i = 0; i<vertices.size(); i++)
	{
		//取出当前点1前后相邻的共4个点，点1与点2的连线作为本次循环处理的边，另外两个点点0和点3用于计算奇点
		int x0 = vertices[(i - 1 + vertices.size()) % vertices.size()].x;
		int x1 = vertices[i].x;
		int x2 = vertices[(i + 1) % vertices.size()].x;
		int x3 = vertices[(i + 2) % vertices.size()].x;
		int y0 = vertices[(i - 1 + vertices.size()) % vertices.size()].y;
		int y1 = vertices[i].y;
		int y2 = vertices[(i + 1) % vertices.size()].y;
		int y3 = vertices[(i + 2) % vertices.size()].y;
		//如果出现-1说明过界，直接continue
		if(x0 == -1 || x1==-1||x2==-1||x3==-1)
			continue;
		//水平线直接舍弃
		if (y1 == y2)
			continue;
		//分别计算下端点y坐标、上端点y坐标、下端点x坐标和斜率倒数
		int ymin = y1 > y2 ? y2 : y1;
		int ymax = y1 > y2 ? y1 : y2;
		float x = y1 > y2 ? x2 : x1;
		float dx = (x1 - x2) * 1.0f / (y1 - y2);
		//奇点特殊处理，若点2->1->0的y坐标单调递减则y1为奇点，若点1->2->3的y坐标单调递减则y2为奇点
		if (((y1 < y2) && (y1 > y0)) || ((y2 < y1) && (y2 > y3)))
		{
			ymin++;
			x += dx;
		}
		//创建新边，插入边表ET
		Edge *p = new Edge();
		p->ymax = ymax;
		p->x = x;
		p->dx = dx;
		p->next = pET[ymin]->next;
		pET[ymin]->next = p;
	}

	//扫描线从下往上扫描，y坐标每次加1
	for (int i = 0; i<maxY; i++)
	{
		//取出ET中当前扫描行的所有边并按x的递增顺序（若x相等则按dx的递增顺序）插入AET
		while (pET[i]->next)
		{
			//取出ET中当前扫描行表头位置的边
			Edge *pInsert = pET[i]->next;
			Edge *p = AET;
			//在AET中搜索合适的插入位置
			while (p->next)
			{
				if (pInsert->x > p->next->x)
				{
					p = p->next;
					continue;
				}
				if (pInsert->x == p->next->x && pInsert->dx > p->next->dx)
				{
					p = p->next;
					continue;
				}
				//找到位置
				break;
			}
			//将pInsert从ET中删除，并插入AET的当前位置
			pET[i]->next = pInsert->next;
			pInsert->next = p->next;
			p->next = pInsert;
		}

		//AET中的边两两配对并比对大小
		Edge *p = AET;
		while (p->next && p->next->next)
		{
			for (int x = p->next->x; x < p->next->next->x; x++)
			{
				//进行碰撞威胁预测分析,这里前后坐标可能有点问题
				colli_pred(safe_height, dem_init[x][i].hgt);
				//持续告警
			}
			p = p->next->next;
		}

		//删除AET中满足y=ymax的边
		p = AET;
		while (p->next)
		{
			if (p->next->ymax == i)
			{
				Edge *pDelete = p->next;
				p->next = pDelete->next;
				pDelete->next = nullptr;
				delete pDelete;
			}
			else
			{
				p = p->next;
			}
		}

		//更新AET中边的x值，进入下一循环
		p = AET;
		while (p->next)
		{
			p->next->x += p->next->dx;
			p = p->next;
		}

	}

	

}




