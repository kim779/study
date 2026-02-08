#pragma once

#pragma	pack(1)

/******************************************************************************
* NAME : tr1001
* DESC : 현물 시세 조회
 ******************************************************************************/
struct tr1001_mid
{
	char cod2[6];
};
#define L_tr1001_mid		sizeof(struct tr1001_mid)
struct tr1001_mod
{
    char    cod2[12];       /* RTS 종목코드     */
    char    jmgb[1];        /* 종목구분(+KP, -KQ)*/
    char    curr[9];        /* 현재가           */
    char    diff[9];        /* 전일대비         */
    char    gvol[12];       /* 거래량           */
    char    gamt[12];       /* 거래대금         */
    char    jvol[12];       /* 전일 거래량      */
    char    rate[9];        /* 등락율           */
    char    shga[9];        /* 상한가           */
    char    hhga[9];        /* 하한가           */
    char    gjga[9];        /* 기준가           */
    char    siga[9];        /* 시가             */
    char    koga[9];        /* 고가             */
    char    jega[9];        /* 저가             */
    char    jgbn[3];        /* 증거금율         */
    struct  {
        char    mdga[9];    /* 매도호가         */
        char    msga[9];    /* 매수호가         */
        char    dvol[9];    /* 매도호가수량     */
        char    svol[9];    /* 매수호가수량     */
        char    dcha[9];    /* 직전매도호가수량 */
        char    scha[9];    /* 직전매수호가수량 */
        char    dcnt[9];    /* 매도호가건수     */
        char    scnt[9];    /* 매수호가건수     */
    } hoga[10];             /* 선물/옵션 호가   */
    char    dvol[9];        /* 호가총수량 : 매도*/
    char    svol[9];        /*            : 매수*/
    char    dcha[9];        /* 직전대비총량:매도*/
    char    scha[9];        /*             :매수*/
    char    sdch[9];        /* 잔량차(svol-dvol)*/
    char    sum4[9];        /* 종가합계 : 5일   */
    char    sum9[9];        /* 종가합계 : 9일   */
    char    jggy[9];        /* 증거금율         */
    char    jqty[9];        /* 주문단위         */
};
#define L_tr1001_mod		sizeof(struct tr1001_mod)


/******************************************************************************
* NAME : tr1201
* DESC : 현물 주문
 ******************************************************************************/
struct tr1201_mid
{
	char gubn[2];	/* ^C : 프로그램매매	2014.04.18 추가				*/
	char nrec[4];	/* 반복횟수                                         */
	char odgb[1];	/* 주문유형 : 1:일반 2:선옵 3:예약                  */
	char mkgb[1];	/* Filler                                           */
	char mmgb[1];	/* 매매구분 : 1:매도 2:매수 3:정정 4: 취소          */
	char accn[11];	/* 계좌번호                                         */
	char pswd[8];	/* 비밀번호                                         */
	char ojno[12];	/* 원주문번호                                       */
	char fcod[12];	/* 단축코드                                         */
	char jqty[8];	/* 주문수량                                         */
	char jprc[10];	/* 주문단가                                         */
	char hogb[2];	/* 호가구분                                         */
	char cdgb[1];	/* 조문조건 : 0:X   1:IOC 2: FOK                    */
	char mdgb[1];	/* 정정취소 : 1:일부 2:전부                         */
	char prgb[1];	/* 처리구분 : 'A,B,C,D' : 이하 가변 'X' : 사용안함  */
	char usrf[40];	
	char pggb[2];	/* 프로그램(00:일반 01:차익 03:비차익) 2014.04.18 추가 */
};
#define L_tr1201_mid		sizeof(struct tr1201_mid)
struct tr1201_mod
{
	char nrec[4];
	char jmno[6];	/* 주문번호   */
	char ojno[6];	/* 원주문번호 */
	char emsg[80];	/* 오류메세지 */
};
#define L_tr1201_mod		sizeof(struct tr1201_mod)


/******************************************************************************
* NAME : tr1211
* DESC : 현물 체결내역 조회
 ******************************************************************************/
struct tr1211_mid
{
	char	gubn[1];	/* '>' */						//2015.05.28  구분추가
    char    acno[11];
    char    pswd[8];
    char    date[8];
    char    cod2[12];
    char    fill[5];    /* 시작주문번호             */	//2015.05.28  filler추가
    char    dsgb[1];    /* 0.All 1.매도 2.매수      */
    char    otim[1];    /* 0.All 1.장중 2.시간외    */
    char    sygb[1];    /* 0.All 1.신용 2.담보      */
    char    dllf[1];    /* 0.MAP 1.DLL              */
    char    dlgb[1];    /* 0.All 1.체결 2.미체결    */
    char    sort[1];    /* 1.역순 2.정순            */
	char    juno[10];    /* 시작주문번호             */	//2015.05.28 5 -> 10로 변경
};
#define L_tr1211_mid		sizeof(struct tr1211_mid)
struct tr1211_grid
{
    char    juno[10];    /* 주문   번호      */	//2015.05.28 5 -> 10로 변경
    char    ojno[10];    /* 원주문 번호      */	//2015.05.28 5 -> 10로 변경
    char    cod2[12];   /* 종목코드         */
    char    hnam[40];   /* 종목명           */
    char    odgb[20];   /* 주문구분         */
    char    mcgb[20];   /* 정취구분         */
    char    hogb[20];   /* 호가구분         */
    char    oprc[12];   /* 주문가격         */
    char    oqty[12];   /* 주문수량         */
    char    dprc[12];   /* 체결가격         */
    char    dqty[12];   /* 체결수량         */
    char    tqty[12];   /* 체결수량합       */
    char    wqty[12];   /* 미체결수량       */
    char    stat[20];   /* 접수상태         */
    char    time[8];    /* 주문시간         */
};
#define L_tr1211_grid		sizeof(struct tr1211_grid)
struct tr1211_mod
{
    char    acno[11];                 /* 계좌번호         */
    char    nrec[4];                  /* 반복횟수         */
    struct  tr1211_grid grid[1];
};
#define L_tr1211_mod		sizeof(struct tr1211_mod)


/******************************************************************************
* NAME : tr1221
* DESC : 현물 잔고내역 조회
 ******************************************************************************/
struct  tr1221_mid {   
    char    acno[11];
    char    pswd[8];
    char    cod2[12];
    char    allf[1];    /* 0.신용제외 1.All 2.신용 */
    char    fill[10];
};
#define L_tr1221_mid		sizeof(struct tr1221_mid)
struct  tr1221_grid {   
    char    cod2[12];   /* 종목코드         */
    char    hnam[40];   /* 종목명           */
    char    jgyh[2];    /* 잔고유형         */
    char    jqty[10];   /* 잔고수량         */
    char    xqty[10];   /* 매도가능         */
    char    pamt[10];   /* 매입평균가       */
    char    mamt[15];   /* 매입금액         */
    char    curr[10];   /* 현재가     (*)   */
	char    rate[10];   /* 등락률           */		
	char    diff[10];   /* 대비             */    
    char    camt[15];   /* 평가금액         */
    char    tamt[15];   /* 평가손익   (*)   */
    char    srat[10];   /* 평가수익률 (*)   */
    char    sycd[2];    /* 신용코드         */
    char    sydt[8];    /* 대출일           */
    char    samt[15];   /* 신용금액         */
};
#define L_tr1221_grid		sizeof(struct tr1221_grid)
struct  tr1221_mod   {   
    char    acno[11];                 /* 계좌번호         */
    char    nrec[4];                  /* 반복횟수         */
    struct  tr1221_grid  grid[1];
};
#define L_tr1221_mod		sizeof(struct tr1221_mod)


/******************************************************************************
* NAME : tr1231
* DESC : 현물 주문가능수량조회
 ******************************************************************************/
struct tr1231_mid
{
	char In[5];                             /* 레코드갯수                     */     
	char zBnsTp[1];                         /* 매매구분                       */
	char zAcntNo[20];                       /* 계좌번호                       */
	char zPwd[8];                           /* 비밀번호                       */
	char zIsuNo[12];                        /* 종목번호                       */
	char dOrdPrc[13];                       /* 주문가                         */
	char zCommdaCode[2];                    /* 통신매체코드                   */
	char zLoanPtnCode[2];                   /* 대출유형코드                   */
};
#define L_tr1231_mid		sizeof(struct tr1231_mid)
struct tr1231_mod
{
	struct tr1231_mid mid;
	char Out[5];                            /* 레코드갯수                     */
	char zAcntNm[40];                       /* 계좌명                         */
	char zIsuNm[40];                        /* 종목명                         */
	char zBnsTpNm[10];                      /* 매매구분                       */
	char lDps[16];                          /* 예수금                         */
	char lSubstAmt[16];                     /* 대용금액                       */
	char lMnyMgn[16];                       /* 현금증거금액                   */
	char lSubstMgn[16];                     /* 대용증거금액                   */
	char lRcvblSumAmt[16];                  /* 미수합계금액                   */
	char lRuseOrdAbleAmt[16];               /* 재사용주문가능금액             */
	char lMnyRuseObjAmt[16];                /* 현금재사용대상금액             */
	char lSubstRuseObjAmt[16];              /* 대용재사용대상금액             */
	char lPrdayRuseOrdAbleAmt[16];          /* 전일재사용주문가능금액         */
	char lCrdayRuseOrdAbleAmt[16];          /* 금일재사용주문가능금액         */
	char lMnyOrdAbleAmt[16];                /* 현금주문가능금액               */
	char lSubstOrdAbleAmt[16];              /* 대용주문가능금액               */
	char lCrdtPldgRuseAmt[16];              /* 신용담보재사용금액             */
	char lSeOrdAbleAmt[16];                 /* 거래소금액                     */
	char lKdqOrdAbleAmt[16];                /* 코스닥금액                     */
	char lAbleAmt[16];                      /* 가능금액                       */
	char lMgnRat100pctOrdAbleAmt[16];       /* 증거금률100퍼센트주문가능금액  */
	char lMgnRat100OrdAbleQty[16];          /* 증거금률100퍼센트주문가능수량??*/
	char dLoanPldgRat[9];                   /* 대출담보율                     */
	char dPldgMaintRat[7];                  /* 담보유지비율                   */
	char lOrdAbleQty[16];                   /* 주문가능수량                   */
	char lUnercBuyOrdAmt[16];               /* 미체결매수주문금액             */
	char dCmsnAmtRat[11];                   /* 수수료율                       */
	char lAddCmsnAmtLevyAmt[16];            /* 추가수수료징수금액             */
	char lBalQty[16];                       /* 잔고수량                       */
	char lHtsOrdAbleAmt[16];                /* HTS주문가능금액                */
	char lD1Dps[16];                        /* D+1예수금                      */
	char lD2Dps[16];                        /* D2예수금                       */
	char lPrdayRuseOrdAbleQty[16];          /* 전일재사용주문가능수량         */
	char lCrdayRuseOrdAbleQty[16];          /* 금일재사용주문가능수량         */
	char lCrdtOrdAbleAmt[16];               /* 신용주문가능금액               */
};
#define L_tr1231_mod		sizeof(struct tr1231_mod)


/******************************************************************************
* NAME : tr3001
* DESC : 선물옵션 시세 조회
 ******************************************************************************/
struct tr3001_mid
{
	char cod2[8];
};
#define L_tr3001_mid		sizeof(struct tr3001_mid)
struct  tr3001_mod {   
    char    curr[9];        /* 현재가           */
    char    diff[9];        /* 전일대비         */
    char    gvol[12];       /* 거래량           */
    char    gamt[12];       /* 거래대금         */
    char    rate[9];        /* 등락율           */
    char    shga[9];        /* 상한가           */
    char    hhga[9];        /* 하한가           */
    char    gjga[9];        /* 기준가           */
    char    siga[9];        /* 시가             */
    char    koga[9];        /* 고가             */
    char    jega[9];        /* 저가             */
    struct  {
        char    mdga[9];    /* 매도호가         */
        char    msga[9];    /* 매수호가         */
        char    dvol[9];    /* 매도호가수량     */ 
        char    svol[9];    /* 매수호가수량     */ 
        char    dcnt[9];    /* 매도호가건수     */
        char    scnt[9];    /* 매수호가건수     */
    } hoga[5];  
    char    dvol[9];        /* 호가총수량 : 매도    */
    char    svol[9];        /*            : 매수    */
    char    dcnt[9];        /* 매도호가건수         */
    char    scnt[9];        /* 매수호가건수         */
    char    sdch[9];        /* 잔량차(svol-dvol)    */
    char    mgjv[9];        /* 미결제약정수량       */
};


/******************************************************************************
* NAME : tr3201
* DESC : 선물옵션 주문
 ******************************************************************************/
struct tr3201_mid
{
	char gubn[2];	/* ^C : 프로그램매매	2014.04.18 추가				*/
	char nrec[4];	/* 반복횟수                                         */
	char odgb[1];	/* 주문유형 : 1:일반 2:선옵 3:예약                  */
	char mkgb[1];	/* Filler                                           */
	char mmgb[1];	/* 매매구분 : 1:매도 2:매수 3:정정 4: 취소          */
	char accn[11];	/* 계좌번호                                         */
	char pswd[8];	/* 비밀번호                                         */
	char ojno[12];	/* 원주문번호                                       */
	char fcod[12];	/* 단축코드                                         */
	char jqty[8];	/* 주문수량                                         */
	char jprc[10];	/* 주문단가                                         */
	char hogb[2];	/* 호가구분                                         */
	char cdgb[1];	/* 조문조건 : 0:X   1:IOC 2: FOK                    */
	char mdgb[1];	/* 정정취소 : 1:일부 2:전부                         */
	char prgb[1];	/* 처리구분 : 'A,B,C,D' : 이하 가변 'X' : 사용안함  */
	char usrf[40];	
	char pggb[2];	/* 프로그램(00:일반 01:차익 03:비차익) 2014.04.18 추가 */
};
#define L_tr3201_mid		sizeof(struct tr3201_mid)
struct tr3201_mod
{
	char nrec[4];
	char jmno[6];	/* 주문번호   */
	char ojno[6];	/* 원주문번호 */
	char emsg[80];	/* 오류메세지 */
};
#define L_tr3201_mod		sizeof(struct tr3201_mod)


/******************************************************************************
* NAME : tr3211
* DESC : 선물옵션 체결내역 조회
 ******************************************************************************/
struct tr3211_mid
{
	char acno[11];
	char pswd[8];
	char dlgb[1];		/* 0.All 1.체결 2.미체결                                */
	char juno[6];
	char sort[1];		/* 0.X   1.역순 2.정순                                  */
	char date[8];
	char cod2[8];		/* 종목코드                                             */
	char cpgb[1];		/* 0.All F.선물 C.콜옵션 P.풋옵션 S.스프레드 X.특정종목 */
	char dsgb[1];		/* 0.All 1.매도 2.매수                                  */
	char dllf[1];		/* 0.MAP 1.DLL                                          */	
};
#define L_tr3211_mid		sizeof(struct tr3211_mid)
struct tr3211_grid {
	char    mono[6];    /* 모주문번호       */
	char    juno[6];    /* 주문  번호       */
	char    ojno[6];    /* 원주문 번호      */
	char    cod2[8];    /* 종목코드         */
	char    hnam[30];   /* 종목명           */
	char    odgb[8];    /* 주문구분         */
	char    hogb[20];   /* 주문유형         */
	char    oprc[11];   /* 주문가격         */
	char    oqty[7];    /* 주문수량         */
	char    dlgb[4];    /* 체결구분         */
	char    dprc[11];   /* 체결가격         */
	char    dqty[7];    /* 체결수량         */
	char    dtim[6];    /* 체결시간         */
	char    wqty[7];    /* 미체결수량       */
	char    hqty[7];    /* 정정/취소수량    */
	char    stat[8];    /* 처리상태         */
	char    time[6];    /* 처리시간         */
	char    jseq[6];    /* 접수번호         */
	char    yseq[7];    /* 약정번호         */
	char    ecod[4];    /* 거부코드         */
	char    dseq[4];    /* 체결횟수         */
};
#define L_tr3211_grid		sizeof(struct tr3211_grid)
struct tr3211_mod {
	char    acno[11];                 /* 계좌번호         */
	char    nrec[4];                  /* 반복횟수         */
	struct  tr3211_grid grid[1];
};
#define L_tr3211_mod		sizeof(struct tr3211_mod)


/******************************************************************************
* NAME : tr3221
* DESC : 선물옵션 잔고내역 조회
 ******************************************************************************/
struct tr3221_mid {
	char acno[11];
	char pswd[8];
	char date[8];
	char cod2[8];
};
#define L_tr3221_mid		sizeof(struct tr3221_mid)
struct tr3221_grid {
	char    cod2[8];    /* 종목코드         */
	char    hnam[30];   /* 종목명           */
	char    dsgb[6];    /* 구분             */
	char    jqty[10];   /* 보유수량         */
	char    xqty[10];   /* 청산가능수량     */
	char    pamt[10];   /* 평균가/정산가    */
	char    curr[10];   /* 현재가           */
	char    diff[10];   /* 전일대비         */
	char    camt[15];   /* 평가금액         */
	char    tamt[15];   /* 평가손익         */
	char    srat[10];   /* 수익률           */
	char    mamt[15];   /* 매입금액         */
};
#define L_tr3221_grid		sizeof(struct tr3221_grid)
struct tr3221_mod {
	char    acno[11];                 /* 계좌번호         */
	char    nrec[4];                  /* 반복횟수         */
	struct  tr3221_grid grid[1];
};
#define L_tr3221_mod		sizeof(struct tr3221_mod)


/******************************************************************************
* NAME : tr3231
* DESC : 선물옵션 주문가능수량 조회
 ******************************************************************************/
struct tr3231_mid
{
	char In[5];                    /* 레코드갯수            */
	char zAcntNo[20];              /* 계좌번호              */
	char zPwd[8];                  /* 비밀번호              */
	char zQryTp[1];                /* 조회구분              */
	char lOrdAmt[16];              /* 주문금액              */
	char dRatVal[19];              /* 비율값                */
	char zFnoIsuNo[32];            /* 선물옵션종목번호      */
	char zBnsTp[1];                /* 매매구분              */
	char dOrdPrc[13];              /* 주문가                */
	char zFnoOrdprcPtnCode[2];     /* 선물옵션호가유형코드  */
};
#define L_tr3231_mid		sizeof(struct tr3231_mid)
struct tr3231_mod
{
	struct tr3231_mid mid;
	char Out[5];                   /* 레코드갯수            */
	char zAcntNm[40];              /* 계좌명                */
	char zQryDt[8];                /* 조회일                */
	char dCurPrc[13];              /* 현재가                */
	char lOrdAbleQty[16];          /* 주문가능수량          */
	char lNewOrdAbleQty[16];       /* 신규주문가능수량      */
	char lLqdtOrdAbleQty[16];      /* 청산주문가능수량      */
	char lUsePreargMgn[16];        /* 사용예정증거금액      */
	char lUsePreargMnyMgn[16];     /* 사용예정현금증거금액  */
	char lOrdAbleAmt[16];          /* 주문가능금액          */
	char lMnyOrdAbleAmt[16];       /* 현금주문가능금액      */
};
#define L_tr3231_mod		sizeof(struct tr3231_mod)

/******************************************************************************
* NAME : tr3232
* DESC : 선물옵션 당일손익 조회
 ******************************************************************************/
struct tr3232_mid
{
	char zIn[5];		/* 레코드갯수 */
	char zAcntNo[20];	/* 계좌번호 */
	char zPwd[8];		/* 비밀번호 */
	char zSrtDt[8];		/* 시작일 */
	char zEndDt[8];		/* 종료일 */
	char zFtsubtTp[1];	/* 선물대용구분 */
	char zErnratTp[1];	/* 수익률구분 */
};
#define L_tr3232_mid		sizeof(struct tr3232_mid)
struct tr3232_mod
{
	struct tr3232_mid mid;
	char zOut[5];					/* 레코드갯수 */
	char zAcntNm[40];				/* 계좌명 */
	char zInvstPramtAvrbalAmt[16];	/* 투자원금평잔금액 */
	char zPnlAmt[16];				/* 손익금액 */
	char zErnRat[12];				/* 수익률 */
	char zRat01[19];				/* 비율값 */
	char zFdDpsastAmt[16];			/* 기초예탁자산금액 */
	char zEotDpsastAmt[16];			/* 기말예탁자산금액 */
	char zInAmt[16];				/* 입금액 */
	char zOutAmt[16];				/* 출금액 */
	char zBnsctrAmt[16];			/* 매매약정금액 */
	char zFutsAdjstDfamt[16];		/* 선물정산차금 */
	char zBnsCmsnAmt[16];			/* 매매수수료 */
	char zOptBnsplAmt[16];			/* 옵션매매손익금액 */
	char zDpsamtUtlfee[16];			/* 예탁금이용료 */
	char zOptEvalPnl[16];			/* 옵션평가손익 */
	char zAcntOpnDt[8];				/* 계좌개설일 */
	char zAcntCloseDt[8];			/* 계좌폐쇄일 */
};
#define L_tr3232_mod		sizeof(struct tr3232_mod)

/******************************************************************************
* NAME : tr3411
* DESC : 옵션 전종목 시세조회
* DATE : 2013.04.01 
 ******************************************************************************/
struct  tr3411_grid {
    char    c_rtcd[8];  /* 콜RTS CODE           */
    char    c_mdif[8];  /* 콜증감(전일)         */
    char    c_mgjv[8];  /* 콜미결제약정         */
    char    c_gvol[8];  /* 콜거래량             */
    char    c_msga[8];  /* 콜매수               */
    char    c_mdga[8];  /* 콜매도               */
    char    c_rate[8];  /* 콜등락률             */
    char    c_diff[7];  /* 콜대비               */
    char    c_curr[8];  /* 콜현재가             */
    char    hang[8];    /* 행사가               */
    char    jihs[8];    /* 지수환산             */
    char    p_rtcd[8];  /* 풋RTS CODE           */
    char    p_curr[8];  /* 풋현재가             */
    char    p_diff[7];  /* 풋대비               */
    char    p_rate[8];  /* 풋등락률             */
    char    p_mdga[8];  /* 풋매도               */
    char    p_msga[8];  /* 풋매수               */
    char    p_gvol[8];  /* 풋거래량             */
    char    p_mgjv[8];  /* 풋미결제약정         */
    char    p_mdif[8];  /* 풋증감(전일)         */
};

struct tr3411_mid{
	char ysgb;	//예상가 구분 default 0
	char func;	// start "F"
	char mymd[6];	// 만기월 ex. 201304
};

#define	MAX_R		30

struct  tr3411_mod {
    char    atmg[8];    /* ATM  */
    char    nrec[4];
    struct  tr3411_grid    grid[MAX_R];
};


/******************************************************************************
* NAME : tr1801
* DESC : 복수종목 시세조회
 ******************************************************************************/
struct tr1801_grid
{
    char code[6];	/* RTS Symbol  */
    char hnam[20];  /* 한글명      */
    char curr[7];   /* 현재가      */
	char giho[1];   /* 등락부호    1.상한가, 2.상승, 3.보합, 4.하한가, 5.하락*/
    char diff[7];   /* 전일대비    */
    char mdga[7];	/* 매도호가    */
    char msga[7];   /* 맴수호가    */
    char gvol[12];  /* 거래량      */
};
#define L_tr1801_grid		sizeof(struct tr1801_grid)
struct tr1801_mod
{
    char nrec[4];
    struct tr1801_grid grid[1];
};
#define L_tr1801_mod		sizeof(struct tr1801_mod)

/******************************************************************************
* NAME : tr1802
* DESC : 복수선옵 시세조회
 ******************************************************************************/
struct tr1802_grid
{
    char code[8];	/* RTS Symbol  */
    char seqn[1];	/* 종목SEQN    */
	char giho[1];   /* 등락부호    1.상한가, 2.상승, 3.보합, 4.하한가, 5.하락*/
    char diff[6];	/* 전일대비    */
    char curr[6];	/* 현재가      */
    char mdga[6];	/* 매도호가    */
    char msga[6];	/* 매수호가    */
    char gvol[12];	/* 거래량      */
    char mgjv[12];  /* 미결약정    */
};
#define L_tr1802_grid		sizeof(struct tr1802_grid)
struct tr1802_mod
{
    char nrec[4];
    struct tr1802_grid grid[1];
};
#define L_tr1802_mod		sizeof(struct tr1802_mod)

/******************************************************************************
* NAME : tr1803
* DESC : 복수업종 등락조회
 ******************************************************************************/
struct tr1803_grid
{
    char jggb[1];   /* 장구분 */
    char time[6];   /* 시간   */
    char ttjs[5];   /* 전체 종목수 */
    char trjs[5];   /* 거래 종목수 */
    char ssjs[5];   /* 상승 종목수 */
    char shjs[5];   /* 상한 종목수 */
    char hrjs[5];   /* 하락 종목수 */
    char hhjs[5];   /* 하한 종목수 */
    char bhjs[5];   /* 보합 종목수 */
    char gsjs[5];   /* 기세 형성 종목수 */
    char gsss[5];   /* 기세 상승 종목수 */
    char ghjs[5];   /* 기세 하락 종목수 */
    char gvol[10];  /* 거래량 (천주) */
    char gamt[10];  /* 거래대금 (백만) */
};
#define L_tr1803_grid		sizeof(struct tr1803_grid)
struct tr1803_mod
{
    char nrec[4];
    struct tr1803_grid grid[1];
};
#define L_tr1803_mod		sizeof(struct tr1803_mod)

/******************************************************************************
* NAME : tr1804
* DESC : 복수업종 지수조회
 ******************************************************************************/
struct tr1804_grid
{
    char symb[16];		/* RTS Symbol */
    char jggb[1];		/* 장구분     */
    char time[6];		/* 시간       */
    char curr[7];       /* 현재가     */
	char giho[1];		/* 등락부호    '+', '-'로 표시됨*/
    char diff[7];       /* 전일대비   */
    char gvol[12];      /* 거래량     */
    char gamt[12];      /* 거래대금   */
};
#define L_tr1804_grid		sizeof(struct tr1804_grid)
struct tr1804_mod
{
    char nrec[4];
    struct tr1804_grid grid[1];
};
#define L_tr1804_mod		sizeof(struct tr1804_mod)

/******************************************************************************
* NAME : tr2001
* DESC : 투자자별 매매동향
 ******************************************************************************/
struct tr2001_grid
{
    char code[10];	/* RTS Symbol  */
    char dvol[12];	/* 매도수량 333 */
	char svol[12];  /* 매수수량 334 */
    char rvol[12];	/* 순매수수량 343 */
    char damt[12];	/* 매도금액 339 */
    char samt[12];	/* 매수금액 340 */
    char ramt[12];	/* 순매수금액 344 */
};
#define L_tr2001_grid		sizeof(struct tr2001_grid)
struct tr2001_mod
{
    struct tr2001_grid grid[12];	//외국인, 개인, 기관계, 투신, 금융투자, 보험, 은행, 기타금융, 연기금, 사모, 국가, 기타법인
};
#define L_tr2001_mod		sizeof(struct tr2001_mod)


/******************************************************************************
* NAME : GetCode("hjcode")
* DESC : 현물종목마스터 [ hjcode * n ]
 ******************************************************************************/
struct hjcode {
	char	codx[12];		/* 코드(7) : A######			*/
							/* 코드(9) : 신주인수권, 증서	*/
	char	hnam[40];		/* 한글명						*/
	char	enam[40];		/* 영문명						*/
	char	ecnf;			/* ECN (1:거래가능)				*/
	char	jsiz;			/* 업종자본금 대중소			*/
							/* 장내(대: 38, 중: 39, 소: 40)	*/  
							/* 장외(1: KOSDAQ 100)		 	*/
							/* 장외(2: KOSDAQ MID 300)		*/
							/* 장외(3: KOSDAQ SMALL)		*/
							/* ############################ */
							/* 배당지수 : (jsiz += 41)		*/
							/* 신주인수권종목 FULL코드 용	*/
	char	symb[7];		/* SYMBOL						*/
	char	size;    		/* 시가총액규모(2, 3, 4)		*/
	char	ucdm;			/* 업종중분류       99:미분류	*/
	char	ucds;			/* 업종소분류       99:미분류	*/
							/* KOSDAQ 7:벤쳐				*/
	char	jjug;			/* 제조업 구분(27: 제조업)		*/
							/* KOSDAQ 벤쳐 종합(=2)			*/
	char	kpgb;			/* KOSPI200/KOSDAQ 구분			*/
							/* 0:미포함 1:K200 2:K100 3:K50 */
							/* 0:미포함 1:KQ50				*/
	char	kosd;			/* 종목구분						*/
							/* 0:장내  10:장외  30:제3시장	*/
	char	ssgb;			/* 소속구분						*/
							/* 0:일반						*/
							/* 3:외국인예탁증서				*/
							/* 4:뮤추얼펀드					*/
							/* 5:신주인수권(중권)			*/
							/* 6:리츠						*/
							/* 7:신주인수권(증서)			*/
							/* 8:ETF						*/
							/* 9:하이일드펀드(수익증권)		*/
	char	ucmd;			/* 우선주구분 0:보통주 5:우선주	*/
	char	itgb;			/* KOSPI-IT/KQ-IT50구분(1:채택)	*/
	char	wsgb;			/* 0:일반 1:지배구조우수기업	*/
	char	jqty;			/* 기본주문단위					*/
	char	star;			/* Star 종목구분				*/
	char	unio;			/* 통합지수종목 구분			*/
							/* 0: 일반, 1:통합지수			*/
	char	jchk;			/* 종목검색용 구분				*/
							/* 0x01     불성실공시종목   	*/
							/* 0x02     신규상장종목     	*/
							/* 0x04     권리락종목       	*/
							/* 0x08     우선주종목       	*/
							/* 0x10     관리종목         	*/
							/* 0x20     감리종목         	*/
							/* 0x40     거래정지종목     	*/
							/* 0x80     투자유의종목     	*/
	char	elwf;			/* 0: 일반, 1: 기초자산 		*/
	char	sjjs[10];		/* 상장주식수(단위:주)			*/
	char	fill[10];		/* reserved						*/
};
#define L_hjcode		sizeof(struct hjcode)

/******************************************************************************
* NAME : GetCode("fjcode")
* DESC : 선물종목마스터 [ fjcode * n ]
 ******************************************************************************/
struct fjcode {			/* 선물종목코드 정보			*/
	char	codx[8];		/* 단축코드 (8) 				*/
	char	hnam[20];		/* 한글명						*/
	char	enam[20];		/* 영문명						*/
	char	mchk;			/* 매매유형허용					*/
							/* 0x01:시장가허용				*/
							/* 0x02:조건부지정가허용		*/
							/* 0x04:최유리지정가허용		*/
};
#define L_fjcode		sizeof(struct fjcode)

/******************************************************************************
* NAME : GetCode("opcode")
* DESC : 옵션종목마스터 [opcode_h + (opcode * n)]
 ******************************************************************************/
struct opcode_h {			/* 옵션종목코드 HEADER 정보		*/
	char	chdr[11][6];		/* CALL OPTION 종목 HEADER		*/
	char	phdr[11][6];		/* PUT OPTION 종목 HEADER		*/
};

struct opcode {			/* 옵션종목코드 정보			*/
	char	hsga[5];		/* 행사가격(999V99)				*/
	char	atmg;			/* ATM구분(1:ATM, 2:ITM, 3:OTM)	*/
	struct	{
		char	yorn[1];	/* 종목 코드 유무				*/
		char	codx[9];	/* 단축종목코드					*/
		char	hnam[20];	/* 한글종목명					*/
		char	enam[20];	/* 영문명						*/
		char	mchk;		/* 매매유형허용					*/
							/* 0x01:시장가허용				*/
							/* 0x02:조건부지정가허용		*/
							/* 0x04:최유리지정가허용		*/
	} call[11]; 
	struct	{
		char	yorn[1];	/* 종목 코드 유무				*/
		char	codx[9];	/* 단축종목코드					*/
		char	hnam[20];	/* 한글종목명					*/
		char	enam[20];	/* 영문명						*/
		char	mchk;		/* 매매유형허용					*/
							/* 0x01:시장가허용				*/
							/* 0x02:조건부지정가허용		*/
							/* 0x04:최유리지정가허용		*/
	} put[11]; 
};
#define L_opcode		sizeof(struct opcode)

#define	N_LP	(5)
struct elwcode {			/* ELW종목코드	정보				*/
	char	codx[12];		/* 코드(6) : J######				*/
	char	hnam[32];		/* 한글명							*/
	char	enam[20];		/* 영문명							*/
	char	symb[7];		/* SYMBOL							*/
	char	lphn[N_LP][3];	/* ELW - LP 회원번호 1~5			*/
	char	lpse[5];		/* ELW 스프레드	   (999V99)			*/	
	char 	hbge[20];		/* ELW 한글발행기관명				*/
	char	ebge[20]; 		/* ELW 영문발행기관명				*/
	char	bgce[12];		/* ELW 발행기관코드					*/
	char	gcjs[N_LP][12];	/* ELW 기초자산 1~5					*/	
							/* KOSPI100:표준코드,KOSPI200:"KOSPI200    " */
	char	grat[N_LP][5];	/* ELW 기초자산구성비율 1~5 (999V99)*/
	char	gcge[10];		/* ELW 기초가격	     (99999999V99) 	*/
	char	hsge[10];		/* ELW 행사가격	     (99999999V99) 	*/	
	char	krye[2];		/* ELW 권리유형						*/
							/* 01:CALL,02:PUT,03:기타  		 	*/
	char 	khbe[2];		/* ELW 권리행사방식					*/
							/* 01:유럽형;02:미국형;03:기타	 	*/
	char	edte[8];		/* 최종거래일   YYYYMMDD			*/
	char	fill[40];		/* reserved							*/
};
#define L_elwcode		sizeof(struct elwcode)

struct Jango{
	char flag;		// for Window Message
	char acno[12];	// 계좌번호
	char code[12];	// 종목코드
	char name[40];	// 종목명
	char gubn[10];	// 구분
	int  jqty;		// 보유수량
	int  xqty;		// 청산가능수량
	int  pprc;		// 평균가
	double camt;	// 평가금액
	double tamt;	// 평가손익
	double srat;	// 수익률
	double mamt;	// 매입금액
};


#pragma	pack()
