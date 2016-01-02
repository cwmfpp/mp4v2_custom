//将DHCP与即插即用分开，即插即用负责连接策略，DHCP负责设备设备IP等。
//其中只有DHCP开一个线程，发现有IP出现或者消失时通过回调即插即用层。
#ifndef _PLUG_H
#define _PLUG_H
#include "public_def.h"

#define DIG_CHN_PTZ_NAME	31
#define DIG_CHN_NAME_LEN	31
#define MAX_DEV_NUM	(64)
#define MAX_OLD_DEV_NUM	(200)
typedef int (*LinkCallBack)(unsigned int _ip, char *_mac, int _iChn,int _iStat, int _iLink);

typedef struct 
{
	char m_cMac[32];
	unsigned int m_u32Delay;
	unsigned int m_iTime;
}TChangeMacList;

typedef struct 
{
	unsigned int m_u32Time;
	unsigned int m_inter;
	int (*fun)(unsigned int);
}Ttask;

typedef struct
{
	unsigned int m_Ip;
	int m_iChn;
	char m_cMac[40];
	char m_Use;//0:没用，1:正在使用，2:暂时不能用
	char m_cConfict;
	unsigned int m_submask;
	unsigned int m_iTime;
	unsigned int m_iDelay;
}TUseIp;

typedef struct
{
	unsigned int m_ip;
	unsigned int m_iChn;
	char m_cMac[40];
	unsigned int m_iTime;
	unsigned int m_iDelay;
}TLinkStat;

typedef struct
{
	int m_ibonding;
	unsigned int m_u32MyIp;
	char *m_strMac;
	unsigned int m_u32MyIp2;
	char *m_strMac2;
	TUseIp *m_pUseLink;
	TUseIp *m_strOldDev;
	TUseIp *m_strNewDev;
	int *m_piFlushNewDevFlag;
}TPlugSeachInit;
	
typedef enum
{
	PLUG_SEARCH,		//搜索IP实现
	PLUG_CHECKIP,		//检查IP看是否有冲突
	PLUG_LINKTASK,			//连接
	PLUG_MAX				//限定大小
} TPlugMode;//即插即用的实现方式，

typedef enum
{
	PLUG_NODEV,	//该通道没有设备进行连接，即该通道是空闲状态
	PLUG_LINKING,
	PLUG_LINK,		//该通道已链接
	PLUG_BRADKOFFING,
	PLUG_BRADKOFF = 0xff,	//连接中断，可能因为网络原因导致的暂时中断
}TPlugStat;

typedef struct
{
	int m_iOtherChn;	//对方通道号
	int m_iPrivate;		//0-私有，1-onvif
	int m_iTcp;		//0-TCP,		1-UDP
	int m_iPort;	//端口号
	int m_iStreamType;	//码流类型,0:主码流, 1副码流
	unsigned int m_ip;
	char m_strPtzProtocol[DIG_CHN_PTZ_NAME + 1]; //ptz协议
	char m_strUserName[DIG_CHN_NAME_LEN + 1]; //用户名
	char m_strUserPsw[DIG_CHN_NAME_LEN + 1];	//密码
	char m_strVdecPsw[DIG_CHN_NAME_LEN + 1];	//视频解密密码
}TDevAttribute;
typedef struct 
{
	int m_id;
	int m_iChn;
	char m_cMac[40]; 
	char m_strIp[40];
}TUiGetDev;
/////////////////////////plug////////////////////
int Plug_init(void);
int Plug_SetMod(int _Mod);
int Plug_Destroy(void);
int Plug_GetStat(void);
int plug_UiGetNewDev(int *_iNum, TUiGetDev *_pDev);
int plug_UiSetNewDev(int _iMove, int _id, int _iChn);
////////////////////////dhcp////////////////////
////////////////////////search///////////////////
int search_init(TPlugSeachInit *_tPlug);

#endif


