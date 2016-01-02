
#ifndef  _RTMSG_H__
#define  _RTMSG_H__

#include <pthread.h>

#define     TD_ERROR            	-1
#define		TD_OK					0
#define     MAX_MODULE_NUM    	    20
#define		MAX_FILENAME_LEN		20
#define     MAX_MSG_LEN				12 * 1024 //huaqiang mod 20120611

// -------------------------   interface   ----------------------------

int RtMsg_Reg(char * _strName, int (*_getmsg)(char * _strBuf, int _iBufSize));

#ifdef MASTER_CHIP
int RtMsg_WriteLog(int _iLevel, char * _strFormat, ...);
#else
#define RtMsg_WriteLog(_iLevel, _strFormat, ...) 
#endif

//int RtMsg_Start();

int RtMsg_WriteProcMsg(void);

int RtMsg_Init(char * _strPath, int _iMaxSize);

void RtMsg_UnInit(void);


#endif//rtmsg.c
