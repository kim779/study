#ifndef COLOR_TYPEDEFINE
#define COLOR_TYPEDEFINE

//# Use Common
#define COLOR_BLACK											  RGB(0, 0, 1)
#define COLOR_WHITE											  RGB(255, 255, 255)
#define COLOR_BLUE										  	RGB(0, 0, 255)
#define COLOR_RED                         RGB(255, 0, 0)

//# Main Color
#define COLOR_MAIN_CANVAS		RGB(247, 248, 252)

#define COLOR_DLG_CANVAS		RGB(233, 233, 233)
#define COLOR_SPLITTER			RGB(171, 168, 153)

//# Tab Color
#define COLOR_TAB_SELECT									RGB(218, 218, 218)
#define COLOR_TAB_NORMAL									RGB(192, 192, 192)
#define COLOR_TAB_CANVAS									RGB(232, 226, 210)

//# Grid Color
#define COLOR_GRID_HEADER_TEXT				    RGB(255, 255, 255)
#define COLOR_GRID_HEADER_FACE				    RGB(227, 227, 227)

#define COLOR_GRID_SEL_ROW								RGB(255, 243, 198)
#define COLOR_GRID_EVEN_ROW								RGB(255, 255, 255)
#define COLOR_GRID_ODD_ROW								RGB(242, 245, 250)
#define COLOR_GRID_LINE										RGB(167, 167, 167)

#define BTN_WIDTH_BASE      12
#define BTN_WIDTH_STEP      13

#define COLOR_STATE_MAX										RGB(249, 60, 76)
#define COLOR_STATE_UP										RGB(243, 187, 81)
#define COLOR_STATE											RGB(182, 228, 104)
#define COLOR_STATE_DOWN									RGB(105, 222, 229)
#define COLOR_STATE_MIN										RGB(106, 158, 242)
#define COLOR_STATE_CNT										RGB(255, 255, 255)
#define COLOR_STATE_AVG										RGB(242, 245, 250)


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


/*
//#define FOLDER_IMG            "\\Image\\µ¿¿ì_Img\\"
#define FOLDER_IMG            "\\Image\\"

#define IDB_UILEFT_UP            FOLDER_IMG + "UI_Left_up.bmp"
#define IDB_UILEFT_DOWN          FOLDER_IMG + "UI_Left_down.bmp"
#define IDB_UILEFT_DIS           FOLDER_IMG + "UI_Left_dis.bmp"

#define IDB_UIRIGHT_UP           FOLDER_IMG + "UI_Right_up.bmp"
#define IDB_UIRIGHT_DOWN         FOLDER_IMG + "UI_Right_down.bmp"
#define IDB_UIRIGHT_DIS          FOLDER_IMG + "UI_Right_dis.bmp"

#define IDB_RUN_UP                FOLDER_IMG + "run_up.bmp"
#define IDB_RUN_DOWN              FOLDER_IMG + "run_down.bmp"
#define IDB_RUN_DIS               FOLDER_IMG + "run_dis.bmp"

#define IDB_STOP_UP               FOLDER_IMG + "stop_up.bmp"
#define IDB_STOP_DOWN             FOLDER_IMG + "stop_down.bmp"
#define IDB_STOP_DIS              FOLDER_IMG + "stop_dis.bmp"
*/

#endif // COLOR_TYPEDFINE
