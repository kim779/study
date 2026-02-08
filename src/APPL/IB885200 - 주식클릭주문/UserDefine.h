#pragma once

//----------------------------------------------------------------------------------------
//	ETC
//----------------------------------------------------------------------------------------
#define	cDEL	0x7f		// Delimiter
#define	cTAB	'\t'
#define	cLF		'\n'
#define	cCR		'\r'
#define	cNL		0x0A

#define GAP	3


#define ST_TEXT         0x00000000	// 텍스트
#define ST_NORMAL       0x00000001	// 0 제거 
#define ST_COLOR        0x00000002	// 칼라 
#define ST_REMOVE	0x00000004	// +, - 제거
#define ST_COMMA	0x00000008	// comma
#define ST_PERCENT      0x00000010	// %
#define ST_SHGA		0x00000020	// 상한/하한 상승 하락 표시
#define ST_BOLD		0x00000040	// bold
#define ST_DRYL		0x00000080	// 등락율 표시	


#define MAP_TITLE	"[8852] 주식 클릭주문"
#define MAP_NAME	"IB885200"
#define MAP_NO		"8852"




//----------------------------------------------------------------------------------------
//	User Message & Parameter
//----------------------------------------------------------------------------------------
#define	UM_MYMESSAGE		WM_USER + 1
#define	UP_GET_JCOD		10
#define	UP_GET_CODX		11
#define	UP_GET_ACCOUNT		12
#define	UP_GET_PASSWORD		13
#define	UP_GET_NAME		14
#define	UP_GET_CODENAME		15
#define UP_GET_CONDITION	16
#define	UP_SET_CODE		30
#define	UP_SET_GUIDE		31
#define	UP_SEND_JUMUN		40
#define	UP_SEND_MAXJMCN		41
#define	UP_INVALIDATE_HOGA	51





#define	UM_HOGAMESSAGE		WM_USER + 2
#define	UP_RESERVE		10




//----------------------------------------------------------------------------------------
//	매매구분
//----------------------------------------------------------------------------------------
#define MD_JUMUN	1
#define MS_JUMUN	2
#define MS_MISU		3


//----------------------------------------------------------------------------------------
//	Size
//----------------------------------------------------------------------------------------
#define ACC_WIDTH		140
#define CTRL_HEIGHT		20

#define MAP_WIDTH		653
#define MAP_HEIGHT		555	//487

#define ATTACHMAP_WIDTH		300
#define	ATTACHMAP_HEIGHT	493

#define HOGA_WIDTH		480	//430
#define HOGA_HEIGHT		449	//403

#define CTRL_WIDTH		150

#define VERT_GAP	3
#define HORI_GAP	3
#define WIDE_GAP	5
#define BOT_GAP		5




//----------------------------------------------------------------------------------------
//	TR Name
//----------------------------------------------------------------------------------------
#define TN_SISE		"PIBO1003"
#define TN_MICHE	"PIBOSCHG"
#define TN_JUMUN	"PIBOSODR"
#define TN_SYMBOL	"POOPPOOP"
//#define TR_JANGO	"PIBOTUXQ"	// TxHead: SC234500_Q
#define TN_MAXJMCT	"pibopbxq"	// TxHead: SC217000_Q
#define TN_CHECKPSWD	"pibopswd"


//----------------------------------------------------------------------------------------
//	Service Name
//----------------------------------------------------------------------------------------
//#define SN_MAXJMCT	"SC217000_Q"
#define SN_MAXJMCT	"SONAQ200"


//----------------------------------------------------------------------------------------
//	TR Key
//----------------------------------------------------------------------------------------
#define TK_SISE		1
#define TK_JUMUN	2
#define TK_MICHE	3
#define TK_MDJMCN	10
#define	TK_MSJMCN	11
#define TK_MAXMSJMCN	12
#define TK_ACCOUNT	200
#define TK_SYMBOL	21
#define TK_NOTIFYCTRL	22
#define TK_CHECKPSWD	23

#define TK_GROUP			100
#define TK_ACCNTNAME		101




//----------------------------------------------------------------------------------------
//	Color
//----------------------------------------------------------------------------------------
#define COLOR_LINE	93
#define COLOR_TABLE	75
#define COLOR_BK	64
#define COLOR_CONTENTS_BK	181
#define COLOR_GRIDHEAD	74
#define COLOR_GUIDEBACK	66
#define COLOR_DATA	99
#define COLOR_TEXT	130
#define COLOR_TABLEHEAD	96
#define COLOR_GRIDHEADTXT	76



//----------------------------------------------------------------------------------------
//	ETC Structure
//----------------------------------------------------------------------------------------
struct _jumun
{
	char	cMmgb;
	int	iPrice;
	int	iJuno;
	int	iJqty;
};


//----------------------------------------------------------------------------------------
//	Notice Symbol
//----------------------------------------------------------------------------------------
#define ACCN	901		// 계좌
#define JUNO	904		// 주문번호
#define OJNO	905		// 원주문번호
#define CODX	907		// RTS 종목코드
#define JQTY	909		// 주문수량
#define JPRC	910		// 주문가격
#define MMGB	912		// 매매구분 (매도, 매수)
#define MCCT	921		// 미체결수량
#define OKCT	931		// 체결, 정정, 취소 수량
#define NTKD	996		// Notice Kind
				//  2:    // 정정확인     
				//  3:    // 취소확인     
				//  1:    // 체결
				// 21:    // 접수         
				// 24:    // 기타접수     
				//  4:    // 거부         
				// 31:    // 접수오류     
				// 32:    // 정정접수오류 
				// 33:    // 취소접수오류 
				// 34:    // 기타접수오류 
				// 22:    // 정정접수     
				// 23:    // 취소접수


//----------------------------------------------------------------------------------------
//	ETC Structure
//----------------------------------------------------------------------------------------
struct _config 
{
	int	percent{};		// 등락률 표시 0:표시안함, 1:표시
	int	percentRadio{};		// 0:시고저, 1:BEP, 2:user
	int	percentKind{};		// 0:기준가, 1:전일종가, 2:시가, 3:고가: 4:저가
	CString	BEP;
	CString user;
	int	mod{};			// 정정주문시 모두/일부 0:모두 1:일부
	int	jmcn{};			// 주문수량 표시 방법 0:주문(건수) 1:주문
	int	confirm{};		// 주문확인창 보기 유무 0:안보기 1:보기
	int	pgsi{};			// 평가손익 표시 0:안보기, 1:보기
};


//----------------------------------------------------------------------------------------
//	TR Structure
//----------------------------------------------------------------------------------------

struct _sise_mid
{
	char	code[6];	        // 종목코드
};
#define sz_SISEMID		sizeof(struct _sise_mid) 

struct	_sise_mod	{
	char	codx[12];		// RTS CODE
	char	jmgb[1];		// 종목구분(+KP, -KQ)	
	char	curr[9];		// 현재가	
	char	diff[9];		// 전일대비	
	char	gvol[12];		// 거래량	
	char	gamt[12];		// 거래대금	
	char	jvol[12];		// 전일 거래량	
	char	rate[9];		// 등락율	
	char	shga[9];		// 상한가	
	char	hhga[9];		// 하한가	
	char	gjga[9];		// 기준가	
	char	siga[9];		// 시가		
	char	koga[9];		// 고가		
	char	jega[9];		// 저가		
	char 	jgbn[3];		// 증거금율	
	struct	{
		char	mdga[9];	// 매도호가     	
		char    msga[9];	// 매수호가			
		char    dvol[9];	// 매도호가수량 	 
		char    svol[9];	// 매수호가수량		 
		char    dcha[9];	// 직전매도호가수량 
		char    scha[9];	// 직전매수호가수량 
		char    dcnt[9];	// 매도호가건수     
		char    scnt[9];	// 매수호가건수		
	} hoga[10];		// 선물/옵션 호가 	

	char	dvol[9];		// 호가총수량 : 매도
	char	svol[9];		//            : 매수
	char	dcha[9];		// 직전대비총량:매도
	char	scha[9];		// 	       :매수	
	char	sdch[9];		// 잔량차(svol-dvol)
	char	sum4[9];		// 종가합계 : 5일 	
	char	sum9[9];		// 종가합계 : 9일 	
	char	jggy[9];		// 증거금율
	char	jmdw[9];		// 주문단위
};
#define sz_SISEMOD		sizeof(struct _sise_mod)

struct	_miche_mid	
{
	char    accn[11];
	char    pswd[8];
	char    date[8];	//  => "            "
	char    cod2[12];	//  => "A        "
	char    juno[5];	// 시작주분번호               
	char    dsgb[1];	// 0.All 1.매도 2.매수      
	char    otim[1];	// 0.All 1.장중 2.시간외    
	char	tcod[2];	// 00.All else. 해당매체코드
	char    dlgb[1];	// 0.All 1.체결 2.미체결    
	char    sort[1];	// 1.역순 2.정순            
};
#define sz_MICHEMID		sizeof(struct _miche_mid)

struct	s_rec	{
	char    juno[5];    // 주문   번호      
	char    ojno[5];    // 원주문 번호      
	char    cod2[12];   // 종목코드         
	char    hnam[40];   // 종목명           
	char    odgb[20];   // 주문구분         
	char    jcgb[20];   // 정정취소구분 
	char    hogb[20];   // 호가구분         
	char    jprc[12];   // 주문가격         
	char    jqty[12];   // 주문수량         
	char    dprc[12];   // 체결가격         
	char    dqty[12];   // 체결수량         
	char    tqty[12];   // 체결수량합       
	char    wqty[12];   // 미체결수량       
	char    stat[20];   // 접수상태         
	char    time[8];    // 주문시간         
};
#define L_MICHE_REC		sizeof(struct s_rec)

struct	_miche_mod	
{
	char	accn[11];
	char	nrec[4];
	struct	s_rec	rec[250];
};


struct _jumun_mid		// 주문
{
	char	rcnt[4];
	char	odgb;
	char	mkgb;
	char	mmgb;
	char	acno[11];
	char	pswd[8];
	char	ogno[12];
	char	code[12];
	char	jqty[8]; 
	char	jprc[10];  
	char	hogb[2];
	char	jmgb[1];
	char	mdgb;
	char	prgb;
	char	gmo[20];
	char	gmm[20];
};
#define sz_JUMUNMID		sizeof(struct _jumun_mid)

struct _jumunmod
{
	char	rcnt[4];
	char	jmno[6];
	char	mono[6];
	char	omsg[80];
};

// 최대주문가능수량

struct _maxjmct_mid
{
	char in[5];				//	레코드갯수
	char zBnsTp[1];			//	매매구분
	char zAcntNo[20];		//	계좌번호
	char zPwd[8];			//	비밀번호
	char zIsuNo[12];		//	종목번호
	char dOrdPrc[13];		//	주문가
	char zCommdaCode[2];	//	통신매체코드(미사용)
	char zLoanPtnCode[2];	//	대출유형코드(미사용)
};

#define L_maxjmct_mid	sizeof(struct _maxjmct_mid)


struct _maxjmct_mod
{
	char Out[5];						//	레코드갯수											
	char zAcntNm[40];					//	계좌명
	char zIsuNm[40];					//	종목명
	char zBnsTpNm[10];					//	매매구분
	char lDps[16];						//	예수금
	char lSubstAmt[16];					//	대용금액
	char lMnyMgn[16];					//	현금증거금액
	char lSubstMgn[16];					//	대용증거금액
	char lRcvblSumAmt[16];				//	미수합계금액
	char lRuseOrdAbleAmt[16];			//	재사용주문가능금액
	char lMnyRuseObjAmt[16];			//	현금재사용대상금액
	char lSubstRuseObjAmt[16];			//	대용재사용대상금액
	char lPrdayRuseOrdAbleAmt[16];		//	전일재사용주문가능금액
	char lCrdayRuseOrdAbleAmt[16];		//	금일재사용주문가능금액
	char lMnyOrdAbleAmt[16];			//	현금주문가능금액
	char lSubstOrdAbleAmt[16];			//	대용주문가능금액
	char lCrdtPldgRuseAmt[16];			//	신용담보재사용금액
	char lSeOrdAbleAmt[16];				//	거래소금액
	char lKdqOrdAbleAmt[16];			//	코스닥금액
	char lAbleAmt[16];					//	가능금액
	char lMgnRat100pctOrdAbleAmt[16];	//	증거금률100퍼센트주문가능금액
	char lMgnRat100OrdAbleQty[16];		//	증거금률100퍼센트주문가능수량
	char dLoanPldgRat[9];				//	대출담보율
	char dPldgMaintRat[7];				//	담보유지비율
	char lOrdAbleQty[16];				//	주문가능수량
	char lUnercBuyOrdAmt[16];			//	미체결매수주문금액
	char dCmsnAmtRat[11];				//	수수료율
	char lAddCmsnAmtLevyAmt[16];		//	추가수수료징수금액
	char lBalQty[16];					//	잔고수량
	char lHtsOrdAbleAmt[16];			//	HTS주문가능금액
	char lD1Dps[16];					//	D+1예수금
	char lD2Dps[16];					//	D2예수금
	char lPrdayRuseOrdAbleQty[16];		//	전일재사용주문가능수량
	char lCrdayRuseOrdAbleQty[16];		//	금일재사용주문가능수량
	char lCrdtOrdAbleAmt[16];			//	신용주문가능금액
};

#define L_maxjmct_mod	sizeof(struct _maxjmct_mod)

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

/*
struct _maxjmct_mid
{
	char	accn[10];	// 계좌번호
	char	pswd[8]	;	// 비밀번호
	char	dsgb;		// 조회구분	1: 매수 2: 매도     
	char	jcod[12];	// 종목번호
	char	jmdg[12];	// 주문가격
	char	sjmg[2]	;	// 수수료적용매체구분	00:일반    100%주문가능수량의 수수료 계산시 적용할 매체구분  
						//	01:사이버      
						//	02:콜센터      
						//	03:오토고객     
	char	sgog;		// 시간외구분	0:일반 1:시간외     
};

struct _maxjmct_mod
{
	char	ajgy[9];	// 계좌증거금률
	char	jjgy[9];	// 종목증거금률
	char	ajyn[20];	// 계좌증거금률명
	char	jjyn[20];	// 종목증거금률명
	char	mxjg[12];	// 최대주문가능금액
	char	mnjg[12];	// 최소주문가능금액
	char	sghg[12];	// 사용가능현금
	char	sgdy[12];	// 사용가능대용
	char	sdjs[12];	// 신용담보재사용
	char	msgc[12];	// 매수/매도가능수량
	char	bpsc[12];	// 100%매수가능수량
	char	jjgc[12];	// 전일재사용가능수량
	char	gjgc[12];	// 금일재사용가능수량
	char	ysgm[12];	// 예수금
	char	dygm[12];	// 대용금
	char	hgjg[12];	// 현금증거금
	char	dyjg[12];	// 대용증거금
 };
*/


