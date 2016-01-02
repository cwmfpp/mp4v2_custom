#ifndef _ALARMER_CNF_H_
#define _ALARMER_CNF_H_

#include "alarmer.h"
#include "td_types.h"

//网络报警主机信息
typedef struct
{
	td_s32 m_iDevNo;						//主机编号
	td_s32 m_iIsUsed;						//启用
	char m_strAddress[32];	//ip地址
	td_s32 m_iPort;							//端口号

	td_s32 m_iAlarmInNum;	   	 			//报警输入数
	td_s32 m_iAlarmOutNum;   	 			//报警输出数
}TNetAlarmerCnf;

//串口报警主机信息
typedef struct
{
	td_s32 m_iDevNo;					//主机编号
	td_s32 m_iIsUsed;		   	 		//是否启用
	td_s32 m_iSerialNo;	     		//串口号
	td_s32 m_iProtocoType;	     		//协议类型
	td_s32 m_iAddress;					//地址(0~255)
	td_s32 m_iAlarmInNum;	   	 		//报警输入数
	td_s32 m_iAlarmOutNum;   	 		//报警输出数
}TSerialarmerCnf;

//读取配置文件
td_s32 alarmhost_GetNetAlarmerCnf(td_s32 _iAhDevNo, TNetAlarmerCnf *_ptCnf);
//写配置文件
td_s32 alarmhost_SetNetAlarmer(td_s32 _iAhDevNo, TNetAlarmerCnf *_ptCnf);
//读取串口报警主机配置
td_s32 alarmhost_GetSerialAlarmerCnf(td_s32 _iAhDevNo, TSerialarmerCnf *_ptCnf);
//写配置文件
td_s32 alarmhost_SetSerialAlarmer(td_s32 _iAhDevNo, TSerialarmerCnf *_ptCnf);
//创建报警主机报警输入数
td_s32 alarmhost_CreateAlarmInPort(TAlarmer *_ptAlarms[], td_s32 _iMaxCnt);
//创建报警主机报警输出数
td_s32 alarmhost_CreateAlarmOutPort(TAlarmOutPort *_ptAlarmOutPort[], td_s32 _iMaxCnt);

td_s32 alarmhost_Init(TComInstance *_ptComIns, void (*ComDataProc)(td_s32, td_s32, td_u8 *));

//恢复报警主机设置
td_s32 alarmerHost_DefaultCfg(void);

int alarmhost_GetOutPortAlamerAttr(int _iportNo, char * _strMsg);



#endif

