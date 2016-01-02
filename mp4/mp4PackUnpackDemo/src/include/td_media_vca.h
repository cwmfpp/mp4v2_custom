
#ifndef _TD_MEDIA_VCA_H
#define _TD_MEDIA_VCA_H

#include "vca.h"
#include "vca_head.h"
#include "libdatacenter.h"
#include "hi_tde_type.h"

#define VCA_DRAW_WIDTH_MAX	(2560)//(1920)
#define VCA_DRAW_HEIGHT_MAX	(2048)//(1080)
#define VCA_SCALE_STRIDE	384
#define VCA_SCALE_WIDTH 	vca_GetAnalyseWidth()
#define VCA_SCALE_HEIGHT 	vca_GetAnalyseHeight()
#define VCA_FRAME_DEPTH 	5
#define VCA_CHN_MAX			2
#define VCA_VENC_CHN_1		16
#define VCA_VENC_CHN_2		17
#define VCA_VDEC_CHN_1		59
#define VCA_VDEC_CHN_2		60


typedef enum
{
	VCA_TYPE_LOCAL = 0,	
	VCA_TYPE_IP,		
	VCA_TYPE_UNDEF,		
}EVcaChnType;

typedef struct
{
	td_s32 m_used;
	td_s32 m_chn_id;
}TChnInfo;


td_s32 vca_SetAnalyseSize(td_s32 _iAnalyseWidth, td_s32 _iAnalyseHeight);
td_s32 vca_GetAnalyseWidth();
td_s32 vca_GetAnalyseHeight();
td_s32 vca_ipchn_run(int _chn);
td_s32 vca_InitChnInfo(td_s32 _iChnNum);
int vca_start_local_channel(int _chn);
//td_s32 vca_SetIpcStreamCrypt(td_s32 _iChn, td_u8 *_pStreamCryptPsw);
td_s32 vca_ChnStreamCrypt(td_s32 _iChn, td_u8 *ucVdecPsw, TDataPackage *_pPackageTmp, S_FrameHeader *_ptFrmTmp);
td_s32 vca_VdecSendStream(td_s32 _iChn, TDataPackage *_pPackage);
td_s32 vca_SendAudioDataPackage(td_s32 _iChn, TDataPackage *_pPackage);
td_s32 vca_ForceIFrame(td_s32 _iChn);
TDataPackage * vca_GetDataPackage(td_s32 _iChn);
//td_s32 vca_GetDCNum(td_s32 _iChn);


td_s32 vca_GetWorkState(td_s32 _iVcaChn);
void vca_set_draw_line_method(int _chn,int _method);

td_s32 vca_StartProess(td_s32 _iIpVcaChn, td_s32 _iVdecChn, td_s32 _iVencChn);
td_s32 vca_StopProess(td_s32 _iIpVcaChn);
td_s32 vca_StartIpVcaChn(td_s32 _iIpVcaChn, td_s32 _iVdecChn, td_s32 _iVencChn);
td_s32 vca_StopIpVcaChn(td_s32 _iIpVcaChn);
td_s32 vca_StartLocalVcaChn(td_s32 _iLocalVcaChn);
td_s32 vca_StopLocalVcaChn(int _chn);
td_s32 vca_CreateVdecChannel(td_s32 _iVW, td_s32 _iVH);
td_s32 vca_DestroyVdecChannel(td_s32 _iChn, td_s32 _iVW, td_s32 _iVH);
td_s32 vca_CreateVencChannel(void *_pBuf, td_s32 _iVW, td_s32 _iVH);
td_s32 vca_DestroyVencChannel(td_s32 _iChn, td_s32 _iVW, td_s32 _iVH);

td_s32 vca_SetQuality(td_s32 _s32VencChn,td_s32 _s32Quality);
td_s32 vca_SetBitrate(td_s32 _s32VencChn,td_s32 _s32Bitrate);
td_s32 vca_SetCVBRType(td_s32 _s32VencChn, td_s32 _s32CVBRType, td_s32 _s32Quality);
td_s32 vca_SetIFrameRate(td_s32 _s32VencChn, td_s32 _s32IFrameRate);
td_s32 vca_SetFrameRate(td_s32 _s32VencChn, td_s32 _s32IFrameRate);

td_s32 vca_TdeOptPic(VIDEO_FRAME_S *_pstSrcData, td_u32 _u32DstPhyAddr1, td_s32 _u32DstPhyAddr2);

td_s32 vca_GetFifoPackNum(td_s32 _iChn, td_s32 * _piVNum, td_s32 * _piANum);
td_s32 vca_ClearFifo(td_s32 _iIpVcaChn);
td_s32 vca_ResetFifo(td_s32 _iIpVcaChn, td_s32 _iVdecChn, td_s32 _iVencChn);
#endif

