#ifndef _LIB_DEC_H_
#define _LIB_DEC_H_

#include "td_types.h"
#include "td_errno.h"
#include "td_media.h"

typedef struct
{
    int 	m_iWidth;				//视频宽度
    int 	m_iHeight;				//视频高度
}VideoDimSize;	//视频尺寸

typedef struct
{
	td_s32	m_iType; 				//解码类型	
	td_s32	m_iMod; 				//帧模式  流模式	
	td_s32 	m_iHeight;				//视频高度	
	td_s32	m_iWidth;				//视频宽度
}TVdecPara;


typedef struct
{
	td_s32 	m_iVdecChan;													//解码通道号		
	
	TVdecPara  m_TVdecPara;				
	
	td_s32 	m_iIsUse;														//是否占用
	
	td_s32 	m_iStat;															//是否创建
	
	td_s32 	m_iVsize;															//视频大小
	
	td_s32 (* Reset)(void * _pThis);											//查询状态

	td_s32 (* QueryStat)(void * _pThis);											//查询状态
	
	td_s32 (*SendStream)(void * _pThis,td_u8 *_cFrameBuf,td_s32 _iFlen, td_u32 _uiPts, td_s32 _iBlockFlag);	//送码流
	
}TVdecChannel; 



typedef struct
{
	td_s32 	m_iAdecChan;		// 解码通道
	
	td_s32	m_iType;			// 编码类型	
	
	td_s32	m_iIsUse;			// 是否在使用
	
	td_s32	m_iStat;			// 是否已经创建
	
	td_s32 (*SendStream)(void * _pThis,td_u8 *_cFrameBuf,td_s32 _iFlen,td_s32 _iBlockFlag);	//送码流
	
	td_s32 (*ClearChnBuf)(void * _pThis);	//清除缓冲

}TAdecChannel; 


td_s32 tvdec_Init(td_s32 _iNum, int _iMaxDecAbilty);

td_s32 tadec_Init(td_s32 _iNum);

//_iType=0 commonvdec ; _iType=1 vcavdec
TVdecChannel * media_CreateVdecChannel(TVdecPara *_pTVdecPara, td_s32 _iType, td_s32 _s32LogChn);

//td_s32 media_DestroyVdecChannel(TVdecChannel *_pTVdec);

td_s32 media_FreeVdecChannel(TVdecChannel *_pTVdec,  td_s32 _s32LogChn);

TAdecChannel * media_CreateAdecChannel(td_s32 _iType);

td_s32 media_FreeAdecChannel(TAdecChannel *_pTAdec);

td_s32 media_ClearAdecChannel(TAdecChannel *_pTAdec);


td_s32 libdec_GetDebug(char *_cString,int _iLen);

td_s32 media_SetDecMaxSize(td_s32 _iMaxDecSize);

td_s32 media_FreeAllNoUseVdecChannel(void);

td_s32 media_ClearAdecChannel(TAdecChannel *_pTAdec);

#endif

