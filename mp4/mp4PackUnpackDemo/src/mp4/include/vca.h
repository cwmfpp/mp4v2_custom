
/*zhangyi add 20110309*/
#ifndef __VCA_H__
#define __VCA_H__

#include "vca_head.h"
#include "td_types.h"

#include "alarm.h"				//zhangyi add 20110426
extern td_s32 g_blClrAlmCntVca;	

#define		MAX_RULE_NUM				8
#define		MAX_POLYGON_POINT_NUM		8
#define		MAX_TRIPWIRE_NUM			8
//#define		MAX_RULE_NAME_LEN			16  detel by nsg 20120321
#define		MAX_TIMESEGMENT				4
#define		MAX_EVENT_NUM				14
#define		MAX_SAVE_TARGET_NUM			16
#define		LINKAGE_VCA					3
#define     MAX_SLAVE_CHN_NUM  			8
#define 	MAX_MSG_NUM      			30  // 智能分析警情最多缓存条数
#define		MAX_FDEPTH_LINE_NUM			3	// 智能分析标定个数(默认为3个)

//move from old vca_core by caizhaoxu 2012-08-02
#define   	VIDEOINPUT_VCA_MAX_CHANNEL_NUM  32 //3516只有一路，此参数预留给其他项目
#define   	MAX_VCA_TIMESEGMENT  		4

//shihc add 20130111 为智能分析跟踪仪增加需要的宏
#define CAP_WIDTH 352
#define CAP_HEIGHT 288
#define CAP_PWIDTH 11 //352/32
#define CAP_PHEIGHT 9//288/32
#define PTZ_OFFSET_SPEED 6


#define		VCA_MODULE	  				1
#define 	PI 3.1415926
#define 	ARROW_ANGLE PI/10
#define 	ARROW_LEN 10
#define 	TRIPWIRE_ARROW_LEN 30
#define 	PERIMETER_ARROW_LEN 60
#define 	ASCLENGTH 4096

#define		RET_CAP_STOP 1
//----------------------------------------------------------------------------------
typedef struct
{
	int		m_iVideoSize;
	int		m_iDisplayTarget;			// 是否叠加目标框
	int		m_iDisplayTrace;			// 是否叠加轨迹	
	int		m_iDisplayAllTargetTrace;	// 是否叠加所有目标及轨迹		//zhangyi add 20110720
	int		m_iTargetColor;				// 目标框颜色
	int		m_iTargetAlarmColor;		// 报警时目标框颜色
	int		m_iTraceLength;				// 轨迹长度
} vca_TConfig_ex; 			// 智能分析配置参数  
//----------------------------------------------------------------------------------
typedef struct
{
	int m_iX;
	int	m_iY;
} vca_TPoint_ex;

typedef struct
{
	vca_TPoint_ex m_sTripwireArrow[4];
	vca_TPoint_ex m_sPerimeterArrow[4];
}vca_TRuleArrow_ex;
#if 0
//liusong added
typedef struct
{
	td_s32             m_VideoSize;
	vca_TRuleArrow m_stArrowInfo[MAX_RULE_NUM];
}vca_ArrowInfo;
#endif
typedef struct
{
	TVcaPoint 	m_stStart;
	TVcaPoint 	m_stEnd;
} vca_TLine_ex;

typedef struct
{
	int				m_iValid;
	int				m_iTargetTypeCheck;
	int				m_iMinDistance;
	int				m_iMinTime;	
	int				m_iDisplayRule;
	int				m_iDisplayStat;
	int				m_iColor;
	int				m_iAlarmColor;
	int				m_iType;
	int				m_iTDirection;
	vca_TLine_ex	m_stLine;
    
} TVcaTripParam_ex;  	// 单绊线事件配置相关参数

typedef struct
{
	//TVcaTripParam_ex	m_stVcaTripParam;
	int				m_iAlarmCount;
	TAlarmParam		m_stAlarmParam;	
} vca_TTripEvent_ex; 	// 单绊线参数

typedef struct
{
	int 		    m_iPointNum;
	TVcaPoint 	m_stPoint[MAX_POLYGON_POINT_NUM];   
} vca_TPolygon_ex;

typedef struct
{
	int				m_iValid;
	int				m_iDisplayRule;
	int				m_iDisplayStat;
	int				m_iColor;
	int				m_iAlarmColor;	
	int				m_iTargetTypeCheck;
	int				m_iMode;
	int 			m_iMinDistance;					
	int 			m_iMinTime;					
	int				m_iType;
	int 			m_iDirection;
	vca_TPolygon_ex	m_stRegion;		
} TVcaPeriParam_ex;	// 周界事件配置相关参数

typedef struct
{
	//TVcaPeriParam_ex	m_stVcaPeriParam;
	int				m_iAlarmCount;					
	TAlarmParam		m_stAlarmParam;	
} vca_TPeriEvent_ex; // 周界参数

typedef struct
{
    int				m_iValid;
	int				m_iDisplayRule;
	int				m_iDisplayStat;
	int				m_iColor;
	int				m_iAlarmColor;	
	int				m_iMinTime;
	int				m_iMinSize;
	int 			m_iMaxSize;	
	int				m_iChildAreaNum;    //无效子区域个数
	vca_TPolygon_ex	m_stMainRegion;	    //最外的多边形区域	
    vca_TPolygon_ex	m_stRegionA;	    //子区域A
	vca_TPolygon_ex	m_stRegionB;        //子区域B
	vca_TPolygon_ex	m_stRegionC;        //子区域C

}TVcaLeaveParam_ex;// 物体遗留相关参数

typedef struct
{
    //TVcaLeaveParam_ex  m_stLeaveParam;
    int             m_iAlarmCount;
    TAlarmParam		m_stAlarmParam;	
}vca_TLeaveEnevnt_ex;

typedef struct
{
	int 			m_iValid;
	int 			m_iDisplayRule;
	int 			m_iDisplayStat;
	int				m_iColor;
	int 			m_iAlarmColor;		//人脸识别没有此位
	vca_TPolygon_ex	m_stRegion;
} TVcaFaceParam_ex;	// 人脸检测事件配置相关参数

typedef struct
{
	//TVcaFaceParam_ex	m_stVcaFaceParam;
	int				m_iAlarmCount;				
	TAlarmParam		m_stAlarmParam;	
} vca_TFaceEvent_ex;	// 人脸检测参数

typedef struct
{
	int				m_iValid;
	int				m_iDisplayStat;
	int				m_iCheckTime;
} TVcaDiagParam_ex;	// 视频诊断事件配置参数

typedef struct
{
	//TVcaDiagParam_ex	m_stVcaDiagParam;
	int				m_iAlarmCount;				
	TAlarmParam		m_stAlarmParam;	
} vca_TDiagEvent_ex;	// 视频诊断参数

typedef struct
{
	int				m_iAlarmCount;				
	TAlarmParam		m_stAlarmParam;	
} vca_TCAPEvent_ex;	// 智能跟踪参数

typedef struct
{
	int				m_iAlarmCount;				
	TAlarmParam		m_stAlarmParam;	
} vca_TCpcEvent_ex;	// 流量统计

typedef struct
{
	int				m_iAlarmCount;				
	TAlarmParam		m_stAlarmParam;	
} vca_TCddEvent_ex;	// 人群聚集
typedef struct
{
	int				m_iAlarmCount;				
	TAlarmParam		m_stAlarmParam;	
} vca_TSvdEvent_ex;	// 离岗检测

typedef struct
{
	vca_TTripEvent_ex		m_stTripEvent;		// 穿越绊线参数
	vca_TPeriEvent_ex		m_stPeriEvent;		// 周界参数
	vca_TFaceEvent_ex		m_stFaceEvent;		// 人脸识别参数
	vca_TDiagEvent_ex		m_stDiagEvent;		// 视频诊断参数	zhangyi add 20110601
	vca_TLeaveEnevnt_ex    	m_stLeaveEvent;     // 物品遗留参数   add by nsg 20120313	
	vca_TCAPEvent_ex		m_stCAPEvent;       // 智能跟踪参数  add by shihc 20130227
	vca_TCpcEvent_ex		m_stCpcEvent; 		// 流量统计参数
	vca_TCddEvent_ex		m_stCddEvent; 		//人群聚集参数
	vca_TSvdEvent_ex		m_stSvdEvent;		//离岗检测参数
} vca_TEventParam_ex; 		// 行为分析事件参数

typedef struct
{
	vca_TEventParam_ex	m_stEventParam;						// 行为分析事件参数
} vca_TRuleParam_ex;   	// 规则设置参数


//----------------------------------------------------------------------------------
typedef struct 
{
	int		m_iEnableAdvanced; 				//是否启用高级参数
	int		m_iMinSize; 					//目标最小像素数
	int		m_iMaxSize; 					//目标最大像素数
	int   	m_iMinWidth;  					//最大宽度
	int		m_iMaxWidth;  					//最小宽度
	int		m_iMinHeight;  					//最大高度
	int		m_iMaxHeight;  					//最小高度
	int		m_iTargetMinSpeed;				//目标最小像素速度(-1为不做限制)
	int 	m_iTargetMaxSpeed;				//目标最大像素速度(-1为不做限制)
	int		m_iMinWHRatio;					//最小长宽比
	int 	m_iMaxWHRatio;					//最大长宽比
	int		m_iSensitivity;					//灵敏度级别
} vca_TAdvancedParam_ex; 	// 智能分析高级参数
//-----------------------------------------------------------------------------------
typedef struct 
{
	vca_TLine_ex 	m_stRefLine;		// 垂直线段坐标(unit: pixel)
	int 		m_s32RefLen;		// 线段长度(unit: cm)
} vca_TFdeLine_ex;		// 景深线结构

typedef struct
{
	unsigned int	m_i32NumUsed; 					// 样本数目 大于等于2
	vca_TFdeLine_ex 	m_stLines[MAX_FDEPTH_LINE_NUM];	// 样本数目
} vca_TFdeMeasure_ex; 	// 标定样本参数

typedef struct
{
	unsigned int 			m_iEnableFdepth;	//启用标识
	vca_TFdeMeasure_ex 		m_stMeasure;		//标定数据
} vca_TFdepthParam_ex;	// 智能分析标定配置参数
//-----------------------------------------------------------------------------------
typedef struct
{
	int 					m_chn;
   	vca_TRuleParam_ex		m_stRule[MAX_RULE_NUM]; 	// 规则设置参数
} vca_TVCAParam_ex;	// 智能分析配置参数
//-----------------------------------------------------------------------------------
typedef struct 
{
	time_t  		m_iTime;
	int 			m_iChn;
	int     		m_iRule;
	TVcaEventType 	m_eType;
	int     		m_iState;
	unsigned int 	m_u32TId;
	int 			m_u32Type;	
}AlarmData_ex;
typedef enum CAPResultStatus
{
	CAP_RESULT_A=0,//PT to origin & Z wide
	CAP_RESULT_B=1,//To do nothing
	CAP_RESULT_C=2,//PT move
	CAP_RESULT_D=3,//ZoomBegin&Z close up
	CAP_RESULT_E=4,//ZoomProcess & To do nothing
	CAP_RESULT_F=5,//ZoomEnd & Z stop
	CAP_RESULT_G=6//PT Stop
	
}CAP_RESULT_STATUS_E;

typedef struct 
{
	TVcaEventType 	m_eType;
	TAlarmParam     *m_pAlarmParam;
}vca_strRuleAlarmMap;

typedef struct
{
	vca_strRuleAlarmMap m_RuleAlarmMap[MAX_RULE_NUM];
}vca_AlarmMap;

int vca_SetAlarmMap(int _iChn, int _iRule, TVcaEventType _type);

vca_strRuleAlarmMap* vca_GetAlarmMap(int _iChn, int _iRule);

#define DFT_WRITE_CFG	0		//写默认配置
#define	SET_WRITE_CFG	1		//写设置配置
td_s32 vca_WriteCfg();
/*
 * 函数名称：vca_WriteCfg
 * 简要描述: 写智能分析配置文件
 * 输	  入：
 * 			
 * 输	  出：
 *			ERR_VCA_SUCCESSFUL
 *			ERR_VCA_FAILED
 *			ERR_VCA_OPEN_FILE_ERROR			
 *			ERR_VCA_WRITE_FILE_ERROR					
 *			ERR_VCA_NO_SUCH_FILE			
 * 修改日志：
 * 			2011-03-21,张仪,创建
 *			2011-08-02,张仪,修改
 */
td_s32 vca_ReadCfg();
/*
 * 函数名称：vca_ReadCfg
 * 简要描述: 读智能分析配置文件
 * 输	  入：
 * 			
 * 输	  出：
 *			ERR_VCA_SUCCESSFUL
 *			ERR_VCA_FAILED
 *			ERR_VCA_OPEN_FILE_ERROR					
 *			ERR_VCA_READ_ERROR				
 *			ERR_VCA_NO_SUCH_FILE
 * 修改日志：
 * 			2011-03-21,张仪,创建
 */

td_s32 vca_DefaultCfg();
/*
 * 函数名称：vca_DefaultCfg
 * 简要描述: 恢复智能分析默认配置文件
 * 输	  入：
 * 			
 * 输	  出：
 *			ERR_VCA_SUCCESSFUL
 *			ERR_VCA_FAILED
 *			ERR_VCA_OPEN_FILE_ERROR			
 *			ERR_VCA_WRITE_FILE_ERROR					
 * 修改日志：
 * 			2011-03-21,张仪,创建
 */
 
td_s32 vca_Init(const MediaDevice* _pMediaDevice,strChannelParam *_tChnPara,td_s32 _iCfgVideo,td_s32 _iVideoInput);

/*
 * 函数名称：vca_Init
 * 简要描述: 初始化智能分析模块
 * 输	  入：
 * 			
 * 输	  出：
 *			ERR_VCA_SUCCESSFUL				
 * 修改日志：
 * 			2011-03-21,张仪,创建
 */

#if 0 		//zhangyi add 20110707
td_s32 vca_Exit(void);
/*
 * 函数名称：vca_Exit
 * 简要描述: 卸载智能分析模块
 * 输	  入：
 * 			
 * 输	  出：
 *			ERR_VCA_SUCCESSFUL					
 * 修改日志：
 * 			2011-03-21,张仪,创建
 */
#endif

vca_TVCAParam_ex* vca_GetAlarmParam(td_s32 _iChannelNo);
/*
 * 函数名称：vca_GetParam
 * 简要描述: 获取智能分析报警参数
 * 输	  入：
 * 			
 * 输	  出：
 *								
 * 修改日志：
 * 			2012-09-03,蔡兆旭,对外开放
 */


//vca_TVCAParam_ex* vca_GetParam(td_s32 _iChannelNo);
TVcaParam* vca_GetParam(td_s32 _iChannelNo);
/*
 * 函数名称：vca_GetParam
 * 简要描述: 获取单通道智能分析配置参数	
 * 输	  入：
 * 			
 * 输	  出：
 *								
 * 修改日志：
 * 			2011-04-22,张仪,创建
 */

TVcaEventType vca_GetChanRuleEventType(td_s32 _iChannelNo, td_s32 _iRuleID);
/*
 * 函数名称：vca_GetChanRuleEventType
 * 简要描述: 
 * 输	  入：
 * 输	  出：
 * 修改日志：
 *			2011-06-03,张仪，创建
 */


//-------------------------------------------------------------------------------------------------------------
td_s32 vca_SetChannelEnable(td_s32 _iChn, td_s32  _iEnable);
td_s32 vca_GetChannelEnable(td_s32 _iChn, td_s32 *_piEnable);
//-------------------------------------------------------------------------------------------------------------
td_s32 vca_SetVideoInputSize(td_s32 _iChannelNo, td_s32  _iVideoSize);
td_s32 vca_GetVideoInputSize(td_s32 _iChannelNo, td_s32 *_piVideoSize);
//-------------------------------------------------------------------------------------------------------------
td_s32 vca_SetDrawVSize(td_s32 _iChn, td_s32 _iVideoWidth, td_s32 _iVideoHeight);
td_s32 vca_GetDrawVSize(td_s32 _iChn, td_s32 *_iVideoWidth, td_s32 *_iVideoHeight);
//-------------------------------------------------------------------------------------------------------------
td_s32 vca_SetAdvancedParameter(td_s32 _iChn, TVcaAdvanceParam *_pstAdvancedParam);
td_s32 vca_GetAdvancedParameter(td_s32 _iChn, TVcaAdvanceParam *_pstAdvancedParam);
//-------------------------------------------------------------------------------------------------------------
td_s32 vca_SetParameterandColour(td_s32 _iChn, TVcaConfig *_pstConfig);
td_s32 vca_GetParameterandColour(td_s32 _iChn, TVcaConfig *_pstConfig);
//-------------------------------------------------------------------------------------------------------------
td_s32 vca_SetRuleParameter(td_s32 _iChannelNo, td_s32 _iRuleID, char *_cRuleName, td_s32  _iValid);
td_s32 vca_GetRuleParameter(td_s32 _iChannelNo, td_s32 _iRuleID, char *_cRuleName, td_s32 *_iValid);
//-------------------------------------------------------------------------------------------------------------
td_s32 CoordinateVaryformNTSCtoPAL(int iChn, TVcaPoint *pstPoint);
td_s32 CoordinateVaryformPALtoNTSC(int iChn, TVcaPoint *pstPoint);
td_s32 CoordinateVaryFormCurrentSizetoFD1(int _iChn, TVcaPoint *_pstPoint);
td_s32 CoordinateVaryformFD1toCurrentSize(int _iChn, TVcaPoint *_pstPoint);
//-------------------------------------------------------------------------------------------------------------
td_s32 vca_SetTripwireParameter(td_s32 _iChn, td_s32 _iRul, TVcaParamTripwire *_pstVcaTripParam, td_s32 _iType);
td_s32 vca_GetTripwireParameter(td_s32 _iChn, td_s32 _iRul, TVcaParamTripwire *_pstVcaTripParam, td_s32 _iType);
//-------------------------------------------------------------------------------------------------------------
//caizhaoxu add 20130117 设置智能分析跟踪
td_s32 vca_SetCAPParameter(td_s32 _iChn, td_s32 _iRul, TVcaParamCAP *_pstVcaCapParam, td_s32 _iType);
td_s32 vca_GetCAPParameter(td_s32 _iChn, td_s32 _iRul, TVcaParamCAP *_pstVcaCapParam, td_s32 _iType);
td_s32 vca_SetCPCParameter(td_s32 _iChn, td_s32 _iRul, TVcaParamCPC *_pstVcaCpcParam, td_s32 _iType);
td_s32 vca_GetCPCParameter(td_s32 _iChn, td_s32 _iRul, TVcaParamCPC *_pstVcaCpcParam, td_s32 _iType);
td_s32 vca_SetCDDParameter(td_s32 _iChn, td_s32 _iRul, TVcaParamCDD *_pstVcaCddParam, td_s32 _iType);
td_s32 vca_GetCDDParameter(td_s32 _iChn, td_s32 _iRul, TVcaParamCDD *_pstVcaCddParam, td_s32 _iType);
td_s32 vca_SetSvdParameter(td_s32 _iChn, td_s32 _iRul, TVcaParamSVD *_pstVcaSvdParam);
td_s32 vca_GetSvdParameter(td_s32 _iChn, td_s32 _iRul, TVcaParamSVD *_pstVcaSvdParam);


//设置跟踪所使用的PTZ回调函数
td_s32 vca_SetCapPTZCallBack(td_u32 _iChn, td_u32 _iRule, void* _pFuncPtz);
//-------------------------------------------------------------------------------------------------------------
td_s32 vca_SetPerimeterParameter(td_s32 _iChn, td_s32 _iRul, TVcaParamPerimeter *_pstVcaPeriParam, td_s32 _iType);
td_s32 vca_GetPerimeterParameter(td_s32 _iChn, td_s32 _iRul, TVcaParamPerimeter *_pstVcaPeriParam, td_s32 _iType);
//-------------------------------------------------------------------------------------------------------------
//td_s32 vca_SetFaceParameter(td_s32 _iChannelNo, td_s32 _iRuleID, TVcaFaceParam_ex *_pstVcaFaceParam, td_s32 _iType);
//td_s32 vca_GetFaceParameter(td_s32 _iChannelNo, td_s32 _iRuleID, TVcaFaceParam_ex *_pstVcaFaceParam, td_s32 _iType);
//-------------------------------------------------------------------------------------------------------------
td_s32 vca_SetDiagnoseParameter(td_s32 _iChn, td_s32 _iRul, TVcaParamAVD *_pstVcaDiagParam, td_s32 _iType);
td_s32 vca_GetDiagnoseParameter(td_s32 _iChn, td_s32 _iRul, TVcaParamAVD *_pstVcaDiagParam, td_s32 _iType);
//-------------------------------------------------------------------------------------------------------------
td_s32 vca_SetOscParameter(td_s32 _iChn, td_s32 _iRul, TVcaParamOSC * _pstVcaOscParam, td_s32 _iType);
td_s32 vca_GetOscParameter(td_s32 _iChn, td_s32 _iRul, TVcaParamOSC * _pstVcaOscParam, td_s32 _iType);
//-------------------------------------------------------------------------------------------------------------
//td_s32 vca_Reboot(void);
//td_s32 vca_ParaSetAlmCountClr(td_s32 _iChannelNo, td_s32 _iRuleID, TVcaEventType _iEventType);
//td_s32 vca_ParaGetAlmCountTol(td_s32 _iChannelNo, td_s32 _iRuleID, TVcaEventType _iEventType, td_s32 *_iCount);

//-----------------------------------------------------------------------------------
int vca_putAlarmMsg(int _iChn, int _iRule, TVcaEventType _eType, int _iState, unsigned int _u32TId, unsigned int _u32Type);
int vca_getAlarmMsg(void);
//---------------------------------------------------------------------------------

td_s32 vca_ReSetDrawVSize(td_s32 _iChn, td_s32 _iVWSize, td_s32 _iVHSize);

int vca_get_alarm_state(int _iChn, int _iRule, int _iType);

int vca_set_module_reboot(int _iChn);
int vca_get_all_alarm();
int vca_clean_alarm(int _iChn, int _iRuleID, int _iEventType,int _iZeroFlag);

//void vca_get_avd_alarm_count(int _chn, int _rule);

int vca_get_alarm_count(int _chn, int _rule, TVcaAlarmMsg *pAlarMsg); // reopen by caizhaoxu for fix warning

//int vca_impowerlib_state();

int vca_GetVcaModuleEnable();

td_u32 vca_IfCapEnable(td_u32 _iChn, td_u32 _iRule);

//td_s32 vca_VCAAlarmInTimeSegToCore(int _iChn, vca_TRuleParam_ex *_stRuleParam);
//shihc move 20130225
td_s32 vca_capptz_speed(unsigned int _iW, unsigned int _iH);
td_s32 vca_capptz_action(td_s32 _s32Xcoordinate, td_s32 _s32Ycoordinate,td_s32 * _nx,td_s32 * _my,td_s32 * _action);
//shihc add 20130425 增加
td_s32 vca_SetCapPtzPriority(td_s32 _iChn, td_s32 _iPriority);
td_s32 vca_GetCapDelayFlat(td_s32 _iChn);
td_s32 Vca_CoordinateD1ToNet(td_s32 _iChn, TVcaPoint *_pstPoint);
td_s32 Vca_CoordinateNetToD1(td_s32 _iChn, TVcaPoint *_pstPoint);



#endif





