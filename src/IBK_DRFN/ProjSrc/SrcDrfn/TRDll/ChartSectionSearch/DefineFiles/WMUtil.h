// WMUtil.h: interface for the CWMUtil class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WMUTIL_H__61A0968A_BCC0_4B3C_96F6_9C7AC762ED28__INCLUDED_)
#define AFX_WMUTIL_H__61A0968A_BCC0_4B3C_96F6_9C7AC762ED28__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CWMUtil  
{
public:
	CWMUtil();
	virtual ~CWMUtil();

};

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2002.07.29
//
//	목적	:	종목 Group의 Type을 관리한다.
//
//	내용	:	기본적으로 관심과 그외의 당일 특이 종목에 대한 Group으로 분류된다.
//				특히 Tree Combo의 Item에 Data를 저장하기 위하여 상위 31번째 Bit로
//					Flag이 되도록 한다.
//				따라서, TYPE_INTEREST Bit-Flag가 설정되지 않은 Item은 당일 특이 종목 Group이다. 
///////////////////////////////////////////////////////////////////////////////
#define TYPE_INTEREST	0x40000000
#define TYPE_BOYOU		0x20000000
#define TYPE_MASTER		0x10000000			// 전체/거래소/코스닥 종목

//WMLGT_NEW 731 START
#define TYPE_OTHER		0x40000001

#define FETCHITEM_COUNT  50
#define FETCHITEM_ALL	 0
//WMLGT_NEW 731 END

#define GUBUN_KOSPI		1
#define GUBUN_KOSDAQ	2
#define GUBUN_FUTURE	3
//#define GUBUN_ROPTION	4
//#define GUBUN_THIRD		5
#define GUBUN_SOPTION	4

#define GUBUN_KOSPISTR   "코드-종목"
#define GUBUN_KOSDAQSTR  "코드-종목"
#define GUBUN_FUTURESTR  "코드-선물"
#define GUBUN_ROPTIONSTR "코드-옵션"
#define GUBUN_THIRDSTR   "코드-제3시장"
#define GUBUN_SOPTIONSTR "코드-선물옵션"
#define GUBUN_ELW        "코드-ELW"

#define MAXCONNECT 6
#define _MAXGROUP  100


typedef struct {
	CString DataName;
	CString FieldName;
	CString KeyName;
	UINT	nID;
	BOOL	bAdvise;
} STDRDSINFO;


typedef struct _S11050_I {
    char jmcode[12];     /* 종목코드 */
} S11050_I;

/////////////////////////////////////////WMLGT_NEW
typedef struct S07010_T
{
    char    part        [ 1];       /* 시장구분 */
    char    code        [12];       /* 종목코드 */
    char    price		[ 9];       /* 현재가   */
	char    sign		[ 1];       /* 등락 부호 */ 
} S07010_T;

typedef struct {
	char	Under		[  1];				/* 관/감리구분					*/
	char	Codegb		[  1];				/* 코드구분(공유를 위해)		*/
	char	code		[ 15];				/* 종목코드						*/
	char	Name        [ 30];				/* 종목명						*/
	char	Price		[  9];				/* 현재가						*/
	char	Change		[  6];				/* 전일대비(등락폭)				*/
	char	Rate		[  5];				/* 등락율(%)					*/
	char	Sign		[  1];				/* 등락부호						*/
	char	Volume		[ 12];				/* 누적거래량					*/
	char	Offerho		[  9];				/* 매도호가						*/
	char	Bidho		[  9];				/* 매수호가						*/
	char	Ofremain	[ 12];				/* 매도 최우선잔량				*/
	char	Bidremain	[ 12];				/* 매수 최우선잔량				*/
	char	Offertot	[ 12];				/* 총매도잔량					*/
	char	Bidtot		[ 12];				/* 총매수잔량					*/
	char	Open		[  9];				/* 시가							*/
	char	High		[  9];				/* 고가							*/
	char	Low			[  9];				/* 저가							*/
	char	Value		[ 14];				/* 거래 대금	                */ 
	char	UpLmtPrice	[  9];				/* 상한가						*/
	char	DnLmtPrice	[  9];				/* 하한가						*/
	char	Openyak		[  9];				/* 미결제약정수량				*/
	char	FacePrice	[  9];				/* 액면가       				*/
	char	YDSign		[  1];				/* 전일 등락부호				*/
	char	YSGubun		[  1];				/* 연속 구분					*/
	char	YSCount		[  2];				/* 연속 일수					*/
	char	Recprice	[  9];				/* 기준가						*/

	char    cvolume     [ 9];				/* 체결량           */
    char    open_chrate [ 5];				/* 시가전일대비율   */
    char    high_chrate [ 5];				/* 고가전일대비율   */
    char    low_chrate  [ 5];				/* 저가전일대비율   */
	char    high_time   [6];				/* 고가 시간*/
	char    low_time    [6];                /* 저가 시간*/
// 예상체결 추가 - cst 2004/12/09
    char    expsign         [ 1];			// 예상체결 부호                        
    char    expprice        [ 9];			// 예상체결 가                          
    char    expchang        [ 9];			// 예상체결 대비                        
    char    expchgrt        [ 9];			// 예상체결 대비율                      
    char    expvol          [12];			// 예상체결 거래량 
	char    color           [ 1];			/* 1:Blue 2:Yellow 3:Green 4:Red        */
	
	char    lendgb          [ 1];			/* 0:신용불가 1:신용가능                */
    char    lendrate        [ 3];			/* 대출적용비율 50,60                   */	
//	char	Name		[ 20];				/* 종목명 						*/
//	char	pos			[  2];				/* 순서                         */   
//	char	Momentvol	[ 10];				/* 체결량						*/
//	char	Rights		[  2];				/* 액면분할여부 : 4*=액면분할	*/
//	char	HSign		[  1];				/* 상한가부호(선물,옵션,주옵만)	*/
//	char	LSign		[  1];				/* 하한가부호(선물,옵션,주옵만)	*/
//	char	NewsGb		[  2];				/* 뉴스 유무 bit setting		*/
//	char	Gsiyn		[  1];				/* 1개월 공시 유무				*/
} ST_INQUIRYDATA; //

enum DATA_TYPE
{
	DT_SIGNAL = 0, 			// 0 : 신호처리 - (이전:	DT_SHOWSIGN = 0,	// 0 기타)
	DT_MANAGER,				// 1 : 	관/감리구분
	DT_CODEGB ,				// 2 :  코드 구분
	DT_CODE,				// 3 :  종목코드 
	DT_NAME,				// 4 :  종목명
	DT_PRICE,				// 5 :  현재가
	DT_CHANGE,				// 6 :  전일대비(등락폭)
	DT_RATE,				// 7 :  등락율
	DT_SIGN,				// 8 :  등락부호
	DT_VOLUME,				// 9 : 누적거래량
	DT_OFFERHO,				// 10 : 매도호가
	DT_BIDHO,				// 11 : 매수호가
	DT_OFREMAIN,			// 12 : 매도 최우선잔량
	DT_BIDREMAIN,			// 13 : 매수 최우선 잔량	
	DT_OFFERTOT,			// 14 : 총매도 잔량
	DT_BIDTOT,				// 15 : 총매수 잔량
	DT_OPEN,				// 16 : 시가 
	DT_HIGH,				// 17 : 고가
	DT_LOW,				    // 18 : 저가
	DT_VALUE,				// 19 : 거래대금
	DT_UPLMTPRICE,			// 20 : 상한가	
	DT_DNLMTPRICE,		    // 21 : 하한가
	DT1_OPENYAK,			// 22 : 미결제 약정수량
	DT_FACEPRICE,			// 23 : 액면가
	DT_YDSIGN,				// 24 : 전일 등락부호
	DT_YSGUBUN,				// 25 : 연속 구분
	DT_YSCOUNT,			    // 26 : 연속 일수
	DT_RECPRICE,			// 27 : 기준가
	DT_CVOLUME,				// 28 : 체결량
	DT_OPEN_CHRATE,			// 29 : 시가 전일 대비율
	DT_HIGH_CHRATE,			// 30 : 고가 전일 대비율
	DT_LOW_CHRATE,			// 31 : 저가 전일 대비율
//////////////////////////////////////////////////////////////
	DT_BUYVALUE,			// 32 : 매입단가
	DT_BUYVOLUME,			// 33 : 매입수량
	DT_ADDVALUE,			// 34 : 수익율
	DT_HOWMUCH,				// 35 : 손익금액
	DT_OFFERTOTHOW,			// 36 : 총매도잔량증감
	DT_BIDTOTHOW,		    // 37 : 총매수잔량증감
	DT_OFREMAINHOW,			// 38 : 매도 최우선잔량증감
	DT_BIDREMAINHOW,		// 39 : 매수 최우선잔량증감	
	DT_CANDLE,				// 40 : CANDLE
	DT_OPEN_CHANGE,			// 41 : 시가 전일 대비
	DT_HIGH_CHANGE,			// 42 : 고가 전일 대비
	DT_LOW_CHANGE,			// 43 : 저가 전일 대비
	DT_LOWTIME,				// 44 : 저가시간
	DT_OFREMAINDEF,			// 45 : 잔량차이
	DT_OFFERTOTDEF,			// 46 : 총잔량 차이
	DT_HIGTROWDEF,			// 47 : 고가저가 차이
	DT_HIGHTIME,			// 48 : 고가시간
	DT_EXSIGN,				// 49 : 예상체결부호 
	DT_EXPRICE,				// 50 : 예상체결가
	DT_EXCHANGE,			// 51 : 예상체결 대비
	DT_EXCHANGERATE,		// 52 : 예상체결 대비율
	DT_EXVOL,				// 53 : 예상체결 거래량
	DT_REALSIGNAL,			// 54 : 신호등
	DT_LENDGB,				// 55 : 신용가능여부
	DT_LENDRATE				// 56 : 신용대출비
};

enum IFYPE
{
	IF_CHANGE = 0,
	IF_PRICE,
	IF_VOLUME,
	IF_RATE,
	IF_OFFERHO,				// 10 : 매도호가
	IF_BIDHO,				// 11 : 매수호가
	IF_OFREMAIN,			// 12 : 매도 최우선잔량
	IF_BIDREMAIN,			// 13 : 매수 최우선 잔량	
	IF_OFFERTOT,			// 14 : 총매도 잔량
	IF_BIDTOT,				// 15 : 총매수 잔량
	IF_ADD,					// 17 : 수익율	
	IF_OPEN,				// 16 : 시가 
	IF_OPEN_CHRATE,			// 29 : 시가 전일 대비율
	IF_HIGH,				// 17 : 고가
	IF_HIGH_CHRATE,			// 30 : 고가 전일 대비율
	IF_LOW,				    // 18 : 저가
	IF_LOW_CHRATE,			// 31 : 저가 전일 대비율
};


enum COLOR_INDEX
{
	COLOR_ROW1=0,
	COLOR_ROW2,
	COLOR_REAL,
	COLOR_FOCUS,
	COLOR_KOSPI,
	COLOR_KODOQ,
	COLOR_FUTURE,
	COLOR_OPTION,
	COLOR_HIGH,
	COLOR_EQUAL,
	COLOR_LOW,
	COLOR_GRIDTITLE,
	COLOR_DTNAME
};
#define	 CNTMAX   2000 
#define  SH_KSIZE  6
typedef struct
{
	char	gubun[2];         /* '01'상한 '02'상승 '03'보합 '04'하한 '05'하락 '06'신고 '07'신저*/
} TR_05021_I;

typedef struct _TR_05021_S{
	char    bit[1];           /* '1'장내(KOSPI) '2'장외(KOSDOQ) */
	char	code[SH_KSIZE];          /* 종목 코드 */
	char	hname[20];        /* 종목명 */
} TR_05021_S;

typedef struct _TR_05030_S
{
    char    time        [ 6];   /* System 현재시간  */
    char    status      [ 6];   /* 장운용구분       */
    char    statContent [40];   /* 장구분설명       */
} TR_05030_S;

typedef struct
{
	char        pWnd[4];
	char        ogubun[2];    /* 구분 */
	char        totcnt[4];    /* total Count */
	TR_05021_S  data[1];
} TR_05021_O;

#define KOSPI_RATE		0.15
#define KOSDAQ_RATE		0.15

#define TR_INTEREST  "05010"
#define TR_INTETC	 "05020"
#define TR_INTETC2	 "05021"
#define TR_EXPCHECK	 "05030"
#define TR_INTLOOK	 "99999"
//#define TR_INTEREST "07010"
////////////////////////////////////////WMLGT_NEW 731 START
#define TYPE_ALL			 "0"		//코스닥, 코스피 모두 
#define TYPE_KOSPI			 "1"
#define TYPE_KOSDAQ			 "2"

#define GWAN_MAX_GROUP_COUNT	50
#define GWAN_MAX_GRID_COUNT		3
#define GWAN_GROUPMNG_VERSION	1
#define GWAN_GROUPMNG_NEWVERSION	2

#define GWANGROUP_USER			0		// 사용자 그룹은 0~49번 까지이지만 확장을 대비하여 0~999번 까지
#define GWANGROUP_USER_END		999		// 사용할 수 있도록 트리에서 선택되는 그룹번호를 1000번으로 한다.
#define GWANGROUP_TREE			1000
#define GWANGROUP_ZZIM			1001
#define	GWANGROUP_THEME			1002

#define FIELD_PRICE			 "price"
#define FIELD_SIGN			 "sign"
////////////////////////////////////////WMLGT_NEW 731 END
//#define WM_DBLCLICK_DATA			WM_USER + 1599
//#define WM_GRILCLICK_DATA			WM_USER + 1598


///////////////////////////////////////////////////////
// GRID COLOR INFORMATION START
#define GRID_HEADER_FORCOLOR  RGB(255,255,255)
#define GRID_HEADER_BACKCOLOR RGB(0,0,0)
#define GRID_SELECT_COLOR	  RGB(0,128,128)

#define GRID_STREAM_HIGHCOLOR   RGB(255, 0, 0)
#define GRID_STREAM_LOWCOLOR	RGB(0,255,0)

#define GRID_DATA_HIGHCOLOR   RGB(222, 22, 22)
#define GRID_DATA_LOWCOLOR	  RGB(22, 22, 222)
#define GRID_DATA_EQUALCOLOR  RGB(0, 0, 0)
#define GRID_DATA_IFCOLOR1    RGB(222, 22, 22)
#define GRID_DATA_IFCOLOR2    RGB(222, 128, 22)
#define GRID_DATA_IFCOLOR3    RGB(222, 192, 22)
//#define GRID_DATA_IFCOLOR4    RGB(222, 255, 22)
#define GRID_DATA_IFCOLOR4    RGB(110, 160, 220)

#define STREAM_FORECOLOR RGB(198, 223, 198)
#define STREAM_BACKCOLOR RGB(0,0,0)
#define STREAM_HFORECOLOR RGB(255,255,255)
#define STREAM_HBACKCOLOR RGB(100,100,100)
#define STREAM_HEADERFORECOLOR RGB(255,168,0)

#define GRID_DATA_BACKCOLOR   RGB(255,255,255)

#define GRID_TITLE_BACKCOLOR  RGB( 255, 255, 255)
#define GRID_TITLE_FORECOLOR  RGB( 0, 0, 0)

#define TYPE_DEFAULT 0

#define STATE_IF1   1
#define STATE_IF2   2
#define STATE_IF3   4
#define STATE_IF4   8
#define STATE_IFEND		 2147483648l

#define DEF_COLOR_ROW1   0
#define DEF_COLOR_ROW2   1
#define DEF_COLOR_REAL   2
#define DEF_COLOR_FOCUS  3
#define DEF_COLOR_KOSPI  4
#define DEF_COLOR_KOSDOQ 5
#define DEF_COLOR_OPTION 6
#define DEF_COLOR_FUTURE 7
#define DEF_COLORB_HIGH   8
#define DEF_COLORB_EQUAL  9
#define DEF_COLORB_LOW    10
#define DEF_COLORB_TITLE  11
#define DEF_COLORB_ITEMTITLE  12
#define DEF_COLORB_BORDERLINE 13

#define MAX_ROW_CNT 25

typedef struct _MKT_BOARD_IN
{
     char mastgb[1];            /* 장구분 1:코스피, 2:코스닥, 3:둘다 */
     char signgb[5];            /* 상한, 상승, 보합, 하한, 하락 순으로 체크인 경우 해당배열 1, 아니경우 0 */
     char condition[1];         /* 조건구분 1:and, 2:or */
     char volume[10];            /* 거래량 */
     char value[10];               /* 거래대금(만단위) */  
	 char price[10];
} MKT_BOARD_IN;


typedef struct _MKT_BOARD_GRID
{
	 char Codegb[1];
     char Sign[1];                /* 등락부호 */
     char shcode[6];           /* 종목코드 */
     char hname[20];          /* 종목명(한글) */
     char Price[9];             /* 현재가 */
     char Change[9];         /* 등락폭 */
     char volume[10];          /* 체결량 */       
} MKT_BOARD_GRID;

typedef struct _MKT_BOARD_OUT
{
	 char pWnd[4];
     char cnt[4];
     MKT_BOARD_GRID grid[1];//MAX_GRID_CNT]; 
} MKT_BOARD_OUT;

typedef struct {
	char    pWnd[4];    
	char	Count[4];
	char	Data1[1];
} SSP_GETGCODE;

typedef struct
{
//	char Iwho[2]; // 0 : 관심, 1 : 메인, 2 : 조건, 3 : 신호, 4 : 차트
	char Igubun[2]; // 0 : ERROR, 1 : 1전체 정보(종목리스트 포함), 2 : 조건 그룹 등록, 3 : 종목 등록    
	char Icnt[3]; // 그룹 수
	char IopenDlg[1]; // 0 : Open, 1 : No Open
	void* Idata;  // 그룹 데이터들(CPtrArray) 
	char Ititle[256];
}INT_TOTAL;

typedef struct
{
	char Ino[2];		// 그룹 번호 
	char Ititle[20];	// 그룹 명
	char Icnt[3];		// 그룹 안에 종목 수
	void* Idata;		//종목 데이터들(CPtrArray)
	char Itime[6];
	char Idate[8];
}INT_GROUP_ST;

typedef struct
{
	char Icode[12];  // 코드  
	char Igubun[2];		   // 0:Kospi, 1:kosdoq,,,
	char Itime[6];		   // HHMMSS   시,분,초
	char Idate[8];		   // YYYYMMDD 년,월,일
	char Ivol[10];		   // 수량 	
	char Ival[10];		   // 편입가격
	char ICheck[1];		   // 체크유무
	void* Idata;
	char IName[30];
} INT_ITEM_ST;

// RMSG_INTMESSAGE 의 LPARAM에 아래의 구조체 포인터가 들어간다.
// WPARAM에는 메시지의 세부 구분값이 들어간다.
typedef struct
{
	HWND hWnd;				// 메시지를 날린 윈도우 핸들
	INT_TOTAL * pIntTotal;
	LONG lData;
} INT_MSG_ST;

typedef struct 
{
	HWND hWnd;
	char szCode[15];
	char szMemo[512];
	LOGFONT lf;
} INT_MEMO_ST;

#define FILEPATH_USER	"GwanNew.usr"
#define FILEPATH_DEF	"\\Data\\Gwansim\\Gwan.cfg"
#define FILEPATH_BLINKDEF  "\\Data\\Gwansim\\Gwan_b.cfg"
#define FILEPATH_BLINKUSER  "Gwan_b.usr"
#define FILEPATH_LOOKUSER  "Gwan_L.usr"
#define FILEPATH_BLINKUSER30  "Gwan_b30.usr"
#define FILEPATH_LOOKUSERIF  "Gwan_LIF.usr"
#define FILEPATH_LOOKDEF  "\\Data\\Gwansim\\Gwan_L.cfg"

#include "../Common/LocalUtil.h"
#include <AFXTEMPL.H>
typedef CList<CJMaster,CJMaster> TDEFCODEJ;
typedef CList<CKMaster,CKMaster> TDEFCODEK;
typedef CList<CKUMaster,CKUMaster> TDEFCODEKU;
typedef CList<CGMaster,CGMaster> TDEFCODEG;
typedef CList<CUMaster,CUMaster> TDEFCODEU;
typedef CList<CFMaster,CFMaster> TDEFCODEF;
typedef CList<CPMaster,CPMaster> TDEFCODEP;
typedef CList<CUPMaster,CUPMaster> TDEFCODEUP200;
typedef CList<CUDMaster,CUDMaster> TDEFCODEUD50;
typedef CList<CTHMaster*,CTHMaster*> TDEFCODETH;


const UINT RMSM_RESETGRID = ::RegisterWindowMessage("RMSM_RESETGRID");
const UINT RMSG_GRIDLCLICK = ::RegisterWindowMessage("RMSG_GRIDLCLICK");
const UINT RMSG_GRIDRCLICK = ::RegisterWindowMessage("RMSG_GRIDRCLICK");
const UINT RMSG_GRIDDLCLICK = ::RegisterWindowMessage("RMSG_GRIDDLCLICK");
const UINT RMSG_GRIDHLCLICK = ::RegisterWindowMessage("RMSG_GRIDHLCLICK");
const UINT RMSG_GRIDHRCLICK = ::RegisterWindowMessage("RMSG_GRIDHRCLICK");
const UINT RMSG_GRIDCELLCHANGE = ::RegisterWindowMessage("RMSG_GRIDCELLCHANGE");
const UINT RMSG_GRIDLMOVELINE = ::RegisterWindowMessage(_T("RMSG_GRIDLMOVELINE"));

//2004.12.07 by LYH - Grid 팝업메뉴
const UINT RMSG_MENUPOPUP = ::RegisterWindowMessage(_T("RMSG_MENUPOPUP"));
#define BASE_MAINCONTEXTMENU			30000

const UINT RMSG_SHOWITEMINFO = ::RegisterWindowMessage("RMSG_SHOWITEMINFO");
const UINT RMSG_OPENSIGNAL = ::RegisterWindowMessage("RMSG_OPENSIGNAL");

const UINT RMSG_INTSETTING = ::RegisterWindowMessage("RMSG_INTSETTING");
const UINT RMSG_INTMESSAGE = ::RegisterWindowMessage("RMSG_INTMESSAGE");
const UINT RMSG_INTREQUEST = ::RegisterWindowMessage("RMSG_INTREQUEST");
const UINT RMSG_SHARDDATA = ::RegisterWindowMessage("RMSG_SHARDDATA");
const UINT RMSG_CONCERNHWND	= ::RegisterWindowMessage(_T("RMSG_CONCERNHWND"));

const UINT RMSG_GETRECENTCODE = ::RegisterWindowMessage("RMSG_GETRECENTCODE");
const UINT RMSG_MARKETSTATE = ::RegisterWindowMessage("RMSG_MARKETSTATE");

#define NOTIFY_REFLESHROLL  1	// 돌려보기에 업데이트 된 것을 알림
#define NOTIFY_RESETALLDATA 3   // 모든 관심 관련된 것에 업데이트된 것을 알미
#define NOTIFY_RESETALLETC1 6   // 관심에서 변경된 것을 요청 되었을때 처리 
                                // 보낸 놈은 처리 안하는 것임.
//2004.12.02 by LYH
#define NOTIFY_CHANGEDFILE   7   // 서버에서 파일 내려받기를 한 경우. 관심 파일 변경 처리

#define REQUEST_CHANGEGROUP 11

#define DEF_GRID_CURRENT 1
#define DEF_GRID_CANDLE  2
#define DEF_GRID_STRING  4
#define DEF_GRID_FLOAT   8
#define DEF_GRID_CHECK	 16
#define DEF_GRID_EXP	 32 // 예상체결
#define DEF_GRID_COMMA   64
#define DEF_GRID_IMAGE   128
#define DEF_GRID_SIGN	 256
/////////////////////////////////////////////////////////////
#endif // !defined(AFX_WMUTIL_H__61A0968A_BCC0_4B3C_96F6_9C7AC762ED28__INCLUDED_)
