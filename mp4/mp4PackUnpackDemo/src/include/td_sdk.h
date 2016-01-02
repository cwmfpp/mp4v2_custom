#ifndef _TD_SDK_H_
#define _TD_SDK_H_

enum TVVIDEOSIZE
{
	TVQCIF  =	0,
	TVHCIF	=	1,
	TVFCIF	=	2,
	TVHD1	=	3,
	TVFD1	=	4,
	TVMD1	=	5,
	TVQVGA	=	6,
	TVFVGA	=	7,
	TVHVGA	=	8,
	TVF720P	=	9,
	TVF960P	=	10,
	TVF1200	=	11,
	TVF1080P	=	12,
	TVF960H		=	13, 
	TVF300W		= 	14,
	TVF500W		= 	15,
	TVMAX		=	16
};

typedef struct
{
	int 		m_iType;							//设备厂家
	char		m_serName[40];						//服务器名称(暂时保留)
	char		m_serIP[40];						//服务器地址，可以是IP地址，也可以是域名；  m_strUrl
	char		m_serProxy[40];						//服务器上级代理m_strProxy
	int			m_serNetmode;						//传输协议0：TCP，1：UDP
	int			m_iCoonectMod;						//连接方式  0--IP   1----域名 2----主动模式
	int			m_iPort;							//服务器端口  
	int 		m_serChan;							//服务器通道m_iChn
	int			m_serStream;						//服务器码流类型m_iStremType  0主码流 1副码流	2音频流
	char		m_strUserName[40+1];				//用户名
	char		m_strUserPsw[40+1];					//密码
	char		m_strVdecPsw[40 + 1];				//视频解密密码
	char		m_strMac[40];
	char		m_strKey[8];
	int		m_iUseKey;// 0:第一次登录，1:采用密文登录，2采用明文登录
	char		m_strRtspUrl[256];   //用于扩展RTSP的地址，当RTSP 地址长度大于80字节时，使用本字段，否则使用原字段。
}TNVSITEM; 


typedef struct
{
    char  * m_cBuf;					
    int 	m_iLen;		
    int 	m_iCom;
}ComRcv;	


typedef struct
{
    int 	m_iWidth;				//视频宽度
    int 	m_iHeight;				//视频高度
    int 	m_iType;				//视频压缩格式
    int 	m_iFrame;				//视频帧率
}VideoDim;	//视频尺寸

typedef struct				//视频输入参数定义
{
	int m_iHue;				//色　调 0-255
	int m_iLum;				//亮　度 0-255
	int m_iContrast;			//对比度 0-255
	int m_iSaturation;		//饱和度 0-255
} VideoP;


typedef struct
{
	int 	m_iLeft;
	int 	m_iTop;
	int 	m_iWidth;
	int 	m_iHeight;
} RectArea;

#define CHN_MAX_VC_AREAS					4
typedef struct	// 视频遮挡区域设置
{
	int m_iAreaCnt;							//区域个数
	RectArea m_tAreas[CHN_MAX_VC_AREAS];	//区域(尺寸按实际像素)
} VideoCoverAreaCnf;

typedef struct				//视频输入参数定义
{
	char m_cAreaEnBmp[18][22];
} MdArea;

typedef struct
{
	int			m_iState;		// 报警状态
	unsigned int 	m_iTid;			// 目标ID
	int 			m_iEventType;	// 事件类型
	int			m_iRuleID;		// 规则ID
	int			m_iTType;		// 目标类型
	int			m_iTposition;		// 目标位置
	int			m_iTspeed;		// 目标速度
	int			m_iTdirection;	// 目标运动方向
}VCAAlarmParam, *pVCAAlarmParam;

typedef struct	//OSD叠加通用使能与位置参数
{
	int m_iEnable;	//使能
	int m_iLeft;		//x
	int m_iTop;		//y
	int m_iType;		//类型1--时间 2--通道名称  4---自定义字符
} OsdDspCnf;

typedef struct	//OSD叠加颜色
{
	int m_iType;		//类型1--时间 2--通道名称  4---自定义字符
	int m_iColor;		//颜色
} OsdColor;


typedef struct	//通道名称叠加信息
{
	int m_iLeft;								//x坐标
	int m_iTop;								//y坐标
	int m_iColor;								//颜色RGB
	char m_strText[256];						//通道名文本

	int m_iOldLeft;								//x坐标
	int m_iOldTop;								//y坐标
} NameOsdCnf;

typedef struct	//时间叠加信息
{
	int m_iLeft;								//x坐标
	int m_iTop;								//y坐标
	int m_iColor;								//颜色RGB
	int m_iFormate;							//时间格式
	int  m_iGit;

	int m_iOldLeft;								//x坐标
	int m_iOldTop;								//y坐标
} TimeOsdCnf;

typedef struct	//自定义文本叠加信息
{
	int m_iLeft;								//x坐标
	int m_iTop;								//y坐标
	int m_iColor;								//颜色RGB
	char m_strText[512];						//文本
	int m_iLayNo;
	int m_iFontSize;
	int m_iOldLeft;								//x坐标
	int m_iOldTop;								//y坐标
} TextOsdCnf;

typedef struct	// LOGO叠加信息
{
	int m_iDsp;	                            //shc add 增加这个表示位
	int m_iLeft;							//x坐标
	int m_iTop;							//y坐标
	int m_iBmpWidth;					//位图宽
	int m_iBmpHeight;					//位图高
	char *m_pcBmpData;					//位图数据(24色RGB)
	int m_iOldLeft;								//x坐标
	int m_iOldTop;								//y坐标
} LogOsdCnf;


typedef struct
{
	char m_cIp[32];
	int m_iPort;
	char m_cUserName[16];
	char m_cUserPass[16];
}DdnsInfor;


enum SETMSGTYPE
{
	MSG_DDNS = 0,
	MSG_MAINDDNS,
	MSG_SUBDDNS
};

enum STREAMTYPE
{
	VIDEO_STREAM = 1,
	AUDIEO_STREAM = 2,
	AV_STREAM
};


enum EVENTTYPE
{
	EVENT_CONNECT = 0,
	EVENT_CHANPARA,
	EVENT_ALARM,
	EVENT_TALK,
	EVENT_COMRCV,
	EVENT_UPGRATE
};


enum MSGTYPE
{
	/*视频参数相关 */
	MSG_VIDEOSIZE = 0x00000,			//视频尺寸
	MSG_VIDEOMODE,					//视频制式
	MSG_FRAMERATE,					//帧率
	MSG_IFRAMERATE,					//I帧间隔
	MSG_VIDEOQUALITY,					//视频质量
	MSG_VENCTYPE,						//视频编码格式
	MSG_ENCODETYPE,					//位率类型(变码率/定码率)
	MSG_BITRATE,						//码率kbps

	MSG_STREAMTYPE,					//复合码流类型
	MSG_AUDIOCODER,					//音频编码格式
	
	MSG_VIDEOPIXEL,
	MSG_MAXVIDEOSIZE,					//视频最大的编码尺寸
	MSG_ATUOCHECK,
	MSG_VIDEOPROFILE,				//视频profile
	MSG_AUDIOSAMPLE,					//音频采样率
	MSG_AUDIOSAMPLELIST,
	MSG_VIDEOLIST,						// 视频分辨率列表
	MSG_AUDIOCODERLIST,				// 音频编码类型类表
	
	//视频输入参数
	MSG_VIDOEPARA = 0x00050,			//亮度色度饱和度对比度(传TChnViCnf)
	MSG_COLORPARA,					//彩转黑
	MSG_SENSORSPIN = 0x00053,					//视频翻转
	MSG_VIDEOENCRYPT,					// 视频加密

	//音频输入
	MSG_AIVOL = 0x00060,				//音频输入音量

	//OSD相关
	MSG_OSDTITLE_ENABLE = 0x00100,	//通道名称叠加使能
	MSG_OSDTITLE,						//通道名称叠加文本位置颜色(传TChnNameOsdCnf)
	MSG_OSDTITLE_DSP,
	MSG_OSDTITLE_STR,
	MSG_OSDTITLE_COLOR,
	
	MSG_OSDTIME_ENABLE = 0x00110,	//时间叠加使能
	MSG_OSDTIME,						//时间叠加信息(传TChnTimeOsdCnf)

	MSG_OSDTEXT_ENABLE = 0x00120,		//自定义文本叠加使能
	MSG_OSDTEXT,						//自定义文本叠加信息(传TChnTextOsdCnf)

	MSG_OSDLOGO_ENABLE = 0x00130,	//LOGO叠加使能
	MSG_OSDLOGO,						//LOGO叠加信息(传TChnLogOsdCnf)

	MSG_VIDEOCOV_ENABL = 0x00140,	//视频遮挡使能
	MSG_VIDEOCOV,						//视频遮挡区域(传TChnVideoCoverAreaCnf)
	MSG_OSD_FONTSIZE,				// OSD 字号

	//报警相关
	MSG_MDALARM_ENABLE = 0x00200,	//移动侦测使能
	MSG_MDALARM_SEN,				//移动侦测灵敏度	
	MSG_MDALARM_AREAS,				//移动侦测区域(传TChnMdAreaCnf)

	MSG_VLALARM_ENABLE = 0x00210,	//视频丢失检测使能


	/*报警本地*/
	MSG_PORTALARM_ENABLE = 0x00220,		
	MSG_PORTALARM_TYPE,
	MSG_PORTALARM_STATE,
	MSG_MDALARM_STATE,					//移动侦测状态
	MSG_VLALARM_STATE,					//视频丢失报警状态

	/*  视频遮挡报警相关*/
	MSG_ODALARM_ENABLE = 0x00230, // 视频遮挡报警使能
	MSG_ODALARM_STATE,			 // 视频遮挡报警状态

	MSG_VCAALARM_STATE = 0x00240, // 智能分析报警状态

	/*连接相关*/
	MSG_CONNECT_SUCCESS =0x00300,	//连接成功
	MSG_CONNECT_ING,					//重连中
	MSG_CONNECT_ERROR,				//连接错误设备
	MSG_VIDEO_HEADER,
	MSG_VIDEO_BITRATE,
	MSG_CONNECT_MAC,

	MSG_COMDATA	=0x00500,

	/* 对讲相关*/
	MSG_TALK_OK = 0x00600,			//对讲成功
	MSG_TALK_ERROR, 				//对讲出错
	MSG_TALK_END, 					//对讲结束
	MSG_TALK_AENC_TYPE,

	
	//激励相关
	MSG_BESTIR = 0x00800, // 语音激励

	/*其他*/
	MSG_MASTER_VERSION	= 0x01006,
	MSG_UPGRATE_STA =0x01007, //远程升级IPC 状态

	//所有针对副码流的属性设置在主码流对应值上+ MSG_SUB_START
	MSG_SUB_START = 0x100000,	//副码流命令码开始
};

// 通道命令码编号定义
typedef enum
{
	// 编码相关
	CMD_START_ENCODE = 0x00001,	//开始编码
	CMD_STOP_ENCODE,				//停止编码
	CMD_FORCE_IFRAME,			//强制I帧
	CMD_SNAPSHOT,				//图片抓拍(传送参数为TChnSnapshotMsg)

	//预览相关
	CMD_BIND_VO 		= 0x00010,	//绑定预览画面(传送参数为TVoBindMsg)
	CMD_UNBIND_VO,				//解绑定预览画面(传送参数为TVO)
	CMD_BIND_AO,					//绑定音频预览
	CMD_UNBIND_AO,				//解绑定音频预览
	CMD_DRAW_PREVIEW_OSD,		//显示预览OSD字符(传送参数为TChnOsdCnf)
	CMD_CLEAR_PREVIEW_OSD,		//清除预览OSD字符显示
	CMD_CHG_DSP_AREA_ON_VO,	//设置预览VO显示区域(传TVoBindMsg)
	CMD_PRV_CHECK,				//预览状态检测(传送参数为TVoBindMsg)


	//码流相关
	CMD_START_AS_SEND	= 0x00020, //启动纯音频发送(传送参数TRingBuf)
	CMD_STOP_AS_SEND	= 0x00021, //停止纯音频发送

	// 云台控制相关
	CMD_PTZ_CTR 		= 0x00100,	//发送云镜控制码(传送TPtzCtrParam)

	//虚拟合成通道相关
	CMD_ATYPE_CHG		= 0x00200,	//通道音频类型变更(传通道号)
	CMD_ASAMPLERATE_CHG,                        //音频采样率变更 ljj add
	//智能分析相关 nsg add 2012007
	CMD_VCA_REBOOT		= 0x00300,	// 重启智能分析库
	CMD_VCA_ALARMSTATCLR,			// 智能分析报警计数清零
	//end
	CMD_VCA_RECREATE,				//智能分析重建

	// 切换预览码流
	CMD_CHANGE_PREVIEWSTREAM = 0x00400,

	// 特殊的命令(与具体通道无关，但依然以通道的形式传达)
	CMD_SPECIAL		= 0xF0000,
	CMD_IPC_SEARCH,				//数字通道搜索(传TIpcSearchParam)
	CMD_IPC_LIST,				//获取IPC 连接状态列表(传字符串指针，6K大小)
	CMD_IPC_SET_TIME,			//修改时间(无参数)
	CMD_IPC_SET_TIMEZONE,       //修改时区
	CMD_CLIP_SIDE,				//设置裁边
	CMD_IPC_CHANGE,
	CMD_IPC_TALK_START,		//与数字通道开始对讲(传送参数TIpTalkPara)
	CMD_IPC_TALK_STOP,		//与数字通道停止对讲
	CMD_IPC_SEND_PUSH_DATA,
	
	CMD_IPC_REBOOT,				//重启前端设备
	CMD_IPC_UPGRADE,			//升级bin或box文件

	CMD_CHN_REBOOT = 0xF0010,// 重启前端IPC

	CMD_SUB_START 	= 0x100000,	//副码流命令码开始
} CHN_CMD;


enum SERCHTYPE
{
	NET_TYPE = 0,
	DNS_TYPE  =  1,        
	ID_TYPE   = 2,
	ALL_TYPE = 0xFF
};

enum SENDVCPARAMSTAT
{
	PARAM_NOSEND = 0,
	PARAM_RCVHEAD  =  1,        
	PARAM_SENDED = 2,
	PARAM_LIMIT = 3
};

// zty 20120220
//enum
//{
//	GUI_DEV_TYPE_PRIVATE = 0, //设备类型为私有类型
//	GUI_DEV_TYPE_ONVIF,      //设备类型为onvif
//};
// zty add end
#define GUI_DEV_TYPE_PRIVATE 	0
#define GUI_DEV_TYPE_ONVIF		1
#define GUI_DEV_TYPE_PUSH		2		
#define GUI_DEV_TYPE_RTSP		3		

typedef struct
{
	char m_cMac[32];
	char m_cMask[40];			
	char m_cGateWay[40];		
	char m_cDNS[40];
	char m_cFactoryID[40];
	char m_cName[32];
	char m_cIp[40];
	char m_cProxyIp[40];
	int m_iPort;
	int m_iChanNum;
	char m_strServerType[32];//服务器类型
}NvsLook;







typedef int  (*EVENT_PROCESS)(void * _pIpChan[],int _iNum, int _EventID,int _MsgID, void *_pMsg);
typedef int  (*DATA_PROCESS)(void * _pIpChan[],int _iNum, char *_cBuf,int _iLen,int _iFrameType);
typedef int (*NVSSERCH_PROCESS)(void * _pModule,NvsLook * _pNvs);



typedef struct
{
	int		m_iType;																				//厂家类型
	char 	m_cTypeStrig[128];																		//厂家描述	
	
	int		m_iNum;																				//厂家类型
	
	int (*module_SetTime)(void *_pMChan,int _iYear,int _iMonth,int _iData,int _iHour,int _iMin,int _iSec);			//校时

	int (*module_SetTimeZone)(void *_pMChan, int _iTimeZone);

	int (*module_SetVideoPara)(void *_pMChan, VideoP * _pVideo);											//设置视频参数
	
	int (*module_SetFrameRate)(void *_pMChan, int _iFrame);												//设置帧率
	
	int (*module_SetIFrameRate)(void *_pMChan, int _iIFrame);											//设置I帧率
	
	int (*module_SetBitRate)(void *_pMChan, int _iBitRate);												//设置码率
	
	int (*module_SetVideoSize)(void *_pMChan, int _iVideoSize);											//设置视频大小

	int (*module_SetVideoProfile)(void *_pMChan, int _iVideoProfile);									//设置视频profile
	
	int (*module_SetAideoEnType)(void *_pMChan, int _iType);											//设置音频编码格式
	
	int (*module_SetStreamType)(void *_pMChan, int _iStreamType);										//设置码流类型 音频 视频 音视频
	
	int (*module_SetVideoQuality)(void *_pMChan,int _iVideoQuality);										//设置视频质量
	
	int	(*module_SetEncodeType)(void *_pMChan,int _iEncodeType);										//设置CBR或者VBR
	
	int	(*module_SetAudioSample)(void *_pMChan,int _iEncodeType);	

	int	(*module_SetVencType)(void *_pMChan,int _iVencType);											//设置编码格式0，H.264；1，M-JPEG；2，MPEG4
	
	int (*module_SetInputNorm)(void *_pMChan,int _iMod);												//设置制式
	
	int (*module_SetColorToGray)(void *_pMChan,int _iColorGray);											//设置彩转黑
	
	int (*module_SetVideoCovArea)(void *_pMChan,VideoCoverAreaCnf * _pCover);							//设置视频遮挡区域

	int (*module_SetOsdDisplay)(void *_pMChan, OsdDspCnf* _pOsdDspCnf);								//设置字符叠加的位置及使能
	
	int (*module_SetOsdColor)(void *_pMChan, OsdColor* _pOsdColor);										//设置字符叠加的颜色
	int (*module_SetOsdTitle)(void *_pMChan, char * _cTitle);												//设置字符叠加 通道名称
	
	int (*module_SetOsdTime)(void *_pMChan, int _iFormat);												//设置字符叠加 时间格式

	int (*module_SetOsdChannnelTitlePara)(void *_pMChan, NameOsdCnf* _pOsdChannelTitle);					//设置通道名称叠加 参数
	
	int (*module_SetOsdChannnelTitleEnable)(void *_pMChan, int  _iEnable);								//设置通道名称叠加 使能
	
	int (*module_SetOsdTimePara)(void *_pMChan, TimeOsdCnf* _pOsdTime);								//设置时间叠加 参数
	
	int (*module_SetOsdTimeEnable)(void *_pMChan, int  _iEnable);										//设置时间叠加 使能
	
	int (*module_SetOsdLogPara)(void *_pMChan, LogOsdCnf* _pOsdLog);									//设置LOG叠加 参数
	
	int (*module_SetOsdLogEnable)(void *_pMChan, int  _iEnable);											//设置log叠加 使能
	
	int (*module_SetOsdTextPara)(void *_pMChan, TextOsdCnf* _pOsdText);									//设置自定义字符L叠加 参数
	
	int (*module_SetOsdTextEnable)(void *_pMChan, int  _iEnable);											//设置自定义字符叠加 使能

	int (*module_SetInportEnable)(void *_pMChan,int _iEnable);											//设置端口报警使能
	
	int (*module_SetInputAlarmMode)(void *_pMChan, int _iLowOrHigh);									//设置端口报警触发方式
	
	int (*module_SetAlarmVideoLostEnanle)(void *_pMChan,int _iEnable);									//设置视频丢失报警使能

	int (*module_SetMotionAlarmEnable)(void *_pMChan,int _iEnable);										//设置视频移动报警使能
	
	int (*module_SetMotionAlarmThreshold)(void *_pMChan,int _iThreshold);								//设置视频移动报警检测灵敏度
	
	int (*module_SetMotionAlarmArea)(void *_pMChan,MdArea * _pMD);									//设置视频移动报警检测区域
	
	int (*module_SetAttribute)(void *_pMChan,int _iAid, const void *_pValue);									//用于命令扩展
	
	int (*module_ForceIFrame)(void *_pMChan);															//强制I帧
	
	//int (*module_PtzContorl)(void *_pMChan,void * _cbuf, int _iLen,void * _ComNum);							//PTZ控制
	// zty 20120220
	int (*module_PtzContorl)(void *_pMChan,void * _cbuf, int _iLen,void * _ComNum, void *_ptProtocolData);	//PTZ控制
	
	int (*module_Talk)(void *_pChan,void * _pIpChan,int _iFlag);											//对讲 请求及关闭
	
	int (*module_SendTalkData)(void *_pMChan,void * _cbuf, int _iLen);										//对讲数据
	
	void *  (*module_Connect)(TNVSITEM *_pIpChan,void * _pIpchan);										//建立通道 连接视频
	
	int (*module_DisConnect)(void *_pMChan,void * _pIpChan);											//销毁通道 断开连接
	
	int (*module_DoCmd)(void *_pMChan,int _iCid, void *_pArg);											// 命令扩展
	
	int (*module_GetAttribute)(void *_pMChan,int _iAid, void *_pValue);										//获取参数

	int (*module_SetMoudAttribute)(void *_pMod,int _iAid, void *_pValue);									// 设置模块参数

	int (*module_SerchNvs)(int _iType);	
	
	int (*module_StopSerchNvs)(int _iType);	

	int (*module_ChangeIp)(void *_tChgIp);
	
	void (*module_Uninit)(void);
	
	int (*module_GetDebug)(char * _cSting,int _iLen);
	
	int (*module_GetNetCost)(void);
	
	int (*module_SetEncrypt)(void *_pMChan,char *_pcPasswd);//add by gyn 20120918数字通道视频加密

	EVENT_PROCESS	MsgNotifyCallback;																// 消息处理回调	
	
	DATA_PROCESS	VidoeCallback;																	// 音视频数据回调  
	
	NVSSERCH_PROCESS	SerchResultCallback;															// 搜索结果回调
	
}TNVSMODULE;						//打开库的时候初始化  多少个厂家多少个实例


typedef struct 
{
	char m_strMac[40];
	char m_strIP[40];
	char m_strSubMask[40];
	char m_strGateWay[40];
	char m_strDNS[40];
}TChgIp;

#endif


