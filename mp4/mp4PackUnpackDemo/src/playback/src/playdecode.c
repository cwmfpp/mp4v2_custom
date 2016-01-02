#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>

#ifdef MEMWATCH
#include "memwatch.h"
#endif

#include "libtdao.h"
#include "decode.h"
#include "libstorage.h"
#include "td_media.h"

TVdecChannel *playdec_CreateVdec(int _iHeight, int _iWith, int _iChn)
{
	TVdecPara	vDecPara;
	TVdecChannel *ptVdec;
	vDecPara.m_iMod = 0;
	vDecPara.m_iType = VENC_H264;
	vDecPara.m_iWidth = _iWith;
	vDecPara.m_iHeight = _iHeight;

		
	if(NULL != (ptVdec = media_CreateVdecChannel(&vDecPara, 0, _iChn)))//RANDOM_NUM);//创建解码通道
	{
		return ptVdec;
	}
	
	return NULL;
}

int playdec_SendFileVStream(TVdecChannel *_tVdecChn, void *_buf, int _iVLen)
{
	if (0 != _tVdecChn->SendStream(_tVdecChn, _buf, _iVLen, 0, 0))
    	{
		return td_failure;
    	}
	return td_success;
}

int playdec_DestroyVdec(TVdecChannel *_tVdecChn, int _iChn)
{
	media_FreeVdecChannel(_tVdecChn, _iChn);
	return 0;
}