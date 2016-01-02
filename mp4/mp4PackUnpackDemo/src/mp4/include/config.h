/*********************************************************************************
[File information]
FileName = config.h
Description =  configuration head file
Corp.= Tianjin Tiandy Tech
Date= 2006-09-01
[History] 	
XH1=XM20060901,
XH2=XM20070701,
*********************************************************************************/
#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "td_types.h"
#include "wifi.h"
#include "guiparameter.h"

#define LENGTH_CHN_TITLE    	    			32
#define LENGTH_USERDATA				32						//用户数据长度
#define VIDEOSIZELIST  				64

#define _MIN(a, b)  (a<b ? a:b)
#define _MAX(a, b)  (a>b ? a:b)
#define _MIN_VIDEO_SIZE(a, b) (CmpVideoSize(a, b) <= 0 ? (a) : (b))
#define _MAX_VIDEO_SIZE(a, b) (CmpVideoSize(a, b) >= 0 ? (a) : (b))


//陶永亮 090514	总带宽限制
//#define MAIN_TOTAL_BANDWIDTH			65536					//主码流总带宽限制,40M->64M  20120214 wsl 修改
//#define SUB_TOTAL_BANDWIDTH				20480					//副码流总带宽限制,8M->20M

//陶永亮 090909 定义硬件型号
#define HARDWARE_3511_16			0x0100					//基于3511的16路CIF硬件
#define HARDWARE_3511_08			0x0110					//基于3511的8路CIF硬件
#define HARDWARE_3520_16			0X0120					//基于3520的16路D1硬件

typedef enum
{
	DZ_NULL = 0,
	DZ_SHANGHAI_TS = 1,
	DZ_MAX,
}TDZMODELID;

//陶永亮 090911 GUI主配置文件及其备份，主要用于恢复出厂设置
#define DISPLAY_CONFIG_FILE			"./gui_config/display.cnf"		//显示配置
#define DISPLAY_BACKUP_CONFIG		"./gui_config/display.cnf.back"	//备份显示配置
#define GUI_ATTRIBUTE_FILE			"./gui_config/gui_conf.xml"		//GUI基本属性文件
#define GUI_ATTRIBUTE_BACKUP		"./gui_config/gui_conf.xml.back"	//GUI基本属性文件备份
#define GUI_GUIDE_CONF				"./gui_config/guide.cnf"		//GUI开机向导配置文件
#define GUI_USER_CONF				"./gui_config/user.his"			//GUI用户名最后一次登录用户名
#define DEFAULT_CONFIG_SERVER		"./config_default.ini"		//陶永亮 100914 默认的配置文件
#define DEFAULT_NOT_FOUND			"QRS"					//陶永亮 100914 配置文件中不存在则使用该值,嵌入式
#define EXTEND_WORD_FILE            "extendword.txt"        //wangsong, 20121106, 附加字符叠加信息文件
#define EXTEND_WORD_DEFAULT         "extendword_default.txt"    //wangsong, 20121106, 默认附加字符叠加文件，用于恢复特定附加字符信息
#define CONFIG_LASTLINE		"[lastline]"					//配置文件的最后结束标志
#define CONFIG_BAK 			"/nand/recovery/"				//配置文件的备份文件存放路径
#define IE_FILE_PATH			"/nand/dvr/ie/"
#define IE_FILE_LINKPATH		"/usr/sbin/dvr"

#define SHORTCUT_CONFIG_FILE			"./gui_config/shortcut.cnf"		//自定义快捷方式
#define SHORTCUT_BACKUP_CONFIG		"./gui_config/shortcut.cnf.back"	//备份自定义快捷方式

//摄像头类型
//enum SENSOR_TYPE {CCD=0,VIMICRO,OV7725,SUPERPIX};

//extern TRailWay g_stRailWay;
//extern MediaDevice mDev;

//陶永亮 090509

//视频输入通道数
#define ACTUAL_CHANNEL_NUM	Cfg_gChannelNumber()

//定义对硬件类型的区分方式
#define _video_only			(Cfg_gAudioChn() == 0)		//纯视频DVR

#define _main_stream(chn)	(chn >= 0 && chn < ACTUAL_CHANNEL_NUM)									//主码流,设置编码参数时表示即时值
#define _sub_stream(chn)		(chn >= ACTUAL_CHANNEL_NUM && chn < ACTUAL_CHANNEL_NUM * 2)//副码流
#define _main_stream_time(chn)	((chn >= (ACTUAL_CHANNEL_NUM * 2)) && (chn < (ACTUAL_CHANNEL_NUM * 3)))//dce add,主码流(定时)
#define _main_stream_event(chn)	((chn >= (ACTUAL_CHANNEL_NUM * 3)) && (chn < (ACTUAL_CHANNEL_NUM * 4))) //dce add,20130314  主码流(事件)

//主副码流通道号对应关系
#define Main2Sub(chn)		((chn) + ACTUAL_CHANNEL_NUM)											//主->副
#define Sub2Main(chn)		((chn) - ACTUAL_CHANNEL_NUM)											//副->主
#define Instant2Event(chn)		((chn) + (ACTUAL_CHANNEL_NUM * 3))		//主码流(即时)->主码流(事件)
#define Event2Instant(chn)		((chn) - (ACTUAL_CHANNEL_NUM * 3))		//主码流(事件)->主码流(即时)
#define Instant2Time(chn)		((chn) + (ACTUAL_CHANNEL_NUM * 2))		//主码流(即时)->主码流(定时)
#define Time2Instant(chn)		((chn) - (ACTUAL_CHANNEL_NUM * 2))		//主码流(定时)->主码流(即时)
#define Sub2MainEX(chn)		(_main_stream(chn) ? chn : Sub2Main(chn))

#define MAX_SMART_TEMPLATE 2	//最大智能存储模板数
int  Cfg_sInputNorm(int  _mode);
int  Cfg_gInputNorm(int*  _mode);
int Cfg_gOutputNorm(void);
int Cfg_sOutputNorm(int mode);

const NetDevice  * Cfg_gNetConfig(NetDevice* _nDev);
const TDevTV *Cfg_gDevTV(td_s32 _iDevNo);
td_s32 Cfg_sDevTV(td_s32 _iDevNo, TDevTV *_ptDevTV);
const MediaDevice  *Cfg_gMediaDevice(MediaDevice* _mDev);
	
int  Cfg_gChnPara(td_u8 _u8Channel,strChannelParam* _chnPara);

int  Cfg_sChnPara(td_u8 _u8Channel,strChannelParam* _chnPara);
strChannelParam *Cfg_gChnParaArrayInstance();
int Cfg_ChangeCurrentMode(int _iChn,int _mode);

int    Cfg_sStreamType(td_u8 _u8Channel,td_u8 _u8StreamType);
td_u8  Cfg_gStreamType(td_u8 _u8Channel);


int    Cfg_sFrameRate(td_u8 _u8Channel,td_u8 _u8FrameRate);
td_u8  Cfg_gFrameRate(td_u8 _u8Channel);
td_s32 Cfg_gAudioOnlySetFlag();

int    Cfg_sIFrameRate(td_u8 _u8Channel,td_u8 _u8IFrameRate);
td_u8  Cfg_gIFrameRate(td_u8 _u8Channel);


int    Cfg_sOsdType(td_u8 _u8Channel,int _ix,int _iy,int _iOsdType,int _iEnabled);

int	   Cfg_sOsdText(td_u8 _u8Channel,char* _cText);
int	   Cfg_gOsdText(td_u8 _u8Channel,char *_cText);

int    Cfg_sVideoQuality(td_u8 _u8Channel,td_u8 _u8VideoQuality);
td_u8  Cfg_gVideoQuality(td_u8 _u8Channel);


int    Cfg_sVideoParam(td_u8 _u8Channel,structVideoParam* _sp,int _iNum);
int    Cfg_gVideoParam(td_u8 _u8Channel,structVideoParam* _sp,int _iNum);

int  Cfg_sMotionDetectArea(td_u8 _u8Channel,td_u32 _u32Area[18]);
td_u32 Cfg_gMotionDetectArea(td_u8 _u8Channel,int _iNo);
int    Cfg_sMotionDetectSensitivity(td_u8 _u8Channel,td_u32 _u32Sensitivity);
td_u32 Cfg_gMotionDetectSensitivity(td_u8 _u8Channel);

int    Cfg_sVideoLostAlarm(td_u8 _u8Channel,td_u32 _u32Enabled);

td_u32 Cfg_gGPinEnable(td_u8 _pin);

int Cfg_sGPinLink(td_u8 _pin,td_u8 _link);
td_u32 Cfg_gGPinLink(td_u8 _pin);
	
int    Cfg_sGPoutEnable(td_u8 _pout,td_u32 _enable);

int    Cfg_sGPoutTrigger(td_u8 _pout,td_u8 _trigger);
int    Cfg_gGPoutTrigger(td_u8 _pout);

int  Cfg_sDevice(td_u32 _u32Com,char* _cDeviceType,char* _cComFormat,int iAddr,int iMode);

int    Cfg_sAlmVideoCovThreshold(td_u8 _u8Channel,td_u32 _u32Threshold);
td_u32 Cfg_gAlmVideoCovThreshold(td_u8 _u8Channel);

int    Cfg_sAlmVideoCov(td_u8 _u8Channel,td_u32 _u8Enabled);
td_u32 Cfg_gAlmVideoCov(td_u8 _u8Channel);

td_u32 Cfg_gAudioChn(void);

int Cfg_sIpFilter(TIpFilter* _stFilter);
int	Cfg_gIpFilter(TIpFilter* _stFilter);

int  Cfg_sNetDevice();

int  Cfg_gComNum(int* _iComNum, int* _iComEnable);
int Cfg_sComNum(int _iComNum, int _iComEnable);

int  Cfg_gVideoInNum(int* _iChannelNum);
int Cfg_sIPAddress(char* _cIPAddress,char* _cSubMask,char* _cGateWay,char* _cDNS, char* _cBackDNS);

//zhangweiwei  20120131
int Cfg_sndIPAddress(int bind, char* _cIPAddress,char* _cSubMask,char* _cGateWay,char* _cDNS, char* _cBackDNS);
//单独设置是否绑定
int Cfg_NetWorkMode(int _bind);

int Cfg_sMacAddress(td_u8 _u8MacAddress[6]);
int Cfg_gMacAddress(char _cMacAddress[18]);

int Cfg_sSndMacAddress(td_u8 _u8MacAddress[6]);


int  Cfg_gCoverArea(int _iChannelNum, Rect_t* rcCov, int Num);
int  Cfg_sCoverArea(int _iChannelNum, Rect_t rcCov, int Num);


int  Cfg_sBitRate(int _iChannelNum,td_u32 _u32BitRate);

int  Cfg_sVideoSize(int _iChannelNum,td_u32 _u8VideoSize);
int Cfg_gVideoSize(int _iChannelNum);
int  Cfg_sUserData(td_u8* _u8Buf,int _iOffset,int _iLength);
int  Cfg_gUserData(td_u8* _u8Buf,int _iOffset,int _iLength);

int  Cfg_sParsePara(strParsePara* _pParsePara);
const strParsePara *Cfg_gParsePara(strParsePara* _pParsePara);

//PPPoe
int  Cfg_sPPPoePara(strPPPoePara* _pPPPoePara);
const strPPPoePara *Cfg_gPPPoePara(strPPPoePara* _pPPPoePara);

int  Cfg_sAudioCoder(int _chn,int  _coder);
int  Cfg_sAudioSample(int _chn,int  _iSample);
int  Cfg_gAudioCoder(int _chn);

int InitAppChannel();
int	 Cfg_gAppChannel(strAppChannel* _pAppChannel,int _iChannel);
int	 Cfg_sAppChannel(strAppChannel* _pAppChannel,int _iChannel);


int Cfg_sNetPort(int *_iServerPort,int *_iClientPort);
int Cfg_gNetPort(int* _iServerPort,int* _iClientPort);


int Cfg_sNetUser(void* _NetUser);
int Cfg_gNetUser(void* _NetUser);
td_u32 Cfg_SetUserRight(char* _strName,char* _pcUserRight,int *_iUserRight);

int Cfg_sOsdInfo(int chn,strChannelParam chnPara[]);
int Cfg_gOsdInfo(strChannelParam chnPara[]);

int    Cfg_sEncodeMode(td_u8 _u8Channel,td_u8 _u8EncodeMode);
td_u8  Cfg_gEncodeMode(td_u8 _u8Channel);

int    Cfg_sPreferMode(td_u8 _u8Channel,td_u8 _u8PreferMode);
td_u8  Cfg_gPreferMode(td_u8 _u8Channel);
int Cfg_sMaxVideoSize(int _MaxVideoSize);
int Cfg_gMaxVideoSize(void);
int Cfg_sVideoModeCheck(int _iVideoModeCheck);
int Cfg_gVideoModeCheck(void);

int Cfg_sMaxVideoChannel(int _MaxVideochan);
int Cfg_sReduceNoise(td_u8 _u8Channel,td_u32 _u32Enable);


const STR_COMMON_INFO *Cfg_gCommonInfo(STR_COMMON_INFO* Info);

int Cfg_sCommonInfo(STR_COMMON_INFO* Info);

int Cfg_sDeviceId(char * Id);
int Cfg_gDeviceId(char * Id);

void Cfg_toDefault(int _iLocal);
//int Cfg_SaveIni(dictionary * d, char* pcIni);
int Cfg_SaveIniById(int id, char* pcIni);			//陶永亮 091119 保存配置文件的新接口
int Cfg_SetItemById(int id, const char * sec, const char * key, const char * val);

int Cfg_sDateFormat(STR_DATE_FORMAT* dateformat);
int Cfg_gDateFormat(STR_DATE_FORMAT* dateformat);

int Cfg_sHttpPort(int port);
int Cfg_gHttpPort(int* port);

int Cfg_sRtspPort(int port);
int Cfg_gRtspPort(int* port);

int Cfg_sHttpsConf(char* _cHttpPort,  char* _cHttpsPort);//zhangshenghttps
int Cfg_gHttpsPort(int* port);


int Cfg_gSwitchInfo();
int Cfg_sSwitchInfo();

int Cfg_Initialization(td_s32 s32FNum, td_char* pszFNames[]);
//void timer_CfgSave(unsigned long arg);
int Cfg_gFtpPort(int* port);
int Cfg_sFtpPort(int port);

int Cfg_Check_Port_unUsed(int _iPort, int _iType);

int Cfg_sExtendWordOverlay();
int Cfg_gExtendWordOverlay();			//Added by Leeon Tonie, 090211
STR_WordOverlay **Cfg_gExtendWordOverlayArrayInstance();

//add caohaibo start
int Cfg_gExtendWordOverlayDefault();
STR_WordOverlay **Cfg_gExtendWordOverlayDefaultArrayInstance();
STR_WordOverlay **Cfg_gExtendWordOverlayCurArrayInstance();
//add  caohaibo  end


const NtpServer *Cfg_gNtpServer(NtpServer* _stNtp);
td_u32 Cfg_sNtpServer( const char*_cServerIP ,const char* _cPort,  const char*_cInterval);

td_u32 Cfg_sDHCP(char* itemname,char* cisEnable); //Added by Leeon Tonie, 090225
//zhangweiwei add 20120131
td_u32 Cfg_sSndSHCP(char* itemname,char* cisEnable);
int Cfg_gAlarmHWInPortNum(int * _iNum);
int Cfg_sSndIPAddress(int _bind, char* _cIPAddress,char* _cSubMask,char* _cGateWay,char* _cDNS, char* _cBackDNS);
int Cfg_sNetWorkMode(int _bind);

int  Cfg_sDdnsPara(strDdnsPara* _pDdnsPara);
const strDdnsPara *Cfg_gDdnsPara(strDdnsPara* _pDdnsPara);

td_u32 Cfg_sUPNP(char* cisEnable);
int Cfg_sException(td_s32* _iList);
int Cfg_gException(td_s32* _iList);
int Cfg_gRailWay(void);
int Cfg_sRailWay(void);

int Cfg_sFastFormat(td_s32 _iEnable);		//陶永亮 090515 快速格式化磁盘
int Cfg_gFastFormat(void);				//陶永亮 090515 快速格式化磁盘
int Cfg_gCheckFs(void);					//陶永亮 090615 检测文件系统
int Cfg_gSupportHardReset(void);		//陶永亮 091118 支持硬复位
int Cfg_gWatchDogInterval(void);		//陶永亮 091118 硬件狗超时时间

int Cfg_sComAttr(td_s32 _iComNo, TCom* _pstCom); 
												//陶永亮 091117 设置串口属性
int Cfg_gComAttr(td_s32 _iComNo, TCom* _pstCom);

// 获取串口路径dxl 20110313
td_s32 Cfg_gComPath(td_s32 _iComNo, char *_strPath);
												
int Cfg_gChannelPTZ(td_s32 _iCh, TPTZChannel* _pstP);
int Cfg_sChannelPTZ(td_s32 _iCh, TPTZChannel* _pstP);
td_s32 Cfg_gAlias(TAlias* _pstAlias);
td_s32 Cfg_sAlias(td_s32 _iType, td_s32 _iNo, td_u8* _pcAlias);
int Cfg_gAlarmPortNum(td_s32* _iInportNum, td_s32* _iOutportNum);
int Cfg_gExAlarmPortNum(td_s32* _iInportNum, td_s32* _iOutportNum);
//zhangweiwei add 20111220
// 获取本地视频输入数
int Cfg_gBncInputNum(td_s32* _iVideoNum);  //zhangweiwei add 20111220
int Cfg_gIpcInputNum(td_s32* _iVideoNum);

//获取回放窗体数目
int Cfg_gPlayBackNum(td_s32* _iPlayNum);

int Cfg_gLog(td_s32* _iMinLevel, td_s32* _iFileSize, td_s32* _blSave, td_u8* _pcFileName);
int Cfg_sLog(td_s32 _iMinLevel, td_s32 _iFileSize);
//dce add,获取日志写磁盘使能
td_s32 Cfg_gLogWriteDiskEnable();

int Cfg_gLibsqliteMd5(td_u8* _pcMd5);
int Cfg_sLibsqliteMd5(td_u8* pcMd5);

//-----gxl added 20100601--------
int  Cfg_sMaxVencSize(int _iChannelNum,td_u32 _u32MaxVencSize);
td_u8  Cfg_gMaxVencSize(td_u8 _u8Channel);
int  Cfg_sMaxFrmRate(int _iChannelNum,td_u32 _u32MaxFrmRate);
td_u8  Cfg_gMaxFrmRate(td_u8 _u8Channel);

strChannelParam* GetChnParam(td_s32 _s32Chn);
strAppChannel* GetAppChnParam(td_s32 _s32Chn);
strAppChannel *Cfg_gAppChnParamArrayInstance();

int Cfg_sPreviewRec(int _iChannelNum, int _iEnc);
td_u8 Cfg_gPreviewRec(int _iChannelNum);
td_s32 Cfg_sPreviewMod(int _iPreviewMod);
td_s32 Cfg_gPreviewMod(void);

int Cfg_sVideoEncrypt(int _iChannelNum,unsigned char* _cPassword);
int Cfg_gVideoEncrypt(int _iChannelNum,unsigned char _cPassword[ENCRYPT_PWD_LENGTH+1]);
int Cfg_sCdrwPolicy(TCdrwPolicy* _pCdrwPolicy);
const TCdrwPolicy *Cfg_gCdrwPolicy(TCdrwPolicy* _pCdrwPolicy);

int Cfg_InitH323LocalPara(void);
//初始化H323本地参数
int Cfg_gH323LocalPara(TH323LocalPara* _pstH323LocalPara);
//获取H323本地参数
int Cfg_sH323LocalPara(const TH323LocalPara* _pstH323LocalPara);
//设置H323本地参数

int Cfg_InitH323GkPara(void);
//初始化H323网守参数
int Cfg_gH323GkPara(TH323GKPara* _pstH323GKPara);
//获取H323网守参数
int Cfg_sH323GkPara(const TH323GKPara* _pstH323GKPara);
//设置H323网守参数

int Cfg_sCdrwAdvBurnPara(const TCdrwAdvBurnPara* const _pCdrwAdvBurnPara);
const TCdrwAdvBurnPara *Cfg_gCdrwAdvBurnPara(TCdrwAdvBurnPara* _pCdrwAdvBurnPara);

int Cfg_sDvdResSpace(int _iResSpace);
//设置实时刻录的预留空间大小
int Cfg_gDvdResSpace(int* _piResSpace);
//获取实时刻录的预留空间大小

int Cfg_gInVolume(int _iChn);
int Cfg_sInVolume(int _iChn,int _iVolume);

// dxl 20110308
td_s32 Cfg_gChannelNumber();
td_s32 Cfg_AddRsvPort(td_s32 _uiPort);

// dxl 20110309
// 设置数字通道参数
td_s32 Cfg_sIpcParam(td_s32 _iChn, TIpChannelMsg *_ptMsg);
// 设置虚拟合成通道参数
td_s32 Cfg_sVccParam(td_s32 _iChn, TVcChannelMsg *_ptMsg);

// dlx 获取logo文件名
int Cfg_gLogoPath(char *_strPath);

// dxl设置外挂程序
int Cfg_sAppName(char *_strApp);
int Cfg_GetAppReadOnly();
char *Cfg_gAppName();

td_s32 Cfg_gAppNetType(char *_strAppType);
td_s32 Cfg_sAppNetType(char *_strAppType);

td_s32 Cfg_gRtmpInfo(td_s32 _iNo, TGuiRtmpInfoItem *_ptRtmpInfo);
td_s32 Cfg_sRtmpInfo(const TGuiRtmpInfoItem* _ptRtmpInfo);

td_s32 Cfg_gRtspInfo(td_s32 _iNo, void *_ptRtspInfo);
td_s32 Cfg_sRtspInfo(void* _ptRtspInfo);

// dxl  201000607  从配置文件获取通道类型可修改标识
// 返回值:
// -1 : 错误
// 可修类型按位取:数字通道:bit0，模拟通道:bit1，虚拟合成通道:bit2
int Cfg_gChnTypeEditAbleFlg(td_s32 _iChn);

// 设置通道类型
int Cfg_sChnType(td_s32 _iChn, td_s32 _iType, td_s32 _iParam);

// 获取VC配置
int Cfg_gVcCount(td_s32 *_piVcCnt, td_s32 *_piVcVodevEn);

//时区接口
td_s32 Cfg_gCurTimeZone(td_s32 _iForNetProt);				//当前时区
td_s32 Cfg_sCurTimeZone(td_s32 _iTmZone);	//设置时区信息
int NewTzToOldTz(int _iTmZone); //新时区转旧时区

//为外挂提供PU信息接口
const STR_NET_MANAGE_INFO * Cfg_gManageInfo(STR_NET_MANAGE_INFO* NetInfo);
int Cfg_sManageInfo(STR_NET_MANAGE_INFO* NetInfo);

//为外挂提供相关的sip协议接口2011/10/19
const TSIPDeviceInfo* Cfg_GetSIPConfig(TSIPDeviceInfo* pSipConfig);
int Cfg_SetSIPConfig(TSIPDeviceInfo* pSipConfig);

int Cfg_GetSIPChnPara(int _iChn, TSIPChnPara* _pSipChn);
int Cfg_SetSIPChnPara(TSIPChnPara* _pSipCh);

int Cfg_GetSIPAlarmPara(int _iChn, TSIPAlarmPara* _pSipAlarm);
int Cfg_SetSIPAlarmPara(TSIPAlarmPara* _pSipAlarm);
int disk_Crtl_Check(td_s32 _iEnable);
int Disk_Get_Check();
int Disk_Get_Repair();
int disk_Repair_Check(td_s32 _iEnable);


int Cfg_SetStorageStat(td_s32 _s32Chn, char *_sEnable);
int Cfg_GetStorageStat(td_s32 _s32Chn);
int Cfg_SetBackupStat(td_s32 _s32Chn, char *_sEnable);
int Cfg_GetBackupStat(td_s32 _s32Chn);


int Cfg_SetIpChannle(int _iChn, int  _Enable);
int Cfg_GetIpChannle(int _iChn);
int Cfg_gVideoSizeList(int _iChn, int  *_ptVideoList, int _iMaxCnt);
int Cfg_sVideoSizeList(int _iChn, const int *_ptVideoList, int _iCnt, int _iSaveToCfg);

int Cfg_gFrameRateList(int _iChn, int  *_ptFrameList, int _iMaxCnt);
int Cfg_sFrameRateList(int _iChn, const int *_ptFrameList, int _iCnt, int _iSaveToCfg);
//add by nsg 20120129
//设置混音通道参数
td_s32 Cfg_sMixAudioChn(const TMixAudio *_ptMixaudioMsg);
//获取混应通道设置参数
td_s32 Cfg_gMixAudioChn(td_s32 _uiMixAduiochn,  TMixAudio *_ptMixaudioMsg);

//added by lc
//获取通道静音检测阈值
td_s32 Cfg_gMuteBounds(td_s32 _iChn);
//设置通道静音检测阈值
td_s32 Cfg_sMuteBounds( td_s32 _iChn,td_s32 _iBounds);

// add by yangjue 20120213
//获取SATA硬盘分区数
td_s32 Cfg_gSataPartNum(void);
//获取U盘分区数
td_s32 Cfg_gUsbPartNum(void);
td_s32 Cfg_gSdCardPartNum(void);

//获取ptz控制使能标志
td_s32 Cfg_gPtzExEnable(void);
//获取ptz记录模式路径使能
td_s32 Cfg_gPtzExSetRoutineEnable(void);

td_s32 Cfg_ResetViNorm(int _iViNorm);

// 杨珏20120418 报警联动单画面巡航时间
td_s32 Cfg_gAlarmSinglePicIntv(td_s32* _iInterval);
td_s32 Cfg_sAlarmSinglePicIntv(td_s32 _iInterval);

/*************************************************************
* get vca type for 3531 vca device add by caizhaoxu 2012-05-22
**************************************************************/
td_s32 Cfg_gChannelVcaType(int _chn);
td_s32 Cfg_sChannelVcaType(int _chn, int _vca_type);


//add by nsg 120905
//从配置文件获取ATM配置信息

int Cfg_gAtmPara(TAtmPara *_pstAtmPara);

//设置ATM参数配置
int Cfg_sAtmPara(TAtmPara _stAtmPara);

//设置图片抓拍参数
int Cfg_sSnapShotPara(td_u8 _u8Channel, TSnapShot _stSnapShotPara);

//获取卡号叠加信息
int Cfg_gAtmCardOverlay(int iOverlayChn, STR_WordOverlay *_pstWordOverlay);
//设置卡号叠加信息
int Cfg_sAtmCardOverlay(int iOverlayChn, STR_WordOverlay _stWordOverlay);

//设置FTP配置信息
td_s32 Cfg_sFTPPara(TFtpPara _stFtpPara);
//获取FTP配置信息
td_s32 Cfg_gFTPPara(TFtpPara *_stFtpPara);
//获取FTP配置信息实例
TFtpPara* Cfg_gFTPParaArrayInstance();
//图片抓拍联动参数设置
int Cfg_sSnapShotLinkPara(td_u8 _u8Channel, int _iMode);


//end
//20130314 SUN Wei add for smtp
int Cfg_sSmtpServerInfo(TSmtpPara * _smtp_config);
int Cfg_gSmtpServerInfo(TSmtpPara * _smtp_config);
TSmtpPara* Cfg_gSMTPParaArrayInstance();
//20130314 SUN Wei add for smtp end

//wsl 20120904 裁边区域的设置和获取
td_s32 Cfg_gChnClipRect(td_s32 _LogChn, TRitioRect *_PtRect);
td_s32 Cfg_sChnClipRect(td_s32 _LogChn, TRitioRect *_PtRect);

int  Cfg_sChnProfile(int _iChannelNum,td_s32 _s32Profile);



//end

//add by sunln 20120703
//get passwd from config_server.ini
int Cfg_Get_Encrypt(int _iChannelNum,unsigned char _cPassword[ENCRYPT_PWD_LENGTH+1]);

int Cfg_Set_Ipcdecrypt(int _iChn,char *_pAsswd);//save dig passwd 
void Cfg_Rm_Burn();//删除断电复刻配置文件
td_s32 Cfg_GetVcChnNo();//获取vc通道号，-1表示失败
//end
//add end

TPlugConf *Cfg_gPlugConf(void);
int Cfg_sPlugConf(TPlugConf *_ptPlug);
int Cfg_sPlugSeverMac(td_s32 _iChn, char *_pstrMac);
//shc add 20120919
td_s32 NetWifi_Cfg_sAPDHCP(td_char *_cdhcpstart,td_char *_cdhcpend, td_char *_dhcplease);
td_s32 NetWifi_Cfg_sWIFIAP(td_char *_cIPAddress,td_char *_cSubMask,td_char *_cGateWay,td_char *_cDNS, td_char* _cESSID,td_char* _cENCRYPTION,td_char *_cPWDTYPE,td_char* _cPWD, td_char* _cKeyNum);
td_s32 NetWifi_Cfg_sWifiMode(td_char *_cwifimode);
td_s32 NetWifi_Cfg_sAPDHCPEnable(td_char *_cdhcpenable);
td_s32 NetWifi_Cfg_sWIFIAPIPAddress(td_char* _cIPAddress,td_char* _cSubMask,td_char* _cGateWay,td_char* _cDNS);

td_s32 SetDevProducter(td_s32 _iProducter);

td_s32 GetDevProducter();

//get avd alarm delay from config file (delay time unit: second)
int Cfg_gAVDAlarmDelaySec(td_s32* _alarm_delay);

int Cfg_gMDAlarmDelaySec(td_s32 * _alarm_delay);

//获取是否开启获取音频状态标志
int Cfg_GetAudioStatFlg(int *_piFlg);

//获取前端数字通道校时时间间隔
int Cfg_GetAdjustDigChnTimeFrequency(int *_piFlg);

//获取是否开启合成通道控制主通道的云台功能
int Cfg_GetVcCtrlMainPicFlg(int *_piFlg);//返回值见枚举TPREVIEWSAMETYPE

int Cfg_GetVcBindFlg(int *_piFlg);
int Cfg_SetVcBindFlg(int _iFlg);
int Cfg_InitVcBindFlg(void);

typedef enum
{	
	PRE_NO_SAME,
	PRE_ALL_SAME,
	PRE_VC_VGA_SAME,
	PRE_VC_HDMI2_SAME,
}TPREVIEWSAMETYPE;    //预览模式同源类型

//初始化合成通道控制主通道的云台功能的标志变量
int Cfg_InitVcCtrlMainPicFlg(void);
	
//初始化主机的funclist参数	
int Cfg_InitFunclist(void);

//获取主机的funclist参数
int Cfg_gGetFunclist(int *_piFlg);

//设置主机的funclist参数
int Cfg_sGetFunclist(int *_piFlg);
td_s32 Cfg_gVcChnNum(int _iInputChnNum);
td_s32 Cfg_sChnBurnPara(td_s32 _iChannelNum,td_s32 _iVideoSize,
									td_s32 _iEncodeType,
									td_s32 _iBitrate,
									td_s32 _iQuality);
td_s32 Cfg_gChnBurnPara(td_s32 _iChannelNum,td_s32 *_iVideoSize,
									td_s32 *_iEncodeType,
									td_s32 *_iBitrate,
									td_s32 *_iQuality);


//获取混音延时时间
td_s32 Cfg_gVcSleepTime();

//设置vc混音延时时间
td_s32 Cfg_sVcSleepTime(int _ivcSleepTime);

td_s32 Cfg_gDatapoolMaxSize();

//音频制式列表
enum
{
	AUDIO_CODEC_FORMAT_G711A = 1,
	AUDIO_CODEC_FORMAT_G711Mu = 2,
	AUDIO_CODEC_FORMAT_ADPCM = 3,
	AUDIO_CODEC_FORMAT_G726 = 4,
	AUDIO_CODEC_FORMAT_AAC_LC = 22,
}AUDIO_CODEC_LIST_E;
td_s32 Cfg_sAudioCoderList(int _iChn, const int *_ptAudioCoderList, int _iCnt);
int Cfg_gAudioCoderList(int _iChn, int  *_ptAudioCoderList, int _iMaxCnt);
//shihc add 20130328
td_s32 Cfg_gSNMP();
td_s32 Cfg_sSNMP(td_s32 _isetable);
td_s32 Cfg_gRtspAuthor();//rtspauthor
td_s32 Cfg_sRtspAuthor(td_s32 _isetable);//rtspauthor
td_s32 Cfg_gVcaCfgVideo();//获取智能分析的分析分辨率
td_s32 Cfg_gVcaStartPolicy();//获取智能分析的启动策略

//20130327 王磊add 通道录像保留时长写配置
int Cfg_SetStoreTime(int _iChn, char *_cDays);
//20130327 王磊add 从配置文件中获取通道录像保留时长
int Cfg_getStoreTime(int _iChn);

//20130401 王磊add 抽帧使能写配置
int Cfg_SetFramePick(int _iChn, char * _pcPickFrame);
//20130401 王磊add 从配置文件中获取抽帧录像状态
int Cfg_GetFramePick(int _iChn);
//副码流录像add
int Cfg_SetSubRecord(int _iChn, char* _pcSubRecord);
int Cfg_GetSubRecord(int _iChn);
//副码流录像end
//20130408 王磊add 冗余使能写配置
int Cfg_SetRedundRecState(int _iChn, char * _pcRedundRec);

//20130408 王磊add 从配置文件中获取抽帧录像状态
int Cfg_GetRedundRec(int _iChn);


// liyang add
int cfg_getUserAuthority (NetUser* _NetUser);
int cfg_setUserAuthority(NetUser *_ptNetUser);

int cfg_getUserGroupDefaultAuthority(int _iGroupId, OUT unsigned int *_iAuthority, int _iLen, int _iType);
int cfg_getUserGroupMaxAuthority(int _iGroupId, OUT unsigned int *_iAuthority, int _iLen, int _iType);


/**************************User Right File Header Start*******************************/
#define CFG_USER_RIGHT_ID	0x10000000
#define CFG_USER_VERSION		1
typedef struct
{
	unsigned int m_u32FileId;//用于标记文件的合法性，0x10000000为合法
	unsigned int m_u32Version;//标记文件的版本号
	unsigned int m_u32RecoderSum;//记录总条数
	unsigned int m_iRecoderLen;//记录长度
	unsigned int m_u32FileLen;//文件总长度
	unsigned int m_iReserved[4];//预留位置
}TUserCfgFileHeader;
/**************************User Right File Header End*******************************/

//dce add,保存主码流(定时)参数
td_s32 Cfg_setMainTime(td_s32 _iChn, TChannelParamEvent *_ptParam);

//dce add,保存主码流(事件)参数
td_s32 Cfg_setMainEvent(td_s32 _iChn, TChannelParamEvent *_ptParam);

//dce add, 获取主码流(定时)参数
TChannelParamEvent* GetChnParamTime(td_s32 _iChannelNo);

//dce add,获取主码流(事件)参数
TChannelParamEvent* GetChnParamEvent(td_s32 _iChannelNo);

//dce add,从配置文件中获取主码流(事件)参数
td_s32 Cfg_gChnParaEvent(td_s32 _iChannel, TChannelParamEvent *_pstChnParaEvent);

//dce add, 初始化保存主码流定时参数
td_s32 Cfg_gChnParaTime(td_s32 _iChannel, TChannelParamEvent *_pstChnParaTime);

td_s32 Cfg_setMainSmart(td_s32 _iChn, td_s32 _iTemplate, TChannelParamSmart *_ptParam);
TChannelParamSmart *Cfg_getMainSmart(td_s32 _iChn, td_s32 _iTemplate);
td_s32 Cfg_gChnParaSmart(td_s32 _iChannel, td_s32 _iTemplate, TChannelParamSmart *_pstChnParaSmart);

int cfg_gAoInitVolume(void);

int cfg_sAoInitVolume(int _iVolume);
int cfg_getMaxIPCNetBitrate(void);
td_s32 Cfg_getAudioPreviewAttribute(int *_piMod, int *_piDefaultLR, int *_piLeft, int *_piRight, int *_piLeft1, int *_piRight1);
td_s32 Cfg_setAudioPreviewAttribute(int _iMod, int _iDefaultLR, int _iLeft, int _iRight);
int cfg_getMaxNetTransmitBitrate(void);
int Cfg_gVideoModleEnable(int _iCh);
int  Cfg_sVideoModleEnable(int _iCh);

// for decoder
int Cfg_sControlInfor(TPlatformCtrlInfo * _DeControl);
const TPlatformCtrlInfo  * Cfg_gControlInfor(TPlatformCtrlInfo * _DeControl);
int Cfg_sDevInfor(TPlatformDevInfo * _pDev);
const TPlatformDevInfo * Cfg_gDevInfor(TPlatformDevInfo * _pDev);
int Cfg_sChannelInfor(int iChannelNo,char * _cName);
int Cfg_gChannelInfor(int iChannelNo,char * _cName);

int Cfg_sAlarmInfo4Dec(int _iAlarmOut ,int _iAlarmType);
int Cfg_gAlarmInfo4Dec(int* _iAlarmOut,int* _iAlarmType);
int Cfg_sSaveLastFrame(int _iSaveLastFrameFlag);
int Cfg_gSaveLastFrame(int * _iSaveLastFrameFlag);
int Cfg_sPreviewPrefer(int _iPreviewPrefer);
int Cfg_gPreviewPrefer();
int Cfg_gDvrType(void);
//解码器logo
int  Cfg_sLogoInfo4Dec(TLogoParam* _strLogoPara);
int  Cfg_gLogoInfo4Dec(TLogoParam* _strLogoPara);
int Cfg_s485Addr4Dec(td_s32 _iComNo, int _iComAddr);//设置解码器485地址(用于解码器串联)
int Cfg_g485Addr4Dec(td_s32 _iComNo, int * _iComAddr);//获取解码器485地址(用于解码器串联)

int Cfg_gVoNovideo();
int Cfg_sVoNovideo(int _iNovideo);

//shihc add 20130825 增加车载相关读写配置文件

td_s32  Cfg_gWifi_Mode();
td_s32	Cfg_sWifi_Mode(td_s32 _iMode);
td_s32	Cfg_gWifi_Sta_Para(strWifiCnf * _pWifipara);
td_s32	Cfg_gWifi_Ap_Para(NetWifiAP * _pWifipara);
#if 0
td_s32 Cfg_s3GPara(ST3G_INFO *_ps3gInfo,td_char* _pcSim);
td_s32 Cfg_g3GPara(ST3G_INFO *_ps3gInfo,td_char *_pcSim);
#endif
td_s32 Cfg_gWifi_FtpAutoEn();
td_s32 Cfg_sWifi_FtpAutoEn(td_s32 _iEnable);
td_s32 Cfg_sWifi_Sta_Para(strWifiCnf * _pWifipara);
td_s32 Cfg_sWifi_Ap_Para(NetWifiAP * _pWifipara);

//4g相关
td_s32 Cfg_g4GEnable();
td_s32 Cfg_g4GPara(ST3G_INFO *_ps3gInfo ,td_char *_pcSim);
td_s32 Cfg_s4GPara(ST3G_INFO *_ps3gInfo,td_char* _pcSim);

//dce add about vehicle_mounted
//从配置文件中读取车载模块相关参数
td_s32 Cfg_GetVehclPara();

//从内存中获取联动打点使能标志
td_s32 Cfg_GetLinkVideoMark();
//dce add 电源管理相关
td_s32 Cfg_sPowerAlarmMsg (td_s32 _iAlarm);
td_s32 Cfg_sPowerOffMsg (td_s32 _iShutDown);
td_s32 Cfg_gPowerAlarmMsg ();
td_s32 Cfg_gPowerOffMsg ();
td_s32 Cfg_sPowerManagerMsg(td_s32 _iOnEnable, td_s32 _iOnTime, td_s32 _iOffEnable, td_s32 _iOffTime);
td_s32 Cfg_gPowerManagerMsg (td_s32 *_piOnEnable, td_s32 *_piOnTime, td_s32 *_piOffEnable, td_s32 *_piOffTime);
td_s32 Cfg_gVoltageAlmDelay();
//add end

//获取车载模块使能
td_s32 Cfg_gVehicleEnable();

td_s32 Cfg_sGPSMsgOverlay(td_s32 _iEnable);
td_s32 Cfg_gGPSMsgOverlay();
td_s32 Cfg_sGPSTimeEnable(td_s32 _iEnable);
td_s32 Cfg_gGPSTimeEnable();
td_s32 Cfg_sGPSMsgChnOverlay(td_s32 _iEnable);
td_s32 Cfg_gGPSMsgChnOverlay();
td_s32 Cfg_sGPSFilter(td_s32 _iFilter);
td_s32 Cfg_gGPSFilter();

//add end

//by zhangsheng
int cfg_sAutoMaintain(int _iRebootDay, int _iRebootTime);
int cfg_gAutoMaintain();
int cfg_getIpChnHeart();
td_s32 Cfg_gAACfgEnable(void);
int cfg_gAppcomsend(void);

//是否初始化字库,为1表示不初始化
int Cfg_gFontSupportInfo();
td_s32 Cfg_sVideoOutMode(int _iMode);
td_s32 Cfg_gVideoOutMode(int * _iMode);
td_s32 Cfg_gDBErrRebldEnable();
td_s32 Cfg_getNoVaildVideoSize(TVideoSize **_ptVideoSize, td_s32 *_piCnt);
int Cfg_gMasterChip();
int cfg_getDelFileSize();// 获取每次录像删除文件的大小

//哈希校验功能设置和获取
td_s32 Cfg_GetCdrwHashEnable(void);
td_s32 Cfg_sCdrwHashEnable(td_s32 _iEnable);
td_s32 Cfg_gCdrwHashEnable(void);


const char *Cfg_gRecExtName(void);

//政法行业获取固定密码
int Cfg_gPoliticsVideoEncrypt(char _cPassword[ENCRYPT_PWD_LENGTH+1], int *_iEnable);
int Cfg_sPoliticsVideoEncrypt(char *_cPassword, int _iEnable);


//设置提取纯音频格式文件使能
td_s32 Cfg_sSplitAvChnEnable(td_s32 _iChn, td_s32 _iEnable);
td_s32 Cfg_gSplitAvEnable();

//提取刻录以及备份纯音频格式文件使能
td_s32 Cfg_gBurnSplitAvEnable(void);
td_s32 Cfg_sBurnSplitAvEnable(td_s32 _iEnable);
td_s32 Cfg_gBkBurnSplitAvEnable(void);
td_s32 Cfg_sBkBurnSplitAvEnable(td_s32 _iEnable);
td_s32 Cfg_InitBkBurnSplitAvEnable(void);
td_s32 Cfg_InitBurnSplitAvEnable(void);
td_s32 Cfg_sRaidStat(td_s32 _iEnable);
td_s32 Cfg_gRaidStat(td_s32 *_piEnable);
//设备支持最大帧率
td_s32 Cfg_gMaxFrameSupport(void);

//28181是否使能纯视频
td_s32 Cfg_gAVBoth();
td_s32 Cfg_sAVBoth(td_s32 _isetable);

//设置获取风扇转动温度阀值
int Cfg_sDevTemp(int _iStartTemp);
int Cfg_gDevTemp();
//获取直刻使能以及直刻所用硬盘号
td_s32 Cfg_InitFlyBurnEnable(void);
td_s32 Cfg_gFlyBurnEnable(void);
td_s32 Cfg_InitFlyBurnSataNum(void);
td_s32 Cfg_gFlyBurnSataNum(void);
td_s32 Cfg_InitOverLapStopRecord(void);
td_s32 Cfg_gOverLapStopRecord(void);


//设置视频源切换
td_u32 Cfg_InitViMod();
td_s32 Cfg_SetViMod(td_s32 _iChn, td_s32 _s32ViMod);
td_s32 Cfg_GetViMod(td_s32 _iChn);

//获取设置音频频拼接使能
int Cfg_gMedleyAVEnable(int _iChn); //获取音频分离拼接使能
int Cfg_sMedleyAVEnable(int _iChn, int _iEnable); //设置音频分离拼接使能
int Cfg_InitBackFileFormat(void);
int Cfg_InitBurnFileFormat(void);
int Cfg_gBackFileFormat(int *_piFileFormat);
int Cfg_gBurnFileFormat(int *_piFileFormat);
int Cfg_sBurnFileFormat(int _iFileFrmt);
int Cfg_sBackFileFormat(int _iFileFrmt);
//int Cfg_GetDzModel(); //不需要判断，改为标准功能
int Cfg_InitBackSingleFileFormat(void);
int Cfg_sBackSingleFileFormat(int _iFileFrmt);
int Cfg_gBackSingleFileFormat(int *_piFileFormat);

int Cfg_InitBurnSingleFileFormat(void);
int Cfg_sBurnSingleFileFormat(int _iFileFrmt);
int Cfg_gBurnSingleFileFormat(int *_piFileFormat);

int Cfg_gAudioVolume(td_s32 _iId, td_s32 *_piVolume);
int Cfg_sAudioVolume(td_s32 _iId, td_s32 _iVolume);
int Cfg_InitBurnMp4DatSize (void);
/*
函数名 	    :	Cfg_InitBurnMp4DatSize
功能	    :	初始化mp4的mdat大小
输入参数	:	 无
返回值	: 	〉=0 成功  -1 失败

*/
int Cfg_gBurnMp4DatSize (int *_piBurnMp4DatSize);
/*
函数名 	    :	Cfg_gBurnMp4DatSize
功能	    :	获取mp4的mdat大小
输入参数	:	 _piBurnMp4DatSize : mdat大小
返回值	: 	〉=0 成功  -1 失败

*/
int Cfg_InitMp4TailSize (void);
/*
函数名 	    :	Cfg_InitMp4TailSize
功能	    :	初始化mp4尾大小
输入参数	:	 无
返回值	: 	〉=0 成功  -1 失败

*/
int Cfg_gMp4TailSize (int *_piMp4TailSize);
/*
函数名 	    :	Cfg_gMp4TailSize
功能	    :	获取mp4尾大小
输入参数	:	 无
返回值	: 	〉=0 成功  -1 失败

*/
int Cfg_InitPerFileLen(void);
/*
函数名 	    :	Cfg_InitPerFileLen
功能	    :	无硬盘时，初始化单个文件打包的时间
输入参数	:	 无
返回值	: 	〉=0 成功  -1 失败
*/
int Cfg_gPerFileLen(void);
/*
函数名 	    :	Cfg_gPerFileLen
功能	    :	无硬盘时，获取单个文件打包的时间大小
输入参数	:	 无
返回值	: 	打包的时间
*/


td_s32 Cfg_getAoChnCnt(void);

int Cfg_getJoinAacEnable(); //连接分离出来的独立音频为一个文件
int Cfg_getSplitVcChnAacEnable(); //使能独立音频分离
int Cfg_InitThirdIniConfig(); //获取使能参数


int Cfg_getRealVideoChnNum(); //视频通道数，不包括虚拟通道
int Cfg_gStoragePortNum(int _iType);

int Cfg_InitVirDiskSize (void); // 初始化虚拟硬盘大小

int Cfg_sSchedulePort(int _iPort); //设置排期端口号
int Cfg_gSchedulePort(int *_iPort);//获取排期端口号

int Cfg_gShowPreviewFrameEnable();//获取显示预览边框使能

//=================================================================
//以下接口为政法行业增加的能力集支持的获取接口

int Cfg_getScheduleEnable(); //获取是否支持庭审主机排期
int Cfg_getProofEnable();//获取是否支持示证切换
int Cfg_getVcSetVodevListEnable();//获取是否支持合成画面输出设备设置
int Cfg_getVcSetVoDevList(int *_iVoDevList, int _iMaxCnt); //获取支持合成画面输出设备列表

#endif

