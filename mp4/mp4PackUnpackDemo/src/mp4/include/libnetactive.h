#ifndef NET_ACTIVE_TCP_H_
#define NET_ACTIVE_TCP_H_

typedef struct
{
	strParsePara 	m_ParsePara;			            //需修改 自己定义
	td_u8		m_cLocalIP[16];							//设备的局域网IP
 	td_u8		m_cFactoryID[32];		                //设备产品ID
 	td_u8		m_cRegistedIp[16];	                    //成功注册的本地ip
	td_u8		m_cProxiedIp[16];	                    //成功被代理的本地ip
}strRegisterMsg;

typedef td_s32 (*ACTIVE_PROCESS_CMD)(td_u8* _u8Buf, td_s32 _iBufLength, void* _pClient);			
/*
* 函数名称：ACTIVE_PROCESS_CMD
* 简要描述：处理命令的回调函数
* 输           入：_pClient		该连接的指针
*				  _u8Buf			命令缓冲
*				  _iBufLength 		命令长度
* 输           出：
* 修改日志：2010-01-28,陆翔云，创建
*/

typedef td_s32 (*ACTIVE_PROCESS_DATA)(td_u8* _u8Buf, td_s32 _iBufLength,void* _pClient);			
/*
* 函数名称：ACTIVE_PROCESS_DATA
* 简要描述：处理数据的回调函数
* 输           入：_pClient		该连接的指针
*				  _u8Buf			数据缓冲
*				  _iBufLength 		数据长度
* 输           出：
* 修改日志：2010-01-28,陆翔云，创建
*/

typedef td_s32 (*ACTIVE_PROCESS_ERROR)(td_u8 _u8Error, void* _pClient);
/*
* 函数名称：ACTIVE_PROCESS_ERROR
* 简要描述：处理连接出错的回调函数
* 输           入： _u8Error		错误码
				  _pClient			该连接的指针
* 输           出：
* 修改日志：2010-01-28,陆翔云，创建
*/
strRegisterMsg* active_GetRegisterInfo();

td_s32 	active_Init(strRegisterMsg* _pRegisterMsg,ACTIVE_PROCESS_CMD _recvCmdEvent,
			ACTIVE_PROCESS_DATA _recvDataEvent,ACTIVE_PROCESS_ERROR _errEvent);
/*
* 函数名称：active_Init
* 简要描述：初始化回调函数模块
* 输           入：_pParsePara		目录服务器的相关参数
*				  _RecvCmdEvent	用于处理命令的回调函数
*				  _recvDataEvent 	用于处理数据的回调函数
*				 _errEvent		用于处理连接出错的回调函数	
* 输           出：
* 修改日志：2010-01-28,陆翔云，创建
*/
void TcpClient_Init(strParsePara* ParsePara);

void active_DebugInfor(char *_cString);

void* active_FindConnect(td_u32 _Socket);
int active_FindCmdSocket(td_u32 _Socket);

#endif
