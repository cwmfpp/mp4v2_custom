#ifndef _LIB_DISKGRP_H
#define _LIB_DISKGRP_H



#include "public_def.h"
#include "libdiskmnt.h"

//文件打包策略
typedef enum
{
	PACKET_TIME = 0,	//按文件时长打包
	PACKET_SIZE,		//按文件大小打包
}TPacketPolicy;

//磁盘满策略
typedef enum
{
	POLICY_STOP_RECORD = 0,         //停止录像
	POLICY_OVERWRITE_ALL,			//覆盖全部
	POLICY_OVERWRITE_EX_ALARM,		//覆盖除了报警录像文件的所有文件
}TDiskFullPolicy;
#define MAX_EXT_LEN			10		//最大扩展名长度
#define MAX_DISKGRP_NUM		8

#define MIN_IDLE_DISK		2048	//最小剩余磁盘空间，单位: M   杨珏20120601 900->2048
#define	MAX_IDLE_DISK		10240	//最大剩余磁盘空间，单位：M
#define DEFALUT_IDLE_DISK	2048	//默认剩余磁盘空间，单位: M
#define MIN_SNAP_SPACE		40		//最小抓拍空间，单位M

#define MIN_TIME_LEN		2		//最小打包时间
#define MIN_FILE_SIZE		20		//最小打包大小
#define MAX_TIME_LEN		60		//单个录像文件的最大时长，单位:分
#define MAX_FILE_SIZE		1024//200		//单个文件最大大小，单位M

#define FILE_SIZE_500M      251  //单位M
#define FILE_SIZE_1024M     252  //单位M


typedef struct
{
	TDiskFullPolicy 	m_ePolicy;					//磁盘满策略
	td_u8				m_ucExtName[MAX_EXT_LEN];	//录像文件扩展名移植到config_server.ini中了
	TPacketPolicy		m_ePacket;					//打包策略
	td_u8				m_ucPerFileSize;			//单个文件大小，单位M
	td_u8				m_ucPerFileLen;				//单个录像文件时长，单位分
	td_s32				m_iFreeSpace;				//预留磁盘空间,单位M,MIN_IDLE_DISK~MAX_IDLE_DISK

	//Added by Leeon Tonie, 90219, for disk exception.
	//Leeon Tonie 100128 delete below
	//td_u8				m_blHandleType[2];					//处理方式使能，0为屏幕显示，1为声音提示
}TRecordConfig;



const TRecordConfig* dg_GetResHost();

int dg_WriteCfg();
/*
 * 函数名称:dg_WriteCfg
 * 简要描述:磁盘相关配置信息保存，包括录像模式、盘组信息、配额信息、覆盖及打包策略、磁盘用途等
 * 输	 入:
 *          无
 * 输	 出:
 *			0，写配置成功；-1，写配置失败
 * 修改日志:
 *			2013-06-06，某人添加函数说明
 */
 

int dg_DefaultCfg();
/*
 * 函数名称:dg_DefaultCfg
 * 简要描述:磁盘相关配置信息恢复默认
 * 输	 入:
 *          无
 * 输	 出:
 *			
 * 修改日志:
 *			2013-06-06，某人添加函数说明
 */

td_s32 dg_AddChnUsed(int _iChn, TFileType _eType, int _iSize);
/*
 * 函数名称:dg_AddChnUsed
 * 简要描述:统计各通道录像或抓拍的增加
 * 输	 入:
 *          _iChn, 通道号；_eType,文件类型；_iSize,增加大小
 * 输	 出:
 *			
 * 修改日志:
 *			2013-06-06，某人添加函数说明
 */

td_s32 dg_GetGrpDisks(td_s32 _iDiskGrp, td_u32 *_upiDisks);
/*
 * 函数名称:dg_GetGrpDisks
 * 简要描述:获取盘组内包含磁盘信息
 * 输	 入:
 *          _iDiskGrp,盘组号；*_upiDisks,所包含磁盘，按位表示
 * 输	 出:
 *			
 * 修改日志:
 *			2013-06-06，某人添加函数说明
 */

td_s32 dg_SetGrpDisks(td_u32 _uiDisks[]);
/*
 * 函数名称:dg_SetGrpDisks
 * 简要描述:设置所有盘组的磁盘包含信息
 * 输	 入:
 *          _uiDisks[],各盘组对应磁盘信息，数组下标对应盘组号，磁盘信息按位表示
 * 输	 出:
 *			
 * 修改日志:
 *			2013-06-06，某人添加函数说明
 */

td_s32 dg_GetGrpChannels(td_s32 _iDiskGrp, td_u32 *_puiChannels);
/*
 * 函数名称:dg_GetGrpChannels
 * 简要描述:获取盘组对应通道
 * 输	 入:
 *          _iDiskGrp,盘组号；*_puiChannels，所对应通道信息，按位表示
 * 输	 出:
 *			
 * 修改日志:
 *			2013-06-06，某人添加函数说明
 */

td_s32 dg_SetGrpChannels(td_u32 _uiChannels[]);
/*
 * 函数名称:dg_SetGrpChannels
 * 简要描述:设置盘组对应通道
 * 输	 入:
 *          _uiChannels[],各盘组对应通道信息，数组下标对应盘组号，通道信息按位表示
 * 输	 出:
 *			
 * 修改日志:
 *			2013-06-06，某人添加函数说明
 */

td_s32 dg_GetDiskUsage(td_s32 _iDiskNum, td_s32 *_piUsage);
/*
 * 函数名称:dg_GetDiskUsage
 * 简要描述:获取磁盘用途信息
 * 输	 入:
 *          _iDiskNum,磁盘号;*_piUsage,磁盘用途，读写、冗余、只读或备份
 * 输	 出:
 *			
 * 修改日志:
 *			2013-06-06，某人添加函数说明
 */

td_s32 dg_SetDiskUsage(td_s32 _iDiskNum, td_s32 _iUsage);
/*
 * 函数名称:dg_SetDiskUsage
 * 简要描述:设置磁盘用途信息，碍于需求有诸多限制，其中不深合理
 * 输	 入:
 *          _iDiskNum,磁盘号；_iUsage,磁盘用途，读写、冗余、只读、备份
 * 输	 出:
 *			
 * 修改日志:
 *			2013-06-06，某人添加函数说明
 */

td_s32 dg_GetRecPath(td_s32 _iChn, char* _ucRecPath, char* _ucRecPathSed, td_s32 _iRedund);//返回值，0:	路径不变；1:		路径改变
/*
 * 函数名称:dg_GetRecPath
 * 简要描述:获取可用录像路径信息，两种存储模式都调用此函数获取可用路径
 * 输	 入:
 *          _iChn,通道号；*_ucRecPath,传出可用路径；*_ucRecPathSed,传出可用冗余路径；_iRedund,通道的冗余录像属性
 * 输	 出:
 *		返回值表示两路径是否发生变化，0位对应首路径；1位对应冗余路径	
 * 修改日志:
 *			2013-06-06，某人添加函数说明
 */

td_s32 dg_GetChnSpace(td_s32 _iChn, td_u32* _puiUsedRec, td_u32* _puiLimitRec, td_u32* _puiUsedSnap, td_u32* _puiLimitSnap);
/*
 * 函数名称:dg_GetChnSpace
 * 简要描述:获取通道配额信息
 * 输	 入:
 *          _iChn,通道号；*_puiUsedRec,录像已用空间；*_puiLimitRec,录像设定配额；*_puiUsedSnap,抓拍已用空间；*_puiLimitSnap,抓拍设定配额
 * 输	 出:
 *			
 * 修改日志:
 *			2013-06-06，某人添加函数说明
 */

td_s32 dg_SetChnLimitSpace(td_s32 _iChn, td_u32 _uiLimitRec, td_u32 _uiLimitSnap);
/*
 * 函数名称:dg_SetChnLimitSpace
 * 简要描述:设定通道配额信息
 * 输	 入:
 *          _iChn, 通道号；_uiLimitRec, 录像限额；_uiLimitSnap,抓拍限额
 * 输	 出:
 *			
 * 修改日志:
 *			2013-06-06，某人添加函数说明
 */

td_s32 dg_GetStorageMode();
/*
 * 函数名称:dg_GetStorageMode
 * 简要描述:获取录像模式信息
 * 输	 入:
 *          无
 * 输	 出:
 *		0，盘组模式；1，配额模式	
 * 修改日志:
 *			2013-06-06，某人添加函数说明
 */

td_s32 dg_SetStorageMode(td_s32 _iMode);
/*
 * 函数名称:dg_SetStorageMode
 * 简要描述:设置录像模式信息
 * 输	 入:
 *          _iMode, 录像模式，0，盘组模式；1，配额模式
 * 输	 出:
 *			
 * 修改日志:
 *			2013-06-06，某人添加函数说明
 */

td_s32 dg_GetExistDisks(td_s32 *_piDisk);

td_s32 dg_SetExitStatus();

td_s32 dg_Init();

td_s32 dg_Uninit();

td_s32 rec_GetPartSpace(const char* _pucPath, TPartInfo* _stInfo);

td_s32 rec_SetDiskStatus(td_s32 _iStat);

td_s32 rec_HandleIOError(td_s32 _Type, td_u8* _pcPath, const td_u8* _pcFunc);

td_s32 rec_DetectNoDisk(void);
/*
 * 函数名称:DetectNoDisk
 * 简要描述:检测无磁盘异常
 * 输	 入:
 *          无
 * 输	 出:
 *			0,无磁盘异常消失;1,无磁盘异常发生
 * 修改日志:
 *			2009-11-27,陶永亮，创建。
 */
 
td_s32 rec_DetectFullDisk(void);
/*
 * 函数名称:DetectFullDisk
 * 简要描述:检测磁盘满异常
 * 输	 入:
 *          无
 * 输	 出:
 *			0,磁盘满异常消失;1,磁盘满异常发生
 * 修改日志:
 *			2009-11-27,陶永亮，创建。
 */
 
td_s32 rec_DetectIOError(void);
/*
 * 函数名称:DetectIOError
 * 简要描述:检测磁盘IO错误
 * 输	 入:
 *          无
 * 输	 出:
 *			0,磁盘IO错误消失;1,磁盘IO错误发生
 * 修改日志:
 *			2009-11-27,陶永亮，创建。
 */
 //检测无冗余磁盘异常
td_s32 rec_DetectNoRebundanceDisk(void);

const TRecordConfig* dg_GetRecHost();
/*
 * 函数名称:dg_GetNoDiskRecHost
 * 简要描述:获取无硬盘模式下的录像打包策略
 * 输	 入:
 *          无
 * 输	 出:
 *			TRecordConfig*
 * 修改日志:
 *			2015-05-13,王自强，创建。
 */

const TRecordConfig* dg_GetNoDiskRecHost();

td_s32 rec_SetStoreRules(td_s32 _iPolicy, td_s32 _iIdleDisk);
/*
 * 函数名称：SetRules
 * 简要描述：设置存储规则
 * 输	 入:
 * 			_iPolicy,覆盖策略,TDiskFullPolicy
 * 			_iIdleDisk,剩余磁盘空间,单位M,0~MAX_IDLE_DISK
 * 输	 出:
 *			ERR_REC_SUCCESSFUL,设置存储规则成功
 *			ERR_REC_WRONG_PARAM,错误的参数
 * 修改日志：
 * 			2008-08-28,陶永亮,创建
 */
td_s32 rec_GetStorageRules(td_s32* _piPolicy, td_s32* _piIdleDisk);
/*
 * 函数名称：GetStorageRules
 * 简要描述：获取存储规则
 * 输	 入:
 * 			_piPolicy,存储策略，非空
 * 			_piIdleDisk,最小剩余磁盘空间，非空
 * 输	 出：
 * 			ERR_REC_SUCCESSFUL,获取存储规则成功
 * 			ERR_REC_WRONG_PARAM,错误的参数
 * 修改日志：
 * 			2008-08-28,陶永亮，创建
 */
 td_s32 rec_SetPacketRules(td_s32 _iPacket, td_s32 _iFileLen, td_s32 _iFileSize);
/*
 * 函数名称:SetPacketRules
 * 简要描述:设置打包规则
 * 输	 入:
 *			_iPacket,打包策略，TPacketPolicy
 *			_iFileLen,单个录像文件时长,0~MAX_TIME_LEN
 *			_iFileSize,单个录像文件大小，0~MAX_FILE_SIZE
 * 输	 出:
 *			ERR_REC_SUCCESSFUL,设置成功
 * 修改日志:
 *			2008-09-28,陶永亮，创建
 */
 td_s32 rec_GetPacketRules(td_s32 * _piPacket, td_s32 * _piFileLen, td_s32 * _piFileSize);
/*
 * 函数名称:GetPacketRules
 * 简要描述:获取打包规则
 * 输	 入:
 *			_piPacket,用于传出打包策略，非空
 *			_piFileLen,用于传出单个录像文件时长，非空
 *			_piFileSize,用于传出单个录像文件大小，非空
 * 输	 出:
 *			ERR_REC_SUCCESSFUL,获取成功
 * 修改日志:
 *			2008-09-28,陶永亮，创建
 */
 #if 0
td_s32 rec_SetExtName(const td_u8* _cst_pucNewExt);
/*
 * 函数名称：SetExtName
 * 简要描述：设置录像文件的扩展名
 * 输	 入:
 *			_pucNewExt,新的文件扩展名,非空
 * 输	 出:
 *			ERR_REC_SUCCESSFUL,设置文件扩展名成功
 *			ERR_REC_WRONG_PARAM,设置文件扩展名成功
 * 修改日志:
 *			2008-08-28,陶永亮，创建
 */
td_s32 rec_GetExtName(td_u8* _pucExtName);
/*
 * 函数名称：GetExtName
 * 简要描述：获取录像文件的扩展名
 * 输	 入:
 *			_pucNewExt,输出文件扩展名，非空
 * 输	 出:
 *			ERR_REC_SUCCESSFUL,获取文件扩展名成功
 *			ERR_REC_WRONG_PARAM,错误的参数
 * 修改日志：
 *			2008-08-28,陶永亮，创建
 */
 #endif
td_s32 rec_OverWriteImages(void);
/*
 * 函数名称:OverWriteImages
 * 简要描述:覆盖图片。该函数是阻塞式操作，每24h调用一次即可。不是接口，需要在定时器中调用
 * 输	 入:
 *			无
 * 输	 出:
 *			ERR_REC_NO_DBFILE, 数据库未初始化
 *			ERR_REC_SUCCESSFUL, 操作成功
 *			ERR_REC_FAILED,其他原因导致操作失败
 * 修改日志:
 *			2009-12-03,陶永亮，创建。
 */

td_s32 dg_CalcAllDiskSixe(void);

typedef void (*dg_AlterFirstDiskCallBack)(td_s32 _iDiskNum);
td_s32 dg_RegAlterFirstDiskCallBack(dg_AlterFirstDiskCallBack _CallBack);



#endif
