/*************************************************************************************/
/*  1. 프로그램ID : EU_p0602                                                         */
/*  2. 프로그램명 : 차트 조회(주식)                                                  */
/*  3. 서비스명   : EU_p0602                                                         */
/*  4. 사용테이블 :                                                                  */
/*  5. 작성자     : drfn                                                             */
/*  6. 입력항목   :                                                                  */
/*  7. 출력항목   :                                                                  */
/*************************************************************************************/
#ifndef __EU_TRAN_EU_p0602_H__
#define __EU_TRAN_EU_p0602_H__

/*************************************************************************************/
/* include file                                                                      */
/*************************************************************************************/

/*************************************************************************************/
/* define constant                                                                   */
/*************************************************************************************/
//#define EU_TRAN                       "EU_p0602"
//#define LLONG                         long long


/*************************************************************************************/
/* Input structure                                                                   */
/*************************************************************************************/

#pragma pack(push, 1)
/* InRec1:                                                                           */
typedef struct
{
	char		szMemberNo[3];		/* 회원사 ID : 회원사 ID */
	char		szTradeType[2];		/* 종목 구분 - 거래단위(00 : 10만, 01:1만) */
	char		szDealerGrpID[3];	/* 딜러 그룹 코드 : 딜러 그룹 코드 */
	char		szCurNo[12];		/* 종목 코드 : 종목 코드 */
	char		szTermDiv[1];		/* 기간 구분 : 기간 구분 */
	long		nMinute;			/* 분 : 분 */
	char		szDate[8];			/* 기준 일자 : 기준 일자 */
	char		szTime[6];			/* 기준 시간 : 기준 시간 */
	long		nDataCnt;			/* 데이터 개수 : 데이터 개수 */
	char		szPOCode[5];		/* 매수 079, 매도 081 */

} EU_T9726_INREC1;

typedef struct _EU_T9726_OUT_SINGLE
{
	char		szTradeType[2];		/* 종목 구분 - 거래단위(00 : 10만, 01:1만) */
	char		SHCODE[12];			/* 종목 코드 : 종목 코드 */
	char		cTermDiv[1];		/* 기간 구분 : 기간 구분 */
	char		cPipLowest[1];		/* 소수점 유효자리수 */
} EU_T9726_OUT_SINGLE;

#define EU_T9726_OUT_SINGLE_SZ sizeof(EU_T9726_OUT_SINGLE)

typedef struct _EU_T9726_OUT_ARR
{
	char		szDate[8];			/* 일자 : 일자 */
	char		szTime[6];			/* 시간 : 시간 */
	char		szBid[10];			/* 매수가(bid) : 매수가(bid) */
	char		szAsk[10];			/* 매도가(ask) : 매도가(ask) */
	char		szBidOpen[10];		/* 시가 : 시가 */
	char		szBidHigh[10];		/* 고가 : 고가 */
	char		szBidLow[10];		/* 저가 : 저가 */
	char		szBidClose[10];		/* 종가 : 종가 */
	char		szAmount[10];		/* 거래량 : 거래량 */
	long		nInfoCnt;			/* 해당일자 정보 건수 : 해당일자 정보 건수 */
	long		nNewsCnt;			/* 해당일자 뉴스 건수 : 해당일자 뉴스 건수 */
	long		nOrdCnt;			/* 해당일자 주문 건수 : 해당일자 주문 건수 */
	long		nExecutedCnt;		/* 해당일자 체결 건수 : 해당일자 체결 건수 */
	long		nModCnt;			/* 해당일자 정정취소 건수 : 해당일자 정정취소 건수 */
} EU_T9726_OUT_ARR;

#define EU_T9726_OUT_ARR_SZ sizeof(EU_T9726_OUT_ARR)


typedef struct _EU_T9726_OUT_KEY
{
    char    display_cnt[ 4];		/* 조회 리스트 COUNT    */
}EU_T9726_OUT_KEY;

#define EU_T9726_OUT_KEY_SZ sizeof(EU_T9726_OUT_KEY)


/*************************************************************************************/
/* Output structure                                                                  */
/*************************************************************************************/

/* OutRec1:                                                                          */
// 사용하지 않는 Field 주석처리
typedef struct
{
    char    name[20];                             /* 종목명(20)                      */
    char    price[10];                            /* 현재가(10)                      */
    char    sign[1];                              /* 등락부호(1)                     */
    char    change[10];                           /* 대비(10)                        */
    char    chrate[6];                            /* 대비율(6)                       */
    char    preprice[10];                         /* 기준가(10)                      */
    char    nkey[14];                             /* 이전키(14)                      */
    char    fcnt[5];                              /* 차트카운트(5)                   */
    char    bojomsg[300];                         /* 보조메시지(300)                 */
} EU_p0602_OutRec1;

typedef struct
{
    char    name[20];                             /* 종목명(20)                      */
    char    price[10];                            /* 현재가(10)                      */
    char    sign[1];                              /* 등락부호(1)                     */
    char    change[10];                           /* 대비(10)                        */
    char    chrate[6];                            /* 대비율(6)                       */
    char    volume[10];                           /* 거래량(10)                      */
    char    preprice[10];                         /* 기준가(10)                      */
    char    nkey[14];                             /* 이전키(14)                      */
    char    fcnt[5];                              /* 차트카운트(5)                   */
    char    bojomsg[300];                         /* 보조메시지(300)                 */
} EU_p0602_OutRec1_EX;

/* OutRec2:                                                                          */
// 사용하지 않는 Field 주석처리
//char EU_p0602_OutRec2_count[4];
typedef struct
{
    char    date[8];                              /* 날짜(8)                         */
    char    open[10];                             /* 시가(10)                        */
    char    high[10];                             /* 고가(10)                        */
    char    low[10];                              /* 저가(10)                        */
    char    close[10];                            /* 종가(10)                        */
    char    volume[10];                           /* 거래량(10)                      */
    char    rights[1];                            /* 권리락(1)                       */
} EU_p0602_OutRec2;

typedef struct
{
    char    date[8];                              /* 날짜(8)                         */
    char    open[10];                             /* 시가(10)                        */
    char    high[10];                             /* 고가(10)                        */
    char    low[10];                              /* 저가(10)                        */
    char    close[10];                            /* 종가(10)                        */
} EU_p0602_OutRec2_COMP;

/* Chart Data:																		 */
typedef struct _EU_p0602_CHART
{
	char    fcnt[5];         			/* 차트카운트(5)                   */
	char    bojomsg[300];				/* 보조메시지(300)                 */
	EU_p0602_OutRec2  chart[1];			// 동적크기임.
} EU_p0602_CHART;

typedef struct _EU_p0602_CHART_COMP
{
	char    fcnt[5];         			/* 차트카운트(5)                   */
	char    bojomsg[300];				/* 보조메시지(300)                 */
	EU_p0602_OutRec2_COMP  chart[50000];
} EU_p0602_CHART_COMP;

/*************************************************************************************/

#define TR3930	"t3930"

typedef struct _EU_T3930_INREC
{
	char  szMemberNo[3];     /* 회원사 */
	char  szTradeType[2];    /* 거래단위 */
	char  szCustNo[17];      /* 사용자ID */
} EU_T3930_INREC;

typedef struct _EU_T3930_OUTREC
{
	char  szCurNo[12];      /* 종목명 */
	char  szSide[3];        /* 매매구분 - 매수(079), 매도(081) */
	char  szPrice[10];      /* 가격 */
	char  szAmount[11];     /* 수량 */

} EU_T3930_OUTREC;

typedef struct _Order_Info
{
	CString szCurNo;         /* 종목명 */
	CString	szSide;          /* 매매구분 - 매수(079), 매도(081) */
	CString	szPrice;         /* 가격 */
	CString	szAmount;        /* 수량 */

} ORDER_INFO;

#pragma pack(pop)

#endif

