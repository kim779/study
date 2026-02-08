/*************************************************************************************/
/*  1. 프로그램ID : eu_CheJanStruct                                                  */
/*  2. 프로그램명 : aa                                                               */
/*  3. 서비스명   : eu_CheJanStruct	                                                 */
/*  4. 사용테이블 :                                                                  */
/*  5. 작성자     : gds                                                              */
/*  6. 입력항목   :                                                                  */
/*  7. 출력항목   :                                                                  */
/*************************************************************************************/
#ifndef __EU_TRAN_EU_CHEJANSTRUCT_H__
#define __EU_TRAN_EU_CHEJANSTRUCT_H__

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
#define		UM_RESET_CHEJANINFO		WM_USER + 1150		//	체결잔고 재설정 메시지


#define ulong	 ULONG

#pragma pack(push, 1) 

//===================================================================================
//	통신 데이터 구조체 ( OUTPUT )
//===================================================================================
//	계좌 정보 데이터
typedef struct 
{
	char	accno			[11];		// 계좌번호 
	char	accpwd			[ 8];		// 비밀번호 
} ACCOUNT_RQ_INPUT;

typedef struct 
{
	char	order_amt	[13];		/*  [주문가능현금]     */
	char	order_dae	[13];		/*  [주문가능대용]     */
	char	measu_amt	[13];		/*  [재매수가능금액]     */
	char	add_amt		[13];		/*  [추가담보현금]     */
	char	credit		[13];		/*  [융자한도]     */
	char	acc_amt		[1];		/*  [계좌증거금 'Y', 'N' ]     */
} ACCOUNT_RQ_OUTPUT1;

//	체결 정보 데이터 조회 구조체
typedef struct 
{
	char	accno		[11];		// 계좌번호 
} CHE_RQ_INPUT;

typedef struct 
{
	char	nCnt		[ 4];		//	체결데이터 건수
} CHE_RQ_OUTPUT1;

typedef struct 
{
	//	유진증권 주식체결 구조체
	char	jcgb       [ 2];	 /* 주문체결구분					 */
	                             /*"10":원주문 "11":정정주문         */
	                             /*"12":취소주문                     */ 
	char 	accno      [11];     /* 계좌번호                         */
	char 	orderno    [ 5];     /* 주문번호                         */
	char 	jmcode     [12];     /* 종목코드                         */
	char 	ordgb      [ 2];     /* 주문업무분류                     */
	                             /* "10":장내,	"11":시간외		     */
								 /* "12":Kosdaq,	"13":kosdaq시간외*/
								 /* "14":OTCBB,	"51":ECN(거래소)     */
								 /* "52":ECN(코스닥)				 */
	char 	ordst      [ 2];     /* 주문상태                         */
							     /* "21":체결정리, "22":정정확인	 */
								 /* "23":취소확인, "24":주문거부	 */
								 /* "25":주문접수확인			     */
								 /* "30":신용약정정정			     */
								 /* "31":주문별통신매체정정		     */
	char 	jmname     [20];     /* 종목명                           */
	char 	jqty       [ 9];     /* 주문수량                         */
	char 	jprc       [ 9];     /* 주문가격                         */
	char 	mecheqty   [ 9];     /* 미체결수량                       */
	char 	sumprice   [12];     /* 체결누계금액                     */
	char 	orgorderno [ 5];     /* 원주문번호                       */
	char 	jugb       [ 2];     /* 주문구분                         */
	                             /* "01":현금매도				     */
								 /* "02":현금매수				     */
								 /* "03":신용매도				     */
								 /* "04":신용매수				     */
								 /* "05":저축매도				     */
								 /* "06":저축매수				     */
								 /* "07":상품매도(차입)			     */
								 /* "09":상품매도				     */
								 /* "10":상품매수				     */
								 /* "11":선물대용일반매도		     */
								 /* "12":선물대용반대매도		     */
								 /* "13":현금매도(프)			     */
								 /* "14":현금매수(프)			     */
								 /* "15":현금매수(유가)			     */
								 /* "16":현금매수(정리)			     */
								 /* "17":상품매도(차입.프)		     */
								 /* "19":상품매도(프)			     */
								 /* "20":상품매수(프)			     */
								 /* "21":인수예정수량매도		     */
								 /* "22":인도예정수량매수		     */
								 /* "23":권리분공매도    		     */
	char 	memegb     [ 2];     /* 매매구분                         */
	                             /* "00": 보통   "03": 시장가        */
                                 /* "05": 조건부지정가               */
                                 /* "09": 자사주                     */
                                 /* "10": 보통(IOC)                  */
                                 /* "13": 시장가(IOC)                */
                                 /* "16": 최유리(IOC)                */
                                 /* "20": 보통(FOK)                  */
                                 /* "23": 시장가(FOK)                */
                                 /* "26": 최유리(FOK)                */
                                 /* "61": 장전 시간외                */
                                 /* "71": 신고대량전장시가           */
                                 /* "73": 신고대량종가               */
                                 /* "81": 시간외 종가                */
                                 /* "87": 시간외 대량                */
                                 /* "88": 시간외 바스켓              */
                                 /* "89": 시간외 자사주              */
	char 	medosu     [ 1];     /* 매도수구분 ( 1:매도, 2:매수 )    */
	char 	chetime    [ 8];     /* 주문/체결 시간                   */
	char 	cheno      [ 7];     /* 체결번호                         */
	char 	cheprice   [ 9];     /* 체결가                           */
	char 	cheqty     [ 9];     /* 체결량                           */
	char 	price      [ 9];     /* 현재가                           */
	char 	sign       [ 1];     /* 전일대비부호                     */
	char 	medoga     [ 9];     /* (최우선)매도호가                 */
	char 	mesuga     [ 9];     /* (최우선)매수호가                 */
	char 	percheprice[ 9];     /* 단위체결가                       */
	char 	percheqty  [ 9];     /* 단위체결량                       */
	char 	memefee    [16];     /* 당일매매 수수료                  */
	char 	memetax    [16];     /* 당일매매세금                     */
} CHE_RQ_OUTPUT2;



//===================================================================================
//	잔고 통신 데이터 구조체
//===================================================================================
typedef struct 
{
	char	accno			[11];		// 계좌번호 
} JAN_RQ_INPUT;

typedef struct 
{
	char    trsonik     [16];		/*  당일 실현손익              */
	char	nCnt		[4 ];
} JAN_RQ_OUTPUT1;

typedef struct 
{
	char    accno         [11];  /*  계좌번호                        */
    char    jmcode        [12];  /*  종목코드                        */
    char    sygb          [ 2];  /*  신용구분                        */
    char    sdate         [ 8];  /*  대출일                          */
    char    jmname        [20];  /*  종목명                          */
    char    estsonik      [16];  /*  평가손익                        */
    char    rate          [ 8];  /*  수익률                          */
    char    bqty          [ 9];  /*  보유수량                        */
    char    buyamt        [ 9];  /*  매입단가                        */
	char    pre_price     [ 9];  /*  전일종가                        */
	char    bep_price     [ 9];  /*  BEP 단가                        */
    char    janqty        [ 9];  /*  결제잔고                        */
    char    price         [ 9];  /*  현재가                          */
    char    sign          [ 1];  /*  전일구분                        */
    char    psmevol       [ 9];  /*  전일순매수량                    */
    char    tsmevol       [ 9];  /*  당일 순매수량                   */
    char    menusum       [16];  /*  매입금액                        */
    char    estamt        [16];  /*  평가금액                        */
    char    fee           [16];  /*  수수료                          */
    char    tax           [16];  /*  세금                            */
    char    tmedosonik    [16];  /*  당일 총 매도손익                */
    char    medosu        [ 1];  /*  매도/매수 구분                  */
    char    medoga        [ 9];  /*  (최우선)매도호가                */
    char    mesuga        [ 9];  /*  (최우선)매수호가                */
    char    ordgaqty      [ 9];  /*  주문가능 수량                   */
    char    giga          [ 9];  /*  기준가                          */
    char    sonrate       [ 9];  /*  손익률                          */
    char    sinamt        [16];  /*  신용금액                        */
    char    sininter      [16];  /*  신용이자                        */
    char    lastday       [ 8];  /*  만기일                          */
    char    trsonik       [16];  /*  당일실현손익(유가)              */
    char    ratesonik     [16];  /*  당일실현손익률(유가)            */
    char    strsonik      [16];  /*  당일실현손익(신용)              */
    char    sratesonik    [16];  /*  당일실현손익률(신용)            */
    char    pjogasonik    [16];  /*  전일종가 기준 평가손익          */
    char    pjogarate     [16];  /*  전일종가 기준 수익률            */
    char    holdrate      [ 6];  /*  보유비중                        */
    char    pjogaest      [16];  /*  전일종가 전일평가               */
    char    sygbtxt       [10];  /*  신용구분(TEXT)                  */
    char    dnlmtprice    [ 9];  /*  하한가                          */
    char    uplmtprice    [ 9];  /*  상한가                          */
    char    berate        [16];  /*  손익분기                        */
    char    damboloanamt  [ 9];  /*  담보대출수량                    */
    char    sygb2         [ 2];  /*  신용구분2                       */
	char    change        [ 8];  /*  전일대비                        */
    char    diff          [ 6];  /*  등락률 ( 소수점 2자리 )         */
    char    volume        [12];  /*  거래량                          */

	char	totalsellprice[16];/*매도 금액합 */
	char	totalbuyprice [16];/*매수 금액합 */
} JAN_RQ_OUTPUT2;


//	선물옵션 체결 정보 데이터 조회 구조체
typedef struct 
{
	char	accno		[11];		// 계좌번호 
} FO_CHE_RQ_INPUT;

typedef struct 
{
	char	nCnt		[ 4];		//	체결데이터 건수
} FO_CHE_RQ_OUTPUT1;

typedef struct 
{
	//	유진증권 선물체결 구조체
	char	jcgb			[ 2];	 /* 주문체결구분					 */
	char 	accno           [11];    /* 계좌번호                             */
	char 	orderno         [ 7];    /* 주문번호                             */ 
	char 	jmcode          [12];    /* 종목코드   "XXXXXXXX"                */
	char 	jmname          [30];    /* 종목명                               */
	char 	ordgb			[ 2];    /* 주문업무분류                     */
								     /* "10":장내,	"11":시간외		     */
									 /* "12":Kosdaq,	"13":kosdaq시간외*/
									 /* "14":OTCBB,	"51":ECN(거래소)     */
									 /* "52":ECN(코스닥)				 */
	char 	ordst           [ 2];    /* 주문상태                             */
						    	     /* "21":체결정리, "22":정정확인	     */
						    		 /* "23":취소확인, "24":주문거부	     */
						    		 /* "25":주문접수확인			         */
						    		 /* "30":신용약정정정			         */
						    		 /* "31":주문별통신매체정정		         */
	char 	jqty            [ 9];    /* 주문수량                             */
	char 	jprc            [ 9];    /* 주문가격                             */
	char 	mecheqty        [ 9];    /* 미체결수량                           */
	                                 /* 수량1 -                              */
	                                 /* (정정수량-취소수량-거부수량-체결수량)*/
	char 	sumprice        [12];    /* 약정금액합계                         */
	char 	orgorderno      [ 7];    /* 원주문번호                           */
	char 	medosu          [ 1];    /* 매도수구분 [매도:1/매수:2]           */ 
	char 	memegb          [ 2];    /* 매매구분[주문유형]                   */
	                                 /* "00": 보통   "03": 시장가            */
                                     /* "05": 조건부지정가                   */
                                     /* "09": 자사주                         */
                                     /* "10": 보통(IOC)                      */
                                     /* "13": 시장가(IOC)                    */
                                     /* "16": 최유리(IOC)                    */
                                     /* "20": 보통(FOK)                      */
                                     /* "23": 시장가(FOK)                    */
                                     /* "26": 최유리(FOK)                    */
                                     /* "61": 장전 시간외                    */
                                     /* "71": 신고대량전장시가               */
                                     /* "73": 신고대량종가                   */
                                     /* "81": 시간외 종가                    */
                                     /* "87": 시간외 대량                    */
                                     /* "88": 시간외 바스켓                  */
                                     /* "89": 시간외 자사주                  */
	char 	chetime         [ 8];    /* 주문/체결 시간                       */ 
	char 	cheno           [ 7];    /* 체결번호                             */
	char 	cheprice        [ 9];    /* 체결가                               */	
	char 	cheqty			[ 9];    /* 체결량                           */
	char 	price           [ 9];    /* 현재가                               */
	char    sign            [ 1];    /* 전일구분                             */
	char 	medoga          [ 9];    /* (최우선)매도호가                     */
	char 	mesuga          [ 9];    /* (최우선)매수호가                     */
	char 	percheprice     [ 9];    /* 체결단가                             */
	char 	percheqty       [ 9];    /* 체결수량                             */
} FO_CHE_RQ_OUTPUT2;



//===================================================================================
//	잔고 통신 데이터 구조체
//===================================================================================
typedef struct 
{
	char	accno			[11];		// 계좌번호 
} FO_JAN_RQ_INPUT;

typedef struct 
{
	char    trsonik     [16];		/*  당일 실현손익              */
	char	nCnt		[4 ];
} FO_JAN_RQ_OUTPUT1;

typedef struct 
{
	char    accno         [11];		/*  계좌번호                        */
    char    jmcode        [12];     /*  종목코드  "XXXXXXXX"            */
    char    jmname        [30];     /*  종목명                          */
									/*  없으면 ' '                      */
    char    sign          [ 1];		/*  전일구분                        */
    char    price         [ 9];		/*  현재가                          */
									/*  0이면 기준가,                   */
									/*  기준가도 0이면 전일종가         */
    char    bqty          [ 9];		/*  보유수량                        */
    char    buyamt        [ 9];		/*  평균매입지수                    */
									/*  평균매입지수/100                */
    char    mesusum       [16];		/*  총매입금액                      */
									/*  총매입금액/1000+0.5             */
    char    ordgaqty      [ 9];		/*  청산가능 수량                   */
    char    tsmevol       [ 9];		/*  당일순매수량                    */
									/*  당일매수수량-당일매도수량       */
    char    medosu        [ 1];		/*  매도/매수 구분                  */
    char    medoga        [ 9];		/*  (최우선)매도호가                */
    char    mesuga        [ 9];		/*  (최우선)매수호가                */
	char    fee           [16];		/*  수수료                          */
    char    tax           [16];		/*  세금                            */
	char    preqty        [ 9];		/*  전일보유수량		            */
} FO_JAN_RQ_OUTPUT2;




/*************************************************************************************/
//	실시간 데이터 구조체
/*************************************************************************************/
// 주식체결 실시간
typedef struct
{
	//	유진증권 주식체결 구조체
	char 	type       [ 4];     /* 데이터 Type  [CHE0]        	*/
	char 	gb		   [ 1];     /* 데이터 Type  구분자        	*/
	char 	acckey     [11];     /* 계좌번호 키값              	*/
	char 	realkey    [16];     /* 조작자 ID                	*/
    char 	lineseq    [ 8];     /* 라인일련번호             	*/
	char	jcgb       [ 2];	 /* 주문체결구분					 */
	                             /*"10":원주문 "11":정정주문         */
	                             /*"12":취소주문                     */ 
	char 	accno      [11];     /* 계좌번호                         */
	char 	orderno    [ 5];     /* 주문번호                         */
	char 	adminno    [ 8];     /* 관리자사번                       */
	char 	jmcode     [12];     /* 종목코드                         */
	char 	ordgb      [ 2];     /* 주문업무분류                     */
	                             /* "10":장내,	"11":시간외		     */
								 /* "12":Kosdaq,	"13":kosdaq시간외*/
								 /* "14":OTCBB,	"51":ECN(거래소)     */
								 /* "52":ECN(코스닥)				 */
	char 	ordst      [ 2];     /* 주문상태                         */
							     /* "21":체결정리, "22":정정확인	 */
								 /* "23":취소확인, "24":주문거부	 */
								 /* "25":주문접수확인			     */
								 /* "30":신용약정정정			     */
								 /* "31":주문별통신매체정정		     */
	char 	jmname     [20];     /* 종목명                           */
	char 	jqty       [ 9];     /* 주문수량                         */
	char 	jprc       [ 9];     /* 주문가격                         */
	char 	mecheqty   [ 9];     /* 미체결수량                       */
	char 	sumprice   [12];     /* 체결누계금액                     */
	char 	orgorderno [ 5];     /* 원주문번호                       */
	char 	jugb       [ 2];     /* 주문구분                         */
	                             /* "01":현금매도				     */
								 /* "02":현금매수				     */
								 /* "03":신용매도				     */
								 /* "04":신용매수				     */
								 /* "05":저축매도				     */
								 /* "06":저축매수				     */
								 /* "07":상품매도(차입)			     */
								 /* "09":상품매도				     */
								 /* "10":상품매수				     */
								 /* "11":선물대용일반매도		     */
								 /* "12":선물대용반대매도		     */
								 /* "13":현금매도(프)			     */
								 /* "14":현금매수(프)			     */
								 /* "15":현금매수(유가)			     */
								 /* "16":현금매수(정리)			     */
								 /* "17":상품매도(차입.프)		     */
								 /* "19":상품매도(프)			     */
								 /* "20":상품매수(프)			     */
								 /* "21":인수예정수량매도		     */
								 /* "22":인도예정수량매수		     */
								 /* "23":권리분공매도    		     */
	char 	memegb     [ 2];     /* 매매구분                         */
	                             /* "00": 보통   "03": 시장가        */
                                 /* "05": 조건부지정가               */
                                 /* "09": 자사주                     */
                                 /* "10": 보통(IOC)                  */
                                 /* "13": 시장가(IOC)                */
                                 /* "16": 최유리(IOC)                */
                                 /* "20": 보통(FOK)                  */
                                 /* "23": 시장가(FOK)                */
                                 /* "26": 최유리(FOK)                */
                                 /* "61": 장전 시간외                */
                                 /* "71": 신고대량전장시가           */
                                 /* "73": 신고대량종가               */
                                 /* "81": 시간외 종가                */
                                 /* "87": 시간외 대량                */
                                 /* "88": 시간외 바스켓              */
                                 /* "89": 시간외 자사주              */
	char 	medosu     [ 1];     /* 매도수구분 ( 1:매도, 2:매수 )    */
	char 	chetime    [ 8];     /* 주문/체결 시간                   */
	char 	cheno      [ 7];     /* 체결번호                         */
	char 	cheprice   [ 9];     /* 체결가                           */
	char 	cheqty     [ 9];     /* 체결량                           */
	char 	price      [ 9];     /* 현재가                           */
	char 	sign       [ 1];     /* 전일대비부호                     */
	char 	medoga     [ 9];     /* (최우선)매도호가                 */
	char 	mesuga     [ 9];     /* (최우선)매수호가                 */
	char 	percheprice[ 9];     /* 단위체결가                       */
	char 	percheqty  [ 9];     /* 단위체결량                       */
	char 	memefee    [16];     /* 당일매매 수수료                  */
	char 	memetax    [16];     /* 당일매매세금                     */
	char	order_amt  [13];	 /*  주문가능현금                    */
	char	order_dae  [13];	 /*  주문가능대용                    */
	char	measu_amt  [13];	 /*  재매수가능금액                  */
	char	add_amt	   [13];	 /*  추가담보현금                    */

}S_CHE_REAL_OUTREC;

// 주식잔고 실시간
typedef struct
{
	char 	type          [ 4];  /* 데이터 Type                      */
	char 	gubun         [ 1];  /* 데이터 Gb                        */
	char 	acckey		  [11];  /* 계좌번호 키값              	*/
	char 	realkey       [16];  /* 조작자 ID                        */
    char 	lineseq       [ 8];  /* 라인일련번호                     */
	char    accno         [11];  /*  계좌번호                        */
    char    jmcode        [12];  /*  종목코드                        */
    char    sygb          [ 2];  /*  신용구분                        */
    char    sdate         [ 8];  /*  대출일                          */
    char    jmname        [20];  /*  종목명                          */
    char    estsonik      [16];  /*  평가손익                        */
    char    rate          [ 8];  /*  수익률                          */
    char    bqty          [ 9];  /*  보유수량                        */
    char    buyamt        [ 9];  /*  매입단가                        */
	char    pre_price     [ 9];  /*  전일종가                        */
	char    bep_price     [ 9];  /*  BEP 단가                        */
    char    janqty        [ 9];  /*  결제잔고                        */
    char    price         [ 9];  /*  현재가                          */
    char    sign          [ 1];  /*  전일구분                        */
    char    psmevol       [ 9];  /*  전일순매수량                    */
    char    tsmevol       [ 9];  /*  당일 순매수량                   */
    char    menusum       [16];  /*  매입금액                        */
    char    estamt        [16];  /*  평가금액                        */
    char    fee           [16];  /*  수수료                          */
    char    tax           [16];  /*  세금                            */
    char    tmedosonik    [16];  /*  당일 총 매도손익                */
    char    medosu        [ 1];  /*  매도/매수 구분                  */
//    char    depoamt       [16];  /*  예수금                          */
    char    medoga        [ 9];  /*  (최우선)매도호가                */
    char    mesuga        [ 9];  /*  (최우선)매수호가                */
    char    ordgaqty      [ 9];  /*  주문가능 수량                   */
    char    giga          [ 9];  /*  기준가                          */
    char    sonrate       [ 9];  /*  손익률                          */
    char    sinamt        [16];  /*  신용금액                        */
    char    sininter      [16];  /*  신용이자                        */
    char    lastday       [ 8];  /*  만기일                          */
    char    trsonik       [16];  /*  당일실현손익(유가)              */
    char    ratesonik     [16];  /*  당일실현손익률(유가)            */
    char    strsonik      [16];  /*  당일실현손익(신용)              */
    char    sratesonik    [16];  /*  당일실현손익률(신용)            */
    char    pjogasonik    [16];  /*  전일종가 기준 평가손익          */
    char    pjogarate     [16];  /*  전일종가 기준 수익률            */
    char    holdrate      [ 6];  /*  보유비중                        */
    char    pjogaest      [16];  /*  전일종가 전일평가               */
    char    sygbtxt       [10];  /*  신용구분(TEXT)                  */
    char    dnlmtprice    [ 9];  /*  하한가                          */
    char    uplmtprice    [ 9];  /*  상한가                          */
    char    berate        [16];  /*  손익분기                        */
    char    damboloanamt  [ 9];  /*  담보대출수량                    */
    char    sygb2         [ 2];  /*  신용구분2                       */
//	char	order_amt	  [13];	 /*  주문가능현금                    */
//	char	order_dae	  [13];	 /*  주문가능대용                    */
//	char	measu_amt	  [13];	 /*  재매수가능금액                  */
//	char	add_amt		  [13];	 /*  추가담보현금                    */
	char    change        [ 8];  /*  전일대비                        */
    char    diff          [ 6];  /*  등락률 ( 소수점 2자리 )         */
    char    volume        [12];  /*  거래량                          */			//	총 사이즈는 596
	char    pertrsonik    [16];  /*  당일실현손익변동분              */
}S_JAN_REAL_OUTREC;


// 선물옵션체결 실시간
typedef struct
{
	//	유진증권 주식체결 구조체
	char 	type			[ 4];    /* 데이터 Type  [CHE0]     	    	 */
	char 	gb			    [ 1];    /* 데이터 Type  구분자        			 */
	char 	acckey			[11];    /* 계좌번호 키값				     	*/
	char 	realkey		    [16];    /* 조작자 ID        		        	 */
    char 	lineseq		    [ 8];    /* 라인일련번호            		 	 */
	char	jcgb		    [ 2];	 /* 주문체결구분				    	 */
	                                 /*"10":원주문 "11":정정주문             */
	                                 /*"12":취소주문                         */ 
	char 	accno           [11];    /* 계좌번호                             */
	char 	orderno         [ 7];    /* 주문번호                             */ 
	char 	jmcode          [12];    /* 종목코드   "XXXXXXXX"                */
	char 	jmname          [30];    /* 종목명                               */
	char 	ordgb			[ 2];     /* 주문업무분류                     */
		                             /* "10":장내,	"11":시간외		     */
									 /* "12":Kosdaq,	"13":kosdaq시간외*/
									 /* "14":OTCBB,	"51":ECN(거래소)     */
									 /* "52":ECN(코스닥)				 */
	char 	ordst           [ 2];    /* 주문상태                             */
						    	     /* "21":체결정리, "22":정정확인	     */
						    		 /* "23":취소확인, "24":주문거부	     */
						    		 /* "25":주문접수확인			         */
						    		 /* "30":신용약정정정			         */
						    		 /* "31":주문별통신매체정정		         */
	char 	jqty            [ 9];    /* 주문수량                             */
	char 	jprc            [ 9];    /* 주문가격                             */
	char 	mecheqty        [ 9];    /* 미체결수량                           */
	                                 /* 수량1 -                              */
	                                 /* (정정수량-취소수량-거부수량-체결수량)*/
	char 	sumprice        [12];    /* 약정금액합계                         */
	char 	orgorderno      [ 7];    /* 원주문번호                           */
	char 	medosu          [ 1];    /* 매도수구분 [매도:1/매수:2]           */ 
	char 	memegb          [ 2];    /* 매매구분[주문유형]                   */
	                                 /* "00": 보통   "03": 시장가            */
                                     /* "05": 조건부지정가                   */
                                     /* "09": 자사주                         */
                                     /* "10": 보통(IOC)                      */
                                     /* "13": 시장가(IOC)                    */
                                     /* "16": 최유리(IOC)                    */
                                     /* "20": 보통(FOK)                      */
                                     /* "23": 시장가(FOK)                    */
                                     /* "26": 최유리(FOK)                    */
                                     /* "61": 장전 시간외                    */
                                     /* "71": 신고대량전장시가               */
                                     /* "73": 신고대량종가                   */
                                     /* "81": 시간외 종가                    */
                                     /* "87": 시간외 대량                    */
                                     /* "88": 시간외 바스켓                  */
                                     /* "89": 시간외 자사주                  */
	char 	chetime         [ 8];    /* 주문/체결 시간                       */ 
	char 	cheno           [ 7];    /* 체결번호                             */
	char 	cheprice        [ 9];    /* 체결가                               */	
	char 	cheqty			[ 9];    /* 체결량                           */
	char 	price           [ 9];    /* 현재가                               */
	char    sign            [ 1];    /* 전일구분                             */
	char 	medoga          [ 9];    /* (최우선)매도호가                     */
	char 	mesuga          [ 9];    /* (최우선)매수호가                     */
	char 	percheprice     [ 9];    /* 체결단가                             */
	char 	percheqty       [ 9];    /* 체결수량                             */
	char	order_amt		[13];	 /*  주문가능현금                    */
	char	order_dae		[13];	 /*  주문가능대용                    */
	char	measu_amt		[13];	 /*  재매수가능금액                  */
	char	add_amt			[13];	 /*  추가담보현금                    */
}FO_CHE_REAL_OUTREC;

// 선물옵션 잔고 실시간
typedef struct
{
	char 	type          [ 4];		/* 데이터 Type                      */
	char 	gubun         [ 1];		/* 데이터 Gb                        */
	char 	acckey		  [11];     /* 계좌번호 키값					*/
	char 	realkey       [16];		/* 조작자 ID                        */
    char 	lineseq       [ 8];		/* 라인일련번호                     */
	char    accno         [11];		/*  계좌번호                        */
    char    jmcode        [12];     /*  종목코드  "XXXXXXXX"            */
    char    jmname        [30];     /*  종목명                          */
									/*  없으면 ' '                      */
    char    sign          [ 1];		/*  전일구분                        */
    char    price         [ 9];		/*  현재가                          */
									/*  0이면 기준가,                   */
									/*  기준가도 0이면 전일종가         */
    char    bqty          [ 9];		/*  보유수량                        */
    char    buyamt        [ 9];		/*  평균매입지수                    */
									/*  평균매입지수/100                */
    char    mesusum       [16];		/*  총매입금액                      */
									/*  총매입금액/1000+0.5             */
    char    ordgaqty      [ 9];		/*  청산가능 수량                   */
    char    tsmevol       [ 9];		/*  당일순매수량                    */
									/*  당일매수수량-당일매도수량       */
    char    medosu        [ 1];		/*  매도/매수 구분                  */
    char    medoga        [ 9];		/*  (최우선)매도호가                */
    char    mesuga        [ 9];		/*  (최우선)매수호가                */
	char    fee           [16];		/*  수수료                          */
    char    tax           [16];		/*  세금                            */
	char    preqty        [ 9];		/*  전일보유수량		            */
	char    pertrsonik    [16];     /*  당일실현손익변동분              */
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




/*************************************************************************************/
//	잔고 데이터 저장 구조체
/*************************************************************************************/
//	계좌 데이터 구조체
typedef struct 
{
	char	accNo		[11 + 1];		// 계좌번호
	char	accName		[30 + 1];		// 계좌명
	char	accPasswd	[ 8 + 1];		// 비밀번호
	BYTE	accgb;						// 계좌구분 ['1': 위임, '2': 수임, '0':해당사항없음 ]	
	BYTE	recordtp;					// 비밀번호저장여부(1:저장, 0:미저장)
	char	fee1		[ 4 + 1];		// 수수료 1 [default 는 스페이스 처리]
	char	fee2		[ 4 + 1];		// 수수료 2 [default 는 스페이스 처리]	
	char	accPenName	[30 + 1];		// 계좌명 (필명)
	char	accOrgPasswd[ 8 + 1];		// 원본 비밀번호
	BYTE	realdatatp;					// 주문/체결 리얼 데이타 처리구분 (1:처리, 0:미처리)
	char	filler		[10    ];		// 
	char	order_amt	[13 + 1];		// 주문가능현금
	char	order_dae	[13 + 1];		// 주문가능대용
	char	measu_amt	[13 + 1];		// 재매수가능금액
	char	add_amt		[13 + 1];		// 추가담보현금
	char	credit		[13 + 1];		// 융자한도
	char	acc_amt		[ 1 + 1];		// 계좌증거금
	char    trsonik     [16 + 1];		/*  당일 실현손익              */
	

	CObArray arrJan;
	CObArray arrChe;
} CHEJAN_ACC_STRUCT;

//	주식 잔고 구조체
typedef struct 
{
	char 	type          [ 4 + 1 ];  /* 데이터 Type                      */
	char 	gubun         [ 1 + 1 ];  /* 데이터 Gb                        */
	char 	realkey       [16 + 1 ];  /* 조작자 ID                        */
    char 	lineseq       [ 8 + 1 ];  /* 라인일련번호                     */
	char    accno         [11 + 1 ];  /*  계좌번호                        */
    char    jmcode        [12 + 1 ];  /*  종목코드                        */
    char    sygb          [ 2 + 1 ];  /*  신용구분                        */
									  //  00:보통
									  //  01:유통융자매수         11:유통융자매도상환
									  //  02:유통대주매도         22:유통대주매수상환
									  //  03:자기융자매수         33:자기융자매도상환
									  //  07:자기대주매도         77:자기대주매수상환
									  //  08:대출매수             88:대출매도상환

    char    sdate         [ 8 + 1 ];  /*  대출일                          */
    char    jmname        [20 + 1 ];  /*  종목명                          */
    char    estsonik      [16 + 1 ];  /*  평가손익                        */
    char    rate          [ 8 + 1 ];  /*  수익률                          */
    char    bqty          [ 9 + 1 ];  /*  보유수량                        */
    char    buyamt        [ 9 + 1 ];  /*  매입단가                        */
	char    pre_price     [ 9 + 1 ];  /*  전일종가                        */
	char    bep_price     [ 9 + 1 ];  /*  BEP 단가                        */
    char    janqty        [ 9 + 1 ];  /*  결제잔고                        */
    char    price         [ 9 + 1 ];  /*  현재가                          */
    char    sign          [ 1 + 1 ];  /*  전일구분                        */
    char    psmevol       [ 9 + 1 ];  /*  전일순매수량                    */
    char    tsmevol       [ 9 + 1 ];  /*  당일 순매수량                   */
    char    menusum       [16 + 1 ];  /*  매입금액                        */
    char    estamt        [16 + 1 ];  /*  평가금액                        */
    char    fee           [16 + 1 ];  /*  수수료                          */
    char    tax           [16 + 1 ];  /*  세금                            */
    char    tmedosonik    [16 + 1 ];  /*  당일 총 매도손익                */
    char    medosu        [ 1 + 1 ];  /*  매도/매수 구분                  */
//    char    depoamt       [16 + 1 ];  /*  예수금                          */
    char    medoga        [ 9 + 1 ];  /*  (최우선)매도호가                */
    char    mesuga        [ 9 + 1 ];  /*  (최우선)매수호가                */
    char    ordgaqty      [ 9 + 1 ];  /*  주문가능 수량                   */
    char    giga          [ 9 + 1 ];  /*  기준가                          */
    char    sonrate       [ 9 + 1 ];  /*  손익률                          */
    char    sinamt        [16 + 1 ];  /*  신용금액                        */
    char    sininter      [16 + 1 ];  /*  신용이자                        */
    char    lastday       [ 8 + 1 ];  /*  만기일                          */
    char    trsonik       [16 + 1 ];  /*  당일실현손익(유가)              */
    char    ratesonik     [16 + 1 ];  /*  당일실현손익률(유가)            */
    char    strsonik      [16 + 1 ];  /*  당일실현손익(신용)              */
    char    sratesonik    [16 + 1 ];  /*  당일실현손익률(신용)            */
    char    pjogasonik    [16 + 1 ];  /*  전일종가 기준 평가손익          */
    char    pjogarate     [16 + 1 ];  /*  전일종가 기준 수익률            */
    char    holdrate      [ 6 + 1 ];  /*  보유비중                        */
    char    pjogaest      [16 + 1 ];  /*  전일종가 전일평가               */
    char    sygbtxt       [10 + 1 ];  /*  신용구분(TEXT)                  */
    char    dnlmtprice    [ 9 + 1 ];  /*  하한가                          */
    char    uplmtprice    [ 9 + 1 ];  /*  상한가                          */
    char    berate        [16 + 1 ];  /*  손익분기                        */
    char    damboloanamt  [ 9 + 1 ];  /*  담보대출수량                    */
    char    sygb2         [ 2 + 1 ];  /*  신용구분2                       */
	char    change        [ 8 + 1 ];  /*  전일대비                        */
    char    diff          [ 6 + 1 ];  /*  등락률 ( 소수점 2자리 )         */
    char    volume        [12 + 1 ];  /*  거래량                          */
} S_JAN_STRUCT;

//	주식 체결 구조체
typedef struct 
{
	//	유진증권 주식체결 구조체
//	char 	type       [ 4 + 1 ];	/* 데이터 Type  [CHE0]        	*/
//	char 	gb         [ 1 + 1 ];	/* 데이터 Type  구분자        	*/
//	char 	userid     [ 9 + 1 ];	/* 조작자 ID                	*/
//	char 	lineseq    [ 8 + 1 ];	/* 라인일련번호             	*/
	char	jcgb       [ 2 + 1 ];	/* 주문체결구분					 */
				                    /*"10":원주문 "11":정정주문         */
									/*"12":취소주문                     */ 
	char 	accno      [11 + 1 ];   /* 계좌번호                         */
	char 	orderno    [ 5 + 1 ];   /* 주문번호                         */
	char 	jmcode     [12 + 1 ];   /* 종목코드                         */
	char 	ordgb      [ 2 + 1 ];   /* 주문업무분류                     */
									/* "10":장내,	"11":시간외		     */
									/* "12":Kosdaq,	"13":kosdaq시간외*/
									/* "14":OTCBB,	"51":ECN(거래소)     */
									/* "52":ECN(코스닥)				 */
	char 	ordst      [ 2 + 1 ];   /* 주문상태                         */
									/* "21":체결정리, "22":정정확인	 */
									/* "23":취소확인, "24":주문거부	 */
									/* "25":주문접수확인			     */
									/* "30":신용약정정정			     */
									/* "31":주문별통신매체정정		     */
	char 	jmname     [20 + 1 ];   /* 종목명                           */
	char 	jqty       [ 9 + 1 ];   /* 주문수량                         */
	char 	jprc       [ 9 + 1 ];   /* 주문가격                         */
	char 	mecheqty   [ 9 + 1 ];   /* 미체결수량                       */
	char 	sumprice   [12 + 1 ];   /* 체결누계금액                     */
	char 	orgorderno [ 5 + 1 ];   /* 원주문번호                       */
	char 	jugb       [ 2 + 1 ];   /* 주문구분                         */
									 /* "01":현금매도				     */
									 /* "02":현금매수				     */
									 /* "03":신용매도				     */
									 /* "04":신용매수				     */
									 /* "05":저축매도				     */
									 /* "06":저축매수				     */
									 /* "07":상품매도(차입)			     */
									 /* "09":상품매도				     */
									 /* "10":상품매수				     */
									 /* "11":선물대용일반매도		     */
									 /* "12":선물대용반대매도		     */
									 /* "13":현금매도(프)			     */
									 /* "14":현금매수(프)			     */
									 /* "15":현금매수(유가)			     */
									 /* "16":현금매수(정리)			     */
									 /* "17":상품매도(차입.프)		     */
									 /* "19":상품매도(프)			     */
									 /* "20":상품매수(프)			     */
									 /* "21":인수예정수량매도		     */
									 /* "22":인도예정수량매수		     */
									 /* "23":권리분공매도    		     */
	char 	memegb     [ 2 + 1 ];     /* 매매구분                         */
									 /* "00": 보통   "03": 시장가        */
									 /* "05": 조건부지정가               */
									 /* "09": 자사주                     */
									 /* "10": 보통(IOC)                  */
									 /* "13": 시장가(IOC)                */
									 /* "16": 최유리(IOC)                */
									 /* "20": 보통(FOK)                  */
									 /* "23": 시장가(FOK)                */
									 /* "26": 최유리(FOK)                */
									 /* "61": 장전 시간외                */
									 /* "71": 신고대량전장시가           */
									 /* "73": 신고대량종가               */
									 /* "81": 시간외 종가                */
									 /* "87": 시간외 대량                */
									 /* "88": 시간외 바스켓              */
									 /* "89": 시간외 자사주              */
	char 	medosu     [ 1 + 1 ];    /* 매도수구분 ( 1:매도, 2:매수 )    */
	char 	chetime    [ 8 + 1 ];    /* 주문/체결 시간                   */
	char 	cheno      [ 7 + 1 ];    /* 체결번호                         */
	char 	cheprice   [ 9 + 1 ];    /* 체결가                           */
	char 	cheqty     [ 9 + 1 ];    /* 체결량                           */
	char 	price      [ 9 + 1 ];    /* 현재가                           */
	char 	sign       [ 1 + 1 ];    /* 전일대비부호                     */
	char 	medoga     [ 9 + 1 ];    /* (최우선)매도호가                 */
	char 	mesuga     [ 9 + 1 ];    /* (최우선)매수호가                 */
	char 	percheprice[ 9 + 1 ];    /* 단위체결가                       */
	char 	percheqty  [ 9 + 1 ];    /* 단위체결량                       */
	char 	memefee    [16 + 1 ];    /* 당일매매 수수료                  */
	char 	memetax    [16 + 1 ];    /* 당일매매세금                     */
} S_CHE_STRUCT;

// 선물옵션체결 구조체
typedef struct
{
	//	유진증권 주식체결 구조체
	char 	type			[ 4 + 1 ];  /* 데이터 Type  [CHE1]     	    	    */
	char 	gb			    [ 1 + 1 ];  /* 데이터 Type  구분자        		    */
	char 	realkey		    [16 + 1 ];  /* 조작자 ID        		        	    */
    char 	lineseq		    [ 8 + 1 ];  /* 라인일련번호            		 	    */
	char	jcgb		    [ 2 + 1 ];  /* 주문체결구분				    	    */
	                                    /*"10":원주문 "11":정정주문               */
	                                    /*"12":취소주문                           */ 
	char 	accno           [11 + 1 ];  /* 계좌번호                               */
	char 	orderno         [ 7 + 1 ];  /* 주문번호                               */ 
	char 	jmcode          [12 + 1 ];  /* 종목코드   "XXXXXXXX"                  */
	char 	jmname          [30 + 1 ];  /* 종목명                                 */
	char 	ordgb			[ 2 + 1 ];	/* 주문업무분류                     */
										/* "10":장내,	"11":시간외		     */
										/* "12":Kosdaq,	"13":kosdaq시간외*/
										/* "14":OTCBB,	"51":ECN(거래소)     */
										/* "52":ECN(코스닥)				 */
	char 	ordst           [ 2 + 1 ];  /* 주문상태                               */
										/* "21":체결정리, "22":정정확인	        */
							    	    /* "23":취소확인, "24":주문거부	        */
							    	    /* "25":주문접수확인			            */
							    	    /* "30":신용약정정정			            */
							    	    /* "31":주문별통신매체정정		        */
	char 	jqty            [ 9 + 1 ];  /* 주문수량                               */
	char 	jprc            [ 9 + 1 ];  /* 주문가격                               */
	char 	mecheqty        [ 9 + 1 ];  /* 미체결수량                             */
	                                    /* 수량1 -                                */
	                                    /* (정정수량-취소수량-거부수량-체결수량)  */
	char 	sumprice        [12 + 1 ];  /* 약정금액합계                           */
	char 	orgorderno      [ 7 + 1 ];  /* 원주문번호                             */
	char 	medosu          [ 1 + 1 ];  /* 매도수구분 [매도:1/매수:2]             */ 
	char 	memegb          [ 2 + 1 ];  /* 매매구분[주문유형]                     */
	                                    /* "00": 보통   "03": 시장가              */
                                        /* "05": 조건부지정가                     */
                                        /* "09": 자사주                           */
                                        /* "10": 보통(IOC)                        */
                                        /* "13": 시장가(IOC)                      */
                                        /* "16": 최유리(IOC)                      */
                                        /* "20": 보통(FOK)                        */
                                        /* "23": 시장가(FOK)                      */
                                        /* "26": 최유리(FOK)                      */
                                        /* "61": 장전 시간외                      */
                                        /* "71": 신고대량전장시가                 */
                                        /* "73": 신고대량종가                     */
                                        /* "81": 시간외 종가                      */
                                        /* "87": 시간외 대량                      */
                                        /* "88": 시간외 바스켓                    */
                                        /* "89": 시간외 자사주                    */
	char 	chetime         [ 8 + 1 ];  /* 주문/체결 시간                         */ 
	char 	cheno           [ 7 + 1 ];  /* 체결번호                               */
	char 	cheprice        [ 9 + 1 ];  /* 체결가                                 */	
	char 	cheqty			[ 9 + 1 ];  /* 체결량                           */
	char 	price           [ 9 + 1 ];  /* 현재가                                 */
	char    sign            [ 1 + 1 ];  /* 전일구분                               */
	char 	medoga          [ 9 + 1 ];  /* (최우선)매도호가                       */
	char 	mesuga          [ 9 + 1 ];  /* (최우선)매수호가                       */
	char 	percheprice     [ 9 + 1 ];  /* 체결단가                               */
	char 	percheqty       [ 9 + 1 ];  /* 체결수량                               */
}FO_CHE_STRUCT;

// 선물옵션 잔고 구조체
typedef struct
{
	char 	type          [ 4 + 1 ];	/* 데이터 Type                      */
	char 	gubun         [ 1 + 1 ];	/* 데이터 Gb                        */
	char 	realkey       [16 + 1 ];	/* 조작자 ID                        */
    char 	lineseq       [ 8 + 1 ];	/* 라인일련번호                     */
	char    accno         [11 + 1 ];	/*  계좌번호                        */
    char    jmcode        [12 + 1 ];    /*  종목코드  "XXXXXXXX"            */
    char    jmname        [30 + 1 ];    /*  종목명                          */
									 	/*  없으면 ' '                      */
    char    sign          [ 1 + 1 ];	/*  전일구분                        */
    char    price         [ 9 + 1 ];	/*  현재가                          */
								  	 	/*  0이면 기준가,                   */
								 		/*  기준가도 0이면 전일종가         */
    char    bqty          [ 9 + 1 ];	/*  보유수량                        */
    char    buyamt        [ 9 + 1 ];	/*  평균매입지수                    */
									 	/*  평균매입지수/100                */
    char    mesusum       [16 + 1 ];	/*  총매입금액                      */
									 	/*  총매입금액/1000+0.5             */
    char    ordgaqty      [ 9 + 1 ];	/*  청산가능 수량                   */
    char    tsmevol       [ 9 + 1 ];	/*  당일순매수량                    */
								 		/*  당일매수수량-당일매도수량       */
    char    medosu        [ 1 + 1 ];	/*  매도/매수 구분                  */
    char    medoga        [ 9 + 1 ];	/*  (최우선)매도호가                */
    char    mesuga        [ 9 + 1 ];	/*  (최우선)매수호가                */
	char    fee           [16 + 1 ];	/*  수수료                          */
    char    tax           [16 + 1 ];	/*  세금                            */
	char    preqty        [ 9 + 1 ];	/*  전일보유수량		            */
}FO_JAN_STRUCT;



//===================================================================================
//	잔고 데이터 offset 정의 ( 데이터 시작 위치 지정 )
//===================================================================================
//	유진증권 잔고 데이터 사이즈 정의
#define J_JMCODE      	  0			   /* 종목코드 		          */		
#define J_JMNAME      	  ( J_JMCODE		+ 6				)    /* 종목명			          */
#define J_SIGN            ( J_JMNAME		+ 20			)    /* 전일대비구분               */
#define J_CHANGE		  ( J_SIGN			+ 1				)    /* 전일대비                   */
#define J_DIFF_RATE       ( J_CHANGE		+ sizeof(long)	)    /* 등락률                     */
#define J_VOLUME          ( J_DIFF_RATE		+ sizeof(double))    /* 거래량                     */
#define J_JANGO_QTY       ( J_VOLUME		+ sizeof(long)	)    /* 잔고수량                   */
#define J_MESU_PRICE      ( J_JANGO_QTY		+ sizeof(long)	)    /* 매입단가                   */
#define J_PRE_CLOSE       ( J_MESU_PRICE	+ sizeof(double))    /* 전일종가                   */
#define J_BEP_PRICE       ( J_PRE_CLOSE		+ sizeof(double))    /* BEP 단가                   */
#define J_BUYAMT          ( J_BEP_PRICE		+ sizeof(double))    /* 매입금액                   */
#define J_PRE_PYUNGAMT    ( J_BUYAMT		+ sizeof(double))    /* 전일평가금액               */
#define J_BEP_BUYAMT      ( J_PRE_PYUNGAMT	+ sizeof(double))    /* BEP 단가 매입금액          */
#define J_PRICE           ( J_BEP_BUYAMT	+ sizeof(double))    /* 현재가                     */
#define J_MESU            ( J_PRICE			+ sizeof(double))    /* 매수호가                   */
#define J_PYUNGAMT        ( J_MESU			+ sizeof(double))    /* 평가금액                   */
#define J_PYUNGSONIK      ( J_PYUNGAMT		+ sizeof(double))    /* 평가손익                   */
#define J_PRE_SONIK       ( J_PYUNGSONIK	+ sizeof(double))    /* 전일대비손익               */
#define J_PROFIT          ( J_PRE_SONIK		+ sizeof(double))    /* 수익률                     */
#define J_PRE_PROFIT      ( J_PROFIT		+ sizeof(double))    /* 전일대비 수익률            */
#define J_FEE             ( J_PRE_PROFIT	+ sizeof(double))    /* 수수료                     */
#define J_TAX             ( J_FEE			+ sizeof(double))    /* 제세금                     */
#define J_MEDO_QTY        ( J_TAX			+ sizeof(double))    /* 매도가능수량               */
#define J_RATE            ( J_MEDO_QTY		+ sizeof(long)	)    /* 비중                       */
#define J_SIN_GB          ( J_RATE			+ sizeof(double))    /* '1' : 현금, '3' : 융자, '8' : 담보대출  */
#define J_LOAN_DATE       ( J_SIN_GB		+ 1				)    /* 대출일                     */
#define J_EXPR_DATE       ( J_LOAN_DATE		+ 8				)    /* 만기일                     */
#define J_SINAMT          ( J_EXPR_DATE		+ 8				)    /* 신용금액                   */
#define J_SINRATE         ( J_SINAMT		+ sizeof(double))    /* 신용이자                   */	                               
#define J_CHE_QTY         ( J_SINRATE		+ sizeof(double))    /* 결제잔고                   */
#define J_PRE_DAEBI       ( J_CHE_QTY		+ sizeof(long)	)    /* 전일수량대비               */
#define J_DAY_DAEBI       ( J_PRE_DAEBI		+ sizeof(long)	)    /* 금일수량대비               */
#define J_STOP_TP         ( J_DAY_DAEBI		+ sizeof(long)	)    /* 스탑로스 감시여부 [Y/N]    */


#pragma pack(pop)
#endif