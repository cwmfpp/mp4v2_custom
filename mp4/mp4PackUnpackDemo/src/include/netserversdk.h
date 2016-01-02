#ifndef _NET_SERVER_SDK
#define _NET_SERVER_SDK

#include "public_def.h"
#include "libdecoder.h"

typedef struct
{
	int m_iChnType; // 1 代表命令，2代表数据
	int m_iNetMode; //1，TCP； 2，UDP； 3，多播； 4，主动模式
	int m_iConnectState; // 0代表连接状态；  1代表断开状态
	int m_iChnNo; //通道号
	unsigned int m_uiServerIp; //客户端连接IP
	char m_cUserName[16]; //用户名
	int m_isocket;//wangsong0616
}TClientConnectInfo;

typedef struct
{
	char m_cUserName[16];
	char m_cUserIp[16];
}TClientUserInfo;  

void NetServer_Init(char* _cIpAddress,td_s32 _s32ServerPort,td_s32 _s32ChannelNum );
void NetMsg_HeartBeat(unsigned long _param);

// dxl 添加到此
void NetMsg_NFSState(unsigned int state);
int 	NetMsg_VideoLost(int chn,int state, td_u32 _u32IP);
int 	NetMsg_GPin(int pin,int state, td_u32 _u32IP);
int NetMsg_MotionDetect(int chn,int state, td_u32 _u32IP);
void NetMsg_OutPortState(td_s32 _iPortNo, td_s32 _iState);
void NetMsg_ReportException(unsigned int state, unsigned int type, td_u32 _u32IP);
void NetMsg_RecordState(unsigned int chn, td_u32 _u32IP);
int  NetServer_SendAlarmMsg(char* _cMsg,int _iLen,  td_u32 _u32IP);

td_s32 DetectUnallowableAccess(void);
int  NetServer_SendDataToAllClient(td_u8* _u8Data,int _iLen,int _iComNo);
int  NetServer_SendStringToAllClient(char* _cMsg,int _iLen,int _iType);//20121022 王磊add

int NetServer_SendAppStringToClient(td_u32 _u32Ip, char *_strData);

void NetServer_DebugInfor(char *_cString);
td_s32 NetServer_GetRemotePreviewCost(td_s32 _iChn);

int NetServer_SetAppStreamProcFun(void (*_Fun)(td_u32 _uiIp, char *_str));
int NetServer_SetAppRecvComDataFun(void (*_Fun)(td_s32 iComNo, td_u8* _pcData, td_s32 _iLen));
int NetServer_SetRestartAppFun(void (*_Fun)());

td_s32 NetServer_GetOnLineUser(td_s32 * s32UserConNum, TClientUserInfo onLineUser []);
td_s32 NetSever_GetConnectInfo(td_s32 *s32ConnectNum, TClientConnectInfo pClientInfo[]);
td_s32 NetSever_GetUserNameBySocket(td_u32 _iSocket, char cUserName[LENGTH_USER_NAME]);

int NetMsg_AudioLost(int chn,int state,td_u32 _u32IP);//zhuangzhibing 发送音频丢失报警信息
int NetMsg_VideoCover(int chn,int state,td_u32 _u32IP);//20130305 SUN Wei add for OD alarm 发送视频遮挡报警信息
td_s32 NetMsg_VcaAlarmCount(td_u32 _u32IP, td_s32 _iChn, td_s32 _iRule, td_s32 _iVcaType, td_s32 _iTotalCount, td_s32 _iCpcIn, td_s32 _iCpcOut);
td_s32 NetMsg_VcaAlarmMsg(td_s32 _iNo, td_s32 _iRulesNo, TVcaEventType _eType, td_s32 _iState, td_u32 _u32TId, td_u32 _u32Type);


int NetMsg_AutoTest_LAN(int port, int iTestResult);
int NetMsg_AutoTest_USB(int port, int iTestResult);
int NetMsg_AutoTest_CheckSATA(int port,int iTestResult);
int NetMsg_AutoTest_VGA(int iTestResult);
int NetMsg_AutoTest_I2C(int iTestResult);
int NetMsg_AutoTest_HDMI(int iTestResult);
int NetMsg_AutoTest_Over(int iTestResult, td_u32 _u32IP);
int NetMsg_VcaAlarmCount(td_u32 _u32IP, int _iChn, int _iRule, int _iVcaType, int _iTotalCount, int _iCpcIn, int _iCpcOut);
int NetServer_Register(strParsePara* _pParsePara);
int ParseProtocolDirection(char* _cCmdMsg, int _iSocket);
// for decoder
void Dec_NetServer_Init(char* _cIpAddress,td_s32 _s32ServerPort,td_s32 _s32ChannelNum );
// "STARTVIEW" 回调
int cbk_Event_ViewState(int _iChn,int _iPic, TDECNVSITEM* _nvsItem,int  _iState);

//曹海波  添加  获取升级包 当前包号和总包长
int NetGetTotalPacketNum();
int NetGetProPacketNum();
#endif

