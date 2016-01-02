#ifndef _PTZ_EX_H_
#define _PTZ_EX_H_

#define MAX_TIME_DELAY 300 * 1000

typedef struct STRUCT_PTZ_DATA{
	int iAddress;
	int iPreset;
	int iSpeedX;
	int iSpeedY;
	int iCount;
	char DecBuf[32];
}PTZ_DATA;

int ptzex_Init();
int ptzex_Control(int _iChn, td_s32 _iAction , PTZ_DATA* _pData);
int ptzex_StopRoutine(int _iChn, int _iRoutineNo);
int ptzex_StartRoutine(int _iChn, int _iRoutineNo);
int ptzex_StopCruise(int _iChn, int _iCruiseNo);
int ptzex_StartCruise(int _iChn, int _iCruiseNo);
int ptzex_get_Routine_stat(int _iChn);
int ptzex_get_Cruise_stat(int _iChn);
#endif

