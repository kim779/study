#ifndef COLOR_TYPEDEFINE
#define COLOR_TYPEDEFINE

//# Use Common
#define COLOR_BLACK					RGB(0, 0, 1)
#define COLOR_WHITE					RGB(255, 255, 255)
#define COLOR_BLUE					RGB(0, 0, 255)
#define COLOR_RED						RGB(255, 0, 0)

//# Main Color
#define COLOR_MAIN_CANVAS		RGB(247, 248, 252)
#define COLOR_DLG_CANVAS		RGB(233, 233, 233)
#define COLOR_SPLITTER			RGB(171, 168, 153)

//# Tab Color
#define COLOR_TAB_SELECT									RGB(218, 218, 218)
#define COLOR_TAB_NORMAL									RGB(192, 192, 192)
#define COLOR_TAB_CANVAS									COLOR_MAIN_CANVAS

//# Grid Color
//#define COLOR_GRID_HEADER_TEXT				    RGB(0, 0, 1)
#define COLOR_GRID_HEADER_TEXT				    RGB(255, 255, 255)
#define COLOR_GRID_HEADER_FACE				    RGB(227, 227, 227)

#define COLOR_GRID_SEL_ROW								RGB(255, 243, 198)
#define COLOR_GRID_EVEN_ROW								RGB(255, 255, 255)
#define COLOR_GRID_ODD_ROW								RGB(242, 245, 250)
#define COLOR_GRID_LINE										RGB(167, 167, 167)

#define BTN_WIDTH_BASE      12
#define BTN_WIDTH_STEP      13

#define COLOR_QUICK_BTN			RGB(239, 239, 242)
#define COLOR_QUICK_CANVAS	RGB(237, 237, 237)

#define COLOR_MAPLOAD				COLOR_MAIN_CANVAS

//# Button
#define COLOR_NORMAL_FACE								RGB(220, 220, 220)
#define COLOR_BUTTON_IN                 RGB(222, 236, 156)

#define COLOR_BUTTON											RGB(239, 235, 239)
#define COLOR_BUTTON_TEXT									RGB(41, 85, 107)
#define COLOR_BUTTON_SEARCH								RGB(232, 215, 215)
#define COLOR_BORDER			RGB(101, 102, 108)


#define GETFONT(logFont, nHeight, nWeight, fontData) \
	logFont.lfHeight = nHeight;                            \
	logFont.lfWidth = 0;                                    \
	logFont.lfEscapement = 0;                               \
	logFont.lfOrientation = 0;                              \
	logFont.lfWeight = nWeight;                             \
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
	for (int k = strAppPath.GetLength() - 1; k > 0; k--)                  \
	{                                                                     \
		if (strAppPath[k] == '\\')                                          \
		{                                                                   \
			strAppPath = strAppPath.Mid(0, k);                                \
			break;                                                            \
		}                                                                   \
  }           
*/

/*
#define FOLDER_IMG            "\\Image\\button\\"

#define IDB_TREESEARCH_UP         FOLDER_IMG + "search_up.bmp"
#define IDB_TREESEARCH_DOWN       FOLDER_IMG + "search_down.bmp"
#define IDB_TREESEARCH_DIS        FOLDER_IMG + "search_dis.bmp"

#define IDB_TREENEXT_UP           FOLDER_IMG + "뒤로_up.bmp"
#define IDB_TREENEXT_DOWN         FOLDER_IMG + "뒤로_down.bmp"
#define IDB_TREENEXT_DIS          FOLDER_IMG + "뒤로_dis.bmp"

#define IDB_TREEPRE_UP            FOLDER_IMG + "앞으로_up.bmp"
#define IDB_TREEPRE_DOWN          FOLDER_IMG + "앞으로_down.bmp"
#define IDB_TREEPRE_DIS           FOLDER_IMG + "앞으로_dis.bmp"

#define IDB_TREECLOSE_UP          FOLDER_IMG + "트리축소_up.bmp"
#define IDB_TREECLOSE_DOWN        FOLDER_IMG + "트리축소_down.bmp"
#define IDB_TREECLOSE_DIS         FOLDER_IMG + "트리축소_dis.bmp"

#define IDB_TREEOPEN_UP           FOLDER_IMG + "트리확장_up.bmp"
#define IDB_TREEOPEN_DOWN         FOLDER_IMG + "트리확장_down.bmp"
#define IDB_TREEOPEN_DIS          FOLDER_IMG + "트리확장_dis.bmp"

#define IDB_TIP_UP                FOLDER_IMG + "tip_up.bmp"
#define IDB_TIP_DOWN              FOLDER_IMG + "tip_down.bmp"
#define IDB_TIP_DIS               FOLDER_IMG + "tip_dis.bmp"

#define IDB_EXP_UP                FOLDER_IMG + "확장_up.bmp"
#define IDB_EXP_DOWN              FOLDER_IMG + "확장_down.bmp"
#define IDB_EXP_DIS               FOLDER_IMG + "확장_dis.bmp"

#define IDB_CUT_UP                FOLDER_IMG + "확장_up.bmp"
#define IDB_CUT_DOWN              FOLDER_IMG + "확장_down.bmp"
#define IDB_CUT_DIS               FOLDER_IMG + "확장_dis.bmp"

#define IDB_UIUP_UP               FOLDER_IMG + "UI_Up_up.bmp"
#define IDB_UIUP_DOWN             FOLDER_IMG + "UI_Up_down.bmp"
#define IDB_UIUP_DIS              FOLDER_IMG + "UI_Up_dis.bmp"

#define IDB_UIDOWN_UP             FOLDER_IMG + "UI_Down_up.bmp"
#define IDB_UIDOWN_DOWN           FOLDER_IMG + "UI_Down_down.bmp"
#define IDB_UIDOWN_DIS            FOLDER_IMG + "UI_Down_dis.bmp"

#define IDB_MATCHVAL_UP           FOLDER_IMG + "matchval_up.bmp"
#define IDB_MATCHVAL_DOWN         FOLDER_IMG + "matchval_down.bmp"
#define IDB_MATCHVAL_DIS          FOLDER_IMG + "matchval_dis.bmp"

#define IDB_SAVE_UP               FOLDER_IMG + "조건저장_up.bmp"
#define IDB_SAVE_DOWN             FOLDER_IMG + "조건저장_down.bmp"
#define IDB_SAVE_DIS              FOLDER_IMG + "조건저장_dis.bmp"

#define IDB_RUN_UP                FOLDER_IMG + "run_up.bmp"
#define IDB_RUN_DOWN              FOLDER_IMG + "run_down.bmp"
#define IDB_RUN_DIS               FOLDER_IMG + "run_dis.bmp"

#define IDB_STOP_UP               FOLDER_IMG + "stop_up.bmp"
#define IDB_STOP_DOWN             FOLDER_IMG + "stop_down.bmp"
#define IDB_STOP_DIS              FOLDER_IMG + "stop_dis.bmp"

#define IDB_FIELDEDIT_UP          FOLDER_IMG + "항목편집_up.bmp"
#define IDB_FIELDEDIT_DOWN        FOLDER_IMG + "항목편집_down.bmp"
#define IDB_FIELDEDIT_DIS         FOLDER_IMG + "항목편집_dis.bmp"

#define IDB_INFOSEND_UP          FOLDER_IMG + "종목보내기_up.bmp"
#define IDB_INFOSEND_DOWN        FOLDER_IMG + "종목보내기_down.bmp"
#define IDB_INFOSEND_DIS         FOLDER_IMG + "종목보내기_dis.bmp"
*/


#endif // COLOR_TYPEDFINE
