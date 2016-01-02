/*******************************************************************
文件名	:Storage.h
公司	:天津天地伟业数码科技有限公司
创建者	:陶永亮
创建日期:2008-08-28
简要描述:与存储相关的变量、宏的定义，存储模块接口函数原型
修改记录:2008-08-28,陶永亮，创建
        
******************************************************************/
#ifndef STORAGE_H_
#define STORAGE_H_

#include <stdio.h>
#include <sys/time.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>

#include "public_def.h"

//#define MAX_DVD_NUM		2		//支持最大的DVD刻录机个数
#define PAGE_SHOW_NUM		20		//录像查询结果中一页最多显示的条目数
#if 0
#define DATA_BUFFER_LEN		512 * BYTES_1K 	//一帧的最大长度

#define VOD_BUFFER_LEN		512 * BYTES_1K 	//VOD缓冲区大小
#else
#define DATA_BUFFER_LEN MAX_FRAME_SIZE

#define VOD_BUFFER_LEN MAX_FRAME_SIZE
#endif


//#define MAX_RESERVED_INFO_LEN	64	//录像文件信息中的预留信息长度


//陶永亮 091105 定义自定义类型的范围
#define CUSTOM_RECORD_NAME_LEN	16	//自定义录像类型名的长度
#define MIN_CUSTOM_RECORD_TYPE	32	//自定义录像类型的最小值
#define MAX_CUSTOM_RECORD_TYPE	64	//自定义录像类型的最大值
#define ATM_RECORD_TYPE         34  //自定义atm录像类型
#define CUSTOM_RECORD_NUM		(MAX_CUSTOM_RECORD_TYPE - MIN_CUSTOM_RECORD_TYPE)
									//自定义类型数目
#define MAX_RECORD_TYPE	MAX_CUSTOM_RECORD_TYPE //录像类型的最大值
									

#define IS_FS_ERROR			(EIO == errno || EROFS == errno)	//是否为磁盘读写错误

#define REC_IFRAME_INDEX_MAGIC_NUMBER   0x0123abcd//wangsong,check the file contains iFramIndex or not
#define REC_FALSE_FRAME_FLAG			     0x7788
#define REC_APPEND_CHAR_FLAG			0x1199aabb

#define RECORDER_BLOCK_FILE		"/dev/sr0"
#define RECORD_TEMP_DIRECTORY(iDiskNo)	  rec_TempRecorderPath(iDiskNo) 	//陶永亮 100802 将常量字符串修改为函数
#define RECORD_TEMP_MD5DIRECTORY(iDiskNo) rec_TempRecorderMd5Path(iDiskNo)
//add by gyn
#define RECORD_CONVERTVIDEO_DIRECTORY   rec_ConvertRecorderPath()
	

#define RECORD_DVDINFO_DIRECTORY    rec_DvdInfoFile()
#define RECORD_MID_DIRECTORY(iDiskNo)	rec_MidRecorderPath(iDiskNo) //add by gyn	
#define RECORD_FIN_DIRECTORY(iDiskNo)	rec_FinRecorderPath(iDiskNo) //add by gyn	
#define RECORD_AUD_DIRECTORY(iDiskNo)	rec_AudRecorderPath(iDiskNo) //add by gyn	

#define RECORD_TRUEFIN_DIRECTORY(iDiskNo) rec_TrueFinRecorderPath(iDiskNo) 
//add end
//郭亚楠增加生成配置文件的文件影像，这样可以减少一个文件的刻录

#define RECORD_OVERLAP_DIRECTORY rec_OverLapRecorderPath() //zhuangzhibing 断电复刻

#define MAX_RECORDER_DISK_VOLUME	4200 * BYTES_1K
#define FRAME_INDEX 				//按索引帧定位

#define MAX_HOLIDAY_NUM 			10	//dce add 20130320,假日最大数
#define	HOLIDAY_NAME_LENTH			32	//假日名称最大值

#ifdef TD_DEBUG
#define MAX_IMAGE_NUM_PER_DAY	50000
#else
#define MAX_IMAGE_NUM_PER_DAY	3000	//每天最多可抓拍的文件数目
#endif
#define MAX_IMAGE_NUM_PER_DEL	500	    //每次覆盖删除的最大抓拍文件数目
#define MAX_IMAGE_NUM			100000	//图片总数，如果达到这个总数将开始覆盖
#define OVERWRITE_IMAGE_INTERVAL	86400	//图片覆盖间隔
#define OVERWRITE_LIMIT		    100     //覆盖删除每次查询文件数


/*
	在发生录像覆盖或者重建索引时调用该函数通知ATM模块
	_iType 	0: 删除文件  			文件名保存在_ucFileName[0]中
	        1: 重建索引更新文件名	老文件名保存在_ucFileName[0]中，新文件名保存在_ucFileName[1]中
	        2: 重建索引文件名未更改 文件名保存在_ucFileName[0]中
	        3: 重建索引结束
*/

typedef td_s32 (*REC_NOTIFY_TO_ATM)(char* _ucFileName[2],int _iType);
typedef int (*MainTimeChnPara)(int _iChn);
typedef td_s32 (*SetSmartRecCallBack)(td_s32 _iChn, td_s32 _iType);

#if 0//cwm_todo
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
	time_t 		m_iTimeNow;
	td_u32		m_iPos;
	td_u32 		m_iSign;//0代表未标记，1代表标记,2-光盘序列号//add by gyn
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
#endif//cwm_todo
//根据声音打点结构体
typedef struct
{
	unsigned char m_ucVideoChn; //视频通道号
	unsigned char m_ucHasAudioStat; //是否有音频 (0--无音频1-有音频)
	unsigned short m_usFrameNo; //帧号的低16位
}TAudioTagInfo;

//事务操作
typedef enum
{
	BEGIN = 0,		//开始事务
	COMMIT,			//提交事务
	ROLLBACK,		//回滚事务
	END,			//结束事务
}TTransaction;

//录像类型，除REC_NONE外，数字越小，优先级越高
/*陶永亮 091203 为支持自定义录像类型，将原来的TRecordType定义为TBasicRecordType，
 *将新的TRecordType定义为td_s32
 */
//typedef td_s32 TRecordType;

typedef enum
{
	REC_NONE = 0,    //无录像
	REC_MANUAL,      //手动录像
	REC_TIMING,      //定时录像
	REC_PORT,        //端口报警录像
	REC_MD,          //移动侦测报警录像,Motion Detect
	REC_VL,			 //Video Lost，视频丢失报警录像，一般是被其他通道联动
	REC_AL,			 //Audio Lost; 120229 added by lc
	REC_OD,			 //20130305 SUN Wei add for OD alarm
	REC_BASIC_TYPE,	 //录像类型数，如果要其他类型，请在该值以上添加，主要用于建立数组
	REC_MP = 31,//此类型仅用于排队，实际类型为MD
	REC_VCA = 32,
	REC_ATM = 34,	 //卡号录像	
	REC_SNAPEX = 40//外挂抓拍类型
}TBasicRecordType;

//判断某种录像类型是否报警录像
#define REC_ALARM(type)		(type == REC_PORT || type == REC_MD || type == REC_VL || type == REC_AL || type == REC_OD)
#define REC_CSTM(type)		(type >= MIN_CUSTOM_RECORD_TYPE && type <= MAX_CUSTOM_RECORD_TYPE && type != ATM_RECORD_TYPE)//wangsong, atm, 20120917


#if 0
//视频源设备类型
typedef enum
{
	DEV_CAMERA,		//本地录像，摄像机
	DEV_NVS,		//网络设备，网络视频服务器
	DEV_IPC,		//网络设备，网络摄像机
}TDeviceType;
#endif
#if 0
//文件索引结构
typedef struct
{
	td_u64	m_ulFileIndex;					//文件索引号
	td_u8	m_ucChannelNo;					//通道号
	TFileType	m_eFileType;				//文件类型,TFileType
	TRecordType	m_eRecordType;				//录像类型,TRecordType
	td_u64 	m_eDevType;						//视频源设备类型 
	td_s32 	m_iRecAttr;						//录像文件的属性，用不同录像类型权值的乘积来表示一个录像文件所具有的多种属性
	td_u8	m_ucFileName[FILE_NAME_LEN];	//文件名，不含路径
	td_u8	m_ucFileNameSed[FILE_NAME_LEN];//冗余录像文件名
	td_u32	m_uiFileSize;					//文件大小，单位BYTES
	time_t	m_iStartTime;					//录像开始时间
	time_t	m_iStopTime;					//录像结束时间
	td_u8	m_cReserved[MAX_RESERVED_INFO_LEN];	//预留信息，用于填充自定义信息	
}TFileInfo;
#endif
//查询条件结构
typedef struct
{
	td_u64		m_ulFileIndex;					//文件索引号
	td_u8		m_ucChannelNo;					//通道号
	td_u8		m_ucFileType;					//文件类型
	td_u8		m_ucRecType;					//录像类型
	td_u64		m_ucDevType;					//视频源设备类型
	time_t		m_iStartTime;					//时间段起始时间
	time_t		m_iStopTime;					//时间段结束时间
	td_u8		m_cReserved[MAX_RESERVED_INFO_LEN];	//预留信息，用于填充自定义信息	
	td_u32		m_uiPageNo;						//查询结果页号,从0开始
	td_u32		m_uiPageSize;					//每页条目数
	td_s32		m_uiTotalNum;					//总条目数
	td_u32		m_uiShowNum;					//已显示的条目数
	TFileInfo	m_stFileInfo[PAGE_SHOW_NUM];	//文件信息
}TQuery;

//周期录像时间段
typedef struct
{
	td_u32		m_blSegmentEnable;					//该时间段的使能
	td_u32		m_uiStartHour;						//起始时
	td_u32		m_uiStartMin;						//起始分
	td_u32		m_uiStopHour;						//结束时
	td_u32		m_uiStopMin;						//结束分
	td_u8		m_blTimingEnable;				//定时录像使能
	td_u8		m_blPortEnable;					//端口报警录像使能
	td_u8		m_blMDEnable;					//移动侦测报警录像使能
	td_u8		m_blVLEnable : 1;					//视频丢失报警录像使能
	td_u8		m_blODEnable : 1;					//视频遮挡报警录像使能
	td_u8       m_blMPEnable : 1;               //动测且报警录像模板使能
	td_u16      m_TemplateType;                     //SUN Wei add record Template end
}TSchedule;


//录像参数
typedef struct
{
	td_u8				m_ucChannelNo;				//通道号
	TRecordType			m_eRecordType;				//录像类型
	td_u8				m_blScheduleEnable;			//是否启动录像模板
	td_u8				m_blTimingEnable;			//定时录像使能
	td_u8				m_blPortEnable;				//端口报警录像使能
	td_u8				m_blMDEnable;				//移动侦测报警录像使能
	td_u8				m_blVLEnable : 1;				//视频丢失报警录像使能
	td_u8               m_blODEnable : 1;               //视频遮挡报警录像使能
	td_u8		        m_blMPEnable : 1;               //动测且报警录像模板使能
	td_u8				m_blPreEnable;				//预录使能
	td_u8				m_ucPreTime;				//预录时长
	td_u8				m_blDelayEnable;			//延录使能
	td_u32				m_uiDelayTime;				//延录时长
	TSchedule			m_stSchedule[7][MAX_TIME_SEGMENT];	//周期录像模板
}TRecordParam;

//20130327 王磊add
typedef struct 
{
	td_s32 m_iStoreTime;// 录像保存时长，0~60，单位：天
	td_s32 m_iPickFrame;//抽帧录像
	td_s32 m_iAudioStat;//记录音频
	td_s32 m_iRedundRec;//冗余录像
	td_s32 m_iSubRec;//副码流录像
	td_s32 m_iSubStoreTime;//副码流保存时长
} TChnRecPara;


#if 0
//缓冲区节点
typedef struct
{
	td_s16				m_sFrameType;				//帧类型
	td_s16				m_sFramePos;				//帧在缓冲区中所处的位置
	td_s32				m_iFrameSize;				//帧大小
}TBufferNode;
#endif

//陶永亮 100120 新的抓拍方式，不再需要这个结构体
//#if 0
//抓拍结构体
typedef struct td_Snap_S								
{
	int  		iChn;				//视频通道号
	int  		iPicChn;				//抓拍通道号
	int  		iEnable; 			//抓拍使能标志
	//FILE* 	pFile;				//抓拍文件句柄 20100919 // dxl删除文件句柄(解决句柄泄漏)
	//TFileInfo	stInfo;				//图片文件索引
	td_u8	ucFileName[128];		//文件名 全路径
	pthread_mutex_t	stMutex;		//抓拍互斥锁，主要用于保护文件名	
}TD_SNAP_S;
//#endif

//dce add 20130320,假日设置参数
typedef struct	
{
	int m_iId;   //假日编号
	char m_strHolidayName[HOLIDAY_NAME_LENTH + 1];  //假日名称
	int m_iType;   //按照哪种类型，分为星期(1)、月份(2)、日期(3)
	int m_iStatus;  //0表不开启，1表示开启
	int m_iStartFirst;    //通用参数
	int m_iStartSecond;   //通用参数
	int m_iStartLast;     //通用参数,按月份时预留
	int m_iEndFirst;
	int m_iEndSecond;
	int m_iEndLast;
}THolidayInfo;

//////////////录像标签相关//////////////
typedef struct
{
	td_u32 	m_iTagTime;
	td_u8 	m_cTagName[64];
}TTagInfo;
////////////////////////////////////////

td_s32 rec_StartAlarmRecord(td_s32 _iCh, TRecordType _eRecType, td_u64 _ulTrigger);

#define rec_StartRecord(_iCh, _eRecType) rec_StartAlarmRecord(_iCh, _eRecType, -1)

td_s32 rec_UpdateAlarmRecord(td_s32 _iCh, TRecordType _eRecType, td_u64 _ulTrigger);

#if 0
td_s32 rec_StartRecord(td_s32 _iCh, TRecordType _eRecType);
/*
 * 函数名称：StartRecord
 * 简要描述: 开始录像
 * 输	  入：
 * 			_iCh,通道号,0~15
 * 			_eRecType,录像类型,TRecordType
 * 输	  出：
 * 		ERR_REC_WRONG_PARAM,错误的参数
 * 		ERR_REC_SUCCESSFUL,开始录像成功
 * 		ERR_REC_OTHER_RECORDING,该通道正在进行其他类型录像,或该类型录像
 * 		ERR_REC_DISK_FULL,磁盘满
 * 		ERR_REC_NO_DISK,无可用磁盘
 * 		ERR_REC_DISABLED_TYPE,移动侦测报警录像未使能
 * 修改日志：
 * 			2008-08-28,陶永亮,创建
 */
#endif
td_s32 rec_StopRecord(td_s32 _iCh,TRecordType _eRecType);
/*
 * 函数名称：StopRecord
 * 简要描述：停止录像，如果停止某种类型的录像后有比其优先级低的其他类型排队，则立即开始该类型的录像。
 * 输	  入：
 * 			_iCh,通道号，0~15
 * 			_eRecType,录像类型,TRecordType
 * 输	 出：
 * 			ERR_REC_WRONG_PARAM,错误的参数
 * 			ERR_REC_SUCCESSFUL,停止录像成功
 * 			ERR_REC_NONE_RECORDING,当前未在录像
 * 			ERR_REC_WRONG_TYPE,停止录像类型错误
 * 修改日志：
 * 			2008-08-28,陶永亮,创建
 */

td_s32 rec_ForcePackage(td_s32 _iCh);
/*
 * 函数名称：ForcePackage
 * 简要描述：将某个通道强制打包
 * 输	  入：
 * 			_iCh,通道号，0~15
 * 输	 出：
 * 			ERR_REC_WRONG_PARAM,错误的参数
 * 			ERR_REC_SUCCESSFUL,停止录像成功
 * 			ERR_REC_NONE_RECORDING,当前未在录像
 * 修改日志：
 * 			2009-12-01,陶永亮,创建
 */

 
td_s32 rec_SnapShot(td_s32 _iCh, td_s32 _iSnapType, char *_strFileName, td_u64 _ulTrigger);
/*
 * 函数名称：SnapShot
 * 简要描述：抓拍图片
 * 输	  入：
 * 			_iCh,通道号，0~MAX_CHANNEL_NUM
 * 			_iSnapType,抓拍类型,TRecordType，16位以上，每位表示一种报警类型，以此表示多类型
 			_strFileName:抓拍图片名称dxl 20110805
 			_ulTrigger,联动报警的触发源，按位表示，值为-1时无效
 * 输	 出：
 * 			ERR_REC_WRONG_PARAM,错误的参数
 * 			ERR_REC_SUCCESSFUL,抓拍成功
 * 修改日志：
 * 			2008-10-28,陶永亮,创建
 *                  2012-09-19,wangsong
 */
 
td_s32 rec_RecordData(td_s32 _iCh, td_u8* _pcData, td_s32 _iDataLen, td_s32 _iFrameType, td_s32 _iFlagAudio);
/*
 * 函数名称:RecordData
 * 简要描述:从音视频流中获取数据
 * 输	 入:
 *			_iCh,通道号，0~MAX_CHANNEL_NUM
 *			_pcData,音视频流数据缓冲，非空
 *			_iDataLen,缓冲区中的数据长度,0~128*1024
 *			_iFrameType,帧类型,0,1,2
 			_iFlagAudio,0:无音频数据;1:有音频数据。
 * 输	 出:
 *			ERR_REC_SUCCESSFUL,获取数据成功
 * 			ERR_REC_WRONG_PARAM,错误的参数
 * 修改日志:
 *			2008-09-19,陶永亮，创建
 */
td_s32 rec_StopAllRecord();
/*
 * 函数名称：StopAllRecord
 * 简要描述：停止所有录像
 * 输	 入：无
 * 输	 出：
 * 			ERR_REC_SUCCESSFUL,停止录像成功
 * 			ERR_REC_NONE_RECORDING,当前未在录像
 * 修改日志：
 * 			2008-08-28,陶永亮,创建
 */
td_s32 rec_QueryFiles(td_u32 _uiSocket, TQuery* _stQ);
/*
 * 函数名称：QueryFiles
 * 简要描述：查询录像或图片文件
 * 输	 入：
 *			_iSocket,SOCKET,用于表示是谁执行了这个查询;0表示UI，>0表示网络客户端，其他非法
 * 			_stQ,查询体，用于输入查询信息和输出查询结果
 * 输        出：
 *			ERR_REC_WRONG_PARAM,错误的参数
 *			ERR_REC_SUCCESSFUL,查询录像文件成功
 * 修改日志：
 * 			2008-08-28,陶永亮,创建
 *			2008-10-23,陶永亮，添加参数_iSocket
 */

td_s32 rec_QueryAllFiles(TQuery *_stQ, TFileInfo* _stInfo);
/*
 * 函数名称：QueryAllFiles
 * 简要描述：查询所有文件
 * 输	 入：
 * 			_stQ,查询体，用于输入查询条件.注意其m_uiTotalNum成员要限制文件列表的大小。
 *			_stInfo, 文件列表,用于输出查询结果
 * 输        出：
 *			ERR_REC_SUCCESSFUL,查询成功
 *			ERR_REC_NO_DISK,无磁盘
 *			ERR_REC_OPEN_DB_FAILED,数据库未初始化
 *			ERR_REC_FAILED，其他原因导致查询失败
 * 修改日志：
 * 			2008-12-02,陶永亮,创建
 */

td_s32 rec_StopQuery(td_u32 _uiSocket);
/*
 * 函数名称：StopQuery
 * 简要描述：停止查询录像或图片文件
 * 输	 入：
 *			_iSocket,SOCKET,用于表示是谁执行了这个查询;0表示UI，>0表示网络客户端，其他非法
 * 输        出：
 *			ERR_REC_WRONG_PARAM,错误的参数
 *			ERR_REC_SUCCESSFUL,停止某个查询成功
 * 修改日志：
 * 			2008-10-23,陶永亮,创建
 */
#if 0
	td_s32 rec_RebuildIndex();
	/*
	 * 函数名称：RebuildIndex
	 * 简要描述：重建所有存储体上的索引
	 * 输	  入：无
	 * 输	  出：
	 *			ERR_REC_SUCCESSFUL,重建索引成功
	 *			ERR_REC_SOME_RECORDING,有通道正在录像
	 * 修改日志：
	 *			2008-08-28,陶永亮,创建
	 */
 #else

td_s32 rec_StartRebuildIdx(td_u32 _socket);
/*
 * 函数名称:rec_StartRebuildIdx
 * 简要描述:重建索引
 * 输	 入:
 *			启动位置
 * 输	 出:
 *			ERR_REC_SUCCESSFUL,操作成功
 *			ERR_REC_FAILED,操作失败
 * 修改日志:
 *			2012-10-22,王磊，创建。
*/
#endif


td_s32 rec_GetAlarmRules(td_s32 _iCh, td_s32 * _pblPreEnable, td_s32 * _piPreTime, td_s32 * _pblDelayEnable, td_s32 * _piDelayTime);
/*
 * 函数名称：GetAlarmRules
 * 简要描述：获取报警录像规则
 * 输	 入:
 *			_iCh,待获取的通道号，0~MAX_CHANNEL_NUM
 * 			_pblPreEnable,输出报警预录使能，非空
 * 			_piPreTime,输出报警预录时间，非空
 *			_ppblDelayTime,用于传出延录使能，非空
 * 			_piDelayTime,输出报警延录时间，非空
 * 输	 出:
 * 			ERR_REC_SUCCESSFUL,获取报警规则成功
 * 			ERR_REC_WRONG_PARAM,错误的参数
 * 修改日志：
 * 			2008-08-28,陶永亮，创建
 */
 td_s32 rec_SetAlarmRules(td_s32 _iCh, td_s32 _blPreEnable, td_s32 _iPreTime, td_s32 _blDelayEnable, td_s32 _iDelayTime);
/*
 * 函数名称:SetAlarmRules
 * 简要描述:设置报警录像规则
 * 输	 入:
 *			_iCh,待设置的通道号，0~MAX_CHANNEL_NUM
 *			_blPreEnable,预录使能，0或1
 *          _iPreTime,预录时间，0~MAX_PRE_TIME
 *			_blDelayEnable,延录使能,0或1
 *			_iDelayTime,延录时间，0~MAX_DELAY_TIME
 * 输	 出:
 *			ERR_REC_SUCCESSFUL,设置成功
 *			ERR_REC_WRONG_PARAM,错误的参数
 * 修改日志:
 *			2008-09-28,陶永亮，创建
 */

td_s32 rec_SetSchedule(td_s32 _iCh,td_s32 _iWeekly,TSchedule* _pstSchedule);
/*
 * 函数名称：SetSchedule
 * 简要描述：设置周期录像模板
 * 输	 入:
 * 			_iCh,通道号,0~MAX_CHANNEL_NUM
 * 			_iWeekly,0~6,一周的7天，0～6分别代表周日到周六
 * 			_pstSchedule,模板，包括时间段和各录像类型的使能，具体内容参见TSchedule的定义
 * 输	 出:
 *			ERR_REC_SUCCESSFUL,设置模板成功
 *			ERR_REC_WRONG_PARAM,错误的参数
 * 修改日志：
 * 			2008-08-29,陶永亮，创建
 */
td_s32 rec_GetSchedule(td_s32 _iCh,td_s32 _iWeekly,TSchedule* _pstSchedule);
/*
 * 函数名称：GetSchedule
 * 简要描述：获取周期录像模板
 * 输	 入:
 * 			_iCh,通道号，0～MAX_CHANNEL_NUM
 * 			_iWeekly,星期，0～6，表示周日到周六
 * 			_pstSchedule,传出周期录像模板，非空
 * 输	 出:
 * 			ERR_REC_SUCCESSFUL,获取周期录像模板成功
 * 			ERR_REC_WRONG_PARAM,错误的参数
 * 修改日志：
 * 			2008-08-29,陶永亮，创建
 */
td_s32 rec_SetRecEnable(td_s32 _iCh,td_s32 _blTimingEnable,td_s32 _blMDEnable,td_s32 _blPortEnable);
/*
 * 函数名称：SetRecEnable
 * 简要描述：设置各种录像类型的使能
 * 输	 入:
 * 			_iCh,通道号，0～MAX_CHANNEL_NUM
 * 			_blTimingEnable,周期录像使能，布尔值
 * 			_blMDEnable,移动侦测录像使能，布尔值
 * 			_blPortEnable,端口报警录像使能，布尔值
 * 输	 出:
 * 			ERR_REC_SUCCESSFUL,设置成功
 * 			ERR_REC_WRONG_PARAM,错误的参数
 */
td_s32 rec_GetRecEnable(td_s32 _iCh,td_s32* _pblTimingEnable,td_s32* _pblMDEnable,td_s32* _pblPortEnable);
/*
 * 函数名称：GetRecEnable
 * 简要描述：获取各种录像类型的使能
 * 输	 入:
 * 			_iCh,通道号，0～MAX_CHANNEL_NUM
 * 			_pblTimingEnable,传出周期录像使能，布尔值，指针，可为NULL
 * 			_pblMDEnable,移动侦测录像使能，布尔值，指针，可为NULL
 * 			_pblPortEnable,端口报警录像使能，布尔值，指针，可为NULL
 * 输	 出:
 * 			ERR_REC_SUCCESSFUL,获取成功
 * 			ERR_REC_WRONG_PARAM,错误的参数
 * 修改日志：
 * 			2008-08-29,陶永亮，创建
 */
 td_s32 rec_GetRecEnableEx(td_s32 _iCh, td_s32 iMode, td_s32* _pblEnable);
td_s32 rec_GetRecordStatus(td_s32 _iCh, td_s32* _piType);
/*
 * 函数名称:GetRecordStatus
 * 简要描述:获取指定通道的录像类型
 * 输    入:
 *			_iCh,通道号，0~MAX_CHANNEL_NUM
 *			_piType,用于传出录像类型，TRecordType,非空
 * 输	 出:
 *			ERR_REC_SUCCESSFUL，获取成功
 *			ERR_REC_WRONG_PARAM,错误的参数
 * 修改日志:
 *			2008-09-18,陶永亮，创建
 */
td_s32  rec_Init();
/* 
 * 函数名称: Init
 * 简要描述:初始化存储模块，初始化全局变量，获取磁盘信息，启动存储线程
 * 输    入:
 *			无
 * 输	 出:
 *			ERR_REC_SUCCESSFUL
 * 修改日志:
 *			2008-09-11,陶永亮，创建
 */
td_s32 rec_Exit();
/*
 * 函数名称:Exit
 * 简要描述:退出存储模块,停止所有录像，关闭数据库
 * 输    入:
 *			无
 * 输    出:
 *		    ERR_REC_SUCCESSFUL
 * 修改日志:
 *			2008-09-17,陶永亮，创建
 */

td_s32 rec_BackupFile(td_s8* _pcFileName, td_s32 _iDiskNo, td_char *_pcDirName);
/*
 * 函数名称:BackUpFile
 * 简要描述:备份指定文件
 * 输	 入:
 *			_pcFileName,需要备份的文件名
 *			_iDiskNo,备份目标磁盘编号
 * 输	 出:
 *			ERR_REC_SUCCESSFUL,备份成功
 *			ERR_REC_WRONG_PARAM,错误的参数
 *			ERR_REC_DISK_FULL，目标磁盘空间不足
 * 修改日志:
 *			2008-10-13,陶永亮，创建
 */

td_s32 rec_BackupPlayer(td_s32 _iDiskNo,const td_s8 *_pcDirName, td_s8* _pcDestPath);
/*
 * 函数名称:rec_BackupPlayer
 * 简要描述:备份播放软件
 * 输	 入:
 *			_pcDestPath,传出需要备份到的U盘路径
 *			_iDiskNo,备份目标磁盘编号
 * 输	 出:
 *			ERR_REC_SUCCESSFUL,备份成功
 *			ERR_REC_WRONG_PARAM,错误的参数
 *			ERR_REC_DISK_FULL，目标磁盘空间不足
 * 修改日志:
 *			2013-02-05,王磊，创建
 */

td_s32 rec_DeleteFile(td_s8* _pcFileName);
/*
 * 函数名称:DeleteFile
 * 简要描述:删除指定文件
 * 输    入:
 *		    _pcFileName,待删除的文件名
 * 输    出:
 *			ERR_REC_SUCCESSFUL,删除成功
 *			SQLITE_ERROR,SQL语句执行错误
 * 修改日志:
 *			2008-09-18,陶永亮，创建
 */
td_s32 rec_DefaultCfg();
/*
 * 函数名称:DefaultCfg
 * 简要描述:恢复存储模块的初始设置
 * 输	 入:
 *          无
 * 输	 出:
 *			ERR_REC_SUCCESSFUL,操作成功
 *			ERR_REC_OPEN_FILE_FAILED,打开配置文件失败
 *			ERR_REC_WRITE_FILE_ERROR,写配置文件错误
 * 修改日志:
 *			2008-09-23,陶永亮，创建
 */

void rec_RecordTimer(unsigned long _param);
/*
 * 函数名称:rec_RecordTimer
 * 简要描述:用于检测录像打包的定时器
 * 输	 入:
 *          无
 * 输	 出:
 *			无
 * 修改日志:
 *			2009-01-23,陶永亮，将原有的线程修改为TIMER。
 */
 
void rec_CheckScheduleTimer(unsigned long _param);
/*
 * 函数名称:CheckScheduleTimer
 * 简要描述:用于检测录像模板的定时器
 * 输	 入:
 *          无
 * 输	 出:
 *			无
 * 修改日志:
 *			2009-01-23,陶永亮，将原有的线程修改为TIMER。
 */

//td_s32 rec_SetDiskException(td_s32 _blDiskFull, td_s32 _blDiskErr, td_s32 _blDisplay, td_s32 _blSound);
/*
 * 函数名称:SetDiskException
 * 简要描述:设置磁盘异常处理参数
 * 输	 入:
 *          _blDiskFull, 0或1， 磁盘满异常处理使能
 *			_blDiskErr, 0或1， 磁盘错异常处理使能
 *			_blDisplay, 0或1， 磁盘异常时屏幕显示
 *			_blSound, 0或1， 磁盘异常时声音提示
 * 输	 出:
 *			ERR_REC_SUCCESSFUL, 设置成功
 * 修改日志:
 *			2009-02-19,陶永亮，创建。
 */

//td_s32 rec_GetDiskException(td_s32* _blDiskFull, td_s32* _blDiskErr, td_s32* _blDisplay, td_s32* _blSound);
/*
 * 函数名称:GetDiskException
 * 简要描述:获取磁盘异常处理参数
 * 输	 入:
 *          _blDiskFull, 为空时不作处理， 传出磁盘满异常处理使能
 *			_blDiskErr, 为空时不作处理， 传出磁盘错异常处理使能
 *			_blDisplay, 为空时不作处理， 传出磁盘异常时屏幕显示
 *			_blSound, 为空时不作处理， 传出磁盘异常时声音提示
 * 输	 出:
 *			ERR_REC_SUCCESSFUL, 获取成功
 * 修改日志:
 *			2009-02-19,陶永亮，创建。
 */


td_s32 rec_DataBaseDiskNo(void);
/*
 * 函数名称:DataBaseDiskNo
 * 简要描述:返回数据库正在使用的磁盘号
 * 输	 入:
 *          无
 * 输	 出:
 *			如成功则返回磁盘编号，失败则返回-1
 * 修改日志:
 *			2009-03-11,陶永亮，创建。
 */

td_u8* rec_TempRecorderPath(int _iDvdNo);
/*
 * 函数名称:TempRecorderPath
 * 简要描述:获取刻录所使用的临时路径
 * 输	 入:
 *          无
 * 输	 出:
 *			临时路径，如失败则返回"/dev/null"
 * 修改日志:
 *			2010-08-02,陶永亮，创建。
 */
td_u8* rec_TempRecorderMd5Path(int _iDvdNo);
/*
 * 函数名称:rec_TempRecorderMd5Path
 * 简要描述:获取刻录所使用的临时路径
 * 输	 入:
 *			无
 * 输	 出:
 *			临时路径，如失败则返回"/dev/null"
 * 修改日志:
 *			2010-08-02,陶永亮，创建。
 */

td_s32 rec_Pause(td_s32 _iParam, td_s32 _iDisk);
/*
 * 函数名称:Pause
 * 简要描述:暂停存储模块，通常用于格式化磁盘时
 * 输	 入:
 *          _iParam,暂停参数.bit0,停止所有录像；bit1，停止所有查询；bit2，关闭数据库；bit3，关闭临时所有文件
 * 输	 出:
 *			ERR_REC_SUCCESSFUL;
 * 修改日志:
 *			2009-04-07,陶永亮，创建。
 */

td_s32 rec_Restart(td_s32 _iDisk);
/*
 * 函数名称:Restart
 * 简要描述:恢复存储模块
 * 输	 入:
 *          无
 * 输	 出:
 *			ERR_REC_SUCCESSFUL
 * 修改日志:
 *			2009-04-07,陶永亮，创建。
 */

td_s32 rec_IfRecEnable(void);
/*
 * 函数名称:IfRecEnable
 * 简要描述:存储模块是否使能
 * 输	 入:
 *          无
 * 输	 出:
 *			1, 使能；0， 不使能
 * 修改日志:
 *			2009-05-11,陶永亮，创建。
 */

td_s32 rec_TryRebuildIndex(void);
/*
 * 函数名称:TryRebuildIndex
 * 简要描述:尝试重建索引
 * 输	 入:
 *          无
 * 输	 出:
 *			ERR_REC_SUCCESSFUL,操作成功
 *			ERR_REC_FAILED,操作失败
 * 修改日志:
 *			2009-05-21,陶永亮，创建。
 */

td_s32 rec_FixBadFileSystem(void);
/*
 * 函数名称:FixBadFileSystem
 * 简要描述:修复坏文件系统
 * 输	 入:
 *          无
 * 输	 出:
 *			ERR_REC_SUCCESSFUL,操作成功
 *			ERR_REC_FAILED,操作失败
 * 修改日志:
 *			2009-06-13,陶永亮，创建。
 */

td_s32 rec_IsRebuildingIndex(void);
/*
 * 函数名称:IsRebuildingIndex
 * 简要描述:查询重建索引状态
 * 输	 入:
 *          无
 * 输	 出:
 *			0,未在重建索引或重建索引结束;1,正在重建索引
 * 修改日志:
 *			2009-08-06,陶永亮，创建。
 */

 
td_s32 rec_SetCustomRecordType(td_s32 _iType, td_u8* _pucName);
/*
 * 函数名称:SetCustomRecordType
 * 简要描述:设置用户自定义录像类型
 * 输	 入:
 *          _iType, 自定义录像类型值
 *			_pucName， 自定义录像类型名
 * 输	 出:
 *			ERR_REC_SUCCESSFUL, 成功
 *			ERR_REC_FAILED, 失败
 *			ERR_REC_WRONG_PARAM, 错误的参数
 * 修改日志:
 *			2009-11-05,陶永亮，创建。
 */

td_u8* rec_GetCustomRecordType(td_s32 _iType);
/*
 * 函数名称:GetCustomRecordType
 * 简要描述:获取用户自定义录像类型
 * 输	 入:
 *          _iType, 自定义录像类型值
 * 输	 出:
 *			NULL, 获取失败
 *			其他, 类型名
 * 修改日志:
 *			2009-11-05,陶永亮，创建。
 */


td_s32 rec_AddReserveInfoToFile(td_s32 _iCh, td_s32 _iFlag);
/*
 * 函数名称:AddReserveInfoToFile
 * 简要描述: 更改录像文件的附加信息
 * 输	 入:
 *			_iCh, 0~MAX_CHANNEL_NUM, 通道号
 *			_iFlag, 0, 修改原有信息；1，追加新的信息
 * 输	 出:
 *			ERR_REC_NO_DBFILE, 数据库未初始化
 *			ERR_REC_SUCCESSFUL, 操作成功
 *			ERR_REC_FAILED,其他原因导致操作失败
 * 修改日志:
 *			2009-12-03,陶永亮，创建。
 */

td_s32 rec_ReadFileHeader(FILE* _fp, td_u8* _pcFileName, S_FileHeader* _pHeader);
/*
 * 函数名称:ReadFileHeader
 * 简要描述:读取文件头
 * 输	 入:
 *			_fp, 待读取的流，如果为空则根据文件打开
 *			_pcFileName, 非空, 待读取的文件
 *			_pHeader, 用于传出文件头
 * 输	 出:
 *			ERR_REC_NO_DBFILE, 数据库未初始化
 *			ERR_REC_SUCCESSFUL, 操作成功
 *			ERR_REC_FAILED,其他原因导致操作失败
 * 修改日志:
 *			2009-12-03,陶永亮，创建。
 */

td_s32 rec_WriteCfg(void);
td_s32 rec_IsValidFrame(S_FrameHeader _stHead);
td_s32 rec_CompleteFileName(td_s8* _pcFileName, td_s8* _pcFullPath, td_s8* _pcPath, td_s8* _pcShortPath);
/*
 * 函数名称: CompleteFileName
 * 简要描述:根据文件补全其绝对路径
 * 输	 入:
 *			_pcFileName,输入文件名,H00CFT20070802113038C38.sdv
 *			_pcFullPath,用于传出文件的绝对路径,/mnt/sata/0/0/20070802/H00CFT20070802113038C38.sdv
 *			_pcPath,用于传出文件的中路径,/mnt/sata/0/0/20070802
 *			_pcShortPath,用于传出文件的短路径,/mnt/ide/0/0
 * 输	 出:
 *			ERR_REC_SUCCESSFUL,操作成功
 * 修改日志:
 *			2008-09-10,陶永亮，创建
 */
td_s32 rec_IsRecordFile(td_u8* _pucFileName);
td_s32 rec_IsValidFileName(td_u8* _pucFileName);
td_s32 rec_AddFileToIndex(td_s8* _pcFullPath, td_s32 _iAtmFlag);
typedef void (*rec_AtmRebuildCallBack)(td_s8* _ucFileName, td_s8* _ucNewFileName, td_s32 _iType);
td_s32 rec_RegAtmRebuildCallBack(rec_AtmRebuildCallBack _CallBack);
typedef void (*rec_AtmRecordUpdateCallBack)(td_s8 *_pcFileName, td_s32 _iFileSize, td_s32 _iStopTime);
td_s32 rec_RegAtmRecordUpdateCallBack(rec_AtmRecordUpdateCallBack _CallBack);


//20130322 SUN Wei add for send pic
typedef void (*rec_PicSendCallBack)(td_s8 * _pcFileName, td_s32 _iCh, td_s32 _ioffset);
td_s32 rec_RegPicSendCallBack(rec_PicSendCallBack _CallBack);


// 录像状态改变通知
typedef void (*rec_StatChgNotifyCallBack)(int _iChn, int _iStat);

td_s32 rec_RegStatNotify(rec_StatChgNotifyCallBack _CallBack);

td_s32 rec_RegStatUiNotify(rec_StatChgNotifyCallBack _CallBack);
#ifdef OverLapBurn
//回调函数增加一个参数，该参数表示该文件是否已经刻录过
typedef void (*rec_StartRecorderNotifyCallBack)(td_s32 _iChn, const char *_strFileName, td_s32 _iFileLen, TFileType _FileType,td_s32 iIsOverBurn);
#else
typedef void (*rec_StartRecorderNotifyCallBack)(td_s32 _iChn, const char *_strFileName, td_s32 _iFileLen, TFileType _FileType);
#endif
typedef void (*rec_GetCDSerialNoCallBack)(int _iCDindex, td_u8* _cSerialNo);
td_s32 rec_RegRecorderStartNotifyCallBack(rec_StartRecorderNotifyCallBack _CallBack);
td_s32 rec_RegGetCDSerialNoCallBack(rec_GetCDSerialNoCallBack _CallBack);

td_s32 rec_GetFdTempIndex();  //获取临时索引文件句柄
//wdlxh td_s32 rec_InitTempIndex();
td_s32 rec_GetFilePathSizeLog();

td_s32 rec_InitBfIndex();//miracle,ws

td_s32 rec_SeekIFrame_Ex(FILE* _fp, td_s32 *_pStop);
#define rec_SeekIFrame(_fp)	rec_SeekIFrame_Ex(_fp, NULL)

td_s32 rec_SetBuf(FILE *_fp, td_s32 _iSize);

td_s32 rec_OpenFile(FILE** _fp, td_s8* _pcFileName);
/*
 * 函数名称:rec_OpenFile
 * 简要描述:打开文件
 * 输	 入:
 *			_fp, 待读取的流指针的指针
 *			_pcFileName, 非空, 待读取的文件
 * 输	 出:
 *			ERR_REC_SUCCESSFUL, 操作成功
 *			ERR_REC_FAILED,其他原因导致操作失败
 * 修改日志:
 *			2011-04-01,王松，创建。
 */

td_s32 rec_CloseFile(FILE **_fp);
/*
 * 函数名称:rec_CloseFile
 * 简要描述:关闭文件
 * 输	 入:
 *			_fp, 待读取的流指针的指针
 * 输	 出:
 *			ERR_REC_SUCCESSFUL, 操作成功
 *			ERR_REC_FAILED,其他原因导致操作失败
 * 修改日志:
 *			2011-04-01,王松，创建。
 */

td_s32 rec_ValidFrame(S_FrameHeader *_pTFrameheader, int _iMaxLen);
td_s32 rec_SeekFile_Ex(FILE*_fp, td_s32 _iPosOrTime, td_s32 *_iRemainT, td_s32 *_pStop);
#define rec_SeekFile(_fp, _iPosOrTime, _iRemainT)	rec_SeekFile_Ex(_fp,  _iPosOrTime, _iRemainT, NULL)
/*
 * 函数名称:rec_SeekFile
 * 简要描述:定位文件
 * 输	 入:
 *			_fp, 待读取的流指针
 *			_iPosOrTime,定位点，当小于100时为百分比定位；否则为时间点定位
 * 输	 出:
 *			ERR_REC_SUCCESSFUL, 操作成功
 *			ERR_REC_FAILED,其他原因导致操作失败
 * 修改日志:
 *			2011-04-01,王松，创建。
 */
td_s32 rec_GetFileStartTime(td_s8* _pcFileName, time_t* _pStartTime);
td_s32 rec_GetFileStopTime(td_s8* _pcFileName, time_t* _pStopTime);
td_s32 rec_GetFileSize(td_s8* _pcFileName, int* _pSize);
td_s32 rec_GetFileTimeLen(td_s8* _pcFileName, time_t* _pTimeLen);

int Storage_GetDebug(char *_cString,int _iLen);
td_s32 rec_WriteBfIndex();
td_s32 rec_GetPeriodOfFileMark(IN char *_strName, OUT int *_iCnt, OUT int *_iMark, int _iLen);
/*
函数作用:按照时间段获取打点信息
入参:
_iStartTm :获取打点信息的开始时间
_iEndTm :获取打点信息的结束时间
_iChn :获取打点信息的通道

_iCnt:文件中打点的个数
_iMark:打点的开始时间
_iLen:_iMark的长度
*/
td_s32 rec_GetPeriodOfTimeMark(int _iChn, time_t _iStartTm, time_t _iEndTm, OUT int *_iCnt, OUT int *_iMark, int _iLen);

//////////////////////////////////////////////////////////////////atm
td_s32 rec_AtmStartRecord(void);
/*
*函数名称:rec_AtmStartRecord
*简要描述:开启卡号录像
*输出:ERR_REC_SUCCESSFUL, 操作成功
*		ERR_REC_FAILED,其他原因导致操作失败
*修改日志:2012-09-14，王松，创建
*/
td_s32 rec_AtmStopRecord(void);
/*
*函数名称:rec_AtmStopRecord
*简要描述:停止卡号录像
*输出:ERR_REC_SUCCESSFUL, 操作成功
*		ERR_REC_FAILED,其他原因导致操作失败
*修改日志:2012-09-14，王松，创建
*/
td_s32 rec_GetNowRecord(int *_iRecChn, char _strFileName[][FILE_NAME_LEN]);
/*
*函数名称:rec_GetNowRecord
*简要描述:获取当前录像信息
*输入:_iRecChn，按位表示各通道是否正在录像
*		_strFileName[][FILE_NAME_LEN],获取存在录像的文件名，此数组长度为MAX_CHANNEL_NUM
*输出:ERR_REC_SUCCESSFUL, 操作成功
*		ERR_REC_FAILED,其他原因导致操作失败
*修改日志:2012-09-14，王松，创建
*/

//////////////////////////////////////////////////////////////////atm
td_s32 rec_GetPartFile(td_u8* _pcFileName, td_u8 *_pClipName, td_s32 _iStartPos, td_s32 _iStopPos);
int rec_playCapPicture(int _iTime, char _cChn, void *_buf, int _iSize);
td_s32 rec_StopCopy(void);
td_s32 rec_ClearStopCopyFlag(void);
int rec_SetAudioStorageStat(int _iChn, int _iEnable);
int rec_GetAudioStorageStat(int _iChn);
/*
*简要描述:截取文件片段
*输入:_iStartPos,开始点之前最近I帧位置，-1表示文件开始段不需截断
*		_iStopPos，结束点P帧结束位置，-1表示文件结束段不需截断
*输出:ERR_REC_SUCCESSFUL, 操作成功
*		ERR_REC_FAILED,其他原因导致操作失败
*/
td_s32 rec_DvrSignToFile(td_s32 _iCh);//add by gyn
//20120913  王磊 add 判断文件是否为当前正在录像文件
td_s32 rec_IsCurrentFile(FILE*_fp);
//20120913  王磊 add 
td_u32 rec_GetCurFileSize(int _iCh);
//20121120  王磊 add 
td_s32 rec_GetRebuildStatus(void);
TFileInfo rec_GetCurFileInfo(td_s32 _iChnNo);
/*
**20130326 王磊add
*修改文件为加锁或解锁
*iLockFlag: 1---加锁，0---解锁
*/
td_s32 rec_RenameFileModifyLock(char *_pcFileName, td_u32 iLockFlag);
td_s32 rec_GetRecFileLockStatus(char *_pcFileName, td_s32 *_piLockStatus);

td_u8* rec_MidRecorderPath(int _iDvdNo);
/*
 * 函数名称:rec_MidRecorderPath
 * 简要描述:获取当前需要刻录的录像文件
 * 输	 入:
 *          无
 * 输	 出:
 *			临时路径，如失败则返回"/dev/null"
 * 修改日志:
 *			2012 06 12
 */
 
td_u8 *rec_ConvertRecorderPath();
/*
 * 函数名称:rec_ConvertRecorderPath
 * 简要描述:获取转换录像的存放路径
 * 输	 入:
 *          无
 * 输	 出:
 *			临时路径，如失败则返回"/dev/null"
 * 修改日志:
 *			2014 11 27
 */
 
  td_u8 *rec_DvdInfoFile();
/*
 * 函数名称:rec_DvdInfoFile
 * 简要描述:获取光盘好盘坏盘的存放路径
 * 输	 入:
 *          无
 * 输	 出:
 *			临时路径，如失败则返回"/dev/null"
 * 修改日志:
 *			2015 04 11
 */
 
td_u8* rec_FinRecorderPath(int _iDvdNo);
/*
 * 函数名称:rec_FinRecorderPath
 * 简要描述:获取刻录一张光盘录像的录像文件
 * 输	 入:
 *          无
 * 输	 出:
 *			临时路径，如失败则返回"/dev/null"
 * 修改日志:
 *			2012 06 06
 */
td_u8* rec_TrueFinRecorderPath(int _iDvdNo);


td_u8* rec_AudRecorderPath(int _iDvdNo);
/*
 * 函数名称:rec_AudRecorderPath
 * 简要描述:获取刻录一张光盘录像的录像文件
 * 输	 入:
 *          无
 * 输	 出:
 *			临时路径，如失败则返回"/dev/null"
 * 修改日志:
 *			2012 06 06
 */

td_s32 rec_OverLapRecorderPath(char *_pcPath);
/*
* 函数名称:rec_OverLapRecorderPath
 * 简要描述:获取复刻配置文件的路径
 * 输	 入:
 *			文件路径
 *
 * 输	 出:成功返回0，否则返回负值
 *			
 * 修改日志:2012-1-29,庄志兵，创建。
 *			
*/
td_s32 rec_Stat(char * _pcFilePath, struct stat * _pstStat);

td_s32 rec_Atoi(char a);//

int rec_GetFileChn(char * _pcFileName);
int rec_GetFileChnEx(char * _pcFileName, OUT int *_piMainOrSub);
#define rec_GetFileChn(_pcFileName) 	rec_GetFileChnEx(_pcFileName, NULL)

td_s32 rec_QueryMemCurFile(TQuery* _pstQuery, TFileInfo *_pstFileInfo);

td_s32 rec_GetIFrameIndexInfo(IN char *_strName, OUT S_FrameHeader *FrameHeader, OUT int *_iLocIFrameIndexHeader, OUT int *_iIndexLen, OUT int *_iIndexCnt, OUT int *_iFileLoc);

td_s32 rec_GetPeriodOfFileMark(IN char *_strName, OUT int *_iCnt, OUT int *_iMark, int _iLen);
/*
函数作用:按照时间段获取打点信息
入参:
_iStartTm :获取打点信息的开始时间
_iEndTm :获取打点信息的结束时间
_iChn :获取打点信息的通道

_iCnt:文件中打点的个数
_iMark:打点的开始时间
_iLen:_iMark的长度
*/
td_s32 rec_GetPeriodOfTimeMark(int _iChn, time_t _iStartTm, time_t _iEndTm, OUT int *_iCnt, OUT int *_iMark, int _iLen);
/********************************假日计划****************************/

/*
 * 函数名称:rec_SetHolidayParam
 * 简要描述:保存假日信息参数
 * 输	 入:
 *			_pstHoliday, 假日信息
 * 输	 出:
 *			td_success, 保存成功
 *			td_failure, 保存失败		
 */
td_s32 rec_SetHolidayParam(THolidayInfo *_pstHoliday);

/*
 * 函数名称:rec_GetHolidayParam
 * 简要描述:保存假日信息参数
 * 输	 入:
 *			_iHolidayId, 假日编号
 * 输	 出:	
 */
td_s32 rec_GetHolidayParam(td_s32 _iHolidayId, THolidayInfo *_pstHolidayInfor);


/*
 * 函数名称:rec_SetHolidaySchedule
 * 简要描述:保存假日模板
 * 输	 入:
 *			_iChannelNo, 通道号
 *			_pstSchedule, 模板信息
 * 输	 出:	
 *			td_success, 保存成功
 *			td_failure, 保存失败
 */	
td_s32 rec_SetHolidaySchedule(td_s32 _iChannelNo, TSchedule* _pstSchedule);

/*
 * 函数名称:rec_GetHolidaySchedule
 * 简要描述:获取假日模板
 * 输	 入:
 *			_iChannelNo, 通道号
 *			_pstSchedule, 模板信息
 * 输	 出:	
 *			td_success, 获取成功
 *			td_failure, 获取失败
 */
td_s32 rec_GetHolidaySchedule(td_s32 _iChannelNo, TSchedule* _pstSchedule);

/************************************end******************************/

/*
**20130327 王磊add
*设置记录录像保留时长的全局变量
*_iChn------通道号
*/
td_s32 rec_SetStoreTime(int _iChn, int _iDays);

/*
**20130401 王磊add
*设置记录录像是否抽帧的全局变量
*_iChn------通道号
*_iEnable---抽帧状态
*/
td_s32 rec_SetPickFrame(int _iChn, int _iEnable);
//福码流录像
td_s32 rec_SetSubRecord(int _iChn, int _iEnable);

/*
**20130401 王磊add
*设置记录录像是否冗余的全局变量
*_iChn------通道号
*_iEnable---冗余录像状态
*/
td_s32 rec_SetRedundRecState(int _iChn, int _iEnable);

//王磊实时获取最大分辨率
td_s32 rec_GetMaxVideoSize(td_s32 _iCh, td_u32 _u8VideoSize, td_s32 _iNorm);

//注册设置主码流(定时)参数回调
td_s32 rec_MainTimeStreamPara(MainTimeChnPara _MainTimeChnParaCallBack);

//标记是否需要切换参数
td_s32 rec_ChangeVencParam(td_s32 _iChn, td_s32 _iVencParamChangeFlag);

TChnRecPara rec_GetChnRecParam(int _iChn);

td_s32 rec_MakeUpIframeIndex(td_u8 * _pcFullFilePath, td_u8 * _pcCDROMSerialNo);
td_s32 rec_UpdateFileTail4CDSerialNo(td_u8 * _pcFullFilePath, td_u8 * _pcCDROMSerialNo);

td_s32 rec_AlterFirstDiskNum(td_s32 _iDiskNum);
//SUN Wei add record Template 
td_s32 rec_SetSmartRecTemplate(td_s32 _iCh,td_s32 _iWeekly,td_s32 _iSegment,td_s32 _iTemplateType);
td_s32 rec_GetSmartRecTemplate(td_s32 _iCh,td_s32 _iWeekly,td_s32 _iSegment);
td_s32 rec_RegSmartRecCallBack(SetSmartRecCallBack _SetSmartRecCallBack);
//SUN Wei add record Template 
//SUN Wei add record Template 
/////////////录像标签/////////////
td_s32 rec_TagModify(int _iOperation, char* _cFileName, int* _piTagTm,char* _cTagName);
td_s32 rec_TagQuery(IN char *_cFileName,IN char* _cTagName, OUT TTagInfo _stTagInfo[], int _iMaxTagNum);
//////////////////////////////////
void rec_doSendStat();
void rec_ResetDBFile(int _iDBNum);

//设置音频打点信息到存储模块，用于合成通道对各单通道音频文件的提取标记
int rec_DvrAudioSignToFile(TAudioTagInfo *_tAudioTagInfo, int _iChnCnt);

//通用打点功能
td_s32 rec_DvrCommonSignToFile(td_s32 _iCh, td_s32 _iType, td_char *_pcTag);

#endif /* STORAGE_H_ */

