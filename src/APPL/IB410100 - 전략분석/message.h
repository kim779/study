#pragma once
#define MSG_ACCOUNTCHANGE	10	// CMapWnd
#define	MSG_NOTIFY		20	// CMapWnd
#define	MSG_DEPOSIT		40	// 증거금->CMapWnd
#define	MSG_SELECTSTG		30	// CStgBoard

#define KEY_ACCOUNT		99
#define KEY_CODEMINGAM		1
#define KEY_CODEMINGAM2		2
#define	KEY_INITVALUE		3
#define	KEY_UPJONG		4
#define	KEY_DEPOSIT		5
#define	KEY_DEPGUESS		6
#define	KEY_HEDGEOPTION		10
#define KEY_JJGA	11

//***************************************************************
#define	WM_MAPAddSymbol		WM_USER + 3
#define	WM_MAPGraph			WM_USER + 4
	#define	GrpReset		1
	#define	GrpChange		2
	#define	SetSumDelta		3
	#define	SetSumGamma		4
	#define	SetSumSeta		5
	#define	SetSumVega		6
	#define	SetK200			7
	#define	SetYetak		8
#define	WM_MAPStrategy		WM_USER + 5
	#define	ApplyStdStg		0	// apply standard strategy
	#define	ApplyUsrStg		1	// apply user strategy
	#define	SaveStg			2	// save to server
	#define	LoadStg			3	// load from server
#define	WM_MAPHedgeData		WM_USER + 6
	#define	IsHedgeable		1
	#define	GetHedgeTarget		2
	#define	GetHedgeData		3
	#define	ReqOrder		4
#define	WM_MAPRefresh		WM_USER + 7
	#define	UpdateUserList	1
	#define	ResetHoga		2	// reset HogaWind
	#define	UpdateHoga		3	// update hogawnd
#define	WM_MAPLineMove		WM_USER + 9
	#define	LineMovePoint	1
	#define	LineMoveAll		2
#define	WM_MAPUser			WM_USER + 10
	#define	RequestOrder	0
	#define	RequestBalance	1
	#define	RequestIV		2
	#define	RequestMargin	3
	#define	RequestMoney	4
	#define	SetJKey			5
	#define	RequestGuide	6
#define	WM_MAPFileEnd		WM_USER + 11	// end of save/load
	#define	EndUpLoad		1
	#define	EndDnLoad		2
#define	WM_MAPAccnInfo		WM_USER + 12
	//#define	AccnChg			1	// account change event(ProWnd -> ProPage2_3)
	#define	AccnOrg			2	// accn + pswd
	#define	AccnEnc			3	// accn + pswd(encryption)
#define	WM_MAPGetProfile	WM_USER + 13
#define WM_MAPMouseUp		WM_USER + 14
#define	WM_MAPStgApply		WM_USER + 15
#define	WM_MAPStgAdd		WM_USER + 16

// profile message
#define WM_PROPrice			WM_USER + 21
#define WM_PROSave			WM_USER + 22
#define WM_PROStgCode		WM_USER + 23
#define WM_PROStgApplyData	WM_USER + 24
#define WM_PROStgAddData	WM_USER + 25
#define	WM_PROBlcApplyData	WM_USER + 26
#define WM_PROAddSymbol		WM_USER + 27
#define WM_PROUser			WM_USER + 28
	#define	EnableOdrBtn	1
	#define	DestroyOdrDlg	2
	#define	ResetBtn		3
	#define	GetGridCode		4
#define	WM_PROAccnInfo		WM_USER + 29
	#define	ChangeAccn		1	// account change event(AccountCtrl -> ProWnd)
	#define	GetOrgAccn		2	// accn + pswd
	#define	GetEncAccn		3	// accn + pswd(encryption)
#define	WM_PROGetHedgeInfo	WM_USER + 30

// position analysis message
#define	WM_POSMSVAL			WM_USER + 41
#define	WM_POSGetHV			WM_USER + 42
#define	WM_POSUser			WM_USER + 43
	#define	SetIVTR			1
#define	WM_POSGetFee		WM_USER + 44
#define	WM_POSInitData		WM_USER + 45
#define	WM_POSInitParam		WM_USER + 46
#define	WM_POSDEPOSITSEL	WM_USER + 47
#define	WM_POSCLEAR		WM_USER + 48


// position graph message
#define	WM_GRPRefresh		WM_USER + 51
	#define	DeleteGrpData	0
	#define	RedrawGraph		1
	#define	SetPayOff		2
	#define	ResetSensitive	3
	#define	SetSensitive	4
	#define SetStgMouseMove	5
#define	WM_GRPAlert		WM_USER + 52
#define	WM_GRPAddData		WM_USER + 53
#define	WM_GRPSetParam		WM_USER + 54
	#define GetJisuStart	0
	#define	GetJisuEnd	1
	#define SetJisuStart	2
	#define	SetJisuEnd	3
	#define	SetJjilMove	4
	//#define	SetCallPutVolMove	5
#define	WM_GRPBasicPrc		WM_USER + 55

// left sheet message
#define	WM_SHTApplyStdStg	WM_USER + 61
#define	WM_SHTUser			WM_USER + 62
	#define	SetStgIndex		0
	#define	GetStgIndex		1

// sheet - propertypage1 message
#define	WM_SP11ApplyStdStg	WM_USER + 91
#define	WM_SP11GrpMov		WM_USER + 92

// sheet - propertypage03 message
#define	WM_SP13User			WM_USER + 93

// sheet - propertypage2_2 message
#define	WM_SP22Data			WM_USER + 94


// strategy 
#define WM_STGInitData		WM_USER + 71


#define INFO_MAX 35		//2014.04.16 KSJ 33 -> 35로 수정

// initial - Info:Information, Stg:Strategy, Usr:User, Odr:Order
#define	TR_InfoInit		0	// 초기정보
#define	TR_InfoHedge	1	// hedge data
#define	TR_InfoUpDown	2	// 사용자정보 서버에 저장/로드
#define TR_InfoIV		3	// 대표iv, call iv, put iv
#define TR_StgApply		4	// 전략적용
#define TR_StgAdd		5	// 전략추가
#define	TR_UsrApply		6	// 사용자전략적용
#define	TR_UsrAdd		7	// 사용자전략추가
#define	TR_UsrMoveApply	8	// Graph LineMove
#define	TR_OdrAccn		11	// 주문계좌(계좌컨트롤)
#define TR_OdrOrder		12	// 주문매매(주문버튼)
#define TR_OdrClear		13	// 주문청산(청산버튼)
#define	TR_OdrHedge		14	// 주문헤지
#define TR_OdrBalance	15	// 주문잔고(잔고버튼)
#define	TR_OdrBlcStg	16	// 주문잔고전략(잔고버튼)
#define TR_OdrBlcMap	17	// 잔고요청(간략잔고)
#define	TR_OdrMargin	18	// 옵션매도증거금
#define	TR_OdrMoney		19	// 주문가능금액
#define SYMBOL_KEY		20	// 종목추가


#define	TM_LOADMAP		2001
#define	TM_MGAP			10
#define	BASE_MID		150

// 20071214
#define	FILE3101		"IB410100.ini"		// 정보저장
#define	RANGESECTION		"RANGE"			// section
#define	STARTRANGE		"STARTRANGE"		// start
#define	ENDRANGE		"ENDRANGE"		// end
#define	CHKRANGE		"CHKRANGE"		// range사용여부

// 20121008
#define MULTICODE	"MULTICODE"		//멀티종목창

#define	STRATEGY_LIST	"Strategy.ini"	// 전략저장파일 목록정보파일
#define	STRATEGY_FILE	"Strategy.i"	// 전략저장파일이름(뒤에 00, 01...이 붙음)
#define	SN_NAME			"NAME"			// name section
#define	SN_CODE			"CODE"			// code section
#define	SN_PROJ			"PROJ"			// project name key
#define	FOSYMBOL		"focod"			// 선물/옵션 심볼
#define	cTAB			'\t'			// tab
#define	cDEL			0x7f			// delimiter

enum { SONIK = 0, DELTA, GAMMA, CETA, VEGA, IRON};
//***************************************************************		

// OOP
#define	TRN_OOP		"POOPPOOP"

// PIBO4101
#define	TRN_MINGAM	"PIBO4101"
#define MAX_CODE	100		//2013.07.24 KSJ
#define DBASE		250000 // 2017.03.06 거래승수

struct  _mid {
	char	crec[3];	// 코드개수
	char	codes[MAX_CODE][8];	// 종목코드(최대 50개) 선물 or 옵션
};
#define	L_MID	sizeof (struct _mid)

struct  _gmod {
	char	ccod[9];	// 종목코드	//2012.04.06 KSJ 8자리에서 9자리로 변경함.
	char	ccur[8];	// 현재가
	char	cdif[8];	// 대비
        char    shga[8];	// 상한가
        char    hhga[8];	// 하한가
        char    mdga[5][8];	// 매도호가1~5
        char    msga[5][8];	// 매수호가1~5
	char	cgvol[12];	// 거래량
	char	jjil[2][3];	// 잔존일수 (거래일수/달력일수)
	char	bdjs[5];	// 배당지수(20060323추가)
	char	gdan[9];	// 거래승수(2012.05.17 추가됨) 선물 500000, 옵션 6월물 경우에 따라서 100000 이거나 5000000이어서 추가됨.
};
#define L_GMOD	sizeof (struct _gmod)

struct  _mod {
	char	frts[8];	// 최근월물 선물
	char	fnam[20];
	char	fcur[8];
	char	krts[8];	// KOSPI
	char	kcur[8];
	char	k200rts[8];	// KOSPI 200
	char	k200cur[8];
	char	k200tim[8];	// KOSPI 200 Time(20070319추가)
	char	hv[4][8];	// hv (90/60/30/21)
	char	cdgl[5];	// CD 금리
	//char	bdjs[5];	// 배당지수(20060323삭제)
	char	grec[4];	// 코드개수( count of gmod )
	struct  _gmod    gmod[MAX_CODE];  // MAX_R = 50
};
#define	L_MOD	sizeof (struct _mod)
//-----------------------------------

struct _GraphData	{
	char		code[9];// 종목코드 저장
	double		hsga;	// 행사가
	BYTE		gubn;	// 종목코드 구분자
	double		numb;	// 수량
	double		curr;	// 현재가
	double		prc;	// 주문단가
//	double		cdgr;	// CD금리
	double		hvbd;	// 역사적변동성
	double		jjis[2];	// 잔존만기일(달력,거래)
	int		mmgb;	// 매매구분 (-1. 매도, +1. 매수)
	double		gdan;	//2012.05.17 KSJ 거래승수 추가함. 값은 거래승수 / 100000
};

struct _mouseLink	{
	int	xpt{};
	int	ypt{};
	CString symb;
	CString data;
};

struct _userMsg{
	int		nSize{};
	int		kind{};
	CString		gab;
};

struct _MouseValue	{
	CString		kpt;
	CString		sValue;
};

// PIBO4013
#define	TRN_HEDGE	"BO410101"
struct _mido{
	char func;	// start "F"
	char mymd[6];	// 만기월
};

#define TRN_JJGA	"BO410102"
struct _jdat{
	char code[8];
	char jjga[6];
};

#define	MAX_R		30
struct  _gmodo {
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
		
struct  _modo {
    char    atmg[8];    /* ATM  */
    char    grec[4];
    struct  _gmodo    gmod[MAX_R];
};
//-----------------------------------