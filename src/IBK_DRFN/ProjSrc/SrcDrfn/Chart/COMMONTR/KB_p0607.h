/*************************************************************************************/
/*  1. 프로그램ID : KB_d1015                                                         */
/*  2. 프로그램명 : 차트 조회(시장)                                                  */
/*  3. 서비스명   : KB_d1015                                                         */
/*  4. 사용테이블 :                                                                  */
/*  5. 작성자     : drfn                                                             */
/*  6. 입력항목   :                                                                  */
/*  7. 출력항목   :                                                                  */
/*************************************************************************************/
#ifndef __KB_TRAN_KB_d1015_H__
#define __KB_TRAN_KB_d1015_H__

/*************************************************************************************/
/* include file                                                                      */
/*************************************************************************************/

/*************************************************************************************/
/* define constant                                                                   */
/*************************************************************************************/
//#define KB_TRAN                       "KB_d1015"
//#define LLONG                         long long


/*************************************************************************************/
/* Input structure                                                                   */
/*************************************************************************************/

#define		ChartKeySize_d1015		84

/* InRec1:                                                                           */
typedef struct
{
	char		winid[8];			/* 윈도우아이디				*/
	char		type[1];			/* 타입(0:종목/1:업종)		*/
	char		shcode[8];			/* 종목코드					*/
	char		edate[8];			/* 검색일자(최근일)			*/
	char		num[4];				/* 검색건수					*/
	char		img[1];				/* 봉구분(0:허/1:실봉)		*/
	char		dataNm[100];		/* 데이터					*/
	char		nxtGb[1];			/* 구분(0:조회/1:다음)		*/
//	char		nxtKey[21];			/* 조회키(다음/이전)		*/
	char		nxtKey[ChartKeySize_d1015];			/* 조회키(다음/이전)		*/	
} KB_d1015_InRec1;

/*************************************************************************************/
/* Output structure                                                                  */
/*************************************************************************************/

/* OutRec1:                                                                          */
// 사용하지 않는 Field 주석처리
typedef struct
{
	char		winid[8];						  /* 윈도우아이디					 */
	char		dataNm[100];					  /* 데이타번호						 */
	char		inNumGb[1];						  /* 구분값							 */
	char		shcode[8];						  /* 종목코드						 */
	char		nxtKey[ChartKeySize_d1015];		  /* 검색키							 */
	char		chtCnt[5];						  /* 조회건수						 */
	char		chtBjM[400];					  /* 보조메시지						 */
} KB_d1015_OutRec1;

/* OutRec2:                                                                          */
typedef struct
{
    char    date[8];                              /* 날짜(8)                         */
    char    data1[15];                            /* 데이타1(15)                     */
} KB_d1015_OutRec2_1;

typedef struct
{
    char    date[8];                              /* 날짜(8)                         */
    char    data1[15];                            /* 데이타1(15)                     */
    char    data2[15];                            /* 데이타2(15)                     */
    char    data3[15];                            /* 데이타3(15)                     */
} KB_d1015_OutRec2_3;

/* Chart Data:																		 */
typedef struct _KB_d1015_CHART_1
{
	char    chtCnt[5];         					  /* 차트카운트(5)                   */
	char    chtBjM[400];						  /* 보조메시지(300)                 */
	//KB_d1015_OutRec2_1  chart[50000];
	KB_d1015_OutRec2_1  chart[1];
} KB_d1015_CHART_1;

typedef struct _KB_d1015_CHART_3
{
	char    chtCnt[5];         					  /* 차트카운트(5)                   */
	char    chtBjM[400];						  /* 보조메시지(300)                 */
	//KB_d1015_OutRec2_3  chart[50000];
	KB_d1015_OutRec2_3  chart[1];
} KB_d1015_CHART_3;

typedef struct
{
	char		winid[8];						  /* 윈도우아이디					 */
	char		dataNm[100];					  /* 데이타번호						 */
	char		inNumGb[1];						  /* 구분값							 */
	char		shcode[8];						  /* 종목코드						 */
	char		nxtKey[ChartKeySize_d1015];		  /* 검색키							 */

	KB_d1015_CHART_1	kb0607_CHART;
} KB_d1015_OutRec_1;

typedef struct
{
	char		winid[8];						  /* 윈도우아이디					 */
	char		dataNm[100];					  /* 데이타번호						 */
	char		inNumGb[1];						  /* 구분값							 */
	char		shcode[8];						  /* 종목코드						 */
	char		nxtKey[ChartKeySize_d1015];		  /* 검색키							 */

	KB_d1015_CHART_3	kb0607_CHART;
} KB_d1015_OutRec_3;

/*************************************************************************************/

#endif

