//
//// JSJ_ITCastle_New_040503
//// 10단계 호가조회
//
//typedef struct _D49890_I {
//	char shcode[6];             /* 종목코드M */
//} D49890_I;
//
//typedef struct _D49890_T {
//	char hname[20];              /* 종목명(한글)M  */
//	char price[9];               /* 현재가9(9),원  */
//	char sign[1];                /* 등락부호  */
//	char change[9];              /* 등락폭9(6)  */
//	char chgrate[10];            /* 대비율999.99  */
//	char volume[10];             /* 누적거래량9(9),주  */
//	char totprice[12];           /* 누적거래대금(12),천원M  */
////	char htime[6];               /* 호가잔량기준시간HHMMSS  */
//
//	char offerho1[9];            /* 매도최우선호가9(7)  */
//	char offerho2[9];            /* 매도2차선호가9(7)  */
//	char offerho3[9];            /* 매도3차선호가9(7)  */
//	char offerho4[9];            /* 매도4차선호가9(7)  */
//	char offerho5[9];            /* 매도5차선호가9(7)  */
//	//char offerho6[9];            /* 매도6차선호가9(7)  */
//	//char offerho7[9];            /* 매도7차선호가9(7)  */
//	//char offerho8[9];            /* 매도8차선호가9(7)  */
//	//char offerho9[9];            /* 매도9차선호가9(7)  */
//	//char offerho10[9];           /* 매도10차선호가9(7)  */
//	char offerrem1[9];           /* 매도최우선잔량9(8),주  */
//	char offerrem2[9];           /* 매도2차선잔량9(8),주  */
//	char offerrem3[9];           /* 매도3차선잔량9(8),주  */
//	char offerrem4[9];           /* 매도4차선잔량9(8),주  */
//	char offerrem5[9];           /* 매도5차선잔량9(8),주  */
//	//char offerrem6[9];           /* 매도6우선잔량9(8),주  */
//	//char offerrem7[9];           /* 매도7차선잔량9(8),주  */
//	//char offerrem8[9];           /* 매도8차선잔량9(8),주  */
//	//char offerrem9[9];           /* 매도9차선잔량9(8),주  */
//	//char offerrem10[9];          /* 매도10차선잔량9(8),주  */
//
//	char bidho1[9];              /* 매수최우선호가9(7)  */
//	char bidho2[9];              /* 매수2차선호가9(7)  */
//	char bidho3[9];              /* 매수3차선호가9(7)  */
//	char bidho4[9];              /* 매수4차선호가9(7)  */
//	char bidho5[9];              /* 매수5차선호가9(7)  */
//	//char bidho6[9];              /* 매수6우선호가9(7)  */
//	//char bidho7[9];              /* 매수7차선호가9(7)  */
//	//char bidho8[9];              /* 매수8차선호가9(7)  */
//	//char bidho9[9];              /* 매수9차선호가9(7)  */
//	//char bidho10[9];             /* 매수10차선호가9(7)  */
//	char bidrem1[9];             /* 매수최우선잔량9(8),주  */
//	char bidrem2[9];             /* 매수2차선잔량9(8),주  */
//	char bidrem3[9];             /* 매수3차선잔량9(8),주  */
//	char bidrem4[9];             /* 매수4차선잔량9(8),주  */
//	char bidrem5[9];             /* 매수5차선잔량9(8),주  */
//	//char bidrem6[9];             /* 매수6우선잔량9(8),주  */
//	//char bidrem7[9];             /* 매수7차선잔량9(8),주  */
//	//char bidrem8[9];             /* 매수8차선잔량9(8),주  */
//	//char bidrem9[9];             /* 매수9차선잔량9(8),주  */
//	//char bidrem10[9];            /* 매수10차선잔량9(8),주  */
//
//	char offertotrem[9];         /* 총매도잔량9(8),주  */
//	char bidtotrem[9];           /* 총매수잔량9(8),주  */
//	char offerrem_ot[9];         /* 시간외매도잔량9(8),주  */
//	char bidrem_ot[9];           /* 시간외매수잔량9(8),주  */
//	char hprice[9];              /* 상한가9(9),원M  */
//	char lprice[9];              /* 하한가9(9),원M  */
//	char open[9];                /* 시가9(7),원  */
//	char high[9];                /* 고가9(7),원  */
//	char low[9];                 /* 저가9(7),원  */
//
//	char prevol[10];             /* 전일거래량9(9),주  */
//	char listing[10];            /* 상장(Kospi)발행(Kosdaq)주식수9listing  */
//} D49890_T;
//
//typedef struct _D49890_O
//{
//	char	jmcode			[ 6];		/* 종목코드			*/
//	char	baseprice		[ 9];		/* 기준가  			*/
//	char	expprice		[ 9];		/*  예상 체결가격 */
//	char	expvol			[12];		/*  예상 체결수량 */
//
//	D49890_T	table;		
//} D49890_O;

typedef struct _EU_s0001_OutRec1 {
    char    shcode[6];                            /* 종목코드(6)                     */
    char    recprice[8];                          /* 기준가(8)                       */
    char    yeprice[8];                           /* 예상체결가격(8)                 */
    char    yevolume[8];                          /* 예상체결수량(8)                 */
    char    hname[40];                            /* 종목명(40)                      */
    char    price[8];                             /* 현재가(8)                       */
    char    sign[1];                              /* 현재가전일대비구분(1)           */
    char    change[8];                            /* 현재가전일대비(8)               */
    char    rate[7];                              /* 현재가전일대비등락율(7)         */
    char    volume[12];                           /* 거래량(12)                      */
    char    value[15];                            /* 거래대금(15)                    */
    char    offerho1[8];                          /* 매도호가1(8)                    */
    char    offerho2[8];                          /* 매도호가2(8)                    */
    char    offerho3[8];                          /* 매도호가3(8)                    */
    char    offerho4[8];                          /* 매도호가4(8)                    */
    char    offerho5[8];                          /* 매도호가5(8)                    */
    char    offerrem1[8];                         /* 매도호가수량1(8)                */
    char    offerrem2[8];                         /* 매도호가수량2(8)                */
    char    offerrem3[8];                         /* 매도호가수량3(8)                */
    char    offerrem4[8];                         /* 매도호가수량4(8)                */
    char    offerrem5[8];                         /* 매도호가수량5(8)                */
    char    bidho1[8];                            /* 매수호가1(8)                    */
    char    bidho2[8];                            /* 매수호가2(8)                    */
    char    bidho3[8];                            /* 매수호가3(8)                    */
    char    bidho4[8];                            /* 매수호가4(8)                    */
    char    bidho5[8];                            /* 매수호가5(8)                    */
    char    bidrem1[8];                           /* 매수호가수량1(8)                */
    char    bidrem2[8];                           /* 매수호가수량2(8)                */
    char    bidrem3[8];                           /* 매수호가수량3(8)                */
    char    bidrem4[8];                           /* 매수호가수량4(8)                */
    char    bidrem5[8];                           /* 매수호가수량5(8)                */
    char    totofferrem[8];                       /* 매도호가총수량(8)               */
    char    totbidrem[8];                         /* 매수호가총수량(8)               */
    char    tmofferrem[8];                        /* 시간외매도잔량(8)               */
    char    tmbidrem[8];                          /* 시간외매수잔량(8)               */
    char    uplmtprice[8];                        /* 상한가(8)                       */
    char    dnlmtprice[8];                        /* 하한가(8)                       */
    char    preychange[8];                        /* 예상체결가직전현재가대비(8)     */
    char    high[8];                              /* 고가(8)                         */
    char    low[8];                               /* 저가(8)                         */
    char    jnilvolume[12];                       /* 전일거래량(12)                  */
    char    listing[12];                          /* 상장주식수(12)                  */
} EU_s0001_OutRec1;
