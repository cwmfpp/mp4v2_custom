#ifndef __VCA_HEAD_H__
#define __VCA_HEAD_H__

#define VCA_OAB_FLAG 0
#define VCA_LPR_FLAG 0

#define	MAX_POLYGON_POINT_NUM		8
#define	MAX_CPC_POINT_NUM			4
#define	MAX_RULE_NUM				8
#define	MAX_RULE_NAME_LEN			16
#define	MAX_SAVE_TARGET_NUM		16
#define	MAX_ALARM_NUMBER		20000
#define MAX_PARAM_SIZE			2048		/*联合体参数数据最大时为2048个字节*/
#define MAX_DATA_SIZE			2048		/*智能分析上传主程序的参数大小*/
#define MAX_IGNORE_REGION_NUM	3

#define TD_VCA_SUCCESS          0
#define TD_VCA_FAILURE          (-1)

#define	TD_LICENSE_CHECK_FAILE		0x0101			/*MAC地址校验失败*/
#define	TD_LICENSE_NO_FILE			0x0102			/*没有库加密文件*/
#define	TD_DLOPEN_ERROR				0x0103			/*没有库加密文件*/

#define	TD_VCA_DISABLE				0x0201			/*相应通道的智能分析功能未打开*/
#define	TD_VCA_DRAW_SIZE_ERROR		0x0202			/*叠加画线的图像尺寸与初始化的尺寸不一致*/
#define	TD_VCA_ANALYSE_SIZE_ERROR	0x0203			/*分析图像的尺寸与初始化的尺寸不一致*/
#define	TD_VCA_CREAT_IMAGE_FAILE	0x0204			/*创建图像失败*/
#define	TD_VCA_INIT_PEA_FAIL		0x0205			/*PEA初始化失败*/
#define	TD_VCA_INIT_OSC_FAIL		0x0206			/*OSC初始化失败*/
#define	TD_VCA_INIT_AVD_FAIL		0x0207			/*AVD初始化失败*/
#define TD_VCA_NO_ANALYSE			0x0208			/*该通道的分析线程未创建，禁止reload。*/
#define TD_VCA_HAVE_NOT_INIT		0x0209			/*该通道的智能分析还没有初始化*/
#define TD_VCA_REINIT				0x020a			/*该通道智能分析重复初始化*/
#define TD_VCA_NO_PTZ_FUNCTION		0x020b			/*控制球机的PTZ 回调函数指针为空*/
		/*预留如干项，为将来添加新的智能分析功能保留*/

#define	TD_VCA_PARAM_ERROR			0x0901			/*函数参数不正确*/

#define ENABLE_PEA			0x01
#define ENABLE_OSC			0x02
#define ENABLE_AVD			0x04
#define ENABLE_CAP			0x08
#define ENABLE_CAP_td		0x10
#define ENABLE_CDD			0x20
#define ENABLE_CPC			0x40
#define ENABLE_VFD			0x80
#define ENABLE_SVD			0x100
#if VCA_OAB_FLAG
#define ENABLE_OAB			0x200	/*逆行- 双伴线- 徘徊*/
#endif

#if VCA_LPR_FLAG
#define ENABLE_LPR			0x400
#endif

/*坐标点结构体*/
typedef struct
{
	int	m_iX;
	int	m_iY;
} TVcaPoint;

/*直线结构体*/
typedef struct
{
	TVcaPoint 	m_stStart;
	TVcaPoint 	m_stEnd;
}TVcaLine;

typedef struct
{
	int m_s32X1;
	int m_s32Y1;
	int m_s32X2;
	int m_s32Y2;
}TVcaRect;

/*多边形结构体*/
typedef struct
{
	int			m_iPointNum;
	TVcaPoint	m_stPoint[MAX_POLYGON_POINT_NUM];   
}TVcaPolygon;

/*矢量坐标*/
typedef struct
{
	int     flag;			/**获取球机状态：
			1: 表示刚回到预置位; 0表示运动中;
			2:表示球机跟踪刚停下来; 3:表示回预置位过程中;4:手控*/
							/**设置球机状态：
			0:球机不动; 1:球机根据以下坐标运动*/
	float	m_s32X1;		/**上下方向运动坐标*/
	float	m_s32Y1;		/**左右方向运动坐标*/
	float	m_s32Z1;		/**倍数*/

	int		m_s32Speed;	/*速度*/
}TVcaMovVector;

typedef struct		/*视频原始数据的参数*/
{
	int				m_iPixelFormat;		/*YUV格式*/
	int				m_iWidth;			/*YUV图像的宽度*/
	int				m_iHeight;			/*YUV图像的高度*/
	int				m_iStride;			/*YUV图像的跨度*/
	unsigned char*	m_u8Data;			/*YUV图像数据*/
	unsigned int*	m_u32PhyAddr;		/*YUV物理地址,留作以后扩展*/
}TVcaFrameParma;

/*颜色枚举*/
typedef enum
{
	VCA_COLOR_red	= 1,	
	VCA_COLOR_green,			
	VCA_COLOR_yellow,		
	VCA_COLOR_blue,			
	VCA_COLOR_magenta,		
	VCA_COLOR_cyan,		
	VCA_COLOR_black,			
	VCA_COLOR_white			
}TVcaColor;

typedef struct
{
	TVcaPoint	m_sTripwireArrow[4];
	TVcaPoint	m_sPerimeterArrow[4];
}TVcaRule;

typedef enum
{
	YUV_PLANAR_420 = 1,
	YUV_PLANAR_422,
	YUV_PLANAR_444
}EVcaPixelFormat;

typedef enum  
{
	VCA_TRIPWIRE = 0,		/* 绊线事件*/
	VCA_DBTRIPWIRE,			/* 双绊线事件*/
	VCA_PERIMETER,			/* 周界事件*/
	VCA_LOITER,				/* 徘徊事件*/
	VCA_PARKING,			/* 停车事件*/
	VCA_RUN,				/* 奔跑事件*/
	VCA_HIGH_DENSITY,		/* 人员密度事件*/
	VCA_ABANDUM,			/* 遗弃物事件*/
	VCA_OBJSTOLEN,			/* 被盗物事件*/
	VCA_FACESPOT,           /* 人脸识别*/
	VCA_AVD,				/*视频诊断*/
	VCA_CAP,				/*目标跟踪*/
	VCA_CAP_td,				/*td目标跟踪*/
	VCA_CDD,				/*人员聚集*/
	VCA_CPC,				/*人流统计*/
	VCA_VFD,				/*人脸检测*/
	VCA_SVD,				/*离岗检测*/
#if VCA_OAB_FLAG
	VCA_OAB_RTO,			/*逆行*/
	VCA_OAB_BTP,			/*双伴线*/
	VCA_OAB_LOI,			/*徘徊*/
#endif

#if VCA_LPR_FLAG
	VCA_LPR,				/*车牌识别*/
#endif
	
	VCA_MAX
} TVcaEventType;

typedef enum	/*球机控制标识,vca库想要控制球机,虚通过回调函数,此标识告诉
				回调函数需要做什么*/
{
	CAP_SET_DOME_STATUS		= 0x00000000,		/**< 设置球机状态和坐标*/
	CAP_GET_DOME_STATUS		= 0x00000001,		/**< 获取球机状态和坐标*/

	CAP_SET_DOME_TO_PRESET	= 0x00000002,		/**< 设置球机到预置位*/	
	CAP_ENABLE_DOME			= 0x00000003,		/**< 使能球机跟踪*/
	CAP_DISABLE_DOME			= 0x00000004		/**< 非使能球机跟踪*/
}TVcaCapControlFlag;

typedef struct
{
	int				s32DetPanRange;
	int				s32DetTiltRange;
	unsigned char	cVelocityStatus;	//0:disable   0xff:enable
	int				s32PanVelocity;
 	int				s32TiltVelocity;
}TVcaPTZContrule;

typedef struct vcaCapSpeedResult
{
	int sUseSpeedCalc;
	int sXSpeed;
	int sYSpeed;
}TvcaCapSpeedResult;

typedef struct vcaCapResult
{
	int s32TargetStatus;					//Target Status.	-1锛歀ost 0锛歋earching  1锛歍racking
	TVcaPTZContrule stPtzResult;			//Information about PTZ Control. valid only when s32TargetStatus=1.
	int	s32ReservedResult1;				//Reserved Result output
	unsigned int u32ReservedResult2;				//Reserved Result output
	TvcaCapSpeedResult stSpeedResult;
	unsigned int m_iTargetW;
	unsigned int m_iTargetH;
}TVcaCapResult;

//回调函数
typedef int (* VcaContrlCapStatus)(TVcaCapResult *_VcaCapResult);
typedef int (* VcaContrlCap_td)(TVcaMovVector* _pVcaMovVector, TVcaCapControlFlag _iFlag);
typedef int (* VcaEraseFog)(int _iFlag);

typedef struct		/* 单绊线参数*/
{
	int				m_iStructSize;			/*该结构体的字节数，包括该参数本身*/
	int				m_iTargetTypeCheck;		/*检测类型: 1-人、2-车、3-人车、0-无限制*/
	int				m_iType;				/*0-单向绊线或1-双向绊线*/
	int				m_iTDirection;			/*绊线报警方向*/
	int				m_iMinDistance;			/*设置检测最小像素: 默认0*/
	int				m_iMinTime;				/*报警时间: 默认0*/
	int				m_iDisplayRule;			/*是否显示规则线: 0 - 不显示	1-显示 */
	int				m_iDisplayStat;			/*是否显示报警计数: 0 - 不显示	1-显示*/
	int				m_iAlarmCount;			/*报警计数: 该个数将在屏幕上显示*/
	int				m_iColor;				/*未报警时规则线颜色*/
	int				m_iAlarmColor;			/*报警时的规则线颜色*/
	TVcaLine		m_stLine;				/**/
}TVcaParamTripwire;

typedef struct		/*周界参数*/
{
	int				m_iStructSize;			/*该结构体的字节数，包括该参数本身*/
	int				m_iTargetTypeCheck;		/*检测类型: 1-人、2-车、3-人车、0-无限制*/
	int				m_iMode;				/*将侧模式: 入侵、进入、离开*/
	int				m_iType;				/*有无方向*/
	int 			m_iDirection;       	/*方向限制*/
	int				m_iMinDistance;			/*报警最小像素距离: 默认0	*/
	int 			m_iMinTime;				/*报警最短时间: 默认0*/
	int				m_iDisplayRule;			/*是否显示规则线: 0 - 不显示	1-显示 */
	int				m_iDisplayStat;			/*是否显示报警计数: 0 - 不显示	1-显示*/
	int				m_iAlarmCount;			/*报警计数: 该个数将在屏幕上显示*/
	int				m_iColor;           	/*未报警时规则线颜色*/
	int				m_iAlarmColor;      	/*报警时的规则线颜色*/
	TVcaPolygon		m_stRegion;	
}TVcaParamPerimeter;

typedef struct		/*物品看护参数*/
{
	int				m_iStructSize;		/*该结构体的字节数，包括该参数本身*/
	int				m_iSceneType;		/*场景类型  0室外 1室内*/
	int				m_iCameraType;		/*相机类型  0普通相机 1针孔相机*/
	int				m_iDisplayRule;		/*是否显示规则线: 0 - 不显示	1-显示 */
	int				m_iDisplayStat;		/*是否显示报警计数: 0 - 不显示	1-显示*/
	int				m_iAlarmCount;		/*报警计数: 该个数将在屏幕上显示*/
	int				m_iColor;			/*未报警时规则线颜色*/
	int				m_iAlarmColor;		/*报警时的规则线颜色*/
	int				m_iTimeMin;    		/*报警检测时间，物品遗失时间必须要大于此阀值*/
	int				m_iSizeMin;			/*被检测物体的最小像素数*/
	int				m_iSizeMax;			/*被检测物体的最大像素数*/
	TVcaPolygon		m_stMainRegion;		/*检测区域结构*/
	TVcaPolygon		m_stIgnoreRegion[MAX_IGNORE_REGION_NUM]; 	/*忽略检测的小框*/
}TVcaParamOSC;

typedef struct		/*视频诊断AVD 参数*/
{
	int				m_iStructSize;			/*该结构体的字节数，包括该参数本身*/
	int				m_iRuleEvevtid;		/* 视频诊断规则标识，按位与协议文档相对
											噪声诊断 	0x0000000001
											清晰度诊断	0x0000000010
											亮度诊断	0x0000000100 
											往后依次为偏色诊断、画面冻结诊断、信号缺失诊断
											场景变换诊断 人为干扰诊断 PTZ失控诊断*/
	int				m_iAlarmCount;      /*报警计数: 该个数将在屏幕上显示*/
	int				m_iDisplayStat;
	int				m_iCheckTime;
} TVcaParamAVD;

typedef struct		/*物体跟踪CAP 配置参数*/
{
	int				m_iStructSize;		/*该结构体的字节数，包括该参数本身*/
	int				m_s32Mode;			/*大中小场景*/
	int				m_s32Color;		/*未报警时规则线颜色*/
	int				m_s32AlarmColor;	/*报警时的规则线颜色*/	
	int				m_s32PresetNum;	/*起始位置编号*/
	int				m_s32ShowPresetNum;	/*需要显示的起始位置编号*/
	TVcaPolygon		m_stRegion;			/*检测区域结构*/
	VcaContrlCapStatus	m_pVcaContrlCapStatus;	/*控制球机转动的回调函数*/
	VcaContrlCap_td		m_pVcaContrlCap_td;		/*td控制球机转动的回调函数*/
	int             m_s32ZoneSet;       /*跟踪区域设置0代表全屏跟踪1代表跨线跟踪2代表区域跟踪*/
	TVcaLine		m_stLine;			/*检测跨线结构*/
}TVcaParamCAP;

typedef struct 
{
	TVcaPoint m_LeftUp;
	TVcaPoint m_RightDown;
}TVcaRect_Ex;

typedef struct		/*人员聚集CDD配置参数*/
{
	int				m_iStructSize;		/*结构体大小*/
	int				m_iSensitivity;		/*灵敏度*/
	int				m_iFlag;			/*暂时保留*/
	int				m_iRegionNum;		/*区域个数*/
	//TVcaRect		m_stRegion[4];		/*检测区域,最大4个*/
	TVcaRect_Ex		m_stRegion[4];		/*检测区域,最大4个*/
}TVcaParamCDD;

typedef struct		/*流量统计CPC配置参数*/
{
	int				m_iStructSize;		/*结构体大小*/
	int				m_iSensitivity;		/*灵敏度*/
	int				m_iPointNum;		/*不规则线段点的个数,最多4个*/
	int				m_iAlarmCount;		/*报警计数*/
	TVcaPoint		m_stPoint[MAX_CPC_POINT_NUM];
}TVcaParamCPC;

typedef struct		/*人脸识别VFD配置参数*/
{
	int				m_iStructSize;		/*结构体大小*/
	int				m_iRegionNum;
	TVcaRect_Ex		m_stRegion[4];		/*检测区域,最大4个*/
}TVcaParamVFD;

typedef struct		/*离岗检测SVD配置参数*/
{
	int				m_iStructSize;		/*结构体大小*/
	int				m_iAlertTime;
	int				m_iClearTime;
	int				m_iSensitivity;
	int				m_iAlarmCount;
	int				m_iColor;			/*未报警时规则线颜色*/
	int				m_iAlarmColor;		/*报警时的规则线颜色*/
	int				m_iDisplayRule;		
	int				m_iDisplayStat;
	int				m_iRegionNum;
	TVcaRect_Ex		m_stRegion[4];		/*检测区域,最大4个*/
}TVcaParamSVD;

#if VCA_OAB_FLAG
typedef struct		/*徘徊检测配置参数*/
{
	int				m_iStructSize;		/*结构体大小*/
	int				m_iMinTime;			/*最小时间*/
	int				m_iAlarmCount;		/*报警计数: 该个数将在屏幕上显示*/
	TVcaPolygon		m_stRegion;	
}TVcaParamOabLoi;
#endif

#if VCA_LPR_FLAG
typedef struct		/*车牌识别配置参数*/
{
	int				m_iStructSize;		/*结构体大小*/
	int				m_iRegionNum;
	TVcaRect_Ex		m_stRegion[4];		/*检测区域,最大4个*/
}TVcaParamOabLpr;
#endif
typedef struct
{
	unsigned short	lb_x;
	unsigned short	lb_y;
	unsigned short	ru_x;
	unsigned short	ru_y;
}vca_TRECT;
//用户数据中保存的信息
typedef struct
{
	vca_TRECT		rect;		/*左上、右下坐标*/
	unsigned short	type;		/*类型，人、车等*/
	unsigned short	velocity;	/*速度*/
	unsigned short	direction; 	/*方向*/
	unsigned short	id;			/*目标ID*/
}vca_TTargetInfo;
typedef struct
{
	unsigned short	target_num;	/*目标个数*/
	vca_TTargetInfo	targets[MAX_SAVE_TARGET_NUM]; /*目标信息*/
}vca_TTargetSet;

typedef struct		/*除雾功能*/
{
	int 	m_iFlag;	/*保留参数*/
}TVcaParamEFG;

/**********           行为分析事件参数        **********/
typedef union
{
	TVcaParamTripwire	m_stTripwire;	/* 穿越绊线参数*/
	TVcaParamPerimeter	m_stPerimeter;	/* 周界参数*/
	TVcaParamOSC		m_stoscer;		/*物品看护参数*/
	TVcaParamAVD		m_stDiagEvent;	/*视频诊断参数*/
	TVcaParamCAP		m_stCap;		/*自动目标跟踪*/
	TVcaParamEFG		m_stEfg;		/*除雾功能*/
	TVcaParamCDD		m_stCdd;		/* 人员聚集*/
	TVcaParamCPC		m_stCpc;		/* 流量统计*/
	TVcaParamVFD		m_stVfd;		/* 人脸检测*/
	TVcaParamSVD		m_stSvd;		/* 离岗检测*/
#if VCA_OAB_FLAG
	TVcaParamOabLoi		m_stOabLoi;		/* 徘徊*/
#endif

#if VCA_LPR_FLAG
	TVcaParamOabLpr		m_stOabLpr;		/*车牌识别*/
#endif
				/*该联合体的最大空间。以后添加的智能分析总参数不能超过该类型大小*/
	char				m_s8MaxSize[MAX_PARAM_SIZE];	
}UVcaEventParam;		

typedef struct			/*基本配置参数*/
{
	int		m_iAnalyseVideoWidth;			/*分析图像的宽度*/
	int		m_iAnalyseVideoStride;		/*分析图像的跨度*/
	int		m_iAnalyseVideoHeight;			/*分析图像的高度*/
	int		m_iTargetMainVideoWidth;		/*主码流图像的宽度*/
	int		m_iTargetMainVideoStride;	/*主码流图像的跨度*/
	int		m_iTargetMainVideoHeight;		/*主码流图像的高度*/
	int		m_iTargetSubVideoWidth;		/*副码流图像的宽度*/
	int		m_iTargetSubVideoStride;		/*副码流图像的跨度*/
	int		m_iTargetSubVideoHeight;			/*副码流图像的高度*/
	int		m_iDisplayTarget;		/* 是否叠加目标框*/
	int		m_iDisplayTrace;		/* 是否叠加轨迹线*/
	int		m_iTraceLength;			/* 轨迹长度*/
	int		m_iTargetColor;			/* 目标框颜色*/
	int		m_iTargetAlarmColor;	/* 报警时目标框颜色*/
}TVcaConfig; // 智能分析配置参数  

typedef struct
{
	int					m_iRuleID;						/*规则号*/
	char				m_cRuleName[MAX_RULE_NAME_LEN];	/* 规则名字*/
	int					m_iValid;				/* 该规则是否生效*/
	TVcaEventType		m_eEventType;			/* 行为分析事件类型 */
	UVcaEventParam		m_stEventParam;			/* 行为分析事件参数*/
}TVcaRuleParam;   /* 规则设置参数*/

typedef struct 
{
	int		m_iEnableAdvanced; 	/*是否启用高级参数*/
	int		m_iMinSize; 		/*目标最小像素数*/
	int		m_iMaxSize; 		/*目标最大像素数*/
	int		m_iMinWidth;  		/*最小宽度*/
	int		m_iMaxWidth;  		/*最大宽度*/
	int		m_iMinHeight;  		/*最小高度*/
	int		m_iMaxHeight;  		/*最大高度*/
	int		m_iTargetMinSpeed;	/*目标最小像素速度(-1为不做限制) */
	int		m_iTargetMaxSpeed;	/*目标最大像素速度(-1为不做限制) */
	int 	m_iMinWHRatio;		/*最小长宽比。银瀑已取消此参数*/
	int 	m_iMaxWHRatio;		/*最大长宽比。银瀑已取消此参数*/
	int		m_iSensitivity;		/*灵敏度级别*/
}TVcaAdvanceParam;		/* 智能分析高级参数*/

typedef struct
{
	int					m_iEnable;		/* 该参数无效，未被使用*/
	int 				m_iEnVca;		/*使能了哪种智能分析，按位使能，0-关闭	1-使能。
											该变量由主函数控制，决定使能哪种智能分析。
											每一位表示的智能分析类型分别为为，0-PEA	  1-OSC    2-AVD	*/
	int					m_iChn;			/* 智能分析通道号*/
	
	TVcaConfig			m_stConfig;		/* 智能分析配置参数  */
	
  	TVcaRuleParam		m_stRule[MAX_RULE_NUM];		/* 行为规则设置参数*/
	
  	TVcaAdvanceParam	m_stAdvancedParam;			/* 智能分析高级参数*/
}TVcaParam;

//报警信息 用于business层给客户端发送报警消息
typedef struct
{
	unsigned int	m_iTargetId;			/*目标id*/
	int				m_iTargetType;			/*1- 人 2- 物（其他） 3-车*/
	int				m_iTargetSpeed;			/*目标速度*/
	int				m_iTargetDirection;		/*目标运动方向*/
	TVcaRect		m_stPosition;			/*目标的位置*/
}TPeaOscTargetInfo;

typedef struct
{
	int						m_s32AlarmTargetNum;	/*当前报警目标的个数*/
	TPeaOscTargetInfo		m_stAlarmTargetInfo[MAX_SAVE_TARGET_NUM];		/*报警目标信息*/
}TVcaPeaOscAlmInfo;/*OAB也用此结构*/

typedef enum	/*跟踪状态，算法当前状态*/
{
	ATF_FOLLOW_START         = 0x00000000,		/**< 跟踪初启动*/
	ATF_FOLLOW_RUNING        = 0x00000001,		/**< 跟踪运行中*/	
	ATF_FOLLOW_POZITION      = 0x00000002		/**< 重回预置位*/	
}TVcaCapStatus;

typedef struct		/*智能球跟踪算法的数据结构*/
{
	//int					m_iTargetFlag;				/*是否叠加目标框1-叠加  !1-不叠加*/
	//TVcaCapStatus		m_stStatus;			/*返回算法当前的状态*/
	TVcaMovVector		m_stMovVector;		/*如果需要镜头运动，镜头的运动方式*/
	TVcaRect			m_stRect;
}TVcaCapAlmInfo;

typedef struct		/*cdd 报警信息*/
{
	char m_iStatusBuf[4][32];	/*记录人多人少*/
}TVcaCddAlmInfo;

typedef struct
{
    unsigned int m_iInNum;
    unsigned int m_iOutNum;
}TVcaCpcAlmInfo;

typedef struct
{
	TVcaEventType			m_eEventType;			/*事件类型*/
	int						m_s32AlarmStatus;		/*报警状态。0-消警1-报警2-(CDD时报警级别1-MIDD  2-HIGH)*/
													/*事件类型为视频诊断时,按位表示报警状态
													*噪声诊断 		0x0000000001
													*清晰度诊断	0x0000000010
													*亮度诊断		0x0000000100 
													*往后依次为偏色诊断、画面冻结诊断、信号缺失诊断
													*场景变换诊断 人为干扰诊断 PTZ失控诊断*/
	int						m_s32AlarmCount;		/*截止当前已累加的真实报警计数*/
	unsigned char			m_u8Data[MAX_DATA_SIZE];			/*该数据要根据不同的报警类型强制转换。*/
}TVcaRuleAlarmInfo;

typedef struct
{
	TVcaRuleAlarmInfo	m_stAlarmRuleInfo[MAX_RULE_NUM];
}TVcaAlarmMsg;

/**************************************************************************************************************
*名称：td_vca_DrawTarget
*功能：调用该函数将在YUV 数据上叠加画线，需要告知YUV 数据的地址大小等相应信息。
			注: 这里的通道号必须为一个已经进行智能分析初始化的通道。否则会提示
			你想要画线的通道还没有对智能分析进行初始化。
*输入：	_iChn :		通道号			_pstFrameParam : 视频图像参数
			_pstFrameAND, _pstFrameSUB : IVE 画线
			_iHardOrSoft : 画线方式。1-IVE画线		0-软件画线
*输出: 无
*返回值:TD_VCA_HAVE_NOT_INIT		TD_VCA_DRAW_SIZE_ERROR		TD_VCA_SUCCESS
**************************************************************************************************************/
int td_vca_DrawTarget(int _iChnId, TVcaFrameParma* _pstFrameParam,
			TVcaFrameParma* _pstFrameAND, TVcaFrameParma* _pstFrameSUB, int _iHardOrSoft);

/****************************************************
*名称：td_vca_AnalyseSource
*功能：调用该函数会将视频YUV 数据送智能分析。
			注: 这里的通道号必须为一个已经进行智能分析初始化的通道。否则会提示
			你想要画线的通道还没有对智能分析进行初始化。
*输入：_iChn:通道号			_iWidth:视频宽度			_iHeight:视频高度
		    _pFrame:当前帧数据	_ibufsize:帧大小			_iFrameNum:帧序号
*返回值: TD_VCA_SUCCESS			
****************************************************/
int td_vca_AnalyseSource(int _iChnId, TVcaFrameParma* _pstFrameParam);

/**************************************************************************************************************
*名称：td_vca_Init
*功能：校验智能分析加密文件，检测是能分析可否运行。
			如果返回失败，则不能对任何视频通道进行智能分析初始化。
*参数：无
*返回值: TD_VCA_SUCCESS		TD_LICENSE_CHECK_FAILE		TD_LICENSE_NO_FILE
***************************************************************************************************************/
int td_vca_Init(void);

/***************************************************************************************************************
*名称：td_vca_Creat
*功能：智能分析内核初始化，每初始化一个通道要调用该函数一次。
*参数：_pstVCAParam:智能分析配置参数
			_iChnNo : 智能分析通道号。
*返回值: TD_VCA_SUCCESS		TD_LICENSE_NO_FILE		TD_LICENSE_CHECK_FAILE
**************************************************************************************************************/
int td_vca_Create(int _iChnId, TVcaParam* _pstVCAParam);

/*************************************************************************************************************
*名称：td_vca_Destroy
*功能：将某个通道的智能分析去初始化。
*参数：_iChnNo : 去初始化的通道号
*返回值: TD_VCA_SUCCESS		TD_VCA_HAVE_NOT_INIT
*************************************************************************************************************/
int td_vca_Destroy(int _iChnId);

/*************************************************************************************************************
*名称：td_vca_Reload
*功能：在重设规则后调用此函数将会重新加载智能分析规则。
*参数：_iChnNo : 重新加载的通道号
*返回值: TD_VCA_SUCCESS		TD_VCA_HAVE_NOT_INIT		TD_VCA_NO_ANALYSE
*************************************************************************************************************/
int td_vca_Reload(int _iChnId, TVcaParam* _pstVCAParam);


/*************************************************************************************************************
*名称：td_vca_GetUserData
*功能：智能分析私有数据获取
*参数：_iChn : 通道号		_cBuffer :  缓存地址	iBufferLen : 数据长度
*返回值:  TD_VCA_SUCCESS		TD_VCA_HAVE_NOT_INIT		TD_VCA_NO_ANALYSE
***************************************************************************************************************/
int td_vca_GetUserData(int _iChnId, unsigned char* _pcBuffer, int* _iBufferLen);

/*************************************************************************************************************
*名称：td_vca_StopDraw
*功能：停止划线,改分辨率时使用,前提是划线位置没变,
			库里面默认是划线的,不改分辨率没有任何影响
*参数：_iChn : 通道号
*返回值:  TD_VCA_SUCCESS		TD_VCA_HAVE_NOT_INIT		TD_VCA_NO_ANALYSE
***************************************************************************************************************/
int td_vca_StopDraw(int _iChnId, TVcaParam* _pstVCAParam);

/*************************************************************************************************************
*名称：td_vca_StartDraw
*功能：开启划线,针对td_vca_StopDraw接口
*参数：_iChn : 通道号		
*返回值:  TD_VCA_SUCCESS		TD_VCA_HAVE_NOT_INIT		TD_VCA_NO_ANALYSE
***************************************************************************************************************/
int td_vca_StartDraw(int _iChnId, TVcaParam* _pstVCAParam);

/**************************************************************************************************************
*名称：td_vca_GetAlarmInfo
*功能：获取智能分析报警数据
*参数：_iChnNo : 通道号
*返回值: 
***************************************************************************************************************/
TVcaAlarmMsg*  td_vca_GetAlarmInfo(int _iChnId);


/***************************************************************************************************************
*名称：td_vca_SetAlarmInfo
*功能：设置智能分析模块报警计数累加初值。
*参数：_pstVCAParam:智能分析配置参数			_iRuleId : 规则号
*返回值: TD_VCA_HAVE_NOT_INIT		TD_VCA_SUCCESS
****************************************************************************************************************/
int td_vca_SetAlarmCount(int _iChnId, int _iRuleId, int _iCount);

/*
* 比较安全的获取报警信息接口。
*/
int td_vca_GetAlarmInfo_s(int _iChnId, TVcaAlarmMsg *stVcaAlarmMsg);

#endif
