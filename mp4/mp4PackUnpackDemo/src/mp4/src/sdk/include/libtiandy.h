#ifndef _LIBTIANDY_
#define _LIBTIANDY_

#include <pthread.h>
#include "common.h"



/******************************net********************************************/
unsigned short int GetRandomPort(unsigned short int miniseed);
/*
 * 函数名称:GetRandomPort
 * 简要描述:获取一个随机端口
 * 输	 入:
 *			miniseed，指定端口号的下限，即获取到的端口必须在这个数以上
 * 输	 出:
 *			获取到的随机端口
 * 修改日志:
 *			2009-10-26,陶永亮，整理。
 */
 
int GetIPbyDNS(char* _domainName, char *_cIP);
/*
 * 函数名称:GetIPbyDNS
 * 简要描述: 将一个域名通过DNS转换为IP地址
 * 输	 入:
 *			_domainName，待转换的域名
 *			_cIP，点分发表示的目标IP地址
 * 输	 出:
 *			0，成功
 *			-1，失败
 * 修改日志:
 *			2009-10-26,陶永亮，整理。
 */
int get_local_address(char* szIPAddr,char* szNetMask, char * _cDevName);

int IPAddressU32ToChar(unsigned int _u32IPAddress,char* cIpAddress);
/*
 * 函数名称:IPAddressU32ToChar
 * 简要描述:将一个整型表示的IP地址转换为点分发表示的IP
 * 输	 入:
 *			_u32IPAddress，待转换的整型IP
 *			cIpAddress，转换后的点分法IP
 * 输	 出:
 *			0，成功
 *			-1，失败
 * 修改日志:
 *			2009-10-26,陶永亮，整理。
 */

unsigned int ipstring2u32(char* _ip);
/*
 * 函数名称:ipstring2u32
 * 简要描述:将一个点分法表示的IP地址转换为32位无符号整数
 * 输	 入:
 *			_ip，非空，待转换IP
 * 输	 出:
 *			转换后的无符号整数
 * 修改日志:
 *			2009-10-26,陶永亮，整理。
 */
void ReverseMAC(char *_cMac);

int IsValidIP(const char* _cIP);
/*
 * 函数名称:IsValidIP
 * 简要描述:判断一个字符串是否为合法IP地址
 * 输	 入:
 *			_cIP，非空，待判断IP
 * 输	 出:
 *			-1,非法IP地址
 *			1，标准IP地址
 *			2，广播IP地址
 *			3，空IP地址(全0)
 * 修改日志:
 *			2009-10-26,陶永亮，整理。
 */

//zhangweiwei 0412
//int CheckIp(int _iEth, char *_strIp, int _iTimeOut);
int CheckIp(int _iEth, char *_strIp, char *_mac, int _iTimeOut, int _iReciveCnt);
/*
 * 函数名称:CheckIp
 * 简要描述:检测IP地址冲突
 * 输	 入:
 			_iEth    device
 *			_strIp，非空，ip地址，如10.10.10.61
 *			_iTimeOut，超时时间，秒，>= 1
 * 输	 出:
 *			-1，发生错误
 *			0，IP地址未被占用
 *			1，IP地址被占用
 * 修改日志:
 *			2009-10-26,陶永亮，整理。
 */
int CheckMac(char *_cstrIp,char *_cInterfaceName,char *_cdstIp, int _iTimeOut);
	/*
	 * 函数名称:CheckMac
	 * 简要描述:检测MAC地址冲突
	 * 输	 入:
				_cstrIp	 设备ip地址
	 *			_cInterfaceName，网卡名称
				 _cdstIp         MAC 冲突对应的ip地址
	 *			_iTimeOut，超时时间，秒，>= 1
	 * 输	 出:
	 *			-1，发生错误
	 *			0， MAC 地址未被占用
	 *			1， MAC 地址被占用
	 * 修改日志:
	 *			2013-10-26,zhb，修改。
	 */

int CheckNetDelay(int _iEth,char *_cIp,int *_pidelay,int *_piLostrate,int  _icount, int _itimeout);

int DetectEth0(int _iEth);
/*
 * 函数名称:DetectEth0
 * 简要描述:检测有线网络连接是否中断
 * 输	 入:
 *			0: eth0
                   1: eth1
 * 输	 出:
 *			1，连接中断
 *			0，连接正常
 *			其他，状态未知
 * 修改日志:
 *			2009-10-26,陶永亮，整理。
 *			2010-02-03,杜秀龙，向Hi3520切换时改用新的方法实现
 */

int DetectEth0Ex(void);
/*
 * 函数名称:DetectEth0Ex
 * 简要描述:检测有线网络连接是否中断,向Hi3520切换时改用新的方法实现
 * 输	 入:
 *			无
 * 输	 出:
 *			1，连接正常
 *			0，连接中断
 *			其他，状态未知
 * 修改日志:
 *			2010-02-03,杜秀龙，创建
 */

int get_gateway(char *gateway,char* dev);
/*
 * 函数名称:get_gateway
 * 简要描述:获取网关
 * 输	 入:
 *			gateway，非空，传出获取到的网关
 *			dev，设备名称，如有线网卡为"eth0"
 * 输	 出:
 *			-1，失败
 *			0，成功
 * 修改日志:
 *			2009-10-26,陶永亮，整理。
 */
 
/******************************string*****************************************/
/*----------------------------------------------------------------------------*/
typedef struct 
{
    int      m_iStringCount;
    char**   m_cStrings;

    int (*Add)(void* _strThis,char* _cBuf);
    int (*Clear)(void* _strThis);
    int (*Insert)(void* _strThis,char* _cBuf,int _iIndex);
    int (*Delete)(void* _strThis,int _iIndex);
    int (*Init)(void* _strThis);
    int (*Destroy)(void* _strThis);
    int (*IndexOf)(void* _strThis,char* _cBuf);
}StringList;

/*----------------------------------------------------------------------------*/
int sList_Add(void* _strThis,char* _cBuf);
int sList_Clear(void* _strThis);
int sList_Insert(void* _strThis,char* _cBuf,int _iIndex);
int sList_Delete(void* _strThis,int _iIndex);
int sList_Init(void* _strThis);
int sList_IndexOf(void* _strThis,char* _cBuf);
int sList_Destroy(void* _strThis);

#ifndef FOR_MP4SDK
void itoa(unsigned int value, char *string, int radix);
/*
 * 函数名称:itoa
 * 简要描述:将一个整型数转换为字符串，如456->"456"
 * 输	 入:
 *			value，源整型数
 *			string，非空，目标字符串
 *			radix，进制，目前只支持十进制
 * 输	 出:
 *			无
 * 修改日志:
 *			2009-10-26,陶永亮，整理。
 */
 #endif
 
int OnCharLeft(const char* c1,const char* c2,char* c3);
/*
 * 函数名称:OnCharLeft
 * 简要描述:截取源字符串c1中分隔字符串c2左边的部分，并拷贝至目标字符串c3
 * 输	 入:
 *			c1，非空，源字符串
 *			c2，非空，分隔字符串
 *			c3，非空，目标字符串
 * 输	 出:
 *			0，成功
 *			-1，失败
 * 修改日志:
 *			2009-10-26,陶永亮，整理。
 */
 
int OnCharRight(const char* c1,const char* c2,char* c3);
/*
 * 函数名称:OnCharRight
 * 简要描述:截取源字符串c1中分隔字符串c2右边的部分，并拷贝至目标字符串c3
 * 输	 入:
 *			c1，非空，源字符串
 *			c2，非空，分隔字符串
 *			c3，非空，目标字符串
 * 输	 出:
 *			0，成功
 *			-1，失败
 * 修改日志:
 *			2009-10-26,陶永亮，整理。
 */
 
int SplitStringToArrayEx(char* cSource,char* cSplitChar,char** cDest, int len);
int SplitStringToArray(char* cSource,char* cSplitChar,char** cDest);
/*
 * 函数名称:SplitStringToArray
 * 简要描述:将一个字符串按照一定的分隔符分割为字符串数组，有一定风险，不推荐使用
 * 输	 入:
 *			cSource，源字符串
 *			cSplitChar，分隔符
 *			cDest，目标字符串数组
 * 输	 出:
 *			目标字符串个数
 * 修改日志:
 *			2009-10-26,陶永亮，整理。
 */
 
int SplitString(unsigned char* _pcSrc, unsigned char* _pcSplit, unsigned char** _ppcDst, int _iFieldLen, int _iFieldNum);
/*
 * 函数名称:SplitString
 * 简要描述:功能同上一个函数:将一个字符串按照一定的分隔符分割为字符串数组，但比上一个函数安全
 * 输	 入:
 *			_pcSrc，源字符串
 *			_pcSplit，分隔符
 *			_ppcDst，目标字符串数组
 *			_iFieldLen，目标字符串的长度上限
 *			_iFieldNum，目标字符串数组的字符串数上限
 * 输	 出:
 *			-1，指针参数非空
 *			-2，字符串长度超过上限
 *			-3，字符串个数超过上限
 *			其他，目标字符串个数
 * 修改日志:
 *			2009-10-26,陶永亮，整理。
 */
 #if 0
int decimal2binary(int decimal, int* binary);
 #else
int decimal2binary(unsigned long long decimal, int* binary);
#endif
/*
 * 函数名称:decimal2binary
 * 简要描述:将一个整型数转换为二进制数组，如12->1100
 * 输	 入:
 *			decimal，待转换的整型数
 *			binary，传出转换后的二进制数组
 * 输	 出:
 *			-1，待转换的整数小于0
 *			-2，用于传出的二进制数组为空
 *			其他，转换后二进制数组的成员个数
 * 修改日志:
 *			2009-10-26,陶永亮，整理。
 */
#if 0
int binary2decimal(int nbits, int* binary);
#else
unsigned long long binary2decimal(int nbits, int* binary);
#endif
/*
 * 函数名称:binary2decimal
 * 简要描述:将一个二进制数组转换为一个十进制数，如1100->12
 * 输	 入:
 *          nbits，二进制数组的成员个数
 *			binary，非空，待转换的二进制数组
 * 输	 出:
 *			-1，转换失败
 *			其他，转换后的十进制数
 * 修改日志:
 *			2009-10-26,陶永亮，整理。
 */
int binary2decimalEx(int nbits, int* binary, unsigned int decimal[]);//wangsong16D1
int Netbinary2decimalEx(int nbits, int* binary, unsigned int decimal[]);
int decimal2binaryEx(unsigned int decimal[], int* binary, int nbits);//wangsong16D1

int check_string(char* _string);
/*
 * 函数名称:check_string
 * 简要描述:检测一个字符串是否为合法的字符串(只包含字母和数字，如Admin123)
 * 输	 入:
 *          _string，非空，待检测的字符串
 * 输	 出:
 *			1，是
 *			0，不是
 * 修改日志:
 *			2009-10-26,陶永亮，整理。
 */
 
/******************************file*******************************************/
int remove_file(const char *path);
/*
 * 函数名称:remove_file
 * 简要描述:删除一个文件或目录，可以递归删除
 * 输	 入:
 *          path，非空，待删除的文件或目录
 * 输	 出:
 *			0，成功
 *			-1，失败
 * 修改日志:
 *			2009-10-26,陶永亮，整理。
 */
 
int copy_file(const char* src,const char* des, int file_size);
/*
 * 函数名称:copy_file
 * 简要描述:拷贝文件，只能拷贝单个文件，不能递归拷贝
 * 输	 入:
 *          src，非空，源文件
 *			des，非空，目标文件
 *			file_size，需要拷贝的字节数；如果小于0，表示拷贝整个文件
 * 输	 出:
 *			0，拷贝成功
 *			-1，打开源文件失败
 *			-2，打开目标文件失败
 *			-3，写文件失败
 *			-4，读文件失败
 *			-5，申请内存失败
 *			-6，修改源文件为缓存模式失败
 *			-7，修改目标文件为缓存模式失败
 *			-8，获取源文件信息失败
 * 修改日志:
 *			2009-10-26,陶永亮，整理。
 */

int is_dir(const unsigned char* _pucPath);
/*
 * 函数名称:is_dir
 * 简要描述:判断一个字符串是否一个合法的目录
 * 输	 入:
 *          _pucPath，非空，待判断的字符串
 * 输	 出:
 *			1，是
 *			0，不是
 * 修改日志:
 *			2009-10-26,陶永亮，整理。
 */

int is_empty_dir(const signed char* _cst_pcPath);
/*
 * 函数名称:is_empty_dir
 * 简要描述:判断一个目录是否为空，如果该目录及其所有子目录均为空则认为其是空目录
 * 输	 入:
 *          _cst_pcPath，非空，待判断的目录
 * 输	 出:
 *			1，空
 *			0，非空
 * 修改日志:
 *			2009-10-26,陶永亮，整理。
 */

int extract_file_name(unsigned char* _pucFullPath);
/*
 * 函数名称:extract_file_name
 * 简要描述:从全路径中提取文件名
 * 输	 入:
 *          _pucFullPath，非空，全路径，提取文件名的结果仍从该参数返回
 * 输	 出:
 *			0，成功
 *			-1，失败
 * 修改日志:
 *			2009-10-26,陶永亮，整理。
 */

int extract_file_path(unsigned char* _pucFilePath);
/*
 * 函数名称:extract_file_path
 * 简要描述:从全路径中提取文件所在目录
 * 输	 入:
 *          _pucFilePath，非空，全路径，提取路径的结果仍从该参数返回
 * 输	 出:
 *			0，成功
 *			-1，失败
 * 修改日志:
 *			2009-10-26,陶永亮，整理。
 */
 
int make_dir(unsigned char* _pcDirName);
/*
 * 函数名称:make_dir
 * 简要描述:创建目录
 * 输	 入:
 *          _pcDirName，非空，待创建目录的全路径
 * 输	 出:
 *			0，成功
 *			-1，失败
 * 修改日志:
 *			2009-10-26,陶永亮，整理。
 */


enum
{
	DU_FLAG_FOLLOW_LINK = (1 << 0),	//跟踪软链接，计算软链接所指向文件的大小
	DU_FLAG_B_BYTES = (1 << 8),	//返回值以字节为单位
	DU_FLAG_K_BYTES = (1 << 9),	//返回值以KB为单位
	DU_FLAG_M_BYTES = (1 << 10),	//返回值以MB为单位
	DU_FLAG_G_BYTES = (1 << 11),	//返回值以GB为单位
};
unsigned long long du(const char *_pcFileName, int _iFlag);
/*
 * 函数名称:du
 * 简要描述:统计一个目录/文件的总大小
 * 输	 入:
 *          _pcFileName，非空，文件或目录
 *		_iFlag，可以是上述枚举的组合，如DU_FLAG_FOLLOW_LINK | DU_FLAG_M_BYTES
 * 输	 出:
 *			文件/目录的总大小
 * 修改日志:
 *			2014-07-24,陶永亮，实现。
 */


/******************************math*******************************************/
void to_base64 (unsigned char *out, const unsigned char *in, int len);
/*
 * 函数名称:to_base64
 * 简要描述:raw bytes to null-terminated base 64 string
 * 输	 入:
 *          out，编码结果
 *			in，源数据
 *			len，源数据长度
 * 输	 出:
 *			无
 * 修改日志:
 *			2009-10-26,陶永亮，整理。
 */

int EncodeBase64(char* Result,char* Source);
/*
 * 函数名称:EncodeBase64
 * 简要描述:Base64编码算法
 * 输	 入:
 *          Result，非空，编码结果
 *			Source，非空，源数据
 * 输	 出:
 *			0，成功
 *			-1,失败
 * 修改日志:
 *			2009-10-26,陶永亮，整理。
 */

typedef unsigned int Uint32;
#define SHA1HashSize 20

/*
 *  This structure will hold context information for the SHA-1
 *  hashing operation
 */
typedef struct SHA1Context
{
    Uint32 Intermediate_Hash[SHA1HashSize/4]; /* Message Digest  */

    Uint32 Length_Low;            /* Message length in bits      */
    Uint32 Length_High;           /* Message length in bits      */

                               /* Index into message block array   */
    Uint32 Message_Block_Index;
    unsigned char Message_Block[64];      /* 512-bit message blocks      */

    int Computed;               /* Is the digest computed?         */
    int Corrupted;             /* Is the message digest corrupted? */
} SHA1Context;

int SHA1Reset(  SHA1Context * p);
/*
 * 函数名称:SHA1Reset
 * 简要描述:SHA-1语法结构复位
 * 输	 入:
 *          p，SHA-1算法语法结构
 * 输	 出:
 *			0，成功
 *			1，参数为空
 * 修改日志:
 *			2009-10-26,陶永亮，整理。
 */
 
int SHA1Input(  SHA1Context * p, const unsigned char * data, unsigned int len);
/*
 * 函数名称:SHA1Input
 * 简要描述:输入需要SHA-1编码的数据
 * 输	 入:
 *          p，SHA-1算法语法结构
 *			data，待编码数据
 *			len，编码数据长度
 * 输	 出:
 *			0，成功
 *			1，参数非法
 *			3，在result之后调用了input
 * 修改日志:
 *			2009-10-26,陶永亮，整理。
 */
 
int SHA1Result( SHA1Context * p, const unsigned char Message_Digest[SHA1HashSize]);
/*
 * 函数名称:SHA1Result
 * 简要描述:输出经SHA-1编码的结果
 * 输	 入:
 *          p，SHA-1算法语法结构
 *			Message_Digest，输出结果
 * 输	 出:
 *			0，成功
 *			1，参数为空
 * 修改日志:
 *			2009-10-26,陶永亮，整理。
 */

int pow1(int a, int _iMax);
/*
 * 函数名称:pow1
 * 简要描述:求底数的n次幂
 * 输	 入:
 *          a，底数
 *			_iMax，次幂
 * 输	 出:
 *			a的_iMax次幂
 * 修改日志:
 *			2009-10-26,陶永亮，整理。
 */
 
/******************************system*****************************************/

void set_uptime(unsigned long _msec);
int get_uptime(int* _msec);
time_t getmytime(time_t *_T);
void setmytime(unsigned long _uLTm);
//#define time(n)	getmytime(n)

/*
 * 函数名称:get_uptime
 * 简要描述:获取系统启动后的绝对时间
 * 输	 入:
 *          _msec，可以为空，如果需要精确到毫秒，可以通过该参数传出
 * 输	 出:
 *			返回启动自启动至现在流逝的秒数
 * 修改日志:
 *			2009-10-26,陶永亮，整理。
 */

#ifndef FOR_MP4SDK
void GetSystemInfo(double *_pdCPUuse, double *_pdMEMuse, double *_pdFlashUse);
/*
 * 函数名称:GetSystemInfo
 * 简要描述:获取系统信息
 * 输	 入:
 *          _pdCPUuse，非空，用于传出CPU使用率
 *			_pdMEMuse，非空，用于传出内存使用率
 *			_pdFlashUse，非空，用于传出FLASH使用率
 * 输	 出:
 *			无
 * 修改日志:
 *			2009-10-26,陶永亮，整理。
 */
#endif

/******************************timer******************************************/
struct timer_list{
	unsigned long expires;
	void (*fuc)(unsigned long);
	unsigned long data;
};

int init_timer(struct timer_list* timer);
/*
 * 函数名称:init_timer
 * 简要描述:初始化定时器
 * 输	 入:
 *          timer，非空，在定时器链表中添加的第一个定时器
 * 输	 出:
 *			0，成功
 *			-1，参数非法
 *			-2，链表已存在
 *			-3，创建线程失败
 * 修改日志:
 *			2009-10-26,陶永亮，整理。
 */

int add_timer(struct timer_list* timer);
/*
 * 函数名称:add_timer
 * 简要描述:添加一个新的定时器
 * 输	 入:
 *          timer，非空，待添加的定时器
 * 输	 出:
 *			0，成功
 *			-1，参数非法
 *			-2，链表不存在
 * 修改日志:
 *			2009-10-26,陶永亮，整理。
 */

int	del_timer(struct timer_list* timer);
/*
 * 函数名称:del_timer
 * 简要描述:删除一个定时器
 * 输	 入:
 *          timer，非空，待删除的定时器
 * 输	 出:
 *			0，成功
 *			-1，参数非法
 *			-2，链表不存在
 * 修改日志:
 *			2009-10-26,陶永亮，整理。
 */

struct timer_list* search_timer(struct timer_list* timer);
/*
 * 函数名称:search_timer
 * 简要描述:在定时器链表中查找一个特定的定时器
 * 输	 入:
 *          timer，非空，待查找的定时器
 * 输	 出:
 *			NULL，查找失败
 *			非空，查找成功
 * 修改日志:
 *			2009-10-26,陶永亮，整理。
 */

/******************************thread*****************************************/
int CreateThreadInPool(pthread_t *_ptThr, void *(*_Task)(void *), void *_pArg);
/*
 * 函数名称:CreateThreadInPool
 * 简要描述:在线程池中创建线程
 * 输	 入:
 *          _ptThr，非空，用于传出线程号
 *			_Task，非空，线程任务
 *			_pArg，可以为空，线程任务参数
 * 输	 出:
 *			0，成功
 *			<0，失败
 * 修改日志:
 *			2009-10-26,陶永亮，整理。
 */

int GetIPbyDNS_Ex(char * _domainName, char _cIP [ 16 ]);			//陆翔云 20110303 主动模式需要使用该函数
unsigned short cal_chksum(unsigned short *addr, int len);

long long get_uptime_ms (void);
/*
*函数名称:get_uptime_ms
*简要描述:获取系统运行时间，精度为毫秒
*输出:返回系统自启动运行的毫米数，-1表示失败
*修改日志:
*               2012-11-06，王松创建。    
*/
int SetDeletelFileSize(int _iS);//设置删除文件单元大小
int myunlink(const char *_pcfilepath);//封装删除函数

int td_strnchr(char *_strSrc, int _iLen, char _c, char *_strDes[], int *_pistrLen, int _n);

int strtoken_r(char *str, char *delim, char ** array, int kwSect);
/*
 * 函数名称:strtoken_r
 * 简要描述:将字符串按照分隔符分配到字符指针数组
 * 输	 入:
 *			str，需要被解析的源字符串
 *			delim，分隔符
 *			array, 目标指针数组
 *			kwSect, 指针数组的维数
 * 输	 出:
 *			无
 * 修改日志:
 *			2010-01-28,郭伟，整理。
 */
/******************************debug******************************************/
#ifdef FOR_MP4SDK
void PrtDbgInfo(int level,char *fmt, ...);

#endif
#ifdef TD_DEBUG
#define TD_RETURN(exp,ret,arg...)  \
	do{	\
		if (exp) {\
		td_printf(0XFF0000,"\nTD_RETURN{" #exp "}\r\n");\
		printf(" >Error   : 0x%08x\r\n", ret);\
		printf(" >File    : %s\r\n >In Func : %s\r\n >Line    : %d\r\n >ERR MSG : \r\n", __FILE__, __FUNCTION__, __LINE__);\
		}\
        }while(0)
#ifndef ALONE
int	td_printf(unsigned int handle, char* pszfmt, ...);
/*
 * 函数名称:td_printf
 * 简要描述:格式化打印字符串，添加了时间信息，并且时间信息可以选取不同的颜色
 * 输	 入:
 *			handle，打印类型:0，黄色;0xFF0000,红色;0x12345678,青色;其他，打印到文件printf.txt
 *			pszfmt，非空，格式串
 *			...，可变参数列表
 * 输	 出:
 *			已打印的字符数
 * 修改日志:
 *			2009-10-26,陶永亮，整理。
 */
#else
#define td_printf td_printf_alone
#endif

void print_hex(unsigned char* _string, int _len);
/*
 * 函数名称:print_hex
 * 简要描述:按照十六进制数打印字符数组
 * 输	 入:
 *			_string，待打印的字符数组
 *			_len，字符数组成员个数
 * 输	 出:
 *			无
 * 修改日志:
 *			2009-10-26,陶永亮，整理。
 */

int strtoken_r(char *str, char *delim, char ** array, int kwSect);
/*
 * 函数名称:strtoken_r
 * 简要描述:将字符串按照分隔符分配到字符指针数组
 * 输	 入:
 *			str，需要被解析的源字符串
 *			delim，分隔符
 *			array, 目标指针数组
 *			kwSect, 指针数组的维数
 * 输	 出:
 *			无
 * 修改日志:
 *			2010-01-28,郭伟，整理。
 */

//去掉ip地址每个分段的前导0, wzy 20140730
int ChangeIpAddr(char *_strIp);

#else
	#ifndef FOR_MP4SDK
		#define TD_RETURN(exp,ret,arg...)
	#else
		#define TD_MP4LOG
	#endif
	
	#ifdef TD_MP4LOG
		#ifdef FOR_MP4SDK
			#define td_printf PrtDbgInfo
		#else
			#define td_printf td_printf_mp4log
		#endif
	#else
		#define td_printf(a,b,...) 
	#endif
	#define print_hex(_string,_len)
#endif

#endif /*_LIBTIANDY_*/
