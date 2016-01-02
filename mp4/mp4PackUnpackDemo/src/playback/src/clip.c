#define PLAYBACK1_H

#include <unistd.h>
#include <sys/stat.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <guiparameter.h>

#ifdef MEMWATCH
#include "memwatch.h"
#endif
#include "guiparameter.h"
#include "playback.h"
#include "decode.h"
#include "libstorage.h"
#include "libdiskmnt.h"
#include "td_system.h"
#include "config.h"

/////////////////////////////////////////////////////////////////////
/////////////////////////剪切功能////////////////////////////////
#define MAX_CLIP_PROS	32
#define CLIP_MAX_FILE_NUM	1024
#define GUI_VIDEO_CLIP_SAVE_PATH_LEN   127
#define CUT_FILE_NAME_LEN ( 2  + 2  + 1  + 4      + 2   +2    + 2   +2  +2  +1 +2   +4   +1)
#define NOFILEERR	(-100)
//						    CH    xx    T   xxxx     xx    xx    xx    xx   xx   N  xx   .sdv

#define PLAYER "MiniPlayer.exe"
#define TEMP_PLAYER_PATH "/mnt/MiniPlayer/"//wangsong, 20121224, 解压tar包至此
static int g_iTarFlag = 0;
static int g_ithreadFlag = 0;
static int g_ClipFileNum = 0;
static TFileInfo g_pstFileListInfo[CLIP_MAX_FILE_NUM];
static pthread_mutex_t g_clipLock = PTHREAD_MUTEX_INITIALIZER;

typedef struct 
{
	int m_iUseFlag;//0:代表未使用，1:代表使用
	int m_iChn;//剪切的通道号
	int m_iStartTime;
	int m_iEndTime;
	int m_iClipID;//剪切的ID号//从1-1024循环

	int m_ierr;//在存储过程中产生错误
	int m_iHasQuery;
	int m_iFileNum;//文件号
	int m_iFileSum;//总的文件数
	int m_iCutLen;//-1:剪辑整个文件，>0的数表示剪辑的文件的部分大小
	int m_iFileListHead;
	TFileInfo *m_stFileListInfo;
	char m_cPath[GUI_VIDEO_CLIP_SAVE_PATH_LEN + CUT_FILE_NAME_LEN];
	int m_iNameLen;
}TClip;

static TClip *g_pClip[8] = {NULL};
//搜索I帧，从传入时间的前一个I帧开始
//返回值:返回搜索到的位置
static int Playback_Clip_ISeek(int _iTime, char *_pcFileName)
{
	int iRet = 0;
	int iLocation = 0;
	FILE *fp;
	if(ERR_REC_SUCCESSFUL !=rec_OpenFile(&fp, (td_s8 *)_pcFileName))
	{
		return -1;
	}
	iRet = rec_SeekFile(fp, _iTime, NULL);
	if(iRet != ERR_REC_SUCCESSFUL)
	{
		td_printf(0,"rec_SeekFile err return is %d",iRet);
		rec_CloseFile(&fp);
		return -1;
	}
	iLocation = ftell(fp);
	rec_CloseFile(&fp);
	return iLocation;
}

//搜索P帧，计算出入时间最近的前一个P帧
//返回值:返回搜索到的位置
static int Playback_Clip_PSeek(int _iTime, char *_pcFileName)
{
	int iRet = 0;
	int iRemainTms = 0;
	int iLoction = 0;
	int iFrist = 0;
	unsigned int iIFrameTm = 0;
	FILE *fp;
	S_FrameHeader FrameHead;
	int iFrameLen = sizeof(S_FrameHeader);
	rec_OpenFile(&fp, (td_s8 *)_pcFileName);
	iRet = rec_SeekFile(fp, _iTime, &iRemainTms);
	if(iRet != ERR_REC_SUCCESSFUL)
	{
		td_printf(0,"rec_SeekFile err return is %d",iRet);
		rec_CloseFile(&fp);
		return -1;
	}

	iLoction = ftell(fp);
	iRemainTms *= 1000; 
	while(iRemainTms > 0)
	{
		fseek(fp, iLoction, SEEK_SET);
		if(fread(&FrameHead,1,iFrameLen,fp) < iFrameLen)
		{
			td_printf(0,"read m_FrameHeader err");
			break;
		}
		if(1 != IsValidFrame(&FrameHead))//读到最后，或者是产生了错误
		{
			break;
		}
		if(iFrist == 0)
		{
			iFrist++;
			iIFrameTm = FrameHead.u32TimeStamp;
		}
		if(FrameHead.u32TimeStamp > iIFrameTm + 1000 || FrameHead.u32TimeStamp < iIFrameTm)
		{
			iIFrameTm = FrameHead.u32TimeStamp;
		}
		iRemainTms -= (FrameHead.u32TimeStamp - iIFrameTm);
		iIFrameTm = FrameHead.u32TimeStamp;
		iLoction += FrameHead.u32FrameSize;
	}
	rec_CloseFile(&fp);
	return iLoction;	
}

//外部调用函数
static  void *ClipThread(void *t);//剪辑线程
int Playback_Video_Clip(int _iChn, int _iStartTime, int _iEndTime, char *_pcPath)
{
	static char strPath[GUI_VIDEO_CLIP_SAVE_PATH_LEN + 1];
	pthread_t threadId;
	int i = 0;
	int m = 0;
    char cSysCmd[128] = {0};
    //char cTempPath[128] = {0};
	if(_iChn < 0 || _iStartTime > _iEndTime  || _pcPath == NULL || _iEndTime - _iStartTime > 24*60*60)
	{
		return -1;//参数传递有误
	}
	rec_ClearStopCopyFlag();
	td_printf(0,"_iChn = %d",_iChn);
	td_printf(0,"_iStartTime = %d",_iStartTime);
	td_printf(0,"_iEndTime = %d",_iEndTime);
	td_printf(0,"_pcPath = %s",_pcPath);
	
	//传递参数
	pthread_mutex_lock(&g_clipLock);
	if('\0' == *_pcPath)
	{
		disk_UmonutMountUsbPath(strPath);
		pthread_mutex_unlock(&g_clipLock);
		return 0;
	}
	strncpy(strPath, _pcPath, GUI_VIDEO_CLIP_SAVE_PATH_LEN);
	if(g_ithreadFlag == 0)//创建线程
	{
		int iRet = pthread_create(&threadId,NULL,(void *)ClipThread,(void *)0);
		if(iRet != 0)//穿件线程未成功
		{
			pthread_mutex_unlock(&g_clipLock);
			return -2;
		}
		g_ithreadFlag = 1;
		pthread_detach(threadId);
	}

    if (g_iTarFlag == 0)
    {
        sprintf(cSysCmd, "mkdir %s", TEMP_PLAYER_PATH);
        TdSystem(cSysCmd);
        sprintf(cSysCmd, "mount -t tmpfs size=2M %s", TEMP_PLAYER_PATH);
        TdSystem(cSysCmd);
        sprintf(cSysCmd, "tar -zxf %s -C %s", PLAYPATH, TEMP_PLAYER_PATH);
		TdSystem(cSysCmd);
        g_iTarFlag = 1;
    }
	int iMin = 8;
	for(m = 0; m < 8; m++)
	{
		if(g_pClip[m] != NULL)
		{
			for(i = 0; i < MAX_CLIP_PROS; i++)
			{
				if(g_pClip[m][i].m_iUseFlag == 0)
				{
					iMin = MAX_CLIP_PROS;
					break;
				}
			}
			if(iMin == MAX_CLIP_PROS)
			{
				break;
			}
		}
		else
		{
			if(iMin > m)
			{
				iMin = m;
			}
		}
	}
	if(iMin == 8)
	{
		pthread_mutex_unlock(&g_clipLock);
		return -2;		
	}
	int itemp1 = 0,itemp2 = 0;
	if(iMin == MAX_CLIP_PROS)
	{
		itemp1 = m;
		itemp2 = i;
	}
	else
	{
		g_pClip[iMin] = (TClip *)malloc(sizeof(TClip) * MAX_CLIP_PROS);
		if(g_pClip[iMin] == NULL)
		{
			pthread_mutex_unlock(&g_clipLock);
			return -2;
		}
		int ii = 0;
		for(ii = 0; ii < MAX_CLIP_PROS; ii++)
		{
			g_pClip[iMin][ii].m_stFileListInfo = NULL;
		}
		memset(g_pClip[iMin], 0, sizeof(TClip) * MAX_CLIP_PROS);
		itemp1 = iMin;
		itemp2 = 0;
	}
	//算出存储文件的全名
	int iLen = strlen(_pcPath);
	char cPath[GUI_VIDEO_CLIP_SAVE_PATH_LEN + CUT_FILE_NAME_LEN + 32];
	
	memset(cPath, 0, GUI_VIDEO_CLIP_SAVE_PATH_LEN);
	memset(g_pClip[itemp1][itemp2].m_cPath, 0, GUI_VIDEO_CLIP_SAVE_PATH_LEN + CUT_FILE_NAME_LEN);
	if(_pcPath[iLen - 1] == '/')
	{
		g_pClip[itemp1][itemp2].m_iNameLen = iLen + 4;

		//sprintf(cPath, "%s%s%s","cp -f ", PLAYPATH, _pcPath);
		sprintf(cPath, "%s%s", _pcPath, PLAYER);
		sprintf(g_pClip[itemp1][itemp2].m_cPath, "%s%s%02d",_pcPath, "CH", _iChn);
	}
	else
	{
		g_pClip[itemp1][itemp2].m_iNameLen = iLen + 5;
		//sprintf(cPath, "%s%s%s%s","cp -f ", PLAYPATH, _pcPath, "/");
		sprintf(cPath, "%s%s%s", _pcPath, "/", PLAYER);
		sprintf(g_pClip[itemp1][itemp2].m_cPath, "%s%s%02d",_pcPath, "/CH", _iChn);
	}
	#ifndef HI3520D_128M	
	struct stat buf;
	if(stat(cPath, &buf) < 0)
	{
	  char cTempPath[128] = {0};
	    sprintf(cTempPath, "%s%s", TEMP_PLAYER_PATH, PLAYER);
		copy_file(cTempPath, cPath, -1);	
	}
	#endif
	td_printf(0,"cPath = %s",cPath);
	//system(cPath);
	g_ClipFileNum++;
	g_ClipFileNum %= 10000;
	if(g_ClipFileNum == 0)
	{
		g_ClipFileNum++;
	}
	
	g_pClip[itemp1][itemp2].m_iChn = _iChn;
	g_pClip[itemp1][itemp2].m_iStartTime = _iStartTime;
	g_pClip[itemp1][itemp2].m_iEndTime = _iEndTime;
	g_pClip[itemp1][itemp2].m_iUseFlag = 1;
	g_pClip[itemp1][itemp2].m_iHasQuery = 0;
	g_pClip[itemp1][itemp2].m_iFileNum = -1;
	g_pClip[itemp1][itemp2].m_iFileSum = 0;
	g_pClip[itemp1][itemp2].m_ierr = 0;
	g_pClip[itemp1][itemp2].m_iFileListHead = 0;
	g_pClip[itemp1][itemp2].m_iClipID = g_ClipFileNum;
	
	pthread_mutex_unlock(&g_clipLock);
	return g_ClipFileNum;
}
static int Playback_Get_FileLen(char *_FileName)
{
	int iRet = 0;
	struct stat buf;
	td_s32 iChnNum = 0;
	td_u8 cFullPath[FULL_PATH_LEN] = {0};
	TFileInfo	g_stCurFileInfo;				//当前正在录像的文件信息
	iRet = rec_CompleteFileName((td_s8 *)_FileName, (td_s8 *)cFullPath, NULL, NULL);
	if (ERR_REC_SUCCESSFUL != iRet)
	{
		td_printf(0, "complete filename err %d\n", iRet);
		return -1;
	}
	
	//20121112 王磊add 防止备份正在录像的文件时出现预分配大小的文件 
	iChnNum = rec_Atoi(_FileName[4]);
	g_stCurFileInfo = rec_GetCurFileInfo(iChnNum);
	if(0 == strcmp((const char *)g_stCurFileInfo.m_ucFileName, (const char *)_FileName))
	{
		buf.st_size = g_stCurFileInfo.m_uiFileSize;
	}
	else if(rec_Stat((char *)cFullPath,&buf) != 0)
	{
		return -1;
	}

	
	return buf.st_size;
}

void PlaybackVideoBackup(void)
{
	rec_StopCopy();
}

//申请的ID片段号的进度
//返回-100，在保存中产生了错误
int Playback_Video_Clip_Query_Pro(int _iClipID)
{
	int i = MAX_CLIP_PROS;
	int m = 0;
	int iPro = 100;
	struct stat buf;
	double dPos = 0;
	int iProRet = 0;
	int iLen = 0;
	int iFileSum = 0;
	pthread_mutex_lock(&g_clipLock);
	
	for(m = 0; m < 8; m++)
	{
		if(g_pClip[m] != NULL)
		{
			for(i = 0; i < MAX_CLIP_PROS; i++)
			{
				if(g_pClip[m][i].m_iClipID == _iClipID)
				{
					break;
				}
			}
			if(i != MAX_CLIP_PROS)
			{
				break;
			}
		}
	}
	if( (i != MAX_CLIP_PROS) && (g_pClip[m][i].m_ierr < 0) )//如果在进程保存中产生了错误
	{
		iProRet = g_pClip[m][i].m_ierr;
		goto Fail;
	}

	//计算进度
	if(i < MAX_CLIP_PROS && g_pClip[m][i].m_iUseFlag == 1)
	{
		if(g_pClip[m][i].m_iFileNum <= g_pClip[m][i].m_iFileListHead)
		{
			iPro = 0;
		}
		else
		{
			iFileSum = ((g_pClip[m][i].m_iFileSum -g_pClip[m][i].m_iFileListHead) ? 
						(g_pClip[m][i].m_iFileSum -g_pClip[m][i].m_iFileListHead) : 1);
			if(0 == g_pClip[m][i].m_iCutLen)
			{
				
				dPos = (g_pClip[m][i].m_iFileNum -g_pClip[m][i].m_iFileListHead) * 1.0;
				iPro = dPos * 100 / iFileSum;
			}
			else if(rec_Stat(g_pClip[m][i].m_cPath, &buf) != 0)
			{
				if(errno == ENOENT)
				{
					iPro = (g_pClip[m][i].m_iFileNum -g_pClip[m][i].m_iFileListHead -1) * 100 / iFileSum;
				}
				else
				{
					goto Fail;
				}
			}
			else
			{
				
				iLen = (0 == g_pClip[m][i].m_iCutLen) ? 1 : g_pClip[m][i].m_iCutLen; 
				dPos = (g_pClip[m][i].m_iFileNum -g_pClip[m][i].m_iFileListHead -1 + buf.st_size * 1.0 / iLen) * 1.0;
				iPro = dPos * 100 / iFileSum;
				if(buf.st_size < g_pClip[m][i].m_iCutLen && iPro == 100)
				{
					iPro = 99;
				}
			}
		}
	}	
	pthread_mutex_unlock(&g_clipLock); 
	return iPro;
	
Fail:
	g_pClip[m][i].m_iUseFlag = 0;
	pthread_mutex_unlock(&g_clipLock);
	
	iPro = (iProRet == NOFILEERR) ? 100 : iProRet;
	if(iPro < 0 && iPro != GUI_VIDEO_CLIP_DISK_NOSAPCE)
	{
		iPro = GUI_VIDEO_CLIP_GENERIC_ERR;
	}
	td_printf(0,"%s(%d) pro = %d",__func__,__LINE__,iPro);
	return iPro;	
}

static int QueryCondion(TClip *_ptClip)
{
	int iRet = 0;
	TQuery Query;

	//查询条件
	Query.m_uiPageSize = PAGE_SHOW_NUM;
	Query.m_iStartTime = (_ptClip->m_iStartTime - 7200 > 0) ? _ptClip->m_iStartTime - 7200 : 0;//提前2小时查询，判断边界
	Query.m_iStopTime = _ptClip->m_iEndTime;
	Query.m_ucFileType = FILE_REC;
	Query.m_ucRecType = 0xff;			//查询录像所有类型
	Query.m_ucDevType =0xff;
	Query.m_ucChannelNo = UkSubToData(_ptClip->m_iChn) - 1;
	Query.m_uiPageNo = 0;
	Query.m_uiTotalNum = CLIP_MAX_FILE_NUM - 1;
	Query.m_cReserved[0] = '\0';	

	//查询文件列表
	iRet = rec_QueryAllFiles(&Query, _ptClip->m_stFileListInfo);
	if(ERR_REC_SUCCESSFUL == iRet)
	{
		if(0 == Query.m_uiTotalNum)
		{
			return -1;
		}
		Query.m_uiTotalNum = (Query.m_uiTotalNum> MAXFILELISTLEN) ? MAXFILELISTLEN : Query.m_uiTotalNum;
	}
	else
	{
		return -1;
	}
	_ptClip->m_iFileSum = Query.m_uiTotalNum;
	_ptClip->m_iFileNum = 0;
	int i = 0;
	for (i = 0; i < Query.m_uiTotalNum; i++)
	{
		//重新获得文件的起始时间与结束时间
		if(_ptClip->m_stFileListInfo[i].m_iStopTime <= _ptClip->m_iStartTime 
			&& _ptClip->m_stFileListInfo[i].m_iStartTime != 0
			&& _ptClip->m_stFileListInfo[i].m_iStopTime != 0)
		{			
			_ptClip->m_iFileListHead++;
		}
	}
	_ptClip->m_iFileNum = _ptClip->m_iFileListHead;
	if((_ptClip->m_iFileListHead == _ptClip->m_iFileSum) || (0 == _ptClip->m_iFileSum))
	{
		return -1;
	}
	return 0;
}

static int AdjustFile(TClip *_ptClip, int *_piStart, int *_piEnd)
{
	
	int i = 0;
	int flag = 0;
	int FileNum = _ptClip->m_iFileNum;
	int iRet = -2;
	struct tm Time;
	if(FileNum < 0)
	{
		td_printf(0,"%s(%d)",__func__,__LINE__);
		_ptClip->m_ierr = -1;
		return -1;
	}
	for(i = FileNum; i < _ptClip->m_iFileSum; i++)
	{
		if(_ptClip->m_stFileListInfo[i].m_iStartTime >= _ptClip->m_iStartTime)
		{
			flag = 1;//完全包含
			if(_ptClip->m_stFileListInfo[i].m_iStopTime > _ptClip->m_iEndTime)
			{
				flag = 2;//包含文件的前半部分
			}
			break;
		}
		if(_ptClip->m_stFileListInfo[i].m_iStartTime <= _ptClip->m_iStartTime 
			&& _ptClip->m_stFileListInfo[i].m_iStopTime > _ptClip->m_iEndTime)
		{
			flag = 4;
			break;
		}
		if(_ptClip->m_stFileListInfo[i].m_iStopTime > _ptClip->m_iStartTime)
		{
			flag = 3;//包含文件后半部分
			break;
		}
	}
	if(i != _ptClip->m_iFileSum)
	{
		iRet = i;
	}
	if(iRet < 0)
		return NOFILEERR;
	char *pName = (char *)_ptClip->m_stFileListInfo[i].m_ucFileName;
	
	switch(flag)
	{
		case 1://完全包含
			*_piStart = -1;
			*_piEnd = -1;
			td_printf(0,"_ptClip->m_stFileListInfo->m_ucFileName = %s",_ptClip->m_stFileListInfo[iRet].m_ucFileName);
			localtime_r( (time_t *)(&(_ptClip->m_stFileListInfo[iRet].m_iStartTime)) , &Time );
			_ptClip->m_iCutLen = Playback_Get_FileLen((char *)_ptClip->m_stFileListInfo[iRet].m_ucFileName);
			td_printf(0,"case 1 = %d, Time min = %d,_ptClip->m_stFileListInfo[iRet].m_iStartTime = %d,_ptClip->m_iCutLen =%d",iRet,Time.tm_min,_ptClip->m_stFileListInfo[iRet].m_iStartTime,_ptClip->m_iCutLen);
			break;
			
		case 2://包含文件的前半部分
			*_piStart = -1;
			*_piEnd = Playback_Clip_PSeek(_ptClip->m_iEndTime, pName);
			if(*_piEnd < 0)
			{
				*_piEnd = -1;
				_ptClip->m_iCutLen = Playback_Get_FileLen((char *)_ptClip->m_stFileListInfo[iRet].m_ucFileName);
			}
			else
			{
				_ptClip->m_iCutLen = *_piEnd;
			}

			localtime_r( (time_t *)(&(_ptClip->m_stFileListInfo[iRet].m_iStartTime)), &Time );
			td_printf(0,"case 2 = %d, Time min = %d,_ptClip->m_stFileListInfo[iRet].m_iStartTime = %d",iRet,Time.tm_min,_ptClip->m_stFileListInfo[iRet].m_iStartTime);
			break;
			
		case 3://包含文件后半部分
			
			*_piEnd = -1;
			*_piStart = Playback_Clip_ISeek(_ptClip->m_iStartTime, pName);
			if(*_piStart < 0)
			{
				*_piStart = -1;
				_ptClip->m_iCutLen = Playback_Get_FileLen((char *)_ptClip->m_stFileListInfo[iRet].m_ucFileName);
			}
			else
			{
				_ptClip->m_iCutLen = Playback_Get_FileLen((char *)_ptClip->m_stFileListInfo[iRet].m_ucFileName) - *_piStart;
			}
			localtime_r( (time_t *)(&(_ptClip->m_iStartTime)), &Time );
			td_printf(0,"case 3 = %d, Time min = %d,_ptClip->m_iStartTime = %d",iRet,Time.tm_min,_ptClip->m_iStartTime);
			break;
		case 4://包含文件的中间部分
			*_piStart = Playback_Clip_ISeek(_ptClip->m_iStartTime, pName);
			*_piEnd = Playback_Clip_PSeek(_ptClip->m_iEndTime, pName);
			if(*_piStart < 0 ||*_piEnd < 0 )
			{
				*_piStart = -1;
				*_piEnd = -1;
				_ptClip->m_iCutLen = Playback_Get_FileLen((char *)_ptClip->m_stFileListInfo[iRet].m_ucFileName);
			}
			else
			{
				_ptClip->m_iCutLen = *_piEnd - *_piStart;
			}
			
			localtime_r( (time_t *)(&(_ptClip->m_iStartTime)) ,&Time);
			break;
		default :;
	}
	
	_ptClip->m_cPath[_ptClip->m_iNameLen] = '\0';
	td_printf(0,"_ptClip->m_cPath = %s",_ptClip->m_cPath);	

	#if 0
	sprintf(_ptClip->m_cPath, "%s%s%04d%02d%02d%02d%02d%02d%s%02d%s",_ptClip->m_cPath,
							"T", 
							Time.tm_year + 1900, 
							Time.tm_mon + 1,
							Time.tm_mday,
							Time.tm_hour,
							Time.tm_min,
							Time.tm_sec,
							"N", g_ClipFileNum%100, ".sdv");
	#else
	
	char cRecExtName[16] = {'.',0};
	strncpy(cRecExtName + 1, Cfg_gRecExtName(), MAX_EXTNAME_LEN - 1);
	td_printf(0x00ff0000, "=== =H8===RecExtName:%s\n", cRecExtName);
	sprintf(_ptClip->m_cPath, "%s%s%04d%02d%02d%02d%02d%02d%s%02d%s",_ptClip->m_cPath,
							"T", 
							Time.tm_year + 1900, 
							Time.tm_mon + 1,
							Time.tm_mday,
							Time.tm_hour,
							Time.tm_min,
							Time.tm_sec,
							"N", g_ClipFileNum%100, cRecExtName);
	#endif
	return iRet;
}

int ClipGetSegmentLen(int _iChn, int _iStartTime, int _iEndTime)
{	
	int iRet = 0;
	int i = 0;
	TClip Clip;
	int iStart = 0;
	int iEnd = 0;
	unsigned int iLen = 0;
	
	Clip.m_iChn = _iChn;
	Clip.m_iStartTime = _iStartTime;
	Clip.m_iEndTime = _iEndTime;
	Clip.m_iUseFlag = 1;
	Clip.m_iHasQuery = 0;
	Clip.m_iFileNum = -1;
	Clip.m_iFileSum = 0;
	Clip.m_ierr = 0;
	Clip.m_iFileListHead = 0;
	Clip.m_iClipID = 0;
	Clip.m_iCutLen = 0;
	Clip.m_stFileListInfo = g_pstFileListInfo;
	Clip.m_iNameLen = 5;
	
	if(QueryCondion(&Clip) < 0)
	{
		iLen = 0;
		goto leave;
	}
	for(i = 0; i < Clip.m_iFileSum; )
	{
		if((iRet = AdjustFile(&Clip, &iStart, &iEnd)) < 0)
		{
			break;
		}
		Clip.m_iFileNum = iRet + 1;
		i = iRet + 1;
		iLen += Clip.m_iCutLen;
	}
leave :
	 return iLen;
}

static int ThreadRunErr(int _iNum, int _iClipNum, int errValue)
{
	pthread_mutex_lock(&g_clipLock);
	if(g_pClip[_iNum] != NULL)
	{
		g_pClip[_iNum][_iClipNum].m_ierr = errValue;
		g_pClip[_iNum][_iClipNum].m_iUseFlag = 0;
	}
	pthread_mutex_unlock(&g_clipLock);
	return 0;
}
static void *ClipThread(void *t)//剪辑线程
{
	int i = 0;
	int m = 0;
	int iRet = -1;
	int iClipNum = 0;
	int FreeTime = 0;
	int iStart = 0;
	int iEnd = 0;
	TClip tClip;

	func_info();
	
	while(1)
	{
		//首先判断有没有要处理的剪切信息
		i = MAX_CLIP_PROS;
		pthread_mutex_lock(&g_clipLock);
		for(m = 0; m < 8; m++)
		{
			
			if(g_pClip[m] == NULL)
			{
				continue;
			}
			for(i = 0; i < MAX_CLIP_PROS; i++)
			{
				if(g_pClip[m][i].m_iFileNum >= g_pClip[m][i].m_iFileSum)
				{
					g_pClip[m][i].m_iUseFlag = 0;
				}
				
				if(1 == g_pClip[m][i].m_iUseFlag)
				{		
					break;
				}
			}
			if(i != MAX_CLIP_PROS)
			{
				break;
			}
		}
		//获得结构体信息
		if(i != MAX_CLIP_PROS)
		{
			memcpy(&tClip, &g_pClip[m][i], sizeof(TClip));
		}
		

		if(MAX_CLIP_PROS == i)//没有要剪切的数据
		{
			int j = 0;
			FreeTime++;
			if(FreeTime > 60)//如果大于一定的时间，将部分堆栈释放，
			{
				for(j = 0; j < 8; j++)
				{
					if(g_pClip[j] == NULL)
					{
						continue;
					}
					for(i = 0; i < MAX_CLIP_PROS; i++)
					{
						if(g_pClip[j][i].m_stFileListInfo != NULL)
						{
							free(g_pClip[j][i].m_stFileListInfo);
							g_pClip[j][i].m_stFileListInfo = NULL;
						}
					}
					free(g_pClip[j]);
					g_pClip[j] = NULL;
				}
			}
			pthread_mutex_unlock(&g_clipLock);
			sleep(1);
			continue;
		}
		pthread_mutex_unlock(&g_clipLock);

		FreeTime = 0;
		iClipNum = i;
		if(tClip.m_stFileListInfo == NULL)
		{
			tClip.m_stFileListInfo = (TFileInfo *)malloc(sizeof(TFileInfo) * CLIP_MAX_FILE_NUM);
			if(tClip.m_stFileListInfo == NULL)
			{
				//通知上层，没有分配到足够的空间
				ThreadRunErr(m, iClipNum, -1);
				continue;
			}
		}

		//查询文件
		if(tClip.m_iHasQuery == 0)
		{
			if(QueryCondion(&tClip) < 0)
			{
				td_printf(0,"QueryCondion err ");
				ThreadRunErr(m, iClipNum, 100);
				continue;
			}
			td_printf(0,"_ptClip->m_iFileNum = %d, _ptClip->m_iFileSum = %d",tClip.m_iFileNum,tClip.m_iFileSum);
			tClip.m_iHasQuery = 1;
		}

		//判断文件性质
		int iFn;
		iFn = AdjustFile(&tClip, &iStart, &iEnd);
		if(iFn >= 0)
		{
			tClip.m_iFileNum = iFn;
		}
		else
		{
			td_printf(0,"%s(%d)",__func__,__LINE__);
			ThreadRunErr(m, iClipNum, iFn);
			continue;
		}
		//改变结构体信息
		int temp1 = 0;
		int temp2 = 0;
		char *pName = (char *)tClip.m_stFileListInfo[tClip.m_iFileNum].m_ucFileName;
		static int FileNum = 0;
		FileNum++;
		temp1 = FileNum / 100;
		temp2 = FileNum % 100;
		tClip.m_iFileNum++;
		pthread_mutex_lock(&g_clipLock);
		memcpy(&g_pClip[m][iClipNum], &tClip, sizeof(TClip));
		//修改文件名
		pthread_mutex_unlock(&g_clipLock);
		//保存片段
		#if 0
		td_printf(0,"pName %s",pName);
		td_printf(0,"g_tClip[iClipNum].m_cPath %s",g_pClip[m][iClipNum].m_cPath);
		td_printf(0,"iStart %d",iStart);
		td_printf(0,"iEnd %d",iEnd);
		#endif
		if(!tClip.m_iCutLen)
		{
			continue;
		}
		iRet = rec_GetPartFile((td_u8 *)pName, (td_u8 *)g_pClip[m][iClipNum].m_cPath, iStart, iEnd);
		if(iRet != ERR_REC_SUCCESSFUL && iRet != 0)
		{
			td_printf(0,"rec_GetPartFile iRet = %d",iRet);
			if(iRet == -5)
			{
				ThreadRunErr(m, iClipNum, -5);
			}
			else if(iRet == ERR_REC_OPEN_FILE_FAILED)
			{
				//ThreadRunErr(m, iClipNum, -2);
			}
			else if(iRet == ERR_REC_DISK_FULL)
			{
				ThreadRunErr(m, iClipNum, -3);
			}
			else 
			{
				ThreadRunErr(m, iClipNum, -1);
			}			
		}
	}
	return 0;
}


