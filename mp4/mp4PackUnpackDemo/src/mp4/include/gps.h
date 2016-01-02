#ifndef _GPS_H_
#define _GPS_H_

#include <time.h>

typedef struct GPSWORKMODE
{
	int	m_iMode;	//gxl added 20130815 mode for gps or bd, gps:0 bd:1auto:3
}GPSWORKMODE_T;

typedef struct stDataGroup
{    	
	float m_fLatitude;			//纬度，单位是度
	float m_fLongitude;			//经度，单位是度
	char m_cWorE;				//东或者西
	char m_cNorS;				//南或者北
	float m_fSpeed;				//速度，单位是千米/小时
	float m_fElevation;			//海拔，单位米	
	int m_iLocated;				//是否定位, 1为定位，2为未定位
	char m_cgpsWord[1024];		//串口原始数据	
	int m_iIntensity;			//信号强度,0-100
	int m_iSateliteNum;			//当前卫星个数
	time_t m_iTimer;			//gps校时时间	
}GPSData;

/*
GPS模块初始化,
参数:_pcComPath: 	串口设备节点,
             _pstComAttr:串口属性
返回值:0 成功
		    -1失败
*/
int gps_init(char *_pcComPath, TCom *_pstComAttr);

/*
获取GPS模块工作模式
返回值:模块工作模式
*/
int gps_getworkmode(void);


/*
GPS模块重启
参数:_tWorkMode: 	模块工作模式
返回值:0 成功
		    -1失败
*/
int gps_reset(void);


/*
设置gps信息过滤系数
参数:_iFilter: 信息过滤系数,每位代表一种协议，当某位被设置为1，表示客户端需要这条GPS协议，此协议将被转发到客户端，0表示不需要，不转发。iFilter为0表示不转发任何GPS协议。为65535时表示所有信息都需要（相当于不过滤任何信息）。
GPGGA---第1位，GPRMC---第2位，GPVTG---第3位， GPGLL地理定位信息---第4位， GPGSA---第5位， GPGSV---第6位，其它位暂时保留。
返回值:0 成功
		    -1失败
*/
int gps_setfilter(int _iFilter);


/*
获取GPS数据
返回值:GPS数据
*/
const GPSData* gps_getdata(void);
#endif

