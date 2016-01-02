/*******************************************************************
文件名	:DiskManagement.h
公司	:天津天地伟业数码科技有限公司
创建者	:陶永亮
创建日期:2008-08-29
简要描述:磁盘管理相关变量、宏的定义，接口函数原型
修改记录:2008-08-29,陶永亮，创建
        
******************************************************************/

#ifndef DISKMANAGMENT_H_
#define DISKMANAGMENT_H_

#include "smart.h"
#include "td_types.h"

#ifndef FULL_PATH_LEN
#define FULL_PATH_LEN		64
#endif

#define DEV_PATH	 "/dev"
#define SATA_PATH    "/mnt/sata" //SATA硬盘挂载根目录
#define USB_PATH     "/mnt/usb" //USB设备挂载目录
#define NFS_PATH      "/mnt/nfs"  //NFS挂载目录
#define ESATA_PATH   SATA_PATH   //eSATA设备挂载目录
#define SDCARD_PATH   "/mnt/sdcard"   //SdCard设备挂载目录
#define DB_FILE_NAME  "index.db"//新建数据库文件名
#define MAX_BAD_FILE_SYSTEM	16		//最大只读文件系统个数

//网络协议中硬盘ID设定
#define BASEID_HDD_GROUP0  0    //老协议的硬盘起始设备号
#define BASEID_HDD_GROUP1  1008 //新扩展的硬盘起始设备号
#define DEVNUM_HDD_GROUP   8    //硬盘编号分组，每组内硬盘个数
#define BASEID_VD          2000 //VD起始设备号
#define BASEID_IPSAN       3000 //IPSAN起始设备号
#define BASEID_USB         8    //USB起始设备号
#define BASEID_ESATA       32   //eSATA起始设备号
#define BASEID_ESATA_UK    14   //UK协议的eSATA编号，与网络协议不同
#define BASEID_SD          50   //SDCARD起始设备号


//硬盘用途
typedef enum
{
	RECORD = 0,									//录像
	BACKUP,										//备份
	REDUNDANCE,
	READONLY,
}TDiskUsage;

//文件系统类型
typedef enum
{
	FS_EXT3 = 0,		//ext3文件系统
	FS_EXT2,			//ext2文件系统
	FS_FAT32,			//FAT32文件系统
}TFSType;

//存储体类型
typedef enum
{
	DISK_USB = 1,	//USB存储设备
	DISK_NFS,		//网络文件系统
	DISK_SATA,		//SATA硬盘
	DISK_RECORDER,	//DVR刻录机
	DISK_ESATA,
	DISK_SD,        //SDCard
}TDiskType;

typedef enum
{
	FORMAT_MODE_NORMAL = 0,	//正常格式化
	FORMAT_MODE_FAST = 1, 	//快速格式化
}TFormatMode;

//格式化磁盘参数
typedef struct
{
	td_s32 		m_iDiskNo;					//正在格式化的磁盘编号
	TFSType 	m_eFsType;					//文件系统类型
	TFormatMode m_eMode;					//格式化类型，快速还是正常
}TFormatArg;

//磁盘状态
typedef enum
{
	STATE_NONE = 0,								//该通道无磁盘
	STATE_UNFORMATED,							//该磁盘未格式化
	STATE_FORMATED,								//该磁盘已格式化
	STATE_MOUNTED,								//该磁盘已挂载
	STATE_USING,								//该磁盘当前正在读写
}TDsikState;

//磁盘信息
typedef struct
{
	td_u32		m_uiDiskNo;							//磁盘编号
	td_u64		m_ulTotalSize;						//磁盘总大小,单位Bytes
	td_u64		m_uldleSize;						//剩余空间大小，单位Bytes
	td_u64		m_ulUsedSize;						//已使用的空间大小，单位Bytes
	td_s32		m_iPartNum;							//分区数目
	//TDsikState	m_blHasFormated;				//是否格式化
	TDsikState	m_eState;							//磁盘状态，无磁盘，未格式化，已格式化
	td_u8		m_ucDeviceFilePath[FULL_PATH_LEN];	//设备文件路径
	td_u8		m_blSleeping;						//是否休眠
	time_t		m_tLastActiveTime;					//最后活跃时间
	TDiskType	m_eDiskType;						//存储设备类型
	TDiskUsage  m_eUsage;							//用途，TDiskUsage
}TDiskInfo;

typedef enum
{
	ALL_DISK = 0,
	SATA,
	USB,
	NFS,
	ESATA,
	SDCARD,
}TDiskTypeNum;

//分区信息
typedef struct
{
	td_u8		m_ucPart;								//分区编号
	td_u64		m_ulTotal;								//总分区大小
	td_u64		m_ulFree;								//剩余空间大小
	td_u64		m_ulUsed;								//已使用空间大小
}TPartInfo;

typedef struct
{
	td_s32 m_iDiskId; //磁盘id号
	td_s32 m_iDiskPartId; //磁盘分区id 号
	td_s32 m_iDiskPartWriteProtect;					//磁盘分区写保护
	char m_strDiskName[32]; //磁盘名
	td_u64 m_ullDiskPartSize; 			//磁盘分区大小
	td_u64 m_ullDiskPartUsed;			//磁盘分区已用空间
	td_u64 m_ullDiskPartFree;			//磁盘分区空闲空间
}TDiskPartMsg;

td_s32 disk_FormatDisk(td_s32 _iDiskNo,td_s32 _iFSType, td_s32 _iMode);
/*
 * 函数名称：FormatDisk
 * 简要描述：格式化硬盘
 * 输	 入:
 * 			_iDiskNo,磁盘编号，0~MAX_DISK_NUM
 * 			_iFSType,文件系统类型，TFSType
 * 输	 出:
 * 			ERR_REC_SUCCESSFUL,格式化成功
 * 			ERR_REC_WRONG_PARAM,错误的参数
 * 修改日志：
 * 			2008-08-29,陶永亮，创建
 */
td_s32 disk_PartDisk(td_s32 _iDiskNo,td_s32 _iPartNum,td_s32 _blIfFormat);
/*
 * 函数名称：PartDisk
 * 简要描述：磁盘分区
 * 输	 入:
 * 			_iDiskNo,磁盘编号，0~MAX_DISK_NUM
 * 			_iPartNum,分区数目，1~MAX_PARTITION_NUM
 * 			_blIfFormat,分区后是否立即格式化，布尔值
 * 输	 出:
 * 			ERR_REC_SUCCESSFUL,分区成功
 * 			ERR_REC_WRONG_PARAM,错误的参数
 * 修改日志：
 * 			2008-08-29,陶永亮，创建
 */
td_s32 disk_IsCheckDiskExist(td_s32 _iDisk);
 /*
  * 函数名称：IsCheckDiskExist
  * 简要描述：磁盘带电拔掉的检查
  * 输	  入:无
  * 输	  出:坏掉的磁盘编号
  * 修改日志：
  * 		 2012-12-21,王磊，创建
  */

 td_s32 disk_GetDiskNum(TDiskTypeNum _iType);
 /* 函数名称:GetDiskNum
  * 简要描述:获取SATA磁盘的数目
  * 输	  入:
  			_iType,用于传入要获取那种磁盘的数目。
  *			
  * 输	  出:
  *			 0 表示返回无磁盘
  			 > 0 表示返回磁盘数
  *			
  * 修改日志:
  *			 2011-03-15,yjq 修改
  */
 td_s32 disk_GetDiskInfo(td_s32 _iDiskNo, TDiskInfo* _pstInfo);
 /* 函数名称:GetDiskInfo
  * 简要描述:获取对应磁盘号的信息
  * 输	  入:
  *			 _iDiskNo,磁盘编号,0~MAX_DISK_NUM
  *			 _pstInfo,用于传出磁盘信息，非空
  * 输	  出:
  * 		 ERR_REC_SUCCESSFUL，获取磁盘信息成功
  * 	     ERR_REC_WRONG_PARAM,错误的参数
  * 修改日志:
  *			 2008-09-18,陶永亮，创建
  */
 td_s32 disk_GetFormatProgress(td_s32 _iDiskNo, td_s32* _pucP);
 /* 函数名称:GetFormatProgress
  * 简要描述:获取当前格式化的进度
  * 输	  入:
  *			 _iDiskNo,正在格式化的磁盘编号
  *			 _pucP,用于传出当前磁盘格式化的进度
  * 输	  出:
  * 		 ERR_REC_SUCCESSFUL,获取成功
  * 		 ERR_REC_WRONG_PARAM,错误的参数
  * 修改日志:
  * 		 2008-09-18,陶永亮，创建
  */

 td_s32 disk_SetDiskUsage(td_s32 _iDiskNo, TDiskUsage _eUsage);
 
 /* 函数名称:SetDiskUsage
  * 简要描述:获取当前格式化的进度
  * 输	  入:
  *			 _iDiskNo,磁盘编号,0~MAX_DISK_NUM
  *			 _eUsage,用途，录像或备份
  * 输	  出:
  * 		 ERR_REC_SUCCESSFUL,获取成功
  * 		 ERR_REC_WRONG_PARAM,错误的参数
  * 修改日志:
  * 		 2008-10-23,陶永亮，创建
  */
 td_s32 disk_DMInit(void (*_ReportDiskInitNum)(int));
 /*
  * 函数名称:DMInit
  * 简要描述:初始化磁盘管理模块，挂载SATA、NFS，启动USB挂载线程
  * 输	  入:
  *			 无
  *	输	  出:
  *			 ERR_REC_SUCCESSFUL,初始化成功
  * 修改日志:
  *			 2008-09-25,陶永亮，创建
  */


#define VIR_DISK_NUMBER			Cfg_gFlyBurnSataNum()	//虚拟磁盘编号
td_s32 disk_GetDistIsExist(void);
/*
 * 函数名称:disk_GetDistIsExist
 * 简要描述:获取是否有硬盘
 * 输	 入:
 *          无
 * 输	 出:
 *			如成功则返回td_success，失败则返回td_failure
 * 修改日志:
 *			2015-05-14,wzq，创建。
 */
 td_s32 disk_DMExit();
 /*
  * 函数名称:DMExit
  * 简要描述:安全退出磁盘管理模块
  * 输	  入:
  *			 无
  * 输	  出:
  *			 ERR_REC_SUCCESSFUL,退出成功
  * 修改日志:
  *			 2008-09-25,陶永亮，创建
  */

 td_s32 disk_UMountNfs();
 /*
  * 函数名称:UMountNfs
  * 简要描述:卸载网络文件系统
  * 输	  入:
  *			 无
  * 输	  出:
  *			 ERR_REC_SUCCESSFUL,卸载成功
  *			 ERR_REC_UMOUNT_NFS_FAILED, 卸载失败
  * 修改日志:
  *			 2008-09-25,陶永亮，创建
  */
  
 td_s32 disk_MountNfs(td_s8 * _pcArg);
 /*
  * 函数名称:MountNfs
  * 简要描述:挂载网络文件系统
  * 输	  入:
  *			 _pcArg, 挂载网络文件系统所需参数，格式为IP:Path
  * 输	  出:
  *			 ERR_REC_SUCCESSFUL,挂载成功
  *			 ERR_REC_WRONG_PARAM, 参数错误
  *			 ERR_REC_MOUNT_NFS_FAILED,挂载失败
  * 修改日志:
  *			 2008-09-25,陶永亮，创建
  */

void disk_NeedSleepTimer(unsigned long param);
 /*
  * 函数名称:NeedSleepTimer
  * 简要描述:硬盘休眠检测定时器
  * 输	  入:
  *			 无
  * 输	  出:
  *			 ERR_REC_SUCCESSFUL,检测成功
  *			 ERR_REC_FAILED
  * 修改日志:
  *			 2008-01-16,陶永亮，创建
  */

void disk_WakeUpDisk(td_s32 _iDiskNo);
 /*
  * 函数名称:WakeUpDisk
  * 简要描述:唤醒磁盘
  * 输	  入:
  *			 _iDiskNo, SATA磁盘编号,0~MAX_SATA_NUM
  * 输	  出:
  *			 无
  * 修改日志:
  *			 2010-01-20,陶永亮，创建
  */
  
td_s32 disk_FormatFail(td_s32 _iDiskNo);
 /*
  * 函数名称:FormatFail
  * 简要描述:格式化磁盘失败的回调函数
  * 输	  入:
  *			 _iDiskNo, 磁盘编号,0~MAX_DISK_NUM
  * 输	  出:
  *			 ERR_REC_SUCCESSFUL,操作成功
  *			 ERR_REC_WRONG_PARAM, 错误的参数
  * 修改日志:
  *			 2009-04-07,陶永亮，创建
  */

td_s32 disk_GetUSBPath(td_s32 _iUsbNo, td_s32 _iPartNo, td_u8* _pcPath);
/*
 * 函数名称:GetUSBPath
 * 简要描述:获取某个USB磁盘分区的挂载路径
 * 输	  入:
 *			 _iUsbNo, U盘编号,0~MAX_USB_NUM
 *			 _iPartNo, 分区编号,0~MAX_PART_NUM
 *			 _pcPath, 传出挂载路径，非空
 * 输	  出:
 *			 ERR_REC_SUCCESSFUL,操作成功
 *			 ERR_REC_WRONG_PARAM, 错误的参数
 *			 ERR_REC_WRONG_DISK,错误的磁盘编号或者分区编号
 * 修改日志:
 *			 2009-12-06,陶永亮，创建
 */
	
//Not interface
//td_s32 extract_file_path(td_u8* _pucFilePath);
//td_s32 make_dir(td_u8* _pcDirName);

td_s32 disk_GetDiskPartMsg(td_s32 _iDiskNo, td_s32 _iPartNo, TDiskPartMsg* _tMsg);

td_s32 disk_AddBadFileSystem(td_u8* _pcPath);
td_s32 disk_DelBadFileSystem(td_u8* _pcPath);

td_s32 disk_IsBadFileSystem(td_u8* _pcPath);
td_s32 disk_InitBadFileSystem(void);
td_s32 disk_GetMountNfsFlag(); //获取是否需要挂载NFS标识
td_s32 disk_SetMountNfsFlag(td_s32 _iFlag); //设置挂在NFS标识

td_s32 disk_SetInUseDiskNo(td_s32 _iDiskNo); //代替了调用 td_s32 rec_DataBaseDiskNo(void); 接口

// 磁盘消息通知
typedef enum
{
	FORMATE_INUSE_DISK, //格式化使用磁盘
	DISK_READY,			//有磁盘准备好
	NFS_MOUNT,			//NFS挂载
	NFS_UNMOUNT,		//NFS卸载
	USB_MOUNT,		//USB挂载
	ESATA_MOUNT,		//eSATA挂载
	SATA_PART,		//磁盘分区
}TEN_DiskSetNotify;

typedef void (*disk_Notify)(td_s32 _iAction, td_s32 _iDisk);
td_s32 disk_SetNotify(disk_Notify _Notify);

const TDiskInfo *disk_GetDiskInfoArray();
td_u8* disk_GetBadFileSystem(td_s32 _iBad);

td_s32 disk_GetSmartMsg(td_s32 _iDiskNo, TSmartMsg *_ptMsg, int _iMaxCnt);
td_s32 disk_SetSmartStatus(td_s32 _iDiskNo, td_s32 _iStatus);
td_s32 disk_GetSmartBasicMsg(td_s32 _iDiskNo, TSmartBasicMsg *_ptMsg);
//20121115 王磊add 对U盘实现卸载加载工作
int disk_UmonutMountUsbEx(int _iDiskNo, char *_strPath);
#define disk_UmonutMountUsb(_iDiskNo)	disk_UmonutMountUsbEx(_iDiskNo, NULL)
#define disk_UmonutMountUsbPath(_strpath)	disk_UmonutMountUsbEx(0, _strpath)
td_s32 disk_GetExistDisks();

td_s32 disk_StopSata(td_s32 _iDiskNo);
char* disk_GetDevName(char* _pcDevName, td_s32 _iDiskNo);
td_s32 disk_DisknoToUniqID(td_s32 _iDiskNo);
td_s32 disk_HddnoToUniqID(td_s32 _iHddNo);
td_s32 disk_UniqIDToDiskno(td_s32 _iUniqID);

td_s32 raid_Init();
td_s32 raid_StartUp();
td_s32 raid_Shuwdown();
td_s32 raid_GetVDInfo(TRaidVdStat *_pstRaidVdStat, td_s32 *_piTotal);
td_s32 raid_GetRAIDInfo(TRaidArrayStat *_pstRaidArrayStat, td_s32 *_piTotal);
td_s32 raid_CreateRAID(td_s8 *_pcRaidName, td_s32 _iRaidType, td_s32 *_piDisklist, td_s32 _iTotal);
td_s32 raid_DeleteRAID(td_s8 *_pcRaidName);
td_s32 raid_RebuildRAID(td_s8 *_pcRaidName, td_s32 *_piDisklist, td_s32 _iTotal);
td_s32 raid_GetVDCfgMsg(td_s32 _iNo, TRaidVdCfg *_pstRaidVdCfg);
td_s32 raid_GetRaidStat(td_s32 *_piEnable);
td_s32 raid_GetHDInfo(TSataDiskCfg *_pstSataDiskCfg, td_s32 *_piTotal);
td_s32 raid_GetRaidArrayCfgMsg(td_s32 _iNo, TRaidArrayCfg *_pstRaidArrayCfg);
td_s32 raid_AddSpare(td_s32 _iHddNo, td_s8 *_pcRaidName, td_s32 _WorkMode);
td_s32 raid_DelSpare(td_s32 _iHddNo, td_s8 *_pcRaidName, td_s32 _WorkMode);
td_s32 raid_CreateVD(TRaidVdCfg * _pstRaidVdCfg);
td_s32 raid_DeleteVD(td_s8 *_pcVDName);
td_s32 raid_RebuildVD(td_s8 *_pcVDName);
#if JMB39X == ON
td_s32 raid_JMB393_MapDisc(int _iDisk);
void raid_JMB393_ScanDisc(int _iDisk);
#endif

#endif //DISKMANAGMENT_H_

