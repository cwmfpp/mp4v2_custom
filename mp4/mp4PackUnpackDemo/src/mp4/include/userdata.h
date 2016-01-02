#ifndef _USER_DATA_H_
#define _USER_DATA_H_

#include <time.h>

typedef enum
{
    USER_DATA_TIME = 0,
    USER_DATA_VCA,
    USER_DATA_VITAL_SIGN,
    USER_DATA_MAX,
}TUserDataType;

//时间段封装结构
typedef struct
{
	unsigned short	m_usSegmentflag;
	unsigned short	m_usSegmentLength;
	int 			m_CurrentTime;		
}TUserDataTimeSegment;


typedef enum
{
	VITAL_SIGN_HR = 1, //心率
	VITAL_SIGN_BO = 2, //血氧
 	VITAL_SIGN_BP = 3, //血压 
}TVitalSignDataType;

typedef struct //生命体征采集数据封装
{
	char m_cVersion;//协议版本
	char m_cType;	//生命体征采集类型,1：心率 2：血氧 3：血压
	char m_cMonFlg; //1:正常2:超上限 3：超下限4:脱落 5:设备离线    
	char m_iOscilloGramSeqID; //从1开始，心电最多三波依次传递1，2，3，血氧一波，使用1
	int m_iMonVal[4]; //体征实时值, 血压3个,SYS DIA PR依次存放，精度：原值乘以100;
	int m_iGramRate;//1s有多少个点
	int m_iSizeType; // 1:大 2：中 3：小
	int m_iXPosition; // 相对于视频区左上角(0,0)的相对横坐标
	int m_iYPosition; // 相对于视频区左上角(0,0)的相对纵坐标
	int m_iPointLen;
	unsigned char m_cPointValue[0]; //点集
} TUserDataVitalSign;

#define MAX_VITAL_DATA_POINT_LEN 125		//每个波形最多125个点
#define MAX_USER_DATA_VITAL_DATA_LEN		(sizeof(TUserDataVitalSign) + MAX_VITAL_DATA_POINT_LEN)

typedef struct
{
	unsigned short		m_usType;    			/*0xFEDA */
	unsigned short		m_usLentgh;  			/*数据长度*/
	TUserDataVitalSign	m_tVitalSignData;		/*生命体征数据*/
} TUserDataVitalSignSegment;

//定义用户私有数据结构
typedef struct
{
	char			m_cCheckoutSegment[4];
	unsigned short	m_usUserDataSegmentTotal;
	unsigned short	m_usUserDataSegmentLength;	
} TInsertUserData;

typedef int(* _InserUserDataFun)(int _iId,void * _pBuf,int _iSize);
int UserDataInit(int(*_InserUserDataFun)(int _iId, void *_pBuf, int _iSize));
void UserDataUninit();
typedef int (*_GetUserDataFun)(int _iId, void *_pBuf, int *_iBufSize);
void UserDataRegist(int _iType, int _iInterval, int _iId, int (*_GetUserDataFun)(int _iId, void *_pBuf, int *_iBufSize));

typedef struct _TUserDataTask
{
    TUserDataType m_iType;                        //用户数据类型
    int m_iInterval;                    //数据更新间隔,毫秒表示
    long long m_llNextUpdateTime;        //下一次更新时间
    struct _TUserDataTask *m_ptNextTask;//下一个任务指针    

    int (*GetUserDataFun)(int _iId, void *_pBuf, int *_iBufSize);//用户数据生成函数
}TUserDataTask;//用户数据任务结构，一种类型的用户数组对应一个任务

typedef struct _UserDataGroup
{
    int m_iId;                          //用户数据组编号，通常为编码通道号
    TUserDataTask *m_ptTaskList;        //本组对应用户数据任务列表
    struct _UserDataGroup *m_ptNextGrp; //下一个用户数据组
}TUserDataGroup;        //用户数据组，一个编码通道对应一个组

#endif
