
#ifndef _TD_PLAY_BACK_1_H_
#define _TD_PLAY_BACK_1_H_
#ifdef PLAYBACK1_H
#define EXT 
#else
#define EXT extern 
#endif

#include <semaphore.h>

#include "libstorage.h"
#include "decode.h"

//#define PLAYPATH 			"/dvr/player/MiniPlayer.exe"
#define PLAYPATH 			"/dvr/player/MiniPlayer.exe.tar.gz"
//硬盘快照
#define SNAP_ERR_ARG    -1
#define SNAP_HAVE       1
#define SNAP_EMPTY      0

//回放
#define MAX_CUT_NUM 30
#if 1
#define 	USLEEP(t)		do \
							{  \
								struct timeval tm = {0, t};  \
								select(0, NULL, NULL, NULL, &tm);\
							}while(0);

#else
#define 	USLEEP(t) usleep(t)
#endif


//#define MAXFILELISTLEN (1024 + 512)   	//单窗口查询最长文件数目
#define MAXFILELISTLEN (1024)   	//单窗口查询最长文件数目

#define USUALLY 1
#define CUTVO 2
#define RECONNECT 3

#define PLAYBACKNOCREATE (-1)
#define FILENAMELENOVER	(-2)
#define TIMEERR	(-3)
#define OPENFILEERR (-4)
#define NOFILE	(-5)
#define SPEEDMAXOVER (-6)
#define SPEEDMINOVER (-7)
#define SPEEDSETERR (-8)

#define FINDING	1
#define FINDED	2

#define START	1
#define PAUSE	0
#define STOP		2

#define MAXSPEED  16
#define MINSPEED  (-16)

#if 0
#define SpeedFastTwo (15)
#define SpeedFastFour	(18)
#define SpeedFastEight	(20)
#endif
#define SpeedFastTwo 	(2 * 10)
#define SpeedFastFour	(4 * 10)
#define SpeedFastEight	(6 * 10)
#define SpeedFastMax	(MAXSPEED*10)
#define GUI_VIDEO_PLAY_NEED_PWD  200

#define PICLIMITFLAG 		1
#define SPEEDLIMITFLAG 	2
#define PICANDSPEEDLIMITFLAG 3




typedef struct
{
	int m_iPid;			//回放器句柄
	volatile PLAYBACK_STAT m_iStat;		//状态PLAYBACK_STAT
	int m_iSpeed;		//当前回放速度
	int m_iFinishFileFlag;
	int m_iFullScreen;
	time_t m_tCurPlayTm; 	//当前回放位置（时间值）//百分比
	int m_iTimeSeg;//时间进度
	int m_iBlackScreen;//空文件时黑屏的标识
	int m_iRate;
	int iATrueFalse;

	int m_iNeedSecret;
	double m_dSpeed;//速度控制(实际速度)
		
} TPlayerStat;

typedef struct
{
	int iEncrypt;
	int iATrueFalseBak;//是否播放声音
	double m_dSpeedBak;//速度标识备份
	PLAYBACK_STAT m_iRunFlagBak;//开始/暂停标识备份
	long long m_iSkipPosBak;
}TPlayerInterface;//外部控制接口

typedef struct 
{
	Decode m_pDc;//
	TPlayerStat m_RunStat;
	int m_ptFileBeginTm;//录像文件的开始时间
	int m_ptFileEndTm;//录像文件的结束时间
	unsigned int m_u32UpFrameSTime;//上一帧的时间戳
	long long m_iSkipPos;//跳转的时间(精度:S,百分比)
	int m_iPlayILocation;//在播放过程中前一个I帧的位置
	int iTimeChange;//改变基准时间标识
	int iHasBind;//表示解码通道与视频输出通道是否绑定
	volatile int m_iImperTm;
	unsigned int m_uiTimePot;//记录的时间点
	int m_iOlnyReadIFrame;
	double m_dBefSpeed;//改变帧率时的运行状态
	double m_fSpeedValue;//改变帧率时的运行状态
	pthread_mutex_t AudioLock;
	unsigned int m_u32TimeStamp;
	unsigned short m_AudioFlag;
	unsigned short m_u16BefAudioFrames;
	unsigned int m_u32BefSleepTm;
	int iSpeed16Rate;
	int m_iOptStat;
	struct 
	{
		long long m_lFrameTime;//帧时间戳，在多种条件下是变化的
		long long m_lForkTime;//进程时间戳，在快慢放的时候是变化的
	}s_TimeStamp;//基准
}TPlayBack;

typedef struct
{
	TFileInfo *m_pFileList;
	int 		m_NULL;//代表文件是否为空，0没有文件，1 空白文件，2:实际文件
	time_t	m_iStartTime;					//录像开始时间
	time_t	m_iEndTime;					//录像结束时间
	//int 	m_FileRunTime;
}TFileListPlay;

typedef struct 
{
	td_s32 	m_s32ListHead;
	td_s32 	m_s32FileListLength;
	int m_iFileNum;//文件的总数(从1开始)
	int m_iFileCurrent;//当前读取的文件处于列表中的位置(从0开始)
	TFileInfo 	*m_ptFileListInfo;
	TFileListPlay m_pFileListPlay[2 * MAXFILELISTLEN];
}TFileList;//文件列表结构


typedef struct 
{
	TFileList	m_TFileListMsg[MAXPLAYER];
}TsynPlayBack;



//异步回放激活函数
//功能确定VO的现实区域，与建立VO的个数
int TD_Playback_Active(int _s32VoDev, int _iVoCnt, TRect *_pVoRect);//ok


//功能:初始化播放器所需变量
//返回值:0:		成功
//			负数:	失败
int TD_Playback_Init(int _iPlayid);//ok

// 名称：playback_SetPlayeFile
// 功能：设置回放器播放文件，调用此接口将导致回放器重置到初始状态
// 参数：
//	_pPlayBack	:回放器句柄
//	_strFile		:录像文件名（全路径）
//	_ptBeginTm	:指示此录像文件的开始时间
//	_ptEndTm		:指示此录像文件的结束时间
// 返回值：0成功能，负数表失败
int Playback_SetPlayeFile(IN int _iPlayid, IN char *_strFile, IN time_t _iBeginTm, IN time_t _iEndTm, IN int _iFileNum);//ok

// 名称：playback_CreatePlayer
// 功能：创建一个回放器，构建回放所需资源，绑定解码器与VO显示关系，回放器处于初始就绪状态
// 参数：
//	_ptVo	:指定显示区域
// 返回值：负值表失败，非负表回放器句柄，后续回放操作均使用此句柄
int Playback_CreatePlayer(IN int _iPlayid);//ok

// 回放器操作相关接口，返回值0表成功，负数表失败
int Playback_Start(int _iPlayid);		//开始回放//ok

//暂停
int Playback_Pause(int _iPlayid);//ok
int Playback_BackStep(int _iPlayid);		//步进回放
int Playback_Back(int _iPlayid);		//倒放
int Playback_Stop(int _iPlayid);		//结束回放//ok

int Playback_WaitUinit(int _iPlayid, int _iWaitTime, int _iClearFlag);
#define Playback_WaitStop(_iPlayid)		Playback_WaitUinit(_iPlayid, 10 * 1000 * 1000, 1)
//步进
int Playback_Step(int _iPlayid);		//步进回放//ok

int Playback_Resume(int _iPlayid);//ok

int Playback_Fast(int _iPlayid, int _iCheck);//回放加速//ok

int Playback_Slow(int _iPlayid, int _iCheck);//回放减速//ok

int Playback_PlayAudio(int _iPlayid);//ok

int Playback_StopAudio(int _iPlayid);//只要创建了播放器就可以关闭声音//ok

//全屏播放
int Playback_SetFullScreen(int _iPlayid);//只有创建且不是空文件的时候，才可以全屏//ok

//退出全屏
int Playback_ESCFullScreen(int _iPlayid);//只要创建就可以退出全屏//ok

/////////////////////////////////////////////////////////
// 名称：playback_GetCurStat
// 功能：获取当前回放状态
// 参数：
//	_iPid		:回放器句柄
//	_ptStat	:状态信息（输出）
// 返回值：0成功能，-1失败
int Playback_GetCurStat(int _iPlayid, OUT TPlayerStat *_ptStat);//ok

int Playback_SetDecodePassword(int _iPlayid, char* _cPassword);//ok

int Playback_SetSpeed(int _iPlayid, int _iSpeed);//ok

int Playback_Seek(int _iPlayid, IN long long _iSeekTm);
int Playback_Sync_Seek(IN int _iSeekTm, int _iValue);
int Playback_Sync_Step(void);
int Playback_Sync_Stop(void);
int Playback_Sync_Pause(void);
int Playback_GetSynCurStat(OUT TPlayerStat *_ptStat);//ok
int Playback_GuiGetSynStat(OUT PLAYBACK_STAT * _piStat);
int Playback_Sync_Start(void);
td_s32 Playback_Sync_Build_FileList(time_t _iStartTime,time_t _iStopTime,int _Chan[]);
int  Playback_Sync_Active(int _s32VoDev, int _iVoCnt, TRect *_pVoRect);
int Playback_Sync_Destroy(void);
int Playback_Sync_SetDecodePassword(char* _cPassword);//ok
int Playback_Active(int _s32VoDev, int _iVoCnt, TRect *_pVoRect);
int Playback_Init(int _iPlayid);
int Playback_Sync_Continue(void);

int Playback_Sync_Fast(void);
int Playback_Sync_Resume(void);

int Playback_Sync_Slow(void);
int Playback_Clear(void);
int Playback_StatInit(int _iPlayid);
int Playback_DestroyPlayer(IN int _iPlayid);
int Playback_SetSpeedEx(int _iPlayid, int _iSpeed);
int Playback_Sync_SpeedEx(int _iSpeed);
void Playback_InitLimit(void);

void Playback_ClearLimitFlag(int _iPicOrSpeedFlag);

void Playback_SetLimit(int _iPicOrSpeedFlag, int _iChannelNo);
void Playback_TestLimit(int _iPicOrSpeedFlag, int *_piLimit);
int Playback_Sync_Clear();
int Playback_UkStart(int _iPlayid, IN char *_strFile);
void PlaybackSetMaxDecode(td_s32 _iMaxDecode);
///////////////电子放大///////////////
int  Playback_Ezoom(int _iHandle);
int  Playback_ExitEzoom( );
int  Playback_AdjustEzoom(TRect * _pRect);
//////////////抓拍预览////////////////
int Playback_OpenJpeg(char* _cFileName);
int Playback_ExitJpeg(td_s32 _s32VoDev);
int Playback_InitJpeg(int _s32VoDev);
//硬盘快照
#define SNAP_DESK_Path "/mnt/sata/"
typedef struct
{
	int m_iType;
	int m_iStartTime;
	int m_iStopTime;
} TSnapTime;
int rec_GetMonthMap(char* _pcPath, int _iYear, int _iMonth, int _iChannelNo, int* _piInfo);
int rec_GetMonthMapInit();
int rec_GetDayMap(int _iYear, int _iMonth, int _iDay, int _iChannelNo, int _iBlurTime, int _iMaxCnt, TSnapTime* _pstTime);


//剪辑
void PlaybackVideoBackup(void);
int Playback_GetPlayState(void);//add by gyn获取当前回放的状态大于0正在回放，=0没有回放
int Playback_Video_Clip_Query_Pro(int _iClipID);
int Playback_Video_Clip(int _iChn, int _iStartTime, int _iEndTime, char *_pcPath);
int ClipGetSegmentLen(int _iChn, int _iStartTime, int _iEndTime);
//抓拍
int Playback_CapPicture(int _iPlayid);
//
int Playback_GetOptStat(int _iPlayid);
#define UkSubToData(chn)	((((unsigned int)(chn) >> 16) << 7) | ((chn) & 0xffff))

///////////////////////////////////////////////////
#define PLAYFILE void
//#define SELF_DEC
typedef struct
{
	int m_iStat;
	int m_iSpeed;
	int m_iSchedule;
	int m_iPassword;
	int m_iCurTime;
}TplyStat;
PLAYFILE *playfile_build_List(time_t _iStartTime, time_t _iStopTime, int _Chn);
int playfile_unbuild_List(PLAYFILE *_ptPlayFile);
int playfile_read(PLAYFILE *_PlayId, void *_buf, int _iLen, int _iType, S_FrameHeader **_ptFrameHeader, S_FileHeader **_ptFileHeader, int *_piChgFile);
int playfile_seek(PLAYFILE *_PlayId, int _pos, int _iTime, int *_iReTime, S_FileHeader **_ptFileHeader);
int playfile_GetCmd(PLAYFILE *_PlayId, int _iCmd, void *_ptValue);
int playfile_SetCmd(PLAYFILE *_PlayId, int _iCmd, void *_ptValue);
int Play_SetTime(int _iChn, int _iVoChn, int _iVoDevId, IN time_t _iBeginTm, IN time_t _iEndTm);
int Play_Exit(int _iPlayId);
int Play_Pause(int _iPlayId);
int Play_Start(int _iPlayId);
int Play_Seek(int _iPlayId, int _iPos, int _iTm);
int Play_GetStat(int _iPlayId, TplyStat *_ptStat);
int Play_SetEncrypt(int _iPlayId, char *_cPassWord);
int playback_setdefaultAo(int _iDefaultAo);

int Playback_SetPoliticsVideoEncrypt(int _iPlayid, char *_cPasswd);


#endif

