#ifndef _NET_TCP_COMMON_H
#define _NET_TCP_COMMON_H

#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>

#include "public_def.h"
#include "td_types.h"
#include "libdatacenter.h"

#define USE_SYS_MUTEX	1

#if USE_SYS_MUTEX
#include <pthread.h>

#define TDataLock 			pthread_mutex_t
#define InitDataLock(lck)		pthread_mutex_init(&(lck), NULL)
#define LockDataLock(lck)		pthread_mutex_lock(&(lck))
#define UnLockDataLock(lck)	pthread_mutex_unlock(&(lck))
#else
#include <unistd.h>

#define TDataLock	int
#define InitDataLock(lck)		(lck) = 0
#define LockDataLock(lck)		{while ((lck) > 0) {usleep(14);}; (lck)++;}
#define UnLockDataLock(lck)	(lck)--
#endif

#define SENDBUFNUM 	4

#define TCPSTREAMSIZE		1460
#define RECVBUFSIZE			4096
#define BUFFER_LENGTH_CMD         384

#define STREAMSIZE              1440
#define COMMANDSIZE             1440
#define TCPSTREAMSIZE		1460
#define RECVBUFSIZE				4096

typedef int HI_SOCKET;

#define CONNECTION_ALIVE  0x00
#define CONNECTION_CLOSE 0x01

#define CONNECTION_PASSIVE 0x00			//add 2010/5/7
#define CONNECTION_ACTIVE  0x01			//add 2010/5/7	

#define CONNECT_FREE 0x00
#define CONNECT_BUSY 0x01

#ifndef INVALID_SOCKET
#define INVALID_SOCKET  -1
#endif

#define  DATAPACKHEAD			0xF9EAF5F1    //数据包包头 最后4位为版本号
#define  COMMPACKHEAD			0xF5EAF5F1    //命令包包头

typedef struct
{
	td_u32	m_ui32Head;			//数据包包头
	td_u32    m_ui32Serial;         //序列号
	td_u32	  m_ui32Source;	        //数据包源地址
	td_u8     m_ui8Channel;         //通道号
	td_u8     m_ui8FrameType;       //帧类型
	td_u16    m_ui16Size;      		//数据长度
    	td_u32    m_ui32StreamType;//码流类型
}STREAMHEAD,*PSTREAMHEAD;


//命令包包头
typedef struct
{
	td_u32     m_ui32Head;			    //命令包包头
	td_u16	   m_ui16Serial;	        //序列号,用来判断是否丢包
	td_u16     m_ui16CommandSize;       //命令总长度
	td_u32	   m_ui32Target;		    //命令包目的地，如果为全1则是广播包
	td_u32	   m_ui32Source;	        //命令包源地址
    td_u16	   m_ui16Order;		        //命令码
	td_u16	   m_ui16ParaCount;	        //参数的长度
}COMMANDHEAD,*PCOMMANDHEAD;


typedef td_u8 (*TCP_PROCESSDISCONNECT)(void* _pClient);
/*
	功能：	客户端断开连接的回调函数
	参数：  _pClient	该连接的指针
*/
typedef td_u8 (*TCP_PROCESSCONNECT)(void* _pClient);
/*
    功能：  连接成功建立后的回调函数()
	参数：  _pClient	该连接的指针          
*/
typedef td_u8 (*TCP_PROCESSRECVCMD)(td_u8* _u8Buf, int _iBufLength, void* _pClient);
/*
    功能：  SOCKET收到命令后的回调函数
    
    参数：  _cBuf：			接收缓冲指针
            _iBufLength：	接收缓冲大小
			_pClient		该连接的指针  
*/
typedef td_u8 (*TCP_PROCESSRECVDATA)(td_u8* _u8Buf, int _iBufLength, void* _pClient);
/*
功能：  SOCKET收到数据后的回调函数
    
    参数：  _cBuf：			接收缓冲指针
            _iBufLength：	接收缓冲大小
			_pClient		该连接的指针
*/
typedef td_u8 (*TCP_PROCESSERROR)(td_u8 _u8Error, void* _pClient);
/*
	功能：	SOCKET发生错误后的回调函数
	
	参数：	_u8Error:		错误号
			_pClient		该连接的指针  
*/

/*为每一个连接建立一个结构,记录连接信息*/
typedef struct ConnectNode
{
	HI_SOCKET			m_socket;					//与客户端连接的SOCKET，必须写在第一行
	HI_SOCKET			m_Cmdsocket;				//与数据通道对应的命令通道
	td_u32				m_u32IpAddress;				//客户端的IP地址
	pthread_t				m_pthreadRecv;				//接收线程
	pid_t					m_pidRecv;					//接收线程PID 陶永亮 090619 添加
	pthread_t				m_pthreadSend;				//发送线程
	volatile int					m_hRecState;				//发送任务句柄  Sherry add 20051121	
	volatile int 					m_hSendState;
	int					m_iNeedFrameType;
	//Cheng 02/04 添加,与m_LiveTime联合使用，当命令通道执行阻塞操作
	//设置m_BlockExec为1，心跳不累加，否则累加
	td_u32                             m_BlockExec;
	td_u32				m_LiveTime;
	//int 				m_hMbxSend;					//发送数据邮箱句柄
	td_u32				m_ConnectType;				//连接标记1:cmd  2;data 0:dida
	td_u32				m_bConnected;				//该连接是否已被占用
	td_u32				m_chnNum;					//记录连接哪个通道数据
	td_u8				m_u8SendBuf[TCPSTREAMSIZE];	//数据发送缓冲 记录
	TDataLock				m_tDataLock;			//数据锁dxl 2010/08/13
	TDataPackage *                m_CurrentDataPkt;      //记录未发送成功的数据包
	td_u32                             	m_SendPos;                 //记录已发送的位置
	td_u8				m_u8RecvBuf[RECVBUFSIZE];	//数据接收缓冲	Sherry add 20051122
	td_u32				m_u32RecvLen;				//接收到的数据长度
	td_u32			  	m_u32BufDataLen;            //记录m_u8SendBuf的有效数据长度， 0 为无数据
	td_u16				m_u16Port;					//端口
	td_u8				m_u8Flag;				//add 2010/5/7用于标识该连接的模式，主动或被动
	td_s32              m_iCharSet;            //记录客户端字符集 1--GB2312   2--UTF-8
	//Cheng 10/01/13 from S2 for multi IP addr
	td_u8					m_u8ServerIP[16];				//客户端连接的服务器IP
	TCP_PROCESSRECVCMD     	m_RecvCmd;					//命令接收回调函数
	TCP_PROCESSRECVDATA		m_RecvData;					//数据接收回调函数
	TCP_PROCESSERROR		m_ErrEvent;					//错误处理回调函数
	TCP_PROCESSCONNECT  		m_ConnectEvent;				//连接成功建立后的回调函数
	TCP_PROCESSDISCONNECT	m_DisConnectEvent;			//客户端断开连接的回调函数
	TDataFIFO* 				m_DataFifo;
	
	TDataCenter* m_pDataCenter;

	TDataLock	m_tSafeLock;						//安全锁 20110721
	
	struct ConnectNode	*m_next;
	
	
	void (*ParseStreamData)(void* _pClient,td_u8* _u8RecvBuffer, td_u32 _u32RecvLength);
	/*
		功能：	解析收到的数据，如果收到的为命令包，则调用命令回调函数
	    	    如果说道的为数据包，则调数据回调函数	
	
		参数：	_u8RecvBuffer	接收数据缓冲
				_u32RecvLength	接收数据长度
	
		返回：	无	
	*/
}CONNECT, *PCONNECT;	

static inline TDataPackage *GetDataPakageOfConnect(PCONNECT _ptConnect) //dxl
{
	TDataPackage *ptPkg = NULL;
	
	LockDataLock(_ptConnect->m_tDataLock);

	ptPkg = _ptConnect->m_CurrentDataPkt;

	_ptConnect->m_CurrentDataPkt = NULL;
		
	UnLockDataLock(_ptConnect->m_tDataLock);

	return ptPkg;
}

static inline void SetDataPakageOfConnect(PCONNECT _ptConnect, TDataPackage *_ptPkg) //dxl
{
	LockDataLock(_ptConnect->m_tDataLock);

	if (_ptConnect->m_CurrentDataPkt)
	{
		datacenter_PackageRelease(_ptConnect->m_CurrentDataPkt);
	}

	_ptConnect->m_CurrentDataPkt = _ptPkg;
		
	UnLockDataLock(_ptConnect->m_tDataLock);
}

#endif
