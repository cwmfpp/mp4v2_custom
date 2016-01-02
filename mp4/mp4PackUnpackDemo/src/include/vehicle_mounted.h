/***********************************************************************
文件名	:vehicle_mounted.h
公司	:天津天地伟业数码科技有限公司
创建者	:
创建日期:2013-8-21
简要描述:车载模块，主要实现以下功能
         1、初始化，初始化3g、GPS、及低电压报警低电压关机的相关功能。
         2、主要实现以下功能
            电源管理、温度上报、端口报警联动打点、GPS管理、3G管理。
         3、单片机版本考虑增加在ver.c文件中，不在此处实现
            延时关机及延时开机功能，内核仅实现保存配置功能，也不在此实现
         4、录像文件ftp上传及录像文件导出功能
*************************************************************************/

#ifndef VEHICLE__MOUNTED_H
#define VEHICLE__MOUNTED_H

#include "public_def.h"

#define MAX_PHONE_COUNT		5

typedef int  (*SEND_MSG_TO_ALL_CLIENT)(char* _cMsg,int _iLen,int _iType);
typedef int  (*SEND_COMDATA_TO_ALL_CLIENT)(td_u8* _u8Data,td_s32 _iLen,td_s32 _iComNo);

typedef struct 
{
	int m_iOpenEnable;		//开机延时使能
	int m_iOpenDevDelayTime;	//开机延时时间
	int m_iCloseEnable;		//关机延时使能
	int m_iCloseDevDelayTime;       //关机延时时间
}TPowerManageMsg;//电源管理信息结构

typedef struct 
{
	int m_iPoweroffVoltage;	 //关机低压值
	int m_iAlarmVoltage;	 //低压报警值
}TPowerAlarmMsg;//报警关机电压值结构



td_s32 Vehcl_Init(SEND_MSG_TO_ALL_CLIENT _SendMsgToAllClient);
/*
 * 函数名称：Vehcl_Init
 * 简要描述: 车载模块初始化
 *           依次调用Vehcl_PowerManagerInit,Vehcl_GPSInit,Vehcl_3GInit函数
 *           分别进行电压检测,GPS,3G的初始化操作             
 * 输	  入：SEND_MSG_TO_ALL_CLIENT 用于向客户端广播消息的回调函数
 * 输	  出：无
 */


td_s32 Vehcl_DetectVoltage(void);
/*
 * 函数名称：Vehcl_DetectVoltage
 * 简要描述: 用于向异常检测模块注册的函数           
             当检测到电源电压低于设定的报警电压值时，触发异常报警
 * 输	  入：无
 * 输	  出：0		无异常
 			  >0	电压异常
 */

td_s32 Vehcl_GetVoltageTemperature();
/*
 * 函数名称：Vehcl_GetVoltageTemperature
 * 简要描述: 获取电压和温度值,用于客户端登录时发送登录信息
 * 输	  入：无
 * 输	  出：电压和温度值(高16位温度值,低16位电压值)
 */

#if 0
//GPS管理
td_s32 Vehcl_SetGPSTimeEnable(int _iEnable);
/*
 * 函数名称：Vehcl_SetGPSTimeEnable
 * 简要描述: 设置使能GPS校时
 * 输	  入：_iEnable	1	使能
 * 						0   不使能
 * 输	  出：td_success	成功
 *			  td_failuer	失败
 */
#endif

td_s32 Vehcl_SetGPSMsgChnOverlay(int _iChn,int _iEnable);
/*
 * 函数名称：Vehcl_SetGPSMsgChnOverlay
 * 简要描述: 设置GPS信息通道叠加使能
 * 输	  入：_iChn		通道号
 			  _iEnable	1	使能
 * 						0   不使能
 * 输	  出：td_success	成功
 *			  td_failuer	失败
 */
 
td_s32 Vehcl_GetGPSModuleType();
/*
 * 函数名称：Vehcl_GetGPSModuleType
 * 简要描述: 获取GPS模块类型，GPS或北斗
 * 输	  入：无
 * 输	  出：模块类型
 */


td_s32 Vehcl_GetGPSInfor(td_s32 *_iSateNum, td_s32 *_iIntensity);
/*
 * 函数名称：Vehcl_GetGPSInfor
 * 简要描述: 客户端登录时获取需要发送的GPS信息,包括卫星数(低16位)和信号强度(高16位)
 * 输	  入：
 * 输	  出：
 */

td_s32 Vehcl_SetGPSFilter(td_s32 _iFilter);
/*
 * 函数名称：Vehcl_SetGPSFilter
 * 简要描述:设置GPS信息过滤 
 * 输	  入：无
 * 输	  出：无
 */

td_s32 Vehcl_PortAlarmLinkVideoMark();
/*
 * 函数名称：	Vehcl_PortAlarmLinkVideoMark
 * 简要描述: 	端口报警联动打点
 * 				在DvrAlarmStatChgNotify函数中调用
 *              如果车载模块没有初始化则直接返回
 *				如果车载模块已初始化则进行打点操作
 * 输	  入：	无
 * 输	  出：	td_success	成功
 *			  	td_failure	失败
 */

td_s32 Vehcl_SetPowerAlarmMsg(td_s32 _iAlarm);
/*
 * 函数名称：	Vehcl_SetPowerAlarmMsg
 * 简要描述:	设置报警电压值
 * 输	  入：	
 * 输	  出：	td_success	成功
 *				
 */

td_s32 Vehcl_SetPowerOffMsg(td_s32 _iShutDown);
/*
 * 函数名称：	Vehcl_SetPowerOffMsg
 * 简要描述:	设置关机电压值
 * 输	  入：	
 * 输	  出：	td_success	成功
 *				
 */


td_s32 Vehcl_UsbEsataState(td_s32 _iDiskNo);
/*
 * 函数名称：	Vehcl_UsbEsataState
 * 简要描述:		收到磁盘管理模块通知的有新USB或Esata插入消息并作记录
 * 输	  入：	
 * 输	  出：	td_success	成功
 *			  	
 */
td_s32 Vehcl_Set_WifiFtp_AutoEn(td_s32 _iEnable);

td_s32 Vehcl_Get_WifiFtp_AutoEn();

td_s32 Vehcl_Set_WifiFtp_Manual();
td_s32 Vehcl_IsInited();

td_s32 Vehcl_GetVoltage();

td_s32 Vehcl_GetTemperature();

td_s32 	Vehcl_SendComDataCallBack(SEND_COMDATA_TO_ALL_CLIENT _SendComDataCallBack);


#endif


