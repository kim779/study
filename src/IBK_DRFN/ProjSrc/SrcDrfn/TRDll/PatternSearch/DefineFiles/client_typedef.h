#ifndef CLIENT_TYPEDEF
#define CLIENT_TYPEDEF

#define KEY_SHARD_CODE				"코드-종목"
#define KEY_SHARED_GETTIME			"시간-차트"
#define KEY_SHARED_SETTIME			"시간-그리드;"

#define PROGRAM_TITLE				"추세패턴검색"

#define XML_VERSION_FIXFIELDFILE	"1.0"

#define WON_TYPE					0

#define FOLDER_DATA					"\\icss\\finddata\\PatternData"
#define FOLDER_FIND					"\\icss\\finddata"

#define FOLDER_IMAGE				"\\icss\\Image\\drchart"
#define FOLDER_PATTERNIMAGE			"\\icss\\finddata\\PatternData\\PatternImage"
// #define FOLDER_IMAGE				"\\Image\\Chart"
// #define FOLDER_PATTERNIMAGE		"\\Image\\Chart\\Pattern"

#define FINDERINFO_INIFILE			"FindInfo.ini"
#define FOLDER_FINDDATA				"\\FindData"

#define FIXFIELD_SYS_FILE			"fixfield_sys.xml"
#define FIXFIELD_USER_PATTERN_FILE	"fixfield_User_Pattern.xml"

#define PATTERNINFO_INIFILE			"PatternInfo.ini"

#define MAX_FIELD_CNT				17		//# 결과 Grid Column 구성 Max Count

#define USRDIR						"\\user\\"
#define gszCacheDir					"DrCache"
#define gszDataKey_MstCode			"MstCode"
#define gszDataKey_WorkDate			"WorkDate"
#define usrMasterCondition			"SearchTemp"

#define _countof(array) (sizeof(array)/sizeof(array[0]))

#define GETFONT(logFont, nHeight, nWeight, fontData)        \
	logFont.lfHeight = nHeight;                             \
	logFont.lfWidth = 0;                                    \
	logFont.lfEscapement = 0;                               \
	logFont.lfOrientation = 0;                              \
	logFont.lfWeight = nWeight;                             \
	logFont.lfItalic = 0;                                   \
	logFont.lfUnderline	= 0;                                \
	logFont.lfStrikeOut	= 0;                                \
	logFont.lfCharSet = 129;                                \
	logFont.lfOutPrecision = 3;                             \
	logFont.lfClipPrecision = 2;                            \
	logFont.lfQuality = 1;                                  \
	logFont.lfPitchAndFamily = 49;                          \
	strcpy(logFont.lfFaceName, (const char*)_T("굴림체"));  \
	fontData.CreateFontIndirect(&logFont);

#define LONG2STR_BUF(pBuff, nSize, strData, strBuff)	\
    strBuff.Format("%.*ld", nSize, strData);			\
	memcpy(pBuff, strBuff, strBuff.GetLength());

#define Safe_CheckCopy2(xSource, xTarget, xLen, xStr)   \
	xStr.Format("%-*.*s", xLen, xLen, xTarget);         \
	xStr.TrimRight();									\
	if(xStr.GetLength()) memcpy(xSource, (LPCSTR)xStr, xStr.GetLength())

#define Safe_CheckCopy(xSource, xTarget) if(xTarget.GetLength()) memcpy(xSource, (LPCSTR)xTarget, xTarget.GetLength())

#define STR_CONVERT(pBuff, cData, nSize, strData)	    \
	pBuff = new char[nSize + 1];					    \
	memset(pBuff, 0x00, nSize + 1);						\
	memcpy(pBuff, cData, nSize);						\
	strData = (CString)pBuff;							\
	strData.TrimRight();								\
delete [] pBuff;

#define INT_CONVERT(pBuff, cData, nSize, nData)     	\
	pBuff = new char[nSize + 1];						\
	memset(pBuff, 0x00, nSize + 1);					    \
	memcpy(pBuff, cData, nSize);						\
	nData = atoi(pBuff);								\
delete [] pBuff;

#define LONG_CONVERT(pBuff, cData, nSize, nData)		\
  pBuff = new char[nSize + 1];							\
  memset(pBuff, 0x00, nSize + 1);						\
  memcpy(pBuff, cData, nSize);							\
  nData = atol(pBuff);									\
delete [] pBuff;

#define ID_MENU_START			10010
#define ID_MENU_END				10200		//# 10190, 10200 : Naver Serch

const UINT			RBTNMENU_INDEX[] =		{ 10010, 10020 };
const LPCTSTR		RBTNMENU_NAME[] =		{ _T("현재가"), _T("종합차트") };
const LPCTSTR		RBTNMENU_SCREEN_NO[] =	{ _T("0501"), _T("1401") };
const LPCTSTR		RBTNMENU_SCREEN_NOEMP[] = { _T("00310"), _T("00583") };
const LPCTSTR		RBTNMENU_MAP_NO[] =		{ _T("hfis077aq"), _T("hfit031aq") };

const LPCTSTR		MENU_RESULTCTRL_NAME[] =		 {"성과분석", "관심등록", "돌려보기", _T(""), "EXCEL"};
const LPCTSTR		MENU_RESULTCTRL_SCREEN_NOMAX[] = {"7129", "관심등록", "돌려보기", _T(""), _T("")};

const LPCTSTR		MENU_RESULTCTRL_SCREEN_NOEMP[] = { "2001", "4501", "관심등록", _T(""), _T("")};
const UINT			MENU_RESULTCTRL_INDEX[] =		 { 11010, 11020, 11030, 11040, 11050, 11055};
const	LPCTSTR		MENU_RESULTCTRL_MAP_NO[] = { _T("2001"), _T("4501"), _T("관심등록"), _T(""), _T("") }; 
const BOOL			MENU_RESULTCTRL_SCREEN_OPEN[] =  { TRUE, TRUE, TRUE, TRUE, FALSE, FALSE};
const LPCSTR		FIELDEDIT_TAB_STR[] = {_T("시세"), _T("호가"), _T("시고저가"), _T("최고/최저"), _T("투자자"), _T("예상"), _T("기타")};

#define SCREEN_INDEX_EXCEL			MENU_RESULTCTRL_INDEX[4]
#define SCREEN_INDEX_RUSULTPRINT	MENU_RESULTCTRL_INDEX[4]

#define SCREEN_NO_ANALYSISEMP		MENU_RESULTCTRL_SCREEN_NOEMP[1]

#define SCREEN_NO_CHART				MENU_RESULTCTRL_SCREEN_NOMAX[0]
#define SCREEN_NO_ANALYST			MENU_RESULTCTRL_SCREEN_NOMAX[0]
#define SCREEN_NO_ORDER				MENU_RESULTCTRL_SCREEN_NOMAX[2]
#define LINK_PORT					MENU_RESULTCTRL_SCREEN_NOMAX[3]

#define FILE_GROUP				"\\portfolio.ini"


#define ID_MENU_RESULTCTRL_START	11010
#define ID_MENU_RESULTCTRL_END		11300

typedef struct tagSignalData
{
	int nRealType;
	CString strType;  //# News : ProfferID, Sys : NULL
	
	int nMarket;
	CString strCode;
	CString strName;
	
	CString strDate;
	CString strTime;
	
	CString strTitle;
	int nPoint;
	int nSignalImageIndex;
	COLORREF clrTitle;
} SignalData;


typedef struct tagSignalSetData
{
	int nTextSharedChk;
	int nNewsType;
	CString strNewsScreen;
	int nConditionType;
	CString strConditionScreen;
	
	int nOrderSharedChk;
	int nOrderType;
	CString strOrderScreen;
	
	int nSoundChk;
	int nSoundType;
	CString strSound;
} SignalSetData;



typedef struct tagPostureInfo
{
	CString strCode;
	CString strPosture;
	int nPoint;
} PostureInfo;

typedef struct tagCodeData
{
	CString strCode;
	CString strName;
} CodeData;

typedef struct tagHeadInfoType
{
	long        lHeadID;
	CString     strHeadName;
	int         nUnitNum;
	CString     strUnitText;
	CString     strDataType;
	int         nDataLength;
	int         nColWidth;
	CString     strAlignment;
} HeadInfoType;

typedef struct tagHeadFixInfo
{
	int           nColNum;        
	HeadInfoType  stHeadType;
	BOOL          bView;
} HeadFixInfo;


typedef struct tagUniversGroup
{
	CString strGroup;
	CStringArray *psaItem;
} UniversGroup;

typedef struct tagShardData
{
	BOOL bOpen;
	CString strScreenNo;
	CString strMapNo;
	
	CUIntArray iaMarkType;
	CStringArray saCode;
	CStringArray saName;
} ShardData;

typedef struct 
{
	CString szCtrlName;
	CWnd*   pWnd;
	int 	nType;
	BOOL	bEnable;
} ST_MOVEITEM;

struct	Hana_UpMaster	{
	char	jgub;			// 장구분
	// 0:장내 1:장외
	// 1: KOSPI200
	// 3: KOSDAQ50(1), KQ-IT 50(2)
	// 4: K100P(1), K50(2), IT(3
	// 6: SECTOR, 7:프리보드 
	char	ucod;			// 업종코드
	char	hnam[20];	// 한글명
};

struct	CUMaster	{
	CString m_szName;		// 18 업종명
	CString m_szGubun;		// 1 구분
	CString m_szCode;		// 3  업종코드
};

#define SET_ST_MOVEITEM(xST, xName, xpWnd) xST.szCtrlName=xName; xST.pWnd=xpWnd; xST.nType=0; xST.bEnable=1;
#define PF_ITEM_BTN		    0
#define PF_ITEM_RADIOBTN    1
#define PF_ITEM_CHECKBTN    2
#define PF_ITEM_COMBOBOX    3
#define PF_ITEM_CALENDAR	4

#define PF_EVENT_COMBO_ADDLIST		 1
#define PF_EVENT_COMBO_ADDSTRING	 2
#define PF_EVENT_COMBO_GETWINDOWTEXT 3
#define PF_EVENT_COMBO_SETCURSEL     4
#define PF_EVENT_COMBO_GETCURSEL     5
#define PF_EVENT_COMBO_RESETCONTENTS 6

#define PF_EVENT_BTN_CLICK			 7
#define PF_EVENT_BTN_SETWINDOWTEXT   8

#define PF_EVENT_CHK_SETCHECK		20
#define PF_EVENT_SHOWWINDOW			10

#define PF_CONTROL_SHOW	1
#define PF_CONTROL_HIDE	0

typedef struct  
{
	int		nCurSel;
	CString strWindowText;
} ST_PF_CONTROL_RETURN;

typedef struct  
{
	CString szCtrlName;
	int		nControlType;
	int		nEventID;
	BOOL	bState;
	int		nData;
	CString szData;
	CStringArray arrData;
	ST_PF_CONTROL_RETURN stReturn;
} ST_PF_CONTROL_EVENT;
#define SET_PF_CONTROL_EVENT(xST, xName, xType, xState) xST.szCtrlName=xName; xST.nControlType=xType;\
	xST.bState=xState; xST.nEventID=0; xST.szData="";\
	xST.stReturn.nCurSel=0; xST.stReturn.strWindowText="";


typedef struct  
{
	COLORREF clrF;
	COLORREF clrB;	
	COLORREF clrSF;
	COLORREF clrSB;
	COLORREF clrBACK;
} ST_TABCOLOR;

typedef struct  
{
	COLORREF clrGridHeader;
	COLORREF clrGridHText;  
	COLORREF clrGridHilight;
	COLORREF clrGridSBk;
	COLORREF clrGridSText;  
	COLORREF clrGridUpText;  
	COLORREF clrGridDownText;
	COLORREF clrGridVolText; 
} ST_GRIDCOLOR;

#define GETMARKET_ALL		1000
#define GETMARKET_KOSPI		0
#define GETMARKET_KOSPI200	100
#define	GETMARKET_KOSDAQ	200
#define GETMARKET_KRX		900

#define FILE_DATAVERSION		"SvrVersionDataFile.cfg"
#define FILE_DATAVERSION_LOCAL	"LocalVersionDataFile.cfg"
#define FILE_FINDDATA			"finddata"
#define FILE_LANGGUAGEDATA		"data"
#define FILE_EXP				"exp"
#define FILE_MAP				"map"
#define FILE_OTHER				"other"
#define FILE_PATTERNDATA		"patterndata"
#define FILE_SYS				"sys"
#define FILE_EXTENSION			".zip"

#endif // CLIENT_TYPEDEF
