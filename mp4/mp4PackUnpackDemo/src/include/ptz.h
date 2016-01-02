/*******************************************************************
文件名	:PTZ.h
公司	:天津天地伟业数码科技有限公司
创建者	:陶永亮
创建日期:2008-11-12
简要描述:串口的初始化和设置、云台控制相关的函数原型以及定义
修改记录:2008-11-12,陶永亮，创建
		 2008-11-28,陶永亮，添加动态管理PTZ协议的功能，修改两个接口
******************************************************************/
#ifndef PTZ_H_
#define PTZ_H_

#include "ptzex.h"
#include "td_types.h"


#define		MAX_PTZ_ADDRESS			256				//PTZ地址的最大值
#define		MIN_PTZ_ADDRESS			0				//PTZ地址的最小值
#define 	MAX_PTZ_ROUTINE_NUM		8				//最大模式路径数
#define 	MAX_PTZ_CRUISE_NUM		8				//最大巡航轨迹数
#define 	MAX_PTZ_ASSISTANT_NUM	8				//最大辅助开关数
#define 	MAX_PTZ_PRESET_NUM		255				//最大预置位数
#define		MAX_PRESET_PER_CRUISE	32				//每条巡航轨迹允许设置的最多预置点数
#define		MAX_PROTOCOL_NUM		64				//最多支持的PTZ协议数
#define		MAX_PROTOCOL_PATH_LEN	64				//最大协议动态库路径长度
#define		MAX_PROTOCOL_NAME_LEN	32				//最大协议名称长度
#define     MAX_PTZ_DO_ROUTINE_NUM	65				//最大轨迹操作数
#define		MAX_CHANNLE_NUM			32				//最大通道数
#define SLEEP_FOR_BALLINTER usleep(100000)                              //设置球机发码间隔时间100ms


typedef int (*pPtzAction)(int, PTZ_DATA *);

//可控制的协议(PROTOCOL)类型
typedef enum
{
	PRT_TIANDY = 0, 								//天地协议
	PRT_PELCO_P,									//Pelco_p
	PRT_PELCO_D,									//Pelco_D
	PRT_TC615_P,									//615_P
	PRT_TC615_D,									//615_D
	PRT_TYPES,										//设备所支持的协议数目，如要扩展，请在该条以上添加。
}TProtocolType;


//巡航点
typedef struct
{
	td_u8		m_cPreset;									//预置位号
	td_u8		m_cStayTime;								//停留时间
	td_u8		m_cSpeed;									//速度
	td_u8		m_cReserved;								//预留
}TCruisePoint;	

//巡航轨迹,对应UI的巡航路径
typedef struct
{
	td_s32			m_blEnable;								//使能
	td_s32			m_blState;								//当前巡航轨迹正在被1, 调用.0,未被调用
	td_s32			m_iCruiseNum;							//巡航点数
	TCruisePoint	m_stCrisePoint[MAX_PRESET_PER_CRUISE];	//巡航点
}TCruise;

//各PTZ协议的属性
typedef struct
{
	td_u8			m_cProtocolName[MAX_PROTOCOL_NAME_LEN];	//协议名称
	td_u8			m_cSoPath[MAX_PROTOCOL_PATH_LEN];		//动态库路径长度
	void*			m_pHandle;								//动态库句柄
	pPtzAction		m_fAction;								//该协议所对应的组码函数
}TProtocol;

typedef struct
{
	td_s32			m_time;
	PTZ_DATA		m_data;
	td_s32 			m_iAction;
}RoutinePama;

typedef enum{
	PTZ_TU = 1, 	//上
	PTZ_TD,		//下
	PTZ_PL,		//左
	PTZ_PR,		//右
	PTZ_TUPR,	//右上
	PTZ_TUPL,	//左 上
	PTZ_TDPR,	//右下
	PTZ_TDPL,	//左下
	PTZ_STOP,	//停止
	PTZ_ZIN,	// 焦距拉近
	PTZ_ZOUT,	//焦距拉远
	PTZ_IST,	//变化停止
	PTZ_FN,		//焦点调近
	PTZ_FR,		//焦点调远
	PTZ_FST,	//焦点变化停止
	PTZ_IA_ON,	//光圈自动调整
	PTZ_IO,		//光圈增大
	PTZ_IC,		//光圈缩小
	PTZ_WP_ON,//擦拭启动
	PTZ_WP_OFF,//擦拭停止
	PTZ_LP_ON,	//灯光开启
	PTZ_LP_OFF,//灯光停止
	PTZ_AUTO_START,//自动扫描开始
	PTZ_AUTO_STOP,//自动扫描停止
	PTZ_GOTO_PRESET,//转到预设点
	PTZ_START_TRACK_CRUISE,//开始轨迹巡航
	PTZ_STOP_TRACK_CRUISE,//停止轨迹巡航
	PTZ_SET_PRESET,
	PTZ_POWER_ON,	//电源开启
	PTZ_POWER_OFF,//电源关闭
	PTZ_3D_POSITION,		//PTZ 3D定位
	PTZ_3D_ROCKER,			//对3维摇杆的响应
	PTZ_ASSISTANT_ON,		//辅助开关开
	PTZ_ASSISTANT_OFF,		//辅助开关关
	PTZ_START_ROUTINE,	//开始演示轨迹
	PTZ_STOP_ROUTINE,		//结束演示轨迹
}ENUM_PTZ;

td_s32 ptz_GetComNum(td_s32* _piComNum, td_s32* _piComEnable);
/*
 * 函数名称：GetComNum
 * 简要描述: 获取设备的串口数
 * 输	  入：
 * 			_piComNum, 用于传出设备串口数，非空
 *			_piComEnable,各个串口是否可用，非空
 * 输	  出：
 * 			ERR_PTZ_SUCCESSFUL,操作成功
 *			ERR_PTZ_WRONG_PARAM, 错误的参数
 * 			ERR_PTZ_FAILED,其他原因导致操作失败
 * 修改日志：
 * 			2008-12-25,陶永亮,创建, Merry Christmas
 */

td_s32 ptz_SetBaudRate(td_s32 _iComNo, td_s32 speed);
/*
 * 函数名称：SetBaudRate
 * 简要描述: 设置某通道的波特率
 * 输	  入：
 * 			_iComNo, 0~MAX_SERIAL_NUM,端口号
 *			speed, 波特率，见PTZ.c文件中所定义的数组name_arr
 * 输	  出：
 * 			ERR_PTZ_WRONG_PARAM,错误的参数
 * 			ERR_PTZ_SUCCESSFUL,操作成功
 * 			ERR_PTZ_FAILED,其他原因导致操作失败
 * 修改日志：
 * 			2008-11-13,陶永亮,创建
 */
 
td_s32 ptz_GetBaudRate(td_s32 _iComNo, td_s32* _piBaudRate);
/*
 * 函数名称：GetBaudRate
 * 简要描述: 获取某通道的波特率
 * 输	  入：
 * 			_iComNo, 0~MAX_SERIAL_NUM,端口号
 *			_piBaudRate, 用于传出波特率，非空，可能取值见PTZ.c文件中所定义的数组name_arr
 * 输	  出：
 * 			ERR_PTZ_WRONG_PARAM,错误的参数
 * 			ERR_PTZ_SUCCESSFUL,操作成功
 * 			ERR_PTZ_FAILED,其他原因导致操作失败
 * 修改日志：
 * 			2008-11-13,陶永亮,创建
 */

td_s32 ptz_SetBits (td_s32 _iComNo, td_s32 databits, td_s32 stopbits, td_s32 parity);
/*
 * 函数名称：SetBits
 * 简要描述: 设置某通道的数据位、停止位、校验位
 * 输	  入：
 * 			_iComNo, 0~MAX_SERIAL_NUM,端口号
 *			databits, 数据位,取7或8
 *			stopbits, 停止位,取1或2
 *			parity,校验方式,可能的取值为'N','n','O','o','E','e','S','s'
 * 输	  出：
 * 			ERR_PTZ_WRONG_PARAM,错误的参数
 * 			ERR_PTZ_SUCCESSFUL,操作成功
 * 			ERR_PTZ_FAILED,其他原因导致操作失败
 * 修改日志：
 * 			2008-11-13,陶永亮,创建
 */

td_s32 ptz_GetBits(td_s32 _iComNo, td_s32* _piDataBits, td_s32* _piStopBits, td_s32* _piVerifyBits);
/*
 * 函数名称：GetBits
 * 简要描述: 获取某通道的数据位、停止位、校验位
 * 输	  入：
 * 			_iComNo, 0~MAX_SERIAL_NUM,端口号
 *			_piDataBits, 用于传出数据位,可能的取值为取7或8
 *			_piStopBits, 用于传出停止位,可能的取值为取1或2
 *			_piVerifyBits,用于传出校验方式,可能的取值为'N','n','O','o','E','e','S','s'
 * 输	  出：
 * 			ERR_PTZ_WRONG_PARAM,错误的参数
 * 			ERR_PTZ_SUCCESSFUL,操作成功
 * 			ERR_PTZ_FAILED,其他原因导致操作失败
 * 修改日志：
 * 			2008-11-13,陶永亮,创建
 */
 
td_s32 ptz_SetMode(td_s32 _iComNo, td_s32 _iMode);
/*
 * 函数名称：SetMode
 * 简要描述: 设置某通道的工作模式
 * 输	  入：
 * 			_iComNo, 0~MAX_SERIAL_NUM,端口号
 *			_iMode, 端口的工作模式，1为控制协议模式，2为透明通道
 * 输	  出：
 * 			ERR_PTZ_WRONG_PARAM,错误的参数
 * 			ERR_PTZ_SUCCESSFUL,操作成功
 * 			ERR_PTZ_FAILED,其他原因导致操作失败
 * 修改日志：
 * 			2008-11-13,陶永亮,创建
 */

td_s32 ptz_GetMode(td_s32 _iComNo, td_s32* _piMode);
/*
 * 函数名称：SetMode
 * 简要描述: 设置某通道的工作模式
 * 输	  入：
 * 			_iComNo, 0~MAX_SERIAL_NUM,端口号
 *			_piMode, 用于传出端口的工作模式，1为控制协议模式，2为透明通道
 * 输	  出：
 * 			ERR_PTZ_WRONG_PARAM,错误的参数
 * 			ERR_PTZ_SUCCESSFUL,操作成功
 * 			ERR_PTZ_FAILED,其他原因导致操作失败
 * 修改日志：
 * 			2008-11-13,陶永亮,创建
 */

td_s32 ptz_SetSlaveCom(void);
/*
 * 函数名称：SetSlaveCom
 * 简要描述: 初始化从片时设置从片的串口属性，主要用于从片异常重启后的操作
 * 输	  入：
 * 			无
 * 输	  出：
 * 			ERR_PTZ_SUCCESSFUL,操作成功
 * 			ERR_PTZ_FAILED,其他原因导致操作失败
 * 修改日志：
 * 			2009-06-18,陶永亮,创建
 */

td_s32 ptz_Clarity(td_s32 _iComNo, td_u8 * _pcData, td_s32 _iLen);
/*
 * 函数名称：Clarity
 * 简要描述: 对某通道进行透明通道下的PTZ控制
 * 输	  入：
 * 			_iComNo, 串口号，0~MAX_CHANNEL_NUM
 *			_pcData, 要写往串口的数据
 *			_iLen, 要写往串口的数据长度
 * 输	  出：
 * 			ERR_PTZ_WRONG_PARAM,错误的参数
 *			ERR_PTZ_UNKNOWN_PROTOCOL,不支持该协议
 *			ERR_PTZ_MAKE_CODE_ERROR,组码失败
 *			ERR_PTZ_WRITE_COM_ERROR,向串口写数据失败
 * 			ERR_PTZ_SUCCESSFUL,操作成功
 * 			ERR_PTZ_FAILED,其他原因导致操作失败
 * 修改日志：
 * 			2008-12-03,陶永亮,创建
 */


td_s32 ptz_GetProtocolList(td_u8 _cList[MAX_PROTOCOL_NUM][MAX_PROTOCOL_NAME_LEN]);
/*
 * 函数名称：GetProtocolList
 * 简要描述: 获取设备支持的协议列表
 * 输	  入：
 * 			_cList, 非空,用于传出协议列表
 * 输	  出：
 * 			设备支持的协议数，-1为错误的参数
 * 修改日志：
 * 			2009-11-01,陶永亮,创建
 */

td_s32 ptz_GetDelProtocolList(td_u8 _cList[MAX_PROTOCOL_NUM][MAX_PROTOCOL_NAME_LEN]);
/*
 * 函数名称：GetDelProtocolList
 * 简要描述: 获取已卸载的协议列表
 * 输	  入：
 * 			_cList, 非空,用于传出协议列表
 * 输	  出：
 * 			-1,错误的参数
 * 			其他,已卸载的协议数
 * 修改日志：
 * 			2009-11-28,陶永亮,创建
 */
 
td_s32 ptz_AddProtocol(td_u8 _cProtocol[MAX_PROTOCOL_NAME_LEN]);
/*
 * 函数名称：AddProtocol
 * 简要描述: 添加一个新的协议
 * 输	  入：
 * 			_cProtocol, 非空，待添加的协议
 * 输	  出：
 * 			ERR_PTZ_MAX_PRTNUM,已达设备支持的协议数上限
 * 			ERR_PTZ_UNKNOWN_PROTOCOL,不支持该协议
 * 			ERR_PTZ_WRONG_PARAM,错误的参数
 *			ERR_PTZ_SUCCESSFUL,添加成功
 * 修改日志：
 * 			2009-11-28,陶永亮,创建
 */
 
td_s32 ptz_DelProtocol(td_u8 _cProtocol[MAX_PROTOCOL_NAME_LEN]);
/*
 * 函数名称：DelProtocol
 * 简要描述: 卸载某一个PTZ协议
 * 输	  入：
 * 			_cProtocol, 非空，待删除的协议
 * 输	  出：
 * 			ERR_PTZ_WRONG_PARAM,错误的参数
 * 			ERR_PTZ_SUCCESSFUL,操作成功
 * 			ERR_PTZ_USING_PROTOCOL,该协议正在被某一个通道使用
 *			ERR_PTZ_UNKNOWN_PROTOCOL,不支持该协议，通道是找不到该协议对应的so文件
 * 修改日志：
 * 			2009-11-28,陶永亮,创建
 */
 
td_s32 ptz_SetChannelPTZ(td_s32 _iChannelNo, td_s32 _iComNo, td_u8* _cProtocolName, td_s32 _iAddress);
/*
 * 函数名称：SetChannelPTZ
 * 简要描述: 设置某通道的PTZ属性
 * 输	  入：
 * 			_iChannelNo, 0~MAX_CHANNEL_NUM,通道号
 *			_iComNo, 设置该通道用于PTZ控制的端口,0~MAX_SERIAL_NUM
 *			_cProtocolName,该通道用于PTZ控制的协议类型名称,非空
 *			_iAddress,该通道所接设备的PTZ地址
 * 输	  出：
 * 			ERR_PTZ_WRONG_PARAM,错误的参数
 *			ERR_PTZ_UNKNOWN_PROTOCOL,不支持的协议
 * 			ERR_PTZ_SUCCESSFUL,操作成功
 * 			ERR_PTZ_FAILED,其他原因导致操作失败
 * 修改日志：
 * 			2008-11-13,陶永亮,创建
 *			2008-11-28,陶永亮，将第三个参数修改为协议名称
 */

td_s32 ptz_GetChannelPTZ(td_s32 _iChannelNo, td_s32* _piComNo, td_u8* _pcProtocol, td_s32* _piAddress);
/*
 * 函数名称：GetChannelPTZ
 * 简要描述: 获取某通道的PTZ属性
 * 输	  入：
 * 			_iChannelNo, 0~MAX_CHANNEL_NUM,通道号
 *			_piComNo, 用于传出该通道用于PTZ控制的端口,0~MAX_SERIAL_NUM
 *			_pcProtocol,用于传出该通道用于PTZ控制的协议类型名称,非空
 *			_piAddress,用于传出该通道所接设备的PTZ地址
 * 输	  出：
 * 			ERR_PTZ_WRONG_PARAM,错误的参数
 * 			ERR_PTZ_SUCCESSFUL,操作成功
 * 			ERR_PTZ_FAILED,其他原因导致操作失败
 * 修改日志：
 * 			2008-11-13,陶永亮,创建
 *			2008-11-28,陶永亮，将第三个参数修改为协议名称
 */
 
td_s32 ptz_Control(td_s32 _iChannelNo, td_s32 _iAction ,PTZ_DATA* _pData);
/*
 * 函数名称：Control
 * 简要描述: 对某通道进行PTZ控制
 * 输	  入：
 * 			_iChannelNo, 通道号，0~MAX_CHANNEL_NUM
 *			_iAction, 动作类型, 见上面的枚举类型ENUM_PTZ;在透明通道模式下不使用该参数
 *			_pData, 进行PTZ控制所需的参数,在透明通道模式下只使用其中的iCount和DecBuf两个字段
 * 输	  出：
 * 			ERR_PTZ_WRONG_PARAM,错误的参数
 *			ERR_PTZ_UNKNOWN_PROTOCOL,不支持该协议
 *			ERR_PTZ_MAKE_CODE_ERROR,组码失败
 *			ERR_PTZ_WRITE_COM_ERROR,向串口写数据失败
 * 			ERR_PTZ_SUCCESSFUL,操作成功
 * 			ERR_PTZ_FAILED,其他原因导致操作失败
 * 修改日志：
 * 			2008-11-13,陶永亮,创建
 */

td_s32 ptz_Init(td_s32 _iMaxChannelCnt);
/*
 * 函数名称：Init
 * 简要描述: 初始化PTZ模块
 * 输	  入：
 * 			无
 * 输	  出：
 * 			ERR_PTZ_SUCCESSFUL,操作成功
 * 			ERR_PTZ_FAILED,其他原因导致操作失败
 * 修改日志：
 * 			2008-11-13,陶永亮,创建
 */
 
td_s32 ptz_Exit(void);
/*
 * 函数名称：Exit
 * 简要描述: 去初始化PTZ模块
 * 输	  入：
 * 			无
 * 输	  出：
 * 			ERR_PTZ_SUCCESSFUL,操作成功
 * 			ERR_PTZ_FAILED,其他原因导致操作失败
 * 修改日志：
 * 			2008-11-13,陶永亮,创建
 */

td_s32 ptz_SetCruise(td_s32 _iChannelNo, td_s32 _iCruiseNo, TCruise* _pstCruise);
/*
 * 函数名称：SetCruise
 * 简要描述: 设置某通道的某条巡航轨迹
 * 输	  入：
 * 			_iChannelNo, 0~MAX_CHANNEL_NUM, 通道号
 *			_iCruiseNo, 0~MAX_PTZ_CRUISE_NUM, 巡航轨迹号
 *			_pstCruise, 非空，设置巡航轨迹参数
 * 输	  出：
 * 			ERR_PTZ_SUCCESSFUL,操作成功
 *			ERR_PTZ_WRONG_PARAM, 错误的参数
 * 			ERR_PTZ_FAILED,其他原因导致操作失败
 * 修改日志：
 * 			2008-12-25,陶永亮,创建, Merry Christmas
 */

td_s32 ptz_GetCruise(td_s32 _iChannelNo, td_s32 _iCruiseNo, TCruise * _pstCruise);
/*
 * 函数名称：GetCruise
 * 简要描述: 获取某通道的某条巡航轨迹
 * 输	  入：
 * 			_iChannelNo, 0~MAX_CHANNEL_NUM, 通道号
 *			_iCruiseNo, 0~MAX_PTZ_CRUISE_NUM, 巡航轨迹号
 *			_pstCruise, 非空，传出巡航轨迹参数
 * 输	  出：
 * 			ERR_PTZ_SUCCESSFUL,操作成功
 *			ERR_PTZ_WRONG_PARAM, 错误的参数
 * 			ERR_PTZ_FAILED,其他原因导致操作失败
 * 修改日志：
 * 			2008-12-25,陶永亮,创建, Merry Christmas
 */
td_s32 ptz_StartCruise(td_s32 _iChannelNo, td_s32 _iCruiseNo);
/*
 * 函数名称：StartCruise
 * 简要描述: 开始调用某条巡航轨迹
 * 输	  入：
 * 			_iChannelNo, 0~MAX_CHANNEL_NUM, 通道号
 *			_iCruiseNo, 0~MAX_PTZ_CRUISE_NUM, 巡航轨迹号
 * 输	  出：
 * 			ERR_PTZ_SUCCESSFUL,操作成功
 *			ERR_PTZ_WRONG_PARAM, 错误的参数
 *			ERR_PTZ_UNUSABLE_FUNCTION,该巡航轨迹未被启用
 * 			ERR_PTZ_FAILED,其他原因导致操作失败,如该轨迹已经在运行
 * 修改日志：
 * 			2008-12-25,陶永亮,创建, Merry Christmas
 */
 
td_s32 ptz_StopCruise(td_s32 _iChannelNo, td_s32 _iCruiseNo);
/*
 * 函数名称：StopCruise
 * 简要描述: 停止调用某条巡航轨迹
 * 输	  入：
 * 			_iChannelNo, 0~MAX_CHANNEL_NUM, 通道号
 *			_iCruiseNo, 0~MAX_PTZ_CRUISE_NUM, 巡航轨迹号
 * 输	  出：
 * 			ERR_PTZ_SUCCESSFUL,操作成功
 *			ERR_PTZ_WRONG_PARAM, 错误的参数
 *			ERR_PTZ_UNUSABLE_FUNCTION,该巡航轨迹未被启用
 * 			ERR_PTZ_FAILED,其他原因导致操作失败,如该轨迹已经在运行
 * 修改日志：
 * 			2008-12-25,陶永亮,创建, Merry Christmas
 */
 
td_s32 ptz_StartSetRoutine(td_s32 _iChannelNo, td_s32 _iRoutineNo);
/*
 * 函数名称：StartSetRoutine
 * 简要描述: 开始设置模式路径
 * 输	  入：
 * 			_iChannelNo, 0~MAX_CHANNEL_NUM, 通道号
 *			_iRoutineNo, 0~MAX_PTZ_ROUTINE_NUM, 模式路径号
 * 输	  出：
 * 			ERR_PTZ_SUCCESSFUL,操作成功
 *			ERR_PTZ_WRONG_PARAM, 错误的参数
 * 			ERR_PTZ_FAILED,其他原因导致操作失败
 * 修改日志：
 * 			2008-12-25,陶永亮,创建, Merry Christmas
 */
 
td_s32 ptz_StopSetRoutine(td_s32 _iChannelNo, td_s32 _iRoutineNo);
/*
 * 函数名称：StopSetRoutine
 * 简要描述: 结束设置某条模式路径
 * 输	  入：
 * 			_iChannelNo, 0~MAX_CHANNEL_NUM, 通道号
 *			_iRoutineNo, 0~MAX_PTZ_ROUTINE_NUM, 模式路径号
 * 输	  出：
 * 			ERR_PTZ_SUCCESSFUL,操作成功
 *			ERR_PTZ_WRONG_PARAM, 错误的参数
 * 			ERR_PTZ_FAILED,其他原因导致操作失败
 * 修改日志：
 * 			2008-12-25,陶永亮,创建, Merry Christmas
 */
 
td_s32 ptz_StartRoutine(td_s32 _iChannelNo, td_s32 _iRoutineNo);
/*
 * 函数名称：StartRoutine
 * 简要描述: 开始调用某条模式路径
 * 输	  入：
 * 			_iChannelNo, 0~MAX_CHANNEL_NUM, 通道号
 *			_iRoutineNo, 0~MAX_PTZ_ROUTINE_NUM, 模式路径号
 * 输	  出：
 * 			ERR_PTZ_SUCCESSFUL,操作成功
 *			ERR_PTZ_WRONG_PARAM, 错误的参数
 * 			ERR_PTZ_FAILED,其他原因导致操作失败
 * 修改日志：
 * 			2008-12-25,陶永亮,创建, Merry Christmas
 */
 
td_s32 ptz_StopRoutine(td_s32 _iChannelNo, td_s32 _iRoutineNo);
/*
 * 函数名称：StopRoutine
 * 简要描述: 结束调用某条模式路径
 * 输	  入：
 * 			_iChannelNo, 0~MAX_CHANNEL_NUM, 通道号
 *			_iRoutineNo, 0~MAX_PTZ_ROUTINE_NUM, 模式路径号
 * 输	  出：
 * 			ERR_PTZ_SUCCESSFUL,操作成功
 *			ERR_PTZ_WRONG_PARAM, 错误的参数
 * 			ERR_PTZ_FAILED,其他原因导致操作失败
 * 修改日志：
 * 			2008-12-25,陶永亮,创建, Merry Christmas
 */

td_s32 ptz_GotoPreset(td_s32 _iChannelNo, td_s32 _iPresetNo);
/*
 * 函数名称：GotoPreset
 * 简要描述: 调用预置位
 * 输	  入：
 * 			_iChannelNo, 0~MAX_CHANNEL_NUM, 通道号
 *			_iPresetNo, 预置位号,1~MAX_PTZ_PRESET_NUM 
 * 输	  出：
 * 			ERR_PTZ_SUCCESSFUL,操作成功
 *			ERR_PTZ_WRONG_PARAM, 错误的参数
 * 			ERR_PTZ_FAILED,其他原因导致操作失败
 * 修改日志：
 * 			2008-08-21,陶永亮,从Alarm.c中移至PTZ.c
 */

td_s32 ptz_DefaultCfg(void);
/*
 * 函数名称：DefaultCfg
 * 简要描述: 恢复巡航轨迹的初始设置
 * 输	  入：
 * 			无
 * 输	  出：
 * 			ERR_PTZ_SUCCESSFUL,操作成功
 *			ERR_PTZ_WRONG_PARAM, 错误的参数
 * 			ERR_PTZ_FAILED,其他原因导致操作失败
 * 修改日志：
 * 			2009-01-16,陶永亮,创建
 */

td_s32 ptz_AssistantSwitch(td_s32 _iChannelNo, td_s32 _iSwitchNo, td_s32 _iType);
/*
 * 函数名称：AssistantSwitch
 * 简要描述: 打开或关闭辅助开关
 * 输	  入：
 * 			_iChannelNo, 通道号, 0~MAX_CHANNEL_NUM
 *			_iSwitchNo, 辅助开关号，0~MAX_PTZ_ASSISTANT_NUM
 *			_iType, 1为打开， 0为关闭
 * 输	  出：
 * 			ERR_PTZ_SUCCESSFUL,操作成功
 *			ERR_PTZ_WRONG_PARAM, 错误的参数
 * 			ERR_PTZ_FAILED,其他原因导致操作失败
 * 修改日志：
 * 			2009-05-16,陶永亮,创建
 */

td_s32 ptz_SetChannelComAttr(td_s32 _iChannelNo, TCom* _pstCom, td_s32 _blEnable);
/*
 * 函数名称：SetChannelComAttr
 * 简要描述: 设置通道PTZ的串口属性
 * 输	  入：
 * 			_iChannelNo, 通道号, 0~MAX_CHANNEL_NUM
 *			_pstCom, 串口属性，非空
 *			_blEnable, 是否自定义串口属性，0或1
 * 输	  出：
 * 			ERR_PTZ_SUCCESSFUL,操作成功
 *			ERR_PTZ_WRONG_PARAM, 错误的参数
 * 			ERR_PTZ_FAILED,其他原因导致操作失败
 * 修改日志：
 * 			2009-11-20,陶永亮,创建
 */
 
td_s32 ptz_GetChannelComAttr(td_s32 _iChannelNo, TCom* _pstCom, td_s32* _pblEnable);
/*
 * 函数名称：GetChannelComAttr
 * 简要描述: 获取通道PTZ的串口属性
 * 输	  入：
 * 			_iChannelNo, 通道号, 0~MAX_CHANNEL_NUM
 *			_pstCom, 传出串口属性，非空
 *			_pblEnable, 用于传出是否自定义串口属性，非空
 * 输	  出：
 * 			ERR_PTZ_SUCCESSFUL,操作成功
 *			ERR_PTZ_WRONG_PARAM, 错误的参数
 * 			ERR_PTZ_FAILED,其他原因导致操作失败
 * 修改日志：
 * 			2009-11-20,陶永亮,创建
 */

td_s32 ptz_NetKeyBorad(td_s32 _iCh, td_s32 _iCode);
/*
 * 函数名称：NetKeyBorad
 * 简要描述: 通过网络键盘进行PTZ控制
 * 输	  入：
 * 			_iCh, 通道号, 0~MAX_CHANNEL_NUM
 *			_iCode, 键盘码
 * 输	  出：
 * 			ERR_PTZ_WRONG_PARAM,错误的参数
 *			ERR_PTZ_UNKNOWN_PROTOCOL,不支持该协议
 *			ERR_PTZ_MAKE_CODE_ERROR,组码失败
 *			ERR_PTZ_WRITE_COM_ERROR,向串口写数据失败
 * 			ERR_PTZ_SUCCESSFUL,操作成功
 * 			ERR_PTZ_FAILED,其他原因导致操作失败
 * 修改日志：
 * 			2009-11-28,陶永亮,从gui_cmd_execute.c中移至此，并独立为函数
 */

td_s32 ptz_3DPosition(td_s32 _iCh, td_s32* _iParam);
/*
 * 函数名称：3DPosition
 * 简要描述: 通过网络键盘进行PTZ控制
 * 输	  入：
 * 			_iCh, 通道号, 0~MAX_CHANNEL_NUM
 *			_iParam, 3D定位参数，整型数组，依此为鼠标位置x/鼠标位置y/屏幕宽/屏幕高/
 *			区域宽(可正负，负表反方向, 0表点)/区域高(可正负，负表反方向, 0表点)
 * 输	  出：
 * 			ERR_PTZ_WRONG_PARAM,错误的参数
 *			ERR_PTZ_UNKNOWN_PROTOCOL,不支持该协议
 *			ERR_PTZ_MAKE_CODE_ERROR,组码失败
 *			ERR_PTZ_WRITE_COM_ERROR,向串口写数据失败
 * 			ERR_PTZ_SUCCESSFUL,操作成功
 * 			ERR_PTZ_FAILED,其他原因导致操作失败
 * 修改日志：
 * 			2009-11-28,陶永亮,从gui_cmd_execute.c中移至此，并独立为函数
 */

td_s32 ptz_GetRoutineStat(int _iChn);
td_s32 ptz_GetCruiseStat(int _iChn);

td_s32 ptz_GetVcChnComNo(td_s32 *_piComNo,td_s32 *_piVcMainChnNo);
/*
 * 函数名称：ptz_GetVcChnComNo
 * 简要描述: 获取合成通道的串口编号，默认是最大的串口号
 * 输	  入：
 * 			_piComNo, 串口号
 *			_piVcMainChnNo,  合成通道的主画面号
 * 输	  出：
 * 			
 * 修改日志：
 * 			2009-11-10,郭亚楠
 */

td_s32 Ptz_sComAttrForVc(int _iComNo);

td_s32 Ptz_sChannelPTZForVc(td_s32 _iMainChannelNo);
/*
 * 函数名称：Ptz_sChannelPTZForVc
 * 简要描述: 
 * 输	  入：
 * 			_iMainChannelNo, 通道号, 当前设置ptz的通道号
 * 输	  出：

 * 修改日志：
 * 			2012-11-15,郭亚楠，为了实现合成通道控制住画面时候在
 			透明通道模式下可以正确组码
 */


/*
 * 函数名称：RockerAction
 * 简要描述: 
 * 输	  入：

 * 输	  出：

 * 修改日志：
 * 			2012-12-25,董利强，为了兼容ONVIF网络摇杆
 */

int RockerAction(PTZ_DATA* _pData, TPtzCtrProtocolData *_pPtzCtrl);
void ptz_DelProtocoltoDefalut();
//for decoder
td_s32 ptz_SetComAddr4Decoder(td_s32 _iComNo, td_u8 _u8Addr);

#endif  /*PTZ_H_*/

