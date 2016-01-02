/*************************************************************
*george(393) transplant&modify for this linux version  2006.9.1
*
**************************************************************/
#ifndef _USER_H_
#define _USER_H_

#include "td_types.h"

//#define USER_BEGIN				1248
//#define LENGTH_USER				268

//用户权限
#define AUT_BROWSE				1		//仅可浏览视频		
#define AUT_BROWSE_CTRL			2		//可浏览视频并对设备进行控制	
#define AUT_BROWSE_CTRL_SET		3		//浏览视频、控制设备、修改设置参数
#define AUT_ADMIN				4		//超级用户，第0号用户为超级用户，
										//用户名为Admin且不可修改,初始密码为tiandy，
										//可进行用户的添加、删除、权限修改等操作

//Leeon Tonie add for Tiandy-Dvr IP Filter
#define IP_FILTER_ALLOW				2
#define IP_FILTER_FORBID			1
#define IP_FILTER_NONE				0
#define USER_ALL_CHAL_RIGHT	0XFFFFFFFF


/*
void InitUser();
//int GetUserAuthority(char* _cUserName);
*/
int  LoadUserList();
/*
 * 函数名称:LoadUserList
 * 简要描述:从中获得用户列表，同时初始化用于保存用户IP地址的StringList
 * 输	 入:
 *			无
 * 输	 出:
 *			0,成功
 * 			<0,失败
 * 修改日志:
 *			2008-10-28,陶永亮，整理
 */

int UserLogon(char* _cUserName,char* _cPassWord,td_u32 _u32IP,int _iAuthority,int _iNetMode, void* _cthis, td_u32 _u32Socket);
/*
 * 函数名称:UserLogon
 * 简要描述:网络用户登录
 * 输	 入:
 *			_cUserName,用户名
 *			_cPassWord,密码
 *			_u32IP,网络连接socket/IP地址
 *			_iAuthority,用户权限
 *			_iNetMode,网络模式:1,TCP 0,UDP
 *			_cthis,网络连接节点
 *			_u32Socket,socket号,dxl 20110307，添加此参数使user模块不依赖pconnect
 * 输	 出:
 *			0,成功
 * 			-2,已达用户数上限
 *			-3,非法IP(该IP已被屏蔽)
 *			-4,用户不存在或密码不正确
 * 修改日志:
 *			2008-10-28,陶永亮，整理
 */	
 
int UserLogoff(char* _cUserName,int _cIP);
/*
 * 函数名称:UserLogoff
 * 简要描述:注销网络用户登录
 * 输	 入:
 *			_cUserName,用户名
 *			_cIP,网络连接socket/IP地址
 * 输	 出:
 *			0,成功
 * 			-1,失败,该IP地址的用户未登录
 * 修改日志:
 *			2008-10-28,陶永亮，整理
 */	
 
int  AddUser(char* _cUserName,char* _cPwd,int _iAuthority);
/*
 * 函数名称:AddUser
 * 简要描述:添加用户
 * 输	 入:
 *			_cUserName,用户名
 *			_cPwd,密码
 *			_iAuthority,权限
 * 输	 出:
 *			0,成功
 * 			-1,该用户名已存在
 *			-2,用户数已达上限
 *			-3,用户名或密码非法(超过最大长度，包含字母和数字以外的字符)
 * 修改日志:
 *			2008-10-28,陶永亮，整理
 */	

int  DelUser(char* _cUserName);
/*
 * 函数名称:DelUser
 * 简要描述:删除用户
 * 输	 入:
 *			_cUserName,用户名
 * 输	 出:
 *			0,成功
 * 			-1,该用户名不存在
 *			-2,超级用户不允许被删除
 * 修改日志:
 *			2008-10-28,陶永亮，整理
 */	
 
int  ModifyPwd(char* _cUserName,char* _cPwdOld,char* _cPwdNew);
/*
 * 函数名称:ModifyPwd
 * 简要描述:修改用户密码
 * 输	 入:
 *			_cUserName,用户名
 *			_cPwdOld,原密码
 *			_cPwdNew,新密码
 * 输	 出:
 *			0,成功
 * 			-1,该用户名不存在
 *			-2,原密码不正确
 *			-3,新密码不合法(超过长度上限/包含字母或数字以外的字符)
 * 修改日志:
 *			2008-10-28,陶永亮，整理
 */
 
td_u8 GetUserNum();
/*
 * 函数名称:GetUserNum
 * 简要描述:获取用户数
 * 输	 入:
 *			无
 * 输	 出:
 *			用户数
 * 修改日志:
 *			2008-10-28,陶永亮，整理
 */
 
int GetUserInfo(td_u8 _u8UserSerial,char _cUserName[LENGTH_USER_NAME],char _cPassword[LENGTH_PASSWORD],int* _iAuthority);
/*
 * 函数名称:GetUserInfo
 * 简要描述:根据用户编码获取某个用户的信息
 * 输	 入:
 *			_u8UserSerial,用户编号
 *			_cUserName,传出用户名			
 *			_cPassword,传出用户密码
 *			_iAuthority,传出用户权限
 * 输	 出:
 *			0,成功
 * 			-1,用户编号不合法
 * 修改日志:
 *			2008-10-28,陶永亮，整理
 */
 
int  SetMaxConUser(int _iMaxConUser);
/*
 * 函数名称:SetMaxConUser
 * 简要描述:设置最大网络连接数
 * 输	 入:
 *			_iMaxConUser,最大网络连接用户数
 * 输	 出:
 *			0,成功
 * 			-1,用户编号不合法
 * 修改日志:
 *			2008-10-28,陶永亮，整理
 */
 
int  GetMaxConUser();
/*
 * 函数名称:GetMaxConUser
 * 简要描述:获取最大网络连接数
 * 输	 入:
 *			无
 * 输	 出:
 *			最大网络连接用户数
 * 修改日志:
 *			2008-10-28,陶永亮，整理
 */
 
int IsUserLogon(td_u32 _u32IP);
/*
 * 函数名称:IsUserLogon
 * 简要描述:判断某个用户是否已登录
 * 输	 入:
 *			_u32IP,网络连接socket/IP地址
 * 输	 出:
 *			1,已登录
 * 			0,未登录
 * 修改日志:
 *			2008-10-28,陶永亮，整理
 */

int ModifyLevel(char* _pcUser, int iLevel);
/*
 * 函数名称:ModifyLevel
 * 简要描述:修改用户权限
 * 输	 入:
 *			_pcUser,用户名
 *			iLevel,用户权限
 * 输	 出:
 *			0,成功
 *			-1,该用户不存在
 * 			-2,不允许修改Admin用户的权限
 * 修改日志:
 *			2008-10-28,陶永亮，整理
 */
 
int LocalLogOn(td_u8* _pcName, td_u8* _pcPass);
/*
 * 函数名称:LocalLogOn
 * 简要描述:本地用户登录
 * 输	 入:
 *			_pcName,用户名
 *			_pcPass,密码
 * 输	 出:
 *			-1,已经有其他用户登录
 *			-2,密码不正确
 *			-3,用户名不正确
 *			0,登录成功
 * 修改日志:
 *			2008-10-28,陶永亮，整理
 */
 
int LocalLogOff(td_u8 * _pcName);
/*
 * 函数名称:LocalLogOff
 * 简要描述:本地用户登出
 * 输	 入:
 *			_pcName,用户名
 * 输	 出:
 *			-1,无登入用户
 *			-2,已登入的不是该用户
 *			0,登出成功
 * 修改日志:
 *			2008-10-28,陶永亮，整理
 */
 
int LocalGetNowUser(td_u8* _pcName);
/*
 * 函数名称:LocalGetNowUser
 * 简要描述:获取本地已登录的用户名
 * 输	 入:
 *			_pcName,传出用户名
 * 输	 出:
 *			-1,参数为空或者当前无用户登录
 *			0,成功
 * 修改日志:
 *			2008-10-28,陶永亮，整理
 */
 
int GetUserLevel(td_u8* _pcName);
/*
 * 函数名称:GetUserLevel
 * 简要描述:获取用户权限
 * 输	 入:
 *			_pcName,用户名
 * 输	 出:
 *			-1,经查无此用户
 *			-2,非法输入
 *			>0,该用户的权限
 * 修改日志:
 *			2008-10-28,陶永亮，整理
 */

td_u8* GetUserPwd(const td_u8* _pcUserName);
/*
 * 函数名称:GetUserPwd
 * 简要描述:获取用户密码
 * 输	 入:
 *			_pcUserName,用户名
 * 输	 出:
 *			NULL,获取失败
 *			非空,用户密码
 * 修改日志:
 *			2008-10-28,陶永亮，整理
 */

int UpdateFtpuser(void);
/*
 * 函数名称:UpdateFtpuser
 * 简要描述:将用户列表同步到FTP
 * 输	 入:
 *			无
 * 输	 出:
 *			-1,同步失败
 *			0,同步成功
 * 修改日志:
 *			2008-10-28,陶永亮，整理
 */

td_s32 SetExpectIp(td_u32 _uiIp, td_s32 _uiMask);
/*
 * 函数名称:SetExpectIp
 * 简要描述:设置屏蔽IP
 * 输	 入:
 *			_uiIp,屏蔽IP
 *			_uiMask,屏蔽IP掩码
 * 输	 出:
 *			-1,参数不合法
 *			-2,保存配置文件失败
 *			0,成功
 * 修改日志:
 *			2008-10-28,陶永亮，整理
 */
 
td_s32 GetExpectIp(td_u32* _puiIp, td_u32* _puiMask);
/*
 * 函数名称:GetExpectIp
 * 简要描述:获取屏蔽IP
 * 输	 入:
 *			_puiIp,传出屏蔽IP
 *			_puiMask,传出屏蔽IP掩码
 * 输	 出:
 *			-1,参数非法
 *			0,成功
 * 修改日志:
 *			2008-10-28,陶永亮，整理
 */
td_s32 SetIpFilter(td_s32 _iType, td_s32 _iNum, td_u32* _puiList);
/*
 * 函数名称:SetIpFilter
 * 简要描述:设置IP黑名单/白名单
 * 输	 入:
 *			_iType,IP_FILTER_FORBID,黑名单/IP_FILTER_ALLOW,白名单/IP_FILTER_NONE,不启用
 *			_iNum,名单长度(IP数)
 *			_puiList,名单
 * 输	 出:
 *			-1,参数非法
 *			-2,保存配置文件失败
 *			0,成功
 * 修改日志:
 *			2008-10-28,陶永亮，整理
 */
 
td_s32 GetIpFilter(td_s32* _piType, td_s32* _piNum, td_u32* _puiList);
/*
 * 函数名称:GetIpFilter
 * 简要描述:获取IP黑名单/白名单
 * 输	 入:
 *			_piType,传出IP_FILTER_FORBID,黑名单/IP_FILTER_ALLOW,白名单/IP_FILTER_NONE,不启用
 *			_piNum,传出名单长度(IP数)
 *			_puiList,传出名单
 * 输	 出:
 *			-1,参数非法
 *			0,成功
 * 修改日志:
 *			2008-10-28,陶永亮，整理
 */

int  GetConUserNum();
/*
 * 函数名称:GetConUserNum
 * 简要描述:获取已登录的客户端数目
 * 输	 入:
 *			无
 * 输	 出:
 *			已登录的客户端数
 * 修改日志:
 *			2010-02-03,陶永亮，整理
 */
 
unsigned int GetConUserIp(int _iUserNo);
/*
 * 函数名称:GetConUserIp
 * 简要描述:获取已登录的客户端IP地址
 * 输	 入:
 *			_iUserNo, 已登录的用户编号, 0~MAX_CLIENT
 * 输	 出:
 *			已登录客户端的IP
 * 修改日志:
 *			2010-02-03,陶永亮，整理
 */
 
void* GetConUserPoint(int _iUserNo);
/*
 * 函数名称:GetConUserPoint
 * 简要描述:获取已登录的客户端socket
 * 输	 入:
 *			_iUserNo, 已登录的用户编号, 0~MAX_CLIENT
 * 输	 出:
 *			已登录客户端的socket(PCONNECT)
 * 修改日志:
 *			2010-02-03,陶永亮，整理
 */



int user_JudgeRight(unsigned int _iSocket, int _iOperate, unsigned int _u32ChnlBit);
int user_GetRight(IN char* _strUserName, int iAuthority, unsigned int *_pu32ChnlBit);
int user_SetRight(char* _strUserName, int _iAuthority, unsigned int _u32ChnlBit);

td_s32 GetUserName(td_s32 _s32UserNo, char cUserName[LENGTH_USER_NAME]);
int GetUserRight(char *_pcUserName, int *_iAuthority); 
//zhangshengusr
TIPFilter_ex *GetIPFilter();
NetUser *GetNetUser();

td_s32 GetUserInfoOfWritingLog(int _socket, char *_pcstrUserName, int _iMaxUserNameLen);

td_s32 GetUserKeyIp(int _socket, char _ckey[USER_KEY_LEN], unsigned int *_Ip);
#define GetUserKey(_socket, _ckey)	GetUserKeyIp(_socket, _ckey, NULL)
td_s32 GetUserKeyStat(int _socket, char _ckey[USER_KEY_LEN]);
int userCreatekey(td_u32 _u32IP, OUT char _cKey[USER_KEY_LEN]);
int UserSetLogonDecodeCallBack(LogonDecodeCallBack _func);

td_s32 VerifyUserInfo(td_char* _cUserName, td_char* _cPassWord);
void *GetConFormSocket(int _socket);

#endif /*_USER_H_*/
