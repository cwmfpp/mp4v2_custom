#ifndef __FONT__H_
#define __FONT__H_

#define DVR_GUI_LAN_PATH "./lang"      //UI 所有的语言包所在路径
#define CUR_GUI_LAN_PATH "./gui_config/curlang"  //当前语言配置路径
#define CUR_GUI_LAN_DIR  "./lang/%s"   //当前语言包文件所在目录

#define FONTLIB_FILE	   		"./gui_config/fontinfo.txt"
#define DEFAULT_FONT_FILE	 "./gui_config/arial.ttf"

//#define ft_render_mode_normal  FT_RENDER_MODE_NORMAL
//#define ft_render_mode_mono    FT_RENDER_MODE_MONO


typedef struct
{
	int iLangNo;
	char cLangBuf[32];
}TGuiLangType; //语言文件的结构lang.txt

typedef struct
{
	int m_iWidth;
	int m_iHeight;
	int m_borderX;
	int m_bordery;
	void *m_usBmp;
} TFontBmp;   //字符位图信息

int  GetFontName(char* _pcFontPath, char* _cFileName, int* _iFontSize); //获取字库名称
int IsFontFileExist(char* _cFileName);  //判断字库文件是否存在
int SetFontSize(int _iFontSize[], int _iLen);	
int LoadFontResourse(const  char* _pcFontFile); //装载初始化字库
int LoadSupportLang(void);
int LoadSlaveFontResource();
int ReleaseFontResource();//释放字库资源

int GetFontBits(unsigned short _uCode, int _iIndex,  unsigned int _tFontColor, unsigned int _tBgColor, TFontBmp* _tFontBmp);

int GetUniCharWidth(unsigned short _usUni, int _iSizeIndex); //获取一个字符的宽度
int GetUniStringWidth(const unsigned char *_strWords, int _iSizeIndex);// 获取一个字符串的宽度
int GetUniStringWidthEx(unsigned char *_strWords, int _iSizeIndex, int _iMaxWidth);

int GetUniCharBytes(const unsigned char *_strWords);//获取一个unicode编码字符占几个字节
int GetFontHeight(int _iSizeIndex);  //获取字体的高度
int GetIndexBySize(int _iSize);  //根据字体宽度 分配字号

const int GetCurGuiLangValue(); //获取当前语言的值
const char *GetCurGuiLangDir(char *_cDir);//获取当前语言的目录

int SetCurGuiLangValue(int _iValue); //设置当前语言

int GetCurDevGuiLangList(TGuiLangType *_GuiLangType, int *_iLangCnt); //获取当前语言列表返回值为当前支持语言的个数

unsigned short Utf8toUnicode ( const unsigned char  *_pucUtf8, int *_iBytes);

int GB2312ToUTF8(const unsigned char *_Gb2312, unsigned char *_Utf8, int _iBufSize);//gb2312转utf8
int UTF8ToGB2312(const unsigned char *_Utf8, unsigned char *_Gb2312, int _iBufSize);//utf8转gb2312
unsigned short Gb2312ToUnicode(unsigned char *str);

//获取自定义语言编号对应的国际简写代码
char* GetSimpleCodeByIndex(int _iIndx);

//获取简写代码对应的自定义编号
int GetLangIndexBySimepleCode(char* _pCode);

//中文字符串的截断
int TruncationString(unsigned char* _strText, int _iStrSize);

#endif
