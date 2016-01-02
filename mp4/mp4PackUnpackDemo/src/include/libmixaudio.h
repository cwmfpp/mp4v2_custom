#ifndef _LIBMIX_AUDIO_H_
#define _LIBMIX_AUDIO_H_
#include <unistd.h>
#include <pthread.h>
#include "td_types.h"

//静音阈值转换系数，UI 用的值为0-100，在uk作相应的转换
#define THRESHOLD_COEFFICIENT 	1000

#define AUDIO_BOUNDS            50000//150000  	//方差小于5000认为是静音 add by 宁思光 2011.10.20
#define MAX_MIX_AUDIO_CHN		2     	//每路混音通道合成音频通道数
#define MAX_AUDIO_FRAME_LENGTH		512
#define AUDIO_MIX_CHN_NUM		1		//混音通道数
#define PCM_BUFFER_LEN			480//320     //一帧音频由320个short组成
#define MAX_ANEC_CHNNUM  		30 		//定义最大音频编码通道号
#define BUFFER_SIZE   			200 * 480 //2560*2 	//环形buffer大小320*2*4
#define AUDIO_DATE_LEN      	960//640   	//编码时音频数据的长度
#define MAX_AUDIO_LEN			484*110 //56*324
#define MIXAUDIO_NULL_CHN_NO	0x7FFFFFFF	//混音空音源通道号, 用以表示"--"选项
#define MAX_READ_CNT 8			//ljj add

#define AUDIO_BUF_LEN 			 MAX_READ_CNT*484//2*484//

typedef struct
{
	int m_iAudioSrcChn;//音源逻辑通道号
	int m_iAudioType;
	int m_iAudioLen;
	int m_iAudioSample;//ljj add 20121022
	unsigned char m_bAudio[AUDIO_BUF_LEN];
	int m_iAudioNUll[2];   //代表这一帧是否是静音，0不是静音，1是静音
} TMixAudioMsg;


typedef struct
{
	int m_iUsedFlag;
	int m_iMixAdecType[MAX_MIX_AUDIO_CHN]; 
	int m_iMixAencType;
	pthread_mutex_t m_MutexAudioMix;
} TMixAudioChn;


//音频状态，预留用于静音检测，gxl
typedef enum
{
	AUDIO_MUTE = 0,		  //静音
	AUDIO_NORM,   //正常音频
	AUDIO_NULL,		  //空
}AUDIO_STATUS;

//buffer状态
typedef enum
{
	BUFFER_NULL = 0,	//buffer中数据长度不够320
	BUFFER_HALF = 1,	//buffer中数据长度大于320但没有满
	BUFFER_FULL = 2,	//buffer已满
}TBuffer_Status;			//add by nsg 20111103

typedef struct
{
	int m_iResult;	//方差检测结果
	int m_iSeq;	//音频帧序号，每25帧进行一次方差检测
}AUDIO_VARIANCE;        //add by 宁思光20111020


typedef struct
{
	short m_iMax;	//这帧音频数据的最大值
	short m_iMin;	//这帧音频数据的最小值
	int m_iNum;	//所有数据之和
}AUDIO_DATA;

// 创建一个混音通道，返回其句柄
int CreateMixAudioChannel();

// 销毁一个混音通道
int DestroyMixAudioChannel(int _iId);

// 两个音频源可能有一个为NULL，注意此时的处理
// 混音的编码方式由m_iAudioType指定
// 混音后的编码长度由m_iAudioLen返回
int AudioCheckVarianceEx(const TMixAudioMsg *_ptAudio);
int MixAudio(int _iId, TMixAudioMsg *_ptAudio1, TMixAudioMsg *_ptAudio2, TMixAudioMsg *_ptAudioMix);
//int MixAudio(int _iId,td_u8 *_ptAudio1,  td_u8 *_ptAudio2, td_u8 *_ptAudioMix);

int ClearMixaudio(int _iId);//ljj add

int AudioCheckVariance(const unsigned char g711a_data[], int g711a_bytes);//add by gyn

//int GetChnAudioState( void );// 获取哪个通道是静音add by nsg 20120129

int SetAudioLen(int _iAudioLen);
//设置/ 获取静音检测阈值;
//获取入参: 音频通道, 返回阈值
int AudioCheckGetBounds(int _iAudioChn);
//设置:音频通道 、阈值;返回成功或失败
int AudioCheckSetBounds(int _iAudioChn,int _iBounds);

td_s32 MixAudioGetSleepTm(td_s32 _iSleepTm);

#endif

