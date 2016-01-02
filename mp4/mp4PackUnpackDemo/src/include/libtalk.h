#ifndef _LIB_TALK_H
#define _LIB_TALK_H

#include "public_def.h"

#define SINGLE_TALK_START	0
#define SINGLE_TALK_STOP	1

//typedef td_s32 (*SendAudioStream)(const td_u8 *_pBuf, td_u32 _iLen);
//单向对讲
td_s32 StartSingleTalking(td_s32 _iAOpreviewDefault);
td_s32 StartSingleTalkingPCM(td_s32 _iAOpreviewDefault);
//td_s32 SendSingleTalkingData(const td_u8* _cAudioData, td_u32 _iDataLen);
td_s32 SendSingleTalkingData(const td_u8* _cAudioData, td_u32 _iDataLen,int _itype);//modify by zhb 20121030 
td_s32 StopSingleTalking();
//单向对讲心跳，_iFlg:心跳清0，其他值:心跳+1并做超时处理
td_s32 SingleTalkingHeartbeat(td_s32 _iFlg);


//双向对讲
TDataCenter* StartDoubleTalking(td_s32 s32AencType,td_s32 s32PtNumPerFrm);
td_s32 StopDoubleTalking();
td_s32 InitTalk();
td_s32 GetDoubleTalkStat();
td_s32 SetIpTalkChn(int _iChn);
td_s32 GetIpTalkChn();

#endif
