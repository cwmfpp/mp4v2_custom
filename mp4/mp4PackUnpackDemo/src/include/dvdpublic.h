#ifndef _DVD_PUBLIC_H
#define _DVD_PUBLIC_H

enum DVD_FILE_TYPE
{
	DVD_FILE = 0,
	DVD_DIR = 1	
};

enum DVD_DEVNO_TYPE
{
	DVD_SR0 = 0,
	DVD_SR1 = 1
};

enum DVD_INIT_STATUS
{
	DVD_UNINIT = 0,
	DVD_INITING = 1,
	DVD_INITED = 2
};

enum DVD_DISK_TYPE
{
	 DVD_ROM,
	 DVD_MINUS_R_SEQ,
	 DVD_RAM,
	 DVD_MINUS_RW_RO,
	 DVD_MINUS_RW_SEQ,
	 DVD_MINUS_R_DL_SEQ,
	 DVD_MINUS_R_DL_J,
	 DVD_PLUS_RW,
	 DVD_PLUS_R,
	 DVD_PLUS_RW_DL,
	 DVD_PLUS_R_DL,
	 BD_ROM,
	 BD_R_SRM,
	 BD_R_RRM,
	 BD_RE,
	 DVD_UNKNOWN	    
};

enum DVD_TRAY_ACTION
{
	DVD_EJECT_TRAY = 0,
	DVD_LOAD_TRAY = 1
};

enum DVD_LOCK_ACTION
{
	DVD_UNLOCK = 0,
	DVD_LOCK = 1
};



enum DVD_MEDIA_EXIST
{
	DVD_DISK_NO_EXIST = 0,
	DVD_DISK_EXIST  = 1
};

enum DVD_DISKSPACE_BLANK
{
	DVD_DISK_NO_BLANK = 0,
	DVD_DISK_BLANK = 1
};



#define DVD_MAX_TRACK_NUM 5
#define DVD_MAX_FILE_NAME_LEN 255
#define DVD_DEVNO_VALID(_uiDEVNo) (  _uiDEVNo >= DVD_SR0 && _uiDEVNo <= DVD_SR1)
#define DVD_DEVFD_VALID(_uiDEVFD) (  _uiDEVFD > 0)
#define DVD_TRACK_VALID(_uiTrackNo) (_uiTrackNo >= 1 && _uiTrackNo < DVD_MAX_TRACK_NUM)
#define DVD_IS_BLANK(_ucBuf)  ( (_ucBuf[2]&3) == 0)
#define DVD_DEV_FULL_PATH_LEN 64
#define DVD_INVALID_UINT_VALUE 0xFFFFFFFF
#define DVD_DEVNUM 2
#define DVD_CLEAR_DISK_BUFFER(pDev) do{ memset(pDev->m_pBuffer, 0x00, (pDev->m_uiBlockSize * pDev->m_uiBlockNumPerPacket));}while(0)
#define DVD_MARK_DISK_BUFFER(pDev) do{ memset(pDev->m_pBuffer, 0xAA, (pDev->m_uiBlockSize * pDev->m_uiBlockNumPerPacket));}while(0)


#define DVD_MIN_VALUE 1e-8  //根据需要调整这个值
#define DVD_IS_DOUBLE_ZERO(d) (abs(d) < DVD_MIN_VALUE) 
#define DVD_1X	1352	// 1385 * 1000 / 1024
#define BD_1X	4390	// 4495.5 * 1000 / 1024
#define REVERSE_TRACK_BLOCKNUM 1024
#define FILE_TRACK_START_LBA ((unsigned int) 0x400)
#define DVD_ALIGN(size, boundary) (((size) + ((boundary) - 1)) & ~((boundary) - 1))
#define DVD_ALIGN_DEFAULT(size) DVD_ALIGN(size, (32 * 1024))

#define DVD_DISK_BAD (-1)
#define DVD_UDF_EXIST (0)
#define DVD_UDF_ERROR (1)
#define DVD_NOT_UDF (2)
#define DVD_FIRSTTRACK_BLANK (3)
#define DVD_UDF_FILEBODY_NUM 2

//#define IN
#define OUT
#define INOUT

typedef struct udf_file
{
	unsigned int m_uiIndex;
	unsigned int m_uiParentIndex; // 0代表root
	unsigned int m_uiBodyStartLBA[DVD_UDF_FILEBODY_NUM]; // 文件体起始地址 类型为目录的文件填0
	unsigned int m_uiBodyEndLBA[DVD_UDF_FILEBODY_NUM];// 文件体结束地址 类型为目录的文件填0
	unsigned int m_uiHeadStartLBA;// 文件头起始地址 类型为目录的文件填0
	unsigned int m_uiHeadEndLBA;// 文件头结束地址 类型为目录的文件填0
	unsigned long long  m_uiHeadSize; // 真实长度, 单位：字节，类型为目录的文件填0
	unsigned long long m_uiBodySize[DVD_UDF_FILEBODY_NUM]; // 真实长度，单位：字节，类型为目录的文件填0
	unsigned int m_uiFiletype;	// 0：数据文件 1：目录
       unsigned char m_pFileName[256];	// 文件(目录)名
	unsigned char m_rezerved[32]; // 保留字段，为以后扩展使用
}udf_file;

typedef struct udf_fs
{
	unsigned int m_uiDevNo; // 光驱设备号
	unsigned int m_uiFileNum; // 这张盘一共要刻几个数据文件
	unsigned int m_uiFolderNum; // 这张盘一共要刻几个目录
	unsigned char m_pLogicVolIdent[128]; // 光盘卷标
	unsigned int m_lengthAlloc; // 变长分配，这个字段记录allocUdfFile和m_allocUdfFolder的分配长度，单位：字节
	unsigned char m_allocUdfFolder[0]; // 实际的struct udf_file 目录，自顶向下组装
	unsigned char m_allocUdfFile[0]; // 实际的struct udf_file  数据文件
}udf_fs;

/************************************************* 
Function: udf_Check
Description: 检查光盘及设备是否可用
Input: _uiDevNo： 光驱设备号
Output: 
Return: 返回成功或失败 0：td_success；-1：td_failure；
Others: 
*************************************************/
int udf_Check(unsigned int  _uiDevNo);

/************************************************* 
Function: udf_Ready
Description: 初始化光盘，使光驱处于工作状态
Input: _uiDevNo： 光驱设备号
Output: 
Return: 返回成功或失败 0：td_success；-1：td_failure；
Others: 
*************************************************/
int udf_Ready(unsigned int  _uiDevNo);

/************************************************* 
Function: udf_IsBlankDisk
Description: 判断是否为空白光盘
Input:  _uiDevNo：光驱设备号
Output: 
Return: 0:非空      1：空光盘-1:失败
Others: 
*************************************************/
int udf_IsBlankDisk(unsigned int _uiDevNo);

/************************************************* 
Function: udf_ControlDVDRomUnit 
Description: 控制光驱芯片运转
Input: _uiDevNo：光驱设备号
          _uiAction：1：启动Unit  0：停止Unit
Output: 
Return: 返回成功或失败 0：td_success；-1：td_failure；
Others:
*************************************************/
int udf_ControlDVDRomUnit (unsigned int _uiDevNo, unsigned int _uiAction);

/************************************************* 
Function: udf_TestDVDRomUnit 
Description: 测试光驱芯片运转
Input: _uiDevNo：光驱设备号     
Output: 
Return: 返回成功或失败 0：芯片运转正常；非0：芯片异常；
Others:
*************************************************/
int udf_TestDVDRomUnit (unsigned int _uiDevNo);


/************************************************* 
Function: udf_GetDiskType
Description: 获取光盘类型
Input: _uiDevNo：光驱设备号
Output: _pType：光盘类型，详情参见枚举体DVD_DISK_TYPE
Return: 返回成功或失败 0：td_success；-1：td_failure；
Others: 
*************************************************/
int udf_GetDiskType (unsigned int _uiDevNo, OUT unsigned int * _pType);

/************************************************* 
Function: udf_GetCapacity
Description: 获取光盘剩余容量
Input:    _uiDevNo：光驱设备号        
Output: 	_puiTotalSpace光盘总容量，单位：MB
		_puiUsedSpace光盘已用容量，单位：MB
Return: 返回成功或失败 0：td_success；-1：td_failure；
Others:
*************************************************/
int udf_GetCapacity(unsigned int _uiDevNo, OUT unsigned int * _puiTotalSpace, OUT unsigned int * _puiUsedSpace);

/************************************************* 
Function: udf_GetNextWriteLBA
Description: 获取当前盘中可以写入的地址 
Input:  _uiDevNo：光驱设备号      
Output: _uiLBA：当前盘中可写入的逻辑地址
Return:        返回成功或失败 0：td_success；-1：td_failure；
Others: 
*************************************************/
int udf_GetNextWriteLBA(unsigned int _uiDevNo, OUT unsigned int *_uiLBA);

/************************************************* 
Function: udf_WriteDisk
Description: 写盘 
Input:  _uiDevNo：光驱设备号
_pbuf：写入数据
_uiLen：写入数据的实际长度
Output: _puiEndLBA：写盘的结束地址
Return: >=0：返回写入盘里的长度；-1：td_failure；
Others: 
*************************************************/
int udf_WriteDisk(unsigned int _uiDevNo, IN unsigned char* _pbuf, unsigned int _uiLen, OUT unsigned int * _puiEndLBA);

/************************************************* 
Function: udf_ReadDisk
Description: 读取文件数据体 
Input:  	_uiDevNo：光驱设备号
       	_uiStartLBA：读盘的起始地址从0开始
		_pbuf：读取数据
		_uiLen：读取数据的实际长度
Output: 
Return: >=0：返回读取的长度；-1：td_failure；
Others: 

*************************************************/
int udf_ReadDisk(unsigned int _uiDevNo, unsigned int _uiStartLBA, OUT unsigned char* _pbuf, int _uiLen);

/************************************************* 
Function: udf_CreateFS
Description: 生成UDF文件系统
Input:   _uiDevNo：光驱设备号
		_pFS：标识需要刻录的所有文件(数据+目录)
Output: 
Return: 0：td_success；-1：td_failure   >0:代表失败，值为光驱错误码
Others: 
*************************************************/
int udf_CreateFS (unsigned int _uiDevNo, IN struct udf_fs * _pFS);

/************************************************* 
Function: udf_RepairDisk
Description: 修复光盘
Input:   _uiDevNo：光驱设备号
 	    _pFS：标识需要刻录的所有文件(数据+目录)
Output: 
Return: 返回成功或失败 0：td_success；-1：td_failure；
Others: 
*************************************************/
int udf_RepairDisk(unsigned int _uiDevNo, IN struct udf_fs * _pFS);

/************************************************* 
Function: udf_CloseDisk
Description: 封盘
Input:_uiDevNo：光驱设备号        
Output: 
Return: 返回成功或失败 0：td_success；-1：td_failure；
Others: 
*************************************************/
int udf_CloseDisk(unsigned int _uiDevNo);

/************************************************* 
Function: udf_SetSpeed 
Description: 获取光盘类型
Input: _uiDevNo：光驱设备号
         _uiSpeed：4.0、6.0、8.0、12.0、16.0
Output: 
Return: 返回成功或失败 0：td_success；-1：td_failure；
Others: 
*************************************************/
int udf_SetSpeed (unsigned int _uiDevNo, double _dSpeed);

/************************************************* 
Function:udf_IsUDFExist
Description: 判断是否在光盘已经生成文件系统
Input:  _uiDevNo：光驱设备号		
Output: 
Return: -1 :光盘损坏
           0:UDF存在
           1:是UDF但是不完整
           2:有内容但不是UDF
           3:第一个轨道没有刻录内容
Others: 
*************************************************/
int udf_IsUDFExist(unsigned int _uiDevNo);

/************************************************* 
Function: udf_SyncDVDCache 
Description: 将光驱内的缓存刷新到盘上
Input: _uiDevNo：光驱设备号        
Output: 
Return: 返回成功或失败 0：td_success；-1：td_failure；
Others: 
*************************************************/
int udf_SyncDVDCache (unsigned int _uiDevNo);

/************************************************* 
Function: udf_ReleaseDVDDevice 
Description:释放光驱设备资源接口
Input: _uiDevNo：光驱设备号          
Output: 
Return: 返回成功或失败 0：td_success；-1：td_failure；
Others: 
*************************************************/
int udf_ReleaseDVDDevice(unsigned int _uiDevNo);

/************************************************* 
Function: udf_Eject_Load_Tray 
Description:弹仓接口
Input: _uiDevNo：光驱设备号
          _uiAction :    0:弹仓 1:关仓
Output: 
Return: 返回成功或失败 0：td_success；-1：td_failure；
Others: 
*************************************************/
int udf_Eject_Load_Tray(unsigned int _uiDevNo, unsigned int _uiAction);

/************************************************* 
Function: udf_IsDiskExist 
Description:判断光驱内是否有盘
Input: _uiDevNo：光驱设备号        
Output: 
Return: 1:有盘 0:没盘 -1:失败
Others: 
*************************************************/
int udf_IsDiskExist(unsigned int _uiDevNo);

/************************************************* 
Function: udf_OpenDVDDevice 
Description:打开光驱设备资源接口
Input: _uiDevNo：光驱设备号          
Output: 
Return: 返回成功或失败 0：td_success；-1：td_failure；
Others: 
*************************************************/
int udf_OpenDVDDevice(unsigned int _uiDevNo);

int udf_RepairLastWritenBlock(unsigned int _uiDevNo);

int udf_InitDVDDevice(unsigned int _uiDevNum);
int udf_WriteDiskByOneBlock(unsigned int _uiDevNo, IN unsigned char* _pbuf, unsigned int _uiLen, OUT unsigned int * _puiEndLBA);
int udf_ReadDiskByOneBlock(unsigned int _uiDevNo, unsigned int _uiStartLBA, OUT unsigned char* _pbuf, int _iLen);
int udf_ControlDVDLock (unsigned int _uiDevNo, unsigned int _uiAction);
int udf_KeepBDAlive(unsigned int  _uiDevNo);


int udf_GetNextFakeWriteLBA(unsigned int _uiDevNo, OUT unsigned int *_uiLBA);
#endif
