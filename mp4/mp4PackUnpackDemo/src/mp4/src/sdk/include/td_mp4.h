/*****************************************************************************
 * NAME         : td_mp4.h
 * FUNCTION     : MP4  转换回放相关
 * PROGRAMMED   : 张胜
 * DATE         : 2014-12-10
 * PROJECT      : 
 * OS           : Linux
 ****************************************************************************/  

#ifndef __TD_MP4_H_INCLUDED__
#define __TD_MP4_H_INCLUDED__

#define FOR_MP4SDK
#define	ZHENGFA
#ifdef FOR_MP4SDK
#ifndef ALONE
#define ALONE
#endif
typedef unsigned char	MP4_VOID;
#else
typedef void			MP4_VOID;
#endif

#include "public_def.h"
#include "ringbuf.h"
//#include "dlist.h"

#ifdef TD_VFS
#include "libvfs.h"
#include "libstdfs.h"
#include "libtdfs.h"
#endif


#ifndef FILENAMEMAXNUM
#define FILENAMEMAXNUM	128
#endif

typedef off_t __int64;

#define MAX_ExInfo_SIZE       						1024
#define MAX_VIDEO_SAMPLE_SIZE 						1024*1024
#define MAX_AUDIO_SAMPLE_SIZE 						1024
#define MAX_AUDIO_FRAMENUM                          20
#define MP4_BOX_SIEZ_LENGTH   						4
#define MP4_BOX_SIZE_LENGTH_BIG                     8
#define MP4_FILE_FLAG_LENGTH 						4
#define MP4_FILE_TYPE_LENGTH 						4
#define MP4_BOX_TYPE_LENGTH 						4
#define MP4_CUSTOM_MDATFLAG_LENGTH 					8
#define MP4_MDAT_VERSION_LENGTH 					4
#define MP4_SAMPLE_TYPE_LENGTH 						1
#define MP4_MAX_RECORD_BUF_SIZE 					512*1024
#define MP4_CYSTOMBOX_HEAD_LEN 						28
#define MP4_RESUM_DATA_BUF_SIZE 					1024*1024
#define MP4_MAX_WRITELEN_ONCE                       512*1024

#define MAX_FRAME_INTERVAL        1000 //ms
#define DEFAULT_FRAME_INTERVAL    40   //ms

#define REC_ISAMPLE_INDEX_MAGIC_NUMBER   0x0123abce//MP4文件标示

#define MP4_AAC_TYPE_SIGN 		"AAC"
#define MP4_G711A_TYPE_SIGN 	"alaw"
#define MP4_G711U_TYPE_SIGN 	"ulaw"
#define MP4_PCM_TYPE_SIGN 		"pcm"
#define MP4_PCM_TYPE_SIGN2 		"PCM"

#define TYPE_TIANDY_MP4_FILE     1 		
#define TYPE_OTHER_MP4_FILE      2 		

#ifdef FOR_MP4SDK
//add for windows 
#define MAX_WIDE_CHAR_NUM     254
//add end
#endif

/* H264 NALU TYPES */
#define SLICE_LAYER_WITHOUT_PARTITIONING_RBSP    0x01  /*非IDR 图像的编码slice*/
#define SLICE_DATA_PARTITION_A_LAYER_RBSP        0x02  /*编码slice 数据划分A*/
#define SLICE_DATA_PARTITION_B_LAYER_RBSP    	 0x03  /*编码slice 数据划分B*/
#define SLICE_DATA_PARTITION_C_LAYER_RBSP        0x04  /*编码slice 数据划分C*/
#define SLICE_LAYER_WITH_PARTITIONING_RBSP       0x05  /*IDR 图像中的编码slice*/
#define SEI_RBSP                                 0x06  /*SEI (补充增强信息)*/
#define SEQ_PARAMETER_SET_RBSP                   0x07  /*SPS (序列参数集)*/
#define PIC_PARAMETER_SET_RBSP                   0x08  /*PPS (图像参数集)*/
#define ACCESS_UNIT_DELIMITER_RBSP               0x09  /*接入单元定界符*/
#define END_OF_SEQ_RBSP                          0x10  /*序列结束*/
#define END_OF_STREAM_RBSP                       0x11  /*码流结束*/
#define FILLER_DATA_RBSP                         0x12  /*填充数据*/

/**
 * Table 7-3: NAL unit type codes H265 cwm
 */
typedef enum {
    NAL_TRAIL_N    = 0,//默认类型，无效
    NAL_TRAIL_R    = 1,
    NAL_TSA_N      = 2,
    NAL_TSA_R      = 3,
    NAL_STSA_N     = 4,
    NAL_STSA_R     = 5,
    NAL_RADL_N     = 6,
    NAL_RADL_R     = 7,
    NAL_RASL_N     = 8,
    NAL_RASL_R     = 9,
    NAL_BLA_W_LP   = 16,
    NAL_BLA_W_RADL = 17,
    NAL_BLA_N_LP   = 18,
    NAL_IDR_W_RADL = 19,
    NAL_IDR_N_LP   = 20,
    NAL_CRA_NUT    = 21,
    NAL_VPS        = 32,
    NAL_SPS        = 33,
    NAL_PPS        = 34,
    NAL_AUD        = 35,
    NAL_EOS_NUT    = 36,
    NAL_EOB_NUT    = 37,
    NAL_FD_NUT     = 38,
    NAL_SEI_PREFIX = 39,//cwm 为公司的私有数据
    NAL_SEI_SUFFIX = 40,
} NALUnitType;

//cwm视频编解码类型
typedef enum AVCodecID{
	AV_CODEC_ID_H264,
	AV_CODEC_ID_HEVC,
}AVCodecID;

/***************************************************************************/
//Macro defined area begin
#ifndef MAX_RESERVED_INFO_LEN
#define MAX_RESERVED_INFO_LEN	64	//录像文件信息中的预留信息长度
#endif
#define CRYPT_KEY_STR       "yszjtszj"
#define MP4_APPEND_CHAR_INFO_SIZE	(MAX_RESERVED_INFO_LEN+8)	//72
#define ADTS_HEADER_LENGTH				7
#define HAVE_SDV_VIRTUAL_FRAME_FLAG		1
#define HAVE_NO_SDV_VIRTUAL_FRAME_FLAG  0
#define APPEAR_SDV_VIRTUAL_FRAME_FLAG	1
#define TD_MP4_CONVERT_RUNNING	1
#define TD_MP4_CONVERT_STOP		0
//Macro defined area end
/***************************************************************************/

typedef struct
{
	unsigned int m_VideoTrackID;    /*视频ID*/
	unsigned int m_AudioTrackID;    /*音频ID*/
	void* m_MP4Handle;    /*MP4 文件句柄*/
#ifdef TD_VFS
	TVfile* m_pfSDVFileHandle;        /*SDV 文件句柄*/
#else
	FILE* m_pfSDVFileHandle;        /*SDV 文件句柄*/
#endif
	char m_cMP4FilePath[FILENAMEMAXNUM];  /*MP4 文件路径*/
	char m_cSDVFilePath[FILENAMEMAXNUM];  /*SDV 文件路径*/
	S_FileHeader m_stSDVFileHead;   /*记录SDV 文件格式头，固定信息*/
	S_FrameHeader m_stSDVFrameHead; /*记录音视频帧，实时信息*/
	unsigned long long m_VedioDuration;    /*视频步进间隔，实际帧间隔*/
	int m_bVedioFirstAppearFlag;   /*视频第一次出现标识，初始值为0，第一次之后为1，计算第一次写入MP4视频帧时使用*/
	int m_bFirstSpsFlag;           /*只写入一次SPS 的标识，初值为0，第一次之后为1*/
	int m_bFirstPpsFlag;           /*只写入一次PPS 的标识，初值为0，第一次之后为1*/
	int m_bFoundSpsPpsFlag;     
	int m_bHaveSDVVirtualFrameFlag;   /*初值为0，打开SDV 文件时检测到SDV 虚拟帧尾时设为HAVE_SDV_VIRTUAL_FRAME_FLAG*/
	int m_bConvertExitFlag;           /*转换停止标识*/
	int m_bAudioFirstAppear;    /*音频第一次出现标识，初始值为0，出现后为1*/
	int m_bIFrameFlag;
	int m_bCryptFlag;          /*是否转换中解密数据*/
	off_t m_lCurFilePos;         /*SDV文件当前游标位置*/
	off_t m_lStartConvertPos;  /*SDV文件开始转换的部位*/
	off_t m_lStopConvertPos;		/*SDV文件停止转换部位*/
	off_t m_lFileStopPos;        /*SDV文件结束部位*/
	unsigned long long m_AudioDuration;       /*音频步进间隔，实际帧间隔，备注同一次的音频间隔为0*/
	unsigned int m_uiAudioSampleID;    /*音频ID 计数*/
	unsigned int m_uiVedioSampleID;    /*视频频ID 计数*/
	unsigned int m_uiUselessCount;     /*无效帧当前计数*/
	unsigned long long m_AudioTotalDuration;
	unsigned long long m_VedioTotalDuration;
	int m_iIFrameCount;
	unsigned int m_uiLastAudioTime;    /*记录上一次从SDV文件中读取到音频时间*/
	void* m_pvMP4VirtualFramebuffer;   /*指向MP4 索引帧的指针，备注，并不是指向虚拟帧帧头*/
	int m_uiMP4VirtualFramebufferSize;
	void* m_pvSDVVirtualFramebuffer;   /*指向SDV 索引帧的指针，备注，并不是指向虚拟帧帧头*/
	int m_uiSDVVirtualFramebufferSize;
	unsigned char* m_ucAVDataBuf;/*音视频数据buf*/
	int m_iAVDataBufSize;        /*音视频数据buf申请长度*/
	unsigned char* m_ucVideoSampleDataBuf;/*视频数据buf*/
	int m_iVideoSampleDataBufSize;         /*视频数据buf 申请长度*/
}TMP4Mgr;


/***************************************************************************/
//Data structure defined area begin

typedef struct
{
	int m_iHasCnt;
	int m_iMaxCnt;
	int *m_piMark;
}MP4_TMark;

typedef struct
{// NALU
	int m_iType;  
	int m_iVedioSize;  
	unsigned char* m_pucVedioData;  
}TMP4Nalu;  

typedef struct
{//ISO/IEC 14496-3 ADTS部分
	//adts_fixed_header
	int synword;										//0~11		12 bslbf
	unsigned char ID;									//12			1  bslbf
	unsigned char layer;								//13~14		2  uimsbf
	unsigned char protection_absent;					//15			1  bslbf
	unsigned char profile_ObjectType;					//16~17		2  uimsbf
	unsigned char sampling_frequency_index;			//18~21		4  uimsbf
	unsigned char private_bit;							//22			1  bslbf
	unsigned char channel_configuration;				//23~25		3  uimsbf
	unsigned char original_copy;						//26			1  bslbf
	unsigned char home;								//27			1  bslbf
	//adts_variable_header
	unsigned char copyright_identification_bit;			//28			1  bslbf
	unsigned char copyright_identification_start;			//29			1  bslbf
	unsigned char _[1];
	int aac_frame_length;								//30~42		13 bslbf
	int adts_buffer_fullness;							//33~53		11 bslbf
	unsigned char number_of_raw_data_blocks_in_frame;	//54~55		2 uimsfb
	unsigned char __[3];
}TADTSHeader;
//Data structure defined area end
/***************************************************************************/

//MP4转换失败返回值定义
#define MP4_CONVERT_ERROR_CRYPT_FAILED          -2
#define MP4_CONVERT_ERROR_SPSPPS_CONTINUE       -3
#define MP4_CONVERT_ERROR_FATAL                 -4
#define MP4_CONVERT_ERROR_TRYNEXT               -5
#define MP4_CONVERT_ERROR_OVER_CURCONVERT       -6

//光盘修复返回值定义
#define MP4_DIRBURN_READCD_FAILED  				-11
#define MP4_DIRBURN_READCD_NOTENOUGH 			-12
#define MP4_DIRBURN_READCD_OVER    				-13
#define MP4_DIRBURN_NOT_CONFORM 				-14


#ifdef ALONE
//附加字符信息结构
typedef struct
{
	td_s32		 m_iFlag;
	td_u32 		 m_iSize;
	td_u8		 m_pcCharInfo[0];
}TAppendCharInfo;

//I帧索引信息结构
typedef struct
{
#ifdef FOR_MP4SDK
	td_u32 		m_iTimeNow;
#else
	time_t		m_iTimeNow;
#endif
	td_u32		m_iPos;
	td_u32 		m_iSign;//0代表未标记，1代表标记//add by gyn
	td_u32      m_iVdeoSize;
	td_u8		m_cTagName[64];//录像标签名
}TIFrameIndexInfo;

 //虚拟帧头结构体
typedef struct
{
	S_FrameHeader  	m_stHead;//枕头
	td_s32 			m_iNum;//I帧数目
	td_s32			m_iInfoSize;//I帧信息大小
	td_s32          m_iMemNum;
}TIFrameIndexHead;

//虚拟帧尾结构体
typedef struct
{
	td_s32		m_iFlag;
	td_s32      m_iSize;//索引帧大小
}TIFrameIndexTail;
#endif

 //---------------------------------------------------------------------------------------------

typedef struct
{
	 S_FileHeader	  m_stSdvFileHead;   	/*用于保存原SDV 文件头*/
	 int	          m_iFileEndTime;	    /*用于保存原SDV 文件结束时间*/
	 int              m_iReserveFlag;       /*待用标志*/
}TExRecInfo;

 //MP4虚拟帧头结构体
typedef struct
{
	 TIFrameIndexHead m_stSdvIndexHead;//原SDV 索引头
	 TExRecInfo 	  m_stExRecInfo;//额外记录的原SDV 文件信息
}TMP4ISampleIndexHead;

 //虚拟帧尾结构体
typedef struct
{
	td_s32		m_iFlag;
	td_s32      m_iSize;//索引帧大小
	//td_u8 		m_cCDSerialNo[MAX_CDROMSERIALNO_LEN];
}TMP4ISampleIndexTail;

 //扩展MP4 索引信息
typedef struct
{
	 int				 m_iSampleID;		 //帧序号
#ifdef FOR_MP4SDK
	 td_u32 			 m_tSampleTimeStamp; // 时间戳
#else
	 time_t 			 m_tSampleTimeStamp; // 时间戳		
#endif
 }TMP4ISampleExIndexInfo;

 //MP4索引帧信息结构单元
typedef struct
{
	 TIFrameIndexInfo		 m_stSdvIndexInfo;	 // SDV索引信息
	 unsigned int            m_uiUselessCount;    // 无效帧帧号
	 TMP4ISampleExIndexInfo  m_stVideoSampleIndexInfo;	 //视频索引信息
	 TMP4ISampleExIndexInfo  m_stAudioSampleIndexInfo;	 //音频索引信息
 }TMP4ISampleIndexInfo;

//附加字符信息结构
typedef struct
{
	td_s32		 m_iFlag;
	td_u32 		 m_iSize;
	td_u8		 m_pcCharInfo[MAX_RESERVED_INFO_LEN];
}TAppendCharInfoEx;

typedef struct
{
	char                 	m_cBoxheader[MP4_BOX_SIEZ_LENGTH+MP4_SAMPLE_TYPE_LENGTH];
	TMP4ISampleIndexHead 	m_stMP4ISampleIndexHead;
	TMP4ISampleIndexInfo*	m_pMP4ISampleMem;//虚拟信息缓存
	TAppendCharInfoEx    	m_stMP4Appendinfo;
	TMP4ISampleIndexTail 	m_stMP4ISampleIndexTail;
}TMP4VirFrameInfo; 

typedef struct
{
	int m_iStartTime;
	int m_iEndTime;
}TSdvFileTime;

typedef struct
{
	int m_iStartTime;
	int m_iEndTime;
}TAdjournInfo;

typedef struct
{
	int         m_iFlag;          //标志0x0123abcf
	int 		m_iCounts;        //转换的小文件个数
	int 		m_iMemNum;        //申请TSdvFileTime 内存个数
	TSdvFileTime* 	m_pSdvFileTimeMem;//转换文件时间信息buf
}TSdvTimeSumInfo;
 
//---------------------------------------------------------------------------------------------
#define MAX_MP4_FILE_HEADER_SIZE       	1024
#define MAX_MP4_File_TAIL_SIZE         	512*1024
#define MAX_MP4_CONVSESSION_NUM     	2
#define MAX_RELLOC_MEMNUM               500
#define MAX_ADJOURN_TIMES               50

#ifndef FILE_HEADER_COVER_WORD
#define FILE_HEADER_COVER_WORD	"S MULTI-MEDIA STREAM (H.264)"
#endif

typedef enum
{
	DIRBURN_WITHSATA = 1,//有硬盘直刻
	DIRBURN_WITHOUTSATA,//无硬盘直刻
}MP4ConvertType;

typedef enum
{
	VIDEODATA = 1,//视频数据
	AUDIODATA,   //音频数据
}DataType;

typedef enum
{
	READFILE = 1,//读文件
	READCDROM,   //读光盘
}ReadType;

typedef enum
{
	CHINESE = 1,//汉语
	ENGLISH, //英语
}LanguageType;

#ifdef FOR_MP4SDK
#pragma pack (1)
typedef struct
{
	int                     m_iUsing;                   /*是否正在使用*/
	unsigned int       		m_uiDevNo;					/*   本回话对应光驱号*/
	int                		m_iConvertNO;				/*   转换的文件序号*/
	int                		m_iConvertType;				/*   转换类型*/
	unsigned int            m_uiEncryptFlag;			/*   是否加密0、不加密1、加密*/
	unsigned int            m_uiAudioType;				/*   音频类型*/
	int                		m_iStopConvert;				/*   强制停止标志*/
	int	               		m_iFileBeginTime;			/*   总MP4文件的转换开始时间*/
#ifdef TD_VFS
	TVfile* 				m_CurMP4Handle;		  		/*   当前MP4纯数据文件句柄*/
#else
	FILE* 					m_CurMP4Handle;		
#endif
	TMP4Mgr            		m_stMP4Manager;
	TSdvTimeSumInfo     	m_stSdvTimeSumInfo;      /*   刻录大文件中的休庭信息*/
	unsigned long long 		m_ullFileSize;				/*   当前记录总文件大小*/	
	unsigned long long    	m_ullVideoTotalDuration;	/*   转换的总视频持续刻度*/
	unsigned long long    	m_ullAudioTotalDuration;	/*   转换的总音频持续刻度*/
	unsigned char*          m_puVirFrameBuf;        /*   MP4 虚拟帧缓存*/
	pthread_mutex_t    		m_SesssionLock;
}TMP4ConvertSession;
#pragma pack ()

#else
typedef struct
{
	int                     m_iUsing;                   /*是否正在使用*/
	unsigned int       		m_uiDevNo;					/*   本回话对应光驱号*/
	int                		m_iConvertNO;				/*   转换的文件序号*/
	int                		m_iConvertType;				/*   转换类型*/
	unsigned int            m_uiEncryptFlag;			/*   是否加密0、不加密1、加密*/
	unsigned int            m_uiAudioType;				/*   音频类型*/
	int                		m_iStopConvert;				/*   强制停止标志*/
	int	               		m_iFileBeginTime;			/*   总MP4文件的转换开始时间*/
#ifdef TD_VFS
	TVfile* 				m_CurMP4Handle;		  		/*   当前MP4纯数据文件句柄*/
#else
	FILE* 					m_CurMP4Handle;		
#endif
	TMP4Mgr            		m_stMP4Manager;
	TSdvTimeSumInfo     	m_stSdvTimeSumInfo;      /*   刻录大文件中的休庭信息*/
	unsigned long long 		m_ullFileSize;				/*   当前记录总文件大小*/	
	unsigned long long    	m_ullVideoTotalDuration;	/*   转换的总视频持续刻度*/
	unsigned long long    	m_ullAudioTotalDuration;	/*   转换的总音频持续刻度*/
	unsigned char*          m_puVirFrameBuf;        /*   MP4 虚拟帧缓存*/
	pthread_mutex_t    		m_SesssionLock;
}__attribute__((packed))TMP4ConvertSession;
#endif
//---------------------------------------------------------------------------------------------
int td_printf_mp4log(unsigned int handle, char* pszfmt, ...);

void* td_mp4_fopen(void* _pFileParam, const char* _cFileName, char* _cMode);

int td_mp4_fread(void *ptr, size_t size, size_t nmemb, void *stream);

int td_mp4_fwrite(const void *ptr, size_t size, size_t nmemb, void *stream);

#ifdef FOR_MP4SDK

int td_mp4_fseek(void *stream, __int64 offset, int whence);

__int64 td_mp4_ftell(void *stream);
#else


//int td_mp4_fseek(void *stream, long offset, int whence);

int td_mp4_fseek(void *stream, off_t offset, int whence);

//long td_mp4_ftell(void *stream);

off_t td_mp4_ftell(void *stream);
#endif

int td_mp4_feof(void *stream);

int td_mp4_fclose(void *fp);

int td_mp4_stat(char* _cFileName, void*_buf);

int td_mp4_unlink(char* _cFileName);
//---------------------------------------------

int MP4_GetMP4FreeVerSion(char* _pcFileName, int _iFreeNo, unsigned int* _puiVersion);

//设置强制停止转换状态，调用时需保证转换接口串行调用
int MP4_SetForceStopState(int _iState);//0 不停止1 停止

int MP4_GetForceStopState();

/************************************************* 
  Function: MP4_ConvertSdvToMp4
  Author: 黄志刚(Huangzhigang)
  Description: 用于将SDV 文件转换成标准MP4 文件
  Input: _pcSrcFilePath ，源SDV 文件路径
  Input: _pcDstFilePath ，目标待生成的MP4 文件路径
  Output:
  Return: 函数执行成功返回 -1, 失败返回 0
  Others: 
*************************************************/
int MP4_ConvertSdvToMp4(const char* _pcSrcFilePath, char* _pcDstFilePath);
int MP4_ConvertSdvToMp4Ex(const char* _pcSrcFilePath,  char* _pcDstFilePath, unsigned int _uiStartPos, unsigned int _uiEndPos);
//判断是否为MP4 文件
int MP4_IsMP4File(char* _pcFileName);
//判断文件是否已完整转换
int MP4_CheckFileConvertState(char* _pcFileName);
//获取媒体TrackId
int MP4_GetTrackId(char* _pcFileName, unsigned int* _puTrackId, char* _pcTrackType);
int MP4_GetTrackId_V2(void* _pMP4Handle, unsigned int* _puTrackId, char* _pcTrackType);
//获取相应Track的sample总数
int MP4_GetTrackSampleNums(char* _pcFileName, unsigned int _uTrackId, unsigned int* _piNums);
//获取文件SPS、PPS信息
int MP4_GetSpsPps(char* _pcFileName, char* _pcSps, int* _piSpsSize, char* _pcPps, int* _piPpsSize);
int MP4_GetSpsPps_V2(void* _pMP4Handle, char* _pcSps, int* _piSpsSize, char* _pcPps, int* _piPpsSize);

//获取文件VPS、SPS、PPS信息
int MP4_GetVpsSpsPps(char* _pcFileName, char* _pcVps, int* _piVpsSize, char* _pcSps, int* _piSpsSize, char* _pcPps, int* _piPpsSize);
int MP4_GetVpsSpsPps_V2(void* _pMP4Handle, char* _pcVps, int* _piVpsSize, char* _pcSps, int* _piSpsSize, char* _pcPps, int* _piPpsSize);
//获取MP4 文件中的音频类型
int MP4_GetAudioType(char* _pcFileName, int* _piAudioType);
//获取mp4文件的加密状态
int MP4_GetMP4FileEncryptStat(char* _pcFileName, int* _piEncryptStat);
//读取文件索引信息(第一个参数为文件名，每调一次，会执行一遍
//MP4Read，对大文件而言耗时较长)
int MP4_ReadSampleIndex(char* _pcFileName, 
								TMP4ISampleIndexHead *_pstMP4ISampleIndexHead, 
								TMP4ISampleIndexTail* _pstMP4ISampleIndexTail, 
								void ** _pIMP4SampleIndex, 
								int* _piIMP4SampleIndexmollocLen);
//读取文件索引信息(第一个参数为文件句柄，减少耗时最长的MP4Read操作
//但对开源库的其他几个接口调用，加起来依然会耗时多秒钟)
int MP4_ReadSampleIndex_V2(void* _pMP4Handle, 
									TMP4ISampleIndexHead *_pstMP4ISampleIndexHead, 
									TMP4ISampleIndexTail* _pstMP4ISampleIndexTail, 
									void ** _pIMP4SampleIndex, 
									int* _piIMP4SampleIndexmollocLen);
//读取文件索引信息(按新的文件记录机制去解读虚拟帧，不调用一个开源库接口
//耗时最短，前提是新版本的大MP4文件)
int MP4_ReadSampleIndex_V3(char* _pcFileName, 
									TMP4ISampleIndexHead *_pstMP4ISampleIndexHead, 
									TMP4ISampleIndexTail* _pstMP4ISampleIndexTail, 
									void ** _pIMP4SampleIndex, 
									int* _piIMP4SampleIndexmollocLen);
#ifdef ZHENGFA
//获取文件CD 序列号
int MP4_GetFileCDSerialNo(char* _pcFileName, td_u8* _CDSerialNOBuf);
#endif
//获取文件结束时间
int MP4_GetFileEndTime(char* _pcFileName, time_t *_pFileEndTime);
//读取索引信息中保存的SDV 文件头
int MP4_ReadSdvFileHeader(char* _pcFileName, S_FileHeader* _pstFileHeader);
int MP4_ReadSdvFileHeader_V2(void* _pMP4Handle, S_FileHeader* _pstFileHeader);
//读取文件重点标记信息
int MP4_ReadSign(char*_pcFileName, void* _pMarkInfo);
//定位MP4 文件
int MP4_SeekFile(TMP4ISampleIndexHead *_pstMP4ISampleIndexHead, 
				TMP4ISampleIndexTail* _pstMP4ISampleIndexTail, 
				void * _pIMP4SampleIndex, 
				int _iPosOrTime, 
				unsigned int* _puiSeekVideoSampleId, 
				unsigned int* _puiSeeKAudioSampleId);
//V2版本，涉及到了SEI 等无效帧的计数
int MP4_SeekFile_V2(TMP4ISampleIndexHead *_pstMP4ISampleIndexHead, 
					TMP4ISampleIndexTail* _pstMP4ISampleIndexTail, 
					void * _pIMP4SampleIndex, 
					int _iPosOrTime, 
					unsigned int* _puiSeekVideoSampleId, 
					unsigned int* _puiSeeKAudioSampleId,
					unsigned int* _puiUselessCount);
//根据时间定位帧号
unsigned int MP4_GetSeekPosSampleId(void* _hFile, unsigned int _trackId, unsigned long long _when, int _wantSyncSample);
//打开MP4 文件
int MP4_OpenFile(void** _MP4fp, char* _pcFileName);
//关闭MP4 文件
int MP4_CloseFile(void** _MP4fp);
//从MP4 文件中获取一帧私有帧
int MP4_GetOneTiandyFrame(void* _pMP4fp, 
									 unsigned int _u32VideoTrackId, 
									 unsigned int _u32AudioTrackId,
									 unsigned int* _puiCurVideoSampleId, 
									 unsigned int* _puiCurAudioSampleId, 
									 char* _pSps, 
									 int _iSpsSize, 
									 char* _pPps, 
									 int _iPpsSize,
									 void* _pTiandyFrameBuf, 
									 S_FrameHeader* _pstFrmaeHeader, 
									 int _iAudioType);
//从MP4 文件中获取下一个I  私有帧
int MP4_GetOneITiandyFrame(void* _pMP4fp, 
									 unsigned int _u32VideoTrackId, 
									 unsigned int _u32AudioTrackId,
									 unsigned int* _puiCurVideoSampleId, 
									 unsigned int* _puiCurAudioSampleId, 
									 char* _pSps, 
									 int _iSpsSize,
									 char* _pPps, 
									 int _iPpsSize,
									 void* _pTiandyFrameBuf, 
									 S_FrameHeader* _pstFrmaeHeader, 
									 int _iAudioType);
//从MP4 文件中获取一帧私有帧，包含SEI 数据
int MP4_GetOneTiandyFrame_V2(void* _pMP4fp, 
										 unsigned int _u32VideoTrackId, 
										 unsigned int _u32AudioTrackId,
										 unsigned int* _puiCurVideoSampleId, 
										 unsigned int* _puiCurAudioSampleId, 
										 char* _pSps, 
										 int _iSpsSize, 
										 char* _pPps, 
										 int _iPpsSize,
										 void* _pTiandyFrameBuf, 
										 S_FrameHeader* _pstFrmaeHeader, 
										 int _iAudioType, 
										 unsigned int* _puiUseLessIdCount, 
										 int _iNeedIFrame);
//从加密MP4文件中获取一个天地私有帧
td_s32 MP4_GetOneEncryptTiandyFrame(void* _pMP4fp,  
												 unsigned int _u32VideoTrackId, 
												 unsigned int _u32AudioTrackId,
												 unsigned int* _piCurVideoSampleId, 
												 unsigned int* _piCurAudioSampleId, 
												 void* _pTiandyFrameBuf, 
												 S_FrameHeader* _pstFrmaeHeader, 
												 int _iAudioType,
												 int _iNeedIFrame);

int MP4_GetOneTiandyFrameH265(void* _pMP4fp, 
									 unsigned int _u32VideoTrackId, 
									 unsigned int _u32AudioTrackId,
									 unsigned int* _puiCurVideoSampleId, 
									 unsigned int* _puiCurAudioSampleId,
									 char* _pVps, 
									 int _iVpsSize, 
									 char* _pSps, 
									 int _iSpsSize, 
									 char* _pPps, 
									 int _iPpsSize,
									 void* _pTiandyFrameH265Buf, 
									 S_FrameHeader* _pstFrmaeHeader, 
									 int _iAudioType);
int MP4_GetOneITiandyFrameH265(void* _pMP4fp, 
									 unsigned int _u32VideoTrackId, 
									 unsigned int _u32AudioTrackId,
									 unsigned int* _puiCurVideoSampleId, 
									 unsigned int* _puiCurAudioSampleId,
									 char* _pVps, 
									 int _iVpsSize,
									 char* _pSps, 
									 int _iSpsSize,
									 char* _pPps, 
									 int _iPpsSize,
									 void* _pTiandyFrameH265Buf, 
									 S_FrameHeader* _pstFrmaeHeader, 
									 int _iAudioType);
int MP4_GetOneTiandyFrameH265_V2(void* _pMP4fp, 
										 unsigned int _u32VideoTrackId, 
										 unsigned int _u32AudioTrackId,
										 unsigned int* _puiCurVideoSampleId, 
										 unsigned int* _puiCurAudioSampleId, 
										 char* _pVps, 
										 int _iVpsSize, 
										 char* _pSps, 
										 int _iSpsSize, 
										 char* _pPps, 
										 int _iPpsSize,
										 void* _pTiandyFrameH265Buf, 
										 S_FrameHeader* _pstFrmaeHeader, 
										 int _iAudioType, 
										 unsigned int* _puiUseLessIdCount, 
										 int _iNeedIFrame);
td_s32 MP4_GetOneEncryptTiandyFrameH265(void* _pMP4fp,  
												 unsigned int _u32VideoTrackId, 
												 unsigned int _u32AudioTrackId,
												 unsigned int* _piCurVideoSampleId, 
												 unsigned int* _piCurAudioSampleId, 
												 void* _pTiandyFrameH265Buf, 
												 S_FrameHeader* _pstFrmaeHeader, 
												 int _iAudioType,
												 int _iNeedIFrame);




//new interface
typedef int (*MP4BigFileWirteBuffer)(unsigned int _uiDevNo, unsigned char* _pcbuf, unsigned int _uiBufferLen);
typedef int (*MP4BigFileReadDisk)(unsigned int _uiDevNo, unsigned long long _ullAddress, unsigned char* _pcbuf, unsigned long long _ullLen);

//初始化MP4 转换模块
void MP4_BurnInit();
//注册MP4 写buf 回调函数
int MP4_RegWriteBufCallBack(MP4BigFileWirteBuffer _CallBack);
//注册MP4 读硬盘回调函数
int MP4_RegReadDiskCallBack(MP4BigFileReadDisk _CallBack);
//获取转换的尾大小
int MP4_GetConvertTailSize(void* _MP4File, unsigned long long* _pullTailSize);
//更新转换MP4 文件的时间
int MP4_DirBurnUpdateMP4FileTime(unsigned long long _ullMP4Time);
//获取MP4 文件头
void* MP4_DirBurnGetFileHeader(unsigned int _uiDevNo, unsigned int _uiMdatSize, int _iConvertType);
//获取MP4 文件数据
int MP4_DirBurnGetFileData(void* _pMP4FileHandle, char *_pSrcFile,  char* _pDstFile);
//获取MP4 文件尾
int MP4_DirBurnGetFileTail(void* _pMP4FileHandle, char* _pTailFilePath);
//断电修复MP4 文件
int MP4_DirBurnResum(unsigned int _uiDevNo, unsigned long long _iDiskTotalSize, char* _pFilePath);
//结束转换
int MP4_StopConvert(void* _pMP4FileHandle);
//获取所有转换过的sdv的文件时间
int MP4_GetSdvConvertInfo(char* _pFilename, int* _piCounts, char** _pcInfoBuf, int* _piInfoLen);
//获取大文件中的休庭时间
int MP4_GetFileAdjournTime(char* _pFilename, int* _piAdjournCnt, char** _pcAdjournInfo);
//断电修复相关
#ifdef FOR_MP4SDK
#pragma pack (1)
typedef struct 
{
	unsigned char AudioType;
	unsigned char ProfileLevel;
	unsigned char languagetype;
	unsigned char other;//字节对齐所需
	unsigned int timeScale;
	unsigned long long sampleDuration;
}TAudioTrackInfo;

typedef struct 
{
	unsigned char AVCProfileIndication;
	unsigned char profile_compat;
	unsigned char AVCLevelIndication;
	unsigned char sampleLenFieldSizeMinusOne;
	unsigned short sequenceLen;
	unsigned short pictLen;						
	unsigned short width;
	unsigned short height;
	unsigned int timeScale;
	unsigned long long sampleDuration;
	//unsigned char pSequence; 
	//unsigned char pPict;
}TVideoTrackInfo;

typedef struct
{
	unsigned int 	     uiSampleSize;      /* 大小*/
	unsigned long long   sampleDuration;/* 持续刻度*/
}TSizeAndDuring;


typedef struct
{
	int 			m_iFrameFlag;     /* 是否I 帧或音频(-1: 音频0:视频非I帧1:视频I 帧)*/
	TSizeAndDuring 	m_uiAVSizeAndDuring;      /*视频信息*/
}TAVSampleInfo;

//Box size 4, type 4       4 字节
//Custom Flag               8 字节
//vsersion                     4 字节

typedef struct
{
	unsigned int  m_uiSize;    /* 整个信息大小 */
	unsigned char m_ucType[4]; /* 类型 VIRTUAL_FRAME_TYPE  SAMPLE_OFFSET_TYPE SAMPLE_TS_TYPE AV_INFO_TYPE*/
	unsigned int  m_uiMemSize; /*单元信息的大小*/
	unsigned char m_pData[0];	    /* 单元信息数据*/
}TBaseUnit;

typedef enum
{
	BOXHEAD_HALF = 1,    
	BOXHEAD_WHOL, 
	SMALLMDAT,
	OTHER,
}AnalasyType;

typedef struct
{
	int                   m_iUsing;//此会话是否在使用
	unsigned int          m_uiDevNo;
	unsigned int 		  m_uiCurMP4BoxSize;
	unsigned int          m_uiLastBoxResumSum;//最后一个损坏box 补充数据总数
	unsigned int          m_uiLastBoxResumZero;//最后一个损坏box 补充0 数据总数
	int                   m_iFileBeginTime;
	unsigned int 		  m_VideoTrackID;    /*视频ID*/
	unsigned int          m_AudioTrackID;    /*音频ID*/
	unsigned int          m_AudioType;       /*音频类型*/
	unsigned int 	      m_uiCurBoxReadSum;
	unsigned long long    m_ullCDSize;
	unsigned long long    m_ullCDAddress;	
	unsigned long long    m_ullVideoTotalDuration;//修复记录总视频持续刻度
	unsigned long long    m_ullAudioTotalDuration;//修复记录总音频持续刻度
	TVideoTrackInfo       m_VideoTrackInfo;
	TAudioTrackInfo       m_AudioTrackInfo;	
	int                   m_iMoovOrFree;   //如果最后损坏的是moov 或free ，要注明此类型，如果不是，传默认值0
	unsigned int          m_uiMoovOrFreeLen;//最后损坏的moov 或free 长度
	void*				  m_MP4Handle;    /*MP4 文件句柄*/
	unsigned char*        m_pVideoTrackInfo;
	unsigned char*        m_pLastResumBox;
	unsigned char*		  m_puVirFrameBuf;		/*	 MP4 虚拟帧缓存*/
	unsigned char*        m_pDataBuf;
	pthread_mutex_t    	  m_SesssionLock;
}TMP4ResumSession;
#pragma pack ()
#else
typedef struct 
{
	unsigned char AudioType;
	unsigned char ProfileLevel;
	unsigned char languagetype;
	unsigned char other;//字节对齐所需
	unsigned int timeScale;
	unsigned long long sampleDuration;
}__attribute__((packed))TAudioTrackInfo;

typedef struct 
{
	unsigned char AVCProfileIndication;
	unsigned char profile_compat;
	unsigned char AVCLevelIndication;
	unsigned char sampleLenFieldSizeMinusOne;
	unsigned short sequenceLen;
	unsigned short pictLen;						
	unsigned short width;
	unsigned short height;
	unsigned int timeScale;
	unsigned long long sampleDuration;
	//unsigned char pSequence; 
	//unsigned char pPict;
}__attribute__((packed))TVideoTrackInfo;

typedef struct
{
	unsigned int 	     uiSampleSize;      /* 大小*/
	unsigned long long   sampleDuration;/* 持续刻度*/
}__attribute__((packed))TSizeAndDuring;


typedef struct
{
	int 			m_iFrameFlag;     /* 是否I 帧或音频(-1: 音频0:视频非I帧1:视频I 帧)*/
	TSizeAndDuring 	m_uiAVSizeAndDuring;      /*视频信息*/
}__attribute__((packed))TAVSampleInfo;

//Box size 4, type 4       4 字节
//Custom Flag               8 字节
//vsersion                     4 字节

typedef struct
{
	unsigned int  m_uiSize;    /* 整个信息大小 */
	unsigned char m_ucType[4]; /* 类型 VIRTUAL_FRAME_TYPE  SAMPLE_OFFSET_TYPE SAMPLE_TS_TYPE AV_INFO_TYPE*/
	unsigned int  m_uiMemSize; /*单元信息的大小*/
	unsigned char m_pData[0];	    /* 单元信息数据*/
}__attribute__((packed))TBaseUnit;

typedef enum
{
	BOXHEAD_HALF = 1,    
	BOXHEAD_WHOL,  
	SMALLMDAT,
	OTHER,
}AnalasyType;

typedef struct
{
	int                   m_iUsing;//此会话是否在使用
	unsigned int          m_uiDevNo;
	unsigned int 		  m_uiCurMP4BoxSize;
	unsigned int          m_uiLastBoxResumSum;//最后一个损坏box 补充数据总数
	unsigned int          m_uiLastBoxResumZero;//最后一个损坏box 补充0 数据总数
	int                   m_iFileBeginTime;
	unsigned int 		  m_VideoTrackID;    /*视频ID*/
	unsigned int          m_AudioTrackID;    /*音频ID*/
	unsigned int          m_AudioType;       /*音频类型*/
	unsigned int 	      m_uiCurBoxReadSum;
	unsigned long long    m_ullCDSize;
	unsigned long long    m_ullCDAddress;	
	unsigned long long    m_ullVideoTotalDuration;//修复记录总视频持续刻度
	unsigned long long    m_ullAudioTotalDuration;//修复记录总音频持续刻度
	TVideoTrackInfo       m_VideoTrackInfo;
	TAudioTrackInfo       m_AudioTrackInfo;	
	int                   m_iMoovOrFree;   //如果最后损坏的是moov 或free ，要注明此类型，如果不是，传默认值0
	unsigned int          m_uiMoovOrFreeLen;//最后损坏的moov 或free 长度
	void*				  m_MP4Handle;    /*MP4 文件句柄*/
	unsigned char*        m_pVideoTrackInfo;
	unsigned char*        m_pLastResumBox;
	unsigned char*		  m_puVirFrameBuf;		/*	 MP4 虚拟帧缓存*/
	unsigned char*        m_pDataBuf;
	pthread_mutex_t    	  m_SesssionLock;
}__attribute__((packed))TMP4ResumSession;

#endif

//==========================mp4 decode start=============================================


typedef void*       td_MP4FileHandle;

typedef struct{
	TMP4ISampleIndexHead	m_stTMP4ISampleIndexHead;
	TMP4ISampleIndexTail	m_stTMP4ISampleIndexTail;
}TMP4FileHead;

typedef enum{
	SEEK_FORWARD,
	SEEK_BACKWARD,
	SEEK_ABSOLUTE,		
}ESeekOps;

td_MP4FileHandle * MP4_Open (td_char *_pcFileName);
td_s32 MP4_Close (td_MP4FileHandle *_pMP4fh);
td_s32 MP4_GetFileHead(td_MP4FileHandle *_pMP4fh, TMP4FileHead *_pstMP4FileHead);
td_s32 MP4_Read (td_MP4FileHandle *_pMP4fh, td_void *_pFrameBuf, td_u32 _bufLen);
td_s32 MP4_Seek(td_MP4FileHandle *_pMP4fh, td_u64 _frameId, ESeekOps _type);


//==========================mp4 decode end==============================================


#endif //__TD_MP4_H_INCLUDED__

