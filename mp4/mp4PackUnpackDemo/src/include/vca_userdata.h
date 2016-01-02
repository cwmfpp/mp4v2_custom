#ifndef __VCA_USER_DATA_H_
#define __VCA_USER_DATA_H_

#include "public_def.h"
#include "vca_head.h"

#define		MAX_SAVE_TARGET_NUM			16
#define     USERDATA_FLAG_GPS            	(0xFED4)       	                    //用户数据中附加GPS信息的标识
#define     USERDATA_FLAG_USER_DEFINE       (0x0001)       	                    //用户数据中附加用户自定义数据
#define     USERDATA_MAX_LENGTH             1024
#define     FLAG_VIRTUAL_FRAME_HEAD			0x02000000
#define     FLAG_VIRTUAL_FRAME_TAIL			0x0123ABCD


#define     USERDATA_FLAG_HEADER         	{ 0x00, 0x00, 0x00, 0x01, 0x06, 0xf0 }	//H264数据中用户数据标示
#define     USERDATA_FLAG_NEW_DATA_HEADER 	{'T', 'D', 'S', 'T'}                    //用于区别老设备用户数据的标识
#define     USERDATA_FLAG_OSD            	(0xFED1)         	                //用户数据中附加字符叠加的标识
#define     USERDATA_FLAG_VCA            	(0xFED2)          	                //用户数据中附加智能分析的标识
#define     USERDATA_FLAG_FRAMETIME      	(0xFED3)                                //用户数据中附加日期时间的标识
#define     USERDATA_FLAG_GPS            	(0xFED4)       	                        //用户数据中附加GPS信息的标识
#define     USERDATA_FLAG_USER_DEFINE           (0x0001)       	                        //用户数据中附加用户自定义数据


/////////////////////////////////H264用户数据结构头部/////////////////////////////////////////
typedef struct TUserDataHeader
{
	unsigned char ucFlag[4];           //TDST用于与现在已有的附加OSD信息和智能分析信息区分开。
	unsigned short uiSegmentCount; //段总数
	unsigned short uiDataLen;      //数据总长度（用户数据总长度包括段1至段n数据的长度之和，不包括“校验码”、“段总数”、“用户数据总长度”的长度之和。）
}TUserDataHeader,*pTUserDataHeader;

typedef struct TSegmentHeader
{
	unsigned short sFlag;           //
	unsigned short uiDataLen;      //数据总长度（用户数据总长度包括段1至段n数据的长度之和，不包括“校验码”、“段总数”、“用户数据总长度”的长度之和。）
}TSegmentHeader,*pTSegmentHeader;


int rec_vca_SetRuleparam(TVcaEventType _eVcaEventType,void *_pstVcaParam,time_t _starttime);
int rec_vca_GetVcaMsg(IN char *_strName, OUT int *_iCnt, OUT vca_TimeSegment *_ptTimeSegment, IN int _iLen);
#endif
