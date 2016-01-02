#ifndef _TD_VO_H
#define _TD_VO_H

#include "public_def.h"
#include "td_media.h"

typedef enum
{
	VO_BIND_VI,		//绑定VI通道
	VO_BIND_VDEC,	//绑定解码通道
	VO_BIND_PCI,	//绑定PCI通道
	VO_BIND_BT1120,
	VO_BIND_BUTT,
}TVOBindType;

typedef struct Td_VO
{
	td_s32 		m_iDispDev;		//VO所在的显示设备编号
	td_s32		m_iVoChn;		//VO通道号
	td_s32		m_iEzoom;		//当前是否处在电子放大状态
	TRect		m_VoRect;		//VO在显示设备上的矩形区域
	TRect		m_DspArea;		//
	TVOBindType 	m_iBindType;		//VO绑定的类型
	td_s32 		m_iBindChn;		//绑定的通道号
	td_s16		m_u16Clear;
	td_s16		m_iIsOnShow;
	pthread_mutex_t m_tLock;	// dxl 20120517
	
	td_s32  (*Bind)(void* _pThis,TVOBindType _iType,td_s32 _iChn,TRect* _stZoom);						//绑定一个视频输入通道
	/*
		功能:	对VO进行绑定操作		
		
		参数:	_iType		绑定类型
				_iChn		绑定通道号
				_stZoom		需进行放大操作的矩形区域，
							如果该指针为空则不需要进行电子放大操作
				
		返回:	SUCCESS		成功
				FAILD		失败
	*/
	
	td_s32  (*UnBind)(void* _pThis);							
	/*
		功能:	解绑定
		
		参数:	无
				
		返回:	SUCCESS		成功
				FAILD		失败
	*/
	
	td_s32  (*AdjustArea)(void* _pThis,TRect* _stZoomRect);
	/*
		功能:	对电子放大区域进行调整
		
		参数:	_stZoomRect	矩形区域
				
		返回:	SUCCESS		成功
				FAILD		失败
	*/

	td_s32 (*AdjustVoRect)(void* _pThis,TRect* _stZoomRect);
	/*
		功能:	调整VO显示区域
		
		参数:	_stZoomRect	矩形区域
				
		返回:	SUCCESS		成功
				FAILD		失败
	*/
	
	
	td_s32  (*SetVoFrameRate)(void* _pThis,td_s32 _iFrameRate);
	/*
		功能:	设置VO输出帧率
		
		参数:	_iFrameRate	帧率
				
		返回:	SUCCESS		成功
				FAILD		失败
	*/

	td_s32  (*VoChnCtrl)(void* _pThis,VO_CTRL_E _eCtrl);
	/*
		功能:	vo通道显示控制
		参数:	_s32VoDev	vo设备号
				_s32VoChn	vo通道号
				_eCtrl		显示控制码
							0 使能
							1 禁止
							2 暂停
							3 恢复
							4 步进
							5 隐藏
							6 显示
							7 清空		
	*/

	td_s32 (*ShowNoVideo)(void* _pThis);
	/*
		功能:	显示No Video 提供给数字通道使用
	*/

	td_s32 (*SetVoChnField)(void* _pThis, td_s32 _s32Field);	
	

	td_s32 (*CopyFrame)(void* _pFrom, void* _pTo);

	
}Td_VO;

td_s32 TdVO_Init();

td_s32 TdVO_Uninit();

td_s32 TdVO_SetVODiff(td_s32  _s32VoDev,  td_s32 _s32Diff, td_s32 _s32VoDevIntfType, td_s32 _s32WbcBindVoDev);
/*
	功能:设置视频输出分辨率
	参数:	_s32VoDev	VO设备编号
			_s32Diff		VO分辨率
*/
td_s32 TdVO_SetVodevDspArea(td_s32 _s32VoDev, td_s32 _x, td_s32 _y, td_s32 _w, td_s32 _h);
td_s32 TdVO_SetVodevDspAreaEx(td_s32 _s32VoDev, td_s32 _s32VoChn,TRect * _ptRect);//ljj add 20130827
td_s32 TdVO_GetVODiff(td_s32  _s32VoDev,int* _iVideoWidth,int* _iVideoHeight);

Td_VO* CreateVO(int _iDispDev, TRect* _stVO);
Td_VO* CreateVO_Ex(int _iDispDev, TRect* _stVO, int _iLevel);	//zhangyi modify 20120207
/*
	功能:	创建一个VO实例
			需进行以下操作
			1、为VO实例分配内存
			2、进行初始化操作，为实例中的成员变量赋初值
			
	参数:  无

	返回:	成功	返回VO实例的指针
			失败	返回空指针
*/

td_s32 DestroyVO(Td_VO* _tdVO);
/*
	功能:	销毁一个VO实例
		 	1、销毁VO通道
		 	2、释放VO实例占用的内存
*/

td_s32 TdVO_DisableVoDev(td_s32 _s32VoDev);
/*
禁用VO显示设备
*/

td_s32 TdVO_SetVoDevImgSize(td_s32 _s32VoDev, td_s32 _iWidth, td_s32 _iHeight);

td_s32 TdVO_GetVoDevImgSize(td_s32 _s32VoDev, td_s32* _iWidth, td_s32* _iHeight);

td_s32 TdVo_Snapshot(Td_VO *_ptVo, strPicParam *_ptSnapshotParam);

#define TdVo_SetVoClearStat(_ptVo, _iClear)	((NULL != _ptVo) && (_ptVo->m_u16Clear = _iClear))
#define TdVo_GetVoClearStat(_ptVo, _piClear)	((*_piClear = 0, NULL != _ptVo) && (*_piClear = _ptVo->m_u16Clear))

#endif
