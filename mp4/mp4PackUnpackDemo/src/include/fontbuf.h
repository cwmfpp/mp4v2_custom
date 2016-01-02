#ifndef _FONTBUF_H_
#define _FONTBUF_H_
#include "libfont.h"
#include "public_def.h"
#include "hi_comm_video.h"

//陆翔云 add   

/*	矢量字库缓存设计,只针对通道时间
	需要缓存的字符包括:A-Z a-z 0~9  /  .  - :

	0-9    	32-41
	A-Z   	65-90
	a-z     97-122
*/
/*
	视频大小转换为数组下标
	QIF     0
	CIF 	1	
	HD1 	2
	D1		3
	960H	4	
	720P	5
	1080I	6
	1080P	7
*/


#define MAX_VIDEO_SIZE_NUM 	8 //shihc mod 20121224 增加1080I的支持
#define MAX_COLOR_NUM		16
#define MAX_CHAR_NUM		127


typedef struct 
{
	td_s32 			m_iVideoSize;	//视频大小	
	td_u32 		    m_uiColor;		//颜色
	td_char 		m_cChar;		//字符
	int 			m_iWidth;
	int 			m_iBytes;
	TFontBmp 		m_FontBmp;
}str_FontBuf;

typedef struct
{
	int 			m_iVideoSize;
	unsigned int 	m_uiColor;
	BITMAP_S 		m_Bitmap;	
	int 			m_iInit;
}STR_TimeBmpBuf;

BITMAP_S* GetTimeBmpBuf(td_s32 _iVideoSizeIndex,td_s32 _s32Color);

td_void SetTimeBmpBuf(td_s32 _iVideoSizeIndex,td_s32 _s32Color,BITMAP_S* _TimeBmp,td_s32 _sBitMapSize);
/*
	如果位图存在直接返回m_Bitmap
	如果不存在
		1、申请内存，生成位图
		2、将m_iInit置1
		3、返回位图指针
*/

td_s32 ClearTimeBmpBuf();
/*
	下一秒开始时，将所有位图缓存清空，只需将m_iInit置为0即可
*/

TFontBmp *GetFontBitsFromBuf(td_s32 _iSizeIndex,td_char _cChar,td_u32 _s32Color,int *_iWidth,int *_iBytes);

/*
	如果缓存中已经有该字符的点阵信息，返回点阵的指针
	否则返回空
*/

td_void SetFontBitsFromBuf(td_s32 _iSizeIndex,char _cChar,td_u32 _s32Color,TFontBmp* _tFontBmp,td_s32 _iBitLen,int _iWidth,int _iBytes);

/*
	将点阵信息保存在缓存中	
*/
td_void initFontBuf();
/*
	初始化FontBuf
*/

td_s32  initBmpBuf();


#endif

