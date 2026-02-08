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

#ifndef ChartKeySize
#define		ChartKeySize		28
#endif

/*************************************************************************************/
/* Input structure                                                                   */
/*************************************************************************************/

/* InRec1:                                                                           */
typedef struct
{
    char    shcode[15];                           /* 종목코드(15)                    */
    char    date[8];                              /* 검색시작일(8)                   */
    char    num[4];                               /* 조회건수(4)                     */
    char    unit[3];                              /* 조회단위(3)                     */
    char    button[1];                            /* 조회구분(1)                     */
    char    modGb[1];                             /* 수정주가(1)                     */
    char    nflag[1];                             /* 조회모드(1)                     */
    char    nkey[ChartKeySize];                   /* 검색키(21)                      */
    char    nDataGubun[1];                        /* 가격구분(1) (0:가격, 1:비율,2:달러) */
    char    ImgRgb[1];                            /* 봉구분(1)                       */
    char    Irecordgb[1];                         /* 복기구분값(1)(1)                */
    char    nDayBefore[2];                        /* 복기시작n일전(2)                */
    char    Itime[6];                             /* 복기시간(hhmmss)(6)             */
	char	nowData[1];							  /*  [당일조회]     */
} EU_p0602_InRec1;


/*************************************************************************************/
/* Output structure                                                                  */
/*************************************************************************************/

typedef struct
{
    char    name[30];                             /* 종목명(20)                      */
    char    codetype[1];                          /* 종목구분(1)                     */
    char    price[10];                            /* 현재가(10)                      */
    char    sign[1];                              /* 등락부호(1)                     */
    char    change[10];                           /* 대비(10)                        */
    char    chrate[6];                            /* 대비율(6)                       */
    char    open[10];                             /* 시가(10)                        */
    char    high[10];                             /* 고가(10)                        */
    char    low[10];                              /* 저가(10)                        */
    char    volume[10];                           /* 거래량(10)                      */
    char    preprice[10];                         /* 기준가(10)                      */
    char    startDate[10];                        /* 조회시작일                      */
    char    endDate[10];						  /* 조회종료일                      */
	char    nkey[ChartKeySize];                   /* 이전키(14)                      */
} EU_p0602_OutRec1;

/* OutRec2:                                                                          */
typedef struct
{
	// 2011.01.13 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
    //char    date[8];                             /* 날짜(8)                         */
    char    date[14];                              /* 날짜(14)                         */
	// 2011.01.13 by SYS <<
    char    open[10];                             /* 시가(10)                        */
    char    high[10];                             /* 고가(10)                        */
    char    low[10];                              /* 저가(10)                        */
    char    close[10];                            /* 종가(10)                        */
    char    volume[10];                           /* 거래량(10)                      */
} EU_p0602_OutRec2;

/* Chart Data:																		 */
typedef struct _EU_p0602_CHART
{
	char    fcnt[5];         			/* 차트카운트(5)                   */
	char    bojomsg[600];				/* 보조메시지(400)                 */
	EU_p0602_OutRec2  chart[1];			// 동적크기임.
} EU_p0602_CHART;

/*************************************************************************************/

#endif

