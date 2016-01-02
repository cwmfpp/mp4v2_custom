#ifndef _LIB_DECODER_H
#define _LIB_DECODER_H

#include "public_def.h"
#include "ptz.h"
#include "td_types.h"
#include "guiparameter.h"
/*
	主要功能:
	1、隔离解码器和DVR之间的差异，可以由网络解析模块直接调用
	        将对某个显示设备，某个画面的操作变换为对某个数字通道的操作。
                具体的操作包括，
                连接/断开视频
                打开/关闭音频预览
                打开/关闭对讲
                设备控制/发送透明通道数据
                开始/停止PUSH流 
	2、用户设置和获取切换列表，直接操作配置文件。
	3、对切换操作进行控制，开始/停止脱机切换操作。
	               
*/

#define LOOPNVS_MAX_NUM		64		//切换列表的最大长度为64

#define DEC_MAX_VO_DEV_NUM		2
#define DEC_MAX_VO_CHN_NUM		16
#define DEC_MAX_CHN_NUM			32

/*
	解码器目前就两个VOdev,为了实现解码器通道1:VGA+BNC  2:HDMI,需要将下发的通道号
	进行转换
*/
//#define DecChanNoSwitch(_iChanNo)  ((0 == (_iChanNo)) ? 1 : 0)	

/*视频通道循环表定义======================================*/
typedef struct
{
    td_s32			m_iEnable;			//是否循环1；循环
    td_s32			m_iCount;			//循环表中有多少连接
    td_s32			m_iIndex;			//目前是哪个连接
    td_s32			m_iAudio;			//目前是哪个音频输出
    td_s32 			m_iTalk;				//是否正在进行对讲
    TDECNVSITEM	m_strNVSItem[LOOPNVS_MAX_NUM];	
} TLoopNvsItem; 

typedef enum
{
	DEC_ENABLEID_START = 0x13000,	// 解码器使能ID 起始
	DEC_SAVELASTFRAME,			// 预览保留最后一帧
	DEC_PREVIEWPREFER,			// 预览偏好

	DEC_ENABLEID_END = 0x13fff,	// 解码器使能ID 结束
} DEC_ENABLEID;


//定义一个回调函数
typedef int  (*EVENT_VIEWSTATE)(int _iChn,int _iPic, TDECNVSITEM* _nvsItem,int  _iState);
typedef void (*DEC_RESTARTAPPCALLBAK)();


td_s32 Dec_Init(EVENT_VIEWSTATE _eViewState);


/*
	从配置文件中获取切换列表
*/
td_s32  Dec_GetLoopItem(TLoopNvsItem* _LoopItem, int _iChan, int _iPic);


/*
	将切换列表保存到配置文件中
*/
td_s32 Dec_SetLoopItem(TLoopNvsItem* _LoopItem, int _iChan, int _iPic);


/*
	在切换列表中增加一条前端设备的设置信息
*/
td_s32 Dec_AddLoopItem(int _iChan,int _iPic,int _iIndex,TDECNVSITEM* _pNvsItem);


/*
	删除切换列表中的某条信息
*/
td_s32 Dec_DeleteLoopItem(int _iChan,int _iPic,int _iIndex);


/*
	开始切换
*/
td_s32 Dec_StartLoop(int _iChan,int _iPic);

/*
	停止切换
*/
td_s32 Dec_StopLoop(int _iChan,int _iPic);

int Dec_LoopListGetAudioStat(int _iChan,int _iPic);

void Dec_LoopNotifyToUI(int _iChan,int _iPic,int _iLoopStat);

/*
	连接某个NVS
*/
td_s32 Dec_StartView(int _iChan,int _iPic,TDECNVSITEM* _nvsItem,int _iTimeOut,int _iLoop);


/*
	断开与某个NVS的连接
*/
td_s32 Dec_StopView(int _iChan,int _iPic,int _iAudioMark,int _iTimeOut,int _iLoop, int _iSaveLastFrame);

/*
	切换到上一个设备
*/
td_s32 Dec_SwitchUp(int _iChan,int _iPic);

/*
	切换到下一个设备
*/
td_s32 Dec_SwitchDown(int _iChan,int _iPic);

/*
	打开某个画面的音频
*/
td_s32 Dec_StartAudio(int _iChan,int _iPic,int _iLoop);


/*
	关闭某个画面的音频
*/
td_s32 Dec_StopAudio(int _iChan,int _iPic);


/*
	与某个画面进行对讲操作
*/
td_s32 Dec_StartTalk(int _iChan,int _iPic);

/*
	停止与某个画面进行对讲操作
*/
td_s32 Dec_StopTalk(int _iChan,int _iPic);


/*
	对某个画面进行设备控制操作
*/
td_s32 Dec_PtzCtrl(int _iChan,int _iPic,td_s32 _iAction ,PTZ_DATA* _pData);


/*
	向前端某个画面发送485数据
*/
td_s32 Dec_PtzClarity(int _iChan,int _iPic,td_u8* _u8Data,int _iDataLen);


/*
	解码器客户端开始PUSH流操作
	返回:新创建的用于push流操作的通道号	
*/
td_s32 Dec_StartPush(int _iChan,int _iPic,VENC_TYPE_E _VencType,VIDEO_SIZE_E _VideoSize, const char* _VencPsw, int _iAudioSample);

td_s32 Dec_SetPushData(td_s32 _s32Chn,td_u8* _u8DataBuf,int _iDataLen);


/*
	解码器客户端停止PUSH流操作
*/
td_s32 Dec_StopPush(int _iChan, int _iSaveLastFrame, int _iFrom);

/*
	设置解码器的报警联动关系
	该联动关系对所有的数字通道均生效
	对应第0号报警输出端口
	_iAlarmOut 	0x01:端口报警
				0x02:丢失报警
				0x04:移动报警
	_iAlarmOutStat		0:联动打开
					1:联动关闭
*/
td_s32 Dec_SetAlarmLink(int _iAlarmOut,int _iAlarmOutStat);

/*
	获取解码器的报警联动关系
*/
td_s32 Dec_GetAlarmLink();


td_s32 Dec_GetCurNvsItem(int _iChan,int _iIndex,TDECNVSITEM* _pNvsItem);

td_s32 Dec_GetCurTalkState(int _iChan,int _iIndex);

td_s32 Dec_GetCurAudioState(int _iChan,int _iIndex);

td_s32 Dec_ConnectStateChange(int _iChn,int _iState);

int Dec_GetDebug(char *_cString,int _iLen);

td_s32 Dec_AddPTZProtocols(td_s8 _cProtocol[MAX_PROTOCOL_NAME_LEN]);

td_s32 Dec_WritePTZProtocols(int _len, char * _buf);

void Dec_AlarmChange(int _iChn,TAlarmType _alarmType,int _iStat);

td_s32 Dec_LoopListClear(int _iChn,int _iPic);

td_s32 Dec_LoopListClearAll();

int Dec_ChnListFindByPos(int _iVoDev,int _iVoChn);

td_s32 Dec_GetConnetcStat(int _iChan,int _iPic);

void Dec_AlarmNotifyToUI(int _iChn,TAlarmType _alarmType,int _iStat);

void Dec_AlarmNotifyToUIByPos(int _iChn, int _iChan, int _iPic, TAlarmType _alarmType,int _iStat);

void Dec_GetCurrentPic(int* _iChan,int* _iPic);

void Dec_SelectPicNotieyToUI(int _iChan,int _iPic);

void Dec_PicsNotifyToUI(int _iChan,int _iPics);

void Dec_ResChangeNotifyToUI(int _iChan, int _iVgaMode);

void Dec_LogoNotifyToUI(int _iChan);

void Dec_LogoNotifyToUIEx(int _iChan);

void Dec_PtzNotifyToUI(int _iChan,int _iPic,int _iCtrlStat);

void Dec_ComAttrNotifyToUI(int _iChan);

td_s32 Dec_SwitchUp(int _iChan,int _iPic);
td_s32 Dec_SwitchDown(int _iChan,int _iPic);

td_s32 Dec_NameFifo_Init();

td_s32 Dec_Send485DataToFifo(char *_buf, int _len);

td_s32 Dec_Exit();
int Dec_LoopListGetAudioStat(int _iChan,int _iPic);
int Dec_LoopListGetTalkStat(int _iChan,int _iPic);
//td_s32 Dec_BigSmallSwitch(int _iChan);

void Dec_AlarmDisableToUI(TAlarmType _alarmType);
void Dec_AlarmEnableToUI(TAlarmType _alarmType);
int Dec_LoopListGetLoopStat(int _iChan,int _iPic);
td_s32 Dec_SetDebugFlag(td_s32 _s32Flag);

td_s32 Dec_SetPreviewPrefer(int _iChan, int _iEnable);
td_s32 Dec_SetSaveLastFrame(int _iChan, int _iEnable);
td_s32 Dec_ChanNoSwitch(int _iChan);
td_s32 Dec_Save_Bnc_mode(td_s32 _iMode);
td_s32 Dec_VoDevGetVgaPixel(int _iChan);
int Dec_GetInitFlag();
void  Dec_RestartApp(DEC_RESTARTAPPCALLBAK  _Fun); 
#endif
