/********************************************************************
Copyright (C), 1994-2009, 天地伟业数码科技有限公司
录像文件支持hash校验功能程序
FileName: libhash.c
Description: hash校验功能基础函数
History: 

*******************************************************************/
#ifndef _LIBHASH_
#define _LIBHASH_

#define MAX_CALC_FILE_LIST 64
#define MD5_FILE_EXTNAME ".md5"
#define MD5_VALUE_LEN 16

typedef enum
{
	MD5MODE = 1, //md5
	OTHREMODE, //其他方式	
}HashMode;


typedef enum
{
	CalcWait = 0, //未计算
	CalcFinished, //计算完成	
}CalcFileStatus;

typedef struct
{
	td_u8   	m_ucFileName[FULL_PATH_LEN];		//文件名(绝对路径)
}TCalcFileInfo;

typedef struct
{
	TCalcFileInfo   stFileInfo; //文件信息
	CalcFileStatus 	Fileinfo; //计算状态
}hash_FileStat;

typedef struct t_Calc_File_List
{
	hash_FileStat    Rec_File[MAX_CALC_FILE_LIST];  //64个刻录文件列表
	int 			m_iRead;		//读标志
	int 			m_iWrite;  		//写标志
	pthread_mutex_t m_ListMutex;	//锁
}T_CALC_FILE_LIST;//当前的刻录队列


/*
函数名 	:	td_s32 Hash_Init(td_s32 _iEnable)
功能	:	初始化hash模块
输入参数	:	_ iEnable	: 0 不使能 1 使能
返回值	: 	>=0 成功   -1 失败
*/
td_s32 Hash_Init(td_s32 _iEnable);
/*
函数名 	:	td_s32 Hash_UnInit(void)
功能	:	反初始化hash模块
输入参数	:	无
返回值	: 	>=0 成功   -1 失败
*/
td_s32 Hash_UnInit(void);
/*
函数名 	:	td_s32 Hash_SetHashMode (int _iHashMode)
功能	:	设置哈希校验算法类型
输入参数	:	_iHashMode  : 算法类型 1-md5 
返回值	: 	>=0 成功   -1 失败
*/
td_s32 Hash_SetHashMode (int _iHashMode);
/*
函数名 	:	td_s32 Hash_DeleteHashFile (const td_s8* _pcRecFileName)
功能	:	删除录像文件对应的哈希文件
输入参数	:	_pcRecFileName: 录像文件名
返回值	: 	>=0 成功   -1 失败
*/
td_s32 Hash_DeleteHashFile (const td_s8* _pcRecFileName);
/*
函数名 	:	td_s32 Hash_ReadHashValue (const td_s8* _pcRecFileName, td_s8* _pcHashValue)
功能	:	获取哈希文件中的哈希值
输入参数	:	_pcRecFileName: 录像文件名
输出参数: 	_pcHashValue: 哈希值
返回值	: 	>=0 成功   -1 失败
*/
td_s32 Hash_ReadHashValue (const td_s8* _pcRecFileName, td_s8* _pcHashValue);
/*
函数名 	:	td_s32 Hash_AddToHashFileList (const td_s8* _pcRecFileName)
功能	:	将打包的录像文件存入待计算的哈希队列
输入参数	:	_pcRecFileName: 录像文件名
返回值	: 	>=0 成功   -1 失败
*/
td_s32 Hash_AddToHashFileList (const td_s8* _pcRecFileName);
/*
函数名 	:	td_s32 Hash_CalcHashFile (const td_s8* _pcRecFileName, td_s8* _pcHashValue)
功能	:	立即获取文件的哈希值
输入参数	:	_pcRecFileName: 录像文件名
输出参数：	_pcHashValue: 哈希值
返回值	: 	>=0 成功   -1 失败
*/
td_s32 Hash_CalcHashFile (const td_s8* _pcRecFileName, td_s8* _pcHashValue);
/*
函数名 	:	td_s32 Hash_AddToHashBigFileList (const td_s8* _pcRecFileName)
功能	:	将打包的录像文件存入待计算的哈希队列
输入参数	:	_pcRecFileName: 录像文件名
返回值	: 	>=0 成功   -1 失败
*/
td_s32 Hash_AddToHashBigFileList (const td_s8* _pcRecFileName);
/*
函数名 	:	td_s32 Hash_RenameHashFile (const td_s8* _pcSrcRecFileName，const td_s8* _pcNewRecFileName)
功能	:	修改录像文件名同时修改哈希文件名称
输入参数	:	_pcSrcRecFileName: 原录像文件名
_pcNewRecFileName: 新的录像文件名
返回值	: 	>=0 成功   -1 失败
*/
td_s32 Hash_CalcHashBigFile (const td_s8* _pcRecFileName);
/*
函数名 	:	td_s32 Hash_CalcHashBigFile (const td_s8* _pcRecFileName)
功能	:	计算某个文件的md5值
输入参数	:	_pcRecFileName: 录像文件名
返回值	: 	>=0 成功   -1 失败
*/
td_s32 hash_WriteHashFile(td_s8* _pcHashFullPath, td_s8* _pcHashValue);
td_s32 hash_GenerateHashFileFullpath(td_s8* _pcTempName, td_s8* _pcHashFullPath);
td_s32 Hash_Md5BigFileInit(int _iCdNo);
/*
函数名 	:	td_s32 Hash_Md5BigFileInit(int _iCdNo)
功能	:	刻录大文件时,hash初始化
输入参数	:	_iCdNo: 光驱号
返回值	: 	>=0 成功   -1 失败
*/
td_s32 Hash_Md5Update(int _iCdNo,unsigned char *_pcBuffer,unsigned int iBufferLen);
/*
函数名 	:	td_s32 Hash_Md5Update(int _iCdNo,unsigned char *_pcBuffer,unsigned int iBufferLen)
功能	:	把流送进去然后更新md5
输入参数	:	_iCdNo :光驱号 _pcBuffer: 视频流  iBufferLen:视频流长度

返回值	: 	>=0 成功   -1 失败
*/
td_s32 Hash_GetMd5Info(int _iCdNo,char *_pstMd5Info);
td_s32 Hash_SetMd5Info(int _iCdNo,char *_pstMd5Info);
td_s32 Hash_Md5BigFileFinal(int _iCdNo,char *_cOutputBuf);
/*
函数名 	:	Hash_Md5BigFileFinal
功能	:	产生md5值
输入参数	:	_iCdNo :光驱号 _cOutputBuf: md5值

返回值	: 	>=0 成功   -1 失败
*/
td_s32 Hash_UpdateHashFile (const td_s8* _pcRecFileName);
/*
函数名 	:	Hash_UpdateHashFile
功能	:	根据录像文件名更新md5文件。
输入参数	:	_pcRecFileName: 录像文件名

返回值	: 	>=0 成功   -1 失败
*/
td_s32 Hash_RenameHashFile (const td_s8* _pcSrcRecFileName, const td_s8* _pcNewRecFileName);
/*
函数名 	:	td_s32 Hash_BackupHashFile (const td_s8* _pcSrcRecFileName，const td_s8* _pcDstRecFileName)
功能	:	录像备份时候同时备份哈希文件
输入参数	:	_pcSrcRecFileName: 源录像文件名
_pcDstRecFileName: 备份后的录像文件名
返回值	: 	>=0 成功   -1 失败
*/
td_s32 Hash_BackupHashFile (const td_s8* _pcSrcRecFileName, const td_s8* _pcDstRecFileName);
/*
函数名 	:	td_s32 Hash_GetHashEnble (void)
功能	:	获取哈希功能使能状态
输入参数	:	无
返回值	: 	哈希使能状态 0 不使能 1 使能
*/
td_s32 Hash_iGetHashEnble (void);
/*
函数名 	:	td_s32 Hash_iSetHashEnble (td_S32 _iEnable)
功能	:	设置哈希功能使能状态
输入参数	:	_iEnable：哈希功能使能状态 0 不使能 1 使能
返回值	: 	>=0 成功   -1 失败
*/
td_s32 Hash_iSetHashEnble (td_s32 _iEnable);

/*
函数名 	:	td_s32 hach_CheckHashFileExist(td_s8* _pcTempName, td_s8* _pcHashFullPath);
功能	:	获取MD5文件路径
输入参数	:	_pcTempName：录像文件名 _pcHashFullPath: md5文件路径
返回值	: 	>=0 成功   -1 失败
*/

td_s32 hach_CheckHashFileExist(td_s8* _pcTempName, td_s8* _pcHashFullPath);
td_s32 Hash_GetHashBigFileList(void);
/*
函数名 	:	td_s32 Hash_GetHashBigFileList();
功能	:	获取MD5是否转换完成
输入参数:   无
返回值	: 	1 完成   0 未完成
*/
#endif /*_LIBTIANDY_*/
