/*******************************************************************
文件名     : smartctl.h
公司          : Tianjin Tiandy Tech.
创建者     : 杨珏
创建日期: 2011-12-5
简要描述: smart主程序
修改记录: 2011-12-5 yangjue Create this File
******************************************************************/

#ifndef SMART_H
#define SMART_H

#define SMART_NAME_LEN  32		// 信息名最大长度
#define SMART_VALUE_NUM 6		// 每条信息包含信息的种类
#define SMART_VALUE_LEN 16		// 每种信息的信息长度

// 实际使用的返回值也会根据情况进行进一步的更改
// 函数返回值
#define SMART_SUCCESS		0		// 程序执行成功
#define SMART_OPEN			0		// 设备开启
#define SMART_NOTSUPPORT	-1
#define SMART_CLOSE			-2
#define SMART_READERROR		-3
#define SMART_ARGERROR		-4
#define SMART_OPENERROR		-5
#define SMART_SETERROR		-6
#define SMART_AUTOTIMEERR	-7
#define SMART_AUTOSAVEERR	-8

// 信息索引号
#define SMART_MSGIDX_FLAG	0
#define SMART_MSGIDX_VALUE	1
#define SMART_MSGIDX_WORST	2
#define SMART_MSGIDX_THRE	3
#define SMART_MSGIDX_STATUS	4
#define SMART_MSGIDX_RAW	5

// smart信息
typedef struct
{
	int  m_iSmartID;  //HEX 值
	char m_cSmartName[SMART_NAME_LEN];    //smart 属性名称
	int  m_iSmartFlag;   //标识
	int  m_iSmartValue;
	int  m_iSmartWorst;
	int  m_iSmartThresh;
	char m_cSmartStatus[SMART_VALUE_LEN]; //硬盘状态
	int  m_iResInt;      // 字节对齐添加
	long long m_llSmartRawValue;
} TSmartMsg;

// smart基本信息
typedef struct
{
	long long m_llCapacity;  //硬盘容量
	char m_cDevModel[SMART_NAME_LEN];
	char m_cSerialNum[SMART_VALUE_LEN];
	int m_iTemp; //温度
	int m_iUsedTime;// 按天计算
	int m_iHealth; // 1-通过 0-不通过
	int m_iAllHealth; // 1-健康状况良好
}TSmartBasicMsg;


/******************************************************************************
	函数名称: smart_GetDiskMsg
	简要描述: 读取硬盘信息
	输入          : _strDiskDev-------硬盘设备名
	                      _ptMsg-----------信息内容
	                      _iMaxCnt----------所需读取的最大数量
	输出          : 返回实际读出的信息数量
	                      负值表示读取出错，具体错误详见错误函数
	修改日志: 2011-12-05 杨珏创建该函数
******************************************************************************/
int smart_GetDiskMsg(char *_strDiskDev, TSmartMsg *_ptMsg, int _iMaxCnt);


/******************************************************************************
	函数名称: smart_SetStatus
	简要描述: 变更硬盘状态
	输入          : _strDiskDev-------硬盘设备名
	                      _iStatus----------smart开启状态
	                                                 SMART_OPEN为开启SMART_CLOSE为关闭
	输出          : 0 代表smart设置成功
	                      负值表示读取出错，具体错误详见错误函数
	修改日志: 2011-12-05 杨珏创建该函数
******************************************************************************/
int smart_SetStatus(char *_strDiskDev, int _iStatus);


/******************************************************************************
	函数名称: smart_GetDiskBasicMsg
	简要描述: 读取硬盘基本信息
	输入          : _strDiskDev-------硬盘设备名
	                      _ptMsg-----------信息内容
	输出          : 0表示读取正确
	                      负值表示读取出错，具体错误详见错误函数
	修改日志: 2012-01-18 杨珏创建该函数
******************************************************************************/
int smart_GetDiskBasicMsg(char *_strDiskDev, TSmartBasicMsg *_ptMsg);



#endif

/***********************end of the file***************************/

