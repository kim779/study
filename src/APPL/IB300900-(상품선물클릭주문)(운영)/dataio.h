#pragma once
typedef enum { 
	TK_PIBO3002      =   1,
	TK_PIBOFCHG      =   2,
	TK_PIBOFJGO      =   3,
	TK_PIBOFODR      =   4,
	TK_SDLDQ003      =   5,
	TK_SACMT238      =   6,
	TK_SONBQ101      =   7,
	TK_CODE_CTRL     = 100,
	TK_NOTIFY_CTRL   = 200,
	TK_ACCOUNT_CTRL1 = 254,
	TK_ACCOUNT_CTRL2 = 255,
} TR_KEY;

typedef struct _pibo3002_mid {
	char	code[8];	        // 종목코드
} pibo3002_mid;

#define MAX_FC	30

typedef struct _pibo3002_mod {
	char    curr[9];                // 현재가       
	char    diff[9];                // 전일대비     
	char    gvol[12];               // 거래량  
	char 	gamt[12];		// 거래대금  
	char    rate[9];		// 등락율
	char    shga[9];                // 상한가       
	char    hhga[9];                // 하한가 
	char    gjga[9];                // 기준가       
	char    siga[9];                // 시가         
	char    koga[9];                // 고가         
	char    jega[9];                // 저가         
	struct  {
		char     mdga[9];	// 매도호가            (999V99) 
		char     msga[9];       // 매수호가            (999V99)
		char     dvol[9];       // 매도호가수량        (계약)  
		char     svol[9];       // 매수호가수량        (계약)  
		char     dcnt[9];       // 매도호가건수
		char     scnt[9];       // 매수호가건수
	} hoga[5];		        // 선물/옵션 호가
	char    dvol[9];                // 매도호가총수량
	char    svol[9];                // 매수호가총수량
	char    dcnt[9];                // 매도호가총건수
	char    scnt[9];                // 매수호가총건수
	char 	sdch[9];		// 잔량차
	char	mgjv[9];		// 미결제약정
	struct  {
		char time[6];		// 체결시간 HHMMSS 
		char curr[9];		// 체결가격
		char cvol[6];		// 체결수량
	} cheg[MAX_FC];
} pibo3002_mod;

typedef struct _pibofchg_mid {
    char accn[11];
    char pswd[8];
    char dlgb[1];    /* 0.All 1.체결 2.미체결    */
    char juno[6];
    char sort[1];    /* 0.X   1.역순 2.정순      */
    char date[8];
    char cod2[8];    /* 종목코드                 */
    char cpgb[1];    /* 0.All F.선물 C.콜옵션 P.풋옵션 S.스프레드 X.특정종목 */
    char dsgb[1];    /* 0.All 1.매도 2.매수      */
    char dllf[1];    /* 0.MAP 1.DLL      */
} pibofchg_mid;

typedef struct _pibofchg_grid {
    char mono[6];    /* 모주문번호       */
    char juno[6];    /* 주문  번호       */
    char ojno[6];    /* 원주문 번호      */
    char cod2[8];    /* 종목코드         */
    char hnam[30];   /* 종목명           */
    char odgb[8];    /* 주문구분         */
    char hogb[20];   /* 주문유형         */
    char oprc[11];   /* 주문가격         */
    char oqty[7];    /* 주문수량         */
    char dlgb[4];    /* 체결구분         */
    char dprc[11];   /* 체결가격         */
    char dqty[7];    /* 체결수량         */
    char dtim[6];    /* 체결시간         */
    char wqty[7];    /* 미체결수량       */
    char hqty[7];    /* 정정/취소수량    */
    char stat[8];    /* 접수상태         */
    char time[6];    /* 접수시간         */
    char jseq[6];    /* 접수번호         */
    char yseq[7];    /* 약정번호         */
    char ecod[4];    /* 거부코드         */
    char dseq[4];    /* 체결횟수         */
} pibofchg_grid;

typedef struct _pibofchg_mod {
    char    accn[11];
    char    nrec[4];
    pibofchg_grid grid[1];
} pibofchg_mod;

typedef struct _pibofodr_mid {
	char nrec[4];
	char odgb[1];    /* 주문유형 : 1:일반 2:선옵 3:예약                  */
	char mkgb[1];    /* Filler                                           */
	char mmgb[1];    /* 매매구분 : 1:매도 2:매수 3:정정 4: 취소          */
	char accn[11];   /* 계좌번호                                         */
	char pswd[8];    /* 비밀번호                                         */
	char ojno[12];   /* 원주문번호                                       */
	char cod2[12];   /* 단축코드                                         */
	char jqty[8];    /* 주문수량                                         */
	char jprc[10];   /* 주문단가                                         */
	char hogb[2];    /* 호가구분                                         */
	char cdgb[1];    /* 조문조건 : 0:X   1:IOC 2: FOK                    */
	char mdgb[1];    /* 정정취소 : 1:일부 2:전부                         */
	char prgb[1];    /* 처리구분 : 'A,B,C,D' : 이하 가변 'X' : 사용안함  */
	char usrf[40];
} pibofodr_mid;

typedef struct _pibofodr_grid {
	char    jmno[6];    /* 주문 번호        */
	char    ojno[6];    /* 원주문번호       */
	char    emsg[80];   /* 오류 메세지      */
} pibofodr_grid;

typedef struct _pibofodr_mod {
	char    nrec[4];        /* 갯수        */
	pibofodr_grid grid[1];   /* 응답        */
} pibofodr_mod;

typedef struct _SDLDQ003_mid {
	char In[5];				/* 레코드갯수                                 */
	char QryTp[1];			/* 조회구분       1:그룹조회 2:계좌조회       */
	char GrpTp[2];			/* 그룹유형코드   10:주식, 11:선물, 12:혼합   */
	char GrpSeqn[20];		/* 그룹번호                                   */
	char GrpName[40];		/* 그룹명                                     */
} SDLDQ003_mid;

typedef struct _SDLDQ003_grid {
	char GrpSeqn[20];		/* 그룹번호       */
	char GrpName[40];		/* 그룹명         */
	char Acno[20];			/* 그룹계좌번호   */
	char Acnm[40];			/* 계좌명         */
	char Pswd[8];			/* 비밀번호       */
	char Rate[17];			/* 빈도           */
	char Mult[17];			/* 승수비율       */
	char Alias[40];			/* 그룹계좌부기명 */
} SDLDQ003_grid;

typedef struct _SDLDQ003_mod {
	SDLDQ003_mid mid;
	char In[5];				/* 레코드갯수     */
	SDLDQ003_grid grid[1];
} SDLDQ003_mod;

typedef struct _SACMT207_mid {
	char in[5];
	char acctNo[20];
	char password[8];
	char zTrxTp[1];
} SACMT207_mid;

typedef struct _SACMT207_mod {
	SACMT207_mid in;
	char out[5];
	char lPwdContErrCnt[9];
	char lAtmPwdContErrCnt[9];
	char lMtrsfPwdContErrCnt[9];
	char zAcntNm[40];
} SACMT207_mod;

typedef struct _SACMT238_mid {
	char In[5];
	char zAcntNo[20];
	char zPwd[8];
} SACMT238_mid;

typedef struct _SACMT238_mod {
	struct _SACMT238_mid mid;
	char Out[5];
	char zAvalYn[1];
} SACMT238_mod;

typedef struct _SONBQ101_mid {
	char In[5];                        /* 레코드갯수            */
	char zAcntNo[20];                  /* 계좌번호              */
	char zPwd[8];                      /* 비밀번호              */
	char zQryTp[1];                    /* 조회구분              */
	char lOrdAmt[16];                  /* 주문금액              */
	char dRatVal[19];                  /* 비율값                */
	char zFnoIsuNo[32];                /* 선물옵션종목번호      */
	char zBnsTp[1];                    /* 매매구분              */
	char dOrdPrc[13];                  /* 주문가                */
	char zFnoOrdprcPtnCode[2];         /* 선물옵션호가유형코드  */
} SONBQ101_mid;

typedef struct _SONBQ101_mod {
	SONBQ101_mid mid;
	char Out[5];                       /* 레코드갯수            */
	char zAcntNm[40];                  /* 계좌명                */
	char zQryDt[8];                    /* 조회일                */
	char dCurPrc[13];                  /* 현재가                */
	char lOrdAbleQty[16];              /* 주문가능수량          */
	char lNewOrdAbleQty[16];           /* 신규주문가능수량      */
	char lLqdtOrdAbleQty[16];          /* 청산주문가능수량      */
	char lUsePreargMgn[16];            /* 사용예정증거금액      */
	char lUsePreargMnyMgn[16];         /* 사용예정현금증거금액  */
	char lOrdAbleAmt[16];              /* 주문가능금액          */
	char lMnyOrdAbleAmt[16];           /* 현금주문가능금액      */
} SONBQ101_mod;

