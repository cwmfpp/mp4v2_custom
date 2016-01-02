//*********************************************
// 数据分发中心
// duxiulong 2011/03/01 create
//
// 说明:
// 1 使用库前需要调用datacneter_PoolInit()函数
// 2 每个数据源对应一个TDataCenter
// 3 每个需要对数据进行处理的业务对应一个DataFIFO
// 4 业务需要处理某个源的数据，需将其DataFIFO绑定到对应的DataCenter
// 5 源每产生一包数据，通过
//*********************************************
#ifndef _LIB_DATA_CENTER_H_
#define _LIB_DATA_CENTER_H_

#include "public_def.h"

#define TDataCenter 	void
#define TDataFIFO 	void

#ifndef byte
#define byte unsigned char
#endif

typedef struct
{
	int m_iDataSize;			// 包实际数据长度
	int m_iType;				// 数据类型
	byte m_bData[0];			// 包数据
} TDataPackage;

enum
{
	DC_FIFO_PRI_CYC = 1,	//缓存环，缓存溢出时循环覆盖旧数据
	DC_FIFO_PRI_COM = 2,	//普通FIFO，缓存溢出时丢弃新数据
	DC_FIFO_PRI_SEQ = 3,	//序列化FIFO，无数据丢失，缓存溢出时阻塞其它操作
	DC_FIFO_PRI_SEQ_NB = 4,
	DC_FIFO_PRI_IF	= 5, //
};

// 数据缓存池初始化
td_s32 datacenter_PoolInit(int _iMaxSize);
td_s32 datacenter_PoolUnint();

// 创建一块内存
TDataPackage *datacenter_CreatePackage(td_s32 _iSize);

// 添加内存引用记数 
void datacenter_PackageRefer(TDataPackage *_ptPkg);
// 减少内存引用记数
void datacenter_PackageRelease(TDataPackage *_ptPkg);

// 创建一个FIFO队列
TDataFIFO *datacenter_MakeFifo(td_s32 _iPri, td_s32 _iSize);
// 销毁一个队列
void datacenter_DestroyFifo(TDataFIFO *_ptFifo);

void datacenter_ChgFifoPri(td_s32 _iPri, TDataFIFO *_ptFifo);


// 从队列获取一包数据
TDataPackage *datacenter_GetPackFromFifo(TDataFIFO *_ptFifo);
// 清空队列数据
void datacenter_ClearFifo(TDataFIFO *_ptFifo);

// 创建一个数据分发中心
TDataCenter *datacenter_Create();
// 销毁一个数据分发中心
void datacenter_Destroy(TDataCenter *_ptDc);
// 绑定fifo到数据分发中心
td_s32 datacenter_BindFifo(TDataFIFO *_ptFifo, TDataCenter *_ptDc);
// 解绑定fifo到数据分发中心
td_s32 datacenter_UnBindFifo(TDataFIFO *_ptFifo, TDataCenter *_ptDc);
// 
td_s32 datacenter_HasNoFifoBind(TDataCenter *_ptDc);

// 将数据包交与数据分发中心分发
td_s32 datacenter_PushPackage(TDataPackage *_ptPkg, TDataCenter *_ptDc);

void datacenter_DebugMsg(char *_strMsg);

//int datacenter_RtMsgGetDebugInfo(char *_strMsg, int _iLen)


td_s32 datacenter_GetFifoPackNum(TDataFIFO *_ptFifo);

#endif

