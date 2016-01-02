#ifndef _RING_BUF_H_
#define _RING_BUF_H_

#include "public_def.h"

typedef struct
{
	void *m_ptSelftData;	//应用层数据
} TRingBuf;

enum
{
	RINGBUF_WRITE_BLOCK = 0,	//空间不够时写返回
	RINGBUF_WRITE_COVER = 1,	//空间不够时写覆盖
};

// 创建一个环形buf
TRingBuf *ringbuf_Create(td_s32 _iBufSize, int _iType);

// 销毁一个环形buf
void *ringbuf_Destroy(TRingBuf *_ptBuf);

// 环形buf中现存可读数据
td_s32 ringbuf_DataSize(TRingBuf *_ptBuf);

// 环形buf现可写数据
td_s32 ringbuf_Capacity(TRingBuf *_ptBuf);

// 写数据到环形buf
td_s32 ringbuf_Write(td_s32 _iSize, const void *_ptSrc, TRingBuf *_ptBuf);

// 从环形buf读取数据
td_s32 ringbuf_Read(td_s32 _iSize, void *_ptDst, TRingBuf *_ptBuf);

td_s32 ringbuf_Clear(TRingBuf *_ptBuf);

// zty 20120220
//复制环形buf数据,不删除
td_s32 ringbuf_Copy(td_s32 _iSize, void *_ptDst, TRingBuf *_ptBuf);

//删除环形buf数据
td_s32 ringbuf_Remove(td_s32 _iSize, TRingBuf *_ptBuf);
// zty add end

//获取ring buf中的起始数据地址 - dlq added at 20120919
td_u8 *ringbuf_GetData(TRingBuf *_ptBuf);
#endif

