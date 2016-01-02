#ifndef _TD_WIFI_H
#define _TD_WIFI_H

typedef enum    //wifi 模式
{
	MODE_NONE = 0,
	MODE_STA,
	MODE_AP,
	MODE_BUT,
}TWIFIMODE;


typedef struct 
{
	int m_iInterVal;                        //检测在线状态间隔
	int m_iSearchInterVal;      //搜索AP间隔
	int m_iSearchFlag;              //搜索触发
	int m_iSearchState;           //当前搜索状态
}stWifiSearch;

typedef enum
{
	AUTH_OPEN = 0,
	AUTH_WEP,
	AUTH_WPA,
	AUTH_WPA2,
}TSafeType;

typedef enum
{
	ENC_NONE = 0,
	ENC_TKIP,
	ENC_AES,
}TEncryptType;

typedef struct
{
	
	char m_cEssid[32];//要连接的wifi热点名称
	int m_iSetSafeEnable; //是否开启安全设置
	TSafeType m_iSafeType;//加密方式//WPA2、WEP、WPA 参见enum TSafeType 定义
	int m_iKeyFormat;//密钥格式//ASCII、HEX
	TEncryptType m_iKeyType;//密钥类型//2: AES, 1: TKIP
	int m_iKeySelect;//密钥选择//WEP
	char m_cWifiEncryption[16]; //存储字符串类型的加密方式 //WPA-PSK WPA2-PSK
	char m_cWifiPwdType[16]; //存储字符串类型的密钥类型
	char m_cWifiPwd[80]; //密钥
	int m_iWifiKeyNum; //保留wifilib库里不用

	unsigned int m_uiIpAddress;//连接热点设置的ip
	unsigned int m_uiSubMask;//连接热点设置的掩码
	unsigned int m_uiGateWay;//连接热点设置的网关
	unsigned int m_uiDNS;//连接热点设置的DNS
	unsigned int m_uiBackDNS;
//	unsigned  char  m_ucMac[8];//获取的wifi网卡mac

	int m_DHCPEnable;//启用DHCP, ip为空并且此值为1生效
	int m_DHCPTimes;//自动获取ip的次数	
	int m_iOnLine;//保留变量，暂不使用
}strWifiCnf;

typedef struct 
{								
   	unsigned int m_uiAPIpAddress;     	//
   	unsigned int m_uiAPSubMask;       	//ap 模式下根据IP自动设置
   	unsigned int m_uiAPGateWay;		//ap 模式下根据IP自动设置
   	unsigned int m_uiAPDNS;			//暂且不用

	int m_iChannel;
	int m_iAPKeyFormat;				//密码格式: ASCII HEX  暂不支持HEX
	TSafeType m_iSafeType;			//加密方式 : OPEN WPA-PSK WPA2-PSK 参见enum TSafeType 定义
	TEncryptType m_iAPKeyNum;		//密码序号:	0 NONE, 1 TKIP, 2 AES ， 3 WEP(不支持)	
	char m_cAPESSID[32];			//essid
	char m_cAPPWD[80];				//连接密码
}StrWifiAP;

typedef struct
{
	unsigned int m_uiIPStart;		//起始IP
	unsigned int m_uiIPEnd;		//结束IP
	unsigned int m_uiSubMask;	//子网掩码
	unsigned int m_uiGateWay;	//网关
	unsigned int m_uiDNS;		//首选DNS
	unsigned int m_uiBackDNS;	//备用DNS
	int m_iLease;		//租期，单位分钟
}DHCPServerInfo;

typedef struct 
{								
	//wifi ap parameters yansm
	unsigned int m_uiWifiMode;         	 //0：不启动  1：普通wifi模式(sta) 2：ap模式  3：AP+sta
	StrWifiAP m_sWifiAPConfig;        //wireless 参数
	unsigned int m_uiAPDHCPenable;   	//ap 模式下是否启用dhcp
	DHCPServerInfo m_TDhcpServerConfig;	//DHCP 服务器参数
}NetWifiAP;


typedef struct
{
	char m_cMAC[20];//MAC地址字符串，形如00:50:C2:28:10:00
	char m_cProtocol[32];//WiFi 协议名称
	char m_cESSID[64];
	char m_cMode[32];
	int m_iChannel;//信道
	unsigned int m_uiQuality;//质量，高16 位代表分子，低16 位代表分母
	int m_iSignalLevel;//信号水准(dBm)
	int m_iNoiseLevel;//噪声水准(dBm)
	int m_blEncryptionKey;//是否加密
	int m_iBitRate;//码率，(Mb/s)
}TAPattribute;



/***********************************************************
功能
子网掩码是否合法

参数
_uiSubMask	子网掩码，小端顺序存放

返回值
0	合法掩码
-1	非法掩码

created by Liu Deqiang on October 08, 2013
************************************************************/
int NetWifi_IsValidMask(unsigned int _uiSubMask);


/***********************************************
功能: WiFi 初始化
WiFi 初始化函数,

参数
_tMode	wifi模式，可为MODE_STA 或MODE_AP

返回值
0		找到网卡，初始化成功
非0	初始化失败

created by Liu Deqiang on August 26, 2013
************************************************/
int NetWifi_Init(TWIFIMODE _tMode);


/***********************************************
功能
配置WiFi 参数并连接到热点
STA 模式下调用

参数:	wifi 配置参数

返回值
0		成功
非0	失败

created by Liu Deqiang on August 27, 2013
************************************************/
int NetWifi_Connect(strWifiCnf *_psWifiPara);


/*****************************************************************************************
功能
获取sta wifi 的在线状态

参数
_piOnline	1: 在线，   0: 不在线
_piSign	信号强度0-100

返回值
0		成功
非0	失败

created by Liu Deqiang on August 28, 2013
******************************************************************************************/
int NetWifi_GetWifiState(int* _piOnline,int* _piSign);


//AP相关设置
/*****************************************************************************************
功能
设置DHCP 服务器参数
参数
_iStart	起始IP, IP 地址的最后一位
_iEnd	结束IP, IP 地址的最后一位
_iLease: 租期，单位分钟，范围60~14400, 1小时~10天

返回值
0		成功
非0	失败
可用此函数设置默认dhcp参数: "0" "0" lease

created by Liu Deqiang on August 28, 2013
******************************************************************************************/
int  NetWifi_Setwifidhcpconfig(int _iStart,int _iEnd, int _iLease);


/*****************************************************************************************
功能
AP 模式下启动/停止DHCP 服务器

参数
_iDhcpenable	启动/停止
注意，每调用一次，DHCP 服务器进程都会重建，请勿频繁调用

created by Liu Deqiang on August 28, 2013
******************************************************************************************/
int NetWifi_Setapdhcp(int _iDhcpenable);


/*****************************************************************************************
功能
设置AP参数

参数
_pWifiAp	Ap参数

返回值
0		成功
非0	失败

网关和dns未使用 保留设置参数
created by Liu Deqiang on August 28, 2013
******************************************************************************************/
int NetWifi_Setwifiapconfig(StrWifiAP *_psWifiAp);


/*****************************************************************************************
功能：获取wifi的ip等信息,Sta模式下调用

参数
_pcMac		MAC 地址
_pcIp		IP 地址
_pcMask		子网掩码
_pcGateWay	网关
_pcDns		DNS

返回值
0		成功
非0	失败

created by Liu Deqiang on August 27, 2013
******************************************************************************************/
int NetWifi_Getwifinetconfig(char* _pcMac,char* _pcIp, char* _pcMask, char* _pcGateWay, char* _pcDns);


/*****************************************************************************************
*名称：NetWifi_setwifimode
*功能：设置wifi模式
*参数：	_iwifimode:
*返回值: 0 success   <0 fail   
*yanshiming creat the fun 20120710
注: 重启后生效
******************************************************************************************/
int NetWifi_Setwifimode(int _iWifiMode);


/***********************************************
功能
WiFi 模式检测

返回值
MODE_NONE	无网卡或未初始化
MODE_STA	STA 模式启动
MODE_AP	AP 模式启动	

created by Liu Deqiang on August 29, 2013
************************************************/
int NetWifi_WifiTypeCheck();


/****************************************************
功能
把无线网关闭

created by Liu Deqiang on July 11, 2012
*****************************************************/
int NetWifi_ResetWiFiCard();


/***********************************************
功能
搜索可用AP

参数
_pcWifiList	搜索结果字符串

返回值:
0		搜索成功
非0	无可用AP

created by Liu Deqiang on August 28, 2013
************************************************/
int NetWifi_DetectAP(char *_pcWifiList);


/****************************************************
功能
对iwlist scan 获取的字符串进行解析，将每个
热点的信息写入TAPattribute 结构体中

参数
_iMaxCells	最多解析多少个热点
_pcToAnalyze	从iwlist 返回的字符串
_PTinfo		指向TAPattribute 数组，分析结果写入
此数组

返回值
正数	解析出多少个热点信息
-1		失败

created by Liu Deqiang on August 28, 2013
*****************************************************/
int IwlistAnalyse(int _iMaxCells, TAPattribute * _PTinfo);


/***********************************************************
功能
把无线网断开

返回值
0		成功
非0	失败

created by Liu Deqiang on August 28, 2013
************************************************************/
int NetWifi_SetOffline();


/***********************************************************
功能
把无线网上线

返回值
0		成功
非0	失败

created by Liu Deqiang on August 29, 2013
************************************************************/
int NetWifi_SetOnline();

#endif
