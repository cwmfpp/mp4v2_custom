#ifndef _GUI_MSG_H_
#define _GUI_MSG_H_

#include "guitransport.h"

/***********************Alarm Message Begin************************************/
//通知UI某个通道的报警状态
#define msg_TellUIAlarmState(ch, state)			g_ptDvrAlarmStaForGui->m_iAlarmSta[ch] = state

//通知UI设备当前是否为报警状态
#define msg_TellUIIfAlarming(state)				g_ptDvrAlarmStaForGui->m_iAlarmLed = state

//判断某个通道的状态是否为。。。
#define msg_IfAlarmState(ch, state)				(g_ptDvrAlarmStaForGui->m_iAlarmSta[ch] == state)
/***********************Alarm Message End**************************************/

/***********************Storage Message Begin**********************************/
//通知UI录像状态
#define msg_TellUIRecordStatus(ch, state)		g_ptDvrVideoStaForGui->m_iVideoSta[ch] = state

//从UI获取某个通道的状态
#define msg_GetUIRecordStatus(ch)				g_ptDvrVideoStaForGui->m_iVideoSta[ch]

/***********************Storage Message End************************************/

/***********************Public Message Begin***********************************/
//触发蜂鸣器
#define msg_Buzzer(enable)				g_ptDvrAlarmStaForGui->m_iAlarmSound = enable

//通知UI目前的运行状态
#define msg_TellUIStatus(status, param)	g_ptGuiSysStatus->m_iSysSta = status;\
										g_ptGuiSysStatus->m_iParams[0] = param
//重启/关机命令
#define GUI_GET_NET_REBOOT_CMD 			20120224
	
//获取UI目前的运行状态参数
#define msg_GetUIStatusParam()			g_ptGuiSysStatus->m_iParams[0]
//通知UI磁盘编号
#define msg_TellUIDiskNo(diskno)		g_ptGuiSysStatus->m_iParams[1] = diskno

//通知UI平台初始化完毕
#define msg_TellUIPlatformInit(status)		g_ptGuiSysStatus->m_iParams[2] = status

//向GUI报告异常情况
#define msg_ReportException(exception)	g_ptGuiSysStatus->m_iSysErr = exception

//向GUI报告错误码
#define msg_ReportError(errorno)				SetDvrErrNo(errorno)   //yp2009

//通知预览状态(VO)
#define msg_ReportPreviewStat(s)		g_ptGuiSysStatus->m_iSysErrMsg[GUI_SYS_ERR_PCNT - 1] = s
#define msg_GetPrePreviewStat()		(g_ptGuiSysStatus->m_iSysErrMsg[GUI_SYS_ERR_PCNT - 1])

//通知UI开关机延时时长
#define msg_ReportPowerDelay(ontime, offtime) g_ptGuiDevStatShrMsg->m_sOnDelayTm = ontime;\
											  g_ptGuiDevStatShrMsg->m_sOffDelayTm = offtime
/***********************Public Message End*************************************/

#endif /*_GUI_MSG_H_*/

