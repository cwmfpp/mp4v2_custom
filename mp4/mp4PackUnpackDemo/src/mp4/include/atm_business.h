#ifndef ATM_BUSINESS_H_
#define ATM_BUSINESS_H_
//查询条件结构

#include "atm_protocol.h"

#define		MAX_ATM_PROTOCOL_NUM		64				//最多支持的PTZ协议数
#define		MAX_ATM_PROTOCOL_PATH_LEN	256				//最大协议动态库路径长度

typedef struct
{
	td_s32			m_iType;				//类型，0为按卡号查询，1为按域查询
	td_s8			m_ucChannelNo;			//通道号
	time_t			m_iStartTime;			//录像开始时间
	time_t			m_iStopTime;			//录像结束时间	
	//20121130 SUN Wei debug for Domain Query 12733 
	td_u8			m_QueryKey[5][DOMAINDATA_LEN];		//复用结构体，卡号/域	
	//20121130 SUN Wei debug for Domain Query 12733 end
	td_u32			m_uiPageNo;				//查询结果页号,从0开始
	td_u32			m_uiPageSize;			//每页条目数
	td_s32			m_uiTotalNum;			//总条目数
	td_u32			m_uiShowNum;						//已显示的条目数
	TAtmTransaction	m_atmTransaction[PAGE_SHOW_NUM];	//ATM交易信息
	TAtmException	m_atmException[PAGE_SHOW_NUM];		//ATM异常信息
	TFileInfo		m_stFileInfo[PAGE_SHOW_NUM];		//文件信息（使用原来的）
}TAtmQuery;
/*
	atmbusiness模块需要做的工作

	1、初始化/去初始化atm_protocol库
	2、从主程序中获取别名信息、录像状态、报警状态，并将获取到的信息下发给atm协议模块
	3、根据上传的atm信息进行相应的处理
	//增加启动开关，根据配置文件决定是否启动
	
*/

//Define in atm_protocol.h

//初始化，确定厂家，串口/网口，
//typedef struct
//{
//	td_u8			m_u8ProtocolName[32];		//atm协议名称
//	td_s32 			m_s32Type;					//协议类型 串口协议或者网络协议
//	unsigned long 	m_ulSrcIp;					//源地址
//	unsigned long	m_ulSrcPort;				//源端口	
//	unsigned long	m_ulDestIp;					//目的地址
//	unsigned long	m_ulDestPort;				//目的端口
//	TComInstance* 	m_ptComIns;					//串口实例
//}TAtmProtocolParam;



//初始化
td_s32 atmbusiness_Init();
/*
	功能:	初始化数据库
			初始化ATM协议解析库

			向协议解析库注册用于处理ATM上传信息的回调函数
			回调函数包含以下处理
			1）	插卡事件，走插卡联动录像的操作流程。
			2）	拔卡事件，走拔卡停止录像的操作流程。	
			3）	其他操作事件，仅记录数据库，不联动录像。
			4）	校时操作，调用系统校时函数进行校时操作。			
				以上四项操作需记录日志
	
	参数:	_pProtocolParam		协议参数，包括名称、类型等
			
	返回值:	0		成功
			其他	失败	
*/


td_s32 atmbusiess_DeInit();
/*
	功能:去初始化ATM业务模块
*/

td_s32 atmbusiness_AlarmStateNotify(TAlarmType _eType,int _iAlarmNo,TAlarmState _eState);
/*
	功能:	在DvrAlarmStatChgNotify函数中增加向ATM业务模块传递报警信息的处理。
			向atm协议解析模块通知报警状态改变。	
	参数:	_eType		报警类型
			_iAlarmNo	报警通道号或报警端口号
			_eState		报警状态，报警或消警	
	返回值: 0		成功
			其他	失败
*/
td_s32 atmbusiness_OutPortStateNotify(int _iPortNo, TAlarmState _eState);
/*
	功能:	在DvrAlarmStatChgNotify函数中增加向ATM业务模块传递报警信息的处理。
			向atm协议解析模块通知报警状态改变。	
	参数:	_eType		报警类型
			_iAlarmNo	报警通道号或报警端口号
			_eState		报警状态，报警或消警	
	返回值: 0		成功
			其他	失败
*/

td_s32 atmbusiness_RecordStateNotify(int _iChnNo,TRecordType _eRecordState);
/*
	功能:	在DvrRecStatChgNotify函数中增加向ATM业务模块传录像状态的处理。
			向atm协议解析模块通知报录像状态改变。
	参数:	_iChnNo		通道号
			_eState		录像状态
	返回值: 0		成功
			其他	失败
*/

td_s32 atmbusiness_AliasChangeNotify(TAliasType _eType,int _iChnNo,td_u8* _cAlias);
/*
	功能:	在ChnAttrChgMsgProc函数中增加向ATM业务模块传递别名信息的处理。
			向atm协议解析模块通知别名改变。
	参数:	_eType		别名类型
			_iChnNo		通道号
			_cAlias		别名
	返回值: 0		成功
			其他	失败
*/

td_s32 atmbusiness_Query(td_u32 _uiSocket, TAtmQuery* _stQ);
/*
	功能:	卡号录像查询
	参数:	_uiSocket	用于表示是谁执行了这个查询;0表示UI，>0表示网络客户端，其他非法
			_iChnNo		查询体，用于输入查询信息和输出查询结果
	返回值: 0		成功
			其他	失败
*/

void atmbusiness_UpgradeIndex(td_s8* _ucFileName, td_s8* _ucNewFileName,td_s32 _iType);
/*
	功能:	用于向存储模块注册的回调函数,在发生录像覆盖或者重建索引时调用该函数通知ATM模块
	_iType 	0: 删除文件  				文件名保存在_ucFileName[0]中
	        1: 重建索引更新文件名		老文件名保存在_ucFileName[0]中，新文件名保存在_ucFileName[1]中
	        2: 重建索引文件名未更改 	文件名保存在_ucFileName[0]中
	        3: 重建索引结束
*/

td_s32 atmbusiness_GetProtocolList(char _cList[MAX_ATM_PROTOCOL_NUM][256],int* _iProtocolNum);
/*
	功能:	获取atm协议列表
	参数:	_cList	协议列表
	返回值: 0		成功
			其他	失败
*/

td_s32 atmbusiness_SetPara(TAtmPara* _patmPara);
//20121120 SUN Wei add for filesize problem
void cbk_UpdateAtmTableRecFileSize(td_s8 * _ucFileName, td_s32 _iRecFileSize, td_s32 _iStopTime);
//20121120 SUN Wei
td_s32 atmbusiness_DeleteDataBase(void);
//wangsong, 20121224, 删除超过数据库最大限制的条目
#endif
