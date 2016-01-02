/*******************************************************************
文件名	:td_errno.h
公司	:天津天地伟业数码科技有限公司
创建者	:陶永亮
创建日期:2008-8-28
简要描述:TD错误码定义
修改记录:2008-08-28,陶永亮，创建
		 2009-10-29,陶永亮，针对新模块添加
        
******************************************************************/

#ifndef TD_ERRNO_H_
#define TD_ERRNO_H_

#define TD_SUCCESS						0				//成功
#define TD_FAILURE						(-1)			//失败
#define TD_TRUE							1				//真
#define TD_FALSE						0				//假
#define TD_ENABLE						1				//使能
#define TD_DISABLE						0				//不使能

//存储模块错误码
#define	ERR_REC_BASE					1000			//存储模块错误码起始码
#define ERR_REC_SUCCESSFUL				ERR_REC_BASE+1	//开始录像成功
#define ERR_REC_OTHER_RECORDING 		ERR_REC_BASE+2	//当前该通道正在进行其他类型录像,或该类型录像
#define ERR_REC_DISK_FULL				ERR_REC_BASE+3	//磁盘满
#define ERR_REC_NO_DISK					ERR_REC_BASE+4	//无可用磁盘
#define ERR_REC_DISABLED_TYPE			ERR_REC_BASE+5	//该录像类型未使能
#define ERR_REC_PORT_DISABLED			ERR_REC_BASE+6	//端口报警录像未使能
#define ERR_REC_TIMING_DISABLED			ERR_REC_BASE+7	//周期录像未使能
#define ERR_REC_NONE_RECORDING			ERR_REC_BASE+8	//当前未在录像
#define ERR_REC_WRONG_TYPE				ERR_REC_BASE+9	//停止录像的类型不正确
#define ERR_REC_WRONG_PARAM				ERR_REC_BASE+10	//错误的参数，参数不在合法的取值范围
#define ERR_REC_OPEN_DB_FAILED			ERR_REC_BASE+11	//打开数据库文件失败
#define ERR_REC_CREATE_TABLE_FAILED		ERR_REC_BASE+12	//创建索引表失败
#define ERR_REC_NO_DBFILE				ERR_REC_BASE+13 //无数据库文件
#define ERR_REC_CLOSE_DB_FAILED			ERR_REC_BASE+14 //关闭数据库失败
#define ERR_REC_WRITE_INDEX_FAILED		ERR_REC_BASE+15 //写入索引文件失败
#define ERR_REC_OPEN_DEVICEFILE_FAILED	ERR_REC_BASE+16 //打开硬盘设备文件失败
#define ERR_REC_IOCTL_FAILED			ERR_REC_BASE+17	//IO操作失败
#define ERR_REC_READ_MBR_FAILED			ERR_REC_BASE+18	//读MBR失败
#define ERR_REC_LEGAL_MBR				ERR_REC_BASE+19	//非法的MBR
#define ERR_REC_OPEN_INTERFILE_FAILED	ERR_REC_BASE+20	//打开交互文件失败
#define ERR_REC_DELETE_PART_FAILED		ERR_REC_BASE+21	//删除分区失败
#define ERR_REC_FDISK_FAILED			ERR_REC_BASE+22	//分区失败
#define ERR_REC_QUERY_INDEX_FAILED		ERR_REC_BASE+23 //查询索引文件失败
#define ERR_REC_QUERY_COUNT_FAILED		ERR_REC_BASE+24	//获取查询结果总数失败
#define ERR_REC_GET_TABLE_FAILED		ERR_REC_BASE+25 //执行sqlite3_get_table失败
#define ERR_REC_NONEXIST_NAME			ERR_REC_BASE+26 //文件名不存在
#define ERR_REC_NOT_DIRRECTORY			ERR_REC_BASE+27 //不是目录
#define ERR_REC_UMOUNT_NFS_FAILED		ERR_REC_BASE+28 //卸载NFS失败
#define ERR_REC_MOUNT_NFS_FAILED		ERR_REC_BASE+29	//挂载NFS失败
#define ERR_REC_NO_RECORD_SPACE			ERR_REC_BASE+30	//录像空间不足，但还可以抓拍
#define ERR_REC_NO_SNAP_SPACE			ERR_REC_BASE+31	//抓拍空间不足
#define ERR_REC_VALID_FRAME				ERR_REC_BASE+32 //非法的帧头
#define ERR_REC_OPEN_FILE_FAILED		ERR_REC_BASE+33 //打开文件失败
#define ERR_REC_ALARM_DELAY				ERR_REC_BASE+34	//转为报警延时录像
#define ERR_REC_NO_INDEX_PATH			ERR_REC_BASE+35 //无索引文件
#define ERR_REC_TABLE_NOT_EXISTED		ERR_REC_BASE+36	//表不存在
#define ERR_REC_SOME_RECORDING			ERR_REC_BASE+37	//有通道正在录像
#define ERR_REC_INVALID_FILE_NAME		ERR_REC_BASE+38 //非法文件名
#define ERR_REC_MALLOC_ERROR			ERR_REC_BASE+39 //malloc执行失败
#define ERR_REC_WRITE_FILE_ERROR		ERR_REC_BASE+40 //写文件失败
#define ERR_REC_NOT_I_FRAME				ERR_REC_BASE+41	//不是I帧
#define ERR_REC_NO_SUCH_INDEX			ERR_REC_BASE+42 //指定记录不存在
#define ERR_REC_REBUILDING_INDEX		ERR_REC_BASE+43 //正在重建索引
#define ERR_REC_NOT_EMPTY_DIR			ERR_REC_BASE+44	//目录不为空
#define ERR_REC_READ_FILE_ERROR			ERR_REC_BASE+45	//读文件错误
#define ERR_REC_MKDIR_FAILED			ERR_REC_BASE+46	//创建目录失败
#define ERR_REC_USING_DISK				ERR_REC_BASE+47	//试图格式化或分区正在使用的磁盘
#define ERR_REC_SYSTEM_ERROR			ERR_REC_BASE+48	//系统调用异常退出
#define ERR_REC_WRONG_DISK				ERR_REC_BASE+49	//磁盘不可用，或磁盘用途不正确
#define ERR_REC_OTHER_FORMATING			ERR_REC_BASE+50 //正在格式化其他磁盘
#define ERR_REC_NO_IDLE_POS				ERR_REC_BASE+51	//没有空余位置(一般在挂载USB设备的时候)
#define ERR_REC_TOO_MANY_USERS			ERR_REC_BASE+52	//VOD已达最大连接数
#define ERR_REC_BACKUP_TO_USB_ONLY		ERR_REC_BASE+53	//二次备份文件只允许备份在U盘上
#define ERR_REC_NOT_ALLOWED_BACKUP		ERR_REC_BASE+54	//SATA0不允许设置为备份盘
#define ERR_REC_NO_FIRST_SATA			ERR_REC_BASE+55	//SATA0不存在
#define ERR_REC_FILES_NOT_READY			ERR_REC_BASE+56	//刻录文件还没有准备好
#define ERR_REC_RECORDER_NOT_READY		ERR_REC_BASE+57	//刻录机还没有准备好
#define ERR_REC_DVD_DISK_NOT_READY		ERR_REC_BASE+58	//刻录机中没有放入光盘或者光盘不可用。
#define ERR_REC_HUGE_SIZE_TO_BURN		ERR_REC_BASE+59 //待刻录文件总大小超过4200M，请重新选择。
#define ERR_REC_INVALID_RECORD_FILE		ERR_REC_BASE+60	//非法的录像文件
#define ERR_REC_BURNING_FORBIDSETTING   ERR_REC_BASE+61 //刻录过程中不允许进行录像策略参数设置
#define ERR_REC_PACKING                 ERR_REC_BASE+62  //正在打包，请稍后
#define ERR_REC_NO_CHANNEL_SELECT      	ERR_REC_BASE+63  //未选择通道
#define ERR_REC_NO_FILE_IN_DVD          ERR_REC_BASE+64   //DVR中无文件列表
#define ERR_REC_DEVICE_FAIL             ERR_REC_BASE+65   //设备故障，无法启动刻录操作
#define ERR_REC_STOP_MANUAL				ERR_REC_BASE+66   //有些通道正在刻录，无法停止手动录像
#define ERR_REC_INTERLACE               ERR_REC_BASE+67   //CD1与CD2所选通道有重复，请重新选择
#define ERR_REC_BURNING_FORMAT			ERR_REC_BASE+68	//刻录过程中不允许格式化硬盘!
#define ERR_REC_REINDEX_FORMAT			ERR_REC_BASE+69	//重建索引过程中不允许格式化硬盘!
#define ERR_REC_LACK_OF_MEMORY			ERR_REC_BASE+70	//内存不足
#define ERR_REC_DEL_RECING_FAILED		ERR_REC_BASE+71	//内存不足
#define ERR_REC_NOT_ALLOWED_READONLY	ERR_REC_BASE+72
#define ERR_REC_FAILED					ERR_REC_BASE+99//失败
#define ERR_RTBURN_FINISHED				ERR_REC_BASE+100//实时刻录结束
#define ERR_REC_MAX_NUM_TO_BURN			ERR_REC_BASE+101 //待刻录文件总个数超过512，请重新选择
#define ERR_REC_DVD_DISK_NOT_EMPTY		ERR_REC_BASE+102	//刻录机中有光盘，但不满足光盘备份条件(空光盘)

//报警模块错误码
#define ERR_ALM_BASE					2000			//报警模块错误码基数
#define ERR_ALM_SUCCESSFUL				ERR_ALM_BASE+1	//操作成功
#define ERR_ALM_WRITE_COM_FAILED		ERR_ALM_BASE+2	//向串口写数据失败
#define ERR_ALM_WRONG_PARAM				ERR_ALM_BASE+3	//错误的参数
#define ERR_ALM_OPEN_FILE_ERROR 		ERR_ALM_BASE+4	//打开文件错误
#define ERR_ALM_READ_FILE_ERROR			ERR_ALM_BASE+5	//读文件错误
#define ERR_ALM_WRITE_FILE_ERROR		ERR_ALM_BASE+6	//写文件错误
#define ERR_ALM_SYSTEM_ERROR			ERR_ALM_BASE+7	//执行系统调用错误
#define ERR_ALM_MALLOC_ERROR			ERR_ALM_BASE+8	//MALLOC分配内存错误
#define ERR_ALM_NO_SUCH_FILE			ERR_ALM_BASE+9	//文件不存在
#define ERR_ALM_MEMMAP_FAILED			ERR_ALM_BASE+10	//memmap失败(在检测报警输入端口时)
#define ERR_ALM_FAILED					ERR_ALM_BASE+999//操作失败

//PTZ模块错误码
#define ERR_PTZ_BASE					3000			//PTZ模块错误码的基数
#define ERR_PTZ_SUCCESSFUL				ERR_PTZ_BASE+1	//操作成功
#define ERR_PTZ_WRONG_PARAM				ERR_PTZ_BASE+2	//错误的参数
#define ERR_PTZ_OPEN_FILE_ERROR			ERR_PTZ_BASE+3	//打开文件失败
#define ERR_PTZ_READ_FILE_ERROR			ERR_PTZ_BASE+4	//读文件失败
#define ERR_PTZ_WRITE_FILE_ERROR		ERR_PTZ_BASE+5	//写文件失败
#define ERR_PTZ_UNKNOWN_PROTOCOL		ERR_PTZ_BASE+6	//设备不支持该协议，或无法识别
#define ERR_PTZ_MAKE_CODE_ERROR			ERR_PTZ_BASE+7	//组控制码失败
#define ERR_PTZ_WRITE_COM_FAILED		ERR_PTZ_BASE+8	//向串口写数据失败
#define ERR_PTZ_UNUSEABLE_FUNCTION		ERR_PTZ_BASE+9	//未启用的功能
#define ERR_PTZ_USING_PROTOCOL			ERR_PTZ_BASE+10	//该协议正在被使用
#define ERR_PTZ_MAX_PRTNUM				ERR_PTZ_BASE+11	//已达设备支持的PTZ协议数上限
#define ERR_PTZ_FAILED					ERR_PTZ_BASE+999//操作失败

//日志模块错误码
#define	ERR_LOG_BASE					4000			//日志模块错误码基数
#define ERR_LOG_SUCCESSFUL				ERR_LOG_BASE+1 	//操作成功
#define ERR_LOG_WRONG_PARAM				ERR_LOG_BASE+2	//错误的参数
#define ERR_LOG_FAILED					ERR_LOG_BASE+999//操作失败

//系统模块错误码
#define ERR_SYS_BASE					5000			//系统模块错误码基数
#define ERR_SYS_SUCCESSFUL				ERR_SYS_BASE+1	//操作成功
#define ERR_SYS_WRONG_PARAM				ERR_SYS_BASE+2	//错误的参数
#define ERR_SYS_OPEN_FILE_ERROR			ERR_SYS_BASE+3	//打开文件错误
#define ERR_SYS_WRITE_FILE_ERROR		ERR_SYS_BASE+4	//写文件错误
#define ERR_SYS_READ_FILE_ERROR			ERR_SYS_BASE+5	//读文件错误
#define ERR_SYS_IOCTL_ERROR				ERR_SYS_BASE+6	//ioctl错误
#define ERR_SYS_NO_USB					ERR_SYS_BASE+7	//无USB设备
#define ERR_SYS_NO_FTP					ERR_SYS_BASE+8	//无法连接到指定FTP站点
#define ERR_SYS_NO_FILE					ERR_SYS_BASE+9	//找不到待升级文件
#define ERR_SYS_OTHER_UPDATING			ERR_SYS_BASE+10	//其他客户端正在升级
#define ERR_SYS_GET_FILE_FAILED			ERR_SYS_BASE+11	//从FTP站点获取升级文件失败
#define ERR_SYS_ALPHA_DIGIT_ONLY		ERR_SYS_BASE+12 //文件名中只允许字母或数字
#define ERR_SYS_FILE_EXIST				ERR_SYS_BASE+13	//备份数据库已经存在
#define ERR_SYS_FAILED					ERR_SYS_BASE+999//操作失败

//陶永亮 091029 异常处理模块错误码
#define ERR_EXCP_BASE					6000			//异常处理模块错误码基数
#define ERR_EXCP_SUCCESSFUL				TD_SUCCESS		//成功
#define ERR_EXCP_FAILED					TD_FAILURE		//失败
#define ERR_EXCP_WRONG_PARAM			ERR_EXCP_BASE+1	//参数错误
#define ERR_EXCP_WRITE_CONFIG			ERR_EXCP_BASE+2	//写配置文件失败
#define ERR_EXCP_READ_CONFIG			ERR_EXCP_BASE+3	//读配置文件失败
#define ERR_EXCP_CREATE_THREAD			ERR_EXCP_BASE+4	//创建线程失败

//陶永亮 091101 串口管理模块错误码
#define ERR_COM_BASE					7000			//串口管理模块错误码基数
#define ERR_COM_SUCCESSFUL				TD_SUCCESS		//成功
#define ERR_COM_FAILED					TD_FAILURE		//失败
#define ERR_COM_WRONG_PARAM				ERR_COM_BASE+1	//传入参数错误
#define ERR_COM_TOO_MUCH_DATA			ERR_COM_BASE+2	//缓冲区数据过长
#define ERR_COM_OPEN_ERROR				ERR_COM_BASE+3	//打开串口错误
#define ERR_COM_CLOSE_ERROR				ERR_COM_BASE+4	//关闭串口错误
#define ERR_COM_READ_ERROR				ERR_COM_BASE+5	//读串口错误
#define ERR_COM_WRITE_ERROR				ERR_COM_BASE+6	//写串口错误
#define ERR_COM_CALL_ERROR				ERR_COM_BASE+7	//调用底层函数错误
#define ERR_COM_CREATE_THREAD_ERROR		ERR_COM_BASE+8	//创建线程失败

//杨平 091229 预览错误码
#define ERR_VIEW_BASE					10000
#define ERR_VIEW_SUCCESSFUL				TD_SUCCESS		//成功
#define ERR_VIEW_FAILED					TD_FAILURE		//失败
#define ERR_VIEW_SET_LIMIT_VO			ERR_VIEW_BASE+1	//预览设置VO数目超过33

//陶永亮 100311 媒体层错误码
#define ERR_MEDIA_BASE					11000
#define ERR_MEDIA_SUCCESSFUL			TD_SUCCESS			//成功
#define ERR_MEDIA_FAILED				TD_FAILURE			//失败
#define ERR_MEDIA_BIT_FRAME_BALANCE		ERR_MEDIA_BASE+1	//小帧率大码率错误
#define ERR_MEDIA_TOTAL_BANDWIDTH		ERR_MEDIA_BASE+2	//通道总带宽限制
#define ERR_MEDIA_AREA_CONFLICT			ERR_MEDIA_BASE+3	//字符叠加区域重叠
#define ERR_MEDIA_WRONG_STREAM_TYPE		ERR_MEDIA_BASE+4	//不支持的媒体流类型
#define ERR_MEDIA_UNKNOWN_AUDIO_TYPE	ERR_MEDIA_BASE+5	//不支持的音频编码格式
#define ERR_MEDIA_TALK_IS_OPEN			ERR_MEDIA_BASE+6	//对讲时不允许修改音频编码格式
#define ERR_MEDIA_VCA_UNSUPPORT_SIZE	ERR_MEDIA_BASE+9	//数字通道智能分析功能目前不支持CIF以下分辨率
#define ERR_MEDIA_VCA_VDEC_VENC_FAILED  ERR_MEDIA_BASE+10   //数字通道启用智能分析编解码通道失败

//Cheng 100407 网络相关错误码
#define ERR_NETWORK_BASE                12000
#define ERR_NETWORK_SUCCESSFUL          TD_SUCCESS          ///
#define ERR_NETWORK_FAILED              TD_FAILURE
#define ERR_NETWORK_IP_ALIMIT           ERR_NETWORK_BASE+1    //网络地址 第一个不在1-223之间
#define ERR_NETWORK_IP_MASK             ERR_NETWORK_BASE+2   //设置mask与IP组合不正确
#define ERR_NETWORK_IP_INVALID          ERR_NETWORK_BASE+3   //IP设置不合法，超过255
#define ERR_NETWORK_NTPPARA_INVALID     ERR_NETWORK_BASE+4   //NTP参数设置不合法
#define ERR_NETWORK_NTPINTVAL_INVALID   ERR_NETWORK_BASE+5   //NTP校时间隔设置不合法
#define ERR_NETWORK_OLD_PACKET      	ERR_NETWORK_BASE+6
#define ERR_NETWORK_NEW_PACKET			ERR_NETWORK_BASE+7
#define ERR_NETWORK_IP_GATEWAY          ERR_NETWORK_BASE+8    //IP与gateway不在同一网段
#define ERR_NETWORK_MASK_INVALID        ERR_NETWORK_BASE+9   //子网掩码不合法
#define ERR_NETWORK_SAME_NET_SEGMENT    ERR_NETWORK_BASE+10  //双网卡同一网段
#define ERR_NETWORK_DIALING_CHANGETYPE    ERR_NETWORK_BASE+11  //正在拨号过程中不可以修改拨号方式！

#define ERR_NETWORK_MAX                 ERR_NETWORK_BASE+500

//nsg 20120307 智能分析模块错误码
#define ERR_VCA_BASE					8000
#define ERR_VCA_SUCCESSFUL				TD_SUCCESS		//成功
#define ERR_VCA_FAILED					TD_FAILURE		//失败
#define ERR_VCA_OPEN_FILE_ERROR			ERR_VCA_BASE+1	//打开配置文件失败
#define ERR_VCA_WRITE_FILE_ERROR		ERR_VCA_BASE+2	//写配置文件失败
#define ERR_VCA_READ_ERROR				ERR_COM_BASE+3	//读配置文件错误
#define ERR_VCA_NO_SUCH_FILE			ERR_VCA_BASE+4	//没有这个文件

//没有响应的用户权限
#define ERR_USER_BASE					9000
#define ERR_USER_RIGHT					ERR_USER_BASE + 1	//权限不足，操作失败

//#define ERR_NETWORK_PPPOE_CONNECT        ERR_NETWORK_BASE+4
//#define ERR_NETWORK_PPPOE_DISCONNECT        ERR_NETWORK_BASE+5

//回放操作
#define ERR_PLAY_BASE					10000
#define ERR_PLAY_CAP_ERR				ERR_PLAY_BASE
#define ERR_PLAY_CAP_OK					ERR_PLAY_BASE + 1
#define ERR_PLAY_BACK_ERR				ERR_PLAY_BASE + 2
#endif /* TD_ERRNO_H_ */

