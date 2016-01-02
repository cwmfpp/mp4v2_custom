// 通用数据结构定义
// duxiulong 2011/03/01
// 注意: 此文件只定义通用的数据结构，不定义接口

#ifndef _TD_TYPES_H_
#define _TD_TYPES_H_

#include <time.h>

#include "public_def.h"

//#include "alarmer.h"

// =========== 串口相关定义==================================
//usb模拟串口为NULL 时所用逻辑号
#define NULL_USB_SERIAL_COMID 201104

typedef struct	//串口属性
{
	td_s32 m_iBitRate;		//波特率
	td_s32 m_iDataBits;		//数据位
	td_s32 m_iStopBits;		//停止位
	td_s32 m_iChecksum;	//奇偶校验
} TComAttribute;

typedef struct _TComInstance	//串口实例
{
	td_s32 m_iComId;	//串口逻辑号

	//串口销毁
	td_s32 (*Close)(struct _TComInstance *_ptThis); 
	//串口读数据(异步)
	td_s32 (*Read)(td_s32 _iSize, void *_pBuf, struct _TComInstance *_ptThis);
	//串口写数据
	td_s32 (*Write)(td_s32 _iSize, const void *_pBuf, struct _TComInstance *_ptThis);
	//串口设置属性
	td_s32 (*SetAtrribute)(TComAttribute *_ptAttr, struct _TComInstance *_ptThis);
} TComInstance;

// ============ 通道属性相关定义==============================

// !!!!!注意，本结构如需扩充内容，只允许在末尾追加，不允许中间插入 
typedef struct	//各通道模块初始化共用参数结构
{
	td_s32 m_iLenght;		//结构长度

	// 本地通道专用
	td_s32 m_iVideoNorm;	//视频输入制式

	// 合成通道专用
	td_s32 m_iVoDev;		//设备号,VO_DEV_VGA,VO_DEV_BNCx表屏幕录像,VO_DEV_VC表VO合成

	// 数字通道使用(注册中心)
	td_u8 m_strRcIp[2][32];			//注册中心IP地址(不支持域名)
	td_s32 m_iRcPort[2];			//注册中心端号
	char m_strRcUserName[2][16];	//注册中心用户名
	char m_strRcUserPass[2][16];	//注册中心密码
	td_s32 m_iMaxNetBitrate;			//数字通道总连接码流上限
	
} TChnModuleInitParam;

// !!!!!注意，本结构如需扩充内容，只允许在末尾追加，不允许中间插入 
typedef struct //通道初始化属性
{
	td_s32 m_iLength;				//本结构长度
	td_s32 m_iLogicNo;				//逻辑通道号(上层指定)
	td_s32 m_iType;					//通道类型
	
	// 模拟通道用
	td_s32 m_iChnNo;	 			//通道号(数字通道是为对方通道号)
	td_s32 m_iMaxVideoSize;			//最大视频尺寸
	td_s32 m_iVideoNorm;			// 视频制式
	td_s32 m_iVencType;				//视频编码格式
	td_s32 m_iAencType;				//音频编码模式
	td_s32 m_iAudioSample;			//音频采样率
	td_s32 m_iVideoSize[2];			//当前视频尺寸
	td_s32 m_iFrameRate[2];			//帧率
	td_s32 m_iIFrameRate[2];			//I帧间隔
	td_s32 m_iQuality[2];				//视频质量
	td_s32 m_iStreamType[2];			//码流类型
	td_s32 m_iBitsType[2];			//位率类型
	td_s32 m_iBitsRate[2];			//码率(上限)

	// 数字通道用
	td_s32 m_iIpcEnable;				//使能标识
	td_s32 m_iServerType;			//设备厂家
	td_u8 m_strServerName[40];		//服务器名称(暂时保留)
	td_u8 m_strServerIP[40];			//服务器地址，可以是IP地址，也可以是域名；  m_strUrl
	td_u8 m_strServerProxy[40];		//服务器上级代理m_strProxy
	td_s32 m_iChnType;				//服务器码流类型0主码流 1副码流
	td_s32 m_iNetMode;				//传输协议1：TCP，2：UDP，3：多播；m_iConntectType
	td_s32 m_iCoonectMode;			//连接方式  0--IP   1----域名 2----主动模式
	td_s32 m_iServerPort;			//服务器端口   
	td_u8 m_strUserName[20 + 1];		//用户名
	td_u8 m_strUserPsw[20 + 1];		//密码
	td_u8 m_strVdecPsw[20 + 1];		//视频解密密码

	// 画面合成通道
	td_s32 m_iVoDev;				//显示设备号	

	//hq:20121210 为适应16:9视频源在4:3分辨率下显示，0:平铺在显示区域，1:保持宽屏比例显示
	td_s32 m_iPreviewMode; //数字通道使用
	td_s32 m_iaudiochn;		//通道对应的音频通道
} TChnInitParam;

typedef struct	//视频输入参数定义
{
	td_s32 m_iHue;								//色　调 0-255
	td_s32 m_iLum;								//亮　度 0-255
	td_s32 m_iContrast;							//对比度 0-255
	td_s32 m_iSaturation;						//饱和度 0-255
} TChnViCnf;

typedef struct	//OSD叠加通用使能与位置参数
{
	td_s32 m_iEnable;	//使能
	td_s32 m_iLeft;		//x
	td_s32 m_iTop;		//y
} TOsdDspCnf;

typedef struct	//通道名称叠加信息
{
	td_s32 m_iLeft;								//x坐标
	td_s32 m_iTop;								//y坐标
	td_s32 m_iColor;								//颜色RGB
	td_u8 m_strText[256];						//通道名文本
} TChnNameOsdCnf;


typedef struct	//时间叠加信息
{
	td_s32 m_iLeft;								//x坐标
	td_s32 m_iTop;								//y坐标
	td_s32 m_iColor;							//颜色RGB
	td_s32 m_iFormate;							//时间格式
	td_s32 m_iSeparator;						//分隔符(当字符处理)
} TChnTimeOsdCnf;

typedef struct	//自定义文本叠加信息
{
	td_s32 m_iLeft;								//x坐标
	td_s32 m_iTop;								//y坐标
	td_s32 m_iColor;								//颜色RGB
	td_u8  m_strText[512];						//文本
	td_s32 m_iLayNo; //区域ID,支持5个	
	td_s32 m_iFontSize;
} TChnTextOsdCnf;

typedef struct	// LOGO叠加信息
{
	td_s32 m_iDsp;								//是否显示
	td_s32 m_iLeft;								//x坐标
	td_s32 m_iTop;								//y坐标
	td_s32 m_iBmpWidth;						//位图宽
	td_s32 m_iBmpHeight;						//位图高
	td_u8 *m_pcBmpData;						//位图数据(24色RGB)
} TChnLogOsdCnf;

typedef struct // 所有叠加信息
{
	void *m_ptVo;	//vo
	TChnNameOsdCnf *m_ptChnNameOsd; //通道信息
	TChnTimeOsdCnf *m_ptTimeOsd;
	TChnTextOsdCnf *m_ptTextOsd;
	TChnLogOsdCnf *m_ptLogOsd;
} TChnOsdCnf;


typedef struct
{
	td_s32 m_iLeft;
	td_s32 m_iTop;
	td_s32 m_iWidth;
	td_s32 m_iHeight;
} TChnArea;

#define CHN_MAX_VC_AREAS					4
typedef struct	// 视频遮挡区域设置
{
	td_s32 m_iAreaCnt;							//区域个数
	TChnArea m_tAreas[CHN_MAX_VC_AREAS];	//区域(尺寸按实际像素)
} TChnVideoCoverAreaCnf;

typedef struct	// 移动侦测区域
{
	td_u8 m_cAreaEnBmp[18][22];				//按18x22小格，1表示选中
} TChnMdAreaCnf;

typedef struct		//抓拍信息
{
	td_u8 m_strFileName[128];	//图片文件名
	td_u8 m_strFileNameRedund[128];	//冗余图片文件名
	td_s32 m_iBufSize;			//图片数据缓存内存大小
	td_s32 m_iPicSize;			//实际图片抓拍大小
	td_u64 m_ulTrigger;          //报警触发源，wangsong, atm, 20120920
	td_s32 m_iRecAttr;          //文件属性，wangsong, atm, 20120920
	td_u8 *m_pcPicBuf;			//图片数据
} TChnSnapshotMsg;

typedef struct		// 数字通道信息
{
	td_s32 m_iEnable;				//使能标识
	td_s32 m_iServerType;			//设备厂家
	td_u8 m_strServerName[40];		//服务器名称(暂时保留)
	td_u8 m_strServerIP[40];			//服务器地址，可以是IP地址，也可以是域名；  m_strUrl
	td_u8 m_strServerProxy[40];		//服务器上级代理m_strProxy
	td_s32 m_iChnType;				//服务器码流类型0主码流 1副码流 2音频流
	td_s32 m_iChnNo;				//对方通道号
	td_s32 m_iNetMode;				//传输协议1：TCP，2：UDP，3：多播；m_iConntectType
	td_s32 m_iCoonectMode;			//连接方式  0--IP   1----域名 2----主动模式
	td_s32 m_iServerPort;			//服务器端口   
	td_u8 m_strUserName[20 + 1];		//用户名
	td_u8 m_strUserPsw[20 + 1];		//密码
	td_u8 m_strVdecPsw[20 + 1];		//视频解密密码
	//hq:20121210 为适应16:9视频源在4:3分辨率下显示，0:平铺在显示区域，1:保持宽屏比例显示
	td_s32 m_iPreviewMode;
	td_u8	m_strRtspUrl[256];	// 用于扩展RTSP的地址，当RTSP 地址长度大于80字节时，使用本字段，否则使用原字段。
} TIpChannelMsg;

typedef struct
{
	td_s32 m_iTigerType;	//报警触发方式
	void *m_ptAlarmer;		//报警器实例指针
} TIpAlarmerMsg;	//数字通道报警触发方式

typedef struct		//注册中心信息
{
	td_u8 m_strIp[2][32];			//注册中心IP地址(不支持域名)
	td_s32 m_iPort[2];			//注册中心端号
	char m_strUserName[2][16];	//注册中心用户名
	char m_strUserPass[2][16];	//注册中心密码
} TRegCenterMsg;

typedef struct		// 合成通道信息		
{
	td_s32 m_iVoDev;				//显示设备号
} TVcChannelMsg;

typedef struct		//预览设备焦点通道
{
	td_s32 m_iVoDev;	//设备号
	td_s32 m_iChn;		//通道号
	td_s32 m_iChncnt;    //是否发送音频20120831 add by zhuangzhibing
} TVoDevFocusMsg;

typedef struct		//混音通道
{
	int m_iEnable;//add by nsg
	int m_iChns[3]; //20120203mode by nsg 2 to 3
} TVcMixAudioChnMsg;

#if 0
typedef struct // 云镜透明通道数据
{
	td_s32 m_iDataSize; 	//数据长度
	td_u8 m_uDataBuf[0]; //数据内容(变长)
} TPtzData;
#endif

// zty 20120220
typedef struct
{
	td_s32 m_iPtzCode;			//ptz控制码
	td_s32 m_iAddress;			//ptz地址号
	td_s32 m_iPreset;			//预置位号
	td_s32 m_iSpeedX;			//水平速度
	td_s32 m_iSpeedY;			//竖直速度
} TPtzCtrProtocolData;

typedef struct //云镜控制命令参数
{
	TComInstance *m_ptCom;		//串口实例
	TComAttribute *m_ptAttribute; //串口属性
	td_s32 m_iDataSize; 			//数据长度
	td_u8 *m_ucData;			//数据内容

	// dxl 20120131 协议模式
	TPtzCtrProtocolData *m_ptProtocolData;
	
} TPtzCtrParam;

enum
{
	CHN_ALARMER_VL = 0x000001,  //视频丢失
	CHN_ALARMER_AL,				// 音频丢失
	CHN_ALARMER_MD,				// 移动侦测
	CHN_ALARMER_VC,				// 视频遮挡
	CHN_ALARMER_OD,				// 遮挡报警		//zhangyi add 20130411
	CHN_ALARMER_VCA	= 0x100000,// 智能分析报警(0x10ttrr, tt表类型,rr表规则号)
} CHN_ALARMER_TYPE;

/*typedef struct
{
	int m_iType;	// 报警器类型(见CHN_ALARMER_TYPE)
	TAlarmer *m_ptAlarm;	//报警器实类
} TChnAlarmer;*/

//================= 其余通道数据结构==============

typedef struct	//区域定义
{
	td_s32 m_iLeft;
	td_s32 m_iTop;
	td_s32 m_iWidth;
	td_s32 m_iHeight;
} TRect;

typedef struct	//区域定义
{
	td_s32 m_iClipStat;
	td_s32 m_iTop;
	td_s32 m_iBottom;
	td_s32 m_iLeft;
	td_s32 m_iRight;
} TRitioRect;


typedef struct	//视频尺寸像素信息
{
	td_s32 m_iWidth;
	td_s32 m_iHeight;
} TVideoSize;

typedef struct	// vo绑定信息
{
	void *m_ptVo;		//vo对像
	TRect *m_ptRect;	//区域大小(以D1为标准)
} TVoBindMsg;

typedef struct //VO设备四度信息 
{ 
    td_s32 m_iLuma;                     /* 亮度:   0 ~ 100 */ 
    td_s32 m_iContrast;                /* 对比度 :  0 ~ 100 */ 
    td_s32 m_iHue;                      /*灰度      :  0 ~ 100 */ 
    td_s32 m_iSatuature;               /* 饱和度 :  0 ~ 100 */ 
	td_s32 m_iGain;                    /*  增益      :  0 ~64  */
} TVoCSCMsg;


typedef enum
{
	VO_DEV_VGA 		= 0x00,
	VO_DEV_BNC1 	= 0x01,
	VO_DEV_BNC2 	= 0x02,
	VO_DEV_HDMI1 	= 0x00,
	VO_DEV_HDMI2 	= 0x01,
	VO_DEV_WBC      = 0x02,
	VO_DEV_VC		= 0x10,		//合成通道
	VO_DEV_VIR_PIP	= 0x15,
} VO_DEV_ID;

typedef enum
{
	VO_SIZE_NTSC = 0,	// 720x480
	VO_SIZE_PAL,		// 720x576
	VO_SIZE_VGA,		// 800x600
	VO_SIZE_1024x768,
	VO_SIZE_1280x1024,
	VO_SIZE_1440x900,
	
	VO_SIZE_720P_50 = 0x100,
	VO_SIZE_720P_60,
	VO_SIZE_720P_25,
	VO_SIZE_720P_30,
	VO_SIZE_1080P_50 = 0x110,
	VO_SIZE_1080P_60,
	VO_SIZE_1080P_25,
	VO_SIZE_1080P_30,
	VO_SIZE_1080I_50 = 0x120,
	VO_SIZE_1080I_60,
	VO_SIZE_1080I_25,
	VO_SIZE_1080I_30,
	VO_SIZE_1280x800_60,	//add by zxs 2012.8.15
	VO_SIZE_1366x768_60,	//add by lxy 2012.11.23	
} VO_DEV_SIZE;

typedef enum
{
	SENSOR_FLIP = 1,
	SENSOR_MIRROR ,
	SENSOR_MAX
}TSensorSpin;

// ================== 原config.h里面的内容================
//Cheng 11/12
typedef struct
{
	char		m_PPPoeUserName[32];	//PPPoe Username
	char 		m_PPPoePassword[32];	//PPPoe password
	td_u8	m_u8AutoDial;			//Enable dial
	td_u8      	m_u8DialTimes;               //Dial times
}strPPPoePara;

//Sherry add for Domain Name Parse

//定制功能类型
typedef enum
{
	CSTM_RAILWAY = 0,				//铁路视频监控系统
}TCustomization;

typedef struct
{
	char    	m_cUserId  [16];		//屎	
	char    	m_cNvsName [32];		//	
	char    	m_cPassword[16];		//
	td_u32  	m_u32RegTime;			//频远注时
	char		m_cDomainName1[32];		//(目前IP牡址)
	int		m_iPort1;
	char		m_cDomainName2[32];		//(莶使)
	int		m_iPort2;
}strParsePara;

//Cheng 12/02 add 
typedef struct 
{
	char		m_NtpServerIp[32];	//ntp服务器IP, 有可能是域名。
	unsigned int		m_NtpServerPort;		//ntp服务器端口
	unsigned int		m_Interval;	//校验间隔
	struct timespec m_Ntptime;
}NtpServer;

//zy-add ddns-080708 
typedef struct
{
	char 	m_cUserName[32];
	char	m_cPassword[32];
	char	m_cNvsName[32];
	char	m_cDomainName[32];
	int	m_iPort;
	int 	m_iDdnsEnable;
	int 	m_iChanged; //Cheng 100907 add for change 
}strDdnsPara;

#if 0
//shihc add 20121027
//*******wifi*********************************/
typedef struct
{
	//int m_iNetlistNo;
	char m_cEssid[32];
	int m_iSetSafeEnable;
	int m_iSafeType;
	int m_iKeyFormat;
	int m_iKeyType;
	int m_iKeySelect;
	//Cheng 100905 move from nDev
	char		m_cWifiEncryption[16]; //replaced with m_iSafeType
	char		m_cWifiPwdType[16]; //replaced with m_iKeyType
	char		m_cWifiPwd[80]; //password
	td_u16	m_u16WifiKeyNum; //seems == m_iKeySelect
	
	//char m_cPassword[GUI_WIFI_PWD_STR_LEN + 1];
	unsigned int m_uiIpAddress;
	unsigned int m_uiSubMask;
	unsigned int m_uiGateWay;
	unsigned int m_uiDNS;
	unsigned int m_uiBackDNS;
    	td_u8   	m_u8Mac[6];

	int m_DHCPEnable;
	int m_DHCPTimes;	
	int m_iOnLine;
}strWifiCnf;
//add end
#endif

typedef struct 
{
	td_u8   	m_u8Adapter;        	//
	char 		m_cHostName[16];		//
	char		m_cDomainName[16];		//	
	
//第一块网卡参数
	td_u32  	m_u32IpAddress;     	//IP址
	td_u32  	m_u32SubMask;       	//
	td_u32  	m_u32GateWay;       	//氐址
	td_u32  	m_u32DNS;           	//DNS址
	td_u32       m_u32BackDNS;
	td_u16		m_u16DHCPenable;
								
//第二块网卡参数	
	td_u32  	m_SndIpAddress;     	
	td_u32  	m_SndSubMask;       	
	td_u32  	m_SndGateWay;       	
	td_u32  	m_SndDNS;           	
	td_u32      m_SndBackDNS;
	td_u16		m_SndDHCPenable;	
	
	td_u8   	m_u8Mac[6];         	//
	td_u8       m_u8SecondMac[6];       //第二块网卡mac
	td_u8		m_u8FactoryID[32];
	td_u8		m_cFactory[32];	
	td_u8		m_u8UPNPenable;
	td_u8		m_u8DHCPTimes;

	td_u32      m_u32bonding;
}NetDevice;

typedef struct 
{
	td_u8       m_product;				//product: 1 channel 4channel 8channel 16channel
	td_u8     	m_u8VideoInput;      	//video input num
	td_u8		m_u8AudioInput;			//audio input num
	td_u8     	m_u8VideoOutport;    	//video output num
	td_u32		m_u32VoDevIntfType;		//zhangyi add 20130104
	td_u8     	m_u8AudioOutport;    	//audio output num
	td_u8		m_InputNorm;			//video input mode
	td_u8		m_OutputNorm;			//video output mode		Add by Leeon Tonie for Tiandy-Dvr, 090106
	//enum SENSOR_TYPE	m_u8SensorType;	//sensor type
	td_s32		m_iProductModel;		//产品能力集,表征产品的基本特性
	td_s32		m_iCustomization;		//定制功能集
	td_u8		m_cAppName[32];			//定制功能主程序名
	td_u8		m_u8DHCPTimes;//DHCP搜索次数 11/15 from S2
	//td_u8		m_u8AlarmType;

	// dxl 20110309
	td_u8	m_u8BncInput;		//本地模拟通道数
	td_u8	m_u8IpcInput;		//数字通道数
	td_u8	m_u8VcInput;		//虚拟合成通道数

	td_s32 m_iVcDevSize;		//虚拟合成通道分辨率(非编码分辨率)

	//zhangweiwei 20120130
	td_s32 m_iNetWorkCardNum;   //网卡数目 
	//lbl 20121206
	td_s32 m_iPlaybackCost;
}MediaDevice;

//存储附加的字符叠加信息 陶永亮 091117 从cmd_execute.h移到此

#define MAX_WORDOVERLAY_LEN	256

typedef struct
{
	td_s16	m_u16x;			//Sherry modify 20120925 
	td_s16  m_u16y;			//Sherry modify 20120925 
	//td_u16	m_u16xApp;
	//td_u16  m_u16yApp;	
	td_u16	m_u16Size;
	td_u16	m_usEnable;		//Added by Leeon Tonie 090211, for 3511.
	td_u8	m_cWord[MAX_WORDOVERLAY_LEN];//xujiayu 081204
	//td_u8	m_u8Renew;//xujiayu 081226 表示叠加字符是否更新
	td_u32    m_osdColor;            //cheng 02/08 记录颜色	
	td_u32    m_iFontSize;            //cheng 02/08 记录颜色	
}STR_WordOverlay;

//TV调节结构体
typedef struct
{
	int m_iTop;			//上边距(范围:0~100)
	int m_iBottom;		//下边距(范围:0~100)
	int m_iLeft;		//左边距(范围:0~100)
	int m_iRight;		//右边距(范围:0~100)
}TDevTV;

#define MAX_MIX_AUDIO_CHN_CNT	2

//add by nsg 20120130
typedef struct
{
	td_s32 m_uiMixAudiocnt;    		//声音源的个数
	td_s32 m_uiMixAduiochn;  		//合成通道号
	td_s32 m_uiState;               //结构体是否被使用过1代表被使用过
	td_s32 m_uiAudiochnlist[MAX_MIX_AUDIO_CHN_CNT]; //声音源通道号
	td_s32 m_uiAudiochnState;  		//声音源状态,哪位为1代表哪个通道不是静音
    
} TMixAudio;
//add end

//add by nsg 120905

#define SNAPTYPE_SINGLE 2
#define SNAPTYPR_MULTI 3

typedef struct
{
	td_s32	m_iType;	//抓拍模式0：不使能 1：使能（定时） 2:（报警联动抓拍）3：报警联动多次抓拍注释：默认为2
	td_s32	m_iQvalue;  //抓拍图片质量
	td_s32	m_iTimeval;	//抓拍时间间隔
	td_s32	m_iMethod;	//联动方式，按位表示0x1:开启ftp上传、0x2:开启Email通知 0x3:同时开启 0x0 均不开启。//20130314 SUN Wei add for 7.6
	td_s32	m_iSize;	//抓拍图片大小，0x7fff：代表自动，其余对应分辨率大小
	td_s32  m_iRetryTimes; //失败重发次数 20130311 SUN Wei add for 7.6

}TSnapShot;			//图片抓拍参数设置
//end add

#define MAX_AUDIO_LIST 16
#define MAX_VIDEOLIST_CNT 32
#define MAX_AUDIOCODERLIST_CNT 16 // huaqiang add 20130218
typedef struct
{
	td_u8			m_u8MediaType;                     	//32,1 	 3--频 2--频  1--频
	td_u8			m_u8FrameRate;                     	//33,1   帧
	td_u8			m_u8VideoQuality;                  	//34,1	 频   
	OSD_CHANNAME_T 	stu_OSDChannel;
	OSD_CHANNAME_T 	stu_OSDTime;		
	OSD_CHANNAME_T	stu_OsdLogo;
	structVideoParam    m_VideoParam[MAX_TIME_SEGMENT];                       //video setting
	td_u32			m_MDArea[18];            	//motion detect area 22*18 blocks ,one block size is 16*16(D1)
	td_u8			m_u8Threshold;                     	//244,1	 频贫卸
	td_u8			m_u8ReduceNoiseEnable;			//1,?
	td_u8			m_u8IFrameRate;						//251,1	 I帧帧
	td_u32			m_u32EncodeType;						//251,0	时模式CBR(0)/VBR (1)选
	td_u32			m_u32PreferMode;						//251,0	?模式(0)/帧模式(1)
	td_u32			m_AudioType;
	Rect_t			m_VideoCov[MAX_VIDEO_COVER_NUM];							//253,8	 modified by gxl 20081022	,四个遮挡区域	        	
	td_u32			m_u8VideoSize;						//261,1    
	td_u32			m_u32BitRate;						//262,4
	td_u32			m_u32VideoWidth;
	td_u32			m_u32VideoHeight;	
	td_s32			m_s32AudioSample;		
	td_u8           		m_u8VideoMode;                     	//频式璞2,系统??远识
	structVideoParam	m_NowVideoParam;
	td_u8			m_blManualVideoParam;				//是否手动设置过视频参数
	td_u8			m_blVideoParamTimerEnable;			//视频参数定时检测使能
	td_s32 			m_MaxVencSize;				//最大编码分辨率
	td_s32 			m_MaxFrmRate;				//最大帧率
	td_u8			m_PreviewRec;						//是否为VO编码
	td_u8			m_VideoEncrypt[ENCRYPT_PWD_LENGTH+1];	//视频加密密码
	td_u8			m_u8InVolume;

	td_u8			m_u8Norm;		//制式
	td_s32			m_u32VideoModeCheck;//自动检测
	// dxl 20110308
	td_u32			m_u32ChnType;		//通道类型
	td_u32			m_u32OriginalChnType;	//记录原始的通道类型
	td_s32			m_s32BncNo;		//BNC号，本地模拟通道专用
	td_s32			m_s32BncNoAlias;	//BNC alias，可能用于ui上显示通道属性
	TVcChannelMsg	m_tVcMsg;			// 虚拟画面合成通道专用
	TIpChannelMsg	m_tIpMsg;			// 数字通道专用
	// dxl 20111013	混音
	td_s32			m_iMixAudioChnCnt;	//混音个数
	td_s32			m_iMixAudioChnList[MAX_MIX_AUDIO_CHN_CNT];	//混音通道列表
	//TMixAudio       m_tMixAudio;//add by gyn 20120130 混音功能
	
	TSnapShot m_stSnapShotPara;  //add by nsg 120905在此类中，加入一个图片抓拍参数属性
	//wsl 20120906 裁边
	TRitioRect		m_iRitioClipRect;
	td_s32			m_iProfile;

	int m_iChnAudioSizeListCnt;
	int m_iChnAudioSizeList[MAX_AUDIO_LIST];
	char m_strPNPMac[40];

	td_s32			m_iVideoListCnt;
	td_s32			m_iVideoList[MAX_VIDEOLIST_CNT];

	// huaqiang add 20130218
	td_s32			m_s32AudioCoderListCnt;
	td_s32			m_s32AudioCoderList[MAX_AUDIOCODERLIST_CNT];
	// add end
	td_s32			m_audiochn;	//通道对应的音频通道wsl add
	td_s32			m_iVideoModleEnable;
	td_s32			m_iVideoTurnEnable;//视频翻转0x0:不使能;0x1:上下翻转使能;0x2:左右使能;0x3:上下左右都使能
}strChannelParam;
typedef struct
{
	td_u8		m_u8IFrameRate;			//鸥通I帧帧
	td_u8   		m_u8FrameRate;          //矢通帧
	td_u8   		m_u8VideoQuality;       //矢通频
	td_u8		m_u8VideoSize;			//通频小
	td_u32		m_u32VideoWidth;
	td_u32		m_u32VideoHeight;	
	td_u8		m_u8StreamType;			//通停频?频
	td_u8   		m_u8OneStream;
	td_u32		m_u32BitRate;			//通
	td_u32		m_u32EncodeType;						//251,0	帽模式CBR(0)/VBR (1)选
	td_u32		m_u32PreferMode;						//251,0	?模式(0)/帧模式(1)
	td_u8		m_VideoEncrypt[ENCRYPT_PWD_LENGTH+1];	//视频加密密码    	
	td_s32		m_s32Profile;
	td_s32		m_iVideoListCnt;
	td_s32		m_iVideoList[MAX_VIDEOLIST_CNT];
}strAppChannel;

//dongchengen add
typedef struct
{
	td_u32			m_u32VideoSize;		//分辨率
	td_u8			m_u8FrameRate;		//帧率
	td_u8			m_u8IFrameRate;		//I帧帧率
	td_u8			m_u8VideoQuality;	//画面质量
	td_u32			m_u32EncodeType;	//位率类型
	td_u32			m_u32BitRate;		//码率
	td_u8			m_u8MediaType;		//码流类型
}TChannelParamEvent;
//add end 

//dlq add for smart template at 20130907
typedef struct
{
	td_u32			m_u32FrameRate;		//帧率
	td_u32			m_u32VideoQuality;	//画面质量
	td_u32			m_u32VideoSize;		//分辨率
	td_u32			m_u32BitRate;		//码率
}TChannelParamSmart;
//add end 

//铁路视频监控系统参数
typedef struct
{
	td_u8		m_cFtpAddress[64];						//ftp地址
	td_s32		m_iFtpPort;								//ftp端口号
	td_u8		m_cFtpUser[32];							//ftp用户名
	td_u8		m_cFtpPassword[32];						//ftp密码
	td_u8		m_cSoapUrl[64];							//soap入口
	td_s32		m_iSoapPort;							//soap端口
	td_s32		m_iDistance;							//摄像机与识别装置的距离
	td_u8		m_cSoapActionUrl[64];
	td_u8		m_cSoapRange[64];
}TRailWay;

typedef struct
{
	td_u32	m_u8Mode;//刻录方式
	td_u32	m_u8DevList;//刻录设备列表
	td_u32	m_u32ChannelList[MAX_RECORDER_NUM];//刻录通道
}TCdrwPolicy;
typedef enum
{
	DISK_AUTO = 0,
	STD_DVD_DISK,
	BLUE_DISK,
	DISK_MUTE
}tag_DiskType;//碟片类型
//add by gyn
typedef struct
{
	td_u32 m_uCloseDisk;//刻录封盘
	td_u32 m_uEjectDisk;//是否弹盘
	td_u32 m_uBurnHour;//刻录时长，0代表不设置
	td_u32 m_uBurnSpeed;//刻录速度
	td_u32 m_uDiskType;//光盘类型//add by gyn 20130802
}TCdrwAdvBurnPara;//刻录的高级参数
//add end

// dxl 20110307 从user中移到此处
#define LENGTH_USER_NAME		16
#define LENGTH_PASSWORD		16
#define MAX_CLIENT				32
#define MAX_USER_NUM			32//liyang modify
#define USER_RIGHT_MAX_NUM  32
#define USER_CHANNEL_MAX_NUM 32
#define USER_KEY_LEN	8
#define USER_ENCRYPT_LEN	128
#define PASSWORD_ENCRYPT_LEN	128
//每一台网络视频服务器最多可有16个用户
typedef struct 											
{
	td_u16 		m_u16UserNum;									//0,4		该网络视频服务器的用户总数
	td_u16		m_u16MaxUser;									//该网络视频服务器所允许的连接用户数
	char			m_cUserName[MAX_USER_NUM][LENGTH_USER_NAME + 1];	//4,128		用户名
	char        	m_cPassWord[MAX_USER_NUM][LENGTH_PASSWORD + 1];		//132,128	密码
	td_u8		m_u8Level[MAX_USER_NUM];						//260,8		权限
	td_u8		m_u8ConUserNum;									//用于记录已经登陆的用户数
	td_u32		m_ClientIP[MAX_CLIENT];							//记录已经登陆的客户端的IP地址
	void*		m_ClientPoint[MAX_CLIENT];							//记录已经登陆的客户端的IP地址
	td_u8		m_u8VerifyNum[MAX_CLIENT];						//记录每一客户端的校验次数,Sherry add 20050804
	td_u8		m_u8NetMode[MAX_CLIENT];						//记录每一客户端的网络模式
	char			m_cLogonUser[MAX_CLIENT][LENGTH_USER_NAME];	//record the logon user's name
	td_u32		m_u32Authority[MAX_USER_NUM][USER_RIGHT_MAX_NUM];//20120920 王磊add 用户权限

	td_u8		m_u8Key[MAX_CLIENT][USER_KEY_LEN];//登录后每个用户登录的密钥，每个客户端一个
} NetUser;
typedef int (*LogonDecodeCallBack)(const char* _cSrcStr1, int _iStrLen1, const char * _cSrcStr2, int _iStrLen2, OUT char *_cOutBuf, int _iOutLen);
// dxl 20110309 数字通道搜索设备条目
typedef struct
{
	td_s8 m_cMac[32];			
	td_s8 m_cMask[40];			
	td_s8 m_cGateWay[40];		
	td_s8 m_cDNS[40];
	
	td_s32 m_iServerType;		//厂商类型
	td_u8 m_strIp[40];			//设备IP
	td_s32 m_iPort;				//设备服务端口号
	td_s32 m_iChnNum;			//通道个数
	td_u8 m_strFactoryID[40];	//出厂ID
	td_u8 m_strName[32];		//设备名称
	td_u8 m_strProxyIp[40];		//代理服务器
	td_u8 m_strDevType[32];		//服务器类型(DVR/NVR/UNK...)
} TIpcDevItem;

#define MAN_PNP			0
#define AUTO_PNP		2
#define MAN_AUTO_PNP	1
typedef struct
{
	int m_iPlugStart;
}TPlugConf;

//#define MAX_WORDOVERLAY_NUM	1	//政法暂时只支持一块，modify by liuqi 2014-7-18
// for decoder
typedef struct
{
    td_s32		m_iEnable;           //是否叠加Logo
    td_s32		m_iPosX;            //叠加Logo横坐标
    td_s32		m_iPosY;            //叠加Logo纵坐标
    td_s32		m_iAlpha;           //透明度(0~100)，100为完全不透明
    td_s32		m_iTransparentColor;//背景色，RGB格式(0x00000000~0x00ffffff)
}TLogoParam;

//zhangyi add 20140314
#define MAX_RAIDARRAY_NUM	4

#if JMB39X == ON
#define MAX_LOCAL_DISK_NUM	5
#else
#define MAX_LOCAL_DISK_NUM	4
#endif

typedef struct
{
	td_s32	m_iNo;
	td_s32	m_iEnableRaidArray;
	td_s8 	m_cRaidName[64];
	td_s32 	m_iRaidType;
	td_s32	m_iDisklist[MAX_LOCAL_DISK_NUM];
	td_s32	m_iDiskTotal;
}TRaidArrayCfg;

typedef struct
{
	td_s32 m_iRaidId;
	td_s32 m_iRaidAllSize;
	td_s32 m_iRiadUseAbleSize;
	td_s32 m_iDiskList;
	td_s32 m_iDiskList1;
	td_s32 m_iDiskBackUpId;  	//0表示没有热备盘
	td_s32 m_iRaidStat;   		//阵列状态 TGuiRaidStatus
	td_s32 m_iRaidType;
	td_s32 m_iRaidTask;  		//阵列任务TGuiRaidTaskType
	td_s32 m_iRaidTaskPara[4]; 	//预留，百分比，剩余时间等
	td_s8 m_strRaidName[31 + 1];
}TRaidArrayStat;

typedef struct
{
	td_s32	m_iDiskId;
	td_s8 	m_cRaidName[64];
	td_s32	m_iWorkMode;
}TRaidHdCfg;

typedef struct
{
	td_s32 m_iDiskId;
	td_s32 m_iDiskType;
	td_s32 m_iDiskStat;
	td_s32 m_iDiskSize; 						//单位为M
	td_s8 m_strRaidName[31 + 1];
	td_s8 m_strDiskModel[31 + 1];
}TSataDiskCfg;


#define MAX_VIRDISK_NUM	8
typedef struct
{
	td_s8 	m_cVdName[64];
	td_s32	m_iVdSize;
	td_s32	m_iInitType;
	td_s8	m_cRaidName[64];
}TRaidVdCfg;
typedef struct
{
	td_s32 	m_iRaidVdSize;
	td_s32  m_iVdStat;       		//虚拟磁盘状态TGuiVdStatType
	td_s32 	m_iRaidtype;    		//虚拟磁盘所属的raid类型
	td_s32 	m_iVdTask;      		//虚拟磁盘任务类型TGuiVdTaskType
	td_s32 	m_iVdTaskPara[4];  		//任务预留参数，包括百分比，剩余时间等
	td_s8 	m_strVdName[31 + 1];
	td_s8 	m_strRaidName[31 + 1];
}TRaidVdStat;

// 解码器外挂
typedef struct		//平台对接接口IP及端口
{
	char m_cControlIp[40];	
	int m_iPort;	
	int m_iEnable;
}TPlatformCtrlInfo;

typedef struct		//平台对接接口IP及端口
{
	char m_cDevID[21];	
	char m_cDevName[33];		
}TPlatformDevInfo;

typedef struct
{
	char Pro_ID[4];
	char Data_ID[2];
	char Rsp_Ctl[2];
	char Cmd_Len[2];
	char FrameHead[2];
	char Chn;
	char PicNum;
	char LinkMode;
	char IpAddr[4];
	char  DomainName[32];
	char  ID[32];
	char ProxyIP[4];
	char DevChn[2];
	char Port[5];
	char  StreamType;
	char  NetType;
	char  PtzProtocol;
	char  PtzAddr[3];
	char   UserName[16];
	char   Password[16];
	char   VideoEncryptPass[16];
	char  CheckSum;
	char  UserID;
	char  FrameTail[2];
}stVirtualMatrixCtlMsg;

typedef struct
{
	int m_iVideoPwdEnable;
	char m_cVideoPwd[ENCRYPT_PWD_LENGTH + 1];
}TPoliticsVideoPwdMsg;

#define STREAM_AUDIO_START_CHN 7

typedef enum
{
	BESTIR_STAT_RESERVER = 0,
	BESTIR_STAT_VALID,
	BESTIR_STAT_CANCEL,
	BESTIR_STAT_LIMIT
}E_BESTIR_STAT;

#endif

