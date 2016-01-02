#ifndef _LIB_TD_VO_H_
#define _LIB_TD_VO_H_

#include "public_def.h"
#include "td_types.h"

#define MAX_PRIORITY_LEVEL 	8
#define MAX_AO_NUM		 	4		// 3531SDK实际只有2个AoDev,其余做扩展
#define MAX_AO_CHNNUM		2
typedef enum
{
	AO_BIND_AI,			//绑定AI通道
	AO_BIND_ADEC,		//数字通道预览
	AO_BIND_PCI,		//绑定PCI通道
	AO_BIND_BUTT,
}TAOBindType;

typedef enum
{
	AO_PRI_PREVIEW_AI		=0,
	AO_PRI_PREVIEW_ADEC		=0,	
	AO_PRI_PALYBACK			=1,
	AO_PRI_TALK				=2,
	AO_PRI_ALARM			=3,
	AO_PRI_BUTT				= MAX_PRIORITY_LEVEL,
}TAOPriority;

td_s32 TdAO_Init(td_s32 _iAoChnCnt);
/*
	功能:初始化AO
*/

//td_s32 TdAO_Uninit();
/*
	功能:去初始化AO
*/

td_s32 TdAO_Start(td_s32 _iAoDevId,TAOPriority _iPriority,TAOBindType _iBindType,td_s32 _iChnNum, AUDIO_TYPE_E _iAdecType, td_s32 _iAoSamples, td_s32 _iAoSampleRate);
/*
	功能:与TdAO_Stop配合使用，主要用来重置音频输出的采样频率
	输入:
		_iAoDevId:Ao设备号；
		_iPriority:优先级;
		_iBindType:绑定类型;
		_iChnNum:绑定通道号,解码时设为任意值；
		_iAdecType:解码类型，预览时设为任意值;
		_iAoSamples:采样率.
	输出:
		Ao编号 成功；
		-1 失败
*/


td_s32 TdAO_SendStream(td_s32 _iAoId, td_u8* _u8Data,td_s32 _iLen, td_s32 _s32BlockFlag);
/*
	功能:向Ao输送码流
	输入:
		_iAoId:Ao编号;
		_u8Data:数据流指针;
		_iLen:数据长度
		_s32BlockFlag:阻塞标志;
	输出:
		数据长度   成功
		-1 失败
		-2 丢弃
*/

td_s32 TdAO_Stop(td_s32 _iAoId);
/*
	功能:停止某一对应编号的Ao,若其为优先级最高，则启动其低优先级的AO
	输入:
		_iAoDevId:Ao设备号；
*/

//td_s32 TdAO_Clear(td_s32 _iAoId);
td_s32 TdAO_CheckAOChnBufStat(td_s32 _iAoId);

int libTdAO_GetDebug(char *_cString,int _iLen);

void vGetAACEnble(td_s32 _iEnble);
#endif
