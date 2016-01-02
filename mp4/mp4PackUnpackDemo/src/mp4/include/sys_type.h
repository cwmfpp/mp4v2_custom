/*
 ====================================================================
 Name         : sys_type.h
 Author       : shanbingdong
 Version      : 0.1
 Copyright    : Tiandy
 Description  : 大路数NVR raid/iscsi模块公共头文件
 ====================================================================
 */
#ifndef SYS_TYPEH
#define SYS_TYPEH

#define MAX_PATH 260	/**< 最大路径长度 */
#define MAX_IP_LEN 16	/**< 最大IP地址长度 */
#define MAX_USERNAME_LEN 60	/**< 最大用户名长度 */
#define MAX_PASSWORD_LEN 60	/**< 最大密码长度 */

//RAID级别
#define RAID_0 		0
#define RAID_1 		1
#define RAID_5 		5
#define RAID_10     10
#define RAID_JBOD 	100

#define RAID_SYNC_PRIORITY_LOW      0
#define RAID_SYNC_PRIORITY_MIDDLE   1
#define RAID_SYNC_PRIORITY_HIGH     2

#define DISK_STATUS_OK 				1
#define DISK_STATUS_NO 				0

#define MAX_BIG_DATA_NVR_DISK_NUM 	16//与NVR原先的MAX_DISK_NUM冲突
#define MAX_VIRTUAL_DISK_NUM		16
#define MAX_RAID_NUM 				8
#define MAX_ISCSI_TARGET_NUM  		8
#define MAX_SEARCH_DISK_NUM 		64

#define MAX_MSG_LEN  	128
#define MAX_NAME_LEN	64


//磁盘smart 状态信息
#define DISK_SMART_GOOD		0
#define DISK_SMART_LOWHEALTH	1
#define DISK_SMART_BAD			2

#define UDV_TYPE_NONE     	0
#define UDV_TYPE_RAW      	1
#define UDV_TYPE_ISCSI    	2
#define UDV_TYPE_NAS       	3

#define UDV_CREATE_START_SIZE 209715200	//200M

//报警模块标示
#define MODULE_PHYSICAL_DISK            0
#define MODULE_RAID                     1
#define MODULE_VIRTUAL_PARTITION        2 //暂不支持

/* 报警类型*/
//磁盘状态设置:正常、专用热备盘、全局热备盘、RAID盘、无效RAID盘、故障盘
#define STATUS_TYPE_DISK_FREE     				0
#define STATUS_TYPE_DISK_SPECIAL				1
#define STATUS_TYPE_DISK_GLOBAL			    	2
#define STATUS_TYPE_DISK_RAID     				3
#define STATUS_TYPE_DISK_INVALID				4
#define STATUS_TYPE_DISK_FAIL					5
#define STATUS_TYPE_DISK_REALLOCATE_SECTORS 6	//重新分配扇区计数报警
#define STATUS_TYPE_DISK_PENDING_SECTORS 	7	//不稳定扇区计数报警
#define STATUS_TYPE_DISK_UNCORRECTABLE 		8	//无法修正错误计数报警

//RAID状态:正常(normal)、降级(degrade)、失效(fail)、重建(rebuild、recovering)、初始化(initial)、扩容(reshape)
#define STATUS_TYPE_RAID_NORMAL         100 
#define STATUS_TYPE_RAID_DEGRADED       101
#define STATUS_TYPE_RAID_FAIL           102
#define STATUS_TYPE_RAID_RECOVERING     103
#define STATUS_TYPE_RAID_INITIAL        104
#define STATUS_TYPE_RAID_RESHAPE        105
#define STATUS_TYPE_RAID_ACTIVE         106 

//ISCSI客户端结构体
typedef struct _IscsiDiskInfo {
	int iscsi_id;								//iscsi标示(16-23) ，上层UI唯一标示
	char id_serial[MAX_NAME_LEN];				//iscsi  id_serial，系统kernel唯一标示
	long long int volume; 						//磁盘容量，单位字节
	long long int avalible_volumn;				//可用磁盘容量，单位字节
	int disk_status;							//磁盘状态
	char iscsi_dev_dir[MAX_PATH];				//iscsi设备名 /dev/s(p)db
	char tgt_name[MAX_NAME_LEN];			//target名称
} IscsiDiskInfo;

typedef struct _TargetInfo {
	char tgt_name[MAX_NAME_LEN];			//target名称
	IscsiDiskInfo* disk[MAX_SEARCH_DISK_NUM ];//target对应磁盘指针数组
} TargetInfo;


typedef struct _IscsiInfo {
	char ip[MAX_IP_LEN];  						//ipsan的IP
	int port;									//iscsi映射端口
	char username[MAX_USERNAME_LEN];		//用户名
	char passwd[MAX_PASSWORD_LEN];			//密码
	TargetInfo* target[MAX_ISCSI_TARGET_NUM];//target名指针数组
} IscsiInfo;


//RAID模块结构体
/*****以下功能将要使用RetResult结构体来返回结果信息*****
2.3 设置磁盘状态
3.1 创建卷组、3.2 删除卷组 、3.4 验证重名卷组、3.5 生成卷组名称、3.6 设置卷组初始化/重建优先级
3.7 获取卷组初始化/重建优先级（msg字段的输出为：low|middle|high）、3.8 卷组扩容（仅支持raid5/6）
4.1 创建用户数据卷、4.2 删除用户数据卷、4.3 修改用户数据卷名称
6.1.2 修改Target属性、6.1.3 增加Target、6.1.4 删除Target
*/
typedef struct _RetResult {				//返回结果信息结构体
	int status;				//执行的结果
	char msg[MAX_MSG_LEN];	//执行命令成功的提示或失败原因。
} RetResult;


/*****磁盘列表*****/
typedef struct _DiskInfo {				//磁盘信息
	int slot;						//位置；狭槽   ( x: 0~4  y: 1~16  ==>  0-15, 100-115, 200-215, 300-315, 400-415)
	char model[MAX_NAME_LEN];	//磁盘型号，字符串，由数字和字符构成
	char serial[MAX_NAME_LEN];	//序列号，字母和数字组合
	char firmware[MAX_NAME_LEN];	//固件版本号
	long long int capacity;			//容量，单位字节
	int smart;					//Good – 良好，SMART信息无异常;LowHealth – 健康度低，坏扇区超过某个阀值;(大约5w个坏扇区); Bad – 磁盘故障，例如无法识别；
	int state;					//磁盘管理状态：Free – 空闲盘 Special – 专用热备盘 Global – 全局热备盘 RAID – RAID盘
	char name[MAX_NAME_LEN];	//卷组名称
} DiskInfo;

typedef struct _DiskListInfo {			//磁盘列表信息
	int total;						//磁盘结果条目数量
	DiskInfo row[MAX_BIG_DATA_NVR_DISK_NUM];	//磁盘每个条目的定义
} DiskListInfo;

/*****获取磁盘详细信息*****/
typedef struct _DiskSmartInfo {			//磁盘smart信息
	int	read_err;			//读错误率
	int	spin_up;			//起旋时间
	int	reallocate_sectors;	//重新分配扇区计数
	int	pending_sectors;	//不稳定扇区计数
	int	uncorrectable;		//无法修正错误计数
	int	power_on_hours;	//累计通电时间
	int	temperature;		//温度
} DiskSmartInfo;

typedef struct _DiskDetailInfo {		//磁盘详细信息
	DiskInfo disk_info;		//磁盘信息
	DiskSmartInfo smart_info;	//smart信息
	int bus_type;				//SAS / SATA
	int	rpm;				//常见取值：5400 / 7200 / 10000
	int wr_cache;			//写缓存：enable / disable
	int rd_ahead;			//预读：enable / disable
	int	standby;			//待机模式：经过多少秒磁盘无读写进入待机模式，0表示关闭，即永不进入待机模式。常见取值：30 / 60 / 600 / 1800
	int cmd_queue;			//命令队列：enable / disable
	int	mapped_cnt;		//已修复扇区计数
	int	max_map_cnt;		//最大可修复扇区数	
} DiskDetailInfo;

/*****卷组列表*****/
typedef struct _VGInfo {					//卷组信息
	char name[MAX_NAME_LEN];		//卷组名称
	char dev[MAX_NAME_LEN];			//卷组的设备名称	例如：/dev/md1
	int raid_level;					//取值范围：0 / 1 / 5 /  JBOD
	int raid_state;					//正常（normal）、降级（degrade）、失效（fail）、重建（rebuild）、初始化（initial）、扩容（reshape）
	int raid_strip;					//条带大小	取值范围：4/8/16/32/64/128/256/512/1024，单位KB
	float sync_percent;				//RAID初始化/重建进度	仅当raid_state为rebuild或者initial时有效。百分比形式输出，保留一位小数。
	float sync_speed;					//RAID初始化/重建速度	仅当raid_state为rebuild或者initial时有效。输出保留一位小数，单位：MB/s
	int sync_finish;					//RAID初始化/重建剩余时间	仅当raid_state为rebuild或者initial时有效。输出为整数，单位：分钟 min
	long long int capacity;				//卷组总容量	单位：字节
	long long int remain;				//剩余容量	单位：字节
	int max_single;					//最大连续容量	单位：字节
	int disk_cnt;						//当前RAID成员盘个数	取值范围：1~磁盘阵列支持最大磁盘数
	int disk_total;					//总的RAID成员盘个数	取值范围：1~磁盘阵列支持最大磁盘数
	char raid_uuid[MAX_NAME_LEN];	//卷组内部唯一标识	例如：3a32c2ed:1df8f4d2:a62159fd:8ecc787b
	int disk_list[MAX_BIG_DATA_NVR_DISK_NUM];		//RAID成员磁盘列表	格式：磁盘ID列表，使用空格分割。磁盘号为1-16
} VGInfo;

typedef struct _VGListInfo {			//卷组信息列表
	int total;						//卷组结果条目数量
	VGInfo row[MAX_RAID_NUM];	//卷组每个条目的定义
} VGListInfo;


/*****获取卷组可用空间*****/
typedef struct _VGAvalibaleInfo {		//卷组可用信息
	char vg[MAX_NAME_LEN];		//卷组名称
	long long int max_avaliable;	//可用空间总和	单位：字节
	long long int max_single;		//连续最大可用空间	单位：字节
} VGAvalibaleInfo;

/*****验证重名用户数据卷 (可以不要这个结构体)*****/
typedef struct _VGDup {
	char vg[MAX_NAME_LEN];		//卷组名称
	int duplicate;					//是否重复:true - 表示name已经存在,false - 表示name不存在
} VGDup;

/*****用户数据卷列表*****/
typedef struct _UDVInfo { 				//用户数据卷信息
	char name[MAX_NAME_LEN];	//用户数据卷名称
	long long int capacity;			//容量 单位：字节
	int state;					//状态 取值范围：未使用（raw）、映射为iSCSI卷（iscsi）、映射为NAS卷（nas）
	char vg[MAX_NAME_LEN];		//用户数据卷所在的VG名称	
	char combin[MAX_NAME_LEN];	//用户数据卷名称和容量	
	char dev[MAX_NAME_LEN];		//设备节点路径名	
} UDVInfo;

typedef struct _UDVListInfo {			//用户数据卷列表信息
	int total;							//用户数据卷结果条目数量
	UDVInfo row[MAX_BIG_DATA_NVR_DISK_NUM];		//用户数据卷信息
} UDVListInfo;

/*

//ISCSI服务端结构体
//搜索Target
typedef struct _TargetNameList {
	int total;							//target个数
	char* target_name[MAX_NAME_LEN];	//target名称列表
} TargetNameList;


//获取Target列表
typedef struct _IscsiProtocol {			//iSCSI协议相关的信息
	int HeaderDigest;							//协议头校验方式	目前支持None方式，表示不校验
	int DataDigest;							//数据校验方式	目前支持None方式，表示不校验
	int ImmediateData;						//是否允许携带立即数据	取值：Yes（允许）；No（不允许）
	int InitialR2T;								//是否允许非请求类数据	取值：Yes（允许）；No（不允许）
	long long int FirstBurstLength;				//非请求类数据最大长度	单位：字节
	long long int MaxBurstLength;				//请求类数据最大长度	单位：字节
	long long int MaxOutstandingR2T;				//每个任务可用R2T上限	正整数
	long long int MaxRecvDataSegmentLength;		//PDU数据段最大接收长度	单位：字节
	long long int MaxXmitDataSegmentLength;		//PDU数据段最大发送长度	单位：字节
	int IncommingChap;						//访问Target CHAP认证	取值范围：enable / disable
} IscsiProtocol;

typedef struct _TargetAttribute {		//Target相关属性
	int NopInInterval;			//无操作时间间隔	单位：秒
	int NopInTimeout;			//无操作超时时间	单位：秒
	int RspTimeout;			//响应超时时间	单位：秒
	int MaxSessions;			//最大允许Session个数	正整数（可修改参数）
} TargetAttribute;

typedef struct _TargetInfo {			//Target信息
	int tid;							//Target ID
	char target_name[MAX_NAME_LEN];	//Target名称
	int state;							//状态 取值范围：enable / disable
	int luns;							//已经映射到此Target的iSCSI卷个数	0 ~ 实际数量
	int sessions;						//session数量 0 ~ 实际数量
	IscsiProtocol iscsi_protocal;			//iSCSI协议相关的信息
	TargetAttribute target_attr;			//Target相关属性
	int QueuedCommands;				//在队列中缓存的命令个数 正整数 (statistics	统计信息)
} TargetInfo;

typedef struct _TargetListInfo {			//ISCSI的target列表信息
	int total;						//ISCSI的target结果条目数量
	TargetInfo row[MAX_BIG_DATA_NVR_DISK_NUM];	//target信息
} TargetListInfo;

*/

#endif
