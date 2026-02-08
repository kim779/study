#ifndef COLOR_TYPEDEFINE
#define COLOR_TYPEDEFINE

//# Use Common
#define COLOR_BLACK											  RGB(0, 0, 1)
#define COLOR_WHITE											  RGB(255, 255, 255)
#define COLOR_BLUE										  	RGB(0, 0, 255)
#define COLOR_RED                         RGB(255, 0, 0)

//# Main Color
#define COLOR_MAIN_CANVAS		RGB(213, 213, 213)
#define COLOR_TAB_BACK			RGB(232, 226, 210)
#define COLOR_NORMAL_FACE		RGB(247, 248, 252)
#define COLOR_DLG_CANVAS		COLOR_MAIN_CANVAS
#define COLOR_SPLITTER			RGB(171, 168, 153)

//# Tab Color
#define COLOR_TAB_SELECT									RGB(218, 218, 218)
#define COLOR_TAB_NORMAL									RGB(192, 192, 192)
#define COLOR_TAB_CANVAS									COLOR_TAB_SELECT

//# Grid Color
#define COLOR_GRID_HEADER_TEXT				    RGB(255, 255, 255)
#define COLOR_GRID_HEADER_FACE				    RGB(218, 218, 218)

#define COLOR_GRID_SEL_ROW								RGB(255, 243, 198)
#define COLOR_GRID_EVEN_ROW								RGB(255, 255, 255)
#define COLOR_GRID_ODD_ROW								RGB(242, 245, 250)
#define COLOR_GRID_LINE										RGB(167, 167, 167)

#define BTN_WIDTH_BASE      12
#define BTN_WIDTH_STEP      13

#define COLOR_STATE_MAX										RGB(190, 2, 1)
#define COLOR_STATE_UP										RGB(221, 130, 47)
#define COLOR_STATE												RGB(111, 126, 155)
#define COLOR_STATE_DOWN									RGB(107, 191, 228)
#define COLOR_STATE_MIN										RGB(1, 83, 165)
#define COLOR_STATE_CNT										RGB(255, 255, 255)
#define COLOR_STATE_AVG										COLOR_NORMAL_FACE

#define COLOR_BTN										      RGB(212, 208, 200)



#define GETFONT(logFont, nHeight, nWeight, fontData) \
	logFont.lfHeight = nHeight;                            \
	logFont.lfWidth = 0;                                    \
	logFont.lfEscapement = 0;                               \
	logFont.lfOrientation = 0;                              \
	logFont.lfWeight = nWeight;                            \
	logFont.lfItalic = 0;                                   \
	logFont.lfUnderline	=	0;                                \
	logFont.lfStrikeOut	=	0;                                \
	logFont.lfCharSet	=	129;                                \
	logFont.lfOutPrecision	=	3;                            \
	logFont.lfClipPrecision	=	2;                            \
	logFont.lfQuality	=	1;                                  \
	logFont.lfPitchAndFamily=	49;                           \
	strcpy(logFont.lfFaceName, (const char*)_T("±¼¸²Ã¼"));  \
  fontData.CreateFontIndirect(&logFont);

// #define GETFONT(logFont, nHeight, nWeight, fontData) \
// 	logFont.lfHeight = nHeight;                            \
// 	logFont.lfWidth = 0;                                    \
// 	logFont.lfEscapement = 0;                               \
// 	logFont.lfOrientation = 0;                              \
// 	logFont.lfWeight = FW_NORMAL;                            \
// 	logFont.lfItalic = 0;                                   \
// 	logFont.lfUnderline	=	0;                                \
// 	logFont.lfStrikeOut	=	0;                                \
// 	logFont.lfCharSet	=	DEFAULT_CHARSET;                                \
// 	logFont.lfOutPrecision	=	OUT_DEFAULT_PRECIS;                            \
// 	logFont.lfClipPrecision	=	CLIP_DEFAULT_PRECIS;                            \
// 	logFont.lfQuality	=	DEFAULT_QUALITY;                                  \
// 	logFont.lfPitchAndFamily=	FF_ROMAN;                           \
// 	strcpy(logFont.lfFaceName, (const char*)_T("µ¸¿òÃ¼"));  \
//   fontData.CreateFontIndirect(&logFont);
	

#define GETPATH_APP(strAppPath)       \
  char buf[255];                                                        \
	GetModuleFileName(NULL, buf, 255);                                    \
	int lastlocationofbackslash = 0;                                      \
	int nBufCnt = strlen(buf);                                            \
	for (int i = 0; i < nBufCnt; i++)                                     \
	{                                                                     \
		if (buf[i] == '\\')                                                 \
			lastlocationofbackslash = i;                                      \
	}                                                                     \
	CString strAppPath = _T("");                                          \
	for (int j = 0; j < lastlocationofbackslash; j++)                     \
		strAppPath += buf[j];          
	
#endif // COLOR_TYPEDFINE
