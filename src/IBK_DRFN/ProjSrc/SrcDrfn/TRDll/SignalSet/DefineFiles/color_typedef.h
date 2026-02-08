#ifndef COLOR_TYPEDEFINE
#define COLOR_TYPEDEFINE
//# Use Common
#define COLOR_BLACK				RGB(0, 0, 1)
#define COLOR_WHITE				RGB(255, 255, 255)
#define COLOR_BLUE				RGB(0, 0, 255)
/*#define COLOR_RED				RGB(255, 0, 0)*/

//# Main Color
#define COLOR_MAIN_CANVAS		RGB(213, 213, 213)
#define COLOR_DLG_CANVAS		COLOR_MAIN_CANVAS
#define COLOR_SPLITTER			RGB(171, 168, 153)

//# Tab Color
#define COLOR_TAB_SELECT		RGB(218, 218, 218)
#define COLOR_TAB_NORMAL		RGB(192, 192, 192)
#define COLOR_TAB_CANVAS		COLOR_TAB_SELECT

//# Grid Color
#define COLOR_GRID_HEADER_TEXT	RGB(255, 255, 255)
#define COLOR_GRID_HEADER_FACE	RGB(227, 227, 227)

#define COLOR_GRID_SEL_ROW		RGB(255, 243, 198)
#define COLOR_GRID_EVEN_ROW		RGB(255, 255, 255)
#define COLOR_GRID_ODD_ROW		RGB(242, 245, 250)
#define COLOR_GRID_LINE			RGB(167, 167, 167)

#define COLOR_GRID_BTN_TEXT		RGB(0, 0, 1)
#define COLOR_GRID_BTN			RGB(218, 218, 218)

#define BTN_WIDTH_BASE			12
#define BTN_WIDTH_STEP			13


#define COLOR_SETWND_FACE		COLOR_WHITE
#define COLOR_TITLE_POINT		RGB(34, 143, 255)
#define COLOR_SUBTITLE_POINT	COLOR_BLACK
#define COLOR_SUBTITLE_TEXT		RGB(75, 75, 75)

#define COLOR_OUTLINE			RGB(163, 163, 163)
#define COLOR_MENULINE			RGB(111, 111, 111)


// #define GETFONT(logFont, nHeight, nWeight, fontData)		\
// 	logFont.lfHeight = nHeight;								\
// 	logFont.lfWidth = 0;                                    \
// 	logFont.lfEscapement = 0;                               \
// 	logFont.lfOrientation = 0;                              \
// 	logFont.lfWeight = nWeight;								\
// 	logFont.lfItalic = 0;                                   \
// 	logFont.lfUnderline	=	0;								\
// 	logFont.lfStrikeOut	=	0;								\
// 	logFont.lfCharSet	=	129;							\
// 	logFont.lfOutPrecision	=	3;							\
// 	logFont.lfClipPrecision	=	2;							\
// 	logFont.lfQuality	=	1;								\
// 	logFont.lfPitchAndFamily=	49;							\
// 	strcpy(logFont.lfFaceName, (const char*)_T("±¼¸²Ã¼"));	\
// 	fontData.CreateFontIndirect(&logFont);

#define GETFONT(logFont, nHeight, nWeight, fontData) \
	logFont.lfHeight = nHeight;                            \
	logFont.lfWidth = 0;                                    \
	logFont.lfEscapement = 0;                               \
	logFont.lfOrientation = 0;                              \
	logFont.lfWeight = FW_NORMAL;                            \
	logFont.lfItalic = 0;                                   \
	logFont.lfUnderline	=	0;                                \
	logFont.lfStrikeOut	=	0;                                \
	logFont.lfCharSet	=	DEFAULT_CHARSET;                                \
	logFont.lfOutPrecision	=	OUT_DEFAULT_PRECIS;                            \
	logFont.lfClipPrecision	=	CLIP_DEFAULT_PRECIS;                            \
	logFont.lfQuality	=	DEFAULT_QUALITY;                                  \
	logFont.lfPitchAndFamily=	FF_ROMAN;                           \
	strcpy(logFont.lfFaceName, (const char*)_T("µ¸¿òÃ¼"));  \
  fontData.CreateFontIndirect(&logFont);

#endif // COLOR_TYPEDFINE
