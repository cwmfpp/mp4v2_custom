#include "playback.h"
#include "decode.h"
#include "td_sdk.h"
#include "channelmnt.h"
#include "libstorage.h"
#include <stdlib.h>
#include <string.h>

///////////////////////////
//#define MAX_FILENAME_LEN	64

/////////////////////////////////////////////////
typedef struct
{
	void *m_father;
	int m_iStartTime;
	int m_iEndTime;
	int m_iID;
	char m_cFileName[32];
}TFile;

#define TLIST void

typedef struct _data
{
	void *m_pNode;
	int m_iLen;
	char m_data[0];
}Tdata;

typedef struct _node
{
	struct _node *m_pBef;
	struct _node *m_pNext;
	Tdata m_tdata[0];
}Tnode;

typedef struct _list
{	
	int m_iUse;
	int m_iCnt;
	int m_iChn;//作为ID号处理
	Tnode *m_tCur;
	Tnode *m_tHead;
	Tnode *m_tWail;
}Tlist;

static Tnode *list_mallocNode(TLIST *_List, int _iLen)
{
	Tlist *pList = (Tlist *)_List;
	Tnode *pNode;

	//动态申请
	pNode = (Tnode *)malloc(sizeof(Tnode) +sizeof(Tdata) +  _iLen);
	if(NULL != pNode)
	{
		pList->m_iCnt++;
		pNode->m_pBef = NULL;
		pNode->m_pNext = NULL;
	}
	return pNode;
}

#if 0
static int list_freeNode(TLIST *_List, Tnode*_ptNode)
{
	Tlist *pList = (Tlist *)_List;
	if(NULL == pList || NULL == _ptNode)
	{
		return td_failure;
	}

	if(_ptNode == pList->m_tCur)
	{
		pList->m_tCur = pList->m_tCur->m_pNext;
	}

	if(_ptNode == pList->m_tHead)
	{
		pList->m_tHead = pList->m_tHead->m_pNext;
	}

	if(_ptNode == pList->m_tWail)
	{
		pList->m_tWail = pList->m_tWail->m_pBef;
	}

	free(_ptNode);
	pList->m_iCnt--;
	return td_success;
}
#endif
static Tdata *list_addNode(TLIST *_List, Tnode *_tNode, int _iLoc, void *_data, int _iLen)
{
	Tlist *pList = (Tlist *)_List;
	Tnode *pNode;
	Tnode *temp = _tNode;
	#if 1
	if(NULL == (pNode = list_mallocNode(_List, _iLen)))
	{
		return NULL;
	}
	if(NULL == pList->m_tHead)
	{
		pList->m_tWail = pList->m_tHead = pNode;
		pList->m_tCur = pList->m_tHead;
		pList->m_tHead->m_pBef = pList->m_tHead->m_pNext = NULL;
		temp = pList->m_tHead;
	}
	else
	{
		if(-1 == _iLoc)
		{
			if(NULL == temp)
			{
				pNode->m_pBef = NULL;
				pNode->m_pNext = pList->m_tHead;
				pList->m_tHead->m_pBef = pNode;
				pList->m_tHead = pNode;
			}
			else
			{
				if(pList->m_tHead == temp)
				{
					pList->m_tHead = pNode;
				}
				
				pNode->m_pBef = temp->m_pBef;
				pNode->m_pNext = temp;
				temp->m_pBef = pNode;
			}
		}
		else
		{
			if(NULL == temp)
			{
				pNode->m_pNext = NULL;
				pNode->m_pBef = pList->m_tWail;
				pList->m_tWail->m_pNext = pNode;
				pList->m_tWail = pNode;
			}
			else
			{
				if(pList->m_tWail == temp)
				{
					pList->m_tWail = pNode;
				}
				pNode->m_pNext = temp->m_pNext;
				pNode->m_pBef = temp;
				temp->m_pNext = pNode;
			}
		}
	}
	pNode->m_tdata->m_iLen = _iLen;
	pNode->m_tdata->m_pNode = pNode;
	memcpy(pNode->m_tdata->m_data, _data, _iLen);
	td_printf(0, "name = %s\n", ((TFile *)_data)->m_cFileName);
	#endif
	return (NULL != pNode) ? pNode->m_tdata : NULL; 
}

static int list_setLoc(TLIST *_List, Tnode *_tNode)
{
	Tlist *pList = (Tlist *)_List;
	pList->m_tCur = _tNode;
	return td_success;
}
#if 0
static int list_delNode(TLIST *_List, Tdata *_tdata)
{
	Tlist *pList = (Tlist *)_List;
	return list_freeNode(pList, (Tnode *)(_tdata->m_pNode));
}
#endif

static TLIST *list_createList(void)
{
	Tlist *pList = NULL;
	pList = (Tlist *)malloc(sizeof(Tlist));
	if(NULL == pList)
	{
		goto leave;
	}

	pList->m_iUse = 0;
	pList->m_iCnt = 0;
	pList->m_tCur = pList->m_tHead = pList->m_tWail = NULL;
leave:
	return pList;
}

static int list_delList(TLIST *_ptList)
{
	Tlist *pList = (Tlist *)_ptList;
	Tnode *tNode = NULL;
	Tnode *tNode1 = NULL;
	
	if(NULL == pList)
	{
		return td_failure;
	}
	for(tNode = pList->m_tHead; NULL != tNode; tNode = tNode1)
	{
		td_printf(0, "xxxxxxname = %s\n", ((TFile *)(tNode->m_tdata->m_data))->m_cFileName);
		tNode1 = tNode->m_pNext;
		free(tNode);
	}
	free(pList);
	return td_success;
}



/////////////////////////////////////////////////
///////////////////////////////////////////////////
#define MAX_FILENAME_LEN 32
#if 0
typedef struct
{
	void *m_father;
	int m_iStartTime;
	int m_iEndTime;
	int m_iID;
	char m_cFileName[MAX_FILENAME_LEN];
}TFile;
#endif

#define NEXT_FILE	0
#define CUR_FILE		1
#define NOOPT_FILE	2

typedef struct
{
	Tlist *m_ptList;
	int m_iOpenId;
	int m_iStartTime;
	int m_iStopTime;
	unsigned int m_u32UpFrameTm;
	long long m_llCurTm;
	char m_File[MAX_FILENAME_LEN];
	FILE *m_fp;
	S_FileHeader m_FileHeader;//文件头
	S_FrameHeader m_FrameHeader;//帧头
	int m_iIFrameNum;
	int m_iSingleIndexLen;
	void *m_pIFrameIndex;
	int m_IFrameLoaction;
	int m_FileLoction;
	char m_cPassWord[ENCRYPT_PWD_LENGTH + 1];
	char m_cPass[64];
	int m_iPassFlag;
}TPlayFile;

//////////////////////////////////////////////////////
static Tlist *playfile_addList(time_t _iStartTime, time_t _iStopTime, TFileInfo *_ptFileListInfo, int _iCnt)
{
	int i;
	Tnode *ptNode = NULL;
	TFile *ptFile = NULL;
	Tlist *ptList = NULL;
	TFile tFile;
	int iFind;
	int iCnt = 0;
	if(NULL == (ptList = list_createList()))
	{
		return NULL;
	}
	for(i = 0; i < _iCnt; i++)
	{
		if(_ptFileListInfo[i].m_iStartTime > _iStopTime || _ptFileListInfo[i].m_iStopTime < _iStartTime ||
			_ptFileListInfo[i].m_iStartTime == 0 || _ptFileListInfo[i].m_iStopTime == 0)
		{
			continue;
		}
		if(_ptFileListInfo[i].m_iStartTime >= _ptFileListInfo[i].m_iStopTime)
		{
			continue;
		}
		for(iFind = 0, ptNode = ptList->m_tWail; NULL != ptNode; ptNode = ptNode->m_pBef)
		{
			ptFile = (TFile *)(ptNode->m_tdata->m_data);
			if(ptFile->m_iEndTime <= _ptFileListInfo[i].m_iStartTime)
			{
				iFind = 1;
				break;
			}
		}
		tFile.m_iStartTime = _ptFileListInfo[i].m_iStartTime;
		tFile.m_iEndTime = _ptFileListInfo[i].m_iStopTime;
		tFile.m_iID = (iCnt++);
		strncpy(tFile.m_cFileName, (char *)_ptFileListInfo[i].m_ucFileName, MAX_FILENAME_LEN - 1);

		if(iFind)
		{
			list_addNode(ptList, ptNode, 1, &tFile, sizeof(tFile));	
		}
		else
		{
			list_addNode(ptList, NULL, -1, &tFile, sizeof(tFile));
		}
	}
	list_setLoc(ptList, ptList->m_tHead);
	return ptList;
}

PLAYFILE *playfile_build_List(time_t _iStartTime, time_t _iStopTime, int _Chn)
{
	int iRet;
	TQuery Query;
	TFileInfo *ptFileListInfo = NULL;
	Tlist *ptFile = NULL;
	TPlayFile *ptPlayFile = NULL;

	if(_Chn < 0)
	{
		return NULL;
	}
	if(NULL == (ptPlayFile = malloc(sizeof(TPlayFile))))
	{	
		goto failure;
	}
	if(NULL == (ptFileListInfo = malloc(sizeof(TFileInfo) * MAXFILELISTLEN)))
	{
		goto failure;
	}

	Query.m_uiPageSize = PAGE_SHOW_NUM;
	Query.m_iStartTime = _iStartTime - 7200;
	if(Query.m_iStartTime < 0)				//提前2小时查询，判断边界
	{
		Query.m_iStartTime = 0;
	}
	Query.m_iStopTime = _iStopTime;
	Query.m_ucFileType = FILE_REC;
	Query.m_ucRecType = 0xff;			//查询录像所有类型
	Query.m_ucDevType =0xff;
	Query.m_ucChannelNo = UkSubToData(_Chn);
	Query.m_uiPageNo = 0;	
	Query.m_uiTotalNum = MAXFILELISTLEN - 1;
	Query.m_cReserved[0] = '\0';
	if(ERR_REC_SUCCESSFUL != (iRet = rec_QueryAllFiles(&Query, ptFileListInfo)))
	{
		goto failure;
	}
	Query.m_uiTotalNum = (Query.m_uiTotalNum> MAXFILELISTLEN) ? MAXFILELISTLEN : Query.m_uiTotalNum;
	if(NULL == (ptFile = playfile_addList(_iStartTime, _iStopTime, ptFileListInfo, Query.m_uiTotalNum)))
	{
		goto failure;
	}

	td_printf(0,"ptFile->m_iCnt = %d", ptFile->m_iCnt);
	if(ptFile->m_iCnt <= 0)
	{
		goto failure;
	}
	free(ptFileListInfo);
	//////////////////////
	//初始化变量
	ptPlayFile->m_ptList = ptFile;
	ptPlayFile->m_iOpenId = CUR_FILE;
	ptPlayFile->m_iStartTime = _iStartTime;
	ptPlayFile->m_iStopTime = _iStopTime;
	ptPlayFile->m_llCurTm = (long long)_iStartTime * 1000;
	ptPlayFile->m_u32UpFrameTm = 0;
	ptPlayFile->m_fp = NULL;
	memset(&ptPlayFile->m_FileHeader, 0, sizeof(ptPlayFile->m_FileHeader));
	memset(&ptPlayFile->m_FrameHeader, 0, sizeof(ptPlayFile->m_FrameHeader));	
	memset(&ptPlayFile->m_File, 0, sizeof(ptPlayFile->m_FrameHeader));	
	ptPlayFile->m_iIFrameNum = 0;
	ptPlayFile->m_iSingleIndexLen = 0;
	ptPlayFile->m_pIFrameIndex = NULL;
	ptPlayFile->m_IFrameLoaction = 0;
	ptPlayFile->m_FileLoction = sizeof(S_FileHeader);
	ptPlayFile->m_iPassFlag = 0;
	ptPlayFile->m_cPass[0] = 0;
	ptPlayFile->m_cPassWord[0] = 0;
	//////////////////////
	return ptPlayFile;
	
failure:
	if(NULL != ptPlayFile)
	{
		free(ptPlayFile);
	}
	if(NULL != ptFileListInfo)
	{
		free(ptFileListInfo);
	}
	if(NULL != ptFile)
	{
		list_delList(ptFile);
	}
	return NULL;
}

int playfile_unbuild_List(PLAYFILE *_ptPlayFile)
{
	TPlayFile *ptPlayFile = _ptPlayFile;
	if(NULL == _ptPlayFile)
	{
		return td_success;
	}
	
	if(NULL != ptPlayFile->m_fp)
	{
		rec_CloseFile(&ptPlayFile->m_fp);
	}

	if(NULL != ptPlayFile->m_pIFrameIndex)
	{
		free(ptPlayFile->m_pIFrameIndex);
	}
	if(NULL != ptPlayFile->m_ptList)
	{
		list_delList(ptPlayFile->m_ptList);
	}
	
	free(ptPlayFile);
	return td_success;
}

static int Is_passwordFrame(S_FrameHeader *_pTFrameheader, char *_password, char *_Databuf, char *_PassBuf)
{
	int iEncryptLen;
	if(0x02 == (_pTFrameheader->reserved & 0x03) && (_pTFrameheader->u32FrameSize - sizeof(S_FrameHeader)) >= 64)
	{
		if(0 == (iEncryptLen = strlen(_password)))
		{
			memcpy(_PassBuf, _Databuf, 64);
			return td_failure;
		}

		memcpy(_PassBuf, _Databuf, 64);
		aes_crypt((unsigned char *)_PassBuf, 64, _pTFrameheader->u32FrameNO, (unsigned char *)_password, iEncryptLen);
		if(((int *)_PassBuf)[0] != 0X1000000)
		{
			memcpy(_PassBuf, _Databuf, 64);
			return td_failure;
		}
		else
		{
			_pTFrameheader->reserved &= ~0x03;
			memcpy(_Databuf, _PassBuf, 64);
			return td_success;
		}
	}
	return td_success;
}

static int  Is_ValidFrame(S_FrameHeader* frHeader, int _iLen)
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
	if(frHeader->u32FrameSize>_iLen)
    	{
		td_printf(0, "%s 12 \n", __FUNCTION__ );
        	return -1;
    	}
    	if(frHeader->u32VStreamLen>_iLen)
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

static int Is_InsertFrame(S_FrameHeader *_pTFrameheader, int _iLen)
{
	if(_pTFrameheader->u32FrameID == 0 && _pTFrameheader->u32FrameSize == 0)
	{
		return -1;
	}
	if(1 != Is_ValidFrame(_pTFrameheader, _iLen))
	{
		return -1;
	}
	if(_pTFrameheader->reserved == REC_FALSE_FRAME_FLAG)
	{
		td_printf(0, "xxxxxxx xxxptPlayFile->m_FrameHeader.u32FrameSize = %d\n", _pTFrameheader->u32FrameSize);
		return 0;
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

static int playfile_readFrameIndex(PLAYFILE *_PlayId)
{
	TPlayFile *ptPlayFile = _PlayId;
	S_FrameHeader FrameHeader;//帧头
	
	int iFlag = 0;
	int iLocationTemp;
	int iIframeLen = 0;
	int iINum = 0, iIInfoSize = 0;

	iLocationTemp = ftell(ptPlayFile->m_fp);
	fseek(ptPlayFile->m_fp, -(sizeof(TIFrameIndexTail)), SEEK_END);
	fread(&iFlag, 1, sizeof(int), ptPlayFile->m_fp);
	if(REC_IFRAME_INDEX_MAGIC_NUMBER != iFlag)
	{
		goto leave;	
	}

	fread(&iIframeLen, 1, sizeof(int), ptPlayFile->m_fp);
	fseek(ptPlayFile->m_fp, -iIframeLen, SEEK_END);
	fread(&FrameHeader, 1, sizeof(S_FrameHeader), ptPlayFile->m_fp);
	if(FrameHeader.u32FrameID != 0x02000000 || FrameHeader.u32FrameSize != iIframeLen)
	{
		goto leave;
	}

	fread(&iINum, 1, sizeof(int), ptPlayFile->m_fp);
	fread(&iIInfoSize, 1, sizeof(int), ptPlayFile->m_fp);
	fseek(ptPlayFile->m_fp, sizeof(int), SEEK_CUR);

	iIframeLen = iINum * iIInfoSize;
	if(iIframeLen > 500 * 1024 || (0 == iIframeLen))
	{
		goto leave;
	}

	if(NULL == (ptPlayFile->m_pIFrameIndex = malloc(iIframeLen)))
	{
		goto leave;
	}

	ptPlayFile->m_iIFrameNum = iINum;
	ptPlayFile->m_iSingleIndexLen = iIInfoSize;
	fread(ptPlayFile->m_pIFrameIndex, 1, iIframeLen, ptPlayFile->m_fp);
	ptPlayFile->m_IFrameLoaction = 0;	
	fseek(ptPlayFile->m_fp, iLocationTemp, SEEK_SET);
	return td_success;
	
leave:
	if(NULL != ptPlayFile->m_pIFrameIndex)
	{
		free(ptPlayFile->m_pIFrameIndex);
		ptPlayFile->m_pIFrameIndex = NULL;
	}	
	ptPlayFile->m_iIFrameNum = 0;
	ptPlayFile->m_IFrameLoaction = 0;
	ptPlayFile->m_iSingleIndexLen = 0;
	fseek(ptPlayFile->m_fp, iLocationTemp, SEEK_SET);
	return td_failure;
}


static FILE *playfile_open(PLAYFILE *_PlayId, Tnode *_ptNode, int *_piChgFile)
{
	TPlayFile *ptPlayFile = _PlayId;
	Tlist *ptList;
	Tnode *ptNode;
	TFile *ptFile;
	if(NULL != _piChgFile)
	{
		*_piChgFile = 0;
	}
//	unsigned char cTmpPath[FULL_PATH_LEN] = {0};
	if(NULL != _ptNode)
	{
		ptNode = _ptNode;
	}
	else
	{
		ptNode = ptPlayFile->m_ptList->m_tCur;
	}
	if(NULL == ptNode)
	{
		return NULL;
	}
	ptList = ptPlayFile->m_ptList;
	//ptNode = ptList->m_tCur;
	ptFile = (TFile *)(ptNode->m_tdata->m_data);
	
	//if(ptPlayFile->m_iOpenId != ptFile->m_iID)
	if(NOOPT_FILE != ptPlayFile->m_iOpenId)
	{
		if(NEXT_FILE == ptPlayFile->m_iOpenId)
		{
			ptNode = ptNode->m_pNext;
		}
		ptList->m_tCur = NULL;
		for(; NULL != ptNode; ptNode = ptNode->m_pNext)
		{		
			if(NULL != ptPlayFile->m_fp)
			{
				rec_CloseFile(&ptPlayFile->m_fp);
				ptPlayFile->m_fp = NULL;
			}
			
			if(NULL != ptPlayFile->m_pIFrameIndex)
			{
				free(ptPlayFile->m_pIFrameIndex);
				ptPlayFile->m_pIFrameIndex = NULL;
			}
			
			ptFile = (TFile *)(ptNode->m_tdata->m_data);
			//获取全称
			//rec_CompleteFileName((td_s8 *)ptFile->m_cFileName, (td_s8 *)cTmpPath, NULL, NULL);
			if(ERR_REC_SUCCESSFUL == rec_OpenFile(&ptPlayFile->m_fp, (td_s8 *)ptFile->m_cFileName))
			{
				td_printf(0, "ptFile->m_cFileName = %s\n", ptFile->m_cFileName);
				if(ERR_REC_SUCCESSFUL == rec_ReadFileHeader(ptPlayFile->m_fp, (td_u8 *)ptFile->m_cFileName, &ptPlayFile->m_FileHeader)) 
				{
					if(IsValidFileHeader(&ptPlayFile->m_FileHeader) >= 0)
					{
						if(NULL != _piChgFile)
						{
							*_piChgFile = 1;
						}
						fseek(ptPlayFile->m_fp, sizeof(S_FileHeader), SEEK_SET);
						td_printf(0,"ptPlayFile->m_FileHeader.Width = %d, ptPlayFile->m_FileHeader.Height = %d", ptPlayFile->m_FileHeader.Width, ptPlayFile->m_FileHeader.Height);
						ptPlayFile->m_iOpenId = NOOPT_FILE;
						ptList->m_tCur = ptNode;
						memset(ptPlayFile->m_File, 0, MAX_FILENAME_LEN);
						strncpy(ptPlayFile->m_File, ptFile->m_cFileName, MAX_FILENAME_LEN - 1);
						playfile_readFrameIndex(_PlayId);
						ptPlayFile->m_llCurTm = (long long)(((TFile *)(ptList->m_tCur->m_tdata->m_data))->m_iStartTime) * 1000;
						td_printf(0,"ptPlayFile->m_llCurTm = %lld", ptPlayFile->m_llCurTm);
						ptPlayFile->m_u32UpFrameTm = 0;
						break;
					}
				}
			}
		}
	}


	return ptPlayFile->m_fp;
}

static int playfile_readOrder(PLAYFILE *_PlayId, void *_buf, int _iLen)
{
	int temp;
	int iFrameHeaderLen = sizeof(S_FrameHeader);
	TPlayFile *ptPlayFile = _PlayId;

	while(1)
	{
		if(1 == ptPlayFile->m_iPassFlag)
		{
			return td_failure - 1;
		}

		if(2 == ptPlayFile->m_iPassFlag)
		{
			temp = ptPlayFile->m_FrameHeader.u32FrameSize - iFrameHeaderLen;
			goto leave;
		}
		if(iFrameHeaderLen == fread(&ptPlayFile->m_FrameHeader, 1, iFrameHeaderLen, ptPlayFile->m_fp))
		{
			int iType = Is_InsertFrame(&ptPlayFile->m_FrameHeader, _iLen);
			if(iType < 0)
			{
				ptPlayFile->m_iOpenId = NEXT_FILE;
				return td_failure;
			}
			else if(0 == iType)
			{
				td_printf(0,"xxxxxxx ptPlayFile->m_FrameHeader.u32FrameSize = %d", ptPlayFile->m_FrameHeader.u32FrameSize);
				fseek(ptPlayFile->m_fp, ptPlayFile->m_FrameHeader.u32FrameSize - iFrameHeaderLen, SEEK_CUR);
				continue;
			}
			else
			{
				temp = ptPlayFile->m_FrameHeader.u32FrameSize - iFrameHeaderLen;
				//#ifndef SELF_DEC
				if(temp != fread(_buf + iFrameHeaderLen, 1, temp, ptPlayFile->m_fp))
				//#else
				//if(temp != fread(_buf, 1, temp, ptPlayFile->m_fp))
				//#endif
				{
					ptPlayFile->m_iOpenId = NEXT_FILE;						
					return td_failure;
				}
				break;
			}				
		}
		else
		{
			ptPlayFile->m_iOpenId = NEXT_FILE;
			return td_failure;
		}
	}
leave:
	ptPlayFile->m_iPassFlag = 0;
	if(td_failure == Is_passwordFrame(&ptPlayFile->m_FrameHeader, ptPlayFile->m_cPassWord, _buf + iFrameHeaderLen, ptPlayFile->m_cPass))
	{
		ptPlayFile->m_iPassFlag = 1;
		return td_failure - 1;
	}
	
	if((0 != ptPlayFile->m_u32UpFrameTm) && 
		(ptPlayFile->m_FrameHeader.u32TimeStamp > ptPlayFile->m_u32UpFrameTm) && 
		(ptPlayFile->m_FrameHeader.u32TimeStamp <= ptPlayFile->m_u32UpFrameTm + 1 * 60 * 1000))
	{
		ptPlayFile->m_llCurTm += (ptPlayFile->m_FrameHeader.u32TimeStamp - ptPlayFile->m_u32UpFrameTm);
		td_printf(0,"ptPlayFile->m_llCurTm = %lld", ptPlayFile->m_llCurTm);
	}
	ptPlayFile->m_u32UpFrameTm = ptPlayFile->m_FrameHeader.u32TimeStamp; 
	memcpy(_buf, &ptPlayFile->m_FrameHeader, iFrameHeaderLen);
	temp += iFrameHeaderLen;

	#ifndef SELF_DEC
	char *pcFreamTail = _buf + temp;
	pcFreamTail[0]='F';
	pcFreamTail[1]='E';
	pcFreamTail[2]='O';
	pcFreamTail[3]='F';
	pcFreamTail[4]=(((ptPlayFile->m_FrameHeader.u32FrameSize+8)&0xff0000)>>16);
	pcFreamTail[5]=(((ptPlayFile->m_FrameHeader.u32FrameSize+8)&0xff000000)>>24);
	pcFreamTail[6]=((ptPlayFile->m_FrameHeader.u32FrameSize+8)&0xff);				
	pcFreamTail[7]=(((ptPlayFile->m_FrameHeader.u32FrameSize+8)&0xff00)>>8);
	return temp + 8;
	#else
	return temp;
	#endif
}


//_iBefAfter: 0:找当前帧所在的I帧
//>0:向前找n帧
//<0:向后找n帧:目前只是支持1帧

static int playfile_IFmFromIndex(PLAYFILE *_PlayId, int _iLocation, int *_piSize)
{
	int i = 0;
	int iRet = PLAYBACK_FILEFINISH_NOEFFECT;
	char *ptr = NULL;
	int iLoc = 0;


	TPlayFile *ptPlayFile = _PlayId;
	
	if(0 == ptPlayFile->m_iIFrameNum)
	{
		return td_failure;
	}
	ptr = (char *)ptPlayFile->m_pIFrameIndex;	

	if(ptPlayFile->m_IFrameLoaction > ptPlayFile->m_iIFrameNum)
	{
		ptPlayFile->m_IFrameLoaction = 0;
	}
	
	i = ptPlayFile->m_IFrameLoaction;
	if(*(int *)(ptr + ptPlayFile->m_IFrameLoaction * ptPlayFile->m_iSingleIndexLen + sizeof(int)) <= _iLocation)
	{
		for(; i < ptPlayFile->m_iIFrameNum; i++)
		{
			if(*(int *)(ptr + i * ptPlayFile->m_iSingleIndexLen + sizeof(int)) > _iLocation)
			{
				ptPlayFile->m_IFrameLoaction = i;
				iRet = *(int *)(ptr + i * ptPlayFile->m_iSingleIndexLen + sizeof(int));
				iLoc = i;
				goto leave;
			}
		}
	}
	else
	{
		int tmpLoc = *(int *)(ptr + i * ptPlayFile->m_iSingleIndexLen + sizeof(int));
		for(; i >= 0; i--)
		{
			if(*(int *)(ptr + i * ptPlayFile->m_iSingleIndexLen + sizeof(int)) <= _iLocation)
			{
				ptPlayFile->m_IFrameLoaction = i;
				iRet = tmpLoc;
				goto leave;
			}
			tmpLoc = *(int *)(ptr + i * ptPlayFile->m_iSingleIndexLen + sizeof(int));
			iLoc = i;
		}
	}
leave:
	if((NULL != _piSize) && (iRet > 0) && (ptPlayFile->m_iSingleIndexLen > sizeof(int) * 3))
	{
		*_piSize = *(int *)(ptr + iLoc * ptPlayFile->m_iSingleIndexLen + 3 * sizeof(int));
	}
	return iRet;
}

static int playfile_readIFrame(PLAYFILE *_PlayId, void *_buf, int _iLen)//帧长限制
{
	int iFrameHeaderLen = sizeof(S_FrameHeader);
	TPlayFile *ptPlayFile = _PlayId;
	int iRealLocation;
	int iLocation;
	int iLen = 0;
	while(1)
	{
		iRealLocation = ftell(ptPlayFile->m_fp);
		if((iLocation = playfile_IFmFromIndex(_PlayId, iRealLocation, NULL)) < 0)
		{
			goto leave;
		}

		fseek(ptPlayFile->m_fp, iLocation, SEEK_SET);
		if(fread(&ptPlayFile->m_FrameHeader,1,iFrameHeaderLen,ptPlayFile->m_fp) < iFrameHeaderLen)
		{
			goto leave;
		}
		
		if(Is_InsertFrame(&ptPlayFile->m_FrameHeader, _iLen) <= 0)
		{	
			goto leave;
		}

		iLen = ptPlayFile->m_FrameHeader.u32FrameSize - iFrameHeaderLen;
		if(fread(_buf,1, iLen, ptPlayFile->m_fp) < iLen)
		{
			goto leave;
		}
		return iLen;
	}
	
	
leave:
	fseek(ptPlayFile->m_fp, iRealLocation, SEEK_SET);
	return td_failure;
}


//创建列表时，注意出事化
//int playfile_read(PLAYFILE *_PlayId, void *_buf, int _iLen, int _iType, int _iTime)
int playfile_read(PLAYFILE *_PlayId, void *_buf, int _iLen, int _iType, S_FrameHeader **_ptFrameHeader, S_FileHeader **_ptFileHeader, int *_piChgFile)
{
	TPlayFile *ptPlayFile = _PlayId;
	Tlist *ptList;
	int iRet;
	if(NULL == ptPlayFile)
	{
		return td_failure;
	}
	ptList = ptPlayFile->m_ptList;

	while(1)
	{
		if((long long)ptPlayFile->m_iStopTime * 1000 <= ptPlayFile->m_llCurTm)
		{
			return td_failure;
		}
		if(NULL == playfile_open(_PlayId, NULL, _piChgFile))
		{
			return td_failure;
		}
		iRet = td_failure;
		if(0 != _iType)
		{
			iRet = playfile_readIFrame(_PlayId, _buf, _iLen);
		}
		td_printf(0, "xxxxxxxxxx %s, %d, %d, time = %d", __func__, __LINE__, (int)ftell(ptPlayFile->m_fp), time(NULL));
		if(td_failure == iRet)
		{
			iRet = playfile_readOrder(_PlayId, _buf, _iLen);
		}
		if(iRet != td_failure)
		{
			if(NULL != _ptFrameHeader)
			{
				*_ptFrameHeader = &ptPlayFile->m_FrameHeader;
			}
			if(NULL != _ptFileHeader)
			{
				*_ptFileHeader = &ptPlayFile->m_FileHeader;
			}
			break;
		}
	}

	return iRet;
}

int playfile_seek(PLAYFILE *_PlayId, int _pos, int _iTime, int *_iReTime, S_FileHeader **_ptFileHeader)
{
	TPlayFile *ptPlayFile = _PlayId;
	Tlist *ptList;
	Tnode *ptNode;
	TFile *ptFile = NULL;	
	int iTime = _iTime;
	int iRemainTms = 0;
	int iSeek = 1;
	//读列表
	if(_pos >= 0 && _pos <= 100000)
	{
		iTime = (long long)(ptPlayFile->m_iStopTime - ptPlayFile->m_iStartTime) * _pos / 100000 + ptPlayFile->m_iStartTime;
		td_printf(0,"%s, %d, iTime = %d, ptPlayFile->m_iStopTime = %d,  ptPlayFile->m_iStartTime = %d, _pos =%d", __func__, __LINE__, iTime, ptPlayFile->m_iStopTime,  ptPlayFile->m_iStartTime, _pos);
	}
	ptList = ptPlayFile->m_ptList;
	for(ptNode = ptList->m_tHead; NULL != ptNode; ptNode = ptNode->m_pNext)
	{
		ptFile = (TFile *)(ptNode->m_tdata->m_data);
		if(ptFile->m_iStartTime <= iTime && ptFile->m_iEndTime > iTime)
		{
			break;
		}
		if(ptFile->m_iStartTime > iTime)
		{
			iSeek = 0;
			break;
		}
	}
	//定位
	if(NULL == ptFile)
	{
		return td_failure;
	}
	ptPlayFile->m_iOpenId = CUR_FILE;
	if(NULL == playfile_open(_PlayId, ptNode, NULL))
	{
		return td_failure;
	}
	if(0 == iSeek)
	{
		goto leave;
	}
	if((0 == ptPlayFile->m_iIFrameNum) || (ERR_REC_SUCCESSFUL != rec_SeekFile(ptPlayFile->m_fp, iTime,&iRemainTms)))
	{
		int iTemp;
		int iFilePos = 0;
		int iFileLen = 0;
		int iChno;
		struct stat stStat;
		unsigned char cTmpPath[FULL_PATH_LEN] = {0};
		rec_CompleteFileName((td_s8 *)ptPlayFile->m_File, (td_s8 *)cTmpPath, NULL, NULL);
		if (!rec_Stat((char *)cTmpPath, &stStat))
		{
			iFileLen = stStat.st_size;
		}

		if(-1 != (iChno = rec_IsCurrentFile(ptPlayFile->m_fp)))
		{
			iFileLen = rec_GetCurFileSize(iChno);
		}

		iTemp = ptFile->m_iEndTime - ptFile->m_iStartTime;
		iFilePos = (0 == iTemp) ? 0 : ((long long)(iTime - ptFile->m_iStartTime) * iFileLen / iTemp);
		iFilePos = (iFilePos < sizeof(S_FileHeader) ? sizeof(S_FileHeader) : iFilePos);
		iFilePos = (iFilePos > iFileLen ? iFileLen : iFilePos);
		fseek(ptPlayFile->m_fp, iFilePos - sizeof(S_FileHeader), SEEK_SET);
		if(rec_SeekIFrame(ptPlayFile->m_fp) < 0)
		{
			if(ftell(ptPlayFile->m_fp) < sizeof(S_FileHeader))
			{
				fseek(ptPlayFile->m_fp, sizeof(S_FileHeader), SEEK_SET);
			}
		}
		iRemainTms = 0;
	}
	if(iRemainTms > 10)
	{
		iRemainTms = 10;
	}
	
	if(iTime - iRemainTms < ptPlayFile->m_iStartTime)
	{
		ptPlayFile->m_llCurTm = (long long)ptPlayFile->m_iStartTime * 1000;
	}
	else
	{
		ptPlayFile->m_llCurTm = (long long)(iTime - iRemainTms) * 1000;
	}
	
leave:
	ptPlayFile->m_iPassFlag = 0;
	if(NULL != _iReTime)
	{
		*_iReTime = iRemainTms;
	}
	if(NULL != _ptFileHeader)
	{
		*_ptFileHeader = &ptPlayFile->m_FileHeader;
	}
	td_printf(0,"ptPlayFile->m_llCurTm = %lld", ptPlayFile->m_llCurTm);
	return td_success;
}

int playfile_GetCmd(PLAYFILE *_PlayId, int _iCmd, void *_ptValue)
{
	int iRet = td_failure;
	TPlayFile *ptPlayFile = _PlayId;
	td_printf(0,"ptPlayFile->m_llCurTm = %lld, ptPlayFile->m_iStartTime = %d, ptPlayFile->m_iStopTime = %d", ptPlayFile->m_llCurTm, ptPlayFile->m_iStartTime, ptPlayFile->m_iStopTime);
	switch(_iCmd)
	{
		case 0:	
			{
				*((int *)_ptValue) = (long long)(ptPlayFile->m_llCurTm / 1000 - ptPlayFile->m_iStartTime) * 100000/ (ptPlayFile->m_iStopTime - ptPlayFile->m_iStartTime);
				if(*((int *)_ptValue)  > 100000)
				{
					*((int *)_ptValue)  = 100000;
				}
				iRet = td_success;	
			}
			break;
		case 1:
			{
				*((int *)_ptValue) = ptPlayFile->m_llCurTm;
				iRet = td_success;	
			}
			break;
		case 2:
			{
				*((int *)_ptValue) = (1 == ptPlayFile->m_iPassFlag) ? 1 : 0;
				iRet = td_success;
			}
		default :		break;
	}
	return iRet;
}

int playfile_SetCmd(PLAYFILE *_PlayId, int _iCmd, void *_ptValue)
{
	int iRet = td_failure;	
	TPlayFile *ptPlayFile = _PlayId;	
	switch(_iCmd)
	{
		case 0:	
			{
				int iPassLen;
				char *pass = (char *)_ptValue;
				char tmp[64];
				iPassLen = strlen(pass);
				if(iPassLen == 0 || iPassLen > ENCRYPT_PWD_LENGTH)
				{
					return td_failure;
				}

				if(0 == ptPlayFile->m_iPassFlag)
				{
					strncpy(ptPlayFile->m_cPassWord, pass, iPassLen);
					return td_success;
				}
				memcpy(tmp, ptPlayFile->m_cPass, 64);
				aes_crypt((unsigned char *)tmp, 64, ptPlayFile->m_FrameHeader.u32FrameNO, (unsigned char *)pass, iPassLen);
				if(((int *)tmp)[0] != 0X1000000)
				{
					return td_failure;
				}
				strncpy(ptPlayFile->m_cPassWord, pass, iPassLen);
				ptPlayFile->m_cPassWord[iPassLen] = 0;
				ptPlayFile->m_iPassFlag = 2;//第一次被解除
				iRet = td_success;	
			}
			break;
		default :		break;
	}
	return iRet;
}
