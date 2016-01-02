/*******************************************************************
文件名	:exception.h
公司	:天津天地伟业数码科技有限公司
创建者	:陶永亮
创建日期:2009-10-28
简要描述:异常处理模块宏定义、数据结构定义及接口函数原型
修改记录:2009-10-28,陶永亮，创建
        
******************************************************************/

#ifndef _EXCEPTION_H_
#define _EXCEPTION_H_

#define EXCEPTION_TYPES		10

//异常状态
typedef enum
{
	ESTATE_HAPPEN = 0,							//产生异常
	ESTATE_DISAPPEAR = 1,							//异常消失
}TEstate;


//异常类型
typedef enum
{
	EXP_DISK_FULL = 0,							//磁盘满 陶永亮 091127 1->0
	EXP_NO_DISK = 1,							//无磁盘 陶永亮 091127 new
	EXP_DISK_IOERROR = 2,						//磁盘读写错误 陶永亮 091127 3->2
	EXP_ACCESS = 3,								//非法访问 陶永亮 091127 4->3
	EXP_IP_CONFLICT = 4,						//IP冲突 陶永亮 091127 5->4
	EXP_ETHERNET = 5,							//网络中断 陶永亮 091127 6->5
	EXP_SYSTEMBUSY = 6,                         //系统忙
	EXP_NOREBUNDANCE_DISK = 7,                  //无冗余盘 孙伟 20130327
	EXP_LOWPOWERVOLTAGE = 8,					//电源电压低dce20130823
	EXP_MAC_CONFLICT = 9,                       //MAC冲突 庄志兵 20131024
}TException;


typedef void (*ExcptionReprot)(int _iStat, int _iWho);


td_s32 excp_SetCheckFunction(td_s32 _iId, td_s32 (*_CheckFun)(void));

td_s32 excp_SetEnable(td_s32 _iType, td_s32 _blEnable);
/*
 * 函数名称：excp_SetEnable
 * 简要描述：设置异常使能
 * 输	  入：
 * 			_iType, 异常类型，TException
 * 			_blEnable,使能，0或1
 * 输	 出：
 * 			ERR_EXCP_SUCCESSFUL,成功
 *			ERR_EXCP_WRONG_PARAM,错误的参数
 *			ERR_EXCP_WRITE_CONFIG,写配置文件失败
 * 修改日志：
 * 			2009-10-29,陶永亮,创建
 */
 
td_s32 excp_GetEnable(td_s32 _iType);
/*
 * 函数名称：excp_GetEnable
 * 简要描述：获取异常使能
 * 输	  入：
 * 			_iType, 异常类型，TException
 * 输	 出：
 * 			TD_DISABLE,不使能
 *			TD_ENABLE,不使能
 *			ERR_EXCP_WRONG_PARAM,错误的参数
 * 修改日志：
 * 			2009-10-29,陶永亮,创建
 */
 
td_s32 excp_SetParam(td_s32 _iType, td_s32 _blHint, td_s32 _blBuzzer, td_s32 _blNotify);
/*
 * 函数名称：excp_SetParam
 * 简要描述：设置异常处理参数
 * 输	  入：
 * 			_iType, 异常类型，TException
 *			_blHint,用户界面对话框提示，TD_ENABLE/TD_DISABLE
 *			_blBuzzer,蜂鸣器提示，TD_ENABLE/TD_DISABLE
 *			_blNotify,通知网络客户端，TD_ENABLE/TD_DISABLE
 * 输	 出：
 * 			ERR_EXCP_SUCCESSFUL,成功
 *			ERR_EXCP_WRONG_PARAM,错误的参数
 *			ERR_EXCP_WRITE_CONFIG,写配置文件失败
 * 修改日志：
 * 			2009-10-29,陶永亮,创建
 */
 
td_s32 excp_GetParam(td_s32 _iType, td_s32* _blHint, td_s32* _blBuzzer, td_s32* _blNotify);
/*
 * 函数名称：excp_GetParam
 * 简要描述：获取异常处理参数
 * 输	  入：
 * 			_iType, 异常类型，TException
 *			_blHint,传出用户界面对话框提示，非空
 *			_blBuzzer,传出蜂鸣器提示，非空
 *			_blNotify,传出通知网络客户端，非空
 * 输	 出：
 * 			ERR_EXCP_SUCCESSFUL,成功
 *			ERR_EXCP_WRONG_PARAM,错误的参数
 * 修改日志：
 * 			2009-10-29,陶永亮,创建
 */
 
//获取异常处理状态
td_s32 excp_GetStat(td_s32 _iType);
td_s32 excp_SetStat(td_s32 _iType, td_s32 _iState);


//td_s32 excp_Init(void);
td_s32 excp_Init(ExcptionReprot _ReportUI, ExcptionReprot _ReportNet, ExcptionReprot _ReportUIBuzzer);

/*
 * 函数名称：excp_Init
 * 简要描述：异常处理模块初始化
 * 输	  入：
 * 			无
 * 输	 出：
 * 			ERR_EXCP_SUCCESSFUL,成功
 *			ERR_EXCP_READ_CONFIG, 读取配置文件失败
 *			ERR_EXCP_CREATE_THREAD, 创建线程失败
 * 修改日志：
 * 			2009-10-29,陶永亮,创建
 */

td_s32 excp_Exit(void);
/*
 * 函数名称：excp_Exit
 * 简要描述：异常处理模块退出
 * 输	  入：
 * 			无
 * 输	 出：
 * 			ERR_EXCP_SUCCESSFUL,成功
 *			ERR_EXCP_FAILED，失败
 * 修改日志：
 * 			2009-10-29,陶永亮,创建
 */
 
#endif /*_EXCEPTION_H_*/

