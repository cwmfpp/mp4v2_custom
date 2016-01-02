#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "public_def.h"
#include "libtiandy.h"
#include "decode.h"

#include "td_media.h"

//内存管理
#ifndef td_failure
#define td_failure (-1)
#endif

#ifndef td_success
#define td_success (0)
#endif

#define DECODE_MAX_MEM_BLK	8
#define DECODE_MAX_MEM	(8 * 1024 *1024)

//#define DECODE_MAX_MEM	(3 * 1024 *1024)

#define DECODE_RM_ALLDATA		(0)
#define DECODE_RM_hONEFRAME	(1)
#define DECODE_RM_SIZE			(2)
#define DECODE_OUT_ONEFRAMR	(3)

#define DECODE_DATA_HEAD_LEN	(sizeof(TDecLinkHead))

static int g_iVedioFrmInfoSLen = 0;

typedef struct
{
	int m_blkSize;
	int m_blkCUseLocStart;
	int m_blkCUseSize;
	char dat[0];
}_TDecMemBlk;

typedef struct
{
	_TDecMemBlk *m_blk[DECODE_MAX_MEM_BLK];
	int m_blkNum;
	int m_MemSize;
	int m_MemCUseSize;
	int m_CUseBlkStart;
}TDecMemBlk;


typedef struct
{
	void *m_ptr;
	int m_iLen;
	int m_iBlkNo;
}TGetAddr;

enum
{
	HasUseMemSize = 1,
	CouldUseMemSize,
	HasUseBlkEnd,
	CouldUseBlkStart,
	MemSize,
	blkNum,
};


/////////////////////块begin/////////////////////////////
static int _decode_alloc_memblk(_TDecMemBlk **_blk,int _iSize)
{
	*_blk = malloc(sizeof(_TDecMemBlk) + _iSize);
	if(*_blk)
	{
		(*_blk)->m_blkSize = _iSize;
		(*_blk)->m_blkCUseLocStart = 0;
		(*_blk)->m_blkCUseSize = _iSize;
		return td_success;
	}
	return td_failure;
}

static int _decode_free_memblk(_TDecMemBlk **_blk)
{
	if(_blk && *_blk)
	{
		free(*_blk);
		*_blk = NULL;
	}
	return td_success;
}

int decode_destroy_memblk(TDecMemBlk *_TdecMem)
{
	int i = 0;
	for(i = 0; i < _TdecMem->m_blkNum; i++)
	{
		_decode_free_memblk(&_TdecMem->m_blk[i]);
	}
	_TdecMem->m_blkNum = 0;
	_TdecMem->m_MemSize = 0;
	_TdecMem->m_MemCUseSize = 0;
	_TdecMem->m_CUseBlkStart = 0;
	return td_success;
}

int decode_init_memblk(TDecMemBlk *_TdecMem)
{
	int i = 0;
	#if 1
	if(td_success == _decode_alloc_memblk(&_TdecMem->m_blk[0], DECODE_MAX_MEM))
	{
		_TdecMem->m_blkNum = 1;
		_TdecMem->m_MemSize = DECODE_MAX_MEM;
		_TdecMem->m_MemCUseSize = DECODE_MAX_MEM;
		_TdecMem->m_CUseBlkStart = 0;
	}
	else
	#endif
	{
		for(i = 0; i < DECODE_MAX_MEM_BLK; i++)
		{
			if(td_success != _decode_alloc_memblk(&_TdecMem->m_blk[i], DECODE_MAX_MEM/DECODE_MAX_MEM_BLK))
			{
				goto leave;
			}
		}
		_TdecMem->m_blkNum = DECODE_MAX_MEM_BLK;
		_TdecMem->m_MemSize = DECODE_MAX_MEM;
		_TdecMem->m_MemCUseSize = DECODE_MAX_MEM;
		_TdecMem->m_CUseBlkStart = 0;
	}
	
	return td_success;
leave:
	decode_destroy_memblk(_TdecMem);
	return td_failure;
	
}

#define Decode_HasUseMemSize(TdecMem)		decode_getMemblkStat(HasUseMemSize, TdecMem)
#define Decode_CouldUseMemSize(TdecMem)	decode_getMemblkStat(CouldUseMemSize, TdecMem)
#define Decode_HasUseBlkEnd(TdecMem)		decode_getMemblkStat(HasUseBlkEnd, TdecMem)
#define Decode_CouldUseBlkStart(TdecMem)	decode_getMemblkStat(CouldUseBlkStart, TdecMem)
#define Decode_MemSize(TdecMem)			decode_getMemblkStat(MemSize, TdecMem)
#define Decode_blkNum(TdecMem)				decode_getMemblkStat(blkNum, TdecMem)
int decode_getMemblkStat(int _iFlag, TDecMemBlk *_TdecMem)
{
	int iRet = -1;
	switch(_iFlag)
	{
		case 0:
			break;
		case HasUseMemSize:
			iRet = _TdecMem->m_MemSize - _TdecMem->m_MemCUseSize;
			break;
		case CouldUseMemSize:
			iRet = _TdecMem->m_MemCUseSize;
			break;
		case HasUseBlkEnd:
		{
			iRet = _TdecMem->m_CUseBlkStart;
			break;
		}
		case CouldUseBlkStart:
		{
			iRet = _TdecMem->m_CUseBlkStart;
			break;
		}
		case MemSize:
		{
			iRet = _TdecMem->m_MemSize;
			break;
		}
		case blkNum:
			iRet = _TdecMem->m_blkNum;
			break;
		default :
			;
	}
	return iRet;
}


static int decode_alloc_memblk(TDecMemBlk *_TdecMem, int _iSize, TGetAddr *_addr, int *_piNum)
{
	int i = 0;
	int n = 0;
	int iSize = _iSize;
	int iFirstCouldBlk = 0;
	if(Decode_CouldUseMemSize(_TdecMem) < _iSize)
	{
		return td_failure;
	}
	if(-1 == (iFirstCouldBlk = Decode_CouldUseBlkStart(_TdecMem)))
	{
		return td_failure;
	}
	if(0 == _TdecMem->m_blk[iFirstCouldBlk]->m_blkCUseSize)
	{
		iFirstCouldBlk++;
		iFirstCouldBlk %= _TdecMem->m_blkNum;
	}
	for(i = 0; i < _TdecMem->m_blkNum ; i++)
	{
		int temp = (i + iFirstCouldBlk) % _TdecMem->m_blkNum;
		_TDecMemBlk *blktemp = _TdecMem->m_blk[temp];
		if(blktemp->m_blkCUseSize <= iSize)
		{
			if(blktemp->m_blkCUseSize + blktemp->m_blkCUseLocStart > blktemp->m_blkSize)
			{
				_addr[n].m_iBlkNo = temp;
				_addr[n].m_ptr = blktemp->dat + blktemp->m_blkCUseLocStart;
				_addr[n++].m_iLen = blktemp->m_blkSize - blktemp->m_blkCUseLocStart;

				_addr[n].m_iBlkNo = temp;				
				_addr[n].m_ptr = blktemp->dat;
				_addr[n++].m_iLen = blktemp->m_blkCUseLocStart + blktemp->m_blkCUseSize - blktemp->m_blkSize;
			}
			else
			{
				_addr[n].m_iBlkNo = temp;
				_addr[n].m_ptr = blktemp->dat + blktemp->m_blkCUseLocStart;
				_addr[n++].m_iLen = blktemp->m_blkCUseSize;
			}

			blktemp->m_blkCUseLocStart += blktemp->m_blkCUseSize;
			iSize -= blktemp->m_blkCUseSize;
			blktemp->m_blkCUseSize = 0;
		}
		else
		{
			if(iSize + blktemp->m_blkCUseLocStart > blktemp->m_blkSize)
			{
				_addr[n].m_iBlkNo = temp;
				_addr[n].m_ptr = blktemp->dat + blktemp->m_blkCUseLocStart;
				_addr[n++].m_iLen = blktemp->m_blkSize - blktemp->m_blkCUseLocStart;

				_addr[n].m_iBlkNo = temp;				
				_addr[n].m_ptr = blktemp->dat;
				_addr[n++].m_iLen = iSize - (blktemp->m_blkSize - blktemp->m_blkCUseLocStart);
			}
			else
			{
				_addr[n].m_iBlkNo = temp;
				_addr[n].m_ptr = blktemp->dat + blktemp->m_blkCUseLocStart;
				_addr[n++].m_iLen = iSize;
			}
			
			blktemp->m_blkCUseSize -= iSize;
			blktemp->m_blkCUseLocStart += iSize;
			blktemp->m_blkCUseLocStart %= blktemp->m_blkSize;
			iSize = 0;
		}
		if(0 == iSize)
		{
			_TdecMem->m_MemCUseSize -= _iSize;
			_TdecMem->m_CUseBlkStart = temp;
			break;
		}
	}
	*_piNum = n;
	return td_success;
}

static int decode_free_memblk(TDecMemBlk *_TdecMem, TGetAddr *_addr, int _iNum, int _flag)//开头或者结束删除
{
	int i = 0;
	if(!_addr)
	{
		for(i = 0; i < _TdecMem->m_blkNum; i++)
		{
			_TdecMem->m_blk[i]->m_blkCUseLocStart = 0;
			_TdecMem->m_blk[i]->m_blkCUseSize = _TdecMem->m_blk[i]->m_blkSize;
			_TdecMem->m_MemCUseSize = _TdecMem->m_MemSize;
			_TdecMem->m_CUseBlkStart = 0;
		}
	}
	else
	{
		for(i = 0; i < _iNum; i++)
		{
			int iBlkNo = _addr[i].m_iBlkNo;
			_TDecMemBlk *blktemp = _TdecMem->m_blk[iBlkNo];
			if(0 == blktemp->m_blkCUseSize)//数据全满
			{
				blktemp->m_blkCUseSize = _addr[i].m_iLen;
				blktemp->m_blkCUseLocStart = _addr[i].m_ptr - (void *)blktemp->dat;
			}
			else//有空闲
			{
				blktemp->m_blkCUseSize += _addr[i].m_iLen;
				if((_addr[i].m_ptr - (void *)blktemp->dat + _addr[i].m_iLen) % blktemp->m_blkSize == blktemp->m_blkCUseLocStart)
				{
					blktemp->m_blkCUseLocStart = _addr[i].m_ptr - (void *)blktemp->dat;
				}
			}
			_TdecMem->m_MemCUseSize +=  _addr[i].m_iLen;
		}
		if(0 == _flag)//结束删除
		{
			_TdecMem->m_CUseBlkStart = _addr[0].m_iBlkNo;
			if(_TdecMem->m_blk[_addr[0].m_iBlkNo]->m_blkCUseSize == _TdecMem->m_blk[_addr[0].m_iBlkNo]->m_blkSize)
			{
				_TdecMem->m_CUseBlkStart--;
				_TdecMem->m_CUseBlkStart = _TdecMem->m_CUseBlkStart < 0 ? _TdecMem->m_blkNum - 1 : _TdecMem->m_CUseBlkStart;
			}
		}
		if(_TdecMem->m_MemCUseSize == _TdecMem->m_MemSize)
		{
			_TdecMem->m_CUseBlkStart = 0;
		}
	}
	return td_success;
}
/////////////////////块end/////////////////////////////

////////////////////链表begin/////////////////////////

typedef struct
{
	int m_iDataLen;//不包含帧头帧尾的长度
	unsigned char m_u8DataType;
	unsigned int m_u32Fileloc;
	unsigned int m_u32TimeStamp;
	char m_Coach[128];
}TDecLinkHead;

typedef struct
{
	int m_iNum;
	TDecLinkHead m_DataHead;
	TGetAddr 	m_add[DECODE_MAX_MEM_BLK];
	void *next;
}_TDecMemLink;

typedef struct
{
	int m_iUseNum;
	int m_iIdleNum;
	void *m_Link;
	_TDecMemLink *m_TUseLink;
	_TDecMemLink *m_TIdleLink;
	TDecMemBlk 	m_DecMem;
}TDecodeMemLink;

int decode_Init_memlink(TDecodeMemLink *_pTMem, int _iSize)
{
	if(!_pTMem || _iSize <= 0) 
	{
		return td_failure;
	}
	if(td_success != decode_init_memblk(&_pTMem->m_DecMem))
	{
		return td_failure;
	}
	_pTMem->m_Link = malloc(sizeof(_TDecMemLink) * _iSize);
	if(!_pTMem->m_Link)
	{
		return td_failure;
	}
	int i = _iSize - 1;
	_TDecMemLink *link = _pTMem->m_Link;
	_TDecMemLink *linkTmp = &link[i];
	linkTmp->next = NULL;
	for(; i > 0;)
	{
		i--;
		link[i].next = linkTmp;
		linkTmp = &link[i];
	}
	_pTMem->m_iUseNum = 0;
	_pTMem->m_iIdleNum = _iSize;
	_pTMem->m_TUseLink = NULL;
	_pTMem->m_TIdleLink = _pTMem->m_Link;
	return td_success;
}

int decode_destroy_memlink(TDecodeMemLink *_pTMem)
{
	if(_pTMem->m_Link)
	{
		free(_pTMem->m_Link);
		_pTMem->m_Link = NULL;
		_pTMem->m_TUseLink = NULL;
		_pTMem->m_TIdleLink = NULL;
		_pTMem->m_iUseNum = 0;
		_pTMem->m_iIdleNum = 0;
	}
	decode_destroy_memblk(&_pTMem->m_DecMem);
	return td_success;
}

_TDecMemLink *decode_alloc_linknode(TDecodeMemLink *_pTMem, int _iSize)
{
	_TDecMemLink *tmp = NULL;
	if(_pTMem->m_TIdleLink)
	{
		tmp = _pTMem->m_TIdleLink;
		if(td_success == decode_alloc_memblk(&_pTMem->m_DecMem, _iSize, tmp->m_add, &tmp->m_iNum))
		{
			_TDecMemLink *UseTmp;
			_pTMem->m_TIdleLink = _pTMem->m_TIdleLink->next;
			UseTmp = _pTMem->m_TUseLink;
			_pTMem->m_TUseLink = tmp;
			_pTMem->m_TUseLink->next = UseTmp;
			_pTMem->m_iUseNum++;
			_pTMem->m_iIdleNum--;
		}
		else
		{
			tmp = NULL;
		}
	}
	return tmp;
}

int decode_free_linknode(TDecodeMemLink *_pTMem, int _iFlag)
{
	_TDecMemLink *tmp = _pTMem->m_TUseLink;
	if(!tmp)
	{
		return td_failure;
	}
	
	if(0 == _iFlag)//从最上端开始删除
	{
		if(tmp)
		{
			_TDecMemLink *IdleTmp;
			decode_free_memblk(&_pTMem->m_DecMem, tmp->m_add, tmp->m_iNum, _iFlag);
			IdleTmp = _pTMem->m_TUseLink;
			_pTMem->m_TUseLink = _pTMem->m_TUseLink->next;
			IdleTmp->next = _pTMem->m_TIdleLink;
			_pTMem->m_TIdleLink = IdleTmp;
			_pTMem->m_iUseNum--;
			_pTMem->m_iIdleNum++;
		}
	}
	else if(1 == _iFlag)//从最下端删除
	{
		_TDecMemLink *secLast;
		for(;;)
		{
			secLast = tmp;
			if(tmp->next)
			{
				tmp = tmp->next;
			}
			else
			{
				break;
			}
		}
		decode_free_memblk(&_pTMem->m_DecMem, tmp->m_add, tmp->m_iNum, _iFlag);
		secLast->next = NULL;
		tmp->next = _pTMem->m_TIdleLink;
		_pTMem->m_TIdleLink = tmp;
		_pTMem->m_iUseNum--;
		_pTMem->m_iIdleNum++;
	}

	return td_success;
}

_TDecMemLink *decode_get_linknode(TDecodeMemLink *_pTMem, int _iNum)
{
	int i = 0;
	_TDecMemLink *tmp = _pTMem->m_TUseLink;
	if(!tmp)
	{
		return NULL;
	}
	for(;;)
	{
		if(i++ == _iNum)
		{
			return tmp;
		}
		
		if(tmp->next)
		{
			tmp = tmp->next;
		}
		else
		{
			return NULL;
		}
	}
	return NULL;
}

int decode_get_linknodestat(TDecodeMemLink *_pTMem, int _iUse)
{
	if(_iUse)
	{
		return _pTMem->m_iUseNum;
	}
	else
	{
		return _pTMem->m_iIdleNum;
	}
}


////////////////////链表end/////////////////////////

//////////////////////////////////////////////////////
typedef struct
{
	TDecodeMemLink m_tMem[2];//倒放所要维护的两块内存。
	int m_iCouldRead[2];//上述两块内存的读写情况
	int m_iReadMem;//当前正在操作哪块内存
	pthread_mutex_t m_memlock;
}TBackMem;

#define DECODE_MEM_NODE	25

int decode_mem_could_write(void *_this)//0:不可以写，1:可以写
{
	int iRet = 0;
	TDecodeMem *tDecodeMem = (TDecodeMem *)_this;
	TBackMem *tBack = (TBackMem *)tDecodeMem->m_data;
	if(!tBack)
	{
		return td_failure;
	}
	
	pthread_mutex_lock(&tBack->m_memlock);
	if(0 == tBack->m_iCouldRead[tBack->m_iReadMem])
	{
		iRet = 2;//必须写
		goto leave;
	}
	
	if(0 == tBack->m_iCouldRead[1 - tBack->m_iReadMem])
	{
		iRet = 1;//可以写
		goto leave;
	}
leave:
	pthread_mutex_unlock(&tBack->m_memlock);
	return iRet;
}

int decode_mem_write(void *_this,TOrigData *ptr, S_FrameHeader *_pFrameHeader, int _iFileLoc, int _iFinishFlag)
{
	int i = 0;
	int iLoc = 0;
	int *pCouldRead = NULL;
	TDecodeMemLink *pTdec = NULL;
	_TDecMemLink *pDatLink = NULL;
	TDecodeMem *tDecodeMem = (TDecodeMem *)_this;
	TBackMem *tBack = (TBackMem *)tDecodeMem->m_data;
	if(!tBack)
	{
		return td_failure;
	}
	//判断是否可写
	if(1 == tBack->m_iCouldRead[0] && 1 == tBack->m_iCouldRead[1])
	{
		return td_success;
	}
	pthread_mutex_lock(&tBack->m_memlock);
	if(0 == tBack->m_iCouldRead[tBack->m_iReadMem])
	{
		pCouldRead = &tBack->m_iCouldRead[tBack->m_iReadMem];
		pTdec = &tBack->m_tMem[tBack->m_iReadMem];
	}
	else
	{
		pCouldRead = &tBack->m_iCouldRead[1 - tBack->m_iReadMem];
		pTdec = &tBack->m_tMem[1 - tBack->m_iReadMem];
	}
	pthread_mutex_unlock(&tBack->m_memlock);
	//申请一块节点，如果申请出错，判断是否列表中有数据，如果有，置该块可读
	if((pDatLink = decode_alloc_linknode(pTdec, ptr->m_iLen)) == NULL)
	{
		if(decode_get_linknodestat(pTdec, 1))
		{
			*pCouldRead = 1;
		}
	}
	else
	{
		//拷贝数据
		pDatLink->m_DataHead.m_u8DataType  = _pFrameHeader->u8FrameType;
		pDatLink->m_DataHead.m_u32Fileloc = _iFileLoc;
		pDatLink->m_DataHead.m_u32TimeStamp = _pFrameHeader->u32TimeStamp;
		if(0xff == _pFrameHeader->u8FrameType)
		{
			*pCouldRead = 1;
		}
		else
		{
			for(i = 0; i < pDatLink->m_iNum; i++)
			{
				memcpy(pDatLink->m_add[i].m_ptr, ptr->m_pData+ iLoc, pDatLink->m_add[i].m_iLen);
				iLoc += pDatLink->m_add[i].m_iLen;
			}
			memcpy(pDatLink->m_DataHead.m_Coach, ptr->m_pDataHeader, g_iVedioFrmInfoSLen);
			//_iFinishFlag,进行处理，以确定是否可读
			if(_iFinishFlag)
			{
				*pCouldRead = 1;
			}
		}
	}
	
	return td_success;
}
int decode_mem_read(void *_this, TOrigData *ptr)//返回帧头信息
{
	int i = 0;
	int loc = 0;
	int nodeNum = 0;
	_TDecMemLink *pDatLink;
	//获取一包数据
	TDecodeMem *tDecodeMem = (TDecodeMem *)_this;
	TBackMem *tBack = (TBackMem *)tDecodeMem->m_data;
	if(!tBack)
	{
		return td_failure;
	}
	
	pDatLink = decode_get_linknode(&tBack->m_tMem[tBack->m_iReadMem], 0);
	if(!pDatLink)//只要有数据，改情况不会发生
	{
		return  td_failure;
	}
	//cp
	memcpy(ptr->m_pDataHeader, pDatLink->m_DataHead.m_Coach, g_iVedioFrmInfoSLen);
	for(i = 0; i < pDatLink->m_iNum; i++)
	{
		memcpy(ptr->m_pData + loc, pDatLink->m_add[i].m_ptr, pDatLink->m_add[i].m_iLen);
		loc += pDatLink->m_add[i].m_iLen;
	}	
	//释放该包数据
	decode_free_linknode(&tBack->m_tMem[tBack->m_iReadMem], 0);
	//获取队列中还有的包的个数
	nodeNum = decode_get_linknodestat(&tBack->m_tMem[tBack->m_iReadMem], 1);
	//判断个数，如果读完则切换到另一个块，并且置该块不可读

	if(0 == nodeNum)
	{
		pthread_mutex_lock(&tBack->m_memlock);
		tBack->m_iCouldRead[tBack->m_iReadMem] = 0;
		tBack->m_iReadMem = 1 - tBack->m_iReadMem;
		pthread_mutex_unlock(&tBack->m_memlock);
	}
	
	return pDatLink->m_DataHead.m_u32Fileloc;
}
int decode_mem_read_nofree(void *_this, S_FrameHeader *_pheader)//返回帧头信息
{
	_TDecMemLink *pDatLink;
	TDecodeMem *tDecodeMem = (TDecodeMem *)_this;
	TBackMem *tBack = (TBackMem *)tDecodeMem->m_data;
	if(!tBack)
	{
		return td_failure;
	}
	
	//获取一包数据
	pthread_mutex_lock(&tBack->m_memlock);
	pDatLink = decode_get_linknode(&tBack->m_tMem[tBack->m_iReadMem], 0);
	if(!pDatLink || 0xff == pDatLink->m_DataHead.m_u8DataType)
	{
		pthread_mutex_unlock(&tBack->m_memlock);
		return td_failure;
	}
	_pheader->u32TimeStamp = pDatLink->m_DataHead.m_u32TimeStamp;
	pthread_mutex_unlock(&tBack->m_memlock);
	return pDatLink->m_DataHead.m_u32Fileloc;
}

int decode_mem_clear(void *_this)
{
	int i = 0;
	TDecodeMem *tDecodeMem = (TDecodeMem *)_this;
	TBackMem *tBack = (TBackMem *)tDecodeMem->m_data;

	for(i = 0; i < 2; i++)
	{
		for(;;)
		{
			if(td_failure == decode_free_linknode(&tBack->m_tMem[i], 0))
			{
				break;
			}
		}
		tBack->m_iCouldRead[i] = 0;
	}
	tBack->m_iReadMem = 0;
	return td_success;
}

int decode_mem_get_size(void *_this, int _iOrgDataLen)
{
	int iMemBlk = 0;
	TDecodeMem *tDecodeMem = (TDecodeMem *)_this;
	TBackMem *tBack = (TBackMem *)tDecodeMem->m_data;
	if(!tBack)
	{
		return td_failure;
	}
	
	if(_iOrgDataLen <= 0)
	{
		return 25;
	}

	iMemBlk = (DECODE_MAX_MEM / _iOrgDataLen);

	return iMemBlk > 25 ? 25 : iMemBlk;
}


int decode_mem_destroy(void *_this)
{
	TDecodeMem *tDecodeMem = (TDecodeMem *)_this;
	TBackMem *tBack = (TBackMem *)tDecodeMem->m_data;
	if(!tBack)
	{
		goto leave;
	}
	decode_destroy_memlink(&tBack->m_tMem[0]);
	decode_destroy_memlink(&tBack->m_tMem[1]);
	pthread_mutex_destroy(&tBack->m_memlock);
	free(tBack);
	tDecodeMem->m_data = NULL;

leave:
	return td_success;
}


int decode_mem_Init(void *_this)
{
	int iRet = 0;
	TBackMem *tBack = NULL;
	TDecodeMem *tDecodeMem = (TDecodeMem *)_this;
	if(tDecodeMem->m_data)
	{
		goto leave;
	}
	tDecodeMem->m_data = malloc(sizeof(TBackMem));
	if(!tDecodeMem->m_data)
	{
		iRet = td_failure;
		goto leave;
	}

	//平台相关需要获取
	g_iVedioFrmInfoSLen = media_GetFrmInfoStrucLen();
	
	tBack = (TBackMem *)tDecodeMem->m_data;
	tDecodeMem->BackMem_Init = decode_mem_Init;
	tDecodeMem->BackMem_destroy = decode_mem_destroy;
	tDecodeMem->BackMem_could_write = decode_mem_could_write;
	tDecodeMem->BackMem_write = decode_mem_write;
	tDecodeMem->BackMem_read = decode_mem_read;
	tDecodeMem->BackMem_read_nofree = decode_mem_read_nofree;
	tDecodeMem->BackMem_getSize = decode_mem_get_size;
	tDecodeMem->BackMem_clear = decode_mem_clear;
	iRet = decode_Init_memlink(&tBack->m_tMem[0], DECODE_MEM_NODE);
	iRet |= decode_Init_memlink(&tBack->m_tMem[1], DECODE_MEM_NODE);
	if(td_success != iRet)
	{
		goto leave;
	}
	tBack->m_iCouldRead[0] = 0;
	tBack->m_iCouldRead[1] = 0;
	tBack->m_iReadMem = 0;
	pthread_mutex_init(&tBack->m_memlock,NULL);
leave:
	if(td_success != iRet)
	{
		decode_mem_destroy(tDecodeMem);		
	}
	return iRet;
}


//////////////////////////////////////////////////////

