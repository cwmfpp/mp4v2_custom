#ifndef _LIBDVRRECORDER_H_
#define _LIBDVRRECORDER_H_

#include "public_def.h"
#include "td_types.h"
#include "libdiskmnt.h"
#include "dvdpublic.h"
#include "libstorage.h"
#include "libdiskgrp.h"

#define MAX_DVD_SPACE 				(4.2*1024)
#define MAX_BLUE_SPACE          	(23*1024)
#define DVD_MAX_REMAIN_CD_SPACE 	(1.5*1024) //最大预留1.5G
#define DVD_MIN_REMAIN_CD_SPACE 	(0.6*1024) //基于光盘满时容易刻坏将光盘最小预留由0.5G改为0.6G

#define BLUE_MAX_REMAIN_CD_SPACE 	(2.0*1024) //最大预留2.0G
#define BLUE_MIN_REMAIN_CD_SPACE 	(0.5*1024) //最小预留0.5G
#define RES_FILE_SIZE            	2        //M
#define CONVERT_DIR_SPACE           25       //G
#define DISK_REMAIN_SPACE           8        //硬盘0分区预留空间为8G
#define MIN_RECORD_SPACE            (128*1024) //最小录像大小128K,刻录录像如果小于128K,则不添加到刻录队列
#define UDF_INIT_COUNT              5 //光盘udf初始化最大次数
#define UDF_CHECK_COUNT             5 //光盘check最大次数
#define UDF_START_ADDRESS           1040//UDF光盘第二个轨道起始地址

//add by gyn 20121008
typedef enum
{
	DISK_BK_NONE = -1,
	DISK_BK_ING = 0,
	DISK_BK_FIN = 1,
	DISK_BK_PRE = 2,
	DISK_BK_MUTE
}tag_RecordBackupType;//光盘备份的状态
//add by gyn 20121008
typedef enum
{
	NotCloseDisk = 0,//不封盘
	CloseDisk = 1,//封盘
}tag_BurnCloseCmd;//此命令仅仅用于下传给recorder程序，并非网络协议对应关系


typedef enum
{
	BurnIsEject = 0,//弹盘
	BurnNotEject = 1,//不弹盘
}tag_DiskEject;//光盘弹盘状态


typedef enum
{
	ISO_DISK = 0,//ios光盘
	UDF_DISK = 1,//udf光盘
}Type_Disk;//光盘刻录暂停状态

typedef enum
{
	DIRECT_OK = 0,//直刻正常
	DIRECT_ERROR = 1,//直刻损坏
}DIRECT_STATE;//无硬盘直刻状态

typedef enum
{
	BurnIsPause = 0,//暂停
	BurnNotPause = 1,//不暂停
}tag_BurnPauseType;//光盘刻录暂停状态

enum // 刻录状态
{
	CBL_STA_FREE	= 0,	 //空闲中，
	CBL_STA_DOING	= 1, //正在刻录，
	CBL_STA_PACK	= 2, //正在打包
};

enum // 光盘状态
{
	CBL_CD_STA_FREE	= 0, //光盘未使用
	CBL_CD_STA_BUSY	= 1, //光盘正在刻录
	CBL_CD_STA_BAD	= 2, //光盘已损坏
	CBL_CD_STA_FULL	= 3, //光盘已经满
	CBL_CD_STA_PACK	= 4, //光盘正在打包
	CBL_CD_STA_NODISK = 5, //光盘没有放入光盘
	CBL_CD_STA_CHECKDISK = 6, //光盘检测中
	CBL_CD_STA_BURN_FINISH =7,//光盘 刻录 结束
	CBL_CD_STA_FORMATDISK = 8,//光盘格式化中 
};

enum //上传给客户端的刻录状态
{
	CBL_CD_STA_START_SUCCESS = 101,//开始刻录成功
	CBL_CD_STA_START_FAILED  = 102,//开始刻录失败
	CBL_CD_STA_PAUSE         = 103,//暂停刻录
	CBL_CD_STA_RESUME		 = 104,//恢复刻录
	CBL_CD_STA_STOP_SUCCESS  = 105,//停止刻录成功
	CBL_CD_STA_STOP_FAILED   = 106,//停止刻录失败
	CBL_CD_STA_FORCE_SUCCESS = 107,//强制停止成功
	CBL_CD_STA_FORCE_FAILED  = 108,//强制停止失败
	CBL_CD_STA_NOFREE_SPACE  = 109,//预留空间不足
};

typedef enum
{
	SINGLE_BURN=0, 			//单盘刻录
	DOUBLE_BURN,  			//双盘同时刻录
	RECYCLE_BURN,   		//循环刻录
	BUTT_BURN,      		//空，该盘未参加刻录
}BURN_TYPE_E;

typedef struct	//光盘信息
{
	int m_iSta;		//状态
	int m_iUsed;		// 占用标识(0表式未占用，正数表式光盘号，并在使用)
	int m_iUsedSize;	//已刻录大小(单位K)
	int m_iTotalSize;	//光盘总大小(单位K)
} TCdSta;

//用于保存光盘录像文件的结构体
#define MAX_CD_FILE_NUM			128		//光盘回放的队列长度
#define MAX_FILE_LIST 			1024   //刻录列表长度
#define MAX_BACKUP_FILE_LIST    1024    //备份列表长度
#define MAX_UDF_FILE  100    //Udf文件系统的光盘最多支持刻录100个文件.
#define DIRECT_BUFFER_SIZE  (2*1024*1024)  //直刻循环buffer开辟大小为2M
#define DIRECT_MD5_COUNT   1500   //无硬盘时,每次刻录64KB,刻录1500次后把当前md5值记录在nand上,为了断电修复md5值
#define DVD_READ_BLOCK  (32*1024)  //无硬盘时,断电修复md5时,每次读取32KB数据
#define DVD_BLOCK (64*1024)   //DVD光盘直刻时,每次刻录的基本单位为64KB
 
typedef struct
{
	int m_iChn;
	int m_iStartTm;
	int m_iEndTm;
	char m_strFileName[FILE_NAME_LEN+1];
}TCdVideoFile;

typedef struct
{
	int 			m_iCd;		//cd编号，默认0表式全部cd
	int 			m_iChn;		//通道号, 0表示所有
	int 			m_iStartId;	// 按时间排序后记录的开始条数
	int 			m_iMaxCnt;	// 想要获取的记录条数
	int 			m_iStartTm;	// 开始时间(预留，默认为0)
	int 			m_iEndTm;	// 结束时间(预留，默认为0)
	int 			m_iCount;
	TCdVideoFile	m_VideoFile[MAX_CD_FILE_NUM]; 
}TCdQuery;




#define	MAX_VIDEO_MARK_NUM		256
typedef struct 
{
	int m_iChn;
	int m_iStartTm;
	int m_iEndTm;
	
}TCDVideoMark;

typedef struct tdINQUEST_MARK_QUERY
{
	int 			m_iCount;
	TCDVideoMark	m_VideoMark[MAX_VIDEO_MARK_NUM];	
}TMarkQuery;

typedef struct
{
	int		m_hasDisk;					//光驱中是否有光盘
	int		m_diskFreeSpace;				//光驱中光盘的可用空间
	unsigned long long		m_diskFullSpace;	     //光驱中光盘的已用空间
}TRecoderDvdInfo;

//////////////////////
typedef enum
{
	PreBurn= 0, //  未刻录
	//Burnning,	//   刻录中
	BurnFinished, //  刻录完成	
}FileStatus;

typedef enum
{
	PRE_CONVERT= 0, //  转换未完成
	CONVERT_FINISHED, //  转换完成	
}ConvertStatus;

typedef enum
{
	CONVERT_THREAD_FINISHED= 0,//转换线程未开启
	CONVERT_THREAD_START,//  转换线程已开启
}ConvertThreadStatus;

typedef enum
{
	DVD_NOT_REPAIR= 0,//光盘此时没有在修复
	DVD_REPAIRING,//光盘此时在修复  
}DvdRepairStatus;

typedef enum
{
	StartBurn= 0, //  开始刻录
	StopBurn, //    停止刻录	
}BurnStatus;

typedef enum
{
	NOT_CONVERT_FULL= 0, //  转换未满一张光盘
	CONVERT_FULL, //  转换录像已满一张光盘	
}DvdConvertStatus;

typedef enum
{
	VIDEO_CONVERT_NOT_FINISH= 0, // 录像转换未完成
	VIDEO_CONVERT_FINISH, //  录像转换完成 	
}VideoConvertStatus;

typedef enum
{
	MP4_CONVERT_SUCESS= 0, // MP4录像转换正常	
	MP4_CONVERT_FAILURE,//MP4录像转换失败
}MP4ConvertStatus;


typedef enum
{
	DVD_NOT_BURNNING = 0, // 光驱没在刻录
	DVD_BURNNING, // 光驱正在刻录
}DvdBurnStatus;

typedef enum
{
	BIG_FILE_MIDDLE = 0,//大文件中的中间文件
	BIG_FILE_END,//大文件中的最后一个文件
}BigFileStatus;


typedef struct
{
	td_u8   	m_ucFileName[FILE_NAME_LEN];		//文件名
	td_u32		m_uiFileSize;					//文件长度
	TFileType	m_eFileType;					//文件类型
	
}TBurnFileInfo;

typedef struct
{
	TBurnFileInfo   stFileInfo;
	FileStatus 	      Fileinfo;
}Rec_FileStat;

typedef enum
{
	Burnning=0,  //正在刻录
	Reburnning,  //正在重刻
	Eject, 		//光盘已弹出
	Nodisk,		//无盘
	DiskFull, 	//光盘剩余空间不足
	Diskerror,	//光盘烧写失败
	Other,
}Rec_Stats;
////////////////////////

typedef struct t_Burn_File_List
{
	Rec_FileStat    Rec_File[MAX_FILE_LIST];  //1024个刻录文件列表
	int 			m_iChnNum;		//刻录通道号，按位
	int 			m_iRead;		//读标志
	int 			m_iWrite;  		//写标志
	int				m_iStatus;		//刻录状态
	int				m_iType;		//刻录策略
	pthread_mutex_t m_ListMutex;	//锁
}T_BURN_FILE_LIST;//当前的刻录队列

typedef struct t_Temp_Burn_List  //同一时间段的录像文件列表
{
	TFileInfo     Rec_File[MAX_CHANNEL_NUM]; //最多所有通道
	int 		  m_iRead;                   //读标志
	int 		  m_iWrite;                  //写标志
	int           m_iChnCount;               //所选通道个数
	unsigned int  m_iTotalFileSize;          //同一时间段录像文件总大小
	pthread_mutex_t m_ListMutex;	         //锁
}T_TEMP_BURN_LIST;//当前的刻录临时表队列

typedef struct
{
	unsigned int m_uiDevNo; // 光驱设备号
	unsigned int m_uiFileNum; // 这张盘一共要刻几个数据文件
	unsigned int m_uiFolderNum; // 这张盘一共要刻几个目录
	unsigned char m_pLogicVolIdent[128]; // 光盘卷标
	unsigned int m_lengthAlloc; // 变长分配，这个字段记录allocUdfFile和m_allocUdfFolder的分配长度，单位：字节
	udf_file    m_allocUdfFolder[MAX_UDF_FILE];// 实际的struct udf_file 目录，自顶向下组装
	udf_file    m_allocUdfFile[MAX_UDF_FILE];// 实际的struct udf_file  数据文件
}udf_temp_fs;

/* MD5 context. */
typedef struct MD5_CTX
{
	/* state (ABCD) */   
	/*四个32bits数，用于存放最终计算得到的消息摘要。当消息长度〉512bits时，也用于存放每个512bits的中间结果*/ 
	unsigned long int state[4];  

	/* number of bits, modulo 2^64 (lsb first) */    
	/*存储原始信息的bits数长度,不包括填充的bits，最长为 2^64 bits，因为2^64是一个64位数的最大值*/
	unsigned long int count[2];

	/* input buffer */ 
	/*存放输入的信息的缓冲区，512bits*/
	unsigned char buffer[64]; 
} MD5_CTX;

typedef struct 
{
	char   cFileName[64];
    unsigned int iMd5Addreess;
	unsigned int iPreMd5Addreess;
	MD5_CTX   Md5_File_Info;
	MD5_CTX   PRE_Md5_File_Info;
}T_MD5_INFO;


//add by gyn
typedef struct t_Back_Burn_List  //同一时间段的录像文件列表
{
	Rec_FileStat    Rec_File[MAX_BACKUP_FILE_LIST];  	//1024个刻录文件列表
	int				m_iStatus;					//标识是否进行备份
	int 		  	m_iRead;                    //读标志
	int 		  	m_iWrite;                   //写标志
	unsigned int  	m_iTotalFileSize;           //录像文件总大小
	pthread_mutex_t m_ListMutex;	         	//锁
}T_BACK_BURN_LIST;//当前备份队列

typedef struct	// 实时刻录状态信息
{
	int m_iStat;	//刻录状态
	TCdSta m_tSCdSta[MAX_RECORDER_NUM];
	
} TCblSta;//UI会使用该结构体


typedef struct	
{
	int m_iStat;  			//备份光盘状态
	int m_iLastPrg; 		//上一张光盘进度
	int m_iTotalSize; 		//总容量
	int m_iBackUpSize;		//已刻录容量
	int m_iTotalFileNum; 	//总文件数
	int m_iBackUpFile; 		//已刻录文件数
} TCblBackUpStat;			//备份的状态信息


typedef enum
{
    BURN_TYPE_CBL= 1,   //实时刻录
    BURN_TYPE_BACKUP,  //备份刻录
    BURN_TYPE_MAX
}TCblBurnType;

typedef struct
{
	int m_iOkNum;
	int m_iErrNum;
}TSaveDvdInfo;

td_s32 rec_GetRecorderList(TDiskInfo* _pstDisk);
/*
 * 函数名称：rec_GetRecorderList
 * 简要描述: 获取刻录机列表
 * 输    入:  无
 *
 * 输	 出：_pstDisk
 *
 * 修改日志：
 * 			2012-10-08，sunln注释
 */

td_s32 Burn_GetDvdInfo(int _iDvdNo,TRecoderDvdInfo* _ptDvdInfo);
/*
 * 函数名称：Burn_GetDvdInfo
 * 简要描述: 获取光驱信息
 * 输    入: _iDvdNo
 *
 * 输	 出：_ptDvdInfo
 *
 * 修改日志：
 * 			2012-10-08，sunln注释
 */

 
td_s32 RtBurn_SetBackUpState(td_s32 _iDvdNo,td_s32 _iBkState);
/*
 * 函数名称：RtBurn_SetBackUpState
 * 简要描述：设置光盘备份状态
 * 输入     : _iBkState,光盘备份状态
 * 返回值  : >=0 成功   -1 失败				 
 *			
 *			
 * 
 * 			
 *			
 *			
 * 修改日志：
 * 			2014-8-14,zmw,创建
 */

 td_s32 RtBurn_setBackupIndex(td_s32 _iBackupIndex,td_s32 _iBackupCnt);
 /*
 * 函数名称：RtBurn_setBackupIndex
 * 简要描述：设置当前备份第几个时间的录像
 * 输入     : _iBackupIndex,光盘备份时间段索引号
 			  _iBackupCnt,光盘备份时间段个数
 * 返回值  : >=0 成功   -1 失败				 
 *			
 *			
 * 
 * 			
 *			
 *			
 * 修改日志：
 * 			2014-8-14,zmw,创建
 */
td_s32 RtBurn_GetBurnStatus();
 /*
 * 函数名称：RtBurn_GetBurnStatus
 * 简要描述：刻录总状态
 * 输入     : 无
 * 返回值  :   刻录总状态			 
 *			
 *			
 * 
 * 			
 *			
 *			
 * 修改日志：
 * 			2014-12-19,zmw,创建
 */
 td_s32 RtBurn_getBackupIndex(td_s32* _iBackupIndex,td_s32* _iBackupCnt);
  /*
 * 函数名称：RtBurn_getBackupIndex
 * 简要描述：获取当前备份第几个时间的录像
 * 输出     : _iBackupIndex,光盘备份时间段索引号
 			: _iBackupCnt,光盘备份时间段个数
 * 返回值  : >=0 成功   -1 失败				 
 *			
 *			
 * 
 * 			
 *			
 *			
 * 修改日志：
 * 			2014-8-14,zmw,创建
 */
  //判断光盘的已用空间是否小于限定值
td_s32 RtBurn_CheckDiskCanUsed(int _iDvdNo,int _iUsedLimit);
/*
 * 函数名称： Burn_CheckDiskCanUsed
 * 简要描述:
 * 输    入: _iDvdNo
 *			_iUsedLimit
 * 输	 出：光盘的已用空间
 *
 * 修改日志：
 * 			2014-03-08 zmw 注释
 */

td_s32 BigFile_CheckDiskCanUsed(int _iDvdNo);
/*
 * 函数名称： BigFile_CheckDiskCanUsed
 * 简要描述:
 * 输    入: _iDvdNo
 *			
 * 输	 出：光盘是否是空白光盘
 *
 * 修改日志：
 * 			2015-05-07 zmw 注释
 */


int RtBurn_MountCD(int _iCdNo,int _iDiskType);
/*
 * 函数名称：RtBurn_MountCD
 * 简要描述: 挂载光盘
 * 输    入: _iCdNo		光驱号
 			_iDiskType	光盘类型
 *
 * 输	 出：
 *
 * 修改日志：
 * 			2015-02-02，zmw注释
 */
int BigFile_CheckUDFExist(int _iCdNo);
/*
 * 函数名称：BigFile_CheckUDFExist
 * 简要描述: 检测光驱里的光盘是哪种类型
 * 输    入: _iCdNo : 光驱号 
 *
 * 输	 出：光盘是哪种类型
 *
 * 修改日志：
 * 			2015-06-08，xiejing注释
 */

td_s32 RtBurn_GetFileListFromCd(int _iChn,TCdQuery* _CdQuery);
/*
 * 函数名称：RtBurn_GetFileListFromCd
 * 简要描述: 获得光驱上的文件列表
 * 输    入: _iChn : 通道号 _CdQuery:查询结果
 *
 * 输	 出：
 *
 * 修改日志：
 * 			2012-10-08，sunln注释
 */
td_s32 DvdEject(int _iDvdNo);
/*
 * 函数名称:	DvdEject
 * 简要描述:	弹出光驱，给业务模块用，用户无需知道具体命令
 * 输	 入: 	_iDvdNo 光驱编号 从0开始
 * 输	 出:	成功 0 ，失败 1
 * 修改日志:
 *				2013-08-02,郭亚楠，创建。
 */
td_s32 DvdUnEject(td_s32 _iDvdNo,td_s32 _s32Type);
/*
 * 函数名称:	DvdUnEject
 * 简要描述:	推入光驱
 * 输	 入: 	_iDvdNo,光驱号 _s32Type，刻录类型，当为单盘刻录时，只推入指定光驱，否则推入所有光驱
 * 输	 出:	成功 0 ，失败 1
 * 修改日志:
 *				2009-12-03,陶永亮，创建。
 */

#if 0
//add by gyn
//获取硬盘上的打点信息
int 	RtBurn_ReadVideoMarkEx(char * _pcFileName,TMarkQuery *_MarkQuery);
/*
	功能		:获取打点信息
	参数		:_MarkQuery		用于返回查询后的结果
	返回值	:0		成功
				 其他	失败
*/
//add end

int 	RtBurn_ReadVideoMark(TMarkQuery *_MarkQuery);
/*
	功能		:获取打点信息
	参数		:_MarkQuery		用于返回查询后的结果
	返回值	:0		成功
				 其他	失败
*/
#endif
td_s32 Burn_GetCDNum();
/*
 * 函数名称：RtBurn_GetCDNum
 * 简要描述:    获取有效光驱数
 * 输       入: 	
 * 输	  出：	有效光驱数
 				
 * 修改日志				
 * 			2010-09-19,高旭麟创建
 */



//------------备份刻录接口------------------------------------------
td_s32 BkBurn_PrepareToRecord(td_s32 _iCdNo,td_s32 _iType);		
/*
 * 函数名称：rec_PrepareToRecord
 * 简要描述: 备份准备
 * 输    入: _iCdNo
 * 输	 出：无
 * 修改日志：
 * 			2012-10-08，sunln注释
 */
 
td_s32 BkBurn_MakeLinkForFile(td_u8* _pcFileName,td_s32 _iCdNo);
/*
 * 函数名称：BkBurn_MakeLinkForFile
 * 简要描述: 为待刻录的文件创建链接
 * 输    入: _pcFileName,_iCdNo
 * 输	 出：无
 * 修改日志：
 * 			2012-10-08，sunln注释
 */

td_s32 BkBurn_DoRecord(td_s32 _iCdNo);
/*
 * 函数名称：BkBurn_DoRecord
 * 简要描述: 刻录准备好的目录
 * 输    入: _iCdNo
 * 输	 出：无
 * 修改日志：
 * 				2012-10-08，sunln注释
 */

td_s32 BkBurn_DoBigFileRecord(td_s32 _iCdNo);
/*
 * 函数名称：BkBurn_DoBigFileRecord
 * 简要描述: 刻录准备好的目录
 * 输    入: _iCdNo
 * 输	 出：无
 * 修改日志：
 * 				2012-10-08，zmw注释
 */

td_s32 BkBurn_GetBurnProgress(td_s32* _piStat,td_s32 _iCdNo );
/*
 * 函数名称：BkBurn_GetBurnProgress
 * 简要描述: 获取刻录进度
 * 输    入: _iCdNo 
 * 输	 出：_piStat
 * 修改日志：
 * 				2014-03-10，zmw修改
 */

td_s32 BkBurn_ForceStopBackUp(td_s32 _iCdNo );
/*
 * 函数名称：BkBurn_ForceStopBackUp
 * 简要描述: 强制停止备份
 * 输    入: _iCdNo 
 * 输	 出：无
 * 修改日志：
 * 			2014-03-10，zmw修改
 */

typedef td_s32 (*GETBITRATE)(td_s32 _u8Channel);

//------------实时刻录接口gxl------------------------------------------
td_s32 CDROM_init(void);
td_s32 RtBurn_Init(GETBITRATE _cbkGetBitRate);
/*
 * 函数名称:	tBurn_Init
 * 简要描述: 	初始化共享内存，获取实际有效光驱数
 *
 * 输       入	:      _cbkGetBitRate: 获取实时刻录码率的回调函数
 * 输	 出：	有效光驱数
 *			
 * 修改日志：
 * 				2010-09-19,高旭麟创建
 */

//改为回调函数
//void RtBurn_Add2List(int _iChn, const char *_strFileName,int _iFileLen,TFileType _FileType);
/*
 * 函数名称:	tBurn_Init
 * 简要描述: 	初始化共享内存，获取实际有效光驱数
 *
 * 输       入	:      _cbkGetBitRate: 获取实时刻录码率的回调函数
 * 输	 出：	有效光驱数
 *			
 * 修改日志：
 * 				2010-09-19,高旭麟创建
 */

//int RtBurn_ListGetType(int _iDiskNo, BURN_TYPE_E* _piType);
td_s32 BkBurn_SetPolicy(BURN_TYPE_E _s32Type, td_s32 _s32DiskNo);
/*
 * 函数名称：BkBurn_SetPolicy
 * 简要描述: 设置备份策略。
 *
 * 输       入: _s32Type:备份策略
					 	SINGLE_BURN=0,	//单盘备份
						DOUBLE_BURN,  	//双盘同时刻录
						BUTT_BURN,     	//空，该盘未参加刻录
						
			    			    
			    _s32DiskNo: 刻录机编号
			   
 * 输	  出：成功
 *			
 * 修改日志：
 * 			2014-11-27,zmw创建
 */

td_s32 BkBurn_SetBigFilePolicy(BURN_TYPE_E _s32Type, td_s32 _s32DiskNo);
/*
 * 函数名称：BkBurn_SetBigFilePolicy
 * 简要描述: 设置备份策略。
 *
 * 输       入: _s32Type:备份策略
					 	SINGLE_BURN=0,	//单盘备份
						DOUBLE_BURN,  	//双盘同时刻录
						BUTT_BURN,     	//空，该盘未参加刻录
						
			    			    
			    _s32DiskNo: 刻录机编号
			   
 * 输	  出：成功
 *			
 * 修改日志：
 * 			2014-11-27,zmw创建
 */

td_s32 RtBurn_SetPolicy(BURN_TYPE_E _s32Type, td_s32 _s32Chn, td_s32 _s32DiskNo,td_s32 _s32Operation);
/*
 * 函数名称：RtBurn_SetPolicy
 * 简要描述: 设置刻录策略。若都设置单盘刻录不同的光驱可以刻录不同的通道
 *                         否则以后一次设置为准
 *
 * 输       入: _s32Type:刻录策略
					 	SINGLE_BURN=0,	//单盘刻录
						DOUBLE_BURN,  	//双盘同时刻录
						RECYCLE_BURN,  	//循环刻录
						BUTT_BURN,     	//空，该盘未参加刻录
						
			    _s32Chn:刻录通道号，按位设置。
			    
			    _s32DiskNo: 刻录机编号，按位设置。
			    _s32Operation 为0不检测光盘，为1检测光盘
 * 输	  出：有效光驱数
 *			
 * 修改日志：
 * 			2010-09-19,高旭麟创建
 */
td_s32 RtBurn_Isinsmod(td_s32 _s32BurnType);
/*
 * 函数名称：RtBurn_Isinsmod
 * 简要描述:  根据刻录策略以及光驱号，看是否要重新加载光驱驱动
 *
 * 输       入: _s32BurnType:刻录策略
					 	SINGLE_BURN=0,	//单盘刻录
						DOUBLE_BURN,  	//双盘同时刻录
						RECYCLE_BURN,  	//循环刻录
						BUTT_BURN,     	//空，该盘未参加刻录
						
			    
 * 输	  出：成功
 *			
 * 修改日志：
 * 			2014-07-21,zmw创建
 */
td_s32 RtBurn_GetPolicy(BURN_TYPE_E* _ps32Type, td_s32 _s32DiskNo, td_s32* _ps32ChnNum);
/*
 * 函数名称：RtBurn_GetPolicy
 * 简要描述: 获取刻录策略。
 *
 * 输       入: _ps32Type:传出刻录类型
					 	SINGLE_BURN=0, 	//单盘刻录
						DOUBLE_BURN,  	//双盘同时刻录
						RECYCLE_BURN,   	//循环刻录
						BUTT_BURN,      	//空，该盘未参加刻录
									    
			    _s32DiskNo: 刻录机编号
			    _ps32ChnNum:传出刻录通道列表
			    
 * 输	  出：当前刻录状态
 						GUI_CBL_STA_FREE: 磁盘空闲
 						GUI_CBL_STA_DOING:正在刻录
 						GUI_CBL_STA_PACK:打包状态(已制成结束刻录状态，但还未刻录完成)
 						
 *			
 * 修改日志：
 * 			2010-09-19,高旭麟创建
 */


td_s32 RtBurn_Start(td_s32 _s32DiskNo);
/*
 * 函数名称：RtBurn_Start
 * 简要描述: 	开始刻录
 * 输       入: 	_s32DiskNo	光驱编号
 							如果刻录策略是双盘同刻或者轮刻，该参数不起作用
 * 输	  出：	成功ERR_REC_SUCCESSFUL
 				无可用磁盘:ERR_REC_NO_DISK
 				正在刻录:ERR_REC_USING_DISK
 * 修改日志：
 * 			2010-09-19,高旭麟创建
 */
td_s32 RtBurn_StartEx(td_s32 _s32DiskNo,td_s32 _iIsOverLap);
/*
 * 函数名称：RtBurn_StartEx
 * 简要描述: 	开始刻录
 * 输       入: 	_s32DiskNo	光驱编号
 							如果刻录策略是双盘同刻或者轮刻，该参数不起作用
 * 输	  出：	成功ERR_REC_SUCCESSFUL
 				无可用磁盘:ERR_REC_NO_DISK
 				正在刻录:ERR_REC_USING_DISK
 * 修改日志：
 * 			2012-10-08，sunln注释
 */


td_s32 RtBurn_Stop(td_s32 _s32DiskNo);
/*
 * 函数名称：RtBurn_Stop
 * 简要描述:    停止刻录
 * 输       入: 	_s32DiskNo	光驱编号
 							如果刻录策略是双盘同刻或者轮刻，该参数不起作用
 * 输	  出：		
 * 修改日志：
 * 			2010-09-19,高旭麟创建
 */

 
 td_s32 RtBurn_DelayPackTime(td_s32 _s32DiskNo);
 /*
 * 函数名称：RtBurn_DelayPackTime
 * 简要描述:    刻录结束后延长录像打包时长
 * 输       入: 	_s32DiskNo	光驱编号
 				
 * 输	  出：		
 * 修改日志：
 * 			2014-04-10,zmw注释
 */
td_s32 RtBurn_StopEx(td_s32 _s32DiskNo);
/*
 * 函数名称：RtBurn_StopEx
 * 简要描述:    停止刻录
 * 输       入: 	_s32DiskNo	光驱编号
 				如果光盘正常则正常停止，否则强制停止
 * 输	  出：		
 * 修改日志：
 * 			2012-10-16,sunln注释
 */


//自己定义状态
td_s32 RtBurn_GetStatus(td_s32 _s32DiskNo, TCdSta* _pCDStatus);
/*
 * 函数名称：RtBurn_GetStatus
 * 简要描述:    获取刻录状态
 * 输       入: 	_s32DiskNo:	刻录机编号
 				_pCDStatus:传出参数，用于传出刻录状态
 * 输	  出：	ERR_REC_NO_DISK；w无有效光盘
 				ERR_REC_SUCCESSFUL:成功
 * 修改日志				
 * 			2010-09-19,高旭麟创建
 */
 
td_s32 DvdEjectRemote(int _iOption,int _iDvdNo);
/*
 * 函数名称：DvdEjectRemote
 * 简要描述: 远程弹仓
 * 输    入: _iOption,_iDvdNo
 			
 * 输	 出：	
 			0:成功
 * 修改日志				
 * 			
 			2012-09-29，郭亚楠
 */

td_s32 RtBurn_DvdEject(int _iOption,int _iDvdNo,td_bool _iForce);
/*
  * 函数名称:RtBurn_DvdEject
  * 简要描述:弹出/收回光驱
  * 输	  入:
  *			 _iDvdNo, 刻录机编号
  *			 _iOption,（1 收回，0 弹出）
 */

td_s32 BigFile_DvdEject(int _iOption,int _iDvdNo,td_bool _iForce);
 				
td_s32 RtBurn_SetCloseProperty(int _iClose,int _iJet);
/*
 * 函数名称：RtBurn_SetCloseProperty
 * 简要描述: 设置光驱刻录是否
 * 输       入: 	
 //判断是不是封盘与弹出光盘_iClose = 0不封盘_iClose = 1封盘
//_iJet = 0弹出光盘，_iJet = 1不弹出光盘				
 * 输	  出：	
 				0:成功
 * 修改日志				
 * 			
 			2012-03-13，郭亚楠
 */
 td_s32 RtBurn_GetCloseProperty(int *_iClose,int* _iJet);
/*
 * 函数名称：RtBurn_GetCloseProperty
 * 简要描述: 获取光驱刻录是否
 * 输       入: 	
 //判断是不是封盘与弹出光盘_iClose = 0不封盘_iClose = 1封盘
//_iJet = 0弹出光盘，_iJet = 1不弹出光盘				
 * 输	  出：	
 				0:成功
 * 修改日志				
 * 			
 			2012-09-29，郭亚楠
 */
void GetAdditionalFilePath(char *_pcDesFilePath);//获取当前的路径
/*
 * 函数名称：GetAdditionalFilePath
 * 简要描述: 获取播放器的路径
 * 输     入: 	
 				
 * 输	  出：	_pcDesFilePath文件夹路径	
 				
 * 修改日志				
 			2012-03-13，郭亚楠注释
 */

void SetAdditionalFilePathOther(const char *_pcFilePath);
/*
 * 函数名称：SetAdditionalFilePathOther
 * 简要描述: 设置当前系统设置显示证据资料的文件夹
 * 输       入: _pcFilePath文件夹路径	
 				
 * 输	  出：	
 * 修改日志				
 * 			
 			2012-03-13，郭亚楠注释
 */
void GetAdditionalFilePathOther(char *_pcDesFilePath);//获取当前的路径
/*
 * 函数名称：GetAdditionalFilePathOther
 * 简要描述: 获取当前系统设置显示证据资料的文件夹
 * 输     入: 	
 				
 * 输	  出：	_pcDesFilePath文件夹路径	
 * 修改日志				
 * 			
 			2012-03-13，郭亚楠注释
 */


 int RtBurn_OverLapBurn();
 /*
  * 函数名称:RtBurn_OverLapBurn
  * 简要描述:查询录像文件，并加入到刻录队列
  * 输	  入:
  *			 查询录像的开始时间
  * 修改日志				
  * 			
 			2012-01-29，zhuangzhibing
  */
 td_s32 RtBurn_IsOverLapBurn();
 /*
  * 函数名称:RtBurn_IsOverLapBurn
  * 简要描述:判断是否有复刻文件
  * 输	  入:
  *			无
  * 修改日志				
  * 			
 			2012-01-29，zhuangzhibing
  */
  
int RtBurn_SetOverLapBurnInfo(int  _iDiskNo,int _iChn,char *_pcName,char *_pcFlag);
 /*
  * 函数名称:RtBurn_SetOverLapBurnInfo
  * 简要描述:保存已刻录文件名到配置文件中
  * 输	  入:
  *			刻录机编号、通道号、文件名、第一次写配置文件标识
  * 修改日志				
  * 			
 			2012-01-29，zhuangzhibing
  */
int RtBurn_ClearOverLapBurnInfo(int  _iDiskNo);
 /*
  * 函数名称:RtBurn_ClearOverLapBurnInfo
  * 简要描述:清空刻录配置文件
  * 输	  入:
  *			刻录机编号
  * 修改日志				
  * 			
 			2012-01-29，zhuangzhibing
  */


td_s32 RtBurn_Force_Stop(td_s32 _s32DiskNo);
/*
* 函数名称：RtBurn_Force_Stop
* 简要描述: 强制停止刻录
* 输		 入:	 _s32DiskNo光驱编号
		 
* 输	   出：  
		 
* 修改日志			 
	 2012-03-13，郭亚楠注释
*/
td_s32 RtBurn_Reset_Force(td_s32 _s32BurnType,td_s32 _s32DiskNo);
/*
* 函数名称：RtBurn_Reset_Force
* 简要描述: 置强制停止
* 输		 入:	_s32BurnType,刻录策略， _s32DiskNo，光驱号
		 
* 输	   出：  
		 
* 修改日志			 
	 2014-04-01    zmw 注视
*/
td_s32 RtBurn_Uninit();

void SetAdditionalFilePath(const char *_pcFilePath);
/*
* 函数名称：SetAdditionalFilePath
* 简要描述: 设置播放器的路径
* 输       入: 	_pcFilePath文件夹路径
				
* 输	  出：	
				
* 修改日志				
			2012-03-13，郭亚楠注释
*/
 void GetAdditionalFilePath(char *_pcDesFilePath);
/*
* 函数名称：GetAdditionalFilePath
* 简要描述: 获取当前系统播放器的路径
* 输     入: 	
				
* 输	  出：	_pcDesFilePath文件夹路径	
* 修改日志				
* 			
			2012-03-13，郭亚楠注释
*/

int  GetRecorderConfig(TRecordConfig *_pstConfig);//
/*
* 函数名称：GetRecorderConfig
* 简要描述: 获取录像策略，录像时长固定为10分钟
* 输       入: 
				
* 输	  出：	_pstConfig 录像策略结构
				
* 修改日志				
*
*			2012-09-29，sunln注释
*/

int SetOldRecordConfig(TRecordConfig *_pstOldConfig);//
/*
* 函数名称：SetOldRecordConfig
* 简要描述: 设置旧的录像策略
* 输       入: _pstConfig 录像策略结构
				
* 输	  出：	
				
* 修改日志				
*			2012-09-29，sunln注释
*/

int ResumeOldRecordConfig(void);//
/*
* 函数名称：ResumeOldRecordConfig
* 简要描述: 恢复原有的录像策略
* 输       入: 无
				
* 输	  出：	
				
* 修改日志				
*			2012-09-29，sunln注释
*/
//add by sunln 20120401
int RtBurn_ClearList(int _iDiskNo);//
/*
* 函数名称：RtBurn_ClearList
* 简要描述: 清空刻录队列 
* 输       入: _iDiskNo 光驱号
				
* 输	  出：	
				
* 修改日志				
*			2012-09-29，sunln注释
*/

td_s32 OneRtBurn_IsOverLapBurn(int _iDisknum);
/*
* 函数名称:OneRtBurn_IsOverLapBurn
* 简要描述:判断光驱是否有复刻文件
*
* 输	  入:_iDisknum 光驱号
*			
* 修改日志				
*			2012-09-29，sunln注释
*/

int RtBurn_SetOverLapBurnInfoEx(int _iDiskNo,char *_pcName,int _iFirstFlag);
/*
* 函数名称:RtBurn_SetOverLapBurnInfoEx
* 简要描述:保存已刻录文件名到配置文件中
* 输	  入:
*			刻录机编号、文件名、第一次写配置文件标识
* 修改日志				
*			2012-09-29，sunln注释
*/

int RtBurn_ClearOverLapBurnInfoEx(int  _iDiskNo);
/*
* 函数名称:RtBurn_ClearOverLapBurnInfoEx
* 简要描述:清空刻录配置文件
* 输	  入:
*			刻录机编号
* 修改日志				
*			2012-09-29，sunln注释
*/

td_s32 RtBurn_IsOverLapBurnEx();
/*
* 函数名称:RtBurn_IsOverLapBurnEx
* 简要描述:判断是否有复刻文件
* 输	  入:
*			无
* 修改日志				
*			2012-09-29，sunln注释
*/
void  RtBurnDeleteVideo(char *_pcRecorderPath);
/*
* 函数名称:RtBurnDeleteVideo
* 简要描述:删除硬盘中的视频文件目录
* 输	  入:要删除的目录
*			
* 修改日志				
*			2014-05-26，zmw注释
*/
td_s32 OneRtBurn_IsOverLapBurnEx(int _iDisknum);
/*
* 函数名称:OneRtBurn_IsOverLapBurnEx
* 简要描述:判断光驱是否有复刻文件
*
* 输	  入:_iDisknum 光驱号
*			
* 修改日志				
*			2012-09-29，sunln注释
*/


void RtBurn_ResetRecoverCfg();
/*
* 函数名称：RtBurn_ResetRecoverCfg
* 简要描述: 将断电复刻标志复位，用于刻录播放器
* 输       入: 无
				
* 输	  出：	
				
* 修改日志				
*			2012-09-29，sunln注释
*/
td_s32 RtBurn_SetBurnHours(int _iBurnHour,int _iDiskType);
/*
* 函数名称：RtBurn_SetBurnHours
* 简要描述: 设置当前的刻录时长和光盘类型
* 输       入: 
				
* 输	  出：	0代表成功
				
* 修改日志				
*			2012-10-08，sunln注释
*/


td_s32 RtBurn_GetBurnHours(int *_iBurnHour,int *_iDiskType);
/*
* 函数名称：RtBurn_GetBurnHours
* 简要描述: 获取当前刻录时长和光盘类型
* 输       入: 
				
* 输	  出：	0代表成功
				
* 修改日志				
*			2012-10-08，sunln注释
*/		
td_s32 RtBurn_GetBurnChnBitrates(td_s32 _iCdNo);
/*
* 函数名称：RtBurn_GetBurnChnBitrates
* 简要描述: 获取刻录通道的总码率
* 输       入:    光驱号
				
* 输	  出：	刻录通道的总码率
				
* 修改日志				
*			2014-3-6，zmw注释
*/	
td_s32 RtBurn_GetLeftBurnTime(td_s32 _iCdNo);
/*
* 函数名称：RtBurn_GetLeftBurnTime
* 简要描述: 获取光盘的剩余刻录时长
* 输       入:   光盘编号
				
* 输	  出:光盘的剩余刻录时长
				
* 修改日志				
*			2014-2-27，zmw注释
*/		

td_s32 RtBurn_GetLeftPackTime(td_s32 _iCdNo);
/*
* 函数名称：RtBurn_GetLeftPackTime
* 简要描述: 获取光盘的剩余打包时长
* 输       入:   光盘编号
				
* 输	  出:光盘的剩余打包时长
				
* 修改日志				
*			2014-2-27，zmw注释
*/	

td_s32 RtBurn_GetRealSpeed(int _iCdNo,int * _iSpeed);

td_s32 RtBurn_GetBurnSpeed(int *_iSpeed);
/*
* 函数名称：RtBurn_GetBurnSpeed
* 简要描述: 获取当前刻录倍速
* 输       入: 
				
* 输	  出：	0代表成功
				
* 修改日志				
*			2012-10-08，sunln注释
*/
td_s32  RtBurn_GetBurnRealSpeed(int _iCdNo,int * _iSpeed);
/*
* 函数名称：RtBurn_GetBurnRealSpeed
* 简要描述:获取平均刻录速度
* 输       入: _iCdNo 光驱号， * _iSpeed,刻录速度
				
* 输	  出：	0代表成功
				
* 修改日志				
*			2014-03-23，zmw注释
*/
td_s32 RtBurn_SetBurnRealSpeed(td_s32 _iDvdNo,td_s32 _iUseTime,td_s32 _iBurnSize);
/*
* 函数名称：RtBurn_SetBurnRealSpeed
* 简要描述: 设置刻录速度
* 输       入: _iCdNo 光驱号，_iUseTime，刻录所用的时间 _iBurnSize,刻录大小
				
* 输	  出：	0代表成功
				
* 修改日志				
*			2014-03-23，zmw注释
*/
td_s32 RtBurn_SetConvertFileExtName(td_s32 _iFileFormate);
/*
* 函数名称：RtBurn_SetConvertFileExtName
* 简要描述: 设置转换文件的后缀名
* 输       入: _iFileFormate :文件转换类型.
				
* 输	  出：	0代表成功
				
* 修改日志				
*			2014-12-09，zmw注释
*/
td_s32 Convert_GenerateVideoFileExtName(char* _pcTempName);
/*
* 函数名称：Convert_GenerateVideoFileExtName
* 简要描述: 根据文件转换类型修改录像后缀名
* 输       入: _pcTempName :原文件名.
				
* 输	  出：	0代表成功
				
* 修改日志				
*			2014-12-09，zmw注释
*/
//td_s32 ConvertVideoFile(char* _pcSrcFile, char *_pcDesFile, int _iFileFormate);

td_s32 Convert_VideoFile ( char* _pcSrcFileName, char* _pcDstFileName, int _iFileFormate);

/*
* 函数名称：ConvertVideoFile
* 简要描述: 文件转换程序
* 输       入: _pcSrcFile :原文件名.
			   _pcDesFile :目标文件
			   _iFileFormate : 转换格式
				
* 输	  出：	0代表成功
				
* 修改日志				
*			2014-12-09，zmw注释
*/
td_s32 RtBurn_SetBurnSpeed(int _iSpeed);
/*
* 函数名称：RtBurn_SetBurnSpeed
* 简要描述: 设置刻录倍速
* 输       入: 
				
* 输	  出：	0代表成功
				
* 修改日志				
*			2012-10-08，sunln注释
*/
int RtBurn_ReadResSpace(void);
/*
* 函数名称：RtBurn_ReadResSpace
* 简要描述: 读取配置文件中剩余空间值
* 输	   入: 
				
* 输	  出：	0代表成功
				
* 修改日志				
*			2015-04-27，xiejing注释
*/
int RtBurn_GetResSpace(int *_piResSpace);
/*
* 函数名称：RtBurn_GetResSpace
* 简要描述: 获取当前剩余空间
* 输	   入: 
				
* 输	  出：	0代表成功
				
* 修改日志				
*			2015-04-27，xiejing注释
*/
int RtBurn_SetResSpace(int _iResSpace);
/*
* 函数名称：RtBurn_SetResSpace
* 简要描述: 设置当前剩余空间
* 输	   入: 
				
* 输	  出：	0代表成功
				
* 修改日志				
*			2015-04-27，xiejing注释
*/

td_s32 RtBurn_SetBurnCdNameAndSpace(int _iSpace,const char *_pcName);
/*
* 函数名称：RtBurn_SetBurnCdNameAndSpace
* 简要描述: 设置光盘名及预留空间
* 输       入: //_pcName 传入的必须是gb2312格式
				
* 输	  出：	0代表成功
				
* 修改日志				
*			2012-10-08，sunln注释
*/

td_s32 RtBurn_GetBurnCdNameAndSpace(int *_iSpace,char *_pcName);
/*
* 函数名称：RtBurn_GetBurnCdNameAndSpace
* 简要描述: 获取光盘名及预留空间
* 输       入: 
				
* 输	  出：	0代表成功
				
* 修改日志				
*			2012-10-08，sunln注释
*/
td_s32 RtBurn_SetBurnPauseState(int _iState);
/*
* 函数名称：RtBurn_SetBurnPauseState
* 简要描述: 设置光盘满暂停或者恢复
* 输       入: 
				
* 输	  出：	0代表成功
				
* 修改日志				
*			2012-10-08，sunln注释
*/

td_s32 RtBurn_GetBurnPauseState(int *_iState);
/*
* 函数名称：RtBurn_GetBurnPauseState
* 简要描述: 获取光盘满暂停或者恢复
* 输       入: 
				
* 输	  出：	0代表成功
				
* 修改日志				
*			2012-10-08，sunln注释
*/


td_s32 RtBurn_SaveCfg();
/*
* 函数名称：RtBurn_SaveCfg
* 简要描述: 保存高级设置的参数
* 输       入: 
				
* 输	  出：	0代表成功
				
* 修改日志				
*			2012-10-08，sunln注释
*/
td_s32 RtBurn_ClearBurnLable();
/*
* 函数名称：RtBurn_ClearBurnLable
* 简要描述: 清除光盘标签
* 输       入: 
				
* 输	  出：	0代表成功
				
* 修改日志				
*			2012-10-08，sunln注释
*/

td_s32 RtBurn_SetCDTitle(td_s32 _s32DiskNo,const char* const _pcTitle);
/*
* 函数名称：RtBurn_SetCDTitle
* 简要描述:  设置光盘的光标名称
* 输       入: 
				
* 输	  出：	0代表成功
				
* 修改日志				
*			2012-10-08，sunln注释
*/

td_s32 RtBurn_DefaultCfg();
/*
* 函数名称：Burn_DefaultCfg
* 简要描述: 倍速及封盘弹盘参数恢复默认
* 输       入: 
				
* 输	  出：	0代表成功
				
* 修改日志				
*			2012-10-08，sunln注释
*/

td_s32 RtBurn_RmDvdConfigFile(const char *_pcPath,int _iDiskNo);
/*
* 函数名称：RtBurn_RmDvdConfigFile
* 简要描述: 删除config.ini及Player、Other文件夹中的指定内容
* 输       入: 
				
* 输	  出：	0代表成功
				
* 修改日志				
*			2012-10-19，sunln注释
*/
td_s32 RtBurn_SetToDefalutStat(td_bool _iState);
/*
* 函数名称：RtBurn_SetToDefalutStat
* 简要描述:设置当前状态为恢复默认
* 输       入: 
				
* 输	  出：	0代表成功
				
* 修改日志				
*			2012-10-19，sunln注释
*/


td_s32 RtBurn_GetCDStatus(void);
/*
* 函数名称：RtBurn_GetCDStatus
* 简要描述: 获取刻录状态
* 输    入: 无
				
* 输    出：无	
				
* 修改日志				
*		  20121109 王磊add
*/

char * RtBurn_GetTDplayerPath(void);
/*
* 函数名称：RtBurn_GetTDplayerPath
* 简要描述: 获取行业软件路径
* 输    入: 无
				
* 输    出：无	
				
* 修改日志				
*		  20130205 王磊add
*/

td_s32 RtBurn_GetBackUpState(void);
/*
* 函数名称：RtBurn_GetBackUpState
* 简要描述:获取备份状态
* 输       入: 
				
* 输	  出：	DISK_BK_ING表示正在备份
				
* 修改日志				
*			2012-10-19，sunln注释
*/


td_s32 RtBurn_GetBurnIfStop(int _iDiskNo);
/*
* 函数名称：RtBurn_GetBurnIfStop
* 简要描述:获取断电时是否停止刻录了
* 输       入: 
				
* 输	  出：	1代表停止刻录了
				
* 修改日志				
*			2012-10-19，sunln注释
*/

td_s32 RtBurn_SetBurnIsStop(int _iDiskNo,int _iFlag);
/*
* 函数名称：RtBurn_SetBurnIsStop
* 简要描述:设置停止刻录状态
* 输       入: 
				
* 输	  出：	1代表停止刻录了
				
* 修改日志				
*			2012-10-19，sunln注释
*/


td_s32 RtBurn_ClearStopInfo(int  _iDiskNo);
/*
* 函数名称：RtBurn_ClearStopInfo
* 简要描述:清除配置文件
* 输       入: 
				
* 输	  出：	
				
* 修改日志				
*			2012-10-19，sunln注释
*/
td_s32 RtBurn_StopOverLapBurn(int _iDiskNo);
/*
* 函数名称：RtBurn_StopOverLapBurn
* 简要描述:停止断电复刻，用于停止的时候发生断电复刻，在刻录起来后用此函数停止刻录
* 输       入: 
				
* 输	  出：	
				
* 修改日志				
*			2012-10-19，sunln注释
*/
td_s32 RtBurn_ClearBurnCountInfo(int  _iDiskNo);
/*
* 函数名称：RtBurn_ClearBurnCountInfo
* 简要描述:清除记录刻录次数的配置文件
* 输       入: 
				
* 输	  出：	
				
* 修改日志				
*			2012-10-19，sunln注释
*/
td_s32 BkBurn_AddFileToBackUpList(td_u8* _pcFileName,td_s32 _iCdNum);
/*
* 函数名称：BkBurn_AddFileToBackUpList
* 简要描述:将录像文件加入备份队列中
* 输       入: _iCdNum
				
* 输	  出：	
				
* 修改日志				
*			2012-10-19，sunln注释
*/

td_s32 BkBurn_AddFileToConvertList(td_u8* _pcFileName);
/*
* 函数名称：BkBurn_AddFileToConvertList
* 简要描述:将录像文件加入转换队列中
* 输       入: _pcFileName 录像文件名
				
* 输	  出：	
				
* 修改日志				
*			2014-11-27，sunln注释
*/

int RtBurn_GetDebugInfo(char *_cString);
/*
* 函数名称：RtBurn_GetDebugInfo
* 简要描述:获取调试信息
* 输       入: 
				
* 输	  出：	
				
* 修改日志				
*			2012-10-19，sunln注释
*/

int RtBurn_ReleasePlayer(void);
/*
* 函数名称：RtBurn_ReleasePlayer
* 简要描述:释放播放器到Player 目录中
* 输       入: 
				
* 输	  出：	
				
* 修改日志				
*			2012-10-19，sunln注释
*/
td_s32 RtBurn_GetDriverStateForUk(int _s32DiskNo);
td_s32 RtBurn_GetDriverState(int _s32DiskNo);
/*
* 函数名称：RtBurn_GetDriverState
* 简要描述:获取光驱状态
* 输       入: 
				
* 输	  出：	
				
* 修改日志				
*			
*/
int RtBurn_CheckCdError(int _iCdNo);
/*
* 函数名称：RtBurn_CheckCdError
* 简要描述:获取光盘是否是坏盘
* 输       入: 光驱号
				
* 输	  出：	
				
* 修改日志				
*			
*/
int BigFile_CheckCdError(int _iCdNo);
/*
* 函数名称：BigFile_CheckCdError
* 简要描述:直刻完成时，获取光盘是否是坏盘
* 输       入: 光驱号
				
* 输	  出：	
				
* 修改日志				
*			
*/

int RtBurn_UmountCD(int _iCdNo);
/*
* 函数名称：RtBurn_UmountCD
* 简要描述:卸载光盘
* 输       入: 光驱号
				
* 输	  出：	
				
* 修改日志				
*			
*/
int RtBurn_init_CDROMSerialNo(int _iCDROMNum);		
int RtBurn_GetCDSerialNo(int _iCDindex, td_u8* _cSerialNo);
int	RtBurn_UpdateCDSerialNo(int _iCDROMNum,int _iIsOverLap);
int RtBurn_UpdateFileTail_CDSerialNo(td_u8* _cFileName);
int RtBurn_ReadCDROMSerialNoValue4GUI (int _iDiskNo, const td_s8* _pcFileFullPath, td_s8* _pcCDROMSerialNoValue);
td_s32 RtBurn_CleanBurnStatus(void);//将刻录状态复位
td_s32 RtBurn_GetConvertThreadState(void);//获取mp4转换线程状态
int RtBurn_ListGetStatus(int _iDiskNo, int* _piStatus);

td_s32 RtBurn_SetDirectPolicy(BURN_TYPE_E _s32Type, td_s32 _s32Chn, td_s32 _s32DiskNo,td_s32 _s32Operation);
/*
函数名 	    :   RtBurn_SetDirectPolicy
功能	    :	 无硬盘时,设置直刻策略.
输入参数	:	 _s32Type:刻录策略
					 	SINGLE_BURN=0,	//单盘刻录
						DOUBLE_BURN,  	//双盘同时刻录
						RECYCLE_BURN,  	//循环刻录
						BUTT_BURN,     	//空，该盘未参加刻录
						
			    _s32Chn:刻录通道号，按位设置。
			    
			    _s32DiskNo: 刻录机编号，按位设置。
			    _s32Operation 为0不检测光盘，为1检测光盘			            
返回值	: 	 >=0 成功   -1 失败

*/

int BurnComplete_ProcessDvdInfo(int _iDiskNo, int _iRtn);
/*
* 函数名称：BurnComplete_ProcessDvdInfo
* 简要描述:记录在开一次庭的期间，各个光驱产生的好盘与坏盘
* 输	   入: 
					
* 输	  出：	
					
* 修改日志				
*			2015-04-10，xiejing注释
*/

int SetDvdInfo_Config();
/*
* 函数名称：SetDvdInfo_Config
* 简要描述:把光盘的信息写入硬盘
* 输	   入: 
					
* 输	  出：	
					
* 修改日志				
*			2015-04-11，xiejing注释
*/

int GetDvdInfo_Config();
/*
* 函数名称：GetDvdInfo_Config
* 简要描述:从硬盘读出光盘信息
* 输	   入: 
					
* 输	  出：	
					
* 修改日志				
*			2015-04-11，xiejing注释
*/

int DvdNumInfo_WriteConfig();
/*
* 函数名称：DvdNumInfo_WriteConfig
* 简要描述:记录在开庭的期间，各个光驱产生的好盘与坏盘，并将它们写入flash
* 输	   入: 
					
* 输	  出：	
					
* 修改日志				
*			2015-04-11，xiejing注释
*/

int DvdNumInfo_ReadConfig();
/*
* 函数名称：DvdNumInfo_ReadConfig
* 简要描述:从Flash中读出配置文件，并将他们写入对应的全局变量
* 输	   入: 
					
* 输	  出：	
					
* 修改日志				
*			2015-04-11，xiejing注释
*/

td_s32 RtBurn_SetBigFilePolicy(BURN_TYPE_E _s32Type, td_s32 _s32Chn, td_s32 _s32DiskNo,td_s32 _s32Operation);
/*
函数名 	    :   RtBurn_SetDirectPolicy
功能	    :	  有硬盘时,设置大文件刻录策略.
输入参数	:	 _s32Type:刻录策略
					 	SINGLE_BURN=0,	//单盘刻录
						DOUBLE_BURN,  	//双盘同时刻录
						RECYCLE_BURN,  	//循环刻录
						BUTT_BURN,     	//空，该盘未参加刻录
						
			    _s32Chn:刻录通道号，按位设置。
			    
			    _s32DiskNo: 刻录机编号，按位设置。
			    _s32Operation 为0不检测光盘，为1检测光盘			            
返回值	: 	 >=0 成功   -1 失败

*/

td_s32 Rtburn_StartDirectBurn(td_s32 _s32DiskNo);
/*
函数名 	:	Rtburn_StartDirectBurn
功能	    :	ie上开启刻录,无硬盘时开始直刻接口,完成一些刻录变量的初始化.
输入参数	:	 _s32DiskNo : 光驱号  
返回值	: 	〉=0 成功  -1 失败

*/
td_s32 Rtburn_StartBigFileBurn(td_s32 _s32DiskNo);
/*
函数名 	:	Rtburn_StartBigFileBurn
功能	    :	ie上开启刻录,有硬盘时开启大文件刻录接口,完成一些刻录变量的初始化.
输入参数	:	 _s32DiskNo : 光驱号  
返回值	: 	〉=0 成功  -1 失败

*/
td_s32 Rtburn_StartBigFileBurnEx(td_s32 _s32DiskNo,td_s32 _iIsOverLap);
/*
函数名 	    :	Rtburn_StartBigFileBurnEx
功能	    :	ui上开庭,有硬盘时开启大文件刻录接口,完成一些刻录变量的初始化
输入参数	:	 _iCdNo : 光驱号  _iIsOverLap:是否断电补刻
返回值	: 	〉=0 成功  -1 失败

*/

int  Direct_AddUdfFile(int  _CdNo,char* _pcFileName,int _iFileType,int  _iFileLocate);
/*
函数名 	  :	Direct_AddUdfFile
功能	  :	在刻录文件时,调用该接口用于描述该文件的相关信息,如文件名,文件开始地址,文件类型(目录还是文件),文件位置(根目录下还是Other目录下).
输入参数  :	 _iCdNo:光驱号.  _pcFileName:要刻录的文件名. _iFileType:文件类型,目录还是文件. _iFileLocate:文件地址,是根目录还是子目录下的文件.
返回值	  : 	 >=0 成功, -1 失败

*/
int BigFile_CheckBurnList(int _iCdNo);
/*
函数名 	  :	 BigFile_CheckBurnList
功能	  :	    获取刻录情况.
输入参数  :	 _iCdNo:光驱号.
返回值	  :   ERR_RTBURN_FINISHED 刻录完成, ERR_REC_DISK_FULL 光盘满,
			 td_success 队列有文件, td_failure  队列没有文件.

*/
int BigFile_AddBurnFileList(int _iDiskNo);
/*
函数名 	  :	 BigFile_AddBurnFileList
功能	  :	  增加刻录队列读指针.
输入参数  :	 _iDiskNo:光驱号.
返回值	  :   >=0 成功, -1 失败
*/
int BigFile_FillTempBurnList(int _iDiskNo);
/*
函数名    :	BigFile_FillTempBurnList
功能	  :	    从刻录列表读取录像添加到直刻对列..
输入参数  :	 _iDiskNo:光驱号.
返回值	  : 	 >=0 成功, -1 失败

*/
td_s32 BigFile_BurnComplete(td_s32 _s32DiskNo, td_s32 _s32CompleteType);
/*
函数名 	  :   BigFile_BurnComplete
功能	  :	     光盘实时刻录盘满或者光盘刻录完成调用此接口,完成其它文件的刻录以及udf文件系统的刻录然后封盘.
输入参数  :	 _s32DiskNo:光驱号  _s32CompleteType:光盘满或者光盘刻录完成.
返回值	  : 	 >=0 成功, -1 失败

*/
int BigFile_BurnOneTempFileEx(td_s32 _iCdNo);
/*
函数名 	  :	 BigFile_BurnOneTempFileEx
功能	  :	     调用此接口直刻一批录像.
输入参数  :	 _iCdNo:光驱号
返回值	  : 	 >=0 成功, -1 失败

*/
int BigFileBurn_Direct(int _iCdNo);
/*
函数名 	  :	 BigFileBurn_Direct
功能	  :	 从直刻队列templist中依次取录像进行直刻.
输入参数  :	 _iCdNo:光驱号
返回值	  : 	 >=0 成功, -1 失败

*/

int  Direct_BurnFile(int  _iCdNo,char* _pcFileName,int _iEndFile);
/*
函数名 	  :	Direct_BurnFile
功能	  :	直接刻录文件,把一个文件打开后以流的方式进行刻录.
输入参数  :	 _iCdNo:光驱号.  _pcFileName:要刻录的文件名. _iEndFile:是否要结束这个文件,如果要结束,文件尾不足32KB的则补充0.如果不结束,文件尾不足32KB的则暂不刻录.
返回值	  : 	 >=0 成功, -1 失败

*/
int  Direct_EndUdfFile(int  _CdNo,int _iFileType);
/*
函数名 	  :	Direct_EndUdfFile
功能	  :	 当把一个文件刻录完成后,调用此接口来更新当前刻录文件的结束地址.
输入参数  :	 _iCdNo:光驱号. _iFileType:文件类型
返回值	  : 	 >=0 成功, -1 失败

*/
int BigAacFileBurn_Direct(int _iCdNo);
/*
函数名    :  BigAacFileBurn_Direct
功能	  :	 当刻录完视频大文件后,调此接口刻录大文件对应的aac文件
输入参数  :	 _iCdNo:光驱号  
返回值	  :  >=0 成功, -1 失败

*/
int BigFile_GetNotesFielName(int _iCdNo,char *_pcFileName);
/*
函数名 	  :	 BigFile_GetNotesFielName 
功能	  :	 有硬盘审讯刻录大文件时获取Notes.doc文件
输入参数  :	 _iCdNo:光驱号
			 _pcFileName:文件名
返回值	  :  >=0 成功, -1 失败
*/

int  BigFile_BurnOtherFile(int  _CdNo);
/*
函数名 	  :	 BigFile_BurnOtherFile 
功能	  :	 有硬盘审讯刻录大文件时刻录除音视频文件以外的其它所有文件.
输入参数  :	 _iCdNo:光驱号
返回值	  :  >=0 成功, -1 失败
*/
int BigFile_BurnOtherDir(int _s32DiskNo,char * _pcDirName);
/*
函数名 	  :	BigFile_BurnOtherDir
功能	  :	光盘直刻Other文件夹.
输入参数  :	 _iCdNo:光驱号. _pcDirName:Other 文件夹所在路径
返回值	  :  >=0 成功, -1 失败

*/

int BigFile_BurnOtherDirEx(int _s32DiskNo,int _iOtherDirIndex);
/*
函数名 	  :	BigFile_BurnOtherDirEx
功能	  :	光盘直刻Other文件夹里边除了Notes.doc和config.ini的其它文件.
输入参数  :	 _s32DiskNo:光驱号. _iOtherDirIndex:Other 文件夹的索引
返回值	  :  >=0 成功, -1 失败

*/

int BigFile_BurnNotesFile(int _s32DiskNo,char * _pcDirName);
/*
函数名 	  :	BigFile_BurnNotesFile
功能	  :	光盘直刻Other文件夹里边的Notes.doc文件与config.ini文件.
输入参数  :	_s32DiskNo:光驱号. _pcDirName:Other 文件夹所在路径
返回值	  :  >=0 成功, -1 失败

*/

void BigFile_DeleteConvertVideo(BURN_TYPE_E _s32Type,td_s32 _s32DiskNo);
/*
函数名 	  :  BigFile_DeleteConvertVideo
功能	  :	 光盘刻录满盘后,然后删除ConvertVideo目录下的转换文件.
输入参数  :	 _ s32Type:刻录类型  _ s32DiskNo:光驱号
返回值	  :  >=0 成功, -1 失败

*/
int BigFile_Add2TempBurnList(TBurnFileInfo stFileInfo, int _iDiskNo);
/*
函数名 	    :    BigFile_Add2TempBurnList(TBurnFileInfo stFileInfo, int _iDiskNo)
功能	    :	 从刻录队列里边取录像添加到直刻队列中
输入参数	:	 stFileInfo:录像信息   _iDiskNo:光驱号
返回值	    : 	 >=0 成功, -1 失败

*/
td_s32 BkBigFile_BurnComplete(td_s32 _s32DiskNo, td_s32 _s32CompleteType);
/*
函数名 	  :  BkBigFile_BurnComplete
功能	  :	 光盘备份满或者光盘备份完成调用此接口,完成其它文件的刻录以及udf文件系统的刻录然后封盘.
输入参数  :	 _s32DiskNo:光驱号  _s32CompleteType:光盘满或者光盘刻录完成.
返回值	  :   >=0 成功, -1 失败

*/
int  BkBigFile_BurnOtherFile(int  _CdNo);
/*
函数名 	  :	 BkBigFile_BurnOtherFile 
功能	  :	 光盘备份大文件时刻录除音视频文件以外的其它所有文件.
输入参数  :	 _iCdNo:光驱号
返回值	  :  >=0 成功, -1 失败
*/
int  Direct_BurnDirFile(int  _CdNo,char * _pcDirName,int _iDirIndex);
/*
函数名 	  :	 Direct_BurnOtherFile
功能	  :	 在无硬盘情况下刻录除视频文件以外的其它文件.
输入参数  :	 _iCdNo:光驱号
返回值	  :  >=0 成功, -1 失败

*/
int Direct_GetBurnState(int  _CdNo);
/*
函数名    :	Direct_GetBurnState 
功能	  :	在无硬盘模式下,获取直刻状态.
输入参数  :	 _iCdNo:光驱号
返回值	  :  ERR_RTBURN_FINISHED 所有文件转换完成, ERR_REC_DISK_FULL:光盘满
             VIDEO_CONVERT_FINISH:一个文件转换完成

*/


int Direct_CreatMp4Buffer(int _iCdNo);
/*
函数名 	  :	Direct_CreatMp4Buffer
功能	  :	无硬盘直刻时,创建循环buffer用于保存mp4码流.
输入参数  :	_iCdNo:光驱号
返回值	  : >=0 成功, -1 失败
*/
int RtBurn_CreatUdfFs(int _iCdNo);
/*
函数名 	  :	RtBurn_CreatUdfFs
功能	  :	光盘刻录完所有的音视频文件后,调用此接口创建udf文件系统.
输入参数  :	_iCdNo:光驱号
返回值	  : >=0 成功, -1 失败
*/
int RtBurn_InitDirectBurn(int _iCdNo);
/*
函数名 	  :	RtBurn_InitDirectBurn
功能	  :	初始化直刻所用的变量.
输入参数  :	_iCdNo:光驱号
返回值	  : >=0 成功, -1 失败
*/
int rec_CheckDiskIsExist();
/*
函数名 	  :	rec_CheckDiskIsExist
功能	  :	获取有没有真正的硬盘.
输入参数  :	无
返回值	  : >=0 返回可用的磁盘, -1 没有可用的磁盘
*/
int RtBurn_ListGetType(int _iDiskNo, BURN_TYPE_E* _piType);
int Direct_SetMp4FileHeader(unsigned int _uiCDNo, unsigned int _uiMdatSize, int _iConvertType);
/*
函数名 	  :	Direct_SetMp4FileHeader
功能	  :	获取MP4文件头.
输入参数  :	_CdNo:光驱号 _uiMdatSize:mp5包大小,_iConvertType:转换类型 
返回值	  : >=0 成功, -1 失败
*/
int  Direct_SetMp4FileTail(unsigned int _uiCDNo,int _iConvertType);
/*
函数名 	  :	Direct_SetMp4FileTail
功能	  :	获取MP4文件尾.
输入参数  :	_uiCDNo:光驱号 
返回值	  : >=0 成功, -1 失败
*/
int Direct_DestoryMp4Buffer(int _iCdNo);
/*
函数名 	  :	Direct_DestoryMp4Buffer
功能	  :	销毁循环buffer
输入参数  :	_CdNo:光驱号
返回值	  : >=0 成功, -1 失败
*/
int Direct_InitMp4Buffer(int _iCdNo);
/*
函数名 	  :	Direct_InitMp4Buffer
功能	  :	初始化循环buffer
输入参数  :	_CdNo:光驱号
返回值	  : >=0 成功, -1 失败
*/

int Direct_MakeUpDvd();
/*
函数名 	  :	Direct_MakeUpDvd
功能	  :	无硬盘直刻时,断电修复函数
输入参数  :	无
返回值	  : >=0 成功, -1 失败
*/
int RtBurn_UpdateMp4FileTime(char *_cFileName);
/*
函数名 	  :	RtBurn_UpdateMp4FileTime
功能	  :	更新mp4时间
输入参数  :	_cFileName:文件名
返回值	  : >=0 成功, -1 失败
*/
int BigFile_WriteBuffer(unsigned int _uiDevNo, unsigned char* _pcbuf, unsigned int _uiBufferLen);
/*
函数名 	  :	BigFile_WriteBuffer
功能	  :	无硬盘时往循环buffer里边写码流.
输入参数  :	_uiDevNo:光驱号._pcbuf:码流._uiBufferLen:码流长度
返回值	  : >=0 成功, -1 失败
*/
int BigFile_ReadDisk(unsigned int _uiDevNo, unsigned long long  _uiAddress, unsigned char* _pcbuf, unsigned long long _uiLen);
/*
函数名 	  :	BigFile_ReadDisk
功能	  :	从光盘里边裸读数据
输入参数  :	_uiDevNo:光驱号._uiAddress:相对文件头地址._pcbuf:光盘中的裸流._uiLen:要读的数据长度
返回值	  : >=0 成功, -1 失败
*/
int Direct_SetMp4FileHeader(unsigned int _uiCDNo, unsigned int _uiMdatSize, int _iConvertType);
/*
函数名 	  :	Direct_SetMp4FileHeader
功能	  :	获取MP4文件头.
输入参数  :	_CdNo:光驱号 _uiMdatSize:mp4包大小,_iConvertType:转换类型 
返回值	  : >=0 成功, -1 失败
*/

int BigFile_BurnMp4FileTail(int _iCdNo);
/*
函数名 	  :	BigFile_BurnMp4FileTail
功能	  :	有硬盘时刻录mp4文件尾
输入参数  :	_iCdNo:光驱号.
返回值	  : >=0 成功, -1 失败
*/

int BigFile_AddMp4TailToHashList(int _iCdNo);
/*
函数名 	  :	BigFile_AddMp4TailToHashList
功能	  :	有硬盘时刻录时,把mp4文件尾添加到计算hash队列中
输入参数  :	_iCdNo:光驱号.
返回值	  : >=0 成功, -1 失败
*/
int BigFile_StartRepair(int _iCdNo);
/*
函数名 	  :	BigFile_StartRepair
功能	  :	无硬盘直刻,断电修复将刻录状态置成打包状态
输入参数  :	_iCdNo:光驱号.
返回值	  : >=0 成功, -1 失败
*/
int BigFile_FinishRepair(int _iCdNo);
/*
函数名 	  :	BigFile_FinishRepair
功能	  :	无硬盘直刻,断电修复完成后将刻录状态置成空闲
输入参数  :	_iCdNo:光驱号.
返回值	  : >=0 成功, -1 失败
*/
int BigFile_SetMD5Info(int _iCdNo);
/*
函数名 	  :	BigFile_SetMD5Info
功能	  :	设置断电上电修复的md5值
输入参数  :	_iCdNo:光驱号.
返回值	  : >=0 成功, -1 失败
*/
int BigFile_RepairMp4FileMd5(int _iCdNo);
/*
函数名 	  :	BigFile_RepairMp4FileMd5
功能	  :	无硬盘直刻,断电上电修复md5值
输入参数  :	_iCdNo:光驱号.
返回值	  : >=0 成功, -1 失败
*/
int BigFile_CreatMd5File(int _iCdNo);
/*
函数名 	  :	BigFile_CreatMd5File
功能	  :	刻录大文件时,产生md5文件
输入参数  :	_iCdNo:光驱号.
返回值	  : >=0 成功, -1 失败
*/
int BigFile_ReleaseDVDDevice(int _iCdNo);
/*
函数名 	  :	BigFile_ReleaseDVDDevice
功能	  :	释放光驱设备资源接口
输入参数  :	_iCdNo:光驱号.
返回值	  : >=0 成功, -1 失败
*/

int BigFile_SetCdTitle(int _iDiskNo);
/*
函数名 	  :	BigFile_SetCdTitle
功能	  :	设置光盘标签
输入参数  :	_iDiskNo:光驱号
返回值	  : >=0 成功, -1 失败
*/
int GB2312ToUNICODE16(const unsigned char *_Gb2312, unsigned char *_Unicode16, int _iBufSize);
/*
函数名 	  :	GB2312ToUNICODE16
功能	  :	GB2312转换成UNICODE16
输入参数  :	_Gb2312:GB2312字符._Unicode16:unicode字符,_iBufSize:unicode字符长度
返回值	  : >=0 成功, -1 失败
*/
int Direct_SetMd5Name(int _iCdNo,char *_pcMd5FileName);
/*
函数名 	  :	Direct_SetMd5Name
功能	  :	设置md5文件的名字
输入参数  :	_iCdNo:光驱号._pcMd5FileName:md5文件名
返回值	  : >=0 成功, -1 失败
*/
int Direct_GetMd5Name(int _iCdNo,char *_pcMd5FileName);
/*
函数名 	  :	Direct_GetMd5Name
功能	  :	获取md5文件的名字
输入参数  :	_iCdNo:光驱号._pcMd5FileName:md5文件名
返回值	  : >=0 成功, -1 失败
*/
int BigFile_BurnMd5File(int _iCdNo);
/*
函数名 	  :	BigFile_BurnMd5File
功能	  :	刻录md5文件
输入参数  :	_iCdNo:光驱号.
返回值	  : >=0 成功, -1 失败
*/

int Direct_BurnMp4Tail(int _iCdNo);
/*
函数名 	  :	Direct_BurnMp4Tail
功能	  :	断电修复时刻录文件尾
输入参数  :	_iCdNo:光驱号.
返回值	  : >=0 成功, -1 失败
*/
void* Direct_RepairVideo(void* _iCdNo);
/*
函数名 	  :	Direct_RepairVideo
功能	  :	断电修复线程
输入参数  :	_iCdNo:光驱号.
返回值	  : >=0 成功, -1 失败
*/
int BigFile_SetStartConvertFile(int _iCdNo,int _iStartConvertFile);
/*
函数名 	  :	BigFile_SetStartConvertFile
功能	  :	设置sdv文件开始转换mp4文件
输入参数  :	_iCdNo:光驱号 _iStartConvertFile:是否开始转换
返回值	  : >=0 成功, -1 失败
*/

int BigFile_GetStartConvertFile(int _iCdNo);
/*
函数名 	  :	BigFile_GetStartConvertFile
功能	  :	获取sdv文件是否开始转换mp4文件
输入参数  :	_iCdNo:光驱号
返回值	  : >=0 成功, -1 失败
*/
int BigFile_CheckStartConvertFile(int _iCdNo);
/*
函数名 	  :	BigFile_CheckStartConvertFile
功能	  :	检测是否已经开始转换
输入参数  :	_iCdNo:光驱号
返回值	  : >=0 成功, -1 失败
*/
int BigFile_GetBurnChn(int* _piChnList);
/*
函数名 	  :	BigFile_GetBurnChn
功能	  :	获取刻录单一大文件的刻录通道
输入参数  :	_piChnList:获取刻录单一大文件的刻录通道
返回值	  : 
*/
int Direct_RecoveryConfigIni();
/*
*函数: Direct_RecoveryConfigIni
*功能: 无硬盘直刻时,把在nand上的config.ini文件拷贝到硬盘上,用于断电修复
*输入: 无
*		   
*输出: >= 0 成功,-1 失败

*/
int DirectBurn_GetState(int _iCdNo);
/*
*函数: DirectBurn_GetState
*功能: 无硬盘直刻时,获取刻录状态.
*输入: _iCdNo :光驱号
*		   
*输出: 光驱直刻状态

*/
int DirectBurn_SetState(int _iCdNo,int _iDirectBurnState);
/*
*函数: DirectBurn_SetState
*功能:  无硬盘直刻时,设置刻录状态
*输入: _iCdNo:光驱号. _iDirectBurnState:直刻状态
*		   
*输出: >= 0 成功,-1 失败

*/

#endif /*_RECORDER_H_*/

