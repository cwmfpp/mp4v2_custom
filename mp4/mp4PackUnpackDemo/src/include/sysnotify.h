#ifndef _SYS_NOTIFY_H_
#define _SYS_NOTIFY_H_


typedef enum
{
	IS_ENCODER 	= 0x001,		//编码线程
	IS_DECODER,					//解码线程
	IS_HARDWARE,				//硬件
	IS_SYSTEM,					//系统
	IS_PREVIEW,					//预览
	IS_CONTECT,					//连接20121106 wsl add
	
	IS_SLAVER_CHIP = 0x2001,	// 从片
} WHO;

typedef enum
{
	GET_SOMETHING_ERROR 	= 0x000,	//通用的错误指示
	LOST_HEARTBEAR			= 0x001,	//丢失心跳
	REC_ERROR				= 0x002,	//录像错误
	TIME_CHANGED			= 0x1000,	//系统时间改变	
	REBOOT_SLV				= 0x1001,	//重启从片
	REBOOT_SYS				= 0x1002,	//系统重启中20120510 dxl
	AUDIO_SAMPLERATE_CHANGE = 0x1003, 	//音频采样率改变	Sherry add 20130110

	CHN_PV_HIDEN			= 0x2000,	//通道预览隐藏
	CHN_PV_RE_MAP_ERR		= 0x2100,	//重复快捷通道错误
	CHN_NOT_CONTECT			= 0x2200,	//通道取消连接20121106 wsl add
	CHN_PV_VDEC_LIMIT		= 0x2300,	//解码通道创建不成功

    DB_OVERWRT_ERR          = 0x3000,   //覆盖删除录像异常
    DB_OVERWRT_OK           = 0x3001,   //覆盖删除录像恢复正常
} WHAT;

typedef enum
{
	DO_NOTHING			= 0x000,		//无需任何处理(自己搞定)
	DO_ANYTHING			= 0x001,		//上层决定如何处理
	DO_SYSTEM_RESET 	= 0x100,		//系统重启
	DO_SELF_RESET		= 0x101,		//模块重启
} HOW;

int SysNotifyInit();
void SysNotifyUninit();

#ifdef MASTER_CHIP
void SysNotify(int _iWho, int _iWhat, int _iHow);
#else
#define SysNotify(_iWho, _iWhat, _iHow)
#endif

int GetSysNotify(int *_piWho, int *_piWhat, int *_iHow);

#endif

