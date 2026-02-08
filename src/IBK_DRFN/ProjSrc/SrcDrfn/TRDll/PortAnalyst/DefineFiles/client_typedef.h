#ifndef CLIENT_TYPEDEF
#define CLIENT_TYPEDEF

#define KEY_SHARD_CODE      "코드-코코2"
#define PROGRAM_TITLE		"조건검색 성과분석"
#define USERDIR				"\\user\\"

#define _countof(array) (sizeof(array)/sizeof(array[0]))

#define STR_CONVERT(pBuff, cData, nSize, strData)	\
  pBuff = new char[nSize + 1];							\
  memset(pBuff, 0x00, nSize + 1);						\
  memcpy(pBuff, cData, nSize);							\
  strData = (CString)pBuff;									\
  strData.TrimRight();											\
delete [] pBuff;

#define INT_CONVERT(pBuff, cData, nSize, nData)	\
  pBuff = new char[nSize + 1];						\
  memset(pBuff, 0x00, nSize + 1);					\
  memcpy(pBuff, cData, nSize);						\
  nData = atoi(pBuff);										\
delete [] pBuff;

//# [Group Analyst] ####################################################################

#define ID_MENU_START			10010
#define ID_MENU_END				10200		//# 10190, 10200 : Naver Serch
const		UINT			MENU_INDEX[] = { 10010, 10020 };
//const		LPCTSTR		MENU_NAME[] = { _T("현재가"), _T("종합차트") };
const		LPCTSTR		DRMENU_NAME[] = { _T("현재가"), _T("종합차트") };
const		LPCTSTR		MENU_SCREEN_NOMAX[] = { _T("0101"), _T("0301") };
const		LPCTSTR		MENU_SCREEN_NOEMP[] = { _T("00310"), _T("00583") };
const		LPCTSTR		MENU_MAP_NO[] = { _T("hfis077aq"), _T("hfit031aq") };


typedef struct tagCodeInfo
{
	int   nRow;
	BOOL	bUser;
	int   nMarket;
	CString strCode;
	CString strName;
} CodeInfo;

typedef struct tagAnalystData
{
	int nType;

	BOOL	bUser;
	CString strCode;
} AnalystData;

// typedef struct tagShardData
// {
// 	BOOL bOpen;
// 	CString strMapNo;
// 	
// 	CUIntArray iaMarkType;
// 	CStringArray saCode;
// } ShardData;

typedef struct tagShardData
{
	BOOL bOpen;
	CString strScreenNo;
	CString strMapNo;
	
	CUIntArray iaMarkType;
	CStringArray saCode;
	CStringArray saName;
} ShardData;

typedef struct tagAnalystInfo
{
	//# Data
	CString strDate;
	CString strTime;

	double dCloseData_Kospi;
	double dCloseData_Kosdaq;

	CString strName[CODE_MAX_CNT];
	int nCloseData[CODE_MAX_CNT];

	//# Result
	double dParallel;
	double dAccumulate;
	int nUpCnt;
	int nSteadyCnt;
	int nDownCnt;

	double dParallel_Kospi;
	double dParallel_Kosdaq;
	double dParallel_Kospi_All;
	double dParallel_Kosdaq_All;
} AnalystInfo;

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

#define PF_EVENT_CALENDAR_SETTIME    30

#define PF_EVENT_BTN_CLICK			 7
#define PF_EVENT_BTN_SETWINDOWTEXT   8
#define PF_EVENT_BTN_SETTEXTCOLOR    9

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

#define ID_MENU_RESULTCTRL_START			11010
#define ID_MENU_RESULTCTRL_END				11300
const UINT			MENU_RESULTCTRL_INDEX[] = { 11010, 11020, 11040, 11050, 11050 };
const LPCTSTR		MENU_RESULTCTRL_NAME[] =		 {"관심등록", "돌려보기", _T(""), "EXCEL"};

const LPCTSTR		MENU_RESULTCTRL_SCREEN_NOMAX[] = {"관심등록", "돌려보기", _T(""), _T("")};
const BOOL			MENU_RESULTCTRL_SCREEN_OPEN[] = { TRUE, TRUE, TRUE, FALSE, FALSE};

#define SCREEN_NO_PORTMAX				MENU_RESULTCTRL_SCREEN_NOMAX[2]
#define SCREEN_NO_PORTEMP				MENU_RESULTCTRL_SCREEN_NOEMP[2]
#define SCREEN_NO_CHARTMAX				MENU_RESULTCTRL_SCREEN_NOMAX[1]
#define SCREEN_NO_CHARTEMP				MENU_RESULTCTRL_SCREEN_NOEMP[1]
#define SCREEN_NO_NEVIGATOR				MENU_RESULTCTRL_SCREEN_NOMAX[3]

#define SCREEN_NO_CHART				MENU_RESULTCTRL_SCREEN_NOMAX[0]
#define SCREEN_NO_ORDER				MENU_RESULTCTRL_SCREEN_NOMAX[1]
#define LINK_PORT					MENU_RESULTCTRL_SCREEN_NOMAX[2]

#define SCREEN_INDEX_EXCEL		MENU_RESULTCTRL_INDEX[3]

#define GETMARKET_ALL		1000
#define GETMARKET_KOSPI		0
#define GETMARKET_KOSPI200	100
#define	GETMARKET_KOSDAQ	200
#define GETMARKET_KRX		900

typedef struct  
{
	int xPoint;
	int yPoint;
	CString strCode;
} RButtonInfo;

#endif // CLIENT_TYPEDEF
