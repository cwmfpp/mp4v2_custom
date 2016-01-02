//====================================================== 
// libftp.h by jiaofy at 20060224 
//====================================================== 
#ifndef _FTPC_H
#define _FTPC_H

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <strings.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 
#include <ctype.h> 
#include <unistd.h> 
#include <errno.h> 
#include <dirent.h> 
//#include <curses.h> 
#include <termios.h> 
#include <sys/stat.h> 
#include <fcntl.h> 
#include <signal.h> 

#include <sys/ioctl.h>


#define BINARY 1 
#define ASCII 0 

#define PASV 1 
#define PORT 0 

#define DEFAULT_FTP_PORT 21 
#define TIMEOUT 			5 

//Add by yangyanqiang for AlcaTel
#define SOFTWARESIZE 1500000  //Byte

#define INVALID_SOCKET -1

typedef struct 
{ 
	long size_bytes; 			/* total bytes transferred */ 
	float seconds; 				/* time taken to transfer */ 
	float kbs; 					/* kilobytes per second */ 
} SPEED; 

typedef struct { 
	FILE *filedes; 				/* data file descriptor */ 
	char transf_type; 			/* type of transfer (bin/ascii) */ 
	char transf_mode; 			/* mode of transfer (pasv/port) */ 
	char ftp_msg[1024+1]; 		/* error status of function or reply string from remote server */ 
	int  transf_calc; 			/* flag to turn on xfer rate calc */ 
	SPEED speed; 				/* xfer rate information */ 
	int sockfd; 				/* CONTRL connection socket id */ 
	int datasd; 				/* DATA connection socket id */ 
	int reply; 					/* return value of function or reply code from remote server */ 
	int debug; 					/* flag to turn on debugging */ 
	int linger; 				/* flag to turn on socket linger opt */ 
	int connected; 				/* flag to indicate the existance of a control connection for the session under question */ 
	int running;				/* zhangyi add 2010/11/30 */
	struct servent *ftp_sp; 	/* specific service entry */ 
} FTPINFO; 

//Cheng 100907 add 
#define TRANSFER_APPEND  	1
#define TRANSFER_STORE   	2

#define FILE_NAME_LEN 32
#define FILE_PATH_LEN  		64
#define FILE_PATH_LEN_FTP  	128
#define FTP_QUERY_PAGESIZE 	20


typedef struct 
{
	char m_cFileName[FTP_QUERY_PAGESIZE][FILE_PATH_LEN]; //file name include path
	char m_cFtpFileName[FTP_QUERY_PAGESIZE][FILE_PATH_LEN_FTP];
	char m_cFName[FTP_QUERY_PAGESIZE][FILE_NAME_LEN]; //file name without parth
	int m_iTotalNum;
	int m_iCompledNum ;
	int m_iCurrent;
	int m_iCurPageNo;
	int m_iPageSize;
	time_t m_tBegin;
	time_t m_tEnd;
	time_t m_tCurrent;
	int m_iFinish;
}sFtpFileInfo;

//sFtpFileInfo g_FtpFileInfo;

/* quit ftp */
int ftp_bye (FTPINFO * g_pCfgFileFtp );  

/* change remote server directory */
int ftp_chdir (FTPINFO *g_pCfgFileFtp, const char *rempath); 

/* send ftp command to remote server */
int ftp_command(FTPINFO *g_pCfgFileFtp, const char *optinfo, const char *action);

/* create data connect */
int ftp_dataconn ();  

/* delete file of remote server */
int ftp_del (FTPINFO *g_pCfgFileFtp, const char *file); 

/* list directory of remote server */
int ftp_dir (FTPINFO *g_pCfgFileFtp, const char *rempath,const char *localpath); 

/* get file from remote server */
int ftp_getfile (FTPINFO *g_pCfgFileFtp, const char *rempath,const char *localpath); 

/* login remote server *//* login after: *//* mode = PASV *//* type = BINARY */
int ftp_login (FTPINFO *g_pCfgFileFtp, const char *remhost,const char *user,const char *passwd,
	const char *account, const char *devname); 

/* create directory in remote server */
int ftp_mkdir (FTPINFO *g_pCfgFileFtp, const char *remdir); 

/* nlist directory of remote server */
int ftp_nlist (FTPINFO *g_pCfgFileFtp, const char *rempath,const char *localpath); 

/* send user password to remote server */
int ftp_passwd (FTPINFO *g_pCfgFileFtp, const char *password); 

/* send pasv to the remote server */
int ftp_pasv (FTPINFO *g_pCfgFileFtp, int *port); 

/* send port to the remote server */
int ftp_port (FTPINFO *g_pCfgFileFtp, const char *ip,const int port); 

/* create cortrol connect */
int ftp_prconnect(FTPINFO *g_pCfgFileFtp,const char *remhost, const char *devname); 

/* put file to remote server */
int ftp_putfile (FTPINFO *g_pCfgFileFtp, const char *rempath,const char *localpath); 

/* print the current directory of remote server */
int ftp_pwd ();  

/* recvdata from data connect */
int ftp_recvdata (FTPINFO *g_pCfgFileFtp, const char *localfile); 

/* rename file of remote server */
int ftp_rena (FTPINFO *g_pCfgFileFtp, const char *remsrcfile,const char *remdstfile); 

/* delete directory of remote server */
int ftp_rmdir (const char *remdir); 

/* senddata to data connect */
int ftp_senddata (FTPINFO *g_pCfgFileFtp, const char *localfile); 

/* set transfer mode [PASV/PORT] */
int ftp_setmode (FTPINFO *g_pCfgFileFtp, const int mode); 

/* set port of cortrol connect */
int ftp_setport (const int portnumber);  

/* set transfer type [BINARY/ASCII] */
int ftp_settype (FTPINFO *g_pCfgFileFtp, const int type); 

/* send user name to remote server */
int ftp_user(FTPINFO *g_pCfgFileFtp, const char *name);

/* get file by url*/
int ftp_getfileurl(FTPINFO *g_pCfgFileFtp, const char* ftpurl, const char *localPath);
//Cheng 0830 add mode for append

int ftp_putfileurl_ex(FTPINFO *_g_pCfgFileFtp, char* _ftpurl, const char *_localPath, int _mode, int _port, char *_nic);

void _initftpinfo(FTPINFO *g_pCfgFileFtp) ;

int ftp_start();
int ftp_stop();

int ftp_isstop();

//int ftp_bye_ex(FTPINFO *g_pCfgFileFtp) ;
#endif
