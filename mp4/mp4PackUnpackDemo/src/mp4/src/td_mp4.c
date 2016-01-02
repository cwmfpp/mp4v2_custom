#ifdef FOR_MP4SDK
/*****************************************************************************
* NAME         : td_mp4.c
* FUNCTION     : MP4  ×ª»»»Ø·ÅÏà¹Ø
* PROGRAMMED   : º«ÓÂÇ¿
* DATE         : 2014-12-19
* PROJECT      : 
* OS           : Windows
****************************************************************************/
#include <WinSock2.h>
#include <windows.h>
#include <strsafe.h>
#include <io.h>
void PrtDbgInfo(int level,char *fmt, ...)
{
	if(level < 0)//´«Èë²ÎÊýÐèÒª´óÓÚµÈÓÚ1£¬·ñÔò²»´òÓ¡
	{
		return;
	}

	char out[1024];
	va_list body;
	va_start(body, fmt);
	vsprintf(out, fmt, body);     // Òë×¢£º¸ñÊ½»¯ÊäÈëµÄ×Ö·û´® fmtt
	va_end(body);                 //       µ½Êä³ö×Ö·û´® ou
	OutputDebugStringA(out);       // Òë×¢£ºÊä³ö¸ñÊ½»¯ºóµÄ×Ö·û´®µ½µ÷ÊÔÆ÷
}
#define __func__ __FUNCTION__
#pragma comment(lib, "libmp4v2")
#else
/*****************************************************************************
 * NAME         : td_mp4.c
 * FUNCTION     : MP4  ×ª»»»Ø·ÅÏà¹Ø
 * PROGRAMMED   : ÕÅÊ¤
 * DATE         : 2014-12-10
 * PROJECT      : 
 * OS           : Linux
 ****************************************************************************/
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <unistd.h>
#include <malloc.h>
#include <dirent.h>
#include <arpa/inet.h>
#endif

#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <sys/stat.h>

#include "libtiandy.h"
//#include "vlc_bits.h"
#include "libother.h"
#include "td_mp4.h"
#include "td_errno.h"
#include "list.h"

#ifdef MEMWATCH
#include "memwatch.h"
#endif

#include "mp4v2/mp4v2.h"

#include "log.h"//cwm_todo
#include "decode.h"

#define DECODE_BUFSIZE	(1024 * 128)

#define PLAY_CAP_BUF	(256*1024)
#define MAXBUF	(1024 * 1064)
#define MAXVSTREAM (MAXBUF - 64 * 1024)

#define MAX_BACK_NUM	4

#ifdef ALONE
#include "common.h"
#define MAXBUFEX (1024 * 1064)
#else
#include "decode.h"
#define MAXBUFEX decode_Get_MAXBUFEX()
#endif

#define MP4USLEEP(t)		do \
							{  \
								struct timeval tm = {0, t};  \
								select(0, NULL, NULL, NULL, &tm);\
							}while(0);

//------------mp4 µ÷ÊÔÐÅÏ¢¼ÇÂ¼
#define MP4LOG_FILE "/mnt/rtmsg/td_mp4.log"
#define MAX_MP4LOG_FILE_SIZE (256 * 1024)

static TMP4ConvertSession gstMP4ConvertSession[MAX_MP4_CONVSESSION_NUM];
static TMP4ResumSession   gstMP4ResumSession[MAX_MP4_CONVSESSION_NUM];
static MP4BigFileWirteBuffer gBigFile_WirteBuffer = NULL;
static MP4BigFileReadDisk    gBigFile_ReadDisk = NULL;

static void file_Check(FILE *_pFile)
{
	struct stat buf;
	FILE *pFile = _pFile;
	if(pFile == NULL)
	{
		return;
	}
	
	int ret = stat(MP4LOG_FILE, &buf);
	if(ret == 0)
	{
		if(buf.st_size >= MAX_MP4LOG_FILE_SIZE)
		{
			//Çå¿ÕÎÄ¼þ
			#ifdef CWM //FOR_MP4SDK cwm_todo
			if (0 == _chsize_s(fileno(pFile), _filelengthi64(fileno(pFile))))
			#else
			if(0 == ftruncate(fileno(pFile), 0))
			#endif
			{
				fseek(pFile, 0, SEEK_SET);
				fflush(pFile);
			}
			else
			{
				perror("ftruncate error!");
			}
		}
	}
}

int td_printf_mp4log(unsigned int handle, char* pszfmt, ...)
{	
	int ret;
	static FILE *file = NULL;
	static	pthread_mutex_t tMutex = PTHREAD_MUTEX_INITIALIZER;

#ifdef FOR_MP4SDK
	if(0 != access("MP4LOG", 0))
#else
	if(0 != access("MP4LOG", F_OK))
#endif
	{
		if(file != NULL)
		{
			fclose(file);
			file = NULL;
		}
		ret = -3;
		goto leave;
	}

#ifdef FOR_MP4SDK
	if(0 != access("MP4LOG", 0))
#else
	if(0 != access("MP4LOG", F_OK))
#endif
	{
		if(file != NULL)
		{
			fclose(file);
			file = NULL;
		}
	}

	if(file == NULL)
	{
		file = fopen(MP4LOG_FILE, "a+");

		if(NULL == file)
		{
			ret = -3;
			goto leave;
		}
	}

	pthread_mutex_lock(&tMutex);
	
	file_Check(file);
	
	va_list struAp;
	time_t now;
	struct tm *ptm_now;
	char* format= NULL;
	
	if(pszfmt == NULL) 
	{
		pthread_mutex_unlock(&tMutex);
		ret = -1;
		goto leave;
	}
	format = (char*)calloc(strlen(pszfmt)+3,1);
	if(format == NULL) 
	{
		pthread_mutex_unlock(&tMutex);
		ret = -2;
		goto leave;
	}
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
	ptm_now = localtime(&now);

#ifdef FOR_MP4SDK
	if(0 == handle)
	{
		fprintf(file,"[1;33m""[%04d/%02d/%02d %02d:%02d:%02d ]""[0;39m",
			ptm_now->tm_year+1900,
			ptm_now->tm_mon+1,
			ptm_now->tm_mday,
			(ptm_now->tm_hour)%24,
			ptm_now->tm_min,
			ptm_now->tm_sec);
	}
	else if(0xff0000 == handle)
	{
		fprintf(file,"[1;31m""[%04d/%02d/%02d %02d:%02d:%02d ]""[0;31m",
			ptm_now->tm_year+1900,
			ptm_now->tm_mon+1,
			ptm_now->tm_mday,
			(ptm_now->tm_hour)%24,
			ptm_now->tm_min,
			ptm_now->tm_sec);
	}
	else
	{
		fprintf(file,"[1;39m""[%04d/%02d/%02d %02d:%02d:%02d ]""[0;39m",
			ptm_now->tm_year+1900,
			ptm_now->tm_mon+1,
			ptm_now->tm_mday,
			(ptm_now->tm_hour)%24,
			ptm_now->tm_min,
			ptm_now->tm_sec);
	}
#else
	if(0 == handle)
	{
		fprintf(file,"[1;33m""[%04d/%02d/%02d %02d:%02d:%02d %d]""[0;39m",
			ptm_now->tm_year+1900,
			ptm_now->tm_mon+1,
			ptm_now->tm_mday,
			(ptm_now->tm_hour)%24,
			ptm_now->tm_min,
			ptm_now->tm_sec,
			(int)syscall(224));
	}
	else if(0xff0000 == handle)
	{
		fprintf(file,"[1;31m""[%04d/%02d/%02d %02d:%02d:%02d %d]""[0;31m",
			ptm_now->tm_year+1900,
			ptm_now->tm_mon+1,
			ptm_now->tm_mday,
			(ptm_now->tm_hour)%24,
			ptm_now->tm_min,
			ptm_now->tm_sec,
			(int)syscall(224));
	}
	else
	{
		fprintf(file,"[1;39m""[%04d/%02d/%02d %02d:%02d:%02d %d]""[0;39m",
			ptm_now->tm_year+1900,
			ptm_now->tm_mon+1,
			ptm_now->tm_mday,
			(ptm_now->tm_hour)%24,
			ptm_now->tm_min,
			ptm_now->tm_sec,
			(int)syscall(224));
	}
#endif
	      
	va_start(struAp, pszfmt);
	ret = vfprintf(file,format, struAp);
	va_end(struAp);
	fflush(file);
	free(format);
	pthread_mutex_unlock(&tMutex);
leave:
	return ret;
}
//----------------------------------------------------------------------------------

static int g_iForceStopFlag = 0;

//////////////////////////ÎÄ¼þ²Ù×÷ÏµÍ³º¯Êý////////////////////////////
void* td_mp4_fopen(void* _pFileParam, const char* _cFileName, char* _cMode)
{
	#ifdef TD_VFS
		return (void*) vfs_fopen((TTFileParam*)_pFileParam,_cFileName,_cMode);
	#else
		return (void*) fopen(_cFileName, _cMode);
	#endif
}
int td_mp4_fread(void *ptr, size_t size, size_t nmemb, void *stream)
{
	#ifdef TD_VFS
		return vfs_fread(ptr, size, nmemb, (TVfile *)stream);
	#else
		return fread(ptr, size, nmemb, (FILE *)stream);
	#endif
}
int td_mp4_fwrite(const void *ptr, size_t size, size_t nmemb, void *stream)
{
	#ifdef TD_VFS
		return vfs_fwrite(ptr, size, nmemb, (TVfile *)stream);
	#else
		return fwrite(ptr, size, nmemb, (FILE *)stream);
	#endif
}

#if 0//´óÎÄ¼þ²»ºÏÊÊ
int td_mp4_fseek(void *stream, long offset, int whence)
{
	#ifdef TD_VFS
		return vfs_fseek((TVfile *)stream, offset, whence);
	#else
		return fseek((FILE*)stream, offset, whence);
	#endif
}
#endif

#ifdef  CWM //FOR_MP4SDK cwm_todo
int td_mp4_fseek(void *stream, __int64 offset, int whence)
{
#ifdef TD_VFS
	return vfs_fseek((TVfile *)stream, offset, whence);
#else
	return _fseeki64((FILE*)stream, offset, whence);
#endif
}
#else
int td_mp4_fseek(void *stream, off_t offset, int whence)
{
	#ifdef TD_VFS
		return vfs_fseek((TVfile *)stream, offset, whence);
	#else
		return fseeko((FILE*)stream, offset, whence);
	#endif
}
#endif

#if 0//´óÎÄ¼þ²»ºÏÊÊ
long td_mp4_ftell(void *stream)
{
	#ifdef TD_VFS
		return vfs_ftell((TVfile*)stream);
	#else
		return ftell((FILE*)stream);
	#endif
}
#endif

#ifdef CWM //FOR_MP4SDK cwm_todo
__int64 td_mp4_ftell(void *stream)
{
#ifdef TD_VFS
	return vfs_ftell((TVfile*)stream);
#else
	return _ftelli64((FILE*)stream);
#endif
}
#else
off_t td_mp4_ftell(void *stream)
{
	#ifdef TD_VFS
		return vfs_ftell((TVfile*)stream);
	#else
		return ftello((FILE*)stream);
	#endif
}
#endif
int td_mp4_feof(void *stream)
{
	#ifdef TD_VFS
		return vfs_feof((TVfile*)stream);
	#else
		return feof((FILE*)stream);
	#endif
}
int td_mp4_fclose(void *fp)
{
	#ifdef TD_VFS
		return vfs_fclose((TVfile *)fp);
	#else
		return fclose((FILE*)fp);
	#endif
}
int td_mp4_stat(char* _cFileName, void*_buf)
{
	#ifdef TD_VFS
		return vfs_Stat(_cFileName,(TTdStat *)_buf);
	#else
		return stat(_cFileName, (struct stat*)_buf);
	#endif
}

int td_mp4_unlink(char* _cFileName)
{
	#ifdef TD_VFS
		return vfs_Unlink(_cFileName);
	#else
		return unlink(_cFileName);
	#endif
}

#ifdef CWM//FOR_MP4SDK cwm_todo
//add for windows 
int code_convert(int from_charset, int to_charset,char *inbuf, int inlen, char *outbuf, int outlen )
{
	int wclen = 0;
	int mblen = 0;
	wchar_t wchar[MAX_WIDE_CHAR_NUM];
	wclen = MultiByteToWideChar(from_charset,0,inbuf,inlen,wchar,MAX_WIDE_CHAR_NUM); 
	if (wclen <= 0)
	{
		return -1;
	}

	memset(outbuf, 0, outlen); 
	mblen = WideCharToMultiByte(to_charset,0,wchar,wclen,outbuf,outlen,NULL,NULL); 
	return mblen;
}

int AnsiToUTF8( char* _pcInbuf, int _iInlen, char *_pcOutbuf, int _iOutlen )
{
	//return code_convert(CP_ACP,CP_UTF8,_pcInbuf,_iInlen,_pcOutbuf,_iOutlen);//cwm_todo
}
//add end
#endif

void printfdataX(char* _pinfo, char* _pData, int _iLen)
{
	#ifdef TD_DEBUG
	int i = 0;

	td_printf(0, "\n\n>>>>>>>[%s]:len[%d]", _pinfo, _iLen);
	
	if(NULL != _pData)
	{
		for(i = 0; i < _iLen; i++)
		{
			if((0 == (i+1)%30)&&(0 != i))
			{
				printf(" %02x\n", _pData[i]);
			}
			else if((_iLen - 1) == i)
			{
				printf(" %02x\n\n", _pData[i]);
			}
			else
			{
				printf(" %02x", _pData[i]);
			}
		}
	}
	#endif
}

static void ConvertSort(uint8_t* uiBuf, uint32_t uiValue)
{
    uiBuf[0] = (uiValue >> 24) & 0xFF;
    uiBuf[1] = (uiValue >> 16) & 0xFF;
    uiBuf[2] = (uiValue >> 8) & 0xFF;
    uiBuf[3] = uiValue & 0xFF;
}

static void ConvertSortLong(uint8_t* _ullBuf, uint64_t _ullValue)
{
    _ullBuf[0] = (_ullValue >> 56) & 0xFF;
    _ullBuf[1] = (_ullValue >> 48) & 0xFF;
    _ullBuf[2] = (_ullValue >> 40) & 0xFF;
    _ullBuf[3] = (_ullValue >> 32) & 0xFF;
    _ullBuf[4] = (_ullValue >> 24) & 0xFF;
    _ullBuf[5] = (_ullValue >> 16) & 0xFF;
    _ullBuf[6] = (_ullValue >> 8) & 0xFF;
    _ullBuf[7] = _ullValue & 0xFF;
}

#if 0
static int Inner_GetAudioPoiNum(int _iType,int _iFramLen)
{
	td_s32 iPtNumPerFrm= -1;	
//	td_printf(0, "%s    _iType:%d _iFramLen:%d\n", __FUNCTION__,_iType,_iFramLen);
   	switch(_iType & 0xFF)
	{
		case 0x03:	//ADPCM_DIV4
    		{
			if (_iFramLen == 48)
			{
				iPtNumPerFrm = 80;
			}
			else if (_iFramLen == 168)
			{
				iPtNumPerFrm = 320;
			}
			else if (_iFramLen == 248)
			{
				iPtNumPerFrm = 480;
			}
			break;
    		}
		case 0x23:	//ADPCM_IMA
		{
			if (_iFramLen == 46)
			{
				iPtNumPerFrm = 80;
			}
			else if (_iFramLen == 166)
			{
				iPtNumPerFrm = 320;
			}
			break;
    		}

		case 0x01:	//G.711A
		{
			if (_iFramLen == 84)
			{
				iPtNumPerFrm = 80;
			}
			else if (_iFramLen == 324)
			{
				iPtNumPerFrm = 320;
			}
 			else if (_iFramLen == 484)
			{
				iPtNumPerFrm = 480;
			}
			break;			
		}
		case 0x02:	//G.711U
		{
			if (_iFramLen == 84)
			{
				iPtNumPerFrm = 80;
			}
			else if (_iFramLen == 324)
			{
				iPtNumPerFrm = 320;
			}
			else if (_iFramLen == 484)
			{
				iPtNumPerFrm = 480;
			}
			break;			
		}
		case 0x05:   //G726_24KBPS
		{
			if (_iFramLen == 34)
			{
				iPtNumPerFrm = 80;
			}
			else if (_iFramLen == 124)
			{
				iPtNumPerFrm = 320;
			}
			break;
		}
		case 0x06:   //G726_32KBPS
		{
			if (_iFramLen == 44)
			{
				iPtNumPerFrm = 80;
			}
			else if (_iFramLen == 164)
			{
				iPtNumPerFrm = 320;
			}
 
			break;	
		}
		case 0x07:   //G726_40KBPS
		{
			if (_iFramLen == 54)
			{
				iPtNumPerFrm = 80;
			}
			else if (_iFramLen == 204)
			{
				iPtNumPerFrm = 320;
			}
			break;	
		}
		case 0x24:    //MEDIA_G726_16KBPS
		{
			if (_iFramLen == 24)
			{
				iPtNumPerFrm = 80;
			}
			else if (_iFramLen == 84)
			{
				iPtNumPerFrm = 320;
			}
			break;	
		}

		case 0x25:   //MEDIA_G726_24KBPS
		{
			if (_iFramLen == 34)
			{
				iPtNumPerFrm = 80;
			}
			else if (_iFramLen == 124)
			{
				iPtNumPerFrm = 320;
			}
			break;	
		}

		case 0x26:   //MEDIA_G726_32KBPS
		{
			if (_iFramLen == 44)
			{
				iPtNumPerFrm = 80;
			}
			else if (_iFramLen == 164)
			{
				iPtNumPerFrm = 320;
			}
			break;	
		}
	
		case 0x27:   //MEDIA_G726_40KBPS
		{
			if (_iFramLen == 54)
			{
				iPtNumPerFrm = 80;
			}
			else if (_iFramLen == 204)
			{
				iPtNumPerFrm = 320;
			}
			break;	
		}
		case 0x15:	//AMR
		{
			if (_iFramLen == 32)
			{
				iPtNumPerFrm = 80;
			}
			else if (_iFramLen == 64)
			{
				iPtNumPerFrm = 160;
			}
			break;
		}
		case 0x16:	//AAC
		{
			iPtNumPerFrm = 1024;
            break;
		}
		// add end
		default:
		{
			iPtNumPerFrm = -1;		
			break;
		}
	}
	//td_printf(0x12345678, "%s(%d):iPtNumPerFrm=%d", __func__, __LINE__, iPtNumPerFrm);
	
 	return iPtNumPerFrm;
}
#endif

static int Inner_extract_file_name(unsigned char* _pucFullPath)
{
    //Example: /mnt/sata/0/0/20070802/H00C00T20070802113C38.h264
    int iLen = 0;
    int i = 0;
    int iPos = 0;
	int iSize;
    signed char cTemp[256];

	iLen = strlen((char *)_pucFullPath);
	if (iLen >= 256)
    {
        return -1;
    }

	strcpy((char *)cTemp, (char *)_pucFullPath);
	for(i = 0;i < iLen; i++)
	{
		if(_pucFullPath[i] == '/')
		{
			iPos = i;
		}
	}
	memset(_pucFullPath,0,iLen);
	iSize = iLen - iPos - 1;
	if (iSize > 0)
	{
	memcpy(_pucFullPath, cTemp + iPos + 1, iLen - iPos -1);
	}
	else
	{
		_pucFullPath[0] = 0;
	}
	return 0;
}

static int Inner_StrToTime(const char* _strTime)
{
	struct tm tCurTime;
	sscanf(_strTime, "%4d%2d%2d%2d%2d%2d", &tCurTime.tm_year, &tCurTime.tm_mon, &tCurTime.tm_mday,
		       &tCurTime.tm_hour, &tCurTime.tm_min, &tCurTime.tm_sec);
	tCurTime.tm_year -= 1900;
	tCurTime.tm_mon--;
	//add by ssn 2010/12/09
	tCurTime.tm_isdst = 0;
	//add end 
	int iTime = mktime(&tCurTime);
	return iTime;
}

//////////////////////////////////////////////////////////////////////////////

//MP4--ZS
static td_s32 Inner_BuildAACHeader(unsigned int _uiAudio_sample, unsigned int _iLen, unsigned char *_ptAAC_Header)
{
	unsigned char* AAC_Header = _ptAAC_Header;
	unsigned char AAC_Block = 0;
	int iAudioLen = 0;
	
	// ´¿ÂëÁ÷+ AACÒôÆµÍ·
	iAudioLen = _iLen;

	if(NULL != AAC_Header)
	{
		AAC_Header[0] = 0xff; // 11111111
		AAC_Header[1] = 0xf1; // 1111, ID_MPEG:0, layer:00, protection:1
		//AAC_Header[2] ----  profile:01, frequency:1011, private:0, channel[2]:0
		switch(_uiAudio_sample)
		{
			case 96000: // frequency:0000
				AAC_Header[2] = 0x40;
				break;
			case 88200: // frequency:0001
				AAC_Header[2] = 0x44;
				break;
			case 64000: // frequency:0010
				AAC_Header[2] = 0x48;
				break;
			case 48000: // frequency:0011
				AAC_Header[2] = 0x4c;
				break;
			case 44100: // frequency:0100
				AAC_Header[2] = 0x50;
				break;
			case 32000: // frequency:0101
				AAC_Header[2] = 0x54;
				break;
			case 24000: // frequency:0110
				AAC_Header[2] = 0x58;
				break;
			case 22050: // frequency:0111
				AAC_Header[2] = 0x5c;
				break;
			case 16000: // frequency:1000
				AAC_Header[2] = 0x60;
				break;
			case 2000: // frequency:1001
				AAC_Header[2] = 0x64;
				break;
			case 11025: // frequency:1010
				AAC_Header[2] = 0x68;
				break;
			case 8000: // frequency:1011
				AAC_Header[2] = 0x6c;
				break;
			default: // 8k--frequency:1011
				AAC_Header[2] = 0x6c;
				break;
		}
		
		//AAC_Header[2] = 0x4c; // 48k ÁÙÊ±Ð´ËÀ
		
		AAC_Header[3] = 0x80; // channel[1:0]:01, original:0, home:0, bit:0, start:0, XX
		AAC_Header[3] |= (iAudioLen & 0x1800) >> 11;
		AAC_Header[4] = (iAudioLen & 0x07f8) >> 3;
		AAC_Header[5] = (iAudioLen & 0x07) << 5;
		AAC_Header[5] |= 0x1f; // XXX, 11111
		AAC_Header[6] = 0xfc; // 111111, XX
		AAC_Block = iAudioLen / 1024;
		AAC_Header[6] |= AAC_Block & 0x03;

		#if 0
		int i = 0; 
		printf("%s, %d, sample is %d, AAC_Header = ", __func__, __LINE__, _uiAudio_sample);
		for(i = 0; i < 7; i++)
		{
			printf(" %2x",AAC_Header[i]);
		}
		printf("\n");
		#endif
	}
	else
	{
		td_printf(0xff0000, "%s failed! the buf is null!\n", __FUNCTION__);
		return td_failure;
	}
	
	return td_success;
}

static td_s32 Inner_BuildFreamHeader(S_FrameHeader* _ptHeader, bool _IFrameflag, MP4SampleId _sampleId,
							uint64_t _vedio_time_scale_sum, uint32_t _sample_size_vedio, int _iEncyptFlag )
{
	S_FrameHeader *ptHeader = _ptHeader;

	if(NULL == ptHeader)
	{
		return td_failure;
	}
	ptHeader->u32FrameID	= ntohl(0x00000002);
	ptHeader->u8FrameType	= (_IFrameflag == true ? 0:1);
	ptHeader->u32TimeStamp	= (unsigned int) _vedio_time_scale_sum;
	ptHeader->u32VStreamLen = (unsigned int) _sample_size_vedio;
	//ptHeader->u8ACoder		= AUDIO_TYPE_MEDIA_AAC;

	if(1 == _iEncyptFlag)
	{
		ptHeader->reserved = 0xfff2;
	}
	else
	{
		ptHeader->u32FrameNO = _sampleId;
	}

	//td_printf(0x12345678, "===============>ptHeader->u32FrameNO is %d", ptHeader->u32FrameNO);
	
	#if 0
	if(ptHeader->u8ACoder == AUDIO_TYPE_MEDIA_AAC)
	{
		if(audioframesize > 0)
		{
			ptHeader->u16AFrames	= 1;	//AACÒôÆµÒ»Ö¡
			ptHeader->u16AFrameSize = audioframesize;	//7×Ö½ÚÍ· + ´¿ÒôÆµÊý¾Ý
		}
		else
		{
			ptHeader->u16AFrames	= 0;
			ptHeader->u16AFrameSize = 0;
		}
	}
	#endif

	ptHeader->u32FrameSize	= sizeof(S_FrameHeader) + ptHeader->u32VStreamLen + ptHeader->u16AFrames * ptHeader->u16AFrameSize;
#if 0
	{
		td_printf(0, "==========%s==========", __FUNCTION__);
		td_printf(0, "u32FrameID:%x",		ptHeader->u32FrameID); 
		td_printf(0, "u32FrameSize:%u",	ptHeader->u32FrameSize);	
		td_printf(0, "u8FrameType:%u", 	ptHeader->u8FrameType); 
		td_printf(0, "u32FrameNO:%u",		ptHeader->u32FrameNO); 
		td_printf(0, "u32TimeStamp:%u",	ptHeader->u32TimeStamp); 
		td_printf(0, "u32VStreamLen:%u",	ptHeader->u32VStreamLen); 
		td_printf(0, "u16AFrames:%u",		ptHeader->u16AFrames); 
		td_printf(0, "u16AFrameSize:%u",	ptHeader->u16AFrameSize); 
		td_printf(0, "u8ACoder:%u",		ptHeader->u8ACoder);
		td_printf(0, "reserved:%d", ptHeader->reserved);
		td_printf(0, "====================");
	}
#endif
	return td_success;
}


static td_s32 Inner_ReadOneAacSample(MP4FileHandle _mp4File, MP4TrackId _audioTrackId, MP4SampleId _audiosampleId, 
                    OUT uint8_t *_sample_audio, uint32_t* _sample_size_audio, MP4Duration* _sampleDuration_audio, uint8_t* ptAacHeader)
{
	if((NULL == _mp4File)||(NULL == _sample_audio)||(NULL == _sampleDuration_audio)||(NULL == ptAacHeader))
	{
		goto leave;
	}
	uint32_t ui_Audio_sample = MP4GetTrackTimeScale(_mp4File, _audioTrackId);
		
	MP4Timestamp sampleTime_audio;
	MP4Duration sampleRenderingOffset_audio;
	bool isSyncSample_audio;
	bool ret_audio = false;

	//td_printf(0, "*_sample_size_audio is %d, _audioTrackId is %d", *_sample_size_audio, _audioTrackId);

	ret_audio = MP4ReadSample(_mp4File,
							 _audioTrackId,
							 _audiosampleId,
							 &_sample_audio,
							 _sample_size_audio,
						     &sampleTime_audio,
							 _sampleDuration_audio,
							 &sampleRenderingOffset_audio,
							 &isSyncSample_audio);
	if(ret_audio == false)
	{
		td_printf(0xff0000, "Read MP4_file_audio failed!");
		goto leave;
	}
	
	Inner_BuildAACHeader(ui_Audio_sample, *_sample_size_audio + 7, ptAacHeader);
	
	return td_success;
leave:
	return td_failure;
}

static td_s32 Inner_ReadOneNormalAudioSample(MP4FileHandle _mp4File, MP4TrackId _audioTrackId, MP4SampleId _audiosampleId, 
                    OUT uint8_t *_sample_audio, uint32_t* _sample_size_audio, MP4Duration* _sampleDuration_audio, uint8_t* ptHeader)
{
	if((NULL == _mp4File)||(NULL == _sample_audio)||(NULL == _sampleDuration_audio)||(NULL == ptHeader))
	{
		goto leave;
	}
		
	MP4Timestamp sampleTime_audio;
	MP4Duration sampleRenderingOffset_audio;
	bool isSyncSample_audio;
	bool ret_audio = false;

	//td_printf(0, "%s _audioTrackId is %d", __FUNCTION__, _audioTrackId);
	ret_audio = MP4ReadSample(_mp4File,
							 _audioTrackId,
							 _audiosampleId,
							 &_sample_audio,
							 _sample_size_audio,
						     &sampleTime_audio,
							 _sampleDuration_audio,
							 &sampleRenderingOffset_audio,
							 &isSyncSample_audio);
	if(ret_audio == false)
	{
		td_printf(0xff0000, "Read MP4_file_audio failed!");
		goto leave;
	}

	ptHeader[2] = (*_sample_size_audio)/2;
	
	return td_success;
leave:
	return td_failure;
}

static td_s32 Inner_GetAudioSamples(MP4FileHandle _pMP4fp, uint64_t _llVideoTimeStamp, 
		MP4TrackId _AudioTrackId, MP4SampleId* _puiAudioSampleId, int _iAudioType, 
			OUT uint8_t* _pucAudiobuf, IN uint32_t _uiAudioLen, OUT S_FrameHeader* _pstFrameheader, int _iEncrypt)
{
	int iRet = td_success;

	if((NULL == _pMP4fp)||(MP4_INVALID_TRACK_ID == _AudioTrackId)||(NULL == _puiAudioSampleId)
		||(NULL == _pucAudiobuf)||(NULL == _pstFrameheader))
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}

	if(*_puiAudioSampleId <= 0)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}

	uint64_t audio_timestamp = 0;
	uint32_t sample_size_audio = MAX_AUDIO_SAMPLE_SIZE;
	uint8_t sample_audio[MAX_AUDIO_SAMPLE_SIZE];
	uint32_t audioframesize = 0;
	MP4Duration sampleDuration_audio = 0;

	MP4SampleId numSamples_audio = MP4GetTrackNumberOfSamples(_pMP4fp, _AudioTrackId);
	uint32_t    uAudio_sample = MP4GetTrackTimeScale(_pMP4fp, _AudioTrackId);

	_pstFrameheader->u16AFrames = 0;

	if(AUDIO_TYPE_MEDIA_AAC == _iAudioType)
	{
		uint8_t AacHeader[7] = {0};

		while((audio_timestamp < _llVideoTimeStamp)&&(*_puiAudioSampleId <= numSamples_audio))
		{
			if(Inner_ReadOneAacSample(_pMP4fp,_AudioTrackId,*_puiAudioSampleId,
					sample_audio,&sample_size_audio,&sampleDuration_audio,AacHeader))
			{
				break;
			}
		
			if(audioframesize + sizeof(AacHeader) + sample_size_audio <= _uiAudioLen)
			{
				memcpy(_pucAudiobuf+audioframesize, AacHeader, sizeof(AacHeader));
				audioframesize += sizeof(AacHeader);

				if(_iEncrypt)
				{
					if(sample_size_audio > 0)
					{
						sample_audio[0] ^= 0xab;//AAC¼òµ¥½âÃÜËã×Ó
					}
				}
				
				//fwrite(sample_audio, 1, sample_size_audio, AudioFile_Fan);
				memcpy(_pucAudiobuf+audioframesize, sample_audio, sample_size_audio);
				audioframesize += sample_size_audio;
						
				audio_timestamp = (MP4GetSampleTime(_pMP4fp,_AudioTrackId,*_puiAudioSampleId)*1000/uAudio_sample);
				(*_puiAudioSampleId)++;//ÒôÆµÖ¡µÝÔö
				sample_size_audio = MAX_AUDIO_SAMPLE_SIZE;
			}
			else
			{
				break;
			}

			//td_printf(0, "audio_timestamp is %lld, _llVideoTimeStamp is %lld, uAudio_sample is %d", 
				//audio_timestamp, _llVideoTimeStamp, uAudio_sample);
		}

		_pstFrameheader->u16AFrames = 1;
		_pstFrameheader->u16AFrameSize = audioframesize;
	}	
	else if((AUDIO_TYPE_G711A == _iAudioType)||(AUDIO_TYPE_G711U == _iAudioType)
		||(AUDIO_TYPE_DVI4 == _iAudioType))
	{
		unsigned char ucHeader[4] = {0x00,0x01,0xa0,0x00};

		while((audio_timestamp < _llVideoTimeStamp)&&(*_puiAudioSampleId <= numSamples_audio))
		{
			if(Inner_ReadOneNormalAudioSample(_pMP4fp,_AudioTrackId,*_puiAudioSampleId,
					sample_audio,&sample_size_audio,&sampleDuration_audio,ucHeader))
			{
				break;
			}
		
			if(audioframesize + sizeof(ucHeader) + sample_size_audio <= _uiAudioLen)
			{
				memcpy(_pucAudiobuf+audioframesize, ucHeader, sizeof(ucHeader));
				audioframesize += sizeof(ucHeader);
				memcpy(_pucAudiobuf+audioframesize, sample_audio, sample_size_audio);
				audioframesize += sample_size_audio;

				if(0 == _pstFrameheader->u16AFrameSize)
				{
					_pstFrameheader->u16AFrameSize = sample_size_audio + sizeof(ucHeader);
				}
				else if(_pstFrameheader->u16AFrameSize != (sample_size_audio + sizeof(ucHeader)))
				{
					td_printf(0xff0000, "the audio frame size error, u16AFrameSize is %d, realsize is %d",
						_pstFrameheader->u16AFrameSize, sample_size_audio + sizeof(ucHeader));
					iRet = td_failure;
					goto leave;
				}
						
				audio_timestamp = (MP4GetSampleTime(_pMP4fp,_AudioTrackId,*_puiAudioSampleId)*1000/uAudio_sample);
				(*_puiAudioSampleId)++;//ÒôÆµÖ¡µÝÔö
				(_pstFrameheader->u16AFrames)++;
				//td_printf(0, "sample_size_audio is %d, audio_timestamp is %lld, _llVideoTimeStamp is %lld, uAudio_sample is %d", 
					//sample_size_audio, audio_timestamp, _llVideoTimeStamp, uAudio_sample);
				sample_size_audio = MAX_AUDIO_SAMPLE_SIZE;
			}
			else
			{
				td_printf(0xff0000, "the data sum is %d, _uiAudioLen is %u",
					audioframesize + sizeof(ucHeader) + sample_size_audio, _uiAudioLen);
				break;
			}
		}
	}
	else
	{
		td_printf(0xff0000, "%s %s %d error! the audio type [%d] is not support!", __FILE__, __FUNCTION__, __LINE__, _iAudioType);
		iRet = td_failure;
		goto leave;
	}

	_pstFrameheader->u8ACoder = _iAudioType;
	
leave:

	return iRet;
}

static td_s32 Inner_ReadOneVideoSample(MP4FileHandle _mp4File, MP4TrackId _vedioTrackId, MP4SampleId _vediosampleId,  
                    OUT uint8_t *_sample_vedio, uint32_t* _sample_size_vedio, MP4Duration* _sampleDuration_vedio, bool* _isSyncSample_vedio)
{
	if((NULL == _mp4File)||(NULL == _sample_vedio)||(NULL == _sample_size_vedio)||(NULL == _sampleDuration_vedio)||(NULL == _isSyncSample_vedio))
	{
		goto leave;
	}
			
	MP4Timestamp sampleTime_vedio;
	MP4Duration sampleRenderingOffset_vedio;
	bool ret_vedio = false;


	ret_vedio = MP4ReadSample( _mp4File,
							   _vedioTrackId,
							   _vediosampleId,
							   &_sample_vedio,
							   _sample_size_vedio,
							   &sampleTime_vedio,
							   _sampleDuration_vedio,
							   &sampleRenderingOffset_vedio,
							   _isSyncSample_vedio);

	if(ret_vedio == false)
	{
		td_printf(0xff0000, "Read MP4_file_video failed!");
		goto leave;
	}

	return td_success;
leave:

	return td_failure;
}

static td_s32 Inner_RecordFileBeginTime(td_s8*_strFile, int* _iFileBeginTime)
{
	int iRet = td_failure;
	int iBeginTime = -1;

	if((NULL == _strFile)||(NULL == _iFileBeginTime))
	{
		td_printf(0xff0000,"%s %s %d error!",__FILE__, __FUNCTION__, __LINE__);
		goto leave;
	}
	
	td_u8 cTmpFullPath[FULL_PATH_LEN] = {0};
	char cTmpShort[SHORT_PATH_LEN] = {0};
	*_iFileBeginTime = 0;
	if(strlen((char *)_strFile) <= 29)
	{//cwm_todo
		if (0/*rec_CompleteFileName((td_s8 *)_strFile, (td_s8 *)cTmpFullPath, NULL, NULL) != ERR_REC_SUCCESSFUL*/)
		{
			td_printf(0xff0000,"%s %s %d error!",__FILE__, __FUNCTION__, __LINE__);
			goto leave;
		}
	}
	else
	{
		strncpy((char*)cTmpFullPath, (char*)_strFile, FULL_PATH_LEN - 1);
	}

	td_printf(0, "cTmpFullPath before is %s", cTmpFullPath);
	Inner_extract_file_name((unsigned char *)cTmpFullPath);
	td_printf(0, "cTmpFullPath after is %s", cTmpFullPath);
	memcpy(cTmpShort,cTmpFullPath+6,14);	
	td_printf(0, "cTmpShort is %s", cTmpShort);
	iBeginTime = Inner_StrToTime(cTmpShort);

	if(iBeginTime < 0)
	{
		td_printf(0xff0000,"%s %s %d error!",__FILE__, __FUNCTION__, __LINE__);
		goto leave;
	}
	else
	{
		*_iFileBeginTime = iBeginTime;
	}

	iRet = td_success;
leave:
	return iRet;
}


static td_s32 Inner_RecordFileEndTime(td_s8*_strFile, int* _iFileEndTime)
{
	int iRet = td_success;

	if((NULL == _strFile)||(NULL == _iFileEndTime))
	{
		iRet = td_failure;
		goto leave;
	}
	
#ifndef ALONE
	if(ERR_REC_SUCCESSFUL != rec_GetFileStopTime(_strFile, (time_t*)_iFileEndTime))
	{
		iRet = td_failure;
		goto leave;
	}
#else
#ifdef TD_VFS
	TTdStat stStat;
#else
	struct stat stStat;
#endif
	td_u8 cTmpFullPath[FULL_PATH_LEN] = {0};
	*_iFileEndTime = 0;
	int iStatret = -1;

	if(strlen((char *)_strFile) <= 29)
	{
		if (0/*rec_CompleteFileName((td_s8 *)_strFile, (td_s8 *)cTmpFullPath, NULL, NULL) != ERR_REC_SUCCESSFUL*/)//cwm_todo
		{
			td_printf(0xff0000,"%s %s %d error!",__FILE__, __FUNCTION__, __LINE__);
			iRet = td_failure;
			goto leave;
		}
	}
	else
	{
		strncpy((char*)cTmpFullPath, (char*)_strFile, FULL_PATH_LEN - 1);
	}

	td_printf(0, "cTmpFullPath is %s", cTmpFullPath);

	//iStatret = rec_Stat((char *)cTmpFullPath, &stStat);//cwm_todo

	if (iStatret != td_success)
	{
		td_printf(0xff0000,"%s %s %d error! iStatret is %d",__FILE__, __FUNCTION__, __LINE__, iStatret);
		iRet = td_failure;
		goto leave;
	}

#ifdef TD_VFS
	*_iFileEndTime = stStat.m_stat.st_mtime;
#else
	*_iFileEndTime = stStat.st_mtime;
#endif
#endif
leave:
	return iRet;
}

////////////////////////////////////////////////////////////////////////////////////////////////////combine begin
/************************************************* 
  Function: Inner_GetRealValue
  Description: »ñÈ¡Êµ¼Ê¶þ½øÖÆµÄÖµ
  Input: _pucSrcData £¬´ý·ÖÎöÔ´
  Input: _iStartPos £¬ËùÔÚÎ»ÖÃ
  Input: _iBitCount£¬¶þ½øÖÆ¸öÊý
  Output: 
  Return: º¯ÊýÖ´ÐÐ³É¹¦·µ»Ø Êµ¼ÊÖµ, Ê§°Ü·µ»Ø td_failure
  Author: »ÆÖ¾¸Õ(Huangzhigang)
  Others: 
*************************************************/
static int Inner_GetRealValue(unsigned char *_pucSrcData, int _iStartPos, int _iBitCount)
{
	int ret=0;
	int i = 0;
	int iBit = 0;
	int iIndex = 0;
	int iOffset = 0;
	int iThisFunRes = td_failure;

	if((NULL == _pucSrcData))
	{
		td_printf(0, "%d: Input parameters is error!\n", __LINE__);
		iThisFunRes = td_failure;
		goto LABLE_TD_MP4_EXIT;
	}

	unsigned char *pucSrcData = _pucSrcData;
	int iStartPos = _iStartPos;
	int iBitCount = _iBitCount;
	
	for(i=0; i<iBitCount; i++)
	{
		iBit = iStartPos+i;
		iIndex = iBit/8;
		iOffset = iBit%8;
		ret = ret << 1;
		ret += (pucSrcData[iIndex]&(1<<(7-iOffset)))>0?1:0;
	}

	iThisFunRes = ret;
	
LABLE_TD_MP4_EXIT:
	return iThisFunRes; 
}



/************************************************* 
  Function: Inner_AnalyzeAdtsHead
  Description: ½âÎöADTS Í·
  Input: _pucAacData £¬´ý·ÖÎöÔ´
  Input: _pucAacData £¬´ý·ÖÎöÔ´³¤¶È
  Output: _pstAdtsHead£¬½«½âÎöºóµÄÊý¾Ý·Åµ½¸Ã²ÎÊýÖÐ
  Return: º¯ÊýÖ´ÐÐ³É¹¦·µ»Ø td_success, Ê§°Ü·µ»Ø td_failure
  Author: »ÆÖ¾¸Õ(Huangzhigang)
  Others: 
*************************************************/
static int Inner_AnalyzeAdtsHead(TADTSHeader* _pstAdtsHead, unsigned char *_pucAacData, int _iAacDataLen)
{
	int iThisFunRes = td_failure;
	unsigned char* pucAacData = NULL;
	TADTSHeader* pstAdtsHead = NULL;

	if((NULL==_pstAdtsHead) || (NULL==_pucAacData) || (_iAacDataLen<ADTS_HEADER_LENGTH) )
	{
		td_printf(0xff0000, "%d: Input parameters is error!\n", __LINE__);
		iThisFunRes = td_failure;
		goto LABLE_TD_MP4_EXIT;
	}
	
	pucAacData = _pucAacData;
	pstAdtsHead = _pstAdtsHead;

	pstAdtsHead->synword = Inner_GetRealValue(pucAacData, 0, 12);							//0~11		12 bslbf
	if(pstAdtsHead->synword != 0xFFF)
	{
		td_printf(0xff0000, "%d: Aac flag is error!\n", __LINE__);
		iThisFunRes = td_failure;
		goto LABLE_TD_MP4_EXIT;
	}
	pstAdtsHead->ID = Inner_GetRealValue(pucAacData, 12, 1);									//12			1  bslbf
	pstAdtsHead->layer = Inner_GetRealValue(pucAacData, 13, 2);								//13~14		2  uimsbf
	pstAdtsHead->protection_absent = Inner_GetRealValue(pucAacData, 15, 1);					//15			1  bslbf
	pstAdtsHead->profile_ObjectType = Inner_GetRealValue(pucAacData, 16, 2);					//16~17		2  uimsbf
	pstAdtsHead->sampling_frequency_index = Inner_GetRealValue(pucAacData, 18, 4);			//18~21		4  uimsbf
	pstAdtsHead->private_bit = Inner_GetRealValue(pucAacData, 22, 1);						//22			1  bslbf
	pstAdtsHead->channel_configuration = Inner_GetRealValue(pucAacData, 23, 3);				//23~25		3  uimsbf
	pstAdtsHead->original_copy = Inner_GetRealValue(pucAacData, 26, 1);						//26			1  bslbf	
	pstAdtsHead->home = Inner_GetRealValue(pucAacData, 27, 1);						//27			1  bslbf	
	pstAdtsHead->copyright_identification_bit = Inner_GetRealValue(pucAacData, 28, 1);			//28			1  bslbf
	pstAdtsHead->copyright_identification_start = Inner_GetRealValue(pucAacData, 29, 1);		//29			1  bslbf
	pstAdtsHead->aac_frame_length = Inner_GetRealValue(pucAacData, 30, 13);					//30~42		13 bslbf
	pstAdtsHead->adts_buffer_fullness = Inner_GetRealValue(pucAacData, 33, 11);				//33~53		11 bslbf
	pstAdtsHead->number_of_raw_data_blocks_in_frame = Inner_GetRealValue(pucAacData, 54, 2);//54~55		2 uimsfb

	iThisFunRes = td_success;
	
LABLE_TD_MP4_EXIT:
	return iThisFunRes; 

}

/************************************************* 
  Function: Inner_GetAacFrameCount
  Description: »ñÈ¡AAC ÒôÆµ¸öÊý
  Input: _pvAccData £¬AAC Êý¾ÝÄÚ´æÖ¸Õë
  Input: _iAacDataSize £¬AAC Êý¾ÝÄÚ´æ³¤¶È
  Output: 
  Return: º¯ÊýÖ´ÐÐ³É¹¦·µ»Ø Êµ¼ÊAAC ¸öÊý, Ê§°Ü·µ»Ø td_failure
  Author: »ÆÖ¾¸Õ(Huangzhigang)
  Others: 
*************************************************/
static int Inner_GetAacFrameCount(void* _pvAccData, int _iAacDataSize)
{
	int iThisFunRes = td_failure;
	int iCountaac = 0;
	unsigned char* pucAudioDataBegin = NULL;
	int iAudioLeftSize = 0;
	void* pvAccData = NULL;
	int iAacDataSize = 0;

	if((NULL == _pvAccData))
	{
		td_printf(0xff0000, "%d: Input parameters is error!\n", __LINE__);
		iThisFunRes = td_failure;
		goto LABLE_TD_MP4_EXIT;
	}

	pvAccData = _pvAccData;
	iAacDataSize = _iAacDataSize;
	
	pucAudioDataBegin = (unsigned char*)pvAccData;
	iAudioLeftSize = iAacDataSize;
	
	for(iCountaac = 0; iAudioLeftSize > ADTS_HEADER_LENGTH; iCountaac++)
	{
		TADTSHeader tHeader;
		if(Inner_AnalyzeAdtsHead(&tHeader, pucAudioDataBegin, iAudioLeftSize) < 0)
		{
			td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
			iThisFunRes = td_failure;
			goto LABLE_TD_MP4_EXIT;
		}
		pucAudioDataBegin += tHeader.aac_frame_length;
		iAudioLeftSize -= tHeader.aac_frame_length;
	}
	
	iThisFunRes = iCountaac;
	
LABLE_TD_MP4_EXIT:
	return iThisFunRes; 
}

/************************************************* 
  Function: _pcVedioBuf
  Description: ·ÖÎöH264 NUAL µ¥Ôª
  Input: _pcVedioBuf £¬H264 Êý¾Ý
  Input: _uiVedioBufLen £¬H264 Êý¾Ý³¤¶È
  Input: _uiOffSet £¬Æ«ÒÆÁ¿
  Output: _pstNalu£¬½«·ÖÎöµÄÖµ´«¸Ã¸Ã²ÎÊý
  Return: º¯ÊýÖ´ÐÐ³É¹¦·µ»Ø ·ÖÎöµÄ³¤¶È, Ê§°Ü·µ»Ø td_failure
  Author: »ÆÖ¾¸Õ(Huangzhigang)
  Others: 
*************************************************/
static int Inner_AnalyzeNalu(const unsigned char* _pcVedioBuf, unsigned int _uiVedioBufLen, unsigned int _uiOffSet, TMP4Nalu* _pstNalu)
{
	int iThisFunRes = td_failure;
	unsigned char* pcVedioBuf = NULL;
	unsigned int uiVedioBufLen = 0;
	TMP4Nalu* pstNalu = NULL;
	int i = 0;
	int iPos = 0;
	
	if((NULL == _pcVedioBuf) || (NULL == _pstNalu))
	{
		td_printf(0xff0000, "%d: Input parameters is error!\n", __LINE__);
		iThisFunRes = td_failure;
		goto LABLE_TD_MP4_EXIT;
	}
	
	pcVedioBuf = (unsigned char*)_pcVedioBuf;
	uiVedioBufLen = _uiVedioBufLen;
	i = _uiOffSet;
	pstNalu = _pstNalu;
 
	while(i<uiVedioBufLen)  
	{  
		if(pcVedioBuf[i++] == 0x00 && pcVedioBuf[i++] == 0x00 && pcVedioBuf[i++] == 0x00 && pcVedioBuf[i++] == 0x01)
		{
			iPos = i;
			while (iPos<uiVedioBufLen)  
			{  
				if(pcVedioBuf[iPos+0] == 0x00 && pcVedioBuf[iPos+1] == 0x00 && pcVedioBuf[iPos+2] == 0x00 && pcVedioBuf[iPos+3] == 0x01)  
				{  
					iPos += 4;
					break;  
				}  
				iPos = iPos + 1;
			}  
			
			if(iPos == uiVedioBufLen)  
			{  
				pstNalu->m_iVedioSize= iPos-i;
			}  
			else  
			{  
				pstNalu->m_iVedioSize = (iPos-4)-i;  
			}
			pstNalu->m_iType = pcVedioBuf[i] & 0x1f;
			pstNalu->m_pucVedioData =(unsigned char*)&pcVedioBuf[i];	
			iThisFunRes = (pstNalu->m_iVedioSize+i-_uiOffSet); 
			goto LABLE_TD_MP4_EXIT;
		}  
	} 
	
	iThisFunRes = td_success;
	
LABLE_TD_MP4_EXIT:
	return iThisFunRes;  
}


/************************************************* 
  Function: Inner_WriteAac
  Description: ½«AAC ÒôÆµÐ´ÈëÎÄ¼þ
  Input: _pvMP4Mgr £¬MP4 ¹ÜÀí½á¹¹
  Input: _pucAacData £¬AAC Êý¾Ý
  Input: _iAacDataSize £¬AAC Êý¾Ý³¤¶È
  Output: 
  Return: º¯ÊýÖ´ÐÐ³É¹¦·µ»Ø td_success, Ê§°Ü·µ»Ø td_failure
  Author: »ÆÖ¾¸Õ(Huangzhigang)
  Others: 
*************************************************/
static int Inner_WriteAac(void* _pvMP4Mgr, unsigned char* _pucAacData, int _iAacDataSize)
{

	TMP4Mgr* pstMP4Mgr = (TMP4Mgr*)_pvMP4Mgr;
	int iThisFunRes = td_failure;
	TADTSHeader stHeader = {0};
	unsigned char *pucAudioDataBegin = NULL;
	int iAudioDataLeftSize = 0;
	int iAccFrameCount = 0;
	int iTmpAudioDuration = 0;
	int i = 0;

	if ((NULL==pstMP4Mgr) || (NULL==pstMP4Mgr->m_MP4Handle) || (NULL==_pucAacData) || (_iAacDataSize<=0))
	{
		td_printf(0xff0000, "%d: Input parameters is error!\n", __LINE__);
		iThisFunRes = td_failure;
		goto LABLE_TD_MP4_EXIT;
	}


	if(MP4_INVALID_TRACK_ID == pstMP4Mgr->m_AudioTrackID)
	{
		pstMP4Mgr->m_AudioTrackID = MP4AddAudioTrack(pstMP4Mgr->m_MP4Handle, pstMP4Mgr->m_stSDVFileHead.AudioSample, 1024, MP4_MPEG2_AAC_LC_AUDIO_TYPE);
		if (pstMP4Mgr->m_AudioTrackID == MP4_INVALID_TRACK_ID)
		{
			td_printf(0xff0000, "%d: Call MP4AddAudioTrack failed!\n", __LINE__);
			iThisFunRes = td_failure;
			goto LABLE_TD_MP4_EXIT;
		}
		MP4SetAudioProfileLevel(pstMP4Mgr->m_MP4Handle, 0x7f);
		MP4SetTrackDurationPerChunk(pstMP4Mgr->m_MP4Handle, pstMP4Mgr->m_AudioTrackID, 1024);
		MP4SetTrackLanguage(pstMP4Mgr->m_MP4Handle, pstMP4Mgr->m_AudioTrackID, "English");
	}

	iTmpAudioDuration = pstMP4Mgr->m_AudioDuration;
	pucAudioDataBegin = _pucAacData;
	iAudioDataLeftSize = _iAacDataSize;

	iAccFrameCount = Inner_GetAacFrameCount(pucAudioDataBegin, iAudioDataLeftSize);

	if(iAccFrameCount <= 0)
	{
		td_printf(0xff0000, "%s %s %d the aac framecount is %d , error!", __FILE__, __FUNCTION__, __LINE__, iAccFrameCount);
		iThisFunRes = td_failure;
		goto LABLE_TD_MP4_EXIT;
	}
	
	for(i = 0; i < iAccFrameCount; i++)
	{
		if(Inner_AnalyzeAdtsHead(&stHeader, pucAudioDataBegin, iAudioDataLeftSize) < 0)
		{
			td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
			iThisFunRes = td_failure;
			goto LABLE_TD_MP4_EXIT;
		}

		if(stHeader.aac_frame_length <= 0)
		{
			td_printf(0xff0000, "%s %s %d the aac len is [%d] too short!error!", __FILE__, __FUNCTION__, __LINE__, stHeader.aac_frame_length);
			iThisFunRes = td_failure;
			goto LABLE_TD_MP4_EXIT;
		}
		
		#if 1
		if (!MP4WriteSample(pstMP4Mgr->m_MP4Handle, pstMP4Mgr->m_AudioTrackID, pucAudioDataBegin, stHeader.aac_frame_length, MP4_INVALID_DURATION, 1, 1))
		{
			td_printf(0xff0000, "%d: Call MP4WriteSample failed![%d][%d]\n", __LINE__, pstMP4Mgr->m_AudioTrackID, stHeader.aac_frame_length);
			iThisFunRes = td_failure;
			goto LABLE_TD_MP4_EXIT;
		}
		#endif

		//Ð´Èë³É¹¦²Å¼ÆÊý
		pstMP4Mgr->m_uiAudioSampleID++;
		pstMP4Mgr->m_AudioTotalDuration += iTmpAudioDuration;
		iTmpAudioDuration = 0;
		
		pucAudioDataBegin += stHeader.aac_frame_length;
		iAudioDataLeftSize -= stHeader.aac_frame_length;
	}
	
	iThisFunRes = td_success;
	
LABLE_TD_MP4_EXIT:
	return iThisFunRes; 
}

int Inner_WriteUlaw(void* _pvMP4Mgr, unsigned char *_pG711Udata, int _iDatasize)
{
	TMP4Mgr* pstMP4Mgr = (TMP4Mgr*)_pvMP4Mgr;
	int iThisFunRes = td_success;
	unsigned char *pucAudioDataBegin = NULL;
	int iTmpAudioDuration = 0;
	int iCount = 0;

	if (pstMP4Mgr == NULL || _pG711Udata == NULL)
	{
		td_printf(0xff0000, "%d: Input parameters is error!\n", __LINE__);
		iThisFunRes = td_failure;
		goto LABLE_TD_MP4_EXIT;
	}
	
	if(_iDatasize != (pstMP4Mgr->m_stSDVFrameHead.u16AFrames)*(pstMP4Mgr->m_stSDVFrameHead.u16AFrameSize))
	{
		td_printf(0xff0000, "%s %s %d error!_iDatasize is %d, u16AFrames is %d, u16AFrameSize is %d", 
			__FILE__, __FUNCTION__, __LINE__, _iDatasize, pstMP4Mgr->m_stSDVFrameHead.u16AFrames, pstMP4Mgr->m_stSDVFrameHead.u16AFrameSize);
		iThisFunRes = td_failure;
		goto LABLE_TD_MP4_EXIT;
	}

	if(MP4_INVALID_TRACK_ID == pstMP4Mgr->m_AudioTrackID)
	{
		pstMP4Mgr->m_AudioTrackID = MP4AddULawAudioTrack(pstMP4Mgr->m_MP4Handle, pstMP4Mgr->m_stSDVFileHead.AudioSample);
		if (pstMP4Mgr->m_AudioTrackID == MP4_INVALID_TRACK_ID)
		{
			td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
			iThisFunRes = td_failure;
			goto LABLE_TD_MP4_EXIT;
		}
		MP4SetAudioProfileLevel(pstMP4Mgr->m_MP4Handle, 0x2);
	}

	iTmpAudioDuration = pstMP4Mgr->m_AudioDuration;
	pucAudioDataBegin = _pG711Udata;

	if((pstMP4Mgr->m_stSDVFrameHead.u16AFrameSize - 4) <= 0)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iThisFunRes = td_failure;
		goto LABLE_TD_MP4_EXIT;
	}

	for(iCount = 0; iCount < pstMP4Mgr->m_stSDVFrameHead.u16AFrames; iCount++)
	{
		if (!MP4WriteSample(pstMP4Mgr->m_MP4Handle, pstMP4Mgr->m_AudioTrackID, pucAudioDataBegin+4, pstMP4Mgr->m_stSDVFrameHead.u16AFrameSize - 4, iTmpAudioDuration, 0, 1))
		{
			td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
			iThisFunRes = td_failure;
			goto LABLE_TD_MP4_EXIT;
		}

		pstMP4Mgr->m_uiAudioSampleID++;
		pstMP4Mgr->m_AudioTotalDuration += iTmpAudioDuration;
		iTmpAudioDuration = 0;
		pucAudioDataBegin += pstMP4Mgr->m_stSDVFrameHead.u16AFrameSize;
	}

LABLE_TD_MP4_EXIT:	

	return iThisFunRes;
}

int Inner_WriteAlaw(void* _pvMP4Mgr, unsigned char *_pG711Adata, int _iDatasize)
{
	TMP4Mgr* pstMP4Mgr = (TMP4Mgr*)_pvMP4Mgr;
	int iThisFunRes = td_success;
	unsigned char *pucAudioDataBegin = NULL;
	int iTmpAudioDuration = 0;
	int iCount = 0;

	if (pstMP4Mgr == NULL || _pG711Adata == NULL)
	{
		td_printf(0xff0000, "%d: Input parameters is error!\n", __LINE__);
		iThisFunRes = td_failure;
		goto LABLE_TD_MP4_EXIT;
	}
	
	if(_iDatasize != (pstMP4Mgr->m_stSDVFrameHead.u16AFrames)*(pstMP4Mgr->m_stSDVFrameHead.u16AFrameSize))
	{
		td_printf(0xff0000, "%s %s %d error!_iDatasize is %d, u16AFrames is %d, u16AFrameSize is %d", 
			__FILE__, __FUNCTION__, __LINE__, _iDatasize, pstMP4Mgr->m_stSDVFrameHead.u16AFrames, pstMP4Mgr->m_stSDVFrameHead.u16AFrameSize);
		iThisFunRes = td_failure;
		goto LABLE_TD_MP4_EXIT;
	}

	if(MP4_INVALID_TRACK_ID == pstMP4Mgr->m_AudioTrackID)
	{
		pstMP4Mgr->m_AudioTrackID = MP4AddALawAudioTrack(pstMP4Mgr->m_MP4Handle, pstMP4Mgr->m_stSDVFileHead.AudioSample);
		if (pstMP4Mgr->m_AudioTrackID == MP4_INVALID_TRACK_ID)
		{
			td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
			iThisFunRes = td_failure;
			goto LABLE_TD_MP4_EXIT;
		}
		MP4SetAudioProfileLevel(pstMP4Mgr->m_MP4Handle, 0x2);
	}

	iTmpAudioDuration = pstMP4Mgr->m_AudioDuration;
	pucAudioDataBegin = _pG711Adata;

	if((pstMP4Mgr->m_stSDVFrameHead.u16AFrameSize - 4) <= 0)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iThisFunRes = td_failure;
		goto LABLE_TD_MP4_EXIT;
	}

	//td_printf(0, "pstMP4Mgr->m_stSDVFrameHead.u16AFrames is %d, iTmpAudioDuration is %d", pstMP4Mgr->m_stSDVFrameHead.u16AFrames, iTmpAudioDuration);

	for(iCount = 0; iCount < pstMP4Mgr->m_stSDVFrameHead.u16AFrames; iCount++)
	{
		if (!MP4WriteSample(pstMP4Mgr->m_MP4Handle, pstMP4Mgr->m_AudioTrackID, pucAudioDataBegin+4, pstMP4Mgr->m_stSDVFrameHead.u16AFrameSize-4, iTmpAudioDuration, 0, 1))
		{
			td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
			iThisFunRes = td_failure;
			goto LABLE_TD_MP4_EXIT;
		}
		
		pstMP4Mgr->m_uiAudioSampleID++;
		pstMP4Mgr->m_AudioTotalDuration += iTmpAudioDuration;
		iTmpAudioDuration = 0;
		pucAudioDataBegin += pstMP4Mgr->m_stSDVFrameHead.u16AFrameSize;
	}

LABLE_TD_MP4_EXIT:	
	
		return iThisFunRes;
}

static int Inner_WriteAdpcm(void* _pvMP4Mgr, unsigned char* _pucAdpcmData, int _iAudioDataSize)
{
	TMP4Mgr* pstMP4Mgr = (TMP4Mgr*)_pvMP4Mgr;
	int iThisFunRes = td_success;
	unsigned char *pucAudioDataBegin = NULL;
	int iTmpAudioDuration = 0;

	if ((NULL==pstMP4Mgr) || (NULL==pstMP4Mgr->m_MP4Handle) || (NULL==_pucAdpcmData) || (_iAudioDataSize<=0))
	{
		td_printf(0xff0000, "%d: Input parameters is error!\n", __LINE__);
		iThisFunRes = td_failure;
		goto LABLE_TD_MP4_EXIT;
	}

	if(MP4_INVALID_TRACK_ID == pstMP4Mgr->m_AudioTrackID)
	{
		pstMP4Mgr->m_AudioTrackID = MP4AddAudioTrack(pstMP4Mgr->m_MP4Handle, pstMP4Mgr->m_stSDVFileHead.AudioSample, 512, MP4_PCM16_LITTLE_ENDIAN_AUDIO_TYPE);
		if (pstMP4Mgr->m_AudioTrackID == MP4_INVALID_TRACK_ID)
		{
			td_printf(0xff0000, "%d: Call MP4AddAudioTrack failed!\n", __LINE__);
			iThisFunRes = td_failure;
			goto LABLE_TD_MP4_EXIT;
		}
	}

	iTmpAudioDuration = pstMP4Mgr->m_AudioDuration;
	pucAudioDataBegin = _pucAdpcmData;

	if((pstMP4Mgr->m_stSDVFrameHead.u16AFrameSize - 4) <= 0)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iThisFunRes = td_failure;
		goto LABLE_TD_MP4_EXIT;
	}

	td_printf(0, "pstMP4Mgr->m_stSDVFrameHead.u16AFrames is %d, iTmpAudioDuration is %d", pstMP4Mgr->m_stSDVFrameHead.u16AFrames, iTmpAudioDuration);

	int iCount = 0;
	for(iCount = 0; iCount < pstMP4Mgr->m_stSDVFrameHead.u16AFrames; iCount++)
	{
		if (!MP4WriteSample(pstMP4Mgr->m_MP4Handle, pstMP4Mgr->m_AudioTrackID, pucAudioDataBegin+4, pstMP4Mgr->m_stSDVFrameHead.u16AFrameSize-4, iTmpAudioDuration, 0, 1))
		{
			td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
			iThisFunRes = td_failure;
			goto LABLE_TD_MP4_EXIT;
		}
		
		pstMP4Mgr->m_uiAudioSampleID++;
		pstMP4Mgr->m_AudioTotalDuration += iTmpAudioDuration;
		iTmpAudioDuration = 0;
		pucAudioDataBegin += pstMP4Mgr->m_stSDVFrameHead.u16AFrameSize;
	}
	
LABLE_TD_MP4_EXIT:
	return iThisFunRes; 
}

/************************************************* 
  Function: Inner_WriteH264
  Description: ½«AAC ÒôÆµÐ´ÈëÎÄ¼þ
  Input: _pvMP4Mgr £¬MP4 ¹ÜÀí½á¹¹
  Input: _pcH264 £¬H264 Êý¾Ý
  Input: _iH264Size £¬H264 Êý¾Ý³¤¶È
  Output: 
  Return: º¯ÊýÖ´ÐÐ³É¹¦·µ»Ø Ð´Èë³¤¶È, Ê§°Ü·µ»Ø td_failure
  Author: »ÆÖ¾¸Õ(Huangzhigang)
  Others: 
*************************************************/
static int Inner_WriteH264(void* _pvMP4Mgr, const unsigned char* _pcH264, int _iH264Size)  
{
	TMP4Mgr* pstMP4Mgr = (TMP4Mgr*)_pvMP4Mgr;
	int iThisFunRes = td_failure;
	unsigned char* pcH264 = NULL;
	int iH264Size = 0;
	unsigned char* pcTmpMP4SameBuf = NULL;
	int iTmpMP4SameBufLen = MP4_BOX_SIEZ_LENGTH;
	TMP4Nalu stNalu = {0};  
	int iPos = 0;
	int iAnalyzeRes = 0;	
	//int iEnCryptDataLen = 0;
	unsigned long long ullTmpVideoDuration = 0;
	int iIFrameFlag = 6;//ÎÞÐ§Ö¡
	int iNeedWriteBufToMp4 = 0;
	static int iFirstFrame = 0;
	td_printf(0, "%s %d test!", __FUNCTION__, __LINE__);	
	if((NULL == pstMP4Mgr) || (NULL == _pcH264) || (0 >= _iH264Size))
	{
		td_printf(0xff0000, "%d: Input parameters is error!\n", __LINE__);
		iThisFunRes = td_failure;
		goto LABLE_TD_MP4_EXIT;
	}

	pcH264 = (unsigned char*)_pcH264;
	iH264Size = _iH264Size;

	pcTmpMP4SameBuf = pstMP4Mgr->m_ucVideoSampleDataBuf;
	
	if(NULL == pcTmpMP4SameBuf)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		goto LABLE_TD_MP4_EXIT;
	}

	#if 0
	if((pstMP4Mgr->m_stSDVFrameHead.reserved & 0x03) == 0x02)
	{
		//Ö»Òª·¢ÏÖÓÐÒ»Ö¡¼ÓÃÜ£¬¾Í±ê¼ÇMP4 ÎÄ¼þÎª¼ÓÃÜ×´Ì¬
		if(MP4_INVALID_TRACK_ID == pstMP4Mgr->m_VideoTrackID)
		{
			pstMP4Mgr->m_VideoTrackID = MP4AddH264VideoTrack(pstMP4Mgr->m_MP4Handle, 90000, 90000/pstMP4Mgr->m_stSDVFileHead.FrameRate, pstMP4Mgr->m_stSDVFileHead.Width
				, pstMP4Mgr->m_stSDVFileHead.Height, 0, 0, 0, 0);

			if (pstMP4Mgr->m_VideoTrackID == MP4_INVALID_TRACK_ID)	
			{
				td_printf(0xff0000, "%d: Add video track failed!\n", __LINE__);
				goto LABLE_TD_MP4_EXIT;
			}
			else
			{
				MP4SetTrackDurationPerChunk(pstMP4Mgr->m_MP4Handle, pstMP4Mgr->m_VideoTrackID, 90000 / pstMP4Mgr->m_stSDVFileHead.FrameRate);
			}
		}
	
		iEnCryptDataLen = sizeof(pstMP4Mgr->m_stSDVFrameHead.u32FrameNO) + iH264Size;

		if((iTmpMP4SameBufLen+iEnCryptDataLen) <= pstMP4Mgr->m_iVideoSampleDataBufSize)
		{
			pcTmpMP4SameBuf[0] = iEnCryptDataLen>>24;  
			pcTmpMP4SameBuf[1] = ((iEnCryptDataLen)&0x00ff0000)>>16;  
			pcTmpMP4SameBuf[2] = ((iEnCryptDataLen)&0x0000ff00)>>8;  
			pcTmpMP4SameBuf[3] = (iEnCryptDataLen)&0x000000ff;
			memcpy(pcTmpMP4SameBuf+iTmpMP4SameBufLen, &(pstMP4Mgr->m_stSDVFrameHead.u32FrameNO), sizeof(pstMP4Mgr->m_stSDVFrameHead.u32FrameNO));
			iTmpMP4SameBufLen += sizeof(pstMP4Mgr->m_stSDVFrameHead.u32FrameNO);
			memcpy(pcTmpMP4SameBuf+iTmpMP4SameBufLen,pcH264,iH264Size);
			iTmpMP4SameBufLen += iH264Size;
		}
		else
		{
			td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
			iThisFunRes = td_failure;
			goto LABLE_TD_MP4_EXIT;
		}
		
		pstMP4Mgr->m_bIFrameFlag = 0;
		
		if (pstMP4Mgr->m_stSDVFrameHead.u8FrameType == 0)
		{
			pstMP4Mgr->m_bIFrameFlag = 1;
		}
							
		if(!MP4WriteSample(pstMP4Mgr->m_MP4Handle, pstMP4Mgr->m_VideoTrackID, pcTmpMP4SameBuf, iTmpMP4SameBufLen, pstMP4Mgr->m_VedioDuration, 0, pstMP4Mgr->m_bIFrameFlag))  
		{
			td_printf(0xff0000, "%d: Call MP4WriteSample failed!.\n", __LINE__);
			iThisFunRes = td_failure;
			goto LABLE_TD_MP4_EXIT;
		}
		
		//Ð´Èë³É¹¦²Å¼ÆÊý
		pstMP4Mgr->m_uiVedioSampleID++;
		pstMP4Mgr->m_VedioTotalDuration += pstMP4Mgr->m_VedioDuration;
		if (pstMP4Mgr->m_stSDVFrameHead.u8FrameType == 0)
		{
			pstMP4Mgr->m_iIFrameCount++;
		}
	}
	else
	#endif
	{
		unsigned int uiNaluheader = ntohl(0x00000001);
		td_printf(0, "%s %d test!", __FUNCTION__, __LINE__);	
		while (0 != (iAnalyzeRes = Inner_AnalyzeNalu(pcH264,iH264Size,iPos,&stNalu)))  
		{
			td_printf(0, "%s %d test!", __FUNCTION__, __LINE__);	
			//td_printf(0x12345678, "Inner_WriteH264 the nalu type is ================>%2x", stNalu.m_iType);
		
			if(stNalu.m_iType == SEQ_PARAMETER_SET_RBSP)
			{//sps
				if(0 == pstMP4Mgr->m_bFirstSpsFlag)
				{
					if(MP4_INVALID_TRACK_ID == pstMP4Mgr->m_VideoTrackID)
					{
						td_printf(0, "%s %d test!", __FUNCTION__, __LINE__);	
						td_printf(0x12345678, "---------------->>>>>write Sps^^^^^^^^^^^^pstMP4Mgr->m_stSDVFileHead.FrameRate is %d", pstMP4Mgr->m_stSDVFileHead.FrameRate);
						pstMP4Mgr->m_VideoTrackID = MP4AddH264VideoTrack(pstMP4Mgr->m_MP4Handle, 90000, 90000/pstMP4Mgr->m_stSDVFileHead.FrameRate, pstMP4Mgr->m_stSDVFileHead.Width
							, pstMP4Mgr->m_stSDVFileHead.Height, stNalu.m_pucVedioData[1], stNalu.m_pucVedioData[2], stNalu.m_pucVedioData[3], 3);
						td_printf(0, "%s %d test!", __FUNCTION__, __LINE__);	
						if (pstMP4Mgr->m_VideoTrackID == MP4_INVALID_TRACK_ID)	
						{
							td_printf(0xff0000, "%d: Add video track failed!\n", __LINE__);
							iThisFunRes = td_failure;
							goto LABLE_TD_MP4_EXIT;
						}
						td_printf(0, "%s %d test!", __FUNCTION__, __LINE__);	
						MP4SetVideoProfileLevel(pstMP4Mgr->m_MP4Handle, 0x01);
						MP4AddH264SequenceParameterSet(pstMP4Mgr->m_MP4Handle,pstMP4Mgr->m_VideoTrackID, stNalu.m_pucVedioData, stNalu.m_iVedioSize); 
						MP4SetTrackDurationPerChunk(pstMP4Mgr->m_MP4Handle, pstMP4Mgr->m_VideoTrackID, 90000 / pstMP4Mgr->m_stSDVFileHead.FrameRate);
					}
					pstMP4Mgr->m_bFirstSpsFlag = 1;
				}
			}  
			else if(stNalu.m_iType == PIC_PARAMETER_SET_RBSP)
			{//pps
				td_printf(0, "%s %d test!", __FUNCTION__, __LINE__);	
				if(0 == pstMP4Mgr->m_bFirstPpsFlag)
				{
					if(MP4_INVALID_TRACK_ID != pstMP4Mgr->m_VideoTrackID)
					{
						td_printf(0x12345678, "---------------->>>>> write Pps");
						MP4AddH264PictureParameterSet(pstMP4Mgr->m_MP4Handle,pstMP4Mgr->m_VideoTrackID, stNalu.m_pucVedioData, stNalu.m_iVedioSize);
					}
					pstMP4Mgr->m_bFirstPpsFlag = 1;
				}
			} 
			else  if((1 == pstMP4Mgr->m_bFoundSpsPpsFlag)&&
				((SLICE_LAYER_WITHOUT_PARTITIONING_RBSP == stNalu.m_iType) || 
					(SLICE_LAYER_WITH_PARTITIONING_RBSP == stNalu.m_iType) ||
					((SEI_RBSP == stNalu.m_iType)&&(pstMP4Mgr->m_iIFrameCount > 0))))	
			{
				td_printf(0, "%s %d test!", __FUNCTION__, __LINE__);	
				if (SLICE_LAYER_WITH_PARTITIONING_RBSP == stNalu.m_iType)
				{
						PrintInfo("SLICE_LAYER_WITH_PARTITIONING_RBSP========================");
					pstMP4Mgr->m_bIFrameFlag = 1;					
					iIFrameFlag = 1;			
					iFirstFrame = 1;
				}
				else if(SLICE_LAYER_WITHOUT_PARTITIONING_RBSP == stNalu.m_iType)
				{
						PrintInfo("SLICE_LAYER_WITHOUT_PARTITIONING_RBSP========================");
					pstMP4Mgr->m_bIFrameFlag = 0;
					iIFrameFlag = 0;				
				}
				else if(SEI_RBSP == stNalu.m_iType)
				{
						PrintInfo("SEI_RBSP========================");
					pstMP4Mgr->m_bIFrameFlag = 0;
					iIFrameFlag = 6;//ÓÃÓÚ×Ô¶¨ÒåÐÅÏ¢¼ÇÂ¼£¬¶ÏµçÐÞ¸´Ê¹ÓÃ				
				}
				else//²»¿ÉÄÜ×ßµ½Õâ
				{
					td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
					goto LABLE_TD_MP4_EXIT;
				}

				if(stNalu.m_iVedioSize > 0)
				{
					if((SLICE_LAYER_WITH_PARTITIONING_RBSP == stNalu.m_iType)
						||(SLICE_LAYER_WITHOUT_PARTITIONING_RBSP == stNalu.m_iType))
					{
						if(MP4_BOX_SIEZ_LENGTH == iTmpMP4SameBufLen)//µÚÒ»¸öslice
						{
							if((iTmpMP4SameBufLen+stNalu.m_iVedioSize) <= pstMP4Mgr->m_iVideoSampleDataBufSize)
							{
								memcpy(pcTmpMP4SameBuf+iTmpMP4SameBufLen,stNalu.m_pucVedioData,stNalu.m_iVedioSize);
								iTmpMP4SameBufLen += stNalu.m_iVedioSize;
							}
							else
							{
								td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
								goto LABLE_TD_MP4_EXIT;
							}
						}
						else
						{
							if((iTmpMP4SameBufLen+4+stNalu.m_iVedioSize) <= pstMP4Mgr->m_iVideoSampleDataBufSize)
							{
								memcpy(pcTmpMP4SameBuf+iTmpMP4SameBufLen,&uiNaluheader,4);
								iTmpMP4SameBufLen += 4;
								memcpy(pcTmpMP4SameBuf+iTmpMP4SameBufLen,stNalu.m_pucVedioData,stNalu.m_iVedioSize);
								iTmpMP4SameBufLen += stNalu.m_iVedioSize;
							}
							else
							{
								td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
								goto LABLE_TD_MP4_EXIT;
							}
						}
						
						iNeedWriteBufToMp4 = 1;//ÐèÒª½«»º´æÖÐµÄÊý¾Ý×ª»»Îªmp4 sample
					}
					else if(SEI_RBSP == stNalu.m_iType)
					{
						PrintInfo("SEI_RBSP==========MP4WriteSample==============");
						memcpy(pcTmpMP4SameBuf+iTmpMP4SameBufLen,stNalu.m_pucVedioData,stNalu.m_iVedioSize);
						iTmpMP4SameBufLen += stNalu.m_iVedioSize;
	
						pcTmpMP4SameBuf[0] = (iTmpMP4SameBufLen-MP4_BOX_SIEZ_LENGTH)>>24;  
						pcTmpMP4SameBuf[1] = ((iTmpMP4SameBufLen-MP4_BOX_SIEZ_LENGTH)&0x00ff0000)>>16;	
						pcTmpMP4SameBuf[2] = ((iTmpMP4SameBufLen-MP4_BOX_SIEZ_LENGTH)&0x0000ff00)>>8;  
						pcTmpMP4SameBuf[3] = (iTmpMP4SameBufLen-MP4_BOX_SIEZ_LENGTH)&0x000000ff;	
						
						ullTmpVideoDuration = 0;
						//td_printf(0x12345678, "------------------------->m_uiUselessCount is %u", pstMP4Mgr->m_uiUselessCount);
						
						//td_printf(0, "********************iTmpMP4SameBufLen is %d, iIFrameFlag is %d", iTmpMP4SameBufLen, iIFrameFlag);
						
						if((1 == iFirstFrame) && !MP4WriteSample(pstMP4Mgr->m_MP4Handle, pstMP4Mgr->m_VideoTrackID, pcTmpMP4SameBuf, iTmpMP4SameBufLen, ullTmpVideoDuration, 0, pstMP4Mgr->m_bIFrameFlag))  
						{
							td_printf(0xff0000, "%d: Call MP4WriteSample failed!.\n", __LINE__);
							goto LABLE_TD_MP4_EXIT;
						}
						//Ð´Èë³É¹¦²Å¼ÆÊý
						pstMP4Mgr->m_uiUselessCount++;//ÎÞÐ§Ö¡¼ÆÊýÀÛ¼Ó
						pstMP4Mgr->m_uiVedioSampleID++;
						pstMP4Mgr->m_VedioTotalDuration += ullTmpVideoDuration;
						iTmpMP4SameBufLen = MP4_BOX_SIEZ_LENGTH;//Ð´ÍêÒ»¸öNALUºó£¬ÖØÐÂ¸³Öµ
						iNeedWriteBufToMp4 = 0;//»º´æÊý¾ÝÒÑ×ª»»£¬ÖÃÁã
					}
				}	
			}
			iPos += iAnalyzeRes;  
		} 
		td_printf(0, "%s %d test!", __FUNCTION__, __LINE__);	
		if(1 == iNeedWriteBufToMp4)
		{
						PrintInfo("==========MP4WriteSample==============");
			pcTmpMP4SameBuf[0] = (iTmpMP4SameBufLen-MP4_BOX_SIEZ_LENGTH)>>24;  
			pcTmpMP4SameBuf[1] = ((iTmpMP4SameBufLen-MP4_BOX_SIEZ_LENGTH)&0x00ff0000)>>16;	
			pcTmpMP4SameBuf[2] = ((iTmpMP4SameBufLen-MP4_BOX_SIEZ_LENGTH)&0x0000ff00)>>8;  
			pcTmpMP4SameBuf[3] = (iTmpMP4SameBufLen-MP4_BOX_SIEZ_LENGTH)&0x000000ff;	
						
			//td_printf(0, "********************iTmpMP4SameBufLen is %d, iIFrameFlag is %d", iTmpMP4SameBufLen, iIFrameFlag);
			ullTmpVideoDuration = pstMP4Mgr->m_VedioDuration;
			
			if((1 == iFirstFrame) && !MP4WriteSample(pstMP4Mgr->m_MP4Handle, pstMP4Mgr->m_VideoTrackID, pcTmpMP4SameBuf, iTmpMP4SameBufLen, ullTmpVideoDuration, 0, pstMP4Mgr->m_bIFrameFlag))  
			{
				td_printf(0xff0000, "%d: Call MP4WriteSample failed!.\n", __LINE__);
				goto LABLE_TD_MP4_EXIT;
			}
										
			//Ð´Èë³É¹¦²Å¼ÆÊý
			pstMP4Mgr->m_uiVedioSampleID++;
			pstMP4Mgr->m_VedioTotalDuration += ullTmpVideoDuration;

			if (1 == pstMP4Mgr->m_bIFrameFlag)
			{
				pstMP4Mgr->m_iIFrameCount++;
			}
		}
		td_printf(0, "%s %d test!", __FUNCTION__, __LINE__);	
		if((1 != pstMP4Mgr->m_bFirstSpsFlag)||(1 != pstMP4Mgr->m_bFirstPpsFlag))
		{
			td_printf(0xff0000, "%d: have not find SpsPps, go on!\n", __LINE__);
			iThisFunRes = MP4_CONVERT_ERROR_SPSPPS_CONTINUE;
			goto LABLE_TD_MP4_EXIT;
		}
		else
		{
			if(0 == pstMP4Mgr->m_bFoundSpsPpsFlag)
			{
				td_printf(0x12345678, "%d: have found SpsPps, seek from head again!\n", __LINE__);
				
				//ÒÆ¶¯µ½ÎÄ¼þÍ·Î»ÖÃ
				int iSeekRet = 0;
				iSeekRet = td_mp4_fseek((void*)(pstMP4Mgr->m_pfSDVFileHandle), sizeof(S_FileHeader), SEEK_SET);
				
				if(-1 == iSeekRet)
				{
					td_printf(0xff0000, "%d: Seek failed! iSeekRet=%d. Error info[%s].\n", __LINE__, iSeekRet, strerror(errno));
					iThisFunRes = td_failure;
					goto LABLE_TD_MP4_EXIT;
					
				}
		
				pstMP4Mgr->m_bFoundSpsPpsFlag = 1;
				iThisFunRes = MP4_CONVERT_ERROR_SPSPPS_CONTINUE;
				goto LABLE_TD_MP4_EXIT;
			}
		}  
	}
	iThisFunRes = td_success;
	td_printf(0, "%s %d test!", __FUNCTION__, __LINE__);	
LABLE_TD_MP4_EXIT:
	td_printf(0, "%s %d test!", __FUNCTION__, __LINE__);	
	return iThisFunRes;  
	
}    


//cwmÅÐ¶ÏÊÓÆµ±àÂë¸ñÊ½
typedef struct AVProbeData {
    const char *filename;
    unsigned char *buf; /**< Buffer must have AVPROBE_PADDING_SIZE of extra allocated bytes filled with zero. */
    int buf_size;       /**< Size of buf except extra allocated bytes */
    const char *mime_type; /**< mime_type, when known. */
} AVProbeData;

/************************************************* 
  Function: init_hevc_probe
  Description:
  Input: 
  Input: 
  Input: 
  Output: 
  Return: 
  Author: ³ÂÎÄÃô
  Others: 20151208
*************************************************/

int init_hevc_probe(AVProbeData *p, const unsigned char *src, int size)
{
	if((NULL == p) || (size < 2048))
	{
		return -1;
	}
	p->buf_size = size;
	p->buf = (unsigned char *)malloc(p->buf_size);
	if(NULL == p->buf)
	{
		return -1;
	}
	memcpy(p->buf, src, p->buf_size);
	
	return 0;
}

/************************************************* 
  Function: hevc_probe
  Description:
  Input: 
  Input: 
  Input: 
  Output: 
  Return: 
  Author: ³ÂÎÄÃô
  Others: 20151208
*************************************************/

int hevc_probe(AVProbeData *p)
{
    uint32_t code = -1;
    int vps = 0, sps = 0, pps = 0, irap = 0;
    int i;

	if(NULL == p)
	{
		return -1;
	}
    for (i = 0; i < p->buf_size - 1; i++) {
        code = (code << 8) + p->buf[i];
        if ((code & 0xffffff00) == 0x100) {
            uint8_t nal2 = p->buf[i + 1];
            int type = (code & 0x7E) >> 1;

            if (code & 0x81) // forbidden and reserved zero bits
                return 0;

            if (nal2 & 0xf8) // reserved zero
                return 0;

            switch (type) {
            case NAL_VPS:        vps++;  break;
            case NAL_SPS:        sps++;  break;
            case NAL_PPS:        pps++;  break;
            case NAL_BLA_N_LP:
            case NAL_BLA_W_LP:
            case NAL_BLA_W_RADL:
            case NAL_CRA_NUT:
            case NAL_IDR_N_LP:
            case NAL_IDR_W_RADL: irap++; break;
            }
        }
    }
//Í¨¹ývps sps pps irapÀ´ÅÐ¶ÏÊÇ·ñhevcÊý¾Ý£¬ÓÈÆäÊÇvps£¬´æÔÚvps¾Í¿ÉÒÔÅÐ¶ÏÎªhevcÊý¾ÝÁË
    if (vps && sps && pps && irap)
        return 1; // 1 more than .mpg
    return 0;
}

/************************************************* 
  Function: free_hevc_pro
  Description:
  Input: 
  Input: 
  Input: 
  Output: 
  Return: 
  Author: ³ÂÎÄÃô
  Others: 20151208
*************************************************/

void free_hevc_pro(AVProbeData *p)
{
	if(p->buf != NULL)
	{
		free(p->buf);
		p->buf = NULL;
		p->buf_size = 0;
	}
}


/************************************************* 
  Function: Inner_AnalyzeNaluH265
  Description:¼ÆËãnaluµ¥ÔªµÄ´óÐ¡Óë¿ªÊ¼Î»ÖÃ
  Input: 
  Input: 
  Input: 
  Output: 
  Return: 
  Author: ³ÂÎÄÃô
  Others: 20151208
*************************************************/

int Inner_AnalyzeNaluH265(const unsigned char *_pcVedioBuf, unsigned int _uiVedioBufLen, unsigned int _uiOffSet, TMP4Nalu* _pstNalu)
{  
	int i = _uiOffSet;  
	while(i<_uiVedioBufLen)  
	{  
		if(_pcVedioBuf[i++] == 0x00 && _pcVedioBuf[i++] == 0x00 && _pcVedioBuf[i++] == 0x00 && _pcVedioBuf[i++] == 0x01)
		{
			int pos = i;  
			while (pos<_uiVedioBufLen)  
			{  
				if(_pcVedioBuf[pos+0] == 0x00 && _pcVedioBuf[pos+1] == 0x00 && _pcVedioBuf[pos+2] == 0x00 && _pcVedioBuf[pos+3] == 0x01)  
				{  
					pos += 4;
					break;  
				}  
				pos = pos + 1;
			}  
			if(pos == _uiVedioBufLen)  
			{  
				_pstNalu->m_iVedioSize = pos-i;
			}  
			else  
			{  
				_pstNalu->m_iVedioSize = (pos-4)-i;  
			} 
			_pstNalu->m_iType = _pcVedioBuf[i]&0x7e;
			_pstNalu->m_iType >>= 1; //ÖÐ¼ä6 bit
			_pstNalu->m_pucVedioData =(unsigned char*)&_pcVedioBuf[i];  //nula 2 byte

			return (_pstNalu->m_iVedioSize+i-_uiOffSet);  
		}  
	}  
	return 0;  
}




/************************************************* 
  Function: Inner_WriteH265
  Description: ½«AAC ÒôÆµÐ´ÈëÎÄ¼þ
  Input: _pvMP4Mgr £¬MP4 ¹ÜÀí½á¹¹
  Input: _pcH265 £¬H265 Êý¾Ý
  Input: _iH265Size £¬H265 Êý¾Ý³¤¶È
  Output: 
  Return: º¯ÊýÖ´ÐÐ³É¹¦·µ»Ø Ð´Èë³¤¶È, Ê§°Ü·µ»Ø td_failure
  Author: ³ÂÎÄÃô
  Others: 20151208
*************************************************/
	

static int Inner_WriteH265(void* _pvMP4Mgr, const unsigned char* _pcH265, int _iH265Size)  
{
	TMP4Mgr* pstMP4Mgr = (TMP4Mgr*)_pvMP4Mgr;
	int iThisFunRes = td_failure;
	unsigned char* pcH265 = NULL;
	int iH265Size = 0;
	unsigned char* pcTmpMP4SameBuf = NULL;
	int iTmpMP4SameBufLen = MP4_BOX_SIEZ_LENGTH;
	TMP4Nalu stNalu = {0};  
	int iPos = 0;
	int iAnalyzeRes = 0;	
	//int iEnCryptDataLen = 0;
	unsigned long long ullTmpVideoDuration = 0;
	int iIFrameFlag = 6;//ÎÞÐ§Ö¡
	int iNeedWriteBufToMp4 = 0;
	static int iFirstFrame = 0;//cwmÐ´Èëµ½mp4ÎÄ¼þÖÐµÄÊý¾Ý(stsz)²»ÊÇSEIÖ¡£¬¼´ÆÁ±ÎµôsdvµÄµÚÒ»Ö¡Êý¾ÝÎªSEIÖ¡£¬½â¾öÁË½âÎömp4ÎÄ¼þÊ±£¬ÒòÎªµÚÒ»Ö¡stszÎªSEIÖ¡£¬Ôì³ÉµÄmp4v2¿â±¨´í
	td_printf(0, "%s %d test!", __FUNCTION__, __LINE__);	
	if((NULL == pstMP4Mgr) || (NULL == _pcH265) || (0 >= _iH265Size))
	{
		td_printf(0xff0000, "%d: Input parameters is error!\n", __LINE__);
		iThisFunRes = td_failure;
		goto LABLE_TD_MP4_EXIT;
	}

	pcH265 = (unsigned char*)_pcH265;
	iH265Size = _iH265Size;

	pcTmpMP4SameBuf = pstMP4Mgr->m_ucVideoSampleDataBuf;
	
	if(NULL == pcTmpMP4SameBuf)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		goto LABLE_TD_MP4_EXIT;
	}

	#if 0
	if((pstMP4Mgr->m_stSDVFrameHead.reserved & 0x03) == 0x02)
	{
		//Ö»Òª·¢ÏÖÓÐÒ»Ö¡¼ÓÃÜ£¬¾Í±ê¼ÇMP4 ÎÄ¼þÎª¼ÓÃÜ×´Ì¬
		if(MP4_INVALID_TRACK_ID == pstMP4Mgr->m_VideoTrackID)
		{
			pstMP4Mgr->m_VideoTrackID = MP4AddH265VideoTrack(pstMP4Mgr->m_MP4Handle, 90000, 90000/pstMP4Mgr->m_stSDVFileHead.FrameRate, pstMP4Mgr->m_stSDVFileHead.Width
				, pstMP4Mgr->m_stSDVFileHead.Height, 0, 0, 0, 0);

			if (pstMP4Mgr->m_VideoTrackID == MP4_INVALID_TRACK_ID)	
			{
				td_printf(0xff0000, "%d: Add video track failed!\n", __LINE__);
				goto LABLE_TD_MP4_EXIT;
			}
			else
			{
				MP4SetTrackDurationPerChunk(pstMP4Mgr->m_MP4Handle, pstMP4Mgr->m_VideoTrackID, 90000 / pstMP4Mgr->m_stSDVFileHead.FrameRate);
			}
		}
	
		iEnCryptDataLen = sizeof(pstMP4Mgr->m_stSDVFrameHead.u32FrameNO) + iH265Size;

		if((iTmpMP4SameBufLen+iEnCryptDataLen) <= pstMP4Mgr->m_iVideoSampleDataBufSize)
		{
			pcTmpMP4SameBuf[0] = iEnCryptDataLen>>24;  
			pcTmpMP4SameBuf[1] = ((iEnCryptDataLen)&0x00ff0000)>>16;  
			pcTmpMP4SameBuf[2] = ((iEnCryptDataLen)&0x0000ff00)>>8;  
			pcTmpMP4SameBuf[3] = (iEnCryptDataLen)&0x000000ff;
			memcpy(pcTmpMP4SameBuf+iTmpMP4SameBufLen, &(pstMP4Mgr->m_stSDVFrameHead.u32FrameNO), sizeof(pstMP4Mgr->m_stSDVFrameHead.u32FrameNO));
			iTmpMP4SameBufLen += sizeof(pstMP4Mgr->m_stSDVFrameHead.u32FrameNO);
			memcpy(pcTmpMP4SameBuf+iTmpMP4SameBufLen,pcH265,iH265Size);
			iTmpMP4SameBufLen += iH265Size;
		}
		else
		{
			td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
			iThisFunRes = td_failure;
			goto LABLE_TD_MP4_EXIT;
		}
		
		pstMP4Mgr->m_bIFrameFlag = 0;
		
		if (pstMP4Mgr->m_stSDVFrameHead.u8FrameType == 0)
		{
			pstMP4Mgr->m_bIFrameFlag = 1;
		}
							
		if(!MP4WriteSample(pstMP4Mgr->m_MP4Handle, pstMP4Mgr->m_VideoTrackID, pcTmpMP4SameBuf, iTmpMP4SameBufLen, pstMP4Mgr->m_VedioDuration, 0, pstMP4Mgr->m_bIFrameFlag))  
		{
			td_printf(0xff0000, "%d: Call MP4WriteSample failed!.\n", __LINE__);
			iThisFunRes = td_failure;
			goto LABLE_TD_MP4_EXIT;
		}
		
		//Ð´Èë³É¹¦²Å¼ÆÊý
		pstMP4Mgr->m_uiVedioSampleID++;
		pstMP4Mgr->m_VedioTotalDuration += pstMP4Mgr->m_VedioDuration;
		if (pstMP4Mgr->m_stSDVFrameHead.u8FrameType == 0)
		{
			pstMP4Mgr->m_iIFrameCount++;
		}
	}
	else
	#endif
	{
		unsigned int uiNaluheader = ntohl(0x00000001);
		td_printf(0, "%s %d test!", __FUNCTION__, __LINE__);	
		while (0 != (iAnalyzeRes = Inner_AnalyzeNaluH265(pcH265,iH265Size,iPos,&stNalu)))  
		{
			td_printf(0, "%s %d test!", __FUNCTION__, __LINE__);	
		
			if(stNalu.m_iType == NAL_VPS)
			{//vps
				static int vps = 0;
				if(0 == vps)
				{					
					pstMP4Mgr->m_VideoTrackID = MP4AddH265VideoTrack(pstMP4Mgr->m_MP4Handle, 90000, 90000/pstMP4Mgr->m_stSDVFileHead.FrameRate, pstMP4Mgr->m_stSDVFileHead.Width
						, pstMP4Mgr->m_stSDVFileHead.Height, stNalu.m_pucVedioData[1], stNalu.m_pucVedioData[2], stNalu.m_pucVedioData[3], 3);
					td_printf(0, "%s %d test!", __FUNCTION__, __LINE__);	
					if (pstMP4Mgr->m_VideoTrackID == MP4_INVALID_TRACK_ID)	
					{
						td_printf(0xff0000, "%d: Add video track failed!\n", __LINE__);
						iThisFunRes = td_failure;
						goto LABLE_TD_MP4_EXIT;
					}
					td_printf(0, "%s %d test!", __FUNCTION__, __LINE__);	
					MP4SetVideoProfileLevel(pstMP4Mgr->m_MP4Handle, 0x01);
					MP4AddH265VideoParameterSet(pstMP4Mgr->m_MP4Handle,pstMP4Mgr->m_VideoTrackID, stNalu.m_pucVedioData, stNalu.m_iVedioSize);
					MP4SetTrackDurationPerChunk(pstMP4Mgr->m_MP4Handle, pstMP4Mgr->m_VideoTrackID, 90000 / pstMP4Mgr->m_stSDVFileHead.FrameRate);
					vps = 1;
				}
			}  		
			else if(stNalu.m_iType == NAL_SPS)
			{//sps
				if(0 == pstMP4Mgr->m_bFirstSpsFlag)
				{
					MP4AddH265SequenceParameterSet(pstMP4Mgr->m_MP4Handle,pstMP4Mgr->m_VideoTrackID, stNalu.m_pucVedioData, stNalu.m_iVedioSize); 
					
					pstMP4Mgr->m_bFirstSpsFlag = 1;
				}
			}  
			else if(stNalu.m_iType == NAL_PPS)
			{//pps
				td_printf(0, "%s %d test!", __FUNCTION__, __LINE__);	
				if(0 == pstMP4Mgr->m_bFirstPpsFlag)
				{
					if(MP4_INVALID_TRACK_ID != pstMP4Mgr->m_VideoTrackID)
					{
						td_printf(0x12345678, "---------------->>>>> write Pps");
						MP4AddH265PictureParameterSet(pstMP4Mgr->m_MP4Handle,pstMP4Mgr->m_VideoTrackID, stNalu.m_pucVedioData, stNalu.m_iVedioSize);
					}
					pstMP4Mgr->m_bFirstPpsFlag = 1;
				}
			} 
			else  if((1 == pstMP4Mgr->m_bFoundSpsPpsFlag)&&
				((NAL_TRAIL_R == stNalu.m_iType) || 
					(NAL_IDR_W_RADL == stNalu.m_iType) ||
					((NAL_SEI_PREFIX == stNalu.m_iType)&&(pstMP4Mgr->m_iIFrameCount > 0))))	
			{
				td_printf(0, "%s %d test!", __FUNCTION__, __LINE__);	
				if (NAL_IDR_W_RADL == stNalu.m_iType)//cwm ÎªidrÖ¡Ê±£¬µ÷ÓÃMP4WriteSample£¬»áÔÚmp4ÏÂÀàÐÍÎªstssµÄboxÉú³ÉÄÚÈÝ
				{
					pstMP4Mgr->m_bIFrameFlag = 1;					
					iIFrameFlag = 1;		
					iFirstFrame = 1;
					PrintInfo("iFirstFrame = 1");
				}
				else if(NAL_TRAIL_R == stNalu.m_iType)
				{
					pstMP4Mgr->m_bIFrameFlag = 0;
					iIFrameFlag = 0;				
				}
				else if(NAL_SEI_PREFIX == stNalu.m_iType)//cwm×Ô¶¨ÒåµÄÊý¾Ý
				{
					pstMP4Mgr->m_bIFrameFlag = 0;
					iIFrameFlag = 6;//ÓÃÓÚ×Ô¶¨ÒåÐÅÏ¢¼ÇÂ¼£¬¶ÏµçÐÞ¸´Ê¹ÓÃ				
				}
				else//²»¿ÉÄÜ×ßµ½Õâ
				{
					td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
					goto LABLE_TD_MP4_EXIT;
				}

				if(stNalu.m_iVedioSize > 0)
				{
					if((NAL_IDR_W_RADL == stNalu.m_iType)
						||(NAL_TRAIL_R == stNalu.m_iType))
					{
						if(MP4_BOX_SIEZ_LENGTH == iTmpMP4SameBufLen)//µÚÒ»¸öslice
						{
							if((iTmpMP4SameBufLen+stNalu.m_iVedioSize) <= pstMP4Mgr->m_iVideoSampleDataBufSize)
							{
								memcpy(pcTmpMP4SameBuf+iTmpMP4SameBufLen,stNalu.m_pucVedioData,stNalu.m_iVedioSize);
								iTmpMP4SameBufLen += stNalu.m_iVedioSize;
							}
							else
							{
								td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
								goto LABLE_TD_MP4_EXIT;
							}
						}
						else
						{
							if((iTmpMP4SameBufLen+4+stNalu.m_iVedioSize) <= pstMP4Mgr->m_iVideoSampleDataBufSize)
							{
								memcpy(pcTmpMP4SameBuf+iTmpMP4SameBufLen,&uiNaluheader,4);
								iTmpMP4SameBufLen += 4;
								memcpy(pcTmpMP4SameBuf+iTmpMP4SameBufLen,stNalu.m_pucVedioData,stNalu.m_iVedioSize);
								iTmpMP4SameBufLen += stNalu.m_iVedioSize;
							}
							else
							{
								td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
								goto LABLE_TD_MP4_EXIT;
							}
						}
						
						iNeedWriteBufToMp4 = 1;//ÐèÒª½«»º´æÖÐµÄÊý¾Ý×ª»»Îªmp4 sample
					}
					else if(NAL_SEI_PREFIX == stNalu.m_iType)
					{
						memcpy(pcTmpMP4SameBuf+iTmpMP4SameBufLen,stNalu.m_pucVedioData,stNalu.m_iVedioSize);
						iTmpMP4SameBufLen += stNalu.m_iVedioSize;
	
						pcTmpMP4SameBuf[0] = (iTmpMP4SameBufLen-MP4_BOX_SIEZ_LENGTH)>>24;  
						pcTmpMP4SameBuf[1] = ((iTmpMP4SameBufLen-MP4_BOX_SIEZ_LENGTH)&0x00ff0000)>>16;	
						pcTmpMP4SameBuf[2] = ((iTmpMP4SameBufLen-MP4_BOX_SIEZ_LENGTH)&0x0000ff00)>>8;  
						pcTmpMP4SameBuf[3] = (iTmpMP4SameBufLen-MP4_BOX_SIEZ_LENGTH)&0x000000ff;	
						
						ullTmpVideoDuration = 0;
						//td_printf(0x12345678, "------------------------->m_uiUselessCount is %u", pstMP4Mgr->m_uiUselessCount);
						
						//td_printf(0, "********************iTmpMP4SameBufLen is %d, iIFrameFlag is %d", iTmpMP4SameBufLen, iIFrameFlag);
						
						if((1 == iFirstFrame) && !MP4WriteSample(pstMP4Mgr->m_MP4Handle, pstMP4Mgr->m_VideoTrackID, pcTmpMP4SameBuf, iTmpMP4SameBufLen, ullTmpVideoDuration, 0, pstMP4Mgr->m_bIFrameFlag))  
						{
							td_printf(0xff0000, "%d: Call MP4WriteSample failed!.\n", __LINE__);
							goto LABLE_TD_MP4_EXIT;
						}
						//Ð´Èë³É¹¦²Å¼ÆÊý
						pstMP4Mgr->m_uiUselessCount++;//ÎÞÐ§Ö¡¼ÆÊýÀÛ¼Ó
						pstMP4Mgr->m_uiVedioSampleID++;
						pstMP4Mgr->m_VedioTotalDuration += ullTmpVideoDuration;
						iTmpMP4SameBufLen = MP4_BOX_SIEZ_LENGTH;//Ð´ÍêÒ»¸öNALUºó£¬ÖØÐÂ¸³Öµ
						iNeedWriteBufToMp4 = 0;//»º´æÊý¾ÝÒÑ×ª»»£¬ÖÃÁã
					}
				}	
			}
			iPos += iAnalyzeRes;  
		} 
		td_printf(0, "%s %d test!", __FUNCTION__, __LINE__);	
		if(1 == iNeedWriteBufToMp4)
		{
			pcTmpMP4SameBuf[0] = (iTmpMP4SameBufLen-MP4_BOX_SIEZ_LENGTH)>>24;  
			pcTmpMP4SameBuf[1] = ((iTmpMP4SameBufLen-MP4_BOX_SIEZ_LENGTH)&0x00ff0000)>>16;	
			pcTmpMP4SameBuf[2] = ((iTmpMP4SameBufLen-MP4_BOX_SIEZ_LENGTH)&0x0000ff00)>>8;  
			pcTmpMP4SameBuf[3] = (iTmpMP4SameBufLen-MP4_BOX_SIEZ_LENGTH)&0x000000ff;	
						
			//td_printf(0, "********************iTmpMP4SameBufLen is %d, iIFrameFlag is %d", iTmpMP4SameBufLen, iIFrameFlag);
			ullTmpVideoDuration = pstMP4Mgr->m_VedioDuration;
			
			if((1 == iFirstFrame) && !MP4WriteSample(pstMP4Mgr->m_MP4Handle, pstMP4Mgr->m_VideoTrackID, pcTmpMP4SameBuf, iTmpMP4SameBufLen, ullTmpVideoDuration, 0, pstMP4Mgr->m_bIFrameFlag))  
			{
				td_printf(0xff0000, "%d: Call MP4WriteSample failed!.\n", __LINE__);
				goto LABLE_TD_MP4_EXIT;
			}
										
			//Ð´Èë³É¹¦²Å¼ÆÊý
			pstMP4Mgr->m_uiVedioSampleID++;
			pstMP4Mgr->m_VedioTotalDuration += ullTmpVideoDuration;

			if (1 == pstMP4Mgr->m_bIFrameFlag)
			{
				pstMP4Mgr->m_iIFrameCount++;
			}
		}
		td_printf(0, "%s %d test!", __FUNCTION__, __LINE__);	
		if((1 != pstMP4Mgr->m_bFirstSpsFlag)||(1 != pstMP4Mgr->m_bFirstPpsFlag))
		{
			td_printf(0xff0000, "%d: have not find SpsPps, go on!\n", __LINE__);
			iThisFunRes = MP4_CONVERT_ERROR_SPSPPS_CONTINUE;
			goto LABLE_TD_MP4_EXIT;
		}
		else
		{
			if(0 == pstMP4Mgr->m_bFoundSpsPpsFlag)
			{
				td_printf(0x12345678, "%d: have found SpsPps, seek from head again!\n", __LINE__);
				
				//ÒÆ¶¯µ½ÎÄ¼þÍ·Î»ÖÃ
				int iSeekRet = 0;
				iSeekRet = td_mp4_fseek((void*)(pstMP4Mgr->m_pfSDVFileHandle), sizeof(S_FileHeader), SEEK_SET);
				
				if(-1 == iSeekRet)
				{
					td_printf(0xff0000, "%d: Seek failed! iSeekRet=%d. Error info[%s].\n", __LINE__, iSeekRet, strerror(errno));
					iThisFunRes = td_failure;
					goto LABLE_TD_MP4_EXIT;
					
				}
		
				pstMP4Mgr->m_bFoundSpsPpsFlag = 1;
				iThisFunRes = MP4_CONVERT_ERROR_SPSPPS_CONTINUE;
				goto LABLE_TD_MP4_EXIT;
			}
		}  
	}
	iThisFunRes = td_success;
	td_printf(0, "%s %d test!", __FUNCTION__, __LINE__);	
LABLE_TD_MP4_EXIT:
	td_printf(0, "%s %d test!", __FUNCTION__, __LINE__);	
	return iThisFunRes;  
	
}


/************************************************* 
  Function: Inner_GetSDVVirtualFrame
  Description: ÓÃÓÚ»ñÈ¡SDV ÐéÄâÖ¡£¬²¢½«ÐéÄâÖ¡Í¨¹ý´«³ö²ÎÊý´«³ö
  Input: _pfSDVFile £¬SDV ÎÄ¼þ¾ä±ú
  Output: _pvSDVVirtualFramBuf £¬½«ÐéÄâÖ¡·ÅÈë¸Ã´«³ö²ÎÊýÖÐ£¬×¢Òâ: ¸Ã²ÎÊýÔÚº¯ÊýÄÚÉêÇëÁËÄÚ´æ£¬Íâ²¿Ê¹ÓÃÊ±ÐèÒªÖ÷¶¯ÊÍ·Å
  Output: _piVirtualFramBufSize £¬½«ÐéÄâÖ¡´óÐ¡·ÅÈë¸Ã´«³ö²ÎÊýÖÐ
  Output: _bHaveSDVVirtualFrameFlag £¬ÎÄ¼þ´æÔÚSDV ÐéÄâÖ¡±êÊ¶
  Return: º¯ÊýÖ´ÐÐ³É¹¦·µ»Ø td_success, Ê§°Ü·µ»Ø td_failure
  Author: »ÆÖ¾¸Õ(Huangzhigang)
  Others: 
*************************************************/
static int Inner_GetSDVVirtualFrame(void* _pstMP4Mgr)
{
	TMP4Mgr* pstMP4Mgr = (TMP4Mgr*)_pstMP4Mgr;
	unsigned int iSDVVirtualFrameSize = 0;
	unsigned int iSDVVirtualFrameTailFlag = 0;
	unsigned int iMallocSize = 0;
	int iThisFunRes = td_success;
	char* pcVirtualFramBuf = NULL;
	int iTmpRes = 0;
	int iTryTimes = 0;
	long SeekOffSet = 0;
	
Begin:
	iTryTimes++;
	
	if(NULL == pstMP4Mgr)
	{
		td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
		iThisFunRes = td_failure;
		goto LABLE_TD_MP4_EXIT;
	}
	td_printf(0, "%d: Inner_GetSDVVirtualFrame begin!\n", __LINE__);
		
	//ÒÆ¶¯µ½ÐéÄâÖ¡Î²Î»ÖÃ
	SeekOffSet = -sizeof(TIFrameIndexTail);
	iTmpRes = td_mp4_fseek((void*)(pstMP4Mgr->m_pfSDVFileHandle), SeekOffSet, SEEK_END);
	
	if(-1 == iTmpRes)
	{
		td_printf(0xff0000, "%d: Seek failed! Error info[%s].\n", __LINE__, strerror(errno));
		iThisFunRes = td_failure;
		goto LABLE_TD_MP4_EXIT;
	}
	//td_printf(0, "%d: Current nonius pos=%ld.\n", __LINE__, td_mp4_ftell((void*)(pstMP4Mgr->m_pfSDVFileHandle)));

	
	//¶ÁÈ¡SDV ÐéÄâÖ¡ Î²±êÊ¶ 0x0123abcd
	iTmpRes = td_mp4_fread(&iSDVVirtualFrameTailFlag, 1, 4, (void*)(pstMP4Mgr->m_pfSDVFileHandle));
	
	if(4 != iTmpRes)
	{
		td_printf(0xff0000, "%d: Read SDV virtual frame size failed!\n", __LINE__);
		iThisFunRes = td_failure;
		goto LABLE_TD_MP4_EXIT;
	}
	
	//td_printf(0, "%d: iSDVVirtualFrameTailFlag=%u.%x\n", __LINE__, iSDVVirtualFrameTailFlag,iSDVVirtualFrameTailFlag);
	//td_printf(0, "%d: Current nonius pos=%ld.\n", __LINE__, td_mp4_ftell((void*)(pstMP4Mgr->m_pfSDVFileHandle)));

	if(0x0123abcd != iSDVVirtualFrameTailFlag)
	{
		td_printf(0xff0000, "%d: iSDVVirtualFrameTailFlag=%x.\n", __LINE__, iSDVVirtualFrameTailFlag);
		iThisFunRes = td_failure;
		goto LABLE_TD_MP4_EXIT;
	}

	//¶ÁÈ¡SDV ÐéÄâÖ¡´óÐ¡
	iTmpRes = td_mp4_fread(&iSDVVirtualFrameSize, 1, 4, (void*)(pstMP4Mgr->m_pfSDVFileHandle));
	
	if(4 != iTmpRes)
	{
		td_printf(0xff0000, "%d: Read SDV virtual frame size failed!\n", __LINE__);
		iThisFunRes = td_failure;
		goto LABLE_TD_MP4_EXIT;
	}
	//td_printf(0, "%d: iSDVVirtualFrameSize=%u.\n", __LINE__, iSDVVirtualFrameSize);
	//td_printf(0, "%d: Current nonius pos=%ld.\n", __LINE__, td_mp4_ftell((void*)(pstMP4Mgr->m_pfSDVFileHandle)));
	
	//ÉêÇëÐéÄâÖ¡ÄÚ´æ
	iMallocSize = iSDVVirtualFrameSize + MP4_BOX_SIEZ_LENGTH + MP4_SAMPLE_TYPE_LENGTH; /*MP4 Ö¡´óÐ¡±äÁ¿4Byte + H264ÊÓÆµµÄSEI Í·1Byte£¬Hex Value:  00 00 00 00 + 06*/
	pcVirtualFramBuf = (char*)malloc(iMallocSize);

	if(NULL == pcVirtualFramBuf)
	{
		td_printf(0xff0000, "%d: Malloc memery failed! Error info[%s].\n", __LINE__, strerror(errno));
		iThisFunRes = td_failure;
		goto LABLE_TD_MP4_EXIT;
	}

	//ÒÆ¶¯µ½ÐéÄâÖ¡Í·Î»ÖÃ
	SeekOffSet = -iSDVVirtualFrameSize;
	iTmpRes = td_mp4_fseek((void*)(pstMP4Mgr->m_pfSDVFileHandle), SeekOffSet, SEEK_END);
	
	if(-1 == iTmpRes)
	{
		td_printf(0xff0000, "%d: Seek failed! iTmpRes=%d. Error info[%s].\n", __LINE__, iTmpRes, strerror(errno));
		iThisFunRes = td_failure;
		goto LABLE_TD_MP4_EXIT;
		
	}
	
	pstMP4Mgr->m_lStopConvertPos = td_mp4_ftell((void*)(pstMP4Mgr->m_pfSDVFileHandle));
	td_printf(0x12345678, "11have virframe m_lStopConvertPos[%ld]", pstMP4Mgr->m_lStopConvertPos);
	td_printf(0x12345678, "22have virframe m_lStopConvertPos[%ld]", pstMP4Mgr->m_lStopConvertPos);
	if(-1 == pstMP4Mgr->m_lStopConvertPos)
	{
		td_printf(0xff0000, "%d: ftell failed! Error info[%s].\n", __LINE__, strerror(errno));
		iThisFunRes = td_failure;
		goto LABLE_TD_MP4_EXIT;
		
	}
	td_printf(0x12345678, "33have virframe m_lStopConvertPos[%ld]", pstMP4Mgr->m_lStopConvertPos);
	//td_printf(0x12345678, "%d: pstMP4Mgr->m_lStopConvertPos=%ld.\n", __LINE__, pstMP4Mgr->m_lStopConvertPos);

	//´òÉÏSEI ±êÊ¶. ±¸×¢: MP4 ´óÐ¡µÄ±êÊ¶×îºóÐ´ÈëÊ±ÔÙ¸³Öµ³ÉÊµ¼Ê´óÐ¡
	pcVirtualFramBuf[MP4_BOX_SIEZ_LENGTH] = 0x06;
	
	//¶ÁÈ¡SDV ÐéÄâÖ¡£¬¸ÃË÷ÒýÊÇÒÔ0x02000000 ¿ªÍ·µÄ
	iTmpRes = td_mp4_fread(pcVirtualFramBuf+MP4_BOX_SIEZ_LENGTH + MP4_SAMPLE_TYPE_LENGTH, 1, iSDVVirtualFrameSize, (void*)(pstMP4Mgr->m_pfSDVFileHandle));/*Ìø¹ý9Bbyte ¿½±´ÐéÄâÖ¡£¬Hex Value:00 00 00 00 + 06	00 00 00 02 ... ...*/

	if(iSDVVirtualFrameSize != iTmpRes)
	{
		td_printf(0xff0000, "%d: Read failed! iTmpRes=%d. Error info[%s].\n", __LINE__, iTmpRes, strerror(errno));
		iThisFunRes = td_failure;
		goto LABLE_TD_MP4_EXIT;
	}
	//td_printf(0, "%d: Current nonius pos=%ld.\n", __LINE__, td_mp4_ftell((void*)(pstMP4Mgr->m_pfSDVFileHandle)));

	//ÏÔÊ¾Hex Value:00 00 00 00 + 06   00 00 00 02£¬¿´ÊÇ·ñÕýÈ·
#ifdef TD_DEBUG
	int i = 0;
	for(i=0; i<13; i++)
	{
		td_printf(0, "%02x ", pcVirtualFramBuf[i]);
	}
#endif			
LABLE_TD_MP4_EXIT:
	if(td_success == iThisFunRes)
	{
		//´«¸øÍâ²¿
		pstMP4Mgr->m_pvSDVVirtualFramebuffer = pcVirtualFramBuf;
		pstMP4Mgr->m_uiSDVVirtualFramebufferSize = iMallocSize;
		pstMP4Mgr->m_bHaveSDVVirtualFrameFlag = HAVE_SDV_VIRTUAL_FRAME_FLAG;
	}
	else
	{
		if(NULL != pcVirtualFramBuf)
		{
			free(pcVirtualFramBuf);
			pcVirtualFramBuf = NULL;
		}

		//´«¸øÍâ²¿
		pstMP4Mgr->m_pvSDVVirtualFramebuffer = NULL;
		pstMP4Mgr->m_uiSDVVirtualFramebufferSize = 0;
		pstMP4Mgr->m_bHaveSDVVirtualFrameFlag = HAVE_NO_SDV_VIRTUAL_FRAME_FLAG;

		td_printf(0xff0000, "%s %s %d Get SDV VirtualFrame Failed! iTryTimes is %d", __FILE__, __FUNCTION__, __LINE__, iTryTimes);

		if(iTryTimes < 2)//Èç¹û¶ÁÈ¡´óÓÚµÈÓÚ2´Î£¬Ôò²»»áÔÙ³¢ÊÔ¶ÁÈ¡
		{
			goto Begin;
		}
	}
	
	td_printf(0, "%d: Inner_GetSDVVirtualFrame end!\n", __LINE__);
	return	iThisFunRes;
}


/************************************************* 
  Function: Inner_AnalyzeSDVVirtualFrameInfo
  Description: ÓÃÓÚ·ÖÎöSDV ÐéÄâÖ¡Í·¡¢Ìå¡¢Î²ÄÚµÄÏêÏ¸ÐÅÏ¢
  Input: _pvSDVVirtualFramBuf £¬Ë÷ÒýÐÅÏ¢Ö¸Õë
  Input: _uiVirtualFramBufSize £¬Ë÷ÒýÐÅÏ¢´óÐ¡
  Input: _pstSDVFrameIndexHead £¬Ö¡Í·Ö¸Õë
  Input: _pstSDVFrameIndexBody £¬°üÌåÖ¸Õë
  Input: _pstSDVAppendCharInfo £¬¸½¼ÓÐÅÏ¢Ö¸Õë
  Input: _pstSDVFrameIndexTail £¬Ö¡Î²Ö¸Õë
  Output: ÎÞ
  Return: º¯ÊýÖ´ÐÐ³É¹¦·µ»Ø td_success, Ê§°Ü·µ»Ø td_failure
  Author: »ÆÖ¾¸Õ(Huangzhigang)
  Others: 
*************************************************/
static int Inner_AnalyzeSDVVirtualFrameInfo(const void* _pvSDVVirtualFramBuf, unsigned int _uiVirtualFramBufSize
						, TIFrameIndexHead** _pstSDVFrameIndexHead, TIFrameIndexInfo** _pstSDVFrameIndexBody
						, TAppendCharInfo** _pstSDVAppendCharInfo, TIFrameIndexTail** _pstSDVFrameIndexTail)
{
	MP4_VOID* pvVirtualFramBuf = NULL;
	int iThisFunRes = td_success;

	TIFrameIndexHead* pstSDVFrameIndexHead = NULL;
	TIFrameIndexInfo* pstSDVFrameIndexBody = NULL;
	TAppendCharInfo*  pstSDVAppendCharInfo = NULL;
	TIFrameIndexTail* pstSDVFrameIndexTail = NULL;
	unsigned int uiPos = 0;
	
	if( (NULL == _pvSDVVirtualFramBuf)
		|| (_uiVirtualFramBufSize <= 0)
		|| (NULL == _pstSDVFrameIndexHead)
		|| (NULL == _pstSDVFrameIndexBody)
		|| (NULL == _pstSDVAppendCharInfo)
		|| (NULL == _pstSDVFrameIndexTail)
		)
	{
		td_printf(0xff0000, "%d: Input parameters is error![%p][%d][%p][%p][%p][%p]\n", __LINE__
			,_pvSDVVirtualFramBuf, _uiVirtualFramBufSize, _pstSDVFrameIndexHead
			, _pstSDVFrameIndexBody, _pstSDVAppendCharInfo, _pstSDVFrameIndexTail);
		iThisFunRes = td_failure;
		goto LABLE_TD_MP4_EXIT;
	}

	pvVirtualFramBuf = (MP4_VOID*)((MP4_VOID*)_pvSDVVirtualFramBuf+MP4_BOX_SIEZ_LENGTH + MP4_SAMPLE_TYPE_LENGTH);
	//¶¨Î»           °üÍ·:  Ö¸Ïòbuffer Ê×Î»ÖÃ¼´¿É
	uiPos = 0;
	pstSDVFrameIndexHead = (TIFrameIndexHead*)(pvVirtualFramBuf + uiPos);
	//¶¨Î»           °üÌå:  Ìø¹ýÖ¡Í·
	uiPos += sizeof(TIFrameIndexHead);
	pstSDVFrameIndexBody = (TIFrameIndexInfo*)(pvVirtualFramBuf + uiPos);
	//¶¨Î»¸½¼ÓÐÅÏ¢:  Ìø¹ýÖ¡Í·+  °üÌå
	#ifdef ZHENGFA
	uiPos += (pstSDVFrameIndexHead->m_iNum + 1) * pstSDVFrameIndexHead->m_iInfoSize;
	#else
	uiPos += pstSDVFrameIndexHead->m_iNum * pstSDVFrameIndexHead->m_iInfoSize;
	#endif
	pstSDVAppendCharInfo = (TAppendCharInfo*)(pvVirtualFramBuf + uiPos);
	/*¶¨Î»           °üÎ²:  Ìø¹ýÖ¡Í·+  °üÌå+  ¸½¼ÓÐÅÏ¢£¬
	±¸×¢TAppendCharInfo ½á¹¹ÖÐm_pcCharInfoÊÇÒ»¸öÖ¸Õë£¬¸ÃÖ¸ÕëÖ¸ÏòÁËMAX_RESERVED_INFO_LEN´óÐ¡µÄÄÚ´æ£¬´Ë´¦²»¿ÉÓÃsizeof ¼ÆËã´óÐ¡*/
	uiPos += MP4_APPEND_CHAR_INFO_SIZE;
	pstSDVFrameIndexTail = (TIFrameIndexTail*)(pvVirtualFramBuf + uiPos);

	//´«¸øÍâ²¿
	*_pstSDVFrameIndexHead = pstSDVFrameIndexHead;
	*_pstSDVFrameIndexBody = pstSDVFrameIndexBody;
	*_pstSDVAppendCharInfo = pstSDVAppendCharInfo;
	*_pstSDVFrameIndexTail = pstSDVFrameIndexTail;

LABLE_TD_MP4_EXIT:
	return iThisFunRes;	
}


/************************************************* 
  Function: Inner_CreateMP4VirtualFrame
  Description: ´´½¨MP4 ÐéÄâÖ¡
  Input: _pvSDVVirtualFramBuf£¬SDV ÐéÄâÖ¡
  Input: _iSDVVirtualFramBufLen£¬SDV ÐéÄâÖ¡³¤¶È
  Output: _pvMP4VirtualFramBuf£¬MP4 ÐéÄâÖ¡
  Output: _piMP4VirtualFramBufLen£¬MP4 ÐéÄâÖ¡³¤¶È
  Return: º¯ÊýÖ´ÐÐ³É¹¦·µ»Ø td_success, Ê§°Ü·µ»Ø td_failure
  Author: »ÆÖ¾¸Õ(Huangzhigang)
  Others: 
*************************************************/
static int Inner_CreateMP4VirtualFrame(void* _pvMP4Mgr, const void* _pvSDVVirtualFramBuf, int _iSDVVirtualFramBufLen, void** _pvMP4VirtualFramBuf, int* _piMP4VirtualFramBufLen)
{
	TMP4Mgr* pvMP4Mgr = (TMP4Mgr*)_pvMP4Mgr;
	void* pvSDVVirtualFramBuf = NULL;
	unsigned char* pucMP4VirtualFramBuf = NULL;
	int iThisFunRes = td_success;
	
	TIFrameIndexHead* pstSDVFrameIndexHead = NULL;
	TIFrameIndexInfo* pstSDVFrameIndexBody = NULL;
	TAppendCharInfo*  pstSDVAppendCharInfo = NULL;
	TIFrameIndexTail* pstSDVFrameIndexTail = NULL;
	int iSDVVirtualFramBufLen = 0;
	int iMP4VirtualFramBufLen = 0;
	
	if((NULL == pvMP4Mgr)||(NULL == _pvMP4VirtualFramBuf)||(NULL == _piMP4VirtualFramBufLen))
	{
		td_printf(0xff0000, "%d: Input parameters is error!\n", __LINE__);
		iThisFunRes = td_failure;
		goto LABLE_TD_MP4_EXIT;
	}
	
	pvSDVVirtualFramBuf = (void*)_pvSDVVirtualFramBuf;
	iSDVVirtualFramBufLen = _iSDVVirtualFramBufLen;
	
	if(HAVE_SDV_VIRTUAL_FRAME_FLAG == pvMP4Mgr->m_bHaveSDVVirtualFrameFlag)
	{
		//»ñÈ¡SDV ÎÄ¼þÐéÄâÖ¡ÐÅÏ¢
		if(Inner_AnalyzeSDVVirtualFrameInfo(pvSDVVirtualFramBuf, iSDVVirtualFramBufLen,&pstSDVFrameIndexHead,
					&pstSDVFrameIndexBody, &pstSDVAppendCharInfo, &pstSDVFrameIndexTail) < 0)
		{  
			td_printf(0xff0000, "%d: Call Inner_AnalyzeSDVVirtualFrameInfo failed!\n", __LINE__);
			iThisFunRes = td_failure;
			goto LABLE_TD_MP4_EXIT;
		}

		#ifdef ZHENGFA
		/*Hex Value:  (00 00 00 00 + 00 00 00 01 66) 9Byte µÄMP4Ö¡´óÐ¡ºÍSEIÐÅÏ¢
		9 +  MP4ÐéÄâÖ¡Í·MP4ÐéÄâÖ¡°üÌå  +¹âÅÌÐòÁÐºÅÐéÄâÐÅÏ¢	+MP4ÐéÄâÖ¡¸½¼ÓÐÅÏ¢+  MP4ÐéÄâÖ¡Î²
		*/
			iMP4VirtualFramBufLen = MP4_BOX_SIEZ_LENGTH + MP4_SAMPLE_TYPE_LENGTH + sizeof(TMP4ISampleIndexHead) + (pstSDVFrameIndexHead->m_iNum + 1) * sizeof(TMP4ISampleIndexInfo) 
								+ MP4_APPEND_CHAR_INFO_SIZE + sizeof(TIFrameIndexTail);
		#else
		iMP4VirtualFramBufLen = MP4_BOX_SIEZ_LENGTH + MP4_SAMPLE_TYPE_LENGTH + sizeof(TMP4ISampleIndexHead) + pstSDVFrameIndexHead->m_iNum* sizeof(TMP4ISampleIndexInfo) 
								+ MP4_APPEND_CHAR_INFO_SIZE + sizeof(TIFrameIndexTail);
		#endif
	}
	else
	{
		iMP4VirtualFramBufLen = MP4_BOX_SIEZ_LENGTH + MP4_SAMPLE_TYPE_LENGTH + sizeof(TMP4ISampleIndexHead) + MP4_APPEND_CHAR_INFO_SIZE + sizeof(TIFrameIndexTail);
	}
		
	pucMP4VirtualFramBuf = (unsigned char*)malloc(iMP4VirtualFramBufLen);
	
	if(NULL == pucMP4VirtualFramBuf)
	{
		td_printf(0xff0000, "%d: Malloc memery for pvMP4VirtualFramBuf failed! Error info[%s].\n", __LINE__, strerror(errno));
		iThisFunRes = td_failure;
		goto LABLE_TD_MP4_EXIT;
	}
	
	pucMP4VirtualFramBuf[MP4_BOX_SIEZ_LENGTH] = 0x06;
	
	//´«¸øÍâ²¿
	*_pvMP4VirtualFramBuf = (void*)pucMP4VirtualFramBuf;
	*_piMP4VirtualFramBufLen = iMP4VirtualFramBufLen;

		
LABLE_TD_MP4_EXIT:
	if((td_failure == iThisFunRes)&&(NULL != pucMP4VirtualFramBuf))
	{
		free(pucMP4VirtualFramBuf);
		pucMP4VirtualFramBuf = NULL;
	}
	return iThisFunRes;
}

/************************************************* 
  Function: Inner_InitMP4Mgr
  Description: ÓÃÓÚ³õÊ¼»¯²¢´´½¨¹ÜÀí½á¹¹
  Input: _pcSrcFilePath £¬Ô´SDV ÎÄ¼þÂ·¾¶
  Input: _DstFilePath £¬Ä¿±ê´ýÉú³ÉµÄMP4 ÎÄ¼þÂ·¾¶
  Output: _pvMP4Mgr£¬´«³ö²ÎÊý£¬ÓÃÓÚ½ÓÊÕ¹ÜÀí½á¹¹µÄÖ¸Õë
  Return: º¯ÊýÖ´ÐÐ³É¹¦·µ»Ø td_success, Ê§°Ü·µ»Ø td_failure
  Author: »ÆÖ¾¸Õ(Huangzhigang)
  Others: 
*************************************************/
static int Inner_InitMP4Mgr(void** _pvMP4Mgr, const char* _pcSrcFilePath, const char* _pcDstFilePath)
{	
	TMP4Mgr* pstMP4Mgr = NULL;
	int iThisFunRes = td_success;
	int iSeekRet = 0;
	
	if(NULL == _pcSrcFilePath || NULL == _pcDstFilePath)
	{
		td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
		iThisFunRes = td_failure;
		goto LABLE_TD_MP4_EXIT;
	}
	td_printf(0, "%d: Inner_InitMP4Mgr begin!\n", __LINE__);

	//ÉêÇë¹ÜÀíÆ÷ÄÚ´æ²¢³õÊ¼»¯
	pstMP4Mgr = (TMP4Mgr*)malloc(sizeof(TMP4Mgr));
	
	if(NULL == pstMP4Mgr)
	{
		td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
		iThisFunRes = td_failure;
		goto LABLE_TD_MP4_EXIT;
	}
	//----------------------------------------
	memset((void*)pstMP4Mgr, 0, sizeof(TMP4Mgr));
	pstMP4Mgr->m_bCryptFlag              = 1;
	pstMP4Mgr->m_pfSDVFileHandle         = NULL;
	pstMP4Mgr->m_MP4Handle 				 = NULL;
	pstMP4Mgr->m_pvSDVVirtualFramebuffer = NULL;
	pstMP4Mgr->m_pvMP4VirtualFramebuffer = NULL;
	pstMP4Mgr->m_ucAVDataBuf = (unsigned char*)malloc(MAX_VIDEO_SAMPLE_SIZE + MAX_AUDIO_SAMPLE_SIZE);

	if(NULL == pstMP4Mgr->m_ucAVDataBuf)
	{
		td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
		iThisFunRes = td_failure;
		goto LABLE_TD_MP4_EXIT;
	}
	
	pstMP4Mgr->m_iAVDataBufSize = MAX_VIDEO_SAMPLE_SIZE + MAX_AUDIO_SAMPLE_SIZE;
	
	pstMP4Mgr->m_ucVideoSampleDataBuf = (unsigned char*)malloc(MAX_VIDEO_SAMPLE_SIZE + MP4_BOX_SIEZ_LENGTH);

	if(NULL == pstMP4Mgr->m_ucVideoSampleDataBuf)
	{
		td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
		iThisFunRes = td_failure;
		goto LABLE_TD_MP4_EXIT;
	}
	
	pstMP4Mgr->m_iVideoSampleDataBufSize = MAX_VIDEO_SAMPLE_SIZE + MP4_BOX_SIEZ_LENGTH;
	//----------------------------------------
	//¿½±´ÎÄ¼þÂ·¾¶
	strncpy(pstMP4Mgr->m_cSDVFilePath, _pcSrcFilePath, FILENAMEMAXNUM - 1);
	strncpy(pstMP4Mgr->m_cMP4FilePath, _pcDstFilePath, FILENAMEMAXNUM - 1);
	td_printf(0, "%d: SDV file path is:\n%s\nMP4 file is:\n%s\n", __LINE__, pstMP4Mgr->m_cSDVFilePath, pstMP4Mgr->m_cMP4FilePath);
	pstMP4Mgr->m_bConvertExitFlag = TD_MP4_CONVERT_RUNNING;
	
	//´ò¿ªSDV ÎÄ¼þ
#ifdef TD_VFS
	pstMP4Mgr->m_pfSDVFileHandle = (TVfile*)td_mp4_fopen(NULL, pstMP4Mgr->m_cSDVFilePath, "rb");
#else
	pstMP4Mgr->m_pfSDVFileHandle = (FILE*)td_mp4_fopen(NULL, pstMP4Mgr->m_cSDVFilePath, "rb");
#endif
	
	if(NULL == pstMP4Mgr->m_pfSDVFileHandle)
	{
		td_printf(0xff0000, "%d: Open SDV file failed! Error info[%s].\n", __LINE__, strerror(errno));
		iThisFunRes = td_failure;
		goto LABLE_TD_MP4_EXIT;
	}
	
	//´ò¿ªMP4 ÎÄ¼þ²¢ÉèÖÃÊÓÆµ²ÉÑùÆµÂÊ90000
	pstMP4Mgr->m_MP4Handle = MP4CreateEx(pstMP4Mgr->m_cMP4FilePath, 0, 1, 1, 0, 1, 0, 1);
	
	if (MP4_INVALID_FILE_HANDLE == pstMP4Mgr->m_MP4Handle)  
	{  
		PrintInfo("MP4CreateEx failed");
		td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
		iThisFunRes = td_failure;
		goto LABLE_TD_MP4_EXIT;
	}

	//ÒÆ¶¯µ½ÎÄ¼þÎ²Î»ÖÃ
	iSeekRet = td_mp4_fseek((void*)(pstMP4Mgr->m_pfSDVFileHandle), 0, SEEK_END);

	if(-1 == iSeekRet)
	{
		td_printf(0xff0000, "%d: Seek failed! Error info[%s].\n", __LINE__, strerror(errno));
		iThisFunRes = td_failure;
		goto LABLE_TD_MP4_EXIT;
	}
	
	pstMP4Mgr->m_lFileStopPos = td_mp4_ftell((void*)(pstMP4Mgr->m_pfSDVFileHandle));
	
	if(-1 == pstMP4Mgr->m_lFileStopPos)
	{
		td_printf(0xff0000, "%d: ftell failed! Error info[%s].\n", __LINE__, strerror(errno));
		iThisFunRes = td_failure;
		goto LABLE_TD_MP4_EXIT;
		
	}

	//ÒÆ¶¯µ½ÎÄ¼þÍ·Î»ÖÃ
	iSeekRet = td_mp4_fseek((void*)(pstMP4Mgr->m_pfSDVFileHandle), 0, SEEK_SET);

	if(-1 == iSeekRet)
	{
		td_printf(0xff0000, "%d: Seek failed! Error info[%s].\n", __LINE__, strerror(errno));
		iThisFunRes = td_failure;
		goto LABLE_TD_MP4_EXIT;
	}
	
	MP4SetTimeScale(pstMP4Mgr->m_MP4Handle, 90000);

	//»ñÈ¡SDV ÎÄ¼þÐéÄâÖ¡
	if(Inner_GetSDVVirtualFrame((void*)pstMP4Mgr))
	{  
		td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
		pstMP4Mgr->m_lStopConvertPos = pstMP4Mgr->m_lFileStopPos;
		td_printf(0xff0000, "%d: pstMP4Mgr->m_lFileStopPos=%ld.\n", __LINE__, pstMP4Mgr->m_lFileStopPos);
		td_printf(0xff0000, "%d: pstMP4Mgr->m_lStopConvertPos=%ld.\n", __LINE__, pstMP4Mgr->m_lStopConvertPos);
	}
	td_printf(0x12345678, "44 m_lStopConvertPos[%ld]", pstMP4Mgr->m_lStopConvertPos);
	if(HAVE_SDV_VIRTUAL_FRAME_FLAG != pstMP4Mgr->m_bHaveSDVVirtualFrameFlag)
	{
		td_printf(0xff0000, "%s %s %d error! pstMP4Mgr->m_bHaveSDVVirtualFrameFlag is %x\n", __FILE__, __FUNCTION__, __LINE__, pstMP4Mgr->m_bHaveSDVVirtualFrameFlag);
	}
	
	//´´½¨MP4 ÎÄ¼þÐéÄâÖ¡
	if(Inner_CreateMP4VirtualFrame(pstMP4Mgr, pstMP4Mgr->m_pvSDVVirtualFramebuffer, pstMP4Mgr->m_uiSDVVirtualFramebufferSize,
			&pstMP4Mgr->m_pvMP4VirtualFramebuffer, &pstMP4Mgr->m_uiMP4VirtualFramebufferSize) < 0)
	{  
		td_printf(0xff0000, "%d: Call Inner_CreateMP4VirtualFrame failed!\n", __LINE__);
		iThisFunRes = td_failure;
		goto LABLE_TD_MP4_EXIT;
	}
	td_printf(0x12345678, "55 m_lStopConvertPos[%ld]", pstMP4Mgr->m_lStopConvertPos);
	//td_printf(0, "%d: ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^[%p][%p]\n", __LINE__, pstMP4Mgr->m_pvMP4VirtualFramebuffer,
	//pstMP4Mgr->m_pvMP4VirtualFramebuffer+MP4_BOX_SIEZ_LENGTH + MP4_SAMPLE_TYPE_LENGTH);
	
LABLE_TD_MP4_EXIT:
	
	if(td_success == iThisFunRes)
	{/*¹Ø±ÕSDV ÎÄ¼þ¾ä±ú*/
		//´«¸øÍâ²¿
		td_printf(0, "========================>iThisFunRes is %d", iThisFunRes);
		*_pvMP4Mgr = (void*)pstMP4Mgr;
	}
	
	if((td_failure == iThisFunRes) && (NULL != pstMP4Mgr) && (NULL != pstMP4Mgr->m_pfSDVFileHandle))
	{/*¹Ø±ÕSDV ÎÄ¼þ¾ä±ú*/
		td_mp4_fclose((void*)pstMP4Mgr->m_pfSDVFileHandle);
		pstMP4Mgr->m_pfSDVFileHandle = NULL;
	}
	if((td_failure == iThisFunRes) && (NULL != pstMP4Mgr) && (NULL != pstMP4Mgr->m_MP4Handle))
	{/*¹Ø±ÕMP4 ÎÄ¼þ¾ä±ú*/
		MP4Close(pstMP4Mgr->m_MP4Handle, MP4_CLOSE_DO_NOT_COMPUTE_BITRATE);
		pstMP4Mgr->m_MP4Handle = NULL;
	}
	if((td_failure == iThisFunRes) && (NULL != pstMP4Mgr) && (NULL != pstMP4Mgr->m_pvSDVVirtualFramebuffer))
	{/*ÊÍ·ÅSDV ÐéÄâÖ¡ÄÚ´æ*/
		free(pstMP4Mgr->m_pvSDVVirtualFramebuffer);
		pstMP4Mgr->m_pvSDVVirtualFramebuffer = NULL;
	}
	if((td_failure == iThisFunRes) && (NULL != pstMP4Mgr) && (NULL != pstMP4Mgr->m_pvMP4VirtualFramebuffer))
	{/*ÊÍ·ÅMP4 ÐéÄâÖ¡ÄÚ´æ*/
		free(pstMP4Mgr->m_pvMP4VirtualFramebuffer);
		pstMP4Mgr->m_pvMP4VirtualFramebuffer = NULL;
	}
	if((td_failure == iThisFunRes) && (NULL != pstMP4Mgr))
	{/*ÊÍ·Å¹ÜÀí½á¹¹ÄÚ´æ*/
		free(pstMP4Mgr);
		pstMP4Mgr = NULL;
	}
	td_printf(0x12345678, "66 m_lStopConvertPos[%ld]", pstMP4Mgr->m_lStopConvertPos);
	td_printf(0, "-----------------1111------------------");
	td_printf(0, "pstMP4Mgr->m_cMP4FilePath is %s", pstMP4Mgr->m_cMP4FilePath);
	td_printf(0, "pstMP4Mgr->m_bHaveSDVVirtualFrameFlag is %d", pstMP4Mgr->m_bHaveSDVVirtualFrameFlag);
	td_printf(0, "pstMP4Mgr->m_lStartConvertPos is %ld", pstMP4Mgr->m_lStartConvertPos);
	td_printf(0, "pstMP4Mgr->m_lStopConvertPos is %ld", pstMP4Mgr->m_lStopConvertPos);
	td_printf(0, "pstMP4Mgr->m_lFileStopPos is %ld", pstMP4Mgr->m_lFileStopPos);
	td_printf(0, "pstMP4Mgr->m_ucVideoSampleDataBuf is %p", pstMP4Mgr->m_ucVideoSampleDataBuf);
	td_printf(0, "-----------------1111------------------");
	td_printf(0, "%d: Inner_InitMP4Mgr end! *_pvMP4Mgr is %p, pstMP4Mgr is %p\n", __LINE__, *_pvMP4Mgr, pstMP4Mgr);	
	td_printf(0, "-----------------2222------------------");
	td_printf(0, "((TMP4Mgr*)(*_pvMP4Mgr))->m_cMP4FilePath is %s", ((TMP4Mgr*)(*_pvMP4Mgr))->m_cMP4FilePath);
	td_printf(0, "((TMP4Mgr*)(*_pvMP4Mgr))->m_bHaveSDVVirtualFrameFlag is %d", ((TMP4Mgr*)(*_pvMP4Mgr))->m_bHaveSDVVirtualFrameFlag);
	td_printf(0, "((TMP4Mgr*)(*_pvMP4Mgr))->m_lStartConvertPos is %ld", ((TMP4Mgr*)(*_pvMP4Mgr))->m_lStartConvertPos);
	td_printf(0, "((TMP4Mgr*)(*_pvMP4Mgr))->m_lStopConvertPos is %ld", ((TMP4Mgr*)(*_pvMP4Mgr))->m_lStopConvertPos);
	td_printf(0, "((TMP4Mgr*)(*_pvMP4Mgr))->m_lFileStopPos is %ld", ((TMP4Mgr*)(*_pvMP4Mgr))->m_lFileStopPos);
	td_printf(0, "((TMP4Mgr*)(*_pvMP4Mgr))->m_ucVideoSampleDataBuf is %p", ((TMP4Mgr*)(*_pvMP4Mgr))->m_ucVideoSampleDataBuf);
	td_printf(0, "-----------------2222------------------");
	return iThisFunRes;	
}


/************************************************* 
  Function: Inner_DestoryMP4Mgr
  Description: Ïú»Ù×ÊÔ´
  Input: _pvMP4Mgr £¬MP4 ¹ÜÀí½á¹¹
  Output: ÎÞ
  Return: void
  Author: »ÆÖ¾¸Õ(Huangzhigang)
  Others: 
*************************************************/
static void Inner_DestoryMP4Mgr(void* _pvMP4Mgr)
{
	TMP4Mgr* pstMP4Mgr = (TMP4Mgr*)_pvMP4Mgr;
	if(NULL == pstMP4Mgr)
	{
		td_printf(0xff0000, "%d: Input parameters is error!\n", __LINE__);
		return;
	}
	td_printf(0, "%d: Inner_DestoryMP4Mgr begin!-----111\n", __LINE__);	
	pstMP4Mgr->m_bConvertExitFlag = TD_MP4_CONVERT_STOP;
	
	if((NULL != pstMP4Mgr) && (NULL != pstMP4Mgr->m_pfSDVFileHandle))
	{/*¹Ø±ÕSDV ÎÄ¼þ¾ä±ú*/
		td_mp4_fclose((void*)pstMP4Mgr->m_pfSDVFileHandle);
		pstMP4Mgr->m_pfSDVFileHandle = NULL;
	}
	td_printf(0, "%d: Inner_DestoryMP4Mgr begin!-----222\n", __LINE__);
	if((NULL != pstMP4Mgr) && (NULL != pstMP4Mgr->m_MP4Handle))
	{/*¹Ø±ÕMP4 ÎÄ¼þ¾ä±ú*/
		MP4Close(pstMP4Mgr->m_MP4Handle, MP4_CLOSE_DO_NOT_COMPUTE_BITRATE);
		pstMP4Mgr->m_MP4Handle = NULL;
	}
	td_printf(0, "%d: Inner_DestoryMP4Mgr begin!-----333\n", __LINE__);
	if((NULL != pstMP4Mgr) && (NULL != pstMP4Mgr->m_pvSDVVirtualFramebuffer))
	{/*ÊÍ·ÅSDV ÐéÄâÖ¡ÄÚ´æ*/
		free(pstMP4Mgr->m_pvSDVVirtualFramebuffer);
		pstMP4Mgr->m_pvSDVVirtualFramebuffer = NULL;
	}
	td_printf(0, "%d: Inner_DestoryMP4Mgr begin!-----444\n", __LINE__);
	if((NULL != pstMP4Mgr) && (NULL != pstMP4Mgr->m_pvMP4VirtualFramebuffer))
	{/*ÊÍ·ÅMP4 ÐéÄâÖ¡ÄÚ´æ*/
		free(pstMP4Mgr->m_pvMP4VirtualFramebuffer);
		pstMP4Mgr->m_pvMP4VirtualFramebuffer = NULL;
	}
	if((NULL != pstMP4Mgr) && (NULL != pstMP4Mgr->m_ucAVDataBuf))
	{
		free(pstMP4Mgr->m_ucAVDataBuf);
		pstMP4Mgr->m_ucAVDataBuf = NULL;
	}
	if((NULL != pstMP4Mgr) && (NULL != pstMP4Mgr->m_ucVideoSampleDataBuf))
	{
		free(pstMP4Mgr->m_ucVideoSampleDataBuf);
		pstMP4Mgr->m_ucVideoSampleDataBuf = NULL;
	}
	td_printf(0, "%d: Inner_DestoryMP4Mgr begin!-----555\n", __LINE__);
	if(NULL != pstMP4Mgr)
	{/*ÊÍ·Å¹ÜÀí½á¹¹ÄÚ´æ*/
		free(pstMP4Mgr);
		pstMP4Mgr = NULL;
	}
	
	td_printf(0, "%d: Inner_DestoryMP4Mgr end!\n", __LINE__);
}

/************************************************* 
  Function: Inner_GetSDVFileHeadInfo
  Description: »ñÈ¡SDV ÎÄ¼þÍ·ÐÅÏ¢
  Input: _pvMP4Mgr£¬MP4 ¹ÜÀí½á¹¹
  Input: pvSDVFileHeadBuf£¬ÎÄ¼þÍ·½á¹¹Ìå
  Input: iSDVFileHeadBufLen£¬ÎÄ¼þÍ·½á¹¹Ìå³¤¶È
  Output: 
  Return: º¯ÊýÖ´ÐÐ³É¹¦·µ»Ø td_success, Ê§°Ü·µ»Ø td_failure
  Author: »ÆÖ¾¸Õ(Huangzhigang)
  Others: 
*************************************************/
static int Inner_GetSDVFileHeadInfo(void* _pvMP4Mgr, void* pvSDVFileHeadBuf, int iSDVFileHeadBufLen)
{
	TMP4Mgr* pstMP4Mgr = (TMP4Mgr*)_pvMP4Mgr;
	S_FileHeader* pstFileHead = NULL;
#ifdef TD_VFS
	TVfile* pfSDVFile = NULL;
#else
	FILE*	pfSDVFile = NULL;
#endif
	int iThisFunRes = td_success;
	int iTmpRes = 0;
	
	if((NULL == pstMP4Mgr) ||(NULL == pstMP4Mgr->m_pfSDVFileHandle) || (NULL == pvSDVFileHeadBuf) || (sizeof(S_FileHeader) > iSDVFileHeadBufLen))
	{
		td_printf(0xff0000, "%d: Input parameters is error![%p][%d]\n", __LINE__,pvSDVFileHeadBuf, iSDVFileHeadBufLen);
		iThisFunRes = td_failure;
		goto LABLE_TD_MP4_EXIT;
	}

	//td_printf(0, "%d: Inner_AnalyzeSDVFileHead begin!\n", __LINE__);

#ifdef TD_VFS
	pfSDVFile = (TVfile*)(pstMP4Mgr->m_pfSDVFileHandle);
#else
	pfSDVFile = (FILE*)(pstMP4Mgr->m_pfSDVFileHandle);
#endif
	pstFileHead = (S_FileHeader*)pvSDVFileHeadBuf;
	
	//Ìøµ½ÎÄ¼þÊ×Î»ÖÃ
	iTmpRes = td_mp4_fseek((void*)pfSDVFile, 0, SEEK_SET);
	
	if(-1 == iTmpRes)
	{
		td_printf(0xff0000, "%d: Seek failed! iTmpRes=%d. Error info[%s].\n", __LINE__, iTmpRes, strerror(errno));
		iThisFunRes = td_failure;
		goto LABLE_TD_MP4_EXIT;
		
	}
	//td_printf(0, "%d: Current nonius pos=%ld.\n", __LINE__, td_mp4_ftell((void*)pfSDVFile));
	
	//¶ÁÈ¡SDV ÎÄ¼þÍ·
	iTmpRes = td_mp4_fread((void*)pstFileHead, 1, sizeof(S_FileHeader), (void*)pfSDVFile);
	
	if(sizeof(S_FileHeader) != iTmpRes)
	{
		td_printf(0xff0000, "%d: Read SDV virtual frame size failed!\n", __LINE__);
		iThisFunRes = td_failure;
		goto LABLE_TD_MP4_EXIT;
	}
	//td_printf(0, "%d: Current nonius pos=%ld.\n", __LINE__, td_mp4_ftell((void*)pfSDVFile));	
LABLE_TD_MP4_EXIT:
	
	//td_printf(0, "%d: Inner_AnalyzeSDVFileHead end!\n", __LINE__);
	return iThisFunRes;
	
}

static void Inner_ShowSDVFileHeadInfo(const void* pstSDVFileHeadBuf, int iSDVFileHeadBufLen)
{
	S_FileHeader* pstFileHead = NULL;

	pstFileHead = (S_FileHeader*)pstSDVFileHeadBuf;
	
 	if((NULL == pstSDVFileHeadBuf) || (sizeof(S_FileHeader) > iSDVFileHeadBufLen))
	{
		td_printf(0xff0000, "%d: Input parameters is error![%p][%d]\n", __LINE__, pstSDVFileHeadBuf, iSDVFileHeadBufLen);
		goto LABLE_TD_MP4_EXIT;
	}
	
	if(NULL != pstFileHead)
	{
		td_printf(0,"%d:\nSDVFileHead Info begin:\n", __LINE__);
		td_printf(0," AChannels      =%d\n",pstFileHead->AChannels);
		td_printf(0," AudioSample    =%u\n",pstFileHead->AudioSample);
		td_printf(0," bAudio         =%d\n",pstFileHead->bAudio);
		td_printf(0," BitsPerSample  =%d\n",pstFileHead->BitsPerSample);
		td_printf(0," cCovWord       =%s\n",pstFileHead->cCovWord);
		td_printf(0," CoverMask      =%u\n",pstFileHead->CoverMask);
		td_printf(0," Day            =%d\n",pstFileHead->Day);
		td_printf(0," FrameCount     =%u\n",pstFileHead->FrameCount);
		td_printf(0," FrameRate      =%d\n",pstFileHead->FrameRate);
		td_printf(0," Height         =%d\n",pstFileHead->Height);
		td_printf(0," Hour           =%d\n",pstFileHead->Hour);
		td_printf(0," Minute         =%d\n",pstFileHead->Minute);
		td_printf(0," Mode           =%d\n",pstFileHead->Mode);
		td_printf(0," Month          =%d\n",pstFileHead->Month);
		td_printf(0," m_uiFrameNoDiff=%u\n",pstFileHead->m_uiFrameNoDiff);
		td_printf(0," reserved       =%d\n",pstFileHead->reserved);
		td_printf(0," TotalSize      =%d\n",pstFileHead->Second);
		td_printf(0," TotalSize      =%d\n",pstFileHead->TotalSize);
		td_printf(0," TriggerHigh    =%d\n",pstFileHead->TriggerHigh);
		td_printf(0," TriggerLow     =%d\n",pstFileHead->TriggerLow);
		td_printf(0," Width          =%d\n",pstFileHead->Width);
		td_printf(0," Year           =%d\n",pstFileHead->Year);
		td_printf(0,"%d:\nSDVFileHead Info end.\n", __LINE__);
	}
	
LABLE_TD_MP4_EXIT:
	return;	
}

/************************************************* 
  Function: Inner_AnalyzeMP4VirtualFrameInfo
  Description: ·ÖÎöMP4 ÐéÄâÖ¡
  Input: _pvMP4VirtualFramBuf £¬MP4 ÐéÄâÖ¡Ö¸Õë
  Input: _uiVirtualFramBufSize £¬MP4 ÐéÄâÖ¡³¤¶È
  Input: _MP4UnitCount £¬´«Èë´óÓÚ0µÄÖµÔòÊ¹ÓÃ¸ÃÖµ×÷ÎªI Ö¡×ÜÊý£¬-1 Ê¹ÓÃÊý¾ÝÖÐÊµ¼Ê°üº¬µÄÖµ
  Output: _pstMP4FrameIndexHead£¬Í·Ö¸Õë
  Output: _pstMP4FrameIndexBody£¬°üÌåÖ¸Õë
  Output: _pstMP4AppendCharInfo£¬¸½¼ÓÐÅÏ¢Ö¸Õë
  Output: _pstMP4FrameIndexTail£¬Î²Ö¸Õë
  Return: º¯ÊýÖ´ÐÐ³É¹¦·µ»Ø td_success, Ê§°Ü·µ»Ø td_failure
  Author: »ÆÖ¾¸Õ(Huangzhigang)
  Others: 
*************************************************/
static int Inner_AnalyzeMP4VirtualFrameInfo(const void* _pvMP4VirtualFramBuf, unsigned int _uiVirtualFramBufSize
						, TMP4ISampleIndexHead** _pstMP4FrameIndexHead, TMP4ISampleIndexInfo** _pstMP4FrameIndexBody
						, TAppendCharInfo** _pstMP4AppendCharInfo, TIFrameIndexTail** _pstMP4FrameIndexTail
						, int _SDVUnitCount)
{
	int MP4UnitSize = 0;
	int MP4UnitCount = 0;
	unsigned int uiPos = 0;
	MP4_VOID* pvVirtualFramBuf = NULL;
	int iThisFunRes = td_failure;
	TMP4ISampleIndexHead* pstMP4FrameIndexHead = NULL;
	TMP4ISampleIndexInfo* pstMP4FrameIndexBody = NULL;
	TAppendCharInfo*	  pstMP4AppendCharInfo = NULL;
	TIFrameIndexTail*	  pstMP4FrameIndexTail = NULL;
	
	if( (NULL == _pvMP4VirtualFramBuf)
		|| (_uiVirtualFramBufSize <= 0)
		|| (NULL == _pstMP4FrameIndexHead)
		|| (NULL == _pstMP4FrameIndexBody)
		|| (NULL == _pstMP4AppendCharInfo)
		|| (NULL == _pstMP4FrameIndexTail))
	{
		td_printf(0xff0000, "%d: Input parameters is error![%p][%d][%p][%p][%p][%p][%d]\n", __LINE__
			,_pvMP4VirtualFramBuf, _uiVirtualFramBufSize, _pstMP4FrameIndexHead
			, _pstMP4FrameIndexBody, _pstMP4AppendCharInfo, _pstMP4FrameIndexTail, _SDVUnitCount);
		iThisFunRes = td_failure;
		goto LABLE_TD_MP4_EXIT;
	}

	pvVirtualFramBuf = (MP4_VOID*)((MP4_VOID*)_pvMP4VirtualFramBuf+MP4_BOX_SIEZ_LENGTH + MP4_SAMPLE_TYPE_LENGTH);
	MP4UnitCount = _SDVUnitCount;
	MP4UnitSize = sizeof(TMP4ISampleIndexInfo);
	
	//¶¨Î»           °üÍ·:  Ö¸Ïòbuffer Ê×Î»ÖÃ¼´¿É
	uiPos = 0;
	pstMP4FrameIndexHead = (TMP4ISampleIndexHead*)(pvVirtualFramBuf + uiPos);
	//td_printf(0,"%d:**************************pstMP4FrameIndexHead=[%p], uiPos=[%u]\n", __LINE__, pstMP4FrameIndexHead, uiPos);
	//¶¨Î»           °üÌå:  Ìø¹ýÖ¡Í·
	uiPos += sizeof(TMP4ISampleIndexHead);
	pstMP4FrameIndexBody = (TMP4ISampleIndexInfo*)(pvVirtualFramBuf + uiPos);
	//td_printf(0,"%d:**************************pstMP4FrameIndexBody=[%p], uiPos=[%u]\n", __LINE__, pstMP4FrameIndexBody, uiPos);
	//¶¨Î»¸½¼ÓÐÅÏ¢:  Ìø¹ýÖ¡Í·+  °üÌå
	if(MP4UnitCount > 0)
	{
		#ifdef ZHENGFA
		uiPos += (MP4UnitCount + 1) * MP4UnitSize;
		#else
		uiPos += MP4UnitCount * MP4UnitSize;
		#endif
	}
	
	pstMP4AppendCharInfo = (TAppendCharInfo*)(pvVirtualFramBuf + uiPos);
	//td_printf(0,"%d:**************************pstMP4AppendCharInfo=[%p], uiPos=[%u]MP4UnitCount=[%d]MP4UnitSize=[%d]\n"
	//											, __LINE__, pstMP4AppendCharInfo, uiPos, MP4UnitCount, MP4UnitSize);
	/*¶¨Î»           °üÎ²:  Ìø¹ýÖ¡Í·+  °üÌå+  ¸½¼ÓÐÅÏ¢£¬
	±¸×¢TAppendCharInfo ½á¹¹ÖÐm_pcCharInfoÊÇÒ»¸öÖ¸Õë£¬¸ÃÖ¸ÕëÖ¸ÏòÁËMAX_RESERVED_INFO_LEN´óÐ¡µÄÄÚ´æ£¬´Ë´¦²»¿ÉÓÃsizeof ¼ÆËã´óÐ¡*/
	uiPos += MP4_APPEND_CHAR_INFO_SIZE;
	pstMP4FrameIndexTail = (TIFrameIndexTail*)(pvVirtualFramBuf + uiPos);
	//td_printf(0,"%d:**************************pstMP4FrameIndexTail=[%p], uiPos=[%u]\n", __LINE__, pstMP4FrameIndexTail, uiPos);

	//´«¸øÍâ²¿
	*_pstMP4FrameIndexHead = pstMP4FrameIndexHead;
	*_pstMP4FrameIndexBody = pstMP4FrameIndexBody;
	*_pstMP4AppendCharInfo = pstMP4AppendCharInfo;
	*_pstMP4FrameIndexTail = pstMP4FrameIndexTail;

	iThisFunRes = td_success;

LABLE_TD_MP4_EXIT:
	
	return iThisFunRes;
}

/************************************************* 
  Function: Inner_GetSDVAVFramInfo
  Description: »ñÈ¡SDV ÒôÊÓÆµÍ·ÐÅÏ¢
  Input: _pvMP4Mgr£¬MP4 ¹ÜÀí½á¹¹
  Input: pvSDVAVFramInfoBuf£¬ÒôÊÓÆµÍ·½á¹¹Ìå
  Input: _iSDVAVFramInfoBufLen£¬ÒôÊÓÆµÍ·½á¹¹Ìå³¤¶È
  Output: 
  Return: º¯ÊýÖ´ÐÐ³É¹¦·µ»Ø td_success, Ê§°Ü·µ»Ø td_failure
  Author: »ÆÖ¾¸Õ(Huangzhigang)
  Others: 
*************************************************/
static int Inner_GetSDVAVFramInfo(void* _pvMP4Mgr, void* _pvSDVAVFramInfoBuf, int _iSDVAVFramInfoBufLen)
{
	TMP4Mgr* pstMP4Mgr = (TMP4Mgr*)_pvMP4Mgr;
#ifdef TD_VFS
	TVfile* pfSDVFile = NULL;
#else
	FILE*	pfSDVFile = NULL;
#endif
	int iThisFunRes = td_success;
	S_FrameHeader* pstSDVAVFrameHead = NULL;
	int iSDVAVFramInfoBufLen = 0;
	int iTmpRes = 0;
	
	if((NULL == pstMP4Mgr)
		|| (NULL == pstMP4Mgr->m_pfSDVFileHandle) 
		|| (NULL == _pvSDVAVFramInfoBuf) 
		|| (sizeof(S_FrameHeader) > _iSDVAVFramInfoBufLen))
	{
		td_printf(0, "%d: Input parameters is error![%p][%p][%p][%d]\n", __LINE__,_pvMP4Mgr, pstMP4Mgr->m_pfSDVFileHandle,
			_pvSDVAVFramInfoBuf, _iSDVAVFramInfoBufLen);
		iThisFunRes = td_failure;
		goto LABLE_TD_MP4_EXIT;
	}

	pfSDVFile = pstMP4Mgr->m_pfSDVFileHandle;
	pstSDVAVFrameHead = (S_FrameHeader*)_pvSDVAVFramInfoBuf;
	iSDVAVFramInfoBufLen = _iSDVAVFramInfoBufLen;

	//¶ÁÈ¡SDV ÒôÊÓÆµÍ·Ê§°Ü
	iTmpRes = td_mp4_fread(pstSDVAVFrameHead, 1, iSDVAVFramInfoBufLen, (void*)pfSDVFile);
	
	if(iSDVAVFramInfoBufLen != iTmpRes)
	{
		td_printf(0, "%d: Read pvSDVAVFramInfoBuf failed! Error info[%s],iSDVAVFramInfoBufLen=[%d]\n", __LINE__, strerror(errno), iSDVAVFramInfoBufLen);
		iThisFunRes = td_failure;
		goto LABLE_TD_MP4_EXIT;
	}	

LABLE_TD_MP4_EXIT:
	return iThisFunRes;
}

/************************************************* 
  Function: MP4_SetFileCreateTime
  Author: »ÆÖ¾¸Õ(Huangzhigang)
  Description: ÉèÖÃMP4ÎÄ¼þÐÞ¸ÄÊ±¼ä
  Input: _ui64Time,ÃëÊ±¼ä
  Output:
  Return: º¯ÊýÖ´ÐÐ³É¹¦·µ»Ø td_success, Ê§°Ü·µ»Ø td_failure
  Others: 
*************************************************/
int Inner_SetFileCreateTime(uint64_t _ui64Time)
{
#ifdef FOR_MP4SDK
#else
	MP4SetAllCreateTime(_ui64Time);
#endif
	return td_success;
}


#if 0
static void Inner_ShowSDVAVFramInfo(void* _pvSDVAVFramInfoBuf, int _iSDVAVFramInfoBufLen)
{
	S_FrameHeader* pstAVFrameHead = NULL;
	
	if((NULL == _pvSDVAVFramInfoBuf) || (0 > _iSDVAVFramInfoBufLen))
	{
		td_printf(0xff0000, "%d: Input parameters is error![%p][%d]\n", __LINE__,_pvSDVAVFramInfoBuf, _iSDVAVFramInfoBufLen);
		goto LABLE_TD_MP4_EXIT;
	}
	
	pstAVFrameHead = (S_FrameHeader*)_pvSDVAVFramInfoBuf;

	td_printf(0,"\n SDVFrameIndexHead.m_stHead begin\n");
	td_printf(0," reserved     =%u\n",pstAVFrameHead->reserved );
	td_printf(0," u16AFrames   =%u\n",pstAVFrameHead->u16AFrames );
	td_printf(0," u16AFrameSize=%u\n",pstAVFrameHead->u16AFrameSize );
	td_printf(0," u32FrameID   =%u\n",pstAVFrameHead->u32FrameID );
	td_printf(0," u32FrameNO   =%u\n",pstAVFrameHead->u32FrameNO );
	td_printf(0," u32FrameSize =%u\n",pstAVFrameHead->u32FrameSize );
	td_printf(0," u32TimeStamp =%u\n",pstAVFrameHead->u32TimeStamp );
	td_printf(0," u32VStreamLen=%u\n",pstAVFrameHead->u32VStreamLen );
	td_printf(0," u8ACoder     =%u\n",pstAVFrameHead->u8ACoder );
	td_printf(0," u8FrameType  =%u\n",pstAVFrameHead->u8FrameType );
	td_printf(0," SDVFrameIndexHead.m_stHead end.\n");
	
LABLE_TD_MP4_EXIT:	
	return;
}
static void Inner_ReleaseSDVAVSampleMem(void** _pvSDVVedioBuf, void** _pvSDVAudioBuf)
{
	if(NULL != *_pvSDVVedioBuf)
	{
		free(*_pvSDVVedioBuf);
		*_pvSDVVedioBuf = NULL;
	}
	if(NULL != *_pvSDVAudioBuf)
	{
		free(*_pvSDVAudioBuf);
		*_pvSDVAudioBuf = NULL;
	}
}

/************************************************* 
  Function: Inner_ShowSDVVirtualFrameInfo
  Description: ÏÔÊ¾SDVÐéÄâÖ¡ÐÅÏ¢£¬²Î¿¼Inner_AnalyzeMP4VirtualFrameInfo£¬´Ë´¦²»ÔÙÀÛ×¸ÐðÊö
  Input: 
  Output: 
  Return: void
  Author: »ÆÖ¾¸Õ(Huangzhigang)
  Others: 
*************************************************/
static void Inner_ShowSDVVirtualFrameInfo(const TIFrameIndexHead* _pstSDVFrameIndexHead, const TIFrameIndexInfo* _pstSDVFrameIndexBody
						, const TAppendCharInfo* _pstSDVAppendCharInfo, const TIFrameIndexTail* _pstSDVFrameIndexTail)
{
	TIFrameIndexHead* pstSDVFrameIndexHead = NULL;
	TIFrameIndexInfo* pstSDVFrameIndexBody = NULL;
	TAppendCharInfo*  pstSDVAppendCharInfo = NULL;
	TIFrameIndexTail* pstSDVFrameIndexTail = NULL;
	
	if(    (NULL == _pstSDVFrameIndexHead)
		&& (NULL == _pstSDVFrameIndexBody)
		&& (NULL == _pstSDVAppendCharInfo)
		&& (NULL == _pstSDVFrameIndexTail)
		)
	{
		td_printf(0xff0000, "%d: Input parameters is error![%p][%p][%p][%p]\n", __LINE__
			,_pstSDVFrameIndexHead, _pstSDVFrameIndexBody, _pstSDVAppendCharInfo, _pstSDVFrameIndexTail);
		goto LABLE_TD_MP4_EXIT;
	}
	
	pstSDVFrameIndexHead = (TIFrameIndexHead*)_pstSDVFrameIndexHead;
	pstSDVFrameIndexBody = (TIFrameIndexInfo*)_pstSDVFrameIndexBody;
	pstSDVAppendCharInfo = (TAppendCharInfo*)_pstSDVAppendCharInfo;
	pstSDVFrameIndexTail = (TIFrameIndexTail*)_pstSDVFrameIndexTail;

	if(NULL != pstSDVFrameIndexHead)
	{
		td_printf(0,"%d:\nSDVFrameIndexHead Info begin:\n", __LINE__);
		td_printf(0," m_iInfoSize=%d\n",pstSDVFrameIndexHead->m_iInfoSize);
		td_printf(0," m_iMemNum  =%d\n",pstSDVFrameIndexHead->m_iMemNum);
		td_printf(0," m_iNum     =%d\n",pstSDVFrameIndexHead->m_iNum);
		td_printf(0,"\n SDVFrameIndexHead.m_stHead begin\n");
		td_printf(0,"   m_stHead.reserved     =%u\n",pstSDVFrameIndexHead->m_stHead.reserved );
		td_printf(0,"   m_stHead.u16AFrames   =%u\n",pstSDVFrameIndexHead->m_stHead.u16AFrames );
		td_printf(0,"   m_stHead.u16AFrameSize=%u\n",pstSDVFrameIndexHead->m_stHead.u16AFrameSize );
		td_printf(0,"   m_stHead.u32FrameID   =%u\n",pstSDVFrameIndexHead->m_stHead.u32FrameID );
		td_printf(0,"   m_stHead.u32FrameNO   =%u\n",pstSDVFrameIndexHead->m_stHead.u32FrameNO );
		td_printf(0,"   m_stHead.u32FrameSize =%u\n",pstSDVFrameIndexHead->m_stHead.u32FrameSize );
		td_printf(0,"   m_stHead.u32TimeStamp =%u\n",pstSDVFrameIndexHead->m_stHead.u32TimeStamp );
		td_printf(0,"   m_stHead.u32VStreamLen=%u\n",pstSDVFrameIndexHead->m_stHead.u32VStreamLen );
		td_printf(0,"   m_stHead.u8ACoder     =%u\n",pstSDVFrameIndexHead->m_stHead.u8ACoder );
		td_printf(0,"   m_stHead.u8FrameType  =%u\n",pstSDVFrameIndexHead->m_stHead.u8FrameType );
		td_printf(0," SDVFrameIndexHead.m_stHead end.\n");
		td_printf(0,"%d:\nSDVFrameIndexHead Info end.\n", __LINE__);
	}
	if(NULL != pstSDVFrameIndexBody)
	{
		int i = 0;
		td_printf(0,"%d:\nSDVFrameIndexBody Info begin:\n", __LINE__);
		for(i=0; i<pstSDVFrameIndexHead->m_iNum; i++)
		{
			pstSDVFrameIndexBody[i].m_cTagName[63] = 0;
			td_printf(0," ------------------------------>The %dst I frame info is:\n",i+1);
			td_printf(0," m_cTagName  =%s\n",pstSDVFrameIndexBody[i].m_cTagName);
			td_printf(0," m_iPos      =%u\n", pstSDVFrameIndexBody[i].m_iPos );
			td_printf(0," m_iSign     =%u\n",pstSDVFrameIndexBody[i].m_iSign );
			td_printf(0," m_iTimeNow  =%u\n",(unsigned)pstSDVFrameIndexBody[i].m_iTimeNow );
			td_printf(0," m_iVdeoSize =%u\n",pstSDVFrameIndexBody[i].m_iVdeoSize );
		}
		td_printf(0,"%d:\nSDVFrameIndexBody Info end.\n", __LINE__);
	}
	if(NULL != pstSDVAppendCharInfo)
	{
		pstSDVAppendCharInfo->m_pcCharInfo[MAX_RESERVED_INFO_LEN-1] = 0;
		td_printf(0,"%d:\nSDVAppendCharInfo Info begin:\n", __LINE__);
		td_printf(0," m_iFlag     =%x\n",pstSDVAppendCharInfo->m_iFlag);
		td_printf(0," m_iSize     =%u\n",pstSDVAppendCharInfo->m_iSize);
		td_printf(0," m_pcCharInfo=%s\n",pstSDVAppendCharInfo->m_pcCharInfo);
		td_printf(0,"%d:\nSDVAppendCharInfo Info end.\n", __LINE__);
	}
	if(NULL != pstSDVFrameIndexTail)
	{
		//pstSDVFrameIndexTail->m_cCDSerialNo[MAX_CDROMSERIALNO_LEN-1] = 0;
		td_printf(0,"%d:\nSDVFrameIndexTail Info begin:\n", __LINE__);
		//td_printf(0," m_cCDSerialNo=%s\n",pstSDVFrameIndexTail->m_cCDSerialNo);
		td_printf(0," m_iFlag      =%x\n",pstSDVFrameIndexTail->m_iFlag );
		td_printf(0," m_iSize      =%d\n",pstSDVFrameIndexTail->m_iSize );
		td_printf(0,"%d:\nSDVFrameIndexTail Info end.\n", __LINE__);
	}
	
LABLE_TD_MP4_EXIT:
	return;
	
}

int MP4_IsMP4File(char* _pcFileName)
{		
	int iRet = td_true;
	
	if(NULL == _pcFileName)
	{
		td_printf(0xff0000,"%d, _pucPath = NULL\n",__LINE__);
		iRet = td_false;
		goto leave;
	}
	
	char cFullPath[FULL_PATH_LEN] = {0};

	if(Inner_CompleteFileName(_pcFileName, cFullPath, FULL_PATH_LEN) < 0)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_false;
		goto leave;
	}

	char* pMP4Info = MP4FileInfo(cFullPath, MP4_INVALID_TRACK_ID);

	if(NULL == pMP4Info)
	{
		td_printf(0xff0000, "the file[%s] is not MP4 file,please cheak!", cFullPath);
		iRet = td_false;
		goto leave;
	}

    td_printf(0x12345678, "the MP4_file[%s] info: \n %s", cFullPath, pMP4Info);
	free(pMP4Info);
	pMP4Info = NULL;
leave:
	return iRet;
}

static int Inner_GetInfoFromLastFree(MP4_VOID* _pFileHandle, char* _pcInfoType, int* _pNeedInfo)
{
	int iRet = td_failure;
	off_t OrgPos = 0;	
	int iReadNum = 0;
	int iReadPos = 0;
	unsigned int uiVersionBefore = 0;
	unsigned int uiVersionAfter  = 0;
	unsigned int uiNeedInfo = 0;
	
	if((NULL == _pFileHandle)||(NULL == _pNeedInfo))
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		goto LEAVE;
	}

	OrgPos = td_mp4_ftell(_pFileHandle);

	if(td_mp4_fseek(_pFileHandle, -128, SEEK_END) < 0)
	{
		td_printf(0xff0000, "%s %d: Seek failed! Error info[%s].\n", __FUNCTION__, __LINE__, strerror(errno));
		goto LEAVE;
	}

	iReadNum = td_mp4_fread(&uiVersionBefore, 1, 4, _pFileHandle);
	ConvertSort((unsigned char*)&uiVersionAfter, uiVersionBefore);
	
	if((iReadNum != 4)||(1 != uiVersionAfter))
	{
		td_printf(0xff0000, "%s %s %d error! Version is %d", __FILE__, __FUNCTION__, __LINE__, uiVersionAfter);
		goto LEAVE;
	}

	if(0 == memcmp(_pcInfoType, ENCRYPTION_TYPE, MP4_BOX_TYPE_LENGTH))
	{
		iReadPos += 8;			
	}
	else if(0 == memcmp(_pcInfoType, AUDIO_ENCODE_TYPE, MP4_BOX_TYPE_LENGTH))
	{
		iReadPos += (8 + 4);
	}
	else
	{
		td_printf(0xff0000, "the type[%s] is not support!", _pcInfoType);
		goto LEAVE;
	}

	if(td_mp4_fseek(_pFileHandle, iReadPos, SEEK_CUR) < 0)
	{
		td_printf(0xff0000, "%s %d: Seek failed! Error info[%s].\n", __FUNCTION__, __LINE__, strerror(errno));
		goto LEAVE;
	}

	iReadNum = td_mp4_fread(&uiNeedInfo, 1, 4, _pFileHandle);
	
	if(iReadNum != 4)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		goto LEAVE;
	}
		
	printfdataX("uiNeedInfo:", (char*)&uiNeedInfo,4);
	td_printf(0, "------------------->uiNeedInfo is %d", uiNeedInfo);

	if(uiNeedInfo != 0)
	{
		*_pNeedInfo = uiNeedInfo;
		td_printf(0, "*_pNeedInfo is %d", *_pNeedInfo);

		iRet = td_success;
	}
LEAVE:
	if(NULL != _pFileHandle)
	{
		td_mp4_fseek(_pFileHandle, OrgPos, SEEK_SET);
	}
	return iRet;
}

static int Inner_GetInfoFromAll(MP4_VOID* _pFIleHandle, MP4_VOID* _pcBoxType, int* _piNeedInfo)
{
	int iRet = td_failure;
	int iFount = 0;
	int iInfoTmp = 0;
	unsigned int uiTmp = 0;
	unsigned int uiReadLen = 0;
	unsigned int uiBoxSize = 0;
	unsigned int uiVersion = 0;
	unsigned int uiBaseUnitSize = 0;
	unsigned int uiMemsize = 0;
	unsigned int uiPos = 0;
	unsigned long long ullBoxSizeBig = 0;
	char BoxType[MP4_FILE_TYPE_LENGTH] = {0};
	char CustomMdatFlag[MP4_CUSTOM_MDATFLAG_LENGTH] = {0};

	if((NULL == _pFIleHandle)||(NULL == _pcBoxType)||(NULL == _piNeedInfo))
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		goto LEAVE;
	}
	
	while(0 == td_mp4_feof(_pFIleHandle))
	{
		uiBoxSize = 0;
		uiPos = 0;

		uiReadLen = td_mp4_fread(&uiTmp, 1, MP4_BOX_SIEZ_LENGTH, _pFIleHandle);
		uiPos += uiReadLen;

		if(uiReadLen != MP4_BOX_SIEZ_LENGTH)
		{
			td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
			goto IFFEOF;
		}
		
		ConvertSort((unsigned char*)&uiBoxSize, uiTmp);
		
		td_printf(0, "-------->m_uiCurMP4BoxSize is %d", uiBoxSize);

		uiReadLen = td_mp4_fread(BoxType, 1, MP4_BOX_TYPE_LENGTH, _pFIleHandle);
		uiPos += uiReadLen;
		
		if(uiReadLen != MP4_BOX_TYPE_LENGTH)
		{
			td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
			goto IFFEOF;
		}
							
		td_printf(0, "-------->BoxType is [%s]", BoxType);

		if(1 == uiBoxSize)
		{
			uiReadLen = td_mp4_fread(&ullBoxSizeBig, 1, MP4_BOX_SIZE_LENGTH_BIG, _pFIleHandle);
			uiPos += uiReadLen;
			
			if(uiReadLen != MP4_BOX_SIZE_LENGTH_BIG)
			{
				td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
				goto IFFEOF;
			}
			
			ConvertSortLong((unsigned char*)&ullBoxSizeBig, ullBoxSizeBig);
			uiBoxSize = (unsigned int)ullBoxSizeBig;
			
			td_printf(0, "-------->m_uiCurMP4BoxSizeBig is %llu", ullBoxSizeBig);
		}
			
		if(0 == memcmp((char*)BoxType, "mdat", MP4_BOX_TYPE_LENGTH))
		{
			uiReadLen = td_mp4_fread(CustomMdatFlag, 1, MP4_CUSTOM_MDATFLAG_LENGTH, _pFIleHandle);
			uiPos += uiReadLen;
			
			if(uiReadLen != MP4_CUSTOM_MDATFLAG_LENGTH)
			{
				td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
				goto IFFEOF;
			}

			if(0 == memcmp((char*)CustomMdatFlag, SELF_DEFINE_FLAG, MP4_CUSTOM_MDATFLAG_LENGTH))
			{				
				uiReadLen = td_mp4_fread(&uiTmp, 1, MP4_MDAT_VERSION_LENGTH, _pFIleHandle);
				uiPos += uiReadLen;
				
				if(uiReadLen != MP4_MDAT_VERSION_LENGTH)
				{
					td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
					goto IFFEOF;
				}

				ConvertSort((unsigned char*)&uiVersion, uiTmp);

				if(1 == uiVersion)
				{
					uiReadLen = td_mp4_fread(&uiTmp, 1, MP4_BOX_SIEZ_LENGTH, _pFIleHandle);
					uiPos += uiReadLen;
					
					if(uiReadLen != MP4_BOX_SIEZ_LENGTH)
					{
						td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
						goto IFFEOF;
					}

					ConvertSort((unsigned char*)&uiBaseUnitSize, uiTmp);

					uiReadLen = td_mp4_fread(BoxType, 1, MP4_BOX_TYPE_LENGTH, _pFIleHandle);
					uiPos += uiReadLen;
					
					if(uiReadLen != MP4_BOX_TYPE_LENGTH)
					{
						td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
						goto IFFEOF;
					}

					if(0 == memcmp((char*)BoxType, _pcBoxType, MP4_BOX_TYPE_LENGTH))
					{
						td_printf(0x12345678, "find the need box!");

						uiReadLen = td_mp4_fread(&uiTmp, 1, sizeof(unsigned int), _pFIleHandle);
						uiPos += uiReadLen;
						
						if(uiReadLen != sizeof(unsigned int))
						{
							td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
							goto IFFEOF;
						}
						
						ConvertSort((unsigned char*)&uiMemsize, uiTmp);

						uiReadLen = td_mp4_fread(&iInfoTmp, 1, sizeof(int), _pFIleHandle);
						uiPos += uiReadLen;
						
						if(uiReadLen != sizeof(int))
						{
							td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
							goto IFFEOF;
						}	
						else
						{
							iFount = 1;
							*_piNeedInfo = iInfoTmp;
							goto LEAVE;
						}
					}
					else
					{
						goto NEXTBOX;
					}
				}
				else
				{
					goto NEXTBOX;
				}

			}
			else
			{
				goto NEXTBOX;
			}
		}
		else
		{
			goto NEXTBOX;
		}
IFFEOF:
		if(td_mp4_feof(_pFIleHandle))
		{
			goto LEAVE;
		}
NEXTBOX:
		td_printf(0xff0000, "%s %s %d the current box is not need mdat!\n", __FILE__, __FUNCTION__, __LINE__);
		td_mp4_fseek(_pFIleHandle,uiBoxSize-uiPos,SEEK_CUR);
		continue;
	}
	
LEAVE:
	if(1 == iFount)
	{
		iRet = td_success;
	}
	return iRet;
}
#endif

/************************************************* 
  Function: Inner_WriteSDVAVSampleToMP4
  Description: »ñÈ¡ÒôÊÓÆµÊý¾Ý
  Input: _pvMP4Mgr£¬MP4 ¹ÜÀí½á¹¹
  Input: _iSDVVedioBufLen£¬ÊÓÆµÊý¾Ý³¤¶È
  Input: _iSDVAudioBufLen£¬ÒôÆµÊý¾Ý³¤¶È
  Output: _pvSDVVedioBuf£¬ÊÓÆµÊý¾Ý
  Output: _pvSDVAudioBuf£¬ÒôÆµÊý¾Ý
  Return: º¯ÊýÖ´ÐÐ³É¹¦·µ»Ø td_success, Ê§°Ü·µ»Ø td_failure
  Author: »ÆÖ¾¸Õ(Huangzhigang)
  Others: 
*************************************************/
static int Inner_GetSDVAVData(void* _pvMP4Mgr, void** _pvSDVVedioBuf, int* _piSDVVedioBufLen, void** _pvSDVAudioBuf, int* _piSDVAudioBufLen)
{
	TMP4Mgr* pvMP4Mgr = NULL;

	int iThisFunRes = td_success;
	TMP4Mgr* pstMP4Mgr = NULL;
	MP4_VOID* pSDVAVDataBuf = NULL;
#ifdef TD_VFS
	TVfile* pfSDVFile = NULL;
#else
	FILE*	pfSDVFile = NULL;
#endif
	int iSDVAVDataBufLen = 0;
	int iSDVVedioBufLen = 0;
	int iSDVAudioBufLen = 0;
	int iTmpRes = 0;

	pvMP4Mgr = (TMP4Mgr*)_pvMP4Mgr;
	
	if((NULL == pvMP4Mgr) || (NULL == pvMP4Mgr->m_pfSDVFileHandle) 
		||(NULL == _pvSDVVedioBuf) ||(NULL == _pvSDVAudioBuf)|| (NULL == _piSDVVedioBufLen)||(NULL == _piSDVAudioBufLen))
	{
		td_printf(0xff0000, "%d: Input parameters is error!\n", __LINE__);
		iThisFunRes = td_failure;
		goto LABLE_TD_MP4_EXIT;
	}
	
	if(*_piSDVVedioBufLen <= 0)
	{
		td_printf(0xff0000, "%d: Input parameters is error!\n", __LINE__);
		iThisFunRes = td_failure;
		goto LABLE_TD_MP4_EXIT;
	}

	pstMP4Mgr = pvMP4Mgr;
	pfSDVFile = pvMP4Mgr->m_pfSDVFileHandle;
	iSDVVedioBufLen = *_piSDVVedioBufLen;
	iSDVAudioBufLen = *_piSDVAudioBufLen;
	iSDVAVDataBufLen = *_piSDVVedioBufLen + *_piSDVAudioBufLen;

	pSDVAVDataBuf = (MP4_VOID*)(pvMP4Mgr->m_ucAVDataBuf);
		
	if(NULL == pSDVAVDataBuf)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iThisFunRes = td_failure;
		goto LABLE_TD_MP4_EXIT;
	}

	if(iSDVAVDataBufLen <= pvMP4Mgr->m_iAVDataBufSize)
	{
		//¶ÁÈ¡SDV ÊÓÒôÆµÖ¡
		iTmpRes = td_mp4_fread(pSDVAVDataBuf, 1, iSDVAVDataBufLen, (void*)pfSDVFile);

		if(iSDVAVDataBufLen != iTmpRes)
		{
			if(iSDVVedioBufLen == iTmpRes)
			{
				td_printf(0xff0000, "iSDVVedioBufLen == iTmpRes");
				iSDVAudioBufLen = 0;
				*_piSDVAudioBufLen = 0;
			}
			else
			{
				td_printf(0, "%d: Read pvSDVVedioBuf failed! Error info[%s].\n", __LINE__, strerror(errno));
				iThisFunRes = td_failure;
				goto LABLE_TD_MP4_EXIT;
			}
		}
	}
	else
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iThisFunRes = td_failure;
		goto LABLE_TD_MP4_EXIT;
	}
	

	//ÅÐ¶ÏÊÇ·ñÒÑ¼ÓÃÜ
	if(((pstMP4Mgr->m_stSDVFrameHead.reserved & 0x03) == 0x02)&&(iSDVAVDataBufLen >= 64))
	{
		#ifdef ZHENGFA
		if((1 == pstMP4Mgr->m_bCryptFlag)||(iSDVVedioBufLen < 64))
		{
			aes_crypt((unsigned char*)pSDVAVDataBuf,64,pstMP4Mgr->m_stSDVFrameHead.u32FrameNO,(unsigned char*)CRYPT_KEY_STR,strlen(CRYPT_KEY_STR));
			td_printf(0, "%s %s %d jiemi avdata", __FILE__, __FUNCTION__, __LINE__);
			if( ((int *)pSDVAVDataBuf)[0] != 0X1000000 )
			{
				td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
				iThisFunRes = MP4_CONVERT_ERROR_CRYPT_FAILED;
				goto LABLE_TD_MP4_EXIT;
			}
		}
		#else
		td_printf(0xff0000, "%s %s %d error! the file has been encrypt! please check!", __FILE__, __FUNCTION__, __LINE__);
		iThisFunRes = MP4_CONVERT_ERROR_CRYPT_FAILED;
		goto LABLE_TD_MP4_EXIT;
		#endif
	}	

	if(NULL != _pvSDVVedioBuf)
	{
		*_pvSDVVedioBuf = pSDVAVDataBuf;
	}

	if(NULL != _pvSDVAudioBuf)
	{
		*_pvSDVAudioBuf = pSDVAVDataBuf + iSDVVedioBufLen;
	}
LABLE_TD_MP4_EXIT:
	return iThisFunRes;	
}


/************************************************* 
  Function: Inner_CalcSDVAVTimeInterval
  Description: ¼ÆËãÒôÊÓÆµÊ±¼ä¼ä¸ô
  Input: _pvMP4Mgr£¬MP4 ¹ÜÀí½á¹¹
  Output:
  Return: º¯ÊýÖ´ÐÐ³É¹¦·µ»Ø td_success, Ê§°Ü·µ»Ø td_failure
  Author: »ÆÖ¾¸Õ(Huangzhigang)
  Others: 
*************************************************/
static int Inner_CalcSDVAVTimeInterval(void* _pvMP4Mgr)
{
	TMP4Mgr* pstMP4Mgr = (TMP4Mgr*)_pvMP4Mgr;
	unsigned int uiVideoInterval = 0;
	unsigned int uiAudioInterval = 0;
	int iThisFunRes = td_success;
	S_FrameHeader* pstSDVAVFrameHead = NULL;
	S_FrameHeader stNextSDVAVFrameHead = {0};
	int iTmpRes = 0;
	off_t lFileNonius = 0;
	int iSynFlag = 0;
#ifdef TD_VFS
	TVfile* pfSDVFile = NULL;
#else
	FILE*	pfSDVFile = NULL;
#endif
	//static int iVideo = 0;
	//static int iAudio = 0;
	
	if((NULL == pstMP4Mgr) ||(NULL == pstMP4Mgr->m_pfSDVFileHandle))
	{
		td_printf(0xff0000, "%d: Input parameters is error![%p]\n", __LINE__,pstMP4Mgr);
		iThisFunRes = td_failure;
		goto LABLE_TD_MP4_EXIT;
	}

	pstSDVAVFrameHead = &(pstMP4Mgr->m_stSDVFrameHead);
	pfSDVFile = pstMP4Mgr->m_pfSDVFileHandle;

	//¼ÆËãÊÓÆµÊ±¼ä¼ä¸ô
	//¼ÇÂ¼µ±Ç°ÓÎ±êÎ»ÖÃ
	lFileNonius = td_mp4_ftell((void*)pfSDVFile);
	if(-1 == lFileNonius)
	{
		td_printf(0xff0000, "%d: ftell failed! Error info[%s].\n", __LINE__, strerror(errno));
		iThisFunRes = td_failure;
		goto LABLE_TD_MP4_EXIT;
		
	}
	//td_printf(0, "%d: Current nonius pos=%ld.\n", __LINE__, lFileNonius);

	iTmpRes = td_mp4_fseek((void*)pfSDVFile, pstSDVAVFrameHead->u32VStreamLen+pstSDVAVFrameHead->u16AFrameSize*pstSDVAVFrameHead->u16AFrames, SEEK_CUR);
	if(-1 == iTmpRes)
	{
		td_printf(0xff0000, "%d: Seek failed! iTmpRes=%d. Error info[%s].\n", __LINE__, iTmpRes, strerror(errno));
		iThisFunRes = td_failure;
		goto LABLE_TD_MP4_EXIT;
		
	}
	//td_printf(0, "%d: Current nonius pos=%ld.next pos=%d\n", __LINE__, ftell(pfSDVFile), pstSDVAVFrameHead->u32FrameSize);
	
	if(0 == pstMP4Mgr->m_bVedioFirstAppearFlag)
	{
		pstMP4Mgr->m_uiLastAudioTime = pstSDVAVFrameHead->u32TimeStamp;
		pstMP4Mgr->m_bVedioFirstAppearFlag = 1;
	}

	//¶ÁÈ¡ÏÂÒ»Ö¡
	iTmpRes = td_mp4_fread(&stNextSDVAVFrameHead, 1, sizeof(S_FrameHeader), (void*)pfSDVFile);
	if(sizeof(S_FrameHeader) != iTmpRes)
	{
		if(0!=td_mp4_feof((void*)pfSDVFile))
		{
			td_printf(0, "%d: Read stNextSDVAVFrameHead feof.td_success\n", __LINE__);
			pstMP4Mgr->m_bConvertExitFlag = TD_MP4_CONVERT_STOP;
			iThisFunRes = td_success;
			goto LABLE_TD_MP4_EXIT;
		}

		td_printf(0, "%d: Read failed! iTmpRes=%d. Error info[%s].\n", __LINE__, iTmpRes, strerror(errno));
		iThisFunRes = td_failure;
		goto LABLE_TD_MP4_EXIT;
		
	}
	//td_printf(0, "%d: Current nonius pos=%ld.\n", __LINE__, ftell(pfSDVFile));

	pstMP4Mgr->m_lCurFilePos = td_mp4_ftell((void*)pfSDVFile);

	if(-1 == pstMP4Mgr->m_lCurFilePos)
	{
		td_printf(0xff0000, "%d: ftell failed! Error info[%s].\n", __LINE__, strerror(errno));
		iThisFunRes = td_failure;
		goto LABLE_TD_MP4_EXIT;
		
	}

	if(pstMP4Mgr->m_lCurFilePos >= pstMP4Mgr->m_lStopConvertPos)
	{
		td_printf(0xff0000, "pstMP4Mgr->m_lCurFilePos is %ld", pstMP4Mgr->m_lCurFilePos);
		td_printf(0xff0000, "pstMP4Mgr->m_lStopConvertPos is %ld", pstMP4Mgr->m_lStopConvertPos);
		td_printf(0, "%d: NextSDVAVFrameHead is virtual frame.td_success\n", __LINE__);
		pstMP4Mgr->m_bConvertExitFlag = TD_MP4_CONVERT_STOP;
		iThisFunRes = td_success;
		goto LABLE_TD_MP4_EXIT;
	}
	
	uiVideoInterval = stNextSDVAVFrameHead.u32TimeStamp - pstSDVAVFrameHead->u32TimeStamp;

	if((uiVideoInterval > MAX_FRAME_INTERVAL)||(uiVideoInterval < 0))
	{
		uiVideoInterval = DEFAULT_FRAME_INTERVAL;
		iSynFlag = 1;
	}

	pstMP4Mgr->m_VedioDuration = uiVideoInterval*90;	
	//td_printf(0, "%d: pstMP4Mgr->m_VedioDuration=%llu.N=%u,C=%u\n", __LINE__, pstMP4Mgr->m_VedioDuration, stNextSDVAVFrameHead.u32TimeStamp, pstSDVAVFrameHead->u32TimeStamp);
	//td_printf(0, "+++++++++++++++uiVideoInterval[%d] is %d", ++iVideo, uiVideoInterval);
	
	//¼ÆËãÒôÆµÊ±¼ä¼ä¸ô
	if(pstSDVAVFrameHead->u16AFrames > 0)
	{	
		uiAudioInterval = pstSDVAVFrameHead->u32TimeStamp - pstMP4Mgr->m_uiLastAudioTime;

		if((uiAudioInterval > MAX_FRAME_INTERVAL)&&(1 == iSynFlag))
		{
			td_printf(0, "Syn==================uiAudioInterval is %u", uiAudioInterval);
			uiAudioInterval = DEFAULT_FRAME_INTERVAL;
		}

		//td_printf(0, "----------------uiAudioInterval[%d] is %d", ++iAudio,uiAudioInterval);
			
		pstMP4Mgr->m_AudioDuration = uiAudioInterval * pstMP4Mgr->m_stSDVFileHead.AudioSample / 1000;
		pstMP4Mgr->m_uiLastAudioTime = pstSDVAVFrameHead->u32TimeStamp;
	}
	//td_printf(0, "hahahhahahah123123==================>%d: u16AFrames=[%u]m_AudioDuration=[%llu].\n", __LINE__, pstSDVAVFrameHead->u16AFrames, pstMP4Mgr->m_AudioDuration);
LABLE_TD_MP4_EXIT:
	if((NULL != pfSDVFile)&&(0 != lFileNonius))
	{
		//»Ö¸´µ±Ç°ÓÎ±êÎ»ÖÃ
		iTmpRes = td_mp4_fseek((void*)pfSDVFile, lFileNonius, SEEK_SET);
		if(-1 == iTmpRes)
		{
			td_printf(0xff0000, "%d: Seek failed! iTmpRes=%d. Error info[%s].\n", __LINE__, iTmpRes, strerror(errno));
			iThisFunRes = td_failure;
		}
		//td_printf(0, "%d: Current nonius pos=%ld.\n", __LINE__, td_mp4_ftell((void*)pfSDVFile));
	}
	return iThisFunRes;	
}


/************************************************* 
  Function: Inner_GetSDVAVSample
  Description: »ñÈ¡SDV ÒôÊÓÆµ
  Input: _pvMP4Mgr£¬MP4 ¹ÜÀí½á¹¹
  Output: _pvSDVVedioBuf£¬ÊÓÆµÊý¾Ý
  Output: _piSDVVedioBufLen£¬ÊÓÆµ³¤¶È
  Output: _pvSDVAudioBuf£¬ÒôÆµÊý¾Ý
  Output: _piSDVAudioBufLen£¬ ÒôÆµÊý¾Ý³¤¶È
  Return: º¯ÊýÖ´ÐÐ³É¹¦·µ»Ø td_success, Ê§°Ü·µ»Ø td_failure
  Author: »ÆÖ¾¸Õ(Huangzhigang)
  Others: 
*************************************************/
static int Inner_GetSDVAVSample(void* _pvMP4Mgr, void** _pvSDVVedioBuf, int* _piSDVVedioBufLen, void** _pvSDVAudioBuf, int* _piSDVAudioBufLen)
{
	TMP4Mgr* pstMP4Mgr = (TMP4Mgr*)_pvMP4Mgr;
	
	int iThisFunRes = td_success;
	S_FrameHeader* pstSDVAVFrameHead = NULL;
	void* pvSDVVedioBuf = NULL;
	int iSDVVedioBufLen = 0;
	void* pvSDVAudioBuf = NULL;
	int iSDVAudioBufLen = 0;
	
	if((NULL == pstMP4Mgr) || (NULL == _pvSDVVedioBuf) ||(NULL == _piSDVVedioBufLen) 
		|| (NULL == _pvSDVAudioBuf)|| (NULL == _piSDVAudioBufLen))
	{
		td_printf(0xff0000, "%d: Input parameters is error![%p][%p][%p][%p][%p]\n", __LINE__,pstMP4Mgr,
			_pvSDVVedioBuf, _piSDVVedioBufLen, _pvSDVAudioBuf, _piSDVAudioBufLen);
		iThisFunRes = td_failure;
		goto LABLE_TD_MP4_EXIT;
	}
 
	pstSDVAVFrameHead = &(pstMP4Mgr->m_stSDVFrameHead);
	
	//»ñÈ¡SDV ÒôÊÓÆµÍ·
	if(Inner_GetSDVAVFramInfo((void*)pstMP4Mgr, (void*)pstSDVAVFrameHead, sizeof(S_FrameHeader)) < 0)
	{  
		td_printf(0xff0000, "%d: Call Inner_GetSDVAVFramInfo failed!\n", __LINE__);
		iThisFunRes = td_failure;
		goto LABLE_TD_MP4_EXIT;
	}

	td_printf(0, "*****************************************************************");
	td_printf(0, "u32FrameSize is %d", pstSDVAVFrameHead->u32FrameSize);
	td_printf(0, "u8FrameType is %d", pstSDVAVFrameHead->u8FrameType);
	td_printf(0, "u32VStreamLen is %d", pstSDVAVFrameHead->u32VStreamLen);
	td_printf(0, "u16AFrames is %d", pstSDVAVFrameHead->u16AFrames);
	td_printf(0, "*****************************************************************");
	
	//ÏÔÊ¾SDV ÒôÊÓÆµÍ·ÐÅÏ¢
	//Inner_ShowSDVAVFramInfo((void*)(&pstMP4Mgr->m_stSDVFrameHead), sizeof(S_FrameHeader));
	
	iSDVVedioBufLen = pstSDVAVFrameHead->u32VStreamLen;
	iSDVAudioBufLen = pstSDVAVFrameHead->u16AFrameSize * pstSDVAVFrameHead->u16AFrames;
	
	if(iSDVVedioBufLen <= 0)
	{
		td_printf(0xff0000, "%d: iSDVVedioBufLen is error!\n", __LINE__);		
		iThisFunRes = td_failure;
		goto LABLE_TD_MP4_EXIT;
	}

	//¼ÆËãÊÓÆµÊ±¼ä¼ä¸ô
	if(Inner_CalcSDVAVTimeInterval((void*)pstMP4Mgr) < 0)
	{  
		td_printf(0xff0000, "%d: Call Inner_CalcSDVAVTimeInterval failed!\n", __LINE__);
		iThisFunRes = td_failure;
		goto LABLE_TD_MP4_EXIT;
	}
	
	//»ñÈ¡ÒôÊÓÆµÊý¾Ý
	iThisFunRes = Inner_GetSDVAVData(pstMP4Mgr, &pvSDVVedioBuf, &iSDVVedioBufLen, &pvSDVAudioBuf, &iSDVAudioBufLen);

	if(iThisFunRes < 0)
	{  
		td_printf(0xff0000, "%d: Call Inner_GetSDVAVData failed!\n", __LINE__);
		goto LABLE_TD_MP4_EXIT;
	}

	//´«¸øÍâ²¿
	*_pvSDVVedioBuf = pvSDVVedioBuf;
	*_pvSDVAudioBuf = pvSDVAudioBuf;
	*_piSDVVedioBufLen = iSDVVedioBufLen;
	*_piSDVAudioBufLen = iSDVAudioBufLen;
	
	iThisFunRes = td_success;
	
LABLE_TD_MP4_EXIT:
	
	return iThisFunRes;	
	
}

/************************************************* 
  Function: Inner_WriteIndexInfoToMP4VirtualFrame
  Description: Ð´ÈëÐéÄâÖ¡
  Input: _pvMP4Mgr£¬MP4 ¹ÜÀí½á¹¹
  Input: _iFinishWritting£¬Ìí¼ÓI Ö¡Ë÷Òý´«0£¬Ð´ÈëÐéÄâÖ¡´«1
  Output: 
  Return: º¯ÊýÖ´ÐÐ³É¹¦·µ»Ø td_success, Ê§°Ü·µ»Ø td_failure
  Author: »ÆÖ¾¸Õ(Huangzhigang)
  Others: 
*************************************************/
static int Inner_WriteIndexInfoToMP4VirtualFrame(void* _pvMP4Mgr, int _iFinishWritting)
{
	int i = 0;
	int iTmpSdvICount = 0;
	int iFinishWritting = 0;
	TMP4Mgr* pstMP4Mgr = (TMP4Mgr*)_pvMP4Mgr;
	int iMP4VirtualFramLen = 0;
	int iMP4VirtualFramBufLen = 0;
	int iThisFunRes = td_failure;
	TIFrameIndexHead* pstSDVFrameIndexHead = NULL;
	TIFrameIndexInfo* pstSDVFrameIndexBody = NULL;
	TAppendCharInfo*  pstSDVAppendCharInfo = NULL;
	TIFrameIndexTail* pstSDVFrameIndexTail = NULL;
	int iSDVVirtualFramBufLen = 0;
	TMP4ISampleIndexHead* pstMP4FrameIndexHead = NULL;
	TMP4ISampleIndexInfo* pstMP4FrameIndexBody = NULL;
	TAppendCharInfo*	  pstMP4AppendCharInfo = NULL;
	TIFrameIndexTail*	  pstMP4FrameIndexTail = NULL;
	unsigned char* pucMP4VirtualFramebuffer = NULL;
	unsigned char* pucSDVVirtualFramebuffer = NULL;
	
	if((NULL == pstMP4Mgr) ||(NULL == pstMP4Mgr->m_pvMP4VirtualFramebuffer))
	{
		td_printf(0xff0000, "%d: Input parameters is error![%p]\n", __LINE__, _pvMP4Mgr);
		iThisFunRes = td_failure;
		goto LABLE_TD_MP4_EXIT;
	}
	
	iFinishWritting = _iFinishWritting;
	pucMP4VirtualFramebuffer = (unsigned char*)pstMP4Mgr->m_pvMP4VirtualFramebuffer;
	pucSDVVirtualFramebuffer = (unsigned char*)pstMP4Mgr->m_pvSDVVirtualFramebuffer;
	iMP4VirtualFramBufLen = pstMP4Mgr->m_uiMP4VirtualFramebufferSize;
	iSDVVirtualFramBufLen = pstMP4Mgr->m_uiSDVVirtualFramebufferSize;
	
	//²»ÊÇI Ö¡²»½øÐÐ´¦Àí
	if((1 != pstMP4Mgr->m_bIFrameFlag) && (0 == _iFinishWritting))
	{
		iThisFunRes = td_success;
		goto LABLE_TD_MP4_EXIT;
	}

	if(HAVE_SDV_VIRTUAL_FRAME_FLAG == pstMP4Mgr->m_bHaveSDVVirtualFrameFlag)
	{
		//»ñÈ¡SDV Ë÷ÒýÐÅÏ¢
		if(Inner_AnalyzeSDVVirtualFrameInfo((const void*)pucSDVVirtualFramebuffer, iSDVVirtualFramBufLen
							, &pstSDVFrameIndexHead, &pstSDVFrameIndexBody
							, &pstSDVAppendCharInfo, &pstSDVFrameIndexTail) < 0)
		{  
			td_printf(0xff0000, "%d: Call Inner_AnalyzeSDVVirtualFrameInfo failed!\n", __LINE__);
			iThisFunRes = td_failure;
			goto LABLE_TD_MP4_EXIT;
		}
		PrintInfo("get sdv virtual frame info===========");
		iTmpSdvICount = pstSDVFrameIndexHead->m_iNum;
	}

	//»ñÈ¡MP4 Ë÷ÒýÐÅÏ¢
	if(Inner_AnalyzeMP4VirtualFrameInfo((const void*)pucMP4VirtualFramebuffer
							, iMP4VirtualFramBufLen, &pstMP4FrameIndexHead, &pstMP4FrameIndexBody
							, &pstMP4AppendCharInfo, &pstMP4FrameIndexTail, iTmpSdvICount) < 0)
	{  
		td_printf(0xff0000, "%d: Call Inner_AnalyzeMP4VirtualFrameInfo failed!\n", __LINE__);
		iThisFunRes = td_failure;
		goto LABLE_TD_MP4_EXIT;
	}

	if(0 == iFinishWritting)
	{
		if(HAVE_SDV_VIRTUAL_FRAME_FLAG == pstMP4Mgr->m_bHaveSDVVirtualFrameFlag)
		{
			i = pstMP4Mgr->m_iIFrameCount-1;
			
			if((i >= 0)&&(i < iTmpSdvICount))
			{
				//¿½±´µ¥ÔªË÷ÒýÐÅÏ¢
				memcpy(&pstMP4FrameIndexBody[i].m_stSdvIndexInfo, &pstSDVFrameIndexBody[i], sizeof(TIFrameIndexInfo));
				//»ñÈ¡ÒôÊÓÆµ¶ÔÓ¦µÄI Ö¡ID ºÍÊ±¼ä´Á
				pstMP4FrameIndexBody[i].m_uiUselessCount = pstMP4Mgr->m_uiUselessCount;
				pstMP4FrameIndexBody[i].m_stVideoSampleIndexInfo.m_iSampleID = pstMP4Mgr->m_uiVedioSampleID;
				pstMP4FrameIndexBody[i].m_stVideoSampleIndexInfo.m_tSampleTimeStamp = pstMP4Mgr->m_VedioTotalDuration;
				pstMP4FrameIndexBody[i].m_stAudioSampleIndexInfo.m_iSampleID = pstMP4Mgr->m_uiAudioSampleID;
				pstMP4FrameIndexBody[i].m_stAudioSampleIndexInfo.m_tSampleTimeStamp = pstMP4Mgr->m_AudioTotalDuration;
			}
			else
			{
				td_printf(0xff0000, "%d: pstMP4Mgr->m_iIFrameCount is error! i is %d, iTmpSdvICount is %d\n", __LINE__, i, iTmpSdvICount);
			}			
		}
	}
	else
	{
		//------------------------------------------------------------------------------------
		//Ð´ÈëSDV ÐéÄâÖ¡²¿·Ö
		if(HAVE_SDV_VIRTUAL_FRAME_FLAG == pstMP4Mgr->m_bHaveSDVVirtualFrameFlag)
		{
			#if 0
			if((NULL == pucSDVVirtualFramebuffer)||(iSDVVirtualFramBufLen <= 0))
			{
				td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
				iThisFunRes = td_failure;
				goto LABLE_TD_MP4_EXIT;
			}

			pucSDVVirtualFramebuffer[0] = (iSDVVirtualFramBufLen-MP4_BOX_SIEZ_LENGTH)>>24;
			pucSDVVirtualFramebuffer[1] = ((iSDVVirtualFramBufLen-MP4_BOX_SIEZ_LENGTH)&0x00ff0000)>>16;
			pucSDVVirtualFramebuffer[2] = ((iSDVVirtualFramBufLen-MP4_BOX_SIEZ_LENGTH)&0x0000ff00)>>8;
			pucSDVVirtualFramebuffer[3] = (iSDVVirtualFramBufLen-MP4_BOX_SIEZ_LENGTH)&0x000000ff;
						
			if(!MP4WriteSample(pstMP4Mgr->m_MP4Handle, pstMP4Mgr->m_VideoTrackID, pucSDVVirtualFramebuffer, iSDVVirtualFramBufLen, 1, 0, 0))  
			{
				td_printf(0xff0000, "%d: Call MP4WriteSample failed!\n", __LINE__);
				iThisFunRes = td_failure;
				goto LABLE_TD_MP4_EXIT;
			}
			#endif
			td_printf(0, "%d: Finish writting SDV virtual frame!\n", __LINE__);
		}
		//--------------------------------------------------------------------------------------
		//Ð´ÈëMP4 ÐéÄâÖ¡²¿·Ö
		int tFileEndTime = 0;
		if(Inner_RecordFileEndTime((signed char*)(pstMP4Mgr->m_cSDVFilePath), &tFileEndTime) < 0)
		{  
			td_printf(0xff0000, "%d: Call Inner_RecordFileEndTime failed!\n", __LINE__);
			//iThisFunRes = td_failure;
			//goto LABLE_TD_MP4_EXIT;
		}
		
		//»ñÈ¡SDV ÎÄ¼þÍ·ÐÅÏ¢
		memcpy(&pstMP4FrameIndexHead->m_stExRecInfo.m_stSdvFileHead, &pstMP4Mgr->m_stSDVFileHead, sizeof(S_FileHeader));
		pstMP4FrameIndexHead->m_stExRecInfo.m_iFileEndTime = tFileEndTime;
		pstMP4FrameIndexHead->m_stExRecInfo.m_iReserveFlag = 0;

		if(HAVE_SDV_VIRTUAL_FRAME_FLAG == pstMP4Mgr->m_bHaveSDVVirtualFrameFlag)
		{
			if((NULL == pstSDVFrameIndexHead)
				||(NULL == pstSDVFrameIndexBody)
				  ||(NULL == pstSDVAppendCharInfo)
				    ||(NULL == pstSDVFrameIndexTail))
			{
				td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
				iThisFunRes = td_failure;
				goto LABLE_TD_MP4_EXIT;
			}

			//»ñÈ¡SDV ÐéÄâÖ¡Ë÷ÒýÍ·ÐÅÏ¢
			memcpy(&pstMP4FrameIndexHead->m_stSdvIndexHead, pstSDVFrameIndexHead, sizeof(TIFrameIndexHead));
			pstMP4FrameIndexHead->m_stSdvIndexHead.m_iInfoSize = sizeof(TMP4ISampleIndexInfo);
 			#ifdef ZHENGFA
			//¹âÅÌÐòÁÐºÅ¸ÄÎª×îºóÒ»¸öÐéÄâÐÅÏ¢,µ«²»¼ÇÈëÐéÄâÖ¡Í·µÄ×Ü¸öÊýÖÐ
			if(pstSDVFrameIndexTail->m_iSize == sizeof(TIFrameIndexHead)+(pstSDVFrameIndexHead->m_iNum+1)*(pstSDVFrameIndexHead->m_iInfoSize)
				 + MP4_APPEND_CHAR_INFO_SIZE + sizeof(TIFrameIndexTail))
			{
				int CD_NO = pstSDVFrameIndexHead->m_iNum;
				//±ê¼ÇÎª2µÄÐéÄâÐÅÏ¢Àï±£´æ×Å¹âÅÌÐòÁÐºÅ
				if(2 == (&pstSDVFrameIndexBody[CD_NO])->m_iSign)
				{
					#if 0
					memcpy(&pstMP4FrameIndexBody[CD_NO].m_stSdvIndexInfo, &pstSDVFrameIndexBody[CD_NO], sizeof(TIFrameIndexInfo));
					#else
					memcpy(&pstMP4FrameIndexBody[CD_NO].m_stSdvIndexInfo, &pstSDVFrameIndexBody[CD_NO], sizeof(TMP4ISampleIndexInfo));
					#endif
					
				}
		        else
		        {
					td_printf(0xff0000, "%s %s %d the file[%s] CDserialNum Sign is %d, not 2", __FILE__, __FUNCTION__, __LINE__, pstMP4Mgr->m_cSDVFilePath, (&pstSDVFrameIndexBody[CD_NO])->m_iSign);
				}
			}
		    else
		    {
		        td_printf(0xff0000, "%s %s %d the file[%s] has no CDserialNum", __FILE__, __FUNCTION__, __LINE__, pstMP4Mgr->m_cSDVFilePath);
		    }
			#endif
			//»ñÈ¡SDV ÐéÄâÖ¡¸½¼ÓÐÅÏ¢
			#if 0
			memcpy(pstMP4AppendCharInfo, pstSDVAppendCharInfo, sizeof(TAppendCharInfo));
			#else
			memcpy(pstMP4AppendCharInfo, pstSDVAppendCharInfo, MP4_APPEND_CHAR_INFO_SIZE);
			#endif
		}
		else
		{
			//×é½¨MP4 Í·²¿Ð£ÑéÐÅÏ¢
			memset(&pstMP4FrameIndexHead->m_stSdvIndexHead, 0, sizeof(TIFrameIndexHead));
 			pstMP4FrameIndexHead->m_stSdvIndexHead.m_iInfoSize = sizeof(TMP4ISampleIndexInfo);
			pstMP4FrameIndexHead->m_stSdvIndexHead.m_stHead.u32FrameID = ntohl(0x00000002);
			pstMP4FrameIndexHead->m_stSdvIndexHead.m_stHead.u8FrameType = FRAME_P;
		}

		pstMP4FrameIndexHead->m_stSdvIndexHead.m_iMemNum = pstMP4Mgr->m_uiMP4VirtualFramebufferSize;
		
		//Ìí¼ÓMP4 Î²±êÊ¶ 0x0123abce
		pstMP4FrameIndexTail->m_iFlag = REC_ISAMPLE_INDEX_MAGIC_NUMBER;

		if(iTmpSdvICount > 0)
		{
			#ifdef ZHENGFA
			iMP4VirtualFramLen = sizeof(TMP4ISampleIndexHead) + (iTmpSdvICount+1)*sizeof(TMP4ISampleIndexInfo) + MP4_APPEND_CHAR_INFO_SIZE + sizeof(TIFrameIndexTail);
			#else
			iMP4VirtualFramLen = sizeof(TMP4ISampleIndexHead) + iTmpSdvICount*sizeof(TMP4ISampleIndexInfo) + MP4_APPEND_CHAR_INFO_SIZE + sizeof(TIFrameIndexTail);
			#endif
		}
		else
		{
			iMP4VirtualFramLen = sizeof(TMP4ISampleIndexHead) + MP4_APPEND_CHAR_INFO_SIZE + sizeof(TIFrameIndexTail);
		}
		
		pstMP4FrameIndexTail->m_iSize = iMP4VirtualFramLen;

		pucMP4VirtualFramebuffer[0] = (iMP4VirtualFramBufLen-MP4_BOX_SIEZ_LENGTH)>>24;
		pucMP4VirtualFramebuffer[1] = ((iMP4VirtualFramBufLen-MP4_BOX_SIEZ_LENGTH)&0x00ff0000)>>16;
		pucMP4VirtualFramebuffer[2] = ((iMP4VirtualFramBufLen-MP4_BOX_SIEZ_LENGTH)&0x0000ff00)>>8;
		pucMP4VirtualFramebuffer[3] = (iMP4VirtualFramBufLen-MP4_BOX_SIEZ_LENGTH)&0x000000ff;

		if(iMP4VirtualFramBufLen <= 0)
		{
			td_printf(0xff0000, "%s %s %d the iMP4VirtualFramBufLen is [%d] too short!error!", __FILE__, __FUNCTION__, __LINE__, iMP4VirtualFramBufLen);
			iThisFunRes = td_failure;
			goto LABLE_TD_MP4_EXIT;
		}
		#if 0
		FILE* pfTestFile = NULL;
		char  strFileName[128] = {0};
		sprintf(strFileName, "/mnt/sata/4/1/%lu-vmf.hzg", time(NULL));

		pfTestFile = fopen(strFileName, "wb");
		fwrite(pucMP4VirtualFramebuffer, 1, iMP4VirtualFramBufLen, pfTestFile);
		fclose(pfTestFile);		
		#endif
		PrintInfo("write virtual frame");
		if(!MP4WriteSample(pstMP4Mgr->m_MP4Handle, pstMP4Mgr->m_VideoTrackID, pucMP4VirtualFramebuffer, iMP4VirtualFramBufLen, 1, 0, 0))  
		{
			td_printf(0xff0000, "%d: Call MP4WriteSample failed!\n", __LINE__);
			iThisFunRes = td_failure;
			goto LABLE_TD_MP4_EXIT;
		}
		td_printf(0, "%d: Finish writting MP4 virtual frame!iMP4VirtualFramBufLen=%d\n", __LINE__, iMP4VirtualFramBufLen);
						
		iThisFunRes = td_success;
		goto LABLE_TD_MP4_EXIT;		
	}
	
	iThisFunRes = td_success;
	
LABLE_TD_MP4_EXIT:
	return iThisFunRes;	
}

/************************************************* 
  Function: Inner_ShowMP4VirtualFrameInfo
  Description: ÏÔÊ¾MP4ÐéÄâÖ¡ÐÅÏ¢£¬²Î¿¼Inner_AnalyzeMP4VirtualFrameInfo£¬´Ë´¦²»ÔÙÀÛ×¸ÐðÊö
  Input: 
  Output: 
  Return: void
  Author: »ÆÖ¾¸Õ(Huangzhigang)
  Others: 
*************************************************/
static void Inner_ShowMP4VirtualFrameInfo(const TMP4ISampleIndexHead* _pstMP4FrameIndexHead, const TMP4ISampleIndexInfo* _pstMP4FrameIndexBody
						, const TAppendCharInfo* _pstMP4AppendCharInfo, const TIFrameIndexTail* _pstMP4FrameIndexTail)
{

	TMP4ISampleIndexHead* pstMP4FrameIndexHead = NULL;
	TMP4ISampleIndexInfo* pstMP4FrameIndexBody = NULL;
	TAppendCharInfo*	  pstMP4AppendCharInfo = NULL;
	TIFrameIndexTail*	  pstMP4FrameIndexTail = NULL;
	
	if(    (NULL == _pstMP4FrameIndexHead)
		&& (NULL == _pstMP4FrameIndexBody)
		&& (NULL == _pstMP4AppendCharInfo)
		&& (NULL == _pstMP4FrameIndexTail)
		)
	{
		td_printf(0xff0000, "%d: Input parameters is error![%p][%p][%p][%p]\n", __LINE__, _pstMP4FrameIndexHead
									,_pstMP4FrameIndexBody, _pstMP4AppendCharInfo, _pstMP4FrameIndexTail);
		goto LABLE_TD_MP4_EXIT;
	}
	
	pstMP4FrameIndexHead = (TMP4ISampleIndexHead*)_pstMP4FrameIndexHead;
	pstMP4FrameIndexBody = (TMP4ISampleIndexInfo*)_pstMP4FrameIndexBody;
	pstMP4AppendCharInfo = (TAppendCharInfo*)_pstMP4AppendCharInfo;
	pstMP4FrameIndexTail = (TIFrameIndexTail*)_pstMP4FrameIndexTail;

	if(NULL != pstMP4FrameIndexHead)
	{
		td_printf(0,"%d:\nMP4FrameIndexHead Info begin:\n", __LINE__);
		
		td_printf(0,"\n MP4FrameIndexHead->m_stSdvIndexHead begin:\n");
		
		td_printf(0," m_iInfoSize=%d\n",pstMP4FrameIndexHead->m_stSdvIndexHead.m_iInfoSize);
		td_printf(0," m_iMemNum  =%d\n",pstMP4FrameIndexHead->m_stSdvIndexHead.m_iMemNum);
		td_printf(0," m_iNum     =%d\n",pstMP4FrameIndexHead->m_stSdvIndexHead.m_iNum);
		td_printf(0,"\n  MP4FrameIndexHead->m_stSdvIndexHead.m_stHead begin:\n");
		td_printf(0,"  m_stHead.reserved     =%u\n",pstMP4FrameIndexHead->m_stSdvIndexHead.m_stHead.reserved );
		td_printf(0,"  m_stHead.u16AFrames   =%u\n",pstMP4FrameIndexHead->m_stSdvIndexHead.m_stHead.u16AFrames );
		td_printf(0,"  m_stHead.u16AFrameSize=%u\n",pstMP4FrameIndexHead->m_stSdvIndexHead.m_stHead.u16AFrameSize );
		td_printf(0,"  m_stHead.u32FrameID   =%u\n",pstMP4FrameIndexHead->m_stSdvIndexHead.m_stHead.u32FrameID );
		td_printf(0,"  m_stHead.u32FrameNO   =%u\n",pstMP4FrameIndexHead->m_stSdvIndexHead.m_stHead.u32FrameNO );
		td_printf(0,"  m_stHead.u32FrameSize =%u\n",pstMP4FrameIndexHead->m_stSdvIndexHead.m_stHead.u32FrameSize );
		td_printf(0,"  m_stHead.u32TimeStamp =%u\n",pstMP4FrameIndexHead->m_stSdvIndexHead.m_stHead.u32TimeStamp );
		td_printf(0,"  m_stHead.u32VStreamLen=%u\n",pstMP4FrameIndexHead->m_stSdvIndexHead.m_stHead.u32VStreamLen );
		td_printf(0,"  m_stHead.u8ACoder     =%u\n",pstMP4FrameIndexHead->m_stSdvIndexHead.m_stHead.u8ACoder );
		td_printf(0,"  m_stHead.u8FrameType  =%u\n",pstMP4FrameIndexHead->m_stSdvIndexHead.m_stHead.u8FrameType );
		td_printf(0,"\n  MP4FrameIndexHead->m_stSdvIndexHead.m_stHead end.\n");
		
		td_printf(0,"\n pstMP4FrameIndexHead->m_stExRecInfo begin:\n");
		td_printf(0," m_iFileEndTime=%d\n",pstMP4FrameIndexHead->m_stExRecInfo.m_iFileEndTime);
		td_printf(0,"\n pstMP4FrameIndexHead->m_stExRecInfo end.\n");
		
		td_printf(0,"\n pstMP4FrameIndexHead->m_stExRecInfo.m_stSdvFileHead begin:\n");
		td_printf(0," AChannels 	 =%d\n",pstMP4FrameIndexHead->m_stExRecInfo.m_stSdvFileHead.AChannels);
		td_printf(0," AudioSample	 =%u\n",pstMP4FrameIndexHead->m_stExRecInfo.m_stSdvFileHead.AudioSample);
		td_printf(0," bAudio		 =%d\n",pstMP4FrameIndexHead->m_stExRecInfo.m_stSdvFileHead.bAudio);
		td_printf(0," BitsPerSample  =%d\n",pstMP4FrameIndexHead->m_stExRecInfo.m_stSdvFileHead.BitsPerSample);
		td_printf(0," cCovWord		 =%s\n",pstMP4FrameIndexHead->m_stExRecInfo.m_stSdvFileHead.cCovWord);
		td_printf(0," CoverMask 	 =%u\n",pstMP4FrameIndexHead->m_stExRecInfo.m_stSdvFileHead.CoverMask);
		td_printf(0," Day			 =%d\n",pstMP4FrameIndexHead->m_stExRecInfo.m_stSdvFileHead.Day);
		td_printf(0," FrameCount	 =%u\n",pstMP4FrameIndexHead->m_stExRecInfo.m_stSdvFileHead.FrameCount);
		td_printf(0," FrameRate 	 =%d\n",pstMP4FrameIndexHead->m_stExRecInfo.m_stSdvFileHead.FrameRate);
		td_printf(0," Height		 =%d\n",pstMP4FrameIndexHead->m_stExRecInfo.m_stSdvFileHead.Height);
		td_printf(0," Hour			 =%d\n",pstMP4FrameIndexHead->m_stExRecInfo.m_stSdvFileHead.Hour);
		td_printf(0," Minute		 =%d\n",pstMP4FrameIndexHead->m_stExRecInfo.m_stSdvFileHead.Minute);
		td_printf(0," Mode			 =%d\n",pstMP4FrameIndexHead->m_stExRecInfo.m_stSdvFileHead.Mode);
		td_printf(0," Month 		 =%d\n",pstMP4FrameIndexHead->m_stExRecInfo.m_stSdvFileHead.Month);
		td_printf(0," m_uiFrameNoDiff=%u\n",pstMP4FrameIndexHead->m_stExRecInfo.m_stSdvFileHead.m_uiFrameNoDiff);
		td_printf(0," reserved		 =%d\n",pstMP4FrameIndexHead->m_stExRecInfo.m_stSdvFileHead.reserved);
		td_printf(0," TotalSize 	 =%d\n",pstMP4FrameIndexHead->m_stExRecInfo.m_stSdvFileHead.Second);
		td_printf(0," TotalSize 	 =%d\n",pstMP4FrameIndexHead->m_stExRecInfo.m_stSdvFileHead.TotalSize);
		td_printf(0," TriggerHigh	 =%d\n",pstMP4FrameIndexHead->m_stExRecInfo.m_stSdvFileHead.TriggerHigh);
		td_printf(0," TriggerLow	 =%d\n",pstMP4FrameIndexHead->m_stExRecInfo.m_stSdvFileHead.TriggerLow);
		td_printf(0," Width 		 =%d\n",pstMP4FrameIndexHead->m_stExRecInfo.m_stSdvFileHead.Width);
		td_printf(0," Year			 =%d\n",pstMP4FrameIndexHead->m_stExRecInfo.m_stSdvFileHead.Year);
		td_printf(0,"\n pstMP4FrameIndexHead->m_stExRecInfo.m_stSdvFileHead end.\n");
		td_printf(0,"%d:\nMP4FrameIndexHead Info end.\n", __LINE__);
	}
	
	if(NULL != pstMP4FrameIndexBody)
	{
		#if 0
		int i = 0;
		td_printf(0,"%d:\nMP4FrameIndexBody Info begin:\n", __LINE__);
		for(i=0; i<pstMP4FrameIndexHead->m_stSdvIndexHead.m_iNum; i++)
		{
			td_printf(0," ------------------------------>The %dst I frame info is:\n",i+1);
			
			td_printf(0,"MP4FrameIndexBody[i].m_stSdvIndexInfo begin:\n");
			pstMP4FrameIndexBody[i].m_stSdvIndexInfo.m_cTagName[63] = 0;
			td_printf(0," m_cTagName  =%s\n",pstMP4FrameIndexBody[i].m_stSdvIndexInfo.m_cTagName);
			td_printf(0," m_iPos      =%u\n",pstMP4FrameIndexBody[i].m_stSdvIndexInfo.m_iPos );
			td_printf(0," m_iSign     =%u\n",pstMP4FrameIndexBody[i].m_stSdvIndexInfo.m_iSign );
			td_printf(0," m_iTimeNow  =%u\n",(unsigned)pstMP4FrameIndexBody[i].m_stSdvIndexInfo.m_iTimeNow );
			td_printf(0," m_iVdeoSize =%u\n",pstMP4FrameIndexBody[i].m_stSdvIndexInfo.m_iVdeoSize );
			td_printf(0," MP4FrameIndexBody[i].m_stSdvIndexInfo end\n");

			td_printf(0," MP4FrameIndexBody[i].m_stVideoSampleIndexInfo begin:\n");
			td_printf(0," m_stVideoSampleIndexInfo.m_iSampleID        =%d\n",pstMP4FrameIndexBody[i].m_stVideoSampleIndexInfo.m_iSampleID);
			td_printf(0," m_stVideoSampleIndexInfo.m_tSampleTimeStamp =%d\n",pstMP4FrameIndexBody[i].m_stVideoSampleIndexInfo.m_tSampleTimeStamp);
			td_printf(0," MP4FrameIndexBody[i].m_stVideoSampleIndexInfo end\n");

			
			td_printf(0," MP4FrameIndexBody[i].m_stAudioSampleIndexInfo begin:\n");
			td_printf(0," m_stAudioSampleIndexInfo.m_iSampleID        =%d\n",pstMP4FrameIndexBody[i].m_stAudioSampleIndexInfo.m_iSampleID);
			td_printf(0," m_stAudioSampleIndexInfo.m_tSampleTimeStamp =%d\n",pstMP4FrameIndexBody[i].m_stAudioSampleIndexInfo.m_tSampleTimeStamp);
			td_printf(0," MP4FrameIndexBody[i].m_stAudioSampleIndexInfo end\n");
			
		}
		td_printf(0,"%d:\nMP4FrameIndexBody Info end.\n", __LINE__);
		#endif
	}
	
	if(NULL != pstMP4AppendCharInfo)
	{
		//pstMP4AppendCharInfo->m_pcCharInfo[MAX_RESERVED_INFO_LEN-1] = 0;
		td_printf(0,"%d:\nMP4AppendCharInfo Info begin:\n", __LINE__);
		td_printf(0," m_iFlag     =%x\n",pstMP4AppendCharInfo->m_iFlag);
		td_printf(0," m_iSize     =%u\n",pstMP4AppendCharInfo->m_iSize);
		//td_printf(0," m_pcCharInfo=%s\n",pstMP4AppendCharInfo->m_pcCharInfo);
		td_printf(0,"%d:\nMP4AppendCharInfo Info end.\n", __LINE__);
	}
	
	if(NULL != pstMP4FrameIndexTail)
	{
		//pstMP4FrameIndexTail->m_cCDSerialNo[MAX_CDROMSERIALNO_LEN-1] = 0;
		td_printf(0,"%d:\nMP4FrameIndexTail Info begin:\n", __LINE__);
		//td_printf(0," m_cCDSerialNo=%s\n",pstMP4FrameIndexTail->m_cCDSerialNo);
		td_printf(0," m_iFlag      =%x\n",pstMP4FrameIndexTail->m_iFlag );
		td_printf(0," m_iSize      =%d\n",pstMP4FrameIndexTail->m_iSize );
		td_printf(0,"%d:\nMP4FrameIndexTail Info end.\n", __LINE__);
	}
	
	td_printf(0, "%d:::[%p][%p][%p][%p]\n", __LINE__, pstMP4FrameIndexHead ,pstMP4FrameIndexBody, pstMP4AppendCharInfo, pstMP4FrameIndexTail);
LABLE_TD_MP4_EXIT:
	return;
}
static void Inner_ShowMP4VirtualFrameDemo(void *pstMP4Mgr)
{

	TMP4ISampleIndexHead* pstMP4FrameIndexHead = NULL;
	TMP4ISampleIndexInfo* pstMP4FrameIndexBody = NULL;
	TAppendCharInfo*	  pstMP4AppendCharInfo = NULL;
	TIFrameIndexTail*	  pstMP4FrameIndexTail = NULL;
	
	TIFrameIndexHead* pstSDVFrameIndexHead = NULL;

	if(NULL == pstMP4Mgr)
	{
		td_printf(0xff0000, "%d: Input parameters is error![%p]\n", __LINE__, pstMP4Mgr);
		goto LABLE_TD_MP4_EXIT;
	}

	TMP4Mgr* TmpMgr = (TMP4Mgr*)pstMP4Mgr;
	int iSdvIFrameNum = 0;

	if(HAVE_SDV_VIRTUAL_FRAME_FLAG == TmpMgr->m_bHaveSDVVirtualFrameFlag)
	{
		if(NULL != TmpMgr->m_pvSDVVirtualFramebuffer)
		{
			pstSDVFrameIndexHead = (TIFrameIndexHead*)((MP4_VOID*)TmpMgr->m_pvSDVVirtualFramebuffer 
				+ MP4_BOX_SIEZ_LENGTH + MP4_SAMPLE_TYPE_LENGTH);
			iSdvIFrameNum = pstSDVFrameIndexHead->m_iNum;
		}
	}
	Inner_AnalyzeMP4VirtualFrameInfo(TmpMgr->m_pvMP4VirtualFramebuffer,TmpMgr->m_uiMP4VirtualFramebufferSize
									, &pstMP4FrameIndexHead, &pstMP4FrameIndexBody
									, &pstMP4AppendCharInfo, &pstMP4FrameIndexTail, iSdvIFrameNum);
	Inner_ShowMP4VirtualFrameInfo(pstMP4FrameIndexHead,pstMP4FrameIndexBody,pstMP4AppendCharInfo,pstMP4FrameIndexTail);


LABLE_TD_MP4_EXIT:
	return;

}


/************************************************* 
  Function: Inner_WriteSDVAVSampleToMP4
  Description: ½«ÒôÊÓÆµÊý¾ÝÐ´ÈëMP4
  Input: _pvMP4Mgr£¬MP4 ¹ÜÀí½á¹¹
  Input: _pvSDVVedioBuf£¬ÊÓÆµÊý¾Ý
  Input: _iSDVVedioBufLen£¬ÊÓÆµÊý¾Ý³¤¶È
  Input: _pvSDVAudioBuf£¬ÒôÆµÊý¾Ý
  Input: _iSDVAudioBufLen£¬ÒôÆµÊý¾Ý³¤¶È
  Output:
  Return: º¯ÊýÖ´ÐÐ³É¹¦·µ»Ø td_success, Ê§°Ü·µ»Ø td_failure
  Author: »ÆÖ¾¸Õ(Huangzhigang)
  Others: 
*************************************************/
static int Inner_WriteSDVAVSampleToMP4(void* _pvMP4Mgr, void* _pvSDVVedioBuf, int _iSDVVedioBufLen, void* _pvSDVAudioBuf, int _iSDVAudioBufLen)
{
	TMP4Mgr* pstMP4Mgr = (TMP4Mgr*)_pvMP4Mgr;
	int iThisFunRes = td_success;
	int iTmpRes = 0;
	void* pvSDVVedioBuf = NULL;
	int iSDVVedioBufLen = 0;
	void* pvSDVAudioBuf = NULL;
	int iSDVAudioBufLen = 0;
	
	if((NULL == pstMP4Mgr) || (NULL == _pvSDVVedioBuf) ||(0 >= _iSDVVedioBufLen))
	{
		td_printf(0xff0000, "%d: Input parameters is error![%p][%p][%d]\n", __LINE__,_pvMP4Mgr,
			_pvSDVVedioBuf, _iSDVVedioBufLen);

		iThisFunRes = td_failure;
		goto LABLE_TD_MP4_EXIT;
	}
	
	if( ((NULL == _pvSDVAudioBuf) && (0 < _iSDVAudioBufLen)) || ((NULL != _pvSDVAudioBuf) && (0 > _iSDVAudioBufLen)) )
	{
		td_printf(0xff0000, "%d: Input audio parameters is error![%p][%d]\n", __LINE__,_pvSDVAudioBuf,_iSDVAudioBufLen);
		iThisFunRes = td_failure;
		goto LABLE_TD_MP4_EXIT;
	}

	pvSDVVedioBuf = _pvSDVVedioBuf;
	pvSDVAudioBuf = _pvSDVAudioBuf;
	iSDVVedioBufLen = _iSDVVedioBufLen;
	iSDVAudioBufLen = _iSDVAudioBufLen;
	td_printf(0, "%s %d test!", __FUNCTION__, __LINE__);	
	//Ð´ÈëMP4 ÎÄ¼þ
	static int judgeVideoCodec = 0;
	static int isH265 = 0;
	if(0 == judgeVideoCodec)
	{
		AVProbeData avPb;
		memset(&avPb, 0, sizeof(avPb));
		init_hevc_probe(&avPb, (const unsigned char*)pvSDVVedioBuf, (iSDVVedioBufLen > 2048 ? 2048 : iSDVVedioBufLen));
		
		if(hevc_probe(&avPb) > 0)
		{
			isH265 = 1;
			td_printf(0, "%s %d isH265 = 1!", __FUNCTION__, __LINE__);
		}else
		{
			isH265 = 0;
			td_printf(0, "%s %d isH265 = 0!", __FUNCTION__, __LINE__);
		}

		free_hevc_pro(&avPb);

		judgeVideoCodec = 1;
	}

	if(isH265 == 1)
	{
		iTmpRes = Inner_WriteH265(pstMP4Mgr, (unsigned char*)pvSDVVedioBuf, iSDVVedioBufLen);
	}else if(isH265 == 0)
	{
		iTmpRes = Inner_WriteH264(pstMP4Mgr, (unsigned char*)pvSDVVedioBuf, iSDVVedioBufLen);
	}
	 
	if (iTmpRes < 0)  
	{  
		td_printf(0xff0000, "%d: Call Inner_WriteH264! iTmpRes is %d\n", __LINE__, iTmpRes);
		iThisFunRes = iTmpRes;
		goto LABLE_TD_MP4_EXIT;
	}
	td_printf(0, "%s %d test!", __FUNCTION__, __LINE__);
	if(iSDVAudioBufLen > 0)
	{
		switch(pstMP4Mgr->m_stSDVFrameHead.u8ACoder)
		{
			case AUDIO_TYPE_G711A:
			{
				//td_printf(0, "------------>AUDIO_TYPE_G711A, frame len is %d", pstMP4Mgr->m_stSDVFrameHead.u16AFrameSize);
				iTmpRes = Inner_WriteAlaw(pstMP4Mgr, (unsigned char*)pvSDVAudioBuf, iSDVAudioBufLen); 
				break;
			}
			case AUDIO_TYPE_G711U:
			{
				//td_printf(0, "------------>AUDIO_TYPE_G711U, frame len is %d", pstMP4Mgr->m_stSDVFrameHead.u16AFrameSize);
				iTmpRes = Inner_WriteUlaw(pstMP4Mgr, (unsigned char*)pvSDVAudioBuf, iSDVAudioBufLen); 
				break;
			}
			case AUDIO_TYPE_MEDIA_AAC:
			{
				//td_printf(0, "------------>AUDIO_TYPE_AAC, frame len is %d", pstMP4Mgr->m_stSDVFrameHead.u16AFrameSize);
				iTmpRes = Inner_WriteAac(pstMP4Mgr, (unsigned char*)pvSDVAudioBuf, iSDVAudioBufLen); 
				break;
			}
			case AUDIO_TYPE_DVI4:
			{
				//td_printf(0, "------------>AUDIO_TYPE_ADPCM, frame len is %d", pstMP4Mgr->m_stSDVFrameHead.u16AFrameSize);
				iTmpRes = Inner_WriteAdpcm(pstMP4Mgr, (unsigned char*)pvSDVAudioBuf, iSDVAudioBufLen); 
				break;
			}
			default:
			{
				td_printf(0xff0000, "%d: The Audio Type Is Not Support!\n", __LINE__);
				//iThisFunRes = td_failure;
				//goto LABLE_TD_MP4_EXIT;
			}
			
		}
		td_printf(0, "%s %d test!", __FUNCTION__, __LINE__);	
		if (td_failure == iTmpRes)	
		{  
			td_printf(0xff0000, "%d: Call Write Audio data failed!\n", __LINE__);
			//iThisFunRes = td_failure;
			//goto LABLE_TD_MP4_EXIT;
		}
	}
	iThisFunRes = td_success;
	
LABLE_TD_MP4_EXIT:
	return iThisFunRes;	
}


/************************************************* 
  Function: Inner_StartConvertSDVToMP4
  Description: ¿ªÊ¼½«SDV ÎÄ¼þ×ª»»³ÉMP4 ÎÄ¼þ
  Input: _pvMP4Mgr £¬MP4 ¹ÜÀí½á¹¹
  Output: ÎÞ
  Return: º¯ÊýÖ´ÐÐ³É¹¦·µ»Ø td_success, Ê§°Ü·µ»Ø td_failure
  Author: »ÆÖ¾¸Õ(Huangzhigang)
  Others: 
*************************************************/
static int Inner_StartConvertSDVToMP4(void* _pvMP4Mgr)
{
	TMP4Mgr* pstMP4Mgr = (TMP4Mgr*)_pvMP4Mgr;
	int iThisFunRes = td_failure;
	S_FileHeader* pstFileHead = NULL;
	S_FrameHeader* pstSDVAVFrameHead = NULL;
	int iTmpRes = 0;
	void* pvSDVVedioBuf = NULL;
	int iSDVVedioBufLen = 0;
	void* pvSDVAudioBuf = NULL;
	int iSDVAudioBufLen = 0;
#ifdef TD_VFS
	TVfile* pfSDVFileHdl = NULL;
#else
	FILE*	pfSDVFileHdl = NULL;
#endif
	int iConditionsOne = 0;
	int iConditionsTwo = 0;

	if(NULL == pstMP4Mgr)
	{
		td_printf(0xff0000, "%d: Input parameters is error!\n", __LINE__);
		iThisFunRes = td_failure;
		goto LABLE_TD_MP4_EXIT;
	}

	td_printf(0, "%d: Inner_StartConvertSDVToMP4 begin!\n", __LINE__);

	pstFileHead = &(pstMP4Mgr->m_stSDVFileHead);
	pstSDVAVFrameHead = &(pstMP4Mgr->m_stSDVFrameHead);
	pfSDVFileHdl = pstMP4Mgr->m_pfSDVFileHandle;
	
	//»ñÈ¡SDV ÎÄ¼þÍ·ÐÅÏ¢
	iTmpRes = Inner_GetSDVFileHeadInfo(pstMP4Mgr, pstFileHead, sizeof(S_FileHeader));
	if (td_failure == iTmpRes)  
	{  
		td_printf(0xff0000, "%d: Call Inner_GetSDVFileHeadInfo failed!\n", __LINE__);
		iThisFunRes = td_failure;
		goto LABLE_TD_MP4_EXIT;
	}

	//ÏÔÊ¾SDV ÎÄ¼þÍ·ÐÅÏ¢
	Inner_ShowSDVFileHeadInfo(pstFileHead,sizeof(S_FileHeader));

	int iCount = 0;

	while(pstMP4Mgr->m_bConvertExitFlag == TD_MP4_CONVERT_RUNNING)
	{
		td_printf(0, "11============>m_lCurFilePos is %ld", pstMP4Mgr->m_lCurFilePos);
		
		if(1 == MP4_GetForceStopState())
		{
			td_printf(0xff0000, "---------------------------Force Stop The Current Convert!");
			iThisFunRes = td_failure;
			goto LABLE_TD_MP4_EXIT;
		}

		td_printf(0, "22============>m_lCurFilePos is %ld", pstMP4Mgr->m_lCurFilePos);

		iSDVVedioBufLen = 0;
		iSDVAudioBufLen = 0;

		//»ñÈ¡ÒôÊÓÆµÊý¾Ý
		iTmpRes = Inner_GetSDVAVSample((void*)pstMP4Mgr, &pvSDVVedioBuf, &iSDVVedioBufLen, &pvSDVAudioBuf, &iSDVAudioBufLen);
		if (iTmpRes < 0)  
		{  
			td_printf(0xff0000, "%d: Call Inner_GetSDVAVSample failed!\n", __LINE__);
			goto STOP_CONVERT;
		}
		td_printf(0, "33============>m_lCurFilePos is %ld", pstMP4Mgr->m_lCurFilePos);
		//½«ÒôÊÓÆµÊý¾ÝÐ´ÈëMP4 ÎÄ¼þ
		iTmpRes = Inner_WriteSDVAVSampleToMP4(pstMP4Mgr, pvSDVVedioBuf, iSDVVedioBufLen, pvSDVAudioBuf, iSDVAudioBufLen);
		if (td_failure == iTmpRes)  
		{  
			td_printf(0xff0000, "%d: Call Inner_WriteSDVAVSampleToMP4 failed!\n", __LINE__);
			iThisFunRes = td_failure;
			goto LABLE_TD_MP4_EXIT;
		}
		else if(MP4_CONVERT_ERROR_SPSPPS_CONTINUE == iTmpRes)
		{
			if(pstMP4Mgr->m_lStartConvertPos > 0)
			{
				//Ö±½Ó¶¨Î»µ½Óû¿ªÊ¼×ª»»µÄÎ»ÖÃ
				td_mp4_fseek((void*)(pstMP4Mgr->m_pfSDVFileHandle),pstMP4Mgr->m_lStartConvertPos,SEEK_SET);
				td_printf(0x12345678, "seek to the start pos[%ld]", pstMP4Mgr->m_lStartConvertPos);
			}
			continue;
		}
		td_printf(0, "44============>m_lCurFilePos is %ld", pstMP4Mgr->m_lCurFilePos);
		//  ¼ÇÂ¼MP4 ÐéÄâÖ¡ÐÅÏ¢
		if(HAVE_SDV_VIRTUAL_FRAME_FLAG == pstMP4Mgr->m_bHaveSDVVirtualFrameFlag)
		{
			PrintInfo("HAVE_SDV_VIRTUAL_FRAME_FLAG================");
			iTmpRes = Inner_WriteIndexInfoToMP4VirtualFrame(pstMP4Mgr, 0);
			if (td_failure == iTmpRes)  
			{  
				td_printf(0xff0000, "%d: Call Inner_WriteIndexInfoToMP4VirtualFrame failed!\n", __LINE__);
				iThisFunRes = td_failure;
				goto LABLE_TD_MP4_EXIT;
			}
		}

		td_printf(0, "55============>m_lCurFilePos is %ld", pstMP4Mgr->m_lCurFilePos);
		pstMP4Mgr->m_lCurFilePos = td_mp4_ftell((void*)pfSDVFileHdl);
		
		if(-1 == pstMP4Mgr->m_lCurFilePos)
		{
			td_printf(0xff0000, "%d: ftell failed! Error info[%s].\n", __LINE__, strerror(errno));
			iThisFunRes = td_failure;
			goto LABLE_TD_MP4_EXIT;
			
		}
		td_printf(0, "66============>m_lCurFilePos is %ld", pstMP4Mgr->m_lCurFilePos);
		if((iConditionsOne=(0!=td_mp4_feof((void*)pfSDVFileHdl))) 
			|| (iConditionsTwo=(pstMP4Mgr->m_lCurFilePos >= pstMP4Mgr->m_lStopConvertPos)))
		{
			pstMP4Mgr->m_bConvertExitFlag = TD_MP4_CONVERT_STOP;
			td_printf(0x12345678, "pstMP4Mgr->m_lCurFilePos is %ld", pstMP4Mgr->m_lCurFilePos);
			td_printf(0x12345678, "pstMP4Mgr->m_lStopConvertPos is %ld", pstMP4Mgr->m_lStopConvertPos);
			td_printf(0x12345678, "%d: pstSDVAVFrameHead->reserved=%u,%x!\n", __LINE__, pstSDVAVFrameHead->reserved, pstSDVAVFrameHead->u32FrameID);
			td_printf(0x12345678, "%d: iConditionsOne=%d,iConditionsTwo=%d!\n", __LINE__, iConditionsOne, iConditionsTwo);
		}
		td_printf(0, "77============>m_lCurFilePos is %ld", pstMP4Mgr->m_lCurFilePos);
		#if 1 
		iCount++;

		if(iCount == 10)
		{
			//Í£¶ÙÒ»ÏÂ£¬·ÀÖ¹cpu ¹ý¸ß
			MP4USLEEP(5);
			iCount = 0;
		}
		#endif
	}
	
	//  Ð´ÈëMP4 ÐéÄâÖ¡ÐÅÏ¢
STOP_CONVERT:
	{
		iTmpRes = Inner_WriteIndexInfoToMP4VirtualFrame(pstMP4Mgr, 1);
		if (td_failure == iTmpRes)  
		{  
			td_printf(0xff0000, "%d: Call Inner_WriteIndexInfoToMP4VirtualFrame failed!\n", __LINE__);
			iThisFunRes = td_failure;
			goto LABLE_TD_MP4_EXIT;
		}
	}	
	iThisFunRes = td_success;
	
LABLE_TD_MP4_EXIT:
	td_printf(0, "%d: Inner_StartConvertSDVToMP4 end!\n", __LINE__);
	return iThisFunRes;
}

static int Inner_CompleteFileName(char* _pcFileName, char* _pcFullPath, int _iPathMaxLen)
{
	int iRet = td_success;
	char cFullPath[FULL_PATH_LEN] = {0};

	if((NULL == _pcFileName)||(NULL == _pcFullPath))
	{
		td_printf(0xff0000,"%s %s %d error!",__FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}
	
	td_printf(0, "Inner_CompleteFileName _pcFileName is [%s]", _pcFileName);

#ifdef FOR_MP4SDK
	strncpy(cFullPath, (char*)_pcFileName, FULL_PATH_LEN - 1);
#else
	if (strlen((char *)_pcFileName) <= 29)	
	{
		if (rec_CompleteFileName((td_s8 *)_pcFileName, (td_s8 *)cFullPath, NULL, NULL) != ERR_REC_SUCCESSFUL)
		{
			td_printf(0xff0000,"%s %s %d error!",__FILE__, __FUNCTION__, __LINE__);
			iRet = td_failure;
			goto leave;
		}
	}
	else
	{
		strncpy(cFullPath, (char*)_pcFileName, FULL_PATH_LEN - 1);
	}
#endif

	strncpy(_pcFullPath, cFullPath, _iPathMaxLen - 1);
	td_printf(0, "Inner_CompleteFileName _pcFullPath is [%s]", _pcFullPath);
leave:
	return iRet;
}

static int Inner_GetInfoFromFirstFree(int _iReadType, MP4_VOID* _pFileHandle, unsigned int _iCdNo, char* _pcInfoType, unsigned int* _pNeedInfo)
{
	int iRet = td_failure;
	off_t OrgPos = 0;	
	unsigned int uiReadLen = 0;
	unsigned int uiNeedInfo = 0;
	unsigned int uiReadPos  = 0;
	unsigned int uiInfoTmp = 0;
	unsigned int uiVersionBefore = 0;
	unsigned int uiVersionAfter  = 0;
	
	if((NULL == _pcInfoType)||(NULL == _pNeedInfo))
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		goto leave;
	}
	if(0 == memcmp(_pcInfoType, ENCRYPTION_TYPE, MP4_BOX_TYPE_LENGTH))
	{
		uiReadPos = 36;			
	}
	else if(0 == memcmp(_pcInfoType, AUDIO_ENCODE_TYPE, MP4_BOX_TYPE_LENGTH))
	{
		uiReadPos = 40;
	}
	else
	{
		uiReadPos = 44;//mdat size
	}
	if(READFILE == _iReadType)
	{
		if(NULL != _pFileHandle)
		{
			OrgPos = td_mp4_ftell(_pFileHandle);
			td_mp4_fseek(_pFileHandle, 32, SEEK_SET);
			uiReadLen = td_mp4_fread(&uiVersionBefore, 1, 4, _pFileHandle);
			printfdataX("uiVersionBefore",(char *)&uiVersionBefore,4);
			ConvertSort((unsigned char*)&uiVersionAfter, uiVersionBefore);
			if((uiReadLen != 4)||(1 != uiVersionAfter))
			{
				td_printf(0xff0000, "%s %s %d error! uiReadLen is %d, Version is %d", __FILE__, __FUNCTION__, __LINE__, uiReadLen, uiVersionAfter);
				goto leave;
			}

			td_mp4_fseek(_pFileHandle, uiReadPos, SEEK_SET);
			uiReadLen = td_mp4_fread((unsigned char*)&uiInfoTmp, 1, sizeof(uiInfoTmp), _pFileHandle);
		}
	}
	else if(READCDROM == _iReadType)
	{
		if(NULL != gBigFile_ReadDisk)
		{
			uiReadLen = gBigFile_ReadDisk(_iCdNo, 32, (unsigned char*)&uiVersionBefore, sizeof(uiVersionBefore));	
			printfdataX("uiVersionBefore",(char *)&uiVersionBefore,4);
			ConvertSort((unsigned char*)&uiVersionAfter, uiVersionBefore);

			if((uiReadLen != 4)||(1 != uiVersionAfter))
			{
				td_printf(0xff0000, "%s %s %d error! uiReadLen is %d, Version is %d", __FILE__, __FUNCTION__, __LINE__, uiReadLen, uiVersionAfter);
				goto leave;
			}
			
			uiReadLen = gBigFile_ReadDisk(_iCdNo, uiReadPos, (unsigned char*)&uiInfoTmp, sizeof(uiInfoTmp));	
		}
	}
	else
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		goto leave;
	}

	if(uiReadLen != sizeof(uiInfoTmp))
	{
		td_printf(0xff0000, "%s %s %d error! uiReadLen is %d\n", __FILE__, __FUNCTION__, __LINE__, uiReadLen);
		goto leave;
	}				
	else
	{
		ConvertSort((unsigned char*)&uiNeedInfo, uiInfoTmp);
		*_pNeedInfo = uiNeedInfo;
	}
	iRet = td_success;
leave:
	if((READFILE == _iReadType)&&(NULL != _pFileHandle))
	{
		td_mp4_fseek(_pFileHandle, OrgPos, SEEK_SET);
	}
	return iRet;
}

static int Inner_GetAudioTypeFromOldMP4File(char* _pcFullPath, int* _piAudioType)
{
	td_s32 iRet = td_failure;
	char* pMP4Info = NULL;

	if((NULL == _pcFullPath)||(NULL == _piAudioType))
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		goto leave;
	}

#ifdef CWM//FOR_MP4SDK cwm_todo
	//add for windows 
	char cOutbuf[1024] = {0};
	AnsiToUTF8(_pcFullPath,strlen(_pcFullPath)+1,cOutbuf,sizeof(cOutbuf));
	pMP4Info = MP4FileInfo(cOutbuf, MP4_INVALID_TRACK_ID);
	//add end
#else
	pMP4Info = MP4FileInfo(_pcFullPath, MP4_INVALID_TRACK_ID);
#endif
	
	if(NULL == pMP4Info)
	{
		td_printf(0xff0000, "the file[%s] is not MP4 file,please cheak!", _pcFullPath);
		goto leave;
	}
	else
	{
		td_printf(0x12345678, "MP4_File[%s] info :\n%s\n", _pcFullPath, pMP4Info);
	}

#ifdef FOR_MP4SDK
	if (strstr(pMP4Info, MP4_AAC_TYPE_SIGN) != NULL) 
	{
		*_piAudioType = AUDIO_TYPE_MEDIA_AAC;
	} 
	else if(strstr(pMP4Info, MP4_G711A_TYPE_SIGN) != NULL)
	{
		*_piAudioType = AUDIO_TYPE_G711A;
	} 
	else if(strstr(pMP4Info, MP4_G711U_TYPE_SIGN) != NULL) 
	{
		*_piAudioType = AUDIO_TYPE_G711U;
	}
	else if((strstr(pMP4Info, MP4_PCM_TYPE_SIGN) != NULL)
	||(strstr(pMP4Info, MP4_PCM_TYPE_SIGN2) != NULL))
	{
		*_piAudioType = AUDIO_TYPE_DVI4;
	}
#else
	if (NULL != strcasestr(pMP4Info, MP4_AAC_TYPE_SIGN)) 
	{
		*_piAudioType = AUDIO_TYPE_MEDIA_AAC;
	} 
	else if(NULL != strcasestr(pMP4Info, MP4_G711A_TYPE_SIGN))
	{
		*_piAudioType = AUDIO_TYPE_G711A;
	} 
	else if(NULL != strcasestr(pMP4Info, MP4_G711U_TYPE_SIGN)) 
	{
		*_piAudioType = AUDIO_TYPE_G711U;
	}
	else if(NULL != strcasestr(pMP4Info, MP4_PCM_TYPE_SIGN)) 
	{
		*_piAudioType = AUDIO_TYPE_DVI4;
	}
#endif
	else
	{
		td_printf(0xff0000, "the audio type has not find or can't support!");	
		goto leave;
	}

	iRet = td_success;
	
leave:
	if(NULL != pMP4Info)
	{
		free(pMP4Info);
		pMP4Info = NULL;
	}
	return iRet;
}

static int Inner_GetFileOtherInfo(void* _pFileHandle, int _iType, unsigned int* _puiInfo)
{
	int iRet = td_failure;
	S_FrameHeader stSDVAVFrameHead = {0};
	int iCount = 0;
	int iReadLen = 0;
	unsigned int uiInfo = 0xff;
	off_t OrgPos = 0;	

	if((NULL == _pFileHandle)||(NULL == _puiInfo))
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		goto EXIT;
	}

	OrgPos = td_mp4_ftell(_pFileHandle);

	//Ô¤¶Á10 Ö¡£¬»ñÈ¡ÏàÓ¦ÐÅÏ¢
	while((0 ==td_mp4_feof(_pFileHandle))&&(iCount < 10))
	{
		td_printf(0, "iCount is %d", iCount);
		
		iReadLen = td_mp4_fread(&stSDVAVFrameHead, 1, sizeof(S_FrameHeader), (void*)_pFileHandle);
		
		if(sizeof(S_FrameHeader) != iReadLen)
		{
			td_printf(0xff0000, "%s %s %d error! iReadLen is %d", __FILE__, __FUNCTION__, __LINE__, iReadLen);
			if(0 != td_mp4_feof(_pFileHandle))
			{
				td_printf(0x12345678, "%s %s %d has reach end of file!", __FILE__, __FUNCTION__, __LINE__);
				break;
			}
			goto EXIT;
		}

		if(ENCT == _iType)
		{
			if((stSDVAVFrameHead.reserved & 0x03) == 0x02)
			{
				uiInfo = 1;//¼ÓÃÜ´ú±íÖµ
				break;
			}
		}
		else if(ADET == _iType)
		{
			if(0xff != stSDVAVFrameHead.u8ACoder)
			{
				uiInfo = stSDVAVFrameHead.u8ACoder;
				break;
			}
		}
		else
		{
			break;
		}

		td_mp4_fseek(_pFileHandle, stSDVAVFrameHead.u32FrameSize-sizeof(S_FrameHeader), SEEK_CUR);
		iCount++;
	}

	if(0xff != uiInfo)
	{
		*_puiInfo = uiInfo;
		td_printf(0, "%d Type is %d, uiInfo is %d", __LINE__, _iType, *_puiInfo);
	}
	else 
	{
		if(ENCT == _iType)
		{
			*_puiInfo = 0;//²»¼ÓÃÜ´ú±íÖµ
			td_printf(0, "%d Type is %d, uiInfo is %d", __LINE__, _iType, *_puiInfo);
		}
		else
		{
			*_puiInfo = uiInfo;
			td_printf(0, "%d Type is %d, uiInfo is %d", __LINE__, _iType, *_puiInfo);
		}
	}

	iRet = td_success;
EXIT:
	if(NULL != _pFileHandle)
	{
		td_mp4_fseek(_pFileHandle, OrgPos, SEEK_SET);
	}
	return iRet;
}

/************************************************* 
  Function: MP4_GetMP4VideoCodecId
  Description: ÅÐ¶Ïmp4ÎÄ¼þÊÓÆµ±àÂëµÄÀàÐÍ
  Input: _MP4Hdl£¬			mp4ÎÄ¼þ¾ä±ú
  Output: pCodecId£¬±àÂëÀàÐÍ
  Return: º¯ÊýÖ´ÐÐ³É¹¦·µ»Ø td_success, Ê§°Ü·µ»Ø td_failure
  Author: ³ÂÎÄÃô(chenwenmin)
  Others: 
*************************************************/
int MP4_GetMP4VideoCodecId(void* _MP4Hdl, AVCodecID *pCodecId)
{
	int ret = -1;
	if(NULL == _MP4Hdl)
	{
		return -1;
	}
	const char *s2 = MP4GetTrackMediaDataName(_MP4Hdl, 1);
	if(!strcmp("avc1", s2))
	{
		*pCodecId = AV_CODEC_ID_H264;
		ret = 0;
	}else if(!strcmp("hev1", s2))
	{
		*pCodecId = AV_CODEC_ID_HEVC;
		ret = 0;
	}else
	{
		//todo
		*pCodecId = (AVCodecID)-1;
		ret = -1;
	}

	return ret;
}

int MP4_GetMP4FreeVerSion(char* _pcFileName, int _iFreeNo, unsigned int* _puiVersion)
{
	int iRet = td_failure;
	int iReadPos = 0;
	off_t OrgPos = 0;
	unsigned int uiReadLen = 0;
	unsigned int uiVersionBefore = 0;
	unsigned int uiVersionAfter  = 0;
#ifdef TD_VFS
	TVfile* pfFileHdl = NULL;
#else
	FILE*	pfFileHdl = NULL;
#endif
	char cFullPath[FULL_PATH_LEN] = {0};

	if((NULL == _pcFileName)||(NULL == _puiVersion))
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		goto leave;
	}

	if(Inner_CompleteFileName(_pcFileName, cFullPath, FULL_PATH_LEN) < 0)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		goto leave;
	}

	pfFileHdl = (FILE*)td_mp4_fopen(NULL, cFullPath, "rb");

	if(NULL == pfFileHdl)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		goto leave;
	}

	OrgPos = td_mp4_ftell(pfFileHdl);

	if(1 == _iFreeNo)
	{
		iReadPos = 32;
		td_mp4_fseek(pfFileHdl, iReadPos, SEEK_SET);
	}
	else if(2 == _iFreeNo)
	{
		iReadPos = -128;//µ¹Êý128 ×Ö½Ú¿ªÊ¼
		td_mp4_fseek(pfFileHdl, iReadPos, SEEK_END);
	}
	else
	{
		td_printf(0xff0000, "%s %s %d error! Free[%d] Not Exit!", __FILE__, __FUNCTION__, __LINE__, _iFreeNo);
		goto leave;
	}

	uiReadLen = td_mp4_fread(&uiVersionBefore, 1, 4, pfFileHdl);
	printfdataX("uiVersionBefore",(char *)&uiVersionBefore,4);
	ConvertSort((unsigned char*)&uiVersionAfter, uiVersionBefore);

	*_puiVersion = uiVersionAfter;

	td_printf(0, "%s %d ============>>>>>>>>>>>>Cur MP4 Version is %u", __FUNCTION__, __LINE__, uiVersionAfter);
	iRet = td_success;
leave:
	if(NULL != pfFileHdl)
	{
		td_mp4_fseek(pfFileHdl, OrgPos, SEEK_SET);
		td_mp4_fclose(pfFileHdl);
		pfFileHdl = NULL;
	}
	return iRet;
}

int MP4_IsSDVFile(const char* _pcFileName)
{
	S_FileHeader stFileHeader = {0};
#ifdef TD_VFS
	TVfile* pfSDVFileHdl = NULL;
#else
	FILE*	pfSDVFileHdl = NULL;
#endif
	int iThisFunRes = td_success;
	int iTmpRes = 0;
	
	if(NULL == _pcFileName)
	{
		td_printf(0xff0000, "%s %s %d: Input parameters is error!\n", __FILE__, __FUNCTION__, __LINE__);
		iThisFunRes = td_failure;
		goto LABLE_TD_MP4_EXIT;
	}

	char cFullPath[FULL_PATH_LEN] = {0};

	if(Inner_CompleteFileName((char*)_pcFileName, cFullPath, FULL_PATH_LEN) < 0)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iThisFunRes = td_false;
		goto LABLE_TD_MP4_EXIT;
	}
		
	//´ò¿ªÎÄ¼þ
#ifdef TD_VFS
	pfSDVFileHdl = (TVfile*)td_mp4_fopen(NULL, cFullPath, "rb");
#else
	pfSDVFileHdl = (FILE*)td_mp4_fopen(NULL, cFullPath, "rb");
#endif
	
	if(NULL == pfSDVFileHdl)
	{
		td_printf(0xff0000, "%s %s %d: Open file failed! Error info[%s].\n", __FILE__, __FUNCTION__, __LINE__, strerror(errno));
		iThisFunRes = td_failure;
		goto LABLE_TD_MP4_EXIT;
	}
	
	//¶ÁÈ¡ÎÄ¼þÍ·"
	iTmpRes = td_mp4_fread(&(stFileHeader), 1, sizeof(S_FileHeader), (void*)pfSDVFileHdl);
	
	if(sizeof(S_FileHeader) != iTmpRes)
	{
		td_printf(0xff0000, "%d: Read failed! Error info[%s].\n", __LINE__, strerror(errno));
		iThisFunRes = td_failure;
		goto LABLE_TD_MP4_EXIT;
	}
	td_printf(0, "%s\n",stFileHeader.cCovWord);
	
	//ÅÐ¶ÏÎÄ¼þ
	if(0 != strcmp(stFileHeader.cCovWord, "S MULTI-MEDIA STREAM (H.264)"))
	{
		td_printf(0xff0000, "%d: Not sdv file!\n", __LINE__);
		iThisFunRes = td_failure;
		goto LABLE_TD_MP4_EXIT;
	}
	
LABLE_TD_MP4_EXIT:
	
	if(NULL != pfSDVFileHdl)
	{
		td_mp4_fclose((void*)pfSDVFileHdl);
		pfSDVFileHdl = NULL;
	}
	
	return iThisFunRes;
}

int MP4_SetForceStopState(int _iState)
{
	g_iForceStopFlag = _iState;
	return td_success;
}

int MP4_GetForceStopState()
{
	//td_printf(0, "%s %s %d g_iForceStopFlag is %d", __FILE__, __FUNCTION__, __LINE__, g_iForceStopFlag);
	return g_iForceStopFlag;
}

/************************************************* 
  Function: MP4_ConvertSdvToMp4
  Description: ÓÃÓÚ½«SDV ÎÄ¼þ×ª»»³É±ê×¼MP4 ÎÄ¼þ
  Input: _pcSrcFilePath £¬Ô´SDV ÎÄ¼þÂ·¾¶
  Input: _pcDstFilePath £¬Ä¿±ê´ýÉú³ÉµÄMP4 ÎÄ¼þÂ·¾¶
  Output:
  Return: º¯ÊýÖ´ÐÐ³É¹¦·µ»Ø td_success, Ê§°Ü·µ»Ø td_failure
  Author: »ÆÖ¾¸Õ(Huangzhigang)
  Others: 
*************************************************/
int MP4_ConvertSdvToMp4(const char* _pcSrcFilePath, char* _pcDstFilePath)
{
	void* pstMP4Mgr = NULL;
	char pcSrcFilePath[FILENAMEMAXNUM] = {0};
	char pcDstFilePath[FILENAMEMAXNUM] = {0};
	int iThisFunRes = td_success;
	
	if(NULL == _pcSrcFilePath || NULL == _pcDstFilePath)
	{
		td_printf(0xff0000, "%d: Input parameters is error!\n", __LINE__);
		iThisFunRes = td_failure;
		goto LABLE_TD_MP4_EXIT;
	}

	MP4_SetForceStopState(0);

	strncpy(pcSrcFilePath, _pcSrcFilePath, FILENAMEMAXNUM - 1);
	strncpy(pcDstFilePath, _pcDstFilePath, FILENAMEMAXNUM - 1);

	td_printf(0x12345678, "--------------------------------->%d: MP4_ConvertSdvToMp4 begin!SRC_PATH=%s DST_PATH=%s\n", __LINE__,pcSrcFilePath, pcDstFilePath);

	//ÅÐ¶ÏÊÇ·ñÊÇSDV ÎÄ¼þ
	if(MP4_IsSDVFile(pcSrcFilePath) < 0)
	{
		td_printf(0xff0000, "%d: Source File is not SDV File or file is damaged!\n", __LINE__);
		iThisFunRes = td_failure;
		goto LABLE_TD_MP4_EXIT;
	}

	if(Inner_InitMP4Mgr(&pstMP4Mgr,pcSrcFilePath,pcDstFilePath) < 0)
	{
		td_printf(0xff0000, "%d: Call Inner_InitMP4Mgr failed!\n", __LINE__);
		iThisFunRes = td_failure;
		goto LABLE_TD_MP4_EXIT;
	}
	
	iThisFunRes = Inner_StartConvertSDVToMP4(pstMP4Mgr);

	if(iThisFunRes < 0)
	{
		td_printf(0xff0000, "%d: Call Inner_StartConvertSDVToMP4 failed!\n", __LINE__);
		goto LABLE_TD_MP4_EXIT;
	}
	
	Inner_ShowMP4VirtualFrameDemo(pstMP4Mgr);
LABLE_TD_MP4_EXIT:
	Inner_DestoryMP4Mgr(pstMP4Mgr);
	td_printf(0x12345678, "--------------------------------->%d: MP4_ConvertSdvToMp4 end! iThisFunRes is %d\n", __LINE__, iThisFunRes);
	MP4_SetForceStopState(0);
	return 0;
	if(iThisFunRes < 0)
	{
		if(strlen(pcDstFilePath) > 0)
		{
			td_mp4_unlink(pcDstFilePath);
		}
	}
	return iThisFunRes;
}

int MP4_ConvertSdvToMp4Ex(const char* _pcSrcFilePath,  char* _pcDstFilePath, unsigned int _uiStartPos, unsigned int _uiEndPos)
{
	TMP4Mgr* pstMP4Mgr = NULL;
	char pcSrcFilePath[FILENAMEMAXNUM] = {0};
	char pcDstFilePath[FILENAMEMAXNUM] = {0};
	int iThisFunRes = td_success;
	
	if(NULL == _pcSrcFilePath || NULL == _pcDstFilePath)
	{
		td_printf(0xff0000, "%d: Input parameters is error!\n", __LINE__);
		iThisFunRes = td_failure;
		goto LABLE_TD_MP4_EXIT;
	}

	MP4_SetForceStopState(0);

	strncpy(pcSrcFilePath, _pcSrcFilePath, FILENAMEMAXNUM - 1);
	strncpy(pcDstFilePath, _pcDstFilePath, FILENAMEMAXNUM - 1);

	td_printf(0x12345678, "--------------------------------->%d: MP4_ConvertSdvToMp4 begin!SRC_PATH=%s DST_PATH=%s\n", __LINE__,pcSrcFilePath, pcDstFilePath);

	//ÅÐ¶ÏÊÇ·ñÊÇSDV ÎÄ¼þ
	if(MP4_IsSDVFile(pcSrcFilePath) < 0)
	{
		td_printf(0xff0000, "%d: Source File is not SDV File or file is damaged!\n", __LINE__);
		iThisFunRes = td_failure;
		goto LABLE_TD_MP4_EXIT;
	}

	td_printf(0, "before pstMP4Mgr is %p", pstMP4Mgr);

	if(Inner_InitMP4Mgr((void**)(&pstMP4Mgr),pcSrcFilePath,pcDstFilePath) < 0)
	{
		td_printf(0xff0000, "%d: Call Inner_InitMP4Mgr failed!\n", __LINE__);
		iThisFunRes = td_failure;
		goto LABLE_TD_MP4_EXIT;
	}

	td_printf(0, "-----------------3333------------------");
	td_printf(0, "pstMP4Mgr->m_cMP4FilePath is %s", pstMP4Mgr->m_cMP4FilePath);
	td_printf(0, "pstMP4Mgr->m_bHaveSDVVirtualFrameFlag is %d", pstMP4Mgr->m_bHaveSDVVirtualFrameFlag);
	td_printf(0, "pstMP4Mgr->m_lStartConvertPos is %ld", pstMP4Mgr->m_lStartConvertPos);
	td_printf(0, "pstMP4Mgr->m_lStopConvertPos is %ld", pstMP4Mgr->m_lStopConvertPos);
	td_printf(0, "pstMP4Mgr->m_lFileStopPos is %ld", pstMP4Mgr->m_lFileStopPos);
	td_printf(0, "pstMP4Mgr->m_ucVideoSampleDataBuf is %p", pstMP4Mgr->m_ucVideoSampleDataBuf);
	td_printf(0, "-----------------3333------------------");

	td_printf(0, "recv start[%d] stop[%d]", _uiStartPos, _uiEndPos);
	td_printf(0, "cur start[%ld] stop[%ld] pstMP4Mgr is %p", 
		pstMP4Mgr->m_lStartConvertPos, pstMP4Mgr->m_lStopConvertPos, pstMP4Mgr);

	td_printf(0, "-----------------4444------------------");
	td_printf(0, "pstMP4Mgr->m_cMP4FilePath is %s", pstMP4Mgr->m_cMP4FilePath);
	td_printf(0, "pstMP4Mgr->m_bHaveSDVVirtualFrameFlag is %d", pstMP4Mgr->m_bHaveSDVVirtualFrameFlag);
	td_printf(0, "pstMP4Mgr->m_lStartConvertPos is %ld", pstMP4Mgr->m_lStartConvertPos);
	td_printf(0, "pstMP4Mgr->m_lStopConvertPos is %ld", pstMP4Mgr->m_lStopConvertPos);
	td_printf(0, "pstMP4Mgr->m_lFileStopPos is %ld", pstMP4Mgr->m_lFileStopPos);
	td_printf(0, "pstMP4Mgr->m_ucVideoSampleDataBuf is %p", pstMP4Mgr->m_ucVideoSampleDataBuf);
	td_printf(0, "pstMP4Mgr is %p", pstMP4Mgr);
	td_printf(0, "-----------------4444------------------");

	if(_uiStartPos  > 0)
	{
		pstMP4Mgr->m_lStartConvertPos = _uiStartPos;
	}

	if((_uiEndPos > 0)&&(_uiEndPos < pstMP4Mgr->m_lStopConvertPos))
	{
		pstMP4Mgr->m_lStopConvertPos = _uiEndPos;
	}

	td_printf(0, "after start pos is %ld, stop pos is %ld", pstMP4Mgr->m_lStartConvertPos, pstMP4Mgr->m_lStopConvertPos);
	
	iThisFunRes = Inner_StartConvertSDVToMP4((void*)pstMP4Mgr);

	if(iThisFunRes < 0)
	{
		td_printf(0xff0000, "%d: Call Inner_StartConvertSDVToMP4 failed!\n", __LINE__);
		goto LABLE_TD_MP4_EXIT;
	}
	
	Inner_ShowMP4VirtualFrameDemo((void*)pstMP4Mgr);
LABLE_TD_MP4_EXIT:
	
	Inner_DestoryMP4Mgr((void*)pstMP4Mgr);
	td_printf(0x12345678, "--------------------------------->%d: MP4_ConvertSdvToMp4 end! iThisFunRes is %d\n", __LINE__, iThisFunRes);
	MP4_SetForceStopState(0);
	return iThisFunRes;
	if(iThisFunRes < 0)
	{
		if(strlen(pcDstFilePath) > 0)
		{
			td_mp4_unlink(pcDstFilePath);
		}
	}
	return iThisFunRes;
}
////////////////////////////////////////////////////////////////////////////////////////////////////combine end

int MP4_IsMP4File(char* _pcFileName)
{		
	int iRet = td_false;
	int iTmpRes = 0;
	char cftypflag[8] = {0};
	char cMp4Type[8] = {0};
#ifdef TD_VFS
	TVfile* test_fp = NULL; 	 
#else
	FILE* test_fp = NULL;		 
#endif
	
	if(NULL == _pcFileName)
	{
		td_printf(0xff0000,"%d, _pucPath = NULL\n",__LINE__);
		iRet = td_false;
		goto leave;
	}
	
	char cFullPath[FULL_PATH_LEN] = {0};

	if(Inner_CompleteFileName(_pcFileName, cFullPath, FULL_PATH_LEN) < 0)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_false;
		goto leave;
	}

#ifdef TD_VFS
	test_fp = (TVfile*)td_mp4_fopen(NULL,cFullPath,"rb");
#else
	test_fp = (FILE*)td_mp4_fopen(NULL,cFullPath,"rb");
#endif

	if(NULL == test_fp)
	{		
		td_printf(0xff0000, "%s %s %d error! td_mp4_fopen failed!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_false;
		goto leave;
	}

	iTmpRes = td_mp4_fseek((void*)test_fp, MP4_BOX_SIEZ_LENGTH, SEEK_SET);
	
	if(-1 == iTmpRes)
	{
		td_printf(0xff0000, "%d: Seek failed! Error info[%s].\n", __LINE__, strerror(errno));
		iRet = td_false;
		goto leave;
	}
	//td_printf(0, "%d: Current nonius pos=%ld.\n", __LINE__, td_mp4_ftell((void*)(test_fp)));

	iTmpRes = td_mp4_fread(cftypflag, 1, MP4_FILE_FLAG_LENGTH, (void*)(test_fp));

	if(MP4_FILE_FLAG_LENGTH != iTmpRes)
	{
		td_printf(0xff0000, "%d: Seek failed! Error info[%s].\n", __LINE__, strerror(errno));
		iRet = td_false;
		goto leave;
	}

	if(!strstr(cftypflag, "ftyp"))
	{
		td_printf(0xff0000, "cftypflag[%s] the file is not mp4 file, please check!", cftypflag);
		iRet = td_false;
		goto leave;
	}

	iTmpRes = td_mp4_fread(cMp4Type, 1, MP4_FILE_TYPE_LENGTH, (void*)(test_fp));

	if(MP4_FILE_TYPE_LENGTH != iTmpRes)
	{
		td_printf(0xff0000, "%d: Seek failed! Error info[%s].\n", __LINE__, strerror(errno));
		iRet = td_false;
		goto leave;
	}

	if(strstr(cMp4Type, "mp42"))
	{
		td_printf(0x12345678, "cMp4Type[%s] the file is td_mp4 file!", cMp4Type);
		iRet = TYPE_TIANDY_MP4_FILE;
		goto leave;
	}
	else
	{
		td_printf(0xff0000, "cMp4Type[%s] the file is mp4 file, but not td_mp4 file, please check!", cMp4Type);
		iRet = TYPE_OTHER_MP4_FILE;
		goto leave;
	}
	
leave:
	if(NULL != test_fp)
	{
		td_mp4_fclose((void*)test_fp);
		test_fp = NULL;
	}	
	return iRet;
}

int MP4_CheckFileConvertState(char* _pcFileName)
{
	int iRet = td_success;
	MP4FileHandle pMP4fp  = NULL;

	if(NULL == _pcFileName)
	{
		td_printf(0xff0000, "the %s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}

	char cFullPath[FULL_PATH_LEN] = {0};

	if(Inner_CompleteFileName(_pcFileName, cFullPath, FULL_PATH_LEN) < 0)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}
	
	pMP4fp  = MP4Read(cFullPath);

	if(NULL == pMP4fp)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}

	unsigned int uTrackId = MP4_INVALID_TRACK_ID;

	if(MP4_GetTrackId_V2(pMP4fp, &uTrackId, MP4_VIDEO_TRACK_TYPE) < 0)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}

	MP4SampleId numSamples_vedio = MP4GetTrackNumberOfSamples(pMP4fp, uTrackId);
	uint32_t sample_size_vedio = MAX_VIDEO_SAMPLE_SIZE;
	uint8_t  sample_vedio[MAX_VIDEO_SAMPLE_SIZE] = {0};
	MP4Duration sampleDuration_vedio = 0;
	bool isSyncSample_vedio = false;

	int iVSampleId = numSamples_vedio;//ÐéÄâsample¹Ì¶¨Îªµ¹ÊýµÚÒ»¸ösample

	if(Inner_ReadOneVideoSample(pMP4fp,uTrackId,
		 iVSampleId, sample_vedio, 
		 &sample_size_vedio,&sampleDuration_vedio,&isSyncSample_vedio) < 0)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}

	//sampleÍùºóÆ«ÒÆ(4 +1) ¸ö×Ö½Ú¡¾Ö¡´óÐ¡4  ¡¿¡¾Ö¡ÀàÐÍ1¡¿£¬ÎªË÷Òý¿ªÊ¼²¿Î»
	char* pIndexBeginPos = (char*)(sample_vedio + MP4_BOX_SIEZ_LENGTH + MP4_SAMPLE_TYPE_LENGTH);

	TMP4ISampleIndexHead* pstMP4ISampleIndexHead = NULL;
	TMP4ISampleIndexTail* pstMP4ISampleIndexTail = NULL;
	
	pstMP4ISampleIndexHead = (TMP4ISampleIndexHead*)(pIndexBeginPos);

	if(NULL == pstMP4ISampleIndexHead)
	{
		td_printf(0xff0000, "the %s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}

	if(pstMP4ISampleIndexHead->m_stSdvIndexHead.m_stHead.u32FrameID != 0x02000000)
	{
		td_printf(0xff0000, "the %s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}

	int iMP4ISampleNum = pstMP4ISampleIndexHead->m_stSdvIndexHead.m_iNum;
	int	iMP4ISampleInfoSize = pstMP4ISampleIndexHead->m_stSdvIndexHead.m_iInfoSize;
	int iMP4ISampleIndexLen = iMP4ISampleNum*iMP4ISampleInfoSize;
 
	if(iMP4ISampleNum > 0)
	{
		#ifdef ZHENGFA
		pstMP4ISampleIndexTail = (TMP4ISampleIndexTail*)(pIndexBeginPos + 
			sizeof(TMP4ISampleIndexHead) + iMP4ISampleIndexLen + iMP4ISampleInfoSize + MP4_APPEND_CHAR_INFO_SIZE);
		#else
		pstMP4ISampleIndexTail = (TMP4ISampleIndexTail*)(pIndexBeginPos + 
			sizeof(TMP4ISampleIndexHead) + iMP4ISampleIndexLen + MP4_APPEND_CHAR_INFO_SIZE);
		#endif
	}
	else
	{
		pstMP4ISampleIndexTail = (TMP4ISampleIndexTail*)(pIndexBeginPos + 
			sizeof(TMP4ISampleIndexHead) + MP4_APPEND_CHAR_INFO_SIZE);
	}

	if(NULL == pstMP4ISampleIndexTail)	
	{
		td_printf(0xff0000, "the %s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}

	if(pstMP4ISampleIndexTail->m_iFlag != REC_ISAMPLE_INDEX_MAGIC_NUMBER)
	{
		td_printf(0xff0000, "the %s %s %d error!m_iFlag is %x", __FILE__, __FUNCTION__, __LINE__, pstMP4ISampleIndexTail->m_iFlag);
		iRet = td_failure;
		goto leave;
	}

leave:
	
	if(NULL != pMP4fp)
	{
		MP4Close(pMP4fp, 0);
		pMP4fp = NULL;
	}

	if(NULL != _pcFileName)
	{
		if(td_success == iRet)
		{
			td_printf(0x12345678, "The File[%s] Has Been Converted Over! You Can Go On, Boy!", _pcFileName);
		}
		else
		{
			td_printf(0xff0000, "The File[%s] Hasn't Been Converted Over! Try Convert It Again, Boy!", _pcFileName);
		}
	}

	return iRet;
}

int MP4_GetTrackId(char* _pcFileName, unsigned int* _puTrackId, char* _pcTrackType)
{
	td_s32 iRet = td_success;
	MP4FileHandle pMP4fp= NULL;

	if((NULL == _pcFileName)||(NULL == _puTrackId)||(NULL == _pcTrackType))
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}

	char cFullPath[FULL_PATH_LEN] = {0};

	if(Inner_CompleteFileName(_pcFileName, cFullPath, FULL_PATH_LEN) < 0)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}

	td_s32 i = 0;
	uint32_t numTracks = 0;
	
	pMP4fp  = MP4Read(cFullPath);

	if(NULL == pMP4fp)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}

	if (*_puTrackId == MP4_INVALID_TRACK_ID) 
	{
		numTracks = MP4GetNumberOfTracks(pMP4fp, NULL, 0);
	
		for ( i = 0; i < numTracks; i++ ) 
		{
			*_puTrackId = MP4FindTrackId(pMP4fp, i, _pcTrackType, 0);
	
			if(*_puTrackId != MP4_INVALID_TRACK_ID)
			{
				break;
			}
		}
	}
	
	if(*_puTrackId == MP4_INVALID_TRACK_ID)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}
	else
	{
		iRet = td_success;
	}
leave:
	if(NULL != pMP4fp)
	{
		MP4Close(pMP4fp, 0);
		pMP4fp = NULL;
	}
    return iRet;
}

int MP4_GetTrackId_V2(void* _pMP4Handle, unsigned int* _puTrackId, char* _pcTrackType)
{
	td_s32 iRet = td_success;

	if((NULL == _pMP4Handle)||(NULL == _puTrackId)||(NULL == _pcTrackType))
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}
	
	td_s32 i = 0;
	uint32_t numTracks = 0;

	if (*_puTrackId == MP4_INVALID_TRACK_ID) 
	{
		numTracks = MP4GetNumberOfTracks(_pMP4Handle, NULL, 0);
	
		for ( i = 0; i < numTracks; i++ ) 
		{
			*_puTrackId = MP4FindTrackId(_pMP4Handle, i, _pcTrackType, 0);
	
			if(*_puTrackId != MP4_INVALID_TRACK_ID)
			{
				break;
			}
		}
	}
	
	if(*_puTrackId == MP4_INVALID_TRACK_ID)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}
	else
	{
		iRet = td_success;
	}
leave:
	
	return iRet;
}

int MP4_GetTrackSampleNums(char* _pcFileName, unsigned int _uTrackId, unsigned int* _piNums)
{
	td_s32 iRet = td_success;
	MP4FileHandle pMP4fp= NULL;

	if((NULL == _pcFileName)||(NULL == _piNums)||(MP4_INVALID_TRACK_ID == _uTrackId))
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}

	char cFullPath[FULL_PATH_LEN] = {0};

	if(Inner_CompleteFileName(_pcFileName, cFullPath, FULL_PATH_LEN) < 0)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}

#ifdef CWM//FOR_MP4SDK cwm_todo
	//add for windows 
	char cOutbuf[1024] = {0};
	AnsiToUTF8(cFullPath,strlen(cFullPath)+1,cOutbuf,sizeof(cOutbuf));
	pMP4fp	= MP4Read(cOutbuf);
	//add end
#else
	pMP4fp	= MP4Read(cFullPath);
#endif

	if(NULL == pMP4fp)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}

	*_piNums = MP4GetTrackNumberOfSamples(pMP4fp, _uTrackId);

	if(*_piNums < 0)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}
leave:
	if(NULL != pMP4fp)
	{
		MP4Close(pMP4fp, 0);
		pMP4fp = NULL;
	}
	return iRet;
}

/************************************************* 
  Function: MP4_GetSpsPps
  Description: »ñÈ¡mp4ÊÓÆµÎÄ¼þ¶ÔÓ¦±àÂë¸ñÊ½µÄ²ÎÊý¼¯Êý¾Ý
  Input: _pcFileName £¬mp4ÎÄ¼þÂ·¾¶
  Output:_pcSps
  Output:_piSpsSize
  Output:_pcPps
  Output:_piPpsSize
  Return: º¯ÊýÖ´ÐÐ³É¹¦·µ»Ø td_success, Ê§°Ü·µ»Ø td_failure
  Author: ³ÂÎÄÃô(chenwenmin)
  Others: ÐÞ¸Ä£¬Ìí¼Ó¶Ôh265±àÂëµÄÖ§³Ö
*************************************************/

int MP4_GetSpsPps(char* _pcFileName, char* _pcSps, int* _piSpsSize, char* _pcPps, int* _piPpsSize)
{
	int iRet = td_success;
	MP4FileHandle pMP4fp = NULL;
	int iVpsFind = 0;
	int iSpsFind = 0;
	int iPpsFind = 0;
	uint8_t **vidheader = NULL;
	uint8_t **seqheader = NULL;
	uint8_t **pictheader = NULL;
	uint32_t *pictheadersize = NULL;
	uint32_t *seqheadersize = NULL;
	uint32_t *vidheadersize = NULL;
	uint32_t ix;

	if((NULL == _pcFileName)||(NULL == _pcSps)||(NULL == _piSpsSize)||(NULL == _pcPps)||(NULL == _piPpsSize))
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}

	char cFullPath[FULL_PATH_LEN] = {0};

	if(Inner_CompleteFileName(_pcFileName, cFullPath, FULL_PATH_LEN) < 0)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}
	
	pMP4fp  = MP4Read(cFullPath);

	if(NULL == pMP4fp)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}

	unsigned int uTrackId = MP4_INVALID_TRACK_ID;

	if(MP4_GetTrackId(cFullPath, &uTrackId, MP4_VIDEO_TRACK_TYPE) < 0)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}

	AVCodecID vCodecType;
	if(MP4_GetMP4VideoCodecId(pMP4fp, &vCodecType) < 0)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}

	int iCount = 0;
	switch(vCodecType)
	{
		case AV_CODEC_ID_H264:
		MP4GetTrackH264SeqPictHeaders(pMP4fp, uTrackId,
									  &seqheader, &seqheadersize,
									  &pictheader, &pictheadersize);		
		
		if((NULL != seqheader)&&(NULL != pictheader)
			&&(NULL != seqheadersize)&&(NULL != pictheadersize))
		{
			for (ix = 0; seqheadersize[ix] != 0; ix++) 
			{
				if((seqheadersize[ix] < *_piSpsSize)&&(0 == iSpsFind))
				{
					memcpy(_pcSps, seqheader[ix], seqheadersize[ix]);
					*_piSpsSize = seqheadersize[ix];
					iSpsFind = 1;
				}
					
				for(iCount = 0; iCount < seqheadersize[ix]; iCount++)
				{
					td_printf(0, "the sps[%d][%d] is :%x", ix, iCount, seqheader[ix][iCount]);	
				}
			
				free(seqheader[ix]);
			}
		
			td_printf(0, "-------------------------------------------------------------->total sps num is %d", ix);
		
			for (ix = 0; pictheadersize[ix] != 0; ix++) 
			{
				if((pictheadersize[ix] < *_piPpsSize)&&(0 == iPpsFind))
				{
					memcpy(_pcPps, pictheader[ix], pictheadersize[ix]);
					*_piPpsSize = pictheadersize[ix];
					iPpsFind = 1;
				}
		
				for(iCount = 0; iCount < pictheadersize[ix]; iCount++)
				{
					td_printf(0, "the pps[%d][%d] is :%x", ix, iCount, pictheader[ix][iCount]); 
				}
			
				free(pictheader[ix]);
			}
		
			if((0 == *_piSpsSize)||(0 == *_piPpsSize))
			{
				td_printf(0xff0000, "%s %s %d failed!", __FILE__, __FUNCTION__, __LINE__);
				iRet = td_failure;
				goto leave;
			}
			
			td_printf(0, "-------------------------------------------------------------->total pps num is %d", ix);
		}
			break;
			
		case AV_CODEC_ID_HEVC:
		MP4GetTrackH265SeqPictHeaders(pMP4fp, uTrackId,
									  &vidheader, &vidheadersize,
									  &seqheader, &seqheadersize,
									  &pictheader, &pictheadersize);
				
		if((NULL != vidheader)&&(NULL != seqheader)&&(NULL != pictheader)
			&&(NULL != vidheadersize)&&(NULL != seqheadersize)&&(NULL != pictheadersize))
		{
			//vps
			int vpsSize = 0;
			for (ix = 0; vidheadersize[ix] != 0; ix++) 
			{
				if((vidheadersize[ix] < *_piSpsSize)&&(0 == iVpsFind))
				{
					memcpy(_pcSps, vidheader[ix], vidheadersize[ix]);
					vpsSize = vidheadersize[ix];
					iVpsFind = 1;
				}
					
				for(iCount = 0; iCount < vidheadersize[ix]; iCount++)
				{
					td_printf(0, "the sps[%d][%d] is :%x", ix, iCount, vidheader[ix][iCount]);	
				}
			
				free(vidheader[ix]);
			}
			//sps
			for (ix = 0; seqheadersize[ix] != 0; ix++) 
			{
				if((seqheadersize[ix] < (*_piSpsSize - vpsSize))&&(0 == iSpsFind))
				{
					unsigned char naluStart[4] = {0x00, 0x00, 0x00, 0x01};
					memcpy(_pcSps + vpsSize, naluStart, sizeof(naluStart));
					vpsSize += sizeof(naluStart);
					memcpy(_pcSps + vpsSize, seqheader[ix], seqheadersize[ix]);
					*_piSpsSize = vpsSize + seqheadersize[ix];
					iSpsFind = 1;
				}
					
				for(iCount = 0; iCount < seqheadersize[ix]; iCount++)
				{
					td_printf(0, "the sps[%d][%d] is :%x", ix, iCount, seqheader[ix][iCount]);	
				}
			
				free(seqheader[ix]);
			}

			td_printf(0, "-------------------------------------------------------------->total sps num is %d", ix);
			//pps
			for (ix = 0; pictheadersize[ix] != 0; ix++) 
			{
				if((pictheadersize[ix] < *_piPpsSize)&&(0 == iPpsFind))
				{
					memcpy(_pcPps, pictheader[ix], pictheadersize[ix]);
					*_piPpsSize = pictheadersize[ix];
					iPpsFind = 1;
				}

				for(iCount = 0; iCount < pictheadersize[ix]; iCount++)
				{
					td_printf(0, "the pps[%d][%d] is :%x", ix, iCount, pictheader[ix][iCount]); 
				}
			
				free(pictheader[ix]);
			}

			if((0 == *_piSpsSize)||(0 == *_piPpsSize))
			{
				td_printf(0xff0000, "%s %s %d failed!", __FILE__, __FUNCTION__, __LINE__);
				iRet = td_failure;
				goto leave;
			}
			
			td_printf(0, "-------------------------------------------------------------->total pps num is %d", ix);
		}
			break;

		default:
			break;
	}

leave:
	//vps
	if(NULL != vidheader)
	{
		free(vidheader);
		vidheader = NULL;
	}

	if(NULL != vidheadersize)
	{
		free(vidheadersize);
		vidheadersize = NULL;
	}
	//sps
	if(NULL != seqheader)
	{
		free(seqheader);
		seqheader = NULL;
	}

	if(NULL != seqheadersize)
	{
		free(seqheadersize);
		seqheadersize = NULL;
	}
	//pps
	if(NULL != pictheader)
	{
		free(pictheader);
		pictheader = NULL;
	}

	if(NULL != pictheadersize)
	{
		free(pictheadersize);
		pictheadersize = NULL;
	}

	if(NULL != pMP4fp)
	{
		MP4Close(pMP4fp, 0);
		pMP4fp = NULL;
	}
	return iRet;
}


/************************************************* 
  Function: MP4_GetSpsPps
  Description: »ñÈ¡mp4ÊÓÆµÎÄ¼þ¶ÔÓ¦±àÂë¸ñÊ½µÄ²ÎÊý¼¯Êý¾Ý
  Input: _pMP4Handle £¬mp4ÎÄ¼þ¾ä±ú
  Output:_pcSps
  Output:_piSpsSize
  Output:_pcPps
  Output:_piPpsSize
  Return: º¯ÊýÖ´ÐÐ³É¹¦·µ»Ø td_success, Ê§°Ü·µ»Ø td_failure
  Author: ³ÂÎÄÃô(chenwenmin)
  Others: ÐÞ¸Ä£¬Ìí¼Ó¶Ôh265±àÂëµÄÖ§³Ö
*************************************************/
int MP4_GetSpsPps_V2(void* _pMP4Handle, char* _pcSps, int* _piSpsSize, char* _pcPps, int* _piPpsSize)
{
	int iRet = td_success;
	int iVpsFind = 0;
	int iSpsFind = 0;
	int iPpsFind = 0;
	uint8_t **vidheader = NULL;
	uint8_t **seqheader = NULL;
	uint8_t **pictheader = NULL;
	uint32_t *pictheadersize = NULL;
	uint32_t *seqheadersize = NULL;
	uint32_t *vidheadersize = NULL;
	uint32_t ix;

	if((_pMP4Handle == NULL)||(NULL == _pcSps)||(NULL == _piSpsSize)||(NULL == _pcPps)||(NULL == _piPpsSize))
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}

	unsigned int uTrackId = MP4_INVALID_TRACK_ID;
	
	if(MP4_GetTrackId_V2(_pMP4Handle, &uTrackId, MP4_VIDEO_TRACK_TYPE) < 0)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}

	
	AVCodecID vCodecType;
	if(MP4_GetMP4VideoCodecId(_pMP4Handle, &vCodecType) < 0)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}

	int iCount = 0;
	switch(vCodecType)
	{
		case AV_CODEC_ID_H264:
		MP4GetTrackH264SeqPictHeaders(_pMP4Handle, uTrackId,
									  &seqheader, &seqheadersize,
									  &pictheader, &pictheadersize);		
		
		if((NULL != seqheader)&&(NULL != pictheader)
			&&(NULL != seqheadersize)&&(NULL != pictheadersize))
		{
			for (ix = 0; seqheadersize[ix] != 0; ix++) 
			{
				if((seqheadersize[ix] < *_piSpsSize)&&(0 == iSpsFind))
				{
					memcpy(_pcSps, seqheader[ix], seqheadersize[ix]);
					*_piSpsSize = seqheadersize[ix];
					iSpsFind = 1;
				}
					
				for(iCount = 0; iCount < seqheadersize[ix]; iCount++)
				{
					td_printf(0, "the sps[%d][%d] is :%x", ix, iCount, seqheader[ix][iCount]);	
				}
			
				free(seqheader[ix]);
			}
		
			td_printf(0, "-------------------------------------------------------------->total sps num is %d", ix);
		
			for (ix = 0; pictheadersize[ix] != 0; ix++) 
			{
				if((pictheadersize[ix] < *_piPpsSize)&&(0 == iPpsFind))
				{
					memcpy(_pcPps, pictheader[ix], pictheadersize[ix]);
					*_piPpsSize = pictheadersize[ix];
					iPpsFind = 1;
				}
		
				for(iCount = 0; iCount < pictheadersize[ix]; iCount++)
				{
					td_printf(0, "the pps[%d][%d] is :%x", ix, iCount, pictheader[ix][iCount]); 
				}
			
				free(pictheader[ix]);
			}
		
			if((0 == *_piSpsSize)||(0 == *_piPpsSize))
			{
				td_printf(0xff0000, "%s %s %d failed!", __FILE__, __FUNCTION__, __LINE__);
				iRet = td_failure;
				goto leave;
			}
			
			td_printf(0, "-------------------------------------------------------------->total pps num is %d", ix);
		}
			break;
			
		case AV_CODEC_ID_HEVC:
		MP4GetTrackH265SeqPictHeaders(_pMP4Handle, uTrackId,
									  &vidheader, &vidheadersize,
									  &seqheader, &seqheadersize,
									  &pictheader, &pictheadersize);
				
		if((NULL != vidheader)&&(NULL != seqheader)&&(NULL != pictheader)
			&&(NULL != vidheadersize)&&(NULL != seqheadersize)&&(NULL != pictheadersize))
		{
			//vps
			int vpsSize = 0;
			for (ix = 0; vidheadersize[ix] != 0; ix++) 
			{
				if((vidheadersize[ix] < *_piSpsSize)&&(0 == iVpsFind))
				{
					memcpy(_pcSps, vidheader[ix], vidheadersize[ix]);
					vpsSize = vidheadersize[ix];
					iVpsFind = 1;
				}
					
				for(iCount = 0; iCount < vidheadersize[ix]; iCount++)
				{
					td_printf(0, "the sps[%d][%d] is :%x", ix, iCount, vidheader[ix][iCount]);	
				}
			
				free(vidheader[ix]);
			}
			//sps
			for (ix = 0; seqheadersize[ix] != 0; ix++) 
			{
				if((seqheadersize[ix] < (*_piSpsSize - vpsSize))&&(0 == iSpsFind))
				{
					unsigned char naluStart[4] = {0x00, 0x00, 0x00, 0x01};
					memcpy(_pcSps + vpsSize, naluStart, sizeof(naluStart));
					vpsSize += sizeof(naluStart);
					memcpy(_pcSps + vpsSize, seqheader[ix], seqheadersize[ix]);
					*_piSpsSize = vpsSize + seqheadersize[ix];
					iSpsFind = 1;
				}
					
				for(iCount = 0; iCount < seqheadersize[ix]; iCount++)
				{
					td_printf(0, "the sps[%d][%d] is :%x", ix, iCount, seqheader[ix][iCount]);	
				}
			
				free(seqheader[ix]);
			}

			td_printf(0, "-------------------------------------------------------------->total sps num is %d", ix);
			//pps
			for (ix = 0; pictheadersize[ix] != 0; ix++) 
			{
				if((pictheadersize[ix] < *_piPpsSize)&&(0 == iPpsFind))
				{
					memcpy(_pcPps, pictheader[ix], pictheadersize[ix]);
					*_piPpsSize = pictheadersize[ix];
					iPpsFind = 1;
				}

				for(iCount = 0; iCount < pictheadersize[ix]; iCount++)
				{
					td_printf(0, "the pps[%d][%d] is :%x", ix, iCount, pictheader[ix][iCount]); 
				}
			
				free(pictheader[ix]);
			}

			if((0 == *_piSpsSize)||(0 == *_piPpsSize))
			{
				td_printf(0xff0000, "%s %s %d failed!", __FILE__, __FUNCTION__, __LINE__);
				iRet = td_failure;
				goto leave;
			}
			
			td_printf(0, "-------------------------------------------------------------->total pps num is %d", ix);
		}
			break;

		default:
			break;
	}

leave:

	//vps
	if(NULL != vidheader)
	{
		free(vidheader);
		vidheader = NULL;
	}

	if(NULL != vidheadersize)
	{
		free(vidheadersize);
		vidheadersize = NULL;
	}
	//sps
	if(NULL != seqheader)
	{
		free(seqheader);
		seqheader = NULL;
	}

	if(NULL != seqheadersize)
	{
		free(seqheadersize);
		seqheadersize = NULL;
	}
	//pps
	if(NULL != pictheader)
	{
		free(pictheader);
		pictheader = NULL;
	}

	if(NULL != pictheadersize)
	{
		free(pictheadersize);
		pictheadersize = NULL;
	}

	return iRet;
}

/************************************************* 
  Function: MP4_GetVpsSpsPps
  Description: »ñÈ¡mp4(h265±àÂë)ÎÄ¼þÖÐµÄÊÓÆµ²ÎÊý¼¯¡¢ÐòÁÐ²ÎÊý¼¯¡¢Í¼Ïñ²ÎÊý¼¯Êý¾Ý¼°´óÐ¡
  Input: _pcFileName£¬mp4ÎÄ¼þÃû
  Input: _pcVps£¬		ÊÓÆµ²ÎÊý¼¯buffer£¬·ÖÅäµÄ¿Õ¼ä×îºÃ´óÓÚ256B
  Input: _piVpsSize£¬	ÊÓÆµ²ÎÊý¼¯´óÐ¡
  Input: _pcSps£¬		ÐòÁÐ²ÎÊý¼¯buffer£¬·ÖÅäµÄ¿Õ¼ä×îºÃ´óÓÚ256B
  Input: _piSpsSize£¬	ÐòÁÐ²ÎÊý¼¯´óÐ¡
  Input: _pcPps£¬		Í¼Ïñ²ÎÊý¼¯buffer£¬·ÖÅäµÄ¿Õ¼ä×îºÃ´óÓÚ256B
  Input: _piPpsSize£¬	Í¼Ïñ²ÎÊý¼¯´óÐ¡
  Output: 
  Return: º¯ÊýÖ´ÐÐ³É¹¦·µ»Ø td_success, Ê§°Ü·µ»Ø td_failure
  Author: ³ÂÎÄÃô(chenwenmin)
  Others: 
*************************************************/
//cwm
int MP4_GetVpsSpsPps(char* _pcFileName, char* _pcVps, int* _piVpsSize, char* _pcSps, int* _piSpsSize, char* _pcPps, int* _piPpsSize)
{
	int iRet = td_success;
	MP4FileHandle pMP4fp = NULL;
	int iVpsFind = 0;
	int iSpsFind = 0;
	int iPpsFind = 0;
	uint8_t **vidheader = NULL;
	uint8_t **seqheader = NULL;
	uint8_t **pictheader = NULL;
	uint32_t *pictheadersize = NULL;
	uint32_t *seqheadersize = NULL;
	uint32_t *vidheadersize = NULL;
	uint32_t ix;

	if((NULL == _pcFileName)||(NULL == _pcVps)||(NULL == _piVpsSize)||(NULL == _pcSps)||(NULL == _piSpsSize)||(NULL == _pcPps)||(NULL == _piPpsSize))
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}

	char cFullPath[FULL_PATH_LEN] = {0};

	if(Inner_CompleteFileName(_pcFileName, cFullPath, FULL_PATH_LEN) < 0)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}
	
	pMP4fp  = MP4Read(cFullPath);

	if(NULL == pMP4fp)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}

	unsigned int uTrackId = MP4_INVALID_TRACK_ID;

	if(MP4_GetTrackId(cFullPath, &uTrackId, MP4_VIDEO_TRACK_TYPE) < 0)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}
	
	MP4GetTrackH265SeqPictHeaders(pMP4fp, uTrackId,
								  &vidheader, &vidheadersize,
								  &seqheader, &seqheadersize,
								  &pictheader, &pictheadersize);

	int iCount = 0;
		
	if((NULL != vidheader)&&(NULL != seqheader)&&(NULL != pictheader)
		&&(NULL != vidheadersize)&&(NULL != seqheadersize)&&(NULL != pictheadersize))
	{
		//vps
		for (ix = 0; vidheadersize[ix] != 0; ix++) 
		{
			if((vidheadersize[ix] < *_piVpsSize)&&(0 == iVpsFind))
			{
				memcpy(_pcVps, vidheader[ix], vidheadersize[ix]);
				*_piVpsSize = vidheadersize[ix];
				iVpsFind = 1;
			}
				
			for(iCount = 0; iCount < vidheadersize[ix]; iCount++)
			{
				td_printf(0, "the sps[%d][%d] is :%x", ix, iCount, vidheader[ix][iCount]);	
			}
		
			free(vidheader[ix]);
		}
		//sps
		for (ix = 0; seqheadersize[ix] != 0; ix++) 
		{
			if((seqheadersize[ix] < *_piSpsSize)&&(0 == iSpsFind))
			{
				memcpy(_pcSps, seqheader[ix], seqheadersize[ix]);
				*_piSpsSize = seqheadersize[ix];
				iSpsFind = 1;
			}
				
			for(iCount = 0; iCount < seqheadersize[ix]; iCount++)
			{
				td_printf(0, "the sps[%d][%d] is :%x", ix, iCount, seqheader[ix][iCount]);	
			}
		
			free(seqheader[ix]);
		}

		td_printf(0, "-------------------------------------------------------------->total sps num is %d", ix);
		//pps
		for (ix = 0; pictheadersize[ix] != 0; ix++) 
		{
			if((pictheadersize[ix] < *_piPpsSize)&&(0 == iPpsFind))
			{
				memcpy(_pcPps, pictheader[ix], pictheadersize[ix]);
				*_piPpsSize = pictheadersize[ix];
				iPpsFind = 1;
			}

			for(iCount = 0; iCount < pictheadersize[ix]; iCount++)
			{
				td_printf(0, "the pps[%d][%d] is :%x", ix, iCount, pictheader[ix][iCount]); 
			}
		
			free(pictheader[ix]);
		}

		if((0 == *_piSpsSize)||(0 == *_piPpsSize))
		{
			td_printf(0xff0000, "%s %s %d failed!", __FILE__, __FUNCTION__, __LINE__);
			iRet = td_failure;
			goto leave;
		}
		
		td_printf(0, "-------------------------------------------------------------->total pps num is %d", ix);
	}

leave:
	//vps
	if(NULL != vidheader)
	{
		free(vidheader);
		vidheader = NULL;
	}

	if(NULL != vidheadersize)
	{
		free(vidheadersize);
		vidheadersize = NULL;
	}
	//sps
	if(NULL != seqheader)
	{
		free(seqheader);
		seqheader = NULL;
	}

	if(NULL != seqheadersize)
	{
		free(seqheadersize);
		seqheadersize = NULL;
	}
	//pps
	if(NULL != pictheader)
	{
		free(pictheader);
		pictheader = NULL;
	}

	if(NULL != pictheadersize)
	{
		free(pictheadersize);
		pictheadersize = NULL;
	}

	if(NULL != pMP4fp)
	{
		MP4Close(pMP4fp, 0);
		pMP4fp = NULL;
	}

	return iRet;
}

/************************************************* 
  Function: MP4_GetVpsSpsPps
  Description: »ñÈ¡mp4(h265±àÂë)ÎÄ¼þÖÐµÄÊÓÆµ²ÎÊý¼¯¡¢ÐòÁÐ²ÎÊý¼¯¡¢Í¼Ïñ²ÎÊý¼¯Êý¾Ý¼°´óÐ¡
  Input: _pcFileName£¬mp4ÎÄ¼þÃû
  Input: _pcVps£¬		ÊÓÆµ²ÎÊý¼¯buffer£¬·ÖÅäµÄ¿Õ¼ä×îºÃ´óÓÚ256B
  Input: _piVpsSize£¬	ÊÓÆµ²ÎÊý¼¯´óÐ¡
  Input: _pcSps£¬		ÐòÁÐ²ÎÊý¼¯buffer£¬·ÖÅäµÄ¿Õ¼ä×îºÃ´óÓÚ256B
  Input: _piSpsSize£¬	ÐòÁÐ²ÎÊý¼¯´óÐ¡
  Input: _pcPps£¬		Í¼Ïñ²ÎÊý¼¯buffer£¬·ÖÅäµÄ¿Õ¼ä×îºÃ´óÓÚ256B
  Input: _piPpsSize£¬	Í¼Ïñ²ÎÊý¼¯´óÐ¡
  Output: 
  Return: º¯ÊýÖ´ÐÐ³É¹¦·µ»Ø td_success, Ê§°Ü·µ»Ø td_failure
  Author: ³ÂÎÄÃô(chenwenmin)
  Others: 
*************************************************/

//cwm
int MP4_GetVpsSpsPps_V2(void* _pMP4Handle, char* _pcVps, int* _piVpsSize, char* _pcSps, int* _piSpsSize, char* _pcPps, int* _piPpsSize)
{
	int iRet = td_success;
	int iVpsFind = 0;
	int iSpsFind = 0;
	int iPpsFind = 0;
	uint8_t **vidheader = NULL;
	uint8_t **seqheader = NULL;
	uint8_t **pictheader = NULL;
	uint32_t *pictheadersize = NULL;
	uint32_t *seqheadersize = NULL;
	uint32_t *vidheadersize = NULL;
	uint32_t ix;
	

	if((_pMP4Handle == NULL)||(NULL == _pcVps)||(NULL == _piVpsSize)||(NULL == _pcSps)||(NULL == _piSpsSize)||(NULL == _pcPps)||(NULL == _piPpsSize))
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}

	unsigned int uTrackId = MP4_INVALID_TRACK_ID;
	
	if(MP4_GetTrackId_V2(_pMP4Handle, &uTrackId, MP4_VIDEO_TRACK_TYPE) < 0)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}
	
	MP4GetTrackH265SeqPictHeaders(_pMP4Handle, uTrackId,
								  &vidheader, &vidheadersize,
								  &seqheader, &seqheadersize,
								  &pictheader, &pictheadersize);

	int iCount = 0;
		
	if((NULL != vidheader)&&(NULL != seqheader)&&(NULL != pictheader)
		&&(NULL != vidheadersize)&&(NULL != seqheadersize)&&(NULL != pictheadersize))
	{
		//vps
		for (ix = 0; vidheadersize[ix] != 0; ix++) 
		{
			if((vidheadersize[ix] < *_piVpsSize)&&(0 == iVpsFind))
			{
				memcpy(_pcVps, vidheader[ix], vidheadersize[ix]);
				*_piVpsSize = vidheadersize[ix];
				iVpsFind = 1;
			}
				
			for(iCount = 0; iCount < vidheadersize[ix]; iCount++)
			{
				td_printf(0, "the sps[%d][%d] is :%x", ix, iCount, vidheader[ix][iCount]);	
			}
		
			free(vidheader[ix]);
		}
		//sps
		for (ix = 0; seqheadersize[ix] != 0; ix++) 
		{
			if((seqheadersize[ix] < *_piSpsSize)&&(0 == iSpsFind))
			{
				memcpy(_pcSps, seqheader[ix], seqheadersize[ix]);
				*_piSpsSize = seqheadersize[ix];
				iSpsFind = 1;
			}
				
			for(iCount = 0; iCount < seqheadersize[ix]; iCount++)
			{
				td_printf(0, "the sps[%d][%d] is :%x", ix, iCount, seqheader[ix][iCount]);	
			}
		
			free(seqheader[ix]);
			seqheader[ix] = NULL;
		}

		td_printf(0, "-------------------------------------------------------------->total sps num is %d", ix);
		//pps
		for (ix = 0; pictheadersize[ix] != 0; ix++) 
		{
			if((pictheadersize[ix] < *_piPpsSize)&&(0 == iPpsFind))
			{
				memcpy(_pcPps, pictheader[ix], pictheadersize[ix]);
				*_piPpsSize = pictheadersize[ix];
				iPpsFind = 1;
			}

			for(iCount = 0; iCount < pictheadersize[ix]; iCount++)
			{
				td_printf(0, "the pps[%d][%d] is :%x", ix, iCount, pictheader[ix][iCount]); 
			}
		
			free(pictheader[ix]);
			pictheader[ix] = NULL;
		}

		if((0 == *_piSpsSize)||(0 == *_piPpsSize))
		{
			td_printf(0xff0000, "%s %s %d failed!", __FILE__, __FUNCTION__, __LINE__);
			iRet = td_failure;
			goto leave;
		}
		
		td_printf(0, "-------------------------------------------------------------->total pps num is %d", ix);
	}

leave:
	//vps
	if(NULL != vidheader)
	{
		free(vidheader);
		vidheader = NULL;
	}

	if(NULL != vidheadersize)
	{
		free(vidheadersize);
		vidheadersize = NULL;
	}
	//sps
	if(NULL != seqheader)
	{
		free(seqheader);
		seqheader = NULL;
	}

	if(NULL != seqheadersize)
	{
		free(seqheadersize);
		seqheadersize = NULL;
	}
	//pps
	if(NULL != pictheader)
	{
		free(pictheader);
		pictheader = NULL;
	}

	if(NULL != pictheadersize)
	{
		free(pictheadersize);
		pictheadersize = NULL;
	}

	return iRet;
}


int MP4_GetAudioType(char* _pcFileName, int* _piAudioType)
{
	td_printf(0, "%s start!", __FUNCTION__);
	td_s32 iRet = td_failure;
	char cFullPath[FULL_PATH_LEN] = {0};
	off_t OrgPos = 0;	
#ifdef TD_VFS
	TVfile* fp = NULL;	 
#else
	FILE* fp = NULL;		 
#endif

	if((NULL == _pcFileName)||(NULL == _piAudioType))
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		goto leave;
	}

	if(Inner_CompleteFileName(_pcFileName, cFullPath, FULL_PATH_LEN) < 0)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		goto leave;
	}

#ifdef TD_VFS
	fp = (TVfile*)td_mp4_fopen(NULL,cFullPath,"rb");
#else
	fp = (FILE*)td_mp4_fopen(NULL,cFullPath,"rb");
#endif

	if(NULL == fp)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		goto leave;
	}
	OrgPos = td_mp4_ftell(fp);
	td_mp4_fseek(fp, 0, SEEK_SET);
	if(Inner_GetInfoFromFirstFree(READFILE, (MP4_VOID*)fp, 0, (char*)AUDIO_ENCODE_TYPE, (unsigned int*)_piAudioType) < 0)
	{
		td_printf(0xff0000, "the file is may be a old mp4 file!");

		if(Inner_GetAudioTypeFromOldMP4File(cFullPath, _piAudioType) < 0)
		{
			td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
			goto leave;
		}
	}

	td_printf(0, "------------------->AudioType is %d", *_piAudioType);

	iRet = td_success;
leave:
	if(NULL != fp)
	{
		td_mp4_fseek(fp, OrgPos, SEEK_SET);
		td_mp4_fclose(fp);
		fp = NULL;
	}
	td_printf(0, "%s end!", __FUNCTION__);
    return iRet;
}

int MP4_GetMP4FileEncryptStat(char* _pcFileName, int* _piEncryptStat)
{
	td_printf(0, "%s start!", __FUNCTION__);
	td_s32 iRet = td_failure;
	char cFullPath[FULL_PATH_LEN] = {0};
	off_t OrgPos = 0;	
#ifdef TD_VFS
	TVfile* fp = NULL;	 
#else
	FILE* fp = NULL;		 
#endif

	if((NULL == _pcFileName)||(NULL == _piEncryptStat))
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		goto leave;
	}

	if(Inner_CompleteFileName(_pcFileName, cFullPath, FULL_PATH_LEN) < 0)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		goto leave;
	}

#ifdef TD_VFS
	fp = (TVfile*)td_mp4_fopen(NULL,cFullPath,"rb");
#else
	fp = (FILE*)td_mp4_fopen(NULL,cFullPath,"rb");
#endif

	if(NULL == fp)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		goto leave;
	}

	OrgPos = td_mp4_ftell(fp);
	
	
	td_mp4_fseek(fp, 0, SEEK_SET);

	if(Inner_GetInfoFromFirstFree(READFILE, (MP4_VOID*)fp, 0, (char*)ENCRYPTION_TYPE, (unsigned int*)_piEncryptStat) < 0)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		*_piEncryptStat = 0;//²éÕÒÊ§°ÜÄ¬ÈÏ²»¼ÓÃÜ
	}

	td_printf(0, "------------------->EncryptState is %d", *_piEncryptStat);
	iRet = td_success;
leave:
	if(NULL != fp)
	{
		td_mp4_fseek(fp, OrgPos, SEEK_SET);
		td_mp4_fclose(fp);
		fp = NULL;
	}
	td_printf(0, "%s end!", __FUNCTION__);
	return iRet;
}

int MP4_ReadSampleIndex(char* _pcFileName, 
								TMP4ISampleIndexHead *_pstMP4ISampleIndexHead, 
								TMP4ISampleIndexTail* _pstMP4ISampleIndexTail, 
								void ** _pIMP4SampleIndex, 
								int* _piIMP4SampleIndexmollocLen)
{
	td_printf(0, "---------->%s %d start!", __FUNCTION__, __LINE__);
	int iRet = td_success;
	MP4FileHandle pMP4fp  = NULL;
	uint8_t*  sample_vedio = NULL;

	if((NULL == _pcFileName)||(NULL == _pstMP4ISampleIndexHead)||
		 (NULL == _pstMP4ISampleIndexTail)||(NULL == _piIMP4SampleIndexmollocLen))
	{
		td_printf(0xff0000, "the %s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}

	char cFullPath[FULL_PATH_LEN] = {0};

	if(Inner_CompleteFileName(_pcFileName, cFullPath, FULL_PATH_LEN) < 0)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}
	
	pMP4fp  = MP4Read(cFullPath);
	td_printf(0, "---------->%s %d start!", __FUNCTION__, __LINE__);
	if(NULL == pMP4fp)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}

	unsigned int uTrackId = MP4_INVALID_TRACK_ID;

	if(MP4_GetTrackId(cFullPath, &uTrackId, MP4_VIDEO_TRACK_TYPE) < 0)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}

	MP4SampleId numSamples_vedio = MP4GetTrackNumberOfSamples(pMP4fp, uTrackId);
	MP4Duration sampleDuration_vedio = 0;
	bool isSyncSample_vedio = false;

	int iVSampleId = numSamples_vedio;//ÐéÄâsample¹Ì¶¨Îªµ¹ÊýµÚÒ»¸ösample
	td_printf(0x12345678, "FILE[%s]------------------------<<<<numSamples_vedio is %d, iMP4IndexSampleId is %d>>>>", cFullPath, numSamples_vedio, iVSampleId);

	uint32_t sample_size_vedio = MP4GetSampleSize(pMP4fp,uTrackId,iVSampleId);
	td_printf(0, "virframe size is %d", sample_size_vedio);
	sample_size_vedio += 1024;//¶à¿ª±ÙÒ»µã
	sample_vedio = (uint8_t *)malloc(sample_size_vedio*sizeof(uint8_t));
	
	if (NULL == sample_vedio)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		goto leave;
	}

	if(Inner_ReadOneVideoSample(pMP4fp,uTrackId,
		 iVSampleId, sample_vedio, 
		 &sample_size_vedio,&sampleDuration_vedio,&isSyncSample_vedio) < 0)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}
	td_printf(0, "---------->%s %d start!", __FUNCTION__, __LINE__);
	//sampleÍùºóÆ«ÒÆ(4 +1) ¸ö×Ö½Ú¡¾Ö¡´óÐ¡4  ¡¿¡¾Ö¡ÀàÐÍ1¡¿£¬ÎªË÷Òý¿ªÊ¼²¿Î»
	char* pIndexBeginPos = (char*)(sample_vedio + MP4_BOX_SIEZ_LENGTH + MP4_SAMPLE_TYPE_LENGTH);

	TMP4ISampleIndexHead* pstMP4ISampleIndexHead = NULL;
	TMP4ISampleIndexTail* pstMP4ISampleIndexTail = NULL;
	
	pstMP4ISampleIndexHead = (TMP4ISampleIndexHead*)(pIndexBeginPos);

	if(NULL == pstMP4ISampleIndexHead)
	{
		td_printf(0xff0000, "the %s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}

	if(pstMP4ISampleIndexHead->m_stSdvIndexHead.m_stHead.u32FrameID != 0x02000000)
	{
		td_printf(0xff0000, "the %s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}

	int iMP4ISampleNum = pstMP4ISampleIndexHead->m_stSdvIndexHead.m_iNum;
	int	iMP4ISampleInfoSize = pstMP4ISampleIndexHead->m_stSdvIndexHead.m_iInfoSize;
	int iMP4ISampleIndexLen = iMP4ISampleNum*iMP4ISampleInfoSize;
 
	if(iMP4ISampleNum > 0)
	{
		#ifdef ZHENGFA
		pstMP4ISampleIndexTail = (TMP4ISampleIndexTail*)(pIndexBeginPos + 
			sizeof(TMP4ISampleIndexHead) + iMP4ISampleIndexLen + iMP4ISampleInfoSize + MP4_APPEND_CHAR_INFO_SIZE);
		#else
		pstMP4ISampleIndexTail = (TMP4ISampleIndexTail*)(pIndexBeginPos + 
			sizeof(TMP4ISampleIndexHead) + iMP4ISampleIndexLen + MP4_APPEND_CHAR_INFO_SIZE);
		#endif
	}
	else
	{
		pstMP4ISampleIndexTail = (TMP4ISampleIndexTail*)(pIndexBeginPos + 
			sizeof(TMP4ISampleIndexHead) + MP4_APPEND_CHAR_INFO_SIZE);
	}

	if(NULL == pstMP4ISampleIndexTail)	
	{
		td_printf(0xff0000, "the %s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}

	if(pstMP4ISampleIndexTail->m_iFlag != REC_ISAMPLE_INDEX_MAGIC_NUMBER)
	{
		td_printf(0xff0000, "the %s %s %d error!m_iFlag is %x", __FILE__, __FUNCTION__, __LINE__, pstMP4ISampleIndexTail->m_iFlag);
		iRet = td_failure;
		goto leave;
	}

	memcpy(_pstMP4ISampleIndexHead, pstMP4ISampleIndexHead, sizeof(TMP4ISampleIndexHead));
	memcpy(_pstMP4ISampleIndexTail, pstMP4ISampleIndexTail, sizeof(TMP4ISampleIndexTail));

	if(*_piIMP4SampleIndexmollocLen < iMP4ISampleIndexLen)
	{
		if(*_pIMP4SampleIndex != NULL)
		{
			free(*_pIMP4SampleIndex);
			*_pIMP4SampleIndex = NULL;
			*_piIMP4SampleIndexmollocLen = 0;
		}
		*_pIMP4SampleIndex = malloc(iMP4ISampleIndexLen);//Íâ²¿º¯Êýµ÷ÓÃÊ±×îºóÐèÅÐ¶ÏÏÂÊÍ·Å¸ÃÄÚ´æ
		if(*_pIMP4SampleIndex == NULL)
		{
			td_printf(0xff0000, "the %s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
			iRet = td_failure;
			goto leave;
		}
		
		*_piIMP4SampleIndexmollocLen = iMP4ISampleIndexLen;
		td_printf(0," %s, %d  , malloc = %d",__func__,__LINE__, iMP4ISampleIndexLen);
	}
	
	if(NULL != *_pIMP4SampleIndex)
	{
		memcpy(*_pIMP4SampleIndex, (char*)(pIndexBeginPos + sizeof(TMP4ISampleIndexHead)), iMP4ISampleIndexLen);	
	}	
leave:
	
	if(NULL != pMP4fp)
	{
		MP4Close(pMP4fp, 0);
		pMP4fp = NULL;
	}
	if(NULL != sample_vedio)
	{
		free(sample_vedio);
		sample_vedio = NULL;
	}
	if(iRet == td_failure)
	{
		if(*_pIMP4SampleIndex != NULL)
		{
			free(*_pIMP4SampleIndex);
			*_pIMP4SampleIndex = NULL;
		}

		*_piIMP4SampleIndexmollocLen = 0;
	}
	td_printf(0, "---------->%s %d end!", __FUNCTION__, __LINE__);
	return iRet;
}

int MP4_ReadSampleIndex_V2(void* _pMP4Handle, 
									TMP4ISampleIndexHead *_pstMP4ISampleIndexHead, 
									TMP4ISampleIndexTail* _pstMP4ISampleIndexTail, 
									void ** _pIMP4SampleIndex, 
									int* _piIMP4SampleIndexmollocLen)
{
	td_printf(0, "---------->%s %d start!", __FUNCTION__, __LINE__);
	int iRet = td_success;
	uint8_t* sample_vedio = NULL;

	if((NULL == _pMP4Handle)||(NULL == _pstMP4ISampleIndexHead)||
		 (NULL == _pstMP4ISampleIndexTail)||(NULL == _piIMP4SampleIndexmollocLen))
	{
		td_printf(0xff0000, "the %s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}
	
	unsigned int uTrackId = MP4_INVALID_TRACK_ID;

	if(MP4_GetTrackId_V2(_pMP4Handle, &uTrackId, MP4_VIDEO_TRACK_TYPE) < 0)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}
	td_printf(0, "---------->%s %d start!", __FUNCTION__, __LINE__);
	MP4SampleId numSamples_vedio = MP4GetTrackNumberOfSamples(_pMP4Handle, uTrackId);	

	MP4Duration sampleDuration_vedio = 0;
	bool isSyncSample_vedio = false;

	int iVSampleId = numSamples_vedio;//ÐéÄâsample¹Ì¶¨Îªµ¹ÊýµÚÒ»¸ösample
	td_printf(0x12345678, "------------------------<<<<numSamples_vedio is %d, iMP4IndexSampleId is %d>>>>", numSamples_vedio, iVSampleId);
	uint32_t sample_size_vedio = MP4GetSampleSize(_pMP4Handle,uTrackId,iVSampleId);
	td_printf(0, "virframe size is %d", sample_size_vedio);
	sample_size_vedio += 1024;
	sample_vedio = (uint8_t *)malloc(sample_size_vedio*sizeof(uint8_t));
	
	if (NULL == sample_vedio)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		goto leave;
	}
	
	if(Inner_ReadOneVideoSample(_pMP4Handle,uTrackId,
		 iVSampleId, sample_vedio, 
		 &sample_size_vedio,&sampleDuration_vedio,&isSyncSample_vedio) < 0)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}
	td_printf(0, "---------->%s %d start!", __FUNCTION__, __LINE__);
	//sampleÍùºóÆ«ÒÆ(4 +1) ¸ö×Ö½Ú¡¾Ö¡´óÐ¡4  ¡¿¡¾Ö¡ÀàÐÍ1¡¿£¬ÎªË÷Òý¿ªÊ¼²¿Î»
	char* pIndexBeginPos = (char*)(sample_vedio + MP4_BOX_SIEZ_LENGTH + MP4_SAMPLE_TYPE_LENGTH);

	TMP4ISampleIndexHead* pstMP4ISampleIndexHead = NULL;
	TMP4ISampleIndexTail* pstMP4ISampleIndexTail = NULL;
	
	pstMP4ISampleIndexHead = (TMP4ISampleIndexHead*)(pIndexBeginPos);

	if(NULL == pstMP4ISampleIndexHead)
	{
		td_printf(0xff0000, "the %s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}

	if(pstMP4ISampleIndexHead->m_stSdvIndexHead.m_stHead.u32FrameID != 0x02000000)
	{
		PrintInfo("pstMP4ISampleIndexHead->m_stSdvIndexHead.m_stHead.u32FrameID != 0x02000000");
		td_printf(0xff0000, "the %s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}
	PrintInfo("pstMP4ISampleIndexHead->m_stSdvIndexHead.m_stHead.u32FrameID == 0x02000000");

	int iMP4ISampleNum = pstMP4ISampleIndexHead->m_stSdvIndexHead.m_iNum;
	int	iMP4ISampleInfoSize = pstMP4ISampleIndexHead->m_stSdvIndexHead.m_iInfoSize;
	int iMP4ISampleIndexLen = iMP4ISampleNum*iMP4ISampleInfoSize;
 
	if(iMP4ISampleNum > 0)
	{
		#ifdef ZHENGFA
		pstMP4ISampleIndexTail = (TMP4ISampleIndexTail*)(pIndexBeginPos + 
			sizeof(TMP4ISampleIndexHead) + iMP4ISampleIndexLen + iMP4ISampleInfoSize + MP4_APPEND_CHAR_INFO_SIZE);
		#else
		pstMP4ISampleIndexTail = (TMP4ISampleIndexTail*)(pIndexBeginPos + 
			sizeof(TMP4ISampleIndexHead) + iMP4ISampleIndexLen + MP4_APPEND_CHAR_INFO_SIZE);
		#endif
	}
	else
	{
		pstMP4ISampleIndexTail = (TMP4ISampleIndexTail*)(pIndexBeginPos + 
			sizeof(TMP4ISampleIndexHead) + MP4_APPEND_CHAR_INFO_SIZE);
	}

	if(NULL == pstMP4ISampleIndexTail)	
	{
		td_printf(0xff0000, "the %s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}

	PrintInfo("pstMP4ISampleIndexTail->m_iFlag=0x%x", pstMP4ISampleIndexTail->m_iFlag);
	if(pstMP4ISampleIndexTail->m_iFlag != REC_ISAMPLE_INDEX_MAGIC_NUMBER)
	{
		td_printf(0xff0000, "the %s %s %d error!m_iFlag is %x", __FILE__, __FUNCTION__, __LINE__, pstMP4ISampleIndexTail->m_iFlag);
		iRet = td_failure;
		goto leave;
	}

	memcpy(_pstMP4ISampleIndexHead, pstMP4ISampleIndexHead, sizeof(TMP4ISampleIndexHead));
	memcpy(_pstMP4ISampleIndexTail, pstMP4ISampleIndexTail, sizeof(TMP4ISampleIndexTail));

	if(*_piIMP4SampleIndexmollocLen < iMP4ISampleIndexLen)
	{
		if(*_pIMP4SampleIndex != NULL)
		{
			free(*_pIMP4SampleIndex);
			*_pIMP4SampleIndex = NULL;
			*_piIMP4SampleIndexmollocLen = 0;
		}
		*_pIMP4SampleIndex = malloc(iMP4ISampleIndexLen);//Íâ²¿º¯Êýµ÷ÓÃÊ±×îºóÐèÅÐ¶ÏÏÂÊÍ·Å¸ÃÄÚ´æ
		if(*_pIMP4SampleIndex == NULL)
		{
			td_printf(0xff0000, "the %s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
			iRet = td_failure;
			goto leave;
		}
		
		*_piIMP4SampleIndexmollocLen = iMP4ISampleIndexLen;
		td_printf(0," %s, %d  , malloc = %d",__func__,__LINE__, iMP4ISampleIndexLen);
	}
	
	if(NULL != *_pIMP4SampleIndex)
	{
		memcpy(*_pIMP4SampleIndex, (char*)(pIndexBeginPos + sizeof(TMP4ISampleIndexHead)), iMP4ISampleIndexLen);	
	}	
leave:
	
	if(iRet == td_failure)
	{
		if(*_pIMP4SampleIndex != NULL)
		{
			free(*_pIMP4SampleIndex);
			*_pIMP4SampleIndex = NULL;
		}

		*_piIMP4SampleIndexmollocLen = 0;
	}

	if (sample_vedio)
	{
		free(sample_vedio);
		sample_vedio = NULL;
	}
	td_printf(0, "---------->%s %d end!", __FUNCTION__, __LINE__);
	return iRet;
}

int MP4_ReadSampleIndex_V3(char* _pcFileName, 
									TMP4ISampleIndexHead *_pstMP4ISampleIndexHead, 
									TMP4ISampleIndexTail* _pstMP4ISampleIndexTail, 
									void ** _pIMP4SampleIndex, 
									int* _piIMP4SampleIndexmollocLen)
{
	td_printf(0, "---------->%s %d start!", __FUNCTION__, __LINE__);
	int iRet = td_failure;
	off_t OrgPos = 0;
#ifdef TD_VFS
	TVfile* pfFileHdl = NULL;
#else
	FILE*	pfFileHdl = NULL;
#endif
	char cFullPath[FULL_PATH_LEN] = {0};
	int iReadPos = 0;
	int iReadLen = 0;
	unsigned long long ullVirFramePos = 0;
	unsigned long long ullVirFrameLen = 0;
	void* pIMP4SampleIndex = NULL;

	if((NULL == _pcFileName)||(NULL == _pstMP4ISampleIndexHead)||
		 (NULL == _pstMP4ISampleIndexTail)||(NULL == _piIMP4SampleIndexmollocLen))
	{
		td_printf(0xff0000, "the %s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		goto leave;
	}

	if(Inner_CompleteFileName(_pcFileName, cFullPath, FULL_PATH_LEN) < 0)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		goto leave;
	}

	pfFileHdl = (FILE*)td_mp4_fopen(NULL, cFullPath, "rb");

	if(NULL == pfFileHdl)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		goto leave;
	}

	OrgPos   = td_mp4_ftell(pfFileHdl);
	
	iReadPos = -108;//ÐéÄâÖ¡¿ªÊ¼Î»ÖÃ
	
	if(td_mp4_fseek(pfFileHdl, iReadPos, SEEK_END) < 0)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		goto leave;
	}

	iReadLen = td_mp4_fread(&ullVirFramePos, 1, 8, pfFileHdl);

	if(iReadLen != 8)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		goto leave;
	}

	ConvertSortLong((unsigned char*)&ullVirFramePos, ullVirFramePos);

	if(td_mp4_fseek(pfFileHdl, ullVirFramePos, SEEK_SET) < 0)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		goto leave;
	}

	iReadLen = td_mp4_fread(&ullVirFrameLen, 1, MP4_BOX_SIEZ_LENGTH, pfFileHdl);

	if(iReadLen != MP4_BOX_SIEZ_LENGTH)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		goto leave;
	}

	ConvertSort((unsigned char*)&ullVirFrameLen, ullVirFrameLen);
	

	pIMP4SampleIndex = (char*)malloc(ullVirFrameLen);

	if(NULL == pIMP4SampleIndex)
	{
		td_printf(0xff0000, "%s %s %d error! ullVirFrameLen is %llu, because[%s]", __FILE__, __FUNCTION__, __LINE__, ullVirFrameLen, strerror(errno));
		goto leave;
	}

	iReadLen = td_mp4_fread(pIMP4SampleIndex, 1, ullVirFrameLen, pfFileHdl);

	if(iReadLen != ullVirFrameLen)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		goto leave;
	}

	td_printf(0, "---------->%s %d start!", __FUNCTION__, __LINE__);
	//sampleÍùºóÆ«ÒÆ1 ¸ö×Ö½Ú¡¾Ö¡ÀàÐÍ1¡¿£¬ÎªË÷Òý¿ªÊ¼²¿Î»
	char* pIndexBeginPos = (char*)pIMP4SampleIndex + MP4_SAMPLE_TYPE_LENGTH;

	TMP4ISampleIndexHead* pstMP4ISampleIndexHead = NULL;
	TMP4ISampleIndexTail* pstMP4ISampleIndexTail = NULL;
	
	pstMP4ISampleIndexHead = (TMP4ISampleIndexHead*)(pIndexBeginPos);

	if(NULL == pstMP4ISampleIndexHead)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		goto leave;
	}

	if(pstMP4ISampleIndexHead->m_stSdvIndexHead.m_stHead.u32FrameID != 0x02000000)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		goto leave;
	}

	int iMP4ISampleNum = pstMP4ISampleIndexHead->m_stSdvIndexHead.m_iNum;
	int	iMP4ISampleInfoSize = pstMP4ISampleIndexHead->m_stSdvIndexHead.m_iInfoSize;
	int iMP4ISampleIndexLen = iMP4ISampleNum*iMP4ISampleInfoSize;
 
	if(iMP4ISampleNum > 0)
	{
		#ifdef ZHENGFA
		pstMP4ISampleIndexTail = (TMP4ISampleIndexTail*)(pIndexBeginPos + 
			sizeof(TMP4ISampleIndexHead) + iMP4ISampleIndexLen + iMP4ISampleInfoSize + MP4_APPEND_CHAR_INFO_SIZE);
		#else
		pstMP4ISampleIndexTail = (TMP4ISampleIndexTail*)(pIndexBeginPos + 
			sizeof(TMP4ISampleIndexHead) + iMP4ISampleIndexLen + MP4_APPEND_CHAR_INFO_SIZE);
		#endif
	}
	else
	{
		pstMP4ISampleIndexTail = (TMP4ISampleIndexTail*)(pIndexBeginPos + 
			sizeof(TMP4ISampleIndexHead) + MP4_APPEND_CHAR_INFO_SIZE);
	}

	if(NULL == pstMP4ISampleIndexTail)	
	{
		td_printf(0xff0000, "the %s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}

	if(pstMP4ISampleIndexTail->m_iFlag != REC_ISAMPLE_INDEX_MAGIC_NUMBER)
	{
		td_printf(0xff0000, "the %s %s %d error!m_iFlag is %x", __FILE__, __FUNCTION__, __LINE__, pstMP4ISampleIndexTail->m_iFlag);
		iRet = td_failure;
		goto leave;
	}

	memcpy(_pstMP4ISampleIndexHead, pstMP4ISampleIndexHead, sizeof(TMP4ISampleIndexHead));
	memcpy(_pstMP4ISampleIndexTail, pstMP4ISampleIndexTail, sizeof(TMP4ISampleIndexTail));

	if(*_piIMP4SampleIndexmollocLen < iMP4ISampleIndexLen)
	{
		if(*_pIMP4SampleIndex != NULL)
		{
			free(*_pIMP4SampleIndex);
			*_pIMP4SampleIndex = NULL;
			*_piIMP4SampleIndexmollocLen = 0;
		}
		*_pIMP4SampleIndex = malloc(iMP4ISampleIndexLen);//Íâ²¿º¯Êýµ÷ÓÃÊ±×îºóÐèÅÐ¶ÏÏÂÊÍ·Å¸ÃÄÚ´æ
		if(*_pIMP4SampleIndex == NULL)
		{
			td_printf(0xff0000, "the %s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
			iRet = td_failure;
			goto leave;
		}
		
		*_piIMP4SampleIndexmollocLen = iMP4ISampleIndexLen;
		td_printf(0," %s, %d  , malloc = %d",__func__,__LINE__, iMP4ISampleIndexLen);
	}
	
	if(NULL != *_pIMP4SampleIndex)
	{
		memcpy(*_pIMP4SampleIndex, (char*)(pIndexBeginPos + sizeof(TMP4ISampleIndexHead)), iMP4ISampleIndexLen);	
	}

	iRet = td_success;
leave:
	
	if(iRet == td_failure)
	{
		if(*_pIMP4SampleIndex != NULL)
		{
			free(*_pIMP4SampleIndex);
			*_pIMP4SampleIndex = NULL;
		}

		*_piIMP4SampleIndexmollocLen = 0;
	}

	if(pfFileHdl)
	{
		td_mp4_fseek(pfFileHdl, OrgPos, SEEK_SET);
		td_mp4_fclose(pfFileHdl);
		pfFileHdl = NULL;
	}

	if (pIMP4SampleIndex)
	{
		free(pIMP4SampleIndex);
		pIMP4SampleIndex = NULL;
	}
	td_printf(0, "---------->%s %d end!", __FUNCTION__, __LINE__);
	return iRet;
}

#ifdef ZHENGFA
int MP4_GetFileCDSerialNo(char* _pcFileName, td_u8* _CDSerialNOBuf)
{
	int iRet = td_failure;
	unsigned int uiVersion = 0;
	MP4_VOID* pMP4ISampleIndex = NULL;
	int  iMP4ISampleIndexMallocLen = 0;
	TMP4ISampleIndexHead stMP4ISampleIndexHead;
	memset(&(stMP4ISampleIndexHead), 0, sizeof(TMP4ISampleIndexHead));
	TMP4ISampleIndexTail stMP4ISampleIndexTail = {0};
	
	if((NULL == _pcFileName)||(NULL == _CDSerialNOBuf))
	{
		td_printf(0xff0000, "the %s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		goto leave;
	}

	if(MP4_GetMP4FreeVerSion(_pcFileName, 2, &uiVersion) < 0)
	{
		td_printf(0xff0000,"%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
		goto leave;
	}

	if(1 == uiVersion)
	{
		if(MP4_ReadSampleIndex_V3(_pcFileName, &stMP4ISampleIndexHead, &stMP4ISampleIndexTail, (void**)&pMP4ISampleIndex, &iMP4ISampleIndexMallocLen) < 0)
		{
			td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
			goto leave;
		}
	}
	else
	{
		if(MP4_ReadSampleIndex(_pcFileName, &stMP4ISampleIndexHead, &stMP4ISampleIndexTail, (void**)&pMP4ISampleIndex, &iMP4ISampleIndexMallocLen) < 0)
		{
			td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
			goto leave;
		}
	}
	
	#if 0
	int iCount = 0;
	TMP4ISampleIndexInfo *IMP4SampleIndexInfo_temp = NULL;
	td_printf(0, "num is %d", stMP4ISampleIndexHead.m_stSdvIndexHead.m_iNum);
	int timelast = 0;
	for(iCount=0;iCount < stMP4ISampleIndexHead.m_stSdvIndexHead.m_iNum; iCount++)
	{
		IMP4SampleIndexInfo_temp = (TMP4ISampleIndexInfo *)((char *)(pMP4ISampleIndex) + iCount * stMP4ISampleIndexHead.m_stSdvIndexHead.m_iInfoSize);
		td_printf(0, "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%");
		td_printf(0, "IndexInfo[%d]->m_iTimeNow 		is %d", iCount, IMP4SampleIndexInfo_temp->m_stSdvIndexInfo.m_iTimeNow);
		td_printf(0, "IndexInfo[%d]->m_iSign 			is %d", iCount, IMP4SampleIndexInfo_temp->m_stSdvIndexInfo.m_iSign);
		td_printf(0, "IndexInfo[%d]->m_uiUselessCount	is %d", iCount, IMP4SampleIndexInfo_temp->m_uiUselessCount);
		td_printf(0, "IndexInfo[%d]->videosampleId 		is %d", iCount, IMP4SampleIndexInfo_temp->m_stVideoSampleIndexInfo.m_iSampleID);
		td_printf(0, "IndexInfo[%d]->audiosampleId 		is %d", iCount, IMP4SampleIndexInfo_temp->m_stAudioSampleIndexInfo.m_iSampleID);
		td_printf(0, "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%");

		if(iCount > 0)
		{
			if(IMP4SampleIndexInfo_temp->m_stSdvIndexInfo.m_iTimeNow != (timelast+1))
			{
				td_printf(0xff0000, "--------------------------------------->warning!!!");
			}
		}

		timelast = IMP4SampleIndexInfo_temp->m_stSdvIndexInfo.m_iTimeNow;
	}
	#endif

	//printfdataX("IFrameHeader:",(char*)&stMP4ISampleIndexHead,sizeof(TMP4ISampleIndexHead));
	//printfdataX("IFramebody:",(char*)pMP4ISampleIndex,iMP4ISampleIndexMallocLen);
	//printfdataX("IFrameTail:",(char*)&stMP4ISampleIndexTail,sizeof(TMP4ISampleIndexTail));

		
	//memcpy(_CDSerialNOBuf, stMP4ISampleIndexTail.m_cCDSerialNo, MAX_CDROMSERIALNO_LEN);

	if(stMP4ISampleIndexTail.m_iSize == (sizeof(TMP4ISampleIndexHead)+(stMP4ISampleIndexHead.m_stSdvIndexHead.m_iNum + 1) * stMP4ISampleIndexHead.m_stSdvIndexHead.m_iInfoSize
		+ MP4_APPEND_CHAR_INFO_SIZE + sizeof(TMP4ISampleIndexTail)))
	{
		TMP4ISampleIndexInfo* pMP4ISampleIndexInfo = (TMP4ISampleIndexInfo*)(pMP4ISampleIndex + stMP4ISampleIndexHead.m_stSdvIndexHead.m_iNum * stMP4ISampleIndexHead.m_stSdvIndexHead.m_iInfoSize);

		if((NULL != pMP4ISampleIndexInfo)&&(2 == pMP4ISampleIndexInfo->m_stSdvIndexInfo.m_iSign))
		{
			memcpy(_CDSerialNOBuf, pMP4ISampleIndexInfo->m_stSdvIndexInfo.m_cTagName, MAX_CDROMSERIALNO_LEN);
		}
	}
	else
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		td_printf(0, "---stMP4ISampleIndexTail.m_iSize[%d] size2 is %d", stMP4ISampleIndexTail.m_iSize,
			(sizeof(TMP4ISampleIndexHead)+(stMP4ISampleIndexHead.m_stSdvIndexHead.m_iNum + 1) * stMP4ISampleIndexHead.m_stSdvIndexHead.m_iInfoSize
		+ MP4_APPEND_CHAR_INFO_SIZE + sizeof(TMP4ISampleIndexTail)));
		goto leave;
	}

	iRet = td_success;
leave:	
	if(NULL != pMP4ISampleIndex)
	{
		free(pMP4ISampleIndex);
		pMP4ISampleIndex = NULL;
	}
	return iRet;
}
#endif

int MP4_GetFileEndTime(char* _pcFileName, time_t *_pFileEndTime)
{
	td_printf(0, "%s start!", __FUNCTION__);
	int	iRet = td_failure;
	void* pMP4ISampleIndex = NULL;
	int  iMP4ISampleIndexMallocLen = 0;
	unsigned int uiVersion = 0;
	TMP4ISampleIndexHead stMP4ISampleIndexHead;
	memset(&(stMP4ISampleIndexHead), 0, sizeof(TMP4ISampleIndexHead));
	TMP4ISampleIndexTail stMP4ISampleIndexTail = {0};

	if((NULL == _pcFileName)||(NULL == _pFileEndTime))
	{
		td_printf(0xff0000,"%s %s %d, _pcFileName = NULL\n", __FILE__, __FUNCTION__, __LINE__);
		goto leave;
	}	
		
	if(MP4_GetMP4FreeVerSion(_pcFileName, 2, &uiVersion) < 0)
	{
		td_printf(0xff0000,"%s %s %d, error!\n", __FILE__, __FUNCTION__, __LINE__);
		goto leave;
	}

	if(1 == uiVersion)
	{
		if(MP4_ReadSampleIndex_V3(_pcFileName, &stMP4ISampleIndexHead, &stMP4ISampleIndexTail, &pMP4ISampleIndex, &iMP4ISampleIndexMallocLen) < 0)
		{
			td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
			goto leave;
		}
	}
	else
	{
		if(MP4_ReadSampleIndex(_pcFileName, &stMP4ISampleIndexHead, &stMP4ISampleIndexTail, &pMP4ISampleIndex, &iMP4ISampleIndexMallocLen) < 0)
		{
			td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
			goto leave;
		}
	}

	*_pFileEndTime = stMP4ISampleIndexHead.m_stExRecInfo.m_iFileEndTime;

	iRet = td_success;
leave:
	if(NULL != pMP4ISampleIndex)
	{
		free(pMP4ISampleIndex);
		pMP4ISampleIndex = NULL;
	}
	td_printf(0, "%s end!", __FUNCTION__);
	return iRet;
}

int MP4_ReadSdvFileHeader(char* _pcFileName, S_FileHeader* _pstFileHeader)
{
	td_printf(0, "%s start!", __FUNCTION__);
	int	iRet = td_success;
	void* pMP4ISampleIndex = NULL;
	int  iMP4ISampleIndexMallocLen = 0;
	TMP4ISampleIndexHead stMP4ISampleIndexHead;
	memset(&(stMP4ISampleIndexHead), 0, sizeof(TMP4ISampleIndexHead));
	TMP4ISampleIndexTail stMP4ISampleIndexTail = {0};

	if((NULL == _pcFileName)||(NULL == _pstFileHeader))
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}

	if(MP4_ReadSampleIndex_V3(_pcFileName, &stMP4ISampleIndexHead, &stMP4ISampleIndexTail, &pMP4ISampleIndex, &iMP4ISampleIndexMallocLen) < 0)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}
	
	if(0/*IsValidFileHeader(&(stMP4ISampleIndexHead.m_stExRecInfo.m_stSdvFileHead)) < 0*/)//cwm_todo
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}
	else
	{
		memcpy(_pstFileHeader, &(stMP4ISampleIndexHead.m_stExRecInfo.m_stSdvFileHead), sizeof(S_FileHeader));
	}
	
leave:
	if(NULL != pMP4ISampleIndex)
	{
		free(pMP4ISampleIndex);
		pMP4ISampleIndex = NULL;
	}
	td_printf(0, "%s end!", __FUNCTION__);
	return iRet;
}

int MP4_ReadSdvFileHeader_V2(void* _pMP4Handle, S_FileHeader* _pstFileHeader)
{
    int	iRet = td_success;
	void* pMP4ISampleIndex = NULL;
	int  iMP4ISampleIndexMallocLen = 0;
	TMP4ISampleIndexHead stMP4ISampleIndexHead;
	memset(&(stMP4ISampleIndexHead), 0, sizeof(TMP4ISampleIndexHead));
	TMP4ISampleIndexTail stMP4ISampleIndexTail = {0};

	if((NULL == _pMP4Handle)||(NULL == _pstFileHeader))
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}

	if(MP4_ReadSampleIndex_V2(_pMP4Handle, &stMP4ISampleIndexHead, &stMP4ISampleIndexTail, &pMP4ISampleIndex, &iMP4ISampleIndexMallocLen) < 0)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}
	
	if(0/*IsValidFileHeader(&(stMP4ISampleIndexHead.m_stExRecInfo.m_stSdvFileHead)) < 0*/)//cwm_todo
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}
	else
	{
		memcpy(_pstFileHeader, &(stMP4ISampleIndexHead.m_stExRecInfo.m_stSdvFileHead), sizeof(S_FileHeader));
	}
	
leave:
	if(NULL != pMP4ISampleIndex)
	{
		free(pMP4ISampleIndex);
		pMP4ISampleIndex = NULL;
	}
	return iRet;
}


int MP4_ReadSign(char*_pcFileName, void* _pMarkInfo)
{
	td_printf(0, "---------->%s %d start!", __FUNCTION__, __LINE__);
	int i = 0;
	int iRet = td_success;
	unsigned int uiVersion = 0;
	MP4_TMark *pMarkinfo = (MP4_TMark *)_pMarkInfo;
	MP4_VOID *pMP4ISampleIndexBuf = NULL;
	MP4_VOID* pMP4ISampleIndex = NULL;
	int  iMP4ISampleIndexMallocLen = 0;
	TMP4ISampleIndexHead stMP4ISampleIndexHead;
	memset(&(stMP4ISampleIndexHead), 0, sizeof(TMP4ISampleIndexHead));
	TMP4ISampleIndexTail stMP4ISampleIndexTail = {0};
	
	if((NULL == _pcFileName)||(NULL == pMarkinfo))
	{
		td_printf(0xff0000, "the %s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}

	if(MP4_GetMP4FreeVerSion(_pcFileName, 2, &uiVersion) < 0)
	{
		td_printf(0xff0000,"%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}

	if(1 == uiVersion)
	{
		if(MP4_ReadSampleIndex_V3(_pcFileName, &stMP4ISampleIndexHead, &stMP4ISampleIndexTail, (void**)&pMP4ISampleIndex, &iMP4ISampleIndexMallocLen) < 0)
		{
			td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
			iRet = td_failure;
			goto leave;
		}
	}
	else
	{
		if(MP4_ReadSampleIndex(_pcFileName, &stMP4ISampleIndexHead, &stMP4ISampleIndexTail, (void**)&pMP4ISampleIndex, &iMP4ISampleIndexMallocLen) < 0)
		{
			td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
			iRet = td_failure;
			goto leave;
		}
	}
	
	td_printf(0, "---------->%s %d start!", __FUNCTION__, __LINE__);
	if(NULL == pMP4ISampleIndex)
	{
		td_printf(0xff0000, "the %s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}

	int iMP4ISampleNum = stMP4ISampleIndexHead.m_stSdvIndexHead.m_iNum;

	if(iMP4ISampleNum <= 0)
	{
		td_printf(0xff0000, "the %s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}
	
	int iMP4ISampleInfoSize = stMP4ISampleIndexHead.m_stSdvIndexHead.m_iInfoSize;
	
	if(NULL == (pMP4ISampleIndexBuf = (MP4_VOID*)malloc(iMP4ISampleInfoSize * 128)))
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}

	int iReadCnt = 0;
	int iHasReadCnt = 0;
	TIFrameIndexInfo *temp = NULL;
	td_printf(0, "---------->%s %d start!", __FUNCTION__, __LINE__);
	while(1)
	{
		iReadCnt = (iMP4ISampleNum - iHasReadCnt > 128) ? 128 : iMP4ISampleNum - iHasReadCnt;
		memcpy(pMP4ISampleIndexBuf, (char*)(pMP4ISampleIndex + iHasReadCnt*iMP4ISampleInfoSize), iReadCnt*iMP4ISampleInfoSize);	

		for(i = 0; i < iReadCnt; i++)
		{
			temp = (TIFrameIndexInfo *)(pMP4ISampleIndexBuf + i * iMP4ISampleInfoSize);

			if(NULL != temp)
			{
				if(1 == temp->m_iSign)
				{
					if(pMarkinfo->m_iHasCnt < pMarkinfo->m_iMaxCnt)
					{
						pMarkinfo->m_piMark[pMarkinfo->m_iHasCnt] = temp->m_iTimeNow;
					}					
					pMarkinfo->m_iHasCnt++;
				}				
			}
		}
		
		iHasReadCnt += iReadCnt;
		
		if(iHasReadCnt >= iMP4ISampleNum)
		{
			break;
		}
	}
	
leave:
	if(NULL != pMP4ISampleIndexBuf)
	{
		free(pMP4ISampleIndexBuf);
		pMP4ISampleIndexBuf = NULL;
	}
	if(NULL != pMP4ISampleIndex)
	{
		free(pMP4ISampleIndex);
		pMP4ISampleIndex = NULL;
	}
	td_printf(0, "---------->%s %d end!", __FUNCTION__, __LINE__);
	return iRet;
}

int MP4_ReadSign_V2(void* _pMP4Handle, void* _pMarkInfo)
{
	td_printf(0, "---------->%s %d start!", __FUNCTION__, __LINE__);
	int i = 0;
	int iRet = td_success;
	MP4_TMark *pMarkinfo = (MP4_TMark *)_pMarkInfo;
	MP4_VOID *pMP4ISampleIndexBuf = NULL;
	MP4_VOID* pMP4ISampleIndex = NULL;
	int  iMP4ISampleIndexMallocLen = 0;
	TMP4ISampleIndexHead stMP4ISampleIndexHead;
	memset(&(stMP4ISampleIndexHead), 0, sizeof(TMP4ISampleIndexHead));
	TMP4ISampleIndexTail stMP4ISampleIndexTail = {0};
	
	if((NULL == _pMP4Handle)||(NULL == pMarkinfo))
	{
		td_printf(0xff0000, "the %s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}

	if(MP4_ReadSampleIndex_V2(_pMP4Handle, &stMP4ISampleIndexHead, &stMP4ISampleIndexTail, (void**)&pMP4ISampleIndex, &iMP4ISampleIndexMallocLen) < 0)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}
	td_printf(0, "---------->%s %d start!", __FUNCTION__, __LINE__);
	if(NULL == pMP4ISampleIndex)
	{
		td_printf(0xff0000, "the %s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}

	int iMP4ISampleNum = stMP4ISampleIndexHead.m_stSdvIndexHead.m_iNum;

	if(iMP4ISampleNum <= 0)
	{
		td_printf(0xff0000, "the %s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}
	
	int iMP4ISampleInfoSize = stMP4ISampleIndexHead.m_stSdvIndexHead.m_iInfoSize;
	
	if(NULL == (pMP4ISampleIndexBuf = (MP4_VOID*)malloc(iMP4ISampleInfoSize * 128)))
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}

	int iReadCnt = 0;
	int iHasReadCnt = 0;
	TIFrameIndexInfo *temp = NULL;
	td_printf(0, "---------->%s %d start!", __FUNCTION__, __LINE__);
	while(1)
	{
		iReadCnt = (iMP4ISampleNum - iHasReadCnt > 128) ? 128 : iMP4ISampleNum - iHasReadCnt;
		memcpy(pMP4ISampleIndexBuf, (char*)(pMP4ISampleIndex + iHasReadCnt*iMP4ISampleInfoSize), iReadCnt*iMP4ISampleInfoSize);	

		for(i = 0; i < iReadCnt; i++)
		{
			temp = (TIFrameIndexInfo *)(pMP4ISampleIndexBuf + i * iMP4ISampleInfoSize);

			if(NULL != temp)
			{
				if(1 == temp->m_iSign)
				{
					if(pMarkinfo->m_iHasCnt < pMarkinfo->m_iMaxCnt)
					{
						pMarkinfo->m_piMark[pMarkinfo->m_iHasCnt] = temp->m_iTimeNow;
					}					
					pMarkinfo->m_iHasCnt++;
				}				
			}
		}
		
		iHasReadCnt += iReadCnt;
		
		if(iHasReadCnt >= iMP4ISampleNum)
		{
			break;
		}
	}
	
leave:
	if(NULL != pMP4ISampleIndexBuf)
	{
		free(pMP4ISampleIndexBuf);
		pMP4ISampleIndexBuf = NULL;
	}
	if(NULL != pMP4ISampleIndex)
	{
		free(pMP4ISampleIndex);
		pMP4ISampleIndex = NULL;
	}
	td_printf(0, "---------->%s %d end!", __FUNCTION__, __LINE__);
	return iRet;
}

int MP4_SeekFile(TMP4ISampleIndexHead *_pstMP4ISampleIndexHead, 
					TMP4ISampleIndexTail* _pstMP4ISampleIndexTail, 
					void * _pIMP4SampleIndex, 
					int _iPosOrTime, 
					unsigned int* _puiSeekVideoSampleId, 
					unsigned int* _puiSeeKAudioSampleId)
{
	td_s32 iRet = td_success;
	
	if((NULL == _pstMP4ISampleIndexHead)||(NULL == _pstMP4ISampleIndexTail)
		||(NULL == _pIMP4SampleIndex)||(NULL == _puiSeekVideoSampleId)||(NULL == _puiSeeKAudioSampleId))
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}

	if(_pstMP4ISampleIndexHead->m_stSdvIndexHead.m_iNum > 0)
	{
		time_t iSeekTime = 0;

		if (_iPosOrTime <= 100)
		{
			time_t tmpStartTime;
			time_t tmpStopTime;
			TMP4ISampleIndexInfo *ISampleIndexInfoFirst = NULL;			
			TMP4ISampleIndexInfo *ISampleIndexInfolast  = NULL;
			ISampleIndexInfoFirst = (TMP4ISampleIndexInfo *)((char *)_pIMP4SampleIndex);
			ISampleIndexInfolast  = (TMP4ISampleIndexInfo *)((char *)(_pIMP4SampleIndex) + _pstMP4ISampleIndexHead->m_stSdvIndexHead.m_iNum* _pstMP4ISampleIndexHead->m_stSdvIndexHead.m_iInfoSize);
			tmpStartTime = ISampleIndexInfoFirst->m_stSdvIndexInfo.m_iTimeNow;
			tmpStopTime  = ISampleIndexInfolast->m_stSdvIndexInfo.m_iTimeNow;
			iSeekTime = (tmpStopTime - tmpStartTime) * _iPosOrTime/ 100 + tmpStartTime;
			td_printf(0, "wwwwwwwwww%s, %d, start:%d, stop:%d", __func__, __LINE__, tmpStartTime,tmpStopTime);
		}
		else
		{
			iSeekTime = _iPosOrTime;
		}

		td_s32 iCount;
		td_s32 iIMP4SampleIndexNo = -1;
		TMP4ISampleIndexInfo *IMP4SampleIndexInfo_temp = NULL;
		TMP4ISampleIndexInfo *IMP4SampleIndexInfo_real = NULL;
	
		td_printf(0x12345678, "m_stSdvIndexHead.m_iNum^^^^^^^^^^^^^^^^^^%d", 
			_pstMP4ISampleIndexHead->m_stSdvIndexHead.m_iNum);
		
		#if 0
		for(iCount=0;iCount < _pstMP4ISampleIndexHead->m_stSdvIndexHead.m_iNum; iCount++)
		{
			IMP4SampleIndexInfo_temp = (TMP4ISampleIndexInfo *)((char *)(_pIMP4SampleIndex) + iCount * _pstMP4ISampleIndexHead->m_stSdvIndexHead.m_iInfoSize);
			td_printf(0, "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%");
			td_printf(0, "IndexInfo[%d]->m_iTimeNow is %d", iCount, IMP4SampleIndexInfo_temp->m_stSdvIndexInfo.m_iTimeNow);
			td_printf(0, "IndexInfo[%d]->videosampleId is %d", iCount, IMP4SampleIndexInfo_temp->m_stVideoSampleIndexInfo.m_iSampleID);
			td_printf(0, "IndexInfo[%d]->audiosampleId is %d", iCount, IMP4SampleIndexInfo_temp->m_stAudioSampleIndexInfo.m_iSampleID);
			td_printf(0, "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%");
		}
		#endif
	
		for(iCount=0; iCount < _pstMP4ISampleIndexHead->m_stSdvIndexHead.m_iNum; iCount++)
		{
			IMP4SampleIndexInfo_temp = (TMP4ISampleIndexInfo *)((char *)(_pIMP4SampleIndex) + iCount * _pstMP4ISampleIndexHead->m_stSdvIndexHead.m_iInfoSize);
	
			if (IMP4SampleIndexInfo_temp->m_stSdvIndexInfo.m_iTimeNow >= iSeekTime)
			{
				td_printf(0, "ppppppppppppppppp%s, %d, iTimeNow:%d", __func__, __LINE__, IMP4SampleIndexInfo_temp->m_stSdvIndexInfo.m_iTimeNow);
				iIMP4SampleIndexNo = IMP4SampleIndexInfo_temp->m_stSdvIndexInfo.m_iTimeNow == iSeekTime ? iCount: iCount-1;
				iIMP4SampleIndexNo = iIMP4SampleIndexNo <0 ?  0 : iIMP4SampleIndexNo;
				td_printf(0, "ttttttttttttttttt%s, %d, iISampleIndexNo:%d", __func__, __LINE__, iIMP4SampleIndexNo);
				break;
			}
		}
		
		if(-1 == iIMP4SampleIndexNo)
		{
			if(_pstMP4ISampleIndexHead->m_stExRecInfo.m_iFileEndTime >= iSeekTime)
			{
				iIMP4SampleIndexNo = _pstMP4ISampleIndexHead->m_stSdvIndexHead.m_iNum - 1;
				td_printf(0,"iMP4ISampleIndexNo = %d",_pstMP4ISampleIndexHead->m_stSdvIndexHead.m_iNum - 1);
			}
		}
		
		IMP4SampleIndexInfo_real = (TMP4ISampleIndexInfo *)((char *)(_pIMP4SampleIndex) + iIMP4SampleIndexNo * _pstMP4ISampleIndexHead->m_stSdvIndexHead.m_iInfoSize);
	
		if(IMP4SampleIndexInfo_real != NULL)
		{
			if(IMP4SampleIndexInfo_real->m_stVideoSampleIndexInfo.m_iSampleID <= 0)
			{
				td_printf(0xff0000, "the videosample is %d, force to 1", IMP4SampleIndexInfo_real->m_stVideoSampleIndexInfo.m_iSampleID);
				IMP4SampleIndexInfo_real->m_stVideoSampleIndexInfo.m_iSampleID = 1;
			}

			if(IMP4SampleIndexInfo_real->m_stAudioSampleIndexInfo.m_iSampleID <= 0)
			{
				td_printf(0xff0000, "the audiosample is %d, force to 1", IMP4SampleIndexInfo_real->m_stAudioSampleIndexInfo.m_iSampleID);
				IMP4SampleIndexInfo_real->m_stAudioSampleIndexInfo.m_iSampleID = 1;
			}
			
			*_puiSeekVideoSampleId = IMP4SampleIndexInfo_real->m_stVideoSampleIndexInfo.m_iSampleID;//±£´æ¶¨Î»µ½µÄµ±Ç°VideosampleID
			*_puiSeeKAudioSampleId = IMP4SampleIndexInfo_real->m_stAudioSampleIndexInfo.m_iSampleID;//±£´æ¶¨Î»µ½µÄµ±Ç°AudiosampleID
			td_printf(0x12345678, "seek the videosampleid is %d, the audiosampleid is %d", 
				*_puiSeekVideoSampleId, *_puiSeeKAudioSampleId);
			iRet = td_success;
		}
	}
	else
	{	
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}
	
leave:
	return iRet;
}

int MP4_SeekFile_V2(TMP4ISampleIndexHead *_pstMP4ISampleIndexHead, 
					TMP4ISampleIndexTail* _pstMP4ISampleIndexTail, 
					void * _pIMP4SampleIndex, 
					int _iPosOrTime, 
					unsigned int* _puiSeekVideoSampleId, 
					unsigned int* _puiSeeKAudioSampleId,
					unsigned int* _puiUselessCount)
{
	td_s32 iRet = td_success;
	
	if((NULL == _pstMP4ISampleIndexHead)||(NULL == _pstMP4ISampleIndexTail)
		||(NULL == _pIMP4SampleIndex)||(NULL == _puiSeekVideoSampleId)||(NULL == _puiSeeKAudioSampleId))
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}

	if(_pstMP4ISampleIndexHead->m_stSdvIndexHead.m_iNum > 0)
	{
		time_t iSeekTime = 0;

		if (_iPosOrTime <= 100)
		{
			time_t tmpStartTime;
			time_t tmpStopTime;
			TMP4ISampleIndexInfo *ISampleIndexInfoFirst = NULL;			
			TMP4ISampleIndexInfo *ISampleIndexInfolast  = NULL;
			ISampleIndexInfoFirst = (TMP4ISampleIndexInfo *)((char *)_pIMP4SampleIndex);
			ISampleIndexInfolast  = (TMP4ISampleIndexInfo *)((char *)(_pIMP4SampleIndex) + _pstMP4ISampleIndexHead->m_stSdvIndexHead.m_iNum* _pstMP4ISampleIndexHead->m_stSdvIndexHead.m_iInfoSize);
			tmpStartTime = ISampleIndexInfoFirst->m_stSdvIndexInfo.m_iTimeNow;
			tmpStopTime  = ISampleIndexInfolast->m_stSdvIndexInfo.m_iTimeNow;
			iSeekTime = (tmpStopTime - tmpStartTime) * _iPosOrTime/ 100 + tmpStartTime;
			td_printf(0, "wwwwwwwwww%s, %d, start:%d, stop:%d", __func__, __LINE__, tmpStartTime,tmpStopTime);
		}
		else
		{
			iSeekTime = _iPosOrTime;
		}

		td_s32 iCount;
		td_s32 iIMP4SampleIndexNo = -1;
		TMP4ISampleIndexInfo *IMP4SampleIndexInfo_temp = NULL;
		TMP4ISampleIndexInfo *IMP4SampleIndexInfo_real = NULL;
	
		td_printf(0x12345678, "m_stSdvIndexHead.m_iNum^^^^^^^^^^^^^^^^^^%d", 
			_pstMP4ISampleIndexHead->m_stSdvIndexHead.m_iNum);
		
		#if 0
		for(iCount=0;iCount < _pstMP4ISampleIndexHead->m_stSdvIndexHead.m_iNum; iCount++)
		{
			IMP4SampleIndexInfo_temp = (TMP4ISampleIndexInfo *)((char *)(_pIMP4SampleIndex) + iCount * _pstMP4ISampleIndexHead->m_stSdvIndexHead.m_iInfoSize);
			td_printf(0, "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%");
			td_printf(0, "IndexInfo[%d]->m_iTimeNow 		is %d", iCount, IMP4SampleIndexInfo_temp->m_stSdvIndexInfo.m_iTimeNow);
			td_printf(0, "IndexInfo[%d]->m_uiUselessCount 	is %d", iCount, IMP4SampleIndexInfo_temp->m_uiUselessCount);
			td_printf(0, "IndexInfo[%d]->videosampleId 		is %d", iCount, IMP4SampleIndexInfo_temp->m_stVideoSampleIndexInfo.m_iSampleID);
			td_printf(0, "IndexInfo[%d]->audiosampleId 		is %d", iCount, IMP4SampleIndexInfo_temp->m_stAudioSampleIndexInfo.m_iSampleID);
			td_printf(0, "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%");
		}
		#endif
	
		for(iCount=0; iCount < _pstMP4ISampleIndexHead->m_stSdvIndexHead.m_iNum; iCount++)
		{
			IMP4SampleIndexInfo_temp = (TMP4ISampleIndexInfo *)((char *)(_pIMP4SampleIndex) + iCount * _pstMP4ISampleIndexHead->m_stSdvIndexHead.m_iInfoSize);
	
			if (IMP4SampleIndexInfo_temp->m_stSdvIndexInfo.m_iTimeNow >= iSeekTime)
			{
				td_printf(0, "ppppppppppppppppp%s, %d, iTimeNow:%d", __func__, __LINE__, IMP4SampleIndexInfo_temp->m_stSdvIndexInfo.m_iTimeNow);
				iIMP4SampleIndexNo = IMP4SampleIndexInfo_temp->m_stSdvIndexInfo.m_iTimeNow == iSeekTime ? iCount: iCount-1;
				iIMP4SampleIndexNo = iIMP4SampleIndexNo <0 ?  0 : iIMP4SampleIndexNo;
				td_printf(0, "ttttttttttttttttt%s, %d, iISampleIndexNo:%d", __func__, __LINE__, iIMP4SampleIndexNo);
				break;
			}
		}
		
		if(-1 == iIMP4SampleIndexNo)
		{
			if(_pstMP4ISampleIndexHead->m_stExRecInfo.m_iFileEndTime >= iSeekTime)
			{
				iIMP4SampleIndexNo = _pstMP4ISampleIndexHead->m_stSdvIndexHead.m_iNum - 1;
				td_printf(0,"iMP4ISampleIndexNo = %d",_pstMP4ISampleIndexHead->m_stSdvIndexHead.m_iNum - 1);
			}
		}
		
		IMP4SampleIndexInfo_real = (TMP4ISampleIndexInfo *)((char *)(_pIMP4SampleIndex) + iIMP4SampleIndexNo * _pstMP4ISampleIndexHead->m_stSdvIndexHead.m_iInfoSize);
	
		if(IMP4SampleIndexInfo_real != NULL)
		{
			if(IMP4SampleIndexInfo_real->m_stVideoSampleIndexInfo.m_iSampleID <= 0)
			{
				td_printf(0xff0000, "the videosample is %d, force to 1", IMP4SampleIndexInfo_real->m_stVideoSampleIndexInfo.m_iSampleID);
				IMP4SampleIndexInfo_real->m_stVideoSampleIndexInfo.m_iSampleID = 1;
			}

			if(IMP4SampleIndexInfo_real->m_stAudioSampleIndexInfo.m_iSampleID <= 0)
			{
				td_printf(0xff0000, "the audiosample is %d, force to 1", IMP4SampleIndexInfo_real->m_stAudioSampleIndexInfo.m_iSampleID);
				IMP4SampleIndexInfo_real->m_stAudioSampleIndexInfo.m_iSampleID = 1;
			}
			
			*_puiUselessCount      = IMP4SampleIndexInfo_real->m_uiUselessCount;
			*_puiSeekVideoSampleId = IMP4SampleIndexInfo_real->m_stVideoSampleIndexInfo.m_iSampleID;//±£´æ¶¨Î»µ½µÄµ±Ç°VideosampleID
			*_puiSeeKAudioSampleId = IMP4SampleIndexInfo_real->m_stAudioSampleIndexInfo.m_iSampleID;//±£´æ¶¨Î»µ½µÄµ±Ç°AudiosampleID
			td_printf(0x12345678, "seek the videosampleid is %d, the audiosampleid is %d, UselessCount is %u", 
				*_puiSeekVideoSampleId, *_puiSeeKAudioSampleId, *_puiUselessCount);
			iRet = td_success;
		}
	}
	else
	{	
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}
	
leave:
	return iRet;
}

unsigned int MP4_GetSeekPosSampleId(MP4FileHandle _hFile, unsigned int _trackId, unsigned long long _when, int _wantSyncSample)
{	
	unsigned int uiSampleId = 0;
	
	if((NULL == _hFile)||(MP4_INVALID_TRACK_ID == _trackId))
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		goto EXIT;
	}

	uiSampleId = MP4GetSampleIdFromTime(_hFile, _trackId, _when, _wantSyncSample);
		
EXIT:
	return uiSampleId;
}

int MP4_OpenFile(void** _MP4fp, char* _pcFileName)
{
	td_printf(0, "%s start!", __FUNCTION__);
	td_s32 iRet = td_success;

	if((NULL == _MP4fp)||(NULL == _pcFileName))
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}

	char cFullPath[FULL_PATH_LEN] = {0};

	if(Inner_CompleteFileName(_pcFileName, cFullPath, FULL_PATH_LEN) < 0)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}
	
#ifdef CWM//FOR_MP4SDK cwm_todo
	//add for windows 
	char cOutbuf[1024] = {0};
	AnsiToUTF8(cFullPath,strlen(cFullPath)+1,cOutbuf,sizeof(cOutbuf));
	*_MP4fp = MP4Read(cOutbuf);
	//add end
#else
	*_MP4fp = MP4Read(cFullPath);
#endif

	if (NULL == *_MP4fp)
	{
		td_printf(0xff0000, "open file %s err!\n", cFullPath);
		iRet = td_failure;
		goto leave;
	}
	
leave:
	td_printf(0, "%s end!", __FUNCTION__);
	return iRet;
}

int MP4_CloseFile(void** _MP4fp)
{
	int iRet = td_success;

	if(NULL == *_MP4fp)
	{
		iRet = td_failure;
		goto leave;
	}
	
	MP4Close(*_MP4fp, 0);
	*_MP4fp = NULL;
leave:
	return iRet;
}

int MP4_GetOneTiandyFrame(void* _pMP4fp, 
									 unsigned int _u32VideoTrackId, 
									 unsigned int _u32AudioTrackId,
									 unsigned int* _puiCurVideoSampleId, 
									 unsigned int* _puiCurAudioSampleId, 
									 char* _pSps, 
									 int _iSpsSize, 
									 char* _pPps, 
									 int _iPpsSize,
									 void* _pTiandyFrameBuf, 
									 S_FrameHeader* _pstFrmaeHeader, 
									 int _iAudioType)
{
	int iRet = td_success;
	#ifdef FOR_MP4SDK 
	uint8_t* videoframe = NULL;
	#endif

	if((NULL == _pMP4fp)||(NULL == _puiCurVideoSampleId)||(NULL == _puiCurAudioSampleId)
		||(NULL == _pTiandyFrameBuf)||(NULL == _pstFrmaeHeader))
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}

	if(MP4_INVALID_TRACK_ID == _u32VideoTrackId)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}
										
	char nalu_header[] = {00,00,00,01};	
					
	uint64_t video_timestamp = 0;
	MP4SampleId numSamples_vedio = MP4GetTrackNumberOfSamples(_pMP4fp, _u32VideoTrackId);
	uint32_t sample_size_vedio = MAX_VIDEO_SAMPLE_SIZE;
#ifdef FOR_MP4SDK
	videoframe = (uint8_t*)malloc(MAX_VIDEO_SAMPLE_SIZE + MAX_ExInfo_SIZE);
	if (NULL == videoframe)
	{
		goto leave;
	}
#else
	uint8_t videoframe[MAX_VIDEO_SAMPLE_SIZE + MAX_ExInfo_SIZE];
#endif
	uint32_t videoframesize = 0;
	MP4Duration sampleDuration_vedio = 0;
	bool isSyncSample_vedio = false;
	
	//×î´óµ½MP4 ÐéÄâÖ¡Ö®Ç°Ò»Ö¡
	if((0 >= *_puiCurVideoSampleId)||(*_puiCurVideoSampleId >= (numSamples_vedio - 1)))
	{
		td_printf(0xff0000, "%s %s %d error! *_piCurVideoSampleId is %d, numSamples_vedio is %d", __FILE__, __FUNCTION__, __LINE__, *_puiCurVideoSampleId, numSamples_vedio);
		iRet = td_failure;
		goto leave;
	}

	unsigned int iSpsPpsSize = sizeof(nalu_header) + _iSpsSize + sizeof(nalu_header) + _iPpsSize;
	unsigned int u32VideoPos = 0;

	if((iSpsPpsSize + sizeof(nalu_header)) > MAX_ExInfo_SIZE)
	{
		td_printf(0xff0000, "%s %s %d error!u32Pos is %u", __FILE__, __FUNCTION__, __LINE__, _iSpsSize + sizeof(nalu_header));
		iRet = td_failure;
		goto leave;
	}

	if(Inner_ReadOneVideoSample(_pMP4fp,_u32VideoTrackId,*_puiCurVideoSampleId,
		videoframe+iSpsPpsSize+sizeof(nalu_header),&sample_size_vedio,&sampleDuration_vedio,&isSyncSample_vedio) < 0)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}

	if(true == isSyncSample_vedio)
	{
		u32VideoPos = 0;
		memcpy(videoframe, nalu_header, sizeof(nalu_header));
		u32VideoPos += sizeof(nalu_header);
		memcpy(videoframe + u32VideoPos, _pSps, _iSpsSize);
		u32VideoPos += _iSpsSize;
		memcpy(videoframe + u32VideoPos, nalu_header, sizeof(nalu_header));
		u32VideoPos += sizeof(nalu_header);
		memcpy(videoframe + u32VideoPos, _pPps, _iPpsSize);
		u32VideoPos += _iPpsSize;
		memcpy(videoframe + u32VideoPos, nalu_header, sizeof(nalu_header));
		u32VideoPos += sizeof(nalu_header);
		
		u32VideoPos += sizeof(nalu_header);//¶Á³öÀ´µÄÊý¾ÝÍ·ËÄ×Ö½ÚÎÞÐ§,ÔÙÆ«ÒÆ4×Ö½Ú
		videoframesize = iSpsPpsSize + sample_size_vedio;//ÒòÆ«ÒÆ4¸ö×Ö½Ú£¬¹Ê×Ü´óÐ¡ÒªËã×¼
	}
	else
	{
		u32VideoPos = iSpsPpsSize;
		memcpy(videoframe + u32VideoPos, nalu_header, sizeof(nalu_header));
		u32VideoPos += sizeof(nalu_header);
		
		u32VideoPos += sizeof(nalu_header);//¶Á³öÀ´µÄÊý¾ÝÍ·ËÄ×Ö½ÚÎÞÐ§,ÔÙÆ«ÒÆ4×Ö½Ú
		videoframesize = sample_size_vedio;//ÒòÆ«ÒÆ4¸ö×Ö½Ú£¬¹Ê×Ü´óÐ¡ÒªËã×¼
	}

	video_timestamp = (MP4GetSampleTime(_pMP4fp,_u32VideoTrackId,*_puiCurVideoSampleId)*1000/90000);
	(*_puiCurVideoSampleId)++;//ÊÓÆµÖ¡ºÅµÝÔö
	//td_printf(0, "------------>VideoSampleId is %d", *_puiCurVideoSampleId);
	
	uint8_t audioframe[MAX_AUDIO_FRAMENUM*MAX_AUDIO_SAMPLE_SIZE];
	int audioframebuflen = sizeof(audioframe);
	uint32_t audioframesize = 0;

	if(Inner_GetAudioSamples(_pMP4fp,video_timestamp,_u32AudioTrackId,
		_puiCurAudioSampleId,_iAudioType,audioframe,audioframebuflen,_pstFrmaeHeader,0) < 0)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		audioframesize = 0;
	}
	else
	{
		audioframesize = (_pstFrmaeHeader->u16AFrames)*(_pstFrmaeHeader->u16AFrameSize);
	}

	if(videoframesize > 0)
	{
		Inner_BuildFreamHeader(_pstFrmaeHeader, isSyncSample_vedio, *_puiCurVideoSampleId - 1, 
			video_timestamp, videoframesize, 0);

		if((videoframesize+audioframesize) < MAXBUFEX)
		{			
			int iBufPos = 0;
			
			if(true == isSyncSample_vedio)
			{
				memcpy((MP4_VOID*)_pTiandyFrameBuf + iBufPos, videoframe, iSpsPpsSize + sizeof(nalu_header));
				iBufPos += (iSpsPpsSize + sizeof(nalu_header));
				memcpy((MP4_VOID*)_pTiandyFrameBuf + iBufPos, videoframe + u32VideoPos, sample_size_vedio - sizeof(nalu_header));
				iBufPos += (sample_size_vedio - sizeof(nalu_header));
			}
			else
			{
				memcpy((MP4_VOID*)_pTiandyFrameBuf + iBufPos, videoframe + iSpsPpsSize, sizeof(nalu_header));
				iBufPos += sizeof(nalu_header);
				memcpy((MP4_VOID*)_pTiandyFrameBuf + iBufPos, videoframe + u32VideoPos, sample_size_vedio - sizeof(nalu_header));
				iBufPos += (sample_size_vedio - sizeof(nalu_header));
			}

			memcpy((MP4_VOID*)_pTiandyFrameBuf + iBufPos, audioframe, audioframesize);
		}
		else
		{
			td_printf(0xff0000, "the length has beyond limit!");
			iRet = td_failure;
			goto leave;
		}

		iRet = td_success;
	}	
	else
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}
leave:
#ifdef FOR_MP4SDK
	if (videoframe)
	{
		free(videoframe);
		videoframe = NULL;
	}
#endif
	return iRet;
}

int MP4_GetOneITiandyFrame(void* _pMP4fp, 
									 unsigned int _u32VideoTrackId, 
									 unsigned int _u32AudioTrackId,
									 unsigned int* _puiCurVideoSampleId, 
									 unsigned int* _puiCurAudioSampleId, 
									 char* _pSps, 
									 int _iSpsSize,
									 char* _pPps, 
									 int _iPpsSize,
									 void* _pTiandyFrameBuf, 
									 S_FrameHeader* _pstFrmaeHeader, 
									 int _iAudioType)
{	
	int iRet = td_success;

	if((NULL == _pMP4fp)||(NULL == _puiCurVideoSampleId)||(NULL == _puiCurAudioSampleId)
		||(NULL == _pTiandyFrameBuf)||(NULL == _pstFrmaeHeader))
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}

	if(MP4_INVALID_TRACK_ID == _u32VideoTrackId)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}
										
	char nalu_header[] = {00,00,00,01};	
					
	uint64_t video_timestamp = 0;
	MP4SampleId numSamples_vedio = MP4GetTrackNumberOfSamples(_pMP4fp, _u32VideoTrackId);
	uint32_t sample_size_vedio = MAX_VIDEO_SAMPLE_SIZE;
	uint8_t videoframe[MAX_VIDEO_SAMPLE_SIZE + MAX_ExInfo_SIZE];
	uint32_t videoframesize = 0;
	MP4Duration sampleDuration_vedio = 0;
	bool isSyncSample_vedio = false;
	
	//×î´óµ½MP4 ÐéÄâÖ¡Ö®Ç°Ò»Ö¡
	if((0 >= *_puiCurVideoSampleId)||(*_puiCurVideoSampleId >= (numSamples_vedio - 1)))
	{
		td_printf(0xff0000, "%s %s %d error! *_piCurVideoSampleId is %d", __FILE__, __FUNCTION__, __LINE__, *_puiCurVideoSampleId);
		iRet = td_failure;
		goto leave;
	}

	unsigned int iSpsPpsSize = sizeof(nalu_header) + _iSpsSize + sizeof(nalu_header) + _iPpsSize;
	unsigned int u32VideoPos = 0;

	if((iSpsPpsSize + sizeof(nalu_header)) > MAX_ExInfo_SIZE)
	{
		td_printf(0xff0000, "%s %s %d error!u32Pos is %u", __FILE__, __FUNCTION__, __LINE__, _iSpsSize + sizeof(nalu_header));
		iRet = td_failure;
		goto leave;
	}

	while(*_puiCurVideoSampleId < (numSamples_vedio - 1))
	{
		if(Inner_ReadOneVideoSample(_pMP4fp,_u32VideoTrackId,*_puiCurVideoSampleId,
			videoframe+iSpsPpsSize+sizeof(nalu_header),&sample_size_vedio,&sampleDuration_vedio,&isSyncSample_vedio) < 0)
		{
			td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
			iRet = td_failure;
			goto leave;
		}

		video_timestamp = (MP4GetSampleTime(_pMP4fp,_u32VideoTrackId,*_puiCurVideoSampleId)*1000/90000);

		if(true == isSyncSample_vedio)
		{
			u32VideoPos = 0;
			memcpy(videoframe, nalu_header, sizeof(nalu_header));
			u32VideoPos += sizeof(nalu_header);
			memcpy(videoframe + u32VideoPos, _pSps, _iSpsSize);
			u32VideoPos += _iSpsSize;
			memcpy(videoframe + u32VideoPos, nalu_header, sizeof(nalu_header));
			u32VideoPos += sizeof(nalu_header);
			memcpy(videoframe + u32VideoPos, _pPps, _iPpsSize);
			u32VideoPos += _iPpsSize;
			memcpy(videoframe + u32VideoPos, nalu_header, sizeof(nalu_header));
			u32VideoPos += sizeof(nalu_header);
			
			u32VideoPos += sizeof(nalu_header);//¶Á³öÀ´µÄÊý¾ÝÍ·ËÄ×Ö½ÚÎÞÐ§,ÔÙÆ«ÒÆ4×Ö½Ú
			videoframesize = iSpsPpsSize + sample_size_vedio;//ÒòÆ«ÒÆ4¸ö×Ö½Ú£¬¹Ê×Ü´óÐ¡ÒªËã×¼
			
			(*_puiCurVideoSampleId)++;//ÊÓÆµÖ¡ºÅµÝÔö
			//td_printf(0, "------------>VideoSampleId is %d", *_puiCurVideoSampleId);
			break;
		}
		
		(*_puiCurVideoSampleId)++;//ÊÓÆµÖ¡ºÅµÝÔö
		//td_printf(0, "------------>VideoSampleId is %d", *_puiCurVideoSampleId);
		sample_size_vedio = MAX_VIDEO_SAMPLE_SIZE;//´«Èë´«³ö²ÎÊý£¬Ã¿´ÎÑ­»·ÒªÖØÐÂ¸³Öµ£¬·ñÔò¿âÀï¿ÉÄÜ»á±¨bufferÐ¡µÄ´íÎó
	}

	if(0 >= videoframesize)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}
	
	uint8_t audioframe[MAX_AUDIO_FRAMENUM*MAX_AUDIO_SAMPLE_SIZE];
	int audioframebuflen = sizeof(audioframe);
	uint32_t audioframesize = 0;

	if(Inner_GetAudioSamples(_pMP4fp,video_timestamp,_u32AudioTrackId,
		_puiCurAudioSampleId,_iAudioType,audioframe,audioframebuflen,_pstFrmaeHeader,0) < 0)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		audioframesize = 0;
	}
	else
	{
		audioframesize = (_pstFrmaeHeader->u16AFrames)*(_pstFrmaeHeader->u16AFrameSize);
	}
						
	if(videoframesize > 0)
	{
		Inner_BuildFreamHeader(_pstFrmaeHeader, isSyncSample_vedio, *_puiCurVideoSampleId - 1, 
			video_timestamp, videoframesize, 0);

		if((videoframesize+audioframesize) < MAXBUFEX)
		{
			int iBufPos = 0;
			
			memcpy((MP4_VOID*)_pTiandyFrameBuf + iBufPos, videoframe, iSpsPpsSize + sizeof(nalu_header));
			iBufPos += (iSpsPpsSize + sizeof(nalu_header));
			memcpy((MP4_VOID*)_pTiandyFrameBuf + iBufPos, videoframe + u32VideoPos, sample_size_vedio - sizeof(nalu_header));
			iBufPos += (sample_size_vedio - sizeof(nalu_header));
			
			memcpy((MP4_VOID*)_pTiandyFrameBuf + iBufPos, audioframe, audioframesize);
		}
		else
		{
			td_printf(0xff0000, "the length has beyond limit!");
			iRet = td_failure;
			goto leave;
		}

		iRet = td_success;
	}	
	else
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}
leave:
	return iRet;
}

int MP4_GetOneTiandyFrame_V2(void* _pMP4fp, 
										 unsigned int _u32VideoTrackId, 
										 unsigned int _u32AudioTrackId,
										 unsigned int* _puiCurVideoSampleId, 
										 unsigned int* _puiCurAudioSampleId, 
										 char* _pSps, 
										 int _iSpsSize, 
										 char* _pPps, 
										 int _iPpsSize,
										 void* _pTiandyFrameBuf, 
										 S_FrameHeader* _pstFrmaeHeader, 
										 int _iAudioType, 
										 unsigned int* _puiUseLessIdCount, 
										 int _iNeedIFrame)
{
	int iRet = td_success;
	uint8_t* videoframe = NULL;
	char nalu_header[] = {00,00,00,01};	
	uint64_t video_timestamp = 0;
	MP4SampleId numSamples_vedio = 0;
	uint32_t sample_size_vedio = 0;
	uint32_t videoframesize = 0;
	MP4Duration sampleDuration_vedio = 0;
	bool isSyncSample_vedio = false;
	unsigned int uiSpsPpsSize = 0;
	unsigned int uiDataBufPos = 0;
	//unsigned int uiFrameBufPos = 0;
	int iSeiCount = 0;
	unsigned int uiFirstSeiTail = 0;

	//td_printf(0, "-------------->_iNeedIFrame is %d", _iNeedIFrame);

	if((NULL == _pMP4fp)||(NULL == _puiCurVideoSampleId)||(NULL == _puiCurAudioSampleId)
		||(NULL == _pTiandyFrameBuf)||(NULL == _pstFrmaeHeader))
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}

	if(MP4_INVALID_TRACK_ID == _u32VideoTrackId)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}
													
	numSamples_vedio  = MP4GetTrackNumberOfSamples(_pMP4fp, _u32VideoTrackId);
	
	//×î´óµ½MP4 ÐéÄâÖ¡Ö®Ç°Ò»Ö¡
	if((0 >= *_puiCurVideoSampleId)||(*_puiCurVideoSampleId >= (numSamples_vedio - 1)))
	{
		td_printf(0xff0000, "%s %s %d error! *_piCurVideoSampleId is %d, numSamples_vedio is %d", __FILE__, __FUNCTION__, __LINE__, *_puiCurVideoSampleId, numSamples_vedio);
		iRet = td_failure;
		goto leave;
	}

	videoframe = (uint8_t*)malloc(MAX_VIDEO_SAMPLE_SIZE);

	if (NULL == videoframe)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}

	while(*_puiCurVideoSampleId < (numSamples_vedio - 1))
	{
		sample_size_vedio = MP4GetSampleSize(_pMP4fp, _u32VideoTrackId, *_puiCurVideoSampleId);
		
		uiSpsPpsSize = sizeof(nalu_header) + _iSpsSize + sizeof(nalu_header) + _iPpsSize;
		uiDataBufPos += uiSpsPpsSize;
		
		if((sample_size_vedio + uiDataBufPos) > MAX_VIDEO_SAMPLE_SIZE)
		{
			td_printf(0xff0000, "%s %s %d error! sample_size_vedio is %d, uiDataBufPos is %d", 
				__FILE__, __FUNCTION__, __LINE__, sample_size_vedio, uiDataBufPos);
			iRet = td_failure;
			goto leave;
		}
		
		if(Inner_ReadOneVideoSample(_pMP4fp,_u32VideoTrackId,*_puiCurVideoSampleId,
			videoframe+uiDataBufPos,&sample_size_vedio,&sampleDuration_vedio,&isSyncSample_vedio) < 0)
		{
			td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
			iRet = td_failure;
			goto leave;
		}
		//cwm add Ìí¼ÓNAL_IDR_N_LPÖ§³Öh265
		while((SEI_RBSP == videoframe[uiDataBufPos+MP4_BOX_SIEZ_LENGTH]) || (NAL_IDR_N_LP == videoframe[uiDataBufPos+MP4_BOX_SIEZ_LENGTH]))
		{
			iSeiCount++;
			//²»¼ÆÊýÖ¡ÊýÀÛ¼Ó¼ÆÊý
			(*_puiUseLessIdCount)++;

			//×î´óµ½MP4 ÐéÄâÖ¡Ö®Ç°Ò»Ö¡
			if(*_puiCurVideoSampleId >= (numSamples_vedio - 1))
			{
				td_printf(0xff0000, "%s %s %d error! *_piCurVideoSampleId is %d, numSamples_vedio is %d", __FILE__, __FUNCTION__, __LINE__, *_puiCurVideoSampleId, numSamples_vedio);
				iRet = td_failure;
				goto leave;
			}

			//td_printf(0, "--------->This Sample SEI iCount is %d", iSeiCount);
			
			memcpy(videoframe+uiDataBufPos, nalu_header, sizeof(nalu_header));
			uiDataBufPos += sample_size_vedio;

			if(1 == iSeiCount)
			{
				uiFirstSeiTail = uiDataBufPos;
			}
			(*_puiCurVideoSampleId)++;//ÊÓÆµÖ¡ºÅµÝÔö
			sample_size_vedio = MP4GetSampleSize(_pMP4fp, _u32VideoTrackId, *_puiCurVideoSampleId);

			if((sample_size_vedio + uiDataBufPos) > MAX_VIDEO_SAMPLE_SIZE)
			{
				td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
				iRet = td_failure;
				goto leave;
			}

			if(Inner_ReadOneVideoSample(_pMP4fp,_u32VideoTrackId,*_puiCurVideoSampleId,
				videoframe+uiDataBufPos,&sample_size_vedio,&sampleDuration_vedio,&isSyncSample_vedio) < 0)
			{
				td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
				iRet = td_failure;
				goto leave;
			}
		}

		memcpy(videoframe+uiDataBufPos, nalu_header, sizeof(nalu_header));
		uiDataBufPos += sample_size_vedio;

		if(true == isSyncSample_vedio)
		{
			memcpy(videoframe, nalu_header, sizeof(nalu_header));
			memcpy(videoframe + sizeof(nalu_header), _pSps, _iSpsSize);
			memcpy(videoframe + sizeof(nalu_header) + _iSpsSize, nalu_header, sizeof(nalu_header));
			memcpy(videoframe + sizeof(nalu_header) + _iSpsSize + sizeof(nalu_header), _pPps, _iPpsSize);
			
			videoframesize = uiDataBufPos;
			break;
		}
		else
		{		
			if(1 == _iNeedIFrame)
			{
				uiDataBufPos = 0;
				(*_puiCurVideoSampleId)++;//ÊÓÆµÖ¡ºÅµÝÔö
				continue;
			}
			else
			{
				videoframesize = uiDataBufPos - uiSpsPpsSize;
				break;
			}
		}
	}

	video_timestamp = (MP4GetSampleTime(_pMP4fp,_u32VideoTrackId,*_puiCurVideoSampleId)*1000/90000);
	(*_puiCurVideoSampleId)++;//ÊÓÆµÖ¡ºÅµÝÔö
	//td_printf(0, "------------>VideoSampleId is %d", *_puiCurVideoSampleId);
	
	uint8_t audioframe[MAX_AUDIO_FRAMENUM*MAX_AUDIO_SAMPLE_SIZE];
	int audioframebuflen = sizeof(audioframe);
	uint32_t audioframesize = 0;

	if(Inner_GetAudioSamples(_pMP4fp,video_timestamp,_u32AudioTrackId,
		_puiCurAudioSampleId,_iAudioType,audioframe,audioframebuflen,_pstFrmaeHeader,0) < 0)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		audioframesize = 0;
	}
	else
	{
		audioframesize = (_pstFrmaeHeader->u16AFrames)*(_pstFrmaeHeader->u16AFrameSize);
	}

	if(videoframesize > 0)
	{
		Inner_BuildFreamHeader(_pstFrmaeHeader, isSyncSample_vedio, *_puiCurVideoSampleId - (*_puiUseLessIdCount + 1), 
			video_timestamp, videoframesize, 0);

		if((videoframesize+audioframesize) < MAXBUFEX)
		{						
			if(true == isSyncSample_vedio)
			{
				#if 0
				if(iSeiCount > 1)
				{
					memcpy((MP4_VOID*)_pTiandyFrameBuf+uiFrameBufPos, videoframe+uiSpsPpsSize, uiFirstSeiTail - uiSpsPpsSize);
					uiFrameBufPos += (uiFirstSeiTail - uiSpsPpsSize);
					memcpy((MP4_VOID*)_pTiandyFrameBuf+uiFrameBufPos, videoframe, uiSpsPpsSize);
					uiFrameBufPos += uiSpsPpsSize;
					memcpy((MP4_VOID*)_pTiandyFrameBuf+uiFrameBufPos, videoframe+uiFirstSeiTail, videoframesize-uiFirstSeiTail);
					uiFrameBufPos += videoframesize-uiFirstSeiTail;
				}
				else
				{
					memcpy((MP4_VOID*)_pTiandyFrameBuf, videoframe, videoframesize);
				}
				#else
				memcpy((MP4_VOID*)_pTiandyFrameBuf, videoframe, videoframesize);
				#endif
			}
			else
			{
				memcpy((MP4_VOID*)_pTiandyFrameBuf, videoframe + uiSpsPpsSize,videoframesize);
			}

			memcpy((MP4_VOID*)_pTiandyFrameBuf+videoframesize, audioframe, audioframesize);
		}
		else
		{
			td_printf(0xff0000, "the length has beyond limit!");
			iRet = td_failure;
			goto leave;
		}

		iRet = td_success;
	}	
	else
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}
leave:
	if (videoframe)
	{
		free(videoframe);
		videoframe = NULL;
	}
	return iRet;
}

td_s32 MP4_GetOneEncryptTiandyFrame(void* _pMP4fp,  
												 unsigned int _u32VideoTrackId, 
												 unsigned int _u32AudioTrackId,
												 unsigned int* _piCurVideoSampleId, 
												 unsigned int* _piCurAudioSampleId, 
												 void* _pTiandyFrameBuf, 
												 S_FrameHeader* _pstFrmaeHeader, 
												 int _iAudioType,
												 int _iNeedIFrame)
{
	int iRet = td_success;

	if((NULL == _pMP4fp)||(NULL == _piCurVideoSampleId)||(NULL == _piCurAudioSampleId)
		||(NULL == _pTiandyFrameBuf)||(NULL == _pstFrmaeHeader))
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}

	if(MP4_INVALID_TRACK_ID == _u32VideoTrackId)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}
															
	uint64_t video_timestamp = 0;
	MP4SampleId numSamples_vedio = MP4GetTrackNumberOfSamples(_pMP4fp, _u32VideoTrackId);
	uint32_t sample_size_vedio = MAX_VIDEO_SAMPLE_SIZE;
	uint8_t videoframe[MAX_VIDEO_SAMPLE_SIZE];
	uint32_t videoframesize = 0;
	MP4Duration sampleDuration_vedio = 0;
	bool isSyncSample_vedio = false;
	
	//×î´óµ½MP4 ÐéÄâÖ¡Ö®Ç°Ò»Ö¡
	if((0 >= *_piCurVideoSampleId)||(*_piCurVideoSampleId >= (numSamples_vedio - 1)))
	{
		td_printf(0xff0000, "%s %s %d error! *_piCurVideoSampleId is %d, numSamples_vedio is %d", __FILE__, __FUNCTION__, __LINE__, *_piCurVideoSampleId, numSamples_vedio);
		iRet = td_failure;
		goto leave;
	}

	while(*_piCurVideoSampleId < (numSamples_vedio - 1))
	{
		if(Inner_ReadOneVideoSample(_pMP4fp,_u32VideoTrackId,*_piCurVideoSampleId,
			videoframe,&sample_size_vedio,&sampleDuration_vedio,&isSyncSample_vedio) < 0)
		{
			td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
			iRet = td_failure;
			goto leave;
		}

		//¼õÈ¥Í·²¿µÄ4×Ö½Ú¼ÓÃÜÖ¡ºÅÊý¾Ý
		memcpy(&(_pstFrmaeHeader->u32FrameNO), videoframe+MP4_BOX_SIEZ_LENGTH, sizeof(_pstFrmaeHeader->u32FrameNO));		
		td_printf(0, "realdatalen is %d, the Frame No is %d", sample_size_vedio -MP4_BOX_SIEZ_LENGTH, _pstFrmaeHeader->u32FrameNO);
		videoframesize = sample_size_vedio -MP4_BOX_SIEZ_LENGTH - sizeof(_pstFrmaeHeader->u32FrameNO);
		
		video_timestamp = (MP4GetSampleTime(_pMP4fp,_u32VideoTrackId,*_piCurVideoSampleId)*1000/90000);

		if(1 == _iNeedIFrame)
		{
			if(true == isSyncSample_vedio)
			{
				td_printf(0, "------------>Find I Frame VideoSampleId is %d", *_piCurVideoSampleId);
				break;
			}
			else
			{
				//¼ÌÐøÕÒ				
				(*_piCurVideoSampleId)++;//ÊÓÆµÖ¡ºÅµÝÔö
				sample_size_vedio = MAX_VIDEO_SAMPLE_SIZE;//´«Èë´«³ö²ÎÊý£¬Ã¿´ÎÑ­»·ÒªÖØÐÂ¸³Öµ£¬·ñÔò¿âÀï¿ÉÄÜ»á±¨bufferÐ¡µÄ´íÎó
				continue;
			}
		}
		else
		{
			break;
		}
	}

	(*_piCurVideoSampleId)++;//ÊÓÆµÖ¡ºÅµÝÔö
	td_printf(0, "------------>VideoSampleId is %d", *_piCurVideoSampleId);
	
	uint8_t audioframe[MAX_AUDIO_FRAMENUM*MAX_AUDIO_SAMPLE_SIZE];
	int audioframebuflen = sizeof(audioframe);
	uint32_t audioframesize = 0;

	if(Inner_GetAudioSamples(_pMP4fp,video_timestamp,_u32AudioTrackId,
		_piCurAudioSampleId,_iAudioType,audioframe,audioframebuflen,_pstFrmaeHeader,1) < 0)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		audioframesize = 0;
	}
	else
	{
		audioframesize = (_pstFrmaeHeader->u16AFrames)*(_pstFrmaeHeader->u16AFrameSize);
	}

	if(videoframesize > 0)
	{
		Inner_BuildFreamHeader(_pstFrmaeHeader, isSyncSample_vedio, *_piCurVideoSampleId - 1, 
			video_timestamp, videoframesize, 1);
		
		//²»½âÃÜ
		#if 0
		if(((_pstFrmaeHeader->reserved & 0x03) == 0x02)&&(videoframesize >= 64))
		{
			#ifdef ZHENGFA
				unsigned char* pDataBuf = (unsigned char*)(videoframe+MP4_BOX_SIEZ_LENGTH+sizeof(_pstFrmaeHeader->u32FrameNO));
				aes_crypt((unsigned char*)pDataBuf,64,_pstFrmaeHeader->u32FrameNO,(unsigned char*)CRYPT_KEY_STR,strlen(CRYPT_KEY_STR));
				
				if( ((int *)pDataBuf)[0] != 0X1000000 )
				{
					td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
					iRet = MP4_CONVERT_ERROR_CRYPT_FAILED;
					goto leave;
				}

				_pstFrmaeHeader->reserved = 0xFFFF;
			#else
				td_printf(0xff0000, "%s %s %d error! the file has been encrypt! please check!", __FILE__, __FUNCTION__, __LINE__);
				iRet = MP4_CONVERT_ERROR_CRYPT_FAILED;
				goto leave;
			#endif
		}
		#endif

		if((videoframesize+audioframesize) < MAXBUFEX)
		{			
			int iBufPos = 0;
			memcpy((MP4_VOID*)_pTiandyFrameBuf + iBufPos, videoframe+MP4_BOX_SIEZ_LENGTH+sizeof(_pstFrmaeHeader->u32FrameNO), videoframesize);
			iBufPos += videoframesize;
			memcpy((MP4_VOID*)_pTiandyFrameBuf + iBufPos, audioframe, audioframesize);
		}
		else
		{
			td_printf(0xff0000, "the length has beyond limit!");
			iRet = td_failure;
			goto leave;
		}

		iRet = td_success;
	}	
	else
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}
leave:
	return iRet;
}


/************************************************* 
  Function: MP4_GetOneTiandyFrameH265
  Description: »ñÈ¡mp4(h265±àÂë)ÎÄ¼þµÄÊÓÆµÊý¾Ý
  Input: _pMP4fp£¬			mp4ÎÄ¼þ¾ä±ú
  Input:_u32VideoTrackId£¬	ÊÓÆÁµÄtrack id
  Input:_u32AudioTrackId£¬	ÒôÆµµÄtrack id
  Input Output:_puiCurVideoSampleId£¬ÊÓÆµµÄ²ÉÑùË÷Òý£¬¶ÔÓ¦mp4ÀàÐÍÎªstszµÄbox
  Input Output:_puiCurAudioSampleId£¬ÒôÆµµÄ²ÉÑùË÷Òý£¬¶ÔÓ¦mp4ÀàÐÍÎªstszµÄbox
  Input: _pcVps£¬		ÊÓÆµ²ÎÊý¼¯Êý¾Ý
  Input: _piVpsSize£¬	ÊÓÆµ²ÎÊý¼¯´óÐ¡
  Input: _pcSps£¬		ÐòÁÐ²ÎÊý¼¯Êý¾Ý
  Input: _piSpsSize£¬	ÐòÁÐ²ÎÊý¼¯´óÐ¡
  Input: _pcPps£¬		Í¼Ïñ²ÎÊý¼¯Êý¾Ý
  Input: _piPpsSize£¬	Í¼Ïñ²ÎÊý¼¯´óÐ¡
  Output: _pTiandyFrameH265Buf£¬µÃµ½h265µÄÒ»Ö¡Êý¾Ý£¬°üº¬¿ªÊ¼±êÖ¾0x00000001
  Output:_pstFrmaeHeader
  Input:_iAudioType£¬ÒôÆµ±à½âÂëÀàÐÍ
  Return: º¯ÊýÖ´ÐÐ³É¹¦·µ»Ø td_success, Ê§°Ü·µ»Ø td_failure
  Author: ³ÂÎÄÃô(chenwenmin)
  Others: 
*************************************************/

int MP4_GetOneTiandyFrameH265(void* _pMP4fp, 
									 unsigned int _u32VideoTrackId, 
									 unsigned int _u32AudioTrackId,
									 unsigned int* _puiCurVideoSampleId, 
									 unsigned int* _puiCurAudioSampleId,
									 char* _pVps, 
									 int _iVpsSize, 
									 char* _pSps, 
									 int _iSpsSize, 
									 char* _pPps, 
									 int _iPpsSize,
									 void* _pTiandyFrameH265Buf, 
									 S_FrameHeader* _pstFrmaeHeader, 
									 int _iAudioType)
{
	int iRet = td_success;
	#ifdef FOR_MP4SDK 
	uint8_t* videoframe = NULL;
	#endif

	if((NULL == _pMP4fp)||(NULL == _puiCurVideoSampleId)||(NULL == _puiCurAudioSampleId)
		||(NULL == _pTiandyFrameH265Buf)||(NULL == _pstFrmaeHeader))
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}

	if(MP4_INVALID_TRACK_ID == _u32VideoTrackId)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}
										
	char nalu_header[] = {00,00,00,01};	
					
	uint64_t video_timestamp = 0;
	MP4SampleId numSamples_vedio = MP4GetTrackNumberOfSamples(_pMP4fp, _u32VideoTrackId);
	uint32_t sample_size_vedio = MAX_VIDEO_SAMPLE_SIZE;
#ifdef FOR_MP4SDK
	videoframe = (uint8_t*)malloc(MAX_VIDEO_SAMPLE_SIZE + MAX_ExInfo_SIZE);
	if (NULL == videoframe)
	{
		goto leave;
	}
#else
	uint8_t videoframe[MAX_VIDEO_SAMPLE_SIZE + MAX_ExInfo_SIZE];
#endif
	uint32_t videoframesize = 0;
	MP4Duration sampleDuration_vedio = 0;
	bool isSyncSample_vedio = false;
	
	//×î´óµ½MP4 ÐéÄâÖ¡Ö®Ç°Ò»Ö¡
	if((0 >= *_puiCurVideoSampleId)||(*_puiCurVideoSampleId >= (numSamples_vedio - 1)))
	{
		td_printf(0xff0000, "%s %s %d error! *_piCurVideoSampleId is %d, numSamples_vedio is %d", __FILE__, __FUNCTION__, __LINE__, *_puiCurVideoSampleId, numSamples_vedio);
		iRet = td_failure;
		goto leave;
	}

	unsigned int iSpsPpsSize = sizeof(nalu_header) + _iVpsSize + sizeof(nalu_header) + _iSpsSize + sizeof(nalu_header) + _iPpsSize;
	unsigned int u32VideoPos = 0;

	if((iSpsPpsSize + sizeof(nalu_header)) > MAX_ExInfo_SIZE)
	{
		td_printf(0xff0000, "%s %s %d error!u32Pos is %u", __FILE__, __FUNCTION__, __LINE__, _iSpsSize + sizeof(nalu_header));
		iRet = td_failure;
		goto leave;
	}

	if(Inner_ReadOneVideoSample(_pMP4fp,_u32VideoTrackId,*_puiCurVideoSampleId,
		videoframe+iSpsPpsSize+sizeof(nalu_header),&sample_size_vedio,&sampleDuration_vedio,&isSyncSample_vedio) < 0)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}

	if(true == isSyncSample_vedio)
	{
		u32VideoPos = 0;
		memcpy(videoframe, nalu_header, sizeof(nalu_header));
		u32VideoPos += sizeof(nalu_header);
		memcpy(videoframe + u32VideoPos, _pVps, _iVpsSize);
		u32VideoPos += _iVpsSize;
		memcpy(videoframe, nalu_header, sizeof(nalu_header));
		u32VideoPos += sizeof(nalu_header);
		memcpy(videoframe + u32VideoPos, _pSps, _iSpsSize);
		u32VideoPos += _iSpsSize;
		memcpy(videoframe + u32VideoPos, nalu_header, sizeof(nalu_header));
		u32VideoPos += sizeof(nalu_header);
		memcpy(videoframe + u32VideoPos, _pPps, _iPpsSize);
		u32VideoPos += _iPpsSize;
		memcpy(videoframe + u32VideoPos, nalu_header, sizeof(nalu_header));
		u32VideoPos += sizeof(nalu_header);
		
		u32VideoPos += sizeof(nalu_header);//¶Á³öÀ´µÄÊý¾ÝÍ·ËÄ×Ö½ÚÎÞÐ§,ÔÙÆ«ÒÆ4×Ö½Ú
		videoframesize = iSpsPpsSize + sample_size_vedio;//ÒòÆ«ÒÆ4¸ö×Ö½Ú£¬¹Ê×Ü´óÐ¡ÒªËã×¼
	}
	else
	{
		u32VideoPos = iSpsPpsSize;
		memcpy(videoframe + u32VideoPos, nalu_header, sizeof(nalu_header));
		u32VideoPos += sizeof(nalu_header);
		
		u32VideoPos += sizeof(nalu_header);//¶Á³öÀ´µÄÊý¾ÝÍ·ËÄ×Ö½ÚÎÞÐ§,ÔÙÆ«ÒÆ4×Ö½Ú
		videoframesize = sample_size_vedio;//ÒòÆ«ÒÆ4¸ö×Ö½Ú£¬¹Ê×Ü´óÐ¡ÒªËã×¼
	}

	video_timestamp = (MP4GetSampleTime(_pMP4fp,_u32VideoTrackId,*_puiCurVideoSampleId)*1000/90000);
	(*_puiCurVideoSampleId)++;//ÊÓÆµÖ¡ºÅµÝÔö
	//td_printf(0, "------------>VideoSampleId is %d", *_puiCurVideoSampleId);
	
	uint8_t audioframe[MAX_AUDIO_FRAMENUM*MAX_AUDIO_SAMPLE_SIZE];
	int audioframebuflen = sizeof(audioframe);
	uint32_t audioframesize = 0;

	if(Inner_GetAudioSamples(_pMP4fp,video_timestamp,_u32AudioTrackId,
		_puiCurAudioSampleId,_iAudioType,audioframe,audioframebuflen,_pstFrmaeHeader,0) < 0)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		audioframesize = 0;
	}
	else
	{
		audioframesize = (_pstFrmaeHeader->u16AFrames)*(_pstFrmaeHeader->u16AFrameSize);
	}

	if(videoframesize > 0)
	{
		Inner_BuildFreamHeader(_pstFrmaeHeader, isSyncSample_vedio, *_puiCurVideoSampleId - 1, 
			video_timestamp, videoframesize, 0);

		if((videoframesize+audioframesize) < MAXBUFEX)
		{			
			int iBufPos = 0;
			
			if(true == isSyncSample_vedio)
			{
				memcpy((MP4_VOID*)_pTiandyFrameH265Buf + iBufPos, videoframe, iSpsPpsSize + sizeof(nalu_header));
				iBufPos += (iSpsPpsSize + sizeof(nalu_header));
				memcpy((MP4_VOID*)_pTiandyFrameH265Buf + iBufPos, videoframe + u32VideoPos, sample_size_vedio - sizeof(nalu_header));
				iBufPos += (sample_size_vedio - sizeof(nalu_header));
			}
			else
			{
				memcpy((MP4_VOID*)_pTiandyFrameH265Buf + iBufPos, videoframe + iSpsPpsSize, sizeof(nalu_header));
				iBufPos += sizeof(nalu_header);
				memcpy((MP4_VOID*)_pTiandyFrameH265Buf + iBufPos, videoframe + u32VideoPos, sample_size_vedio - sizeof(nalu_header));
				iBufPos += (sample_size_vedio - sizeof(nalu_header));
			}

			memcpy((MP4_VOID*)_pTiandyFrameH265Buf + iBufPos, audioframe, audioframesize);
		}
		else
		{
			td_printf(0xff0000, "the length has beyond limit!");
			iRet = td_failure;
			goto leave;
		}

		iRet = td_success;
	}	
	else
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}
leave:
#ifdef FOR_MP4SDK
	if (videoframe)
	{
		free(videoframe);
		videoframe = NULL;
	}
#endif
	return iRet;
}

/************************************************* 
  Function: MP4_GetOneTiandyFrameH265
  Description: »ñÈ¡mp4(h265±àÂë)ÎÄ¼þµÄÊÓÆµÊý¾Ý
  Input: _pMP4fp£¬			mp4ÎÄ¼þ¾ä±ú
  Input:_u32VideoTrackId£¬	ÊÓÆÁµÄtrack id
  Input:_u32AudioTrackId£¬	ÒôÆµµÄtrack id
  Input Output:_puiCurVideoSampleId£¬ÊÓÆµµÄ²ÉÑùË÷Òý£¬¶ÔÓ¦mp4ÀàÐÍÎªstszµÄbox
  Input Output:_puiCurAudioSampleId£¬ÒôÆµµÄ²ÉÑùË÷Òý£¬¶ÔÓ¦mp4ÀàÐÍÎªstszµÄbox
  Input: _pcVps£¬		ÊÓÆµ²ÎÊý¼¯Êý¾Ý
  Input: _piVpsSize£¬	ÊÓÆµ²ÎÊý¼¯´óÐ¡
  Input: _pcSps£¬		ÐòÁÐ²ÎÊý¼¯Êý¾Ý
  Input: _piSpsSize£¬	ÐòÁÐ²ÎÊý¼¯´óÐ¡
  Input: _pcPps£¬		Í¼Ïñ²ÎÊý¼¯Êý¾Ý
  Input: _piPpsSize£¬	Í¼Ïñ²ÎÊý¼¯´óÐ¡
  Output: _pTiandyFrameH265Buf£¬µÃµ½h265µÄÒ»Ö¡Êý¾Ý£¬°üº¬¿ªÊ¼±êÖ¾0x00000001
  Output:_pstFrmaeHeader
  Input:_iAudioType£¬ÒôÆµ±à½âÂëÀàÐÍ
  Return: º¯ÊýÖ´ÐÐ³É¹¦·µ»Ø td_success, Ê§°Ü·µ»Ø td_failure
  Author: ³ÂÎÄÃô(chenwenmin)
  Others: 
*************************************************/

int MP4_GetOneITiandyFrameH265(void* _pMP4fp, 
									 unsigned int _u32VideoTrackId, 
									 unsigned int _u32AudioTrackId,
									 unsigned int* _puiCurVideoSampleId, 
									 unsigned int* _puiCurAudioSampleId,
									 char* _pVps, 
									 int _iVpsSize,
									 char* _pSps, 
									 int _iSpsSize,
									 char* _pPps, 
									 int _iPpsSize,
									 void* _pTiandyFrameH265Buf, 
									 S_FrameHeader* _pstFrmaeHeader, 
									 int _iAudioType)
{	
	int iRet = td_success;

	if((NULL == _pMP4fp)||(NULL == _puiCurVideoSampleId)||(NULL == _puiCurAudioSampleId)
		||(NULL == _pTiandyFrameH265Buf)||(NULL == _pstFrmaeHeader))
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}

	if(MP4_INVALID_TRACK_ID == _u32VideoTrackId)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}
										
	char nalu_header[] = {00,00,00,01};	
					
	uint64_t video_timestamp = 0;
	MP4SampleId numSamples_vedio = MP4GetTrackNumberOfSamples(_pMP4fp, _u32VideoTrackId);
	uint32_t sample_size_vedio = MAX_VIDEO_SAMPLE_SIZE;
	uint8_t videoframe[MAX_VIDEO_SAMPLE_SIZE + MAX_ExInfo_SIZE];
	uint32_t videoframesize = 0;
	MP4Duration sampleDuration_vedio = 0;
	bool isSyncSample_vedio = false;
	
	//×î´óµ½MP4 ÐéÄâÖ¡Ö®Ç°Ò»Ö¡
	if((0 >= *_puiCurVideoSampleId)||(*_puiCurVideoSampleId >= (numSamples_vedio - 1)))
	{
		td_printf(0xff0000, "%s %s %d error! *_piCurVideoSampleId is %d", __FILE__, __FUNCTION__, __LINE__, *_puiCurVideoSampleId);
		iRet = td_failure;
		goto leave;
	}

	unsigned int iSpsPpsSize = sizeof(nalu_header) + _iVpsSize + sizeof(nalu_header) + _iSpsSize + sizeof(nalu_header) + _iPpsSize;
	unsigned int u32VideoPos = 0;

	if((iSpsPpsSize + sizeof(nalu_header)) > MAX_ExInfo_SIZE)
	{
		td_printf(0xff0000, "%s %s %d error!u32Pos is %u", __FILE__, __FUNCTION__, __LINE__, _iSpsSize + sizeof(nalu_header));
		iRet = td_failure;
		goto leave;
	}

	while(*_puiCurVideoSampleId < (numSamples_vedio - 1))
	{
		if(Inner_ReadOneVideoSample(_pMP4fp,_u32VideoTrackId,*_puiCurVideoSampleId,
			videoframe+iSpsPpsSize+sizeof(nalu_header),&sample_size_vedio,&sampleDuration_vedio,&isSyncSample_vedio) < 0)
		{
			td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
			iRet = td_failure;
			goto leave;
		}

		video_timestamp = (MP4GetSampleTime(_pMP4fp,_u32VideoTrackId,*_puiCurVideoSampleId)*1000/90000);

		if(true == isSyncSample_vedio)
		{
			u32VideoPos = 0;
			memcpy(videoframe, nalu_header, sizeof(nalu_header));
			u32VideoPos += sizeof(nalu_header);
			memcpy(videoframe + u32VideoPos, _pVps, _iVpsSize);
			u32VideoPos += _iVpsSize;
			memcpy(videoframe, nalu_header, sizeof(nalu_header));
			u32VideoPos += sizeof(nalu_header);
			memcpy(videoframe + u32VideoPos, _pSps, _iSpsSize);
			u32VideoPos += _iSpsSize;
			memcpy(videoframe + u32VideoPos, nalu_header, sizeof(nalu_header));
			u32VideoPos += sizeof(nalu_header);
			memcpy(videoframe + u32VideoPos, _pPps, _iPpsSize);
			u32VideoPos += _iPpsSize;
			memcpy(videoframe + u32VideoPos, nalu_header, sizeof(nalu_header));
			u32VideoPos += sizeof(nalu_header);
			
			u32VideoPos += sizeof(nalu_header);//¶Á³öÀ´µÄÊý¾ÝÍ·ËÄ×Ö½ÚÎÞÐ§,ÔÙÆ«ÒÆ4×Ö½Ú
			videoframesize = iSpsPpsSize + sample_size_vedio;//ÒòÆ«ÒÆ4¸ö×Ö½Ú£¬¹Ê×Ü´óÐ¡ÒªËã×¼
			
			(*_puiCurVideoSampleId)++;//ÊÓÆµÖ¡ºÅµÝÔö
			//td_printf(0, "------------>VideoSampleId is %d", *_puiCurVideoSampleId);
			break;
		}
		
		(*_puiCurVideoSampleId)++;//ÊÓÆµÖ¡ºÅµÝÔö
		//td_printf(0, "------------>VideoSampleId is %d", *_puiCurVideoSampleId);
		sample_size_vedio = MAX_VIDEO_SAMPLE_SIZE;//´«Èë´«³ö²ÎÊý£¬Ã¿´ÎÑ­»·ÒªÖØÐÂ¸³Öµ£¬·ñÔò¿âÀï¿ÉÄÜ»á±¨bufferÐ¡µÄ´íÎó
	}

	if(0 >= videoframesize)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}
	
	uint8_t audioframe[MAX_AUDIO_FRAMENUM*MAX_AUDIO_SAMPLE_SIZE];
	int audioframebuflen = sizeof(audioframe);
	uint32_t audioframesize = 0;

	if(Inner_GetAudioSamples(_pMP4fp,video_timestamp,_u32AudioTrackId,
		_puiCurAudioSampleId,_iAudioType,audioframe,audioframebuflen,_pstFrmaeHeader,0) < 0)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		audioframesize = 0;
	}
	else
	{
		audioframesize = (_pstFrmaeHeader->u16AFrames)*(_pstFrmaeHeader->u16AFrameSize);
	}
						
	if(videoframesize > 0)
	{
		Inner_BuildFreamHeader(_pstFrmaeHeader, isSyncSample_vedio, *_puiCurVideoSampleId - 1, 
			video_timestamp, videoframesize, 0);

		if((videoframesize+audioframesize) < MAXBUFEX)
		{
			int iBufPos = 0;
			
			memcpy((MP4_VOID*)_pTiandyFrameH265Buf + iBufPos, videoframe, iSpsPpsSize + sizeof(nalu_header));
			iBufPos += (iSpsPpsSize + sizeof(nalu_header));
			memcpy((MP4_VOID*)_pTiandyFrameH265Buf + iBufPos, videoframe + u32VideoPos, sample_size_vedio - sizeof(nalu_header));
			iBufPos += (sample_size_vedio - sizeof(nalu_header));
			
			memcpy((MP4_VOID*)_pTiandyFrameH265Buf + iBufPos, audioframe, audioframesize);
		}
		else
		{
			td_printf(0xff0000, "the length has beyond limit!");
			iRet = td_failure;
			goto leave;
		}

		iRet = td_success;
	}	
	else
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}
leave:
	return iRet;
}

/************************************************* 
  Function: MP4_GetOneTiandyFrameH265
  Description: »ñÈ¡mp4(h265±àÂë)ÎÄ¼þµÄÊÓÆµÊý¾Ý
  Input: _pMP4fp£¬			mp4ÎÄ¼þ¾ä±ú
  Input:_u32VideoTrackId£¬	ÊÓÆÁµÄtrack id
  Input:_u32AudioTrackId£¬	ÒôÆµµÄtrack id
  Input Output:_puiCurVideoSampleId£¬ÊÓÆµµÄ²ÉÑùË÷Òý£¬¶ÔÓ¦mp4ÀàÐÍÎªstszµÄbox
  Input Output:_puiCurAudioSampleId£¬ÒôÆµµÄ²ÉÑùË÷Òý£¬¶ÔÓ¦mp4ÀàÐÍÎªstszµÄbox
  Input: _pcVps£¬		ÊÓÆµ²ÎÊý¼¯Êý¾Ý
  Input: _piVpsSize£¬	ÊÓÆµ²ÎÊý¼¯´óÐ¡
  Input: _pcSps£¬		ÐòÁÐ²ÎÊý¼¯Êý¾Ý
  Input: _piSpsSize£¬	ÐòÁÐ²ÎÊý¼¯´óÐ¡
  Input: _pcPps£¬		Í¼Ïñ²ÎÊý¼¯Êý¾Ý
  Input: _piPpsSize£¬	Í¼Ïñ²ÎÊý¼¯´óÐ¡
  Output: _pTiandyFrameH265Buf£¬µÃµ½h265µÄÒ»Ö¡Êý¾Ý£¬°üº¬¿ªÊ¼±êÖ¾0x00000001
  Output:_pstFrmaeHeader
  Input:_iAudioType£¬ÒôÆµ±à½âÂëÀàÐÍ
  Return: º¯ÊýÖ´ÐÐ³É¹¦·µ»Ø td_success, Ê§°Ü·µ»Ø td_failure
  Author: ³ÂÎÄÃô(chenwenmin)
  Others: 
*************************************************/

int MP4_GetOneTiandyFrameH265_V2(void* _pMP4fp, 
										 unsigned int _u32VideoTrackId, 
										 unsigned int _u32AudioTrackId,
										 unsigned int* _puiCurVideoSampleId, 
										 unsigned int* _puiCurAudioSampleId, 
										 char* _pVps, 
										 int _iVpsSize, 
										 char* _pSps, 
										 int _iSpsSize, 
										 char* _pPps, 
										 int _iPpsSize,
										 void* _pTiandyFrameH265Buf, 
										 S_FrameHeader* _pstFrmaeHeader, 
										 int _iAudioType, 
										 unsigned int* _puiUseLessIdCount, 
										 int _iNeedIFrame)
{
	int iRet = td_success;
	uint8_t* videoframe = NULL;
	char nalu_header[] = {00,00,00,01};	
	uint64_t video_timestamp = 0;
	MP4SampleId numSamples_vedio = 0;
	uint32_t sample_size_vedio = 0;
	uint32_t videoframesize = 0;
	MP4Duration sampleDuration_vedio = 0;
	bool isSyncSample_vedio = false;
	unsigned int uiSpsPpsSize = 0;
	unsigned int uiDataBufPos = 0;
	//unsigned int uiFrameBufPos = 0;
	int iSeiCount = 0;
	unsigned int uiFirstSeiTail = 0;

	//td_printf(0, "-------------->_iNeedIFrame is %d", _iNeedIFrame);

	if((NULL == _pMP4fp)||(NULL == _puiCurVideoSampleId)||(NULL == _puiCurAudioSampleId)
		||(NULL == _pTiandyFrameH265Buf)||(NULL == _pstFrmaeHeader))
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}

	if(MP4_INVALID_TRACK_ID == _u32VideoTrackId)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}
													
	numSamples_vedio  = MP4GetTrackNumberOfSamples(_pMP4fp, _u32VideoTrackId);
	
	//×î´óµ½MP4 ÐéÄâÖ¡Ö®Ç°Ò»Ö¡
	if((0 >= *_puiCurVideoSampleId)||(*_puiCurVideoSampleId >= (numSamples_vedio - 1)))
	{
		td_printf(0xff0000, "%s %s %d error! *_piCurVideoSampleId is %d, numSamples_vedio is %d", __FILE__, __FUNCTION__, __LINE__, *_puiCurVideoSampleId, numSamples_vedio);
		iRet = td_failure;
		goto leave;
	}

	videoframe = (uint8_t*)malloc(MAX_VIDEO_SAMPLE_SIZE);

	if (NULL == videoframe)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}

	while(*_puiCurVideoSampleId < (numSamples_vedio - 1))
	{
		sample_size_vedio = MP4GetSampleSize(_pMP4fp, _u32VideoTrackId, *_puiCurVideoSampleId);
		
		uiSpsPpsSize = sizeof(nalu_header) + _iVpsSize + sizeof(nalu_header) + _iSpsSize + sizeof(nalu_header) + _iPpsSize;
		uiDataBufPos += uiSpsPpsSize;
		
		if((sample_size_vedio + uiDataBufPos) > MAX_VIDEO_SAMPLE_SIZE)
		{
			td_printf(0xff0000, "%s %s %d error! sample_size_vedio is %d, uiDataBufPos is %d", 
				__FILE__, __FUNCTION__, __LINE__, sample_size_vedio, uiDataBufPos);
			iRet = td_failure;
			goto leave;
		}
		
		if(Inner_ReadOneVideoSample(_pMP4fp,_u32VideoTrackId,*_puiCurVideoSampleId,
			videoframe+uiDataBufPos,&sample_size_vedio,&sampleDuration_vedio,&isSyncSample_vedio) < 0)
		{
			td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
			iRet = td_failure;
			goto leave;
		}
		//cwm add Ìí¼ÓNAL_IDR_N_LPÖ§³Öh265
		while((SEI_RBSP == videoframe[uiDataBufPos+MP4_BOX_SIEZ_LENGTH]) || (NAL_IDR_N_LP == videoframe[uiDataBufPos+MP4_BOX_SIEZ_LENGTH]))
		{
			iSeiCount++;
			//²»¼ÆÊýÖ¡ÊýÀÛ¼Ó¼ÆÊý
			(*_puiUseLessIdCount)++;

			//×î´óµ½MP4 ÐéÄâÖ¡Ö®Ç°Ò»Ö¡
			if(*_puiCurVideoSampleId >= (numSamples_vedio - 1))
			{
				td_printf(0xff0000, "%s %s %d error! *_piCurVideoSampleId is %d, numSamples_vedio is %d", __FILE__, __FUNCTION__, __LINE__, *_puiCurVideoSampleId, numSamples_vedio);
				iRet = td_failure;
				goto leave;
			}

			//td_printf(0, "--------->This Sample SEI iCount is %d", iSeiCount);
			
			memcpy(videoframe+uiDataBufPos, nalu_header, sizeof(nalu_header));
			uiDataBufPos += sample_size_vedio;

			if(1 == iSeiCount)
			{
				uiFirstSeiTail = uiDataBufPos;
			}
			(*_puiCurVideoSampleId)++;//ÊÓÆµÖ¡ºÅµÝÔö
			sample_size_vedio = MP4GetSampleSize(_pMP4fp, _u32VideoTrackId, *_puiCurVideoSampleId);

			if((sample_size_vedio + uiDataBufPos) > MAX_VIDEO_SAMPLE_SIZE)
			{
				td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
				iRet = td_failure;
				goto leave;
			}

			if(Inner_ReadOneVideoSample(_pMP4fp,_u32VideoTrackId,*_puiCurVideoSampleId,
				videoframe+uiDataBufPos,&sample_size_vedio,&sampleDuration_vedio,&isSyncSample_vedio) < 0)
			{
				td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
				iRet = td_failure;
				goto leave;
			}
		}

		memcpy(videoframe+uiDataBufPos, nalu_header, sizeof(nalu_header));
		uiDataBufPos += sample_size_vedio;

		if(true == isSyncSample_vedio)
		{
			memcpy(videoframe, nalu_header, sizeof(nalu_header));
			memcpy(videoframe + sizeof(nalu_header), _pVps, _iVpsSize);			
			memcpy(videoframe + sizeof(nalu_header) + _iVpsSize, nalu_header, sizeof(nalu_header));
			memcpy(videoframe + sizeof(nalu_header) + _iVpsSize + sizeof(nalu_header), _pSps, _iSpsSize);
			memcpy(videoframe + sizeof(nalu_header) + _iVpsSize + sizeof(nalu_header) + _iSpsSize, nalu_header, sizeof(nalu_header));
			memcpy(videoframe + sizeof(nalu_header) + _iVpsSize + sizeof(nalu_header) + _iSpsSize + sizeof(nalu_header), _pPps, _iPpsSize);
			
			videoframesize = uiDataBufPos;
			break;
		}
		else
		{		
			if(1 == _iNeedIFrame)
			{
				uiDataBufPos = 0;
				(*_puiCurVideoSampleId)++;//ÊÓÆµÖ¡ºÅµÝÔö
				continue;
			}
			else
			{
				videoframesize = uiDataBufPos - uiSpsPpsSize;
				break;
			}
		}
	}

	video_timestamp = (MP4GetSampleTime(_pMP4fp,_u32VideoTrackId,*_puiCurVideoSampleId)*1000/90000);
	(*_puiCurVideoSampleId)++;//ÊÓÆµÖ¡ºÅµÝÔö
	//td_printf(0, "------------>VideoSampleId is %d", *_puiCurVideoSampleId);
	
	uint8_t audioframe[MAX_AUDIO_FRAMENUM*MAX_AUDIO_SAMPLE_SIZE];
	int audioframebuflen = sizeof(audioframe);
	uint32_t audioframesize = 0;

	if(Inner_GetAudioSamples(_pMP4fp,video_timestamp,_u32AudioTrackId,
		_puiCurAudioSampleId,_iAudioType,audioframe,audioframebuflen,_pstFrmaeHeader,0) < 0)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		audioframesize = 0;
	}
	else
	{
		audioframesize = (_pstFrmaeHeader->u16AFrames)*(_pstFrmaeHeader->u16AFrameSize);
	}

	if(videoframesize > 0)
	{
		Inner_BuildFreamHeader(_pstFrmaeHeader, isSyncSample_vedio, *_puiCurVideoSampleId - (*_puiUseLessIdCount + 1), 
			video_timestamp, videoframesize, 0);

		if((videoframesize+audioframesize) < MAXBUFEX)
		{						
			if(true == isSyncSample_vedio)
			{
				#if 0
				if(iSeiCount > 1)
				{
					memcpy((MP4_VOID*)_pTiandyFrameH265Buf+uiFrameBufPos, videoframe+uiSpsPpsSize, uiFirstSeiTail - uiSpsPpsSize);
					uiFrameBufPos += (uiFirstSeiTail - uiSpsPpsSize);
					memcpy((MP4_VOID*)_pTiandyFrameH265Buf+uiFrameBufPos, videoframe, uiSpsPpsSize);
					uiFrameBufPos += uiSpsPpsSize;
					memcpy((MP4_VOID*)_pTiandyFrameH265Buf+uiFrameBufPos, videoframe+uiFirstSeiTail, videoframesize-uiFirstSeiTail);
					uiFrameBufPos += videoframesize-uiFirstSeiTail;
				}
				else
				{
					memcpy((MP4_VOID*)_pTiandyFrameH265Buf, videoframe, videoframesize);
				}
				#else
				memcpy((MP4_VOID*)_pTiandyFrameH265Buf, videoframe, videoframesize);
				#endif
			}
			else
			{
				memcpy((MP4_VOID*)_pTiandyFrameH265Buf, videoframe + uiSpsPpsSize,videoframesize);
			}

			memcpy((MP4_VOID*)_pTiandyFrameH265Buf+videoframesize, audioframe, audioframesize);
		}
		else
		{
			td_printf(0xff0000, "the length has beyond limit!");
			iRet = td_failure;
			goto leave;
		}

		iRet = td_success;
	}	
	else
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}
leave:
	if (videoframe)
	{
		free(videoframe);
		videoframe = NULL;
	}
	return iRet;
}

td_s32 MP4_GetOneEncryptTiandyFrameH265(void* _pMP4fp,  
												 unsigned int _u32VideoTrackId, 
												 unsigned int _u32AudioTrackId,
												 unsigned int* _piCurVideoSampleId, 
												 unsigned int* _piCurAudioSampleId, 
												 void* _pTiandyFrameH265Buf, 
												 S_FrameHeader* _pstFrmaeHeader, 
												 int _iAudioType,
												 int _iNeedIFrame)
{
	int iRet = td_success;

	if((NULL == _pMP4fp)||(NULL == _piCurVideoSampleId)||(NULL == _piCurAudioSampleId)
		||(NULL == _pTiandyFrameH265Buf)||(NULL == _pstFrmaeHeader))
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}

	if(MP4_INVALID_TRACK_ID == _u32VideoTrackId)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}
															
	uint64_t video_timestamp = 0;
	MP4SampleId numSamples_vedio = MP4GetTrackNumberOfSamples(_pMP4fp, _u32VideoTrackId);
	uint32_t sample_size_vedio = MAX_VIDEO_SAMPLE_SIZE;
	uint8_t videoframe[MAX_VIDEO_SAMPLE_SIZE];
	uint32_t videoframesize = 0;
	MP4Duration sampleDuration_vedio = 0;
	bool isSyncSample_vedio = false;
	
	//×î´óµ½MP4 ÐéÄâÖ¡Ö®Ç°Ò»Ö¡
	if((0 >= *_piCurVideoSampleId)||(*_piCurVideoSampleId >= (numSamples_vedio - 1)))
	{
		td_printf(0xff0000, "%s %s %d error! *_piCurVideoSampleId is %d, numSamples_vedio is %d", __FILE__, __FUNCTION__, __LINE__, *_piCurVideoSampleId, numSamples_vedio);
		iRet = td_failure;
		goto leave;
	}

	while(*_piCurVideoSampleId < (numSamples_vedio - 1))
	{
		if(Inner_ReadOneVideoSample(_pMP4fp,_u32VideoTrackId,*_piCurVideoSampleId,
			videoframe,&sample_size_vedio,&sampleDuration_vedio,&isSyncSample_vedio) < 0)
		{
			td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
			iRet = td_failure;
			goto leave;
		}

		//¼õÈ¥Í·²¿µÄ4×Ö½Ú¼ÓÃÜÖ¡ºÅÊý¾Ý
		memcpy(&(_pstFrmaeHeader->u32FrameNO), videoframe+MP4_BOX_SIEZ_LENGTH, sizeof(_pstFrmaeHeader->u32FrameNO));		
		td_printf(0, "realdatalen is %d, the Frame No is %d", sample_size_vedio -MP4_BOX_SIEZ_LENGTH, _pstFrmaeHeader->u32FrameNO);
		videoframesize = sample_size_vedio -MP4_BOX_SIEZ_LENGTH - sizeof(_pstFrmaeHeader->u32FrameNO);
		
		video_timestamp = (MP4GetSampleTime(_pMP4fp,_u32VideoTrackId,*_piCurVideoSampleId)*1000/90000);

		if(1 == _iNeedIFrame)
		{
			if(true == isSyncSample_vedio)
			{
				td_printf(0, "------------>Find I Frame VideoSampleId is %d", *_piCurVideoSampleId);
				break;
			}
			else
			{
				//¼ÌÐøÕÒ				
				(*_piCurVideoSampleId)++;//ÊÓÆµÖ¡ºÅµÝÔö
				sample_size_vedio = MAX_VIDEO_SAMPLE_SIZE;//´«Èë´«³ö²ÎÊý£¬Ã¿´ÎÑ­»·ÒªÖØÐÂ¸³Öµ£¬·ñÔò¿âÀï¿ÉÄÜ»á±¨bufferÐ¡µÄ´íÎó
				continue;
			}
		}
		else
		{
			break;
		}
	}

	(*_piCurVideoSampleId)++;//ÊÓÆµÖ¡ºÅµÝÔö
	td_printf(0, "------------>VideoSampleId is %d", *_piCurVideoSampleId);
	
	uint8_t audioframe[MAX_AUDIO_FRAMENUM*MAX_AUDIO_SAMPLE_SIZE];
	int audioframebuflen = sizeof(audioframe);
	uint32_t audioframesize = 0;

	if(Inner_GetAudioSamples(_pMP4fp,video_timestamp,_u32AudioTrackId,
		_piCurAudioSampleId,_iAudioType,audioframe,audioframebuflen,_pstFrmaeHeader,1) < 0)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		audioframesize = 0;
	}
	else
	{
		audioframesize = (_pstFrmaeHeader->u16AFrames)*(_pstFrmaeHeader->u16AFrameSize);
	}

	if(videoframesize > 0)
	{
		Inner_BuildFreamHeader(_pstFrmaeHeader, isSyncSample_vedio, *_piCurVideoSampleId - 1, 
			video_timestamp, videoframesize, 1);
		
		//²»½âÃÜ
		#if 0
		if(((_pstFrmaeHeader->reserved & 0x03) == 0x02)&&(videoframesize >= 64))
		{
			#ifdef ZHENGFA
				unsigned char* pDataBuf = (unsigned char*)(videoframe+MP4_BOX_SIEZ_LENGTH+sizeof(_pstFrmaeHeader->u32FrameNO));
				aes_crypt((unsigned char*)pDataBuf,64,_pstFrmaeHeader->u32FrameNO,(unsigned char*)CRYPT_KEY_STR,strlen(CRYPT_KEY_STR));
				
				if( ((int *)pDataBuf)[0] != 0X1000000 )
				{
					td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
					iRet = MP4_CONVERT_ERROR_CRYPT_FAILED;
					goto leave;
				}

				_pstFrmaeHeader->reserved = 0xFFFF;
			#else
				td_printf(0xff0000, "%s %s %d error! the file has been encrypt! please check!", __FILE__, __FUNCTION__, __LINE__);
				iRet = MP4_CONVERT_ERROR_CRYPT_FAILED;
				goto leave;
			#endif
		}
		#endif

		if((videoframesize+audioframesize) < MAXBUFEX)
		{			
			int iBufPos = 0;
			memcpy((MP4_VOID*)_pTiandyFrameH265Buf + iBufPos, videoframe+MP4_BOX_SIEZ_LENGTH+sizeof(_pstFrmaeHeader->u32FrameNO), videoframesize);
			iBufPos += videoframesize;
			memcpy((MP4_VOID*)_pTiandyFrameH265Buf + iBufPos, audioframe, audioframesize);
		}
		else
		{
			td_printf(0xff0000, "the length has beyond limit!");
			iRet = td_failure;
			goto leave;
		}

		iRet = td_success;
	}	
	else
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}
leave:
	return iRet;
}


//---------------new interfaces----------------------------------------------

#if 1
static TMP4ConvertSession* Inner_FindConvertSession(MP4FileHandle _pMP4FileHandle)
{	
	int i = 0;
	TMP4ConvertSession* pMP4ConvertSession = NULL;
	
	if(NULL == _pMP4FileHandle)
	{
		td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
		goto Exit;
	}
	else
	{
		for(i = 0; i < MAX_MP4_CONVSESSION_NUM; i++)
		{
			if((1 == gstMP4ConvertSession[i].m_iUsing)
					&&(gstMP4ConvertSession[i].m_stMP4Manager.m_MP4Handle == _pMP4FileHandle))
			{
				pMP4ConvertSession = &gstMP4ConvertSession[i];
				break;
			}
		}
	}

	if(NULL == pMP4ConvertSession)
	{
		td_printf(0x12345678, "%s %d _pMP4FileHandle is %p", __FUNCTION__, __LINE__, _pMP4FileHandle);
		for(i = 0; i < MAX_MP4_CONVSESSION_NUM; i++)
		{
			td_printf(0xff0000, "%s gstMP4ConvertSession[%d].m_stMP4Manager.m_MP4Handle is %p",
				__FUNCTION__, i, gstMP4ConvertSession[i].m_stMP4Manager.m_MP4Handle);
		}
	}
Exit:
	return pMP4ConvertSession;
}

static TMP4ResumSession* Inner_FindResumSession(MP4FileHandle _pMP4FileHandle)
{	
	int i = 0;
	TMP4ResumSession* pMP4ResumSession = NULL;
	
	if(NULL == _pMP4FileHandle)
	{
		td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
		goto Exit;
	}
	else
	{
		for(i = 0; i < MAX_MP4_CONVSESSION_NUM; i++)
		{
			if((1 == gstMP4ResumSession[i].m_iUsing)
				&&(gstMP4ResumSession[i].m_MP4Handle == _pMP4FileHandle))
			{
				pMP4ResumSession = &gstMP4ResumSession[i];
				break;
			}
		}
	}

	if(NULL == pMP4ResumSession)
	{
		td_printf(0, "%s %d _pMP4FileHandle is %p", __FUNCTION__, __LINE__, _pMP4FileHandle);
		for(i = 0; i < MAX_MP4_CONVSESSION_NUM; i++)
		{
			td_printf(0, "%s gstMP4ResumSession[%d].m_stMP4Manager.m_MP4Handle is %p",
				__FUNCTION__, i, gstMP4ResumSession[i].m_MP4Handle);
		}
	}
Exit:
	return pMP4ResumSession;
}

static int Inner_UpdateFileHeaderInfo(TMP4ConvertSession* _pMP4ConvertSession, S_FileHeader* _pstFileHeader)
{
	int iRet = td_failure;
	TMP4ISampleIndexHead* pMP4ISampleIndexHead = NULL;
	S_FileHeader* pstFileHeader = NULL;
	
	if((NULL == _pMP4ConvertSession)||(NULL == _pstFileHeader))
	{
		td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
		goto Exit;
	}
	
	pMP4ISampleIndexHead = (TMP4ISampleIndexHead*)(_pMP4ConvertSession->m_puVirFrameBuf+MP4_BOX_SIEZ_LENGTH+MP4_SAMPLE_TYPE_LENGTH);
	pstFileHeader = &(pMP4ISampleIndexHead->m_stExRecInfo.m_stSdvFileHead);

	#if 1
	if(0 == pstFileHeader->FrameRate)
	{
		pstFileHeader->FrameRate = _pstFileHeader->FrameRate;
	}
	else if(_pstFileHeader->FrameRate != pstFileHeader->FrameRate)
	{
		td_printf(0xff0000, "%s %s %d error! Frame Rate Changed!\n", __FILE__, __FUNCTION__, __LINE__);
		goto Exit;
	}
	#endif

	if(0 == pstFileHeader->Width)
	{
		pstFileHeader->Width = _pstFileHeader->Width;
	}
	else if(_pstFileHeader->Width != pstFileHeader->Width)
	{
		td_printf(0xff0000, "%s %s %d error! Video Size Changed!\n", __FILE__, __FUNCTION__, __LINE__);
		goto Exit;
	}

	if(0 == pstFileHeader->Height)
	{
		pstFileHeader->Height = _pstFileHeader->Height;
	}
	else if(_pstFileHeader->Height != pstFileHeader->Height)
	{
		td_printf(0xff0000, "%s %s %d error!Video Size Changed!\n", __FILE__, __FUNCTION__, __LINE__);
		goto Exit;
	}
	
	pstFileHeader->Mode      = 21;
	
	//Ö»ÒªÓÐÒ»¸öÂ¼Ïñ´øÒôÆµ£¬Ôò´óÂ¼ÏñÒ²´øÒôÆµ
	if(0 == pstFileHeader->bAudio)
	{
		pstFileHeader->bAudio = _pstFileHeader->bAudio;
	}
	
	pstFileHeader->Year      = 0xFFFF;
	pstFileHeader->Month     = 0xFFFF;
	pstFileHeader->Day       = 0xFFFF;
	pstFileHeader->Hour      = 0xFFFF;
	pstFileHeader->Minute    = 0xFFFF;
	pstFileHeader->Second    = 0xFFFF;
	pstFileHeader->CoverMask = 0xFFFF;

	if(!strstr(pstFileHeader->cCovWord, FILE_HEADER_COVER_WORD))
	{
		strncpy(pstFileHeader->cCovWord, FILE_HEADER_COVER_WORD, 35);
	}
	
	//ÒÔÊµ¼Ê×ª»»µÄÖ¡ÊýÎª×¼
	#if 0
	td_printf(0, "the m_uiFrameNoDiff is %d, FrameCount is %d", 
		_pstFileHeader->m_uiFrameNoDiff, pstFileHeader->FrameCount);
	
	if(0 == pstFileHeader->m_uiFrameNoDiff)
	{
		pstFileHeader->m_uiFrameNoDiff = _pstFileHeader->m_uiFrameNoDiff;
	}
	else
	{
		pstFileHeader->m_uiFrameNoDiff += (_pstFileHeader->m_uiFrameNoDiff+1);
	}
	#endif
	
	pstFileHeader->TriggerHigh = 0;
	pstFileHeader->TriggerLow  = 0;
	pstFileHeader->reserved    = MAX_COVWORD_LEN; 

	#if 0//Ó¦¸ÃÃ»Ó°Ïì£¬²»¿¼ÂÇ´ËÊôÐÔµÄ±ä»¯
	if(0 == pstFileHeader->AChannels)
	{
		pstFileHeader->AChannels = _pstFileHeader->AChannels;
	}
	else if(_pstFileHeader->AChannels != pstFileHeader->AChannels)
	{
		td_printf(0xff0000, "%s %s %d error! Audio Channels is %d, before is %d\n", 
			__FILE__, __FUNCTION__, __LINE__, _pstFileHeader->AChannels, pstFileHeader->AChannels);
		goto Exit;
	}
	#endif
	
	if(0 == pstFileHeader->BitsPerSample)
	{
		pstFileHeader->BitsPerSample = _pstFileHeader->BitsPerSample;
	}
	else if(_pstFileHeader->BitsPerSample != pstFileHeader->BitsPerSample)
	{
		td_printf(0xff0000, "%s %s %d error! BitsPerSample is %d, before is %d\n", 
			__FILE__, __FUNCTION__, __LINE__, _pstFileHeader->BitsPerSample, pstFileHeader->BitsPerSample);
		goto Exit;
	}
	
	if(0 == pstFileHeader->AudioSample)
	{
		pstFileHeader->AudioSample = _pstFileHeader->AudioSample;
	}
	else if(_pstFileHeader->AudioSample != pstFileHeader->AudioSample)
	{
		td_printf(0xff0000, "%s %s %d error! AudioSample is %d, before is %d\n", 
			__FILE__, __FUNCTION__, __LINE__, _pstFileHeader->AudioSample, pstFileHeader->AudioSample);
		goto Exit;
	}
	
	//Èç¹ûÎÄ¼þ´óÐ¡ÀÛ»ý³¬¹ý4G,ÔòÊý¾ÝÀàÐÍÒç³ö
	//pstFileHeader->TotalSize   += _pstFileHeader->TotalSize;//´ËÀàÐÍÔÝÊ±Ã»ÓÐÀ©Õ¹
	
	//ÒÔÊµ¼Ê×ª»»µÄÖ¡ÊýÎª×¼
	//pstFileHeader->FrameCount  += _pstFileHeader->FrameCount;

	iRet = td_success;

Exit:
	return iRet;
}

static int Inner_WriteBaseUnitToMP4(MP4FileHandle _pMP4FileHandle, unsigned int _uiBaseUnitSize, unsigned int _uiMemsize,
	int _iBaseUnitType, unsigned char* _pDataBuf, unsigned int _uiDataSize)
{
	//static int a = 1;
	//static int b = 1;
	//int iFlag = -3;
	TAVSampleInfo* ptmp = NULL;
	int iRet = td_failure;
	TBaseUnit* pstMP4Virinfo = NULL;
	unsigned char ucBaseUnitType[MP4_BOX_TYPE_LENGTH] = {0};
#ifdef FOR_MP4SDK
#else
	if((NULL == _pMP4FileHandle)||(NULL == _pDataBuf))
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		goto Exit;
	}

	//td_printf(0, "--->BaseUnitType is %d", _iBaseUnitType);

	switch (_iBaseUnitType)
	{
		case VMFT:
		{
			memcpy((char*)ucBaseUnitType, (char*)VIRTUAL_FRAME_TYPE, MP4_BOX_TYPE_LENGTH);
			break;
		}
		case AVST:
		{
			ptmp = (TAVSampleInfo*)_pDataBuf;
			//iFlag = ptmp->m_iFrameFlag;
			//td_printf(0, "==========>DataSize  is %d", ptmp->m_uiAVSizeAndDuring.uiSampleSize);
			memcpy((char*)ucBaseUnitType, (char*)AV_SAMPLE_INFO_TYPE, MP4_BOX_TYPE_LENGTH);
			break;
		}
		case VDTT:
		{
			memcpy((char*)ucBaseUnitType, (char*)VIDEO_TRACK_INFO_TYPE, MP4_BOX_TYPE_LENGTH);
			break;
		}
		case ADTT:
		{
			memcpy((char*)ucBaseUnitType, (char*)AUDIO_TRACK_INFO_TYPE, MP4_BOX_TYPE_LENGTH);
			break;
		}
		case ADET:
		{
			memcpy((char*)ucBaseUnitType, (char*)AUDIO_ENCODE_TYPE, MP4_BOX_TYPE_LENGTH);
			break;
		}
		case ENCT:
		{
			memcpy((char*)ucBaseUnitType, (char*)ENCRYPTION_TYPE, MP4_BOX_TYPE_LENGTH);
			break;
		}
		case AJON:
		{
			memcpy((char*)ucBaseUnitType, (char*)ADJOURN, MP4_BOX_TYPE_LENGTH);
			break;
		}
		default:
		{
			td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
			goto Exit;
		}
	}

	pstMP4Virinfo  = (TBaseUnit*)malloc(sizeof(TBaseUnit)+_uiDataSize);

	if(NULL != pstMP4Virinfo)
	{
		pstMP4Virinfo->m_uiSize = _uiBaseUnitSize;
		pstMP4Virinfo->m_uiMemSize = _uiMemsize;
		memcpy((char*)(pstMP4Virinfo->m_ucType), (char*)ucBaseUnitType, MP4_BOX_TYPE_LENGTH);
		memcpy(pstMP4Virinfo->m_pData, _pDataBuf, _uiDataSize);

		//td_printf(0, "+++++duibi++++++++++++write>>>>>>>>>>>pstMP4Virinfo->m_ucType[%d][%d] is %s, iFlag is %d, _uiBaseUnitSize is %d, m_uiMemSize is %d, _uiDataSize is %d", 
			//a++, ((iFlag == 0)||(iFlag == 1)? b++:b), pstMP4Virinfo->m_ucType, iFlag, _uiBaseUnitSize, _uiMemsize, (NULL != ptmp) ? ptmp->m_uiAVSizeAndDuring.uiSampleSize : 0);
	
		if(MP4WriteBaseUnit(_pMP4FileHandle, _iBaseUnitType, pstMP4Virinfo->m_uiMemSize, pstMP4Virinfo->m_pData, _uiDataSize) < 0)
		{
			td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
			goto Exit;
		}	
	}
	else
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		goto Exit;
	}

	iRet = td_success;
Exit:
	if(NULL != pstMP4Virinfo)
	{
		free(pstMP4Virinfo);
		pstMP4Virinfo = NULL;
	}
#endif
	return iRet;
}

static int Inner_WriteAVSampleInfoBaseUnit(void* _pMP4handle, int _iFrameFlag, 
	unsigned int _uiSampleSize, unsigned long long _ullSampleDuration)
{
	int iRet = td_failure;
	TAVSampleInfo stAVSampleInfo = {0};

	if(NULL == _pMP4handle)
	{
		td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
		return iRet;
	}

	stAVSampleInfo.m_iFrameFlag = _iFrameFlag;
	stAVSampleInfo.m_uiAVSizeAndDuring.uiSampleSize   = _uiSampleSize;
	stAVSampleInfo.m_uiAVSizeAndDuring.sampleDuration = _ullSampleDuration;

	//td_printf(0, "+++++duibi--Inner_WriteAVSampleInfoBaseUnit---------------------->_iFrameFlag is %d, uiSampleSize is %d, sampleDuration is %lld",
			//_iFrameFlag, _uiSampleSize, _ullSampleDuration);

	if(Inner_WriteBaseUnitToMP4(_pMP4handle,
		sizeof(TBaseUnit)+sizeof(TAVSampleInfo),
		sizeof(TAVSampleInfo),
		AVST,(unsigned char*)&stAVSampleInfo,sizeof(TAVSampleInfo)) < 0)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		goto Exit;
	}

	iRet = td_success;
Exit:
	return iRet;
}

static int Inner_WriteSimpleInfoBaseUnit(void* _pMP4handle, int _iMdatType, unsigned int _uiSimpeInfo)
{
	int iRet = td_failure;
	unsigned int uiSimpleInfo = 0xff;

	if(NULL == _pMP4handle)
	{
		td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
		return iRet;
	}

	uiSimpleInfo = _uiSimpeInfo;
	
	if(Inner_WriteBaseUnitToMP4(_pMP4handle,
		sizeof(TBaseUnit)+sizeof(uiSimpleInfo),
		sizeof(uiSimpleInfo),
		_iMdatType,(unsigned char*)&uiSimpleInfo, sizeof(uiSimpleInfo)) < 0)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		goto Exit;
	}

	iRet = td_success;
Exit:
	return iRet;
}


static int Inner_GetAndWriteMP4Data(TMP4ConvertSession* _pstMP4ConvertSession, unsigned char* _pData, int iDataLen, 
	int iDataType, MP4Duration _ullDataDuration, MP4Duration renderingOffset, bool isSyncSample, bool isVirFrame)
{
	int iRet = td_failure;
	unsigned int uiTrackId = MP4_INVALID_TRACK_ID;
	TMP4Mgr* _pTMP4Mgr = NULL;

#ifdef FOR_MP4SDK
#else
	if((NULL == _pstMP4ConvertSession)||(NULL == _pData))
	{
		td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
		return iRet;
	}

	_pTMP4Mgr = &(_pstMP4ConvertSession->m_stMP4Manager);

	if(VIDEODATA == iDataType)
	{
		uiTrackId = _pTMP4Mgr->m_VideoTrackID;
	}
	else if(AUDIODATA == iDataType)
	{
		uiTrackId = _pTMP4Mgr->m_AudioTrackID;
	}

	if(MP4_INVALID_TRACK_ID == uiTrackId)
	{
		td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
		td_printf(0xff0000, "warning the trackid is error! type :[%d]", iDataType);
		goto Exit;
	}

	//td_printf(0, "+++++duibi-------->MP4WriteSampleRT iDataLen[%d] _ullDataDuration[%lld] isSyncSample[%d]", iDataLen, _ullDataDuration, isSyncSample);
	if(!MP4WriteSampleRT(_pTMP4Mgr->m_MP4Handle, uiTrackId, _pData, iDataLen, _ullDataDuration, renderingOffset, isSyncSample, isVirFrame, NULL, NULL))
	{
		td_printf(0xff0000, "%d: Call MP4WriteSample failed!.\n", __LINE__);
		goto Exit;
	}
	
	iRet = td_success;

Exit:
#endif
	return iRet;
}

static int Inner_ConvertVDataToMP4Sample(TMP4ConvertSession* _pstMP4ConvertSession, const unsigned char* _pcH264, int _iH264Size)  
{
	int iRet = td_failure;
#ifdef FOR_MP4SDK
#else
	int iSeekRet = 0;
	unsigned char* pcH264 = NULL;
	int iH264Size = 0;
	unsigned char* pcTmpMP4SameBuf = NULL;
	int iTmpMP4SameBufLen = MP4_BOX_SIEZ_LENGTH;
	TMP4Nalu stNalu = {0};  
	int iPos = 0;
	int iAnalyzeRes = 0;
	int iEnCryptDataLen = 0;
	TMP4Mgr* pstMP4Mgr = NULL;
	unsigned char* pucVideoTrackData = NULL;
	TVideoTrackInfo stVideoTrackInfo = {0};
	unsigned char* pSps = NULL;
	unsigned char* pPps = NULL;
	unsigned long long ullTmpVideoDuration = 0;
	TMP4ISampleIndexHead* pMP4ISampleIndexHead = NULL;
	int iIFrameFlag = 6;//ÎÞÐ§Ö¡
	int iNeedWriteBufToMp4 = 0;
		
	if((NULL == _pstMP4ConvertSession) || (NULL == _pcH264) || (0 >= _iH264Size))
	{
		td_printf(0xff0000, "%d: Input parameters is error!\n", __LINE__);
		goto EXIT;
	}
	
	pMP4ISampleIndexHead = (TMP4ISampleIndexHead*)(_pstMP4ConvertSession->m_puVirFrameBuf+MP4_BOX_SIEZ_LENGTH+MP4_SAMPLE_TYPE_LENGTH);
	pstMP4Mgr = &(_pstMP4ConvertSession->m_stMP4Manager);
	pcH264 = (unsigned char*)_pcH264;
	iH264Size = _iH264Size;

	pcTmpMP4SameBuf = pstMP4Mgr->m_ucVideoSampleDataBuf;
	
	if(NULL == pcTmpMP4SameBuf)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		goto EXIT;
	}
		
	if((pstMP4Mgr->m_stSDVFrameHead.reserved & 0x03) == 0x02)
	{		
		if(MP4_INVALID_TRACK_ID == pstMP4Mgr->m_VideoTrackID)
		{
			pstMP4Mgr->m_VideoTrackID = MP4AddH264VideoTrack(pstMP4Mgr->m_MP4Handle, 90000, 90000/pstMP4Mgr->m_stSDVFileHead.FrameRate, pstMP4Mgr->m_stSDVFileHead.Width
				, pstMP4Mgr->m_stSDVFileHead.Height, 0, 0, 0, 0);

			if (pstMP4Mgr->m_VideoTrackID == MP4_INVALID_TRACK_ID)	
			{
				td_printf(0xff0000, "%d: Add video track failed!\n", __LINE__);
				goto EXIT;
			}
			else
			{				
				MP4SetTrackDurationPerChunk(pstMP4Mgr->m_MP4Handle, pstMP4Mgr->m_VideoTrackID, 90000 / pstMP4Mgr->m_stSDVFileHead.FrameRate);

				if(DIRBURN_WITHOUTSATA == _pstMP4ConvertSession->m_iConvertType)
				{
					stVideoTrackInfo.timeScale = 90000;
					stVideoTrackInfo.sampleDuration = 90000/pstMP4Mgr->m_stSDVFileHead.FrameRate;
					stVideoTrackInfo.width = pstMP4Mgr->m_stSDVFileHead.Width;
					stVideoTrackInfo.height = pstMP4Mgr->m_stSDVFileHead.Height;
					stVideoTrackInfo.AVCProfileIndication = 0;
					stVideoTrackInfo.profile_compat = 0;
					stVideoTrackInfo.AVCLevelIndication = 0;
					stVideoTrackInfo.sampleLenFieldSizeMinusOne = 0;
					stVideoTrackInfo.sequenceLen = 0;
					stVideoTrackInfo.pictLen = 0;
					
					if(Inner_WriteBaseUnitToMP4(_pstMP4ConvertSession->m_stMP4Manager.m_MP4Handle,
						sizeof(TBaseUnit)+sizeof(TVideoTrackInfo),sizeof(TVideoTrackInfo),VDTT,(unsigned char*)&stVideoTrackInfo,sizeof(TVideoTrackInfo)) < 0)
					{
						td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
						goto EXIT;
					}
				}
				else
				{
					if(MP4WriteBaseUnit(_pstMP4ConvertSession->m_stMP4Manager.m_MP4Handle
						, VDTT, 0, (unsigned char*)"uselessinfo", 11) < 0)
					{
						td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
						goto EXIT;
					}		
				}
			}

			td_printf(0, "+++++++++>++++++++++++> 11111111111111111111111111");
			if(Inner_WriteSimpleInfoBaseUnit(pstMP4Mgr->m_MP4Handle, ENCT, _pstMP4ConvertSession->m_uiEncryptFlag) < 0)
			{
				td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
				goto EXIT;
			}			
			if(Inner_WriteSimpleInfoBaseUnit(pstMP4Mgr->m_MP4Handle, ADET, _pstMP4ConvertSession->m_uiAudioType) < 0)
			{
				td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
				goto EXIT;
			}
			td_printf(0, "+++++++++>++++++++++++> 22222222222222222222222222");
		}
	
		iEnCryptDataLen = sizeof(pstMP4Mgr->m_stSDVFrameHead.u32FrameNO) + iH264Size;

		if((iTmpMP4SameBufLen+iEnCryptDataLen) <= pstMP4Mgr->m_iVideoSampleDataBufSize)
		{
			pcTmpMP4SameBuf[0] = iEnCryptDataLen>>24;  
			pcTmpMP4SameBuf[1] = ((iEnCryptDataLen)&0x00ff0000)>>16;  
			pcTmpMP4SameBuf[2] = ((iEnCryptDataLen)&0x0000ff00)>>8;  
			pcTmpMP4SameBuf[3] = (iEnCryptDataLen)&0x000000ff;	
			memcpy(pcTmpMP4SameBuf+iTmpMP4SameBufLen, &(pstMP4Mgr->m_stSDVFrameHead.u32FrameNO), sizeof(pstMP4Mgr->m_stSDVFrameHead.u32FrameNO));
			iTmpMP4SameBufLen += sizeof(pstMP4Mgr->m_stSDVFrameHead.u32FrameNO);
			memcpy(pcTmpMP4SameBuf+iTmpMP4SameBufLen,pcH264,iH264Size);
			iTmpMP4SameBufLen += iH264Size;
		}
		else
		{
			td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
			iRet = td_failure;
			goto EXIT;
		}

		pstMP4Mgr->m_bIFrameFlag = 0;
		
		if (pstMP4Mgr->m_stSDVFrameHead.u8FrameType == 0)
		{
			pstMP4Mgr->m_bIFrameFlag = 1;
		}
							
		if(Inner_GetAndWriteMP4Data(_pstMP4ConvertSession, pcTmpMP4SameBuf, iTmpMP4SameBufLen, VIDEODATA, pstMP4Mgr->m_VedioDuration, 0, pstMP4Mgr->m_bIFrameFlag, 0) < 0)
		{
			td_printf(0xff0000, "%d: Call Inner_GetAndWriteMP4Data failed!.\n", __LINE__);
			iRet = td_failure;
			goto EXIT;
		}
		
		if(DIRBURN_WITHOUTSATA == _pstMP4ConvertSession->m_iConvertType)
		{
			if(Inner_WriteAVSampleInfoBaseUnit(_pstMP4ConvertSession->m_stMP4Manager.m_MP4Handle,
				pstMP4Mgr->m_bIFrameFlag, iTmpMP4SameBufLen, pstMP4Mgr->m_VedioDuration) < 0)
			{
				td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
				goto EXIT;
			}
		}
		
		//Ð´Èë³É¹¦²Å¼ÆÊý
		pstMP4Mgr->m_uiVedioSampleID++;
		pstMP4Mgr->m_VedioTotalDuration += pstMP4Mgr->m_VedioDuration;
		pMP4ISampleIndexHead->m_stExRecInfo.m_stSdvFileHead.FrameCount++;
		pMP4ISampleIndexHead->m_stExRecInfo.m_stSdvFileHead.m_uiFrameNoDiff = 
			pMP4ISampleIndexHead->m_stExRecInfo.m_stSdvFileHead.FrameCount-1;
		_pstMP4ConvertSession->m_ullVideoTotalDuration += pstMP4Mgr->m_VedioDuration;

		if (pstMP4Mgr->m_stSDVFrameHead.u8FrameType == 0)
		{
			pstMP4Mgr->m_iIFrameCount++;
		}	
	}
	else
	{			
		unsigned int uiNaluheader = ntohl(0x00000001);

		while (0 != (iAnalyzeRes = Inner_AnalyzeNalu(pcH264,iH264Size,iPos,&stNalu)))  
		{
			td_printf(0x12345678, "the nalu type is ================>%2x", stNalu.m_iType);
		
			if(stNalu.m_iType == SEQ_PARAMETER_SET_RBSP)
			{//sps
				if(0 == pstMP4Mgr->m_bFirstSpsFlag)
				{
					if(MP4_INVALID_TRACK_ID == pstMP4Mgr->m_VideoTrackID)
					{
						//td_printf(0x12345678, "---------------->>>>>write Sps^^^^^^^^^^^^pstMP4Mgr->m_stSDVFileHead.FrameRate is %d", pstMP4Mgr->m_stSDVFileHead.FrameRate);
						pstMP4Mgr->m_VideoTrackID = MP4AddH264VideoTrack(pstMP4Mgr->m_MP4Handle, 90000, 90000/pstMP4Mgr->m_stSDVFileHead.FrameRate, pstMP4Mgr->m_stSDVFileHead.Width
							, pstMP4Mgr->m_stSDVFileHead.Height, stNalu.m_pucVedioData[1], stNalu.m_pucVedioData[2], stNalu.m_pucVedioData[3], 3);
						if (pstMP4Mgr->m_VideoTrackID == MP4_INVALID_TRACK_ID)	
						{
							td_printf(0xff0000, "%d: Add video track failed!\n", __LINE__);
							goto EXIT;
						}
		
						MP4SetVideoProfileLevel(pstMP4Mgr->m_MP4Handle, 0x01);
						MP4AddH264SequenceParameterSet(pstMP4Mgr->m_MP4Handle,pstMP4Mgr->m_VideoTrackID, stNalu.m_pucVedioData, stNalu.m_iVedioSize); 
						MP4SetTrackDurationPerChunk(pstMP4Mgr->m_MP4Handle, pstMP4Mgr->m_VideoTrackID, 90000 / pstMP4Mgr->m_stSDVFileHead.FrameRate);

						if(DIRBURN_WITHOUTSATA == _pstMP4ConvertSession->m_iConvertType)
						{
							stVideoTrackInfo.timeScale = 90000;
							stVideoTrackInfo.sampleDuration = 90000/pstMP4Mgr->m_stSDVFileHead.FrameRate;
							stVideoTrackInfo.width = pstMP4Mgr->m_stSDVFileHead.Width;
							stVideoTrackInfo.height = pstMP4Mgr->m_stSDVFileHead.Height;
							stVideoTrackInfo.AVCProfileIndication = stNalu.m_pucVedioData[1];
							stVideoTrackInfo.profile_compat = stNalu.m_pucVedioData[2];
							stVideoTrackInfo.AVCLevelIndication = stNalu.m_pucVedioData[3];
							stVideoTrackInfo.sampleLenFieldSizeMinusOne = 3;
							stVideoTrackInfo.sequenceLen = stNalu.m_iVedioSize;

							pSps = (unsigned char*)malloc(stVideoTrackInfo.sequenceLen);

							if(NULL == pSps)
							{
								td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
								goto EXIT;
							}
							memcpy(pSps, stNalu.m_pucVedioData, stNalu.m_iVedioSize);
						}
					}
					
					pstMP4Mgr->m_bFirstSpsFlag = 1;
				}
			}  
			else if(stNalu.m_iType == PIC_PARAMETER_SET_RBSP)
			{//pps
				if(0 == pstMP4Mgr->m_bFirstPpsFlag)
				{
					if(MP4_INVALID_TRACK_ID != pstMP4Mgr->m_VideoTrackID)
					{
						td_printf(0x12345678, "---------------->>>>> write Pps");
						MP4AddH264PictureParameterSet(pstMP4Mgr->m_MP4Handle,pstMP4Mgr->m_VideoTrackID, stNalu.m_pucVedioData, stNalu.m_iVedioSize);

						if(DIRBURN_WITHOUTSATA == _pstMP4ConvertSession->m_iConvertType)
						{
							stVideoTrackInfo.pictLen = stNalu.m_iVedioSize;
						
							pPps = (unsigned char*)malloc(stVideoTrackInfo.pictLen);

							if(NULL == pPps)
							{
								td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
								goto EXIT;
							}
							memcpy(pPps, stNalu.m_pucVedioData, stNalu.m_iVedioSize);
						}
					}
					pstMP4Mgr->m_bFirstPpsFlag = 1;
				}
			} 
			else  if((1 == pstMP4Mgr->m_bFoundSpsPpsFlag)&&
				((SLICE_LAYER_WITHOUT_PARTITIONING_RBSP == stNalu.m_iType) || 
					(SLICE_LAYER_WITH_PARTITIONING_RBSP == stNalu.m_iType) ||
					((SEI_RBSP == stNalu.m_iType)&&(pstMP4Mgr->m_iIFrameCount > 0))))
			{
				//td_printf(0x12345678, "---------------->>>>>write IFrame or PFrame! stNalu.m_iType [%02x]", stNalu.m_iType);
				if (SLICE_LAYER_WITH_PARTITIONING_RBSP == stNalu.m_iType)
				{
					pstMP4Mgr->m_bIFrameFlag = 1;					
					iIFrameFlag = 1;				
				}
				else if(SLICE_LAYER_WITHOUT_PARTITIONING_RBSP == stNalu.m_iType)
				{
					pstMP4Mgr->m_bIFrameFlag = 0;
					iIFrameFlag = 0;				
				}
				else if(SEI_RBSP == stNalu.m_iType)
				{
					pstMP4Mgr->m_bIFrameFlag = 0;
					iIFrameFlag = 6;//ÓÃÓÚ×Ô¶¨ÒåÐÅÏ¢¼ÇÂ¼£¬¶ÏµçÐÞ¸´Ê¹ÓÃ				
				}
				else//²»¿ÉÄÜ×ßµ½Õâ
				{
					td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
					goto EXIT;
				}

				if(stNalu.m_iVedioSize > 0)
				{
					if((SLICE_LAYER_WITH_PARTITIONING_RBSP == stNalu.m_iType)
						||(SLICE_LAYER_WITHOUT_PARTITIONING_RBSP == stNalu.m_iType))
					{
						if(MP4_BOX_SIEZ_LENGTH == iTmpMP4SameBufLen)//µÚÒ»¸öslice
						{
							if((iTmpMP4SameBufLen+stNalu.m_iVedioSize) <= pstMP4Mgr->m_iVideoSampleDataBufSize)
							{
								memcpy(pcTmpMP4SameBuf+iTmpMP4SameBufLen,stNalu.m_pucVedioData,stNalu.m_iVedioSize);
								iTmpMP4SameBufLen += stNalu.m_iVedioSize;
							}
							else
							{
								td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
								goto EXIT;
							}
						}
						else
						{
							if((iTmpMP4SameBufLen+4+stNalu.m_iVedioSize) <= pstMP4Mgr->m_iVideoSampleDataBufSize)
							{
								memcpy(pcTmpMP4SameBuf+iTmpMP4SameBufLen,&uiNaluheader,4);
								iTmpMP4SameBufLen += 4;
								memcpy(pcTmpMP4SameBuf+iTmpMP4SameBufLen,stNalu.m_pucVedioData,stNalu.m_iVedioSize);
								iTmpMP4SameBufLen += stNalu.m_iVedioSize;
							}
							else
							{
								td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
								goto EXIT;
							}
						}
						
						iNeedWriteBufToMp4 = 1;//ÐèÒª½«»º´æÖÐµÄÊý¾Ý×ª»»Îªmp4 sample
					}
					else if(SEI_RBSP == stNalu.m_iType)
					{
						memcpy(pcTmpMP4SameBuf+iTmpMP4SameBufLen,stNalu.m_pucVedioData,stNalu.m_iVedioSize);
						iTmpMP4SameBufLen += stNalu.m_iVedioSize;

						pcTmpMP4SameBuf[0] = (iTmpMP4SameBufLen-MP4_BOX_SIEZ_LENGTH)>>24;  
						pcTmpMP4SameBuf[1] = ((iTmpMP4SameBufLen-MP4_BOX_SIEZ_LENGTH)&0x00ff0000)>>16;	
						pcTmpMP4SameBuf[2] = ((iTmpMP4SameBufLen-MP4_BOX_SIEZ_LENGTH)&0x0000ff00)>>8;  
						pcTmpMP4SameBuf[3] = (iTmpMP4SameBufLen-MP4_BOX_SIEZ_LENGTH)&0x000000ff;	
						
						ullTmpVideoDuration = 0;
						//td_printf(0x12345678, "------------------------->m_uiUselessCount is %u", pstMP4Mgr->m_uiUselessCount);
						
						//td_printf(0, "********************iTmpMP4SameBufLen is %d, iIFrameFlag is %d", iTmpMP4SameBufLen, iIFrameFlag);
						
						if(Inner_GetAndWriteMP4Data(_pstMP4ConvertSession, pcTmpMP4SameBuf, iTmpMP4SameBufLen, VIDEODATA, ullTmpVideoDuration, 0, pstMP4Mgr->m_bIFrameFlag, 0) < 0)
						{
							td_printf(0xff0000, "%d: Call Inner_GetAndWriteMP4Data failed!.\n", __LINE__);
							iRet = td_failure;
							goto EXIT;
						}
						
						if(DIRBURN_WITHOUTSATA == _pstMP4ConvertSession->m_iConvertType)
						{
							if(Inner_WriteAVSampleInfoBaseUnit(_pstMP4ConvertSession->m_stMP4Manager.m_MP4Handle,
								iIFrameFlag, iTmpMP4SameBufLen, ullTmpVideoDuration) < 0)
							{
								td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
								goto EXIT;
							}
						}
										
						//Ð´Èë³É¹¦²Å¼ÆÊý
						pstMP4Mgr->m_uiUselessCount++;//ÎÞÐ§Ö¡¼ÆÊýÀÛ¼Ó
						pstMP4Mgr->m_uiVedioSampleID++;
						pstMP4Mgr->m_VedioTotalDuration += ullTmpVideoDuration;
						_pstMP4ConvertSession->m_ullVideoTotalDuration += ullTmpVideoDuration;
						iTmpMP4SameBufLen = MP4_BOX_SIEZ_LENGTH;//Ð´ÍêÒ»¸öNALUºó£¬ÖØÐÂ¸³Öµ
						iNeedWriteBufToMp4 = 0;//»º´æÊý¾ÝÒÑ×ª»»£¬ÖÃÁã
					}
				}			
			}
			iPos += iAnalyzeRes;  
		}

		if(1 == iNeedWriteBufToMp4)
		{
			pcTmpMP4SameBuf[0] = (iTmpMP4SameBufLen-MP4_BOX_SIEZ_LENGTH)>>24;  
			pcTmpMP4SameBuf[1] = ((iTmpMP4SameBufLen-MP4_BOX_SIEZ_LENGTH)&0x00ff0000)>>16;	
			pcTmpMP4SameBuf[2] = ((iTmpMP4SameBufLen-MP4_BOX_SIEZ_LENGTH)&0x0000ff00)>>8;  
			pcTmpMP4SameBuf[3] = (iTmpMP4SameBufLen-MP4_BOX_SIEZ_LENGTH)&0x000000ff;	
						
			//td_printf(0, "********************iTmpMP4SameBufLen is %d, iIFrameFlag is %d", iTmpMP4SameBufLen, iIFrameFlag);
			ullTmpVideoDuration = pstMP4Mgr->m_VedioDuration;

			if(Inner_GetAndWriteMP4Data(_pstMP4ConvertSession, pcTmpMP4SameBuf, iTmpMP4SameBufLen, VIDEODATA, ullTmpVideoDuration, 0, pstMP4Mgr->m_bIFrameFlag, 0) < 0)
			{
				td_printf(0xff0000, "%d: Call Inner_GetAndWriteMP4Data failed!.\n", __LINE__);
				iRet = td_failure;
				goto EXIT;
			}
			
			if(DIRBURN_WITHOUTSATA == _pstMP4ConvertSession->m_iConvertType)
			{
				if(Inner_WriteAVSampleInfoBaseUnit(_pstMP4ConvertSession->m_stMP4Manager.m_MP4Handle,
					iIFrameFlag, iTmpMP4SameBufLen, ullTmpVideoDuration) < 0)
				{
					td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
					goto EXIT;
				}
			}
							
			//Ð´Èë³É¹¦²Å¼ÆÊý
			pstMP4Mgr->m_uiVedioSampleID++;
			pstMP4Mgr->m_VedioTotalDuration += ullTmpVideoDuration;
			_pstMP4ConvertSession->m_ullVideoTotalDuration += ullTmpVideoDuration;
			if (1 == pstMP4Mgr->m_bIFrameFlag)
			{
				pstMP4Mgr->m_iIFrameCount++;
			}
			pMP4ISampleIndexHead->m_stExRecInfo.m_stSdvFileHead.FrameCount += 1;
			pMP4ISampleIndexHead->m_stExRecInfo.m_stSdvFileHead.m_uiFrameNoDiff = pMP4ISampleIndexHead->m_stExRecInfo.m_stSdvFileHead.FrameCount-1;
		}
		
		if((1 != pstMP4Mgr->m_bFirstSpsFlag)||(1 != pstMP4Mgr->m_bFirstPpsFlag))
		{
			td_printf(0xff0000, "%d: have not find SpsPps, go on!\n", __LINE__);
			iRet = MP4_CONVERT_ERROR_SPSPPS_CONTINUE;
			goto EXIT;
		}
		else
		{
			if(0 == pstMP4Mgr->m_bFoundSpsPpsFlag)
			{
				td_printf(0x12345678, "%d: have found SpsPps, seek from head again!\n", __LINE__);
				
				//ÒÆ¶¯µ½ÎÄ¼þÍ·Î»ÖÃ
				iSeekRet = td_mp4_fseek((void*)(pstMP4Mgr->m_pfSDVFileHandle), sizeof(S_FileHeader), SEEK_SET);
				
				if(-1 == iSeekRet)
				{
					td_printf(0xff0000, "%d: Seek failed! iSeekRet=%d. Error info[%s].\n", __LINE__, iSeekRet, strerror(errno));
					goto EXIT;
					
				}

				if(DIRBURN_WITHOUTSATA == _pstMP4ConvertSession->m_iConvertType)
				{					
					pucVideoTrackData = (unsigned char*)malloc(sizeof(TVideoTrackInfo)+stVideoTrackInfo.sequenceLen+stVideoTrackInfo.pictLen);
					if(NULL != pucVideoTrackData)
					{
						memcpy(pucVideoTrackData, &stVideoTrackInfo, sizeof(TVideoTrackInfo));
						memcpy(pucVideoTrackData+sizeof(TVideoTrackInfo), pSps, stVideoTrackInfo.sequenceLen);
						memcpy(pucVideoTrackData+sizeof(TVideoTrackInfo)+stVideoTrackInfo.sequenceLen, pPps, stVideoTrackInfo.pictLen);
									
						if(Inner_WriteBaseUnitToMP4(_pstMP4ConvertSession->m_stMP4Manager.m_MP4Handle,
							sizeof(TBaseUnit)+sizeof(TVideoTrackInfo)+stVideoTrackInfo.sequenceLen+stVideoTrackInfo.pictLen,
							sizeof(TVideoTrackInfo)+stVideoTrackInfo.sequenceLen+stVideoTrackInfo.pictLen,
							VDTT,(unsigned char*)pucVideoTrackData,sizeof(TVideoTrackInfo)+stVideoTrackInfo.sequenceLen+stVideoTrackInfo.pictLen) < 0)
						{
							td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
							goto EXIT;
						}
					}
					else
					{
						td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
						goto EXIT;
					}
				}
				else
				{
					if(MP4WriteBaseUnit(_pstMP4ConvertSession->m_stMP4Manager.m_MP4Handle
						, VDTT, 0, (unsigned char*)"uselessinfo", 11) < 0)
					{
						td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
						goto EXIT;
					}		
				}
				td_printf(0, "+++++++++>++++++++++++> 33333333333333333333333333");
				if(Inner_WriteSimpleInfoBaseUnit(pstMP4Mgr->m_MP4Handle, ENCT, _pstMP4ConvertSession->m_uiEncryptFlag) < 0)
				{
					td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
					goto EXIT;
				}
				if(Inner_WriteSimpleInfoBaseUnit(pstMP4Mgr->m_MP4Handle, ADET, _pstMP4ConvertSession->m_uiAudioType) < 0)
				{
					td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
					goto EXIT;
				}			
				td_printf(0, "+++++++++>++++++++++++> 44444444444444444444444444");
				pstMP4Mgr->m_bFoundSpsPpsFlag = 1;
				iRet = MP4_CONVERT_ERROR_SPSPPS_CONTINUE;
				goto EXIT;
			}
		}
		
	}
		
	iRet = td_success;	
EXIT:
	if(NULL != pucVideoTrackData)
	{
		free(pucVideoTrackData);
		pucVideoTrackData = NULL;
	}
	if(NULL != pSps)
	{
		free(pSps);
		pSps = NULL;
	}
	if(NULL != pPps)
	{
		free(pPps);
		pPps = NULL;
	}
	
#endif
	return iRet;  
	
}



static int Inner_ConvertAACToMP4Sample(TMP4ConvertSession* _pstMP4ConvertSession, unsigned char* _pucAacData, int _iAacDataSize)
{
	int iRet = td_failure;
	TADTSHeader stHeader = {0};
	unsigned char *pucAudioDataBegin = NULL;
	int iAudioDataLeftSize = 0;
	int iAccFrameCount = 0;
	int iTmpAudioDuration = 0;
	int i = 0;
	TMP4Mgr* pstMP4Mgr = NULL;	
	TAudioTrackInfo stAudioTrackInfo = {0};	
	
	if ((NULL==_pstMP4ConvertSession)|| (NULL==_pucAacData) || (_iAacDataSize<=0))
	{
		td_printf(0xff0000, "%d: Input parameters is error!\n", __LINE__);
		goto EXIT;
	}

	pstMP4Mgr = &(_pstMP4ConvertSession->m_stMP4Manager);

	if(MP4_INVALID_TRACK_ID == pstMP4Mgr->m_AudioTrackID)
	{
		pstMP4Mgr->m_AudioTrackID = MP4AddAudioTrack(pstMP4Mgr->m_MP4Handle, pstMP4Mgr->m_stSDVFileHead.AudioSample, 1024, MP4_MPEG2_AAC_LC_AUDIO_TYPE);
		if (pstMP4Mgr->m_AudioTrackID == MP4_INVALID_TRACK_ID)
		{
			td_printf(0xff0000, "%d: Call MP4AddAudioTrack failed!\n", __LINE__);
			goto EXIT;
		}
		
		MP4SetAudioProfileLevel(pstMP4Mgr->m_MP4Handle, 0x7f);
		MP4SetTrackDurationPerChunk(pstMP4Mgr->m_MP4Handle, pstMP4Mgr->m_AudioTrackID, 1024);
		MP4SetTrackLanguage(pstMP4Mgr->m_MP4Handle, pstMP4Mgr->m_AudioTrackID, "English");

		if(DIRBURN_WITHOUTSATA == _pstMP4ConvertSession->m_iConvertType)
		{
			stAudioTrackInfo.AudioType = AUDIO_TYPE_MEDIA_AAC;
			stAudioTrackInfo.ProfileLevel = 0x7f;
			stAudioTrackInfo.languagetype = ENGLISH;
			stAudioTrackInfo.timeScale = pstMP4Mgr->m_stSDVFileHead.AudioSample;
			stAudioTrackInfo.sampleDuration = 1024;

			if(Inner_WriteBaseUnitToMP4(_pstMP4ConvertSession->m_stMP4Manager.m_MP4Handle,
				sizeof(TBaseUnit)+sizeof(TAudioTrackInfo),
				sizeof(TAudioTrackInfo),
				ADTT,(unsigned char*)&stAudioTrackInfo,
				sizeof(TAudioTrackInfo)) < 0)
			{
				td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
				goto EXIT;
			}
		}
	}

	iTmpAudioDuration = pstMP4Mgr->m_AudioDuration;
	pucAudioDataBegin = _pucAacData;
	iAudioDataLeftSize = _iAacDataSize;

	iAccFrameCount = Inner_GetAacFrameCount(pucAudioDataBegin, iAudioDataLeftSize);

	if(iAccFrameCount <= 0)
	{
		td_printf(0xff0000, "%s %s %d the aac framecount is %d , error!", __FILE__, __FUNCTION__, __LINE__, iAccFrameCount);
		goto EXIT;
	}
	
	for(i = 0; i < iAccFrameCount; i++)
	{
		if(Inner_AnalyzeAdtsHead(&stHeader, pucAudioDataBegin, iAudioDataLeftSize) < 0)
		{
			td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
			goto EXIT;
		}

		if(stHeader.aac_frame_length <= 0)
		{
			td_printf(0xff0000, "%s %s %d the aac len is [%d] too short!error!", __FILE__, __FUNCTION__, __LINE__, stHeader.aac_frame_length);
			goto EXIT;
		}

		//td_printf(0, "iAccFrameCount is %d, stHeader.aac_frame_length is %d, iTmpAudioDuration is %d", iAccFrameCount, stHeader.aac_frame_length, iTmpAudioDuration);
		
		if(((pstMP4Mgr->m_stSDVFrameHead.reserved & 0x03) == 0x02))
		{
			if(stHeader.aac_frame_length > ADTS_HEADER_LENGTH)
			{
				pucAudioDataBegin[ADTS_HEADER_LENGTH] ^= 0xab;//AAC¼òµ¥¼ÓÃÜËã×Ó
			}
		}

		if(Inner_GetAndWriteMP4Data(_pstMP4ConvertSession, pucAudioDataBegin, stHeader.aac_frame_length, AUDIODATA, MP4_INVALID_DURATION, 1, 1, 0) < 0)
		{
			td_printf(0xff0000, "%d: Call Inner_GetAndWriteMP4Data failed![%d][%d]\n", __LINE__, pstMP4Mgr->m_AudioTrackID, stHeader.aac_frame_length);
			goto EXIT;
		}

		if(DIRBURN_WITHOUTSATA == _pstMP4ConvertSession->m_iConvertType)
		{
			if(Inner_WriteAVSampleInfoBaseUnit(_pstMP4ConvertSession->m_stMP4Manager.m_MP4Handle,
				-1, stHeader.aac_frame_length, 1024) < 0)
			{
				td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
				goto EXIT;
			}
		}

		//Ð´Èë³É¹¦²Å¼ÆÊý
		pstMP4Mgr->m_uiAudioSampleID++;
		pstMP4Mgr->m_AudioTotalDuration += iTmpAudioDuration;
		_pstMP4ConvertSession->m_ullAudioTotalDuration += iTmpAudioDuration;
		iTmpAudioDuration = 0;
		
		pucAudioDataBegin += stHeader.aac_frame_length;
		iAudioDataLeftSize -= stHeader.aac_frame_length;
	}
	
	iRet = td_success;
	
EXIT:	
	return iRet; 
}

static int Inner_ConvertG711AToMP4Sample(TMP4ConvertSession* _pstMP4ConvertSession, unsigned char *_pG711Adata, int _iDatasize)
{
	int iRet = td_failure;
	unsigned char *pucAudioDataBegin = NULL;
	int iTmpAudioDuration = 0;
	int iCount = 0;
	TMP4Mgr* pstMP4Mgr = NULL;	
	TAudioTrackInfo stAudioTrackInfo = {0};

	if ((_pstMP4ConvertSession == NULL) || (_pG711Adata == NULL) || (_iDatasize <= 0))
	{
		td_printf(0xff0000, "%d: Input parameters is error!\n", __LINE__);
		goto EXIT;
	}
		
	pstMP4Mgr = &(_pstMP4ConvertSession->m_stMP4Manager);
	
	if(_iDatasize != (pstMP4Mgr->m_stSDVFrameHead.u16AFrames)*(pstMP4Mgr->m_stSDVFrameHead.u16AFrameSize))
	{
		td_printf(0xff0000, "%s %s %d error!_iDatasize is %d, u16AFrames is %d, u16AFrameSize is %d", 
			__FILE__, __FUNCTION__, __LINE__, _iDatasize, pstMP4Mgr->m_stSDVFrameHead.u16AFrames, pstMP4Mgr->m_stSDVFrameHead.u16AFrameSize);
		goto EXIT;
	}

	if(MP4_INVALID_TRACK_ID == pstMP4Mgr->m_AudioTrackID)
	{
		if(((pstMP4Mgr->m_stSDVFrameHead.reserved & 0x03) != 0x02))
		{
			pstMP4Mgr->m_AudioTrackID = MP4AddALawAudioTrack(pstMP4Mgr->m_MP4Handle, pstMP4Mgr->m_stSDVFileHead.AudioSample);
			if (pstMP4Mgr->m_AudioTrackID == MP4_INVALID_TRACK_ID)
			{
				td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
				goto EXIT;
			}
			MP4SetAudioProfileLevel(pstMP4Mgr->m_MP4Handle, 0x2);
			
			if(DIRBURN_WITHOUTSATA == _pstMP4ConvertSession->m_iConvertType)
			{
				stAudioTrackInfo.AudioType = AUDIO_TYPE_G711A;
				stAudioTrackInfo.ProfileLevel = 0x2;
				stAudioTrackInfo.timeScale = pstMP4Mgr->m_stSDVFileHead.AudioSample;
				td_printf(0, "444 g711a audiotrack!");
				if(Inner_WriteBaseUnitToMP4(_pstMP4ConvertSession->m_stMP4Manager.m_MP4Handle,
					sizeof(TBaseUnit)+sizeof(TAudioTrackInfo),
					sizeof(TAudioTrackInfo),
					ADTT,(unsigned char*)&stAudioTrackInfo,
					sizeof(TAudioTrackInfo)) < 0)
				{
					td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
					goto EXIT;
				}
			}
		}
		else
		{
			pstMP4Mgr->m_AudioTrackID = MP4AddAudioTrack(pstMP4Mgr->m_MP4Handle, pstMP4Mgr->m_stSDVFileHead.AudioSample, pstMP4Mgr->m_stSDVFrameHead.u16AFrameSize-4, MP4_INVALID_AUDIO_TYPE);

			if (pstMP4Mgr->m_AudioTrackID == MP4_INVALID_TRACK_ID)
			{
				td_printf(0xff0000, "%d: Call MP4AddAudioTrack failed!\n", __LINE__);
				goto EXIT;
			}
						
			if(DIRBURN_WITHOUTSATA == _pstMP4ConvertSession->m_iConvertType)
			{
				stAudioTrackInfo.AudioType = AUDIO_TYPE_G711A;
				stAudioTrackInfo.timeScale = pstMP4Mgr->m_stSDVFileHead.AudioSample;
			
				if(Inner_WriteBaseUnitToMP4(_pstMP4ConvertSession->m_stMP4Manager.m_MP4Handle,
					sizeof(TBaseUnit)+sizeof(TAudioTrackInfo),
					sizeof(TAudioTrackInfo),
					ADTT,(unsigned char*)&stAudioTrackInfo,
					sizeof(TAudioTrackInfo)) < 0)
				{
					td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
					goto EXIT;
				}
			}
		}
	}

	iTmpAudioDuration = pstMP4Mgr->m_AudioDuration;
	pucAudioDataBegin = _pG711Adata;

	if((pstMP4Mgr->m_stSDVFrameHead.u16AFrameSize - 4) <= 0)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		goto EXIT;
	}

	//²ÉÓÃ¹Ì¶¨²ÉÑùµãÊý(²ÉÓÃÖ¡²î¼ÆËã£¬ÔÚ¶àÎÄ¼þÆ´½ÓÊ±Ð§¹û²»ºÃ)
	//iTmpAudioDuration = pstMP4Mgr->m_stSDVFrameHead.u16AFrameSize - 4;
		
	for(iCount = 0; iCount < pstMP4Mgr->m_stSDVFrameHead.u16AFrames; iCount++)
	{
		if(Inner_GetAndWriteMP4Data(_pstMP4ConvertSession, pucAudioDataBegin+4, pstMP4Mgr->m_stSDVFrameHead.u16AFrameSize - 4, AUDIODATA, iTmpAudioDuration, 0, 1, 0) < 0)
		{
			td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
			goto EXIT;
		}
		
		if(DIRBURN_WITHOUTSATA == _pstMP4ConvertSession->m_iConvertType)
		{
			if(Inner_WriteAVSampleInfoBaseUnit(_pstMP4ConvertSession->m_stMP4Manager.m_MP4Handle,
				-1, pstMP4Mgr->m_stSDVFrameHead.u16AFrameSize-4, iTmpAudioDuration) < 0)
			{
				td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
				goto EXIT;
			}
		}
		
		pstMP4Mgr->m_uiAudioSampleID++;
		pstMP4Mgr->m_AudioTotalDuration += iTmpAudioDuration;
		_pstMP4ConvertSession->m_ullAudioTotalDuration += iTmpAudioDuration;
		iTmpAudioDuration = 0;
		pucAudioDataBegin += pstMP4Mgr->m_stSDVFrameHead.u16AFrameSize;
	}

    iRet = td_success;
EXIT:
	return iRet;
}

static int Inner_ConvertG711UToMP4Sample(TMP4ConvertSession* _pstMP4ConvertSession, unsigned char *_pG711Udata, int _iDatasize)
{
	int iRet = td_failure;
	unsigned char *pucAudioDataBegin = NULL;
	int iTmpAudioDuration = 0;
	int iCount = 0;
	TMP4Mgr* pstMP4Mgr = NULL;
	TAudioTrackInfo stAudioTrackInfo = {0};

	if ((_pstMP4ConvertSession == NULL) || (_pG711Udata == NULL) || (_iDatasize <= 0))
	{
		td_printf(0xff0000, "%d: Input parameters is error!\n", __LINE__);
		goto EXIT;
	}


	pstMP4Mgr = &(_pstMP4ConvertSession->m_stMP4Manager);
	
	if(_iDatasize != (pstMP4Mgr->m_stSDVFrameHead.u16AFrames)*(pstMP4Mgr->m_stSDVFrameHead.u16AFrameSize))
	{
		td_printf(0xff0000, "%s %s %d error!_iDatasize is %d, u16AFrames is %d, u16AFrameSize is %d", 
			__FILE__, __FUNCTION__, __LINE__, _iDatasize, pstMP4Mgr->m_stSDVFrameHead.u16AFrames, pstMP4Mgr->m_stSDVFrameHead.u16AFrameSize);
		goto EXIT;
	}

	if(MP4_INVALID_TRACK_ID == pstMP4Mgr->m_AudioTrackID)
	{
		if(((pstMP4Mgr->m_stSDVFrameHead.reserved & 0x03) != 0x02))
		{
			pstMP4Mgr->m_AudioTrackID = MP4AddULawAudioTrack(pstMP4Mgr->m_MP4Handle, pstMP4Mgr->m_stSDVFileHead.AudioSample);
			if (pstMP4Mgr->m_AudioTrackID == MP4_INVALID_TRACK_ID)
			{
				td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
				goto EXIT;
			}
			MP4SetAudioProfileLevel(pstMP4Mgr->m_MP4Handle, 0x2);
			
			if(DIRBURN_WITHOUTSATA == _pstMP4ConvertSession->m_iConvertType)
			{
				stAudioTrackInfo.AudioType = AUDIO_TYPE_G711U;
				stAudioTrackInfo.ProfileLevel = 0x2;
				stAudioTrackInfo.timeScale = pstMP4Mgr->m_stSDVFileHead.AudioSample;
				td_printf(0, "555 g711u audiotrack!");
				if(Inner_WriteBaseUnitToMP4(_pstMP4ConvertSession->m_stMP4Manager.m_MP4Handle,
					sizeof(TBaseUnit)+sizeof(TAudioTrackInfo),
					sizeof(TAudioTrackInfo),
					ADTT,(unsigned char*)&stAudioTrackInfo,
					sizeof(TAudioTrackInfo)) < 0)
				{
					td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
					goto EXIT;
				}
			}
		}
		else
		{
			pstMP4Mgr->m_AudioTrackID = MP4AddAudioTrack(pstMP4Mgr->m_MP4Handle, pstMP4Mgr->m_stSDVFileHead.AudioSample, pstMP4Mgr->m_stSDVFrameHead.u16AFrameSize-4, MP4_PRIVATE_AUDIO_TYPE);

			if (pstMP4Mgr->m_AudioTrackID == MP4_INVALID_TRACK_ID)
			{
				td_printf(0xff0000, "%d: Call MP4AddAudioTrack failed!\n", __LINE__);
				goto EXIT;
			}
						
			if(DIRBURN_WITHOUTSATA == _pstMP4ConvertSession->m_iConvertType)
			{
				stAudioTrackInfo.AudioType = AUDIO_TYPE_G711U;
				stAudioTrackInfo.timeScale = pstMP4Mgr->m_stSDVFileHead.AudioSample;
			
				if(Inner_WriteBaseUnitToMP4(_pstMP4ConvertSession->m_stMP4Manager.m_MP4Handle,
					sizeof(TBaseUnit)+sizeof(TAudioTrackInfo),
					sizeof(TAudioTrackInfo),
					ADTT,(unsigned char*)&stAudioTrackInfo,
					sizeof(TAudioTrackInfo)) < 0)
				{
					td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
					goto EXIT;
				}
			}
		}
	}

	iTmpAudioDuration = pstMP4Mgr->m_AudioDuration;
	pucAudioDataBegin = _pG711Udata;

	if((pstMP4Mgr->m_stSDVFrameHead.u16AFrameSize - 4) <= 0)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		goto EXIT;
	}

	//²ÉÓÃ¹Ì¶¨²ÉÑùµãÊý(²ÉÓÃÖ¡²î¼ÆËã£¬ÔÚ¶àÎÄ¼þÆ´½ÓÊ±Ð§¹û²»ºÃ)
	//iTmpAudioDuration = pstMP4Mgr->m_stSDVFrameHead.u16AFrameSize - 4;
	
	for(iCount = 0; iCount < pstMP4Mgr->m_stSDVFrameHead.u16AFrames; iCount++)
	{
		if(Inner_GetAndWriteMP4Data(_pstMP4ConvertSession, pucAudioDataBegin+4, pstMP4Mgr->m_stSDVFrameHead.u16AFrameSize - 4, AUDIODATA, iTmpAudioDuration, 0, 1, 0) < 0)
		{
			td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
			goto EXIT;
		}

		if(DIRBURN_WITHOUTSATA == _pstMP4ConvertSession->m_iConvertType)
		{
			if(Inner_WriteAVSampleInfoBaseUnit(_pstMP4ConvertSession->m_stMP4Manager.m_MP4Handle,
				-1, pstMP4Mgr->m_stSDVFrameHead.u16AFrameSize - 4, iTmpAudioDuration) < 0)
			{
				td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
				goto EXIT;
			}
		}
		
		pstMP4Mgr->m_uiAudioSampleID++;
		pstMP4Mgr->m_AudioTotalDuration += iTmpAudioDuration;
		_pstMP4ConvertSession->m_ullAudioTotalDuration += iTmpAudioDuration;
		iTmpAudioDuration = 0;
		pucAudioDataBegin += pstMP4Mgr->m_stSDVFrameHead.u16AFrameSize;
	}
	
	iRet = td_success;
EXIT:
	return iRet;
}

static int Inner_ConvertAdpcmToMP4Sample(TMP4ConvertSession* _pstMP4ConvertSession, unsigned char* _pucAdpcmData, int _iAudioDataSize)
{
	int iRet = td_failure;
	unsigned char *pucAudioDataBegin = NULL;
	int iTmpAudioDuration = 0;
	TMP4Mgr* pstMP4Mgr = NULL;
	int iCount = 0;
	TAudioTrackInfo stAudioTrackInfo = {0};

	if ((_pstMP4ConvertSession == NULL) || (_pucAdpcmData == NULL) || (_iAudioDataSize <= 0))
	{
		td_printf(0xff0000, "%d: Input parameters is error!\n", __LINE__);
		goto EXIT;
	}

	pstMP4Mgr = &(_pstMP4ConvertSession->m_stMP4Manager);

	if(MP4_INVALID_TRACK_ID == pstMP4Mgr->m_AudioTrackID)
	{
		pstMP4Mgr->m_AudioTrackID = MP4AddAudioTrack(pstMP4Mgr->m_MP4Handle, pstMP4Mgr->m_stSDVFileHead.AudioSample, pstMP4Mgr->m_stSDVFrameHead.u16AFrameSize-4, MP4_PRIVATE_AUDIO_TYPE);
		if (pstMP4Mgr->m_AudioTrackID == MP4_INVALID_TRACK_ID)
		{
			td_printf(0xff0000, "%d: Call MP4AddAudioTrack failed!\n", __LINE__);
			goto EXIT;
		}
				
		if(DIRBURN_WITHOUTSATA == _pstMP4ConvertSession->m_iConvertType)
		{
			stAudioTrackInfo.AudioType = AUDIO_TYPE_DVI4;
			stAudioTrackInfo.timeScale = pstMP4Mgr->m_stSDVFileHead.AudioSample;

			if(Inner_WriteBaseUnitToMP4(_pstMP4ConvertSession->m_stMP4Manager.m_MP4Handle,
				sizeof(TBaseUnit)+sizeof(TAudioTrackInfo),
				sizeof(TAudioTrackInfo),
				ADTT,(unsigned char*)&stAudioTrackInfo,
				sizeof(TAudioTrackInfo)) < 0)
			{
				td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
				goto EXIT;
			}
		}
	}

	iTmpAudioDuration = pstMP4Mgr->m_AudioDuration;
	pucAudioDataBegin = _pucAdpcmData;

	if((pstMP4Mgr->m_stSDVFrameHead.u16AFrameSize - 4) <= 0)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		goto EXIT;
	}
	
	for(iCount = 0; iCount < pstMP4Mgr->m_stSDVFrameHead.u16AFrames; iCount++)
	{
		if(Inner_GetAndWriteMP4Data(_pstMP4ConvertSession, pucAudioDataBegin+4, pstMP4Mgr->m_stSDVFrameHead.u16AFrameSize - 4, AUDIODATA, iTmpAudioDuration, 0, 1, 0) < 0)
		{
			td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
			goto EXIT;
		}

		if(DIRBURN_WITHOUTSATA == _pstMP4ConvertSession->m_iConvertType)
		{
			if(Inner_WriteAVSampleInfoBaseUnit(_pstMP4ConvertSession->m_stMP4Manager.m_MP4Handle,
				-1, pstMP4Mgr->m_stSDVFrameHead.u16AFrameSize - 4, iTmpAudioDuration) < 0)
			{
				td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
				goto EXIT;
			}
		}
		
		pstMP4Mgr->m_uiAudioSampleID++;
		pstMP4Mgr->m_AudioTotalDuration += iTmpAudioDuration;
		_pstMP4ConvertSession->m_ullAudioTotalDuration += iTmpAudioDuration;
		iTmpAudioDuration = 0;
		pucAudioDataBegin += pstMP4Mgr->m_stSDVFrameHead.u16AFrameSize;
	}

	iRet = td_success;
EXIT:
	return iRet; 
}

static int Inner_ConvertAVDataToMP4Sample(TMP4ConvertSession* _pstMP4ConvertSession, 
	void* _pvSDVVedioBuf, int _iSDVVedioBufLen, void* _pvSDVAudioBuf, int _iSDVAudioBufLen)
{
	int iRet = td_failure;
	int iTmpRes = 0;
	void* pvSDVVedioBuf = NULL;
	int iSDVVedioBufLen = 0;
	void* pvSDVAudioBuf = NULL;
	int iSDVAudioBufLen = 0;
	TMP4ISampleIndexHead* pMP4ISampleIndexHead = NULL;
				
	if((NULL == _pstMP4ConvertSession) || (NULL == _pvSDVVedioBuf) ||(0 >= _iSDVVedioBufLen))
	{
		td_printf(0xff0000, "%d: Input parameters is error![%p][%p][%d]\n", __LINE__,_pstMP4ConvertSession,
			_pvSDVVedioBuf, _iSDVVedioBufLen);
		goto EXIT;
	}
	
	if( ((NULL == _pvSDVAudioBuf) && (0 < _iSDVAudioBufLen)) || ((NULL != _pvSDVAudioBuf) && (0 > _iSDVAudioBufLen)) )
	{
		td_printf(0xff0000, "%d: Input audio parameters is error![%p][%d]\n", __LINE__,_pvSDVAudioBuf,_iSDVAudioBufLen);
		goto EXIT;
	}

	pMP4ISampleIndexHead = (TMP4ISampleIndexHead*)(_pstMP4ConvertSession->m_puVirFrameBuf+MP4_BOX_SIEZ_LENGTH+MP4_SAMPLE_TYPE_LENGTH);

	pvSDVVedioBuf = _pvSDVVedioBuf;
	pvSDVAudioBuf = _pvSDVAudioBuf;
	iSDVVedioBufLen = _iSDVVedioBufLen;
	iSDVAudioBufLen = _iSDVAudioBufLen;	
	iTmpRes = Inner_ConvertVDataToMP4Sample(_pstMP4ConvertSession, (unsigned char*)pvSDVVedioBuf, iSDVVedioBufLen); 
	
	if (iTmpRes < 0)  
	{  
		td_printf(0xff0000, "%d: Call Inner_WriteH264! iTmpRes is %d\n", __LINE__, iTmpRes);
		iRet = iTmpRes;
		goto EXIT;
	}

	//ÊµÊ±¼ÇÂ¼×ÜÎÄ¼þ½áÊøÊ±¼ä
	pMP4ISampleIndexHead->m_stExRecInfo.m_iFileEndTime = _pstMP4ConvertSession->m_iFileBeginTime + (_pstMP4ConvertSession->m_ullVideoTotalDuration/90000);
	//td_printf(0, "==========>m_ullVideoTotalDuration is %llu, CurMP4BeginTime is %d, CurMP4EndTime is %d", 
		//_pstMP4ConvertSession->m_ullVideoTotalDuration,
		//_pstMP4ConvertSession->m_iFileBeginTime,
		//pMP4ISampleIndexHead->m_stExRecInfo.m_iFileEndTime);
	
	if(iSDVAudioBufLen > 0)
	{
		switch(_pstMP4ConvertSession->m_stMP4Manager.m_stSDVFrameHead.u8ACoder)
		{
			case AUDIO_TYPE_MEDIA_AAC:
			{
				iTmpRes = Inner_ConvertAACToMP4Sample(_pstMP4ConvertSession, (unsigned char*)pvSDVAudioBuf, iSDVAudioBufLen); 
				break;
			}
			case AUDIO_TYPE_G711A:
			{
				iTmpRes = Inner_ConvertG711AToMP4Sample(_pstMP4ConvertSession, (unsigned char*)pvSDVAudioBuf, iSDVAudioBufLen); 
				break;
			}
			case AUDIO_TYPE_G711U:
			{
				iTmpRes = Inner_ConvertG711UToMP4Sample(_pstMP4ConvertSession, (unsigned char*)pvSDVAudioBuf, iSDVAudioBufLen); 
				break;
			}
			case AUDIO_TYPE_DVI4:
			{
				iTmpRes = Inner_ConvertAdpcmToMP4Sample(_pstMP4ConvertSession, (unsigned char*)pvSDVAudioBuf, iSDVAudioBufLen); 
				break;
			}
			default:
			{
				td_printf(0xff0000, "%d: The Audio Type Is Not Support!\n", __LINE__);
			}
			
		}

		if (td_failure == iTmpRes)  
		{  
			td_printf(0xff0000, "%d: Call Write Audio data failed!\n", __LINE__);
		}
	}
	
	iRet = td_success;
	
EXIT:
	return iRet;	
}


static int Inner_FindAndSaveVirFrameInfo(TMP4ConvertSession* _pstMP4ConvertSession, int _iFinishFlag)
{
	int i = 0;
	int iTmpSdvICount = 0;
	int iThisFunRes = td_failure;	
	int iHaveMemNum = 0;
	TIFrameIndexHead* pstSDVFrameIndexHead = NULL;
	TIFrameIndexInfo* pstSDVFrameIndexBody = NULL;
	TAppendCharInfo*  pstSDVAppendCharInfo = NULL;
	TIFrameIndexTail* pstSDVFrameIndexTail = NULL;
	unsigned char* pucSDVVirtualFramebuffer = NULL;
	int iSDVVirtualFramBufLen = 0;
	TMP4Mgr* pstMP4Mgr = NULL;
	TMP4ISampleIndexHead* pMP4ISampleIndexHead = NULL;
	TMP4ISampleIndexInfo* pTmpMP4ISampleMem = NULL;
	
	if(NULL == _pstMP4ConvertSession)
	{
		td_printf(0xff0000, "%d: Input parameters is error!\n", __LINE__);
		goto LABLE_TD_MP4_EXIT;
	}

	pstMP4Mgr = &(_pstMP4ConvertSession->m_stMP4Manager);
	
	pucSDVVirtualFramebuffer = (unsigned char*)pstMP4Mgr->m_pvSDVVirtualFramebuffer;
	iSDVVirtualFramBufLen = pstMP4Mgr->m_uiSDVVirtualFramebufferSize;
	
	//²»ÊÇI Ö¡²»½øÐÐ´¦Àí
	if((1 != pstMP4Mgr->m_bIFrameFlag)&&(0 == _iFinishFlag))
	{
		iThisFunRes = td_success;
		goto LABLE_TD_MP4_EXIT;
	}
	
	if(0 == _iFinishFlag)
	{
		if(HAVE_SDV_VIRTUAL_FRAME_FLAG == pstMP4Mgr->m_bHaveSDVVirtualFrameFlag)
		{
			//»ñÈ¡SDV Ë÷ÒýÐÅÏ¢
			if(Inner_AnalyzeSDVVirtualFrameInfo((const void*)pucSDVVirtualFramebuffer, iSDVVirtualFramBufLen
								, &pstSDVFrameIndexHead, &pstSDVFrameIndexBody
								, &pstSDVAppendCharInfo, &pstSDVFrameIndexTail) < 0)
			{  
				td_printf(0xff0000, "%d: Call Inner_AnalyzeSDVVirtualFrameInfo failed!\n", __LINE__);
				goto LABLE_TD_MP4_EXIT;
			}
			
			iTmpSdvICount = pstSDVFrameIndexHead->m_iNum;

			td_printf(0x12345678, "Cur SDV IndexNum is -------------------> %d", iTmpSdvICount);

			if(iTmpSdvICount > 0)
			{
				i = pstMP4Mgr->m_iIFrameCount-1;
				
				if((i >= 0)&&(i < iTmpSdvICount))
				{
					pMP4ISampleIndexHead = (TMP4ISampleIndexHead*)(_pstMP4ConvertSession->m_puVirFrameBuf+MP4_BOX_SIEZ_LENGTH+MP4_SAMPLE_TYPE_LENGTH);
					pMP4ISampleIndexHead->m_stSdvIndexHead.m_iNum += 1;//×Ü¼ÆÊý¼Ó1

					td_printf(0, "1111111---------------------->pMP4ISampleIndexHead->m_stSdvIndexHead.m_iNum-1 is %d", pMP4ISampleIndexHead->m_stSdvIndexHead.m_iNum-1);

					if(pMP4ISampleIndexHead->m_stSdvIndexHead.m_iNum > pMP4ISampleIndexHead->m_stSdvIndexHead.m_iMemNum)
					{
						td_printf(0, "1111_pstMP4ConvertSession->m_puVirFrameBuf is %p", _pstMP4ConvertSession->m_puVirFrameBuf);
						pMP4ISampleIndexHead->m_stSdvIndexHead.m_iMemNum += MAX_RELLOC_MEMNUM;
						iHaveMemNum = pMP4ISampleIndexHead->m_stSdvIndexHead.m_iMemNum;//´Ë´¦ÒªÁÙÊ±±£´æ£¬·ÀÖ¹realloc ºó£¬µØÖ·¸Ä±äµ¼ÖÂÒì³£
						_pstMP4ConvertSession->m_puVirFrameBuf = (unsigned char*)realloc(_pstMP4ConvertSession->m_puVirFrameBuf, MP4_BOX_SIEZ_LENGTH+MP4_SAMPLE_TYPE_LENGTH+sizeof(TMP4ISampleIndexHead) + sizeof(TMP4ISampleIndexInfo)*iHaveMemNum + MP4_APPEND_CHAR_INFO_SIZE + sizeof(TIFrameIndexTail));

						if(NULL == _pstMP4ConvertSession->m_puVirFrameBuf)
						{
							td_printf(0xff0000, "%s %s %d error! NULL == m_puVirFrameBuf", __FILE__, __FUNCTION__, __LINE__);
							goto LABLE_TD_MP4_EXIT;
						}
						else
						{
							td_printf(0, "2222_pstMP4ConvertSession->m_puVirFrameBuf is %p", _pstMP4ConvertSession->m_puVirFrameBuf);
							pMP4ISampleIndexHead = (TMP4ISampleIndexHead*)(_pstMP4ConvertSession->m_puVirFrameBuf+MP4_BOX_SIEZ_LENGTH+MP4_SAMPLE_TYPE_LENGTH);
						}
					}

					td_printf(0, "tttttttttttttt");
					td_printf(0, "2222222---------------------->pMP4ISampleIndexHead->m_stSdvIndexHead.m_iNum-1 is %d", pMP4ISampleIndexHead->m_stSdvIndexHead.m_iNum-1);
					pTmpMP4ISampleMem = (TMP4ISampleIndexInfo*)(_pstMP4ConvertSession->m_puVirFrameBuf+MP4_BOX_SIEZ_LENGTH+MP4_SAMPLE_TYPE_LENGTH+sizeof(TMP4ISampleIndexHead));
					memcpy(&(pTmpMP4ISampleMem[pMP4ISampleIndexHead->m_stSdvIndexHead.m_iNum-1].m_stSdvIndexInfo), &pstSDVFrameIndexBody[i], sizeof(TIFrameIndexInfo));
					/*´Ë´¦µÄÊ±¼ä´Á£¬Èç¹ûÓÐÐÝÍ¥Ê±¼ä£¬ÒªÔõÃ´ÖØÐÂËãÒ»ÏÂ? */
					//ÒòÎªÃ»ÓÐÀ©Õ¹£¬ÔÝÊ±²»¼ÇÂ¼´Ë×Ö¶ÎÊµ¼ÊÖµ
					//pTmpMP4ISampleMem[pMP4ISampleIndexHead->m_stSdvIndexHead.m_iNum-1].m_stSdvIndexInfo.m_iPos += _pstMP4ConvertSession->m_llFileSize;//Ë÷ÒýÖ¡µÄÎ»ÖÃÐÅÏ¢Òª±ä¸ü
					//pTmpMP4ISampleMem[pMP4ISampleIndexHead->m_stSdvIndexHead.m_iNum-1].m_stSdvIndexInfo.m_iTimeNow = pMP4ISampleIndexHead->m_stExRecInfo.m_iFileEndTime;//ÖØÐÂ¼ÆËãË÷ÒýÐÅÏ¢µÄÊ±¼ä£¬·ÀÖ¹Ê±¼ä±ä»¯£¬¶¨Î»Ê§°Ü
					pTmpMP4ISampleMem[pMP4ISampleIndexHead->m_stSdvIndexHead.m_iNum-1].m_uiUselessCount = pstMP4Mgr->m_uiUselessCount;
					pTmpMP4ISampleMem[pMP4ISampleIndexHead->m_stSdvIndexHead.m_iNum-1].m_stVideoSampleIndexInfo.m_iSampleID = pstMP4Mgr->m_uiVedioSampleID;
					pTmpMP4ISampleMem[pMP4ISampleIndexHead->m_stSdvIndexHead.m_iNum-1].m_stVideoSampleIndexInfo.m_tSampleTimeStamp = pstMP4Mgr->m_VedioTotalDuration;
					pTmpMP4ISampleMem[pMP4ISampleIndexHead->m_stSdvIndexHead.m_iNum-1].m_stAudioSampleIndexInfo.m_iSampleID = pstMP4Mgr->m_uiAudioSampleID;
					pTmpMP4ISampleMem[pMP4ISampleIndexHead->m_stSdvIndexHead.m_iNum-1].m_stAudioSampleIndexInfo.m_tSampleTimeStamp = pstMP4Mgr->m_AudioTotalDuration;

					td_printf(0, "[%d] m_uiUselessCount[%d] m_uiVedioSampleID[%d] m_VedioTotalDuration[%llu] m_uiAudioSampleID[%d] m_AudioTotalDuration[%llu]", 
						pMP4ISampleIndexHead->m_stSdvIndexHead.m_iNum-1,
						pstMP4Mgr->m_uiUselessCount,
						pstMP4Mgr->m_uiVedioSampleID,
						pstMP4Mgr->m_VedioTotalDuration,
						pstMP4Mgr->m_uiAudioSampleID,
						pstMP4Mgr->m_AudioTotalDuration);
					//ÊµÊ±¼ÇÂ¼mp4 Ë÷ÒýÐÅÏ¢,¶ÏµçÐÞ¸´Ê±Ê¹ÓÃ
					if(Inner_WriteBaseUnitToMP4(_pstMP4ConvertSession->m_stMP4Manager.m_MP4Handle,
						sizeof(TBaseUnit)+sizeof(TMP4ISampleIndexInfo),
						sizeof(TMP4ISampleIndexInfo),
						VMFT,(unsigned char*)&(pTmpMP4ISampleMem[pMP4ISampleIndexHead->m_stSdvIndexHead.m_iNum-1]),sizeof(TMP4ISampleIndexInfo)) < 0)
					{
						td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
						goto LABLE_TD_MP4_EXIT;
					}
				}
				else
				{
					td_printf(0xff0000, "%d: pstMP4Mgr->m_iIFrameCount is error! i is %d, iTmpSdvICount is %d\n", __LINE__, i, iTmpSdvICount);
				}
			}
			else
			{	
				td_printf(0xff0000, "%s %s %d the sdv file have not virtual frame!", __FILE__, __FUNCTION__, __LINE__);
			}
		}
	}
	else
	{
		_pstMP4ConvertSession->m_ullFileSize += pstMP4Mgr->m_stSDVFileHead.TotalSize;
	}
		
	iThisFunRes = td_success;
	
LABLE_TD_MP4_EXIT:
		
	return iThisFunRes; 
}

static int Inner_CompleteMP4VirFrameInfo(unsigned char** _pstMP4VirInfoBuf,unsigned char** _pVitualFrameBuf, int* _iBufLen)
{
	int iRet = td_failure;
	int iPos = 0;
	int iInfoNum = 0;
	int iVirFrameSize = 0;
	unsigned char* pMP4VirInfoBuf = NULL;
	TMP4ISampleIndexHead* pstMP4ISampleIndexHead = NULL;
	TMP4ISampleIndexTail* pstMP4ISampleIndexTail = NULL;

	if(NULL == _pstMP4VirInfoBuf)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		goto Exit;
	}

	pMP4VirInfoBuf = *_pstMP4VirInfoBuf;
	pstMP4ISampleIndexHead = (TMP4ISampleIndexHead*)(pMP4VirInfoBuf+MP4_BOX_SIEZ_LENGTH+MP4_SAMPLE_TYPE_LENGTH);
	pstMP4ISampleIndexHead->m_stSdvIndexHead.m_iInfoSize = sizeof(TMP4ISampleIndexInfo);
	pstMP4ISampleIndexHead->m_stSdvIndexHead.m_stHead.u32FrameID = ntohl(0x00000002);
	pstMP4ISampleIndexHead->m_stSdvIndexHead.m_stHead.u8FrameType = FRAME_P;
	iInfoNum = pstMP4ISampleIndexHead->m_stSdvIndexHead.m_iNum;
	iPos = MP4_BOX_SIEZ_LENGTH+MP4_SAMPLE_TYPE_LENGTH+sizeof(TMP4ISampleIndexHead)+sizeof(TMP4ISampleIndexInfo)*iInfoNum;

#ifdef ZHENGFA
	iVirFrameSize = iPos + sizeof(TMP4ISampleIndexInfo) + MP4_APPEND_CHAR_INFO_SIZE + sizeof(TIFrameIndexTail);
	if((iInfoNum+1) > pstMP4ISampleIndexHead->m_stSdvIndexHead.m_iMemNum)
	{
		pstMP4ISampleIndexHead->m_stSdvIndexHead.m_iMemNum += MAX_RELLOC_MEMNUM;
		pMP4VirInfoBuf = (unsigned char*)realloc(pMP4VirInfoBuf, MP4_BOX_SIEZ_LENGTH+MP4_SAMPLE_TYPE_LENGTH+sizeof(TMP4ISampleIndexHead)+sizeof(TMP4ISampleIndexInfo)*(pstMP4ISampleIndexHead->m_stSdvIndexHead.m_iMemNum) + MP4_APPEND_CHAR_INFO_SIZE + sizeof(TIFrameIndexTail));

		if(NULL == pMP4VirInfoBuf)
		{
			td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
			goto Exit;
		}
		else
		{
			//ËùÓÐÖ®Ç°ÓëpMP4VirInfoBuf Ïà¹ØµÄÖØÖ¸Ò»ÏÂ
			pstMP4ISampleIndexHead = (TMP4ISampleIndexHead*)(pMP4VirInfoBuf+MP4_BOX_SIEZ_LENGTH+MP4_SAMPLE_TYPE_LENGTH);
		}
	}
	memset(pMP4VirInfoBuf+iPos, 0, sizeof(TMP4ISampleIndexInfo)+MP4_APPEND_CHAR_INFO_SIZE);
	iPos += (sizeof(TMP4ISampleIndexInfo)+MP4_APPEND_CHAR_INFO_SIZE);
#else
	iVirFrameSize = iPos + MP4_APPEND_CHAR_INFO_SIZE + sizeof(TIFrameIndexTail);
	memset(pMP4VirInfoBuf+iPos, 0, MP4_APPEND_CHAR_INFO_SIZE);
	iPos += MP4_APPEND_CHAR_INFO_SIZE;
#endif

	pstMP4ISampleIndexTail = (TMP4ISampleIndexTail*)(pMP4VirInfoBuf+iPos);
	
	pMP4VirInfoBuf[0] = (iVirFrameSize-MP4_BOX_SIEZ_LENGTH)>>24;
	pMP4VirInfoBuf[1] = ((iVirFrameSize-MP4_BOX_SIEZ_LENGTH)&0x00ff0000)>>16;
	pMP4VirInfoBuf[2] = ((iVirFrameSize-MP4_BOX_SIEZ_LENGTH)&0x0000ff00)>>8;
	pMP4VirInfoBuf[3] = (iVirFrameSize+-MP4_BOX_SIEZ_LENGTH)&0x000000ff;
	pMP4VirInfoBuf[4] = 0x06;

	pstMP4ISampleIndexTail->m_iFlag = REC_ISAMPLE_INDEX_MAGIC_NUMBER;
	pstMP4ISampleIndexTail->m_iSize = iVirFrameSize-MP4_BOX_SIEZ_LENGTH-MP4_SAMPLE_TYPE_LENGTH;
	
	td_printf(0x12345678, "pstMP4ISampleIndexTail->m_iSize is %d", pstMP4ISampleIndexTail->m_iSize);
	*_pVitualFrameBuf = pMP4VirInfoBuf;
	*_iBufLen         = iVirFrameSize;

	printfdataX("Complete VirFrameData",(char *)pMP4VirInfoBuf,iVirFrameSize);

	iRet = td_success;
Exit:
	return iRet;
}

static MP4FileHandle Inner_GetMP4FileHeader(int _iType, char* _pMP4FilePath, unsigned int _uiMdatSize)
{
	MP4FileHandle pMP4FileHandle = NULL;
	#ifdef FOR_MP4SDK
	#else
	pMP4FileHandle = MP4CreateRT(_pMP4FilePath, 1, 1, 1, 0, 0, 0, 0, _iType, _uiMdatSize, false, NULL, NULL);
	 
	if (MP4_INVALID_FILE_HANDLE == pMP4FileHandle)  
	{  
		td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
		goto Exit;
	}
	else
	{			
		MP4SetTimeScale(pMP4FileHandle, 90000);
	}

Exit:
	#endif
	return pMP4FileHandle;
}

static int Inner_GetMP4Tail(void** _pMP4Handle)
{	
	int iRet = td_failure;
#ifdef FOR_MP4SDK
#else
	if(NULL == _pMP4Handle)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		goto Exit;
	}
	
	MP4CloseRT(*_pMP4Handle, MP4_CLOSE_DO_NOT_COMPUTE_BITRATE, NULL, NULL);

	*_pMP4Handle = NULL;

	iRet = td_success;
Exit:
#endif
	return iRet;
}

static int Inner_SetFileStartAndStopTime(char* _pcFileName, TSdvFileTime* _pSdvFileTime)
{
	int iRet = td_failure;
	int iFileStartTime = 0;
	int iFileStopTime  = 0;
#ifdef TD_VFS
	TTdStat stStat;
#else
	struct stat stStat;
#endif

	char cTmpFullPath[FULL_PATH_LEN] = {0};
	char cTmpShort[SHORT_PATH_LEN] = {0};

	if(( NULL == _pcFileName) || (NULL == _pSdvFileTime))
	{
		td_printf(0xff0000,"%s %s %d error!",__FILE__, __FUNCTION__, __LINE__);
		goto LEAVE;
	}

	if(strlen((char *)_pcFileName) <= 29)
	{
		if (0/*rec_CompleteFileName((td_s8 *)_pcFileName, (td_s8 *)cTmpFullPath, NULL, NULL) != ERR_REC_SUCCESSFUL*/)//cwm_todo
		{
			td_printf(0xff0000,"%s %s %d error!",__FILE__, __FUNCTION__, __LINE__);
			goto LEAVE;
		}
	}
	else
	{
		strncpy((char*)cTmpFullPath, (char*)_pcFileName, FULL_PATH_LEN - 1);
	}

	td_printf(0, "cTmpFullPath before is %s", cTmpFullPath);

	if(td_mp4_stat((char*)cTmpFullPath, &stStat) != 0)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		goto LEAVE;
	}

#ifdef TD_VFS
	iFileStopTime = stStat.m_stat.st_mtime;//Ô­Â¼ÏñÎÄ¼þµÄ×îºóÐÞ¸ÄÊ±¼ä
#else
	iFileStopTime = stStat.st_mtime;
#endif

	Inner_extract_file_name((unsigned char *)cTmpFullPath);
	td_printf(0, "cTmpFullPath after is %s", cTmpFullPath);
	memcpy(cTmpShort,cTmpFullPath+6,14);	
	td_printf(0, "cTmpShort is %s", cTmpShort);
	iFileStartTime = Inner_StrToTime(cTmpShort);

	if(iFileStartTime < 0)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		goto LEAVE;
	}

	_pSdvFileTime->m_iStartTime = iFileStartTime;
	_pSdvFileTime->m_iEndTime   = iFileStopTime;

	td_printf(0, "------->file[%s] start : %d end : %d", cTmpFullPath, _pSdvFileTime->m_iStartTime, _pSdvFileTime->m_iEndTime);
		
	iRet = td_success;
LEAVE:
	return iRet;
}

static int Inner_ReadDataFromCD(TMP4ResumSession* _pMP4ResumSession, unsigned char* _ucBuf, unsigned int _uiExpectReadDataLen, unsigned int* _puiRealReadLen)
{
	int iRet = td_failure;
	unsigned int uiExpectReadDataLen = 0;
	unsigned int uiRealDataLen = -1;

	if((NULL == _ucBuf)||(NULL == _puiRealReadLen)||(NULL == _pMP4ResumSession)||(NULL == gBigFile_ReadDisk))
	{
		td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
		goto Exit;
	}
	
	//Bigfile_ReadDisk
	if((_pMP4ResumSession->m_ullCDAddress + _uiExpectReadDataLen) >= _pMP4ResumSession->m_ullCDSize)
	{
		uiExpectReadDataLen = _pMP4ResumSession->m_ullCDSize - _pMP4ResumSession->m_ullCDAddress;
	}
	else
	{
		uiExpectReadDataLen = _uiExpectReadDataLen;
	}
	
	uiRealDataLen = gBigFile_ReadDisk(_pMP4ResumSession->m_uiDevNo, _pMP4ResumSession->m_ullCDAddress, _ucBuf, uiExpectReadDataLen);

	if(uiRealDataLen != uiExpectReadDataLen)
	{
		td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
		goto Exit;
	}
	else
	{
		*_puiRealReadLen = uiRealDataLen;
		_pMP4ResumSession->m_ullCDAddress += uiRealDataLen;
		_pMP4ResumSession->m_uiCurBoxReadSum += uiRealDataLen;
		printfdataX("ReadDataFromCD", (char *)_ucBuf, uiRealDataLen);
	}

	iRet = td_success;
Exit:
	return iRet;
}

static int Inner_WriteLastBoxData(TMP4ResumSession* _pMP4ResumSession)
{
	int iRet = td_failure;
	unsigned int uiHaveWriteLen = 0;

	if(NULL == _pMP4ResumSession)
	{
		td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
		goto EXIT;
	}

	if(_pMP4ResumSession->m_uiLastBoxResumZero > 0)
	{
		if(NULL == _pMP4ResumSession->m_pLastResumBox)
		{
			_pMP4ResumSession->m_pLastResumBox = (unsigned char*)malloc(MP4_MAX_WRITELEN_ONCE);
			memset(_pMP4ResumSession->m_pLastResumBox, 0, MP4_MAX_WRITELEN_ONCE);
		}

		td_printf(0, "%s %d m_uiLastBoxResumZero is %u, m_uiCurMP4BoxSize is %u, m_ullCDAddress is %llu",
			__FUNCTION__, __LINE__, 
			_pMP4ResumSession->m_uiLastBoxResumZero, _pMP4ResumSession->m_uiCurMP4BoxSize, _pMP4ResumSession->m_ullCDAddress);

		while(uiHaveWriteLen < _pMP4ResumSession->m_uiLastBoxResumZero)
		{
			if((_pMP4ResumSession->m_uiLastBoxResumZero-uiHaveWriteLen)>MP4_MAX_WRITELEN_ONCE)
			{
				if(NULL != gBigFile_WirteBuffer)
				{
					gBigFile_WirteBuffer(_pMP4ResumSession->m_uiDevNo, (unsigned char*)(_pMP4ResumSession->m_pLastResumBox), MP4_MAX_WRITELEN_ONCE);
				}
		
				uiHaveWriteLen += MP4_MAX_WRITELEN_ONCE;
			}
			else
			{
				if(NULL != gBigFile_WirteBuffer)
				{
					gBigFile_WirteBuffer(_pMP4ResumSession->m_uiDevNo, (unsigned char*)(_pMP4ResumSession->m_pLastResumBox), _pMP4ResumSession->m_uiLastBoxResumZero-uiHaveWriteLen);
				}
		
				uiHaveWriteLen += (_pMP4ResumSession->m_uiLastBoxResumZero-uiHaveWriteLen);
			}
		}
	}

	iRet = td_success;
EXIT:
	return iRet;
}

static int Inner_AnalasyReadCDRetValue(TMP4ResumSession* _pMP4ResumSession, unsigned int _uiExpectLen, unsigned long long _ullReadLen, int _iType, int iNeedResumLastMdat)
{
	int iRet = MP4_DIRBURN_READCD_FAILED;

	//td_printf(0, "_ullExpectLen is %lld, _ullReadLenis %lld", _ullExpectLen, _ullReadLen);

	if(NULL == _pMP4ResumSession)
	{
		td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
		goto Exit;
	}
	
	switch(_iType)
	{
		case BOXHEAD_HALF:
		{
			if((_ullReadLen > 0)&&(_ullReadLen < _uiExpectLen))
			{
				if(iNeedResumLastMdat)
				{
					if(_pMP4ResumSession->m_uiCurMP4BoxSize - _pMP4ResumSession->m_uiCurBoxReadSum < 0)
					{
						td_printf(0xff0000, "%d m_uiCurMP4BoxSize is %u, m_uiCurBoxReadSum is %u", __LINE__, _pMP4ResumSession->m_uiCurMP4BoxSize, _pMP4ResumSession->m_uiCurBoxReadSum);
						iRet = MP4_DIRBURN_READCD_FAILED;
						goto Exit;
					}

					unsigned int uiMdatSizeBefore = 1;
					unsigned int uiMdatSizeAfter = 0;
					unsigned long long ullMdatSizeBigBefore = 16;
					unsigned long long ullMdatSizeBigAfter  = 0;
					unsigned char BoxHead[MP4_BOX_SIEZ_LENGTH+MP4_BOX_TYPE_LENGTH+MP4_BOX_SIZE_LENGTH_BIG] = {0};

					ConvertSort((unsigned char*)&uiMdatSizeAfter, uiMdatSizeBefore);
					ConvertSortLong((unsigned char*)&ullMdatSizeBigAfter, ullMdatSizeBigBefore);
					memcpy(BoxHead, &uiMdatSizeAfter, MP4_BOX_SIEZ_LENGTH);
					memcpy(BoxHead+MP4_BOX_SIEZ_LENGTH, "mdat", MP4_BOX_TYPE_LENGTH);
					memcpy(BoxHead+MP4_BOX_SIEZ_LENGTH+MP4_BOX_TYPE_LENGTH, &ullMdatSizeBigAfter, MP4_BOX_SIZE_LENGTH_BIG);
					td_printf(0x12345678, "%s %s %d Polish the last Box!------------------->Resum the BOXHEAD_HALF!", __FILE__, __FUNCTION__, __LINE__);
					
					if(NULL != gBigFile_WirteBuffer)
					{
						gBigFile_WirteBuffer(_pMP4ResumSession->m_uiDevNo, BoxHead+_ullReadLen, _uiExpectLen-_ullReadLen);
					}
			
					//¼ÆËã²¹³äÊý¾Ý¸öÊý
					_pMP4ResumSession->m_uiLastBoxResumSum = _uiExpectLen-_ullReadLen;
								
					iRet = MP4_DIRBURN_READCD_NOTENOUGH;
					goto Exit;
				}
				else
				{
					iRet = MP4_DIRBURN_READCD_NOTENOUGH;
					goto Exit;
				}
			}

			break;
		}
		case BOXHEAD_WHOL:
		{
			if((_ullReadLen > 0)&&(_ullReadLen < _uiExpectLen))
			{				
				if(iNeedResumLastMdat)
				{
					td_printf(0x12345678, "%s %s %d Polish the last Box!------------------->Resum the BOXHEAD_WHOL!", __FILE__, __FUNCTION__, __LINE__);

					unsigned long long ullMdatSizeBigBefore = 0;
					unsigned long long ullMdatSizeBigAfter	= 0;

					if(_pMP4ResumSession->m_uiCurMP4BoxSize < (MP4_BOX_SIEZ_LENGTH+MP4_BOX_TYPE_LENGTH+MP4_BOX_SIZE_LENGTH_BIG))
					{
						td_printf(0, "_ullReadLen is %llu", _ullReadLen);
						ullMdatSizeBigBefore = _pMP4ResumSession->m_uiCurMP4BoxSize;

						if(0 == ullMdatSizeBigBefore)
						{
							unsigned char* tmp = (unsigned char*)&ullMdatSizeBigBefore;
							tmp[0] = 0x10;//±£Ö¤mdat´óÐ¡²»Ð¡ÓÚ16
						}
						_pMP4ResumSession->m_uiCurMP4BoxSize = (unsigned int)ullMdatSizeBigBefore;
						td_printf(0, "%d m_uiCurMP4BoxSize after is %u", __LINE__, _pMP4ResumSession->m_uiCurMP4BoxSize);

						ConvertSortLong((unsigned char*)&ullMdatSizeBigAfter, ullMdatSizeBigBefore);
					}

					//¼ÆËã²¹³äÊý¾Ý¸öÊý
					if(_pMP4ResumSession->m_uiCurMP4BoxSize >= _pMP4ResumSession->m_uiCurBoxReadSum)
					{
						_pMP4ResumSession->m_uiLastBoxResumSum = _pMP4ResumSession->m_uiCurMP4BoxSize - _pMP4ResumSession->m_uiCurBoxReadSum;

						if(ullMdatSizeBigAfter > 0)
						{
							if(NULL != gBigFile_WirteBuffer)
							{
								gBigFile_WirteBuffer(_pMP4ResumSession->m_uiDevNo, (unsigned char*)&ullMdatSizeBigAfter+_ullReadLen, _uiExpectLen-_ullReadLen);
								_pMP4ResumSession->m_uiCurBoxReadSum += (_uiExpectLen-_ullReadLen);
							}
						}

						if(_pMP4ResumSession->m_uiCurMP4BoxSize >= _pMP4ResumSession->m_uiCurBoxReadSum)
						{
							_pMP4ResumSession->m_uiLastBoxResumZero = _pMP4ResumSession->m_uiCurMP4BoxSize - _pMP4ResumSession->m_uiCurBoxReadSum;
						}
						else
						{
							td_printf(0xff0000, "%d m_uiCurMP4BoxSize is %u, m_uiCurBoxReadSum is %u", __LINE__, _pMP4ResumSession->m_uiCurMP4BoxSize, _pMP4ResumSession->m_uiCurBoxReadSum);
							iRet = MP4_DIRBURN_READCD_FAILED;
							goto Exit;
						}
					}
					else
					{
						td_printf(0xff0000, "%d m_uiCurMP4BoxSize is %u, m_uiCurBoxReadSum is %u", __LINE__, _pMP4ResumSession->m_uiCurMP4BoxSize, _pMP4ResumSession->m_uiCurBoxReadSum);
						iRet = MP4_DIRBURN_READCD_FAILED;
						goto Exit;
					}

					if(Inner_WriteLastBoxData(_pMP4ResumSession) < 0)
					{
						iRet = MP4_DIRBURN_READCD_FAILED;
						goto Exit;
					}
					
					iRet = MP4_DIRBURN_READCD_NOTENOUGH;
					goto Exit;
				}
				else
				{
					iRet = MP4_DIRBURN_READCD_NOTENOUGH;
					goto Exit;
				}
			}
			
			break;
		}
		case SMALLMDAT:
		{
			if((_ullReadLen > 0)&&(_ullReadLen <= _uiExpectLen))
			{
				if(iNeedResumLastMdat)
				{
					td_printf(0x12345678, "%s %s %d Polish the small mdat!------------------->Resum the small mdat!", __FILE__, __FUNCTION__, __LINE__);
					unsigned int uiPos = 0;
					unsigned char BoxHead[MP4_BOX_SIEZ_LENGTH+MP4_BOX_TYPE_LENGTH] = {0};
					memcpy(BoxHead, &(_pMP4ResumSession->m_uiCurMP4BoxSize), MP4_BOX_SIEZ_LENGTH-_ullReadLen);
					uiPos += (MP4_BOX_SIEZ_LENGTH-_ullReadLen);
					memcpy(BoxHead+uiPos, "mdat", MP4_BOX_TYPE_LENGTH);
					uiPos += MP4_BOX_TYPE_LENGTH;

					if(NULL != gBigFile_WirteBuffer)
					{
						gBigFile_WirteBuffer(_pMP4ResumSession->m_uiDevNo, (unsigned char*)BoxHead, uiPos);
						_pMP4ResumSession->m_uiCurBoxReadSum += uiPos;
					}

					//¼ÆËã²¹³äÊý¾Ý¸öÊý
					if(_pMP4ResumSession->m_uiCurMP4BoxSize >= _pMP4ResumSession->m_uiCurBoxReadSum)
					{
						_pMP4ResumSession->m_uiLastBoxResumZero = _pMP4ResumSession->m_uiCurMP4BoxSize - _pMP4ResumSession->m_uiCurBoxReadSum;
						_pMP4ResumSession->m_uiLastBoxResumSum  = _pMP4ResumSession->m_uiLastBoxResumZero + uiPos;
					}
					else
					{
						td_printf(0xff0000, "%d m_uiCurMP4BoxSize is %u, m_uiCurBoxReadSum is %u", __LINE__, _pMP4ResumSession->m_uiCurMP4BoxSize, _pMP4ResumSession->m_uiCurBoxReadSum);
						iRet = MP4_DIRBURN_READCD_FAILED;
						goto Exit;
					}

					if(Inner_WriteLastBoxData(_pMP4ResumSession) < 0)
					{
						iRet = MP4_DIRBURN_READCD_FAILED;
						goto Exit;
					}
									
					iRet = MP4_DIRBURN_READCD_NOTENOUGH;
					goto Exit;
				}
				else
				{
					iRet = MP4_DIRBURN_READCD_NOTENOUGH;
					goto Exit;
				}
			}
			
			break;
		}
		case OTHER:
		{
			if((_ullReadLen > 0)&&(_ullReadLen < _uiExpectLen))
			{
				if(iNeedResumLastMdat)
				{					
					td_printf(0x12345678, "%s %s %d Polish the last mdat!------------------->Resum the other!", __FILE__, __FUNCTION__, __LINE__);

					//¼ÆËã²¹³äÊý¾Ý¸öÊý
					if(_pMP4ResumSession->m_uiCurMP4BoxSize >= _pMP4ResumSession->m_uiCurBoxReadSum)
					{
						_pMP4ResumSession->m_uiLastBoxResumSum = _pMP4ResumSession->m_uiCurMP4BoxSize - _pMP4ResumSession->m_uiCurBoxReadSum;
						_pMP4ResumSession->m_uiLastBoxResumZero = _pMP4ResumSession->m_uiCurMP4BoxSize - _pMP4ResumSession->m_uiCurBoxReadSum;
					}
					else
					{
						td_printf(0xff0000, "%d m_uiCurMP4BoxSize is %u, m_uiCurBoxReadSum is %u", __LINE__, _pMP4ResumSession->m_uiCurMP4BoxSize, _pMP4ResumSession->m_uiCurBoxReadSum);
						iRet = MP4_DIRBURN_READCD_FAILED;
						goto Exit;
					}

					if(Inner_WriteLastBoxData(_pMP4ResumSession) < 0)
					{
						iRet = MP4_DIRBURN_READCD_FAILED;
						goto Exit;
					}
										
					iRet = MP4_DIRBURN_READCD_NOTENOUGH;
					goto Exit;
				}
				else
				{
					iRet = MP4_DIRBURN_READCD_NOTENOUGH;
					goto Exit;
				}
			}
			
			break;
		}
		default:
		{
			break;
		}
	}
	iRet = td_success;
Exit:
	return iRet;
}

static int Inner_GetNextCustomMdat(TMP4ResumSession* _pMP4ResumSession, int _iNeedResumLastMdat)
{
	int iRet = MP4_DIRBURN_READCD_FAILED;
	unsigned long long ullBoxSizeBig = 0;
	unsigned int uiVersion = 0;
	unsigned long long ullReadLen = 0;
	unsigned int  uiExpectLen = 0;
	unsigned char ucBoxType[MP4_BOX_TYPE_LENGTH] = {0};

	if(NULL == _pMP4ResumSession)
	{
		td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
		goto Exit;
	}

	if(Inner_ReadDataFromCD(_pMP4ResumSession, _pMP4ResumSession->m_pDataBuf, MP4_CYSTOMBOX_HEAD_LEN, (unsigned int*)&ullReadLen) < 0)
	{
		td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
		goto Exit;
	}

	td_printf(0, "==========================>ullReadLen is %u", ullReadLen);

	//ÅÐ¶ÏÊÇ·ñÒÑ¶Áµ½Í·
	if(0 == ullReadLen)
	{
		td_printf(0, "%s %s %d Has Reach the End of File!", __FILE__, __FUNCTION__, __LINE__);
		iRet = MP4_DIRBURN_READCD_OVER;
		goto Exit;
	}
	
	//ÓÅÏÈÅÐ¶ÏÊÇ·ñÎªËð»µµÄmoov  »òÕß×îºóÒ»¸öfree
	if((ullReadLen > 0)&&(ullReadLen <= MP4_BOX_SIEZ_LENGTH))
	{
		_pMP4ResumSession->m_uiCurMP4BoxSize = 0;
		memcpy(&(_pMP4ResumSession->m_uiCurMP4BoxSize), _pMP4ResumSession->m_pDataBuf, ullReadLen);
		printfdataX("mdatsize",(char *)&(_pMP4ResumSession->m_uiCurMP4BoxSize),ullReadLen);
		ConvertSort((unsigned char*)&(_pMP4ResumSession->m_uiCurMP4BoxSize), _pMP4ResumSession->m_uiCurMP4BoxSize);
		td_printf(0, "00-------->m_uiCurMP4BoxSize is %d", _pMP4ResumSession->m_uiCurMP4BoxSize);
	
		//Èç¹ûÒÑ¾­±éÀú¹ýmoov£¬Ôòµ±Ç°box¿Ï¶¨Îªfree
		if(MP4_MOOV == _pMP4ResumSession->m_iMoovOrFree)
		{
			_pMP4ResumSession->m_iMoovOrFree = MP4_FREE;
			_pMP4ResumSession->m_uiMoovOrFreeLen = _pMP4ResumSession->m_uiCurBoxReadSum;
			iRet = MP4_DIRBURN_READCD_OVER;
			goto Exit;
		}
		//Èç¹ûbox´óÐ¡´óÓÚ1£¬Òª°´Ð¡ÎÄ¼þmdat ½øÐÐ²¹È«
		else if(_pMP4ResumSession->m_uiCurMP4BoxSize > 1)
		{
			iRet = Inner_AnalasyReadCDRetValue(_pMP4ResumSession, MP4_BOX_SIEZ_LENGTH, ullReadLen, SMALLMDAT, _iNeedResumLastMdat);
			goto Exit;
		}
	}
	else if((ullReadLen > MP4_BOX_SIEZ_LENGTH)&&(ullReadLen < MP4_CYSTOMBOX_HEAD_LEN))
	{
		memcpy(&(_pMP4ResumSession->m_uiCurMP4BoxSize), _pMP4ResumSession->m_pDataBuf, MP4_BOX_SIEZ_LENGTH);
		ConvertSort((unsigned char*)&(_pMP4ResumSession->m_uiCurMP4BoxSize), _pMP4ResumSession->m_uiCurMP4BoxSize);
		td_printf(0, "11-------->m_uiCurMP4BoxSize is %d", _pMP4ResumSession->m_uiCurMP4BoxSize);
	
		if(_pMP4ResumSession->m_uiCurMP4BoxSize > 1)
		{
			memset(ucBoxType, 0, MP4_BOX_TYPE_LENGTH);
			memcpy(ucBoxType, _pMP4ResumSession->m_pDataBuf+MP4_BOX_SIEZ_LENGTH, (ullReadLen - MP4_BOX_SIEZ_LENGTH));
			
			printfdataX("ucBoxType",(char *)ucBoxType,MP4_BOX_TYPE_LENGTH);
			if(0 == memcmp("moov", ucBoxType, (ullReadLen - MP4_BOX_SIEZ_LENGTH)))
			{
				_pMP4ResumSession->m_iMoovOrFree = MP4_MOOV;
				_pMP4ResumSession->m_uiMoovOrFreeLen = _pMP4ResumSession->m_uiCurBoxReadSum;
				iRet = MP4_DIRBURN_READCD_OVER;
				goto Exit;
			}
			else if(0 == memcmp("free", ucBoxType, (ullReadLen - MP4_BOX_SIEZ_LENGTH)))
			{
				_pMP4ResumSession->m_iMoovOrFree = MP4_FREE;
				_pMP4ResumSession->m_uiMoovOrFreeLen = _pMP4ResumSession->m_uiCurBoxReadSum;
				iRet = MP4_DIRBURN_READCD_OVER;
				goto Exit;
			}
		}
	}
	
	//ÒÔÉÏ¶¼²»·ûºÏ£¬ÔÙÅÐ¶ÏÊÇ·ñÎªËð»µµÄ×Ô¶¨ÒåMdat
	if(ullReadLen < MP4_CYSTOMBOX_HEAD_LEN)
	{		
		if(ullReadLen <= (MP4_BOX_SIEZ_LENGTH+MP4_BOX_TYPE_LENGTH))
		{
			_pMP4ResumSession->m_uiCurMP4BoxSize = MP4_BOX_SIEZ_LENGTH+MP4_BOX_TYPE_LENGTH+MP4_BOX_SIZE_LENGTH_BIG;
			td_printf(0, "1111-------->m_uiCurMP4BoxSize is %d", _pMP4ResumSession->m_uiCurMP4BoxSize);
			iRet = Inner_AnalasyReadCDRetValue(_pMP4ResumSession, MP4_BOX_SIEZ_LENGTH+MP4_BOX_TYPE_LENGTH+MP4_BOX_SIZE_LENGTH_BIG, ullReadLen, BOXHEAD_HALF, _iNeedResumLastMdat);
		}
		else if((ullReadLen > (MP4_BOX_SIEZ_LENGTH+MP4_BOX_TYPE_LENGTH))&&(ullReadLen < (MP4_BOX_SIEZ_LENGTH+MP4_BOX_TYPE_LENGTH+MP4_BOX_SIZE_LENGTH_BIG)))
		{
			memcpy(&ullBoxSizeBig, _pMP4ResumSession->m_pDataBuf+MP4_BOX_SIEZ_LENGTH+MP4_BOX_TYPE_LENGTH, ullReadLen-(MP4_BOX_SIEZ_LENGTH+MP4_BOX_TYPE_LENGTH));
			printfdataX("1111ullBoxSizeBig", (char*)&ullBoxSizeBig, MP4_BOX_SIZE_LENGTH_BIG);
			ConvertSortLong((unsigned char*)&ullBoxSizeBig, ullBoxSizeBig);
			_pMP4ResumSession->m_uiCurMP4BoxSize = (unsigned int)ullBoxSizeBig;
			iRet = Inner_AnalasyReadCDRetValue(_pMP4ResumSession, MP4_BOX_SIZE_LENGTH_BIG, ullReadLen-(MP4_BOX_SIEZ_LENGTH+MP4_BOX_TYPE_LENGTH), BOXHEAD_WHOL, _iNeedResumLastMdat);		
		}
		else 
		{
			memcpy(&ullBoxSizeBig, _pMP4ResumSession->m_pDataBuf+MP4_BOX_SIEZ_LENGTH+MP4_BOX_TYPE_LENGTH, MP4_BOX_SIZE_LENGTH_BIG);
			printfdataX("2222ullBoxSizeBig", (char*)&ullBoxSizeBig, MP4_BOX_SIZE_LENGTH_BIG);
			ConvertSortLong((unsigned char*)&ullBoxSizeBig, ullBoxSizeBig);
			_pMP4ResumSession->m_uiCurMP4BoxSize = (unsigned int)ullBoxSizeBig;
			iRet = Inner_AnalasyReadCDRetValue(_pMP4ResumSession, MP4_CYSTOMBOX_HEAD_LEN-MP4_BOX_SIEZ_LENGTH-MP4_BOX_TYPE_LENGTH-MP4_BOX_SIZE_LENGTH_BIG, ullReadLen-MP4_BOX_SIEZ_LENGTH-MP4_BOX_TYPE_LENGTH-MP4_BOX_SIZE_LENGTH_BIG, OTHER, _iNeedResumLastMdat);		
		}

		if(iRet < 0)
		{
			td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
			goto Exit;
		}
	}
	else
	{
		memset(ucBoxType, 0, MP4_BOX_TYPE_LENGTH);
		memcpy(ucBoxType, _pMP4ResumSession->m_pDataBuf+MP4_BOX_SIEZ_LENGTH, MP4_BOX_TYPE_LENGTH);

		//Èç¹û²»ÊÇmdat,  ÒÔÇ°4 ¸ö×Ö½ÚÎª´óÐ¡
		if(0 != memcmp("mdat", ucBoxType, MP4_BOX_TYPE_LENGTH))
		{
			memcpy(&(_pMP4ResumSession->m_uiCurMP4BoxSize), _pMP4ResumSession->m_pDataBuf, MP4_BOX_SIEZ_LENGTH);
			ConvertSort((unsigned char*)&(_pMP4ResumSession->m_uiCurMP4BoxSize),_pMP4ResumSession->m_uiCurMP4BoxSize);
		}
		else
		{
			memcpy(&ullBoxSizeBig, _pMP4ResumSession->m_pDataBuf+MP4_BOX_SIEZ_LENGTH+MP4_BOX_TYPE_LENGTH, MP4_BOX_SIZE_LENGTH_BIG);
			ConvertSortLong((unsigned char*)&ullBoxSizeBig, ullBoxSizeBig);
			_pMP4ResumSession->m_uiCurMP4BoxSize = (unsigned int)ullBoxSizeBig;
			td_printf(0, "2222-------->m_uiCurMP4BoxSize is %d", _pMP4ResumSession->m_uiCurMP4BoxSize);
			memcpy(&uiVersion, _pMP4ResumSession->m_pDataBuf+MP4_BOX_SIEZ_LENGTH+MP4_BOX_TYPE_LENGTH+MP4_BOX_SIZE_LENGTH_BIG+MP4_CUSTOM_MDATFLAG_LENGTH, MP4_MDAT_VERSION_LENGTH);
			ConvertSort((unsigned char*)&uiVersion, uiVersion);
		}	
	}
				
	//ÅÐ¶ÏÊÇ·ñÎª×Ô¶¨Òåmdat 
	if((0 != memcmp((char*)(_pMP4ResumSession->m_pDataBuf+MP4_BOX_SIEZ_LENGTH), "mdat", MP4_BOX_TYPE_LENGTH))||
		(0 != memcmp((char*)(_pMP4ResumSession->m_pDataBuf+MP4_BOX_SIEZ_LENGTH+MP4_BOX_TYPE_LENGTH+MP4_BOX_SIZE_LENGTH_BIG), SELF_DEFINE_FLAG, MP4_CUSTOM_MDATFLAG_LENGTH)))
	{
		td_printf(0xff0000, "%s %s %d the current box is not custom mdat!\n", __FILE__, __FUNCTION__, __LINE__);
		//Ô½¹ý´ËBOX
		ullReadLen   =  _pMP4ResumSession->m_ullCDSize - _pMP4ResumSession->m_ullCDAddress;
		uiExpectLen  =  _pMP4ResumSession->m_uiCurMP4BoxSize - _pMP4ResumSession->m_uiCurBoxReadSum;
		_pMP4ResumSession->m_ullCDAddress += uiExpectLen;

		memset(ucBoxType, 0, MP4_BOX_TYPE_LENGTH);
		memcpy(ucBoxType, _pMP4ResumSession->m_pDataBuf+MP4_BOX_SIEZ_LENGTH, MP4_BOX_TYPE_LENGTH);

		if(_pMP4ResumSession->m_ullCDAddress >= _pMP4ResumSession->m_ullCDSize)
		{
			_pMP4ResumSession->m_uiCurBoxReadSum += ullReadLen;

			//Èç¹û²»ÊÇ×Ô¶¨Òåmdat£¬ÅÐ¶ÏÊÇ·ñÎªmoov »òfree
			if(0 == memcmp("moov", ucBoxType, MP4_BOX_TYPE_LENGTH))
			{
				_pMP4ResumSession->m_iMoovOrFree = MP4_MOOV;
				_pMP4ResumSession->m_uiMoovOrFreeLen = _pMP4ResumSession->m_uiCurBoxReadSum;
			}
			else if(0 == memcmp("free", ucBoxType, MP4_BOX_TYPE_LENGTH))
			{
				_pMP4ResumSession->m_iMoovOrFree = MP4_FREE;
				_pMP4ResumSession->m_uiMoovOrFreeLen = _pMP4ResumSession->m_uiCurBoxReadSum;
			}
			else
			{
				iRet = Inner_AnalasyReadCDRetValue(_pMP4ResumSession, uiExpectLen, ullReadLen, OTHER, _iNeedResumLastMdat);
				
				if(iRet < 0)
				{
					td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
					goto Exit;
				}
			}
			iRet = MP4_DIRBURN_READCD_OVER;
			goto Exit;
		}
		else
		{
			if(0 == memcmp("moov", ucBoxType, MP4_BOX_TYPE_LENGTH))
			{
				_pMP4ResumSession->m_iMoovOrFree = MP4_MOOV;
				_pMP4ResumSession->m_uiMoovOrFreeLen = _pMP4ResumSession->m_uiCurMP4BoxSize;
			}
		}
		iRet = MP4_DIRBURN_NOT_CONFORM;
		goto Exit;
	}
			
	if(1 != uiVersion)
	{
		td_printf(0xff0000, "%s %s %d error! Version[%d] is ERROR\n", __FILE__, __FUNCTION__, __LINE__, uiVersion);
	}
	
	iRet = td_success;
Exit:
	return iRet;
}

static int Inner_AnalasyOneBaseUnit(TMP4ResumSession* _pMP4ResumSession, char* _pBaseUnitType, unsigned int* _puiPos, unsigned int _uiBaseUnitSize, unsigned int _uiBaseMemSize)
{
	int iRet = td_failure;
#ifdef FOR_MP4SDK
#else
	int i = 0;
	unsigned int uiPos = 0;
	unsigned int uiMemNum = 0;
	TMP4ISampleIndexHead* pTMP4ISampleIndexHead = NULL;

	if((NULL == _pMP4ResumSession)||(NULL == _pBaseUnitType)||(NULL == _puiPos))
	{
		td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
		goto Exit;
	}

	pTMP4ISampleIndexHead = (TMP4ISampleIndexHead*)(_pMP4ResumSession->m_puVirFrameBuf+MP4_BOX_SIEZ_LENGTH+MP4_SAMPLE_TYPE_LENGTH);
	td_printf(0, "aa pTMP4ISampleIndexHead->m_stSdvIndexHead.m_iNum-1 is %d", pTMP4ISampleIndexHead->m_stSdvIndexHead.m_iNum-1);
	uiPos = *_puiPos;

	//·ÖÎöVDTT
	if(0 == memcmp(_pBaseUnitType, VIDEO_TRACK_INFO_TYPE, MP4_BOX_TYPE_LENGTH))
	{
		td_printf(0, "---------------------------------------------}}}}video track info");

		if(sizeof(TVideoTrackInfo) > (_uiBaseUnitSize - sizeof(TBaseUnit)))
		{
			td_printf(0xff0000, "%s %s %d error! _uiBaseUnitSize is %u\n", __FILE__, __FUNCTION__, __LINE__, _uiBaseUnitSize);
			iRet = MP4_DIRBURN_READCD_FAILED;
			goto Exit;
		}

		memcpy(&(_pMP4ResumSession->m_VideoTrackInfo), _pMP4ResumSession->m_pDataBuf+uiPos, sizeof(TVideoTrackInfo));
		uiPos += sizeof(TVideoTrackInfo);
		_pMP4ResumSession->m_pVideoTrackInfo = (unsigned char*)malloc(_uiBaseUnitSize - sizeof(TBaseUnit));

		if(NULL == _pMP4ResumSession->m_pVideoTrackInfo)
		{
			td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
			iRet = MP4_DIRBURN_READCD_FAILED;
			goto Exit;
		}
		
		memcpy(_pMP4ResumSession->m_pVideoTrackInfo, &(_pMP4ResumSession->m_VideoTrackInfo), sizeof(TVideoTrackInfo));
		memcpy(_pMP4ResumSession->m_pVideoTrackInfo+sizeof(TVideoTrackInfo), _pMP4ResumSession->m_pDataBuf+uiPos, (_pMP4ResumSession->m_VideoTrackInfo.sequenceLen + _pMP4ResumSession->m_VideoTrackInfo.pictLen));
		uiPos += (_pMP4ResumSession->m_VideoTrackInfo.sequenceLen + _pMP4ResumSession->m_VideoTrackInfo.pictLen);

		//¼ÇÂ¼Ïà¹ØÐÅÏ¢´æÔÚsdvµÄÎÄ¼þÍ·½á¹¹ÖÐ(»Ø·ÅÊ±Ê¹ÓÃ)
		pTMP4ISampleIndexHead->m_stExRecInfo.m_stSdvFileHead.Width  = _pMP4ResumSession->m_VideoTrackInfo.width;
		pTMP4ISampleIndexHead->m_stExRecInfo.m_stSdvFileHead.Height = _pMP4ResumSession->m_VideoTrackInfo.height;
		pTMP4ISampleIndexHead->m_stExRecInfo.m_stSdvFileHead.FrameRate = 90000/_pMP4ResumSession->m_VideoTrackInfo.sampleDuration;

		if(MP4_INVALID_TRACK_ID == _pMP4ResumSession->m_VideoTrackID)
		{
			_pMP4ResumSession->m_VideoTrackID = MP4AddH264VideoTrack(_pMP4ResumSession->m_MP4Handle, _pMP4ResumSession->m_VideoTrackInfo.timeScale, _pMP4ResumSession->m_VideoTrackInfo.sampleDuration, 
				_pMP4ResumSession->m_VideoTrackInfo.width,_pMP4ResumSession->m_VideoTrackInfo.height, 
				_pMP4ResumSession->m_VideoTrackInfo.AVCProfileIndication,  
				_pMP4ResumSession->m_VideoTrackInfo.profile_compat, 
				_pMP4ResumSession->m_VideoTrackInfo.AVCLevelIndication,
				_pMP4ResumSession->m_VideoTrackInfo.sampleLenFieldSizeMinusOne);

			if(MP4_INVALID_TRACK_ID == _pMP4ResumSession->m_VideoTrackID)
			{
				td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
				goto Exit;
			}

			MP4SetVideoProfileLevel(_pMP4ResumSession->m_MP4Handle, 0x01);
			MP4AddH264SequenceParameterSet(_pMP4ResumSession->m_MP4Handle,_pMP4ResumSession->m_VideoTrackID, _pMP4ResumSession->m_pVideoTrackInfo+sizeof(TVideoTrackInfo), _pMP4ResumSession->m_VideoTrackInfo.sequenceLen); 
			MP4AddH264PictureParameterSet(_pMP4ResumSession->m_MP4Handle,_pMP4ResumSession->m_VideoTrackID, _pMP4ResumSession->m_pVideoTrackInfo+sizeof(TVideoTrackInfo)+_pMP4ResumSession->m_VideoTrackInfo.sequenceLen, _pMP4ResumSession->m_VideoTrackInfo.pictLen);

			if(Inner_WriteBaseUnitToMP4(_pMP4ResumSession->m_MP4Handle,
				_uiBaseUnitSize,
				_uiBaseMemSize,
				VDTT,(unsigned char*)(_pMP4ResumSession->m_pVideoTrackInfo),
				_uiBaseUnitSize - sizeof(TBaseUnit)) < 0)
			{
				td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
				goto Exit;
			}
		}
		td_printf(0, "bb pTMP4ISampleIndexHead->m_stSdvIndexHead.m_iNum-1 is %d", pTMP4ISampleIndexHead->m_stSdvIndexHead.m_iNum-1);
	}
	else if(0 == memcmp((char*)_pBaseUnitType, AUDIO_TRACK_INFO_TYPE, MP4_BOX_TYPE_LENGTH))
	{
		td_printf(0, "---------------------------------------------}}}}audio track info");
		memcpy(&(_pMP4ResumSession->m_AudioTrackInfo), _pMP4ResumSession->m_pDataBuf+uiPos, sizeof(TAudioTrackInfo));
		uiPos += sizeof(TAudioTrackInfo);
		
		pTMP4ISampleIndexHead->m_stExRecInfo.m_stSdvFileHead.AudioSample = _pMP4ResumSession->m_AudioTrackInfo.timeScale;
		
		if(MP4_INVALID_TRACK_ID == _pMP4ResumSession->m_AudioTrackID)
		{
			switch(_pMP4ResumSession->m_AudioTrackInfo.AudioType)
			{
				case AUDIO_TYPE_MEDIA_AAC:
				{
					pTMP4ISampleIndexHead->m_stExRecInfo.m_stSdvFileHead.AChannels = 2;
					_pMP4ResumSession->m_AudioType = AUDIO_TYPE_MEDIA_AAC;
					_pMP4ResumSession->m_AudioTrackID = MP4AddAudioTrack(_pMP4ResumSession->m_MP4Handle, _pMP4ResumSession->m_AudioTrackInfo.timeScale, _pMP4ResumSession->m_AudioTrackInfo.sampleDuration, MP4_MPEG2_AAC_LC_AUDIO_TYPE);
		
					if (_pMP4ResumSession->m_AudioTrackID == MP4_INVALID_TRACK_ID)
					{
						td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
						goto Exit;
					}
		
					MP4SetAudioProfileLevel(_pMP4ResumSession->m_MP4Handle, _pMP4ResumSession->m_AudioTrackInfo.ProfileLevel);
					MP4SetTrackDurationPerChunk(_pMP4ResumSession->m_MP4Handle, _pMP4ResumSession->m_AudioTrackID, _pMP4ResumSession->m_AudioTrackInfo.sampleDuration);
					MP4SetTrackLanguage(_pMP4ResumSession->m_MP4Handle, _pMP4ResumSession->m_AudioTrackID, "English");
					break;
				}
				case AUDIO_TYPE_G711A:
				{
					pTMP4ISampleIndexHead->m_stExRecInfo.m_stSdvFileHead.AChannels = 1;
					_pMP4ResumSession->m_AudioType = AUDIO_TYPE_G711A;
					_pMP4ResumSession->m_AudioTrackID = MP4AddALawAudioTrack(_pMP4ResumSession->m_MP4Handle, _pMP4ResumSession->m_AudioTrackInfo.timeScale);
					
					if (_pMP4ResumSession->m_AudioTrackID == MP4_INVALID_TRACK_ID)
					{
						td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
						goto Exit;
					}
					MP4SetAudioProfileLevel(_pMP4ResumSession->m_MP4Handle, _pMP4ResumSession->m_AudioTrackInfo.ProfileLevel);
					break;
				}
				case AUDIO_TYPE_G711U:
				{
					pTMP4ISampleIndexHead->m_stExRecInfo.m_stSdvFileHead.AChannels = 1;
					_pMP4ResumSession->m_AudioType = AUDIO_TYPE_G711U;
					_pMP4ResumSession->m_AudioTrackID = MP4AddULawAudioTrack(_pMP4ResumSession->m_MP4Handle, _pMP4ResumSession->m_AudioTrackInfo.timeScale);
		
					if (_pMP4ResumSession->m_AudioTrackID == MP4_INVALID_TRACK_ID)
					{
						td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
						goto Exit;
					}
					MP4SetAudioProfileLevel(_pMP4ResumSession->m_MP4Handle, _pMP4ResumSession->m_AudioTrackInfo.ProfileLevel);
					break;
				}
				case AUDIO_TYPE_DVI4:			
				{
					pTMP4ISampleIndexHead->m_stExRecInfo.m_stSdvFileHead.AChannels = 1;
					_pMP4ResumSession->m_AudioType = AUDIO_TYPE_DVI4;
					_pMP4ResumSession->m_AudioTrackID = MP4AddAudioTrack(_pMP4ResumSession->m_MP4Handle, _pMP4ResumSession->m_AudioTrackInfo.timeScale, 512, MP4_PCM16_LITTLE_ENDIAN_AUDIO_TYPE);
		
					if (_pMP4ResumSession->m_AudioTrackID == MP4_INVALID_TRACK_ID)
					{
						td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
						goto Exit;
					}
					MP4SetAudioProfileLevel(_pMP4ResumSession->m_MP4Handle, _pMP4ResumSession->m_AudioTrackInfo.ProfileLevel);
					break;
				}
				default:
				{
					td_printf(0xff0000, "Don't support the AudioType!");
					break;
				}
			}

			if(Inner_WriteBaseUnitToMP4(_pMP4ResumSession->m_MP4Handle,
				_uiBaseUnitSize,
				_uiBaseMemSize,
				ADTT,(unsigned char*)&(_pMP4ResumSession->m_AudioTrackInfo),
				_uiBaseUnitSize - sizeof(TBaseUnit)) < 0)
			{
				td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
				goto Exit;
			}
		}
		td_printf(0, "cc pTMP4ISampleIndexHead->m_stSdvIndexHead.m_iNum-1 is %d", pTMP4ISampleIndexHead->m_stSdvIndexHead.m_iNum-1);
	}
	else if(0 == memcmp((char*)_pBaseUnitType, AV_SAMPLE_INFO_TYPE, MP4_BOX_TYPE_LENGTH))
	{		
		td_printf(0, "---------------------------------------------}}}}avsample info");

		if(_uiBaseMemSize != sizeof(TAVSampleInfo))
		{
			td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
			goto Exit;
		}
		
		uiMemNum = (_uiBaseUnitSize - sizeof(TBaseUnit))/_uiBaseMemSize;

		td_printf(0, "uiMemNum is %d", uiMemNum);

		TAVSampleInfo* pAVSampleInfo = NULL;	
		
		for(i = 0; i < uiMemNum; i++)
		{
			pAVSampleInfo = (TAVSampleInfo*)(_pMP4ResumSession->m_pDataBuf+uiPos);
		
			if(-1 == pAVSampleInfo->m_iFrameFlag)//ÒôÆµÐÅÏ¢
			{
				if(0 == pTMP4ISampleIndexHead->m_stExRecInfo.m_stSdvFileHead.bAudio)
				{
					pTMP4ISampleIndexHead->m_stExRecInfo.m_stSdvFileHead.bAudio = 1;
				}

				if(MP4_INVALID_TRACK_ID != _pMP4ResumSession->m_AudioTrackID)
				{
					if(AUDIO_TYPE_MEDIA_AAC == _pMP4ResumSession->m_AudioType)
					{
						//td_printf(0, "+++++duibi-------->MP4WriteSampleRT iDataLen[%d] _ullDataDuration[%lld] isSyncSample[%d]", pAVSampleInfo->m_uiAVSizeAndDuring.uiSampleSize, pAVSampleInfo->m_uiAVSizeAndDuring.sampleDuration, 1);
						if(!MP4WriteSampleRT(_pMP4ResumSession->m_MP4Handle, _pMP4ResumSession->m_AudioTrackID, NULL, pAVSampleInfo->m_uiAVSizeAndDuring.uiSampleSize, pAVSampleInfo->m_uiAVSizeAndDuring.sampleDuration, 1, 1, 0, NULL, NULL))	
						{
							td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
							goto Exit;
						}
					}
					else
					{
						//td_printf(0, "+++++duibi-------->MP4WriteSampleRT iDataLen[%d] _ullDataDuration[%lld] isSyncSample[%d]", pAVSampleInfo->m_uiAVSizeAndDuring.uiSampleSize, pAVSampleInfo->m_uiAVSizeAndDuring.sampleDuration, 0);
						if(!MP4WriteSampleRT(_pMP4ResumSession->m_MP4Handle, _pMP4ResumSession->m_AudioTrackID, NULL, pAVSampleInfo->m_uiAVSizeAndDuring.uiSampleSize, pAVSampleInfo->m_uiAVSizeAndDuring.sampleDuration, 0, 1, 0, NULL, NULL))	
						{
							td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
							goto Exit;
						}
					}

					_pMP4ResumSession->m_ullAudioTotalDuration += pAVSampleInfo->m_uiAVSizeAndDuring.sampleDuration;
				}
			}
			else if((0 == pAVSampleInfo->m_iFrameFlag)||(1 == pAVSampleInfo->m_iFrameFlag))//ÊÓÆµÐÅÏ¢
			{
				pTMP4ISampleIndexHead->m_stExRecInfo.m_stSdvFileHead.FrameCount += 1;
				pTMP4ISampleIndexHead->m_stExRecInfo.m_stSdvFileHead.m_uiFrameNoDiff = 
					pTMP4ISampleIndexHead->m_stExRecInfo.m_stSdvFileHead.FrameCount-1;

				if(MP4_INVALID_TRACK_ID != _pMP4ResumSession->m_VideoTrackID)
				{
					//td_printf(0, "+++++duibi-------->MP4WriteSampleRT iDataLen[%d] _ullDataDuration[%lld] isSyncSample[%d]", pAVSampleInfo->m_uiAVSizeAndDuring.uiSampleSize, pAVSampleInfo->m_uiAVSizeAndDuring.sampleDuration, pAVSampleInfo->m_iFrameFlag);
					if(!MP4WriteSampleRT(_pMP4ResumSession->m_MP4Handle, _pMP4ResumSession->m_VideoTrackID, NULL, pAVSampleInfo->m_uiAVSizeAndDuring.uiSampleSize, pAVSampleInfo->m_uiAVSizeAndDuring.sampleDuration, 0, pAVSampleInfo->m_iFrameFlag, 0, NULL, NULL))	
					{
						td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
						goto Exit;
					}

					_pMP4ResumSession->m_ullVideoTotalDuration += pAVSampleInfo->m_uiAVSizeAndDuring.sampleDuration;
				}
			}
			else if(6 == pAVSampleInfo->m_iFrameFlag)
			{
				if(MP4_INVALID_TRACK_ID != _pMP4ResumSession->m_VideoTrackID)
				{
					//td_printf(0, "+++++duibi-------->MP4WriteSampleRT iDataLen[%d] _ullDataDuration[%lld] isSyncSample[%d]", pAVSampleInfo->m_uiAVSizeAndDuring.uiSampleSize, pAVSampleInfo->m_uiAVSizeAndDuring.sampleDuration, pAVSampleInfo->m_iFrameFlag);
					if(!MP4WriteSampleRT(_pMP4ResumSession->m_MP4Handle, _pMP4ResumSession->m_VideoTrackID, NULL, pAVSampleInfo->m_uiAVSizeAndDuring.uiSampleSize, pAVSampleInfo->m_uiAVSizeAndDuring.sampleDuration, 0, 0, 0, NULL, NULL))	
					{
						td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
						goto Exit;
					}

					_pMP4ResumSession->m_ullVideoTotalDuration += pAVSampleInfo->m_uiAVSizeAndDuring.sampleDuration;
				}
			}
			else
			{
				td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
				goto Exit;
			}
		
			uiPos += sizeof(TAVSampleInfo);

			if(Inner_WriteBaseUnitToMP4(_pMP4ResumSession->m_MP4Handle,
				sizeof(TBaseUnit)+_uiBaseMemSize,
				_uiBaseMemSize,
				AVST,(unsigned char*)pAVSampleInfo,
				_uiBaseMemSize) < 0)
			{
				td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
				goto Exit;
			}
		}
		td_printf(0, "dd pTMP4ISampleIndexHead->m_stSdvIndexHead.m_iNum-1 is %d", pTMP4ISampleIndexHead->m_stSdvIndexHead.m_iNum-1);
	}
	else if(0 == memcmp((char*)_pBaseUnitType, VIRTUAL_FRAME_TYPE, MP4_BOX_TYPE_LENGTH))
	{		
		td_printf(0, "---------------------------------------------}}}}virtual frame info");

		if(_uiBaseMemSize != sizeof(TMP4ISampleIndexInfo))
		{
			td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
			goto Exit;
		}
		
		uiMemNum = (_uiBaseUnitSize - sizeof(TBaseUnit))/_uiBaseMemSize;
		td_printf(0, "==============>uiMemNum is %d", uiMemNum);

		TMP4ISampleIndexInfo* pTmpMP4ISampleMem = NULL;
		int iHaveMemNum = 0;

		for(i = 0; i < uiMemNum; i++)
		{				
			pTMP4ISampleIndexHead->m_stSdvIndexHead.m_iNum++;
			td_printf(0, "111111111 pTMP4ISampleIndexHead->m_stSdvIndexHead.m_iNum-1 is %d", pTMP4ISampleIndexHead->m_stSdvIndexHead.m_iNum-1);
	
			if(pTMP4ISampleIndexHead->m_stSdvIndexHead.m_iNum >= pTMP4ISampleIndexHead->m_stSdvIndexHead.m_iMemNum)
			{
				td_printf(0, "1111_pMP4ResumSession->m_puVirFrameBuf is %p", _pMP4ResumSession->m_puVirFrameBuf);
				pTMP4ISampleIndexHead->m_stSdvIndexHead.m_iMemNum+=MAX_RELLOC_MEMNUM;
				iHaveMemNum = pTMP4ISampleIndexHead->m_stSdvIndexHead.m_iMemNum;
				_pMP4ResumSession->m_puVirFrameBuf = (unsigned char*)realloc(_pMP4ResumSession->m_puVirFrameBuf,
					MP4_BOX_SIEZ_LENGTH+MP4_SAMPLE_TYPE_LENGTH+sizeof(TMP4ISampleIndexHead)+sizeof(TMP4ISampleIndexInfo)*iHaveMemNum+MP4_APPEND_CHAR_INFO_SIZE + sizeof(TIFrameIndexTail));

				if(NULL == _pMP4ResumSession->m_puVirFrameBuf)
				{
					td_printf(0xff0000, "%s %s %d error! NULL == m_puVirFrameBuf", __FILE__, __FUNCTION__, __LINE__);
					goto Exit;
				}
				else
				{
					//´Ë´¦ÖØÐÂÖ¸ÏòÒ»ÏÂ£¬·ÀÖ¹realloc µØÖ·¸Ä±ä³öÒì³£
					td_printf(0, "2222_pMP4ResumSession->m_puVirFrameBuf is %p", _pMP4ResumSession->m_puVirFrameBuf);
					pTMP4ISampleIndexHead = (TMP4ISampleIndexHead*)(_pMP4ResumSession->m_puVirFrameBuf+MP4_BOX_SIEZ_LENGTH+MP4_SAMPLE_TYPE_LENGTH);
				}
			}
			
			td_printf(0, "ssssssssssssss");
			td_printf(0, "222222222 pTMP4ISampleIndexHead->m_stSdvIndexHead.m_iNum-1 is %d", pTMP4ISampleIndexHead->m_stSdvIndexHead.m_iNum-1);
			pTmpMP4ISampleMem = (TMP4ISampleIndexInfo*)(_pMP4ResumSession->m_puVirFrameBuf+MP4_BOX_SIEZ_LENGTH+MP4_SAMPLE_TYPE_LENGTH+sizeof(TMP4ISampleIndexHead));
			memcpy(&(pTmpMP4ISampleMem[pTMP4ISampleIndexHead->m_stSdvIndexHead.m_iNum-1]), _pMP4ResumSession->m_pDataBuf+uiPos, _uiBaseMemSize);
			uiPos += _uiBaseMemSize;
			
			if(Inner_WriteBaseUnitToMP4(_pMP4ResumSession->m_MP4Handle,
				sizeof(TBaseUnit)+_uiBaseMemSize,
				_uiBaseMemSize,
				VMFT,(unsigned char*)&(pTmpMP4ISampleMem[pTMP4ISampleIndexHead->m_stSdvIndexHead.m_iNum-1]),
				_uiBaseMemSize) < 0)
			{
				td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
				goto Exit;
			}
		}		
	}
	else if(0 == memcmp((char*)_pBaseUnitType, AUDIO_ENCODE_TYPE, MP4_BOX_TYPE_LENGTH))
	{		
		td_printf(0, "---------------------------------------------}}}}audioType info");

		if(_uiBaseMemSize != sizeof(unsigned int))
		{
			td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
			goto Exit;
		}

		unsigned int uiAudioType = 0xff;

		memcpy(&uiAudioType, _pMP4ResumSession->m_pDataBuf+uiPos, sizeof(unsigned int));
		uiPos += sizeof(unsigned int);
		
		if(Inner_WriteSimpleInfoBaseUnit(_pMP4ResumSession->m_MP4Handle, ADET, uiAudioType) < 0)
		{
			td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
			goto Exit;
		}
	}
	else if(0 == memcmp((char*)_pBaseUnitType, ENCRYPTION_TYPE, MP4_BOX_TYPE_LENGTH))
	{		
		td_printf(0, "---------------------------------------------}}}}EncryptType info");

		if(_uiBaseMemSize != sizeof(unsigned int))
		{
			td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
			goto Exit;
		}

		unsigned int uiEncryptFlag = 0xff;
		
		memcpy(&uiEncryptFlag, _pMP4ResumSession->m_pDataBuf+uiPos, sizeof(unsigned int));
		uiPos += sizeof(unsigned int);
		
		if(Inner_WriteSimpleInfoBaseUnit(_pMP4ResumSession->m_MP4Handle, ENCT, uiEncryptFlag) < 0)
		{
			td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
			goto Exit;
		}
	}
	else if(0 == memcmp((char*)_pBaseUnitType, ADJOURN, MP4_BOX_TYPE_LENGTH))
	{		
		td_printf(0, "---------------------------------------------}}}}Adjourn info");
		
		if(Inner_WriteBaseUnitToMP4(_pMP4ResumSession->m_MP4Handle,
			_uiBaseUnitSize,
			_uiBaseMemSize,
			AJON,(unsigned char*)(_pMP4ResumSession->m_pDataBuf+uiPos), _uiBaseMemSize) < 0)
		{
			td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
			goto Exit;
		}

		uiPos += _uiBaseMemSize;
	}
	else
	{
		td_printf(0xff0000, "%s %s %d error! Don't support the Custom Mdat[%s]!", __FILE__, __FUNCTION__, __LINE__,_pBaseUnitType);
		goto Exit;
	}

	*_puiPos = uiPos;

	iRet = td_success;
Exit:	
#endif
	return iRet;
}

static int Inner_AnalasyOneCustomMdat(TMP4ResumSession* _pMP4ResumSession, int _iNeedResumLastMdat)
{
	int iRet = MP4_DIRBURN_READCD_FAILED;	
#ifdef FOR_MP4SDK
#else
	unsigned int uiBoxSize = 0;
	unsigned long long ullReadLen = 0;
	unsigned int uiBaseUnitSize = 0;
	unsigned char ucBaseUnitType[MP4_BOX_TYPE_LENGTH] = {0};
	unsigned int uiMemSize = 0;
	unsigned int uiBaseMemSize = 0;
	unsigned int uiExpectLen = 0;
	unsigned int uiPos = 0;
	unsigned int uiCurBURemainLen = 0;
	//unsigned int uiCount = 0;

	if(NULL == _pMP4ResumSession)
	{
		td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
		goto Exit;
	}

	while(_pMP4ResumSession->m_uiCurBoxReadSum < _pMP4ResumSession->m_uiCurMP4BoxSize)
	{		
		uiPos = 0;
		//td_printf(0, "11 uiPos is %d, uiCurBURemainLen is %d", uiPos, uiCurBURemainLen);

		if((_pMP4ResumSession->m_uiCurMP4BoxSize - _pMP4ResumSession->m_uiCurBoxReadSum) > (MP4_RESUM_DATA_BUF_SIZE-uiCurBURemainLen))
		{
			uiExpectLen = MP4_RESUM_DATA_BUF_SIZE-uiCurBURemainLen;
		}
		else
		{
			uiExpectLen = _pMP4ResumSession->m_uiCurMP4BoxSize-_pMP4ResumSession->m_uiCurBoxReadSum;
		}

		//td_printf(0, "22 uiPos is %d, uiCurBURemainLen is %d, uiExpectLen is %d", uiPos, uiCurBURemainLen, uiExpectLen);

		if(Inner_ReadDataFromCD(_pMP4ResumSession, _pMP4ResumSession->m_pDataBuf+uiCurBURemainLen, uiExpectLen, (unsigned int*)&ullReadLen) < 0)
		{
			td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
			goto Exit;
		}
		
		if(ullReadLen != uiExpectLen)
		{
			iRet = Inner_AnalasyReadCDRetValue(_pMP4ResumSession, uiExpectLen, ullReadLen, OTHER, _iNeedResumLastMdat);
		
			if(iRet < 0)
			{
				td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
				goto Exit;
			}
		}
		else
		{
			uiCurBURemainLen += ullReadLen;

			//td_printf(0, "33 uiPos is %d, uiCurBURemainLen is %d, ullReadLen is %d", uiPos, uiCurBURemainLen, ullReadLen);

			while(uiCurBURemainLen > MP4_BOX_SIEZ_LENGTH)
			{
				memcpy(&uiBoxSize, _pMP4ResumSession->m_pDataBuf+uiPos, MP4_BOX_SIEZ_LENGTH);
				ConvertSort((unsigned char*)&(uiBaseUnitSize), uiBoxSize);
				//td_printf(0, "uiBaseUnitSize is %d", uiBaseUnitSize);
			
				if(uiBaseUnitSize > MP4_RESUM_DATA_BUF_SIZE)
				{
					td_printf(0, "m_uiCurMP4BoxSize is %u, m_uiCurBoxReadSum is %u",
						_pMP4ResumSession->m_uiCurMP4BoxSize,
						_pMP4ResumSession->m_uiCurBoxReadSum);
					td_printf(0xff0000, "%d buf is not enough! uiBaseUnitSize is %u", __LINE__, uiBaseUnitSize);
					goto Exit;
				}

				if(uiCurBURemainLen >= uiBaseUnitSize)
				{
					uiPos += MP4_BOX_SIEZ_LENGTH;
					memcpy(ucBaseUnitType, _pMP4ResumSession->m_pDataBuf+uiPos, MP4_BOX_TYPE_LENGTH);
					uiPos += MP4_BOX_TYPE_LENGTH;
					memcpy(&uiMemSize, _pMP4ResumSession->m_pDataBuf+uiPos, sizeof(unsigned int));
					ConvertSort((unsigned char*)&(uiBaseMemSize), uiMemSize);
					uiPos += sizeof(unsigned int);
					
					if(Inner_AnalasyOneBaseUnit(_pMP4ResumSession, (char*)ucBaseUnitType, &uiPos, uiBaseUnitSize, uiBaseMemSize) <0)
					{
						td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
						goto Exit;
					}
					uiCurBURemainLen -= uiBaseUnitSize;
					//td_printf(0, "44 uiPos is %d, uiCurBURemainLen is %d", uiPos, uiCurBURemainLen);
				}
				else
				{
					td_printf(0, "55 uiPos is %d, uiCurBURemainLen is %d", uiPos, uiCurBURemainLen);
					memmove(_pMP4ResumSession->m_pDataBuf, _pMP4ResumSession->m_pDataBuf+uiPos, uiCurBURemainLen);
					break;
				}	

				#if 0 
				uiCount++;
			
				if(uiCount == 20)
				{
					//Í£¶ÙÒ»ÏÂ£¬·ÀÖ¹cpu ¹ý¸ß
					MP4USLEEP(1);
					uiCount = 0;
				}
				#endif
			}
		}
	}

	iRet = td_success;
Exit:
#endif
	return iRet;
}
void DirBurn_WriteRealTimeData(MP4FileHandle _hMP4Hadle, int _iErrorCode, unsigned char* _pRTStream, unsigned long long _ullRTStreamSize)
{
	TMP4ConvertSession* pstMP4ConvertSession = NULL;
	
	if(NULL != _hMP4Hadle)
	{
		pstMP4ConvertSession = Inner_FindConvertSession(_hMP4Hadle);

		if(NULL == pstMP4ConvertSession)
		{
			td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
			goto Exit;
		}
		
		//td_printf(0, "%s %s %d data len is %llu", __FILE__, __FUNCTION__, __LINE__, _ullRTStreamSize);

		if((NULL != _pRTStream)&&(_ullRTStreamSize >0))
		{
			if(pstMP4ConvertSession->m_iConvertType == DIRBURN_WITHSATA)
			{		
				if(NULL != pstMP4ConvertSession->m_CurMP4Handle)
				{
					td_mp4_fwrite(_pRTStream, 1, _ullRTStreamSize, (void*)pstMP4ConvertSession->m_CurMP4Handle);
				}
			}
			else if(pstMP4ConvertSession->m_iConvertType == DIRBURN_WITHOUTSATA)
			{
				if(NULL != gBigFile_WirteBuffer)
				{
					gBigFile_WirteBuffer(pstMP4ConvertSession->m_uiDevNo, (unsigned char*)_pRTStream, (unsigned int)_ullRTStreamSize);
				}
				else
				{
					td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
					goto Exit;
				}
			}
			else
			{
				td_printf(0xff0000, "%s %s %d the convert type is not support! please check!", __FILE__, __FUNCTION__, __LINE__);
				goto Exit;
			}
		}
	}
	else
	{
		td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
		goto Exit;
	}
Exit:
	return;
}

void DirBurn_WriteResumData(MP4FileHandle _hMP4Hadle, int _iErrorCode, unsigned char* _pRTStream, unsigned long long _ullRTStreamSize)
{	
	TMP4ResumSession* pMP4ResumSession = NULL;

	if(NULL != _hMP4Hadle)
	{
		pMP4ResumSession = Inner_FindResumSession(_hMP4Hadle);

		if(NULL == pMP4ResumSession)
		{
			td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
			goto Exit;
		}
		
		if((NULL != _pRTStream)&&(_ullRTStreamSize >0))
		{
			if(NULL != gBigFile_WirteBuffer)
			{
				gBigFile_WirteBuffer(pMP4ResumSession->m_uiDevNo, (unsigned char*)_pRTStream, (unsigned int)_ullRTStreamSize);
			}
			else
			{
				td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
				goto Exit;
			}
		}
	}
	else
	{
		td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
		goto Exit;
	}
Exit:
	return;
}

static TMP4ConvertSession* DirBurn_AddConvertSession(MP4FileHandle _pMP4FileHandle, unsigned int _uiDevNo, int _iConvertType)
{
	TMP4ConvertSession* pMP4ConvertSession = NULL;
	int i = 0;
	int iFound = 0;
	unsigned char* ucAVDataBuf = NULL;
	unsigned char* ucVideoSampleDataBuf = NULL;
	unsigned char* puVirFrameBuf = NULL;
	TMP4ISampleIndexHead* pMP4ISampleIndexHead = NULL;
	TSdvFileTime* pSdvFileTimeMem = NULL;
	unsigned int  uiInitMallocSize = 0;
	
	if(NULL == _pMP4FileHandle)
	{
		td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
		goto Exit;
	}
	else
	{
		td_printf(0, "%s _pMP4FileHandle is %p", __FUNCTION__, _pMP4FileHandle);

		uiInitMallocSize = MAX_VIDEO_SAMPLE_SIZE + MAX_AUDIO_SAMPLE_SIZE;
		ucAVDataBuf = (unsigned char*)malloc(uiInitMallocSize);

		if(NULL == ucAVDataBuf)
		{
			td_printf(0xff0000, "%s %s %d error! %s\n", __FILE__, __FUNCTION__, __LINE__, strerror(errno));
			goto Exit;
		}
		else
		{
			memset(ucAVDataBuf, 0, uiInitMallocSize);
		}

		uiInitMallocSize = MAX_VIDEO_SAMPLE_SIZE + MP4_BOX_SIEZ_LENGTH;
		ucVideoSampleDataBuf = (unsigned char*)malloc(uiInitMallocSize);
			
		if(NULL == ucVideoSampleDataBuf)
		{
			td_printf(0xff0000, "%s %s %d error! %s\n", __FILE__, __FUNCTION__, __LINE__, strerror(errno));
			goto Exit;
		}
		else
		{
			memset(ucVideoSampleDataBuf, 0, uiInitMallocSize);
		}

		uiInitMallocSize = MP4_BOX_SIEZ_LENGTH+MP4_SAMPLE_TYPE_LENGTH+sizeof(TMP4ISampleIndexHead) + sizeof(TMP4ISampleIndexInfo)*MAX_RELLOC_MEMNUM + MP4_APPEND_CHAR_INFO_SIZE + sizeof(TIFrameIndexTail);
		puVirFrameBuf = (unsigned char*)malloc(uiInitMallocSize);

		if(NULL == puVirFrameBuf)
		{
			td_printf(0xff0000, "%s %s %d error! %s\n", __FILE__, __FUNCTION__, __LINE__, strerror(errno));
			goto Exit;
		}
		else
		{
			memset(puVirFrameBuf, 0, uiInitMallocSize);
			pMP4ISampleIndexHead = (TMP4ISampleIndexHead*)(puVirFrameBuf+MP4_BOX_SIEZ_LENGTH+MP4_SAMPLE_TYPE_LENGTH);
			pMP4ISampleIndexHead->m_stSdvIndexHead.m_iMemNum = MAX_RELLOC_MEMNUM;
		}
		
		if(DIRBURN_WITHSATA == _iConvertType)
		{
			uiInitMallocSize = sizeof(TSdvFileTime)*MAX_RELLOC_MEMNUM;
			pSdvFileTimeMem = (TSdvFileTime*)malloc(uiInitMallocSize);

			if(NULL == pSdvFileTimeMem)
			{
				td_printf(0xff0000, "%s %s %d error! %s\n", __FILE__, __FUNCTION__, __LINE__, strerror(errno));
				goto Exit;
			}
			else
			{
				memset(pSdvFileTimeMem, 0, uiInitMallocSize);
			}
		}
		
		if(!MP4SetRealtimeCallbackFun(_pMP4FileHandle, (RealTimeCallbackFun)DirBurn_WriteRealTimeData))
		{
			td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
			goto Exit;
		}
		
		MP4SetSelfDataMode(_pMP4FileHandle,MP4_COMPACT_MODE);

		for(i = 0; i < MAX_MP4_CONVSESSION_NUM; i++)
		{
			pthread_mutex_lock(&(gstMP4ConvertSession[i].m_SesssionLock));
			if(0 == gstMP4ConvertSession[i].m_iUsing)
			{
				gstMP4ConvertSession[i].m_iUsing = 1;
				gstMP4ConvertSession[i].m_iStopConvert = 0;
				gstMP4ConvertSession[i].m_uiDevNo = _uiDevNo;
				gstMP4ConvertSession[i].m_iConvertType = _iConvertType;
				gstMP4ConvertSession[i].m_stMP4Manager.m_MP4Handle = _pMP4FileHandle;	
				gstMP4ConvertSession[i].m_stMP4Manager.m_ucAVDataBuf = ucAVDataBuf;				
				gstMP4ConvertSession[i].m_stMP4Manager.m_iAVDataBufSize = MAX_VIDEO_SAMPLE_SIZE + MAX_AUDIO_SAMPLE_SIZE;
				gstMP4ConvertSession[i].m_stMP4Manager.m_ucVideoSampleDataBuf = ucVideoSampleDataBuf;				
				gstMP4ConvertSession[i].m_stMP4Manager.m_iVideoSampleDataBufSize = MAX_VIDEO_SAMPLE_SIZE + MP4_BOX_SIEZ_LENGTH;
				gstMP4ConvertSession[i].m_puVirFrameBuf = puVirFrameBuf;

				if(DIRBURN_WITHSATA == gstMP4ConvertSession[i].m_iConvertType)
				{
					gstMP4ConvertSession[i].m_stSdvTimeSumInfo.m_iFlag = 0x0123abcf;
					gstMP4ConvertSession[i].m_stSdvTimeSumInfo.m_iMemNum = MAX_RELLOC_MEMNUM;
					gstMP4ConvertSession[i].m_stSdvTimeSumInfo.m_pSdvFileTimeMem = pSdvFileTimeMem;				
				}
				
				pMP4ConvertSession = &(gstMP4ConvertSession[i]);
				iFound = 1;
			}
			pthread_mutex_unlock(&(gstMP4ConvertSession[i].m_SesssionLock));

			if(1 == iFound)
			{
				td_printf(0, "%s %d found session[%d] is no using...", __FUNCTION__, __LINE__, i);
				break;
			}
		}
	}
Exit:	
	if(NULL == pMP4ConvertSession)
	{
		if(NULL != ucAVDataBuf)
		{
			free(ucAVDataBuf);
			ucAVDataBuf= NULL;
		}
		if(NULL != ucVideoSampleDataBuf)
		{
			free(ucVideoSampleDataBuf);
			ucVideoSampleDataBuf= NULL;
		}
		if(NULL != puVirFrameBuf)
		{
			free(puVirFrameBuf);
			puVirFrameBuf= NULL;
		}
		if(NULL != pSdvFileTimeMem)
		{
			free(pSdvFileTimeMem);
			pSdvFileTimeMem= NULL;
		}
	}
	
	return pMP4ConvertSession;
}

static int DirBurn_FreeConvertSession(TMP4ConvertSession* _pstMP4ConvertSession)
{
	td_printf(0, "%s %d 11", __FUNCTION__, __LINE__);
	int iRet = td_failure;
#ifdef FOR_MP4SDK
#else
	if(NULL == _pstMP4ConvertSession)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		goto Exit;
	}
	
	if(NULL != _pstMP4ConvertSession->m_CurMP4Handle)
	{
		td_printf(0, "%s %d 22", __FUNCTION__, __LINE__);
		td_mp4_fclose((void*)(_pstMP4ConvertSession->m_CurMP4Handle));
		td_printf(0, "%s %d 222", __FUNCTION__, __LINE__);
		_pstMP4ConvertSession->m_CurMP4Handle = NULL;
	}
	if(NULL != _pstMP4ConvertSession->m_stMP4Manager.m_pfSDVFileHandle)
	{
		td_printf(0, "%s %d 33", __FUNCTION__, __LINE__);
		td_mp4_fclose((void*)(_pstMP4ConvertSession->m_stMP4Manager.m_pfSDVFileHandle));
		td_printf(0, "%s %d 333", __FUNCTION__, __LINE__);
		_pstMP4ConvertSession->m_stMP4Manager.m_pfSDVFileHandle = NULL;
	}

	if(NULL != _pstMP4ConvertSession->m_stMP4Manager.m_MP4Handle)
	{   /*¹Ø±ÕMP4 ÎÄ¼þ¾ä±ú*/
		td_printf(0, "%s %d 44", __FUNCTION__, __LINE__);
		MP4CloseRT(_pstMP4ConvertSession->m_stMP4Manager.m_MP4Handle, MP4_CLOSE_DO_NOT_COMPUTE_BITRATE_V2, NULL, NULL);
		td_printf(0, "%s %d 444", __FUNCTION__, __LINE__);
		_pstMP4ConvertSession->m_stMP4Manager.m_MP4Handle = NULL;
	}
	
	if(NULL != _pstMP4ConvertSession->m_stMP4Manager.m_pvSDVVirtualFramebuffer)
	{
		td_printf(0, "%s %d 55", __FUNCTION__, __LINE__);
		free(_pstMP4ConvertSession->m_stMP4Manager.m_pvSDVVirtualFramebuffer);
		td_printf(0, "%s %d 555", __FUNCTION__, __LINE__);
		_pstMP4ConvertSession->m_stMP4Manager.m_pvSDVVirtualFramebuffer = NULL;
	}
	if(NULL != _pstMP4ConvertSession->m_stMP4Manager.m_pvMP4VirtualFramebuffer)
	{
		td_printf(0, "%s %d 66", __FUNCTION__, __LINE__);
		free(_pstMP4ConvertSession->m_stMP4Manager.m_pvMP4VirtualFramebuffer);
		td_printf(0, "%s %d 666", __FUNCTION__, __LINE__);
		_pstMP4ConvertSession->m_stMP4Manager.m_pvMP4VirtualFramebuffer = NULL;
	}
	//ÊÍ·ÅÉêÇëµÄÁÙÊ±¿Õ¼ä
	if(NULL != _pstMP4ConvertSession->m_stMP4Manager.m_ucAVDataBuf)
	{
		td_printf(0, "%s %d 77", __FUNCTION__, __LINE__);
		free(_pstMP4ConvertSession->m_stMP4Manager.m_ucAVDataBuf);
		td_printf(0, "%s %d 777", __FUNCTION__, __LINE__);
		_pstMP4ConvertSession->m_stMP4Manager.m_ucAVDataBuf = NULL;
	}
	if(NULL != _pstMP4ConvertSession->m_stMP4Manager.m_ucVideoSampleDataBuf)
	{
		td_printf(0, "%s %d 88", __FUNCTION__, __LINE__);
		free(_pstMP4ConvertSession->m_stMP4Manager.m_ucVideoSampleDataBuf);
		td_printf(0, "%s %d 888", __FUNCTION__, __LINE__);
		_pstMP4ConvertSession->m_stMP4Manager.m_ucVideoSampleDataBuf = NULL;
	}
	if(NULL != _pstMP4ConvertSession->m_puVirFrameBuf)
	{
		td_printf(0, "%s %d 99", __FUNCTION__, __LINE__);
		free(_pstMP4ConvertSession->m_puVirFrameBuf);
		td_printf(0, "%s %d 999", __FUNCTION__, __LINE__);
		_pstMP4ConvertSession->m_puVirFrameBuf = NULL;
	}
	if(NULL != _pstMP4ConvertSession->m_stSdvTimeSumInfo.m_pSdvFileTimeMem)
	{
		td_printf(0, "%s %d aa", __FUNCTION__, __LINE__);
		free(_pstMP4ConvertSession->m_stSdvTimeSumInfo.m_pSdvFileTimeMem);
		td_printf(0, "%s %d aaa", __FUNCTION__, __LINE__);
		_pstMP4ConvertSession->m_stSdvTimeSumInfo.m_pSdvFileTimeMem = NULL;
	}
	
	_pstMP4ConvertSession->m_iUsing = 0;
	_pstMP4ConvertSession->m_uiDevNo = 0;
	_pstMP4ConvertSession->m_iConvertNO = 0;
	_pstMP4ConvertSession->m_iConvertType = 0;
	_pstMP4ConvertSession->m_uiEncryptFlag 	= 0xff;
	_pstMP4ConvertSession->m_uiAudioType 	= 0xff;
    _pstMP4ConvertSession->m_iStopConvert 	= 1;
	_pstMP4ConvertSession->m_iFileBeginTime = 0;
	memset(&(_pstMP4ConvertSession->m_stMP4Manager), 0, sizeof(TMP4Mgr));
	memset(&(_pstMP4ConvertSession->m_stSdvTimeSumInfo), 0, sizeof(TSdvTimeSumInfo));
	_pstMP4ConvertSession->m_ullFileSize = 0;
	_pstMP4ConvertSession->m_ullVideoTotalDuration = 0;
	_pstMP4ConvertSession->m_ullAudioTotalDuration = 0;

	iRet = td_success;
Exit:	
#endif
	return iRet;
}

static TMP4ResumSession* DirBurn_AddResumSession(MP4FileHandle _pMP4FileHandle, unsigned int _uiDevNo, unsigned long long _ullDiskTotalSize)
{
	TMP4ResumSession* pMP4ResumSession = NULL;
	unsigned char* pDataBuf = NULL;
	unsigned char* puVirFrameBuf = NULL;
	TMP4ISampleIndexHead* pMP4ISampleIndexHead = NULL;
	int i = 0;
	int iFound = 0;
	unsigned int uiInitMallocSize = 0;
	
	if(NULL == _pMP4FileHandle)
	{
		td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
		goto Exit;
	}
	else
	{
		uiInitMallocSize = MP4_RESUM_DATA_BUF_SIZE;
		pDataBuf = (unsigned char*)malloc(uiInitMallocSize);

		if(NULL == pDataBuf)
		{
			td_printf(0xff0000, "%s %s %d error! %s\n", __FILE__, __FUNCTION__, __LINE__, strerror(errno));
			goto Exit;
		}
		else
		{
			memset(pDataBuf, 0, uiInitMallocSize);
		}

		uiInitMallocSize = MP4_BOX_SIEZ_LENGTH+MP4_SAMPLE_TYPE_LENGTH+sizeof(TMP4ISampleIndexHead) + sizeof(TMP4ISampleIndexInfo)*MAX_RELLOC_MEMNUM + MP4_APPEND_CHAR_INFO_SIZE + sizeof(TIFrameIndexTail);
		puVirFrameBuf = (unsigned char*)malloc(uiInitMallocSize);

		if(NULL == puVirFrameBuf)
		{
			td_printf(0xff0000, "%s %s %d error! %s\n", __FILE__, __FUNCTION__, __LINE__, strerror(errno));
			goto Exit;
		}
		else
		{
			memset(puVirFrameBuf, 0, uiInitMallocSize);
			printfdataX("puVirFrameBuf", (char*)puVirFrameBuf, uiInitMallocSize);
			pMP4ISampleIndexHead = (TMP4ISampleIndexHead*)(puVirFrameBuf+MP4_BOX_SIEZ_LENGTH+MP4_SAMPLE_TYPE_LENGTH);
			printfdataX("pMP4ISampleIndexHead",(char*)pMP4ISampleIndexHead,sizeof(TMP4ISampleIndexHead));
			td_printf(0, "init-------------->pMP4ISampleIndexHead->m_stSdvIndexHead.m_iNum is %d", pMP4ISampleIndexHead->m_stSdvIndexHead.m_iNum);
			pMP4ISampleIndexHead->m_stSdvIndexHead.m_iMemNum = MAX_RELLOC_MEMNUM;
		}

		if(!MP4SetRealtimeCallbackFun(_pMP4FileHandle, (RealTimeCallbackFun)DirBurn_WriteResumData))
		{
			td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
			goto Exit;
		}
		
		MP4SetSelfDataMode(_pMP4FileHandle,MP4_COMPACT_MODE);

		for(i = 0; i < MAX_MP4_CONVSESSION_NUM; i++)
		{
			pthread_mutex_lock(&(gstMP4ResumSession[i].m_SesssionLock));
			if(0 == gstMP4ResumSession[i].m_iUsing)
			{
				gstMP4ResumSession[i].m_iUsing = 1;
				gstMP4ResumSession[i].m_MP4Handle = _pMP4FileHandle;
				gstMP4ResumSession[i].m_puVirFrameBuf = puVirFrameBuf;
				gstMP4ResumSession[i].m_uiDevNo   = _uiDevNo;
				gstMP4ResumSession[i].m_ullCDSize = _ullDiskTotalSize;
				gstMP4ResumSession[i].m_pDataBuf  = pDataBuf;			
				pMP4ResumSession = &(gstMP4ResumSession[i]);
				iFound = 1;
			}
			pthread_mutex_unlock(&(gstMP4ResumSession[i].m_SesssionLock));

			if(1 == iFound)
			{
				td_printf(0, "%s %d found session[%d] is no using...", __FUNCTION__, __LINE__, i);
				break;
			}
		}
	}
Exit:	
	if(NULL == pMP4ResumSession)
	{
		if(NULL != pDataBuf)
		{
			free(pDataBuf);
			pDataBuf= NULL;
		}
		if(NULL != puVirFrameBuf)
		{
			free(puVirFrameBuf);
			puVirFrameBuf = NULL;
		}
	}
	return pMP4ResumSession;
}

static int DirBurn_FreeMP4ResumSession(TMP4ResumSession* _pMP4ResumSession)
{
	td_printf(0, "%s %d 11", __FUNCTION__, __LINE__);
	int iRet = td_failure;
#ifdef FOR_MP4SDK
#else
	if(NULL == _pMP4ResumSession)
	{
		td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
		goto Exit;
	}

	if(NULL != _pMP4ResumSession->m_pLastResumBox)
	{
		td_printf(0, "%s %d 22", __FUNCTION__, __LINE__);
		free(_pMP4ResumSession->m_pLastResumBox);
		td_printf(0, "%s %d 222", __FUNCTION__, __LINE__);
		_pMP4ResumSession->m_pLastResumBox = NULL;
	}
	if(NULL != _pMP4ResumSession->m_pDataBuf)
	{
		td_printf(0, "%s %d 33", __FUNCTION__, __LINE__);
		free(_pMP4ResumSession->m_pDataBuf);
		td_printf(0, "%s %d 333", __FUNCTION__, __LINE__);
		_pMP4ResumSession->m_pDataBuf = NULL;
	}
	if(NULL != _pMP4ResumSession->m_pVideoTrackInfo)
	{
		td_printf(0, "%s %d 44", __FUNCTION__, __LINE__);
		free(_pMP4ResumSession->m_pVideoTrackInfo);
		td_printf(0, "%s %d 444", __FUNCTION__, __LINE__);
		_pMP4ResumSession->m_pVideoTrackInfo = NULL;
	}
	if(NULL != _pMP4ResumSession->m_MP4Handle)
	{
		td_printf(0, "%s %d 55", __FUNCTION__, __LINE__);
		MP4CloseRT(_pMP4ResumSession->m_MP4Handle, MP4_CLOSE_DO_NOT_COMPUTE_BITRATE_V2, NULL, NULL);
		td_printf(0, "%s %d 555", __FUNCTION__, __LINE__);
		_pMP4ResumSession->m_MP4Handle = NULL;
	}
	if(NULL != _pMP4ResumSession->m_puVirFrameBuf)
	{
		td_printf(0, "%s %d 66", __FUNCTION__, __LINE__);
		free(_pMP4ResumSession->m_puVirFrameBuf);
		td_printf(0, "%s %d 666", __FUNCTION__, __LINE__);
		_pMP4ResumSession->m_puVirFrameBuf = NULL;
	}

	_pMP4ResumSession->m_iUsing = 0;
	_pMP4ResumSession->m_uiDevNo = 0;
	_pMP4ResumSession->m_uiCurMP4BoxSize = 0;
	_pMP4ResumSession->m_uiLastBoxResumSum = 0;
	_pMP4ResumSession->m_uiLastBoxResumZero = 0;
	_pMP4ResumSession->m_iFileBeginTime = 0;
	_pMP4ResumSession->m_VideoTrackID = 0;
	_pMP4ResumSession->m_AudioTrackID = 0;
	_pMP4ResumSession->m_AudioType = 0;
	_pMP4ResumSession->m_uiCurBoxReadSum = 0;
	_pMP4ResumSession->m_ullCDSize = 0;
	_pMP4ResumSession->m_ullCDAddress = 0;
	_pMP4ResumSession->m_ullVideoTotalDuration = 0;
	_pMP4ResumSession->m_ullAudioTotalDuration = 0;
	memset(&(_pMP4ResumSession->m_VideoTrackInfo), 0, sizeof(TVideoTrackInfo));
	memset(&(_pMP4ResumSession->m_AudioTrackInfo), 0, sizeof(TAudioTrackInfo));
	_pMP4ResumSession->m_iMoovOrFree = 0;
	_pMP4ResumSession->m_uiMoovOrFreeLen = 0;

	iRet = td_success;
Exit:
#endif
	td_printf(0, "%s %d 77", __FUNCTION__, __LINE__);
	return iRet;
}

static int DirBurn_UpDateSessionOtherInfo(TMP4ConvertSession* _pstMP4ConvertSession)
{
	int iRet = td_failure;

	if(NULL == _pstMP4ConvertSession)
	{
		td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
		return iRet;
	}

	if(0xff == _pstMP4ConvertSession->m_uiAudioType)
	{
		if(Inner_GetFileOtherInfo(_pstMP4ConvertSession->m_stMP4Manager.m_pfSDVFileHandle, ADET, &(_pstMP4ConvertSession->m_uiAudioType)) < 0)
		{
			td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
			goto Exit;
		}
	}

	if(0xff == _pstMP4ConvertSession->m_uiEncryptFlag)
	{
		if(Inner_GetFileOtherInfo(_pstMP4ConvertSession->m_stMP4Manager.m_pfSDVFileHandle, ENCT, &(_pstMP4ConvertSession->m_uiEncryptFlag)) < 0)
		{
			td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
			goto Exit;
		}
	}

	iRet = td_success;
Exit:
	return iRet;

}

static int DirBurn_UpdateSessionMgrInfo(TMP4ConvertSession* _pstMP4ConvertSession, char * _pSdvFile, char * _pDstFile)
{
	int iRet = MP4_CONVERT_ERROR_FATAL;
	int iTmpRet = td_failure;
	S_FileHeader stFileHeader = {0};
	TSdvFileTime* pSdvFileTimeMem = NULL;
	
	if(NULL == _pstMP4ConvertSession)
	{
		td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
		return iRet;
	}

	if(DIRBURN_WITHSATA == _pstMP4ConvertSession->m_iConvertType)
	{
		td_printf(0, "%d: SDV file path is: %s", __LINE__, _pSdvFile);
		td_printf(0, "%d: MP4 file path is: %s", __LINE__, _pDstFile);

		if((NULL != _pSdvFile)&&(NULL != _pDstFile))
		{
			strncpy(_pstMP4ConvertSession->m_stMP4Manager.m_cSDVFilePath, _pSdvFile, FILENAMEMAXNUM - 1);
			strncpy(_pstMP4ConvertSession->m_stMP4Manager.m_cMP4FilePath, _pDstFile, FILENAMEMAXNUM - 1);
			if(NULL != _pstMP4ConvertSession->m_CurMP4Handle)
			{
				td_mp4_fclose((void*)_pstMP4ConvertSession->m_CurMP4Handle);
				_pstMP4ConvertSession->m_CurMP4Handle = NULL;
			}
			#ifdef TD_VFS
			_pstMP4ConvertSession->m_CurMP4Handle = (TVfile*)td_mp4_fopen(NULL, _pstMP4ConvertSession->m_stMP4Manager.m_cMP4FilePath, "wb+");
			#else
			_pstMP4ConvertSession->m_CurMP4Handle = (FILE*)td_mp4_fopen(NULL, _pstMP4ConvertSession->m_stMP4Manager.m_cMP4FilePath, "wb+");
			#endif
			if(NULL == _pstMP4ConvertSession->m_CurMP4Handle)
			{
				td_printf(0xff0000, "%s %s %d error! %s\n", __FILE__, __FUNCTION__, __LINE__, strerror(errno));
				iRet = MP4_CONVERT_ERROR_TRYNEXT;
				goto Exit;
			}
		}
		else
		{
			td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
			iRet = MP4_CONVERT_ERROR_TRYNEXT;
			goto Exit;
		}
	}
	else if(DIRBURN_WITHOUTSATA == _pstMP4ConvertSession->m_iConvertType)
	{
		td_printf(0, "%d: SDV file path is: %s", __LINE__, _pSdvFile);

		if(NULL != _pSdvFile)
		{
			strncpy(_pstMP4ConvertSession->m_stMP4Manager.m_cSDVFilePath, _pSdvFile, FILENAMEMAXNUM - 1);
		}
		else
		{
			td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
			iRet = MP4_CONVERT_ERROR_TRYNEXT;
			goto Exit;
		}
	}

	if(NULL != _pstMP4ConvertSession->m_stMP4Manager.m_pfSDVFileHandle)
	{
		td_mp4_fclose((void*)_pstMP4ConvertSession->m_stMP4Manager.m_pfSDVFileHandle);
		_pstMP4ConvertSession->m_stMP4Manager.m_pfSDVFileHandle = NULL;
	}
#ifdef TD_VFS
	_pstMP4ConvertSession->m_stMP4Manager.m_pfSDVFileHandle = (TVfile*)td_mp4_fopen(NULL, _pSdvFile, "rb");
#else
	_pstMP4ConvertSession->m_stMP4Manager.m_pfSDVFileHandle = (FILE*)td_mp4_fopen(NULL, _pSdvFile, "rb");
#endif
		
	if(NULL == _pstMP4ConvertSession->m_stMP4Manager.m_pfSDVFileHandle)
	{
		td_printf(0xff0000, "%s %s %d error! %s\n", __FILE__, __FUNCTION__, __LINE__, strerror(errno));
		iRet = MP4_CONVERT_ERROR_TRYNEXT;
		goto Exit;
	}

	_pstMP4ConvertSession->m_iConvertNO += 1;

	if(1 == _pstMP4ConvertSession->m_iConvertNO)
	{
		if(Inner_RecordFileBeginTime((signed char*)_pSdvFile, &(_pstMP4ConvertSession->m_iFileBeginTime)) < 0)
		{
			td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
			goto Exit;
		}
		td_printf(0, "_pstMP4ConvertSession->m_iFileBeginTime is %d", _pstMP4ConvertSession->m_iFileBeginTime);
	}
	
	//ÒÆ¶¯µ½ÎÄ¼þÎ²Î»ÖÃ
	td_mp4_fseek((void*)(_pstMP4ConvertSession->m_stMP4Manager.m_pfSDVFileHandle), 0, SEEK_END);
	//»ñÈ¡ÎÄ¼þ½áÎ²Î»ÖÃ
	_pstMP4ConvertSession->m_stMP4Manager.m_lFileStopPos = td_mp4_ftell((void*)(_pstMP4ConvertSession->m_stMP4Manager.m_pfSDVFileHandle));
	//ÒÆ¶¯µ½ÎÄ¼þÍ·Î»ÖÃ
	td_mp4_fseek((void*)(_pstMP4ConvertSession->m_stMP4Manager.m_pfSDVFileHandle), 0, SEEK_SET);
	//È·¶¨ÎÄ¼þ×ª»»Í£Ö¹Î»ÖÃ
	if(Inner_GetSDVVirtualFrame((void*)&(_pstMP4ConvertSession->m_stMP4Manager)) < 0)
	{  
		td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
		_pstMP4ConvertSession->m_stMP4Manager.m_lStopConvertPos = _pstMP4ConvertSession->m_stMP4Manager.m_lFileStopPos;
	}
	//ÒÆ¶¯µ½ÎÄ¼þÍ·Î»ÖÃ
	td_mp4_fseek((void*)_pstMP4ConvertSession->m_stMP4Manager.m_pfSDVFileHandle, 0, SEEK_SET);
	//¶ÁÈ¡µ±Ç°ÎÄ¼þÎÄ¼þÍ·
	iTmpRet = td_mp4_fread((void*)(&stFileHeader), 1, sizeof(S_FileHeader), (void*)_pstMP4ConvertSession->m_stMP4Manager.m_pfSDVFileHandle);
	
	if(sizeof(S_FileHeader) != iTmpRet)
	{
		td_printf(0xff0000, "%s %s %d error! iTmpRet is %d\n", __FILE__, __FUNCTION__, __LINE__, iTmpRet);
		iRet = MP4_CONVERT_ERROR_TRYNEXT;
		goto Exit;
	}
	
	iTmpRet = Inner_UpdateFileHeaderInfo(_pstMP4ConvertSession, &stFileHeader);

	//Èç¹û×ª»»¹ý³ÌÖÐÄ³Ð©ÖØÒªÊôÐÔ¸Ä±ä£¬Ôò½áÊø±¾´Î×ª»»
	if(iTmpRet < 0)
	{
		td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
		iRet = MP4_CONVERT_ERROR_OVER_CURCONVERT;
		goto Exit;
	}
	
	if(DIRBURN_WITHSATA == _pstMP4ConvertSession->m_iConvertType)
	{
		//¼ÇÂ¼´ËÎÄ¼þµÄ¿ªÊ¼½áÊøÊ±¼ä
		_pstMP4ConvertSession->m_stSdvTimeSumInfo.m_iCounts += 1;
		
		if(_pstMP4ConvertSession->m_stSdvTimeSumInfo.m_iCounts > _pstMP4ConvertSession->m_stSdvTimeSumInfo.m_iMemNum)
		{
			_pstMP4ConvertSession->m_stSdvTimeSumInfo.m_iMemNum += MAX_RELLOC_MEMNUM;
			_pstMP4ConvertSession->m_stSdvTimeSumInfo.m_pSdvFileTimeMem = 
				(TSdvFileTime*)realloc(_pstMP4ConvertSession->m_stSdvTimeSumInfo.m_pSdvFileTimeMem,sizeof(TSdvFileTime)*(_pstMP4ConvertSession->m_stSdvTimeSumInfo.m_iMemNum));

			if(NULL == _pstMP4ConvertSession->m_stSdvTimeSumInfo.m_pSdvFileTimeMem)
			{
				td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
				goto Exit;
			}
		}
		
		pSdvFileTimeMem = (TSdvFileTime*)(_pstMP4ConvertSession->m_stSdvTimeSumInfo.m_pSdvFileTimeMem);
		
		if(NULL != pSdvFileTimeMem)
		{
			if(Inner_SetFileStartAndStopTime(_pSdvFile, &(pSdvFileTimeMem[_pstMP4ConvertSession->m_stSdvTimeSumInfo.m_iCounts - 1])) < 0)
			{
				td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
				goto Exit;
			}
		}
		else
		{
			td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
			goto Exit;
		}
	}
	_pstMP4ConvertSession->m_stMP4Manager.m_bConvertExitFlag = TD_MP4_CONVERT_RUNNING;
	iRet = td_success;
Exit:
	if(td_success != iRet)
	{
		if(NULL != _pstMP4ConvertSession->m_stMP4Manager.m_pfSDVFileHandle)
		{
			td_mp4_fclose((void*)(_pstMP4ConvertSession->m_stMP4Manager.m_pfSDVFileHandle));
			_pstMP4ConvertSession->m_stMP4Manager.m_pfSDVFileHandle = NULL;
		}

		if(NULL != _pstMP4ConvertSession->m_CurMP4Handle)
		{
			td_mp4_fclose((void*)(_pstMP4ConvertSession->m_CurMP4Handle));
			_pstMP4ConvertSession->m_CurMP4Handle = NULL;
		}
	}
	return iRet;
}

static int DirBurn_ConvertSessionFileData(TMP4ConvertSession* _pstMP4ConvertSession)
{
	int iRet = td_failure;
	int iTmpRet = 0;
	void* pvSDVVedioBuf = NULL;
	int iSDVVedioBufLen = 0;
	void* pvSDVAudioBuf = NULL;
	int iSDVAudioBufLen = 0;
	TMP4Mgr* pstMP4Mgr = NULL;
		
	if(NULL == _pstMP4ConvertSession)
	{
		td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
		return iRet;
	}
	
	pstMP4Mgr = &(_pstMP4ConvertSession->m_stMP4Manager);

	//»ñÈ¡SDV ÎÄ¼þÍ·ÐÅÏ¢
	if(Inner_GetSDVFileHeadInfo(pstMP4Mgr, &(pstMP4Mgr->m_stSDVFileHead), sizeof(S_FileHeader)) < 0)
	{  
		td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
		goto Exit;
	}

	//ÏÔÊ¾SDV ÎÄ¼þÍ·ÐÅÏ¢
	Inner_ShowSDVFileHeadInfo(&(pstMP4Mgr->m_stSDVFileHead),sizeof(S_FileHeader));

	int iCount = 0;
	
	while(pstMP4Mgr->m_bConvertExitFlag == TD_MP4_CONVERT_RUNNING)
	{
		iSDVVedioBufLen = 0;
		iSDVAudioBufLen = 0;

		if(1 == _pstMP4ConvertSession->m_iStopConvert)
		{
			td_printf(0xff0000, "---------------------------Force Stop The Current Convert!");
			goto Exit;
		}

		//»ñÈ¡ÒôÊÓÆµÊý¾Ý
		if (Inner_GetSDVAVSample((void*)pstMP4Mgr, &pvSDVVedioBuf, &iSDVVedioBufLen, &pvSDVAudioBuf, &iSDVAudioBufLen) < 0)  
		{  
			td_printf(0xff0000, "%d: Call Inner_GetSDVAVSample failed!\n", __LINE__);
			goto STOP_CONVERT;
		}

		//½«ÒôÊÓÆµÊý¾Ý½øÐÐ×ª»»
		iTmpRet = Inner_ConvertAVDataToMP4Sample(_pstMP4ConvertSession, pvSDVVedioBuf, iSDVVedioBufLen, pvSDVAudioBuf, iSDVAudioBufLen);
		if (td_failure == iTmpRet)	
		{  
			td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
			goto Exit;
		}
		else if(MP4_CONVERT_ERROR_SPSPPS_CONTINUE == iTmpRet)
		{
			continue;
		}

		//	¼ÇÂ¼MP4 ÐéÄâÖ¡ÐÅÏ¢
		if(HAVE_SDV_VIRTUAL_FRAME_FLAG == pstMP4Mgr->m_bHaveSDVVirtualFrameFlag)
		{
			if(Inner_FindAndSaveVirFrameInfo(_pstMP4ConvertSession, 0) < 0)
			{  
				td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
				goto Exit;
			}
		}
		
		
		pstMP4Mgr->m_lCurFilePos = td_mp4_ftell((void*)(pstMP4Mgr->m_pfSDVFileHandle));
				
		if((0 != td_mp4_feof((void*)(pstMP4Mgr->m_pfSDVFileHandle))) 
			|| (pstMP4Mgr->m_lCurFilePos >= pstMP4Mgr->m_lStopConvertPos))
		{
			pstMP4Mgr->m_bConvertExitFlag = TD_MP4_CONVERT_STOP;
			td_printf(0x12345678, "pstMP4Mgr->m_lCurFilePos is %ld", pstMP4Mgr->m_lCurFilePos);
			td_printf(0x12345678, "pstMP4Mgr->m_lStopConvertPos is %ld", pstMP4Mgr->m_lStopConvertPos);
		}

		#if 1 
		iCount++;

		if(iCount == 10)
		{
			//Í£¶ÙÒ»ÏÂ£¬·ÀÖ¹cpu ¹ý¸ß
			MP4USLEEP(5);
			iCount = 0;
		}
		#endif
	}

	//	¸üÐÂMP4 ÐéÄâÖ¡×ÜÐÅÏ¢
STOP_CONVERT:
	{
		if(Inner_FindAndSaveVirFrameInfo(_pstMP4ConvertSession, 1) < 0)
		{  
			td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
			goto Exit;
		}
	}	

    iRet = td_success;
Exit:
	return iRet;
}

static int DirBurn_ReleaseSessionMgrSource(TMP4ConvertSession* _pstMP4ConvertSession)
{
	int iRet = td_failure;

	if(NULL == _pstMP4ConvertSession)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		goto Exit;
	}
	
	if(NULL != _pstMP4ConvertSession->m_CurMP4Handle)
	{
		td_mp4_fclose((void*)(_pstMP4ConvertSession->m_CurMP4Handle));
		_pstMP4ConvertSession->m_CurMP4Handle = NULL;
	}

	if(NULL != _pstMP4ConvertSession->m_stMP4Manager.m_pfSDVFileHandle)
	{
		td_mp4_fclose((void*)(_pstMP4ConvertSession->m_stMP4Manager.m_pfSDVFileHandle));
		_pstMP4ConvertSession->m_stMP4Manager.m_pfSDVFileHandle = NULL;
	}

	memset(_pstMP4ConvertSession->m_stMP4Manager.m_cMP4FilePath, 0, FILENAMEMAXNUM);
	memset(_pstMP4ConvertSession->m_stMP4Manager.m_cSDVFilePath, 0, FILENAMEMAXNUM);
	memset(&(_pstMP4ConvertSession->m_stMP4Manager.m_stSDVFileHead), 0, sizeof(S_FileHeader));
	memset(&(_pstMP4ConvertSession->m_stMP4Manager.m_stSDVFrameHead), 0, sizeof(S_FrameHeader));

	//_pstMP4ConvertSession->m_stMP4Manager.m_bVedioFirstAppearFlag = 0;
	//_pstMP4ConvertSession->m_stMP4Manager.m_bFirstSpsFlag = 0;
	//_pstMP4ConvertSession->m_stMP4Manager.m_bFirstPpsFlag = 0;
	//_pstMP4ConvertSession->m_stMP4Manager.m_bFoundSpsPpsFlag = 0;
	_pstMP4ConvertSession->m_stMP4Manager.m_bHaveSDVVirtualFrameFlag = 0;
	_pstMP4ConvertSession->m_stMP4Manager.m_bConvertExitFlag = TD_MP4_CONVERT_STOP;//½«µ¥Ò»sdvÎÄ¼þµÄ×ª»»×´Ì¬ÖÃ¿Õ
	//_pstMP4ConvertSession->m_stMP4Manager.m_bAudioFirstAppear = 0;
	_pstMP4ConvertSession->m_stMP4Manager.m_bIFrameFlag = 0;
	_pstMP4ConvertSession->m_stMP4Manager.m_lCurFilePos = 0;
	_pstMP4ConvertSession->m_stMP4Manager.m_lStopConvertPos = 0;
	_pstMP4ConvertSession->m_stMP4Manager.m_lFileStopPos = 0;
	_pstMP4ConvertSession->m_stMP4Manager.m_iIFrameCount = 0;

	if(NULL != _pstMP4ConvertSession->m_stMP4Manager.m_pvMP4VirtualFramebuffer)
	{
		free(_pstMP4ConvertSession->m_stMP4Manager.m_pvMP4VirtualFramebuffer);
		_pstMP4ConvertSession->m_stMP4Manager.m_pvMP4VirtualFramebuffer = NULL;
	}

	_pstMP4ConvertSession->m_stMP4Manager.m_uiMP4VirtualFramebufferSize = 0;

	if(NULL != _pstMP4ConvertSession->m_stMP4Manager.m_pvSDVVirtualFramebuffer)
	{
		free(_pstMP4ConvertSession->m_stMP4Manager.m_pvSDVVirtualFramebuffer);
		_pstMP4ConvertSession->m_stMP4Manager.m_pvSDVVirtualFramebuffer = NULL;
	}

	_pstMP4ConvertSession->m_stMP4Manager.m_uiSDVVirtualFramebufferSize = 0;

	//ÉêÇëµÄ¿Õ¼äÔÚÕâÀï²»ÊÍ·Å£¬×îºóÒ»ÆðÊÍ·Å
	#if 0
	if(NULL != _pstMP4ConvertSession->m_stMP4Manager.m_ucAVDataBuf)
	{
		free(_pstMP4ConvertSession->m_stMP4Manager.m_ucAVDataBuf);
		_pstMP4ConvertSession->m_stMP4Manager.m_ucAVDataBuf = NULL;
	}

	_pstMP4ConvertSession->m_stMP4Manager.m_iAVDataBufSize = 0;

	if(NULL != _pstMP4ConvertSession->m_stMP4Manager.m_ucVideoSampleDataBuf)
	{
		free(_pstMP4ConvertSession->m_stMP4Manager.m_ucVideoSampleDataBuf);
		_pstMP4ConvertSession->m_stMP4Manager.m_ucVideoSampleDataBuf = NULL;
	}

	_pstMP4ConvertSession->m_stMP4Manager.m_iVideoSampleDataBufSize = 0;
	#endif

	iRet = td_success;
Exit:
	return iRet;
}

static int DirBurn_WriteSessionVirFrameInfo(TMP4ConvertSession* _pstMP4ConvertSession)
{
	int iRet = td_failure;
	unsigned char* pVirtual = NULL;
	int iVitualLen = 0;	
	TSdvFileTime* pSdvFileTime = NULL;
	TMP4ISampleIndexHead* pstMP4ISampleIndexHead = NULL;

	if(NULL == _pstMP4ConvertSession)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		return iRet;
	}

	pstMP4ISampleIndexHead = (TMP4ISampleIndexHead*)(_pstMP4ConvertSession->m_puVirFrameBuf+MP4_BOX_SIEZ_LENGTH+MP4_SAMPLE_TYPE_LENGTH);

	if(DIRBURN_WITHSATA == _pstMP4ConvertSession->m_iConvertType)
	{
		//¸üÐÂÎÄ¼þ½áÊøÊ±¼ä	
		if(_pstMP4ConvertSession->m_stSdvTimeSumInfo.m_iCounts >= 1)
		{
			pSdvFileTime = (TSdvFileTime*)(_pstMP4ConvertSession->m_stSdvTimeSumInfo.m_pSdvFileTimeMem);
			
			if((pSdvFileTime != NULL)&&
				(pSdvFileTime[_pstMP4ConvertSession->m_stSdvTimeSumInfo.m_iCounts-1].m_iEndTime 
				> pstMP4ISampleIndexHead->m_stExRecInfo.m_iFileEndTime))
			{
				pstMP4ISampleIndexHead->m_stExRecInfo.m_iFileEndTime = 
					pSdvFileTime[_pstMP4ConvertSession->m_stSdvTimeSumInfo.m_iCounts-1].m_iEndTime;
			}
		}
	}

	//»ã×Ümp4×ÜÐéÄâÐÅÏ¢
	if(Inner_CompleteMP4VirFrameInfo(&(_pstMP4ConvertSession->m_puVirFrameBuf), &pVirtual, &iVitualLen) < 0)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		goto Exit;
	}

	if(Inner_GetAndWriteMP4Data(_pstMP4ConvertSession, pVirtual, iVitualLen, VIDEODATA, 0, 0, 0, 1) < 0)
	{
		td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
		goto Exit;
	}

	iRet = td_success;
Exit:
    return iRet;
}

static int DirBurn_WriteSessionSdvTimeInfo(TMP4ConvertSession* _pstMP4ConvertSession)
{
	int iRet = td_failure;
	unsigned char* pSdvTimeInfo = NULL;
	unsigned int uiMemLen = 0;

	if(NULL == _pstMP4ConvertSession)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		return iRet;
	}

	uiMemLen = sizeof(TSdvFileTime)*(_pstMP4ConvertSession->m_stSdvTimeSumInfo.m_iCounts);
	pSdvTimeInfo = (unsigned char*)malloc(sizeof(int) + uiMemLen);

	if(NULL == pSdvTimeInfo)
	{
		td_printf(0xff0000, "%s %s %d error! %s", __FILE__, __FUNCTION__, __LINE__, strerror(errno));
		goto Exit;
	}
	else
	{
		memcpy(pSdvTimeInfo, &(_pstMP4ConvertSession->m_stSdvTimeSumInfo.m_iFlag), sizeof(int));
		memcpy(pSdvTimeInfo+sizeof(int), _pstMP4ConvertSession->m_stSdvTimeSumInfo.m_pSdvFileTimeMem, uiMemLen);
		
		if(Inner_WriteBaseUnitToMP4(_pstMP4ConvertSession->m_stMP4Manager.m_MP4Handle,
			sizeof(TBaseUnit)+sizeof(int)+uiMemLen,
			sizeof(int)+uiMemLen,
			AJON,(unsigned char*)pSdvTimeInfo, sizeof(int)+uiMemLen) < 0)
		{
			td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
			goto Exit;
		}
		iRet = td_success;
	}	
Exit:
	if(NULL != pSdvTimeInfo)
	{
		free(pSdvTimeInfo);
		pSdvTimeInfo = NULL;
	}	
    return iRet;
}

static int DirBurn_GetAndAnalasyAllCustomMdat(TMP4ResumSession* _pMP4ResumSession, int _iNeedResumLastMdat)
{
	int iRet = td_failure;
#ifdef FOR_MP4SDK
#else
	
	if(NULL ==_pMP4ResumSession)
	{
		td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
		return iRet;
	}

	while(1)
	{
		_pMP4ResumSession->m_uiCurMP4BoxSize = 0;
		_pMP4ResumSession->m_uiCurBoxReadSum = 0;
		
		iRet = Inner_GetNextCustomMdat(_pMP4ResumSession, _iNeedResumLastMdat);
		if(iRet < 0)
		{
			if(MP4_DIRBURN_READCD_FAILED == iRet)
			{
				td_printf(0xff0000, "%s %s %d read CD error!\n", __FILE__, __FUNCTION__, __LINE__);
				goto Exit;
			}
			else if(MP4_DIRBURN_READCD_NOTENOUGH == iRet)
			{
				td_printf(0xff0000, "%s %s %d read CD not enough!\n", __FILE__, __FUNCTION__, __LINE__);
				goto Exit;
			}
			else if(MP4_DIRBURN_READCD_OVER == iRet)
			{
				td_printf(0xff0000, "%s %s %d read CD over!\n", __FILE__, __FUNCTION__, __LINE__);
				goto Exit;
			}
			else if(MP4_DIRBURN_NOT_CONFORM == iRet)
			{
				continue;
			}
		}

		iRet = Inner_AnalasyOneCustomMdat(_pMP4ResumSession, _iNeedResumLastMdat);
		if(iRet < 0)
		{
			if(MP4_DIRBURN_READCD_FAILED == iRet)
			{
				td_printf(0xff0000, "%s %s %d read CD error!\n", __FILE__, __FUNCTION__, __LINE__);
				goto Exit;
			}
			else if(MP4_DIRBURN_READCD_NOTENOUGH == iRet)
			{
				td_printf(0xff0000, "%s %s %d read CD not enough!\n", __FILE__, __FUNCTION__, __LINE__);
				goto Exit;
			}
			else if(MP4_DIRBURN_READCD_OVER == iRet)
			{
				td_printf(0xff0000, "%s %s %d read CD over!\n", __FILE__, __FUNCTION__, __LINE__);
				goto Exit;
			}
			else if(MP4_DIRBURN_NOT_CONFORM == iRet)
			{
				continue;
			}
		}
		
	}

	iRet = td_success;
Exit:
#endif
	return iRet;
}

void MP4_BurnInit()
{
	int i = 0;

	for(i = 0; i < MAX_MP4_CONVSESSION_NUM; i++)
	{
		//³õÊ¼»¯×ª»»»á»°
		memset(&gstMP4ConvertSession[i], 0, sizeof(TMP4ConvertSession));
		gstMP4ConvertSession[i].m_iUsing       = 0;
		gstMP4ConvertSession[i].m_iStopConvert = 1;
		gstMP4ConvertSession[i].m_CurMP4Handle = NULL;
		gstMP4ConvertSession[i].m_uiEncryptFlag= 0xff;
		gstMP4ConvertSession[i].m_uiAudioType  = 0xff;
		gstMP4ConvertSession[i].m_stMP4Manager.m_pfSDVFileHandle = NULL;
		gstMP4ConvertSession[i].m_stMP4Manager.m_MP4Handle = NULL;
		gstMP4ConvertSession[i].m_stMP4Manager.m_pvSDVVirtualFramebuffer = NULL;
		gstMP4ConvertSession[i].m_stMP4Manager.m_pvMP4VirtualFramebuffer = NULL;
		gstMP4ConvertSession[i].m_puVirFrameBuf = NULL;
		gstMP4ConvertSession[i].m_stSdvTimeSumInfo.m_pSdvFileTimeMem = NULL;
		pthread_mutex_init(&(gstMP4ConvertSession[i].m_SesssionLock), NULL);
		//³õÊ¼»¯ÐÞ¸´»á»°
		memset(&gstMP4ResumSession[i], 0, sizeof(TMP4ResumSession));
		gstMP4ResumSession[i].m_iUsing        = 0;
		gstMP4ResumSession[i].m_puVirFrameBuf = NULL;
		gstMP4ResumSession[i].m_MP4Handle = NULL;
		gstMP4ResumSession[i].m_pVideoTrackInfo = NULL;
		gstMP4ResumSession[i].m_pLastResumBox = NULL;
		gstMP4ResumSession[i].m_pDataBuf = NULL;
		pthread_mutex_init(&(gstMP4ResumSession[i].m_SesssionLock), NULL);
	}
}

int MP4_RegWriteBufCallBack(MP4BigFileWirteBuffer _CallBack)
{
	int iRet = td_failure;
	
	if(NULL == _CallBack)
	{
		td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
		goto Exit;
	}

	gBigFile_WirteBuffer = _CallBack;

	iRet = td_success;
Exit:
	return iRet;
}

int MP4_RegReadDiskCallBack(MP4BigFileReadDisk _CallBack)
{
	int iRet = td_failure;
	
	if(NULL == _CallBack)
	{
		td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
		goto Exit;
	}

	gBigFile_ReadDisk = _CallBack;

	iRet = td_success;
Exit:
	return iRet;
}

int MP4_GetConvertTailSize(MP4FileHandle _MP4File, unsigned long long* _pullTailSize)
{
	int iRet = td_failure;

#ifdef FOR_MP4SDK
#else
	if((NULL == _MP4File)||(NULL == _pullTailSize))
	{
		td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
		goto Exit;
	}

	if(NULL == Inner_FindConvertSession(_MP4File))
	{
		td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
		goto Exit;
	}

	*_pullTailSize = MP4GetFileTailSize(_MP4File);

	iRet = td_success;
Exit:
#endif
	return iRet;
}

int MP4_GetSdvConvertInfo(char* _pFilename, int* _piCounts, char** _pcInfoBuf, int* _piInfoLen)
{
	int iRet = td_failure;
	int iFileOs = 0;
	unsigned int uiVersionBefore = 0;
	unsigned int uiVersionAfter  = 0;
	unsigned long long ullMdatPosBefore = 0;
	unsigned long long ullMdatPosAfter = 0;
	unsigned int uiInfoSize = 0;
	unsigned int uiBaseUnitSizeBefore = 0;
	unsigned int uiBaseUnitSizeAfter = 0;
	char cFullPath[FULL_PATH_LEN] = {0};	
#ifdef TD_VFS
	TVfile* TmpFileHandle = NULL;
#else
	FILE*   TmpFileHandle = NULL;
#endif
	int iFlag = 0;
	char* pInfoBuf = NULL;

	if((NULL == _pFilename)||(NULL == _piCounts)||(NULL == _pcInfoBuf)||(NULL == _piInfoLen))
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		goto Exit;
	}
	
	if(Inner_CompleteFileName((char*)_pFilename, cFullPath, FULL_PATH_LEN) < 0)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		goto Exit;
	}
		
	//´ò¿ªÎÄ¼þ
	#ifdef TD_VFS
	TmpFileHandle = (TVfile*)td_mp4_fopen(NULL, cFullPath, "rb");
	#else
	TmpFileHandle = (FILE*)td_mp4_fopen(NULL, cFullPath, "rb");
	#endif
	
	if(NULL == TmpFileHandle)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		goto Exit;
	}

	if(td_mp4_fseek(TmpFileHandle, -128, SEEK_END) < 0)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		goto Exit;
	}

	iFileOs = td_mp4_fread(&uiVersionBefore, 1, 4, TmpFileHandle);
	ConvertSort((unsigned char*)&uiVersionAfter, uiVersionBefore);

	if((iFileOs != 4)||(1 != uiVersionAfter))
	{
		td_printf(0xff0000, "%s %s %d error! Version is %d", __FILE__, __FUNCTION__, __LINE__, uiVersionAfter);
		goto Exit;
	}


	iFileOs = td_mp4_fread(&ullMdatPosBefore, 1, 8, TmpFileHandle);
	ConvertSortLong((unsigned char*)&ullMdatPosAfter, ullMdatPosBefore);

	if(iFileOs != 8)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		goto Exit;
	}

	if(td_mp4_fseek(TmpFileHandle, ullMdatPosAfter, SEEK_SET) < 0)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		goto Exit;
	}

	iFileOs = td_mp4_fread(&uiBaseUnitSizeBefore, 1, 4, TmpFileHandle);

	if(iFileOs != 4)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		goto Exit;
	}

	ConvertSort((unsigned char*)&uiBaseUnitSizeAfter, uiBaseUnitSizeBefore);
	uiInfoSize = uiBaseUnitSizeAfter - sizeof(TBaseUnit) - sizeof(unsigned int);

	if(td_mp4_fseek(TmpFileHandle, ullMdatPosAfter+sizeof(TBaseUnit), SEEK_SET) < 0)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		goto Exit;
	}

	iFileOs = td_mp4_fread(&iFlag, 1, 4, TmpFileHandle);

	if((iFileOs != 4)||(0x0123abcf != iFlag))
	{
		td_printf(0xff0000, "%s %s %d error!iFlag is %x", __FILE__, __FUNCTION__, __LINE__, iFlag);
		goto Exit;
	}

	pInfoBuf = (char*)malloc(uiInfoSize);

	if(NULL == pInfoBuf)
	{
		td_printf(0xff0000, "%s %s %d error!iFlag is %d", __FILE__, __FUNCTION__, __LINE__, iFlag);
		goto Exit;
	}

	iFileOs = td_mp4_fread(pInfoBuf, 1, uiInfoSize, TmpFileHandle);

	if(iFileOs != uiInfoSize)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		goto Exit;
	}

	*_pcInfoBuf = pInfoBuf;
	*_piCounts  = uiInfoSize/sizeof(TSdvFileTime);
	*_piInfoLen = uiInfoSize;

	iRet = td_success;
Exit:
	if(td_failure == iRet)
	{
		if(NULL != pInfoBuf)
		{
			free(pInfoBuf);
			pInfoBuf= NULL;
		}
	}

	if(NULL != TmpFileHandle)
	{
		td_mp4_fclose(TmpFileHandle);
		TmpFileHandle = NULL;
	}
	return iRet;
}

int MP4_GetFileAdjournTime(char* _pFilename, int* _piAdjournCnt, char** _pcAdjournInfo)
{
	int iRet = td_failure;
	int iMp4Ret = td_success;
	int i = 0;
	int iSdvFileCnt = 0;
	int iAdjournCnt = 0;
	char *pcInfoBuf = NULL;
	int iBufLen = 0;
	TSdvFileTime* ptSdvFileTime = NULL;
	TAdjournInfo* pAjournInfo = NULL;
	int iMemNum = 0;

	if((NULL == _pFilename)||(NULL == _piAdjournCnt)||(NULL == _pcAdjournInfo))
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		goto Exit;
	}
	
	iMp4Ret = MP4_GetSdvConvertInfo(_pFilename, &iSdvFileCnt, &pcInfoBuf, &iBufLen);
	ptSdvFileTime = (TSdvFileTime *)pcInfoBuf;

	if((td_success == iMp4Ret)&&(NULL != pcInfoBuf)&&(iSdvFileCnt > 0))
	{			
		for (i = 0; i < (iSdvFileCnt - 1); i++)
		{
			if (ptSdvFileTime[i + 1].m_iStartTime > (ptSdvFileTime[i].m_iEndTime + 10))
			{
				iAdjournCnt++;

				if(iAdjournCnt > iMemNum)
				{
					if(0 == iMemNum)
					{
						iMemNum = MAX_ADJOURN_TIMES;
						pAjournInfo = (TAdjournInfo*)malloc(sizeof(TAdjournInfo)*MAX_ADJOURN_TIMES);
					}
					else
					{
						iMemNum += MAX_ADJOURN_TIMES;
						pAjournInfo = (TAdjournInfo*)realloc(pAjournInfo, sizeof(TAdjournInfo)*iMemNum);
					}
					
					if(NULL == pAjournInfo)
					{
						td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
						goto Exit;
					}					
				}

				pAjournInfo[iAdjournCnt-1].m_iStartTime = ptSdvFileTime[i].m_iEndTime;
				pAjournInfo[iAdjournCnt-1].m_iEndTime   = ptSdvFileTime[i+1].m_iStartTime;
				
				td_printf(0,"iFreeCourtCnt= %d, tMsg.m_iBeginTm = %d, m_iEndTm = %d", iAdjournCnt, pAjournInfo[iAdjournCnt-1].m_iStartTime, pAjournInfo[iAdjournCnt-1].m_iEndTime);					
			}
		}
	}
	else 
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		goto Exit;
	}

	*_pcAdjournInfo = (char*)pAjournInfo;
	*_piAdjournCnt = iAdjournCnt;

	iRet = td_success;
Exit:		
	return iRet;		
}


int MP4_DirBurnUpdateMP4FileTime(unsigned long long _ullMP4Time)
{
	int iRet = td_failure;
	iRet = Inner_SetFileCreateTime(_ullMP4Time);
	return iRet;
}

void* MP4_DirBurnGetFileHeader(unsigned int _uiDevNo, unsigned int _uiMdatSize, int _iConvertType)
{
	td_printf(0, "%s start!", __FUNCTION__);
	int iRet = td_failure;
	MP4FileHandle pMP4FileHandle = NULL;
#ifdef FOR_MP4SDK
#else
	TMP4ConvertSession* pstMP4ConvertSession = NULL;

	if((_uiDevNo < 0)||(_iConvertType < 0))
	{
		td_printf(0xff0000, "%s %s %d error! Input Parameter error!\n", __FILE__, __FUNCTION__, __LINE__);
		goto Exit;
	}
	
	//´ò¿ªMP4 ÎÄ¼þ²¢ÉèÖÃÊÓÆµ²ÉÑùÆµÂÊ90000
	pMP4FileHandle = Inner_GetMP4FileHeader(MP4_ADD_INFO, NULL, _uiMdatSize);

	if (MP4_INVALID_FILE_HANDLE == pMP4FileHandle)  
	{  
		td_printf(0xff0000, "%s %s %d error! pMP4FileHandle is %p", 
			__FILE__, __FUNCTION__, __LINE__, pMP4FileHandle);
		goto Exit;
	}
	else
	{				
		pstMP4ConvertSession = DirBurn_AddConvertSession(pMP4FileHandle, _uiDevNo, _iConvertType);

		if(NULL == pstMP4ConvertSession)
		{
			td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
			goto Exit;
		}
	}
	iRet = td_success;
	
Exit:
	if(td_failure == iRet)
	{
		if((NULL != pMP4FileHandle)&&(NULL == pstMP4ConvertSession))
		{
			MP4CloseRT(pMP4FileHandle, MP4_CLOSE_DO_NOT_COMPUTE_BITRATE_V2, NULL, NULL);
			pMP4FileHandle = NULL;
		}

		if(NULL != pstMP4ConvertSession)
		{
			pthread_mutex_lock(&(pstMP4ConvertSession->m_SesssionLock));
			DirBurn_FreeConvertSession(pstMP4ConvertSession);
			pthread_mutex_unlock(&(pstMP4ConvertSession->m_SesssionLock));
		}
	}
	td_printf(0, "%s End!", __FUNCTION__);
#endif
	return pMP4FileHandle;
}

int MP4_DirBurnGetFileData(MP4FileHandle _pMP4FileHandle, char *_pSrcFile,  char* _pDstFile)
{
	td_printf(0, "%s start!", __FUNCTION__);
	int iRet = MP4_CONVERT_ERROR_FATAL;
	int iTmpRet = td_failure;
	TMP4ConvertSession* pstMP4ConvertSession = NULL;
	
	if(NULL == _pMP4FileHandle)
	{
		td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
		goto Exit2;
	}

	pstMP4ConvertSession = Inner_FindConvertSession(_pMP4FileHandle);

	if(NULL == pstMP4ConvertSession)
	{
		td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
		goto Exit2;
	}

	td_printf(0, "%s %d lock begin! 1", __FUNCTION__, __LINE__);
	pthread_mutex_lock(&(pstMP4ConvertSession->m_SesssionLock));
	td_printf(0, "%s %d lock begin! 2", __FUNCTION__, __LINE__);

	if(0 == pstMP4ConvertSession->m_iUsing)
	{
		td_printf(0xff0000, "%s %s %d error! the session has been freed!", __FILE__, __FUNCTION__, __LINE__);
		goto Exit1;
	}
	
	//¸üÐÂµ±Ç°SDV ×ª»»Ïà¹ØÐÅÏ¢
	iTmpRet = DirBurn_UpdateSessionMgrInfo(pstMP4ConvertSession, _pSrcFile, _pDstFile);

	if(iTmpRet < 0)
	{
		td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
		iRet = iTmpRet;
		goto Exit1;
	}

	if(1 == pstMP4ConvertSession->m_iConvertNO)
	{
		iTmpRet = DirBurn_UpDateSessionOtherInfo(pstMP4ConvertSession);
			
		if(iTmpRet < 0)
		{
			td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
			goto Exit1;
		}
	}
	
	//×ª»»µ±Ç°SDV ÐÅÏ¢
	iTmpRet = DirBurn_ConvertSessionFileData(pstMP4ConvertSession);

	if(iTmpRet < 0)
	{
		td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
		goto Exit1;
	}
	
	iRet = td_success;
	
Exit1:
	//ÊÍ·Åµ±Ç°SDV×ª»»¹ÜÀíÐÅÏ¢
	DirBurn_ReleaseSessionMgrSource(pstMP4ConvertSession);
	
	if(MP4_CONVERT_ERROR_FATAL == iRet)
	{
		DirBurn_FreeConvertSession(pstMP4ConvertSession);
	}

	td_printf(0, "%s %d unlock begin! 1", __FUNCTION__, __LINE__);
	pthread_mutex_unlock(&(pstMP4ConvertSession->m_SesssionLock));
	td_printf(0, "%s %d unlock begin! 2", __FUNCTION__, __LINE__);
Exit2:
	td_printf(0, "%s End!", __FUNCTION__);
	return iRet;
}

int MP4_DirBurnGetFileTail(void* _pMP4FileHandle, char* _pTailFilePath)
{
	td_printf(0, "%s start!", __FUNCTION__);
	int iRet = td_failure;
	//unsigned long long  TmpDatalen = 0;
	TMP4ConvertSession* pstMP4ConvertSession = NULL;

	if(NULL == _pMP4FileHandle)
	{
		td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
		goto Exit2;
	}

	pstMP4ConvertSession = Inner_FindConvertSession(_pMP4FileHandle);

	if(NULL == pstMP4ConvertSession)
	{
		td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
		goto Exit2;
	}

	td_printf(0, "%s %d lock begin! 1", __FUNCTION__, __LINE__);
	pthread_mutex_lock(&(pstMP4ConvertSession->m_SesssionLock));
	td_printf(0, "%s %d lock begin! 2", __FUNCTION__, __LINE__);

	if(0 == pstMP4ConvertSession->m_iUsing)
	{
		td_printf(0xff0000, "%s %s %d error! the session has been freed!", __FILE__, __FUNCTION__, __LINE__);
		goto Exit1;
	}
	
	if(pstMP4ConvertSession->m_iConvertType == DIRBURN_WITHSATA)
	{
		if(NULL == _pTailFilePath)
		{
			td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
			goto Exit1;
		}
		else
		{
			if(NULL != pstMP4ConvertSession->m_CurMP4Handle)
			{
				td_mp4_fclose((void*)pstMP4ConvertSession->m_CurMP4Handle);
				pstMP4ConvertSession->m_CurMP4Handle = NULL;
			}
		#ifdef TD_VFS
			pstMP4ConvertSession->m_CurMP4Handle = (TVfile*)td_mp4_fopen(NULL, _pTailFilePath, "wb+");
		#else
			pstMP4ConvertSession->m_CurMP4Handle = (FILE*)td_mp4_fopen(NULL, _pTailFilePath, "wb+");
		#endif
	
			if(NULL == pstMP4ConvertSession->m_CurMP4Handle)
			{
				td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
				goto Exit1;
			}
		}
	}

    //»ã×ÜMP4 ´óÎÄ¼þµÄÐéÄâÖ¡ÐÅÏ¢²¢Ìî³äµ½video trackÖÐ
	if(DirBurn_WriteSessionVirFrameInfo(pstMP4ConvertSession) < 0)
	{
		td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
		goto Exit1;
	}

	if(DIRBURN_WITHSATA == pstMP4ConvertSession->m_iConvertType)
	{
		//»ã×ÜMP4 ´óÎÄ¼þµÄsdv ×ª»»Ê±¼äÐÅÏ¢²¢¼ÇÂ¼µ½×îºóÒ»¸ömdat ÖÐ
		if(DirBurn_WriteSessionSdvTimeInfo(pstMP4ConvertSession) < 0)
		{
			td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
			goto Exit1;
		}
	}
	
	if(Inner_GetMP4Tail(&(pstMP4ConvertSession->m_stMP4Manager.m_MP4Handle)) < 0)
	{
		td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
		goto Exit1;
	}

	//td_printf(0, "pstMP4ConvertSession[%p], m_ullVideoTotalDuration[%llu], m_ullAudioTotalDuration[%llu]", 
		//pstMP4ConvertSession, 
		//pstMP4ConvertSession->m_ullVideoTotalDuration,
		//pstMP4ConvertSession->m_ullAudioTotalDuration);
	
	iRet = td_success;
Exit1:
	DirBurn_FreeConvertSession(pstMP4ConvertSession);
	td_printf(0, "%s %d unlock begin! 1", __FUNCTION__, __LINE__);
	pthread_mutex_unlock(&(pstMP4ConvertSession->m_SesssionLock));
	td_printf(0, "%s %d unlock begin! 2", __FUNCTION__, __LINE__);
Exit2:
	td_printf(0, "%s End!", __FUNCTION__);
	return iRet;
}

int MP4_DirBurnResum(unsigned int _uiDevNo, unsigned long long _ullDiskTotalSize, char* _pFilePath)
{
	int iRet = td_failure;
#ifdef FOR_MP4SDK
#else
	unsigned int uiMdatSize = 0;
	MP4FileHandle pMP4FileHandle = NULL;
	TMP4ResumSession* pMP4ResumSession = NULL;
	unsigned char* pVirtual = NULL;
	int iVitualLen = 0;
	S_FileHeader* pstFileHeader = NULL;
	TMP4ISampleIndexHead* pTMP4ISampleIndexHead = NULL;
	
	//²»×ãmp4 Í·²¿´óÐ¡160£¬Ö±½Ó·µ»Ø£¬²»×öÐÞ¸´
	if((_uiDevNo < 0)||(_ullDiskTotalSize <= 160)||(NULL == _pFilePath))	
	{
		td_printf(0xff0000, "%s %s %d error! _ullDiskTotalSize is %llu", __FILE__, __FUNCTION__, __LINE__, _ullDiskTotalSize);
		goto Exit2;
	}
	
	if(Inner_GetInfoFromFirstFree(READCDROM, NULL, _uiDevNo, "mdat", &uiMdatSize) < 0)
	{
		td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
		goto Exit2;
	}

	td_printf(0, "mdatsize is %u", uiMdatSize);

	//´ò¿ªMP4 ÎÄ¼þ²¢ÉèÖÃÊÓÆµ²ÉÑùÆµÂÊ90000
	pMP4FileHandle = Inner_GetMP4FileHeader(MP4_RT_MOOV, NULL, uiMdatSize);

	if(MP4_INVALID_FILE_HANDLE == pMP4FileHandle)
	{
		td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
		goto Exit2;
	}

	pMP4ResumSession = DirBurn_AddResumSession(pMP4FileHandle, _uiDevNo, _ullDiskTotalSize);

	if(NULL == pMP4ResumSession)
	{
		td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
		goto Exit2;
	}

	td_printf(0, "%s %d lock begin! 1", __FUNCTION__, __LINE__);
	pthread_mutex_lock(&(pMP4ResumSession->m_SesssionLock));
	td_printf(0, "%s %d lock begin! 2", __FUNCTION__, __LINE__);

	if(0 == pMP4ResumSession->m_iUsing)
	{
		td_printf(0xff0000, "%s %s %d error! the session has been freed!", __FILE__, __FUNCTION__, __LINE__);
		goto Exit1;
	}
	
	if(Inner_RecordFileBeginTime((signed char*)_pFilePath, &(pMP4ResumSession->m_iFileBeginTime)) < 0)
	{
		td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
		goto Exit1;
	}
	pMP4ResumSession->m_ullCDAddress += 160;//´ÓÍ·²¿ÒÔºó¿ªÊ¼·ÖÎö
	
	//±éÀúMP4 ÎÄ¼þ£¬·ÖÎöËùÓÐ×Ô¶¨Òåmdat 
	iRet= DirBurn_GetAndAnalasyAllCustomMdat(pMP4ResumSession, 1);
	if(iRet < 0)
	{
		if(MP4_DIRBURN_READCD_FAILED == iRet)
		{
			td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
			goto Exit1;
		}
		else if(MP4_DIRBURN_READCD_NOTENOUGH == iRet)
		{
			td_printf(0x12345678, "-----------------------------------------------------Create Resum Tail!");
			goto Tail;
		}
		else if(MP4_DIRBURN_READCD_OVER == iRet)
		{
			td_printf(0x12345678, "-----------------------------------------------------Create Resum Tail!");
			goto Tail;
		}
	}
	else
	{
		td_printf(0xff0000, "%s %s %d error! RetValue is Not Right!\n", __FILE__, __FUNCTION__, __LINE__);
		goto Exit1;
	}

Tail:
	//ÍêÉÆsdv ¸ñÊ½ÎÄ¼þÍ·ÐÅÏ¢
	td_printf(0, "~~~~~~~~~~~~~~~~~~~1 write resum mdat! complete fileheader!");
	pTMP4ISampleIndexHead = (TMP4ISampleIndexHead*)(pMP4ResumSession->m_puVirFrameBuf+MP4_BOX_SIEZ_LENGTH+MP4_SAMPLE_TYPE_LENGTH);
	pstFileHeader = &(pTMP4ISampleIndexHead->m_stExRecInfo.m_stSdvFileHead);
	pstFileHeader->Mode 				= 21;
	pstFileHeader->Year 				= 0xFFFF;
	pstFileHeader->Month				= 0xFFFF;
	pstFileHeader->Day  				= 0xFFFF;
	pstFileHeader->Hour 				= 0xFFFF;
	pstFileHeader->Minute 				= 0xFFFF;
	pstFileHeader->Second 				= 0xFFFF;
	pstFileHeader->CoverMask 			= 0xFFFF;	
	strncpy(pstFileHeader->cCovWord, FILE_HEADER_COVER_WORD, 35);		
	pstFileHeader->TriggerHigh 			= 0;
	pstFileHeader->TriggerLow  			= 0;
	pstFileHeader->reserved    			= MAX_COVWORD_LEN; 
	pstFileHeader->BitsPerSample 		= 16;

	//Í¨¹ýÊ±¼ä´Á¼ÆËãÎÄ¼þ×Ü³ÖÐøÊ±¼ä
	pTMP4ISampleIndexHead->m_stExRecInfo.m_iFileEndTime = 
	pMP4ResumSession->m_iFileBeginTime + (pMP4ResumSession->m_ullVideoTotalDuration / 90000);

	//td_printf(0, "%s m_ullVideoTotalDuration is %llu, m_ullAudioTotalDuration is %llu, m_iFileEndTime is %d", 
		//__FUNCTION__,
		//pMP4ResumSession->m_ullVideoTotalDuration,
		//pMP4ResumSession->m_ullAudioTotalDuration,
		//pTMP4ISampleIndexHead->m_stExRecInfo.m_iFileEndTime);

	//½«moov »òfree ÒÑÓÐµÄÊý¾Ý¸æËßµ×²ã
	td_printf(0, "pMP4ResumSession->m_iMoovOrFree is %d, pMP4ResumSession->m_uiMoovOrFreeLen is %d",
		pMP4ResumSession->m_iMoovOrFree, pMP4ResumSession->m_uiMoovOrFreeLen);
	if(!MP4AlignTail(pMP4ResumSession->m_MP4Handle, pMP4ResumSession->m_iMoovOrFree, pMP4ResumSession->m_uiMoovOrFreeLen))
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		goto Exit1;
	}

	td_printf(0, "~~~~~~~~~~~~~~~~~~~2 write I Index frame!");
	//ºÏ³É¿ÌÂ¼ÐéÄâÖ¡
	if(Inner_CompleteMP4VirFrameInfo(&(pMP4ResumSession->m_puVirFrameBuf), &pVirtual, &iVitualLen) < 0)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		goto Exit1;
	}

	td_printf(0, "m_uiLastBoxResumSum is %u", pMP4ResumSession->m_uiLastBoxResumSum);

	//½«²¹³äÊý¾Ý³¤¶È¸æËßµ×²ã
	if(!MP4WriteAlignData(pMP4ResumSession->m_MP4Handle, NULL, pMP4ResumSession->m_uiLastBoxResumSum + pMP4ResumSession->m_ullCDSize, iVitualLen))
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		goto Exit1;
	}
	
	//td_printf(0, "+++++duibi-------->MP4WriteSampleRT iDataLen[%d] _ullDataDuration[%lld] isSyncSample[%d]", iVitualLen, 0, 0);
	if(!MP4WriteSampleRT(pMP4ResumSession->m_MP4Handle, pMP4ResumSession->m_VideoTrackID, pVirtual, iVitualLen, 0, 0, 0, 1, NULL, NULL))
	{
		td_printf(0xff0000, "%d: Call MP4WriteSample failed!.\n", __LINE__);
		goto Exit1;
	}
	
	//ºÏ³ÉMP4 Î²
	td_printf(0, "~~~~~~~~~~~~~~~~~~~3 write MP4 Tail!");
	if(Inner_GetMP4Tail(&(pMP4ResumSession->m_MP4Handle)) < 0)
	{
		td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
		goto Exit1;
	}
	
	iRet = td_success;
Exit1:
	DirBurn_FreeMP4ResumSession(pMP4ResumSession);
	td_printf(0, "%s %d unlock begin! 1", __FUNCTION__, __LINE__);
	pthread_mutex_unlock(&(pMP4ResumSession->m_SesssionLock));
	td_printf(0, "%s %d unlock begin! 2", __FUNCTION__, __LINE__);
Exit2:
	if(td_success != iRet)
	{
		if((NULL != pMP4FileHandle)&&(NULL == pMP4ResumSession))
		{
			td_printf(0, "-----------------------------------------failed!");
			MP4CloseRT(pMP4FileHandle, MP4_CLOSE_DO_NOT_COMPUTE_BITRATE_V2, NULL, NULL);
			pMP4FileHandle = NULL;
		}
	}
#endif
	return iRet;
}

static void* DirBurn_ResetSession(void* _pstMP4ConvertSession)
{
	//¼ÇÂ¼Ïß³ÌÐÅÏ¢
#ifndef FOR_MP4SDK
	func_info();
#endif

	td_printf(0, "%s %d %s start!\n", __FILE__, __LINE__, __FUNCTION__);

	TMP4ConvertSession* pstMP4ConvertSession = (TMP4ConvertSession*)_pstMP4ConvertSession;

	if(NULL == pstMP4ConvertSession)
	{
		td_printf(0xff0000, "%s %s %d error! the session is NULL!", __FILE__, __FUNCTION__, __LINE__);
		goto Exit2;
	}

	//Èç¹û×ª»»»á»°Ä¿Ç°ÔÚ½øÐÐÖÐ£¬ÔòÖ»ÖÃÒ»¸ö×´Ì¬±êÊ¶£¬
	//ÓÉ×ª»»¹ý³Ì×Ô¶¯Í£ÏÂÀ´;  ·ñÔòÇ¿ÖÆÊÍ·Å»á»°×ÊÔ´¡£
	if((0 == pstMP4ConvertSession->m_iStopConvert)
		&&(TD_MP4_CONVERT_RUNNING == pstMP4ConvertSession->m_stMP4Manager.m_bConvertExitFlag))
	{
		pstMP4ConvertSession->m_iStopConvert = 1;
	}
	else
	{
		td_printf(0, "%s %d lock begin! 1", __FUNCTION__, __LINE__);
		pthread_mutex_lock(&(pstMP4ConvertSession->m_SesssionLock));
		td_printf(0, "%s %d lock begin! 2", __FUNCTION__, __LINE__);
	
		if(0 == pstMP4ConvertSession->m_iUsing)
		{
			td_printf(0xff0000, "%s %s %d error! the session has been freed!", __FILE__, __FUNCTION__, __LINE__);
			goto Exit1;
		}
	
		DirBurn_FreeConvertSession(pstMP4ConvertSession);
	
Exit1:
		td_printf(0, "%s %d unlock begin! 1", __FUNCTION__, __LINE__);
		pthread_mutex_unlock(&(pstMP4ConvertSession->m_SesssionLock));
		td_printf(0, "%s %d unlock begin! 2", __FUNCTION__, __LINE__);		
	}
Exit2:
	td_printf(0, "%s %d %s end!\n", __FILE__, __LINE__, __FUNCTION__);
	return NULL;
}

int MP4_StopConvert(void* _pMP4FileHandle)
{
	int iRet = td_failure;
	TMP4ConvertSession* pstMP4ConvertSession = NULL;
	pthread_t m_tThread;	
	
	if(NULL == _pMP4FileHandle)
	{
		td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
		goto Exit;
	}

	pstMP4ConvertSession = Inner_FindConvertSession(_pMP4FileHandle);

	if(NULL == pstMP4ConvertSession)
	{
		td_printf(0xff0000, "%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
		goto Exit;
	}

	//´´½¨Ïß³ÌÈ¥Ö´ÐÐÊÍ·Å²Ù×÷£¬·ÀÖ¹¼ÓËø×èÈûµ¼ÖÂÉÏ²ãËÀËø
//	iRet = pthread_create(&m_tThread, NULL, DirBurn_ResetSession, (void*)pstMP4ConvertSession);//cwm_todo

Exit:
	return iRet;
}

#endif
//----------------------------new interface end--------------------------------



int sdvToVideoRawData(int argc, char ** argv)
{

	char *pInFile = argv[2];
	char *pOutFile = argv[3];

	if(argc > 3)
	{
		pInFile = argv[2];
		pOutFile = argv[3];
	}else
	{
		pInFile = "tiandy.sdv";
		pOutFile = "tiandy.h265";
	}

	//´ò¿ªÌìµØÎÄ¼þ	
	FILE *pFileIn = fopen(pInFile, "rw");
	if(NULL == pFileIn)
	{
		PrintInfo("open failed");
		return -1;		
	}

	//¶ÁÈ¡ÎÄ¼þÍ·
	S_FileHeader stSDVFileHeadBuf = {0};
	size_t ret = fread((char *)&stSDVFileHeadBuf, 1, sizeof(S_FileHeader), pFileIn);
	if(ret != sizeof(S_FileHeader))
	{
		PrintInfo("read failed");
		return -1;
	}else
	{		
		show_fileHeadInfo(&stSDVFileHeadBuf);
		
		PrintInfo("sizeof(S_FileHeadert)=%d", sizeof(S_FileHeader));//88B
		PrintInfo("sizeof(S_FrameHeader)=%d", sizeof(S_FrameHeader));//32B
		//PrintInfo("curr offset=%ld", ftell(pFileIn));	
		//return 0;
	}
	
	FILE *pFileOut = fopen(pOutFile, "a+");
	if(NULL == pFileOut)
	{
		PrintInfo("open failed");
		return -1;		
	}

	int frameCount = 0;
	int inum = 0;
	while(1)
	{
		//PrintInfo("======================================================");
		S_FrameHeader stSDVAVFrameHead = {0};
		if(feof(pFileIn))
		{
			PrintInfo("end file");
			break;
		}		
		if(fread((char *)&stSDVAVFrameHead, 1, sizeof(stSDVAVFrameHead), pFileIn) != sizeof(stSDVAVFrameHead))
		{
			PrintInfo("read failed");
			if(feof(pFileIn))
			{
				PrintInfo("end file");
				break;
			}		
			return -1;
		}else
		{			
			show_framHeadInfo(&stSDVAVFrameHead);	
			//PrintInfo("sizeof(stSDVAVFrameHead)=%d", sizeof(stSDVAVFrameHead));
			//PrintInfo("curr offset=%ld", ftell(pFileIn));	

			switch(stSDVAVFrameHead.u8FrameType)
			{
				case 0:
					PrintInfo("FRAME_I");
					inum++;
					break;
				case 1:
					PrintInfo("FRAME_P");
					break;
				case 2:
					PrintInfo("FRAME_B");
					break;
				default:
					PrintInfo("default");
					break;
			}
			frameCount++;
		}
		char *pBuffer = NULL;
		if(stSDVAVFrameHead.u32VStreamLen > 0)
		{
			pBuffer = (char *)malloc(stSDVAVFrameHead.u32VStreamLen);
			if(NULL == pBuffer)
			{
				PrintInfo("malloc failed");
				return -1;
			}
		}

		if(fread(pBuffer, 1, stSDVAVFrameHead.u32VStreamLen, pFileIn) != stSDVAVFrameHead.u32VStreamLen)
		{
			PrintInfo("read failed");
			return -1;
		}else
		{	
			//PrintInfo("curr offset=%ld", ftell(pFileIn));		
			if(fseek(pFileIn, stSDVAVFrameHead.u16AFrameSize*stSDVAVFrameHead.u16AFrames, SEEK_CUR) != 0)
			{
				PrintInfo("fseek failed");
				return -1;
			}
		
		}
		
		//PrintInfo("curr offset=%ld", ftell(pFileIn));
#if 0
		PrintInfo("pBuffer[0]=0x%x", pBuffer[0]);
		PrintInfo("pBuffer[1]=0x%x", pBuffer[1]);
		PrintInfo("pBuffer[2]=0x%x", pBuffer[2]);
		PrintInfo("pBuffer[3]=0x%x", pBuffer[3]);
#endif
		PrintInfo("type=%d", (pBuffer[4]>>1)&0x3f);
		if(fwrite(pBuffer, 1, stSDVAVFrameHead.u32VStreamLen, pFileOut) != stSDVAVFrameHead.u32VStreamLen)
		{
			PrintInfo("write failed");
			return -1;
		}

		if(pBuffer != NULL)
		{
			free(pBuffer);
		}
		
	}
		
	PrintInfo("frameCount=%d", frameCount);
	PrintInfo("inum=%d", inum);

	fclose(pFileIn);
	fclose(pFileOut);

	return 0;
}


#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>




int sdvToMp4(int argc, char ** argv)
{
	int i = 0;
	for(i = 0; i < argc; i++)
	{
		PrintInfo("argv[%d]=%s", i, argv[i]);
	}

	if(argc > 3)
	{
		MP4_ConvertSdvToMp4(argv[2], argv[3]);
	}else
	{
		//MP4_ConvertSdvToMp4("tiandy.sdv", "tiandy.mp4");
		MP4_ConvertSdvToMp4("H31C0M20151219140904C35h264.sdv", "H31C0M20151219140904C35h264.mp4");
		//MP4_ConvertSdvToMp4("ch004_20151217115424t.sdv", "ch004_20151217115424t.mp4");
		//MP4_ConvertSdvToMp4Ex("H31C0M20151219140904C35.sdv", "H31C0M20151219140904C35.mp4", 0, 0);
		//MP4_ConvertSdvToMp4("H31C5M20151223134538C24.sdv", "H31C5M20151223134538C24.mp4");
		//MP4_ConvertSdvToMp4("ch07_20151223101732m9747.sdv", "ch07_20151223101732m9747.mp4");
	}

}



typedef struct{
	td_u8	*m_uiVTrackType;
	td_u32	m_uiVTrackId;
	AVCodecID	m_eVCodecType;
	td_u32	m_uiVNumberOfSamples;
	td_u32	m_uiVCurrSampleId;
	td_void	*m_pucVParameSet;
	td_u16	m_usVParameSetLen;
	td_u64	m_ulVIFrameOffset;
}TMP4DecVideo;

typedef struct{
	td_u8	*m_uiATrackType;
	td_u32	m_uiATrackId;
	td_u32	m_uiACodecType;
	td_u32	m_uiANumberOfSamples;
	td_u32	m_uiACurrSampleId;	
}TMP4DecAudio;

typedef struct{
	td_void			*m_pMP4Handle;
	TMP4DecVideo	m_stMP4DecVideo;
	TMP4DecAudio	m_stMP4DecAudio;
}TMP4Dec;

static td_s32 Inner_GetH264ParameSet(MP4FileHandle *_pMP4fh, td_u32 _trackId, td_void **_pParameSet, td_s16 *_pParameSetlen)
{
	int iRet = td_success;
	int iSpsFind = 0;
	int iPpsFind = 0;
	uint8_t **seqheader = NULL;
	uint8_t **pictheader = NULL;
	uint32_t *seqheadersize = NULL;
	uint32_t *pictheadersize = NULL;
	uint32_t ix;

	uint8_t spsBuf[512] = {0x00, 0x00, 0x00, 0x01, 0x00};
	uint8_t ppsBuf[512] = {0x00, 0x00, 0x00, 0x01, 0x00};
	uint32_t spsSize = sizeof(spsBuf);
	uint32_t ppsSize = sizeof(ppsBuf);

	if((NULL == _pMP4fh)||(NULL == _pParameSet)||(NULL == _pParameSetlen))
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}
	
	MP4GetTrackH264SeqPictHeaders(_pMP4fh, _trackId,
								  &seqheader, &seqheadersize,
								  &pictheader, &pictheadersize);


	int iCount = 0;
		
	if((NULL != seqheader)&&(NULL != pictheader)
		&&(NULL != seqheadersize)&&(NULL != pictheadersize))
	{
		for (ix = 0; seqheadersize[ix] != 0; ix++) 
		{
			if((seqheadersize[ix] < (spsSize - 4))&&(0 == iSpsFind))
			{
				memcpy(spsBuf + 4, seqheader[ix], seqheadersize[ix]);
				spsSize = 4 + seqheadersize[ix];
				iSpsFind = 1;
			}
				
			for(iCount = 0; iCount < seqheadersize[ix]; iCount++)
			{
				td_printf(0, "the sps[%d][%d] is :%x", ix, iCount, seqheader[ix][iCount]);	
			}
		
			free(seqheader[ix]);
			seqheader[ix] = NULL;
		}

		td_printf(0, "-------------------------------------------------------------->total sps num is %d", ix);
	
		for (ix = 0; pictheadersize[ix] != 0; ix++) 
		{
			if((pictheadersize[ix] < (ppsSize - 4))&&(0 == iPpsFind))
			{
				memcpy(ppsBuf + 4, pictheader[ix], pictheadersize[ix]);
				ppsSize = 4 + pictheadersize[ix];
				iPpsFind = 1;
			}

			for(iCount = 0; iCount < pictheadersize[ix]; iCount++)
			{
				td_printf(0, "the pps[%d][%d] is :%x", ix, iCount, pictheader[ix][iCount]); 
			}
		
			free(pictheader[ix]);
			pictheader[ix] = NULL;
		}

		if((0 == (spsSize - 4))||(0 == (ppsSize - 4)))
		{
			td_printf(0xff0000, "%s %s %d failed!", __FILE__, __FUNCTION__, __LINE__);
			iRet = td_failure;
			goto leave;
		}

		
		td_printf(0, "-------------------------------------------------------------->total pps num is %d", ix);

		*_pParameSetlen = spsSize + ppsSize;
		*_pParameSet= malloc(*_pParameSetlen);
		if(NULL == *_pParameSet)
		{
			td_printf(0xff0000, "%s %s %d failed!", __FILE__, __FUNCTION__, __LINE__);
			iRet = td_failure;
			goto leave;
		}

		memcpy(*_pParameSet, spsBuf, spsSize);
		memcpy(*_pParameSet + spsSize, ppsBuf, ppsSize);
	}

leave:
	if(NULL != seqheader)
	{
		free(seqheader);
		seqheader = NULL;
	}

	if(NULL != seqheadersize)
	{
		free(seqheadersize);
		seqheadersize = NULL;
	}

	if(NULL != pictheader)
	{
		free(pictheader);
		pictheader = NULL;
	}

	if(NULL != pictheadersize)
	{
		free(pictheadersize);
		pictheadersize = NULL;
	}

	return iRet;
}


static td_s32 Inner_GetH265ParameSet(MP4FileHandle *_pMP4fh, td_u32 _trackId, td_void **_pParameSet, td_s16 *_pParameSetlen)
{
	int iRet = td_success;
	int iVpsFind = 0;
	int iSpsFind = 0;
	int iPpsFind = 0;
	uint8_t **vidheader = NULL;
	uint8_t **seqheader = NULL;
	uint8_t **pictheader = NULL;
	uint32_t *pictheadersize = NULL;
	uint32_t *seqheadersize = NULL;
	uint32_t *vidheadersize = NULL;
	uint32_t ix;
	

	uint8_t vpsBuf[512] = {0x00, 0x00, 0x00, 0x01, 0x00};
	uint8_t spsBuf[512] = {0x00, 0x00, 0x00, 0x01, 0x00};
	uint8_t ppsBuf[512] = {0x00, 0x00, 0x00, 0x01, 0x00};
	uint32_t vpsSize = sizeof(vpsBuf);
	uint32_t spsSize = sizeof(spsBuf);
	uint32_t ppsSize = sizeof(ppsBuf);

	if((NULL == _pMP4fh)||(NULL == _pParameSet)||(NULL == _pParameSetlen))
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}
	
	MP4GetTrackH265SeqPictHeaders(_pMP4fh, _trackId,
								  &vidheader, &vidheadersize,
								  &seqheader, &seqheadersize,
								  &pictheader, &pictheadersize);

	int iCount = 0;
		
	if((NULL != vidheader)&&(NULL != seqheader)&&(NULL != pictheader)
		&&(NULL != vidheadersize)&&(NULL != seqheadersize)&&(NULL != pictheadersize))
	{
		//vps
		for (ix = 0; vidheadersize[ix] != 0; ix++) 
		{
			if((vidheadersize[ix] < (vpsSize - 4))&&(0 == iVpsFind))
			{
				memcpy(vpsBuf + 4, vidheader[ix], vidheadersize[ix]);
				vpsSize = 4 + vidheadersize[ix];
				iVpsFind = 1;
			}
				
			for(iCount = 0; iCount < vidheadersize[ix]; iCount++)
			{
				td_printf(0, "the sps[%d][%d] is :%x", ix, iCount, vidheader[ix][iCount]);	
			}
		
			free(vidheader[ix]);
		}
		//sps
		for (ix = 0; seqheadersize[ix] != 0; ix++) 
		{
			if((seqheadersize[ix] < (spsSize - 4))&&(0 == iSpsFind))
			{
				memcpy(spsBuf + 4, seqheader[ix], seqheadersize[ix]);
				spsSize = 4 + seqheadersize[ix];
				iSpsFind = 1;
			}
				
			for(iCount = 0; iCount < seqheadersize[ix]; iCount++)
			{
				td_printf(0, "the sps[%d][%d] is :%x", ix, iCount, seqheader[ix][iCount]);	
			}
		
			free(seqheader[ix]);
			seqheader[ix] = NULL;
		}

		td_printf(0, "-------------------------------------------------------------->total sps num is %d", ix);
		//pps
		for (ix = 0; pictheadersize[ix] != 0; ix++) 
		{
			if((pictheadersize[ix] < (ppsSize - 4))&&(0 == iPpsFind))
			{
				memcpy(ppsBuf + 4, pictheader[ix], pictheadersize[ix]);
				ppsSize = 4 + pictheadersize[ix];
				iPpsFind = 1;
			}

			for(iCount = 0; iCount < pictheadersize[ix]; iCount++)
			{
				td_printf(0, "the pps[%d][%d] is :%x", ix, iCount, pictheader[ix][iCount]); 
			}
		
			free(pictheader[ix]);
			pictheader[ix] = NULL;
		}

		if((0 == (vpsSize - 4))||(0 == (spsSize - 4))||(0 == (ppsSize - 4)))
		{
			td_printf(0xff0000, "%s %s %d failed!", __FILE__, __FUNCTION__, __LINE__);
			iRet = td_failure;
			goto leave;
		}
		
		td_printf(0, "-------------------------------------------------------------->total pps num is %d", ix);
		
		*_pParameSetlen = vpsSize + spsSize + ppsSize;
		*_pParameSet= malloc(*_pParameSetlen);
		if(NULL == *_pParameSet)
		{
			td_printf(0xff0000, "%s %s %d failed!", __FILE__, __FUNCTION__, __LINE__);
			iRet = td_failure;
			goto leave;
		}

		memcpy(*_pParameSet, vpsBuf, vpsSize);
		memcpy(*_pParameSet + vpsSize, spsBuf, spsSize);
		memcpy(*_pParameSet + vpsSize + spsSize, ppsBuf, ppsSize);
		
	}

leave:
	//vps
	if(NULL != vidheader)
	{
		free(vidheader);
		vidheader = NULL;
	}

	if(NULL != vidheadersize)
	{
		free(vidheadersize);
		vidheadersize = NULL;
	}
	//sps
	if(NULL != seqheader)
	{
		free(seqheader);
		seqheader = NULL;
	}

	if(NULL != seqheadersize)
	{
		free(seqheadersize);
		seqheadersize = NULL;
	}
	//pps
	if(NULL != pictheader)
	{
		free(pictheader);
		pictheader = NULL;
	}

	if(NULL != pictheadersize)
	{
		free(pictheadersize);
		pictheadersize = NULL;
	}

	return iRet;
}


static td_s32 Inner_GetParameSet(MP4FileHandle *_pMP4fh, td_u32 _trackId, AVCodecID _eAVCodecID, td_void **_pParameSet, td_s16 *_pParameSetlen)
{
	td_s32 iRet = td_success;

	if((NULL == _pMP4fh) || (NULL == _pParameSet) || (NULL == _pParameSetlen))
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}

	switch(_eAVCodecID)
	{
		case AV_CODEC_ID_H264:
			if(Inner_GetH264ParameSet(_pMP4fh, _trackId, _pParameSet, _pParameSetlen) < 0)
			{
				td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
				iRet = td_failure;
				goto leave;
			}
			break;
			
		case AV_CODEC_ID_HEVC:
			if(Inner_GetH265ParameSet(_pMP4fh, _trackId, _pParameSet, _pParameSetlen) < 0)
			{
				td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
				iRet = td_failure;
				goto leave;
			}
			break;
			
		default:
			break;
	}

	leave:

	return iRet;
}

static td_s32 Inner_InitTMP4DecVideo(MP4FileHandle *_pMP4fh, TMP4DecVideo *_pstTMP4DecVideo)
{
	td_s32 iRet = td_success;

	_pstTMP4DecVideo->m_uiVTrackType = MP4_VIDEO_TRACK_TYPE;
	_pstTMP4DecVideo->m_uiVTrackId = MP4_INVALID_TRACK_ID;
	_pstTMP4DecVideo->m_eVCodecType = 0;
	_pstTMP4DecVideo->m_uiVNumberOfSamples = 0;
	_pstTMP4DecVideo->m_uiVCurrSampleId = 1;
	_pstTMP4DecVideo->m_pucVParameSet = NULL;
	_pstTMP4DecVideo->m_usVParameSetLen = 0;
	
	if(MP4_GetTrackId_V2(_pMP4fh, &_pstTMP4DecVideo->m_uiVTrackId, _pstTMP4DecVideo->m_uiVTrackType) < 0)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}

	if(MP4_GetMP4VideoCodecId(_pMP4fh, &_pstTMP4DecVideo->m_eVCodecType) < 0)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}

	_pstTMP4DecVideo->m_uiVNumberOfSamples = MP4GetTrackNumberOfSamples(_pMP4fh, _pstTMP4DecVideo->m_uiVTrackId);
	_pstTMP4DecVideo->m_uiVCurrSampleId = 1;
	iRet = Inner_GetParameSet(_pMP4fh, 
					_pstTMP4DecVideo->m_uiVTrackId, 
					_pstTMP4DecVideo->m_eVCodecType, 
					&_pstTMP4DecVideo->m_pucVParameSet, 
					&_pstTMP4DecVideo->m_usVParameSetLen);

	if(iRet < 0)
	{		
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}

	leave:

	return iRet;
}

static int Inner_GetAudioType(char* _pcFileName, int* _piAudioType)
{
	td_printf(0, "%s start!", __FUNCTION__);
	td_s32 iRet = td_failure;
	char cFullPath[FULL_PATH_LEN] = {0};
	off_t OrgPos = 0;	
#ifdef TD_VFS
	TVfile* fp = NULL;	 
#else
	FILE* fp = NULL;		 
#endif

	if((NULL == _pcFileName)||(NULL == _piAudioType))
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		goto leave;
	}

	if(Inner_CompleteFileName(_pcFileName, cFullPath, FULL_PATH_LEN) < 0)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		goto leave;
	}

#ifdef TD_VFS
	fp = (TVfile*)td_mp4_fopen(NULL,cFullPath,"rb");
#else
	fp = (FILE*)td_mp4_fopen(NULL,cFullPath,"rb");
#endif

	if(NULL == fp)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		goto leave;
	}
	OrgPos = td_mp4_ftell(fp);
	td_mp4_fseek(fp, 0, SEEK_SET);
	if(Inner_GetInfoFromFirstFree(READFILE, (MP4_VOID*)fp, 0, (char*)AUDIO_ENCODE_TYPE, (unsigned int*)_piAudioType) < 0)
	{
		td_printf(0xff0000, "the file is may be a old mp4 file!");

		if(Inner_GetAudioTypeFromOldMP4File(cFullPath, _piAudioType) < 0)
		{
			td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
			goto leave;
		}
	}

	td_printf(0, "------------------->AudioType is %d", *_piAudioType);

	iRet = td_success;
	
leave:
	if(NULL != fp)
	{
		td_mp4_fseek(fp, OrgPos, SEEK_SET);
		td_mp4_fclose(fp);
		fp = NULL;
	}
	td_printf(0, "%s end!", __FUNCTION__);
    return iRet;
}


static td_s32 Inner_InitTMP4DecAudio(MP4FileHandle *_pMP4fh, TMP4DecAudio *_pstTMP4DecVideo)
{
	td_s32 iRet = td_success;

	td_u32	m_uiAudioType;
	td_u32	m_uiATrackId;
	td_u32	m_uiANumberOfSamples;
	td_u32	m_uiACurrSampleId;

	_pstTMP4DecVideo->m_uiATrackType = MP4_AUDIO_TRACK_TYPE;
	_pstTMP4DecVideo->m_uiATrackId = MP4_INVALID_TRACK_ID;
	_pstTMP4DecVideo->m_uiACodecType= 0; 
	_pstTMP4DecVideo->m_uiANumberOfSamples = 0;
	_pstTMP4DecVideo->m_uiACurrSampleId = 1;

	//get track id
	if(MP4_GetTrackId_V2(_pMP4fh, &_pstTMP4DecVideo->m_uiATrackId, _pstTMP4DecVideo->m_uiATrackType) < 0)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}
	//get number of samples
	_pstTMP4DecVideo->m_uiANumberOfSamples = MP4GetTrackNumberOfSamples(_pMP4fh, _pstTMP4DecVideo->m_uiATrackId);

	//set curr sample id
	_pstTMP4DecVideo->m_uiACurrSampleId = 1;

	leave:
		
	return iRet;
}


static Inner_ShowMP4DecInfo(TMP4Dec *_pstTMP4Dec)
{
	switch(_pstTMP4Dec->m_stMP4DecVideo.m_eVCodecType)
	{
		case AV_CODEC_ID_H264:
			PrintInfo("AV_CODEC_ID_H264");
			break;
			
		case AV_CODEC_ID_HEVC:
			PrintInfo("AV_CODEC_ID_HEVC");
			break;
			
		default:
			PrintInfo("invalic video invalid");
			break;
	}
		
	PrintInfo("m_uiVTrackId = %d", _pstTMP4Dec->m_stMP4DecVideo.m_uiVTrackId);
	PrintInfo("m_uiVNumberOfSamples = %d", _pstTMP4Dec->m_stMP4DecVideo.m_uiVNumberOfSamples);
	PrintInfo("m_uiVCurrSampleId = %d", _pstTMP4Dec->m_stMP4DecVideo.m_uiVCurrSampleId);
	PrintInfo("m_usVParameSetLen = %d", _pstTMP4Dec->m_stMP4DecVideo.m_usVParameSetLen);
	//PrintInfo("m_usVParameSetLen = %d", _pstTMP4Dec->m_stMP4DecVideo.m_pucVParameSet);
	
	PrintInfo("m_uiATrackType = %s", _pstTMP4Dec->m_stMP4DecAudio.m_uiATrackType);
	PrintInfo("m_uiATrackId = %d", _pstTMP4Dec->m_stMP4DecAudio.m_uiATrackId);
	PrintInfo("m_uiACodecType = %d", _pstTMP4Dec->m_stMP4DecAudio.m_uiACodecType);
	PrintInfo("m_uiANumberOfSamples = %d", _pstTMP4Dec->m_stMP4DecAudio.m_uiANumberOfSamples);
	PrintInfo("m_uiACurrSampleId = %d", _pstTMP4Dec->m_stMP4DecAudio.m_uiACurrSampleId);
}


static TMP4Dec *Inner_MallocTMP4Dec(td_char *_pcFileName)
{
	if(NULL == _pcFileName)
	{
		td_printf(0xff0000, "%s %s %d invalid _pcFileName!", __FILE__, __FUNCTION__, __LINE__);
		goto invalidFileName;
	}

	td_s32 iRet = td_success;
	TMP4Dec *pTMp4Dec = NULL;
	MP4FileHandle *pMP4FileHandle = NULL;
	pTMp4Dec = (TMP4Dec *)malloc(sizeof(TMP4Dec));
	if(NULL == pTMp4Dec)
	{
		td_printf(0xff0000, "%s %s %d NULL == pTMp4Dec malloc failed!", __FILE__, __FUNCTION__, __LINE__);
		goto mallocFailed;
	}
	
	td_char cFullPath[FULL_PATH_LEN] = {0};

	if(Inner_CompleteFileName(_pcFileName, cFullPath, FULL_PATH_LEN) < 0)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		goto getFullFileNameFailed;
	}
#ifdef CWM//FOR_MP4SDK //cwm_mod
		//add for windows 
		td_char cOutbuf[1024] = {0};
		AnsiToUTF8(cFullPath,strlen(cFullPath)+1,cOutbuf,sizeof(cOutbuf));
		pMP4FileHandle = MP4Read(_pcFileName);
		//add end
#else
		pMP4FileHandle = MP4Read(_pcFileName);
#endif
	
	if(MP4_INVALID_FILE_HANDLE == pMP4FileHandle)
	{
		td_printf(0xff0000, "%s %s %d invalid pMP4FileHandle!", __FILE__, __FUNCTION__, __LINE__);
		goto mp4ReadFailed;
	}
	
	pTMp4Dec->m_pMP4Handle = pMP4FileHandle;
	if(Inner_InitTMP4DecVideo(pTMp4Dec->m_pMP4Handle, &pTMp4Dec->m_stMP4DecVideo) < 0)
	{
		iRet = td_failure;
		td_printf(0xff0000, "%s %s %d Inner_InitTMP4DecVideo failed!", __FILE__, __FUNCTION__, __LINE__);
		goto mp4InitVFailed;
	}

	if(Inner_InitTMP4DecAudio(pTMp4Dec->m_pMP4Handle, &pTMp4Dec->m_stMP4DecAudio) < 0)
	{
		iRet = td_failure;
		td_printf(0xff0000, "%s %s %d Inner_InitTMP4DecAudio failed!", __FILE__, __FUNCTION__, __LINE__);
		goto mp4InitAFailed;
	}
	if(Inner_GetAudioType(_pcFileName, &pTMp4Dec->m_stMP4DecAudio.m_uiACodecType) < 0)
	{
		iRet = td_failure;
		td_printf(0xff0000, "%s %s %d Inner_GetAudioType failed!", __FILE__, __FUNCTION__, __LINE__);
		goto mp4InitAFailed;
	}
	
	Inner_ShowMP4DecInfo(pTMp4Dec);
	
mp4InitAFailed:
mp4InitVFailed:
mp4ReadFailed:
getFullFileNameFailed:
	if(td_failure == iRet)
	{
		if(NULL != pTMp4Dec)
		{
			free(pTMp4Dec);
			pTMp4Dec = NULL;
		}
	}
	
mallocFailed:
	
invalidFileName:

	return pTMp4Dec;	

}


static td_void Inner_FreeTMP4Dec(TMP4Dec *_pstTMP4Dec)
{

}


td_MP4FileHandle * MP4_Open (td_char *_pcFileName)
{
	TMP4Dec *pMP4Dec = NULL;
	if(NULL == _pcFileName)
	{
		td_printf(0xff0000, "%s %s %d invalid _pcFileName!", __FILE__, __FUNCTION__, __LINE__);
		goto invalidFileName;
	}
	
	td_char cFullPath[FULL_PATH_LEN] = {0};

	if(Inner_CompleteFileName(_pcFileName, cFullPath, FULL_PATH_LEN) < 0)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		goto getFullFileNameFailed;
	}
		
	pMP4Dec = Inner_MallocTMP4Dec(_pcFileName);
	if(NULL == pMP4Dec)
	{
		td_printf(0xff0000, "%s %s %d Inner_MallocTMP4Dec failed!", __FILE__, __FUNCTION__, __LINE__);
	}
	
	getFullFileNameFailed:
		
	invalidFileName:
		
	return (td_MP4FileHandle *)pMP4Dec;	
}

td_s32 MP4_Close(td_MP4FileHandle *_pMP4fh)	
{
	int iRet = td_success;
	
	if(NULL == _pMP4fh)
	{
		td_printf(0xff0000, "%s %s %d NULL == _pMP4fh!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}

	TMP4Dec *pTMP4Dec = (TMP4Dec *)_pMP4fh;

	if(MP4_INVALID_FILE_HANDLE != pTMP4Dec->m_pMP4Handle)
	{	
		MP4Close(pTMP4Dec->m_pMP4Handle, 0);
		pTMP4Dec->m_pMP4Handle = NULL;
	}else
	{
		td_printf(0xff0000, "%s %s %d invalid m_pMP4Handle!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
	}

	if(NULL != pTMP4Dec->m_stMP4DecVideo.m_pucVParameSet)
	{
		free(pTMP4Dec->m_stMP4DecVideo.m_pucVParameSet);
		pTMP4Dec->m_stMP4DecVideo.m_pucVParameSet= NULL;
	}else
	{
		td_printf(0xff0000, "%s %s %d m_pucVParameSet == NULL!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
	}

	pTMP4Dec->m_stMP4DecVideo.m_uiVTrackType = NULL;
	pTMP4Dec->m_stMP4DecVideo.m_uiVTrackId = MP4_INVALID_TRACK_ID;
	pTMP4Dec->m_stMP4DecVideo.m_eVCodecType = 0;
	pTMP4Dec->m_stMP4DecVideo.m_uiVNumberOfSamples = 0;
	pTMP4Dec->m_stMP4DecVideo.m_uiVCurrSampleId = 0;
	pTMP4Dec->m_stMP4DecVideo.m_usVParameSetLen = 0;
	free(pTMP4Dec);
	
	leave:

	return iRet;
}

static int Inner_ReadSdvFileHeader(void* _pMP4Handle, S_FileHeader* _pstFileHeader)
{
    int	iRet = td_success;
	void* pMP4ISampleIndex = NULL;
	int  iMP4ISampleIndexMallocLen = 0;
	TMP4ISampleIndexHead stMP4ISampleIndexHead;
	memset(&(stMP4ISampleIndexHead), 0, sizeof(TMP4ISampleIndexHead));
	TMP4ISampleIndexTail stMP4ISampleIndexTail = {0};

	if((NULL == _pMP4Handle)||(NULL == _pstFileHeader))
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}

	if(MP4_ReadSampleIndex_V2(_pMP4Handle, &stMP4ISampleIndexHead, &stMP4ISampleIndexTail, &pMP4ISampleIndex, &iMP4ISampleIndexMallocLen) < 0)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}
	
	if(0/*IsValidFileHeader(&(stMP4ISampleIndexHead.m_stExRecInfo.m_stSdvFileHead)) < 0*/)//cwm_todo
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}
	else
	{
		memcpy(_pstFileHeader, &(stMP4ISampleIndexHead.m_stExRecInfo.m_stSdvFileHead), sizeof(S_FileHeader));
	}
	
leave:
	if(NULL != pMP4ISampleIndex)
	{
		free(pMP4ISampleIndex);
		pMP4ISampleIndex = NULL;
	}
	return iRet;
}

td_s32 MP4_GetFileHead(td_MP4FileHandle *_pMP4fh, TMP4FileHead *_pstMP4FileHead)	
{
	int iRet = td_success;
	void* pMP4ISampleIndex = NULL;
	int  iMP4ISampleIndexMallocLen = 0;
	TMP4ISampleIndexHead *pstMP4ISampleIndexHead = &_pstMP4FileHead->m_stTMP4ISampleIndexHead;
	TMP4ISampleIndexTail *pstMP4ISampleIndexTail = &_pstMP4FileHead->m_stTMP4ISampleIndexTail;

	memset(pstMP4ISampleIndexHead, 0, sizeof(TMP4ISampleIndexHead));
	memset(pstMP4ISampleIndexTail, 0, sizeof(TMP4ISampleIndexTail));

	if((NULL == _pMP4fh)||(NULL == _pstMP4FileHead))
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}

	TMP4Dec *pTMP4Dec = (TMP4Dec *)_pMP4fh;
	
	td_void *_pMP4Handle = pTMP4Dec->m_pMP4Handle;
	if(NULL == _pMP4Handle)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}
		
	if(MP4_ReadSampleIndex_V2(_pMP4Handle, pstMP4ISampleIndexHead, pstMP4ISampleIndexTail, &pMP4ISampleIndex, &iMP4ISampleIndexMallocLen) < 0)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}
	
	if(0/*IsValidFileHeader(&(pstMP4ISampleIndexHead->m_stExRecInfo.m_stSdvFileHead)) < 0*/)//cwm_todo
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}
	
leave:
	if(NULL != pMP4ISampleIndex)
	{
		free(pMP4ISampleIndex);
		pMP4ISampleIndex = NULL;
	}
	return iRet;
}


td_s32 MP4_Read (td_MP4FileHandle *_pMP4fh, td_void *_pFrameBuf, td_u32 _bufLen)	
{
	int iRet = td_success;
#ifdef FOR_MP4SDK 
	uint8_t* videoframe = NULL;
#endif

	if((NULL == _pMP4fh)||(NULL == _pFrameBuf))
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}

	TMP4Dec *pTMP4Dec = (TMP4Dec *)_pMP4fh;
	MP4FileHandle _pMP4fp = pTMP4Dec->m_pMP4Handle;
	uint32_t _u32VideoTrackId = pTMP4Dec->m_stMP4DecVideo.m_uiVTrackId;
	uint32_t *_puiCurVideoSampleId = &pTMP4Dec->m_stMP4DecVideo.m_uiVCurrSampleId;	

	
	if(MP4_INVALID_TRACK_ID == _u32VideoTrackId)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}
										
	char nalu_header[] = {00,00,00,01}; 
					
	uint64_t video_timestamp = 0;
	MP4SampleId numSamples_vedio = MP4GetTrackNumberOfSamples(_pMP4fp, _u32VideoTrackId);
	uint32_t sample_size_vedio = MAX_VIDEO_SAMPLE_SIZE;
#ifdef FOR_MP4SDK
	videoframe = (uint8_t*)malloc(MAX_VIDEO_SAMPLE_SIZE + MAX_ExInfo_SIZE);
	if (NULL == videoframe)
	{
		goto leave;
	}
#else
	uint8_t videoframe[MAX_VIDEO_SAMPLE_SIZE + MAX_ExInfo_SIZE];
#endif
	uint32_t videoframesize = 0;
	MP4Duration sampleDuration_vedio = 0;
	bool isSyncSample_vedio = false;
	
	//×î´óµ½MP4 ÐéÄâÖ¡Ö®Ç°Ò»Ö¡
	if((0 >= *_puiCurVideoSampleId)||(*_puiCurVideoSampleId >= (numSamples_vedio - 1)))
	{
		td_printf(0xff0000, "%s %s %d error! *_piCurVideoSampleId is %d, numSamples_vedio is %d", __FILE__, __FUNCTION__, __LINE__, *_puiCurVideoSampleId, numSamples_vedio);
		iRet = td_failure;
		goto leave;
	}

	unsigned int iSpsPpsSize = pTMP4Dec->m_stMP4DecVideo.m_usVParameSetLen;
	void *pParameSet = pTMP4Dec->m_stMP4DecVideo.m_pucVParameSet;
	unsigned int u32VideoPos = 0;
	

	if((iSpsPpsSize + sizeof(nalu_header)) > MAX_ExInfo_SIZE)
	{
		td_printf(0xff0000, "%s %s %d error!u32Pos is %u", __FILE__, __FUNCTION__, __LINE__, _iSpsSize + sizeof(nalu_header));
		iRet = td_failure;
		goto leave;
	}

	if(Inner_ReadOneVideoSample(_pMP4fp,_u32VideoTrackId,*_puiCurVideoSampleId,
		videoframe+iSpsPpsSize+sizeof(nalu_header),&sample_size_vedio,&sampleDuration_vedio,&isSyncSample_vedio) < 0)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}

	if(true == isSyncSample_vedio)
	{
		u32VideoPos = 0;
		memcpy(videoframe, pParameSet, iSpsPpsSize);
		u32VideoPos += iSpsPpsSize;
		memcpy(videoframe + u32VideoPos, nalu_header, sizeof(nalu_header));
		u32VideoPos += sizeof(nalu_header);
		
		u32VideoPos += sizeof(nalu_header);//¶Á³öÀ´µÄÊý¾ÝÍ·ËÄ×Ö½ÚÎÞÐ§,ÔÙÆ«ÒÆ4×Ö½Ú
		videoframesize = iSpsPpsSize + sample_size_vedio;//ÒòÆ«ÒÆ4¸ö×Ö½Ú£¬¹Ê×Ü´óÐ¡ÒªËã×¼
	}
	else
	{
		u32VideoPos = iSpsPpsSize;
		memcpy(videoframe + u32VideoPos, nalu_header, sizeof(nalu_header));
		u32VideoPos += sizeof(nalu_header);
		
		u32VideoPos += sizeof(nalu_header);//¶Á³öÀ´µÄÊý¾ÝÍ·ËÄ×Ö½ÚÎÞÐ§,ÔÙÆ«ÒÆ4×Ö½Ú
		videoframesize = sample_size_vedio;//ÒòÆ«ÒÆ4¸ö×Ö½Ú£¬¹Ê×Ü´óÐ¡ÒªËã×¼
	}

	video_timestamp = (MP4GetSampleTime(_pMP4fp,_u32VideoTrackId,*_puiCurVideoSampleId)*1000/90000);
	(*_puiCurVideoSampleId)++;//ÊÓÆµÖ¡ºÅµÝÔö
	//td_printf(0, "------------>VideoSampleId is %d", *_puiCurVideoSampleId);
	
	uint8_t audioframe[MAX_AUDIO_FRAMENUM*MAX_AUDIO_SAMPLE_SIZE];
	int audioframebuflen = sizeof(audioframe);
	uint32_t audioframesize = 0;
	uint32_t _u32AudioTrackId = pTMP4Dec->m_stMP4DecAudio.m_uiATrackId;
	MP4SampleId *_puiCurAudioSampleId = &pTMP4Dec->m_stMP4DecAudio.m_uiACurrSampleId;
	int _iAudioType = pTMP4Dec->m_stMP4DecAudio.m_uiACodecType;
	S_FrameHeader *_pstFrmaeHeader = (S_FrameHeader *)_pFrameBuf;
	void *_pTiandyFrameBuf = (uint8_t *)_pFrameBuf + sizeof(S_FrameHeader);
		
	if(Inner_GetAudioSamples(_pMP4fp,video_timestamp,_u32AudioTrackId,
		_puiCurAudioSampleId,_iAudioType,audioframe,audioframebuflen,_pstFrmaeHeader,0) < 0)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		audioframesize = 0;
	}
	else
	{
		audioframesize = (_pstFrmaeHeader->u16AFrames)*(_pstFrmaeHeader->u16AFrameSize);
	}

	if(videoframesize > 0)
	{
		Inner_BuildFreamHeader(_pstFrmaeHeader, isSyncSample_vedio, *_puiCurVideoSampleId - 1, 
			video_timestamp, videoframesize, 0);

		if((videoframesize+audioframesize) < MAXBUFEX)
		{			
			int iBufPos = 0;
			
			if(true == isSyncSample_vedio)
			{
				memcpy((MP4_VOID*)_pTiandyFrameBuf + iBufPos, videoframe, iSpsPpsSize + sizeof(nalu_header));
				iBufPos += (iSpsPpsSize + sizeof(nalu_header));
				memcpy((MP4_VOID*)_pTiandyFrameBuf + iBufPos, videoframe + u32VideoPos, sample_size_vedio - sizeof(nalu_header));
				iBufPos += (sample_size_vedio - sizeof(nalu_header));
			}
			else
			{
				memcpy((MP4_VOID*)_pTiandyFrameBuf + iBufPos, videoframe + iSpsPpsSize, sizeof(nalu_header));
				iBufPos += sizeof(nalu_header);
				memcpy((MP4_VOID*)_pTiandyFrameBuf + iBufPos, videoframe + u32VideoPos, sample_size_vedio - sizeof(nalu_header));
				iBufPos += (sample_size_vedio - sizeof(nalu_header));
			}

			memcpy((MP4_VOID*)_pTiandyFrameBuf + iBufPos, audioframe, audioframesize);
		}
		else
		{
			td_printf(0xff0000, "the length has beyond limit!");
			iRet = td_failure;
			goto leave;
		}

		iRet = td_success;
	}	
	else
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}
leave:
#ifdef FOR_MP4SDK
	if (videoframe)
	{
		free(videoframe);
		videoframe = NULL;
	}
#endif
	return iRet;
}

static td_s32 Inner_FindSyncFrame(MP4FileHandle _pMP4fh, td_u32 _trackId, MP4SampleId startSampleId, MP4SampleId *sampleId)
{
	int iRet = td_success;
	
	*sampleId = 0;
	MP4SampleId findSampleId = 0;

	MP4Timestamp timestamp = MP4GetSampleTime(_pMP4fh, _trackId, startSampleId);
	if(MP4_INVALID_TIMESTAMP == timestamp)
	{
		td_printf(0xff0000, "%s %s %d MP4GetSampleTime failed!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}
	
	findSampleId = MP4GetSampleIdFromTime(_pMP4fh, _trackId, timestamp, true);	
	if(MP4_INVALID_SAMPLE_ID == findSampleId)
	{
		td_printf(0xff0000, "%s %s %d MP4GetSampleIdFromTime failed!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}

	*sampleId = findSampleId;
	
leave:

	return iRet;
}

static td_s32 Inner_SeekForward(MP4FileHandle _pMP4fh, TMP4DecVideo *_pTMP4DecVideo, td_u64 _frameOffset)
{
	int iRet = td_success;	
	
	uint32_t trackId = _pTMP4DecVideo->m_uiVTrackId;
	MP4SampleId startSampleId = _pTMP4DecVideo->m_uiVCurrSampleId;
	
	if(startSampleId > _frameOffset)
	{
		startSampleId -= _frameOffset;
	}else
	{
		td_printf(0xff0000, "%s %s %d _frameOffset=%lld invalid!", __FILE__, __FUNCTION__, __LINE__, _frameOffset);
		goto leave;
	}	

	while(startSampleId > 0)
	{
	
		if(MP4GetSampleSync(_pMP4fh, trackId, startSampleId) == 1)
		{
			td_printf(0xff0000, "%s %s %d find sample(%lld) sync successful!", __FILE__, __FUNCTION__, __LINE__, startSampleId);
			_pTMP4DecVideo->m_uiVCurrSampleId = startSampleId;
			break;
		}
		
		startSampleId--;
	}

leave:
	
	return iRet;
}


static td_s32 Inner_SeekBackward(MP4FileHandle _pMP4fh, TMP4DecVideo *_pTMP4DecVideo, td_u64 _frameOffset)
{
	int iRet = td_success;	
	
	uint32_t trackId = _pTMP4DecVideo->m_uiVTrackId;
	MP4SampleId startSampleId = _pTMP4DecVideo->m_uiVCurrSampleId;
	td_u32	numberOfSamples = _pTMP4DecVideo->m_uiVNumberOfSamples;
	
	if((startSampleId + _frameOffset) < numberOfSamples)
	{
		startSampleId += _frameOffset;
	}else
	{
		td_printf(0xff0000, "%s %s %d _frameOffset=%lld invalid!", __FILE__, __FUNCTION__, __LINE__, _frameOffset);
		goto leave;
	}	

	while(startSampleId <= numberOfSamples)
	{
	
		if(MP4GetSampleSync(_pMP4fh, trackId, startSampleId) == 1)
		{
			td_printf(0xff0000, "%s %s %d find sample(%lld) sync successful!", __FILE__, __FUNCTION__, __LINE__, startSampleId);
			_pTMP4DecVideo->m_uiVCurrSampleId = startSampleId;
			break;
		}
		
		startSampleId++;
	}
	
leave:
	
	return iRet;
}

static td_s32 Inner_SeekAbsolute(MP4FileHandle _pMP4fh, TMP4DecVideo *_pTMP4DecVideo, td_u64 _frameId)
{
	int iRet = td_success;	

	uint32_t trackId = _pTMP4DecVideo->m_uiVTrackId;
	td_u32	numberOfSamples = _pTMP4DecVideo->m_uiVNumberOfSamples;
	MP4SampleId startSampleId = _frameId;
	
	if((_frameId < 1) || (_frameId > numberOfSamples))
	{
		td_printf(0xff0000, "%s %s %d _frameId=%lld invalid!", __FILE__, __FUNCTION__, __LINE__, _frameId);
		iRet = td_failure;
		goto leave;
	}
	

	while(startSampleId > 0)
	{
	
		if(MP4GetSampleSync(_pMP4fh, trackId, startSampleId) == 1)
		{
			td_printf(0xff0000, "%s %s %d find sample(%lld) sync successful!", __FILE__, __FUNCTION__, __LINE__, startSampleId);
			_pTMP4DecVideo->m_uiVCurrSampleId = startSampleId;
			break;
		}
		
		startSampleId--;
	}
	
leave:
	
	return iRet;
}

td_s32 MP4_Seek(td_MP4FileHandle *_pMP4fh, td_u64 _frameId, ESeekOps _type)	
{	
	int iRet = td_success;

	if(NULL == _pMP4fh)
	{
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		iRet = td_failure;
		goto leave;
	}

	TMP4Dec *pTMP4Dec = (TMP4Dec *)_pMP4fh;
	MP4FileHandle _pMP4fp = pTMP4Dec->m_pMP4Handle;

	switch(_type)
	{
		case SEEK_FORWARD:
 			Inner_SeekForward(_pMP4fp, &pTMP4Dec->m_stMP4DecVideo, _frameId);
			break;
			
		case SEEK_BACKWARD:
 			Inner_SeekBackward(_pMP4fp, &pTMP4Dec->m_stMP4DecVideo, _frameId);
			break;
				
		case SEEK_ABSOLUTE:
 			Inner_SeekAbsolute(_pMP4fp, &pTMP4Dec->m_stMP4DecVideo, _frameId);
			break;
		default:
			td_printf(0xff0000, "%s %s %d invalid ops!", __FILE__, __FUNCTION__, __LINE__);
			break;			
	}

leave:

	return iRet;
}



int decode_Init(void *_This, int *_piStopFlag)
{
	Decode *pDec = _This;
	pDec->m_Stream.m_iForceDestroyFlag = 0;
	pDec->m_Stream.m_fp = NULL;
	pDec->m_Stream.m_pTVDec = NULL;
	pDec->m_Stream.m_AO.m_iAoId = -1;
	pDec->m_Stream.m_pVO = NULL;
	memset(&pDec->m_Stream.m_vdecPara,0,sizeof(TVdecPara));
	memset(pDec->m_Stream.m_File,0,FILENAMEMAXNUM);
	pDec->m_Stream.m_FileLoction = 0;
	pDec->m_Stream.m_iOnlyIFrame = 0;
	pDec->m_Stream.m_u16BefAudioFrames = MAXAUDIOFRAMES;
	pDec->m_Stream.m_u16ZeroAudioFrames = 0;
	pDec->m_Stream.m_pIFrameIndex = NULL;
	pDec->m_Stream.m_mollocLen = 0;
	pDec->m_Stream.m_IFrameLen = 0;
	pDec->m_Stream.m_IFrameLoaction = 0;
	pDec->m_Stream.m_iFindNoIndex = 0;

	pDec->m_Stream.m_iFileStartTime = 0;
	pDec->m_Stream.m_iFileEndTime = 0;

	pDec->m_Stream.m_iLeaveAudioFrames = 0;
	pDec->m_Stream.m_iLeaveAudioLoaction = 0;
	pDec->m_Stream.m_iFrameRate = 0;
	//µ¹·Å
	pDec->m_Stream.m_piStop = _piStopFlag;
	pDec->m_Stream.m_BackId = 0;
	pDec->m_Stream.m_iBackFlag = 0;
	pDec->m_Stream.m_iSeparate = 0;
	pDec->m_Stream.m_iVoHide = 0;
	pDec->m_Stream.m_iBackClip = PLAYBACK_BACK_NOCLIP;
	memset(pDec->m_Stream.m_tOrigData, 0, sizeof(pDec->m_Stream.m_tOrigData));
	//×¥ÅÄ
	Play_Clr_All(pDec->m_Stream.m_iOptFlag);
	//
	memset(pDec->m_Stream.m_ucEncrypt,0,ENCRYPT_PWD_LENGTH + 1);
	memset(&pDec->m_Stream.m_FileHeader,0,sizeof(S_FileHeader));
	memset(&pDec->m_Stream.m_FrameHeader,0,sizeof(S_FrameHeader));
	pDec->m_Stream.m_tBackMem.m_data = NULL;
//	decode_clearLeaveAudioStream(&pDec->m_Stream);
	
	//MP4--ZS
	pDec->m_Stream.m_iFileType = FILE_TYPE_SDV;//Ä¬ÈÏsdvÎÄ¼þ
	pDec->m_Stream.m_stMP4Info.m_pMp4_fp = NULL;
	pDec->m_Stream.m_stMP4Info.m_u32Version = 0;
	pDec->m_Stream.m_stMP4Info.m_u32VideoTrackId = 0;
	pDec->m_Stream.m_stMP4Info.m_u32AudioTrackId = 0;
	pDec->m_Stream.m_stMP4Info.m_iAudioType = -1;
	memset(pDec->m_Stream.m_stMP4Info.m_cSps, 0, sizeof(pDec->m_Stream.m_stMP4Info.m_cSps));
	pDec->m_Stream.m_stMP4Info.m_iSpsSize = sizeof(pDec->m_Stream.m_stMP4Info.m_cSps);
	memset(pDec->m_Stream.m_stMP4Info.m_cPps, 0, sizeof(pDec->m_Stream.m_stMP4Info.m_cPps));
	pDec->m_Stream.m_stMP4Info.m_iPpsSize = sizeof(pDec->m_Stream.m_stMP4Info.m_cPps);
	pDec->m_Stream.m_stMP4Info.m_iEncryptFlag = 0;
	memset(&(pDec->m_Stream.m_stMP4Info.m_stMP4ISampleIndexHead), 0, sizeof(TMP4ISampleIndexHead));
	memset(&(pDec->m_Stream.m_stMP4Info.m_stMP4ISampleIndexTail), 0, sizeof(TMP4ISampleIndexTail));
	pDec->m_Stream.m_stMP4Info.m_uiCurVideoSampleId = 1;
	pDec->m_Stream.m_stMP4Info.m_uiCurAudioSampleId = 1;
	pDec->m_Stream.m_stMP4Info.m_u32UseLessIdCount = 0;
	pDec->m_Stream.m_stMP4Info.m_iIMP4SampleIndexmollocLen = 0;
	pDec->m_Stream.m_stMP4Info.m_pIMP4SampleIndex = NULL;
	#if 0
	pDec->Init = decode_Init;
	pDec->Destroy = decode_Destroy;
	pDec->ForceDestroy = decode_ForceDestroy;
	pDec->CreateVo = decode_CreateVo;
	pDec->CreateVdec = decode_CreateVdec;
	pDec->GetRate = decode_GetRate;
	pDec->SetRate = decode_SetRate;
	pDec->FullScr = decode_FullScr;
	pDec->FullScrExit = decode_FullScrExit;
	pDec->OpenFile = decode_OpenFile;
	pDec->CloseFile = decode_CloseFile;
	pDec->ReadFrame = decode_ReadFrame;
	pDec->SendVStream = decode_SendVStream;
	pDec->SendAStream = decode_SendAStream;
	pDec->SetPassword = decode_SetPassword;
	pDec->GetFileHeader = decode_GetFileHeader;
	pDec->GetFileName = decode_GetFileName;
	pDec->GetFileAttribute = decode_GetFileAttribute;
	pDec->SeekPos = decode_SeekPos;
	pDec->ReleaseVStream = decode_ReleaseVStream;
	pDec->DestroyAo = decode_DestroyAo;
	pDec->GetFrameTm = decode_GetFrameTm;
	pDec->TestEncrypt = decode_TestEncrypt;
	pDec->VoContrl = decode_VoContrl;
	pDec->DestroyVo = decode_DestroyVo;
	pDec->DestroyVdec = decode_DestroyVdec;
	pDec->SendLeaveAudioFrames = decode_SendLeaveAudioFrames;
	pDec->clearLeaveAudioStream =  decode_clearLeaveAudioStream;
	pDec->capPicture = decoder_capPicture;
	pDec->init_back = decode_init_back;
	pDec->destroy_back = decode_destroy_back;
	pDec->getOptStat = decode_getOptStat;
	pDec->m_Stream.m_pBuf = malloc(MAXBUF);
	if(pDec->m_Stream.m_pBuf == NULL)
	{
		return -1;
	}

	pthread_cond_init(&pDec->m_Stream.m_iMemCondFullLock, NULL);
	pthread_mutex_init(&pDec->m_Stream.m_StructLock,NULL);
	#endif
	
	return td_success;
}


static int decode_GetMP4FileInfo(TStream *_This,IN td_s8* _cFileName)
{
	int iRetValue = td_failure;
	TStream *pStream = _This;	

	if((pStream == NULL)||((_cFileName == NULL)))
	{
		goto leave;
	}
	//»ñÈ¡mp4ÎÄ¼þ±àÂëÀàÐÍcwm
	if(MP4_GetMP4VideoCodecId(pStream->m_stMP4Info.m_pMp4_fp, (AVCodecID *)&(pStream->m_stMP4Info.m_iVideoType)) < 0)
	{
		PrintInfo("MP4_GetMP4VideoCodecId failed!");
		td_printf(0xff0000,"%s %s %d error!\n", __FILE__, __FUNCTION__, __LINE__);
		goto leave;
	}

	//»ñÈ¡MP4 °æ±¾
	if(MP4_GetMP4FreeVerSion((char*)_cFileName, 2, &(pStream->m_stMP4Info.m_u32Version)) < 0)
	{
		PrintInfo("MP4_GetMP4FreeVerSion failed!");
		td_printf(0xff0000,"%s %s %d, _pcFileName = NULL\n", __FILE__, __FUNCTION__, __LINE__);
		goto leave;
	}
	
	//»ñÈ¡MP4ÎÄ¼þÊÓÆµtrackId
	//pStream->m_stMP4Info.m_u32VideoTrackId = MP4_INVALID_TRACK_ID;
	if(MP4_GetTrackId_V2(pStream->m_stMP4Info.m_pMp4_fp, &(pStream->m_stMP4Info.m_u32VideoTrackId), "vide") < 0)//MP4_VIDEO_TRACK_TYPE
	{
		PrintInfo("MP4_GetTrackId_V2 failed!");
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		goto leave;
	}
	
	PrintInfo("pStream->m_stMP4Info.m_u32VideoTrackId=%d", pStream->m_stMP4Info.m_u32VideoTrackId);

	//»ñÈ¡MP4ÎÄ¼þÒôÆµtrackId
	//pStream->m_stMP4Info.m_u32AudioTrackId = MP4_INVALID_TRACK_ID;
	if(MP4_GetTrackId_V2(pStream->m_stMP4Info.m_pMp4_fp, &(pStream->m_stMP4Info.m_u32AudioTrackId), "soun") < 0)
	{
		PrintInfo("MP4_GetTrackId_V2 failed!");
		//ÒôÆµ»ñÈ¡²»µ½²»´ú±í´íÎó£¬²»·µ»Ø
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
	}
	else
	{
		PrintInfo("pStream->m_stMP4Info.m_u32AudioTrackId=%d", pStream->m_stMP4Info.m_u32AudioTrackId);
		//»ñÈ¡ÒôÆµÀàÐÍ
		if(MP4_GetAudioType((char*)_cFileName,&(pStream->m_stMP4Info.m_iAudioType)) < 0)
		{
			PrintInfo("MP4_GetAudioType failed!");
			//ÒôÆµÀàÐÍ»ñÈ¡²»µ½²»´ú±íÎÄ¼þ´íÎó£¬²»·µ»Ø
			td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		}
	}

	if(AV_CODEC_ID_H264 == pStream->m_stMP4Info.m_iVideoType)
	{
		PrintInfo("AV_CODEC_ID_H264 == pStream->m_stMP4Info.m_iVideoType");
		//»ñÈ¡MP4ÎÄ¼þSPS¡¢PPS(Ã¿¸öMP4ÎÄ¼þÖ»ÓÐÒ»¸öSPSºÍPPS)
		if(MP4_GetSpsPps_V2(pStream->m_stMP4Info.m_pMp4_fp, pStream->m_stMP4Info.m_cSps, &(pStream->m_stMP4Info.m_iSpsSize), 
				pStream->m_stMP4Info.m_cPps, &(pStream->m_stMP4Info.m_iPpsSize)) < 0)
		{
			PrintInfo("MP4_GetSpsPps_V2 failed!");
			td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
			goto leave;
		}
	}else if(AV_CODEC_ID_HEVC == pStream->m_stMP4Info.m_iVideoType)
	{
		PrintInfo("AV_CODEC_ID_HEVC == pStream->m_stMP4Info.m_iVideoType");
		//»ñÈ¡MP4ÎÄ¼þVPS¡¢SPS¡¢PPS(h265±àÂëµÄÊÓÆµÎÄ¼þ¶à³öÒ»¸öVPS²ÎÊý¼¯)
		if(MP4_GetVpsSpsPps_V2(pStream->m_stMP4Info.m_pMp4_fp, 
								pStream->m_stMP4Info.m_cVps, &(pStream->m_stMP4Info.m_iVpsSize),
								pStream->m_stMP4Info.m_cSps, &(pStream->m_stMP4Info.m_iSpsSize), 
								pStream->m_stMP4Info.m_cPps, &(pStream->m_stMP4Info.m_iPpsSize)) < 0)
		{
			PrintInfo("MP4_GetVpsSpsPps_V2 failed!");
			td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
			goto leave;
		}

	}


	//¶ÁÈ¡MP4ÎÄ¼þË÷ÒýÐÅÏ¢
	if(1 == pStream->m_stMP4Info.m_u32Version)
	{
		PrintInfo("1 == pStream->m_stMP4Info.m_u32Version");
		if(MP4_ReadSampleIndex_V3((char*)_cFileName, &(pStream->m_stMP4Info.m_stMP4ISampleIndexHead), 
			&(pStream->m_stMP4Info.m_stMP4ISampleIndexTail), &(pStream->m_stMP4Info.m_pIMP4SampleIndex), 
			&(pStream->m_stMP4Info.m_iIMP4SampleIndexmollocLen)) < 0)
		{
			PrintInfo("MP4_ReadSampleIndex_V3 failed!");
			td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
			goto leave;
		}
	}
	else
	{
		PrintInfo("1 != pStream->m_stMP4Info.m_u32Version");
		if(MP4_ReadSampleIndex_V2(pStream->m_stMP4Info.m_pMp4_fp, &(pStream->m_stMP4Info.m_stMP4ISampleIndexHead), 
			&(pStream->m_stMP4Info.m_stMP4ISampleIndexTail), &(pStream->m_stMP4Info.m_pIMP4SampleIndex), 
			&(pStream->m_stMP4Info.m_iIMP4SampleIndexmollocLen)) < 0)
		{
			PrintInfo("MP4_ReadSampleIndex_V2 failed!");
			td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
			goto leave;
		}
	}

	//¼ìË÷´ËÎÄ¼þÊÇ·ñÎª¼ÓÃÜÎÄ¼þ
	if(MP4_GetMP4FileEncryptStat((char *)_cFileName, &(pStream->m_stMP4Info.m_iEncryptFlag)) < 0)
	{
		PrintInfo("MP4_GetMP4FileEncryptStat failed!");
		td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
		goto leave;
	}

	iRetValue = td_success;
leave:
	return iRetValue;
}

int IsValidFileHeader(S_FileHeader* _header)
{
	if(!_header)
	{
		return -1;
	}
	PrintInfo("_header->Mode=%d", _header->Mode);
	if(!((_header->Mode == 21) || (_header->Mode == 23)))
	{
		return -1;
	}
	if(0/*(_header->FrameRate > MAX_FRAMERATE_NTSC)*/)
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


static int  decode_OpenFile(TStream *_This,IN td_s8* _cFileName)
{
	int iRet = 0;
	int iRetValue = 0;
	int iNameLen = 0;
	int iMP4FileType = 0;
	TStream *pStream = _This;	
	S_FileHeader stLastFileHeader = {0};
	
	if(pStream == NULL)
	{
		iRetValue = -1;
		goto leave;
	}
	if(_cFileName == NULL)
	{
		iRetValue = -2;
		goto leave;
	}
	iNameLen = strlen((char *)_cFileName);

	iMP4FileType = MP4_IsMP4File((char*)_cFileName);
	
	PrintInfo("iMP4FileType=%d", iMP4FileType);

	if(TYPE_TIANDY_MP4_FILE == iMP4FileType)//Ö»Ö§³Ö×Ô¼º×ª»»µÄMP4ÎÄ¼þ
	{
		pStream->m_iFileType = FILE_TYPE_MP4;
		PrintInfo("pStream->m_iFileType = FILE_TYPE_MP4");
	}
	else if(TYPE_OTHER_MP4_FILE == iMP4FileType)
	{
		td_printf(0xff0000,"%s %s %d MP4_OpenFile err\n", __FILE__, __FUNCTION__, __LINE__);
		iRetValue = -1;
		goto leave;
	}
	
	if(FILE_TYPE_MP4 == pStream->m_iFileType)
	{
		if(pStream->m_stMP4Info.m_pMp4_fp != NULL)
		{
			MP4_CloseFile(&(pStream->m_stMP4Info.m_pMp4_fp));
			pStream->m_stMP4Info.m_pMp4_fp = NULL;
		}
		
		if(TD_SUCCESS != MP4_OpenFile(&(pStream->m_stMP4Info.m_pMp4_fp),(char *)_cFileName))
		{
			td_printf(0xff0000,"%s %s %d MP4_OpenFile err\n", __FILE__, __FUNCTION__, __LINE__);
			iRetValue = -1;
			goto leave;
		}

		if(decode_GetMP4FileInfo(pStream, _cFileName) < 0)
		{
			td_printf(0xff0000, "%s %s %d error!", __FILE__, __FUNCTION__, __LINE__);
			iRetValue = -1;
			goto leave;
		}
	}
	

	stLastFileHeader = pStream->m_FileHeader;	

	if(FILE_TYPE_MP4 == pStream->m_iFileType)
	{
		if(1 == pStream->m_stMP4Info.m_u32Version)
		{
			iRet = MP4_ReadSdvFileHeader((char*)_cFileName, &(pStream->m_FileHeader));
		}
		else
		{
			iRet = MP4_ReadSdvFileHeader_V2(pStream->m_stMP4Info.m_pMp4_fp, &(pStream->m_FileHeader));
		}

		if(iRet < 0)
		{
			td_printf(0xff0000,"MP4_ReadSdvFileHeader err %d\n", iRet);			
			iRetValue = -4;
			goto leave;
		}
		td_printf(0,"MP4_ReadSdvFileHeader pStream->m_FileHeader.FrameRate = %d", pStream->m_FileHeader.FrameRate);
	}
		
	if(IsValidFileHeader(&pStream->m_FileHeader) < 0)
	{
		td_printf(0,"IsValidFileHeader err\n");
//		rec_CloseFile(&pStream->m_fp);//cwm_todo
		pStream->m_fp = NULL;
		
		iRetValue = -4;
		goto leave;
	}
	pStream->m_iFrameRate = pStream->m_FileHeader.FrameRate;
	pStream->m_FileLoction = 88;
	pStream->m_iOrigFrameLen = pStream->m_FileHeader.Height * pStream->m_FileHeader.Width * 1.5;
	//¸´ÖÆÎÄ¼þÃû
	memset(pStream->m_File,0,FILENAMEMAXNUM);
	memcpy(pStream->m_File,_cFileName,iNameLen);

	if(stLastFileHeader.AudioSample != pStream->m_FileHeader.AudioSample)
	{
		//TdAO_Stop(pStream->m_AO.m_iAoId);//cwm_todo
		pStream->m_AO.m_iAoId = -1;
	}

leave:
	if((iRetValue < 0)&&(FILE_TYPE_MP4 == pStream->m_iFileType))
	{
		if(pStream->m_stMP4Info.m_pMp4_fp != NULL)
		{
			MP4_CloseFile(&(pStream->m_stMP4Info.m_pMp4_fp));
			pStream->m_stMP4Info.m_pMp4_fp = NULL;
		}
	}
	return iRetValue;
}


int  IsValidFrame(S_FrameHeader* frHeader)
{
	if(!frHeader)
	{		
		td_printf(0, "%s 11 \n", __FUNCTION__ );
		return -1;
	}   	
	if(frHeader->u32FrameID != 0x02000000)
	{
		td_printf(0, "%s 14 \n", __FUNCTION__ );
		return -1;
	}
	if(frHeader->u32FrameSize>MAXVSTREAM)
	{
		td_printf(0, "%s 12 \n", __FUNCTION__ );
    	return -1;
	}
	if(frHeader->u32VStreamLen>MAXVSTREAM)
	{
		td_printf(0, "%s 13 \n", __FUNCTION__ );
    	return -1;
	}
	if(frHeader->u32FrameSize!=sizeof(S_FrameHeader)+frHeader->u32VStreamLen+frHeader->u16AFrames*frHeader->u16AFrameSize)
	{
		td_printf(0, "%s  frHeader->u32FrameNO:%d \n", __FUNCTION__ ,frHeader->u32FrameNO);	
		td_printf(0, "%s  frHeader->u32FrameSize:%d  frHeader->u32VStreamLen:%d \n", __FUNCTION__ ,frHeader->u32FrameSize,frHeader->u32VStreamLen);
		td_printf(0, "%s  frHeader->u16AFrames:%d  frHeader->u16AFrameSize:%d \n", __FUNCTION__ ,frHeader->u16AFrames,frHeader->u16AFrameSize);
		td_printf(0, "%s  frHeade:%d \n", __FUNCTION__ ,sizeof(S_FrameHeader));
		return -1;
	}
	return 1;
}


//·µ»Ø -2:´ú±í´íÎóÖ¡
//		  -1:´ú±í²åÈëÖ¡
//            0:´ú±í×îºóµÄË÷ÒýÖ¡
//		1:´ú±íÕý³£Ö¡
static td_s32 Is_InsertFrame(S_FrameHeader *_pTFrameheader)
{
	if(_pTFrameheader->u32FrameID == 0 && _pTFrameheader->u32FrameSize == 0)
	{
		return PLAYBACK_READFILE_ERR;
	}
	if(1 != IsValidFrame(_pTFrameheader))
	{
		PrintInfo("1 != IsValidFrame");
		return PLAYBACK_READFILE_ERR;
	}
	if(_pTFrameheader->reserved == REC_FALSE_FRAME_FLAG)
	{
		return PLAYBACK_PLUGFRAME;
	}
	if(_pTFrameheader->reserved != 0
	  || _pTFrameheader->u16AFrames != 0
	  || _pTFrameheader->u16AFrameSize != 0
	  || _pTFrameheader->u32TimeStamp != 0)
	{
		return 1;
	}
	return 0;
}

//_iBefAfter: 0:ÕÒµ±Ç°Ö¡ËùÔÚµÄIÖ¡
//>0:ÏòÇ°ÕÒnÖ¡
//<0:ÏòºóÕÒnÖ¡:Ä¿Ç°Ö»ÊÇÖ§³Ö1Ö¡
static int decode_IFmFromIndex(TStream *_This, int _iLocation, int _iBefAfter, int *_piSize)
{
	int i = 0;
	int iRet = PLAYBACK_FILEFINISH_NOEFFECT;
	char *ptr = NULL;
	TStream *pStream = _This;
	int iLoc = 0;
	int iBefAfter = _iBefAfter < 0 ? (0 - _iBefAfter) : _iBefAfter;
	if(0 == pStream->m_mollocLen)
	{
		return PLAYBACK_NOINDEX;
	}
	ptr = (char *)pStream->m_pIFrameIndex;	

	if(pStream->m_IFrameLoaction > pStream->m_IFrameLen)
	{
		pStream->m_IFrameLoaction = 0;
	}
	if(_iBefAfter < 0)//²éÕÒ½Ïµ±Ç°Î»ÖÃ¿¿ºóµÄIÖ¡
	{
		i = pStream->m_IFrameLoaction;
		if(*(int *)(ptr + pStream->m_IFrameLoaction * pStream->m_iIndexLen + sizeof(int)) <= _iLocation)
		{
			for(; i < pStream->m_IFrameLen; i++)
			{
				if(*(int *)(ptr + i * pStream->m_iIndexLen + sizeof(int)) > _iLocation)
				{
					pStream->m_IFrameLoaction = i;
					iRet = *(int *)(ptr + i * pStream->m_iIndexLen + sizeof(int));
					iLoc = i;
					goto leave;
				}
			}
		}
		else
		{
			int tmpLoc = *(int *)(ptr + i * pStream->m_iIndexLen + sizeof(int));
			for(; i >= 0; i--)
			{
				if(*(int *)(ptr + i * pStream->m_iIndexLen + sizeof(int)) <= _iLocation)
				{
					pStream->m_IFrameLoaction = i;
					iRet = tmpLoc;
					goto leave;
				}
				tmpLoc = *(int *)(ptr + i * pStream->m_iIndexLen + sizeof(int));
				iLoc = i;
			}
		}
	}
	else //²éÕÒ½Ïµ±Ç°Î»ÖÃ¿¿Ç°µÄIÖ¡
	{
		i = pStream->m_IFrameLoaction; 
		int iCnt = 0;
		int tmpLoc = *(int *)(ptr + i * pStream->m_iIndexLen + sizeof(int));
		if(0 == iBefAfter)
		{
			iBefAfter = 1;
			_iLocation++;
		}
		while(iBefAfter--)
		{
			if((iCnt++) != 0)
			{
				_iLocation = tmpLoc;
			}
 
			if(*(int *)(ptr + pStream->m_IFrameLoaction * pStream->m_iIndexLen + sizeof(int)) < _iLocation)
			{ 
				for(; i < pStream->m_IFrameLen; i++)
				{ 
					if(*(int *)(ptr + i * pStream->m_iIndexLen + sizeof(int)) > _iLocation)
					{ 
						pStream->m_IFrameLoaction = i;
						break;
					}
					tmpLoc = *(int *)(ptr + i * pStream->m_iIndexLen + sizeof(int));
					iLoc = i;
				}
				pStream->m_IFrameLoaction = ((i >= 1) ? (i - 1) : 0);
			}
			else
			{ 
				for(; i >= 0; i--)
				{
					if(*(int *)(ptr + i * pStream->m_iIndexLen + sizeof(int)) < _iLocation)
					{
						pStream->m_IFrameLoaction = i;
						tmpLoc = *(int *)(ptr + i * pStream->m_iIndexLen + sizeof(int));
						iLoc = i;
						break;
					}
				}
				if(i < 0)
				{
					iRet = PLAYBACK_FILEFINISH_NOEFFECT;
					goto leave;
				}
			}
		}
		iRet = tmpLoc;
		goto leave;
	}
leave:
	if((NULL != _piSize) && (iRet > 0) && (pStream->m_iIndexLen > sizeof(int) * 3))
	{
		*_piSize = *(int *)(ptr + iLoc * pStream->m_iIndexLen + 3 * sizeof(int));
	}
	return iRet;
}

static td_s32 RDIFrame(TStream * _This)
{
	int iRet;
	int iLoc = 0;
	int iRealLocation = 0;
	int iLocation = 0;
	int iFrameLen = sizeof(S_FrameHeader);
	TStream *pStream = _This;
	PrintInfo("RDIFrame");
	if(NULL == pStream)
	{
		iRet = PLAYBACK_READFILE_ERR;
		goto leave1;
	}

	if(FILE_TYPE_MP4 == pStream->m_iFileType)
	{
		if(pStream->m_stMP4Info.m_pMp4_fp == NULL)
		{
			td_printf(0, "%s start!----->error", __FUNCTION__);
			iRet = PLAYBACK_READFILE_ERR;
			goto leave1;
		}

		if(1 != pStream->m_stMP4Info.m_iEncryptFlag)
		{
			#if 1
			if(AV_CODEC_ID_H264 == pStream->m_stMP4Info.m_iVideoType)
			{
				if(MP4_GetOneTiandyFrame_V2(pStream->m_stMP4Info.m_pMp4_fp, 
											pStream->m_stMP4Info.m_u32VideoTrackId, 
											pStream->m_stMP4Info.m_u32AudioTrackId,
											&(pStream->m_stMP4Info.m_uiCurVideoSampleId), 
											&(pStream->m_stMP4Info.m_uiCurAudioSampleId), 
											pStream->m_stMP4Info.m_cSps, 
											pStream->m_stMP4Info.m_iSpsSize, 
											pStream->m_stMP4Info.m_cPps, 
											pStream->m_stMP4Info.m_iPpsSize,
											pStream->m_pBuf, 
											&(pStream->m_FrameHeader), 
											pStream->m_stMP4Info.m_iAudioType, 
											&(pStream->m_stMP4Info.m_u32UseLessIdCount), 1) < 0)
				{
					td_printf(0xff0000, "%s %s %d MP4_GetOneTiandyFrame_V2 err!", __FILE__, __FUNCTION__, __LINE__);
					iRet = PLAYBACK_READFILE_ERR; 
					goto leave1;
				}
			}else if(AV_CODEC_ID_HEVC == pStream->m_stMP4Info.m_iVideoType)
			{
				if(MP4_GetOneTiandyFrameH265_V2(pStream->m_stMP4Info.m_pMp4_fp, 
											pStream->m_stMP4Info.m_u32VideoTrackId, 
											pStream->m_stMP4Info.m_u32AudioTrackId,
											&(pStream->m_stMP4Info.m_uiCurVideoSampleId), 
											&(pStream->m_stMP4Info.m_uiCurAudioSampleId), 
											pStream->m_stMP4Info.m_cVps, 
											pStream->m_stMP4Info.m_iVpsSize, 
											pStream->m_stMP4Info.m_cSps, 
											pStream->m_stMP4Info.m_iSpsSize, 
											pStream->m_stMP4Info.m_cPps, 
											pStream->m_stMP4Info.m_iPpsSize,
											pStream->m_pBuf, 
											&(pStream->m_FrameHeader), 
											pStream->m_stMP4Info.m_iAudioType, 
											&(pStream->m_stMP4Info.m_u32UseLessIdCount), 1) < 0)
				{
					td_printf(0xff0000, "%s %s %d MP4_GetOneTiandyFrameH265_V2 err!", __FILE__, __FUNCTION__, __LINE__);
					iRet = PLAYBACK_READFILE_ERR; 
					goto leave1;
				}

			}else
			{
				//todo
			}
			#else
			if(AV_CODEC_ID_H264 == pStream->m_stMP4Info.m_iVideoType)
			{
				if(MP4_GetOneITiandyFrame(pStream->m_stMP4Info.m_pMp4_fp,
										pStream->m_stMP4Info.m_u32VideoTrackId,
										pStream->m_stMP4Info.m_u32AudioTrackId,
										&(pStream->m_stMP4Info.m_uiCurVideoSampleId),
										&(pStream->m_stMP4Info.m_uiCurAudioSampleId),
										pStream->m_stMP4Info.m_cSps,
										pStream->m_stMP4Info.m_iSpsSize,
										pStream->m_stMP4Info.m_cPps,
										pStream->m_stMP4Info.m_iPpsSize,
										pStream->m_pBuf,
										&(pStream->m_FrameHeader),
										pStream->m_stMP4Info.m_iAudioType) < 0)
				{
					td_printf(0xff0000, "%s %s %d MP4_GetOneITiandyFrame err!", __FILE__, __FUNCTION__, __LINE__);
					iRet = PLAYBACK_READFILE_ERR; 
					goto leave1;
				}
			}else if(AV_CODEC_ID_HEVC == pStream->m_stMP4Info.m_iVideoType)
			{
				if(MP4_GetOneITiandyFrameH265(pStream->m_stMP4Info.m_pMp4_fp,
										pStream->m_stMP4Info.m_u32VideoTrackId,
										pStream->m_stMP4Info.m_u32AudioTrackId,
										&(pStream->m_stMP4Info.m_uiCurVideoSampleId),
										&(pStream->m_stMP4Info.m_uiCurAudioSampleId),
										pStream->m_stMP4Info.m_cVps,
										pStream->m_stMP4Info.m_iVpsSize,
										pStream->m_stMP4Info.m_cSps,
										pStream->m_stMP4Info.m_iSpsSize,
										pStream->m_stMP4Info.m_cPps,
										pStream->m_stMP4Info.m_iPpsSize,
										pStream->m_pBuf,
										&(pStream->m_FrameHeader),
										pStream->m_stMP4Info.m_iAudioType) < 0)
				{
					td_printf(0xff0000, "%s %s %d MP4_GetOneITiandyFrameH265 err!", __FILE__, __FUNCTION__, __LINE__);
					iRet = PLAYBACK_READFILE_ERR; 
					goto leave1;
				}
			}else
			{
				//todo
			}
			#endif
		}
		else
		{
			if(AV_CODEC_ID_H264 == pStream->m_stMP4Info.m_iVideoType)
			{
				if(MP4_GetOneEncryptTiandyFrame(pStream->m_stMP4Info.m_pMp4_fp, 
												pStream->m_stMP4Info.m_u32VideoTrackId,
												pStream->m_stMP4Info.m_u32AudioTrackId, 
												&(pStream->m_stMP4Info.m_uiCurVideoSampleId), 
												&(pStream->m_stMP4Info.m_uiCurAudioSampleId),
												pStream->m_pBuf, 
												&(pStream->m_FrameHeader), 
												pStream->m_stMP4Info.m_iAudioType, 1) < 0)
				{
					td_printf(0xff0000, "%s %s %d MP4_GetOneEncryptTiandyFrame err!", __FILE__, __FUNCTION__, __LINE__);
					iRet = PLAYBACK_READFILE_ERR; 
					goto leave1;
				}
			}else if(AV_CODEC_ID_HEVC == pStream->m_stMP4Info.m_iVideoType)
			{
				if(MP4_GetOneEncryptTiandyFrameH265(pStream->m_stMP4Info.m_pMp4_fp, 
												pStream->m_stMP4Info.m_u32VideoTrackId,
												pStream->m_stMP4Info.m_u32AudioTrackId, 
												&(pStream->m_stMP4Info.m_uiCurVideoSampleId), 
												&(pStream->m_stMP4Info.m_uiCurAudioSampleId),
												pStream->m_pBuf, 
												&(pStream->m_FrameHeader), 
												pStream->m_stMP4Info.m_iAudioType, 1) < 0)
				{
					td_printf(0xff0000, "%s %s %d MP4_GetOneEncryptTiandyFrameH265 err!", __FILE__, __FUNCTION__, __LINE__);
					iRet = PLAYBACK_READFILE_ERR; 
					goto leave1;
				}
			}else
			{
				//todo
			}
		}

		iRet = PLAYBACK_IFRAME;
		goto leave1;
	}
	else
	{
	if(pStream->m_iOnlyIFrame == 1)
	{		

		iLoc = pStream->m_FileLoction;

		pStream->m_iFrameLoc = pStream->m_FileLoction;
		
		iRealLocation = ftell(pStream->m_fp);
		if((iLocation = decode_IFmFromIndex(_This, iLoc, -1, NULL)) < 0)
		{
			iRet = iLocation;
				goto leave2;
		}
		fseek(pStream->m_fp, iLocation, SEEK_SET);
		if(fread(&pStream->m_FrameHeader,1,iFrameLen,pStream->m_fp) < iFrameLen)
		{
			iRet = PLAYBACK_READFILE_ERR;
			td_printf(0,"read m_FrameHeader err");
				goto leave2;
		}
		
		iRet = Is_InsertFrame(&(pStream->m_FrameHeader));
		if(iRet < 0)
		{	
				goto leave2;//²åÈëÖ¡
		}
		
		if(fread(pStream->m_pBuf,1,pStream->m_FrameHeader.u32FrameSize - iFrameLen,pStream->m_fp) < pStream->m_FrameHeader.u32FrameSize - iFrameLen)
		{
			iRet = PLAYBACK_READFILE_ERR;
			td_printf(0,"read data err,len is err");
				goto leave2;
		}
		pStream->m_iIFrameFileLoc = iLocation;
		pStream->m_FileLoction = pStream->m_FrameHeader.u32FrameSize + iLocation;
		return PLAYBACK_IFRAME;
	}
	}
	
leave1:
	return iRet;
leave2:
	fseek(pStream->m_fp, iRealLocation, SEEK_SET);
	td_printf(0,"index_frame is err");
	return iRet;
}

static td_s32 RDFrame(TStream *_This)// -2:¶ÁÎÄ¼þ´íÎó
{
	int iRet = 0;
	int iFrameLen = sizeof(S_FrameHeader);
	TStream *pStream = _This;

	PrintInfo("RDFrame");
	if(FILE_TYPE_MP4 == pStream->m_iFileType)
	{	
		if(pStream == NULL || pStream->m_stMP4Info.m_pMp4_fp == NULL)
		{
			td_printf(0, "%s start!----->error", __FUNCTION__);
			iRet = PLAYBACK_READFILE_ERR;
			goto leave;
		}

		if(1 != pStream->m_stMP4Info.m_iEncryptFlag)
		{
			if(AV_CODEC_ID_H264 == pStream->m_stMP4Info.m_iVideoType)
			{
				if(MP4_GetOneTiandyFrame_V2(pStream->m_stMP4Info.m_pMp4_fp, 
											pStream->m_stMP4Info.m_u32VideoTrackId, 
											pStream->m_stMP4Info.m_u32AudioTrackId,
											&(pStream->m_stMP4Info.m_uiCurVideoSampleId), 
											&(pStream->m_stMP4Info.m_uiCurAudioSampleId), 
											pStream->m_stMP4Info.m_cSps, 
											pStream->m_stMP4Info.m_iSpsSize, 
											pStream->m_stMP4Info.m_cPps, 
											pStream->m_stMP4Info.m_iPpsSize,
											pStream->m_pBuf, 
											&(pStream->m_FrameHeader), 
											pStream->m_stMP4Info.m_iAudioType, 
											&(pStream->m_stMP4Info.m_u32UseLessIdCount), 0) < 0)
				{
					td_printf(0xff0000, "%s %s %d MP4_GetOneTiandyFrame_V2 err!", __FILE__, __FUNCTION__, __LINE__);
					iRet = PLAYBACK_READFILE_ERR; 
					goto leave;
				}
			}else if(AV_CODEC_ID_HEVC == pStream->m_stMP4Info.m_iVideoType)
			{
				if(MP4_GetOneTiandyFrameH265_V2(pStream->m_stMP4Info.m_pMp4_fp, 
											pStream->m_stMP4Info.m_u32VideoTrackId, 
											pStream->m_stMP4Info.m_u32AudioTrackId,
											&(pStream->m_stMP4Info.m_uiCurVideoSampleId), 
											&(pStream->m_stMP4Info.m_uiCurAudioSampleId), 
											pStream->m_stMP4Info.m_cVps, 
											pStream->m_stMP4Info.m_iVpsSize, 
											pStream->m_stMP4Info.m_cSps, 
											pStream->m_stMP4Info.m_iSpsSize, 
											pStream->m_stMP4Info.m_cPps, 
											pStream->m_stMP4Info.m_iPpsSize,
											pStream->m_pBuf, 
											&(pStream->m_FrameHeader), 
											pStream->m_stMP4Info.m_iAudioType, 
											&(pStream->m_stMP4Info.m_u32UseLessIdCount), 0) < 0)
				{
					td_printf(0xff0000, "%s %s %d MP4_GetOneTiandyFrameH265_V2 err!", __FILE__, __FUNCTION__, __LINE__);
					iRet = PLAYBACK_READFILE_ERR; 
					goto leave;
				}

			}else
			{
				//todo
			}
		}
		else
		{
			if(AV_CODEC_ID_H264 == pStream->m_stMP4Info.m_iVideoType)
			{
				if(MP4_GetOneEncryptTiandyFrame(pStream->m_stMP4Info.m_pMp4_fp, 
												pStream->m_stMP4Info.m_u32VideoTrackId,
												pStream->m_stMP4Info.m_u32AudioTrackId, 
												&(pStream->m_stMP4Info.m_uiCurVideoSampleId), 
												&(pStream->m_stMP4Info.m_uiCurAudioSampleId),
												pStream->m_pBuf, 
												&(pStream->m_FrameHeader), 
												pStream->m_stMP4Info.m_iAudioType, 0) < 0)
				{
					td_printf(0xff0000, "%s %s %d MP4_GetOneEncryptTiandyFrame err!", __FILE__, __FUNCTION__, __LINE__);
					iRet = PLAYBACK_READFILE_ERR; 
					goto leave;
				}
			}else if(AV_CODEC_ID_HEVC == pStream->m_stMP4Info.m_iVideoType)
			{
				if(MP4_GetOneEncryptTiandyFrameH265(pStream->m_stMP4Info.m_pMp4_fp, 
												pStream->m_stMP4Info.m_u32VideoTrackId,
												pStream->m_stMP4Info.m_u32AudioTrackId, 
												&(pStream->m_stMP4Info.m_uiCurVideoSampleId), 
												&(pStream->m_stMP4Info.m_uiCurAudioSampleId),
												pStream->m_pBuf, 
												&(pStream->m_FrameHeader), 
												pStream->m_stMP4Info.m_iAudioType, 0) < 0)
				{
					td_printf(0xff0000, "%s %s %d MP4_GetOneEncryptTiandyFrameH265 err!", __FILE__, __FUNCTION__, __LINE__);
					iRet = PLAYBACK_READFILE_ERR; 
					goto leave;
				}
			}else
			{
				//todo
			}
		}
	}
	else
	{
		if(pStream == NULL || pStream->m_fp == NULL)
		{
			iRet = PLAYBACK_READFILE_ERR;
			goto leave;
		}

		pStream->m_iFrameLoc = pStream->m_FileLoction;
		fseek(pStream->m_fp, pStream->m_FileLoction, SEEK_SET);

		if(fread(&pStream->m_FrameHeader,1,iFrameLen,pStream->m_fp) < iFrameLen)
		{
			//decode_CloseFile(_This);
			iRet = PLAYBACK_READFILE_ERR;
			goto leave;
		}

		//¸Ä±äÎÄ¼þÖ¸Õë
		pStream->m_FileLoction += pStream->m_FrameHeader.u32FrameSize;
		if((iRet  = Is_InsertFrame(&(pStream->m_FrameHeader))) < 0)
		{
			goto leave;
		}
		else if(0 == iRet)
		{
			iRet = PLAYBACK_FILEFINISH_NOEFFECT;
			goto leave;
		}
		
		//¶ÁÊý¾Ý
		int temp = pStream->m_FrameHeader.u32FrameSize - iFrameLen;
		if(pStream->m_iOnlyIFrame != 1 || (pStream->m_iOnlyIFrame == 1 && pStream->m_FrameHeader.u8FrameType == 0))
		{
			if(fread(pStream->m_pBuf,1,temp,pStream->m_fp) < temp)
			{
				iRet = PLAYBACK_READFILE_ERR;
				//decode_CloseFile(_This);
				goto leave;
			}
		}
		else
		{
			fseek(pStream->m_fp, pStream->m_FileLoction, SEEK_SET);
		}

		//ÅÐ¶ÏÎÄ¼þÊÇ·ñ½áÊø
		if(pStream->m_FileLoction >= pStream->m_iFileLen)
		{
			td_printf(0,"%s , %d",__func__,__LINE__);
			iRet = PLAYBACK_FILEFINISH_NOEFFECT;
			goto leave;
		}
		if(pStream->m_FrameHeader.u8FrameType == 0)//IÖ¡
		{
			iRet = PLAYBACK_IFRAME;
			pStream->m_iIFrameFileLoc = pStream->m_iFrameLoc;
			goto leave;
		}
		iRet = td_success;
	}
	
leave:
	return iRet;
}

td_s32 decode_readfile(TStream *_This)
{
	int iRet = PLAYBACK_NOINDEX;
	TStream *pStream = _This;

	do
	{
		if(pStream->m_iOnlyIFrame == 1)
		{
			iRet = RDIFrame(_This);
		}
		if(PLAYBACK_NOINDEX == iRet || PLAYBACK_PLUGFRAME == iRet)
		{
			iRet = RDFrame(_This);
		}
	}while(PLAYBACK_PLUGFRAME == iRet);
	
	return iRet;
}

static td_s32 decode_ReadFrame(TStream *_This)
{
	int iRet = 0;
	TStream *pStream = _This;
	if(pStream->m_iBackFlag)
	{
		PrintInfo("decode_ReadMem");
		//iRet = decode_ReadMem(_This);
	}
	else
	{
		PrintInfo("decode_readfile");
		iRet = decode_readfile(_This);
	}
	return iRet;
}

void DoRead(int fd, TStream *_This)
{
	int iRet = -1;
	int readCount = 0;
	
	while(1)
	{
		iRet = decode_ReadFrame(_This);
		PrintInfo("iRet=%d", iRet);
		if(iRet < 0)
		{
			break;
		}
		readCount ++;
		
		iRet = write(fd, &_This->m_FrameHeader, sizeof(S_FrameHeader));
		if(iRet != sizeof(S_FrameHeader))
		{
			PrintInfo("write failed!");
		}		
		
		iRet = write(fd, _This->m_pBuf, _This->m_FrameHeader.u32VStreamLen + _This->m_FrameHeader.u16AFrameSize*_This->m_FrameHeader.u16AFrames);
		PrintInfo("_This->m_FrameHeader.u32VStreamLen=%d", _This->m_FrameHeader.u32VStreamLen);
		PrintInfo("_This->m_FrameHeader.u16AFrameSize=%d", _This->m_FrameHeader.u16AFrameSize);
		PrintInfo("_This->m_FrameHeader.u16AFrames=%d", _This->m_FrameHeader.u16AFrames);
		if(iRet != _This->m_FrameHeader.u32VStreamLen + _This->m_FrameHeader.u16AFrameSize*_This->m_FrameHeader.u16AFrames)
		{
			PrintInfo("write failed!");
		}		
	}

	PrintInfo("readCount=%d", readCount);
}

int show_fileHeadInfo(S_FileHeader *pfileHeader)
{
		PrintInfo("stSDVFileHeadBuf.FrameRate=%d", pfileHeader->FrameRate);
		PrintInfo("stSDVFileHeadBuf.Width=%d", pfileHeader->Width);
		PrintInfo("stSDVFileHeadBuf.Height=%d", pfileHeader->Height);
		PrintInfo("stSDVFileHeadBuf.Mode=%d", pfileHeader->Mode);
		PrintInfo("stSDVFileHeadBuf.bAudio=%d", pfileHeader->bAudio);
		PrintInfo("stSDVFileHeadBuf.Year=%d", pfileHeader->Year);
		PrintInfo("stSDVFileHeadBuf.Month=%d", pfileHeader->Month);
		PrintInfo("stSDVFileHeadBuf.Day=%d", pfileHeader->Day);
		PrintInfo("stSDVFileHeadBuf.Hour=%d", pfileHeader->Hour);
		PrintInfo("stSDVFileHeadBuf.Minute=%d", pfileHeader->Minute);
		PrintInfo("stSDVFileHeadBuf.Second=%d", pfileHeader->Second);
		PrintInfo("stSDVFileHeadBuf.CoverMask=%d", pfileHeader->CoverMask);
		PrintInfo("stSDVFileHeadBuf.cCovWord=%s", pfileHeader->cCovWord);
		PrintInfo("stSDVFileHeadBuf.m_uiFrameNoDiff=%d", pfileHeader->m_uiFrameNoDiff);
		PrintInfo("stSDVFileHeadBuf.TriggerHigh=%d", pfileHeader->TriggerHigh);
		PrintInfo("stSDVFileHeadBuf.TriggerLow=%d", pfileHeader->TriggerLow);
		PrintInfo("stSDVFileHeadBuf.reserved=%d", pfileHeader->reserved);
		PrintInfo("stSDVFileHeadBuf.AChannels=%d", pfileHeader->AChannels);
		PrintInfo("stSDVFileHeadBuf.BitsPerSample=%d", pfileHeader->BitsPerSample);
		PrintInfo("stSDVFileHeadBuf.AudioSample=%d", pfileHeader->AudioSample);
		PrintInfo("stSDVFileHeadBuf.TotalSize=%d", pfileHeader->TotalSize);
		PrintInfo("stSDVFileHeadBuf.FrameCount=%d", pfileHeader->FrameCount);

		return 0;
}

int show_framHeadInfo(S_FrameHeader *pFrameHeader)
{	
	PrintInfo("pFrameHeader.u32FrameID=0x%x", pFrameHeader->u32FrameID);
	PrintInfo("pFrameHeader.u32FrameSize=%d", pFrameHeader->u32FrameSize);
	PrintInfo("pFrameHeader.u8FrameType=%d", pFrameHeader->u8FrameType);
	PrintInfo("pFrameHeader.u32FrameNO=%d", pFrameHeader->u32FrameNO);
	PrintInfo("pFrameHeader.u32TimeStamp=%d", pFrameHeader->u32TimeStamp);
	PrintInfo("pFrameHeader.u32VStreamLen=%d", pFrameHeader->u32VStreamLen);
	PrintInfo("pFrameHeader.u16AFrames=%d", pFrameHeader->u16AFrames);
	PrintInfo("pFrameHeader.u16AFrameSize=%d", pFrameHeader->u16AFrameSize);
	PrintInfo("pFrameHeader.u8ACoder=%d", pFrameHeader->u8ACoder);
	PrintInfo("pFrameHeader.reserved=%d", pFrameHeader->reserved);

	return 0;
}


static int demoNewInterface(void)
{	
	char *name = "H31C0M20151219140904C35.mp4";
	int iRet = td_success;
	void* pMp4Hdlr = NULL;
	
	char _pFrameBuf[1024*1024];
	td_u32 _bufLen = sizeof(_pFrameBuf);	
	int len = 0;
	S_FrameHeader *_pstFrmaeHeader = (S_FrameHeader *)_pFrameBuf;
	
	int fdTmp = open("tmpFile.sdv", O_RDWR|O_APPEND|O_CREAT, 0644);
	pMp4Hdlr = MP4_Open(name);

	TMP4FileHead _pstMP4FileHead;
	iRet = MP4_GetFileHead(pMp4Hdlr, &_pstMP4FileHead);
	if(iRet < 0)
	{
		PrintInfo("MP4_GetFileHead failed!");
		iRet= td_failure;
		goto leave;
	}
	
	iRet = write(fdTmp, &_pstMP4FileHead.m_stTMP4ISampleIndexHead.m_stExRecInfo.m_stSdvFileHead, sizeof(S_FileHeader));
	if(iRet != sizeof(S_FileHeader))
	{
		PrintInfo("write failed!");
		iRet= td_failure;
		goto leave;
	}
	
	
	while(1)
	{
		iRet = MP4_Read(pMp4Hdlr, _pFrameBuf, _bufLen);
		if(iRet < 0)
		{
			PrintInfo("MP4_Read failed!");
			iRet= td_failure;
			break;
		}
		len = sizeof(S_FrameHeader) + _pstFrmaeHeader->u32VStreamLen + _pstFrmaeHeader->u16AFrameSize*_pstFrmaeHeader->u16AFrames;
		
		iRet = write(fdTmp, _pFrameBuf, len);
		if(iRet != len)
		{
			PrintInfo("write failed!");
			iRet= td_failure;
			goto leave;
		}
	}

leave:

	if(fdTmp > 0)
	{
		close(fdTmp);
	}
	if(MP4_Close(pMp4Hdlr) < 0)
	{
		PrintInfo("MP4_Close failed!");
	}else
	{
		PrintInfo("MP4_Close successful!");
	}
	
	return iRet;
}


int playMp4(int argc, char ** argv)
{
	void* pMp4Hdlr = NULL;
	char *name = NULL;
	int iRet = -1;
	if(argc < 3)
	{
		//name = "tiandy.mp4";
		//name = "h264.mp4";
		//name = "ch004_20151217115424t.mp4";
		name = "H31C0M20151219140904C35h264.mp4";//H264 Baseline@3.1£¬Èý·½¡¢¹«Ë¾²¥·ÅÆ÷¶¼ÄÜ²¥·Å
		//name = "H31C5M20151223134538C24.mp4";//H265 Unknown Profile 1@unknown 
		//name = "ch07_20151223101732m9747.mp4";//H264 High@2£¬¹«Ë¾²¥·ÅÆ÷ÎÞ·¨²¥·Å¡¢Èý·½¿ÉÒÔ²¥·Å
	}else
	{
		name = *(argv+1);
	}

	//demoNewInterface();
	
	int fdTmp = open("tmpFile.sdv", O_RDWR|O_APPEND|O_CREAT, 0644);
	pMp4Hdlr = MP4_Open(name);
	if(NULL == pMp4Hdlr)
	{
		PrintInfo("MP4_Open failed!");
		return -1;
	}
	TMP4Dec *pTMP4Dec = (TMP4Dec *)pMp4Hdlr;
	return 0;
	TMP4FileHead _pstMP4FileHead;
	iRet = MP4_GetFileHead(pMp4Hdlr, &_pstMP4FileHead);
	if(iRet < 0)
	{
		PrintInfo("MP4_GetFileHead failed!");
	}
	
	//show_fileHeadInfo(&_pstMP4FileHead.m_stTMP4ISampleIndexHead.m_stExRecInfo.m_stSdvFileHead);
	iRet = write(fdTmp, &_pstMP4FileHead.m_stTMP4ISampleIndexHead.m_stExRecInfo.m_stSdvFileHead, sizeof(S_FileHeader));
	if(iRet != sizeof(S_FileHeader))
	{
		PrintInfo("write failed!");
	}
	char _pFrameBuf[1024*1024];
	td_u32 _bufLen = sizeof(_pFrameBuf);	
	
	int len = 0;
	int count = 0;
	S_FrameHeader *_pstFrmaeHeader = (S_FrameHeader *)_pFrameBuf;
/*
    MP4FileHandle hFile,
    MP4TrackId    trackId,
    MP4Timestamp  when,
    bool          wantSyncSample DEFAULT(false) );
*/
	MP4SampleId numberOfSamples = MP4GetTrackNumberOfSamples(pTMP4Dec->m_pMP4Handle, pTMP4Dec->m_stMP4DecVideo.m_uiVTrackId);
	MP4SampleId sampleid = MP4GetSampleIdFromTime(pTMP4Dec->m_pMP4Handle, pTMP4Dec->m_stMP4DecVideo.m_uiVTrackId, 0, true);
	MP4SampleId sampleidPrev = 0;
	PrintInfo("sampleid=%d", sampleid);
	int id;
	MP4Duration sumDuration = 0;
	int IFrameCount = 0;
	MP4SampleId stss[20];
	numberOfSamples = 0;
	for(id = 1; id <= numberOfSamples; id++)
	{

		if(MP4GetSampleSync(pTMP4Dec->m_pMP4Handle, pTMP4Dec->m_stMP4DecVideo.m_uiVTrackId, id) == 1)
		{
			PrintInfo("id=%d", id);
			stss[IFrameCount] = id;
			IFrameCount ++;
		}
		#if 0
		if(MP4GetSampleSync(pTMP4Dec->m_pMP4Handle, pTMP4Dec->m_stMP4DecVideo.m_uiVTrackId, id) == 1)
		{
			PrintInfo("================================id=%d", id);// 69 136 204
		}
	
		MP4Duration duration = MP4GetSampleDuration(pTMP4Dec->m_pMP4Handle, pTMP4Dec->m_stMP4DecVideo.m_uiVTrackId, id);
		PrintInfo("sampleid[%d]:duration=%lld", id, duration);
		sumDuration += duration;
		
		PrintInfo("sumDuration=%lld", sumDuration);
		sampleid = MP4GetSampleIdFromTime(pTMP4Dec->m_pMP4Handle, pTMP4Dec->m_stMP4DecVideo.m_uiVTrackId, sumDuration, true);
		MP4Timestamp timestamp = MP4GetSampleTime(pTMP4Dec->m_pMP4Handle, pTMP4Dec->m_stMP4DecVideo.m_uiVTrackId, id);
		PrintInfo("timestamp=%lld", timestamp);
			PrintInfo("sampleid=%d", sampleid);// 69 136 204
		if(sampleid != sampleidPrev)
		{
			PrintInfo("sampleid=%d", sampleid);// 69 136 204
			sampleidPrev = sampleid;
			IFrameCount ++;
		}
		#endif
		
		PrintInfo("====================================");
		
	}
	
	PrintInfo("numberOfSamples=%d", numberOfSamples);
	PrintInfo("IFrameCount=%d", IFrameCount);
	for(id = 0; id < IFrameCount; id++)
	{
		PrintInfo("stss[%d]=%d", id, stss[id]);
	}

	for(id = 1; id < 3; id++)
	{
		uint32_t sampleSize = MP4GetSampleSize(pTMP4Dec->m_pMP4Handle, pTMP4Dec->m_stMP4DecVideo.m_uiVTrackId, id);
		PrintInfo("sampleSize=%d", sampleSize);
	}
	MP4Duration duration;
	MP4GetTrackDurationPerChunk(pTMP4Dec->m_pMP4Handle, pTMP4Dec->m_stMP4DecVideo.m_uiVTrackId, &duration);
	PrintInfo("duration=%lld", duration);
	return 0;
	
	while(1)
	{
		iRet = MP4_Read(pMp4Hdlr, _pFrameBuf, _bufLen);
		if(iRet < 0)
		{
			PrintInfo("MP4_Read failed!");
			break;
		}
		//PrintInfo("sizeof(S_FrameHeader)=%d", sizeof(S_FrameHeader));
		len = sizeof(S_FrameHeader) + _pstFrmaeHeader->u32VStreamLen + _pstFrmaeHeader->u16AFrameSize*_pstFrmaeHeader->u16AFrames;
		
		iRet = write(fdTmp, _pFrameBuf, len);
		if(iRet != len)
		{
			PrintInfo("write failed!");
		}
		count++;
		//show_framHeadInfo(_pstFrmaeHeader);
	}

	PrintInfo("count=%d", count);
	
	close(fdTmp);
	if(MP4_Close(pMp4Hdlr) < 0)
	{
		PrintInfo("MP4_Close failed!");
	}else
	{
		PrintInfo("MP4_Close successful!");
	}
	PrintInfo("====================================================================");
	return 0;
#if 0
	Decode stDecode;
	iRet = decode_Init(&stDecode, 0);
	if(iRet < 0)
	{
		PrintInfo("decode_Init failed!");
	}
	int fdDec = open("mp4v2_codec.sdv", O_RDWR|O_APPEND|O_CREAT, 0644);
	iRet = decode_OpenFile(&stDecode.m_Stream, name);
	if(iRet < 0)
	{
		PrintInfo("decode_OpenFile failed!");
	}
	iRet = write(fdDec, &stDecode.m_Stream.m_FileHeader, sizeof(S_FileHeader));
	if(iRet != sizeof(S_FileHeader))
	{
		PrintInfo("write failed!");
	}		
	
	stDecode.m_Stream.m_pBuf = malloc(1024*1024);
	if(NULL == stDecode.m_Stream.m_pBuf)
	{
		PrintInfo("malloc failed!");
	}
	DoRead(fdDec, &stDecode.m_Stream);
	close(fdDec);
	return 0;
#endif

	char* pcBufInfo = NULL;
	int iCount = 0;//´Ó¿ªÍ¥µ½ÐÝÍ¥µÄÊ±¼äµÄÊýÁ¿¼´TSdvFileTimeµÄÊýÁ¿
	int iBufLen = 0;//»ñµÃµÄbufµÄ´óÐ¡£¬ÓÃÓÚÐ£Ñé
	MP4_GetSdvConvertInfo(name, &iCount, &pcBufInfo, &iBufLen);
	if(MP4_OpenFile(&pMp4Hdlr, name) < 0)
	{
		PrintInfo("MP4_OpenFile failed!");
	}
	char *fileInfo = NULL;
	fileInfo = MP4FileInfo(name, 1);
	PrintInfo("strlen(fileInfo)=%d", strlen(fileInfo));
	PrintInfo("fileInfo=%s", fileInfo);
	//return 0;
	S_FileHeader m_stHeader;
	MP4_ReadSdvFileHeader_V2(pMp4Hdlr, &m_stHeader);

		
	TMP4ISampleIndexHead _pstMP4ISampleIndexHead;
	TMP4ISampleIndexTail _pstMP4ISampleIndexTail; 
	void * _pIMP4SampleIndex = NULL;
	int _piIMP4SampleIndexmollocLen = 0;
	memset(&_pstMP4ISampleIndexHead, 0, sizeof(_pstMP4ISampleIndexHead));
	memset(&_pstMP4ISampleIndexTail, 0, sizeof(_pstMP4ISampleIndexTail));
	MP4_ReadSampleIndex_V2(pMp4Hdlr, &_pstMP4ISampleIndexHead, &_pstMP4ISampleIndexTail, &_pIMP4SampleIndex, &_piIMP4SampleIndexmollocLen);

	
	unsigned int uTrackId = MP4_INVALID_TRACK_ID;
	
	if(MP4_GetTrackId_V2(pMp4Hdlr, &uTrackId, MP4_VIDEO_TRACK_TYPE) < 0)
	{
		PrintInfo("MP4_GetTrackId_V2 failed");
	}else
	{
		PrintInfo("MP4_GetTrackId_V2 successful uTrackId=%d", uTrackId);
	}

#define PS_SIZE		(1024*1024)
	
	int vpsSize = PS_SIZE;
	char* _pcVps = (char *)malloc(vpsSize);
	if(NULL == _pcVps)
	{
		PrintInfo("_pcVps == NULL, malloc failed");
		goto vps_error;
	}
	memset(_pcVps, 0, vpsSize);
	
	int spsSize = PS_SIZE;
	char* _pcSps = (char *)malloc(spsSize);
	if(NULL == _pcSps)
	{
		PrintInfo("_pcSps == NULL, malloc failed");
		goto sps_error;
	}
	memset(_pcSps, 0, spsSize);
	
	int ppsSize = PS_SIZE;
	char* _pcPps = (char *)malloc(ppsSize);
	if(NULL == _pcPps)
	{
		PrintInfo("_pcPps == NULL, malloc failed");
		goto pps_error;
	}
	memset(_pcPps, 0, ppsSize);
	
	MP4_GetSpsPps_V2(pMp4Hdlr,_pcSps, &spsSize, _pcPps, &ppsSize);//h264
	//MP4_GetVpsSpsPps_V2(pMp4Hdlr, _pcVps, &vpsSize, _pcSps, &spsSize, _pcPps, &ppsSize);//h265
	//PrintInfo("vpsSize=%d", vpsSize);
	PrintInfo("spsSize=%d", spsSize);
	PrintInfo("ppsSize=%d", ppsSize);
	//return 0;
	uint64_t video_timestamp = 0;
	MP4SampleId numSamples_vedio = MP4GetTrackNumberOfSamples(pMp4Hdlr, uTrackId);
	uint32_t sample_size_vedio = MAX_VIDEO_SAMPLE_SIZE;
	PrintInfo("numSamples_vedio=%d", numSamples_vedio); 
	uint8_t *videoframe = NULL;
	int curVideoSampleId = 0;
	
	videoframe = (uint8_t*)malloc(MAX_VIDEO_SAMPLE_SIZE + MAX_ExInfo_SIZE);
	if (NULL == videoframe)
	{
		PrintInfo("malloc failed"); 
	}
	uint32_t videoframesize = 0;
	MP4Duration sampleDuration_vedio = 0;
	bool isSyncSample_vedio = false;

	unsigned int iSpsPpsSize /*= sizeof(nalu_header) + _iSpsSize + sizeof(nalu_header) + _iPpsSize*/;
	unsigned int u32VideoPos = 0;
	
	memset(videoframe, 0, MAX_VIDEO_SAMPLE_SIZE + MAX_ExInfo_SIZE);

	uint8_t nalu_start[4] = {0x00, 0x00, 0x00, 0x01};
	int fd = open("mp4v2_codec.sdv", O_RDWR|O_APPEND|O_CREAT, 0644);
	ssize_t ret;

	int i = 0;
	unsigned int _u32VideoTrackId = uTrackId;
	unsigned int _u32AudioTrackId = 2;
	unsigned int _puiCurVideoSampleId = 1;
	unsigned int _puiCurAudioSampleId = 1;
	void* _pTiandyFrameH265Buf = malloc(1024*1024);;
	S_FrameHeader stFrmaeHeader;
	int _iAudioType = MP4_AUDIO_TRACK_TYPE;
	unsigned int _puiUseLessIdCount = 0;
	int _iNeedIFrame = 0;
	
	ret = write(fd, &m_stHeader, sizeof(S_FileHeader));
	if(ret != sizeof(S_FileHeader))
	{
		PrintInfo("write failed!");
	}		

	MP4_GetAudioType(name, &_iAudioType);

	for(_puiCurVideoSampleId = 1; _puiCurVideoSampleId <= numSamples_vedio; )
	{
		#if 1
		ret = MP4_GetOneTiandyFrame_V2(pMp4Hdlr, _u32VideoTrackId, _u32AudioTrackId, &_puiCurVideoSampleId, &_puiCurAudioSampleId,
									_pcSps, spsSize, 
									_pcPps, ppsSize, 
									_pTiandyFrameH265Buf, 
									&stFrmaeHeader, 
									_iAudioType, 
									&_puiUseLessIdCount, 
									_iNeedIFrame);
		show_framHeadInfo(&stFrmaeHeader);
		return 0;
		#else
		ret = MP4_GetOneTiandyFrameH265_V2(pMp4Hdlr, _u32VideoTrackId, _u32AudioTrackId, &_puiCurVideoSampleId, &_puiCurAudioSampleId,
									_pcVps, vpsSize, 
									_pcSps, spsSize, 
									_pcPps, ppsSize, 
									_pTiandyFrameH265Buf, 
									&stFrmaeHeader, 
									_iAudioType, 
									&_puiUseLessIdCount, 
									_iNeedIFrame);
		#endif
		if(ret < 0)
		{
			PrintInfo("MP4_GetOneTiandyFrameH265_V2 failed!");
			break;
		}
		
		ret = write(fd, &stFrmaeHeader, sizeof(S_FrameHeader));
		if(ret != sizeof(S_FrameHeader))
		{
			PrintInfo("write failed!");
		}		
	
		ret = write(fd, _pTiandyFrameH265Buf, stFrmaeHeader.u32VStreamLen + stFrmaeHeader.u16AFrameSize*stFrmaeHeader.u16AFrames);
		PrintInfo("stFrmaeHeader.u32VStreamLen=%d", stFrmaeHeader.u32VStreamLen);
		PrintInfo("stFrmaeHeader.u16AFrameSize=%d", stFrmaeHeader.u16AFrameSize);
		PrintInfo("stFrmaeHeader.u16AFrames=%d", stFrmaeHeader.u16AFrames);
		if(ret != stFrmaeHeader.u32VStreamLen + stFrmaeHeader.u16AFrameSize*stFrmaeHeader.u16AFrames)
		{
			PrintInfo("write failed!");
		}		
	}

	
	PrintInfo("ok");
	close(fd);
	//MP4_ReadSampleIndex();
	//MP4_ReadSampleIndex_V3();

pps_error:
	free(_pcSps);
sps_error:
	free(_pcVps);
vps_error:
	
	return 0;
}


void initFileHeader(S_FileHeader *_pstFileHeader)
{
	memset(_pstFileHeader, 0, sizeof(S_FileHeader));
}

//====================================20151231mp4 format=====================start==========

typedef struct{
	struct list_head list;
	uint32_t	m_uiName;
	uint32_t 	m_uiSize;
	uint64_t 	m_ullPos;	
}TMP4Box;

static int Inner_CToU32(char *_pStr)
{
	return (_pStr[0] << 24 | _pStr[1] << 16 | _pStr[2] << 8 | _pStr[3] << 0);
}

int Inner_MP4BoxListInit(struct list_head *_pHead)
{
	INIT_LIST_HEAD(_pHead);
	return 0;
}

static int gCount = 0;
int Inner_MP4PushBox(struct list_head *_pHead, TMP4Box *_pTMP4Box)
{
	list_add_tail(&_pTMP4Box->list, _pHead);

	PrintInfo("gCount=%d", ++gCount);
	return 0;
}

TMP4Box *Inner_MP4PopBox(struct list_head *_pHead)
{
	PrintInfo("gCount=%d", --gCount);
	struct list_head *pList = _pHead->next;
	if(_pHead != pList)
	{
		TMP4Box *pMP4Box = container_of(pList, TMP4Box, list);
		__list_del_entry(pList);
		PrintInfo("pop successful");
		return pMP4Box;	
	}
	
	PrintInfo("pop failed");

	return NULL;
}

TMP4Box * Inner_MP4FindBoxByName(struct list_head *_pHead, TMP4Box *pTMP4Box)
{
	TMP4Box *pDstMember = NULL;
	struct list_head *pCurr;
	for(pCurr = _pHead->next; pCurr != _pHead; pCurr = pCurr->next)
	{
		TMP4Box *pCurrMember = container_of(pCurr, TMP4Box, list);
		//if(!strncmp((char *)(&pTMP4Box->m_uiName), (char *)(&pCurrMember->m_uiName), 4))
		if(pTMP4Box->m_uiName == pCurrMember->m_uiName)
		{	
			pDstMember = pCurrMember;
			PrintInfo("find successful pTMP4Box->m_uiName=%s", (char *)(&pTMP4Box->m_uiName));
			break;
		}
	}

	return pDstMember;

}

static int Inner_MP4ShowBox(TMP4Box *pTMP4Box)
{
	char name[5] = {0};

	*((uint32_t *)name) = pTMP4Box->m_uiName;
	PrintInfo("m_uiName=%s", name);
	PrintInfo("m_uiName=0x%x", pTMP4Box->m_uiName);
	PrintInfo("m_uiSize=0x%x", pTMP4Box->m_uiSize);
	PrintInfo("m_ullPos=0x%llx", pTMP4Box->m_ullPos);

}

static int Inner_MP4ListAllBox(struct list_head *_pHead)
{
	struct list_head *pCurr;
	TMP4Box *pCurrMember = NULL;
	
	for(pCurr = _pHead->next; pCurr != _pHead; pCurr = pCurr->next)
	{
	PrintInfo("===============================");
		pCurrMember = container_of(pCurr, TMP4Box, list);
		Inner_MP4ShowBox(pCurrMember);
	PrintInfo("===============================");
	}

	return 0;	
}


static TMP4Box *Inner_MallocMP4Box(char *_pName, uint32_t _size, uint64_t _pos)
{
	TMP4Box *pTMP4Box = (TMP4Box *)malloc(sizeof(TMP4Box));
	if(NULL == pTMP4Box)
	{
		PrintInfo("malloc failed!");
		return pTMP4Box;
	}
	memset(pTMP4Box, 0, sizeof(TMP4Box));
	PrintInfo("*((uint32_t *)_pName)=0x%x", *((uint32_t *)_pName));
	pTMP4Box->m_uiName = *((uint32_t *)_pName);
	pTMP4Box->m_uiSize = _size;
	pTMP4Box->m_ullPos = _pos;

	return pTMP4Box;
}

static int Inner_FreeMP4Box(TMP4Box *_pTMP4Box)
{
	int iRet = td_success;
	
	if(NULL == _pTMP4Box)
	{
		PrintInfo("free failed!");
		iRet = td_failure;
		goto leave;
	}
	
	memset(_pTMP4Box, 0, sizeof(TMP4Box));
	free(_pTMP4Box);

leave:

	return iRet;
}

#define	MP4BOX_COUNT		10

static char * localBoxBuf[] = {
		"ftyp",
		"free",
		"mdat",
		"moov",
			"mvhd",
			"iods", 
			"trak",
				"tkhd",
				"mdia",
					"mdhd",
					"hdlr",
					"minf",
						"vmhd",
						"smhd",
						"dinf",
							"dref",
							"url",
						"stbl",
							"stsd",
							"stts",
							"stsz",
							"stsc",
							"stco",
							"stss",
};


int Inner_FindBox(char *buf, int len)
{

	int iRet = td_failure;
	int i;
	if(len < 8)
	{
		goto leave;
	}
	
	for(i = 0; i < sizeof(localBoxBuf)/sizeof(localBoxBuf[0]); i++)
	{
		if(0 == strncmp(localBoxBuf[i], buf + 4, 4))
		{
			PrintInfo("localBoxBuf[%d]=%s", i, localBoxBuf[i]);
			iRet = td_success;
			break;
		}		
	}

	if(iRet == 0)
	{
		static int boxCount = 0;
		PrintInfo("boxCount=%d", ++boxCount);
	}
	
leave:

	return iRet;
}

static int Inner_GetMP4AllBox(char *name, struct list_head *_pListHead)
{	
	int iRet = td_success;
	size_t ret = 0;
	FILE *file = NULL;

	if(NULL == name)
	{
		PrintInfo("NULL == name");
		iRet = td_failure;
		goto leave;
	}
	file  = fopen(name, "r");
	if(NULL == file)
	{
		PrintInfo("fopen failed");
		iRet = td_failure;
		goto leave;
	}

	char buf[16] = {0};	

	Inner_MP4BoxListInit(_pListHead);
	while(1)
	{
		
		if(sizeof(buf) != fread(buf, 1, sizeof(buf), file))
		{
			if(feof(file))
			{
				PrintInfo("read end");
				break;
			}else
			{
				PrintInfo("read failed");
				iRet = td_failure;
				goto leave;
			}
		}
		
		if(Inner_FindBox(buf, sizeof(buf)) == 0)
		{			
			long pos = 0;
			long offset = 0;
			pos = ftell(file);
			if(pos < 0)
			{
				PrintInfo("ftell failed");
				iRet = td_failure;
				goto leave;
			}
			offset = ntohl(*((uint32_t *)buf)) - sizeof(buf);
			TMP4Box *pTMP4Box = Inner_MallocMP4Box(buf + 4, ntohl(*((uint32_t *)buf)), pos - sizeof(buf));
			if(NULL != pTMP4Box)
			{
				Inner_MP4PushBox(_pListHead,pTMP4Box);
			}
			if(Inner_FindBox(buf + 8, sizeof(buf) - 8) == 0)
			{			
				long pos = 0;
				pos = ftell(file);
				if(pos < 0)
				{
					PrintInfo("ftell failed");
					iRet = td_failure;
					goto leave;
				}
				offset = ntohl(*((uint32_t *)(buf + 8))) - sizeof(buf) + 8;
				TMP4Box *pTMP4Box = Inner_MallocMP4Box(buf + 12, ntohl(*((uint32_t *)(buf + 8))), pos - sizeof(buf) + 8);
				if(NULL != pTMP4Box)
				{
					Inner_MP4PushBox(_pListHead,pTMP4Box);
				}
			}
			
			if(fseek(file, offset, SEEK_CUR) < 0)
			{
				PrintInfo("fseek failed");
				iRet = td_failure;
				goto leave;
			}
		}
	
	}

leave:
	if((NULL != file) && (fclose(file) < 0))
	{
		PrintInfo("fclose failed!");
	}
	
	return iRet;
}

static int Inner_MP4JudgeFormat(struct list_head *_pInListHead)
{	
	int iRet = td_success;
	TMP4Box tMP4Box = {0};
	TMP4Box *pMdat= NULL;
	TMP4Box *pMoov= NULL;
	
	tMP4Box.m_uiName = htonl(Inner_CToU32("mdat"));
	pMdat = Inner_MP4FindBoxByName(_pInListHead, &tMP4Box);
	if(NULL == pMdat)
	{
		iRet = td_failure;
		goto leave;
	}
	Inner_MP4ShowBox(pMdat);
	
	tMP4Box.m_uiName = htonl(Inner_CToU32("moov"));
	pMoov = Inner_MP4FindBoxByName(_pInListHead, &tMP4Box);
	if(NULL == pMoov)
	{
		iRet = td_failure;
		goto leave;
	}
	Inner_MP4ShowBox(pMoov);

	if(pMdat->m_ullPos < pMoov->m_ullPos)
	{
		iRet= td_success;
	}else
	{
		iRet = td_failure;
		goto leave;
	}

leave:

	return iRet;

}

static int Inner_FreeMP4AllBox(struct list_head *_pListHead)
{
	int iRet = td_success;
	struct list_head *pCurr;
	TMP4Box *pCurrMember = NULL;
	
	for(pCurr = _pListHead->next; pCurr != _pListHead; )
	{
		pCurrMember = Inner_MP4PopBox(_pListHead);
		pCurr = pCurr->next;
		if(NULL != pCurrMember)
		{
			Inner_MP4ShowBox(pCurrMember);
			memset(pCurrMember, 0, sizeof(TMP4Box));
			free(pCurrMember);
			pCurrMember = NULL;
			PrintInfo("====Inner_FreeMP4AllBox=====")
		}
	}

	return iRet;	
}


#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

static  int Inner_MP4Conver(char *_pNameInput, char *_pNameOutput, struct list_head *_pListHeadInput)
{
	int iRet = td_success;
	int fdIn = 0;
	int fdOut = 0;
	
	fdIn = open(_pNameInput, O_RDWR);
	if(fdIn < 0)
	{
		PrintInfo("open failed!");
		iRet = td_failure;
		goto leave;
	}
	fdOut = open(_pNameOutput, O_RDWR|O_CREAT, 0644);
	if(fdOut < 0)
	{
		PrintInfo("open failed!");
		iRet = td_failure;
		goto leave;
	}

	struct stat statTmp;
	if(fstat(fdIn, &statTmp) < 0)
	{
		PrintInfo("fstat failed!");
		iRet = td_failure;
		goto leave;
	}
	
	char *fdInPtr = mmap(NULL, statTmp.st_size, PROT_READ, MAP_SHARED, fdIn, 0);
	if(MAP_FAILED == fdInPtr)
	{
		PrintInfo("mmap failed!");
		iRet = td_failure;
		goto leave;
	}
	
	TMP4Box tMP4Box = {0};
	TMP4Box *ptr= NULL;
	tMP4Box.m_uiName = htonl(Inner_CToU32("ftyp"));
	ptr = Inner_MP4FindBoxByName(_pListHeadInput, &tMP4Box);
	if(NULL == ptr)
	{
		PrintInfo("Inner_MP4FindBoxByName failed!");
		iRet = td_failure;
		goto leave;
	}
	Inner_MP4ShowBox(ptr);
	PrintInfo("offset page = %lld", ptr->m_ullPos/(1 << 12));
	if(write(fdOut, fdInPtr + ptr->m_ullPos, ptr->m_uiSize) != ptr->m_uiSize)
	{
		PrintInfo("write failed!");
		iRet = td_failure;
		goto leave;
	}

	
	tMP4Box.m_uiName = htonl(Inner_CToU32("moov"));
	ptr = Inner_MP4FindBoxByName(_pListHeadInput, &tMP4Box);
	if(NULL == ptr)
	{
		PrintInfo("Inner_MP4FindBoxByName failed!");
		iRet = td_failure;
		goto leave;
	}
	Inner_MP4ShowBox(ptr);
	PrintInfo("offset page = %lld", ptr->m_ullPos/(1 << 12));
	if(write(fdOut, fdInPtr + ptr->m_ullPos, ptr->m_uiSize) != ptr->m_uiSize)
	{
		PrintInfo("write failed!");
		iRet = td_failure;
		goto leave;
	}
	
	tMP4Box.m_uiName = htonl(Inner_CToU32("mdat"));
	ptr = Inner_MP4FindBoxByName(_pListHeadInput, &tMP4Box);
	if(NULL == ptr)
	{
		PrintInfo("Inner_MP4FindBoxByName failed!");
		iRet = td_failure;
		goto leave;
	}
	Inner_MP4ShowBox(ptr);
	PrintInfo("offset page = %lld", ptr->m_ullPos/(1 << 12));
	if(write(fdOut, fdInPtr + ptr->m_ullPos, ptr->m_uiSize) != ptr->m_uiSize)
	{
		PrintInfo("write failed!");
		iRet = td_failure;
		goto leave;
	}
	

	tMP4Box.m_uiName = htonl(Inner_CToU32("stco"));
	ptr = Inner_MP4FindBoxByName(_pListHeadInput, &tMP4Box);
	Inner_MP4ShowBox(ptr);


leave:
	
	if((NULL != fdInPtr) && (munmap(fdInPtr, statTmp.st_size) < 0))
	{
		PrintInfo("munmap failed!");
		iRet = td_failure;
	}
	
	if((fdIn > 0) && (close(fdIn) < 0))
	{
		PrintInfo("close failed!");
		iRet = td_failure;
	}
	if((fdOut > 0) && (close(fdOut) < 0))
	{
		PrintInfo("close failed!");
		iRet = td_failure;
	}
	
	return iRet;
}

static int Inner_MP4ModStco(char *_pNameOutput, struct list_head *_pInListHead, struct list_head *_pOutListHead)
{
	int iRet = td_success;
	int fdOut = -1;
	TMP4Box tMP4Box = {0};
	TMP4Box *ptr= NULL;
	uint64_t offset = 0;
	
	if((NULL == _pNameOutput) || (NULL == _pInListHead) || (NULL == _pOutListHead))
	{
		iRet = td_failure;
		goto leave;
	}
	fdOut = open(_pNameOutput, O_RDWR, 0644);
	if(fdOut < 0)
	{
		PrintInfo("open failed!");
		iRet = td_failure;
		goto leave;
	}

	tMP4Box.m_uiName = htonl(Inner_CToU32("mdat"));
	ptr = Inner_MP4FindBoxByName(_pInListHead, &tMP4Box);
	if(NULL == ptr)
	{
		PrintInfo("Inner_MP4FindBoxByName failed!");
		iRet = td_failure;
		goto leave;
		
	}
	Inner_MP4ShowBox(ptr);
	offset = ptr->m_ullPos;
	
	
	tMP4Box.m_uiName = htonl(Inner_CToU32("mdat"));
	ptr = Inner_MP4FindBoxByName(_pOutListHead, &tMP4Box);
	if(NULL == ptr)
	{
		PrintInfo("Inner_MP4FindBoxByName failed!");
		iRet = td_failure;
		goto leave;
		
	}
	Inner_MP4ShowBox(ptr);
	offset = ptr->m_ullPos - offset;

	PrintInfo("offset=0x%llx", offset);
	tMP4Box.m_uiName = htonl(Inner_CToU32("stco"));
	ptr = Inner_MP4FindBoxByName(_pOutListHead, &tMP4Box);
	if(NULL == ptr)
	{
		PrintInfo("Inner_MP4FindBoxByName failed!");
		iRet = td_failure;
		goto leave;
		
	}
	Inner_MP4ShowBox(ptr);
	struct stat statTmp;
	if(fstat(fdOut, &statTmp) < 0)
	{
		PrintInfo("fstat failed!");
		iRet = td_failure;
		goto leave;
	}

	char *fdOutPtr = mmap(NULL, statTmp.st_size, PROT_READ|PROT_WRITE, MAP_SHARED, fdOut, 0);
	if(MAP_FAILED == fdOutPtr)
	{
		PrintInfo("mmap failed!");
		iRet = td_failure;
		goto leave;
	}

	
	uint32_t *stcoPtr = (uint32_t *)(fdOutPtr + ptr->m_ullPos);
	int nums = ntohl(*(stcoPtr + 3));
	PrintInfo("nums = %x", nums);
	
	int i = 0;
	uint32_t value;
	for(i = 0; i < nums; i++)
	{
		value = ntohl(*(stcoPtr + 4 + i)) + offset;
		PrintInfo("value = 0x%x", value);
		*(stcoPtr + 4 + i) = htonl(value);
	}
	
leave:
	if((fdOut > 0) && (close(fdOut) < 0))
	{
		PrintInfo("close failed!");
		iRet = td_failure;
	}
	
	if((NULL != fdOutPtr) && (munmap(fdOutPtr, statTmp.st_size) < 0))
	{
		PrintInfo("munmap failed!");
		iRet = td_failure;
	}

	return iRet;
}

static int Inner_MP4HandleFile(char *_pOldFileName, char *_pNewFileName)
{
	//_pNewFileName = "zzzz.mp4";
	int iRet = td_success;
	
	if(rename(_pOldFileName, _pNewFileName) < 0)
	{
		PrintInfo("rename failed!");
		iRet = td_failure;
	}

	return iRet;
}

td_s32 MP4_Format(char *_pFileName)
{
	int iRet = td_success;
	char *inName = _pFileName;
	char tempName[512] = {0};	
	int len = strlen(_pFileName);
	struct list_head inputFileListHead;
	struct list_head outputFileListHead;

	if(NULL == _pFileName)
	{
		PrintInfo("NULL == _pFileName");
		iRet = td_failure;
		goto leave;
	}
	PrintInfo("_pFileName=%s",_pFileName);
	
	if(len > sizeof(tempName))
	{
		PrintInfo("tempName array outside");
		iRet = td_failure;
		goto leave;
	}	
	strncpy(tempName, _pFileName, strlen(_pFileName));
	tempName[len-3] = 't';
	tempName[len-2] = 'm';
	tempName[len-1] = 'p';

	Inner_GetMP4AllBox(inName, &inputFileListHead);
	if(Inner_MP4JudgeFormat(&inputFileListHead) < 0)
	{
		PrintInfo("stop conver");
		iRet = td_failure;
		goto leave;
	}else
	{
		PrintInfo("start conver");
	}
	
	if(Inner_MP4Conver(inName, tempName, &inputFileListHead) < 0)
	{
		PrintInfo("Inner_MP4Conver failed!");
		iRet = td_failure;
		goto leave;
	}
	
	if(Inner_GetMP4AllBox(tempName, &outputFileListHead) < 0)
	{
		PrintInfo("Inner_MP4Conver failed!");
		iRet = td_failure;
		goto leave;
	}
	
	if(Inner_MP4ListAllBox(&outputFileListHead) < 0)
	{
		PrintInfo("Inner_MP4Conver failed!");
		iRet = td_failure;
		goto leave;
	}
	if(Inner_MP4ModStco(tempName, &inputFileListHead, &outputFileListHead) < 0)
	{
		PrintInfo("Inner_MP4Conver failed!");
		iRet = td_failure;
		goto leave;
	}
	
	if(Inner_FreeMP4AllBox(&inputFileListHead) < 0)
	{
		PrintInfo("Inner_MP4Conver failed!");
		iRet = td_failure;
		goto leave;
	}
	if(Inner_MP4ListAllBox(&inputFileListHead) < 0)
	{
		PrintInfo("Inner_MP4Conver failed!");
		iRet = td_failure;
		goto leave;
	}
	if(Inner_FreeMP4AllBox(&outputFileListHead) < 0)
	{
		PrintInfo("Inner_MP4Conver failed!");
		iRet = td_failure;
		goto leave;
	}
	if(Inner_MP4ListAllBox(&outputFileListHead) < 0)
	{
		PrintInfo("Inner_MP4Conver failed!");
		iRet = td_failure;
		goto leave;
	}
	
	if(Inner_MP4HandleFile(tempName, _pFileName) < 0)
	{
		PrintInfo("Inner_MP4Conver failed!");
		iRet = td_failure;
		goto leave;
	}

leave:
	
	return iRet;
}


int parseMp4(int argc, char ** argv)
{
	int i = 0;
	char *name = NULL;
	td_MP4FileHandle *pMp4Hdlr = NULL;
	
	for(i = 0; i < argc; i++)
	{
		PrintInfo("argv[%d]=%s", i, argv[i]);
	}

	if(argc > 2)
	{
		name = argv[2];
	}else
	{
		//name = "tiandy.mp4";
		//name = "H31C0M20151219140904C35h264.mp4";
		//name = "H31C0M20151219140904C35h264cwm.mp4";
		//name = "ch004_20151217115424t.mp4";
		name = "ch004_20151217115424tcwm.mp4";
		//name = "H31C0M20151219140904C35.mp4";
		//name = "ch07_20151223101732m9747.mp4";
	}

	MP4_Format(name);	

}

//====================================20151231mp4 format=====================end==========


int main(int argc, char ** argv)
{
	if(argc < 2)
	{		
		PrintInfo("./bin/mp4-convert-tool.bin 0 inputfile.sdv outputfile.h265");
		PrintInfo("./bin/mp4-convert-tool.bin 1 inputfile.sdv outputfile.mp4");
		PrintInfo("./bin/mp4-convert-tool.bin 2 inputfile.mp4 outputfile.sdv");
		PrintInfo("./bin/mp4-convert-tool.bin 3 inputfile.mp4");
		return 0;
	}
	int i = 0;
	for(i = 0; i < argc; i++)
	{
		PrintInfo("argv[%d]=%s", i, argv[i]);
	}
	PrintInfo("argv[1]=%d", i, argv[1]);
	PrintInfo("argv[1]=%d", i, atoi(argv[1]));
	switch(atoi(argv[1]))
	{
		case 0:
		sdvToVideoRawData(argc, argv);
			break;
			
		case 1:
		sdvToMp4(argc, argv);
			break;
			
		case 2:
		playMp4(argc, argv);
			break;
			
		case 3:
		parseMp4(argc, argv);
			break;
			
		default:
			break;
	}

	return 0;
}


