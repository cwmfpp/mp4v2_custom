/****************************************************************************
文件名:		guitransport.h
功	能:		图形界面与主控程序数据交换接口

修改日志:
修改人		修改日期		修改说明
==========  ==============  ===========================================
杜秀龙		2008-09-16		创建并实现基本功能
****************************************************************************/

#ifndef _GUI_TRANSPORT_H_
#define _GUI_TRANSPORT_H_

#define GUI_TX_NO_WAIT			0					//数据传输不等待
#define GUI_TX_WAIT_DONE		0x7FFFF000		//数据传输等待完成
#define GUI_TX_WAIT_DEF			10					//默认超时时间

#define GUI_TX_SERVER			0	//服务器类型码(用于主控程序)
#define GUI_TX_CLIENT			1	//客户端类型码(用于图形界面)

#define GUI_TX_LOCAL_IP			0	//本机IP
#define GUI_TX_MAIN_PORT		1	//主传输通道端口
#define GUI_TX_MSG_PORT			2	//消息传输通道端口
#define GUI_TX_DATA_PORT		3	//透明数据、串口数据传输通道

#define GUI_VIDEO_IN_CHN_CNT	32	//视频输入通道数

#define GUI_VIDEO_FREE			0	//通道未录像
#define GUI_VIDEO_HUM			1	//通道正在进行手动录像
#define GUI_VIDEO_TM			2	//通道正在进行定时录像
#define GUI_VIDEO_ALARM			3	//通道正在进行报警录像
#define GUI_VIDEO_DEF			4	//通道正在进行自定义录像
#define GUI_VIDEO_NONE_HUM		11	//通道正在进行手动录像(无视频)
#define GUI_VIDEO_NONE_TM		12	//通道正在进行定时录像(无视频)
#define GUI_VIDEO_NONE_ALARM	13	//通道正在进行报警录像(无视频)
#define GUI_VIDEO_NONE_DEF		14	//通道正在进行自定义录像(无视频)
#define GUI_VIDEO_NONE			-1	//通道无视频输入

typedef struct					//录像状态
{
	int m_iVideoSta[GUI_VIDEO_IN_CHN_CNT];	//各通道对应的录像状态
} TGuiVideoStatus;

extern TGuiVideoStatus *g_ptDvrVideoStaForGui; //录像状态保存区

#define GUI_ALARM_NONE			0	//无报警
#define GUI_ALARM_PORT			1	//端口报警
#define GUI_ALARM_MD			2	//移动侦测报警
#define GUI_ALARM_VLST			3	//视频丢失报警
#define GUI_ALARM_VCVR			4	//视频遮档报警
//add 
#define GUI_ALARM_TW            5   //绊线报警
#define GUI_ALARM_PM            6   //周界报警
#define GUI_ALARM_FACE          7   //人脸侦测 zhangweiwei add for VCA 0527
#define GUI_ALARM_DIAGNOSE      8   //视频诊断
#define	GUI_ALARM_OSCER			9	//物品看护add by nsg 20120801
#define GUI_ALARM_VCA           10   //智能分析 zhangweiwei 06/21/2011
#define GUI_ALARM_ALST			11	//音频丢失报警

typedef struct					//报警状态
{
	int m_iAlarmSta[GUI_VIDEO_IN_CHN_CNT];	//各通道对应的报警状态
	int m_iAlarmSound;			//声音报警提示(由主模块设置并清除)
	int m_iAlarmLed;			//前面板报警状态灯
} TGuiAlarmStatus;

extern TGuiAlarmStatus *g_ptDvrAlarmStaForGui; //报警状态保存区

#define GUI_PRIVATE_DATA_SIZE		512
extern void *g_pGuiPrivateData;				 //U私有数据

#define GUI_SYS_STA_STARTUP			0		//系统正在启动
#define GUI_SYS_STA_RUNNING			1		//系统正在运行
#define GUI_SYS_STA_REBOOT			2		//系统正在重启
#define GUI_SYS_STA_SHUTDOWN		3		//系统正在关机
#define GUI_SYS_STA_SATA_REBOOT_APP 4		//有硬盘重启外挂
#define GUI_SYS_STA_MEMSATA_REBOOT_APP 5	//直刻重启外挂

#define GUI_SYS_ERR_NONE			0		//无错误
#define GUI_SYS_ERR_DISK_FULL		1		//磁盘满
#define GUI_SYS_ERR_DISK_NULL		2		//无可用磁盘
#define GUI_SYS_ERR_DISK_RW			3		//读写磁盘失败
#define GUI_SYS_ERR_ACCESS			4		//非法访问
#define GUI_SYS_ERR_IP_CONF			5		//IP地址冲突
#define GUI_SYS_ERR_NET_INTE		6		//网络中断
#define GUI_SYS_ERR_VIDEOIN_MODE	7		//输入制式错误

#define GUI_SYS_STA_PCNT			4		//系统状态参数个数
#define GUI_SYS_ERR_PCNT			4		

#define GUI_SHR_MEM_EXT				1
//m_cShmRsv字段内存各功能起始地址
#define GUI_GPS_OFFSET            	0  //用于GPS显示的偏移GPS占用36字节
#define GUI_ALM_SPIC_OFFSET			48		//报警联动单画面(4 * 3字节)
#define GUI_CBL_SHM_START			128    //用与CBL的偏移字节 占用44字节
#define GUI_TEMP_SHM_START          256    //用于温湿度的偏移(32字节) 
#define GUI_PIC_STAT_START			288     //用于UI同步网络设置多画面数及红框的位置校正(预留6字节)
#define GUI_DEV_STAT_START			294		//用于车载wifi,GPS信号强度电源电压温度及卫星个数(预留12字节)
#define GUI_SHR_FANTEMP_STAT_STAET  306  //用于风扇控制温度 （预留1字节）
#define GUI_SHR_STAT_UPDATE_STAET   307  //用于通知外挂状态信息有更新 （预留4字节） 按位使用
#define GUI_ALAR_STA_START			512		//用于报警输入输出状态(预留128字节)
#define GUI_AUDIO_STA_START			640		//用于音频输入状态(预留128字节)
#define GUI_SHR_DECODER_START       770     //用于和解码器通讯(预留56字节)
#define GUI_VIDEO_HEADER_START		826		//用于片头是否叠加(预留2字节)
#define GUI_FIND_UPNP_DEV_OFFSET	828		// 用于即插即用获取有新设备标识 (预留2个字节)      
#define GUI_PREVIEW_WIN_MSG_START		830		//预览画面数，及当前窗口对应预览的通道号 (130  个字节)	
#define GUI_PREVIEW_SLAVER_WIN_MSG_START	960	 //辅屏预览画面状态信息(预留130个字节)
#define GUI_POE_MSG_START						1090   //每个POE通道信息(预留96个字节)
#define GUI_DIGITAL_CHN_START					1186	//每个通道是否连接过(预留8个字节)
#define GUI_PICSIZE_START						1194	//每个通道的分辨率(预留256个字节)
#define GUI_H323_STAT_START			1450 //h323网守状态和网络联网状态(预留1个字节)取值使用
#define GUI_TXSLEEP_START						1451      //recv和send是否sleep标志(预留4个字节)

#if 1
#define GUI_SHR_MEM_RSV				(4 * 1024)
#else
#define GUI_SHR_MEM_RSV				0
#endif

typedef struct						//GUI与系统状态
{
	int m_iGuiPid;					//UI进程号
	int m_iDvrPid;					//DVR主程序进程号
	int m_iGuiLastActiveTm;		//UI最后活跃时间，相当于UI心跳，由UI设置
	int m_iSysUpdateTm;			//主程序更新状态的时间，由主程序设置
	int m_iSysSta;					//系统状态，由主程序设置
	int m_iParams[GUI_SYS_STA_PCNT];	//状态辅助参数，依据m_iSysSta解释 
	//m_iParams[2] 值为0xFF-表示等待，0x1XX 表示有HDMI0
	int m_iSysErr;					//系统错误标识,由主程序维护
	int m_iSysErrTm;				//系统错误发生时间(time()值)，由主程序维护
	int m_iSysErrMsg[GUI_SYS_ERR_PCNT]; //系统错误拓展信息，暂时保留
									//2011-8-10 最后一个参数被定义为预览错误码
	int m_iDvrErrNo;				//DVR错误码
	int m_iGuideFlg;				//开机向导运行标识,由UI标识
	int m_iGuideEnFlg;				//开机向导可运行标识,由dvr标识
	char m_cShmRsv[GUI_SHR_MEM_RSV];	 //预留共享内存，留着它用
} TGuiSysStatus;

extern TGuiSysStatus *g_ptGuiSysStatus;		//UI心跳及系统状态

typedef struct
{
	int m_iEast; //经度X10000  (-180.0000~180.0000)
	int m_iNorth; //纬度X10000  (-180.0000~180.0000)
	int m_iSpeed; //速度X100(0.01km/h)
	int m_iHeight;//海拔X100 (0.01m)
	char m_cEnable;
	char m_cEastWest; //东西(E, W )
	char m_cSouthNorth; //南北(S,N)
}TGuiGpsShrMsg;

#define g_ptGuiGpsShrMsg ((TGuiGpsShrMsg *)(g_ptGuiSysStatus->m_cShmRsv + GUI_GPS_OFFSET))


typedef struct
{
	// 端口的真实状态
	int m_iAlarmOutValue;		//报警输出端口状态1-32
	int m_iAlarmOutValue1;		//预留
	int m_iAlarmInValue;			//报警输入端口状态1-32
	int m_iAlarmInValue2;		//预留

	// 端口是否正在报警
	int m_iAlarmOut;				//端口输出报警1-32
	int m_iAlarmOut1;
	int m_iAlarmIn;				//端口输入报警1-32
	int m_iAlarmIn1;

	// 视频丢失报警
	int m_iVideoLost;				//视频丢失报警1-32
	int m_iVideoLost1;			//预留

	// 视频移动报警
	int m_iVideoMd;				//移动侦测报警1-32
	int m_iVideoMd1;

	// 视频遮挡
	int m_iVideoCover;
	int m_iVideoCover1;

	//音频丢失报警
	int m_iAudioLost;			//音频丢失报警1-32
	int m_iAudioLost1;			
} TGuiAlarmInOutStatus;

#define g_ptGuiAlarmInOutStatus	((TGuiAlarmInOutStatus *)(g_ptGuiSysStatus->m_cShmRsv + GUI_ALAR_STA_START))

typedef struct
{
	int m_iInputBits;			//音频输入状态,bit0-bit32表式音频0-31通道,1表式有音频,0表式无音频
	int m_iInputBitsEx;			//将来扩展64路使用
	unsigned char m_ucVolValue[64];	//音量值(保留)
} TGuiAudioInputStatus;

#define g_ptGuiAudioInputStatus ((TGuiAudioInputStatus *)(g_ptGuiSysStatus->m_cShmRsv + GUI_AUDIO_STA_START))

enum
{
	GUI_NO_SHOW = 0,
	GUI_SHOW_TEMP_MSG,
	GUI_SHOW_HEARTRATE = 2,			//心率
	GUI_SHOW_BLOODOXYGEN,		//血氧
	GUI_SHOW_BLOODPRESSURE,		//血压
};//体征类型


typedef struct   //温湿度信息
{
	int m_iDspEnable;  //是否显示的标志0:隐藏1:显示温湿度2:显示心率见上方枚举
	float m_fTemp;       //温度，m_iDspEnable为2、3、4时为对应的值
	float m_fHumidity;  //湿度，m_iDspEnable为2、3、4时不使用
}TGuiTempMsg;
#define g_ptGuiTemperatureMsg ((TGuiTempMsg *)(g_ptGuiSysStatus->m_cShmRsv + GUI_TEMP_SHM_START))

typedef struct	//光盘信息
{
	int m_iSta;		//状态
	int m_iUsed;		// 占用标识(0表式未占用，正数表式光盘号，并在使用)
	int m_iUsedSize;	//已刻录大小(单位K)
	int m_iTotalSize;	//光盘总大小(单位K)
} TGuiCdSta;

#define GUI_CBL_MAX_CD		2
typedef struct	// 实时刻录状态信息
{
	int m_iStat;	// 刻录状态
	TGuiCdSta m_tSCdSta[GUI_CBL_MAX_CD];
} TGuiCblSta;
#define g_ptGuiCblStat ((TGuiCblSta *)(g_ptGuiSysStatus->m_cShmRsv + GUI_CBL_SHM_START))

typedef struct	// 实时刻录状态信息
{
	TGuiCblSta m_tGuiCblSta;//old
	int m_iCdLeftTime[GUI_CBL_MAX_CD]; //刻录状态下表示光盘剩余时间，
								   //在打包状态下表示光盘弹盘时间
} TGuiCblStaEx;

#define g_ptGuiCblStatEx ((TGuiCblStaEx *)(g_ptGuiSysStatus->m_cShmRsv + GUI_CBL_SHM_START))

typedef struct			//报警联动单画面消息体
{
	char m_iVoDev[4];	//VO设备号
	char m_iChn[4];		//通道号
	char m_iAlarm[4];	//报警类型
} TGuiAlarmLinkSinglePicMsg;
#define g_ptGuiAlarmLinkSinglePicMsg ((TGuiAlarmLinkSinglePicMsg *)(g_ptGuiSysStatus->m_cShmRsv + GUI_ALM_SPIC_OFFSET))


typedef struct
{
	char m_cDevNo;  //主屏显示设备号 
	char m_cCurWinId; //当前选中窗口，即红框所在主屏窗口 最大为31，(-1)-不显示红框
	char m_cPvMode[2]; //画面模式[0]-VGA  [1]-HDMI  m_cPvMode[0]-在政法中用于iE设置画面模式后置位，UI判断这个置位后发送获取画面模式协议。
}TGuiPicStatShrMsg;  //UI 同步客户端设置的多画面及当前画面红框位置
#define g_ptGuiPicStatShrMsg ((TGuiPicStatShrMsg *)(g_ptGuiSysStatus->m_cShmRsv + GUI_PIC_STAT_START))


//解码器
#if 1 //V3.0版本以前老版本使用
typedef struct
{
	char m_cDecChnCnt[2];  		//解码器画面数
	char m_cDecCurChn;    		//解码器当前画面(预留)
	char m_cDecAlarmer[16]; 		//解码器报警，按位表示 bit0 视频丢失 bit1端口 bit2视频移动 (高低位复用,低4位为输出设备1,高4位输出设备2)
	char m_cDecSwitchStat[16];  	//解码器切换状态(高低位复用,低4位为输出设备1,高4位输出设备2)
	char m_cDecCtrlStat[16];    	//解码器控制状态(高低位复用,低4位为输出设备1,高4位输出设备2)
	char m_cLogoDspStat;         //Logo显示 0-不叠加 1-叠加 2-更新
	char m_cDecAttr;            //按位bit0-串口属性改变
	unsigned char m_cOtherAttr[2];          //显示分辨率，值见guiparameter.h (0~160的离散值)
}TGuiDecShrMsg;
#define g_ptGuiDecoderShrMsg ((TGuiDecShrMsg *)(g_ptGuiSysStatus->m_cShmRsv + GUI_SHR_DECODER_START))
#endif

//行嵌用于是否进行片头叠加
typedef struct
{
	char m_cIsHeaderDsp;
}TGuiVideoHeaderShrMsg;
#define g_ptGuiVideoHeaderShrMsg ((TGuiVideoHeaderShrMsg *)(g_ptGuiSysStatus->m_cShmRsv + GUI_VIDEO_HEADER_START))

typedef struct
{
	char m_cUpnpNewDev; //1--有新设备 ,0 无新设备
	char m_cUpnpAddType; //0 --不启用即插即用，1--自动方式，2--提示方式
}TGuiFindUpnpNewDevShrMsg;
#define g_ptGuiFindUpnpNewDevShrMsg ((TGuiFindUpnpNewDevShrMsg *)(g_ptGuiSysStatus->m_cShmRsv + GUI_FIND_UPNP_DEV_OFFSET))

typedef struct
{
	char m_cWifiSignalState;//Wifi 信号强度 
	char m_cSatelliteCnt;   //卫星个数
	char m_cGpsSignalState; //GPS信号强度	
	char m_c3GSignalState;	// 3G信号强度,高4位代表在线状态，低四位代表信号强度
	short m_sOnDelayTm;	    //开机延时(Max:3600s) 禁用为0
	short m_sOffDelayTm;    //关机延时(Max:3600s) 禁用为0  [以上内核上发]
	short m_sPowerVoltage; 	//电源电压(乘10)  [ 以下UI下发]
	short m_sDevTemp;     	//设备温度值(乘10)
}TGuiDevStatShrMsg;
#define g_ptGuiDevStatShrMsg ((TGuiDevStatShrMsg *)(g_ptGuiSysStatus->m_cShmRsv + GUI_DEV_STAT_START))

typedef struct
{
	unsigned int m_iStatUpdate; //按位0x0001 --RTMP信息有更新  0x0002--RTSP列表信息有更新,涉及到移位操作，定义成无符号型
}TGuiStatUpdateShrMsg; //状态更新通知App
#define g_ptGuiStatUpdateShrMsg ((TGuiStatUpdateShrMsg *)(g_ptGuiSysStatus->m_cShmRsv + GUI_SHR_STAT_UPDATE_STAET))

typedef struct
{
	char m_cTempValue;
}TGuiFanTempShrMsg; //风扇启动转动温度值(取整数可正负)
#define g_ptGuiFanTempShrMsg ((TGuiFanTempShrMsg *)(g_ptGuiSysStatus->m_cShmRsv + GUI_SHR_FANTEMP_STAT_STAET))

typedef struct
{
	unsigned char m_cReferCnt;             //引用计数，内核不断循环增加
	unsigned char m_cPreviewMode;            	//当前预览模式
	short m_sChn[64];			//每个窗口当前预览的通道号
}TGuiPreviewWinMsg;
#define g_ptGuiPreviewWinMsg ((TGuiPreviewWinMsg *)(g_ptGuiSysStatus->m_cShmRsv + GUI_PREVIEW_WIN_MSG_START))
#define g_ptGuiPreviewSlaverWinMsg ((TGuiPreviewWinMsg *)(g_ptGuiSysStatus->m_cShmRsv + GUI_PREVIEW_SLAVER_WIN_MSG_START))

#define GUI_POE_OFFLINE		1	//poe断开
#define GUI_POE_ONLINE		2	//POE连接成功
#define GUI_POE_OFFLINE_OUTOFPOWER	3	//POE超负荷断开

typedef struct
{
	unsigned short m_sPower[GUI_VIDEO_IN_CHN_CNT];    //每个poe通道功率，内核乘以100发过来，UI除以100后显示
	unsigned char m_cStatus[GUI_VIDEO_IN_CHN_CNT];	//每个POE通道的状态
}TGuiPoeChnMsg;		
#define g_ptGuiPoeMsg  ((TGuiPoeChnMsg *)(g_ptGuiSysStatus->m_cShmRsv + GUI_POE_MSG_START))

typedef struct
{
	unsigned int m_iChn;    //按位1表示连接过，0表示未连接过
	unsigned int m_iChnEx;  //后32画面
}TGuiDigitalChnMsg;
#define g_ptDigitalChnMsg ((TGuiDigitalChnMsg *)(g_ptGuiSysStatus->m_cShmRsv + GUI_DIGITAL_CHN_START))

typedef struct
{
	int m_iPicSize[GUI_VIDEO_IN_CHN_CNT];   //前32个画面所对应通道的分辨率
	int m_iPicSizeEx[GUI_VIDEO_IN_CHN_CNT];   //后32个画面所对应通道的分辨率
}TGuiChnPicSizeMsg;
#define g_ptPicSizeMsg  ((TGuiChnPicSizeMsg *)(g_ptGuiSysStatus->m_cShmRsv + GUI_PICSIZE_START))

//===============状态===========================
enum
{
	GUI_H323_STAT_FREE  = 0, //空闲									   //空闲
	GUI_H323_STAT_LINKING, //正在连接中						   //主叫连接中
	GUI_H323_STAT_LINK,  //已连接									    //已链接
	GUI_H323_STAT_REFUSE, //对方拒绝
	GUI_H323_STAT_CLOSE, //对方关闭									//被叫挂断中
	GUI_H323_STAT_CALLIN,	 //远程呼入								//被叫接听中
	GUI_H323_STAT_ERROR,	//呼叫失败，请重试
};

typedef struct
{
	unsigned char m_cH323Stat; //按位高四位-GK状态，低四位-联网状态
}TGuiH323StatMsg;
#define g_ptH323StatMsg ((TGuiH323StatMsg *)(g_ptGuiSysStatus->m_cShmRsv + GUI_H323_STAT_START))

#define TX_MAGIC_NO     20150422  //Send和Recv是否需要sleep标志
typedef struct
{
	int m_iSleepFlg;
}TGuiTxSleepMsg;

#define g_ptGuiTxSleepMsg ((TGuiTxSleepMsg *)(g_ptGuiSysStatus->m_cShmRsv + GUI_TXSLEEP_START))

static inline int GetDvrErrNo()			//获取DVR错误码，由UI调用
{
	int n = g_ptGuiSysStatus->m_iDvrErrNo;
	
	g_ptGuiSysStatus->m_iDvrErrNo = 0;
	return n;
}

static inline void SetDvrErrNo(int _iNo)			//设置DVR错误码由dvr调用
{
	g_ptGuiSysStatus->m_iDvrErrNo = _iNo;
}

#define GUI_VIDEO_KEY_FILE  "guibmp/main/systemSetting_1.bmp"
#define GUI_ALARM_KEY_FILE	"guibmp/main/systemSetting_2.bmp"
#define GUI_ALARM_ALLMSG_KEY_FILE "guibmp/main/VideoPlayback_3.bmp"
#define GUI_CHNSTA_KEY_FILE  "guibmp/main/videoSetting_1.bmp"

enum
{
	GUI_VIDEO_STA = 1, //录像状态
	GUI_ALARM_STA,      //报警状态
	GUI_ALARM_MSG,      
	GUI_CHN_STA,          //数字通道连接状态
};

extern void* g_pAlarmStatusMem;
extern void* g_pVideoStatusMem;
extern void *g_pAlarmStatusForPlat;
extern void *g_ptChnStatusMem;

/**********************************************************************
函数名:		GuiTxInit()
功	能:		数据传输器初始化
参　数:		无
返回值:		0:成功, -1失败
日	志:		2008-09-22，杜秀龙，创建并实现
**********************************************************************/
int GuiTxInit();

/**********************************************************************
函数名:		GuiTxUninit()
功	能:		数据传输器反初始化
参　数:		无
返回值:		无
日	志:		2008-09-22，杜秀龙，创建并实现
**********************************************************************/
void GuiTxUninit();

/**********************************************************************
函数名:		GuiTxOpen()
功	能:		打开一个数据传输通道
参　数:		_uiIp:		本机IP地址
			_uiPort:	本机端口号
返回值:		负数表失败，其它值为传输通道描述符
日	志:		2008-09-16，杜秀龙，创建并实现
			2008-92-22，杜秀龙，添加参数ip与port对
**********************************************************************/
int GuiTxOpen(unsigned int _uiIp, int _iPort);

/**********************************************************************
函数名:		GuiTxWaitConnector()
功	能:		等待已打开的传输通道被对方连入，建议调用GuiOpen后随即调用
			该函数
参　数:		iTid:		传输通道编号
			iTimeOut:	超时时间，秒
返回值:		无
日	志:		2008-92-22，杜秀龙，添加参数ip与port对
***********************************************************************/
void GuiTxWaitConnector(int _iTid, int _iTimeOut);


/**********************************************************************
函数名:		GuiTxWaitSeverStart()
功	能:		等待对方在指定的IP与Port上创建传输通道，建议调用GuiTxConnect
			函数前调用此接口
参　数:		_iPort:		端口
			_uiIp:		ip地址
			_iTimeOut:	超时时间,秒
返回值:		无
日	志:		2008-92-22，杜秀龙，添加参数ip与port对
***********************************************************************/
void GuiTxWaitServerStart(unsigned int _uiIp, int _iPort, int _iTimeOut);

/**********************************************************************
函数名:		GuiTxConnect()
功	能:		打开一个数据传输通道
参　数:		_uiIp:		对方IP地址
			_uiPort:	对方端口号
返回值:		负数表失败，其它值为传输通道描述符
日	志:		2008-09-22，杜秀龙，创建并实现
**********************************************************************/
int GuiTxConnect(unsigned int _uiIp, int _iPort);

/**********************************************************************
函数名:		GuiTxReConnect()
功	能:		打开一个数据传输通道(可重连接)
参　数:		_uiIp:		对方IP地址
			_uiPort:	对方端口号
返回值:		负数表失败，其它值为传输通道描述符
日	志:		2008-09-22，杜秀龙，创建并实现
**********************************************************************/
int GuiTxReConnect(unsigned int _uiIp, int _iPort);

/**********************************************************************
函数名:		GuiTxClose()
功	能:		关闭指定的已打开传输通道
参　数:		_iTid:		传输通道描述符
返回值:		无
日	志:		2008-09-16，杜秀龙，创建并实现
**********************************************************************/
void GuiTxClose(int _iTid);

/**********************************************************************
函数名:		GuiTxSend()
功	能:		通过传输器发送数据
参　数:		_iTid:		传输通道编号
			_pSndBuf:	发送数据首地址
			_iSize:		发送数据大小
			_iTimeOut:	超时设置，可为GUI_TX_NO_WAIT, GUI_TX_WAIT_DONE，
						正数表式对应的秒数后超时
返回值:		负数表失败，其它值表示实际传输的数据量
日	志:		2008-09-16，杜秀龙，创建并实现
**********************************************************************/
int GuiTxSend(int _iTid, const void *_pSndBuf, int _iSize, int _iTimeOut);

/**********************************************************************
函数名:		GuiTxRcv()
功	能:		从传输器获取数据
参　数:		_iTid:		传输通道编号
			_pRcvBuf:	接收缓存首地址
			_iSize:		接收数据大小
			_iTimeOut:	超时设置，可为GUI_TX_NO_WAIT, GUI_TX_WAIT_DONE，
						正数表式对应的秒数后超时
返回值:		负数表失败，其它值表示实际读取的数据量
日	志:		2008-09-16，杜秀龙，创建并实现
**********************************************************************/
int GuiTxRcv(int _iTid, void *_pRcvBuf, int _iSize, int _iTimeOut);

/**********************************************************************
函数名:		GuiTxReset(int _iTid)
功	能:		重设传输通道状态
参　数:		_iTid:		传输通道编号
返回值:		无
日	志:		2008-09-16，杜秀龙，创建并实现
**********************************************************************/
void GuiTxReset(int _iTid);

/*********************************************************************
函数名: GuiGetShmAddr
功能: 返回共享内存地址
参数: _iKey:共享内存键值,_iSize:共享内存大小
返回值: 共享内存地址
日志: 2014-03-17, wzy，创建并实现
**********************************************************************/
void *GuiGetShmAddr(int _iKey, int _iSize);

int GuiVideoAlarmStaShm(int _iType, int _iChnCnt);

#endif

