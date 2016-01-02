#ifndef SYS_RAIDH
#define SYS_RAIDH

#ifndef SYS_TYPEH
#include "sys_type.h"
#endif

#define TURE  1
#define FALSE 0

enum
{
	RAID_STAT_NORMAL=1,
	RAID_STAT_DEGRADED,	
	RAID_STAT_FAIL,
};

enum
{
	RAID_TSK_NULL=1,
	RAID_TSK_INIT,	
	RAID_TSK_REBUILD,
};


/*
	函数: sys_raid_startup
	功能: raid初始化
	参数: 无
	返回: 0 成功
		  >0 失败
*/
int 
sys_raid_startup();


/*
	函数: sys_raid_cleanup
	功能: raid反初始化
	参数: 无
	返回: 0 成功
		  >0 失败
*/
int 
sys_raid_cleanup();


/*
	函数: sys_disk_list
	功能: 列出磁盘信息
	参数: disk_info 磁盘信息
		total 磁盘数量
	返回: 0 成功
		  >0 失败
*/
int 
//sys_disk_list( OUT DiskInfo *disk_info, IN OUT int* total);
sys_disk_list( OUT DiskInfo *disk_info, OUT int* total);


/*
	函数: sys_disk_get_detail
	功能: 列出磁盘详细信息
	参数: slot_id 硬盘槽号
		disk_detail_info 磁盘详细信息
	返回: 0 成功
		  >0 失败
*/
int
sys_disk_get_detail(int slot_id, OUT DiskDetailInfo* disk_detail_info);


/*
	函数: sys_disk_set
	功能: iscsi登出
	参数: slot_id 硬盘槽号
		type 磁盘类型
		vg_name 卷组名称
	返回: 0 成功
		  >0 失败
*/
int
sys_disk_set(int  slot_id, int type, const char* vg_name);


/*
	函数: sys_raid_create
	功能: 创建raid/卷组
	参数: vg_name  卷组名称
		level 阵列级别(0,1,5,10,JBOD)
		raid_devices   阵列数据磁盘数组
		raid_dev_num 阵列数据磁盘数量
		spare_devices 阵列热备盘数组
		spare_dev_num 阵列热备盘数量
		chunk 条带大小(默认128KB)
	返回: 0 成功
		  >0 失败
*/
int
sys_raid_create(const char* vg_name, int level, int* raid_devices, int raid_dev_num, int* spare_devices, int spare_dev_num, int chunk);


/*
	函数: sys_raid_delete
	功能: 删除raid/卷组
	参数: vg_name  卷组名称
	返回: 0 成功
		  >0 失败
*/
int
sys_raid_delete(const  char* vg_name);


/*
	函数: sys_raid_list
	功能: 获取raid列表
	参数: vg_info 卷组信息
		total 卷组个数
	返回: 0 成功
		  >0 失败
*/
int
sys_raid_list(OUT VGInfo * vg_info,  OUT int* total);


/*
	函数: sys_raid_assemble
	功能: 手动重建卷组
	参数: vg_name:卷组名称
		 old_disk_list：已有磁盘列表
		 old_disk_num:已有磁盘数量
		 new_disk_list：新加磁盘列表
		 new_disk_num：新加磁盘数量
	返回: 0 不存在
		  >0 有问题
*/
int 
sys_raid_assemble(const char* vg_name, int* old_disk_list, int old_disk_num, int* new_disk_list, int new_disk_num);

/*
	函数: sys_raid_duplicate_check
	功能: 验证重命名卷组
	参数: vg_name  卷组名称
	返回: 0 成功(说明raid名称已经存在)
		  >0 失败
*/
int
sys_raid_duplicate_check(const  char* vg_name);


/*
	函数: sys_raid_generate_name
	功能: 生成卷组名称
	参数: suffix 要生成的卷组名称前缀
		vg_name  卷组名称
	返回: 0 成功
		  >0 失败
*/
int
sys_raid_generate_name(const char* suffix,  OUT char* vg_name);


/*
	函数: sys_raid_set_sync_prio
	功能: 设置卷组初始化/重建优先级
	参数: init_level 初始化级别
	返回: 0 成功
		  >0 失败
*/
int
sys_raid_set_sync_prio(int init_level);


/*
	函数: sys_raid_get_sync_prio
	功能: 获取卷组初始化/重建优先级
	参数: init_level 初始化级别
	返回: 0 成功
		  >0 失败
*/
int
sys_raid_get_sync_prio(OUT int* init_level);


/*
	函数: sys_udv_create
	功能: 创建用户数据卷
	参数: vg_name  卷组名称
		udv_name   用户数据卷(虚拟分区)名称
		offset  用户数据卷偏移量,起始大小为209715200Byte(200M),做4K对齐
		size 用户数据卷大小,两个分区中间有小于1G的空间，就认为是碎片，隐藏
	返回: 0 成功
		  >0 失败
*/
int
sys_udv_create(const char* vg_name, const char* udv_name, long long int offset, long long int size);


/*
	函数: sys_udv_delete
	功能: 删除用户数据卷
	参数: vg_name  卷组名称
	返回: 0 成功
		  >0 失败
*/
int
sys_udv_delete(const  char*  udv_name);


/*
	函数: sys_udv_remain_capacity
	功能: 获取卷组可用空间
	参数: vg_name  卷组名称
		vg_avalibale_info 卷组可用信息
	返回: 0 成功
		  >0 失败
*/
int
sys_udv_remain_capacity(const  char*  vg_name,  OUT  VGAvalibaleInfo* vg_avalibale_info);


/*
	函数: sys_udv_list
	功能: 用户数据卷列表
	参数: udv_info 用户卷列表信息
		total 用户数据卷数量
	返回: 0 成功
		  >0 失败
*/
int
sys_udv_list(OUT UDVInfo * udv_info, OUT int* total);

/*
	函数: sys_udv_assemble
	功能: 删除原先虚拟磁盘软连接，获取磁盘列表信息，重新进行建立
	参数: 
	返回: 0 成功
		  >0 失败
*/
int
sys_udv_assemble();

/*
	函数: RECV_ALARM_NOTIFY
	功能: 函数指针
	参数: module  模块名称 物理磁盘 0 ，RAID 1 ， (虚拟分区 2,暂不支持)
		type 报警事件类型:物理磁盘损坏、RAID降级等
		param   指定报警类型的数据指针
		usr 用户数据
*/
typedef int (*RECV_ALARM_NOTIFY)(int module, int type, void* param, void* usr);

/*
	函数: sys_alarm_set_callback
	功能: 设置报警回调
	参数: module  模块名称 物理磁盘 0 ，RAID 1 
		alarm_callback   报警回调函数
		usr 用户数据
	返回: 0 成功
		  >0 失败
*/
int sys_alarm_set_callback(int module, RECV_ALARM_NOTIFY alarm_callback, void* usr);
#if JMB39X == ON
int sys_raid_mapPort(int _iDisk);
void sys_raid_init(void);
void raid_JMB393_ScanDisc(int _iDisk);
#endif

#endif
