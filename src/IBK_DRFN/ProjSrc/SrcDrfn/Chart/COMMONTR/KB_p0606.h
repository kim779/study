/*************************************************************************************/
/*  1. 프로그램ID : KB_p0606                                                         */
/*  2. 프로그램명 : 차트 조회(해외)                                                  */
/*  3. 서비스명   : KB_p0606                                                         */
/*  4. 사용테이블 :                                                                  */
/*  5. 작성자     : drfn                                                             */
/*  6. 입력항목   :                                                                  */
/*  7. 출력항목   :                                                                  */
/*************************************************************************************/
#ifndef __KB_TRAN_KB_p0606_H__
#define __KB_TRAN_KB_p0606_H__

/*************************************************************************************/
/* include file                                                                      */
/*************************************************************************************/

/*************************************************************************************/
/* define constant                                                                   */
/*************************************************************************************/
//#define KB_TRAN                       "KB_p0606"
//#define LLONG                         long long


/*************************************************************************************/
/* Input structure                                                                   */
/*************************************************************************************/

#ifndef ChartKeySize
#define		ChartKeySize		28
#endif

/* InRec1:                                                                           */
typedef struct
{
	char	Shcode[16];							/* 종목코드							*/
	char	date[8];							/* 검색시작일						*/
	char	num[4];								/* 조회건수							*/
	char	unit[3];							/* 구분(Tick/Sec/Min/Day)			*/
	char	button[1];							/* 조회모드(틱:0/분:1/일:2/주:3/월:4)*/
	char	nflag[1];							/* 조회구분(다음:1/조회:0)			*/
	char	nkey[ChartKeySize];					/* 페치키							*/
	char	nowData[1];							/* 당일조회							*/
	char	timeGB[1];							/* 현지시간구분(1: 한국시간, 0:현지시간)*/
	char	dataType[1];						/* 차트구분 (0:가격, 1:비율,2:달러)*/
} KB_p0606_InRec1;


/*************************************************************************************/
/* Output structure                                                                  */
/*************************************************************************************/

/* OutRec1:                                                                          */
// 사용하지 않는 Field 주석처리
typedef struct
{
    char    name[40];                             /* 종목명(40)                      */
    char    price[10];                            /* 현재가(10)                      */
    char    sign[1];                              /* 등락부호(1)                     */
    char    change[10];                           /* 대비(10)                        */
    char    chrate[6];                            /* 대비율(6)                       */
    char    volume[10];                           /* 거래량(10)                      */
    char    offer[10];                            /* 매도호가(10)                    */
    char    bid[10];                              /* 매수호가(10)                    */
    char    preprice[10];                         /* 기준가(10)                      */
    //char    nkey[21];                             /* 이전키(21)                      */
	char	nkey[ChartKeySize];
    char    cntsz[6];                             /* 차트사이즈(6)                   */
    char    fcnt[5];                              /* 차트카운트(5)                   */
    char    bojomsg[400];                         /* 보조메시지(300)                 */
} KB_p0606_OutRec1;

typedef struct
{
    char    name[40];                             /* 종목명(40)                      */
    char    price[10];                            /* 현재가(10)                      */
    char    sign[1];                              /* 등락부호(1)                     */
    char    change[10];                           /* 대비(10)                        */
    char    chrate[6];                            /* 대비율(6)                       */
    char    volume[10];                           /* 거래량(10)                      */
//    char    offer[10];                            /* 매도호가(10)                    */
//    char    bid[10];                              /* 매수호가(10)                    */
    char    preprice[10];                         /* 기준가(10)                      */
    //char    nkey[21];                             /* 이전키(21)                      */
	char	nkey[ChartKeySize];
//    char    cntsz[6];                             /* 차트사이즈(6)                   */
    char    fcnt[5];                              /* 차트카운트(5)                   */
    char    bojomsg[400];                         /* 보조메시지(300)                 */
} KB_p0606_OutRec1_EX;

typedef struct
{
    char	name[40];                         /* 종목명(20)                      */
    char	price[10];                         /* 현재가(10)                      */
    char	sign[1];                         /* 등락부호(1)                     */
    char	change[10];                         /* 대비(10)                        */
    char	chrate[6];                         /* 대비율(6)                       */
    char	preprice[10];                         /* 기준가(10)                      */
    //char	nkey[21];                         /* 이전키(21)                      */
	char	nkey[ChartKeySize];
    char	fcnt[5];                         /* 차트카운트(5)                   */
    char	bojomsg[400];                         /* 보조메시지(400)                 */
} KB_p0606_OutRec1_COMP;

/* OutRec2:                                                                          */
// 사용하지 않는 Field 주석처리
//char KB_p0606_OutRec2_count[4];
typedef struct
{
    char    date[8];                              /* 날짜(8)                         */
    char    open[10];                             /* 시가(10)                        */
    char    high[10];                             /* 고가(10)                        */
    char    low[10];                              /* 저가(10)                        */
    char    close[10];                            /* 종가(10)                        */
    char    volume[10];                           /* 거래량(10)                      */
} KB_p0606_OutRec2;

typedef struct
{
    char    date[8];                              /* 날짜(8)                         */
    char    open[10];                             /* 시가(10)                        */
    char    high[10];                             /* 고가(10)                        */
    char    low[10];                              /* 저가(10)                        */
    char    close[10];                            /* 종가(10)                        */
} KB_p0606_OutRec2_COMP;

/* Chart Data:																		 */
typedef struct _KB_p0606_CHART
{
	char    fcnt[5];         					  /* 차트카운트(5)                   */
	char    bojomsg[400];						  /* 보조메시지(300)                 */
	//KB_p0606_OutRec2  chart[50000];
	KB_p0606_OutRec2  chart[1];						// 동적크기임.
} KB_p0606_CHART;

typedef struct _KB_p0606_CHART_COMP
{
	char    fcnt[5];         			/* 차트카운트(5)                   */
	char    bojomsg[400];				/* 보조메시지(300)                 */
	//KB_p0602_OutRec2_COMP  chart[50000];
	KB_p0606_OutRec2_COMP  chart[1];
} KB_p0606_CHART_COMP;
/*************************************************************************************/

#endif

