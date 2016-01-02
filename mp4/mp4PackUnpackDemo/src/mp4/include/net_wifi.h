#ifndef _NET_WIFI_H_
#define _NET_WIFI_H_

#include <stdio.h>
#include <string.h> 
#include <sys/stat.h>
#include <stdlib.h>			
#include <unistd.h> 

#include "wifi.h"
#include "public_def.h"
#include "libtiandy.h"

typedef enum//日志类型
{
	TWIFI_ONLINE = 0,
	TWIFI_OFFLINE,
	TWIFI_MODULE_ERROR,
}TWIFI_NOTIFY_TYPE;


typedef int (*FtpUploadRecFile)(td_s32 _iUploadFlag);
td_s32 Net_Wifi_CheckApIp(StrWifiAP * _stStaWifi);

td_s32 Net_Wifi_CheckStaIp(strWifiCnf * _stStaWifi);

td_s32 Net_Wifi_GetSta_NetIp (td_char* _pcMac,td_char* _pcIp, td_char* _pcMask, td_char* _pcGateWay, td_char* _pcDns);

td_s32 Net_Wifi_GetSta_APLsit (td_char * _pcAplist);
td_s32 Net_Wifi_GetSta_APLsit_Ex (td_s32 * _iApNum,  td_char ** _pcApList ,td_char ** _pcApEncyp);


td_s32 Net_Wifi_Get_State(td_s32 * _piOnline ,td_s32 *_piSign);
td_s32 Net_Wifi_Get_State_Ex(td_s32 * _piOnline ,td_s32 *_piSign);

td_s32 Net_Wifi_SetSta_ALLConfig(strWifiCnf * _stStaWifi);

td_s32 Net_Wifi_SetAP_DhcpConfig(td_s32 _iStart,td_s32 _iEnd, td_s32 _pcLease);

td_s32 Net_Wifi_SetAp_DhcpEnable(td_s32 _iEnable);

td_s32 Net_Wifi_SetAp_NetConfig(StrWifiAP *_psWifiAp);

strWifiCnf * Net_Wifi_GetSta_ALLConfig();

NetWifiAP* Net_Wifi_GetAp_NetConfig();
td_s32 Net_Wifi_SetMode(td_s32 _iWifiMode);

td_s32 Net_Wifi_SetConnect(td_s32 _iConnect);

td_s32 Net_Wifi_SetWireBreak(td_s32 _iBreak);

td_s32 Net_Wifi_GetWireBreak();

td_s32 Net_Wifi_GetMode();

td_s32 Net_Wifi_Init(FtpUploadRecFile _Func);

#endif
