#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>

#ifdef MEMWATCH
#include "memwatch.h"
#endif

#include "libtdao.h"
#include "decode.h"
#include "libstorage.h"
#include "td_media.h"
#include "config.h"
#include "td_mp4.h"

#define PLAY_CAP_BUF	(256*1024)
#define MAXBUF	(1024 * 1064)
#define MAXVSTREAM (MAXBUF - 64 * 1024)

#define MAX_BACK_NUM	4
static TStream *gStream[MAX_BACK_NUM] = {NULL};
static int gBack = 0;
static pthread_mutex_t gBackMutexLock = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t gBackLinkLock = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t gBackCondEmptyLock = PTHREAD_COND_INITIALIZER;
static volatile void *gCaping = NULL;
static void *gCapBuf = NULL;
#define DECODE_STOP(flag)	(PLAYBACK_STAT_STOP == (flag) || PLAYBACK_STAT_INIT == (flag))
typedef struct 
{
	S_FrameHeader m_FrameHead;
	int m_IndexNum;//索引帧大小
	int m_Index;//I帧数目
	int m_IndexLen;//I帧信息大小
}TIndex;

static int g_iOpenBuf = 0;
static long long lHistoryTime = 0;
static int g_defaultAo = 0;
#define PLAYBACK_CAPPIC_TIME_INTERVAL		(2 * 1000)	// 2 S
static int decode_GetFileAttribute(TStream *_This,void * _Value, int _iFlag);
static int decode_GetFileHeader(TStream *_This, S_FileHeader * _pFileHeader);
static int decode_ReadMem(TStream * _This);
static int decode_SendMemVStream(TStream * _This);
static td_s32 decode_fromBackToPlay(TStream * _This, int _iCommonly);
static int decoder_GetCapPicTimeInterval(void);
static int decoder_SetCapPicTime(void);
#define decode_TestBufFlag()		g_iOpenBuf
#define DECODE_BUFSIZE	(1024 * 128)

int decode_Get_MAXBUFEX(void)
{
	return MAXBUF;
}
int decode_EnableBuf(void)
{
	g_iOpenBuf = 1;
	return 0;
}
int decode_DisableBuf(void)
{
	g_iOpenBuf = 0;
	return 0;
}

int decode_getLobby(int _iHeight, int _iWith)
{
	if((1280 == _iHeight) && (720 == _iWith))
	{
		return 1;
	}
	if((1920 == _iHeight) && (1080 == _iWith))
	{
		return 1;
	}
	return 0;
}

int  IsValidFrame(S_FrameHeader* frHeader)
{
	if(!frHeader)
	{		
		td_printf(0, "%s 11 \n", __FUNCTION__ );
		return -1;
	}   	
	if(frHeader->u32FrameID != 0x02000000)
	{
		td_printf(0, "%s 14 \n", __FUNCTION__ );
		return -1;
	}
	if(frHeader->u32FrameSize>MAXVSTREAM)
	{
		td_printf(0, "%s 12 \n", __FUNCTION__ );
    	return -1;
	}
	if(frHeader->u32VStreamLen>MAXVSTREAM)
	{
		td_printf(0, "%s 13 \n", __FUNCTION__ );
    	return -1;
	}
	if(frHeader->u32FrameSize!=sizeof(S_FrameHeader)+frHeader->u32VStreamLen+frHeader->u16AFrames*frHeader->u16AFrameSize)
	{
		td_printf(0, "%s  frHeader->u32FrameNO:%d \n", __FUNCTION__ ,frHeader->u32FrameNO);	
		td_printf(0, "%s  frHeader->u32FrameSize:%d  frHeader->u32VStreamLen:%d \n", __FUNCTION__ ,frHeader->u32FrameSize,frHeader->u32VStreamLen);
		td_printf(0, "%s  frHeader->u16AFrames:%d  frHeader->u16AFrameSize:%d \n", __FUNCTION__ ,frHeader->u16AFrames,frHeader->u16AFrameSize);
		td_printf(0, "%s  frHeade:%d \n", __FUNCTION__ ,sizeof(S_FrameHeader));
		return -1;
	}
	return 1;
}
int IsValidFileHeader(S_FileHeader* _header)
{
	if(!_header)
	{
		return -1;
	}
	if(_header->Mode != 21)
	{
		return -1;
	}
	if((_header->FrameRate > MAX_FRAMERATE_NTSC))
	{
    	return -1;
	}
	if((_header->Width==720)&&(_header->Height==1280))
	{
		return 2;
	}
	if((_header->Width==1080)&&(_header->Height==1920))
	{
		return 2;		
	}
	if(_header->Width == 352)
	{   //Half-Cif, Cif
    	if( (_header->Height == 144) || (_header->Height == 288) ||
        	(_header->Height == 120) || (_header->Height == 240) )
        	return 1;
	}
	else if((_header->Width == 720) || (_header->Width == 704))
	{   //Half-D1, D1, 4Cif, 2Cif
    	if( (_header->Height == 288) || (_header->Height == 576) ||
        	(_header->Height == 240) || (_header->Height == 480) )
        	return 1;
	}
	else if(_header->Width == 176)
	{   //QCIF
    	if(_header->Height == 144)
        	return 1;
	}
	else if(_header->Width == 640)
	{
    	//VGA
    	if(_header->Height == 480||_header->Height == 240)//xujiayu for HVGA
        		return 1;
	}
	//add by 1499 for nvr
	else if((_header->Width == 1280)&&(_header->Height== 720))
	{
            	return 1;
	}
	else if((_header->Width == 1280)&&(_header->Height== 960))
	{
            	return 1;
	}
	else if((_header->Width == 1600)&&(_header->Height== 1200))
	{
            	return 1;
	}
	else if((_header->Width == 1920)&&(_header->Height== 1080 || _header->Height== 1088))
	{
            	return 1;
	}
	else if((_header->Width == 960)&&(_header->Height== 576))
	{
            	return 1;
	}
	else if((_header->Width == 960)&&(_header->Height== 480))
	{
            	return 1;
	}
	else if((_header->Width == 320)&&(_header->Height== 240))
	{
            	return 1;
	}
	else if((_header->Width == 2560)&&(_header->Height== 2048))
	{
            	return 1;
	}
	else if((_header->Width == 2560)&&(_header->Height== 1920))
	{
            	return 1;
	}
	else if((_header->Width == 2448)&&(_header->Height== 2048))//QHD
	{
            	return 1;
	}
	else if((_header->Width == 2048)&&(_header->Height== 1536))//QXGA
	{
            	return 1;
	}
    	return -1;
}

//获得系统时间
int decode_GetSysTime(OUT long long *_lTime)
{
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC,&ts);                      //此处可以判断一下返回值
	*_lTime = (long long)ts.tv_sec * 1000 * 1000;
	*_lTime += ts.tv_nsec/1000;
	return 0;
}

//用于创建VO /初始化VO
static int decode_CreateVo(TStream *_This,TRect *_ptRect,int _iVoDev)
{
	int iRet = td_failure;
	TStream *pStream = _This;
	if(pStream == NULL)
	{
		return td_failure;
	}
	pthread_mutex_lock(&pStream->m_StructLock);
	if(pStream->m_pVO != NULL)
	{
		iRet = td_success;
		goto leave;
	}
	if(pStream->m_iForceDestroyFlag)
	{
		goto leave;
	}
	pStream->m_pVO = CreateVO_Ex(_iVoDev, _ptRect, VO_VIDEOLEVEL);
	if(pStream->m_pVO == NULL)
	{
		goto leave;
	}
	pStream->m_pVO->m_iDispDev 			= _iVoDev;
	pStream->m_pVO->m_VoRect.m_iLeft 		= _ptRect->m_iLeft;
	pStream->m_pVO->m_VoRect.m_iTop		= _ptRect->m_iTop;
	pStream->m_pVO->m_VoRect.m_iWidth	= _ptRect->m_iWidth;
	pStream->m_pVO->m_VoRect.m_iHeight	= _ptRect->m_iHeight;
	
	pStream->m_pVO->VoChnCtrl(pStream->m_pVO,VO_ENABLE);
	pStream->m_pVO->VoChnCtrl(pStream->m_pVO, VO_CLEAR);
	//pStream->m_pVO->VoChnCtrl(pStream->m_pVO,VO_SHOW);
	iRet = td_success;
leave:
	pthread_mutex_unlock(&pStream->m_StructLock);
	return iRet;
}
static int decode_GetChnFromFile(TStream *_This)
{
	int i = 0;
	TStream *pStream = _This;
	char *name = pStream->m_File;
	int iLen = strlen(name);
	int iRet = '0';
	iLen = (iLen > FILENAMEMAXNUM) ? FILENAMEMAXNUM : iLen;
	while(i < iLen - 1)
	{
		if(name[i] == 'C' && i != 0)
		{
			iRet = tolower(name[i+1]);
			break;
		}
		i++;
	}
	if(iRet >= '0' && iRet <= '9')
	{
		iRet -= '0';
	}
	else if(iRet >= 'a' && iRet <= 'z')
	{
		iRet -= 'a';
		iRet += 10;
	}
	else
	{
		iRet = 0;
	}
	td_printf(0,"201210081415 iRet = %d",iRet);
	return iRet;
}

static int decode_CreateVdec(TStream *_This)
{
	int iCnt = 0;
	int iRet = td_success;
	int iWidth = 0;
	int iHeight = 0;
	TVdecPara	vDecPara;
	TStream *pStream = _This;

	TRect  *tRect;
	//TRitioRect tRitioRetct;
	td_s32 s32ClipStat;
	td_s32 s32Ret = -1;
	
	if(pStream == NULL)
	{
		return td_failure;
	}
	iWidth = pStream->m_FileHeader.Width;
	iHeight = pStream->m_FileHeader.Height;
	if(iWidth == 0 || iHeight == 0)
	{
		return -2;
	}

	pthread_mutex_lock(&pStream->m_StructLock);
	while(iCnt++ < 5)
	{
	if(pStream->m_iForceDestroyFlag)
	{
		iRet = td_failure;
		goto leave;
	}
	vDecPara.m_iMod = 0;
	vDecPara.m_iType = VENC_H264;
	vDecPara.m_iWidth = iWidth;
	vDecPara.m_iHeight = iHeight;
	td_printf(0,"%s(%d) , iHeight = %d,iWidth = %d\n",__func__,__LINE__,iHeight,iWidth);
	td_printf(0,"%s(%d) , pStream->m_vdecPara.m_iHeight = %d,pStream->m_vdecPara.m_iWidth = %d\n",
		__func__,__LINE__,pStream->m_vdecPara.m_iHeight,pStream->m_vdecPara.m_iWidth);
	td_s32 s32LogChn = decode_GetChnFromFile(_This);
	if(pStream->m_pTVDec != NULL)
	{
		if(pStream->m_vdecPara.m_iHeight != iHeight || pStream->m_vdecPara.m_iWidth != iWidth)
		{
			td_printf(0,"free media vdec\n");
			pStream->m_pVO->UnBind(pStream->m_pVO);
			pStream->m_pVO->AdjustVoRect(pStream->m_pVO, &pStream->m_pVO->m_VoRect);
			media_FreeVdecChannel(pStream->m_pTVDec, s32LogChn);
			pStream->m_pTVDec = NULL;
		}
	}
	if(pStream->m_pTVDec == NULL)
	{
		
		td_printf(0,"201209041402  s32LogChn = %d", s32LogChn);
		pStream->m_pTVDec = media_CreateVdecChannel(&vDecPara, 0, s32LogChn);//RANDOM_NUM);//创建解码通道
		if(pStream->m_pTVDec != NULL)
		{
			//先解绑定
			if(decode_getLobby(iHeight, iWidth))
			{
				TRect Rect;
				float rate1;
				rate1 = ((float)vDecPara.m_iWidth)/((float)vDecPara.m_iHeight);
				Rect.m_iHeight	= pStream->m_pVO->m_VoRect.m_iHeight;
				Rect.m_iWidth		= Rect.m_iHeight * rate1;
				Rect.m_iTop		= pStream->m_pVO->m_VoRect.m_iTop; 
				Rect.m_iLeft		= pStream->m_pVO->m_VoRect.m_iLeft + ((pStream->m_pVO->m_VoRect.m_iWidth -  Rect.m_iWidth) >> 1);
				pStream->m_pVO->AdjustVoRect(pStream->m_pVO, &Rect);
			}
			else
			{
				pStream->m_pVO->AdjustVoRect(pStream->m_pVO, &pStream->m_pVO->m_VoRect);
			}
		
			pStream->m_pVO->m_iBindChn = pStream->m_pTVDec->m_iVdecChan;
			pStream->m_pVO->VoChnCtrl(pStream->m_pVO,VO_CLEAR);
			pStream->m_pVO->Bind(pStream->m_pVO,VO_BIND_VDEC,pStream->m_pVO->m_iBindChn,NULL);
			//pStream->m_pVO->SetVoFrameRate(pStream->m_pVO, g_PlayBkVo[playid]->m_FileHeader.FrameRate + 5);
			//wsl 20120904 for clipside
			tRect = media_GetLogChnClipRect(s32LogChn);
			s32ClipStat = media_GetLogChnClipStat(s32LogChn);
			if(s32ClipStat == 1)
			{
				s32Ret = media_VdecStartClipSide(s32LogChn, tRect);
			}
			//wsl add end
				iRet = td_success;
				memcpy(&pStream->m_vdecPara, &vDecPara, sizeof(TVdecPara));
				goto leave;
		}
		else
		{
			pStream->m_vdecPara.m_iHeight = 0;
			pStream->m_vdecPara.m_iWidth = 0;
			iRet = -3;
				usleep(100 * 1000);
				continue;
			}
		}
		iRet = td_success;
		goto leave;
	}
	
leave:
	pthread_mutex_unlock(&pStream->m_StructLock);
	return iRet;
}
//功能获取当前文件的播放速率
//当_iFlag == 2时，获取当前播放的速率(带补偿)
//当_iFlag == 1时，获取当前播放的速率
//_iFlag == 0时，获取该文件的固有速率
static int decode_GetRate(TStream *_This,double _dSpeed,int _iFlag)//
{
	unsigned short Rate = 0;
	TStream *pStream = _This;
	if(pStream == NULL)
	{
		return -1;
	}
	
	Rate = pStream->m_iFrameRate <= 0 ? 1 : pStream->m_iFrameRate;

	if(_dSpeed <= 0)
		return -2;
	switch(_iFlag)
	{
		case 0:
			break;
		case 1:
			Rate = _dSpeed * Rate;
			Rate++;
			break;
		case 2:
			Rate = _dSpeed*5/4 * Rate;
			Rate++;
			break;
		default :
			;
	}
	return Rate;
}

//设置帧率
static int decode_SetRate(TStream *_This,int _iFrame, int _iPreFrameRate)
{
	int iChg = 0;
	int iDif = 0;
	int iRet = 0;
	TStream *pStream = _This;
	if(pStream == NULL)
	{
		return -1;
	}
	pthread_mutex_lock(&pStream->m_StructLock);
	if(pStream->m_pVO == NULL)
	{
		iRet = -1;
		goto leave;
	}
	if(_iFrame < 0)
	{
		iRet = -2;
		goto leave;
	}
	if(-1 == _iPreFrameRate)
	{
		pStream->m_iFrameRate = _iFrame;
	}
	else if(-2 == _iPreFrameRate)
	{
	}
	else 
	{
		iDif = pStream->m_iFrameRate / 5;
		iChg = (pStream->m_iFrameRate > _iPreFrameRate ? 
		pStream->m_iFrameRate - _iPreFrameRate > iDif : 
		_iPreFrameRate - pStream->m_iFrameRate > iDif);
		if(iChg)
		{
			pStream->m_iFrameRate = _iPreFrameRate;
		}
		
	}
	
	pStream->m_pVO->SetVoFrameRate(pStream->m_pVO, ((_iFrame <= 0) ? 1 : _iFrame) + BASE_VO_VDEC_FRATE);
leave:
	pthread_mutex_unlock(&pStream->m_StructLock);
	return iRet;
}

static int decode_FullScr(TStream *_This)
{
	int iRet = 0;
	int iWidth;
	int iHeight;
	TRect Rect;
	TStream *pStream = _This;
	int iFlag = 0;
	if(pStream == NULL)
	{
		return td_failure;
	}
	pthread_mutex_lock(&pStream->m_StructLock);
	if(pStream->m_pVO == NULL)
	{
		iRet = td_failure;
		goto leave;
	}
	//获取当前VO设备的分辨率
	TdVO_GetVODiff(pStream->m_pVO->m_iDispDev,&iWidth,&iHeight);
	Rect.m_iTop = 0;
	Rect.m_iLeft = 0;
	Rect.m_iWidth = iWidth;
	Rect.m_iHeight = iHeight;

	if(NULL != pStream->m_pTVDec)
	{
		iFlag = decode_getLobby(pStream->m_pTVDec->m_TVdecPara.m_iHeight, pStream->m_pTVDec->m_TVdecPara.m_iWidth);
		if(iFlag)
		{
			TRect Rect1;
			float rate1;
			rate1 = ((float)pStream->m_pTVDec->m_TVdecPara.m_iWidth)/((float)pStream->m_pTVDec->m_TVdecPara.m_iHeight);
			Rect1.m_iHeight	= Rect.m_iHeight;
			Rect1.m_iWidth		= Rect.m_iHeight * rate1;
			Rect1.m_iTop		= Rect.m_iTop; 
			Rect1.m_iLeft		= Rect.m_iLeft + ((Rect.m_iWidth -  Rect1.m_iWidth) >> 1);
			pStream->m_pVO->AdjustVoRect(pStream->m_pVO, &Rect1);
		}
	}

	if(0 == iFlag)
	{
	pStream->m_pVO->AdjustVoRect(pStream->m_pVO,&Rect);
	}
leave:
	pthread_mutex_unlock(&pStream->m_StructLock);
	return iRet;	
}

// 退出全屏
static int decode_FullScrExit(TStream *_This, TRect *_pRect)
{
	int iRet = 0;
	int iFlag = 0;
	TStream *pStream = _This;
	if(pStream == NULL)
	{
		return td_failure;
	}
	pthread_mutex_lock(&pStream->m_StructLock);
	if(pStream->m_pVO == NULL)
	{
		iRet = td_failure;
		goto leave;
	}
	if(_pRect->m_iHeight == 0 || _pRect->m_iWidth == 0)
	{
		iRet = -2;
		goto leave;
	}

	if(NULL != pStream->m_pTVDec)
	{
		iFlag = decode_getLobby(pStream->m_pTVDec->m_TVdecPara.m_iHeight, pStream->m_pTVDec->m_TVdecPara.m_iWidth);
		if(iFlag)
		{
			TRect Rect1;
			float rate1;
			rate1 = ((float)pStream->m_pTVDec->m_TVdecPara.m_iWidth)/((float)pStream->m_pTVDec->m_TVdecPara.m_iHeight);
			Rect1.m_iHeight	= _pRect->m_iHeight;
			Rect1.m_iWidth		= _pRect->m_iHeight * rate1;
			Rect1.m_iTop		= _pRect->m_iTop; 
			Rect1.m_iLeft		= _pRect->m_iLeft + ((_pRect->m_iWidth -  Rect1.m_iWidth) >> 1);
			pStream->m_pVO->AdjustVoRect(pStream->m_pVO, &Rect1);
		}
	}

	if(0 == iFlag)
	{
		pStream->m_pVO->AdjustVoRect(pStream->m_pVO,_pRect);
	}
	
leave:
	pthread_mutex_unlock(&pStream->m_StructLock);
	return iRet;
}

static int decode_ReadFrameIndex(TStream *_This)
{
	int iFlag = 0;
	int iIframeLen = 0;
	int iLocationTemp = 0;
	TStream *pStream = _This;
	S_FrameHeader FrameHeader;//帧头
	iLocationTemp = ftell(pStream->m_fp);	
	//fseek(pStream->m_fp, -2 * sizeof(int), SEEK_END);
	fseek(pStream->m_fp, -(sizeof(TIFrameIndexTail)), SEEK_END);
	fread(&iFlag, 1, sizeof(int), pStream->m_fp);
	if(REC_IFRAME_INDEX_MAGIC_NUMBER != iFlag)
	{
		goto leave;	
	}
	fread(&iIframeLen, 1, sizeof(int), pStream->m_fp);
	fseek(pStream->m_fp, -iIframeLen, SEEK_END);
	fread(&FrameHeader, 1, sizeof(S_FrameHeader), pStream->m_fp);
	if(FrameHeader.u32FrameID != 0x02000000 || FrameHeader.u32FrameSize != iIframeLen)
	{
		goto leave;
	}

	int iINum = 0, iIInfoSize = 0;
	fread(&iINum, 1, sizeof(int), pStream->m_fp);
	fread(&iIInfoSize, 1, sizeof(int), pStream->m_fp);
	fseek(pStream->m_fp, sizeof(int), SEEK_CUR);

	iIframeLen = iINum * iIInfoSize;
	pStream->m_IFrameLen = iINum;
	pStream->m_iIndexLen = iIInfoSize;
	
	if(iIframeLen > 500 * 1024 || (0 == iIframeLen))
	{
		goto leave;
	}
	
	if(pStream->m_mollocLen < iIframeLen)
	{
		if(pStream->m_pIFrameIndex != NULL)
		{
			free(pStream->m_pIFrameIndex);
			pStream->m_pIFrameIndex = NULL;
			pStream->m_mollocLen = 0;
		}
		pStream->m_pIFrameIndex = malloc(iIframeLen);
		if(pStream->m_pIFrameIndex == NULL)
		{
			goto leave;
		}
		pStream->m_mollocLen = iIframeLen;
		td_printf(0," %s, %d  , malloc = %d",__func__,__LINE__, iIframeLen);
	}
	fread(pStream->m_pIFrameIndex, 1, iIframeLen, pStream->m_fp);
	pStream->m_IFrameLoaction = 0;
	fseek(pStream->m_fp, iLocationTemp, SEEK_SET);
	return td_success;
	
leave:
	if(pStream->m_pIFrameIndex != NULL)
	{
		free(pStream->m_pIFrameIndex);
		pStream->m_pIFrameIndex = NULL;
	}
	pStream->m_mollocLen = 0;
	pStream->m_IFrameLen = 0;
	pStream->m_IFrameLoaction = 0;
	fseek(pStream->m_fp, iLocationTemp, SEEK_SET);
	return td_failure;
}

//_iBefAfter: 0:找当前帧所在的I帧
//>0:向前找n帧
//<0:向后找n帧:目前只是支持1帧
static int decode_IFmFromIndex(TStream *_This, int _iLocation, int _iBefAfter, int *_piSize)
{
	int i = 0;
	int iRet = PLAYBACK_FILEFINISH_NOEFFECT;
	char *ptr = NULL;
	TStream *pStream = _This;
	int iLoc = 0;
	int iBefAfter = _iBefAfter < 0 ? (0 - _iBefAfter) : _iBefAfter;
	if(0 == pStream->m_mollocLen)
	{
		return PLAYBACK_NOINDEX;
	}
	ptr = (char *)pStream->m_pIFrameIndex;	

	if(pStream->m_IFrameLoaction > pStream->m_IFrameLen)
	{
		pStream->m_IFrameLoaction = 0;
	}
	if(_iBefAfter < 0)//查找较当前位置靠后的I帧
	{
		i = pStream->m_IFrameLoaction;
		if(*(int *)(ptr + pStream->m_IFrameLoaction * pStream->m_iIndexLen + sizeof(int)) <= _iLocation)
		{
			for(; i < pStream->m_IFrameLen; i++)
			{
				if(*(int *)(ptr + i * pStream->m_iIndexLen + sizeof(int)) > _iLocation)
				{
					pStream->m_IFrameLoaction = i;
					iRet = *(int *)(ptr + i * pStream->m_iIndexLen + sizeof(int));
					iLoc = i;
					goto leave;
				}
			}
		}
		else
		{
			int tmpLoc = *(int *)(ptr + i * pStream->m_iIndexLen + sizeof(int));
			for(; i >= 0; i--)
			{
				if(*(int *)(ptr + i * pStream->m_iIndexLen + sizeof(int)) <= _iLocation)
				{
					pStream->m_IFrameLoaction = i;
					iRet = tmpLoc;
					goto leave;
				}
				tmpLoc = *(int *)(ptr + i * pStream->m_iIndexLen + sizeof(int));
				iLoc = i;
			}
		}
	}
	else //查找较当前位置靠前的I帧
	{
		i = pStream->m_IFrameLoaction; 
		int iCnt = 0;
		int tmpLoc = *(int *)(ptr + i * pStream->m_iIndexLen + sizeof(int));
		if(0 == iBefAfter)
		{
			iBefAfter = 1;
			_iLocation++;
		}
		while(iBefAfter--)
		{
			if((iCnt++) != 0)
			{
				_iLocation = tmpLoc;
			}
 
			if(*(int *)(ptr + pStream->m_IFrameLoaction * pStream->m_iIndexLen + sizeof(int)) < _iLocation)
			{ 
				for(; i < pStream->m_IFrameLen; i++)
				{ 
					if(*(int *)(ptr + i * pStream->m_iIndexLen + sizeof(int)) > _iLocation)
					{ 
						pStream->m_IFrameLoaction = i;
						break;
					}
					tmpLoc = *(int *)(ptr + i * pStream->m_iIndexLen + sizeof(int));
					iLoc = i;
				}
				pStream->m_IFrameLoaction = ((i >= 1) ? (i - 1) : 0);
			}
			else
			{ 
				for(; i >= 0; i--)
				{
					if(*(int *)(ptr + i * pStream->m_iIndexLen + sizeof(int)) < _iLocation)
					{
						pStream->m_IFrameLoaction = i;
						tmpLoc = *(int *)(ptr + i * pStream->m_iIndexLen + sizeof(int));
						iLoc = i;
						break;
					}
				}
				if(i < 0)
				{
					iRet = PLAYBACK_FILEFINISH_NOEFFECT;
					goto leave;
				}
			}
		}
		iRet = tmpLoc;
		goto leave;
	}
leave:
	if((NULL != _piSize) && (iRet > 0) && (pStream->m_iIndexLen > sizeof(int) * 3))
	{
		*_piSize = *(int *)(ptr + iLoc * pStream->m_iIndexLen + 3 * sizeof(int));
	}
	return iRet;
}

static int decode_GetMP4FileInfo(TStream *_This,IN td_s8* _cFileName)
{
	int iRetValue = td_failure;
	TStream *pStream = _This;	

	if((pStream == NULL)||((_cFileName == NULL)))
	{
		goto leave;
	}
	//获取mp4文件编码类型cwm
	if(MP4_GetMP4VideoCodecId((char *)_cFileName, (AVCodecID *)&(pStream->m_stMP4Info.m_iVideoType)) < 0)
	{
		td_printf(0xff0000,"%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
		goto leave;
	}

	//获取MP4 版本
	if(MP4_GetMP4FreeVerSion((char*)_cFileName, 2, &(pStream->m_stMP4Info.m_u32Version)) < 0)
	{
		td_printf(0xff0000,"%s %s %d, _pcFileName = NULL\n", __FILE__, __FUNCTION__, __LINE__);
		goto leave;
	}
	
	//获取MP4文件视频trackId
	if(MP4_GetTrackId_V2(pStream->m_stMP4Info.m_pMp4_fp, &(pStream->m_stMP4Info.m_u32VideoTrackId), "vide") < 0)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		goto leave;
	}

	//获取MP4文件音频trackId
	if(MP4_GetTrackId_V2(pStream->m_stMP4Info.m_pMp4_fp, &(pStream->m_stMP4Info.m_u32AudioTrackId), "soun") < 0)
	{
		//音频获取不到不代表错误，不返回
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
	}
	else
	{
		//获取音频类型
		if(MP4_GetAudioType((char*)_cFileName,&(pStream->m_stMP4Info.m_iAudioType)) < 0)
		{
			//音频类型获取不到不代表文件错误，不返回
			td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		}
	}

	if(AV_CODEC_ID_H264 == pStream->m_stMP4Info.m_iVideoType)
	{
		//获取MP4文件SPS、PPS(每个MP4文件只有一个SPS和PPS)
		if(MP4_GetSpsPps_V2(pStream->m_stMP4Info.m_pMp4_fp, pStream->m_stMP4Info.m_cSps, &(pStream->m_stMP4Info.m_iSpsSize), 
				pStream->m_stMP4Info.m_cPps, &(pStream->m_stMP4Info.m_iPpsSize)) < 0)
		{
			td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
			goto leave;
		}
	}else if(AV_CODEC_ID_HEVC == pStream->m_stMP4Info.m_iVideoType)
	{
		//获取MP4文件VPS、SPS、PPS(h265编码的视频文件多出一个VPS参数集)
		if(MP4_GetVpsSpsPps_V2(pStream->m_stMP4Info.m_pMp4_fp, 
								pStream->m_stMP4Info.m_cVps, &(pStream->m_stMP4Info.m_iVpsSize),
								pStream->m_stMP4Info.m_cSps, &(pStream->m_stMP4Info.m_iSpsSize), 
								pStream->m_stMP4Info.m_cPps, &(pStream->m_stMP4Info.m_iPpsSize)) < 0)
		{
			td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
			goto leave;
		}

	}


	//读取MP4文件索引信息
	if(1 == pStream->m_stMP4Info.m_u32Version)
	{
		if(MP4_ReadSampleIndex_V3((char*)_cFileName, &(pStream->m_stMP4Info.m_stMP4ISampleIndexHead), 
			&(pStream->m_stMP4Info.m_stMP4ISampleIndexTail), &(pStream->m_stMP4Info.m_pIMP4SampleIndex), 
			&(pStream->m_stMP4Info.m_iIMP4SampleIndexmollocLen)) < 0)
		{
			td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
			goto leave;
		}
	}
	else
	{
		if(MP4_ReadSampleIndex_V2(pStream->m_stMP4Info.m_pMp4_fp, &(pStream->m_stMP4Info.m_stMP4ISampleIndexHead), 
			&(pStream->m_stMP4Info.m_stMP4ISampleIndexTail), &(pStream->m_stMP4Info.m_pIMP4SampleIndex), 
			&(pStream->m_stMP4Info.m_iIMP4SampleIndexmollocLen)) < 0)
		{
			td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
			goto leave;
		}
	}

	//检索此文件是否为加密文件
	if(MP4_GetMP4FileEncryptStat((char *)_cFileName, &(pStream->m_stMP4Info.m_iEncryptFlag)) < 0)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		goto leave;
	}

	iRetValue = td_success;
leave:
	return iRetValue;
}

static int  decode_OpenFile(TStream *_This,IN td_s8* _cFileName)
{
	int iRet = 0;
	int iRetValue = 0;
	int iNameLen = 0;
	int iMP4FileType = 0;
	TStream *pStream = _This;	
	S_FileHeader stLastFileHeader = {0};
	
	if(pStream == NULL)
	{
		iRetValue = -1;
		goto leave;
	}
	if(_cFileName == NULL)
	{
		iRetValue = -2;
		goto leave;
	}
	iNameLen = strlen((char *)_cFileName);

	iMP4FileType = MP4_IsMP4File((char*)_cFileName);

	if(TYPE_TIANDY_MP4_FILE == iMP4FileType)//只支持自己转换的MP4文件
	{
		pStream->m_iFileType = FILE_TYPE_MP4;
	}
	else if(TYPE_OTHER_MP4_FILE == iMP4FileType)
	{
		td_printf(0xff0000,"%s %s %d MP4_OpenFile err\n", __FILE__, __FUNCTION__, __LINE__);
		iRetValue = -1;
		goto leave;
	}
	
	if(FILE_TYPE_MP4 == pStream->m_iFileType)
	{
		if(pStream->m_stMP4Info.m_pMp4_fp != NULL)
		{
			MP4_CloseFile(&(pStream->m_stMP4Info.m_pMp4_fp));
			pStream->m_stMP4Info.m_pMp4_fp = NULL;
		}
		
		if(TD_SUCCESS != MP4_OpenFile(&(pStream->m_stMP4Info.m_pMp4_fp),(char *)_cFileName))
		{
			td_printf(0xff0000,"%s %s %d MP4_OpenFile err\n", __FILE__, __FUNCTION__, __LINE__);
			iRetValue = -1;
			goto leave;
		}

		if(decode_GetMP4FileInfo(pStream, _cFileName) < 0)
		{
			td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
			iRetValue = -1;
			goto leave;
		}
	}
	else
	{
		if(pStream->m_fp != NULL)
		{
			rec_CloseFile(&pStream->m_fp);
			pStream->m_fp = NULL;
		}

		if(ERR_REC_SUCCESSFUL != rec_OpenFile(&pStream->m_fp,(td_s8 *)_cFileName))
		{
			td_printf(0,"rec_OpenFile err\n");
			if(pStream->m_fp != NULL)
			{
				rec_CloseFile(&pStream->m_fp);
				pStream->m_fp = NULL;
			}
			iRetValue = -1;
			goto leave;
		}

		if(decode_TestBufFlag())
		{
			rec_SetBuf(pStream->m_fp, DECODE_BUFSIZE);
		}

		rec_GetFileSize((td_s8 *)_cFileName, &pStream->m_iFileLen);

		decode_ReadFrameIndex(_This);
		pStream->m_iFileLen -= pStream->m_IFrameLen;
	}

	stLastFileHeader = pStream->m_FileHeader;	

	if(FILE_TYPE_MP4 == pStream->m_iFileType)
	{
		if(1 == pStream->m_stMP4Info.m_u32Version)
		{
			iRet = MP4_ReadSdvFileHeader((char*)_cFileName, &(pStream->m_FileHeader));
		}
		else
		{
			iRet = MP4_ReadSdvFileHeader_V2(pStream->m_stMP4Info.m_pMp4_fp, &(pStream->m_FileHeader));
		}

		if(iRet < 0)
		{
			td_printf(0xff0000,"MP4_ReadSdvFileHeader err %d\n", iRet);			
			iRetValue = -4;
			goto leave;
		}
		td_printf(0,"MP4_ReadSdvFileHeader pStream->m_FileHeader.FrameRate = %d", pStream->m_FileHeader.FrameRate);
	}
	else
	{
		iRet = rec_ReadFileHeader(pStream->m_fp, (td_u8 *)_cFileName, &pStream->m_FileHeader);
		td_printf(0,"pStream->m_FileHeader.FrameRate = %d", pStream->m_FileHeader.FrameRate);
		if(ERR_REC_SUCCESSFUL != iRet)
		//if(ERR_REC_SUCCESSFUL != rec_ReadFileHeader(pStream->m_fp, (td_s8 *)_cFileName, &pStream->m_FileHeader))
		{
			td_printf(0,"rec_ReadFileHeader err %d\n",iRet);
			if(pStream->m_fp != NULL)
			{
				rec_CloseFile(&pStream->m_fp);
				pStream->m_fp = NULL;
			}
			iRetValue = -4;
			goto leave;
		}
	}
	
	if(IsValidFileHeader(&pStream->m_FileHeader) < 0)
	{
		td_printf(0,"IsValidFileHeader err\n");
		rec_CloseFile(&pStream->m_fp);
		pStream->m_fp = NULL;
		
		iRetValue = -4;
		goto leave;
	}
	pStream->m_iFrameRate = pStream->m_FileHeader.FrameRate;
	pStream->m_FileLoction = 88;
	pStream->m_iOrigFrameLen = pStream->m_FileHeader.Height * pStream->m_FileHeader.Width * 1.5;
	//复制文件名
	memset(pStream->m_File,0,FILENAMEMAXNUM);
	memcpy(pStream->m_File,_cFileName,iNameLen);

	if(stLastFileHeader.AudioSample != pStream->m_FileHeader.AudioSample)
	{
		TdAO_Stop(pStream->m_AO.m_iAoId);	
		pStream->m_AO.m_iAoId = -1;
	}

leave:
	if((iRetValue < 0)&&(FILE_TYPE_MP4 == pStream->m_iFileType))
	{
		if(pStream->m_stMP4Info.m_pMp4_fp != NULL)
		{
			MP4_CloseFile(&(pStream->m_stMP4Info.m_pMp4_fp));
			pStream->m_stMP4Info.m_pMp4_fp = NULL;
		}
	}
	return iRetValue;
}


static int  decode_CloseFile(TStream *_This)
{
	TStream *pStream = _This;
	if(pStream == NULL)
	{
		return -1;
	}

	if(FILE_TYPE_MP4 == pStream->m_iFileType)
	{
		if(pStream->m_stMP4Info.m_pMp4_fp != NULL)
		{
			MP4_CloseFile(&(pStream->m_stMP4Info.m_pMp4_fp));
		}
	}
	else
	{
	if(pStream->m_fp != NULL)
	{
		rec_CloseFile(&pStream->m_fp);
		pStream->m_fp = NULL;	
	}
	}
	return 0;
}
//返回 -2:代表错误帧
//		  -1:代表插入帧
//            0:代表最后的索引帧
//		1:代表正常帧
static td_s32 Is_InsertFrame(S_FrameHeader *_pTFrameheader)
{
	if(_pTFrameheader->u32FrameID == 0 && _pTFrameheader->u32FrameSize == 0)
	{
		return PLAYBACK_READFILE_ERR;
	}
	if(1 != IsValidFrame(_pTFrameheader))
	{
		return PLAYBACK_READFILE_ERR;
	}
	if(_pTFrameheader->reserved == REC_FALSE_FRAME_FLAG)
	{
		return PLAYBACK_PLUGFRAME;
	}
	if(_pTFrameheader->reserved != 0
	  || _pTFrameheader->u16AFrames != 0
	  || _pTFrameheader->u16AFrameSize != 0
	  || _pTFrameheader->u32TimeStamp != 0)
	{
		return 1;
	}
	return 0;
}

static td_s32 RDIFrame(TStream * _This)
{
	int iRet;
	int iLoc = 0;
	int iRealLocation = 0;
	int iLocation = 0;
	int iFrameLen = sizeof(S_FrameHeader);
	TStream *pStream = _This;

	if(NULL == pStream)
	{
		iRet = PLAYBACK_READFILE_ERR;
		goto leave1;
	}

	if(FILE_TYPE_MP4 == pStream->m_iFileType)
	{
		if(pStream->m_stMP4Info.m_pMp4_fp == NULL)
		{
			td_printf(0, "%s start!----->error", __FUNCTION__);
			iRet = PLAYBACK_READFILE_ERR;
			goto leave1;
		}

		if(1 != pStream->m_stMP4Info.m_iEncryptFlag)
		{
			#if 1
			if(AV_CODEC_ID_H264 == pStream->m_stMP4Info.m_iVideoType)
			{
				if(MP4_GetOneTiandyFrame_V2(pStream->m_stMP4Info.m_pMp4_fp, 
											pStream->m_stMP4Info.m_u32VideoTrackId, 
											pStream->m_stMP4Info.m_u32AudioTrackId,
											&(pStream->m_stMP4Info.m_uiCurVideoSampleId), 
											&(pStream->m_stMP4Info.m_uiCurAudioSampleId), 
											pStream->m_stMP4Info.m_cSps, 
											pStream->m_stMP4Info.m_iSpsSize, 
											pStream->m_stMP4Info.m_cPps, 
											pStream->m_stMP4Info.m_iPpsSize,
											pStream->m_pBuf, 
											&(pStream->m_FrameHeader), 
											pStream->m_stMP4Info.m_iAudioType, 
											&(pStream->m_stMP4Info.m_u32UseLessIdCount), 1) < 0)
				{
					td_printf(0xff0000, "%s %s %d MP4_GetOneTiandyFrame_V2 err!", __FILE__, __FUNCTION__, __LINE__);
					iRet = PLAYBACK_READFILE_ERR; 
					goto leave1;
				}
			}else if(AV_CODEC_ID_HEVC == pStream->m_stMP4Info.m_iVideoType)
			{
				if(MP4_GetOneTiandyFrameH265_V2(pStream->m_stMP4Info.m_pMp4_fp, 
											pStream->m_stMP4Info.m_u32VideoTrackId, 
											pStream->m_stMP4Info.m_u32AudioTrackId,
											&(pStream->m_stMP4Info.m_uiCurVideoSampleId), 
											&(pStream->m_stMP4Info.m_uiCurAudioSampleId), 
											pStream->m_stMP4Info.m_cVps, 
											pStream->m_stMP4Info.m_iVpsSize, 
											pStream->m_stMP4Info.m_cSps, 
											pStream->m_stMP4Info.m_iSpsSize, 
											pStream->m_stMP4Info.m_cPps, 
											pStream->m_stMP4Info.m_iPpsSize,
											pStream->m_pBuf, 
											&(pStream->m_FrameHeader), 
											pStream->m_stMP4Info.m_iAudioType, 
											&(pStream->m_stMP4Info.m_u32UseLessIdCount), 1) < 0)
				{
					td_printf(0xff0000, "%s %s %d MP4_GetOneTiandyFrameH265_V2 err!", __FILE__, __FUNCTION__, __LINE__);
					iRet = PLAYBACK_READFILE_ERR; 
					goto leave1;
				}

			}else
			{
				//todo
			}
			#else
			if(AV_CODEC_ID_H264 == pStream->m_stMP4Info.m_iVideoType)
			{
				if(MP4_GetOneITiandyFrame(pStream->m_stMP4Info.m_pMp4_fp,
										pStream->m_stMP4Info.m_u32VideoTrackId,
										pStream->m_stMP4Info.m_u32AudioTrackId,
										&(pStream->m_stMP4Info.m_uiCurVideoSampleId),
										&(pStream->m_stMP4Info.m_uiCurAudioSampleId),
										pStream->m_stMP4Info.m_cSps,
										pStream->m_stMP4Info.m_iSpsSize,
										pStream->m_stMP4Info.m_cPps,
										pStream->m_stMP4Info.m_iPpsSize,
										pStream->m_pBuf,
										&(pStream->m_FrameHeader),
										pStream->m_stMP4Info.m_iAudioType) < 0)
				{
					td_printf(0xff0000, "%s %s %d MP4_GetOneITiandyFrame err!", __FILE__, __FUNCTION__, __LINE__);
					iRet = PLAYBACK_READFILE_ERR; 
					goto leave1;
				}
			}else if(AV_CODEC_ID_HEVC == pStream->m_stMP4Info.m_iVideoType)
			{
				if(MP4_GetOneITiandyFrameH265(pStream->m_stMP4Info.m_pMp4_fp,
										pStream->m_stMP4Info.m_u32VideoTrackId,
										pStream->m_stMP4Info.m_u32AudioTrackId,
										&(pStream->m_stMP4Info.m_uiCurVideoSampleId),
										&(pStream->m_stMP4Info.m_uiCurAudioSampleId),
										pStream->m_stMP4Info.m_cVps,
										pStream->m_stMP4Info.m_iVpsSize,
										pStream->m_stMP4Info.m_cSps,
										pStream->m_stMP4Info.m_iSpsSize,
										pStream->m_stMP4Info.m_cPps,
										pStream->m_stMP4Info.m_iPpsSize,
										pStream->m_pBuf,
										&(pStream->m_FrameHeader),
										pStream->m_stMP4Info.m_iAudioType) < 0)
				{
					td_printf(0xff0000, "%s %s %d MP4_GetOneITiandyFrameH265 err!", __FILE__, __FUNCTION__, __LINE__);
					iRet = PLAYBACK_READFILE_ERR; 
					goto leave1;
				}
			}else
			{
				//todo
			}
			#endif
		}
		else
		{
			if(AV_CODEC_ID_H264 == pStream->m_stMP4Info.m_iVideoType)
			{
				if(MP4_GetOneEncryptTiandyFrame(pStream->m_stMP4Info.m_pMp4_fp, 
												pStream->m_stMP4Info.m_u32VideoTrackId,
												pStream->m_stMP4Info.m_u32AudioTrackId, 
												&(pStream->m_stMP4Info.m_uiCurVideoSampleId), 
												&(pStream->m_stMP4Info.m_uiCurAudioSampleId),
												pStream->m_pBuf, 
												&(pStream->m_FrameHeader), 
												pStream->m_stMP4Info.m_iAudioType, 1) < 0)
				{
					td_printf(0xff0000, "%s %s %d MP4_GetOneEncryptTiandyFrame err!", __FILE__, __FUNCTION__, __LINE__);
					iRet = PLAYBACK_READFILE_ERR; 
					goto leave1;
				}
			}else if(AV_CODEC_ID_HEVC == pStream->m_stMP4Info.m_iVideoType)
			{
				if(MP4_GetOneEncryptTiandyFrameH265(pStream->m_stMP4Info.m_pMp4_fp, 
												pStream->m_stMP4Info.m_u32VideoTrackId,
												pStream->m_stMP4Info.m_u32AudioTrackId, 
												&(pStream->m_stMP4Info.m_uiCurVideoSampleId), 
												&(pStream->m_stMP4Info.m_uiCurAudioSampleId),
												pStream->m_pBuf, 
												&(pStream->m_FrameHeader), 
												pStream->m_stMP4Info.m_iAudioType, 1) < 0)
				{
					td_printf(0xff0000, "%s %s %d MP4_GetOneEncryptTiandyFrameH265 err!", __FILE__, __FUNCTION__, __LINE__);
					iRet = PLAYBACK_READFILE_ERR; 
					goto leave1;
				}
			}else
			{
				//todo
			}
		}

		iRet = PLAYBACK_IFRAME;
		goto leave1;
	}
	else
	{
	if(pStream->m_iOnlyIFrame == 1)
	{		

		iLoc = pStream->m_FileLoction;

		pStream->m_iFrameLoc = pStream->m_FileLoction;
		
		iRealLocation = ftell(pStream->m_fp);
		if((iLocation = decode_IFmFromIndex(_This, iLoc, -1, NULL)) < 0)
		{
			iRet = iLocation;
				goto leave2;
		}
		fseek(pStream->m_fp, iLocation, SEEK_SET);
		if(fread(&pStream->m_FrameHeader,1,iFrameLen,pStream->m_fp) < iFrameLen)
		{
			iRet = PLAYBACK_READFILE_ERR;
			td_printf(0,"read m_FrameHeader err");
				goto leave2;
		}
		
		iRet = Is_InsertFrame(&(pStream->m_FrameHeader));
		if(iRet < 0)
		{	
				goto leave2;//插入帧
		}
		
		if(fread(pStream->m_pBuf,1,pStream->m_FrameHeader.u32FrameSize - iFrameLen,pStream->m_fp) < pStream->m_FrameHeader.u32FrameSize - iFrameLen)
		{
			iRet = PLAYBACK_READFILE_ERR;
			td_printf(0,"read data err,len is err");
				goto leave2;
		}
		pStream->m_iIFrameFileLoc = iLocation;
		pStream->m_FileLoction = pStream->m_FrameHeader.u32FrameSize + iLocation;
		return PLAYBACK_IFRAME;
	}
	}
	
leave1:
	return iRet;
leave2:
	fseek(pStream->m_fp, iRealLocation, SEEK_SET);
	td_printf(0,"index_frame is err");
	return iRet;
}
static td_s32 RDFrame(TStream *_This)// -2:读文件错误
{
	int iRet = 0;
	int iFrameLen = sizeof(S_FrameHeader);
	TStream *pStream = _This;

	if(FILE_TYPE_MP4 == pStream->m_iFileType)
	{	
		if(pStream == NULL || pStream->m_stMP4Info.m_pMp4_fp == NULL)
		{
			td_printf(0, "%s start!----->error", __FUNCTION__);
			iRet = PLAYBACK_READFILE_ERR;
			goto leave;
		}

		if(1 != pStream->m_stMP4Info.m_iEncryptFlag)
		{
			if(AV_CODEC_ID_H264 == pStream->m_stMP4Info.m_iVideoType)
			{
				if(MP4_GetOneTiandyFrame_V2(pStream->m_stMP4Info.m_pMp4_fp, 
											pStream->m_stMP4Info.m_u32VideoTrackId, 
											pStream->m_stMP4Info.m_u32AudioTrackId,
											&(pStream->m_stMP4Info.m_uiCurVideoSampleId), 
											&(pStream->m_stMP4Info.m_uiCurAudioSampleId), 
											pStream->m_stMP4Info.m_cSps, 
											pStream->m_stMP4Info.m_iSpsSize, 
											pStream->m_stMP4Info.m_cPps, 
											pStream->m_stMP4Info.m_iPpsSize,
											pStream->m_pBuf, 
											&(pStream->m_FrameHeader), 
											pStream->m_stMP4Info.m_iAudioType, 
											&(pStream->m_stMP4Info.m_u32UseLessIdCount), 0) < 0)
				{
					td_printf(0xff0000, "%s %s %d MP4_GetOneTiandyFrame_V2 err!", __FILE__, __FUNCTION__, __LINE__);
					iRet = PLAYBACK_READFILE_ERR; 
					goto leave;
				}
			}else if(AV_CODEC_ID_HEVC == pStream->m_stMP4Info.m_iVideoType)
			{
				if(MP4_GetOneTiandyFrameH265_V2(pStream->m_stMP4Info.m_pMp4_fp, 
											pStream->m_stMP4Info.m_u32VideoTrackId, 
											pStream->m_stMP4Info.m_u32AudioTrackId,
											&(pStream->m_stMP4Info.m_uiCurVideoSampleId), 
											&(pStream->m_stMP4Info.m_uiCurAudioSampleId), 
											pStream->m_stMP4Info.m_cVps, 
											pStream->m_stMP4Info.m_iVpsSize, 
											pStream->m_stMP4Info.m_cSps, 
											pStream->m_stMP4Info.m_iSpsSize, 
											pStream->m_stMP4Info.m_cPps, 
											pStream->m_stMP4Info.m_iPpsSize,
											pStream->m_pBuf, 
											&(pStream->m_FrameHeader), 
											pStream->m_stMP4Info.m_iAudioType, 
											&(pStream->m_stMP4Info.m_u32UseLessIdCount), 0) < 0)
				{
					td_printf(0xff0000, "%s %s %d MP4_GetOneTiandyFrameH265_V2 err!", __FILE__, __FUNCTION__, __LINE__);
					iRet = PLAYBACK_READFILE_ERR; 
					goto leave;
				}

			}else
			{
				//todo
			}
		}
		else
		{
			if(AV_CODEC_ID_H264 == pStream->m_stMP4Info.m_iVideoType)
			{
				if(MP4_GetOneEncryptTiandyFrame(pStream->m_stMP4Info.m_pMp4_fp, 
												pStream->m_stMP4Info.m_u32VideoTrackId,
												pStream->m_stMP4Info.m_u32AudioTrackId, 
												&(pStream->m_stMP4Info.m_uiCurVideoSampleId), 
												&(pStream->m_stMP4Info.m_uiCurAudioSampleId),
												pStream->m_pBuf, 
												&(pStream->m_FrameHeader), 
												pStream->m_stMP4Info.m_iAudioType, 0) < 0)
				{
					td_printf(0xff0000, "%s %s %d MP4_GetOneEncryptTiandyFrame err!", __FILE__, __FUNCTION__, __LINE__);
					iRet = PLAYBACK_READFILE_ERR; 
					goto leave;
				}
			}else if(AV_CODEC_ID_HEVC == pStream->m_stMP4Info.m_iVideoType)
			{
				if(MP4_GetOneEncryptTiandyFrameH265(pStream->m_stMP4Info.m_pMp4_fp, 
												pStream->m_stMP4Info.m_u32VideoTrackId,
												pStream->m_stMP4Info.m_u32AudioTrackId, 
												&(pStream->m_stMP4Info.m_uiCurVideoSampleId), 
												&(pStream->m_stMP4Info.m_uiCurAudioSampleId),
												pStream->m_pBuf, 
												&(pStream->m_FrameHeader), 
												pStream->m_stMP4Info.m_iAudioType, 0) < 0)
				{
					td_printf(0xff0000, "%s %s %d MP4_GetOneEncryptTiandyFrameH265 err!", __FILE__, __FUNCTION__, __LINE__);
					iRet = PLAYBACK_READFILE_ERR; 
					goto leave;
				}
			}else
			{
				//todo
			}
		}
	}
	else
	{
		if(pStream == NULL || pStream->m_fp == NULL)
		{
			iRet = PLAYBACK_READFILE_ERR;
			goto leave;
		}

		pStream->m_iFrameLoc = pStream->m_FileLoction;
		fseek(pStream->m_fp, pStream->m_FileLoction, SEEK_SET);

		if(fread(&pStream->m_FrameHeader,1,iFrameLen,pStream->m_fp) < iFrameLen)
		{
			//decode_CloseFile(_This);
			iRet = PLAYBACK_READFILE_ERR;
			goto leave;
		}

		//改变文件指针
		pStream->m_FileLoction += pStream->m_FrameHeader.u32FrameSize;
		if((iRet  = Is_InsertFrame(&(pStream->m_FrameHeader))) < 0)
		{
			goto leave;
		}
		else if(0 == iRet)
		{
			iRet = PLAYBACK_FILEFINISH_NOEFFECT;
			goto leave;
		}
		
		//读数据
		int temp = pStream->m_FrameHeader.u32FrameSize - iFrameLen;
		if(pStream->m_iOnlyIFrame != 1 || (pStream->m_iOnlyIFrame == 1 && pStream->m_FrameHeader.u8FrameType == 0))
		{
			if(fread(pStream->m_pBuf,1,temp,pStream->m_fp) < temp)
			{
				iRet = PLAYBACK_READFILE_ERR;
				//decode_CloseFile(_This);
				goto leave;
			}
		}
		else
		{
			fseek(pStream->m_fp, pStream->m_FileLoction, SEEK_SET);
		}

		//判断文件是否结束
		if(pStream->m_FileLoction >= pStream->m_iFileLen)
		{
			td_printf(0,"%s , %d",__func__,__LINE__);
			iRet = PLAYBACK_FILEFINISH_NOEFFECT;
			goto leave;
		}
		if(pStream->m_FrameHeader.u8FrameType == 0)//I帧
		{
			iRet = PLAYBACK_IFRAME;
			pStream->m_iIFrameFileLoc = pStream->m_iFrameLoc;
			goto leave;
		}
		iRet = td_success;
	}
	
leave:
	return iRet;
}


td_s32 decode_readfile(TStream *_This)
{
	int iRet = PLAYBACK_NOINDEX;
	TStream *pStream = _This;

	do
	{
		if(pStream->m_iOnlyIFrame == 1)
		{
			iRet = RDIFrame(_This);
		}
		if(PLAYBACK_NOINDEX == iRet || PLAYBACK_PLUGFRAME == iRet)
		{
			iRet = RDFrame(_This);
		}
	}while(PLAYBACK_PLUGFRAME == iRet);
	
	return iRet;
}


static td_s32 decode_ReadFrame(TStream *_This)
{
	int iRet = 0;
	TStream *pStream = _This;
	if(pStream->m_iBackFlag)
	{
		iRet = decode_ReadMem(_This);
	}
	else
	{
		iRet = decode_readfile(_This);
	}
	return iRet;
}

static td_s32 decode_TestEncrypt(TStream *_This, char* _cPassword)
{
	//测试密码是否匹配
	int *ptr = NULL;
	unsigned char buf[64];
	TStream *pStream = _This;
	int iLen = strlen(_cPassword);
	if(iLen == 0)
	{
		return -1;
	}
	memcpy(buf, pStream->m_tEncrypbuf.EncrptTestBuf, 64);
	aes_crypt(buf, 64, pStream->m_tEncrypbuf.FrameID, (unsigned char*)_cPassword, iLen);
	ptr = (int *)buf;
	if( *ptr != 0X1000000 )
	{
		return -1;
	}
	return 0;
}



static td_s32 decode_SendFileVStream(TStream *_This)
{
	int iRet = 0;
	TStream *pStream = _This;
	int VStreamLen = 0;
	unsigned int VStreamPts	= 0;		//zhangyi add 20130601
	int iAudioSumLen = 0;
	if(pStream == NULL)
	{
		return -100;
	}
	pthread_mutex_lock(&pStream->m_StructLock);
	if(FILE_TYPE_MP4 != pStream->m_iFileType)
	{
	if(pStream->m_fp == NULL || pStream->m_pTVDec == NULL ||pStream->m_pBuf == NULL)
	{
		iRet = -100;
		goto leave;
	}
	}
	else
	{	
		if(pStream->m_stMP4Info.m_pMp4_fp == NULL || pStream->m_pTVDec == NULL ||pStream->m_pBuf == NULL)
		{
			iRet = -100;
			goto leave;
		}
	}
	VStreamLen = pStream->m_FrameHeader.u32VStreamLen;//视频帧的长度
	VStreamPts		= pStream->m_FrameHeader.u32TimeStamp;	//zhangyi add 20130601
	iAudioSumLen = pStream->m_FrameHeader.u16AFrames * pStream->m_FrameHeader.u16AFrameSize;
	//检查数据
	if((pStream->m_FrameHeader.reserved & 0x03) == 0x02 && (iAudioSumLen + VStreamLen) >= 64)
	{
		unsigned char temp[64];
		//如果密码为空则返回错误	
		//td_printf(0,"m_ucEncrypt = %s\n",pStream->m_ucEncrypt);
		int iEncryptLen = strlen((char *)pStream->m_ucEncrypt);
		if(iEncryptLen == 0)
		{
			memcpy(pStream->m_tEncrypbuf.EncrptTestBuf, pStream->m_pBuf, 64);
			pStream->m_tEncrypbuf.FrameID = pStream->m_FrameHeader.u32FrameNO;
			iRet = 1;
			goto leave;
		}
		memcpy(temp, pStream->m_pBuf, 64);
		aes_crypt(pStream->m_pBuf,64,pStream->m_FrameHeader.u32FrameNO,pStream->m_ucEncrypt,iEncryptLen);
		if( ((int *)pStream->m_pBuf)[0] != 0X1000000 )
		{
			memcpy(pStream->m_pBuf,temp, 64);
			memcpy(pStream->m_tEncrypbuf.EncrptTestBuf, temp, 64);
			pStream->m_tEncrypbuf.FrameID = pStream->m_FrameHeader.u32FrameNO;
			iRet = 1;
			goto leave;
		}
	}			
	//发送数据
	//zhangyi modify 20130601		add pts
	//td_printf(0x12345678, "--------------------->send stream to media!");
	//media_VdecSendStream
	if (0 != pStream->m_pTVDec->SendStream(pStream->m_pTVDec,pStream->m_pBuf,VStreamLen,0,0))
    	{
		iRet = -8;
		goto leave;
    	}
	//处理文件的进度
leave:
	pthread_mutex_unlock(&pStream->m_StructLock);
	return iRet;
}

int decode_SendVStream(TStream *_This)
{
	int iRet = 0;
	TStream *pStream = _This;
	if(pStream->m_iBackFlag)
	{
		//td_printf(0, "--------------------------->decode_SendMemVStream");
		iRet = decode_SendMemVStream(_This);
	}
	else
	{
		//td_printf(0, "--------------------------->decode_SendFileVStream");
		iRet = decode_SendFileVStream(_This);
	}
	return iRet;
}

//wsl add 将默认音频输出通道保存到全局变量中
int decode_setdefaultAo(int _iDefaultAo)
{
	if(_iDefaultAo >= 0)
	{
		g_defaultAo = _iDefaultAo;
	}
	
	return td_success;
}

static int decode_CreateAo(TStream *_This)
{
	int iRet = 0;
	int iNum = -1;
   	int iAFrameSize = 0;
	int iAFrames = 0;
	TStream *pStream = _This;
	iAFrameSize = pStream->m_FrameHeader.u16AFrameSize;//视频帧的长度
	iAFrames = pStream->m_FrameHeader.u16AFrames;//视频帧的长度
	if(pStream->m_iForceDestroyFlag)
	{
		iRet = -6;
		goto leave;
	}
	if(pStream->m_AO.m_iAoId <= 0)	
	{		
		iNum = AudioFramLenToAoPoiNum(pStream->m_FrameHeader.u8ACoder, iAFrameSize);	
		if(iNum == -1)		
		{
			iRet = -6;
			goto leave;
		}		
		pStream->m_AO.m_iAencType = pStream->m_FrameHeader.u8ACoder;	
		pStream->m_AO.m_iAoPoiNum = iNum;	
		if((AUDIO_TYPE_MEDIA_AAC == pStream->m_AO.m_iAencType)&&(8000 == pStream->m_FileHeader.AudioSample))
		{
			pStream->m_FileHeader.AudioSample = 32000;
		}
		pStream->m_AO.m_iAoId = TdAO_Start(g_defaultAo, AO_PRI_PALYBACK, AO_BIND_ADEC, 0, 
			                                                     pStream->m_AO.m_iAencType, pStream->m_AO.m_iAoPoiNum,
			                                        			pStream->m_FileHeader.AudioSample);	
	}	
	else	
	{	
		iNum = AudioFramLenToAoPoiNum(pStream->m_FrameHeader.u8ACoder, iAFrameSize);	
		if(iNum == -1)	
		{		
			iRet = -6;
			goto leave;
		}		
		if(pStream->m_FrameHeader.u8ACoder != pStream->m_AO.m_iAencType 
		   || iNum != pStream->m_AO.m_iAoPoiNum )	
		{		
			TdAO_Stop(pStream->m_AO.m_iAoId);	
			pStream->m_AO.m_iAoId = -1;
			iRet = -6;
		}
	}
leave:
	return iRet;
}

static td_s32 decode_copyAudioStream(TStream *_This, int _iDes, int _iSrc, int _iSize)
{
	TStream *pStream = _This;
	if(pStream->m_pBuf == NULL)
	{
		return -1;
	}

	memcpy((char *)(pStream->m_pBuf) + _iDes, (char *)pStream->m_pBuf + _iSrc, _iSize);
	return 0;
}


static td_s32 decode_clearLeaveAudioStream(TStream *_This)
{
	TStream *pStream = _This;
	pStream->m_iLeaveAudioFrames = 0;
	pStream->m_iLeaveAudioLoaction = 0;
	pStream->m_iLeaveAudioFramesLen = 0;
	return 0;
}

#define AUDIO_SEND_BUF_FULL	0xa018800f
static td_s32 decode_SendAStream(TStream *_This)
{
	int iRet = 0;
	TStream *pStream = _This;
    	int iAFrameSize = 0;
	int iAFrames = 0;
	int i = 0;
	if(pStream == NULL)
	{
		return -100;
	}
	pthread_mutex_lock(&pStream->m_StructLock);
	if(FILE_TYPE_MP4 == pStream->m_iFileType)
	{
		if(pStream->m_stMP4Info.m_pMp4_fp == NULL || pStream->m_pTVDec == NULL ||pStream->m_pBuf == NULL)
		{
			iRet = -100;
			goto leave;
		}
	}
	else
	{
		if(pStream->m_fp == NULL || pStream->m_pTVDec == NULL ||pStream->m_pBuf == NULL)
		{
			iRet = -100;
			goto leave;
		}
	}

	if(pStream->m_iBackFlag)
	{
		goto leave;
	}
	iAFrameSize = pStream->m_FrameHeader.u16AFrameSize;//视频帧的长度
	iAFrames = pStream->m_FrameHeader.u16AFrames;//视频帧的长度
	//创建音频通道
	if(decode_CreateAo(_This) < 0)
	{
		iRet = -1;
		goto leave;
	}	

	if(0 == iAFrames)
	{
		pStream->m_u16ZeroAudioFrames++;
		if(pStream->m_u16ZeroAudioFrames > 100)
		{
			pStream->m_u16ZeroAudioFrames = 100;
		}
		iRet = 0;
		goto leave;
	}
	
	//如果有一视频帧后带的音频帧数突然增大，则将此帧音频不播放。
	#if 0
	if(iAFrames > pStream->m_u16BefAudioFrames + 10 || pStream->m_u16ZeroAudioFrames == 10)
	{
		pStream->m_u16BefAudioFrames = iAFrames;
		pStream->m_u16ZeroAudioFrames = 0;
		return -1;
	}
	pStream->m_u16ZeroAudioFrames = 0;
	pStream->m_u16BefAudioFrames = iAFrames;
	#endif
	if(pStream->m_u16ZeroAudioFrames == 100)
	{
		pStream->m_u16ZeroAudioFrames = 0;
		iRet = -1;
		goto leave;
	}
	pStream->m_u16ZeroAudioFrames = 0;
	int iAudioRes = 0;

//#if 1	
	char *pAudioBuf;
	pAudioBuf = (char *)(pStream->m_pBuf + pStream->m_FrameHeader.u32VStreamLen);
	//发送音频帧
	if(AUDIO_TYPE_MEDIA_AAC == pStream->m_AO.m_iAencType )
	{		
		unsigned char ptAacHeader[7] = {0,0,0,0,0,0,0};
		int iAudioLen = 0;
		int iAudioStreamLen = 0;
		//int s = 0;
		int a = 0;

		//td_printf(0, "pStream->m_FrameHeader.u16AFrameSize is %d", pStream->m_FrameHeader.u16AFrameSize);
		while(iAudioStreamLen < pStream->m_FrameHeader.u16AFrameSize)
		{
			memcpy(ptAacHeader, pAudioBuf, sizeof(ptAacHeader));
			if(0 == a)
			{
				#if 0
				td_printf(0, "ptAacHeader[0] is %2x", ptAacHeader[0]);
				td_printf(0, "ptAacHeader[1] is %2x", ptAacHeader[1]);
				td_printf(0, "ptAacHeader[2] is %2x", ptAacHeader[2]);
				td_printf(0, "ptAacHeader[3] is %2x", ptAacHeader[3]);
				td_printf(0, "ptAacHeader[4] is %2x", ptAacHeader[4]);
				td_printf(0, "ptAacHeader[5] is %2x", ptAacHeader[5]);
				td_printf(0, "ptAacHeader[6] is %2x", ptAacHeader[6]);
				#endif
				a = 1;
			}
			if(ptAacHeader[0] == 0xff && (ptAacHeader[1] & 0xf0) == 0xf0)
			{
				//calc len
				iAudioLen = ((ptAacHeader[3] & 0x03)<<11) + (ptAacHeader[4]<<3) + ((ptAacHeader[5] & 0xe0) >> 5);
				//td_printf(0, "%s, %d, iAudioLen = %d", __func__, __LINE__, iAudioLen);
				if(iAudioLen == 0)
				{
					pAudioBuf++;
					iAudioStreamLen++;		
					continue;
				}
				//td_printf(0,"20140329 pbpb iAudioLen = %d, pStream->m_FrameHeader.u16AFrameSize %d, iAudioStreamLen %d",
				//	 iAudioLen, pStream->m_FrameHeader.u16AFrameSize, iAudioStreamLen);
				iRet = TdAO_SendStream(pStream->m_AO.m_iAoId, (td_u8 *)(pAudioBuf), iAudioLen, 0); 
				if(iRet == 0)
				{
					pAudioBuf += iAudioLen;
					iAudioStreamLen += iAudioLen;
				}
				else
				{
					//td_printf(0xff0000, "---->iRet is %d", iRet);
					pAudioBuf++;
					iAudioStreamLen++;
				}
			}
			else
			{
				//td_printf(0, "------------>iAudioStreamLen is %d", iAudioStreamLen);
				pAudioBuf++;
				iAudioStreamLen++;
			}
		}
	}
	else
	{
		for(i = 0; i < iAFrames; i++)
		{	
			iAudioRes = TdAO_SendStream(pStream->m_AO.m_iAoId,
							pStream->m_pBuf + pStream->m_FrameHeader.u32VStreamLen+ i*iAFrameSize,iAFrameSize,0);	
			if(AUDIO_SEND_BUF_FULL == iAudioRes)
			{
				break;
			}
		}	

		if(i != iAFrames)
		{
			if((iAFrames - i) * iAFrameSize > MAXBUF -MAXVSTREAM)
			{
				iRet = -1;
				goto leave;
			}
			if(pStream->m_iLeaveAudioFrames == 0)
			{
				pStream->m_iLeaveAudioFrames = iAFrames - i;
				pStream->m_iLeaveAudioLoaction = MAXVSTREAM;
				pStream->m_iLeaveAudioFramesLen = iAFrameSize;
				decode_copyAudioStream(_This, MAXVSTREAM, pStream->m_FrameHeader.u32VStreamLen+ i*iAFrameSize, (iAFrames - i) * iAFrameSize);
			}
		}
	}

leave:
	pthread_mutex_unlock(&pStream->m_StructLock);
	return iRet;		
}

static int decode_SendLeaveAudioFrames(TStream *_This)
{
	int iRet = 0;
	TStream *pStream = _This;
    	int iAFrameSize = 0;
	int iAFrames = 0;
	int i = 0;
	if(pStream == NULL)
	{
		return -100;
	}
	pthread_mutex_lock(&pStream->m_StructLock);
	if(pStream->m_fp == NULL || pStream->m_pTVDec == NULL ||pStream->m_pBuf == NULL)
	{
		iRet = -100;
		goto leave;
	}
	iAFrameSize = pStream->m_iLeaveAudioFramesLen;//视频帧的长度
	iAFrames = pStream->m_iLeaveAudioFrames;//视频帧数
	//创建音频通道
	if(pStream->m_AO.m_iAoId <= 0)
	{
		decode_clearLeaveAudioStream(_This);
		iRet = -1;
		goto leave;
	}	
	
	//发送音频帧
	int iAudioRes = 0;
	for(i = 0; i < iAFrames; i++)
	{	
		iAudioRes = TdAO_SendStream(pStream->m_AO.m_iAoId,
						pStream->m_pBuf + pStream->m_iLeaveAudioLoaction + i * iAFrameSize,iAFrameSize,0);
		if(AUDIO_SEND_BUF_FULL == iAudioRes)
		{
			break;
		}
	}	

	//if(i != iAFrames)
	{
		pStream->m_iLeaveAudioFrames = iAFrames - i;
		pStream->m_iLeaveAudioLoaction += i * iAFrameSize;
		pStream->m_iLeaveAudioFramesLen = iAFrameSize;
	}
	
leave:
	pthread_mutex_unlock(&pStream->m_StructLock);
	return iRet;			
}

static int decode_DestroyAo(TStream *_This)
{
	int iRet = 0;
	TStream *pStream = _This;
	pthread_mutex_lock(&pStream->m_StructLock);
	if(pStream->m_AO.m_iAoId >= 0)
	{
		TdAO_Stop(pStream->m_AO.m_iAoId);	
		pStream->m_AO.m_iAoId = -1;
	}
	pthread_mutex_unlock(&pStream->m_StructLock);
	return iRet;	
}
		
static int   decode_SetPassword(TStream *_This,char* _cPassword)
{
	int iPwdLen = 0;
	TStream *pStream = _This;
	if(pStream == NULL)
	{
		return -1;
	}
	iPwdLen = strlen(_cPassword);
	if(iPwdLen>ENCRYPT_PWD_LENGTH)
	{
		return -2;
	}
	memset(pStream->m_ucEncrypt,0,ENCRYPT_PWD_LENGTH+1);
	memcpy(pStream->m_ucEncrypt,_cPassword,iPwdLen);
	return 0;
}

static int decode_GetFileHeader(TStream *_This, S_FileHeader * _pFileHeader)
{
	TStream *pStream = _This;
	if(pStream == NULL || _pFileHeader == NULL)
	{
		return -1;
	}
	memcpy(_pFileHeader,&pStream->m_FileHeader,sizeof(S_FileHeader));
	return 0;
}
//获取文件正在打开的文件名_pcFileName为数组
static int decode_GetFileName(TStream *_This, td_u8 *_pcFileName)
{
	int iNameLen = 0;
	TStream *pStream = _This;
	if(pStream == NULL || _pcFileName == NULL)
	{
		return -1;
	}
	iNameLen = strlen(pStream->m_File);
	memcpy(_pcFileName,pStream->m_File,iNameLen);
	return 0;
}
//0:返回帧率_Value : unsigned short
// 1:返回图像的宽 _Value:unsigned short
// 2:返回图像的高 _Value:unsigned short
// 3.返回图像的编码模式 _Value:unsigned short
// 4.返回图像是否携带音频 _Value:unsigned short 
static int decode_GetFileAttribute(TStream *_This,void * _Value, int _iFlag)
{
	S_FileHeader FileHeader;
	TStream *pStream = _This;
	if(pStream == NULL)
	{
		return -1;
	}
	decode_GetFileHeader(_This,&FileHeader);
	//判断是否是合法的头
	if(IsValidFileHeader(&FileHeader) <= 0)
	{
		return -2;
	}
	switch(_iFlag)
	{
		case 0://返回帧率
			*((unsigned short *)_Value) = FileHeader.FrameRate;
			break;
		case 1://图像的宽
			*((unsigned short *)_Value) = FileHeader.Width;
			break;
		case 2://图像的高
			*((unsigned short *)_Value) = FileHeader.Height;
			break;
		case 3://图像的编码模式
			*((unsigned short *)_Value) = FileHeader.Mode;
			break;
		case 4://图像是否携带音频
			*((unsigned short *)_Value) = 1;//FileHeader.bAudio;
			break;
		case 5://实时图像的宽高
			if((decode_IFmFromIndex(pStream, pStream->m_iFrameLoc, 0, _Value) < 0) || (0 == (*(unsigned int *)_Value)))
			{
				*(unsigned int *)_Value = FileHeader.Height;
				*(unsigned int *)_Value <<= 16;
				*(unsigned int *)_Value += FileHeader.Width;
			}
			break;
		default :
			return -3;
	}
	return 0;
}

static unsigned int decode_GetFrameTm(TStream *_This)
{
	TStream *pStream = _This;
	S_FrameHeader *ptFrameHeader = NULL;
	if(pStream->m_iBackFlag)
	{
		ptFrameHeader = &pStream->m_tBackFrameHeader;
	}
	else
	{
		ptFrameHeader = &pStream->m_FrameHeader;
	}
	
	return ptFrameHeader->u32TimeStamp;
}

static int decode_ReleaseVStream(TStream *_This)
{
	TStream *pStream = _This;
	int iRet = 0;
	if(pStream == NULL)
	{
		return td_failure;
	}
	pthread_mutex_lock(&pStream->m_StructLock);
	if(pStream->m_fp == NULL || pStream->m_pTVDec == NULL ||pStream->m_pBuf == NULL)
	{
		iRet = td_failure;
		goto leave;
	}
	
	iRet = media_VdecClearStreamData(pStream->m_pTVDec->m_iVdecChan);
leave:
	pthread_mutex_unlock(&pStream->m_StructLock);
	return iRet;	
}
static int decode_wait_BackMemFull(TStream *_This)
{
	while(1)
	{
		if(0 == _This->m_tBackMem.BackMem_could_write(&_This->m_tBackMem))
		{
			break;
		}
		pthread_mutex_lock(&gBackMutexLock);
		pthread_cond_signal(&gBackCondEmptyLock);
		pthread_mutex_unlock(&gBackMutexLock);
		usleep(100);
	}
	return td_success;
}

static int decode_reset_BackMem(TStream *_This, int _iLoc)
{
	_This->m_iMemFrameLoc = _iLoc;
	pthread_mutex_lock(&gBackMutexLock);	
	_This->m_tBackMem.BackMem_clear(&_This->m_tBackMem);
	pthread_cond_signal(&gBackCondEmptyLock);
	pthread_mutex_unlock(&gBackMutexLock);	
	return td_success;
}


static int decode_SeekPos(TStream *_This,IN int _iPos, int *_pStopFlag)//文件内部搜索(百分比)
{
	int iRet = td_success;
	int iUnbindFlag = 0;
	unsigned int PreFrames= 0;
	int iRemainTms = 0;
	TStream *pStream = _This;
	int tempIFrameFlag;
	int cnt = 0;
	int playid = 0; 
	int iCntReleaseFrame = 0;

	if(pStream == NULL)
	{
		return PLAYBACK_READFILE_ERR;
	}
	if(pStream->m_iBackFlag)
	{
		decode_wait_BackMemFull(_This);
	}
	pthread_mutex_lock(&pStream->m_StructLock);

	if(((FILE_TYPE_MP4 != pStream->m_iFileType)&&(pStream->m_fp == NULL))
		||((FILE_TYPE_MP4 == pStream->m_iFileType)&&(pStream->m_stMP4Info.m_pMp4_fp == NULL))
		  || pStream->m_pTVDec == NULL ||pStream->m_pBuf == NULL)
	{
		iRet = PLAYBACK_READFILE_ERR;
		pthread_mutex_unlock(&pStream->m_StructLock);
		goto leave;
	}
	playid = pStream->m_id;
	tempIFrameFlag = pStream->m_iOnlyIFrame;
	pStream->m_iOnlyIFrame = 0;
	if(pStream->m_AO.m_iAoId > 0)	
	{	
		TdAO_Stop(pStream->m_AO.m_iAoId);		
		pStream->m_AO.m_iAoId = -1;				
	}
	pthread_mutex_unlock(&pStream->m_StructLock);
	if(FILE_TYPE_MP4 == pStream->m_iFileType)
	{
		if(MP4_SeekFile_V2(&(pStream->m_stMP4Info.m_stMP4ISampleIndexHead), 
							&(pStream->m_stMP4Info.m_stMP4ISampleIndexTail),
							pStream->m_stMP4Info.m_pIMP4SampleIndex, 
							_iPos, 
							&(pStream->m_stMP4Info.m_uiCurVideoSampleId), 
							&(pStream->m_stMP4Info.m_uiCurAudioSampleId), 
							&(pStream->m_stMP4Info.m_u32UseLessIdCount))< 0)
		{
			unsigned long long ullVideoTimeStamp = (_iPos - pStream->m_iFileStartTime)*90000;
			unsigned long long ullAudioTimeStamp = (_iPos - pStream->m_iFileStartTime)*(pStream->m_FileHeader.AudioSample);
			unsigned int VideoSampleId = 0; 
			unsigned int AudioSampleId = 0;
			
			VideoSampleId = MP4_GetSeekPosSampleId(pStream->m_stMP4Info.m_pMp4_fp,
				pStream->m_stMP4Info.m_u32VideoTrackId, 
				ullVideoTimeStamp, 1);

			if(VideoSampleId <= 0)
			{
				td_printf(0xff0000, "%s %s %d failed! VideoSampleId is %d", __FILE__, __FUNCTION__, __LINE__, VideoSampleId);
				pStream->m_stMP4Info.m_uiCurVideoSampleId = 1;
				pStream->m_stMP4Info.m_uiCurAudioSampleId = 1;
				iRet = td_failure;
			}

			if(0 != pStream->m_stMP4Info.m_u32AudioTrackId)
			{
				AudioSampleId = MP4_GetSeekPosSampleId(pStream->m_stMP4Info.m_pMp4_fp,
					pStream->m_stMP4Info.m_u32AudioTrackId, 
					ullAudioTimeStamp, 0);

				if(AudioSampleId <= 0)
				{
					td_printf(0xff0000, "%s %s %d failed! AudioSampleId is %d", __FILE__, __FUNCTION__, __LINE__, AudioSampleId);
					pStream->m_stMP4Info.m_uiCurVideoSampleId = 1;
					pStream->m_stMP4Info.m_uiCurAudioSampleId = 1;
					iRet = td_failure;
				}
			}
			
			td_printf(0, "---->SeekEx VideoSampleId is %d, AudioSampleId is %d", VideoSampleId, AudioSampleId);
			pStream->m_stMP4Info.m_uiCurVideoSampleId = VideoSampleId;
			pStream->m_stMP4Info.m_uiCurAudioSampleId = AudioSampleId;
		}
	}
	else
	{
		int iSeekRet = 100;
		if(pStream->m_iFindNoIndex == 1 || 
			ERR_REC_SUCCESSFUL != (iSeekRet = rec_SeekFile_Ex(pStream->m_fp, _iPos,&iRemainTms, _pStopFlag)))
		{
			int iPos = 0;
			int iFilePos = 0;
			int temp = pStream->m_iFileEndTime - pStream->m_iFileStartTime;
			//获取开始时间，获取结束时间
			if(temp <= 0)
			{
				temp = 1;
			}
			td_printf(0,"_iPos = %d, pStream->m_iFileEndTime = %d, pStream->m_iFileStartTime = %d, iSeekRet = %d",_iPos,pStream->m_iFileEndTime,pStream->m_iFileStartTime,iSeekRet);
			iPos = (double)(_iPos - pStream->m_iFileStartTime) * 1000.0 / temp;
			if(iPos < 0)
			{
				iPos = 0;
			}
			if( iPos > 1000)
			{
				iPos = 1000;
			}
			td_printf(0,"iPos = %d",iPos);
			//获取文件的长度，根据文件长度进行换算
			unsigned int uiFileLen = pStream->m_FileHeader.TotalSize;
			if(!pStream->m_FileHeader.TotalSize)
			{
				uiFileLen = pStream->m_iFileLen;
			}
			//20120913 王磊 add ,若为当前录像文件则重新给uiFileLen赋值
			int iChno = -1;
			iChno = rec_IsCurrentFile(pStream->m_fp);
			if(iChno != -1)
			{
				uiFileLen = rec_GetCurFileSize(iChno);
			}
			//add end
			
			iFilePos = (int)((double)uiFileLen/1000.0 * iPos);
			//获取当前点的前一个I帧
			iFilePos = (iFilePos < sizeof(S_FileHeader) ? sizeof(S_FileHeader) : iFilePos);
			iFilePos = (iFilePos > uiFileLen ? uiFileLen : iFilePos);
			fseek(pStream->m_fp, iFilePos - sizeof(S_FileHeader), SEEK_SET);

			iRet = rec_SeekIFrame_Ex(pStream->m_fp, _pStopFlag); 
			if(iRet < 0)
			{
				iRet = PLAYBACK_FILEFINISH_NOEFFECT;
				if(ftell(pStream->m_fp) < sizeof(S_FileHeader))
				{
					fseek(pStream->m_fp, sizeof(S_FileHeader), SEEK_SET);
				}
			}
			else
			{
				iRet = td_success;
			}
			
			pStream->m_FileLoction = ftell(pStream->m_fp);
			pStream->m_iOnlyIFrame = tempIFrameFlag;
			if(pStream->m_iBackFlag)
			{
				decode_reset_BackMem(_This, pStream->m_FileLoction);
			}
			return iRet;
			
		}
		td_printf(0,"iRemainTms = %d\n",iRemainTms);
		iRemainTms *= 1000;
		if(iRemainTms < 0)
		{
			iRemainTms = 0;
		}
		pStream->m_FileLoction = ftell(pStream->m_fp);
		
		while(iRemainTms > 0 && cnt++ < 200)
		{
			if(*_pStopFlag == 1)
			{
				break;
			}
			if((iRet = decode_readfile(_This)) < 0)
			{
				break;
			}
			
			if(PreFrames == 0)
			{
				PreFrames = pStream->m_FrameHeader.u32TimeStamp;
			}
			else
			{
				if(iUnbindFlag == 0)
				{
					pthread_mutex_lock(&pStream->m_StructLock);
					if(pStream->m_pTVDec && 0 == pStream->m_iBackFlag)
					{
						media_VdecUnBind(pStream->m_pTVDec->m_iVdecChan);
					}
					pthread_mutex_unlock(&pStream->m_StructLock);
					iUnbindFlag = 1;
				}
				if(pStream->m_FrameHeader.u32TimeStamp > PreFrames && iRemainTms > 0)
				{
					iRemainTms -= pStream->m_FrameHeader.u32TimeStamp - PreFrames;
				}
			}
			
			if(1 == decode_SendFileVStream(_This))//发送数据
			{
				if(iUnbindFlag == 1)
				{
					pthread_mutex_lock(&pStream->m_StructLock);
					if(pStream->m_pTVDec && 0 == pStream->m_iBackFlag)
					{
						media_VdecBind(pStream->m_pTVDec->m_iVdecChan);
					}
					pthread_mutex_unlock(&pStream->m_StructLock);
				}
				pStream->m_iOnlyIFrame = tempIFrameFlag;
				iRet = 1;
				goto leave;
			}
			if(iUnbindFlag == 1)
			{
				(td_failure == decode_ReleaseVStream(_This)) ? (iCntReleaseFrame++) : (iCntReleaseFrame = 0);//释放数据
			}
			PreFrames = pStream->m_FrameHeader.u32TimeStamp;
			if(iCntReleaseFrame == 4)
			{
				break;
			}
		}
		if(iUnbindFlag == 1)
		{
			//绑定			//zhangyi modify 20120222
			pthread_mutex_lock(&pStream->m_StructLock);
			if(pStream->m_pTVDec && 0 == pStream->m_iBackFlag)
			{
				media_VdecBind(pStream->m_pTVDec->m_iVdecChan);
			}
			pthread_mutex_unlock(&pStream->m_StructLock);
		}

		iRemainTms = 0;
		pStream->m_iOnlyIFrame = tempIFrameFlag;
	}
leave:
	if(pStream->m_iBackFlag)
	{
		decode_reset_BackMem(_This, pStream->m_FileLoction);
	}
	return iRet;

}


int decode_ChangeVoDisplayArea(int _iWidth,int _iHeight,TRect *_pRect,void  *_pVo)
{
	Td_VO *pTVo = _pVo;
	TRect Rect;
	float rate1,rate2;
	if(_pVo == NULL)
	{
		return -1;
	}
	if(((_iWidth==1280)&&(_iHeight==720))||((_iWidth==1920)&&(_iHeight==1080)))
	{
		rate1 = ((float)_iHeight)/((float)_iWidth);//图像的高/宽
		rate2 = ((float)_pRect->m_iHeight)/((float)_pRect->m_iWidth);//通道的高/宽
		if (rate1 < rate2)//横向显示完整等于通道宽带，纵向显示完整但纵向小于通道高度
		{			//横向不变，纵向变
			Rect.m_iLeft=_pRect->m_iLeft;
			Rect.m_iWidth = _pRect->m_iWidth;
			Rect.m_iHeight = Rect.m_iWidth * rate1;
			Rect.m_iTop =_pRect->m_iTop + (_pRect->m_iHeight - Rect.m_iHeight)/2;	
			pTVo->AdjustVoRect(pTVo,&Rect);	
		}
	}
#if 1
	else if(((_iWidth==720)&&(_iHeight==1280))||((_iWidth==1080)&&(_iHeight==1920)))
	{
		rate1 = ((float)_iWidth)/((float)_iHeight);//图像的高/宽
		rate2 = ((float)pTVo->m_VoRect.m_iWidth)/((float)pTVo->m_VoRect.m_iHeight);//通道的高/宽
		if (rate1 != rate2)//横向显示完整等于通道宽带，纵向显示完整但纵向小于通道高度
		{	
			//纵向不变，横向变	
			Rect.m_iHeight 	= pTVo->m_VoRect.m_iHeight;
			Rect.m_iWidth 	= Rect.m_iHeight * rate1;
			Rect.m_iTop 	= pTVo->m_VoRect.m_iTop; 
			Rect.m_iLeft	= pTVo->m_VoRect.m_iLeft + ((pTVo->m_VoRect.m_iWidth -  Rect.m_iWidth) >> 1);
			pTVo->AdjustVoRect(pTVo,&Rect); 
		}
	}
#endif
	else
	{
		pTVo->AdjustVoRect(pTVo,_pRect);
	}
	return td_success;	
}
static int decode_VoContrl(TStream *_This, int _iFlag)
{
	int iRet = 0;
	TStream *pStream = _This;
	pthread_mutex_lock(&pStream->m_StructLock);
	if(pStream->m_pVO == NULL)
	{
		iRet = -1;
		goto leave;
	}
	switch(_iFlag)
	{
		case VO_HIDE:
			pStream->m_iVoHide = 1;
			pStream->m_pVO->VoChnCtrl(pStream->m_pVO, VO_HIDE);
			break;
		case VO_CLEAR:
			pStream->m_pVO->VoChnCtrl(pStream->m_pVO, VO_CLEAR);
			break;
		case VO_SHOW:
			pStream->m_iVoHide = 0;
			pStream->m_pVO->VoChnCtrl(pStream->m_pVO, VO_SHOW);
			break;
		case -1:
			iRet = 0;
			break;
		default :
			;
	}
leave:
	pthread_mutex_unlock(&pStream->m_StructLock);
	return iRet;
}
	
static int decode_DestroyVo(TStream *_This)
{
	TStream *pStream = _This;
	
	pthread_mutex_lock(&pStream->m_StructLock);
	if(pStream->m_pVO == NULL)
	{
		goto leave;
	}
	pStream->m_pVO->VoChnCtrl(pStream->m_pVO, VO_CLEAR);
	pStream->m_pVO->VoChnCtrl(pStream->m_pVO, VO_HIDE);
	pStream->m_pVO->VoChnCtrl(pStream->m_pVO, VO_DISABLE);
	DestroyVO(pStream->m_pVO);
	pStream->m_pVO = NULL;
leave:
	pthread_mutex_unlock(&pStream->m_StructLock);
	return 0;
}
static int decode_DestroyVdec(TStream * _This)
{
	TStream *pStream = _This;
	
	pthread_mutex_lock(&pStream->m_StructLock);
	if(pStream->m_pTVDec == NULL)
	{
		goto leave;
	}
	if(pStream->m_pVO != NULL)
	{
		pStream->m_pVO->UnBind(pStream->m_pVO);
	}
	td_s32 s32LogChn = decode_GetChnFromFile(_This);
	pStream->m_pVO->AdjustVoRect(pStream->m_pVO, &pStream->m_pVO->m_VoRect);
	media_FreeVdecChannel(pStream->m_pTVDec, s32LogChn);
	pStream->m_pTVDec = NULL;
leave:
	pthread_mutex_unlock(&pStream->m_StructLock);
	return 0;
}

static int decode_ForceDestroy(void *_This)
{
	Decode *pDec = _This;
	pthread_mutex_lock(&pDec->m_Stream.m_StructLock);
	pDec->m_Stream.m_iForceDestroyFlag = 1;
	pthread_mutex_unlock(&pDec->m_Stream.m_StructLock);
	decode_DestroyVdec(&pDec->m_Stream);
	decode_DestroyVo(&pDec->m_Stream);	
	decode_DestroyAo(&pDec->m_Stream);
	return 0;
}

//////////////////////////////倒放/////////////////////////

int decode_destroy_back(TStream * _This, int _iCommonly)
{
	TStream *pStream = _This;
	int iBackFlag;
	if(!pStream)
	{
		return td_success;
	}

	iBackFlag = pStream->m_iBackFlag;

	pStream->m_iBackFlag = 0;

	if(0 != pStream->m_BackId)
	{
		pthread_mutex_lock(&gBackMutexLock);
		pthread_cond_signal(&gBackCondEmptyLock);
		pthread_mutex_unlock(&gBackMutexLock);
		while(pStream->m_BackId)
		{
			usleep(0);
		}
	}
	if(iBackFlag)
	{
		if(NULL != pStream->m_fp && (!DECODE_STOP(*pStream->m_piStop)))
		{
			decode_fromBackToPlay(pStream, _iCommonly);
		}
		
		pStream->m_tBackMem.BackMem_destroy(&pStream->m_tBackMem);
		if(1 == pStream->m_iSeparate)
		{
			pStream->m_iSeparate = 0;
			media_VdecBind(pStream->m_pTVDec->m_iVdecChan);
		}
		
		media_DestoryCoach(&pStream->m_tOrigData[0].m_pCoach);
		media_DestoryCoach(&pStream->m_tOrigData[1].m_pCoach);
	}
	return td_success;
}
static int decode_adjustMoveFramToMem(TStream * _This, int _iFramNo)
{
	int iRet = 0;
	TStream *pStream = _This;
	int iSigleLen = sizeof(int) * 8;
	int iSigleNo = _iFramNo / iSigleLen;
	int iBitNo = _iFramNo % iSigleLen;
	if(_iFramNo < 0 || _iFramNo >= sizeof(int) * 8 * PLAYBACK_BACK_BITNUM)
	{
		return iRet;
	}
	//td_printf(0,"pStream->m_u32MoveMemFrameBit[%d] = %x, _iFramNo = %d", iSigleNo, pStream->m_u32MoveMemFrameBit[iSigleNo], _iFramNo);
	return pStream->m_u32MoveMemFrameBit[iSigleNo] & (1 << iBitNo);
}

static int decode_GetVdecForDat(TStream * _This)
{
	int iRet = 0;
	int iVdecret = 0;

	_This->m_iBackFileLoc[1] = _This->m_iBackFileLoc[0];
	_This->m_iBackFileLoc[0] = _This->m_iFrameLoc;
	
	//读文件
	if((iRet = decode_readfile(_This)) < 0)
	{
		goto leave;
	}
	
	//送解码
	if(td_success != (iRet = decode_SendFileVStream(_This)))
	{
		goto leave;
	}

	#if 0
	if(2 == _This->m_iMoveMemFrameNo)
	{
		_This->m_iMemFrameLoc = _This->m_iFrameLoc;
	}
	#endif
	
	if(_This->m_iMoveMemFrameNo > 0)
	{
		_This->m_iMoveMemFrameNo--;
		//media_VdecGetStreamData(_This->m_pTVDec->m_iVdecChan, NULL, _This->m_pTVDec->m_TVdecPara.m_iWidth, _This->m_pTVDec->m_TVdecPara.m_iHeight);
		media_VdecGetStreamData(_This->m_pTVDec->m_iVdecChan, NULL);
	}
	else
	{	
		if(0 == _This->m_iMoveMemFrameNo)
		{
			_This->m_iMemFrameLoc = _This->m_iBackFileLoc[1];
		}
		
		_This->m_iMoveMemFrameNo--;
		_This->m_iMoveMemFrameNum--;
		
		//if(_This->m_u32MoveMemFrameBit & (1 << _This->m_iMoveMemFrameNum))
		if(decode_adjustMoveFramToMem(_This, _This->m_iMoveMemFrameNum))
		{
			//iVdecret =  media_VdecGetStreamData(_This->m_pTVDec->m_iVdecChan, (void *)_This->m_tOrigData[0].m_pCoach, _This->m_pTVDec->m_TVdecPara.m_iWidth, _This->m_pTVDec->m_TVdecPara.m_iHeight);
			iVdecret =  media_VdecGetStreamData(_This->m_pTVDec->m_iVdecChan, (void *)_This->m_tOrigData[0].m_pCoach);
			_This->m_tOrigData[0].m_iLen = _This->m_iOrigFrameLen;
			if(_This->m_iMoveMemFrameNum == 0)
			{
				if(0 == iVdecret)
				{
					_This->m_tBackMem.BackMem_write(&_This->m_tBackMem, &_This->m_tOrigData[0], &_This->m_FrameHeader, _This->m_iBackFileLoc[1], 1);
				}
				_This->m_iMoveMemFrameNo = 0;
				iRet = 2;//完成
				goto leave;
			}
			else
			{
				if(0 == iVdecret)
				{
					_This->m_tBackMem.BackMem_write(&_This->m_tBackMem,&_This->m_tOrigData[0], &_This->m_FrameHeader, _This->m_iBackFileLoc[1], 0);
				}
			}
		}
		else
		{
			media_VdecGetStreamData(_This->m_pTVDec->m_iVdecChan, NULL);
			//media_VdecGetStreamData(_This->m_pTVDec->m_iVdecChan, NULL, _This->m_pTVDec->m_TVdecPara.m_iWidth, _This->m_pTVDec->m_TVdecPara.m_iHeight);
		}
	}
leave:
	return iRet;
}

static void decode_average_frame(TStream * _This, int _iFrameNum, int _iLimitNum)
{
	int i = 0;
	int iBitNum = sizeof(int) * 8 * PLAYBACK_BACK_BITNUM;
	int iCnt = 0;

	if(_iFrameNum > iBitNum || _iLimitNum <= 0 || _iFrameNum == _iLimitNum)
	{
		for(i = 0; i < PLAYBACK_BACK_BITNUM; i++)
		{
			_This->m_u32MoveMemFrameBit[i] = 0xffffffff;
	}
		return ;
	}
	
	int temp1 = (_iFrameNum * 100)/ _iLimitNum;
	int j = temp1 / 100;
	int y = temp1 % 100;
	int n = 0;
	int m = 0;

	for(i = 0; i < PLAYBACK_BACK_BITNUM; i++)
	{
		_This->m_u32MoveMemFrameBit[i] = 0;
	}
	
	for(i = 0; i < _iFrameNum; i += (j + n))
	{
		_This->m_u32MoveMemFrameBit[i/(sizeof(int) * 8)] |= (1<<(i%(sizeof(int) * 8)));
		if(++iCnt == _iLimitNum)
		{
			break;
		}
		m += y;
		if(m > 99)
		{
			n = 1;
			m -= 100;
		}
		
	}
	return ;
}

static int decode_calFrameSum(TStream * _This, int _iStartLoc, int _iEndLoc, int *_piFrameSum, long long * _pLTimeSum, unsigned int *_iTmStamp)
{
	int iFlag = 0;
	int iRet = 0;
	int i = 0;
	long long LTimeSum = 0;
	int iFrameSum = 0;
	int iRealLocation = -1;
	unsigned int 	u32BefTimeStamp = 0;
	int iFrameHeadrLen = sizeof(S_FrameHeader);
	TStream *pStream = _This;
	S_FrameHeader FrameHeader;
	
	if(pStream->m_iIFrameFileLoc > 0)
	{
		iRealLocation = ftell(pStream->m_fp);
		fseek(pStream->m_fp, _iStartLoc, SEEK_SET);
	}
	for(i = 0; ;i++)
	{
		if(fread(&FrameHeader, 1, iFrameHeadrLen, pStream->m_fp) < iFrameHeadrLen)//读到文件最后或出错
		{
			iFlag = 1;
			break;
		}
		
		iRet = Is_InsertFrame(&FrameHeader);
		if(PLAYBACK_PLUGFRAME == iRet)
		{
			fseek(pStream->m_fp, FrameHeader.u32FrameSize - iFrameHeadrLen, SEEK_CUR);
			continue;
		}
		else if(iRet <= 0)
		{
			iFlag = 1;
			break;
		}
		
		fseek(pStream->m_fp, FrameHeader.u32FrameSize - iFrameHeadrLen, SEEK_CUR);
		
		if(0 == i)
		{
			_iTmStamp[0] = FrameHeader.u32TimeStamp;
			u32BefTimeStamp = FrameHeader.u32TimeStamp;
		}

		#if 0
		if(0 == FrameHeader.u8FrameType && 0 != i)//读到下一个I帧
		{
			break;
		}
		#endif
		if(FrameHeader.u32TimeStamp > u32BefTimeStamp)// && FrameHeader.u32TimeStamp < u32BefTimeStamp + 2000)
		{
			LTimeSum += FrameHeader.u32TimeStamp - u32BefTimeStamp;
		}
		u32BefTimeStamp = FrameHeader.u32TimeStamp;
		
		iFrameSum++;

		if(ftell(pStream->m_fp) >= _iEndLoc)//读到上一次开始写内存的位置
		{
			break;
		}

		#if 0
		if(pStream->m_iMoveMemFrameNoOrig == iFrameSum)//读到上次开始写内存的位置
		{
			break;
		}
		#endif
		
	}
	_iTmStamp[1] = FrameHeader.u32TimeStamp;
	if(iRealLocation >= 0)
	{
		fseek(pStream->m_fp, iRealLocation, SEEK_SET);
	}
	if(iFlag)
	{
		td_printf(0,"%s, %d, iFrameSum = %d",__func__,__LINE__, iFrameSum);
		iFrameSum -= 2;
	}
	*_piFrameSum = iFrameSum;
	*_pLTimeSum = LTimeSum;

	return (0 == iFrameSum) ? td_failure : td_success;

}

static int decode_calFrameWriteMem(TStream * _This,IN int *_piFrameSum,IN long long * _pLTimeSum, int _iRemain)
{
	TStream *pStream = _This;
	int iFrame;
	int iFrameSum = *_piFrameSum;
	long long LTimeSum = *_pLTimeSum;
	int iTmTmp = 0;
	
	if(_iRemain >= iFrameSum)
	{
		decode_average_frame(_This, iFrameSum, iFrameSum);
		pStream->m_iMoveMemFrameNum = iFrameSum;
		pStream->m_iMoveMemFrameNo = 2;
	}
	else
	{
		if(LTimeSum <= 500)
		{
			iFrame = iFrameSum;
		}
		else if(PLAYBACK_BACK_NOCLIP == pStream->m_iBackClip)
		{
			iFrame = iFrameSum;
			if(iFrame > _iRemain)
			{
				iFrame = _iRemain;
			}
		}
		else
		{
			if(pStream->m_iBackClip > 0)
			{
				iTmTmp = ((LTimeSum / pStream->m_iBackClip)+ 500)/ 1000;
			}
			else if(pStream->m_iBackClip < 0)
			{
				iTmTmp = ((LTimeSum * (-pStream->m_iBackClip))+ 500)/ 1000;
			}

			iFrame = iFrameSum / ((iTmTmp <= 1) ? 1 : iTmTmp);			
		}

		if(iFrame <= 0)//限制保证1S钟至少1帧
		{
			iFrame = 1;
		}
		else if(iFrame > sizeof(int) * 8 * PLAYBACK_BACK_BITNUM)
		{
			iFrame = sizeof(int) * 8 * PLAYBACK_BACK_BITNUM;
		}
		
		pStream->m_iMoveMemFrameNo = iFrameSum - iFrame + 2;
		pStream->m_iMoveMemFrameNum = iFrame;
		decode_average_frame(_This, iFrame, _iRemain);
		//td_printf(0,"pStream->m_u32MoveMemFrameBit[0] = %x, iFrame = %d , _iRemain = %d", pStream->m_u32MoveMemFrameBit[0], iFrame, _iRemain);
	}
	
	return td_success;

}

#if 1
static int decode_getFrame(TStream * _This, int _iLestFrame, int _iLestTime, int *_piLoc, int *_piFrameSum, long long * _pLTimeSum)
{
	int iRet = 0;
	int iIframeLoc = 0;
	int iFrameSum = 0;
	long long LTimeSum = 0;
	TStream *pStream = _This;
	int iLoc = pStream->m_iMemFrameLoc;
	int iLestFrame = _iLestFrame < 2 ? 2 : _iLestFrame;
	int iLestTime = _iLestTime < 1000 ? 1000 : _iLestTime;

	int iHistoryLoc = pStream->m_iMemFrameLoc;
	long long llHistoryTm = 0;
	int iHistorySum = 0;
	unsigned int u32TimeStamp[2];
	unsigned int u32TimeStamp1[2];
	int i = 0;
	for(;;)
	{
		#if 1
		if(DECODE_STOP(*pStream->m_piStop))
		{
			break;
		}
		#endif
		iIframeLoc = decode_IFmFromIndex(_This, iLoc, 1, NULL);
		if(iIframeLoc < 0)
		{	
			iRet = -1;
			break;
		}
		iLoc = iIframeLoc;
		if(iIframeLoc == pStream->m_iMemFrameLoc)
		{
			continue;
		}
		if(td_failure == decode_calFrameSum(_This, iIframeLoc, iHistoryLoc, &iFrameSum, &LTimeSum, u32TimeStamp))
		{
			iRet = -1;
			break;
		}
		
		if(0 != (i++))
		{
			LTimeSum += (u32TimeStamp1[0] - u32TimeStamp[1]);
			td_printf(0,"(u32TimeStamp[0] - u32TimeStamp1[1]) = %d", (u32TimeStamp1[0] - u32TimeStamp[1]));
	}

		iHistoryLoc = iIframeLoc;
		llHistoryTm += LTimeSum;
		iHistorySum += iFrameSum;
		if(((int)llHistoryTm >= iLestTime)|| iHistorySum >= iLestFrame)
		{
			break;
		}
		u32TimeStamp1[1] = u32TimeStamp[1];
		u32TimeStamp1[0] = u32TimeStamp[0];
	}

	*_piLoc = iLoc;
	*_piFrameSum = iHistorySum;
	*_pLTimeSum = llHistoryTm;
	
	if(iRet < 0 && iHistorySum < 2)
	{	
		iRet = PLAYBACK_FILEFINISH_NOEFFECT;
	}
	return iRet;
}
#endif
static int decode_SetFileLoc(TStream * _This, int _iLoc)
{
	TStream *pStream = _This;
	if(pStream->m_fp)
	{
		pStream->m_FileLoction = _iLoc;
		return td_success;
	}
	return td_failure;
}
static int decode_GetPutMemFrame(TStream * _This)
{
	int iRet = 0;
	TStream *pStream = _This;
	int iIframeLoc = 0;
	int iFrameSum = 0;
	long long LTimeSum = 0;
	
	int iNumTmp = 0;
	int iTmTmp = 0;
	if(pStream->m_iBackClip > 0)
	{
		iNumTmp = 25 * pStream->m_iBackClip;
		iTmTmp = 1000 * pStream->m_iBackClip;
	}
	else if(pStream->m_iBackClip < 0)
	{
		iNumTmp = 25 / (-pStream->m_iBackClip);
		iTmTmp = 1000 / (-pStream->m_iBackClip);
	}
	else
	{
		iNumTmp = 25;
		iTmTmp = 1000;
	}

	if(PLAYBACK_FILEFINISH_NOEFFECT == decode_getFrame(_This,  iNumTmp, iTmTmp, &iIframeLoc, &iFrameSum, &LTimeSum))
	{
		return PLAYBACK_FILEFINISH_NOEFFECT;
	}
	
	decode_calFrameWriteMem(_This, &iFrameSum, &LTimeSum,	
		pStream->m_tBackMem.BackMem_getSize(&pStream->m_tBackMem, pStream->m_iOrigFrameLen));

	decode_SetFileLoc(_This, iIframeLoc);
	return iRet;
}

int decode_ResetVdec(TStream * _This)
{
	int i = 0;
	int iRet = 0;
	TStream *pStream = _This;
	//如此调媒体层此部分可能存在线程同步问题
	#if 1
	media_VdecStopRecvStream(pStream->m_pTVDec->m_iVdecChan);	
	media_VdecReset(pStream->m_pTVDec->m_iVdecChan);
	media_VdecStartRecvStream(pStream->m_pTVDec->m_iVdecChan);	
	#endif
	for(i = 0; i < 2; i++)
	{
		#if 1
		if(DECODE_STOP(*pStream->m_piStop))
		{
			break;
		}
		#endif
		pStream->m_iBackFileLoc[1] = pStream->m_iBackFileLoc[0];
		pStream->m_iBackFileLoc[0] = pStream->m_iFrameLoc;
		
		if((iRet = decode_readfile(_This)) < 0)
		{
			goto leave;
		}
		//送解码
		if(td_success != (iRet = decode_SendFileVStream(_This)))
		{
			goto leave;
		}
		#if 0
		if(2 == pStream->m_iMoveMemFrameNo)
		{
			_This->m_iMemFrameLoc = _This->m_iFrameLoc;
		}
		#endif
		pStream->m_iMoveMemFrameNo--;
		
	}
leave:
	return iRet;
}

int decode_WriteMem(TStream * _This)
{
	int iRet = 0;
	TStream *pStream = _This;
	
	if(0 == pStream->m_tBackMem.BackMem_could_write(&pStream->m_tBackMem))
	{
		return PLAYBACK_MEM_FULL;
	}

	if(td_success != (iRet = decode_GetPutMemFrame(_This)))
	{
		goto leave;
	}
	if(td_success != (iRet = decode_ResetVdec(_This)))
	{
		goto leave;
	}
	for(;;)
	{
		#if 1
		if(DECODE_STOP(*pStream->m_piStop))
		{
			iRet = td_failure;
			break;
		}
		#endif
		iRet = decode_GetVdecForDat(_This);
		if(iRet < 0 || 1 == iRet)
		{
			goto leave;
		}
		else if(2 == iRet)
		{
			iRet = 0;
			break;
		}
	}

leave:
	if(1 == iRet)
	{
	}
	else if(td_success != iRet)
	{		
		iRet = PLAYBACK_FILEFINISH_NOEFFECT;
	}
	return iRet;
}
static int decode_ReadMem(TStream * _This)
{
	TStream *pStream = _This;

	pthread_mutex_lock(&gBackMutexLock);
	for(;(2 == pStream->m_tBackMem.BackMem_could_write(&pStream->m_tBackMem)) && (1 == pStream->m_iBackFlag);)
	{
		pthread_cond_wait( &pStream->m_iMemCondFullLock, &gBackMutexLock);
	}
	pthread_mutex_unlock(&gBackMutexLock);	
	if(td_failure == pStream->m_tBackMem.BackMem_read_nofree(&pStream->m_tBackMem, &pStream->m_tBackFrameHeader))
	{	
		return PLAYBACK_FILEFINISH_NOEFFECT;
	}
	return td_success;
}


static int decode_SendMemVStream(TStream * _This)
{
	int iRet = 0;
	TStream *pStream = _This;

	iRet = pStream->m_tBackMem.BackMem_read(&pStream->m_tBackMem, &_This->m_tOrigData[1]);
	pthread_mutex_lock(&gBackMutexLock);
	pthread_cond_signal(&gBackCondEmptyLock);	
	pthread_mutex_unlock(&gBackMutexLock);
	if(td_failure == iRet)
	{
		return td_success;
	}
	if(0 == pStream->m_iVoHide)
	{
		media_VpssSendStreamData(_This->m_pVO->m_iDispDev, _This->m_pVO->m_iVoChn, _This->m_tOrigData[1].m_pCoach);
	}
	pStream->m_iSendStreamLoc = iRet;
	//media_VpssSendStreamData(temp, _This->m_pVO->m_iDispDev, _This->m_pVO->m_iVoChn,576, 704, iRet);
	return td_success;
}

void *decoderThread(void *this);
int decode_creat_back()
{
	int iRet = td_success;
	pthread_mutex_lock(&gBackLinkLock);
	if(0 == gBack)
	{
		pthread_t ThreadId;
		if(0 != pthread_create(&ThreadId,NULL,(void *)decoderThread,(void *)0))
		{
			iRet = td_failure;
			goto leave;
		}
		pthread_detach(ThreadId);
		gBack = 1;
	}
leave:
	pthread_mutex_unlock(&gBackLinkLock);
	return iRet;
}

int decode_del_backLink(TStream *_This)
{
	int i = 0;
	if(NULL == _This)
	{
		return td_success;
	}
	pthread_mutex_lock(&gBackLinkLock);
	for(i = 0; i < MAX_BACK_NUM; i++)
	{
		if(_This == gStream[i])
		{
			gStream[i] = NULL;
			break;
		}
	}
	_This->m_BackId = 0;
	pthread_mutex_unlock(&gBackLinkLock);
	
	return td_success;
}

int decode_add_backLink(TStream *_This)
{
	int i = 0;
	int iRet = td_failure;
	if(NULL == _This)
	{
		return td_failure;
	}
	pthread_mutex_lock(&gBackLinkLock);
	for(i = 0; i < MAX_BACK_NUM; i++)
	{
		if(!gStream[i])
		{
			gStream[i] = _This;
			iRet = td_success;
			break;
		}
	}
	pthread_mutex_unlock(&gBackLinkLock);
	
	pthread_mutex_lock(&gBackMutexLock);
	pthread_cond_signal(&gBackCondEmptyLock);
	pthread_mutex_unlock(&gBackMutexLock);	
	
	if(td_failure == (iRet = decode_creat_back()))
	{
		decode_del_backLink(_This);
	}
	return iRet;
}

void *decoderThread(void *this)
{
	int i = 0;
	TStream *pStream = NULL;
	int iNow = 0;
	int iBackNum = 0;
	func_info();
	while(1)
	{
		iBackNum = 0;
		pthread_mutex_lock(&gBackMutexLock);
		for(;;)
		{
			for(i = 0; i < MAX_BACK_NUM; i++)
			{
				iNow++;
				iNow %= MAX_BACK_NUM;
				if(gStream[iNow])
				{	
					pStream = gStream[iNow];
					iBackNum = 1;
					if(0 == pStream->m_iBackFlag)
					{
						break;//退出倒放
					}
					else if(pStream->m_tBackMem.BackMem_could_write(&pStream->m_tBackMem))//可以写
					{
						break;
					}
				}
			}

			if(0 == iBackNum)
			{
				break;
			}
			if(MAX_BACK_NUM == i)
			{
				pthread_cond_wait(&gBackCondEmptyLock, &gBackMutexLock);
				continue;
			}
			break;
		}
		pthread_mutex_unlock(&gBackMutexLock);	

		if(0 == iBackNum)
		{
			break;
		}
		if(0 == pStream->m_iBackFlag)
		{
			decode_del_backLink(pStream);
			continue;
		}
		
		if(PLAYBACK_FILEFINISH_NOEFFECT == decode_WriteMem(pStream))
		{
			pStream->m_FileLoction = 0;
		}

		if(0 == pStream->m_FileLoction)
		{
			pStream->m_FrameHeader.u8FrameType = 0xff;
			pStream->m_tOrigData[0].m_iLen = pStream->m_iOrigFrameLen;
			pStream->m_tBackMem.BackMem_write(&pStream->m_tBackMem, &pStream->m_tOrigData[0],  &pStream->m_FrameHeader, 88, 1);
			pStream->m_tBackMem.BackMem_write(&pStream->m_tBackMem, &pStream->m_tOrigData[0],  &pStream->m_FrameHeader, 88, 1);
		}
		pthread_mutex_lock(&gBackMutexLock);
		pthread_cond_signal(&pStream->m_iMemCondFullLock);
		pthread_mutex_unlock(&gBackMutexLock);	
	}
	gBack = 0;
	return 0;
}

int decode_init_back(TStream * _This, int _iClip)
{
	int iRet = td_failure;
	TStream *pStream = _This;
	int iClip = pStream->m_iBackClip;//丢帧程序1:正常丢帧，2:2倍速丢帧，-2:慢放2倍速丢帧....
	int iOnlyIFrame;
	int iHeight;
	int iWith;
	
	pStream->m_iBackClip = _iClip;//(_iClip * 3) / 2;
	td_printf(0,"pStream->m_iBackClip = %d", pStream->m_iBackClip);
	if(pStream->m_iBackFlag)
	{
	//	if(1 == iClip && 0 == _iClip)
		if(iClip > pStream->m_iBackClip)
		{
			td_printf(0,"xxxxxxxxxxxpStream->m_iBackClip = %d", pStream->m_iBackClip);
			decode_wait_BackMemFull(_This);
			decode_reset_BackMem(_This, pStream->m_iSendStreamLoc);
			decode_wait_BackMemFull(_This);
		}
		return td_success;
	}

	if(0 == pStream->m_mollocLen)
	{
		goto leave;
	}
	
	if(td_success != decode_mem_Init(&pStream->m_tBackMem))
	{
		goto leave;
	}
	pStream->m_iSendStreamLoc = 0;
	pStream->m_iBackFlag = 1;	
	if(NULL != pStream->m_pTVDec)
	{
		pStream->m_iSeparate = 1;
		media_VdecUnBind(pStream->m_pTVDec->m_iVdecChan);
	}
	else
	{
		pStream->m_iSeparate = 0;
	}

	iHeight = pStream->m_FileHeader.Height <= 1089 ? 1089 : pStream->m_FileHeader.Height;
	iWith = pStream->m_FileHeader.Width <= 1920 ? 1920 : pStream->m_FileHeader.Width;
	if(NULL == (pStream->m_tOrigData[0].m_pCoach = media_initCoach(iWith, iHeight, (int *)&pStream->m_tOrigData[0].m_pData, (int *)&pStream->m_tOrigData[0].m_pDataHeader)))
	{
		goto leave;
	}

	if(NULL == (pStream->m_tOrigData[1].m_pCoach = media_initCoach(iWith, iHeight, (int *)&pStream->m_tOrigData[1].m_pData, (int *)&pStream->m_tOrigData[1].m_pDataHeader)))
	{
		media_DestoryCoach(&pStream->m_tOrigData[0].m_pCoach);
		goto leave;
	}

	pStream->m_iMoveMemFrameNum = 0;
	pStream->m_iMoveMemFrameNoOrig = 0xffff;
	pStream->m_iMemFrameLoc = pStream->m_iFrameLoc;
	iOnlyIFrame = pStream->m_iOnlyIFrame;
	pStream->m_iOnlyIFrame = 0;
	if(td_failure == decode_add_backLink(pStream))
	{
		pStream->m_iOnlyIFrame = iOnlyIFrame;
		pStream->m_BackId = 0;
		goto leave;
	}
	pStream->m_BackId = 1;
	iRet = td_success;
	
leave:
	if(td_failure == iRet)
	{
		decode_destroy_back(_This, 1);
	}
	return iRet;
}

static td_s32 decode_fromBackToPlay(TStream * _This, int _iCommonly)
{	
	int iRet = 0;
	int iIframeLoc = 0;
	int iFrameSum = 0;
	long long LTimeSum = 0;

	_This->m_iMemFrameLoc = _This->m_iSendStreamLoc;
	if(decode_getFrame(_This,  25, 1000, &iIframeLoc, &iFrameSum, &LTimeSum))
	{
		return td_failure;
	}
	decode_SetFileLoc(_This, iIframeLoc);
	if((iRet = decode_ResetVdec(_This)) < 0)
	{
		return td_failure;
	}
	//送解码
	for(;;)
	{
		#if 1
		if(DECODE_STOP(*_This->m_piStop))
		{
			break;
		}
		#endif
		if((iRet = decode_readfile(_This)) < 0)
		{
			break;
		}
		if(td_success != (iRet = decode_SendFileVStream(_This)))
		{
			break;
		}
		media_VdecGetStreamData(_This->m_pTVDec->m_iVdecChan, NULL);
		//media_VdecGetStreamData(_This->m_pTVDec->m_iVdecChan, NULL, _This->m_pTVDec->m_TVdecPara.m_iWidth, _This->m_pTVDec->m_TVdecPara.m_iHeight);

		if(_iCommonly)
		{
			if(_This->m_iSendStreamLoc < _This->m_iFrameLoc)
			{
				break;
			}
		}
		else
		{
			if(_This->m_iSendStreamLoc <= _This->m_iFrameLoc)
			{
				break;
			}
		}
	}
	return iRet;
}

/////////////////////////////////////////////////////////////
static int _decoder_capPicture(TStream * _This, int _iFlag);
int Decoder_capPicCallBack(int _iChn, int _iSize)
{
	int iReCapFlag = 0;
	int iRet = td_failure;
	TStream *pStream = (TStream *)_iChn;
	int iFileNameLen = strlen(pStream->m_File);
	int iChnLoc = strlen("7T20130202221234C23.sdv");
	char cChn = 0;

	if(NULL == pStream)
	{
		return td_failure;
	}
	
	pthread_mutex_lock(&pStream->m_StructLock);
	if(_iSize <= 0)
	{
		if(0 == pStream->m_iCapCnt)
		{
			iReCapFlag = 1;
			pStream->m_iCapCnt = 1;
		}
		goto leave;
	}

	if(strstr(pStream->m_File, "usb"))
	{
		cChn = '0';
	}
	else	if(iFileNameLen <= iChnLoc)
	{
		goto leave;
	}
	else
	{
		cChn = pStream->m_File[iFileNameLen-iChnLoc];
	}
	
	iRet =  rec_playCapPicture(pStream->m_iTime, cChn, gCapBuf, _iSize);
leave:
	pthread_mutex_unlock(&pStream->m_StructLock);
	if(iReCapFlag)
	{
		if(td_failure == (iRet = _decoder_capPicture(pStream, 0)))
		{
			Play_Set_CapErr(pStream->m_iOptFlag);
			gCaping = NULL;
		}
		return iRet;
	}
	(td_success == iRet) ? Play_Set_CapOk(pStream->m_iOptFlag):Play_Set_CapErr(pStream->m_iOptFlag);	
	gCaping = NULL;
	return iRet;
}

static int decoder_GetCapPicTimeInterval(void)//返回毫秒间隔
{
	long long lNowTime = 0;
	decode_GetSysTime(&lNowTime);
	if(0 == lHistoryTime)
	{
		lHistoryTime = lNowTime;
		return lNowTime / 1000;
	}
	return (lNowTime - lHistoryTime) / 1000;
}

static int decoder_SetCapPicTime(void)
{
	decode_GetSysTime(&lHistoryTime);
	return lHistoryTime;
}

static int _decoder_capPicture(TStream * _This, int _iFlag)
{
	int iRet = td_failure;
	strPicParam SnapMsg;
	Td_VO  *pVo = NULL;
	int iHeight = 0;
	int iWidth = 0;
	if(NULL == _This)
	{
		return td_failure;
	}
	pthread_mutex_lock(&_This->m_StructLock);
	if(NULL == gCapBuf)
	{
		gCapBuf = valloc(PLAY_CAP_BUF);
	}
	if((1 == _iFlag) && (decoder_GetCapPicTimeInterval() <= 1))
	{
		goto leave;
	}
	if(NULL == gCapBuf)
	{
		goto leave;
	}

	if(NULL == _This->m_pVO || NULL == _This->m_pTVDec)
	{
		goto leave;
	}
	decoder_SetCapPicTime();
	iHeight = _This->m_FileHeader.Height;
	iWidth = _This->m_FileHeader.Width;
	pVo = _This->m_pVO;

	memset(&SnapMsg,0,sizeof(SnapMsg));
	if(_iFlag)
	{
		SnapMsg.m_s32Quality = DEFAULT_PIC_QUALITY | (SNAP_HD << 16);
	}
	else
	{
		SnapMsg.m_s32Quality = DEFAULT_PIC_QUALITY | (SNAP_SD << 16);
	}
	SnapMsg.m_iVoDev= pVo->m_iDispDev;
	SnapMsg.m_iVoChan= pVo->m_iVoChn;
	SnapMsg.m_pChan = (void *)_This;
			
	SnapMsg.m_s32VideoSize 	= (iHeight << 16) | iWidth;
	SnapMsg.m_s32BuffLen = PLAY_CAP_BUF;
	SnapMsg.m_pu8Buffer = gCapBuf;
	SnapMsg.m_cbkSnapShot = Decoder_capPicCallBack;
	iRet = TdVo_Snapshot(pVo, &SnapMsg);
leave:
	pthread_mutex_unlock(&_This->m_StructLock);
	return iRet;	
}


static int decoder_capPicture(TStream * _This, int _iTime, int _iFlag)
{
	int iRet = td_failure;
	if(NULL == _This)
	{
		return td_failure;
	}
	if(NULL != gCaping)
	{
		return td_failure;
	}
	gCaping = _This;
	pthread_mutex_lock(&_This->m_StructLock);
	_This->m_iCapCnt = 0;
	_This->m_iTime = _iTime;
	pthread_mutex_unlock(&_This->m_StructLock);	
	iRet = _decoder_capPicture(_This, _iFlag);
	if(iRet < 0)
	{
		gCaping = NULL; 
	}
	return iRet;
}
/////////////////////////////////////////////////////////////
static int decode_Destroy(void *_This)
{
	int iFlag = 0;
	Decode *pDec = _This;
	TStream *pTStream = NULL;
	decode_DisableBuf();
	if(pDec == NULL)
	{
		return -1;
	}
	pTStream = (&pDec->m_Stream);
	
	while((NULL != pTStream) && (pTStream == gCaping))
	{
		usleep(1000);
	}
	td_printf(0,"%s , %d",__func__,__LINE__);
	if(pDec->m_Stream.m_pBuf != NULL)
	{
		iFlag = 1;
		free(pDec->m_Stream.m_pBuf);
		pDec->m_Stream.m_pBuf = NULL;
	}
	decode_destroy_back(&pDec->m_Stream, 1);
	decode_CloseFile(&pDec->m_Stream);
	decode_DestroyVdec(&pDec->m_Stream);
	decode_DestroyVo(&pDec->m_Stream);
	decode_DestroyAo(&pDec->m_Stream);
	if(pDec->m_Stream.m_pIFrameIndex != NULL)
	{
		free(pDec->m_Stream.m_pIFrameIndex);
		pDec->m_Stream.m_pIFrameIndex = NULL;
	}
	if(pDec->m_Stream.m_stMP4Info.m_pIMP4SampleIndex != NULL)
	{
		free(pDec->m_Stream.m_stMP4Info.m_pIMP4SampleIndex);
		pDec->m_Stream.m_stMP4Info.m_pIMP4SampleIndex = NULL;
	}
	pDec->m_Stream.m_iForceDestroyFlag = 1;
	if(iFlag)
	{
		pthread_mutex_destroy(&pDec->m_Stream.m_StructLock);
	}
	return 0;
}

int decode_getOptStat(TStream * _This, int *_piOptFlag)
{
	if(NULL == _This)
	{
		return td_failure;
	}
	pthread_mutex_unlock(&_This->m_StructLock);
	*_piOptFlag = _This->m_iOptFlag;
	Play_Clr_All(_This->m_iOptFlag);
	pthread_mutex_unlock(&_This->m_StructLock);
	return td_success;
}
int decode_Init(void *_This, int *_piStopFlag)
{
	Decode *pDec = _This;
	pDec->m_Stream.m_iForceDestroyFlag = 0;
	pDec->m_Stream.m_fp = NULL;
	pDec->m_Stream.m_pTVDec = NULL;
	pDec->m_Stream.m_AO.m_iAoId = -1;
	pDec->m_Stream.m_pVO = NULL;
	memset(&pDec->m_Stream.m_vdecPara,0,sizeof(TVdecPara));
	memset(pDec->m_Stream.m_File,0,FILENAMEMAXNUM);
	pDec->m_Stream.m_FileLoction = 0;
	pDec->m_Stream.m_iOnlyIFrame = 0;
	pDec->m_Stream.m_u16BefAudioFrames = MAXAUDIOFRAMES;
	pDec->m_Stream.m_u16ZeroAudioFrames = 0;
	pDec->m_Stream.m_pIFrameIndex = NULL;
	pDec->m_Stream.m_mollocLen = 0;
	pDec->m_Stream.m_IFrameLen = 0;
	pDec->m_Stream.m_IFrameLoaction = 0;
	pDec->m_Stream.m_iFindNoIndex = 0;

	pDec->m_Stream.m_iFileStartTime = 0;
	pDec->m_Stream.m_iFileEndTime = 0;

	pDec->m_Stream.m_iLeaveAudioFrames = 0;
	pDec->m_Stream.m_iLeaveAudioLoaction = 0;
	pDec->m_Stream.m_iFrameRate = 0;
	//倒放
	pDec->m_Stream.m_piStop = _piStopFlag;
	pDec->m_Stream.m_BackId = 0;
	pDec->m_Stream.m_iBackFlag = 0;
	pDec->m_Stream.m_iSeparate = 0;
	pDec->m_Stream.m_iVoHide = 0;
	pDec->m_Stream.m_iBackClip = PLAYBACK_BACK_NOCLIP;
	memset(pDec->m_Stream.m_tOrigData, 0, sizeof(pDec->m_Stream.m_tOrigData));
	//抓拍
	Play_Clr_All(pDec->m_Stream.m_iOptFlag);
	//
	memset(pDec->m_Stream.m_ucEncrypt,0,ENCRYPT_PWD_LENGTH + 1);
	memset(&pDec->m_Stream.m_FileHeader,0,sizeof(S_FileHeader));
	memset(&pDec->m_Stream.m_FrameHeader,0,sizeof(S_FrameHeader));
	pDec->m_Stream.m_tBackMem.m_data = NULL;
	decode_clearLeaveAudioStream(&pDec->m_Stream);
	
	//MP4--ZS
	pDec->m_Stream.m_iFileType = FILE_TYPE_SDV;//默认sdv文件
	pDec->m_Stream.m_stMP4Info.m_pMp4_fp = NULL;
	pDec->m_Stream.m_stMP4Info.m_u32Version = 0;
	pDec->m_Stream.m_stMP4Info.m_u32VideoTrackId = 0;
	pDec->m_Stream.m_stMP4Info.m_u32AudioTrackId = 0;
	pDec->m_Stream.m_stMP4Info.m_iAudioType = -1;
	memset(pDec->m_Stream.m_stMP4Info.m_cSps, 0, sizeof(pDec->m_Stream.m_stMP4Info.m_cSps));
	pDec->m_Stream.m_stMP4Info.m_iSpsSize = sizeof(pDec->m_Stream.m_stMP4Info.m_cSps);
	memset(pDec->m_Stream.m_stMP4Info.m_cPps, 0, sizeof(pDec->m_Stream.m_stMP4Info.m_cPps));
	pDec->m_Stream.m_stMP4Info.m_iPpsSize = sizeof(pDec->m_Stream.m_stMP4Info.m_cPps);
	pDec->m_Stream.m_stMP4Info.m_iEncryptFlag = 0;
	memset(&(pDec->m_Stream.m_stMP4Info.m_stMP4ISampleIndexHead), 0, sizeof(TMP4ISampleIndexHead));
	memset(&(pDec->m_Stream.m_stMP4Info.m_stMP4ISampleIndexTail), 0, sizeof(TMP4ISampleIndexTail));
	pDec->m_Stream.m_stMP4Info.m_uiCurVideoSampleId = 1;
	pDec->m_Stream.m_stMP4Info.m_uiCurAudioSampleId = 1;
	pDec->m_Stream.m_stMP4Info.m_u32UseLessIdCount = 0;
	pDec->m_Stream.m_stMP4Info.m_iIMP4SampleIndexmollocLen = 0;
	pDec->m_Stream.m_stMP4Info.m_pIMP4SampleIndex = NULL;
	
	pDec->Init = decode_Init;
	pDec->Destroy = decode_Destroy;
	pDec->ForceDestroy = decode_ForceDestroy;
	pDec->CreateVo = decode_CreateVo;
	pDec->CreateVdec = decode_CreateVdec;
	pDec->GetRate = decode_GetRate;
	pDec->SetRate = decode_SetRate;
	pDec->FullScr = decode_FullScr;
	pDec->FullScrExit = decode_FullScrExit;
	pDec->OpenFile = decode_OpenFile;
	pDec->CloseFile = decode_CloseFile;
	pDec->ReadFrame = decode_ReadFrame;
	pDec->SendVStream = decode_SendVStream;
	pDec->SendAStream = decode_SendAStream;
	pDec->SetPassword = decode_SetPassword;
	pDec->GetFileHeader = decode_GetFileHeader;
	pDec->GetFileName = decode_GetFileName;
	pDec->GetFileAttribute = decode_GetFileAttribute;
	pDec->SeekPos = decode_SeekPos;
	pDec->ReleaseVStream = decode_ReleaseVStream;
	pDec->DestroyAo = decode_DestroyAo;
	pDec->GetFrameTm = decode_GetFrameTm;
	pDec->TestEncrypt = decode_TestEncrypt;
	pDec->VoContrl = decode_VoContrl;
	pDec->DestroyVo = decode_DestroyVo;
	pDec->DestroyVdec = decode_DestroyVdec;
	pDec->SendLeaveAudioFrames = decode_SendLeaveAudioFrames;
	pDec->clearLeaveAudioStream =  decode_clearLeaveAudioStream;
	pDec->capPicture = decoder_capPicture;
	pDec->init_back = decode_init_back;
	pDec->destroy_back = decode_destroy_back;
	pDec->getOptStat = decode_getOptStat;
	pDec->m_Stream.m_pBuf = malloc(MAXBUF);
	if(pDec->m_Stream.m_pBuf == NULL)
	{
		return -1;
	}

	pthread_cond_init(&pDec->m_Stream.m_iMemCondFullLock, NULL);
	pthread_mutex_init(&pDec->m_Stream.m_StructLock,NULL);
	return td_success;
}


