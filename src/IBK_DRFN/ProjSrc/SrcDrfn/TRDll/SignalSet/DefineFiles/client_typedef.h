#ifndef CLIENT_TYPEDEF
#define CLIENT_TYPEDEF

#define _countof(array) (sizeof(array)/sizeof(array[0]))

#define PROGRAM_TITLE				"신호검색설정"

#define FOLDER_DATA							"\\icss\\finddata"
#define FOLDER_NEWS							"\\NewsData"
#define FOLDER_FIND							"\\FindData"
#define NEWS_POSTURE_FILE				"\\News_case_stmt.dat"

#define NEWS_POSTUREUSER_FILE		"\\News_UserPosture.dat"

//#define FOLDER_MASTER						"\\Master"
#define FOLDER_MASTER						""
#define NEWS_SYSLIST_FILE				"\\signal_syslist.dat"

#define gszCacheDir							"DrCache"
#define gszDataKey_MstCode					"MstCode"
#define gszDataKey_WorkDate					"WorkDate"
#define usrMasterCondition					"SearchTemp"
#define USRDIR								"\\user\\"
#define FILE_GROUP							"\\portfolio.ini"

typedef struct tagShardData
{
	BOOL bOpen;
	CString strMapNo;
	
	CUIntArray iaMarkType;
	CStringArray saCode;
} ShardData;

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


typedef struct 
{
	CString szCtrlName;
	CWnd*   pWnd;
	int 	nType;
	BOOL	bEnable;
} ST_MOVEITEM;
#define SET_ST_MOVEITEM(xST, xName, xpWnd) xST.szCtrlName=xName; xST.pWnd=xpWnd; xST.nType=0; xST.bEnable=1;
#define PF_ITEM_BTN		    0
#define PF_ITEM_RADIOBTN    1
#define PF_ITEM_CHECKBTN    2
#define PF_ITEM_COMBOBOX    3

#define PF_EVENT_COMBO_ADDLIST		 1
#define PF_EVENT_COMBO_ADDSTRING	 2
#define PF_EVENT_COMBO_GETWINDOWTEXT 3
#define PF_EVENT_COMBO_SETCURSEL     4
#define PF_EVENT_COMBO_GETCURSEL     5
#define PF_EVENT_COMBO_RESETCONTENTS 6

#define PF_EVENT_CHK_SETCHECK		20


#define PF_EVENT_SHOWWINDOW			 10

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

typedef struct tagMSTCode
{
	char    sMarket[1];
	char    sCode[7];
	char    sName[20];
	char    sSetPos[4];
	char    sUsjg[1];               /* 우선주   (0:해당없음,1:해당) */
	char    sUnder[1];              /* 관리종목                     */
	char    sStop[1];               /* 거래정지종목                 */
	char    sDishonest[1];          /* 불성실공시종목               */
	char    sConfidencelimit[1];    /* 신용제한종목?               */
	char    sJrmm[1];          	    /* 정리매매종목                 */
	char    sDrat[3];               /* 증거금비율                   */
	char    sMrktalrm[1];           /* 시장경보
										1:투자주의, 2:투자경고
										3:투자위험, 4:투자위험예고*/
	char	sMonth[2];				/* 결산월 */
} MSTCode;

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

#define FOLDER_FINDDATA				"\\FindData"

//관심조회관련 tr
struct _uinfo {
	char gubn[2]; // 업무구분
	   // "MY" : 관심종목
	   // "UL" : 사용자 데이터 목록 
	   // "UD" : 사용자 데이터
	char dirt[1]; // 전송방향 'U' : pc==>host, 'D' : host==>pc, 'X' : host==>pc (conversion), 'R' : 재조회
	char cont[1]; // 연속구분 'F':First, 'M':Middle, 'L':First&Last, 'C':Cancel, 
		 // 'S':그룹편집, 'E':그룹조합처리, 'b':DB백업, 'R':DB원복, 'V':최종상태저장, 'r':최종상태복원
		 // 'D':그룹삭제, 'j':해당그룹종목전송, 'g':그룹리스트조회, 'G':그룹종목전송, 'C':전송취소
	char name[80]; // file name
	char nblc[5]; // 블럭개수, 파일크기(max:64k)
	char retc[1]; // return value 'O':정상 'E':에러 'U'신규 정상
	char emsg[40]; // error message
};

struct _ginfo {
	char gnox[2]; // group number
	char gnam[20]; // group name
	char jrec[4]; // _jinfo count
};

struct _jinfo {
	char gubn[1]; // 종목구분 0:none, 1:현물, 2:선물, 3:옵션, 4:개별주식옵션, 5:지수
	char code[12]; // 종목코드
	char xprc[10]; // 보유단가
	char xnum[10]; // 보유수량
};

struct _updn {
	_uinfo uinfo;
	_ginfo ginfo;
};

const int sz_uinfo = sizeof(struct _uinfo);
const int sz_ginfo = sizeof(struct _ginfo);
const int sz_jinfo = sizeof(struct _jinfo);

#endif // CLIENT_TYPEDEF
