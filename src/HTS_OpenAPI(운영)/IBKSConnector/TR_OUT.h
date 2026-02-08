#pragma once

#pragma	pack(1)

// Transaction Key
#define TK_TR1001	(1)
#define TK_TR1002	(2)
#define TK_TR1003	(3)
#define TK_TR1201	(4)
#define TK_TR1202	(5)
#define TK_TR1211	(6)
#define TK_TR1221	(7)
#define TK_TR1231	(8)
#define TK_TR1801	(9)
#define TK_TR1802	(10)
#define TK_TR1803	(11)
#define TK_TR1804	(12)
#define TK_TR3001	(13)
#define TK_TR3002	(14)
#define TK_TR3003	(15)
#define TK_TR3201	(16)
#define TK_TR3202	(17)
#define TK_TR3211	(18)
#define TK_TR3221	(19)
#define TK_TR3231	(20)
#define TK_TR4003	(21)
#define TK_TR4004	(22)
#define TK_TR4101	(23)
#define TK_TR4201	(24)
#define TK_TR4202	(25)
#define TK_TR4221	(26)
#define TK_TR4223	(27)
#define TK_TR4224	(28)
#define TK_TR4303	(29)
#define TK_TR4401	(30)
#define TK_GETCODE	(31)

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
    char    acno[11];
    char    pswd[8];
    char    date[8];
    char    cod2[12];
    char    juno[5];    /* 시작주문번호             */
    char    dsgb[1];    /* 0.All 1.매도 2.매수      */
    char    otim[1];    /* 0.All 1.장중 2.시간외    */
    char    sygb[1];    /* 0.All 1.신용 2.담보      */
    char    dllf[1];    /* 0.MAP 1.DLL              */
    char    dlgb[1];    /* 0.All 1.체결 2.미체결    */
    char    sort[1];    /* 1.역순 2.정순            */
};
#define L_tr1211_mid		sizeof(struct tr1211_mid)
struct tr1211_grid
{
    char    juno[5];    /* 주문   번호      */
    char    ojno[5];    /* 원주문 번호      */
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
#define L_tr1221_grid		sizeof(struct tr1221_grid)


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
* NAME : tr1801
* DESC : 복수종목 시세조회
 ******************************************************************************/
struct tr1801_grid
{
    char code[6];	/* RTS Symbol  */
    char hnam[20];  /* 한글명      */
    char curr[7];   /* 현재가      */
    char giho[1];   /* 등락부호    */
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
    char giho[1];	/* 등락부호    */
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
    char giho[1];		/* 등락부호   */
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
	char	chdr[4][6];		/* CALL OPTION 종목 HEADER		*/
	char	phdr[4][6];		/* PUT OPTION 종목 HEADER		*/
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
	} call[4]; 
	struct	{
		char	yorn[1];	/* 종목 코드 유무				*/
		char	codx[9];	/* 단축종목코드					*/
		char	hnam[20];	/* 한글종목명					*/
		char	enam[20];	/* 영문명						*/
		char	mchk;		/* 매매유형허용					*/
							/* 0x01:시장가허용				*/
							/* 0x02:조건부지정가허용		*/
							/* 0x04:최유리지정가허용		*/
	} put[4]; 
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

//////////////////////////////////////////////////////////////////////////////
//PTS 추가
//////////////////////////////////////////////////////////////////////////////

/******************************************************************************
* NAME : TR4224
* DESC : 선물옵션 - 선물옵션 계좌익일예상증거금조회2
 ******************************************************************************/
struct TR4224_mid
{
	char In[5];							/* 레코드갯수             */
	char zAcntNo[20];					/* 계좌번호               */
	char zInptPwd[8];					/* 입력비밀번호           */
	char zKpi200Idx[13];				/* 코스피200지수          */
};
#define L_TR4224_mid	sizeof(struct TR4224_mid)

struct TR4224_mod
{
	struct TR4224_mid mid;
	char Out[5];						/* 레코드갯수				*/
	char zAcntNm[40];					/* 계좌명					*/
	char zFnoAcntAfmgnNm[16];			/* 선물옵션계좌사후증거금명	*/
	char zDpsTotamt[16];				/* 예수금총액				*/
	char zDps[16];						/* 예수금					*/
	char zSubstAmt[16];					/* 대용금액					*/
	char zCsgnMgn[16];					/* 위탁증거금액				*/
	char zMnyCsgnMgn[16];				/* 현금위탁증거금액			*/
	char zAddMgn[16];					/* 추가증거금액				*/
	char zMnyAddMgn[16];				/* 현금추가증거금액			*/
	char zPsnOutAbleAmt[16];			/* 인출가능금액				*/
	char zPsnOutAbleCurAmt[16];			/* 인출가능현금액			*/
	char zOrdAbleTotAmt[16];			/* 주문가능총금액			*/
	char zOrdAbleAmt[16];				/* 주문가능금액				*/
	char zOut2[5];						/* 레코드갯수				*/
	char zDpsTotamt2[16];				/* 예수금총액				*/
	char zDps2[16];						/* 예수금					*/
	char zSubstAmt2[16];					/* 대용금액					*/
	char zCsgnMgn2[16];					/* 위탁증거금액				*/
	char zMnyCsgnMgn2[16];				/* 현금위탁증거금액			*/
	char zAddMgn2[16];					/* 추가증거금액				*/
	char zMnyAddMgn2[16];				/* 현금추가증거금액			*/
	char zPsnOutAbleAmt2[16];			/* 인출가능금액				*/
	char zPsnOutAbleCurAmt2[16];			/* 인출가능현금액			*/
	char zOrdAbleTotAmt2[16];			/* 주문가능총금액			*/
	char zOrdAbleAmt2[16];				/* 주문가능금액				*/
	char zOut3[5];						/* 레코드갯수				*/
	char zDpstgTotamtLackAmt[16];		/* 예탁총액부족금액(위탁증거금기준)				*/
	char zDpstgMnyLackAmt[16];			/* 예탁현금부족금액(위탁증거금기준)				*/
	char zDpstgTotamtLackAmt1[16];		/* 예탁총액부족금액(유지증거금기준)				*/
	char zDpstgMnyLackAmt1[16];			/* 예탁현금부족금액(유지증거금기준)				*/
	char zFutsThdayDfamt[16];			/* 선물당일차금				*/
	char zFutsUdamt[16];				/* 선물갱신차금				*/
	char zFutsLastSettDfamt[16];		/* 선물최종결제차금			*/
	char zAdjstAmt[16];					/* 정산금액					*/
	char zOptBuyAmt[16];				/* 옵션매수금액				*/
	char zOptSellAmt[16];				/* 옵션매도금액				*/
	char zOptRgtexeDfamt[16];			/* 옵션권리행사차금			*/
	char zOptRgtAsgnDfamt[16];			/* 옵션권리배정차금			*/
	char zFutsCmsnAmt[16];				/* 선물수수료				*/
	char zOptCmsnAmt[16];				/* 옵션수수료				*/
	char zFnoPrdaySubstSellAmt[16];		/* 선물옵션전일대용매도금액				*/
	char zFnoCrdaySubstSellAmt[16];		/* 선물옵션금일대용매도금액				*/
	char zOptEvalPnlAmt[16];			/* 옵션평가손익금액			*/
	char zOut4[16];						/* 레코드갯수				*/
	char zFnoIsuNo[32];					/* 선물옵션종목번호			*/
	char zBnsTp[1];					/* 매매구분					*/
	char zBnsTpNm[10];					/* 매매구분					*/
	char zUnsttQty[16];					/* 미결제수량				*/
	char zCurPrc[13];					/* 현재가					*/
	char zIvlatRat[7];					/* 내재변동성비율			*/
	char zFcastThrprc[19];				/* 예상이론가				*/
	char zRemnDays[6];					/* 잔존일수					*/
};
#define L_TR4224_mod		sizeof(struct TR4224_mod)

/******************************************************************************
* NAME : TR4223
* DESC : 선물옵션 - 선물옵션 추가증거금발생현황
 ******************************************************************************/
struct TR4223_mid
{
	char In[5];							/* 레코드갯수           */
	char zAcntNo[20];					/* 계좌번호				*/
	char zPwd[8];						/* 비밀번호				*/
	char zSrtDt[8];						/* 시작일				*/
	char zEndDt[8];						/* 종료일				*/
};
#define L_TR4223_mid	sizeof(struct TR4223_mid)

struct TR4223_mod
{
	struct TR4223_mid mid;
	char Out[5];						/* 레코드갯수				*/
	char zOcrDt[8];						/* 발생일					*/
	char zEvalDpsamtTotamt[15];			/* 평가예탁금총액			*/
	char zOpnmkAddMgn[16];				/* 개장추가증거금액			*/
	char zOpnmkMnyAddMgn[16];			/* 개장현금추가증거금액		*/
	char zOpnmkFcurrAddMgn[16];			/* 개장외화추가증거금		*/
	char zOpnmkAddMgnTpNm[12];			/* 추가증거금상태			*/
	char zAddMgnTpNm[12];				/* 추가증거금발생			*/
	char zIsuNo[12];					/* 종목번호					*/
	char zBnsTp[1];						/* 매매구분					*/
	char zBnsTpNm[10];					/* 매매구분					*/
	char zCvrgQty[16];					/* 반대매매수량				*/
	char zOrdPrc[13];					/* 주문가					*/
	char zCvrgAmt[16];					/* 반대매매금액				*/
	char zLnkAcntNo[20];				/* 연계계좌번호				*/
	char zAddMgnTrxTp[1];				/* 추가증거금처리구분		*/
	char zTrxStatNm[20];				/* 처리상태명				*/
};
#define L_TR4223_mod		sizeof(struct TR4223_mod)

/******************************************************************************
* NAME : TR4303
* DESC : 선물옵션 - 선물옵션 일별 계좌손익내역
 ******************************************************************************/
struct TR4303_mid
{
	char In[5];							/* 레코드갯수           */
	char zAcntNo[20];					/* 계좌번호				*/
	char zPwd[8];						/* 비밀번호				*/
	char zQrySrtDt[8];					/* 조회시작일			*/
	char zQryEndDt[8];					/* 조회종료일			*/
	char zQryTp[1];						/* 조회구분			1.일자별, 2.월별, 3.주간별	*/
	char zStnlnSeqTp[1];				/* 정렬순서구분		1.순, 2.역순*/
	char zBalEvalTp[1];					/* 잔고평가구분		0.기본, 1.이동평균법, 2.선입선출법	*/
};
#define L_TR4303_mid	sizeof(struct TR4303_mid)

struct TR4303_grid
{
	char zQryDt[8];			/*조회일*/
	char zDpstgTotamt[16];	/*예탁총액*/
	char zDpstgMny[16];		/*예탁현금*/
	char zFnoMgn[16];		/*선물옵션증거금액*/
	char zFutsPnlAmt[16];	/*선물손익금액*/
	char zOptBsnPnlAmt[16];	/*옵션매매손익금액*/
	char zOptEvalPnlAmt[16];/*옵션평가손익금액*/
	char zCmsnAmt[16];		/*수수료*/
	char zSumAmt1[16];		/*합계금액1*/
	char zSumAmt2[16];		/*합계금액*/
	char zPnlSumAmt[16];	/*손익합계금액*/
	char zFutsBuyAmt[16];	/*선물매수금액*/
	char zFutsSellAmt[16];	/*선물매도금액*/
	char zOptBuyAmt[16];	/*옵션매수금액*/
	char zOptSellAmt[16];	/*옵션매도금액*/
	char zInAmt[16];		/*입금액*/
	char zOutAmt[16];		/*출금액*/
	char zEvalAmt[16];		/*평가금액*/
	char zAddupEvalAmt[16];	/*합산평가금액*/
	char zAmt2[16];			/*금액2*/
};
#define L_TR4303_grid		sizeof(struct TR4303_grid)

struct TR4303_mod
{
	struct TR4303_mid mid;
	char Out[5];						/* 레코드갯수				*/
	char zFutsAdjstDfamt[16];			/*선물정산차금*/
	char zOptBnsplAmt[16];				/*옵션매매손익금액*/
	char zFnoCmsnAmt[16];				/*선물옵션수수료*/
	char zPnlSumAmt[16];				/*손익합계금액*/
	char zMnyinAsmAmt[16];				/*입금누계금액*/
	char zMnyoutAsmAmt[16];				/*출금누계금액*/
	char Out2[5];                       /* 레코드갯수             */
    struct TR4303_grid grid[1];
};
#define L_TR4303_mod		sizeof(struct TR4303_mod)

/******************************************************************************
* NAME : TR4201
* DESC : 선물옵션 - 선물옵션 지수잔고조회
 ******************************************************************************/
struct TR4201_mid
{
	char In[5];							/* 레코드갯수           */
	char zAcntNo[20];					/* 계좌번호				*/
	char zPwd[8];						/* 비밀번호				*/
	char zFnoClssCode[2];				/* 선물옵션분류코드		00.전체, 11.선물, 12.옵션*/
	char zPdGrpCode[2];					/* 상품군코드			default 00 */
};
#define L_TR4201_mid	sizeof(struct TR4201_mid)

struct TR4201_grid
{
	char zFnoIsuNo[32];		/* 선물옵션종목번호 */
	char zBnsTp[1];			/* 매매구분 */
	char zBnsTpNm[10];		/* 매매구분 */
	char zBgnDt[8];			/* 당초일 */
	char zUnsttQty[16];		/* 미결제수량 */
	char zFnoAvrPrc[19];	/* 평균가 */
	char zCurprc[13];		/* 현재가 */
	char zUnsttAmt[16];		/* 미결제금액 */
	char zEvalPnlAmt[16];	/* 평가손익금액 */
};
#define L_TR4201_grid		sizeof(struct TR4201_grid)

struct TR4201_mod
{
	struct TR4201_mid mid;
	char Out[5];				/* 레코드갯수 */
	char zAcntNm[40];			/* 계좌명 */
	char zDpstgTotamt[16];		/* 예탁총액 */
	char zDps[16];				/* 예수금 */
	char zMgn[16];				/* 증거금액 */
	char zMnyMgn[16];			/* 현금증거금액 */
	char zOrdAbleAmt[16];		/* 주문가능금액 */
	char zMnyOrdAbleAmt[16];	/* 현금주문가능금액 */
	char zPsnOutAbleAmt[16];	/* 인출가능금액 */
	char zPsnOutAbleCurAmt[16];	/* 인출가능현금액 */
	char zFutsPnlAmt[16];		/* 선물손익금액 */
	char zOptBnsplAmt[16];		/* 옵션매매손익금액 */
	char zPnlSumAmt[16];		/* 손익합계금액 */
	char Out2[5];               /* 레코드갯수 */
    struct TR4201_grid grid[1];
};
#define L_TR4201_mod		sizeof(struct TR4201_mod)

/******************************************************************************
* NAME : TR4101
* DESC : 계좌 비밀번호 검증
 ******************************************************************************/
struct TR4101_mid
{
	char In[5];							/* 레코드갯수           */
	char zAcntNo[20];					/* 계좌번호				*/
	char zPwd[8];						/* 비밀번호				*/
};
#define L_TR4101_mid	sizeof(struct TR4101_mid)

struct TR4101_mod
{
	struct TR4101_mid mid;
	char Out[5];				/* 레코드갯수 */
	char zAvalYn[1];			/* 유효여부 */
};
#define L_TR4101_mod		sizeof(struct TR4101_mod)

/******************************************************************************
* NAME : TR4221
* DESC : 선물옵션 계좌예탁금증거금조회
 ******************************************************************************/
struct TR4221_mid
{
	char In[5];							/* 레코드갯수           */
	char zAcntNo[20];					/* 계좌번호				*/
	char zPwd[8];						/* 비밀번호				*/
};
#define L_TR4221_mid	sizeof(struct TR4221_mid)

struct TR4221_grid
{
	char zPdGrpCodeNm[20];				/* 상품군코드명*/
	char zNetRiskMgn[16];				/* 순위험증거금액*/
	char zPrcFlctMgn[16];				/* 가격변동증거금액*/
	char zSprdMgn[16];					/* 스프레드증거금액*/
	char zMgnRdctAmt[16];				/* 증거금감면액총액*/
	char zMinMgn[16];					/* 최소증거금액*/
	char zPrcMgn[16];					/* 가격증거금액*/
	char zOrdMgn[16];					/* 주문증거금액*/
	char zOptNetBuyAmt[16];				/* 옵션순매수금액*/
	char zCsgnMgn[16];					/* 위탁증거금액*/
	char zUndMgn[16];					/* 인수도증거금액*/
	char zMaintMgn[16];					/* 유지증거금액*/
	char zFutsBuyExecAmt[16];			/* 선물매수체결금액*/
	char zFutsSellExecAmt[16];			/* 선물매도체결금액*/
	char zOptBuyExecAmt[16];			/* 옵션매수체결금액*/
	char zOptSellExecAmt[16];			/* 옵션매도체결금액*/
	char zFutsPnlAmt[16];				/* 선물손익금액*/
	char zTotRiskCsgnMgn[16];			/* 총위험위탁증거금*/
};
#define L_TR4221_grid		sizeof(struct TR4221_grid)

struct TR4221_mod
{
	struct TR4221_mid mid;
	char Out[5];						/* 레코드갯수 */
	char zAcntNm[40];					/* 계좌명	*/
	char zDpsTotamt[16];				/* 예수금총액	*/
	char zDps[16];						/* 예수금		*/
	char zSubstAmt[16];					/* 대용금액		*/
	char zFilupDpsamtTotamt[16];		/* 충당예탁금총액*/
	char zFilupDps[16];					/* 충당예수금*/
	char zFutsPnlAmt[16];				/* 선물손익금액*/
	char zPsnOutAbleAmt[16];			/* 인출가능금액*/
	char zPsnOutAbleCurAmt[16];			/* 인출가능현금액*/
	char zPsnOutAbleSubstAmt[16];		/* 인출가능대용금액*/
	char zMgn[16];						/* 증거금액*/
	char zMnyMgn[16];					/* 현금증거금액*/
	char zOrdAbleAmt[16];				/* 주문가능금액*/
	char zMnyOrdAbleAmt[16];			/* 현금주문가능금액*/
	char zAddMgn[16];					/* 추가증거금액*/
	char zMnyAddMgn[16];				/* 현금추가증거금액*/
	char zAmtPrdayChckInAmt[16];		/* 금전일수표입금액*/
	char zFnoPrdaySubstSellAmt[16];		/* 선물옵션전일대용매도금액*/
	char zFnoCrdaySubstSellAmt[16];		/* 선물옵션금일대용매도금액*/
	char zFnoPrdayFdamt[16];			/* 선물옵션전일가입금액*/
	char zFnoCrdayFdamt[16];			/* 선물옵션금일가입금액*/
	char zFcurrSubstAmt[16];			/* 외화대용금액*/
	char zFnoAcntAfmgnNm[20];			/* 선물옵션계좌사후증거금명*/
	char Out2[5];						/* 레코드갯수 */
    struct TR4221_grid grid[1];
};
#define L_TR4221_mod		sizeof(struct TR4221_mod)

/******************************************************************************
* NAME : TR4401
* DESC : 선물옵션 계좌 거래내역
 ******************************************************************************/
struct TR4401_mid
{
	char In[5];							/* 레코드갯수           */
	char zQryTp[1];						/* 조회구분 0.전체, 1.입출금, 2.입출고, 3.매매 */
	char zAcntNo[20];					/* 계좌번호*/
	char zPwd[8];						/* 비밀번호*/
	char zQrySrtDt[8];					/* 조회시작일*/
	char zQryEndDt[8];					/* 조회종료일*/
	char zSrtNo[10];					/* 시작번호 Default +000000000 */
	char zPdptnCode[2];					/* 상품유형코드 Default 00 */
	char zIsuNo[12];					/* 종목번호*/
};
#define L_TR4401_mid	sizeof(struct TR4401_mid)

struct TR4401_grid
{
	char zTrdDate[8];					/* 거래일자*/
	char zTrdNo[10];					/* 거래번호*/
	char zTrdTpNm[20];					/* 거래구분*/
	char zSmryNo[4];					/* 적요번호*/
	char zSmryNm[40];					/* 적요명*/
	char zCancTpNm[20];					/* 취소구분*/
	char zTrdQty[16];					/* 거래수량*/
	char zTrtax[16];					/* 거래세*/
	char zAdjstAmt[16];					/* 정산금액*/
	char zOvdSum[16];					/* 연체합*/
	char zDpsBfbalAmt[16];				/* 예수금전잔금액*/
	char zSellPldgRfundAmt[16];			/* 매도담보상환금*/
	char zDpspdgLoanBfbalAmt[16];		/* 예탁담보대출전잔금액*/
	char zTrdmdaNm[40];					/* 거래매체명*/
	char zOrgTrdNo[10];					/* 원거래번호*/
	char zIsuNm[40];					/* 종목명*/
	char zTrdUprc[13];					/* 거래단가*/
	char zCmsnAmt[16];					/* 수수료*/
	char zRfundDiffAmt[16];				/* 상환차이금액*/
	char zRepayAmtSum[16];				/* 변제금합계*/
	char zSecCrbalQty[16];				/* 유가증권금잔수량*/
	char zCslLoanRfundIntrstAmt[16];	/* 매도대금담보대출상환이자금액*/
	char zDpspdgLoanCrbalAmt[16];		/* 예탁담보대출금잔금액*/
	char zTrxTime[9];					/* 처리시각*/
	char zInouno[10];					/* 출납번호*/
	char zIsuNo[12];					/* 종목번호*/
	char zTrdAmt[16];					/* 거래금액*/
	char zTaxSumAmt[16];				/* 세금합계금액*/
	char zIntrstUtlfee[16];				/* 이자이용료*/
	char zDvdAmt[16];					/* 배당금액*/
	char zRcvblOcrAmt[16];				/* 미수발생금액*/
	char zTrxBrnNo[3];					/* 처리지점번호*/
	char zTrxBrnNm[40];					/* 처리지점명*/
	char zDpspdgLoanAmt[16];			/* 예탁담보대출금액*/
	char zDpspdgLoanRfundAmt[16];		/* 예탁담보대출상환금액*/
	char zBasePrc[13];					/* 기준가*/
	char zDpsCrbalAmt[16];				/* 예수금금잔금액*/
	char zBoa[16];						/* 과표*/
	char zMnyoutAbleAmt[16];			/* 출금가능금액*/
	char zBcrLoanOcrAmt[16];			/* 수익증권담보대출발생금*/
	char zBcrLoanBfbalAmt[16];			/* 수익증권담보대출전잔금*/
	char zBnsBasePrc[20];				/* 매매기준가*/
	char zTaxchrBasePrc[20];			/* 과세기준가    */
	char zTrdUnit[16];					/* 거래좌수      */
	char zBalUnit[16];					/* 잔고좌수      */
	char zEvrTax[16];					/* 제세금      */  
	char zEvalAmt[16];					/* 평가금액      */
	char zBcrLoanRfundAmt[16];			/* 수익증권담보대출상환금*/
	char zBcrLoanCrbalAmt[16];			/* 수익증권담보대출금잔금*/
	char zAddMgnOcrTotamt[16];			/* 추가증거금발생총액*/
	char zAddMnyMgnOcrAmt[16];			/* 추가현금증거금발생금액*/
	char zAddMgnDfryTotamt[16];			/* 추가증거금납부총액*/
	char zAddMnyMgnDfryAmt[16];			/* 추가현금증거금납부금액*/
	char zBnsplAmt[16];					/* 매매손익금액  */
	char zIctax[16];					/* 소득세        */
	char zIhtax[16];					/* 주민세      */  
	char zLoanDt[8];					/* 대출일    */    
};
#define L_TR4401_grid		sizeof(struct TR4401_grid)

struct TR4401_mod
{
	struct TR4401_mid mid;
	char Out[5];						/* 레코드갯수 */
	char zAcntNm[40];					/* 계좌명	*/
	char Out2[5];						/* 레코드갯수 */
    struct TR4401_grid grid[1];
	char Out3[5];						/* 레코드갯수 */
	char zPnlSumAmt[16];				/* 손익합계금액  */
	char zCtrctAsm[16];					/* 약정누계      */
	char zCmsnAmtSumAmt[16];			/* 수수료합계금액*/
};
#define L_TR4401_mod		sizeof(struct TR4401_mod)

/******************************************************************************
* NAME : TR4202
* DESC : 선물옵션 계좌잔고 및 평가현황2
 ******************************************************************************/
struct TR4202_mid
{
	char In[5];                        /* 레코드갯수             */
	char zAcntNo[20];                  /* 계좌번호               */
	char zInptPwd[8];                  /* 입력비밀번호           */
	char zOrdDt[8];                    /* 주문일                 */
	char zBalEvalTp[1];                /* 잔고평가구분           1.이동평균법, 2.선입선출법*/
	char zFutsPrcEvalTp[1];            /* 선물가격평가구분       1.당초가, 2.전일종가*/
	char zLqdtQtyQryTp[1];             /* 청산수량조회구분       */
};
#define L_TR4202_mid		sizeof(struct TR4202_mid)

struct TR4202_grid
{
	char zFnoIsuNo[32];                /* 선물옵션종목번호       */
	char zIsuNm[40];                   /* 종목명                 */
	char zBnsTp[1];                    /* 매매구분               */
	char zBnsTpNm[10];                 /* 매매구분               */
	char lUnsttQty[16];                /* 미결제수량             */
	char dFnoAvrPrc[19];               /* 평균가                 */
	char dNowPrc[13];                  /* 현재가                 */
	char dCmpPrc[13];                  /* 대비가                 */
	char lEvalPnl[16];                 /* 평가손익               */
	char dPnlRat[12];                  /* 손익률                 */
	char lEvalAmt[16];                 /* 평가금액               */
	char dEvalRat[7];                  /* 평가비율               */
	char lLqdtAbleQty[16];             /* 청산가능수량           */
};
#define L_TR4202_grid		sizeof(struct TR4202_grid)

struct TR4202_mod
{
	struct TR4202_mid mid;
	char Out[5];                       /* 레코드갯수             */
	char zAcntNm[40];                  /* 계좌명                 */
	char lEvalDpsamtTotamt[15];        /* 평가예탁금총액         */
	char lMnyEvalDpstgAmt[15];         /* 현금평가예탁금액       */
	char lDpsamtTotamt[16];            /* 예탁금총액             */
	char lDpstgMny[16];                /* 예탁현금               */
	char lDpstgSubst[16];              /* 예탁대용               */
	char lFcurrSubstAmt[16];           /* 외화대용금액           */
	char lPsnOutAbleTotAmt[15];        /* 인출가능총금액         */
	char lPsnOutAbleCurAmt[16];        /* 인출가능현금액         */
	char lPsnOutAbleSubstAmt[16];      /* 인출가능대용금액       */
	char lOrdAbleTotAmt[15];           /* 주문가능총금액         */
	char lMnyOrdAbleAmt[16];           /* 현금주문가능금액       */
	char lCsgnMgnTotamt[16];           /* 위탁증거금총액         */
	char lMnyCsgnMgn[16];              /* 현금위탁증거금액       */
	char lMtmgnTotamt[15];             /* 유지증거금총액         */
	char lMnyMaintMgn[16];             /* 현금유지증거금액       */
	char lAddMgnTotamt[15];            /* 추가증거금총액         */
	char lMnyAddMgn[16];               /* 현금추가증거금액       */
	char lCmsnAmt[16];                 /* 수수료                 */
	char lRcvblAmt[16];                /* 미수금액               */
	char lRcvblOdpnt[16];              /* 미수연체료             */
	char lFutsEvalPnlAmt[16];          /* 선물평가손익금액       */
	char lOptEvalPnlAmt[16];           /* 옵션평가손익금액       */
	char lOptEvalAmt[16];              /* 옵션평가금액           */
	char lOptBnsplAmt[16];             /* 옵션매매손익금액       */
	char lFutsAdjstDfamt[16];          /* 선물정산차금           */
	char lTotPnlAmt[16];               /* 총손익금액             */
	char lNetPnlAmt[16];               /* 순손익금액             */
	char Out2[5];                      /* 레코드갯수             */
	struct TR4202_grid grid[1];
};
#define L_TR4202_mod		sizeof(struct TR4202_mod)

/******************************************************************************
* NAME : TR4003
* DESC : 선물옵션 복수청산주문체결예상증거금
 ******************************************************************************/
struct TR4003_grid_mid
{
	char zFnoIsuNo[32];			/* 선물옵션종목번호*/
	char zBnsTp[1];				/* 매매구분        1.매도, 2.매수*/
	char zBnsTpNm[10];			/* 매매구분        1.매도, 2.매수*/
	char zUnsttQty[16];			/* 미결제수량      */
	char zLqdtQty[16];			/* 청산수량        */
	char zExecPrc[13];			/* 체결가          */
};
#define L_TR4003_grid_mid		sizeof(struct TR4003_grid_mid)

struct TR4003_mid
{
	char In[5];            /* 레코드 갯수      */
	char zAcntNo[20];      /* 계좌번호         */
	char zInptPwd[8];      /* 입력비밀번호     */
	char zMprcOrdTp[1];    /* 시장가주문구분   0.현재가, 1.시장가 */
};
#define L_TR4003_mid		sizeof(struct TR4003_mid)

struct TR4003_mid2
{
	char In2[5];           /* 레코드 갯수      */
};
#define L_TR4003_mid2		sizeof(struct TR4003_mid2)

struct TR4003_mod
{
	TR4003_mid mid;
	char Out[5];                /* 레코드갯수             */
	char zAcntNm[40];			/* 계좌명                  */
	char zCsgnMgn[16];			/* 위탁증거금액			   */
	char zMnyCsgnMgn[16];		/* 현금위탁증거금액        */
	char zExecAfCsgnMgn[16];	/* 체결후위탁증거금액      */
	char zExecAfMnyCsgnMgn[16];	/* 체결후현금위탁증거금액  */
};
#define L_TR4003_mod		sizeof(struct TR4003_mod)

/******************************************************************************
* NAME : TR4004
* DESC : 선물옵션 복수종목주문
 ******************************************************************************/
struct TR4004_grid_mid
{
	char zFnoIsuNo[32];			/* 선물옵션종목번호*/
	char zBnsTp[1];				/* 매매구분        */
	char zFnoOrdprcPtnCode[2];	/* 선물옵션호가유형코드	03.시장가, 00.현재가*/
	char zOrdQty[16];			/* 주문수량*/
	char zOrdPrc[13];			/* 주문가*/
	char zCurPrc[13];			/* 현재가*/
	char zOrdNo[10];			/* 주문번호 Default +000000000 */
	char zMsgCode[4];			/* 메시지코드 Default 빈값 */
};
#define L_TR4004_grid_mid		sizeof(struct TR4004_grid_mid)

struct TR4004_mid
{
	char In[5];				/* 레코드 갯수      */
	char zAcntNo[20];		/* 계좌번호         */
	char zInptPwd[8];		/* 입력비밀번호     */
	char zTrxTp[1];			/* 처리구분     Default 2*/
	char zFnoTrdPtnCode[2];	/* 선물옵션거래유형코드   Default 03  */
	char zCommdaCode[2];	/* 통신매체코드     Default 01*/
	char zIp[12];			/* IP주소			*/
};
#define L_TR4004_mid		sizeof(struct TR4004_mid)

struct TR4004_mid2
{
	char In2[5];           /* 레코드 갯수      */
};
#define L_TR4004_mid2		sizeof(struct TR4004_mid2)

struct TR4004_mod
{
	TR4004_mid mid;
	char Out[5];                /* 레코드갯수             */
	char zAcntNm[40];			/* 계좌명                  */
	char zCsgnMgn[16];			/* 위탁증거금액			   */
	char zMnyCsgnMgn[16];		/* 현금위탁증거금액        */
	char zExecAfCsgnMgn[16];	/* 체결후위탁증거금액      */
	char zExecAfMnyCsgnMgn[16];	/* 체결후현금위탁증거금액  */
};
#define L_TR4004_mod		sizeof(struct TR4004_mod)


#pragma	pack()