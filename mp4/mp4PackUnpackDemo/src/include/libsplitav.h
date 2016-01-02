#ifndef _LIBSPLITAV_
#define _LIBSPLITAV_

#define MAX_SPLITAV_FILE_LIST 64
#define AAC_FILE_EXTNAME ".aac"
#define AAC_VALUE_LEN 16

typedef enum
{
	AACMODE = 1, //AAc
	MAXEMODE, //其他方式	
}SplitAvMode;


typedef enum
{
	SplitWait = 0, //未分离
	SplitFinished, //分离完成	
}SplitFileStatus;

typedef struct
{
	td_u8   	m_ucFileName[FULL_PATH_LEN];		//文件名(绝对路径)
}TSplitFileInfo;

typedef struct
{
	TSplitFileInfo   stFileInfo; //文件信息
	SplitFileStatus 	Fileinfo; //计算状态
}splitAv_FileStat;

typedef struct t_Split_File_List
{
	splitAv_FileStat    Rec_File[MAX_CALC_FILE_LIST];  //256个刻录文件列表
	int 			m_iRead;		//读标志
	int 			m_iWrite;  		//写标志
	pthread_mutex_t m_ListMutex;	//锁
}T_SPLIT_FILE_LIST;//当前的刻录队列

td_s32 SplitAv_Init();

td_s32 SplitAv_UnInit(void);

td_s32 SplitAv_SetSplitAvMode (int _iMode);

td_s32 SplitAv_DeleteSplitAvFile (const td_s8* _pcRecFileName);

td_s32 SplitAv_AddToSplitAvFileList (const td_s8* _pcRecFileName);

td_s32 SplitAv_RenameSplitAvFile (const td_s8* _pcSrcRecFileName, const td_s8* _pcNewRecFileName);

td_s32 SplitAv_BackupSplitAvFile (const td_s8* _pcSrcRecFileName, const td_s8* _pcDstRecFileName);

td_s32 SplitAv_iGetSplitAvEnble (td_s32 _iChn);

td_s32 SplitAv_iSetSplitAvEnble (td_s32 _iChn, td_s32 _iEnable);

td_s32 SplitAv_CheckSplitAvFileExist(td_s8* _pcRecFileName, td_s8* _pcFullPath);
td_s32 SplitAv_GenerateAudioFileFullpath(td_s8* _pcTempName, td_s8* _pcAudioFullPath);

td_s32 SplitAv_GetSplitAvFileSize(td_s8 *_pcRecFileName);

#endif /*_LIBTIANDY_*/

