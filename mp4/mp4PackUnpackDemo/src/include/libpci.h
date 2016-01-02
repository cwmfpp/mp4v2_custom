#ifndef _TD_PCI_H_
#define _TD_PCI_H_

#include "public_def.h"
#include "td_types.h"

typedef int (*PCI_RECVCMD)(int _iTid, unsigned char* _ucBuf,int _iBufLength);    //通用命令回调函数
typedef int (*PCI_RECVCOM)(int _iTid, td_s32 _iSubComNo, unsigned char* _ucBuf,int _iBufLength);  //串口数据回调函数
typedef int (*PCI_RECVSTREAM)(int _iTid, int _iChn, td_u8* _pcData, int _iDataLen, int _iFrameType);  //码流回调函数
typedef int (*PCI_RECVJPEG)(int _iTid,int _iChn, td_u8* _pcData, int _iDataLen, int _iType);		 //抓拍回调函数

//td_s32 td_pci_init(td_s32 _s32ChnNum[], td_s32 _s32VideoNorm);
td_s32 td_pci_initPort(td_s32 _s32ChnNum[], td_s32 _s32VideoNorm);
td_s32 td_pci_initFunc(void);
/****************************************
	功能:	pci初始化函数，分别完成通信端口、线程及功能初始化两部分工作	
	参数:	
			_s32ChnNum:含有从片编码路数信息的数组，下标1对应第一个从片，类推
			_s32VideoNorm: 视频输入制式

	输出:

			
	修改日志:20130417 hq changed

*****************************************/

//码流和抓拍回调函数放到创建通道时实现
//td_s32 td_pci_SetAVCbk(PCI_RECVSTREAM _cbkRecvStream, PCI_RECVJPEG _cbkRecvJpeg);
/****************************************
	功能:	设置视频及抓拍回调函数	
	参数:	
			_cbkRecvStream: 视频回调函数
			_cbkRecvJpeg : 抓拍回调函数

	输出:

			
	修改日志:20110311 高旭麟创建
*****************************************/

td_s32 td_pci_SetCOMCbk(PCI_RECVCOM _cbkRecCom);
/****************************************
	功能:	设置串口回调函数	
	参数:	
			_cbkRecCmd: 命令回调函数

	输出:

			
	修改日志:20110311 高旭麟创建
*****************************************/

td_s32 td_pci_SetCMDCbk(PCI_RECVCMD _cbkRecCmd);
/****************************************
	功能:	设置命令回调函数	
	参数:	
			_cbkRecCmd: 命令回调函数

	输出:

			
	修改日志:20110311 高旭麟创建
*****************************************/

//td_s32 td_pci_SendCMD(td_s32 _s32TID, char* _pcMsg);
/****************************************
	功能:	通用命令发送函数
	参数:	
			_s32TID 从片id
			_pcMsg: 命令体

	输出:

			
	修改日志:20110311 高旭麟创建
*****************************************/

td_s32 pci_Host_GetPicSize(TRect *_pRect);
td_s32 pci_Host_GetPcivChnAttr(td_s32 _s32PcivChn, void* _stPcivAttr);
td_s32 pci_Host_CreatePcivChn(td_s32 _s32Index, td_s32 _s32LocPcivChn, td_s32 _s32RemotePcivChn, TRect *_pRect);
td_s32 pci_Host_DestroyPcivChn(td_s32 _s32PcivChn);
td_u8 shm_SetVlState(td_s32 _s32Index, td_s32 _s32Offset, td_s32 _s32Value);
/*************************************************************************
	功能: 设置视频丢失报警状态
	参数: _s32Index   目标芯片标识
			_s32Offset 偏移量(通道数)
			_s32Value 设置状态值
**************************************************************************/

td_s32 master_set_alarm_video_lost(td_s32 _s32Index, const td_s32 _s32Chn,const td_s32 _s32Enable); 		
td_s32 master_set_video_size(td_s32 _s32Index, const td_s32 _s32Chn, const td_s32 _s32Size);
td_s32 master_set_video_profile(td_s32 _s32Index, const td_s32 _s32Chn, const td_s32 _s32Profile);
td_s32 master_set_clip_side(td_s32 _s32Index, const td_s32 _s32Chn,TRect *_ptRect);

td_s32 master_start_rec(td_s32 _s32Index, const td_s32 _s32Chn,void* _vencAttr);
td_s32 master_set_force_iframe(td_s32 _s32Index, const td_s32 _s32Chn); 
td_s32 master_heart_beat(td_s32 _s32Index, THeartBeat _eType);
td_s32 master_set_bitrate(td_s32 _s32Index, const td_s32 _s32Chn,const td_s32 _s32Bitrate); 
td_s32 master_set_frame_rate(td_s32 _s32Index, const td_s32 _s32Chn, const td_s32 _s32Framerate);
td_s32 master_set_iframe_rate(td_s32 _s32Index, const td_s32 _s32Chn,const td_s32 _s32Iframerate);	

//zhangyi modify 20111125
//td_s32 master_set_cover_area(td_s32 _s32TID, const td_s32 _s32Chn,Rect_t* _pRect, const td_s32 _s32Num);	
td_s32 master_set_cover_area(td_s32 _s32Index, const td_s32 _s32Chn,TRect* _pRect, const td_s32 _s32Num, const td_s32 _s32VideoSize);

td_s32 master_set_dateformat(td_s32 _s32Index, td_s32 _iFormat, td_u8* _pcSeparater);	//Added by Leeon Tonie 090206.
td_s32 master_set_osd_text(td_s32 _s32Index, const td_s32 _s32Chn, td_s8* const _pText);	
td_s32 master_set_osd_display(td_s32 _s32Index, const td_s32 _s32Chn,const td_s32 _s32Distype,const td_s32 _s32X,const td_s32 _s32Y,const td_s32 _s32Enable);		
td_s32 master_set_wordoverlay(td_s32 _s32Index, td_s32 _s32Chn, td_s32 _s32X, td_s32 _s32Y, td_u8* _pText);
td_s32 master_set_test( td_s32 _s32Index);
int master_parse_cmd(td_s32 _s32Index, unsigned char* cmdbuf);
td_s32 master_set_init( td_s32  _s32Index, td_s32 _s32ChnCount, td_s32 _s32VideoMode, td_s32 _s32ProductModel, td_bool _bSlave, td_s32 _ChipNum);
td_s32 master_set_time(td_s32 _s32Index, td_s32 _s32Time);
td_s32 master_set_timeEx(td_s32 _s32Index);
td_s32 master_set_encoder_type(td_s32 _s32Index, const td_s32 _s32Chn,const td_s32 _s32Encodertype);		
td_s32 master_set_motion_area(td_s32 _s32Index, const td_s32 _s32Chn,const td_s32 _s32Area[18]);		
td_s32 master_set_motion_sen(td_s32 _s32Index, const td_s32 _s32Chn,const td_s32 _s32Sen);				
td_s32 master_set_motion_enable(td_s32 _s32Index, const td_s32 _s32Chn,const td_s32 _s32Enable);	
td_s32 master_set_colortogray(td_s32 _s32Index,td_s32 _s32Chn, td_s32 _s32ColorToGray); 
td_s32 master_set_video_quality(td_s32 _s32Index, const td_s32 _s32Chn,const td_s32 _s32Quality);	
td_s32 master_set_stream_type(td_s32 _s32Index,const td_s32 _s32Chn,const td_s32 _s32Streamtype);	
td_s32 master_set_audio_coder(td_s32 _s32Index, td_s32 _s32Chn,const td_s32 _s32Coder,td_s32 _sample );
td_s32 master_set_channel_param(td_s32 _s32Index, const td_s32 _s32Chn,const td_s32 _s32Brightness,const td_s32 _s32Contrast,const td_s32 _s32Saturation,const td_s32 _s32Hue); 
td_s32 master_capture_picture(td_s32 _s32Index, td_s32 _s32Chn, const td_u8* _pu8FileName);
td_s32 master_com_send_data(td_s32 _s32Index, td_s32 _s32Comnum, td_u8* _pu8Buf,td_s32 _s32Len);
td_s32 master_set_com_para(td_s32 _s32Index, const td_s32 _s32Comnum, const td_s32 * _ps32Para, const td_s32 _s32Len);
td_s32 master_stop_rec(td_s32 _s32Index, const td_s32 _s32Chn); 
td_s32 master_reset_timeosd(td_s32 _s32Index, td_s32 _s32Chn);
td_s32 master_start_aenc(td_s32 _s32Index, const td_s32 _s32AudioChn,const td_s32 _s32VideoChn, td_s32 _s32AudioType,td_s32 _sample);
td_s32 master_stop_aenc(td_s32 _s32Index, const td_s32 _s32Chn);
td_s32 master_reboot_slave(td_s32 _s32Index);
td_s32 check_slave_heartbeat();
td_s32 master_set_textcolor(td_s32 _s32Index, const td_s32 _s32Chn, td_s32 _s32Color);
td_s32 mastrer_set_timecolor(td_s32 _s32Index, const td_s32 _s32Chn, td_s32 _s32Color);
td_s32 mastrer_set_overlaycolor(td_s32 _s32Index, const td_s32 _s32Chn, td_s32 _s32Color);

//设置叠加字符字体大小函数
td_s32 mastrer_set_overlayFontSize(td_s32 _s32Index, const td_s32 _s32Chn, td_s32 _s32Size);

td_s32 master_set_bitmapoverlay(td_s32 _s32Index, const td_s32 _s32Chn, td_s32 _iWidth, td_s32 _iHeight, char *_pcLogoInfo);
td_s32 master_set_bitmap_show(td_s32 _s32Index, const td_s32 _s32Chn, td_s32 _ix, td_s32 _iy, int _iShow);
td_s32 master_get_md(td_s32 _s32Index, const td_s32 _s32Chn);
td_s32 master_get_vl(td_s32 _s32Index, const td_s32 _s32Chn);
td_s32 master_set_vl(td_s32 _s32Index, const td_s32 _s32Chn, td_s32 _s32Value);
td_u8* master_get_version(td_s32 _s32Index);
//td_s32 master_set_fontdata(td_s32 _s32Index, td_u8* _pcFontData, td_s32 _iLen);
td_s32 master_find_slave(td_s32 _s32Index);
td_s32 master_set_autofile(td_s32 _s32Index, char *_pNameFile);
td_s32 master_set_slave_file(td_s32 _s32Index, td_u8* _pcFontData, td_s32 _iLen, td_u8 *_pName);
td_s32 master_bind_cascade(td_s32 _s32Index, td_s32 _s32Chn, td_s32 _s32VoDev, td_s32 _s32VoChn, TRect *_pRect);
td_s32 master_unbind_cascade(td_s32 _s32Index, td_s32 _s32Chn, td_s32 _s32VoDev, td_s32 _s32VoChn);
td_s32 master_get_frmrate(td_s32 _s32Index, const td_s32 _s32Chn);
td_u32 master_get_venc_size(td_s32 _s32Index, const td_s32 _s32Chn);
// huaqiang modify 20130111 for AAC
td_s32 master_reset_audio(td_s32 _s32Index, td_s32 _s32Chn, td_s32 _s32AudioSmple, td_s32 _s32AencType);
td_s32 master_set_slave_audio(td_s32 _s32Index, td_s32 _s32AudioSmple, td_s32 _32AencType);
// modify end
//shihc add 
td_s32 master_bind_pciv(td_s32 _s32Index, td_s32 _s32LocPcivChn, td_s32 _s32RemotePcivChn, void *_pPcivChnAttr);
td_s32 master_unbind_pciv(td_s32 _s32Index, td_s32 _iRemotePcivChn);

td_s32 master_get_od(td_s32 _s32Index, const td_s32 _s32Chn);
td_s32 master_set_OdEn(td_s32 _s32Index, const td_s32 _s32Chn, td_s32 _s32En);

#endif
