#ifndef _ALARMER_H_
#define _ALARMER_H_

// 报警器基类定义
#include "public_def.h"
#include "vca_head.h"
#include "libstorage.h"
#define  NSGTEST  //add by nsg0926
typedef enum
{
	ALARMER_ATTR_EN		= 0x0000, //使能，传参为整数, 0不使能,1使能
	ALARMER_ATTR_PORT_MOD 	= 0x0001, //端口报警器模式, 传参为整数,  0 : 开路, 1闭路
} ALARMER_ATTR_ID;

typedef enum
{
	ALARMER_PORT 	= 0x0100,	//端口报警
	ALARMER_MD	= 0x0200,	//移动侦测
	ALARMER_VL		= 0x0300,	//视频丢失
    ALARMER_OD		= 0x0A00,	//视频遮挡报警  20130305 SUN Wei add for video cover

	//add by caizhaoxu for vca 20120321
	ALARMER_TRIPWIRE   = 0x0400,    //绊线报警     
	ALARMER_PERIMETER  = 0x0500,    // 周界报警
	ALARMER_FACE       = 0x0600,    // 人脸侦测
	ALARMER_DIAGNOSE   = 0x0700,    //视频诊断
	ALARMER_OSCER      = 0x0800,    //物品看护
    ALARMER_AL         = 0x0900,    //音频丢失

	//shihc add 20130225 
	ALARMER_CAP         = 0x1000,    //智能跟踪
	ALARMER_CPC         = 0x2000,    //流量统计
	ALARMER_CDD         = 0x3000,    //人群聚集
	ALARMER_SVD         = 0x4000,    //人群聚集
} ALARMER_TYPE;

typedef struct _TAlarmer	//报警器通用定义
{
	td_s32 m_iType;		//报警器类型
	
	td_s32 (*GetStat)(struct _TAlarmer *_ptThis);		//状态检测
	td_s32 (*SetAttr)(td_s32 _iId, void *_pValue, struct _TAlarmer *_ptThis); //设置属性
} TAlarmer;


typedef struct _TAlarmOutPort // 报警输出端口通用定义
{
	td_s32 m_iType;
	td_s32 (*GetStat)(struct _TAlarmOutPort *_ptThis); //获取状态
	td_s32 (*SetStat)(td_s32 _iStat, struct _TAlarmOutPort *_ptThis); //设置状态
} TAlarmOutPort;
#endif

