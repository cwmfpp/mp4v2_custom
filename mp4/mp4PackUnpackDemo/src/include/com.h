#ifndef _COM_H_
#define _COM_H_

#include "td_types.h"

typedef void (*ComDataProcCallback)(td_s32 _iComId, td_s32 _iDataLen, td_u8 *_pcData);

// 初始化串口模块
// 参数说明:
// 	_iComCnt : 串口数
//	_strPaths[] : 各串口路径数组 /dev/xxx 为正真的串口, /pci/x/y表从片x的第y个串口
// ComDataProcCallback : 串口接收数据回调
td_s32 com_Init(td_s32 _iComCnt, td_u8 *_strPaths[], TComAttribute _ptAttr[], td_s32 _iRead[], ComDataProcCallback _CallBack);

// 串口模块去初始化函数
td_s32  com_Uninit();

// 获取串口实例
TComInstance *com_GetInstance(td_s32 _iComdId);
//获取空串口实例
TComInstance *com_GetNullInstance();

td_s32 com_SetBaudRateOfCPLD(td_s32 _iUardId, td_s32 _iSpeed);

//add by zhuangzhibing 20120218
//初始化pl2303驱动的usb串口
td_s32 comInitusbserial(int _iRead,int _iComNum,td_u8 *_strPaths, TComAttribute *_ptAttr);
//反初始化串口
td_s32 comUinitusbserial(int _iComNum);
//设置串口实例
td_s32 com_SetInstance(td_s32 _iComdId,TComInstance *_ptCom);
//add end

#endif

