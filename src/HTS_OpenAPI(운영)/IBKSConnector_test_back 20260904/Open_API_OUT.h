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
	char    mkgb[1];   /* 시장구분         */
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
	char    stopPrc[12];   /* 스톱가격       */
	char    stopStat[20];   /* 스톱유형       */
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

	
/* COSAQ005 해외시장주문체결조회 */  
struct cosaq005_mid	//TR5002
{
	char In[5];			// 1. 입력건수 (X(5))
	char BnsTp[1];			// 2. 매매구분 (X(1))
	char ExecYn[1];			// 3. 체결여부 (X(1))
	char AcntNo[20];		// 4. 계좌번호 (X(20))
	char Pwd[8];			// 5. 비밀번호 (X(8))
	char SrtOrdNo2[10];		// 6. 시작주문번호2 (S9(9))
	char SrtDt[8];			// 7. 시작일 (X(8))
	char EndDt[8];			// 8. 종료일 (X(8))
	char QryTp[1];			// 9. 조회구분 (X(1))
	char RcptStatTp[1];		// 10. 접수상태구분 (X(1))
	char IsuNo[12];			// 11. 종목번호 (X(12))
	char OrdMktCode[2];		// 12. 주문시장코드 (X(2))
	char ReqMktCode[2];		// 13. 등록시장코드 (X(2))
	char BkseqTp[1];		// 14. 역순구분 (X(1))
	char CommdaCode[2];		// 15. 통신매체코드 (X(2))
	char ChgFnoOrdprcPtnCode[2];    // 16. 변경호가유형코드 (X(2))
	char OrdprcPtnCode[2];		// 17. 호가유형코드 (X(2))
	char ClntClssCode[2];		// 18. 고객구분코드 (X(2))
	char MngtBrnNo[3];		// 19. 관리점번호 (X(3))
	char StkTp[1];			// 20. 주식구분 (X(1))
	char LoanTp[1];			// 21. 대출구분 (X(1))
};
#define L_cosaq005_mid sizeof(struct cosaq005_mid)


struct cosaq005_rec
{
	char OrdNo[10];					// 1. 주문번호 (S9(9))
	char ShtnHangulssuNm[40];			// 2. 단축한글종목명 (X(40))
	char OrgOrdNo[10];				// 3. 원주문번호 (S9(9))
	char OrdPtnNm[40];				// 4. 주문유형명 (X(40))
	char ShtnIsuNo[9];				// 5. 단축종목번호 (X(9))
	char IsuNo[12];					// 6. 종목번호 (X(12))
	char OrdPtnCode[2];				// 7. 주문유형코드 (X(2))
	char HangulsuNm80[100];				// 8. 한글종목명80 (X(100))
	char AcntNo[20];				// 9. 계좌번호 (X(20))
	char AcntNm[40];				// 10. 계좌명 (X(40))
	char OrdQty[16];				// 11. 주문수량 (S9(15))
	char FrctSellOrdQty[23];			// 12. 소수점매매 매도주문수량 (S9(15),9(6))
	char ExecQty[16];				// 13. 체결수량 (S9(15))
	char FrctBuyExecQty[23];			// 14. 소수점매매 매수체결수량 (S9(15),9(6))
	char TotExecQty[16];				// 15. 전체체결수량 (S9(15))
	char FrctSellExecQty[23];			// 16. 소수점매매 매도체결수량 (S9(15),9(6))
	char BnsTp[1];					// 17. 매매구분 (X(1))
	char MrcAbleQty[16];				// 18. 정정취소가능수량 (S9(15))
	char FrctCnclQty[23];				// 19. 소수점매매 취소수량 (S9(15),9(6))
	char OvrsOrdPrc[21];				// 20. 해외주문가격 (S9(12),9(7))
	char UnercQty[16];				// 21. 미체결수량 (S9(15))
	char FrctPrsltQty[23];				// 22. 소수점매매 신청수량 (S9(15),9(6))
	char OvrsExecPrc[21];				// 23. 해외증권체결가격 (S9(12),9(7))
	char OrdMktCode[2];				// 24. 주문시장코드 (X(2))
	char CnclQty[16];				// 25. 확인수량 (S9(15))
	char FrctTrstPrsltQty[23];			// 26. 소수점매매 자기 신청수량 (S9(15),9(6))
	char OrdTime[9];				// 27. 주문시각 (X(9))
	char ExecTime[9];				// 28. 체결시각 (X(9))
	char CommdaCode[2];				// 29. 통신매체코드 (X(2))
	char MrcTptNm[10];				// 30. 정정취소주문명 (X(10))
	char CommdaNm[40];				// 31. 통신매체명 (X(40))
	char MktNm[40];					// 32. 시장명 (X(40))
	char OrdprcPtnNm[40];				// 33. 호가유형명 (X(40))
	char CrcyCode[3];				// 34. 통화코드 (X(3))
	char OrdTxPtnNm[50];				// 35. 주문처리유형명 (X(50))
	char ClntNo[20];				// 36. 고객번호 (X(20))
	char RcvExecId[18];				// 37. 수신체결ID (X(18))
	char UserId[16];				// 38. 사용자ID (X(16))
	char ExecDt[8];					// 39. 체결일 (X(8))
	char OrdDt[8];					// 40. 주문일 (X(8))
	char ClntClssCode[2];				// 41. 고객구분코드 (X(2))
	char OppBrnNm[40];				// 42. 상대방지점명 (X(40))
	char BrnNo[3];					// 43. 지점번호 (X(3))
	char BrnNm[40];					// 44  지점명
	char RjtRsn[100];				// 45. 거부사유 (X(100))
	char CvrgTpNm[10];				// 46. 반대매매구분명 (X(10))
};
#define L_cosaq005_rec sizeof(struct cosaq005_rec)


struct cosaq005_mod
{
	char count[5]; // 출력건수 (X(5))
	struct cosaq005_rec rec[1];
};
#define L_cosaq005_mod sizeof(struct cosaq005_mod)


/* COSAQ027 해외증권 예수금 조회 */
struct cosaq027_mid	//TR5000
{
	char In[5];			// 1. 입력건수 (X(5))
	char AcntNo[20];		// 2. 계좌번호 (X(20))
	char Pwd[8];			// 3. 비밀번호 (X(8))
	char CrcyCode[3];		// 4. 통화코드 (X(3))
	char prcptntp[1];		// 5. 가격유형구분
};
#define L_cosaq027_mid sizeof(struct cosaq027_mid)

struct cosaq027_out1
{
	char CrcyCode[3];			// 6.   통화코드 (X(3))
	char FcurrBuyAdjustAmt1[17];		// 7.   외화매수정산금1 (S9(11),9(4))
	char FcurrBuyAdjustAmt2[17];		// 8.   외화매수정산금2 (S9(11),9(4))
	char FcurrBuyAdjustAmt3[17];		// 9.   외화매수정산금3 (S9(11),9(4))
	char FcurrBuyAdjustAmt4[17];		// 10.  외화매수정산금4 (S9(11),9(4))
	char FcurrSellAdjustAmt1[17];		// 11.  외화매도정산금1 (S9(11),9(4))
	char FcurrSellAdjustAmt2[17];		// 12.  외화매도정산금2 (S9(11),9(4))
	char FcurrSellAdjustAmt3[17];		// 13.  외화매도정산금3 (S9(11),9(4))
	char FcurrSellAdjustAmt4[17];		// 14.  외화매도정산금4 (S9(11),9(4))
	char PrsmptFcurrDps1[17];		// 15.  추정외화예수금1 (S9(11),9(4))
	char PrsmptFcurrDps2[17];		// 16.  추정외화예수금2 (S9(11),9(4))
	char PrsmptFcurrDps3[17];		// 17.  추정외화예수금3 (S9(11),9(4))
	char PrsmptFcurrDps4[17];		// 18.  추정외화예수금4 (S9(11),9(4))
	char PrsmptMxchgAbleAmt1[17];		// 19.  추정환전가능금1 (S9(11),9(4))
	char PrsmptMxchgAbleAmt2[17];		// 20.  추정환전가능금2 (S9(11),9(4))
	char PrsmptMxchgAbleAmt3[17];		// 21.  추정환전가능금3 (S9(11),9(4))
	char PrsmptMxchgAbleAmt4[17];		// 22.  추정환전가능금4 (S9(11),9(4))
};
#define L_cosaq027_out1 sizeof(struct cosaq027_out1)

struct cosaq027_out2
{
	char CntryNm[40];		// 24. 국가명 (X(40))
	char CrcyCode[3];		// 25. 통화코드 (X(3))
	char FcurrAmt1[17];		// 26. 외화금액1 (S9(11),9(4))
	char FcurrDps[17];		// 27. 외화예수금 (S9(11),9(4))
	char FcurrOrdAbleAmt[17];	// 28. 외화주문가능금액 (S9(11),9(4))
	char FcurrAmt2[17];		// 29. 외화금액2 (S9(11),9(4))
	char FcurrOrdAmt[17];		// 30. 외화주문금액 (S9(11),9(4))
	char FcurrPldgAmt[17];		// 31. 외화담보금액 (S9(11),9(4))
	char ExecRuseFcurrAmt[17];	// 32. 체결재사용외화금액 (S9(11),9(4))
	char FcurrMxchgAbleAmt[17];	// 33. 외화환전가능금 (S9(11),9(4)
	char FcurrRcvblAmt[20];		// 34. 미수금 (S9(14),9(4))
	char FcurrFundAmt[17];		// 35. 외화상환금액 (S9(11),9(4))
};
#define L_cosaq027_out2 sizeof(struct cosaq027_out2)

struct cosaq027_out3
{
	char WwonDpsBalAmt[16];		// 37. 원화예수금잔고금액 (S9(15))
	char MnyoutAbleAmt[16];		// 38. 출금가능금액 (S9(15))
	char WwonConvTrdAmt[16];	// 39. 원화환산거래금액 (S9(15))
	char BnsMgn[16];		// 40. 매매증거금 (S9(15))
};
#define L_cosaq027_out3 sizeof(struct cosaq027_out3)


struct cosaq027_out4
{
	char NfCode[2];			// 42. 아이템명: NfCode, 내외국인코드 (X(2))
};
#define L_cosaq027_out4 sizeof(struct cosaq027_out4)

struct cosaq027_out5
{
	char DpstgTotamt[16];		// 예탁총액
	char BscDpsamt[16];		// 기본예탁금
	char WonConvAmt[16];		// 원화환산금액
	char BscAmt[16];		// 기본금액
	char WonAmt[16];		// 원화금액
};


struct cosaq027_mod
{
	char count1[5];
	struct cosaq027_out1 out1;
	char count2[5];
	struct cosaq027_out2 out2;
	char count3[5];
	struct cosaq027_out3 out3;
	char count4[5];
	struct cosaq027_out4 out4;
};
#define L_cosaq027_mod sizeof(struct cosaq027_mod)

//TR5001
struct cosaq003_mid
{
	char In[5];		// 1. 입력건수 (X(5))
	char AcntNo[20];	// 2. 계좌번호 (X(20))
	char Pwd[8];		// 3. 비밀번호 (X(8))
	char IsuNo[12];		// 4. 종목번호 (X(12))
	char OrdMktCode[2];	// 5. 주문시장코드 (X(2))
	char QryTp[1];		// 6. 조회구분 (X(1))
	char QryDt[8];		// 7. 조회일자 (X(8))
	char MktTp[1];		// 8. 시장구분 (X(1))
};
#define L_cosaq003_mid sizeof(struct cosaq003_mid)

struct cosaq003_out
{
	char HangulsuNm80[100];			// 10. HangulsuNm80, 한글종목명80 (X(100))
	char IsuNo[12];				// 11. IsuNo, 종목번호 (X(12))
	char ShtnIsuNo[9];			// 12. ShtnIsuNo, 단축종목번호 (X(9))
	char HldQty[16];			// 13. HldQty, 보유수량 (S9(15))
	char HanglCrcyNm[40];			// 14. HanglCrcyNm, 한글통화명 (X(40))
	char FcurrBookPrc[15];			// 15. FcurrBookPrc, 장부단가 (S9(9),9(4))
	char FcurrBookAmt[17];			// 16. FcurrBookAmt, 외화장부금액 (S9(11),9(4))
	char SellAbleQty[16];			// 17. SellAbleQty, 매도가능수량 (S9(15))
	char BuyExecQty[16];			// 18. BuyExecQty, 매수체결수량 (S9(15))
	char PrdpsQty[16];			// 19. PrdpsQty, 보호예수수량 (S9(15))
	char SellOrdQty[16];			// 20. SellOrdQty, 매도주문수량 (S9(15))
	char SellExecQty[16];			// 21. SellExecQty, 매도체결수량 (S9(15))
	char NowMprc[15];			// 22. NowMprc, 현재시가 (S9(9),9(4))
	char ErnRat[12];			// 23. ErnRat, 수익률 (S9(4),9(6))
	char FcurrMktCode[2];			// 24. FcurrMktCode, 외화시장코드 (X(2))
	char OnlineBnsAbleYn[1];		// 25. OnlineBnsAbleYn, 온라인매매가능여부 (X(1))
	char FcurrEvalAmt[17];			// 26. FcurrEvalAmt, 외화평가금액 (S9(11),9(4))
	char FcurrEvalPflt[17];			// 27. FcurrEvalPflt, 외화평가손익 (S9(11),9(4))
	char WonEvalAmt[16];			// 28. WonEvalAmt, 원화평가금액 (S9(15))
	char WonEvalLoss[16];			// 29. WonEvalLoss, 원화평가손익 (S9(15))
	char BaseXchrat[11];			// 30. BaseXchrat, 기준환율 (S9(5),9(4))
	char BalBaseTp[1];			// 31. BalBaseTp, 잔고기준구분 (X(1))
	char BalPtnNm[40];			// 32. BalPtnNm, 잔고구분명 (X(40))
	char CrdtAmt[16];			// 33. CrdtAmt, 신용금액 (S9(15))
	char LoanDt[8];				// 34. LoanDt, 대출일 (X(8))
	char DueDt[8];				// 35. DueDt, 만기일 (X(8))
};
#define L_cosaq003_out sizeof(struct cosaq003_out)


struct cosaq003_mod
{
	char Out[5];				// 9. Out, 출력건수 (X(5))=
	struct cosaq003_out rec[1];
};
#define L_cosaq003_mod sizeof(struct cosaq003_mod)


struct cosaq208_mid	//TR5003
{
	char In[5];				// 1. In, 입력건수 (X(5))
	char AcntNo[20];			// 2. AcntNo, 계좌번호 (X(20))
	char Pwd[8];				// 3. Pwd, 비밀번호 (X(8))
	char SrtDt[8];				// 4. SrtDt, 시작일자 (X(8))
	char EndDate[8];			// 5. EndDate, 종료일자 (X(8))
	char IsuNo[12];				// 6. IsuNo, 종목번호 (X(12))
	char TrdNo[10];				// 7. TrdNo, 거래번호 (S9(9))
	char CancTp[1];				// 8. CancTp, 취소구분 (X(1))
	char TrdTpCode[2];			// 9. TrdTpCode, 거래구분코드 (X(2))
	char StkTp[1];				// 10. StkTp, 주식구분 (X(1))
};
#define L_cosaq208_mid sizeof(struct cosaq208_mid)

struct cosaq208_rec
{
	char TrdDt[8];				// 14. TrdDt, 거래일 (X(8))
	char TrdNo[10];				// 15. TrdNo, 거래번호 (S9(9))
	char CancTpNm[20];			// 16. CancTpNm, 취소구분 (X(20))
	char IsuNo[12];				// 17. IsuNo, 종목번호 (X(12))
	char ShtnIsuNo[9];			// 18. ShtnIsuNo, 단축종목번호 (X(9))
	char IsuNm[40];				// 19. IsuNm, 종목명 (X(40))
	char CrcyCode[3];			// 20. CrcyCode, 통화코드 (X(3))
	char FcurrAmt[17];			// 21. FcurrAmt, 외화금액 (S9(11),9(4))
	char WonAmt[16];			// 22. WonAmt, 원화금액 (S9(15))
	char FcurrDpsBalAmt[16];		// 23. FcurrDpsBalAmt, 외화예수금잔고금액 (S9(11),9(4))
	char WwonDpsBalAmt[16];			// 24. WwonDpsBalAmt, 원화예수금잔고금액 (S9(15))
	char SecRbalQty[16];			// 25. SecRbalQty, 유가증권잔수량 (S9(15))
	char WonBoaAmt[16];			// 26. WonBoaAmt, 원화표준금액 (S9(15))
	char FcurrBoaAmt[17];			// 27. FcurrBoaAmt, 외화표준금액 (S9(11),9(4))
	char Position[22];			// 28. Position, 수량 (S9(14),9(6))
	char MdaCodeNm[40];			// 29. MdaCodeNm, 매체코드명 (X(40))
	char MgnUseTpNm[20];			// 30. MgnUseTpNm, 증거금사용구분 (X(20))
	char OrdDt[8];				// 31. OrdDt, 주문일 (X(8))
	char OrdNo[10];				// 32. OrdNo, 주문번호 (S9(9))
	char OrgTrdNo[10];			// 33. OrgTrdNo, 원거래번호 (S9(9))
	char TrdXchrat[11];			// 34. TrdXchrat, 거래환율 (S9(5),9(4))
	char SmryNm[40];			// 35. SmryNm, 적요명 (X(40))
	char PremFcurrAmt[17];			// 36. PremFcurrAmt, 프리미엄외화금액 (S9(11),9(4))
	char PassIntrstFcurrAmt[17];		// 37. PassIntrstFcurrAmt, 경과이자외화금액 (S9(11),9(4))
	char PassIntrstWonAmt[16];		// 38. PassIntrstWonAmt, 경과이자원화금액 (S9(15))
	char DomCmsnAmt[16];			// 39. DomCmsnAmt, 국내수수료 (S9(15))
	char DomFcurrCmsnAmt[17];		// 40. DomFcurrCmsnAmt, 국내외화수수료 (S9(11),9(4))
	char AbrdFcurrCmsnAmt[17];		// 41. AbrdFcurrCmsnAmt, 국외외화수수료 (S9(11),9(4))
	char AbrdEvrprcFcurrAmt[17];		// 42. AbrdEvrprcFcurrAmt, 국외제비용외화금액 (S9(11),9(4))
	char MnyinCheckAmt[16];			// 43. MnyinCheckAmt, 입금수표금액 (S9(15))
	char BuyDt[8];				// 44. BuyDt, 매수일 (X(8))
	char UserId[16];			// 45. UserId, 사용자ID (X(16))
	char RcvblClrAmt[16];			// 46. RcvblClrAmt, 미수발생금액 (S9(15))
	char FwexchCvntDt[8];			// 47. FwexchCvntDt, 선물환계약일 (X(8))
	char FwexchCvntNo[9];			// 48. FwexchCvntNo, 선물환계약번호 (S9(9))
	char RvsnAmt[16];			// 49. RvsnAmt, 보정금액 (S9(15))
	char Ictax[16];				// 50. Ictax, 소득세 (S9(15))
	char FcurrIctax[17];			// 51. FcurrIctax, 외화소득세 (S9(11),9(4))
	char Ihtax[16];				// 52. Ihtax, 주민세 (S9(15))
	char FcurrIhtax[17];			// 53. FcurrIhtax, 외화주민세 (S9(11),9(4))
	char Fstax[16];				// 54. Fstax, 농특세 (S9(15))
	char Trtax[16];				// 55. Trtax, 거래세 (S9(15))
	char Uprc[15];				// 56. Uprc, 단가 (S9(9),9(4))
	char ClntNo[20];			// 57. ClntNo, 고객번호 (X(20))
	char BrkCmsnAmt[17];			// 58. BrkCmsnAmt, BROKER수수료 (S9(11),9(4))
	char KsdCmsnAmt[17];			// 59. KsdCmsnAmt, KSD수수료 (S9(11),9(4))
	char SecCmsnAmt[17];			// 60. SecCmsnAmt, SEC수수료 (S9(11),9(4))
	char FcurrEtcCmsnAmt[17];		// 61. FcurrEtcCmsnAmt, 외화기타수수료 (S9(11),9(4))
	char FcurrEtcTax[17];			// 62. FcurrEtcTax, 외화기타세금 (S9(11),9(4))
	char BloterCode[2];			// 63. BloterCode, BLOTER 코드 (X(2))
	char MktNm[40];				// 64. MktNm, 시장명 (X(40))
	char FcurrAdjustAmt[17];		// 65. FcurrAdjustAmt, 외화정산금액 (S9(11),9(4))
	char SmryCode[4];			// 66. SmryCode, 적요코드 (X(4))
	char IttCode[9];			// 67. IttCode, 기관코드 (S9(8))
	char CntryCode[3];			// 68. CntryCode, 국가코드 (X(3))
	char TrxTime[9];			// 69. TrxTime, 처리시각 (X(9))
	char FcurrStrmpTax[17];			// 70. FcurrStrmpTax, 외화인지세 (S9(11),9(4))
	char FcurrAmt1[17];			// 71. FcurrAmt1, 외화금액1 (S9(11),9(4))
	char FcurrFwdthdAbleAmt[22];		// 72. FcurrFwdthdAbleAmt, 인출가능금액 (S9(14),9(6))
	char TpNm1[40];				// 73. TpNm1, 구분명1 (X(40))
};
#define L_cosaq208_rec sizeof(struct cosaq208_rec)


struct cosaq208_mod
{
	char count[5];				// 11. Out1, 출력건수 (X(5))
	char AcntNm[40];			// 12. AcntNm, 계좌명 (X(40))		
	char out[5];
	struct cosaq208_rec rec[1];
};
#define L_cosaq208_mod sizeof(struct cosaq208_mod)



/* SDPDT080 환전 */    //점검
struct sdpdt080_mid
{
	char In[5];			// 1. In, 입력건수 (X(5))
	char MxchgTpCode[1];		// 2. MxchgTpCode, 환전구분코드 (X(1))
	char TrxTp[1];			// 3. TrxTp, 처리구분 (X(1))
	char AcntNo[20];		// 4. AcntNo, 계좌번호 (X(20))
	char Pwd[8];			// 5. Pwd, 비밀번호 (X(8))
	char RegTpCode[2];		// 6. RegTpCode, 등록구분코드 (X(2))
	char XchratTpCode[2];		// 7. XchratTpCode, 환율구분코드 (X(2))
	char MxchgPtnCode[2];		// 8. MxchgPtnCode, 환전유형코드 (X(2))
	char TgrtMnyoutAmt[26];		// 9. TgrtMnyoutAmt, 대상출금금액 (S9(18),9(6))
	char MxchgMnyinAmt[26];		// 10. MxchgMnyinAmt, 환전입금금액 (S9(18),9(6))
	char TgrtCrcyCode[3];		// 11. TgrtCrcyCode, 대상통화코드 (X(3))
	char MxchgCrcyCode[3];		// 12. MxchgCrcyCode, 환전통화코드 (X(3))
	char AppXcharat[15];		// 13. AppXcharat, 적용환율 (S9(9),9(4))
	char PrvlgRat[15];		// 14. PrvlgRat, 우대율 (S9(9),9(4))
	char Tp2[2];			// 15. Tp2, 구분2 (X(2))
	char RsvMxchgSeqno[10];		// 16. RsvMxchgSeqno, 예약환전일련번호 (X(10))
	char BaseXchrat[11];		// 17. BaseXchrat, 기준환율 (S9(5),9(4))
	char PrvlgRatAppTp[1];		// 18. PrvlgRatAppTp, 우대율적용구분 (X(1))
	char MxchgAmt[18];		// 19. MxchgAmt, 환전금액 (S9(14),99)
	char WonAmt[16];		// 20. WonAmt, 원화금액 (S9(15))
	char FcurrAmt[17];		// 21. FcurrAmt, 외화금액 (S9(11),9(4))
	char RespnseCode1[3];		// 22. RespnseCode1, 응답코드 (X(3))
	char FrcustXchrat[15];		// 23. FrcustXchrat, 대고객환율 (S9(9),9(4))
	char TqSeqno[10];		// 24. TqSeqno, 전문일련번호 (S9(9))
	char TrxSeqno[10];		// 25. TrxSeqno, 처리일련번호 (S9(9))
	char TrxBrnNo[3];		// 26. TrxBrnNo, 처리지점번호 (X(3))
	char TrdDt[8];			// 27. TrdDt, 거래일 (X(8))
};
#define L_sdpdt080_mid sizeof(struct sdpdt080_mid)

struct sdpdt080_rec
{
	char TrdNo[10];			// 29. TrdNo, 거래번호 (S9(9))
	char WwonCbalAmt[16];		// 30. WwonCbalAmt, 원화잔고 (S9(15))
	char WonFbalAmt[16];		// 31. WonFbalAmt, 원화환전 (S9(15))
	char FcurrFbalAmt[17];		// 32. FcurrFbalAmt, 외화환전 (S9(11),9(4))
	char FcurrCbalAmt[17];		// 33. FcurrCbalAmt, 외화잔고 (S9(11),9(4))
	char WonAmt[16];		// 34. WonAmt, 원화금액 (S9(15))
	char FcurrAmt[17];		// 35. FcurrAmt, 외화금액 (S9(11),9(4))
	char AppXcharat[15];		// 36. AppXcharat, 적용환율 (S9(9),9(4))
};
#define L_sdpdt080_rec sizeof(struct sdpdt080_rec)

struct sdpdt080_mod
{
	char Out[5];			// 28. Out, 출력건수 (X(5))
	struct sdpdt080_rec rec[1];
};
#define L_sdpdt080_mod sizeof(struct sdpdt080_mod)



/* SDPDQ099 해외주식 환전 적용환율 조회 */
struct sdpdq099_mid
{
	char In[5];			// 1. In, 입력건수 (X(5))
	char AcntNo[20];		// 2. AcntNo, 계좌번호 (X(20))
	char Pwd[8];			// 3. Pwd, 비밀번호 (X(8))
	char GdXchrat[17];		// 4. GdXchrat, 환율 (S9(10),9(5))
	char MxchgPtnCode[2];		// 5. MxchgPtnCode, 환전유형코드 (X(2))
};
#define L_sdpdq099_mid sizeof(struct sdpdq099_mid)


struct sdpdq099_rec
{
	char AppXchrat[15];		// 7. AppXchrat, 적용환율 (S9(9),9(4))
	char PrvlgRat[15];		// 8. PrvlgRat, 우대율 (S9(9),9(4))
};

struct sdpdq099_mod
{
	char count[5];			// 9. count, 출력건수 (X(5))
	struct sdpdq099_rec rec[1];
};
#define L_sdpdq099_mod sizeof(struct sdpdq099_mod)


/* TR2002 주식 거래내역 조회 */
struct tr2002_mid
{
	char In[5];			// 1. In, 입력건수 (X(5))
	char AcntNo[20];		// 2. AcntNo, 계좌번호 (X(20))
	char Pwd[8];			// 3. Pwd, 비밀번호 (X(8))
	char QrySrtDt[8];		// 4. QrySrtDt, 조회시작일 (X(8))
	char QryEndDt[8];		// 5. QryEndDt, 조회종료일 (X(8))
	char QryTp[1];			// 6. QryTp, 조회구분 (X(1))
					// 0 : 전체, 1 : 입출금, 2 : 입출고, 3 : 매매,  E: 매수, F : 매도, 4 : 기타, 5 : 담보대출
	char IsuNo[12];			// 7. IsuNo, 종목번호 (X(12))
	char xx[1];
};
#define L_tr2002_mid sizeof(struct tr2002_mid)


struct tr2002_out
{
	char Out1[5];			// 8. Out1, 출력건수 (X(5))
	char SellQty[16];		// 9. SellQty, 매도수량 (S9(15))
	char SellAmt[16];		// 10. SellAmt, 매도금액 (S9(15))
	char SellCmsnAmt[16];		// 11. SellCmsnAmt, 매도수수료 (S9(15))
	char SellEvrTax[16];		// 12. SellEvrTax, 매도제세금 (S9(15))
	char SellAdjstAmt[16];		// 13. SellAdjstAmt, 매도정산금액 (S9(15))
	char BuyQty[16];		// 14. BuyQty, 매수수량 (S9(15))
	char BuyAmt[16];		// 15. BuyAmt, 매수금액 (S9(15))
	char BuyCmsnAmt[16];		// 16. BuyCmsnAmt, 매수수수료 (S9(15))
	char BuyEvrTax[16];		// 17. BuyEvrTax, 매수제세금 (S9(15))
	char BuyAdjstAmt[16];		// 18. BuyAdjstAmt, 매수정산금액 (S9(15))
	char SumQty[16];		// 19. SumQty, 합계수량 (S9(15))
	char SumAmt[16];		// 20. SumAmt, 합계금액 (S9(15))
	char CmsnAmtSum[16];		// 21. CmsnAmtSum, 수수료합계 (S9(15))
	char EvrTax[16];		// 22. EvrTax, 제세금 (S9(15))
	char AdjstAmt[16];		// 23. AdjstAmt, 정산금액 (S9(15))
	char MnyinTotamt[17];		// 24. MnyinTotamt, 입금총액 (S9(16))
	char MnyoutTotamt[16];		// 25. MnyoutTotamt, 출금총액 (S9(15))
	char NetInAmt[16];		// 26. NetInAmt, 순입금액 (S9(15))
	char PrtctCnts[100];		// 27. PrtctCnts, 보호내용 (X(100))
	char RiskCnts[100];		// 28. RiskCnts, 위험내용 (X(100))
	char FdastCnts[120];		// 29. FdastCnts, 기초자산내용 (X(120))
};
#define L_tr2002_out sizeof(struct tr2002_out)


struct tr2002_rec
{
	char TrdDate[8];		// 31. TrdDate, 거래일자 (X(8))
	char SmryNm[40];		// 32. SmryNm, 적요명 (X(40))
	char SmryNo[4];			// 33. SmryNo, 적요번호 (X(4))
	char TrdQty[16];		// 34. TrdQty, 거래수량 (S9(15))
	char TrdAmt[16];		// 35. TrdAmt, 거래금액 (S9(15))
	char CmsnAmt[16];		// 36. CmsnAmt, 수수료 (S9(15))
	char OvdSum[16];		// 37. OvdSum, 연체합 (S9(15))
	char DpspdgLoanAmt[16];		// 38. DpspdgLoanAmt, 예탁담보대출금액 (S9(15))
	char SecCrbalQty[16];		// 39. SecCrbalQty, 유가증권잔수량 (S9(15))
	char TrdNo[10];			// 40. TrdNo, 거래번호 (S9(9))
	char IsuNm[40];			// 41. IsuNm, 종목명 (X(40))
	char TrdUprc[13];		// 42. TrdUprc, 거래단가 (S9(9),9(2))
	char AdjstAmt[16];		// 43. AdjstAmt, 정산금액 (S9(15))
	char TaxSumAmt[16];		// 44. TaxSumAmt, 세금합계금액 (S9(15))
	char IntrstUlffee[16];		// 45. IntrstUlffee, 이자이용료 (S9(15))
	char SellPldgRfundAmt[16];	// 46. SellPldgRfundAmt, 매도담보상환금 (S9(15))
	char DpsCrbalAmt[16];		// 47. DpsCrbalAmt, 예수금잔금액 (S9(15))
	char EtcBndInfo[100];		// 48. EtcBndInfo, 기타채권정보 (X(100))
	char CrdtGrdVal[40];		// 49. CrdtGrdVal, 신용등급값 (X(40))
	char TrxTime[9];		// 50. TrxTime, 처리시각 (X(9))
};
#define L_tr2002_rec sizeof(struct tr2002_rec)

struct tr2002_mod
{
	struct tr2002_out out1;
	char Out2[5]; // 30. Out2, 출력건수 (X(5))
	struct tr2002_rec rec[1];
};
#define L_tr2002_mod sizeof(struct tr2002_mod)


struct pibogorder_mid // 해외주식 주문 
{
	char gubn[2];			//1 구분
	char icont[4];			//2 주문개수	
	char odgb[1];			//3 주문유형 
	char mmgb[2];			//4 매매구분
	char opcd[2];			//5 주문유형코드
	char ogno[10];			//6 원주문번호
	char accn[20];			//7 계좌번호
	char pswd[8];			//8 계좌비번
	char mkgb[2];			//9 주문시장코드
	char code[12];			//10 종목코드
	char jqty[16];			//11 주문수량
	char jprc[15];			//12 주문가격
	char hogb[2];			//13 호가구분
	char jmgb[2];			//14 통신매체
	char mdgb[2];			//15 중개인구분코드
	char rsvgubn[1];		//16 예약구분
	char curCode[3];		//17 국가코드
	char rsvlpdt[8];		//18 예약일자
	char rsvStDt[8];		//19 예약시작일자
	char rsvEndDt[8];		//20 예약종료일자
	char rsvOrdNo[2];		//21 예약주문조건코드
	char rsv_lndt[8];		//22 대출일
	char creditCode[3];		//23 신용거래코드
	char filler[8];			//24 filler
};
#define L_pibogorder_mid sizeof(struct pibogorder_mid)

struct pibogorder_rec
{
	char jmno[10];			//2 주문번호
	char accn[40];			//3 계좌번호
	char codename[40];		//4 종목명
	char result[80];		//5 결과코드
};
#define L_pibogorder_rec sizeof(struct pibogorder_rec)

struct pibogorderr_mod
{
	char cnt[4];			//1 주문개수
	pibogorder_rec rec[1];
};
#define L_pibogorder_mod sizeof(struct pibogorderr_mod)


struct sdpkt183_mid		//TR5007
{
	char In[5];			// 1. In, 입력건수 (X(5))
	char TrxTp[1];			// 2. TrxTp, 처리구분 (X(1))
	char CrcyCode[3];		// 3. CrcyCode, 통화코드 (X(3))
	char AcntNo[20];		// 4. AcntNo, 계좌번호 (X(20))
};

struct skpkt183_rec
{
	char SysTime[17];		// 6. SysTime, 시스템시각 (X(17))
	char BfXchrat[11];		// 7. BfXchrat, 환율 (S9(5),9(4))
	char Xchrat[11];		// 8. Xchrat, 환율 (S9(5),9(4))
	char RlyBnkCode[3];		// 12. RlyBnkCode, 중계은행코드 (X(3))
	char RlyBnkNm[40];		// 13. RlyBnkNm, 중계은행명 (X(40)) 
	char BnkRpsnCode[4];		// 14. BnkRpsnCode, 은행응답코드 (X(4))
	char BnkMsgCnts[100];		// 15. BnkMsgCnts, 은행메시지내용 (X(100))
	char IfMgmtNo[10];		// 16. ifMgmtNo, IF관리번호 (X(10))
};

struct skpkt183_mod
{
	char Out[5];			// 17. Out, 출력건수 (X(5))
	skpkt183_rec rec[1];
};

/* SDPKT169 거래환율조회(자동환전) */
struct sdpkt169_mid	//TR5004
{
	char In[5];			// 1. In, 입력건수 (X(5))
	char TrxTp[1];			// 2. TrxTp, 처리구분 (X(1))
	char CrcyCode[3];		// 3. CrcyCode, 통화코드 (X(3))
	char AcntNo[20];		// 4. AcntNo, 계좌번호 (X(20))
};
#define L_sdpkt169_mid sizeof(struct sdpkt169_mid)


struct sdpkt169_rec
{
	char SysTime[17];		// 6. SysTime, 시스템시각 (X(17))
	char BfXchrat[11];		// 7. BfXchrat, 환율 (S9(5),9(4))
	char Xchrat[11];		// 8. Xchrat, 환율 (S9(5),9(4))
	char BnkRpsnCode[4];		// 9. BnkRpsnCode, 은행응답코드 (X(4))
	char BnkMsgCnts[100];		// 10. BnkMsgCnts, 은행메시지내용 (X(100))
};
#define L_sdpkt169_mod sizeof(struct sdpkt169_mod)

struct sdpkt169_mod
{
	char Out[5];			// 5. Out, 출력건수 (X(5))
	sdpkt169_rec rec[1];
};


struct sdpwq002_mid	//TR5010
{
	char In[5]; // 1. In, 입력건수 (X(5))
	char filler[1];
};


struct sdpwq002_rec
{
	char xxx[6]; // 1. xxx, 설명 (X(6))
	char bnkcode[3]; // 2. bnkcode, 은행코드 (X(3))
	char bnkname[40]; // 3. bnkname, 은행명 (X(40))
};

struct sdpwq002_mod
{
	char out5[5];
	sdpwq002_rec rec[1];
};

#pragma	pack()
