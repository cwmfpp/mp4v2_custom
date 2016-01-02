#ifndef _NET_NETUDP_H
#define _NET_NETUDP_H


typedef int (*PROCESSRECV)(td_u8* _u8Buf,int _iBufLength,
	td_u32 _u32IpAddress,td_u16 _u16Port,int _socket);
/*
    功能：  SOCKET收到数据后的回调函数
    
    参数：  _cBuf：			接收缓冲指针
            _iBufLength：	接收缓冲大小
            _astrFromIP：	数据发送端的IP地址
            _iPort：		接收数据的端口
            
    返回值:	0	成功
			-1	失败
*/

typedef int (*PROCESSERROR)(td_u8 _u8Error,td_u32 _u32IpAddress,td_u16 _u16Port);
/*
	功能：	SOCKET发生错误后的回调函数
	
	参数：	_u8Error:		错误号
			_u32IpAddress:	IP地址
			_u8Port:		端口号
	
	返回值:	0	成功
			-1	失败
*/

typedef struct
{
	td_u8       m_u8Type;		//该消息所应用的网络类型
	//td_u8	m_u8Buf[STREAMSIZE];
	td_u8*	m_u8Buf;
	td_u32 	m_u32Length;	//发送数据缓冲大小
	td_u32      m_u32IpAddress;	//目的IP
	td_u32	m_u32Port;
	td_u16      m_u16Reply;
}DataSend_Msg;


typedef struct CMDUdp
{
  	int 				m_sUdp;
	td_u32  			m_u32Port;
	
	td_u8*			m_u8SendBuffer;
	td_u8*			m_u8RecvBuffer;	
	PROCESSRECV  	m_RecvEvent;
	PROCESSERROR 	m_ErrEvent;
	
	pthread_t			m_pthreadRecv;
	pthread_t			m_pthreadSchedule;
	

	void (*Init)(void* _uThis,td_u32 _u32Port,PROCESSRECV _RecvEvent,
				PROCESSERROR _ErrEvent,int _iMode);
/*	
	功能：创建Udp
			声明SOCKET 为SOCKET绑定地址和端口
			为数据接收回调函数赋值
			分别创建数据发送任务和数据接收任务
			创建邮箱(若只进行数据接收操作可不创建)
				
	参数：_u32Port		端口
			_RecvEvent		处理数据接收的回调函数
			_ErrEvent		处理网络错误的回调函数
			_iMode			0x01 该UDP仅用于数据发送操作
							0x02 该UDP可同时应用于数据接收及发送操作
		
	返回值：无
*/	
	void (*SendData)(void* _uThis, td_u32 _u32IpAddress, td_u32 _u32Port,
					td_u8* _u8SendBuffer, td_u32 _u32SendLength, td_u16 _reply);
/*	
	功能：进行数据发送操作	
			根据函数参数为m_MsgDataSend的两个成员 m_cBuf和m_iLength赋值
			向数据发送任务Post m_MsgDataSend
			(数据发送任务启动后即处于等待消息的状态，每收到一次Msg即进行一次数据发送操作)
		
	参数：_u32IpAddress	目的IP地址
			_u8SendBuffer	发送数据缓冲
			_u32SendLength	发送数据缓冲长度	
			_reply			该命令是否需要回应		
		
	返回值：无
*/	
	
	void (*Close)(void* _uThis);
/*	
	功能：关闭Udp
			停掉数据发送和数据接收任务
			将邮箱删除
	参数：	无
	返回值：无
*/	
		
	//DataSend_Msg	m_MsgDataSend;	

	char*			m_cmdQueue;
}CMDUdp;


typedef struct DATAUdp
{
  	int 			m_sUdp;  	

	td_u32  		m_u32Port;
	
	td_u8*			m_u8SendBuffer;
	td_u8*			m_u8RecvBuffer;	
	PROCESSRECV  	m_RecvEvent;
	PROCESSERROR 	m_ErrEvent;
	
	pthread_t			m_pthreadRecv;
	pthread_t			m_pthreadSchedule;
	
	td_u32		m_RecvStopFlag;
	td_u32		m_ScheduleStopFlag;

	td_u32		m_ExitFlag;
	TDataFIFO* 	m_DataFifo;
	
	//MBX_Handle  	m_hUDPDataSendMbx;	//用于数据发送操作的邮箱

	void (*Init)(void* _uThis, td_u32 _u32Host, td_u16 _u16Port, PROCESSRECV _RecvEvent, 
				PROCESSERROR _ErrEvent, int _iMode);
/*	
	功能：	创建Udp
			声明HI_SOCKET 为HI_SOCKET绑定地址和端口
			为数据接收回调函数赋值
			分别创建数据发送任务和数据接收任务
			创建邮箱(若只进行数据接收操作可不创建)
				
	参数：	_u32Port		端口
			_RecvEvent		处理数据接收的回调函数
			_ErrEvent		处理网络错误的回调函数
			_iMode			0x01 该UDP仅用于数据发送操作
							0x02 该UDP可同时应用于数据接收及发送操作
		
	返回值：无
*/	
	void (*SendData)(void* _uThis, td_u8* _u8SendBuffer,td_u32 _u32SendLength,
		td_u32 _u32IpAddress,td_u16 _u16Port);
/*	
	功能：	进行数据发送操作	
			根据函数参数为m_MsgDataSend的两个成员 m_cBuf和m_iLength赋值
			向数据发送任务Post m_MsgDataSend
			(数据发送任务启动后即处于等待消息的状态，每收到一次Msg即进行一次数据发送操作)
		
	参数：	_u32IpAddress	目的IP地址
			_u8SendBuffer	发送数据缓冲
			_u32SendLength	发送数据缓冲长度	
			_reply			该命令是否需要回应		
		
	返回值：无
*/	
	
	void (*Close)(void* _uThis);
/*	
	功能：	关闭Udp
			停掉数据发送和数据接收任务
			将邮箱删除
	参数：	无
	返回值：无
*/	
		
	DataSend_Msg	m_MsgDataSend;			
	char*		m_dataBuffer;  //头结点(不包含信息)	
}DATAUdp;

//初始化
int Udp_Init();			

//去初始化
int Udp_Uninit();		

//创建一个UDP命令连接
CMDUdp *udp_CreateCmdNode(td_u32 _u32Port,	
	PROCESSRECV _RecvEvent,PROCESSERROR _ErrEvent,int _iMode);

//销毁一个UDP命令连接
int Udp_DestroyCmdNode(CMDUdp* _CmdUdp);

//创建一个UDP数据连接
DATAUdp *Udp_CreateDataNode( td_u32 _u32Host, 
	td_u16 _u16Port, PROCESSRECV _RecvEvent, PROCESSERROR _ErrEvent, int _iMode);

//销毁一个UDP数据连接
int Udp_DestroyDataNode(DATAUdp* _DataUdp);

#endif
