#ifndef _H_TD_STREAM_H_
#define _H_TD_STREAM_H_

#include "sysnotify.h"
#include "libtiandy.h"
#include "libother.h"
#include "hisi_head.h"

#define BASE_VO_VDEC_FRATE			10000
#define BASE_VO_VI_FRATE			20000
#define BASE_VO_CASCADE_VI_FRATE 	30000
#define BASE_VO_PCI_FRATE			40000

#define SNAP_FILE_NAME_LEN	64
#define DEFAULT_PIC_QUALITY	3	//默认抓拍图片质量
#define SNAP_HD				0
#define SNAP_SD				1

#define VI_EZOOM	0
#define VDEC_EZOOM	1

#define VDEC_CHAN_NUM		20		
#define ADEC_CHAN_NUM		4

#define VIR_CHN_1			16  //合成通道1的通道号



#define VIR_VO_VPSS_1		VI_VPSS_VIRVO_START		//48	//合成通道1的vpss通道号
#define VIR_VO_VENC_1_MAIN	48	//合成通道1的主码流编码通道号
#define VIR_VO_VENC_1_SUB	50	//合成通道1的副码流编码通道号
#define VIR_VO_VENC_1_CAP	52	//合成通道1的抓拍编码通道号

#define VIR_VO_VENC_2_MAIN	49	//合成通道2的主码流编码通道号
#define VIR_VO_VENC_2_SUB	51	//合成通道2的副码流编码通道号
#define VIR_VO_VENC_2_CAP	53	//合成通道2的抓拍编码通道号


#define AODEV_NUM_BASS	((td_s32)MediaGetChipInfo(MEDIA_INFO_AOBASSDEV))
#define AUDIO_OUT_DEV 	0			
#define AUDIO_OUT_CHN 	0

//暂时使用，改写后，下面两个宏可能不在需要
#define MIC_AI_DEV	tdmedia_InitMicDev()        //mic 音频输入设备号wsl 20111222 3531修改

#define HDMI_AO_DEV ((td_s32)MediaGetChipInfo(MEDIA_INFO_AOHDMIDEV))			//hdmi输出设备号
#define TC_908NC_AI_CNT 5

//音频功能类别划分
#define F_MICIN			0x01 //1st tlv320 用于麦克输入
#define F_AOUT			F_MICIN //1st tlv320 用于本地输出
#define F_HD_AIN		0x02 // 2nd,3rd,4th,5th tlv320 用于高清通道输入
#define F_SD_AIN		0x03 //使用AD芯片提供标清音频输入

//#define	OSD_OVERLAYEX_MAX_LEN 796 //hq, sdk7.1以前限制叠加在vi上的不得超过800宽600高
#define	OSD_OVERLAYEX_MAX_LEN 1920 /*hq, sdk7.2以后限制叠加在vi上的不得超过1920宽1080高
									同时面积不能超过800x600,这里我们未使用面积限制*/

typedef struct
{
	td_u32 m_uPoolId;
	td_u32 m_uPhyAddr;
	td_u32 m_VbBlk;
	td_u8  *	m_VirAddr;
	td_u32		m_uBlkSize;
}strVbCoach;

typedef struct
{
	td_u8 *pu8Addr[2];
	td_u32 u32Len[2];
}strVencPack;

typedef struct 
{
	td_u32 u32PackCount;
	td_u32 u32Seq;
	td_u32 u32FrameType;	//帧类型    FRAME_I = 0,   FRAME_P = 1;  
	td_u64 u64PTS;			//时间戳, us
	strVencPack* pstPack;   //gxl modified 20110323
}strVencStream;

typedef struct 
{
	td_u8 *pStream; /*stream buffer */
	td_u32 u32Len; /*stream lenth*/
	td_u64 u64TimeStamp; /*frame time stamp*/
	td_u32 u32Seq; /*frame seq,if stream is not a valid frame,u32Seq is 0*/
}strAencStream;

typedef td_s32 (*VIDEOSTREAMCBK)(td_s32 _s32Chn, td_s32 _s32IsMainChn, strVencStream* _pstVStream);
typedef td_s32 (*AUDIOSTREAMCBK)(td_s32 _s32Chn,strAencStream* _pstAStream);
typedef td_s32 (*SNAPSHOTCBK)(td_s32 _s32Chn, td_s32 _s32BuffLen);

typedef struct
{	
	td_s32		m_s32VencChn;			//编码通道号，内部使用，设置时无效
	td_s32 		m_s32ChnType;			//编码通道类型0 模拟通道2合成编码通道
	td_s32		m_s32EncType;			//编码类型0 H264 1 JPEG 
	td_s32		m_s32VideoSize;			//视频大小
	td_s32 		m_s32FrameRate;			//编码帧率
	td_s32 		m_s32BitRate;			//码率
	td_s32		m_s32IFrameRate;		//I帧帧率
	td_s32 		m_s32MainStream;		//主副码流标识 0 主码流 1副码流
	td_s32 		m_s32StreamType;		//码流类型 0 定码率 1 变码率
	td_s32 		m_s32Quality;			//视频质量
	td_s32		m_s32VoDev;				//vo编码设备号，仅vo编码通道有效
	td_s32		m_s32SlaveStreamType;	//启动从片编码时的码流类型(音视频 纯视频)
	td_s32		m_s32VideoProfile;		//视频profile类型
	pthread_mutex_t	m_vencLock;			//lxy add 20120515
} strVencParameter;

typedef struct
{
	strVencParameter 	m_stVencParamter;
	td_s32				m_Reserved[4];  //m_Reserved[1]为码流回调函数指针，
	 									//m_Reserved[2]为抓拍回调函数指针，
	 									//m_Reserved[3]为该通道最大编码能力
	VIDEOSTREAMCBK m_cbkVStream; 		//视频回调函数
}strVencParam;
#if 0
typedef struct
{
	td_s32 		m_s32AencChn;	//音频编码通道号，内部使用，设置时无效 
	td_s32 		m_s32AudioType;	//音频编码类型
	pthread_mutex_t	m_aencLock;			//ljj add 20121221
	td_s32		m_Reserved[4];
	td_s32		m_s32AudioSample;// huaqiang add 20130105 for AAC
	AUDIOSTREAMCBK m_cbkAStream;
}strAencParam;
#else
typedef struct
{
	td_s32 		m_s32ChnCount;		//注册通道个数
	td_s32 		m_s32ChnNum[16];	//每个注册通道的通道号
	td_s32 		m_s32RegEnable;     //使能时，使用复制模式，否则非复制模式
}strChnRegister;

typedef struct
{
	td_s32 		m_s32AencChn;	//音频编码通道号，内部使用，设置时无效 
	td_s32 		m_s32AudioType;	//音频编码类型
	pthread_mutex_t	m_aencLock;			//ljj add 20121221
	td_s32		m_Reserved[4];
	td_s32      m_s32AudioSample;
	strChnRegister m_register;
	AUDIOSTREAMCBK m_cbkAStream;
}strAencParam;

#endif

typedef struct
{
	td_s32 		m_s32PicChn;	//抓拍编码通道号，内部使用，设置时无效 
	td_s32 		m_s32VideoSize;	//图片大小  数字通道抓拍高16位表示高度，低16位表示宽度
	td_s32 		m_s32Quality;	//图片质量
	td_s32		m_iVoDev;		
	td_s32		m_iVoChan;
	void * 		m_pChan;		//数字通道地址
	td_u8		m_FileName[SNAP_FILE_NAME_LEN]; //抓拍文件名
	td_s32		m_s32BuffLen;	//buffer长度
	td_u8*		m_pu8Buffer;	//buffer
	pthread_mutex_t	m_vencLock;	
	td_s32		m_Reserved[4];
	SNAPSHOTCBK m_cbkSnapShot;
}strPicParam;


typedef struct  //视频参数，-1表示不设置，保持原值
{
	td_s32 m_s32Bright;
	td_s32 m_s32Contrast;
	td_s32 m_s32Saturation;
	td_s32 m_s32Hue;
}strVideoParam;



typedef enum 
{
	VO_ENABLE = 0,
	VO_DISABLE,
	VO_PAUSE,
	VO_RESUME,
	VO_STEP,
	VO_HIDE,
	VO_SHOW,
	VO_CLEAR,
	VO_REFRESH,
	VO_BUTT,
}VO_CTRL_E;

typedef enum
{
	VO_VIDEOLEVEL = 0,
	VO_PIPLEVEL,
	VO_MULTISCREEN,//add by gyn 
	VO_LEVENBUTT,
}VO_CHN_LEVEL_E;

typedef struct
{
	td_s32	m_iType; 		//解码类型	0 :H264  1: JPEG
	td_s32	m_iMod; 		//制式	
	td_s32  m_iHeight;		//视频高度	
	td_s32	m_iWidth;		//视频宽度
}strVdecPara;

//hq add, 20130424
#define MEDIA_INFO_TYPE_CNT 64
#define MEDIA_INFO_MAXAIDEV_CNT 8
//存储MEDIA_INFO_AICONF中定义的各类信息
typedef struct
{
	td_s32 s32MediaInfo[MEDIA_INFO_TYPE_CNT];  //其中MEDIA_INFO_AICONF信息不在此处存储
}stChipMediaInfo; 

//存储MEDIA_INFO_AICONF信息,例如3531的16路标清dvr, 使用的是ai1含16个通道
//那么s8NormAiCnt=1, s8NormAiChnCnt=16,s8NormAiDevList[0]=1
typedef struct
{
	td_s8 s8NormAiCnt;		//普通ai的数量
	td_s8 s8MicAiCnt;		//对讲ai的数量
	td_s8 s8NormAiChnCnt;	//普通ai设备下的通道数量
	td_s8 s8MicAiChnCnt;	//对讲ai设备下的通道数量
	td_s8 s8NormAiDevList[MEDIA_INFO_MAXAIDEV_CNT];	//普通ai的设备号依次取值,不存在为-1
	td_s8 s8MicAiDevList[MEDIA_INFO_MAXAIDEV_CNT];	//对讲ai的设备号依次取值,不存在为-1
}stMediaAiInfo; 

// 芯片媒体配置差异类型定义
typedef enum
{
	//系统
	MEDIA_INFO_DDRCTL = 0,		//ddr控制器的数量
	MEDIA_INFO_HDDEV,			//是否是高清设备0:sd; 1:hd
	//AI
	MEDIA_INFO_AICONF,			//设置普通音频及对讲使用的设备数和设备值
	//AO
	MEDIA_INFO_AOBASSDEV,		//Ao输出设备
	MEDIA_INFO_AOHDMIDEV,		//hdmi 音频输出设备
	//VI
	MEDIA_INFO_VIDEVCOEF,		//逻辑vi设备同物理vi设备转换的系数
	//VO
	MEDIA_INFO_BT1120,			//是否支持bt1120, 0:不支持; 1:支持
	MEDIA_INFO_HDVOCNT, 		//HD VO设备数量
	MEDIA_INFO_MAXVOCHN,		//VO设备通道数量
	MEDIA_INFO_VIRVOSTART,		//虚拟vo设备的起始号
	//VENC
	MEDIA_INFO_PROFILELEV,		//编码h264的级别
	MEDIA_INFO_IPSNAPNUM,		//JPEG抓拍编码器数量
	//VDEC
	MEDIA_INFO_MAXH264DECSIZE,		//解码或者回放的最大解码大小
	MEDIA_INFO_MAXSNAPDECSIZE,		//JPEG抓拍回放大小
	MEDIA_INFO_MAXH264VDECNUM,		//vdec解码器的数量
	//M7.6以后升级只允许在后面追加
	MEDIA_INFO_VIRVOINTYSYNC,		//虚拟VO的时序信息
} MEDIA_INFO_TYPE;
//end

#define DECODE_MODE_FRAME 0   //帧模式解码
#define DECODE_MODE_STREAM 1  //流模式解码
#define VI_MODE_LOCAL		2

#if 1
/********************td_media.c********************************/
td_s32 tdmedia_Init(td_u8 _u8InputNorm, td_u8 _u8BncInput, td_u8 _u8IpcInput, td_u8 _u8VcInput, 
	td_s32 _uiCodePerformance,td_s32 _s32BncBits, td_s32 _s32ADChkMode);
/****************************************
	功能:	初始化媒体层	
	参数:	_s32VideoNorm: 视频输入制式
			_s32ViNum:实际启动vi数
			_s32MaxDecAbilty:最大解码能力
			_s32BncBits:模拟通道类型，按位表示，0 本地模拟通道 1 pci通道
	输出:

			
	修改日志:20110301 高旭麟创建
			 20110302 去掉_s32ChnCnt
			 20110307 修改接口
			 20110506 增加_s32ViNum
*****************************************/

td_s32 tdmedia_InitLocalChn(td_s32 _s32Chn,td_s32 _samplerate, td_s32 _s32AencType);
/****************************************
	功能:	初始化本地通道所需的媒体层资源，函数从通道层create中被调用
	参数:	_s32Chn:通道号，本地通道为bnc号
	输出:
	修改日志:20120829 hq创建:将tdmedia_Init中与通道相关的部分提取出来 
				   20130107 huaqiang 修改: 添加音频编码方式参数
*****************************************/

td_s32 tdmedia_InitPciChn(td_s32 _s32Chn,td_s32 _samplerate, td_s32 _s32AencType);
/****************************************
	功能:	初始化PCI通道所需的媒体层资源，函数从通道层create中被调用
	参数:	_s32Chn:通道号，本地通道为bnc号
	输出:
	修改日志:20120829 hq创建:将tdmedia_Init中与通道相关的部分提取出来 
				  20130107 huaqiang 修改: 添加音频编码方式参数
*****************************************/

td_s32 tdmedia_InitVcChn(td_s32 _s32Chn);
/****************************************
	功能:	初始化VC通道所需的媒体层资源，函数从通道层create中被调用
	参数:	_s32Chn:通道号，vc通道号从VIR_CHN_1起增加
	输出:
	修改日志:20120829 hq创建:将tdmedia_Init中与通道相关的部分提取出来 
*****************************************/

td_s32 tdmedia_InitSdiPciChn(td_s32 _s32Chn, td_s32 _s32AudioChn, td_s32 _s32Samplerate, td_s32 _s32AencType);
/****************************************
	功能:	初始化SDIPCI通道所需的媒体层资源，函数从通道层create中被调用
	参数:	_s32Chn:通道号，本地通道为bnc号，
	输出:
	修改日志:20120829 hq创建:将tdmedia_Init中与通道相关的部分提取出来 
				  20130107 huaqiang 修改: 添加音频编码方式参数
*****************************************/

td_s32 tdmedia_InitSdiLocalChn(td_s32 _s32Chn, td_s32 _s32AudioChn, td_s32 _s32Samplerate, td_s32 _s32AencType);
/****************************************
	功能:	初始化SdiLocal通道所需的媒体层资源，函数从通道层create中被调用
	参数:	_s32Chn:通道号，本地通道为bnc号，
	输出:
	修改日志:201301 shihc create
*****************************************/


td_s32 tdmedia_CreatePool(td_s32 _iVW, td_s32 _iVH, strVbCoach * _pstPool);
td_s32 tdmedia_DestoryPool(strVbCoach * _pstr_pool);
td_s32 media_DestoryCoach(void **Coach);
void *media_initCoach(td_s32 _iW, td_s32 _iH, int *_CoachAdr, int *_DataHeader);
td_s32 media_GetFrmInfoStrucLen(void);
td_s32 media_VdecGetStreamData(td_s32 _s32ID, void *_Coach);
td_s32 media_VpssSendStreamData(td_s32 _s32Dev, td_s32 _s32Chn, void *_Coach);

#endif
#if 1
/********************td_media_pub.c********************************/
td_s32 MediaSetChipInfo(td_s32 _s32Type, void * _s32Value);
/****************************************
	功能:	由于海思芯片不同初始化媒体层同hi3531的差异部分
	参数:	_s32Type: 某类媒体层差异
	              _s32Value: 实际平台使用的值
	输出:
	修改日志:20130403 hq创建
*****************************************/

void * MediaGetChipInfo(td_s32 _s32Type);
/****************************************
	功能:	由于海思芯片不同,获取某类差异同hi3531的差异情况
	参数:	_s32Type: 某类媒体层差异
	输出:
	修改日志:20130403 hq创建
*****************************************/

td_s32 MediaInitChipInfo(void);
/****************************************
	功能:	按照hi3531的情况初始化媒体层资源
	参数:	
	输出:
	修改日志:20130403 hq创建
*****************************************/
#endif
#if 1
/********************音视频编码td_media_avenc.c*********************************/
td_s32 tdmedia_StartAenc(td_s32 _s32Chn, strAencParam* _pstAencParam); 
/****************************************
	功能:	启动音频编码
	参数:	_s32Chn 音频编码通道号
			_pstAencParam 音频编码参数
	输出:


	修改日志:20110301 高旭麟创建
*****************************************/


td_s32 tdmedia_StopAenc(td_s32 _s32Chn); 
/****************************************
	功能:	停止音频编码
	参数:	_s32Chn 音频编码通道号
	输出:


	修改日志:20110301 高旭麟创建
*****************************************/


td_s32 tdmedia_StartTalk(strAencParam* _pstAencParam); 
/****************************************
	功能:	启动对讲编码
	参数:
			_pstAencParam 音频编码参数
	输出:


	修改日志:20110322 高旭麟创建
*****************************************/


td_s32 tdmedia_StopTalk(); 
/****************************************
	功能:	停止对讲编码
	参数:
	输出:


	修改日志:20110322 高旭麟创建
*****************************************/

void tdmedia_StartTalkStreamProc();
/****************************************
	功能:	
	参数:
	输出:
	修改日志:20120704  create
*****************************************/

void tdmedia_StopTalkStreamProc();
/****************************************
	功能:	
	参数:
	输出:
	修改日志:20120704  create
*****************************************/

td_s32 tdmedia_StartVenc(td_s32 _s32Chn, td_s32 _s32IsMainChn, strVencParam* _pstVencParam); 
/****************************************
	功能:	启动视频编码	
	参数:	_s32Chn 视频编码通道号
			_s32IsMainChn: 主副码流0主1副
			_pstVencParam: 视频编码参数
	输出:


	修改日志:20110301 高旭麟创建
*****************************************/


td_s32 tdmedia_StopVenc(td_s32 _s32Chn, td_s32 _s32IsMainChn); 
/****************************************
	功能:	停止视频编码	
	参数:	_s32Chn 视频编码通道号
			_s32IsMainChn 主副码流0主1副
	输出:


	修改日志:20110301 高旭麟创建
*****************************************/


td_s32 tdmedia_SetBitrate(td_s32 _s32Chn,td_s32 _s32IsMainChn, td_s32 _s32Bitrate); 
/***********************************************
	功能:	设置编码码率
	参数:	_s32Chn 通道号
			_s32Bitrate 码率
	输出:
	
	修改日志:20110301 高旭麟创建
************************************************/

td_s32 tdmedia_GetFramerate( td_s32 _s32Chn,td_s32* _s32Framerate, td_s32 _s32IsMainChn);
/***********************************************
	功能:	获取用户设置的帧率值
	参数:	_s32Chn: 通道号
			_s32Framerate : 帧率
			_s32IsMainChn : 主(0)副(1)码流
	输出:
	
	修改日志:20120419 lxy create
************************************************/

td_s32 tdmedia_GetFramerateEx( td_s32 _s32Chn,td_s32* _s32Framerate, td_s32 _s32IsMainChn);
/***********************************************
	功能:	从底层获取实际在使用的帧率
	参数:	_s32Chn: 通道号
			_s32Framerate : 帧率
			_s32IsMainChn : 主(0)副(1)码流
	输出:
	
	修改日志:20120419 lxy create
************************************************/


td_s32 tdmedia_SetFramerateEx( td_s32 _s32Chn,td_s32 _s32Framerate, td_s32 _s32IsMainChn);
/***********************************************
	功能:	设置帧率,但不修改全局变量
	参数:	_s32Chn: 通道号
			_s32Framerate : 帧率
			_s32IsMainChn : 主(0)副(1)码流
	输出:
	
	修改日志:20120424 lxy create
************************************************/


td_u32 tdmedia_GetHDMaxFrmRate(td_s32 _s32Chn, td_s32 _s32IsMainChn);
/***********************************************
	功能:	在数字高清sdi设备时摄像机决定了通道的最大帧率，
			此函数可以获取当时支持的最大帧率
	参数:	_s32Chn: 通道号
	输出:
	
	修改日志:20130228 hq create
************************************************/


td_s32 tdmedia_SetFramerate( td_s32 _s32Chn, td_s32 _s32IsMainChn, td_s32 _s32Framerate); 
/***********************************************
	功能:	设置帧率，修改全局变量
	参数:	_s32Chn: 通道号
			_s32Framerate : 帧率
		
	输出:
	
	修改日志:20110301 高旭麟创建
************************************************/


td_s32 tdmedia_SetIframerate( td_s32 _s32Chn, td_s32 _s32IsMainChn, td_s32 _s32Iframerate);
/***********************************************
	功能:	设置I帧帧率
	参数:	_s32Chn: 通道号
			_s32Iframerate :I帧 帧率
		
	输出:
	
	修改日志:20110301 高旭麟创建
************************************************/


td_s32 tdmedia_ForceIFrame(td_s32 _s32Chn, td_s32 _s32IsMainChn);
/***********************************************
	功能:	强制I 帧
	参数:	_s32Chn 通道号
	输出:
	修改日志:20110301 高旭麟创建
************************************************/


td_s32 tdmedia_SetEncodetype(td_s32 _s32Chn, td_s32 _s32IsMainChn, td_s32 _s32Type);
/***********************************************
	功能:	设置编码类型
	参数:	_s32Chn 通道号
			_s32Type 编码类型
			0 定码率
			1 变码率
	输出:
	修改日志:20110301 高旭麟创建
************************************************/

td_s32 tdmedia_SetPciChnVencInfo(td_s32 _s32Chn, strVencParam* _pstVencParam);
/***********************************************
	功能:	保存从片编码通道信息
	参数:	_s32Chn 通道号
			_pstVencParam 编码通道参数
	输出:
	修改日志:20120321 hq创建,保存pci通道的编码大小等信息
************************************************/


td_s32 tdmedia_SetProFile(td_s32 _s32Chn,td_s32 _s32Type, td_s32 _s32IsMainChn);
/***********************************************
	功能:	设置ProFile类型
	参数:	_s32Chn 通道号
			_s32Type  ProFile类型
			0 :BaseLine
			1:MainLine
			2:HighLine
			输出:
	修改日志:20120308 王少龙创建
************************************************/


td_s32 tdmedia_SetVideosize(td_s32 _s32Chn, td_s32 _s32IsMainChn, td_s32  _s32Videosize); 
/***********************************************
	功能:	设置视频大小
	参数:	_s32Chn 通道号
			_s32Videosize 视频大小

	输出:
	修改日志:20110301 高旭麟创建
************************************************/


td_s32 tdmedia_CapPicture(td_s32 _s32Chn, strPicParam* _pstSnapMsg);
/***********************************************
	功能:抓拍图片
	参数:	_s32Chn:通道号
			_pstSnapMsg 抓拍结构体
	输出:
	修改日志:20110301 高旭麟创建
************************************************/


td_s32 tdmedia_GetVoPicture(strPicParam* _pstSnapMsg);
/***********************************************
	功能:VO 抓拍
	参数:	_s32VoDev:VO设备号
			_s32VoChn: vo通道号
			_pstSnapMsg 抓拍结构体
	输出:
	修改日志:20110301 高旭麟创建
************************************************/


td_s32 tdmedia_SetVideoQuality(td_s32 _s32Chn, td_s32 _s32IsMainChn, td_s32  _s32Quality);
/***********************************************
	功能:设置视频质量
	参数:	_s32Chn:通道号
			_s32Quality 视频质量
			_s32IsMainChn 主付码流标志
	输出:
	修改日志:20110309 高旭麟创建
************************************************/

td_s32 tdmedia_GetFrameSize(td_s32 _sample,td_s32 _AencType);

#endif
#if 1
/************************OSD td_media_osd.c*******************************************/
td_s32 tdmdia_StartOsd(td_s32 _s32Chn);
/***********************************************
	功能:	启动字符叠加，在创建编码通道
	             后设置生效
	参数:	_s32Chn 通道号
			
	输出:
	修改日志:20110315高旭麟创建
				
************************************************/

td_s32 tdmedia_SetVideoLost(td_s32 _s32Chn, td_s32 _s32lost);
/***********************************************
	功能:	高清vi发生丢失报警时，提供标志位
	参数:	_s32Chn 通道号
			_s32lost 是否丢失
	输出:
	修改日志:shihc add 2012123
************************************************/

td_s32 tdmedia_SetText(td_s32 _s32Chn,td_u8* _pu8text);
/***********************************************
	功能:	设置字符叠加爱内容
	参数:	_s32Chn 通道号
			_pu8text 字符叠加内容
			
	输出:
	修改日志:20110301 高旭麟创建
************************************************/


td_s32  tdmedia_SetOverlay(td_s32 _s32Chn,td_s32 _s32X,td_s32 _s32Y,td_u8* _pu8Text);
/***********************************************
	功能:	设置字符叠加位置
	参数:	_s32Chn 通道号
			_s32X: X坐标 (当_s32Y为-1时为叠加位置)
			_s32Y: Y坐标 (按位置叠加时为-1)
			
	输出:
	修改日志:20110301 高旭麟创建
************************************************/

td_s32  tdmedia_SetMutiOverlay(td_s32 _s32Chn, td_s32 _s32LayNo, td_s32 _s32X, td_s32 _s32Y, td_u8* _pu8Text);
/***********************************************
	功能:	设置字符叠加位置
	参数:	_s32Chn 通道号
			_s32LayNo	区域编号
			_s32X: X坐标 (当_s32Y为-1时为叠加位置)
			_s32Y: Y坐标 (按位置叠加时为-1)
			
	输出:
	修改日志:2015-8-7 刘琦创建
************************************************/

td_s32  tdmedia_SetLogo(td_s32 _s32Chn,td_s32 _s32Width,td_s32 _s32Height,td_u8* _pu8Buf);
/***********************************************
	功能:	设置logo叠加
	参数: _s32Chn 通道号
			_s32Width : logo宽
			_s32Height: logo高
			_pu8Buf: logo buffer
	输出:
	修改日志:20110301 高旭麟创建
************************************************/


void tdmedia_SetVICoverPos(td_s32 _s32Chn, TRect* _pstRegion, td_s32 _s32Num, td_s32 _s32VencSize);
/***********************************************
	功能:	
	参数: _s32Chn 通道号
			_pstRegion : 遮挡区域位置信息
			_s32Num: : 遮挡区域编号
			_s32VencSize :_pstRegion此时对应的venc的大小		
	输出:
	修改日志:20121008 hq create
************************************************/

void *tdmedia_GetVINamePos(td_s32 _s32Chn);
/***********************************************
	功能:	
	参数: _s32Chn 通道号
	输出:
	修改日志:20121008 hq create
************************************************/

td_s32 tdmedia_SetTextLocalVencChn(td_s32 _s32Chn, td_u8* _pu8text);
/***********************************************
	功能:	设定叠加字符
	参数:	_s32Chn 通道号
			_pu8text: 叠加内容指针
			
	输出:
	修改日志:201302 create
************************************************/

td_s32 tdmedia_SetTextPos(td_s32 _s32Chn,td_s32 _s32X, td_s32 _s32Y);
td_s32  tdmedia_SetTextPosLocalVencChn(td_s32 _s32Chn, td_s32 _s32X, td_s32 _s32Y);
/***********************************************
	功能:	设置字符叠加位置
	参数:	_s32Chn 通道号
			_s32X: X坐标 (当_s32Y为-1时为叠加位置)
			_s32Y: Y坐标 (按位置叠加时为-1)
			
	输出:
	修改日志:20110301 高旭麟创建
************************************************/


td_s32 tdmedia_SetTimePos(td_s32 _s32Chn,td_s32 _s32X, td_s32 _s32Y);
/***********************************************
	功能:	设置时间叠加位置
	参数:	_s32Chn 通道号
			_s32X: X坐标 (当_s32Y为-1时为叠加位置)
			_s32Y: Y坐标 (按位置叠加时为-1)
			
	输出:
	修改日志:20110301 高旭麟创建
************************************************/


td_s32  tdmedia_ShowLogo(td_s32 _s32Chn,td_s32 _s32X,td_s32 _s32Y,td_s32 _s32Show);
/***********************************************
	功能:	显示/隐藏logo
	参数: _s32Chn 通道号
			_s32X : X坐标
			_s32Y: Y坐标
			_s32Show: 1 显示logo
					   0 隐藏logo
	输出:
	修改日志:20110301 高旭麟创建
************************************************/


td_s32  tdmedia_ShowText(td_s32 _s32Chn,td_s32 _s32Show);
td_s32  tdmedia_ShowTextLocalVencChn(td_s32 _s32Chn,td_s32 _s32Show);
/***********************************************
	功能:	显示/隐藏字符叠加
	参数: _s32Chn 通道号
			_s32Show 1  显示字符
			               0 隐藏字符
	输出:
	修改日志:20110301 高旭麟创建
************************************************/


td_s32  tdmedia_ShowTime(td_s32 _s32Chn,td_s32 _s32Show);
/***********************************************
	功能:	显示/隐藏时间叠加
	参数: _s32Chn 通道号
			_s32Show 1  显示字符
			               0 隐藏字符
	输出:
	修改日志:20110301 高旭麟创建
************************************************/


td_s32 tdmedia_SetColor(td_s32 _s32Chn, td_s32 _s32Color, td_s32 _s32Type);
/***********************************************
	功能:	设置字符叠加颜色
	参数: _s32Chn 通道号
		  _s32Color 字符颜色
		  _s32Type 设置类型:
					0 字符叠加
					1 时间叠加
					2 附加字符叠加
	输出:
	修改日志:20110301 高旭麟创建
************************************************/

td_s32 tdmedia_SetFontSize(td_s32 _s32Chn, td_s32 _s32FontSize, td_s32 _s32Type);
/***********************************************
	功能:	设置字符叠加颜色
	参数: _s32Chn 通道号
		  _s32FontSize 字符字号
		  _s32Type 设置类型:
					0 字符叠加
					1 时间叠加
					2 附加字符叠加
	输出:
	修改日志:2015-08-19 zhangchao
************************************************/


td_s32 tdmedia_SetOverLayColor(td_s32 _s32Chn, td_s32 _s32LayNo, td_s32 _s32Color);
/***********************************************
	功能:	设置字符叠加颜色
	参数: _s32Chn 通道号
		  _s32LayNo	区域编号
		  _s32Color 字符颜色
	输出:
	修改日志:2015-8-7 刘琦创建
************************************************/
td_s32 tdmedia_SetOverLayFontSize(td_s32 _s32Chn, td_s32 _s32LayNo, td_s32 _s32FontSize);
/***********************************************
	功能:	设置字符叠加字号
	参数: _s32Chn 通道号
		  _s32LayNo	区域编号
		  _s32FontSize 字符字号
	输出:
	修改日志:2015-08-19 zhangchao
************************************************/

td_s32	tdmedia_SetTimeMode(td_s32 _s32Mode,td_u8* _pu8SplitChar);
/***********************************************
	功能:	设置时间叠加格式
	参数:	_s32Mode 时间模式
	                           0 : yyyy/mm/dd
	                           1 : mm/dd/yyyy
	                           2 : dd/mm/yyyy
	                           3 : M day yyyy
	                           4 : day M yyyy
			_pu8SplitChar: 分隔符
			
	输出:
	修改日志:20110301 高旭麟创建
****************************************************/

td_s32 tdmdia_DestoryTimeOsd(td_s32 _s32Chn);
/***********************************************
	功能:	OSD时间复位
	参数: _s32Chn 通道号
	输出:
	修改日志:shihc mod 20111227
			liuqi 2014-11-14 修改函数名,tdmdia_osd_DestoryAllRegion-->tdmdia_DestoryTimeOsd
************************************************/

td_s32 tdmedia_ResetTimeOsd(td_s32 _s32Chn);
/***********************************************
	功能:	OSD时间复位
	参数: _s32Chn 通道号
	输出:
	修改日志:20110301 高旭麟创建
************************************************/

td_s32	tdmedia_SetVideoCover(td_s32 _s32Chn,TRect* _pstRegion,td_s32 _s32Num);
/***********************************************
	功能:	显示/隐藏时间叠加
	参数: _s32Chn 通道号
			_pstRegion : 遮挡区域,为NULL时为取消视频遮挡
			_s32Num: : 遮挡区域编号
	输出:
	修改日志:20110301 高旭麟创建
************************************************/


td_s32 tdmedia_SetVideoCoverEx(td_s32 _s32Chn,TRect* _pstRegion,td_s32 _s32Num, td_s32 _s32VideoSize);
/***********************************************
	功能:	显示/隐藏时间叠加
	参数: _s32Chn 通道号
		  _pstRegion : 遮挡区域,为NULL时为取消视频遮挡
		  _s32Num: : 遮挡区域编号
	输出:
	修改日志:20110301 高旭麟创建
************************************************/

td_s32 tdmedia_SetVideoCoverPos(td_s32 _s32Chn, td_s32 _s32IsMainChn, td_s32 _s32NewVencSize);
/***********************************************
	功能:	切换编码分辨率时更新视频遮挡的位置和大小信息
	参数: _s32Chn 通道号
		  _s32IsMainChn : 主副码流
		  _s32NewVencSize: 新的编码大小
	输出:
	修改日志:20120908 hq create
************************************************/
#endif
#if 1
/************************移动侦测td_media_md.c****************************************/
td_s32 tdmedia_StartMD(td_s32 _s32Chn);
/***********************************************
	功能:	启动移动侦测，在创建编码通道
	             后设置生效
	参数:	_s32Chn 通道号
			
	输出:
	修改日志:20110315高旭麟创建
				
************************************************/


td_s32 tdmedia_SetMDArea(td_s32 _s32Chn,td_s32  _s32Area[]);
/***********************************************
	功能:	设置移动侦测报警区域
	参数: _s32Chn 通道号
			_s32Color 字符颜色
			_s32Area 移动侦测报警区域
	输出:
	修改日志:20110301 高旭麟创建
************************************************/


td_s32 tdmedia_SetMDSen(td_s32 _s32Chn,td_s32 _s32Sen);
/***********************************************
	功能:	设置移动侦测报警灵敏度
	参数: _s32Chn 通道号
			_s32Color 字符颜色
			_s32Area 移动侦测报警区域
	输出:
	修改日志:20110301 高旭麟创建
************************************************/


td_s32 tdmedia_GetMD(td_s32 _s32Chn);
/***********************************************
	功能:获取移动侦测报警状态
	参数:	_s32Chn:通道号
	返回值: 1: 有报警
			     0: 无报警
	输出:
	修改日志:20110301 高旭麟创建
************************************************/
#endif
#if 1
/************************视频遮挡td_media_od.c****************************************/
td_s32 tdmedia_GetOD(td_s32 _s32Chn);//获取某通道的移动报警状态

td_s32 tdmedia_SetOdEn(td_s32 _s32Chn, td_s32  _s32En);
/***********************************************
	功能:	使能视频遮挡报警
	参数: _s32Chn 通道号
			_s32En 使能
	输出:
	修改日志:20130313
************************************************/
#endif
#if 1
/****************************视频输入输出td_media_vio.c********************************/
td_s32 InitAD(td_s32 _s32ViNorm);
/***********************************************
	功能:	初始化AD芯片
	参数:	_s32ViNorm 视频制式

	输出:
	修改日志:20111221 wsl
************************************************/

td_s32 tdmedia_ChangeColortogray(td_s32 _s32Chn, td_s32 _s32ColorToGray);
/***********************************************
	功能:	彩转灰
	参数: 	_s32Chn 通道号
			_s32ColorToGray 0 取消彩转灰
						    1 设置彩转灰

	输出:
	修改日志:20110301 高旭麟创建
************************************************/


td_s32 tdmedia_ChangeVideopara(td_s32 _s32Chn, strVideoParam* _pstVideoParam);
/***********************************************
	功能:	设置视频参数
	参数: _s32Chn 通道号
			_pstVideoParam  视频参数结构体

	输出:
	修改日志:20110301 高旭麟创建
************************************************/


td_s32 tdmedia_EnableUserPic(td_s32 _s32Chn, td_s32 _s32Enable);
/***********************************************
	功能:	使能/取消novido 图片
	参数: _s32Chn 通道号
			_s32Enable  1 使能
					    0 取消

	输出:
	修改日志:20110301 高旭麟创建
************************************************/
  
td_s32 tdmedia_GetVirVODiff(void);
/***********************************************
  功能: 获取虚拟vo分辨率
  参数:
  输出:
  修改日志:created by hq(2175)
  ************************************************/

td_s32 tdmedia_GetSupportVencSize(td_s32 _s32Chn);
/***********************************************
  功能: 获取本地高清通道当前支持的最大编码大小
  参数:
  输出:
  修改日志:20120305 created by hq(2175)
  ************************************************/

td_s32 tdmedia_CheckHD_ViFormat(td_s32 _s32chn);
/***********************************************
  功能: 自动检测高清摄像机的vi的视频大小
  参数:
  输出:
  修改日志:created by shihc 20121223
  ************************************************/

td_s32 tdmedia_CopyVpssFrame(td_s32 _iFromDev, td_s32 _iFromChan, td_s32 _iToDev, td_s32 _iToChan);
/***********************************************
	功能:电子放大，解码通道对应的vpss发送图片到电子放大vpss
	参数:	_iFromDev: 拷贝图像显示所在的vo设备
			_iFromChan:  拷贝图像显示所在的vo通道
			_iToDev: 要拷贝到的vo显示设备
			_iToChan: 要拷贝到的vo显示通道
	修改日志:20130205 hq create
            20130301 hq modified
************************************************/


td_s32 tdmedia_SendVoUserPic(td_s32 _VoDev, td_s32 _VoChn);
/***********************************************
	功能:	向VO通道送novido 图片
	参数: _VoDev VO设备号
			_VoChn  VO通道号

	输出:
	修改日志:20110301 高旭麟创建
************************************************/


td_s32 tdmedia_Start_WBC(td_s32 _s32VoNorm, td_s32 _s32BindVoDev);
/*********************************
	功能:启动同源显示
	参数:_s32VoDev固定为1
*********************************/


td_s32 tdmedia_StartVideoPreview(td_s32 _s32Chn,  td_s32 _s32VoDev, td_s32 _s32VoChn);
/***********************************************
	功能: 开始视频预览
	参数:	_s32VoDev vo设备号
			_s32Chn vi通道号
			_s32VoChn  vo通道号
	输出:
	修改日志:20110301 高旭麟创建
************************************************/

td_s32 tdmedia_StartCascadePreview(td_s32 _s32VoDev, td_s32 _s32VoChn);
/***********************************************
	功能: sdi dvr开始视频预览,级联预览实现
	参数:	_s32VoDev vo设备号
			_s32VoChn  vo通道号
	输出:
	修改日志:20120601
************************************************/

td_s32 tdmedia_StopVideoPreview(td_s32 _s32Chn, td_s32 _s32VoDev, td_s32 _s32VoChn);
/***********************************************
	功能: 停止视频预览
	参数: _s32VoDev vo设备号
	      _s32Chn vi通道号
	      _s32VoChn  vo通道号
	输出:
	修改日志:20110301 高旭麟创建
************************************************/

td_s32 tdmedia_StopCascadePreview(td_s32 _s32VoDev, td_s32 _s32VoChn);
/***********************************************
	功能:sdi dvr 停止视频预览,级联预览
	参数: _s32VoDev vo设备号
	      _s32VoChn  vo通道号
	输出:
	修改日志:20120601
************************************************/

td_s32 tdmedia_StartAudioPreview(td_s32 _s32Chn,td_s32 _s32AoBindChn);
/***********************************************
	功能: 开始音频预览
	参数: _s32Chn 通道号
			_s32AoBindChn :绑定的AO通道号
	输出:
	修改日志:20110301 高旭麟创建
************************************************/


td_s32 tdmedia_StopAudioPreview(td_s32 _s32Chn,td_s32 _s32AoBindChn);

/***********************************************
	功能: 停止音频预览
	参数: _s32Chn 通道号
	输出:
	修改日志:20110301 高旭麟创建
************************************************/

td_s32 tdmedia_GetViNorm();
//shihc add 20121120 增加主片启动pciv通道，并且绑定
td_s32 tdmedia_BindPciv(td_s32 _s32LocPcivChn);
td_s32 tdmedia_UnbindPciv(td_s32 _s32PcivChn);
td_s32 tdmedia_BindPcivChn(td_s32 _s32VoDev, td_s32 _s32VoChn, td_s32 _s32PcivChn);
td_s32 tdmedia_UnbindPcivChn(td_s32 _s32VoDev, td_s32 _s32VoChn);

td_s32 tdmedia_GetVideoLost(td_s32 _s32Chn);
/***********************************************
	功能:获取视频丢失报警状态
	参数:	_s32Chn:通道号
	返回值: 1: 有报警
		0: 无报警
	输出:
	修改日志:20110301 高旭麟创建
************************************************/

td_s32 tdmedia_ReConfigVIChn(td_s32 _s32Chn, td_s32 _s32Size);
/***********************************************
	功能:D1&960H标清DVR统一后，需要根据编码分辨率更改vi的尺寸
	参数:	_s32Chn: vi 通道
		_s32Size:编码分辨率
	输出:
	修改日志:20120906 add
************************************************/

td_s32 GetViSize(td_s32 _s32Chn, td_s32* _Width,td_s32* _Height);

td_s32 tdmedia_ChangeVI(td_s32 _s32Chn, td_s32 _s32ViType);
/****************************************
	功能:	改变vi分辨率
	参数:	_s32Chn	vi 通道
			_s32ViType vi分辨率
	输出:
	修改日志:20110301 高旭麟创建
	         20111212 张仪保留接口，删除实现
*****************************************/

td_s32 ChangeHdmiDevice(td_s32 _s32DevType);
td_s32 GetHdmiCapability(td_s32 *_s32DevType);
void SetHdmiPixelRange(td_s32 _s32ChipModel);
td_s32 tdmedia_VoInit(td_s32 _s32VoDev, td_s32 _s32Diff, td_s32 _s32VoDevIntftype, td_s32 _s32WbcBindVoDev);
td_s32 tdmedia_HdmiInit(td_s32 _iHdmi0_Size);
td_s32 StopHdmiDevice();
td_s32 tdmedia_HdmiReInit(td_s32 _iHdmi0_Size);
void DetectHdmiDevice();

td_s32 tdmedia_VoDevSize(td_s32  _s32VoDev,  td_s32 _s32Diff, td_s32 _s32VoDevIntfType, td_s32 _s32WbcBindVoDev);
/****************************************
	功能:	设置视频输出分辨率
	参数:	_s32VoDev	vo设备号
			_s32Diff		vo分辨率
	输出:
	修改日志:20110301 高旭麟创建
			 20111212 张仪修改函数名
*****************************************/


//td_s32 tdmedia_SetVoCnt(td_s32  _s32VoDev,  td_s32 _s32Cnt);
/****************************************
	功能:	设置视频输出窗口数
	参数:	_s32VoDev	vo设备号
			_s32Diff		vo分辨率
	输出:
	修改日志:20110301 高旭麟创建
			 20111212 张仪删除
*****************************************/


td_s32 media_DisableVo( td_s32 _s32VoDev);
/************************************************************
	功能: 禁止所有vo通道、同步组、视频层。
	改变vo分辨率前调用
	参数:	
				_s32VoDev VO设备号
	输出:
	修改日志:20110301 高旭麟创建
*************************************************************/

td_s32 tdmedia_SetVoChnRect(td_s32 _s32VoDev, td_s32 _s32VoChn, TRect* _ptRect); 
/****************************************
	功能:	设置vo显示区域
	参数:	_s32VoDev	vo设备号
			_s32VoChn	vo通道号
			_ptRect		显示区域
	输出:
	修改日志:20110301 高旭麟创建
*****************************************/

td_s32 tdmedia_EnablePipLevel(td_s32 _s32VoDev, td_s32 _s32Diff);
/****************************************
	功能:	使能PIP层，包括PIP绑定VO设备
	参数:	_s32VoDev	vo设备号

	输出:
	修改日志:20120209 张仪创建
*****************************************/

td_s32 tdmedia_DisablePipLevel(td_s32 _s32VoDev);
/****************************************
	功能:	禁止PIP层，包括PIP解绑定VO设备
	参数:	_s32VoDev	vo设备号

	输出:
	修改日志:20120209 张仪创建
*****************************************/

td_s32 tdmedia_SetPipChnAttr(td_s32 _s32VoDev, td_s32 _s32VoChn, TRect* _stVO);
/******************************************
	功能:	设置PIP层通道属性

	修改日志:20130321 张仪创建
*******************************************/


td_s32 tdmedia_SetVoChnPri(td_s32 _s32VoDev, td_s32 _s32VoChn, td_s32  _s32Pri);
/****************************************
	功能:	设置vo显示优先级
	参数:	_s32VoDev	vo设备号
			_s32VoChn	vo通道号
			_s32Pri		优先级
	输出:
	修改日志:20110301 高旭麟创建
*****************************************/

td_s32 tdmedia_GetVoChnPri(td_s32 _s32VoDev, td_s32 _s32VoChn);
/****************************************
	功能:	获取vo显示优先级
	参数:	_s32VoDev	vo设备号
			_s32VoChn	vo通道号
	返回: 优先级
	修改日志:20120607 hq create
*****************************************/


td_s32 tdmedia_SetVoChnCtrl(td_s32  _s32VoDev, td_s32 _s32VoChn, VO_CTRL_E _eCtrl);
/****************************************
	功能:	vo通道显示控制
	参数:	_s32VoDev	vo设备号
			_s32VoChn	vo通道号
			_eCtrl		显示控制码
						0 使能
						1 禁止
						2 暂停
						3 恢复
						4 步进
						5 隐藏
						6 显示
						7 清空
	输出:
	修改日志:20110301 高旭麟创建
*****************************************/

//td_s32 media_ResumeVo(const td_s32 _s32VoDev, const td_s32 _s32VoChn, TRect* _ptRect);
td_s32 media_ViStartClipSide(const td_s32 _s32ViChan,TRect* _ptRect);
TRect *media_GetViEZoomClipSide(const td_s32 _s32ViChn, TRect* _ptRect);
TRect *media_SetViEZoomClipSide(const td_s32 _s32VoDev, const td_s32 _s32VoChn,TRect* _ptRect);
td_s32 media_ViStopClipSide(const td_s32 _s32ViChn);

td_s32 tdmedia_SetVoImgRect(td_s32 _s32VoDev, TRect* _pstRect);
/****************************************
	功能:设置image层大小
	参数:	_s32VoDev vo设备号
			pstRect: image层大小
	输出: 
	修改日志:20110311 高旭麟创建
*****************************************/

td_s32 tdmedia_SetVoFrameRate(td_s32  _s32VoDev, td_s32 _s32VoChn, td_s32 _s32FrameRate);
/****************************************
	功能:	设置vo显示帧率
	参数:	_s32VoDev	vo设备号
			_s32VoChn	  vo通道号
			_s32FrameRate	显示帧率
	输出:
	修改日志:20110301 高旭麟创建
*****************************************/

td_s32 tdmedia_SetVoChnField(td_s32 _s32VoDev, td_s32 _s32VoChn, td_s32 _s32Field);
td_s32 tdmeida_SetSampleRate(td_s32 _s32AFunc, td_s32 _s32ADev, td_s32 _samRate);
td_s32 media_StopAi(td_s32 _s32AiDevId );
td_s32 media_StopHDAi(void);



td_s32 media_GetEzoomChn(td_s32 _iType);
/********************************************************************
	功能:	获取电子放大通道号
	输入:	_iType 0:本地	_iType 1:数字
********************************************************************/

td_s32 media_EnterEzoom(td_s32 _s32VoDev, td_s32 _s32VoChan);
/********************************************************************
	功能:	开始电子放大功能
	修改日志:20120209 张仪创建
*********************************************************************/

td_s32 media_EnterCascadeEzoom(td_s32 _s32VoDev, td_s32 _s32VoChan);
/********************************************************************
	功能:	sdi dvr开始电子放大功能,级联电子放大
	修改日志:20120601
*********************************************************************/

td_s32 media_ExitEzoom(td_s32 _s32VoDev, td_s32 _s32VoChan);
/********************************************************************
	功能:	结束电子放大功能
	修改日志:20120209 张仪创建
*********************************************************************/

td_s32 media_ExitCascadeEzoom(td_s32 _s32VoDev, td_s32 _s32VoChan);
/********************************************************************
	功能:	sdi dvr结束电子放大功能,级联电子放大
	修改日志:20120601
*********************************************************************/

td_s32 media_SetEzoom(td_s32 _s32VoDev,  td_s32 _s32VoChan, TRect* _pstZoomAttr);
/********************************************************************
	功能:	调整电子放大区域
	参数:	
			_s32VoDev    		vo设备号							
			_pstZoomRect		电子放大区域
	输出:
	修改日志:20110301 高旭麟创建
*********************************************************************/

td_s32 media_SetVoClipArea(td_s32 _s32VoDev,  td_s32 _s32VoChan, TRect* _pstZoomAttr);
/**********************************************************************
	功能:	用于视频级联裁剪
	参数:	
		  _s32VoDev   	vo设备号
		  _s32Chn	通道号
				预览电子放大时为vi通道号
				回放电子放大时为vdec通道号
							
		  _pstZoomRect	电子放大区域
	修改日志:20111213 张仪创建
**********************************************************************/

td_s32 media_StartMicAi(td_s32 s32PtNumPerFrm);
/********************************************************************

*********************************************************************/

td_s32 media_StartNormAiChn(td_s32 _s32Chn,td_s32 _s32AudioSample, td_s32 _s32AencType);
/********************************************************************
功能:音频设备通道初始化
修改日志:ljj 整合标清和高清音频设备的初始化
*********************************************************************/
td_s32 media_GetNormAiInfo(td_s32 _s32Chn, td_s32 *_s32AiDev, td_s32 *_s32AiChn);
/********************************************************************
功能:获取音频设备的相关参数
修改日志:ljj 添加
*********************************************************************/

td_s32 media_StartAo(td_s32 _s32AoDev, td_s32 _s32AoCnt,td_s32 _sample);
/********************************************************************
	功能:	启动音频输出设备
	参数:	
			//_pstVoParam	VO设备参数
			_s32AoCnt : 音频采样间隔
	输出:
	修改日志:20110301 高旭麟创建
				   20110301 高旭麟修改参数
	
*********************************************************************/


td_s32 media_SetAoChnCnt(td_s32 _s32AoChnCnt);
/********************************************************************
	功能:设置ao通道个数
	参数:	
			_s32AoChnCnt : ao 通道个数
	输出:
	修改日志:
				   
	
*********************************************************************/



td_s32 media_StopAo(td_s32 _s32AoDev);
/********************************************************************
	功能:	停止音频输出设备
	参数:	
			//_pstVoParam VO设备参数
	输出:
	修改日志:20110301 高旭麟创建
				    20110301 高旭麟修改参数
*********************************************************************/

td_s32 media_StopAoChn(td_s32 _s32AoDev, td_s32 _s32AoChn);
/********************************************************************
	功能:	停止音频输出设备通道
	参数:	
	输出:
	修改日志:2015-1-10 刘琦创建
*********************************************************************/

td_s32 media_ClearAo(td_s32 _s32AoDev, td_s32 _s32AoBindChn);
/********************************************************************
	功能:	清空AO中缓存的数据
	参数:	
	输出:
	修改日志:20110301 高旭麟创建
*********************************************************************/


//Sherry add 20130111 获取2964的运行状态
td_s32 tdmedia_GetAddevState(void);
//更新Norm AI设备属性
td_s32 tdmedia_Change_NormAiDevAttr(td_s32 _iChn,strAencParam* _pstAencParam);
td_s32 tdmedia_Change_SVCAiDevAttr(td_s32 _iChn,strAencParam* _pstAencParam);

td_s32 tdmedia_SetRealBnc(td_s32 _s32RealBncNum);
td_s32 tdmedia_Change_AoDevAttr(td_s32 _s32AoDev,td_s32 _s32SampleRate,td_s32 _s32PtNumPerFrm);
td_s32 tdmedia_GetNormAiSampleRate();

#endif
#if 1
/******************音视频解码td_media_avdec.c************************************/
td_s32 media_CreateVdec(strVdecPara* _pstVdecParam, td_s32 _iType, td_s32 _s32LogChn);
/********************************************************************
	功能:	创建视频解码通道
	参数:	
			_pstVoParam 解码参数
			_iType  	0:common vdec; 1:vca vdec
	输出: 解码通道ID
	修改日志:20110301 高旭麟创建
*********************************************************************/


td_s32 media_DestoryVdec(td_s32 _s32ID, td_s32 _iType);
/********************************************************************
	功能:	销毁视频解码通道
	参数:	
			_s32ID 视频解码通道ID
			_iType		0:common vdec; 1:vca vdec
	输出: 
	修改日志:20110301 高旭麟创建
*********************************************************************/

td_s32 media_SetLogChnToVdecChn(td_s32 _s32LogChn, td_s32 _VdecChn);
td_s32 media_GetLogChnToVdecChn(td_s32 _s32LogChn);

td_s32 media_QueryVdec(td_s32 _s32ID);
/****************************************
	功能:	查询解码通道状态，阻塞函数，解码通道无数据时返回
	参数:	_s32ID	解码通道ID
	输出: 
	修改日志:20110301 高旭麟创建
*****************************************/


td_s32 media_VdecBindOutput(td_s32 _s32ID, td_s32 _s32VoDev, td_s32 _s32VoChn);
/****************************************
	功能:	绑定Vdec到Vo设备下某一通道
	参数:	_s32ID		解码通道ID(解码通道是动态创建的)
			_s32VoDev	绑定的Vo设备号
			_s32VoChn	绑定的Vo通道号
	输出: 
	修改日志:20110301 高旭麟创建
*****************************************/


td_s32 media_VdecUnBindOutput(td_s32 _s32ID, td_s32 _s32VoDev, td_s32 _s32VoChn);
/****************************************
	功能:	解绑定Vdec
	参数:	_s32ID		解码通道ID
			_s32VoDev	解绑定的Vo设备号
			_s32VoChn	解绑定的Vo通道号
	输出: 
	修改日志:20110301 高旭麟创建	
*****************************************/


td_s32 media_VdecSendStream(td_s32 _s32ID, td_u8* _pstStream, td_s32 _s32StreamLen, td_u32 _u32Pts, td_u32 _s32BlockFlag);
/****************************************
	功能:	向视频解码通道发送码流
	参数:	_s32ID		Vdec 通道号
			_pstStream	解码码流数据指针
			_s32StreamLen 长度
			_s32BlockFlag	阻塞/非阻塞标志
	输出: 
	修改日志:20110301 高旭麟创建
	 			   20110317增加长度变量
*****************************************/


td_s32 media_VdecStartRecvStream(td_s32 _s32ID);
/****************************************
	功能:解码通道开始接收码流
	参数:	_s32ID		Vdec 通道号

	输出: 
	修改日志:20110314 高旭麟创建	
*****************************************/


td_s32 media_VdecStopRecvStream(td_s32 _s32ID);
/****************************************
	功能:解码通道停止接收码流
	参数:	_s32ID		Vdec 通道号

	输出: 
	修改日志:20110314 高旭麟创建	
*****************************************/


td_s32 media_CreateAdec(td_s32 _s32Type);
/****************************************
	功能:	启动音频解码通道
	参数:	_pstAdecParam	音频编码参数
	输出: 音频解码通道ID
	修改日志:20110301 高旭麟创建	
	
*****************************************/


td_s32 media_DestoryAdec(td_s32 _s32ID);
/****************************************
	功能:	销毁音频解码通道
	参数:	_s32ID	解码通道ID
	输出:
	修改日志:20110301 高旭麟创建	
	
*****************************************/


td_s32 media_AdecSendStream(td_s32 _s32ID, td_u8* _pstStream, td_s32 _s32StreamLen, td_u32 _s32BlockFlag);
/****************************************
	功能:	向视频解码通道发送码流
	参数:	_s32ID		Adec 通道号
			_pstStream	解码码流数据指针
			_s32StreamLen 长度
			_s32BlockFlag	阻塞/非阻塞标志
	输出: 
	修改日志:20110301 高旭麟创建
				   20110317增加长度变量
*****************************************/


td_s32 media_AdecBindAo(td_s32 _s32AoDev, td_s32 _s32AdecID,td_s32 _s32AoBindChn);
/****************************************
	功能:	将音频解码绑定输出
	参数:	_s32AoDev		音频输出设备号
			_s32AoChn		音频输出通道号
			_s32AdecID		音频解码通道ID
	输出: 
	修改日志:20110301 高旭麟创建	
*****************************************/


td_s32 media_AdecUnBindAo(td_s32 _s32AoDev, td_s32 _s32AdChn,td_s32 _s32AoBindChn);
/****************************************
	功能:	将ADEC从AO解绑定
	参数:	_s32AoDev		音频输出设备号
			_s32AoChn		音频输出通道号
			_s32AdChn		音频解码通道号
	输出: 
	修改日志:20110301 高旭麟创建
*****************************************/


td_s32 media_AdecClear(td_s32 _s32AdecID);
/****************************************
	功能: 清空音频解码通道
	参数:	_s32AdecID		音频解码通道
	输出: 
	修改日志:20110311 高旭麟创建
*****************************************/

td_s32 media_VdecClearStreamData(td_s32 _s32ID);
/****************************************
        修改日志:20120131
*****************************************/

td_s32 media_VdecReset(td_s32 _s32ID);
td_s32 media_VdecStartClipSide(const td_s32  _s32VdecChn, TRect* _ptRect);
td_s32 media_VdecStopClipSide(const td_s32  _s32VdecChn);
TRect *media_GetVdecEZoomClipSide(const td_s32 _s32VoDev, const td_s32 _s32VoChn, const td_s32 _s32ViChn, TRect* _ptRect);
TRect *media_SetVdecEZoomClipSide(const td_s32 _s32VoDev, const td_s32 _s32VoChn,TRect* _ptRect);
td_s32 media_SetLogChnClipRect(td_s32 _s32LogChn, TRect *_ptRect);
TRect *media_GetLogChnClipRect(td_s32 _s32LogChn);
td_s32 media_SetLogChnClipStat(td_s32 _s32LogChn, td_s32 _s32tClipStat);
td_s32 media_GetLogChnClipStat(td_s32 _s32LogChn);
//暂时使用，改写后，下面连个函数应该删除
td_s32 tdmedia_InitMicDev();

// *************************VDEC&VPSS Bind/UnBind****************************
int tmcm_VdecBindVpss(int _s32DecChn);
int tmcm_VdecUnBindVpss(int _s32DecChn);

td_s32 media_TalkingAiAoStream(td_s32 _BindFlag);

//td_s32 media_getAoChnBuf(td_s32 _s32AoDev,td_s32 _s32AoChn);

td_s32 media_VdecBind(td_s32 _s32DecChn);
td_s32 media_VdecUnBind(td_s32 _s32DecChn);

//td_s32 tdmedia_VideoOpt(VIDEO_FRAME_INFO_S * _pstFrmSrcInfo, TSize * _pstSize, strVbCoach * _pstFrmDstInfo);//cwm_todo

td_s32 tdmedia_SetVoCSC(td_s32 _s32VoDev, TVoCSCMsg *_pVoOutCsc);

td_s32 media_getAoChnBufStat(td_s32 _s32AoDev, td_s32 _s32AoChn, td_s32 _s32ClearNum);

#endif

#endif

