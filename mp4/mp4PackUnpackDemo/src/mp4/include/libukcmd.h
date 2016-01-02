/*******************************************************************
文件名	:GuiCmdExcute.h
公      司	:天津天地伟业数码科技有限公司
创建者	:高旭麟
创建日期:2008-10-09
简要描述:主控与UI  命令数据交换接口函数头文件
修改记录:2008-10-10,高旭麟  创建
        
******************************************************************/
#ifndef  GUICMDEXECUTE_H
#define  GUICMDEXECUTE_H

//#include "guiparameter.h"
void UKCmd_CreateRecvThread(td_s32 _iTid, td_s32 _iWait, td_s32 *_iHeardbeat);
void UKCmd_Start(td_s32 _iTid, td_s32 _iWait, td_s32 *_iHeardbeat);
void UKCmd_CreateDataProcThread(td_s32 _iTid, td_s32 _iWait, td_s32 *_iHeardbeat);

// 网络<->外挂透明数据
td_s32 UKCmd_SendAppString(td_u32 _uiIp, char *_str);
td_s32 UKCmd_SetAppStringProcFun(void (*_Fun)(td_u32 _uiIp, char *_str));


// UK透明数据
td_s32 UKCmd_SendUKData(void *_pData, int _iSize);
td_s32 UKCmd_SetUKDataProcFun(void (*_Fun)(void *_pData, int _iSize));
td_s32 UKCmd_SetChnCommDataFun(td_s32 (*ChnComDataProc)(td_s32 _iChn, td_s32 _iComdId, td_s32 _iLength, void *_pBuf));

// 串口数据
td_s32 UKCmd_SendComData(int _iComd, void *_pData, int _iSize);

td_s32 UKCmd_ClearTalkStat();

td_s32 UKCmd_SendComDataToApp(td_u32 _iComId, char *_str,  int _iSize);

td_s32 UKCmd_SendDataToApp(td_u32 _iComId, char *_str, int _iSize,int _itype);//modify by zhuangzhibing

td_s32 UKCmd_ClearRingBuf();//add by zhuangzhibing 20120215 清空环形buffer

#define GUI_RECORD_STATE
#ifdef GUI_RECORD_STATE
#define GUI_IDLE_STATE 0
#define GUI_SPIC_PREVIE_STATE 1
#define GUI_JPEG_VIEWER_STATE 2
#define GUI_VIDEO_PLAYER_STATE 3
#define GUI_TRACK_PTZ_STATE 4
#define GUI_ZOOM_STATE 5
#endif
//add by sunlining
td_s32 UKCmd_ReburnCreatFileReq(td_s32 _iDiskNo,const char *_pcPath);   
/*
通知外挂创建播放器配置文件
*/
td_s32 UKCmd_ReburnRmFileReq(const char *_pcPath);
/*
通知外挂删除上一次生成的配置文件，如果审讯中配置文件已经存在则不删除。
*/


//td_s32 UKCmd_SendOsdWord(td_s32  _iChn,td_s32 _iX,td_s8* _pstrOsdtext);

/*
通知外挂网页设置的附加字符
*/


td_s32 UKCmd_Burn_Send_Encrypt(int iChn);
/*
通知外挂创建密码字段
*/
td_s32 UKCmd_SettingRenewReq();
/*
通知外挂恢复默认
*/
//td_s32 UKCmd_Set307PTimeReq();
/*
通知外挂给307p校时
*/

td_s32 UKCmd_FormatNoteWgReq();
/*
通知外挂断开数据库的连接
*/
td_s32 UKCmd_PoweroffStatRsp();
/*
通知外挂发送给中控关机指令
*/
td_s32 UKCmd_FormatStatRsp();
/*
通知外挂格式化完成
*/

int UKCmd_DvdNumStatusToApp(int _iOkNum, int _iErrNum);
/*
通知外挂 开一次庭所产生的好盘与坏盘
*/

/*
UI设置多画面时，通知外挂
*/
td_s32 UKCmd_SetPreviewReq(TGuiSetPreviewReq *_ptReq);

td_s32 UKCmd_VideoHeaderToApp(void *_pData);

//add end
//liyang add 
td_s32 UKCmd_GetUKTid(td_s32 _iTid);
//add end
/*
通知外挂激励
*/
td_s32 UKCmd_SendBestirToApp(int _iChn, int _iStat);

//创建语音激励广播线程
void CreateVoiceBestirThread(void *_pArg);
//设置通道最后一次激励的时间
void SetLastBestirTime(int _iChn, int _iTime);

td_s32 UKCmd_ForceIframeWgReq(int _iChn);//向外挂请求强制I帧

#endif

