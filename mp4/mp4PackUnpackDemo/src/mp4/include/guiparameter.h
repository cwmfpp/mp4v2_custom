/********************************************************
// gui/dvr主程序间通信协议定义
Create by duxiulong(duxlong@live.cn), 2009
*********************************************************/
#ifndef _GUI_PARAMETER_H_
#define _GUI_PARAMETER_H_

//公共设置成功失败返回码
#define GUI_SET_OK  0  //设置成功
#define GUI_SET_ERR 1  //设置失败

#define GUI_SET_DISENABLE   0 //设置不使能
#define GUI_SET_ENABLE 1      //设置使能

#define MAX_CONFIGFILE_LISTNUM 64 //UI 限制


//公共数组的长度 (使用时需要 + 1)
#define GUI_PUBLIC_LEN_15	 15
#define GUI_PUBLIC_LEN_31     31
#define GUI_PUBLIC_LEN_63     63
#define GUI_PUBLIC_LEN_127    127
#define GUI_PUBLIC_LEN_255    255


#define GUI_DVR_SCR_COLS			22			//屏幕小格列数
#define GUI_DVR_SCR_ROWS			18			//屏幕小格行数

#define GUI_DVR_CHN_CNT			    32			//通道个数

#define GUI_SERIAL_PORT_CNT			16			//串口总个数

#define GUI_PAL_MODE				1			//PAL制
#define GUI_NTSC_MODE				2			//NTSC制
#define GUI_AUTO_MODE               3           //自适应制式

// 显示设备编号
#define GUI_VGA_DEV					0			//VGA输出
#define GUI_BNC_DEV					1			//BNC输出
#define GUI_BNCA_DEV				2			//BNC输出(辅口)
#define GUI_VC_DEV 					16          //虚拟设备
#define GUI_HDMI0_DEV				0         //HDMI0设备
#define GUI_HDMI1_DEV				1		  //HDMI1设备

//磁盘编号
#define GUI_NFS_DISK_ID				12			//nfs磁盘编号
#define GUI_DISK_CD					13			//光驱磁盘编号 
#define GUI_ESATA_DISK_ID			14          //ESATA磁盘编号
#define GUI_DISK_SEC_CD_ID			15          //第二个光驱编号
#define GUI_SD_DISK_ID				50			//SD卡编号

#define GUI_MIN_SATA_EXTEND_NUM			1008  //扩展磁盘起始位置
#define GUI_MAX_SATA_EXTEND_NUM			1015  //扩展磁盘结束位置	
#define GUI_VDISK_NUM_BEGIN				2000  //虚拟磁盘开始	
#define GUI_IPSAN_NUM_BEGIN				3000  //IPSAN开始	
#define GUI_IPSAN_NUM_END					3007
#define GUI_USB_NUM_BEGIN					8
#define GUI_USB_NUM_END					12


// VGA分辨率
#define VGA_800x600_60				0
#define VGA_1024x768_60				10
#define VGA_1280x1024_60			20
#define VGA_1440x900_60				30
#define VGA_1366x768_60				40
#define VGA_DEF_SIZE				VGA_1024x768_60
#define VGA_640x480					90
//HDMI
#define HDMI_1280x720_50            100
#define HDMI_1280x720_60            110
#define HDMI_1920x1080_25			120
#define HDMI_1920x1080_30			130
#define HDMI_1920x1080_50           140
#define HDMI_1920x1080_60           150
#define HDMI_1280x800_60            160
#define HDMI_2560x1600_60          170
#define HDMI_3840x2160_30          180 //暂时没用到
#define HDMI_3840x2160_60          190


// 视频分辨率
#define GUI_QCIF 				0		  //P:176×144 N:176×120
#define GUI_HCIF 			    1         //P:352×144 N:352×120
#define GUI_CIF					2		  //P:352×288 N:352×240
#define GUI_2CIF				3		  //P:720×288 N:720×240
#define GUI_D1					4         //P:720×576 N:720×480
#define GUI_MD1					5         //P:720×288 N:720×240
#define GUI_QVGA            	6		  //320×240
#define GUI_FVGA            	7         //640×480
#define GUI_HVGA            	8         //640×240
#define GUI_720P           		9		  //1280×720
#define GUI_960P           		10		  //1280×960
#define GUI_1200           		11		  //1600×1200
#define GUI_1080P          		12        //1920×1080
#define GUI_QXGA				13        //2048×1536
#define GUI_QHD					0x0100    //960×540
#define GUI_960H				0x0200    //P:960×576 N:960×480
#define GUI_4M					0x0210    //2560*1440
#define GUI_4MB					0x0220	//2592*1520
#define GUI_5MA					0x0300    //2448*2048
#define GUI_5M 					0x0400    //2560*1920
#define GUI_5MB					0x0410    //2592*1944
#define GUI_5MC					0x0420    //2592*2048
#define GUI_5MD					0x0430	//2528*2128
#define GUI_QSXGA				0x0500    //2560*2048
#define GUI_6M					0x0600    //2752*2208
#define GUI_6MB					0x0610	//3008*2008
#define GUI_6MC					0x0620    //3408*2008
#define GUI_7M					0x0700	//3392x2008     
#define GUI_8M					0x0800    //3840x2160
#define GUI_8MB					0x0810	//3264x2448
#define GUI_8MC					0x0820	//3296*2472
#define GUI_8MD					0x0830	//4096*2160

//走廊模式分辨率格式:100000 + N(N为以上分辨率代码),以此类推
#define GUI_COORIDOR_BASE		0x100000     //走廊模式起始    
#define GUI_720P_9_16			0x100009    //720x 1280
#define GUI_960P_3_4			0x10000A      //960x1280
#define GUI_1080P_9_16		  	0x10000C	//1080x 1920
#define GUI_QXGA_3_4		  	0x10000D	//1536 x 2048
#define GUI_4M_9_16				0x100210	//1440*2560
#define GUI_4MB_9_16			0x100220	//1520*2592

// 视频编码方式
#define GUI_H264			1
#define GUI_MPG4			2
#define GUI_MJPG			3
#define GUI_H265			23     //h265编码

//音频编码方式
#define GUI_AUDIO_TYPE_G711A	1
#define GUI_AUDIO_TYPE_G711U	2
#define GUI_AUDIO_TYPE_ADPCM	3
#define GUI_AUDIO_TYPE_AAC		22

// 全帧效(PAL:25 NTSC:30)
#define GUI_FULL_FRAME_RATE		2009
//视频参数配置帧率
#define GUI_0_FRAME_RATE     0
#define GUI_1_FRAME_RATE     1
#define GUI_5_FRAME_RATE     5
#define GUI_10_FRAME_RATE    10
#define GUI_15_FRAME_RATE    15
#define GUI_20_FRAME_RATE    20
#define GUI_25_FRAME_RATE    25
#define GUI_30_FRAME_RATE    30
#define GUI_50_FRAME_RATE    50
#define GUI_60_FRAME_RATE    60
#define GUI_MAX_FRAME_RATE   480

#define GUI_NUM_FRAME_RATE   	7  //0, 1, 5, 10, 15, 20, full

#define GUI_CMD_NOP				0			//空操作

//网络重启关机命令参数 赋值给g_ptGuiSysStatus->m_iParams[0]变量
#define GUI_GET_NET_REBOOT_CMD  20120224 

// 用户管理
#define GUI_USER_LOGIN_REQ		1				//用户登录请求
#define GUI_USER_LOGIN_RSP		-1				//用户登录回应
#define GUI_USER_LOGOUT_REQ		2			//用户注销请求
#define GUI_USER_LOGOUT_RSP		-2			//用户注销回应
#define GUI_ADD_USER_REQ		3			//添加用户请求
#define GUI_ADD_USER_RSP		-3			//添加用户回应
#define GUI_RM_USER_REQ			4			//删除用户请求
#define GUI_RM_USER_RSP			-4				//删除用户回应
#define GUI_EDIT_USER_REQ		5			//修改用户资料请求
#define GUI_EDIT_USER_RSP		-5			//修改用户资料回应
#define GUI_GET_USER_MSG_REQ	6			//获取用户资料请求
#define GUI_GET_USER_MSG_RSP	-6			//获取用户资料回应
#define GUI_GET_USER_GROUP_REQ	7			//获取用户组列表请求
#define GUI_GET_USER_GROUP_RSP	-7				//获取用户组列表回应
#define GUI_GET_USER_LIST_REQ	8			//获取用户列表请求
#define GUI_GET_USER_LIST_RSP	-8			//获取用户列表回应
#define GUI_GET_USER_LIST_REQ_EX	 9			//获取用户列表请求扩展
#define GUI_GET_USER_LIST_RSP_EX	-9			//获取用户列表回应扩展

//UI启动信息
#define GUI_START_UP_MSG		10			//UI启动信息
#define GUI_START_UP_RSP		-10			//UI启动信息回应

//设置VGA格式
#define GUI_SET_VGA_MOD_REQ		11			//设置VGA格式请求
#define GUI_SET_VGA_MOD_RSP		-11			//设置VGA格式回应

//设置BNC格式
#define GUI_SET_BNC_MOD_REQ		12
#define GUI_SET_BNC_MOD_RSP		-12


//------------------------------系统设置---------------------------------
// 1.磁盘管理
#define GUI_GET_DISK_LIST_REQ	50			//获取磁盘列表请求
#define GUI_GET_DISK_LIST_RSP	-50			//获取硬盘列表回应
#define GUI_FORMAT_DISK_REQ		51			//格式化磁盘请求
#define GUI_FORMAT_DISK_RSP		-51			//格式化磁盘回应
#define GUI_GET_FORMAT_STAT_REQ	52			//获取磁盘格式化状态请求
#define GUI_GET_FORMAT_STAT_RSP	-52			//获取磁盘格式化状态回应
#define GUI_DISK_PART_REQ		53			//硬盘分区请求
#define GUI_DISK_PART_RSP		-53			//磁盘分区回应
#define GUI_GET_DISK_MSG_REQ	54			//获取磁盘信息请求
#define GUI_GET_DISK_MSG_RSP	-54			//获取磁盘信息回应
#define GUI_SET_DISK_MSG_REQ	55			//设置磁盘信息请求
#define GUI_SET_DISK_MSG_RSP	-55			//徽墨磁盘信息回应
// 2.串口设置
#define GUI_GET_SPORT_MSG_REQ		60			//获取串口配置请求
#define GUI_GET_SPORT_MSG_RSP		-60			//获取串口配置回应
#define GUI_SET_SPORT_MSG_REQ		61			//设置串口配置请求
#define GUI_SET_SPORT_MSG_RSP		-61			//设置串口配置回应

// 3.系统信息
#define GUI_GET_SYS_MSG_REQ		70			//获取系统信息请求
#define GUI_GET_SYS_MSG_RSP		-70			//获取系统信息回应
#define GUI_SET_SYS_MSG_REQ		71			//设置系统信息请求
#define GUI_SET_SYS_MSG_RSP		-71			//设置系统信息回应
#define GUI_CHG_SYS_TIME_MSG		72			//系统时间更新通知


// 5.系统升级
#define GUI_SYS_UPDATE_STA_REQ		91			//系统升级状态查询请求
#define GUI_SYS_UPDATE_STA_RSP		-91			//系统升级状态查询回应
#define GUI_SYS_FILE_UPDATE_REQ	92			//系统文件升级请求
#define GUI_SYS_FILE_UPDATE_RSP	-92			//系统文件升级回应
#define GUI_SYS_FTP_UPDATE_REQ		93			//系统FTP升级请求
#define GUI_SYS_FTP_UPDATE_RSP		-93			//系统FTP升级回应
#define GUI_SYS_FILE_UPDATE_NUM_REQ	94		//系统文件请求
#define GUI_SYS_FILE_UPDATE_NUM_RSP	-94		//系统文件回应
#define GUI_SYS_UPDATE_SUCCESS 	0
#define GUI_SYS_UPDATE_FAIL    		1


// 6.恢复出厂设置
#define GUI_SET_RENEW_REQ			100		//系统参数重设请求
#define GUI_SET_RENEW_RSP			-100		//系统参数重设回应

// 7.网络设置
#define GUI_GET_IP_REQ				110		//获取IP参数请求
#define GUI_GET_IP_RSP				-110		//获取IP参数回应
#define GUI_SET_IP_REQ				111		//设置IP参数请求
#define GUI_SET_IP_RSP				-111		//设置IP参数回应
#define GUI_GET_NET_CNF_REQ		112		//获取网络服务配置请求
#define GUI_GET_NET_CNF_RSP		-112		//获取网络服务配置回应
#define GUI_SET_NET_CNF_REQ		113		//设置网络服务配置请求
#define GUI_SET_NET_CNF_RSP			-113		//设置网络服务配置回应
#define GUI_GET_REG_CNF_REQ		114		//获取注册中心信息请求
#define GUI_GET_REG_CNF_RSP		-114		//获取注册中心信息回应
#define GUI_SET_REG_CNF_REQ		115		//设置注册中心信息请求
#define GUI_SET_REG_CNF_RSP		-115		//设置注册中心信息回应
#define GUI_GET_DDNS_REQ			130     	 //DDNS请求
#define GUI_GET_DDNS_RSP 			-130      	//DDNS请求回应
#define GUI_SET_DDNS_REQ			131  		//给dvr发送有关DDNS的设置参数
#define GUI_SET_DDNS_RSP			-131   	//给dvr发送有关DDNS的设置参数回应
#define GUI_GET_NTP_REQ				132      	//NTP请求
#define GUI_GET_NTP_RSP				-132      //NTP请求回应
#define GUI_SET_NTP_REQ				133   	//给dvr发送有关NPT的设置参数
#define GUI_SET_NTP_RSP				-133 	//dvr回应
#define GUI_CONNECT_OVER_PPPOE_STAT_REQ 	134        //获取PPPoE状态请求
#define GUI_CONNECT_OVER_PPPOE_STAT_RSP	-134		//回应
#define GUI_CONNECT_OVER_PPPOE_REQ     	135       // PPPoE连接请求
#define GUI_CONNECT_OVER_PPPOE_RSP 	-135       //PPPoE连接回应
#define GUI_CUT_CONNECT_OVER_PPPOE_REQ 136     // 断开PPPoE连接
#define GUI_CUT_CONNECT_OVER_PPPOE_RSP -136   // 断开PPPoE连接回应
#define GUI_SET_PPPOE_REQ 		137                      //设置PPPOE拨号请求
#define GUI_SET_PPPOE_RSP 		-137                     //回应

// 8.异常处理
#define GUI_GET_EXCEP_CNF_REQ		120		//获取异常处理参数请求
#define GUI_GET_EXCEP_CNF_RSP		-120		//获取异常处理参数回应
#define GUI_SET_EXCEP_CNF_REQ		121		//设置异常处理参数请求
#define GUI_SET_EXCEP_CNF_RSP		-121		//设置异常处理参数回应

//9.别名管理
#define GUI_GET_CHN_ALIAS_REQ		140      	//获取通道别名请求
#define GUI_GET_CHN_ALIAS_RSP		-140
#define GUI_SET_CHN_ALIAS_REQ 		141     	 //通道设置
#define GUI_SET_CHN_ALIAS_RSP		-141  	//通道设置回应
#define GUI_GET_CHN_ALIAS_VER_REQ	142		//获取通道别名版本请求
#define GUI_GET_CHN_ALIAS_VER_RSP	-142		//获取通道别名版本回应


//------------------------------图像设置---------------------------------
// 1.预览设置
#define GUI_GET_PREVIEW_REQ		200			//获取预览参数请求
#define GUI_GET_PREVIEW_RSP		-200		//获取预览参数回应
#define GUI_SET_PREVIEW_REQ		201			//设置预览参数请求
#define GUI_SET_PREVIEW_RSP		-201		//设置预览参数回应
#define GUI_PREVIEW_SW_START_REQ	204			//预览切换开始请求
#define GUI_PREVIEW_SW_START_RSP	-204		//预览切换开始回应
#define GUI_PREVIEW_SW_STOP_REQ	205			//预览切换暂停请求
#define GUI_PREVIEW_SW_STOP_RSP	-205		//预览切换暂停回应
#define GUI_PREVIEW_SINGLE_PIC	206			//单画面(与GUI_PREVIEW_SPIC_REQ同)
#define GUI_PREVIEW_SPIC_REQ		206			//单画面
#define GUI_PREVIEW_SPIC_RSP		-206		//单画面回应
#define GUI_PREVIEW_SPIC_EXIT_MSG	251		//单画面退出消息

// 1.1 声音控制
#define GUI_SND_DEC_MSG			253			//音量减消息
#define GUI_SND_ASC_MSG			254			//音量增消息
#define GUI_VOL_CTR_REQ			255			//音量设置请求(音频输入)
#define GUI_VOL_CTR_RSP				-255	

// 2.本地显示
#define GUI_GET_DISPLAY_CNF_REQ	210			//获取本地显示属性请求
#define GUI_GET_DISPLAY_CNF_RSP	-210		//获取本地显示属性回应
#define GUI_SET_DISPLAY_CNF_REQ	211			//设置本地显示属性请求
#define GUI_SET_DISPLAY_CNF_RSP	-211		//设置本地显示属性回应


// 3.视频输入参数
#define GUI_GET_VIDEOIN_CNF_REQ	220			//获取视频输入参数请求
#define GUI_GET_VIDEOIN_CNF_RSP	-220		//获取视频输入参数回应
#define GUI_SET_VIDEOIN_CNF_REQ	221			//设置视频输入参数请求
#define GUI_SET_VIDEOIN_CNF_RSP	-221		//设置视频输入参数回应
#define GUI_CHG_VIDEOIN_CNF_REQ	222			//(临时)更改视频输入参数请求
#define GUI_CHG_VIDEOIN_CNF_RSP	-222		//(临时)更改视频输入参数回应
#define GUI_GET_VIDEOIN_NOR_REQ	223			//获取通道视频输入制式请求	//2009/12/22
#define GUI_GET_VIDEOIN_NOR_RSP	-223		//获取通道视频输入制式回应
#define GUI_SET_VIDEOIN_NOR_REQ	224
#define GUI_SET_VIDEOIN_NOR_RSP	-224

#define GUI_GET_VIDEOIN_MOD_AUTO_CHK_REQ 225	//获取视频制式自动检测请求
#define GUI_GET_VIDEOIN_MOD_AUTO_CHK_RSP -225	//获取视频制式自动检测回应
#define GUI_SET_VIDEOIN_MOD_AUTO_CHK_REQ 226	//设置视频制式自动检测请求
#define GUI_SET_VIDEOIN_MOD_AUTO_CHK_RSP -226	//设置视频制式自动检测回应


// 5.视频前端叠加
#define GUI_GET_VOVERLAY_CNF_REQ	240			//获取视频前端叠加参数请求
#define GUI_GET_VOVERLAY_CNF_RSP	-240		//获取视频前端叠加参数回应
#define GUI_SET_VOVERLAY_CNF_REQ	241			//设置视频前端叠加参数请求
#define GUI_SET_VOVERLAY_CNF_RSP	-241		//设置视频前端叠加参数回应
#define GUI_CHG_LOGO_BMP_REQ		242			//更换logo图标请求
#define GUI_CHG_LOGO_BMP_RSP		-242		//更换logo图标回应
//----------------------视频编码能力配置----------------------------
//视频参数配置
#define GUI_GET_VIDEOPARA_CNF_REQ 232       //获取视频参数配置请求
#define GUI_GET_VIDEOPARA_CNF_RSP -232     //获取视频参数配置回应
#define GUI_SET_VIDEOPARA_CNF_REQ 233	   //设置视频参数配置请求
#define GUI_SET_VIDEOPARA_CNF_RSP -233     //设置视频参数配置回应
//视频总编码能力配置
#define GUI_GET_TOTALCODE_CNF_REQ 234      //获取视频参数配置中总的编码能力请求
#define GUI_GET_TOTALCODE_CNF_RSP -234     //获取视频参数配置中总的编码能力回应


//------------------------------录像设置---------------------------------
// 1.录像模板
#define GUI_GET_VR_TEMPLATE_REQ	300			//获取录像模板请求
#define GUI_GET_VR_TEMPLATE_RSP	-300		//获取录像模板回应
#define GUI_SET_VR_TEMPLATE_REQ	301			//设置录像模板请求
#define GUI_SET_VR_TEMPLATE_RSP	-301		//设置录像模板回应

// 2.录像策略
#define GUI_GET_VR_STRATEGY_REQ	310			//获取录像策略请求
#define GUI_GET_VR_STRATEGY_RSP	-310		//获取录像策略回应
#define GUI_SET_VR_STRATEGY_REQ	311			//设置录像策略请求
#define GUI_SET_VR_STRATEGY_RSP	-311		//设置录像策略回应

// 3.录像文件
#define GUI_COUNT_VIDEO_FILE_REQ	320			//统计录像文件个数请求
#define GUI_COUNT_VIDEO_FILE_RSP	-320		//统计录像文件个数回应
#define GUI_BACKUP_VIDEO_FILE_REQ	322		//备份录像文件请求
#define GUI_BACKUP_VIDEO_FILE_RSP	-322	//备份录像文件回应
#define GUI_DEL_VIDEO_FILE_REQ	323			//删除录像文件请求
#define GUI_DEL_VIDEO_FILE_RSP	-323		//删除录像文件回应
#define GUI_BACKUP_TODAY_VIDEO_REQ	324		//备份当天录像文件请求
#define GUI_BACKUP_TODAY_VIDEO_RSP	-324	//备份当天录像文件回应	
#define GUI_QUERY_VIDEO_END_MSG		325		//录像文件查询结束
#define GUI_GET_VIDEO_FILE_MAP_REQ	326		//获取录像文件快照请求(一个月哪些天有录像)
#define GUI_GET_VIDEO_FILE_MAP_RSP	-326	//获取录像文件快照回应


// 抓拍图片
#define GUI_COUNT_SNATCH_PIC_REQ	327			//统计抓拍图片个数请求
#define GUI_COUNT_SNATCH_PIC_RSP	-327		//统计抓拍图片个数回应
#define GUI_QUERY_SNATCH_PIC_REQ	328			//查询抓拍图片请求
#define GUI_QUERY_SNATCH_PIC_RSP	-328		//查询抓拍图片回应
#define GUI_DEL_SNATCH_PIC_REQ	329				//删除抓拍图片请求
#define GUI_DEL_SNATCH_PIC_RSP	-329			//删除抓拍图片回应
//图片备份
#define GUI_BACKUP_PIC_REQ		1300	      //备份图片请求
#define GUI_BACKUP_PIC_RSP			-1300		//备份图片回应


// 4.录像回放
#define GUI_PLAY_VIDEO_START_REQ	330			//播放录像文件请求
#define GUI_PLAY_VIDEO_START_RSP	-330		//播放录像文件回应
#define GUI_PLAY_VIDEO_PAUSE_REQ	331			//播放暂停请求
#define GUI_PLAY_VIDEO_PAUSE_RSP	-331		//播放暂停回应
#define GUI_PLAY_VIDEO_CONT_REQ		332			//播放继续请求
#define GUI_PLAY_VIDEO_CONT_RSP		-332		//播放继续回应
#define GUI_PLAY_VIDEO_STOP_REQ		333			//播放停止请求
#define GUI_PLAY_VIDEO_STOP_RSP		-333		//播放停止回应
#define GUI_PLAY_VIDEO_SPEEDX_REQ	334		//N倍速度播放请求
#define GUI_PLAY_VIDEO_SPEEDX_RSP	-334	//N倍速度播放回应
#define GUI_PLAY_VIDEO_SPEED_CMD	336		//设置快放/慢放倍数请求
#define GUI_PLAY_VIDEO_BY_STEP_REQ	335		//单步播放请求
//#define GUI_PLAY_VIDEO_BY_STEP_RSP	-335	//单步播放回应
//#define GUI_PLAY_VIDEO_NEXT_FRM_REQ	336		//前进一帧请求
//#define GUI_PLAY_VIDEO_PRE_FRM_REQ	337		//后进一帧请求
#define GUI_PLAY_VIDEO_LOC_REQ	338			//播放定位请求
//#define GUI_PLAY_VIDEO_LOC_RSP	-338		//播放定位回应
#define GUI_GET_VPLAY_STA_REQ		339			//请求录像文件播放状态
#define GUI_GET_VPLAT_STA_RSP		-339		//回应录像文件播放状态
#define GUI_GET_VPLAY_STA_RSP		GUI_GET_VPLAT_STA_RSP
//#define GUI_PLAY_VIDEO_RESIZE_REQ	340		//播放窗口重设大小请求
//#define GUI_PLAY_VIDEO_RESIZE_RSP	-340	//播放窗口重设大小回应
#define GUI_VIDEO_PLAYER_ACTIVE_MSG		341		//播放器激活通知(初始化VO，并保存当前预览状态)
#define GUI_VIDEO_PLAYER_ACTIVE_REQ		GUI_VIDEO_PLAYER_ACTIVE_MSG
#define GUI_VIDEO_PLAYER_ACTIVE_RSP		-341	//播放器激活回应
#define GUI_VIDEO_PLAYER_EXIT_MSG		342		//播放器退出通知(应当停止当前回放，前恢复此前保存的预览状态)
#define GUI_VPLAY_FULL_SCR_REQ			343		//指定画面全屏播放请求
#define GUI_VPLAY_FULL_SCR_RSP			-343	//指定画面全屏播放回应
#define GUI_VPLAY_FULL_SCR_EXIT_REQ		344		//全屏播放退出请求
#define GUI_VPLAY_FULL_SCR_EXIT_RSP		-344	//全屏播放退出回应
#define GUI_VPLAY_SET_SND_REQ			355		//回放声音设置请求
#define GUI_VPLAY_SET_SND_RSP			-355	//回放声音设置回应

#define GUI_GET_VPLAY_MARK_MSG_REQ      358    //获取录像文件的打点信息请求
#define GUI_GET_VPLAY_MARK_MSG_RSP      -358   //获取录像文件的打点信息回应

#define GUI_SYNC_PLAY_VIDEO_FILE_REQ 		370		//同步请求播放文件
#define GUI_SYNC_PLAY_VIDEO_FILE_RSP  		-370		//同步播放文件请求回应
#define GUI_SYNC_PLAY_VIDEO_NOFILE 			371		//同步回放继续
#define GUI_SYNC_PLAY_VIDEO_PAUSE_REQ 		372 		//同步回放暂停
#define GUI_SYNC_PLAY_VIDEO_FILE_STOP_REQ 373 	//同步停止请求
#define GUI_SYNC_PLAY_VIDEO_FILE_STEP_REQ 374 	//同步步进请求
#define GUI_SYNC_VPLAY_SET_SND_REQ 375       		//同步声音设置请求
#define GUI_SYNC_VPLAY_SET_SND_RSP -375        	//同步声音设置回应
#define GUI_VIDEO_PLAYER_COORDIRATE_REQ 376	//发送坐标,播放器初始化请求
#define GUI_VIDEO_PLAYER_COORDIRATE_RSP -376	//播放器初始化请求回应
#define GUI_SYNC_PLAY_VIDEO_LOC_REQ		377	//同步播放定位请求
#define GUI_SYNC_GET_VPLAY_STA_REQ		378	//请求录像文件播放状态
#define GUI_SYNC_GET_VPLAY_STA_RSP		-378	//回应录像文件播放状态
#define GUI_SYNC_VIDEO_PLAYER_EXIT_MSG	379		//播放器退出通知(应当停止当前回放，前恢复此前保存的预览状态)
#define GUI_SYNC_VPLAY_FULL_SCR_REQ		380		//指定画面全屏播放请求
#define GUI_SYNC_VPLAY_FULL_SCR_RSP		-380		//指定画面全屏播放回应
#define GUI_SYNC_PLAY_VIDEO_SPEEDX_REQ	381		//N倍速度播放请求
#define GUI_SYNC_VPLAY_FULL_SCR_EXIT_REQ	382		//全屏播放退出请求
#define GUI_SYNC_VPLAY_FULL_SCR_EXIT_RSP    -382    //全屏播放退出请求回应
#define GUI_SYNC_SND_DEC_MSG              		383       //同步播放音量减
#define GUI_SYNC_SND_ASC_MSG              		384        //同步播放音量增
#define GUI_SYNC_PLAY_VIDEO_SPEED_CMD		385		//同步设置快放/慢放倍数请求

// 视频解密
#define GUI_VDECODE_PWD_SET_REQ    386    //视频播放输入密码请求
#define GUI_VDECODE_PWD_SET_RSP    -386    //视频播放输入密码回应

// 视频加密
#define GUI_VENCODE_PWD_SET_REQ	387 //设置视频加密请求
#define GUI_VENCODE_PWD_SET_RSP	-387//设置视频加密回应	
#define GUI_VENCODE_PWD_GET_REQ	388 //获取视频加密请求
#define GUI_VENCODE_PWD_GET_RSP	-388//获取视频加密回应

// 5.手动录像
#define GUI_HUM_REC_VIDEO_CMD			350		//手动录像命令
#define GUI_REC_VIDEO_REQ				351		//通用录像命令
#define GUI_REC_VIDEO_RSP				-351	//通用录像回应

// 6.重建索引
#define GUI_REBUILD_INDEX_REQ			356		//重建索引请求
#define GUI_REBUILD_INDEX_RSP			-356	//重建索引回应
#define GUI_REBUILD_INDEX_STA_REQ	357		//获取重建索引状态请求
#define GUI_REBUILD_INDEX_STA_RSP	-357	//获取重建索引状态回应

// 抓拍浏览
#define GUI_JPEG_VIEWER_INIT_REQ		360		//JPEG显示初始化请求
#define GUI_JPEG_VIEWER_INIT_RSP		-360		//JPEG显示初始化回应
#define GUI_JPEG_VIEWER_EXIT_MSG		361		//JPEG显示退出消息
#define GUI_JPEG_DSP_REQ				362		//JPEG显示请求
#define GUI_JPEG_DSP_RSP				-362		//JPEG显示回应



//------------------------------云台控制---------------------------------
// 解码器设置
#define GUI_GET_PTZ_DECODE_CNF_REQ	400		//获取解码器参数请求
#define GUI_GET_PTZ_DECODE_CNF_RSP	-400	//获取解码器参数回应
#define GUI_SET_PTZ_DECODE_CNF_REQ	401		//设置解码器参数请求
#define GUI_SET_PTZ_DECODE_CNF_RSP	-401	//设置解码器参数回应

// 云台控制
#define GUI_GET_PTZ_PRESETPOINT_REQ	405		//获取预置点列表请求
#define GUI_GET_PTZ_PRESETPOINT_RSP	-405	//获取预置点列表回应
#define GUI_ADD_PTZ_PRESETPOINT_REQ	406		//添加新预置点请求
#define GUI_ADD_PTZ_PRESETPOINT_RSP	-406	//添加新预置点回应
#define GUI_DEL_PTZ_PRESETPOINT_REQ	407		//删除预置点请求
#define GUI_DEL_PTZ_PRESETPOINT_RSP	-407	//删除预置点回应
#define GUI_TRN_PTZ_PRESETPOINT_REQ	408		//调用预置点请求
#define GUI_TRN_PTZ_PRESETPOINT_RSP	-408	//调用预置点回应
#define GUI_GET_PTZ_CRUISROUTE_REQ	409		//获取巡航路径请求
#define GUI_GET_PTZ_CRUISROUTE_RSP	-409	//获取巡航路径回应
#define GUI_SET_PTZ_CRUISROUTE_REQ	410		//设置巡航路径请求
#define GUI_SET_PTZ_CRUISROUTE_RSP	-410	//设置巡航路径回应

#define GUI_PTZ_LOCAL_CODE			411		//本地(UI)生成的云台控制码
#define GUI_PTZ_REMOTE_CODE			412		//远程(网络键盘)生成的云台控码	

#define GUI_PTZ_TRACK_START_REQ		415		//开始记录轨迹请求
#define GUI_PTZ_TRACK_START_RSP		-415	//开始记录轨迹回应
#define GUI_PTZ_TRACK_END_REQ			416		//结束记录轨迹请求
#define GUI_PTZ_TRACK_END_RSP			-416	//结束记录轨迹回应
#define GUI_PTZ_TRACK_PLAY_START_REQ	417		//开始播放轨迹请求
#define GUI_PTZ_TRACK_PLAY_START_RSP	-417	//开始播放轨迹回应
#define GUI_PTZ_TRACK_PLAY_END_REQ	418		//结束播放轨迹请求
#define GUI_PTZ_TRACK_PLAY_END_RSP	-418	//结束播放轨迹回应

#define GUI_PTZ_PATH_PLAY_REQ			419		//演示路径请求
#define GUI_PTZ_PATH_PLAY_RSP			-419	//演示路径回应

#define GUI_PTZ_3D_MSG					420		// 3d定位

#define GUI_GET_PROTOCOL_DECODE_CNT_REQ	421		//获取协议总数
#define GUI_GET_PROTOCOL_DECODE_CNT_RSP	-421		//获取协议总数
#define GUI_GET_PROTOCOL_DEL_REQ			422		//删除协议请求
#define GUI_GET_PROTOCOL_DEL_RSP			-422		//删除协议回应



//------------------------------报警设置---------------------------------
// 报警输入
#define GUI_GET_ALARM_IN_CNF_REQ		500		//获取报警输入参数请求
#define GUI_GET_ALARM_IN_CNF_RSP		-500	//获取报警输入参数回应
#define GUI_SET_ALARM_IN_CNF_REQ		501		//设置报警输入参数请求
#define GUI_SET_ALARM_IN_CNF_RSP		-501	//设置报警输入参数回应

#define GUI_GET_ALARM_OUT_CNF_REQ	510		//获取报警输出参数请求
#define GUI_GET_ALARM_OUT_CNF_RSP	-510	//获取报警输出参数回应
#define GUI_SET_ALARM_OUT_CNF_REQ	511		//设置报警输出参数请求
#define GUI_SET_ALARM_OUT_CNF_RSP	-511	//设置报警输出参数回应

#define GUI_GET_MD_ALARM_CNF_REQ		520		//获取移动侦测报警参数请求
#define GUI_GET_MD_ALARM_CNF_RSP		-520	//获取移动侦测报警参数回应
#define GUI_SET_MD_ALARM_CNF_REQ		521		//设置移动侦测报警参数请求
#define GUI_SET_MD_ALARM_CNF_RSP		-521	//设置移动侦测报警参数回应

#define GUI_GET_VL_ALARM_CNF_REQ		530		//获取视频丢失报警参数请求
#define GUI_GET_VL_ALARM_CNF_RSP		-530	//获取视频丢失报警参数回应
#define GUI_SET_VL_ALARM_CNF_REQ		531		//设置视频丢失报警参数请求
#define GUI_SET_VL_ALARM_CNF_RSP		-531	//设置视频丢失报警参数回应

#define GUI_CLEAR_ALARM_CMD			540		//消除报警设置


// 注销
#define GUI_SYS_RESET_REQ				600		//系统重启请求
#define GUI_SYS_RESET_RSP				-600	//系统重启回应
#define GUI_SYS_POWEROFF_REQ			601		//关机请求
#define GUI_SYS_POWEROFF_RSP			-601	//关机回应
#define GUI_EXIT_MSG					602		//UI(外挂)退出消息

// 电子放大
#define GUI_EZOOM_ENTER_REQ			700		//进入电子放大功能请求
#define GUI_EZOOM_ENTER_RSP			-700	//进入电子放大功能回应
#define GUI_EZOOM_EXIT_REQ			701		//退出电子放大功能请求
#define GUI_EZOOM_EXIT_RSP			-701	//退出电子放大功能请求
#define GUI_EZOOM_SET_ATTR_REQ		702		//设置电子放大参数请求(包括通道与放大区域)
#define GUI_EZOOM_SET_ATTR_RSP		-702	//设置电子放大参数回应


// 光盘刻录
#define GUI_CD_BACKUP_START_REQ		720	//刻录开始通知
#define GUI_CD_BACKUP_START_RSP		-720
#define GUI_CD_BACKUP_FILE_REQ			721 	//传送刻录文件列表
#define GUI_CD_BACKUP_FILE_RSP			-721
#define GUI_CD_BACKUP_EXEC_REQ		722	//刻录执行请求
#define GUI_CD_BACKUP_EXEC_RSP		-722
#define GUI_CD_BACKUP_PRG_REQ			723	//刻录进度请求
#define GUI_CD_BACKUP_PRG_RSP			-723

// 实时刻录		20100913 dxl
#define GUI_CBL_TAG_REQ					728	//刻录打点命令
#define GUI_CBL_TAG_RSP					-728	//刻录打点命令
#define GUI_CBL_FORCE_END_REQ            729     //强制终止刻录命令
#define GUI_CBL_FORCE_END_RSP            -729    //强制终止刻录命令回应

//通道管理
#define GUI_GET_CHN_TYPE_REQ         		  800   //获取通道类型请求
#define GUI_GET_CHN_TYPE_RSP           		 -800  //获取通道类型回应
#define GUI_GET_LOCAL_CHN_ATTR_REQ 	 		  801   //获取本地模拟通道属性请求
#define GUI_GET_LOCAL_CHN_ATTR_RSP	 		-801  //获取本地模拟通道属性回应
#define GUI_GET_VC_CHN_ATTR_REQ				  802  //获取合成通道属性请求
#define GUI_GET_VC_CHN_ATTR_RSP 				-802  //获取合成通道属性回应
#define GUI_SET_CHN_TYPE_REQ                   803   //设置通道类型请求
#define GUI_SET_CHN_TYPE_RSP                   -803  //设置通道类型回应
#define GUI_GET_CHN_TYPE_EDIT_ABLE_FLG_REQ     804   //获取通道可编辑类型请求
#define GUI_GET_CHN_TYPE_EDIT_ABLE_FLG_RSP     -804  //获取通道可编辑类型回应

#define GUI_SYSTEM_EXIT			1000		//系统退出通知

// 消息
#define GUI_KEY_TX					5000		//遥控器按键
#define GUI_KEY_NET_TX				5001		//网络键盘

// 码流传输
#define GUI_STREAM_CONNECT_REQ		2001
#define GUI_STREAM_CONNECT_RSP		-2001
#define GUI_STREAM_DISCONNECT_REQ		2002
#define GUI_STREAM_DISCONNECT_RSP		-2002
#define GUI_FORCE_IFRAME_REQ			2003
#define GUI_FORCE_IFRAME_RSP			-2003

/********************************************************************************/
//***********************************报警主机******************************//
#define GUI_GET_NET_ALARMER_CNF_REQ     	100010	//获取网络报警主机配置请求
#define GUI_GET_NET_ALARMER_CNF_RSP    	   -100010	//获取网络报警主机配置回应
#define GUI_SET_NET_ALARMER_CNF_REQ			100011	//设置网络报警主机配置请求
#define GUI_SET_NET_ALARMER_CNF_RSP		   -100011	//设置网络报警主机配置回应

#define GUI_GET_SERIAL_ALARMER_CNF_REQ		100012	//获取串口报警主机配置请求
#define GUI_GET_SERIAL_ALARMER_CNF_RSP		-100012	//获取串口报警主机配置回应
#define GUI_SET_SERIAL_ALARMER_CNF_REQ      100013	//设置串口报警主机配置请求
#define GUI_SET_SERIAL_ALARMER_CNF_RSP      -100013	//设置串口报警主机配置回应


//多国语言语言切换通知
#define GUI_SET_CHANGE_LANG_REQ                100100  //发送语言切换请求
#define GUI_SET_CHANGE_LANG_RSP                -100100  //发送语言切换回应



// 标准协议包
typedef struct
{
	int m_iCmd;									//命令码
	int m_iLength;								//pdu负载长度
	char m_cPayload[0];						//实际数据
} TGuiParamPdu;								//数据传输单元结构

//===========================================

enum
{
	ITEM_HASH_CHECK_STAT = 1,			//哈希校验
	ITEM_HEARTRATE_DATA_STAT = 10000,	//心率数据
	ITEM_HEARTRATE_WAVE_STAT,			//心率波形
	ITEM_BLOODOXYGEN_DATA_STAT,			//血氧数据
	ITEM_BLOODOXYGEN_WAVE_STAT,			//血氧波形
	ITEM_BLOODPRESSURE_DATA_STAT,		//血压数据
	ITEM_VITAL_SIGN_REGPORT_CENTER_STAT,//生命体征上报中心
	ITEM_VITAL_SIGN_OSD_STAT,			//生命体征OSD叠加
	ITEM_VITAL_SIGN_AUTO_BLOODPRESSURE,	//自动测量血压
	ITEM_VITAL_SIGN_AUTO_TIMEINTVAL,    //自动测量血压时间间隔
};//功能状态id

#define GUI_GET_ITEM_STAT_REQ     48	//获取功能是否使能请求
#define GUI_GET_ITEM_STAT_RSP    -48	//获取功能是否使能回应

typedef struct
{
	int m_iChn;  //-1-表示设备属性，>0表示通道。
	int m_iId;
}TGuiGetItemStatReq;

typedef struct
{
	int m_iChn;  //-1-表示设备属性，>0表示通道。
	int m_iId;
	int m_iEnable;				// -1不支持， 0不使能, 1使能
}TGuiGetItemStatRsp;

#define GUI_SET_ITEM_STAT_REQ     49	//设置功能是否使能请求
#define GUI_SET_ITEM_STAT_RSP    -49	//设置功能是否使能回应

typedef struct
{
	int m_iChn;  //-1-表示设备属性，>0表示通道。
	int m_iId;
	int m_iEnable;				// 0不使能, 1使能
}TGuiSetItemStatReq;

typedef struct								
{	
	int m_iRetCode;
}TGuiSetItemStatRsp;
//===========================================


// -----------------------用户管理-----------------------------

#define GUI_MAX_USER_CNT 			32			//最大支持32个用户
#define GUI_USER_NAME_LEN			15
#define GUI_USER_PSW_LEN			15
typedef struct									//用户登录请求结构体
{
	char m_strName[GUI_USER_NAME_LEN + 1];		//用户名
	char m_strPsw[GUI_USER_PSW_LEN + 1];		//密码
} TGuiUserLoginReq;

#define GUI_USER_LOGIN_OK			0			//登录成功
#define GUI_USER_LOGIN_PSW_ERR	1			//密码错误
#define GUI_USER_LOGIN_NAME_ERR	2			//用户名不存在
#define GUI_USER_LOGIN_ERR		3			//登录失败
#define GUI_USER_LOGIN_UPTIMES	4			//连续登陆5次失败，账号锁定

typedef struct 								//用户登录回应
{
	int m_iLoginRs;							//返回码(高16位返回失败次数，低16位表示返回码)
} TGuiUserLoginRsp;

typedef struct 								//用户注销请求
{
	char m_strName[GUI_USER_NAME_LEN + 1];		//用户名
} TGuiUserLogoutReq;

#define GUI_USER_LOGOUT_OK		0			//注销成功
#define GUI_USER_LOGOUT_ERR		1			//注销失败
typedef struct 								//用户注销回应
{
	int m_iLogoutRs;							//返回码
} TGuiUserLogoutRsp;

typedef struct 								//添加用户请求
{
	int m_iGroupId;							//组编号
	char m_strName[GUI_USER_NAME_LEN + 1]; 	//用户名
	char m_strPsw[GUI_USER_PSW_LEN + 1];		//密码
} TGuiAddUserReq;

#define GUI_ADD_USER_OK			0			//添加用户成功
#define GUI_ADD_USER_GID_ERR		1			//用户组不存在
#define GUI_ADD_USER_EXIST		2			//同名用户已存在
#define GUI_ADD_USER_OTHER_ERR	3			//添加用户其它失败码
typedef struct 								//添加用户回应
{
	int m_iAddUserRs;							//回应码
} TGuiAddUserRsp;

typedef struct 								//删除用户请求
{
	char m_strName[GUI_USER_NAME_LEN + 1]; 	//用户名
} TGuiRmUserReq;

#define GUI_RM_USER_OK				0			//删除用户成功
#define GUI_RM_USER_NOT_EXIST		1			//删除的用户不存在
#define GUI_RM_USER_CAN_NO_RM		2			//指定用户无法被删除
#define GUI_RM_USER_OTHER_ERR		3			//删除用户失败其它错误码

typedef struct									//删除用户回应
{
	int m_iRmUserRes;
} TGuiRmUserRsp;

typedef struct 								//修改用户资料请求
{
	char m_strName[GUI_USER_NAME_LEN + 1];		//用户名
	int m_iNewGroupId;							//新组号
	char m_strNewPsw[GUI_USER_PSW_LEN + 1];	//新密码，为空则不修改
} TGuiEditUserReq;

#define GUI_EDIT_USER_OK			0			//修改用户成功
#define GUI_EDIT_USER_ERR			1			//修改用户失败
typedef struct 								//修改用户资料回应
{
	int m_iEditUserRes;
} TGuiEditUserRsp;


typedef struct 								//获取用户信息请求
{
	char m_strName[GUI_USER_NAME_LEN + 1];
} TGuiGetUserMsgReq;


//在线用户
#define GUI_GET_USERS_ONLINE_REQ		23
#define GUI_GET_USERS_ONLINE_RSP	   -23
typedef struct
{
	int m_iGid;  //用户组,目前可以不用
	char m_strUser[GUI_USER_NAME_LEN + 1];
	char m_strIp[GUI_PUBLIC_LEN_31 + 1];
}TGuiOnlineUserMsg;

typedef struct
{
	int m_iUsersCnt;
}TGuiGetUsersMsgRsp; //先返回在线用户个数，然后逐条返回用户信息

//断开在线用户协议
#define GUI_DISCONNECT_ONLINE_USER_CMD_REQ		24
#define GUI_DISCONNECT_ONLINE_USER_CMD_RSP		-24
typedef struct
{
	int m_iDisconnectTime; //断开时间
	TGuiOnlineUserMsg m_tOnlineUserMsg;
}TGuiSetDisOnlineUserCmdReq;

typedef struct
{
	int m_iRetCode;
}TGuiSetDisOnlineUserCmdRsp;

#define GUI_GET_USER_MSG_OK				0	//获取用户信息成功
#define GUI_GET_USER_MSG_ERR				1	//获取用户信息失败
#define GUI_USER_FLG_CNT					12	//用户权限标识数
#define GUI_USER_FLG_ADD_AND_RM_USER		0	//添加用户权限
#define GUI_USER_FLG_SYS_SETTING			1	//系统参数设置权限
#define GUI_USER_FLG_VIDEO_VIEW			2	//视频预览权限
#define GUI_USER_FLG_VIDEO_PLAY			3	//视频回放权限
#define GUI_USER_FLG_VIDEO_RM				4	//视频删除权限
#define GUI_USER_FLG_CAN_BE_DELETED		5	//该帐号是否能被删除标识索引
#define GUI_USER_FLG_USER_MNT				6	//用户管理权限
#define GUI_USER_FLG_SYS_SET_HI			7	//系统设置高级权限
#define GUI_USER_FLG_PTZ					8	//云台控制权限
#define GUI_USER_FLG_REC					9	//手动录像权限

#define GUI_COMMON_USER					1	//普通用户(只允许浏览视频)
#define GUI_SPECIAL_USER					2	//特权用户(可进行云台控制)
#define GUI_SUPPER_USER					3	//超级用户(可进行系统设置)
#define GUI_ADIMN_USER						4	//管理员(可进行用户管理)

typedef struct 								//获取用户信息回应
{
	int m_iGetUserMsgRs;						//返回码
	int m_iGroupId;							//组编号
	// 权限标识暂时保留，现以组id判断
	char m_cFlgs[GUI_USER_FLG_CNT];				//用户权限标识，每种权限对应一个字节
												// 1表示用户拥有该权限，0表示无
} TGuiGetUserMsgRsp;


typedef struct 								//获取组列表回应
{
	int m_iGrpCnt;	 							//组总个数
} TGuiGetUserGroupListRsp;

#define GUI_USER_GROUP_NAME_LEN		23
typedef struct 								//用户组数据
{
	int m_iGrpId; 								//组编号
	char m_strName[GUI_USER_GROUP_NAME_LEN + 1]; //组名称
} TGuiUserGroupMsg;

typedef struct 								//获取用户列表回应
{
	int m_iUserCnt;							//用户总数
} TGuiGetUserListRsp;

typedef struct 								//用户列表项
{
	int m_iGrpId; 								//用户所属组编号
	char m_strName[GUI_USER_NAME_LEN + 1]; 	//用户名
} TGuiUserMsg;

typedef struct 								//用户列表项
{
	int m_iGrpId; 								//用户所属组编号
	char m_strName[GUI_USER_NAME_LEN + 1]; 	//用户名
	char m_strPwd[GUI_USER_NAME_LEN + 1]; //密码
} TGuiUserMsgEx;


//===================用户权限细化====================
#define	GUI_GET_USER_RIGHT_REQ		20			//获取用户权限请求
#define	GUI_GET_USER_RIGHT_RSP		-20			//获取用户权限回应

#define GUI_USER_RIGHT_MAX_NUM    16         //用户权限最大条数

#define GUI_USER_RIGHT_PTZ			0		//通道PTZ控制权限
#define GUI_USER_RIGHT_VIDEO_PLAY		1		//回放控制权限

typedef struct
{
	char m_strName[GUI_USER_NAME_LEN + 1];
}TGuiGetUserRightReq;

typedef struct
{
	char m_strName[GUI_USER_NAME_LEN + 1];
	int m_iUserRight[GUI_USER_RIGHT_MAX_NUM];   //用户权限，每个元素代表一个权限，通道权限int每一位代表一个通道
}TGuiGetUserRightRsp;


#define GUI_SET_USER_RIGHT_REQ		21			//设置用户权限请求
#define GUI_SET_USER_RIGHT_RSP		-21			//设置用户权限回应

typedef struct
{
	char m_strName[GUI_USER_NAME_LEN + 1];
	int m_iUserRight[GUI_USER_RIGHT_MAX_NUM];  //用户权限，每个元素代表一个权限，通道权限int每一位代表一个通道
}TGuiSetUserRightReq;

typedef struct
{
	int m_iRetCode;									//返回码
}TGuiSetUserRightRsp;						//设置用户权限回应

//===================设置获取/设置用户权限范围======================
typedef enum 
{
	GUI_RIGHT_REMOTE_PTZ = 2,       //远程云镜控制   2
	GUI_RIGHT_REMOTE_VPLAY_PSCAN,	//远程回放浏览3
	GUI_RIGHT_REMOTE_PREV,			//远程预览 4
	GUI_RIGHT_LOCAL_HAND_RECSNAP,	//本地手动录像抓拍5
	GUI_RIGHT_REMOTE_HAND_RECSNAP,  //远程手动录像抓拍6
	GUI_RIGHT_LOCAL_ALARMCLEAR,   	//本地消警7
	GUI_RIGHT_LOCAL_DVD,			//本地实时刻录和光盘回放8
	GUI_RIGHT_LOCAL_LOG,			//本地日志搜索9
	GUI_RIGHT_LOCAL_ALARMSET,		//本地报警设置10
	GUI_RIGHT_LOCAL_CHNMANAGE,		//本地通道管理11
	GUI_RIGHT_LOCAL_PARASET,		//本地参数设置(编码，图像显示，录像模板，录像策略等) 12
	GUI_RIGHT_LOCAL_SYSSET,			//本地系统设置13
	GUI_RIGHT_LOCAL_USERMANAGE,		//本地用户管理14
	GUI_RIGHT_REMOTE_ALARMCLEAR,	//远程消警15
	GUI_RIGHT_REMOTE_REBOOT,		//远程重启/关机16
	GUI_RIGHT_REMOTE_DVD,			//远程实时刻录17
	GUI_RIGHT_REMOTE_LOG,			//远程日志搜索18
	GUI_RIGHT_REMOTE_ALARMSET,		//远程报警设置19
	GUI_RIGHT_REMOTE_CHNMANAGE,		//远程通道管理20
	GUI_RIGHT_REMOTE_PARASET,		//远程参数设置21
	GUI_RIGHT_REMOTE_SYSSET,		//远程系统设置22
	GUI_RIGHT_REMOTE_USERMANAGE,	//远程用户管理23
	GUI_RIGHT_REMOTE_TALK,			//语音对讲24
	GUI_RIGHT_LOCAL_REBOOT,			//本地关机、重启 25
	GUI_RIGHT_MAX_NUM,				
}GUI_USER_RIGHT_ID;

#define GUI_SET_USER_RIGHT_ID_REQ		22		//设置用户权限id范围请求(获取/设置前先发此命令标识获取/设置的id范围)
#define GUI_SET_USER_RIGHT_ID_RSP		-22		//设置用户权限id范围回应

typedef struct
{
	char m_strName[GUI_USER_NAME_LEN + 1];
	int m_iStartId;
	int m_iEndId;
}TGuiSetRightIdReq; //设置用户权限id范围请求(获取/设置前先发此命令标识获取/设置的id范围)

#define GUI_SET_RIGHT_ID_OK   0	//设置成功
#define GUI_SET_RIGHT_ID_ERR  1	//设置失败
typedef struct
{
	int m_iRetCode;		//返回码
}TGuiSetRightIdRsp;	//设置用户权限id范围回应


#define	GUI_GET_USER_RIGHT_REQ_EX		25 //获取用户权限请求(大路数扩展)
#define	GUI_GET_USER_RIGHT_RSP_EX		-25	//获取用户权限回应(大路数扩展)
typedef struct
{
	char m_strName[GUI_USER_NAME_LEN + 1];
	int m_iChnOffset;                //通道偏移量，通道数超过32时，使用此协议，传值32
}TGuiGetUserRightReqEx;

typedef struct
{
	char m_strName[GUI_USER_NAME_LEN + 1];
	int m_iUserRight[GUI_USER_RIGHT_MAX_NUM];   //用户权限，每个元素代表一个权限，通道权限int每一位代表一个通道
}TGuiGetUserRightRspEx;

#define GUI_SET_USER_RIGHT_REQ_EX		26  //设置用户权限请求(大路数扩展)
#define GUI_SET_USER_RIGHT_RSP_EX			-26  //设置用户权限回应(大路数扩展)
typedef struct
{	
	char m_strName[GUI_USER_NAME_LEN + 1];
	int m_iChnOffset;   //通道偏移量，通道数超过32时，使用此协议，传值32
	int m_iUserRight[GUI_USER_RIGHT_MAX_NUM];   
}TGuiSetUserRightReqEx;

typedef struct
{
	int m_iRetCode;     //0表成功，1表失败
}TGuiSetUserRightRspEx;


// ------------------------系统设置----------------------------------

//~~~~~~~~~~~~~~~~~~~~~~~~~1.磁盘管理~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

typedef struct 								//获取磁盘列表回应
{
	int m_iDiskCnt;							//磁盘总数
} TGuiGetDiskListRsp;

#define GUI_DISK_NAME_LEN		31

typedef struct 								//磁盘列表项
{
	int m_iDiskId; 							//磁盘唯一编号
	char m_strDiskName[GUI_DISK_NAME_LEN + 1]; //磁盘名
	unsigned long long m_ullDiskSize; 			//磁盘总大小
	unsigned long long m_ullDiskUsed;			//磁盘已用空间
	unsigned long long m_ullDiskFree;			//空闲空间
	int m_iFormatStat;							//是否格式化(0:no, 1:yes)
	int m_iSleepStat;							//休眠状态(0:no, 1:yes)
	int m_iSubAreaCnt;							//分区数
	int m_iUseForBackup;						//是否为备份盘(0:no, 1:yes)08/12/22添加 TGuiDiskMsg结构体里的m_iDiskUsage
} TGuiDiskListItem;

typedef struct									//格式化磁盘请求
{
	int m_iDiskId;								//磁盘编号
} TGuiFormatDiskReq;

typedef struct									//格式化磁盘回应
{
	int m_iDiskId; 							//磁盘编号，
	int m_iFormatId;							//格式化编号，每次格式化操作值唯一, 返回-1表示拒绝格式化操作
} TGuiFormatDiskRsp;

typedef struct 								//获取格式化磁盘状态请求
{
	int m_iDiskId;
	int m_iFormateId;
} TGuiGetDiskFormatStatReq;

#define GUI_DISK_FORMAT_ERROR		0			//格式化错误状态
#define GUI_DISK_FORMAT_DOING		1			//格式化进行中状态
#define GUI_DISK_FORMAT_DONE		2			//格式化完成状态

typedef struct 								//获取格式化磁盘状态回应
{
	int m_iDiskId;
	int m_iFormatId;
	int m_iStat;								//当前格式化状态
	int m_iFormatRate;							//格式化进度百分比,-1表式进度不可知
} TGuiGetDiskFormatStatRsp;

typedef struct									//磁盘分区请求
{
	int m_iDiskId;								//磁盘编号
	int m_iSubAreaCnt;							//分区数目
} TGuiDiskPartReq;

#define GUI_DISK_PART_OK			0			//磁盘分区成功
#define GUI_DISK_PART_ERR			1			//磁盘分区失败
typedef struct									//磁盘分区回应
{
	int m_iDiskPartRetCode;					//分区返回码
} TGuiDiskPartRsp;

typedef struct									//获取磁盘信息请求
{
	int m_iDiskId;								//磁盘编号
} TGuiGetDiskMsgReq;

#define GUI_DISK_USE_FOR_REC			1			//磁盘用于录像
#define GUI_DISK_USE_FOR_BACK			2			//磁盘用于备份
#define GUI_DISK_USE_FOR_REDUNDANCE	    3      		//磁盘冗余
#define GUI_DISK_USE_FOR_READONLY		4        	//磁盘用于只读

typedef struct									//磁盘信息
{
	int m_iDiskUsage;							//磁盘用途
	int m_iAlarmWhenErr;						//磁盘错误时报警标识
	int m_iAlarmWhenOutOfSpace;				//磁盘空间不足时报警标识
	unsigned int m_uiDiskReservedSize;		//磁盘空间下限(M)
	int m_iAlartMethod[2];					//磁盘报警开关量, 0:屏幕显示,1声音提示
} TGuiDiskMsg;

typedef struct									//获取磁盘信息回应
{
	int m_iDiskId;								//磁盘编号，负值表示错误
	TGuiDiskMsg m_tMsg;							//磁盘信息
} TGuiGetDiskMsgRsp;

typedef struct									//设置磁盘信息请求
{
	int m_iDiskId;								//磁盘编号
	TGuiDiskMsg m_tMsg;							//磁盘信息
} TGuiSetDiskMsgReq;

#define GUI_SET_DISK_MSG_OK		0			//设置磁盘信息成功
#define GUI_SET_DISK_MSG_ERR		1			//设置磁盘信息失败
typedef struct									//设置磁盘信息回应
{
	int m_iSetDiskMsgRetCode;					//设置磁盘信息返回码
} TGuiSetDiskMsgRsp;


#define GUI_SET_DISK_SMART_ENABLE_REQ  56   //设置磁盘启用smart信息请求
#define GUI_SET_DISK_SMART_ENABLE_RSP -56   //设置磁盘启用smart信息回应

typedef struct
{
	int m_iDiskNo;  //盘号
	int m_iEnable;   //启用读取smart信息，1-启用，0-不启用
}TGuiSetDiskSmartEnableReq;

typedef struct
{
	int m_iRetCode; //0 - 启用成功，1-失败
}TGuiSetDiskSmartEnableRsp;

#define GUI_GET_DISK_SMART_ENABLE_START_REQ  57  //获取磁盘smart 是否启用状态请求
#define GUI_GET_DISK_SMART_ENABLE_START_RSP  -57 //获取磁盘smart 是否启用状态回应
#define GUI_SMART_STR_NAME_LEN 31
typedef struct
{
	int m_iDiskNo;
}TGuiGetDiskSmartEnableStartReq;
typedef struct
{
	int m_iDiskNo;
	int m_iEnable; //0 - 启用成功，1-失败
}TGuiGetDiskSmartEnableStartRsp; 

#define GUI_GET_DISK_SMART_MSG_REQ   58 //获取磁盘smart信息请求
#define GUI_GET_DISK_SMART_MSG_RSP   -58 //获取磁盘smart 信息回应
typedef struct
{
	long long m_llCapacity;  //硬盘容量
	char m_cDevModel[GUI_SMART_STR_NAME_LEN + 1];
	char m_cSerialNum[GUI_SMART_STR_NAME_LEN + 1];
	int m_iTemp; //温度
	int m_iUsedTime;// 按天计算
	int m_iHealth; // 1-通过 0-不通过
	int m_iAllHealth; // 1-健康状况良好
}TGuiDiskSmartBasicMsg;

typedef struct
{
	int m_iDiskNo;
}TGuiGetDiskSmartMsgReq;

typedef struct
{
	int m_iDiskNo;
	int m_iSmartMsgCnt;
	TGuiDiskSmartBasicMsg m_tSmartBasicMsg;
}TGuiGetDiskSmartMsgRsp;

typedef struct
{
	int m_iSmartID;  //HEX 值
	char m_cSmartName[GUI_SMART_STR_NAME_LEN + 1];    //smart 属性名称
	int m_iSmartFlag;   //标识
	int m_iSmartValue;
	int m_iSmartWorst;
	int m_iSmartThresh;
	char m_cSmartStatus[GUI_SMART_STR_NAME_LEN + 1]; //硬盘状态
	long long int m_llSmartRawValue;
}TGuiDiskSmartMsg;


#define GUI_GET_DISK_PART_MSG_REQ    59  //获取磁盘分区信息请求
#define GUI_GET_DISK_PART_MSG_RSP    -59  //获取磁盘分区信息回应

typedef struct  
{
	int m_iDiskId;   //磁盘id号高16位(0 代表 旧协议， U盘1 代表 U盘，2 代表 esata)
	int m_iDiskPartId; //磁盘分区id 号从0开始，-1-表示所有分区
}TGuiGetDiskPartMsgReq;

typedef struct
{
	int m_iDiskId;   //磁盘id号
	int m_iDiskPartCnt;  //磁盘分区数
}TGuiGetDiskPartMsgRsp;

#define GUI_DISK_WRITE_PROTECT_YES   1
#define GUI_DISK_WRITE_PROTECT_NO    0
typedef struct
{
	int m_iDiskId; //磁盘id号
	int m_iDiskPartId; //磁盘分区id 号从0开始，-1-表示所有分区
	int m_iDiskPartWriteProtect;					//磁盘分区写保护
	char m_strDiskName[GUI_DISK_NAME_LEN + 1]; //磁盘名
	unsigned long long m_ullDiskPartSize; 			//磁盘分区大小
	unsigned long long m_ullDiskPartUsed;			//磁盘分区已用空间
	unsigned long long m_ullDiskPartFree;			//磁盘分区空闲空间
}TGuiDiskPartListItem;




// ---------------------2.串口设置------------------------------------

#define GUI_SPORT_DATA_NONE_CHECK	0 		//无校验
#define GUI_SPORT_DATA_ODD_CHECK		1 		//奇校验
#define GUI_SPORT_DATA_EVEN_CHECK	2 		//偶校验

typedef struct									//串口配置信息结构体
{
	int m_iPortId;								//串口编号
	unsigned int m_uiBitRate;					//波特率
	unsigned char m_ucDataBits;				//数据位
	unsigned char m_ucStopBits;				//停止位
	unsigned char m_ucCheckType;				//校验位
} TGuiSerialPortMsg;


typedef struct									//获取串口配置信息请求
{
	int m_iPortId;								//串口编号
} TGuiGetSerialPortMsgReq;

#define GUI_GET_SPORT_MSG_OK		0
#define GUI_GET_SPORT_MSG_ERR		-1

typedef struct									//获取串口配置信息回应
{
	int m_iRetCode;							//返回码
	TGuiSerialPortMsg m_tPortMsg;				//串口配置信息
} TGuiGetSerialPortMsgRsp;

typedef struct									//串口设置请求
{
	int m_iPortId;								//串口编号
	TGuiSerialPortMsg m_tSetMsg;				//欲设置的配置信息
} TGuiSetSerialPortMsgReq;


#define GUI_SET_SPORT_MSG_OK		0
#define GUI_SET_SPORT_MSG_ERR		-1

typedef struct									//设置串口回应
{
	int m_iRetCode;							//返回码
} TGuiSetSerialPortMsgRsp;


enum
{
	GUI_SERIAL_WORKMODE_PROCO = 1, //协议模式
	GUI_SERIAL_WORKMODE_CLARITY, //透明通道
	GUI_SERIAL_WORKMODE_APP,       //外挂模式
	GUI_SERIAL_WORKMODE_ALARMHOST,  //报警主机
	GUI_SERIAL_WORKMODE_485COM,		//485串口
	GUI_SERIAL_WORKMODE_GPS,		//车载GPS
	GUI_SERIAL_WORKMODE_ATM,		//ATM 
};

#define GUI_GET_SPORT_WORKMODE_REQ  62  //获取串口的工作模式请求
#define GUI_GET_SPORT_WORKMODE_RSP  -62  //获取串口的工作模式回应
typedef struct
{
	int m_iPortId;	//串口编号
}TGuiGetSerialWorkModeReq;

typedef struct
{
	int m_iPortId;
	int m_iWorkMode;
}TGuiGetSerialWorkModeRsp;

#define GUI_SET_SPORT_WORKMODE_REQ  63 //设置串口的工作模式请求
#define GUI_SET_SPORT_WORKMODE_RSP -63 //设置串口的工作模式回应
typedef struct
{
	int m_iPortId;
	int m_iWorkMode;
}TGuiSetSerialWorkModeReq;
typedef struct
{
	int m_iRetCode;
}TGuiSetSerialWorkModeRsp;
	



// ---------------------3.系统信息------------------------------------

#define GUI_DVE_NAME_LEN			31			//设备名长度
#define GUI_SYS_MSG_STR_LEN			31			//系统信息各字符串长度

typedef struct									//获取系统信息回应
{
	char m_strDveName[GUI_DVE_NAME_LEN + 1];	//设备名
	int m_iDevNo;								//设备编号
	char m_strManufacturer[GUI_SYS_MSG_STR_LEN + 1];	//制造商
	char m_strSN[GUI_SYS_MSG_STR_LEN + 1];		//序列号
	char m_strKernelVer[GUI_SYS_MSG_STR_LEN + 1];		//内核版本
	char m_strKernelVerSlaver[GUI_SYS_MSG_STR_LEN + 1];	 //从片内核版本号
	char m_strWebVer[GUI_SYS_MSG_STR_LEN + 1];	//网页版本
	char m_strSDKVer[GUI_SYS_MSG_STR_LEN + 1];	//sdk版本
} TGuiGetSysMsgRsp;

typedef struct									//设置系统信息请求
{
	char m_strDveName[GUI_DVE_NAME_LEN + 1];	//设备名
	int m_iDevNo;								//设备编号	
} TGuiSetSysMsgReq;

#define GUI_SET_SYS_MSG_OK		0			//设置系统信息成功
#define GUI_SET_SYS_MSG_ERR		1			//设置系统信息失败

typedef struct									//设置系统信息回应
{
	int m_iSetSysMsgRetCode;					//返回码
} TGuiSetSysMsgRsp;


//===================================
//获取单片机版本号(车载)
#define GUI_SET_SCM_VER_REQ	 79	//告诉内核单片机版本请求
#define GUI_SET_SCM_VER_RSP	-79	//告诉内核单片机版本回应

#define GUI_SCM_VER_LEN   31
//单片机版本
typedef struct
{
	char m_strScmVer[GUI_SCM_VER_LEN + 1];//单片机版本号
}TGuiSetScmVerReq;

typedef struct								
{	
	int m_iRetCode;
}TGuiSetScmVerRsp;
//===================================

//获取组件版本
#define GUI_GET_COMPONENT_VER_REQ	 85	//向内核获取组件版本请求
#define GUI_GET_COMPONENT_VER_RSP	-85	//回应

typedef enum
{
	GUI_VER = 1,
	ZFZJ_VER,          //政法中的外挂
	ONVIF_VER,
	P2P_VER,
}TGuiComponetEnum;

typedef struct
{
	int m_iComponentType;      //见上面枚举 TGuiComponetEnum
}TGuiGetComponentVerReq;

typedef struct								
{	
	int m_iComponentType; 
	char m_strComponentVer[GUI_SYS_MSG_STR_LEN + 1];	//组件版本号
}TGuiGetComponentVerRsp;


// ---------------------4.日志管理------------------------------------
// ===================统计日志总数=====================
#define GUI_LOG_COUNT_REQ			80			//统计日志总数请求
#define GUI_LOG_COUNT_RSP			-80			//统计日志总数回应

#if 1
#define GUI_LOG_TYPE_ALL		0				//全部日志
#define GUI_LOG_LANG_CHI		1				//中文日志
#define GUI_LOG_LANG_ENG		2				//英文日志
#define GUI_LOG_LANG_TCHI		3				//繁体中文日志
#define GUI_LOG_LANG_KOR		4				//韩文日志
#define GUI_LOG_LANG_SPAN		5				//西班牙文日志
#define GUI_LOG_LANG_ITA		6				//意文日志
#define GUI_LOG_LANG_THAI		7				//泰文日志
#endif

typedef struct									//统计日志请求
{
	int m_iBeginTm;							//开始时间
	int m_iEndTm;								//结束时间
	int m_iType;								//日志类型
	int m_iChn;								//通道，0表全部 dxl 2009/12/01
} TGuiLogCountReq;

typedef struct									//统计日志回应
{
	int m_iLogCnt;								//日志总条数
} TGuiLogCountRsp;

//======================查询日志=======================
#define GUI_LOG_QUERY_REQ			81			//查询日志请求
#define GUI_LOG_QUERY_RSP			-81			//查询日志回应

typedef struct									//日志查询请求
{
	int m_iBeginTm;							//日志开始时间
	int m_iEndTm;								//日志结束时间
	int m_iType;								//日志类型
	int m_iChn;								//通道dxl 2009/12/01
	int m_iLang;								//语言,
	int m_iStartId;							//日志起始编号
	int m_iCnt;									//请求日志条数
} TGuiLogQueryReq;

typedef struct									//日志查询回应
{
	int m_iRcdCnt;								//查获的日志总条数
} TGuiLogQueryRsp;

#define GUI_LOG_CONTENT_LEN	127				//日志内容最大长度
typedef struct									//日志结构体
{
	int m_iRecTm;								//记录时间
	int m_iType;								//日志类型码
	int m_iChn;									//通道,0表示与通道无关dxl 2009/12/01
	char m_strContent[GUI_LOG_CONTENT_LEN + 1];	//日志内容
} TGuiLog;
//======================清除日志=======================

#define GUI_LOG_CLEAR_REQ			82			//清空日志请求
#define GUI_LOG_CLEAR_RSP			-82			//清空日志回应

#define GUI_LOG_CLEAR_OK		0				//清空日志成功
#define GUI_LOG_CLEAR_ERR		1				//清空日志失败
typedef struct									//清空日志回应
{
	int m_iRetCode;							//日志清空返回码
} TGuiLogClearRsp;

//======================记录日志=======================

#define GUI_LOG_NOTE_REQ		83     //记录日志请求
#define GUI_LOG_NOTE_RSP        -83    //记录日志回应

typedef struct
{
	int m_iChn;			//通道，0-表示与通道无关
	int m_iLogLevel;       //日志类型
	char strContent[GUI_LOG_CONTENT_LEN + 1]; //日志内容
}TGuiLogNoteReq;              

typedef struct
{
	int m_iRetCode;
}TGuiLogNoteRsp;   //记录日志回应

//================扩展日志查询========================
#define GUI_LOG_QUERY_EX_REQ		84			//(扩展)日志查询请求
#define GUI_LOG_QUERY_EX_RSP		-84			//(扩展)日志查询回应
typedef TGuiLogQueryReq TGuiLogQueryExReq;

typedef TGuiLogQueryRsp TGuiLogQueryExRsp;

//新加日志用户
typedef struct									//日志结构体
{
	int m_iRecTm;								//记录时间
	int m_iType;								//日志类型码
	int m_iChn;								//通道,0表示与通道无关dxl 2009/12/01
	char m_strContent[GUI_LOG_CONTENT_LEN + 1];	//日志内容
	char m_strUserName[GUI_USER_NAME_LEN + 1];   //日志用户
} TGuiLogEx;



// ---------------------5.系统升级------------------------------------
#define GUI_SYS_UPDATE_USE_USB	0			//USB方式升级
#define GUI_SYS_UPDATE_USE_FTP	1			//FTP方式升级
#define GUI_SYS_UPDATE_FTP_LEN	64			//FTP地址长度
#define GUI_SYS_UPDATE_ACCEPT		0			//系统升级请求被接受
#define GUI_SYS_UPDATE_USB_ERR	1			//系统升级找不到USB设备
#define GUI_SYS_UPDATE_FTP_ERR	2			//系统升级无法连接FTP
#define GUI_SYS_UPDATE_FILE_ERR	3			//在指定设备上找不到升级文件
#define GUI_SYS_UPDATE_OTHER_ERR	4			//其它错误

#define GUI_BACKUP_KERNEL_REQ	1400	
#define GUI_BACKUP_KERNEL_RSP	-1400	
#define GUI_BACKUP_KERNEL_OK	0	//内核备份成功
#define GUI_BACKUP_KERNEL_ERR	1	//内核备份失败

typedef struct
{
	int m_iRetCode;							//返回码，0-成功，1-失败
}TGuiBackupKernelRsp;

typedef struct									//系统升级请求
{
	int m_iMethod;								//升级方式
	char m_strFtpAddr[GUI_SYS_UPDATE_FTP_LEN + 1]; //FTP地址
} TGuiSysUpdateReq;

typedef struct								//系统升级请求
{
	char m_strFtpAddr[GUI_SYS_UPDATE_FTP_LEN + 1];	//FTP地址
	char m_strFtpUser[GUI_SYS_UPDATE_FTP_LEN + 1]; 	//FTP用户名
	char m_strFtpPasswd[GUI_SYS_UPDATE_FTP_LEN + 1]; 	//FTP密码
} TGuiSysFtpUpdateReq;

typedef struct										//升级请求
{
	char m_strFileName[GUI_SYS_UPDATE_FTP_LEN + 1]; 	 //升级文件名
} TGuiSysUpdateFileReq;

typedef struct										//升级文件请求
{
	char m_strFileName[GUI_SYS_UPDATE_FTP_LEN + 1];	 //升级文件名
} TGuiSysUpdateFileRsp;

typedef struct 								//获取升级文件列表回应
{
	int m_iUpdateId;							//升级序号
	int m_iCfgCnt;							//文件总数
} TGuiGetSysUpdateFileListRsp;

#define GUI_UPDATE_REBOOT_HAND		2000  //不自动重启

typedef struct									//系统升级回应
{
	int m_iUpdateId;							//升级序号
	int m_iRetCode;							//返回码
} TGuiSysUpdateRsp;

typedef struct									//系统升级状态查询
{
	int m_iUpdateId;							//升级序号
} TGuiSysUpdateStaReq;

#define GUI_SYS_UPDATE_DOING		0			//系统升级进行中
#define GUI_SYS_UPDATE_DONE		1			//系统升级完成
#define GUI_SYS_UPDATE_ERR		2			//系统升级错误
typedef struct
{
	int m_iUpdateId;							//升级序号 根据Id 为GUI_UPDATE_REBOOT_HAND来判断UI是否自动重启
	int m_iUpdateSta;							//升级状态
	int m_iUpdateRate;							//升级进度百分比(0-100,其它值表示不可预测)
} TGuiSysUpdateStaRsp;

// ---------------------6.参数重设------------------------------------
#define GUI_SET_RENEW_ACCEPT		0			//系统接受参数重设
#define GUI_SET_RENEW_REFUSE		1			//系统拒绝参数重设
typedef struct
{
	int m_iRetCode;							//返回码
} TGuiSettingRenewRsp;

#define GUI_SET_EXPORT_CNF_REQ       866             //导出获取磁盘列表请求
#define GUI_SET_EXPORT_CNF_RSP		  867			 //导出获取磁盘列表回应
#define GUI_CNF_EXPORT_REQ   868             //导出请求
#define GUI_CNF_EXPORT_RSP   869			//导出回应
#define GUI_SET_IMPORT_CNF_REQ 874			//导入获取磁盘列表请求
#define GUI_SET_IMPORT_CNF_RSP 875			//导入获取磁盘列表回应
#define GUI_CNF_IMPORT_REQ   870			//导入请求
#define GUI_CNF_IMPORT_RSP	  871			//导入回应
#define GUI_CNF_IMPORT_FILE_REFRESH_REQ   872   //导入文件刷新请求
#define GUI_CNF_IMPROT_FILE_REFRESH_RSP   873   //导入文件刷新回应
#define GUI_CNF_SUCESS 0
#define GUI_CNF_FAIL   1
#define GUI_CNF_NAME_LEN 32
#define GUI_IMPORT_FILE_STAT_YES	1				//导入文件选中
#define GUI_IMPORT_FILE_STAT_NO		2				//导入文件未选中

typedef struct							
{
	char m_iDstDevName[GUI_CNF_NAME_LEN + 1];	//导出配置文件磁盘名称
} TGuiCfgPositionExportReq;
typedef struct							
{
	char m_strFileName[GUI_CNF_NAME_LEN + 1];  //导出配置文件名
} TGuiCfgFileExportReq;

typedef struct							
{
	char m_strFileName[GUI_CNF_NAME_LEN + 1];  //导入配置文件名
} TGuiCfgFileImportReq;

typedef struct 								
{
	int m_iCfgCnt;							//总数
} TGuiGetCfgListRsp;

typedef struct								//回应
{
	int m_iRetCode;
} TGuiCfgRsp;




// ---------------------7.网络管理------------------------------------
//====================SMTP设置============================
#define GUI_GET_SMTP_CNF_REQ			108    	//获取SMTP配置信息请求
#define GUI_GET_SMTP_CNF_RSP			-108	//获取SMTP配置信息回应
#define GUI_SET_SMTP_CNF_REQ			109		//设置SMTP配置信息请求
#define GUI_SET_SMTP_CNF_RSP			-109	//设置SMTP配置信息回应

#define GUI_RECEVIE_COUNT				4   //支持4个收件人地址
#define GUI_STR_SMTP_ADDRESS_LEN		63
typedef struct
{
	char	m_cServerName[GUI_STR_SMTP_ADDRESS_LEN + 1];    //smtp服务器名称
	char	m_cUserName[GUI_STR_SMTP_ADDRESS_LEN + 1];      //用户名
	char	m_cPassword[GUI_STR_SMTP_ADDRESS_LEN + 1];		//密码
	char	m_cReciveAddress[GUI_RECEVIE_COUNT][GUI_STR_SMTP_ADDRESS_LEN + 1];	//收件人地址
	char	m_cEmailTitle[GUI_STR_SMTP_ADDRESS_LEN + 1];		//邮件主题
	int		m_iPort;											//端口号
	int		m_iType;							//低16位表示登录模式,高16位表示加密方式，0-保留，1-不加密，2-SSL，3-TLS
}TGuiSMTPCnfMsg;		//SMTP配置信息

typedef struct
{
	TGuiSMTPCnfMsg m_tSMTPCnf;
}TGuiGetSMTPCnfRsp;		//获取SMTP配置信息回应

typedef struct
{
	TGuiSMTPCnfMsg m_tSMTPCnf;
}TGuiSetSMTPCnfReq;		//设置SMTP配置信息请求

#define GUI_SET_SMTP_CNF_OK		0 //设置成功
#define GUI_SET_SMTP_CNF_ERR	1  //设置失败
typedef struct
{
	int m_iRetCode;
}TGuiSetSMTPCnfRsp;		//设置SMTP配置信息回应

//===================网络服务========================

#define GUI_GET_NET_SERVICE_ENABLE_REQ		106			//获取网络服务启用状态请求
#define GUI_GET_NET_SERVICE_ENABLE_RSP	    -106		//获取网络服务启用状态回应
#define GUI_SET_NET_SERVICE_ENABLE_REQ		107			//设置网络服务启用状态请求
#define GUI_SET_NET_SERVICE_ENABLE_RSP		-107		//设置网络服务启用状态回应

enum
{
	SNMP_SERVICE = 1, //SNMP网络服务
	HTTPS_SERVICE,    //HTTPS服务
	FTP_SERVICE,	  //ftp服务
	HTTP_SERVICE,	  //http服务
	RTSP_SERVICE,   //rtsp服务 (当有连接时，向内核获取一次协议。)
	T28181_SERVICE,   //28181纯视频 (当有连接时，向内核获取一次协议。)	
	SCHEDULE_SERVICE, //庭审案件排期服务
};//网络服务id

typedef struct
{
	int m_iId;
}TGuiGetNetServiceReq; //获取网络服务启用状态请求

typedef struct
{
	int m_iId;   //网络服务id号
	int m_iEnable;  //是否使能
}TGuiGetNetServiceRsp;    //获取网络服务启用状态回应

typedef TGuiGetNetServiceRsp TGuiSetNetServiceReq;

#define GUI_SET_NET_SERVICE_OK    	0   //设置成功
#define GUI_SET_NET_SERVICE_ERR  	1	//设置失败
typedef struct
{
	int m_iRetCode; 
} TGuiSetNetServiceRsp;  //设置网络服务启用状态回应

#define GUI_GET_NET_SERVICE_PORT_REQ	104	 //获取网络服务端口请求
#define GUI_GET_NET_SERVICE_PORT_RSP	-104 //获取网络服务端口回应
#define GUI_SET_NET_SERVICE_PORT_REQ	105	//设置网络服务端口请求
#define GUI_SET_NET_SERVICE_PORT_RSP	   -105	//设置网络服务端口回应

typedef struct
{
	int m_iId; 
}TGuiGetNetServicePortReq;   //获取网络服务端口号

typedef struct
{
	int m_iId;
	int m_iPort;
} TGuiGetNetServicePortRsp;

typedef TGuiGetNetServicePortRsp TGuiSetNetServicePortReq;

typedef struct
{
	int m_iRetCode;
} TGuiSetNetServicePortRsp;


//===================流量监控===========================
#define GUI_GET_NET_FLUX_STATE_REQ		119   //获取网络流量状态请求
#define GUI_GET_NET_FLUX_STATE_RSP		-119  //获取网络流量状态回应

#define GUI_NET_FLUX_STATE_CNT    31
typedef struct
{
	char m_cUploadRate[GUI_NET_FLUX_STATE_CNT + 1];   //上传速率
	char m_cDownloadRate[GUI_NET_FLUX_STATE_CNT + 1]; //下载速率
	char m_cUploadTotal[GUI_NET_FLUX_STATE_CNT + 1];     //上传总流量 
	char m_cDownloadTotal[GUI_NET_FLUX_STATE_CNT + 1]; //下载总流量  
}TGuiNetFluxState;    //网络信息，包括速率和总流量

typedef struct
{
	int m_iCardNo;  //网卡号  0-预留 1-网卡1，  2-网卡2
}TGuiGetNetFluxStateReq; //获取网络流量状态请求

typedef struct
{
	int m_iCardNo;  //网卡号0-预留 1-网卡1，  2-网卡2
	TGuiNetFluxState m_iNetFluxState; 
}TGuiGetNetFluxStateRsp;  //获取网络流量状态回应

//=================网络丢包、延时测试===================
#define GUI_NET_DELAY_TEST_CMD_REQ	124  //网络延时测试命令请求
#define GUI_NET_DELAY_TEST_CMD_RSP	-124  //网络延时测试命令

typedef struct
{
	int m_iCardNo;         //网卡编号0保留，1，2 ，10 PPP0型外接USB网卡，11 USB0型外接USB网卡
	char m_strDstIp[GUI_PUBLIC_LEN_31 + 1];			//目的ip地址
}TGuiNetDelayTestCmdReq;

typedef struct
{
	int m_iCardNo;
	int m_iReachable; 	//网络地址是否可达,0:可达；其它:不可达
	int m_iDelayTm;		//网络延时,单位(ms)
	int m_iLostrate;		//丢包率,取值范围:0-100,值为50表示:丢包率:50%
}TGuiNetDelayTestCmdRsp;

//====================抓包备份====================
#define GUI_NETPACK_CAPTURE_CMD_REQ	125  //网络抓包命令请求
#define GUI_NETPACK_CAPTURE_CMD_RSP	-125  //网络抓包命令请求回应

typedef struct
{
	int m_iCardNo;	     //网卡号
	int m_iDiskNo;       //磁盘号
	int m_iPackSize;      //网络包大小，最大1M
	int m_iTmDelay;      //超时时间，默认1分钟，先到达为准 
}TGuiNetPackCaptureReq;

typedef struct
{
	int m_iRetCode;
} TGuiNetPackCaptureRsp;

//=================状态检测=======================
#define GUI_NET_STATUS_TEST_CMD_REQ  126   //网络状态检测
#define GUI_NET_STATUS_TEST_CMD_RSP	  -126  //回应

typedef struct
{
	int m_iCardNo;
}TGuiGetNetStatusTestReq;  //获取网络检测请求

//0表示所有网络都可达，1表示网关不可达，2表示DNS不可达，3表示外网不可达
typedef struct
{
	int m_iCardNo;   //网卡号
	int m_iRetCode;	//按位表示，从最低位开始
}TGuiGetNetStatusTestRsp;

//==============网络资源检测=========================
#define GUI_NET_RESOURCE_CHECK_REQ   127  //网络资源检测请求
#define GUI_NET_RESOURCE_CHECK_RSP	   -127  //网络资源检测回应

typedef struct
{
	int m_iIpChnBandWidth;	   //ip通道带宽（单位为M，以下类似）
	int m_iRePreBandWidth;    //远程预览
	int m_iRePlayBandWidth;   //远程回放
	int m_iNetReciveBandWidth;   //网络接收带宽
	int m_iNetSendBandWidth;    //网络发送带宽
}TGuiNetResourceMsg;

typedef struct
{
	TGuiNetResourceMsg m_tNetResMsg;
}TGuiGetNetResourceMsgRsp;  //回复各种带宽

#define GUI_EMAIL_TEST_CMD_REQ	128    //邮件测试命令请求
#define GUI_EMAIL_TEST_CMD_RSP	-128	//邮件测试命令回应

typedef struct
{
	int m_iRetCode; //测试结果，0表成功，其余表失败
}TGuiEmailTestRsp;


//============网络设置管理====================

#define GUI_GET_IP_VIA_DHCP			0			//dhcp方式获取IP
#define GUI_GET_IP_VIA_HUM			1			//手动设置IP
#define GUI_GET_IP_VIA_PPPOE		2			//PPPoE方式
#define GUI_IP_STR_LEN				31			//IP地址字符串长度
#define GUI_GET_DNS_VIA_DHCP    	0
#define GUI_GET_DNS_VIA_HUM    		1

#define GUI_GET_IP_VIA_PPPOE_ENABLE		true			//PPPoE方式能够设置
#define GUI_GET_IP_VIA_PPPOE_UNENABLE		false			//PPPoE方式不能够设置

typedef struct									//网络参数
{
	int m_iGetIpMethod;						//获取ip地址的方式
	int m_blIsHandSetDns;						//手动设置DNS
	char m_strMac[GUI_IP_STR_LEN + 1];			//mac地址
	char m_strIp[GUI_IP_STR_LEN + 1];			//ip地址
	char m_strSubMsk[GUI_IP_STR_LEN + 1];		//子网掩码
	char m_strGetWay[GUI_IP_STR_LEN + 1];		//网关
	char m_strDNS[GUI_IP_STR_LEN + 1];			//首选域名解析
	char m_strSecDNS[GUI_IP_STR_LEN + 1];			//备选域名解析
} TGuiIpMsg;

typedef struct									//获取IP参数回应
{
	TGuiIpMsg m_tIpMsg;						//ip信息
} TGuiGetIpRsp;

typedef struct									//设置IP参数请求
{
	TGuiIpMsg m_tIpMsg;
} TGuiSetIpReq;

#define GUI_SET_IP_OK			0				//设置IP地址成功
#define GUI_SET_IP_ERR			1				//设置IP地址失败
typedef struct									//设置IP地址回应
{
	int m_iRetCode;							//返回码
	TGuiIpMsg m_tIpMsg;						//重设后的IP信息
} TGuiSetIpRsp;

#define GUI_GET_SECOND_NETWORK_IP_REQ  138  //获取第二个网口ip地址信息请求
#define GUI_GET_SECOND_NETWORK_IP_RSP  -138 //获取第二个网口ip地址信息回应

#define GUI_SECOND_NETWORK_CARD_NO 2  //第二个网卡号
#define GUI_BOND_NETWORK_CARD_NO 3  //双网卡聚合模式下的网卡号

enum
{
	GUI_NETWORK_CARD_MODE_DIFFERENT = 0, // 第二个网卡的模式和网卡1不同
	GUI_NETWORK_CARD_MODE_SAME,   //第二个网卡的模式和网卡1相同
};

enum
{
	GUI_NET_WORK_Redundancy = 1, //冗余模式
	GUI_NET_WORK_Balance,        //负载均衡模式
};

typedef struct
{
	int m_iNetWorkCardNo; //网口号
}TGuiGetSecondNetWorkIpReq;
typedef struct
{
	int m_iNetWorkCardNo;
	int m_iNetWorkCardMode;//是否和网卡1相同的配置
	int m_iNetWorkMode; //工作模式，1-冗余模式，2-负载均衡模式
	TGuiIpMsg m_tIpMsg;		
}TGuiGetSecondNetWorkIpRsp;

#define GUI_SET_SECOND_NETWORK_IP_OK			0				//设置IP地址成功
#define GUI_SET_SECOND_NETWORK_IP_ERR			1				//设置IP地址失败
#define GUI_SET_SECOND_NETWORK_IP_REQ   139  //设置第二个网口ip地址信息请求
#define GUI_SET_SECOND_NETWORK_IP_RSP   -139 //设置第二个网口ip地址信息回应

typedef TGuiGetSecondNetWorkIpRsp TGuiSetSecondNetWorkIpReq;

typedef struct
{
	int m_iRetCode;
	int m_iNetWorkCardNo;
	int m_iNetWorkCardMode;//是否和网卡1相同的配置
	int m_iNetWorkMode; //工作模式，1-冗余模式，2-负载均衡模式
	TGuiIpMsg m_tIpMsg;		
}TGuiSetSecondNetWorkIpRsp;
	
//==================IPv6相关设置=====================
#define GUI_GET_NETCARD_IPV6_MSG_REQ	2020//获取网卡IPV6地址请求
#define GUI_GET_NETCARD_IPV6_MSG_RSP	  	-2020//获取网卡IPV6地址回应
typedef struct									//网络参数
{
	int m_iGetIpMethod;						//获取ip地址的方式
	int m_blIsHandSetDns;						//手动设置DNS
	char m_strMac[GUI_PUBLIC_LEN_31 + 1];			//mac地址
	char m_strIp[GUI_PUBLIC_LEN_63  + 1];			//ip地址
	char m_strSubMsk[GUI_PUBLIC_LEN_63  + 1];		//子网掩码
	char m_strGetWay[GUI_PUBLIC_LEN_63  + 1];		//网关
	char m_strDNS[GUI_PUBLIC_LEN_63 + 1];			//首选域名解析
	char m_strSecDNS[GUI_PUBLIC_LEN_63  + 1];			//备选域名解析
} TGuiIpV6Msg;

typedef struct
{ 
	int m_iNetCardNo;        //网卡号
}TGuiGetIpv6MsgReq;
	
typedef struct
{
	int m_iNetCardNo;
	TGuiIpV6Msg m_tIpV6Msg;
}TGuiGetIpv6MsgRsp;

#define GUI_NET_SV_STR_LEN		64
#define GUI_NET_SV_IP_CNT			16
#define GUI_NET_IP_DISENABLE		0			//禁用ip权限设定
#define GUI_NET_IP_FORBID			1			//禁止IP访问
#define GUI_NET_IP_ACCESS			2			//允许IP访问
typedef struct
{
	char m_strDnsServer[GUI_NET_SV_STR_LEN + 1];	//DNS服务器
	int m_iFtpPort;		//ftp端口号
	int m_iHttpPort;		//http端口号
	int m_iNfsEnable;		//网络存储服务使能
	int m_bupnpenable;                //UPNP使能
	char m_strNfsServer[GUI_NET_SV_STR_LEN + 1];	//网络存储服务器
	int m_iNfsPort;								//网络存储端口号
	char m_strNfsPath[GUI_NET_SV_STR_LEN + 1];		//网络存储路径
	char m_strNfsUser[GUI_NET_SV_STR_LEN + 1];		//网络存储帐号
	char m_strNfsPsw[GUI_NET_SV_STR_LEN + 1];		//网络存储密码
	int m_iIpPopedomType;							//IP权限类型 1:禁止,2:允许，其它保留 
	int m_iIpListSize;								//IP列表长度,0-16(GUI_NET_SV_IP_CNT)
	char m_strIpList[GUI_NET_SV_IP_CNT][GUI_NET_SV_STR_LEN + 1]; //IP列表
} TGuiNetServerCnf;

typedef struct										//获取网络配置回应
{
	TGuiNetServerCnf m_tCnf;
} TGuiGetNetCnfRsp;

typedef struct										//设置网络配置请求
{
	TGuiNetServerCnf m_tCnf;
} TGuiSetNetCnfReq;

#define GUI_SET_NET_CNF_OK		0				//设置网络配置成功
#define GUI_SET_NET_CNF_ERR		1				//设置网络配置失败
typedef struct										//设置网络配置回应
{
	int m_iRetCode;
} TGuiSetNetCnfRsp;

#define GUI_REG_CENTER_SVR_LEN		31
#define GUI_REG_CENTER_USER_LEN		15
#define GUI_REG_CENTER_PSW_LEN		15
#define GUI_DEV_NAME_LEN				31
typedef struct										//注册中心信息
{
	int m_iPort1;									//端口
	char m_strSvr1[GUI_REG_CENTER_SVR_LEN + 1];	//地址
	int m_iPort2;									//端口
	char m_strSvr2[GUI_REG_CENTER_SVR_LEN + 1];	//地址
	char m_strDevName[GUI_DEV_NAME_LEN + 1];		//设备名称
	char m_strName[GUI_REG_CENTER_USER_LEN + 1];	//用户名
	char m_strPsw[GUI_REG_CENTER_PSW_LEN + 1];		//密码
} TGuiRegCenterCnf;

#define GUI_GET_REG_CNF_OK			0			//获取成功
#define GUI_GET_REG_CNF_ERR			1			//获取失败
typedef struct										//获取注册中心信息回应
{
	int m_iRetCode;								//返回码
	TGuiRegCenterCnf m_tCnf;						//注册中心配置信息
} TGuiGetRegCenterCnfRsp;

typedef struct										//设置注册中心信息请求	
{
	TGuiRegCenterCnf m_tCnf;						//注册中心配置信息
} TGuiSetRegCenterCnfReq;

#define GUI_SET_REG_CNF_OK			0
#define GUI_SET_REG_CNF_ERR			1
typedef struct
{
	int m_iRetCode;								//返回码
} TGuiSetRegCenterCnfRsp;

#define GUI_PPPOE_DISCONNECT	0				//PPPoE拨号未连接
#define GUI_PPPOE_CONNECTING 	1				//PPPoE正在拨号
#define GUI_PPPOE_CONNECTED    	2				//PPPoE拨号成功(已连接)

typedef struct									//网络参数
{
	int m_iRetCode;                           				 //返回码 0:成功；1:失败
	int m_iConnectStat;							//PPPoE当前连接状态
	int m_bIsPppoE;                            				//PPPoE自动拨号使能开关
	char m_strPPPoeUsr[GUI_IP_STR_LEN + 1]; 		//PPPoE帐号
	char m_strPPPoePsw[GUI_IP_STR_LEN + 1];		//PPPoE密码
	char m_strPPPoEIp[GUI_IP_STR_LEN + 1];			//PPPoE地址
	char m_strPPPoESubMsk[GUI_IP_STR_LEN + 1];		//PPPoE子网掩码
	char m_strPPPoEGetWay[GUI_IP_STR_LEN + 1];		//PPPoE网关
} TGuiPppoeMsg;

typedef struct
{
	char	m_cNvsName[GUI_IP_STR_LEN + 1];          //设备域名
	char	m_cDomainName[GUI_IP_STR_LEN + 1];       //服务器地址
	char 	m_cUserName[GUI_IP_STR_LEN + 1];         //用户名
	char	m_cPassword[GUI_IP_STR_LEN + 1];			//密码
	int		m_iPort;					//端口号
	int 	m_iDdnsEnable;				//DDNS使能
}TGuiStrDdnsPara;

typedef struct
{
	int m_iRetCode;
}TGuiSetDdnsRsp;

typedef struct
{
	char m_strNtpIp[GUI_IP_STR_LEN + 1];			//网络地址
	int m_iport;                           //端口号
	int m_itime;                           //时间间隔                   
}TGuiSetNtp;

typedef struct
{
	int m_iRetCode;
}TGuiSetNtpRsp;

#define GUI_GET_NTP_REQ_EX			5132      	//扩展的NTP请求 (时间间隔单位为s, 界面显示分钟)
#define GUI_GET_NTP_RSP_EX			-5132      //扩展的NTP请求回应
#define GUI_SET_NTP_REQ_EX			5133   	//扩展的NTP设置请求
#define GUI_SET_NTP_RSP_EX				-5133 	//扩展的NTP设置回应
//---------------协议结构体和TGuiSetNtp一样-------------------//

//---------------------------DHCP服务端-------------------------------
#define GUI_GET_DHCP_SERVER_MSG_REQ		116		//获取DHCP服务信息请求
#define GUI_GET_DHCP_SERVER_MSG_RSP		-116		//获取DHCP服务信息回应

typedef struct						//DHCP网络参数
{
	int m_blEnable;				//是否启用DHCP服务
	int m_iRentTime;				//地址租期（分钟）
	char m_strStartIp[GUI_IP_STR_LEN + 1];		//地址池开始IP地址
	char m_strEndIp[GUI_IP_STR_LEN + 1];		//地址池结束IP地址
	char m_strGetWay[GUI_IP_STR_LEN + 1];		//网关
	char m_strDNS[GUI_IP_STR_LEN + 1];		//首选域名解析
	char m_strSecDNS[GUI_IP_STR_LEN + 1];		//备选域名解析
} TGuiDhcpServerMsg;

typedef struct						//获取DHCP服务参数回应
{
	TGuiDhcpServerMsg m_tDhcpServerMsg;
}TGuiGetDhcpServerMsgRsp;

#define GUI_SET_DHCP_SERVER_MSG_REQ		117		//设置DHCP服务信息请求
#define GUI_SET_DHCP_SERVER_MSG_RSP		-117	//设置DHCP服务信息回应

#define GUI_SET_DHCP_SERVER_MSG_OK			0
#define GUI_SET_DHCP_SERVER_MSG_ERR			1

typedef struct						//设置DHCP服务参数请求
{
	TGuiDhcpServerMsg m_tDhcpServerMsg;
}TGuiSetDhcpServerMsgReq;

typedef struct					//设置DHCP服务参数回应
{
	int m_iRetCode;				//设置DHCP服务参数回应，0表示成功
}TGuiSetDhcpServerMsgRsp;

#define GUI_GET_DHCP_CLIENT_MSG_REQ 		118		//获取DHCP客户端连接列表请求
#define GUI_GET_DHCP_CLIENT_MSG_RSP 		-118	//获取DHCP客户端连接列表回应

#define GUI_CLIENT_NAME_LEN		31

typedef struct						//客户端信息
{
	int m_iClientID;				//客户端ID
	int m_iRentTime;				//有效时间
	char m_strClientName[GUI_CLIENT_NAME_LEN + 1];	//客户端名
	char m_strMac[GUI_IP_STR_LEN + 1];		//mac地址
	char m_strIp[GUI_IP_STR_LEN + 1];		//IP地址
} TGuiDhcpClientMsg;

typedef struct						//客户端连接列表请求回应
{
	int m_iDhcpClientCnt;			//客户端连接总条数
}TGuiGetDhcpClientMsgRsp;


// ---------------------8.异常处理------------------------------------
#define GUI_MAX_EXCEPTION_CNT		12
#define GUI_SUGGEST_CNT       3
#define GUI_SCREEN_DSP        0
#define GUI_SOUND_ENABEL      1
#define GUI_TRAN_TO_SERVER    2

typedef enum
{
	DISK_FULL_EXP = 0,			//磁盘满
	DISK_EMPTY_EXP,				//无磁盘
	DISK_ERR_EXP,					//磁盘读写错误
	ACCESS_ERR_EXP,				//非法访问
	IP_ERR_EXP,					//IP冲突	
	NET_ERR_EXP,					//网络中断
	SYSTEM_BUSY_EXP,				//系统忙
	REDUANCYDISK_EMPTY_EXP,		//无冗余盘	
	POWER_EXP,					//电压异常
	MAC_ERR_EXP,					//MAC地址冲突异常
	RAID_ERR_EXP,					//阵列异常
	HOTSPARE_ERR_EXP,				//热备异常
	POWERPSE_EXP = 12,					//PSE工号超负荷异常
	DISK_UPLOAD_EXP,                  //磁盘超负荷
	RECORD_ERR_EXP = 14,			 //录像异常
}EExceptionType;

typedef enum
{    
	GUI_SHOW_ONSCREEN = 0,	//屏幕显示	
	GUI_SOUND_NOTICE,	//声音提示
	GUI_UPDATE_TOSERVER,	//上传中心
	GUI_EMAIL_ALARM,		//邮件报警
	GUI_LINK_ALMOUT,		//联动端口报警输出
}EGuiSuggestType;


typedef struct										//异常处理配置信息
{
	int m_iExHanldFlg[GUI_MAX_EXCEPTION_CNT]; 		// 1: 处理, 0 : 不处理
	int m_iSuggestFlg[GUI_MAX_EXCEPTION_CNT][GUI_SUGGEST_CNT];   //1:处理 0:不处理
} TGuiExceptionCnf;

#define GUI_GET_EXCEP_CNF_OK		1
#define GUI_GET_EXCEP_CNF_ERR		0

typedef struct										//获取配置信息回应
{
	int m_iRetCode;								//回应码
	TGuiExceptionCnf m_tCnf;						//配置信息
} TGuiGetExceptionCnfRsp;

typedef struct										//设置配置信息请求	
{
	TGuiExceptionCnf m_tCnf;
} TGuiSetExceptionCnfReq;

#define GUI_SET_EXCEP_CNF_OK		1
#define GUI_SET_EXCEP_CNF_ERR		0

typedef struct										//设置配置信息回应
{
	int m_iRetCode;								//回应码
} TGuiSetExceptionCnfRsp;

#define GUI_GET_EXCEP_CNF_REQ_EX		122		//扩展的获取异常处理参数请求
#define GUI_GET_EXCEP_CNF_RSP_EX		-122	//扩展的获取异常处理参数回应
typedef struct
{
	int m_iOffset;               //起始编号
}TGuiGetExceptionCnfExReq;

typedef struct										//获取配置信息回应
{
	int m_iRetCode;								//回应码
	int m_iOffset;
	TGuiExceptionCnf m_tCnf;						//配置信息
} TGuiGetExceptionCnfExRsp;

#define GUI_SET_EXCEP_CNF_REQ_EX		123		//扩展的设置异常处理参数请求
#define GUI_SET_EXCEP_CNF_RSP_EX		-123	//扩展的设置异常处理参数回应

typedef struct										//获取配置信息回应
{
	int m_iOffset;
	TGuiExceptionCnf m_tCnf;						//配置信息
} TGuiSetExceptionCnfExReq;

typedef struct										//设置配置信息回应
{
	int m_iRetCode;								//回应码
} TGuiSetExceptionCnfExRsp;

#define GUI_GET_EXCEPTION_LINK_ACT_REQ     2520
#define GUI_GET_EXCEPTION_LINK_ACT_RSP     -2520
typedef struct
{
	int m_iExpType;       	//异常类型
	int m_iSuggestType;	//提示类型 ，见枚举EGuiSuggestType
}TGuiGetExpSuggestReq;

typedef struct
{
	int m_iExpType;
	int m_iSuggestType;
	int m_iSuggestEnable;  //提示类型使能，如果是联动端口输出，使用下面的字段
	int m_iParamEx[32];   //如果是端口报警，按位表示端口使能(数字通道+ 本地+报警主机)
}TGuiGetExpSuggestRsp;

#define GUI_SET_EXCEPTION_LINK_ACT_REQ     2521
#define GUI_SET_EXCEPTION_LINK_ACT_RSP     -2521
typedef struct
{
	int m_iExpType;
	int m_iSuggestType;
	int m_iSuggestEnable;  //提示类型使能，如果是联动端口输出，使用下面的字段
	int m_iParamEx[32];   //如果是端口报警，按位表示端口使能(数字通道+ 本地+报警主机)
}TGuiSetExpSuggestReq;

typedef struct
{
	int m_iRetCode;
}TGuiSetExpSuggestRsp;


//-------------------------------9.别名管理-------------------------------
#define GUI_ALIAS_NAME_STR_LEN 	16
#define GUI_VIDEO_FLAG 1
#define GUI_ALARM_IN_FLAG 2
#define GUI_ALARM_OUT_FLAG 3

typedef struct
{
	int m_iNameFlag;//别名标志，1为视频、2为报警输入、3为报警输出
	int m_iChnNum;//通道号
	char m_cChnName[GUI_ALIAS_NAME_STR_LEN + 1];//通道名	
} TGuiChnAlais;	//通道别名

typedef struct
{
	int m_iNameFlag;
	int m_iChnNum;
} TGuiGetChnAliasReq;

typedef struct
{
	int m_iRetCode;		// 0表示成功
	TGuiChnAlais m_tAlias;
} TGuiGetChnAliasRsp;

typedef struct
{
	TGuiChnAlais m_tAlias;
}TGuiSetChnAliasReq;

typedef struct
{
	int m_iRetCode;	// 0表示成功
}TGuiSetChnAliasRsp;

typedef struct		//获取通道别名版本请求
{
	int m_iType;		//类型号
} TGuiGetChnAliasVerReq;

typedef struct
{
	int m_iType;		//类型号
	int m_iCurVer;	//版本号
} TGuiGetChnAliasVerRsp;

//扩展的通道别名(支持64个字节)
#define GUI_GET_CHN_ALIAS_REQ_EX		143
#define GUI_GET_CHN_ALIAS_RSP_EX		-143
#define GUI_SET_CHN_ALIAS_REQ_EX		144
#define GUI_SET_CHN_ALIAS_RSP_EX		-144

typedef struct
{
	int m_iNameFlag;//别名标志，1为视频、2为报警输入、3为报警输出
	int m_iChnNum;//通道号
	char m_cChnName[GUI_PUBLIC_LEN_63 + 2];//通道名	
} TGuiChnAlaisEx;	//通道别名

typedef struct
{
	int m_iNameFlag;
	int m_iChnNum;
} TGuiGetChnAliasReqEx;

typedef struct
{
	int m_iRetCode;		// 0表示成功
	TGuiChnAlaisEx m_tAliasEx;
} TGuiGetChnAliasRspEx;

typedef struct
{
	TGuiChnAlaisEx m_tAliasEx;
}TGuiSetChnAliasReqEx;

typedef struct
{
	int	m_iRetCode;
}TGuiSetChnAliasRspEx;
//==============end=========


//自动维护
#define GUI_GET_AUTOMAINTAIN_REQ	880
#define GUI_GET_AUTOMAINTAIN_RSP	-880
#define GUI_SET_AUTOMAINTAIN_REQ	881
#define GUI_SET_AUTOMAINTAIN_RSP	-881

//星期日到星期六为0～6，7-每天，8-从不
typedef struct
{
	int m_iType;      //操作类型，按位表示，最低一位为1，表示定时重启
	int m_iWeekDay;   //星期几
	int m_iTm;        //每天从0点开始流逝的秒数
}TGuiClockMsg;

typedef struct
{
	TGuiClockMsg m_tClockMsg;
}TGuiGetAutoMaintainRsp;

typedef struct
{
	TGuiClockMsg m_tClockMsg;
}TGuiSetAutoMaintainReq;

//使用公用的返回码，GUI_SET_OK表成功，GUI_SET_ERR表失败
typedef struct
{
	int m_iRetCode;
} TGuiSetAutoMaintainRsp;


//==============================图像设置 ===============================
//============================预览效果===========================
#define GUI_GET_PREVIEW_PIC_QUALITY_REQ		198			//获取预览效果参数请求
#define GUI_GET_PREVIEW_PIC_QUALITY_RSP		-198		//获取预览效果参数回应
#define GUI_SET_PREVIEW_PIC_QUALITY_REQ		199			//设置预览效果参数请求
#define GUI_SET_PREVIEW_PIC_QUALITY_RSP		-199		//设置预览效果参数回应

enum
{
	PIC_HIGH_QUALITY = 0,			//最佳图像质量
	PIC_HIGH_PERFORMANCE = 1,		//最大预览性能
}; //预览图像效果参数

enum
{
	PIC_LOW_DELAY = 0,                   //低延时
	PIC_HIGH_FLUENT = 1,               //高流畅
};

typedef struct
{
	int m_iPicQP;		//预览图像效果参数, 低16位(0表示最佳图像质量，1表示最大预览性能, 2表示自动选择)，
	//扩展高16位(0表示低延时，1表示高流畅)
}TGuiPreviewPicQualityCnf;			

typedef struct									//获取预览效果参数回应
{
	TGuiPreviewPicQualityCnf m_tCnf;			//预览效果
} TGuiGetPreviewPicQualityRsp;					//获取预览效果参数回应			

typedef struct											
{
	TGuiPreviewPicQualityCnf m_tCnf;			//预览效果
} TGuiSetPreviewPicQualityReq;				//设置预览效果参数请求

#define GUI_SET_PREVIEW_PIC_QUALITY_OK		0	//设置成功
#define GUI_SET_PREVIEW_PIC_QUALITY_ERR		1	//设置失败
typedef struct											
{
	int m_iRetCode;			//返回码
} TGuiSetPreviewPicQualityRsp;				//设置预览效果参数回应

//保留最后一帧或者novideo
#define GUI_GET_PREVIEW_IPC_OFFLINE_STATUS_REQ		196
#define GUI_GET_PREVIEW_IPC_OFFLINE_STATUS_RSP		-196
#define GUI_SET_PREVIEW_IPC_OFFLINE_STATUS_REQ		197
#define GUI_SET_PREVIEW_IPC_OFFLINE_STATUS_RSP		-197
	
enum
{
	PIC_NOVIDEO = 0,	//novideo
	PIC_LASTFRAME,	   //最后一帧
};

typedef struct
{
	int m_iPicStatus;  // 最后一帧或者是novideo 
}TGuiIpcOfflinePicCnf;


typedef struct
{
	TGuiIpcOfflinePicCnf m_tCnf;
}TGuiGetIpcOfflinePicStatusRsp;

typedef struct
{
	TGuiIpcOfflinePicCnf m_tCnf;
} TGuiSetIpcOfflinePicStatusReq;


typedef struct
{
	int m_iRetCode;                   //0成功，1失败
} TGuiSetIpcOfflinePicStatusRsp;


//------------------------------ 1.预览设置 ----------------------------
//固定画面模式下返回序列
typedef struct									//预览切换序列
{
	int m_iCnt;									//序列长度
	int m_iChnList[GUI_DVR_CHN_CNT];		//存放通道号
} TGuiPreviewSeq;
//======================================================

#define GUI_PIC_USER_DEF   0xFFFF
//自定义画面模式下返回坐标点
typedef struct	//区域定义
{
	int m_iLeft;
	int m_iTop;
	int m_iWidth;
	int m_iHeight;	
} TGuiRect;

typedef struct
{
    TGuiRect m_tVoAreas;
}TGuiUserPreviewParamSeq;
//==============================


// 在获取预览参数回应与设置预览参数请求时，除发送相应的rsp与req外，随即发
// 送相应个数的TGuiPreviewSeq

typedef struct
{
	int m_iDevId;								//设备编号
} TGuiGetPreviewReq;						//获取预览参数请求 dxl 2009/11/30

typedef struct									//获取预览参数回应
{
	int m_iPicCnt;								//画面数，0表示获取失败
	int m_iInterval;							//切换时间间隔,0表不切抱换
} TGuiGetPreviewRsp;

typedef struct									//设置预览参数请求
{
	int m_iDevId;								//设备编号 2009/11/30
	int m_iPicCnt;
	int m_iInterval;
} TGuiSetPreviewReq;

#define GUI_SET_PREVIEW_OK		0			//设置成功
#define GUI_SET_PREVIEW_ERR		1			//设置失败
typedef struct									//设置预览参数回应
{
	int m_iRetCode;							//返回码
} TGuiSetPreviewRsp;

//============扩展预览设置(大路数)===================
#define GUI_GET_PREVIEW_REQ_EX          2080 //获取预览配置请求
#define GUI_GET_PREVIEW_RSP_EX	    -2080	//获取预览配置回应

#define GUI_SET_PREVIEW_REQ_EX          2081  //设置预览配置请求
#define GUI_SET_PREVIEW_RSP_EX         -2081  //设置预览配置回应

// 在获取预览参数回应与设置预览参数请求时，除发送相应的rsp与req外，随即发
// 送相应个数的TGuiPreviewSeqEx

typedef struct									//预览切换序列
{
	int m_iCnt;									//序列长度
	int m_iChnList[0];			//存放通道号
} TGuiPreviewSeqEx;

typedef TGuiGetPreviewReq TGuiGetPreviewReqEx;
typedef TGuiGetPreviewRsp	TGuiGetPreviewRspEx;

typedef TGuiSetPreviewReq TGuiSetPreviewReqEx;
typedef TGuiSetPreviewRsp TGuiSetPreviewRspEx;

typedef struct
{
	int m_iDevId;									//设备编号
} TGuiPreviewSwStartReq;					//开始视频预览切换请求dxl 2009/11/30

#define GUI_PREVIEW_SW_START_DONE	0		//成功开启视频切换
#define GUI_PREVIEW_SW_START_ERR		1		//开启视频切换失败
typedef struct									//开启视频切换回应
{
	int m_iRetCode;
} TGuiPreviewSwStartRsp;

typedef struct
{
	int m_iDevId;									//设备编号
} TGuiPreviewSwStopReq;						//停止预览切换请求 2009/11/30

#define GUI_PREVIEW_SW_STOP_DONE		0		//成功停止视频切换
#define GUI_PREVIEW_SW_STOP_ERR		1		//停止视频切换失败
typedef struct									//停止视频切换回应
{
	int m_iRetCode;
} TGuiPreviewSwStopRsp;

typedef struct									//单画面播放命令
{
	int m_iDevId;									//设备编号 dxl 2009/11/30
	int m_iChn;									//单画面通道号
} TGuiPreviewSingPicReq;

#define GUI_PREVIEW_SPIC_OK		0			//设置单画面成功
#define GUI_PREVIEW_SPIC_ERR		1			//设置单画面失败
typedef struct									//单画面播放命令回应
{
	int m_iDevId;
	int m_iChn;									//单画面通道号
	int m_iRetCode;							//返回码
} TGuiPreviewSingPicRsp;

typedef struct
{
	int m_iDevId;									//设备编号
} TGuiPreviewSingPicExitMsg;					//单画面退出通知dxl 2009/11/30

#define GUI_PREVIEW_MAP_REQ		207			//视频映射请求
#define GUI_PREVIEW_MAP_RSP		-207		//视频映射回应
typedef struct									//视频映射请求
{
	int m_iDevId;									//设备号
	int m_iChn;									//通道号
	int m_iLeft;									//左上角x坐标
	int m_iTop;									//左上角y坐标
	unsigned int m_uiWidth;						//宽
	unsigned int m_uiHeight;						//高
} TGuiPreviewMapReq;

#define GUI_PREVIEW_MAP_OK			0		//视频映射成功
#define GUI_PREVIEW_MAP_ERROR		1		//视频映射失败
typedef struct								//视频映射回应
{
	int m_iRetCode;
} TGuiPreviewMapRsp;

#define GUI_PREVIEW_MAP_EXIT_MSG	260		//结束视频映射(无回应)
typedef struct								//结束视频映射
{
	int m_iDevId;	 //设备号
} TGuiPreviewMapExitMsg;

#define GUI_PREVIEW_SW_STA_REQ	208			//获取视频切换状态请求
#define GUI_PREVIEW_SW_STA_RSP	-208		//获取视频切换状态回应
typedef struct
{
	int m_iDevId;								//设备号
} TGuiPreviewSwStaReq;					//获取预览状态请求 2009/11/30

#define GUI_PREVIEW_SW_STA_STARTED	0		//视频预览切换已开启
#define GUI_PREVIEW_SW_STA_STOPED	1		//视频预览切换已停止	
typedef struct									//获取视频预览切换状态回应
{
	int m_iRetStat;							//返回状态
} TGuiPreviewSwStaRsp;

#define GUI_PREVIEW_MULTI_PIC_REQ	209		//设置预览画面数请求
#define GUI_PREVIEW_MULTI_PIC_RSP	-209	//设置预览画面数回应
typedef struct									//更改当前预览画面数请求
{
	int m_iDevId;								//设备号	2009/11/30 dxl
	int m_iPicCnt;								//欲设置的预览画面数(0表示查询)
} TGuiPreviewMultiPicReq;

typedef struct
{
	int m_iPicCnt;								//当前的预览画面数
} TGuiPreviewMultiPicRsp;

#define GUI_PREVIEW_PAGE_UP		202			//预览上一页
typedef struct
{
	int m_iDevId;
} TGuiPreviewPageUpReq;					//预览上翻页

#define GUI_PREVIEW_PAGE_DOWN	203			//预览下一页
typedef struct
{
	int m_iDevId;
} TGuiPreviewPageDownReq;					//预览下翻页

#define GUI_PREVIEW_CHG_CHN_REQ	250			//临时更改画面显示的视频输入通道请求
#define GUI_PREVIEW_CHG_CHN_RSP	-250		//临时更改画面显示的视频输入通道回应
typedef struct									//临时更改预览画面的视频输入通道请求
{
	int m_iDevId;								//设备编号dxl 2009/11/30
	int m_iVoChn;								//vo画面编号(1-16)
	int m_iViChn;								//vi编号(0-16, 0表示查询当前画面的当前视频输入通道)
} TGuiPreviewChgChnReq;

typedef struct									//临时更改预览画面的视频输入通道回应
{
	int m_iDevId;									//设备编号
	int m_iVoChn;								//vo画面编号(1-16)
	int m_iViChn;								//当前vo对应的vi编号
} TGuiPreviewChgChnRsp;

#define GUI_PREVIEW_SND_BIND_MSG	252			//预览声音绑定到VO通知
typedef struct									//预览声音绑定
{
	int m_iDevId;									//显示设备编号
	int m_iVoChn;								//vo通道号(1-16, 0表全部???)
} TGuiPreviewSoundBindMsg;

#define GUI_SET_PREVIEW_EXCHANGE_REQ  249     //设置交换两视频画面请求
#define GUI_SET_PREVIEW_EXCHANGE_RSP  -249     //设置交换两视频画面回应

typedef struct
{
	int m_iDevId;             //高16位为FF时，表示特殊模式(交换两个数字通道的配置)
	int m_iSrcVoChn;       //vo交换源通道号 从1开始
	int m_iDstVoChn;       //vo交换目的通道号 从1开始
}TGuiSetPreviewExchangeReq;   //设置交换两视频画面请求

#define GUI_SET_PREVIEW_EXCHANGE_OK  0
#define GUI_SET_PREVIEW_EXCHANGE_ERR 1
typedef struct
{
	int m_iRetCode;
}TGuiSetPreviewExchangeRsp;  //设置交换两视频画面回应

//------------------------------ 2.本地显示 ----------------------------

#define GUI_LAN_DEFAULT		0				//默认语言
#define GUI_LAN_CHI			1				//简体中文
#define GUI_LAN_ENG			2				//英文
#define GUI_LAN_TCHI		3				//繁体中文
#define GUI_LAN_KOR			4				//韩文
#define GUI_LAN_SPAN		5				//西班牙文
#define GUI_LAN_ITA			6				//意大利文
typedef struct
{
	int m_iDspMode;							//制式,PAL/NTSC
	int m_iTimeDspMode;						//时间显示格式，0表示不显示
	int m_iGuiTrnsRate;						//菜单透明度0-100，值大越透明
	int m_iVgaMode;							//低16位vga显示模式  高16位 HDMI模式
	int m_iSoundFlg;							//声音使能开关
	int m_iAlarmStaDspFlg;					//报警状态显示开关
	int m_iVedioRcdStaDspFlg;					//录像状态显示开关
	int m_iGuiTimeOut;							//图形界面超时设置,0表不超时
	int m_iGuiLang;							//语言
	int m_iStyle;								//样式
	int m_iDeflicker;							//抗闪砾
	int m_iMainportDetect;					//主口检测
	int m_iLinkSPicInterval;          		//报警联动单画面切换时间间隔
	int m_iAutoHidenRecLed;						//低16位，是否隐藏录像状态灯,1隐藏,0不隐藏
										//高16位, 是否显示解码达到上限提示1显示,0不显示
	int m_iHdmiDspMode;                     //HDMI 显示模式0-Auto , 1-YCBCR, 2-sRGB
	int m_iLcdScreenTmOut;					//ATM液晶关屏时间
	int m_iBrightness;						//液晶亮度调节
	int m_iAudioWaveState;					//是否显示音频波形,1显示,0不显示
	int m_iShowCblMsg;					//是否显示刻录信息，1显示，0不显示
	int m_iSizeAuto;						//分辨率自适应，1选中，0不选(低16位表示vga1，高16位表示vga2)
	int m_iMouseSpeed;					//鼠标移动速度0-100
	int m_iBatteryLowBeep;				//电池电量低时是否响蜂鸣器
	int m_iPowerBeep;					//插拔电源时是否响蜂鸣器
	int m_iMouseSoundCtr;				//鼠标滚轮对音量的调节
	int m_iShowH323Stat;				//是否显示H323会议呼叫状态，1显示，0不显示
} TGuiDisplayCnf;

typedef struct									//获取本地显示参数回应
{
	int m_iVideoMode;							//视频输出制式, PAL/NTSC
} TGuiGetDisplayCnfRsp;

typedef struct									//设置本地显示参数请求
{
	int m_iVideoMode;							//视频输出制式, PAL/NTSC
} TGuiSetDisplayCnfReq;

#define GUI_SET_DSP_CNF_OK	0				//设置本地显示参数成功
#define GUI_SET_DSP_CNF_ERR	1				//失败
typedef struct									//设置本地显示参数回应
{
	int m_iRetCode;
} TGuiSetDisplayCnfRsp;

#define GUI_GET_MASTER_DEV_REQ  212         //获取主设备号请求
#define GUI_GET_MASTER_DEV_RSP  -212        //获取主设备号回应
typedef struct
{
	int m_iMasterDev;
}TGuiGetMasterDevRsp; 

//=========TV调节======
#define GUI_GET_DEV_TV_MSG_REQ   256      	//获取设备TV调节参数信息请求
#define GUI_GET_DEV_TV_MSG_RSP   -256		//获取设备TV调节参数信息回应

typedef struct
{
	int m_iVoDevNo;		//输出设备号
	int m_iTop;			//上边距(范围:0~100)
	int m_iBottom;		//下边距(范围:0~100)
	int m_iLeft;		//左边距(范围:0~100)
	int m_iRight;		//右边距(范围:0~100)
}TGuiDevTVMsg;

typedef struct
{
	int m_iVoDevNo;		//输出设备号 默认是BNC设备 GUI_BNC_DEV
}TGuiGetDevTvMsgReq;						//获取设备TV调节参数信息请求

typedef struct
{
	TGuiDevTVMsg m_tTvMsg;
}TGuiGetDevTvMsgRsp;						//获取设备TV调节参数信息回应


#define GUI_SET_DEV_TV_MSG_REQ   257      	//设置设备TV调节参数信息请求
#define GUI_SET_DEV_TV_MSG_RSP   -257		//设置设备TV调节参数信息回应
typedef struct
{
	TGuiDevTVMsg m_tTvMsg;
}TGuiSetDevTvMsgReq;         		//设置设备TV调节参数信息请求

typedef struct
{
	int m_iVoDevNo;
	int m_iRetCode;
}TGuiSetDevTvMsgRsp;   			//设置设备TV调节参数信息回应


//-------------------------预览裁边-----------------------------------------------
#define GUI_GET_PREVIEW_CUT_MSG_REQ		213		//获取视频预览裁边信息请求
#define GUI_GET_PREVIEW_CUT_MSG_RSP		-213		//获取视频预览裁边信息回应

typedef struct					//视频裁边
{
	int m_iEnable;					//是否启用预览裁边
	int m_iTop;					//千分比上部
	int m_iBottom;				//千分比底部
	int m_iLeft;					//千分比左边
	int m_iRight;					//千分比右边
} TGuiPreviewCutMsg;

typedef struct						//获取视频预览裁边信息请求
{
	int m_iChn;					//通道号
}TGuiGetPreviewCutMsgReq;

typedef struct						//获取视频预览裁边信息回应
{
	int m_iChn;
	TGuiPreviewCutMsg m_tPreviewCutMsg;	//裁剪千分比信息	
}TGuiGetPreviewCutMsgRsp;

#define GUI_SET_PREVIEW_CUT_MSG_REQ		214		//设置视频预览裁边信息请求
#define GUI_SET_PREVIEW_CUT_MSG_RSP		-214		//设置视频预览裁边信息回应

typedef struct						//设置视频预览裁边请求
{
	int m_iChn;							//通道号	
	TGuiPreviewCutMsg m_tPreviewCutMsg;	//裁剪千分比信息		
}TGuiSetPreviewCutMsgReq;

#define IP_CHN_NOT_ENABLE  			2				//数字通道未配置或禁用
#define IP_CHN_NOT_VIDEO_CONTECT  	3				//数字通道没有视频连接
#define IP_CHN_NOT_INIT    			4				//数字通道初始化错误

typedef struct						//设置视频预览裁边回应
{
	int m_iRetCode;					//返回码,0:成功2/3/4失败，失败说明如上
}TGuiSetPreviewCutMsgRsp;

//------------------------------ 3.视频输入参数 --------------------------

typedef struct			//获取视频输入通道制式请求
{
	int m_iChn;			//通道号,1 - 16
} TGuiGetVideoInNorReq;

typedef struct			//获取视频输入通道制式回应
{
	int m_iNor;			//制式, GUI_PAL_MODE / GUI_NTSC_MODE
 } TGuiGetVideoInNorRsp;

typedef struct
{
	int m_iChn;
	int m_iNor;
} TGuiSetVideoInNorReq;

typedef struct
{
	int m_iRetCode;
} TGuiSetVideoInNorRsp;

#define GUI_VIVEOIN_TM_CNF_CNT		5		//同一通道支持五个时段	

enum //彩转黑类型
{
	VIDEOIN_COLOR = 0,   		//彩色
	VIDEOIN_GRAY,				//黑白	
	VIDEOIN_INNER,      		//内同步
	VIDEOIN_OUTSIDE,	 		//外同步
	VIDEOIN_ALARM,       		//报警同步
	VIDEOIN_NONE = 0xFF,                   //不启用
};								

typedef struct									//视频输入参数时段信息
{
	int m_iBenginTm;							//开始时间，从当天00:00流过的秒
	int m_iEndTm;								//结束时间
	int m_iHue;									//色　调 0-100
	int m_iLum;									//亮　度 0-100
	int m_iContrast;							//对比度 0-100
	int m_iSaturation;							//饱和度 0-100
	// 2008-12-26 添加彩转黑
	int m_iGrayFlg;								//彩转黑类型(0:彩色, 1:灰色)
} TGuiVideoInCnf;

typedef struct									//获取视频输入参数请求
{
	int m_iChn;									//通道号
} TGuiGetVideoInCnfReq;

typedef struct									//获取视频输入参数回应
{
	int m_iChn;									//通道号, 1-16，0表示错误
	int m_iVideMode;							//视频输入制式,PAL/NTSC/自动检测
	TGuiVideoInCnf m_tCnf[GUI_VIVEOIN_TM_CNF_CNT]; //配置参数
} TGuiGetVideoInCnfRsp;

typedef struct									//设置视频输入参数请求
{
	int m_iChn;									//通道号
	int m_iVideMode;							//视频输入制式,PAL/NTSC
	TGuiVideoInCnf m_tCnf[GUI_VIVEOIN_TM_CNF_CNT]; //配置参数
} TGuiSetVideoInCnfReq;

//设置自动检测视频输入制式
typedef struct
{
	int m_iChn;						//0表示所有通道,其余通道号对应
} TGuiGetVideoInModeAutoCheckReq;

typedef struct
{
	int m_iChn;				
	int m_iViModeAutoCheck;
} TGuiGetVideoInModeAutoCheckRsp;	//获取视频输入自动检测回应


typedef struct
{
	int m_iChn;	
	int m_iViModeAutoCheck;			//视频输入制式自动检测,1表示开启,0表示关闭
} TGuiSetVideoInModeAutoCheckReq;

typedef struct
{
	int m_iRetCode;					//回应码,0:成功，1:失败
} TGuiSetVideoInModeAutoCheckRsp;


#define GUI_SET_VIDEOIN_CNF_OK	0			//设置视频输入参数成功
#define GUI_SET_VIDEOIN_CNF_ERR	1			//设置视频输入参数失败
typedef struct									//设置视频输入参数回应
{
	int m_iRetCode;
} TGuiSetVideoInCnfRsp;

typedef struct									//修改视频输入参数请求
{
	int m_iChn;									//通道编号
	int m_iVideoMod;							//视频输入制式
	int m_iHue;									//色　调 0-100
	int m_iLum;									//亮　度 0-100
	int m_iContrast;							//对比度 0-100
	int m_iSaturation;							//饱和度 0-100
	// 2008-12-26 添加彩转黑
	int m_iGrayFlg;
} TGuiChgVideoInCnfReq;

#define GUI_CHG_VIDEOIN_CNF_OK	0			//修改视频输入参数成功
#define GUI_CHG_VIDEOIN_CNF_ERR	1			//修改视频输入参数失败
typedef struct
{
	int m_iRetCode;
} TGuiChgVideoInCnfRsp;

//=============================编码参数=========================
// 4.视频编码参数
#define GUI_GET_VENCODE_CNF_REQ	230			//获取视频编码参数请求
#define GUI_GET_VENCODE_CNF_RSP	-230		//获取视频编码参数回应
#define GUI_SET_VENCODE_CNF_REQ	231			//设置视频编码参数请求
#define GUI_SET_VENCODE_CNF_RSP	-231		//设置视频编码参数回应


#define GUI_VIDEO_FULL_FRAME_RATE			2009	//视频全帧率

// 因GUI_VIDEO_BIT_STREAM_STATIC定义有差别，
// 判断定码率以!= GUI_VIDEO_BIT_STREAM_DYNAMIC为准
#define GUI_VIDEO_BIT_STREAM_DYNAMIC		1	//变码率
#define GUI_VIDEO_BIT_STREAM_STATIC			2	//定码率老定义,注意兼容
#define GUI_VIDEO_BIT_STREAM_STATIC_NEW		0	//定码率 dxl 2011/02/23

// 因GUI_VIDEO_AUDIO定义有差别，判断复合码流以
// != GUI_VIDEO_ONLY为准
#define GUI_VIDEO_ONLY						1	//纯视频
#define GUI_VIDEO_AUDIO						2	//复合音频	老定义,注意兼容			
#define GUI_VIDEO_AUDIO_NEW					3	//复合音频	
#define GUI_AUDIO_ONLY						20

//编码模式
enum
{
	GUI_ENCODE_MODE_BASELINE_PROFILE = 0,
	GUI_ENCODE_MODE_MAIN_PROFILE,
	GUI_ENCODE_MODE_HIGH_PROFILE,
};

enum//参数类型
{
	GUI_MAIN_CHANNEL_INSTANT = 0, //即时值
	GUI_MAIN_CHANNEL_TIME,    	//主码流（定时）
	GUI_MAIN_CHANNEL_INCIDENT,      //主码流（事件）	
	GUI_SUB_CHANNEL,          		//副码流
	GUI_MAIN_CHANNEL_TIME_TEMP1,    //主码流（定时模板1）
	GUI_MAIN_CHANNEL_TIME_TEMP2,     //主码流（定时模板2）
	GUI_MAX_STREAM_TYPE,               //最大模板数
}TGuiVTempType; 

typedef struct									//视频编码参数
{
	int m_iEncodeType;							//低16位视频编码方式H264, 高16位为编码模式 
	int m_iPicSize;							//分辨率编号
	int m_iFrameRate;							//视频帧率
	int m_iBitStreamType;						//码流类型
	int m_iBitRate;							//码流大小(上限):kbps
	int m_iPicQualityLv;						//画质级别
	int m_iIFrameRate;							//I帧帧率
	int m_iComplexType;						//音视频复合类型
	int m_iAudioEncodeType;					//音频编码方式
} TGuiVideoEncodeCnf;

typedef struct					//获取视频编码参数请求
{
	int m_iChn;					//通道号(扩展参数类型，高16位说明见参数类型TGuiVTempType)
} TGuiGetVideoEncodeCnfReq;

typedef struct					//获取视频编码参数回应
{
	int m_iChn;					//通道号(扩展参数类型，高16位说明见参数类型TGuiVTempType)
	TGuiVideoEncodeCnf m_tCnf;	//配置信息
} TGuiGetVideoEncodeCnfRsp;

// 设置视频编码参数请求
typedef TGuiGetVideoEncodeCnfRsp TGuiSetVideoEncodeCnfReq;

#define GUI_SET_VENCODE_CNF_OK		0		//设置成功
#define GUI_SET_VENCODE_CNF_ERR		1		//设置失败
typedef struct									//设置视频编码参数回应
{
	int m_iRetCode;
} TGuiSetVideoEncodeCnfRsp;


#define GUI_GET_INCIDENT_TEMPLATE_ENABLE_REQ	2300   	//获取事件模板是否启用请求
#define GUI_GET_INCIDENT_TEMPLATE_ENABLE_RSP	-2300 	//获取事件模板是否启用回应

typedef struct
{
	int 	m_iChn;
}TGuiGetIncidentTmpEnableReq;

typedef struct
{
	int 	m_iChn;
	int 	m_iEnable;
}TGuiGetIncidentTmpEnableRsp;

#define GUI_SET_INCIDENT_TEMPLATE_ENABLE_REQ	2301	//设置事件模板是否启用请求
#define GUI_SET_INCIDENT_TEMPLATE_ENABLE_RSP	-2301	//设置事件模板是否启用回应

typedef struct
{
	int 	m_iChn;
	int 	m_iEnable;
}TGuiSetIncidentTmpEnableReq;

typedef struct
{
	int	m_iRetCode;
}TGuiSetIncidentTmpEnableRsp;


//------------------------------音频采样率--------------------------------
#define GUI_GET_AUDIO_RATE_CNF_REQ    236    //获取音频采样率请求
#define GUI_GET_AUDIO_RATE_CNF_RSP 	 -236    //获取音频采样率回应

typedef struct			//获取音频采样率参数请求
{
	int m_iChn;			//通道号
} TGuiGetAudioRateCnfReq;

typedef struct			//获取音频采样率参数回应
{
	int m_iAudioRate;	//音频采样率8000:8K;32000:32K;48000:48K
} TGuiGetAudioRateCnfRsp;

#define GUI_SET_AUDIO_RATE_CNF_REQ   237     //设置音频采样率请求
#define GUI_SET_AUDIO_RATE_CNF_RSP  -237     //设置音频采样率回应

#define GUI_SET_AUDIO_RATE_CNF_OK   0		 //设置成功
#define GUI_SET_AUDIO_RATE_CNF_ERR  1		 //设置失败

typedef struct			//设置音频采样率参数请求
{
	int m_iChn;	//通道号
	int m_iAudioRate;	//音频采样率8:8K;32:32K;48:48K
} TGuiSetAudioRateCnfReq;

typedef struct			//设置音频采样率参数回应
{
	int m_iRetCode;		//返回码，0表示成功
} TGuiSetAudioRateCnfRsp;

#define GUI_GET_CHN_AUDIO_RATE_LST_REQ    238  //获取每个通道的音频采样率列表请求
#define GUI_GET_CHN_AUDIO_RATE_LST_RSP    -238  //获取每个通道的音频采样率列表回应

typedef struct
{
	int m_iChn;   //请求通道
}TGuiGetChnAudioRateLstReq;

typedef struct
{
	int m_iChn;
	int m_iCnt;   //返回请求通道支持几种音频采样率
}TGuiGetChnAudioRateLstRsp;

typedef struct
{
	int m_iAudioRate;  //音频采样率
}TGuiAudioRateItem;

//音视频拼接
#define GUI_SET_AV_MEDLEY_REQ			20007	//设置音视频拼接请求
#define GUI_SET_AV_MEDLEY_RSP			-20007 	//设置音视频拼接回应


typedef struct
{
	int m_iChn;
	int m_iEnable; 			// 音视频拼接通道使能
} TGuiSetAVMedleyReq;

typedef struct
{
	int m_iRetCode;				// 音视频拼接通道使能
} TGuiSetAVMedleyRsp;

#define GUI_GET_AV_MEDLEY_REQ			20008	//获取音视频拼接请求
#define GUI_GET_AV_MEDLEY_RSP			-20008 	//获取音视频拼接回应

typedef struct
{
	int m_iChn; 			// 音视频拼接通道使能
} TGuiGetAVMedleyReq;

typedef struct
{
	int m_iChn;
	int m_iEnable;			// 音视频拼接通道使能
} TGuiGetAVMedleyRsp;

//------------------------音频编码方式-------------------------------
#define GUI_GET_CHN_AUDIO_CODETYPE_LST_REQ  239   //获取通道音频编码方式列表请求
#define GUI_GET_CHN_AUDIO_CODETYPE_LST_RSP  -239  //获取通道音频编码方式列表回应

typedef struct
{
	int m_iChn;
}TGuiGetChnAudioCodeTypeLstReq;   //获取通道音频编码方式列表请求

typedef struct
{
	int m_iChn;
	int m_iCnt;
}TGuiGetChnAudioCodeTypeLstRsp;   //获取通道音频编码方式列表回应

typedef struct
{
	int m_iAudioCodeType;
}TGuiAudioCodeTypeItem;  //音频编码方式

//-----------------------视频编码方式-------------------------------
#define GUI_GET_CHN_VIDEO_CODETYPE_LST_REQ  245   //获取通道视频编码方式列表请求
#define GUI_GET_CHN_VIDEO_CODETYPE_LST_RSP  -245  //获取通道视频编码方式列表回应

typedef struct
{
	int m_iChn;
}TGuiGetChnVideoEncodeTypeLstReq;   //获取通道视频编码方式列表请求

typedef struct
{
	int m_iChn;
	int m_iCnt;
}TGuiGetChnVideoEncodeTypeLstRsp;   //获取通道视频编码方式列表回应

typedef struct
{
	int m_iVideoEncodeType;
}TGuiVideoEncodeTypeItem;  //视频编码方式

//------------------------------ 4.视频前端叠加 --------------------------
typedef struct
{
	int m_iXp;
	int m_iYp;
} TGuiLoc;

typedef struct
{
	int m_iTop;
	int m_iLeft;
	unsigned int m_uiWidth;
	unsigned int m_uiHeight;
} TGuiArea;

#define GUI_CHN_NAME_LEN		32				//通道名最大长度
#define GUI_CHN_TEXT_LEN		255				//自定义文本最大长度
#define GUI_MAX_COVER_AREA		4

typedef struct									//视频叠加参数
{
	int m_iNameOverlayEnable;					//通道名叠加使能标识
	char m_strName[GUI_CHN_NAME_LEN + 1];		//通道名
	int m_iChnColor;                            //通道颜色                          
	TGuiLoc m_tNameLoc;						//通道名叠加位置
	
	int m_iTimeOverlayEnable;					//时间叠加使能标识
	int m_iTimeFormat;							//时间格式,低八位代表format，第九位代表是否叠加星期，第十位为0代表24时制、为1代表12时制
	int m_iTimeColor;                           //叠加时间颜色
	TGuiLoc m_tTimeLoc;						//时间叠加位置
	
	int m_iTextEnable;							//自定义文本叠加使能
	char m_strText[GUI_CHN_TEXT_LEN + 1];		//自定义文本
	TGuiLoc m_tTextLoc;						//文本叠加位置

	int m_iLogoEnable;							//Logo叠加使能标识
	TGuiLoc m_tLogoLoc;						//Logo叠加位置

	int m_iVideoCoverEnable;					//视频遮档使能标识
	int m_iCoverAreaCnt;						//视频遮档区域已启用的个数
	TGuiArea m_tCoverAreas[GUI_MAX_COVER_AREA]; //视频遮档区域
} TGuiVideoOverlayCnf;

typedef struct									//获取视频叠加参数请求
{
	int m_iChn;									//通道号
} TGuiGetVideoOverlayCnfReq;

typedef struct									//获取视频叠加参数回应
{
	int m_iChn;									//通道号
	TGuiVideoOverlayCnf m_tCnf;					//配置参数
} TGuiGetVideoOverlayCnfRsp;

typedef struct									//设置视频叠加参数请求
{
	int m_iChn;									//通道号
	TGuiVideoOverlayCnf m_tCnf;					//配置参数							
} TGuiSetVideoOverlayCnfReq;

#define GUI_SET_VOVERLAY_CNF_OK	0			//设置成功
#define GUI_SET_VOVERLAY_CNF_ERR	1			//设置失败
typedef struct									//设置视频叠加参数回应
{
	int m_iRetCode;							//返回码
} TGuiSetVideoOverlayCnfRsp;

typedef struct									//更换logo图标请求
{
	int m_iChn;									//通道号
} TGuiChgLogoBmpReq;

#define GUI_CHG_LOGO_BMP_OK		0			//更换图标成功
#define GUI_CHG_LOGO_BMP_ERR		1			//更换图标失败
typedef struct									//更换logo图标回应
{
	int m_iRetCode;
} TGuiChgLogoBmpRsp;

//==============================录像设置 ===============================
//=======================假日计划========================
#define GUI_GET_HOLIDAY_MSG_REQ		296	 	//获取假日计划信息请求
#define GUI_GET_HOLIDAY_MSG_RSP		-296	//获取假日计划信息回应

#define GUI_SET_HOLIDAY_MSG_REQ		297		//设置假日计划信息请求
#define GUI_SET_HOLIDAY_MSG_RSP		-297	//设置假日计划信息回应

#define GUI_HOLIDAY_NAME_LEN	32
#define GUI_MAX_HOLIDAY_CNT		10

enum
{
	HOLIDAY_TYPE_WEEK = 1, 	//星期
	HOLIDAY_TYPE_MONTH,		//月份
	HOLIDAY_TYPE_DATE,		//日期
}; //假日类型

typedef struct
{
	int m_iId;   //假日编号
	char m_strHolidayName[GUI_HOLIDAY_NAME_LEN + 1];  //假日名称
	int m_iStatus;  //0表不开启，1表示开启
	int m_iType;   //假日类型，按照哪种类型，分为星期、月份、日期
	int m_iStartFirst;    //
	int m_iStartSecond;  //
	int m_iStartLast;   //
	int m_iEndFirst;
	int m_iEndSecond;
	int m_iEndLast;
}TGuiHolidayMsg;
/*如果类型是日期，则m_iStartFirst，m_iStartSecond，m_iStartLast表示开始时间的年月日
类型是星期，则m_iStartFirst，m_iStartSecond，m_iStartLast表示开始时间的第几个月、第几个星期，星期几；如果类型是月份，则m_iStartFirst，m_iStartSecond表示第几个月，几号，m_iStartLast，m_iEndLast不使用 */

typedef struct
{
	TGuiHolidayMsg m_tHolidayMsg[GUI_MAX_HOLIDAY_CNT];
}TGuiGetHolidayMsgRsp; //获取假日计划信息回应

typedef struct
{
	TGuiHolidayMsg m_tHolidayMsg[GUI_MAX_HOLIDAY_CNT];
}TGuiSetHolidayMsgReq;       //设置假日计划信息请求

#define GUI_SET_HOLIDAY_MSG_OK   0 //设置成功
#define GUI_SET_HOLIDAY_MSG_ERR	1	//设置失败
typedef struct
{
	int m_iRetCode;         //0表示成功，1表示失败
} TGuiSetHolidayMsgRsp; 	//设置假日计划信息回应

//=======================录像保留时间====================
#define GUI_GET_REC_REMAIN_TIME_REQ   298 	//获取录像文件保留时间请求
#define GUI_GET_REC_REMAIN_TIME_RSP   -298	//获取录像文件保留时间回应
#define GUI_SET_REC_REMAIN_TIME_REQ   299	//设置录像文件保留时间请求
#define GUI_SET_REC_REMAIN_TIME_RSP   -299	//设置录像文件保留时间回应

typedef struct
{
	int m_iChn;					//高16位 0-主码流1-副码流
}TGuiGetRecRemainTmReq;		//获取录像文件保留时间请求

typedef struct
{
	int m_iChn;				//高16位 0-主码流1-副码流
	int m_iTm;  			//单位：天数
}TGuiGetRecRemainTmRsp;	//获取录像文件保留时间回应

typedef struct
{
	int m_iChn;				//高16位 0-主码流1-副码流
	int m_iTm;	
}TGuiSetRecRemainTmReq;	//设置录像文件保留时间请求

#define GUI_SET_REC_REMAIN_TM_OK  0 //设置成功
#define GUI_SET_REC_REMAIN_TM_ERR 1	//设置失败
typedef struct
{
	int m_iRetCode;
}TGuiSetRecRemainTmRsp;	//设置录像文件保留时间回应




//=========================录像类型名==============================
//录像类型名
#define GUI_GET_VIDEO_TYPE_REQ 			390      	//录像类型请求
#define GUI_GET_VIDEO_TYPE_RSP			-390
#define GUI_SET_VIDEO_TYPE_REQ 			391      	//录像类型设置
#define GUI_SET_VIDEO_TYPE_RSP 			-391      //录像类型设置回应

#define GUI_VIDEO_TYPE_NAME_STR_LEN	16

typedef struct
{
	int m_iChnNum;       //类型号
	char m_strVideoType[GUI_VIDEO_TYPE_NAME_STR_LEN + 1];           //录像类型名
}TGuiVideoTypeName;

typedef struct
{
	int m_iRetCode;
}TGuiSetTypeRsp;

typedef struct
{
	int m_iTypeId;			//自定义录像类型号，32-63
	char m_strTypeName[GUI_VIDEO_TYPE_NAME_STR_LEN + 1];    //录像类型名
} TGuiGetVideoTypeReq;

typedef struct
{
	int m_iTypeId;			//自定义录像类型号，32-63，-1表获取失败
	char m_strTypeName[GUI_VIDEO_TYPE_NAME_STR_LEN + 1];    //录像类型名
} TGuiGetVideoTypeRsp;

typedef struct
{
	int m_iTypeId;			//自定义录像类型号，32-63
	char m_strTypeName[GUI_VIDEO_TYPE_NAME_STR_LEN + 1];    //录像类型名
} TGuiSetVideoTypeReq;

typedef struct
{
	int m_iRetCode;
} TGuiSetVideoTypeRsp;

//=======================录像文件加锁=====================
#define GUI_VF_NAME_LEN		67				//录像文件名最大长度


#define GUI_GET_RECFILE_LOCK_STATUS_REQ		394   //获取录像文件锁状态请求
#define GUI_GET_RECFILE_LOCK_STATUS_RSP		-394  //获取录像文件锁状态回应

enum//锁状态
{
	RECFILE_UNLOCK = 0,  //0表示解锁
	RECFILE_LOCK,        //1表加锁
};

typedef struct
{
	char m_strFileName[GUI_VF_NAME_LEN + 1];	//录像文件名
}TGuiGetRecFileLockStatusReq;	

typedef struct
{
	char m_strFileName[GUI_VF_NAME_LEN + 1];	//录像文件名
	int m_iAction;  //锁状态
} TGuiGetRecFileLockStatusRsp;

#define GUI_SET_RECFILE_LOCK_STATUS_REQ		395   //设置录像文件锁状态请求
#define GUI_SET_RECFILE_LOCK_STATUS_RSP		-395 //设置录像文件锁状态回应

typedef struct
{
	char m_strFileName[GUI_VF_NAME_LEN + 1];	//录像文件名
	int m_iAction;  //锁状态
} TGuiSetRecFileLockStatusReq;

#define GUI_SET_RECFILE_LOCK_STATUS_OK	 0  //设置成功
#define GUI_SET_RECFILE_LOCK_STATUS_ERR 1	//设置失败
typedef struct
{
	int m_iRetCode;   //0表成功，1表示失败
} TGuiSetRecFileLockStatusRsp;



//=========================录像模板=================================

// 录像类型码
typedef enum
{
	GUI_VR_TYPE_ALL = 0,		//全部录像
	GUI_VR_TYPE_HUM,				//手动录像
	GUI_VR_TYPE_ALARM,				//报警录像
	GUI_VR_TYPE_TIME,				//定时录像
	GUI_VR_TYPE_MD,				//移动侦测录像
	GUI_VR_TYPE_LOST,				//视频丢失录像
	GUI_VR_TYPE_MD_EX,				//移动侦测|报警录像
	GUI_VR_TYPE_ALOST,			//音频丢失报警
	GUI_VR_TYPE_OD,					//视频遮挡报警
	GUI_VR_TYPE_MD_ALARM,			//移动侦测&报警录像
		
	GUI_VR_TYPE_VCA = 32,                      //智能分析| 报警录像  
	GUI_VR_TYPE_TEMP,				//温湿度报警录像
	GUI_VR_TYPE_ATM,				//ATM 卡号查询
	GUI_VR_TYPE_ITS,					//ITS 录像
}EGuiRecType;

typedef struct									//录像时段模板
{
	int m_iEnable;								//该录像时段是否启用
	int m_iStartTm;							//录像开始时间,以当天00:00起流过的秒数
	int m_iEndTm;								//录像结束时间
	int m_iType;								//录像类型
} TGuiVideoRecTmTmp;

#define GUI_VR_TM_CNT_PER_DAY		4			//每天最大可设时段
typedef struct									//录像日期模板
{
	TGuiVideoRecTmTmp m_tRecTm[GUI_VR_TM_CNT_PER_DAY];
} TGuiVideoRecDayTmp;

typedef struct									//录像模板
{
	int m_iPreRcdTm;							//预录时长，以秒计
	int m_iDelayTm;							//延时时长，以秒计
	TGuiVideoRecDayTmp m_tRecDay[7];			//一周七天录像参数
} TGuiVideoRecTemplate;

typedef struct									//获取录像模板请求
{
	int m_iChn;									//录像通道
} TGuiGetVideoTemplateReq;

typedef struct									//获取录像模板回应
{
	int m_iChn;									//通道号
	TGuiVideoRecTemplate m_tTmp;				//通道对应的录像模板
} TGuiGetVideoTemplateRsp;

typedef struct									//设置录像模板请求
{
	int m_iChn;
	TGuiVideoRecTemplate m_tTmp;				//通道对应的录像模板
} TGuiSetVideoTemplateReq;

#define GUI_SET_VR_TEMPLATE_OK		0		//设置录像模板成功
#define GUI_SET_VR_TEMPLATE_ERR		1		//设置录像模板失败
typedef struct									//设置录像模板回应
{
	int m_iRetCode;
} TGuiSetVideoTemplateRsp;

#define GUI_SET_AUDIOREC_TEMPLATE_REQ   302  //录像模板中设置记录音频请求
#define GUI_SET_AUDIOREC_TEMPLATE_RSP   -302  //录像模板中设置记录音频回应
typedef struct
{
	int m_iChn;
	int m_iAudioRec; // 1-记录 0 -不记录
}TGuiSetAudioRecTemplateReq;
typedef struct
{
	int m_iRetCode;
}TGuiSetAudioRecTemplateRsp;

#define GUI_GET_AUDIOREC_TEMPLATE_REQ   303  //录像模板中获取记录音频请求
#define GUI_GET_AUDIOREC_TEMPLATE_RSP   -303  //录像模板中获取记录音频回应
typedef struct
{
	int m_iChn;
}TGuiGetAudioRecTemplateReq;
typedef struct
{
	int m_iChn;
	int m_iAudioRec; // 1-记录 0 -不记录
}TGuiGetAudioRecTemplateRsp;


#define GUI_GET_BACKUPREC_TEMPLATE_REQ   304	//录像模板中获取冗余录像请求
#define GUI_GET_BACKUPREC_TEMPLATE_RSP   -304	//录像模板中获取冗余录像回应

typedef struct
{
	int m_iChn;
}TGuiGetBackUpRecTemplateReq;

typedef struct
{
	int m_iChn;
	int m_iBackUpRec; // 1-冗余录像0 -不冗余录像
}TGuiGetBackUpRecTemplateRsp;

#define GUI_SET_BACKUPREC_TEMPLATE_REQ   305  //录像模板中设置冗余录像请求
#define GUI_SET_BACKUPREC_TEMPLATE_RSP   -305  //录像模板中设置冗余录像回应

#define GUI_SET_BR_TEMPLATE_OK		0		//设置录像模板成功
#define GUI_SET_BR_TEMPLATE_ERR		1		//设置录像模板失败
typedef struct
{
	int m_iChn;
	int m_iBackUpRec; // 1-冗余录像0 -不冗余录像
}TGuiSetBackUpRecTemplateReq;

typedef struct
{
	int m_iRetCode;
}TGuiSetBackUpRecTemplateRsp;

//===================假日计划录像模板=========================
#define GUI_GET_HOLIDAY_VR_TEMPLATE_REQ	  	306		//获取假日计划录像模板请求
#define GUI_GET_HOLIDAY_VR_TEMPLATE_RSP		-306	//获取假日计划录像模板回应

typedef struct
{
	int m_iPreRcdTm;							//预录时长，以秒计
	int m_iDelayTm;							//延时时长，以秒计
	TGuiVideoRecDayTmp m_tRecDay;			//一周七天录像参数	
}TGuiHolidayRecTemplate;

typedef struct
{
	int m_iChn;
}TGuiGetHolidayTempReq;		//获取假日计划录像模板请求

typedef struct
{
	int m_iChn;
	TGuiHolidayRecTemplate m_tHolidayTemp;
}TGuiGetHolidayTempRsp;	//获取假日计划录像模板回应

#define GUI_SET_HOLIDAY_VR_TEMPLATE_REQ		307		//设置假日计划录像模板请求
#define GUI_SET_HOLIDAY_VR_TEMPLATE_RSP		-307	//设置假日计划录像模板回应

typedef struct
{
	int m_iChn;
	TGuiHolidayRecTemplate m_tHolidayTemp;
}TGuiSetHolidayTempReq;		//设置假日计划录像模板请求

#define GUI_SET_HOLIDAY_TEMP_OK   0  //设置成功
#define GUI_SET_HOLIDAY_TEMP_ERR  1	 //设置失败
typedef struct
{
	int m_iRetCode;
}TGuiSetHolidayTempRsp;		//设置假日计划录像模板回应

//=======================抽帧录像===============================
#define GUI_GET_TAKEIFRAME_TEMPLATE_REQ		308		//获取是否抽帧录像请求
#define GUI_GET_TAKEIFRAME_TEMPLATE_RSP		-308	//获取是否抽帧录像回应

typedef struct
{
	int m_iChn;
}TGuiGetTakeIFrameTemplateReq;		//获取是否抽帧录像请求

typedef struct
{
	int m_iChn;
	int m_iTakeIFrame;	 // 0 -不抽帧录像 1-抽帧录像
}TGuiGetTakeIFrameTemplateRsp;		//获取是否抽帧录像回应

#define GUI_SET_TAKEIFRAME_TEMPLATE_REQ		309		//设置是否抽帧录像请求
#define GUI_SET_TAKEIFRAME_TEMPLATE_RSP		-309	//设置是否抽帧录像回应

#define GUI_SET_TAKEIFRAME_TEMPLATE_OK		0		//设置录像模板成功
#define GUI_SET_TAKEIFRAME_TEMPLATE_ERR		1		//设置录像模板失败
typedef struct
{
	int m_iChn;
	int m_iTakeIFrame; //0 -不抽帧录像  1-抽帧录像
} TGuiSetTakeIFrameTemplateReq;		//设置是否抽帧录像请求

typedef struct
{
	int m_iRetCode;
} TGuiSetTakeIFrameTemplateRsp;		//设置是否抽帧录像回应

//=================================
#define GUI_GET_SUBSTREAMREC_TEMPLATE_REQ		312		//获取是否副码流录像请求
#define GUI_GET_SUBSTREAMREC_TEMPLATE_RSP		-312	//获取是否副码流录像回应

typedef struct
{
	int m_iChn;
}TGuiGetSubStreamRecTemplateReq;		//获取是否开启副码流录像请求

typedef struct
{
	int m_iChn;
	int m_iSubStreamRec;	 			// 0 -不启用副码流录像 1-启用(见公共宏定义)
}TGuiGetSubStreamRecTemplateRsp;		//获取是否副码流录像回应

//=================================
#define GUI_SET_SUBSTREAMREC_TEMPLATE_REQ		313		//设置是否副码流录像请求
#define GUI_SET_SUBSTREAMREC_TEMPLATE_RSP		-313	//设置是否副码流录像回应

typedef struct
{
	int m_iChn;
	int m_iSubStreamRec; 		// 0 -不启用副码流录像 1-启用(见公共宏定义)
} TGuiSetSubStreamRecTemplateReq;		//设置是否副码流录像请求

typedef struct
{
	int m_iRetCode; 
} TGuiSetSubStreamRecTemplateRsp;		//设置是否副码流录像回应

//=================================

#define GUI_GET_VR_SMART_TEMPLATE_REQ	314			//获取智能录像模板请求
#define GUI_GET_VR_SMART_TEMPLATE_RSP	-314		//获取智能录像模板回应
#define GUI_SET_VR_SMART_TEMPLATE_REQ	315			//设置智能录像模板请求
#define GUI_SET_VR_SMART_TEMPLATE_RSP	-315		//设置智能录像模板回应

#define GUI_VR_TM_DAY_CNT  8

enum
{
	TEMPLATE_DISABLE,  //模板不生效
	TIME_TEMPLATE_1,   //定时模板1
	TIME_TEMPLATE_2,   //定时模板2
};

typedef  struct
{
	int m_iChn;
}TGuiGetSmartTemplateReq;

//iTemplateType[x][x] 的值枚举参考编码类型，默认为-1 
typedef struct
{
	int iTemplateType[GUI_VR_TM_DAY_CNT] [GUI_VR_TM_CNT_PER_DAY];   //每个通道对应每天对应4个时间段，每个时间段一个模板，按下表索引
}TGuiVideoRecSmartTemplate;


typedef struct
{
	int m_iChn;
	TGuiVideoRecSmartTemplate m_tSmartTempCnf;
} TGuiGetRecSmartTemplateRsp;  //获取智能录像模板回应

typedef struct
{
	int m_iChn;
	TGuiVideoRecSmartTemplate m_tSmartTempCnf;
} TGuiSetRecSmartTemplateReq; //设置智能录像模板请求

//0表成功 1表失败
typedef struct
{
	int m_iRetCode;
} TGuiSetRecSmartTemplateRsp; //设置智能录像模板回应


//================ANR断网补录=============
#define GUI_GET_CHN_ANR_ENABLE_REQ   	2030 //获取ANR使能状态请求
#define GUI_GET_CHN_ANR_ENABLE_RSP   -2030//获取ANR使能状态回应
typedef struct
{
	int m_iChn;
}TGuiGetChnAnrEnableReq;

typedef struct
{
	int m_iChn;
	int m_iEnable;
}TGuiGetChnAnrEnableRsp;

#define GUI_SET_CHN_ANR_ENABLE_REQ 	2031//设置ANR使能状态请求
#define GUI_SET_CHN_ANR_ENABLE_RSP	-2031//设置ANR使能状态回应
typedef struct
{
	int m_iChn;
	int m_iEnable;
}TGuiSetChnAnrEnableReq;

typedef struct
{
	int m_iRetCode;         //0表成功，1表失败
}TGuiSetChnAnrEnableRsp;

//===================录像策略===================================


#define GUI_VRF_ENAME_LEN				5		//录像文件扩展名长度
#define GUI_VRF_PACK_BY_TIME			2		//按时间长度打包
#define GUI_VRF_PACK_BY_SIZE			1		//按文件大小打包

// 磁盘空间不足时动作
#define GUI_DEL_VRF_WHEN_DOS			1		//删除文件
#define GUI_DEL_NA_VRF_WHEN_DOS			2		//删除非报警录像文件
#define GUI_STOP_VR_WHEN_DOS			3		//停止录像

typedef struct									//录像策略参数
{
	char m_strRxtName[GUI_VRF_ENAME_LEN + 1];	//录像文件扩展名
	int m_iFilePackType;						//录像文件打包方式
	int m_iFilePackTime;						//打包时间(单位秒)
	int m_iFilePackSize;						//者打包大小(单位K)
	int m_iActionWhenDiskOutOfSpace;			//磁盘空间不足时的动作(编号1到3)
} TGuiVideoStrategy;

typedef struct									//获取录像策略回应
{
	TGuiVideoStrategy m_tStrategy;
} TGuiGetVideoStrategyRsp;

typedef struct									//设置录像策略请求
{
	TGuiVideoStrategy m_tStrategy;
} TGuiSetVideoStrategyReq;

#define GUI_SET_VR_STRATEGY_OK		0			//设置录像策略成功
#define GUI_SET_VR_STRATEGY_ERR		1			//设置录像策略失败
typedef struct									//设置录像策略回应
{
	int m_iRetCode;
} TGuiSetVideoStrategyRsp;


// 手动录像
#define GUI_START_HUM_VIDEO		1			//启动手动录像
#define GUI_STOP_HUM_VIDEO		2			//停止手动录像
#define GUI_START_ITS_ILLEGAL_VIDEO     69  //启动交通违法录像
#define GUI_STOP_ITS_ILLEGAL_VIDEO      70  //停止交通违法录像

typedef struct									//手动录像命令
{
	int m_iChn;									//通道号(0-16, 0表示针对全部通道)
	int m_iCmd;									//手动录像命令:启动/停止
} TGuiHumRecVideoCmd;

//定义的新的有回应的手动录像命令协议
#define GUI_HUM_REC_VIDEO_CMD_REQ  352            //手动录像命令
#define GUI_HUM_REC_VIDEO_CMD_RSP  -352            //手动录像命令
typedef struct
{
	int m_iChn;              //通道号(0-16, 0表示针对全部通道)
	int m_iCmd;              //手动录像命令:启动/停止 GUI_START_HUM_VIDEO /GUI_STOP_HUM_VIDEO
}TGuiHumRecVideoCmdReq;    //手动录像命令请求

#define GUI_HUM_REC_VIDEO_CMD_OK   0 //命令执行成功
#define GUI_HUM_REC_VIDEO_CMD_ERR  1 //命令执行失败
typedef struct
{
	int m_iRetCode;
}TGuiHumRecVideoCmdRsp;   //手动录像命令回应

// 通用录像
#define GUI_START_REC_CMD		1
#define GUI_STOP_REC_CMD		2
typedef struct
{
	int m_iChn;					//通道号(0表全部,1-16表通道)
	int m_iCmd;					//开始/停止录像
	int m_iType;				//录像类型
} TGuiRecReq;

typedef struct
{
	int m_iRetCode;				//返回码, 0成功,其他失败
} TGuiRecRsp;

// 录像文件
//月照
typedef struct									//获取录像文件快照请求
{
	int m_iChn;     //查询的通道 
	int m_iYear;	//年 >= 2008
	int m_iMon;		//月: 1-12
} TGuiGetVideoFileMapReq;

typedef struct
{
	int m_iChn;
	int m_iYear;
	int m_iMon;
	int m_iFileMap[32];	//0号位置保留，需填充0, 1-31号位置存放对应日期是否有录像标志:0无，1有
} TGuiGetVideoFileMapRsp;

//日照
#define GUI_GET_VIDEO_FILE_DAY_MAP_REQ  319     //获取录像文件按天快照请求(1天)
#define GUI_GET_VIDEO_FILE_DAY_MAP_RSP  -319    //获取录像文件按天快照回应(1天)

#define GUI_MAX_VIDEO_SECT_PER_DAY 300  //每个通道最大300个时间段
typedef struct
{
	int m_iChn;
	int m_iYear;
	int m_iMon;
	int m_iDay;
}TGuiGetVideoFileDayMapReq;

typedef struct
{
	int m_iChn;
	int m_iYear;
	int m_iMon;
	int m_iDay;
	int m_iCnt;  //录像个数
}TGuiGetVideoFileDayMapRsp;

typedef struct
{
	int m_iType; //录像类型
	int m_iStartTime;
	int m_iEndTime;
}TGuiGetVideoFileDayMapMsg;  //录像文件信息


typedef enum
{
	GUI_VIDEO_FILE = 1,
	GUI_IMAGE_FILE,
}EGuiFileType;

#define GUI_GET_VIDEO_FILE_MAP_REQ_EX	1319		//获取录像文件快照请求(一个月哪些天有录像)
#define GUI_GET_VIDEO_FILE_MAP_RSP_EX	-1319	//获取录像文件快照回应
typedef struct									//获取录像文件快照请求
{
	int m_iChn;   		  //查询的通道 
	int m_iYear;			//年 >= 2008
	int m_iMon;			//月: 1-12
	int m_iFileType;			//见枚举EGuiFileType
	int m_iRecordType;		//见枚举EGuiRecType
} TGuiGetVideoFileMapReqEx;
typedef TGuiGetVideoFileMapRsp TGuiGetVideoFileMapRspEx;

#define GUI_GET_VIDEO_FILE_DAY_MAP_REQ_EX		1320	//获取录像文件快照请求(日照)
#define GUI_GET_VIDEO_FILE_DAY_MAP_RSP_EX		-1320	//获取录像文件快照回应
typedef struct
{
	int m_iChn;
	int m_iYear;
	int m_iMon;
	int m_iDay;
	int m_iFileType;
	int m_iRecordType;
}TGuiGetVideoFileDayMapReqEx;

typedef TGuiGetVideoFileDayMapRsp TGuiGetVideoFileDayMapRspEx;


//====================================
//录像查询
#define GUI_QUERY_VIDEO_FILE_REQ	321			//查询录像文件请求
#define GUI_QUERY_VIDEO_FILE_RSP	-321		//查询录像文件回应

typedef struct									//录像文件查询条件
{
	int m_iChn;									//通道号 高16位0-主码流1-副码流
	int m_iDev;									//0~7位表ATM操作类型，8~15ATM异常，16~23ATM端口号24非加锁文件，25~31预留
	int m_iType;								//录像文件类型
	int m_iTypeRange;                        //录像范围
	int m_iBeginTm;							//开始时间
	int m_iEndTm;								//结束时间
} TGuiVideoFileQueryCondition;

typedef struct									//统计录像文件请求
{
	TGuiVideoFileQueryCondition m_tCondition;	//查询条件
} TGuiCountVideoFileReq;

typedef struct									//统计录像文件回应
{
	int m_iVideoFileCnt;						//录像文件总个数
} TGuiCountVideoFileRsp;

typedef struct									//查询录像文件请求
{
	TGuiVideoFileQueryCondition m_tCondition;	//查询条件
	int m_iPageId;								//页码(第一页为0)
	int m_iRcntPerPage;						//每页记录条数
} TGuiQueryVideoFileReq;

typedef struct									//查询录像文件回应
{
	int m_iCnt;									//次此查得录像文件个数
} TGuiQueryVideoFileRsp;						//随后发送相应个数的录像文件源料
//====================================


typedef long long TGuiVideoFileId;			//录像文件编号类型
typedef struct									//录像文件资料
{
	#if 0 //文件编号未用
	TGuiVideoFileId m_tFileId;					//录像文件编号
	#endif
	char m_strFileName[GUI_VF_NAME_LEN + 1];	//录像文件名
	unsigned int m_uiFileSize;					//录像文件大小
	int m_iBeginTm;							//录像文件开始时间
	int m_iEndTm;								//录像文件结束时间
	int m_iChn;									//录像文件对应的通道号
	int m_iDev;									//录像文件对应的设备号
	int m_iType;								//录像文件类型
} TGuiVideoFileMsg;

typedef struct									//备份录像文件请求
{
	#if 0
	TGuiVideoFileId m_tFileId;					//录像文件编号
	#endif
	char m_strFileName[GUI_VF_NAME_LEN + 1];	//录像文件名
	int m_iDstDevId;							//目录设备号
} TGuiVideoFileBackupReq;

#define GUI_BACKUP_VIDEO_FILE_OK			0	//备份录像文件成功
#define GUI_BACKUP_VIDEO_FILE_ERR			1	//备份录像文件失败
#define GUI_BACKUP_VIDEO_FILE_NO_DEV		2	//找不到设备错误
#define GUI_BACKUP_VIDEO_FILE_NO_SPACE		3	//磁盘空间不足错误
typedef struct									//备份录像文件回应
{
	int m_iRetCode;
} TGuiVideoFileBackupRsp;

typedef struct									//删除录像文件请求
{
	#if 0 //文件编号未用
	TGuiVideoFileId m_tFileId;					//录像文件编号
	#endif
	char m_strFileName[GUI_VF_NAME_LEN + 1];	//录像文件名	
} TGuiVideoFileDeleteReq;

#define GUI_DEL_VIDEO_FILE_OK			0		//删除文件成功
#define GUI_DEL_VIDEO_FILE_ERR			1		//删除文件失败
typedef struct									//删除录像文件回应
{
	int m_iRetCode;							//返回码
} TGuiVideoFileDeleteRsp;

#define  GUI_DSP_VIDEOFILE_BYPIC_REQ  	  2302      //请求查询录像结果以图表显示
#define  GUI_DSP_VIDEOFILE_BYPIC_RSP	  -2302    //回应查询录像结果以图表显示
typedef struct
{
	char  m_strVFileName[GUI_VF_NAME_LEN + 1];     //文件名
	char  m_strPicPath[GUI_PUBLIC_LEN_63 + 1];       //内核存放I帧bmp的路径,ui指定
}TGuiDspVFByPicReq;

typedef struct
{
	int  m_iRetCode;          //返回值
}TGuiDspVFByPicRsp;

//重建索引
typedef struct									//重建索引回应
{
	int m_iRebuildId;							//重建索引标识号(小于0表拒绝请求，其他值唯一标识此次重建操作)
} TGuiRebuildIndexRsp;

typedef struct									//获取索引状态请求
{
	int m_iRebuildId;							//重建索引标识号
} TGuiGetRebuildIndexStaReq;

#define GUI_REBUILD_INDEX_ERR		0			//重建索引发生错误
#define GUI_REBUILD_INDEX_DOING		1			//重建索引进行中
#define GUI_REBUILD_INDEX_DONE		2			//重建索引完成
typedef struct									//获取索引状态回应
{
	int m_iState;								//状态码
	int m_iProgress;							//当前进度0-100(保留，如果不支持填充0)
} TGuiGetRebuildIndexStaRsp;

//录像回放
typedef struct									//录像回放请求
{
	int m_iVoId;								//VO通道号(暂时保留)
	char m_strFile[GUI_VF_NAME_LEN + 1];		//欲播放的录像文件
	int m_iSoundEnable;						//声音使能(0/1)
} TGuiVideoPlayReq;

#define GUI_VIDEO_PLAY_OK				0		//录像回放请求成功
#define GUI_VIDEO_PLAY_FILE_LOST		1		//录像文件不存在
#define GUI_VIDEO_PLAY_FILE_FERR		2		//无法打开或者播放指定的文件
typedef struct									//录像回放回应
{
	int m_iRetCode;							//返回码
	int m_iPlayId;								//录像回放编号，其后对该次播放的相关操作均
												//以此号标识
	long long m_llFrameCnt;					//总帧数
	int m_iFrameRate;							//帧率，每秒回放的帧数
} TGuiVideoPlayRsp;

typedef struct									//录像回放暂停请求
{
	int m_iPlayId;								//回放编号，由GuiVideoPlayRsp指定
} TGuiVideoPlayPauseReq;

typedef struct									//录像文件继播放请求
{
	int m_iPlayId;								//回放编号
} TGuiVideoPlayCntnReq;

typedef struct									//录像文件播放停止请求
{
	int m_iPlayId;
} TGuiVideoPlayStopReq;

typedef struct									//单帧播放(无回应)
{
	int m_iPlayId;								//播放编号
	int m_iType;								//步进类型(1:步进, -1步退)
} TGuiVideoPlayStepCmd;

typedef struct									//播放定位请求(无回应)
{
	int m_iPlayId;								//播放编号
	int m_iLoc;									//定位百分比(0-100)
} TGuiVideoPlayLocCmd;

//========================录像倒放====================
#define GUI_VIDEO_PLAY_REVERSE_REQ	            348    //倒放
typedef struct									//录像回放倒放请求
{
	int m_iPlayId;								//回放编号，由GuiVideoPlayRsp指定
} TGuiVideoPlayReverseReq;

//========================录像回放中抓拍=============
#define GUI_VIDEO_PLAY_SNAP_REQ					349    //回放抓拍
typedef struct									//录像回放抓拍请求
{
	int m_iPlayId;								//回放编号，由GuiVideoPlayRsp指定
} TGuiVideoPlaySnapReq;

//-----------------------ATM录像查询定位-----------------------------------------------
//ATM 根据操作时间定位录像
#define GUI_PLAY_VIDEO_LOCEX_REQ		345		//按绝对时间定位
typedef struct
{
	int m_iPlayId;				//播放编号
	int m_iAbsLocTm;			//具体时间
}TGuiVideoPlayLocCmdEx; //按绝对时间定位

//卡号录像查询
#define GUI_QUERY_ATM_VIDEO_FILE_REQ		346
#define GUI_QUERY_ATM_VIDEO_FILE_RSP		-346

#define MAX_CRADNO_LEN		64
#define MAX_FILED_NUM       5   //5个字段

typedef enum
{
	QUERY_BY_CARDNO = 0,    //按卡号查询
	QUERY_BY_FIELD,			//按域查询
}TAtmQueryType;      //卡号录像查询方式，分为按卡号和按域查询

typedef enum
{
	ATM_OPERATE_ALL = 0,	//全部
	ATM_QUERY,				//查询
	ATM_TAKE,				//取款
	ATM_CHANGEPASSWD,		//改密
	ATM_TRANSFER,			//转账
	ATM_DEPOSIT,			//存款
	ATM_QUERY_NOCARD,		//无卡查询
	ATM_DEPOSIT_NOCARD,		//无卡存款
	ATM_OTHER,				//其他
	ATM_INSERT_CARD,    	//插卡
	ATM_REMOVE_CARD,		//拔卡
	ATM_NULL = 0xff,		//无操作
}TAtmOperateType;

typedef enum
{
	EXCEPTION_ALL = 0,			//全部	
	EXCEPTION_EATMONEY,		//吞钞
	EXCEPTION_EATCARD,			//吞卡
	EXCEPTION_NULL = 0xff,		//无异常
}TAtmExpType;

typedef struct
{
	int  iType;                     //0为按卡号查询，1为按域查询
	char m_cCardNum[MAX_FILED_NUM][MAX_CRADNO_LEN + 1];	  //卡号 和域复用，若是卡号，则字符串数组分别为卡号、操作类型、金额、异常，若是域，则分别为域1-域4
}TAtmFileInfo;

typedef struct									//统计录像文件请求
{
	TGuiQueryVideoFileReq m_tBaseCondition;	//查询条件
	TAtmFileInfo	m_tCardCondition;	//卡号查询条件
} TGuiQueryVideoFileExReq;        //扩展的卡号查询录像

#define GUI_QUERY_VIDEO_FILE_BEGIN_END_TM_REQ		347   //查询录像文件的开始时间结束时间请求
#define GUI_QUERY_VIDEO_FILE_BEGIN_END_TM_RSP		-347  //查询录像文件的开始时间结束时间回应
typedef struct
{
	char m_strFileName[GUI_VF_NAME_LEN + 1];	//录像文件名	
}TGuiQueryVideoFileBeginEndTmReq;

typedef struct
{
	char m_strFileName[GUI_VF_NAME_LEN + 1];	//录像文件名	
	int m_iFileBeginTm;                        //文件开始时间
	int m_iFileEndTm;                          //文件结束时间
}TGuiQueryVideoFileBeginEndTmRsp;

//====================即时回放=======================
#define GUI_VIDEO_INSTANT_PLAY_REQ     1100   //即时回放请求
#define GUI_VIDEO_INSTANT_PLAY_RSP	   -1100  //即时回放回应

typedef struct
{
	int	m_iChn;            //通道号
	int	m_iVoDevId; 
	int	m_iVoChn;
	int	m_iTime;
}TGuiVideoInstantPlayReq;

typedef struct
{
	int m_iPlayId;  //-1表示失败，其余表示id
} TGuiVideoInstantPlayRsp;    //0表成功，其余表示失败

//============标签回放============
//标签的增删改查
typedef struct
{
	int m_iTagTm;              //标签时间（与打点信息一致）
	int m_iTagChn;              //标签所在录像文件的VO ID
	char m_strFileName[GUI_VF_NAME_LEN + 1];//录像文件名
	char m_strTagName[GUI_PUBLIC_LEN_63 + 1];   //标签名称   
}TGuiVpTagMsg;     //标签结构体

#define GUI_ADD_VP_TAG_REQ	 1110    //增加标签
#define GUI_ADD_VP_TAG_RSP    -1110
typedef struct
{
	TGuiVpTagMsg m_tVpTagMsg;   
}TGuiAddVpTagReq;                  //增加标签请求

typedef struct
{
	int m_iRetCode; //0表示成功，1表示单个文件满64个标签，2表示其余所有的失败情况，3表示正在录像
	int m_iTagTm;   //返回标签的实际时间
} TGuiAddVpTagRsp;     
      
#define GUI_ADD_VP_TAG_REQ_EX	 	5110    //增加标签
#define GUI_ADD_VP_TAG_RSP_EX   	 -5110
typedef struct
{
	TGuiVpTagMsg m_tVpTagMsg;   
}TGuiAddVpTagReqEx;                  //增加标签请求

typedef struct
{
	int m_iRetCode; //0表示成功，1表示单个文件满64个标签，2表示其余所有的失败情况，3表示正在录像
	TGuiVpTagMsg m_tVpTagMsg;   //返回标签的具体信息
} TGuiAddVpTagRspEx;    

      
#define GUI_DELETE_VP_TAG_REQ 	1111	    //删除标签
#define GUI_DELETE_VP_TAG_RSP	-1111
typedef struct
{
	TGuiVpTagMsg m_tVpTagMsg;   
}TGuiDelVpTagReq;                  //删除标签请求

typedef struct
{
	int m_iRetCode;
} TGuiDelVpTagRsp;

#define GUI_MODIFY_VP_TAG_REQ 	1112	    //编辑标签
#define GUI_MODIFY_VP_TAG_RSP	-1112

typedef struct
{
	TGuiVpTagMsg m_tVpTagMsg;   
}TGuiEditVpTagReq;                  //标签请求

typedef struct
{
	int m_iRetCode;
} TGuiEditVpTagRsp;

#define GUI_SEARCH_VP_TAG_REQ	1113    //搜索标签
#define GUI_SEARCH_VP_TAG_RSP	-1113

typedef struct
{
	TGuiQueryVideoFileReq m_tBaseCondition;
	char m_strTagName[GUI_PUBLIC_LEN_63 + 1];//名字为空表示全部获取，否则按关键字获取
}TGuiSearchVpTagReq;

#define GUI_MAX_VPTAG_CNT_PER_FILE	64   //每个文件最多支持64个标签
//查询时，先返回文件总数，再逐条返回TGuiSearchVpTagRetRsp
typedef TGuiQueryVideoFileRsp TGuiQueryVideoTagRsp;

typedef struct
{
	int m_iFB;   //文件开始时间
	int m_iFE;	 //标签所在文件的结束时间
	TGuiVpTagMsg m_tVpTagMsg;
}TGuiSearchVpTagRetRsp;

//==============外部文件播放============
#define  	 GUI_OUTSIDE_PLAY_VF_REQ	 1114    //播放外部文件请求
#define	 GUI_OUTSIDE_PLAY_VF_RSP	 -1114    //播放外部文件回应

#define GUI_VF_NAME_LEN_EX		127	   //录像文件完整路径
typedef struct
{
	int m_iVoId;				     //VO通道号(暂时保留)	
	int m_iSoundEnable;
	char m_strOutFile[GUI_VF_NAME_LEN_EX + 1];   //录像文件名
}TGuiOutSideVideoPlayReq;

typedef TGuiVideoPlayRsp	TGuiOutSideVpRsp;  //复用之前的结构体

#define GUI_GET_OUTSIDE_VF_TM_REQ	1115	//获取外部文件起止时间的请求
#define GUI_GET_OUTSIDE_VF_TM_RSP	-1115	//获取外部文件起止时间的回应

typedef  struct
{
	char m_strOutFile[GUI_VF_NAME_LEN_EX + 1];   //录像文件名 
}TGuiGetOutSideVfTmReq;

typedef struct
{
	int m_iChn;					//文件所在通道号
	int m_iBeginTm;					//文件开始时间
	int m_iEndTm;					//文件结束时间
	char m_strOutFile[GUI_VF_NAME_LEN_EX + 1];   //录像文件名 
}TGuiGetOutSideVfTmRsp;
//=========end============================


//----------------------------------------------------------------------
typedef struct									//获取当前回放状态请求
{
	int m_iPlayId;
} TGuiGetVideoPlayStaReq;

#define GUI_VIDEO_PLAY_DOING		1			//正在播放状态
#define GUI_VIDEO_PLAY_PUASED		2			//暂停状态
#define GUI_VIDEO_PLAY_STOPED		3			//播放已停止
#define GUI_VIDEO_PLAY_STEP			4			//步进状态
#define GUI_VIDEO_PLAY_REVERSE		5			//倒放状态
#define GUI_VIDEO_PLAY_STEP_REVERSE	6			//步退状态

#define GUI_VIDEO_PLAY_ERR			100			//播放发生错误
#define GUI_VIDEO_PLAY_NEED_PWD		200			//等待密码
//以下四个对应同步回放 按位来
#define CHN_PLAY_PWD(n) 			(200 + (1<<(n - 1))) // n 画面号从1开始
#define GUI_VIDEO_PLAY_NEED_PWD_1  201      //第一画面需要密码
#define GUI_VIDEO_PLAY_NEED_PWD_2  202      //第二画面需要密码
#define GUI_VIDEO_PLAY_NEED_PWD_3  204      //第三画面需要密码
#define GUI_VIDEO_PLAY_NEED_PWD_4  208      //第四画面需要密码

#define GUI_VD_VCA_PLAY_NEED_PWD	-1		//智能搜索用户数据需要密码

#define GUI_VIDEO_PLAY_DECODE_CAP_LIMIT	300         //回放解码性能不足
#define GUI_VIDEO_PLAY_SPEED_CAP_LIMIT  301			//回放速度性能受限，只能越到最大

typedef struct									//获取当前回放状态回应
{
	int m_iPlayId;								//播放序列号
	int m_iCurPlayStat;						//当前播放状态
	int m_iPlayRate;							//当前播放的进度(0-100000)
	int m_iSpeed;								//当前播放倍数(正表加速，负表减速, 0表单步)
} TGuiGetVideoPlayStatRsp;

#define GUI_PLAYER_MAX_VO_CNT		4			//播放器最大支持画面数

typedef struct									//播放器VO属性
{
	int m_iLeft;								//左上角x坐标
	int m_iTop;									//左上角y坐标	
	int m_iWidth;
	int m_iHeight;
} TGuiVoAttr;

typedef struct									//播放器激活初始化消息
{
	int m_iDevId;								//输出设备编号,dxl 2009/12/02
	int m_iVoCnt;								//播放器VO画面数
	// 2008-12-17 dxl 添加Vo属性
	TGuiVoAttr m_tVoAttrs[GUI_PLAYER_MAX_VO_CNT];
} TGuiVideoPlayerActiveMsg;

typedef TGuiVideoPlayerActiveMsg TGuiVideoPlayerActiveReq;

#define GUI_VIDEO_PLAYER_ACTIVE_OK	0		//初始化成功
#define GUI_VIDEO_PLAYER_ACTIVE_ERR	1		//初始化失败
typedef struct									//播放器激活初始化结果回应
{
	int m_iRetCode;
} TGuiVideoPlayerActiveRsp;

typedef struct									//播放倍速命令
{
	int m_iPlayId;								//播放编号
	int m_iSpeed;								//正n表示当前倍速xn倍速, -n表示/n倍速
} TGuiVideoPlayChgSpeedCmd;

typedef struct									//快放/慢放命令
{
	int m_iPlayId;								//播放编号
	int m_iSpeed;								//正n表示n倍速播放,-n表示1/n倍速播放(1, 0, -1均表示正常速播放)
} TGuiVideoPlaySpeedCmd;

typedef struct									//全屏播放请求
{
	int m_iVoId;								//画面号(1-4)
} TGuiVideoPlayFullScrReq;

#define GUI_VIDEO_PLAY_FULL_SCR_OK		0		//全屏播放成功
#define GUI_VIDEO_PLAY_FULL_SCR_ERR		1		//全屏播放失败
typedef struct									//全屏播放回应
{
	int m_iRetCode;
} TGuiVideoPlayFullScrRsp;

typedef struct									//退出全屏播放请求
{
	int m_iVoId;								//画面号(1-4)
} TGuiVideoPlayFullScrExitReq;

#define GUI_VIDEO_PLAY_FULL_SCR_EXIT_OK		0		//退出全屏播放成功
#define GUI_VIDEO_PLAY_FULL_SCR_EXIT_ERR	1		//退出全屏播放失败
typedef struct									//退出全屏播放回应
{
	int m_iRetCode;
} TGuiVideoPlayFullScrExitRsp;

typedef struct									//设置回放声音请求
{
	int m_iActiveVoId;		//当前激活的声音输出通道，1-4表示切换，其它值无操作
	int m_iSoundEnFlg;		// 1 - 4    :　使能指定通道的声音输出
							// - 1 - -4 :  关闭指定通道的声音输出(vo通道号取绝对值)
} TGuiVideoPlaySetSoundReq;

#define	GUI_VPLAY_SET_SOUND_OK		0		//设置回放声音成功
#define GUI_VPLAY_SET_SOUND_ERR		1		//设置回放声音失败
typedef struct									//设置回放声音回应
{
	int m_iRetCode;		//返回码，GUI_VPLAY_SET_SOUND_OK/ERR
} TGuiVideoPlaySetSoundRsp;

//同步回放
#define GUI_VIDEO_SYNC_PLAYER_ACTIVE_OK	0		//初始化成功
#define GUI_VIDEO_SYNC_PLAYER_ACTIVE_ERR	1		//初始化失败

#define GUI_SYNC_PLAY_SOUND_ENABLE 1
#define GUI_SYNC_PLAY_SOUND_DISABLE 0

typedef struct									//录像文件查询条件
{
	int m_iChn[4];									//通道号
	int m_iDev;									//设备号
	int m_iBeginTm;							//开始时间
	int m_iEndTm;								//结束时间
	int m_iSoundEnable[4];
} TGuiVideoFileQuerySyncCondition;


typedef struct
{
	int m_iCmd;									//命令码
	int m_iLength;								//pdu负载长度
	char m_cPayload[0];						//实际数据
} TGuiSyncParamPdu;								//数据传输单元结构

typedef struct									//播放器激活初始化消息
{
	int m_iDevId;								//设备号
	int m_iVoCnt;								//播放器VO画面数
	// 2008-12-17 dxl 添加Vo属性
	TGuiVoAttr m_tVoAttrs[GUI_PLAYER_MAX_VO_CNT];
} TGuiVideoSyncPlayerActiveMsg;
/********************************************/
//新同步回放
#define GUI_SYNC_PLAYER_VIDEO_FILE_REQ 	368			//同步请求播放文件
#define GUI_SYNC_PLAYER_VIDEO_FILE_RSP  -368		//同步播放文件请求回应

#define GUI_SYNC_PLAYER_MAX_CNT   16
typedef struct									//录像文件查询条件
{
	int m_iChn[GUI_SYNC_PLAYER_MAX_CNT];	//通道号高16位0-主码流1-副码流
	int m_iDev;								//设备号
	int m_iBeginTm;							//开始时间
	int m_iEndTm;							//结束时间
	int m_iSoundEnable[GUI_SYNC_PLAYER_MAX_CNT];
} TGuiSyncPlayerVideoQueryConditionReq;

typedef struct
{
	int m_iRetCode;
}TGuiSyncPlayerVideoQueryConditionRsp;

#define GUI_VIDEO_SYNC_PLAYER_COORDIRATE_REQ 369	//发送坐标,播放器初始化请求
#define GUI_VIDEO_SYNC_PLAYER_COORDIRATE_RSP -369	//播放器初始化请求回应

typedef struct									//播放器激活初始化消息
{
	int m_iDevId;								//设备号
	int m_iVoCnt;								//播放器VO画面数
	// 2008-12-17 dxl 添加Vo属性
	TGuiVoAttr m_tVoAttrs[GUI_SYNC_PLAYER_MAX_CNT];
} TGuiSyncPlayerActiveMsgReq;
typedef struct
{
	int m_iRetCode;
}TGuiSyncPlayerActiveMsgRsp;



typedef struct									//播放器激活初始化结果回应
{
	int m_iRetCode;
} TGuiVideoSyncPlayerActiveRsp;

typedef struct									//播放定位请求(无回应)
{
	int m_iLoc;									//定位百分比(0-100)
} TGuiVideoSyncPlayLocCmd;
typedef struct									//播放倍速命令
{
	int m_iSpeed;								//正n表示当前倍速xn倍速, -n表示/n倍速
} TGuiVideoSyncPlayChgSpeedCmd;

typedef struct									//播放倍速命令
{
	int m_iSpeed;								//正n表示当前倍速xn倍速, -n表示/n倍速
} TGuiVideoSyncPlaySpeedCmd;


typedef struct									//获取当前回放状态回应
{
	int m_iCurPlayStat;						//当前播放状态
	int m_iPlayRate;							//当前播放的进度(0-100000)
	int m_iSpeed;								//当前播放倍数(正表加速，负表减速, 0表单步)
} TGuiGetVideoSyncPlayStatRsp;

typedef struct									//全屏播放请求
{
	int m_iVoId;								//画面号(1-4)
} TGuiVideoSyncPlayFullScrReq;

#define GUI_VIDEO_SYNC_PLAY_FULL_SCR_OK	0		//全屏播放成功
#define GUI_VIDEO_SYNC_PLAY_FULL_SCR_ERR	1		//全屏播放失败
typedef struct									//全屏播放回应
{
	int m_iRetCode;
} TGuiVideoSyncPlayFullScrRsp;

#define GUI_VIDEO_SYNC_PLAY_FULL_SCR_EXIT_OK		0		//退出全屏播放成功
#define GUI_VIDEO_SYNC_PLAY_FULL_SCR_EXIT_ERR	1		//退出全屏播放失败
typedef struct									//退出全屏播放回应
{
	int m_iRetCode;
} TGuiVideoSyncPlayFullScrExitRsp;

typedef struct									//退出全屏播放请求
{
	int m_iVoId;								//画面号(1-4)
} TGuiVideoSyncPlayFullScrExitReq;

typedef struct									//播放回应
{
	int m_iRetCode;
} TGuiVideoSyncPlayVideoRsp;

typedef struct									//设置回放声音请求
{
	int m_iActiveVoId;		//当前激活的声音输出通道，1-4表示切换，其它值无操作
	int m_iSoundEnFlg;		// 1 - 4    :　使能指定通道的声音输出
							// - 1 - -4 :  关闭指定通道的声音输出(vo通道号取绝对值)
} TGuiVideoSyncPlaySetSoundReq;

#define	GUI_SYNC_VPLAY_SET_SOUND_OK		0		//设置回放声音成功
#define GUI_SYNC_VPLAY_SET_SOUND_ERR		1		//设置回放声音失败

typedef struct									//设置回放声音回应
{
	int m_iRetCode;		//返回码，GUI_VPLAY_SET_SOUND_OK/ERR
} TGuiVideoSyncPlaySetSoundRsp;

#define GUI_SYNC_PLAY_VIDEO_REVERSE_REQ   367 //同步回放倒放请求


// 抓拍浏览

// 首先，UI向主程序发送jpeg浏览初始化请求(GUI_JPEG_VIEWER_INIT_REQ)
// 主程序接到请求初始化资源并回应GUI_JPEG_VIEWER_INIT_ESP
typedef struct
{
	int m_iDevId;		//设备号
} TGuiJpegViewerInitReq;

#define GUI_JPEG_VIEWER_INIT_OK		0		//jpeg浏览初始化成功
#define GUI_JPEG_VIEWER_INIT_ERR		1		//jpeg浏览初始化失败
typedef struct									//jpeg浏览初始化回应
{
	int m_iRetCode;
} TGuiJpegViewerInitRsp;

// 初始化成功后，UI随时可向主程序发送若干jpeg显示请求
typedef struct
{
	char m_strJpegFile[GUI_VF_NAME_LEN + 1];
} TGuiJpegDspReq;

#define GUI_JPEG_DSP_OK				0		//jpeg显示成功
#define GUI_JPEG_DSP_ERR				1		//jpeg显示失败
typedef struct
{
	int m_iRetCode;
} TGuiJpegDspRsp;

// 最后，UI通知主程序结束jpeg浏览(GUI_JPEG_VIEWER_EXIT_MSG)，
// 主程序应当释放jpeg显示相关的资源，并
// 将dvr恢复到初始化jpeg浏览前的状态。


//==============================云台控制=================================
//-----------------------------解码器设置--------------------------------


typedef struct									//获取解码器参数请求
{
	int m_iChn;									//通道号
} TGuiGetPtzDecodeCnfReq;

#define GUI_PTZ_DEC_PRO_NAME_LEN		31		//解码器协议名长度

typedef struct
{
	int m_iAddr;								//地址
	#if 1
	char m_strProtocol[GUI_PTZ_DEC_PRO_NAME_LEN + 1]; //协议名
	#else
	int m_iProtocol;							//协议编号
	#endif
	int m_iSportId;							//串口编号
	int m_iComAttrEn;							//自定义串口属性使能标识
	int m_iBitRate;							//波特率
	int m_iDataBits;							//数据位
	int m_iStopBits;							//停止位
	int m_iCheckType;							//校验位	
} TGuiPtzDecodeCnf;

typedef struct									//获取解码器参数回应
{
	int m_iChn;									//通道号
	TGuiPtzDecodeCnf m_tCnf;					//配置信息
} TGuiGetPtzDecodeCnfRsp;

typedef struct									//设置解码器参数请求
{
	int m_iChn;									//通道号
	TGuiPtzDecodeCnf m_tCnf;					//配置信息
} TGuiSetPtzDecodeCnfReq;

#define GUI_SET_PTZ_DECODE_CNF_OK	0		//设置参数成功
#define GUI_SET_PTZ_DECODE_CNF_ERR	1		//设置参数失败

typedef struct									//设置解码器参数回应
{
	int m_iRetCode;							//返回码
} TGuiSetPtzDecodeCnfRsp;

typedef struct 								//获取协议列表回应
{
	int m_iProtocolCnt;							//协议总数
} TGuiGetProtocolListRsp;

typedef struct 								//协议列表项
{
	#if 0
	int m_iProtocolId; 							//协议唯一编号
	#endif
	int m_iExistProtocol;      //1为已有协议，0为待添加的协议
	char m_strProtocolName[GUI_PTZ_DEC_PRO_NAME_LEN + 1]; //协议名
} TGuiProtocolListItem;

typedef struct 								//获取协议列表回应
{
	int m_iProtocolCnt;							//协议总数
} TGuiSetProtocolListReq;

//------------------------------云台控制---------------------------------

typedef struct									//获取预置位列表请求
{
	int m_iChn;									//通道编号
} TGuiGetPtzPresetPointListReq;

typedef struct									//获取预置位列表回应
{
	int m_iChn;									//通道编号
	int m_iCnt;									//预置位个数
} TGuiGetPtzPresetPointListRsp;
//随后连续发送各个预置点的编号,用整形表式


#define GUI_PTZ_MAX_POINT_PER_ROUTE		32	//每条巡航路径中最大包含的巡航点数
#define GUI_PTZ_MAX_ROUTE_PER_CHN		8	//每个通道最多可设的巡航路径数

typedef struct									//巡航点
{
	int m_iPointId;							//预置位编号
	int m_iSpeed;								//巡航速度
	int m_iDelay;								//停留时间(单位:s)
} TGuiPtzCruiseRoutePoint;

typedef struct									//巡航路径
{
	int m_iEnable;								//使能标识(1使能/0未使能)
	int m_iPointCnt;							//巡航点数
	TGuiPtzCruiseRoutePoint m_tPoints[GUI_PTZ_MAX_POINT_PER_ROUTE]; //巡航点
} TGuiPtzCruiseRoute;

typedef struct									//获取巡航路径请求
{
	int m_iChn;									//通道号
} TGuiGetPtzCruiseRouteReq;

typedef struct									//获取巡航路径回应
{
	int m_iChn;									//通道号(0表示获取失败)
	TGuiPtzCruiseRoute m_tRoutes[GUI_PTZ_MAX_ROUTE_PER_CHN]; //通道对应的巡航路径
} TGuiGetPtzCruiseRouteRsp;

typedef struct									//设置巡航路径请求
{
	int m_iChn;									//通道号
	TGuiPtzCruiseRoute m_tRoutes[GUI_PTZ_MAX_ROUTE_PER_CHN]; //通道对应的巡航路径
} TGuiSetPtzCruiseRouteReq;

#define GUI_SET_PTZ_CRUISROUTE_OK	0		//设置成功
#define GUI_SET_PTZ_CRUISROUTE_ERR	1		//设置失败
typedef struct									//设置巡航路径回应
{
	int m_iRetCode;							//返回码
} TGuiSetPtzCruiseRouteRsp;

//注意，UI的预置位编号从1开始，0表示特殊值
typedef struct									//添加预置位请求
{
	int m_iChn;									//通道编号
	int m_iPointId;							//预置点编号(0表式添加新预置位,
												//其它值表式在指定编号处更新预置位信息)
} TGuiAddPtzPresetPointReq;

typedef struct									//添加预置位回应
{
	int m_iChn;									//0表示添加预置位失败
	int m_iPointId;							//新加的预置位编号
} TGuiAddPtzPresetPointRsp;

typedef struct									//删除预置位请求
{
	int m_iChn;
	int m_iPointId;
} TGuiDelPtzPresetPointReq;

#define GUI_DEL_PTZ_PRESETPOINT_OK		0	//删除预置位成功
#define GUI_DEL_PTZ_PRESETPOINT_ERR		1	//删除预置位失败
typedef struct									//删除预置位回应
{
	int m_iRetCode;							//返回码
} TGuiDelPtzPresetPointRsp;

typedef struct									//调用预置位请求
{
	int m_iChn;									//通道编号
	int m_iPointId;							//预置位编号
} TGuiTurnToPtzPresetPointReq;

#define GUI_TRN_PTZ_PRESETPOINT_OK		0	//调用预置位成功
#define GUI_TRN_PTZ_PRESETPOINT_ERR		1	//调用预置位失败
typedef struct									//调用预置位回应
{
	int m_iRetCode;							//返回码
} TGuiTurnToPtzPresetPointRsp;

// 云台控制本地控制码:
// 1. 方向控制, m_iParam[0]存放速度(双方向先水平后竖直速度)
#define GUI_PTZ_LOCAL_CODE_UP				1
#define GUI_PTZ_LOCAL_CODE_DOWN			2
#define GUI_PTZ_LOCAL_CODE_LEFT			3
#define GUI_PTZ_LOCAL_CODE_RIGHT			4
#define GUI_PTZ_LOCAL_CODE_RIGHTUP		5
#define GUI_PTZ_LOCAL_CODE_LEFTUP		6
#define GUI_PTZ_LOCAL_CODE_RIGHTDOWN		7
#define GUI_PTZ_LOCAL_CODE_LEFTDOWN		8
#define GUI_PTZ_LOCAL_CODE_STOP			9

// 2. 变焦、变倍、光圈调节
#define GUI_PTZ_LOCAL_CODE_ZOOM_ASC		10	//变倍+
#define GUI_PTZ_LOCAL_CODE_ZOOM_DEC		11	//变倍-
#define GUI_PTZ_LOCAL_CODE_ZOOM_STOP		12	//变倍stop
#define GUI_PTZ_LOCAL_CODE_FOCUS_ASC		13	//变焦+
#define GUI_PTZ_LOCAL_CODE_FOCUS_DEC		14	//变焦-
#define GUI_PTZ_LOCAL_CODE_FOCUS_STOP	15	//变焦stop
#define GUI_PTZ_LOCAL_CODE_IRIS_ASC		17	//光圈+
#define GUI_PTZ_LOCAL_CODE_IRIS_DEC		18	//光圈-

// 3. 自动扫描
#define GUI_PTZ_LOCAL_CODE_AUTO_START	23
#define GUI_PTZ_LOCAL_CODE_AUTO_STOP		24

// 灯光、雨刷、辅助
#define GUI_PTZ_LOCAL_CODE_LIGHT_ON		21
#define GUI_PTZ_LOCAL_CODE_LIGHT_OFF		22
#define GUI_PTZ_LOCAL_CODE_WIPE_START	19
#define GUI_PTZ_LOCAL_CODE_WIPE_STOP		20
#define GUI_PTZ_LOCAL_CODE_ASS_ON		29
#define GUI_PTZ_LOCAL_CODE_ASS_OFF		30

// 4. 预置位
#define GUI_PTZ_LOCAL_CODE_GO_PRESET		25	//转到预置位
#define GUI_PTZ_LOCAL_CODE_SET_PRESET	28	//设置预置位

// 5. 轨迹

typedef struct									//云台控制本地控制码
{
	int m_iChn;									//通道号
	int m_iCode;								//控制码值
	int m_iParam[4];							//相关参数
} TGuiPtzLocalCode;

typedef struct									//开始记录轨迹请求
{
	int m_iChn;									//通道号
} TGuiStartPtzTrackReq;

#define GUI_PTZ_TRACK_START_OK			0	//开始记录轨迹成功
#define GUI_PTZ_TRACK_START_ERR			1	//开始记录轨迹失败
typedef struct									//开始记录轨迹回应
{
	int m_iRetCode;
} TGuiStartPtzTrackRsp;

typedef struct									//结束记录轨迹请求
{
	int m_iChn;
} TGuiEndPtzTrackReq;

#define GUI_PTZ_TRACK_END_OK				0	//结束记录轨迹成功
#define GUI_PTZ_TRACK_END_ERR				1	//结束记录轨迹失败
typedef struct									//结束记录轨迹回应
{
	int m_iRetCode;
} TGuiEndPtzTrackRsp;

typedef struct									//开始演示轨迹请求
{
	int m_iChn;									//通道号
} TGuiStartPtzTrackPlayReq;

#define GUI_PTZ_TRACK_PLAY_START_OK		0	//开始演示轨迹成功
#define GUI_PTZ_TRACK_PLAY_START_ERR		1	//开始演示轨迹失败
typedef struct									//开始演示轨迹回应
{
	int m_iRetCode;
} TGuiStartPtzTrackPlayRsp;

typedef struct									//结束演示轨迹请求
{
	int m_iChn;
} TGuiEndPtzTrackPlayReq;

#define GUI_PTZ_TRACK_PLAY_END_OK		0	//结束演示轨迹成功
#define GUI_PTZ_TRACK_PLAY_END_ERR		1	//结束演示轨迹失败
typedef struct									//结束演示轨迹回应
{
	int m_iRetCode;
} TGuiEndPtzTrackPlayRsp;

typedef struct									// 3d定位
{
	int m_iChn;				//通道号
	int m_iMxp;				//鼠标位置xp
	int m_iMyp;				//鼠标位置yp
	int m_iScrWidth;		//屏幕宽
	int m_iScrHeight;		//屏幕高
	int m_iWidth;			//区域宽(可正负，负表反方向, 0表点)
	int m_iHeight;			//区域高(可正负，负表反方向, 0表点)
} TGuiPtz3DLocMsg;


//==============================报警设置=================================
//------------------------------报警输入---------------------------------
typedef struct									//时间段
{
	int m_iEnable;								//时段使能标识
	int m_iBeginTm;							//开始时间，以当天流过的秒计
	int m_iEndTm;								//结束时间
} TGuiTimePair;

#define GUI_ALARM_TIME_PER_DAY	4			//每天可设时间段个数
#define GUI_ALARM_OPEN_TYPE		1			//常开报警
#define GUI_ALARM_CLOSE_TYPE		2			//常闭报警
#define GUI_DVR_ALARM_OUT_CNT		4			//报警输出通道个数

typedef struct									//ptz联动
{
	int m_iPreSetPointEnable;					//预置点使能
	int m_iPreSetPointId;						//预置点编号
	int m_iCruisePathEnable;					//巡航路径使能
	int m_iCruisePathId;						//巡航路径编号
	int m_iTrackEnable;						//轨迹使能
} TGuiPtzLinkage;

typedef struct									//报警输入配置
{
	int m_iAlarmType;							//报警器类型，常开/常闭
	int m_iEnable;								//报警处理使能 0/1
	int m_iSoundEnable;						//声音使能 0/1 ，次低字节(bit8~bit15)表示是否联动邮件报警0/1
	int m_iDisplayEnable;						//屏幕显示使能低字节 0/1, 次低字节表示使能联动那个画面
	int m_iTranToServerEnable;                  //上传中心使能 0/1
	int m_iVideoEnable[GUI_DVR_CHN_CNT];		//联动各通道录像使能 0/1, (0/1 按位顺序操作，数组元素第1位分别对应1,2,3,4,5….通道，依次循环，其他也按照此处理)
	int m_iPicEnable[GUI_DVR_CHN_CNT];			//联动各通道图片抓拍使能
	int m_iAlarmOutEnable[GUI_DVR_ALARM_OUT_CNT];			//联动报警输出使能 0/1 按位顺序操作，数组元素第1位分别对应1,2,3,4通道，依次循环
	TGuiTimePair m_tTimes[7][GUI_ALARM_TIME_PER_DAY];		//每天四个时段，一周7天
	TGuiPtzLinkage m_tPtzLinkage[GUI_DVR_CHN_CNT];			//ptz联动
} TGuiAlarmInCnf;

typedef struct									//获取报警输入参数请求
{
	int m_iPortId;								//报警输入端口编号1-16
} TGuiGetAlarmInCnfReq;

typedef struct									//获取报警输入参数回应
{
	int m_iPortId;								//端口号
	TGuiAlarmInCnf m_tCnf;						//参数配置
} TGuiGetAlarmInCnfRsp;

typedef struct									//设置报警输入参数请求
{
	int m_iPortId;								//端口号
	TGuiAlarmInCnf m_tCnf;						//参数配置
} TGuiSetAlarmInCnfReq;

#define GUI_SET_ALARM_IN_CNF_OK		0		//设置参数成功
#define GUI_SET_ALARM_IN_CNF_ERR		1		//设置参数失败
typedef struct									//设置报警输入参数回应
{
	int m_iRetCode;
} TGuiSetAlarmInCnfRsp;

//大路数报警联动ptz,前32通道使用老协议
#define GUI_GET_ALARM_LINK_PTZ_REQ    2510
#define GUI_GET_ALARM_LINK_PTZ_RSP	-2510
typedef struct
{
	int m_iChn;
	int m_iAlarmType;   //报警类型见guitransport.h
	int m_iChnOffSet;   
}TGuiGetAlarmLinkPtzReq;

typedef struct
{
	int m_iChn;
	int m_iAlarmType;
	int m_iChnOffSet;
	TGuiPtzLinkage m_tPtzLinkage[GUI_DVR_CHN_CNT];			//ptz联动
}TGuiGetAlarmLinkPtzRsp;

#define GUI_SET_ALARM_LINK_PTZ_REQ	2511
#define GUI_SET_ALARM_LINK_PTZ_RSP	-2511
typedef TGuiGetAlarmLinkPtzRsp  TGuiSetAlarmLinkPtzReq;

typedef struct
{
	int m_iRetCode;
}TGuiSetAlarmLinkPtzRsp;


//------------------------------报警输出---------------------------------
typedef struct									//报警输出参数
{
	int m_iMsgDelay;							//信号延时
	int m_iType;								//报警器类型:常开/常闭
	TGuiTimePair m_tTimes[7][GUI_ALARM_TIME_PER_DAY]; //布防时间,7天，每天四个时段
} TGuiAlarmOutCnf;

typedef struct									//获取报警输出参数请求
{
	int m_iPortId;								//报警输出通道
} TGuiGetAlarmOutCnfReq;

typedef struct									//获取报警输出参数请求
{
	int m_iPortId;								//报警输出通道号
	TGuiAlarmOutCnf m_tCnf;
} TGuiGetAlarmOutCnfRsp;

typedef struct									//设置报警输出参数请求
{
	int m_iPortId;								//报警输出通道号
	TGuiAlarmOutCnf m_tCnf;
} TGuiSetAlarmOutCnfReq;

#define GUI_SET_ALARM_OUT_CNF_OK		0		//设置报警输入参数成功
#define GUI_SET_ALARM_OUT_CNF_ERR	1		//设置报警输入参数失败

typedef struct									//设置报警输出参数回应
{
	int m_iRetCode;							//返回码
} TGuiSetAlarmOutCnfRsp;

// 手动消警

#define GUI_ALARM_ALL					0
// 其它类型见GuiTransport

typedef struct									//消除报警命令
{
	int m_iType;
} TGuiClearAlarmCmd;
//----------------------------报警联动单画面-------------------------
#define GUI_GET_ALARM_LINK_SPIC_INTERVAL_REQ   541 //获取报警联动单画面巡航间隔时间请求
#define GUI_GET_ALARM_LINK_SPIC_INTERVAL_RSP   -541   //获取报警联动单画面巡航间隔时间回应
typedef struct
{
	int m_iTm;
}TGuiGetAlarmLinkSpicIntervalRsp;


#define GUI_SET_ALARM_LINK_SPIC_INTERVAL_REQ  542  //设置报警联动单画面巡航间隔请求
#define GUI_SET_ALRRM_LINK_SPIC_INTERVAL_RSP  -542 //设置报警联动单画面巡航间隔回应
typedef struct
{
	int m_iTm; // 0- 不巡航，其他时间均巡航
}TGuiSetAlarmLinkSpicIntervalReq;
#define GUI_SET_ALARM_LINK_OK    0 //设置成功
#define GUI_SET_ALARM_LINK_ERR   1 //设置失败
typedef struct
{
	int m_iRetCode;   
}TGuiSetAlarmLinkSpicIntervalRsp;


//------------------------------移动侦测---------------------------------
typedef struct									//获取移动侦测报警参数请求
{
	int m_iChn;									//通道号
} TGuiGetMdAlarmCnfReq;

typedef struct									//移动侦测报警配置
{
	int m_iSensitive;							//灵敏度
	int m_iEnable;								//移动侦测使能 0/1
	int m_iSoundEnable;						//声音使能 0/1 ，次低字节(bit8~bit15)表示是否联动邮件报警0/1
	int m_iDisplayEnable;						//屏幕显示使能 0/1,屏幕显示使能低字节 0/1, 次低字节表示使能联动那个画面,
	//bit16~bit17表示联动双灯，bit16 1红外灯启用 0红外灯不启用，bit17-1白光灯启用 0-白光灯不启用
	int m_iToServerEnable;
	int m_iVideoEnable[GUI_DVR_CHN_CNT];		//联动各通道录像使能 0/1, (0/1 按位顺序操作，数组元素第1位分别对应1,2,3,4,5….通道，依次循环，其他也按照此处理)
	int m_iPicEnable[GUI_DVR_CHN_CNT];			//联动各通道图片抓拍使能
	int m_iAlarmOutEnable[GUI_DVR_ALARM_OUT_CNT];			//联动报警输出使能 0/1
	TGuiTimePair m_tTimes[7][GUI_ALARM_TIME_PER_DAY];		//每天四个时段，一周7天
	char m_cAreas[GUI_DVR_SCR_ROWS][GUI_DVR_SCR_COLS];		//移动侦测区域，22x18小格，0/1
} TGuiMdAlarmCnf;

typedef struct									//获取移动侦测报警参数回应
{
	int m_iChn;									//通道号
	TGuiMdAlarmCnf m_tCnf;						//参数配置表
} TGuiGetMdAlarmCnfRsp;						//获取移动侦测报警参数

typedef struct									//设置移动侦测报警参数请求
{
	int m_iChn;									//通道号
	TGuiMdAlarmCnf m_tCnf;						//参数配置表
} TGuiSetMdAlarmCnfReq;						//获取移动侦测报警参数

#define GUI_SET_MD_ALARM_CNF_OK		0		//设置参数成功
#define GUI_SET_MD_ALARM_CNF_ERR		1		//设置参数失败
typedef struct									//设置移动侦测报警参数回应
{
	int m_iRetCode;
} TGuiSetMdAlarmCnfRsp;


//------------------------------视频丢失---------------------------------
typedef struct									//获取视频丢失报警参数请求
{
	int m_iChn;									//通道号
} TGuiGetVideoLostAlarmCnfReq;

typedef struct									//视频丢失报警配置
{
	int m_iEnable;								//移动侦测使能 0/1
	int m_iSoundEnable;							//声音使能 0/1 ，次低字节(bit8~bit15)表示是否联动邮件报警0/1
	int m_iDisplayEnable;						//屏幕显示使能 0/1
	int m_iToServerEnalbe;
	int m_iVideoEnable[GUI_DVR_CHN_CNT];		//联动各通道录像使能 0/1, (0/1 按位顺序操作，数组元素第1位分别对应1,2,3,4,5….通道，依次循环，其他也按照此处理)
	int m_iPicEnable[GUI_DVR_CHN_CNT];			//联动各通道图片抓拍使能
	int m_iAlarmOutEnable[GUI_DVR_ALARM_OUT_CNT];			//联动报警输出使能 0/1
	TGuiTimePair m_tTimes[7][GUI_ALARM_TIME_PER_DAY];		//每天四个时段，一周7天
	TGuiPtzLinkage m_tPtzLinkage[GUI_DVR_CHN_CNT];			//ptz联动
} TGuiVideoLostAlarmCnf;

typedef struct									//获取视频丢失报警参数回应
{
	int m_iChn;									//通道号
	TGuiVideoLostAlarmCnf m_tCnf;				//参数配置表
} TGuiGetVideoLostAlarmCnfRsp;

typedef struct									//设置视频丢失报警参数请求
{
	int m_iChn;									//通道号
	TGuiVideoLostAlarmCnf m_tCnf;				//参数配置表
} TGuiSetVideoLostAlarmCnfReq;

#define GUI_SET_VL_ALARM_CNF_OK		0		//设置参数成功
#define GUI_SET_VL_ALARM_CNF_ERR		1		//设置参数失败
typedef struct									//设置移动侦测报警参数回应
{
	int m_iRetCode;
} TGuiSetVideoLostAlarmCnfRsp;
//===========================音频丢失报警==================================
#define GUI_GET_AL_ALARM_CNF_REQ		532		//设置音频丢失报警参数请求
#define GUI_GET_AL_ALARM_CNF_RSP		-532	//设置音频丢失报警参数回应

typedef struct									//移动侦测报警配置
{
	int m_iEnable;								//移动侦测使能 0/1
	int m_iSoundEnable;						//声音使能 0/1 ，次低字节(bit8~bit15)表示是否联动邮件报警0/1
	int m_iDisplayEnable;						//屏幕显示使能 0/1
	int m_iToServerEnalbe;
	int m_iVideoEnable[GUI_DVR_CHN_CNT];		//联动各通道录像使能 0/1, (0/1 按位顺序操作，数组元素第1位分别对应1,2,3,4,5….通道，依次循环，其他也按照此处理)
	int m_iPicEnable[GUI_DVR_CHN_CNT];			//联动各通道图片抓拍使能
	int m_iAlarmOutEnable[GUI_DVR_ALARM_OUT_CNT];			//联动报警输出使能 0/1
	TGuiTimePair m_tTimes[7][GUI_ALARM_TIME_PER_DAY];		//每天四个时段，一周7天
	TGuiPtzLinkage m_tPtzLinkage[GUI_DVR_CHN_CNT];			//ptz联动
} TGuiAudioLostAlarmCnf;

typedef struct									//获取音频丢失报警参数请求
{
	int m_iChn;									//通道号
} TGuiGetAudioLostAlarmCnfReq;

typedef struct									//获取视频丢失报警参数回应
{
	int m_iChn;									//通道号
	TGuiAudioLostAlarmCnf m_tCnf;				//参数配置表
} TGuiGetAudioLostAlarmCnfRsp;


#define GUI_SET_AL_ALARM_CNF_REQ		533		//设置音频丢失报警参数请求
#define GUI_SET_AL_ALARM_CNF_RSP		-533	//设置音频丢失报警参数回应

typedef struct									//设置视频丢失报警参数请求
{
	int m_iChn;									//通道号
	TGuiAudioLostAlarmCnf m_tCnf;				//参数配置表
} TGuiSetAudioLostAlarmCnfReq;

#define GUI_SET_AL_ALARM_CNF_OK			0		//设置参数成功
#define GUI_SET_AL_ALARM_CNF_ERR		1		//设置参数失败

typedef struct									//设置移动侦测报警参数回应
{
	int m_iRetCode;
} TGuiSetAudioLostAlarmCnfRsp;

//=========================温湿度报警==========================
//行嵌无人值守主机
#define GUI_GET_TEMPHUM_ALARM_CNF_REQ		534		//获取温湿度报警参数请求
#define GUI_GET_TEMPHUM_ALARM_CNF_RSP		-534	//获取温湿度报警参数回应

typedef struct	//报警输入配置
{
	int m_iEnable;									//报警处理使能 0/1
	int m_iSoundEnable;								//声音使能 0/1 ，次低字节(bit8~bit15)表示是否联动邮件报警0/1
	int m_iDisplayEnable;							//屏幕显示使能低字节 0/1 次第字节表示使能联动哪个画面
	int m_iToServerEnalbe;                  		//上传中心使能 0/1
	int m_iVideoEnable[GUI_DVR_CHN_CNT];			//联动各通道录像使能 0/1, (0/1 按位顺序操作，数组元素第1位分别对应1,2,3,4,5….通道，依次循环，其他也按照此处理)
	int m_iPicEnable[GUI_DVR_CHN_CNT];				//联动各通道图片抓拍使能
	int m_iAlarmOutEnable[GUI_DVR_ALARM_OUT_CNT];		//联动报警输出使能 0/1 按位顺序操作，数组元素第1位分别对应1,2,3,4通道，依次循环
	TGuiTimePair m_tTimes[7][GUI_ALARM_TIME_PER_DAY];	//每天四个时段，一周7天
	TGuiPtzLinkage m_tPtzLinkage[GUI_DVR_CHN_CNT];		//ptz联动
} TGuiTempHumAlarmCnf;

typedef struct											//获取温湿度报警参数请求
{
	int m_iDevId;										//温湿度模块编号
} TGuiGetTempHumAlarmCnfReq;

typedef struct											//温湿度模块编号
{
	int m_iDevId;										//温湿度模块编号
	TGuiTempHumAlarmCnf m_tCnf;							//参数配置
} TGuiGetTempHumAlarmCnfRsp;

#define GUI_SET_TEMPHUM_ALARM_CNF_REQ		535			//设置温湿度报警参数请求
#define GUI_SET_TEMPHUM_ALARM_CNF_RSP		-535		//设置温湿度报警参数回应

#define GUI_SET_ALARM_TEMPHUM_CNF_OK		0			//设置参数成功
#define GUI_SET_ALARM_TEMPHUM_CNF_ERR		1			//设置参数失败

typedef struct											//设置温湿度报警参数请求
{
	int m_iDevId;										//温湿度模块地址
	TGuiTempHumAlarmCnf m_tCnf;							//参数配置
} TGuiSetTempHumAlarmCnfReq;

typedef struct											//设置温湿度报警参数回应
{
	int m_iRetCode;
} TGuiSetTempHumAlarmCnfRsp;

//==========================视频遮挡报警=============================
#define GUI_GET_OD_ALARM_CNF_REQ	  536  		//获取视频遮挡报警参数请求
#define GUI_GET_OD_ALARM_CNF_RSP	  -536		//获取视频遮挡报警参数回应
#define	GUI_SET_OD_ALARM_CNF_REQ      537	    //设置视频遮挡报警参数请求
#define	GUI_SET_OD_ALARM_CNF_RSP   	  -537		//设置视频遮挡报警参数回应

typedef struct							//获取视频遮挡报警参数请求
{
	int m_iChn;							//通道号
} TGuiGetODAlarmCnfReq;

typedef struct										//视频遮挡报警配置
{
	int m_iSensitive;								//灵敏度
	int m_iEnable;									//移动侦测使能 0/1
	int m_iSoundEnable;								//声音使能 0/1
	int m_iDisplayEnable;							//屏幕显示使能 0/1
	int m_iToServerEnable;
	int m_iVideoEnable[GUI_DVR_CHN_CNT];			//联动各通道录像使能 0/1, (0/1 按位顺序操作，数组元素第1位分别对应1,2,3,4,5….通道，依次循环，其他也按照此处理)
	int m_iPicEnable[GUI_DVR_CHN_CNT];				//联动各通道图片抓拍使能
	int m_iAlarmOutEnable[GUI_DVR_ALARM_OUT_CNT];	//联动报警输出使能 0/1
	TGuiTimePair m_tTimes[7][GUI_ALARM_TIME_PER_DAY];		//每天四个时段，一周7天
	TGuiPtzLinkage m_tPtzLinkage[GUI_DVR_CHN_CNT];			//联动ptz
	char m_cAreas[GUI_DVR_SCR_ROWS][GUI_DVR_SCR_COLS];		//视频遮挡区域，22x18小格，0/1
} TGuiODAlarmCnf;

typedef struct							//获取视频遮挡报警参数回应
{
	int m_iChn;							//通道号
	TGuiODAlarmCnf m_tCnf;				//参数配置表
} TGuiGetODAlarmCnfRsp;				//获取视频遮挡报警参数

typedef struct							//设置视频遮挡报警参数请求
{
	int m_iChn;							//通道号
	TGuiODAlarmCnf m_tCnf;				//参数配置表
} TGuiSetODAlarmCnfReq;				//获取视频遮挡报警参数

#define GUI_SET_OD_ALARM_CNF_OK			0			//设置参数成功
#define GUI_SET_OD_ALARM_CNF_ERR		1			//设置参数失败
typedef struct								
{
	int m_iRetCode;
} TGuiSetODAlarmCnfRsp;			//设置视频遮挡报警参数回应


// ===========================系统注销==================================
#define GUI_SYS_SPEC_CODE1			0x20090624
#define GUI_SYS_SPEC_CODE2			0x06242009   //本地
#define GUI_SYS_SPEC_CODE3          0x20130425   //远程

typedef struct
{
	int m_iCode1;								//固定值 GUI_SYS_SPEC_CODE1
	int m_iCode2;								//固定值 GUI_SYS_SPEC_CODE2	
} TGuiSystemReSetReq;

#define GUI_SYS_RESET_ACCEPT			0		//允许重启
#define GUI_SYS_RESET_REFUSE			1		//拒绝重启
typedef struct									//系统重启回应
{
	int m_iRetCode;
} TGuiSystemReSetRsp;


typedef struct
{
	int m_iCode1;								//固定值 GUI_SYS_SPEC_CODE1
	int m_iCode2;								//固定值 GUI_SYS_SPEC_CODE2
} TGuiSystemPowerOffReq;
	
#define GUI_SYS_POWEROFF_ACCEPT		0		//允许关机
#define GUI_SYS_POWEROFF_REFUSE		1		//拒绝关机
typedef struct									//系统关机回应
{
	int m_iRetCode;
} TGuiSystemPowerOffRsp;


#define GUI_SET_DEFAULT_REQ		603     //新的恢复默认命令码(带数据长度)
#define GUI_SET_DEFAULT_RSP		-603	//恢复默认回应

#define GUI_SYS_DEF_CODE1			0x20150617
#define GUI_SYS_DEF_CODE2			0x06172015  //本地
#define GUI_SYS_DEF_CODE3          	0x20040425   //远程

typedef struct
{
	int m_iCode1;			//固定值 GUI_SYS_DEF_CODE1
	int m_iCode2;			//固定值 GUI_SYS_DEF_CODE2	
} TGuiSysToDefaultReq;

#define GUI_SYS_DEFAULT_ACCEPT			0		//允许恢复默认
#define GUI_SYS_DEFAULT_REFUSE			1		//拒绝恢复默认

typedef struct							//恢复默认回应
{
	int m_iRetCode;
} TGuiSysToDefaultRsp;


// 消息
typedef int TGuiKey;

// JPGE抓拍
#define GUI_SNAPSHOT_REQ				363		//JPEG抓拍请求
#define GUI_SNAPSHOT_RSP				364		//JPEG抓拍回应
typedef struct									//抓拍请求
{
	int iChn;	 								// 1-16对应通道，0:全部通道
} TGuiSnapshotReq;

#define GUI_SNAPSHOT_OK			0
#define GUI_SNAPSHOT_ERR		1
typedef struct									//抓拍回应
{
	int m_iRetCode;							//返回码
} TGuiSnapshotRsp;

// 加强的抓拍功能，返回抓拍文件名 (IPC使用)
#define GUI_SNAPSHOT_REQ_EX			365			//JPED抓拍，同时返回抓拍图片名称
#define GUI_SNAPSHOT_RSP_EX			-365
typedef struct									//抓拍请求
{
	int iChn;	 								// 1-16对应通道，0:全部通道( 高位置0x1000表示快照临时抓拍)
} TGuiSnapshotReqEx;

typedef struct									//抓拍回应
{
	int m_iRetCode;							//返回码
	char m_cPicName[64];						//抓拍图片名称	
} TGuiSnapshotRspEx;

//多通道抓拍 (上海地标，外挂使用)
#define GUI_MULTI_CHN_SNAPSHORT_REQ			366  //多通道抓拍请求 （上海地标，外挂使用）
#define GUI_MULTI_CHN_SNAPSHORT_RSP	    	-366 //多通道抓拍回应  (上海地标，外挂使用)

typedef struct
{
	int m_iChn;  //需要抓拍的通道列表, 通道按位表示 0-不需要抓，1-需要抓拍。
	int m_iChnEx; //预留扩展用。
}TGuiSnapshotMultiChnReq;

typedef struct
{
	int m_iChnCnt;     //返回抓拍成功通道个数
}TGuiSnapshotMultiChnRsp;

typedef struct
{
	char m_iChn;   //通道号
	char m_cPicName[GUI_PUBLIC_LEN_63 + 1];  //对应通道号抓拍图片路径及名称
}TGuiSnapshotMultiChnMsg;  //返回抓拍图片路径及名称


// 路径演示
#define GUI_PTZ_PATH_PLAY_START	1			//开始演示路径
#define GUI_PTZ_PATH_PLAY_STOP	0			//结束演示路径
typedef struct
{
	int m_iCmd;									//命令码:开始/结束
	int m_iChn;									//通道号, 1-16
	int m_iPath;								//路径号, 1-8
} TGuiPtzPathPlayReq;

#define GUI_PTZ_PATH_PLAY_OK		0			//演示/结束路径成功
#define GUI_PTZ_PATH_PLAY_ERR		1			//演示/结束路径失败
typedef struct
{
	int m_iRetCode;							//返回码
} TGuiPtzPathPlayRsp;

// 电子放大

#define EZOOM_VO_START				1000

typedef struct
{
	int m_iChn;			//通道号: 1-16
	int m_iLeft;		//放大区域XP坐标
	int m_iTop;			//放大区域YP坐标
	int m_iWidth;		//放大区域宽度
	int m_iHeight;		//放大区域高度
} TGuiEzoomAttr;

typedef struct
{
	int m_iDevId;								//设备号dxl 2009/12/02
	TGuiEzoomAttr m_tAttr;						//初始化电子放大参数
} TGuiEzoomEnterReq;							//进入电子放大请求

#define GUI_EZOOM_ENTER_OK		0
#define GUI_EZOOM_ENTER_ERR		1
#define GUI_EZOOM_CHN_BIGGER_14	20131105  //当回放路数超过14路时，不支持电子放大
typedef struct									//进入电子放大回应
{
	int m_iRetCode;
} TGuiEzoomEnterRsp;    

// 退出电子放

typedef struct
{
	int m_iDevId;	
} TGuiEzoomExitReq; // 退出电子放大请求dxl 2009/12/02

#define GUI_EZOOM_EXIT_OK			0
#define GUI_EZOOM_EXIT_ERR		1
typedef struct
{
	int m_iRetCode;
} TGuiEzoomExitRsp;							//退出电子放大回应

typedef struct
{
	TGuiEzoomAttr m_tAttr;						//放大参数
} TGuiEzoomSetAttrReq;						//设置电子放大参数请求

#define GUI_EZOOM_SET_ATTR_OK		0
#define GUI_EZOOM_SET_ATTR_ERR	1
typedef struct
{
	int m_iRetCode;
} TGuiEzoomSetAttrRsp;						//设置电子放大参数请求

// UI启动信息
#define GUI_VER_LEN		31
typedef struct
{
	int m_iMasterDspDev;			//主显示设备
	int m_iVgaMode;				//VGA显示模式
	int m_iBncMode;				//BNC显示模式
	char m_strGuiVer[GUI_VER_LEN + 1];	//UI版本号
} TGuiStartUpMsg;				//通知消息

//add by wanzy 2011/10/11
typedef struct
{
	int m_iMasterDspDev;				//主显示设备
	int m_iVgaMode;				//VGA显示模式
	int m_iBncMode;				//BNC显示模式
	char m_strGuiVer[GUI_VER_LEN + 1 - 4];	//UI版本号
	char m_strCharSet[4];			//字符集版本 "GB"/"UTF"
} TGuiStartUpMsgEx;					//通知消息

typedef struct
{
	int m_iRetCode;				//返回值,0表示成功，其余值表失败
} TGuiStartUpRsp;				//消息回应

//设置VGA分辨率(和HDMI 设备1同源)
//设置HD0\HD1分辨率(高16位：HD0 低16位：HD1)
typedef struct
{
	int m_iVgaMode;				//VGA显示模式
} TGuiSetVgaModeReq;		//设置VGA格式

#define GUI_SET_VGA_OK		0
#define GUI_SET_VGA_ERR		1
typedef struct
{
	int m_iRetCode;				//返回码
} TGuiSetVgaModeRsp;

//设置BNC制式
typedef struct
{
	int m_iBncId;					//BNC编号，暂时使用0，表示全部BNC
	int m_iBncMode;				//模式
} TGuiSetBncModeReq;

#define GUI_SET_BNC_OK		0
#define GUI_SET_BNC_ERR		1
typedef struct
{
	int m_iRetCode;
} TGuiSetBncModeRsp;

//设置HDMI分辨率(设备0)
#define GUI_SET_HDMI_MOD_REQ 13
#define GUI_SET_HDMI_MOD_RSP -13

typedef struct
{
	int m_iHDMIMode;				//HDMI显示模式
} TGuiSetHDMIModeReq;		//设置HDMI格式

#define GUI_SET_HDMI_OK			0
#define GUI_SET_HDMI_ERR		1
typedef struct
{
	int m_iRetCode;				//返回码
} TGuiSetHDMIModeRsp;

//设置HDMI的显示器显示模式

#define GUI_SET_HDMI_DSP_MODE_REQ  14
#define GUI_SET_HDMI_DSP_MODE_RSP  -14
typedef struct
{
	int m_iHDMIDspMode;   //0-Auto, 1-YCBCR, 2-RGB
}TGuiSetHDMIDspModReq;

#define GUI_SET_HDMI_DSP_MODE_OK   0
#define GUI_SET_HDMI_DSP_MODE_ERR  1
typedef struct
{
	int m_iRetCode;
}TGuiSetHDMIDspModeRsp;

//VGA、HDMI输出模式调节
#define GUI_GET_VIDEO_OUTMOD_REQ	15		//获取输出模式请求
#define GUI_GET_VIDEO_OUTMOD_RSP		-15		//获取输出模式回应
#define GUI_SET_VIDEO_OUTMOD_REQ	 	16		//设置输出模式请求
#define GUI_SET_VIDEO_OUTMOD_RSP		-16		//设置输出模式回应

typedef enum
{	
	MOD_STANDARD = 0,		//标准
	MOD_SOFTNESS,			//柔和
	MOD_BRIGHT,			//鲜艳
	MOD_HIGHLIGHT,			//高亮
}TVideoOutputType;     //输出模式

typedef struct
{
	int m_iVoId;     //设备号,0表示全部,1表示VGA,2表示HDMI
}TGuiGetVideoOutputReq;

typedef struct									//获取输出模式参数回应
{
	int m_iVoId;
	int m_iVideoOutMode;							//输出模式
} TGuiGetVideoOutputRsp;

typedef struct									//设置输出模式参数请求
{
	int m_iVoId;
	int m_iVideoOutMode;							//输出模式
} TGuiSetVideoOutputReq;

typedef struct
{
	int m_iRetCode;
}TGuiSetVideoOutputRsp;
//###################end#############

// 光盘刻录
typedef struct				// 刻录开始回应
{
	int m_iRetCode;			// 0成功, 1失败
} TGuiCdBackupStartRsp;

typedef struct				// 刻录文件列表
{
	int m_iSeq;							// 序列号，保留
	char m_strFile[GUI_VF_NAME_LEN + 1];	// 文件名
} TGuiCdBackupFileReq;

typedef struct
{
	int m_iRetCode;			// 0成功，1失败
} TGuiCdBackupFileRsp;

typedef struct
{
	int m_iDid;				// 光盘编号，低八位(bit0-bit7)代表光盘一，bit8-bit15代表光盘二
} TGuiCdBackupExecReq;

typedef struct
{
	int m_iRetCode;			// 0成功，1失败
	int m_iBackupId;			//刻录ID，获取进度时使用，低八位(bit0-bit7)代表光盘一，bit8-bit15代表光盘二
} TGuiCdBackupExecRsp;

typedef struct				// 进度请求
{
	int m_iBackupId;			//刻录ID
} TGuiCdBackupPrgReq;

enum	// 刻录状态
{
	GUI_CD_BACKUP_FREE	= 0,   //空闲
	GUI_CD_BACKUP_DOING = 1,	// 正在刻录
	GUI_CD_BACKUP_DONE,		// 刻录完成
	GUI_CD_BACKUP_ERROR,	// 光盘损坏
	GUI_CD_BACKUP_FULL, 	//备份满一张盘
	GUI_CD_BACKUP_STARTERROR, 	//备份出错
};

typedef struct
{
	int m_iStat;				//当前状态
	int m_iPrg;				//当前进度
} TGuiCdBackupPrgRsp;


/***************************************************/
//视频参数配置(编码能力配置)
typedef struct
{
	int m_iMaxPicSize; //最大分辨率
	int m_iMaxFrameRate; //最大帧率
}TGuiVideoParaConf;
typedef struct
{
	int m_iValue;
}TGuiVideoParaUsedFpsConf;
//获取视频配置参数请求
typedef struct									
{
	int m_iChn;									//通道号
} TGuiGetVideoParaCnfReq;

//获取视频参数配置请求回应
typedef struct									//获取视频编码参数回应
{
	int m_iChn;									//通道号
	TGuiVideoParaConf m_tCnf;					//配置信息
} TGuiGetVideoParaCnfRsp;
// 设置视频编码参数请求
typedef TGuiGetVideoParaCnfRsp TGuiSetVideoParaCnfReq;

#define GUI_SET_VIDEOPARA_CNF_OK		0		//设置成功
#define GUI_SET_VIDEOPARA_CNF_ERR		1		//设置失败

//设置视频参数配置回应
typedef struct									
{
	int m_iRetCode;
} TGuiSetVideoParaCnfRsp;
//************************************




//************************************

//获取总的编码能力请求
typedef struct
{
	int m_iTotalCode;
}TGuiGetTotalFrameRateRsp;
//************************************
//获取某通道支持的视频分辨率大小
#define GUI_GET_CHN_VIDEO_SIZE_REQ     235   //获取单个通道支持的视频分辨率大小请求
#define GUI_GET_CHN_VIDEO_SIZE_RSP     -235   //获取单个通道支持的视频分辨率大小回应
typedef struct
{
	int m_iChn;   //请求通道
}TGuiGetChnVideoSizeReq;

typedef struct
{
	int m_iChn;
	int m_iCnt;   //返回请求通道支持几种视频分辨率
}TGuiGetChnVideoSizeRsp;

typedef struct
{
	int m_iVideoSize;  //分辨率大小(GUI_1080P 为12)
}TGuiVideoSizeItem;

//*****************************WIFI相关**************************************
//无线设置

//Wifi在线状态
#define NET_WIFI_UNONLINE    0
#define NET_WIFI_ONLINE       1
//wifi安全方式
#define NET_WIFI_WEP      1
#define NET_WIFI_WPA_PSK      2
#define NET_WIFI_WPA2_PSK     3
//wifi 密钥格式
#define NET_WIFI_KEYTYPE_HEX    1
#define NET_WIFI_KEYTYPE_ASCII  2
//wifi加密方法
#define NET_WIFI_ENCRYPT_TKIP  1
#define NET_WIFI_ENCRYPT_AES   2
//wifi 密码类型
#define NET_WIFI_PWDTYPE_DISEABLE   0
#define NET_WIFI_PWDTYPE_64BITS  1
#define NET_WIFI_PWDTYPE_128BITS 2
//wifi 密钥选择
#define NET_WIFI_KEYNUM_1     1
#define NET_WIFI_KEYNUM_2     2
#define NET_WIFI_KEYNUM_3     3
#define NET_WIFI_KEYNUM_4     4

//wifi 网络设置
#define GUI_GET_WIFI_NET_CNF_REQ    100000   //获取wifi网络设置信息请求
#define GUI_GET_WIFI_NET_CNF_RSP    -100000//获取wifi网络设置信息回应

//wifi 网络设置设置界面
#define GUI_GET_WIFI_IP_VIA_DHCP		0			//dhcp方式获取IP
#define GUI_GET_WIFI_IP_VIA_HUM		1			//手动设置IP
#define GUI_GET_WIFI_DNS_VIA_DHCP    0      //dhcp方式获取dns
#define GUI_GET_WIFI_DNS_VIA_HUM    1       //手动方式设置dns
#define GUI_WIFIESSID_STR_LEN  31

typedef struct 
{
	//int m_iHaveWifiCard;
	int m_iOnLineState;
	int m_iGetWifiIpMethod; 					//获取ip地址的方式
	int m_blWifiIsHandSetDns;						//手动设置DNS
	char m_strWifiMac[GUI_IP_STR_LEN + 1]; //mac地址
	char m_strWifiEssid[GUI_WIFIESSID_STR_LEN + 1];  //essid
	char m_strWifiIp[GUI_IP_STR_LEN + 1];			//ip地址
	char m_strWifiSubMsk[GUI_IP_STR_LEN + 1];		//子网掩码
	char m_strWifiGateWay[GUI_IP_STR_LEN + 1];		//网关
	char m_strWifiDNS[GUI_IP_STR_LEN + 1];			//首选域名解析
	//char m_strWifiSecDNS[GUI_IP_STR_LEN + 1];			//备选域名解析
}TGuiWifiIpMsg;

//获取wifi网络设置ip参数回应
typedef struct
{
	TGuiWifiIpMsg m_tWifiIpMsg;
}TGuiGetWifiIpRsp;    

#define GUI_SET_WIFI_NET_CNF_REQ    100001 //设置wifi网络设置信息请求
#define GUI_SET_WIFI_NET_CNF_RSP    -100001//设置wifi网络设置信息回应
//设置wifi网络设置ip参数请求
typedef struct
{
	TGuiWifiIpMsg m_tWifiIpMsg;
}TGuiSetWifiIpReq;   


//设置wifi网络设置ip参数回应
#define GUI_SET_WIFI_NET_IP_OK			0				//设置WIFI IP地址成功
#define GUI_SET_WIFI_NET_IP_ERR			1				//设置WIFI IP地址失败
typedef struct
{
	int m_iRetCode; 						//返回码
	TGuiWifiIpMsg m_tWifiIpMsg;             //重设后的IP信息
}TGuiSetWifiIpRsp;

//wifi设置界面
#define GUI_GET_WIFI_PARA_CNF_REQ      100002   //获取wifi设置配置请求
#define GUI_GET_WIFI_PARA_CNF_RSP      -100002 //获取wifi设置配置回应

#define GUI_WIFI_PWD_STR_LEN           64
#define GUI_SAFE_WIFI_UNSET           0
#define GUI_SAFE_WIFI_SET             1

typedef struct
{
	//int m_iOnLineState;
	char m_strEssid[GUI_WIFIESSID_STR_LEN + 1];
	int m_iSetSafeEnable;
	int m_iSecurityType;
	int m_iKeyFormat;
	int m_iKeyType;
	int m_iKeySelect;
	char m_cPassword[GUI_WIFI_PWD_STR_LEN + 1];
}TGuiWifiParaMsg;

typedef struct								//获取wifi参数回应	
{
	TGuiWifiParaMsg m_tWifiParaMsg;						
} TGuiGetWifiParaCnfRsp;

#define GUI_SET_WIFI_PARA_CNF_REQ      100003   //设置wifi设置配置请求
#define GUI_SET_WIFI_PARA_CNF_RSP      -100003   //设置wifi设置配置回应
typedef struct								//设置wifi参数请求
{
	TGuiWifiParaMsg m_tWifiParaMsg;
} TGuiSetWifiParaCnfReq;

#define GUI_SET_WIFI_PARA_CNF_OK			0
#define GUI_SET_WIFI_PARA_CNF_ERR			1
typedef struct								//设置wifi参数回应
{
	int m_iRetCode;
}TGuiSetWifiParaCnfRsp;


//wifi设置界面-搜索命令
//wifi设置-搜索事件
#define GUI_GET_SEARCH_WIFI_NET_REQ    100004 //获取wifi可用网络参数请求
#define GUI_GET_SEARCH_WIFI_NET_RSP   -100004 //获取wifi 可用网络参数回应
typedef struct
{
	char m_cWifiName[GUI_WIFIESSID_STR_LEN + 1];
	char m_cWifiEssid[GUI_WIFIESSID_STR_LEN + 1];
	int m_iSafeEnable;
}TGuiWifiNetListItem;

//获取wifi网络参数回应
#define GUI_WIFI_SEARCH_OK    0   //wifi  执行搜索成功 
#define GUI_WIFI_SEARCH_ERR   1   //wifi 执行搜索 失败
typedef struct								
{
	int m_iWifiNetCnt;
	int m_iRetCode;
}TGuiGetWifiNetParaListRsp;

//=================WIFI=AP===================================
#define GUI_GET_WIFI_AP_CNF_REQ      100005		//获取WIFI AP 参数请求
#define GUI_GET_WIFI_AP_CNF_RSP      -100005	//获取WIFI AP 参数回应

typedef struct
{
	int m_iOnLineState;							//无线状态
	char m_strWifiIp[GUI_IP_STR_LEN + 1];		//ip地址
	char m_strWifiSubMsk[GUI_IP_STR_LEN + 1];	//子网掩码
	TGuiDhcpServerMsg m_tDhcpServerMsg;
	TGuiWifiParaMsg	m_tWifiParaMsg;
}TGuiWifiApMsg;

typedef struct
{
	TGuiWifiApMsg m_tWifiApMsg;
}TGuiGetWifiApRsp;								//获取WIFI AP 参数回应

#define GUI_SET_WIFI_AP_CNF_REQ			100006		//设置WIFI AP参数请求
#define GUI_SET_WIFI_AP_CNF_RSP			-100006		//设置WIFI AP参数回应

typedef struct
{
	TGuiWifiApMsg m_tWifiApMsg;
}TGuiSetWifiApReq;				//设置WIFIAP请求

#define GUI_SET_WIFI_AP_OK    0
#define GUI_SET_WIFI_AP_ERR	  1
typedef struct
{
	int m_iRetCode; 
}TGuiSetWifiApRsp;				//设置WIFIAP回应 


//=====================================================================
//WIFI新增协议

#define GUI_GET_WIFI_ENABLE_STAT_REQ	100021 //获取wifi使能状态请求
#define GUI_GET_WIFI_ENABLE_STAT_RSP	-100021 //获取wifi使能状态回应

#define GUI_WIFI_ENABLE_STAT    1  //使能
#define GUI_WIFI_DISENABLE_STAT 0  //不使能

typedef struct								
{	
	int m_iEnable;  
}TGuiGetWifiEnableStatRsp;

#define GUI_SET_WIFI_ENABLE_STAT_REQ	100022 //设置wifi使能状态请求
#define GUI_SET_WIFI_ENABLE_STAT_RSP	-100022 //设置wifi使能状态回应

typedef struct								
{	
	int m_iEnable;
}TGuiSetWifiEnableStatReq;

typedef struct
{
	int m_iRetCode; // GUI_SET_OK .GUI_SET_ERR
}TGuiSetWifiEnableStatRsp;


//============================
//设置wifi断开和连接

#define GUI_WIFI_DISCONNECT		0  //未连接
#define GUI_WIFI_CONNECT		1  //连接

#define GUI_GET_WIFI_CONNECT_STAT_REQ	100023 //获取wifi连接状态请求
#define GUI_GET_WIFI_CONNECT_STAT_RSP	-100023 //获取wifi连接状态回应

typedef struct								
{
	int m_iConnectCmd;	//0:断开，1：连接
}TGuiGetWifiConnectStaRsp;

#define GUI_SET_WIFI_CONNECT_STAT_REQ	100024 //设置连接或断开wifi请求
#define GUI_SET_WIFI_CONNECT_STAT_RSP	-100024 //设置连接或断开wifi回应

typedef struct								
{
	int m_iConnectStat;	//0:断开，1：连接
}TGuiSetWifiConnectCmdReq;

typedef struct								
{	
	int m_iRetCode;
}TGuiSetWifiConnectCmdRsp;
//============================











//-----------------------------------------------------------------------------------------
#define GUI_CD_BACKUP_INDEX_REQ		717	//通知内核当前备份索引信息请求
#define GUI_CD_BACKUP_INDEX_RSP		-717     //通知内核当前备份索引信息回应

typedef struct
{
	int m_iCdBackUpIndex;	//当前备份第几个时间段
	int m_iCdBackUpCnt;	//光盘备份时间段总个数
}TGuiCdBackUpIndexReq;  

typedef struct
{
	int m_iRetCode;
}TGuiCdBackUpIndexRsp; //通知内核当前备份第几个时间段回应

#define GUI_CD_BACKUP_STOP_REQ			718	//强制停止光盘备份刻录请求
#define GUI_CD_BACKUP_STOP_RSP			-718 //强制停止光盘备份刻录请求回应

#define GUI_CD_BACKUP_STOP_OK      		0    //强制取消成功
#define GUI_CD_BACKUP_STOP_ERR     		1    //强制取消失败

typedef struct
{
	int m_iCdNo;
}TGuiCdBackupStopReq;     //强制停止光盘备份刻录请求

typedef struct				// 强制停止备份回应
{
	int m_iCdNo;
	int m_iRetCode;			
} TGuiCdBackupStopRsp;  	//强制停止光盘备份刻录请求回应


#define GUI_CD_BACKUP_START_REQ_EX		719	//刻录开始通知
#define GUI_CD_BACKUP_START_RSP_EX		-719 

typedef struct
{
	int m_iCdNo;              //光驱号 GUI_DISK_CD(原来逻辑) GUI_DISK_SEC_CD_ID (现在逻辑)
	int m_iCdBackUpMode;      //预留备份模式
}TGuiCdBackUpStartReqEx;    //扩展刻录备份开始协议请求

typedef struct
{
	int m_iCdNo;            //光驱号
	int m_iRetCode;
}TGuiCdBackUpStartRspEx; //扩展刻录备份开始协议回应


// 光盘实时刻录
#define GUI_CBL_MAX_CD		2

enum	//刻录模式
{
	GUI_CBL_MODE_SCD = 0,	//单盘刻录
	GUI_CBL_MODE_DCD = 1,	//双盘刻录
	GUI_CBL_MODE_CYC = 2,	//轮刻
	GUI_CBL_MODE_NSP = -1, //不支持实时刻录功能
};

typedef struct
{
	int m_iMode;				// 刻录模式
	int m_iCd;				// 光驱号,单盘刻录时使用(1, 2)
	int m_iChnEnBits; 			// 刻录通道使能位,bit:0-31，1-32通道
	int m_iReserved;			//通道数目大于32时使用
} TGuiCblModeMsg;

#define GUI_CBL_MODE_GET_REQ			724	//获取实时刻录模式请求
#define GUI_CBL_MODE_GET_RSP			-724 //获取实时刻录模式回应
typedef struct				// 获取刻录模式回应
{
	TGuiCblModeMsg m_tMode;
} TGuiCblModeGetRsp;

//关闭刻录窗体保存刻录模式
#define GUI_CBL_MODE_SET_REQ			725	//设置实时刻录模式请求(预留)
#define GUI_CBL_MODE_SET_RSP			-725	//设置实时刻录模式回应(预留)

typedef struct
{
	TGuiCblModeMsg m_tMode;
}TGuiSetCblModeReq;                   //设置刻录模式请求

typedef struct
{
	int m_iRetCode;
}TGuiSetCblModeRsp;                  //设置刻录模式回应


#define GUI_CBL_START_REQ				726	//启动光盘实时刻录请求
#define GUI_CBL_START_RSP				-726 //启动光盘实时刻录回应

typedef struct				// 启动实时刻录请求
{
	TGuiCblModeMsg m_tMode;
} TGuiCblStartReq;

typedef struct				// 启动实时刻录回应
{
	int m_iPid;				// 实时刻录操作号，-1表式启动失败
} TGuiCblStartRsp;



//=======================行业嵌入式==================
#define GUI_CBL_GET_MODEEX_REQ			733	//获取实时刻录扩展模式请求
#define GUI_CBL_GET_MODEEX_RSP			-733 //获取实时刻录扩展模式回应

#define GUI_CBL_DISC_LABEL_LEN       	63
typedef struct
{
	int m_iSealDisc;									//刻录完成是否封盘
	int m_iSpeed;										//刻录倍速,按倍速值为实际值，“最大”为16倍速
	int m_PopDisc;										//刻录完成是否弹盘
	int m_iDiscType;   									//光盘类型
	int m_iBurnTimes;  									//刻录时长
	char m_cDiscLabel[GUI_CBL_DISC_LABEL_LEN + 1];		//光盘标签
}TGuiCblModeMsgEx;

typedef struct
{
	TGuiCblModeMsgEx m_tModeEx;
}TGuiCblGetModeExRsp;

#define GUI_CBL_SET_MODEEX_REQ			734	//设置实时刻录扩展模式请求
#define GUI_CBL_SET_MODEEX_RSP			-734 //设置实时刻录扩展模式回应

typedef struct
{
	TGuiCblModeMsgEx m_tModeEx;
}TGuiCblSetModeExReq;

#define GUI_CBL_SET_MODEEX_OK  0   //设置成功
#define GUI_CBL_SET_MODEEX_ERR 1	//设置失败
typedef struct
{
	int m_iRetCode;
}TGuiCblSetModeExRsp;

#define GUI_CBL_SET_POP_DISC_REQ      735 //光盘出仓请求
#define GUI_CBL_SET_POP_DISC_RSP      -735 //光盘出仓回应

typedef struct
{
	int m_iMode;   //刻录模式
	int m_iCdNo;   // 0 --所有盘 (双盘和循环刻录所有盘出仓)，单盘时为获取的光驱号
}TGuiCblSetPopDiscReq;

typedef struct
{
	int m_iRetCode;
}TGuiCblSetPopDiscRsp;


#define GUI_SET_CBL_DVD_NUM_REQ   790//设置刻录光盘的好盘和坏盘数量请求
#define GUI_SET_CBL_DVD_NUM_RSP	  -790//设置刻录光盘的好盘和坏盘数量回应

typedef struct
{
	int m_iCdNo; //光驱号 预留
	int m_iOkNum; 
	int m_iErrNum;
}TGuiSetCblDvdNumReq;

typedef struct
{
	int m_iRetCode;
}TGuiSetCblDvdNumRsp;




//======================================================


#define GUI_CBL_END_REQ					727	//终止实时刻录打包命令
#define GUI_CBL_END_RSP					-727	//终止实时刻录打包命令


typedef struct				// 终止光盘实时刻录并打包命令
{
	int m_iPid;				// 操作号，由TGuiCblStartRsp给定
} TGuiCblEndReq;

#define GUI_CBL_END_OK		0
#define GUI_CBL_END_ERR		1
typedef struct				// 终止光盘实时刻录并打包命令
{
	int m_iRetCode;				// 操作号，由TGuiCblStartRsp给定
} TGuiCblEndRsp;


#define GUI_CBL_TAG_ALL_PID	-1
typedef struct				// 打点命令
{
	int m_iPid;				// 操作号，-1表式全部操作均打点
	int m_iChn;				// 通道号，默认0表所有通道
} TGuiCblTagReq;

#define GUI_CBL_TAG_OK 	0
#define GUI_CBL_TAG_ERR	1
typedef struct
{
	int m_iRetCode;
} TGuiCblTagRsp;

#define GUI_ONEKEY_CBL_REQ              732  //一键刻录请求
#define GUI_ONEKEY_CBL_RSP              -732 //一键刻录回应

typedef struct
{
	int m_iRetCode;
}TGuiOneKeyCblRsp;


#define GUI_ONEKEY_CBL_EX_REQ			736 //新增一键刻录请求
#define GUI_ONEKEY_CBL_EX_RSP			-736

typedef enum
{	
	ACT_CBL_START = 0,	//开始一键刻录
	ACT_CBL_STOP,		//结束一键刻录
	ACT_REC_START,		//开始审讯纯录像模式
	ACT_REC_STOP,		//结束审讯纯录像模式
}TGuiOneKeyCblAct;    //输出模式

typedef struct
{
	int m_iAct; //TGuiOneKeyCblAct
}TGuiOneKeyCblExReq;

typedef struct
{
	int m_iRetCode;    //返回值含义同TGuiOneKeyCblRsp的m_iRetCode;
}TGuiOneKeyCblExRsp;


#define GUI_GET_PLAYABLECD_CNT_REQ    737	//获取光驱状态
#define GUI_GET_PLAYABLECD_CNT_RSP    -737	//获取光驱状态

typedef enum
{
	GUI_DVD_DRIVER_NO_INFO	= 0,	
	GUI_DVD_DRIVER_NO_DISC	= 1,	//无盘
	GUI_DVD_DRIVER_OPEN		= 2,	//光驱弹出
	GUI_DVD_DRIVER_NOT_READY	= 3,	//光驱刚推进去
	GUI_DVD_DRIVER_DISC_OK		= 4,	//光驱有盘且准备好
}TGuiDvdDriverState;

typedef struct								
{
	int m_iDvdDriverSta[GUI_CBL_MAX_CD];			//光驱状态，具体状态见枚举
}TGuiGetDvdDriverStateRsp;


#define GUI_GET_DISK_LEFT_RECTIME_REQ    738	//获取硬盘剩余录像时长请求
#define GUI_GET_DISK_LEFT_RECTIME_RSP    -738	//获取硬盘剩余录像时长回应

typedef struct
{
	int m_iWhetherDiskSatisfy;			//硬盘剩余录像时长是否满足条件，开启审讯需保留24小时(0-不满足，1-满足)
	int m_iDiskLeftFreeSize;			//硬盘剩余空间，单位：M
	int m_iDiskLeftRecTime;			//硬盘剩余录像时长，单位：分钟
}TGuiGetDiskLeftRecTimeRsp;


//=================光盘文件查询==================

// 光盘回放相关协议(只添加光盘文件查询及打点信息获取协议)
// 光盘回放协议走原有的标准回放协议
#define GUI_CBL_FILE_QUERY_REQ			730	//搜索光盘文件请求
#define GUI_CBL_FILE_QUERY_RSP			-730

enum // 刻录状态
{
	GUI_CBL_STA_FREE	= 0, //空闲中，
	GUI_CBL_STA_DOING	= 1, //正在刻录，
	GUI_CBL_STA_PACK	= 2, //正在打包
};

enum // 光盘状态
{
	GUI_CBL_CD_STA_FREE	= 0, //光盘未使用
	GUI_CBL_CD_STA_BUSY	= 1, //光盘正在刻录
	GUI_CBL_CD_STA_BAD	= 2, //光盘已损坏
	GUI_CBL_CD_STA_FULL	= 3, //光盘已经满
	GUI_CBL_CD_STA_PACK	= 4, //光盘正在打包
	GUI_CBL_CD_STA_NODISK = 5, //没放入光盘
	GUI_CBL_CD_STA_CHECKDISK = 6,//光盘检测中
	GUI_CBL_CD_STA_BURN_FINISH = 7, //光盘刻录完成
	GUI_CBL_CD_STA_FORMATDISK	= 8,	//光盘初始化中
};

typedef struct		// 光盘录像文件查询请求
{
	int m_iCd;		// cd编号，默认0表式全部cd
	int m_iChn;		// 通道号, 0表示所有
	int m_iStartId;	// 按时间排序后记录的开始条数
	int m_iMaxCnt;	// 想要获取的记录条数
	int m_iStartTm;	// 开始时间(预留，默认为0)
	int m_iEndTm;	// 结束时间(预留，默认为0)
} TGuiCblFileQueryReq;

typedef struct
{
	int m_iCnt;		//查获取的数据条数(-1表失败)
} TGuiCblFileQueryRsp; //后紧跟m_iCnt条TGuiVideoFileMsg记录



#define GUI_CBL_FILE_FREE_COURT_QUERY_REQ			5021	//搜索光盘文件休庭时间段请求
#define GUI_CBL_FILE_FREE_COURT_QUERY_RSP			-5021

#define MAX_FREE_COURT_CNT	512

typedef struct		// 光盘录像文件休庭时间段查询请求
{
	int m_iCd;		// cd编号，默认0表式全部cd
	char m_strFileName[GUI_VF_NAME_LEN + 1];	//录像文件名
	int m_iMaxCnt;	// 想要获取的记录条数
} TGuiCblFileFreeCourtQueryReq;

typedef struct
{
	int m_iCnt;		//查获取的数据条数(-1表失败)
} TGuiCblFileFreeCourtQueryRsp; //后紧跟m_iCnt条TGuiCblVideoFileFreeCourtMsg记录

typedef struct		//休庭时间段资料
{
	int m_iBeginTm;	//休庭时间段开始时间
	int m_iEndTm;	//休庭时间段结束时间
} TGuiCblVideoFileFreeCourtMsg;



//*******获取光盘录像文件打点信息***********//
#define GUI_CBL_TAG_QUERY_REQ			731	//获取打点信息请求
#define GUI_CBL_TAG_QUERY_RSP			-731

typedef struct		// 光盘录像打点信息查询请求
{
	int m_iCd;		// cd编号, 0表所有cd
	int m_iChn;		// 通道号, 0表所有通道
	int m_iStartId;	// 记录开始编号
	int m_iMaxCnt;	// 打点信息最大条数
	int m_iStartTm;	// 开始时间
	int m_iEndTm;	// 结束时间
} TGuiCblTagQueryReq;    //全部获取打点信息

typedef struct
{
	char m_strFileName[GUI_VF_NAME_LEN + 1];
}TGuiCblTagQueryReqEx; //按文件获取打点信息

typedef struct
{
	int m_iCnt;		//查获的条数
} TGuiCblTagQueryRsp; //后紧跟m_iCnt条TGuiCblTagMsg记录;

typedef struct
{
	int m_iChn;
	int m_iStartTm;
	int m_iEndTm;
} TGuiCblTagMsg;

//*****获取硬盘录像文件的打点信息*********//
typedef struct
{
	char m_strFileName[GUI_VF_NAME_LEN + 1];
}TGuiGetMarkMsgReq;   //按文件获取打点信息

typedef struct
{
	int m_iCnt;		//查获的条数
} TGuiGetMarkMsgRsp; //后紧跟m_iCnt条TGuiCblTagMsg记录;

typedef struct
{
	int m_iChn;
	int m_iStartTm;
	int m_iEndTm;
} TGuiMarkMsg;


//================获取硬盘录像文件一段时间的打点信息============
#define GUI_GET_PERIODOFTIME_MARK_MSG_REQ   739  //获取一段时间内的打点信息请求
#define GUI_GET_PERIODOFTIME_MARK_MSG_RSP   -739 //获取一段时间内的打点信息回应
typedef struct
{
	char m_strFileName[GUI_VF_NAME_LEN + 1];  //录像文件名(可按照文件名查询)
	int m_iChn;								  //录像通道
	int m_iStartTm;							  //查询打点信息的开始时间
	int m_iEndTm;							  //查询打点信息的结束时间
}TGuiGetPeriodOfTimeMarkMsgReq;			  //获取一段时间内的打点信息请求

#define TGuiGetPeriodOfTimeMarkMsgRsp TGuiGetMarkMsgRsp  //获取一段时间内的打点信息回应
#define TGuiPeriodOfTimeMarkMsg TGuiMarkMsg				//打点信息结构体
	
//==============智能搜索================
#define GUI_VIDEO_PLAY_VCA_SEARCH_REQ 	1116
#define GUI_VIDEO_PLAY_VCA_SEARCH_RSP	-1116

#define GUI_VP_VCA_SEARCH_MAX_CNT 	1024     //智能搜索结果最大数目
typedef struct
{	
	int m_iCurTime;		//当前时间
	int m_iType;           //智能分析事件类型 使用之前的枚举类型EGuiVcaEventType
	char m_strFileName[GUI_VF_NAME_LEN + 1];
}TGuiVpVcaSearchReq; //智能搜索条件

typedef  TGuiCblTagQueryRsp	TGuiVpVcaSearchRsp;

typedef TGuiCblTagMsg		TGuiVcaSearchMsg; //智能搜索结果

#define GUI_VIDEO_PLAY_VCA_SEARCH_REQ_EX 	 1117
#define GUI_VIDEO_PLAY_VCA_SEARCH_RSP_EX	 -1117

typedef struct
{	
	int m_iChn;		//通道
	int m_iType;           //智能分析事件类型 使用之前的枚举类型EGuiVcaEventType
	int m_iBeginTm;
	int m_iEndTm;
	char m_strFileName[GUI_VF_NAME_LEN + 1];
}TGuiVpVcaSearchReqEx; //智能搜索条件

typedef  TGuiCblTagQueryRsp	TGuiVpVcaSearchRspEx;

//智能回放异步搜索协议
#define GUI_START_VP_VCA_SEARCH_REQ     5010
#define GUI_START_VP_VCA_SEARCH_RSP		-5010

typedef TGuiVpVcaSearchReqEx TGuiStartVpVcaSearcReq;
typedef struct 
{
	int m_iSearchId;
}TGuiStartVpVcaSearcRsp;



#define GUI_GET_VP_VCA_SEARCH_STAT_REQ     5011
#define GUI_GET_VP_VCA_SEARCH_STAT_RSP	-5011
enum
{
	GUI_VP_VCA_SEARCH_OVER = 0,
	GUI_VP_VCA_SEARCH_DOING
}; 
typedef TGuiStartVpVcaSearcRsp TGuiGetVpVcaSearcStatReq;
typedef struct 
{
	int m_iSearchId;
	int m_iStat;			//底16位代表搜索状态，高16位表示是否加密
	int m_iSearchPro;      //进度 0-100
	int m_iSearchCurTm;		//当前搜索时间
}TGuiGetVpVcaSearcStatRsp;


#define GUI_GET_VP_VCA_SEARCH_RESULT_REQ     	5012
#define GUI_GET_VP_VCA_SEARCH_RESULT_RSP		-5012

typedef TGuiStartVpVcaSearcRsp TGuiGetVpVcaSearcResultReq;

typedef  TGuiCblTagQueryRsp TGuiGetVpVcaSearchResultRsp;


// *****************人脸检测**********************//

#define GUI_GET_VP_VCA_FACESEA_RESULT_REQ		5013	//人脸搜索结果请求
#define GUI_GET_VP_VCA_FACESEA_RESULT_RSP		-5013	//人脸搜索结果回应

typedef  TGuiStartVpVcaSearcRsp TGuiGetVpVcaFaceSeaResultReq;
typedef  TGuiCblTagQueryRsp TGuiGetVpVcaFaceSeaResultRsp;

typedef struct
{
	unsigned short	m_usLeft;
	unsigned short	m_usTop;
	unsigned short	m_usRight;
	unsigned short	m_usBottom;
}TGuiVcaRect;

typedef  struct
{
	int 	m_iChn;
	int 	m_iFaceId;      //人脸ID
	int 	m_iBeginTm;
	int 	m_iEndTm;
	int 	m_iFileloc;
	int 	m_iFrameNo;
	TGuiVcaRect m_tRect;
} TGuiFaceMsg;
//紧跟m_iCnt个TGuiFaceMsg

#define  GUI_START_VP_VCA_DSPFACE_REQ			5014	//人脸显示请求
#define  GUI_START_VP_VCA_DSPFACE_RSP			-5014	//人脸显示回应

typedef struct
{
	int  m_iChn;
	int  m_iFaceId;       //FaceID 
	int  m_iBeginTm;
	int	 m_iEndTm;
	int  m_iFileloc;
	int  m_iFrameNo;
	TGuiVcaRect m_tRect;
	char m_strFaceName[ GUI_PUBLIC_LEN_63 + 1];	//人脸存放全路径包含文件名，放在ext4分区
}TGuiStartVpFaceDspReq;

/*ui指定路径，内核往这里存放bmp格式的图片，ui负责描绘显示。*/

typedef struct
{
	int  m_iRetCode;		//0表成功，其余表失败
 }TGuiStartVpFaceDspRsp;

//点右侧人脸的时候 直接发定位的命令

//*************人数统计*********************//
#define GUI_GET_VP_VCA_PEOPLE_CNT_REQ		5015	//人数统计结果请求
#define GUI_GET_VP_VCA_PEOPLE_CNT_RSP		-5015	//人数统计结果回应
typedef  TGuiStartVpVcaSearcRsp TGuiGetVpVcaPeopleSeaResultReq;
typedef  TGuiCblTagQueryRsp TGuiGetVpVcaPeopleSeaResultRsp;

typedef  struct 
{
	int  m_iChn;
	int  m_iBeginTm;
	int		m_iInCnt;			//进入的人数
	int		m_iOutCnt;			//出去的人数
} TGuiGetVpVcaPeopleCntRsp;


//**********************车牌识别*****************//
#define GUI_START_VP_VCA_SEARCHCAR_REQ		5018  //开始车牌搜索请求
#define GUI_START_VP_VCA_SEARCHCAR_RSP		-5018 	//回应

typedef enum
{
	GUI_VCA_SEARCH_BY_CAR = 1, //多通道搜一个车牌
	GUI_VCA_SERACH_BY_CHN = 2, //单通道搜索所有车牌
}EGuiCarType;

typedef struct
{
	int 		m_iChnList[32];	//每个bit表示一个通道，最多支持1024个通道;
	int 		m_iType;		//搜索类型 见枚举EGuiCarType
	int  		m_iBeginTm;		//开始时间
	int 	 	m_iEndTm;		//结束时间
	char 	m_strCarNo[GUI_PUBLIC_LEN_63 + 1];
}TGuiVcaCarSearchReq;

typedef struct
{
	int  m_iSearchId;   //返回id，根据id要结果
}TGuiVcaCarSearchRsp;

#define  GUI_START_VP_VCA_DSPCAR_REQ			5019     //车牌显示请求
#define 	GUI_START_VP_VCA_DSPCAR_RSP			-5019	 //回应

typedef struct
{
	int  m_iSearchId;          
}TGuiStartVpCarDspReq;

typedef struct
{
	int  m_iCnt;         //返回实际的车牌数,后面跟m_iCnt个TGuiVcaCarMsg结构体信息
}TGuiStartVpCarDspRsp;

typedef struct
{
	int		m_iChn;
	int		m_iBeginTm;
	int 	m_iEndTm;
	char 	m_strCarNo[GUI_PUBLIC_LEN_63 + 1];
}TGuiVcaCarMsg;


#define  GUI_VIDEO_PLAY_PERIOD_REQ    5200     //分时段回放请求
#define  GUI_VIDEO_PLAY_PERIOD_RSP	  -5200    //分时段回放回应
typedef struct
{
	int  m_iChn;            //通道号
	int  m_iPlayId;         //vo位置id
	int  m_iBeginTm;		//开始时间	
	int  m_iEndTm;		//结束时间
}TGuiPeriodVideoPlayReq;

typedef struct
{
	int  m_iRetCode;          //返回值
}TGuiPeriodVideoPlayRsp;


#define GUI_SET_PEOPCNT_REPORT_REQ	5201  //人数统计导出报表请求
#define GUI_SET_PEOPCNT_REPORT_RSP	-5201 //人数统计导出报表回应

typedef enum
{
	GUI_XLS = 1,
	GUI_TXT,
	GUI_DOC,
}EGuiReportEnum;

typedef struct
{
	int m_iType;   //导出格式 见枚举EGuiReportEnum
	char m_cPath[GUI_PUBLIC_LEN_127 + 1]; //导出路径
}TGuiPeopCntReportReq;

typedef struct
{
	int m_iRetCode;
}TGuiPeopCntReportRsp;

//========================end================


//================通道管理相关协议=========================
typedef enum
{
	GUI_DIGITAL_CHN  = 0,//数字通道
	GUI_LOCAL_CHN,  //本地模拟通道
	GUI_VC_CHN,	     //合成通道
}TGuiChannelType; //通道类型

typedef struct
{
	int m_iChn; //通道号
}TGuiGetChnTypeReq;

typedef struct
{
	int m_iChn;
	int m_iType;  //通道类型
}TGuiGetChnTypeRsp;


#define GUI_STREAM_ALL_TYPE   0 //全部
#define GUI_STREAM_MAIN_TYPE  1 //主码流 
#define GUI_STREAM_SUB_TYPE   2 //副码流

#define GUI_GET_CHN_ENABLE_STAT_REQ   805  //获取通道连接状态请求
#define GUI_GET_CHN_ENABLE_STAT_RSP   -805  //获取通道连接状态回应
typedef struct
{
	int m_iChn;
	int m_iStreamType; //0-全部，1-主码流,2-副码流 ( 默认为1)
}TGuiGetChnEnableStatReq;
typedef struct
{
	int m_iChn;
	int m_iStat;//通道启用状态1-启用, 2-不启用，(扩展16位,0-通道状态，与原来兼容，1-已连接，2-未连接)
}TGuiGetChnEnableStatRsp;
/*根据成员m_iStat高16位的值来进一步区分未连接成功的原因
3-未连接，用户名或密码错误
4-未连接，IP被屏蔽
5-未连接，账号被锁定
6-未连接，已经达到最大连接数
7-未连接，网络异常
8-未连接，接入带宽达到上限*/


#define GUI_SET_CHN_ENABLE_STAT_REQ   806  //设置通道连接状态请求
#define GUI_SET_CHN_ENABLE_STAT_RSP   -806  //设置通道连接状态回应


typedef struct
{
	int m_iChn;
	int m_iStat;//通道启用状态1-启用 ,2-不启用(扩展16位,0-通道状态，与原来兼容，1-已连接，2-未连接)
	int m_iStreamType; //0-全部，1-主码流,2-副码流(默认为0)
}TGuiSetChnEnableStatReq;  //设置通道连接状态请求
typedef struct
{
	int m_iRetCode;
}TGuiSetChnEnableStatRsp; //设置通道连接状态回应

//本地模拟通道
typedef struct
{
	int m_iChn;
}TGuiGetLocalChnAttrReq; //获取本地模拟通道请求

typedef struct
{
	int m_iChn;
	char m_cLocalChnAttr[32];
}TGuiGetLocalChnAttrRsp; //获取本地模拟通道回应

//合成通道
typedef struct
{
	int m_iChn;
}TGuiGetVcChnAttrReq; //获取合成通道请求

typedef struct
{
	int m_iChn;
	char m_cVcChnAttr[32];
}TGuiGetVcChnAttrRsp; //合成通道属性回应


// 数字通道相关协议

enum
{
	GUI_DIG_CHN_IP = 0,			//IP地址
	GUI_DIG_CHN_HOSTNAME,	 //域名
	GUI_DIG_CHN_IMD,			//主动模式
};

enum
{
	GUI_DIG_CHN_TCP = 0,	//tcp
	GUI_DIG_CHN_UDP,		//udp
	GUI_DIG_CHN_BC,		//多播
};

#define GUI_DIG_CHN_URL_LEN		39
#define GUI_DIG_CHN_PTZ_NAME		31
#define GUI_DIG_CHN_NAME_LEN		31

#define GUI_DIG_CHN_STAT_ENABLE    1
#define GUI_DIG_CHN_STAT_DISENABLE 2


//设备类型
enum
{
	GUI_DEV_TYPE_PRIVATE = 0, //设备类型为私有类型
	GUI_DEV_TYPE_ONVIF,      //设备类型为onvif
	GUI_DEV_TYPE_PUSH,    //设备类型为push流
	GUI_DEV_TYPE_RTSP,   //设备类型为RTSP
};

//连接模式
enum
{
	GUI_DIG_CHN_S_LNET = 0, //局域网
	GUI_DIG_CHN_S_HNAME, 	//域名
	GUI_DIG_CHN_S_AMOD,	//主动模式
};

#define GUI_DIG_TIANDY_DEV   GUI_DEV_TYPE_PRIVATE
#define GUI_DIG_ONVIF_DEV    GUI_DEV_TYPE_ONVIF
#define GUI_DIG_RTSP_DEV     GUI_DEV_TYPE_RTSP

/*
m_iConnectMod 表示如下:

0x00000000  - 私有 ip 
0x00000001 - 私有 域名
0x00000002 - 私有 主动
0x00010000 - onvif TCP
0x00010001 - onvif UDP
0x00020000 - PUSH
0x00030000 - RTSP
*/

// 数字通道
#define GUI_SET_DIG_CHN_REQ			740	//设置数字通道参数请求
#define GUI_SET_DIG_CHN_RSP			-740 //设置数字通道参数回应


typedef struct
{
	int m_iConnectMod;		//高16位0-私有，1-onvif，解析如上 
	char m_strUrl[GUI_DIG_CHN_URL_LEN  + 1]; //服务器地址
	char m_strProxy[GUI_DIG_CHN_URL_LEN  + 1]; //代理IP
	int m_iPort;	//端口号
	int m_iChn;	//对方通道号
	int m_iStreamType;	//码流类型,0:主码流, 1副码流2:音频流
	int m_iConntectType;  //tcp/udp/bc
	int m_iPtzAddr;	//ptz地址 (m_iConntectType为bc模式时，则表示多播端口号)
	char m_strPtzProtocol[GUI_DIG_CHN_PTZ_NAME + 1]; //ptz协议 (m_iConntectType为bc模式时，则表示多播地址)
	char m_strUserName[GUI_DIG_CHN_NAME_LEN + 1]; //用户名
	char m_strUserPsw[GUI_DIG_CHN_NAME_LEN + 1];	//密码
	int m_iEnable;    //状态
	char m_strVdecPsw[GUI_DIG_CHN_NAME_LEN + 1];	//视频解密密码
} TGuiDigChnMsg;


#define GUI_SET_DIG_CHN_OK 0
#define GUI_SET_DIG_CHN_ERR 1

#define GUI_DIG_CHN    0  //数字通道
#define GUI_SIM_CHN    1  //模拟通道
typedef struct		//设置数字通道请求
{
	int m_iType;		//类型:0设置数字通道, 1设置为模式通道(预留)
	int m_iChn;		//通道号
	TGuiDigChnMsg m_tChnMsg; //通道信息
} TGuiSetDigChnReq;

typedef struct
{
	int m_iRetCode;
} TGuiSetDigChnRsp;

#define GUI_GET_DIG_CHN_REQ			741	//设置数字通道参数请求
#define GUI_GET_DIG_CHN_RSP			-741 //设置数字通道参数回应

typedef struct		//获取数字通道信息请求
{
	int m_iChn;
} TGuiGetDigChnReq;

typedef struct
{
	int m_iChn;		// -1表失败
	TGuiDigChnMsg m_tChnMsg;
} TGuiGetDigChnRsp;

typedef struct
{
	int m_iConnectMod;		//高16位0-私有，1-onvif，解析如上 
	char m_strUrl[GUI_DIG_CHN_URL_LEN  + 1]; //服务器地址
	char m_strProxy[GUI_DIG_CHN_URL_LEN  + 1]; //代理IP
	int m_iPort;	//端口号
	int m_iChn;	//对方通道号
	int m_iStreamType;	//码流类型,0:主码流, 1副码流2:音频流
	int m_iConntectType;  //tcp/udp/bc
	int m_iPtzAddr;	//ptz地址 (m_iConntectType为bc模式时，则表示多播端口号)
	char m_strPtzProtocol[GUI_DIG_CHN_PTZ_NAME + 1]; //ptz协议 (m_iConntectType为bc模式时，则表示多播地址)
	char m_strUserName[GUI_DIG_CHN_NAME_LEN + 1]; //用户名
	char m_strUserPsw[GUI_DIG_CHN_NAME_LEN + 1];	//密码
	int m_iEnable;    //状态
	char m_strVdecPsw[GUI_DIG_CHN_NAME_LEN + 1];	//视频解密密码
	char m_strRtspUrl[GUI_PUBLIC_LEN_255 + 1]; //用于扩展RTSP的地址，当RTSP 地址长度大于80字节时，使用本字段，否则使用原字段。
	char m_strMac[GUI_IP_STR_LEN + 1];			//mac地址
} TGuiDigChnMsgEx;

#define	GUI_GET_DIG_CHN_REQ_EX	747	//获取数字通道参数请求
#define GUI_GET_DIG_CHN_RSP_EX	-747	//获取数字通道参数回应
typedef struct		//获取数字通道信息请求
{
	int m_iChn;
} TGuiGetDigChnReqEx;

typedef struct
{
	int m_iChn;		// -1表失败
	TGuiDigChnMsgEx m_tChnMsg;
} TGuiGetDigChnRspEx;

#define GUI_SET_DIG_CHN_REQ_EX			748	//设置数字通道参数请求
#define GUI_SET_DIG_CHN_RSP_EX			-748 //设置数字通道参数回应

typedef struct		//设置数字通道请求
{
	int m_iType;		//类型:0设置数字通道, 1设置为模式通道(预留)
	int m_iChn;		//通道号
	TGuiDigChnMsgEx m_tChnMsg; //通道信息
} TGuiSetDigChnReqEx;

typedef struct
{
	int m_iRetCode;
} TGuiSetDigChnRspEx;



#define GUI_SEARCH_DIG_CHN_REQ		742	//搜索数字通道请求
#define GUI_SEARCH_DIG_CHN_RSP			-742	//搜索数字通道回应

#define GUI_DIG_CHN_ID_LEN  39
typedef struct
{
	char m_strUrl[GUI_DIG_CHN_URL_LEN  + 1]; //服务器地址
	int m_iPort;	//端口号
	int m_iChn;	//对方通道号
	char m_strServerType[GUI_DIG_CHN_NAME_LEN + 1];//服务器类型
	char m_strDeviceID[GUI_DIG_CHN_ID_LEN + 1];//出厂设备ID
	char m_strServerName[GUI_DIG_CHN_NAME_LEN + 1];//服务器名称
	char m_strProxy[GUI_DIG_CHN_URL_LEN  + 1]; //代理IP
}TGuiSearchChnMsg;

//新加数字通道获取命令码
#define GUI_GET_SDIG_CHN_REQ_EX			745	//获取数字通道结果请求
#define GUI_GET_SDIG_CHN_RSP_EX			-745	//获取数字通道结果回应

typedef struct
{
	int m_iPort;	//端口号
	int m_iChn;	//对方通道号
	char m_strUrl[GUI_DIG_CHN_URL_LEN  + 1]; //服务器地址,IP地址
	char m_strServerType[GUI_DIG_CHN_NAME_LEN + 1];//服务器类型
	char m_strDeviceID[GUI_DIG_CHN_ID_LEN + 1];//出厂设备ID
	char m_strServerName[GUI_DIG_CHN_NAME_LEN + 1];//服务器名称
	char m_strProxy[GUI_DIG_CHN_URL_LEN  + 1]; //代理IP
	char m_strMac[GUI_IP_STR_LEN + 1];			//mac地址
	char m_strSubMsk[GUI_IP_STR_LEN + 1];		//子网掩码
	char m_strGateWay[GUI_IP_STR_LEN + 1];		//网关
	char m_strDNS[GUI_IP_STR_LEN + 1];			//域名解析
}TGuiSearchChnMsgEx;

#define	GUI_GET_CBL_CDSERIALNO_REQ	746	//获取光驱序列号请求
#define GUI_GET_CBL_CDSERIALNO_RSP	-746	//获取光驱序列号回应

#define GUI_CBL_SERIALNO_LEN	16	//序列号长度

typedef struct	//获取光驱序列号请求
{
	int m_iCdId;	//光驱号
}TGuiGetSerialNoReq;

typedef struct //获取光驱序列号回应
{
	int m_iCdId;	//光驱号
	char m_strSerialNum[GUI_CBL_SERIALNO_LEN + 1];		//序列号
}TGuiGetSerialNoRsp;


//onvif  修改IP
#define GUI_ONVIF_CHANGE_IP_CMD_REQ	822  //修改前端onvif  IPC的IP请求
#define GUI_ONVIF_CHANGE_IP_CMD_RSP	-822  //回应
#define GUI_ID_STR_LEN 39
#define GUI_SERVNAME_STR_LEN 39

typedef struct
{
	int m_iChn;
	char m_strMac[GUI_IP_STR_LEN + 1];			//mac地址
	char m_strIp[GUI_IP_STR_LEN + 1];			//IP地址
	char m_strSubMsk[GUI_IP_STR_LEN + 1];		//子网掩码
	char m_strGateWay[GUI_IP_STR_LEN + 1];		//网关
	char m_strDNS[GUI_IP_STR_LEN + 1];			//域名解析
	char m_strFactryID[GUI_ID_STR_LEN + 1];		// 
	char m_strServerName[GUI_SERVNAME_STR_LEN + 1];
	char m_strUserName[GUI_ID_STR_LEN + 1];  //用户名
	char m_strPassword[GUI_ID_STR_LEN + 1];	//密码
}TGuiChangeOnvifIpReq;

typedef struct
{
	int m_iRetCode;
} TGuiChangeOnvifIpRsp;


/*
m_iSearchMod 表示如下:

0x00000000  - 私有 ip 
0x00000001 - 私有 域名
0x00000002 - 私有 主动
0x00010000 - onvif TCP
0x00010001 - onvif UDP
0x00020000 - PUSH
0x00030000 - RTSP

*/
//m_iSearchMod 和m_iConnectMod是一样的
typedef struct		//搜索数字通道请求
{
	int m_iSearchMod;	//连接模式 高16 表示设备类型，0-私有，1-onvif, 
						//低16位表连接模式，私有协议下0表示
} TGuiSearchDigChnReq;

typedef struct
{
	int m_iSid;	//搜索查询号，以后按此号获取搜索结果，-1表失败
} TGuiSearchDigChnRsp;
#define GUI_GET_SDIG_CHN_REQ			743	//获取数字通道结果请求
#define GUI_GET_SDIG_CHN_RSP			-743	//获取数字通道结果回应


typedef struct	//获取通道搜索结果请求
{
	int m_iSid;
} TGuiGetSearchedDigChnReq;

typedef struct	//回应，后跟m_iCnt个TGuiSearchDigChnMsg结构
{
	int m_iCnt;	//此次返回的通道条数，可以是0，-1表式搜索结束
} TGuiGetSearchedDigChnRsp;

#define GUI_GET_DIG_CHN_PROTOCOL_CNT_REQ  744  //获取数字通道协议总数
#define GUI_GET_DIG_CHN_PROTOCOL_CNT_RSP  -744 //获取数字通道协议总数回应

typedef struct 								//获取数字通道协议列表请求
{
	int m_iChn; //数字通道号默认为0
} TGuiGetDigChnProtocolListReq;

typedef struct 								//获取数字通道协议列表回应
{
	int m_iChn;                             //数字通道号
	int m_iProtocolCnt;						//协议总数
} TGuiGetDigChnProtocolListRsp;

typedef struct 								//协议列表项
{
	char m_strProtocolName[GUI_DIG_CHN_PTZ_NAME + 1]; //协议名
} TGuiDigChnProtocolListItem;

//数字通道编辑类型
#define GUI_SET_CHN_TYPE_PARAM  -1
typedef struct 
{
	int m_iChn;
	int m_iType;
	int m_iParam; //预留 默认设为-1
}TGuiChnMagSetChnTypeReq; //设置通道类型请求

typedef struct
{
	int m_iRetCode;
}TGuiChnMagSetChnTypeRsp; //设置通道类型回应

typedef struct
{
	int m_iChn;
}TGuiChnGetChnEditAbleTypeReq; //获取可编辑通道类型请求

typedef struct
{
	int m_iChn;
	int m_iEditTypeFlg;
}TGuiChnGetChnEditAbleTypeRsp;  //获取可编辑通道类型回应

#define GUI_BATCH_ACTION_START    750       //批量操作开始
#define GUI_BATCH_ACTION_STOP     751		//批量操作结束

typedef enum
{

	GUI_CHN_ENABLE = 1,
	GUI_CHN_DISABLE,
	GUI_CHN_DEL,
}EGuiBatchEnum;

typedef struct
{
	int m_iBatchType;    //类型
}TGuiSetBatchActionReq;

//---------------------网络前端即插即用-------------------------------
typedef enum                  
{
	GUI_MANUALADD = 0,						//手动添加即不启用即插即用
	GUI_AUTOADD,							//自动添加
	GUI_PROMPTADD,							//提示添加
} EGuiChnAddType;                         //添加数字通道的方式

#define GUI_GET_DIG_CHN_ADD_TYPE_REQ   807  //获取数字通道添加方式请求
#define GUI_GET_DIG_CHN_ADD_TYPE_RSP   -807 //获取数字通道添加方式回应

typedef struct
{
	int m_iAddType;	//数字通道添加方式，0表示不启用，1表示自动添加，2表示提示添加
}TGuiGetDigChnAddTypeRsp;				//获取添加方式回应

#define GUI_SET_DIG_CHN_ADD_TYPE_REQ   808  //设置数字通道添加方式请求
#define GUI_SET_DIG_CHN_ADD_TYPE_RSP   -808  //设置数字通道添加方式回应

#define GUI_SET_DIG_ADD_TYPE_OK   0       //设置成功
#define GUI_SET_DIG_ADD_TYPE_ERR  1       //设置失败

typedef struct
{
	int m_iAddType;	//数字通道添加方式，1表示手动添加，2表示提示添加，3表示自动添加
}TGuiSetDigChnAddtypeReq;	//设置数字通道添加方式请求

typedef struct
{
	int m_iRetCode;
}TGuiSetDigChnAddTypeRsp;	//设置数字通道添加方式回应

#define GUI_GET_DIG_CHN_UPNP_NEW_DEVICE_REQ  809     //即插即用获取新设备请求
#define GUI_GET_DIG_CHN_UPNP_NEW_DEVICE_RSP  -809    //即插即用获取新设备回应
typedef struct
{
	int m_iCnt;   //默认为1
}TGuiGetDigChnUpnpReq;

typedef struct
{
	int m_iCnt;//返回发现的设备个数
}TGuiGetDigChnUpnpDevCntRsp;

typedef struct
{
	int m_iConStat;		//0 - 设备丢失连接，1-发现新设备
	int m_iChn;				//欲添加设备到的通道号
	int m_iConId;        //欲添加设备的连接ID
	char m_strMac[GUI_IP_STR_LEN + 1]; //欲添加设备的mac地址
	char m_strIp[GUI_IP_STR_LEN + 1]; //欲添加设备的IP地址
}TGuiGetDigChnUpnpDevMsgItem;

#define GUI_SET_ADD_DIG_CHN_UPNP_REQ          810    //设置添加数字通道请求
#define GUI_SET_ADD_DIG_CHN_UPNP_RSP          -810    //设置添加数字通道回应

typedef struct		//设置添加数字通道请求
{
	int m_iConCmd;      // 1-连接，0-不连接
	int m_iChn;		//欲添加设备到通道
	int m_iConId;        //欲添加设备的连接ID
	char m_strMac[GUI_IP_STR_LEN + 1]; //欲添加设备的mac地址
	char m_strIp[GUI_IP_STR_LEN + 1]; //欲添加设备的IP地址
} TGuiSetAddDigChnUpnpReq;

typedef struct
{
	int m_iRetCode;
} TGuiSetAddDigChnUpnpRsp;

//------------------------------------------------------------------------------
// 网络连接数字通道剩余带宽

#define GUI_GET_NET_SPARE_BANDWIDTH_REQ  811  //获取网络连接剩余带宽请求
#define GUI_GET_NET_SPARE_BANDWIDTH_RSP  -811 //获取网络连接剩余带宽回应

typedef struct
{
	char m_cBandWidth[GUI_PUBLIC_LEN_31 + 1]; //传字符串
}TGuiGetNetSpareBandwidthRsp;

//是否给前端自动降码率，带宽均衡
#define GUI_GET_BANDWIDTH_BALANCE_ENABEL_REQ	840		//获取
#define GUI_GET_BANDWIDTH_BALANCE_ENABEL_RSP	-840
typedef struct
{
	int m_iBalanced;	//0表示不启用
}TGuiGetBandWidthBalanceRsp;

#define GUI_SET_BANDWIDTH_BALANCE_ENABEL_REQ	841		//设置
#define GUI_SET_BANDWIDTH_BALANCE_ENABEL_RSP	-841
typedef struct
{
	int m_iBalanced;	//0表示不启用
}TGuiSetBandWidthBalanceReq;

typedef struct
{
	int m_iRetCode;  //0表成功 1表示失败
}TGuiSetBandWidthBalanceRsp;

//POE添加通道方式
typedef enum
{
	ADD_BY_HUM = 1,  	 //手动添加
	ADD_BY_PLUG_AND_PLAY,  	//即插即用
}EAddType;

#define GUI_GET_POE_WORKMODE_REQ		820		//获取poe通道添加方式请求
#define GUI_GET_POE_WORKMODE_RSP		-820
typedef struct
{
	int m_iChn;
}TGuiGetIpcPluginWorkModeReq;

typedef struct
{
	int m_iChn;
	int m_iWorkMode;    // 1 手动 2即插即用
}TGuiGetIpcPluginWorkModeRsp;

//设置等
#define GUI_SET_POE_WORKMODE_REQ		821
#define GUI_SET_POE_WORKMODE_RSP		-821

typedef struct
{
	int m_iChn;
	int m_iWorkMode;	//见EAddType
}TGuiSetIpcPluginWorkModeReq;

typedef struct
{
	int m_iRetCode;		//0成功 其余失败
}TGuiSetIpcPluginWorkModeRsp;



//==============IPC集中管理=====================
typedef struct
{
	int m_iChn;
	char m_strIpcVersion[GUI_PUBLIC_LEN_31 + 1];  	//版本号
}TGuiIPCMsg;

//获取IPC通道的设备号及版本号
#define GUI_GET_IPC_VERSION_REQ	812   
#define GUI_GET_IPC_VERSION_RSP	-812
typedef struct
{
	int m_iChn;
}TGuiGetIpcVerReq;

typedef struct
{
	TGuiIPCMsg m_tIpcMsg;
} TGuiGetIpcVerRsp;

//修改IP
#define GUI_IPC_CHANGE_IP_CMD_REQ	813  //修改前端IPC的IP请求
#define GUI_IPC_CHANGE_IP_CMD_RSP	-813  //回应

typedef struct
{
	int m_iChn;
	char m_strMac[GUI_IP_STR_LEN + 1];			//mac地址
	char m_strIp[GUI_IP_STR_LEN + 1];			//IP地址
	char m_strSubMsk[GUI_IP_STR_LEN + 1];		//子网掩码
	char m_strGateWay[GUI_IP_STR_LEN + 1];		//网关
	char m_strDNS[GUI_IP_STR_LEN + 1];			//域名解析
}TGuiChangeIpcIpReq;

typedef struct
{
	int m_iRetCode;
} TGuiChangeIpcIpRsp;

//前端IPC重启
#define GUI_IPC_REBOOT_CMD_REQ		814  //IPC重启
#define GUI_IPC_REBOOT_CMD_RSP		-814 //回应
typedef struct
{
	int m_iChn;
}TGuiIpcRebootReq;           //重启请求

typedef struct
{
	int m_iRetCode;
} TGuiIpcRebootRsp;

#define GUI_IPC_CNF_EXPORT_REQ		815  //IPC配置导出请求
#define GUI_IPC_CNF_EXPORT_RSP		-815  //IPC配置导出回应
typedef struct
{
	int m_iChn;
 	char m_iDstDevName[GUI_CNF_NAME_LEN + 1];	//导出配置文件磁盘名称
	char m_strFileName[GUI_CNF_NAME_LEN + 1];  //导出配置文件名
}TGuiIpcExportReq;

typedef struct
{
	int m_iRetCode;
}TGuiIpcExportRsp;

#define GUI_IPC_CNF_IMPORT_REQ		816  //IPC导入请求
#define GUI_IPC_CNF_IMPORT_RSP		-816
//仿照以前的导入逻辑，只是在发送时，增加通道号
typedef struct
{
	int m_iChn;
char m_strFileName[GUI_CNF_NAME_LEN + 1]; //导入配置文件名
}TGuiIpcImportReq;

typedef struct
{
	int m_iRetCode;
}TGuiIpcImportRsp;

//IPC升级
#define GUI_IPC_UPDATE_CMD_REQ		817   //IPC升级
#define GUI_IPC_UPDATE_CMD_RSP		-817

typedef struct
{
	int m_iChn;
	char m_strFileName[GUI_SYS_UPDATE_FTP_LEN + 1];  //升级文件名
}TGuiIpcUpdateReq;

typedef struct
{
	int m_iRetCode;
}TGuiIpcUpdateRsp;

#define GUI_IPC_UPDATE_STA_REQ		818
#define GUI_IPC_UPDATE_STA_RSP		-818
//使用旧的结构体TGuiSysUpdateStaReq和TGuiSysUpdateStaRsp

#define GUI_IPC_FILE_UPDATE_NUM_REQ		819	//获取IPC升级文件个数请求
#define GUI_IPC_FILE_UPDATE_NUM_RSP		-819  //回应
//使用旧的结构体TGuiGetSysUpdateFileListRsp和TGuiSysUpdateFileRsp

typedef enum
{
	DEFAULT_ISP = 1,     //IPC高清参数
}EDefaultEnum;

#define GUI_SET_MODULE_DEFAULT_REQ	  830   //恢复默认请求(按功能模块划分)
#define GUI_SET_MODULE_DEFAULT_RSP	 -830   //恢复默认回应(按功能模块划分)
typedef struct
{
	int m_iChn;   //通道号从1开始
	int m_iType;    //见枚举EDefaultEnum
}TGuiSetModuleDefaultReq;

typedef struct
{
	int m_iRetCode;   //0表成功,其余表失败
}TGuiSetModuleDefaultRsp; 

//------------------------------------------------------------------------------
// 视频加密解密
#define GUI_VIDEO_PWD_LEN  31    //密码长度

#define GUI_VDECODE_PWD_OK 		0   //验证成功
#define GUI_VDECODE_PWD_ERR 	1  //验证失败
#define GUI_VDECODE_PWD_ERR1 	2 //刻录过程中加密失败


typedef struct	//设置视频解密密码请求
{
	int m_iPlayId;
	char m_strPwd[GUI_VIDEO_PWD_LEN + 1];
}TGuiVdecodePwdSetReq;

typedef struct
{
	int m_iRetCode;
}TGuiVdecodePwdSetRsp;

typedef struct	//设置视频编码加密密码请求
{
	int m_iChn;	//通道号，预留，默认为0
	char m_strPwd[GUI_VIDEO_PWD_LEN + 1]; //密码
} TGuiVEncodePwdSetReq;

typedef struct
{
	int m_iRetCode;	// 0成功，1失败
} TGuiVEncodePwdSetRsp;

typedef struct	//获取视频加密请求
{
	int m_iChn;	//通道号，预留，默认0
} TGuiVencodePwdGetReq;

typedef struct
{
	int m_iChn;	//通道号，与req中的通道号相等，-1表式失败
	char m_strPwd[GUI_VIDEO_PWD_LEN + 1]; //密码
} TGuiVencodePwdGetRsp;

enum
{
	GUI_AI_DEV_CHN_ALL_ID = 0,   //输入设备通道号，0表示全部通道
	GUI_AO_DEV_ID = 1001,
};

enum
{
	GUI_VOLUMECTRL_REVERSE = 0,
	GUI_VOLUMECTRL_IN,
	GUI_VOLUMECTRL_OUT,
	GUI_VOLUMECTRL_LINEIN,
	GUI_VOLUMECTRL_MICIN,
};

typedef struct		//音量控制请求
{
	int m_iDevId;	//声音输入设通道号(保留字，默认为0表全部通道), GUI_AO_MIC_DEV_ID表ao输出
	int m_iVolume;	//音量大小 0-255(0:静音,255:最大音量, -1表式查询当前音量)
} TGuiVolumeCtrReq;

typedef struct
{
	int m_iDevId;        //扩展高16位为音频控制类型，0保留，1输入，2输出，3-LInein,4-macIn
	//低16位表示通道号，0表全部
	int m_iVolume;	//当前的音量值
} TGuiVolumeCtrRsp;

// 码流传输
typedef struct		//视频请求连接
{
	int m_iPid;	//程序标识
	int m_iChn;	//通道号。 4×n～5×n-1，实时图片流通道（交通终端服务专用），设备总通道数用n来表示。
} TGuiStreamConnectReq;

typedef struct		//视频请求回应
{
	int m_iKey;	//共享内存key值(负值表式失败)
	int m_iSize;	//共享内存大小
} TGuiStreamConnectRsp;

typedef struct		//视频断开请求
{
	int m_iPid;	//程序标识
	int m_iChn;	//通道号。4×n～5×n-1，实时图片流通道（交通终端服务专用），设备总通道数用n来表示。
} TGuiStreamDisConnectReq;

typedef struct		//视频断开回应
{
	int m_iRetCode;	//返回码，0表式成功
} TGuiStreamDisConnectRsp;

typedef struct		//码流传输通道
{
	int m_iBase;
	int m_iTop;
	int m_iSize;
	char m_cBuf[0];
} TGuiStreamChannel;

typedef struct						//码流数据信息
{
	unsigned char m_ucFrameType;	//帧数据类型, 0x00:I帧,0x01:P帧,0xFF:填充数据(直接丢弃)
	unsigned char m_ucFrameRate;		//帧率
	unsigned char m_ucR1;			//保留字
	unsigned char m_ucR2;			//保留字
	unsigned int m_iFrameLenght;		//帧长度
	unsigned char m_ucStream[0];		//帧数据
} TGuiStreamFrame;


typedef struct		//强制I帧请求
{
	int m_iChn;		//通道号,0表示全部
} TGuiForceIframeReq;

typedef struct
{
	int m_iRetCode;
} TGuiForceIframeRsp;


//=================== 码流传输 ====================
//从外挂来的视频发给内核
#define GUI_PUSH_STREAM_CONNECT_REQ  2004  //设备接收外部push码流连接请求
#define GUI_PUSH_STREAM_CONNECT_RSP  -2004 //设备接收外部push码流连接回应
typedef struct
{
	int m_iPid; 	//程序标识
	int m_iChn;  //本地通道号，设备用那个通道预览PUSH来的码流
}TGuiPushStreamConnectReq;

typedef struct
{
	int m_iKey;	//共享内存key值(负值表式失败)
	int m_iSize;	//共享内存大小
}TGuiPushStreamConnectRsp;

#define GUI_PUSH_STREAM_DISCONNECT_REQ  2005  //设备断开连接push码流请求
#define GUI_PUSH_STREAM_DISCONNECT_RSP  -2005 //设备断开连接push码流回应

typedef struct		//视频断开请求
{
	int m_iPid;	//程序标识
	int m_iChn;	//本地通道号，设备用那个通道预览PUSH来的码流
} TGuiPushStreamDisConnectReq;

typedef struct		//视频断开回应
{
	int m_iRetCode;	//返回码，0表式成功
} TGuiPushStreamDisConnectRsp;
#if 0
//共用原来已有的结构体
typedef struct		//码流传输通道
{
	int m_iBase;
	int m_iTop;
	int m_iSize;
	char m_cBuf[0];
} TGuiStreamChannel;

typedef struct						//码流数据信息
{
	unsigned char m_ucFrameType;	//帧数据类型, 0x00:I帧,0x01:P帧,0xFF:填充数据(直接丢弃)
	unsigned char m_ucFrameRate;		//帧率
	unsigned char m_ucR1;			//保留字
	unsigned char m_ucR2;			//保留字
	unsigned int m_iFrameLenght;		//帧长度
	unsigned char m_ucStream[0];		//帧数据
} TGuiStreamFrame;
#endif

//==============智能分析=================================
//录像定义

#define MAX_GUI_RULE_NAME_LEN		16
#define MAX_GUI_POLYGON_POINT_NUM	8
#define MAX_GUI_FDEPTH_LINE_NUM 	3

//智能分析通用类型
typedef struct
{
	int m_iX;
	int	m_iY;
} TGuiVcaPoint;

typedef struct
{
	TGuiVcaPoint 	m_stStart;	
	TGuiVcaPoint 	m_stEnd;	
} TGuiVcaLine;

typedef struct
{
	int 			m_iPointNum;
	TGuiVcaPoint 	m_stPoint[MAX_GUI_POLYGON_POINT_NUM];   	
} TGuiVcaPolygon;


//智能分析通道使能设置/获取
#define GUI_GET_VCACHN_CNF_REQ				 900		//获取智能分析通道使能请求
#define GUI_GET_VCACHN_CNF_RSP				-900		//获取智能分析通道使能回应

typedef struct
{
	int m_iChannelNo;
} TGuiGetVcaChnEnableReq;  //获取智能分析通道使能请求

enum
{
	GUI_VCA_DISABLE = 0,    //不使能智能分析
	GUI_VCA_USE_LOCAL,      //使能本地通道智能分析
	GUI_VCA_USE_REMOTE,		//使能前端通道智能分析
}; //智能分析使能标识

typedef struct
{
	int m_iChannelNo;
	int m_iVcaEnable;		//智能分析使能标识
} TGuiGetVcaChnEnableRsp;

#define GUI_SET_VCACHN_CNF_REQ				 901		//设置智能分析通道使能请求
#define GUI_SET_VCACHN_CNF_RSP				-901		//设置智能分析通道使能回应

typedef struct
{
	int m_iChannelNo;
	int m_iVcaEnable;		//智能分析使能位		0不使能，1使能本地，2使能前端
} TGuiSetVcaChnEnableReq;

#define GUI_SET_VCACHN_OK		0
#define GUI_SET_VCACHN_ERR		1
typedef struct
{
	int m_iRetCode;			//返回值,0表示成功，其余值表失败
} TGuiSetVcaChnEnableRsp;

#define GUI_SET_VCA_ACTION_REQ	913		//暂停或者恢复VCA请求
#define GUI_SET_VCA_ACTION_RSP	-913	//回应

typedef enum
{
	GUI_VCA_PAUSE = 1,
	GUI_VCA_RESUME,
}TGuiVcaActEnum;

typedef struct
{
	int m_iChn;
	int m_iAct;			//见枚举TGuiVcaActEnum
}TGuiSetVcaChnActReq;

typedef struct
{
	int m_iRetCode;
}TGuiSetVcaChnActRsp;


//智能分析参数设置/获取
#define GUI_GET_VCAPARA_CNF_REQ		 		 902		//获取智能分析基本参数请求
#define GUI_GET_VCAPARA_CNF_RSP				-902		//获取智能分析基本参数回应

typedef struct
{
	//int m_iVcaEnable;				//智能分析使能位		0-不使能此通道智能分析	1-使能此通道智能分析
	int m_iVcaVideoSize;			//智能分析库视频大小	0-QCIF 1-HCIF 2-FCIF 3-HD1 4-FD1 目前只支持QCIF/FCIF
	int m_iDisplayTarget;			//视频叠加目标			0-不叠加	  1-叠加		
	int m_iDisplayTrace;			//视频叠加轨迹			0-不叠加	  1-叠加
	int m_iDisplayAllTargetTrace;	//是否叠加所有目标和轨迹0-只叠加触发警情的目标、轨迹	1-叠加所有目标、轨迹
	int m_iTargetColor;				//目标轨迹颜色			0:默认 1:红色 2:绿色 3:黄色 4:蓝色 5:紫色 6:青色 7:黑色 8:白色
	int m_iTargetAlarmColor;		//目标轨迹报警颜色		0:默认 1:红色 2:绿色 3:黄色 4:蓝色 5:紫色 6:青色 7:黑色 8:白色
	int m_iTraceLength;				//轨迹长度				0~40
} TGuiVcaParaCnf;

typedef struct
{
	int m_iEnableAdvanced;	//智能分析高级参数使能	0-不使能	1-使能					
	int m_iTargetMinSize;	//目标最小像素数
	int m_iTargetMaxSize;	//目标最大像素数
	int m_iTargetMinWidth;	//最小宽度
	int m_iTargetMaxWidth;	//最大宽度
	int m_iTargetMinHeight;	//最小高度
	int m_iTargetMaxHeight;	//最大高度
	int m_iTargetMinWHRatio;//目标的最小长宽比*100
	int m_iTargetMaxWHRatio;//目标的最大长宽比*100
	int m_iTargetMinSpeed;	//目标最小像素速度
	int m_iTargetMaxSpeed;	//目标最大像素速度
	int m_iSensitivity;		//目标输出灵敏度级别	0:低灵敏度 1:中灵敏度 2:高灵敏度
} TGuiVcaAdvParaCnf;

typedef struct 
{
	TGuiVcaLine m_stRefLine;		// 垂直线段坐标(unit: pixel)
	int 		m_s32RefLen;		// 线段长度(unit: cm)
} TGuiVcaFdeLine;	// 景深线结构

typedef struct
{
	unsigned int	m_i32NumUsed; 							// 样本数目 大于等于2
	TGuiVcaFdeLine 	m_stLines[MAX_GUI_FDEPTH_LINE_NUM];   	// 样本数目
} TGuiVcaFdepthMeasure;  

typedef struct
{
	unsigned int 			m_iEnableFdepth;	//启用标识
	TGuiVcaFdepthMeasure 	m_stMeasure;		//标定数据
} TGuiVcaFdeParaCnf;

typedef struct	//获取智能分析参数配置(通用/高级)请求
{
	int m_iChannelNo;						//智能分析通道号
} TGuiGetVcaParaReq;

typedef struct	//获取智能分析参数配置(通用/高级)回应	
{
	int m_iChannelNo;		
	TGuiVcaParaCnf		m_tVcaParaCnf;		//智能分析通用配置参数
	TGuiVcaAdvParaCnf	m_tVcaAdvParaCnf;	//智能分析高级配置参数
	TGuiVcaFdeParaCnf	m_tVcaFdeParaCnf;	//智能分析标定配置参数
} TGuiGetVcaParaRsp;

#define GUI_SET_VCAPARA_CNF_REQ		 		 903		//设置智能分析基本参数请求	
#define GUI_SET_VCAPARA_CNF_RSP				-903		//设置智能分析基本参数回应

typedef struct	//设置智能分析参数澹(通用/高级)请求
{
	int m_iChannelNo;		
	TGuiVcaParaCnf		m_tVcaParaCnf;
	TGuiVcaAdvParaCnf	m_tVcaAdvParaCnf;
	TGuiVcaFdeParaCnf	m_tVcaFdeParaCnf;
} TGuiSetVcaParaReq; 

#define GUI_SET_VCAPARA_OK		0
#define GUI_SET_VCAPARA_ERR		1
typedef struct	//设置智能分析参数澹(通用/高级)回应
{
	int m_iRetCode;							//返回值,0表示成功，其余值表失败
} TGuiSetVcaParaRsp;

//智能分析规则事件参数设置/获取
#define GUI_GET_VCAEVENT_CNF_REQ			 904		//获取智能分析事件参数请求
#define GUI_GET_VCAEVENT_CNF_RSP			-904		//获取智能分析事件参数回应

typedef enum  //智能回放中的回放类型也摘取了此枚举，见dvr.h Line313
{
	GUI_TRIPWIRE = 0,						//绊线事件
	GUI_DBTRIPWIRE,							//双绊线事件
	GUI_PERIMETER,							//周界事件
	GUI_LOITER,								//徘徊事件
	GUI_PARKING,							//停车事件
	GUI_RUN,								//奔跑事件
	GUI_HIGH_DENSITY,						//人员密度事件
	GUI_ABANDUM,							//遗弃物事件
	GUI_OBJSTOLEN,							//被盗物事件
	GUI_FACESPOT,           				// 人脸识别
	GUI_AVD = 10,							//视频诊断
	GUI_CAP = 11,							//目标跟踪
	GUI_CAP_td = 12,			/*td目标跟踪*/
	GUI_CPC = 13,				/*人流统计*/
	GUI_CDD = 14,				/*人员聚集*/
	GUI_SVD = 15,				/*离岗检测*/
	GUI_AAD = 16,				/*音频检测*/	
	GUI_VFD = 17,				/*人脸检测*/
	GUI_LPR = 18,				/*车牌检测*/
	GUI_MAX
} EGuiVcaEventType;

typedef struct
{
	int				m_iEventValid;		//是否使能单绊线事件
	int				m_iTargetTypeCheck;	//目标检测类型
	int				m_iMinDistance;		//最小距离
	int				m_iMinTime;			//最短时间
	int				m_iDisplayRule;		//是否显示规则
	int				m_iDisplayStat;		//是否显示报警计数统计
	int				m_iColor;			//单绊线颜色	0:默认 1:红色 2:绿色 3:黄色 4:蓝色 5:紫色 6:青色 7:黑色 8:白色
	int				m_iAlarmColor;		//报警绊线颜色	0:默认 1:红色 2:绿色 3:黄色 4:蓝色 5:紫色 6:青色 7:黑色 8:白色
	int				m_iType;			//穿越类型		0:单向 1:双向
	int				m_iTDirection;		//禁止穿越方向	0~359度
	TGuiVcaLine		m_stLine;			//单绊线坐标
} TGuiVcaTripwire;

typedef struct
{
	int				m_iEventValid;		//是否使能周界事件
	int				m_iDisplayRule;		//是否显示规则
	int				m_iDisplayStat;		//是否显示报警计数统计
	int				m_iColor;			//周界颜色		0:默认 1:红色 2:绿色 3:黄色 4:蓝色 5:紫色 6:青色 7:黑色 8:白色
	int				m_iAlarmColor;		//报警周界颜色	0:默认 1:红色 2:绿色 3:黄色 4:蓝色 5:紫色 6:青色 7:黑色 8:白色
	int				m_iTargetTypeCheck;	//目标检测类型
	int				m_iMode;			//周界检测模式	0:入侵 1:进入 2:离开
	int 			m_iMinDistance;		//最小距离					
	int 			m_iMinTime;			//最短时间				
	int				m_iType;			//是否做方向限制
	int 			m_iDirection;		//禁止方向角度	0~359度
	TGuiVcaPolygon	m_stRegion;			//周界坐标
} TGuiVcaPermeter;


typedef struct
{	
	int 			m_iEventValid;		//是否使能人脸侦测事件
	int 			m_iDisplayRule;		//是否显示规则
	int 			m_iDisplayStat;		//是否显示报警计数统计
	int				m_iColor;			//侦测轮廓颜色		0:默认 1:红色 2:绿色 3:黄色 4:蓝色 5:紫色 6:青色 7:黑色 8:白色
	int 			m_iAlarmColor;		//报警侦测轮廓颜色	0:默认 1:红色 2:绿色 3:黄色 4:蓝色 5:紫色 6:青色 7:黑色 8:白色
	TGuiVcaPolygon	m_stRegion;			//人脸侦测区域坐标
} TGuiVcaFace;

typedef struct
{
	int				m_iEventValid;		//是否使能视频诊断事件
	int				m_iDisplayStat;		//是否显示报警计数统计
	int				m_iCheckTime;		//事件有效事件阀值，当事件发生超过此阀值时，认为警情发生
} TGuiVcaDiagnose;


typedef struct
{
    int				m_iEventValid;
	int				m_iDisplayRule;
	int				m_iDisplayStat;
	int				m_iColor;
	int				m_iAlarmColor;	
	int				m_iMinTime;
	int				m_iMinSize;
	int 			m_iMaxSize;	
	int				m_iChildAreaNum;    //无效子区域个数
	TGuiVcaPolygon	m_stMainRegion;	    //最外的多边形区域	
    TGuiVcaPolygon	m_stRegionA;	    //子区域A
	TGuiVcaPolygon	m_stRegionB;        //子区域B
	TGuiVcaPolygon	m_stRegionC;        //子区域C
}TGuiVcaLeave;
#define TGuiVcaLost TGuiVcaLeave


typedef struct
{
	TGuiVcaTripwire	m_tVcaTripwireCnf;		//单绊线事件具体配置参数
	TGuiVcaPermeter m_tVcaPermeterCnf;		//周界事件具体配置参数
	TGuiVcaFace		m_tVcaFaceCnf;			//人脸侦测事件具体配置参数
	TGuiVcaDiagnose m_tVcaDiagnoseCnf;		//视频诊断事件具体配置参数
	TGuiVcaLeave    m_tVcaLeaveCnf;         //物品遗留事件具体配置参数
	TGuiVcaLost     m_tVcaLostCnf;          //物品丢失事件具体配置参数
} TGuiVcaEventCnf;


typedef struct	//获取智能分析具体规则、事件参数请求
{
	int 				m_iChannelNo;		//通道号		
	int 				m_iRulesNo;			//事件号
	EGuiVcaEventType 	m_eEventType;		//事件类型			
} TGuiGetVcaEventReq;

typedef struct	//获取智能分析具体规则、事件参数回应
{
	int 				m_iChannelNo;		//通道号
	int 				m_iRulesNo;			//事件号
	unsigned char		m_cRuleName[MAX_GUI_RULE_NAME_LEN + 1];	//事件名称
	int 				m_iRuleEnable;		//规则是否使能
	EGuiVcaEventType 	m_eEventType;		//事件类型			
	TGuiVcaEventCnf 	m_tVcaEventCnf;		//事件具体配置参数
} TGuiGetVcaEventRsp;

#define	GUI_SET_VCAEVENT_CNF_REQ	 		 905		//设置智能分析事件参数请求
#define GUI_SET_VCAEVENT_CNF_RSP			-905		//设置智能分析事件参数回应

typedef struct	//设置智能分析具体规则、事件参数请求
{
	int 				m_iChannelNo;
	int 				m_iRulesNo;
	unsigned char		m_cRuleName[MAX_GUI_RULE_NAME_LEN + 1];
	int 				m_iRuleEnable;
	EGuiVcaEventType 	m_eEventType;					
	TGuiVcaEventCnf 	m_tVcaEventCnf;
} TGuiSetVcaEventReq;

#define GUI_SET_VCAEVENT_OK		0
#define GUI_SET_VCAEVENT_ERR	1
typedef struct	//设置智能分析具体规则、事件参数回应
{
	int m_iRetCode;			//返回值,0表示成功，其余值表失败
} TGuiSetVcaEventRsp;

//智能分析报警计数统计获取
#define GUI_GET_VCA_ALARM_COUNT_TOTAL_REQ	 	906		//获取智能分析报警计数请求
#define GUI_GET_VCA_ALARM_COUNT_TOTAL_RSP		-906		//获取智能分析报警计数回应

typedef struct
{
	int 				m_iChannelNo;
	int 				m_iRulesNo;
	EGuiVcaEventType 	m_eEventType;
} TGuiVcaAlarmCountCnf;

typedef struct	
{
	TGuiVcaAlarmCountCnf m_VcaAlarmCountCnf;
} TGuiGetVcaAlarmCountTotalReq;

#define GUI_GET_ALARM_COUNT_TOTAL_OK	0
#define GUI_GET_ALARM_COUNT_TOTAL_ERR	1
typedef struct
{
	TGuiVcaAlarmCountCnf 	m_VcaAlarmCountCnf;
	int 					m_iVcaAlarmCount;
} TGuiGetVcaAlarmCountTotalRsp;

//智能分析报警计数清零设置
#define GUI_SET_VCA_ALARM_COUNT_CLEAN_REQ		 907		//设置智能分析报警清零请求
#define GUI_SET_VCA_ALARM_COUNT_CLEAN_RSP		-907		//设置智能分析报警清零回应

typedef struct
{
	TGuiVcaAlarmCountCnf m_VcaAlarmCountCnf;
} TGuiSetVcaCleanAlarmCountReq;

#define GUI_SET_ALARM_COUNT_CLEAN_OK	0
#define GUI_SET_ALARM_COUNT_CLEAN_ERR	1
typedef struct
{
	int m_iRetCode;			//返回值,0表示成功，其余值表失败
} TGuiSetVcaCleanAlarmCountRsp;

//智能分析报警状态获取
#define GUI_GET_VCA_ALARM_STATE_MSG_REQ		 	908		//获取智能分析报警消息请求
#define GUI_GET_VCA_ALARM_STATE_MSG_RSP			-908		//获取智能分析报警消息回应

typedef	struct	
{
	TGuiVcaPoint 		m_stLeftTopPoint;	
	TGuiVcaPoint 		m_stRightBottomPoint;
} TGuiVcaTargetCoordinate;

typedef struct
{
	int 					m_tTime;			//警情发生时间
	int 					m_iState;			//报警状态
	unsigned int 			m_uiTargetId;		//目标ID
	int						m_iTargetType;		//目标类型
	TGuiVcaTargetCoordinate	m_stTargetRegion;	//目标区域坐标
	int						m_iTargetSpeed;		//目标速度
	int						m_iTargetDirection;	//目标运动方向	0~359
} TGuiVcaAlarmStateMsg;

typedef struct	//获取智能分析时时报警个数回应
{
	int 	m_iTol;
} TGuiGetVcaAlarmStateMsgRsp;

typedef struct	
{		
	int 					m_iChannelNo;		//通道号
	int 					m_iRulesNo;			//规则号
	EGuiVcaEventType		m_eEventType;		//事件类型
	TGuiVcaAlarmStateMsg	m_VcaAlarmStateMsg;	//报警状态消息
} TGuiGetVcaAlarmStateMsg;


#define GUI_GET_VCAALM_STATE_MSG_REQ_EX		 	4908		//获取智能分析报警消息请求(新加)
#define GUI_GET_VCAALM_STATE_MSG_RSP_EX			-4908		//获取智能分析报警消息回应(新加)

typedef struct	//获取智能分析时时报警个数回应
{
	int 	m_iTotal;
} TGuiGetVcaAlarmStateMsgRspEx;

typedef struct	
{		
	int 			m_iChannelNo;		//通道号
	int 			m_iRulesNo;			//规则号
	unsigned char		m_cRuleName[MAX_GUI_RULE_NAME_LEN + 1]; //规则名
	EGuiVcaEventType	m_eEventType;		//事件类型
	TGuiVcaAlarmStateMsg	m_VcaAlarmStateMsg;	//报警状态消息
} TGuiGetVcaAlarmStateMsgEx;

#define	GUI_GET_VCA_ALARM_REQ				 909		//获取智能分析报警配置信息请求
#define GUI_GET_VCA_ALARM_RSP				-909		//获取智能分析报警配置信息回应
typedef	struct											//报警配置
{
	int m_iEnable;										//报警使能 				0/1
	int m_iSoundEnable;									//声音使能 				0/1,次低8位(8~15)，表示联动发邮件
	int m_iDisplayEnable;								//屏幕显示使能 			0/1
	int m_iToServerEnalbe;								
	int m_iVideoEnable[GUI_DVR_CHN_CNT];				//联动各通道录像使能 	0/1, (0/1 按位顺序操作，数组元素第1位分别对应1,2,3,4,5….通道，依次循环，其他也按照此处理)
	int m_iPicEnable[GUI_DVR_CHN_CNT];					//联动各通道图片抓拍使能
	int m_iAlarmOutEnable[GUI_DVR_ALARM_OUT_CNT];		//联动报警输出使能 		0/1
	TGuiTimePair m_tTimes[7][GUI_ALARM_TIME_PER_DAY];	//每天四个时段，一周7天
	TGuiPtzLinkage m_tPtzLinkage[GUI_DVR_CHN_CNT];		//ptz联动
} TGuiVcaAlarmCnf;

typedef struct	//获取智能分析报警信息参数请求
{
	int 						m_iChannelNo;			//通道号
	int 						m_iRulesNo;				//规则号
	EGuiVcaEventType			m_eEventType;			//事件类型
} TGuiGetVcaAlarmReq;

typedef struct	//获取智能分析报警信息参数回应
{
	int 						m_iChannelNo;			//通道号
	int 						m_iRulesNo;				//规则号
	EGuiVcaEventType			m_eEventType;			//事件类型
	TGuiVcaAlarmCnf 			m_tAlarmCnf;			//智能分析报警配置	
} TGuiGetVcaAlarmRsp;

#define GUI_SET_VCA_ALARM_REQ				 910		//设置智能分析报警配置信息请求
#define GUI_SET_VCA_ALARM_RSP				-910		//设置智能分析报警配置信息回应

typedef struct	//设置智能分析报警信息参数请求
{
	int 						m_iChannelNo;			//通道号
	int 						m_iRulesNo;				//规则号
	EGuiVcaEventType			m_eEventType;			//事件类型
	TGuiVcaAlarmCnf 			m_tAlarmCnf;			//智能分析报警配置
} TGuiSetVcaAlarmReq;

#define GUI_SET_VCA_ALARM_OK		0
#define GUI_SET_VCA_ALARM_ERR		1
typedef struct	//设置智能分析报警信息参数回应
{
	int m_iRetCode;
} TGuiSetVcaAlarmRsp;


#define GUI_SET_VCA_REBOOT_REQ				 911		//重启智能库请求
#define GUI_SET_VCA_REBOOT_RSP				-911		//重启智能库回应

typedef struct
{
	int	m_iChannel;			//因为改变了某个通道参数而重启的通道号
} TGuiSetVcaRebootReq;

#define GUI_SET_VCA_REBOOT_OK	0
#define GUI_SET_VCA_REBOOT_ERR	1

typedef struct
{
	int m_iRetCode;			//返回值,0表示成功，其余值表失败
} TGuiSetVcaRebootRsp;

//获取智能分析规则事件使能状态
#define GUI_GET_VCA_RULE_EVENT_ENABLE_REQ	 	912		//获取智能分析规则事件使能状态请求
#define GUI_GET_VCA_RULE_EVENT_ENABLE_RSP		-912		//获取智能分析规则事件使能状态回应
typedef struct
{
	int m_iChannel;
	int m_iRulesNo;
} TGuiGetVcaRuleEventEnableReq;

typedef struct
{
	int m_iRuleEnable;			//规则是否使能
	int m_iTripwireEnable;		//单绊线事件是否使能
	int m_iPerimeterEnable;		//周界事件是否使能
	int m_iFaceEnable;			//人脸侦测事件是否使能
	int m_iDiagnoseEnable;		//视频诊断事件是否使能
	int m_iLeaveEnable;         //物品遗留事件是否使能
	int m_iLostEnabe;           //物品丢失事件是否使能
} TGuiVcaRuleEnableMsg;

typedef struct
{
	int 					m_iChannel;
	int 					m_iRulesNo;
	TGuiVcaRuleEnableMsg	m_iEnableMsg;
} TGuiGetVcaRuleEventEnableRsp;

/************新的智能分析算法***************************/

//启用哪个智能分析算法
typedef enum     //VCA算法类型
{
	GUI_AL_BEHAVIOUR = 0,		// 0 行为分析
	GUI_AL_FOLLOW,				// 1 跟踪
	GUI_AL_FACE,				// 2 人脸检测
	GUI_AL_COUNT,				// 3 人数统计
	GUI_AL_CAP,					// 4 视频诊断
	GUI_AL_CARNO,				// 5 车牌识别
	GUI_AL_AUDIOEXP,			// 6 音频异常检测
	GUI_AL_LEAVE,				// 7 离岗检测
	GUI_AL_GATHER,				// 8  人群聚集
}EVCATYPE;

#define GUI_GET_VCATYPE_REQ	4500		//获取智能分析启用类型请求
#define GUI_GET_VCATYPE_RSP	-4500		//获取智能分析启用类型回应

typedef struct
{
	int m_iChn;		//通道号，从1开始，0表示全部
	int m_iSceneId;	//场景 0~15,高端球有用
	unsigned char m_strSceneName[GUI_PUBLIC_LEN_63 + 1];	//场景名称
}TGuiGetVcaTypeReq;

typedef struct
{
	int m_iChn;
	int m_iSceneId;
	int m_iVcaType;		//按位来 详见枚举EVCATYPE，1表示启用，0表示不启用
	unsigned char m_strSceneName[GUI_PUBLIC_LEN_63 + 1];
}TGuiGetVcaTypeRsp;

#define GUI_SET_VCATYPE_REQ	4501	//设置智能分析启用类型请求
#define GUI_SET_VCATYPE_RSP	-4501	//回应
typedef struct
{
	int m_iChn;
	int m_iSceneId;
	int m_iVcaType;		//按位来 详见枚举EVCATYPE，1表示启用，0表示不启用
	unsigned char m_strSceneName[GUI_PUBLIC_LEN_63 + 1];
}TGuiSetVcaTypeReq;

typedef struct
{
	int m_iRetCode;		// 0表成功 其余表示失败
}TGuiSetVcaTypeRsp;

//天地行为分析算法
#define MAX_IGNORE_REGION_NUM	3
#define MAX_PARAM_SIZE			2048		/*联合体参数数据最大时为2048个字节*/

typedef struct		/* 单绊线参数*/
{
	int				m_iEventValid;			/*事件使能*/
	int				m_iTargetTypeCheck;		/*检测类型: 1-人、2-车、3-人车、0-无限制*/
	int				m_iType;				/*0-单向绊线或1-双向绊线*/
	int				m_iDirection;			/*绊线报警方向*/
	int				m_iMinDistance;			/*设置检测最小像素: 默认0*/
	int				m_iMinTime;				/*报警时间: 默认0*/
	int				m_iDisplayRule;			/*是否显示规则线: 0 - 不显示	1-显示 */
	int				m_iDisplayStat;			/*是否显示报警计数: 0 - 不显示	1-显示*/
	int				m_iAlarmCount;			/*报警计数: 该个数将在屏幕上显示*/
	int				m_iColor;				/*未报警时规则线颜色0:默认 1:红色 2:绿色 3:黄色 4:蓝色 5:紫色 6:青色 7:黑色 8:白色*/
	int				m_iAlarmColor;			/*报警时的规则线颜色0:默认 1:红色 2:绿色 3:黄色 4:蓝色 5:紫色 6:青色 7:黑色 8:白色*/
	TGuiVcaLine		m_stLine;				
	int				m_iMinSize;
	int				m_iMaxSize;
	TGuiVcaPolygon	m_stLineEx;
}TGuiVcaParamTripwire;

typedef struct		/* 双绊线参数*/
{
	int				m_iEventValid;			/*事件使能*/
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
	int				m_iMinDBTime;
	int				m_iMaxDBTime;
	int				m_iMaxSize;
	int				m_iMinSize;
	TGuiVcaPolygon	m_stLine[2];
}TGuiVcaParamDBTripwire;


typedef struct		/*周界参数*/
{
	int				m_iEventValid;			/*事件使能*/
	int				m_iTargetTypeCheck;		/*检测类型: 1-人、2-车、3-人车、0-无限制*/
	int				m_iMode;				/*将侧模式: 入侵、进入、离开*/
	int				m_iType;				/*有无方向*/
	int 			m_iDirection;			/*方向限制*/
	int				m_iMinDistance;			/*报警最小像素距离: 默认0	*/
	int 			m_iMinTime;				/*报警最短时间: 默认0*/
	int				m_iDisplayRule;			/*是否显示规则线: 0 - 不显示	1-显示 */
	int				m_iDisplayStat;			/*是否显示报警计数: 0 - 不显示	1-显示*/
	int				m_iAlarmCount;			/*报警计数: 该个数将在屏幕上显示*/
	int				m_iColor;				/*未报警时规则线颜色*/
	int				m_iAlarmColor;			/*报警时的规则线颜色*/
	int				m_iMinSize;
	int				m_iMaxSize;
	TGuiVcaPolygon	m_stRegion;
}TGuiVcaParamPerimeter;


typedef struct		/*物品遗留看护参数*/
{
	int				m_iEventValid;		/*事件使能*/
	int				m_iSceneType;		/*场景类型  0室外 1室内*/
	int				m_iCameraType;		/*相机类型  0普通相机 1针孔相机*/
	int				m_iDisplayRule;		/*是否显示规则线: 0 - 不显示	1-显示 */
	int				m_iDisplayStat;		/*是否显示报警计数: 0 - 不显示	1-显示*/
	int				m_iAlarmCount;		/*报警计数: 该个数将在屏幕上显示*/
	int				m_iColor;			/*未报警时规则线颜色*/
	int				m_iAlarmColor;		/*报警时的规则线颜色*/
	int				m_iTimeMin;			/*报警检测时间，物品遗失时间必须要大于此阀值*/
	int				m_iSizeMin;			/*被检测物体的最小像素数*/
	int				m_iSizeMax;			/*被检测物体的最大像素数*/
	TGuiVcaPolygon	m_stMainRegion;		/*检测区域结构*/
	TGuiVcaPolygon	m_stIgnoreRegion[MAX_IGNORE_REGION_NUM]; /*忽略检测的小框*/
}TGuiVcaParamOSC;


typedef struct		/*徘徊检测配置参数*/
{	
	int				m_iEventValid;			/*事件使能*/
	int				m_iMinTime;				/*最小时间*/
	int				m_iAlarmCount;			/*报警计数: 该个数将在屏幕上显示*/
	int				m_iSensitivity;			/*yansm 20140222 add 灵敏度*/
	int				m_iMinRange;			/*yansm 20140222 add 徘徊最小距离百分比*/
	int				m_iColor;		 		/*区域颜色*/
	int				m_iAlarmColor;			/*报警时区域的颜色*/
	int				m_iDisplayRule;			/*规则是否显示*/
	int				m_iDisplayStat;			/*报警计数是否显示*/
	TGuiVcaPolygon	m_stRegion;
	int				m_iMinBoundy;
}TGuiVcaParamOabLoi;


typedef struct		/*奔跑检测配置参数*/
{
	int				m_iEventValid;			/*事件使能*/
	int				m_iColor;				/*区域颜色*/
	int				m_iAlarmColor;			/*报警时区域的颜色*/
	int				m_iDisplayRule;			/*规则是否显示*/
	int				m_iDisplayStat;			/*报警计数是否显示*/
	int				m_iMaxDistance;			/*人奔跑最大距离*/
	int				m_iMinDistancePreS;		/*每秒最小移动距离*/
	int				m_iAlarmCount;			/*报警计数: 该个数将在屏幕上显示*/
	int				m_iMinSize;
	int				m_iMaxSize;
	TGuiVcaPolygon	m_stRegion;
}TGuiVcaParamRun;

typedef struct		/*停车检测配置参数*/
{
	int				m_iEventValid;			/*事件使能*/
	int				m_iColor;				/*区域颜色*/
	int				m_iAlarmColor;			/*报警时区域的颜色*/
	int				m_iDisplayRule;			/*规则是否显示*/
	int				m_iDisplayStat;			/*报警计数是否显示*/
	int				m_iAlarmTime;			/*触发停车报警的持续时间*/
	int				m_iAlarmCount;			/*报警计数: 该个数将在屏幕上显示*/
	int				m_iMinSize;
	int				m_iMaxSize;
	int				m_ifThVeloc;
	TGuiVcaPolygon	m_stRegion;
}TGuiVcaParamPark;

/**********           行为分析事件参数        **********/

//获取行为分析规则事件使能状态
#define GUI_GET_VCA_ANALY_ENABLE_REQ		4522		//获取智能分析规则事件使能状态请求
#define GUI_GET_VCA_ANALY_ENABLE_RSP		-4522		//获取智能分析规则事件使能状态回应

typedef struct
{
	int 	m_iChannel;		//通道号
	int		m_iSceneId;		//场景编号
	int		m_iRulesNo;		//规则号
} TGuiGetVcaAnalyRuleEventEnableReq;

typedef struct
{
	int 	m_iChannel;				//通道号
	int		m_iSceneId;				//场景编号
	int		m_iRuleEnable;			//规则是否使能
	int 	m_iEventType;			//规则对应的哪条事件生效，bit EGuiVcaEventType按位表示，1启用
} TGuiVcaAnalyEnableMsg;

typedef struct
{
	int						m_iChannel;
	int						m_iRulesNo;
	TGuiVcaAnalyEnableMsg 	m_iEnableMsg;
} TGuiGetVcaAnalyRuleEventEnableRsp;

typedef union
{
	TGuiVcaParamTripwire		m_stTripwire;		/*穿越绊线参数*/
	TGuiVcaParamDBTripwire		m_stDBTripwire;		/*双绊线*/
	TGuiVcaParamPerimeter		m_stPerimeter;		/*周界参数*/
	TGuiVcaParamOSC				m_stoscer;			/*物品看护参数*/
	TGuiVcaParamOabLoi			m_stOabLoi;			/*徘徊*/
	TGuiVcaParamRun				m_stRun;			/*奔跑(快速移动)*/
	TGuiVcaParamPark			m_stPark;			/*停车*/
	/*该联合体的最大空间。以后添加的智能分析总参数不能超过该类型大小*/
	char						m_s8MaxSize[MAX_PARAM_SIZE];
}UGuiVcaEventParam;	

#define GUI_GET_VCAANALYSE_CNF_REQ	 4502	//获取行为分析配置请求
#define GUI_GET_VCAANALYSE_CNF_RSP	-4502	//获取行为分析配置回应
typedef  struct 
{
	int 				m_iChannelNo;		//通道号		
	int					m_iSceneId;			//场景编号
	int 				m_iRulesNo;			//规则号
	EGuiVcaEventType 	m_eEventType;		//事件类型
}TGuiGetVcaAnalyReq;

typedef struct
{
	int 					m_iChannelNo;		//通道号
	int						m_iSceneId;			//场景编号
	unsigned char 			m_strSceneName[GUI_PUBLIC_LEN_63 + 1]; //场景名称
	int 					m_iRulesNo;			//规则号
	unsigned char			m_cRuleName[MAX_GUI_RULE_NAME_LEN + 1];	//事件名称
	int 					m_iRuleEnable;		//规则是否使能
	EGuiVcaEventType 		m_eEventType;		//事件类型	
	UGuiVcaEventParam 	m_stEventParam;			// 行为分析事件参数
} TGuiGetVcaAnalyRsp;

#define GUI_SET_VCAANALYSE_CNF_REQ	 4503	//设置行为分析配置请求
#define GUI_SET_VCAANALYSE_CNF_RSP	-4503	//设置行为分析配置回应
typedef  TGuiGetVcaAnalyRsp  TGuiSetVcaAnalyReq;

typedef struct
{
	int m_iRetCode;	//0表成功, 其余表示失败
} TGuiSetVcaAnalyRsp;

//************人群聚集*********************//
typedef struct 
{
	TGuiVcaPoint m_LeftUp;
	TGuiVcaPoint m_RightDown;
}TGuiVcaRect_Ex;

typedef struct		/*人员聚集CDD配置参数*/
{
	int				m_iStructSize;
	int				m_iSensitivity;		/*灵敏度*/
	int				m_iFlag;			/*暂时保留*/
	int				m_iRegionNum;		/*区域个数*/
	TGuiVcaRect_Ex	m_stRegion[4];		/*检测区域,最大4个*/
	int				m_iTimes;			/*持续时间*/
	TGuiVcaPolygon	m_stMainRegion;		/*修改为多边形*/
	int				m_iDisplayRule;		/*规则是否显示*/
	int				m_iDisplayStat;		/*报警计数是否显示*/
	int 			m_iColor;
	int 			m_iAlarmColor;
}TGuiVcaParamCDD;

#define GUI_GET_VCACCD_REQ		 		4504	//获取人群聚集参数请求
#define GUI_GET_VCACCD_RSP				-4504	//获取人群聚集参数回应
typedef struct
{
	int m_iChn;			//通道号
	int m_iSceneId;		//场景编号
	unsigned char m_strSceneName[GUI_PUBLIC_LEN_63 + 1]; //场景名称
}TGuiGetVcaCCDReq;

typedef struct
{
	int m_iChn;			//通道号
	int m_iSceneId;		//场景编号
	unsigned char m_strSceneName[GUI_PUBLIC_LEN_63 + 1]; //场景名称
	TGuiVcaParamCDD  m_tCcdCnf;
}TGuiGetVcaCCDRsp; 

#define GUI_SET_VCACCD_REQ			4505		//设置人群聚集参数请求
#define GUI_SET_VCACCD_RSP			-4505		//设置人群聚集参数回应
typedef  TGuiGetVcaCCDRsp TGuiSetVcaCCDReq;
typedef struct
{
	int m_iRetCode;		//0表成功，其余表示失败
}TGuiSetVcaCCDRsp;


//***************人数统计*******************//
typedef struct		/*流量统计CPC配置参数*/
{
	int				m_iStructSize;
	int				m_iMode;				/*1-垂直人数统计，2-水平人数统计*/
	int				m_iMinSize;				/*目标最小尺寸*/
	int				m_iMaxSize;				/*目标最大尺寸*/
	int				m_iSensitiv;			/*灵敏度*/
	int				m_iDetecType;			/*检测类型*/
	int 			m_iColor;
	int 			m_iAlarmColor;
	int				m_iAlarmInCount;
	int				m_iAlarmOutConut;
	TGuiVcaPolygon	m_stRegion;
	TGuiVcaPolygon	m_stRegionEx;
}TGuiVcaCPCAdv;

#define GUI_GET_VCA_CPCADV_REQ		4506 //获取人数统计请求
#define GUI_GET_VCA_CPCADV_RSP		-4506 //获取人数统计回应
typedef struct
{
	int m_iChn;
	int m_iSceneId;		//场景编号
	unsigned char m_strSceneName[GUI_PUBLIC_LEN_63 + 1]; //场景名称
}TGuiGetCpcAdvanceReq;

typedef struct
{
	int m_iChn;
	int m_iSceneId;		//场景编号
	unsigned char m_strSceneName[GUI_PUBLIC_LEN_63 + 1]; //场景名称
	TGuiVcaCPCAdv  m_tCpcAdvCnf;
}TGuiGetCpcAdvancedRsp;

#define GUI_SET_CPCADVANCE_REQ		4507  //设置人数统计请求
#define GUI_SET_CPCADVANCE_RSP		-4507 //设置人数统计回应
typedef TGuiGetCpcAdvancedRsp TGuiSetCpcAdvancedReq;
typedef struct
{
	int m_iRetCode;     //0表成功 其余表示失败
}TGuiSetCpcAdvancedRsp;

//**************人脸检测*******************//
typedef struct     /*人脸识别VFD配置参数*/
{
	int			m_iStructSize;
	int			m_iMaxSize;			/*最大人脸尺寸*/
	int			m_iMinSize;			/*最小人脸尺寸*/
	int			m_iLevel;			/*算法运行级别*/
	int			m_iSensitiv;		/*灵敏度*/
	int			m_iPicScal;			/*图像缩放比例*/
	int			m_iSnapEnable;		/*人脸抓拍使能*/
	int			m_iSnapSpace;		/*抓拍间隔*/
	int			m_iSnapTimes;		/*抓拍次数*/
	int 			m_iColor;
	int 			m_iAlarmColor;
	TGuiVcaPolygon	m_stRegion;	
}TGuiVcaVFDAdv;

#define GUI_GET_FACEADVANCE_REQ		4508	//获取人脸检测请求
#define GUI_GET_FACEADVANCE_RSP		-4508	//获取人脸检测回应
typedef struct
{
	int m_iChn;			//通道号
	int m_iSceneId;		//场景编号
	unsigned char m_strSceneName[GUI_PUBLIC_LEN_63 + 1]; //场景名称
}TGuiGetFaceAdvanceReq;

typedef struct
{
	int m_iChn;			//通道号
	int m_iSceneId;		//场景编号
	unsigned char m_strSceneName[GUI_PUBLIC_LEN_63 + 1]; //场景名称
	TGuiVcaVFDAdv m_tVfdCnf;
}TGuiGetFaceAdvanceRsp;

#define GUI_SET_FACEADVANCE_REQ		4509	//设置人脸检测请求
#define GUI_SET_FACEADVANCE_RSP		-4509	//设置人脸检测回应
typedef TGuiGetFaceAdvanceRsp TGuiSetFaceAdvanceReq;
typedef struct
{
	int m_iRetCode;      //0表成功, 1表失败
}TGuiSetFaceAdvanceRsp;

//*****************车牌识别*****************//
#define GUI_GET_ITS_VLOOP_REQ		4510 	//获取虚拟线圈参数请求
#define GUI_GET_ITS_VLOOP_RSP		-4510 	//获取虚拟线圈参数回应
typedef  struct
{
	int m_iChn;          //通道号
	int m_iRoadId;       //车道编号
	int m_iVLoopId;      //虚拟线圈
	int m_iSceneId;		 //场景ID
	int m_iDayNight;	 //时间标志 1-白天0-晚上
	unsigned char m_strSceneName[GUI_PUBLIC_LEN_63 + 1]; //场景名称
}TGuiGetVLoopCnfReq;

typedef struct
{
	int m_iChn; 		 //通道号
	int m_iRoadId;       //车道编号
	int m_iVLoopId;      //虚拟线圈
	int m_iSceneId;		 //场景ID
	int m_iDayNight;	 //时间标志 1-白天0-晚上
	int m_iSensitivity;	 //灵敏度，占比
	unsigned char m_strSceneName[GUI_PUBLIC_LEN_63 + 1]; //场景名称
	TGuiVcaPolygon m_tVLoopRegion;	//线圈坐标
}TGuiGetVLoopCnfRsp;

#define GUI_SET_ITS_VLOOP_REQ		4511  //设置虚拟线圈参数请求
#define GUI_SET_ITS_VLOOP_RSP		-4511 //设置虚拟线圈参数回应
typedef TGuiGetVLoopCnfRsp TGuiSetVLoopCnfReq;

typedef struct
{
	int m_iRetCode;			//0表成功，其余表示失败
}TGuiSetVLoopCnfRsp;		//回应


//*****************车道识别区域*************************//
#define GUI_GET_ITS_RECO_RANGE_REQ	4512	//获取车道识别区域请求
#define GUI_GET_ITS_RECO_RANGE_RSP	-4512	//获取车道识别区域回应
typedef struct
{
	int m_iChn; 		 //通道号
	int m_iRoadId;       //车道  0~3
	int m_iSceneId;		 //场景
	unsigned char m_strSceneName[GUI_PUBLIC_LEN_63 + 1]; //场景名称
}TGuiGetRecoRangeReq;

typedef struct
{
	int m_iChn;
	int m_iRoadId;       //车道  0~3
	int m_iSceneId;		 //场景
	int m_iLeftRange;	 //左边界 0~100
	int m_iTopRange;	 //上边界 0~100
	int m_iRightRange;	 //右边界 0~100
	int m_iBottomRange;	 //下边界 0~100
	unsigned char m_strSceneName[GUI_PUBLIC_LEN_63 + 1]; //场景名称
}TGuiGetRecoRangeRsp;

#define GUI_SET_ITS_RECO_RANGE_REQ	4513	//设置车道识别区域请求
#define GUI_SET_ITS_RECO_RANGE_RSP	-4513	//设置车道识别区域回应
typedef TGuiGetRecoRangeRsp TGuiSetRecoRangeReq;

typedef struct
{
	int m_iRetCode;	//0表成功，其余表失败
}TGuiSetRecoRangeRsp;

//*************************车牌优化**********************//
#define GUI_GET_ITS_OPTIM_REQ		4514	//获取车牌优化请求
#define GUI_GET_ITS_OPTIM_RSP		-4514	//获取车牌优化回应

typedef struct
{
	int m_iChn;			//通道号
	int m_iSceneId;		//场景
	unsigned char m_strSceneName[GUI_PUBLIC_LEN_63 + 1]; //场景名称
}TGuiGetItsOptimReq;

typedef struct
{
	int m_iChn;					//通道号
	int m_iSceneId;				//场景
	char m_strModifyChar[GUI_PUBLIC_LEN_15 + 1];        //优化首汉字  utf-8编码
	int m_iMinCharConfig;		//首汉字优化最低置信度  乘以1000
	char m_strModifyAlpha[GUI_PUBLIC_LEN_15 + 1];       //优化首字母
	int m_iMinAlphaConfig;		//首字母优化最低置信度
	unsigned char m_strSceneName[GUI_PUBLIC_LEN_63 + 1]; //场景名称
}TGuiGetItsOptimRsp;

#define GUI_SET_ITS_OPTIM_REQ		4515	//设置车牌优化请求
#define GUI_SET_ITS_OPTIM_RSP		-4515	//设置车牌优化回应
typedef  TGuiGetItsOptimRsp TGuiSetItsOptimReq;

typedef struct
{
	int m_iRetCode;		//0表成功，其余表失败
}TGuiSetItsOptimRsp;


//**************************离岗检测***********************//   
typedef struct		/*离岗检测SVD配置参数*/
{
	int			    m_iStructSize;
	int				m_iAlertTime;
	int				m_iClearTime;
	int				m_iSensitivity;
	int				m_iAlarmCount;
	int				m_iColor;			/*未报警时规则线颜色*/
	int				m_iAlarmColor;		/*报警时的规则线颜色*/
	int				m_iDisplayRule;		
	int				m_iDisplayStat;
	int				m_iRegionNum;
	int				m_iDutyNum;
	int				m_iMinSize;
	int				m_iMaxSize;
	TGuiVcaRect_Ex	m_stRegion[4];		/*检测区域,最大4个*/
}TGuiVcaParamSVD;

#define GUI_GET_VCASVD_REQ		 		4516	//获取离岗检测参数请求
#define GUI_GET_VCASVD_RSP				-4516	//获取离岗检测参数回应
typedef struct
{
	int m_iChn;		//通道号
	int m_iSceneId;	//场景编号
	unsigned char m_strSceneName[GUI_PUBLIC_LEN_63 + 1]; //场景名称
}TGuiGetVcaSVDReq;

typedef struct
{
	int m_iChn;		//通道号
	int m_iSceneId;	//场景编号
	unsigned char m_strSceneName[GUI_PUBLIC_LEN_63 + 1]; //场景名称
	TGuiVcaParamSVD m_tSvdCnf;
}TGuiGetVcaSVDRsp;


#define GUI_SET_VCASVD_REQ			4517		//设置离岗检测参数请求
#define GUI_SET_VCASVD_RSP			-4517		//设置离岗检测参数回应
typedef  TGuiGetVcaSVDRsp TGuiSetVcaSVDReq;
typedef struct
{
	int m_iRetCode;		//0表成功，其余表示失败
}TGuiSetVcaSVDRsp;

//*****************视频诊断*********************//

typedef struct
{
	int				m_iLevel;
	int				m_iTime;
}TGuiVcaLevelTimePara;

typedef struct		/*视频诊断AVD 参数*/
{
	int			    m_iStructSize;
	int				m_iRuleEvevtid;		/* 视频诊断规则标识，按位与协议文档相对
											噪声诊断 	0x0000000001
											清晰度诊断	0x0000000010
											亮度诊断	0x0000000100 
											往后依次为偏色诊断、画面冻结诊断、信号缺失诊断
											场景变换诊断 人为干扰诊断 PTZ失控诊断*/
	int				m_iAlarmCount;		/*报警计数: 该个数将在屏幕上显示*/
	int				m_iDisplayStat;
	int				m_iCheckTime;
	TGuiVcaLevelTimePara		m_stAvdPara[9];
} TGuiVcaParamAVD;

#define GUI_GET_VCAAVD_REQ		 		4518	//获取视频诊断检测参数请求
#define GUI_GET_VCAAVD_RSP				-4518	//获取视频诊断检测参数回应
typedef struct
{
	int m_iChn;			//通道号
	int m_iSceneId;		//场景编号
	unsigned char m_strSceneName[GUI_PUBLIC_LEN_63 + 1]; //场景名称
}TGuiGetVcaAVDReq;

typedef struct
{
	int m_iChn;			//通道号
	int m_iSceneId;		//场景编号
	unsigned char m_strSceneName[GUI_PUBLIC_LEN_63 + 1]; //场景名称
	TGuiVcaParamAVD m_tAvdCnf;
}TGuiGetVcaAVDRsp; 


#define GUI_SET_VCAAVD_REQ			4519		//设置视频诊断检测参数请求
#define GUI_SET_VCAAVD_RSP			-4519		//设置视频诊断检测参数回应
typedef  TGuiGetVcaAVDRsp TGuiSetVcaAVDReq;
typedef struct
{
	int m_iRetCode;		//0表成功，其余表示失败
}TGuiSetVcaAVDRsp;

//***********************音频异常检测***************************//
typedef struct		/*音频诊断AAD 参数*/
{
	int			    m_iStructSize;
	int				m_iRuleEvevtid;		/* 音频诊断规则标识，按位与协议文档相对
										Lost			-00001
										Abnormal		-00010
										NoiseSupr		-00100
										EchoSupr		-01000
										FeedbackSupr	-10000
										*/
	int				m_iAlarmCount;		/*报警计数: 该个数将在屏幕上显示*/
	int				m_iDisplayStat;
	int				m_iCheckTime;
	TGuiVcaLevelTimePara		m_stAadPara[5];
}TGuiVcaParamAAD;

#define GUI_GET_VCAAAD_REQ				4520	//获取音频异常检测参数请求
#define GUI_GET_VCAAAD_RSP				-4520	//获取音频异常检测参数回应
typedef struct
{
	int m_iChn;			//通道号
	int m_iSceneId;		//场景编号
	unsigned char m_strSceneName[GUI_PUBLIC_LEN_63 + 1]; //场景名称
}TGuiGetVcaAADReq;

typedef struct
{
	int m_iChn;			//通道号
	int m_iSceneId;		//场景编号
	unsigned char m_strSceneName[GUI_PUBLIC_LEN_63 + 1];	//场景名称
	TGuiVcaParamAAD m_tAadCnf;
}TGuiGetVcaAADRsp;


#define GUI_SET_VCAAAD_REQ			4521		//设置音频异常检测参数请求
#define GUI_SET_VCAAAD_RSP			-4521		//设置音频异常检测参数回应
typedef  TGuiGetVcaAADRsp TGuiSetVcaAADReq;
typedef struct
{
	int m_iRetCode;		//0表成功，其余表示失败
}TGuiSetVcaAADRsp;


//*******************跟踪**********************//
typedef enum
{
	GUI_PTZP=1,
	GUI_PTZT,
	GUI_ZOOMRATE,		//跟踪倍率
	GUI_ZOOMRATIO,		//跟踪倍率系数
	GUI_CMRTYPE,			//机芯类型
	GUI_MAXFOLLOWTIME,	//最长跟踪时间
	GUI_HEIGHT,			//球机高度
	GUI_DESSTOPTIME,		//目标静止跟踪时间
	GUI_ENABLEFLAG,		//使能标志按位
							//第0位:检测到人脸后是否返回
							//第1位:是否使用抗遮挡功能
							//第2位:是否启用限位
	GUI_LEFTPOS,		// >= 0 表示有效，否则表示无效，为角度
	GUI_TOPPOS,
	GUI_RIGHTPOS,
	GUI_BOTTOMPOS,
}EGuiVcaParamCAP;

typedef struct
{
	int m_iId;			//TGuiVcaParamCAP KEY
	int m_iValue;		//对应TGuiVcaParamCAP的值
}TGuiItem;

typedef struct
{
	int m_iCnt;  //总数
	TGuiItem m_tItem[0];
}TGuiVcaCAPCnf;

#define GUI_GET_VCACAP_REQ			4523	//获取跟踪参数请求
#define GUI_GET_VCACAP_RSP			-4523	//获取跟踪参数回应

typedef struct
{
	int m_iChn;			//通道号
	int m_iSceneId;		//场景编号
	unsigned char m_strSceneName[GUI_PUBLIC_LEN_63 + 1]; //场景名称
}TGuiGetVcaCAPReq;

typedef struct
{
	int m_iChn;			//通道号
	int m_iSceneId;		//场景编号
	unsigned char m_strSceneName[GUI_PUBLIC_LEN_63 + 1]; //场景名称
	TGuiVcaCAPCnf  m_tCapCnf;
}TGuiGetVcaCAPRsp; 

#define GUI_SET_VCACAP_REQ				4524	//设置跟踪参数请求
#define GUI_SET_VCACAP_RSP				-4524	//设置跟踪参数回应

typedef  TGuiGetVcaCAPRsp TGuiSetVcaCAPReq;
typedef struct
{
	int m_iRetCode;		//0表成功，其余表示失败
}TGuiSetVcaCAPRsp;

// ================================================================
// 2010-11-9 协议扩展

// ========用户管理=======================================
// 获取用户信息协议(扩展)
#define GUI_GET_USER_MSG_EX_REQ	10000
#define GUI_GET_USER_MSG_EX_RSP	-10000

typedef struct
{
	char m_strUser[GUI_USER_NAME_LEN + 1];
} TGuiGetUserMsgExReq;

typedef struct
{
	int m_iGid;	//用户组
	char m_strUser[GUI_USER_NAME_LEN + 1];
	char m_strPsw[GUI_USER_PSW_LEN + 1];
} TGuiGetUserMsgExRsp;

// =====================字符叠加相关设置=============

#define GUI_SET_OSD_OK   0
#define GUI_SET_OSD_ERR  1
//===================扩展 通道名称叠加====================
#define GUI_GET_OSD_CHN_NAME_EX_REQ		1008		//扩展的获取通道名称叠加请求
#define GUI_GET_OSD_CHN_NAME_EX_RSP		-1008		//扩展的获取通道名称叠加回应
#define GUI_SET_OSD_CHN_NAME_EX_REQ		1009		//扩展的设置通道名称叠加请求
#define GUI_SET_OSD_CHN_NAME_EX_RSP		-1009		//扩展的设置通道名称叠加回应

typedef struct
{
	int m_iDsp;		//是否显示
	int m_iColor;		//叠加颜色
	int m_iLocType;	//坐标类型:0 绝对坐标, 1 : 24x16小方格
	int m_iXp;
	int m_iYp;
	char m_strName[0];  //实际的通道名称数据
} TGuiOsdChnNameCnfEx;  //扩展的设置通道名称的结构体

typedef struct
{
	int m_iChn;
} TGuiGetOsdChnNameExReq;		//扩展的获取通道名称叠加请求

typedef struct
{
	int m_iChn;
	TGuiOsdChnNameCnfEx m_tCnf;
} TGuiGetOsdChnNameExRsp; //扩展的获取通道名称叠加回应

typedef struct
{
	int m_iChn;		//通道号
	TGuiOsdChnNameCnfEx m_tCnf;
} TGuiSetOsdChnNameExReq;		//扩展的设置通道名称叠加请求

typedef struct
{
	int m_iRetCode;    //设置成功GUI_SET_OSD_OK， 设置失败GUI_SET_OSD_ERR
} TGuiSetOsdChnNameExRsp;	   //扩展的设置通道名称叠加回应


//=================== 通道名称叠加====================
#define GUI_SET_OSD_CHN_NAME_REQ		1010
#define GUI_SET_OSD_CHN_NAME_RSP		-1010
#define GUI_GET_OSD_CHN_NAME_REQ		1011
#define GUI_GET_OSD_CHN_NAME_RSP		-1011

typedef struct
{
	int m_iDsp;		//是否显示
	int m_iColor;		//叠加颜色
	int m_iLocType;	//坐标类型:0 绝对坐标, 1 : 24x16小方格
	int m_iXp;
	int m_iYp;
	char m_strName[GUI_CHN_NAME_LEN + 1];
} TGuiOsdChnNameCnf;

typedef struct
{
	int m_iChn;		//通道号
	TGuiOsdChnNameCnf m_tCnf;
} TGuiSetOsdChnNameReq;

typedef struct
{
	int m_iRetCode; //设置成功GUI_SET_OSD_OK， 设置失败GUI_SET_OSD_ERR
} TGuiSetOsdChnNameRsp;

typedef struct
{
	int m_iChn;
} TGuiGetOsdChnNameReq;

typedef struct
{
	int m_iChn;
	TGuiOsdChnNameCnf m_tCnf;
} TGuiGetOsdChnNameRsp;


//==================== 时间叠加=====================
#define GUI_SET_OSD_TM_REQ		1012		
#define GUI_SET_OSD_TM_RSP		-1012
#define GUI_GET_OSD_TM_REQ		1013
#define GUI_GET_OSD_TM_RSP		-1013

typedef struct
{
	int m_iDsp;		//叠加全能
	int m_iColor;	
	int m_iLocType;
	int m_iXp;
	int m_iYp;
	int m_iType;		//叠加格式
} TGuiOsdTmCnf;

typedef struct
{
	int m_iChn;
	TGuiOsdTmCnf m_tCnf;
} TGuiSetOsdTmReq;

typedef struct
{
	int m_iRetCode; //设置成功GUI_SET_OSD_OK， 设置失败GUI_SET_OSD_ERR
} TGuiSetOsdTmRsp;

typedef struct
{
	int m_iChn;
} TGuiGetOsdTmReq;

typedef struct
{
	int m_iChn;
	TGuiOsdTmCnf m_tCnf;
} TGuiGetOsdTmRsp;

// =================自定义文本===============
#define GUI_SET_OSD_TEXT_REQ	1014
#define GUI_SET_OSD_TEXT_RSP	-1014
#define GUI_GET_OSD_TEXT_REQ	1015
#define GUI_GET_OSD_TEXT_RSP	-1015

#define GUI_CHG_OSD_TEXT_REQ     1018
#define GUI_CHG_OSD_TEXT_RSP     -1018

typedef struct
{
	int m_iDsp;
	int m_iColor;
	int m_iLocType;
	int m_iXp; 		/*高8位代表 温湿度模块是否存在*/
	int m_iYp;		/*高8位代表 体征模块是否存在*/
	char m_strText[0];
} TGuiOsdTextCnf;

typedef struct
{
	int m_iChn;
	TGuiOsdTextCnf m_tCnf;
} TGuiSetOsdTextReq;

typedef struct
{
	int m_iRetCode;//设置成功GUI_SET_OSD_OK， 设置失败GUI_SET_OSD_ERR
} TGuiSetOsdTextRsp;

typedef struct
{
	int m_iChn;
} TGuiGetOsdTextReq;

typedef struct
{
	int m_iChn;
	TGuiOsdTextCnf m_tCnf;
} TGuiGetOsdTextRsp;
//自定义字符叠加，支持多个区域
#define GUI_SET_MUTI_OSD_TEXT_REQ	1204
#define GUI_SET_MUTI_OSD_TEXT_RSP	-1204
#define GUI_GET_MUTI_OSD_TEXT_REQ	1205
#define GUI_GET_MUTI_OSD_TEXT_RSP	-1205
#define GUI_CHG_MUTI_OSD_TEXT_REQ   1206
#define GUI_CHG_MUTI_OSD_TEXT_RSP   -1206

typedef struct
{
	int m_iLayNo;
	TGuiOsdTextCnf m_tCnf;
} TGuiMutiOsdTextCnf;

typedef struct
{
	int m_iChn;
	TGuiMutiOsdTextCnf m_tCnf;
} TGuiSetMutiOsdTextReq;

typedef struct
{
	int m_iRetCode;//设置成功GUI_SET_OSD_OK， 设置失败GUI_SET_OSD_ERR
} TGuiSetMutiOsdTextRsp;

typedef struct
{
	int m_iChn;
	int m_iLayNo;
} TGuiGetMutiOsdTextReq;

typedef struct
{
	int m_iChn;
	TGuiMutiOsdTextCnf m_tCnf;
} TGuiGetMutiOsdTextRsp;

// 视频遮挡区域设置
#define GUI_SET_OSD_PAREA_REQ		1016
#define GUI_SET_OSD_PAREA_RSP		-1016
#define GUI_GET_OSD_PAREA_REQ		1017
#define GUI_GET_OSD_PAREA_RSP		-1017

typedef struct
{
	int m_iEnable;	//视频遮挡区域全能标识
	int m_iLocType;	//坐标类似
	int m_iAreaCnt;
	TGuiArea m_tPrvAreas[GUI_MAX_COVER_AREA];
} TGuiOsdPrvAreaCnf;

typedef struct
{
	int m_iChn;
	TGuiOsdPrvAreaCnf m_tCnf;
} TGuiSetOsdPrvAreaReq;

typedef struct
{
	int m_iRetCode;
} TGuiSetOsdPrvAreaRsp;

typedef struct
{
	int m_iChn;
} TGuiGetOsdPrvAreaReq;

typedef struct
{
	int m_iChn;
	TGuiOsdPrvAreaCnf m_tCnf;
} TGuiGetOsdPrvAreaRsp;

#define GUI_GET_DYNAMIC_PRVAREA_REQ      1050//获取动态隐私区域请求
#define GUI_GET_DYNAMIC_PRVAREA_RSP	    -1050  //获取动态隐私区域回应
#define GUI_SET_DYNAMIC_PRVAREA_REQ      1051  //设置动态隐私区域请求
#define GUI_SET_DYNAMIC_PRVAREA_RSP	    -1051  //设置动态隐私区域回应

typedef struct
{
	int m_iAreaNo;         //区域编号 1~24
	int m_iMinZoomSize;    //最小变倍 1~50
	int m_iColor;          //颜色 1：红色 2：绿色 3：黄色 4：蓝色 5：紫色 6：青色 7：黑色 8：白色 9:马赛克
	TGuiVcaPolygon m_tPolygon;  
}TGuiDynPrvArea;

typedef struct
{
	int m_iChn;
}TGuiGetDynPrvAreaReq;

typedef struct
{
	int m_iChn;
	TGuiDynPrvArea m_tDynPrvArea[24];  //最多支持24个区域
}TGuiGetDynPrvAreaRsp;

typedef struct
{
	int m_iChn;
	TGuiDynPrvArea m_tDynPrvArea[24];
}TGuiSetDynPrvAreaReq;

typedef struct
{
	int m_iRetCode;       // 0表成功，其余表示失败
}TGuiSetDynPrvAreaRsp;

#define GUI_GET_CHN_ABILITY_REQ 	  1060   //获取通道支持的能力集请求
#define GUI_GET_CHN_ABILITY_RSP	  -1060   //获取通道支持的能力集回应

typedef enum
{
	CHN_DYNPRVAREA = 1,    //通道是否支持动态隐私遮挡(高端球)
	CHN_STATICPRVAREA,        //是否支持隐私遮挡(IPC)
	CHN_LINKDBLIGHT = 3,    //是否支持双灯
	CHN_COLOR2GRAY,       //是否支持彩转黑功能
	CHN_DAYNIGHT,           //是否支持日夜模式(球的彩转黑功能)
}EChnAbilityEnum;

typedef struct
{
	int m_iType;
}TGuiGetChnAbilityReq;            //获取能力集

typedef struct
{
	int m_iType;
	int m_iChn[GUI_DVR_CHN_CNT];      //按位 表示 32*32=1024个
}TGuiGetChnAbilityRsp;


// ===============系统配置相关=================
#define GUI_SET_SYS_TIME_OK        0
#define GUI_SET_SYS_TIME_ERR       1
// 设置系统时间
#define GUI_SET_SYS_TIME_REQ			1020
#define GUI_SET_SYS_TIME_RSP			-1020
#define GUI_GET_SYS_TIME_ZONE_REQ		1023
#define GUI_GET_SYS_TIME_ZONE_RSP		-1023
#define GUI_SET_SYS_TIME_ZONE_REQ		1024
#define GUI_SET_SYS_TIME_ZONE_RSP		-1024		

#define GUI_TIME_ZONE_DEFAULT			100

typedef struct
{
	int m_iTimeZone;					//时区，默认GUI_TIME_ZONE_DEFAULT，表示不改变当前时区
	int m_iYear;
	int m_iMon;
	int m_iDay;
	int m_iHour;
	int m_iMin;
	int m_iSec;
} TGuiSetSysTimeReq;

typedef struct
{
	int m_iRetCode;
} TGuiSetSysTimeRsp;

typedef struct
{
	int m_iTimeZone;   //整点时区加减100(比如-8:00对应的值为-8+100=92, +8为8+100=108)
	//半点和45之类的加减10000(比如+5:40,对应的值为10545, -4:30则为-0430+10000)
} TGuiGetSysTimeZoneRsp;

typedef struct
{
	int m_iTimeZone;
} TGuiSetSysTimeZoneReq;

typedef struct
{
	int m_iRetCode;
} TGuiSetSysTimeZoneRsp;

// 获取通道数目配置信息
#define GUI_GET_DEV_NUM_REQ	1021
#define GUI_GET_DEV_NUM_RSP	-1021

enum
{
	VI_NO = 1,	//视频输入路数
	VO_NO,		//视频输出设备数
	VL_NO,		//视频环出路数
	AI_NO,		//音频输入数
	AO_NO,		//音频输出数
	AL_NO,		//音频环出路数
	AIP_NO,		//报警输入端口数
	AOP_NO,		//报警输出端口数
	COM_NO,		//串口数目
	VC_NO,		//合成编码通道数
	VC_VO_NO,	//合成编码虚拟VO数(0/1) 1-有0-无
	NETWORK_CARD_NO,  //网卡数量
	SYNCPLAY_NO,    //同步回放窗体数
	VI_NO_PSE,		//PSE通道总数
	POWER_PSE,		//PSE总功率
};

typedef struct
{
	int m_iDevId;	//设备类型
} TGuiGetDevNumReq;

typedef struct
{
	int m_iDevId;	//设备类型
	int m_iNum;		//数目
} TGuiGetDevNumRsp;

// 获取参数修改版本号
#define GUI_GET_CNF_VER_REQ	1022
#define GUI_GET_CNF_VER_RSP	-1022

// 主程序对每种匹配作修改后，对其对应的版本号+1
// 外挂在必要的时候通过此协议询问配置的版本号，
// 在版本号发生变化的情况下才通过相应的协议重获取相关参数
typedef enum
{
	GUI_VI_CNF_ID = 0,	//VI配置
	GUI_VENC_CNF_ID,	//编码配置
	GUI_VREC_CNF_ID,	//录像配置
	GUI_SYS_CNF_ID,	//系统参数配置
	GUI_USER_CNF_ID,		//用户配置
	GUI_PTZ_CNF_ID,		//云镜配置
} TGuiCnfMajorId;

typedef struct
{
	int m_iCnfId;		//配置主类型
	int m_iCnfSubId;	//配置子类型，用于细化
	int m_iChn;		//参数对应的通道号,0表示所有通道
} TGuiGetCnfVerReq;

typedef struct
{
	int m_iVer;		//版本号
} TGuiGetCnfVerRsp;

// PUD参数获取
#define GUI_GET_PU_CNF_REQ		1025
#define GUI_GET_PU_CNF_RSP		-1025

typedef struct
{
	char    m_cManagerServerIp[16];    //服务器IP
	long    m_lManagerServerPort;      //服务器端口
	char    m_cHeartBeatServerIp[16];  //心跳服务器IP
	long    m_lHeartBeatPort;  			//心跳服务器端口
	char    m_cRegisterServerIP[16];	//注册服务器IP
	long    m_lRegisterPort;			//注册服务器端口
	char    m_cEnableHeartBeat[16];		//使能心跳
	int     m_iHeartBeatPeriod;			//心跳周期
	int     m_iRegisterTime;			//注册时间
	char    m_cVapPath[256];			//
	char    m_authServerIp[16];			//
	long    m_lauthServerPort;
	char    m_authUserName[16];
	char    m_authUserPassword[16];
	char    m_cputhreshold;
	char	m_memthreshold;
} TGuiPuCnf;

typedef struct
{
	int m_iRetCode;	//返回码
	TGuiPuCnf m_tCnf; //配置信息
} TGuiGetPuCnfRsp;

// PU信息获取
#define GUI_SET_PU_CNF_REQ		1026
#define GUI_SET_PU_CNF_RSP		-1026

typedef struct
{
	TGuiPuCnf m_tCnf;
} TGuiSetPuCnfReq;

typedef struct
{
	int m_iRet;
} TGuiSetPuCnfRsp;

// 获取硬件特征码
#define GUI_GET_HPM_CODE_REQ	1027
#define GUI_GET_HPM_CODE_RSP	-1027

typedef struct
{
	unsigned int m_uiCode[8];
} TGuiGetHpmCodeRsp;

//设置获取设备公共信息
#define GUI_DEV_COMMON_INFO_LEN  63
#define GUI_DEV_NFS_PATH_LEN  255 
#define GUI_DEV_IP_LEN 15
typedef struct
{
    char    m_cDeviceId[GUI_DEV_COMMON_INFO_LEN + 1];   //设备id
    char    m_cDeviceName[GUI_DEV_COMMON_INFO_LEN + 1]; //设备名称
    char    m_cNFSIp[GUI_DEV_IP_LEN + 1];      			//设备nfs挂载ip
    char  	m_cNFSPath[GUI_DEV_NFS_PATH_LEN + 1];  		//设备nfs挂载路径
	int		m_iDevNo;			//设备编号，陶永亮，2008-12-03，DVR新增字段
	int 	m_blNfsEnable;		//NFS使能
	int		m_iVspport;         //贝尔平台使用
	int		m_iVapport;			//贝尔平台使用
	char	m_cAccessnum[GUI_DEV_COMMON_INFO_LEN + 1];   //贝尔平台使用
}TGuiDevCommonInfoCnf;

#define GUI_GET_DEV_COMMON_INFO_CNF_REQ   1028  //获取设备commoninfo信息请求
#define GUI_GET_DEV_COMMON_INFO_CNF_RSP	 -1028	//获取设备commoninfo信息回应

typedef struct
{
	TGuiDevCommonInfoCnf m_tCommonInfo;
}TGuiGetDevCommonInfoCnfRsp;


#define GUI_SET_DEV_COMMON_INFO_CNF_REQ   1029 //设置设备commoninfo信息请求
#define GUI_SET_DEV_COMMON_INFO_CNF_RSP	  -1029 //设置设备commoninfo信息回应
typedef struct
{
	TGuiDevCommonInfoCnf m_tCommonInfo;
}TGuiSetDevCommonInfoCnfReq;  //设置设备commoninfo信息请求

typedef struct
{
	int m_iRetCode;  //GUI_SET_OK -成功，GUI_SET_ERR-失败
}TGuiSetDevCommonInfoCnfRsp;  //设置设备commoninfo信息回应


// 对讲控制
#define GUI_TALK_CTRL_REQ		1030
#define GUI_TALK_CTRL_RSP		-1030

enum //对讲音频数据格式
{
	GUI_AS_NORMAL = 0,
	GUI_AS_PCM,
};

typedef struct		//对讲控制
{
	int m_iAct;		//命令码:1 start, 0: stop
	int m_iDevId;	//对讲设备编号, 预留，默认值1
	int m_iType;		//码流类型
	int m_iReserve;	//保留字	, 默认0	
} TGuiTalkCtrlReq;

typedef struct
{
	int m_iRetCode;
} TGuiTalkCtrlRsp;

//IPC对讲
#define GUI_IPC_TALK_CTRL_REQ	1031   //对讲控制
#define GUI_IPC_TALK_CTRL_RSP	-1031 
typedef struct
{
	int m_iChn;
	int m_iActId;	   //命令码:1 start, 0: stop
	int m_iType;		//码流类型
}TGuiIpcTalkCtrlReq;

typedef struct
{
	int m_iRetCode; //-1表示失败
} TGuiIpcTalkCtrlRsp;

#define GUI_GET_IPC_WHO_TALK_REQ	1032 //获取跟哪个数字通道对讲
#define GUI_GET_IPC_WHO_TALK_RSP	-1032 //获取跟哪个通道对讲回应

typedef struct
{
	int m_iChn;  //当前没有通道进行对讲，则返回-1
}TGuiGetIpcWhoTalkRsp;

// 编码参数设置原子协议 2011/01/10 dxl
#define GUI_SET_ENCODE_PARAM_REQ	1040
#define GUI_SET_ENCODE_PARAM_RSP	-1040
#define GUI_GET_ENCODE_PARAM_REQ	1041
#define GUI_GET_ENCODE_PARAM_RSP	-1041

#define GUI_SET_ENCODE_PARAM_OK    0
#define GUI_SET_ENCODE_PARAM_ERR   1

enum
{
	GUI_ENCODE_VSIZE = 1,			//视频大小
	GUI_ENCODE_FRAME_RATE,		//帧率
	GUI_ENCODE_BITS_RATE_TYPE,	//位率类型
	GUI_ENCODE_BITS_RATE,			//码率
	GUI_ENCODE_QUALITY,			//图像质量
	GUI_ENCODE_TYPE,				//码流类型(复合码流/视频/音频)
	GUI_ENCODE_IFAME_RATE,		//I帧间隔
	GUI_ENCODE_AUDIO_TYPE,		//音频编码方式
};

typedef struct
{
	int m_iChn;				//通道号
	int m_iParamId;			//参数类别(由上述枚举指定)
	int m_iParamValue;		//参数值
} TGuiSetEncodeParamReq;

typedef struct
{
	int m_iRetCode;
} TGuiSetEncodeParamRsp;

typedef struct
{
	int m_iChn;
	int m_iParamId;
} TGuiGetEncodeParamReq;

typedef struct
{
	int m_iParamId;		//与请求值对应，-1表式失败
	int m_iParamValue;	//参数值
} TGuiGetEncodeParamRsp;


// 透明通道/串口数据
#define GUI_TX_DATA_SEND_CMD			2000

enum	//数据类型
{
	GUI_TX_DATA_COM 		= 1, 	//串口数据
	GUI_TX_DATA_LOCAL,			//主程序与外挂的透明通道(不建议使用，除非非常特别的功能)
	GUI_TX_DATA_OTHER, 			//网络透明通道数据(客户端通过网络发给外挂的协议数据)
	GUI_TX_DATA_TALK,				//对讲数据
};

typedef struct
{
	int m_iType;	//数据类型
	int m_iSeq;	//序列号(可以递增或一直填充0)
	int m_iSrc;	//源编号(如来自己哪个串口)(默认值为0)
	int m_iDst;	//目标号(如要发往哪个串口)(默认值为0)
	int m_iR1;	//保留(默认值0)
	char m_cData[0];	 //数据
} TGuiTxDataSendCmd;

//=========================行业嵌入式部=========================
/*行业化时UI和内核通信的协议与结构*/
#define GUI_TX_APP_DATA_SEND_REQ		2100             //UI 发给内核再由内核转发给外挂请求
#define GUI_TX_APP_DATA_SEND_RSP		-2100			//UI 发给内核再由内核转发给外挂回应

typedef struct
{
	int m_iSendSize;		//成功发送的字节数
} TGuiAppDataSendRsp;

#define GUI_TX_APP_DATA_RCV_REQ			2101			//UI向内核发送请求字节数
#define GUI_TX_APP_DATA_RCV_RSP			-2101			//UI向内核发送数据的大小回应

typedef struct
{
	int m_iRcvBufSize;
} TGuiAppDataRecvReq;

//片头叠加
#define GUI_GET_VIDEO_HEADER_OSD_REQ	2180         //UI向内核获取片头叠加信息请求
#define GUI_GET_VIDEO_HEADER_OSD_RSP    -2180        //UI向内核获取片头叠加信息回应

#define MAX_INQUEST_VIDEO_HEADER_LEN	384

typedef struct 
{
	int 		m_iOverlayTime;			//叠加时间
	int			m_iX;					//片头叠加横坐标
	int			m_iY;					//片头叠加纵坐标
	int 		m_isDispaly;			//是否显示	//add by zxs 2012.9.5
	int 		m_iIedemo;				//是否是通过网络下传的片头
	char 		m_cHeaderMsg[MAX_INQUEST_VIDEO_HEADER_LEN + 1];	//片头叠加的内容，最多384字节
}TGuiGetVideoHeaderOsdMsgRsp;

#define GUI_SET_VIDEO_HEADER_OSD_REQ	2181		//外挂向内核设置片头叠加信息请求
#define GUI_SET_VIDEO_HEADER_OSD_RSP	-2181		//外挂向内核设置片头叠加信息回应

#define TGuiSetVideoHeaderOsdMsgReq TGuiGetVideoHeaderOsdMsgRsp

#define GUI_SET_VIDEO_HEADER_OSD_MSG_OK  	0   //设置成功
#define GUI_SET_VIDOE_HEADER_OSD_MSG_ERR	1	//设置失败
typedef struct
{
	int m_iRetCode;
}TGuiSetVideoHeaderOsdMsgRsp;

//-----------------------ATM------卡号叠加----------------------------------
#define GUI_GET_ATM_CARD_OSD_REQ	2010  //获取卡号叠加参数请求
#define GUI_GET_ATM_CARD_OSD_RSP	-2010 //回应

#define GUI_CARD_PROTOCOL_LEN			63
#define	GUI_CARD_PROTOCOL_VIA_COM		0
#define GUI_CARD_PROTOCOL_VIA_NET 		1

typedef struct
{
	int	m_iType;			//通信方式0:串口 1:UDP
	int m_iPortId; 				//串口ID
	char m_strProtocolName[GUI_CARD_PROTOCOL_LEN + 1];	//协议厂家名称	
	char m_strSrcIp[GUI_IP_STR_LEN + 1];	//ATM  IP地址
	char m_strDstIp[GUI_IP_STR_LEN + 1];			//设备IP地址
	int	m_iSrcPort;				//ATM端口号
	int	m_iDstPort;			//设备端口号
}TGuiAtmMsg;    //ATM参数信息

typedef struct
{
	int m_iChn;             //叠加的通道号按位表示 0x1表示第1通道,0x2表示第二通道，依次类推,0表示所有
	int m_iColor;			//叠加颜色
	int	m_iXp;
	int	m_iYp;				//叠加位置   m_iYp = -1按位置叠加，其余传坐标
	TGuiAtmMsg m_tAtmMsg;         //ATM 参数
}TGuiGetAtmCardNoOsdRsp;

#define	GUI_SET_ATM_CARD_OSD_REQ	2011	//设置卡号叠加参数请求
#define GUI_SET_ATM_CARD_OSD_RSP 	-2011    //回应

#define TGuiSetAtmCardNoOsdReq	TGuiGetAtmCardNoOsdRsp  //设置卡号叠加参数请求

#define GUI_SET_ATM_CardNoOsd_MSG_OK		0		//设置参数成功
#define GUI_SET_ATM_CardNoOsd_MSG_ERR		1		//设置参数失败
typedef struct							//设置卡号叠加参数回应
{
	int m_iRetCode;
} TGuiSetAtmCardNoOsdRsp;

#define GUI_GET_ATM_PROTOCOL_LIST_REQ   2012  	//获取ATM支持的协议列表请求
#define GUI_GET_ATM_PROTOCOL_LIST_RSP   -2012   //获取ATM支持的协议列表回应

#define GUI_ATM_PROTOCOL_NAME_LEN 		31
typedef struct 								//获取协议列表回应
{
	int m_iProtocolCnt;							//协议总数
} TGuiGetAtmProtocolListRsp;

typedef struct 								//协议列表项
{
	char m_strAtmProtocolName[GUI_ATM_PROTOCOL_NAME_LEN + 1]; //协议名
} TGuiAtmProtocolListItem;

//----------------------ATM----抓拍设置----------------------
#define GUI_GET_SNAPSHOT_CNF_REQ	2013
#define GUI_GET_SNAPSHOT_CNF_RSP	-2013

typedef struct
{
	int m_iSnaptype;	//抓拍模式，0单张抓拍 1定时抓拍
	int m_iQvalue;	//图片质量
	int m_iTmval;	//抓拍时间间隔 
	int m_iPicSize;   //图片大小 -1表示自动 其他值对应视频大小
	int m_iSnapLink;	//抓拍联动ftp 0/1 按位设置 0x1是ftp 0x2是email，其余扩展
}TGuiSnapShotCnf;

typedef struct                     //获取抓拍设置参数 按通道号查询
{
	int m_iChn;       //通道号	
}TGuiGetSnapshotCnfReq;

typedef struct					 // 获取抓拍设置参数回应
{
	int m_iChn;
	TGuiSnapShotCnf m_tSnapCnf;
}TGuiGetSnapshotCnfRsp;

#define GUI_SET_SNAPSHOT_CNF_REQ	2014
#define GUI_SET_SNAPSHOT_CNF_RSP	-2014

typedef struct
{
	int m_iChn;			//通道号
	TGuiSnapShotCnf m_tSnapCnf;
}TGuiSetSnapshotCnfReq;			//设置抓拍参数请求

#define GUI_SET_SNAPSHOT_CNF_OK		0		//设置参数成功
#define GUI_SET_SNAPSHOT_CNF_ERR	1		//设置参数失败

typedef struct							
{
	int m_iRetCode;
} TGuiSetSnapshotCnfRsp;			//设置抓拍参数回应



/*******************报警主机控制**************************/
#define GUI_SET_ALARMER_CNF_OK   0		//报警主机设置成功
#define GUI_SET_ALARMER_CNF_ERR  1		//报警主机设置失败

#define GUI_ALARM_IP_ADDRESS_LEN	31

typedef struct						//7601B报警主机
{
	int m_iIsUsed;		   	 		//是否启用
	int m_iSerialNo;	     		//串口号
	int m_iProtocoType;	     		//协议类型
	int m_iAddress;					//地址(0~255)
	int m_iAlarmInNum;	   	 		//报警输入数
	int m_iAlarmOutNum;   	 		//报警输出数
}TGuiSerialAlarmerMsg;


typedef struct						//网络报警主机
{
	int m_iIsUsed;					//启用
	char m_strAddress[GUI_ALARM_IP_ADDRESS_LEN + 1];	//ip地址
	int m_iPort;		//端口号
	int m_iAlarmInNum;	//报警输入数
	int m_iAlarmOutNum;	//报警输出数
}TGuiNetAlarmerMsg;

typedef struct									//获取网络报警主机参数请求
{
	int m_iDevNo;									//设备号
} TGuiGetNetAlarmerMsgReq;

typedef struct				
{
	int m_iDevNo;							
	TGuiNetAlarmerMsg	m_tNetAlarmMsg;			//主机信息
} TGuiGetNetAlarmerMsgRsp;						//获取网络报警主机信息

typedef struct
{
	int m_iDevNo;
	TGuiNetAlarmerMsg m_tNetAlarmerMsg;			//设置信息
} TGuiSetNetAlarmerMsgReq;						//设置网络报警主机信息请求

typedef struct
{
	int m_iRetCode;    							 //设置回应 0表示 成功
}TGuiSetNetAlarmerMsgRsp;					     //设置网络报警主机回应

typedef struct									//获取串口报警主机参数请求
{
	int m_iDevNo;									//设备号
} TGuiGetSerialAlarmerMsgReq;

typedef struct
{
	int m_iDevNo;								//返回码，0表示成功
	TGuiSerialAlarmerMsg	m_tSeialAlarmerMsg;	//主机信息
}TGuiGetSerialAlarmerMsgRsp;					//获取串口报警主机信息

typedef struct
{
	int m_iDevNo;
	TGuiSerialAlarmerMsg m_tSeialAlarmerMsg;
}TGuiSetSerialAlarmerMsgReq;

typedef struct
{
	int m_iRetCode;
}TGuiSetSerialAlarmerMsgRsp;

//切换语言重启
typedef struct
{
	int m_iRetCode;
}TGuiChangeLangRebootRsp;

//============================解码器协议部分start==============================
//解码器
#define GUI_DEC_CLARITY_CMD_REQ			3000  //解码器透传命令请求
#define GUI_DEC_CLARITY_CMD_RSP 		-3000 //解码器透传命令回应
#define GUI_DEC_CLARITY_DATA_LEN  		63
typedef struct
{
	int m_iDevId;								//设备编号dxl 2009/11/30
	int m_iVoChn;								//vo画面编号(1-16)
	int m_iDataSize;							//数据长度
	char m_cData[0];							//数据信息
}TGuiDecClarityReq; //解码器透传命令请求

#define GUI_DEC_CLARITY_OK   0 //透传成功
#define GUI_DEC_CLARITY_ERR  1 //透传失败
typedef struct
{
	int m_iRetCode;
}TGuiDecClarityRsp; //解码器透传命令回应


//解码器叠加logo
#define GUI_GET_DEC_LOGO_STAT_REQ     3001  //获取解码器叠加logo信息请求
#define GUI_GET_DEC_LOGO_STAT_RSP     -3001 //获取解码器叠加logo信息回应

#define GUI_BMP_FILE_NAME_LEN 63
typedef struct
{
	int m_iDspEnable;		//是否叠加
	int m_iXp;			//横坐标
	int m_iYp;			//纵坐标
	int m_iAlpha;			//透明度	
	int m_iTransparentColor;	//透明色
	char m_cBmpFileName[GUI_BMP_FILE_NAME_LEN + 1];	//BMP图片文件名
}TGuiDecDspLogoMsgRsp;

#define GUI_GET_DEC_COM_ADDR_REQ    3002  //获取解码器串口地址请求
#define GUI_GET_DEC_COM_ADDR_RSP    -3002 //获取解码器串口地址回应

typedef struct
{
	int m_iPortId;       //串口uart号
}TGuiDecComAddrReq; //获取解码器地址属性号

typedef struct
{
	int m_iAddr;    	//设备地址
}TGuiDecComAddrRsp; 	//获取解码器地址回应

#define GUI_DEC_PREVIEW_PAGE_UP		3003			//预览上一页
#define GUI_DEC_PREVIEW_PAGE_DOWN	3004			//预览下一页
//解码器上一页，下一页
typedef struct
{
	int m_iDevId;
	int m_iChn;
} TGuiDecoderPreviewPageUpReq;					//预览上翻页

typedef struct
{
	int m_iDevId;
	int m_iChn;
} TGuiDecoderPreviewPageDownReq;					//预览下翻页

#define GUI_DEC_PTZ_REMOTE_CODE			3005		//解码器生成的云台控码	

typedef struct									//云台控制本地控制码
{
	int m_iDevId;	//设备号							
	int m_iChn;	//通道号							
	int m_iCode;	//控制码值						
	int m_iParam[4];	//相关参数
} TGuiDecPtzLocalCode;

#define GUI_DEC_SET_NVS_PARA_REQ    3006   //UI设置NVS参数请求
#define GUI_DEC_SET_NVS_PARA_RSP    -3006  //UI设置NVS参数回应

typedef struct
{
    char            	m_serName[36];		//服务器名称(暂时保留)
    char            	m_serIP[36];		//服务器地址，可以是IP地址，也可以是域名；
    char            	m_serProxy[16];		//服务器上级代理
    unsigned char  	 	m_serChan;		//服务器通道
    unsigned char		m_serStream;		//服务器码流类型,主副码流
    unsigned char		m_serNetmode;		//传输协议1：TCP，2：UDP，3：多播；
    unsigned char		m_bUseddns;		//通过解析服务器连接
    unsigned short		m_serPort;		//服务器端口
    char            	m_username[20];		//用户名
    char            	m_password[20];		//密码
    int         		m_holdtime;		//保持时间，取值范围：5――60秒
    int         		m_bisView;
    int         		m_iAudioOut;		//音频解码是否输出
    char            	m_deviceType[16];   	//设备控制协议（需要翻译到的协议）
    unsigned char   	m_iAddress;         	//NVS设备地址（取值范围：1-255）
    unsigned char   	m_iAudioEType;         	//音频编码类型
    unsigned char   	m_cSubStreamFlag;       //设备是否有副码流0----无   1-----有   默认是0
    char            	m_cEncrypt[20];		//密码
    unsigned char		m_u8SerType;		//设备类型0为tiandy，1为onvif  ， 2为push流
} TDECNVSITEM; 

typedef struct
{
	int  m_iChan;   		//  通道(即输出设备号m_iDevId)
	int  m_iPic;      		// 画面(即通道号 m_iChn)
	TDECNVSITEM m_nvsItem;  //前端设备的连接信息
}TGuiDecSetNvsParaReq;

typedef struct
{
	int  m_iRetCode;   //设置返回, 0-表示成功，1-表示失败
}TGuiDecSetNvsParaRsp;

#define GUI_SET_DEC_AUDIO_CTRL_REQ  3007    //设置解码器音频控制请求
#define GUI_SET_DEC_AUDIO_CTRL_RSP  -3007   //设置解码器音频控制回应

#define GUI_DEC_AUDIO_ON   1
#define GUI_DEC_AUDIO_OFF  0

typedef struct
{
	int m_iChn;		//  通道(即输出设备号m_iDevId)
	int m_iPic;		// 画面(即通道号 m_iChn)
	int m_iCtrl;    // 1: GUI_DEC_AUDIO_ON 开  0:GUI_DEC_AUDIO_OFF 关
}TGuiSetDecAuidoCtrlReq;

typedef struct
{
	int m_iRetCode;
}TGuiSetDecAuidoCtrlRsp;


#define GUI_VIRTUAL_MATRIX_DATA_SEND_REQ  3010   //虚拟矩阵接收数据发送给内核请求
#define GUI_VIRTUAL_MATRIX_DATA_SEND_RSP  -3010 //虚拟矩阵接收数据发送给内核回应

#define GUI_VIRTUAL_MATRIX_DATA_LEN  255
typedef struct
{
	int m_iDataSize;   							   //数据长度
	char m_cData[GUI_VIRTUAL_MATRIX_DATA_LEN + 1]; //数据信息
}TGuiVirtualMatrixDataSendReq;

#define GUI_VIRTUAL_MATRIX_DATA_SEND_OK  0 //发送成功
#define GUI_VIRTUAL_MATRIX_DATA_SEND_ERR  1 //发送失败

typedef struct
{
	int m_iRetCode;
}TGuiVirtualMatrixDataSendRsp;


//解码器协议部分end

//*********************Miracle项目新增协议*********************//
//手动触发报警
#define GUI_GET_MANUAL_ALARM_LST_REQ  550    //获取手动报警列表请求
#define GUI_GET_MANUAL_ALARM_LST_RSP  -550   //获取手动报警列表回应

#define GUI_MANUAL_ALARM_NAME    31
typedef struct
{
	int m_iAlarmOutChn; //手动触发报警端口号
}TGuiManualGetAlarmOutReq;

typedef struct
{
	int m_iStat; //手动触发报警状态 0 
	char m_cAlarmOutName[GUI_MANUAL_ALARM_NAME + 1];//报警别名
	char m_cAlarmAddr[GUI_MANUAL_ALARM_NAME + 1];   //模拟通道为"本地",数字通道为"IP地址"
}TGuiManualAlarmOutMsg;

typedef struct
{
	int m_iAlarmOutChn; //手动触发报警端口号
	TGuiManualAlarmOutMsg m_tManAlarmOut;
}TGuiManualGetAlarmOutRsp;   //手动触发报警回应

#define GUI_SET_MANUAL_ALARM_REQ           551     //手动设置报警请求
#define GUI_SET_MANUAL_ALARM_RSP           -551    //设置手动报警回应

typedef struct
{
	int m_iAlarmOutChn;
	int m_iStat; // 1-报警，0 - 不报警
}TGuiSetManualAlarmOutReq;

typedef struct
{
	int m_iRetCode;
}TGuiSetManulAlarmOutRsp;

/********************************************************************/
//视频剪辑功能

#define GUI_VIDEO_CLIP_DISK_NOSAPCE  -3  //保存视频剪辑片断磁盘空间不足
#define GUI_VIDEO_CLIP_GENERIC_ERR    -1 //其他一般性错误

#define GUI_DO_VIDEO_CLIP_REQ    560  //视频剪辑开始请求
#define GUI_DO_VIDEO_CLIP_RSP    -560  //视频剪辑开始回应

#define GUI_VIDEO_CLIP_SAVE_PATH_LEN   127
typedef struct
{
	int m_iChn; //需要剪辑的通道
	int m_iStartTime; //开始时间
	int m_iEndTime;   //结束时间
	char m_cPath[GUI_VIDEO_CLIP_SAVE_PATH_LEN + 1];  //保存路径
}TGuiDoVideoClipReq;
typedef struct
{
	int m_iClipID;  //剪辑ID  据此ID 查询保存进度 
}TGuiDoVideoClipRsp;

#define GUI_VIDEO_CLIP_QUERY_PRO_REQ   561 //视频剪辑保存进度请求
#define GUI_VIDEO_CLIP_QUERY_PRO_RSP   -561  //视频 剪辑保存进度回应
typedef struct
{
	int m_iClipID;
}TGuiVideoClipQueryProReq;

typedef struct
{
	int m_iPro; //百分比
}TGuiVideoClipQueryProRsp;

//录像文件备份、剪辑备份一个文件的过程中中止命令
#define GUI_VIDEO_BACKUP_PRO_STOP_REQ   562 //视频剪辑一个文件的过程中中止请求
#define GUI_VIDEO_BACKUP_PRO_STOP_RSP   -562 //视频剪辑一个文件的过程中中止回应
typedef struct
{
	int m_iRetCode; //0 成功，1失败
}TGuiVideoBackupProStopRsp;

#define GUI_GET_CLIPFILE_MSG_REQ 		563	//获取录像剪辑大小请求
#define GUI_GET_CLIPFILE_MSG_RSP 		-563	//获取录像剪辑大小回应

typedef struct						//获取录像剪辑大小请求
{
	int m_iChn;					//通道
	int m_iStartTime;				//开始剪辑时间
	int m_iEndTime;				//结束剪辑时间
} TGuiGetClipFileMsgReq;

typedef struct						//获取录像剪辑大小回应
{
	TGuiGetClipFileMsgReq m_tGuiClipFileMsg;
	unsigned int m_uiFileSize;				//文件大小
}TGuiGetClipFileMsgRsp;


//磁盘检测与修复功能
#define GUI_GET_DISK_CHECK_REQ      566         //获取磁盘断电检测请求
#define GUI_GET_DISK_CHECK_RSP      -566        //获取磁盘断电检测请求回应
#define GUI_CHECK_ALL_DISK   1    //检测
#define GUI_UNCHECK_ALL_DISK  0   //不检测
#define GUI_REPAIR_DISK   1       //修复坏盘
#define GUI_UNREPAIR_DISK 0       //不修复坏盘
typedef struct
{
	int m_iCheckAllDisk;
	int m_iRepairDisk;   
}GuiGetDiskCheckRsp;

#define GUI_SET_DISK_CHECK_REQ       567        //设置磁盘断电检测请求
#define GUI_SET_DISK_CHECK_RSP       -567        //设置磁盘断电检测请求回应
typedef struct
{
	int m_iCheckAllDisk;
	int m_iRepairDisk;
}TGuiSetDiskCheckReq; //设置磁盘断电检测请求

typedef struct
{
	int m_iRetCode;
}TGuiSetDiskCheckRsp;  //设置磁盘断电检测请求回应

//====================盘组模式==============================
#define GUI_GET_STORAGE_MODE_REQ   	 	568  //获取存储模式请求(盘组或配额   )
#define GUI_GET_STORAGE_MODE_RSP 		-568 //获取存储模式回应
#define GUI_SET_STORAGE_MODE_REQ		569  //设置存储模式请求
#define GUI_SET_STORAGE_MODE_RSP		-569 //设置存储模式回应

enum
{
	DISK_GROUP = 1,   //盘组
	DISK_QUOTA,      //配额
}; //存储模式

typedef struct
{
	int m_iType;  //存储模式
}TGuiGetStorageModeRsp;   //获取存储模式回应(盘组或配额   )

typedef struct
{
	int m_iType;
} TGuiSetStorageModeReq; //设置存储模式请求

typedef struct
{
	int m_iRetCode;
} TGuiSetStorageModeRsp;//设置存储模式回应

//==================盘组设置===============================
#define GUI_GET_DISK_GROUP_REQ      570 	//获取盘组信息请求
#define GUI_GET_DISK_GROUP_RSP		-570	//获取盘组信息回应
#define GUI_SET_DISK_GROUP_REQ		571		//设置盘组信息请求
#define GUI_SET_DISK_GROUP_RSP		-571	//设置盘组信息回应

#define GUI_MAX_DISK_GROUP_CNT		8		//最大的盘组个数

typedef struct
{
	unsigned int m_iDiskLst[GUI_MAX_DISK_GROUP_CNT][8];     //硬盘列表
	unsigned int m_iChannelLst[GUI_MAX_DISK_GROUP_CNT][8];   //通道列表
} TGuiGetDiskGroupRsp; //获取盘组信息回应

typedef TGuiGetDiskGroupRsp TGuiSetDiskGroupReq;  //设置盘组信息请求

#define GUI_SET_DISK_GROUP_OK   0  //设置成功
#define GUI_SET_DISK_GROUP_ERR  1  //设置失败
typedef struct
{
	int m_iRetCode;
} TGuiSetDiskGroupRsp;  //设置盘组信息回应

//==================配额设置==============================
#define GUI_GET_DISK_QUOTA_REQ    572	//获取配额信息请求
#define GUI_GET_DISK_QUOTA_RSP    -572	//获取配额信息回应
#define GUI_SET_DISK_QUOTA_REQ	  573	//设置配额信息请求
#define GUI_SET_DISK_QUOTA_RSP    -573  //设置配额信息回应

typedef struct
{
	int m_iChn;  //通道号
}TGuiGetDiskQuotaReq;      //获取配额信息请求

typedef struct
{
	int m_iChn;
	unsigned int m_iTotalCap;  //所有磁盘的总容量(单位:G)
	unsigned int m_iRecUsed;   //已用录像配额(单位: M)
	unsigned int m_iPicUsed;    //已用图片配额(单位: M)
	unsigned int m_iRecQuota;  //录像配额(单位: G)
	unsigned int m_iPicQuota;    //图片配额(单位: G)
} TGuiGetDiskQuotaRsp;		//获取配额信息回应

typedef TGuiGetDiskQuotaRsp TGuiSetDiskQuotaReq;

typedef struct
{
	int  m_iRetCode;   //0表成功，1表失败
}TGuiSetDiskQuotaRsp;		//设置配额信息回应

//======================IPSAN=============
#define GUI_GET_IPSAN_MSG_REQ		574		//获取IPSAN信息请求
#define GUI_GET_IPSAN_MSG_RSP		-574	//获取IPSAN信息回应
typedef struct
{
	int m_iIPSANId;	//IPSAN编号(1-8)
}TGuiGetIPSANMsgReq;

typedef struct
{
	int m_iIPSANId;	//IPSAN编号(1-8)，0预留
	int m_iPort;			//端口号，默认预留
	char m_strIPSANIp[GUI_PUBLIC_LEN_63 + 1];
	char m_strIPSANPath[GUI_PUBLIC_LEN_63 + 1]; 
	char m_strUserName[GUI_PUBLIC_LEN_31+ 1];
	char m_strPasswd[GUI_PUBLIC_LEN_31+ 1];
} TGuiGetIPSANMsgRsp;

#define GUI_SET_IPSAN_MSG_REQ		575//设置IPSAN信息请求
#define GUI_SET_IPSAN_MSG_RSP		-575//设置IPSAN信息回应
typedef   TGuiGetIPSANMsgRsp TGuiSetIPSANMsgReq;

typedef struct
{
	int m_iRetCode;   //0表成功,1表失败
}TGuiSetIPSANMsgRsp;
  
#define GUI_DEL_IPSAN_REQ		577     //删除ipsan请求
#define GUI_DEL_IPSAN_RSP		-577   //删除ipsan回应
typedef TGuiGetIPSANMsgRsp  TGuiDeleteIpSanReq;
typedef TGuiSetIPSANMsgRsp   TGuiDeleteIpSanRsp;


#define GUI_SEARCH_IPSAN_TARGET_REQ		576//搜索IPSAN信息请求
#define GUI_SEARCH_IPSAN_TARGET_RSP		-576//搜索IPSAN信息回应
typedef struct
{
	int m_iPort;
	char m_strIPSanIP[GUI_PUBLIC_LEN_63 + 1];	
	char m_strUserName[GUI_PUBLIC_LEN_31 + 1];
	char m_strPasswd[GUI_PUBLIC_LEN_31 + 1];
}TGuiSearchIpSanTargetReq;

typedef struct
{
	int m_iTargetCnt;     //先返回条数,然后逐条获取
}TGuiSearchIpSanRsp;

typedef struct
{
	char m_strIPSanIP[GUI_PUBLIC_LEN_63 + 1];	
	char m_strIpSanTarget[GUI_PUBLIC_LEN_63 + 1];
}TGuiSearchIpSanTargetMsg;

#define GUI_SET_UNLOADDISK_REQ			578   //磁盘热插拔请求
#define GUI_SET_UNLOADDISK_RSP			-578 //磁盘热插拔回应

typedef struct
{
	int m_iAction;        // 1- 下线 2-上线
	int m_iDiskId;       //磁盘编号
}TGuiSetUnloadDiskReq;

typedef struct
{
	int m_iRetCode;   //0表成功 其余表失败
}TGuiSetUnloadDiskRsp;


//==================热备配置================
#define GUI_GET_DEV_WORKMODE_REQ     2040  //获取设备工作模式请求
#define GUI_GET_DEV_WORKMODE_RSP 	-2040  //获取设备工作模式回应
enum
{
	GUI_DEV_NORMAL_MODE = 0,   //普通模式
	GUI_DEV_HOTSPARE_MODE,     //热备模式
};

typedef struct
{
	int m_iDevWorkMode;
}TGuiGetDevWorkModeRsp;

#define GUI_SET_DEV_WORKMODE_REQ    2041  //设置设备工作模式请求
#define GUI_SET_DEV_WORKMODE_RSP    -2041 //设置设备工作模式回应
typedef TGuiGetDevWorkModeRsp TGuiSetDevWorkModeReq;
typedef struct
{
	int m_iRetCode;	 //0表成功,1表失败
}TGuiSetDevWorkModeRsp;

//工作机上的设置
#define GUI_GET_HOT_SPARE_ENABLE_REQ       2042    //获取热备使能请求
#define GUI_GET_HOT_SPARE_ENABLE_RSP		-2042  //获取热备使能回应　
typedef struct
{
	int m_iHotSpareEnable ;					//是否启用热备
}TGuiGetHotSpareEnabelRsp;

#define GUI_SET_HOT_SPARE_ENABLE_REQ       2043    //设置热备使能请求
#define GUI_SET_HOT_SPARE_ENABLE_RSP		-2043  //设置热备使能回应　
typedef TGuiGetHotSpareEnabelRsp TGuiSetHotSpareEnableReq;
typedef struct
{
	int m_iRetCode;	 //0表成功,1表失败
}TGuiSetHotSpareEnableRsp;

//搜索工作机或者热备机
#define GUI_HOTSPARE_SEARCH_START_REQ      2044  //搜索请求开始
#define GUI_HOTSPARE_SEARCH_START_RSP	  -2044  //搜索请求回应
typedef struct		//搜索数字通道请求
{
	int m_iDevType;	
} TGuiSearchHotSpareReq;

typedef struct
{
	int m_iSid;	//搜索查询号，以后按此号获取搜索结果，-1表失败
} TGuiSearchHotSpareRsp;


#define GUI_SEARCH_DEVICE_CNF_REQ		2045//获取工作机/热备机配置请求
#define GUI_SEARCH_DEVICE_CNF_RSP		       -2045//获取工作机/热备机配置回应

typedef struct
{
	int m_iDevType;       //设备类型 0表示工作机，1表示热备机
	int m_iSid;               //使用TGuiSearchHotSpareRsp返回的m_iSid
}TGuiGetDevCntReq;

typedef struct
{
	int m_iDevCnt;
}TGuiGetDevCntRsp;   //先返回个数，再逐条返回设备ip地址

typedef struct
{
	char m_strDevIp[GUI_PUBLIC_LEN_63 + 1];
}TGuiDevMsg;

#define GUI_ADD_DEVICE_REQ      2046   //添加热备机或者工作机请求
#define GUI_ADD_DEVICE_RSP	      -2046 //添加热备机或者工作机回应

typedef struct
{
	int m_iDevType;      //设备类型 0表示工作机，1表示热备机          
	char m_strDevIp[GUI_PUBLIC_LEN_63 + 1];
	char m_strPasswd[GUI_PUBLIC_LEN_31 + 1];
}TGuiAddDeviceReq;

typedef struct
{
	int m_iRetCode;	 //0表成功,1表失败
}TGuiAddDeviceRsp;

//热备机添加了多少工作机
#define GUI_GET_HOT_SPARE_CNF_REQ		2047//获取热备配置请求
#define GUI_GET_HOT_SPARE_CNF_RSP		-2047//获取热备配置回应
typedef struct
{
	int m_iHotSpareDevType;       //设备类型 0表示工作机，1表示热备机
}TGuiGetHotSpareDevCntReq;

typedef struct
{
	int m_iHotSpareDevCnt;
}TGuiGetHotSpareDevCntRsp;

typedef  struct
{
	char m_strHotSpareIp[GUI_PUBLIC_LEN_63 + 1];
}TGuiHotSpareDevMsg;

#define GUI_GET_DEV_STAT_REQ 		2048 //获取热备机或者工作机工作状态请求
#define GUI_GET_DEV_STAT_RSP 		-2048 //获取热备机或者工作机工作状态回应

enum
{
	GUI_DEV_ONLINE = 0,       //设备在线
	GUI_DEV_OFFLINE,           //设备不在线
};

enum
{
	GUI_WORK_DEV_STAT_NORMAL = 0,//正在热备
	GUI_WORK_DEV_STAT_BACKUP,//正在备份
	GUI_WORK_DEV_STAT_SYNCING ,//正在同步
	GUI_WORK_DEV_STAT_OFFLINE,   //掉线，但显示正在热备
};  //在热备机上显示工作机状态

enum
{
	GUI_HOT_BACKUP_STAT_LINK_FAILED = 0,   //连接失败
	GUI_HOT_BACKUP_STAT_LINK_SUCCED ,	  //连接成功
	GUI_HOT_BACKUP_STAT_SYNCING,			//正在同步
};//在工作机上显示的热备机状态

typedef struct
{
	int m_iDevType;
	char m_strDevIp[GUI_PUBLIC_LEN_63 + 1];
}TGuiGetDeviceStatReq;

typedef struct
{
	int m_iDevType;
	char m_strDevIp[GUI_PUBLIC_LEN_63 + 1];
	int m_iStat;
	int m_iStatPara;    //同步百分比(0-100)
}TGuiGetDeviceStatRsp;

#define GUI_DEL_DEV_REQ   	2049   //删除设备机或者工作机请求
#define GUI_DEL_DEV_RSP	-2049  //删除设备机或者工作机回应
typedef struct
{
	int m_iDevType;
	char m_strDevIp[GUI_PUBLIC_LEN_63 + 1];
}TGuiDelDevReq;

typedef struct
{
	int m_iRetCode;  //0表成功,1表失败
}TGuiDelDevRsp;


//=====================Raid相关==================
#define GUI_GET_RAID_ENABLE_REQ    2060  //获取raid使能请求
#define GUI_GET_RAID_ENABLE_RSP     -2060 //获取raid使能回应

typedef  struct
{
	int m_iRaidEnable;     // 1表示使能
}TGuiGetRaidEnableRsp;

#define GUI_SET_RAID_ENABLE_REQ      2061     //设置raid使能请求
#define GUI_SET_RAID_ENABLE_RSP	 -2061	//设置raid使能回应
typedef struct
{
	int m_iRaidEnable;
}TGuiSetRaidEnableReq;

typedef struct
{
	int m_iRetCode;
}TGuiSetRaidEnableRsp;

#define GUI_GET_RAID_SATA_LIST_REQ 	  2062    //获取物理磁盘请求
#define GUI_GET_RAID_SATA_LIST_RSP	 -2062	//获取物理磁盘回应
typedef struct
{
	int m_iSataDiskCnt;
}TGuiGetRaidSataListRsp;  //先返回磁盘个数，再逐条返回TGuiSataDiskMsg信息


enum
{
	GUI_DISK_NOR_TYPE = 0,   		//普通盘
	GUI_DISK_RAID_BACKUP_TYPE, 	 //阵列热备
	GUI_DSIK_GLOBAL_BACKUP_TYPE,    //全局热备
	GUI_DISK_RAID_TYPE,			//阵列盘
	GUI_INVALID_DISK_RAID_TYPE,    //无效阵列盘
	GUI_FAIL_DISK_TYPE,			//故障盘
};

enum
{
	GUI_DISK_SMART_GOOD = 0,    //正常
	GUI_DISK_SMART_BAD,         //异常
	GUI_DISK_SMART_LOWHEALTH,   //健康预警
};

typedef struct
{
	int m_iDiskId;
	int m_iDiskType;
	int m_iDiskStat;
	int m_iDiskSize; //单位为M
	char m_strRaidName[GUI_PUBLIC_LEN_31 + 1];
	char m_strDiskModel[GUI_PUBLIC_LEN_31 + 1];
}TGuiSataDiskMsg;

#define GUI_SET_DISK_BACKUP_REQ    2063      //磁盘设置热备请求
#define GUI_SET_DISK_BACKUP_RSP	-2063	//磁盘设置热备回应
typedef struct
{
	int m_iDiskId;
	int m_iHotSpareType;   //热备，分全局或者阵列
	char m_strRaidName[GUI_PUBLIC_LEN_31 + 1];  //如果为全局热备，则字符串为空
}TGuiSetDiskBackupTypeReq;

typedef struct
{
	int m_iRetCode;
}TGuiSetDiskBackupTypeRsp;

//raid类型枚举
enum
{
	GUI_RAID_JBOD = 100,
	GUI_RAID_RIAD0 = 0,
	GUI_RAID_RIAD1 = 1,
	GUI_RIAD_RAID5 = 5,
	GUI_RAID_RAID10 = 10,
};

#define GUI_CREATE_RAID_REQ   		2064   //创建raid请求
#define GUI_CREATE_RAID_RSP  		-2064  //创建raid回应
typedef  struct
{
	int m_iRaidType;
	int m_iDisklist; //按位表示 (0~31表示32块硬盘)
	int m_iDisklist1; //备用
	char m_strRaidName[GUI_PUBLIC_LEN_31 + 1];
}TGuiRaidBasicMsg;

typedef struct
{	
	TGuiRaidBasicMsg m_tRaidMsg;
}TGuiCreateRaidReq; 

typedef struct
{	
	int m_iRetCode;     //0表成功1表失败
}TGuiCreateRaidRsp;

#define GUI_REBUILD_RAID_REQ   		2065   //重建raid请求
#define GUI_REBUILD_RAID_RSP    	-2065 //重建raid回应

typedef struct
{	
	TGuiRaidBasicMsg m_tRaidMsg;
}TGuiRebuildRaidReq; 

typedef struct
{
	int m_iRetCode; 	//0表成功1表失败
}TGuiRebuildRaidRsp;

#define GUI_DEL_RAID_REQ   			2066//删除raid请求
#define GUI_DEL_RAID_RSP  			-2066//删除raid回应

typedef struct
{	
	char m_strRaidName[GUI_PUBLIC_LEN_31 + 1];
}TGuiDelRaidReq; 

typedef struct
{
	int m_iRetCode; 	//0表成功1表失败
}TGuiDelRaidRsp;

#define GUI_GET_RAID_MSG_REQ   2067  //获取raid信息请求
#define GUI_GET_RAID_MSG_RSP   -2067 //获取raid信息回应
typedef struct
{
	int m_iRaidCnt;     //返回个数，再跟TGuiRaidMsg信息
}TGuiGetRaidMsgRsp;  

typedef enum
{
	GUI_RAID_STAT_NORMAL =1,		//正常
	GUI_RAID_STAT_DOWNGRADE,     //降级
	GUI_RAID_STAT_DAMAGE,		//损坏
}TGuiRaidStatus;

typedef enum
{
	GUI_RAID_TASK_NONE = 1,      	//无
	GUI_RAID_TASK_INITING,       	//正在初始化
	GUI_RAID_TASK_REBUILDING,	//正在重建
}TGuiRaidTaskType;

typedef struct
{
	int m_iRaidId;
	int m_iRaidAllSize;
	int m_iRiadUseAbleSize;
	int m_iDiskList;
	int m_iDiskList1;
	int m_iDiskBackUpId;  //-1表示没有热备盘
	int m_iRaidStat;   //阵列状态 TGuiRaidStatus
	int m_iRaidType;
	int m_iRaidTask;  //阵列任务TGuiRaidTaskType
	int m_iRaidTaskPara[4]; //预留，百分比，剩余时间等
	char m_strRaidName[GUI_PUBLIC_LEN_31 + 1];
}TGuiRaidMsg;

//===================虚拟磁盘=============
#define GUI_CREATE_VIRTUALDISK_REQ       2070  //创建虚拟磁盘请求
#define GUI_CREATE_VIRTUALDISK_RSP	   -2070	//创建虚拟磁盘会用
enum
{
	GUI_RAID_VD_INIT_FG = 1,   //前台初始化
	GUI_RAID_VD_INIT_BG,	    //后台初始化
	GUI_RAID_VD_INIT_FAST,    //快速初始化
};

typedef struct
{
	int m_iVdSize;
	int m_iInitType;
	char m_strVdName[GUI_PUBLIC_LEN_31+1];
	char m_strRaidName[GUI_PUBLIC_LEN_31+1];
}TGuiRaidCreateVdReq;

typedef struct
{
	int m_iRetCode;
}TGuiRaidCreateVdRsp;

#define GUI_GET_VD_MSG_REQ   	2071//获取虚拟磁盘信息请求
#define GUI_GET_VD_MSG_RSP   	-2071//获取虚拟磁盘信息回应

 typedef  enum
{
	GUI_VD_STAT_NORMAL = 1,   //正常
	GUI_VD_STAT_DEGRADE,      //降级
	GUI_VD_STAT_OFFLINE,        // 下线
}TGuiVdStatType;

typedef enum
{
	GUI_VD_TASK_NONE = 1,    //无
	GUI_VD_TASK_INITING,     //正在初始化
	GUI_VD_TASK_FIXING,     //正在修复
}TGuiVdTaskType;
typedef struct
{
	int m_iRaidVdCnt;
}TGuiGetVdMsgRsp;

typedef struct
{
	int m_iRaidVdSize;
	int  m_iVdStat;       //虚拟磁盘状态TGuiVdStatType
	int m_iRaidtype;    //虚拟磁盘所属的raid类型
	int m_iVdTask;      //虚拟磁盘任务类型TGuiVdTaskType
	int m_iVdTaskPara[4];  //任务预留参数，包括百分比，剩余时间等
	char m_strVdName[GUI_PUBLIC_LEN_31 + 1];
	char m_strRaidName[GUI_PUBLIC_LEN_31 + 1];
}TGuiVdMsg;

#define GUI_FIX_VD_REQ   	2072  //修复虚拟磁盘请求
#define GUI_FIX_VD_RSP  	-2072  //修复虚拟磁盘回应

typedef struct
{
	char m_strVdName[GUI_PUBLIC_LEN_31 + 1];
}TGuiFixVdReq;

typedef struct
{
	int m_iRetCode;
}TGuiFixVdRsp;

#define GUI_DEL_VD_REQ   	2073 //删除虚拟磁盘请求
#define GUI_DEL_VD_RSP		-2073 //删除虚拟磁盘回应

typedef struct
{
	char m_strVdName[GUI_PUBLIC_LEN_31 + 1];
}TGuiDelVdReq;

typedef struct
{
	int m_iRetCode;
}TGuiDelVdRsp;


//==================/*混音相关add  by quxinfeng 2012.2.7*/============
#define GUI_SET_MIXAUDIO_REQ       20000
#define GUI_SET_MIXAUDIO_RSP 		-20000 	
#define GUI_GET_MIXAUDIO_REQ       20001
#define GUI_GET_MIXAUDIO_RSP 		-20001
#define MIXAUDIO_OPERATER_OK     	0
#define GUI_MAX_AUDIO_SOURCE  		4
typedef struct
{
	int m_iChn;		
}TGuiGetMixAudioReq;

typedef struct
{	 int m_iChn;
	 int m_iMixAudioEnable;//是否混音
	 int m_iMixAudioSourceChnNo[GUI_MAX_AUDIO_SOURCE];//声源
}TGuiGetMixAudioRsp;

#define TGuiSetMixAudioReq   TGuiGetMixAudioRsp

typedef struct
{
	int m_iRetCode;		// 0表示成功
}TGuiSetMixAudioRsp;

#define GUI_SET_AUDIO_THRESHOLD_REQ   20002  //设置音频阈值请求
#define GUI_SET_AUDIO_THRESHOLD_RSP   -20002  //设置音频阈值回应
typedef struct
{
	int m_iChn;
	int m_iThreshold;	 //阈值
}TGuiSetAudioThresholdReq;

typedef struct
{
	int m_iRetCode;
}TGuiSetAudioThresholdRsp; 

#define GUI_GET_AUDIO_THRESHOLD_REQ   20003  //获取音频阈值请求
#define GUI_GET_AUDIO_THRESHOLD_RSP   -20003  //获取音频阈值回应
typedef struct
{
	int m_iChn;
}TGuiGetAudioThresholdReq;

typedef struct
{
	int m_iChn;
	int m_iThreshold;  //阈值
}TGuiGetAudioThresholdRsp;
//****************************************//
//设置摄像机高清参数

typedef struct
{
	int m_iStructLenth;//结构体长度,高16位为快门速度
	/*0--1/25、1--1/50、2--1/60、3--1/80、4--1/100、5--1/120、6--1/125、7--1/150、8--1/180、9--1/200、10--1/250、
	11--1/500、12--1/1000、13--1/2000、14--1/4000、15--1/10000、16--1/30、17-1/100000、18-1/15、19-1/12、20-1/10、
	21-1/8、22-1/6、23-1/5、24-1/4、25-1/3、26-1/2、27-1；*/
  int m_iExposureMode;//曝光模式:0,手动;1,自动
  int m_iExpPriority;//曝光策略:0，噪声优先;1，帧率优先
  int m_iExpCompensiton;//目标亮度0-255
  int m_iExpMaxTime;//最大曝光时间，单位us
  int m_iExpMinTime;//最小曝光时间，单位us
	int m_iExpMaxGain;//最大增益，单位 0.01倍，对应网络协议里的增益0~255
  int m_iExpMinGain;//最小增益，单位 0.01倍
  TGuiArea m_tExpArray;//曝光区域
  int m_iIrisMode;//光圈模式:0，手动；1，自动
	int m_iWBMode;//白平衡模式:0，手动;1,自动， 高16位代表相应模式下的 各种类型
	/*自动模式下0-默认自动1-晴天模式2-日光灯模式3-暖光灯模式4-白炽灯模式5-自然光模式
	6-锁定白平衡 手动模式下0-半自动 1-纯手动*/
  int m_iWBRGain;//手动模式下的红增益:0-255
  int m_iWBBGain;//手动模式下的蓝增益:0-255
	int m_iWDRMode;//宽动态模式:1，打开；2关闭 ；	低16位表示自动(1)或手动(0)，高16位表示开启状态开启(1)关闭(0)可视增强(2)
	int m_iWDRLevel;//宽动态级别（保留）范围是0-255
  int m_iBLCMode;//背光补偿模式:1，打开；2关闭
  int m_iBLCLevel; //背光补偿级别
  int m_iIRMode;//红外滤光片模式:0，自动;1,打开;2，关闭
  int m_iSharpness;//锐度0-255
	int m_iNoise;//噪声0-255 ，最高8位表示噪声模式，最低8位表示2D噪声值，8-15位表示3D噪声值
  int m_iGamma;//gamma值*100
}TGuiVideoISPInfo;

#define GUI_GET_VIDEO_ISP_INFO_REQ  4000 //获取摄像机高清参数请求
#define GUI_GET_VIDEO_ISP_INFO_RSP	-4000 //获取摄像机高清参数请求回应
typedef struct
{
	TGuiVideoISPInfo m_tVideoISPInfo;
}TGuiGetVideoISPInfoRsp; //获取摄像机高清参数请求回应

#define GUI_SET_VIDEO_ISP_INFO_REQ   4001 //设置摄像机高清参数请求
#define GUI_SET_VIDEO_ISP_INFO_RSP   -4001 //设置摄像机高清参数回应

typedef struct
{
	TGuiVideoISPInfo m_tVideoISPInfo;
}TGuiSetVideoISPInfoReq; //设置摄像机高清参数请求

#define GUI_SET_VIDEO_ISP_INFO_OK  0 //设置摄像机高清参数成功
#define GUI_SET_VIDEO_ISP_INFO_ERR	1 //设置摄像机高清参数失败

typedef struct
{
	int m_iRetCode;
}TGuiSetVideoISPInfoRsp; //设置摄像机高清参数回应

#define GUI_GET_VIDEO_ISP_INFO_REQ_EX      4002 //获取摄像机高清参数请求
#define GUI_GET_VIDEO_ISP_INFO_RSP_EX 	-4002//获取摄像机高清参数请求回应

typedef struct
{
	int m_iChn; // 通道号
}TGuiGetVideoISPInfoReqEx;

typedef struct
{
	int m_iChn;                              //通道号
	TGuiVideoISPInfo m_tVideoISPInfo;
}TGuiGetVideoISPInfoRspEx; //获取摄像机高清参数请求回应

#define GUI_SET_VIDEO_ISP_INFO_REQ_EX   4003 //设置摄像机高清参数请求
#define GUI_SET_VIDEO_ISP_INFO_RSP_EX   -4003 //设置摄像机高清参数回应

typedef struct
{
	int m_iChn;
	TGuiVideoISPInfo m_tVideoISPInfo;
}TGuiSetVideoISPInfoReqEx; //设置摄像机高清参数请求

typedef struct
{
	int m_iRetCode;
}TGuiSetVideoISPInfoRspEx; //设置摄像机高清参数回应


//=====================================================
//设置S3E 视频翻转参数
//翻转类型  UI上的视频翻转给内核发枚举类型3即可
typedef enum
{
	GUI_VIDEO_TURN_H = 1,		//水平翻转
	GUI_VIDEO_TURN_V,			//垂直翻转
	GUI_VIDEO_TURN_R,			//倒置翻转
}TGuiVideoTurnType;

#define GUI_SET_VIDEO_TURN_TYPE_REQ  4101 //设置视频翻转类型请求
#define GUI_SET_VIDEO_TURN_TYPE_RSP  -4101 //设置视频翻转类型请求回应

typedef struct
{
	int m_iChn;
	int m_iType; 		//翻转类型
}TSetVideoTurnTypeReq; //设置视频翻转类型请求

typedef struct
{
	int m_iRetCode;     //使用公共设置成功失败返回码
}TSetVideoTurnTypeRsp; //设置视频翻转类型请求回应

//=====================================================
//===============交通主机新增业务===============

//获取存储路径列表(新加)(同步通信)
#define GUI_GET_DEV_STORAGE_REQ    30000   //获取存储设备请求
#define GUI_GET_DEV_STORAGE_RSP    -30000   //获取存储设备回应

typedef struct 	//获取存储设备请求
{
	int m_iDevType; //磁盘类型
}TGuiGetStorageReq;

typedef struct		//获取存储设备回应
{
	int    m_iStorageCnt;	// 存储设备总数
}TGuiGetStorageRsp;


typedef struct 		//磁盘列表项
{
	int m_iDevType;  //磁盘类型
	int m_iStorageId; // 存储设备编号
	char m_pcStorageName[4][GUI_PUBLIC_LEN_31 + 1]; //Usb存储设备路径名称，这个路径名称是指的什么？
}TGuiStorageListItem;
 

//获取当前可用磁盘路径(新加) (同步通信)
#define GUI_GET_APP_STORAGE_PATH_REQ   30001  //获取外挂可用磁盘路径请求
#define GUI_GET_APP_STORAGE_PATH_RSP   -30001  //获取外挂可用磁盘路径回应

#define GUI_GET_APP_STORAGE_PATH_OK			0	//获取可用存储空间成功
#define GUI_GET_APP_STORAGE_PATH_ERR		1	//其他原因失败
#define GUI_GET_APP_STORAGE_PATH_CRITICAL	2	//可用空间已到临界值
#define GUI_GET_APP_STORAGE_PATH_FULL		3	//可用空间满
#define GUI_GET_APP_STORAGE_PATH_NOT_READY	4	//可用空间未准备好

//请求消息无
typedef struct  //回应
{
	int iRetCode;					//错误码
	char cPath[GUI_PUBLIC_LEN_255 + 1];
}TGuiGetAppStoragePathRsp;


//获得设备状态（车检器、线圈、相机温度）(新加)(异步通信)
#define GUI_GET_DEVICE_STATE_REQ    30002   //获取设备状态请求
#define GUI_GET_DEVICE_STATE_RSP    -30002   //获取设备状态回应
//之前车检器定义为0，为了和交通相机网络协议一致，改为1
#define    GUI_REDLIGHT_STATUS 0
#define    GUI_SENSOR_STATUS    1    //车检器状态
#define    GUI_LOOP_STATUS      2    //线圈状态
#define    GUI_CAMERA_TEMP     3     //相机温度
#define    GUI_REDLIGHT_SIGCHEACK_STATUS     4     //查询红灯信号检测器状态

typedef struct		//获取设备状态请求
{
	int  m_iChn;    //通道号(0-n,0表示针对全部通道)
	int  m_iDeviceType; //设备类型，0:红灯 1:车检器；2:线圈；3:相机温度
} TGuiGetDeviceStateReq;

#define    GUI_DEVICE_STATE_OK    0    //成功
#define    GUI_DEVICE_STATE_ERR   -1    //失败

typedef struct		
{
	int  m_iRet;
}TGuiGetDeviceStateRsp;

//录像裁剪(新加) (异步通信)
#define GUI_CUTOUT_VIDEO_REC_REQ    30003   //录像裁剪请求
#define GUI_CUTOUT_VIDEO_REC_RSP    -30003   //录像裁剪回应

#define GUI_CUTOUT_VIDEO_OK    0     //成功
#define GUI_CUTOUT_VIDEO_ERR   -1    //失败

typedef struct		//录像裁剪请求
{
	int  m_iChn;    //通道号(0-n,0表示针对全部通道)
	int  m_iBeginTime; //开始时间
	int  m_iEndTime;  //结束时间
	char m_cPath[GUI_PUBLIC_LEN_255 + 1];	//路径+文件名
} TGuiCutOutVideoRecReq; //内核完成裁剪后发送该结构体

typedef struct		//录像裁剪回应
{
	int  m_iChn;        //通道号(0-n,0表示针对全部通道)
	int  m_iRetCode;    // 0成功，-1失败
}TGuiCutOutVideoRecRsp;

//异步回应消息：
#define GUI_SEND_DEVICE_STATE_REQ    10005   //发送设备状态到对接模块
#define GUI_SEND_DEVICE_STATE_RSP    -10005   //获取设备状态到对接模块

typedef struct		//设备状态结构体
{
	int  m_iDeviceType;   //设备类型，0:红灯 1:车检器；2:线圈；3:相机温度
	int  m_iDevState[4];    //状态
}TGuiDeviceState;

typedef struct		//获取设备状态回应
{
	int  m_iChn;    //通道号(0-n,0表示针对全部通道)
	TGuiDeviceState  m_tDevState; //设备状态结构体
}TGuiSendDeviceStateReq;


//异步回应消息：
#define GUI_SEND_CUTOUT_VIDEO_REQ    10001   //发送裁剪结果到对接模块
#define GUI_SEND_CUTOUT_VIDEO_RSP    -10001   //发送裁剪结果到对接模块

#define GUI_SEND_CUTOUT_VIDEO_OK	0 //裁剪成功
#define GUI_SEND_CUTOUT_VIDEO_ERR	1 //其他原因裁剪失败
#define GUI_SEND_CUTOUT_VIDEO_NOT_EXIST	2 //录像文件不存在
#define GUI_SEND_CUTOUT_VIDEO_DISK_FULL	3 //磁盘空间不足

 //内核完成裁剪后发送该结构体TGuiCutOutVideoRecReq

typedef struct
{
	int m_iRetCode;
	TGuiCutOutVideoRecReq m_tGuiCutOutVideoRec;
}TGuiSendCutOutVideoRsp;

//获取交通违法报警录像消息 (新加) (异步通信)
//异步回应消息：
#define GUI_SEND_ITS_ILLEGAL_RECORD_REQ    10002   //发送违法录像消息到对接模块
#define GUI_SEND_ITS_ILLEGAL_RECORD_RSP    -10002   //发送违法录像消息到对接模块

typedef struct tagNVS_FILE_TIME_V1
{
	unsigned short iYear;   /* Year */
	unsigned short iMonth;  /* Month */
	unsigned short iDay;    /* Day */
	unsigned short iHour;   /* Hour */
	unsigned short iMinute; /* Minute */
	unsigned short iSecond; /* Second */
	unsigned short iMillisecond; /* Millisecond */
} NVS_FILE_TIME_V1,*PNVS_FILE_TIME_V1;

typedef struct		//录像消息结构体
{
	int	 iBufSize;	//混合触发结构体大小
	long lCarID;          //车辆ID
	int  iRecordFlag;     //操作类型0:停止,1:开始，2:取消
	int  iIllegalType;     //违法类型
	int  iPreset;         //场景id
	char cCrossingID[GUI_PUBLIC_LEN_63 + 1];		 //路口编号 64个字符
	char cLaneID[GUI_PUBLIC_LEN_63 + 1];			 //车道编号	64个字符
	NVS_FILE_TIME_V1 m_tSnapTime;//抓拍开始时间(第一张图片抓拍时间) 
}TGuIllegalRecordMsg;

typedef struct		 //内核发送的违法录像消息
{
	int 	m_iChn;	//通道号(0-n,0表示针对全部通道)
	TGuIllegalRecordMsg tRecMsg; //录像消息
}TGuiIllegalRecReq;

//=====================================================


//------------------------------------------------------------------
//车载DVR电源管理
#define GUI_GET_POWER_CNF_REQ   200000     //获取电源管理配置请求
#define GUI_GET_POWER_CNF_RSP    -200000    //获取电源管理配置回应
#define GUI_SET_POWER_CNF_REQ    200001    //设置电源管理配置请求
#define GUI_SET_POWER_CNF_RSP    -200001   //设置电源管理配置回应
typedef struct 
{
	int m_iOpenEnable;
	int m_iOpenDevDelayTime;
	int m_iCloseEnable;
	int m_iCloseDevDelayTime;
}TGuiPowerManageMsg;

typedef struct								//获取电源管理回应	
{
	TGuiPowerManageMsg m_tPowerManageMsg;						
} TGuiGetPowerManageCnfRsp;

typedef struct                          //设置电源管理请求
{
	TGuiPowerManageMsg m_tPowerManageMsg;
}TGuiSetPowerManageCnfReq;

#define GUI_SET_POWER_MANAGE_CNF_OK			0    //设置成功
#define GUI_SET_POWER_MANAGE_CNF_ERR		1    //设置失败
typedef struct                           //设置电源管理回应
{
	int m_iRetCode;
}TGuiSetPowerManageCnfRsp;

//============================ 

typedef struct 
{
	int m_iPoweroffVoltage;  	//关机低压值，需乘以10
	int m_iAlarmVoltage;		//低压报警值，需乘以10
}TGuiPowerAlarmMsg;

#define GUI_GET_POWER_ALARM_MSG_REQ	200002	//获取关机电压和报警电压请求
#define GUI_GET_POWER_ALARM_MSG_RSP	-200002	//获取关机电压和报警电压回应

typedef struct 
{
	TGuiPowerAlarmMsg m_tPowerAlarmMsg;
}TGuiGetPowerAlarmMsgRsp;

#define GUI_SET_POWER_ALARM_MSG_REQ	200003	//设置关机电压和报警电压请求
#define GUI_SET_POWER_ALARM_MSG_RSP	-200003	//设置关机电压和报警电压回应

typedef struct 
{
	TGuiPowerAlarmMsg m_tPowerAlarmMsg;
} TGuiSetPowerAlarmMsgReq;

typedef struct								
{	
	int m_iRetCode;
}TGuiSetPowerAlarmMsgRsp;

//============================ 
//FTP上传配置(该部分协议废止)
#define GUI_STR_SERVER_NAME_LEN    63
typedef struct
{
	char	m_cServerName[GUI_STR_SERVER_NAME_LEN + 1];       //服务器地址
	char    m_cServerPath[GUI_STR_SERVER_NAME_LEN + 1];       //在服务器上得路径
	char 	m_cUserName[GUI_IP_STR_LEN + 1];         //用户名
	char	m_cPassword[GUI_IP_STR_LEN + 1];			//密码
	int		m_iPort;					//端口号
	int     m_iAutoFtpEnable;                 //自动上传是能
}TGuiFTPServerParaMsg;

#define GUI_GET_FTP_CNF_REQ       200004   //获取FTP上传配置请求
#define GUI_GET_FTP_CNF_RSP       -200004  //获取FTP上传配置回应
typedef struct									//获取FTP上传服务参数回应
{
	TGuiFTPServerParaMsg m_tFTPParaMsg;
}TGuiGetFTPServerParaCnfRsp;

#define GUI_SET_FTP_CNF_REQ       200005   //设置FTP 上传配置请求
#define GUI_SET_FTP_CNF_RSP       -200005  //设置FTP上传配置回应
#define GUI_UP_FTP_CNF_REQ        200006   //上传命令请求
#define GUI_UP_FTP_CNF_RSP        -200006  //上传命令回应
typedef struct                               //设置FTP上传服务参数请求
{
	TGuiFTPServerParaMsg m_tFTPParaMsg;
}TGuiSetFTPServerParaCnfReq;

#define GUI_SET_FTP_PARA_CNF_OK			0   //设置成功
#define GUI_SET_FTP_PARA_CNF_ERR			1   //设置失败
typedef struct								//设置FTP上传服务参数回应
{
	int m_iRetCode;
}TGuiSetFTPServerParaCnfRsp;

//============================ 
#define GUI_GET_WIFI_AUTO_FTP_STAT_REQ	200101	//获取wifi自动ftp上传使能状态请求
#define GUI_GET_WIFI_AUTO_FTP_STAT_RSP	-200101	//获取wifi自动ftp上传使能状态回应
typedef struct								
{	
	int m_iEnable; //见公共宏定义GUI_SET_ENABLE
}TGuiGetAutoFtpStatRsp;

#define GUI_SET_WIFI_AUTO_FTP_STAT_REQ	200102	//设置wifi自动ftp上传使能状态请求
#define GUI_SET_WIFI_AUTO_FTP_STAT_RSP	-200102	//设置wifi自动ftp上传使能状态回应

typedef struct								
{	
	int m_iEnable; //见公共宏定义GUI_SET_ENABLE
}TGuiSetAutoFtpStatReq;

typedef struct								
{	
	int m_iRetCode;
}TGuiSetAutoFtpStatRsp;

//============================ 
#define GUI_SEND_WIFI_FTP_UP_CMD_REQ        200103   //发送手动上传命令请求
#define GUI_SEND_WIFI_FTP_UP_CMD_RSP        -200103  //发送手动上传命令回应

typedef struct								
{	
	int m_iRetCode;
}TGuiSendWifiFtpUpCmdRsp;
//============================ 




//------------------------------------------------------------------
//短信管理
#define GUI_SMS_COUNT_REQ        200009   //统计短信总数请求
#define GUI_SMS_COUNT_RSP        -200009  //统计短信总数回应
#define GUI_SMS_QUERY_REQ         200010  //查询短信请求
#define GUI_SMS_QUERY_RSP         -200010  //查询短信回应
#define GUI_SMS_CLEAR_REQ        200011    //清空短信请求
#define GUI_SMS_CLEAR_RSP         -200011   //清空短信回应
#define GUI_SMS_DEL_REQ           200012   //删除短信请求
#define GUI_SMS_DEL_RSP           -200012   //删除短信回应
#define GUI_SMS_SIGN_READ_REQ    200013   //标记已读信息请求
#define GUI_SMS_SIGN_READ_RSP    -200013  //标记已读信息回应

//查询事件
#define GUI_SMS_ALL     1
#define GUI_SMS_READ    2
#define GUI_SMS_UNREAD  3
typedef struct
{
	int m_iCondition;         //查询编号
}TGuiSmsQueryCondition;

typedef struct        //统计短信个数请求
{
	TGuiSmsQueryCondition m_tCondition;
}TGuiCountSmsReq;
typedef struct       //统计短信个数回应
{
   int m_iSmsCnt;   //统计的短信个数
}TGuiCountSmsRsp;

#define GUI_DIAL_NUMBER   31                  //发信人号码最大长度
#define GUI_SMS_CONTENT_LEN  159               //短信内容最大长度(以字符计算)
#define GUI_SMS_NAME   63
typedef struct
{
	int m_iRecTm;                                     //短信接收时间
	char m_strFrom[GUI_DIAL_NUMBER + 1];            //发信人
	char m_strContent[GUI_SMS_CONTENT_LEN + 1];     // 短信内容	
	char m_strName[GUI_SMS_NAME + 1];	//短信名
}TGuiSmsMsg;


typedef struct					//短信查询请求
{
	TGuiSmsQueryCondition m_tCondition; //查询条间
	int m_iPageId;          //起始短信页(第一页为0)
 	int m_iSmsCnt;               //每页短信条数
}TGuiSmsQueryReq;

typedef struct                 //查询短信回应
{
	int m_iSmsCnt;			//返回的条数
}TGuiSmsQueryRsp;

//短信删除事件
typedef struct									//删除短信请求
{
	char m_strName[GUI_SMS_NAME + 1];	//短信文件名	
} TGuiSmsDeleteReq;

#define GUI_SMS_DEL_OK			0		//删除短信成功
#define GUI_SMS_DEL_ERR			1		//删除短信失败
typedef struct									//删除短信回应
{
	int m_iRetCode;							//返回码
} TGuiSmsDeleteRsp;

//短信清空事件
#define GUI_SMS_CLEAR_OK		0				//清空短信成功
#define GUI_SMS_CLEAR_ERR		1				//清空短信失败
typedef struct									//清空短信回应
{
	int m_iRetCode;							//短信清空返回码
} TGuiSmsClearRsp;

//短信标记已读事件
typedef struct									//短信标记请求
{
	char m_strName[GUI_SMS_NAME + 1];	//短信文件名	
} TGuiSmsSignReadReq;

#define GUI_SMS_SIGN_READ_OK			0		//短信标记已读成功
#define GUI_SMS_SIGN_READ_ERR			1		//短信标记已读失败
typedef struct									//短信标记回应
{
	int m_iRetCode;							//返回码
} TGuiSmsSignReadRsp;

//-----------------------------------------------------------------------------------------
//3G管理
#define GUI_GET_3G_STATUS_CNF_REQ       200018 //获取3G管理配置请求
#define GUI_GET_3G_STATUS_CNF_RSP        -200018 //获取3G管理配置回应
//3G拨号方式
#define NET_3G_AUTO_DIAL  0
#define NET_3G_SMS_DIAL   1
#define NET_3G_PHONE_DIAL 2
#define NET_3G_ALARM_DIAL 3

//3G设备类型
#define NET_USB0_DEVICE		7  //移动网络
#define NET_3G_NONE_DEVICE   3 //无法识别
#define NET_3G_WCDMA_DEVICE  2 //联通3G
#define NET_3G_EVDO_DEVICE   1 //电信3G
#define NET_3G_TD_DEVICE     0 //移动3G
//3G在线状态
#define NET_3G_NOTONLINE    0  //不在线
#define NET_3G_ONLINE       1  // 在线 
#define NET_3G_LINING       2   //连接中...
//3G断线方式 
#define NET_3G_INITIATIVE   0
#define NET_3G_PASSIVE       1

//3G信号强度
#define NET_3G_SIGNAL_WEAKER   0
#define NET_3G_SIGNAL_WEAK     1
#define NET_3G_SIGNAL_GENERAL  2
#define NET_3G_SIGNAL_STRONG   3


#define NET_3G_DISCONNECT	    0				//3G未连接
#define NET_3G_CONNECTED    	1				//3G连接成功(已连接)
#define NET_3G_CONNECTING       2               //3G连接中。。。

typedef struct
{
	int m_iConnectState; 
	int m_iDeviceType;//设备类型 
	int m_iOnLineState;//在线状态
	int m_iIntensity;  //信号强度
	char m_strIP[GUI_IP_STR_LEN + 1]; //IP地址
	int m_iDialBeginTm;  //初始拨号时间
}TGui3GStatusMsg;

typedef struct          //获取3G设置信息回应
{
	TGui3GStatusMsg m_t3GStatusMsg;
}TGuiGet3GStatusCnfRsp;

//3G拨号
#define GUI_GET_3G_DIAL_STATUS_CNF_REQ   200019 //获取3G拨号状态请求
#define GUI_GET_3G_DIAL_STATUS_CNF_RSP   -200019 //获取3G拨号状态回应
#define GUI_SET_3G_DIAL_STATUS_CNF_REQ   200020  //  设置3G拨号状态请求
#define GUI_SET_3G_DIAL_STATUS_CNF_RSP   -200020  //  设置3G拨号状态回应

typedef struct
{
	int m_iDialType;   //拨号方式	//0,自动拨号；1，短信拨号；2，电话拨号；3，报警拨号(预留),4.定时  5. 手动
	int m_iStopType;   //断线方式
	int m_iDurationTm; //持续时间
}TGui3GDialStatusMsg;

typedef struct                            //获取3G拨号状态参数回应
{
	TGui3GDialStatusMsg m_t3GDialStatusMsg;
}TGuiGet3GDialStatusMsgRsp;

typedef struct                           //设置3g拨号状态参数请求
{
	TGui3GDialStatusMsg m_t3GDialStatusMsg;
}TGuiSet3GDialStatusMsgReq;

#define GUI_SET_3G_DIAL_STATUS_OK    0    //设置成功
#define GUI_SET_3G_DIAL_STATUS_ERR   1    //设置失败

typedef struct                          //设置3g拨号状态参数回应
{
	int m_iRetCode;
}TGuiSet3GDialStatusMsgRsp;

//3G管理-断开事件
#define GUI_SET_3G_DISCONN_CNF_REQ   200023 //设置3G网落断开请求
#define GUI_SET_3G_DISCONN_CNF_RSP   -200023 //设置3G网络断开回应

//设置3G断开网络的回应
typedef struct
{
	int m_iRetCode;
}TGuiSet3GDisconnectRsp;

//3G管理-连接事件
#define GUI_SET_3G_CONNECT_CNF_REQ   200024 //设置3G网落连接请求
#define GUI_SET_3G_CONNECT_CNF_RSP   -200024 //设置3G网络连接回应
//设置3G连接网络的回应
typedef struct
{
	int m_iRetCode;
}TGuiSet3GConnectRsp;

//3G管理-更多设置
#define GUI_GET_3G_DIAL_CNF_REQ      200025   //获取3G管理拨号配置请求
#define GUI_GET_3G_DIAL_CNF_RSP      -200025  //获取3G 管理拨号配置回应
#define GUI_SET_3G_DIAL_CNF_REQ      200026   //设置3G管理拨号配置请求
#define GUI_SET_3G_DIAL_CNF_RSP      -200026   //设置3G 管理拨号配置回应

#define GUI_DIAL_NOTIFY_LEN 19
typedef struct
{
	char m_strNotify[GUI_DIAL_NOTIFY_LEN + 1];
	char m_strPhone1[GUI_DIAL_NUMBER + 1];
	char m_strPhone2[GUI_DIAL_NUMBER + 1];
	char m_strPhone3[GUI_DIAL_NUMBER + 1];
	char m_strPhone4[GUI_DIAL_NUMBER + 1];
	char m_strPhone5[GUI_DIAL_NUMBER + 1];
}TGui3GDialSetParaMsg;

typedef struct                                //获取3G管理-更多设置界面参数回应
{
	TGui3GDialSetParaMsg m_t3GDialSetParaMsg;
}TGuiGet3GDialSetParaCnfRsp;

typedef struct                               //设置3G 管理-更多设置界面参数请求
{
	TGui3GDialSetParaMsg m_t3GDialSetParaMsg;
}TGuiSet3GDialSetParaCnfReq;

#define GUI_SET_3G_DIAL_SET_PARA_OK    0    //设置成功
#define GUI_SET_3G_DIAL_SET_PARA_ERR   1    //设置失败
typedef struct                              //设置3G管理-更多设置界面参数回应
{
	int m_iRetCode;
}TGuiSet3GDialSetParaCnfRsp;

//3G管理-高级设置
//3G管理-高级设置-3G任务时间表
#define GUI_GET_3G_ADV_SCH_CNF_REQ     200029   //获取3G高级管理中3G任务时间表请求
#define GUI_GET_3G_ADV_SCH_CNF_RSP    -200029 //获取3G高级管理中3G任务时间表回应
#define GUI_SET_3G_ADV_SCH_CNF_REQ     200030  //设置3G高级管理中3G任务时间表请求
#define GUI_SET_3G_ADV_SCH_CNF_RSP    -200030 //设置3G高级管理中3G 任务时间表回应

#define NET_3G_ENABLE_TASK_SCH      1          //启用任务时间表
#define NET_3G_UNENABLE_TASK_SCH    0          //禁用任务时间表
typedef struct
{
	int m_iTmSchEnable;
	int m_iStartTm;
	int m_iEndTm;
}TGui3GTmSchMsg;

typedef struct							//获取3G任务时间表回应
{
	TGui3GTmSchMsg m_t3GTmSchMsg;
}TGuiGet3GTmSchCnfRsp;

typedef struct                       //设置3G任务时间表请求
{
	TGui3GTmSchMsg m_t3GTmSchMsg;
}TGuiSet3GTmSchCnfReq;

#define GUI_SET_3G_TMSCH_OK    0  //设置成功
#define GUI_SET_3G_TMSCH_ERR   1  //设置失败
typedef struct                      //设置3G任务时间表回应
{
	int m_iRetCode;
}TGuiSet3GTmSchCnfRsp;

//3G管理-高级设置-通知
#define GUI_GET_3G_ADV_NOTIFY_CNF_REQ   200031 //获取3G高级管理中短信通知设置请求
#define GUI_GET_3G_ADV_NOTIFY_CNF_RSP    -200031 //获取3G高级管理中短信通知设置回应
#define GUI_SET_3G_ADV_NOTIFY_CNF_REQ   200032 //设置3G高级管理中短信通知设置请求
#define GUI_SET_3G_ADV_NOTIFY_CNF_RSP    -200032 //设置3G高级管理中短信通知设置回应

typedef struct
{
	int m_iNotifyType;
	char m_strNotifyMsg[GUI_SMS_CONTENT_LEN + 1];
}TGui3GNotifyMsg;

typedef struct                        //获取3G通知的参数信息回应
{
	TGui3GNotifyMsg m_t3GNotifyMsg;
}TGuiGet3GNotifyCnfRsp;


typedef struct                        //设置3G通知的参数信息请求
{
	TGui3GNotifyMsg m_t3GNotifyMsg;
}TGuiSet3GNotifyCnfReq;

#define GUI_SET_3G_NOTIFY_MSG_OK    0  //设置成功
#define GUI_SET_3G_NOTIFY_MSG_ERR   1  //设置失败
typedef struct							//设置3G通知的参数信息回应
{
	int m_iRetCode;
}TGuiSet3GNotifyCnfRsp;

//3G管理-高级设置-VPN
#define GUI_GET_3G_ADV_VPN_CNF_REQ  200035 //获取3G高级管理中VPN配置请求
#define GUI_GET_3G_ADV_VPN_CNF_RSP   -200035 //获取3G高级管理中VPN配置回应
#define GUI_SET_3G_ADV_VPN_CNF_REQ  200036 //设置3G高级管理中VPN配置请求
#define GUI_SET_3G_ADV_VPN_CNF_RSP   -200036 //设置3G高级管理中VPN配置回应

typedef struct
{
	char m_strDial[GUI_DIAL_NUMBER + 1];		//APN
	char m_strAccount[GUI_DIAL_NUMBER + 1];
	char m_strPassword[GUI_DIAL_NUMBER + 1];
}TGui3GVPNMsg;
typedef struct                     //获取3G VPN 参数信息配置回应
{
	TGui3GVPNMsg m_t3GVPNMsg;
}TGuiGet3GVPNCnfRsp;


typedef struct                     //设置3G VPN 参数信息配置请求
{
	TGui3GVPNMsg m_t3GVPNMsg;
}TGuiSet3GVPNCnfReq;

#define GUI_SET_3G_VPN_CNF_OK    0  //设置成功
#define GUI_SET_3G_VPN_CNF_ERR   1  //设置失败

typedef struct                    //设置3GVPN 参数信息配置回应
{
    int m_iRetCode;
}TGuiSet3GVPNCnfRsp;

//3G 管理-高级设置 - SIM 卡
#define GUI_GET_3G_ADV_SIM_CNF_REQ    200037 //获取3G高级管理中SIM卡号配置请求
#define GUI_GET_3G_ADV_SIM_CNF_RSP    -200037//获取3G高级管理中SIM卡号配置回应
#define GUI_SET_3G_ADV_SIM_CNF_REQ    200038 //设置3G高级管理中SIM卡号请求
#define GUI_SET_3G_ADV_SIM_CNF_RSP    -200038// 设置3G高级管理中SIM卡号回应

#define GUI_3G_ADV_SIM_LEN     31
typedef struct
{
	char m_strSim[GUI_3G_ADV_SIM_LEN + 1];
}TGui3GSimMsg;
typedef struct							//获取SIM请求回应
{
	TGui3GSimMsg m_t3GSimMsg;
}TGuiGet3GSimCnfRsp;

typedef struct              //设置SIM请求
{
	TGui3GSimMsg m_t3GSimMsg;
}TGuiSet3GSimCnfReq;  

typedef struct                      //设置SIM回应
{
	int m_iRetCode;
}TGuiSet3GSimCnfRsp;

//--------------------------------------------------------------
//GPS信息叠加预览
#define GUI_GET_GPS_MSG_REQ          200041//获取GPS地址信息请求
#define GUI_GET_GPS_MSG_RSP          -200041//获取GPS地址信息回应

#define GUI_GPS_MSG_LEN  31
#define GUI_GPS_DSP_OFF  0   //不显示
#define GUI_GPS_DSP_ON   1   //显示

typedef struct   //获取GPS地址信息回应
{
	int m_iDspEnable;
	char m_cGPSMsg[GUI_GPS_MSG_LEN + 1];
}TGuiGPSMsg;

typedef struct
{
	TGuiGPSMsg m_tGpsMsg;
}TGuiGetGPSMsgRsp;

#define GUI_SET_GPS_MSG_REQ          200042//设置GPS地址信息请求
#define GUI_SET_GPS_MSG_RSP          -200042//设置GPS地址信息回应
#define GUI_SET_GPS_CNF_OK			0
#define GUI_SET_GPS_CNF_ERR			1

typedef struct
{
	TGuiGPSMsg m_tGpsMsg;
}TGuiSetGPSMsgReq;

typedef struct
{
	int m_iRetCode;
}TGuiSetGPSMsgRsp;

//=============================
#define GUI_GET_GPS_MODULE_TYPE_REQ          200043//获取GPS模块类型请求
#define GUI_GET_GPS_MODULE_TYPE_RSP          -200043//获取GPS模块类型回应

#define MODULE_TYPE_GPS		0
#define MODULE_TYPE_BEIDOU	1

typedef struct
{
	int m_iGPSModuleType;		//GPS模块类型，0：GPS，1：北斗
}TGuiGetGPSModuleTypeRsp;

//=============================
#define GUI_GPS_TIMING_DISENABLE  0 //未使能
#define GUI_GPS_TIMING_ENABLE  	  1 //使能 

#define GUI_GET_GPS_TIMING_STA_REQ	200044 //获取GPS校时使能状态请求
#define GUI_GET_GPS_TIMING_STA_RSP	-200044 //获取GPS校时使能状态回应

typedef struct								
{	
	int m_iEnable; //见宏定义
}TGuiGetGPStimingStatRsp;

#define GUI_SET_GPS_TIMING_STA_REQ          200045//设置GPS校时使能状态请求
#define GUI_SET_GPS_TIMING_STA_RSP          -200045//设置GPS校时使能状态回应

typedef struct								
{	
	int m_iEnable; //见宏定义
}TGuiSetGPStimingStatReq;

typedef struct								
{	
	int m_iRetCode; //公共宏定义返回值
}TGuiSetGPStimingStatRsp;
//=============================

#define GUI_SET_BOARD_CONTROL_FAN_REQ			20004//设置主板控制风扇转动请求
#define GUI_SET_BOARD_CONTROL_FAN_RSP			-20004//设置主板控制风扇转动回应

enum
{
	GUI_CONTROL_FAN_START = 1,		//风扇转动
	GUI_CONTROL_FAN_STOP,			//风扇停止
};

typedef struct
{
	int m_iAct;
}TGuiSetBoardControlFanReq;

typedef struct
{
	int m_iRetCode;
}TGuiSetBoardControlFanRsp;


#define GUI_SET_INQUEST_VIDEO_ENCRYPT_REQ   20005//设置审讯视频加密请求
#define GUI_SET_INQUEST_VIDEO_ENCRYTP_RSP	-20005//设置审讯视频加密回应
typedef struct
{
	int m_iInquestVideoPwdEnable; //0-不使能 1-使能
	char m_cInquestVideoPwd[GUI_VIDEO_PWD_LEN + 1];
}TGuiSetInquestVideoEncryptReq;

typedef struct
{
	int m_iRetCode;
}TGuiSetInquestVideoEncryptRsp;

#define GUI_GET_INQUEST_VIDEO_ENCRYPT_REQ  	20006//获取审讯视频加密请求
#define GUI_GET_INQUEST_VIDEO_ENCRYPT_RSP   -20006//获取审讯视频加密回应

typedef struct
{
	int m_iInquestVideoPwdEnable;
	char m_cInquestVideoPwd[GUI_VIDEO_PWD_LEN + 1];
}TGuiGetInquestVideoEncryptRsp;

// 获取共享内存key值和size大小
#define GUI_GET_SHM_MSG_REQ	      2500
#define GUI_GET_SHM_MSG_RSP	      -2500

typedef struct	
{
	int m_iPid;	//程序标识
	int m_iType;    //共享内存类型
} TGuiGetShmMsgReq;

typedef struct	
{
	int m_iKey;	//共享内存key值(负值表式失败)
	int m_iSize;	//共享内存大小
} TGuiGetShmMsgRsp;



#define GUI_GET_SIP_DEVICE_INFO_REQ  1130 //获取SIP设备信息请求
#define GUI_GET_SIP_DEVICE_INFO_RSP  -1130 //获取SIP设备信息回应

//SIP设备参数
typedef struct
{
	char m_SipServerIp[GUI_PUBLIC_LEN_31 + 1];	//sip服务器ip
	int  m_SipServerPort;	//sip服务器端口号
	char m_SipServerID[GUI_PUBLIC_LEN_31 + 1];	//sip服务器ID
	char m_SipDeviceID[GUI_PUBLIC_LEN_31 + 1]; //设备编号
	char m_SipUserName[GUI_PUBLIC_LEN_31 + 1]; //用户名
	char m_SipPassword[GUI_PUBLIC_LEN_31 + 1];	//密码
	int  m_RegisterValidity;//注册有效期	
	int  m_keepalivetime;	//保活时间
	int  m_HeartBeatinterval;	//心跳间隔时间
	int  m_HeartBeatTimes;	//心跳次数
	int  m_EnableRegister;
}TGuiSIPDeviceInfoRsp;

#define GUI_GET_SIP_CHN_PARA_REQ  1131//获取SIP视频通道参数信息请求
#define GUI_GET_SIP_CHN_PARA_RSP   -1131 //获取SIP视频通道参数信息回应
//SIP视频通道参数
typedef struct
{
	int  m_iChn;
} TGuiGetSIPChnParaReq;

typedef struct
{
	int   	m_iChn;
	char	m_pcChnId[GUI_PUBLIC_LEN_31 + 1];	//通道设备编号
	int   	m_iLevel;		//当前ptz控制级别
	int   	m_iPTZtime;		//上次ptz控制计时
}TGuiGetSIPChnParaRsp;

#define GUI_GET_SIP_ALARM_PARA_REQ  1132 //获取SIP通道报警参数信息请求
#define GUI_GET_SIP_ALARM_PARA_RSP  -1132//获取SIP通道报警参数信息回应
//SIP报警通道参数
typedef struct									
{
	int   	m_iPortNo;
	char	m_pcAlarmInPortId[GUI_PUBLIC_LEN_31 + 1]; //通道设备编号
	int   	m_iLevel;			//通道报警级别
}TGuiGetSIPAlarmParaRsp;

typedef struct
{
	int     m_iPortNo;
}TGuiGetSIPAlarmParaReq;

#define GUI_SET_PU_COMMON_INFO_REQ  1138 
#define GUI_SET_PU_COMMON_INFO_RSP  -1138
#define GUI_SET_PU_COMMON_INFO_OK			0
#define GUI_SET_PU_COMMON_INFO_ERR			1

typedef struct
{
	char		m_cDeviceId[GUI_PUBLIC_LEN_63 + 1];
	char   	m_cDeviceName[GUI_PUBLIC_LEN_63 + 1];
	char  	m_cNFSIp[GUI_PUBLIC_LEN_15 + 1];
	char 	m_cNFSPath[GUI_PUBLIC_LEN_255 + 1];
	int		m_iDevNo;
	int 		m_blNfsEnable;
	int		m_iVspport;
	int		m_iVapport;
	char		m_cAccessnum[GUI_PUBLIC_LEN_63 + 1];
}TGuiSetCommonInfoReq;

typedef struct
{
	int m_iRetCode;
}TGuiSetCommonInfoRsp;

#define GUI_GET_RTMP_LIST_INFO_REQ  1135//获取RTMP信息请求
#define GUI_GET_RTMP_LIST_INFO_RSP  -1135 //获取RTMP信息回应

typedef struct
{
	int m_iUrlIDNo; //序号
	int m_iUrlChn; //推送设备那个通道的码流 第一通道为0
	int m_iUrlEnable; //使能改url,可以推送码流
	char m_cUrl[GUI_PUBLIC_LEN_127 + 1]; //推送码流到RTMP服务器的url地址
}TGuiRtmpInfoItem;

typedef struct
{
	int m_iRtmpCount;
}TGuiGetRtmpListInfoRsp;   //返回RTMP信息条数

#define GUI_GET_RTSP_LIST_INFO_REQ  1136//获取RTSP信息请求
#define GUI_GET_RTSP_LIST_INFO_RSP  -1136 //获取RTSP信息回应

typedef struct
{
	int m_iRtspNo; //序号
	char m_cRtspIp[GUI_PUBLIC_LEN_31 + 1]; //RTSP远程视频连接IP
}TRtspInfoItem;

typedef struct
{
	int m_iRtspCount;
}TGuiGetRtspListInfoRsp;   //返回RTSP信息条数



#define GUI_GET_CHN_FRAMERATE_LST_REQ   1140//获取通道帧率列表请求
#define GUI_GET_CHN_FRAMERATE_LST_RSP    -1140//获取通道帧率列表回应
typedef struct
{
	int m_iChn;
}TGuiGetChnFrameRateLstReq;
typedef struct
{
	int m_iChn;
	int m_iFrameRateCnt;
}TGuiGetChnFrameRateLstRsp;

typedef struct
{
	int m_iFrameRate;
}TGuiGetChnFrameRateItem;

#define  GUI_SET_PATH_NAME_REQ	1141//设置路径请求
#define  GUI_SET_PATH_NAME_RSP  -1141//设置路径回应
typedef struct
{
	char   m_cPathName[GUI_PUBLIC_LEN_127 + 1];		//目录名称
}TGuiSetPathNameReq;//设置路径信息请求

typedef  struct
{
	int m_iRetCode;
}TGuiSetPathNameRsp;

#define GUI_SET_VI_MOD_REQ	1142 //设置VI模式请求 SDI 输入示证输入
#define GUI_SET_VI_MOD_RSP	-1142 //设置VI模式回应 SDI 输入示证输入 
typedef struct
{
	int m_iChn;
	int m_iViMod;		// 1:本地输入，2:示证输入
}TGuiSetViModReq;


typedef struct
{
	int m_iChn;
	int m_iRetCode;
}TGuiSetViModRsp;

#define GUI_GET_VI_MOD_REQ	1143 //获取VI模式请求 SDI 输入示证输入
#define GUI_GET_VI_MOD_RSP	-1143 //获取VI模式回应 SDI 输入示证输入 

typedef struct
{
	int m_iChn;
}TGuiGetViModReq;


typedef struct
{
	int m_iChn;
	int m_iViMod;		// 1:本地输入，2:示证输入
}TGuiGetViModRsp;


/************************************************************
20141107  NVR换代 易用性升级项目
*************************************************************/

//	是否刻录播放器软件
#define GUI_GET_RECORD_PLAYER_REQ	316
#define GUI_GET_RECORD_PLAYER_RSP	-316
#define GUI_SET_RECORD_PLAYER_REQ	317
#define GUI_SET_RECORD_PLAYER_RSP	-317

typedef struct
{
	int m_iReocrdPlayer;   //0 不刻录 1刻录播放器
}TGuiRecPlayerCnf;

typedef struct
{
	TGuiRecPlayerCnf m_tCnf;
}TGuiGetRecordPlayerRsp;


typedef struct
{
	TGuiRecPlayerCnf m_tCnf;
}TGuiSetRecordPlayerReq;

typedef struct
{
	int m_iRetCode;         //0成功，1失败
}TGuiSetRecordPlayerRsp;

//	设置前端IPC的字号
#define GUI_GET_IPCFONT_SIZE_REQ		1200
#define GUI_GET_IPCFONT_SIZE_RSP		-1200
#define GUI_SET_IPCFONT_SIZE_REQ		1201
#define GUI_SET_IPCFONT_SIZE_RSP		-1201

typedef struct			//获取字体大小参数请求
{
	int m_iChn;			//通道号
} TGuiGetIpcFontSizeReq;

typedef struct			//获取字体大小参数回应
{
	int m_iChn;
	int m_iFontSize;	
} TGuiGetIpcFontSizeRsp;


typedef struct			//设置字体大小参数请求
{
	int m_iChn;
	int m_iFontSize;	
} TGuiSetIpcFontSizeReq;

typedef struct			//设置字体大小参数回应
{
	int m_iRetCode; 		//0成功，1失败
} TGuiSetIpcFontSizeRsp;

#define GUI_CMD_IPC_ONKEY_FOCUS_REQ		5020  //一键聚焦请求
#define GUI_CMD_IPC_ONKEY_FOCUS_RSP		-5020 //一键聚焦回应

typedef struct
{
	int m_iChn;
}TGuiSetIpcOneKeyFocusReq;

typedef struct
{
	int m_iChn;
	int m_iRetCode;   //0表成功 其余表失败
}TGuiSetIpcOneKeyFocusRsp;

//	附加字符叠加支持5块区域,第一个用老协议，后面四个用下面的协议
#define GUI_GET_OSD_TEXT_EX_REQ		1202	//扩展附加字符叠加
#define GUI_GET_OSD_TEXT_EX_RSP		-1202
#define GUI_SET_OSD_TEXT_EX_REQ		1203    
#define GUI_SET_OSD_TEXT_EX_RSP		-1203

typedef struct
{
	int m_iChn;
	int m_iIndex;  //1表示第一块区域，依次类推
} TGuiGetOsdTextExReq;

typedef struct
{
	int m_iChn;
	int m_iIndex;  //1表示第一块区域，依次类推
	TGuiOsdTextCnf m_tCnf;
} TGuiGetOsdTextExRsp;

typedef struct
{
	int m_iChn;
	int m_iIndex;
	TGuiOsdTextCnf m_tCnf;
} TGuiSetOsdTextExReq;

typedef struct
{
	int m_iRetCode;			//0成功，1失败
} TGuiSetOsdTextExRsp;

//ROI区域设置
#define GUI_GET_ENCODE_ROI_REQ		1034
#define GUI_GET_ENCODE_ROI_RSP		-1034
#define GUI_SET_ENCODE_ROI_REQ		1035
#define GUI_SET_ENCODE_ROI_RSP		-1035

typedef struct
{
	int m_iEnable;	  //ROI区域使能标识  1表示使能
	int m_iLocType;	//坐标类型
	int m_iAbsQp;	//0相对值，1绝对值	
	int m_iQp;		//范围，可能是帧率之类的，先保留
	TGuiArea m_tROIAreas;
} TGuiROICnf;

typedef struct
{
	int m_iChn;
	int m_iIndex;     //1表示第一块区域，依次类推
} TGuiGetROIAreaReq;

typedef struct
{
	int m_iChn;
	int m_iIndex;   
	TGuiROICnf m_tCnf;
} TGuiGetROIAreaRsp;

typedef struct
{
	int m_iChn;
	int m_iIndex;   
	TGuiROICnf m_tCnf;
} TGuiSetROIAreaReq;

typedef struct
{
	int m_iRetCode;     //0表成功，1表示失败
} TGuiSetROIAreaRsp;


//	回放相关
#define GUI_VIDEO_PLAYER_FULL_SCREEN_REQ   	5000 //最大化
#define GUI_VIDEO_PLAYER_FULL_SCREEN_RSP   	-5000 //最大化

typedef struct
{
	int m_iVoid;
	TGuiVoAttr m_iVoLoc;
}TGuiVideoPlayerFullScreenReq;

typedef struct
{
	int m_iRetCode;
}TGuiVideoPlayerFullScreenRsp;


#define GUI_VIDEO_PLAYER_RESIZEVO_REQ   		5001//全屏（去窗体）和返回
#define GUI_VIDEO_PLAYER_RESIZEVO_RSP  		-5001 //全屏（去窗体）和返回

typedef struct
{
	int m_iVoCnt;
	int m_iType;           //0、录像 1、图片
	TGuiVoAttr m_iVoLoc[GUI_SYNC_PLAYER_MAX_CNT];
}TGuiVideoPlayerResizeVoReq;

typedef struct
{
	int m_iRetCode;
}TGuiVideoPlayerResizeVoRsp;


#define GUI_PLAYER_THUMBNAIL_ACTIVE_REQ    	5002     //进度条预览
#define GUI_PLAYER_THUMBNAIL_ACTIVE_RSP		-5002	
typedef struct    
{
	int m_iVodev;
	int m_iVolevel;       //0表示普通，1表示pip
	TGuiVoAttr m_tVoAttrs;
}TGuiPlayerthumbnailActiveReq;

typedef struct    
{
	int m_iPlayId;
}TGuiPlayerthumbnailActiveRsp;


#define GUI_PLAYER_THUMBNAIL_PLAY_REQ		5003
#define GUI_PLAYER_THUMBNAIL_PLAY_RSP		-5003

typedef struct   
{
	int m_iPlayid;
	int m_iChn;
	int m_iBeginTm;
	int m_iEndTm;
	int m_iStepFlag;
}TGuiPlayerthumbnailPlayReq;

typedef struct
{
	int m_iRetCode;
}TGuiPlayerthumbnailPlayRsp;


#define GUI_PLAYER_THUMBNAIL_EXIT_REQ		5004
#define GUI_PLAYER_THUMBNAIL_EXIT_RSP		-5004

typedef struct
{
	int m_iPlayId;
}TGuiPlayerThunbnailExitReq;
typedef struct
{
	int m_iRetCode;
}TGuiPlayerThunbnailExitRsp;

//jpeg初始化
#define GUI_JPEG_VIEWER_INIT_REQ_EX 			5005
#define GUI_JPEG_VIEWER_INIT_RSP_EX			-5005
typedef struct
{
	int m_iDev;
	TGuiVoAttr m_tVoAttrs;
}TGuiJpegViewerInitReqEx;

#define GUI_SYNC_STEP_REVERSE	 				5006   // 同步步退

//多通道搜索
#define GUI_SET_QUERY_CHN_LIST_REQ		5007
#define GUI_SET_QUERY_CHN_LIST_RSP		-5007
typedef struct
{
	int m_iChnOffSet;
	int m_iChnLlist;
}TGuiSetQueryChnListReq;

typedef struct
{
	int m_iRetCode; 
}TGuiSetQueryChnLiIstRsp;

//多通道搜索(新)    ，先设置要查询的通道号，然后再进行查询
#define GUI_SET_QUERY_CHN_LIST_REQ_EX		5107
#define GUI_SET_QUERY_CHN_LIST_RSP_EX		-5107
typedef struct
{
	int m_iChnOffSet;
	int m_iChnLst[32];   //按位，支持32*32=1024个通道
}TGuiSetQueryChnLstReqEx;

typedef struct
{
	int m_iRetCode; 
}TGuiSetQueryChnLstRspEx;


#define GUI_SET_FILE_CONVERT_FORMAT_REQ   5008    //设置备份文件导出时的文件格式请求
#define GUI_SET_FILE_CONVERT_FORMAT_RSP   -5008   //设置备份文件导出时的文件格式回应

enum
{
	GUI_FILE_SDV = 1,
	GUI_FILE_AVI,
	GUI_FILE_MP4,
};

enum
{
	GUI_DEFAULT = 0,
	GUI_CBL_TYPE,
	GUI_BACKUP_TYPE,
};

#define GUI_GHOST_NUM		0

typedef struct
{
	int m_iDevId;         //保留  ，目前传GUI_GHOST_NUM
	int m_iType;  //0-默认 1-光盘实时刻录 2-光盘备份刻录
	int m_iConvertFormat;
}TGuiSetFileConvertFormatReq;


typedef struct
{
	int m_iRetCode;
}TGuiSetFileConvertFormatRsp;


#define GUI_GET_FILE_CONVERT_FORMAT_REQ    5009    //获取备份文件导出时的文件格式请求
#define GUI_GET_FILE_CONVERT_FORMAT_RSP    -5009   //获取备份文件导出时的文件格式回应

typedef struct
{
	int m_iDevId; //保留 ,目前传GUI_GHOST_NUM
	int m_iType;//0-默认 1-光盘实时刻录 2-光盘备份刻录
}TGuiGetFileConvertFormatReq;

typedef struct
{
	int m_iDevId; //保留 ,目前传GUI_GHOST_NUM
	int m_iType; //0-默认 1-光盘实时刻录 2-光盘备份刻录
	int m_iConvertFormat;
}TGuiGetFileConvertFormatRsp;

#define GUI_SET_CBL_SINGLE_FILE_FORMAT_REQ   20020    //设置刻录或备份单一文件格式请求
#define GUI_SET_CBL_SINGLE_FILE_FORMAT_RSP   -20020   //设置刻录或备份单一文件格式回应

enum
{
	GUI_DISABLE_SINGLE_FILE = 0,
	GUI_SINGLE_FILE_SDV,
	GUI_SINGLE_FILE_AVI,
	GUI_SINGLE_FILE_MP4,
};

typedef struct
{
	int m_iDevId;         //保留  ，目前传GUI_GHOST_NUM
	int m_iType;  //0-默认 1-光盘实时刻录 2-光盘备份刻录
	int m_iSingleFileFormat;
}TGuiSetSingleFileFormatReq;


typedef struct
{
	int m_iRetCode;
}TGuiSetSingleFileFormatRsp;


#define GUI_GET_CBL_SINGLE_FILE_FORMAT_REQ    20021    //获取刻录或备份单一文件格式请求
#define GUI_GET_CBL_SINGLE_FILE_FORMAT_RSP    -20021   //获取刻录或备份单一文件格式回应

typedef struct
{
	int m_iDevId; //保留 ,目前传GUI_GHOST_NUM
	int m_iType;//0-默认 1-光盘实时刻录 2-光盘备份刻录
}TGuiGetSingleFileFormatReq;

typedef struct
{
	int m_iDevId; //保留 ,目前传GUI_GHOST_NUM
	int m_iType; //0-默认 1-光盘实时刻录 2-光盘备份刻录
	int m_iSingleFileFormat;
}TGuiGetSingleFileFormatRsp;

#define GUI_SET_FILE_AUDIO_FORMAT_REQ   5010    //设置备份/刻录音频文件格式请求
#define GUI_SET_FILE_AUDIO_FORMAT_RSP   -5010   //设置备份/刻录音频文件格式回应

enum
{
	GUI_FILE_NONE = 0,	//不备份/刻录音频文件
	GUI_FILE_AAC,	//备份/刻录AAC文件
};


typedef struct
{
	int m_iDevId;         //保留  ，目前传GUI_GHOST_NUM
	int m_iType;  //0-默认 1-光盘实时刻录 2-光盘备份刻录
	int m_iAudioFormat;
}TGuiSetCblAudioFormatReq;


typedef struct
{
	int m_iRetCode;
}TGuiSetCblAudioFormatRsp;


#define GUI_GET_FILE_AUDIO_FORMAT_REQ    5011    //获取备份/刻录音频文件格式请求
#define GUI_GET_FILE_AUDIO_FORMAT_RSP    -5011   //获取备份/刻录音频文件格式回应

typedef struct
{
	int m_iDevId; //保留 ,目前传GUI_GHOST_NUM
	int m_iType;//0-默认 1-光盘实时刻录 2-光盘备份刻录
}TGuiGetCblAudioFormatReq;

typedef struct
{
	int m_iDevId; //保留 ,目前传GUI_GHOST_NUM
	int m_iType; //0-默认 1-光盘实时刻录 2-光盘备份刻录
	int m_iAudioFormat;
}TGuiGetCblAudioFormatRsp;


#define GUI_SET_VCBINDVODEV_REQ  1150   //设置VC和那个Vo设备同源(VGA/HDMI1，HDMI2)请求
#define GUI_SET_VCBINDVODEV_RSP  -1150  //设置VC和那个Vo设备同源(VGA/HDMI1，HDMI2)回应
typedef struct
{
	int m_iVcId;  //VC通道ID
	int m_iVoDevId; // 0-保留 0x1-VGA/HDMI1  0x2-HDMI2  0x3-VGA/HDMI1/HDMI2
}TGuiSetVcBindVoDevReq;

typedef struct
{
	int m_iRetCode;
}TGuiSetVcBindVoDevRsp;

#define GUI_GET_VCBINDVODEV_REQ  1151 //获取VC和那个Vo设备同源(VGA/HDMI1，HDMI2)请求
#define GUI_GET_VCBINDVODEV_RSP  -1151  //获取VC和那个Vo设备同源(VGA/HDMI1，HDMI2)回应
typedef struct
{
	int m_iVcId;	//VC通道ID
}TGuiGetVcBindVoDevReq;

typedef struct
{
	int m_iVcId;   //VC通道ID
	int m_iVoDevId;  // 0-保留 0x1-VGA/HDMI1  0x2-HDMI2  0x3-VGA/HDMI1/HDMI2
}TGuiGetVcBindVoDevRsp;


#define GUI_SET_BESTIR_RESPONSE_REQ 1152 //设置激励响应请求(dvr->app)
#define GUI_SET_BESTIR_RESPONSE_RSP -1152 //设置激励响应回应(app->dvr)

enum
{
	GUI_BESTIR_SAVE = 0,
	GUI_BESTIR_START,
	GUI_BESTIR_END,
};

typedef struct
{
	int m_iChn; //响应激励的视频通道号
	int m_iStat; //激励状态 0-保留 1-激励  2-取消激励
}TGuiSetBestirResponseReq;

typedef struct
{
	int m_iRetCode;
}TGuiSetBestirResponseRsp;

#define GUI_SET_BESTIR_TYPE_REQ 1153 //设置激励方式请求
#define GUI_SET_BESTIR_TYPE_RSP -1153 //设置激励方式回应

enum
{
	GUI_BESTIR_EXCHANGE = 0, //交换激励
	GUI_BESTIR_SPECIFIC,  //特写激励
};

typedef struct
{
	int m_iType;
}TGuiSetBestirTypeReq;
typedef struct
{
	int m_iRetCode;
}TGuiSetBestirTypeRsp;


#define GUI_SET_MIC_TYPE_REQ  1154  //设置MIC类型回应-庭审增加-预留
#define GUI_SET_MIC_TYPE_RSP  -1154 //设置MIC类型回应-庭审增加
enum
{
	GUI_MIC_HAND_TO_HAND = 0, //手拉手MIC
	GUI_MIC_POT_TO_POT, //点对点MIC
};

typedef struct
{
	int  m_iVideoChn;  //MIC对应的视频通道 -预留
	int  m_iAudioChn;  //MIC对应的音频通道-预留
	int  m_iMicType;  //MIC类型
}TGuiSetMicTypeReq;

typedef struct
{
	int m_iRetCode;
}TGuiSetMicTypeRsp;

#define GUI_SET_MIC_AUDIO_AMPLITUDE_REQ  1155 //设置音频振幅值请求
#define GUI_SET_MIC_AUDIO_AMPLITUDE_RSP  -1155 //设置音频振幅回应请求
typedef struct
{
	int m_iChn; //通道号
	int m_iMicId; //MIc号
	int m_iAmplitude; // >= 0
}TGuiSetMicAudioAmplitudeReq;

typedef struct
{
	int m_iRetCode;
}TGuiSetMicAudioAmplitudeRsp;


#define GUI_GET_URLINFO_REQ		1500
#define GUI_GET_URLINFO_RSP		-1500
#define GUI_URL_LEN		128
#define GUI_ID_LEN	32

enum
{
	GUI_ANDROID_ENUM = 1,
	GUI_IOS_ENUM,
	GUI_ID,
};

typedef struct
{
	int m_iType;		//见上面的枚举类型
}TGuiGetURLInfoReq;

typedef struct
{
	int m_iType;
	char m_strUrl[0];
}TGuiGetURLInfoRsp;

//固定式政法主机升级增加  

#define GUI_SET_CBL_RESSPACE_REQ	791  //设置实时刻录的光盘保留空间请求
#define GUI_SET_CBL_RESSPACE_RSP	-791 //设置实时刻录的光盘保留空间回应
typedef struct
{
	int m_iDvdSpace;  //单位M
}TGuiSetCblResSpaceReq;

typedef struct
{
	int m_iRetCode;
}TGuiSetCblResSpaceRsp;

#define GUI_GET_CBL_RESSPACE_REQ	792	//获取实时刻录的光盘保留空间请求
#define GUI_GET_CBL_RESSPACE_RSP	-792	//获取实时刻录的光盘保留空间回应 
typedef struct
{
	int m_iDvdSpace; //单位M
}TGuiGetCblResSpaceRsp;



///////////////////////////////////////////////////////////////////////323参数获取，对接模块通过内核获取
//获取配置文件的通用结构体
#define GUI_GET_CONFIG_VALUE_REQ	793	//获取配置参数请求
#define GUI_GET_CONFIG_VALUE_RSP	-793	//获取配置参数回应

typedef struct
{
	char m_cKeyName[GUI_PUBLIC_LEN_31 + 1];//关键字名称
}TGuiKeyValueInfo;

typedef struct
{
	char m_cSectionName[GUI_PUBLIC_LEN_31 + 1];//Section名字
	int m_iValueCount;//需要获取字段个数
	TGuiKeyValueInfo m_pcKeyValueInfo[0];//Key的结构体指针
}TGuiGetConfigValueReq;

typedef struct
{
	char m_cKeyName[GUI_PUBLIC_LEN_31 + 1];//关键字名称
	char m_cValue[GUI_PUBLIC_LEN_31 + 1];//取值
}TGuiGetConfigValueRsp;

#define GUI_SET_CBL_FILE_TYPE_REQ   794 //设置刻录文件类型请求
#define GUI_SET_CBL_FILE_TYPE_RSP   -794 //设置刻录文件类型请求回应

typedef struct
{
	int m_iFileType;  // GUI_AUDIO_ONLY -20  GUI_VIDEO_AUDIO_NEW-3
}TGuiSetCblFileTypeReq;

typedef struct
{
	int m_iRetCode;
}TGuiSetCblFileTypeRsp;

#define GUI_GET_CBL_FILE_TYPE_REQ 795	//获取刻录文件类型请求
#define GUI_GET_CBL_FILE_TYPE_RSP -795  //获取刻录文件类型回应

typedef struct
{
	int m_iFileType;
}TGuiGetCblFileTypeRsp;



//////////////////////////////////////////////////////////////
//=====================呼叫======================
//1、呼叫设备信息
enum
{
	GUI_CALL_MODEL_TYPE_H323 = 0,
	GUI_CALL_MODEL_TYPE_SIP,
};//呼叫类型

enum
{
	GUI_CALL_ONLY_AUDIO = 0,
	GUI_CALL_VIDEO_AUDIO,
};//呼叫模式

typedef struct
{
	int m_iCallType;//呼叫类型
	int m_iCallModel;//呼叫模式,是323库里面的呼叫类型
	int m_iCallRate;//呼叫速率
	int m_iNetPort;//服务端口
	char m_cIP[GUI_PUBLIC_LEN_31 + 1];//IP地址
	char m_cNetNum[GUI_PUBLIC_LEN_31 + 1];//设备号码
	char m_cURI[GUI_PUBLIC_LEN_63 + 1];//URI字符
}TGuiH323CallDevInfo;

#define GUI_GET_H323_CALL_DEVINFO_REQ  20009	//获取H323呼叫设备信息请求
#define GUI_GET_H323_CALL_DEVINFO_RSP  -20009	//获取H323呼叫设备信息回应

typedef struct
{
	TGuiH323CallDevInfo m_tInfo;	
}TGuiGetH323CallDevInfoRsp;

#define GUI_SET_H323_CALL_DEVINFO_REQ	20010 //设置H323呼叫设备信息请求
#define GUI_SET_H323_CALL_DEVINFO_RSP	-20010 //设置H323呼叫设备信息回应

enum
{
	GUI_H323_SET_TYPE_HUNGUP = 0,	//挂断
	GUI_H323_SET_TYPE_CALL,		//视频呼叫
};//设置类型

typedef struct
{
	int m_iSetType;	//设置类型
	TGuiH323CallDevInfo m_tInfo;
}TGuiSetH323CallDevInfoReq;


typedef struct
{
	int m_iRetCode;  //0-成功，1-失败 用公共宏定义	
}TGuiSetH323CallDevInfoRsp;

#define GUI_GET_H323_SUB_STREAM_STA_REQ	20015 //获取附流连接状态请求
#define GUI_GET_H323_SUB_STREAM_STA_RSP	-20015 //获取附流连接状态回应

typedef struct
{
	char m_cIP[GUI_PUBLIC_LEN_31 + 1];//IP地址
}TGuiGetH323SubStreamLinkStaReq;

enum
{
	GUI_SUB_STREAM_LINK_FREE_STA = 0,	//空闲
	GUI_SUB_STREAM_LINK_SEND_STA,		//发送中
	GUI_SUB_STREAM_LINK_RECEIVE_STA,	//接收中
};


typedef struct
{
	int m_iLinkSta;  //0:空闲，1:发送中，2:接收中
}TGuiGetH323SubStreamLinkStaRsp;


#define GUI_SET_H323_SUB_STREAM_STA_REQ	20016 //推/断开附流请求
#define GUI_SET_H323_SUB_STREAM_STA_RSP	-20016 //推/断开附流回应

enum
{
	GUI_SUB_STREAM_DIS_CONNECT = 0,	//断开连接
	GUI_SUB_STREAM_CONNECT,			//连接
};

typedef struct
{
	int iState;	//0:断开附流， 1:连接附流
	char m_cIP[GUI_PUBLIC_LEN_31 + 1];//IP地址
}TGuiSetH323SubStreamStaReq;

typedef struct
{
	int m_iRetCode;  //0-成功，1-失败 用公共宏定义	
}TGuiSetH323SubStreamStaRsp;


//==============被叫 手动模式===========================

#define GUI_GET_H323_CALLMSG_REQ   20011//获取拨号信息请求
#define GUI_GET_H323_CALLMSG_RSP   -20011//获取拨号信息回应


typedef struct
{
	char m_strCallMsg[GUI_PUBLIC_LEN_63 + 1];	
}TGuiGetH323CallMsgRsp;


#define GUI_SET_H323_RESPONSE_CMD_REQ   20012//响应命令请求
#define GUI_SET_H323_RESPONSE_CMD_RSP   -20012//响应命令回应

enum
{
	GUI_CMD_TYPE_HANGUP = 0,
	GUI_CMD_TYPE_ANSWER,
};

typedef struct
{
	char m_strCallMsg[GUI_PUBLIC_LEN_63 + 1];
	int m_iCmdType;		
}TGuiSetH323ResponseCmdReq;

typedef struct
{
	int m_iRetCode;	
}TGuiSetH323ResponseCmdRsp;

//2、网守参数
enum
{
	GUI_GK_TYPE_DISABLE = 0,//禁用
	GUI_GK_TYPE_FIXIP,			//指定
	GUI_GK_TYPE_AUTOSEARCH,	//自动搜索
};//gk模式

enum
{
	GUI_GK_ENC_DISABLE = 0,//禁用
	GUI_GK_ENC_AUTO,//自动
	GUI_GK_ENC_ZTE,//中兴
	GUI_GK_ENC_NEWTV,//新视通
	GUI_GK_ENC_CISCO,//思科
};//gk加密方式

#define GUI_GET_H323_GK_PARA_REQ   20013//获取GK参数请求
#define GUI_GET_H323_GK_PARA_RSP   -20013//获取GK参数回应
typedef struct
{
	int m_iGKGroupId;
	int m_iGkType;//gk模式
	char m_cGkIP[GUI_PUBLIC_LEN_63 + 1];//gk地址
	int m_iGkPort;//gk端口
	char m_cGkRegName[GUI_PUBLIC_LEN_63 + 1];//gk注册名称
	int m_iEncType;//gk加密方式
}TGuiGetH323GKParaRsp;


//3、本机参数
enum
{
	GUI_ENCODE_UTF8 = 0,
	GUI_ENCODE_UNICODE,
};//ID编码方式
enum
{
	GUI_ANW_TYPE_AUTO = 0,//自动
	GUI_ANW_TYPE_MANUAL,//手动
	GUI_ANW_TYPE_UNANW,//免打扰
};//响应方式

#define GUI_GET_H323_LOCAL_PARA_REQ 20014//获取H323本地参数请求
#define GUI_GET_H323_LOCAL_PRAR_RSP -20014//获取H323本地参数回应
typedef struct
{
	int m_iGroupId;
	int m_iLisenPort;//本机监听端口
	char m_cLocalNo[GUI_PUBLIC_LEN_63 + 1];//终端号码
	char m_cPassWord[GUI_PUBLIC_LEN_63 + 1];//加密密码
	int m_iIdCodeType;//ID编码方式
	int m_iAnwTypeForCall;//点对点呼叫响应方式
	int m_iEncMainVideoChn;//视频源主码流通道
	int m_iEncSubVideoChn;//视频源副码流通道
	int m_iDecMainVideoChn;//解码显示主码流通道
	int m_iDecSubVideoChn;//解码显示副码流通道
}TGuiGetH323LocalParaRsp;

#define GUI_SET_AUDIO_CHANGE_TAG_REQ  20017 //设置通道音频改变打点请求
#define GUI_SET_AUDIO_CHANGE_TAG_RSP  -20017 //设置通道音频改变打点回应
typedef struct
{
	int m_iChn; //通道号
	int m_iSoundStat; //声音状态 0-无声 1-有声
}TGuiSetAudioChangeTagReq;

typedef struct
{
	int m_iRetCode;
}TGuiSetAudioChangeTagRsp;

#define GUI_SET_COMMON_TAG_REQ  20018 //设置通用打点请求
#define GUI_SET_COMMON_TAG_RSP  -20018 //设置通用打点

typedef struct
{
	int m_iType;//按位表示，0代表未标记，1代表标记,2-光盘序列号,4-音频标记，(前三位与已有协议兼容)，8-生命体征,
	int m_iChn; //通道号,默认0表所有通道,
	int m_iLen; //实际标签的长度，最长64字节
	char m_cTag[0];
}TGuiSetCommonTagReq;

typedef struct
{
	int m_iRetCode;
}TGuiSetCommonTagRsp;

#define GUI_GET_VITAL_SIGN_CONFIG_REQ	20022	//获取生命体征配置请求
#define GUI_GET_VITAL_SIGN_CONFIG_RSP	-20022	//获取生命体征配置回应

typedef struct
{
	int m_iDeviceId;			//设备ID，暂时传0
}TGuiGetVitalSignConfigReq;

typedef struct
{
	int m_iDeviceId;							//设备ID，暂时传0
	int m_iPort;									//端口
	char m_cIP[GUI_PUBLIC_LEN_63 + 1];		//IP地址 用专用宏，如果没有加一个
}TGuiGetVitalSignConfigRsp;

#define GUI_SET_VITAL_SIGN_CONFIG_REQ	20023	//设置生命体征配置请求
#define GUI_SET_VITAL_SIGN_CONFIG_RSP		-20023	//设置生命体征配置回应

typedef struct
{
	int m_iDeviceId;							//设备ID，暂时传0
	int m_iPort;									//端口
	char m_cIP[GUI_PUBLIC_LEN_63 + 1];		//IP地址 用专用宏，如果没有加一个
}TGuiSetVitalSignConfigReq;

typedef struct
{
	int m_iRetCode;
}TGuiSetVitalSignConfigRsp;

enum
{
	GUI_VITAL_SIGN_TYPE_HEARTRATE = 1,			//心率
	GUI_VITAL_SIGN_TYPE_BLOODOXYGEN,		//血氧
	GUI_VITAL_SIGN_TYPE_BLOODPRESSURE,		//血压
	GUI_VITAL_SIGN_TYPE_RESP,     				//呼吸
	GUI_VITAL_SIGN_TYPE_MAX
};//体征类型


#define GUI_SET_VITAL_SIGN_DATA_REQ		20024	//设置生命体征数据请求
#define GUI_SET_VITAL_SIGN_DATA_RSP		-20024	//设置生命体征数据回应

#define MAX_VITAL_SIGN_DATA_LEN		125			//生命体征波形点集最大125个点

typedef struct //生命体征采集数据封装
{
	char m_cVersion;//协议版本
	char m_cType;	//生命体征采集类型,1：心率 2：血氧 3：血压  4 呼吸
	char m_cMonFlg; //1:正常2:超上限 3：超下限4:脱落 5:设备离线    
	char m_iOscilloGramSeqID; //从1开始，心电最多三波依次传递1，2，3，血氧一波，使用1
	int m_iMonVal[4]; //体征实时值, 血压3个,SYS DIA PR依次存放，精度：原值乘以100;
	int m_iGramRate;//1s有多少个点
	int m_iSizeType; // 1:大 2：中 3：小
	int m_iXPosition; // 相对于视频区左上角(0,0)的相对横坐标
	int m_iYPosition; // 相对于视频区左上角(0,0)的相对纵坐标
	int m_iPointLen;
	unsigned char m_cPointValue[0]; //点集
}TUserDataVitalSignData;	//和TUserDataVitalSign保持一致

typedef struct 
{
	int m_iType;							//bit0:是否上报IE；bit1:是否保存UserData
	TUserDataVitalSignData m_tVitalSignData;//生命体征数据，与UserDta保持一致
} TGuiSetVitalSignDataReq;

typedef struct
{
	int m_iRetCode;
}TGuiSetVitalSignDataRsp;

//#####################M7升级2015###########begin####################//

//获取设备是否已经连接上p2p服务器
#define GUI_GET_DEV_LINK_INTERNET_REQ		1510
#define GUI_GET_DEV_LINK_INTERNET_RSP		-1510
typedef struct
{
	int m_iLinked;       //0表示连接 其余表示未连接
}TGuiGetDevLinkedRsp;      


//外挂和内核交互
#define GUI_NOTIFY_DEV_LINK_INTERNET_REQ		1511
#define GUI_NOTIFY_DEV_LINK_INTERNET_RSP		-1511
typedef struct
{
	int m_iLinked;
}TGuiNotifyDevLinkedReq;  //notify 外挂通知内核是否连上p2p服务器

typedef struct
{
	int m_iRet;
}TGuiNotifyDevLinkedRsp;

//帧率、码率、分辨率及时值获取
#define	GUI_GET_CHN_ENCODE_CONFIG_REQ		5203
#define	GUI_GET_CHN_ENCODE_CONFIG_RSP		-5203
#define GUI_MAGIC_PICSIZE		-20150917

typedef struct
{
	int m_iChn;
}TGuiGetChnEncodeCfgReq;

typedef struct
{
	int m_iChn;
	int m_iFrmRate;        	//帧率
	int m_iBitRate;			//码率
	int m_iPicSize;				//分辨率 ,如果为GUI_MAGIC_PICSIZE，则表示此通道无视频
	int m_iRemain[4];        //预留4个字节
} TGuiGetChnEncodeCfgRsp;

//彩转黑--获取设备通道相关实时信息
#define GUI_GET_DEVICE_IM_STATE_REQ		5205
#define GUI_GET_DEVICE_IM_STATE_RSP		-5205
typedef enum
{
	GUI_BRIGHTNESS = 1,   //亮度
}EGuiDeviceStateEnum;

typedef struct
{
	int m_iChn;
	int m_iType;
}TGuiGetDeviceImStateReq;

typedef struct
{
	int m_iValue;   //实时值
} TGuiGetDeviceImStateRsp;

//获取支持的彩转黑类型
#define GUI_GET_COLOR2GRAYLST_REQ    5206
#define GUI_GET_COLOR2GRAYLST_RSP	 -5206	
typedef struct
{
	int m_iChn;
}TGuiGetColor2GrayLstReq;

typedef struct
{
	int m_iChn;
	int m_iEnable;  //按位：1: 支持， 0: 不支持bit0，（自动）内同步；bit1，黑白；
	//bit2，彩色；bit3，（自动）外同步; bit4, 报警同步 ;bit5，定时，  6--自动（彩转黑内同步、黑转彩外同步）
}TGuiGetColor2GrayLstRsp;

//彩转黑参数设置获取
#define GUI_GET_COLOR2GRAY_CNF_REQ		5210
#define GUI_GET_COLOR2GRAY_CNF_RSP		-5210
typedef struct
{
	int m_iChn;
}TGuiGetColor2GrayCnfReq;

typedef struct
{
	int m_iChn;
	int m_iColor2Gray; // 0--自动（内同步）1--黑白2--彩色3--自动（外同步）4--报警同步5--定时6--自动（彩转黑内同步、黑转彩外同步）
	int m_iDayRange;  //白天亮度值(0~255)，当超过此数值，切换为白天
	int m_iNightRange;  // 当相对亮度低于此数字，切换为夜晚(0-255)（白天亮度高于晚上）
	int m_iTimeRange;   // bit24-bit31:天亮小时bit16-bit23:天亮分钟bit8-bit15:天黑小时bit0-bit7:天黑分钟
	int m_iColorDelay;	 //彩转黑延时
	int m_iGrayDelay;  //黑转彩延时
} TGuiGetColor2GrayCnfRsp;

#define GUI_SET_COLOR2GRAY_CNF_REQ		5211
#define GUI_SET_COLOR2GRAY_CNF_RSP		-5211

typedef  TGuiGetColor2GrayCnfRsp  TGuiSetColor2GrayCnfReq;

typedef struct
{
	int m_iRetCode;   //0表成功，其余表失败
} TGuiSetColor2GrayCnfRsp;

#define GUI_GET_DAYNIGHT_CNF_REQ		5212
#define GUI_GET_DAYNIGHT_CNF_RSP		-5212
typedef struct
{
	int m_iChn;
}TGuiGetDayNightCnfReq;

typedef struct
{
	int m_iChn;
	int m_iDayNightType;      //模式 0-自动，1-夜晚，2-白天
	int m_iSensitivity;           //灵敏度0--低，1--中，2--高 (灵敏度只在自动时有用)
}TGuiGetDayNightCnfRsp;

#define GUI_SET_DAYNIGHT_CNF_REQ		5213
#define GUI_SET_DAYNIGHT_CNF_RSP		-5213

typedef TGuiGetDayNightCnfRsp TGuiSetDayNightCnfReq;

typedef struct
{
	int m_iRetCode;   //0表成功，其余表失败	
}TGuiSetDayNightCnfRsp;

//========客流量统计===
#define GUI_GET_PEOPLE_CNT_REQ		5214	//客流量统计结果请求
#define GUI_GET_PEOPLE_CNT_RSP		-5214	//客流量统计结果回应

typedef struct 
{
	int m_iChn;
	int m_iGranularity;  // 统计粒度（1--小时，2--天，3--周，4--月，5--年）
	int m_iBeginTm;     //开始时间
	int m_iEndTm;       //结束时间
}TGuiGetPeopleCntReq;

typedef struct
{
	int m_iTotalCnt;     //后面跟n条TGuiPeopleCntCnf信息
}TGuiGetPeopleCntRsp;   

typedef struct
{
	int m_iChn;
	int m_iBeginTm;
	int m_iInCnt;
	int m_iOutCnt;
}TGuiPeopleCntCnf;


//#####################M7升级2015###########end#####################//


//#define GUI_TEST   //某些功能的重启使能，release版本应该注释掉
#endif
