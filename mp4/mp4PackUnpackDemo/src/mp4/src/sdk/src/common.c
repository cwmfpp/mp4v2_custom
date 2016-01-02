#ifdef FOR_MP4SDK
/*****************************************************************************
 * NAME         : common.c
 * FUNCTION     : MP4  ×ª»»»Ø·ÅÏà¹Ø¹«¹²½Ó¿ÚÎÄ¼þ
 * PROGRAMMED   : ´Þè÷âý
 * DATE         : 2015-05-25
 * PROJECT      : 
 * OS           : windows
 ****************************************************************************/
#include <Windows.h>

#define __func__ __FUNCTION__
#else
/*****************************************************************************
 * NAME         : common.c
 * FUNCTION     : MP4  ×ª»»»Ø·ÅÏà¹Ø¹«¹²½Ó¿ÚÎÄ¼þ
 * PROGRAMMED   : ÕÅÊ¤
 * DATE         : 2014-12-10
 * PROJECT      : 
 * OS           : Linux
 ****************************************************************************/
#include <unistd.h>
#include <dirent.h>
#include <arpa/inet.h>
#endif // FOR_MP4SDK

#ifdef ALONE
#include <string.h>
#include <stdio.h>
#include <malloc.h>
#include <errno.h>
#include <time.h>
#include <sys/stat.h>
#include <stdarg.h>


#include "public_def.h"
#include "td_errno.h"
#include "libtiandy.h"
#include "td_mp4.h"

#define PATH_LOCK_POS 43
#define PATH_DISK_POS 10

#define DEV_PATH	 "/dev"
#define SATA_PATH    "/mnt/sata" //SATAÓ²ÅÌ¹ÒÔØ¸ùÄ¿Â¼
#define USB_PATH     "/mnt/usb" //USBÉè±¸¹ÒÔØÄ¿Â¼
#define NFS_PATH      "/mnt/nfs"  //NFS¹ÒÔØÄ¿Â¼
#define ESATA_PATH   SATA_PATH   //eSATAÉè±¸¹ÒÔØÄ¿Â¼
#define SDCARD_PATH   "/mnt/sdcard"   //SdCardÉè±¸¹ÒÔØÄ¿Â¼
#define DB_FILE_NAME  "index.db"//ÐÂ½¨Êý¾Ý¿âÎÄ¼þÃû
#define MAX_BAD_FILE_SYSTEM	16		//×î´óÖ»¶ÁÎÄ¼þÏµÍ³¸öÊý
#define SYS_INFO_PRINTF 0x12345678

#if 1
int td_printf_alone(unsigned int handle, char* pszfmt, ...)
{
	va_list struAp;
	time_t now;
	struct tm tm_now;
	int ret;
	char* format= NULL;
#ifdef TD_DEBUG 
	if(pszfmt == NULL) return -1;

	format = (char*)calloc(strlen(pszfmt)+3,1);
	if(format == NULL) return -2;
	strcpy(format,pszfmt);
	if(format[strlen(pszfmt)-1]=='\n')
	{
		format[strlen(pszfmt)] = format[strlen(pszfmt)-2]=='\r' ?'\0':'\r';
	}
	else if(format[strlen(pszfmt)-1]=='\r')
	{
		format[strlen(pszfmt)] = format[strlen(pszfmt)-2]=='\n' ?'\0':'\n';
	}
	else
	{
		format[strlen(pszfmt)] ='\r';
		format[strlen(pszfmt)+1] ='\n';
	}
	now=time(&now);
	#if 0 //¼ÓÈëÊ±Çøºó£¬Ê±¼äÖµ²îÊ±ÇøÖµ
	ptm_now = gmtime(&now);
	#else
	localtime_r(&now, &tm_now);
	#endif

#ifndef FOR_MP4SDK
	if(0 == handle)
	{ 	
		printf("[1;33m""[%04d/%02d/%02d %02d:%02d:%02d %ld]""[0;39m",
		    tm_now.tm_year+1900,
		    tm_now.tm_mon+1,
		    tm_now.tm_mday,
		    (tm_now.tm_hour)%24,
		    tm_now.tm_min,
		    tm_now.tm_sec,
		    getpid());

		va_start(struAp, pszfmt);
		ret = vprintf(format, struAp);
		va_end(struAp);
	}else if(0xFF0000 == handle){
		printf("[1;31m""[%04d/%02d/%02d %02d:%02d:%02d %ld]""[0;39m",
		    tm_now.tm_year+1900,
		    tm_now.tm_mon+1,
		    tm_now.tm_mday,
		    (tm_now.tm_hour)%24,
		    tm_now.tm_min,
		    tm_now.tm_sec,
		    getpid());

		va_start(struAp, pszfmt);
		ret = vprintf(format, struAp);
		va_end(struAp);
	}else if(SYS_INFO_PRINTF == handle){
		printf("[1;36m");
		va_start(struAp, pszfmt);
		ret = vprintf(format, struAp);
		va_end(struAp);
		printf("[0;39m\r\n");
	}else{
		FILE *file;  
		file = fopen("printf.txt","a+");
		fprintf(file,"[%04d/%02d/%02d %02d:%02d:%02d]",
			tm_now.tm_year+1900,
			tm_now.tm_mon+1,
			tm_now.tm_mday,
			(tm_now.tm_hour)%24,
			tm_now.tm_min,
			tm_now.tm_sec);
	      
		va_start(struAp, pszfmt);
		ret = vfprintf(file,format, struAp);
		va_end(struAp);
		fclose(file);
	}
#else
	#ifdef _DEBUG
		static const int iMaxPath = 512;
		char pcData[iMaxPath] = {0};

		SYSTEMTIME st;
		::GetLocalTime(&st);
		sprintf_s(pcData, iMaxPath - 1, "%02d:%02d:%02d:%03d     ", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
		int iLen = strlen(pcData);

		va_list marker;
		va_start(marker, pszfmt);
		ret = vsnprintf_s(pcData + iLen, iMaxPath - iLen, _TRUNCATE, format, marker);
		va_end(marker);
		::OutputDebugString(pcData);
	#else
		(void)format;
	#endif
#endif

	free(format);
#endif
	return ret;
}
#endif

int Common_IsValidFileHeader(S_FileHeader* _header)
{
	if(!_header)
	{
		return -1;
	}
	if(_header->Mode != 21)
	{
		return -1;
	}
	if((_header->FrameRate > 60))//ÔÝÊ±Ð´Îª60
	{
    	return -1;
	}
	if((_header->Width==720)&&(_header->Height==1280))
	{
		return 2;
	}
	if((_header->Width==1080)&&(_header->Height==1920))
	{
		return 2;		
	}
	if(_header->Width == 352)
	{   //Half-Cif, Cif
    	if( (_header->Height == 144) || (_header->Height == 288) ||
        	(_header->Height == 120) || (_header->Height == 240) )
        	return 1;
	}
	else if((_header->Width == 720) || (_header->Width == 704))
	{   //Half-D1, D1, 4Cif, 2Cif
    	if( (_header->Height == 288) || (_header->Height == 576) ||
        	(_header->Height == 240) || (_header->Height == 480) )
        	return 1;
	}
	else if(_header->Width == 176)
	{   //QCIF
    	if(_header->Height == 144)
        	return 1;
	}
	else if(_header->Width == 640)
	{
    	//VGA
    	if(_header->Height == 480||_header->Height == 240)//xujiayu for HVGA
        		return 1;
	}
	//add by 1499 for nvr
	else if((_header->Width == 1280)&&(_header->Height== 720))
	{
            	return 1;
	}
	else if((_header->Width == 1280)&&(_header->Height== 960))
	{
            	return 1;
	}
	else if((_header->Width == 1600)&&(_header->Height== 1200))
	{
            	return 1;
	}
	else if((_header->Width == 1920)&&(_header->Height== 1080 || _header->Height== 1088))
	{
            	return 1;
	}
	else if((_header->Width == 960)&&(_header->Height== 576))
	{
            	return 1;
	}
	else if((_header->Width == 960)&&(_header->Height== 480))
	{
            	return 1;
	}
	else if((_header->Width == 320)&&(_header->Height== 240))
	{
            	return 1;
	}
	else if((_header->Width == 2560)&&(_header->Height== 2048))
	{
            	return 1;
	}
	else if((_header->Width == 2560)&&(_header->Height== 1920))
	{
            	return 1;
	}
	else if((_header->Width == 2448)&&(_header->Height== 2048))//QHD
	{
            	return 1;
	}
	else if((_header->Width == 2048)&&(_header->Height== 1536))//QXGA
	{
            	return 1;
	}
    	return -1;
}

td_s32 Common_Stat(char * _pcFilePath, void *_pstStat)
{
	td_printf_alone(0, "%s start!", __FUNCTION__);
	int 	iRet = -1;
	char	cPath[128] = {0};
	char	cDisk;
	char	cLock;
	int	i = 0;
	
	if (_pcFilePath == NULL || _pcFilePath[0] == '\0' || strlen(_pcFilePath) > 127)
	{
		td_printf_alone(0, "_pcFilePath is %s, len is %d", _pcFilePath, strlen(_pcFilePath));
		return ERR_REC_FAILED;
	}
	td_printf_alone(0, "%s %d _pcFilePath = %s ", __func__, __LINE__, _pcFilePath);
	memset(cPath, 0, 128);
	strncpy(cPath, _pcFilePath, strlen(_pcFilePath));
	cDisk = cPath[PATH_DISK_POS];
	cLock = cPath[PATH_LOCK_POS];

	for (i = -1; i < MAX_SATA_NUM; i++)
	{
		td_printf_alone(0, "%s, %s", __func__, cPath);
		if (cDisk == '0' + i)
		{
			continue;
		}
		if (i >= 0)
		{
			cPath[PATH_DISK_POS] = '0' + i;
		}
		if ((iRet = td_mp4_stat(cPath, _pstStat)) == 0)
		{
			break;
		}

		if(strstr(cPath, "jpg") != 0)
		{
			continue;
		}
		
		cPath[PATH_LOCK_POS] = (cPath[PATH_LOCK_POS] == 'C') ? 'L' : 'C';
		if ((iRet = td_mp4_stat(cPath, _pstStat)) == 0)
		{
			break;
		}
	}

	if (!iRet && (cDisk != cPath[PATH_DISK_POS] || cLock != cPath[PATH_LOCK_POS]))
	{
		td_printf_alone(0, "%s, From %s To %s", __func__, _pcFilePath, cPath);
		strncpy(_pcFilePath, cPath, strlen(_pcFilePath));
	}
	
	return iRet;
}


#ifdef TD_VFS
#define Common_FileName2FileType(_pcFileName)\
	(strstr((char *)_pcFileName, ".jpg") != NULL ? FILE_IMG : (_pcFileName[20] == 'B' ? FILE_BACKUP : FILE_REC))


//ÅÐ¶ÏÎÄ¼þÃûÊÇ·ñºÏ·¨
td_s32 Common_IsValidFileName(td_u8* _pucFileName)
//Example:H00C0T20070802113C38.sdv
{
	if((_pucFileName[0] != 'U')&&
		(_pucFileName[0] != 'H')&&
		(_pucFileName[0] != 'N'))
	{
		return 0;
	}
	
	/*
	if((_pucFileName[3] != 'C') &&
		(_pucFileName[3] != 'D') &&
		(_pucFileName[3] != 'E') &&
		(_pucFileName[3] != 'F'))
	{
		return 0;
	}
	if((_pucFileName[5] != 'M') && 
		(_pucFileName[5] != 'D') &&
		(_pucFileName[5] != 'P') &&
		(_pucFileName[5] != 'T') && 
		(_pucFileName[5] != 'L') &&
		(_pucFileName[5] != 'O') &&//20130409 ÍõÀÚadd
		(_pucFileName[5] != 'A') && //add by gyn
		(_pucFileName[5] != 'C'))	//ÌÕÓÀÁÁ 100127 ×Ô¶¨ÒåÂ¼ÏñÀàÐÍ
	{
		return 0;
	}
	*/

	//ÌÕÓÀÁÁ 091105 Ôö¼Ó±¸·ÝÂ¼ÏñÎªºÏ·¨Â¼Ïñ
	if (_pucFileName[20] != 'I' && 
		_pucFileName[20] != 'N' && 
		_pucFileName[20] != 'C' &&
		_pucFileName[20] != 'L' &&
		_pucFileName[20] != 'B')
	{
		return 0;
	}
	return 1;
}

//¸ù¾ÝÎÄ¼þÃû²¹È«È«Â·¾¶(¾ø¶ÔÂ·¾¶)
td_s32 Common_CompleteFileName(td_s8* _pcFileName, td_s8* _pcFullPath, td_s8* _pcPath, td_s8* _pcShortPath) //toodoo
{
	//Example: /mnt/sata/0/0/20070802/H00CFT20070802113038C38.sdv
	td_s8 cDevType[4] = {0};					//´æ´¢Éè±¸ÀàÐÍ
	td_u8 cDevNo[4] = {0};						//´æ´¢Éè±¸ºÅ
	td_u8 cPartNo[4] = {0};						//´æ´¢Éè±¸·ÖÇøºÅ
	td_u8 cPath[16] = {0};						//ÈÕÆÚ

	//ÈýÖÖ³¤¶ÌµÄÂ·¾¶
	td_u8	ucShortPath[SHORT_PATH_LEN] = {0};
	td_u8	ucMiddlePath[MIDDLE_PATH_LEN] = {0};
	td_u8	ucFullPath[FULL_PATH_LEN] = {0};

	if (_pcFileName == NULL || _pcFileName[0] == '\0' )
	{
		return ERR_REC_WRONG_PARAM;
	}

	//sherry add 20101020
	if(_pcFileName == NULL || _pcFileName[0] == '\0')
	{
        return ERR_REC_WRONG_PARAM;
	}
	if(strlen((char *)_pcFileName)>28)
	{
		if((_pcFileName[0]=='C')&&(Common_IsValidFileName((td_u8 *)(_pcFileName+2))))
		{
			sprintf((char *)ucFullPath,"/mnt/cdrom%c/%s",_pcFileName[1],_pcFileName+2);
			td_printf_alone(0,"%s %s %s",__func__,_pcFileName,ucFullPath);
			strcpy((char *)_pcFullPath, (char *)ucFullPath);
		}
		else
		{
			td_printf_alone(0, "%s, _pcFileName is %s", __FUNCTION__, _pcFileName);
		}
		return ERR_REC_SUCCESSFUL;
	}

	if (!Common_IsValidFileName((td_u8 *)_pcFileName) || strlen((char *)_pcFileName) > 27)
	{
		return ERR_REC_WRONG_PARAM;
	}
	strncpy((char *)cDevType, (char *)(_pcFileName + 0), 1);
	strncpy((char *)cDevNo, (char *)(_pcFileName + 1), 1);
	strncpy((char *)cPartNo, (char *)(_pcFileName + 2), 1);
	strncpy((char *)cPath, (char *)(_pcFileName + 6), 8);
	//td_printf_alone(0,"FileName : %s", _pcFileName);
	//td_printf_alone(0, "rec_CompleteFileName : cDevType = %s, cDevNo = %s, cPartNo = %s, cPath = %s",
		//cDevType, cDevNo, cPartNo, cPath);

	//²¹È«¶ÌÂ·¾¶,Like  /mnt/sata/0/0
	switch(cDevType[0])
	{
		case 'H':
			//strcpy((char *)ucShortPath, TDFS_SATA_FULLPATH);
			//sprintf((char *)ucShortPath, TDFS_SATA_FULLPATH, atoi((char *)cDevNo));
            //disk_SATATDFSMountPath((char *)ucShortPath, disk_AtoI((char)cDevNo[0]));
			cPath[0] = '\0';
			//ÈÏÎª¼´½«·ÃÎÊ¸Ã´ÅÅÌ£¬»½ÐÑ¸Ã´ÅÅÌ
			//disk_WakeUpDisk(disk_AtoI((char)cDevNo[0]));
			break;
		case 'U':
			strcpy((char *)ucShortPath, USB_PATH);
			strcat((char *)ucShortPath, "/");
			strcat((char *)ucShortPath, (char *)cDevNo);
			strcat((char *)ucShortPath, "/");
			strcat((char *)ucShortPath, (char *)cPartNo);
			break;
		case 'N':
			strcpy((char *)ucShortPath, NFS_PATH);
			break;
		default:
			td_printf_alone(0xFF0000, "%s : Wrong disk type : %c", __func__, cDevType[0]);
			return ERR_REC_WRONG_PARAM;
	}

	//²¹È«ÖÐÂ·¾¶,Like /mnt/sata/0/0/20070802
	strcpy((char *)ucMiddlePath, (char *)ucShortPath);
	//±¸·ÝÂ¼Ïñ¶ÎÂ·¾¶ºÍÖÐÂ·¾¶Ò»Ñù£¬Ã»ÓÐÈÕÆÚÊ±¼äÄ¿Â¼
	if (FILE_BACKUP != Common_FileName2FileType(_pcFileName) && 'H' != cDevType[0])
	{
		strcat((char *)ucMiddlePath, "/");
		strcat((char *)ucMiddlePath, (char *)cPath);
		//Í¼Æ¬ÎÄ¼þ´æ·ÅÔÚpicsÄ¿Â¼ÏÂ
		//ÌÕÓÀÁÁ 100120 Ê¹ÓÃÐÂµÄ·½Ê½ÅÐ¶ÏÎÄ¼þÀàÐÍ
	}

	//²¹È«È«Â·¾¶, Like /mnt/sata/0/0/20070802/H00CFT20070802113038C38.sdv
	strcpy((char *)ucFullPath, (char *)ucMiddlePath);
	strcat((char *)ucFullPath, "/");
	strcat((char *)ucFullPath, (char *)_pcFileName);

	//´«³ö
	if(_pcShortPath != NULL)
	{
		strcpy((char *)_pcShortPath, (char *)ucShortPath);
	}
	if(_pcPath != NULL)
	{
		strcpy((char *)_pcPath, (char *)ucMiddlePath);
	}
	if(_pcFullPath != NULL)
	{
		strcpy((char *)_pcFullPath, (char *)ucFullPath);
	}
	
	return ERR_REC_SUCCESSFUL;
}

#else
int Common_FileName2FileType(td_u8 * _pcFileName)
{
	int iRet = MAX_FILE_TYPE; 

	if(strstr((char *)_pcFileName, ".jpg"))
	{
		iRet = FILE_IMG;
	}
	else
	{
		if((char)_pcFileName[20] == 'B')
		{
			iRet = FILE_BACKUP;
		}
		else
		{
			iRet = FILE_REC;
		}
	}

	return iRet;
}

//ÅÐ¶ÏÎÄ¼þÃûÊÇ·ñºÏ·¨
td_s32 Common_IsValidFileName(td_u8* _pucFileName)
//Example:H00C0T20070802113C38.sdv
{
	if((_pucFileName[0] != 'U')&&
		(_pucFileName[0] != 'H')&&
		(_pucFileName[0] != 'N')&&
		(_pucFileName[0] != 'S'))
	{
		return 0;
	}
	
	if(_pucFileName[3] != 'C')
	{
		return 0;
	}
	if((_pucFileName[5] != 'M') && 
		(_pucFileName[5] != 'D') &&
		(_pucFileName[5] != 'P') &&
		(_pucFileName[5] != 'T') && 
		(_pucFileName[5] != 'L') &&
		(_pucFileName[5] != 'O') &&//20130409 ÍõÀÚadd
		(_pucFileName[5] != 'A') && //add by gyn
		(_pucFileName[5] != 'C'))	//ÌÕÓÀÁÁ 100127 ×Ô¶¨ÒåÂ¼ÏñÀàÐÍ
	{
		return 0;
	}
	
	//ÌÕÓÀÁÁ 091105 Ôö¼Ó±¸·ÝÂ¼ÏñÎªºÏ·¨Â¼Ïñ
	if (_pucFileName[20] != 'I' && 
		_pucFileName[20] != 'N' && 
		_pucFileName[20] != 'C' &&
		_pucFileName[20] != 'L' &&
		_pucFileName[20] != 'B')
	{
		return 0;
	}
	
	return 1;
}

//¸ù¾ÝÎÄ¼þÃû²¹È«È«Â·¾¶(¾ø¶ÔÂ·¾¶)
td_s32 Common_CompleteFileName(td_s8* _pcFileName, td_s8* _pcFullPath, td_s8* _pcPath, td_s8* _pcShortPath)
{
	//Example: /mnt/sata/0/0/20070802/H00CFT20070802113038C38.sdv
	// /dev/scsi/host00/part1/H00001DA000136140000C00.sdv
	td_s8 cDevType[4] = {0};					//´æ´¢Éè±¸ÀàÐÍ
	td_u8 cDevNo[4] = {0};						//´æ´¢Éè±¸ºÅ
	td_u8 cPartNo[4] = {0};						//´æ´¢Éè±¸·ÖÇøºÅ
	td_u8 cPath[16] = {0};						//ÈÕÆÚ

	//ÈýÖÖ³¤¶ÌµÄÂ·¾¶
	td_u8	ucShortPath[SHORT_PATH_LEN] = {0};
	td_u8	ucMiddlePath[MIDDLE_PATH_LEN] = {0};
	td_u8	ucFullPath[FULL_PATH_LEN] = {0};

	//td_printf_alone(0, "%s, %d", __func__, __LINE__);
	//sherry add 20101020
	if(strlen((char *)_pcFileName)>28)
	{
		if((_pcFileName[0]=='C')&&(Common_IsValidFileName((td_u8 *)(_pcFileName+2))))
		{
			sprintf((char *)ucFullPath,"/mnt/cdrom%c/%s",_pcFileName[1],_pcFileName+2);
			td_printf_alone(0,"%s %s %s",__func__,_pcFileName,ucFullPath);
			strcpy((char *)_pcFullPath, (char *)ucFullPath);
		}
		td_printf_alone(0, "%s,_pcPath[%s], _pcFullPath[%s]", __func__, cPath, _pcFullPath);
		return ERR_REC_SUCCESSFUL;
	}
//td_printf_alone(0, "%s, %d", __func__, __LINE__);
	if (_pcFileName == NULL || _pcFileName[0] == '\0' || !Common_IsValidFileName((td_u8 *)_pcFileName))
	{
		return ERR_REC_WRONG_PARAM;
	}
	strncpy((char *)cDevType, (char *)(_pcFileName + 0), 1);
	strncpy((char *)cDevNo, (char *)(_pcFileName + 1), 1);
	strncpy((char *)cPartNo, (char *)(_pcFileName + 2), 1);
	strncpy((char *)cPath, (char *)(_pcFileName + 6), 8);
	td_printf_alone(0,"FileName : %s", _pcFileName);
	td_printf_alone(0, "Common_CompleteFileName : cDevType = %s, cDevNo = %s, cPartNo = %s, cPath = %s",
		cDevType, cDevNo, cPartNo, cPath);

	//²¹È«¶ÌÂ·¾¶,Like  /mnt/sata/0/0
	switch(cDevType[0])
	{
		case 'H':
			strcpy((char *)ucShortPath, SATA_PATH);
			strcat((char *)ucShortPath, "/");
			strcat((char *)ucShortPath, (char *)cDevNo);
			strcat((char *)ucShortPath, "/");
			strcat((char *)ucShortPath, (char *)cPartNo);
			break;
		case 'U':
			strcpy((char *)ucShortPath, USB_PATH);
			strcat((char *)ucShortPath, "/");
			strcat((char *)ucShortPath, (char *)cDevNo);
			strcat((char *)ucShortPath, "/");
			strcat((char *)ucShortPath, (char *)cPartNo);
			break;
		case 'N':
			strcpy((char *)ucShortPath, NFS_PATH);
			break;
		case 'S':
			strcpy((char *)ucShortPath, SDCARD_PATH);
			strcat((char *)ucShortPath, "/");
			strcat((char *)ucShortPath, (char *)cDevNo);
			strcat((char *)ucShortPath, "/");
			strcat((char *)ucShortPath, (char *)cPartNo);
			break;
		default:
			td_printf_alone(0xFF0000, "%s : Wrong disk type : %c", __func__, cDevType[0]);
			return ERR_REC_WRONG_PARAM;
	}

	//²¹È«ÖÐÂ·¾¶,Like /mnt/sata/0/0/20070802
	strcpy((char *)ucMiddlePath, (char *)ucShortPath);
	//±¸·ÝÂ¼Ïñ¶ÎÂ·¾¶ºÍÖÐÂ·¾¶Ò»Ñù£¬Ã»ÓÐÈÕÆÚÊ±¼äÄ¿Â¼
	if (FILE_BACKUP != Common_FileName2FileType((td_u8 *)_pcFileName))
	{
		strcat((char *)ucMiddlePath, "/");
		strcat((char *)ucMiddlePath, (char *)cPath);
		//Í¼Æ¬ÎÄ¼þ´æ·ÅÔÚpicsÄ¿Â¼ÏÂ
		//ÌÕÓÀÁÁ 100120 Ê¹ÓÃÐÂµÄ·½Ê½ÅÐ¶ÏÎÄ¼þÀàÐÍ
		if (FILE_IMG == Common_FileName2FileType((td_u8 *)_pcFileName))
		{
			strcat((char *)ucMiddlePath, "/pics");
		}
	}

	//²¹È«È«Â·¾¶, Like /mnt/sata/0/0/20070802/H00CFT20070802113038C38.sdv
	strcpy((char *)ucFullPath, (char *)ucMiddlePath);
	strcat((char *)ucFullPath, "/");
	strcat((char *)ucFullPath, (char *)_pcFileName);

	//´«³ö
	if(_pcShortPath != NULL)
	{
		strcpy((char *)_pcShortPath, (char *)ucShortPath);
	}
	if(_pcPath != NULL)
	{
		strcpy((char *)_pcPath, (char *)ucMiddlePath);
	}
	
	if(_pcFullPath != NULL)
	{
		strcpy((char *)_pcFullPath, (char *)ucFullPath);
		td_printf_alone(0,"%s:_pcFullPath ===%s\n",__func__,_pcFullPath);
	}
	td_printf_alone(0, "%s, %s", __func__, cPath);
	return ERR_REC_SUCCESSFUL;
}
#endif
#endif

