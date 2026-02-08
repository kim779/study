/*************************************************************************************/
/*  1. 프로그램ID : SM_fx112#                                                       */
/*  2. 프로그램명 : Chart                                                            */
/*  3. 서비스명   : SM_fx112#                                                       */
/*  4. 사용테이블 :                                                                  */
/*  5. 작성자     : kmlee98                                                          */
/*  6. 입력항목   :                                                                  */
/*  7. 출력항목   :                                                                  */
/*************************************************************************************/
#ifndef __EU_TRAN_SM_fx112_H__
#define __EU_TRAN_SM_fx112_H__

/*************************************************************************************/
/* include file                                                                      */
/*************************************************************************************/

/*************************************************************************************/
/* define constant                                                                   */
/*************************************************************************************/
#define SM_fx112                     "SM_fx112#"
#define LLONG                         long long


/*************************************************************************************/
/* Input structure                                                                   */
/*************************************************************************************/

/* InRec1:                                                                           */
typedef struct
{
	char		keyvalue[84];		/* key 일자+시간	: key 일자+시간 */
	char		jmcode[10];			/* 종목코드			: 종목코드 */
	char		date[8];			/* 일자				: 일자 */
	char		chetime[10];		/* 시간				: 시간 */
	char		gubun[1];			/* 구분				: 0:처음1:다음 */
	char		dgubun[1];			/* 데이터구분		: 1:Tick,2:분,3:일,4:주,5:월 */
	char		nsel[4]/*[2]*/;			/* N분구분			: 0:30sec,1:1M,2:2M,... */
	char		mgubun[1];			/* 매도매수구분		: 0:매도,1:매수,2:전체	... */
	char		dcnt[5];			/* 데이터건수		: 데이터건수 */
}SM_fx112_InRec1;


/*************************************************************************************/
/* Output structure                                                                  */
/*************************************************************************************/
//typedef struct _SM_fx112_OUT_HEADER
//{
//	char		keyvalue[18];		/* key 일자+시간	: key 일자+시간 */
//	char		count[4];
//} SM_fx112_OUT_HEADER;
//#define SM_fx112_OUT_SZ sizeof(SM_fx112_OUT_HEADER)

/* OutRec1:                                                                          */
typedef struct
{
    char    price[10];                            /* 현재가(10)                      */
    char    sign[1];                              /* 등락부호(1)                     */
    char    change[10];                           /* 대비(10)                        */
    char    chrate[6];                            /* 대비율(6)                       */
    char    open[10];                             /* 시가(10)                        */
    char    high[10];                             /* 고가(10)                        */
    char    low[10];                              /* 저가(10)                        */
    char    volume[10];                           /* 거래량(10)                      */
    char    preprice[10];							/* 전일종가(10)                      */
	
    char    preopen[10];							/* 전일시가(10)                      */
    char    prehigh[10];							/* 전일고가(10)                      */
    char    prelow[10];								/* 전일저가(10)                      */
	
}SM_fx112_OutRec1_Sub;


typedef struct
{
    char					name[20];                           /* 종목명(20)                       */
    char					codetype[1];                        /* 종목구분(1)                      */
    char					startDate[10];                      /* 조회시작일                      */
    char					endDate[10];						/* 조회종료일                      */
    char					nkey[ChartKeySize];                 /* 이전키(14)                      */

    char					piplowest[6];                       /* 소수점 유효자리(6)( 5자리:100000, 3자리:1000)              */
	char					mgubun[1];							/* 매도매수구분 : 0:매도,1:매수,2:전체		... */

	SM_fx112_OutRec1_Sub	fx112OutRec1_S;						/* 매도								*/
	SM_fx112_OutRec1_Sub	fx112OutRec1_B;						/* 매수								*/
	
}SM_fx112_OutRec1;


#define SM_fx112_OUT1_SZ sizeof(SM_fx112_OutRec1)

/* OutRec2:                                                                          */
// char SM_fx112_OutRec2_count[4];
typedef struct
{
	// 2011.01.18 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
    //char	date[8];                              /* 날짜(8)                         */
	char	date[14];                             /* 날짜(14)                        */
	// 2011.01.18 by SYS <<
	//
    char    open[10];                             /* 시가(10)                        */
    char    high[10];                             /* 고가(10)                        */
    char    low[10];                              /* 저가(10)                        */
    char    close[10];                            /* 종가(10)                        */
//} SM_fx112_OutRec2_Sub;
} SM_fx112_OutRec2;

//typedef struct
//{
//	SM_fx112_OutRec2_Sub	fx112OutRec2_S;						/* 매도								*/
//	SM_fx112_OutRec2_Sub	fx112OutRec2_B;						/* 매수								*/
//
//} SM_fx112_OutRec2;

#define SM_fx112_OUT_KEY_SZ		4	

//typedef struct
//{
//    char    szDate[14];                           /* 일자(14)                        */
//    char    szBid[10];                            /* 매수가(bid)(10)                 */
//    char    szAsk[10];                            /* 매도가(ask)(10)                 */
//    char    szBidOpen[10];                        /* 시가(10)                        */
//    char    szBidHigh[10];                        /* 고가(10)                        */
//    char    szBidLow[10];                         /* 저가(10)                        */
//    char    szBidClose[10];                       /* 종가(10)                        */
//    char    szFiller[2];                          /* Filler(2)                       */
//}SM_fx112_OutRec2;

/* Chart Data:																		 */
typedef struct _SM_fx112_CHART
{
	char				fcnt[5];         			/* 차트카운트(5)                   */
	char				bojomsg[500];				/* 보조메시지(400)                 */
	char				chart[1];					// 동적크기임.
//	SM_fx112_OutRec2	chart[1];			// 동적크기임.
} SM_fx112_CHART;


#define SM_fx112_OUT2_SZ sizeof(SM_fx112_OutRec2)

/*************************************************************************************/

#endif

