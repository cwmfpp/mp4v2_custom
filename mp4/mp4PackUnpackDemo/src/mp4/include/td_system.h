// ******************************************************************
// File		: td_system.h
// Fuction 	: 3511/3520在多线程环境下调用system失败，
//			  本程序模拟system与popen功能
// Author	: duxiulong
// Date		: 2010/02/02
// *******************************************************************

#ifndef _TD_SYSTEM_H_
#define _TD_SYSTEM_H_

#include <stdio.h>

#define TD_SYSTEM_ACTIVE	1

#if TD_SYSTEM_ACTIVE
int TdSystemInit();
void TdSystemUninit();

int TdSystem(const char *_strCmd);
FILE *TdPipeOpen(const char *_strCmd, const char *_strType);
int TdPipeClose(FILE * _ptHnd);

#else
#include <stdlib.h>

#define TdSystemInit() 		0
#define TdSystemUninit
#define TdSystem			system
#define TdPipeOpen		popen
#define TdePipeClose		pclose
#endif

#endif

