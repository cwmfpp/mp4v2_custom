/*********************************************************************************
[File information]
FileName = PublicDef.h
Description =  head file
Corp.= Tianjin Tiandy Tech
Author = george
Date= 2006-09-01
[History] 	
XH1=XM20060901,george
*********************************************************************************/
#ifndef _PUBLICDEF_H_
#define _PUBLICDEF_H_

#include <assert.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <stdio.h>

#ifdef MEMWATCH
#include "memwatch.h"
#endif

#define UILOCALSET -1
#define SYSTEMSTAT -2 

//#define OverLapBurn
#define VIOLENCE_DEBUG
#define DOUBLE_TEST
#define AUDIO_MAXCHN		(4) 
#define AUDIO_LEFTCHN		0x02
#define AUDIO_RIGHTCHN	0x01

#define AUDIO_LEFTCHN1		0x02
#define AUDIO_RIGHTCHN1	0x01


#define AO_DEVICE_LEFT(Device)	((AUDIO_LEFTCHN << 16) | Device)  //×óÉùµÀ
#define AO_DEVICE_RIGHT(Device)	((AUDIO_RIGHTCHN << 16) | Device)  //ÓÒÉùµÀ
#define AO_DEVICE_DUALTRACK(Device)	(((AUDIO_LEFTCHN |AUDIO_RIGHTCHN) << 16) | Device) //Ë«ÉùµÀ

#define IN const
#define OUT

#define HI3510		3510
#define HI3511		3511
#define HI3512		3512	
#define HI3515		3515
#define HI3515A		35150
#define HI3520		3520
#define HI3521		3521
#define HI3520A		35200
#define HI3520D		35203
#define HI3531		3531
#define HI3535		3535
//ÌÕÓÀÁÁ 100727 blade_all
#define HI3520SC		35201

#define ALIGN_UP(x, a)				((x+a-1)&(~(a-1)))
#define ALIGN_BACK(x, a)			((a) * (((x) / (a))))

/*
 * The basic data type defination
 */
#define PRODUCT_CHN_1	1
#define PRODUCT_CHN_4	4
#define PRODUCT_VGA		6
#define PRODUCT_DVR_4	14
#define PRODUCT_DVR_8	18
#define PRODUCT_DVR_16	26
#define		CRE 	"^M^[[K"                                                                    
#define 	NORMAL	"[0;39m"                                                               
#define		RED		"[1;31m"                                                                  
#define		GREEN	"[1;32m"                                                                
#define		YELLOW	"[1;33m"                                                               
#define		BLUE	"[1;34m"                                                                 
#define		MAGENTA	"[1;35m"                                                              
#define		CYAN	"[1;36m"                                                                 
#define		WHITE	"[1;37m"

//string length
#define SHORT_PATH_LEN		16		//¶ÌÂ·¾¶³¤¶È
#define FILE_NAME_LEN		32		//ÎÄ¼þÃû³¤¶È
#define MIDDLE_PATH_LEN		32		//ÖÐÂ·¾¶³¤¶È
#define	FULL_PATH_LEN		64		//È«Â·¾¶³¤¶È
#define MAX_ALIAS_LEN		64		//±ðÃû³¤¶È£¬°üÀ¨ÊÓÆµÍ¨µÀ¡¢±¨¾¯ÊäÈë¡¢±¨¾¯Êä³ö

#define INT_NUM_STR_LEN		16		//ÕûÊý×Ö·û´®³¤¶È dxl 20110722

#define MAX_EXTNAME_LEN     15		//×î´óÂ¼ÏñÀ©Õ¹Ãû³¤¶È
//disk num
#define MAX_SATA_NUM		8		//Ö§³Ö×î´óµÄSATAÓ²ÅÌ¸öÊý
#define MAX_USB_NUM			4		//Ö§³Ö×î´óµÄUSB´ÅÅÌ¸öÊý
#define MAX_NFS_NUM			1		//Ö§³Ö×î´óµÄNFS¸öÊý
#define MAX_ESATA_NUM		1		//Ö§³Ö×î´óµÄeSATA¸öÊý
#define MAX_HDD_NUM	        8		//Ö§³Ö×î´óµÄ±¾µØÓ²ÅÌ¸öÊý
#define MAX_RECORDER_NUM	2		//Ö§³Ö×î´óµÄ¿ÌÂ¼»úÊý
#define MAX_SD_NUM			1		//Ö§³Ö×î´óµÄSDÊý
#define MAX_PART_NUM		4		//Ã¿¸ö´ÅÅÌÖ§³ÖµÄ×î´ó·ÖÇøÊý
#define MAX_DISK_NUM		(MAX_SATA_NUM + MAX_USB_NUM + MAX_NFS_NUM + MAX_ESATA_NUM + MAX_SD_NUM)
									//Ö§³ÖµÄ×î´ó´ÅÅÌ¸öÊý
//device parameter
#define MAX_CHANNEL_NUM		32					//×î´óÍ¨µÀÊýÄ¿
#define MAX_INPORT_NUM		64					//×î´ó±¨¾¯ÊäÈë¶Ë¿ÚÊý
#define MAX_OUTPORT_NUM		32					//×î´ó±¨¾¯Êä³ö¶Ë¿ÚÊý
#define MAX_SERIAL_NUM		5					//×î´ó´®¿ÚÊý,ÆäÊµÖ»ÓÐÇ°4¸ö´®¿ÚÊÇÓÃÓÚPTZ¿ØÖÆµÄ£¬×îºóÒ»¸öÔ¤Áô

#define MAX_VIDEO_COVER_NUM	4					//×î´óÊÓÆµÕÚµ²ÇøÓòÊý
#define MAX_TIME_SEGMENT	4					//×î´óÊ±¼ä¶Î¸öÊý

#define MAX_VCHN_CNT		32					//ÐéÄâºÏ³ÉÍ¨µÀ×î´óµÄºÏ³É»­ÃæÊý
#define ENCRYPT_PWD_LENGTH		16			//ÊÓÆµ¼ÓÃÜÃÜÂë³¤¶È
//20120913 ÍõÀÚadd
#define LOG_ITEM_LEN		(FULL_PATH_LEN + 8)	//logÎÄ¼þ±£´æµÄÃ¿¸öÌõÄ¿µÄ³¤¶È£¬ÎªÂ¼ÏñÎÄ¼þÈ«Â·¾¶¼ÓÉÏÂ¼Ïñ´óÐ¡
//add end

//limits
#define MAX_VIDEO_QUALITY	14					//×î´óÊÓÆµÖÊÁ¿
#define MIN_VIDEO_QUALITY	6					//×îÐ¡ÊÓÆµÖÊÁ¿

#define RETURN_NONE			0x12345678			//Magic Number,ÓÃÆäÅÐ¶ÏÔÚtd_assertÖÐÊÇ·ñÐèÒª·µ»ØÖµ

#define ENCRYPT_PWD_LENGTH		16				//ÊÓÆµ¼ÓÃÜµÄÃÜÂë×î´óÎª16×Ö½Ú	
#define ENCRYPT_VIDEO_LENGTH	64				//¶ÔÊÓÆµÖ¡ÖÐµÄÇ°64×Ö½Ú½øÐÐ¼ÓÃÜ

//ÌÕÓÀÁÁ 100322 ·Ç·¨µÄ×Ö·ûµþ¼ÓÎ»ÖÃ¡£ºÏ·¨µÄÎ»ÖÃÎª0~15
#define INVALID_OSD_POS				0xFF


// dxl 20110307 ´Óassiatan.hÒÆµ½´Ë´¦
#define VIDEO_WIDTH_BASE	1000
#define VIDEO_HEIGHT_BASE	1000

//added by wanzy 20110902
//shihc mod 20130222
#define CHNNAME_LEN	65
#define CHNNAME_LEN_OLD 33

#define MAX_FRAMERATE_NTSC (Cfg_gMaxFrameSupport())
#define MAX_FRAMERATE_PAL (MAX_FRAMERATE_NTSC - (Cfg_gMaxFrameSupport() / 30) * 5)
#define MAX_FRAMERATE MAX_FRAMERATE_NTSC

#define ENCRYPT_STRING	"\1"
#define ENCRYPT_CHAR	'\1'
//ÔËÐÐ×´Ì¬£¬ÐÄÌøÊ±·¢ËÍ,Add by Leeon Tonie, 081219
typedef enum
{
	STATUS_NORMAL = 0,		//Õý³£ÔËÐÐ
	STATUS_EXIT,			//ÍË³ö³ÌÐò
	STATUS_RESTART,			//ÖØÆô³ÌÐò
	STATUS_HOST_READY,		//Ö÷Æ¬Æô¶¯Íê±Ï£¬¿ªÊ¼ÐÄÌø
}THeartBeat;

extern FILE* fpDebugFile;
extern FILE* fpThreadFile;
extern unsigned int g_iProcThreadNum;
#define MAX_CDROMSERIALNO_LEN 32
//´Ónet_public.hÖÐÒÆ¶¯µ½public_def.hÖÐ£¬ÒÔºóËùÓÃº¯Êý¶¼¿ÉÒÔµ÷ÓÃ
#define ClearString(str, size)	*((char *)(str)) = '\0'
//ÌÕÓÀÁÁ 091117 ºÏ·¨µÄÄÚ´æµØÖ··¶Î§
#define valid_logic_addr(pointer)	((void*)pointer >= (void*)0x00008000 && (void*)pointer <= (void*)0xc0000000)
//#define debug_info(string)	printf(MAGENTA"[%s || %d || %s || PID = %d || errno = %d] %s\r\n"NORMAL, __FILE__, __LINE__, __func__, getpid(), errno, string)
#if 0
#define get_tid()	pthread_self()
#else
#define get_tid()	syscall(224)
#endif

#ifdef TD_DEBUG
#define func_info()\
	do{\
		time_t tUniqueName = time(NULL);\
		printf(BLUE"%s"NORMAL, ctime(&tUniqueName));\
		printf(BLUE"PID = %d, PPID = %d, Thread ID = %lu, Thread Name: %s\r\n"NORMAL, getpid(),getppid(), get_tid(), __func__);\
		printf(BLUE"Created at line %d, file %s\r\n"NORMAL, __LINE__, __FILE__);\
		printf(BLUE"=================================================================================\r\n"NORMAL);\
	}while(0)


		
#define start_exec(name)\
		struct timeval unique_start ## name;\
		gettimeofday(&unique_start ## name, (struct timezone *)NULL);
		
#define stop_exec(name)\
	do{\
		struct timeval unique_stop ## name;\
		gettimeofday(&unique_stop ## name, (struct timezone *)NULL);\
		printf(CYAN"In function : %s, tid : %d your test %s costs %d microseconds\r\n"NORMAL,\
		__func__, (int)pthread_self(), #name, (int)(1000000*(unique_stop ## name.tv_sec - unique_start ## name.tv_sec) + \
		(unique_stop ## name.tv_usec - unique_start ## name.tv_usec)));\
	}while(0)
		
#define td_show(color, msg) printf(color"[%s || %d || %s] : %s\r\n"NORMAL, __FILE__, __LINE__, __func__, msg)
	#ifdef VIOLENCE_DEBUG
	#define debug_info(string)	printf(MAGENTA"[%s || %d || %s || PID = %d || errno = %d] %s\r\n"NORMAL, __FILE__, __LINE__, __func__, getpid(), errno, string)
	#endif

//ÌÕÓÀÁÁ 091029 DEBUGÏÂ¶ÏÑÔ£¬´òÓ¡³ö³ö´íÐÅÏ¢²¢·¢ËÍSIGABRTÐÅºÅ
#define td_assert(exp, errorno)	assert(exp)
//ÌÕÓÀÁÁ 100208 ÔÚvoidÐÍµÄº¯ÊýÖÐÊ¹ÓÃ¸Ãºê
#define td_assert_s(exp) td_assert(exp, RETURN_NONE)
//ÌÕÓÀÁÁ 100210 ÐÂÔötd_perror
#define td_perror(string) \
	td_printf(0xFF00000, "[%s]%s : %s", __func__, string, strerror(errno))
#else

#define func_info()\
	do{\
		if(fpThreadFile != NULL )\
		{\
			if(g_iProcThreadNum++ <= 200)\
			{\
				time_t tUniqueNameInFuncInfo = time(NULL);\
				fprintf(fpThreadFile, "%s", ctime(&tUniqueNameInFuncInfo));\
				fprintf(fpThreadFile, "%d, PID = %d, PPID = %d, Thread ID = %lu(%lu), Thread Name: %s\r\n", g_iProcThreadNum,getpid(),getppid(), get_tid(), pthread_self(), __func__);\
				fprintf(fpThreadFile, "Created at line %d, file %s\r\n", __LINE__, __FILE__);\
				fprintf(fpThreadFile, "=================================================================================\r\n");\
				fflush(fpThreadFile);\
			}\
		}\
	}while(0)

#define start_exec(name)
#define stop_exec(name)
#define td_show(color, msg)
	#ifdef VIOLENCE_DEBUG
	#define debug_info(string)
	#endif
	
//ÌÕÓÀÁÁ 091029 RELEASEÏÂ¶ÏÑÔ£¬½«³ö´íÐÅÏ¢¼ÇÂ¼µ½debug_info.txt²¢·µ»Øerrorno
#define td_assert(exp, errorno)\
	do{\
		if (!(exp))\
		{\
			if (fpDebugFile != NULL)\
			{\
				time_t tUniqueNameInTdAssert = time(NULL);\
				fprintf(fpDebugFile, "%s%s: %d: %s: Assertion \'%s\' failed.\r\n",\
				ctime(&tUniqueNameInTdAssert), __FILE__, __LINE__, __func__, #exp);\
			}\
			return (errorno);\
		}\
	}while(0)
//ÌÕÓÀÁÁ 100208 ÔÚvoidÐÍµÄº¯ÊýÖÐÊ¹ÓÃ¸Ãºê
#define td_assert_s(exp)\
	do{\
		if (!(exp))\
		{\
			time_t tUniqueNameInTdAssert = time(NULL);\
			if (fpDebugFile != NULL)\
			{\
				fprintf(fpDebugFile, "%s%s: %d: %s: Assertion \'%s\' failed.\r\n",\
				ctime(&tUniqueNameInTdAssert), __FILE__, __LINE__, __func__, #exp);\
			}\
			return;\
		}\
	}while(0)
#define td_perror(string)	
#endif

#define set1(byte, bitn) (byte) |= 0x1 << (bitn)		//½«byteµÄµÚbitnÎ»ÖÃ1
#define set0(byte, bitn) (byte) &= ~(0x1 << (bitn))		//½«byteµÄµÚbitnÎ»ÖÃ0
#define getbit(byte, bitn) (((byte) >> bitn) & 0x1)	//»ñÈ¡byteµÄµÚbitnÎ»
#define setbit1(byte, bitn, sta) ((byte) = (((byte) & (~(0x1 << (bitn)))) | ((sta) << (bitn))))	//½«byteµÄµÚbitnÎ»ÖÃÎªsta

//#define absollute()		 (int)(times(NULL) / 100)
typedef unsigned char		td_u8;
typedef unsigned char       td_uchar;
typedef unsigned short		td_u16;
typedef unsigned int		td_u32;
typedef unsigned long long 	td_u64;
typedef signed char			td_s8;
typedef short				td_s16;
typedef int					td_s32;
typedef long long			td_s64;


typedef char                td_char;
typedef float				td_float;
typedef double				td_double;
typedef void                td_void;
typedef long double			td_ldouble;
#define PACKED				__attribute__((packed))	

/*******************************************From 3510 below**********************************************************/
#define OSD_REGION_HANDLE td_u32
typedef struct							//Sherry modify 20081121
{
	td_u8 		m_u8Brightness;
	td_u8 		m_u8Hue;
	td_u8 		m_u8Contrast;
	td_u8 		m_u8Saturation;
	td_u8		m_u8StartTime[3];		//0:Hour 1:Minute 2:Second
	td_u8		m_u8StopTime[3];		//0:Hour 1:Minute 2:Second
	td_u8		m_u8CtoG;				//Leeon Added 090112, ²Ê×ª»Ò(Color/Gray)Ê¹ÄÜ
}structVideoParam;

typedef struct __RECT__
{
	unsigned short left;
	unsigned short top;
	unsigned short right;
	unsigned short bottom;
}Rect_t;

typedef struct
{
	td_u8		m_u8SensorFlipEnable;
	td_u8		m_u8SensorMirrorEnable;
	td_u8		m_u8SensorScene;
}SENSORPARA;

/*ÓÃÓÚµþ¼ÓÍ¨µÀÃû³Æ*/
typedef struct osd_text_struct
{
//	OSD_REGION_HANDLE osd_handle;     	/*´´½¨osd Ê±µ×²ã·µ»Ø*/
	int  	show;      					/*true = show £¬ÊÇ·ñÏÔÊ¾*/
	char	text[CHNNAME_LEN];					/*µþ¼Ó×Ö·ûÄÚÈÝ*/
	int	osdpos;          				/*µþ¼ÓÎ»ÖÃ0~15*/
	int 	osdX;						//µþ¼ÓÎ»ÖÃºá×ø±ê
	int 	osdY;						//µþ¼ÓÎ»ÖÃ×Ý×ø±ê
	td_u32	osdColor;      				//µþ¼Ó×Ö·ûÑÕÉ«
}OSD_CHANNAME_T;

/*******************************************From 3510 above**********************************************************/

/*----------------------------------------------*
 * The common constant                          *
 *----------------------------------------------*/
typedef enum {
    td_false    = 0,
    td_true     = 1,
} td_bool;

#ifndef NULL
#define NULL             0L
#endif

#define td_success          0 
#define td_failure          (-1)
//device capability
//ÌÕÓÀÁÁ 100727 blade_all
#define VIDEOENCODE_MAX_CHANNEL_NUM		32//16  //sherry 20100613 modify for 3520-single-chip
#define MAX_VCA_CHN_NUM    32
#define VIDEOINPUT_MAX_CHANNEL_NUM		32
#define AUDIOENCODE_MAX_CHANNEL_NUM	16
#define AUDIOINPUT_MAX_CHANNEL_NUM		16
#define VIDEOOUTPUT_MAX_CHANNEL_NUM		16    //gxl added 20091215
//#define GPIN_MAX_NUM	4
//#define GPOUT_MAX_NUM 4
//Cheng 2010/02/21 add talk channel
#define AUDIO_CHANNEL_TALK_NUM                    1

// for test
#ifdef TD_DEBUG
#define	MAX_CONNECTION		4		//²âÊÔÊ±Ê¹ÓÃ
#else
#define	MAX_CONNECTION		8		//Ã¿Ò»Í¨µÀ×î¶à¿ÉÓÐ8¸öÁ¬½Ó
#endif
#define   MAX_ENCODE_ABILITY	480        //×î´ó±àÂëÄÜÁ¦480 cif

#define NO_STREAM   		0
#define ONLY_VIDEO  		1
#define ONLY_AUDIO  		2
#define VIDEO_AUDIO 		3

#define NETMODE_TCP     	1
#define NETMODE_UDP     	2
#define NETMODE_MC      	3
#define NVS_ACTIVE      	4

#define BYTES_1K			(0x1 << 10)
#define BYTES_1M			(0x1 << 20)
#define BYTES_1G			((td_u64)0x1 << 30)
#define BYTES_1T			((td_u64)0x1 << 40)

#ifndef MIN
#define 	MIN(x,y) ((x) < (y) ? (x) : (y))
#endif
#ifndef MAX
#define 	MAX(x,y) ((x) > (y) ? (x) : (y))
#endif
#ifndef MIN_IN3
#define MIN_IN3(a,b,c) MIN(MIN(a,b),c)
#endif

#define MAX_FRAME_SIZE   BYTES_1M    // ×î´óÖ¡1M 

//Á¬½ÓÀàÐÍ
#define CONNECTMODE_CMD		1
#define CONNECTMODE_DATA		2
#define CONNECTMODE_DIDA		0

#define	 TD_AUTO     		1

#define DEVICE_DVRS 0
#define DEVICE_DECS 1

//²Üº£²¨Ìí¼Ó  ²éÑ¯Î´¼ÓËøÂ¼Ïñ
#define QUERY_RECFILE_UNLOCKED	(0x1 << 24)

typedef enum
{
	AUDIO_TYPE_G711A = 1,
	AUDIO_TYPE_G711U,
	AUDIO_TYPE_DVI4,
	//ljj add
	AUDIO_TYPE_MEDIA_G726_1_24 = 0x05,
	AUDIO_TYPE_MEDIA_G726_1_32 = 0x06,
	AUDIO_TYPE_MEDIA_G726_1_40 = 0x07,
	AUDIO_TYPE_MEDIA_AAC = 0x16, // huaqiang add for AAC 20130107
	AUDIO_TYPE_ADPCM_IMA = 0x23,
	//ljj add end 
	// zty 20120220
	AUDIO_TYPE_MEDIA_G726_16 = 0x24,
	AUDIO_TYPE_MEDIA_G726_24 = 0x25,
	AUDIO_TYPE_MEDIA_G726_32 = 0x26,
	AUDIO_TYPE_MEDIA_G726_40 = 0x27,
	// zty add end
	
	//ljj add 
	AUDIO_TYPE_PCM	= 0x33,
	AUDIO_BUTT,	

    //qiaohaijun for audio error
	AUDIO_TYPE_ERROR = 0xff
}AUDIO_TYPE_E;

typedef enum 
{
	ANALOG_CHN = 0,  //Ä£ÄâÍ¨µÀ
	DIG_CHN,		 //Êý×ÖÍ¨µÀ
	VOENC_CHN,       //VO¸´ºÏ±àÂëÍ¨µÀ
	BUTT,
}CHN_TYPE;

typedef enum
{
	VENC_H264 = 0,
	VENC_MJPEG,
	VENC_BUTT,
}VENC_TYPE_E;

typedef enum
{
	QCIF =0,
	HCIF,
	FCIF,
	HD1,
	FD1,
	MD1,
	QVGA,
	VGA,
	HVGA,
	F720P,
	F960P,
	F1200,
	F1080P,
	QXGA,
	QHD	= 0x0100,
	H960 = 0x0200,
	MA5 = 0x300,
	M5 = 0x400,
	QSXGA = 0x0500,
    //×ßÀÈÄ£Ê½·Ö±æÂÊ¸ñÊ½:100000 + N(NÎªÒÔÉÏ·Ö±æÂÊ´úÂë),ÒÔ´ËÀàÍÆ
    F720P_9_16 = 0x100009,    //720x 1280
    F1080P_9_16 = 0x10000C,	//1080x 1920
	VIEDOSIZE_BUTT,
}VIDEO_SIZE_E;

#define F1080I 40

#define CIF		FCIF
#define D1		FD1
#define SXGA		HVGA


//¼æÈÝ3520 SDIÉè±¸Ëù¸½¼ÓÁ½¸öºê¶¨Òå£¬ÆäËûÉè±¸²»ÒªÓÃ
#define    HD_SDI720P 		2
#define    HD_SDI1080P 	3
//end

typedef enum
{
	VI_NORM_PAL 		= 0x0000,
	VI_NORM_NTSC,
	VI_NORM_AUTO		= 0x0004,

	VI_NORM_960H_PAL 	= 0x0010,
	VI_NORM_960H_NTSC,
	
	VI_NORM_720P		= 0x040,//0x0100
	VI_NORM_1080P		= 0x050,//0x0110
	VI_NORM_BUTT,
}VI_NORM;

typedef enum //shihc add 20121206
{
	VI_1080P30 = 0,
	VI_1080P25,
	VI_1080I60,
	VI_1080I50,
	VI_720P60,
	VI_720P50,
	VI_720P30,
	VI_720P25,
	VI_1080P60,
	VI_1080P50,
} VI_HD_FROMAT_E;


typedef struct
{
    char    m_cDeviceId[64];
    char    m_cDeviceName[64];
    char    m_cNFSIp[16];
    char  	m_cNFSPath[255+1];
	int		m_iDevNo;								//Éè±¸±àºÅ£¬ÌÕÓÀÁÁ£¬2008-12-03£¬DVRÐÂÔö×Ö¶Î
	int 	m_blNfsEnable;							//NFSÊ¹ÄÜ
	int		m_iVspport;
	int		m_iVapport;
	char	m_cAccessnum[64];
}STR_COMMON_INFO;

#define MAX_PATH_LEN         (255+1)
#define USER_NAME_LEN       16
#define PASSWORD_LEN        16

typedef struct
{
	char    m_cManagerServerIp[16];
	long    m_lManagerServerPort;
	char    m_cHeartBeatServerIp[16];
	long    m_lHeartBeatPort;
	char    m_cRegisterServerIP[16];
	long    m_lRegisterPort;
	char     m_cEnableHeartBeat[16];
	int     m_iHeartBeatPeriod;
	int     m_iRegisterTime;
	char    m_cVapPath[MAX_PATH_LEN];
	char    m_authServerIp[16];
	long    m_lauthServerPort;
	char    m_authUserName[USER_NAME_LEN];
	char    m_authUserPassword[PASSWORD_LEN];
	char    m_cputhreshold;
	char	m_memthreshold;
}STR_NET_MANAGE_INFO;


#define VERSION_LEN     64
typedef struct
{
    char    m_csoftWareVersion[VERSION_LEN];
}STR_UPGRADE_INFO;

typedef struct
{
 	int    m_iFormat;
	char m_cSeperatechar[8]; 
}STR_DATE_FORMAT;
/**************************Other Attributes Begin*******************************/
//ÌÕÓÀÁÁ 091222 ±ðÃû¹ÜÀí
typedef enum
{
	ALIAS_CHANNEL = 0,		//Í¨µÀ
	ALIAS_ALARM_INPUT,		//±¨¾¯ÊäÈë
	ALIAS_ALARM_OUTPUT,		//±¨¾¯Êä³ö
}TAliasType;

typedef struct
{
	td_u8	m_cChannel[MAX_CHANNEL_NUM][MAX_ALIAS_LEN + 1];		//Í¨µÀ±ðÃû
	td_u8	m_cAlarmInput[MAX_INPORT_NUM][MAX_ALIAS_LEN + 1];	//±¨¾¯ÊäÈëÍ¨µÀ±ðÃû
	td_u8	m_cAlarmOutput[MAX_OUTPORT_NUM][MAX_ALIAS_LEN + 1];	//±¨¾¯Êä³öÍ¨µÀÃû
	td_s32	m_iChannelVer;									//Í¨µÀ±ðÃû°æ±¾
	td_s32	m_iAlarmInputVer;								//±¨¾¯ÊäÈëÍ¨µÀ±ðÃû°æ±¾
	td_s32	m_iAlarmOutputVer;								//±¨¾¯Êä³öÍ¨µÀ±ðÃû°æ±¾
}TAlias;

/**************************Other Attributes Begin*******************************/
/**************************Media Attributes Begin*******************************/
typedef enum hiFRAME_TYPE_E
{
    FRAME_I = 0,   /* I frame */
    FRAME_P,       /* P frame */
    FRAME_BUTT
} FRAME_TYPE_E;

//ÒôÊÓÆµÖ¡Í·
typedef struct
{
    unsigned int	u32FrameID ;	//±êÊ¾ID,32Î»,¹Ì¶¨Îª: 0x 00 00 00 02
    unsigned int	u32FrameSize;	//Ö¡´óÐ¡32Î»,Õû¸öÖ¡´óÐ¡,×Ö½ÚÊý
    unsigned char	u8FrameType;	//Ö¡ÀàÐÍ,8Î»,FRAME_I=0,  FRAME_P=1, FRAME_B=2 
    unsigned int	u32FrameNO;	//Ö¡ÐòºÅ,32Î»,Ö¡ÐòºÅ,Ñ­»·µÝÔö
    unsigned int	u32TimeStamp;	//Ê±¼ä´Á,32Î»,ÔÝÊ±²»ÖªµÀÓÐÊ²Ã´ÓÃ
    unsigned int	u32VStreamLen;	//ÊÓÆµÊý¾Ý³¤¶È,32Î»,ÊÓÆµÊý¾Ý´óÐ¡
    unsigned short	u16AFrames;	//ÒôÆµÖ¡Êý,16Î»,¸ÃÊÓÆµÖ¡Ð¯´øÒôÆµÖ¡Êý
    unsigned short	u16AFrameSize;	//ÒôÆµÖ¡´óÐ¡,16Î»,ÒôÆµÖ¡´óÐ¡
    unsigned char	u8ACoder;	//ÒôÆµÑ¹Ëõ·½Ê½,8Î»,²»Ñ¹Ëõ:0;G711_A:0x01; G711_U:0x02;ADPCM_A :0x03;G726:0x04
    unsigned short	reserved;		//16Î»,±£Áô,32Î»¶ÔÆë
}S_FrameHeader,*pS_FrameHeader;
//Ö¡Í·ºóÃæÏÈ¸úÕûÖ¡ÊÓÆµÊý¾Ý,ÔÙ¸úËùÓÐÒôÆµÊý¾Ý

#define MAX_COVWORD_LEN 36
//ÎÄ¼þÍ·½á¹¹
typedef struct
{
    unsigned short  FrameRate;	//Ö¡ÂÊ
    unsigned short  Width;		//Í¼Ïñ¿í
    unsigned short  Height;		//Í¼Ïñ¸ß
    unsigned short  Mode;		//±àÂëÄ£Ê½£º11£¨MP4)£¬ 21(H264)
    unsigned short  bAudio;		//ÊÇ·ñ´øÒôÆµ
    unsigned short  Year;		//H264±àÂë²»ÓÃÒÔÏÂÏÂ×Ö¶Î£¬È«²¿ÖÃ0xFF,¿ÉÒÔ²¹³äÎªÂ¼ÏñÊ±¼ä
    unsigned short  Month;
    unsigned short  Day;
    unsigned short  Hour;
    unsigned short  Minute;
    unsigned short  Second;
    unsigned short  CoverMask;
    char cCovWord[36];		//¹Ì¶¨´æ·Å×Ö·û´®"S MULTI-MEDIA STREAM (H.264)"¼Ó0x00½áÊø·û£¬Ê£Óà²¿·ÖÌî³ä0xFF
    unsigned int 	m_uiFrameNoDiff;	// ÎÄ¼þÊ×Î²Ö¡ºÅ²î
    unsigned int 	TriggerHigh;	// ¼ÇÂ¼±¨¾¯¶Ë¿ÚºÅ20120906 Ñîçå
    unsigned int 	TriggerLow;	// ¼ÇÂ¼±¨¾¯¶Ë¿ÚºÅ20120906 Ñîçå
    unsigned short	reserved;		//±ícCovWordµ±Ç°µÄ³¤¶È£¬µ±Ç°ÖµÎª36
    unsigned char   AChannels;   // ÉùµÀ1/2£¬Ä¬ÈÏÎª1
	unsigned char   BitsPerSample;// ²ÉÑùÎ»Êý8/16/24, Ä¬ÈÏ16
    unsigned int 	AudioSample;
    unsigned int    TotalSize;	//ÎÄ¼þ×Ü´óÐ¡(×Ö½ÚÊý)£¬ÀÏ°æ±¾±£Áô
    unsigned int    FrameCount;	//ÎÄ¼þ×ÜÖ¡Êý,ÀÏ°æ±¾±£Áô
}S_FileHeader,*pS_FileHeader;
/**************************Media Attributes End*******************************/
/**************************Alarm Attributes Begin*******************************/
//¼¸ÖÖ±¨¾¯ÀàÐÍ
#if 1 // modify by caizhaoxu 20120321
typedef enum
{
	ALM_VIDEO_LOST,								//ÊÓÆµ¶ªÊ§±¨¾¯,1
	ALM_PORT,									//¶Ë¿Ú±¨¾¯,0
	ALM_MD,										//ÒÆ¶¯Õì²â±¨¾¯,2
	ALM_VIDEO_COVER,							// video cover
	ALM_VCA,									// this value only for ensure ALM_AUDIO_LOST is 5
	//add by gyn 20120903
	ALM_AUDIO_LOST,                             //ÒôÆµ¶ªÊ§±¨¾¯
	//add end
	ALM_TEM_HUM,
	/****************************************************************
	* if you want to save your alarm info into config_alm.dat by type,
	* please add you type over this value
	*****************************************************************/
	ALM_SAVE_TO_CFG, 

	ALM_TW,                						/*°íÏß±¨¾¯ */
	ALM_PM,										/* ÖÜ½ç±¨¾¯*/
	ALM_OSCER,									/*ÎïÆ·¿´»¤*/
	//ALM_FACE,									/*ÈËÁ³¼ì²â*/
	ALM_DIAGNOSE,								/*ÊÓÆµÕï¶Ï*/
	ALM_CAP,								    //ÖÇÄÜ¸ú×Ù//shihc add 20130225
	ALM_CAP_td,
	ALM_CPC,									//Á÷Á¿Í³¼Æ
	ALM_CDD,                                    //ÈËÈº¾Û¼¯
	ALM_SVD,									//Àë¸Ú
	ALM_MAX,									//×î´óÖµ£¬¸ÃÖµÔÚÉùÃ÷Êý×éÊ±Ê¹ÓÃ£¬ÒªÀ©Õ¹ÄÚÈÝÇëÔÚ¸ÃÖµÒÔÉÏÌí¼Ó
}TAlarmType;

#else
typedef enum
{
	ALM_VIDEO_LOST,								//ÊÓÆµ¶ªÊ§±¨¾¯
	ALM_PORT,									//¶Ë¿Ú±¨¾¯
	ALM_MD,										//ÒÆ¶¯Õì²â±¨¾¯
	ALM_TW,                //zhangweiwei add
	ALM_PM,
	ALM_FACE,
	ALM_DIAGNOSE,
	ALM_MAX,									//×î´óÖµ£¬¸ÃÖµÔÚÉùÃ÷Êý×éÊ±Ê¹ÓÃ£¬ÒªÀ©Õ¹ÄÚÈÝÇëÔÚ¸ÃÖµÒÔÉÏÌí¼Ó
}TAlarmType;
#endif
/**************************Alarm Attributes End*******************************/
/**************************Hardware Attributes Begin*******************************/

/**************************Hardware Attributes End*******************************/

/**************************Com Attributes Begin*******************************/

//´®¿ÚµÄ¹¤×÷Ä£Ê½
typedef enum
{
	PTZ_PROTOCOL = 1,								//Ð­Òé¿ØÖÆÄ£Ê½
	PTZ_STREAM = 2,									//Í¸Ã÷Í¨µÀÄ£Ê½
	PTZ_RESERVED = 3,								//Ô¤Áô
	PTZ_7601B = 4,									//7601B´®ÐÐ×ÜÏß±¨¾¯Æ÷
	key_485 = 5,
	GPS_MOD = 6,									//³µÔØGPS
	ATM_PROTOCOL = 7,
	PTZ_WORKMODE_MAX,								//×î´óÖµ£¬Ìí¼ÓÆäËûÀàÐÍÇëÔÚÉÏÃæÌí¼Ó
}TComWorkMode;

//¼ÇÂ¼´®¿ÚÅäÖÃµÄ½á¹¹Ìå
typedef struct
{
	td_s32		m_iBaudRate;								//Éè±¸µÄ²¨ÌØÂÊ
	TComWorkMode	m_eMode;									//´®¿ÚµÄ¹¤×÷Ä£Ê½
	td_u8		m_ucDataBit;								//Êý¾ÝÎ»
	td_u8		m_ucStopBit;								//Í£Ö¹Î»
	td_u8		m_ucVerifyBit;								//Ð£ÑéÎ»
	td_u8		m_uUartId;									// dxl 20110513 ¼ÓÈëUARTÅäÖÃ£¬ÒÔ¼æÈÝ²»Í¬Ó²¼þÆ½Ì¨
}TCom;
/**************************Com Attributes End**********************************/
/**************************PTZ Attributes Begin**********************************/
//Í¨µÀÏà¹ØµÄPTZÊôÐÔ
typedef struct
{
	//Leeon Tonie Added below for 3511, new PTZ mode.
	td_u8			m_cProtocol;						//¸ÃÍ¨µÀËùÊ¹ÓÃµÄ¿ØÖÆÐ­Òé±àºÅ
	td_u8			m_cProtocolName[32];				//¸ÃÍ¨µÀËùÊ¹ÓÃµÄÐ­ÒéÃû³Æ
    td_u8			m_cComNo;							//¸ÃÍ¨µÀ¶ÔÓ¦µÄÍ¨µÀºÅ
    td_s16			m_sAddress;							//¸ÃÍ¨µÀËùÁ¬½ÓÉè±¸µÄµØÖ·
    td_u8			m_blComAttrEnable;					//ÊÇ·ñ×Ô¶¨Òå´®¿ÚÊôÐÔ
    TCom			m_stComAttr;						//¸ÃÐ­ÒéËù¶ÔÓ¦µÄ´®¿ÚÊôÐÔ£¬Èç9600,'n',8,1
}TPTZChannel;
/**************************PTZ Attributes End*******************************/
/**************************User Attributes Begin*******************************/
#define MAX_IPFILTER_NUM			16

//ÌÕÓÀÁÁ 091028 ÎªIP¹ýÂË¶¨Òå¶ÀÁ¢µÄÊý¾Ý½á¹¹£¬²¢ÒÆÖÁÓÃ»§Ä£¿é
typedef struct
{
	//ÕâÁ½ÏîÊÇÍ¨¹ýÍøÂçÉèÖÃµÄÆÁ±Î·½Ê½
    td_u32	    m_u32ExpectIP;					//±»ÆÁ±ÎµÄIPµØÖ·
    td_u32		m_u32ExpectMask;				//±»ÆÁ±ÎµÄIP¶ÎÑÚÂë
	
	//Leeon Tonie add for Tiandy-Dvr, more ip filter
	//ÒÔÏÂ²ÎÊýÊÇÍ¨¹ý±¾µØÉèÖÃµÄºÚÃûµ¥/°×Ãûµ¥µÄ·½Ê½
	td_u8		m_cFilterType;					//ÀàÐÍ:ºÚÃûµ¥/°×Ãûµ¥/²»ÆôÓÃ
	td_u8		m_cIPNum;						//Ãûµ¥³¤¶È
	td_u32		m_uiIPList[MAX_IPFILTER_NUM];	//Ãûµ¥
}TIpFilter;
//zhangshengusr 20131029 Îª¹ýÂË±»¶Ï¿ªµÄip¶¨ÒåµÄ½á¹¹Ìå
typedef struct
{
	td_u32  m_uiIP;		//±»¶Ï¿ªµÄip
	td_s32  m_iTime;	//±»¶Ï¿ªµÄÊ±¼ä
}TdicUsrMsg;

typedef struct
{
	td_s32  m_iNumNow;  //Ä¿Ç°±»½ûÖ¹µÄip¸öÊý
	TdicUsrMsg  m_IPMember[32];//×î´ó±£´æ32¸öÓÃ»§
	pthread_mutex_t g_mutexIPforbidLogon;//ÓÃÓÚ±à¼­½ûÖ¹µÇÂ½ipÁÐ±íµÄ±£»¤
} TIPFilter_ex;

/**************************User Attributes End**********************************/
//Éè±¸Ö§³ÖµÄ×Ö·û¼¯
typedef enum
{
	GB2312 = 1,
	UTF8,
}TCharsetType;


//add by wanzy  2011/10/19 Ôö¼ÓÍâ¹ÒsipÐ­Òé

//SIPÉè±¸²ÎÊý
typedef struct
{
	char m_SipServerIp[33];	//sip·þÎñÆ÷ip
	int  m_SipServerPort;	//sip·þÎñÆ÷¶Ë¿ÚºÅ
	char m_SipServerID[33];	//sip·þÎñÆ÷ID
	char m_SipDeviceID[33]; //Éè±¸±àºÅ
	char m_SipUserName[33]; //ÓÃ»§Ãû
	char m_SipPassword[33];	//ÃÜÂë
	int  m_RegisterValidity;//×¢²áÓÐÐ§ÆÚ	
	int  m_keepalivetime;	//±£»îÊ±¼ä
	int  m_HeartBeatinterval;	//ÐÄÌø¼ä¸ôÊ±¼ä
	int  m_HeartBeatTimes;	//ÐÄÌø´ÎÊý
	int  m_EnableRegister;
}TSIPDeviceInfo;

//SIPÊÓÆµÍ¨µÀ²ÎÊý
typedef struct
{
	int   	m_iChannelNo;
	char	m_pcChannelId[32];	//Í¨µÀÉè±¸±àºÅ
	int   	m_iLevel;		//µ±Ç°ptz¿ØÖÆ¼¶±ð
	int   	m_iPTZtime;		//ÉÏ´Îptz¿ØÖÆ¼ÆÊ±
}TSIPChnPara;

//SIP±¨¾¯Í¨µÀ²ÎÊý
typedef struct									
{
	int   	m_iChannelNo;
	char	m_pcChannelId[32]; //Í¨µÀÉè±¸±àºÅ
	int   	m_iLevel;			//Í¨µÀ±¨¾¯¼¶±ð
}TSIPAlarmPara;

//add by wanzy 2011/10/19
typedef struct
{
	int	m_iCmdNo;
	int   	m_iChannelNo;
	char	m_pcChannelId[32];
	int   	m_iLevel;
	int   	m_iPTZtime;
}TCstmChnParaID;

//add by nsg 120905
typedef struct
{
	td_s32	m_iInformWay;			//Í¨ÐÅ·½Ê½0:´®¿Ú 1:UDP
	char	m_cProtocolType[33];	//Ð­Òé³§¼ÒÃû³Æ	
	td_u32  m_uiSrcIP;				//ATM  IPµØÖ·
	td_s32	m_iSrcPort;				//ATM¶Ë¿ÚºÅ
	td_u32	m_uiDestIP;				//Éè±¸IPµØÖ·
	td_s32	m_iDestPort;			//Éè±¸¶Ë¿ÚºÅ
}TAtmPara;			//ATM²ÎÊý

#define STR_SERVER_NAME_LEN 63
#define IP_STR_LEN 31

typedef struct 
{
	char	m_cServerName[STR_SERVER_NAME_LEN + 1];       	//·þÎñÆ÷µØÖ·
	char	m_cPath[STR_SERVER_NAME_LEN + 1];				//ÎÄ¼þÂ·¾¶
	char 	m_cUserName[IP_STR_LEN + 1];         			//ÓÃ»§Ãû
	char	m_cPassword[IP_STR_LEN + 1];					//ÃÜÂë
	int		m_iPort;										//¶Ë¿ÚºÅ
	int 	m_iUseage;                   					//ÓÃÍ¾0 Éý¼¶ 1ÉÏ´«
	int 	m_iUploadMethod;              					//ÉÏ´«·½Ê½0:×Ô¶¯1:ÊÖ¶¯
}TFtpPara;	//	FTP²ÎÊý

#define MAX_LEN_EMAILPARM 64
typedef struct TSmtpPara
{
	char m_SmtpServerIp[MAX_LEN_EMAILPARM];
	char m_SmtpServerPort[MAX_LEN_EMAILPARM/8];
	char m_SmtpServerAccount[MAX_LEN_EMAILPARM];
	char m_SmtpServerPassword[MAX_LEN_EMAILPARM];
	char m_AuthType[MAX_LEN_EMAILPARM/2];
	char m_TimeOut[MAX_LEN_EMAILPARM/8];
	
	char m_From[MAX_LEN_EMAILPARM];
	char m_To[MAX_LEN_EMAILPARM];
	char m_EmailSubject[MAX_LEN_EMAILPARM*2];
	char m_EmailAddr[4][MAX_LEN_EMAILPARM];
}TSmtpPara;

//end

#define TR_SUBDEVIDREQ 1000
#define TR_SUBDEVIDRSP -1000
#define TR_MAINDEVIDREQ 1001
#define TR_MAINDEVIDRSP -1001

//add by zhuangzhibing 20120216
typedef enum
{
	NETDATA_COM 	= 1,									//ÍøÂçÊý¾ÝÍ¨¹ýÍ¸Ã÷Í¨µÀ·¢ËÍ
	COMDATA			= 2,									//´®¿ÚÊý¾Ý
	UIDATA			= 3,									//UiÊý¾ÝÍ¨¹ýÖ÷³ÌÐò×ª·¢
	NETDATA_DZ		= 4,									//ÍøÂçÊý¾ÝÍ¨¹ýDZÐ­Òé·¢ËÍ
}TDataType;//ÏòÍâ¹Ò·¢ËÍÊý¾ÝµÄÀàÐÍ
//add end
//ÎÄ¼þÀàÐÍ
typedef enum
{
	FILE_REC = 0,    //Â¼ÏñÎÄ¼þ
	FILE_IMG,        //Í¼Æ¬ÎÄ¼þ
	FILE_BACKUP,	 //±¸·ÝÂ¼Ïñ
	FILE_AUDIO,      //ÒôÆµÎÄ¼þ
	MAX_FILE_TYPE,	 //ÎÄ¼þÀàÐÍµÄ×î´óÖµ
}TFileType;

typedef enum
{
	CONVERT_FILETYPE_SDV=1, 	//Ä¬ÈÏSDV¸ñÊ½,²»×ª»»
	CONVERT_FILETYPE_AVI,  	//×ª»»³ÉAVI¸ñÊ½
	CONVERT_FILETYPE_MP4,   	//×ª»»³ÉMP4¸ñÊ½
	CONVERT_FILETYPE_MAX,  //Ô¤Áô
}CONVERT_TYPE_E;

typedef enum
{
	CBL_SINGLE_FILETYPE_DISABLE=0, 	//²»Ê¹ÄÜµ¥Ò»ÎÄ¼þ¿ÌÂ¼
	CBL_SINGLE_FILETYPE_SDV,
	CBL_SINGLE_FILETYPE_AVI,
	CBL_SINGLE_FILETYPE_MP4,
	CBL_SINGLE_FILETYPE_MAX,  //Ô¤Áô
}CBL_SINGLE_TYPE_E;//±¸·Ý»ò¿ÌÂ¼µ¥Ò»ÎÄ¼þÂ¼Ïñ¸ñÊ½


typedef enum
{
	AUDIO_FILETYPE_NONE =0, 	//Ä¬ÈÏ²»±¸·ÝÒôÆµÎÄ¼þ
	AUDIO_FILETYPE_AAC,  	//±¸·ÝAACÒôÆµÎÄ¼þ
	AUDIO_FILETYPE_MAX,  //Ô¤Áô
}BACK_AUDIO_TYPE_E;


typedef td_s32 TRecordType;
#define MAX_RESERVED_INFO_LEN	64	//Â¼ÏñÎÄ¼þÐÅÏ¢ÖÐµÄÔ¤ÁôÐÅÏ¢³¤¶È

//ÎÄ¼þË÷Òý½á¹¹
typedef struct
{
	td_u64	m_ulFileIndex;					//ÎÄ¼þË÷ÒýºÅ
	td_u8	m_ucChannelNo;					//Í¨µÀºÅ
	TFileType	m_eFileType;				//ÎÄ¼þÀàÐÍ,TFileType
	TRecordType	m_eRecordType;				//Â¼ÏñÀàÐÍ,TRecordType
	td_u64 	m_eDevType;						//ÊÓÆµÔ´Éè±¸ÀàÐÍ 
	td_s32 	m_iRecAttr;						//Â¼ÏñÎÄ¼þµÄÊôÐÔ£¬ÓÃ²»Í¬Â¼ÏñÀàÐÍÈ¨ÖµµÄ³Ë»ýÀ´±íÊ¾Ò»¸öÂ¼ÏñÎÄ¼þËù¾ßÓÐµÄ¶àÖÖÊôÐÔ
	td_u8	m_ucFileName[FILE_NAME_LEN];	//ÎÄ¼þÃû£¬²»º¬Â·¾¶
	td_u8	m_ucFileNameSed[FILE_NAME_LEN];//ÈßÓàÂ¼ÏñÎÄ¼þÃû
	td_u32	m_uiFileSize;					//ÎÄ¼þ´óÐ¡£¬µ¥Î»BYTES
	time_t	m_iStartTime;					//Â¼Ïñ¿ªÊ¼Ê±¼ä
	time_t	m_iStopTime;					//Â¼Ïñ½áÊøÊ±¼ä
	td_u8	m_cReserved[MAX_RESERVED_INFO_LEN];	//Ô¤ÁôÐÅÏ¢£¬ÓÃÓÚÌî³ä×Ô¶¨ÒåÐÅÏ¢	
	td_u32  m_iFileSizeIndex;
	td_u32  m_iSedFileSizeIndex;
	td_u32  m_iChnPickFlag;
}TFileInfo;
typedef struct TQueryResult_S TQueryResult;
struct TQueryResult_S
{
	TFileInfo		m_stFileInfo;
	int				m_iDisk;
	TQueryResult*	m_pstNext;
};
//¸Ã½á¹¹ÌåÓÃÓÚ²éÑ¯Ö´ÐÐÕß
typedef struct
{
	td_u32 		m_uiSocket;								//SOCKET£¬ÓÃÓÚÎ¨Ò»±êÊ¶Ë­Ö´ÐÐÁËÕâ¸ö²éÑ¯;0±íÊ¾UI£¬ÆäËû±íÊ¾ÍøÂç¿Í»§¶Ë
	char**		m_ppcResult;							//²éÑ¯½á¹û¼ÇÂ¼
	TQueryResult*	m_stResult;
	td_s32		m_iColumn;								//²éÑ¯µ½µÄÁÐÊý
	td_s32		m_iNum;									//¼ÇÂ¼²éÑ¯µ½µÄ½á¹ûÊý
	td_u8		m_blNewestData;							//ÓÃÓÚ±êÊ¶¸Ã½ÚµãÖÐ´æ´¢µÄÊÇ²»ÊÇ×îÐÂµÄ²éÑ¯½á¹û
	td_s32		m_iStartQueryTime;						//¼ÇÂ¼²éÑ¯¿ªÊ¼Ê±¼ä£¬ÓÃÓÚ³¬Ê±ÍË³ö.
	//ÒÔÏÂ¼ÇÂ¼²éÑ¯Ìõ¼þ
	td_u8		m_ucChannelNo;							//Í¨µÀºÅ
	td_u8		m_ucFileType;							//ÎÄ¼þÀàÐÍ
	td_u8		m_ucRecType;							//Â¼ÏñÀàÐÍ
	td_u8		m_ucDevType;							//ÊÓÆµÔ´Éè±¸ÀàÐÍ
	time_t		m_iStartTime;							//Ê±¼ä¶ÎÆðÊ¼Ê±¼ä
	time_t		m_iStopTime;							//Ê±¼ä¶Î½áÊøÊ±¼ä
	td_u8		m_cReserved[MAX_RESERVED_INFO_LEN];		//Ô¤ÁôÐÅÏ¢£¬ÓÃÓÚÌî³ä×Ô¶¨ÒåÐÅÏ¢	
}TQueryNode;

#ifdef TD_DEBUG
#else
#define TODO
#endif
//shihc add 20130320  ¶¨ÒåÍ¨ÓÃµÄdvr·þÎñµÄIDÖµ,¸ù¾ÝÐèÇó½øÐÐÀ©Õ¹
typedef enum
{
	COMMON_AUDIOVIDEO	= 0x1000,//ÒôÊÓÆµÄ£¿é¿ªÊ¼
	
	COMMON_FONTOVerlAY  = 0x2000,//×Ö·ûµþ¼ÓÄ£¿é¿ªÊ¼

	COMMON_COMPTZ	    = 0x3000,//´®¿ÚPTZÄ£¿é¿ªÊ¼
	
	COMMON_ABNORMAL     = 0x4000,//ÏµÍ³Òì³£Ä£¿é¿ªÊ¼
	
	COMMON_ARLAM_MD	    = 0x5000,//±¨¾¯Ä£¿é¿ªÊ¼//ÒÆ¶¯±¨¾¯Ê¹ÄÜ
	COMMON_ARLAM_OD,
	
	COMMON_STORAGE      = 0x6000,//´æ´¢Ä£¿é¿ªÊ¼	
	COMMON_COMMONENABLE_ONEKEYREC, //Ò»¼üÂ¼Ïñ 0-Í£Ö¹£¬1-¿ªÆô
	COMMON_COMMONENABLE_HASHENABLE, //¹þÏ£Ð£Ñé 0-²»ÊÇÄÜ£¬1-Ê¹ÄÜ
	COMMON_COMMONENABLE_SPLITAVENABLE, //ÌáÈ¡´¿ÒôÆµÎÄ¼þ0-²»Ê¹ÄÜ£¬1-Ê¹ÄÜ
	COMMON_COMMONENABLE_SPLITAVBURNENABLE, //¿ÌÂ¼´¿ÒôÆµÎÄ¼þ0-²»Ê¹ÄÜ£¬1-Ê¹ÄÜ
	COMMON_COMMONENABLE_SPLITAVBACKUPENABLE, //±¸·Ý¿ÌÂ¼´¿ÒôÆµÎÄ¼þ0-²»Ê¹ÄÜ£¬1-Ê¹ÄÜ
	COMMON_COMMONENABLE_BURNFILEFORMAT = 0x6006, //0-±£Áô£¬1-sdv,2-avi, 3-mp4
	

	COMMON_NETSERVICE_SNMP = 0x7000,//ÍøÂç·þÎñÄ£¿é¿ªÊ¼,//SNMP
	COMMON_NETSERVICE_SMTP,//SMTP
	COMMON_NETSERVICE_RTSP_VERIFY,//rtspÐ£Ñé
	COMMON_NETSERVICE_28181,//28181´¿ÊÓÆµ 

	
	COMMON_USERS        = 0x8000,//ÓÃ»§Ä£¿é¿ªÊ¼
	
	COMMON_PUSET	    = 0x9000,//PUÄ£¿é¿ªÊ¼

 
	COMMON_DISK_DISCGROUP= 0xA000,//´ÅÅÌÄ£¿é¿ªÊ¼,//ÅÌ×é²ßÂÔ
	COMMON_DISK_QUOTA,    //Åä¶î²ßÂÔ
	COMMON_RAID,					//RAIDÄ£¿é¿ªÊ¼




	
	COMMON_SYSTEM	    = 0xB000,//ÏµÍ³¹ÜÀíÄ£¿é¿ªÊ¼
	COMMON_TEMPCTRL,//·çÉÈÎÂ¿Ø·§Öµ¿ØÖÆ
	
	COMMON_COMMUNICATION  = 0xC000,//Í¨ÐÅÄ£¿é¿ªÊ¼
	
	COMMON_NETCONNECT	  = 0xD000,//ÍøÂçÁ¬½ÓÄ£¿é¿ªÊ¼
	
	COMMON_VCA            = 0xE000,//ÖÇÄÜ·ÖÎöÄ£¿é¿ªÊ¼

	COMMON_ATM	          = 0xF000,//ATMÄ£¿é¿ªÊ¼
	
	COMMON_TRAFFIC        = 0x10000,//ÐÐÒµ½»Í¨Ä£¿é¿ªÊ¼
	
	COMMON_DZ	          = 0x11000,//DZÏà¹ØÄ£¿é¿ªÊ¼

	COMMON_COMMONENABLE   = 0x12000,//³µÔØÍ¨ÓÃÊ¹ÄÜÄ£¿é¿ªÊ¼
	COMMON_COMMONENABLE_GPSTIMING,//GPSÐ£Ê±
	COMMON_COMMONENABLE_ALARM,//Éè±¸±¨¾¯µçÑ¹ãÐÖµ
	COMMON_COMMONENABLE_SHUTDOWN,//Éè±¸¹Ø»úµçÑ¹ãÐÖµ

	COMMON_TELNETDEN	  = 0x12014,	//telnet·þÎñÊ¹ÄÜ
	COMMON_SETCLOSEPROPERTY = 0x12015,	//·âÅÌµ¯ÅÌÊ¹ÄÜ
	COMMON_END            = 0x14000,//½áÊø·û
	

}SYSTEM_SERVICE_COMMON_ID;
//shihc add 20130320  ¶¨ÒåÍ¨ÓÃµÄÏµÍ³±¨´íIDÖµ,¸ù¾ÝÐèÇó½øÐÐÀ©Õ¹
typedef enum
{
	LASTERROR_AUDIOVIDEO_CHANGECHNTYPE = 0x1000,//ÒôÊÓÆµÄ£¿é¿ªÊ¼,//ÇÐ»»Í¨µÀÀàÐÍÊ§°Ü
	
	LASTERROR_FONTOVerlAY     = 0x2000,//×Ö·ûµþ¼ÓÄ£¿é¿ªÊ¼

	LASTERROR_COMPTZ	      = 0x3000,//´®¿ÚPTZÄ£¿é¿ªÊ¼
	
	LASTERROR_ABNORMAL        = 0x4000,//ÏµÍ³Òì³£Ä£¿é¿ªÊ¼
	
	LASTERROR_ARLAM	          = 0x5000,//±¨¾¯Ä£¿é¿ªÊ¼
	   	
	LASTERROR_SETLOCKSTATUS_FAILURE = 0x6000,//´æ´¢Ä£¿é¿ªÊ¼,	//ÉèÖÃÎÄ¼þËø×´Ì¬Ê§°Ü
	LASTERROR_COMMONENABLE_ONEKEYREC,//Í¨ÓÃÒµÎñ´æ´¢Ä£Ê½ÉèÖÃÊ§°Ü
	LASTERROR_COMMONENABLE_HASHENABLE,//Í¨ÓÃÒµÎñ¹þÏ£Ê¹ÄÜÉèÖÃÊ§°Ü
	LASTERROR_COMMONENABLE_SPLITAVENABLE, //Í¨ÓÃÒµÎñÉú³É´¿ÒôÆµÎÄ¼þÉèÖÃÊ§°Ü
	
	LASTERROR_COMMONENABLE_SPLITAVBURNENABLE, //¿ÌÂ¼´¿ÒôÆµÎÄ¼þ0-²»Ê¹ÄÜ£¬1-Ê¹ÄÜ
	LASTERROR_COMMONENABLE_SPLITAVBACKUPENABLE, //±¸·Ý¿ÌÂ¼´¿ÒôÆµÎÄ¼þ0-²»Ê¹ÄÜ£¬1-Ê¹ÄÜ

	LASTERROR_COMMONENABLE_BURNFILEFORMAT = 0x6006, //Í¨ÓÃÒµÎñÉèÖÃÊµÊ±¿ÌÂ¼Â¼ÏñÎÄ¼þ¸ñÊ½Ê§°Ü
	
	LASTERROR_NETSERVICE	  = 0x7000,//ÍøÂç·þÎñÄ£¿é¿ªÊ¼
	LASTERROR_SMTP,//smtpÉèÖÃÊ§°Ü
	LASTERROR_RTSP_VERIFY,//rtsp¼ìÑéÉèÖÃÊ§°Ü

	LASTERROR_USERS           = 0x8000,//ÓÃ»§Ä£¿é¿ªÊ¼
	
	LASTERROR_PUSET	          = 0x9000,//PUÄ£¿é¿ªÊ¼
	
	LASTERROR_DISK            = 0xA000,//´ÅÅÌÄ£¿é¿ªÊ¼
	LASTERROE_RAID,						//Í¨ÓÃÒµÎñRAIDÄ£Ê½ÉèÖÃÊ§°Ü

	LASTERROR_SYSTEM	      = 0xB000,//ÏµÍ³¹ÜÀíÄ£¿é¿ªÊ¼
	
	LASTERROR_COMMUNICATION   = 0xC000,//Í¨ÐÅÄ£¿é¿ªÊ¼
	
	LASTERROR_NETCONNECT	  = 0xD000,//ÍøÂçÁ¬½ÓÄ£¿é¿ªÊ¼
	
	LASTERROR_VCA             = 0xE000,//ÖÇÄÜ·ÖÎöÄ£¿é¿ªÊ¼

	LASTERROR_ATM	          = 0xF000,//ATMÄ£¿é¿ªÊ¼
	
	LASTERROR_TRAFFIC         = 0x10000,//ÐÐÒµ½»Í¨Ä£¿é¿ªÊ¼
	
	LASTERROR_DZ	          = 0x11000,//DZÏà¹ØÄ£¿é¿ªÊ¼

	LASTERROR_COMMONENABLE_SNMP = 0x12000,//Í¨ÓÃÒµÎñÊ¹ÄÜÄ£¿é¿ªÊ¼	,//SNMPÊ¹ÄÜÊ§°Ü
	LASTERROR_COMMONENABLE_STORAGEMODE,//Í¨ÓÃÒµÎñ´æ´¢Ä£Ê½ÉèÖÃÊ§°Ü
	LASTERROR_COMMONENABLE_GPSTIMING,//Í¨ÓÃÒµÎñGPSÐ£Ê±Ê¹ÄÜÉèÖÃÊ§°Ü
	LASTERROR_COMMON_COMMONENABLE_ALARMPOWER,//±¨¾¯¹Ø»úµçÑ¹ãÐÖµÉèÖÃÊ§°Ü
	LASTERROR_COMMON_SETCLOSEPROPERTY = 0x12015, //·âÅÌ¡¢µ¯ÅÌÉèÖÃÊ§°Ü

	LASTERROR_END             = 0x14000,//½áÊø·û
	

}LASTERROR_ID;


//int td_printf(unsigned int handle, char* pszfmt, ...);
typedef enum
{
	PTZPRI_ALARM  = 0,      //±¨¾¯Áª¶¯
	PTZPRI_VCACAP  ,       //ÖÇÄÜ¸ú×Ù
	PTZPRI_REMOTE,         //Ô¶³ÌÓÅÏÈ¼¶
	PTZPRI_LOCAL,	        //±¾µØÓÅÏÈ¼¶
	PTZPRI_MAX,	        // ×î´ó
}TPTZ_PRIORITY;

//zy 20130926
typedef struct 
{
    td_u32 u32Width;
    td_u32 u32Height;
}TSize;

typedef enum
{
	REC_TAG_ADD  = 0,      //Ìí¼Ó±êÇ©
	REC_TAG_DEL  ,       //É¾³ý±êÇ©
	REC_TAG_UPDATE,         //±à¼­±êÇ©
	REC_TAG_QUERY,         //±à¼­±êÇ©
}TREC_TAG_OPERATION;

typedef struct
{
   time_t	m_iStartTime;					//Ê±¼ä¶ÎÆðÊ¼Ê±¼ä
   time_t	m_iStopTime;					//Ê±¼ä¶Î½áÊøÊ±¼ä
}vca_TimeSegment;

#define MAX_RTMP_LIST_INFO		16					//RTMPÁÐ±íÐÅÏ¢×î´óÊý
#define MAX_RTSP_LIST_INFO		64					//RTSPÁÐ±íÐÅÏ¢×î´óÊý

#define JMB39X ON

#define PM_TC_2800AN_SD		0xC0120	//3520mc
#define PM_TC_2800AN_SX		0xC0140	//3520sc
#define PM_TC_2816AN_SF_H_T	0xC0171	//3531&3532  16_960H(16) || 2UÉóÑ¶960H
#define PM_TC_2804AN_SAW	0xC0172	//3531&3532  sdi(4/8) || 2UÍ¥Éó||2UÉóÑ¶SDI
#define PM_TC_2816AN_SF_T	0xC0174	//3531&3532  16_D1(16)
#define PM_TC_H804BE_P		0xC0175	//3531µ¥Æ¬sdiÉè±¸±ãÐ¯Ê½ÉóÑ¶
#define PM_TC_H804I_HD_BP	0xC0179	//3531&3532  sdi(4)	3UÉóÑ¶SDI
#define PM_TC_H804N_HD_P	0xC0180	//3531  4_960H(4)	3UÉóÑ¶960H
#define PM_TC_H908NC_HD		0xC017B	//3531&3532  SDI	3UÍ¥Éó
#define PM_TC_H904B_HD		0xC017A	//3531&3532  SDI	2U¼òÔ¼Í¥ÉóÖ÷»úV6.2°æ
#define PM_TC_H904BIX_HD_I  0xC0184	//3531 4Â·»ìºÏ±ãÐ¯Ê½Í¥ÉóÖ÷»ú

#define JumpIf(c, lable)			if (c) goto lable
#define BreakIf(c)				if (c) break
#define ContinueIf(c)				if (c) continue
#define RetIf(c)					if (c) return
#define RetValueIf(c, value)		if (c) return (value)
#define DoIf(c, a)			if (c) {a;}

#define MAX_OSD_TYPE 5
typedef struct
{
	unsigned int m_uiFontSize[MAX_OSD_TYPE];			//Ôö¼Ó×ÖºÅ
}TOSDParamEx;

#endif
