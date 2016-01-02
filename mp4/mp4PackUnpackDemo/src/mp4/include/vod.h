/*******************************************************************
文件名	:vod.h
公司	:天津天地伟业数码科技有限公司
创建者	:陶永亮
创建日期:2009-11-05
简要描述:与存储相关的变量、宏的定义，存储模块接口函数原型
修改记录:2009-11-05,陶永亮，创建
        
******************************************************************/
#ifndef _VOD_H_
#define _VOD_H_

#include "public_def.h"
#include "libstorage.h"
#include "net_tcp_common.h"

#define MAX_VOD_FILE_NUM		1024	//按时间段VOD时的文件列表容量
#define MAX_VOD_CONNECT			5		//最大VOD连接数
#define IS_INVALID_VOD(client)	(client == NULL || (client->m_hRecState == 1 || client->m_hSendState == 1))
#define IS_SEGMENT_VOD(vod)		(vod.m_stSeg.m_iStartTime + vod.m_stSeg.m_iStopTime\
								+ vod.m_stSeg.m_iFileNum != 0)
#define SEGMENT_PREFIX			"TM"
#define SEGMENT_SPLIT			":"
#define SEGMENT_INTERVAL		86400 * 7	//按时间段VOD的最大跨度，不允许超过两天
#define SEGMENT_OFFSET			7200		//为了确保时间段所在的首个文件被检索到，需要提前查找。

#define VOD_FILETAIL_TIMEOUT	60000		//VOD播放文件结束超时后会关闭文件。单位:毫秒
#define VOD_PROGRESS_INTERVAL	2000		//按时间段VOD进度发送间隔

//陶永亮 091106 按时间段VOD的信息
typedef struct
{
	time_t		m_iStartTime;							//起始时间
	time_t		m_iStopTime;							//结束时间
	td_s32		m_iFileNum;								//文件列表中的文件数目
	td_s32		m_iHead;								//标识整个队列中首个合法文件的位置
	td_s32		m_iPos;									//当前正在播放的文件位置
	TFileInfo*	m_pstFileList;							//文件列表
	td_s32		m_iCurFrameRate;						//当前正在播放的文件的帧率
	td_s32		m_iCurFirstFrameNo;						//当前正在播放的文件的首帧帧号
}TVodBySegment;

//VOD状态枚举
typedef enum
{
	VOD_STATE_FREE,
	VOD_STATE_PREPARING,
	VOD_STATE_SENDING,
	VOD_STATE_PAUSE,//暂停
	VOD_STATE_END,//标识文件尾.在文件尾时如果超时后会关闭连接
	VOD_STATE_STOP,
	VOD_STATE_BUTT,
}TVodState;


//VOD结构
typedef struct
{
	PCONNECT	m_stClient;								//记录请求视频的连接
	td_s32		m_iInterval;							//发送间隔
	td_u64		m_ulSendTime;							//发送时间， 单位毫秒
	FILE*		m_fp;									//要发送的文件
	td_s32		m_iPosition;							//定位,-1,表示正常播放，0~100表示用百分比定位
	td_s32 		m_iSpeed;								//播放速度
	td_s32		m_iIFrame;								//是否只播I帧
	td_s32		m_iFileSize;							//文件总大小
	td_u8		m_ucFileType;							//文件类型
//	pthread_mutex_t	m_stMutex;							//互斥锁 陶永亮 090804 添加
	TVodState     m_iState;						    	//vod状态标志
	TVodBySegment m_stSeg;					 		    //按时间段VOD的信息
	td_s32        m_iDownMode;                          //下载模式，此值为0
	td_s32        m_iBreakStat;                         //断点续传状态，此值为2
	td_s32        m_iSendFileSize;
}TVod;

 
td_s32 rec_AddLink(void* _stClient, td_u8* _pucFilename, td_s32 _iReqMode, td_s32 _iBreak);
/*
 * 函数名称：rec_AddLink
 * 简要描述：添加一个录像播放客户端.假如该客户端当前已在按时间段回放，则为向这个按时间段追加一个时间段
 * 输	  入：
 * 			_stClient,PCONNECT型，用于标识一个连接，非空
 * 			_pucFilename,所请求的文件名,非空
 *                 _iReqMode, 下载模式，0----帧模式，1----流模式
 *                 _iBreak, 断点续传2 ----断点续传
 * 输	 出：
 * 			ERR_REC_TOO_MANY_USERS,已达最大用户数
 * 			ERR_REC_OPEN_FILE_FAILED,打开文件失败
 * 			ERR_REC_READ_FILE_ERROR,读文件产生错误
 *			ERR_REC_WRONG_PARAM,错误的参数
 * 			ERR_REC_FAILED,其他原因导致失败
 *			ERR_REC_SUCCESSFUL,成功
 * 修改日志：
 * 			2008-10-22,陶永亮,创建
 */
td_s32 rec_CloseLink(void* _stClient);
/*
 * 函数名称：rec_CloseLink
 * 简要描述：停止一个录像文件播放连接
 * 输	  入：
 * 			_stClient,PCONNECT型，用于表示一个客户端连接
 * 输	 出：
 *			ERR_REC_WRONG_PARAM,错误的参数
 * 			ERR_REC_FAILED,其他原因导致失败
 *			ERR_REC_SUCCESSFUL,成功
 * 修改日志：
 * 			2008-10-22,陶永亮,创建
 */
 td_u32 rec_ErrorHandler(td_u32 _iSocket);
/*
 * 函数名称：rec_ErrorHandler
 * 简要描述：错误处理
 * 输	  入：
 * 			无
 * 输	 出：
 * 			ERR_REC_SUCCESSFUL
 *			
 * 修改日志：
 * 			2010-08-14,程显永,创建
 */
td_s32 rec_VodCtrl(void* _stClient, td_s32 _iPosition, td_s32 _iSpeed, td_s32 _iIFrame);
/*
 * 函数名称：rec_VodCtrl
 * 简要描述：控制录像文件播放的速度、进度等
 * 输	  入：
 * 			_stClient,PCONNECT型，用于表示一个客户端连接
 *			_iPosition,用于定位文件，0~100
 *			_iSpeed,用于控制文件的播放速度,0~32,0表示暂停，1表示正常速度播放
 *			_iIFrame,只播放I帧
 * 输	 出：
 *			ERR_REC_WRONG_PARAM,错误的参数
 * 			ERR_REC_FAILED,未找到该连接
 *			ERR_REC_SUCCESSFUL,成功
 * 修改日志：
 * 			2008-10-22,陶永亮,创建
 */

td_s32 rec_IsVodNull(td_s32 _iId);
/*
 * 函数名称：IsVodNull
 * 简要描述：判断某个VOD节点是否为空
 * 输	  入：
 * 			_iId,VOD节点编号，即数组下标，0~MAX_VOD_CONNECT
 * 输	 出：
 *			ERR_REC_WRONG_PARAM,错误的参数
 * 			ERR_REC_FAILED,其他原因导致失败
 *			ERR_REC_SUCCESSFUL,成功
 * 修改日志：
 * 			2009-11-20,陶永亮,创建
 */

td_s32 rec_VodInit(void);
/*
 * 函数名称：VodInit
 * 简要描述：初始化VOD子模块
 * 输	  入：
 * 			无
 * 输	 出：
 * 			ERR_REC_FAILED,其他原因导致失败
 *			ERR_REC_SUCCESSFUL,成功
 * 修改日志：
 * 			2010-01-29,陶永亮,创建
 */
 
td_s32 rec_CloseLinkByID(td_s32 _iID);
/*
 * 函数名称:CloseLinkByID
 * 简要描述:根据ID关闭VOD连接
 * 输	 入:
 *          连接ID
 * 输	 出:
 *			ERR_REC_SUCCESSFUL, 获取成功
 * 修改日志:
 *			2009-03-02,陶永亮，创建。
 */
td_s32 rec_GetVodNetCost();

#endif /*_VOD_H_*/

