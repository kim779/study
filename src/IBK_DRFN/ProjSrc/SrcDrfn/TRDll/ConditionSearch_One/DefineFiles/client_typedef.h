#ifndef CLIENT_TYPEDEF
#define CLIENT_TYPEDEF

#define WMU_SIGNAL_CONDITION	(WM_USER + 657)	// 신호알리미 - 조건검색	

#define _countof(array) (sizeof(array)/sizeof(array[0]))

#define KEY_SHARD_CODE      "코드-코코2"
#define KEY_SHARD_TEMA      "코드-테마"
#define KEY_SHARD_INST      "코드-업종"

#define PROGRAM_TITLE				"빠른종목검색"
#define PROGRAM_TITLE_REAL			"실시간조건검색"
#define PROGRAM_TITLE_INDUSTRY		"빠른업종검색"

const		LPCTSTR		SCREEN_NO[] = { _T("05010"), _T("05020"), _T("05030"), _T("05040"), _T("05050"), 
																	_T("05060"), _T("05070"), _T("05080"), _T("05090") };
const		LPCTSTR		SCREEN_NO_INDUSTRY[] = { _T("05100"), _T("05110"), _T("05120"), _T("05130"), _T("05140") };

const		LPCSTR		FIELDEDIT_TAB_STR[] = {_T("시세"), _T("호가"), _T("시고저가"), _T("최고/최저"), _T("투자자"), _T("예상"), _T("기타")};

#define TAB_TITLE_CONTRACT			"예상체결"
#define TAB_TITLE_USERCONDI			"사용자검색"
#define TAB_TITLE_REALSEARCH		"실시간검색"

#define XML_VERSION_FIXFIELDFILE	"1.0"
#define FIXFIELD_SYS_FILE			"fixfield_sys.xml"
#define FIXFIELD_USER_PATTERN_FILE	"fixfield_User_Pattern.xml"

#define FIXFILED_USER_ONE_ITEM		"fixfield_User_One_Item.xml"
#define FIXFILED_USER_ONE_MAKRET	"fixfield_User_One_Market.xml"
#define FIXFILED_USER_ONE_REAL		"fixfield_User_One_Real.xml"

//# Menu
#define ID_MENU_START			10010
#define ID_MENU_END				10200		//# 10190, 10200 : Naver Serch

// 직원용과 고객용 화면번호가 다르기 때문에 수정함. 07.09.12

const		UINT		MENU_INDEX[] = { 10010, 10020 };
const		LPCTSTR		DRMENU_NAME[] = { _T("현재가"), _T("종합차트") };
//const		LPCTSTR		MENU_SCREEN_NO[] = { _T("0501"), _T("1401") };
const		LPCTSTR		MENU_SCREEN_NOMAX[] = { _T("2001"), _T("4501") };
//const		LPCTSTR		MENU_SCREEN_NOEMP[] = { _T("00310"), _T("00583") };
const		LPCTSTR		MENU_MAP_NO[] = { _T("hfis077aq"), _T("hfit031aq") };

const		UINT		MENU_INDUSTRY_INDEX[] = { 10010, 10020 };
const		LPCTSTR		MENU_INDUSTRY_NAME[] = { _T("업종지수현재가"), _T("업종차트") };
const		LPCTSTR		MENU_INDUSTRY_SCREEN_NOMAX[] = { _T("0531"), _T("1101") };
const		LPCTSTR		MENU_INDUSTRY_SCREEN_NOEMP[] = { _T("00397"), _T("00586") };
const		LPCTSTR		MENU_INDUSTRY_MAP_NO[] = { _T("hfib21020"), _T("hfib21010") };


#define ID_MENU_RESULTCTRL_START			11010
#define ID_MENU_RESULTCTRL_END				11300
const UINT			MENU_RESULTCTRL_INDEX[] = { 11010, 11020, 11040, 11050, 11055, 11060 };
const LPCTSTR		MENU_RESULTCTRL_NAME[] = {"성과분석", "관심등록", "돌려보기", _T(""), "EXCEL"};

const LPCTSTR		MENU_RESULTCTRL_SCREEN_NOMAX[] = {"7129", "관심등록", "돌려보기", _T(""), _T("")};
const LPCTSTR		MENU_RESULTCTRL_SCREEN_NOEMP[] = { "2001", "4501", "관심등록", _T(""), _T("")};

//const	LPCTSTR		MENU_RESULTCTRL_MAP_NO[] = { _T("hfit031aq"), _T("관심등록"), _T(""), _T("") }; 
const	LPCTSTR		MENU_RESULTCTRL_MAP_NO[] = { _T("2001"), _T("4501"), _T("관심등록"), _T(""), _T("") }; 

const BOOL			MENU_RESULTCTRL_SCREEN_OPEN[] = { TRUE, TRUE, TRUE, TRUE, FALSE, FALSE};

#define SCREEN_INDEX_EXCEL		MENU_RESULTCTRL_INDEX[4]

#define SCREEN_NO_PORTMAX				MENU_RESULTCTRL_SCREEN_NOMAX[2]
#define SCREEN_NO_PORTEMP				MENU_RESULTCTRL_SCREEN_NOEMP[2]
#define SCREEN_NO_CHARTMAX				MENU_RESULTCTRL_SCREEN_NOMAX[0]
#define SCREEN_NO_CHARTEMP				MENU_RESULTCTRL_SCREEN_NOEMP[1]
#define SCREEN_NO_LOOK					MENU_RESULTCTRL_SCREEN_NOMAX[2]

#define SCREEN_NO_ROLL					"NOT USER"
//#define SCREEN_NO_ANALYSISMAX			MENU_RESULTCTRL_SCREEN_NOMAX[1]
//#define SCREEN_NO_ANALYSISEMP			MENU_RESULTCTRL_SCREEN_NOEMP[1]

#define SCREEN_NO_ALLBUY			"NOT USER"

//# 조건 list Struct!
typedef struct tagConditionInfo
{
	int nGroupID;
	CString strGroupName;

	CString strName;
	CString strPath;
	CString strAlertKey;
	CString strExplain;

	BOOL bLoad;
	int  nOrderCnt;
	User_Finder		stUser_Finder;
} ConditionInfo;
#define SZ_CONDITIONINFO	sizeof(ConditionInfo)


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
	CWnd*	pWnd;
	BOOL	bVisible;
} ST_MOVEITEM;
#define SET_ST_MOVEITEM(xST, xName, xpWnd) xST.szCtrlName=xName; xST.pWnd=xpWnd;

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

typedef struct tagQuickList
{
	BOOL		bCheck;
	CString	strType;
	BOOL		bAlertReg;

	CString strFileName;
	CString	strQuickName;
	CString	strFilePath;
	CString	strFolderPath;
} QuickList;

typedef struct tagGridEventInfo
{
	CString strGridType;

	int nCol;
	int nRow;
	CString strType; //#=> BUTTON / CHECK / COMBO
	CString strData;
} GridEventInfo;

#define WMU_SIGNAL_CONDSEARCH	(WM_USER + 0x731)
typedef struct _tag_Alarm_Condsearch_Info
{
	TCHAR	szScrNo[4+1];			// - 화면번호
	TCHAR	szCode[12+1];			// - 종목코드
	TCHAR	szMemegb[1+1];		// - 매매구분(0:구분없음 1:매도 2:매수)
} ALARM_CONDSEARCH_INFO, *LPALARM_CONDSEARCH_INFO;

#define GETMARKET_ALL		1000
#define GETMARKET_KOSPI		0
#define GETMARKET_KOSPI200	100
#define	GETMARKET_KOSDAQ	200
#define GETMARKET_KRX		900

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

#endif // CLIENT_TYPEDEF
