#define PLAYBACK1_H

#include <sys/mount.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#include "playback.h"
#include "decode.h"
#include "td_sdk.h"
#include "channelmnt.h"
#include "libstorage.h"
#ifdef MEMWATCH
#include "memwatch.h"
#endif



#define    MAX_READ_LEN       1024
#define	MAX_FRAME_RATE		25


#define	MAX_DECODE		(g_iMaxDecode)
#define	MAX_DECODE_SRATE	(MAX_DECODE * MAX_FRAME_RATE)	
#define MAXENCODECOST	(MAX_DECODE - g_iMaxEncodeCostRemain)
#define MAXENCODESPEEDCOST (MAX_DECODE_SRATE - g_iMaxEncodeSpeedCostRemain)

static int g_iMaxDecode = 24 * 100;
static int g_iMaxEncodeCostRemain = 8 * 130;
static int g_iMaxEncodeSpeedCostRemain = 130 * 30 * 4;

#define FILENULL			0
#define FILEBLANK		1
#define FILEVAILD		2

#define FILE_ERR		-2
#define VOVDEC_ERR	-3
#define PLAYPFRAMERATE	5

#define SetChannelLimit(ChannelValue, ChannelNo)	(ChannelValue |= (1 << ChannelNo) )
#define ClearChannelLimit(ChannelValue, ChannelNo)	(ChannelValue &= ~(1 << ChannelNo) )
#define GetChannelLimit(ChannelValue, ChannelNo)	(ChannelValue & (1 << ChannelNo))

////%%%%%%%%电子放大及图片浏览%%%%%%%%%////
Td_VO* g_ptJpegVo = NULL;				//用于显示抓拍图片的VO		
TVdecChannel* g_ptJpegVdec=NULL;		//用于解码抓拍图片的解码通道
static TRect  g_ZoomRect;
static  int	 g_EzoomHand = -1;	
static  Td_VO* g_EzoomSmallWindow=NULL;	
static  Td_VO* g_EzoomBigWindow=NULL;
static int g_InputEZoomStat = -1;
////%%%%%%%% END %%%%%%%%%////

////%%%%%%%%异步回放变量%%%%%%%%%////
TPlayBack *g_PlayBkVo[MAXPLAYER] = {NULL};
typedef struct
{
	struct 
	{
		int m_iCnt;			//当前UI要求回放的视频个数
		int m_iAudioChannal;	//播放音频的通道.
		int m_FullPlay;		//全屏播放的通道。
	}TPlayAll;

	struct 
	{
		char m_cFile[FILENAMEMAXNUM];
		char iShowFlag;
		int StopFlag;
		int m_Rate;
		TPlayerInterface m_ExternalInterface;
		TPlayerStat m_RunStatBak;
		unsigned char m_ucEncryptBak[ENCRYPT_PWD_LENGTH+1];
		long long m_lPause[3];
		struct 
		{
			int m_s32VoDev;
			TRect m_TVoRect;
			double m_dSpeed;
		}m_PlayBack;
	}TPlaySingel[MAXPLAYER];
}TPlayContrl;
static TPlayContrl g_TPlayContrl;
/////////////性能限制结构体/////////
typedef struct _limit
{	
	struct 
	{
		int m_iRate;
		int m_iSize;
	}TFile[MAXPLAYER];
	struct
	{
		int m_iPicLimitFlag;
		int m_iPicLimitChannel;		
	}m_TPicLimit;
	int m_iSpeedLimit;
}Tlimit;
static volatile Tlimit g_Tlimit = {
	.TFile = {
		{0,0},{0,0},{0,0},{0,0},
		{0,0},{0,0},{0,0},{0,0},
		{0,0},{0,0},{0,0},{0,0},
		{0,0},{0,0},{0,0},{0,0},	
	}
};
/////////END///////////////////
////%%%%%%%% END %%%%%%%%%////

static volatile int g_iReady[MAXPLAYER] = {0};
static volatile int g_OpenNextFileFlag[MAXPLAYER] = {0};
static int g_iAsyOrsynFlag = 0;//同步标志

////%%%%%%%%同步回放变量%%%%%%%%%////
TsynPlayBack *g_synPlayBk = NULL;
typedef struct 
{
	volatile int m_asyStep;
	volatile int m_asyStop;
	long long m_AsySkip;
	int m_asypause;
	int m_asyStart;
	int m_asySpeed;
	int m_asySpeedEx;
	int m_asyResume;
	int m_RecEncrypt;

	td_u32	m_u32ReqStartTime;//在同步播放中的请求播放的起始时间
	td_u32	m_u32ReqStopTime;//在同步播放中请求播放的结束时间
}TAsyContrl;
static TAsyContrl gAsyContrl = {
	.m_asySpeedEx = 1000
};
typedef struct
{
	int m_iMinPlaynum;
	int m_iAudioPlay;
	int m_SegNum;
	int m_SegTempMin;
	int m_SegNumMin;
	int m_RealRunCnt;
	int m_OverPlay;
}TAsyPlay;
////%%%%%%%%END%%%%%%%%%////


pthread_mutex_t g_encryptlock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t g_TFileLock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t	g_CpStatsLock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t g_PlayBkLock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t g_PlayBkAsyLock = PTHREAD_MUTEX_INITIALIZER;

static int GetWaitTime(IN int _iPlayid);
static int DoneCmd(int _iPlayid);
void *PlayThread(IN int *_playid);
static int Playback_SpeedConversion(double *_dSpeed,int *_iSpeed,int flag);
static int FileSpeedContrl(int _iPlayid);
static int Playback_SetCurrentFTime(int _iPlayid, int _ibaseTime);
static int FileListCheckup(int _iPlayid);
static td_s32 Playback_Sync_ReBuild_FileList(td_s32 _s32VoChn);
void *PlayAsyThread(IN int *_num);
int PlayBackStatContrl(TPlayerStat *_ptStat);
int Playback_Sync_Esc(void);//同步回放结束
static int  Playback_StopCD(int _iPlayid);
static int PlayBackOpenerr(int _iPlayid, int _iFlag);
static int Playback_SeekFile(IN int _iPlayid, IN int _iSeekTm);//查找文件
static int Playback_SetTFileValue(int _iPlayid, int _iHeight, int _iWith, int _iRate);
static int Playback_ForceDestroyPlayer(IN int _iPlayid);
static int  Playback_EzoomNolock(int _iHandle);
static int  Playback_ExitEzoomNolock();
static int  Playback_AdjustEzoomNolock(TRect * _pRect);
static int Playback_SetFullScreenNolock(int _iPlayid);

static volatile PLAYBACK_STAT g_iStat = PLAYBACK_STAT_INIT;

void PlaybackSetMaxDecode(td_s32 _iMaxDecode)
{
#define PLAY_HIGHLIMIT	(20 * 100)
#define PLAY_MIDLIMIT		(8 * 100)
#define PLAY_SMALLIMIT	(2 * 100)
	if(_iMaxDecode >= PLAY_HIGHLIMIT)
	{
		g_iMaxDecode =  _iMaxDecode;
		g_iMaxEncodeCostRemain = 8 * 130;
		g_iMaxEncodeSpeedCostRemain = 130 * 30 * 4;
	}
	else if(_iMaxDecode >= PLAY_MIDLIMIT)
	{
		g_iMaxDecode =  _iMaxDecode;
		g_iMaxEncodeCostRemain = 4 * 130;
		g_iMaxEncodeSpeedCostRemain = 130 * 30 * 4;
	}
	else if(_iMaxDecode >= PLAY_SMALLIMIT)
	{
		g_iMaxDecode =  _iMaxDecode;
		g_iMaxEncodeCostRemain = 1 * 130;
		g_iMaxEncodeSpeedCostRemain = 130 * 30 * 1;
	}
#undef PLAY_HIGHLIMIT
#undef PLAY_MIDLIMIT
#undef PLAY_SMALLIMIT
	return ;
}
void Playback_InitLimit(void)
{
	g_Tlimit.m_TPicLimit.m_iPicLimitFlag = 1;
	g_Tlimit.m_TPicLimit.m_iPicLimitChannel = 0;
	g_Tlimit.m_iSpeedLimit = 0;
	return;
}

void Playback_ClearLimitFlag(int _iPicOrSpeedFlag)//uk调用
{

	if(PICLIMITFLAG == _iPicOrSpeedFlag)
	{	
		g_Tlimit.m_TPicLimit.m_iPicLimitFlag = 0;
		return ;
	}
	if(SPEEDLIMITFLAG == _iPicOrSpeedFlag)
	{
		g_Tlimit.m_iSpeedLimit = 0;
		return ;
	}
	if(PICANDSPEEDLIMITFLAG == _iPicOrSpeedFlag)
	{
		g_Tlimit.m_TPicLimit.m_iPicLimitFlag = 0;
		g_Tlimit.m_iSpeedLimit = 0;
		return ;
	}
}

void Playback_SetLimit(int _iPicOrSpeedFlag, int _iChannelNo)
{
	int iFlag = 0;
	switch(_iPicOrSpeedFlag)
	{
		case PICANDSPEEDLIMITFLAG:
			iFlag = 1;
		case PICLIMITFLAG:
			if(_iChannelNo >= 0 && _iChannelNo < 16)
			{
				SetChannelLimit(g_Tlimit.m_TPicLimit.m_iPicLimitChannel, _iChannelNo);
			}
			if(0 == iFlag)
			{
				break;
			}
		case SPEEDLIMITFLAG:
			g_Tlimit.m_iSpeedLimit = 1;
			if(0 == iFlag)
			{
				break;
			}
		default : ;
	}
	td_printf(0,"g_Tlimit.m_iSpeedLimit = %d,g_Tlimit.m_TPicLimit.m_iPicBefLimit = %d ,g_Tlimit.m_TPicLimit.m_iPicAfterLimit = %d",
		g_Tlimit.m_iSpeedLimit,g_Tlimit.m_TPicLimit.m_iPicLimitFlag,g_Tlimit.m_TPicLimit.m_iPicLimitFlag);
	return;
}

void Playback_TestLimit(int _iPicOrSpeedFlag, int *_piLimit)//uk调用
{
	int iLimitFlag = 0;
	int iFlag = 0;
	switch(_iPicOrSpeedFlag)
	{
		case PICANDSPEEDLIMITFLAG:
			iFlag = 1;
		case PICLIMITFLAG:
			if(0 != g_Tlimit.m_TPicLimit.m_iPicLimitChannel)
			{
				if(1 == g_Tlimit.m_TPicLimit.m_iPicLimitFlag)
				{
					iLimitFlag++;
				}
			}
			else
			{
				g_Tlimit.m_TPicLimit.m_iPicLimitFlag = 1;
			}
			if(0 == iFlag)
			{
				break;
			}
		case SPEEDLIMITFLAG:
			if(1 == g_Tlimit.m_iSpeedLimit)
			{
				iLimitFlag += 2;
			}
			if(0 == iFlag)
			{
				break;
			}
		default : ;
	}
	*_piLimit = iLimitFlag;
	return;
}


static int __CpStats(int _iPlayid)
{
	pthread_mutex_lock(&g_CpStatsLock);
	memcpy(&g_TPlayContrl.TPlaySingel[_iPlayid].m_RunStatBak,&g_PlayBkVo[_iPlayid]->m_RunStat,sizeof(TPlayerStat));
	pthread_mutex_unlock(&g_CpStatsLock);
	return 0;
}
static int ReGetBaseTime(IN int _iPlayid,unsigned int StampBase,long long SysBase)
{
	TPlayBack *Playback = g_PlayBkVo[_iPlayid];
	if(NULL == Playback)
	{
		return td_failure;
	}
	Playback->s_TimeStamp.m_lFrameTime = StampBase;
	Playback->s_TimeStamp.m_lForkTime = SysBase;	
	return 0;
}
//异步回放激活函数
//功能确定VO的现实区域，与建立VO的个数
int Playback_Active(int _s32VoDev, int _iVoCnt, TRect *_pVoRect)
{
	int i =0;
	if(_iVoCnt <= 0)
	{
		return -1;
	}
	if(_iVoCnt > MAXPLAYER)
	{
		_iVoCnt = MAXPLAYER;
	}
	g_iAsyOrsynFlag = 0;

	Playback_InitLimit();

	g_TPlayContrl.TPlayAll.m_iCnt = _iVoCnt;
	g_TPlayContrl.TPlayAll.m_iAudioChannal = -1;
	g_TPlayContrl.TPlayAll.m_FullPlay = -1;
	for(i = 0; i < MAXPLAYER; i++)
	{
		g_Tlimit.TFile[i].m_iRate = 0;
		g_Tlimit.TFile[i].m_iSize = 0;
	
		g_TPlayContrl.TPlaySingel[i].m_PlayBack.m_dSpeed = 1.0;
		g_TPlayContrl.TPlaySingel[i].m_PlayBack.m_s32VoDev = _s32VoDev;
		memcpy(&g_TPlayContrl.TPlaySingel[i].m_PlayBack.m_TVoRect, &_pVoRect[i], sizeof(TRect));

		g_TPlayContrl.TPlaySingel[i].m_ExternalInterface.m_dSpeedBak = 1;
		g_TPlayContrl.TPlaySingel[i].m_ExternalInterface.m_iRunFlagBak= PLAYBACK_STAT_INIT;
		g_TPlayContrl.TPlaySingel[i].m_ExternalInterface.m_iSkipPosBak= -1;
		g_TPlayContrl.TPlaySingel[i].m_ExternalInterface.iATrueFalseBak = 0;
		g_TPlayContrl.TPlaySingel[i].m_ExternalInterface.iEncrypt = 0;
		g_TPlayContrl.TPlaySingel[i].iShowFlag = 1;
		g_TPlayContrl.TPlaySingel[i].StopFlag = 0;
		g_TPlayContrl.TPlaySingel[i].m_Rate = 0;
		g_TPlayContrl.TPlaySingel[i].m_cFile[0] = 0;
		g_TPlayContrl.TPlaySingel[i].m_lPause[0] = 0;
		g_TPlayContrl.TPlaySingel[i].m_lPause[1] = 0;
		g_TPlayContrl.TPlaySingel[i].m_lPause[2] = 0;
		memset(g_TPlayContrl.TPlaySingel[i].m_ucEncryptBak, 0, ENCRYPT_PWD_LENGTH+1);

		memset(&g_TPlayContrl.TPlaySingel[i].m_RunStatBak, 0, sizeof(TPlayerStat));
	
	}
	//gAsyContrl.m_asySpeed = 0;

	decode_EnableBuf();
	return 0;
}

static int playback_InitSegmet(int _iPlayid)
{	
	TPlayBack *Playback = g_PlayBkVo[_iPlayid];
	if(NULL == Playback)
	{
		return td_failure;
	}

	pthread_mutex_init(&Playback->AudioLock,NULL);
	Playback->m_RunStat.m_iPid = _iPlayid;
	Playback->m_RunStat.m_iStat = PLAYBACK_STAT_INIT;
	Playback->m_RunStat.m_iSpeed = 1;
	Playback->m_RunStat.m_iFinishFileFlag = 0;
	Playback->m_RunStat.m_iFullScreen = 0;
	Playback->m_RunStat.m_tCurPlayTm = 0;
	Playback->m_RunStat.m_iTimeSeg = 0;//时间进度
	Playback->m_RunStat.m_iBlackScreen = -1;
	Playback->m_RunStat.m_iRate = 0;
	Playback->m_RunStat.iATrueFalse = 0;

	Playback->m_pDc.m_Stream.m_u16BefAudioFrames = MAXAUDIOFRAMES;
	Playback->m_pDc.m_Stream.m_u16ZeroAudioFrames = 0;
	Playback->m_pDc.m_Stream.m_vdecPara.m_iHeight = 0;
	Playback->m_pDc.m_Stream.m_vdecPara.m_iWidth = 0;
	Playback->m_pDc.m_Stream.m_id = _iPlayid;

	td_printf(0, "1 m_ucEncryptBak:%s, m_ucEncrypt:%s\n", g_TPlayContrl.TPlaySingel[_iPlayid].m_ucEncryptBak, Playback->m_pDc.m_Stream.m_ucEncrypt);
	strncpy((char *)Playback->m_pDc.m_Stream.m_ucEncrypt, (char *)g_TPlayContrl.TPlaySingel[_iPlayid].m_ucEncryptBak, ENCRYPT_PWD_LENGTH);
	td_printf(0, "2 m_ucEncryptBak:%s, m_ucEncrypt:%s\n", g_TPlayContrl.TPlaySingel[_iPlayid].m_ucEncryptBak, Playback->m_pDc.m_Stream.m_ucEncrypt);
	Playback->m_RunStat.m_dSpeed = 1; 
	Playback->m_RunStat.m_iNeedSecret = 0;
	Playback->m_ptFileBeginTm = 0;
	Playback->m_ptFileEndTm = 0;
	Playback->m_u32UpFrameSTime = 0xFFFFFFFF;
	Playback->m_iSkipPos= -1;
	Playback->m_iPlayILocation = 0;
	Playback->iTimeChange = 1;
	Playback->iHasBind = 1;
	Playback->m_iImperTm = 0;	
	Playback->m_iOlnyReadIFrame = 0;
	Playback->m_uiTimePot = 0;//记录的时间点
	Playback->m_dBefSpeed = 1;	
	Playback->m_fSpeedValue = 1.0;
	Playback->m_u32TimeStamp = 0;
	Playback->s_TimeStamp.m_lFrameTime = 0;
	Playback->s_TimeStamp.m_lForkTime = 0;
	Playback->m_AudioFlag = 0;
	Play_Clr_All(Playback->m_iOptStat);
	memcpy(&g_TPlayContrl.TPlaySingel[_iPlayid].m_RunStatBak,&Playback->m_RunStat,sizeof(TPlayerStat));
	g_TPlayContrl.TPlaySingel[_iPlayid].m_PlayBack.m_dSpeed = 1.0;	
	g_TPlayContrl.TPlaySingel[_iPlayid].m_ExternalInterface.m_iRunFlagBak = PLAYBACK_STAT_INIT;
	g_TPlayContrl.TPlaySingel[_iPlayid].m_ExternalInterface.m_dSpeedBak = 1;
	g_TPlayContrl.TPlaySingel[_iPlayid].m_ExternalInterface.m_iSkipPosBak = -1;
	g_TPlayContrl.TPlaySingel[_iPlayid].StopFlag = 0;
	g_TPlayContrl.TPlaySingel[_iPlayid].m_lPause[0] = 0;
	g_TPlayContrl.TPlaySingel[_iPlayid].m_lPause[1] = 0;
	g_TPlayContrl.TPlaySingel[_iPlayid].m_lPause[2] = 0;

	return 0;
}

int Playback_WaitUinit(int _iPlayid, int _iWaitTime, int _iClearFlag)
{
	int i = 0;
	int iFlag = 0;
	long long LPreTime = 0;
	long long LTime = 0;
	int iRelayTime = _iWaitTime;
	int iStart = 0;
	int iEnd = 0;
	decode_GetSysTime(&LTime);
	LPreTime = LTime;
	if(_iPlayid < 0)
	{
		iStart = 0;
		iEnd = MAXPLAYER;
	}
	else
	{
		iStart = _iPlayid;
		iEnd = _iPlayid + 1;
	}
	while(1)
	{
		for(i = iStart; i < iEnd; i++)
		{
			if(g_PlayBkVo[i])
			{
				iFlag |= 1<<i;
				Playback_Stop(i);			
			}
			else
			{
				iFlag &= ~(1<<i);
			}
		}
		if(iFlag)
		{
			usleep(1000);
		}
		decode_GetSysTime(&LTime);
		if(LPreTime < LTime)
		{
			iRelayTime -= (int)(LTime - LPreTime);
		}
		LPreTime = LTime;
		if(iRelayTime <= 0 || (!iFlag))
		{
			break;
		}
	}

	if(_iClearFlag)
	{
		for(i = iStart; i < iEnd; i++)
		{
			if(iFlag & (1 << i))
			{
				Playback_ForceDestroyPlayer(i);
			}
		}
	}
	return iFlag;
}
	
//功能:初始化播放器所需变量
//返回值:0:		成功
//			负数:	失败
int Playback_Init(int _iPlayid)
{
	if(_iPlayid >= g_TPlayContrl.TPlayAll.m_iCnt || _iPlayid < 0)
	{
		return -1;
	}
	
	if(Playback_WaitUinit(_iPlayid, 10 * 1000 * 1000, 1))
	{
		return -1;
	}
	
	g_PlayBkVo[_iPlayid] = (TPlayBack *)malloc(sizeof(TPlayBack));//分配回放所需要的存储空间
	if(g_PlayBkVo[_iPlayid] == NULL)
	{
		td_printf(0,"malloc g_PlayBkVo fail   %d",_iPlayid);
		return -2;//分配变量空间错误	
	}
	td_printf(0,"playback_init decode init playid = %d",_iPlayid);
	if(decode_Init(&g_PlayBkVo[_iPlayid]->m_pDc, 
		(int *)&g_TPlayContrl.TPlaySingel[_iPlayid].m_ExternalInterface.m_iRunFlagBak) == -1)
	{	
		td_printf(0,"malloc m_pDc fail   %d",_iPlayid);
		free(g_PlayBkVo[_iPlayid]);
		g_PlayBkVo[_iPlayid] = NULL;
		return -2;
	}
	playback_InitSegmet(_iPlayid);//初始化参数
	return 0;
}

extern int tvdec_VideoDimToSize(int _iWith, int _iHeight);
extern int  g_fVSizeDec[TVMAX+1];
int Playback_GetVideoSize(int _iWith, int _iHeight)
{
	if(1088 == _iWith && 1920 == _iHeight)
	{
		_iWith = 1080;
	}
	else if(1920 == _iWith && 1088 == _iHeight)
	{
		_iHeight = 1080;
	}
	int iSize1 = tvdec_VideoDimToSize(_iWith, _iHeight);
	int iSize2 = tvdec_VideoDimToSize(_iHeight, _iWith);
	int fVsize1;
	int fVsize2;

	fVsize1 = iSize1 >= TVMAX ? 800 :g_fVSizeDec[iSize1+1];
	fVsize2 = iSize2 >= TVMAX ? 800 :g_fVSizeDec[iSize2+1];
	if(fVsize1 > fVsize2)
	{
		return fVsize2;
	}
	return fVsize1;
}

static int Playback_SetTFileValue(int _iPlayid, int _iHeight, int _iWith, int _iRate)
{
	int iSize = 0;
	if(_iPlayid < 0 || _iPlayid >=MAXPLAYER)
	{
		return td_failure;
	}
	if(_iHeight != 0 && _iWith != 0)
	{
		iSize = Playback_GetVideoSize(_iWith, _iHeight);
	}
	pthread_mutex_lock(&g_TFileLock);
	if(-1 == _iHeight&& -1 == _iWith)
	{
		g_Tlimit.TFile[_iPlayid].m_iRate = _iRate;
	}
	else
	{
		g_Tlimit.TFile[_iPlayid].m_iRate = _iRate;
		g_Tlimit.TFile[_iPlayid].m_iSize = iSize;
	}
	pthread_mutex_unlock(&g_TFileLock);
	return 0;
}


static int Playback_GetTFileValue(IN int _iPlayid, OUT int *_piSize, OUT int *_piRate)
{
	int i = 0;
	pthread_mutex_lock(&g_TFileLock);
	if(_iPlayid < 0 || _iPlayid >=MAXPLAYER)
	{
		for(i = 0; i < MAXPLAYER; i++)
		{
			_piRate[i] = g_Tlimit.TFile[i].m_iRate;
			_piSize[i] = g_Tlimit.TFile[i].m_iSize;	
		}
	}
	else
	{
		*_piRate = g_Tlimit.TFile[_iPlayid].m_iRate;
		*_piSize = g_Tlimit.TFile[_iPlayid].m_iSize;		
	}
	pthread_mutex_unlock(&g_TFileLock);
	return 0;
}


	
//_iSpeedPressure:传回带帧率与播放速度的压力
static int Playback_CalPressure(int _iPlayid, int *_iSpeedPressure, int *_iPressure, int _iSpeedX10)
{
	int i = 0;
	int temp = 0;
	int tempfour = 0;
	int iSizeTemp = 0;
	//int iMaxSpeed = 0;
	int iRate[MAXPLAYER] = {0};
	int iSize[MAXPLAYER] = {0};
	double dSpeed = 0;
	double dSpeedNumCIF = 0;
	double dNumCIF = 0;
	int EncodeCost = chnmnt_ChannelEncodePerformanceCost();
	int EncodeCostTmp = EncodeCost;
	Playback_GetTFileValue(-1, iSize, iRate);
	for(i = 0; i < MAXPLAYER; i++)
	{
		if(_iPlayid == i && _iSpeedX10 != 0)
		{
			dSpeed = _iSpeedX10 * 1.0 / 10;
			if(iSize[i] >= 75 && (int)dSpeed == SpeedFastEight/10)
			{
				iSizeTemp++;
			}
			
		}
		else if(_iPlayid == -1)
		{
			dSpeed = _iSpeedX10 * 1.0 / 10;
		}
		else
		{
			dSpeed = g_TPlayContrl.TPlaySingel[i].m_ExternalInterface.m_dSpeedBak;
		}
		
		if(dSpeed > 15)
		{
			if(iSize[i] <= PLAYPFRAMERATE)
			{
				dSpeedNumCIF += dSpeed * iSize[i] * iRate[i] ;
			}
			dSpeed = 1.0;
		}
		else
		{
			dSpeedNumCIF += dSpeed * iSize[i] * iRate[i] ;
		}
		
		dNumCIF += iSize[i];

		if(iSize[i] != 0)
		{
			if((int)g_TPlayContrl.TPlaySingel[i].m_ExternalInterface.m_dSpeedBak == (SpeedFastEight/10) || (int)dSpeed == (SpeedFastEight/10))
			{
				temp++;
				if(iSize[i] >= 100)
				{
					tempfour++;
				}
			}
			else if((int)g_TPlayContrl.TPlaySingel[i].m_ExternalInterface.m_dSpeedBak == (SpeedFastFour/10) || (int)dSpeed == (SpeedFastFour/10))
			{
				if(iSize[i] >= 100)
				{
					tempfour++;
				}
				
			}

		}
			//td_printf(0,"*_iPressure = %d, %s, %d",*_iPressure, __func__, __LINE__);
		//td_printf(0,"dSpeed(%d) = %f",i,dSpeed);
	}


	EncodeCostTmp = (EncodeCost > MAXENCODECOST ? MAXENCODECOST : EncodeCost);
	*_iPressure = (int)(dNumCIF) + EncodeCostTmp;
	//td_printf(0,"*_iPressure = %d, (int)(dNumCIF) = %d, EncodeCostTmp = %d, %s, %d",*_iPressure, (int)(dNumCIF), EncodeCostTmp, __func__, __LINE__);
	EncodeCostTmp = (EncodeCost * 25 > MAXENCODESPEEDCOST ? MAXENCODESPEEDCOST : EncodeCost * 25 );
	*_iSpeedPressure = (int)(dSpeedNumCIF) + EncodeCostTmp;

//以下代码是通过实际测试所得
//在DVR时控制6倍速只能有一个画面，但是此画面不能2cif
//在NVR时控制6倍速不能超过4个画面
	if(EncodeCostTmp > 400)
	{
		if(temp > 0 && iSizeTemp > 0)
		{
			*_iSpeedPressure = MAX_DECODE_SRATE + 1;
		}
		else if(temp > 1)
		{
			*_iSpeedPressure = MAX_DECODE_SRATE + 1;
		}
		else if(tempfour >= 4)
		{
			*_iSpeedPressure = MAX_DECODE_SRATE + 1;
		}
	}
	else
	{
		if(temp > 4)
		{
			*_iSpeedPressure = MAX_DECODE_SRATE + 1;
		}
	}
	//td_printf(0,"_iPressure = %d,  *_iSpeedPressure = %d",*_iPressure,*_iSpeedPressure);
	return 0;
}

static int Playback_SetCurrentFile(int _iPlayid, int iFileCurrent)
{
	pthread_mutex_lock(&g_PlayBkAsyLock);
	if(g_synPlayBk)
	{
		g_synPlayBk->m_TFileListMsg[_iPlayid].m_iFileCurrent = iFileCurrent;
	}
	pthread_mutex_unlock(&g_PlayBkAsyLock);
	return 0;
}

static int Playback_Get_DownFile(int _iPlayid, int *_piFileCurrent, int *_piDownFile)
{
	int iFileCurrent = 0;
	int iFileNum = 0;
	pthread_mutex_lock(&g_PlayBkAsyLock);
	if(g_synPlayBk)
	{
		iFileCurrent = g_synPlayBk->m_TFileListMsg[_iPlayid].m_iFileCurrent;
		iFileNum = g_synPlayBk->m_TFileListMsg[_iPlayid].m_iFileNum;
	}
	pthread_mutex_unlock(&g_PlayBkAsyLock);
	*_piFileCurrent = iFileCurrent;
	if(iFileCurrent  < iFileNum)
	{
		iFileCurrent++;
	}

	if(iFileCurrent >= iFileNum)
	{
		return -1;
	}

	*_piDownFile = iFileCurrent;
	return 0;
}


//-1:文件已经是结束
//非负数表示列表中文件的位置
static int Playback_NextFile(int _iPlayid, int *_piFileCurrent)
{
	int iFileCurrent = 0;
	int iDownFile = 0;

	if(Playback_Get_DownFile(_iPlayid, &iFileCurrent, &iDownFile) == 0)
	{
		Playback_SetCurrentFile(_iPlayid, iDownFile);
		*_piFileCurrent = iFileCurrent;
		return iDownFile;
	}
	*_piFileCurrent = iFileCurrent;
	return -1;
}

static int Playback_Test_File(int _iPlayid, int _iCurrentFile)
{
	int iRet = 0;
	TFileListPlay *pFileListPlay;
	pthread_mutex_lock(&g_PlayBkAsyLock);
	if(g_synPlayBk == NULL)
	{
		iRet = -1;
		goto leave;
	}
	pFileListPlay = &g_synPlayBk->m_TFileListMsg[_iPlayid].m_pFileListPlay[_iCurrentFile];
	iRet = pFileListPlay->m_NULL;
leave:
	pthread_mutex_unlock(&g_PlayBkAsyLock);
	return iRet;
}

static int Playback_Do_OverFile(int _iPlayid)
{
	TPlayBack *Playback = g_PlayBkVo[_iPlayid];
	if(NULL == Playback)
	{
		return td_failure;
	}
	Playback->m_pDc.m_Stream.m_iFindNoIndex = 0;
	Playback->m_pDc.m_Stream.m_iFileStartTime = 0;
	Playback->m_pDc.m_Stream.m_iFileEndTime = 0;
	//全部文件已经操作完成
	Playback->m_pDc.CloseFile(&Playback->m_pDc.m_Stream);
	Playback->m_RunStat.m_iFinishFileFlag = 0;//将文件读完标志清零
	Playback->m_pDc.VoContrl(&Playback->m_pDc.m_Stream,VO_HIDE);
	Playback->m_pDc.VoContrl(&Playback->m_pDc.m_Stream,VO_CLEAR);
	g_TPlayContrl.TPlaySingel[_iPlayid].StopFlag = 1;
	Playback->m_RunStat.m_iBlackScreen = 1;
	Playback->m_RunStat.m_iRate = 0;
	Playback_SetCurrentFTime(_iPlayid, (gAsyContrl.m_u32ReqStopTime - gAsyContrl.m_u32ReqStartTime) * 1000);
	//Playback->m_pDc.DestroyVo(&Playback->m_pDc.m_Stream);
	if(g_TPlayContrl.TPlayAll.m_FullPlay == _iPlayid)
	{
		Playback_SetFullScreenNolock(_iPlayid);
	}
	Playback->m_pDc.DestroyVdec(&Playback->m_pDc.m_Stream);
	Playback_SetTFileValue(_iPlayid, 0, 0, 0);
	__CpStats(_iPlayid);

	return 0;
}
static int Playback_Do_BlankFile(int _iPlayid, int _iCurrentFile)
{
	time_t iEndTime = 1;
	TFileListPlay *pFileListPlay;
	TPlayBack *Playback = g_PlayBkVo[_iPlayid];
	if(NULL == Playback)
	{
		return td_failure;
	}
	pthread_mutex_lock(&g_PlayBkAsyLock);
	if(g_synPlayBk == NULL)
	{
		pthread_mutex_unlock(&g_PlayBkAsyLock);
		return -1;
	}
	pFileListPlay = &g_synPlayBk->m_TFileListMsg[_iPlayid].m_pFileListPlay[_iCurrentFile];
	iEndTime = pFileListPlay->m_iEndTime;
	pthread_mutex_unlock(&g_PlayBkAsyLock);
	
	pthread_mutex_lock(&g_PlayBkLock);
	if(g_EzoomHand == _iPlayid)
	{
		g_EzoomHand += MAXPLAYER;
		Playback_ExitEzoomNolock();
	}
	pthread_mutex_unlock(&g_PlayBkLock);
	
	Playback->m_pDc.m_Stream.m_iFindNoIndex = 0;
	Playback->m_pDc.m_Stream.m_iFileStartTime = 0;
	Playback->m_pDc.m_Stream.m_iFileEndTime = 0;
	Playback->m_pDc.CloseFile(&Playback->m_pDc.m_Stream);
	

	Playback->m_RunStat.m_iFinishFileFlag = 0;//将文件读完标志清零
	
	Playback->m_pDc.VoContrl(&Playback->m_pDc.m_Stream,VO_HIDE);
	Playback->m_pDc.VoContrl(&Playback->m_pDc.m_Stream,VO_CLEAR);
	
	Playback->iTimeChange = 1;
	Playback->m_RunStat.m_iBlackScreen = 1;//黑屏
		
	//黑屏的时候将文件进程跳转到空白文件的最后
	Playback_SetCurrentFTime(_iPlayid, (iEndTime - gAsyContrl.m_u32ReqStartTime) * 1000);
	Playback->m_RunStat.m_iRate = 0;
	//Playback->m_pDc.DestroyVo(&Playback->m_pDc.m_Stream);
	if(g_TPlayContrl.TPlayAll.m_FullPlay == _iPlayid)
	{
		Playback_SetFullScreenNolock(_iPlayid);
	}
	Playback->m_pDc.DestroyVdec(&Playback->m_pDc.m_Stream);
	Playback_SetTFileValue(_iPlayid, 0, 0, 0);
	__CpStats(_iPlayid);

	return 0;
}

// 名称：playback_SetPlayeFile
// 功能：设置回放器播放文件，调用此接口将导致回放器重置到初始状态
// 参数：
//	_pPlayBack	:回放器句柄
//	_strFile		:录像文件名（全路径）
//	_ptBeginTm	:指示此录像文件的开始时间
//	_ptEndTm		:指示此录像文件的结束时间
// 返回值：0成功能，负数表失败
int Playback_SetPlayeFile(IN int _iPlayid, IN char *_strFile, IN time_t _iBeginTm, IN time_t _iEndTm, IN int _iFileNum)
{
	int iRate = 0;
	int iRet = 0;
	//TFileListPlay *pFileListPlay;
	unsigned short usHeightTemp = 0;
	unsigned short usWithTemp = 0;
	int iSpeedPressure = 0;
	int iPressure = 0;
	S_FileHeader fileHeader;
	if(_iPlayid < 0)
	{
		return -1;//传递的参数有误
	}

	TPlayBack *Playback = g_PlayBkVo[_iPlayid];
	if(NULL == Playback)
	{
		return td_failure;
	}	
	//g_FileChangeTime[_iPlayid] = 0;
 
	Playback->m_pDc.m_Stream.m_iFindNoIndex = 0 ;
	Playback->m_pDc.m_Stream.m_iFileStartTime = 0;
	Playback->m_pDc.m_Stream.m_iFileEndTime = 0;

	if( (int)(Playback->m_RunStat.m_dSpeed) == MAXSPEED)
	{
		Playback->iTimeChange = 1;
	}
	Playback->m_RunStat.m_iFinishFileFlag = 0;//将文件读完标志清零
		
	if(_iBeginTm > 0 && _iEndTm > 0)
	{
		if(_iEndTm <= _iBeginTm)
		{
			return -1;//传递的文件开始结束时间有误
		}
		Playback->m_ptFileBeginTm = _iBeginTm;
		Playback->m_ptFileEndTm = _iEndTm;
	}
	else
	{
		
		if(_strFile[0] == 'C' && (_strFile[1] >= '0' && _strFile[1] <= '1') )
		{
			rec_GetFileStartTime((td_s8*)(&_strFile[2]), (time_t*)&Playback->m_ptFileBeginTm);
		}
		else
		{
			rec_GetFileStartTime((td_s8*)_strFile, (time_t*)&Playback->m_ptFileBeginTm);
		}
		
		rec_GetFileStopTime((td_s8*)_strFile, (time_t*)&Playback->m_ptFileEndTm);
	}

	Playback->m_RunStat.m_iBlackScreen = 0;
	td_printf(0,"_strFile = %s" , _strFile);
	Playback->m_pDc.m_Stream.m_id = _iPlayid;
	//decode_OpenFile
	if(Playback->m_pDc.OpenFile(&Playback->m_pDc.m_Stream,(td_s8 *)_strFile) < 0)
	{
		Playback->m_ptFileBeginTm = 0;
		Playback->m_ptFileEndTm = 0;
		return FILE_ERR;
	}

	td_printf(0x12345678, "the file[%s] begin time is %d", Playback->m_pDc.m_Stream.m_File,Playback->m_ptFileBeginTm);
	td_printf(0x12345678, "the file[%s] end   time is %d", Playback->m_pDc.m_Stream.m_File,Playback->m_ptFileEndTm);

	Playback->m_pDc.m_Stream.m_iFileStartTime = Playback->m_ptFileBeginTm;
	Playback->m_pDc.m_Stream.m_iFileEndTime = Playback->m_ptFileEndTm;
	//decode_GetFileHeader
	Playback->m_pDc.GetFileHeader(&Playback->m_pDc.m_Stream, &fileHeader);
	{
		double dTimeLenTemp1,dTimeLenTemp2,dTimeLenTemp3;
		int iTimeLen1, iTimeLen2, iTimeLen3;
		dTimeLenTemp1 = fileHeader.FrameCount * 1.0 / fileHeader.FrameRate;
		dTimeLenTemp2 = fileHeader.FrameCount * 1.0 / 1;
		dTimeLenTemp3 = fileHeader.FrameCount * 1.0 / 30;
		iTimeLen1 = Playback->m_ptFileBeginTm + (int)dTimeLenTemp1;
		iTimeLen2 = Playback->m_ptFileBeginTm + (int)dTimeLenTemp2;
		iTimeLen3 = Playback->m_ptFileBeginTm + (int)dTimeLenTemp3;
		if(iTimeLen2 + 3 * 60 * 60 <  Playback->m_ptFileEndTm
		//||iTimeLen > Playback->m_ptFileEndTm + 2 * 60 * 60
		||iTimeLen3 > Playback->m_ptFileEndTm + 3 * 60 * 60
		||Playback->m_ptFileEndTm <= Playback->m_ptFileBeginTm)
		{
			if(fileHeader.FrameCount)
			{
				Playback->m_ptFileEndTm = iTimeLen1;
				td_printf(0,"%s,%d,iTimeLen = %d",__func__,__LINE__,iTimeLen1);
				Playback->m_pDc.m_Stream.m_iFileEndTime = iTimeLen1;
			}
			else
			{
				Playback->m_ptFileEndTm = Playback->m_ptFileBeginTm + 10 * 60;
				Playback->m_pDc.m_Stream.m_iFileEndTime = iTimeLen1+ 10 * 60;
			}
			Playback->m_pDc.m_Stream.m_iFindNoIndex = 1;
		}
	}
	//decode_GetRate
	Playback->m_RunStat.m_iRate = Playback->m_pDc.GetRate(&Playback->m_pDc.m_Stream,1,0);
	//decode_GetFileAttribute
	Playback->m_pDc.GetFileAttribute(&Playback->m_pDc.m_Stream,&Playback->m_AudioFlag,4);
	Playback->m_pDc.GetFileAttribute(&Playback->m_pDc.m_Stream,&usHeightTemp,2);
	Playback->m_pDc.GetFileAttribute(&Playback->m_pDc.m_Stream,&usWithTemp,1);

	//decode_CreateVo
	if(Playback->m_pDc.CreateVo(&Playback->m_pDc.m_Stream,&g_TPlayContrl.TPlaySingel[_iPlayid].m_PlayBack.m_TVoRect,g_TPlayContrl.TPlaySingel[_iPlayid].m_PlayBack.m_s32VoDev) < 0)
	{
		td_printf(0,"CreateVo err\n");
		iRet = VOVDEC_ERR;
		goto leave;
	}
	//decode_CreateVdec
	if(Playback->m_pDc.CreateVdec(&Playback->m_pDc.m_Stream) < 0)
	{
		td_printf(0,"CreateVdec err\n");
		Playback_SetLimit(PICLIMITFLAG, _iPlayid);
		iRet = VOVDEC_ERR;
		goto leave;
	}

	Playback_SetTFileValue(_iPlayid, usHeightTemp, usWithTemp, Playback->m_RunStat.m_iRate);
	Playback_CalPressure(_iPlayid,&iSpeedPressure, &iPressure, 0);
	if(iPressure > MAX_DECODE)
	{
		Playback_SetLimit(PICLIMITFLAG, _iPlayid);
		//PlayBackOpenerr(_iPlayid, VOVDEC_ERR);
		iRet = VOVDEC_ERR;
		goto leave;
	}
	
	ClearChannelLimit(g_Tlimit.m_TPicLimit.m_iPicLimitChannel, _iPlayid);
	Playback->m_pDc.m_Stream.m_u16BefAudioFrames = MAXAUDIOFRAMES;
	Playback->m_pDc.m_Stream.m_u16ZeroAudioFrames = 0;

	if((int)(Playback->m_RunStat.m_dSpeed *10) >= 10)
	{
		//decode_GetRate
		iRate = Playback->m_pDc.GetRate(&Playback->m_pDc.m_Stream,Playback->m_RunStat.m_dSpeed,2);//
	}
	else if((int)(Playback->m_RunStat.m_dSpeed * 10) < -10)
	{
		iRate = Playback->m_pDc.GetRate(&Playback->m_pDc.m_Stream,0 -1.0/Playback->m_RunStat.m_dSpeed,2);//
	}

	if( (int)(Playback->m_RunStat.m_dSpeed) != MAXSPEED)
	{
		g_TPlayContrl.TPlaySingel[_iPlayid].m_Rate =  iRate;
	}
		
	if(g_TPlayContrl.TPlaySingel[_iPlayid].m_Rate <= 0)
	{
		g_TPlayContrl.TPlaySingel[_iPlayid].m_Rate = 1;
	}
	Playback->iSpeed16Rate = g_TPlayContrl.TPlaySingel[_iPlayid].m_Rate;
	Playback->m_pDc.SetRate(&Playback->m_pDc.m_Stream,g_TPlayContrl.TPlaySingel[_iPlayid].m_Rate, -2);
	td_printf(0,"g_TPlayContrl.TPlaySingel[_iPlayid].m_Rate = %d", g_TPlayContrl.TPlaySingel[_iPlayid].m_Rate);
	strcpy(g_TPlayContrl.TPlaySingel[_iPlayid].m_cFile,_strFile);
	td_printf(0,"g_TPlayContrl.TPlaySingel[_iPlayid].m_cFile = %s",g_TPlayContrl.TPlaySingel[_iPlayid].m_cFile);
	__CpStats(_iPlayid);
	pthread_mutex_lock(&g_PlayBkLock);

	if(g_EzoomHand == _iPlayid + MAXPLAYER && g_iAsyOrsynFlag)
	{
		if(td_success == Playback_EzoomNolock(_iPlayid))
		{
			g_EzoomHand -= MAXPLAYER;
			Playback_AdjustEzoomNolock(NULL);
		}
	}
	else if(g_TPlayContrl.TPlayAll.m_FullPlay == _iPlayid)
	{
		Playback_SetFullScreenNolock(_iPlayid);
	}
	else if(g_TPlayContrl.TPlaySingel[_iPlayid].iShowFlag == 1)
	{
		td_printf(0,"show %d\n",_iPlayid);
		Playback->m_pDc.VoContrl(&Playback->m_pDc.m_Stream,VO_SHOW);
	}

	pthread_mutex_unlock(&g_PlayBkLock);
	return iRet;
leave:
	Playback->m_pDc.DestroyVdec(&Playback->m_pDc.m_Stream);
	Playback_SetTFileValue(_iPlayid, 0, 0, 0);
	return  iRet;

}
// 名称：playback_CreatePlayer
// 功能：创建一个回放器，构建回放所需资源，绑定解码器与VO显示关系，回放器处于初始就绪状态
// 参数：
//	_ptVo	:指定显示区域
// 返回值：负值表失败，非负表回放器句柄，后续回放操作均使用此句柄
int g_PBPthread[MAXPLAYER] = {0};
int Playback_CreatePlayer(IN int _iPlayid)
{
	int iRet = -1;
	pthread_t id;
	if(_iPlayid < 0 || _iPlayid >= MAXPLAYER)
	{
		td_printf(0,"%s,%d, segment is err %d",__func__, __LINE__,_iPlayid);
		return td_failure;
	}

	TPlayBack *Playback = g_PlayBkVo[_iPlayid];
	if(NULL == Playback)
	{
		return td_failure;
	}

	g_PBPthread[_iPlayid] = _iPlayid;
	Playback->m_pDc.m_Stream.m_id = _iPlayid;
	//创建线程		
	iRet = pthread_create(&id,NULL,(void *)PlayThread,(void *)&g_PBPthread[_iPlayid]);
	if(iRet != 0)
	{
		Playback_DestroyPlayer(_iPlayid);
		return td_failure;
	}
	if(0 == g_iAsyOrsynFlag)
	{
		Playback_InitLimit();
		Playback_SetCurrentFTime(_iPlayid, 0);
	}
	
	pthread_detach(id);
	#if 0
	pthread_mutex_init(&g_CutLoctionLock[_iPlayid],NULL);
	#endif
	return _iPlayid;
}

static int Playback_ForceDestroyPlayer(IN int _iPlayid)
{
	TPlayBack *Playback = g_PlayBkVo[_iPlayid];
	pthread_mutex_lock(&g_PlayBkLock);
	if(!Playback)
	{
		pthread_mutex_unlock(&g_PlayBkLock);
		return 0;
	}
	if(g_EzoomHand == _iPlayid || g_EzoomHand == (_iPlayid + MAXPLAYER))
	{
		Playback_ExitEzoomNolock();
		g_EzoomHand = -1;
	}
	Playback->m_pDc.ForceDestroy(&Playback->m_pDc);
	pthread_mutex_unlock(&g_PlayBkLock);
	return 0;
}
int Playback_DestroyPlayer(IN int _iPlayid)
{
	//	销毁播放器资源
	td_printf(0,"%s free playid = %d",__func__,_iPlayid);
	TPlayBack *Playback = g_PlayBkVo[_iPlayid];
	pthread_mutex_lock(&g_PlayBkLock);
	if(NULL == Playback)//如果播放器没有建立，则直接返回成功
	{
		pthread_mutex_unlock(&g_PlayBkLock);
		return 0;
	}

	if(g_EzoomHand == _iPlayid || g_EzoomHand == (_iPlayid + MAXPLAYER))
	{
		Playback_ExitEzoomNolock();
		g_EzoomHand = -1;
	}
	
	if(Playback->m_RunStat.m_iFullScreen == 1)
	{
		g_TPlayContrl.TPlayAll.m_FullPlay = _iPlayid;
	}
	
	Playback->m_pDc.Destroy(&Playback->m_pDc);
//	Playback_StopCD(_iPlayid);
	pthread_mutex_destroy(&Playback->AudioLock);
	Playback_SetTFileValue(_iPlayid, 0, 0, 0);
	g_TPlayContrl.TPlaySingel[_iPlayid].m_cFile[0] = 0;
	free(Playback);
	g_PlayBkVo[_iPlayid] = NULL;
	pthread_mutex_unlock(&g_PlayBkLock);
	
	return 0;
}
#if 1
int play_ZoomSetFrameRate(IN int _iplayid, int _iRate)
{
	int iRet = 0;
	if(NULL == g_PlayBkVo[_iplayid])
	{
		return 0;
	}
	pthread_mutex_lock(&g_PlayBkLock);
	if(g_EzoomHand == _iplayid && g_EzoomBigWindow && g_EzoomSmallWindow)
	{
		g_EzoomBigWindow->SetVoFrameRate(g_EzoomBigWindow,_iRate + BASE_VO_VDEC_FRATE);
		g_EzoomSmallWindow->SetVoFrameRate(g_EzoomSmallWindow,_iRate + BASE_VO_VDEC_FRATE);
	}
	pthread_mutex_unlock(&g_PlayBkLock);
	return iRet;
}
#endif
//处理文件 控制信息:加减速
static int FileSpeedContrl(int _iPlayid)//ok
{
	int iBack = 0;
	TPlayBack *Playback = g_PlayBkVo[_iPlayid];
	if(NULL == Playback)
	{
		return 0;
	}
	iBack = Playback->m_pDc.m_Stream.m_iBackFlag;
	//如果当前速度有变化，改变VO输出帧率
	if((int)(Playback->m_dBefSpeed * 10.0) != (int)(Playback->m_RunStat.m_dSpeed*10.0))
	{
		Playback->m_dBefSpeed = Playback->m_RunStat.m_dSpeed;
		if((int)(Playback->m_RunStat.m_dSpeed *10) > 10)
		{
			Playback->m_fSpeedValue = 1.0/Playback->m_RunStat.m_dSpeed;//处理速度
		}
		else if((int)(Playback->m_RunStat.m_dSpeed * 10) < -10)
		{
			Playback->m_fSpeedValue = 0 - Playback->m_RunStat.m_dSpeed;
		}
		else
		{
			Playback->m_fSpeedValue = 1.0;
		}
	}

	#if 0
	if(Playback->m_u32TimeStamp < Playback->m_u32UpFrameSTime
	||( Playback->m_u32TimeStamp > Playback->m_u32UpFrameSTime + 1500
	&& Playback->m_iOlnyReadIFrame == 0))//大于1秒
	{
		//重新获得基准时间
		Playback->iTimeChange = 1;
		Playback->m_u32UpFrameSTime = Playback->m_u32TimeStamp;
	}
	#endif
	long long lDiff = 0;
	if(!iBack)
	{
		lDiff = Playback->m_u32TimeStamp - Playback->m_u32UpFrameSTime;
	}
	else
	{	
		lDiff = Playback->m_u32UpFrameSTime - Playback->m_u32TimeStamp;
	}
	//通过修改基准时间改变显示速率
	if((int)(Playback->m_RunStat.m_dSpeed * 10) > 10)//加速
	{
		if(iBack)
		{
			Playback->s_TimeStamp.m_lFrameTime -= (long long)(lDiff*(1.0 - Playback->m_fSpeedValue));
		}
		else
		{
			Playback->s_TimeStamp.m_lFrameTime += (long long)(lDiff*(1.0 - Playback->m_fSpeedValue));
		}
	}
	else if((int)(Playback->m_RunStat.m_dSpeed * 10) < -10 )//减速
	{
		if(iBack)
		{
			Playback->s_TimeStamp.m_lFrameTime += (long long)(lDiff*(Playback->m_fSpeedValue - 1));
		}
		else
		{
			Playback->s_TimeStamp.m_lFrameTime -= (long long)(lDiff*(Playback->m_fSpeedValue - 1));
		}
	}

	//当前帧与上一帧的时间差值,以此算出在16倍速时的VO帧率
	if(Playback->m_u32UpFrameSTime < Playback->m_u32TimeStamp)
	{
		int iPreRate = (1000.0/lDiff);
		int iRate = (iPreRate + 1)* 4.0 / 3 / Playback->m_fSpeedValue;
		if(!(Playback->iSpeed16Rate * 2 > iRate &&  Playback->iSpeed16Rate  < iRate * 2))
		{
			Playback->iSpeed16Rate = iRate;
			g_TPlayContrl.TPlaySingel[_iPlayid].m_Rate = iRate;
			if(1 == Playback->m_iOlnyReadIFrame)
			{	
				iPreRate = -2;
			}
			else 
			{
				Playback_SetTFileValue(_iPlayid, -1, -1, iPreRate <= 0 ? 1 : iPreRate);
			}
			Playback->m_pDc.SetRate(&Playback->m_pDc.m_Stream,iRate, iPreRate);
			Playback->m_RunStat.m_iRate = Playback->m_pDc.GetRate(&Playback->m_pDc.m_Stream,1,0);
			play_ZoomSetFrameRate(_iPlayid, iRate);
			td_printf(0, "iSpeed16RateTemp = %d, iPreRate = %d", iRate, iPreRate);
		}
	}
	
	Playback->s_TimeStamp.m_lFrameTime += Playback->m_iImperTm;
	Playback->m_u32UpFrameSTime = Playback->m_u32TimeStamp;
	return 0;
}
static int GetWaitTime(IN int _iPlayid)
{	
	long long difTemp;
	long long LongTemp;
	long long RetValue;
	long long runtime;
	TPlayBack *Playback = g_PlayBkVo[_iPlayid];
	if(NULL == Playback)
	{
		return 0;
	}
	decode_GetSysTime(&runtime);//获得系统时间
	if(1 == Playback->iTimeChange)
	{
		Playback->iTimeChange = 0;
		Playback->m_u32UpFrameSTime = Playback->m_u32TimeStamp;
		ReGetBaseTime(_iPlayid,Playback->m_u32TimeStamp,runtime);
	}
	
	if(Playback->m_pDc.m_Stream.m_iBackFlag)
	{
		LongTemp = Playback->s_TimeStamp.m_lFrameTime - Playback->m_u32TimeStamp;
	}
	else
	{
		LongTemp = Playback->m_u32TimeStamp - Playback->s_TimeStamp.m_lFrameTime;
	}


	if(LongTemp < 0)
	{
		LongTemp = 0;
	}
	LongTemp *= 1000;

	if(LongTemp > (runtime - Playback->s_TimeStamp.m_lForkTime))
	{
		difTemp = LongTemp - (runtime - Playback->s_TimeStamp.m_lForkTime);
		RetValue = difTemp;
	}
	else
	{
		RetValue = 0;
	}
	return RetValue;
}
static int Playback_OpOverWaitStart(int _iPlayid, int _iValue)
{
	int icnt = 0;
	g_iReady[_iPlayid] = _iValue;
	while(icnt++ < 2000)
	{
		if(g_iReady[_iPlayid] == 0 || g_iReady[_iPlayid] == -2 || g_iReady[_iPlayid] == -3)
		{
			break;
		}
		usleep(1000);
	}
	return g_iReady[_iPlayid];
}

static int Playback_FindEncrypt(int _iPlayid)
{
	int i ;
	int ptr ;
	TPlayBack *Playback = g_PlayBkVo[_iPlayid];
	if(NULL == Playback)
	{
		return 0;
	}
	
	pthread_mutex_lock(&g_encryptlock);
	for(i = MAXPLAYER - 1; i >= 0; i--)
	{
		if(g_TPlayContrl.TPlaySingel[i].m_ucEncryptBak[0] != 0)
		{
			break;
		}
	}
	ptr = i;
	if(ptr >= 0)
	{
		for(i = ptr; i >= 0; i--)
		{
			// decode_TestEncrypt;
			if(Playback->m_pDc.TestEncrypt(&Playback->m_pDc.m_Stream, (char *)g_TPlayContrl.TPlaySingel[i].m_ucEncryptBak) == 0)
			{
				//decode_SetPassword
				Playback->m_pDc.SetPassword(&Playback->m_pDc.m_Stream, (char *)g_TPlayContrl.TPlaySingel[i].m_ucEncryptBak);
				break;
			}
		}
		if(i >= 0)
		{
			pthread_mutex_unlock(&g_encryptlock);
			return i;
		}
	}
	pthread_mutex_unlock(&g_encryptlock);
	return -1;
}
static int g_iStepFlag[MAXPLAYER] = {0};
static int DoneCmd(int _iPlayid)
{
	int iRet = 0;
	TPlayBack *Playback = g_PlayBkVo[_iPlayid];
	TPlayerInterface *pExInterface = &(g_TPlayContrl.TPlaySingel[_iPlayid].m_ExternalInterface);
	int iAudioFlag = pExInterface->iATrueFalseBak;
	int iRunFlag = pExInterface->m_iRunFlagBak;
	double dSpeed = pExInterface->m_dSpeedBak;
	long long llSkipPos = pExInterface->m_iSkipPosBak;
	if(NULL == Playback)
	{
		return 0;
	}

	if(Playback->m_RunStat.iATrueFalse != iAudioFlag)//声音状态发生改变
	{
		//decode_clearLeaveAudioStream
		Playback->m_pDc.clearLeaveAudioStream(&Playback->m_pDc.m_Stream);
		Playback->m_RunStat.iATrueFalse = iAudioFlag;
		if(iAudioFlag == 0)
		{
			Playback->m_pDc.DestroyAo(&Playback->m_pDc.m_Stream);
		}
	}
	if(Playback->m_RunStat.m_iStat != iRunFlag || g_iStepFlag[_iPlayid] == 1)//运行状态发生改变
	{
		Playback->m_pDc.clearLeaveAudioStream(&Playback->m_pDc.m_Stream);
		Playback->m_RunStat.m_iStat = iRunFlag;
		if(g_iStepFlag[_iPlayid] == 1)
		{
			g_iStepFlag[_iPlayid] = 0;
			Playback->m_RunStat.m_iStat = PLAYBACK_STAT_PAUSE;
			pExInterface->m_iRunFlagBak = PLAYBACK_STAT_PAUSE;
			__CpStats(_iPlayid);
			if(g_iAsyOrsynFlag == 1)
			{
				Playback_OpOverWaitStart(_iPlayid, 1);
			}
		}
		else
		{
			__CpStats(_iPlayid);
			if(g_iAsyOrsynFlag == 1 && Playback->m_RunStat.m_iStat != PLAYBACK_STAT_STOP
				&&Playback->m_RunStat.m_iStat != PLAYBACK_STAT_STEP)
			{
				Playback_OpOverWaitStart(_iPlayid, 1);
			}	
		}
		
		Playback->iTimeChange = 1;
		
		td_printf(0,"g_PlayBkVo[_iPlayid]->m_RunStat.m_iStat = %d",Playback->m_RunStat.m_iStat);
		if(Playback->m_RunStat.m_iStat == PLAYBACK_STAT_STEP)
		{
			if(Playback->m_pDc.m_Stream.m_BackId)
			{
				//decode_destroy_back
				Playback->m_pDc.destroy_back(&Playback->m_pDc.m_Stream, 1);
				Playback->m_RunStat.m_iStat = PLAYBACK_STAT_PAUSE;
				pExInterface->m_iRunFlagBak = PLAYBACK_STAT_PAUSE;
			}
			else
			{
				g_iStepFlag[_iPlayid] = 1;
				Playback->m_RunStat.m_iStat = PLAYBACK_STAT_PLAY;//2013.3.21
			}
			//g_TPlayContrl.TPlaySingel[_iPlayid].m_ExternalInterface.m_iRunFlagBak = PLAYBACK_STAT_PAUSE;
		}
		else if(Playback->m_RunStat.m_iStat == PLAYBACK_STAT_PLAY)
		{
			Playback->m_pDc.destroy_back(&Playback->m_pDc.m_Stream, 1);
		}
		else if(Playback->m_RunStat.m_iStat == PLAYBACK_STAT_BACKSTEP)
		{
			if(td_success == Playback->m_pDc.init_back(&Playback->m_pDc.m_Stream, PLAYBACK_BACK_NOCLIP))
			{
				g_iStepFlag[_iPlayid] = 1;
			}
			else
			{
				Play_Set_BackErr(Playback->m_iOptStat);
				Playback->m_RunStat.m_iStat = PLAYBACK_STAT_PAUSE;
				pExInterface->m_iRunFlagBak = PLAYBACK_STAT_PAUSE;
			}
		}
		else if(Playback->m_RunStat.m_iStat == PLAYBACK_STAT_BACK)
		{
			if(td_success != Playback->m_pDc.init_back(&Playback->m_pDc.m_Stream, (int)Playback->m_RunStat.m_dSpeed))
			{
				Play_Set_BackErr(Playback->m_iOptStat);
				Playback->m_RunStat.m_iStat = PLAYBACK_STAT_PAUSE;
				pExInterface->m_iRunFlagBak = PLAYBACK_STAT_PAUSE;
			}
		}
		else if(Playback->m_RunStat.m_iStat == PLAYBACK_STAT_STOP)
		{
			Playback->m_pDc.destroy_back(&Playback->m_pDc.m_Stream, 1);
		}

		
	}
	if( (int)(Playback->m_RunStat.m_dSpeed  * 10) != (int)(dSpeed * 10))//运行速度
	{
		iRet = 1;
		Playback->iSpeed16Rate = 0;
		Playback->m_pDc.clearLeaveAudioStream(&Playback->m_pDc.m_Stream);
		Playback->m_RunStat.m_dSpeed = dSpeed;
		Playback->iTimeChange = 1;
		Playback->m_iOlnyReadIFrame = 0;
		Playback->m_pDc.m_Stream.m_iOnlyIFrame = 0;
		
		if(Playback->m_pDc.m_Stream.m_BackId)
		{
			if(td_success != Playback->m_pDc.init_back(&Playback->m_pDc.m_Stream, (int)Playback->m_RunStat.m_dSpeed))
			{
				Play_Set_BackErr(Playback->m_iOptStat);
				Playback->m_RunStat.m_iStat = PLAYBACK_STAT_PAUSE;
				pExInterface->m_iRunFlagBak = PLAYBACK_STAT_PAUSE;
			}
		}
		
		if( (int)(Playback->m_RunStat.m_dSpeed * 10) == (MAXSPEED * 10))
		{
			
			if(Playback->m_RunStat.m_iRate > PLAYPFRAMERATE)
			{
				Playback->m_pDc.m_Stream.m_iOnlyIFrame = 1;
				Playback->m_iOlnyReadIFrame = 1;				
			}
			Playback->iSpeed16Rate = 0;			
			__CpStats(_iPlayid);
			if(g_iAsyOrsynFlag == 0)//在异步的时候由自己等待0.5秒
			{
				usleep(500000);
			}
			else//在异步的时候由同步线程控制等待0.5秒
			{
				Playback_OpOverWaitStart(_iPlayid, 1);
			}
		}
	}

	if((int)(Playback->m_RunStat.m_dSpeed * 10) == (MAXSPEED * 10))
	{
		if(Playback->m_RunStat.m_iRate > PLAYPFRAMERATE)
		{
			if( Playback->m_pDc.m_Stream.m_iOnlyIFrame == 0 )
			{
				Playback->m_pDc.m_Stream.m_iOnlyIFrame = 1;
				Playback->m_iOlnyReadIFrame = 1;				
			}
		
		}
		else 
		{
			if( Playback->m_pDc.m_Stream.m_iOnlyIFrame == 1 )
			{
				Playback->m_pDc.m_Stream.m_iOnlyIFrame = 0;
				Playback->m_iOlnyReadIFrame = 0;				
			}
		}
	}

	if(llSkipPos >= 0)
	{
		Playback->m_pDc.clearLeaveAudioStream(&Playback->m_pDc.m_Stream);
		g_TPlayContrl.TPlaySingel[_iPlayid].StopFlag = 0;
		Playback->m_iSkipPos = llSkipPos;	
		if(Playback->m_iSkipPos <= 100000 && Playback->m_iSkipPos >= 0)
		{
			long long iSkipTime = (Playback->m_ptFileEndTm - Playback->m_ptFileBeginTm);
			iSkipTime *= Playback->m_iSkipPos;
			iSkipTime /= 100;
			//int iSkipTime = (g_PlayBkVo[_iPlayid]->m_ptFileEndTm - g_PlayBkVo[_iPlayid]->m_ptFileBeginTm)* g_PlayBkVo[_iPlayid]->m_iSkipPos /100;
			iSkipTime += (long long)Playback->m_ptFileBeginTm * 1000;
			Playback->m_iSkipPos = iSkipTime;
		}
		
		pExInterface->m_iSkipPosBak = -1;
	}
	
	if(1 == Playback->m_RunStat.m_iNeedSecret && pExInterface->iEncrypt == 1)
	{
		pExInterface->iEncrypt = 0;
		if(g_iAsyOrsynFlag == 1)
		{
			if(-1 != Playback_FindEncrypt(_iPlayid))
			{
				pExInterface->iEncrypt = 0;
				Playback->m_RunStat.m_iNeedSecret = 0;
				__CpStats(_iPlayid);
			}
		}
		else
		{
			pthread_mutex_lock(&g_encryptlock);
			//decode_TestEncrypt
			if(Playback->m_pDc.TestEncrypt(&Playback->m_pDc.m_Stream, (char *)g_TPlayContrl.TPlaySingel[_iPlayid].m_ucEncryptBak) == 0)
			{
				//decode_SetPassword
				Playback->m_pDc.SetPassword(&Playback->m_pDc.m_Stream, (char *)g_TPlayContrl.TPlaySingel[_iPlayid].m_ucEncryptBak);
				pExInterface->iEncrypt = 0;
				Playback->m_RunStat.m_iNeedSecret = 0;
				__CpStats(_iPlayid);
			}
			pthread_mutex_unlock(&g_encryptlock);
		}
	}

	if(g_TPlayContrl.TPlaySingel[_iPlayid].m_lPause[0] < 0)
	{
		long long lTime = 0;
		decode_GetSysTime(&lTime);
		g_TPlayContrl.TPlaySingel[_iPlayid].m_lPause[1] = lTime,
		g_TPlayContrl.TPlaySingel[_iPlayid].m_lPause[2] = g_TPlayContrl.TPlaySingel[_iPlayid].m_lPause[0] * (-1),
		g_TPlayContrl.TPlaySingel[_iPlayid].m_lPause[0] = 0;
	}
	if(g_TPlayContrl.TPlaySingel[_iPlayid].m_lPause[2] > 0)
	{
		long long lCnt = g_TPlayContrl.TPlaySingel[_iPlayid].m_lPause[2];
		long long lTime = 0;
		long long lTime2 = g_TPlayContrl.TPlaySingel[_iPlayid].m_lPause[1];
		
		decode_GetSysTime(&lTime);
		if(lTime > lTime2)
		{
			g_TPlayContrl.TPlaySingel[_iPlayid].m_lPause[2] = (lCnt > lTime - lTime2)
							? lCnt - (lTime - lTime2) : 0;
		}
		g_TPlayContrl.TPlaySingel[_iPlayid].m_lPause[1] = lTime;
	}

	if(g_TPlayContrl.TPlaySingel[_iPlayid].m_lPause[2] > 0 && Playback->m_RunStat.m_iStat != PLAYBACK_STAT_STOP)
	{
		iRet = -1;
		goto leave;
	}
	if(Playback->m_RunStat.m_iStat == PLAYBACK_STAT_PAUSE && Playback->m_iSkipPos== -1)
	{
		iRet = -1;
		goto leave;
	}
	else if(Playback->m_RunStat.m_iStat == PLAYBACK_STAT_STOP)
	{
		iRet = -2;
		goto leave;
	}
	else if(Playback->m_RunStat.m_iStat == PLAYBACK_STAT_INIT && Playback->m_iSkipPos== -1)
	{
		iRet = -1;
		goto leave;
	}
	if(Playback->m_RunStat.m_iBlackScreen == 1 && Playback->m_iSkipPos== -1)
	{
		iRet = -1;
		goto leave;
	}
	if(g_TPlayContrl.TPlaySingel[_iPlayid].StopFlag == 1)
	{
		iRet = -1;
		goto leave;
	}
leave:
	return iRet;
}

static int PlayBackOpenerr(int _iPlayid, int _iFlag)
{
	int iFileCurrent;
	int iDownFile;
	if(0 == g_iAsyOrsynFlag)
	{
		return -1;
	}
	if(FILE_ERR == _iFlag || VOVDEC_ERR == _iFlag)
	{
		if(Playback_Get_DownFile(_iPlayid, &iFileCurrent, &iDownFile) < 0)
		{
			td_printf(0,"%s(%d)",__func__,__LINE__);
			Playback_Do_OverFile(_iPlayid);
		}
		else
		{
			if(Playback_Test_File(_iPlayid, iDownFile) == FILEBLANK)
			{
				Playback_SetCurrentFile(_iPlayid, iDownFile);
				Playback_Do_BlankFile(_iPlayid, iDownFile);
			}
			else
			{
				Playback_Do_BlankFile(_iPlayid, iFileCurrent);
			}
		}
	}
	else
	{
		Playback_Do_OverFile(_iPlayid);
	}
	if(VOVDEC_ERR != _iFlag)
	{
	 	ClearChannelLimit(g_Tlimit.m_TPicLimit.m_iPicLimitChannel, _iPlayid);
	}
	return 0;
}



int DonePressure(IN int _iplayid)
{
	int iSpeedPressure = 0;
	int iPressure = 0;
	Playback_CalPressure(_iplayid,&iSpeedPressure, &iPressure, 0);
	if(iSpeedPressure > MAX_DECODE_SRATE)
	{
		Playback_SetLimit(SPEEDLIMITFLAG, _iplayid);
		if(0 == g_iAsyOrsynFlag)
		{
			Playback_Resume(_iplayid);
		}
		else
		{
			int i = 0;
			for(i = 0; i < MAXPLAYER; i++)
			{
				Playback_Resume(i);
			}
		}
	}
	return 0;
}

int DoSeek(IN int _iplayid)
{
	int iRet = 1;
	int iRetSeek = 0;
	long long iPos = 0;
	int iNote = 1;
	int iWaitValue = 0;
	TPlayBack *Playback = g_PlayBkVo[_iplayid];
	if(NULL == Playback || Playback->m_iSkipPos < 0)
	{
		return 0;
	}
	
	if(g_iAsyOrsynFlag == 1)
	{
		iPos = Playback->m_iSkipPos - (long long)gAsyContrl.m_u32ReqStartTime * 1000;
	}
	else
	{
		iPos = Playback->m_iSkipPos - (long long)Playback->m_ptFileBeginTm * 1000;
	}
	Playback_SetCurrentFTime(_iplayid,iPos);
	__CpStats(_iplayid);

	do
	{
		iRetSeek =  Playback_SeekFile(_iplayid, Playback->m_iSkipPos/1000);
		if(1 == iRetSeek && 1 == g_iAsyOrsynFlag)
		{
			if(-1 == Playback_FindEncrypt(_iplayid))
			{
				break;
			}
		}
	}while(1 == iRetSeek && 1 == g_iAsyOrsynFlag);

	if(iRetSeek == 1)//在解码过程中，遇到需要密码的帧，此时没有跳转完成
	{
		//需要密码
		iNote = 2;
		Playback->m_RunStat.m_iNeedSecret = 1;
		__CpStats(_iplayid);
	}
	else if(iRetSeek < 0)//在跳转过程中，产生了错误
	{
		if(!(Playback->m_pDc.m_Stream.m_BackId && PLAYBACK_FILEFINISH_NOEFFECT == iRetSeek))
		{
		iRet = iRetSeek;
		}
		if(g_iAsyOrsynFlag == 1)
		{
			iNote = 3;//通知总线程，在跳转过程中产生了错误
		}
	}
	
	if(g_iAsyOrsynFlag == 1)//如果是同步，跳转完成后，告诉总线程该线程跳转完成
	{
		iWaitValue = Playback_OpOverWaitStart(_iplayid, iNote);
	}
	Playback->iTimeChange = 1;//重新获得时间戳标志
	Playback->m_iSkipPos = -1;//跳转标志置为无效
	return iRet;
}

int DoRead(IN int _iplayid)
{
	int icnt = 0;
	int iRetRead = 0;
	TPlayBack *Playback = g_PlayBkVo[_iplayid];
	if(NULL == Playback)
	{
		return 0;
	}
	
	do
	{
		//decode_ReadFrame
		iRetRead = Playback->m_pDc.ReadFrame(&Playback->m_pDc.m_Stream);
		if(iRetRead < 0 || PLAYBACK_IFRAME == iRetRead)
		{
			break;
		}
	}while(Playback->m_iOlnyReadIFrame == 1 && icnt++ < 120);
	
	if(iRetRead == PLAYBACK_FILEFINISH_NOEFFECT)//文件读到最后
	{
		Playback->m_RunStat.m_iFinishFileFlag = 1;
		//decode_GetFrameTm
		Playback->m_u32TimeStamp = Playback->m_pDc.GetFrameTm(&Playback->m_pDc.m_Stream);
	}
	else if(iRetRead == PLAYBACK_READFILE_ERR)
	{
		Playback->m_RunStat.m_iFinishFileFlag = 1;
	}
	else
	{
		Playback->m_u32TimeStamp = Playback->m_pDc.GetFrameTm(&Playback->m_pDc.m_Stream);
	}
	
	return iRetRead;
}
int DoSendLeftAudio(IN int _iplayid)
{
	TPlayBack *Playback = g_PlayBkVo[_iplayid];
	if(NULL == Playback)
	{
		return 0;
	}
	
	pthread_mutex_lock(&Playback->AudioLock);
	if(g_TPlayContrl.TPlayAll.m_iAudioChannal == _iplayid)
	{
		Playback->m_pDc.SendLeaveAudioFrames(&Playback->m_pDc.m_Stream);
	}
	else
	{
		Playback->m_pDc.clearLeaveAudioStream(&Playback->m_pDc.m_Stream);
	}
	pthread_mutex_unlock(&Playback->AudioLock);
	
	return 0;
}

int DoSendVStream(IN int _iplayid)
{
	int iRet = 0;
	int iRetSend = 0;
	TPlayBack *Playback = g_PlayBkVo[_iplayid];
	if(NULL == Playback)
	{
		return 0;
	}
	
	do
	{
		//decode_SendVStream
		iRetSend = Playback->m_pDc.SendVStream(&Playback->m_pDc.m_Stream);
		if(iRetSend == 1 && g_iAsyOrsynFlag == 1)
		{
			if(-1 == Playback_FindEncrypt(_iplayid))
			{
				break;
			}
		}
	}while(1 == iRetSend && 1 == g_iAsyOrsynFlag);
	
	if(iRetSend == 1)
	{
		//需要密码
		td_printf(0,"ENCRPT");
		Playback->iTimeChange = 1;
		Playback->m_RunStat.m_iNeedSecret = 1;
		__CpStats(_iplayid);
		iRet = 1;
		goto leave;
	}
	Playback->m_RunStat.m_iNeedSecret = 0;
leave:	
	return iRet;
}
//wsl add 将默认音频输出通道设置到decode中
int playback_setdefaultAo(int _iDefaultAo)
{
	decode_setdefaultAo(_iDefaultAo);
	return td_success;
}

int DoSendAudio(IN int _iplayid)
{
	TPlayBack *Playback = g_PlayBkVo[_iplayid];
	if(NULL == Playback)
	{
		return 0;
	}
	if(Playback->m_RunStat.iATrueFalse == 1 
		&& Playback->m_AudioFlag == 1 
		&& (int)(Playback->m_RunStat.m_dSpeed * 10) == 10)
	{
		pthread_mutex_lock(&Playback->AudioLock);
		if(g_TPlayContrl.TPlayAll.m_iAudioChannal == _iplayid)
		{
			Playback->m_pDc.SendAStream(&Playback->m_pDc.m_Stream);
		}
		pthread_mutex_unlock(&Playback->AudioLock);
	}
	return 0;
}

int DoSleep(IN int _iplayid)
{
	int iRet = td_failure;
	int iRealSleepTm = 0;;
	int iSleepTm;
	
	TPlayBack *Playback = g_PlayBkVo[_iplayid];
	iSleepTm = GetWaitTime(_iplayid);
	if(iSleepTm > 75000)
	{
		iRealSleepTm = 40000;
		goto leave;
	}
	
	if(iSleepTm != 0)
	{
		iRealSleepTm = iSleepTm;
	}
	else
	{
		usleep(0);
		iRealSleepTm = 0;
	}
	iRet = td_success;	
leave:

	if(0 == iRealSleepTm && Playback->m_pDc.m_Stream.m_iBackFlag)
	{
		if(Playback->m_u32BefSleepTm++ == 2)
		{
			USLEEP(40000);
			Playback->iTimeChange = 1;
		}
	}
	else
	{
		Playback->m_u32BefSleepTm = 0;
	}
	USLEEP(iRealSleepTm);
	return iRet;
}
void *PlayThread(IN int *_playid)
{
	int iPlayid = *_playid;
	int iRet = -1;
	int iReRead = 0;
	int iRetSeek = 0;
	int iReWait = 0;
	int icount = 0;
	int iErr = 0;
	int iRetRead = 0;
	func_info();
	TPlayBack *Playback = g_PlayBkVo[iPlayid];
	if(!Playback)
	{
		goto leave;
	}
	Playback->m_u32BefSleepTm = 0;
	while(1)
	{
		//文件处理
		iRet = PlayBackStatContrl(&Playback->m_RunStat);
		if(iRet == FILE_ERR || iRet == VOVDEC_ERR  || iRet == -1 )
		{
			if(PlayBackOpenerr(iPlayid, iRet) == -1)
			{
				break;
			}
		}
		//处理UI发送命令
		iRet = DoneCmd(iPlayid);
		if(iRet == -1)//暂停或者是空文件
		{	
			Playback->m_pDc.DestroyAo(&Playback->m_pDc.m_Stream);
			__CpStats(iPlayid);
			usleep(1000);
			continue ;
		}
		else if(iRet == -2)//播放器结束
		{
			td_printf(0,"%s, %d, STOP\n", __func__, __LINE__);
			break;
		}
		else if(1 == iRet)
		{
			icount = 0;
			td_printf(0,"change speed happen");
		}
		//定位
		iRetSeek = DoSeek(iPlayid);
		if( iRetSeek > 0)
		{
			iReRead = 0;
		}
		else if(iRetSeek < 0)
		{
			iReRead = 0;
			iErr = (-1 == iRetSeek) ? VOVDEC_ERR : FILE_ERR;
			if(PlayBackOpenerr(iPlayid, iErr) < 0)
			{
				break;
			}
		}
		//读音视频数据
		if(Playback->m_RunStat.m_iBlackScreen == 1 || Playback->m_RunStat.m_iNeedSecret == 1)
		{
			continue;
		}
		if(iReRead == 0)
		{
			iRetRead = DoRead(iPlayid);
			if(iRetRead == PLAYBACK_READFILE_ERR || iRetRead == PLAYBACK_FILEFINISH_NOEFFECT)
			{
				continue;
			}
			iReRead = 1;
		}

		//发送剩余的音频数据
		if(Playback->m_pDc.m_Stream.m_iLeaveAudioFrames)
		{
			DoSendLeftAudio(iPlayid);
		}

		//根据时间戳进行睡眠等待
		if(iReWait == 0)
		{
			iReWait = 1;
			FileSpeedContrl(iPlayid);
		}
		if(td_failure == DoSleep(iPlayid))
		{
			continue;
		}
		iReWait = 0;	
		//发送视频数据
		if(1 == DoSendVStream(iPlayid))
		{
			usleep(10000);
			continue;
		}
		iReRead = 0;
		//发送音频数据
		Playback->m_pDc.clearLeaveAudioStream(&Playback->m_pDc.m_Stream);
		DoSendAudio(iPlayid);
		//更新当前状态
		Playback_SetCurrentFTime(iPlayid,-1);
		__CpStats(iPlayid);
		//测试压力
		if(icount++ > 25)
		{
			icount = 0;
			DonePressure(iPlayid);
		}	
	}
	//全部文件已经操作完成
	//销毁资源
	Playback_SetTFileValue(iPlayid, 0, 0, 0);
	Playback->m_RunStat.m_iStat = PLAYBACK_STAT_STOP;
	Playback->m_RunStat.m_tCurPlayTm = 100;
	Playback->m_RunStat.m_iSpeed = 1;
	__CpStats(iPlayid);
	Playback_DestroyPlayer(iPlayid);
leave:
	return 0;
}

//功能:查找文件名
//参数:_pResumeTm :返回从文件的起始的时间
//		_pFileNum:返回文件号(在同步回放时有用,返回0xffff表示是正在操作的文件)
static int Playback_SeekFile(IN int _iPlayid, IN int _iSeekTm)//查找文件
{
	int i = 0;
	int iRet = 0;
	int iReturn = 0;
	int iFileNum = 0;
	int iSeekTm = _iSeekTm;//距离文件开始的时间值
	int iFlag = 0;
	td_u8	ucFileName[FILE_NAME_LEN];
	time_t	iStartTime;
	time_t	iEndTime;	
	if(g_iAsyOrsynFlag != 1)
	{
		//decode_SeekPos
		return g_PlayBkVo[_iPlayid]->m_pDc.SeekPos(&g_PlayBkVo[_iPlayid]->m_pDc.m_Stream, iSeekTm, &g_TPlayContrl.TPlaySingel[_iPlayid].StopFlag);
	}
	pthread_mutex_lock(&g_PlayBkAsyLock);
	if(NULL == g_synPlayBk)
	{
		pthread_mutex_unlock(&g_PlayBkAsyLock);
		return -1;
	}
	TFileList *pTFileList = &g_synPlayBk->m_TFileListMsg[_iPlayid];
	TFileListPlay *pTFileInfoPlay = pTFileList->m_pFileListPlay;
	//先对跳转的时间点进行预处理
	if(iSeekTm < gAsyContrl.m_u32ReqStartTime)
	{
		iSeekTm = gAsyContrl.m_u32ReqStartTime;//整个播放文件的开始
	}
	else if(iSeekTm > gAsyContrl.m_u32ReqStopTime)
	{
		iSeekTm = gAsyContrl.m_u32ReqStopTime;//整个播放文件的结束
	}

	//搜索文件
	for(i = 0; i < g_synPlayBk->m_TFileListMsg[_iPlayid].m_iFileNum; i++)
	{
		if(pTFileInfoPlay[i].m_iEndTime > iSeekTm)
		{
			break;
		}
	}
	iFileNum = g_synPlayBk->m_TFileListMsg[_iPlayid].m_iFileNum;
	if(i == iFileNum)
	{
		i--;
		iFlag = 1;
	}
	if(pTFileInfoPlay[i].m_pFileList)
	{
		memcpy(ucFileName, (char *)pTFileInfoPlay[i].m_pFileList->m_ucFileName, FILE_NAME_LEN);
		iStartTime = pTFileInfoPlay[i].m_iStartTime;
		iEndTime = pTFileInfoPlay[i].m_iEndTime;
	}
	pthread_mutex_unlock(&g_PlayBkAsyLock);
	if(iFlag)
	{
		Playback_SetCurrentFile(_iPlayid, i);
		Playback_Do_OverFile(_iPlayid);
		return 0;
	}
	Playback_SetCurrentFile(_iPlayid, i);
	iRet = Playback_Test_File(_iPlayid, i);
	//td_printf(0,"playid  = %d,     iRet = %d ,i = %d ,m_iFileNum = %d", _iPlayid,iRet , i, g_synPlayBk->m_TFileListMsg[_iPlayid].m_iFileNum);
	if(iRet == FILENULL)
	{
		//td_printf(0,"%s(%d)",__func__,__LINE__);
		Playback_Do_OverFile(_iPlayid);
	 	ClearChannelLimit(g_Tlimit.m_TPicLimit.m_iPicLimitChannel, _iPlayid);
	}
	else if(iRet == FILEBLANK)
	{
		Playback_Do_BlankFile(_iPlayid, i);
		ClearChannelLimit(g_Tlimit.m_TPicLimit.m_iPicLimitChannel, _iPlayid);
	}
	else
	{
		iRet = Playback_SetPlayeFile(_iPlayid, (char *)ucFileName, iStartTime, iEndTime, i);
		if(FILE_ERR == iRet ||VOVDEC_ERR == iRet || -1 == iRet)
		{
			if(PlayBackOpenerr(_iPlayid, iRet) == -1)
			{
				return -1;
			}
			return 0;
		}
		iReturn = g_PlayBkVo[_iPlayid]->m_pDc.SeekPos(&g_PlayBkVo[_iPlayid]->m_pDc.m_Stream,iSeekTm, &g_TPlayContrl.TPlaySingel[_iPlayid].StopFlag);
	}
	//查询完成后打开文件
	return iReturn;
}


int PlayBackStatContrl(TPlayerStat *_ptStat)	
{
	int iRet = 0;
	int _iPlayid = _ptStat->m_iPid;
	TFileList *pTFileList;
	TFileListPlay *pTFileInfoPlay;
	if(g_synPlayBk != NULL && g_iAsyOrsynFlag == 1)//在同步回放的情况下
	{

		
		if(_ptStat->m_iTimeSeg >= (gAsyContrl.m_u32ReqStopTime - gAsyContrl.m_u32ReqStartTime)*1000)
		{
			if(g_TPlayContrl.TPlaySingel[_iPlayid].StopFlag != 1)
			{
				Playback_Do_OverFile(_iPlayid);
			 	ClearChannelLimit(g_Tlimit.m_TPicLimit.m_iPicLimitChannel, _iPlayid);
			}
			return 0;
		}
		if(_ptStat->m_iFinishFileFlag == 1 || g_OpenNextFileFlag[_iPlayid] == 1)//文件播放完成
		{
			int iCurrentFile = 0;
			int iNextFile = 0;
			int iFileStat = 0;
			time_t iStartTime;
			time_t iEndTime;
			unsigned char ucFileName[FILE_NAME_LEN];
			iRet = 0;
			if(g_OpenNextFileFlag[_iPlayid] == 1)
			{
				g_PlayBkVo[_iPlayid]->iTimeChange = 1;
			}
			iNextFile = Playback_NextFile(_iPlayid, &iCurrentFile);
			if(iNextFile < 0)
			{
				//td_printf(0,"%s(%d)",__func__,__LINE__);
				Playback_Do_OverFile(_iPlayid);
			 	ClearChannelLimit(g_Tlimit.m_TPicLimit.m_iPicLimitChannel, _iPlayid);
			}
			else
			{
				iFileStat = Playback_Test_File(_iPlayid, iNextFile);
				if(iFileStat == FILEBLANK)
				{
					Playback_Do_BlankFile(_iPlayid, iNextFile);
				 	ClearChannelLimit(g_Tlimit.m_TPicLimit.m_iPicLimitChannel, _iPlayid);					
				}
				else 
				{
					pthread_mutex_lock(&g_PlayBkAsyLock);
					if(!g_synPlayBk)
					{
						pthread_mutex_unlock(&g_PlayBkAsyLock);
						return -1;
					}
					pTFileList = &g_synPlayBk->m_TFileListMsg[_iPlayid];
					pTFileInfoPlay = pTFileList->m_pFileListPlay;
					iStartTime = pTFileInfoPlay[iNextFile].m_iStartTime;
					iEndTime = pTFileInfoPlay[iNextFile].m_iEndTime;
					if(pTFileInfoPlay[iNextFile].m_pFileList)
					{
						memcpy((char *)ucFileName, (char *)pTFileInfoPlay[iNextFile].m_pFileList->m_ucFileName, FILE_NAME_LEN);
					}					
					pthread_mutex_unlock(&g_PlayBkAsyLock);
					iRet = Playback_SetPlayeFile(_iPlayid, (char *)ucFileName, iStartTime, iEndTime, iNextFile);
				}
			}
			if(g_OpenNextFileFlag[_iPlayid] == 1)
			{
				g_OpenNextFileFlag[_iPlayid] = 2;
			}
			return iRet;
		}
	}
	else
	{
		if(_ptStat->m_iFinishFileFlag == 1)
		{
			Playback_Stop(_iPlayid);
			usleep(10000);
			return 0;
		}
	}
	return 0;
}
int Playback_Seek(int _iPlayid, IN long long _iSeekTm)
{
	if(g_PlayBkVo[_iPlayid] == NULL)
	{
		return -1;
	}
	g_TPlayContrl.TPlaySingel[_iPlayid].m_ExternalInterface.m_iSkipPosBak = _iSeekTm;
	return 0;
}

int Playback_PauseTime(int _iPlayid, int _iTime)
{
	long long lCnt = 200 * 1000;
	long long lTime = 0;
	long long lTime2 = 0;
	if(g_PlayBkVo[_iPlayid] == NULL)//该播放器尚未建立
	{
		return -1;	
	}
	if(!_iTime)
	{
		return 0;
	}
	
	g_TPlayContrl.TPlaySingel[_iPlayid].m_lPause[0] = _iTime * (-1);
	while(g_TPlayContrl.TPlaySingel[_iPlayid].m_lPause[0] < 0)
	{
		decode_GetSysTime(&lTime);
		if((lTime2 != 0) && (lTime > lTime2))
		{
			lCnt = (lCnt > lTime - lTime2)
							? lCnt - (lTime - lTime2) : 0;
		}
		if(lCnt == 0)
		{
			break;
		}
		lTime2 = lTime;
		usleep(1000);			
	}

	if(g_TPlayContrl.TPlaySingel[_iPlayid].m_lPause[0] < 0)
	{
		return -1;
	}
	return 0;
}
// 回放器操作相关接口，返回值0表成功，负数表失败

int Playback_CapPicture(int _iPlayid)
{
	int iRet = 0;
	int iTime = 0;
	TPlayBack *Playback = NULL;

	if(_iPlayid < 0 || _iPlayid >= MAXPLAYER)
	{
		return td_failure;
	}
	Playback = g_PlayBkVo[_iPlayid];
	pthread_mutex_lock(&g_PlayBkLock);
	if(Playback== NULL)//该播放器尚未建立
	{
		iRet = td_failure;
		goto leave;
	}
	
	if(0 == g_iAsyOrsynFlag)//异步回放
	{
		iTime = Playback->m_RunStat.m_iTimeSeg/1000 + Playback->m_ptFileBeginTm;
	}
	else
	{
		iTime = Playback->m_RunStat.m_iTimeSeg/1000 + gAsyContrl.m_u32ReqStartTime;
	}
	
	iRet = Playback->m_pDc.capPicture(&Playback->m_pDc.m_Stream, iTime, 1);
	if(td_success != iRet)
	{
		Play_Set_CapErr(Playback->m_iOptStat);
	}
leave:
	pthread_mutex_unlock(&g_PlayBkLock);
	return td_success;
}

int Playback_GetOptStat(int _iPlayid)		
{
	int i = 0;
	int iStart = _iPlayid;
	int iEnd = _iPlayid;
	int iDecodeRet;
	int iRet = td_failure;
	TPlayBack *Playback = NULL;
	if(_iPlayid < 0 || _iPlayid > MAXPLAYER)
	{
		return td_failure;
	}
	
	if(_iPlayid == MAXPLAYER)
	{
		iStart = 0;
		iEnd = MAXPLAYER - 1;
	}
	Play_Clr_All(iDecodeRet);
	pthread_mutex_lock(&g_PlayBkLock);
	#if 0
	Playback_TestLimit(PICANDSPEEDLIMITFLAG, &iLimit);
	if(iLimit)
	{
		td_printf(0,"iLimit = %d",iLimit);
		if(SPEEDLIMITFLAG == iLimit)
		{
			iRet = GUI_VIDEO_PLAY_SPEED_CAP_LIMIT;
		}
		else
		{
			iRet = GUI_VIDEO_PLAY_DECODE_CAP_LIMIT;
		}
		Playback_ClearLimitFlag(iLimit);
		goto leave;
	}
	#endif
	for(i = iStart; i <= iEnd; i++)
	{
		if(NULL == (Playback = g_PlayBkVo[i]))
		{
			continue;
		}

		if(td_failure == Playback->m_pDc.getOptStat(&Playback->m_pDc.m_Stream, &iDecodeRet))
		{
			Play_Clr_All(iDecodeRet);
		}

		Playback->m_iOptStat |= iDecodeRet;
		if(Play_Test_BackErr(Playback->m_iOptStat))
		{
			iRet = ERR_PLAY_BACK_ERR;
			Play_Clr_Back(Playback->m_iOptStat);
			goto leave;
		}
		else if(Play_Test_CapErr(Playback->m_iOptStat))
		{
			iRet = ERR_PLAY_CAP_ERR;
			Play_Clr_Cap(Playback->m_iOptStat);
			goto leave;
		}
		else if(Play_Test_CapOk(Playback->m_iOptStat))
		{
			iRet = ERR_PLAY_CAP_OK;
			Play_Clr_Cap(Playback->m_iOptStat);
			goto leave;
		}
	}
	
leave:
	if(td_failure != iRet)
	{
		td_printf(0,"sssssssssssssssssss iret = %d", iRet);
	}
	pthread_mutex_unlock(&g_PlayBkLock);
	return iRet;
}
int Playback_Start(int _iPlayid)		//开始回放
{
	if(g_PlayBkVo[_iPlayid] == NULL)//该播放器尚未建立
		return -1;
	g_TPlayContrl.TPlaySingel[_iPlayid].m_ExternalInterface.m_iRunFlagBak = PLAYBACK_STAT_PLAY;
	td_printf(0,"=====================start=======================%d\n",_iPlayid);
	return 0;

}
//暂停
int Playback_Pause(int _iPlayid)
{
	if(g_PlayBkVo[_iPlayid] == NULL)
		return -1;
	g_TPlayContrl.TPlaySingel[_iPlayid].m_ExternalInterface.m_iRunFlagBak = PLAYBACK_STAT_PAUSE;
	td_printf(0,"=====================pause=======================%d\n",_iPlayid);

	return 0;
}

int Playback_Back(int _iPlayid)		//倒放
{
	int i = 0;
	TPlayerStat tStat;
	if(g_PlayBkVo[_iPlayid] == NULL)
		return -1;

	Playback_Resume(_iPlayid);
	while(i++ < 100)
	{
		Playback_GetCurStat(_iPlayid, &tStat);
		if((1 == tStat.m_iSpeed) ||(tStat.m_iStat == PLAYBACK_STAT_STOP || tStat.m_iStat == PLAYBACK_STAT_INIT))
		{
			break;
		}
		usleep(1000);
	}
	g_TPlayContrl.TPlaySingel[_iPlayid].m_ExternalInterface.m_iRunFlagBak = PLAYBACK_STAT_BACK;
	td_printf(0,"=====================back=======================\n");
	return 0;
}

int Playback_Stop(int _iPlayid)		//结束回放
{
	if(g_PlayBkVo[_iPlayid] == NULL)
		return -1;
	g_TPlayContrl.TPlaySingel[_iPlayid].m_ExternalInterface.m_iRunFlagBak = PLAYBACK_STAT_STOP;
	g_TPlayContrl.TPlaySingel[_iPlayid].StopFlag = 1;
	td_printf(0,"=====================stop=======================\n");
	return 0;
}

//步进
int Playback_Step(int _iPlayid)		//步进回放
{
	if(g_PlayBkVo[_iPlayid] == NULL)
		return -1;
	g_TPlayContrl.TPlaySingel[_iPlayid].m_ExternalInterface.m_iRunFlagBak = PLAYBACK_STAT_STEP;
	Playback_Resume(_iPlayid);

	td_printf(0,"=====================step=======================%d\n",_iPlayid);
	return 0;
}
//步退
int Playback_BackStep(int _iPlayid)		//步进回放
{
	int i = 0;
	TPlayerStat tStat;
	if(g_PlayBkVo[_iPlayid] == NULL)
		return -1;
	Playback_Resume(_iPlayid);
	while(i++ < 100)
	{
		Playback_GetCurStat(_iPlayid, &tStat);
		if((1 == tStat.m_iSpeed) ||(tStat.m_iStat == PLAYBACK_STAT_STOP || tStat.m_iStat == PLAYBACK_STAT_INIT))
		{
			break;
		}
		usleep(1000);
	}
	g_TPlayContrl.TPlaySingel[_iPlayid].m_ExternalInterface.m_iRunFlagBak = PLAYBACK_STAT_BACKSTEP;

	td_printf(0,"=====================step=======================%d\n",_iPlayid);
	return 0;
}

int Playback_Resume(int _iPlayid)
{
	if(g_PlayBkVo[_iPlayid] == NULL)
	{
		return -1;
	}
	g_TPlayContrl.TPlaySingel[_iPlayid].m_PlayBack.m_dSpeed = 1;
	Playback_SetSpeed(_iPlayid, g_TPlayContrl.TPlaySingel[_iPlayid].m_PlayBack.m_dSpeed);
	return 0;
}

int Playback_Get_NextSpeedX(int _iCurrentSpeedX10)
{
	int iSpeed = 0;
	switch(_iCurrentSpeedX10)
	{
		case 10:	
			iSpeed = SpeedFastTwo;
			break;
		case SpeedFastTwo: 
			iSpeed = SpeedFastFour;
			break;
		case SpeedFastFour: 
			iSpeed = SpeedFastEight;
			break;
		case SpeedFastEight: 
			iSpeed = SpeedFastMax;
			break;
		default : iSpeed = 10;
	}
	return iSpeed;
}

int Playback_Get_BefSpeedX(int _iCurrentSpeedX10, int *_piSpeed)
{
	int iSpeed = 0;
	switch(_iCurrentSpeedX10)
	{
		case SpeedFastMax:	
			iSpeed = SpeedFastEight;
			*_piSpeed = 8;
			break;
		case SpeedFastEight: 
			iSpeed = SpeedFastFour;
			*_piSpeed = 4;
			break;
		case SpeedFastFour: 
			iSpeed = SpeedFastTwo;
			*_piSpeed = 2;
			break;
		default : iSpeed = 10;
			*_piSpeed = 1;
	}
	return iSpeed;
}

int Playback_Fast(int _iPlayid, int _iCheck)//回放加速
{
	int temp;
	int SpeedPressure = 0;
	int Pressure = 0;
	double dSpeed = 0;
	TPlayerStat PlayStat;
	TPlayerInterface *pExInterface = &(g_TPlayContrl.TPlaySingel[_iPlayid].m_ExternalInterface);
	if(g_PlayBkVo[_iPlayid] == NULL)
	{
		return -1;
	}
	
	if(g_iAsyOrsynFlag != 1)
	{
		Playback_GetCurStat(_iPlayid, &PlayStat);
		if(PlayStat.m_iStat == PLAYBACK_STAT_PAUSE)
		{
			return 0;
		}
	}
	if(_iCheck == 1)
	{
		dSpeed = PlayStat.m_dSpeed;
		int iSpeedX10 = Playback_Get_NextSpeedX((int)(dSpeed * 10));
		Playback_CalPressure(_iPlayid, &SpeedPressure, &Pressure, iSpeedX10);
		if(SpeedPressure > MAX_DECODE_SRATE)
		{

			Playback_SetLimit(SPEEDLIMITFLAG,_iPlayid);
			#if 1
			if(!(pExInterface->m_iRunFlagBak == PLAYBACK_STAT_BACK ||
			pExInterface->m_iRunFlagBak == PLAYBACK_STAT_BACKSTEP))
			#endif
			{
			Playback_SetSpeedEx(_iPlayid, MAXSPEED);
			}

			return 0;
		}
	}

	#if 1
	if(pExInterface->m_iRunFlagBak == PLAYBACK_STAT_BACK ||
		pExInterface->m_iRunFlagBak == PLAYBACK_STAT_BACKSTEP)
	{
		if(8 == g_TPlayContrl.TPlaySingel[_iPlayid].m_PlayBack.m_dSpeed)
		{
			Playback_SetLimit(SPEEDLIMITFLAG,_iPlayid);
			return 0;
		}
	}
	#endif
	
	g_TPlayContrl.TPlaySingel[_iPlayid].m_PlayBack.m_dSpeed *= 2;
	if(g_TPlayContrl.TPlaySingel[_iPlayid].m_PlayBack.m_dSpeed >= 16)
	{
		g_TPlayContrl.TPlaySingel[_iPlayid].m_PlayBack.m_dSpeed = 16;
	}
	
	if(g_TPlayContrl.TPlaySingel[_iPlayid].m_PlayBack.m_dSpeed >= 1) 
	{
		temp =(int)g_TPlayContrl.TPlaySingel[_iPlayid].m_PlayBack.m_dSpeed;
	}
	else
	{
		temp = 0-(int)(1/g_TPlayContrl.TPlaySingel[_iPlayid].m_PlayBack.m_dSpeed);
	}
	Playback_SetSpeed(_iPlayid, temp );
	return 0;
}
int Playback_Slow(int _iPlayid, int _iCheck)//回放减速
{
	int temp;
	int SpeedPressure = 0;
	int Pressure = 0;
	double dSpeed = 0;
	TPlayerStat PlayStat;
	if(g_PlayBkVo[_iPlayid] == NULL)
	{
		return -1;
	}
	if(g_iAsyOrsynFlag != 1)
	{
		Playback_GetCurStat(_iPlayid, &PlayStat);
		if(PlayStat.m_iStat == PLAYBACK_STAT_PAUSE)
		{
			return 0;
		}
	}

	if(_iCheck == 1)
	{
		dSpeed = PlayStat.m_dSpeed;
		int iSpeedX10 = 0;
		int iSpeed = 0;
		td_printf(0,"dSpeed = %f",dSpeed);
		if(dSpeed > 1)
		{
			int iSpeedLimitFlag= 0;
			for(;;)
			{
				iSpeedX10 = Playback_Get_BefSpeedX((int)(dSpeed * 10), &iSpeed);
				if(iSpeedX10 == 10)
				{
					break;
				}
				dSpeed = iSpeedX10 / 10.0; 
				Playback_CalPressure(_iPlayid,&SpeedPressure, &Pressure, iSpeedX10);
				if(SpeedPressure <= MAX_DECODE_SRATE)
				{
					break;
				}
				iSpeedLimitFlag++;
			}
			Playback_SetSpeedEx(_iPlayid, iSpeed);
			if(0 != iSpeedLimitFlag)
			{
				Playback_SetLimit(SPEEDLIMITFLAG,_iPlayid);
			}
			return 0;
		}
					
	}
	g_TPlayContrl.TPlaySingel[_iPlayid].m_PlayBack.m_dSpeed /= 2.0;
	if(g_TPlayContrl.TPlaySingel[_iPlayid].m_PlayBack.m_dSpeed <= 1.0/16)
	{
		g_TPlayContrl.TPlaySingel[_iPlayid].m_PlayBack.m_dSpeed = 1.0/16;
	}
	if(g_TPlayContrl.TPlaySingel[_iPlayid].m_PlayBack.m_dSpeed >= 1) 
	{
		temp =(int)g_TPlayContrl.TPlaySingel[_iPlayid].m_PlayBack.m_dSpeed;
	}
	else
	{
		temp = 0-(int)(1/g_TPlayContrl.TPlaySingel[_iPlayid].m_PlayBack.m_dSpeed);
	}
	Playback_SetSpeed(_iPlayid, temp );
	return 0;
}

// 名称：playback_SetSpeed
// 功能：设置回放速度
// 参数：
//	_iPid		:回放器句柄
//	_iSpeed		:回放速度，正数N表N倍速（快放），负数_N表1/N倍速（慢放）
// 返回值：0成功，-1失败
int Playback_SetSpeed(int _iPlayid, int _iSpeed)
{
	double dSpeed = 0;
	int iSpeed = _iSpeed;
	if(g_PlayBkVo[_iPlayid] == NULL)
		return -1;
	if(_iSpeed > MAXSPEED)
		return SPEEDMAXOVER;
	if(_iSpeed < MINSPEED)
		return SPEEDMINOVER;
	if(_iSpeed > -1 && _iSpeed < 1)
		return SPEEDSETERR;
	Playback_SpeedConversion(&dSpeed,&iSpeed,0);
	g_TPlayContrl.TPlaySingel[_iPlayid].m_ExternalInterface.m_dSpeedBak = dSpeed;
	td_printf(0,"=====================change speed to %d,real %f=======================\n",_iSpeed,dSpeed);
	return 0;
}
//flag == 0:从INT --> DOUBLE
//flag == 1:从DOUBLE --> INT
static int Playback_SpeedConversion(double *_dSpeed,int *_iSpeed,int flag)
{
	if(flag == 0)
	{
		if(*_iSpeed < 0)
		{
			*_dSpeed = (*_iSpeed) * 1.0;
		}
		else
		{
			switch(*_iSpeed)
			{
				case 2 : 
					*_dSpeed = SpeedFastTwo/10.0;
					break;
				case 4 :
					*_dSpeed = SpeedFastFour/10.0;
					break;
				case 8 :
					*_dSpeed = SpeedFastEight/10.0;
					break;
				case 16 :
					*_dSpeed = SpeedFastMax/10.0;
					break;
				default :
					*_dSpeed = 1.0;
			}
		}
	}
	else
	{
		if(*_dSpeed < 0)
		{
			*_iSpeed = (int)(*_dSpeed);
		}
		else
		{
			switch( (int)(*_dSpeed * 10) )
			{
				case SpeedFastTwo : 
					*_iSpeed = 2;
					break;
				case SpeedFastFour :
					*_iSpeed = 4;
					break;
				case SpeedFastEight :
					*_iSpeed = 8;
					break;
				case SpeedFastMax :
					*_iSpeed = 16;
					break;
				default :
					*_iSpeed = 1;
			}
		}
	}
	return 0;
}

static int ExSpeed(int _iPlayid, int _iSpeed)
{
	int temp = _iSpeed;
	if(_iSpeed > 0)
	{
		g_TPlayContrl.TPlaySingel[_iPlayid].m_PlayBack.m_dSpeed = temp;
	}
	else if(_iSpeed < 0)
	{
		temp = _iSpeed * (-1);
		g_TPlayContrl.TPlaySingel[_iPlayid].m_PlayBack.m_dSpeed = 1.0/temp;
	}
	else
	{
		g_TPlayContrl.TPlaySingel[_iPlayid].m_PlayBack.m_dSpeed = 1.0;
	}
	
	if(g_TPlayContrl.TPlaySingel[_iPlayid].m_PlayBack.m_dSpeed >= 1) 
	{
		temp =(int)g_TPlayContrl.TPlaySingel[_iPlayid].m_PlayBack.m_dSpeed;
	}
	else
	{
		temp = 0-(int)(1/g_TPlayContrl.TPlaySingel[_iPlayid].m_PlayBack.m_dSpeed);
	}
	Playback_SetSpeed(_iPlayid, temp );
	return 0;
}

int Playback_SetSpeedEx(int _iPlayid, int _iSpeed)
{
	TPlayerStat tStat;
	if(g_PlayBkVo[_iPlayid] == NULL)
	{
		return -1;
	}
	Playback_GetCurStat(_iPlayid, &tStat);
	if(tStat.m_iStat == PLAYBACK_STAT_PAUSE)
	{
		return 0;
	}
	return ExSpeed(_iPlayid, _iSpeed);
}

int Playback_Sync_SpeedEx(int _iSpeed)
{
	if(g_synPlayBk == NULL)
	{
		return -1;
	}
	gAsyContrl.m_asySpeedEx = _iSpeed;
	return 0;
}
#if 1
int Playback_Sync_Do_SpeedEx(int _iSpeed)
{
	int i = 0;
	TPlayerStat tStat;
	Playback_GetSynCurStat(&tStat);
	if(tStat.m_iStat == PLAYBACK_STAT_PAUSE)
	{
		return 0;
	}
	for(i = 0; i < MAXPLAYER; i++)
	{
		ExSpeed(i, _iSpeed);
	}		
	return 0;
}
#endif

int Playback_PlayAudio(int _iPlayid)
{
	int i = 0;
	
	pthread_mutex_lock(&g_PlayBkLock);
	g_TPlayContrl.TPlayAll.m_iAudioChannal = _iPlayid;
	if(NULL == g_PlayBkVo[_iPlayid])
	{
		pthread_mutex_unlock(&g_PlayBkLock);
		return td_failure;
	}
	

	for(i = 0; i < MAXPLAYER; i++)
	{
		TPlayBack *Playback = g_PlayBkVo[i];
		if(Playback != NULL && i != _iPlayid)
		{
			g_TPlayContrl.TPlaySingel[i].m_ExternalInterface.iATrueFalseBak = 0;
			pthread_mutex_lock(&Playback->AudioLock);
			Playback->m_pDc.DestroyAo(&Playback->m_pDc.m_Stream);
			pthread_mutex_unlock(&Playback->AudioLock);
		}
	}
	
	g_TPlayContrl.TPlaySingel[_iPlayid].m_ExternalInterface.iATrueFalseBak = 1;
	pthread_mutex_unlock(&g_PlayBkLock);
	return 0;
}

int Playback_StopAudio(int _iPlayid)//只要创建了播放器就可以关闭声音//ok
{	
	int i = 0;
	if(g_PlayBkVo[_iPlayid] == NULL)
	{
		return -1;
	}
	for(i = 0; i < MAXPLAYER;i++)
	{
		g_TPlayContrl.TPlaySingel[i].m_ExternalInterface.iATrueFalseBak = 0;
	}
	return 0;
}

//全屏播放
static int Playback_SetFullScreenNolock(int _iPlayid)//只有创建且不是空文件的时候，才可以全屏
{
	int i = 0;
	int iFullflag = 1;
	int iRet = td_success;
	TPlayerStat tStat;

	TPlayBack *Playback = g_PlayBkVo[_iPlayid];
	if(Playback == NULL)
	{
		return td_failure;
	}

	if((Playback->m_RunStat.m_iFullScreen == 1) && (g_TPlayContrl.TPlayAll.m_FullPlay == _iPlayid))
	{
		//iFullflag = 0;
		goto leave2;		
	}
	Playback_GetCurStat(_iPlayid, &tStat);
	if(tStat.m_iBlackScreen == 1)//空文件时仍然可以全屏2012.5.10放开
	{
		//iRet = -1;
		//goto leave2;
	}
	for(i = 0; i < MAXPLAYER; i++)
	{
		Playback = g_PlayBkVo[i];
		if(i == _iPlayid)
		{
			if(-1 == g_EzoomHand)
			{
				g_TPlayContrl.TPlaySingel[i].iShowFlag = 1;
			}
			
			if(Playback != NULL)
			{
				Playback->m_pDc.VoContrl(&Playback->m_pDc.m_Stream,VO_HIDE);
			}
		}
		else
		{
			g_TPlayContrl.TPlaySingel[i].iShowFlag = 0;
			if(Playback != NULL)
			{
				Playback->m_pDc.VoContrl(&Playback->m_pDc.m_Stream,VO_HIDE);
			}
		}

	}

	//////////////////////////////////////////
leave2:
	for(i = 0; i < MAXPLAYER; i++)
	{
		Playback = g_PlayBkVo[i];
		if(i == _iPlayid)
		{
			if(Playback != NULL)
			{
				if(-1 == g_EzoomHand)
				{
					if(iFullflag)
					{
						Playback->m_pDc.FullScr(&Playback->m_pDc.m_Stream);
					}
					Playback->m_pDc.VoContrl(&Playback->m_pDc.m_Stream,VO_SHOW);
					Playback->m_RunStat.m_iFullScreen = 1;
					break;
				}
			}
		}
	}
	g_TPlayContrl.TPlayAll.m_FullPlay = _iPlayid;
	//////////////////////////////////////////
	td_printf(0,"=====================full screen=======================%d, iRet = %d\n",_iPlayid, iRet);
	return iRet;
}

int Playback_SetFullScreen(int _iPlayid)//只有创建且不是空文件的时候，才可以全屏
{
	int iRet = 0;
	pthread_mutex_lock(&g_PlayBkLock);
	iRet = Playback_SetFullScreenNolock(_iPlayid);
	pthread_mutex_unlock(&g_PlayBkLock);
	return iRet;
}
//退出全屏
static int Playback_ESCFullScreenNolock(int _iPlayid)//只要创建就可以退出全屏
{
	int i = 0;
	int iRet = 0;
	TPlayBack *Playback;
	for(i = 0; i < MAXPLAYER; i++)
	{
		Playback = g_PlayBkVo[i];
		if(-1 == g_EzoomHand)
		{
			g_TPlayContrl.TPlaySingel[i].iShowFlag = 1;
		}
		
		if(Playback != NULL && 1 == Playback->m_RunStat.m_iFullScreen)
		{
			Playback->m_RunStat.m_iFullScreen = 0;
			Playback->m_pDc.VoContrl(&Playback->m_pDc.m_Stream,VO_HIDE);
			Playback->m_pDc.FullScrExit(&Playback->m_pDc.m_Stream, &g_TPlayContrl.TPlaySingel[i].m_PlayBack.m_TVoRect);			
		}
	}
	for(i = 0; i < MAXPLAYER; i++)
	{
		Playback = g_PlayBkVo[i];
		if(NULL == Playback)
		{
			continue;
		}
		if(-1 == g_EzoomHand)
		{
			Playback->m_pDc.VoContrl(&Playback->m_pDc.m_Stream,VO_SHOW);
		}
	}
	g_TPlayContrl.TPlayAll.m_FullPlay = -1;
	
	td_printf(0,"=====================exit full screen=======================%d, iRet = %d\n",_iPlayid, iRet);
	return iRet;
}
int Playback_ESCFullScreen(int _iPlayid)//只要创建就可以退出全屏
{
	int iRet = 0;
	pthread_mutex_lock(&g_PlayBkLock);
	iRet = Playback_ESCFullScreenNolock(_iPlayid);
	pthread_mutex_unlock(&g_PlayBkLock);
	return iRet;
}
// 名称：playback_GetCurStat
// 功能：获取当前回放状态
// 参数：
//	_iPid		:回放器句柄
//	_ptStat	:状态信息（输出）
// 返回值：0成功能，-1失败
int Playback_GetCurStat(int _iPlayid, OUT TPlayerStat *_ptStat)
{
	if(g_PlayBkVo[_iPlayid] == NULL)
	{
		//在播放器未创建的时候，回复的是初始值的状态
		//当播放器创建过，但是现在结束了此时的状态是STOP状态
		memcpy(_ptStat,&g_TPlayContrl.TPlaySingel[_iPlayid].m_RunStatBak,sizeof(TPlayerStat));
		return -1;
	}
	
	pthread_mutex_lock(&g_CpStatsLock);
	memcpy(_ptStat,&g_TPlayContrl.TPlaySingel[_iPlayid].m_RunStatBak,sizeof(TPlayerStat));
	pthread_mutex_unlock(&g_CpStatsLock);

	Playback_SpeedConversion(&g_TPlayContrl.TPlaySingel[_iPlayid].m_RunStatBak.m_dSpeed, &(_ptStat->m_iSpeed), 1);//转换一下速度
	return 0;
}


int Playback_SetDecodePassword(int _iPlayid, char* _cPassword)//ok
{
	int iPwdLen = strlen(_cPassword);
	if(g_PlayBkVo[_iPlayid] == NULL)
	{
		return -1;
	}
	if(iPwdLen <= 0 || iPwdLen > ENCRYPT_PWD_LENGTH)
	{
		return -2;
	}
	
	pthread_mutex_lock(&g_encryptlock);
	td_printf(0, "3 m_ucEncryptBak:%s, _cPassword:%s\n",g_TPlayContrl.TPlaySingel[_iPlayid].m_ucEncryptBak,_cPassword);
	memset(g_TPlayContrl.TPlaySingel[_iPlayid].m_ucEncryptBak, 0, ENCRYPT_PWD_LENGTH+1);
	memcpy(g_TPlayContrl.TPlaySingel[_iPlayid].m_ucEncryptBak, _cPassword, iPwdLen);
	pthread_mutex_unlock(&g_encryptlock);
	g_TPlayContrl.TPlaySingel[_iPlayid].m_ExternalInterface.iEncrypt = 1;

	return 0;
}

//在跳转的时候传入跳转点距离文件开始的时间差(ms)
//函数功能:记录从文件开始到现在点播放的时间进度(ms)
//函数返回:返回上述值
static int Playback_SetCurrentFTime(int _iPlayid, int _ibaseTime)
{
	TPlayBack *Playback = g_PlayBkVo[_iPlayid];
	if(_ibaseTime != -1)//跳转的过程中
	{
		//进度的基准时间改变
		Playback->m_RunStat.m_iTimeSeg = _ibaseTime > 0 ? _ibaseTime : 0;
		Playback->m_uiTimePot = 0xffffffff;
	}
	else
	{
		if(0 == Playback->m_pDc.m_Stream.m_iBackFlag)
		{			
			if(Playback->m_uiTimePot < Playback->m_u32TimeStamp && Playback->m_uiTimePot + 100 * 1000> Playback->m_u32TimeStamp)
			{
				Playback->m_RunStat.m_iTimeSeg += (Playback->m_u32TimeStamp -Playback->m_uiTimePot);//时间进度
			}
		}
		else
		{
			if(Playback->m_u32TimeStamp < Playback->m_uiTimePot && Playback->m_u32TimeStamp + 100 * 1000> Playback->m_uiTimePot)
			{
				Playback->m_RunStat.m_iTimeSeg -= (Playback->m_uiTimePot - Playback->m_u32TimeStamp);//时间进度
			}
		}
		Playback->m_uiTimePot = Playback->m_u32TimeStamp;
	}
	if(g_iAsyOrsynFlag == 0)
	{
		Playback->m_RunStat.m_tCurPlayTm = (double)(Playback->m_RunStat.m_iTimeSeg )/(Playback->m_ptFileEndTm - Playback->m_ptFileBeginTm) * 100;
		if(Playback->m_RunStat.m_iFinishFileFlag != 1)
		{
			if(Playback->m_RunStat.m_tCurPlayTm == 100000)
				Playback->m_RunStat.m_tCurPlayTm = 99999;
		}
		else
		{
			Playback->m_RunStat.m_tCurPlayTm = 100000;
		}
	}
	else
	{
		Playback->m_RunStat.m_tCurPlayTm = (double)(Playback->m_RunStat.m_iTimeSeg )/(gAsyContrl.m_u32ReqStopTime - gAsyContrl.m_u32ReqStartTime)*100;
	}
	return Playback->m_RunStat.m_iTimeSeg;
}

int Playback_SetPoliticsVideoEncrypt(int _iPlayid, char *_cPasswd)
{
	if (NULL != _cPasswd && strlen(_cPasswd) > 0)
	{
		if (_iPlayid < g_TPlayContrl.TPlayAll.m_iCnt)
		{
			strncpy((char *)g_TPlayContrl.TPlaySingel[_iPlayid].m_ucEncryptBak, _cPasswd, ENCRYPT_PWD_LENGTH);
		}
	}
	return 0;
}


int Playback_UkStart(int _iPlayid, IN char *_strFile)
{
	int iRet = td_failure;

	TPlayBack *Playback = g_PlayBkVo[_iPlayid];
	if(!_strFile || (-1 != g_TPlayContrl.TPlayAll.m_FullPlay && _iPlayid != g_TPlayContrl.TPlayAll.m_FullPlay))
	{
		goto leave;
	} 
	td_printf(0,"%s,_strFile = %s, Playback = %p, %d, full %d, player %d",__func__,_strFile,Playback, strcmp(g_TPlayContrl.TPlaySingel[_iPlayid].m_cFile, _strFile), g_TPlayContrl.TPlayAll.m_FullPlay ,_iPlayid);
	if(Playback != NULL && strcmp(g_TPlayContrl.TPlaySingel[_iPlayid].m_cFile, _strFile) == 0)
	{

		if(!Playback_PauseTime(_iPlayid, 500 * 1000))
		{
			Playback_Seek(_iPlayid, 0);
			iRet = _iPlayid;
			goto leave;
		}
	}
	
	if(Playback_Init(_iPlayid) < 0)
	{
		td_printf(0, "=============|||||||||||||||||||||||||||====================tReq.m_iVoId - 1 = %d\n\r",_iPlayid);
		goto leave;
	}
	iRet = Playback_SetPlayeFile(_iPlayid, _strFile, 0, 0, 0);
	if (iRet < 0)
	{
		Playback_DestroyPlayer(_iPlayid);
		Playback_StatInit(_iPlayid);
		td_printf(0, "%s :%d, openFile, iRet = %d\n", __func__, __LINE__,iRet);
		goto leave;
	}
	int iHandle = Playback_CreatePlayer(_iPlayid);
	if (iHandle < 0)
	{
		td_printf(0, "%s :%d, Play, iHandle = %d\n", __func__, __LINE__, iHandle);
		goto leave;
	}
	if(Playback_Start(_iPlayid) < 0)
	{
		goto leave;
	}
	iRet = iHandle;
leave:
	return iRet;
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
///////////////////////同步回放代码/////////////////////////////////

////////////////////激活代码//////////////////////////////////////////
//激活同步播放器，如果尚有以前的播放资源，首先销毁
int  Playback_Sync_Active(int _s32VoDev, int _iVoCnt, TRect *_pVoRect)
{
	int i =0;
	if(_iVoCnt <= 0)
	{
		return -1;
	}
	
	gAsyContrl.m_asyStop = 1;

	if(Playback_WaitUinit(-1, 10 * 1000 * 1000, 1))
	{
		return -4;
	}
		
	g_iAsyOrsynFlag = 1;
	g_iStat = PLAYBACK_STAT_INIT;
	if(!g_synPlayBk)
	{
		g_synPlayBk = (TsynPlayBack *)malloc(sizeof(TsynPlayBack));
	}
	else
	{
		for(i = 0; i < MAXPLAYER; i++)
		{
			if(g_synPlayBk->m_TFileListMsg[i].m_ptFileListInfo)
			{
				free(g_synPlayBk->m_TFileListMsg[i].m_ptFileListInfo);
			}
		}
	}
	
	if(g_synPlayBk == NULL)
	{
		g_iAsyOrsynFlag = 0;
		td_printf(0xff0000, "%s(%d):malloc(%d), error.", __func__, __LINE__, sizeof(TsynPlayBack));
		return -3;//激活同步回放失败
	}

	memset(g_synPlayBk, 0,sizeof(TsynPlayBack));
	
	if(_iVoCnt > MAXPLAYER)
	{
		_iVoCnt = MAXPLAYER;
	}
	g_TPlayContrl.TPlayAll.m_iCnt = _iVoCnt;//创建的播放器数量
	for(i = 0; i < _iVoCnt; i++)
	{
		g_TPlayContrl.TPlaySingel[i].m_PlayBack.m_dSpeed = 1.0;
		g_TPlayContrl.TPlaySingel[i].m_PlayBack.m_s32VoDev = _s32VoDev;
		memcpy(&g_TPlayContrl.TPlaySingel[i].m_PlayBack.m_TVoRect, &_pVoRect[i], sizeof(TRect));
		g_TPlayContrl.TPlaySingel[i].m_ExternalInterface.iATrueFalseBak = 0;
		g_TPlayContrl.TPlaySingel[i].m_ExternalInterface.iEncrypt = 0;
		g_TPlayContrl.TPlaySingel[i].iShowFlag = 1;
		g_TPlayContrl.TPlaySingel[i].m_lPause[0] = 0;
		g_TPlayContrl.TPlaySingel[i].m_lPause[1] = 0;
		g_TPlayContrl.TPlaySingel[i].m_lPause[2] = 0;
	}			
	for(i = 0; i < MAXPLAYER; i++)
	{
		td_printf(0, "4 m_ucEncryptBak:%s\n", g_TPlayContrl.TPlaySingel[i].m_ucEncryptBak);
		memset(g_TPlayContrl.TPlaySingel[i].m_ucEncryptBak, 0, ENCRYPT_PWD_LENGTH+1);
		
	}
	Playback_InitLimit();
	g_TPlayContrl.TPlayAll.m_FullPlay = -1;
	return 0;
}

int Playback_StatInit(int _iPlayid)
{
	g_TPlayContrl.TPlaySingel[_iPlayid].m_RunStatBak.m_iSpeed = 1;
	g_TPlayContrl.TPlaySingel[_iPlayid].m_RunStatBak.m_iPid = -1;
	g_TPlayContrl.TPlaySingel[_iPlayid].m_RunStatBak.m_dSpeed = 1;
	g_TPlayContrl.TPlaySingel[_iPlayid].m_RunStatBak.m_iStat = PLAYBACK_STAT_INIT;
	g_TPlayContrl.TPlaySingel[_iPlayid].m_RunStatBak.m_iFinishFileFlag = 0;
	g_TPlayContrl.TPlaySingel[_iPlayid].m_RunStatBak.m_iFullScreen = 0;
	g_TPlayContrl.TPlaySingel[_iPlayid].m_RunStatBak.iATrueFalse = 0;
	g_TPlayContrl.TPlaySingel[_iPlayid].m_RunStatBak.m_iBlackScreen = -1;
	g_TPlayContrl.TPlaySingel[_iPlayid].m_RunStatBak.m_tCurPlayTm = 0;
	g_TPlayContrl.TPlaySingel[_iPlayid].m_RunStatBak.m_iTimeSeg = 0;//时间进度
	g_TPlayContrl.TPlaySingel[_iPlayid].m_RunStatBak.m_iRate = 0;
	return 0;
}

int Playback_Sync_Init(void)
{
	int i = 0;
	int j =0;
	if(g_synPlayBk == NULL)
	{
		return td_failure;
	}
	gAsyContrl.m_RecEncrypt = 0;
	gAsyContrl.m_u32ReqStartTime = 0;
	gAsyContrl.m_u32ReqStopTime = 0;
	gAsyContrl.m_asypause = 0;
	gAsyContrl.m_asyStart = 0;
	gAsyContrl.m_asySpeed = 0;
	gAsyContrl.m_asyResume = 0;
	gAsyContrl.m_asySpeedEx = 1000;
	for(i = 0; i < MAXPLAYER; i++)
	{
		g_iReady[i] = 0;
		g_OpenNextFileFlag[i] = 0;
		Playback_StatInit(i);
		if(NULL != g_synPlayBk->m_TFileListMsg[i].m_ptFileListInfo)
		{
			free(g_synPlayBk->m_TFileListMsg[i].m_ptFileListInfo);
		}
		memset(&g_synPlayBk->m_TFileListMsg[i],'\0',sizeof(TFileList));
		for(j = 0; j < MAXFILELISTLEN; j++)
		{
			g_synPlayBk->m_TFileListMsg[i].m_pFileListPlay[j].m_pFileList = NULL;
		}
	}
	return 0;
}

///////////////////获取文件列表////////////////// 
td_s32 Playback_Sync_Build_FileList(time_t _iStartTime,time_t _iStopTime,int _Chan[])
{
	int j,k,iRet;
	TQuery Query;
	int iNoFileCount = 0;
	int iPlayCount = 0;
	
	if(g_synPlayBk == NULL)
	{
		td_printf(0, "g_synPlayBk == NULL\n");	
		return td_failure;
	}
	Playback_Sync_Init();//初始化同步所需要的变量
	Query.m_uiPageSize = PAGE_SHOW_NUM;
	Query.m_iStartTime = _iStartTime - 7200;
	if(Query.m_iStartTime < 0)				//提前2小时查询，判断边界
	{
		Query.m_iStartTime = 0;
	}
	Query.m_iStopTime = _iStopTime;
	Query.m_ucFileType = FILE_REC;
	Query.m_ucRecType = 0xff;			//查询录像所有类型
	Query.m_ucDevType =0xff;

	gAsyContrl.m_u32ReqStartTime = _iStartTime;
	gAsyContrl.m_u32ReqStopTime  = _iStopTime;

	for(j=0; j<g_TPlayContrl.TPlayAll.m_iCnt; j++)
	{
		k = -1;
		Query.m_uiPageNo = 0;
		if(_Chan[j] != -1)
		{
			iPlayCount++;
			td_printf(0,"%s, %d, chan = %d, UkSubToData(_Chan[j]) = %d", __func__, __LINE__, _Chan[j], UkSubToData(_Chan[j]));
			Query.m_ucChannelNo = UkSubToData(_Chan[j]) - 1;
		}
		else
		{
			continue;
		}
	
		if(g_synPlayBk->m_TFileListMsg[j].m_ptFileListInfo == NULL)
		{
			g_synPlayBk->m_TFileListMsg[j].m_ptFileListInfo = (TFileInfo *)malloc(sizeof(TFileInfo) * MAXFILELISTLEN);
			td_printf(0," %s, %d  , malloc = %d",__func__,__LINE__, sizeof(TFileInfo) * MAXFILELISTLEN);
			if(g_synPlayBk->m_TFileListMsg[j].m_ptFileListInfo == NULL)
			{
				g_synPlayBk->m_TFileListMsg[j].m_s32FileListLength = 0;
				g_synPlayBk->m_TFileListMsg[j].m_iFileNum = FileListCheckup(j);
				continue;
			}
		}
		//查询文件列表
		Query.m_uiTotalNum = MAXFILELISTLEN - 1;
		Query.m_cReserved[0] = '\0';
		iRet = rec_QueryAllFiles(&Query, g_synPlayBk->m_TFileListMsg[j].m_ptFileListInfo);
		{
			if(0 == Query.m_uiTotalNum)
			{
				g_synPlayBk->m_TFileListMsg[j].m_s32FileListLength = 0;
				g_synPlayBk->m_TFileListMsg[j].m_iFileNum = FileListCheckup(j);
				iNoFileCount++;
				continue;
			}
			if (ERR_REC_SUCCESSFUL != iRet)
			{
				td_printf(0, "%s : Query failed %d, num = %d",__func__, iRet, Query.m_uiTotalNum);
				return td_failure;
			}
		}
		Query.m_uiTotalNum = (Query.m_uiTotalNum> MAXFILELISTLEN) ? MAXFILELISTLEN : Query.m_uiTotalNum;
		g_synPlayBk->m_TFileListMsg[j].m_s32FileListLength = Query.m_uiTotalNum;
		td_printf(0,"Query.m_uiTotalNum[%d] = %d",j,Query.m_uiTotalNum);
		Playback_Sync_ReBuild_FileList(j);
		
		td_printf(0,"g_synPlayBk->m_TFileListMsg[j].m_s32FileListLength[%d] = %d",j,g_synPlayBk->m_TFileListMsg[j].m_s32FileListLength);
		if(g_synPlayBk->m_TFileListMsg[j].m_s32FileListLength <= 0)
		{
			continue;
		}
		g_synPlayBk->m_TFileListMsg[j].m_iFileNum = FileListCheckup(j);
		td_printf(0,"g_synPlayBk->m_TFileListMsg[j].m_iFileNum[%d] = %d",j,g_synPlayBk->m_TFileListMsg[j].m_iFileNum);
		
	}

	if(iNoFileCount == iPlayCount)
	{
		return td_failure;
	}
	return td_success;
	
}
static td_s32 Playback_Sync_ReBuild_FileList(td_s32 _s32VoChn)
{
	td_s32 i,totalnum;
	td_s32 s32ReqStartTime;
	if (g_synPlayBk->m_TFileListMsg[_s32VoChn].m_s32FileListLength >= MAXFILELISTLEN)
	{
		td_printf(0,"error:FileListLength >= MAXFILELISTLEN \n");
		return -1;
	}

	s32ReqStartTime = gAsyContrl.m_u32ReqStartTime;
	totalnum = g_synPlayBk->m_TFileListMsg[_s32VoChn].m_s32FileListLength;
	int temp1 = g_synPlayBk->m_TFileListMsg[_s32VoChn].m_s32ListHead;
	for (i = 0; i < totalnum; i++)
	{
		//重新获得文件的起始时间与结束时间
		if(g_synPlayBk->m_TFileListMsg[_s32VoChn].m_ptFileListInfo[i].m_iStopTime < s32ReqStartTime
			&& g_synPlayBk->m_TFileListMsg[_s32VoChn].m_ptFileListInfo[i].m_iStartTime < s32ReqStartTime
			&& g_synPlayBk->m_TFileListMsg[_s32VoChn].m_ptFileListInfo[i].m_iStartTime != 0
			&& g_synPlayBk->m_TFileListMsg[_s32VoChn].m_ptFileListInfo[i].m_iStopTime != 0)
		{			
			g_synPlayBk->m_TFileListMsg[_s32VoChn].m_s32ListHead++;
			g_synPlayBk->m_TFileListMsg[_s32VoChn].m_s32FileListLength--;
		}
	}
	temp1 = g_synPlayBk->m_TFileListMsg[_s32VoChn].m_s32ListHead;
	return 0;
}

static int FileListCheckup(int _iPlayid)
{
	unsigned int arr[2 * MAXFILELISTLEN];
	int m = 0;
	int i = 0;
	int j = 0;
	TFileList *pTFileList = &g_synPlayBk->m_TFileListMsg[_iPlayid];
	TFileInfo *pTFileInfo = pTFileList->m_ptFileListInfo;
	TFileListPlay *pTFileInfoPlay = pTFileList->m_pFileListPlay;//实际播放文件列表
	int head = g_synPlayBk->m_TFileListMsg[_iPlayid].m_s32ListHead;
	if(pTFileInfo != NULL)
	{
		for(i = 0;i < pTFileList->m_s32FileListLength;i++)
		{
			if(pTFileInfo[head + i].m_iStartTime != 0 && pTFileInfo[head + i].m_iStopTime != 0
			 && pTFileInfo[head + i].m_iStartTime != pTFileInfo[head + i].m_iStopTime)
			{
				arr[m] = i;
				m++;
			}
		}	
	}
	else
	{
		m = 0;
	}
	if(m == 0)//列表中没有文件
	{
		pTFileInfoPlay[j].m_pFileList = NULL;
		pTFileInfoPlay[j].m_NULL= 0;
		pTFileInfoPlay[j].m_iStartTime = gAsyContrl.m_u32ReqStartTime;
		pTFileInfoPlay[j].m_iEndTime =gAsyContrl.m_u32ReqStopTime;
		j = 1;
	}
	else
	{
		if(pTFileInfo[arr[0]+head].m_iStartTime > gAsyContrl.m_u32ReqStartTime + 1)//第一个文件较开始时间晚很多也就是说在文件的开始有空白
		{
			pTFileInfoPlay[j].m_pFileList = NULL;
			pTFileInfoPlay[j].m_NULL = 1;
			pTFileInfoPlay[j].m_iStartTime = gAsyContrl.m_u32ReqStartTime;
			pTFileInfoPlay[j].m_iEndTime = pTFileInfo[arr[0]+head].m_iStartTime;
			j++;
		}
		for(i = 0; i < m-1; i++)
		{
			pTFileInfoPlay[j].m_pFileList = &pTFileInfo[arr[i] + head];
			pTFileInfoPlay[j].m_NULL = 2;
			pTFileInfoPlay[j].m_iStartTime = pTFileInfo[arr[i ]+ head].m_iStartTime;
			pTFileInfoPlay[j].m_iEndTime= pTFileInfo[arr[i] + head].m_iStopTime;			
			j++;
			if(pTFileInfo[arr[i] + head].m_iStopTime + 1 < pTFileInfo[arr[i+1] + head].m_iStartTime)//有空白区域
			{
				pTFileInfoPlay[j].m_pFileList = NULL;
				pTFileInfoPlay[j].m_NULL = 1;
				pTFileInfoPlay[j].m_iStartTime = pTFileInfo[arr[i] + head].m_iStopTime;
				pTFileInfoPlay[j].m_iEndTime = pTFileInfo[arr[i+1] + head].m_iStartTime;
				j++;
			}
		}
		pTFileInfoPlay[j].m_pFileList = &pTFileInfo[arr[i] + head];//将最后一个列表添加上
		pTFileInfoPlay[j].m_NULL = 2;
		pTFileInfoPlay[j].m_iStartTime = pTFileInfo[arr[i] + head].m_iStartTime;
		pTFileInfoPlay[j].m_iEndTime = pTFileInfo[arr[i] + head].m_iStopTime;
		j++;
		//检查最后的文件后面是否还有空白，如果还有空白添加到播放列表中
		if(pTFileInfo[arr[i] + head].m_iStopTime < gAsyContrl.m_u32ReqStopTime)
		{
			pTFileInfoPlay[j].m_pFileList = NULL;
			pTFileInfoPlay[j].m_NULL = 1;
			pTFileInfoPlay[j].m_iStartTime = pTFileInfo[arr[i] + head].m_iStopTime;
			pTFileInfoPlay[j].m_iEndTime = gAsyContrl.m_u32ReqStopTime;
			j++;
		}
	}
	td_printf(0,"%s,%d,j = %d",__func__, __LINE__, j);
	return j;
}




int Playback_Sync_Start(void)
{
	int i = 0;
	int MinStartTime = 0;
	pthread_t iThreadId = 0;
	int iRet = 0;
	int iHandle = 0;
	int a[MAXPLAYER];
	int iStartTime[MAXPLAYER] = {0};
	TFileListPlay *pFileListPlay;
	int iHaveFile = 0;
	if(g_synPlayBk == NULL)
	{
		return td_failure;
	}
	Playback_InitLimit();
	for(i = 0; i < MAXPLAYER; i++)
	{
		Playback_SetTFileValue(i, 0, 0, 0);
		g_TPlayContrl.TPlaySingel[i].m_ExternalInterface.m_dSpeedBak = 0;
		if(Playback_WaitUinit(i, 1000, 1))
		{
			return td_failure;
		}
	}
	for(i = 0; i < g_TPlayContrl.TPlayAll.m_iCnt; i++)
	{
		a[i] = i;

		if(Playback_Init(i) < 0)//初始化播放器变量
		{
			td_printf(0, "=============|||||||||||||||||||||||||||====================tReq.m_iVoId - 1 = %d\n\r",i);
			Playback_StatInit(i);//初始化未成功，将初始化改变的全局变量改为初始状态
			continue;
		}
		Playback_SetCurrentFTime(i, 0);
		g_synPlayBk->m_TFileListMsg[i].m_iFileCurrent = 0;
		pFileListPlay = &g_synPlayBk->m_TFileListMsg[i].m_pFileListPlay[0];
		iRet = Playback_Test_File(i, 0);
		if(iRet == FILENULL)
		{
			Playback_Do_OverFile(i);
			Playback_DestroyPlayer(i);
		 	ClearChannelLimit(g_Tlimit.m_TPicLimit.m_iPicLimitChannel, i);
			Playback_StatInit(i);//回复改变的全局变量
			continue ;
		}
		else if(iRet == FILEBLANK)
		{
			iHaveFile = 1;
			Playback_Do_BlankFile(i, 0);
		 	ClearChannelLimit(g_Tlimit.m_TPicLimit.m_iPicLimitChannel, i);
		}	
		else
		{
			iHaveFile = 1;
			int iOpenRet = Playback_SetPlayeFile(i, (char *)pFileListPlay->m_pFileList->m_ucFileName, pFileListPlay->m_iStartTime, pFileListPlay->m_iEndTime, 0);
			if(FILE_ERR == iOpenRet ||VOVDEC_ERR == iOpenRet || -1 == iOpenRet)
			{
				PlayBackOpenerr(i, iOpenRet);
			}
		}
		//创建播放器
		g_PlayBkVo[i]->m_pDc.CreateVo(&g_PlayBkVo[i]->m_pDc.m_Stream,&g_TPlayContrl.TPlaySingel[i].m_PlayBack.m_TVoRect,g_TPlayContrl.TPlaySingel[i].m_PlayBack.m_s32VoDev);
		g_PlayBkVo[i]->m_pDc.VoContrl(&g_PlayBkVo[i]->m_pDc.m_Stream,VO_HIDE);
		
		iHandle = Playback_CreatePlayer(a[i]);
		if (iHandle < 0)
		{
			td_printf(0, "%s :%d, Play, iHandle = %d\n", __func__, __LINE__, iHandle);
			continue;
		}
		//获得开始有视频文件的时间
		int j = 0;
		for(j = 0; j < g_synPlayBk->m_TFileListMsg[i].m_iFileNum; j++)
		{
			if(Playback_Test_File(i, j) == FILEVAILD)
			{
				break;
			}
		}
		iStartTime[i] = g_synPlayBk->m_TFileListMsg[i].m_pFileListPlay[j].m_iStartTime - gAsyContrl.m_u32ReqStartTime;
		#if 1
		if(iStartTime[i] < 0)
		{
			iStartTime[i] = 0;
		}
		#endif
		MinStartTime = iStartTime[i];
	}

	pthread_mutex_lock(&g_PlayBkLock);
	int MinFileNum = 0;
	for(i = 0; i < g_TPlayContrl.TPlayAll.m_iCnt; i++)
	{
		if(g_PlayBkVo[i] != NULL)
		{
			if(MinStartTime > iStartTime[i])
			{
				MinStartTime = iStartTime[i];
				MinFileNum = i;
			}
			if(1 == g_TPlayContrl.TPlaySingel[i].iShowFlag)
			{
				g_PlayBkVo[i]->m_pDc.VoContrl(&g_PlayBkVo[i]->m_pDc.m_Stream,VO_SHOW);
			}
		}
	}
	pthread_mutex_unlock(&g_PlayBkLock);
	
	Playback_Sync_Seek(-1,MinStartTime);

	Playback_Sync_Continue();
	gAsyContrl.m_asyStop = 0;
	iRet = pthread_create(&iThreadId,NULL,(void *)PlayAsyThread,(void *)(g_TPlayContrl.TPlayAll.m_iCnt));
	if(iRet != 0)
	{
		td_printf(0,"pthread_create  PlayAsyThread = %d",iRet);
		Playback_Sync_Esc();
		return -1;
	}
	pthread_detach(iThreadId);
	if(iHaveFile)
	{
		g_iStat = PLAYBACK_STAT_PLAY;
	}
	return 0;
}

static int playback_SetParam(int _iPlayid, int _iValue)//_iValue为MS
{
	if(g_PlayBkVo[_iPlayid] == NULL)
		return -1;
	if(_iValue > 10)
		_iValue= 10;
	else if(_iValue < -10)
		_iValue = -10;
	g_PlayBkVo[_iPlayid]->m_iImperTm = _iValue;	
	return 0;
}


int Playback_Sync_Esc(void)//同步回放结束
{
	int i = 0;
	
	for(i = 0; i < MAXPLAYER; i++)
	{
		Playback_Stop(i);
		g_OpenNextFileFlag[i] = 0;
		g_TPlayContrl.TPlaySingel[i].iShowFlag = 1;
	}
	Playback_WaitUinit(-1, 30 * 1000 * 1000, 1);
	#if 0
	if(Playback_WaitUinit(-1, 30 * 1000 * 1000, 1))
	{
		Playback_Sync_Destroy();
	}
	#endif
	return 0;
}

static int Playback_SetOpenNewFile(int _iPlayid)
{
	if(g_OpenNextFileFlag[_iPlayid] == 0)
	{
		g_OpenNextFileFlag[_iPlayid] = 1;		
	}
	return 0;
}
int Playback_GuiGetSynStat(OUT PLAYBACK_STAT *_piStat)
{
	if(g_synPlayBk == NULL)
	{
		*_piStat = PLAYBACK_STAT_STOP;
		return -1;
	}
	*_piStat = g_iStat;
	return 0;
}
int Playback_WaitStat(IN PLAYBACK_STAT _iStat)
{
	int num = 0;
	int iStat = _iStat;
	while(iStat != g_iStat && ++num < 20)
	{
		usleep(5 * 1000);
	}
	if(iStat != g_iStat && num >= 20)
	{
		return -1;
	}
	return 0;
}
int Playback_GetSynCurStat(OUT TPlayerStat *_ptStat)
{
	int i = 0;
	TPlayerStat tStatTemp[MAXPLAYER];
	int iCurPlaySeg = 0;//0x7fffffff;
	int iBlackCurSeg = 0x7fffffff;
	int iBlackNum = 0;
	int num = 0;
	int Bnum = 0;
	int CreateNum = 0;
	int StopNum = 0;
	int Dif = 0;
	int iencrypt = 0;
	//在同步数据未初始化的情况返回错误
	if(g_synPlayBk == NULL)
	{
		return -1;
	}
	for(i = 0; i < g_TPlayContrl.TPlayAll.m_iCnt; i++)
	{
		Playback_GetCurStat(i,  &tStatTemp[i]);
		if(tStatTemp[i].m_iPid == -1)//该播放器未建立
		{
			continue;
		}
		CreateNum++;//创建播放器的个数
		if(tStatTemp[i].m_iStat == PLAYBACK_STAT_STOP || g_TPlayContrl.TPlaySingel[i].StopFlag == 1)//该播放器已经停止
		{
			StopNum++;
			continue;
		}
		else if(tStatTemp[i].m_iNeedSecret == 1 && iencrypt == 0)//GUI_VIDEO_PLAY_NEED_PWD)
		{
			int temp = 1;
			temp <<= i;
			iencrypt = temp;
		}
		if(tStatTemp[i].m_iBlackScreen == 1)//空文件状态
		{
			iBlackNum++;
			if(iBlackCurSeg > tStatTemp[i].m_iTimeSeg)
			{
				iBlackCurSeg = tStatTemp[i].m_iTimeSeg;
				Bnum = i;
			}
		}
		else
		{
			if(iCurPlaySeg <= tStatTemp[i].m_iTimeSeg)
			{
				
				iCurPlaySeg = tStatTemp[i].m_iTimeSeg;
				num = i;
			}
		}
	}

	if(iBlackNum + StopNum == CreateNum)//现存活的播放器全是空文件状态，使用空文件的时间进度
	{
		num = Bnum;
		
		iCurPlaySeg = iBlackCurSeg;
	}
	
	memcpy(_ptStat,&tStatTemp[num],sizeof(TPlayerStat));

	if(gAsyContrl.m_RecEncrypt == 0 && iencrypt != 0)
	{
		_ptStat->m_iNeedSecret = GUI_VIDEO_PLAY_NEED_PWD;
	}
	else if(iencrypt != 0)
	{
		_ptStat->m_iNeedSecret = GUI_VIDEO_PLAY_NEED_PWD + iencrypt;
	}
	else
	{
		_ptStat->m_iNeedSecret = 0;
	}

	if(StopNum == CreateNum && CreateNum != 0)//如果播放器全部播放完成则返回播放完成
	{
		_ptStat->m_tCurPlayTm = 100000;
		_ptStat->m_iStat = PLAYBACK_STAT_STOP;
		return 0;
	}
	pthread_mutex_lock(&g_PlayBkAsyLock);
	if(g_synPlayBk)
	{
		Dif = gAsyContrl.m_u32ReqStopTime - gAsyContrl.m_u32ReqStartTime;
	}
	pthread_mutex_unlock(&g_PlayBkAsyLock);
	if(Dif == 0)
	{
		_ptStat->m_tCurPlayTm = 0;
	}
	else
	{
		_ptStat->m_tCurPlayTm = (int)(iCurPlaySeg * 1.0 /Dif * 100);
	}

	_ptStat->m_iTimeSeg = iCurPlaySeg;
	return 0;
}
int Playback_Sync_Pause(void) 
{
	if(g_synPlayBk == NULL)
	{
		return -1;
	}
	gAsyContrl.m_asypause = 1;//暂停标志
	Playback_WaitStat(PLAYBACK_STAT_PAUSE);
	return 0;
}
int Playback_Sync_Stop(void)//ok
{
	if(g_synPlayBk == NULL)
	{
		return -1;
	}
	gAsyContrl.m_asyStop = 1;
	usleep(200 * 1000);
	Playback_WaitStat(PLAYBACK_STAT_STOP);
	return 0;
}
int Playback_Sync_Step(void)
{
	if(g_synPlayBk == NULL)
	{
		return -1;
	}
	gAsyContrl.m_asyStep = 1;
	Playback_WaitStat(PLAYBACK_STAT_STEP);
	return 0;
}
int Playback_Sync_Continue(void)
{
	if(g_synPlayBk == NULL)
	{
		return -1;
	}
	gAsyContrl.m_asyStart = 1;
	Playback_WaitStat(PLAYBACK_STAT_PLAY);
	return 0;
}

int Playback_Sync_Resume(void)
{
	if(g_synPlayBk == NULL)
	{
		return -1;
	}
	gAsyContrl.m_asyResume = 1;
	return 0;
}

int Playback_Sync_Fast(void)
{
	if(g_synPlayBk == NULL)
	{
		return -1;
	}

	gAsyContrl.m_asySpeed = 1;
	
	return 0;
}

int Playback_Sync_Slow(void)
{
	if(g_synPlayBk == NULL)
	{
		return -1;
	}
	
	gAsyContrl.m_asySpeed = -1;
	return 0;
}


static int Playback_Sync_ReadyWait(int _iNum)
{
	int i = 0;
	for(i = 0; i < _iNum; i++)
	{
		g_iReady[i] = 0;
	}
	return 0;
}

static int Playback_Sync_Wait(IN int _iWaitNum)
{	
	int i = 0;
	int temp = 0;
	for(i = 0; i < _iWaitNum; i++)
	{
		int cnt = 0;
		//while(g_PlayBkVo[i] != NULL && 1 != g_iReady[i] && cnt++ < 40000)
		while(g_PlayBkVo[i] != NULL && g_iReady[i] <= 0 && cnt++ < 40000)
		{
			usleep(1000);
		}
		if(g_iReady[i] >= 1)//跳转中出现错误,或者跳转中有密码需求
		{
			if(temp < g_iReady[i])//跳转中出现错误
			{
				temp = g_iReady[i];
			}
		}
		td_printf(0,"temp = g_iReady[%d] =%d", i, g_iReady[i]);
	}
	for(i = 0; i < _iWaitNum; i++)
	{
		if(temp == 1)
		{
			g_iReady[i] = 0;
		}
		else if(temp == 3)//跳转中产生错误
		{
			g_iReady[i] = -3;
		}
		else if(temp == 2)
		{
			g_iReady[i] = -2;//跳转中产生密码需求
		}
	}	
	return temp;
}

int Playback_Sync_Do_Fast(void)
{

	int i = 0;
	int SpeedPressure = 0;
	int Pressure = 0;
	double dSpeed = 0;
	TPlayerStat PlayStat;
	int iWaitFlag = 0;
	if(g_iAsyOrsynFlag != 1 ||  g_synPlayBk == NULL)
	{
		return 0;
	}
	
	//获得各个播放器的状态
	Playback_GetSynCurStat(&PlayStat);//获得整个同步状态
	if(PlayStat.m_iStat == PLAYBACK_STAT_PAUSE)
	{
		return 0;
	}
	
	dSpeed = PlayStat.m_dSpeed;
	int iSpeedX10 = Playback_Get_NextSpeedX((int)(dSpeed * 10));
	Playback_CalPressure( -1 ,&SpeedPressure, &Pressure, iSpeedX10);
	
	if(SpeedPressure > MAX_DECODE_SRATE)
	{
		Playback_SetLimit(SPEEDLIMITFLAG, 0);
		if((int)(dSpeed * 10) != MAXSPEED * 10)
		{
			iWaitFlag = 1;
			Playback_Sync_ReadyWait(g_TPlayContrl.TPlayAll.m_iCnt);
		}
		for(i = 0; i < MAXPLAYER; i++)
		{
			if(g_PlayBkVo[i] == NULL)
			{
				g_iReady[i] = 1;
			}
			else
			{
				Playback_SetSpeedEx(i, MAXSPEED);
			}
		}
	}
	else
	{
		if((int)(dSpeed * 10) == SpeedFastEight)
		{
			iWaitFlag = 1;
			Playback_Sync_ReadyWait(g_TPlayContrl.TPlayAll.m_iCnt);
		}
		for(i = 0; i < MAXPLAYER; i++)
		{
			if(g_PlayBkVo[i] == NULL)
			{
				g_iReady[i] = 1;
			}
			else
			{
				Playback_Fast(i, 0);//回放加速
			}
		}
	}
	
	if(iWaitFlag ==  1)
	{
		usleep(500 * 1000);
		Playback_Sync_Wait(g_TPlayContrl.TPlayAll.m_iCnt);
	}
	return 0;
}


int Playback_Sync_Do_Slow(void)
{

	int i = 0;
	int SpeedPressure = 0;
	int Pressure = 0;
	double dSpeed = 0;
	TPlayerStat PlayStat;
	if(g_iAsyOrsynFlag != 1 ||  g_synPlayBk == NULL)
	{
		return 0;
	}
	
	//获得各个播放器的状态
	Playback_GetSynCurStat(&PlayStat);//获得整个同步状态
	if(PlayStat.m_iStat == PLAYBACK_STAT_PAUSE)
	{
		return 0;
	}
	dSpeed = PlayStat.m_dSpeed;
	int iSpeedX10 = 0;
	int iSpeed = 0;
	td_printf(0,"dSpeed = %f",dSpeed);
	if(dSpeed <= 1.1 )
	{
		for(i = 0; i < MAXPLAYER; i++)
		{
			Playback_Slow(i, 0);//回放加速
		}
		return 0;
	}
	int iSpeedLimitFlag = 0;
	for(;;)
	{
		iSpeedX10 = Playback_Get_BefSpeedX((int)(dSpeed * 10), &iSpeed);
		if(iSpeedX10 == 10)
		{
			break;
		}
		dSpeed = iSpeedX10 / 10.0; 
		Playback_CalPressure(-1, &SpeedPressure, &Pressure, iSpeedX10);
		if(SpeedPressure <= MAX_DECODE_SRATE)
		{
			break;
		}
		iSpeedLimitFlag++;
	}
	if(0 != iSpeedLimitFlag)
	{
		Playback_SetLimit(SPEEDLIMITFLAG, 0);
	}
	for(i = 0; i < MAXPLAYER; i++)
	{
		Playback_SetSpeedEx(i, iSpeed);
	}
	return 0;
}

int Playback_Sync_Seek(IN int _iSeekTm, int _iValue)//跳转值是异步(百分比)
{
	long long iSkipTime = 0;
	long long temp = 0;
	pthread_mutex_lock(&g_PlayBkAsyLock);
	if(_iSeekTm != -1)
	{
		if(g_iAsyOrsynFlag == 1 && g_synPlayBk != NULL)
		{
			temp = (long long)(gAsyContrl.m_u32ReqStopTime - gAsyContrl.m_u32ReqStartTime);
			temp *=  _iSeekTm;
			iSkipTime =  temp/100;
			gAsyContrl.m_AsySkip = iSkipTime + (long long)gAsyContrl.m_u32ReqStartTime * 1000;
		}
	}
	else
	{
		gAsyContrl.m_AsySkip = _iValue + gAsyContrl.m_u32ReqStartTime;
		gAsyContrl.m_AsySkip *= 1000;
	}
	pthread_mutex_unlock(&g_PlayBkAsyLock);
	return 0;
}

#if 0
//同步回放时广播密码
int Playback_Sync_SetDecodePassword(char* _cPassword)
{
	int i = 0;
	if(g_synPlayBk == NULL)
		return -1;
	for(i = 0; i < g_TPlayContrl.TPlayAll.m_iCnt;i++)
	{
		Playback_SetDecodePassword(i,_cPassword);
	}
	return 0;
}
#endif
#if 1
int Playback_Sync_SetDecodePassword(char* _cPassword)
{
	int i = 0;
	int iLoc = 0;
	int iLen = strlen(_cPassword);
	if(g_synPlayBk == NULL)
		return -1;
	for(i = 0; i < MAXPLAYER; i++)
	{
		if(g_TPlayContrl.TPlaySingel[i].m_ucEncryptBak[0] == 0)
		{
			break;
		}
	}
	iLoc = i;
	pthread_mutex_lock(&g_encryptlock);
	if(iLoc == MAXPLAYER)
	{
		iLoc = MAXPLAYER - 1;
		for(i = 0; i < MAXPLAYER -1; i++)
		{
			memcpy(g_TPlayContrl.TPlaySingel[i].m_ucEncryptBak, g_TPlayContrl.TPlaySingel[i+1].m_ucEncryptBak, ENCRYPT_PWD_LENGTH+1);	
		}
	}
	memset(g_TPlayContrl.TPlaySingel[iLoc].m_ucEncryptBak, 0, ENCRYPT_PWD_LENGTH+1);
	memcpy(g_TPlayContrl.TPlaySingel[iLoc].m_ucEncryptBak, _cPassword, iLen);
	pthread_mutex_unlock(&g_encryptlock);
	//g_synPlayBk->m_encrypt = 1;
	for(i = 0; i < MAXPLAYER; i++)
	{
		g_TPlayContrl.TPlaySingel[i].m_ExternalInterface.iEncrypt = 1;
	}
	gAsyContrl.m_RecEncrypt = 1;
	return 0;
}
#endif



//在播放过程中(包括正常播放与加减速),当各个播放器偏差较大时调整
static int Playback_Sync_Adjust_Speed(TPlayerStat *_pPlayStat, TPlayerStat *_pPlayStatSingle, int _iBasePlay)
{
	#if 1
	int i = 0;
	static int iTimeSeg[MAXPLAYER] = {0};
	static int iAddDecFlag[MAXPLAYER] = {0};
	if(_pPlayStat->m_iStat != PLAYBACK_STAT_PLAY || -1 == _iBasePlay)
	{
		return 0;
	}

	for(i = 0; i < g_TPlayContrl.TPlayAll.m_iCnt; i++)
	{
		if(_pPlayStatSingle[i].m_iPid == -1 || _pPlayStatSingle[i].m_iBlackScreen == 1 ||i == _iBasePlay || 1 == g_TPlayContrl.TPlaySingel[i].StopFlag)
		{
			iAddDecFlag[i] = 0;
			playback_SetParam(i,0);
			continue;
		}
		if(_pPlayStatSingle[i].m_iTimeSeg == iTimeSeg[i])	//16倍速时调节差一点
		{
			continue;
		}
		if(_pPlayStatSingle[i].m_iTimeSeg > _pPlayStatSingle[_iBasePlay].m_iTimeSeg + 150)//此播放器较基准播放器快
		{
			iAddDecFlag[i] = 1;
		}
		else if(_pPlayStatSingle[i].m_iTimeSeg + 150 < _pPlayStatSingle[_iBasePlay].m_iTimeSeg)//此播放器较基准播发器慢
		{
			iAddDecFlag[i] = -1;
		}

		if(iAddDecFlag[i] == 1)
		{
			if(_pPlayStatSingle[i].m_iTimeSeg <= _pPlayStatSingle[_iBasePlay].m_iTimeSeg)//此播放器较基准播放器快
			{
				iAddDecFlag[i] = 0;
			}
			else
			{
				playback_SetParam(i,0 - 1000/_pPlayStatSingle[i].m_iRate/g_TPlayContrl.TPlaySingel[i].m_PlayBack.m_dSpeed/4);
			}
		}
		else if(iAddDecFlag[i] == -1)
		{
			if(_pPlayStatSingle[i].m_iTimeSeg >= _pPlayStatSingle[_iBasePlay].m_iTimeSeg)
			{
				iAddDecFlag[i] = 0;
			}
			else
			{
				playback_SetParam(i,1000/_pPlayStatSingle[i].m_iRate/g_TPlayContrl.TPlaySingel[i].m_PlayBack.m_dSpeed/4);
			}
		}
		iTimeSeg[i] = _pPlayStatSingle[i].m_iTimeSeg;
	}	
	#endif
	return 0;
}
//在播放器播放到空白文件时的处理
static int Playback_Sync_BlankFile(TPlayerStat *_pPlayStat, TPlayerStat *_pPlayStatSingle)
{
	int i = 0;
	for(i = 0; i < g_TPlayContrl.TPlayAll.m_iCnt; i++)//当播放器中有空白文件时的操作
	{
		if(_pPlayStatSingle[i].m_iPid == -1 || 1 == g_TPlayContrl.TPlaySingel[i].StopFlag)
		{
			continue;
		}
		if(_pPlayStatSingle[i].m_iBlackScreen == 1)//如果此播放器为空白文件，根据同步回放的进度看是否将其打开
		{
			if(_pPlayStatSingle[i].m_iStat == PLAYBACK_STAT_STOP)
			{
				continue;
			}
			if(_pPlayStat->m_iTimeSeg >= _pPlayStatSingle[i].m_iTimeSeg)
			{
				//打开新的文件
				Playback_SetOpenNewFile(i);
			}
		}
	}
	return 0;
}

// 同步步进的控制
//使状态发生了改变
static int Playback_Sync_Do_Step(TPlayerStat *_pPlayStat, TPlayerStat *_pPlayStatSingle, int _iSegTempMin)
{
	td_printf(0,"_pPlayStat->m_iStat = %d",_pPlayStat->m_iStat);
	g_iStat = PLAYBACK_STAT_STEP;
	if(_pPlayStat->m_iStat == PLAYBACK_STAT_PLAY)
	{
		Playback_Sync_Pause();
		return 0;
	}
	else
	{
		int i = 0;
		Playback_Sync_ReadyWait(g_TPlayContrl.TPlayAll.m_iCnt);
		for(i=0;i< g_TPlayContrl.TPlayAll.m_iCnt;i++)
		{
			if(_pPlayStatSingle[i].m_iPid != -1 && g_TPlayContrl.TPlaySingel[i].StopFlag != 1)
			{
				if(_pPlayStatSingle[i].m_iTimeSeg < _iSegTempMin + 500)//SegTempMin
				{
					if(-1 == Playback_Step(i))
					{
						g_iReady[i] = 1;
					}
					continue;
				}
			}
			g_iReady[i] = 1;
		}
		Playback_Sync_Wait(g_TPlayContrl.TPlayAll.m_iCnt);
		return 0;
	}
}
				
static int Playback_Sync_Do_Pause(TPlayerStat *_pPlayStat, TPlayerStat *_pPlayStatSingle,int _iRealRunCnt, int _iSegNum, int _iNoAsyFlag)
{
	int iPauseNum = _iRealRunCnt;
	int stop[MAXPLAYER] = {0};
	int cnt = 0; 
	int i = 0;
	TPlayerStat playStatSingleTemp[MAXPLAYER];
	if(_iSegNum != -1)
	{
		g_iStat = PLAYBACK_STAT_PAUSE;
		if(_pPlayStat->m_iStat == PLAYBACK_STAT_PAUSE)
		{
			return 0;
		}
		Playback_Sync_ReadyWait(g_TPlayContrl.TPlayAll.m_iCnt);
		Playback_Pause(_iSegNum);
		iPauseNum--;
		stop[_iSegNum] = 1;
		//while(iPauseNum > 0 && cnt++ < 100)
		while(cnt++ < 100)
		{
			for(i = 0; i < g_TPlayContrl.TPlayAll.m_iCnt; i++)
			{
				if(g_PlayBkVo[i] == NULL)
				{
					g_iReady[i] = 1;
					continue;
				}
				if(stop[i] == 1)
				{
					continue;
				}
				if(_pPlayStatSingle[i].m_iPid == -1 || _pPlayStatSingle[i].m_iBlackScreen == 1 ||g_TPlayContrl.TPlaySingel[i].StopFlag == 1)
				{
					Playback_Pause(i);
					stop[i] = 1;
					continue;
				}
				if(iPauseNum == 0)
				{
					continue;
				}
				Playback_GetCurStat(i,&playStatSingleTemp[i]);
				if(playStatSingleTemp[i].m_iTimeSeg < 400 || playStatSingleTemp[i].m_iTimeSeg > _pPlayStatSingle[_iSegNum].m_iTimeSeg - 400
				  || _iNoAsyFlag == 1)
				{
					Playback_Pause(i);
					iPauseNum--;
					stop[i] = 1;
				}
			}
			if(_iNoAsyFlag == 1 || iPauseNum == 0)
			{
				break;
			}
			usleep(1000);
		}
		if(cnt >= 100)
		{
			for(i = 0; i < g_TPlayContrl.TPlayAll.m_iCnt; i++)
			{
				if(-1 == Playback_Pause(i))
				{
					g_iReady[i] = 1;
				}
			}
		}

		Playback_Sync_Wait(g_TPlayContrl.TPlayAll.m_iCnt);
	}

	return 0;
}

static int Playback_Sync_Do_Seek(TPlayerStat *_pPlayStat, long long _iAsySkip)
{
	int i = 0;
	int iRet = 0;
	Playback_Sync_ReadyWait(g_TPlayContrl.TPlayAll.m_iCnt);
	for(i = 0; i < g_TPlayContrl.TPlayAll.m_iCnt; i++)//将每个播放器都跳转
	{
		if(g_PlayBkVo[i] != NULL)
		{	
			if(-1 == Playback_Seek(i, _iAsySkip))
			{
				g_iReady[i] = 1;
			}
		}
		else
		{
			g_iReady[i] = 1;
		}
	}
	iRet = Playback_Sync_Wait(g_TPlayContrl.TPlayAll.m_iCnt);
	if(iRet == 2)
	{
		return 2;
	}
#if 0
	Playback_Sync_ReadyWait(g_TPlayContrl.TPlayAll.m_iCnt);
	for(i = 0; i < g_TPlayContrl.TPlayAll.m_iCnt; i++)//将每个播放器都跳转
	{
		if(g_PlayBkVo[i] != NULL)
		{	
			Playback_Start(i);
		}
		else
		{
			g_iReady[i] = 1;
		}
	}
	Playback_Sync_Wait(g_TPlayContrl.TPlayAll.m_iCnt);
#endif
	pthread_mutex_lock(&g_PlayBkAsyLock);
	if(_iAsySkip == gAsyContrl.m_AsySkip)
	{
		gAsyContrl.m_AsySkip = 0;
	}
	pthread_mutex_unlock(&g_PlayBkAsyLock);
	return 1;
}

static int Playback_Sync_Do_Start(TPlayerStat *_pPlayStat)
{
	int i = 0;
	g_iStat = PLAYBACK_STAT_PLAY;
	if(_pPlayStat->m_iStat == PLAYBACK_STAT_PLAY)
	{
		return 0;
	}
	Playback_Sync_ReadyWait(g_TPlayContrl.TPlayAll.m_iCnt);
	
	for(i = 0; i< g_TPlayContrl.TPlayAll.m_iCnt; i++)
	{
		if(-1 == Playback_Start(i))
		{
			g_iReady[i] = 1;
		}
	}
	Playback_Sync_Wait(g_TPlayContrl.TPlayAll.m_iCnt);
	return 0;
}

static int Playback_Sync_Do_Resume(void)
{
	int i = 0;
	for(i = 0; i < g_TPlayContrl.TPlayAll.m_iCnt; i++)
	{
		Playback_Resume(i);
	}
	return 0;
}


static int Playback_Sync_Analyze_Stat(OUT TPlayerStat *_pPlayStat, OUT TPlayerStat *_pPlayStatSingle, OUT TAsyPlay *_ptAsyPlay)//OUT int *_pOverPlay,
										//OUT int *_pRealRunCnt , OUT int *_pMinPlaynum, OUT int *_pAudioPlay, OUT int *_pSegNum, OUT int *_pSegNumMin, OUT int *_pSegTempMin)
{
	int Segtemp = 0;
	int i = 0;
	Playback_GetSynCurStat(_pPlayStat);//获得整个同步状态
	for(i = 0; i < g_TPlayContrl.TPlayAll.m_iCnt; i++)
	{	
		if(g_OpenNextFileFlag[i] == 2)
		{
			g_OpenNextFileFlag[i] = 0;
		}	
		Playback_GetCurStat(i,&_pPlayStatSingle[i]);
		
		if(g_TPlayContrl.TPlaySingel[i].StopFlag == 1 || _pPlayStatSingle[i].m_iPid == -1)//未建立与完成的数(完成但是未结束，等待该控制线程的命令)
		{	
			_ptAsyPlay->m_OverPlay++;
		}
		else if(_pPlayStatSingle[i].m_iBlackScreen != 1)
		{
			_ptAsyPlay->m_RealRunCnt++;
			if(_ptAsyPlay->m_iMinPlaynum > i)//获得有视频的播放器的最小的播放器号
			{
				_ptAsyPlay->m_iMinPlaynum = i;
			}
			if(_pPlayStatSingle[i].iATrueFalse == 1)//获得播放音频的播放器
			{
				_ptAsyPlay->m_iAudioPlay = i;
			}
			if(Segtemp < _pPlayStatSingle[i].m_iTimeSeg)//获得播放进度最快的
			{
				Segtemp = _pPlayStatSingle[i].m_iTimeSeg;
				_ptAsyPlay->m_SegNum = i;
			}
			if(_ptAsyPlay->m_SegTempMin > _pPlayStatSingle[i].m_iTimeSeg)//获得播放进度最慢的
			{
				_ptAsyPlay->m_SegTempMin = _pPlayStatSingle[i].m_iTimeSeg;
				_ptAsyPlay->m_SegNumMin = i;
			}
		}
	}
	return 0;
}
static int Playback_Sync_Stat_Init(TAsyPlay *_ptAsyPlay)
{
	_ptAsyPlay->m_iAudioPlay = g_TPlayContrl.TPlayAll.m_iCnt;
	_ptAsyPlay->m_iMinPlaynum = g_TPlayContrl.TPlayAll.m_iCnt;
	_ptAsyPlay->m_OverPlay = 0;
	_ptAsyPlay->m_RealRunCnt = 0;
	_ptAsyPlay->m_SegNum = -1;
	_ptAsyPlay->m_SegNumMin = -1;
	_ptAsyPlay->m_SegTempMin = 0x7fffffff;
	return 0;
}
void *PlayAsyThread(IN int *_num)
{
	int iFirst = 0;
	int iplaystat = -1;
	TPlayerStat playStat;
	TPlayerStat playStatSingle[MAXPLAYER];
	int iPreSpeed = 0;
	int iBasePlay = -1;
	TAsyPlay tAsyPlay;
	
	func_info();
	Playback_Sync_Stat_Init(&tAsyPlay);
	while(1)
	{
		//同步回放停止
		if(gAsyContrl.m_asyStop == 1)
		{
			g_iStat = PLAYBACK_STAT_STOP;
			td_printf(0,"finish asy thread.zzzzzzzzzzzzzzzzzz......cnt = %d................\n",gAsyContrl.m_asyStop);
			break;
		}
		
		//获得各个播放器的状态
		Playback_Sync_Stat_Init(&tAsyPlay);
		Playback_Sync_Analyze_Stat(&playStat, playStatSingle, &tAsyPlay);//&OverPlay, &RealRunCnt, &iMinPlaynum, &iAudioPlay, &SegNum, &SegNumMin, &SegTempMin);

		//检测到各个播放器等待关闭，则关闭整个播放器
		if(tAsyPlay.m_OverPlay == g_TPlayContrl.TPlayAll.m_iCnt)
		{
			gAsyContrl.m_asyStop = 1;
			break;
		}
		//处理UI传来的密码
		if(playStat.m_iNeedSecret != 0 && iFirst == 0)
		{
			iplaystat = playStat.m_iStat;
			iFirst = 1;
			Playback_Sync_Do_Pause(&playStat, playStatSingle, tAsyPlay.m_RealRunCnt, tAsyPlay.m_SegNum,1);
			continue;
		}
		if(iFirst == 1 && playStat.m_iNeedSecret == 0)
		{
			iFirst = 0;
			if(iplaystat == PLAYBACK_STAT_PLAY)
			{
				Playback_Sync_Do_Start(&playStat);
			}
			continue;
		}
		if(playStat.m_iNeedSecret != 0)
		{
			continue;
		}
		//在播放过程中(包括正常播放与加减速),当各个播放器偏差较大时调整
		if(tAsyPlay.m_iAudioPlay != g_TPlayContrl.TPlayAll.m_iCnt)
		{
			iBasePlay = tAsyPlay.m_iAudioPlay;
		}
		else if(tAsyPlay.m_iMinPlaynum != g_TPlayContrl.TPlayAll.m_iCnt)
		{
			iBasePlay = tAsyPlay.m_iMinPlaynum;
		}
		else
		{
			iBasePlay = -1;
		}
		Playback_Sync_Adjust_Speed(&playStat, playStatSingle, iBasePlay);//不控制需要等待结束的线程
		//在播放器播放到空白文件时的处理
		if(tAsyPlay.m_OverPlay != g_TPlayContrl.TPlayAll.m_iCnt)
		{
			Playback_Sync_BlankFile(&playStat, playStatSingle);//
		}
		//在暂停的时候检测各个播放器的进度，保证在停止时同步
		if(gAsyContrl.m_asypause == 1)
		{
			Playback_Sync_Do_Pause(&playStat, playStatSingle, tAsyPlay.m_RealRunCnt, tAsyPlay.m_SegNum, 0);//
			gAsyContrl.m_asypause = 0;
			continue;
		}
		
		//步进时如果超过最小播放器一定限度，那么不步进，以此控制步进时的同步
		if(gAsyContrl.m_asyStep == 1)
		{
			gAsyContrl.m_asyStep = 0;
			Playback_Sync_Do_Step(&playStat, playStatSingle, tAsyPlay.m_SegTempMin);//
			continue;
		}

		//恢复正常倍速
		if(gAsyContrl.m_asyResume == 1)
		{
			gAsyContrl.m_asyResume = 0;
			Playback_Sync_Do_Resume();
			continue;
		}
		//跳转请求处理，保证在各个播放器都准备好的情况下，同一开始		
		if(gAsyContrl.m_AsySkip != 0 && playStat.m_iNeedSecret != 1)
		{
			long long llSeekValue = gAsyContrl.m_AsySkip;
			Playback_Sync_Do_Seek(&playStat, llSeekValue);
			continue;
		}
		//同步回放开始
		if(gAsyContrl.m_asyStart == 1)
		{
			gAsyContrl.m_asyStart = 0;
			Playback_Sync_Do_Start(&playStat);
			continue;
		}

		if(gAsyContrl.m_asySpeed == 1)
		{
			gAsyContrl.m_asySpeed = 0;
			Playback_Sync_Do_Fast();
			continue;
		}
		if(gAsyContrl.m_asySpeed == -1)
		{
			gAsyContrl.m_asySpeed = 0;
			Playback_Sync_Do_Slow();
			continue;
		}

		if(gAsyContrl.m_asySpeedEx != 1000)
		{
			Playback_Sync_Do_SpeedEx(gAsyContrl.m_asySpeedEx);
			gAsyContrl.m_asySpeedEx = 1000;
			continue;
		}

		//td_printf(0,"playStat.m_iSpeed = %d",playStat.m_iSpeed);
		//如果是16倍速转其他倍速(跳转)
		//如果是其他倍速转16倍速(等待0.5秒)
		if(iPreSpeed == 16 && playStat.m_iSpeed != 16)
		{
			gAsyContrl.m_AsySkip = playStat.m_iTimeSeg+(long long)gAsyContrl.m_u32ReqStartTime * 1000 + 1000;
		}

		iPreSpeed = playStat.m_iSpeed;

		usleep(1000);
	}
	//释放回放的资源
	Playback_Sync_Esc();
	td_printf(0,"finish asy thread.......................\n");
	gAsyContrl.m_asyStop = 0;
	g_iStat = PLAYBACK_STAT_STOP;
	
	return 0;
}


int Playback_Sync_Destroy(void)
{
	int i = 0;
	Playback_Sync_Stop();
	#if 0
	do
	{
		for(i =0;i< MAXPLAYER;i++)
		{
			if(g_PlayBkVo[i] != NULL)
			{
				break;
			}
		}
		usleep(10000);
	}while(i != MAXPLAYER && ++cnt < 1000);
	#endif
	pthread_mutex_lock(&g_PlayBkAsyLock);
	if(g_synPlayBk != NULL)
	{
		for(i = 0; i < MAXPLAYER; i++)
		{
			if(g_synPlayBk->m_TFileListMsg[i].m_ptFileListInfo != NULL)
			{
				free(g_synPlayBk->m_TFileListMsg[i].m_ptFileListInfo);
				g_synPlayBk->m_TFileListMsg[i].m_ptFileListInfo = NULL;
			}
		}
		free(g_synPlayBk);
		g_synPlayBk = NULL;
	}
	pthread_mutex_unlock(&g_PlayBkAsyLock);
	g_iAsyOrsynFlag = 0;
	#if 0
	if(cnt == 300)
	{
		return -1;
	}
	#endif
	return 0;
}

int Playback_Sync_Clear()
{
	Playback_WaitUinit(-1, 30 * 1000 * 1000, 1);
	Playback_Sync_Destroy();
	g_TPlayContrl.TPlayAll.m_FullPlay = -1;
	g_TPlayContrl.TPlayAll.m_iCnt = 0;
	return 0;
}

static int  Playback_StopCD(int _iPlayid)
{	
	int i;
	int iRet;
	char cPath[32]={0};
	
	td_assert(_iPlayid >= 0 && _iPlayid < MAXPLAYER, td_failure);
	//卸载光驱
	for(i=0;i<MAX_RECORDER_NUM;i++)
	{
		sprintf(cPath,"/mnt/cdrom%d",i);
		//iRet = umount(cPath);
		iRet = umount2(cPath, MNT_FORCE);
		td_printf(0,"%s iRet=%d errno=%d",__func__,iRet,errno);		
	}
	return 0;
}

int Playback_Clear(void)
{
	Playback_WaitUinit(-1, 30 * 1000 * 1000, 1);
	
	g_TPlayContrl.TPlayAll.m_FullPlay = -1;
	Playback_StopCD(0);
	//add by gyn
	g_TPlayContrl.TPlayAll.m_iCnt = 0;
	//add end
	return 0;

}


////////////////////////////////////////////////////////
//////////////电子放大//////////////////////////////
static int  Playback_EzoomNolock(int _iHandle)
{
	int iRet = 0;
	int iWidth;
	int iHeight;
	TRect Rect[2];
	TPlayerStat tStat;
	TVdecChannel *pTVDec;
	int iFlag = 0;
	
	Playback_GetCurStat(_iHandle,&tStat);
	if(g_TPlayContrl.TPlaySingel[_iHandle].StopFlag == 1 || !g_PlayBkVo[_iHandle] || tStat.m_iBlackScreen == 1)
	{
		iRet = td_failure;
		goto leave;
	}
	
	//获取当前VO设备的分辨率
	TdVO_GetVODiff(g_TPlayContrl.TPlaySingel[_iHandle].m_PlayBack.m_s32VoDev,&iWidth,&iHeight);
	Rect[0].m_iTop = 0;
	Rect[0].m_iLeft = 0;
	Rect[0].m_iWidth = iWidth;
	Rect[0].m_iHeight = iHeight;
	
	Rect[1].m_iTop = 3*iHeight/4;
	Rect[1].m_iLeft = 3*iWidth/4;
	Rect[1].m_iWidth = iWidth/4;
	Rect[1].m_iHeight = iHeight/4;
	//创建两个VO，1个用于全屏播放另一个用于小画面
	g_EzoomBigWindow = CreateVO_Ex(g_TPlayContrl.TPlaySingel[_iHandle].m_PlayBack.m_s32VoDev,&Rect[0],0);
	if(g_EzoomBigWindow == NULL)
	{
		iRet = td_failure;
		goto leave;
	}
	pTVDec = g_PlayBkVo[_iHandle]->m_pDc.m_Stream.m_pTVDec;
	iFlag = decode_getLobby(pTVDec->m_TVdecPara.m_iHeight, pTVDec->m_TVdecPara.m_iWidth);
	if(iFlag)
	{
		TRect Rect1;
		float rate1;
		rate1 = ((float)pTVDec->m_TVdecPara.m_iWidth)/((float)pTVDec->m_TVdecPara.m_iHeight);
		Rect1.m_iHeight	= Rect[0].m_iHeight;
		Rect1.m_iWidth	= Rect1.m_iHeight * rate1;
		Rect1.m_iTop		= Rect[0].m_iTop; 
		Rect1.m_iLeft		= Rect[0].m_iLeft  + ((Rect[0].m_iWidth -  Rect1.m_iWidth) >> 1);
		g_EzoomBigWindow->AdjustVoRect(g_EzoomBigWindow, &Rect1);
	}
	g_EzoomBigWindow->Bind(g_EzoomBigWindow, VO_BIND_VDEC, g_PlayBkVo[_iHandle]->m_pDc.m_Stream.m_pTVDec->m_iVdecChan, &Rect[0]);
	td_printf(0,"g_TPlayContrl.TPlaySingel[_iHandle].m_Rate = %d", g_TPlayContrl.TPlaySingel[_iHandle].m_Rate);
	g_EzoomBigWindow->SetVoFrameRate(g_EzoomBigWindow,g_TPlayContrl.TPlaySingel[_iHandle].m_Rate + BASE_VO_VDEC_FRATE);
	g_EzoomBigWindow->VoChnCtrl(g_EzoomBigWindow, VO_ENABLE);
	g_EzoomBigWindow->VoChnCtrl(g_EzoomBigWindow, VO_CLEAR);
	g_EzoomBigWindow->VoChnCtrl(g_EzoomBigWindow, VO_RESUME);
	g_EzoomBigWindow->VoChnCtrl(g_EzoomBigWindow, VO_HIDE);

	
	g_EzoomSmallWindow = CreateVO_Ex(g_TPlayContrl.TPlaySingel[_iHandle].m_PlayBack.m_s32VoDev,&Rect[1],1);
	if(g_EzoomSmallWindow==NULL)
	{
		td_printf(0, "%s      111\n", __FUNCTION__);	
		g_EzoomBigWindow->VoChnCtrl(g_EzoomBigWindow, VO_DISABLE);
		g_EzoomBigWindow->UnBind(g_EzoomBigWindow);
		DestroyVO(g_EzoomBigWindow);
		g_EzoomBigWindow = NULL;	
		iRet = td_failure;
		goto leave;
	}
	if(iFlag)
	{
		TRect Rect1;
		float rate1;
		rate1 = ((float)pTVDec->m_TVdecPara.m_iWidth)/((float)pTVDec->m_TVdecPara.m_iHeight);
		Rect1.m_iHeight	= Rect[1].m_iHeight;
		Rect1.m_iWidth	= Rect1.m_iHeight * rate1;
		Rect1.m_iTop		= Rect[1].m_iTop; 
		Rect1.m_iLeft		= Rect[1].m_iLeft  + ((Rect[1].m_iWidth -  Rect1.m_iWidth) >> 1);
		g_EzoomSmallWindow->AdjustVoRect(g_EzoomSmallWindow, &Rect1);
	}
	g_EzoomSmallWindow->Bind(g_EzoomSmallWindow, VO_BIND_VDEC, g_PlayBkVo[_iHandle]->m_pDc.m_Stream.m_pTVDec->m_iVdecChan, NULL);
	td_printf(0,"22g_TPlayContrl.TPlaySingel[_iHandle].m_Rate = %d", g_TPlayContrl.TPlaySingel[_iHandle].m_Rate);
	g_EzoomSmallWindow->SetVoFrameRate(g_EzoomSmallWindow,g_TPlayContrl.TPlaySingel[_iHandle].m_Rate + BASE_VO_VDEC_FRATE);
	g_EzoomSmallWindow->VoChnCtrl(g_EzoomSmallWindow, VO_ENABLE);
	g_EzoomSmallWindow->VoChnCtrl(g_EzoomSmallWindow, VO_CLEAR);
	g_EzoomSmallWindow->VoChnCtrl(g_EzoomSmallWindow, VO_RESUME);
	g_EzoomSmallWindow->VoChnCtrl(g_EzoomSmallWindow, VO_SHOW);
	g_EzoomBigWindow->VoChnCtrl(g_EzoomBigWindow, VO_SHOW);
	//在暂停的时候进行帧拷贝
	//if(tStat.m_iStat == PLAYBACK_STAT_PAUSE)
	{
		//g_EzoomBigWindow->VoChnCtrl(g_EzoomBigWindow, VO_PAUSE);
		g_EzoomBigWindow->CopyFrame(g_EzoomSmallWindow, g_EzoomBigWindow);
		g_EzoomBigWindow->CopyFrame(g_EzoomSmallWindow, g_EzoomSmallWindow);
		g_EzoomBigWindow->VoChnCtrl(g_EzoomBigWindow, VO_REFRESH);
	}
	iRet = td_success;
leave:
	return iRet;

}

static int  Playback_ExitEzoomNolock()
{
	int iRet = 0;
	int iHandle = g_EzoomHand;
	td_printf(0, "%s      \n", __FUNCTION__);	
	//获取当前VO设备的分辨率
	
	if(g_EzoomSmallWindow == NULL || g_EzoomBigWindow == NULL)
	{
		iRet = -1;
		goto leave;
	}
	if(iHandle <0 || iHandle >= 2 * MAXPLAYER)
	{
		iRet = -1;
		goto leave;

	}
	else if(iHandle >= MAXPLAYER)
	{
		iHandle -= MAXPLAYER;
	}
	
	if(g_EzoomSmallWindow != NULL)
	{
		g_EzoomSmallWindow->AdjustVoRect(g_EzoomSmallWindow, &g_EzoomSmallWindow->m_VoRect);
		g_EzoomSmallWindow->UnBind(g_EzoomSmallWindow);
		g_EzoomSmallWindow->VoChnCtrl(g_EzoomSmallWindow, VO_CLEAR);
		g_EzoomSmallWindow->VoChnCtrl(g_EzoomSmallWindow, VO_HIDE);
		g_EzoomSmallWindow->VoChnCtrl(g_EzoomSmallWindow, VO_DISABLE);
		td_printf(0,"%s        g_EzoomSmallWindow = %p",__func__,g_EzoomSmallWindow);
		DestroyVO(g_EzoomSmallWindow);
		g_EzoomSmallWindow =NULL;
	
	}
	
	if(g_EzoomBigWindow != NULL)
	{
		g_EzoomBigWindow->AdjustVoRect(g_EzoomBigWindow, &g_EzoomBigWindow->m_VoRect);
		g_EzoomBigWindow->UnBind(g_EzoomBigWindow);
		g_EzoomBigWindow->VoChnCtrl(g_EzoomBigWindow, VO_CLEAR);
		g_EzoomBigWindow->VoChnCtrl(g_EzoomBigWindow, VO_HIDE);
		g_EzoomBigWindow->VoChnCtrl(g_EzoomBigWindow, VO_DISABLE);
		td_printf(0,"%s        g_EzoomBigWindow = %p",__func__,g_EzoomBigWindow);
		DestroyVO(g_EzoomBigWindow);
		g_EzoomBigWindow =NULL;
	}
leave:
	return iRet;
}

static int  Playback_AdjustEzoomNolock(TRect * _pRect)
{
	int iRet = 0;
	td_printf(0, "%s      \n", __FUNCTION__);	
	int iWidth;
	int iHeight;
	TRect Rect;
	static TRect tpreRect; 
	TPlayerStat tStat;
	Decode *dec;
	int iSize = 0;
	if((g_EzoomHand <0)||(g_EzoomHand>=MAXPLAYER))
	{
		iRet = td_failure;
		goto leave;
	}
    	if(!g_PlayBkVo[g_EzoomHand])
    	{
		iRet = td_failure;
		goto leave;

    	}
	if(g_EzoomSmallWindow == NULL || g_EzoomBigWindow == NULL)
	{
		iRet = td_failure;
		goto leave;

	}
		
	if(_pRect!=NULL)
	{
		g_ZoomRect.m_iHeight= _pRect->m_iHeight;
		g_ZoomRect.m_iLeft= _pRect->m_iLeft;
		g_ZoomRect.m_iTop= _pRect->m_iTop;
		g_ZoomRect.m_iWidth= _pRect->m_iWidth;
		tpreRect = *_pRect;
	}
	else
	{
		g_ZoomRect.m_iHeight= tpreRect.m_iHeight;
		g_ZoomRect.m_iLeft= tpreRect.m_iLeft;
		g_ZoomRect.m_iTop= tpreRect.m_iTop;
		g_ZoomRect.m_iWidth= tpreRect.m_iWidth;
	}
	dec = &g_PlayBkVo[g_EzoomHand]->m_pDc;
	if(td_success != dec->GetFileAttribute(&(dec->m_Stream), &iSize,5))
	{
		iRet = td_failure;
		goto leave;
	}
	iWidth = (((unsigned int)iSize) << 16) >> 16;
	iHeight = ((unsigned int)iSize) >> 16; 
	Rect.m_iTop = g_ZoomRect.m_iTop*iHeight/576;
	Rect.m_iHeight= g_ZoomRect.m_iHeight*iHeight/576;
	Rect.m_iLeft= g_ZoomRect.m_iLeft*iWidth/704;
	Rect.m_iWidth= g_ZoomRect.m_iWidth*iWidth/704;	
	
	g_EzoomSmallWindow->SetVoFrameRate(g_EzoomSmallWindow,g_TPlayContrl.TPlaySingel[g_EzoomHand].m_Rate + BASE_VO_VDEC_FRATE);
	g_EzoomBigWindow->AdjustArea(g_EzoomBigWindow,&Rect);
	g_EzoomBigWindow->VoChnCtrl(g_EzoomBigWindow, VO_REFRESH);
		
	Playback_GetCurStat(g_EzoomHand,&tStat);
	if(tStat.m_iStat == PLAYBACK_STAT_PAUSE)//回放暂停电子放大进入此条件
	{
		g_EzoomBigWindow->CopyFrame(g_EzoomSmallWindow, g_EzoomBigWindow);
	}
leave:
	return iRet;
}

int  Playback_Ezoom(int _iHandle)
{
	int iRet = 0;
	TPlayerStat tStat;
	if(g_TPlayContrl.TPlayAll.m_iCnt > 14)
	{
		return td_failure - 1;
	}
	pthread_mutex_lock(&g_PlayBkLock);
	if(_iHandle < 0 || _iHandle >= MAXPLAYER)
	{
		iRet = td_failure;
		goto leave;
	}
	Playback_GetCurStat(_iHandle,&tStat);
	if(g_TPlayContrl.TPlaySingel[_iHandle].StopFlag == 1 || !g_PlayBkVo[_iHandle] || tStat.m_iBlackScreen == 1 
		|| PLAYBACK_STAT_BACK == g_TPlayContrl.TPlaySingel[_iHandle].m_ExternalInterface.m_iRunFlagBak ||
		PLAYBACK_STAT_BACKSTEP == g_TPlayContrl.TPlaySingel[_iHandle].m_ExternalInterface.m_iRunFlagBak)
	{
		iRet = td_failure;
		goto leave;
	}
	if(NULL == g_PlayBkVo[_iHandle])
	{
		iRet = td_failure;
		goto leave;
	}
	g_PlayBkVo[_iHandle]->m_pDc.destroy_back(&g_PlayBkVo[_iHandle]->m_pDc.m_Stream, 0);
	g_EzoomHand = _iHandle;
	g_InputEZoomStat = g_PlayBkVo[_iHandle]->m_RunStat.m_iFullScreen;
	Playback_SetFullScreenNolock(_iHandle);
	//首先将原先的VO隐藏
	g_PlayBkVo[_iHandle]->m_pDc.VoContrl(&g_PlayBkVo[_iHandle]->m_pDc.m_Stream,VO_HIDE);
	g_TPlayContrl.TPlaySingel[_iHandle].iShowFlag = 0;
	if(g_PlayBkVo[_iHandle]->m_pDc.m_Stream.m_pTVDec == NULL)
	{
		iRet = td_failure;
		goto leave;
	}
	
	iRet = Playback_EzoomNolock(_iHandle);
	g_EzoomHand = (td_success == iRet) ? _iHandle : -1;
leave:
	pthread_mutex_unlock(&g_PlayBkLock);
	return iRet;
}

int  Playback_ExitEzoom()
{
	int iRet = 0;
	int iHandle = 0;
	
	pthread_mutex_lock(&g_PlayBkLock);
	iHandle = (g_EzoomHand >= MAXPLAYER && g_EzoomHand < 2 * MAXPLAYER) ? 
				g_EzoomHand - MAXPLAYER : g_EzoomHand;

	if(iHandle < 0 || iHandle >= MAXPLAYER)
	{
		goto leave;
	}
	
	iRet = Playback_ExitEzoomNolock();
	g_EzoomHand = -1;
	
	if(g_PlayBkVo[iHandle]->m_RunStat.m_iBlackScreen == 0)
	{
		g_PlayBkVo[iHandle]->m_pDc.VoContrl(&g_PlayBkVo[iHandle]->m_pDc.m_Stream,VO_SHOW);
	}
	
	if(g_InputEZoomStat == 1)//全屏状态
	{
		Playback_SetFullScreenNolock(iHandle);
	}
	else if(g_InputEZoomStat == 0)//退出全屏状态
	{
		Playback_ESCFullScreenNolock(iHandle);
	}
leave:
	pthread_mutex_unlock(&g_PlayBkLock);
	return iRet;
}

int  Playback_AdjustEzoom(TRect * _pRect)
{
	int iRet = 0;
	pthread_mutex_lock(&g_PlayBkLock);
	iRet = Playback_AdjustEzoomNolock(_pRect);
	pthread_mutex_unlock(&g_PlayBkLock);
	return iRet;
}

//////////////////////////////////////////////////////
//////////////////预览抓拍图片////////////////////////
	
int Playback_OpenJpeg(char* _cFileName)
{
	FILE* JpegFile=NULL;
	int imgheight = 576;
	int	imgwidth = 704;
	struct stat StatTemp;
	int iJpgLen = 0;
	//td_u32 au32EOS[2] = {0x01000000, 0x0100000b};
	char *buf = NULL;
#if 1
	if((g_ptJpegVdec == NULL)||(g_ptJpegVo==NULL))
	{
		td_printf(0, "g_ptJpegVdec err\n");
		goto leave;
	}
	rec_Stat(_cFileName, &StatTemp);
	int s32ret = 0;
	td_printf(0, "open file %s\n", _cFileName);
	JpegFile = fopen(_cFileName, "r");
	if (!JpegFile)
	{
		td_printf(0, "open file %s err\n", _cFileName);
		goto leave;
	}
	//stat(_cFileName, &StatTemp);
	iJpgLen = StatTemp.st_size;
	if(iJpgLen > 1024 * 1024 || iJpgLen <= 0)
	{
		goto leave;
	}
	buf = (char *)malloc(iJpgLen);
	if(buf == NULL)
	{
		goto leave;
	}
 	unsigned short length,flag=0;
	
  	fread(&flag,sizeof(unsigned short),1,JpegFile);		//FFD8
	td_printf(0xff0000, "%s(%d): flag=0x%x", __func__, __LINE__, flag);

	if(g_ptJpegVo!=NULL)
	{
		g_ptJpegVo->AdjustVoRect(g_ptJpegVo,&g_ptJpegVo->m_VoRect);
	}

  	while(1)  
	{
     	s32ret = fread(&flag,sizeof(unsigned short),1,JpegFile);	
    	td_printf(0, "%s(%d): flag=0x%x", __func__, __LINE__, flag);	
		if(s32ret <=0)
		{
			goto leave;
		}		
		
     	s32ret  = fread(&length,sizeof(short),1,JpegFile);
    	td_printf(0, "%s(%d): length=0x%x", __func__, __LINE__, length);	
     	length=((length<<8)|(length>>8))-2;
    	td_printf(0, "%s(%d): length=0x%x", __func__, __LINE__, length);				
		if(s32ret <=0)
		{
			goto leave;
		}
		
		 if(flag==0xc0ff)
		 {	 
			td_printf(0x12345678, "%s(%d):flag==0xc0ff", __func__, __LINE__);
			fread(buf,10,1,JpegFile);
			imgheight=((*(buf+1))<<8)+(*(buf+2));
			imgwidth =((*(buf+3))<<8)+(*(buf+4));
			decode_ChangeVoDisplayArea(imgwidth,imgheight,&(g_ptJpegVo->m_VoRect),g_ptJpegVo);
        		td_printf(0, "TD_Playback_OpenJpeg imgheight:%d imgwidth :%d\n", imgheight,imgwidth);
			
			break;
		 }
		 else
		 {
			td_printf(0x12345678, "%s(%d):flag!=0xc0ff", __func__, __LINE__);
			fseek(JpegFile,length,1);
		 }
  	}
	
	fseek(JpegFile,0,SEEK_SET);
	s32ret = fread(buf, 1, iJpgLen, JpegFile);
	if(s32ret != iJpgLen)
	{
		goto leave;
	}

	td_s32 i = 0;
	for(i = 0; i < 2; i++)
	{
		//zhangyi modify 20130601		add pts
		s32ret = g_ptJpegVdec->SendStream(g_ptJpegVdec, (td_u8 *)buf, iJpgLen,0,0);
		if (td_success != s32ret)
		{
			td_printf(0, "%s(%d), send to vdec 0x%x", __func__, __LINE__, s32ret);
			//break;
		}
		td_printf(0, "%s(%d):g_ptJpegVdec->SendStream(times:%d)", __func__, __LINE__, i);
	}
#if 0	
	s32ret = g_ptJpegVdec->SendStream(g_ptJpegVdec, (td_u8 *)buf, iJpgLen,0);
	if (td_success != s32ret)
	{
		td_printf(0,"%s, send to vdec 0x%x", __FUNCTION__, s32ret);
//		break;
	}
	td_printf(0, "TD_Playback_OpenJpeg 111111111\n");
#endif
	#if 0
    	while(1)
    	{
		s32ret = fread(buf,1,MAX_READ_LEN,JpegFile);
		if(s32ret <=0)
		{
			g_ptJpegVdec->SendStream(g_ptJpegVdec,(td_u8*)au32EOS,sizeof(au32EOS),1);
			break;
		}
		s32ret = g_ptJpegVdec->SendStream(g_ptJpegVdec, buf, s32ret,1);
		if (td_success != s32ret)
        	{
			td_printf(0,"%s, send to vdec 0x %x \n", __FUNCTION__, s32ret);
            		break;
        	}
    	}
	#endif
leave:
	if(JpegFile)
	{
    	fclose(JpegFile);
	}

	if(buf)
	{
		free(buf);
	}
	/*
	s32ret = media_FreeVdecChannel(g_ptJpegVdec);
	g_ptJpegVdec = NULL;
	*/
#endif
	return 0;
}

 int Playback_ExitJpeg(td_s32 _s32VoDev)
{
	td_s32 iRes = td_success;
	if(g_ptJpegVo!=NULL)
	{
		g_ptJpegVo->UnBind(g_ptJpegVo);
		g_ptJpegVo->AdjustVoRect(g_ptJpegVo,&g_ptJpegVo->m_VoRect);
		g_ptJpegVo->VoChnCtrl(g_ptJpegVo,VO_CLEAR);
		g_ptJpegVo->VoChnCtrl(g_ptJpegVo,VO_HIDE);
		DestroyVO(g_ptJpegVo);
		g_ptJpegVo =NULL;
	}
	if(g_ptJpegVdec != NULL)
	{
		media_FreeVdecChannel(g_ptJpegVdec, 32);
		g_ptJpegVdec =NULL;
	}
	
	return iRes;
}
int Playback_InitJpeg(int _s32VoDev)
{	
	//td_s32 s32Ret = 0;
	int iWidth,iHeight;
	TRect  rect;
#if 0
	//停止预览
	s32Ret = preview_Pause(_s32VoDev);
	if(td_success != s32Ret)		
	{
		memset(u8LogMsg, 0, 128);
		snprintf(u8LogMsg,128, "file:%s line:%d, func:%s TD_Playback_Stop_Preview Error%d\n", __FILE__, __LINE__, __func__, s32Ret);	
		WRITE_LOG_DebugInfo(1,u8LogMsg);
		return s32Ret;
	}

	//创建jpeg解码通道
	//s32Ret = decode_StartJpegDec(JPEG_DEC_CHN);
#endif

	TVdecPara vDecPara;
	if (QXGA == (td_s32)MediaGetChipInfo(MEDIA_INFO_MAXSNAPDECSIZE))
	{
		vDecPara.m_iWidth =  2048;
		vDecPara.m_iHeight =  1536;
	}
	else if (F1080P == (td_s32)MediaGetChipInfo(MEDIA_INFO_MAXSNAPDECSIZE))
	{
		vDecPara.m_iWidth =  1920;
		vDecPara.m_iHeight =  1920;
	}
	else if (F720P == (td_s32)MediaGetChipInfo(MEDIA_INFO_MAXSNAPDECSIZE))
	{
		vDecPara.m_iWidth =  1280;
		vDecPara.m_iHeight =  1280;
	}
	else
	{
		vDecPara.m_iWidth =  1920;
		vDecPara.m_iHeight =  1920;
	}
	vDecPara.m_iType = VENC_MJPEG;
	vDecPara.m_iMod = 0;
	if(g_ptJpegVdec == NULL)
		g_ptJpegVdec	= media_CreateVdecChannel(&vDecPara, 0, 32);
	if(g_ptJpegVdec == NULL)
	{			
		vDecPara.m_iWidth =  704;		//内存不足，创建一个相对小的解码通道
		vDecPara.m_iHeight =  576;
		g_ptJpegVdec	= media_CreateVdecChannel(&vDecPara, 0, 32);
		if(g_ptJpegVdec == NULL)
		{
			td_printf(0xff0000, "%s(%d):g_ptJpegVdec == NULL", __func__, __LINE__);	
			return td_failure;
		}
	}
	
	//初始化VO
	TdVO_GetVODiff(_s32VoDev,&iWidth,&iHeight);
	rect.m_iTop = 0;
	rect.m_iLeft = 0;
	rect.m_iWidth = iWidth;
	rect.m_iHeight = iHeight;
	if(g_ptJpegVo == NULL)
		g_ptJpegVo=CreateVO(_s32VoDev,&rect);
	if(g_ptJpegVo == NULL)
	{
		td_printf(0xff0000, "%s(%d):g_ptJpegVo == NULL", __func__, __LINE__);	

		return td_failure;
	}

	g_ptJpegVo->Bind(g_ptJpegVo,VO_BIND_VDEC,g_ptJpegVdec->m_iVdecChan,NULL);

	//g_ptJpegVo->VoChnCtrl(g_ptJpegVo, VO_SHOW);

	return td_success;
}


//add by gyn
int Playback_GetPlayState(void)
{
	return g_TPlayContrl.TPlayAll.m_iCnt;
}

//add end

