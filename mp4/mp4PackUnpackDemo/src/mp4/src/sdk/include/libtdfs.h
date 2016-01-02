#ifndef __LIBTDFS_H__
#define __LIBTDFS_H__

#include <sys/stat.h> 
#include <sys/statfs.h> 
#include <sys/types.h>
#include <unistd.h>

#include "public_def.h"

#define TDFS_KEY_PATH  "/dvr/tdfs"

enum
{
	FILEUNLOCK,
	FILELOCK,
	FILEBACK,
};

typedef enum
{
	FS_TD,
	FS_NORMAl,
}TFsType;

typedef enum
{
	TD_FILE_TYPE_REC = 0,	//录像文件
	TD_FILE_TYPE_PRO,		//预录文件
	TD_FILE_TYPE_BACKUP,    //备份录像文件
	TD_FILE_TYPE_PIC,		//图片文件
}TTdFileType;

#define FILE_REV_ATTR_LEN 32

typedef struct
{
	struct stat m_stat;
	td_u64		m_iRecChn;
	td_u64 		m_iRecLock;
	td_u64 		m_iRecType;
	td_u64		m_lRecStartTm;
	td_char		m_iOthAttribute[FILE_REV_ATTR_LEN];			//自定义属性	
	char		m_cFileName[FULL_PATH_LEN];		//文件名
}TTdStat;

typedef struct
{
	struct statfs m_statfs;
}TTdStatfs;

typedef struct
{
	int 		m_iChn;							//通道号	
	int			m_iTime;						//录像文件的开始时间		
	TTdFileType m_eType;						//录像文件类型，录像/预录/图片
	int			m_iPreSize;						//多种用途:预录文件大小；备份文件大小;单位byte
	int			m_iParam;						//录像文件类型(手动、定时、报警、......)
	int  		m_iCustomType;					//自定义类型或者毫秒值
	char		m_cBlockDevPath[FULL_PATH_LEN];	//块设备路径
	char        m_cExtName[8];
}TTFileParam;

typedef struct
{
	int 		m_iId;					//块编号
	td_u64 		m_lStartTime;		//块的起始时间
	int 		m_iFileNum;			//块内文件总数
}TBlockInfo;


/*======
需在外部实现的两个函数
1、
	int getDiskType(char* _cBlockDevPath)
	//获取磁盘文件系统的类型
	
2、	int CopyFile(char* _cSrcPath,char* _cDstPath)
	//将td文件系统中的文件拷贝到通用文件系统中
======*/


int tdfs_Init();
/*
	功	能	:	初始化文件系统。
	参	数	:

	返	回	:	td_success	成功
				其他		失败
*/

int tdfs_DeInit();
/*
	功	能	:	关闭文件系统
	参	数	:	无
	返	回	:	td_success	成功
				其他		失败	
*/

int tdfs_Format(char* _cBlockDevPath);
/*
	功	能	:	格式化磁盘
	参	数	:	_cBlockDevPath	块设备路径
	返	回	:	td_success		成功
				其他			失败	
*/

int tdfs_Statfs(char* _cBlockDevPath,TTdStatfs *_pBuf);
/*
	功	能	:	获取磁盘空间信息
	参	数	:	_cBlockDevPath	块设备路径
				_pBuf			保存磁盘空间信息的指针
	返	回	:	td_success		成功
				其他			失败	
*/

int tdfs_GetUsedBlockNum(char* _cPath);
/*	
	功	能	:	获取磁盘内已用的块数
	参	数	:	_cBlockDevPath	块设备路径
	返	回	:	>0				磁盘内已使用的总块数
				<0				失败
*/

int tdfs_GetBlockInfo(char* _cPath,TBlockInfo* _pInfo,int _iNum);
/*
	功	能 	:	获取块的开始时间
	参	数	:	_cBlockDevPath	块设备路径
				_pInfo			属性数组
				_iNum			输入，块属性数组长度
								输出，实际返回的块属性个数
	返	回	:	td_success		成功
				其他			失败		
*/

int tdfs_GetFileNum(char* _cPath,int _iBlockId);
/*
	功	能 	:	获取块内的文件总数
	参	数	:	_cBlockDevPath	块设备路径
				_iBlockId		块编号
	返	回	:	>0				块内的文件总数
				<0				失败
*/
int tdfs_BlockStat(char* _cPath,int _iBlockId,TTdStat* _pStat,int _iNum);
/*	
	功	能 	:	获取指定块内所有文件属性
	参	数	:	_cBlockDevPath	块设备路径
				_iBlockId		块编号
				_pStat			文件属性数组
				_iNum			输入，文件属性数组长度
								输出，实际返回的文件属性个数
	返	回	:	td_success		成功
				其他			失败			
*/

int tdfs_Open(TTFileParam* _pFileParam,char* _cFileName,int _iMode);
/*
	功	能	:	打开文件	
	参	数	:	_cFileName 	文件名，调用tdfs_CreateFile函数获取
				_iMode		打开方式，创建新的文件或者打开已有的文件 (O_RDONLY, O_WRONLY, or O_RDWR  O_TRUNC O_APPEND)
	返	回	:	>0			文件句柄
				其他		失败
*/

ssize_t tdfs_Write(int _iFd, const void *_pBuf, size_t _iCount);
/*
	功	能	:	向文件内写数据
	参	数	:	_iFd		文件句柄
				_ucBuf		数据指针
				_iLen		数据长度
	返	回	:	>0			实际写入的数据长度
				其他		失败
*/

ssize_t tdfs_Read(int _iFd, void *_pBuf, size_t _iCount);
/*
	功	能	:	从文件中读取数据
	参	数	:	_iFd		文件句柄
				_ucBuf		数据指针
				_iLen		数据长度
	返	回	:	>0			实际读取到的数据长度
				其他		失败
*/

off_t tdfs_Seek(int _iFd, off_t _lOffSet, int _iWhere);
/*
	功	能	:	文件定位
	参	数	:	_iFd		文件句柄
				_lOffSet	偏移量
				_iWhere		起始位置
	返	回	:	td_success	成功
				其他		失败
*/

ssize_t tdfs_Tell(int _iFd);
/*
	功	能	:	文件定位
	参	数	:	_iFd		文件句柄
	返	回	:	>0			文件指针的当前位置
				其他		失败
*/

int tdfs_Eof(int _iFd);
/*
	功	能	:	判断是否到文件尾
	参	数	:	_iFd		文件句柄
	返	回	:	非0			到文件尾
				0			未到文件尾
*/

int tdfs_Close(int _iFd);
/*
	功	能	:	关闭文件
	参	数	:	_iFd		文件句柄
	返	回	:	td_success	成功
				其他		失败
*/

int tdfs_Unlink(char* _cFileName);
/*
	功	能	:	删除指定的文件
	参	数	:	_cFileName	文件名
	返	回	:	td_success	成功
				其他		失败
*/

int tdfs_Stat(char* _cFileName,TTdStat *_buf);
/*
	功	能	:	获取文件的相关属性
	参	数	:	_cFileName	文件名
				_buf		文件属性结构体
	返	回	:	td_success	成功
				其他		失败				
*/

int tdfs_Lock(char* _cFileName,int _iLock);
/*
	功	能	:	录像文件加锁/解锁
	参	数	:	_cFileName	文件名
				_iLock		0	解锁
							1	加锁
	返	回	:	td_success	成功
				其他		失败
*/
int tdfs_ChangeCreateTime(char* _cFileName,const struct timeval _times);
/*
功	能	:	修改文件的开始时间
参	数	:	_cFileName	文件名
			_times		时间值	
					
返	回	:	td_success	成功
			其他		失败

*/

int tdfs_ChangeTime(char* _cFileName,const struct timeval _times[2]);
/*
	功	能	:	修改文件的修改时间
	参	数	:	_cFileName	文件名
				_iTime		时间值，使用整型数表示	
						
	返	回	:	td_success	成功
				其他		失败
*/

int tdfs_ChangeRecType(char* _cFileName, int _iRecType);
/*
    功  能  :   修改录像文件的录像类型(预录转正式录像时使用)
    参  数  :   _cFileName  文件名
                _iRecType   录像类型(TRecordType类型)
                        
    返  回  :   td_success  成功
                其他        失败
*/

int tdfs_SetUsrParam(char* _cFileName,void *_pBuf,int _iLen);
/*
	功	能	:	设置用户自定义属性
	参	数	:	_cFileName	文件名
				_pBuf		数据指针
				_iLen		数据长度	
						
	返	回	:	td_success	成功
				其他		失败
*/

int tdfs_error(int _iFd);
/*
	功	能	:	获取文件系统最后一次的出错信息
	参	数	:	返回最后一次出错信息
	返	回	:	td_success		成功
				其他			失败
*/

int tdfs_fsync(int _iFd);
/*
	功	能	:	刷新
	参	数	:	_iFd	句柄
	返	回	:	td_success		成功
				其他			失败
*/

int tdfs_UpdataePreRecPtr(int _iFd,long _lBeginPtr,long _lEndPptr);
/*
	功	能	:	将预录区的开始指针及结束指针写入到文件系统中
	参	数	:	_iFd			文件句柄
				_lBeginPtr		预录数据的开始地址
				_lEndPptr		预录数据的结束地址
	返	回	:	td_success		成功
				其他			失败
*/
int tdfs_ftruncate(int _iFd,td_u64 _lRecSize);
/*
	功	能	:	修改文件大小
	参	数	:	_iFd			文件句柄
				_lRecSize		文件大小
				
	返	回	:	td_success		成功
				其他			失败
*/

int tdfs_Mount(char* _cPath);
/*
*/
int tdfs_UnMount(char* _cPath);

int tdfs_GetDebugInfo(char * _strBuf, int _iBufSize);
/*
*/



#endif 
