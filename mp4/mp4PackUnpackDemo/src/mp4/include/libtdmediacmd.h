/*******************************************************************
文件名	:assistant.h
公司	:天津天地伟业数码科技有限公司
创建者	:陶永亮
创建日期:2009-10-28
简要描述:媒体层辅助模块数据定义及接口函数原型
修改记录:2009-10-28,陶永亮，整理创建
        
******************************************************************/
#ifndef _ASSISTANT_H_
#define _ASSISTANT_H_
#include "guiparameter.h"
#include "ptz.h"
#include "vca.h"
#include "atm_business.h"
#include "user.h"
#include "vehicle_mounted.h" 
#include "wifi.h"
#include "preview.h"
#define ONEKEY_BURN         0  //add by shishengnan 2010/11/16 打包时不能一键刻录
#define PLAYBACK            1  //add by shishengnan 2010/11/16 刻录和打包时不能回放
#define SET_PACKAGE_POLICY  2  //设置打包策略 刻录时只能按时间打包
#define SET_DELETE_POLICY   3  //设置磁盘满时的删除策略，刻录时只能选择删除文件
#define TAG_QUERY           4  //查询打点信息， 刻录或打包时不能查询
#define SET_VIDEO_ENCODER   5  //设置视频编码，修改帧率，分辨率，音频编码会打包，
      						   //刻录时修改会导致各个通道文件时间段不统一，因此加以限制
#define FORCE_PACKAGE       6  //录像文件强制打包

#define ADD_PTZ_PROTOCOL	0	//添加协议
#define DEL_PTZ_PROTOCOL	1	//删除协议
#define PTZ_PROTOCOL_USE	0	//已添加协议
#define PTZ_PROTOCOL_DEL	1	//待添加协议
#define	PTZ_CRUISE_START	0	//开始调用巡航轨迹
#define PTZ_CRUISE_STOP		1	//停止调用巡航轨迹


typedef struct
{
	int m_iConnectMod;		//低16位表示 模式 ip/域名/主动模式，高16位0-私有，1-onvif  例如:
	char m_strUrl[GUI_DIG_CHN_URL_LEN  + 1]; //服务器地址
	char m_strProxy[GUI_DIG_CHN_URL_LEN  + 1]; //代理IP
	int m_iPort;	//端口号
	int m_iChn;	//对方通道号
	int m_iStreamType;	//码流类型,0:主码流, 1副码流
	int m_iConntectType;  //tcp/udp/bc
	int m_iPtzAddr;	//ptz地址
	char m_strPtzProtocol[GUI_DIG_CHN_PTZ_NAME + 1]; //ptz协议
	char m_strUserName[GUI_DIG_CHN_NAME_LEN + 1]; //用户名
	char m_strUserPsw[GUI_DIG_CHN_NAME_LEN + 1];	//密码
	int m_iEnable;    //状态
	char m_strVdecPsw[GUI_DIG_CHN_NAME_LEN + 1];	//视频解密密码
	td_u8 m_strRtspUrl[256];	// 用于扩展RTSP的地址，当RTSP 地址长度大于80字节时，使用本字段，否则使用原字段。
} TDigChnPamaMsg;


typedef struct
{
	int m_iRuleEnable;			//规则是否使能
	int m_iTripwireEnable;		//单绊线事件是否使能
	int m_iPerimeterEnable;		//周界事件是否使能
	int m_iFaceEnable;			//人脸侦测事件是否使能
	int m_iDiagnoseEnable;		//视频诊断事件是否使能
	int m_iLeaveEnable;         //物品遗留事件是否使能
	int m_iLostEnabe;           //物品丢失事件是否使能
	int m_iCapEnable;			//智能跟踪事件是否使能
	int m_iCpcEnable;           //流量统计事件是否使能
	int m_iCddEnabe;            //人群聚集事件是否使能
	int m_iSvdEnable;		//离岗检测事件是否使能

} TVcaRuleEnableType;

typedef struct
{
	int 					m_iChannel;
	int 					m_iRulesNo;
	TVcaRuleEnableType	m_iEnableMsg;
} TVcaRuleEnableStat;

typedef struct
{
	td_s32 m_iIpcInsert;  
	td_s32 m_iRemotePreview;
	td_s32 m_iRemoteDownload;   
	td_s32 m_iRxSum;   
	td_s32 m_iTxSum; 
}TCmdNetPerformanceCost; 

typedef void (*td_cmd_ProcRebldProgCallBack)(int);

void GetZoomTimes(int _iChn,int *_piPx,int *_piPy);
//int Storage_Init();
//int GetComFormat(COM_Params* _comParams,int _iComNum);




//---------------------------------------------------------------------------
//int td_cmd_setwifinetconfig(char* const ip,char* const mask, char* const gateway,char* const dns, char* essid, char* encripy, char* username, char* pwd, char* keynum);//xujiayu wifi
int td_cmd_setInputNorm(td_s32 _iChn, const int mode);
int td_cmd_getInputNorm(td_s32 _iChn, int * mode);
int td_cmd_setOutputNorm(const int mode);		//Added by Leeon Tonie for Tiandy-Dvr, 090106
int td_cmd_getOutputNorm(int * mode);		//Added by Leeon Tonie for Tiandy-Dvr, 090106
int td_cmd_getvideopara(const int chn,structVideoParam* _sp,int _iNum);
int td_cmd_getvideopara(const int chn,structVideoParam* _sp,int _iNum);
td_s32 td_cmd_changevideopara(const td_s32 _iChn, td_s8* const bright, td_s8* const contrast, td_s8* const saturation, td_s8* const hue);
int td_cmd_getvideoquality(const int chn);  //modified by gxl 081023
td_s32 td_cmd_setvideoquality(const td_s32 _iChn,td_s8* const _cQua); //modified by gxl 20081023
int td_cmd_getframerate(const int chn); //modified by gxl 20081023
td_s32 td_cmd_setframerate(const td_s32 _iChn,td_s8* const _Framerate); //modified by gxl 20081023

int td_cmd_getiframerate(const int chn);
td_s32 td_cmd_setiframerate(const td_s32 _iChn,td_s8* const _Framerate);

int td_cmd_getstreamtype(const int chn);
td_s32 td_cmd_setstreamtype(const td_s32 _iChn, td_s8* const _streamtype);
//SUN Wei add for 7.6
td_s32 td_cmd_setmotiondetectenable(const int _iChn, td_s32 _iEnable);
//SUN Wei add for 7.6 end
td_s32 td_cmd_getmotiondetectarea(const td_s32 _iChn, td_s8* area[]);
td_s32 td_cmd_setmotiondetectarea(const int _iChn, char* const area[]);
int td_cmd_getmotiondetectsensitivity(const int chn);
td_s32 td_cmd_setmotiondetectsensitivity(const td_s32 _iChn, td_s8* const _threshold);
td_s32 td_cmd_setOverlaydetectenable(const int _iChn, td_s32 _iEnable);


//---------------------------------gxl-----------------------------------------
td_s32 td_cmd_setdateformat(td_s32 _iMode, td_u8* _pcSeperatechar);			//Added by Leeon Tonie 090206, as an interface function.
td_s32 td_cmd_getdateformat(td_s32* _piMode, td_u8* _pcSeperatechar);		//陶永亮 100107
td_s32 td_cmd_getosdtext(const td_s32 _iChn, td_s8* _text);
td_s32 td_cmd_setosdtext(const td_s32 _iChn, td_s8* const _text);
int td_cmd_getosdpos(const int chn, int distype, int* enable);							//陶永亮 100322

td_s32 td_cmd_getosddisplayex(const td_s32 _iChn, td_s32 layno,td_s32 _iDistype, td_s32* x, td_s32* y, td_s32* _iEnable);

#define td_cmd_getosddisplay(chn, distype, x, y, enable)  td_cmd_getosddisplayex(chn, 0, distype, x, y, enable)

td_s32 td_cmd_setosddisplay(const td_s32 _iChn, td_s8* const _iDistype, td_s8* const x, td_s8* const y, td_s8* const _iEnable);
unsigned int td_cmd_get_osdcolor( int _iChn, int _iOSDType);
int td_cmd_set_osdcolor(int _iChn, int _iOSDType, unsigned int _iColor);	//陶永亮 100210 从cmd_execute.h移至此
td_u32 td_cmd_get_fontsize( td_s32 _iChn, td_s32 _iOSDType);
int td_cmd_setfontsize(td_s32 _iChn, td_s32 _iOSDType, td_u32 _iFontSize);
int td_cmd_getcoverarea(const int chn, int* left, int* top, int* right, int* bottom, const int Num);

td_s32 td_cmd_setcoverarea(const td_s32 chn,char* const left,td_s8* const top,td_s8* const right,td_s8* const bottom, const td_s32 Num);//modified by gxl 081022

//int td_cmd_getmdenable(const int chn,const char* enable);  //gxl modified 20081119
//int td_cmd_setmdenable(const int chn,char* const enable);  //gxl modified 20081119
int td_cmd_getbitrate(const int chn); //gxl modified 20081119
td_s32 td_cmd_setbitrate(const td_s32 _iChn, td_s8 * const _Bitrate);  //modified by gxl 20081119

td_s32 td_cmd_setaudiocoder(const td_s32 _iChn, td_s8* const _cCoder);
const TDevTV *td_cmd_getdevTV(const td_s32 _iDevNo);
td_s32 td_cmd_setdevTV(TGuiDevTVMsg *_pGuiDevTv);
td_s32 td_cmd_resumedevTV(const td_s32 _iDevNo);
int td_cmd_getaudiocoder(const int chn); //gxl modified 20081119
int td_cmd_getencodetype(const int chn); //gxl modified 20081119
td_s32 td_cmd_setencodetype(const td_s32 _iChn, td_s8* const _cType);  //modified by gxl 20081119
int td_cmd_getvideosize(const int chn); //gxl modified 20081119
td_s32 td_cmd_setvideosize(const td_s32 _iChn, td_s8* const videosize); //modified by gxl 20081119
td_s32  td_cmd_setprofile(const td_s32 _iChn, td_s32* const videoprofile);
 td_s32 td_cmd_getvideoprofile(const td_s32 _iChn);


td_s32 td_cmd_setaudiosample(const td_s32 _iChnStart, const td_s32 _iChnEnd, td_u32 const _s32AudioSample);
 td_s32 td_cmd_getprefermode(const td_s32 chn,const td_s8* prefermode); //不再支持

td_s32 td_cmd_setprefermode(const td_s32 chn, td_s8* const prefermode); //不再支持

 td_s32 td_cmd_getaudiochn(const td_s32 chn,const td_s8* audiochn); //?

 td_s32 td_cmd_setaudiochn(const td_s32 chn, td_s8* const audiochn); //?
//------------------------------------------------------------------------------------------------------------
int td_cmd_usbaction(const int action);
td_s32 td_cmd_getrecordschedule(const td_s32 chn,const td_s32 week,const td_s8* time);

td_s32 td_cmd_setrecordschedule(const td_s32 chn,const td_s32 week,td_s8* const time,td_s8* const mode);
int td_cmd_Store_Query(td_u32 _uiSocket, const int chn,const char* recType,const char* beginTime,const char* endTime,const char* pageSize,const char* pageNo, char* fileType, char* devType, const char *reserved , const char* trigger, char* cMsg); //modified by gxl 20081024
int td_cmd_Store_Query_Atm(td_u32 _uiSocket, const int chn,const char* recType,const char* beginTime,const char* endTime,const char* pageSize,const char* pageNo, char* fileType, char* devType, const char *reserved , const char* trigger, const char* atmQueryType, const char* pcStr, TAtmQuery* _tQuery,char* cMsg);

//------------------------------------------------------------------------------------
int td_cmd_ForceIFrame(const int chn);
//int td_cmd_setsmtpinfo(const char* ip,const char* port,const char* account,const char *password,const char* authtype,const char* emailaddr,const char* emailsubject );
//int td_cmd_setsmtpenable(const int chn,const char *smtpenable);
int td_cmd_ReduceNoise(const int chn,int enable);
int td_cmd_setosddiaphaneity(const int chn, const int dia); //gxl added 20081023
int td_cmd_setwordoverlay(int _iChn, int _iX, int _iY, int _iWordSize, td_u8* _cWord, int _blStore);
int td_cmd_setMultiwordoverlay(td_s32 _iChn, td_s32 _iLayNo, td_s32 iColor, td_s32 iDiaphaneity, td_s32 iFontSize, td_s32 _iX, td_s32 _iY, td_u8* _cWord, td_s32 _blStore);
td_s32 td_cmd_getwordoverlay(td_s32 _iCh, td_s32* _piX, td_s32* _piY, td_s8* _pcText);
td_s32 td_cmd_getMultiwordoverlay(td_s32 _iCh, td_s32 _iLayNo, TChnTextOsdCnf *_ptParam);
//int td_cmd_setpppoeconfig(char* cUserName,char* cPassword,int iAutoDial);//Leeon Tonie add for compiling warnings
int td_cmd_setvideopara(const int chn,structVideoParam* _sp,int _iNum);//Leeon Tonie add for compiling warnings
int td_cmd_change_colortogray(td_s32 _iCh, td_s32 _blColorToGray);	//Leeon Tonie added for compiling warnings.
int td_cmd_getencoder_ability(int Chn);

int td_cmd_set_previewrec(int _iChn,int _iEnc);
int td_cmd_get_previewrec(int _iChn);

td_s32 td_cmd_set_previewmode(int _iQualityCnf);
td_s32 td_cmd_get_previewmode(void);

td_s32 td_cmd_set_videoencrypt(int _socket, td_s32 _iChn, td_s8* _cPassword,td_s32 _iType);
td_s32 td_cmd_get_videoencrypt(td_s32 _iChn, td_s8 _cPassword[ENCRYPT_PWD_LENGTH+1]);
int td_cmd_get_videoencrypt_ex(int _iChn);
td_s32 td_cmd_get_videoencrypt_passwd(td_s32 _iChn,char *_pcPassword);
td_s32 td_cmd_set_recorderpolicy(td_s32 _iMode,td_u32 _iChannelListByBits,td_s32 _iDeviceListByBits);
int td_cmd_get_recorderpolicy(int* _iMode,int* _iChannelListByBits,int _iDeviceListByBits[MAX_RECORDER_NUM]);
int td_cmd_startburn(int _iCdNo,td_s32 _iIsOneKey);
int td_cmd_stopburn(int _iCdNo);
td_s32 td_cmd_stopburnEx(td_s32 _iCdNo);
td_s32 td_cmd_startburnEx(td_s32 _iCdNo,td_s32 _iIsOneKey,td_s32 _iIsOverLap);
int td_cmd_force_stopburn(int _iCdNo);
int td_cmd_video_mark(int _iChn);
td_s32 td_cmd_common_mark(td_s32 _iChn, td_s32 _iType, td_char *_pcTag);
int td_cmd_set_video_header(int _iOverlayTime,int _iX,int _iY,char* _cBuf);
int td_cmd_overlay_video_header();
int td_cmd_set_audiovolume(int _iChn,int _iVolume);
int td_cmd_get_audiovolume(int _iChn);
int td_cmd_shortcut_burn();
td_s32 td_cmd_shortcut_burn_ex(int _iAct);

int td_cmd_set_H323LocalPara(int _iLocalGroupId,int _iListenPort,char* _pcLocalNo,char* _pcPassWord,int _iIdCodeType,int _iAnwTypeForCall,int _iMainSrcChnNo,int _iSubSrcChnNo,int _iMainDecChnNo,int _iSubDecChnNo);
/*
	函数名称:td_cmd_set_H323LocalPara
	功能: 设置本地参数
	参数: _iLocalGroupId
		  _iListenPort
	      _pcLocalNo
	      _pcPassWord
	      _iIdCodeType
	      _iAnwTypeForCall
	      _iMainSrcChnNo
	      _iSubSrcChnNo
	      _iMainDecChnNo
	      _iSubDecChnNo	      
	返回值: td_successs 成功 td_failure 失败	
*/
int td_cmd_get_H323LocalPara(int* _piLocalGroupId,int* _piListenPort,char* _pcLocalNo,char* _pcPassWord,int* _piIdCodeType,int* _piAnwTypeForCall,int* _piMainSrcChnNo,int* _piSubSrcChnNo,int* _piMainDecChnNo,int* _piSubDecChnNo);
/*
	函数名称:td_cmd_get_H323LocalPara
	功能: 获取本地参数
	参数: _piLocalGroupId
		  _piListenPort
	      _pcLocalNo
	      _pcPassWord
	      _piIdCodeType
	      _piAnwTypeForCall
	      _piMainSrcChnNo
	      _piSubSrcChnNo
	      _piMainDecChnNo
	      _piSubDecChnNo
	返回值: td_successs 成功 td_failure 失败	
*/
int td_cmd_set_H323GKPara(int _iGKGroupId,int _iGkType,char* _pcGkIP,int _iGkPort,char* _pcGkRegName,int _iEncType);
/*
	函数名称:td_cmd_set_H323GKPara
	功能: 设置网守参数
	参数: _iGKGroupId
		  _iGkType
	      _pcGkIP
	      _iGkPort
	      _pcGkRegName
	      _iEncType
	返回值: td_successs 成功 td_failure 失败	
*/
int td_cmd_get_H323GKPara(int* _piGKGroupId,int* _piGkType,char* _pcGkIP,int* _piGkPort,char* _pcGkRegName,int* _piEncType);
/*
	函数名称:td_cmd_get_H323GKPara
	功能: 获取网守参数
	参数: _piGKGroupId
		  _piGkType
	      _pcGkIP
	      _piGkPort
	      _pcGkRegName
	      _piEncType
	返回值: td_successs 成功 td_failure 失败	
*/

/*
	函数名称:td_cmd_manual_record
	功能: 开启或停止手动录像
	参数: _iChn,通道号 从0开始
	      _iStart: 
	       #define GUI_START_HUM_VIDEO		1			//启动手动录像
		   #define GUI_STOP_HUM_VIDEO		2			//停止手动录像
	返回值: td_successs 成功 td_failure 失败
	
*/
int td_cmd_manual_record(int _iChn, int _iStart);

/*
	函数名称:td_cmd_can_start
	参数:  _iOperaType 操作类型，
		#define ONEKEY_BURN         0  
		#define PLAYBACK            1 
	返回值: td_success 成功  操作可以开始
	 		其他 失败 返回错误码  操作不能进行
		 
*/
int td_cmd_can_start_ex(int _iOperaType,int _iChn);
/*
	函数名称:td_cmd_can_start
	参数:  _iOperaType 操作类型，
		#define ONEKEY_BURN         0  
		#define PLAYBACK            1 
	返回值: td_success 成功  操作可以开始
	 		其他 失败 返回错误码  操作不能进行
		 
*/
int td_cmd_can_start(int _iOperaType);

/*
	函数名称:td_cmd_draw_video_header 
	功能:绘制片头叠加信息到视频预览画面
	参数:_uVideoHeader 片头信息
	说明:供 Inquest.c调用
*/
int td_cmd_draw_video_header(td_u8 *_uVideoHeader);

/*
	函数名称: td_cmd_get_record_policy
	功能:获取刻录策略
	参数:eType，返回:刻录类型(单盘、双盘同刻、轮刻) 
		 _s32DiskNo:光盘号
	返回值: td_success 成功， td_failure 失败

*/
int td_cmd_get_record_policy(td_s32*eType, td_s32 _s32DiskNo);


td_s32 td_cmd_get_ex_cbl_mode(TGuiCblModeMsgEx *_ptMsg);
/*
	函数名称: td_cmd_get_ex_cbl_mode/
	功能:获取/ 设置扩展刻录配置(倍速、封盘、弹盘、时长、盘片)
	参数:_ptMsg，扩展配置信息
	返回值: td_success 成功， td_failure 失败

*/

td_s32 td_cmd_get_dvd_driver_state(td_s32 _iCdNo);
/*
	函数名称:  td_cmd_get_dvd_driver_state/
	功能:获取光驱状态
	参数:_iCdNo， 光盘号
	返回值: 光驱状态

*/
td_s32 td_cmd_set_ex_cbl_mode(TGuiCblModeMsgEx *_ptMsg);
/*
	函数名称: td_cmd_set_ex_cbl_mode/
	功能:获取/ 设置扩展刻录配置(倍速、封盘、弹盘、时长、盘片)
	参数:_ptMsg，扩展配置信息
	返回值: td_success 成功， td_failure 失败

*/
int td_cmd_get_ResSpace(int *_piSpace);
/*
	函数名称: td_cmd_get_ResSpace
	功能:获取光盘剩余空间
	参数:_piSpace
	返回值: td_success 成功， td_failure 失败

*/
int td_cmd_set_ResSpace(int _iSpace);
/*
	函数名称: td_cmd_set_ResSpace
	功能:设置光盘剩余空间
	参数:_iSpace
	返回值: td_success 成功， td_failure 失败

*/
int td_cmd_direct_ConfigVideoPara(int _iCdNo);
/*
	函数名称: td_cmd_direct_ConfigVideoPara
	功能:设置直刻参数(根据时长、盘片类型)
	返回值: td_success 成功，其他 失败

*/

td_s32 td_cmd_config_video_para(td_s32 _iCdNo);
/*
	函数名称: td_cmd_config_video_para
	功能:设置刻录参数(根据时长、盘片类型)
	返回值: td_success 成功，其他 失败

*/
td_s32 td_cmd_force_eject(int _iMode,int _iCDNo);


//zhangyi modify 20120215
//int td_cmd_set_vga_mode(td_s32 _s32VgaMode);
td_s32 td_cmd_set_vga_mode(td_s32 _s32VoDev, td_s32 _s32VoDevMode);

int td_cmd_set_bnc_mode(td_s32 _iBncId, td_s32 _iBncMode);

unsigned int td_cmd_getipaddr(void); 		//陶永亮 091117 获取32位整数形式的IP地址

td_s32 permillage(td_s32 _iType, td_s32 _iChn, td_u16 *_piPx, td_u16 *_piPy);
//td_s32 td_cmd_CreateFtpDirectory();
//int td_cmd_assistant_init(void);
//td_s32 td_cmd_IsTplayerExits();


#if (PLATFORM == HI3520 || PLATFORM == HI3520SC)
#define SLAVE_DOWN_TIMEOUT   40 
void td_heartbeat_with_slave(unsigned long para);
#endif

//视频 参数改变通知
typedef void (*tdMediaCmd_StatChgNotifyCallBack)(int _iChn, int _iCmd);

td_s32 tdMediaCmd_RegStatNotify(tdMediaCmd_StatChgNotifyCallBack _CallBack);



td_s32 td_cmd_GetAlias(TAlias* _pstAlias);
/*
 * 函数名称：GetAlias
 * 简要描述：获取别名配置
 * 输	  入：
 * 			_pstAlias, 非空，别名配置
 * 输	 出：
 * 			ERR_SYS_SUCCESSFUL,成功
 *			ERR_SYS_WRONG_PARAM,错误的参数
 *			ERR_SYS_FAILED, 其他原因导致失败
 * 修改日志：
 * 			2009-12-22,陶永亮,创建
 */
 
td_s32 td_cmd_GetAliasVer(td_s32 _iType);
/*
 * 函数名称：GetAliasVer
 * 简要描述：获取别名版本
 * 输	  入：
 * 			_iType, ALIAS_CHANNEL~ALIAS_ALARM_OUTPUT, 别名类型
 * 输	 出：
 * 			>= 0,别名版本
 *			其他，失败
 * 修改日志：
 * 			2009-12-22,陶永亮,创建
 */
td_s32 td_cmd_SetDevCommonAlias(td_s32 _iType, td_s32 _iNo, td_char* _pcAlias); 
td_s32 td_cmd_SetAlias(td_s32 _iType, td_s32 _iNo, td_u8* _pcAlias);
/*
 * 函数名称：SetAlias
 * 简要描述：设置别名
 * 输	  入：
 * 			_iType, ALIAS_CHANNEL~ALIAS_ALARM_OUTPUT, 别名类型
 *			_iNo, 取值范围由别名类型确定，通道/端口号
 *			_pcAlias, 非空，别名
 * 输	 出：
 * 			ERR_SYS_SUCCESSFUL,成功
 *			ERR_SYS_WRONG_PARAM,错误的参数
 *			ERR_SYS_FAILED, 其他原因导致失败
 * 修改日志：
 * 			2009-12-22,陶永亮,创建
 */



// 恢复出厂设置
// _iFlg : 0远程,1本地
td_s32 td_cmd_ToDefaultCnf(td_s32 _iFlg);
//设置光盘备份状态
td_s32 td_cmd_BkBurn_SetBkState(td_s32 _iDvdNo,td_s32 _iBkState);
td_s32 td_cmd_cleanPathName(void);
/*
 * 函数名称：td_cmd_cleanPathName
 * 简要描述：清除路径信息
 *  
 * 返回值  : >=0 成功   -1 失败				 
 *			
 *			
 * 
 * 			
 *			
 *			
 * 修改日志：
 * 			2014-8-12,zmw,创建
 */
td_s32 td_cmd_getPathName(td_char *_pcPathName);
/*
 * 函数名称：td_cmd_getPathName
 * 简要描述：获取内核路径，用于创建目录
 * 输出:    _pcPathName	：路径信息。
 *
 * 返回值  : >=0 成功   -1 失败				 
 *			
 *			
 * 
 * 			
 *			
 *			
 * 修改日志：
 * 			2014-8-12,zmw,创建
 */
td_s32 td_cmd_setPathName(const td_char *_pcPathName);
/*
 * 函数名称：td_cmd_setPathName
 * 简要描述：UK通知内核路径
 * 输	  入：  _pcPathName	：路径信息。
 *
 * 返回值  :  	>=0 成功   -1 失败				 
 *			
 *			
 * 
 * 			
 *			
 *			
 * 修改日志：
 * 			2014-8-12,zmw,创建
 */
 td_s32 td_cmd_setBackupIndex(td_s32 _iBackupIndex,td_s32 _iBackupCnt);
 /*
 * 函数名称：td_cmd_setBackupIndex
 * 简要描述：UK通知内核当前备份第几个时间段的录像
 * 输	  入：  _iBackupIndex	：时间段索引号。
 				_iBackupCnt     : 时间段个数
 				
 *
 * 返回值  :  	>=0 成功   -1 失败				 
 *			
 *			
 * 
 * 			
 *			
 *			
 * 修改日志：
 * 			2014-8-12,zmw,创建
 */

 td_s32 td_cmd_getBackupIndex(td_s32* _iBackupIndex,td_s32* _iBackupCnt);
  /*
 * 函数名称：td_cmd_getBackupIndex
 * 简要描述：获取当前备份第几个时间段的录像
 * 输	  出：  _iBackupIndex	：时间段索引号。
 *				_iBackupCnt     :光盘备份时间段个数。
 * 返回值  :  	>=0 成功   -1 失败				 
 *			
 *			
 * 
 * 			
 *			
 *			
 * 修改日志：
 * 			2014-8-12,zmw,创建
 */
td_s32 td_cmd_RtBurn_DvdEject(td_s32 _iOption,td_s32 _iDvdNo,td_s32 _iForce);
/*
  * 函数名称:td_cmd_RtBurn_DvdEject
  * 简要描述:弹出/收回光驱
  * 输	  入:
  *			 _iDvdNo, 刻录机编号
  *			 _iOption,（1 收回，0 弹出）
  *          _iForce, 是否强制弹盘
  *  输	  出   ERR_REC_SUCCESSFUL，成功
  *				 ERR_REC_FAILED ,    失败
 */
td_s32 td_cmd_Burn_CheckDiskCanUsed(td_s32 _iDvdNo,td_s32 _iUsedLimit);
/*
 * 函数名称：td_cmd_Burn_CheckDiskCanUsed
 * 简要描述：判断光盘的可用空间是否小于限定值
 * 输	  入：   _iDvdNo   光盘号
 * 				  _iUsedLimit  容量限定值
 *			
 *			
 * 输	 出    ERR_REC_SUCCESSFUL，成功
 				 ERR_REC_HUGE_SIZE_TO_BURN  失败
 * 			
 *			
 *			
 * 修改日志：
 * 			2014-3-12,zmw,创建
 */
 td_s32 td_cmd_BigFile_CheckDiskCanUsed(td_s32 _iDvdNo);
/*
 * 函数名称：td_cmd_BigFile_CheckDiskCanUsed
 * 简要描述：刻录udf格式的光盘,判断是否是空白光盘
 * 输	  入：   _iDvdNo   光盘号
 * 				 
 *			
 *			
 * 输	 出    ERR_REC_SUCCESSFUL，成功
 				 ERR_REC_HUGE_SIZE_TO_BURN  失败
 * 			
 *			
 *			
 * 修改日志：
 * 			2014-3-12,zmw,创建
 */
 td_s32 td_cmd_BkBurn_PrepareToRecord(td_s32 _iCdNo,td_s32 _iType);
 /*
 * 函数名称：td_cmd_BkBurn_PrepareToRecord
 * 简要描述：检查当前的磁盘是否可用，否则无法生成录像文件
 * 输	  入：   _iDvdNo   光盘号
 * 				
 *			
 *			
 * 输	 出    ERR_REC_SUCCESSFUL，成功
 				 ERR_REC_FAILED         失败
 * 			
 *			
 *			
 * 修改日志：
 * 			2014-3-12,zmw,创建
 */

td_s32 td_cmd_BkBurn_AddFileToConvertList(td_u8* _pcFileName);
/*
 * 函数名称：td_cmd_BkBurn_AddFileToConvertList
 * 简要描述：添加转换文件列表
 * 输	  入：    _pcFileName    录像文件名
 *			
 *			
 * 输	 出   td_success，成功
 				  td_failure         失败
 * 			
 *			
 *			
 * 修改日志：
 * 			2014-11-27,zmw,创建
 */
td_s32 td_cmd_BkBurn_AddFileToBackUpList(td_u8* _pcFileName,td_s32 _iCdNum);
/*
 * 函数名称：td_cmd_BkBurn_AddFileToBackUpList
 * 简要描述：添加备份列表
 * 输	  入：   _iDvdNo   光盘号
 * 				 _pcFileName    录像文件名
 *			
 *			
 * 输	 出   td_success，成功
 				  td_failure         失败
 * 			
 *			
 *			
 * 修改日志：
 * 			2014-3-12,zmw,创建
 */
td_s32 td_cmd_BkBurn_DoRecord(td_s32 _iCdNo);
/*
 * 函数名称：td_cmd_BkBurn_DoRecord
 * 简要描述：执行备份
 * 输	  入：   _iDvdNo   光盘号
 * 				
 *			
 *			
 * 输	 出   td_success，成功
 				  td_failure         失败
 * 			
 *			
 *			
 * 修改日志：
 * 			2014-3-12,zmw,创建
 */
td_s32 td_cmd_BkBurn_GetBurnProgress(td_s32* _piStat,td_s32 _iCdNum);
/*
 * 函数名称：td_cmd_BkBurn_GetBurnProgress
 * 简要描述： 获取进度与状态
 * 输	  入：   _iDvdNo   光盘号
 * 				  _piStat	   备份状态指针
 *			
 *			
 * 输	 出    备份进度
 				
 * 			
 *			
 *			
 * 修改日志：
 * 			2014-3-12,zmw,创建
 */
td_s32 td_cmd_BkBurn_ForceStopBackUp(td_s32 _iCdNum);
/*
 * 函数名称：td_cmd_BkBurn_ForceStopBackUp
 * 简要描述： 强制停止备份
 * 输	  入：   _iDvdNo   光盘号
 * 				
 *			
 *			
 * 输	 出    td_success，成功
   			        td_failure         失败
 				
 * 			
 *			
 *			
 * 修改日志：
 * 			2014-3-12,zmw,创建
 */


td_s32 td_cmd_StartSearchIpc(td_s32 _iSearchMod);//下命令查询数字通道设备，传入查询模式
td_s32 td_cmd_GetIpcSearchMsg(TIpcDevItem *_ptItem);//返回UI 数字设备信息，一次传出一个设备的信息，返回值为1 -有设备0 -无设备 -1 -搜索结束
td_s32 td_cmd_EnableVideoParamTimer(td_s32 _blEnable);

// 返回值:0数字通道,1本地通道,2虚拟通道
td_s32 td_cmd_get_ChnType(td_s32 _iChn, td_s32 *_iType);//获取通道类型


td_s32 td_cmd_get_LocalChnAttr(td_s32 _iChn, td_s32 *_iBncInput); //获取本地通道属性
td_s32 td_cmd_set_LocalChnAttr(td_s32 _iChn, td_s32 _iBncInput);
td_s32 td_cmd_get_VcChnAttr(td_s32 _iChn, td_s32 *_iVoDev); //获取虚拟通道属性
td_s32 td_cmd_set_VcChnAttr(td_s32 _iChn, td_s32 _iVodev);

td_s32 td_cmd_set_ParsePara(strParsePara *_ptParam);

// 获取通道可编辑类型,返回-1表失败
// 可更改类型按位表示，0,1,2位分别对应数、模、VC
// 0表示不可更改，1表示可更改成此类型
td_s32 td_cmd_get_ChnTypeEditAbleFlg(td_s32 _iChn);
// 更改通道类型,0,1,2分别对应数、模、VC
td_s32 td_cmd_set_ChnType(td_s32 _iChn, td_s32 _iChnType, td_s32 _iParam);
//网络方式实现切换通道类型shihc add 20130327
td_s32 td_cmd_set_ChnType_Net(td_s32 _iChn, td_s32 _iChnNetType);


/*
_iChn : 通道号 
_iEnable : 使能 0-禁用该数字通道；1-启用该数字通道，默认为启用
_iConnectMode : 0-IP；1-域名；2-主动模式，默认为IP
_strIp : _iConnectMode=0时为IP地址；_iConnectMode=1时为域名；_iConnectMode=2时为设备ID，默认为空，至多32个字符
_strProxy : 代理IP , iConnectMode=2时，该字段无效，默认为空，至多32个字符
_iChnNo : 前端设备通道号, 取值范围已前端设备能力而定，默认为0
_uiPort : 取值81～65535，默认为3000
_iStreamType : 0-主码流；1-副码流，默认为主码流
_iNetMode : 1-TCP；2-UDP；3-组播，暂定为TCP,不可更改
_strName : 登录前端设备的用户名，不可为空，至多16个字符
_strPsw  : 登录前端设备的密码，不可为空，至多16个字符
_strDecrypt : 前端设备视频加密的密码，为空则表示不加密，至多16个字符
*/
td_s32 td_cmd_get_IpcChnAttr(td_s32 _iChn, td_s32 *_iEnable, td_s32 *_iConnectMode, char *_strIp,
		char *_strProxy, td_s32 *_iChnNo, td_u32 *_uiPort, td_s32 *_iStreamType, td_s32 *_iNetMode, char *_strName,
		char *_strPsw, char *_strDecrypt);

td_s32 td_cmd_set_IpcChnAttr(td_s32 _iChn, td_s32 _iEnable, td_s32 _iConnectMode, const char *_strIp,
		const char *_strProxy, td_s32 _iChnNo, td_u32 _uiPort, td_s32 _iStreamType, td_s32 _iNetMode, const char *_strName,
		const char *_strPsw, const char *_strDecrypt, td_s32 _iServerType, const char *_strRtspUrl);


td_s32 td_cmd_setlogodisplay(td_s32 _iChn, td_s32 _iXp, td_s32 _iYp, td_s32 _iDsp);

// 设置外挂模块(_strAppName不包含".bin"，为NULL时表式不启动任何外挂)
td_s32 td_cmd_set_appname(char *_strAppName);
// 获取当前激活的外挂
td_s32 td_cmd_get_appname(char *_strAppName);
// 获取系统当前可用的外挂列表,返回值为个数
td_s32 td_cmd_get_applist(char _strAppList[][64], int _iMaxCnt);

td_s32 td_cmd_getappserverlist(char _strAppSrvList[][64]);
td_s32 td_cmd_setappserverpara(char *_para);
td_s32 td_cmd_getappnettype(char *_strAppType);

td_s32 td_cmd_GetRtmpInfo(td_s32 _iNo, TGuiRtmpInfoItem *_ptInfoItem);
td_s32 td_cmd_SetRtmpInfo(const TGuiRtmpInfoItem *_ptInfoItem);

td_s32 td_cmd_GetRtspInfo(td_s32 _iNo, TRtspInfoItem *_ptInfoItem);
td_s32 td_cmd_SetRtspInfo(TRtspInfoItem *_ptInfoItem);

td_s32 td_cmd_set_channle_venc_stat(int _iChn, int _piStat);
td_s32 td_cmd_get_channle_venc_stat(int _iChn);
int td_cmd_set_channle_stat(int _iChn, int _istat);
void TdGetIpcChnPara(TDigChnPamaMsg *_tChnMsg, int _iChn);
//add by zhuangzhibing 20120129 
int td_cmd_StartOverLapBurn();
//add end
td_s32 td_cmd_can_set_videoencrypt();//判断是否可以进行加密设置
void Public__EncryptString(const char* _pstrSrc,char * _pstrDst);//光盘加密算法


// 获取U 盘分区信息列表20120220 杨珏
td_s32 td_cmd_GetUsbPartMsg(td_s32 _iDiskId, td_s32 _iPartId, td_s32* _piWriteProtect, 
	td_s8* _pcDiskName, td_u64* _pullSize, td_u64* _pullUsed, td_u64* _pullFree);

//add by nsg 20120207  智能分析设置函数
td_s32 td_cmd_get_VcaChnPolicy(td_s32 _iChn);
td_s32 td_cmd_get_VcaCfgChn(const td_s32 _iChn, td_s32 *_piEnable);
td_s32 td_cmd_set_VcaCfgChn(const td_s32 _iChn, const td_s32  _iEnable);
//---------------------------------------------------------------------------------------------------------------------------------------
//td_s32 td_cmd_get_VcaCfgVideo(const td_s32 _iChn, td_s32 *_piVideoSize);
//td_s32 td_cmd_set_VcaCfgVideo(const td_s32 _iChn, const td_s32  _iVideoSize);
//---------------------------------------------------------------------------------------------------------------------------------------
td_s32 td_cmd_get_VcaCfgAdv(const td_s32 _iChn,  TVcaAdvanceParam  *_pstAdvancedParam);
td_s32 td_cmd_set_VcaCfgAdv(const td_s32 _iChn, const TVcaAdvanceParam const *_pstAdvancedParam);
//---------------------------------------------------------------------------------------------------------------------------------------
td_s32 td_cmd_get_VcaCfgFde(const td_s32 _iChn, vca_TFdepthParam_ex *_pstFdepthParam);
td_s32 td_cmd_set_VcaCfgFde(const td_s32 _iChn, const vca_TFdepthParam_ex const *_pstFdepthParam);
//---------------------------------------------------------------------------------------------------------------------------------------
td_s32 td_cmd_get_VcaCfgTarget(const td_s32 _iChn, 		 TVcaConfig 	   *_pstConfig);
td_s32 td_cmd_set_VcaCfgTarget(const td_s32 _iChn, const TVcaConfig const *_pstConfig);
//---------------------------------------------------------------------------------------------------------------------------------------
td_s32 td_cmd_get_VcaRuleSet(const td_s32 _iChn, const td_s32 _iRuleId, char* 		 _pcRuleName, 		td_s32* _piValid);
td_s32 td_cmd_set_VcaRuleSet(const td_s32 _iChn, const td_s32 _iRuleId,  char*  _pcRuleName, const td_s32  _iValid);
//---------------------------------------------------------------------------------------------------------------------------------------
td_s32 td_cmd_get_VcaRuleEvent0(const td_s32 _iChn, const td_s32 _iRuleId,		 TVcaParamTripwire		*_pstVcaTripParam, const td_s32 _iType);
td_s32 td_cmd_set_VcaRuleEvent0(const td_s32 _iChn,	const td_s32 _iRuleId, const TVcaParamTripwire const *_pstVcaTripParam, const td_s32 _iType);
//---------------------------------------------------------------------------------------------------------------------------------------
td_s32 td_cmd_get_VcaRuleEvent2(const td_s32 _iChn, const td_s32 _iRuleId,		 TVcaParamPerimeter 		 *_pstVcaPeriParam, const td_s32 _iType);
td_s32 td_cmd_set_VcaRuleEvent2(const td_s32 _iChn, const td_s32 _iRuleId, const TVcaParamPerimeter const *_pstVcaPeriParam, const td_s32 _iType);
//---------------------------------------------------------------------------------------------------------------------------------------
td_s32 td_cmd_get_VcaRuleEvent7(const td_s32 _iChn,	const td_s32 _iRuleId, 		 TVcaParamOSC  		*_pstVcaOscParam, const td_s32 _iType);
td_s32 td_cmd_set_VcaRuleEvent7(const td_s32 _iChn,	const td_s32 _iRuleId, const TVcaParamOSC const 	*_pstVcaOscParam, const td_s32 _iType);
//---------------------------------------------------------------------------------------------------------------------------------------
td_s32 td_cmd_get_VcaRuleEvent9(const td_s32 _iChn, const td_s32 _iRuleId, 		 TVcaFaceParam_ex  	 *_pstVcaFaceParam, const td_s32 _iType);
td_s32 td_cmd_set_VcaRuleEvent9(const td_s32 _iChn, const td_s32 _iRuleId, const TVcaFaceParam_ex const *_pstVcaFaceParam, const td_s32 _iType);
//---------------------------------------------------------------------------------------------------------------------------------------
td_s32 td_cmd_get_VcaRuleEvent10(const td_s32 _iChn, const td_s32 _iRuleId,		 TVcaParamAVD		 *_pstVcaDiagParam, const td_s32 _iType);
td_s32 td_cmd_set_VcaRuleEvent10(const td_s32 _iChn, const td_s32 _iRuleId,const TVcaParamAVD const *_pstVcaDiagParam, const td_s32 _iType);
//---------------------------------------------------------------------------------------------------------------------------------------
//caizhaoxu 20130117
td_s32 td_cmd_get_VcaRuleEvent11(const td_s32 _iChn, const td_s32 _iRuleId,TVcaParamCAP	*_pstVcaCapParam,const td_s32 _iType);
td_s32 td_cmd_set_VcaRuleEvent11(const td_s32 _iChn, const td_s32 _iRuleId,const TVcaParamCAP const *_pstVcaCapParam,const td_s32 _iType);
//---------------------------------------------------------------------------------------------------------------------------------------
//shihc add 20130323
td_s32 td_cmd_get_VcaRuleEvent12(const td_s32 _iChn, const td_s32 _iRuleId,TVcaParamCPC	*_pstVcaCpcParam,const td_s32 _iType);
td_s32 td_cmd_set_VcaRuleEvent12(const td_s32 _iChn, const td_s32 _iRuleId,const TVcaParamCPC const *_pstVcaCpcParam,const td_s32 _iType);
//---------------------------------------------------------------------------------------------------------------------------------------//caizhaoxu 20130117
td_s32 td_cmd_get_VcaRuleEvent13(const td_s32 _iChn, const td_s32 _iRuleId,TVcaParamCDD	*_pstVcaCddParam,const td_s32 _iType);
td_s32 td_cmd_set_VcaRuleEvent13(const td_s32 _iChn, const td_s32 _iRuleId,const TVcaParamCDD const *_pstVcaCddParam,const td_s32 _iType);
//---------------------------------------------------------------------------------------------------------------------------------------
td_s32 td_cmd_set_VcaRuleEvent(td_s32 _iChn,td_s32 _iRuleId,void* _pstVcaEventParam,td_s32 _iVcaType);
td_s32 td_cmd_get_VcaRuleEvent(td_s32 _iChn,td_s32 _iRuleId,void* _pstVcaEventParam,td_s32 _iVcaType);




td_s32 td_cmd_get_VcaRuleEnableType(const td_s32 _iChn,const td_s32 _iRuleId,TVcaRuleEnableStat* _pEnableStat);

td_s32 td_cmd_setVcaReboot(td_s32 _iChn);
td_s32 td_cmd_setVcaAlarmStatClr(const td_s32 _iChn, const td_s32 _iRuleId, const TVcaEventType _eEventType);


//end


//add by nsg 20120131 
//获取合成通道混音参数设置
 td_s32 td_cmd_get_mixaudio(int *_piEnable, int *_piMixAudioChn, char *_pcAudioChnParam);
//设置合成通道混音参数
td_s32 td_cmd_set_mixaudio(int _iAudioChnCnt, int _iMixAudiochn, char *_pcAudioChnParam);
//end add

//lc 121012;设置/ 获取静音检测阈值;
td_s32 td_cmd_get_mute_bounds(td_s32 _iChn);
td_s32 td_cmd_set_mute_bounds(td_s32 _iChn,td_s32 _iBounds);
//

//add by nsg 120905
//获取ATM参数配置
int td_cmd_get_CnfAtm(TAtmPara *_pstAtmPara);
//设置ATM参数配置
int td_cmd_set_CnfAtm(TAtmPara _stAtmPara);
//设置图像抓拍参数
int td_cmd_set_SnapShotPara(td_u8 _u8Channel, TSnapShot _stSnapShotPara);
//获取图像抓拍参数
int td_cmd_get_SnapShotPara(td_u8 _u8Channel, TSnapShot *_pstSnapShotPara);
//设置卡号叠加参数
int td_cmd_set_AtmCardOverlay(int iOverlayChn, STR_WordOverlay _stWordOverlay);
//获取卡号叠加参数
int td_cmd_get_AtmCardOverlay(int iOverlayChn, STR_WordOverlay *_pstWordOverlay);
//设置图片抓拍联动参数
int td_cmd_set_SnapShotLinkPara(td_u8 _u8Channel, int _iMode);
//获取图片联动抓拍参数
int td_cmd_get_SnapShotLinkPara(td_u8 _u8Channel, int *_piMode );


//end add
//td_s32 td_cmd_CreateOtherFileFtpDirectory();
td_s32 td_cmd_CreateFtpDirectory();

td_s32 td_cmd_CreateBaseFtpDirectory();
td_s32 td_cmd_CreateFtpDirectoryAndLink();


//add by gyn 20120904
//获取画面合成效果
//td_s32 td_cmd_get_multipic(td_s32* _piPicCnt, char* _pcChnParam);
//设置画面合成效果
td_s32 td_cmd_set_multipic(int _iPicCnt, char* _cChannelParam,int _iVoDevId, char *_cPara);

//获取合成画面效果
td_s32 td_cmd_get_multipic(td_s32* _piPicCnt, char* _pcChnParam, int _iVoDevId);

//add end
//获取自定义画面坐标
td_s32 td_cmd_get_picPoint(td_s32 _iVoDevId,TRect *_ptRects);



//设置自定义画面分割方式 
td_s32 td_cmd_set_freevo(td_s32 _iVodev, td_s32 _iPicCnt, TRect _ptRect[],td_s32 *_piChnList);
//设置Vc和VGA/HDMI1或HDMI2输出同源
td_s32 td_cmd_set_vcaffinal(td_s32 _iVcId,td_s32 _iVodev);
td_s32 td_cmd_get_vcaffinal(td_s32 _iVcId);


td_s32 td_cmd_set_inquest_header(TGuiGetVideoHeaderOsdMsgRsp *_pstVideoHeader);

td_s32 td_cmd_get_inquest_header(TGuiGetVideoHeaderOsdMsgRsp *_pstVideoHeader);

td_s32 tdCmdGetFileTime(char *_strName, td_s32 *_iBeginTm, td_s32 *_iEndTm);

//从网络设置片头信息
td_s32 td_cmd_inquest_set_video_header(td_s32 _iOverlayTime,
td_s32 _iX,td_s32 _iY,char* _cBuf);
td_s32 td_cmd_setwordoverlayforatm(td_s32 _iChn, td_s32 _iPos,td_u8* _cText);

//-
int td_cmd_direct_RecoveryConfigIni();
//无硬盘断电直刻修复还原config.ini
int td_cmd_direct_RecoveryVideoPara();
//无硬盘断电直刻修复还原默认参数设置--
int td_cmd_direct_ConfigVideoPara_to_Default();
//无硬盘直刻还原默认参数设置-----------------------------------------------------------------------------------
td_s32 td_cmd_config_video_para_to_default(td_s32 _iCdNo);

td_s32 td_cmd_set_VcTime(td_s32 _isleepTime);
td_s32 td_cmd_get_VcTime();

//Sherry add 20130111 获取音频采样率
td_s32 td_cmd_getaudioSampleRate(const td_s32 _iChn);

td_s32 td_cmd_StartAppEncode(int _iChannelNo);
//shihc  add 20130312 增加数字通道连接状态获取
td_s32 td_cmd_GetIpChanConnect(td_s32 _iChannelNo);
td_s32 td_cmd_SavelogoPic(td_u8* _u8Buf,td_s32 _iLength);
//shihc  add 20130318 增加实时读取网卡流量
td_s32 td_cmd_GetNetWorkState(td_s32 _ilannum,td_char *_pcState,td_s32 _itimers);
void td_cmd_NetWorkState_Timers(unsigned long _param);
//shihc add 20130320 增加模块使能的通用函数
td_s32 td_cmd_common_setserviceable(td_s32 _iServiceID,td_s32 _iChannelno,td_s32 _iable);
td_s32 td_cmd_common_getserviceable(td_s32 _iServiceID,td_s32 _iChannelno,td_s32 *_pGetAble);

td_s32 td_cmd_snmpable(td_s32 _isetget,td_s32 _iable, td_s32 *_pGetAble);


//20130314 SUN Wei add for 7.6 smtp
td_s32 td_cmd_get_smtppara(TSmtpPara *_pstSmtpPara);
td_s32 td_cmd_set_smtppara(TSmtpPara _stSmtpPara);
//20130314 SUN Wei add for 7.6 smtp end

td_s32 td_cmd_setsensorspin (const td_s32 _iChn, TSensorSpin _eType);
td_s32 td_cmd_getsensorspin (td_s32 _iChn);




/************************************云镜控制***************************/

/*
 * 函数名称：td_cmd_ManualPtzControl
 * 简要描述: 手动控制ptz
 * 输	  入：
 * 			_pstData, UI发送过来的ptz参数
 *			_iChannelNo,通道号
 *			_iCmdCode,UI发送过来的控制码值
 * 输	  出：
 * 			ERR_PTZ_WRONG_PARAM,错误的参数
 *			ERR_PTZ_SUCCESSFUL,操作成功
 *			ERR_PTZ_FAILED,操作失败
 *			
 * 修改日志：
 * 			2013-02-20 dce move from ukcmd.c
 */
td_s32 td_cmd_ManualPtzControl(td_s32 _iChannelNo, td_s32 _iCmdCode, PTZ_DATA *_pstData);

/*
 * 函数名称：td_cmd_GuiPtzRemote
 * 简要描述: 进行来自远程(如网络键盘)的PTZ操作
 * 输	  入：
 *			_iChannelNo,通道号
 *			_iCmdCode,控制码值
 * 输	  出：
 * 			ERR_PTZ_WRONG_PARAM,错误的参数
 *			ERR_PTZ_SUCCESSFUL,操作成功
 *			ERR_PTZ_FAILED,操作失败
 *			
 * 修改日志：
 * 			2013-02-20 dce move from ukcmd.c
 */
td_s32 td_cmd_GuiPtzRemote(td_s32 _iChannelNo, td_s32 _iCmdCode);

/*
 * 函数名称：td_cmd_GuiPtzTrackStart
 * 简要描述: 开始录制轨迹
 * 输	  入：
 *			_iChannelNo,通道号
 * 输	  出：
 * 			ERR_PTZ_WRONG_PARAM,错误的参数
 *			
 * 修改日志：
 * 			2013-02-20 dce move from ukcmd.c
 */
td_s32 td_cmd_GuiPtzTrackStart(td_s32 _iChannelNo);

/*
 * 函数名称：td_cmd_GuiPtzTrackEnd
 * 简要描述: 停止录制轨迹
 * 输	  入：
 *			_iChannelNo,通道号
 * 输	  出：
 * 			ERR_PTZ_WRONG_PARAM,错误的参数
 *			
 * 修改日志：
 * 			2013-02-20 dce move from ukcmd.c
 */
td_s32 td_cmd_GuiPtzTrackEnd(td_s32 _iChannelNo);

/*
 * 函数名称：td_cmd_GuiPtzTrackPlayStart
 * 简要描述: 演示轨迹
 * 输	  入：
 *			_iChannelNo,通道号
 * 输	  出：
 * 			ERR_PTZ_WRONG_PARAM,错误的参数
 *			
 * 修改日志：
 * 			2013-02-20 dce move from ukcmd.c
 */
td_s32 td_cmd_PtzTrackPlayStart(td_s32 _iChannelNo);

/*
 * 函数名称：td_cmd_PtzTrackPlayEnd
 * 简要描述: 停止播放轨迹
 * 输	  入：
 *			_iChannelNo,通道号
 * 输	  出：
 * 			ERR_PTZ_WRONG_PARAM,错误的参数
 *			
 * 修改日志：
 * 			2013-02-20 dce move from ukcmd.c
 */
td_s32 td_cmd_PtzTrackPlayEnd(td_s32 _iChannelNo);

/*
 * 函数名称：td_cmd_GetPtzCruise
 * 简要描述: 获取巡航轨迹参数
 * 输	  入：
 *			_iChannelNo,通道号
 *			_iCruiseNo,巡航轨迹号
 *			_pstCruise,巡航轨迹参数
 * 输	  出：
 * 			ERR_PTZ_WRONG_PARAM,错误的参数
 *			ERR_PTZ_SUCCESSFUL,操作成功
 *			
 * 修改日志：
 * 			2013-02-20 dce move from ukcmd.c
 */
td_s32 td_cmd_GuiGetPtzCruise(td_s32 _iChannelNo, td_s32 _iCruiseNo, TCruise *_pstCruise);

/*
 * 函数名称：td_cmd_GuiSetPtzCruise
 * 简要描述: 设置巡航轨迹参数
 * 输	  入：
 *			_iChannelNo,通道号
 *			_iCruiseNo,巡航轨迹号
 *			_pstCruise,巡航轨迹参数
 * 输	  出：
 * 			ERR_PTZ_WRONG_PARAM,错误的参数
 *			ERR_PTZ_SUCCESSFUL,操作成功
 *			ERR_PTZ_FAILED,操作失败
 * 修改日志：
 * 			2013-02-20 dce move from ukcmd.c
 */
td_s32 td_cmd_GuiSetPtzCruise(td_s32 _iChannelNo, td_s32 _iCruiseNo, TCruise *_pstCruise);

/*
 * 函数名称：td_cmd_GuiSetPtzCruise
 * 简要描述: 设置巡航轨迹参数
 * 输	  入：
 *			_iChannelNo,通道号
 *			_piParam,3D定位参数，整型数组，依此为鼠标位置x/鼠标位置y/屏幕宽/屏幕高/
 *			区域宽(可正负，负表反方向, 0表点)/区域高(可正负，负表反方向, 0表点)
 * 输	  出：
 * 			ERR_PTZ_WRONG_PARAM,错误的参数
 * 修改日志：
 * 			2013-02-20 dce move from ukcmd.c
 */
td_s32 td_cmd_Gui3DPosition(td_s32 _iChannelNo, td_s32 *_piParam);

/*
 * 函数名称：td_cmd_GuiGetPtzConfig
 * 简要描述: 获取ptz配置
 * 输	  入：
 *			_iChannelNo,通道号
 *			_pstPtzCnf,PTZ配置信息
 *			_pstCom,串口属性
 * 输	  出：
 * 			ERR_PTZ_WRONG_PARAM,错误的参数
 *			ERR_PTZ_FAILED,操作失败
 *			ERR_PTZ_SUCCESSFUL,操作成功
 * 修改日志：
 * 			2013-02-20 dce move from ukcmd.c
 */
td_s32 td_cmd_GuiGetPtzConfig(td_s32 _iChannelNo, TGuiPtzDecodeCnf *_pstPtzCnf, TCom *_pstCom);

/*
 * 函数名称：td_cmd_SetChannelPTZ
 * 简要描述: 设置某通道的PTZ属性
 * 输	  入：
 * 			_iChannelNo, 通道号
 *			_iComNo, 设置该通道用于PTZ控制的端口,0~MAX_SERIAL_NUM
 *			_cProtocolName,该通道用于PTZ控制的协议类型名称,非空
 *			_iAddress,该通道所接设备的PTZ地址
 * 输	  出：
 * 			ERR_PTZ_WRONG_PARAM,错误的参数
 * 修改日志：
 * 			2013-02-20 dce move from ukcmd.c
 */
td_s32 td_cmd_SetChannelPTZ(td_s32 _iChannelNo, td_s32 _iComNo, td_u8 *_pcProtocolName, td_s32 _iAddress);

/*
 * 函数名称：td_cmd_SetChannelComAttr
 * 简要描述: 设置通道PTZ的串口属性
 * 输	  入：
 * 			_iChannelNo, 通道号
 *			_pstCom, 串口属性，非空
 *			_blEnable, 是否自定义串口属性，0或1
 * 输	  出：
 *			ERR_PTZ_WRONG_PARAM, 错误的参数
 * 修改日志：
 * 			2013-02-20 dce move from ukcmd.c
 */
td_s32 td_cmd_SetChannelComAttr(td_s32 _iChannelNo, TCom* _pstCom, td_s32 _blEnable);

/*
 * 函数名称：td_cmd_DealProtocol
 * 简要描述: 处理ptz协议(添加或删除)
 * 输	  入：
 * 			_cProtocol, 协议名，长度最长为MAX_PROTOCOL_NAME_LEN
 *			_iDealCode, 协议处理码，ADD_PTZ_PROTOCOL(添加)、DEL_PTZ_PROTOCOL(删除)
 * 输	  出：
 *			ERR_PTZ_WRONG_PARAM, 错误的参数
 * 修改日志：
 * 			2013-02-20 dce creat
 */
td_s32 td_cmd_DealProtocol(td_u8 _cProtocol[MAX_PROTOCOL_NAME_LEN], td_s32 _iDealCode);

/*
 * 函数名称：td_cmd_GetProtocol
 * 简要描述: 获取ptz协议(待添加或已添加)
 * 输	  入：
 * 			_cList, 协议列表
 *			_iProtocolAttr, 协议类型，PTZ_PROTOCOL_USE(已添加)、PTZ_PROTOCOL_DEL(待添加)
 * 输	  出：
 *			ERR_PTZ_WRONG_PARAM, 错误的参数
 * 修改日志：
 * 			2013-02-20 dce creat
 */
td_s32 td_cmd_GetProtocol(td_u8 _cList[MAX_PROTOCOL_NUM][MAX_PROTOCOL_NAME_LEN], td_s32 _iProtocolAttr);

/*
 * 函数名称：td_cmd_ControlCruise
 * 简要描述: 开始或结束调用巡航轨迹
 * 输	  入：
 * 			_iChannelNo, 通道号
 *			_iCruiseNo, 巡航轨迹号
 *			_iControlCode,控制码，PTZ_CRUISE_START(开始巡航轨迹)、PTZ_CRUISE_STOP(结束巡航轨迹)
 * 输	  出：
 *			ERR_PTZ_WRONG_PARAM, 错误的参数
 * 修改日志：
 * 			2013-02-20 dce creat
 */
td_s32 td_cmd_ControlCruise(td_s32 _iChannelNo, td_s32 _iCruiseNo, td_s32 _iControlCode);


/*
 * 函数名称：td_cmd_SetComBaudRate
 * 简要描述: 设置串口波特率
 * 输	  入：
 * 			_iComNo, 串口号
 *			_iBaudRate, 欲设的波特率值
 * 输	  出：
 *			ERR_COM_WRONG_PARAM, 错误的参数
 * 修改日志：
 * 			2013-02-21 dce creat
 */
td_s32 td_cmd_SetComBaudRate(td_s32 _iComNo, td_s32 _iBaudRate);

/*
 * 函数名称：td_cmd_GetComBaudRate
 * 简要描述: 获取串口波特率
 * 输	  入：
 * 			_iComNo, 串口号
 *			_piBaudRate, 存取获得的波特率值
 * 输	  出：
 *			ERR_COM_WRONG_PARAM, 错误的参数
 * 修改日志：
 * 			2013-02-21 dce creat
 */
td_s32 td_cmd_GetComBaudRate(td_s32 _iComNo, td_s32 *_piBaudRate);

/*
 * 函数名称：td_cmd_SetComBits
 * 简要描述: 设置串口数据位、停止位、校验位
 * 输	  入：
 * 			_iComNo, 串口号
 *			_iDataBit, 欲设的数据位，取值为7或8
 *			_iStopBit, 欲设的停止位，取值为1或2
 *			_iCheckBit, 欲设的校验位，取值为'o'(奇校验)、'e'(偶校验)、's'(无校验)、'n'(无校验)，不区分大小写
 * 输	  出：
 *			ERR_COM_WRONG_PARAM, 错误的参数
 * 修改日志：
 * 			2013-02-21 dce creat
 */
td_s32 td_cmd_SetComBits(td_s32 _iComNo, td_s32 _iDataBit, td_s32 _iStopBit, td_s32 _iCheckBit);

/*
 * 函数名称：td_cmd_GetComBits
 * 简要描述: 获取串口数据位、停止位、校验位
 * 输	  入：
 * 			_iComNo, 串口号
 *			_piDataBit, 存取获得的数据位
 *			_piStopBit, 存取获得的停止位
 *			_piCheckBit, 存取获得的校验位
 * 输	  出：
 *			ERR_COM_WRONG_PARAM, 错误的参数
 * 修改日志：
 * 			2013-02-21 dce creat
 */
td_s32 td_cmd_GetComBits(td_s32 _iComNo, td_s32 *_piDataBit, td_s32 *_piStopBit, td_s32 *_piCheckBit);

/*
 * 函数名称：td_cmd_SetComWorkMode
 * 简要描述: 设置串口工作模式
 * 输	  入：
 * 			_iComNo, 串口号
 *			_iWorkMode, 欲设的串口工作模式
 * 输	  出：
 *			ERR_COM_WRONG_PARAM, 错误的参数
 * 修改日志：
 * 			2013-02-21 dce creat
 */
td_s32 td_cmd_SetComWorkMode(td_s32 _iComNo, td_s32 _iWorkMode);

/*
 * 函数名称：td_cmd_GetComWorkMode
 * 简要描述: 获取串口工作模式
 * 输	  入：
 * 			_iComNo, 串口号
 *			_piWorkMode, 存取串口的工作模式
 * 输	  出：
 *			ERR_COM_WRONG_PARAM, 错误的参数
 * 修改日志：
 * 			2013-02-21 dce creat
 */
td_s32 td_cmd_GetComWorkMode(td_s32 _iComNo, td_s32 *_piWorkMode);

/*
 * 函数名称：td_cmd_PTZClarity
 * 简要描述: 透明通道下的PTZ控制
 * 输	  入：
 * 			_iComNo, 串口号
 *			_pcData, 要写往串口的数据
 *			_iLen, 数据长度
 * 输	  出：
 *			ERR_COM_WRONG_PARAM, 错误的参数
 * 修改日志：
 * 			2013-02-21 dce creat
 */
td_s32 td_cmd_PTZClarity(td_s32 _iComNo, td_u8* _pcData, td_s32 _iLen);

td_s32 td_cmd_SetChannelPTZForVc(td_s32 _iMainChannelNo);

/*
 * 函数名称: td_cmd_GetComNum
 * 简要描述: 获取串口数和串口使能
 * 输	  入：
 * 			_piComNum, 存取获得串口数
 *			_piComEnable, 存取获取的串口使能
 * 输	  出：
 *			ERR_COM_WRONG_PARAM, 错误的参数
 * 修改日志：
 * 			2013-02-21 dce creat
 */
td_s32 td_cmd_GetComNum(td_s32 *_piComNum, td_s32 *_piComEnable);

/*
 * 函数名称: td_cmd_GetVcChnComNo
 * 简要描述: 获取合成通道的串口编号
 * 输	  入：
 * 			_piComNo, 串口号
 *			_piVcMainChnNo,  合成通道的主画面号
 * 输	  出：
 *			ERR_COM_WRONG_PARAM, 错误的参数
 * 修改日志：
 * 			2013-02-21 dce creat
 */
td_s32 td_cmd_GetVcChnComNo(td_s32 *_piComNo,td_s32 *_piVcMainChnNo);

td_s32 td_cmd_GetChannelPTZ(td_s32 _iChannelNo, td_s32* _piComNo, td_u8* _pcProtocol, td_s32* _piAddress);

td_s32 td_cmd_PtzControl(td_s32 _iChannelNo, td_s32 _iAction, PTZ_DATA *_pData);

td_s32 td_cmd_GetChannelComAttr(td_s32 _iChannelNo, TCom* _pstCom, td_s32* _pblEnable);

//20130305 add

/*
 * 函数名称: td_cmd_GetVideoparaCnf
 * 简要描述: 获取视频参数配置
 * 输	  入：
 * 			_iChannelNo,通道号	
 *			_piMaxFrameRate, 最大帧率
 *			_piMaxVencSize,  最大编码分辨率
 * 输	  出：
 *			TD_SUCCESS 获取成功
 *			TD_FAILURE 获取失败
 */
td_s32 td_cmd_GetVideoparaCnf(td_s32 _iChannelNo, td_s32 *_piMaxFrameRate, td_s32 *_piMaxVencSize);

/*
 * 函数名称: td_cmd_SetVideoparaCnf
 * 简要描述: 设置视频参数配置
 * 输	  入：
 * 			_iChannelNo,通道号	
 *			_iMaxFrameRate, 最大帧率
 *			_iMaxVencSize,  最大编码分辨率
 * 输	  出：
 *			TD_SUCCESS设置成功
 *			TD_FAILURE 设置失败
 */
td_s32 td_cmd_SetVideoparaCnf(td_s32 _iChannelNo, td_s32 _iMaxFrameRate, td_s32 _iMaxVencSize);

/*
 * 函数名称: td_cmd_GetChnParam
 * 简要描述: 获取通道参数
 * 输	  入：
 * 			_iChannelNo,通道号	
 * 输	  出：
 */
strChannelParam *td_cmd_GetChnParam(td_s32 _iChannelNo);

/*
 * 函数名称: td_cmd_GetMediaDevice
 * 简要描述: 获取设备参数
 * 输	  入：
 * 			_pstDev,设备参数
 * 输	  出：
 */
const MediaDevice *td_cmd_GetMediaDevice(MediaDevice *_pstDev);

td_s32 td_cmd_ChannelOsdChangeNotify(td_s32 _iChannelNo);

/*
 * 函数名称: td_cmd_SetVideoModeCheck
 * 简要描述: 设置视频制式自动检测
 * 输	  入：
 * 			_iVideoModeCheck,视频制式自动检测设置值
 * 输	  出：
 */
td_s32 td_cmd_SetVideoModeCheck(td_s32 _iVideoModeCheck);

/*
 * 函数名称: td_cmd_GuiGetMixAudioChn
 * 简要描述: UI获取混音通道配置
 * 输	  入：
 * 			_iChannelNo,音频通道号
 *			_pstMixaudioMsg,混音配置
 * 输	  出：
 *			TD_FAILURE,获取失败
 */
td_s32 td_cmd_GuiGetMixAudioChn(td_s32 _iChannelNo,  TMixAudio *_pstMixaudioMsg);

/*
 * 函数名称: td_cmd_GuiSetMixAudioChn
 * 简要描述: UI设置混音通道配置
 * 输	  入：
 *			_pstMixAudio,混音配置
 * 输	  出：
 *			TD_SUCCESS,设置成功
 *			TD_FAILURE,设置失败
 */
td_s32 td_cmd_GuiSetMixAudioChn(TMixAudio *_pstMixAudio);

/*
 * 函数名称: td_cmd_GetVideoSizeList
 * 简要描述: 获取视频分辨率列表
 * 输	  入：
 *			_iChannelNo,通道号
 *			_iVideoList,视频分辨率列表
 *			_iMaxCnt,
 * 输	  出：
 *			TD_SUCCESS,设置成功
 *			TD_FAILURE,设置失败
 */
td_s32 td_cmd_GetVideoSizeList(td_s32 _iChannelNo, td_s32 *_iVideoList, td_s32 _iMaxCnt);

/*
 * 函数名称: td_cmd_GetFrameRateList
 * 简要描述: 获取通道帧率列表
 * 输	  入：
 *			_iChannelNo,通道号
			 _iMaxCnt,最大个数
 *			
 * 输	  出：
 			_iFrameList,帧率列表
 *			返回帧率列表个数
 */
td_s32 td_cmd_GetFrameRateList(td_s32 _iChannelNo, td_s32 *_iFrameList, td_s32 _iMaxCnt);
/*
 * 函数名称: td_cmd_SetFrameRateList
 * 简要描述: 设置通道帧率列表
 * 输	  入：
 *			_iChannelNo,通道号
 *			_iFrameList,帧率列表
 			_iCnt,列表个数
 * 输	  出：
 *			TD_SUCCESS,成功
 *			TD_FAILURE,失败
 */
td_s32 td_cmd_SetFrameRateList(td_s32 _iChannelNo, td_s32 *_iFrameList, td_s32 _iCnt, int _iSaveToCfg);

td_s32 td_cmd_GuiVolCtr(td_s32 _iChannelNo, td_s32 *_piVolume);

/*
 * 函数名称: td_cmd_SingleTalkingCtrl
 * 简要描述: 对讲控制
 * 输	  入：
 *			_iType,对讲音频数据格式
 *			_iTalkingCtrl,对讲控制方式(只接受开始和停止两种控制)
 * 输	  出：
 *			TD_SUCCESS,控制成功
 *			TD_FAILURE,控制失败
 */
td_s32 td_cmd_SingleTalkingCtrl(td_s32 _iType, td_s32 _iTalkingCtrl);

td_s32 td_cmd_SetAoVolume(td_s32 _iAoDev, td_s32  _iVolume, td_s32 _iFlag);
//用户
td_s32 td_cmd_setUserRight(char *_UserName, int *_iRightId,unsigned int *_iChnBit, int _iCnt);
td_s32 td_cmd_judgeRightType(int _iAuthority);
td_s32 td_cmd_AddUser(char* _cUserName, char* _cPwd, int _iAuthority);
int  td_cmd_DelUser(char* _cUserName);
int td_cmd_ModifyLevel(char* _pcUser, int iLevel);
td_s32 td_cmd_GetNetDefaultGroupRights(int _iGroupId, OUT unsigned int *_u32Authority);
td_s32 td_cmd_GetNetMaxGroupRights(int _iGroupId, OUT unsigned int *_u32Authority);

//20120403 王磊add 实时获取各个通道最大分辨率

td_s32 td_cmd_getMaxVideoSize(td_s32 _iChn, td_u32 _u8VideoSize, td_s32 _iNorm);

/**********************************假日计划**************************/

/*
 * 函数名称: td_cmd_setHolidayPara
 * 简要描述: 设置假日计划参数
 * 输	  入：
 *			_pstHolidayParam,假日信息
 * 输	  出：
 *			td_success,设置成功
 *			td_failure,设置失败
 */
td_s32 td_cmd_setHolidayPara(THolidayInfo *_pstHolidayParam);


/*
 * 函数名称: td_cmd_getHolidayPara
 * 简要描述: 获取已有的假日计划信息
 * 输	  入：
 *			_iHolidayId,假日编号
 *			_pstHolidayInfor,存取假日信息
 * 输	  出：
 */
td_s32 td_cmd_getHolidayPara(td_s32 _iHolidayId, THolidayInfo *_pstHolidayInfor);

/*
 * 函数名称: td_cmd_setHolidaySchedule
 * 简要描述: 设置假日计划录像模板
 * 输	  入：
 *			_iChannelNo,通道号

 *			_pstRecTmp, 模板参数
 * 输	  出：
 *			td_success,设置成功
 *			td_failure,设置失败
 */
td_s32 td_cmd_setHolidaySchedule(td_s32 _iChannelNo, TGuiHolidayRecTemplate *_pstRecTmp);

/*
 * 函数名称: td_cmd_getHolidaySchedule
 * 简要描述: 获取假日计划录像模板
 * 输	  入：
 *			_iChannelNo,通道号

 *			_pstRecTmp, 模板参数
 * 输	  出：
 *			td_success,获取成功
 *			td_failure,获取失败
 */
td_s32 td_cmd_getHolidaySchedule(td_s32 _iChannelNo, TGuiHolidayRecTemplate *_pstRecTmp);

td_s32 td_cmd_set_FileLockStatus(char *_pcFileName, td_u32 iLockFlag);
//20130327 王磊add 获取文件加锁
td_s32 td_cmd_Get_FileLockStatus(char *_pcFileName, td_s32 *_piLockStatus);

//20130328向配置文件中写入保留时长
td_s32 td_cmd_Set_StoreTime(int _iChn, int _iDays);

//20130328从配置文件中读取各个通道文件保留时长
td_s32 td_cmd_GetStoreTime(td_s32 _iChn);

//20130401 王磊add 设置抽帧录像状态
td_s32 td_cmd_SetFramePick(int _iChn, int _iEnable);
//20130401 王磊add	从配置文件中读取各个通道抽帧录像状态
td_s32 td_cmd_GetFramePick(int _iChn);
//副码流录像add
td_s32 td_cmd_SetSubRecord(int _iChn, int _iEnable);
td_s32 td_cmd_GetSubRecord(int _iChn);
//end
//20130408 王磊add 设置冗余录像状态
int td_cmd_SetRedundRecState(int _iChn, int _iEnable);
//20130408 王磊add  从配置文件中读取各个通道冗余录像状态
int td_cmd_GetRedundRecStat(int _iChn);
td_s32 td_cmd_GetChnRecParam(TChnRecPara * _pstChnRecPara, int _iChn);
td_s32 td_cmd_VcaTypeToNet(td_s32 _iEventType,td_s32 _iFlg);



/**************************************end******************************/

int td_cmd_getDiskGrpInfo(int _iDiskGrp, unsigned int* _puiDisks, unsigned int* _puiChannels);

int td_cmd_setDiskGrpInfo(unsigned int _uiDisks[], unsigned int _uiChannels[]);

int td_cmd_getChnSpaceInfo(int _iChn, unsigned int* _puiUsedRec, unsigned int* _puiLimitRec, unsigned int* _puiUsedSnap, unsigned int* _puiLimitSnap);

int td_cmd_setChnSpaceInfo(int _iChn, unsigned int _uiLimitRec, unsigned int _uiLimitSnap);

int td_cmd_getStorageMode();

int td_cmd_setStorageMode(int _iMode);

int td_cmd_setDiskUsage(td_s32 _iDiskNum, td_s32 _iUsage);

int td_cmd_getDiskUsage(td_s32 _iDiskNum, td_s32 *_piUsage);

//计算磁盘剩余空间和录像时长
td_s32 td_cmd_calcDiskandRecTime (int _iDiskNo, int *_iDiskFree, int *_iRecTime);


//dce add ,设置主码流定时参数
td_s32 td_cmd_setMainTimeParam(td_s32 _iChn);

//dce add, 设置主码流事件参数
td_s32 td_cmd_setMainEventParam(td_s32 _iChn);

td_s32 td_cmd_getMainAlarmStat(int _iChn);

//dce add,保存主码流定时视频相关参数至配置
td_s32 td_cmd_SaveMainTimeParam(td_s32 _iChn, TChannelParamEvent *_ptCnf);


//dce add,保存主码流事件参数至配置
td_s32 td_cmd_SaveMainEventParam(td_s32 _iChn, TChannelParamEvent *_ptCnf);


//dce add,设置编码参数
td_s32 td_cmd_setVideoPara(td_s32 _iChn, td_s32 _iType, TChannelParamEvent *_ptCnf);

//dce add,获取保存的主码流(事件)编码参数
TChannelParamEvent *td_cmd_GetChnParamEvent(td_s32 _iChannelNo, TChannelParamEvent *_Arg);

//dce add,获取保存的主码流(定时)编码参数
TChannelParamEvent *td_cmd_GetChnParamTime(td_s32 _iChannelNo, TChannelParamEvent *_Arg);

strAppChannel *td_cmd_GetChnParamSub(td_s32 _iChannelNo);
//智能录像相关
td_s32 td_cmd_SaveVencSamrtParamTemplate(td_s32 _iChn, td_s32 _iTemplateType, TChannelParamSmart *_ptCnf);
TChannelParamSmart *td_cmd_GetMainSmart(td_s32 _iChn, td_s32 _iTemplate);
td_s32 td_cmd_SetSmartRecTemplate(td_s32 _iCh,td_s32 _iWeekly,td_s32 _iSegment,td_s32 _iTemplateType);
td_s32 td_cmd_GetSmartRecTemplate(td_s32 _iCh,td_s32 _iWeekly,td_s32 _iSegment);
td_s32 td_cmd_SetSmartRecCallBack(td_s32 _iChn, td_s32 _iType);

//shihc add 20130422
td_s32 td_cmd_SetChnPtzPriority(td_s32 _iChannelNo,td_s32 _iPriority);
td_s32 td_cmd_setwififtp_auto(td_s32 _iEnable);
td_s32 td_cmd_getwififtp_auto();
td_s32 td_cmd_setwififtp_manual();
//liyang add
td_s32 td_cmd_set3GDialog(td_s32 _iStartType,td_s32 _iStopType,td_s32 _iDuration);
td_s32 td_cmd_set3GMessage(td_char cPhone[5][32]);
td_s32 td_cmd_Set3GSim(td_char * _pcSim);
td_s32 td_cmd_set3GTaskSchdule(td_s32 _iStopType[3],td_s32 _iDuration[3]);
td_s32 td_cmd_set3GNotify(td_s32 _iType, td_char *_pcMessage);
td_s32 td_cmd_set3GVpdnpara(td_char *_pcDialNumber,td_char *_pcAccount,td_char *_pcPassword);
td_s32 td_cmd_Get3GDialog(td_s32* _iStartType,td_s32* _iStopType,td_s32* _iDuration);
td_s32 td_cmd_Get3GMessage(char _cPhone[5][32]);
td_s32 td_cmd_Get3GSim(td_char * _pcSim);
td_s32 td_cmd_Get3GTaskSchdule(td_s32* _iTimeEnable,td_s32 _iStartTime[3],td_s32 _iStopTime[3]);
td_s32 td_cmd_Get3GNotify(td_s32* _iType, td_char *_pcMessage);
td_s32 td_cmd_Get3GVpdnpara(td_char *_pcApn,td_char *_pcAccount,td_char *_pcPassword);


td_s32 td_cmd_SetWifi_Module(td_s32 _iWifiMode);
td_s32 td_cmd_SetWifi_StaConfig(unsigned int _uiIP,unsigned int _uiMask,unsigned int _uiGateWay, unsigned int _uiDNS,td_char* _pcESSID,td_s32 _iENCRYPTION,td_s32 _iWifiKeyType,td_char* _pcWifiPassword,td_s32 iWifiKeyNum);
td_s32 td_cmd_SetWifi_StaDhcpEn(td_s32 _iEnable);
td_s32 td_cmd_SetWifi_StaPara(td_s32 _iKeySelect,td_s32 _iKeyType,td_s32 _iSetSafeEnable);
td_s32 td_cmd_SetWifi_ApNetConfig(td_char* _pcIP,td_char* _pcMask,td_char* _pcGateWay,td_char* _pcDNS,td_char* _pcESSID,td_s32 _pcENCRYPTION,td_s32 _pcWifiKeyType,td_char* _pcWifiPassword,td_s32 _iWifiKeyNum);
td_s32 td_cmd_SetWifi_ApNetConfig_Ex(StrWifiAP* _psWifiAp);
td_s32 td_cmd_SetWifi_ApDhcpConfig(td_s32 _iDHCPStart,td_s32 _iDHCPEnd,td_s32 _iDHCPLease);
td_s32 td_cmd_SetWifi_ApDhcpEnable(td_s32 _iEnable);
td_s32 td_cmd_SetWifi_Break(td_s32 _iBreak);
td_s32 td_cmd_GetWifi_StaConfig(strWifiCnf* _psWifiAp);
td_s32 td_cmd_GetWifi_ModuleEn();
td_s32 td_cmd_GetWifi_ApConfig(NetWifiAP* _psWifiAp);
td_s32 td_cmd_GetWifiConnectStat();
td_s32 td_cmd_Search_ApList(td_char *_pMsg);

//add end


//dce add,电源管理相关
td_s32 td_cmd_SetPowerAlarmMsg (td_s32 _iAlarm);
td_s32 td_cmd_SetPowerOffMsg (td_s32 _iShutdown);

//获取报警关机电压阈值
td_s32 td_cmd_GetPowerAlarmMsg ();
td_s32 td_cmd_GetPowerOffMsg ();

td_s32 td_cmd_SetPowerManagerMsg (TPowerManageMsg *_ptPowerManageMsg);
td_s32 td_cmd_GetPowerManagerMsg (TPowerManageMsg *_ptPowerManageMsg);
td_s32 td_cmd_GetVoltage();
td_s32 td_cmd_GetTemperature();


//add end

//dce add, 获取GPS模块类型
td_s32 td_cmd_GetGPSModuleType();

td_s32 td_cmd_SetGPSMsgOvelay(td_s32 _iEnable);
td_s32 td_cmd_GetGPSMsgOveray();
td_s32 td_cmd_SetGPSTimeEnable(td_s32 _iEnable);
td_s32 td_cmd_GetGPSTimeEnable();
td_s32 td_cmd_SetGPSMsgChnOverlay(td_s32 _iChn, td_s32 _iEnable);
td_s32 td_cmd_GetGPSMsgChnOverlay();

td_s32 td_cmd_GetGPSInfor(td_s32 *_iSateNum, td_s32 *_iIntensity);
td_s32 td_cmd_SetGPSFilter(td_s32 _iFilter);
td_s32 td_cmd_GetGPSFilter();

//格式化完成重新打开日志文件句柄
td_s32 td_cmd_RestartLog(td_s32 _iDiskNo);
//格式化开始关闭日志文件句柄
td_s32 td_cmd_PauseLog(td_s32 _iDiskNo);
//add by liuyan
void td_cmd_RegProcRebldProgCallBack(td_cmd_ProcRebldProgCallBack _CallBack, td_u8 _Who);
td_s32 td_cmd_ProcRebuildProgress(int _iProgress);
td_s32 td_cmd_SetPreviewPrefer(td_s32 _iEnable);
td_s32 td_cmd_GetPreviewPrefer();


//shihc add 20130422
td_s32 td_cmd_SetChnPtzPriority(td_s32 _iChannelNo,td_s32 _iPriority);

td_s32 td_cmd_GetNetPerformanceCost(TCmdNetPerformanceCost * _stPerformanceCost);

td_s32 td_cmd_NetstatusTest(td_s32 _iEth,void *_pNetstatus);//add by zhb 20131027
td_s32 td_cmd_NetDelayTest(td_s32 _iEth,char *_cIp,void *_pNetDelay);//add by zhb 20131027

td_s32 td_cmd_NetPackCapture(td_s32 _iEth, td_u8* _cPath, td_s32 _iPackSize, td_s32 _iTmDelay);


td_s32 td_cmd_IpcChnParaExport(td_s32 _iChn,const td_char* _cFileName);
td_s32 td_cmd_IpcChnParaImport(td_s32 _iChn, const td_char* _cFileName);

//zhangsheng add 20131027
td_s32 td_cmd_SetSysAutoMaintain(td_s32 _iRebootDay, td_s32 _iRebootTime);
td_s32 td_cmd_GetSysAutoMaintain(td_s32* _piRebootDay, td_s32* _piRebootTime);
//zhangshengusr
td_s32 td_cmd_GetUserOnline(td_s32 _iNum, TGuiOnlineUserMsg* _pReq);
td_s32 td_cmd_SetDisconnectUsr(td_s32 _iTime, char* _cIpAddr);
//huweixu add 20150411
td_s32 td_cmd_DisconnectIPFilterUsr(void);
td_s32 td_cmd_GetIpcVersion(td_s32 _iChn,td_char* _cVersion);
td_s32 td_cmd_ChangeIpcIp (const char* _cMac,const  char* _cIP,const char* _cSubMask,const char* _cGateway,const char* _cDns);
td_s32 td_cmd_IpcReboot (int _iChn);
td_s32 td_cmd_RebootChn (int _iChn);
td_s32 td_cmd_IpcUpgrade (int _iChn,const char* _cFileName);
td_s32 td_cmd_GetIpcUpgrateSta(int _iChn,int *_piRate);
//////////////录像标签相关////////////////
td_s32 td_cmd_TagModify(int _iChn, int _iOperation, char* _cFileName, int* _piTagTm, char* _cTagName);
td_s32 td_cmd_TagQuery(char* _cFileName, char* _cTagName, TTagInfo _stTagInfo[],int _iMaxTagNum);
td_s32 td_cmd_setvideooutmode(int _iVoid,TVideoOutputType _iMode);
void td_cmd_RegProcRebldProgCallBack(td_cmd_ProcRebldProgCallBack _CallBack, td_u8 _Who);
td_s32 td_cmd_ProcRebuildProgress(int _iProgress);
//////////////////////////////////////////

//哈希功能使能设置与获取
td_s32 td_cmd_setHashEnable(td_s32 _iEnable);
td_s32 td_cmd_getHashEnable(td_s32 *_iEnable);
td_s32 td_cmd_GetAdditonFileInfo(td_s32 _iFileType,td_s8* _pcFileName);
//画面坐标
td_s32 td_cmd_GetCreateFreeVo();


//政法主机审讯案件加密
td_s32 td_cmd_SetPoliticsVideoEncrypt(td_char *_cPassword, td_s32 _iEnable);

td_s32 td_cmd_GetPoliticsVideoEncrypt(td_char _cPassword[ENCRYPT_PWD_LENGTH+1], td_s32 *_iEnable);


//政法主机回放时用默认密码先解密一次，不对再提示输入密码
td_s32 td_cmd_SetPoliticVideoEncrypt();
td_s32 td_cmd_GetRaidStat(td_s32 *_piEnable);
td_s32 td_cmd_SetRaidStat(td_s32 _iEnable);
td_s32 td_cmd_setRaidName(const char * _pcRaidName);
td_s32 td_cmd_getRaidName(OUT char *_pcRaidName);
//td_s32 td_cmd_setSataRaid(void);
td_s32 td_cmd_setDiskinfo(td_s32 iNowSataNum);

//提取纯音频功能使能设置与获取
td_s32 td_cmd_setSplitAvEnable(td_s32 _iChn, td_s32 _iEnable);
td_s32 td_cmd_getSplitAvEnable(td_s32 _iChn, td_s32 *_iEnable);
	
td_s32 td_cmd_setRaidArrayMsg(td_s8 *_pcRaidName, td_s32 _iOptType, td_s32 _iRaidType, td_s8 *_pcDisklist, td_s32 *_piDisklist, td_s32 _iDiskTotal);
td_s32 td_cmd_getRaidArrayMsg(td_s32 _iNo, TRaidArrayCfg *_pstRaidArryCfg, td_s8 *_pcDisklists);
td_s32 td_cmd_setHddWorkMode(td_s32 _iHddNo, td_s8 *_pcRaidName, td_s32 _WorkMode);
td_s32 td_cmd_setRaidVirDiskMsg(td_s8 *_pcVDName, td_s32 _iOptType, td_s32 _iVdSize, td_s32 _iInitType, td_s8 *_pcRaidName);
td_s32 td_cmd_getRaidVirDiskMsg(td_s32 _iNo, TRaidVdCfg *_pstRaidVdCfg);
td_s32 td_cmd_getRaidSataList(TSataDiskCfg *_pstSataDiskCfg, td_s32 *_piTotal);
td_s32 td_cmd_getRaidVirDiskStatInfo(TRaidVdStat *_pstRaidVdStat, td_s32 *_piTotal);
td_s32 td_cmd_getRaidArrayStatInfo(TRaidArrayStat *_pstRaidArrayStat, td_s32 *_piTotal);
td_s32 td_cmd_GetRaidRiadUseAbleSize(char* _cRaidName);
td_s32 td_cmd_setDevTemp(int _iStartTemp, int _iForceCtrl);
td_s32 td_cmd_getDevTemp();
td_s32 td_cmd_GetDvdSerialNum(td_s32 _iCdNum,td_u8* _pcCDROMSerialNoValue);

//切换视频源输入模式,_iMod取值:0-保留，1-本地输入模式，2-示证输入模式
td_s32 td_cmd_SetViMod(td_s32 _iChn, td_s32 _iMod);
td_s32 td_cmd_GetViMod(td_s32 _iChn);
td_s32 td_cmd_GetCurViMod(void);

//设置和获取Vo设备信息
int td_cmd_get_vodev_cnt();
int td_cmd_set_vodev_cnt(int _iVoDevCnt);

//政法主机获取和设置音频频拼接使能
int td_cmd_GetMedleyAvEnable(int _iChn);
int td_cmd_SetMedleyAVEnable(int _iChn, int _iEnable);
//设置刻录和备份不同录像文件格式的文件
td_s32 td_cmd_SetFileConvertFrmt(td_s32 _iBurnType,td_s32 _iFileFrmt);
td_s32 td_cmd_GetFileConvertFrmt(td_s32 _iBurnType,td_s32 *_piFileFrmt);

//设置/获取刻录单一录像文件格式
td_s32 td_cmd_SetBurnSingleFileFrmt(td_s32 _iBurnType,td_s32 _iFileFrmt);
td_s32 td_cmd_GetBurnSingleFileFrmt(td_s32 _iBurnType,td_s32 *_piFileFrmt);

//设置刻录/备份音频文件类型
td_s32 td_cmd_SetFileAudioBackFrmt(td_s32 _iBurnType,td_s32 _iFileFrmt);
td_s32 td_cmd_GetFileAudioBackFrmt(td_s32 _iBurnType,td_s32 *_piFileFrmt);


td_s32 td_cmd_getSplitAvBurnEnable(td_s32 *_iEnable);
td_s32 td_cmd_getSplitAvBackupEnable(td_s32 *_iEnable);
td_s32 td_cmd_setSplitAvBurnEnable(td_s32 _iEnable);
td_s32 td_cmd_setSplitAvBackupEnable(td_s32 _iEnable);


// 设置语音激励
int td_cmd_SetBestirResponse(int _iChn, int _iStat);

//通知外挂语音激励
int td_cmd_SetBestirToApp(int _iChn, int _iStat);
//设置MIC 类型
td_s32 td_cmd_set_MicType(int _iVideoChn, int _iAudioChn, int _iMicType);
td_s32 td_cmd_ForceIframeToApp(int _iChn);//通知外挂强制I帧


typedef enum
{
	//0-保留，1-输入，2-输出，3-LineIn，4-MicIn
	AUDIO_VOLUME_TYPE_IN = 0,
	AUDIO_VOLUME_TYPE_OUT,
	AUDIO_VOLUME_TYPE_LINE_IN,
	AUDIO_VOLUME_TYPE_MIC_IN,
}TAVLOUMETYPE;
/*
函数名 		:	td_cmd_GetAudioVolume
功能	    :	获取对应ID的音量值
输入参数	:	_iId:ID号
				_piVolume:	声音大小
返回值		: >=0 成功   -1 失败
*/
td_s32 td_cmd_GetAudioVolume(td_s32 _iId, td_s32 *_piVolume);

/*
函数名 		:	td_cmd_SetAudioVolume
功能	    :	获取对应Id的音量值
输入参数	:	_iId:ID号
				_iVolume:声音大小
返回值		: >=0 成功   -1 失败
*/
td_s32 td_cmd_SetAudioVolume(td_s32 _iId, td_s32 _iVolume);
/*
函数名 		:	td_cmd_AudioVolCtr
功能	    :	设置/获取对应Id的音量值
输入参数	:	_iId:ID号
				_iVolume:声音大小,	-1:获取音量
返回值		: >=0 成功   -1 失败
*/
td_s32 td_cmd_AudioVolCtr(td_s32 _iId, td_s32 *_piVolume);

//初始化设备音量值
int td_cmd_AudioVolInit(int _iProductModel);

td_s32 td_cmd_set_commoninfo(STR_COMMON_INFO _stCommonInfo);
td_s32 td_cmd_get_commoninfo(STR_COMMON_INFO *_pstCommonInfo);

//设置获取激励类型
td_s32 td_cmd_set_BestirType(int _iType);
td_s32 td_cmd_get_BestirType(int *_piType);

//获取AO个数
int td_cmd_GetAoCnt();

//获取远程音频通道
int td_cmd_GetAudioRemoteChn(int _iId);

td_s32 td_cmd_DirectStartBurn(td_s32 _iCdNo,td_s32 _iIsOneKey);
/*
函数名 		:	td_cmd_DirectStartburn
功能	    :	ui或者ie实时刻录大文件接口
输入参数	:	 _iCdNo : 光驱号  _iIsOneKey:是否断弹盘
返回值		: 	〉=0 成功  -1 失败
*/
td_s32 td_cmd_DirectStartBurnEx(td_s32 _iCdNo,td_s32 _iIsOneKey,td_s32 _iIsOverLap);
/*
函数名   	:	td_cmd_DirectStartBurnEx
功能	    :	ui开启庭审时刻录大文件接口
输入参数	:	 _iCdNo : 光驱号  _iIsOneKey:是否弹盘  _iIsOverLap:是否断电复刻
返回值	    : 	〉=0 成功  -1 失败

*/

//设置根据中控上报的mic 地址对应的通道是否有音频进行打点
int td_cmd_SetAudioChangeTag(unsigned char _ucVideoChn, unsigned char _ucStat);

td_s32 td_cmd_SetCloseProperty(td_s32 _iEnable);
/*
函数名   	:	td_cmd_SetCloseProperty
功能	    :	设置封盘、弹盘使能
输入参数	:	 _iEnable : 光盘弹盘和封盘状态  0，保留；按位bit0-1,封盘 bit0-0,不封盘; bit1-1,弹盘;bit1-0,不弹盘
返回值	    : 	=0 成功  -1 失败

*/

td_s32 td_cmd_getcloseproperty(td_s32 *_iEnable);
/*
函数名   	:	td_cmd_getcloseproperty
功能	    :	获取封盘、弹盘使能
输入参数	:	 _iEnable : 光盘弹盘和封盘状态  0，保留；按位bit0-1,封盘 bit0-0,不封盘; bit1-1,弹盘;bit1-0,不弹盘
返回值	    : 	=0 成功  -1 失败

*/

int td_cmd_getScheduleEnable(int *_iEnable); //政法 -获取是否支持排期
int td_cmd_getProofEnable(int *_iEnable); //政法-获取是否支持示证切换
int td_cmd_getVcSetVodevListEnable(int *_iEnable); //政法-获取是否支持合成画面设置输出设备列表
int td_cmd_GetVoDevList(int *_iVoDevList, int _iMaxCnt);//政法-获取支持的合成画面设置输出设备列表



#endif /*_ASSISTANT_H_*/


