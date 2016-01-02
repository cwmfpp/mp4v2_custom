/*******************************************************************
文件名	:maintenance.h
公司	:天津天地伟业数码科技有限公司
创建者	:陶永亮
创建日期:2009-10-28
简要描述:系统维护相关数据定义、接口函数原型
修改记录:2009-10-28,陶永亮，整理创建
        
******************************************************************/

#ifndef _MAINTENANCE_H_
#define _MAINTENANCE_H_

#define IMPORT_CONFIG_FILE				"/tmp/config.box"	//导入配置文件后的临时存放地
#define EXPORT_CONFIG_FILE                      "../tmp/config.box"

#define UPGRADE_V2

//for verifying file head while upgrading
#ifdef UPGRADE_V2
#define DVR_BIN         			(1 << 8)    	//DVR-内核，原为1001
#define DVR_WEB         			(2 << 8)    	//DVR-WEB程序，原为1002
#define DVR_FS          			(3 << 8)    	//DVR-文件系统内其他文件，原为1003

#define DVR_SLAVE					(4 << 8) 		//DVR-升级从片文件系统
#define DVR_GUI						(5 << 8) 		//DVR-升级GUI主程序
#define DVR_STARTUP			        (6 << 8) 		//DVR-升级启动画面
#define DVR_CONFIG					(7 << 8)		//DVR-配置文件备份

//NVS/IPC：
#define	NVS_BIN_S					(1 << 16)	//S系列-1/4路内核
#define	NVS_WEB_S					(2 << 16)	//S系列-Web程序
#define	NVS_BIN_FS					(3 << 16)	//S系列-文件系统内其他文件升级
#define	NVD_BIN_S					(4 << 16)	//S系列-解码器内核


#define BOX_FILE_NAME_LEN			128

#define BOX_FILE_HEADER_LEN 176


#define CHIP_All		0X0	//适用于所有主芯片；或者在升级匹配时忽略该条件
#define CHIP_HI3531	0X1	//Hi3531
#define CHIP_HI3521	0X2	//Hi3521/Hi3520A
#define CHIP_HI3520D	0X3	//Hi3520D/Hi3515A
#define CHIP_HI3535	0X4	//Hi3535
#define CHIP_HI3518A	0X5	//Hi3518A/ Hi3518C/Hi3516C
#define CHIP_HI3516	0X6	//Hi3516
#define CHIP_TI8127	0X7	//TI8127


#else
#define DVR_BIN         				1001    	//DVR-内核
#define DVR_WEB         				1002    	//DVR-WEB程序
#define DVR_FS          				1003    	//DVR-文件系统内其他文件
#define DVR_SLAVE					1004		//DVR-升级从片文件系统
#define DVR_GUI						1005		//DVR-升级GUI主程序
#define DVR_STARTUP				1006		//DVR-升级启动画面
#define DVR_CONFIG					1007		//DVR-配置文件备份

#define BOX_FILE_NAME_LEN			32
#endif


//文件头结构：
typedef struct
{
    unsigned char	cCompany[8];	//= "Tiandy"	
    unsigned int	u32Filesize;	//文件总大小，包含文件头
    unsigned int	u32ProductID;	//产品ID，NVS-S内核程序=1，NVS-S网页程序=2，其他另行定义
    unsigned int	u32Version;		//版本号，固定自增
    unsigned int	u32Reserved;	//保留
    unsigned long long u64Checknum; //校验和，64位，在Windows下需要用 long long 类型
}T_BFHeader;

//系统退出时的选项
typedef enum
{
	EXIT_SAVE_CONFIG = 0,										//保存配置文件
	EXIT_STOP_FEEDDOG,											//停止喂狗
	EXIT_DISABLE_DOG,											//将狗置为不使能
	EXIT_STOP_PEVIEW,
}TExitParam;

//工作模式
typedef enum
{
	WORK_MODE_NORMAL,											//正常的工作模式
	WORK_MODE_GUIDE,											//向导工作模式
	WORK_MODE_SAFE,												//安全工作模式
	WORK_MODE_TEST,												//调试工作模式
	WORK_MODE_MAX,												//工作模式数目，新的模式再以上添加
}TWorkMode;

//zhangsheng 自动维护
typedef struct
{
	td_s32 m_iRebootDay;
	td_s32 m_iReboottime;//当天起流过的秒数
}TSysAutoMaintain;

int UpgradeWebPageReType(int _len, td_u8* _buf);
int UpgradeWebPageAnalyse(int _len, td_u8* _buf);
int UpgradeWebPagestart(td_u8* _buf,int *_pipos);

int UpgradeProgam(int _len, td_u8* _buf);

int UpgradeWebPage(int _len, td_u8* _buf);

td_s32 mtn_Upgrade(td_u8* _pcIP, td_u8* _pcFileName, td_u8* _pcUserName, td_u8* _pcPassWd);
/*
 * 函数名称：Upgrade
 * 简要描述：本地升级文件,通过FTP或者通过USB设备。
 * 输	  入：
 *			_pcIP,FTP服务器地址，如 10.10.60.200；如果为空则表示本地通过USB升级
 *			_pcFileName,待升级文件名，非空,如 /ftp/update/blade.bin
 *			_pcUserName,FTP站点用户名，非空 如 Admin
 *			_pcPassWd,FTP站点密码,非空, 如 Admin
 * 输	 出：
 * 			ERR_SYS_NO_FILE, 无可用升级文件
 *			ERR_SYS_OTHER_UPDATING, 其他客户端正在进行升级
 *			ERR_SYS_NO_FTP, 无可用FTP站点
 *			ERR_SYS_WRONG_PARAM, 错误的参数
 *			ERR_SYS_FAILED, 其他原因导致失败
 * 			ERR_SYS_SUCCESSFUL,操作成功
 * 修改日志：
 * 			2008-12-09,陶永亮,创建
 */

td_s32 mtn_ValidFileList(char _cFileList[][256], td_s32* _piFileNum);
/*
 * 函数名称：ValidFileList
 * 简要描述：通过FTP进行升级
 * 输	  入：
 *			_pcIP,FTP服务器地址，非空,如 10.10.60.200
 *			_pcFileName,待升级文件名，非空,如 /ftp/update/blade.bin
 *			_pcUserName,FTP站点用户名，非空 如 Admin
 *			_pcPassWd,FTP站点密码,非空, 如 Admin
 * 输	 出：
 * 			ERR_SYS_NO_FILE, 无可用升级文件
 *			ERR_SYS_OTHER_UPDATING, 其他客户端正在进行升级
 *			ERR_SYS_NO_USB, 无可用的USB设备
 *			ERR_SYS_WRONG_PARAM, 错误的参数
 *			ERR_SYS_FAILED, 其他原因导致失败
 * 			ERR_SYS_SUCCESSFUL,操作成功
 * 修改日志：
 * 			2008-12-09,陶永亮,创建
 */

td_s32 mtn_IpcValidFileList(char _cFileList[][256], td_s32* _piFileNum);
 
//td_s32 sys_UpdateApp(td_s32 _iUpdateType, td_u8* _iFtpAddr);
/*
 * 函数名称：UpdateApp
 * 简要描述：升级内核程序
 * 输	  入：
 			_iUpdateType,升级类型，GUI_SYS_UPDATE_USE_USB 或 GUI_SYS_UPDATE_USE_FTP
 * 			_iFtpAddr， FTP地址，非空
 * 输	 出：
 * 			ERR_SYS_NO_FILE, 无可用升级文件
 *			ERR_SYS_NO_USB, 无可用USB存储设备
 *			ERR_SYS_NO_FTP, 无可用FTP站点
 *			ERR_SYS_WRONG_PARAM, 错误的参数
 *			ERR_SYS_FAILED, 其他原因导致失败
 * 			ERR_SYS_SUCCESSFUL,操作成功
 * 修改日志：
 * 			2008-12-09,陶永亮,创建
 *			2009-12-07,陶永亮，由于FTP升级扩充了参数，该接口停止使用
 */

td_s32 mtn_UpdateProgress(void);
/*
 * 函数名称：UpdateProgress
 * 简要描述：获取升级内核程序进度
 * 输	  入：
 *			无
 * 输	 出：
 * 			
 *			-1, 其他原因导致升级失败
 *			-2, 找不到USB设备
 *			-3, 找不到文件
 *			-4, 无法连接FTP站点
 *			>0, 升级状态正常，表示进度
 * 修改日志：
 * 			2009-01-09,陶永亮,创建
 */
 
td_s32 mtn_BackupApp(void);
/*
 * 函数名称：BackupApp
 * 简要描述：备份当前内核程序
 * 输	  入：
 			无
 * 输	 出：
 * 			ERR_SYS_OPEN_FILE_ERROR, 打开文件失败
 *			ERR_SYS_READ_FILE_ERROR, 读文件失败
 *			ERR_SYS_WRITE_FILE_ERROR, 写文件失败
 *			ERR_SYS_SYSTEM_ERROR, 执行系统命令失败
 *			ERR_SYS_FAILED, 其他原因导致失败
 * 			ERR_SYS_SUCCESSFUL,操作成功
 * 修改日志：
 * 			2008-12-09,陶永亮,创建
 */


td_s32 mtn_UpdateTime(void);
/*
 * 函数名称：UpdateTime
 * 简要描述：更新系统时间
 * 输	  入：
 * 			无
 * 输	 出：
 * 			ERR_SYS_WRONG_PARAM,错误的参数
 * 			ERR_SYS_SUCCESSFUL,初始化成功
 * 修改日志：
 * 			2008-12-03,陶永亮,创建
 */
 
td_s32 mtn_SetTime(td_s32 _iYear, td_s32 _iMon, td_s32 _iDay, td_s32 _iHour, td_s32 _iMin, td_s32 _iSec);
/*
 * 函数名称：SetTime
 * 简要描述：设置系统时间
 * 输	  入：
 * 			6个整型参数分别为年、月、日、时、分、秒
 * 输	 出：
 * 			ERR_SYS_WRONG_PARAM,错误的参数
 * 			ERR_SYS_SUCCESSFUL,初始化成功
 * 修改日志：
 * 			2008-11-25,陶永亮,创建
 */
 
td_s32 mtn_GetTime(td_s32 * _piYear, td_s32 * _piMon, td_s32 * _piDay, td_s32 * _piHour, td_s32 * _piMin, td_s32 * _piSec);
/*
 * 函数名称：GetTime
 * 简要描述：获取系统时间
 * 输	  入：
 * 			6个整型指针分别用于传出年、月、日、时、分、秒
 * 输	 出：
 * 			ERR_SYS_WRONG_PARAM,错误的参数
 * 			ERR_SYS_SUCCESSFUL,初始化成功
 * 修改日志：
 * 			2008-11-25,陶永亮,创建
 */


void mtn_StartUI(void);
/*
 * 函数名称：StartUI
 * 简要描述：启动UI
 * 输	  入：
 			无
 * 输	 出：
 * 			无
 * 修改日志：
 * 			2008-12-18,陶永亮,创建
 */

td_s32	sys_Exit(td_u8 _cParam);
/*
 * 函数名称：Exit
 * 简要描述：去初始化系统管理模块
 * 输	  入：
 * 			_cParam, 退出系统的参数:bit0,是否保存配置文件；bit1,是否停止喂狗;bit2:是否将狗置为不使能
 * 输	 出：
 * 			ERR_SYS_WRONG_PARAM,错误的参数
 * 			ERR_SYS_SUCCESSFUL,初始化成功
 * 修改日志：
 * 			2008-11-25,陶永亮,创建
 */
 
void mtn_Reboot(int level,char* msg);
/*
 * 函数名称：Reboot
 * 简要描述：重启启动
 * 输	  入：
 			level, 0~6, 记录日志的级别
 			msg, 非空，记录日志的内容
 * 输	 出：
 * 			无
 * 修改日志：
 * 			2008-12-31,陶永亮,从NerServer.c移至System.c
 */
 
void mtn_ShutDown(td_s32 _iParam);

/*
 * 函数名称：ShutDown
 * 简要描述：关机，该函数只是执行程序的安全退出，并不真正的关闭设备
 * 输	  入：
 * 			无
 * 输	 出：
 * 			无
 * 修改日志：
 * 			2008-12-06,陶永亮,创建
 */

void mtn_Reset();
/*
 * 函数名称：Reset
 * 简要描述：复位，即恢复出厂设置
 * 输	  入：
 * 			无
 * 输	 出：
 * 			无
 * 修改日志：
 * 			2009-10-29,陶永亮,整理创建
 */

td_s32 mtn_Init(void);
/*
 * 函数名称：Init
 * 简要描述：初始化系统维护模块
 * 输	  入：
 * 			无
 * 输	 出：
 * 			ERR_SYS_SUCCESSFUL, 初始化成功
 *			ERR_SYS_FAILED,	初始化失败
 * 修改日志：
 * 			2010-02-02,陶永亮,整理创建
 */

td_s32 mtn_ValidConfigFileList(char _cFileList[][256], td_s32* _piFileNum);
/*
 * 函数名称：ValidConfigFileList
 * 简要描述：获取可用升级文件列表
 * 输	  入：
 * 			_cFileList, 用于传出文件列表
 *			_piFileNum, 用于传出文件总数
 * 输	 出：
 * 			ERR_SYS_WRONG_PARAM,错误的参数
 *			ERR_SYS_NO_USB, 无可用U盘
 *			ERR_SYS_NO_FILE, 无可用升级文件
 * 			ERR_SYS_SUCCESSFUL, 获取成功
 * 修改日志：
 * 			2010-04-13,陶永亮,创建
 */

td_s32 mtn_ImportConfigFile(td_u8* _pcSrcFile);
/*
 * 函数名称：ImportConfigFile
 * 简要描述：导入配置文件
 * 输	  入：
 * 			_pcSrcFile, 待导入的文件，非空
 * 输	 出：
 * 			ERR_SYS_WRONG_PARAM,错误的参数
 * 			ERR_SYS_SUCCESSFUL,导入成功
 * 修改日志：
 * 			2010-04-13,陶永亮,创建
 */

td_s32 mtn_ExportConfigFile(td_s32 _iDstDevice, td_s32 _iPartNo, td_u8* _pcDstFile);
/*
 * 函数名称：ExportConfigFile
 * 简要描述：导出配置文件
 * 输	  入：
 * 			_iDstDevice, 目标U盘编号， 一般为0
 *                 _iPartNo, 目标U盘分区号20120209杨珏加
 *			_pcDstFile, 目标文件名，如果为空将自动构造。
 * 输	 出：
 * 			ERR_SYS_WRONG_PARAM,错误的参数
 *			ERR_SYS_ALPHA_DIGIT_ONLY, 文件名中只允许有字母或数字
 * 			ERR_SYS_SUCCESSFUL,初始化成功
 *			ERR_SYS_NO_USB, 无可用U盘
 * 修改日志：
 * 			2010-04-13,陶永亮,创建
 *                 2012-02-09,杨珏,更改
 */

td_s32 mtn_RecoverSlaveSuit(td_s32 _iFlag);
td_s32 mtn_BackupSlaveSuit(td_s32 _iFlag);

td_s32 mtn_SetWorkMode(td_s32 _iWorkMode);
//获取工作模式
td_s32 mtn_GetWorkMode(void);
//备份原始文件
td_s32 mtn_BackupImage(void);




//int extractStream(td_u8* _buf,char* _pDstPath,int _iMv);

//*************************版本部分**************************//
const td_s8 *ver_Core();
const td_s8 *ver_Ocx();
const td_s8 *ver_Gui();
const td_s8 *ver_App();
const td_s8 *ver_Slave();
const td_s8 *ver_Sdk();
td_s32 ver_SetGui(const td_s8* _version);
td_s32 ver_SetSlave(const td_s8* _version);
td_s32 ver_setApp(const char *_version);
//设置单片机版本
td_s32 ver_SetSingleChip(const td_char *_pcSingleChip);
//获取单片机版本
const char *ver_GetSingleChip();


//for decoder
int Dec_UpgradeLogo(int _len, char* _buf);

//数字通道心跳
void sys_ChnHeart(unsigned long _ulType);
void sys_TstChnHeart(unsigned long _ulType);
void sys_AutoMaintain(unsigned long _param);//by zhangsheng

void mtn_SetDBStatFlg(td_s32 _iStat);
void mtn_SetRebldProgress(int _iProgress);
void mtn_CheckDBStat(unsigned long _param);
long long CombinDataCheckSum(td_u8 *_pcData, int _iSize);

td_s32 CombineFiles(td_u8* _cFileList[64], td_u8* _pcDstFile,int _iOffset);

#endif /*_MAINTENANCE_H_*/

