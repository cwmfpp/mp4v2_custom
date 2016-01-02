#ifndef __LIBSTDFS_H__
#define __LIBSTDFS_H__

#include <sys/stat.h> 
#include <sys/statfs.h> 
#include "public_def.h"
#include <sys/vfs.h> 
#include "libtdfs.h"
#include "libvfs.h"
#if 0
typedef enum
{
	FS_TD,
	FS_NORMAl,
}TFsType;

typedef enum
{
	TD_FILE_TYPE_REC = 0,	//录像文件
	TD_FILE_TYPE_PRO,		//预录文件
	TD_FILE_TYPE_PIC,		//图片文件
}TTdFileType;

typedef struct
{
	struct stat m_stat;
	td_u64		m_iRecChn;
	td_u64 		m_iRecLock;
	td_u64 		m_iRecType;
	td_char		m_iOthAttribute[32];			//自定义属性	
	char		m_cFileName[MAX_PATH_LEN];		//文件名
}TTdStat;

typedef struct
{
	int 		m_iChn;							//通道号	
	int			m_iTime;						//录像文件的开始时间		
	TTdFileType m_eType;						//录像文件类型，录像/预录/图片
	int			m_iPreSize;						//预录文件大小byte
	int			m_iParam;						//录像文件属性
	int  		m_iCustomType;					//自定义类型或者毫秒值
	char		m_cBlockDevPath[MAX_PATH_LEN];	//块设备路径
}TTFileParam;

typedef struct
{
	int 		m_iId;					//块编号
	td_u64 	m_lStartTime;		//块的起始时间
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
#endif

int stdfs_Init();
/*
	功	能	:	初始化文件系统。
	参	数	:

	返	回	:	td_success	成功
				其他		失败
*/

int stdfs_DeInit();
/*
	功	能	:	关闭文件系统
	参	数	:	无
	返	回	:	td_success	成功
				其他		失败	
*/



int stdfs_Open(TTFileParam* _pFileParam,char* _cFileName,int _iMode);
/*
	功	能	:	打开文件	
	参	数	:	_cFileName 	文件名，调用tdfs_CreateFile函数获取
				_iMode		打开方式，创建新的文件或者打开已有的文件 (O_RDONLY, O_WRONLY, or O_RDWR  O_TRUNC O_APPEND)
	返	回	:	>0			文件句柄
				其他		失败
*/

size_t stdfs_Write(int _iFd, const void *_pBuf, size_t _iCount);
/*
	功	能	:	向文件内写数据
	参	数	:	_iFd		文件句柄
				_ucBuf		数据指针
				_iLen		数据长度
	返	回	:	>0			实际写入的数据长度
				其他		失败
*/

size_t stdfs_Read(int _iFd, void *_pBuf, size_t _iCount);
/*
	功	能	:	从文件中读取数据
	参	数	:	_iFd		文件句柄
				_ucBuf		数据指针
				_iLen		数据长度
	返	回	:	>0			实际读取到的数据长度
				其他		失败
*/

off_t stdfs_Seek(int _iFd,off_t _lOffSet,int _iWhere);
/*
	功	能	:	文件定位
	参	数	:	_iFd		文件句柄
				_lOffSet	偏移量
				_iWhere		起始位置
	返	回	:	td_success	成功
				其他		失败
*/

long stdfs_Tell(int _iFd);
/*
	功	能	:	文件定位
	参	数	:	_iFd		文件句柄
	返	回	:	>0			文件指针的当前位置
				其他		失败
*/

int stdfs_Eof(int _iFd);
/*
	功	能	:	判断是否到文件尾
	参	数	:	_iFd		文件句柄
	返	回	:	非0			到文件尾
				0			未到文件尾
*/

int stdfs_Close(int _iFd);
/*
	功	能	:	关闭文件
	参	数	:	_iFd		文件句柄
	返	回	:	td_success	成功
				其他		失败
*/

int stdfs_Unlink(char* _cFileName);
/*
	功	能	:	删除指定的文件
	参	数	:	_cFileName	文件名
	返	回	:	td_success	成功
				其他		失败
*/

int stdfs_Stat(char* _cFileName,TTdStat *_buf);
/*
	功	能	:	获取文件的相关属性
	参	数	:	_cFileName	文件名
				_buf		文件属性结构体
	返	回	:	td_success	成功
				其他		失败				
*/

int stdfs_Rename(const char *oldpath, const char *newpath);
/*
	功	能	:	录像文件加锁/解锁
	参	数	:	_cFileName	文件名
				_iLock		0	解锁
							1	加锁
	返	回	:	td_success	成功
				其他		失败
*/

int stdfs_ChangeTime(char* _cFileName, const struct timeval times[2]);
/*
	功	能	:	修改文件的修改时间
	参	数	:	_cFileName	文件名
				_iTime		时间值，使用整型数表示	
						
	返	回	:	td_success	成功
				其他		失败
*/


int stdfs_SetUsrParam(char* _cFileName,void *_pBuf,int _iLen);
/*
	功	能	:	设置用户自定义属性
	参	数	:	_cFileName	文件名
				_pBuf		数据指针
				_iLen		数据长度	
						
	返	回	:	td_success	成功
				其他		失败
*/

td_u32 stdfs_error(int _iFd);
/*
	功	能	:	获取文件系统最后一次的出错信息
	参	数	:	返回最后一次出错信息
	返	回	:	td_success		成功
				其他			失败
*/

int stdfs_fsync(int _iFd);
/*
	功	能	:	刷新
	参	数	:	_iFd	句柄
	返	回	:	td_success		成功
				其他			失败
*/
#if 0
void* stdfs_fopen(TTFileParam* _pFileParam, IN OUT char* _cFileName, char* _cMode);
int stdfs_fread(void *ptr, size_t size, size_t nmemb, TVfile *stream);
int stdfs_fwrite(const void *ptr, size_t size, size_t nmemb, TVfile *stream);
int stdfs_fseek(TVfile *stream, long offset, int whence);
int stdfs_ftell(TVfile *stream);
int stdfs_feof(TVfile *stream);
int stdfs_fclose(TVfile *fp);
#endif
//int (*vfile_fstat)(int _iFd, TTdStat *buf);
int stdfs_ftruncate(int _iFd, off_t length);
int stdfs_fcntl(int fd, int cmd, long flg);
int stdfs_Format(char* _cBlockDevPath);
/*
	功	能	:	格式化磁盘
	参	数	:	_cBlockDevPath	块设备路径
	返	回	:	td_success		成功
				其他			失败	
*/

int stdfs_Statfs(char* _cBlockDevPath,TTdStatfs *_pBuf);
/*
	功	能	:	获取磁盘空间信息
	参	数	:	_cBlockDevPath	块设备路径
				_pBuf			保存磁盘空间信息的指针
	返	回	:	td_success		成功
				其他			失败	
*/	

int vfs_PartDisk(char* _cBlockDevPath);
/*
	功	能	:	磁盘分区
	参	数	:	
*/

int stdfs_ScanDir(const char *dirp, struct dirent ***namelist);
VDIR* stdfs_OpenDir(const char* _cDir);
struct dirent * stdfs_ReadDir(VDIR* _cDir);
int stdfs_CloseDir(VDIR* _cDir);

#endif 

