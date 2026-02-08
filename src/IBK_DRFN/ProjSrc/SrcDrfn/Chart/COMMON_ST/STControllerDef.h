
#ifndef __STDLLOPERATOR_DEF__BYGM0604
#define __STDLLOPERATOR_DEF__BYGM0604

#include <AfxTempl.h>
#include "RawDataDef.h"

#define UMSG_ST_SEND2SERVER					WM_USER+1	// Request query to server.
#define UMSG_ST_RECEIVEFROMSERVER			WM_USER+2	// Receive data from server
#define UMSG_ST_SAVE_ENV					WM_USER+3	// Save Environment info
#define UMSG_ST_REMOVE_ENV					WM_USER+4	// Remove Environment info

#define MEMCPYWITHNULL(szTarget,szSrc) {memcpy(szTarget,szSrc,sizeof(szSrc)); szTarget[sizeof(szSrc)] = NULL;}

#define TS_LANG		(0)	// First Index must be 0..
#define YES_LANG	(1)

#define NumericSeries	0
#define NumericSimple	1
#define TRUEFALSE		2

#define QUOTEINDEXLENGTH	(6)
#define FUTUREINDEXLENGTH	(8)
#define OPTIONINDEXLENGTH	(8)

#define VALEXITLONG		(0x00000001L)
#define VALEXITSHORT	(0x00000002L)
#define VALENTRYBUY		(0x00000004L)	
#define VALENTRYSELL	(0x00000008L)

#define MARKETENTRY		(0x00000001L)
#define MARKETCLEAR		(0x00000002L)

#define VALACTION		(0x00000010L)
#define FORCEDEXIT		(0x00000020L)
#define DEALATMARKET	(0x00000040L)
#define DEALATLIMIT		(0x00000080L)
#define DEALATSTOP		(0x00000100L)
#define DEALEXITDEFINED	(0x00000200L)
#define VALBUY			(VALEXITSHORT|VALENTRYBUY)
#define VALSELL			(VALEXITLONG|VALENTRYSELL)

#define VALSONJUL			(0x00000300L)
#define VALSOOIK			(0x00000400L)
#define VALGOGA				(0x00000500L)
#define VALJUGA				(0x00000600L)
#define VALMIN				(0x00000700L)
#define VALDANGIL			(0x00000800L)
#define VALTARGETPRESERV	(0x00000900L)
#define VALBENEFITPRESERV	(0x00000A00L)

/*
#define MAXCNTOFINPUTHOLDER		(50)
#define MAXCNTOFARRAYHOLDER		(10)
#define MAXCNTOFINPUTEXCHANGER	(10)/// Don't change!!
#define MAXCNTOFVARIABLEHOLDER	(50)
#define MAXCNTOFSUBFUNCHOLDER	(50)
#define MAXCNTOFINNFUNCHOLDER	(50)
#define MAXCNTOFMEMBUFFER		(10)
#define MAXCNTOFPLOTHOLDER		(13)// plot1 - plot10, (bull/bear), (deal)
*/

// 기존 위의 세팅에서 변경함. 
// 사유 : 삼성에서 가격이평지표를 최대20개까지 요구함.
// 현재 아래처럼 변경이 문제가 될지는 테스트를 더 해보야 함.
#define MAXCNTOFINPUTHOLDER		(70)
#define MAXCNTOFARRAYHOLDER		(20)
#define MAXCNTOFINPUTEXCHANGER	(20)/// Don't change!!
#define MAXCNTOFVARIABLEHOLDER	(70)
#define MAXCNTOFSUBFUNCHOLDER	(70)
#define MAXCNTOFINNFUNCHOLDER	(70)
#define MAXCNTOFMEMBUFFER		(20)
#define MAXCNTOFPLOTHOLDER		(23)// plot1 - plot10, (bull/bear), (deal)		// 10개 더 추가

#define INDEXTYPEDEFAULT		(0)
#define INDEXCOLORDEFAULT		RGB(0,0,0)
#define INDEXSHAPEDEFAULT		(0)
#define INDEXTHICKDEFAULT		(0)
#define INDEXBULLDEFAULT		RGB(250,220,220)
#define INDEXBEARDEFAULT		RGB(220,220,250)

#define SIGNALBUYDEFAULT			RGB(255,0,0)
#define SIGNALEXITSHORTDEFAULT		RGB(255,0,0)
#define SIGNALSELLDEFAULT			RGB(0,0,255)
#define SIGNALEXITLONGDEFAULT		RGB(0,0,255)

#define DISABLEDEFAULT			RGB(220,220,220)

// logic control..
#define ELEMENTINLOGIC		1
#define OPERATIONINLOGIC	2
#define PARENTHESISINLOGIC	3

#define ANDINLOGIC		0
#define ORINLOGIC		1
#define	NEXTINLOGIC		2

#define NOTINLOGIC		0
#define TRUEINLOGIC		1

#define OPENPARENTHESISINLOGIC		0
#define CLOSEPARENTHESISINLOGIC		1

#define STROPENPARENTHESISINLOGIC		"("
#define STRCLOSEPARENTHESISINLOGIC		")"
#define STRNOTINLOGIC					"!"

#define STRANDINLOGIC	"and"
#define STRORINLOGIC	"or"
#define	STRNEXTINLOGIC	"next"


#define MULTISELECTED				0x0001
#define INCLUDEPARENTHESIS			0x0002
#define NEXTISPOSSIBLE				0x0004
#define PARENTHESISISPOSSIBLE		0x0008
// logic control..

// 	// wizard & center
// 	#define SDSYSTEMFILE		"\\DATA\\STDATA\\STSYSTEM\\*.SD"
// 	#define SDSYSTEMFILE_D		"\\DATA\\STDATA\\STSYSTEMD\\*.SD"
// 
// 	#define SDUSERFILE			"\\DATA\\STDATA\\STUSER\\*.SD"
// 	#define SDUSERFILE_D		"\\DATA\\STDATA\\STUSERD\\*.SD"
// 
// 	#define STUSERFILE			"\\DATA\\STDATA\\STUSER\\*.ST"
// 	#define STUSERFILE_D		"\\DATA\\STDATA\\STUSERD\\*.ST"
// 
// 	#define STSYSTEM			"Data\\STDATA\\STSystem"
// 	#define STSYSTEM_D			"Data\\STDATA\\STSystemD"
// 
// 	#define STSD				"Data\\STDATA\\SD"
// 	#define STSD_D				"Data\\STDATA\\SDD"
// 
// 	#define STSTDIR				"Data\\STDATA\\ST"
// 
// 	#define STUSER				"Usr\\STDATA\\SD"
// 	#define STUSER_D			"Usr\\STDATA\\SDD"
// 
// 	#define STFOLDER_SD			"STDATA\\SD"
// 	#define STFOLDER_SDD		"STDATA\\SDD"
// 	#define STFOLDER_ST			"STDATA\\ST"
// 	#define STFOLDER_STSYSTEM	"STDATA\\STSystem"
// 	#define STFOLDER_STSYSTEMD	"STDATA\\STSystemD"

//-->@Solomon
// wizard & center
#define SDSYSTEMFILE		"\\Icss\\STDATA\\STSYSTEM\\*.SD"
#define SDSYSTEMFILE_D		"\\Icss\\STDATA\\STSYSTEMD\\*.SD"

#define SDUSERFILE			"\\Icss\\STDATA\\STUSER\\*.SD"
#define SDUSERFILE_D		"\\Icss\\STDATA\\STUSERD\\*.SD"

#define STUSERFILE			"\\Icss\\STDATA\\STUSER\\*.ST"
#define STUSERFILE_D		"\\Icss\\STDATA\\STUSERD\\*.ST"

#define STSYSTEM			"Icss\\STDATA\\STSystem"
#define STSYSTEM_D			"Icss\\STDATA\\STSystemD"

#define STSD				"Icss\\STDATA\\SD"
#define STSD_D				"Icss\\STDATA\\SDD"

#define STSTDIR				"Icss\\STDATA\\ST"

#define STUSER				"Usr\\STDATA\\SD"
#define STUSER_D			"Usr\\STDATA\\SDD"

#define STFOLDER_SD			"STDATA\\SD"
#define STFOLDER_SDD		"STDATA\\SDD"
#define STFOLDER_ST			"STDATA\\ST"
#define STFOLDER_STSYSTEM	"STDATA\\STSystem"
#define STFOLDER_STSYSTEMD	"STDATA\\STSystemD"
//<--@Solomon


#define SYSTEM_ST	(0)
#define USER_ST		(1)

#define TIMEINFO		"TimeInfo"
#define TIMEDEFAULT		"일"


// Time Type
#define TIME_TICK		'0'
#define TIME_MIN		'1'
#define TIME_DAY		'2'
//#define TIME_WEEK		'3'
//#define TIME_MONTH		'4'

#define TIME_STRTICK		"틱"
#define TIME_STRMIN			"분"
#define TIME_STRDAY			"일"
//#define TIME_STRWEEK		"주"
//#define TIME_STRMONTH		"월"

#define MAX				(3000)
typedef struct 	{
	BOOL bIsInRealTime;					// 실시간 정보인지 아닌지...
	CPtrArray *pPtrArraySDataItem;		// CSDataItem의 시간 데이터 Array Pointer
	CPtrArray *pPtrArrayHistoryDataItem;// CDataItem의 시고저종 데이터 Array Pointer
	CPtrArray *pPtrArrayPlot;			// 지표데이터 Array Pointer
	void *pDealDataItem;				// CDealData의 매매신호데이터 Array Pointer
} ST_DRAWINFO;

typedef struct : public ST_DRAWINFO 
{
	// 전략실행 결과정보
	CArray<double, double>*		pArDealPosition;
	CArray<CString, LPCTSTR>*	pArDealName;
	CArray<double, double>*		pArDealDrawPosition;
	CString STPath;						// 전략 Full경로 추가 : IndexAddIn에서 사용
} ST_DRAWINFO_EX;

typedef struct  {
	CString		m_strTRCode;
	void*		m_pBuffer;
	long		m_lLength;
	void*		m_pRequester;
}ST_SEND2SERVER;

typedef struct  {
	CString		m_strTRCode;
	void *		m_pBuffer;
	DWORD		m_dwLength;
}ST_RECEIVEFROMSERVER;

typedef struct
{
    char    sBuffer [12];           // 입력 버퍼                    
    char    shcode  [7];            // 종목코드                     
    char    date    [8];            // 검색 시작일                  
    char    num     [4];            // 검색할 데이터 갯수           
    char    tick    [2];            // Tick & Min Number                  
    char    button  [1];            // 틱(0)/분(1)/일(2)/주(3)/월(4)

    char    nflag   [1];            // 다음(1) / 조회(0) 구분자     
    char    nkey    [14];           // 다음 / 이전 Fetch Key        
    char    ImgRgb  [1];            // 0:허수봉 1:실봉              
	/// don't sent to server..
	char	chNull;
} TR10610_I;

typedef struct
{
    char    date    [8];            /* 날짜             */
    char    open    [10];           /* 시가             */
    char    high    [10];           /* 고가             */
    char    low     [10];           /* 저가             */
    char    close   [10];           /* 종가             */
    char    volume  [10];           /* 거래량           */
    char    value   [12];           /* 거래대금         */
									/*매수총잔량		*/
									/*매도총잔량		*/
	char	rights	[1];			/* 대당락, 권리락..	*/
									/*체결건수			*/

} TR10610_O_DATA;


typedef struct 
{
    char    sBuffer [12];           /* out buffer */
	char    tmp     [5];            /* Chart Count      */
   	char    Cnextkey[14];			/* 이전키           */
	TR10610_O_DATA chart  [MAX];	/* data */
}TR10610_O;


typedef struct 
{
    char    sBuffer		[12];
    char    sCode		[15];		/* 종목코드				*/
    char    sDate		[8];		/* 검색 시작일			*/
    char    sFetchCnt	[4];		/* 검색할 데이터 갯수	*/
    char    sTick		[2];		/* tick&Min Number		*/
    char    sBtnGb		[1];		/* 틱(0)/분(1)/일(2)	*/
    char    nFlag		[1];		/* 다음(1) / 조회(0) 구분자 	*/
    char    nKey		[14];		/* 다음 / 이전 Fetch Key		*/
	/// don't sent to server..
	char	chNull;
} TR10611_I;

typedef struct 
{
    char    date		[8];		/* 날짜             */
    char    open		[10];		/* 시가             */
    char    high		[10];		/* 고가             */
    char    low			[10];		/* 저가             */
    char    close		[10];		/* 종가             */
    char    volume		[10];		/* 거래량           */
    char    amount		[12];		/* 거래대금         */
	char	volume2		[12];		/* 미결제약정		*/
} TR10611_O_DATA;


typedef struct 
{
    char    sBuffer		[12];		/* Buffer           */
    char    Cchtcnt		[5];		/* Chart Count      */
    char    Cnextkey	[14];		/* 이전키           */
    TR10611_O_DATA      chart[MAX];

} TR10611_O;

typedef struct{
	long nStart;
	long nEnd;
	CString strStartDate;
	CString strEndDate;
	CString strGubun ;

	CString strInfo1;			//누적수익률, 승률 등 그래프에 나오는 Text 집어 넣기.
	CString strInfo2;
	CString STName;				//전략명 저장..
}ST_DATAINDEX;

#endif // __STDLLOPERATOR_DEF__BYGM0604
