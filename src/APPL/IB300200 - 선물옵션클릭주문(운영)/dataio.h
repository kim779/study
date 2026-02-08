// definitions for Hoga Fast Click IB300200
#pragma once
#define	cDEL	0x7f		// Delimiter
#define	cTAB	'\t'
#define	cLF		'\n'
#define	cCR		'\r'
#define	cNL		0x0A

#define MD_JUMUN	1
#define MS_JUMUN	2

#define ACC_WIDTH	140
#define CTRL_HEIGHT	20

#define LABEL_WIDTH 50
#define TEXTOUT_WIDTH 55

#define MAP_WIDTH	598
#define MAP_HEIGHT	582
#define MAP_HEIGHT2	365

#define ATTACHMAP_WIDTH		300
#define ATTACHMAP_HEIGHT	533

#define HOGA_WIDTH	422
#define HOGA_HEIGHT	454

#define CTRL_WIDTH	160

#define KEY_ACCOUNT	255
#define KEY_ACCNAME	254
#define KEY_JANGO	201
#define KEY_TRAN	202
#define KEY_JUMUN	203
#define KEY_MAXMDJMCN	204
#define	KEY_MAXMSJMCN	205
#define KEY_MAXMISUJMCN	206
#define KEY_MICHEG	207
#define KEY_NOTIFYCTRL	208
#define KEY_JUMUNMDCAN	209
#define KEY_JUMUNMSCAN	210
#define KEY_COVERCAN	211
#define KEY_CANALLS	212
#define KEY_CANALLB	213
#define KEY_CHKPASS	100
#define KEY_OPTIONEX 253

#define TK_CHECKPSWD	23

#define COLOR_LINE	93
#define COLOR_TABLE	75
#define COLOR_BK	64
#define COLOR_GRIDHEAD	74
#define COLOR_GUIDEBACK	213
#define COLOR_PANELBACK	66
#define COLOR_BODYBACK	181
#define COLOR_GRIDHEADTXT	76


#define COLOR_DATA	99
#define COLOR_TEXT	69
#define COLOR_TABLEHEAD	96
#define	COLOR_MDB	171		// 매도 back
#define	COLOR_MSB	170		// 매수 back
#define COLOR_PLUSTXT	94
#define COLOR_MINUSTXT	95

#define VERT_GAP	3
#define HORI_GAP	3
#define BOT_GAP		6

#define GAP	2


#define ST_TEXT         0x00000000	// 텍스트
#define ST_NORMAL       0x00000001	// 0 제거 
#define ST_COLOR        0x00000002	// 칼라 
#define ST_REMOVE	0x00000004	// +, - 제거
#define ST_COMMA	0x00000008	// comma
#define ST_PERCENT      0x00000010	// %
#define ST_SHGA		0x00000020	// 상한/하한 상승 하락 표시
#define ST_BOLD		0x00000040	// bold
#define ST_DRYL		0x00000080	// 등락율 표시	
#define ST_MMCOLOR	0x00000100	// 매매구분
#define ST_BGCOLOR	0x00000200  // 백그라운드 색상	koga~jega range
#define ST_PLUS_REMOVE 0x00000400	// +만 제거 (등락률)

#define MAX_FC	30

struct _smid
{
	char	code[8];	        // 종목코드
};
#define sz_SMID		sizeof(struct _smid) 

struct  _smod
{
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
    char 	dscha[9];		// 잔량차
	char	mgjv[9];		// 미결제약정
	struct	{
		char time[6];		// 체결시간
		char curr[9];		// 체결가격
		char cvol[6];		// 체결수량
	} cheg[MAX_FC];
};
#define sz_SMOD		sizeof(struct _smod)

struct _jinfo
{
	char    juno[6];		// 원주문 번호
	char    mmgb[1];		// 매도/수 구분 
	char    jprc[9];		// 주문가격
	char    jqty[9];		// 주문수량
	char    mcvl[9];		// 미체결 수량
};
#define sz_JINFO	sizeof(struct _jinfo)

struct _config
{
	int	percent{};		// 등락률 표시 0:표시안함, 1:표시
	int	percentRadio{};		// 0:시고저, 1:BEP, 2:user
	int	percentKind{};		// 0:기준가, 1:전일종가, 2:시가, 3:고가: 4:저가
	CString user;
	int	mod{};			// 정정주문시 모두/일부 0:모두 1:일부
	int	jmcn{};			// 주문수량 표시 방법 0:주문(건수) 1:주문
	int	confirm{};		// 주문확인창 보기 유무 0:안보기 1:보기
};

struct _jmid
{
	char	nrec[4];
	char	odgb;		// 주문유형 1:일반, 2:선옵, 3:단주매매, 4:신용매매, 5:채권매매
	char	mkgb;		// 시장구분 1:거래소, 2:코스닥, 3:프리보드, 5:장외
	char	mmgb;		// 매매구분 1:매도, 2:매수, 3:정정, 4:취소
	char	acno[11];	// 계좌
	char	pswd[8];	// 비번
	char	ogno[12];	// 원주문번호
	char	code[12];	// 종목코드
	char	jqty[8];	// 주문수량
	char	jprc[10];	// 주문단가  선옵 100배수처리 112.15 -> 11215
	char	hogb[2];	// 호가구분 00:지정가
	char	jmgb[1];	// 주문조건 1:IOC, 2:FOK
	char	mdgb;		// 정정취소 1:일부, 2:전부
	char	prgb;		// 처리구분 'X'
	char	gmo[20];
	char	gmm[20];
};
#define sz_JMID		sizeof(struct _jmid)

struct _jmod
{
	char	nrec[4];
	char	jmmo[6];	// 주문번호
	char	mono[6];	// 원주문번호 (정정/취소시)
	char	idat[80];	// 입력데이타
};
#define	sz_JMOD		sizeof(struct _jmod)

struct	_cmid	
{
	char	accn[11];
	char	pswd[8];
	char	gubn[1];	/* 0.All 1.체결 2.미체결	*/
	char	juno[6];
	char	sort[1];	/* 1.역순 2.정순		*/
	char	date[8];
	char	cod2[8];	/* 종목코드			*/
	char	mkgb[1];
	char	dsgb[1];
};
#define sz_CMID		sizeof(struct _cmid)

struct	f_rec	{
	char	mono[6];	/* 모주문번호		*/
	char	juno[6];	/* 주문  번호		*/
	char	ojno[6];	/* 원주문 번호		*/
	char	cod2[8];	/* 종목코드		*/
	char	hnam[30];	/* 종목명		*/
	char	odgb[8];	/* 주문구분		*/
	char	hogb[20];	/* 주문유형		*/
	char	jprc[11];	/* 주문가격		*/
	char	jqty[7];	/* 주문수량		*/
	char	dlgb[4];	/* 체결구분		*/
	char	dprc[11];	/* 체결가격		*/
	char	dqty[7];	/* 체결수량		*/
	char	dtim[6];	/* 체결시간		*/
	char	wqty[7];	/* 미체결수량		*/
	char    hqty[7];    /* 정정/취소수량    */
	char	stat[8];	/* 접수상태		*/
    char    time[6];    /* 접수시간         */
    char    jseq[6];    /* 접수번호         */
    char    yseq[7];    /* 약정번호         */
    char    ecod[4];    /* 거부코드         */
	char	dseq[4];	/* deal count		*/
};
#define L_FMICHE_REC		sizeof(struct f_rec)

struct	_cmod	
{
	char	accn[11];
	char	nrec[4];
	struct	f_rec	rec[250];
};
#define sz_CMOD		sizeof(struct _cmod)


struct _fcan_mid
{
	char accn[10];
	char pswd[8];
	char cod2[8];
	char gubn[1]; /* 0. 신규, 1. 청산 */
	char msgb[1]; /* 1. 매도, 2. 매수 */
	/* 청산시 이하 불필요*/
	char jmyh[1]; /* 1.지정, 2.조건 3.시장 4. 최유리 */
	char oprc[9]; /* 기준 가격 ( *100) */
};
#define sz_FCAN_MID	sizeof(struct _fcan_mid)

struct _fcan_mod
{
	char cqty[7]; /* 주문가능 수량 */
};
#define sz_FCAN_MOD	sizeof(struct _fcan_mod)


struct  pmid {
    char    accn[11];
    char    pswd[8];
};

struct  pmod {
    char    sflg[1];                /* OK '1' else ERROR    */
    char    emsg[99];
}; 

#define L_pmid	sizeof(struct pmid)
#define L_pmod	sizeof(struct pmod)


// 최대주문가능수량

struct max_mid
{
	char In[5];               	// 레코드갯수             
	char zAcntNo[20];         	// 계좌번호               
	char zPwd[8];             	// 비밀번호               
	char zQryTp[1];           	// 조회구분 (1:일반, 2:금액, 3:비율)
	char lOrdAmt[16];         	// 주문금액 (조회구분이 2일경우만 사용, 그외는 0으로 입력되어야함)
	char dRatVal[19];         	// 비율값 (조회구분이 3일경우만 사용, 그외는 0으로 입력되어야함)
	char zFnoIsuNo[32];       	// 선물옵션종목번호       
	char zBnsTp[1];           	// 매매구분 (1 : 매도, 2 : 매수)
	char dOrdPrc[13];         	// 주문가                 
	char zFnoOrdprcPtnCode[2];	// 선물옵션호가유형코드 (00:지정가, 03:시장가, 05:조건부지정가, 06:최유리지정가)
   
};

struct max_mod
{
	char Out[5];              // 레코드갯수           
	char zAcntNm[40];         // 계좌명               
	char zQryDt[8];           // 조회일               
	char dCurPrc[13];         // 현재가               
	char lOrdAbleQty[16];     // 주문가능수량         
	char lNewOrdAbleQty[16];  // 신규주문가능수량     
	char lLqdtOrdAbleQty[16]; // 청산주문가능수량     
	char lUsePreargMgn[16];   // 사용예정증거금액     
	char lUsePreargMnyMgn[16];// 사용예정현금증거금액 
	char lOrdAbleAmt[16];     // 주문가능금액         
	char lMnyOrdAbleAmt[16];  // 현금주문가능금액     
};

#define L_max_mid	sizeof(max_mid)
#define L_max_mod	sizeof(max_mod)

/*
struct _maxjmct_mid
{
	char in[5];					// 레코드 갯수
	char zAcntNo[20];			// 계좌번호
	char zPwd[8];				// 비밀번호
};

#define L_maxjmct_mid	sizeof(struct _maxjmct_mid)

struct _maxjmct_rec1
{
	char lDps[16];				// 예수금
	char lMnyoutAbleAmt[16];	// 출금가능금액
	char zTp[1];				// 구분
	char zTpNm1[40];			// 구분명1
};

struct _maxjmct_rec2
{
	char zShtnIsuNo[9];			// 단축종목번호
	char zIsuNo[12];			// 종목번호
	char zIsuNm[40];			// 종목명
	char lBalQty[16];			// 잔고수량
	char lBalAmt[16];			// 잔고금액
	char lEvalAmt[16];			// 평가금액
	char lOrdAbleQty[16];		// 주문가능수량
	char dCurPrc[13];			// 현재가
};

struct _maxjmct_mod
{
	char Out1[5];				// 레코드 갯수
	struct _maxjmct_rec1 rec1[99999];
	char Out2[5];				// 레코드 갯수
	struct _maxjmct_rec2 rec2[99999];
};
*/
