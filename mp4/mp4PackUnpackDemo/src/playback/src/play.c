#include <stdlib.h>
#include <string.h>

#include "playback.h"
#include "decode.h"
#include "td_sdk.h"
#include "channelmnt.h"
#include "libstorage.h"
#include "td_types.h"
#include "channel.h"

#define MAXPLAYBACK 16
#define MAX_FRAME_LEN (1024 * 1024)
#define PLAYBACKCHNNO PLAYBACK_CHN
typedef struct
{
	int m_iChn;
	int m_iHistoryChn;
	int m_iSpeed;
	int m_tStat;
	int m_iHeigh;
	int m_iWith;
	int m_iFrameRate;
	PLAYFILE *m_tFileList;
	int m_iPlayId;
	int m_iBufFull;
	int m_iDataLen;
	Td_VO *m_ptVo;
	Td_VO *m_ptMyVo;
	void *m_pcBuf;
	int m_iChgStd;
	int m_iMode;		//编码模式：11（MP4)， 21(H264)
	int m_iAudioSample;
	int m_iVoDevId;
	int m_iTempStat;
	TVdecChannel *m_ptVdec;
	long long m_lUpFrameTime;
	long long m_lCurFrameTime;
	long long m_lStdFrameTime;//帧时间戳，在多种条件下是变化的
	long long m_lStdForkTime;//进程时间戳，在快慢放的时候是变化的
	pthread_mutex_t m_tLock;
}Tplayback;
td_s32 preview_ChgCurDspChn(td_s32 _iVoDev, td_s32 _iVoChn, td_s32 _iViChn);
td_s32 preview_startpage(td_s32 _iVoDevId);
td_s32 preview_stoppage(td_s32 _iVoDevId);
static unsigned int giActive = 0;
static int gInitResource = 0;
static int gInitLogicPlay = 0;
static int gInitTimePlay = 0;
static Tplayback gtPlayback[MAXPLAYBACK];
static pthread_mutex_t gPlayLock = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t gSyncMutexLock = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t gSyncCondLock = PTHREAD_COND_INITIALIZER;


TVdecChannel *playdec_CreateVdec(int _iHeight, int _iWith, int _iChn);
int playdec_SendFileVStream(TVdecChannel *_tVdecChn, void *_buf, int _iVLen);
int playdec_DestroyVdec(TVdecChannel *_tVdecChn, int _iChn);
//////////////////////////////////////////////////////////////////////////////////////////////
/////////////Play_LogicTask////////////////////////////////
/////////////////////////////////////////////////////////

static int play_StatControl(int _iId, int _iWhere)
{
	if(-1 == gtPlayback[_iId].m_iPlayId)
	{
		return 1;
	}
	
	if(PLAYBACK_STAT_STOP == gtPlayback[_iId].m_tStat) 
	{
		//可以添加播放完时的处理
		return 1;
	}

	if(PLAYBACK_STAT_PAUSE == gtPlayback[_iId].m_tStat) 
	{
		return 1;
	}
	
	if(PLAYBACK_STAT_STOP == gtPlayback[_iId].m_iTempStat)
	{
		return 1;
	}

	if(PLAYBACK_STAT_PAUSE == gtPlayback[_iId].m_iTempStat)
	{
		return 1;
	}	
	return 0;
}
static void play_sleep(unsigned int _iActive)
{
	if(0 == pthread_mutex_trylock(&gSyncMutexLock))
	{
		giActive &= (~_iActive);
		if(0 == giActive)
		{
			pthread_cond_wait(&gSyncCondLock, &gSyncMutexLock);	
		}
		pthread_mutex_unlock(&gSyncMutexLock);	
	}
}

static void play_waken(unsigned int _iActive)
{
	pthread_mutex_lock(&gSyncMutexLock);
	giActive |= ((0 == _iActive) ? (((unsigned int)(~0) << (32 - MAXPLAYBACK)) >> MAXPLAYBACK): _iActive);
	pthread_cond_signal(&gSyncCondLock);
	pthread_mutex_unlock(&gSyncMutexLock);	
}
static void play_doData(Tplayback *_ptPlayback, int _iLen, S_FrameHeader *_ptFrameHeader, S_FileHeader *_ptFileHeader, int _iDiff)
{
	long long lDif;
	long long llTmDif = (1 == _iDiff) ? 2 *1000*1000LL : 30 *1000*1000LL;
	_ptPlayback->m_iDataLen = _iLen;
	_ptPlayback->m_lUpFrameTime = _ptPlayback->m_lCurFrameTime;
	_ptPlayback->m_lCurFrameTime = (long long)_ptFrameHeader->u32TimeStamp * 1000;
	if(_ptPlayback->m_lCurFrameTime < _ptPlayback->m_lUpFrameTime || 
		_ptPlayback->m_lCurFrameTime > _ptPlayback->m_lUpFrameTime + llTmDif)
	{
		_ptPlayback->m_lUpFrameTime = _ptPlayback->m_lCurFrameTime;
		_ptPlayback->m_iChgStd  = 1;
	}
	else
	{
		int iFrameRate;
		if((lDif = _ptPlayback->m_lCurFrameTime - _ptPlayback->m_lUpFrameTime) >= 1000)
		{
			iFrameRate = 1000 / (lDif / 1000);
			if(iFrameRate > 2 * _ptPlayback->m_iFrameRate || iFrameRate < _ptPlayback->m_iFrameRate / 2)
			{
				_ptPlayback->m_iFrameRate = iFrameRate;
				//上报帧率
			}
		}
	}
	
	if(_ptPlayback->m_iHeigh != _ptFileHeader->Height || _ptPlayback->m_iWith != _ptFileHeader->Width)
	{
		_ptPlayback->m_iHeigh = _ptFileHeader->Height;
		_ptPlayback->m_iWith = _ptFileHeader->Width;
		//往上报VDEC(文件头)
		int iVideoSize = Play_WithHeightToVideoSize(_ptFileHeader->Width, _ptFileHeader->Height);
		chnmnt_ChannelSetVideoSize(PLAYBACKCHNNO, iVideoSize);
	}

	if(_ptPlayback->m_iAudioSample != _ptFileHeader->AudioSample)
	{
		_ptPlayback->m_iAudioSample = _ptFileHeader->AudioSample; 
		chnmnt_ChannelSetAudioSample(PLAYBACKCHNNO, _ptFileHeader->AudioSample);
	}
	
	_ptPlayback->m_iBufFull = 1;
}
static void play_getData(Tplayback *_ptPlayback, int _iChg)
{	
	S_FrameHeader *ptFrameHeader;
	S_FileHeader *ptFileHeader;
	int iDataLen;
	int iChgFile = 0;
	
	if(td_failure == (iDataLen = playfile_read(_ptPlayback->m_tFileList, _ptPlayback->m_pcBuf, MAX_FRAME_LEN, 0, &ptFrameHeader, &ptFileHeader, &iChgFile)))
	{
		_ptPlayback->m_tStat = PLAYBACK_STAT_STOP;
	}
	else if((td_failure - 1)== iDataLen)
	{
		_ptPlayback->m_iTempStat = PLAYBACK_STAT_PAUSE;
	}
	else
	{
		play_doData(_ptPlayback, iDataLen, ptFrameHeader, ptFileHeader, 0 == _iChg ? iChgFile : 1);
	}
	return ;
}

/////////////Play_TimeTask////////////////////////////////
/////////////////////////////////////////////////////////
#define MIN_TIME	(5 * 1000)
//获得系统时间
static long long play_GetSysTime(void)
{
	static long long lTime = 0;
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC,&ts);                      //此处可以判断一下返回值
	
	lTime = (long long)ts.tv_sec * 1000 * 1000;
	lTime += ts.tv_nsec/1000;
	return lTime;
}

static int play_GetSleepTm(long long _llInitTm, long long _llForkTm)//注意长时间时可能出现问题，待修正
{
	static long long llTm = 0;
	long long llNowTm = _llForkTm;
	
	if(0 != _llInitTm)
	{
		llTm = _llInitTm;
	}
	else
	{
		llTm += MIN_TIME;
	}

	if(llNowTm + 2000 * MIN_TIME < llTm)
	{
		llTm = _llInitTm;
	}
	
	if(llNowTm < llTm)
	{
		return (llTm - llNowTm);
	}

	return 0;
}

static int play_setStd(Tplayback *_ptPlayback)
{
	_ptPlayback->m_lStdFrameTime = _ptPlayback->m_lCurFrameTime;
	_ptPlayback->m_lStdForkTime = play_GetSysTime();
	return 0;
}
static int play_SendToDec(Tplayback *_ptPlayback, long long _LForkTime)
{
	S_FrameHeader *pFrameHeader;
	if(PLAYBACK_STAT_STOP == _ptPlayback->m_tStat)
	{
		return td_failure;
	}
	if(1 == _ptPlayback->m_iChgStd || _ptPlayback->m_lStdFrameTime > _ptPlayback->m_lCurFrameTime)
	{
		_ptPlayback->m_iChgStd = 0;
		_ptPlayback->m_lStdFrameTime = _ptPlayback->m_lCurFrameTime;
		_ptPlayback->m_lStdForkTime = _LForkTime;
	}

	if(_ptPlayback->m_lCurFrameTime - _ptPlayback->m_lStdFrameTime > _LForkTime - _ptPlayback->m_lStdForkTime)
	{	
		return td_failure;
	}

	//发送数据
	#ifndef SELF_DEC
	TPushData PushData;
	pFrameHeader = (S_FrameHeader *)(_ptPlayback->m_pcBuf);
	PushData.m_pBuf = _ptPlayback->m_pcBuf;
	PushData.m_iBufLen = _ptPlayback->m_iDataLen;
	td_printf(0,"_ptPlayback->m_pcBuf = %p, _ptPlayback->m_iDataLen = %d, pFrameHeader = %d", _ptPlayback->m_pcBuf, _ptPlayback->m_iDataLen, pFrameHeader->u8FrameType);
	chnmnt_ChannelIpChannelPushData(PLAYBACKCHNNO, &PushData);
	#else
	//pFrameHeader
	playdec_SendFileVStream(_ptPlayback->m_ptVdec, _ptPlayback->m_pcBuf + sizeof(S_FrameHeader), _ptPlayback->m_iDataLen);
	#endif
	_ptPlayback->m_iBufFull = 0;
	return td_success;
}
////////////////////////////////////////////////////////////
static int play_readyLeave()
{
	int i;
	int iRet = td_failure;
	
	if(0 != pthread_mutex_trylock(&gPlayLock))
	{
		return td_failure;
	}
	
	for(i = 0; i < MAXPLAYBACK; i++)
	{
		if(-1 != gtPlayback[i].m_iPlayId)
		{
			break;
		}
	}
	
	if(i != MAXPLAYBACK)
	{
		goto leave;
	}

	iRet = td_success;	
leave:
	pthread_mutex_unlock(&gPlayLock);
	return iRet;
}

static void *Play_LogicTask(void *arg)
{
	int i;
	int iCnt;
	unsigned int iActive = 0;
	
	func_info();
	while(gInitLogicPlay)
	{
		iCnt = 0;
		iActive = 0;
		for(i = 0; i < MAXPLAYBACK; i++)
		{
			if(0 != pthread_mutex_trylock(&gtPlayback[i].m_tLock))
			{
				continue;
			}

			iActive |= (1 << i);
			if(-1 == gtPlayback[i].m_iPlayId)
			{
				iCnt++;
				goto continu;
			}
			if(play_StatControl(i, 0) > 0)
			{
				goto continu;
			}

			if(1 == gtPlayback[i].m_iBufFull)
			{
				goto continu;
			}
			play_getData(gtPlayback + i, 0);
			
		continu:
			pthread_mutex_unlock(&gtPlayback[i].m_tLock);
		}	

		if(MAXPLAYBACK == iCnt && 1 == gInitLogicPlay)
		{
			if(td_success == play_readyLeave())
			{
				gInitLogicPlay = 0;
				break;
			}
		}
		play_sleep(iActive);
	}
	return NULL;
}

static void *Play_TimeTask(void *arg)
{
	int i;
	long long llTm;
	long long llForkTm;
	int iSend;
	int iActive;
	int iCnt = 0;
	
	func_info();
	llForkTm = play_GetSysTime();
	play_GetSleepTm(llForkTm, llForkTm);
	while(gInitTimePlay)
	{
		iCnt++;
		iActive = 0;
		
		//计算延时
		llForkTm = play_GetSysTime();
		if(0 != (llTm = play_GetSleepTm(0, llForkTm)))
		{
			usleep(llTm);
		}
		for(i = 0; i < MAXPLAYBACK; iSend = 0, i++)
		{
			if(0 != pthread_mutex_trylock(&gtPlayback[i].m_tLock))
			{
				continue;
			}
			if(-1 == gtPlayback[i].m_iPlayId)
			{
				iActive++;
				goto continu;
			}

			if(0 == gtPlayback[i].m_iBufFull)
			{
				goto continu;
			}

			if(play_StatControl(i, 0) > 0)
			{
				goto continu;
			}
			
			if(td_success == play_SendToDec(gtPlayback + i, llForkTm))
			{
				iSend = 1;
			}

		continu:
			pthread_mutex_unlock(&gtPlayback[i].m_tLock);
			if(1 == iSend)
			{
				play_waken(1 << i);
			}
		}

		if(iCnt > (1000 * 1000 / MIN_TIME))
		{
			iCnt = 0;
			play_waken(0);
		}
		
		if(MAXPLAYBACK == iActive && 1 == gInitTimePlay)
		{
			if(td_success == play_readyLeave())
			{
				gInitTimePlay = 0;
				break;
			}
		}		
	}
	play_waken(0);
	return NULL;
}
///////////////////////////////////////////////////////////
static int Play_FindFromId(int _iPlayId)
{
	int i;
	for(i = 0; i < MAXPLAYBACK; i++)
	{
		if(_iPlayId == gtPlayback[i].m_iPlayId)
		{
			return i;
		}
	}
	return td_failure;
}
static int Play_InitTask()
{
	int i;
	pthread_t threadid;

	if(0 == gInitResource)
	{
		TChnInitParam tParam;
		tParam.m_iLength = sizeof(TChnInitParam);
		tParam.m_iLogicNo = PLAYBACK_CHN+1;
		tParam.m_iType = CHN_TYPE_IP;
		tParam.m_iServerType = 2;
		tParam.m_iIpcEnable = 0;
		if (chnmnt_CreateChannel(PLAYBACK_CHN, &tParam))
		{
			goto fail;
		}
		gInitResource = 1;
		for(i = 0; i < MAXPLAYBACK; i++)
		{
			gtPlayback[i].m_iPlayId = -1;
			gtPlayback[i].m_pcBuf = NULL;
			pthread_mutex_init(&gtPlayback[i].m_tLock, NULL);			
		}
	}

	if((0 != gInitTimePlay) && (0 != gInitLogicPlay))
	{
		return td_success;
	}
	else if(0 != (gInitTimePlay | gInitLogicPlay))
	{
		return td_failure;
	}

	gInitLogicPlay = 2;
	gInitTimePlay = 2;
	if(0 != pthread_create(&threadid, NULL, Play_LogicTask, NULL))
	{
		goto fail;
	}
	if(0 != pthread_create(&threadid, NULL, Play_TimeTask, NULL))
	{
		goto fail;
	}
	return td_success;	
	
fail:
	gInitLogicPlay = 1;
	gInitTimePlay = 1;
	return td_failure;
}


static int Play_AddTask(PLAYFILE *_ptPlayFile, Tplayback *_ptPlayback)
{
	int iPlayId;
	if(td_failure == (iPlayId = Play_FindFromId(-1)))
	{
		return td_failure;
	}

	pthread_mutex_lock(&gtPlayback[iPlayId].m_tLock);
	//初始化变量
	if(NULL == (gtPlayback[iPlayId].m_pcBuf = malloc(MAX_FRAME_LEN)))
	{
		pthread_mutex_unlock(&gtPlayback[iPlayId].m_tLock);
		return td_failure;
	}
	gtPlayback[iPlayId].m_iChn = _ptPlayback->m_iChn;
	gtPlayback[iPlayId].m_iHistoryChn = _ptPlayback->m_iHistoryChn;
	gtPlayback[iPlayId].m_iSpeed = 1;
	gtPlayback[iPlayId].m_tStat = PLAYBACK_STAT_PLAY;
	gtPlayback[iPlayId].m_iHeigh = (NULL != _ptPlayback) ? _ptPlayback->m_iHeigh: 0;
	gtPlayback[iPlayId].m_iWith = (NULL != _ptPlayback) ? _ptPlayback->m_iWith: 0;
	gtPlayback[iPlayId].m_iFrameRate = (NULL != _ptPlayback) ? _ptPlayback->m_iFrameRate : 1;
	gtPlayback[iPlayId].m_ptVdec = _ptPlayback->m_ptVdec;
	gtPlayback[iPlayId].m_tFileList = _ptPlayFile;
	gtPlayback[iPlayId].m_iPlayId = iPlayId;
	gtPlayback[iPlayId].m_iBufFull = 0;
	gtPlayback[iPlayId].m_lUpFrameTime = 0;
	gtPlayback[iPlayId].m_lCurFrameTime = 0;
	gtPlayback[iPlayId].m_lStdFrameTime = 0x6fffffffffffffffLL;
	gtPlayback[iPlayId].m_lStdForkTime = play_GetSysTime();
	gtPlayback[iPlayId].m_iChgStd = 0;
	gtPlayback[iPlayId].m_iMode = (NULL != _ptPlayback) ? _ptPlayback->m_iMode: 0;
	gtPlayback[iPlayId].m_iAudioSample = (NULL != _ptPlayback) ? _ptPlayback->m_iAudioSample: 8000;
	gtPlayback[iPlayId].m_ptVo = _ptPlayback->m_ptVo;
	gtPlayback[iPlayId].m_iVoDevId = _ptPlayback->m_iVoDevId;
	gtPlayback[iPlayId].m_iTempStat = PLAYBACK_STAT_PLAY;
	gtPlayback[iPlayId].m_ptMyVo = _ptPlayback->m_ptMyVo;
	pthread_mutex_unlock(&gtPlayback[iPlayId].m_tLock);
	return iPlayId;
}

static int Play_DelTask(int _iPlayId)
{
	int iPlayId;
	if(td_failure == (iPlayId = Play_FindFromId(_iPlayId)))
	{
		return td_failure;
	}
	pthread_mutex_lock(&gtPlayback[iPlayId].m_tLock);	
	//去初始化
	if(NULL != gtPlayback[iPlayId].m_pcBuf)
	{
		free(gtPlayback[iPlayId].m_pcBuf);
		gtPlayback[iPlayId].m_pcBuf = NULL;
	}
	gtPlayback[iPlayId].m_iSpeed = 1;
	gtPlayback[iPlayId].m_tStat = PLAYBACK_STAT_STOP;
	gtPlayback[iPlayId].m_iTempStat = PLAYBACK_STAT_STOP;
	gtPlayback[iPlayId].m_iHeigh = 0;
	gtPlayback[iPlayId].m_iWith = 0;
	gtPlayback[iPlayId].m_iFrameRate = 1;
	gtPlayback[iPlayId].m_tFileList = NULL;
	gtPlayback[iPlayId].m_iPlayId = -1;
	gtPlayback[iPlayId].m_iBufFull = 0;
	pthread_mutex_unlock(&gtPlayback[iPlayId].m_tLock);
	return td_success;
}

int Play_SetTime(int _iChn, int _iVoChn, int _iVoDevId, IN time_t _iBeginTm, IN time_t _iEndTm)
{
	Td_VO *ptVo = NULL;
	int iPlayId;
	PLAYFILE *ptPlayFile;
	Tplayback tPlayback = {0};
	S_FileHeader *ptFileHeader;
	TIpChannelMsg IpChnMsg;
	pthread_mutex_lock(&gPlayLock);
	preview_stoppage(_iVoDevId);
	if(NULL == (ptPlayFile = playfile_build_List(_iBeginTm, _iEndTm, _iChn)))
	{
		goto failure;
	}
	if(td_failure == Play_InitTask())
	{	
		playfile_unbuild_List(ptPlayFile);
		goto failure;
	}
	playfile_seek(ptPlayFile, -1, _iBeginTm, NULL, &ptFileHeader);
	tPlayback.m_iHeigh = ptFileHeader->Height;
	tPlayback.m_iWith = ptFileHeader->Width;
	tPlayback.m_iFrameRate = ptFileHeader->FrameRate;
	tPlayback.m_iMode = ptFileHeader->Mode; //编码模式：11（MP4)， 21(H264)
	tPlayback.m_iAudioSample = ptFileHeader->AudioSample;
	tPlayback.m_iVoDevId = _iVoDevId;
	#ifndef SELF_DEC
	int iVideoSize = Play_WithHeightToVideoSize(tPlayback.m_iWith, tPlayback.m_iHeigh);
	memset(&IpChnMsg, 0, sizeof(IpChnMsg));
	IpChnMsg.m_iChnNo = PLAYBACKCHNNO;//
	IpChnMsg.m_iServerType = 2;
	IpChnMsg.m_iEnable = 1;
	chnmnt_ChannelSetIpChannelMsg(PLAYBACKCHNNO, &IpChnMsg);
	chnmnt_ChannelSetVencType(PLAYBACKCHNNO, tPlayback.m_iMode);
	chnmnt_ChannelSetVideoSize(PLAYBACKCHNNO, iVideoSize);
	chnmnt_ChannelSetAudioSample(PLAYBACKCHNNO, tPlayback.m_iAudioSample);
	tPlayback.m_iHistoryChn = preview_ChgCurDspChn(_iVoDevId, _iVoChn, PLAYBACKCHNNO);
	td_s32 preview_fristVdec();
	preview_fristVdec();
	#else
	Td_VO *ptVo1;
	void *preview_getVoFromChn(int _iChn, int _iDev);
	ptVo = preview_getVoFromChn(_iVoChn, _iVoDevId);
	if(NULL == ptVo)
	{
		goto leave;
	}
	ptVo->VoChnCtrl(ptVo, VO_HIDE);
	if(NULL == (ptVo1 = CreateVO_Ex(_iVoDevId, &ptVo->m_VoRect, VO_VIDEOLEVEL)))
	{
		ptVo->VoChnCtrl(ptVo, VO_SHOW);
	}
	//chnmnt_ChannelUnBindVO(_iVoChn, ptVo);
	if(NULL == (tPlayback.m_ptVdec = playdec_CreateVdec(tPlayback.m_iHeigh, tPlayback.m_iWith, _iVoChn)))
	{
		//chnmnt_ChannelBindVO(_iVoChn, ptVo, &ptVo->m_VoRect);
		goto leave;
	}
	ptVo1->m_iBindChn = tPlayback.m_ptVdec->m_iVdecChan;
	ptVo1->VoChnCtrl(ptVo1,VO_CLEAR);	//此处必须有，否则会造成卡屏	
	ptVo1->Bind(ptVo1, VO_BIND_VDEC, ptVo1->m_iBindChn, NULL);
	ptVo1->VoChnCtrl(ptVo1,VO_SHOW);
	ptVo1->SetVoFrameRate(ptVo1,25);
	tPlayback.m_ptMyVo = ptVo1;
	#endif
	tPlayback.m_ptVo = ptVo;
	tPlayback.m_iChn = _iVoChn;
	if(td_failure == (iPlayId = Play_AddTask(ptPlayFile, &tPlayback)))
	{
		playfile_unbuild_List(ptPlayFile);
		gInitLogicPlay = 1;
		gInitTimePlay = 1;
		goto failure;
	}
//leave:
	gInitLogicPlay = 1;
	gInitTimePlay =1;
	pthread_mutex_unlock(&gPlayLock);
	play_waken(0);
	return td_success;
	
failure:
	preview_startpage(_iVoDevId);	
	pthread_mutex_unlock(&gPlayLock);
	return td_failure;
}

int Play_Exit(int _iPlayId)
{
	int iPlayId;
	TIpChannelMsg IpChnMsg;
	PLAYFILE *ptFileList = NULL;
	pthread_mutex_lock(&gPlayLock);
	//销毁单个
	if(td_failure == (iPlayId = Play_FindFromId(_iPlayId)))
	{
		pthread_mutex_unlock(&gPlayLock);		
		return td_failure;
	}
	
	ptFileList = gtPlayback[iPlayId].m_tFileList;
	Play_DelTask(_iPlayId);
	playfile_unbuild_List(ptFileList);
	#ifdef SELF_DEC
	if(NULL != gtPlayback[iPlayId].m_ptMyVo)
	{
		gtPlayback[iPlayId].m_ptMyVo->UnBind(gtPlayback[iPlayId].m_ptMyVo);
		DestroyVO(gtPlayback[iPlayId].m_ptMyVo);
	}

	if(NULL != gtPlayback[iPlayId].m_ptVo)
	{
		gtPlayback[iPlayId].m_ptVo->VoChnCtrl(gtPlayback[iPlayId].m_ptVo, VO_SHOW);
	}
	
	if(NULL != gtPlayback[iPlayId].m_ptVdec)
	{
		playdec_DestroyVdec(gtPlayback[iPlayId].m_ptVdec, gtPlayback[iPlayId].m_iChn);
	}
	#else
	memset(&IpChnMsg, 0, sizeof(IpChnMsg));
	IpChnMsg.m_iChnNo = PLAYBACKCHNNO;//
	IpChnMsg.m_iServerType = 2;
	IpChnMsg.m_iEnable = 0;
	chnmnt_ChannelSetIpChannelMsg(PLAYBACKCHNNO, &IpChnMsg);
	preview_ChgCurDspChn(gtPlayback[iPlayId].m_iVoDevId, gtPlayback[iPlayId].m_iChn, gtPlayback[iPlayId].m_iHistoryChn);
	preview_startpage(gtPlayback[iPlayId].m_iVoDevId);
	#endif
	pthread_mutex_unlock(&gPlayLock);
	play_waken(0);
	return td_success;
}

int Play_Pause(int _iPlayId)
{
	int iPlayId;
	if(td_failure == (iPlayId = Play_FindFromId(_iPlayId)))
	{
		return td_failure;
	}
	pthread_mutex_lock(&gtPlayback[iPlayId].m_tLock);
	gtPlayback[iPlayId].m_tStat = PLAYBACK_STAT_PAUSE;
	pthread_mutex_unlock(&gtPlayback[iPlayId].m_tLock);
	return td_success;
}

int Play_Start(int _iPlayId)
{
	int iPlayId;
	if(td_failure == (iPlayId = Play_FindFromId(_iPlayId)))
	{
		return td_failure;
	}
	pthread_mutex_lock(&gtPlayback[iPlayId].m_tLock);
	gtPlayback[iPlayId].m_tStat = PLAYBACK_STAT_PLAY;
	play_setStd(gtPlayback + iPlayId);
	pthread_mutex_unlock(&gtPlayback[iPlayId].m_tLock);
	return td_success;
}

int Play_Seek(int _iPlayId, int _iPos, int _iTm)
{
	int iPlayId;
	S_FileHeader *ptFileHeader;

	if(td_failure == (iPlayId = Play_FindFromId(_iPlayId)))
	{
		return td_failure;
	}
	
	pthread_mutex_lock(&gtPlayback[iPlayId].m_tLock);
	td_printf(0, "%s, %d, _iPos = %d, _iTm = %d\n", __func__, __LINE__, _iPos, _iTm);
	if(td_success == playfile_seek(gtPlayback[iPlayId].m_tFileList, _iPos, _iTm, NULL, &ptFileHeader))
	{
		if(PLAYBACK_STAT_STOP == gtPlayback[iPlayId].m_tStat)
		{
			gtPlayback[iPlayId].m_tStat = PLAYBACK_STAT_PLAY;
		}
		play_getData(gtPlayback + iPlayId, 1);
		play_setStd(gtPlayback + iPlayId);
		play_SendToDec(gtPlayback + iPlayId, play_GetSysTime());
	}

	pthread_mutex_unlock(&gtPlayback[iPlayId].m_tLock);
	play_waken(0);
	td_printf(0,"%s, %d\n", __func__, __LINE__);
	return td_success;
}

int Play_GetStat(int _iPlayId, TplyStat *_ptStat)
{
//	S_FileHeader *ptFileHeader;
	int iPlayId;

	if(NULL == _ptStat)
	{
		return td_failure;
	}
	if(td_failure == (iPlayId = Play_FindFromId(_iPlayId)))
	{
		return td_failure;
	}
	pthread_mutex_lock(&gtPlayback[iPlayId].m_tLock);
	playfile_GetCmd(gtPlayback[iPlayId].m_tFileList, 0, &_ptStat->m_iSchedule);
	playfile_GetCmd(gtPlayback[iPlayId].m_tFileList, 1, &_ptStat->m_iCurTime);
	playfile_GetCmd(gtPlayback[iPlayId].m_tFileList, 2, &_ptStat->m_iPassword);
	_ptStat->m_iStat = gtPlayback[iPlayId].m_tStat;
	_ptStat->m_iSpeed = gtPlayback[iPlayId].m_iSpeed;
	td_printf(0,"_ptStat->m_iSchedule = %d, gtPlayback[iPlayId].m_tStat = %d, _ptStat->m_iCurTime = %d\n", _ptStat->m_iSchedule, gtPlayback[iPlayId].m_tStat, _ptStat->m_iCurTime);
	pthread_mutex_unlock(&gtPlayback[iPlayId].m_tLock);
	return td_success;
}

int Play_SetEncrypt(int _iPlayId, char *_cPassWord)
{
	int iPlayId;
	if(NULL == _cPassWord)
	{
		return td_failure;
	}
	if(td_failure == (iPlayId = Play_FindFromId(_iPlayId)))
	{
		return td_failure;
	}

	pthread_mutex_lock(&gtPlayback[iPlayId].m_tLock);
	if(td_failure == playfile_SetCmd(gtPlayback[iPlayId].m_tFileList, 0, _cPassWord))
	{
		pthread_mutex_unlock(&gtPlayback[iPlayId].m_tLock);
		return td_failure;
	}
	gtPlayback[iPlayId].m_iTempStat = PLAYBACK_STAT_PLAY;
	pthread_mutex_unlock(&gtPlayback[iPlayId].m_tLock);
	
	play_waken(0);
	return td_success;
}
