#ifndef _FB_H_
#define _FB_H_

#ifndef u8
#define u8 unsigned char
#endif

#ifndef u32
#define u32 unsigned int
#endif

#ifndef bool
#define bool	int
#endif

#ifndef true
#define true	1
#endif

#ifndef false
#define false 0
#endif

typedef  unsigned int TFbRgb;	//fb RGB  0xAARGB
typedef struct
{
	int m_iFlg;
	u32 m_u32Width;
	u32 m_u32Height;
} TFbScreenMsg;


#define FB_VIRTUAL_SCREEN_WIDTH  352
#define FB_VIRTUAL_SCREEN_HEIGHT 288


// FB初始化,成功返回true，否则返回false
bool FbInit(int _iVoDevCnt, int _iVoDevs[], char *_iFbDevs[], TFbScreenMsg _tMsg[]);

// FB反初始化
void FbUninit();

// 在FB图层创建一个叠加图层
// xp, yp给定图层在屏幕上的起始坐标；
// width, height指定图层的实际尺寸(图层在屏幕上的大小)
// virwidth, virheight指定图层的虚拟大小
// 成功返回图层编号(>=0)，负数表创建失败
int FbCreateOverlay(int _iVoDev, int _iXp, int _iYp, u32 _u32Width, u32 _u32Height, u32 _u32VirWidth, u32 _u32VirHeight);

// 注销已创建的叠加图层,id为图层编号
void FbDestroyOverlay(int _iVoDev, int _iId);

// 在叠加图层上描点
// id指定图层编号
// xp, yp指定虚拟坐标
// color指定描点色
void FbOverlaySetPixel(int _iVoDev, int _iId, int _iXp, int _iYp, TFbRgb _tColor); 

// 清空指定图层
void FbOverlayClear(int _iVoDev, int _iId);

void FbUpdate(int _iVoDev);

void FbClear(int _iVoDev);

#endif

