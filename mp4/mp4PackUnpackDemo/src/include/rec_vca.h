#ifndef _REC_VCA_H__
#define _REC_VCA_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "vca_head.h"

#ifndef NULL
#ifdef __cplusplus
#define NULL	0
#else
#define NULL	((void *)0)
#endif
#endif

	int rec_vca_Init(void);		//初始化

	int rec_vca_SetRule(TVcaEventType _eVcaEventType, void *_pstVcaParam);		//设置规则
	/*
		参数：
			_eVcaEventType	规则类型，绊线或周界
			_pstVcaParam	规则参数	
		返回：	-1		失败
			>=0		iId,编号		
	*/

	int rec_vca_SetUserData(int _iId, vca_TTargetSet* _ptBuffer);	//将用户数据送智能分析库分析
	/*
		参数：
			_iId		rec_vca_SetRule函数的返回值
			_ptBuffer	用户数据，如果为空，表示此次分析结束，底层可释放相关资源
		返回：	0		未报警
			1		报警	
	*/

#ifdef __cplusplus
}
#endif
#endif
