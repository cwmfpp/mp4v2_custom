/*******************************************************************
文件名	:TD_Preview.h
公司	:天津天地伟业数码科技有限公司
创建者	:王霄
创建日期:2010-02-25
简要描述:视频预览模块头文件
        
******************************************************************/
#ifndef _PREVIEW_H
#define _PREVIEW_H

#include "td_types.h"

#define PV_MAX_QUEUE_LEN	64
#define PV_MAX_VO			32
#define PV_VO_MAX_DEV_NUM 			4		//最大输出设备

typedef enum
{
	PV_MOD_ERR 		= -1,	// 错误
	PV_SINGLE_PIC 	= 1, 	// 单画面

	PV_PIC_IN_PIC 	= 2, 	// 画中画
	PV_PIC_IN_PIC_1 = 2001,
	PV_PIC_IN_PIC_2 = 2002,
	PV_PIC_IN_PIC_3 = 2003,
	PV_EZOOM		= 2011,

	PV_3PICS 		= 3,
	PV_3PICS_1 		= 3001,
	PV_4PICS_MIN	= 4000,
	PV_4PICS_MAX 	= 4301,
	PV_USER_DEF = 0xFFFF,  //自定义模式
} PV_MOD;


typedef struct
{
    int m_iVoChnCnt;//自定义画面分割方式下画面数
    TRect m_tVoAreas[PV_MAX_VO]; // PV_MAX_VO=32
}TCustomPreviewParam;


typedef struct
{
	td_s32 m_iPicCnt;									//预览模式
	td_s32 m_iChnCnt[PV_MAX_VO];						//队列长度
	td_s32 m_iChnList[PV_MAX_VO][PV_MAX_QUEUE_LEN];		//队列列表
	td_s32 m_iSwitchTime;								//巡航时间间隔
	TCustomPreviewParam m_tCustomPvPara;//自定义画面分割方式
}TPreviewParam;




//操作状态
typedef enum
{
	PV_STA_ERR = -1,	//错误
	PV_STA_PUASE = 0, 	//预览暂停
	PV_STA_NORMAL, 	//正常预览
	PV_STA_SINGLE_PIC,	//单画面
	PV_STA_EZOOM,		//电子放大
} PREVIEW_STA;


//----------------------------接口函数---------------------------
td_s32 preview_Init_Ex(td_s32 _iVoDev, td_s32 _iVoDevSize, td_s32 _iWbcBindVoDev);

td_s32 preview_Init(td_s32 _iVoDevCnt, td_s32 _iVoDevIds[], td_s32 _iVoDevSize[], td_s32 _iVoDevIntfType[], td_s32 _iWbcBindVoDev);
/*
 * 函数名称：PreviewInit
 * 简要描述: 视频预览模块初始化
 *
 * 输     入: _iCnt: 预览画面数
 * 输	  出： 0:初始化成功
 			  -1:初始化失败

*/

td_s32 preview_Uninit();

// 设置预览总性能1个D1等价于100
td_s32 preview_SetMaxPerformance(td_s32 _iMaxPerformance);


td_s32 preview_SetConfig(td_s32 _iVoDev, TPreviewParam *_ptArg);
/*
 * 函数名称：PreviewSetConfig
 * 简要描述: 设置预览配置
 *
 * 输       入: _DispDev:      预览设备
 * 输	  出：
*/


td_s32 preview_StartSwitch(td_s32 _iVoDev);
/*
 *函数名称: PreviewStartSwitch
 *简要描述: 启动预览切换

 *输入:_iDispDev:预览设备
 *输出:

*/
td_s32 preview_StopSwitch(td_s32 _iVoDev);
/*
 *函数名称: PreviewStopSwitch
 *简要描述: 停止视频预览

 *输入:_iDispDev:预览设备
 	  :_iPicCnt :预览画面数
 *输出:

*/

td_s32 preview_PageUp(td_s32 _iVoDev);
/*
 *函数名称: PreviewPageUp
 *简要描述: 视频预览上一页

 *输入:
 *输出:

*/
td_s32 preview_PageDown(td_s32 _iVoDev);
/*
 *函数名称: PreviewPageDown
 *简要描述: 视频预览下一页

 *输入:
 *输出:

*/

td_s32 preview_EnterSinglePic(td_s32 _iVodev, td_s32 _iChn);
/*
 *函数名称: PreviewEnterSinglePic
 *简要描述: 单画面预览

 *输入:_iVodev:预览设备
 		_iChn:预览通道
 *输出:

*/
td_s32 preview_ExitSinglePic(td_s32 _iVodev);
/*
 *函数名称: PreviewEnterSinglePic
 *简要描述: 退出单画面预览

 *输入:_iVodev:预览设备
 		_iVoChn:预览通道
 *输出:

*/

td_s32 preview_EnterEzoom(td_s32 _iVodev, td_s32 _iChn);

/*
 *函数名称: PreviewEnterEzoom
 *简要描述: 进入电子放大

 *输入:_iVodev:预览设备
 		_iChn:预览通道
 *输出:

*/
td_s32 preview_ExitEzoom(td_s32 _iVodev);
/*
 *函数名称: PreviewExitEzoom
 *简要描述: 退出电子放大

 *输入:_iVodev:预览设备
 		_iVoChn:预览通道
 *输出:

*/

// 设置电子放大显示区域
td_s32 preview_SetEzoomArea(td_s32 _iVoDev, TRect *_ptArea);

td_s32 preview_Pause(td_s32 _iVoDev);
/*
 *函数名称: PreviewPause
 *简要描述: 预览暂停

 *输入:_iVodev:预览设备
 *输出:

*/
td_s32 preview_Resume(td_s32 _iVoDev);
/*
 *函数名称: PreviewResume
 *简要描述: 恢复预览

 *输入:_iVodev:预览设备
 *输出:

*/
td_s32 preview_GetCurStat(td_s32 _iVoDev);
/*
 *函数名称: PreviewCurStat
 *简要描述: 当前预览状态

 *输入:_iVodev:预览设备
 		_iVoChn:预览通道
 *输出:

*/

// 获取预览切换状态
td_s32 preview_IsSwitch(td_s32 _iVoDev);

// 更改当前预览模式
td_s32 preview_ChgMod(td_s32 _iVoDev, td_s32 _iMod);

td_s32 preview_GetCurMode(td_s32 _iVoDev);
/*
 *函数名称: PreviewGetMode
 *简要描述: 获取当前预览模式

 *输入:_iVodev:预览设备
 		_iVoChn:预览通道
 *输出:

*/
td_s32 preview_GetCurDspChn(td_s32 _iVoDev, td_s32 _iVoChn);
/*
 *函数名称: PreviewGetCurDsp
 *简要描述: 获取当前预览显示

 *输入:_iVodev:预览设备
 		_iVoChn:预览窗体号
 *输出:

*/

// 临时修改窗口画面显示
td_s32 preview_ChgCurDspChn(td_s32 _iVoDev, td_s32 _iVoChn, td_s32 _iViChn);

td_s32 preview_SetFocus(td_s32 _iVoDev, td_s32 _iVoChn);
/*
 *函数名称: PreviewSetFocus
 *简要描述: 设置预览焦点(类似"红框")

 *输入:_iVodev:预览设备
 		_iVoChn:VO窗口号
 *输出:

*/

td_s32 preview_PicMap(td_s32 _iVoDev, td_s32 _iChn, TRect *_ptRect);
td_s32 preview_PicUnMap(td_s32 _iVoDev);


td_s32 preview_SetVoDevSize(td_s32 _iVoDev, td_s32 _iSize);


td_s32 preview_ChannelOsdChangeNotify(td_s32 _iChn);

td_s32 preview_SetFocusVoDev(td_s32 _iVoDev);
td_s32 preview_SetMasterVoDev(td_s32 _iVoDev);
#define preview_GetMasterVoDev()	preview_SetMasterVoDev(-1)

td_s32 preview_SetVoDevsSize(td_s32 _iHdmi0Size, td_s32 _iHdmi1Size, td_s32 _iBncSize);

// 配置文件相关
td_s32 preview_LoadCnf(td_s32 _iVoDev, TPreviewParam *_ptParam);
td_s32 preview_GetPicPoint(td_s32 _iVoDev, td_s32 _iMod, TRect * _ptRects);

td_s32 preview_SaveCnf(td_s32 _iVoDev,const TPreviewParam *_ptParam);
td_s32 preview_MakeDefaultCnf(td_s32 _iMod, td_s32 _iChnCnt, TPreviewParam *_ptParam);
// 恢复默认
td_s32 preview_DefaultCfg();

// 设置视频输入制式
td_s32 preview_SetViNorm(td_s32 _iNorm);

// 为保证回放解除/恢复数字通道预览
td_s32 preview_VdencPause();
td_s32 preview_VdencResum();

td_s32 preview_VoDevTest(td_s32 _iVoDev);

td_s32 preview_SetOldFocusVoDev(td_s32 _iVoDev);

td_s32 preview_GetOldFocusVoDev(void);


//设置TV显示区域
td_s32 preview_SetVoDevDspAreaOnScreen(td_s32 ivodev, td_s32 iXp, td_s32 iYp, td_s32 iWidth, td_s32 iHeight);
//设置TV坐标参数
td_s32 preview_SetTVParam(TDevTV * _ptDevTv);


td_s32 preview_GetBncDevMode();

////获取当前系统的合成画面主画面的通道号
td_s32 preview_CurVoMainChn(int _iVoDev,int *_piVcMainChnNo);
td_s32 Preview_SetPreviewQualityCnf(int _iPreviewCnf);
td_s32 preview_SetchgValue(int _iValue);
td_s32 Preview_SetPreviewAudioMod(int _iAOPreviewMod);
td_s32 preview_SetStartAudioLeftRight(td_s32 _iVoDev, td_s32 _iLeft, td_s32 _iRight, td_s32 _iLeft1, td_s32 _iRight1);
td_s32 Preview_SetPreviewDefaultAudioLR(int _iDefaultLR);

// for decoder临时，解码器结项时应该删除
td_s32 preview_StopCurChnDsp(td_s32 _iVoDev, td_s32 _iVoChn);
td_s32 preview_sShowNovideo(int _iNovideo);
td_s32 preview_sVoNoClearStat(td_s32 _iVoDev, td_s32 _iVoChn, td_s32 _iClear);

td_s32 preview_GetFocusVoDev(void);
td_s32 preview_GetVoDevSize(td_s32 _iVoDev, td_s32 *_piWith, td_s32 *_piHeight);

td_s32 preview_startpage(td_s32 _iVoDevId);
td_s32 preview_stoppage(td_s32 _iVoDevId);
td_s32 preview_SetNoValidSize(TVideoSize _tVideoSize[], td_s32 _iCnt);

td_s32 preview_sShowPreviewFrameEnable(int _iEnable); //设置预览黑边框使能--浙江地区需要

int Perview_GetDebugInfo(char *_cString);


#endif
