#ifndef _LIB_INIT_H_
#define _LIB_INIT_H_

#include "td_types.h"

#define SYS_ALIGN_WIDTH  16	

enum
{
	PLATFORM_CMD_TM_SYNC = 0,
	PLATFORM_CMD_GET_SLV = 1,	//获取从片编码版本
	PLATFORM_CMD_RBOOT_SLV,		//重启从片
};

typedef struct
{
	td_s32 (*HeartbeatCheck)();		//心跳检测
	td_s32 (*DoCmd)(td_s32 _iId, void *_arg);
	td_s32 m_uiBncBits;				//本地输入主从标识,0为主片通道,1为从片pci通道
	td_s32 m_uiCodePerformance;		//硬件编解码性能,以D1为标准，1个D1为100
	td_s32 m_uiPreviewPerformance;	//硬件预览性能,以D1为标准, 1个D1为100
	td_s32 m_iMediaChnCnt;			//主片处理媒体通道号个数
} TPlatformMsg;

// 平台特性初始化
// _HeartBeatFun : 返回平台硬件心跳检测指针
// _uiBncBits : 返回平台硬件本地通道属性，按位，0表主片通道，1表从片pci通道
td_s32 init_PlatformInit(IN MediaDevice *_ptDev, OUT TPlatformMsg *_ptMsg, td_s32 _iHdmi0_Size, td_s32 _iHdmi1_Size, td_s32 _iBnc_Size);


#endif

