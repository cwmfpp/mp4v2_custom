//*****************************************
// 虚拟通道结构与接口定义
//
// 维护记录:
// 杜秀龙,2011/02/24,创建
//******************************************

#ifndef _CHANNEL_H_
#define _CHANNEL_H_

#include "td_types.h"
#include "libdatacenter.h"
#include "alarmer.h"
#include "ringbuf.h"
#include "vca.h"
#define PLAYBACK_CHN	10000

typedef struct _TChannel
{
	td_s32 m_iChn;					//逻辑通道号
	td_s32 m_iType;					//通道类型(备用)
	
	td_s32 m_iVideoSize[2];			//视频尺寸
	td_s32 m_iVideNorm[2];			//视频制式
	td_s32 m_iVencType[2];			//编码方式

	TDataCenter *m_ptStreanDc[2];	//码流缓存池

	TAlarmer *m_ptMdAlarmer;		//移动侦测报警
	TAlarmer *m_ptVideoLostAlarmer;	//视频丢失报警
	//TAlarmer *m_ptOdAlarmer;		//视频遮挡报警		//zhangyi del 20130411

	// 注意，为保证兼容性，此结构体不允许再被更新
	//add by caizhaoxu for vca 20120321
	TAlarmer * m_ptTripWireAlarmer[MAX_RULE_NUM];  // 绊线报警    
	TAlarmer * m_ptPeriMeterAlarmer[MAX_RULE_NUM]; //周界报警
	TAlarmer * m_ptDiagnoseAlarmer[MAX_RULE_NUM];  //视频诊断报警
	TAlarmer * m_ptOSCerAlarmer[MAX_RULE_NUM];     // 物品看护

	TAlarmer *m_ptAudioLostAlarmer;//音频丢失检测add by zhuangzhibing 20120222

	//参数设置/获取接口
	td_s32 (*SetAttribute)(td_s32 _iAid, const void *_pValue, struct _TChannel *_ptThis);
	td_s32 (*GetAttribute)(td_s32 _iAid, void *_pValue, struct _TChannel *_ptThis);
	// 命令操作接口
	td_s32 (*DoCmd)(td_s32 _iCid, void *_pArg, struct _TChannel *_ptThis);
	// 计算硬件性能消耗
	td_s32 (*PerformanceCost)(td_s32 _iPid, struct _TChannel *_ptThis);
	// 消息处理回调
	td_s32 (*MsgNotifyCallback)(td_s32 _iMid, td_s32 _iSubId, void *_pMsg, struct _TChannel *_ptThis);								//合成通道的音频是否静音
} TChannel;

// 接口返回值定义
typedef enum
{
	CHN_RET_FAILD = -1,			//通用失败码(其它负数可表示明确的错误码)
	CHN_RET_SUCCESS = 0,		//成功
	CHN_RET_PENDING = 1,		//未决
	CHN_RET_NOSUPPORT = 2,	//不支持
} CHANNEL_RET_VALUE;

// 通道参数编码定义
typedef enum
{
	//编码参数
	CHN_ATTR_VIDEO_SIZE = 0x00000,	//视频尺寸
	CHN_ATTR_VIDEO_NORM,			//视频制式
	CHN_ATTR_VENC_FRAME_RATE,	//帧率
	CHN_ATTR_VENC_IFRAME_RATE,	//I帧间隔
	CHN_ATTR_VENC_QUALITY,		//视频质量
	CHN_ATTR_VENC_TYPE,			//视频编码格式
	CHN_ATTR_VENC_BIT_TYPE,		//位率类型(变码率/定码率)
	CHN_ATTR_VENC_BIT_RATE,		//码率kbps

	CHN_ATTR_STREAM_TYPE,			//复合码流类型
	CHN_ATTR_AVENC_TYPE,			//音频编码格式

	CHN_ATTR_VENC_PIXEL_SIZE,		//视频编码真实的像素分辨率(获取参数, 传TVideoSize)
	CHN_ATTR_VENC_MAX_SIZE,		//视频最大的编码尺寸

	CHN_ATTR_VIDEO_MODE_CHECK,	//自动检测制式	
	CHN_ATTR_VIDEO_PROFILE,		//设置视频profile


	CHN_ATTR_AUDIO_SAMPLE,		//音频采样率
	CHN_ATTR_AUDIO_SAMPLE_LIST,
	CHN_ATTR_VIDEOSIZE_LIST,	// huaqiang add 20121126
	CHN_ATTR_AUDIOCODER_LIST,  // huaqiang add 20130218, 18
	CHN_ATTR_VDEC_LIMIT_PREVIEW,
	CHN_ATTR_VDEC_FOR_COST,		// zhangyi add 20130617
	
	CHN_ATTR_MEDLEY_AV_ENABLE, //拼接音视频使能
	CHN_ATTR_FORCEIFRAME,		//强制I帧 liuqi add 2015-5-25

	//视频输入参数
	CHN_ATTR_VI_CNF = 0x00050,		//亮度色度饱和度对比度(传TChnViCnf)
	CHN_ATTR_VI_C2B,				//彩转黑
	CHN_ATTR_VI_SIZE,				//VI大小改变
	CHN_ATTR_SENSORSPIN,			//视频翻转
	CHN_ATTR_VIDEOENCRYPT,		// 视频加密

	//音频输入
	CHN_ATTR_AI_VOL = 0x00060,			//音频输入音量
	CHN_ATTR_AI_STAT,					//音频状态

	//OSD相关
	CHN_ATTR_OSD_NAME_EN = 0x00100,	//通道名称叠加使能
	CHN_ATTR_OSD_NAME_CNF,			//通道名称叠加文本位置颜色(传TChnNameOsdCnf)
	CHN_ATTR_OSD_NAME_DSP,			//通道名称叠加使能与位置参数(传TOsdDspCnf)
	CHN_ATTR_OSD_NAME_STR,			//通道名称叠加字符串
	CHN_ATTR_OSD_NAME_COLOR,		//通道名称叠加颜色

	
	CHN_ATTR_OSD_TIME_EN = 0x00110,	//时间叠加使能
	CHN_ATTR_OSD_TIME_CNF,			//时间叠加信息(传TChnTimeOsdCnf)
	CHN_ATTR_OSD_TIME_DSP,			//时间叠加使能与位置(传TOsdDspCnf)
	CHN_ATTR_OSD_TIME_FORMAT,		//时间叠加模式(格式<<16 | 分隔符,形成一个整数)
	CHN_ATTR_OSD_TIME_COLOR,			//时间叠加颜色
	

	CHN_ATTR_OSD_TEXT_EN = 0x00120,	//自定义文本叠加使能
	CHN_ATTR_OSD_TEXT_CNF,			//自定义文本叠加信息(传TChnTextOsdCnf)
	CHN_ATTR_OSD_TEXT_DSP,			//自定文本显示(传TOsdDspCnf)
	CHN_ATTR_OSD_TEXT_STR,			//自定文本内容
	CHN_ATTR_OSD_TEXT_COLOR,			//自定义文本颜色
	

	CHN_ATTR_OSD_LOGO_EN = 0x00130,	//LOGO叠加使能
	CHN_ATTR_OSD_LOGO_CNF,		//LOGO叠加信息(传TChnLogOsdCnf)

	CHN_ATTR_OSD_VCOVER_EN = 0x00140,	//视频遮挡使能
	CHN_ATTR_OSD_VCOVER_AREAS,	//视频遮挡区域(传TChnVideoCoverAreaCnf)
	CHN_ATTR_OSD_FONT_SIZE,

	//视频加密解密
	CHN_ATTR_STREAM_ENCRYPT_PSW = 0x00150,	//视频加密密码(传字符串指针,为空停止解密)
	CHN_ATTR_STREAM_DECRYPT_PSW = 0x00151,	//视频解密密码(同上)

	//报警相关
	CHN_ATTR_MD_EN = 0x00200,		//移动侦测使能
	CHN_ATTR_MD_SENS,			//移动侦测灵敏度	
	CHN_ATTR_MD_AREAS,		//移动侦测区域(传TChnMdAreaCnf)

	CHN_ATTR_VL_EN = 0x00210,		//视频丢失检测使能
	
	CHN_ATTR_OD_EN = 0x00230,		//视频遮挡报警使能
	CHN_ATTR_OD_ODSADSENS,		//视频遮挡报警宏块灵敏度
	CHN_ATTR_OD_ODAREASENS,		//视频遮挡报警面积灵敏度
	CHN_ATTR_OD_AREAS,		//视频遮挡报警面积区域

	//阈值相关
	CHN_ATTR_MAX_FRAME_SIZE = 0x00300,		//最大帧长度

	
    //智能分析相关	add nsg 20120309
	CHN_ATTR_VCA_CFGCHN = 0x00350,
	CHN_ATTR_VCA_CFGVIDEO,
	CHN_ATTR_VCA_CFGADV,
	CHN_ATTR_VCA_CFGTARGET,
	CHN_ATTR_VCA_RULESET,
	CHN_ATTR_VCA_RULEEVENT0,
	CHN_ATTR_VCA_RULEEVENT2,
	CHN_ATTR_VCA_RULEEVENT7,
	CHN_ATTR_VCA_RULEEVENT8,
	CHN_ATTR_VCA_RULEEVENT9,
	CHN_ATTR_VCA_RULEEVENT10,
	CHN_ATTR_VCA_RULEEVENT11,
	CHN_ATTR_VCA_RULEEVENT12,
	CHN_ATTR_VCA_RULEEVENT13,
	CHN_ATTR_VCA_RULEEVENT_SVD,
	//PTZ 相关
	CHN_ATTR_PTZ_PRIORITY = 0x00400,

	/* 对讲相关*/
	CHN_ATTR_TALK_OK = 0x00600,			//对讲成功
	CHN_ATTR_TALK_ERROR, 				//对讲出错
	CHN_ATTR_TALK_END, 					//对讲结束

	//激励相关
	CHN_ATTR_BESTIR = 0x00800,	 //语音激励
	
	//数字通道
	CHN_ATTR_IP_CNF = 0x01000,		//数字通道配置信息(传TIpChannelMsg)
	CHN_ATTR_REGCENTER,			//注册中心(传TRegCenterMsg，所有通道共用)
	CHN_ATTR_VIDEO_HEADER,
	CHN_ATTR_AENC_FRAME_LEN,	//音频长度(获取)
	CHN_ATTR_CONNECT_MAC,	//mac
	CHN_ATTR_TALK_AENC_TYPE,	//进行对讲操作时使用的音频编码类型
	CHN_ATTR_PREVIEW_PREFER,// 预览偏好
    CHN_ATTR_IPC_GETVERSION,	        //获取ipc版本
	CHN_ATTR_UPGRATE_STA,//获取IPC升级状态
	CHN_ATTR_ALARM_TYPE_CHG = 0x01050,	//报警器类型改变(数字通道专用，传TIpAlarmerMsg)


	// 合成通道
	CHN_ATTR_VC_CNF = 0x01100,		//合成通道配置信息(传TVcChannelMsg)
	CHN_ATTR_VODEV_FOUCUS,		//当前音频通道号(传TVoDevFocusChgMsg)

	CHN_ATTR_MIX_AUDIO_CHN,		//混音通道信息(传TVcMixAudioChnMsg)
	CHN_ATTR_VITAL_DATA_RINGBUF,		//生命体征RingBuf

	CHN_ATTR_CHANNEL_ENABLE = 0X01200, // 通道使能状态

	CHN_ATTR_ALARMER = 0x09000,		//获取报警器实例(传TChnAlarmer)
	
	//所有针对副码流的属性设置在主码流对应值上+ CHN_ATTR_SUB_START
	CHN_ATTR_SUB_START = 0x100000,	//副码流参数码开始

} CHANNEL_ATTRIBUTE;


// 通道命令码编号定义
typedef enum
{
	// 编码相关
	CHN_CMD_START_ENCODE = 0x00001,	//开始编码
	CHN_CMD_STOP_ENCODE,				//停止编码
	CHN_CMD_FORCE_IFRAME,			//强制I帧
	CHN_CMD_SNAPSHOT,				//图片抓拍(传送参数为TChnSnapshotMsg)

	//预览相关
	CHN_CMD_BIND_VO 		= 0x00010,	//绑定预览画面(传送参数为TVoBindMsg)
	CHN_CMD_UNBIND_VO,				//解绑定预览画面(传送参数为TVO)
	CHN_CMD_BIND_AO,					//绑定音频预览
	CHN_CMD_UNBIND_AO,				//解绑定音频预览
	CHN_CMD_DRAW_PREVIEW_OSD,		//显示预览OSD字符(传送参数为TChnOsdCnf)
	CHN_CMD_CLEAR_PREVIEW_OSD,		//清除预览OSD字符显示
	CHN_CMD_CHG_DSP_AREA_ON_VO,	//设置预览VO显示区域(传TVoBindMsg)
	CHN_CMD_PRV_CHECK,				//预览状态检测(传送参数为TVoBindMsg)
	CHN_CMD_AUDIO_PV,				//(音频预览增强接口，可指定AO设备号，传整形数)

	//码流相关
	CHN_CMD_START_AS_SEND	= 0x00020, //启动纯音频发送(传送参数TRingBuf)
	CHN_CMD_STOP_AS_SEND	= 0x00021, //停止纯音频发送

	// 云台控制相关
	CHN_CMD_PTZ_CTR 		= 0x00100,	//发送云镜控制码(传送TPtzCtrParam)

	//虚拟合成通道相关
	CHN_CMD_ATYPE_CHG		= 0x00200,	//通道音频类型变更(传通道号)
	CHN_CMD_ASAMPLERATE_CHG,                        //音频采样率变更 ljj add
	//智能分析相关 nsg add 2012007
	CHN_CMD_VCA_REBOOT		= 0x00300,	// 重启智能分析库
	CHN_CMD_VCA_ALARMSTATCLR,			// 智能分析报警计数清零
	//end
	CHN_CMD_VCA_RECREATE,				//智能分析重建

	// 切换预览码流
	CHN_CMD_CHANGE_PREVIEWSTREAM = 0x00400,

	// 特殊的命令(与具体通道无关，但依然以通道的形式传达)
	CHN_CMD_SPECIAL		= 0xF0000,
	CHN_CMD_IPC_SEARCH,				//数字通道搜索(传TIpcSearchParam)
	CHN_CMD_IPC_LIST,				//获取IPC 连接状态列表(传字符串指针，6K大小)
	CHN_CMD_IPC_SET_TIME,			//修改时间(无参数)
	CHN_CMD_IPC_SET_TIMEZONE,       //修改时区
	CHN_CMD_CLIP_SIDE,				//设置裁边
	CHN_CMD_IPC_CHANGE,
	CHN_CMD_IPC_TALK_START,		//与数字通道开始对讲(传送参数TIpTalkPara)
	CHN_CMD_IPC_TALK_STOP,		//与数字通道停止对讲
	CHN_CMD_IPC_SEND_PUSH_DATA,	//向数字通道发送push流(传送参数TPushData)
	CHN_CMD_IPC_REBOOT,				//重启前端设备
	CHN_CMD_IPC_UPGRADE,			//升级bin或box文件

	CHN_CMD_IPC_CHANGEIP = 0xF000F,	//修改前端IPC IP地址
	CHN_CMD_CHN_REBOOT,
	
	CHN_CMD_SUB_START 	= 0x100000,	//副码流命令码开始
} CHANNEL_CMD;


// 消息通知编号定义:
typedef enum
{
	CHN_MSG_NOP = 0x00000,
	CHN_MSG_ATTR_CHG,					//通道属性更新

	//抓拍相关
	CHN_MSG_SNAPSHOT_DONE = 0x00010,		//抓拍完成
	CHN_MSG_SNAPSHOT_FAILD = 0x00011, 	//抓拍失败

	// 数字通道
	CHN_MSG_CONNECT_BAD = 0x00100,		//连接中断
	CHN_MSG_CONNECT_RECOVER,			//连接恢复

	//心跳
	CHN_MSG_HEART = 0x00200,			//连接恢复
} CHANNEL_MSG;

// 性能消耗ID
typedef enum
{
	CHN_PM_PREVIEW = 0,	//预览
	CHN_PM_ENCODE,		//编码
	CHN_PM_NTX,			//网传
	CHN_PM_USERSET_STREAM,

	CHN_PM_SUB_START = 0x10000,	//副码流通道性能计算
} CHANNEL_PERFORMANCE;

// 通道类型
typedef enum
{
	CHN_TYPE_TEST		= 0x000,		//测试用的通道类型
	CHN_TYPE_BNC 		= 0x100,		//BNC通道类型(通道管理层再区分LOCAL与PCI)
	CHN_TYPE_LOCAL 		= 0x101,		//主片通道
	CHN_TYPE_PCI		= 0x102,		//pci从片通道
	CHN_TYPE_SDIPCI		= 0x150,		//sdi从片通道
	CHN_TYPE_SDILOCAL 	= 0x151,		//sdi主片通道
	CHN_TYPE_VC			= 0x200,		//VO合成通道
	CHN_TYPE_SVC		= 0x201,		//行业VO合成通道
	CHN_TYPE_IP			= 0x300,		//数字通道
} CHANNEL_TYPE;

// 回调函数ID号定义
typedef enum
{
	CHN_CB_ATTACH_AUDIO_STREAM = 0x0001,	//获取通道音频数据回调
	CHN_CB_DETACH_AUDIO_STREAM = 0x0002, 	//停止获取通道音频数据回调
	CHN_CB_GET_CHANNEL_INSTANCE = 0x0003, 	//通过通道号获取通道实例
	CHN_CB_ALARMER_REGISTER = 0x0004,			//报警器注册函数
	CHN_CB_STREAM_CRYPT = 0x0005,				//视频加解密函数

	CHN_CB_COM_DATA		= 0x0101,			//串口数据

	CHN_CB_PTZ_CTRL		= 0x0201,			//ptz//dxl 20130117
} CHANNEL_CALLBACK_ID;
//dxl 20130117
typedef struct
{
	int iPreset;
	int iSpeedX;
	int iSpeedY;
} TChnPtzParam;

typedef TRingBuf *(*ChnAttachAudioStreamCallback)(td_s32 _iChn);
typedef td_s32 (*ChnDetachAudioStreamCallback)(TRingBuf * _ptBuf);
typedef TChannel *(*ChnGetInstanceCallback)(td_s32 _iChn);
typedef td_s32 (*ChnAlarmerRegisterCallBack)(td_s32 _iId, td_s32 _iType, TAlarmer *_ptAlarm);
typedef td_s32 (*ChnStreamCryptCallBack)(void *_pBuf, td_s32 _iSize, td_u32 _uiKey,  td_u8 *_strPsw, td_s32 _iPswLen);
typedef td_s32 (*ChnComDataProcCallBack)(td_s32 _iChn, td_s32 _iComdId, td_s32 _iLength, void *_pBuf);
typedef td_s32 (*ChnPtzCtrlCallBack)(td_s32 _iChn, int _iPtzAction, TChnPtzParam * _ptTChnPtzParam);//dxl 20130117 通用PTZ回调函数

// 每查询到一个设备，通过此回调向上返回一个TIpcDevItem实例, 传NULL表查询结束
typedef td_s32 (*ChnIpcSearchReportCallBack)(TIpcDevItem *_ptItem);

typedef struct //数字通道搜索参数
{
	ChnIpcSearchReportCallBack m_fReportFun;
	td_s32 m_iSearchType;
} TIpcSearchParam;

// for decoder
typedef struct
{
	TDataCenter* m_pIpTalkCenter; 
}TIpTalkPara;

typedef struct
{
	td_u8* 	m_pBuf;
	int 		m_iBufLen;
}TPushData;
//解码器通过数字通道push流操作
td_s32 chnmnt_ChannelIpChannelPushData(td_s32 _iChn,TPushData* _ptPushData);
#endif

