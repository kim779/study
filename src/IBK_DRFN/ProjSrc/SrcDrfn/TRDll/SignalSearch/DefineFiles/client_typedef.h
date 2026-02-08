#ifndef CLIENT_TYPEDEF
#define CLIENT_TYPEDEF

#define WMU_SIGNAL_JM		(WM_USER + 656)	// 신호알리미 - 종목시그널

#define WMU_SGNLCTRL_REG			(WM_APP + 0x300)	// 신호컨트롤을 신호감시(DRFN)에 등록
#define WMU_SGNLCTRL_SETUP			(WM_APP + 0x302)	// 신호컨트롤 신호설정화면을 띄운다
#define WMU_SGNLCTRL_SIGNAL			(WM_APP + 0x303)	// 신호발생을 신호컨트롤에 통지

#define UMSG_CHART_SYMBOLDATA				WM_USER + 100
#define UMSG_CHART_GETDATETIME				WM_USER + 110
#define UMSG_CHART_SETWINDOWDATA			WM_USER + 120
#define UMSG_CHART_SETZOOMVALUE				WM_USER + 130

#define WM_NOTIFY_GRIDEVENT		WM_USER + 340
#define NM_CLICK_L				1
#define NM_CLICK_R				2
#define NM_CLICK_CELLTYPE		3
#define NM_DBCLICK_L			4

#define FOLDER_IMGDATA				"\\icss\\Image\\drchart"

#define FINDERINFO_INIFILE			"FindInfo.ini"
#define FOLDER_FINDDATA				"\\FindData"

#define PROGRAM_TITLE		"신호검색"

#define FIXFILED_USER_ONE_ITEM		"fixfield_User_One_Item.xml"
#define FIXFILED_USER_ONE_MAKRET	"fixfield_User_One_Market.xml"
#define FIXFILED_USER_ONE_REAL		"fixfield_User_One_Real.xml"

#define	WM_BC_DATA_RECEIVED				WM_USER + 703

#define _countof(array) (sizeof(array)/sizeof(array[0]))

#define STR_CONVERT(pBuff, cData, nSize, strData)		\
  pBuff = new char[nSize + 1];							\
  memset(pBuff, 0x00, nSize + 1);						\
  memcpy(pBuff, cData, nSize);							\
  strData = (CString)pBuff;								\
  strData.TrimRight();									\
delete [] pBuff;

#define INT_CONVERT(pBuff, cData, nSize, nData)			\
  pBuff = new char[nSize + 1];							\
  memset(pBuff, 0x00, nSize + 1);						\
  memcpy(pBuff, cData, nSize);							\
  nData = atoi(pBuff);									\
delete [] pBuff;

const UINT RMSG_GETPARTNER_MNG = ::RegisterWindowMessage(_T("RMSG_GETPARTNER_MNG"));
const UINT RMSG_RECEIVEDIRECTFID = ::RegisterWindowMessage(_T("RMSG_RECEIVEDIRECTFID"));

#define ID_MENU_START			10010
#define ID_MENU_END				10200		//# 10190, 10200 : Naver Serch

const UINT		RBTNMENU_INDEX[]			= { 10010, 10020 };
const LPCTSTR	RBTNMENU_NAME[]				= { _T("현재가"), _T("종합차트") };
const LPCTSTR	RBTNMENU_SCREEN_NOMAX[]		= { _T("2001"), _T("4501") };
const LPCTSTR	RBTNMENU_SCREEN_NOEMP[]		= { _T("00310"), _T("00583") };
const LPCTSTR	RBTNMENU_MAP_NO[]			= { _T("hfis077aq"), _T("hfit031aq") };
const LPCTSTR	MATCHVAL_OPT_LIST[]			= { ">", ">=", "=", "<=", "<" };
const LPCTSTR	MATCHVAL_OPT_DATA			= ">\n>=\n=\n<=\n<\n";
const LPCSTR	FIELDEDIT_TAB_STR[]			= {_T("시세"), _T("호가"), _T("시고저가"), _T("최고/최저"), _T("투자자"), _T("예상"), _T("기타")};
const LPCTSTR	SEL_PERIOD[]				= { _T("1"), _T("2"), _T("3"), _T("4"), _T("5"),
												_T("10"), _T("15"), _T("20"), _T("30"), _T("60"), _T("120")};

#define QUERY_STRJONGMOK_MIN		_T("HKST03010200")

typedef struct tagSignalData
{
  int nRealType;
  CString strType;

  int nMarket;
  CString strCode;
  CString strName;
  CString strSqNo;

  CString strProfferID;     //유진추가
  CString strSubProfferID;  //유진추가

  CString strDate;
  CString strTime;

  CString strTitle;
  int nPoint;
  COLORREF clrTitle;
} SignalData;




typedef struct tagSignalNowPrice
{
	CString strJobFlag;   //신규, 탈퇴 여부
	CString strPrice;		//현재가
	CString strCode;		//종목코드
	CString strName;		//종목명
	CString strTime;		//신호시간
	CString strTitle;		//신호명
} STSIGNAL_NOWPRICE;


typedef struct	tagSIGNALMSGINFO
{
	TCHAR	m_sgType[2+1];		//  - 신호구분	: 신호 상세분류
	TCHAR	m_sgTime[6+1];		//  - 발생시간	: HH:MM:SS
	TCHAR	m_sgnskey[26+1];		//  - /* 날짜[8]+시간[6]+일련번호[8]+뉴스구분[2]+ 뉴스서브구분[2]
	TCHAR	m_sgSeqno[8+1];		//  - 일련번호
	TCHAR	m_sgCodeGb[1+1];		//  - 1:코스피,2:코스닥,3:프리보드,4:ELW
	TCHAR	m_sgCode[8+1];		//  - 종목코드	: 주식(1), 선물(2)/옵션(3), 업종지수(4)
	TCHAR	m_sgContext[512+1];	//  - 신호내용	: 뉴스타이틀 / 조건내용등
	TCHAR	m_sgGubun[1+1];		//  - 매도수 구분	: 일반(1) / 매수(2) / 매도(3)
	TCHAR	m_sgBlink[2+1];		//  - 신호속성	: 깜빡임 횟수
	TCHAR	m_sgContinue[1+1];	//  - 신호속성	: 유지여부
	TCHAR	m_sgPriority[2+1];	//  - 신호속성	: 우선순위 
	TCHAR	m_sgIcon[2+1];		//  - 신호속성	: 표시Icon Index
	TCHAR	m_sgBugaInfo[10+1];	//  - 부가정보	: DRFN에서 보내는 신호중 신호강도정보
} EUGENE_SIGNALMSGINFO;

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

// 신호컨트롤의 신호발생시 통보되는 신호내용 관리 Structure
typedef struct tagSCSignal
{
	CString		strCode;				// 종목코드
	CString		strName;				// 종목명
	
	CString		strSgnlTime;			// 신호발생 시간
	CString		strSgnlCode;			// 신호종류
	CString		strSgnlTitle;			// 신호명
	COLORREF	strSgnlText;			// 신호내용에 적용할 폰트컬러
	
} SCSIGNAL, *LPSCSIGNAL;

#define MAX_FIELD_CNT					17		//# 결과 Grid Column 구성 Max Count

#define CANDLE_INDEX_FILE				"candleinfo.xml"
#define PATEN_INDEX_FILE				"patterninfo.xml"
#define GRID_MOVEFIELD_POS_NAME			"[값보기 위치]"

#define XML_VERSION_FIXFIELDFILE		"1.0"
#define FOLDER_DATA						"\\icss\\finddata"
#define FIXFIELD_SYS_FILE				"fixfield_sys.xml"
#define FIXFIELD_USER_SIGNAL_FILE		"fixfield_User_Signal.xml"/*"fixfield_User_section.xml"*/
#define FOLDER_IMAGE					"\\icss\\Image" 

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

typedef struct tagMatchInfo
{
	int			nRow;
	BOOL		bUser;
	int			nOptIndex;
	CString		strField;
	CString		strElement;
	int 		nFieldIndex;

	int			nGridSourceRow;			//# Temp
} MatchInfo;

typedef struct tagGridEventInfo
{
	CString strGridType;

	int nCol;
	int nRow;
	CString strType; //#=> BUTTON / CHECK / COMBO
	CString strData;
} GridEventInfo;

typedef struct tagCandleInfo{
	long    lID;
	CString   strName;
} CandleInfo;

typedef struct tagPatenInfo{
	long    lID;
	CString   strName;
} PatenInfo;

// #define WMU_SGNLCTRL_REG			(WM_APP + 0x300)
// #define WMU_SGNLCTRL_SETUP			(WM_APP + 0x302)	// 신호컨트롤 신호설정화면을 띄운다
// #define WMU_SGNLCTRL_SIGNAL			(WM_APP + 0x303)	// 신호발생을 신호컨트롤에 통지

#define WMU_SIGNAL_CONDSEARCH	(WM_USER + 0x731)
typedef struct _tag_Alarm_Condsearch_Info
{
	TCHAR	szScrNo[4+1];			// - 화면번호
	TCHAR	szCode[12+1];			// - 종목코드
	TCHAR	szMemegb[1+1];		// - 매매구분(0:구분없음 1:매도 2:매수)
} ALARM_CONDSEARCH_INFO, *LPALARM_CONDSEARCH_INFO;

#define gszCacheDir "DrCache"
#define gszDataKey_MstCode "MstCode"
#define gszDataKey_WorkDate "WorkDate"
#define usrMasterCondition "SearchTemp"
#define USRDIR "\\user\\"
#define FILE_GROUP "\\portfolio.ini"

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
