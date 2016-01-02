/*******************************************************************
文件名     : HDsnap.c
公司          : Tianjin Tiandy Tech.
创建者     : 杨珏
创建日期: 2011-12-12
简要描述: 硬盘录像
修改记录: 2011-12-12 yangjue Create this File
******************************************************************/
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <time.h>
#include <pthread.h>
#include "playback.h"
#include "libdatabase.h"
#include "td_errno.h"
#include "public_def.h"
#include "config.h"
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/


#define MAX_PATHLEN      1024
#define DAYNUM_MONTH     31
#define MAXNUM_MONTH     12
#define MAXNUM_YEAR      9999
#define MAXNUM_SNAPBUF   1024
#define DAYNUM_BUF       32
#define MAX_HDNUM        8			// 硬盘数上限
#define MAX_PARTITIONNUM 4			// 最大分区数
#define MAX_EPTFILENUM   64			// 无效文件的最大数量
#define LEN_FOLDERNAME   16
#define MONTHMAP_READ    (2 * 60 * 60)

#define LEN_SDVNAME      27
#define PER_QUERY_NUM	(1024)
#define CHANNEL_OFFSET   4
#define CHANNEL_MAX      32
#define CHANNEL_CHKALL   255

#define SNAP_DB_REC_IDX   5
#define SNAP_DB_START_IDX 8
#define SNAP_DB_STOP_IDX  9
#define SNAP_BUF_STARTNODE_IDX 0
#define SNAP_BUF_STOPNODE_IDX  1

#define SNAP_NODE_ATTR_START 1
#define SNAP_NODE_ATTR_STOP  0

#define SNAP_DEFAULT_BLURTIME 0

#define SNAP_MAX_SPACETIME   (2 * 60 * 60)

// 日快照录像类型划分
#define DAYMAP_TYPE_NUM       3
#define DAYMAP_RECTYPE_ERROR  0xff	// 录像类型出错
#define DAYMAP_RECTYPE_MANUAL 0		// 手动录像
#define DAYMAP_RECTYPE_ALARM  1		// 报警录像
#define DAYMAP_RECTYPE_TIMING 2		// 定时录像
// 从数据库读出的录像类型
#define DAYMAP_IDX_RECTYPE_MANUAL 3		// 手动录像
#define DAYMAP_IDX_RECTYPE_TIMING 5		// 定时录像
#define DAYMAP_IDX_RECTYPE_PORT   7		// 端口报警
#define DAYMAP_IDX_RECTYPE_MD     11	// 移动侦测报警
#define DAYMAP_IDX_RECTYPE_VL     13	// 视频丢失报警
#define DAYMAP_IDX_RECTYPE_OD     19	// 遮挡报警

#define DAYMAP_IDX_RECTYPE_ATM	149	//卡号录像

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/


typedef struct
{
	time_t m_Time;			// 时间值
	int    m_iAttr;			// 属性，开始结束
	int    m_iPreNodeIdx;	// 上一个结点的索引
	int    m_iNextNodeIdx;	// 下一个结点的索引
} TSnapBuf;




/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#if 0
const char cst_SnapExtName[] = ".sdv";
#endif



static TSnapBuf g_stSnapBuf[DAYMAP_TYPE_NUM][MAXNUM_SNAPBUF];
static int iReadMonthSnap = 0;

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/



/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/


/*+++++++++++++++++++++++++++++月快照查询++++++++++++++++++++++++++++++++*/

/******************************************************************************
	函数名称: rec_MonthMapChkExtName
	简要描述: 查找扩展名是否符合标准
	输入          : _pcName------文件名
	                      _pcExtName----扩展名
	输出          : 0为该文件扩展名符合标准
	修改日志: 2011-12-12 杨珏创建该函数
******************************************************************************/
static int rec_MonthMapChkExtName(char* _pcName, const char* _pcExtName)
{
	int iExtLen = 0;
	int iNameLen = 0;
	
	// 参数检测
	if ((NULL == _pcName) || (NULL == _pcExtName))
	{
		return SNAP_ERR_ARG;
	}

	iExtLen = strlen(_pcExtName);					// 读出扩展名长度
	iNameLen = strlen(_pcName);						// 读出文件名长度
	// 扩展名为空说明均符合标准
	if (0 == iExtLen)
	{
		return 0;
	}
	// 文件名长度小于或等于扩展名长度，则返回故障
	if (iNameLen <= iExtLen)
	{
		return -2;
	}

	if (0 == strcmp(_pcName+(iNameLen-iExtLen), _pcExtName))
	{
		return 0;
	}

	return -3;

}

/******************************************************************************
	函数名称: rec_MonthMapGetChannelNo
	简要描述: 通道号转换
	输入          : _iChannelNo----通道号
	输出          : 转换后的结果
	修改日志: 2011-12-13 杨珏创建该函数
******************************************************************************/
static char rec_MonthMapGetChannelNo(int _iChannelNo)
{
	// 小于0和超过上限则认为参数错误
	if ((_iChannelNo < 0) || (_iChannelNo >= CHANNEL_MAX))
	{
		return SNAP_ERR_ARG;
	}
	// 按照ASCII码，0-9则转换成ASCII码
	else if (_iChannelNo <= 9)
	{
		return ('0' + _iChannelNo);
	}
	// 超过9的从A开始递增
	else
	{
		return ('A' + (_iChannelNo - 10));
	}
}

/******************************************************************************
	函数名称: rec_MonthMapChkChanel
	简要描述: 检测通道号是否正确
	输入          : _pcName------文件名
	                      _iChannelNo----通道号
	输出          : 0表示正确
	修改日志: 2011-12-13 杨珏创建该函数
******************************************************************************/
static int rec_MonthMapChkChanel(char* _pcName, int _iChannelNo)
{
	char cChannelNo = 0;

	// 参数检测
	if (NULL == _pcName)
	{
		return SNAP_ERR_ARG;
	}

	// 如果检测所有的通道，则不检测通道号
	if (CHANNEL_CHKALL == _iChannelNo)
	{
		return 0;
	}

	cChannelNo = rec_MonthMapGetChannelNo(_iChannelNo);			// 读出通道号
	//printf("channel number is %c\n", cChannelNo);

	// 如果通道号正确，则返回正确标志0
	if (*(_pcName + CHANNEL_OFFSET) == cChannelNo)
	{
		return 0;
	}

	return -2;
	
}

/******************************************************************************
	函数名称: rec_MonthMapChkSize
	简要描述: 获取一个月的硬盘快照信息
	输入          : _pcPath------查看快照的路径
	                      _pcName------文件名
	输出          : 返回文件的大小
	修改日志: 2011-12-12 杨珏创建该函数
******************************************************************************/
static off_t rec_MonthMapChkSize(char* _pcPath, char* _pcName)
{
	struct stat stBuf;
	char cPathTemp[MAX_PATHLEN];
	
	// 参数检测
	if ((NULL == _pcPath) || (NULL == _pcName))
	{
		return SNAP_ERR_ARG;
	}

	// 组合文件路径
	strcpy(cPathTemp, _pcPath);
	strcat(cPathTemp, "/");
	strcat(cPathTemp, _pcName);

	stat(cPathTemp, &stBuf);				// 读取文件的信息

	return stBuf.st_size;
	
}

/******************************************************************************
	函数名称: rec_CheckSnapFolder
	简要描述: 获取一个月的硬盘快照信息
	输入          : _pcPath------查看快照的路径
	输出          : 0为该文件夹不空
	修改日志: 2011-12-12 杨珏创建该函数
******************************************************************************/
static int rec_MonthMapChkFolder(char* _pcPath, int _iChannelNo)
{
	DIR* pstDir = (DIR*)NULL;
	struct dirent* pstr = NULL;
	struct dirent tmp;

	// 参数检测
	if (NULL == _pcPath)
	{
		return SNAP_ERR_ARG;
	}
	
	pstDir = opendir(_pcPath);				//打开指定路径
	// 打开文件夹失败
	if (NULL == pstDir)
	{
		return SNAP_ERR_ARG;
	}

	// 读取所有文件
	//while (NULL != (pstr = readdir(stDir)))
	memset(&tmp,0,sizeof(tmp));
	while((readdir_r(pstDir, &tmp, &pstr) == 0) && pstr)
	{
		// 检查名称长度
		if (LEN_SDVNAME != strlen(pstr->d_name))
		{
			continue;
		}
		#if 0
		// 检查扩展名是否符合标准
		if (0 != rec_MonthMapChkExtName(pstr->d_name, cst_SnapExtName))
		{
			continue;
		}
		#else
		
		// 检查扩展名是否符合标准
		if (0 != rec_MonthMapChkExtName(pstr->d_name, Cfg_gRecExtName()))
		{
			continue;
		}
		
		#endif
		// 检测通道是否正确
		if (0 != rec_MonthMapChkChanel(pstr->d_name, _iChannelNo))
		{
			continue;
		}
		// 检查指定文件的大小，不为0即为该文件存在
		if (rec_MonthMapChkSize(_pcPath, pstr->d_name) > 0)
		{
			closedir(pstDir);
			return 0;
		}
	}

	closedir(pstDir);
	
	return -2;

}

/******************************************************************************
	函数名称: rec_MonthMapInit
	简要描述: 月快照信息初始化
	输入          : _pcInfo-------读出信息
	输出          : 读出的信息数量
	修改日志: 2011-12-26 杨珏创建该函数
******************************************************************************/
static int rec_MonthMapInit(int* _pcInfo)
{
	int i = 0;

	if (NULL == _pcInfo)
	{
		return SNAP_ERR_ARG;
	}

	for (i=0; i<DAYNUM_BUF; i++)
	{
		*(_pcInfo + i) = SNAP_EMPTY;
	}

	return 0;
}

/******************************************************************************
	函数名称: rec_GetMonthMap
	简要描述: 获取月的硬盘快照信息
	输入          : _pcPath------查看快照的路径
	                      _pcFolder-----文件夹名称
	                      _iChannelNo---通道号
	                      _pcInfo-------读出信息
	输出          : 0为读取成功，其他为读取失败
	修改日志: 2011-12-26 杨珏创建该函数
******************************************************************************/
static int rec_MonthMapRead(char *_pcPath, char *_pcFolder, int _iChannelNo)
{
	int iTemp = 0;
	int i = 0;
	int j = 0;
	char cPath[MAX_PATHLEN];

	// 进行参数监测
	if ((NULL == _pcPath) || (NULL == _pcFolder))
	{
		return SNAP_ERR_ARG;
	}

	for (i = 0; i < MAX_SATA_NUM; i++)
	{
		for (j = 0; j < MAX_PART_NUM; j++)
		{
			sprintf(cPath, "%s/%d/%d/%s", _pcPath, i, j, _pcFolder);
			iTemp = rec_MonthMapChkFolder(cPath, _iChannelNo);			// 查询文件夹下是否有该通道内容
			if (0 == iTemp)
			{
				return 0;
			}
		}
	}

	return 1;
	
}

/******************************************************************************
	函数名称: rec_GetMonthMap
	简要描述: 获取月的硬盘快照信息
	输入          : _pcPath------查看快照的路径
	                      _iYear--------所查看的年份
	                      _iMonth-------所查看的月份
	                      _iChannelNo---通道号
	                      _pcInfo-------读出信息
	输出          : 0为读取成功
	修改日志: 2011-12-26 杨珏创建该函数
******************************************************************************/
int rec_GetMonthMap(char* _pcPath, int _iYear, int _iMonth, int _iChannelNo, int* _piInfo)
{
	static int iFirst = 0;
	char cFolderName[LEN_FOLDERNAME];
	int iTemp = 0;
	int i = 0;

	if ((NULL == _pcPath) || (NULL == _piInfo))
	{
		return SNAP_ERR_ARG;
	}
	
	rec_MonthMapInit(_piInfo);				// 初始化月照信息

	// 若第一次还没有加载完毕，UI获取时直接返回均无效
	if (iFirst && !iReadMonthSnap)
	{
		return 0;
	}
	iFirst = 1;
	
	for (i=1; i<DAYNUM_BUF; i++)
	{
		// 生成文件夹名字
		sprintf(cFolderName, "%.4d%.2d%.2d", _iYear, _iMonth, i);
		iTemp = rec_MonthMapRead(_pcPath, cFolderName, _iChannelNo);
		if (0 == iTemp)
		{
			*(_piInfo + i) = SNAP_HAVE;
		}
	}

	return 0;

}

// 杨珏20120530 初始化加载线程，每隔一小时加载一次，每次读取最近12个月的月照
static void* rec_MonthMapThread(void* _param)
{
	int tmp[CHANNEL_MAX];
	time_t now;
	struct tm tm_now;
	int i = 0;
	int iMonth = 0;
	int iYear = 0;
	int iTime = 0;
	func_info();

	while (1)
	{
		iTime = get_uptime(NULL);
		now = time(&now);
		localtime_r(&now, &tm_now); 

		// 查询近十二个月的月照信息
		iMonth = tm_now.tm_mon+1;
		iYear = tm_now.tm_year+1900;
		for (i = 0; i < MAXNUM_MONTH; i++)
		{
			rec_GetMonthMap(SNAP_DESK_Path, iYear, iMonth, CHANNEL_CHKALL, tmp);
			iReadMonthSnap = 1;
			iMonth--;
			if (iMonth <= 0)
			{
				iMonth = MAXNUM_MONTH;
				iYear--;
			}
		}
		// 每小时读取一次月照信息
		while (get_uptime(NULL) - iTime < MONTHMAP_READ)
		{
			sleep(10);
		}
	}
	
	return NULL;}

int rec_GetMonthMapInit()
{
	pthread_t tThread;
	int iRet = 0;
	iRet = pthread_create(&tThread, NULL, rec_MonthMapThread, NULL);
	if (0 == iRet)
	{
		pthread_detach(tThread);
		return 0;
	}
	return -1;
}


/*+++++++++++++++++++++++++++++日快照查询++++++++++++++++++++++++++++++++*/


/******************************************************************************
	函数名称: rec_DayInitChkBuf
	简要描述: 初始化缓冲区
	输入          : 无
	输出          : 无
	修改日志: 2011-12-13 杨珏创建该函数
******************************************************************************/
int rec_DayInitChkBuf(void)
{
	int i = 0;
	int j = 0;

	for (j=0; j<DAYMAP_TYPE_NUM; j++)
	{
		for (i=0; i<MAXNUM_SNAPBUF; i++)
		{
			g_stSnapBuf[j][i].m_iPreNodeIdx = -1;
			g_stSnapBuf[j][i].m_iNextNodeIdx= -1;
		}
		// 起始节点与结束结点进行初始化
		g_stSnapBuf[j][SNAP_BUF_STARTNODE_IDX].m_iNextNodeIdx = SNAP_BUF_STOPNODE_IDX;
		g_stSnapBuf[j][SNAP_BUF_STOPNODE_IDX].m_iPreNodeIdx = SNAP_BUF_STARTNODE_IDX;
	}

	return 0;
}

/******************************************************************************
	函数名称: rec_DayGetNode
	简要描述: 找到一个空闲的结点
	输入          : 无
	输出          : 结点号
	修改日志: 2011-12-13 杨珏创建该函数
******************************************************************************/
int rec_DayGetNode(int _iType)
{
	int i = 0;

	// 参数监测
	if (_iType >= DAYMAP_TYPE_NUM)
	{
		return -1;
	}

	for (i=0; i<MAXNUM_SNAPBUF; i++)
	{
		// 索引为首尾结点则继续
		if ((SNAP_BUF_STARTNODE_IDX == i) || (SNAP_BUF_STOPNODE_IDX == i))
		{
			continue;
		}
		// 查询该结点的首尾指针是否不在范围内
		if ((g_stSnapBuf[_iType][i].m_iPreNodeIdx < 0) || (g_stSnapBuf[_iType][i].m_iNextNodeIdx < 0))
		{
			return i;
		}
	}
	return -2;
}

/******************************************************************************
	函数名称: rec_DayInsertNodeBuf
	简要描述: 在缓冲区插入一个结点
	输入          : _iTime--------需插入的时间值
	                      _iAttr---------该时间的描述内容，开始还是结束
	输出          : 插入结点号，负值表示插入失败
	修改日志: 2011-12-13 杨珏创建该函数
******************************************************************************/
int rec_DayInsertNodeBuf(time_t _iTime, int _iAttr, int _iType)
{
	int iNode = 0;
	int i = 0;
	int iReadNode = 0;
	int iPreNode = 0;
	int iNextNode = 0;

	// 参数检测
	if ((SNAP_NODE_ATTR_START != _iAttr) && (SNAP_NODE_ATTR_STOP != _iAttr))
	{
		return -1;
	}
	if (_iType >= DAYMAP_TYPE_NUM)
	{
		return -1;
	}

	iNode = rec_DayGetNode(_iType);			// 寻找一个空闲的结点
	if (iNode < 0)
	{
		return -2;
	}
	// 保存该结点信息
	g_stSnapBuf[_iType][iNode].m_Time = _iTime;
	g_stSnapBuf[_iType][iNode].m_iAttr= _iAttr;
	
	iReadNode = SNAP_BUF_STOPNODE_IDX;				// 初始化读入的结点号
	for (i=0; i<MAXNUM_SNAPBUF; i++)
	{
		iReadNode = g_stSnapBuf[_iType][iReadNode].m_iPreNodeIdx;		// 读出上一个结点
		// 读出的结点如果不是开始结点，继续检测
		if (SNAP_BUF_STARTNODE_IDX != iReadNode)
		{
			// 如果插入的时间数比当前读出的时间数小，则继续检测
			if (_iTime < g_stSnapBuf[_iType][iReadNode].m_Time)
			{
				continue;
			}
		}
		// 在当前数据之后插入该结点
		iPreNode = iReadNode;
		iNextNode = g_stSnapBuf[_iType][iReadNode].m_iNextNodeIdx;
		g_stSnapBuf[_iType][iNode].m_iPreNodeIdx = iPreNode;
		g_stSnapBuf[_iType][iNode].m_iNextNodeIdx = iNextNode;
		g_stSnapBuf[_iType][iPreNode].m_iNextNodeIdx = iNode;
		g_stSnapBuf[_iType][iNextNode].m_iPreNodeIdx = iNode;
		
		return iNode;
	}

	return -3;
	
}

/******************************************************************************
	函数名称: rec_DayDelNode
	简要描述: 删除一个结点
	输入          : _iNode------结点号
	输出          : 0表示删除成功
	修改日志: 2011-12-13 杨珏创建该函数
******************************************************************************/
int rec_DayDelNode(int _iNode, int _iType)
{
	int iPreNode = 0;
	int iNextNode = 0;
	
	// 参数监测
	if ((SNAP_BUF_STARTNODE_IDX == _iNode) || (SNAP_BUF_STOPNODE_IDX == _iNode))
	{
		return -1;
	}
	if (_iNode < 0)
	{
		return -1;
	}
	if (_iType >= DAYMAP_TYPE_NUM)
	{
		return -1;
	}

	iPreNode = g_stSnapBuf[_iType][_iNode].m_iPreNodeIdx;
	iNextNode = g_stSnapBuf[_iType][_iNode].m_iNextNodeIdx;
	// 判断结点计算是否正确
	if ((iPreNode < 0) || (iNextNode < 0))
	{
		return -2;
	}

	// 更改索引值
	g_stSnapBuf[_iType][iPreNode].m_iNextNodeIdx = iNextNode;
	g_stSnapBuf[_iType][iNextNode].m_iPreNodeIdx = iPreNode;
	g_stSnapBuf[_iType][_iNode].m_iNextNodeIdx = -1;
	g_stSnapBuf[_iType][_iNode].m_iPreNodeIdx = -1;

	return 0;
}

/******************************************************************************
	函数名称: rec_DayOptNodeBuf
	简要描述: 对该结点内容进行优化
	输入          : _iNode--------结点索引
	                      _iBlurTime-----模糊时间
	输出          : 0表示优化成功
	修改日志: 2011-12-13 杨珏创建该函数
******************************************************************************/
int rec_DayOptNodeBuf(int _iNode, int _iBlurTime, int _iType)
{
	int iChkNode = 0;
	
	// 参数监测
	if (_iNode < 0)
	{
		return -1;
	}
	if (_iType >= DAYMAP_TYPE_NUM)
	{
		return -1;
	}

	// 如果本结点为开始结点，若与其相邻的有开启结点，删除靠后的
	if (SNAP_NODE_ATTR_START == g_stSnapBuf[_iType][_iNode].m_iAttr)
	{
		// 检测前一个结点
		iChkNode = g_stSnapBuf[_iType][_iNode].m_iPreNodeIdx;
		if (SNAP_BUF_STARTNODE_IDX != iChkNode)
		{
			if (g_stSnapBuf[_iType][_iNode].m_iAttr == g_stSnapBuf[_iType][iChkNode].m_iAttr)
			{
				rec_DayDelNode(_iNode, _iType);
				return 0;
			}
		}
		// 检测后一个结点
		iChkNode = g_stSnapBuf[_iType][_iNode].m_iNextNodeIdx;
		if (SNAP_BUF_STOPNODE_IDX != iChkNode)
		{
			if (g_stSnapBuf[_iType][_iNode].m_iAttr == g_stSnapBuf[_iType][iChkNode].m_iAttr)
			{
				rec_DayDelNode(iChkNode, _iType);
			}
		}
		// 如果前一个结点是结束结点，两者相差时间为模糊时间以内，均删除
		iChkNode = g_stSnapBuf[_iType][_iNode].m_iPreNodeIdx;
		if (SNAP_BUF_STARTNODE_IDX != iChkNode)
		{
			if (g_stSnapBuf[_iType][_iNode].m_Time - g_stSnapBuf[_iType][iChkNode].m_Time <= _iBlurTime)
			{
				rec_DayDelNode(_iNode, _iType);
				rec_DayDelNode(iChkNode, _iType);
			}
		}
	}
	// 如果本结点为结束结点，若与其相邻的有结束结点，删除靠前的
	else if (SNAP_NODE_ATTR_STOP == g_stSnapBuf[_iType][_iNode].m_iAttr)
	{
		// 检测后一个结点
		iChkNode = g_stSnapBuf[_iType][_iNode].m_iNextNodeIdx;
		if (SNAP_BUF_STOPNODE_IDX != iChkNode)
		{
			if (g_stSnapBuf[_iType][_iNode].m_iAttr == g_stSnapBuf[_iType][iChkNode].m_iAttr)
			{
				rec_DayDelNode(_iNode, _iType);
				return 0;
			}
		}
		// 检测前一个结点
		iChkNode = g_stSnapBuf[_iType][_iNode].m_iPreNodeIdx;
		if (SNAP_BUF_STARTNODE_IDX != iChkNode)
		{
			if (g_stSnapBuf[_iType][_iNode].m_iAttr == g_stSnapBuf[_iType][iChkNode].m_iAttr)
			{
				rec_DayDelNode(iChkNode, _iType);
			}
		}
		// 如果后一个结点是开始结点，两者相差时间为模糊时间以内，均删除
		iChkNode = g_stSnapBuf[_iType][_iNode].m_iNextNodeIdx;
		if (SNAP_BUF_STOPNODE_IDX != iChkNode)
		{
			if (g_stSnapBuf[_iType][iChkNode].m_Time - g_stSnapBuf[_iType][_iNode].m_Time <= _iBlurTime)
			{
				rec_DayDelNode(_iNode, _iType);
				rec_DayDelNode(iChkNode, _iType);
			}
		}
	}
	else
	{
		return -2;
	}

	return 0;
	
}



/******************************************************************************
	函数名称: rec_DayWriteChkBuf
	简要描述: 将时间存入缓冲区
	输入          : _iStartTime------开始时间
	                      _iStopTime-------结束时间
	                      _iBlurTime--------模糊时间
	输出          : 0表示存入成功
	修改日志: 2011-12-13 杨珏创建该函数
******************************************************************************/
int rec_DayWriteChkBuf(time_t _iStartTime, time_t _iStopTime, int _iBlurTime, int _iType)
{
	int iNode1 = 0;
	int iNode2 = 0;

	if (_iType >= DAYMAP_TYPE_NUM)
	{
		return -1;
	}
	
	// 添加结点
	iNode1 = rec_DayInsertNodeBuf(_iStartTime, SNAP_NODE_ATTR_START, _iType);
	iNode2 = rec_DayInsertNodeBuf(_iStopTime, SNAP_NODE_ATTR_STOP, _iType);

	// 结点添加失败，则删除添加的结点
	if ((iNode1 < 0) || (iNode2 < 0))
	{
		rec_DayDelNode(iNode1, _iType);
		rec_DayDelNode(iNode2, _iType);
		
		return -2;
	}

	// 检测结点，进行优化
	rec_DayOptNodeBuf(iNode1, _iBlurTime, _iType);
	rec_DayOptNodeBuf(iNode2, _iBlurTime, _iType);

	return 0;
}


/******************************************************************************
	函数名称: rec_DayMapGetRecType
	简要描述: 快照信息回调函数
	输入          : _iType-----------读出的文件类型
	输出          : 读出的信息数量
	修改日志: 2011-12-19 杨珏创建该函数
******************************************************************************/
int rec_DayMapGetRecType(int _iType)
{
	// 手动录像类型
	if (0 == (_iType % DAYMAP_IDX_RECTYPE_MANUAL))
	{
		return DAYMAP_RECTYPE_MANUAL;
	}
	// 视频丢失报警
	else if (0 == (_iType % DAYMAP_IDX_RECTYPE_VL))
	{
		return DAYMAP_RECTYPE_ALARM;
	}
	// 移动侦测报警
	else if (0 == (_iType % DAYMAP_IDX_RECTYPE_MD))
	{
		return DAYMAP_RECTYPE_ALARM;
	}
	// 端口报警
	else if (0 == (_iType % DAYMAP_IDX_RECTYPE_PORT))
	{
		return DAYMAP_RECTYPE_ALARM;
	}
	// od 录像
	else if (0 == (_iType % DAYMAP_IDX_RECTYPE_OD))
	{
		return DAYMAP_RECTYPE_ALARM;
	}
	// 定时录像
	else if (0 == (_iType % DAYMAP_IDX_RECTYPE_TIMING))
	{
		return DAYMAP_RECTYPE_TIMING;
	}
	//卡号录像
	else if (0 == (_iType % DAYMAP_IDX_RECTYPE_ATM))
	{
		return DAYMAP_RECTYPE_MANUAL;
	}
	else
	{
		return DAYMAP_RECTYPE_ERROR;
	}
}

#if 0

/******************************************************************************
	函数名称: rec_DayLoadDb
	简要描述: 快照信息回调函数
	输入          : _iTime------------模糊时间
	                      _iColumn----------字段总数
	                      _pcColumn_value---列内容
	                      _pcColumn_name---列名称
	输出          : 读出的信息数量
	修改日志: 2011-12-13 杨珏创建该函数
******************************************************************************/
int rec_DayLoadDb(void* _iTime, int _iColumn, char** _pcColumn_value, char** _pcColumn_name )
{
	int iTemp = 0;
	time_t iStartTime = 0;
	time_t iStopTime = 0;
	int iReadType = 0;
	int iType = 0;

	iStartTime = atoi(_pcColumn_value[SNAP_DB_START_IDX]);
	iStopTime = atoi(_pcColumn_value[SNAP_DB_STOP_IDX]);
	iReadType = atoi(_pcColumn_value[SNAP_DB_REC_IDX]);
	if (((iStopTime > iStartTime) && (iStopTime - iStartTime > SNAP_MAX_SPACETIME)) ||
		((iStartTime > iStopTime) && (iStartTime - iStopTime > SNAP_MAX_SPACETIME)))
	{
		return 0;			// 杨珏20120419 返回负值后其余内容则不会继续读取
	}
	iType = rec_DayMapGetRecType(iReadType);			// 读取日快照类型

	// 将读到的开始时间和结束时间存入到缓冲区中
	iTemp = rec_DayWriteChkBuf(iStartTime, iStopTime, *((int*)_iTime), iType);

	return 0;				// 杨珏20120419 返回负值后其余内容则不会继续读取
	
}
#endif
/******************************************************************************
	函数名称: rev_GetSnapTime
	简要描述: 计算快照信息
	输入          : _iStartTime------所需查看的开始时间
	                      _iStopTime-------所查看的结束时间
	                      _pstTime---------时间数据结构体
	                      _iMaxCnt---------最大读出数量
	输出          : 读出的信息数量
	修改日志: 2011-12-14 杨珏创建该函数
******************************************************************************/
int rev_GetSnapTime(time_t _iStartTime, time_t _iStopTime, TSnapTime* _pstTime, int _iMaxCnt)
{
	int iNum = 0;
	int iNodeStart = 0;
	int iNodeStop = 0;
	int iType = 0;

	iNum = 0;
	for (iType=0; iType<DAYMAP_TYPE_NUM; iType++)
	{
		iNodeStart = SNAP_BUF_STARTNODE_IDX;					// 初始化为头结点
		iNodeStop = SNAP_BUF_STARTNODE_IDX; 					// 初始化为头结点
		for ( ; (iNum<MAXNUM_SNAPBUF)&&(iNum<_iMaxCnt); )
		{
			// 检测起始结点信息
			iNodeStart = g_stSnapBuf[iType][iNodeStop].m_iNextNodeIdx;		// 获取起始结点
			// 该类型数据已经处理完毕
			if ((iNodeStart < 0) || (SNAP_BUF_STOPNODE_IDX == iNodeStart))
			{
				break;
			}
			// 读取错误，认为该类型数据出错，跳过后面所有
			if (SNAP_NODE_ATTR_START != g_stSnapBuf[iType][iNodeStart].m_iAttr)
			{
				break;
			}
			// 检测结束结点信息
			iNodeStop = g_stSnapBuf[iType][iNodeStart].m_iNextNodeIdx;				// 获取结束结点
			// 该类型数据已经处理完毕
			if ((iNodeStop < 0) || (SNAP_BUF_STOPNODE_IDX == iNodeStop))
			{
				break;
			}
			// 读取错误，认为该类型数据出错，跳过后面所有
			if (SNAP_NODE_ATTR_STOP != g_stSnapBuf[iType][iNodeStop].m_iAttr)
			{
				break;
			}

			// 开始结点在起始时间之前
			if (g_stSnapBuf[iType][iNodeStart].m_Time < _iStartTime)
			{
				// 如果两个结点均在开始时间之前，不操作，继续
				if (g_stSnapBuf[iType][iNodeStop].m_Time < _iStartTime)
				{
					continue;
				}
				// 如果结尾结点在结束时间之后，则取起始时间为开始时间，结束时间为结尾时间
				else if (g_stSnapBuf[iType][iNodeStop].m_Time > _iStopTime)
				{
					(_pstTime + iNum)->m_iType = iType;
					(_pstTime + iNum)->m_iStartTime = _iStartTime;
					(_pstTime + iNum)->m_iStopTime = _iStopTime;
					iNum++;
					break;
				}
				// 如果结尾结点在结束时间之前，去结尾结点，并取开始时间为起始时间
				else
				{
					(_pstTime + iNum)->m_iType = iType;
					(_pstTime + iNum)->m_iStartTime = _iStartTime;
					(_pstTime + iNum)->m_iStopTime = g_stSnapBuf[iType][iNodeStop].m_Time;
				}
			}
			// 起始时间如果超过了结束时间，则直接退出该类型
			else if (g_stSnapBuf[iType][iNodeStart].m_Time > _iStopTime)
			{
				break;
			}
			else
			{
				// 结尾结点在结束时间之后，取结束时间为结尾时间
				if (g_stSnapBuf[iType][iNodeStop].m_Time > _iStopTime)
				{
					(_pstTime + iNum)->m_iType = iType;
					(_pstTime + iNum)->m_iStartTime = g_stSnapBuf[iType][iNodeStart].m_Time;
					(_pstTime + iNum)->m_iStopTime = _iStopTime;
					iNum++;
					break;
				}
				else
				{
					(_pstTime + iNum)->m_iType = iType;
					(_pstTime + iNum)->m_iStartTime = g_stSnapBuf[iType][iNodeStart].m_Time;
					(_pstTime + iNum)->m_iStopTime = g_stSnapBuf[iType][iNodeStop].m_Time;
				}
			}
			
			iNum++;
		}
	}
	
	return iNum;
}

static int rec_CalendarMaxDaysForMon(int _iYear, int _iMon, int _iDay)
{
	if(_iYear == 0 || _iMon == 0 || _iDay == 0)
	{
		return -1;
	}
	if ((_iYear < 1900) || (_iYear > 9999))
	{
		return -1;
	}
	if (2 == _iMon)
	{
		if ((_iYear % 100 != 0 && _iYear % 4 == 0) || (_iYear % 400 == 0))
		{
			if(_iDay > 29)
			{
				return -1;
			}
		}
		if(_iDay > 28)
		{
			return -1;
		}
	}
	else if (_iMon <= 7)
	{
		if(_iDay > 30 + ((_iMon & 1) ? 1 : 0))
		{
			return -1;
		}
	}
	else
	{
		if(_iDay > 30 + ((_iMon & 1) ? 0 : 1))
		{
			return -1;
		}
	}
	return 0;
}

/******************************************************************************
	函数名称: rec_GetDayMap
	简要描述: 获取一天硬盘快照信息
	输入          : _pstDb-------数据库指针
	                      _iYear--------所查看的年份
	                      _iMonth-------所查看的月份
	                      _iDay---------所查询的日期
	                      _iChannelNo---通道号
	                      _iBlurTime-----模糊时间
	                      _iMaxCnt------最大数量
	                      _pcInfo-------读出信息
	输出          : 读出的信息数量
	修改日志: 2011-12-13 杨珏创建该函数
******************************************************************************/

static int rec_JudgeFileValid(TFileInfo *_ptInfo, int _iStartTm, int _iEndTm)
{
	int iFileStartTm;
	int iFileEndTm;
	int iFileType;
	
	if(NULL == _ptInfo)
	{
		return td_failure;
	}

	iFileStartTm = _ptInfo->m_iStartTime;
	iFileEndTm = _ptInfo->m_iStopTime;
	iFileType = _ptInfo->m_iRecAttr;
	
	if(DAYMAP_RECTYPE_ERROR == rec_DayMapGetRecType(iFileType))
	{
		return td_failure;
	}
	if (((iFileEndTm > iFileStartTm) && (iFileEndTm - iFileStartTm > SNAP_MAX_SPACETIME)) ||
	((iFileStartTm > iFileEndTm) && (iFileStartTm - iFileEndTm > SNAP_MAX_SPACETIME)))
	{
		return td_failure;
	}
	if(iFileEndTm <= _iStartTm)
	{
		return td_failure;
	}
	return td_success;
}
static int rec_QueryCondion(int _iChannelNo, int _iStartTm, int _iEndTm, int _iBlurTime)
{
	int i = 0;
	int iRet = 0;
	TQuery Query;
	TFileInfo* ptInfo = NULL;;

	//查询条件
	Query.m_uiPageSize = PAGE_SHOW_NUM;
	Query.m_iStartTime = (_iStartTm - 7200 > 0) ? _iStartTm - 7200 : 0;//提前2小时查询，判断边界
	Query.m_iStopTime = _iEndTm;
	Query.m_ucFileType = FILE_REC;
	Query.m_ucRecType = 0xff;			//查询录像所有类型
	Query.m_ucDevType =0xff;
	Query.m_ucChannelNo = _iChannelNo;
	Query.m_uiPageNo = 0;
	Query.m_uiTotalNum = PER_QUERY_NUM;
	Query.m_cReserved[0] = '\0';	

	ptInfo = (TFileInfo *)malloc(sizeof(TFileInfo) * PER_QUERY_NUM);
	if(NULL == ptInfo)
	{
		goto leave;
	}
	//查询文件列表
	iRet = rec_QueryAllFiles(&Query, ptInfo);
	if(ERR_REC_SUCCESSFUL == iRet)
	{
		Query.m_uiTotalNum = (Query.m_uiTotalNum> PER_QUERY_NUM) ? PER_QUERY_NUM : Query.m_uiTotalNum;
		for(i = 0; i < Query.m_uiTotalNum; i++)
		{
			if(rec_JudgeFileValid(ptInfo + i, _iStartTm, _iEndTm))
			{
				continue;
			}
			rec_DayWriteChkBuf(ptInfo[i].m_iStartTime, ptInfo[i].m_iStopTime, 
				_iBlurTime, rec_DayMapGetRecType(ptInfo[i].m_iRecAttr));
		}
	}
leave:
	if(NULL != ptInfo)
	{
		free(ptInfo);
	}
	return iRet;
}

int rec_GetDayMap(int _iYear, int _iMonth, int _iDay, int _iChannelNo, int _iBlurTime, int _iMaxCnt, TSnapTime* _pstTime)
{
	time_t iStartTime = 0;
	time_t iStopTime = 0;
	struct tm stChkTime;
	int iNum = 0;

	// 参数检测

	if(rec_CalendarMaxDaysForMon(_iYear, _iMonth, _iDay) < 0)
	{
		return SNAP_ERR_ARG;
	}
	if (_iBlurTime < SNAP_DEFAULT_BLURTIME)
	{
		_iBlurTime = SNAP_DEFAULT_BLURTIME;
	}
	
	memset(&stChkTime, 1, sizeof(stChkTime));			// 结构体初始化
	// 读出当前日期
	stChkTime.tm_year = _iYear - 1900;
	stChkTime.tm_mon = _iMonth - 1;
	stChkTime.tm_mday = _iDay;

	// 计算开始时间
	stChkTime.tm_hour = 0;
	stChkTime.tm_min = 0;
	stChkTime.tm_sec = 0;
	iStartTime = mktime(&stChkTime);

	// 计算结束时间
	stChkTime.tm_hour = 23;
	stChkTime.tm_min = 59;
	stChkTime.tm_sec = 59;
	iStopTime = mktime(&stChkTime);

	rec_DayInitChkBuf();			// 快照缓冲区初始化

	rec_QueryCondion(_iChannelNo, iStartTime, iStopTime, _iBlurTime);

	iNum = rev_GetSnapTime(iStartTime, iStopTime, _pstTime, _iMaxCnt);		// 计算快照信息
	
	return iNum;

}

/******************************************************************************
	函数名称: main
	简要描述: 主函数
	输入          : 无
	输出          : 返回0表示成功，否则出故障
	修改日志: 2011-11-15 杨珏创建该函数
******************************************************************************/
#if 0
int main(int argc, char *argv[])
{
	int iMonthInfo[DAYNUM_BUF];
	int iTemp = 0;
	int i = 0;
	//sqlite3* pstDb = NULL;
	//TSnapTime stSnapTime[1024];
	//int iNum = 0;
	//int iChannel = 0;
	//int iDay = 13;

	printf("hello,world\n");
	
	iTemp = rec_GetMonthMap("/mnt/sata", 2011, 12, 255, iMonthInfo);
	if (iTemp < 0)
	{
		printf("read error!\n");
		return -1;
	}

	for (i=1; i<DAYNUM_MONTH; i++)
	{
		printf("2011/01/%.2d %s\n", i, (0 == iMonthInfo[i] ? "N" : "Y"));
	}

#if 0
	//iChannel = 28;

	while (1)
	{
		iTemp = sqlite3_open("./index.db", &pstDb);
		if (iTemp != SQLITE_OK)
		{
			//数据库打开失败
			printf("database open failed.\n");
			return -1;
		}
		//printf("database open successful.\n");

		printf("day is %d\n", iDay);
		printf("Channel number is %d\n", iChannel);
		iNum = rec_GetHDSnapDay(pstDb, 2011, 11, iDay, iChannel, 5, 1024, stSnapTime);
		//iNum = rec_GetHDSnapDay(pstDb, 2011, 7, 1, iChannel, 5, 1024, stSnapTime);
		printf("read number is %d\n", iNum);
		for (i=0; i<iNum; i++)
		{
			printf("type: %d\n", stSnapTime[i].m_iType);
			//printf("starttime: %.4d/%.2d/%.2d %.2d:%.2d:%.2d\n", stSnapTime[i].m_stStartTime.tm_year+1900, 
			//													 stSnapTime[i].m_stStartTime.tm_mon+1, 
			//													 stSnapTime[i].m_stStartTime.tm_mday, 
			//													 stSnapTime[i].m_stStartTime.tm_hour, 
			//													 stSnapTime[i].m_stStartTime.tm_min, 
			//													 stSnapTime[i].m_stStartTime.tm_sec);
			//printf("stoptime: %.4d/%.2d/%.2d %.2d:%.2d:%.2d\n", stSnapTime[i].m_stStopTime.tm_year+1900, 
			//													 stSnapTime[i].m_stStopTime.tm_mon+1, 
			//													 stSnapTime[i].m_stStopTime.tm_mday, 
			//													 stSnapTime[i].m_stStopTime.tm_hour, 
			//													 stSnapTime[i].m_stStopTime.tm_min, 
			//													 stSnapTime[i].m_stStopTime.tm_sec);
			printf("starttime is %d\n", stSnapTime[i].m_iStartTime);
			printf("stoptime is %d\n", stSnapTime[i].m_iStopTime);
		}

		printf("\n");
		
		sqlite3_close(pstDb);

		iChannel++;
		//if (iChannel >= 16)
		if (iChannel >= 32)
		{
			iChannel = 0;
			iDay++;
			if (iDay > 30)
			{
				iDay = 12;
			}
		}

		sleep(1);
		
	}

#endif

	return 0;
}

#endif


/*---------------------------end of the file----------------------------*/

