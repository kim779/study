/*************************************************************************************/
/*  1. 프로그램ID : SM_CheJanStruct                                                  */
/*  2. 프로그램명 : aa                                                               */
/*  3. 서비스명   : SM_CheJanStruct	                                                 */
/*  4. 사용테이블 :                                                                  */
/*  5. 작성자     : gds                                                              */
/*  6. 입력항목   :                                                                  */
/*  7. 출력항목   :                                                                  */
/*************************************************************************************/
#ifndef __SM_TRAN_SM_CHEJANSTRUCT_H__
#define __SM_TRAN_SM_CHEJANSTRUCT_H__

/*************************************************************************************/
/* include file                                                                      */
/*************************************************************************************/

/*************************************************************************************/
/* define constant                                                                   */
/*************************************************************************************/
#define LLONG                         long long


#define		UM_CHEJAN_COMMIT		WM_USER + 1100		//	계좌, 잔고, 체결 통신 완료 메시지
#define		UM_GETBROAD				WM_USER + 1101		//	체결, 잔고 실시간 변경 메시지
#define		UM_CHEJAN_NEWDATA		WM_USER + 1102		//	새로운 잔고 추가시 통보 메시지
#define		UM_GETBROADTEMP			WM_USER + 1103		//	새로운 잔고 추가시 통보 메시지
#define		UM_RESET_CHEJANINFO		WM_USER + 1150		//	체결잔고 재설정 메시지


#define ulong	 ULONG

#pragma pack(push, 1) 

//===================================================================================
//	통신 데이터 구조체
//===================================================================================
//	주식 주문체결 정보 데이터 조회 구조체
typedef struct 
{
	char	accno			[11];		// 계좌번호 
	char	scod_gb			[1];		// 종목조회구분(0:전체, 1:종목조회)
	char	debg			[1];		// 매도매수조회구분(0:전체, 1:매도, 2:매수)
	char	scod			[12];		// 단축종목코드
} CHE_RQ_INPUT;

typedef struct 
{
	char	nCnt			[ 4];		// 데이터 건수
} CHE_RQ_OUTPUT1;

typedef struct 
{
	char	or_type			[ 2];	  /* 주문유형코드					  */
	char	prgb			[ 2];	  /* 주문체결유형코드				  jcgb*/
	char 	accno			[11];     /* 계좌번호                         */
	char 	orderno			[10];     /* 주문번호                         */
	char 	orgorderno		[10];     /* 원주문번호                       */
	char 	cheno			[10];     /* 체결번호                         */
	char 	jmcode			[12];     /* 종목코드                         */
	char 	jmname			[40];     /* 종목명                           */
	char 	bid_tpcd		[ 2];     /* 호가유형코드                     memegb*/
	char 	ord_cond		[ 1];     /* 주문조건                         */
	char    cdgb			[ 3];	  /* 신용구분                        */
	char 	ord_tpcd		[ 2];     /* 주문유형코드                     jugb*/
	char 	ord_mkcd		[ 2];     /* 주문시장코드                     ordst*/
	char 	medosu			[ 1];     /* 매도수구분 ( 1:매도, 2:매수 )    */
	char 	jprc			[13];     /* 주문가격                         */
	char 	jqty			[16];     /* 주문수량                         */
	char 	cheprice		[13];     /* 체결가                           */
	char 	cheqty			[16];     /* 체결량                           */
	char 	sumprice		[16];     /* 체결누계금액                     */
	char 	mecheqty		[16];     /* 미체결수량                       */
	char 	chetime			[ 8];     /* 주문시간		                  */
	char 	price			[ 9];     /* 현재가                           */
	char 	sign			[ 1];     /* 전일대비부호                     */
	char 	medoga			[ 9];     /* (최우선)매도호가                 */
	char 	mesuga			[ 9];     /* (최우선)매수호가                 */
} CHE_RQ_OUTPUT2;

//	주식 주문미체결 정보 데이터 조회 구조체
typedef struct 
{
	char	accno			[11];		// 계좌번호 
	char	scod_gb			[1];		// 종목조회구분(0:전체, 1:종목조회)
	char	debg			[1];		// 매도매수조회구분(0:전체, 1:매도, 2:매수)
	char	scod			[12];		// 단축종목코드
} MICHE_RQ_INPUT;

typedef struct 
{
	char	nCnt			[ 4];		// 데이터 건수
} MICHE_RQ_OUTPUT1;

typedef struct 
{
	char	or_type			[ 2];	  /* 주문유형코드					  */
	char	prgb			[ 2];	  /* 주문체결유형코드				  jcgb*/
	char 	accno			[11];     /* 계좌번호                         */
	char 	orderno			[10];     /* 주문번호                         */
	char 	orgorderno		[10];     /* 원주문번호                       */
//	char 	cheno			[10];     /* 체결번호                         */
	char 	jmcode			[12];     /* 종목코드                         */
	char 	jmname			[40];     /* 종목명                           */
	char 	bid_tpcd		[ 2];     /* 호가유형코드                     memegb*/
	char 	ord_cond		[ 1];     /* 주문조건                         */
	char    cdgb			[ 3];	  /*  신용구분                        */
	char 	ord_tpcd		[ 2];     /* 주문유형코드                     jugb*/
	char 	ord_mkcd		[ 2];     /* 주문시장코드                     ordst*/
	char 	medosu			[ 1];     /* 매도수구분 ( 1:매도, 2:매수 )    */
	char 	jprc			[13];     /* 주문가격                         */
	char 	jqty			[16];     /* 주문수량                         */
//	char 	cheprice		[13];     /* 체결가                           */
//	char 	cheqty			[16];     /* 체결량                           */
//	char 	sumprice		[16];     /* 체결누계금액                     */
	char 	mecheqty		[16];     /* 미체결수량                       */
	char 	chetime			[ 8];     /* 주문시간		                  */
	char 	price			[ 9];     /* 현재가                           */
	char 	sign			[ 1];     /* 전일대비부호                     */
	char 	medoga			[ 9];     /* (최우선)매도호가                 */
	char 	mesuga			[ 9];     /* (최우선)매수호가                 */
} MICHE_RQ_OUTPUT2;

//	주식 잔고 통신 데이터 구조체
typedef struct 
{
	char	accno			[11];		// 계좌번호 
} JAN_RQ_INPUT;

typedef struct 
{
	char	accno			[11];	// 계좌번호 
	char    trsonik			[16];	/* 수수료/세금이 포함 안된 당일 실현손익*/
    char    sonik_fee		[16];	/* 실현손익에 대한 수수료				*/
    char    sonik_tax		[16];	/* 실현손익에 대한 세금					*/
	char	nCnt			[ 4];	/* 데이터 건수							*/
} JAN_RQ_OUTPUT1;

typedef struct 
{
	char    accno			[11];  /*  계좌번호                        */
    char    jmcode			[12];  /*  종목코드                        */
	char    jmname			[40];  /*  종목명                          */
    char    sygb			[ 3];  /*  신용구분                        */
	char    bqty			[16];  /*  보유수량                        */
	char    ordgaqty		[16];  /*  주문가능 수량                   */
	char    buyamt			[13];  /*  매입단가                        */
	char    bep_price		[13];  /*  BEP 단가                        */
	char    menusum			[16];  /*  매입금액                        */
    char    estamt			[16];  /*  평가금액                        */
	char    estsonik		[16];  /*  평가손익                        */
	char    rate			[ 8];  /*  수익률                          */
	char    fee				[16];  /*  수수료                          */
	char    tax				[16];  /*  세금                            */
	char    holdrate		[ 6];  /*  보유비중                        */
	char    sdate			[ 8];  /*  대출일                          */
    char    lastday			[ 8];  /*  만기일                          */
    char    sinamt			[16];  /*  신용금액                        */
    char    sininter		[16];  /*  신용이자                        */
    char    janqty			[16];  /*  결제잔고                        */
    char    psmevol			[16];  /*  전일순매수량                    */
    char    tsmevol			[16];  /*  당일 순매수량                   */
    char    damboloanamt	[16];  /*  담보대출수량                    */
	char    tmedosonik		[16];  /*  당일 총 매도손익                */
	char    trsonik			[16];  /*  당일실현손익(유가)              */
    char    ratesonik		[16];  /*  당일실현손익률(유가)            */
    char    strsonik		[16];  /*  당일실현손익(신용)              */
    char    sratesonik		[16];  /*  당일실현손익률(신용)            */
	char    pjogaest		[16];  /*  전일종가 전일평가               */
	char    pjogasonik		[16];  /*  전일종가 기준 평가손익          */
    char    pjogarate		[16];  /*  전일종가 기준 수익률            */
	char 	medoamtsum		[16];  /*  당일매도금액합                  */
	char 	mesuamtsum		[16];  /*  당일매수금액합                  */
	char    giga			[ 9];  /*  기준가                          */
	char    uplmtprice		[ 9];  /*  상한가                          */
	char    dnlmtprice		[ 9];  /*  하한가                          */
	char    price			[ 9];  /*  현재가                          */
	char    mesuga			[ 9];  /*  (최우선)매수호가                */
	char    medoga			[ 9];  /*  (최우선)매도호가                */
	char    sign			[ 1];  /*  전일구분                        */
	char    change			[ 8];  /*  전일대비                        */
	char    diff			[ 6];  /*  등락률 ( 소수점 2자리 )         */
    char    volume			[16];  /*  거래량                          */

} JAN_RQ_OUTPUT2;


//	선물옵션 체결 정보 데이터 조회 구조체
typedef struct 
{
	char	accno			[11];		// 계좌번호 
	char	scod_gb			[ 1];		// 종목조회구분(0:전체, 1:종목조회)
	char	debg			[ 1];		// 매도매수조회구분(0:전체, 1:매도, 2:매수)
	char	scod			[12];		// 단축종목코드
} FO_CHE_RQ_INPUT;

typedef struct 
{
	char	nCnt			[ 4];		//	데이터 건수
} FO_CHE_RQ_OUTPUT1;

typedef struct 
{
	char	or_type			[ 2];	  /* 주문유형코드					  */
	char	prgb			[ 2];	  /* 주문체결유형코드				  jcgb*/
	char 	accno			[11];     /* 계좌번호                         */
	char 	orderno			[10];     /* 주문번호                         */
	char 	orgorderno		[10];     /* 원주문번호                       */
	char 	cheno			[10];     /* 체결번호                         */
	char 	jmcode			[12];     /* 종목코드                         */
	char 	jmname			[40];     /* 종목명                           */
	char 	bid_tpcd		[ 2];     /* 호가유형코드                     memegb*/
	char 	ord_cond		[ 1];     /* 주문조건                         */
	char 	ord_tpcd		[ 2];     /* 주문유형코드                     jugb*/
	char 	ord_mkcd		[ 2];     /* 주문시장코드                     ordst*/
	char 	medosu			[ 1];     /* 매도수구분 ( 1:매도, 2:매수 )    */
	char 	jprc			[13];     /* 주문가격                         */
	char 	jqty			[16];     /* 주문수량                         */
	char 	cheprice		[13];     /* 체결가                           */
	char 	cheqty			[16];     /* 체결량                           */
	char 	sumprice		[16];     /* 약정금액합계                     */
	char 	mecheqty		[16];     /* 미체결수량                       */
	char 	chetime			[ 8];     /* 주문시간		                  */
	char 	price			[ 9];     /* 현재가                           */
	char 	sign			[ 1];     /* 전일대비부호                     */
	char 	medoga			[ 9];     /* (최우선)매도호가                 */
	char 	mesuga			[ 9];     /* (최우선)매수호가                 */
} FO_CHE_RQ_OUTPUT2;

//	선물옵션 미체결 정보 데이터 조회 구조체
typedef struct 
{
	char	accno			[11];		// 계좌번호 
	char	scod_gb			[ 1];		// 종목조회구분(0:전체, 1:종목조회)
	char	debg			[ 1];		// 매도매수조회구분(0:전체, 1:매도, 2:매수)
	char	scod			[12];		// 단축종목코드
} FO_MICHE_RQ_INPUT;

typedef struct 
{
	char	nCnt			[ 4];		//	데이터 건수
} FO_MICHE_RQ_OUTPUT1;

typedef struct 
{
	char	or_type			[ 2];	  /* 주문유형코드					  */
	char	prgb			[ 2];	  /* 주문체결유형코드				  jcgb*/
	char 	accno			[11];     /* 계좌번호                         */
	char 	orderno			[10];     /* 주문번호                         */
	char 	orgorderno		[10];     /* 원주문번호                       */
//	char 	cheno			[10];     /* 체결번호                         */
	char 	jmcode			[12];     /* 종목코드                         */
	char 	jmname			[40];     /* 종목명                           */
	char 	bid_tpcd		[ 2];     /* 호가유형코드                     memegb*/
	char 	ord_cond		[ 1];     /* 주문조건                         */
	char 	ord_tpcd		[ 2];     /* 주문유형코드                     jugb*/
	char 	ord_mkcd		[ 2];     /* 주문시장코드                     ordst*/
	char 	medosu			[ 1];     /* 매도수구분 ( 1:매도, 2:매수 )    */
	char 	jprc			[13];     /* 주문가격                         */
	char 	jqty			[16];     /* 주문수량                         */
//	char 	cheprice		[13];     /* 체결가                           */
//	char 	cheqty			[16];     /* 체결량                           */
//	char 	sumprice		[16];     /* 약정금액합계                     */
	char 	mecheqty		[16];     /* 미체결수량                       */
	char 	chetime			[ 8];     /* 주문시간		                  */
	char 	price			[ 9];     /* 현재가                           */
	char 	sign			[ 1];     /* 전일대비부호                     */
	char 	medoga			[ 9];     /* (최우선)매도호가                 */
	char 	mesuga			[ 9];     /* (최우선)매수호가                 */
} FO_MICHE_RQ_OUTPUT2;


//===================================================================================
//	잔고 통신 데이터 구조체
//===================================================================================
typedef struct 
{
	char	accno			[11];		// 계좌번호 
} FO_JAN_RQ_INPUT;

typedef struct 
{
	char	accno			[11];	// 계좌번호 
	char    trsonik			[16];	/*  수수료/세금이 포함 안된 당일 실현손익   */
    char    sonik_fee		[16];	/*  실현손익에 대한 수수료					*/
    char    sonik_tax		[16];	/*  실현손익에 대한 세금					*/
	char	nCnt			[ 4];
} FO_JAN_RQ_OUTPUT1;

typedef struct 
{
	char    accno			[11];		/*  계좌번호                        */
    char    jmcode			[12];     /*  종목코드  "XXXXXXXX"            */
    char    jmname			[40];     /*  종목명                          */
	char    bqty			[16];		/*  보유수량                        */
    char    buyamt			[13];		/*  평균매입지수                    */ /*  평균매입지수/100                */
	char    mesusum			[16];		/*  총매입금액                      */ /*  총매입금액/1000+0.5             */
	char    ordgaqty		[16];		/*  청산가능 수량                   */
    char    tsmevol			[16];		/*  당일순매수량                    */ /*  당일매수수량-당일매도수량       */
	char    medosu			[ 1];		/*  매도/매수 구분                  */
	char    fee				[16];		/*  수수료                          */
    char    tax				[16];		/*  세금                            */
	char    preqty			[16];		/*  전일보유수량		            */
	char 	medoamtsum		[16];		/* 매도금액합                       */
	char 	mesuamtsum		[16];		/* 매수금액합                       */
	char    price			[ 9];		/*  현재가                          */ /*  0이면 기준가,                   *//*  기준가도 0이면 전일종가         */
    char    sign			[ 1];		/*  전일구분                        */
    char    medoga			[ 9];		/*  (최우선)매도호가                */
    char    mesuga			[ 9];		/*  (최우선)매수호가                */

} FO_JAN_RQ_OUTPUT2;

//	해외선물 체결 정보 데이터 조회 구조체
typedef struct 
{
	char    acntno[11];                           /* 계좌번호(11)                    */
    char    acntpwd[32];                          /* 비밀번호(32)                    */
    char    base_dt[8];                           /* 거래일자(8)                     */
    char    mrkt_cd[3];                           /* 품목(시장)코드(3)               */
    char    che_gbn[1];                           /* 체결구분(1)                     */
    char    ord_gbn[1];                           /* 주문구분(1)                     */
    char    ord_tp[1];                            /* 주문유형(1)                     */
} FR_CHE_RQ_INPUT;

typedef struct 
{
	char    qry_cnt[4];                           /* 카운트(4)                       */
} FR_CHE_RQ_OUTPUT1;

typedef struct 
{
	char    acctno[11];                           /* 계좌번호(11)                    */
	char    ord_no[5];                            /* 주문번호(5)                     */
    char    orig_ord_no[5];                       /* 원주문번호(5)                   */
    char    series[32];                           /* 종목(32)                        */
    char    bysl_tp[1];                           /* 구분(1)                         */
    char    proc_stat[1];                         /* 주문처리상태(1)                 */
    char    ord_q[10];                            /* 주문량(10)                      */
    char    trd_q[10];                            /* 체결량(10)                      */
    char    remainqty[10];                        /* 잔량(10)                        */
    char    ord_p[19];                            /* 주문가(19)                      */
    char    trd_p[19];                            /* 체결가(19)                      */
    char    mdfy_cncl_q[19];                      /* 정정취소(19)                    */
    char    prce_tp[1];                           /* 가격구분(1)                     */
    char    stop_p[19];                           /* 조건가격(19)                    */
    char    ordtype[1];                           /* 주문유형(1)                     */
	char    ord_tp[1];                            /* 주문구분(1)                     */
    char    acpt_time[10];                        /* 접수시간(10)                    */
    char    kr_ord_date[8];                       /* 한국주문일자(8)                 */
    char    kr_acpt_time[10];                     /* 한국접수시간(10)                */
    char    kr_che_time[10];                      /* 한국체결시간(10)                */
    char    ord_way_tp[1];                        /* 매체구분(1)                     */
    char    work_empl[9];                         /* 사용자ID(9)                     */
    char    crc_cd[3];                            /* 통화코드(3)                     */
} FR_CHE_RQ_OUTPUT2;

//	해외선물 잔고 정보 데이터 조회 구조체
typedef struct 
{
	char    acntno[11];                           /* 계좌번호(11)                    */
} FR_JAN_RQ_INPUT;

typedef struct 
{
	char    t_pospl[19];                          /* 미결제약정평가금액	(19,7)(19)   */
    char    t_open_pl[19];                        /* 평가손익(19)                    */
    char    t_invtrate[19];                       /* 수익율(19)                      */
    char    t_real_pl[19];                        /* 실현손익(19)                    */
    char    t_pl[19];                             /* 총손익(19)                      */
    char    t_cmpr_pl[19];                        /* 전일정산대비(19)                */
    char    t_pres_asset[19];                     /* 추정자산(19)                    */
   	char    acctno[11];                           /* 계좌번호(11)                    */ //수수료 등급 때문에 추가
	char    fee_level[1];                         /* 수수료등급(1)                   */ //수수료 등급 때문에 추가
	char    e_sell[8];                            /* 엔화매도환율(8.4)               */ //평가손익 합계 때문에 추가
	char    d_buy[8];                             /* 달러매수환율(8.4)               */ //평가손익 합계 때문에 추가
	char    qry_cnt[4];                           /* 카운트(4)                       */
} FR_JAN_RQ_OUTPUT1;

typedef struct 
{
	char    acctno[11];                           /* 계좌번호(11)                    */
	char    series[32];                           /* 종목(32)                        */
    char    series_nm[50];                        /* 종목명(50)                      */
    char    crc_cd[3];                            /* 거래통화(3)                     */
    char    buysellsect[4];                       /* 구분(4)                         */
    char    open_q[10];                           /* 미결제수량(10)                  */
    char    rsrb_ableqty[10];                     /* 청산가능수량(10)                */
    char    open_uv[19];                          /* 평균약정가(19)                  */
    char    last_p[19];                           /* 현재가(19)                      */
    char    open_pl[19];                          /* 평가손익(19)                    */
    char    invtrate[19];                         /* 수익율(19)                      */
    char    ctrt_size[19];                        /* 계약단위(19)                    */
    char    dealamt[19];                          /* 체결금액(19)                    */
    char    pospl[19];                            /* 미결제약정평가금액(19)          */
} FR_JAN_RQ_OUTPUT2;

// FX 체결 정보 데이터 조회 구조체
typedef struct
{
    char    szAccNo[14];				/* 계좌번호 */
    char    szAccNoPW[8];				/* 비밀번호 */
    char    szItemCode[16];				/* 종목코드 */
    char    szChGB[1];					/* 체결구분 */
    char    szMMGB[1];					/* 매도/매수 구분 */
	char	gubun[1];					/* 연속구분 */
	char	keyvalue[84];				/* key 일자+시간	: key 일자+시간 */
} FX_CHE_RQ_INPUT;

typedef struct
{
    char    szArrayCnt[4];				/* 입력배열 개수 */
    char    szCurNo[12];				/* 종목명 */
} FX_CHE_RQ_INPUT1;

typedef struct
{
    char    szAccNo[14];				/* 계좌번호 */
    char    szAccNoPW[8];				/* 비밀번호 */
} FX_CHE_RQ_INPUT2;    

typedef struct 
{
	char	nCnt[ 4];					/* 데이터 건수 */
} FX_CHE_RQ_OUTPUT1;

typedef struct
{
    char    szTicketNo[20];				/* 체결번호 */
    char    szCurNo[12];				/* 종목명(Currency) */
    char    szSide[3];					/* 매매구분(Buy/Sell) */
    char    szStatus[1];				/* 상태 */
    char    szRate[10];					/* 주문 가격 */
    char    szQuote[10];				/* 현재가 */
    char    szStop[10];					/* Stop */
    char    szLimit[10];				/* Limit */
    int     nTrailingStop;				/* Trailing Stop */
	double	fTSHighPrice;				/* 현재가 대비 직전 최고가 */
    double  fLot;						/* 수량(Lot) */
    double  fTotalPL;					/* Total P/L (총손익) */
    double  fGrossPL;					/* 일일손익 */
    double  fInterest;					/* 이자 */
    double  fAddInterest;				/* 누적된 이자 */
    double  fPipCost;
    int     nPipLowest;
    char    szOrderTime[30];			/* 거래소 접수 시각(Time) */
    char    szCustItem1[11];			/* 회원처리항목1 */
	char    szCustItem2[14];			/* 회원처리항목2 */
	char    szCustItem3[55];			/* 회원처리항목3 */
    char    szStopItem[80];				/* Stop 취정 회원처리항목 */
    char    szLimitItem[80];			/* Limit 취정 회원처리항목 */
    char    szTSCustItem[80];			/* Trailing Stop 회원처리항목 */
	char	szMemberNo[3];				/* 회원사번호 */
    char    szAccNo[11];				/* 계좌번호 */
    char    szUserName[40];				/* 계좌명 */
} FX_CHE_RQ_OUTPUT2;

// FX 미체결 데이터 조회 구조체
typedef struct
{
    char    szArrayCnt[4];				/* 입력배열 개수 */
} FX_MICHE_RQ_INPUT1;

typedef struct
{
    char    szAccNo[14];				/* 계좌번호*/
    char    szAccNoPW[8];				/* 비밀번호 */
} FX_MICHE_RQ_INPUT2;

typedef struct 
{
	char	nCnt[4];					/* 데이터 건수 */
} FX_MICHE_RQ_OUTPUT1;

typedef struct
{
    double  fLot;						/* 수량(Lot) */
	int		nPipLowest;					/* 유효자리수 */
    char    szCustItem1[11];			/* 회원처리항목1 */
	char	szCustItem2[14];			/* 회원처리항목2 */
	char	szCustItem3[55];			/* 회원처리항목3 */
    char    szCurNo[12];				/* 종목명(Currency) */
    char    szSide[3];					/* 매매구분(Buy/Sell) */
    char    szRate[10];					/* 주문 가격(Price) */
    char    szStop[10];					/* Stop */
    char    szLimit[10];				/* Limit */
	char	szTrailingStop[10];			/* Trailing Stop */
    char    szStatus[1];				/* 주문상태 - 0:미접수, 1:접수 */
    char    szOrderTime[30];			/* 거래소 접수 시각(Time) */
    char    szOrdType[5];				/* 주문 유형 */
	char	szMemberNo[3];				/* 회원사 */
    char    szAccNo[11];				/* 계좌번호 */
    char    szUserName[20];				/* 계좌명 */
} FX_MICHE_RQ_OUTPUT2;

// FX 잔고 데이터 조회 구조체
typedef struct
{
    char    szArrayCnt[4];				/* 입력 계좌 개수 */
} FX_JAN_RQ_INPUT1;

typedef struct
{
    char    szAccNo[14];				/* 계좌번호*/
} FX_JAN_RQ_INPUT2;

typedef struct 
{
	char	nCnt[4];					/* 데이터 건수 */
} FX_JAN_RQ_OUTPUT1;

typedef struct
{
    double  fBalance;					/* 예탁금 잔액 */
    double  fOutstanding;				/* 청산손익 */
    double  fGrossPL;					/* 평가손익 */
    double  fEquity;					/* 평가자산 */
    double  fMarginReq;					/* 주문 증거금 합계 */
    double  fMtMargin;					/* 유지 증거금 합계 */
    double  fUsableEquity;				/* 주문가능금액 */
    double  fUEquityRate;				/* 마진콜비율(%) */
    double  fDepoTotalMSubs;			/* 원화대용평가금액 */
    double  fNowSellRate;				/* 원화대용적용환율(기준매도환율) */
    double  fBalanceWon;				/* 원화총예탁금 */
	double	fTotalWon;					/* 원화환산자산 */
	double	fOrderMargin;				/* 주문 증거금 */
    int     nOutCount;					/* 주문가능수량 */
	int		nOpenSum;					/* 미청산 수량 합계 */
	char	szMemberNo[3];				/* 회원사 번호 */
    char    szAccNo[11];				/* 계좌번호*/
    char    szUserName[40];				/* 계좌명 */
    char    cIsHedge;					/* 헤지여부 */
} FX_JAN_RQ_OUTPUT2;

/*************************************************************************************/
//	실시간 데이터 구조체
/*************************************************************************************/
// 주식체결 실시간
typedef struct
{
	//	유진증권 주식체결 구조체
	char 	type			[ 4];     /* 데이터 Type  [CHE0]        	*/
	char 	gb				[ 1];     /* 데이터 Type  구분자        	*/
	char 	acckey			[11];     /* 계좌번호 키값              	*/
	char 	realkey			[11];     /* 조작자 ID                	*/
    char 	lineseq			[ 8];     /* 라인일련번호             	*/
	char	mdgb			[ 2];		     /* 접속구분					         */
	char 	adminno			[ 8];     /* 관리자사번                       */
	char	dsize			[ 5];			 /* Data size (헤더길이 제외)			 */
	char	usrfld			[15];		     /* 사용자정의영역(현재미사용)	         */
	char	or_type			[ 2];	  /* 주문유형코드					  */
	char	prgb			[ 2];	 /* 주문체결구분					 */
									 /*"10":원주문 "11":정정주문         */
									 /*"12":취소주문                     */ 
	char 	accno			[11];     /* 계좌번호                         */
	char 	orderno			[ 10];     /* 주문번호                         */
	char 	orgorderno		[10];     /* 원주문번호                       */
	char 	cheno			[10];     /* 체결번호                         */

	char 	jmcode			[12];     /* 종목코드                         */
	char 	jmname			[40];     /* 종목명                           */
	char 	bid_tpcd			[ 2];     /* 주문상태                         */
	char 	ord_cond         	[ 1];    /* 주문조건	                     */
	char    cdgb			[ 3];	  /*  신용구분                        */
	char 	ord_tpcd			[ 2];     /* 주문구분                         */
	char 	ord_mkcd			[ 2];     /* 주문업무분류                     */
	char 	medosu			[ 1];     /* 매도수구분 ( 1:매도, 2:매수 )    */
		
	char 	jprc			[13];     /* 주문가격                         */
	char 	jqty			[16];     /* 주문수량                         */
	char 	cheprice		[13];     /* 체결가                           */
	char 	cheqty			[16];     /* 체결량                           */
	
	char 	sumprice		[16];     /* 체결누계금액                     */
	char 	mecheqty		[16];     /* 미체결수량                       */
	char 	percheprice		[13];     /* 단위체결가                       */
	char 	percheqty		[16];     /* 단위체결량                       */
	
	char 	chetime			[ 8];     /* 주문/체결 시간                   */

	/* 원주문 DATA부 */
    char    og_orno             [10];    /* 원주문-주문번호                  */
    char    og_opct_qt          [16];    /* 원주문-미체결수량                */

//	char 	price			[ 9];     /* 현재가                           */
//	char 	sign			[ 1];     /* 전일대비부호                     */
//	char 	medoga			[ 9];     /* (최우선)매도호가                 */
//	char 	mesuga			[ 9];     /* (최우선)매수호가                 */
//	char 	memefee			[16];     /* 당일매매 수수료                  */
//	char 	memetax			[16];     /* 당일매매세금                     */
//	char	order_amt		[13];	 /*  주문가능현금                    */
//	char	order_dae		[13];	 /*  주문가능대용                    */
//	char	measu_amt		[13];	 /*  재매수가능금액                  */
//	char	add_amt			[13];	 /*  추가담보현금                    */

}S_CHE_REAL_OUTREC;

/*****************************************************************************/
/* 주식주문체결 (거부)                        							     */
/*****************************************************************************/
typedef struct
{
    //	유진증권 주식체결 구조체
	char 	type			[ 4];     /* 데이터 Type  [CHE0]        	*/
	char 	gb				[ 1];     /* 데이터 Type  구분자        	*/
	char 	acckey			[11];     /* 계좌번호 키값              	*/
	char 	realkey			[11];     /* 조작자 ID                	*/
    char 	lineseq			[ 8];     /* 라인일련번호             	*/
	char	mdgb			[ 2];		     /* 접속구분					         */
	char 	adminno			[ 8];     /* 관리자사번                       */
	char	dsize			[ 5];			 /* Data size (헤더길이 제외)			 */
	char	usrfld			[15];		     /* 사용자정의영역(현재미사용)	         */
	char	or_type			[ 2];	  /* 주문유형코드					  */
    char	prgb			[ 2];	 /* 주문체결구분					 */
									 /*"10":원주문 "11":정정주문         */
									 /*"12":취소주문                     */ 
	char 	accno			[11];     /* 계좌번호                         */
	char 	orderno			[ 10];     /* 주문번호                         */
	char 	orgorderno		[10];     /* 원주문번호                       */
	char 	cheno			[10];     /* 체결번호                         */

	char 	jmcode			[12];     /* 종목코드                         */
	char 	jmname			[40];     /* 종목명                           */
	char 	bid_tpcd			[ 2];     /* 주문상태                         */
	char 	ord_cond         	[ 1];    /* 주문조건	                     */
	char    cdgb			[ 3];	  /*  신용구분                        */
	char 	ord_tpcd			[ 2];     /* 주문구분                         */
	char 	ord_mkcd			[ 2];     /* 주문업무분류                     */
	char 	medosu			[ 1];     /* 매도수구분 ( 1:매도, 2:매수 )    */
		
	char 	jprc			[13];     /* 주문가격                         */
	char 	jqty			[16];     /* 주문수량                         */
	char 	cheprice		[13];     /* 체결가                           */
	char 	cheqty			[16];     /* 체결량                           */
	
	char 	sumprice		[16];     /* 체결누계금액                     */
	char 	mecheqty		[16];     /* 미체결수량                       */
	char 	percheprice		[13];     /* 단위체결가                       */
	char 	percheqty		[16];     /* 단위체결량                       */
	
	char 	chetime			[ 8];     /* 주문/체결 시간                   */
    /* 원주문 DATA부 */
    char    og_orno             [10];    /* 원주문_주문번호                  */
	char    og_or_type          [ 2];    /* 원주문_주문유형   	             */
    char    og_or_ctgb          [ 2];    /* 원주문_주문체결구분              */
    char    og_orgn_orno        [10];    /* 원주문_원주문번호                */
    char    og_ctno             [10];    /* 원주문_체결번호                  */
    char    og_bid_tpcd         [ 2];    /* 원주문_호가유형코드              */
    char    og_ord_cond         [ 1];    /* 원주문_주문조건                  */
    char    og_cdgb             [ 3];    /* 원주문_신용구분                  */
    char    og_ord_tpcd         [ 2];    /* 원주문_주문유형코드              */
    char    og_ord_mkcd         [ 2];    /* 원주문_주문시장코드              */
    char    og_degb             [ 1];    /* 원주문_매매구분 [매도:1/매수:2]  */
    char    og_ord_ucs          [13];    /* 원주문_주문가격                  */
    char    og_orqt             [16];    /* 원주문_주문수량                  */
    char    og_crt_ucs          [13];    /* 원주문_체결가                    */
    char    og_ctqt             [16];    /* 원주문_체결량                    */
    char    og_ctam             [16];    /* 원주문_체결누계금액              */
    char    og_opct_qt          [16];    /* 원주문_미체결수량                */
    char    og_per_crt_ucs      [13];    /* 원주문_단위체결가                */
    char    og_per_ctqt         [16];    /* 원주문_단위체결량                */
    char    og_actm             [ 8];    /* 원주문_주문/체결 시간            */

}S_TREFZ;

// 주식잔고 실시간
typedef struct
{
	char 	type			[ 4];     /* 데이터 Type  [CHE0]        	*/
	char 	gb				[ 1];     /* 데이터 Type  구분자        	*/
	char 	acckey			[11];     /* 계좌번호 키값              	*/
	char 	realkey			[11];     /* 조작자 ID                	*/
    char 	lineseq			[ 8];     /* 라인일련번호             	*/
	char	mdgb			[ 2];		     /* 접속구분					         */
	char 	adminno			[ 8];     /* 관리자사번                       */
	char	dsize			[ 5];			 /* Data size (헤더길이 제외)			 */
	char	usrfld			[15];		     /* 사용자정의영역(현재미사용)	         */
	char    accno			[11];  /*  계좌번호                        */
    char    jmcode			[12];  /*  종목코드                        */
	char    jmname			[40];  /*  종목명                          */
    char    sygb			[ 3];  /*  신용구분                        */
	char    bqty			[16];  /*  보유수량                        */
	char    ordgaqty		[16];  /*  주문가능 수량                   */
	char    buyamt			[13];  /*  매입단가                        */
	char    bep_price		[13];  /*  BEP 단가                        */
	char    menusum			[16];  /*  매입금액                        */
	char    fee				[16];  /*  수수료                          */
    char    tax				[16];  /*  세금                            */
	char    holdrate		[ 6];  /*  보유비중                        */
    char    sdate			[ 8];  /*  대출일                          */
    char    lastday			[ 8];  /*  만기일                          */
	char    sinamt			[16];  /*  신용금액                        */
    char    sininter		[16];  /*  신용이자                        */
	char    janqty			[16];  /*  결제잔고                        */
	char    psmevol			[16];  /*  전일순매수량                    */
    char    tsmevol			[16];  /*  당일 순매수량                   */
	char    damboloanamt	[16];  /*  담보대출수량                    */
	char    tmedosonik		[16];  /*  당일 총 매도손익                */
	char    pertrsonik		[16];  /*  당일실현손익변동분              */
	char    trsonik			[16];  /*  당일실현손익(유가)              */
    char    ratesonik		[16];  /*  당일실현손익률(유가)            */
    char    strsonik		[16];  /*  당일실현손익(신용)              */
    char    sratesonik		[16];  /*  당일실현손익률(신용)            */
	char 	medoamtsum		[16];  /* 매도금액합                       */
	char 	mesuamtsum		[16];  /* 매수금액합                       */

    
}S_JAN_REAL_OUTREC;


// 선물옵션체결 실시간
typedef struct
{
	char 	type			[ 4];     /* 데이터 Type  [CHE0]        	*/
	char 	gb				[ 1];     /* 데이터 Type  구분자        	*/
	char 	acckey			[11];     /* 계좌번호 키값              	*/
	char 	realkey			[11];     /* 조작자 ID                	*/
    char 	lineseq			[ 8];     /* 라인일련번호             	*/
	char	mdgb			[ 2];		     /* 접속구분					         */
	char 	adminno			[ 8];     /* 관리자사번                       */
	char	dsize			[ 5];			 /* Data size (헤더길이 제외)			 */
	char	usrfld			[15];		     /* 사용자정의영역(현재미사용)	         */
	char	or_type			[ 2];	  /* 주문유형코드					  */
	char	prgb			[ 2];	 /* 주문체결구분					 */
									 /*"10":원주문 "11":정정주문         */
									 /*"12":취소주문                     */ 
	char 	accno			[11];     /* 계좌번호                         */
	char 	orderno			[ 10];     /* 주문번호                         */
	char 	orgorderno		[10];     /* 원주문번호                       */
	char 	cheno			[10];     /* 체결번호                         */

	char 	jmcode			[12];     /* 종목코드                         */
	char 	jmname			[40];     /* 종목명                           */
	char 	bid_tpcd			[ 2];     /* 호가유형코드                         */
	char 	ord_cond         	[ 1];    /* 주문조건	                     */
	char 	ord_tpcd			[ 2];     /* 주문유형코드                         */
	char 	ord_mkcd			[ 2];     /* 주문시장코드                     */
	char 	medosu			[ 1];     /* 매도수구분 ( 1:매도, 2:매수 )    */
		
	char 	jprc			[13];     /* 주문가격                         */
	char 	jqty			[16];     /* 주문수량                         */
	char 	cheprice		[13];     /* 체결가                           */
	char 	cheqty			[16];     /* 체결량                           */
	
	char 	sumprice		[16];     /* 체결누계금액                     */
	char 	mecheqty		[16];     /* 미체결수량                       */
	char 	percheprice		[13];     /* 단위체결가                       */
	char 	percheqty		[16];     /* 단위체결량                       */
	
	char 	chetime			[ 8];     /* 주문/체결 시간                   */

	/* 원주문 DATA부 */
    char    og_orno             [10];    /* 원주문-주문번호                  */
    char    og_opct_qt          [16];    /* 원주문-미체결수량                */

}FO_CHE_REAL_OUTREC;

/* FX 잔고 데이터 실시간 */
typedef struct
{
    char    szKeyType[4];				/* ACCN */
	char	szKeyItem[11];				/* Key	*/
	char	szKeyItem1[11];				/* Key1	- 주문자 ID */
	char	szKeyItem2[11];				/* Key2	- 관리자 ID */
    char    szAccNo[11];				/* 계좌 번호 */
    char    szBalanceUSD[16];			/* USD잔고      */
    char    szBalanceKRW[16];			/* KRW잔고      */
    char    szSubsUSD[16];				/* 원화대용USD  */
    char    szSubsKRW[16];				/* 원화대용KRW  */
    char    szRate[9];					/* 적용환율     */
} FX_JAN_REAL_OUTREC;

/* FX 계좌정보 데이터 실시간 */
typedef struct
{
    char    szKeyType[4];				/* ACCM */
	char	szKeyItem[11];				/* Key	*/
	char	szKeyItem1[11];				/* Key1	- 주문자 ID */
	char	szKeyItem2[11];				/* Key2	- 관리자 ID */
    char    szAccNo[11];				/* 계좌 번호) */
    char    szMarginReq[12];			/* FX 증거금 */
    char    szMtMargin[12];				/* 유지증거금 */
    char    szOrderMg[10];				/* 주문증거금   */
    char    szIsHedge[1];				/* 헤지여부     */
} FX_DET_REAL_OUTREC;

/* FX 체결 실시간 */
typedef struct  
{
	char	szKeyType[4];				/*  [CH01]						*/
	char	szKeyItem[11];				/*  Key		- 계자번호			*/
	char	szKeyItem1[11];				/*  Key1	- 주문자 ID			*/
	char	szKeyItem2[11];				/*  Key2	- 관리자 ID			*/
	char	szAccNo[11];				/*	[계좌번호]					*/
	char	szMemberNo[3];				/*  [회원사ID]					*/
	char	szClOrdID[80];				/*  [주문 접수번호]				*/
	char	szNoticeNo[20];				/*  [통지번호]					*/
	char	szOrdType[5];				/*  [주문유형]					*/
	char	SHCODE[12];					/*  [종목명]					*/
	char	szOrderID[20];				/*  [거래소 접수번호]			*/
	char	szPGCode[2];				/*  [투자자구분]				*/
	char	szRate[10];					/*  [약정지수]					*/
	char	szQuote[10];				/*  [현재가]					*/
	char	szExeTime[30];				/*  [약정시각]					*/
	char	szOrgCustItem1[11];			/*  [주문시 회원처리항목1]		*/
	char	szOrgCustItem2[14];			/*  [주문시 회원처리항목2]		*/
	char	szOrgCustItem3[55];			/*  [주문시 회원처리항목3]		*/
	char	szCustItem1[11];			/*  [회원처리항목 1]			*/
	char	szCustItem2[14];			/*  [회원처리항목 2]			*/
	char	szCustItem3[55];			/*  [회원처리항목 3]			*/
	char	szSide[3];					/*  [매매구분]					*/
	char	szOrgTicketNo[20];			/*  [원체결번호]				*/
	char	szOpenTime[30];				/*  [신규주문체결시간(전환매)]	*/
	char	szAccName[20];				/*  [계좌명]					*/
	char	szUserID[20];				/*  [고객ID]					*/
	char	szFXCMPosID[20];			/*  [TicketNo]					*/
    char    szCommition[12];			/* 수수료 */
    char    szAmount[12];				/* 약정수량 */
    char    szTotalPL[12];				/* 핍손익 */
    char    szGrossPL[12];				/* 일일손익(가격으로 환산된 손익) */
    char    szRevRate[12];				/* 체결시 반대지수 */
    char    szOpenMarkup[12];			/* OM (전환매시 사용) */
    char    szInterest[12];				/* 이자 */
    char    szCloseMarkup[12];			/* CM (전환매시 사용) */
    char    szMtMargin[12];				/* 유지증거금 (포지션수 * 주문증거금) */
    char    szOrderMargin[12];			/* 주문증거금 (주문수 * 주문증거금) */
    char    szOutBalance[16];			/* 미결제금액 */
    char    szPipCost[12];				/* 체결시 Pip Cost */
	char	szPipLowest[4];				/* 소수점 유효자리수 */
} FX_CHE_REAL_OUTREC;

/* FX 미체결 실시간 */
typedef struct  
{
	char	szKeyType[4];				/*  [OR01]						*/
	char	szKeyItem[11];				/*  Key		- 계좌번호			*/
	char	szKeyItem1[11];				/*  Key1	- 주문자 ID			*/
	char	szKeyItem2[11];				/*  Key2	- 관리자 ID			*/
	char	szAccNo[11];				/*  [계좌번호]					*/
    char    szAmount[12];				/*	수량						*/
    char    szMtMargin[12];				/*	유지증거금					*/
    char    szOrderMargin[12];			/*	주문증거금					*/
	char	szStatus[1];				/*  [상태 - 0:미접수, 1:접수]   */
	char	SHCODE[12];					/*  [종목명]					*/
	char	szSide[3];					/*  [매매구분]					*/
	char	szOrdType[5];				/*  [주문유형]					*/
	char	szRate[10];					/*  [주문환율]					*/
	char	szOpenTime[30];				/*  [주문시각]					*/
	char	szCustItem1[11];			/*  [회원처리항목 1]			*/
	char	szCustItem2[14];			/*  [회원처리항목 2]			*/
	char	szCustItem3[55];			/*  [회원처리항목 3]			*/
	char	szOrgCustItem1[11];			/*  [취정시 회원처리항목1]		*/
	char	szOrgCustItem2[14];			/*  [취정시 회원처리항목2]		*/
	char	szOrgCustItem3[55];			/*  [취정시 회원처리항목3]		*/
	char	szPipLowest[4];				/* 소수점 유효자리수			*/
} FX_MIC_REAL_OUTREC;

/* FX StopLimit 실시간 */
typedef struct  
{
	char	szKeyType[4];				/*  [CLSL]						*/
	char	szKeyItem[11];				/*  Key		- 계좌번호			*/
	char	szKeyItem1[11];				/*  Key1	- 주문자 ID			*/
	char	szKeyItem2[11];				/*  Key2	- 관리자 ID			*/
	char	szAccNo[11];				/*  [계좌번호]					*/
	char    szAmount[12];				/*  [수량]						*/
	char	szStatus[1];				/*  [주문 접수,체결 상태]		*/
	char	szStop[10];					/*  [Stop]						*/
	char	szLimit[10];				/*  [Limit]						*/
	char	szOrgCustItem1[11];			/*  [원주문회원처리항목 1]		*/
	char	szOrgCustItem2[14];			/*  [원주문회원처리항목 2]		*/
	char	szOrgCustItem3[55];			/*  [원주문회원처리항목 3]		*/
	char	szCustItem1[11];			/*  [SL 회원처리항목 1]			*/
	char	szCustItem2[14];			/*  [SL 회원처리항목 2]			*/
	char	szCustItem3[55];			/*  [SL 회원처리항목 3]			*/
	char	SHCODE[12];					/*  [종목명]					*/
	char	szSide[3];					/*  [매매구분]					*/
	char	szOrdType[5];				/*  [주문유형]					*/
	char	szOpenTime[30];				/*  [주문시각]					*/
	char	szTicketNo[20];				/*  [체결번호]					*/
} FX_CSL_REAL_OUTREC;

/* FX IFO 실시간 */
typedef struct  
{
	char	szKeyType[4];				/* [FIFO]						*/
	char	szKeyItem[11];				/* Key	- 계좌번호				*/
	char	szKeyItem1[11];				/* Key1	- 주문자 ID				*/
	char	szKeyItem2[11];				/* Key2	- 관리자 ID				*/
	char	szAccNo[11];				/* 계좌번호						*/
	char	szEntry[10];				/* Entry Price					*/
	char	szStop[10];					/* Stop  Price					*/
	char	szLimit[10];				/* Limit Price					*/
	char	szTrailingStop[10];			/* TrailingStop Price			*/
	char	szOrgCustItem1[11];			/* 원회원처리항목1				*/
	char	szOrgCustItem2[14];			/* 원회원처리항목2				*/
	char	szOrgCustItem3[55];			/* 원회원처리항목3				*/
	char	szEntryItem1[11];			/* 회원처리항목1				*/
	char	szEntryItem2[14];			/* 회원처리항목2				*/
	char	szEntryItem3[55];			/* 회원처리항목3				*/
	char	szStatus[1];				/* 상태							*/
} FX_IFO_REAL_OUTREC;

/* FX 거부 실시간 */
typedef struct
{
	char	szKeyType[4];				/*  [REJE]						*/
	char	szKeyItem[11];				/*  Key		- 계좌번호			*/
	char	szKeyItem1[11];				/*  Key1	- 주문자 ID			*/
	char	szKeyItem2[11];				/*  Key2	- 관리자 ID			*/
	char	szAccNo[11];				/*  [계좌번호]					*/
    char    szAmount[12];			    /*  약정수량					*/
    char    szMtMargin[12];				/*  유지증거금					*/
    char    szOrderMargin[12];			/*  주문증거금					*/
	char	szCustItem1[11];			/*  [회원처리항목 1]			*/
	char	szCustItem2[14];			/*  [회원처리항목 2]			*/
	char	szCustItem3[55];			/*  [회원처리항목 3]			*/
	char	SHCODE[12];					/*  [종목명]					*/
	char	szSide[3];					/*  [매매구분]					*/
	char	szRate[10];					/*  [약정지수]					*/
	char	szOrdType[5];				/*  [주문유형]					*/
	char	szRejectTime[30];			/*  [거부시각]					*/
} FX_REJ_REAL_OUTREC;

/* FX 실시간 시세 */
typedef struct  
{
	char		KEYTYPE[4];				/*  [9212]						*/
	char		SHCODE[12];				/*  [종목코드]					*/
	char		MEMBER_NO[3];			/*  [회원사 ID]					*/
	char		TRADE_TYPE[2];			/*  [종목구분]					*/
	char		REAL_TIME[14];			/*  [시간]						*/
	char		BID[10];				/*  [현재가(BID) : 현재가(BID)] */
	char		BIDOPEN_PRICE[10];		/*  [시가(BID)]					*/
	char		BIDHIGH_PRICE[10];		/*  [고가(BID)]					*/
	char		BIDLOW_PRICE[10];		/*  [저가(BID)]					*/
	char		BIDCLOSE_PRICE[10];		/*  [종가(BID)]					*/
	char		BID_SIGN[1];			/*  [등락가 부호(BID)]			*/
	char		BID_CHANGE_PRICE[10];	/*  [등락가(BID)]				*/
	char		OFFER[10];				/*  [현재가(OFFER)]				*/
	char		OFFEROPEN_PRICE[10];	/*  [시가(OFFER)]				*/
	char		OFFERHIGH_PRICE[10];	/*  [고가(OFFER)]				*/
	char		OFFERLOW_PRICE[10];		/*  [저가(OFFER)]				*/
	char		OFFERCLOSE_PRICE[10];	/*  [종가(OFFER)]				*/
	char		OFFER_SIGN[1];			/*  [등락가 부호(OFFER)]		*/
	char		OFFER_CHANGE_PRICE[10];	/*  [등락가(OFFER)]				*/
	char		PIP_COST[10];			/*  [핍코스트]					*/
	char		PIP_LOWEST[3];			/*  [유효자리수]				*/
	char		INTEREST_BUY[10];		/*  [이자(Buy)]					*/
	char		INTEREST_SELL[10];		/*  [이자(Sell)]				*/
	char		QUOTE_ID[80];			/*  [시세 고유번호]				*/
} FX_QUOTE_REAL_OUTREC;

// 선물옵션 잔고 실시간
typedef struct
{
	char 	type			[ 4];     /* 데이터 Type  [CHE0]        	*/
	char 	gb				[ 1];     /* 데이터 Type  구분자        	*/
	char 	acckey			[11];     /* 계좌번호 키값              	*/
	char 	realkey			[11];     /* 조작자 ID                	*/
    char 	lineseq			[ 8];     /* 라인일련번호             	*/
	char	mdgb			[ 2];		     /* 접속구분					         */
	char 	adminno			[ 8];     /* 관리자사번                       */
	char	dsize			[ 5];			 /* Data size (헤더길이 제외)			 */
	char	usrfld			[15];		     /* 사용자정의영역(현재미사용)	         */
	char    accno         [11];		/*  계좌번호                        */
    char    jmcode        [12];     /*  종목코드  "XXXXXXXX"            */
    char    jmname        [40];     /*  종목명                          */
	char    bqty          [16];		/*  보유수량                        */
									/*  없으면 ' '                      */
	char    buyamt        [13];		/*  평균매입지수                    */
									/*  평균매입지수/100                */
    char    mesusum       [16];		/*  총매입금액                      */
									/*  총매입금액/1000+0.5             */
    char    ordgaqty      [16];		/*  청산가능 수량                   */
    char    tsmevol       [16];		/*  당일순매수량                    */
									/*  당일매수수량-당일매도수량       */
    char    medosu        [ 1];		/*  매도/매수 구분                  */
	char    fee           [16];		/*  수수료                          */
    char    tax           [16];		/*  세금                            */
	char    preqty        [16];		/*  전일보유수량		            */
	char    pertrsonik    [16];     /*  당일실현손익변동분              */
	char 	medoamtsum    [16];  /* 매도금액합                       */
	char 	mesuamtsum    [16];  /* 매수금액합                       */

}FO_JAN_REAL_OUTREC;

// 선물옵션 잔고 실시간
typedef struct
{
	char 	type          [ 4];		/*	데이터 Type                      */
	char 	gubun         [ 1];		/*	데이터 Gb                        */
	char 	acckey		  [11];     /*	계좌번호 키값					*/
	char 	realkey       [16];		/*	조작자 ID                        */
    char 	lineseq       [ 8];		/*	라인일련번호                     */
	char	order_amt	  [13];	    /*  주문가능현금                    */
	char	order_dae	  [13];	    /*  주문가능대용                    */
	char	measu_amt	  [13];	    /*  재매수가능금액                  */
	char	add_amt		  [13];	    /*  추가담보현금                    */
}AMT_REAL_OUTREC;


// 해외선물 체결 실시간
typedef struct
{
	char		push_div[4];		/*  [실시간 구분 FEXC]     */
	char		push_key[11];		/*  [Key]     */
	char		push_key1[11];		/*  [Key1 : 주문자ID]     */
	char		push_key2[11];		/*  [Key2 : 관리자ID]     */
	char		pkt_div[4];			/*  [패킷구분]     */
	char		acct_no[11];		/*  [계좌번호]     */
	char		acct_nm[50];		/*  [계좌명]     */
	char		ord_no[5];			/*  [주문번호]     */
	char		orig_ord_no[5];		/*  [원주문번호]     */
	char		series[32];			/*  [종목코드]     */
	char		bysl_tp[1];			/*  [매매구분]     */
	char		proc_stat[1];		/*  [주문처리상태]     */
	char		ord_q[10];			/*  [주문수량]     */
	char		trd_q[10];			/*  [체결수량]     */
	char		ord_remq[10];		/*  [잔량]     */
	char		ord_p[19];			/*  [주문가격]     */
	char		trd_p[19];			/*  [체결가격]     */
	char		mdfy_cncl_q[10];	/*  [정정취소]     */
	char		prce_tp[1];			/*  [가격구분]     */
	char		stop_p[19];			/*  [조건가격]     */
	char		ordtype[1];			/*  [주문유형]     */
	char		ord_tp[1];			/*  [주문구분]     */
	char		acpt_time[10];		/*  [거래소접수시간]     */
	char		kr_ord_date[8];		/*  [한국주문일자]     */
	char		kr_acpt_time[10];	/*  [한국접수시간]     */
	char		kr_che_time[10];	/*  [한국체결시간]     */
	char		ord_way_tp[1];		/*  [매체구분]     */
	char		work_empl[9];		/*  [사용자ID]     */
	char		crc_cd[3];			/*  [통화]     */

}FR_CHE_REAL_OUTREC;

// 해외선물 잔고 실시간
typedef struct
{
	char		push_div[4];		/*  [실시간 구분 FOPN]     */
	char		push_key[11];		/*  [Key]     */
	char		push_key1[11];		/*  [Key1 : 주문자ID]     */
	char		push_key2[11];		/*  [Key2 : 관리자ID]     */
	char		acct_no[11];        /* 계좌번호             */
    char		acct_nm[50];        /* 계좌명               */
	char		t_pospl[19];		/*  [미결제약정평가금액]     */
	char		t_open_pl[19];		/*  [평가손익]     */
	char		t_invtrate[19];		/*  [수익율]     */
	char		t_real_pl[19];		/*  [실현손익]     */
	char		t_pl[19];			/*  [총손익]     */
	char		t_cmpr_pl[19];		/*  [전일정산대비]     */
	char		t_pres_asset[19];	/*  [추정자산]     */
	char		series[32];			/*  [종목]     */
	char		series_nm[50];		/*  [종목명]     */
	char		crc_cd[3];			/*  [거래통화]     */
	char		buysellsect[4];		/*  [구분]     */
	char		open_q[10];			/*  [미결제수량]     */
	char		rsrb_ableqty[10];	/*  [청산가능수량]     */
	char		open_uv[19];		/*  [평균약정가]     */
	char		last_p[19];			/*  [현재가]     */
	char		open_pl[19];		/*  [평가손익]     */
	char		invtrate[19];		/*  [수익율]     */
	char		ctrt_size[19];		/*  [계약단위]     */
	char		dealamt[19];		/*  [체결금액]     */
	char		pospl[19];			/*  [미결제약정평가금액]     */

}FR_JAN_REAL_OUTREC;

/*************************************************************************************/
//	잔고 데이터 저장 구조체
/*************************************************************************************/
//	계좌 데이터 구조체
typedef struct 
{
	char	accNo			[11 + 1];		// 계좌번호
	char	accName			[40 + 1];		// 계좌명
	char	accPasswd		[ 8 + 1];		// 비밀번호
	char	accdetlgb		[ 2 + 1];		// 계좌상세유형
	BYTE	accgb;						// 계좌구분 ['1': 위임, '2': 수임, '0':해당사항없음 ]	
	BYTE	recordtp;					// 비밀번호저장여부(1:저장, 0:미저장)
	char	taccno			[11 + 1];		// 종합계좌번호
	char	fee1			[ 8 + 1];		// 수수료 1 [default 는 스페이스 처리]
	char	fee2			[ 8 + 1];		// 수수료 2 [default 는 스페이스 처리]	
	char	fee3			[ 8 + 1];		// 수수료 3 [default 는 스페이스 처리]	
	char	fee4			[ 8 + 1];		// 수수료 4 [default 는 스페이스 처리]	
	char	accPenName		[30 + 1];		// 계좌명 (필명)
	char	accOrgPasswd	[ 8 + 1];		// 원본 비밀번호
	BYTE	realdatatp;					// 주문/체결 리얼 데이타 처리구분 (1:처리, 0:미처리)
	char	filler			[10    ];		// 
	char	order_amt		[13 + 1];		// 주문가능현금(KTB 사용안함)
	char	order_dae		[13 + 1];		// 주문가능대용(KTB 사용안함)
	char	measu_amt		[13 + 1];		// 재매수가능금액(KTB 사용안함)
	char	add_amt			[13 + 1];		// 추가담보현금(KTB 사용안함)
	char	credit			[13 + 1];		// 융자한도(KTB 사용안함)
	char	acc_amt			[ 1 + 1];		// 계좌증거금(KTB 사용안함)
	char    trsonik			[16 + 1];		/*  당일 실현손익              */
    char    sonik_fee		[16 + 1];		/*  실현손익에 대한 수수료					*/
    char    sonik_tax		[16 + 1];		/*  실현손익에 대한 세금					*/

//	char	managegb		[ 1 + 1];		// 체잔에서 관리할 계좌인지 구분... ( 서버에서 받은 데이터는 관리하고... 따로 화면에서 조회한 계좌는 화면이 사라지면 없애버린다... ㅋㅋ )
//											// 0 : 비관리 계좌, 1 : 관리계좌

	char    t_real_pl		[19 + 1];       /* 해외실현손익(19)                    */
    char    t_cmpr_pl		[19 + 1];       /* 해외전일정산대비(19)                */
    char    t_pres_asset	[19 + 1];       /* 해외추정자산(19)                    */
	char    fee_level		[ 1 + 1];		/* 해외선물 수수료등급(1)			   */
	char    e_sell			[ 8 + 1];       /* 엔화매도환율(8.4)				   */
	char    d_buy			[ 8 + 1];       /* 달러매수환율(8.4)                   */

	CObArray arrJan;
	CObArray arrChe;
	CObArray arrFXPos;
	CObArray arrFrJan;
	CObArray arrFrChe;
} CHEJAN_ACC_STRUCT;

//	주식 잔고 구조체
typedef struct 
{
	char 	type          [ 4 + 1 ];  /* 데이터 Type                      */
	char 	gubun         [ 1 + 1 ];  /* 데이터 Gb                        */
	char 	realkey       [11 + 1 ];  /* 조작자 ID                        */
    char 	lineseq       [ 8 + 1 ];  /* 라인일련번호                     */
	char    accno         [11 + 1 ];  /*  계좌번호                        */
    char    jmcode        [12 + 1 ];  /*  종목코드                        */
	char    jmname        [40 + 1 ];  /*  종목명                          */
    char    sygb          [ 3 + 1 ];  /*  신용구분                        */
	char    bqty          [16 + 1 ];  /*  보유수량                        */
	char    ordgaqty      [16 + 1 ];  /*  주문가능 수량                   */
	char    buyamt        [13 + 1 ];  /*  매입단가                        */
	char    bep_price     [13 + 1 ];  /*  BEP 단가                        */
	char    menusum       [16 + 1 ];  /*  매입금액                        */
    char    estamt        [16 + 1 ];  /*  평가금액                        */
	char    estsonik      [16 + 1 ];  /*  평가손익                        */
    char    rate          [ 8 + 1 ];  /*  수익률                          */
//	char    fee           [16 + 1 ];  /*  수수료                          */		//	클라이언트에서 계산해서 사용
//	char    tax           [16 + 1 ];  /*  세금                            */		//	클라이언트에서 계산해서 사용
	char    holdrate      [ 6 + 1 ];  /*  보유비중                        */
    char    sdate         [ 8 + 1 ];  /*  대출일                          */
    char    lastday       [ 8 + 1 ];  /*  만기일                          */
    char    sinamt        [16 + 1 ];  /*  신용금액                        */
    char    sininter      [16 + 1 ];  /*  신용이자                        */
    char    janqty        [16 + 1 ];  /*  결제잔고                        */
    char    psmevol       [16 + 1 ];  /*  전일순매수량                    */
    char    tsmevol       [16 + 1 ];  /*  당일 순매수량                   */
	char    damboloanamt  [16 + 1 ];  /*  담보대출수량                    */
	char    tmedosonik    [16 + 1 ];  /*  당일 총 매도손익                */
	char    trsonik       [16 + 1 ];  /*  당일실현손익(유가)              */
    char    ratesonik     [16 + 1 ];  /*  당일실현손익률(유가)            */
    char    strsonik      [16 + 1 ];  /*  당일실현손익(신용)              */
    char    sratesonik    [16 + 1 ];  /*  당일실현손익률(신용)            */
	char    pjogaest      [16 + 1 ];  /*  전일종가 전일평가               */
	char    pjogasonik    [16 + 1 ];  /*  전일종가 기준 평가손익          */
    char    pjogarate     [16 + 1 ];  /*  전일종가 기준 수익률            */
	char 	medoamtsum    [16 + 1 ];  /* 매도금액합                       */
	char 	mesuamtsum    [16 + 1 ];  /* 매수금액합                       */
	char    giga          [ 9 + 1 ];  /*  기준가                          */
	char    uplmtprice    [ 9 + 1 ];  /*  상한가                          */
	char    dnlmtprice    [ 9 + 1 ];  /*  하한가                          */
	char    price         [ 9+ 1 ];   /*  현재가                          */
	char    mesuga        [ 9 + 1 ];  /*  (최우선)매수호가                */
	char    medoga        [ 9 + 1 ];  /*  (최우선)매도호가                */
	char    sign          [ 1 + 1 ];  /*  전일구분                        */
	char    change        [ 8 + 1 ];  /*  전일대비                        */
    char    diff          [ 6 + 1 ];  /*  등락률 ( 소수점 2자리 )         */
    char    volume        [16 + 1 ];  /*  거래량                          */


} S_JAN_STRUCT;

//	주식 체결 구조체
typedef struct 
{
	char	or_type	   [ 2 + 1 ];				/* 주문유형코드					  */
	char	prgb       [ 2 + 1 ];				/* 주문체결구분						*/
	char 	accno      [11 + 1 ];				/* 계좌번호                         */
	char 	orderno    [10 + 1 ];				/* 주문번호                         */
	char 	orgorderno [10 + 1 ];				/* 원주문번호                       */
	char 	cheno      [10 + 1 ];				/* 체결번호                         */
	char 	jmcode     [12 + 1 ];				/* 종목코드                         */
	char 	jmname     [40 + 1 ];				/* 종목명                           */
	char 	bid_tpcd   [ 2 + 1 ];				/* 호가유형코드                     */
	char 	ord_cond   [ 1 + 1 ];				/* 주문조건                         */
	char 	cdgb	   [ 3 + 1 ];				/* 신용구분                         */
	char 	ord_tpcd   [ 2 + 1 ];				/* 주문유형코드                     */
	char 	ord_mkcd   [ 2 + 1 ];				/* 주문시장코드                     */
	char 	medosu     [ 1 + 1 ];				/* 매도수구분 ( 1:매도, 2:매수 )    */
	char 	jprc       [13 + 1 ];				/* 주문가격                         */
	char 	jqty       [16 + 1 ];				/* 주문수량                         */
	char 	cheprice   [13 + 1 ];				/* 체결가                           */
	char 	cheqty     [16 + 1 ];				/* 체결량                           */
	char 	sumprice   [16 + 1 ];				/* 체결누계금액                     */
	char 	mecheqty   [16 + 1 ];				/* 미체결수량                       */
	char 	chetime    [ 8 + 1 ];				/* 주문시간							*/
	char 	price      [ 9 + 1 ];				/* 현재가                           */
	char 	sign       [ 1 + 1 ];				/* 전일대비부호                     */
	char 	medoga     [ 9 + 1 ];				/* (최우선)매도호가                 */
	char 	mesuga     [ 9 + 1 ];				/* (최우선)매수호가                 */
//	char 	percheprice[ 9 + 1 ];				/* 단위체결가                       */
//	char 	percheqty  [ 9 + 1 ];				/* 단위체결량                       */
//	char 	memefee    [16 + 1 ];				/* 당일매매 수수료                  */
//	char 	memetax    [16 + 1 ];				/* 당일매매세금                     */
} S_CHE_STRUCT;

// 선물옵션체결 구조체
typedef struct
{
	char 	type			[ 4 + 1 ];		/* 데이터 Type  [CHE1]     	    	 */
	char 	gb			    [ 1 + 1 ];		/* 데이터 Type  구분자        		 */
	char 	realkey		    [11 + 1 ];		/* 조작자 ID        		         */
    char 	lineseq		    [ 8 + 1 ];		/* 라인일련번호            		 	 */
	char	or_type			[ 2 + 1 ];		/* 주문유형코드					  */
	char	prgb		    [ 2 + 1 ];		/* 주문체결유형코드				     */
	char 	accno           [11 + 1 ];		/* 계좌번호                          */
	char 	orderno         [10 + 1 ];		/* 주문번호                          */ 
	char 	orgorderno      [10 + 1 ];		/* 원주문번호                        */
	char 	cheno           [10 + 1 ];		/* 체결번호							 */
	char 	jmcode          [12 + 1 ];		/* 종목코드   "XXXXXXXX"             */
	char 	jmname          [40 + 1 ];		/* 종목명                            */
	char 	bid_tpcd        [ 2 + 1 ];		/* 호가유형코드                      */
	char 	ord_cond        [ 1 + 1 ];		/* 주문조건                          */
	char 	ord_tpcd        [ 2 + 1 ];		/* 주문유형코드                      */
	char 	ord_mkcd        [ 2 + 1 ];		/* 주문시장코드                      */
	char 	medosu          [ 1 + 1 ];		/* 매도수구분 [매도:1/매수:2]        */ 
	char 	jprc            [13 + 1 ];		/* 주문가격                          */
	char 	jqty            [16 + 1 ];		/* 주문수량                          */
	char 	cheprice        [13 + 1 ];		/* 체결가							 */	
	char 	cheqty			[16 + 1 ];		/* 체결량							 */
	char 	sumprice        [16 + 1 ];		/* 약정금액합계                      */
	char 	mecheqty        [16 + 1 ];		/* 미체결수량                        */
	char 	chetime         [ 8 + 1 ];		/* 주문시간					         */ 
	char 	price           [ 9 + 1 ];		/* 현재가							 */
	char    sign            [ 1 + 1 ];		/* 전일구분							 */
	char 	medoga          [ 9 + 1 ];		/* (최우선)매도호가					 */
	char 	mesuga          [ 9 + 1 ];		/* (최우선)매수호가					 */
}FO_CHE_STRUCT;

// 선물옵션 잔고 구조체
typedef struct
{
	char 	type          [ 4 + 1 ];	/* 데이터 Type                      */
	char 	gubun         [ 1 + 1 ];	/* 데이터 Gb                        */
	char 	realkey       [11 + 1 ];	/* 조작자 ID                        */
    char 	lineseq       [ 8 + 1 ];	/* 라인일련번호                     */
	char    accno         [11 + 1 ];	/*  계좌번호                        */
    char    jmcode        [12 + 1 ];    /*  종목코드  "XXXXXXXX"            */
    char    jmname        [40 + 1 ];    /*  종목명                          */
	char    bqty          [16 + 1 ];	/*  보유수량                        */
	char    buyamt        [13 + 1 ];	/*  평균매입지수                    */
									 	/*  평균매입지수/100                */
    char    mesusum       [16 + 1 ];	/*  총매입금액                      */
									 	/*  총매입금액/1000+0.5             */
    char    ordgaqty      [16 + 1 ];	/*  청산가능 수량                   */
    char    tsmevol       [16 + 1 ];	/*  당일순매수량                    */
								 		/*  당일매수수량-당일매도수량       */
    char    medosu        [ 1 + 1 ];	/*  매도/매수 구분                  */
	char    fee           [16 + 1 ];	/*  수수료                          */
    char    tax           [16 + 1 ];	/*  세금                            */
	char    preqty        [16 + 1 ];	/*  전일보유수량		            */
	char 	medoamtsum    [16 + 1 ];	/* 매도금액합                       */
	char 	mesuamtsum    [16 + 1 ];	/* 매수금액합                       */
	char    price         [ 9 + 1 ];	/*  현재가                          */
								  	 	/*  0이면 기준가,                   */
								 		/*  기준가도 0이면 전일종가         */
    char    sign          [ 1 + 1 ];	/*  전일구분                        */
    char    medoga        [ 9 + 1 ];	/*  (최우선)매도호가                */
    char    mesuga        [ 9 + 1 ];	/*  (최우선)매수호가                */
	
}FO_JAN_STRUCT;

// 해외선물 체결 구조체
typedef struct
{
	char    acctno			[ 11 + 1 ];				/* 계좌번호(11)                    */
	char    ord_no			[ 5 + 1 ];				/* 주문번호(5)                     */
    char    orig_ord_no		[ 5 + 1 ];              /* 원주문번호(5)                   */
    char    series			[ 32 + 1 ];             /* 종목(32)                        */
    char    bysl_tp			[ 1 + 1 ];              /* 구분(1) 1:매수 2:매도           */
    char    proc_stat		[ 1 + 1 ];              /* 주문처리상태(1)                 */
													/* 0:접수전             */
													/* 1:정상접수           */
													/* 2:접수거부           */
													/* 3:전송실패           */
													/* A:체결               */
													/* B:부분체결           */
    char    ord_q			[ 10 + 1 ];             /* 주문량(10)                      */
    char    trd_q			[ 10 + 1 ];             /* 체결량(10)                      */
    char    remainqty		[ 10 + 1 ];             /* 잔량(10)                        */
    char    ord_p			[ 19 + 1 ];             /* 주문가(19)                      */
    char    trd_p			[ 19 + 1 ];             /* 체결가(19)                      */
    char    mdfy_cncl_q		[ 19 + 1 ];             /* 정정취소(19)                    */
    char    prce_tp			[ 1 + 1 ];              /* 가격구분(1) 1:지정가 2:시장가   */
    char    stop_p			[ 19 + 1 ];             /* 조건가격(19)                    */
    char    ordtype			[ 1 + 1 ];              /* 주문유형(1)                     */
													/* 1 일반 2 STOP ORDER 3 OCO ORDER 4 가격예약 5 MIT ORDER */
	char	ord_tp			[ 1 + 1 ];				/* 주문구분(1)					   */
													/* 1:신규주문           */
													/* 2:정정주문           */
													/* 3:취소주문           */
    char    acpt_time		[ 10 + 1 ];             /* 접수시간(10)                    */
    char    kr_ord_date		[ 8 + 1 ];              /* 한국주문일자(8)                 */
    char    kr_acpt_time	[ 10 + 1 ];             /* 한국접수시간(10)                */
    char    kr_che_time		[ 10 + 1 ];             /* 한국체결시간(10)                */
    char    ord_way_tp		[ 1 + 1 ];              /* 매체구분(1)                     */
													/* 1:FRONT              */
													/* 2:HTS                */
													/* 4:강제체결           */
    char    work_empl		[ 9 + 1 ];              /* 사용자ID(9)                     */
    char    crc_cd			[ 3 + 1 ];              /* 통화코드(3)                     */
	char	sign			[ 1 + 1 ];				/* 대비부호(1)					   */
	char	price			[ 19 + 1 ];				/* 현재가(19)					   */
}FR_CHE_STRUCT;

// 해외선물 잔고 구조체
typedef struct
{
	char    acctno			[ 11 + 1 ];				/* 계좌번호(11)                    */
	char    series			[ 32 + 1 ];             /* 종목(32)                        */
    char    series_nm		[ 50 + 1 ];             /* 종목명(50)                      */
    char    crc_cd			[ 3 + 1 ];              /* 거래통화(3)                     */
    char    buysellsect		[ 4 + 1 ];              /* 구분(4)                         */
    char    open_q			[ 10 + 1 ];             /* 미결제수량(10)                  */
    char    rsrb_ableqty	[ 10 + 1 ];             /* 청산가능수량(10)                */
    char    open_uv			[ 19 + 1 ];             /* 평균약정가(19)                  */
	char	sign			[ 1 + 1 ];				/* 대비부호(1)					   */
    char    last_p			[ 19 + 1 ];             /* 현재가(19)                      */
	char	mesu_hoga		[ 19 + 1 ];				/* 매수호가(19)					   */
	char	medo_hoga		[ 19 + 1 ];				/* 매도호가(19)					   */
    char    open_pl			[ 19 + 1 ];             /* 평가손익(19)                    */
    char    invtrate		[ 19 + 1 ];             /* 수익율(19)                      */
    char    ctrt_size		[ 19 + 1 ];             /* 계약단위(19)                    */
    char    dealamt			[ 19 + 1 ];             /* 체결금액(19)                    */
    char    pospl			[ 19 + 1 ];             /* 미결제약정평가금액(19)          */
}FR_JAN_STRUCT;

// 해외주식 체결
typedef struct
{
    char    type			[ 4];          /* 데이터 Type                          */
    char    gubun			[ 1];          /* 데이터 Gb                            */
    char    key1			[11];          /* KEY (계좌번호)                       */
    char    key2			[16];          /* KEY2 = usrfld                        */
    char    lineseq			[ 8];          /* 라인일련번호                         */
    char  MsgType           [20];			/* 메세지구분                           */
                                     /* TAF_S00  : 신규주문                  */
                                     /* TAF_S01  : 정정주문                  */
                                     /* TAF_S02  : 취소주문                  */
                                     /* TAF_R02  :  정정확인                 */
                                     /* TAF_R04  :  취소확인                 */
                                     /* TAF_R05  :  주문체결                 */
                                     /* TAF_R06  :  주문거부 (ER)            */
                                     /* TAF_R07  :  주문거부 (OCR)           */
									 /* TAF_R08  :  주문거부 (OMS)           */
    char  Account           [20];    /* 계좌번호                             */
    char  Currency           [3];    /* "HKD"                                */
    char  OrderID           [30];    /* 접수번호                             */
    char  ClOrderID          [8];    /* 주문번호   (지점번호3 + 주문번호 5)  */
    char  OrigClOrderID      [8];    /* 원주문번호 (지점번호3 + 주문번호 5)  */
    char  ExecID            [30];    /* ER 번호                              */
    char  OrderQty          [12];    /* 주문수량                             */
    char  OrdType            [1];    /* 매매구분 (1 : Market , 2 : Limit)    */
    char  Price             [12];    /* 주문단가                             */
    char  Side               [1];    /* 매도매수구분 (1:BUY , 2:SELL)        */
    char  Symbol            [12];    /* 종목코드  (ISIN CODE)                */
    char  hname             [40];    /* 한글명                               */
    char  TargetCompID      [10];    /* COMP ID (SELL SIDE ID)               */
	char  DeliverToCompID   [10];    /* COMP ID (SELL SIDE ID)               */
    char  Exdestination      [3];    /* 거래소 구분1 (100)                   */
    char  SecurityEx         [5];    /* 거래소 구분2 (207)                   */
    char  OrderStatus        [1];    /* 주문상태                             */
    char  RejectReason       [2];    /* 거부사유코드 (ER, OCR)               */
    char  CumQty            [12];    /* 누적체결수량                         */
    char  LastShares        [12];    /* 체결수량                             */
    char  LeavesQty         [12];    /* 잔량                                 */
    char  LastPx            [12];    /* 체결단가                             */
    char  TransactTime      [20];    /* 주문수신시간                         */
    char  Text              [80];    /* 거부사유                             */
}OVERSEA_CHE_REAL_OUTREC;

/* FX 체결,미체결 관리 구조체 */
typedef struct  
{
	char	szKeyType[4];				/*  [FX주문체결] 미체결 - OR01, 체결 CH01		*/
	char	szAccNo[11];				/*	[계좌번호]	 주문시 회원사+계좌번호			*/
	char	szMemberNo[3];				/*  [회원사ID]	 디폴트 000 (고정)				*/
	char	szOrdType[5];				/*  [주문유형]	 두번째 자리로 구분		  
																			 //OM - 시장가,
																			 //OE - 지정가,
																			 //OS - 역지정가,
																			 //RE - 지정가 정정,
																			 //RS - 역지정가 정정				
											(1:시장가   2:지정가 3:역지정가 4:STOP-LIMIT 5:OCO V:추적손실)*/
	char	SHCODE[12];					/*  [종목명]	 형태 : CCY1/CCY2				*/
	char	szOrderID[20];				/*  [거래소 접수번호]							*/
	char	szRate[10];					/*  [약정지수]	 미체결시 주문가, 체결시 체결가	*/
	char	szQuote[10];				/*  [현재가]	 현재 체결된 포지션의 반대매매 현재가 */
	char	szStop[10];					/*  [Stop Price]								*/
	char	szLimit[10];				/*	[Limit Price								*/	
	char	szTStop[10];				/*	[Trailing Stop Price						*/	
	char	szOldStop[10];				/*  [Old Stop]									*/
	char	szOldLimit[10];				/*  [Old Limit]									*/
	char	szOpenTime[30];				/*  [주문, 접수 시각]							*/
	char	szSLOpenTime[30];			/*  [SL 주문, 접수 시각]						*/
	char	szExeTime[30];				/*  [약정시각]	 체결시각						*/
	char	szOrgCustItem1[11];			/*  [원회원처리항목1]							*/
	char	szOrgCustItem2[14];			/*  [원회원처리항목2] 원주문번호				*/
	char	szOrgCustItem3[55];			/*  [원회원처리항목3]							*/
	char	szCustItem1[11];			/*  [회원처리항목 1]							*/
	char	szCustItem2[14];			/*  [회원처리항목 2]  주문번호					*/
	char	szCustItem3[55];			/*  [회원처리항목 3]							*/
	char	szStopItem[80];				/*  [Stop 회원처리항목]	Stop 주문번호 (미표시)	*/
	char	szLimitItem[80];			/*  [Limit 회원처리항목] Limit 주문번호	(미표시)*/
	char	szOldStopItem[80];			/*	[정취전 Stop 회원처리항목]					*/
	char	szOldLimitItem[80];			/*  [정취전 Limit 회원처리항목]					*/
	char	szTSCustItem[80];			/*  [TrailingStop 회원처리항목]					*/
	char	szTradeType[3];				/*  [거래유형]	11:진입, 12:청산, 13:마진콜		*/
	char	szSide[3];					/*  [매매구분] 
											079 - 매수, 081 - 매도, 
											080 - 환매, 082 - 전매						*/
	char	szAccName[20];				/*  [계좌명]									*/
	char	szUserID[20];				/*  [고객ID]									*/
	char	szFXCMPosID[20];			/*  [TicketNo]	체결번호						*/
	char	szPositionID[20];			/*  포지션번호									*/

	char	szStatus[2];				/*  [미체결 상태 구분 0:미접수, 1:접수]
										=> 다음과 같이 수정.  솔로몬
											[주문상태정보]
											01:주문접수,02:체결중,03:체결완료,
											04:정정접수,05:정정완료(원주문),06:취소접수,
											07:취소완료(원주문),08:거부,15:정정완료(정정주문),17:취소완료(취소주문) */
	char	szSign[1];					/*  [등락부호]	2 - 상승, 3 - 보합, 4 - 하락	*/
	long	nCommition;					/*  [수수료]									*/
	long	nTrailingStop;				/*  [Trailing Stop Pip]							*/
	long	nPipLowest;					/*	[소수점 유효자리수]							*/
	double	fAmount;					/*  [체결수량]									*/
	double	fOrderAmt;					/*  [미체결수량]								*/
	double	fTotalPL;					/*  [핍손익]									*/
	double	fGrossPL;					/*  [일일손익(가격환산)]						*/
	double	fRevRate;					/*  [체결시 반대지수]							*/
	double	fOpenMarkup;				/*  [OM (전환매시)]								*/
	double	fInterest;					/*  [이자]										*/
	double	fAddInterest;				/*  [누적된 이자]								*/
	double	fCloseMarkup;				/*  [CM (전환매시 사용)]						*/
	double	fOutBalance;				/*  [미결제금액(청산손익)]						*/
	double	fPipCost;					/*  [체결시PipCost]								*/
	double	fPredictMRate;				/*  예상 마진콜 환율							*/
	double	fTSHighPrice;				/*  Trailing Stop 주문 현재가 대비 고가			*/
} FX_CHE_STRUCT;

typedef struct 
{
    double  fBalance;				    /* 예탁금 잔액 */
    double  fOutstanding;				/* 청산손익 */
    double  fGrossPL;					/* 평가손익 */
    double  fEquity;					/* 평가자산 */
    double  fMarginReq;					/* 주문 증거금 합계 */
    double  fMtMargin;					/* 유지 증거금 합계 */
	double  fOrderMargin;				/* 주문 증거금 */
    double  fUsableEquity;				/* 주문가능금액 */
    double  fUEquityRate;			    /* 마진콜비율(%) */
    double  fDepoTotalMSubs;			/* 원화대용금액 */
    double  fNowSellRate;				/* 원화대용적용환율(기준매도환율) */
    double  fBalanceWon;				/* 원화총예탁금 */
	double	fTotalWon;					/* 원화환산자산 */
    int     nOutCount;					/* 주문가능수량 */
	int		nOpenSum;					/* 미청산 수량 합계 */
	char	szMemberNo[3];				/* 회원사 */
    char    szAccNo[11];				/* 계좌번호*/
    char    szUserName[40];				/* 계좌명 */
    char    cIsHedge;					/* 헤지여부 */
} FX_JAN_STRUCT;


typedef struct  
{
	double	fBalance;					/* 달러 예탁금 합계		*/
	double	fOutstanding;				/* 청산손익 합계		*/
	double	fCurPL;						/* 평간손익 합계		*/
	double	fReplaceUSD;				/* 원화대용 USD 합계	*/
	double	fEquity;					/* 총평가예탁금 합계	*/
	double	fTotalWon;					/* 원화 환산자산 합계	*/
	double	fOrderMargin;				/* 주문증거금 합계		*/
	double	fMtMargin;					/* 유지증거금 합계		*/
	double	fUsableEquity;				/* 주문가능금액 합계	*/
	int		nUsableOrderCnt;			/* 주문가능수량 합계	*/
} TOTAL_JAN_ROW;						/* 잔고현황 합계		*/

#pragma pack(pop)
#endif