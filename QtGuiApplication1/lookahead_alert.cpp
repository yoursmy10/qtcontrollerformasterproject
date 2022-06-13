#include "lookahead_alert.h"


LookAheadAlert::LookAheadAlert(QObject *parent):QObject(parent)
{
	
	
}

LookAheadAlert::~LookAheadAlert()
{	
	vertices.clear();
}




//���и澯�߳�,�ö�ʱ���������̶�Ƶ��ִ��һ��
void LookAheadAlert::sltRunAlert(double lat, double lon, double yaw_heli, double heli_hgt, double velocity_x, double velocity_y)
{

	emit sigConsoleInfo("Look_Ahead ALERT is working...");
	//m_mutex.lock();

	//double velocity = std::sqrt(pow(velocity_x,velocity_x) + pow(velocity_y, velocity_y)); //�����xyƽ���ϵ��ٶ�,ͬһ��ֵ����ʾ�����ﶼ�ֱ������һ�Σ��ǳ�����Դ
	double velocity = std::sqrt(velocity_x*velocity_x + velocity_y*velocity_y);
	double LAD_sim = LAD_generate(velocity); //����ǰ���ٶ�����ǰ�Ӿ���,velocityΪxyƽ���ϵĺ�����
	QString lad = QString("LAD = %1").arg(LAD_sim);
	//������ʱ��Ͳ�Ҫ�澯�ˣ�����
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







//����ǰ�Ӿ���
double LookAheadAlert::LAD_generate(double velocity)
{
	
	//���ݹ�ʽ��LAD = (V*V)/2gtan(alpha) + V*T
	double LAD_res = 0;
	emit sigConsoleInfo("LAD_generate is working...");
	//�Ӿ���ֵ����Ϊ���ٶ���ʸ��������
	LAD_res = abs(((abs(velocity) * abs(velocity))) / (2 * GRAVITY*(std::tan(alpha / RAD2DEGREE))) + abs(velocity) * react_time);
	emit sigEmitLAD(LAD_res);//����LAD��������
	return LAD_res;
}





//��ֱ������ǰ����λ��ǰ�����е���ɨ�裬��ͨ���߳̽��жԱȣ�lat,lon:ֱ������ǰλ��,��Ҫ���ǻ�ͷ���򡣿���Ӧ��ɨ�����㷨
//���飺ԭ��terrainscan��ѯ���Ƕ�ά���飬�����˷�Ч�ʵģ���Ȼ�Ѿ��������ݿ⣬��Ӧ��ֱ�������ݿ�����ȡ�������ݣ���ôӦ�ø��ĵ���findLOC����
void LookAheadAlert::terrain_scan(double lat, double lon, double yaw_heli, double LAD, double heli_hgt)
{
	//1.-----------------���ݵ�ǰλ���ҵ�ֱ����ǰ�������ڵ��θ߳�ͼ�е�����------------------------------
	//���õ��θ߳�ͼ����ɨ�裬ȷ��ֱ������ǰλ��O(2n)
	int m_init = 0; //m for lat
	int n_init = 0; //n for lon


	vector<int> tmp;
	tmp = findLoc(lat, lon);
	m_init = tmp[0]; //�Ҳ���λ��
	
	n_init = tmp[1];
	//���������Χ��ֱ�ӷ���
	if(m_init == -1 || n_init ==-1)
	{
		return;
	}
	tmp.clear();
	
	//qDebug()<<"helicopter location in dem map successfully updated..." << endl;
	//���˵õ�ֱ�����ڵ��θ߳�ͼ�еĳ�ʼ����
	/*
	 
	 A-------------------B
	 |    SCANNING       |
	 |      AREA	     |
	 |                   |
	 C-------------------D
		  ORIENTATION
			 Heli
	*/

	//ֱ�����������½ǵ�C
	double lon_lb = lon + ref_lon*LAD_width*sin(yaw_heli / RAD2DEGREE) / 2;
	double lat_lb = lat - ref_lat*LAD_width*cos(yaw_heli / RAD2DEGREE) / 2;
	tmp = findLoc(lat_lb, lon_lb);
	int m_lb = tmp[0];
	int n_lb = tmp[1];
	vertices.push_back(Point(m_lb, n_lb));
	tmp.clear();
	//ֱ�����������½ǵ�D
	double lon_rb = lon - ref_lon*LAD_width*sin(yaw_heli / RAD2DEGREE) / 2;
	double lat_rb = lat + ref_lat*LAD_width*cos(yaw_heli / RAD2DEGREE) / 2;
	tmp = findLoc(lat_rb, lon_rb);
	int m_rb = tmp[0];
	int n_rb = tmp[1];
	vertices.push_back(Point(m_rb, n_rb));
	tmp.clear();
	//ֱ�����������Ͻǵ�A
	double lon_lt = lon + ref_lon*LAD*cos(yaw_heli / RAD2DEGREE) + ref_lon*LAD_width*sin(yaw_heli / RAD2DEGREE) / 2;
	double lat_lt = lat + ref_lat*LAD*sin(yaw_heli / RAD2DEGREE) - ref_lat*LAD_width*cos(yaw_heli / RAD2DEGREE) / 2;
	tmp = findLoc(lat_lt, lon_lt);
	int m_lt = tmp[0];
	int n_lt = tmp[1];
	vertices.push_back(Point(m_lt, n_lt));
	tmp.clear();
	//ֱ�����������Ͻǵ�B
	double lon_rt = lon + ref_lon*LAD*cos(yaw_heli / RAD2DEGREE) - ref_lon*LAD_width*sin(yaw_heli / RAD2DEGREE) / 2;
	double lat_rt = lat + ref_lat*LAD*sin(yaw_heli / RAD2DEGREE) + ref_lat*LAD_width*cos(yaw_heli / RAD2DEGREE) / 2;
	tmp = findLoc(lat_rt, lon_rt);
	int m_rt = tmp[0];
	int n_rt = tmp[1];
	vertices.push_back(Point(m_rt, n_rt));
	tmp.clear();

	double safe_alt = delta_h * heli_hgt; //�ϰ��ﰲȫ�߶�
	//���˵õ��ĸ��ǵ�ľ�γ���Լ�DEMͼ������
	//2.-----------------���ֱ����ǰ����Ҫɨ��������ĸ��ǵ����꣬���ҽ����߱�ET������߱�(AET)------------------
	//3.-----------------��ʼɨ�裬���Ա�ÿһ���еĸ߳���Ϣ,��������澯������澯��Ϣ------------------------------
	polygon_scan(vertices, safe_alt);
	vertices.clear();//ÿִ��һ�ζ�Ҫ���һ��
}






//���������ײ���գ��ͷ���1�����򷵻�0
int LookAheadAlert::colli_pred(double safe_height, double dem_hgt)
{
	if(safe_height <= dem_hgt)
	{
		emit sigColliAlert(1);//�����źŸ��澯ָʾ��
		return 1;
	}
	else
	{
		emit sigColliAlert(0);
		return 0;
	}
}





//������Ӧ��γ���ڵ��θ߳�ͼ�е�����ֵ
//error:����������������
//��Ӧ����ʹ�ö�ά�����ˣ���Ϊ��ǰ���������ԭ��Ӧ����ֻҪ���ڼ���������֮��ͺã�ֱ�������ݿ⣬���ڽ����ﷶΧ�ڵ�ȫ������
//TODO ������ڽ�������,һ�㲻�������
vector<int> LookAheadAlert::findLoc(double lat, double lon)
{
	int m_init = 0;
	vector<int> res;
	//�ж���������
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
	//������ߵ��y����
	int maxY = 0;
	for (int i = 0; i<vertices.size(); i++)
	{
		if (vertices[i].y > maxY)
		{
			maxY = vertices[i].y;
		}
	}
	//��ʼ��ET��AET
	Edge *pET[MAX_LEN];
	for (int i = 0; i<maxY; i++)
	{
		pET[i] = new Edge();
		pET[i]->next = nullptr;
	}
	AET = new Edge();
	AET->next = nullptr;


	//�����߱�ET
	for (int i = 0; i<vertices.size(); i++)
	{
		//ȡ����ǰ��1ǰ�����ڵĹ�4���㣬��1���2��������Ϊ����ѭ������ıߣ������������0�͵�3���ڼ������
		int x0 = vertices[(i - 1 + vertices.size()) % vertices.size()].x;
		int x1 = vertices[i].x;
		int x2 = vertices[(i + 1) % vertices.size()].x;
		int x3 = vertices[(i + 2) % vertices.size()].x;
		int y0 = vertices[(i - 1 + vertices.size()) % vertices.size()].y;
		int y1 = vertices[i].y;
		int y2 = vertices[(i + 1) % vertices.size()].y;
		int y3 = vertices[(i + 2) % vertices.size()].y;
		//�������-1˵�����磬ֱ��continue
		if(x0 == -1 || x1==-1||x2==-1||x3==-1)
			continue;
		//ˮƽ��ֱ������
		if (y1 == y2)
			continue;
		//�ֱ�����¶˵�y���ꡢ�϶˵�y���ꡢ�¶˵�x�����б�ʵ���
		int ymin = y1 > y2 ? y2 : y1;
		int ymax = y1 > y2 ? y1 : y2;
		float x = y1 > y2 ? x2 : x1;
		float dx = (x1 - x2) * 1.0f / (y1 - y2);
		//������⴦������2->1->0��y���굥���ݼ���y1Ϊ��㣬����1->2->3��y���굥���ݼ���y2Ϊ���
		if (((y1 < y2) && (y1 > y0)) || ((y2 < y1) && (y2 > y3)))
		{
			ymin++;
			x += dx;
		}
		//�����±ߣ�����߱�ET
		Edge *p = new Edge();
		p->ymax = ymax;
		p->x = x;
		p->dx = dx;
		p->next = pET[ymin]->next;
		pET[ymin]->next = p;
	}

	//ɨ���ߴ�������ɨ�裬y����ÿ�μ�1
	for (int i = 0; i<maxY; i++)
	{
		//ȡ��ET�е�ǰɨ���е����б߲���x�ĵ���˳����x�����dx�ĵ���˳�򣩲���AET
		while (pET[i]->next)
		{
			//ȡ��ET�е�ǰɨ���б�ͷλ�õı�
			Edge *pInsert = pET[i]->next;
			Edge *p = AET;
			//��AET���������ʵĲ���λ��
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
				//�ҵ�λ��
				break;
			}
			//��pInsert��ET��ɾ����������AET�ĵ�ǰλ��
			pET[i]->next = pInsert->next;
			pInsert->next = p->next;
			p->next = pInsert;
		}

		//AET�еı�������Բ��ȶԴ�С
		Edge *p = AET;
		while (p->next && p->next->next)
		{
			for (int x = p->next->x; x < p->next->next->x; x++)
			{
				//������ײ��вԤ�����,����ǰ����������е�����
				colli_pred(safe_height, dem_init[x][i].hgt);
				//�����澯
			}
			p = p->next->next;
		}

		//ɾ��AET������y=ymax�ı�
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

		//����AET�бߵ�xֵ��������һѭ��
		p = AET;
		while (p->next)
		{
			p->next->x += p->next->dx;
			p = p->next;
		}

	}

	

}




