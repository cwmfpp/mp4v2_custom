#ifndef _NET_3G_H_
#define _NET_3G_H_

#include <stdio.h>
#include <string.h> 
#include <sys/stat.h>
#include <stdlib.h>			
#include <unistd.h> 

#include "td_types.h"
#include "libtiandy.h"

ST3G_INFO * Net_3g_GetBaseInfo();
const ST3G_STAT * Net_3g_GetState();
td_s32 Net_3g_GetSim(td_char * _pcSim);
td_char * Net_3g_GetSim_Ex();

td_s32 Net_3g_SetSim(td_char * _pcSim);
td_s32 Net_3g_SetModuleEnable(td_s32 _iEnable);

td_s32 Net_3g_SetVpdn(const td_char * _pcApn,td_char * _cAccount,td_char * _cPassword);
td_s32 Net_3g_SetDialType(td_s32 _iStartType,td_s32 _iPara1[M3G_TIME_NUM],td_s32 _iPara2[M3G_TIME_NUM]);
td_s32 Net_3g_SetPhonelist(td_char ** _cPhoneList);
td_s32 Net_3g_SetNotify(td_s32 _iNotifyType,td_char * _cNotifyMsg);

typedef int (*SEND_MSG_TO_ALL_CLIENT1)(char* _cMsg,int _iLen,int _iType);

td_s32 Net_3g_Init(SEND_MSG_TO_ALL_CLIENT1 _SendMsgToAllClient);
/*
函数名称:设置3G在线状态
输入参数:_iNICno 网卡号, _iIsOnline 是否在线
*/
int Net_3g_SetOnlineState(int _iNICno, int _iIsOnline);
//获取4G网卡IP
int Net_3g_Get4GLocalIP(char* _pc4GLocalIP);

#endif

