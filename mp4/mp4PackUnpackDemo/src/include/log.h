/*******************************************************************
文件名	  :Log.h
公司	        :TIANDY
创建者	  :YP
创建日期:2009-09-01
简要描述:日志数据结构定义及相关函数声明
修改记录:
        
******************************************************************/

#ifndef LOG_H_
#define LOG_H_

#include "log_template.h"

#define LOG_ONE_SIZE			100  //日志每条纪录的长度，单位Byte
#define LOG_MIN_FILE_SIZE		1000   //日志文件的最小大小，单位KByte
#define LOG_MAX_FILE_SIZE		16 * 1000 //日志文件的最大大小，单位KByte
#define LOG_FLASH_FILE_SIZE		100

#define MAX_LOG_PAGE_SIZE		20	//每页最多显示的日志条数
#define MAX_LOG_TEMPLATE_LEN	100 //日志中英文模板每行的最大长度
#define MAX_LOG_MSG_LEN			128	//日志内容的最大长度
#define MAX_LOG_WRITE_PARA_STRING_TYPE_SIZE 30 //记录日志输入字符串参数类型的最大长度
#define MAX_LOG_LEN			(MAX_LOG_MSG_LEN - 1)		//最大日志内容长度
#define MAX_LENGTH_USER_NAME 16
//#define LANG_EN					0 		//查询英文模板
//#define LANG_CH					1		 //查询中文模板
typedef int (*GetUseInfo)(int _socket, char *_pcstrUserName, int _iMaxUserNameLen);

typedef enum
{
	LANG_EN = 0,     //查询英文模板
	LANG_CH,         //查询中文模板
	LANG_TCH,        //查询繁体中文模板
	LANG_KOR,        //查询韩文模板
	LANG_SPAN,       //查询西班牙文模板
	LANG_ITA,        //查询意大利模板
	LANG_ALL,
}TLogLangType;

#define CHANNEL_ALL				0xFF     //查询所有通道
#define CHANNEL_INVALID			0xF0     //与通道无关
#define LOG_DEL_FLAG			0xFFFF   //日志记录已经删除标志

#define LOG_LEVEL_NUM       	7  		 //日志级别总数
#define LOG_LEVEL_MIN			0		 //日志最小级别
#define LOG_LEVEL_ALL			0xFF	 //查询所有级别
#define LOG_LEVEL_SET_ALL		((0x1 << LOG_LEVEL_NUM) - 1)	//日志设置允许全级别
#define TAB_STR					"\t"	 //分隔符字符串
#define TAB_CHAR				'\t'	 //分隔符字符

typedef struct		//日志文件全局信息结构
{
	td_u32	m_cLevel;		//存储允许纪录的日志的级别
	td_u32 	m_usFileSize;	//上层设置的  存储日志文件的大小，单位k
	td_u32 	m_usIndex;		//用于保存日志的索引
	td_bool	m_bCovered;     //日志文件循环覆盖标记
	td_u8 * m_uLogFileName;	//保存日志记录文件名
} gen_TLogInfo;
#define LOGINFO_DEFAULT \
	(gen_TLogInfo){LOG_LEVEL_SET_ALL, LOG_MAX_FILE_SIZE, 0, td_false}

typedef struct				//日志每条记录的头结构，帮助查询
{
	td_u32	m_usIndex; 		//索引
	time_t	m_iTime;   		//记录的时间
	td_u32	m_iChannel;		//通道号
	td_u32	m_cLevel;  		//级别
	td_u32	m_uiTemInd;		//模板号
	td_u8	m_UserName[MAX_LENGTH_USER_NAME + 1];
} gen_TLogHead;

#define LOG_HEAD_SIZE (sizeof(gen_TLogHead))
#define MSG_SIZE (LOG_ONE_SIZE - LOG_HEAD_SIZE)		  //每条日志参数串大小

typedef struct				//写日志参数结构体
{
	gen_TLogHead m_gLogHead;
	td_u8	m_uMsgParaStr[MSG_SIZE]; //参数集
} gen_TLog;


typedef struct				//日志信息
{
	gen_TLogHead m_gLogHead;
	td_u8	m_cContent[MAX_LOG_MSG_LEN]; //日志内容
}TLogInfo;

typedef struct				//查询日志信息的结构体
{
	td_s32	m_blLang;		//输出中英文
	td_s32	m_iPageNo;		//页数
	td_s32	m_iPageSize;	//每页的大小
	td_s32	m_iTotalNum;	//总查询条数
	td_s32	m_iShowNum;		//本次查询的条数
	td_u32	m_iLevel;		//日志类型
	td_u32	m_iChannel;		//日志关联通道号
	time_t	m_iStartTime;	//起始查询时间
	time_t	m_iStopTime;	//结束查询时间
	TLogInfo m_stInfo[MAX_LOG_PAGE_SIZE]; //查询结果	
}TLogQuery;
#define QUERY_START_TIME_DEFAULT	0			//默认查询开始时间
#define QUERY_STOP_TIME_DEFAULT		1352240666	//默认查询结束时间
#define LOGQUERY_DEFAULT (TLogQuery){LANG_EN,0,MAX_LOG_PAGE_SIZE,0,0,\
	LOG_LEVEL_ALL,CHANNEL_ALL,QUERY_START_TIME_DEFAULT,QUERY_STOP_TIME_DEFAULT}

typedef enum		//日志类型
{
	LOG_LEVEL_SYSTEM = 0,//系统，如重启、关机、升级文件,发生严重错误等
	LOG_LEVEL_WARNING,	 //警告，发生异常时警告，比如磁盘满，网络中断等
	LOG_LEVEL_ALARM,	 //报警，主要指视频丢失、移动侦测、端口报警等
	LOG_LEVEL_OPERATION, //操作，修改设置，连接视频，录像，对讲或进行其他正常操作
	LOG_LEVEL_USER,		 //用户，包括用户登录登出，添加删除，修改密码等
	LOG_LEVEL_OTHER,	 //其他，不包含于以下内容中的其他类型
	LOG_LEVEL_DEBUG,	//调试日志
}TLogLevel;

td_s32 log_Get(td_u32 * _piMinLevel, td_u32 * _piFileSize);
/*
 * 函数名称：log_Get
 * 简要描述: 获取日志模块的相关参数
 * 输	  入：
 * 			_piMinLevel, 用于传出日志显示级别
 *			_piFileSize, 用于传出日志文件的大小，单位为K
 * 输	  出：
 * 			ERR_LOG_WRONG_PARAM,错误的参数
 * 			ERR_LOG_SUCCESSFUL,操作成功
 * 			ERR_LOG_FAILED,其他原因导致操作失败
 * 修改日志：
 * 			2009-09-01
 */
td_s32 log_Set(td_u32 _iMinLevel, td_u32 _iFileSize);
/*
 * 函数名称：log_Set
 * 简要描述: 设置日志模块的相关参数
 * 输	  入：
 * 			_iMinLevel, 日志显示级别
 *			_iFileSize, 日志文件的大小，单位为K
 * 输	  出：
 * 			ERR_LOG_WRONG_PARAM,错误的参数
 * 			ERR_LOG_SUCCESSFUL,操作成功
 * 			ERR_LOG_FAILED,其他原因导致操作失败
 * 修改日志：
 * 			2009-09-01
 */
#if 0
td_s32 log_Write(td_u32 _cLevel, td_u32 _iChannelNo, td_u32 _temID, td_u8* _pcMsg, ...);
/*
 * 函数名称：log_Write
 * 简要描述: 向日志文件写入一条日志
 * 输	  入：
 * 			_iLevel, 该条日志的级别或曰类型
 *			_iChannelNo,该日志相关的通道
 *			_temID,该日志的模板号
 *			_pcMsg, 日志内容参数类型串
 *			..., 日志内容参数集
 *			注意:参数类型串+参数集最大为MSG_SIZE(47Bytes)大小
 * 输	  出：
 * 			ERR_LOG_WRONG_PARAM,错误的参数
 * 			ERR_LOG_SUCCESSFUL,操作成功
 * 			ERR_LOG_FAILED,其他原因导致操作失败
 * 修改日志：
 * 			2009-09-01
 */
 #endif
td_s32 log_Query(TLogQuery* _stQ);
/*
 * 函数名称：Query
 * 简要描述: 查询日志, 根据类型、通道、起至时间、页号来查询
 * 输	  入：
 * 			_stQ, 非空
 * 输	  出：
 * 			ERR_LOG_WRONG_PARAM,错误的参数
 * 			ERR_LOG_SUCCESSFUL,操作成功
 * 			ERR_LOG_FAILED,其他原因导致操作失败
 * 修改日志：
 * 			2009-09-01
 */
td_s32 log_QueryEx(TLogQuery* _stQ);
td_s32 log_Clear();
/*
 * 函数名称：Clear
 * 简要描述: 清空日志, 使之为0
 * 输	  入：
 * 			无
 * 输	  出：
 * 			ERR_LOG_WRONG_PARAM,错误的参数
 * 			ERR_LOG_SUCCESSFUL,操作成功
 * 			ERR_LOG_FAILED,其他原因导致操作失败
 * 修改日志：
 * 			2009-09-01
 */

td_s32 log_Init(GetUseInfo _fun);
/*
 * 函数名称：Init
 * 简要描述: 日志模块全局变量初始化, 打开文件，申请内存
 * 输	  入：
 * 			无
 * 输	  出：
 * 			ERR_LOG_WRONG_PARAM,错误的参数
 * 			ERR_LOG_SUCCESSFUL,操作成功
 * 			ERR_LOG_FAILED,其他原因导致操作失败
 * 修改日志：
 * 			2009-09-01
 */

td_s32 log_Exit();
/*
 * 函数名称：Exit
 * 简要描述: 日志模块的去初始化, 关闭文件，释放内存
 * 输	  入：
 * 			无
 * 输	  出：
 * 			ERR_LOG_WRONG_PARAM,错误的参数
 * 			ERR_LOG_SUCCESSFUL,操作成功
 * 			ERR_LOG_FAILED,其他原因导致操作失败
 * 修改日志：
 * 			2009-09-01
 */


int log_fflushCacheMem(unsigned long _param);

td_s32 log_Write(td_u32 arg, td_u32 _cLevel, td_u32 _iChannelNo, td_u32 _temID, td_u8* _pcMsg, ...);
//日志切换存储路径,暂时保留
td_s32 log_ChangePath(td_s32 _iDiskNo, td_s32 _iFormatStat);
td_s32 log_Pause(td_s32 _iDiskNo);
td_s32 log_Restart(td_s32 _iDiskNo);




#endif
