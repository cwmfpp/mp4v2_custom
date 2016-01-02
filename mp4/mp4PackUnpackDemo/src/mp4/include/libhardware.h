/*******************************************************************
文件名	:hardware.h
公司	:天津天地伟业数码科技有限公司
创建者	:陶永亮
创建日期:2009-10-28
简要描述:硬件管理相关数据定义、接口函数原型
修改记录:2009-10-28,陶永亮，整理创建
		 2009-12-08,程显永，重新规划接口，适应Hi3520的硬件
        
******************************************************************/

#ifndef _LIB_HARD_WARE_H
#define _LIB_HARD_WARE_H

typedef struct rtc_time
{
        unsigned char tm_sec;
        unsigned char tm_min;
        unsigned char tm_hour;
        unsigned char tm_wday;
        unsigned char tm_mday;
        unsigned char tm_mon;
        unsigned char tm_year;
} TRtcTime;

/*
 * 初始化硬件模块
 */
td_s32 hardware_Init(td_s32 _iWatchdogTimeout, td_s32 _iHardReset, td_s32 s32AoVolume);
/*
 * 退出硬件模块
 */
void hardware_Uninit();

#define MAXAUDIO	69
#define MAXTOUIAUDIO	255
//AD类型跟驱动定义保持一致
#define TW2865              2865
#define TW2960              2960
#define TW2964              2964
#define GV7601              7601 //shihc add 20121206
#define SD_VIDEO_WIDTH_D1	0x0
#define SD_VIDEO_WIDTH_960H	0x1
#define HD_VIDEO_WIDTH		0x10

/*
 * 获取报警输入状态
 * Bit0-15 代表 1-16路报警输入
 * 1代表有报警，0代表无报警
 */
void hw_GetAlarmIn(td_u32 *_u32Status);

/*
 * 设置报警输出状态
 * Bit0-3 代表1-4路报警输出
 * 1表示输出报警，0表示无输出报警。
 */
td_s32 hw_GetAlarmOut(td_s32 _iPortNo);
td_s32 hw_SetAlarmOut(td_s32 _iPortNo, td_u32 _u32Status);

/*
 * 获取CPLD版本。目前CPLD的版本读出信息为：0x10，
 * 代表版本V1.0。
 */
td_s32 hw_GetCpld();

/*
 * 设置串口自动转向电路的波特率。
 * 若不设置， 默认以1200波特率进行自动换向控制。
 * 参数:
 *      uart_num:需要设置的UART号(dxl: uart_num左移一位表式aurt号,第一位为1表高位,0表低位)
 *      rate:波特率
 *       0	1200
 *       1	2400
 *       2	4800
 *  	 3	9600
 *       4	19200
 *       5	38400
 *       6	57600
 *       7 	115200
 *       其它 保留
 */
void hw_SetUart(td_s32 _iUartNum, td_s32 _iRate);

/*
设置cpld ,使485换向
*/
td_s32 inline hw_SetCpld(td_s32 _iNo, td_s32 _iSpeed);

/*
 * 设置LED灯的亮灭
 * 参数:
 *     ledno:Led灯号，1:黄灯 2:绿灯
 *     statu: 0:灭， 1亮
 */
void hw_SetLight(td_s32 _iLedNo, td_s32 _iStatus);

td_s32 hw_StopLed();

/*
 * 获取reset按键状态
 * 参数:无
 * 返回值:
 *       1:代表reset按下 0代表未按下
 */
td_s32 hw_GetResetState();

/*
 * 获取拨码开关状态
 */
td_s32 hw_GetSwitchState();

/*
 * 设置蜂鸣器
 * 参数:
 *     status: 1:响 0:不响
 */ 
void hw_SetBuzzer(td_s32 _iStatus);

// 设置蜂鸣器是否使能
void hw_SetBuzzerEnable(td_s32 _iStatus);

/*
 * 设置硬件狗超时
 * 参数:
 *     secs: 超时的秒数
 * 返回值:
 *     <0:设置失败 0:设置成功
 */ 
td_s32 hw_SetDogTimeOut(td_u32 _u32Secs);

/*
 * 喂狗
 * 返回值:
 *      <0:喂狗失败
 */
td_s32 hw_FeedDog();

//陶永亮 091221 停止喂狗
td_s32 hw_StopDog();

//陶永亮 091221 将狗置为不使能
td_s32 hw_DisableDog();

/*
 * 读rtc时间
 * 参数
 *     rtc_time:
 */
td_s32 hw_GetRtcTime(int *_iYear, int *_iMonth, int *_iDay, int *_iHour, int *_Min, int *_iSec);

/*
 * 设置rtc时间
 * 参数:
 *    rtc_time:
 */
td_s32 hw_SetRtcTime(int _iYear, int _iMon, int _iDay, int _iHour, int _iMin, int _iSec);

//获取实际ad设备的类型值，2960或2865等，供host和slave调用
td_s32 hw_GetAdType(void);

//设置ad设备类型值，供slave调用
td_s32 hw_SetAdType(td_s32 _s32Type);

// 设置视频输入制式, TD_PAL/TD_NTSC
td_s32 hw_SetViNorm(int _iVideoMode);

// 获取视频输入制式
td_s32 hw_GetViNorm();

td_s32 hw_GetViNormAuto(td_s32 _s32ADChkMode);

// 设置视频输入参数
td_s32 hw_SetViParam(td_s32 _iChn, td_s32 _iBright, td_s32 _iContrast, td_s32 _iSaturation, td_s32 _iHue);

// 获取视频输入参数
td_s32 hw_GetViParam(td_s32 chn, td_u8 *bright, td_u8 *contrast, td_u8 *saturation, td_u8 *hue);

// 获取当前视频输入状态0:有视频,1无视频
td_s32 hw_QueryVideo(td_s32 _iChn);

// 获取gv7601(sdi高清)视频输入状态0:有视频,1无视频
td_s32 hw_QueryVideo_HD(td_s32 _iChn);

//读取gv7601检测到的插入视频的格式
td_s32 hw_GetViFormat(td_s32 _iChn);
//设置gv7601当前的视频输入模式
int hw_SetCurrentViMod(int _iMod);

// 设置彩转黑
/*
 *	value:
 *	0 : disable colortogray
 *	1 : enable colortogray
 *	2 : default, auto detective
 */
td_s32 hw_SetViColorToGray(td_s32 _iChn, td_s32 _iValue);

// 设置音频输出大小
// 参数:
// aodev:	ao设备号，目前使用0, 
// volume:	音量, -1时flag有效
// flag:		1音量加，0音量减
td_s32 hw_SetAoVolume(td_s32 _iAoDev, td_s32  _iVolume, td_s32 _iFlag);
//libaoliang add 2012.2.16
//获取AO的音量大小
td_s32 hw_GetAoVolume(td_s32 _iAoDev);


// 获取当前音频输入状态, 0有音频输入,1无视频输入
td_s32 hw_QueryAudio(td_s32 _iChn);


// 设置音频参数
td_s32 hw_SetAudioIn(td_s32 _iSamplerate, td_s32 _iBitwidth, td_s32 _iChnnum, td_s32 _iSeq);

// 设置音频输入音量
td_s32 hw_SetAudioInVolume(td_s32 _iChn, td_s32 _iVolume);

//设置驱动某通道的输出图像宽度
td_s32 hw_SetVideoSize(int _iChn, int _iWidth);

//获取驱动中设置的某通道的输出图像宽度
td_s32 hw_GetVideoSize(int _iChn);

//设置音频采样率
td_s32 hw_SetADSamplerate(td_s32 _s32DevId, td_s32 _s32SampleRate);
td_s32 hw_SetTlv320Samplerate(td_s32 _s32FuncTpe, td_s32 _s32Aidev, td_s32 _s32Sample);

//shihc add 20121030 增加音频输入的控制代码
td_s32 hw_GetAioFd(void);
td_s32 hw_GetHDStartFlg(void);
td_s32 hw_GetAIVolume(td_s32 _iChn);
td_s32 hw_SetAIVolume(td_s32 _iChn, td_s32 _iVolume);

// 重启系统
void set_Reboot(void);
void hw_SetFanRun(td_s32 _s32FanType);
td_s32 hw_VgaCheck();

//Sherry add 20130110 获取2964状态 
td_s32 hw_GetAddevState(void);
td_s32 hw_SetVinInitVolume(int _iVolume);//设置VIN的音量初始值


td_s32 hw_Set_3g_Power(td_s32 _iState);
td_s32 hw_Set_Wifi_Power(td_s32 _iState);
td_s32 hw_Set_3g_Light  (td_s32 _iState);
td_s32 hw_Set_Wifi_Light(td_s32 _iState);
td_s32 hw_Reset_BD(td_s32 _iState);
td_s32 hw_SetAutoCopy(td_s32 _iState);
td_s32 hw_Set_Rec_Light(td_s32 _iState);
td_s32 hw_SetAoInitVolume(int _iVolume);//设置音频输出的初始值
td_s32 hw_GetSysTemp();

td_u32 hw_GetViMod();
td_u32 hw_SetViMod(td_u32 _u32ViMod);

enum
{
	AUDIO_VOLUME_CTRL_START_MIC_ID  = 1,  //MIC音频输入编号 预留64个，2U实际4个，3U实际16个，对应设备的实际mic通道id
	AUDIO_VOLUME_CTRL_START_MIC_ID_MAX = 64,//MIC音频输入最大编号

	AUDIO_VOLUME_CTRL_PROOF1_ID = 65,  //示证音频输入编号 示证，只有1个

	AUDIO_VOLUME_CTRL_REMOTE_ID = 100,  //远程音频输入，共4路，对应AO4的0-3路
	AUDIO_VOLUME_CTRL_REMOTE_ID_MAX = 103, //远程音频输入最大ID
};
/*
函数名 		:	hw_SetAudioVolume
功能	    :	设置对应ID的音量值
输入参数	:	_iId:ID号
				_iVolume:声音大小
返回值		: >=0 成功   -1 失败
*/
td_s32 hw_SetAudioVolume(td_s32 _iId, td_s32 _iVolume);

//设置MIC类型到驱动，用于修改点对点和手拉手的增益。
td_s32 hw_SetMicType(td_s32 _iVideoChn, td_s32 _iAudioChn, td_s32 _iMicType);


#endif

