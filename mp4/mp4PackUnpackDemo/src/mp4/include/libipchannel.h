#ifndef _IP_CHANNEL_H_
#define _IP_CHANNEL_H_

#include "vca.h"
#include "td_types.h"
#include "td_errno.h"
#include "td_media.h"
#include "channel.h"
#include "libdatacenter.h"
#include "alarmer.h"
#include "libtdvo.h"
#include "libtdao.h"
#include "td_sdk.h"
#include "libdec.h"



#ifdef IP_CHANNEL_GLOBAL
#define IP_CHANNEL_EXTERN
#else
#define IP_CHANNEL_EXTERN   extern
#endif


#define MODULE_MAX_NUM   		10
#define IPCHANNEL_MAX_NUM   	(32 + 1)
#define	BIND_VO_MAXNUM			16
//登录状态
enum
{
    NOT_CONNECT           		= 0,
    CONNECT_ING ,
    CONNECT_SUCCESS ,
    CONNECT_ERROR  
};

//数字通道类型
enum
{
	TDSTREAM = 0,
	ONVIFSTREAM,
	PUSHSTREAM,
};


typedef struct	// vo绑定信息
{
	void * m_ptVo;		//vo对像
	TRect m_tRect;	//区域大小(以D1为标准)
	TRect * m_ptRect;
} IPTVoBindMsg;





typedef struct
{
	TChannel m_TChannel;
	
	int m_iLogicNo;
	
	TAlarmer *m_ptPortAlarmer;					//端口报警
	
	TDataFIFO * m_PreviewDataFifo;				//用于音视频预览  
	
	TDataPackage * m_PreviewPacket;
	
	TDataPackage * m_PreviewTmpPacket;
	int  			    m_PreviewTmpPacketType;
	
	
	TRingBuf * m_AudioRngBuf;
	volatile  int m_iASendLen;
	volatile  int m_iARnStat;
	
	TNVSMODULE * m_pModule;						//厂家类型相关  创建通道的时候赋值
	
	void *	m_pNext;
	
	int		m_iIsUse;								//是否使用
	
	int		m_iIndex;
	
	volatile  int 	m_iSata;							//连接状态	
	
	/*用于视频预览*/
	pthread_mutex_t m_MutexOpt;

	volatile  int 		m_iVencType;						//编码类型0，H.264；1，M-JPEG；2，MPEG4
	volatile  int		m_iVideNorm;						//制式    
	volatile  int		m_iFrameRate;						//帧率
	volatile  int		m_iFrameRateChgFlg;					// dxl 20120222	用于累加，给VO用
	volatile  int		m_iFrameRateChgFlg_ex;				// 不累加，给智能分析用
	volatile  int 		m_iWidth;							//图象宽度
	volatile  int 		m_iHeight;							//图象高度	
	volatile  int		m_iPreviwSata;						//预览状态
	volatile  int		m_iBusyCount;
	volatile  int		m_iVSendStat;
	volatile  int		m_iVideoSize;						//视频大小 用于统计预览性能
	volatile  int		m_iVideoProfile;
	//volatile  int		m_iVideoProfileChgFlg;
	volatile  int		m_iVideoBitRate;					//视频码率 
	volatile  int 		m_iVideoBitRateChgFlg;
	volatile  int		m_iNeedStartEncode;					//dxl 20120521
	volatile  int		m_iCVBRType;						//码流类型 变码率 定码率
	volatile  int		m_iCVBRTypeChgFlg;
	volatile  int		m_iIFrameRate;						//I 帧帧率
	volatile  int		m_iIFrameRateChgFlg;
	volatile  int		m_iQuality;
	volatile  int		m_iQualityChgFlg;
	volatile  int		m_iSensorSpin;						//视频翻转，aaron
	
	TVdecChannel *	m_pVDecChan;					//解码通道
	
	IPTVoBindMsg	m_TVoList[BIND_VO_MAXNUM];				//预览VO
	
	int 	m_iTVoNum;
	int	m_iCreateVdecFail;

	volatile int m_iRepreiw;
	
	/*用于音频预览*/
	volatile  int		m_iStreamType;						//码流类型纯音频纯视频或者复合码流
	volatile  int		m_iAEnc;							//音频编码方式
	volatile  int		m_iAFrameLen;								//音频帧长度
	volatile  int		m_iAEncold;							//音频编码方式
	volatile int		m_iAPreviwSata;						//音频预览状态
	int		m_iABusyCnt;
	int		m_iSendIndex;
	int		m_iAoDev;
	volatile  int 		m_iAoID;
	int		m_iASendStat;
	
	TChnSnapshotMsg  * m_pSnapMsg;
	
	void * m_pMchan;
	
	TIpChannelMsg m_IpChannelMsg;				//连接所需要的信息
	
	TNVSITEM m_IpChannelInfor;				//连接所需要的信息
	
	/*报警信息*/
	volatile  int  m_iPortAlarmStat;
	volatile  int  m_iVLAlarmStat;
	volatile  int  m_iMDAlarmStat;
	volatile  int  m_iVCAAlarmStat[MAX_RULE_NUM][MAX_EVENT_NUM];
	volatile  int  m_iODAlarmStat;
	
	int  m_iPortAlarmType;
	int  m_iPortAlarmEnable;
	int  m_iVLAlarmEnable;
	int  m_iMDAlarmEnable;
	
	volatile int  m_iBindNum;
	volatile int  m_iBindFlag;
	volatile int  m_iAudioSample;
	volatile int m_iAudioSampleBak;
	volatile td_u16  m_u16AFrameSize;
	/*智能分析相关*/						//zhangyi add 20120904
	td_s32			m_iVcaEnable;	
	volatile td_s32 	m_iReCreateVca;
	//TVdecChannel 	*	m_pVcaVdecPara;
	td_s32				m_iVdecChan;		//zhangyi add 20121114
	td_s32			m_iVencChan;
	td_s32				m_iVcaVideoWidth;
	td_s32				m_iVcaVideoHeight;
	td_s32				m_iVcaVideoNorm;

	//add by gyn
	td_s32 m_iAudioCheckSeed;//add by gyn 20120201 音频检测的间隔
	
	td_s32 m_iAudioStat;								//合成通道的音频是否静音
	//0代表静音，1代表有声音
	//add end

	td_s32 m_iPreviewMode; //hq 20121210 为适应16:9视频源在4:3分辨率下显示，0:平铺在显示区域，1:保持宽屏比例显示

	void* m_pSIpChan;
	td_s32 m_iSubStreamFlag;
	td_s32 m_iPreviewSubStreamFlag;

	TAlarmer * m_ptCapAlarmer[MAX_RULE_NUM];   
	TAlarmer * m_ptCpcAlarmer[MAX_RULE_NUM]; 
	TAlarmer * m_ptCddAlarmer[MAX_RULE_NUM]; 
	TAlarmer * m_ptSvdAlarmer[MAX_RULE_NUM]; 
	//报警相关
	TAlarmer *m_ptOdAlarmer;		//视频遮挡报警;		zhangyi add 20130411

	//add for decoder 注:该部分需要更改，添加采样率，fifo和center需要移入上层实现和维护
	volatile  int		m_iTalkEnc;
	TDataFIFO * m_TalkDataFifo;	
	TDataCenter* m_TalkDataCenter;
	volatile  int   m_iTalkSata;				//对讲状态
	volatile  int   m_iTalkWait;				//对讲状态
	int m_iStartViewFlag;
	
	td_s32 m_iPreviewPrefer;// 预览偏好
	long long m_llLastSendFrameTime;// 最后一次发送帧的时间

	td_char cIpcVersion[32];
	td_s32 m_iUpgrateSta;
	td_s32 m_iUpgrateStaHeartbeat;
	char m_cVideoEncrypt[ENCRYPT_PWD_LENGTH]; // 0: 不加密1: AES加密 
	td_s32 m_iScrect;

	td_s32 m_iMedleyAvEnable;
	td_u32 m_uiLostAudioFrmNum;
	td_s32 m_iPreviewAudioTime;
}IpChannel, *pIpChannnel;


typedef struct
{
	TAlarmer m_tAlarm;
	IpChannel * m_ptChn;
	//move from 3520 by caizhaoxu for vca 20120321
	td_s32 m_iRuleNo;   //zhangweiwei add for VCA
} IpChnAlarmer;

typedef struct
{
	TNVSMODULE * m_pModule[MODULE_MAX_NUM];		
	char 	m_cLibName[MODULE_MAX_NUM][256];	
	void  * m_dp[MODULE_MAX_NUM];		
	int 	m_iModuleNum;
	int 	m_iChannelNum;
	char	m_cVersion[64];
	int 	m_iIsInit;
	
	int (*SearchReportCallBack)(TIpcDevItem *_ptItem);
	int 	m_iSerchModuleNum;		//搜索类型
	int 	m_iSerchType;				//搜索类型
	int 	m_iSerchModu;				//搜索类型
	volatile  int 	m_iLiveTime;				//搜索有效时间
	int 	m_iEndSerchCount;			//搜索有效时间

	int 	m_iMaxBitRate;

	volatile  int 	m_iSnapFlag;				
	
	pthread_mutex_t m_MutexSerch;

	

	int (*ChnAlarmerRegisterCallBack)(int _iId, int _iType, TAlarmer *_ptAlarm);

	int (*ChnStreamCryptCallBack)(void *_pBuf, td_s32 _iSize, td_u32 _uiKey,  td_u8 *_strPsw, td_s32 _iPswLen);
	
	int (*ChnComDataProcCallBack)(int _iChn, int _iComdId, int _iLength, void *_pBuf);
}IpChannelMang;



td_s32 Init(void *_ptParam);



typedef struct				//通道结构体
{
	td_s32 m_iChnNo;	 				//通道号(数字通道是为对方通道号)
	td_s32 m_iAencType;				//音频编码模式
	td_s32 m_iAudioSample;			//音频采样率
	td_s32 m_iAencStart;				//音频编码是否启动
	td_s32 m_iAudioFrameSize;			//音频帧长度
	td_s32 m_iSendAudio;				//是否需要进行音频发送操作
	td_s32 m_iStreamType[2];			//码流类型
	TRingBuf* 			m_ptAudioBuf[2];	//
	TRingBuf* 			m_ptAsRingBuf;			//用于记录上传音频数据的环形Buf
	pthread_mutex_t m_tAudioLock; //dlx 20110408 添加音频缓存锁	
	//TDataPackage* m_ptPackage;
} TLocalAudioChannel;



#undef  IP_CHANNEL_EXTERN

#endif





