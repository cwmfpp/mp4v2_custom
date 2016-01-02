/*************************************************************
*transplant&modify for this linux version  2006.9.1
*
**************************************************************/
#ifndef TCPSERVERH_HEAD_FILE
#define TCPSERVERH_HEAD_FILE

#define NS_MAX_CLIENT	64		/*一个TcpServer最多可以有64个连接	*/

// for test dxl 20110725
#ifdef TD_DEBUG
#define NS_MAX_CONNECT	64
#else
#define NS_MAX_CONNECT 	48		/*一个TcpServer最多可以有32个connect	*/
#endif

#include "public_def.h"
#include "td_types.h"
#include "td_media.h"

typedef struct TcpServer
{
	HI_SOCKET			m_sTcpListen;		//监听SOCKET
	pthread_t				m_pthreadListen;	//监听线程	
	PCONNECT			m_pClients[NS_MAX_CLIENT];			//连接的客户端列表1
	td_u16				m_u16Port;			//监听端口	
	td_u16				m_u16Reserv;		//保留,用于32位对齐
	TCP_PROCESSCONNECT  	m_ConnectEvent;		//连接成功建立后的回调函数
	
	int (*Init)(void* _tsThis, td_u16 _u16Port,TCP_PROCESSRECVCMD _RecvCmdEvent,
		TCP_PROCESSRECVDATA _RecvDataEvent,TCP_PROCESSCONNECT _ConnectEvent, 
		TCP_PROCESSDISCONNECT _DisConnectEvent,TCP_PROCESSERROR _ErrEvent);
	/*
	功能：	创建TcpServer
			绑定端口，为数据接收回调函数赋值
			创建监听任务
			创建邮箱
		
	参数：	_u16Port			端口
			_RecvEvent			处理数据接收的回调函数
			_ErrEvent			错误处理回调函数
			_ConnectEvent		客户端连接成功的回调函数
			_DisConnectEvent	客户端断开连接的回调函数
		
	返回：	无
	*/
	
	int (*SendData)(void* _tsThis, PCONNECT _pClient, td_u8* _u8SendBuffer, td_u32 _u32SendLength,td_u8 _u8Channel);
	/*
	功能：	进行数据发送操作	
	
	参数：	_u8SendBuffer	数据发送缓冲
			_u32SendLength	数据发送缓冲长度	
	
	返回：	无
	*/
	
	int (*SendCmd)(void* _tsThis, PCONNECT _pClient, td_u8* _u8SendBuffer,td_u32 _u32SendLength);
	/*
	功能：	进行命令发送操作	
	
	参数：	_u8SendBuffer	数据发送缓冲
			_u32SendLength	数据发送缓冲长度	
	
	返回：	无
	*/	

	PCONNECT  (*FindConnect)(void* _tsThis, td_u32 _socket);
	/*
	功能：	根据Socket 号来查找连接 
	
	参数：	_socket	socket号
	
	返回：	连接指针		
	*/
	
	void (*CloseOneLink)( PCONNECT _pClient);
	/*
	功能：	关闭某一个连接，即关闭该连接的SOCKET，同时删除该连接的接收任务
			该函数在处理连接错误时在内部调用(不能在内部调用)
			在收到客户端断开消息后由外部调用
	
	参数：	_i	连接的序号
	
	返回：	无
	*/
	
	void (*Close)(void* _tsThis);
	/*
	功能：	关闭TcpServer
			停掉监听任务
			停掉数据发送和接收任务
			删除邮箱
	
	参数：	无
	
	返回：	无
	*/
	
}TcpServer;

//初始化
int Tcp_Init();			

//去初始化
int Tcp_Uninit();		

//创建一个TcpServer
TcpServer *Tcp_CreateTcpServer(td_u16 _u16Port,TCP_PROCESSRECVCMD _RecvCmdEvent,
		TCP_PROCESSRECVDATA _RecvDataEvent,TCP_PROCESSCONNECT _ConnectEvent, 
		TCP_PROCESSDISCONNECT _DisConnectEvent,TCP_PROCESSERROR _ErrEvent);

//销毁一个UDP命令连接
int Tcp_DestroyTcpServer(TcpServer* _tcpServer);


#endif

