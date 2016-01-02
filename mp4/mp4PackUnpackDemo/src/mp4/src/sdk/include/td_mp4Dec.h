
#ifndef	TD_MP4DEC_H
#define TD_MP4DEC_H

#ifndef TD_MP4DEC_H
#define TD_MP4DEC_H


#include <unistd.h>
#include <malloc.h>
#include <dirent.h>
#include <arpa/inet.h>

//#include "public_def.h"
#include "td_mp4.h"
#if 0
typedef void*       td_MP4FileHandle;

typedef struct{
	AVCodecID	m_eVCodecType;
	td_u32	m_uiVTrackId;
	td_u32	m_uiVNumberOfSamples;
	td_u32	m_uiVCurrSampleId;
	td_u8	*m_pucVParameSet;
	td_u16	m_usVParameSetLen;
}TMp4DecVideo;

typedef struct{
	td_u32	m_uiAudioType;
	td_u32	m_uiATrackId;
	td_u32	m_uiANumberOfSamples;
	td_u32	m_uiACurrSampleId;	
}TMp4DecAudio;

typedef struct{
	td_void			*m_pMp4Handle;
	TMp4DecVideo	m_stMp4DecVideo;
	TMp4DecAudio	m_stMp4DecAudio;
}TMp4Dec;


typedef struct{

}TMP4FileHead;

typedef enum{
	SEEK_FORWARD,
	SEEK_BACKWARD,
	SEEK_ABSOLUTE,		
}ESeekOps;

td_MP4FileHandle * MP4_Open (td_char *_pcFileName);
td_void MP4_Close (td_MP4FileHandle *_pMP4fh);
td_s32 MP4_GetFileHead(td_MP4FileHandle *_pMP4fh, TMP4FileHead *_pstMP4FileHead);
td_s32 MP4_Read (td_MP4FileHandle *_pMP4fh, td_void *_pFrameBuf, td_u32 _bufLen);
td_s32 MP4_Seek(td_MP4FileHandle *_pMP4fh, td_u64 _frameId, ESeekOps _type);
#endif
#endif
#endif //TD_MP4DEC_H

