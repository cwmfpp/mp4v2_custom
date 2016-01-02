#ifdef FOR_MP4SDK
#include <Windows.h>

void usleep(int _iMicroSecond)
{
	if (_iMicroSecond < 1000 && _iMicroSecond != 0)
	{
		_iMicroSecond = 1000;
	}
	Sleep(_iMicroSecond/1000);
}

#define __func__ __FUNCTION__
#else
#include <unistd.h>

#endif // FOR_MP4SDK
#include <pthread.h>
#include <malloc.h>
#include <string.h>

#include "ringbuf.h"
#include "libtiandy.h"

#define min(X,Y) (((X) < (Y)) ? (X) : (Y))

typedef struct
{
	TRingBuf m_tNoUse;
	pthread_mutex_t m_Mutex;
	td_s32 m_iType;
	td_s32 m_iBufSize;
	td_u32 m_iReadPos;
	td_u32 m_iWritePos;
	td_u8 m_cBuf[0];
} TRingBuffer;

static inline void RingBufClear(TRingBuffer *_ptBuf)
{
	_ptBuf->m_iReadPos = 0;
	_ptBuf->m_iWritePos = 0;	
}
// 创建一个环形buf
TRingBuf *ringbuf_Create(td_s32 _iBufSize, int _iType)
{
	TRingBuffer *ptBuf = (TRingBuffer*)malloc(sizeof(TRingBuffer) + _iBufSize);
	if(NULL == ptBuf)
	{
		return NULL;
	}

	memset(ptBuf, 0, sizeof(TRingBuffer));
	ptBuf->m_iBufSize = _iBufSize;
	ptBuf->m_iType    = _iType;
	RingBufClear(ptBuf);
	if(0 != pthread_mutex_init(&ptBuf->m_Mutex, NULL))
	{
		free(ptBuf);
		return NULL;
	}

	// for test
	#if 0
	ptBuf->m_iReadPos = 2147466720;
	ptBuf->m_iWritePos = 2147466720;
	#endif
	
	return (TRingBuf *)ptBuf;
}

// 销毁一个环形buf
void *ringbuf_Destroy(TRingBuf *_ptBuf)
{	
	TRingBuffer *ptBuf = (TRingBuffer *)_ptBuf;

	if (ptBuf)
	{
		pthread_mutex_destroy(&ptBuf->m_Mutex);
		free(ptBuf);
	}
	
	return NULL;
}

static inline td_s32 Pos(td_s32 _iPos, td_s32 _iBufSize)
{
	return _iPos % _iBufSize;
}

// 环形buf中现存可读数据
td_s32 ringbuf_DataSize(TRingBuf *_ptBuf)
{
	td_s32 nBytes = 0;
	
	if (_ptBuf)
	{
		TRingBuffer *ptBuf = (TRingBuffer *)_ptBuf;	
		
		
		pthread_mutex_lock(&ptBuf->m_Mutex);

		nBytes = ptBuf->m_iWritePos - ptBuf->m_iReadPos;
		
		pthread_mutex_unlock(&ptBuf->m_Mutex);
	}

	return nBytes;
}

// 环形buf现可写数据
td_s32 ringbuf_Capacity(TRingBuf *_ptBuf)
{
	td_s32 nBytes = 0;
	
	if (_ptBuf)
	{
		TRingBuffer *ptBuf = (TRingBuffer *)_ptBuf;
		td_s32 iSize = ptBuf->m_iBufSize;
		
		pthread_mutex_lock(&ptBuf->m_Mutex);
		nBytes = iSize - (ptBuf->m_iWritePos - ptBuf->m_iReadPos);
		pthread_mutex_unlock(&ptBuf->m_Mutex);
	}
	return nBytes;
}


// 写数据到环形buf
static void ringbuf_WriteData(td_s32 _iSize, const char *_ptSrc, TRingBuffer *_ptBuf)
{
	#if 1
	TRingBuffer *ptBuf = (TRingBuffer *)_ptBuf;
	td_s32 iSize = ptBuf->m_iBufSize;
	td_s32 iWpos = Pos(ptBuf->m_iWritePos, iSize);

	
	if (iWpos + _iSize <= iSize)
	{
		memcpy(ptBuf->m_cBuf + iWpos, _ptSrc, _iSize);
	}
	else
	{
		td_s32 nBytes = iSize - iWpos;
	
		memcpy(ptBuf->m_cBuf + iWpos, _ptSrc, nBytes);
		memcpy(ptBuf->m_cBuf, _ptSrc + nBytes, _iSize - nBytes);
	}

	ptBuf->m_iWritePos += _iSize;
	#else
	_ptBuf->m_iWritePos += _iSize;
	#endif
}

td_s32 ringbuf_Write(td_s32 _iSize, const void *_ptSrc, TRingBuf *_ptBuf)
{
	TRingBuffer *ptBuf = (TRingBuffer *)_ptBuf;
	const char *ptSrc = (const char *)_ptSrc;
	td_s32 iSize = 0;
	td_s32 iDataSize = 0;
	td_s32 iCap = 0;

	if (NULL == _ptSrc || NULL == _ptBuf)
	{
		return -1;
	}

	if (_iSize < 1)
	{
		return _iSize;
	}

	// do safe check 20110715 
	if (ptBuf->m_iBufSize < 1 || ptBuf->m_iReadPos < 0 || ptBuf->m_iWritePos < 0)
	{
		return -1;
	}
	
	pthread_mutex_lock(&ptBuf->m_Mutex);

	//td_printf(0, "ring buf %p write pos = %d, read pos = %d\n", _ptBuf, ptBuf->m_iWritePos, ptBuf->m_iReadPos);

	if(ptBuf->m_iWritePos <= ptBuf->m_iReadPos /*&& ptBuf->m_iWritePos != 2147466720*/)
	{
		RingBufClear(ptBuf);
	}
	else if (ptBuf->m_iWritePos > 0x1FFFFFFF) //解决读写指针无限制递增溢出变负 20110713 dxl
	{
		int iIndx1 = ptBuf->m_iWritePos / ptBuf->m_iBufSize;
		int iIndx2 = ptBuf->m_iReadPos / ptBuf->m_iBufSize;
		int iIndx = iIndx1 < iIndx2 ? iIndx1 : iIndx2;
		int iDec = iIndx * ptBuf->m_iBufSize;

		ptBuf->m_iWritePos -= iDec;
		ptBuf->m_iReadPos -= iDec;
	}

	iSize = ptBuf->m_iBufSize;
	iDataSize = ptBuf->m_iWritePos - ptBuf->m_iReadPos;
	iCap = iSize - iDataSize;

	if (_iSize <= iCap)
	{
		ringbuf_WriteData(_iSize, ptSrc, ptBuf);
	}
	else if (RINGBUF_WRITE_BLOCK == ptBuf->m_iType)
	{
		td_s32 iLeft = _iSize;
		while (iLeft > 0)
		{
			iCap = iSize - (ptBuf->m_iWritePos - ptBuf->m_iReadPos);

			if (iCap > 0)
			{
				// zty 20120331
				iCap = min(iLeft, iCap);
				
				ringbuf_WriteData(iCap, ptSrc, ptBuf);

				td_printf(0, "ringbuf_Write ringbuf_Write ringbuf_Write iCap err:%d" , iCap);
				
				// zty 20120308
				//ptBuf += iCap;
				ptSrc += iCap;
				iLeft -= iCap;
			}
			else
			{
				pthread_mutex_unlock(&ptBuf->m_Mutex); //解锁让对方去处理
				usleep(1000);
				pthread_mutex_lock(&ptBuf->m_Mutex);
			}
		}
	}
	else
	{
		if (_iSize < iSize)
		{
			ringbuf_WriteData(_iSize, ptSrc, ptBuf);
		}
		else
		{
			RingBufClear(ptBuf);
			ringbuf_WriteData(iSize, ptSrc + (_iSize - iSize), ptBuf);
		}

		ptBuf->m_iReadPos = ptBuf->m_iWritePos - iSize;
	}
	
	pthread_mutex_unlock(&ptBuf->m_Mutex);
	
	return iSize;
}

// 从环形buf读取数据
static void ringbuf_ReadData(td_s32 _iSize, char *_ptDst, TRingBuffer *_ptBuf)
{
	#if 1
	TRingBuffer *ptBuf = (TRingBuffer *)_ptBuf;
	td_s32 iSize = ptBuf->m_iBufSize;
	td_s32 iRpos = Pos(ptBuf->m_iReadPos, iSize);

	
	if (iRpos + _iSize <= iSize)
	{
		memcpy(_ptDst, ptBuf->m_cBuf + iRpos, _iSize);
	}
	else
	{
		td_s32 nBytes = iSize - iRpos;
	
		memcpy(_ptDst, ptBuf->m_cBuf + iRpos, nBytes);
		memcpy(_ptDst + nBytes, ptBuf->m_cBuf, _iSize - nBytes);
	}

	ptBuf->m_iReadPos += _iSize;
	#else
	_ptBuf->m_iReadPos += _iSize;
	#endif
}

td_s32 ringbuf_Read(td_s32 _iSize, void *_ptDst, TRingBuf *_ptBuf)
{
	TRingBuffer *ptBuf = (TRingBuffer *)_ptBuf;
	td_s32 nBytes = _iSize;
	td_s32 iDataSize = 0;

	if (NULL == _ptDst || NULL == _ptBuf)
	{
		return -1;
	}

	if (_iSize < 1)
	{
		return _iSize;
	}

	// do safe check 20110715 
	if (ptBuf->m_iBufSize < 1 || ptBuf->m_iReadPos < 0 || ptBuf->m_iWritePos < 0)
	{
		return -1;
	}	
	
	pthread_mutex_lock(&ptBuf->m_Mutex);
	
	if(ptBuf->m_iReadPos >= ptBuf->m_iWritePos)
	{
		nBytes = 0;
		//td_printf(0, "ringbuf_Read clear %d %d##########", ptBuf->m_iWritePos, ptBuf->m_iReadPos);
		RingBufClear(ptBuf);
		goto EXIT;
	}

	iDataSize = ptBuf->m_iWritePos - ptBuf->m_iReadPos;
	if (iDataSize < nBytes)
	{
		nBytes = iDataSize;
	}
	ringbuf_ReadData(nBytes, (char *)_ptDst, ptBuf);
	
	
EXIT:
	pthread_mutex_unlock(&ptBuf->m_Mutex);
	return nBytes;
}

td_s32 ringbuf_Clear(TRingBuf *_ptBuf)
{
	if (_ptBuf)
	{
		TRingBuffer *ptBuf = (TRingBuffer *)_ptBuf;

		pthread_mutex_lock(&ptBuf->m_Mutex);
		RingBufClear(ptBuf);
		pthread_mutex_unlock(&ptBuf->m_Mutex);
	}

	return td_success;
}

// zty 20120220
// 从环形buf复制数据-dlq created at 20111027
static void ringbuf_CopyData(td_s32 _iSize, char *_ptDst, TRingBuffer *_ptBuf)
{
	TRingBuffer *ptBuf = (TRingBuffer *)_ptBuf;
	td_s32 iSize = ptBuf->m_iBufSize;
	td_s32 iRpos = Pos(ptBuf->m_iReadPos, iSize);

	
	if (iRpos + _iSize <= iSize)
	{
		memcpy(_ptDst, ptBuf->m_cBuf + iRpos, _iSize);
	}
	else
	{
		td_s32 nBytes = iSize - iRpos;
	
		memcpy(_ptDst, ptBuf->m_cBuf + iRpos, nBytes);
		memcpy(_ptDst + nBytes, ptBuf->m_cBuf, _iSize - nBytes);
	}

	//ptBuf->m_iReadPos += _iSize;
}

// zty 20120220
td_s32 ringbuf_Copy(td_s32 _iSize, void *_ptDst, TRingBuf *_ptBuf)
{
	TRingBuffer *ptBuf = (TRingBuffer *)_ptBuf;
	td_s32 nBytes = _iSize;
	td_s32 iDataSize = 0;
	
	if (NULL == _ptDst || NULL == _ptBuf)
	{
		return -1;
	}

	if (_iSize < 1)
	{
		return _iSize;
	}

	// do safe check 20110715 
	if (ptBuf->m_iBufSize < 1 || ptBuf->m_iReadPos < 0 || ptBuf->m_iWritePos < 0)
	{
		return -1;
	}	
	
	pthread_mutex_lock(&ptBuf->m_Mutex);

	if(ptBuf->m_iReadPos >= ptBuf->m_iWritePos)
	{
		nBytes = 0;
		//td_printf(0, "ringbuf_Read clear %d %d##########", ptBuf->m_iWritePos, ptBuf->m_iReadPos);
		RingBufClear(ptBuf);
		goto EXIT;
	}

	iDataSize = ptBuf->m_iWritePos - ptBuf->m_iReadPos;
	if (iDataSize < nBytes)
	{
		nBytes = iDataSize;
	}
	
	ringbuf_CopyData(nBytes, (char *)_ptDst, ptBuf);

EXIT:	
	pthread_mutex_unlock(&ptBuf->m_Mutex);
	return nBytes;
}

// zty 20120331
td_s32 ringbuf_Remove(td_s32 _iSize, TRingBuf *_ptBuf)
{
	TRingBuffer *ptBuf = (TRingBuffer *)_ptBuf;
	//td_s32 iSize = ptBuf->m_iBufSize;
	//td_s32 iDataSize = 0;
	td_s32 nBytes = _iSize;
	int iRet = td_failure;

	if(NULL == _ptBuf)
	{
		return td_failure;
	}

	if(nBytes < 1)
	{
		return nBytes;
	}

	if (ptBuf->m_iBufSize < 1 || ptBuf->m_iReadPos < 0 || ptBuf->m_iWritePos < 0)
	{
		return td_failure;
	}

	pthread_mutex_lock(&ptBuf->m_Mutex);

	if(ptBuf->m_iReadPos >= ptBuf->m_iWritePos)
	{
		RingBufClear(ptBuf);
		iRet = td_failure;
		
		goto EXIT;
	}

	nBytes = min(nBytes, (ptBuf->m_iWritePos - ptBuf->m_iReadPos));
	
	ptBuf->m_iReadPos += nBytes;
	iRet = td_success;
	
EXIT:
	
	pthread_mutex_unlock(&ptBuf->m_Mutex);
	
	return iRet;
}
// zty add end

//获取ring buf中的起始数据地址 - dlq added at 20120919
td_u8 *ringbuf_GetData(TRingBuf *_ptBuf)
{
	TRingBuffer *ptBuf = (TRingBuffer *)_ptBuf;
	//td_s32 iSize = ptBuf->m_iBufSize;
	td_s32 iSize = 0;
	//td_s32 iRpos = Pos(ptBuf->m_iReadPos, iSize);
	td_s32 iRpos = 0;
	td_u8 *pcReadPos = NULL;

	pthread_mutex_lock(&ptBuf->m_Mutex);
	iSize = ptBuf->m_iBufSize;
	iRpos = Pos(ptBuf->m_iReadPos, iSize);
	pcReadPos = ptBuf->m_cBuf + iRpos;
	pthread_mutex_unlock(&ptBuf->m_Mutex);
	return pcReadPos;
}

