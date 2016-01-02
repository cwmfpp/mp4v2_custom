//*********************************
// 通用函数库
// dxl 20110314 create
//*********************************

#ifndef _LIB_OTHER_H_
#define _LIB_OTHER_H_

#include "td_types.h"
#include "td_errno.h"

// 获取文件列表
int file_filter(int (*judge)(char* file_name), char* path, char file_list[][256], int* file_num);

// 修改文件扩展名
int chext(char* file_name, const char* new_ext);

int prime_list(int* list, int max);

// 字符串比较
int strcmpx(char* first, char* second);

// 颜色格式转换
td_s32 BGR2RGB(td_u32 _uiColor);
td_u32 RGB2BGR(td_s32 _uiColor);

// 检测字符串_pcSourceString中是否包含指定的字符集_pcCharacterSet内容
td_s32 if_has_bad_character(td_u8* _pcSourceString, td_u8* _pcCharacterSet);


// 串口属性字符串到数据结构的转换
int ComString2Struct(const td_u8* _pcString, TCom* _pstStruct);


/*
 * 函数名称：VideoSize2WidthHeight
 * 简要描述：根据视频大小换算出视频的宽和高
 * 输	  入：
 * 			_iNorm, 制式。PAL/NTSC
 *			_iSize,大小。FD1/HD1/FCIF/HCIF/QCIF
 *			_iVideoWidth,传出视频宽。非空
 * 			_iVideoHeight,传出视频高。非空
 * 输	 出：
 * 			TD_SUCCESS,成功
 *			TD_FAILURE,失败
 * 修改日志：
 * 			2009-10-29,陶永亮,创建
 */
int VideoSize2WidthHeight(int _iNorm,int _iSize,int* _iVideoWidth,int* _iVideoHeight);


/*
 * 函数：WidthHeight2VideoSize
 * 功能：根据宽高换算出视频大小,适用于数字摄像机(如sdi)
 * 输	  入：
 * 			_iNorm, 制式
 *			_iSize,大小。FD1/HD1/FCIF/HCIF/QCIF
 *			_iVideoWidth,传出视频宽。非空
 * 			_iVideoHeight,传出视频高。非空
 * 输	 出：
 * 			大小。HD1/F1080P/F720P等
 * 修改日志：20121009 hq create
 */
int WidthHeight2VideoSize(int _iNorm, int _iVideoWidth, int _iVideoHeight);
int Play_WithHeightToVideoSize(int _iWith, int _iHeight);
/*
shihc add 20120311
为矢量字体增加，字符分辨率换算字型大小。
_iPx:通道分辨率的x轴放大系数，相对cif; _iPy同理
*/
td_s32 osd_CountSizeIndex(td_s32 _iPx , td_s32 _iPy);

//以QCIF为标准，换算不同分辨率下，字符叠加的内容占用的宽高的倍数
td_void osd_GetZoomTime(td_s32 _s32VideoSize, td_s32 *_ps32Px,td_s32 *_ps32Py, td_s32 _s32Type);

/*
实现将CIF的尺寸拉伸2倍，应对hd1,1080I分辨率。
shihc add 20120324:HD1的实现原理是将CIF的字符横向拉伸2倍
shihc add 20120607:增加1080I 字符横向拉伸2倍成1080P
*/
td_s32 CIFx2(td_s32 _iNum, td_s32 _iSizeIndex);


// 根据pos计算osd实际坐标
// width, height:视频尺寸
// pos:位置0-15
// len:ods字符长度
// x, y : 坐标值输出
// dlx 20110331
void OsdPosToXY(int _iWidth, int _iHeight, int _iPos, int _iLen, int *_iX, int *_iY,int _iNorm);

 

/** Places base 64 encoded input buffer into output buffer.
*/
int base64_encode(unsigned char* outputBuf, unsigned int* outputLen,
                          unsigned char* inputBuf, unsigned int inputLen);


/** Places base 64 decoded input buffer into output buffer.
*/
int base64_decode(unsigned char* outputBuf, unsigned int* outputLen,
                          unsigned char* inputBuf, unsigned int inputLen);

/*
    _pcVData    待加密数据缓冲
    _iDataLen   待加密数据长度
    _iCTRKey    唯一标识
    _pucAESKey  密钥，需扩展到16位

    加密算法简介：唯一标识是视频帧的帧号，使用AES算法对帧号进行加密，
                  将加密后的结果与原始视频数据进行异或操作，实现对视频数据的加密。
                  客户端使用相同的算法即可获得解密后的视频数据
*/	 
int aes_crypt(unsigned char* _pcVData,int _iDataLen,unsigned int _iCTRKey,
	unsigned char* _pucAESKey,int _iAESKeyLen);


// 检测ip地址与子网掩码是否合法
// 返回值: 0合法, -1非法
int Valid_IPMask(int _IP, int _SubMask);

// 检测ip地址串是否合法
// 返回值: 0合法，其他值非法
int Valid_IP(const char *  _IPAddress);

// 千分比转换
#define permillage_rate(_iValue, _iMaxValue)	((_iMaxValue) ? (_iValue) * 1000 / (_iMaxValue) : 0)   
#define permillage_value(_iRate,  _iMaxValue)	((_iRate) * (_iMaxValue) / 1000) 
void permillage_shitf(int _iType, int _iWidth, int _iHeight, int *_piXp, int *_piYp);

// 检测字符串是否超出长度
// 参数:
// _psSrc : 字符串
// _pcSplit : 分隔符
// _iMaxLineLen : 每行最大长度
// _iFieldNum : 最大行数
// 返回值: 0合法，-1溢出
int IsStringTooLong(td_u8* _pcSrc, td_u8* _pcSplit, td_s32  _iMaxLineLen, td_s32 _iFieldNum);

// 根据视频尺寸重调整区域
void countMaskRect(int _iVideoSize, Rect_t *_pRect);

char *strncopy(char *_strDst, const char *_strSrc, int n);

// 新旧结构转换
// type : 0 : 老结构->新结构, 1 : 新结构->老结构
// width, height 当前视频宽高
// old : 老结构(千分比)
// new : 新结构(实际值)

//type值定义
#define NEW_TO_OLD   1
#define OLD_TO_NEW   0
void ChnNameOsdShift(int _iType, int _iWidth, int _iHeight, OSD_CHANNAME_T *_ptOld, TChnNameOsdCnf *_ptNew);
void ChnTimeOsdShift(int _iType, int _iWidth, int _iHeight, OSD_CHANNAME_T *_ptOld, TChnTimeOsdCnf *_ptNew);
void ChnTextOsdShift(int _iType, int _iWidth, int _iHeight, STR_WordOverlay *_ptOld, TChnTextOsdCnf *_ptNew);
void ChnVideoCoverAreaShift(int _iType, int _iWidth, int _iHeight, Rect_t *_ptOld, TChnArea *_ptNew);
void ChnMdAreaShift(int _iType, td_u32 *_puiOld, TChnMdAreaCnf *_ptNew);
void ChnLogoOsdShift(int _iType, int _iWidth, int _iHeight, OSD_CHANNAME_T *_ptOld, TChnLogOsdCnf *_ptNew);


int StrToTime(const char* _strTime);

void GuiChannels2DvrChannels(const td_s32* _piGuiCh, td_s32* _piChannelNum, td_s32* _piDvrCh);
void DvrChannels2GuiChannels(td_s32 _iChannelNum, const td_s32* _piDvrCh, td_s32* _piGuiCh);


int AudioFramLenToAoPoiNum(int _iType,int _iFramLen);

//根据视频尺寸计算位置(尺寸越大位置越小)
int VideoSizeIndex(int _iSize);
//比较视屏大小，_size1 > _isze2, 为正
int CmpVideoSize(int _iSize1, int _iSize2);
//根据最大视频分辨率，构造列表
int GetVideoList(int _iMinSize, int _iMaxSize, int *_ptpVideoList, int _iMaxCnt);
/*
*获取rtc时间秒数
*更新日志:20121129，王松创建
*/
int GetRtcTimeSeconds(time_t *_pTime);

//用于计算字符叠加的绝对坐标
td_s32 channel_ComputeCifPos(int _iVideoSize, int _iPosFrom[2], int _iPosTo[2]);
//用于将结构中保存的相对坐标换算成绝对坐标
td_s32 channel_ComputeSetPos(int _iVideoSize, int _iPosFrom[2], int _iPosTo[2]);

int MD5File (char *filename,char *_cOutputBuf);
int MD5BigFile (char *filename);
int Md5BigFileInit(int _iCdNo);
int Md5BigFileFinal(int _iCdNo,char *_pcMd5Buffer);
int MD5_Encrypt(char *_cOutputBuf, int _iOutputLen, const char *_cInputbuf, int _iInputLen);
int DES_Decrypt(char *_cOutputBuf, int _iOutputLen, char _cKey[8], const char *_cInputbuf, int _iInputLen);
int DES_Encrypt(char *_cOutputBuf, int _iOutputLen, char _cKey[8], const char *_cInputbuf, int _iInputLen);
int EncryptSrcToDesToBase64(char *_cOutputBuf, int _iOutputLen, char _cKey[8], const char *_cInputbuf, int _iInputLen);
int DecodeBase64toDesToSrc(char *_cOutputBuf, int _iOutputLen, char _cKey[8], const char *_cInputbuf, int _iInputLen);
int EncryptStrToMd5ToBase64(const char* _cSrcStr1, int _iStrLen1, const char * _cSrcStr2, int _iStrLen2, OUT char *_cOutBuf, int _iOutLen);
#endif

