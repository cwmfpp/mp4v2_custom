/*******************************************************************
文件名	:Alarm.c
公司	:天津天地伟业数码科技有限公司
创建者	:陶永亮
创建日期:2008-10-24
简要描述:报警相关参数的定义，接口函数原型等
修改记录:2008-10-24,陶永亮，创建
        
******************************************************************/

#ifndef ALARM_H
#define ALARM_H

#include "alarmer.h"

#define MAX_OUTPORT_DELAY_TIME	3600			//最大报警输出端口延时时间
#define MOTION_DETECT_DELAY_TIME	5			//移动侦测报警延时时间
#define MAX_PTZ_LINKAGE_TYPE	3				//最大PTZ联动类型数

#define DETECT_INTERVAL		200*1000			//报警检测时间间隔，单位 MicroSecond

//#define 	REC_VCA            32  				//智能分析报警录像类型


// 声音回调(0 : 表式消音, 0x0001 : 蜂鸣器, 其他位预留给别的声音)
typedef void (*AlarmMakeSoundCallBack)(td_s32 _iSndType);
// 联动LED 显示回调(chn:通道号, -1表总LED开关, stat : 1显示，0不显示)
typedef void (*AlarmLedDspCallBack)(td_s32 _iChn, td_s32 _iStat);

// 报警输出端口状态通知回调
typedef void (*AlarmOutPortStatNotifyCallBack)(td_s32 _iPortNo, td_s32 _iStat);
// 报警状态更新回调(type:报警类型, chn:通道, stat:报警状态, 1报警, 0消警)
typedef void (*AlarmStatChgNotifyCallBack)(td_s32 _iType, td_s32 _iChn, td_s32 _iStat);

typedef void (*AlarmSinglePicDspCallBack)(td_s32 _iChn, td_s32 _iAlarm);

//typedef int (*pReportAlarm)(int chn, int state, unsigned int ip);	//报警通知网络的回调函数
typedef int (*MainEventChnParaCallBack)(int _iChn);//设置主码流事件为当前参数
typedef int (*AlarmClearExceptionCallBack)(td_s32 _iType, td_s32 _iState);//全部消警时消除异常报警回调

//几种联动方式
typedef enum
{
	LK_SOUND = 0,								//联动声音提示
	LK_DISPLAY,									//联动屏幕显示
	LK_OUTPORT,									//联动报警输出端口
	LK_RECORD,									//联动录像
	LK_PTZ,										//联动预置位
	LK_SNAPSHOT,								//联动抓拍
	LK_SINGLE_PIC,								//联运单画面显示
	LK_MARKFILE,								//dce add ,车载中加入的端口报警联动录像打点
	LK_MAX,										//最大值，该值在声明数组时使用，要扩展内容请在该值以上添加
}TAlarmLinkage;

//报警状态
typedef enum
{
	STATE_DISAPPEAR = 0,						//正常消警状态							
	STATE_ALARM,								//正常报警状态
	STATE_CLEAR,								//手动消警状态 陶永亮 090414 新增，修改BUG，整理报警逻辑
}TAlarmState;

//报警触发方式
typedef enum
{
	TR_OPEN = 0,								//断开触发报警,即开路报警，常闭报警
	TR_CLOSE,									//闭合触发报警,即闭路报警，常开报警
}TTrigger;

enum
{
	VCA_DISABLE = 0,    //不使能智能分析
	VCA_USE_LOCAL,      //使能本地通道智能分析
	VCA_USE_REMOTE,		//使能前端通道智能分析
}; //智能分析使能标识


//联动PTZ的几种类型
typedef enum
{
	PTZ_NONE = 0,								//不联动
	PTZ_PRESET,									//联动预置位
	PTZ_ROUTINE,								//联动模式路径
	PTZ_CRUISE,									//联动巡航轨迹
}TPTZType;

//陶永亮 091125 将原来的三个成员换成一个数组
//联动PTZ的参数
typedef struct
{
	TPTZType	m_ePTZType;						//联动PTZ类型
	td_s8		m_cChannelNo;					//联动通道
	td_u8		m_PTZNo[MAX_PTZ_LINKAGE_TYPE];	//PTZ联动号
}TPTZLinkage;

//布防时间段
typedef struct
{
	td_s32		m_iStartHour;					//开始时
	td_s32		m_iStartMin;					//开始分
	td_s32		m_iStopHour;					//结束时
	td_s32		m_iStopMin;						//结束分
	td_s32		m_blEnable;						//该时间段的使能
}TTimeSegment;

//报警参数
typedef struct
{
	td_s8				m_blTimeEnable;							//布防时间段使能  mode by nsg 报警模板是否使能
	td_s8				m_blEnable;								//该种报警类型是否启用  mode by nsg 当前时间是否在报警时间段中
	TTimeSegment		m_stSegment[7][MAX_TIME_SEGMENT];		//布防时间段
	td_s8				m_blLinkage[LK_MAX];					//报警联动方式
	TAlarmState			m_eState;								//报警状态
	TTrigger			m_eTrigger;								//报警触发方式
	td_s32				m_blOutPortNo[MAX_OUTPORT_NUM];			//要联动的报警输出端口
	td_s32				m_iPreset;								//要联动的预置位号
	td_s32				m_iChannelNum;							//要联动录像的通道数目
	td_s32				m_iRecordChannels[MAX_CHANNEL_NUM];		//要联动录像的通道号
	td_s32				m_iSnapNum;								//要联动抓拍的通道数目
	td_s32				m_iSnapChannels[MAX_CHANNEL_NUM];		//要联动抓拍的通道号
	td_s32				m_iPTZNum;								//要联动的PTZ通道数
	TPTZLinkage			m_stPTZLinkage[MAX_CHANNEL_NUM];		//要联动的PTZ通道
}TAlarmParam;

//报警输出端口设置,
typedef struct
{
	td_s32				m_iDelayTime;							//报警延时时间，单位:秒，-1表示无穷大，即手动消警
	td_s32				m_iLastTime;							//报警输出持续时间
	td_s32				m_blTimeEnable;							//布防时间段使能
	td_s32				m_blEnable;								//报警输出端口使能
	TAlarmState			m_eState;								//报警输出端口状态
	TTrigger			m_eTrigger;								//报警出发方式
	TTimeSegment		m_stSegment[7][MAX_TIME_SEGMENT];		//布防时间段
}TOutPortParam;

td_s32 alm_SetAlarmSchedule(TAlarmType _eType, td_s32 _iChannelNo, td_s32 _iEnable, td_s32 _iWeekDay, TTimeSegment* _pstSeg);
/*
 * 函数名称：SetAlarmSchedule
 * 简要描述: 设置报警检测时间段和使能
 * 输	  入：
 *			_eType, 报警类型
 * 			_iChannelNo, 0~MAX_CHANNEL_NUM,通道号
 *			_iEnable, 0或1,是否开启时间段检测
 *			_iWeekDay,0~6,星期(wday)
 *			_pstSeg, 非空，时间段指针(最多四个时间段)
 * 输	  出：
 * 			ERR_ALM_WRONG_PARAM,错误的参数
 * 			ERR_ALM_SUCCESSFUL,操作成功
 * 			ERR_ALM_FAILED,其他原因导致操作失败
 * 修改日志：
 * 			2008-11-11,陶永亮,创建
 *			2009-11-25,陶永亮,合并接口
 */
 
td_s32 alm_GetAlarmSchedule(TAlarmType _eType, td_s32 _iChannelNo, td_s32* _piEnable, td_s32 _iWeekDay, TTimeSegment* _pstSeg);
/*
 * 函数名称：GetAlarmSchedule
 * 简要描述: 获取报警检测时间段和使能
 * 输	  入：
 *			_eType,报警类型
 * 			_iChannelNo, 0~MAX_CHANNEL_NUM,通道号
 *			_piEnable, 非空,用于传出是否开启时间段检测
 *			_iWeekDay,0~6,星期(wday)
 *			_pstSeg, 非空，用于传出时间段(最多四个时间段)
 * 输	  出：
 * 			ERR_ALM_WRONG_PARAM,错误的参数
 * 			ERR_ALM_SUCCESSFUL,操作成功
 * 			ERR_ALM_FAILED,其他原因导致操作失败
 * 修改日志：
 * 			2008-11-11,陶永亮,创建
 *			2009-11-25,陶永亮,合并接口
 */

td_s32 alm_SetOutPortSchedule(td_s32 _iPortNo, td_s32 _iEnable, td_s32 _iWeekDay, TTimeSegment* _pstSeg);
/*
 * 函数名称：SetOutPortSchedule
 * 简要描述: 设置端口报警检测时间段和使能
 * 输	  入：
 * 			_iPortNo, 0~MAX_OUTPORT_NUM,通道号
 *			_iEnable, 0 或 1,是否开启时间段检测
 *			_iWeekDay,0~6,星期(wday)
 *			_pstSeg, 非空，时间段指针(最多四个时间段)
 * 输	  出：
 * 			ERR_ALM_WRONG_PARAM,错误的参数
 * 			ERR_ALM_SUCCESSFUL,操作成功
 * 			ERR_ALM_FAILED,其他原因导致操作失败
 * 修改日志：
 * 			2008-11-11,陶永亮,创建
 */
 
td_s32 alm_GetOutPortSchedule(td_s32 _iPortNo, td_s32* _piEnable, td_s32 _iWeekDay, TTimeSegment* _pstSeg);
/*
 * 函数名称：GetOutPortSchedule
 * 简要描述: 获取报警输出端口检测时间段和使能
 * 输	  入：
 * 			_iPortNo, 0~MAX_OUTPORT_NUM,通道号
 *			_piEnable, 非空,用于传出是否开启时间段检测
 *			_iWeekDay,0~6,星期(wday)
 *			_pstSeg, 非空，用于传出时间段(最多四个时间段)
 * 输	  出：
 * 			ERR_ALM_WRONG_PARAM,错误的参数
 * 			ERR_ALM_SUCCESSFUL,操作成功
 * 			ERR_ALM_FAILED,其他原因导致操作失败
 * 修改日志：
 * 			2008-11-11,陶永亮,创建
 */
 
td_s32 alm_SetAlarmLinkRecord(TAlarmType _eType, td_s32 _iChannelNo, td_s32 _iEnable, td_s32* _iChannelList, td_s32 _iChannelNum);
/*
 * 函数名称：SetAlarmLinkRecord
 * 简要描述: 设置报警联动录像参数
 * 输	  入：
 *			_eType,报警类型
 * 			_iChannelNo, 0~MAX_CHANNEL_NUM,通道号
 *			_iEnable, 0 或 1, 是否联动录像
 *			_iChannelList, 非空，要联动的通道编号列表
 *			_iChannelNum, 0~MAX_CHANNEL_NUM, 共联动多少个通道
 * 输	  出：
 * 			ERR_ALM_WRONG_PARAM,错误的参数
 * 			ERR_ALM_SUCCESSFUL,操作成功
 * 			ERR_ALM_FAILED,其他原因导致操作失败
 * 修改日志：
 * 			2008-11-11,陶永亮,创建
 *			2009-11-25,陶永亮,合并接口
 */
 
td_s32 alm_GetAlarmLinkRecord(TAlarmType _eType, td_s32 _iChannelNo, td_s32* _piEnable, td_s32* _iChannels, td_s32* _iChannelNum);
/*
 * 函数名称：GetAlarmLinkRecord
 * 简要描述: 获取报警联动录像参数
 * 输	  入：
 *			_eType,报警类型
 * 			_iChannelNo, 0~MAX_CHANNEL_NUM,通道号
 *			_piEnable, 0 或 1, 用于传出是否联动录像
 *			_iChannels, 非空，用于传出要联动的通道编号列表
 *			_iChannelNum, 0~MAX_CHANNEL_NUM, 用于传出共联动多少个通道
 * 输	  出：
 * 			ERR_ALM_WRONG_PARAM,错误的参数
 * 			ERR_ALM_SUCCESSFUL,操作成功
 * 			ERR_ALM_FAILED,其他原因导致操作失败
 * 修改日志：
 * 			2008-11-11,陶永亮,创建
 *			2009-11-25,陶永亮,合并接口
 */
 
td_s32 alm_SetAlarmLinkSound(TAlarmType _eType, td_s32 _iChannelNo, td_s32 _iEnable);
/*
 * 函数名称：SetAlarmLinkSound
 * 简要描述: 设置报警联动声音提示使能
 * 输	  入：
 * 			_iChannelNo, 0~MAX_CHANNEL_NUM,通道号
 *			_iEnable, 0 或 1, 是否联动声音提示
 * 输	  出：
 * 			ERR_ALM_WRONG_PARAM,错误的参数
 * 			ERR_ALM_SUCCESSFUL,操作成功
 * 			ERR_ALM_FAILED,其他原因导致操作失败
 * 修改日志：
 * 			2008-11-11,陶永亮,创建
 *			2009-11-25,陶永亮,合并接口
 */
 
td_s32 alm_GetAlarmLinkSound(TAlarmType _eType, td_s32 _iChannelNo, td_s32* _piEnable);
/*
 * 函数名称：GetAlarmLinkSound
 * 简要描述: 获取报警联动声音提示使能
 * 输	  入：
 *			_eType,报警类型
 * 			_iChannelNo, 0~MAX_CHANNEL_NUM,通道号
 *			_piEnable, 非空, 用于传出是否联动声音提示
 * 输	  出：
 * 			ERR_ALM_WRONG_PARAM,错误的参数
 * 			ERR_ALM_SUCCESSFUL,操作成功
 * 			ERR_ALM_FAILED,其他原因导致操作失败
 * 修改日志：
 * 			2008-11-11,陶永亮,创建
 *			2009-11-25,陶永亮,合并接口
 */
 
td_s32 alm_SetAlarmLinkDisplay(TAlarmType _eType, td_s32 _iChannelNo, td_s32 _iEnable);
/*
 * 函数名称：SetAlarmLinkDisplay
 * 简要描述: 设置报警联动屏幕显示使能
 * 输	  入：
 *			_eType, 报警类型
 * 			_iChannelNo, 0~MAX_CHANNEL_NUM,通道号
 *			_iEnable, 0 或 1, 是否联动屏幕显示
 * 输	  出：
 * 			ERR_ALM_WRONG_PARAM,错误的参数
 * 			ERR_ALM_SUCCESSFUL,操作成功
 * 			ERR_ALM_FAILED,其他原因导致操作失败
 * 修改日志：
 * 			2008-11-11,陶永亮,创建
 *			2009-11-25,陶永亮,合并接口
 */
 
td_s32 alm_GetAlarmLinkDisplay(TAlarmType _eType, td_s32 _iChannelNo, td_s32* _piEnable);
/*
 * 函数名称：GetAlarmLinkDisplay
 * 简要描述:获取报警联动屏幕显示使能
 * 输	  入：
 *			_eType,报警类型
 * 			_iChannelNo, 0~MAX_CHANNEL_NUM,通道号
 *			_piEnable, 非空, 用于传出是否联动屏幕显示
 * 输	  出：
 * 			ERR_ALM_WRONG_PARAM,错误的参数
 * 			ERR_ALM_SUCCESSFUL,操作成功
 * 			ERR_ALM_FAILED,其他原因导致操作失败
 * 修改日志：
 * 			2008-11-11,陶永亮,创建
 *			2009-11-25,陶永亮,合并接口
 */

td_s32 alm_GetAlarmLinkSingPic(TAlarmType _eType, td_s32 _iChannelNo, td_s32* _piChn);
/*
 * 函数名称：alm_GetAlarmLinkSingPic
 * 简要描述:获取报警联动单画面显示通道
 * 输	  入：
 *			_eType,报警类型
 * 			_iChannelNo, 0~MAX_CHANNEL_NUM,报警器通道号
 *			_piChn, 非空, 用于传出联动哪个通道单画面显示(通道号+1，0表式不联动)
 * 输	  出：
 * 			ERR_ALM_WRONG_PARAM,错误的参数
 * 			ERR_ALM_SUCCESSFUL,操作成功
 * 			ERR_ALM_FAILED,其他原因导致操作失败
 * 修改日志：
 * 			2011-04-07,杜秀龙,创建
 */

td_s32 alm_SetAlarmLinkSingPic(TAlarmType _eType, td_s32 _iChannelNo, td_s32 iChn);
/*
 * 函数名称：alm_SetAlarmLinkSingPic
 * 简要描述:设置报警联动单画面显示通道
 * 输	  入：
 *			_eType,报警类型
 * 			_iChannelNo, 0~MAX_CHANNEL_NUM,报警器通道号
 *			_piChn, 非空, 用于传出联动哪个通道单画面显示(通道号+1，0表式不联动)
 * 输	  出：
 * 			ERR_ALM_WRONG_PARAM,错误的参数
 * 			ERR_ALM_SUCCESSFUL,操作成功
 * 			ERR_ALM_FAILED,其他原因导致操作失败
 * 修改日志：
 * 			2011-04-07,杜秀龙,创建
 */

// 杨珏20120418 报警联动单画面巡航时间设置与获取
td_s32 alm_SetAlarmLInkSingPicIntv(td_s32 _iInterval);
td_s32 alm_GetAlarmLInkSingPicIntv(void);

td_s32 alm_SetAlarmLinkOutPort(TAlarmType _eType, td_s32 _iChannelNo, td_s32 _iEnable, td_s32* _piOutPortNo);
/*
 * 函数名称：SetAlarmLinkOutPort
 * 简要描述: 设置报警联动报警输出端口参数
 * 输	  入：
 *			_eType,报警类型
 * 			_iChannelNo, 0~MAX_CHANNEL_NUM,通道号
 *			_iEnable, 0 或 1, 是否联动输出端口
 *			_iOutPortNo, 0~MAX_OUTPORT_NUM, 要联动的输出端口号
 * 输	  出：
 * 			ERR_ALM_WRONG_PARAM,错误的参数
 * 			ERR_ALM_SUCCESSFUL,操作成功
 * 			ERR_ALM_FAILED,其他原因导致操作失败
 * 修改日志：
 * 			2008-11-11,陶永亮,创建
 *			2009-11-25,陶永亮,合并接口
 */
 
td_s32 alm_GetAlarmLinkOutPort(TAlarmType _eType, td_s32 _iChannelNo, td_s32* _piEnable, td_s32* _piOutPortNo);
/*
 * 函数名称：GetAlarmLinkOutPort
 * 简要描述:获取报警联动输出端口参数
 * 输	  入：
 *			_eType,报警类型
 * 			_iChannelNo, 0~MAX_CHANNEL_NUM,通道号
 *			_piEnable, 非空, 用于传出是否联动输出端口
 *			_piOutPortNo, 非空，用于传出要联动的输出端口号
 * 输	  出：
 * 		ERR_ALM_WRONG_PARAM,错误的参数
 * 		ERR_ALM_SUCCESSFUL,操作成功
 * 		ERR_ALM_FAILED,其他原因导致操作失败
 * 修改日志：
 * 			2008-11-11,陶永亮,创建
 *			2009-11-25,陶永亮,合并接口
 */
 
td_s32 alm_SetAlarmLinkSnap(TAlarmType _eType, td_u64 _ulChannelNo, td_s32 _iEnable, td_s32 _iChannelNum, td_s32* _piChannels);
/*
 * 函数名称：SetAlarmLinkSnap
 * 简要描述: 设置报警联动抓拍参数
 * 输	  入：
 *			_eType, 报警类型
 * 			_iChannelNo, 0~MAX_CHANNEL_NUM,通道号
 *			_iEnable, 0 或 1, 是否联动录像
 *			_piChannels, 非空，要联动的通道编号列表
 *			_iChannelNum, 0~MAX_CHANNEL_NUM, 共联动多少个通道
 * 输	  出：
 * 			ERR_ALM_WRONG_PARAM,错误的参数
 * 			ERR_ALM_SUCCESSFUL,操作成功
 * 			ERR_ALM_FAILED,其他原因导致操作失败
 * 修改日志：
 * 			2008-11-11,陶永亮,创建
 *			2009-11-25,陶永亮,合并接口
 */
 
td_s32 alm_GetAlarmLinkSnap(TAlarmType _eType, td_s32 _iChannelNo, td_s32* _piEnable, td_s32* _piChannelNum, td_s32* _piChannels);
/*
 * 函数名称：GetAlarmLinkSnap
 * 简要描述: 获取报警联动抓拍参数
 * 输	  入：
 *			_eType, 报警类型
 * 			_iChannelNo, 0~MAX_CHANNEL_NUM,通道号
 *			_piEnable, 0 或 1, 用于传出是否联动录像
 *			_piChannels, 非空，用于传出要联动的通道编号列表
 *			_piChannelNum, 0~MAX_CHANNEL_NUM, 用于传出共联动多少个通道
 * 输	  出：
 * 			ERR_ALM_WRONG_PARAM,错误的参数
 * 			ERR_ALM_SUCCESSFUL,操作成功
 * 			ERR_ALM_FAILED,其他原因导致操作失败
 * 修改日志：
 * 			2008-11-11,陶永亮,创建
 *			2009-11-25,陶永亮,合并接口
 */
 
td_s32 alm_SetAlarmLinkPTZ(TAlarmType _eType, td_s32 _iChannelNo, td_s32 _iEnable, td_s32 _iChannelNum, TPTZLinkage* _pstLinkage);
/*
 * 函数名称：SetAlarmLinkPTZ
 * 简要描述: 设置报警联动PTZ参数
 * 输	  入：
 *			_eType, 报警类型
 * 			_iChannelNo, 0~MAX_CHANNEL_NUM,通道号
 *			_iEnable, 0 或 1, 是否联动录像
 *			_pstLinkage, 非空，要联动的通道的PTZ参数列表
 *			_iChannelNum, 0~MAX_CHANNEL_NUM, 共联动多少个通道
 * 输	  出：
 * 			ERR_ALM_WRONG_PARAM,错误的参数
 * 			ERR_ALM_SUCCESSFUL,操作成功
 * 			ERR_ALM_FAILED,其他原因导致操作失败
 * 修改日志：
 * 			2008-11-11,陶永亮,创建
 *			2009-11-25,陶永亮,合并接口
 */
 
td_s32 alm_GetAlarmLinkPTZ(TAlarmType _eType, td_s32 _iChannelNo, td_s32* _piEnable, td_s32* _piChannelNum, TPTZLinkage* _pstLinkage);
/*
 * 函数名称：GetAlarmLinkPTZ
 * 简要描述: 获取报警联动PTZ参数
 * 输	  入：
 *			_eType, 报警类型
 * 			_iChannelNo, 0~MAX_CHANNEL_NUM,通道号
 *			_piEnable, 0 或 1, 用于传出是否联动录像
 *			_pstLinkage, 非空，用于传出要联动的通道的PTZ参数列表
 *			_piChannelNum, 0~MAX_CHANNEL_NUM, 用于传出共联动多少个通道
 * 输	  出：
 * 			ERR_ALM_WRONG_PARAM,错误的参数
 * 			ERR_ALM_SUCCESSFUL,操作成功
 * 			ERR_ALM_FAILED,其他原因导致操作失败
 * 修改日志：
 * 			2008-11-11,陶永亮,创建
 *			2009-11-25,陶永亮,合并接口
 */

td_s32 alm_SetAlarmLinkPTZbyNet(TAlarmType _eType, td_s32 _iChannelNo, td_s32 _iLinkChannel, td_s32 _iLinkType, td_s32 _iLinkNo);
/*
 * 函数名称：SetAlarmLinkPTZbyNet
 * 简要描述: 通过网络设置报警联动PTZ
 * 输	  入：
 *			_eType, 报警类型
 *			_iChannelNo, 待设置的通道号，0~MAX_CHANNEL_NUM
 *			_iLinkChannel, 待联动的通道号，0~MAX_CHANNEL_NUM
 *			_iLinkType, 联动类型， TPTZLinkage
 *			_iLinkNo, 预置位号、轨迹号或路径号
 * 输	  出：
 *			ERR_ALM_WRONG_PARAM,错误的参数
 * 			ERR_ALM_SUCCESSFUL,操作成功
 * 			ERR_ALM_FAILED,其他原因导致操作失败
 * 修改日志：
 * 			2008-12-09,陶永亮,创建
 *			2009-11-25,陶永亮,合并接口
 */
 
td_s32 alm_SetPortAlarmTrigger(td_s32 _iPortNo, td_s32 _iTrigger);
/*
 * 函数名称：SetPortAlarmTrigger
 * 简要描述: 设置端口报警的触发方式
 * 输	  入：
 * 			_iPortNo, 0~MAX_INPORT_NUM,通道号
 *			_iTrigger, 0 或 1, 0为开路报警，1为闭路报警
 * 输	  出：
 * 		ERR_ALM_WRONG_PARAM,错误的参数
 * 		ERR_ALM_SUCCESSFUL,操作成功
 * 		ERR_ALM_FAILED,其他原因导致操作失败
 * 修改日志：
 * 			2008-11-11,陶永亮,创建
 */
 
td_s32 alm_GetPortAlarmTrigger(td_s32 _iPortNo, td_s32* _piTrigger);
/*
 * 函数名称：GetPortAlarmTrigger
 * 简要描述: 获取端口报警的触发方式
 * 输	  入：
 * 			_iPortNo, 0~MAX_INPORT_NUM,通道号
 *			_piTrigger, 非空，用于传出该端口的触发方式, 0为开路报警，1为闭路报警
 * 输	  出：
 * 		ERR_ALM_WRONG_PARAM,错误的参数
 * 		ERR_ALM_SUCCESSFUL,操作成功
 * 		ERR_ALM_FAILED,其他原因导致操作失败
 * 修改日志：
 * 			2008-11-11,陶永亮,创建
 */
 
td_s32 alm_SetOutPortParam(td_s32 _iPortNo, td_s32 _iTrigger, td_s32 _iDelayTime);
/*
 * 函数名称：SetOutPortParam
 * 简要描述: 设置报警输出端口参数
 * 输	  入：
 * 			_iPortNo, 0~MAX_OUTPORT_NUM,通道号
 *			_iTrigger, 0 或 1, 0为开路报警，1为闭路报警
 *			_iDelayTime,,报警延时时间， 0~MAX_OUTPORT_DELAY_TIME,若为-1则表示手动消警，无限延时
 * 输	  出：
 * 		ERR_ALM_WRONG_PARAM,错误的参数
 * 		ERR_ALM_SUCCESSFUL,操作成功
 * 		ERR_ALM_FAILED,其他原因导致操作失败
 * 修改日志：
 * 			2008-11-11,陶永亮,创建
 */
 
td_s32 alm_GetOutPortParam(td_s32 _iPortNo, td_s32* _piTrigger, td_s32* _piDelayTime);
/*
 * 函数名称：GetOutPortParam
 * 简要描述: 获取报警输出端口参数
 * 输	  入：
 * 			_iPortNo, 0~MAX_OUTPORT_NUM,通道号
 *			_piTrigger, 0 或 1, 0为开路报警，1为闭路报警
 *			_piDelayTime, 非空,用于传出报警延时时间，0~MAX_OUTPORT_DELAY_TIME,若为-1则表示手动消警，无限延时
 * 输	  出：
 * 		ERR_ALM_WRONG_PARAM,错误的参数
 * 		ERR_ALM_SUCCESSFUL,操作成功
 * 		ERR_ALM_FAILED,其他原因导致操作失败
 * 修改日志：
 * 			2008-11-11,陶永亮,创建
 */
 
//td_s32 alm_GotoPreset(td_s32 _iChannelNo, td_s32 _iPresetNo);
/*
 * 函数名称：GptoPreset
 * 简要描述: 调用预置位
 * 输	  入：
 * 			_iChannelNo,通道号,0~15
 * 			_iPresetNo,预置位号,0~64
 * 输	  出：
 * 		ERR_ALM_WRONG_PARAM,错误的参数
 * 		ERR_ALM_SUCCESSFUL,调用预置位成功
 * 		ERR_ALM_WRITE_COM_FAILED,向串口写数据失败
 * 		ERR_ALM_FAILED,其他原因导致操作失败
 * 修改日志：
 * 			2008-10-24,陶永亮,创建
 */
 
//td_s32 alm_HandleMotionDetect(td_s32 _iChannelNo);
/*
 * 函数名称：HandleMotionDetect
 * 简要描述: 移动侦测检测的回调函数
 * 输	  入：
 * 			_iChannelNo,通道号,0~MAX_CHANNEL_NUM
 * 输	  出：
 * 		ERR_ALM_WRONG_PARAM,错误的参数
 * 		ERR_ALM_SUCCESSFUL,操作成功
 * 		ERR_ALM_WRITE_COM_FAILED,向串口写数据失败
 * 		ERR_ALM_FAILED,其他原因导致操作失败
 * 修改日志：
 * 			2008-10-24,陶永亮,创建
 */
#if 0
td_s32 alm_HandleVideoLost(td_s32 _iChannelNo, td_s32 _iState);
/*
 * 函数名称：HandleVideoLost
 * 简要描述: 视频丢失报警检测回调函数
 * 输	  入：
 * 			_iChannelNo,通道号,0~MAX_CHANNEL_NUM
 *			_iState, 报警状态，0消警， 1报警
 * 输	  出：
 * 		ERR_ALM_WRONG_PARAM,错误的参数
 * 		ERR_ALM_SUCCESSFUL,操作成功
 * 		ERR_ALM_WRITE_COM_FAILED,向串口写数据失败
 * 		ERR_ALM_FAILED,其他原因导致操作失败
 * 修改日志：
 * 			2008-12-26,陶永亮,创建
 */
#endif
td_s32 alm_GetPortNum(td_s32 * _piInPort, td_s32 * _piOutPort);
/*
 * 函数名称：GetPortNum
 * 简要描述: 获取报警输入输出端口数
 * 输	  入：
 * 			_piInPort,用于传出输入端口数，非空
 *			_piOutPort,用于传出输出端口数，非空
 * 输	  出：
 * 		ERR_ALM_WRONG_PARAM,错误的参数
 * 		ERR_ALM_SUCCESSFUL,操作成功
 * 		ERR_ALM_FAILED,其他原因导致操作失败
 * 修改日志：
 * 			2008-12-2,陶永亮,创建
 */

td_s32 alm_ClearAllAlarm(td_s32 _iAlarmType);
/*
 * 函数名称：ClearAllAlarm
 * 简要描述: 消除所有报警
 * 输	  入：
 * 			_iAlarmType, 报警类型,3~5,0xFF
 * 输	  出：
 * 		ERR_ALM_SUCCESSFUL,操作成功
 * 		ERR_ALM_FAILED,其他原因导致操作失败
 * 修改日志：
 * 			2008-12-7,陶永亮,创建
 */
td_s32 alm_ClearAlarm(td_s32 _iAlarmType, td_s32 _iChn);//wangsong16D1
/*
 *函数名称:ClearAlarm
 *简要描述:消除报警,按通道号或者端口号
 *输入:
 *		_iAlarmType,报警类型
 *		_iChn,通道号或者端口号
 */
td_s32 alm_DefaultCfg(void);
/*
 * 函数名称：DefaultCfg
 * 简要描述: 恢复初始设置
 *			无
 * 输	  出：
 * 		ERR_ALM_SUCCESSFUL,操作成功
 *		ERR_ALM_OPEN_FILE_ERROR,打开文件错误
 *		ERR_ALM_WRITE_FILE_ERROR,写文件错误
 * 		ERR_ALM_FAILED,其他原因导致操作失败
 * 修改日志：
 * 			2008-12-09,陶永亮,创建
 */

td_s32 alm_WriteCfg();
/*
 * 函数名称：DefaultCfg
 * 简要描述: 恢复初始设置
 *			无
 * 输	  出：
 * 		ERR_ALM_SUCCESSFUL,操作成功
 *		ERR_ALM_OPEN_FILE_ERROR,打开文件错误
 *		ERR_ALM_WRITE_FILE_ERROR,写文件错误
 * 		ERR_ALM_FAILED,其他原因导致操作失败
 * 修改日志：
 * 			2008-12-09,陶永亮,创建
 */


td_s32 alm_InitBefor();

td_s32 alm_Init(TComInstance *_ptAlarmerHostCom, void (*ComDataProc)(td_s32, td_s32, td_u8 *));
/*
 * 函数名称：Init
 * 简要描述: 初始化报警模块
 * 输	  入：
 * 			无
 * 输	  出：
 * 		ERR_ALM_WRONG_PARAM,错误的参数
 * 		ERR_ALM_SUCCESSFUL,调用预置位成功
 * 		ERR_ALM_FAILED,其他原因导致操作失败
 * 修改日志：
 * 			2008-11-4,陶永亮,创建
 */

td_s32 alm_Exit();
/*
 * 函数名称：Exit
 * 简要描述: 退出报警模块
 * 输	  入：
 * 			无
 * 输	  出：
 * 		ERR_ALM_WRONG_PARAM,错误的参数
 * 		ERR_ALM_SUCCESSFUL,调用预置位成功
 * 		ERR_ALM_FAILED,其他原因导致操作失败
 * 修改日志：
 * 			2008-11-5,陶永亮,创建
 */

td_s32 alm_ChangeOutPort(td_s32 _iPortNo, td_s32 _iState);
/*
 * 函数名称：ChangeOutPort
 * 简要描述: 改变报警输出端口状态
 * 输	  入：
 * 			_iPortNo, 输出端口号, 0~ MAX_OUTPORT_NUM
 *			_iState, 输出端口状态，1，闭合；0，断开
 * 输	  出：
 * 		ERR_ALM_WRONG_PARAM,错误的参数
 * 		ERR_ALM_SUCCESSFUL,调用预置位成功
 * 		ERR_ALM_FAILED,其他原因导致操作失败
 * 修改日志：
 * 			2009-03-11,陶永亮,创建
 */

td_s32 alm_GetOutPortState(td_s32 _iPortNo);
/*
 * 函数名称：GetOutPortState
 * 简要描述: 获取报警输出端口状态
 * 输	  入：
 * 			_iPortNo, 输出端口号, 0~ MAX_OUTPORT_NUM
 * 输	  出：
 * 			1，闭合；0，断开
 * 修改日志：
 * 			2009-06-10,陶永亮,创建
 */
 
td_s32 alm_GetState(td_s32 _iChannelNo, TAlarmType _eType);
/*
 * 函数名称：GetState
 * 简要描述: 获取各个通道的各种报警的状态
 * 输	  入：
 * 			_iChannelNo, 通道号, 0~ MAX_CHANNEL_NUM
 *			_eType, 报警类型，TAlarmType
 * 输	  出：
 * 		-1, 参数错误
 * 		TAlarmState, 报警状态
 * 修改日志：
 * 			2009-04-20,陶永亮,创建
 */
 
//td_s32 alm_AlarmLinkage(TAlarmType _eType, td_s32 _iNo, td_s32 _iState);
//void alm_PortAlarmTimer(unsigned long _param);
void alm_CheckScheduleTimer(unsigned long _param);
//void alm_ClearAlarmTimer(unsigned long _param);
//void alm_VideoLostTimer(unsigned long _param);
//void alm_CheckSlaveMdTimer(unsigned long _param);

td_s32 alm_ChangeOutPortAlarmState(td_s32 _iPortNo, td_s32 _iState);
/*
 * 函数名称：ChangeOutPortAlarmState
 * 简要描述: 控制报警输出端口的状态
 * 输	  入：
 * 			_iPortNo, 输出端口号, 0~ MAX_OUTPORT_NUM
 *			_iState, 端口状态，1，报警；0，消警
 * 输	  出：
 * 			ERR_ALM_SUCCESSFUL, 操作成功
 * 			ERR_ALM_FAILED, 操作失败
 * 修改日志：
 * 			2009-09-07,陶永亮,创建
 */


td_s32 alm_CD_LinkBuzzer(td_s32 _iState);
/*
	*函数名称:alm_CD_LinkBuzzer
	*简要描述:刻录时报警联动蜂鸣器
	*输入: _iState:
		   STATE_ALARM 正常报警状态
		   STATE_DISAPPEAR 正常消警状态
	*输出: 
	       ERR_ALM_SUCCESSFUL, 操作成功
  		   ERR_ALM_FAILED, 操作失败
  	*修改日志:
  			2010/11/17， 石胜男
*/


// 注册报警状态改变通知函数
td_s32 alm_RegAlarmStatChgNotify(AlarmStatChgNotifyCallBack _CallBack);

// 注册报警输出状态通知函数
td_s32 alm_RegOutPortStatNofify(AlarmOutPortStatNotifyCallBack _CallBack);

// 设置声音回调函数
td_s32 alm_SetMakeSoundCallBack(AlarmMakeSoundCallBack  _MakeSound);

// 设置LED显示回调
td_s32 alm_SetLedDspCallBack(AlarmLedDspCallBack _LedDsp);

// 设置报警联动单画面显示回调
td_s32 alm_SetSinglePicDspCallBack(AlarmSinglePicDspCallBack _CallBack);

// 注册一个报警器
// type : 报警器类型 ALM_MD, ALM_PORT等 
// chn : 充当此类报警的第多少路
// alarm : 报警器实例 
td_s32 alm_RegAlarmer(td_s32 _iType, int _iChn, TAlarmer *_ptAlarm);

// 检测报警器实例查找报警通道号与类型
td_s32 alm_GetAlarmerMesg(TAlarmer *_ptAlarm, td_s32 *_iType, td_s32 *_iNo);
 
//zhangweiwei add 20120113
int  alm_SetInPortAlarmerOrder(int _iOrder);

int alm_GetInPortAlarmerOrder(void);

int alm_UiGetOutPortAlarmState(int _portNo, int * _pState, char * _strMsg, char * _strName);

int alm_UiSetOutPortAlarmState(int _portNo, int _iState);
//手动消警时通知异常处理模块消警
td_s32 alm_RegClearExceptionCallBack(AlarmClearExceptionCallBack _AlarmClearExceptionCallBack);

//add by nsg 20120524
td_s32 vca_alm_GetAlarmLinkOutPort(td_s32 _iChannelNo, td_s32 _iRulesNo, TVcaEventType _eType,  td_s32* _piEnable, td_s32* _piOutPortNo);
td_s32 vca_alm_SetAlarmLinkOutPort(td_s32 _iChannelNo, td_s32 _iRulesNo, TVcaEventType _eType, td_s32 _iEnable, td_s32* _piOutPortNo);
td_s32 vca_alm_GetAlarmSchedule(td_s32 _iChannelNo, td_s32 _iRulesNo, TVcaEventType _eType, td_s32 _iWeekDay, td_s32* _piEnable, TTimeSegment* _pstSeg);
td_s32 vca_alm_SetAlarmSchedule(td_s32 _iChannelNo, td_s32 _iRulesNo, TVcaEventType _eType, td_s32 _iWeekDay, td_s32 _iEnable, TTimeSegment* _pstSeg);
td_s32 vca_alm_GetAlarmLinkDisplay(td_s32 _iChannelNo, td_s32 _iRulesNo, TVcaEventType _eType, td_s32 * _piEnable);
td_s32 vca_alm_SetAlarmLinkDisplay(td_s32 _iChannelNo, td_s32 _iRulesNo, TVcaEventType _eType, td_s32 _iEnable);
td_s32 vca_alm_GetAlarmLinkSingPic(td_s32 _iChannelNo, td_s32 _iRulesNo, TVcaEventType _eType, td_s32 *_piChn );
td_s32 vca_alm_SetAlarmLinkSingPic(td_s32 _iChannelNo, td_s32 _iRulesNo, TVcaEventType _eType, td_s32 _iChn );
td_s32 vca_alm_GetAlarmLinkSnap(td_s32 _iChannelNo, td_s32 _iRulesNo, TVcaEventType _eType, td_s32* _piEnable, td_s32* _piChannelNum, td_s32* _piChannels);
td_s32 vca_alm_SetAlarmLinkSnap(td_s32 _iChannelNo, td_s32 _iRulesNo, TVcaEventType _eType, td_s32 _iEnable, td_s32 _iChannelNum, td_s32* _piChannels);
td_s32 vca_alm_GetAlarmLinkRecord( td_s32 _iChannelNo, td_s32 _iRulesNo, TVcaEventType _eType,  td_s32* _piEnable, td_s32* _iChannels, td_s32* _iChannelNum);
td_s32 vca_alm_SetAlarmLinkRecord(td_s32 _iChannelNo, td_s32 _iRulesNo, TVcaEventType _eType, td_s32 _iEnable, td_s32* _iChannelList, td_s32 _iChnTotalNum);
td_s32 vca_alm_GetAlarmLinkSound(td_s32 _iChannelNo, td_s32 _iRulesNo, TVcaEventType _eType, td_s32* _piEnable);
td_s32 vca_alm_SetAlarmLinkSound(td_s32 _iChannelNo, td_s32 _iRulesNo, TVcaEventType _eType, td_s32 _iEnable);
td_s32 vca_alm_GetAlarmLinkPTZ(td_s32 _iChannelNo, td_s32 _iRulesNo, TVcaEventType _eType, td_s32* _piEnable, td_s32* _piChannelNum, TPTZLinkage* _pstLinkage);
td_s32 vca_alm_SetAlarmLinkPTZ(td_s32 _iChannelNo, td_s32 _iRulesNo, TVcaEventType _eType, td_s32 _iEnable, td_s32 _iChnTotalNum, TPTZLinkage* _pstLinkage );
td_s32 vca_alm_SetAlarmLinkPTZbyNet(td_s32 _iChannelNo, td_s32 _iRulesNo, TVcaEventType _eType, td_s32 _iLinkType, td_s32 _iLinkChannel, td_s32 _iLinkNo);
td_s32 vca_TellUIAndIEAlarmStop(int _iChn, int _iRuleNum, TVcaEventType _eEventType);

//add by nsg120613
//初始化联动多次抓拍全局变量
int alm_LinkMulitSnapParamInit();
//end

void alm_ClearVCAAlarm(int _chn, int _rule, TVcaEventType _type);
/*
 * 函数名称：alm_ClearVCAAlarm
 * 简要描述: 清空智能分析报警联动操作，
 *　由于现在智能分析信息存在联合体中，一旦同一报警规则更改事件类型，
 * 则报警联动将无法停止，只有在设置规则事件时清空报警联动
 * 输	  入：
 * 			_chn, 通道号, 0~ 有效通道
 *			_rule, 智能分析规则好，0~7
 *          _type, 智能分析事件类型 VCA_TRIPWIRE~VCA_CAP
 * 输	  出：
 * 			无
 * 修改日志：
 * 			2012-1031,蔡兆旭,创建
 */

TOutPortParam* alm_GetOutPortStatus(void);

//dce add,编码参数切换至主码流(事件)
void alm_RegMainEventNotify(MainEventChnParaCallBack _MainEventCallBack);


//报警联动多次抓拍
typedef struct  TAlarmInforType
{
	td_u64 m_ulTriggerSource;   //触发源
	int m_iAlarmType;		//报警类型
	struct  TAlarmInforType *m_stListNext;
}TAlarmInform;

typedef struct TMultiSnapShotTyp
{
	td_s32 	m_iSecond;	//时间计数
	td_s32* m_piInterval;	//抓拍时间间隔
	td_s32*	m_piType;	//抓拍模式0：不使能 1：使能（定时） 2:（报警联动抓拍）3：报警联动多次抓拍

	//20121120 SUN Wei modify for vca_linkage start
	//td_s32  m_RecTypeCount[REC_BASIC_TYPE];	//记录每个报警联动抓拍图片类型的触发的次数
	td_s32  m_RecTypeCount[16];	//16种警情按位表示 0~7已定义，8-15未定义，modified by sunwei for vca_linkage
	//20121120 SUN Wei modify for vca_linkage end

	td_u64  m_ulTrigger;					//触发源号，按位表示，代表哪一个触发源联动了此通道

	//SUN Wei 20130722 复用于固定张数抓拍模式下，设置抓拍张数
	td_s32  m_iCount;					//记录此通道被联动次数 
	
	TAlarmInform *m_stListHead;
	pthread_mutex_t m_Mutexlock;

	int (*AddList)(td_u64 _ulTrigger, TAlarmType iAlarmType, struct TMultiSnapShotTyp *stSnapShotParam);
	int (*DelList)(td_u64 _ulTrigger, TAlarmType iAlarmType, struct TMultiSnapShotTyp *stSnapShotParam);
	
}TMultiSnapShot;

//抓拍模式
enum
{
	SNAPSHOT_DISABLE = 0,    //不使能
	SNAPSHOT_ENABLE,         //使能 
	SINGLESHOT,              //单张抓拍
	MULTISNAPSHOT,		     //报警联动多次抓拍模式
	PRESETSHOTNUM,           //预设抓拍张数模式
}; 

//报警联动多次抓拍 end

#endif  /*ALARM_H*/

