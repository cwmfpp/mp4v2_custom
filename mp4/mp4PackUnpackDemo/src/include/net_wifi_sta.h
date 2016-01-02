#ifndef _TD_NETWIFI_H
#define _TD_NETWIFI_H
#include "public_def.h"
#include "guiparameter.h"
#include "td_types.h"//add by zhuangzhibing 20120619

//Cheng 0923 should be 20
#define WIFI_DETECT_COUNT 20

#define IFCONFIG_MAIN_WIFI 1
#define IFCONFIG_MAIN 0
#define bb_perror_msg(...)

//Cheng 0923 add
#define FTP_QUERY_MAGIC 64*1024-1
#define DAY_MINS 24*60
#define ONE_HOUR_MINS 60
#define ONE_MIN_SECS 60

#define WIFIDHCP_PID
#define DHCP_WIFIPID_PATH "/tmpd/dhcp_pid_rausb0"
#define DHCP_WIFI_ENABLE(Para)  (Para->m_DHCPEnable || Para->m_uiIpAddress == 0)

#define WIFI_SEARCH_STATE_NONE 0
#define WIFI_SEARCH_STATE_TRIGGER 1
#define WIFI_SEARCH_STATE_DOING 2

#define PACK_SIZE     4096
#define ERROR           0
#define SUCCESS         1

#define RT_PRIV_IOCTL					(0x8BE0 + 0x0E)
#define RT_OID_802_11_WIFI_STATE	                0x0525
#define WIFI_MODULE "/dvr/ko/rt73.ko"
#define NEW_WIFI_MODULE "/dvr/ko/rt3070sta.ko"
#define WIFI_RESET_PROCESSING    1
#define WIFI_RESET_OK          0

//wifi ftp
#define STR_SERVER_NAME_LEN    63
#define IP_STR_LEN 31
typedef struct 
{
	char	m_cServerName[STR_SERVER_NAME_LEN + 1];       //服务器地址
	char 	m_cUserName[IP_STR_LEN + 1];         //用户名
	char	m_cPassword[IP_STR_LEN + 1];			//密码
	int		m_iPort;					//端口号
	int m_iUploadEnable;                    //自动上传
	int m_iManulUpEnable;                 //手动上传
}sWifiFtpPara;

/**************************************************/
td_u32 net_WifiInit(char* cIP,char* cMask, strWifiCnf *pWifiPara);
//td_void timer_DetectEth0(unsigned long arg);
void timer_DetectAP(char *pcWifiList);
void timer_onlinestate();
td_s32 route_set_netcard(char* _pcNetCardType, const char* _pcStat, const char* _pcGateWay);
int task_ftp_uploadfiles(void* para);
int sys_resetWifi(void);
td_s32 ifconfig_set_stat(const char* _pcNetCardType, const char* _pcStat);
td_s32 ifconfig_set_ip_mask(const char* _pcNetCardType, const char* _pcIp, const char* _pcMask);
td_s32 iwconfig_set_essid(const td_s8 *_pcNetCardType, const td_s8 *_pcEssid);		//zhangyi add 2010/12/13
int net_wifipara_trans();
void *WifiConnectThread(unsigned long para);
int net_switch_init();
int get_wifi_state();
td_s32 get_wifi_online_state();														//zhangyi add 2010/12/13
//td_s32 IsHaveNetCard(char * _cNIC);
int net_ftp_exit();
int net_ftp_init();
//Cheng 20110224 add reset wifi module
int net_WifiReset(int iDelaySecs);
//add by zhuangzhibing 20120619
int wifi_getNetworkBreak();
int wifi_setNetworkBreak(int _iNetworkBreak);
sWifiFtpPara *wifi_getWifiFtpPara();
void *wifi_getWifiSearch();
int wifi_getwifinetconfig(char *cMac,char* ip, char* mask, char* gateway, char* dns);
int wifi_getWifiRestState();
int  wifi_setFtpTrigger(int _iFtpTrigger);
int wifi_setWifiChangeSetting(int _iWifiChangeSetting);
int netwifi_init();
int net_ftp_sendpic(td_s8* _pcFileName);//modify by cgx 20120809
int wifi_getwifimoduletype();//add by zhuangzhibing 20120727

//add end

#define GUI_DBG_MSG_ON		0
#if GUI_DBG_MSG_ON
#define DebugMsg(strMsg) 		printf("%s %s() %d : %s\n", __FILE__, __FUNCTION__, __LINE__, (strMsg))
#define dbgprintf(arg...)			{ \
	printf("%s %s %d : ", __FILE__, __FUNCTION__, __LINE__); \
	printf(arg); \
}
#else
#define DebugMsg(strMsg)
#define dbgprintf(arg, ...)
#endif


#endif

