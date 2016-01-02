#ifndef _MSGBROADCAST_H_
#define _MSGBROADCAST_H_

typedef int  (*MSG_BROADCAST)(char *_cBuf,int _iLen);

enum MSGSNDTYPE
{
	/*视频参数相关 */
	MSGSND_VIDEOSIZE = 0x00000,		//视频大小
	MSGSND_VIDEOHEADER,				//视频头
	MSGSND_VIDEOMODE,				//视频制式         1
	MSGSND_FRAMERATE,					//帧率1
	MSGSND_IFRAMERATE,				//I帧间隔1
	MSGSND_VIDEOQUALITY,				//视频质量1
	MSGSND_VENCTYPE,					//视频编码格式
	MSGSND_ENCODETYPE,				//位率类型(变码率/定码率)
	MSGSND_BITRATE,					//码率kbps
	MSGSND_STREAMTYPE,				//复合码流类型1
	MSGSND_AUDIOCODER,				//音频编码格式1
	MSGSND_VIDEOENCRYPT,				//视频加密密码1
	CHN_ATTR_VIDEO_LIST,               //视频分辨率列表
	MSGSND_VIDEOPROFILE,
	MSGSND_VIDEO_MODE_CHECK,			//发送自动检测制式标志
	MSGSND_AUDIO_SAMPLE,				//采样率
	MSGSND_AUDIO_SAMPLE_LIST,
	MSGSND_AUDIOCODER_LIST,			// 音频编码方式列表
	//视频输入参数
	MSGSND_VIDOEPARA = 0x00050,		//亮度色度饱和度对比度1
	MSGSND_VIDEOPARA_SCHEDULE,		//亮度色度饱和度对比度模板
	MSGSND_COLORPARA,					//彩转黑
	MSGSND_SENSORSPIN,					//视频翻转

	//OSD相关
	MSGSND_OSD_TITLDISPLAY = 0x00100,	//字符叠加 通道名称使能及坐标1
	MSGSND_OSD_TIMEDISPLAY,			//字符叠加时间使能及坐标1
	MSGSND_OSD_WORDDISPLAY,			//字符叠加自定义字符 使能1
	MSGSND_OSD_WORDOVERLAY,			//字符叠加自定义字符坐标及内容1
	MSGSND_OSD_MULTIWORDOVERLAY,			//字符叠加自定义字符坐标及内容(扩展)
	MSGSND_OSD_LOGDISPLAY,			//字符叠加LOG1
	
	MSGSND_OSD_TITLCOLOR,			//通道名称颜色1
	MSGSND_OSD_TIMECOLOR,			//时间颜色1
	MSGSND_OSD_WORDCOLOR,			//自定义字符颜色1
	
	MSGSND_OSDLOGOBACKCOLOR,		//log背景色
	MSGSND_OSDTEXT,					//通道名称叠加内容1
	MSGSND_VIDEOCOV,					//视频遮挡区域

	MSGSND_MOTION_THRESHOLD,		//视频移动侦测灵敏度1
	MSGSND_MOTION_AREA,				//视频移动侦测区域1
	MSGSND_IPCHANPARAM,				//数字通道配置信息
	MSGSND_PORTALARMTYPE,			//数字通道配置信息
	MSGSND_IPCPLUGANDDPLAY,			//数字通道即插即用
	MSGSND_MD_EN,					//移动报警使能
	MSGSND_OD_EN,					//遮挡报警使能
    //智能分析相关 nsg add 20120309
	MSGSND_VCA_CFGCHN	= 0x00150,	//智能分析单通道使能
	MSGSND_VCA_CFGVIDEO,
	MSGSND_VCA_CFGADV,
	MSGSND_VCA_CFGTARGET,
	MSGSND_VCA_RULESET,
	MSGSND_VCA_RULEEVENT0,
	MSGSND_VCA_RULEEVENT2,
	MSGSND_VCA_RULEEVENT7,
	MSGSND_VCA_RULEEVENT8,
	MSGSND_VCA_RULEEVENT9,
	MSGSND_VCA_RULEEVENT10,
	MSGSND_VCA_RULEEVENT11,//caizhaoxu 20130117
	MSGSND_VCA_RULEEVENT12,
	MSGSND_VCA_RULEEVENT13,
	MSGSND_VCA_RULEEVENT14,
	//通用业务使能相关
	MSGSND_COMMON_NETSERVICE_SNMP	= 0x00200,	

};


int msgbroadcast_Init(MSG_BROADCAST _msgsnd);
int msgbroadcast_MsgSnd(int _iChan,int _iMsgId);

//*************************************
int AppChn_MsgSnd(int _iChan,int _iMsgId);
//20130320 wsl add 为副码流回调添加
//*************************************

#endif


