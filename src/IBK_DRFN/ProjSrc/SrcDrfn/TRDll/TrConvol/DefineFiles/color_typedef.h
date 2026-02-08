#ifndef COLOR_TYPEDEFINE
#define COLOR_TYPEDEFINE

// Use Common
#define COLOR_BLACK											  RGB(0, 0, 1)
#define COLOR_WHITE											  RGB(255, 255, 255)
#define COLOR_BLUE										  	RGB(0, 0, 255)

//##### Main Wnd #####
// #define COLOR_NORMAL_FACE									RGB(205, 209, 220)
// #define COLOR_MAIN_CANVAS									RGB(213, 213, 213)
// #define COLOR_SPLITTER										RGB(205, 209, 220)

#define COLOR_MAIN_CANVAS		RGB(247,248,252)
#define COLOR_NORMAL_FACE		RGB(247, 248, 252)
#define COLOR_DLG_CANVAS		COLOR_MAIN_CANVAS
#define COLOR_SPLITTER			RGB(171, 168, 153)

//##### Grid Color #####
//#define COLOR_GRID_HEADER_TEXT				    RGB(0, 0, 1)
#define COLOR_GRID_HEADER_TEXT	RGB(255, 255, 255)
#define COLOR_GRID_HEADER_FACE	RGB(227, 231, 237)

#define COLOR_GRID_SEL_ROW		RGB(255, 243, 198)
#define COLOR_GRID_EVEN_ROW		RGB(255, 255, 255)
#define COLOR_GRID_ODD_ROW		RGB(249, 249, 249)
#define COLOR_GRID_LINE			RGB(172, 178, 185)

#define COLOR_TAB_SELECT		RGB(207, 215, 229)
#define COLOR_TAB_NORMAL		RGB(254, 254, 254)
#define COLOR_TAB_CANVAS		COLOR_TAB_SELECT

#define COLOR_STATE_MAX			RGB(190, 2, 1)
#define COLOR_STATE_UP			RGB(221, 130, 47)
#define COLOR_STATE				RGB(111, 126, 155)
#define COLOR_STATE_DOWN		RGB(107, 191, 228)
#define COLOR_STATE_MIN			RGB(1, 83, 165)
#define COLOR_STATE_CNT			RGB(255, 255, 255)
#define COLOR_STATE_AVG			COLOR_NORMAL_FACE

#define BTN_WIDTH_BASE			12
#define BTN_WIDTH_STEP			13



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
	strcpy(logFont.lfFaceName, (const char*)_T("굴림체"));  \
  fontData.CreateFontIndirect(&logFont);

// #define GETFONT(logFont, nHeight, nWeight, fontData) \
// 	logFont.lfHeight = nHeight;                            \
// 	logFont.lfWidth = 0;                                    \
// 	logFont.lfEscapement = 0;                               \
// 	logFont.lfOrientation = 0;                              \
// 	logFont.lfWeight = nWeight;                            \
// 	logFont.lfItalic = 0;                                   \
// 	logFont.lfUnderline	=	0;                                \
// 	logFont.lfStrikeOut	=	0;                                \
// 	logFont.lfCharSet	=	DEFAULT_CHARSET;                                \
// 	logFont.lfOutPrecision	=	OUT_DEFAULT_PRECIS;                            \
// 	logFont.lfClipPrecision	=	CLIP_DEFAULT_PRECIS;                            \
// 	logFont.lfQuality	=	DEFAULT_QUALITY;                                  \
// 	logFont.lfPitchAndFamily=	FF_ROMAN;                           \
// 	strcpy(logFont.lfFaceName, (const char*)_T("돋움체"));  \
//   fontData.CreateFontIndirect(&logFont);
	
/*
#define GETPATH_APP(strAppPath)      \
  IGateManager* pGateMng;         \
  AfxGetIGateManager(pGateMng);   \
	IMainInfoManager *pMainInfoManager = (IMainInfoManager*)pGateMng->GetInterface(21);\
	strAppPath = pMainInfoManager->GetRootDir();
*/

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

#define FOLDER_IMG            "\\Image\\"

#define IDB_LEXP_UP         FOLDER_IMG + "bt__L화면확장_up.bmp"
#define IDB_LEXP_DOWN       FOLDER_IMG + "bt__L화면확장_down.bmp"
#define IDB_LEXP_DIS        FOLDER_IMG + "bt__L화면확장_dis.bmp"

#define IDB_REXP_UP         FOLDER_IMG + "bt__R화면확장_up.bmp"
#define IDB_REXP_DOWN       FOLDER_IMG + "bt__R화면확장_down.bmp"
#define IDB_REXP_DIS        FOLDER_IMG + "bt__R화면확장_dis.bmp"

#define IDB_RUN_UP                FOLDER_IMG + "run_up.bmp"
#define IDB_RUN_DOWN              FOLDER_IMG + "run_down.bmp"
#define IDB_RUN_DIS               FOLDER_IMG + "run_dis.bmp"

#define IDB_STOP_UP               FOLDER_IMG + "stop_up.bmp"
#define IDB_STOP_DOWN             FOLDER_IMG + "stop_down.bmp"
#define IDB_STOP_DIS              FOLDER_IMG + "stop_dis.bmp"

#endif // COLOR_TYPEDFINE
