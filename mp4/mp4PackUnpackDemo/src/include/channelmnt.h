// 通道管理层
// duxiulong, 2011/03/01 create

#ifndef _CHANNEL_MNT_H_
#define _CHANNEL_MNT_H_

#include "public_def.h"
#include "td_types.h"
#include "channel.h"

// 通道管理模块初始化

typedef td_s32 (*ChannelMsgCallBack)(td_s32 _iChn, td_s32 _iMsgId, td_s32 _iSubId, void *_ptMsg);


td_s32 chnmnt_Init(td_s32 _iMaxChn, TChnModuleInitParam *_ptInitParm,
	ChannelMsgCallBack _MsgCallBack, 
	ChnAlarmerRegisterCallBack _AlarmerRegCallBack,
	ChnStreamCryptCallBack _StreamCryptCallBack);
td_s32 chnmnt_Uninit();

td_s32 chnmnt_SetBncBits(td_s32 _uiBncBits);

// 通用回调函数设置
td_s32 chnmnt_SetCallBack(td_s32 _iCid, void *_CallBack);

// 创建通道
// 参数说明:
// _iChn :	逻辑通道号
// _pCnf :	通道配置参数
// 返回值: 0 成功, -1失败
td_s32 chnmnt_CreateChannel(td_s32 _iChn, TChnInitParam *_ptCnf);

td_s32 chnmnt_DestroyChannel(td_s32 _iChn);

// 通道号检测
// 判断通道号是否为主码流，是则返回主码流通道号，否则返回-1
td_s32 chnmnt_IsMainChnNo(td_s32 _iChn);
// 判断通道号是否为副码流，是则返回主码流通道号，否则返回-1
td_s32 chnmnt_IsSubChnNo(td_s32 _iChn);

// 基本视频参数设置
td_s32 chnmnt_ChannelSetVideoSize(td_s32 _iChn, td_s32 _iSize);
td_s32 chnmnt_ChannelSetProfile(td_s32 _iChn, td_s32 _iProfile);

td_s32 chnmnt_ChannelSetVideoNorm(td_s32 _iChn, td_s32 _iNorm);
td_s32 chnmnt_ChannelSetVencType(td_s32 _iChn, td_s32 _iVencType);
td_s32 chnmnt_ChannelSetVencFrameRate(td_s32 _iChn, td_s32 _iFrameRate);
td_s32 chnmnt_ChannelSetVencIFrameRate(td_s32 _iChn, td_s32 _iIFrameRate);
td_s32 chnmnt_ChannelSetVencBitType(td_s32 _iChn, td_s32 _iBitType);
td_s32 chnmnt_ChannelSetVencBitRate(td_s32 _iChn, td_s32 _iBitRate);
td_s32 chnmnt_ChannelSetVencQuality(td_s32 _iChn, td_s32 _iQuality);
td_s32 chnmnt_ChannelSetVencStreamType(td_s32 _iChn, td_s32 _iStreamType);
td_s32 chnmnt_ChannelSetAencType(td_s32 _iChn, td_s32 _iAencType);
td_s32 chnmnt_ChannelSetVideoInParam(td_s32 _iChn, const TChnViCnf *_ptParam);
td_s32 chnmnt_ChannelSetVideoColor2GrayParam(td_s32 _iChn, td_s32 _iValue);
td_s32 chnmnt_ChannelSetAudioVol(td_s32 _iChn, td_s32 _iVol);
td_s32 chnmnt_ChannelSetAudioSample(td_s32 _iChn, td_s32 _iSample);
//add by gyn 20120201
td_s32 chnmnt_ChannelGetAudioStat(td_s32 _iChn); //获取音频状态(1有音频,0无音频)
//add end
td_s32 chnmnt_ChannelGetChnStat(td_s32 _iChn);
// 数字通道特殊参数设置
td_s32 chnmnt_ChannelSetIpChannelMsg(td_s32 _iChn, TIpChannelMsg *_ptMsg);
// VO合成通道特殊参数设置
td_s32 chnmnt_ChannelSetVcChannelMsg(td_s32 _iChn, TVcChannelMsg *_ptMsg);
// 设置混音(暂只支持VO合成通道, 支持两路音频混音)
td_s32 chnmnt_ChannelSetMixAudioChn(td_s32 _iChn, td_s32 _iAudioChn1, td_s32 _iAudioChn2);
// 设置混音(暂只支持VO合成通道, 支持三路音频混音)  add by nsg 20120203
td_s32 chnmnt_ChannelSetMixAudioChnEx(td_s32 _iChn, TVcMixAudioChnMsg _tChnMsg);
//预览裁片 wsl add 20120905
td_s32 chnmnt_ChannelSetClipRect(td_s32 _iChn, const TRitioRect *_ptRitioRect);

//获取音频码流 liuqi add 2014-11-4
TRingBuf *ChnAttachAudioStreamEx(td_s32 _iChn);
td_s32 ChnDetachAudioStreamEx(TRingBuf * _ptBuf);

// 视频加密解密
// 密码传NULL表式停止加密解密功能
td_s32 chnmnt_ChannelSetStreamEncrypt(td_s32 _iChn, td_u8 *_strPsw);
td_s32 chnmnt_ChannelSetStreamDecrypt(td_s32 _iChn, td_u8 *_strPsw);

// OSD叠加
td_s32 chnmnt_ChannelSetNameOsdEnable(td_s32 _iChn, td_s32 _iEnable);
td_s32 chnmnt_ChannelSetNameOsdParam(td_s32 _iChn, const TChnNameOsdCnf *_ptParam);
td_s32 chnmnt_ChannelSetNameOsdDsp(td_s32 _iChn, const TOsdDspCnf *_ptCnf);
td_s32 chnmnt_ChannelSetNameOsdString(td_s32 _iChn, const td_u8 *_strText);
td_s32 chnmnt_ChannelSetNameOsdColor(td_s32 _iChn, td_u32 _uiColor);


td_s32 chnmnt_ChannelSetTimeOsdEnable(td_s32 _iChn, td_s32 _iEnable);
td_s32 chnmnt_ChannelSetTimeOsdParam(td_s32 _iChn, const TChnTimeOsdCnf *_ptParam);
td_s32 chnmnt_ChannelSetTimeOsdDsp(td_s32 _iChn, const TOsdDspCnf *_ptCnf);
td_s32 chnmnt_ChannelSetTimeOsdFormate(td_s32 _iChn, td_s32 _iFrt);
td_s32 chnmnt_ChannelSetTimeOsdColor(td_s32 _iChn, td_u32 _uiColor);


td_s32 chnmnt_ChannelSetTextOsdEnable(td_s32 _iChn, td_s32 _iEnable);
td_s32 chnmnt_ChannelSetTextOsdParam(td_s32 _iChn, const TChnTextOsdCnf *_ptParam);
td_s32 chnmnt_ChannelSetTextOsdDsp(td_s32 _iChn, const TOsdDspCnf *_ptCnf);
td_s32 chnmnt_ChannelSetTextOsdString(td_s32 _iChn, const td_u8 *_strText);
td_s32 chnmnt_ChannelSetTextOsdColor(td_s32 _iChn, td_u32 _uiColor);
td_s32 chnmnt_ChannelSetTextFontSize(td_s32 _iChn, TChnOsdSizeCnf* _pstCnf);



td_s32 chnmnt_ChannelSetLogoOsdEnable(td_s32 _iChn, td_s32 _iEnable);
td_s32 chnmnt_ChannelSetLogoOsdParam(td_s32 _iChn, const TChnLogOsdCnf *_ptParam);

// 移动侦测
td_s32 chnmnt_ChannelSetMdEnable(td_s32 _iChn, td_s32 _iEnable);
td_s32 chnmnt_ChannelSetMdSense(td_s32 _iChn, td_s32 _iSense);
td_s32 chnmnt_ChannelSetMdArea(td_s32 _iChn, const TChnMdAreaCnf *_ptArea);

// 视频丢失报警
td_s32 chnmnt_ChannelSetVideoLostEnable(td_s32 _iChn, td_s32 _iEnable);

// 视频遮挡区域
td_s32 chnmnt_ChannelSetVideoCoverEnable(td_s32 _iChn, td_s32 _iEnable);
td_s32 chnmnt_ChannelSetVideoCoverArea(td_s32 _iChn, const TChnVideoCoverAreaCnf *_ptArea);
td_s32 chnmnt_ChannelSetOdEnable(td_s32 _iChn, td_s32 _iEnable);

// 命令
td_s32 chnmnt_ChannelStartEncode(td_s32 _iChn);
td_s32 chnmnt_ChannelStopEncode(td_s32 _iChn);
td_s32 chnmnt_ChannelForceIFrame(td_s32 _iChn);
td_s32 chnmnt_ChannelSnapshot(td_s32 _iChn, TChnSnapshotMsg *_ptMsg);

// 预览相关
td_s32 chnmnt_ChannelStartAudioPreview(td_s32 _iChn);
td_s32 chnmnt_ChannelStopAudioPreview(td_s32 _iChn);
td_s32 chnmnt_ChannelStartAudioPreviewEx(td_s32 _iChn,td_s32 _s32AodevId);
td_s32 chnmnt_ChannelStartAudioPreviewV2(td_s32 _iChn);
td_s32 chnmnt_GetPreviewAudioChn(td_s32 _iAudioChn);
td_s32 chnmnt_ChannelSetSensorSpin (td_s32 _iChn, TSensorSpin _eType);
td_s32 chnmnt_ChannelBindVO(td_s32 _iChn, void *_ptVo, TRect *_ptRect);
td_s32 chnmnt_ChannelUnBindVO(td_s32 _iChn, void *_ptVo);
td_s32 chnmnt_ChannelSetDspAreaOnVo(td_s32 _iChn, void *_ptVo, TRect *_ptRect);
td_s32 chnmnt_ChannelHidenPreviewOsd(td_s32 _iChn, void *_ptVo);
td_s32 chnmnt_ChannelDrawPreviewOsd(td_s32 _iChn, void *_ptVo, const TChnOsdCnf *_ptOsd);
td_s32 chnmnt_ChannelCheckPreview(td_s32 _iChn, void *_ptVo, TRect *_ptRect);
td_s32 chnmnt_ChannelGetVdecLimitPreview(td_s32 _iChn, td_s32 *_piLimit);
td_s32 chnmnt_ChannelGetVdecforVcaCost(td_s32 _iChn, td_s32 *_piCost);		//zhangyi add 20130617
td_s32 chnmnt_GetVdecforVcaCost();
td_s32 chnmnt_ChannelEncodePerformanceCost();
td_s32 chnmnt_ChannelSwitchPreviewStream(td_s32 _iChn, td_s32 _arg);
// 云镜控制
td_s32 chnmnt_ChannelPtz(td_s32 _iChn, TComInstance *_ptCom, TComAttribute *_ptAttribute, td_s32 _iDataLen, td_u8 *_pu8Data, TPtzCtrProtocolData *_ptProtocol);

// 性能计算
td_s32 chnmnt_ChannelPerformanceCost(td_s32 _iChn, td_s32 _iPid);
td_s32 chnmnt_ChannelNetCost(td_s32 _iChn, td_s32 _iPid);


// 参数获取接口
td_s32 chnmnt_ChannelGetVencSize(td_s32 _iChn, TVideoSize *_ptSize);

// 数字通道搜索
td_s32 chnmnt_DoIpcSearch(td_s32 _iType, ChnIpcSearchReportCallBack _ReportCallback);

td_s32 chnmnt_DoIpChange(void * _tChgIp);

// 数字通道设置注册中心
td_s32 chnmnt_SetIpcRegCenter(TRegCenterMsg *_ptRegCenter);

// 获取数字通道连接状态信息
td_s32 chnmnt_GetIpcList(char *_strBuf);
// 数字通道时间同步
td_s32 chnmnt_IpcTimeSync();
//数字通道时区同步
td_s32 chnmnt_IpcTimeZoneSync(td_s32 *_iTimeZone);

//解码器与前端设备启动对讲
td_s32 chnmnt_ChannelStartIpChannelTalk(td_s32 _iChn,TIpTalkPara* _ptParam);

//解码器与前端设备停止对讲
td_s32 chnmnt_ChannelStopIpChannelTalk(td_s32 _iChn);

//解码器通过数字通道push流操作
td_s32 chnmnt_ChannelIpChannelPushData(td_s32 _iChn,TPushData* _ptPushData);

// 预览通道焦点变更广播
td_s32 chnmnt_PreviewFocusChgBroadcast(td_s32 _iVoDev, td_s32 _iChn,td_s32 _iChncnt);//modify by zhuangzhibing

//td_s32 chnmnt_PreviewFocusChgBroadcast(td_s32 _iVoDev, td_s32 _iChn);
// 通道音频类型变化广播
td_s32 chnmnt_ChannelAvecnTypeChgBroadcast(td_s32 _iChn);

// 获取通道使能状态
int chnmnt_GetChannelEnable(td_s32 _iChn);

// 获取datacenter实例
// 返回空表式无码流, 返回-1表未启动编码,
TDataCenter *chnmnt_GetChannelDataCenter(td_s32 _iChn);
#define chnmnt_GetChannelDecodeStat(iChn)	(chnmnt_GetChannelDataCenter(iChn) > NULL && chnmnt_GetChannelDataCenter(iChn) != (void *)(-1))

// 获取通道实列(不建议使用)
const TChannel *chnmnt_GetChannelnstance(td_s32 _iChn);


// 获取通道类型
td_s32 chnmnt_GetChannelType(td_s32 _iChn);

#define chnmnt_IsIpc(chn)		(chnmnt_GetChannelType(chn) == CHN_TYPE_IP)

td_s32 chnmnt_ChannelSetReboot(td_s32 _iChn);
td_s32 chnmnt_ChannelSetVcaAlarmStatClrt(td_s32 _iChn, td_s32 _iRule, TVcaEventType _eEventType);
td_s32 chnmnt_ChannelSetVcaCfgChn(td_s32 _iChn, 	td_s32 _iEable);
td_s32 chnmnt_ChannelSetVcaCfgAdv(td_s32 _iChn, 	const void *_pArg);
td_s32 chnmnt_ChannelSetVcaCfgTarget(td_s32 _iChn, 	const void *_pArg);
td_s32 chnmnt_ChannelSetVcaRule(td_s32 _iChn, 		const void *_pArg);
td_s32 chnmnt_ChannelSetVcaTripwire(td_s32 _iChn, 	const void *_pArg);
td_s32 chnmnt_ChannelSetVcaPerimeter(td_s32 _iChn, 	const void *_pArg);
td_s32 chnmnt_ChannelSetVcaOsc(td_s32 _iChn, 		const void *_pArg);
td_s32 chnmnt_ChannelSetVcaAvd(td_s32 _iChn, 		const void *_pArg);
//shihc add 20130323
td_s32 chnmnt_ChannelSetVcaCap(td_s32 _iChn,        const void *_pArg);
td_s32 chnmnt_ChannelSetVcaCpc(td_s32 _iChn,        const void *_pArg);
td_s32 chnmnt_ChannelSetVcaCdd(td_s32 _iChn,        const void *_pArg);
td_s32 chnmnt_ChannelSetVcaSvd(td_s32 _iChn,const TVcaParamSVD  *_pcVcaParamSvd);

td_s32 chnmnt_ChannelReCreateVCA(td_s32 _iChn);


td_s32 chnmnt_ChannelAudiosampleChgBroadcast(td_s32 _iChn);//ljj add 20121019

td_s32 chnmnt_GetPreviewChn();

td_s32 chnmnt_GetPreviewAudioLeft0();
td_s32 chnmnt_GetPreviewAudioRight0();
td_s32 chnmnt_GetPreviewAudioLeft1();
td_s32 chnmnt_GetPreviewAudioRight1();

//shihc add 20130312 获取数字通道的连接状态
td_s32 chnmnt_ChannelGetIpChnConState(td_s32 _iChn);

// 获取通道报警器实例
TAlarmer *chnmnt_GetChannelAlarmer(td_s32 _iChn, td_s32 _iType);
// 设置同一通道PTZ控制事件的优先级
td_s32 chnmnt_ChannelSetPtzPriority(td_s32 _iChn, td_s32 _ipriority);

// for decoder
td_s32 chnmnt_ChannelSetPreviewPrefer(td_s32 _iChn, td_s32 _iPreviewPrefer);
td_s32 chnmnt_ChannelGetIpcVersion (td_s32 _iChn,td_char * _cVersion);
td_s32 chnmnt_RebootChn(int _iChn);
td_s32 chnmnt_IpcReboot (int _iChn);
td_s32 chnmnt_ChannelUpgrade(int _iChn, const char* _cFileName);
td_s32 chnmnt_ChannelUpgradeSta(int _iChn,int *_piRate);

//设置音视频拼接使能
int chnmnt_ChannelSetMedleyAvEnable(int _iChn, int _iEnable);
// 设置语音激励状态
int chnmnt_ChannelSetBestirRsp(int _iChn, int _iStat);

//启动音频流线程
td_s32 chnmnt_ChannelStartAudioStream();
TRingBuf* chnmnt_ChannelGetVitalDataRingbuf(td_s32 _iChn);

#endif
