#ifndef ATM_PROTOCOL_H_
#define ATM_PROTOCOL_H_

#include "td_types.h"
#include "public_def.h"
#include "libstorage.h"
#include "alarm.h"

#define ATM_MAX_RECV_MSG_LEN 1024
#define ATM_MAX_IPADDR_LEN	 20	



#define DOMAINDATA_NUM 5
#define STRBUF_LEN     33
#define DOMAINDATA_LEN 65

//ATM交易类型
typedef enum
{
	TC_QUERY			= 0,	//	查询
	TC_GETMONEY,				//	取款
	TC_CHANGEPASSWORD,			//	改密
	TC_TRANSFER,				//	转账
	TC_DEPOSIT,					//	存款
	TC_QUERYNOCARD,				//	无卡查询
	TC_DEPOSITNOCARD,			//	无卡存款
	TC_OTHER,					//	其他
	TC_INSERT_CARD,				//  插卡
	TC_REMOVE_CARD,				//	拔卡		
	TC_NULL = 0xFE,			
}TAtmTransaction;

//ATM异常类型
typedef enum
{
	EC_SWALLOW_MONEY			= 0,	//	吞钞
	EC_SWALLOW_CARD,					//	吞卡
	EC_NULL = 0xFE,
}TAtmException;

//ATM命令类型
typedef enum
{
	ATM_CMD_SYNC		= 2,	//同步
	ATM_CMD_INSERT_CARD = 8,		//插卡
	ATM_CMD_REMOVE_CARD = 9,		//拔卡
	ATM_CMD_TRANSACTION = 0x0A,		//交易
	ATM_CMD_EXCEPTION 	= 0x0B,			//异常
	ATM_CMD_SET_TIME	= 0x0C,			//校时
	ATM_CMD_DOMAIN_INFO,		//域信息
}TAtmCmd;

//ATM 协议类型
typedef enum
{
	ATM_PROTOCOL_COM = 0,		//串口通讯
	ATM_PROTOCOL_NET,			//网络通讯
}TAtmProtocolType;


typedef struct
{
	unsigned char 	m_uc8ProtocolName[33];		//atm协议名称
	unsigned int 	m_s32Type;					//协议类型 串口协议或者网络协议
	unsigned long 	m_ulSrcIp;					//源地址 ATM
	unsigned long	m_ulSrcPort;				//源端口	
	unsigned long	m_ulDestIp;					//目的地址 DVR
	unsigned long	m_ulDestPort;				//目的端口
	TComInstance* 	m_ptComIns;					//串口实例
}TAtmProtocolParam;

typedef struct
{
	unsigned char 		m_ucCardNo[DOMAINDATA_LEN];
	TAtmTransaction		m_eTransaction;
	TAtmException		m_eException;
	unsigned int 		m_iCash;	
	unsigned int 		m_iTime;
	unsigned char 		m_ucUseInfo[DOMAINDATA_LEN];
}TAtmTransactionMsg;

typedef struct 
{
	unsigned int	m_iYear;	//年	
	unsigned int	m_iMonth;	//月
	unsigned int	m_iDay;		//日
	unsigned int	m_iHour;	//时
	unsigned int	m_iMinute;	//分
	unsigned int	m_iSecond;	//秒
}TAtmSetTime;
typedef struct
{
    //SUN Wei debug for 12725 
    unsigned int 	m_iTime;
	unsigned char 	m_ucDomian1[DOMAINDATA_LEN];
	unsigned char 	m_ucDomian2[DOMAINDATA_LEN];
	unsigned char 	m_ucDomian3[DOMAINDATA_LEN];
	unsigned char 	m_ucDomian4[DOMAINDATA_LEN];
	unsigned char 	m_ucDomian5[DOMAINDATA_LEN];
}TAtmDomainMsg;

typedef td_u8 (*ATMPROTOCOL_RECVMSG)(TAtmProtocolType _eCmd,void* _pCmdMsg);
/*
	用于处理ATM事件的回调函数
	_u8CardNo: 		卡号
	_s32Operate:	操作，插卡/拔卡/查询/取款等
	_s32Cash:		金额
	_s32Event:		异常事件，吞卡吞钞
*/


typedef td_u8 (*ATMPROTOCOL_OSD)(TAtmProtocolType _eCmd,void* _pCmdMsg);
/*
	用于处理ATMOSD的回调函数
*/	

//td_s32 atmprotocol_Init(TAtmProtocolParam* _pProtocolParam,ATMPROTOCOL_RECVMSG _AtmRecvMsg);

/*
	功能:	去初始化atm协议解析模块
	参数:	无
	返回:	0		成功
			其他	失败	

*/
//td_s32 atmprotocol_DeInit(void);

/*
	功能:	通知ATM机别名改变
	参数:	_eType		别名类型
			_iChnNo		通道号
			_cAlias		别名
	返回值: 0		成功
			其他	失败
*/
//td_s32 atmprotocol_AliasChangeNotify(TAliasType _eType,int _iChnNo,char* _cAlias);


/*
	功能:	通知ATM机录像状态改变
	参数:	_iChnNo		通道号
			_eState		录像状态
	返回值: 0		成功
			其他	失败
*/
//td_s32 atmprotocol_RecordStateNotify(int _iChnNo,TRecordType _eRecordState);



/*
	功能:	通知ATM机报警状态改变
	参数:	_eType		报警类型
			_iAlarmNo	报警通道号或报警端口号
			_eState		报警状态，报警或消警	
	返回值: 0		成功
			其他	失败
*/
//td_s32 atmprotocol_AlarmStateNotify(TAlarmType _s32Type,int _iAlarmNo,TAlarmState _eState);


/*
	功能:	通知ATM通道数报警输出端口数和报警输入端口数
	参数:	_iChnNo		通道数
			_iAlarmInputNo	报警输出端口数
			_iAlarmOutputNo		报警输入端口数	
	返回值: 0		成功
			其他	失败
*/
//td_s32 atmprotocol_GetChnNoInOutPortNo(int _iChnNo,int _iAlarmInputNo,int _iAlarmOutputNo);

td_s32 Sendinfo2ATM(unsigned char* _ucSendBuf, int iCommandlen);

#endif
