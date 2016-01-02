#ifndef __LIBVFS_H__
#define __LIBVFS_H__

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/vfs.h>
#include <dirent.h>
#include "libtdfs.h"

typedef struct
{
	int				m_iBlockId;		//块编号
	int 			m_iState;		//打开或关闭标志
	int 			m_iFileNum;		//块内共有多少个文件
	int 			m_iPtr;			//读取位置
	TTdStat*		m_tdStat;	
	pthread_mutex_t m_tLock;
	struct dirent	m_dirent;
	DIR*			m_tDir;		
}VDIR;


/* vfile 结构*/
typedef struct vfile
{
	int vfs_fd; //vfs层句柄
	int tdfs_fd; //tdfs层句柄
	void* vfile_interface;
	
}TVfile;

int vfs_Init();
/*
	功	能	:	初始化文件系统。初始化两种文件系统的实例	
	参	数	:

	返	回	:	td_success	成功
				其他		失败
*/

int vfs_DeInit();
/*
	功	能	:	关闭文件系统
	参	数	:	无
	返	回	:	td_success	成功
				其他		失败	
*/

int vfs_PartDisk(char* _cBlockDevPath);
/*
	功	能	:	磁盘分区
	参	数	:	
*/
	
int vfs_Format(char* _cBlockDevPath);
/*
	功	能	:	格式化磁盘，仅可格式化为TDFS
	参	数	:	_cBlockDevPath	块设备路径
	返	回	:	td_success		成功
				其他			失败	
*/

int vfs_Statfs(char* _cPath,TTdStatfs *_pBuf);
/*
	功	能	:	获取文件系统的相关信息
	参	数	:	_cPath			块设备路径或文件系统挂载后的路径
								函数内部需根据路径名区分使用哪种文件系统的函数指针
				_pBuf			保存磁盘空间信息的指针
	返	回	:	td_success		成功
				其他			失败	
*/

int vfs_Open(TTFileParam* _pFileParam,IN OUT char* _cFileName,int _iMode);
/*
	功	能	:	打开文件或创建文件	
	参	数	:	_pFileParam	文件属性	
				_cFileName 	文件名
				_iMode		打开方式，创建新的文件或者打开已有的文件 (O_RDONLY, O_WRONLY, or O_RDWR  O_TRUNC O_APPEND)
	返	回	:	>0			文件句柄
				其他		失败
*/

ssize_t vfs_Write(int _iFd, const void *_pBuf, size_t _iCount);
/*
	功	能	:	向文件内写数据
	参	数	:	_iFd		文件句柄
				_ucBuf		数据指针
				_iLen		数据长度
	返	回	:	>0			实际写入的数据长度
				其他		失败
*/

ssize_t vfs_Read(int _iFd, void *_pBuf, size_t _iCount);
/*
	功	能	:	从文件中读取数据
	参	数	:	_iFd		文件句柄
				_ucBuf		数据指针
				_iLen		数据长度
	返	回	:	>0			实际读取到的数据长度
				其他		失败
*/

off_t vfs_Seek(int _iFd,long _lOffSet,int _iWhere);
/*
	功	能	:	文件定位
	参	数	:	_iFd		文件句柄
				_lOffSet	偏移量
				_iWhere		起始位置
	返	回	:	td_success	成功
				其他		失败
*/

long vfs_Tell(int _iFd);
/*
	功	能	:	返回文件指针的位置
	参	数	:	_iFd		文件句柄
	返	回	:	>0			文件指针的当前位置
				其他		失败
*/

int vfs_Eof(int _iFd);
/*
	功	能	:	判断是否到文件尾
	参	数	:	_iFd		文件句柄
	返	回	:	非0			到文件尾
				0			未到文件尾
*/

int vfs_Close(int _iFd);
/*
	功	能	:	关闭文件
	参	数	:	_iFd		文件句柄
	返	回	:	td_success	成功
				其他		失败
*/


int vfs_ScanDir(const char *dirp, struct dirent ***namelist);
/*
	功	能	:	对文件夹内的目录按照时间进行排序,需要注意内存的申请和释放问题
	参	数	:	dirp		目录名称
				namelist	排序后的目录列表
							标准文件系统为目录名
							tdfs为块序号
	返	回	:	>0	返回排序后的文件夹个数
				<	失败返回
*/

VDIR* vfs_OpenDir(const char* _cDir);
/*
	功	能	:	打开某个目录
	参	数	:	_cDir		路径名称	
	返	回	:	td_success	成功
				其他		失败	
*/

//struct dirent *vfs_ReadDir(VDIR* _pVDir);
TTdStat *vfs_ReadDir(VDIR* _pVDir);
/*
	功	能	:	读取目录中的内容
	参	数	:	_cDir		路径名称				
	返	回	:	td_success	成功
				其他		失败	
*/

int vfs_CloseDir(VDIR* _pVDir);
/*
	功	能	:	关闭某个目录
	参	数	:	_cDir		路径名称				
	返	回	:	td_success	成功
				其他		失败	
*/

int vfs_Unlink(char* _cFileName);
/*
	功	能	:	删除指定的文件
	参	数	:	_cFileName	文件名
	返	回	:	td_success	成功
				其他		失败
*/

int vfs_Stat(char* _cFileName,TTdStat *_buf);
/*
	功	能	:	获取文件的相关属性
	参	数	:	_cFileName	文件名
				_buf		文件属性结构体
	返	回	:	td_success	成功
				其他		失败				
*/

int vfs_Rename(const char *oldpath, const char *newpath);
/*
	功	能	:	录像文件加锁/解锁
	参	数	:	_cFileName	文件名
				_iLock		0	解锁
							1	加锁
	返	回	:	td_success	成功
				其他		失败
*/

int vfs_utimes(const char *filename, const struct timeval times[2]);
/*
	功	能	:	修改文件的修改时间
	参	数	:	_cFileName	文件名
				_iTime		时间值，使用整型数表示	
						
	返	回	:	td_success	成功
				其他		失败
*/

int vfs_SetUsrParam(char* _cFileName,void *_pBuf,int _iLen);
/*
	功	能	:	设置用户自定义属性,EXT4文件系统不支持，直接返回成功
	参	数	:	_cFileName	文件名
				_pBuf		数据指针
				_iLen		数据长度	
						
	返	回	:	td_success	成功
				其他		失败
*/

int vfs_error(int _iFd);
/*
	功	能	:	获取文件系统最后一次的出错信息
	参	数	:	返回最后一次出错信息
	返	回	:	td_success		成功
				其他			失败
*/

int vfs_fsync(int _iFd);
/*
	功	能	:	刷新
	参	数	:	_iFd	句柄
	返	回	:	td_success		成功
				其他			失败
*/

int vfs_UpdataePreRecPtr(int _iFd,long _lBeginPtr,long _lEndPptr);
/*
	功	能	:	将预录区的开始指针及结束指针写入到文件系统中
	参	数	:	_iFd			文件句柄
				_lBeginPtr		预录数据的开始地址
				_lEndPptr		预录数据的结束地址
	返	回	:	td_success		成功
				其他			失败
*/

int vfs_ChangeRecType(char* _cFileName, int _iRecType);
/*
    功  能  :   修改录像文件的录像类型(预录转正式录像时使用)
    参  数  :   _cFileName  文件名
                _iRecType   录像类型(TRecordType类型)
                        
    返  回  :   td_success  成功
                其他        失败
*/

TVfile* vfs_fopen(TTFileParam* _pFileParam,IN OUT char* _cFileName,char* _cMode);

int vfs_fread(void *ptr, size_t size, size_t nmemb, TVfile *stream);

int vfs_fwrite(const void *ptr, size_t size, size_t nmemb, TVfile *stream);

int vfs_fseek(TVfile *stream, long offset, int whence);

long vfs_ftell(TVfile *stream);

int vfs_feof(TVfile *stream);

int vfs_fclose(TVfile *fp);

int vfs_ftruncate(int _iFd, td_u64 length);

int vfs_fcntl(int fd, int cmd, long flg);

int vfs_ChangeCreateTime(char* _cFileName,const struct timeval _times);

#endif 


