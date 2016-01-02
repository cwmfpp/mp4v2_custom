/*****************************************************************************
 * NAME         : common.h
 * FUNCTION     : MP4  转换回放相关公共头文件
 * PROGRAMMED   : 张胜
 * DATE         : 2014-12-10
 * PROJECT      : 
 * OS           : Linux
 ****************************************************************************/  
#ifndef _COMMON_INCLUDED_
#define _COMMON_INCLUDED_

#ifdef ALONE
#include "public_def.h"

#ifndef FILENAMEMAXNUM
#define FILENAMEMAXNUM	128
#endif
#ifndef MAX_FILE_BUF
#define MAX_FILE_BUF		(1024 * 1024)
#endif

#ifndef MAX_OPEN_FILE
#define MAX_OPEN_FILE       50		//最多能同时打开的文件个数
#endif

int td_printf_alone(unsigned int handle, char* pszfmt, ...);
int Common_FileName2FileType(td_u8 * _pcFileName);
int Common_IsValidFileHeader(S_FileHeader* _header);
td_s32 Common_IsValidFileName(td_u8* _pucFileName);
td_s32 Common_Stat(char * _pcFilePath, void *_pstStat);
td_s32 Common_CompleteFileName(td_s8* _pcFileName, td_s8* _pcFullPath, td_s8* _pcPath, td_s8* _pcShortPath);

#define IsValidFileHeader Common_IsValidFileHeader
#define rec_Stat Common_Stat
#define rec_CompleteFileName Common_CompleteFileName
#endif
#endif
