// MapWnd.cpp : implementation file
//

#include "stdafx.h"
#include <math.h>
#include "../../h/axisfire.h"
#include "../../h/axisvar.h"
#include "../../control/fx_misc/misctype.h"
#include "../../control/fx_misc/fxedit.h"
#include "../../control/fx_misc/fxStatic.h"
#include "../../control/fx_grid/griditem.h"
#include "../../control/fx_grid/fxgrid.h"
#include "../../control/fx_misc/fxbutton.h"
#include "../../control/fx_misc/fxImgButton.h"
#include "../../control/fx_misc/fxTab.h"
#include "../../h/ledger.h"
#include "../../h/jmcode.h"
#include "IB410100.h"
#include "message.h"
#include "MapWnd.h"
#include "AccountCtrl.h"
#include "Cfg.h"
#include "OpSensCal.h"
#include "Notify.h"
#include "Price.h"
#include "Sheet1.h"
#include "SonikPage.h"
#include "Stgpage.h"
#include "MyStgpage.h"
#include "Hedgepage.h"
#include "SearchPage.h"
#include "DlgSave.h"
#include "memdc.h"
#include "excel9.h"
#include "ConfigDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define	ARR_UP		"↑"
#define	ARR_DN		"↓"
#define	TRI_UP		"▲"
#define	TRI_DN		"▼"

#define GUARANTEE_RATE	0.05		// 증거금율

#define LEDGER_TR	"PIBOTUXQ"
#define MAP_MULTIORDER	"IB300400"

#define LINE_1		 1
#define	LINE_2		40
#define	LINE_3Grid	65
#define SHEETTAB_TOP	241

#define mapWidth	935
#define	mapHeight	620
#define	CTRL_TOPSPACE	8
#define	CTRL_HEIGHT	20
#define CTRL_COMBOHEIGHT	100
#define STAT_WIDTH	76
#define STAT_HEIGHT	22
#define	GRID_HEIGHT	171
#define GRID_ROWH	19
#define	nameWidth	88
#define	GAP1		1
#define	GAP5		3
#define	btn2W		36
#define	btn4W		60
#define	btn5W		68
#define	btn6W		80
#define	btn7W		92
#define	btn8		104

#define		BACKINDEXCOLOR			64
#define		TOOLINDEXCOLOR			66
#define USES_CONVERSION_CONST const int _convert = 0; (_convert); const UINT _acp = ATL::_AtlGetConversionACP() /*CP_THREAD_ACP*/; (_acp); LPCWSTR _lpw = NULL; (_lpw); LPCSTR _lpa = NULL; (_lpa)


struct	_mdatsym {
	char*		ccur;	// 현재가
        char*		mdga[5];	// 매도호가1~5
        char*		msga[5];	// 매수호가1~5
} mdatsym[] = {
	       {"023", 
		"051", "052", "053", "054", "055", 
		"071", "072", "073", "074", "075"}
};

const int datBasicPrice = 0, datK200 = 1, datFuture = 2, datHV = 3, datCdgl = 4, datBdjs = 5, datEND = 6;

struct	_staticX {
	int	width;
	int	align;
	DWORD	attr;

	int	clrBk;  
	int	clrFo;
	char*	title;
	int	dataIdx;
	//CfxStatic*	statW;
} staticX[] = {
	{ 60,   alCENTER,	0, 			96, 97,	"기초자산",	-1		}, 
	{ 45,   alRIGHT,	0,			68, 69,	"",		datBasicPrice	},
	{ 60,	alCENTER,	0,			96, 97,	"K200지수",	-1		},
	{ 45,   alRIGHT,	attrNUMERIC|attrCORGBx,	68, 69,	"",		datK200		}, 
	{ 60,   alCENTER,	0,			96, 97,	"최근월물",	-1		},
	{ 45,   alRIGHT,	attrNUMERIC|attrCORGBx,	68, 69,	"",		datFuture	},
	{ 50,   alCENTER,	0, 			96, 97,	"HV(90)",	-1		}, 
	{ 45,   alRIGHT,	attrNUMERIC,		68, 69,	"",		datHV		},
	{ 50,   alCENTER,	0, 			96, 97,	"CD금리",	-1		}, 
	{ 40,   alRIGHT,	attrNUMERIC,		68, 69,	"",		datCdgl		},
	{ 60,   alCENTER,	0, 			96, 97,	"배당지수",	-1		}, 
	{ 46,   alRIGHT,	attrNUMERIC,		68, 69,	"",		datBdjs		}
};

struct	_gridHdr {
	char*	text;		// fixed header
	UINT	width;		// fixed width
	UINT	attr;		// fixed attr

	char	symbol[8];	// nonfixed symbol
	UINT	format;		// nonfixed format
	UINT	attrx;		// nonfixed attr
	char*	defaultVal;
} gridHdr[] = {
	{ "종목코드",	0,	GVAT_SORTSTR,	"",		0,		0,	""},			// 0	
	{ "",		20,	GVAT_CHECK,	"",	GVFM_CENTER,	GVAT_CHECK, ""},			// 1	
	{ "종목명",	130,	GVAT_SORTSTR,	"",	GVFM_LEFT,	0,	""},			// 2
	{ "",		19,	0,	"",		GVFM_LEFT,	0,	""},			// 3 종목선택
	{ "구분",	50,	0,	"",		GVFM_CENTER,	GVAT_CONDITIONx,	"매도|매수|"},	// 4
	{ "수량",	60,	0,	"",		GVFM_RIGHT,	GVAT_COMMA,	""},		// 5
	{ "단가",	60,	0,	"",		GVFM_RIGHT,	0,	""},			// 6
	{ "",		19,	0,	"",		GVFM_RIGHT,	0,	""},			// 7 주문가선택
	{ "현재가",	65,	0,	"",		GVFM_RIGHT,	GVAT_CONDITIONx,	""},	// 8
	{ "평가손익",	98,	0,	"",		GVFM_RIGHT,	GVAT_CONDITIONx|GVAT_COMMA,	""},		// 9
	{ "IV",		60,	0,	"",		GVFM_RIGHT,	0,	""},			// 10
	{ "델타",	80,	0,	"",		GVFM_RIGHT,	GVAT_COMMA,	""},		// 11
	{ "감마",	80,	0,	"",		GVFM_RIGHT,	GVAT_COMMA,	""},		// 12	
	{ "쎄타",	80,	0,	"",		GVFM_RIGHT,	GVAT_COMMA,	""},		// 13	
	{ "베가",	80,	0,	"",		GVFM_RIGHT,	GVAT_COMMA,	""}		// 14	
};

const int gCode =0, gSelect = 1, gName = 2, gCodeCtrl = 3, gGB = 4, gJango = 5, gPrc = 6, gPrcCtrl = 7,
	gCurr = 8, gSonik = 9, gIV = 10, gDelta = 11, gGamma = 12, gTheta = 13, gVegga = 14;

/////////////////////////////////////////////////////////////////////////////
// CMapWnd

CMapWnd::CMapWnd(CWnd* parent)
{
	m_pView = parent;

	m_bRts = false;
	m_cfg		= NULL;
	m_opsenscal	= NULL;
	m_pNotify	= NULL;
	m_price		= NULL;
	m_pAccountCbo	= NULL;
	m_pPwdEdit	= NULL;
	m_pMingamBtn	= NULL;
	m_pJangoBtn	= NULL;
	m_pOrdBtn	= NULL;
	m_pSaveStgBtn	= NULL;
	m_pAddBtn	= NULL;
	m_pDelBtn	= NULL;
	m_pDelAllBtn	= NULL;
	m_pExcelBtn	= NULL;
	m_pSaveStgBtn = NULL;

	m_pPFGrid	= NULL;
	m_imageList	= NULL;

	m_pTab1		= NULL;
	m_pSheet1	= NULL;

	m_allSum.ccod	= " 합  계";
	m_allSum.prc	= 0;	// 소요자금
	m_allSum.jango	= 0;	// 옵션매도수량
	m_allSum.gb	= 0;	// 옵션매수수량
	m_allSum.sonik	= 0;
	m_allSum.delta	= 0;
	m_allSum.gamma	= 0;
	m_allSum.theta	= 0;
	m_allSum.vegga	= 0;

	m_bData = std::make_unique<_bdat>();
	m_priceGrid = NULL;
	m_priceRow = -1;
	m_prcMode = pmBuyPrc;
	m_currMode = cmCurr;
	m_static[0] = NULL;

	m_pView = NULL;
	m_pMainWnd = NULL;
	m_cfg = NULL;
	m_opsenscal = NULL;

	m_excelS = _T("");
	m_excelH = NULL;

	m_strGuideMessage = "";

	m_bOnDialog = false;
	m_bAdd = false;
	m_pSearchPage = NULL;
	m_pOrderBtn = NULL;
	
	m_hInst = NULL;
	m_pwndFOMultiSel = NULL;

	m_pConfigBtn = NULL;
	m_pConfigDlg = NULL;
	m_bMulti = true;
}

CMapWnd::~CMapWnd()
{
	if (m_excelH != NULL)
	{
		m_excelS = _T(""); Sleep(1000);
		TerminateThread(m_excelH, 0); Sleep(50);
	}

	struct _mdat *mdat = nullptr;
	for (int ii = 0; ii < m_mData.GetSize(); ii++)
	{
		mdat = m_mData.GetAt(ii);
		mdat->ccod.Empty();
		mdat->iv.Empty();
		delete mdat;
	}
	m_mData.RemoveAll();
	for (int ii = 0; ii < m_PFCodes.GetSize(); ii++)
		delete m_PFCodes.GetAt(ii);
	m_PFCodes.RemoveAll();

//	DELMEM(m_bData);
//	DELMEM(m_opsenscal);
	DELIMG(m_imageList);

	//DELWND(m_cfg);
	//DELWND(m_pNotify);
	//DELWND(m_price);
	//DELWND(m_pAccountCbo);
	//DELWND(m_pPwdEdit);
	//DELWND(m_pJangoBtn);
	//DELWND(m_pOrdBtn);
	//DELWND(m_pSaveStgBtn);
	//DELWND(m_pAddBtn);
	//DELWND(m_pDelBtn);
	//DELWND(m_pDelAllBtn);
	//DELWND(m_pExcelBtn);
	//DELWND(m_pPFGrid);
	//DELWND(m_pSheet1);
	//DELWND(m_pSearchPage);
	//DELWND(m_pOrderBtn);
	//DELWND(m_pConfigBtn);

	if (m_hInst) 
	{
		FreeLibrary(m_hInst);
	}
}

BEGIN_MESSAGE_MAP(CMapWnd, CWnd)
	//{{AFX_MSG_MAP(CMapWnd)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(ID_BTN_MINGAM, OnClickMingam)
	ON_BN_CLICKED(ID_BTN_JANGO, OnClickJango)
	ON_BN_CLICKED(ID_BTN_JANGOCLEAR, OnClickOrder)
	ON_BN_CLICKED(ID_BTN_SAVESTG, OnClickSaveStg)
	ON_BN_CLICKED(ID_BTN_ADDITEM, OnAddItem)
	ON_BN_CLICKED(ID_BTN_DELITEM, OnDelItem)
	ON_BN_CLICKED(ID_BTN_DELALLITEM, OnDelAllItem)
	ON_BN_CLICKED(ID_BTN_EXCEL, OnExcel)
	ON_BN_CLICKED(ID_BTN_SEARCH, OnClickSearch)
	ON_BN_CLICKED(ID_BTN_ORDER, OnClickOrderBtn)
	ON_BN_CLICKED(ID_BTN_CONFIG, OnClickConfigBtn)
	ON_MESSAGE(WM_USER, OnMessage)
	ON_MESSAGE(WM_USER + 1, OnUser)
	ON_MESSAGE(WM_PROPrice, OnMsgPrice)
	ON_NOTIFY(ENM_VKRETURN, ID_EDIT_PASSWORD, OnEditNotify)
	ON_NOTIFY(GVNM_LMOUSEUP, ID_JANGOGRID, OnGridLMouseUp)
	ON_NOTIFY(GVNM_LMOUSEUP, ID_PFGRID, OnGridLMouseUp)
	ON_NOTIFY(GVNM_LMOUSEDOWN, ID_JANGOGRID, OnGridLMouseDown)
	ON_NOTIFY(GVNM_LMOUSEDOWN, ID_PFGRID, OnGridLMouseDown)	
	ON_NOTIFY(GVNM_ENDEDIT, ID_JANGOGRID, OnGridEndEdit)
	ON_NOTIFY(GVNM_ENDTABKEY, ID_JANGOGRID, OnGridEndEdit)
	ON_NOTIFY(GVNM_ENDRETURN, ID_JANGOGRID, OnGridEndEdit)
	ON_NOTIFY(GVNM_ENDEDIT, ID_PFGRID, OnGridEndEdit)
	ON_NOTIFY(GVNM_ENDTABKEY, ID_PFGRID, OnGridEndEdit)
	ON_NOTIFY(GVNM_ENDRETURN, ID_PFGRID, OnGridEndEdit)
	ON_NOTIFY(TCN_SELCHANGE, ID_TAB1, OnTab1Change)
	ON_NOTIFY(GVNM_ENDSORT, ID_JANGOGRID, OnGridEndSort)
	ON_NOTIFY(GVNM_ENDSORT, ID_PFGRID, OnGridEndSort)	
	ON_MESSAGE(WM_MAPLineMove, OnMsgLineMove)
	ON_MESSAGE(WM_PROSave, OnMsgSave)
	ON_MESSAGE(WM_MAPStgApply, OnMsgApplyStrategy)
	ON_MESSAGE(WM_MAPStgAdd, OnMsgAddStrategy)
	ON_MESSAGE(WM_MAPHedgeData, OnMapHedgeData)
END_MESSAGE_MAP()

void CMapWnd::CreateMap(CWnd* pParent)
{
	m_pView = pParent;
	const int nSideGap = 8;
	Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, CRect(0, 0, mapWidth, mapHeight), pParent, 100, NULL);
	if(m_pView)
		m_pView->SendMessage(WM_USER, MAKEWPARAM(sizeDLL, 0), MAKELPARAM(mapWidth, mapHeight));

	Variant(titleCC, "[4101] 전략분석");

	m_pApp = (CIB410100App *)AfxGetApp();
	m_sRoot = Variant(homeCC, "");
	m_Hts = Variant(systemCC, "");
	m_sUser = Variant(userCC, "");

	m_pMainWnd = this;
	m_sRoot = m_sRoot;
	m_sSavePath.Format("%s\\%s\\%s", m_sRoot, USRDIR, Variant(nameCC, ""));
	LoadOJCode();
	
	LOGBRUSH lb;
	lb.lbStyle = BS_NULL;
	m_brNull.CreateBrushIndirect(&lb);
	m_pFont = getAxFont("굴림", 9);
	m_pBFont = getAxFont("굴림", 9, 3);
	SetPalette();
	
	int line = LINE_1, sx=0, ex=0;
	CRect rc, rcClient;
	GetClientRect(&rcClient);

	CString sImageDir = "";
	sImageDir.Format("%s\\image\\", m_sRoot);

	m_bmp2BTN	= *getBitmap(sImageDir + "2BTN.BMP");
	m_bmp2BTN_DN	= *getBitmap(sImageDir + "2BTN_DN.BMP");
	m_bmp2BTN_EN	= *getBitmap(sImageDir + "2BTN_EN.BMP");
	m_bmp4BTN	= *getBitmap(sImageDir + "4BTN.BMP");
	m_bmp4BTN_DN	= *getBitmap(sImageDir + "4BTN_DN.BMP");
	m_bmp4BTN_EN	= *getBitmap(sImageDir + "4BTN_EN.BMP");
	m_bmp5BTN	= *getBitmap(sImageDir + "5BTN.BMP");
	m_bmp5BTN_DN	= *getBitmap(sImageDir + "5BTN_DN.BMP");
	m_bmp5BTN_EN	= *getBitmap(sImageDir + "5BTN_EN.BMP");
	m_bmp6BTN	= *getBitmap(sImageDir + "6BTN.BMP");
	m_bmp6BTN_DN	= *getBitmap(sImageDir + "6BTN_DN.BMP");
	m_bmp6BTN_EN	= *getBitmap(sImageDir + "6BTN_EN.BMP");

	//2012.07.23 KSJ 조회버튼 추가
	m_bmpSearchBTN	= *getBitmap(sImageDir + "조회BTN.BMP");
	m_bmpSearchBTN_DN	= *getBitmap(sImageDir + "조회BTN_DN.BMP");
	m_bmpSearchBTN_EN	= *getBitmap(sImageDir + "조회BTN_EN.BMP");
	//KSJ

	m_cfg = std::make_unique <CCfg>();
	m_cfg->CreateCfg(m_pView, m_sRoot);
//	m_cfg = m_cfg;

	m_opsenscal = std::make_unique <COpSensCal>();
	m_opsenscal->CreateOpSensCal(this, m_sRoot);
	m_opsenscal->SetUserName(Variant(nameCC, ""));
	m_opsenscal->ReadCfg();
//	m_opsenscal = m_opsenscal;

	m_pNotify = std::make_unique <CNotify>();
	m_pNotify->m_pView = m_pView;
	m_pNotify->CreateNotify(this, m_sRoot);

	m_price = std::make_unique <CPrice>();
	m_price->m_sRoot = m_sRoot;
	m_price->m_pParent = m_pView;
	m_price->Create(NULL, NULL, WS_CHILD|WS_VISIBLE, CRect(0, 0, 1, 1), this, -1);
	m_price->createPrice();

	sx = GAP5 + 32; 
	ex = sx + 143;
	rc = CRect(sx, CTRL_TOPSPACE, ex, CTRL_TOPSPACE + CTRL_HEIGHT);
	m_pAccountCbo = std::make_unique <CAccountCtrl>(m_pView, this, m_sRoot);
	m_pAccountCbo->CreateEx(WS_EX_TOPMOST, NULL, "ACCN", WS_VISIBLE|WS_CHILD|WS_TABSTOP, rc, this, 0);
	m_pAccountCbo->createAccountCtrl("AN20", KEY_ACCOUNT, m_bkColor);

	m_strPal = GetPalette();
	m_iGuideHeight = GUIDE_HEIGHT;
	m_iGridHeadrHeight = GRIDHEADER_HEIGHT;
	
	m_hGuideBitmap = getAxBitmap("알림_ic.bmp")->operator HBITMAP();
	m_hRoundBitmap[0] = getAxBitmap("axpanel1_lt.bmp")->operator HBITMAP();
	m_hRoundBitmap[1] = getAxBitmap("axpanel1_rt.bmp")->operator HBITMAP();
	m_hRoundBitmap[2] = getAxBitmap("axpanel1_lb.bmp")->operator HBITMAP();
	m_hRoundBitmap[3] = getAxBitmap("axpanel1_lb.bmp")->operator HBITMAP();
	
	m_guidecolor = m_pView->SendMessage(WM_USER, MAKEWPARAM(getPALETTE, 0), (LPARAM)GUIDCOLOR_INDEX);
	m_guideTxtcolor = m_pView->SendMessage(WM_USER, MAKEWPARAM(getPALETTE, 0), (LPARAM)GUIDTXTCOLOR_INDEX);

//	m_pAccountCbo->CreateAccount(this, rc, m_sRoot);
//	m_AccnName = m_pAccountCbo->GetName();
//	m_pAccount = new CAccountCtrl(m_pView, this, sRoot);
//	m_pAccount->CreateEx(WS_EX_TOPMOST, NULL, "ACCN", WS_VISIBLE|WS_CHILD|WS_TABSTOP, rc, this, 0);
//	m_pAccount->createAccountCtrl("AN10", TR_DLGACCOUNT, clrBk);
	
	sx = ex + 1 + nameWidth + 2; ex = sx + 60;// 계좌명 여백 cej 20070105 비밀번호 7자리까지밖에 입력이 안됨
	rc = CRect(sx, CTRL_TOPSPACE, ex, CTRL_TOPSPACE + CTRL_HEIGHT);
	m_pPwdEdit = std::make_unique <CfxEdit>(m_pFont);
	m_pPwdEdit->Create(WS_CHILD | WS_VISIBLE | ES_PASSWORD, rc, this, ID_EDIT_PASSWORD);

	if(IsNumber(m_sUser))
	{
		m_pPwdEdit->EnableWindow(TRUE); // 직원용 로그인
		m_pPwdEdit->SetBkColor(RGB(255,255,255));
	}
	else
	{
		m_pPwdEdit->EnableWindow(FALSE); // 고객용 로그인
		m_pPwdEdit->SetBkColor(GetAxColor(96));
	}

	ex = ex + 4;
	for (int ii = 0; ii < sizeof(staticX) / sizeof(staticX[0]); ii++)
	{
		sx = ex -1; ex = sx + staticX[ii].width;
		rc = CRect(sx, CTRL_TOPSPACE, ex, CTRL_TOPSPACE + CTRL_HEIGHT);
		m_static[ii] = std::make_unique <CfxStatic>(m_pFont);
		m_static[ii]->Create(staticX[ii].title, WS_CHILD|WS_VISIBLE|SS_LEFT, rc, this);
		m_static[ii]->Init(ssCOLOR | ssBORDER, staticX[ii].attr, "", "", 
			getIndexColor(staticX[ii].clrBk), 0, getIndexColor(staticX[ii].clrFo),
			staticX[ii].align);
		m_static[ii]->SetBorderColor(getIndexColor(75));
	}

	line = LINE_2;
	
	sx = GAP5; 
	ex = sx; 
	ex += 5; 
	const int nStart = ex;

	// 실계좌잔고 Control ******
	sx = nStart; ex = sx + btn4W;
	rc = CRect(sx, line + GAP1, ex, line + GAP1 + CTRL_HEIGHT);
	m_pJangoBtn = std::make_unique <CfxImgButton>(m_pFont);
	m_pJangoBtn->Create("잔고조회", rc, this, ID_BTN_JANGO, false, true);
	m_pJangoBtn->SetImgBitmap(m_bmp4BTN, m_bmp4BTN_DN, m_bmp4BTN_EN);

//	sx = ex + 40; ex = sx + btn4W;
	sx = ex + 40; ex = sx + 60;
	rc = CRect(sx, line + GAP1, ex, line + GAP1 + CTRL_HEIGHT);
	m_pSaveStgBtn = std::make_unique <CfxImgButton>(m_pFont);
	m_pSaveStgBtn->Create("전략저장", rc, this, ID_BTN_SAVESTG);
	m_pSaveStgBtn->SetImgBitmap(m_bmp4BTN, m_bmp4BTN_DN, m_bmp4BTN_EN);

	// 가상 포트폴리오 Control ******
	sx = ex + 40; ex = sx + btn4W;
	rc = CRect(sx, line + GAP1, ex, line + GAP1 + CTRL_HEIGHT);
	m_pAddBtn = std::make_unique <CfxImgButton>(m_pFont);
	m_pAddBtn->Create("종목추가", rc, this, ID_BTN_ADDITEM);
	m_pAddBtn->SetImgBitmap(m_bmp4BTN, m_bmp4BTN_DN, m_bmp4BTN_EN);

	//일단 종목 다이얼로그
	initFOMultiSel(CRect(sx, line + GAP1, 10, 10));

	sx = ex + 10; ex = sx + btn4W;
	rc = CRect(sx, line + GAP1, ex, line + GAP1 + CTRL_HEIGHT);
	m_pDelBtn = std::make_unique < CfxImgButton>(m_pFont);
	m_pDelBtn->Create("종목삭제", rc, this, ID_BTN_DELITEM);
	m_pDelBtn->SetImgBitmap(m_bmp4BTN, m_bmp4BTN_DN, m_bmp4BTN_EN);

	sx = ex + 10; ex = sx + btn4W;
	rc = CRect(sx, line + GAP1, ex, line + GAP1 + CTRL_HEIGHT);
	m_pDelAllBtn = std::make_unique <CfxImgButton>(m_pFont);
	m_pDelAllBtn->Create("전부삭제", rc, this, ID_BTN_DELALLITEM);
	m_pDelAllBtn->SetImgBitmap(m_bmp4BTN, m_bmp4BTN_DN, m_bmp4BTN_EN);

	sx = rcClient.right - btn4W*3 - btn2W*3 + 10; ex = sx + btn2W;
	//sx = rcClient.right - btn4W*3 + 8; ex = sx + btn2W;
	rc = CRect(sx, line + GAP1, ex, line + GAP1 + CTRL_HEIGHT);
	m_pOrderBtn = std::make_unique < CfxImgButton>(m_pFont);
	m_pOrderBtn->Create("주문", rc, this, ID_BTN_ORDER);
	m_pOrderBtn->SetImgBitmap(m_bmp2BTN, m_bmp2BTN_DN, m_bmp2BTN_EN);

	m_pOrderBtn->ShowWindow(SW_SHOW);

	sx = rcClient.right - btn4W*3 - btn2W*2 + 14; ex = sx + btn4W;
	rc = CRect(sx, line + GAP1, ex, line + GAP1 + CTRL_HEIGHT);
	m_pOrdBtn = std::make_unique <CfxImgButton>(m_pFont);
	m_pOrdBtn->Create("일괄주문", rc, this, ID_BTN_JANGOCLEAR);
	m_pOrdBtn->SetImgBitmap(m_bmp4BTN, m_bmp4BTN_DN, m_bmp4BTN_EN);
	m_pOrdBtn->ShowWindow(SW_SHOW);

	sx = rcClient.right - btn4W*3 + 6; 
	ex = sx + btn4W;
	rc = CRect(sx, line + GAP1, ex, line + GAP1 + CTRL_HEIGHT);
	m_pExcelBtn = std::make_unique < CfxImgButton>(m_pFont);
	m_pExcelBtn->Create("EXCEL", rc, this, ID_BTN_EXCEL);
	m_pExcelBtn->SetImgBitmap(m_bmp4BTN, m_bmp4BTN_DN, m_bmp4BTN_EN);

	sx = rcClient.right - btn4W*2 + 10; 
	ex = sx + btn4W;
	rc = CRect(sx, line + GAP1, ex, line + GAP1 + CTRL_HEIGHT);
	m_pMingamBtn = std::make_unique < CfxImgButton>(m_pFont);
	m_pMingamBtn->Create("민감설정", rc, this, ID_BTN_MINGAM);
	m_pMingamBtn->SetImgBitmap(m_bmp4BTN, m_bmp4BTN_DN, m_bmp4BTN_EN);

	sx = rcClient.right - btn2W - 10 ; 
	ex = sx + btn2W;
	rc = CRect(sx, line + GAP1, ex, line + GAP1 + CTRL_HEIGHT);
	m_pConfigBtn = std::make_unique < CfxImgButton>(m_pFont);
	m_pConfigBtn->Create("설정", rc, this, ID_BTN_CONFIG);
	m_pConfigBtn->SetImgBitmap(m_bmp2BTN, m_bmp2BTN_DN, m_bmp2BTN_EN);

	m_imageList = new CImageList();
	m_imageList->Create(18, 18, ILC_COLORDDB | ILC_MASK, 1, 1);
	addImageList("검색.BMP");
	addImageList("가격.BMP");

	line = LINE_3Grid;
	sx = nSideGap; 
	ex = rcClient.right - nSideGap; 
	rc = CRect(sx, line, ex, line + GRID_HEIGHT);

	m_pPFGrid = std::make_unique<CfxGrid>();
	m_pPFGrid->Create(rc, this, ID_PFGRID, GVSC_VERT, GVDD_FULL, WS_CHILD|WS_TABSTOP|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS);
	m_pPFGrid->SetFont(m_pFont);
	m_pPFGrid->SetImageList(m_imageList);
	initGrid(m_pPFGrid.get());

	SetGridMode();
	m_pAccountCbo->ShowWindow(TRUE);

	m_pPFGrid->GetWindowRect(rc);
	ScreenToClient(rc);

	rc = CRect(3, rc.bottom+35, rcClient.right - GAP1 - GAP1, rc.bottom+35 + TAB_HEIGHT);
	m_pTab1 = std::make_unique <CfxTab>();
	m_pTab1->Create(rc, this, ID_TAB1);
	m_pTab1->LoadTabImage(m_sRoot, false);
	m_pTab1->SetColor(m_pApp->GetIndexColor(m_pView, 70), m_pApp->GetIndexColor(m_pView, 3), m_pApp->GetIndexColor(m_pView, 70), m_pApp->GetIndexColor(m_pView, 181), m_pApp->GetIndexColor(m_pView, 181));
	m_pTab1->SetBorderColor(m_pApp->GetIndexColor(m_pView, 44)); 

	m_pTab1->InsertItem(0, "만기 손익");
	m_pTab1->InsertItem(1, "전략 선택");
	m_pTab1->InsertItem(2, "나의 전략");
	m_pTab1->InsertItem(3, "헤지(단일종목)");
	m_pTab1->InsertItem(4, "증거금조회");		//2012.07.16 KSJ 증거금조회 추가
	m_pTab1->SetStyle(tsIMAGE, true);

	rc = CRect(nSideGap, rc.bottom, rcClient.right- 8, rcClient.bottom - 8 - GUIDEBAR_HEIGHT);
	m_pSheet1 = std::make_unique <CSheet1>("", this, 0, m_pView);
	m_pSheet1->SetWizardMode();
	if (m_pSheet1->Create(this, WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN))
	{
		m_pSheet1->MoveWindow(&rc);
		hideWizardBtn();
	}

	m_pSearchPage = new CSearchPage(m_pView, m_pView);
	m_pSearchPage->Create(NULL, NULL, WS_CHILD, rc, this, IDC_MAPVIEW);
	m_pSearchPage->ChangeMap("IB410110", "");
	m_pSearchPage->ShowWindow(SW_HIDE);

	// 2012.10.08 KSJ 멀티종목
	CString sPath;
	sPath.Format("%s\\%s\\%s\\%s", m_sRoot, USRDIR, Variant(nameCC, ""), FILE3101);

	m_pConfigDlg = std::make_unique<class CConfigDlg>();
	m_pConfigDlg->SetPath(sPath);
	m_bMulti = m_pConfigDlg->GetConfig();
	m_pConfigDlg = nullptr;
	// KSJ

// 	rc = CRect(rc.left , rc.top+20 , rc.right, rc.bottom);
// 
// 	m_pSearchPage->MoveWindow(rc,TRUE);

	//2012.07.23 조회버튼 추가
// 	rc = CRect(rc.left +870 , rc.top+10 , rc.left + 907, rc.top + 30);
// 	m_pSearchBtn = new CfxImgButton(m_pFont);
// 	m_pSearchBtn->Create("조회", rc, this, ID_BTN_SEARCH);
// 	m_pSearchBtn->SetImgBitmap(m_bmpSearchBTN, m_bmpSearchBTN_DN, m_bmpSearchBTN_EN);
// 	m_pSearchBtn->ShowWindow(false);
	//KSJ
	queryInitValue();
	queryMingam();


	if (m_pAccountCbo && m_pPwdEdit)
	{
		CString	pass = m_pAccountCbo->GetPswd();
		if (!pass.IsEmpty()) 
			m_pPwdEdit->SetWindowText(pass);
	}
}

CBitmap* CMapWnd::getAxBitmap(CString path)
{
	return (CBitmap*)m_pMainWnd->SendMessage(WM_USER, getBITMAP, (long)path.operator LPCTSTR());
}

void CMapWnd::OnDestroy() 
{
	m_bRts = false;
	clearOPCodeList();
	if (m_pPFGrid)		
		deleteGrid(m_pPFGrid.get(), 0, true);


	//DELWND(m_pTab1);

	//if (m_static[0] != NULL)
	//{
	//	for (int ii = 0; ii < sizeof(staticX) / sizeof(staticX[0]); ii++)
	//	{
	//		DELWND(m_static[ii]);
	//	}
	//}

	//DELWND(m_pMingamBtn);

	CWnd::OnDestroy();	
}

void CMapWnd::PostNcDestroy() 
{
}

CString CMapWnd::Variant(int comm, CString data)
{
	CString retvalue;
	if (comm == guideCC)
	{
		KillTimer(1);
		SetTimer(1, 2000, NULL);
		return "";
	}
	char* dta = (char*)m_pView->SendMessage(WM_USER, MAKEWPARAM(variantDLL, comm), (LPARAM)(LPCTSTR)data);

	if ((long)dta > 1)
		retvalue = dta;
	else
		return "";

	return retvalue;
}

CBitmap* CMapWnd::getBitmap(CString path)
{
	return (CBitmap*)m_pView->SendMessage(WM_USER, getBITMAP, (long)path.operator LPCTSTR());
}

COLORREF CMapWnd::getIndexColor(int index)
{
	if (index & 0x02000000)
		return index;
	return m_pView->SendMessage(WM_USER, MAKEWORD(getPALETTE, 0), (LPARAM)index);
}

CBrush* CMapWnd::getAxBrush(COLORREF clr)
{
	return (CBrush*)m_pView->SendMessage(WM_USER, getBRUSH, (long)clr);
}

CFont* CMapWnd::getAxFont(CString fName, int point, int style)
{
	struct _fontR fontR;

	fontR.name = (LPSTR)fName.operator LPCTSTR();
	fontR.point = point; fontR.italic = false; fontR.bold = 0;

	switch (style)
	{
	case 0: case 1: // none
		break;
	case 2: // italic
		fontR.italic = true;
		break;
	case 3: // bold
		fontR.bold = FW_BOLD;
		break;
	case 4: // both
		fontR.italic = true;
		fontR.bold = FW_BOLD;
		break;
	}
	return (CFont*)m_pView->SendMessage(WM_USER, getFONT, (long)&fontR);
}

// 직원용 인지 아닌지를 판단하고자 하는 함수
BOOL CMapWnd::IsNumber(CString str)
{
    BOOL bRet = TRUE;
	const int nLen = str.GetLength();
    for( int i=0 ; i < nLen ; ++i)
    {
        if( isdigit(str.GetAt(i)) == FALSE)
        {
            bRet = FALSE;
            break;
        }
    }
    return bRet;
}

void CMapWnd::sendTR(CString trCode, char* datB, int datL, int nKey, int nStat)
{
	std::unique_ptr<char[]> sendB = std::make_unique<char[]>(L_userTH + datL + 1);

	struct	_userTH* uTH;
	uTH = (struct _userTH *) sendB.get();

	strcpy(uTH->trc, trCode);
	uTH->key = nKey;
	uTH->stat = nStat;

	CopyMemory(&sendB.get()[L_userTH], datB, datL);
	m_pView->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, datL), (LPARAM) sendB.get());

	sendB = nullptr;
}

void CMapWnd::sendLedgerTR(char cJobCd, CString strTR, char* strData, int nSize, int nKey, int nStat, CString screen)
{
/*
	CString sTR(LEDGER_TR);
	char*	cDataBuffer = new char [L_ledgerH + L_userTH + nSize + 1];
	struct	_userTH* userTH;
	int	ledgerL;

	userTH = (struct _userTH *)cDataBuffer;
	strcpy(userTH->trc, sTR);
	userTH->key = nKey;
	userTH->stat = nStat;

	struct _ledgerH *ledger;
	ledger = (struct _ledgerH *)&cDataBuffer[L_userTH];
	FillMemory(ledger, L_ledgerH, ' ');
	m_pView->SendMessage(WM_USER, MAKEWPARAM(ledgerDLL, NULL), (LPARAM)ledger);
	CopyMemory(ledger->gubn, (char*)&cJobCd, 1);
	CopyMemory(ledger->svcd, strTR, 8);  
	CopyMemory(ledger->dt_svnm, strTR, strTR.GetLength());  
	CopyMemory(ledger->screen, screen,	sizeof(ledger->screen));

	CopyMemory(&cDataBuffer[L_userTH + ledgerL], strData, nSize);
	m_pView->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, L_ledgerH + nSize), (LPARAM) cDataBuffer);

	delete [] cDataBuffer;
*/
}

CString CMapWnd::Parser(CString &srcstr, CString substr)
{
	if (srcstr.Find(substr) == -1)
	{
		CString temp = srcstr;
		srcstr.Empty();
			return temp;
	}
	else
	{
		CString  temp = srcstr.Left(srcstr.Find(substr));
		srcstr = srcstr.Mid(srcstr.Find(substr) + substr.GetLength());
		return temp;
	}
	return "";
}

void CMapWnd::OpenPriceCtrl(char* data)
{
	struct _jdat *jjga;
	jjga = (struct _jdat*)data;
	
	m_PriceData.Format("%s%-8.2f",m_PriceData,atof(CString(jjga->jjga, sizeof(jjga->jjga)))/100);

// 	OutputDebugString("[KSJ] m_PriceData[" + m_PriceData + "]");

	m_price->SetValues(m_PriceData);
	m_price->SetWindowPos(NULL, m_PriceRect.right, m_PriceRect.top, 0, 0, SWP_NOZORDER|SWP_NOSIZE);
	m_price->BtnClick();
}

long CMapWnd::OnMessage(WPARAM wParam, LPARAM lParam)
{
	CString str, strTemp, strMsg;

	switch (LOBYTE(LOWORD(wParam)))
	{
	case DLL_OUB:
		switch (HIBYTE(LOWORD(wParam)))
		{
		case KEY_ACCOUNT:	
		case KEY_ACCOUNT-1:
			m_pAccountCbo->m_pAccountCtrl->SendMessage(WM_USER, wParam, lParam);
			break;
		case KEY_INITVALUE:
			parseInitValue((char*)lParam);
			break;
		case KEY_UPJONG:
			parseUpJong((char*)lParam);
			break;
		case KEY_CODEMINGAM:
			if (IsWindow(m_pSheet1->m_pSonikPage->GetSafeHwnd()))
				(m_pSheet1->m_pSonikPage)->SendMessage(WM_POSInitData, (long)m_bData.get(), 0);
			m_csSort.Lock();
			parseMingam((char*)lParam);
			m_csSort.Unlock();

			if(m_bAdd)
			{
				m_bAdd = false;
				SetGraph();
			}

			break;
		case KEY_CODEMINGAM2:
			if (m_pSheet1->m_pMyStgPage->GetSafeHwnd())
				m_pSheet1->m_pMyStgPage->SendMessage(WM_USER, wParam, lParam);
			break;
		case KEY_HEDGEOPTION:
			if (IsWindow(m_pSheet1->m_pHedgePage->GetSafeHwnd()))
				(m_pSheet1->m_pHedgePage)->SendMessage(WM_SP22Data, wParam, lParam);
			break;
		case KEY_DEPOSIT:
			if (IsWindow(m_pSheet1->m_pSonikPage->GetSafeHwnd()))
				(m_pSheet1->m_pSonikPage)->SendMessage(WM_USER, wParam, lParam);
			break;
		case KEY_DEPGUESS:
			if (IsWindow(m_pSheet1->m_pSonikPage->GetSafeHwnd()))
				(m_pSheet1->m_pSonikPage)->SendMessage(WM_USER, wParam, lParam);
			break;
		case KEY_JJGA:
			OpenPriceCtrl((char*)lParam);
		default:
			break;
		}
		break;
// 	case DLL_ALERT:
// 		m_csSort.Lock();
// 		parseAlert((char*)lParam);
// 		m_csSort.Unlock();
// 		break;
	case DLL_ALERTx:
		struct _alertR* alertR;
		alertR = (struct _alertR*)lParam;
		m_csSort.Lock();
		parseAlert(alertR);
		m_csSort.Unlock();
		break;
	case DLL_SETPAL:
		m_strPal = GetPalette();
		ChangeColors();
		break;
	case DLL_GUIDE:
		Variant(guideCC, (char*)lParam);
		return true;
		break;
	case DLL_TRIGGER:
// 		strTemp.Format("[KSJ] lParam[%s]", (char*)lParam);
// 		OutputDebugString(strTemp);

		strTemp.Format("%s", (char*)lParam);

		if(strTemp.Mid(0, 7) == "4101SND")
		{
// 			OutputDebugString("[KSJ]4101SND");
			OnClickSearch();
		}
		else if(strTemp.Mid(0, 7) == "4101MSG")
		{
			strMsg = strTemp.Mid(8);
			setMessage(strMsg.GetBuffer(strMsg.GetLength()));
			strMsg.ReleaseBuffer();
		}

	}
	return 0;
}

void CMapWnd::setMessage(char* format, ...)
{
	CString tmpS;
	va_list valist;
	va_start(valist, format);
	tmpS.FormatV(format, valist);
	va_end(valist);
	
	m_strGuideMessage = tmpS;
}

long CMapWnd::OnUser(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case MSG_ACCOUNTCHANGE:
		{
			CString val, sPwd;
			val.Format("%s", (char*) lParam);
			Parser(val, "\t");		// 계좌
			m_AccnName = Parser(val, "\t");	// 이름
			sPwd = Parser(val, "\t");	// pswd

			m_pPwdEdit->SetWindowText(sPwd);
			m_pPwdEdit->SetFocus();
		
			Invalidate();

			if (sPwd.IsEmpty())
				break;

			if (IsWindow(m_pSheet1->m_pSonikPage->GetSafeHwnd()))
			{
				char k200[7];
				memset(&k200, ' ', sizeof(k200));
				const BOOL bDeposit = (BOOL)(m_pSheet1->m_pSonikPage)->SendMessage(WM_POSDEPOSITSEL, 0, (LPARAM)k200);
				if (bDeposit)
				{
					CString k200S = CString(k200);
					k200S.TrimRight();
					queryDeposit(FALSE, k200S);
				}
			}
		}
		break;
	case MSG_NOTIFY: // 잔고 조회 및 변화
		{
			CString dat;
			dat = m_pNotify->GetDataList();
// 			OutputDebugString("[KSJ][MSG_NOTIFY]" + dat);
			if (m_pNotify->GetFlag() == "A")
			{
				setJango(dat);
				if (m_PFCodes.GetSize() != 0)
					queryMingam();
				setMessage("[0000] 처리가 완료되었습니다.");
			}
			else
			{
				// 직원인 경우 비밀번호 입력 조회, 고객인 경우 자동 조회
				if(IsNumber(m_sUser))
					setMessage("[%4s] 계좌비밀번호가 잘못 입력되었습니다.", m_pNotify->GetFlag());
				else
					setMessage("[0000] 처리가 완료되었습니다.");
			}
			Invalidate();
		}
		break;
	case MSG_DEPOSIT:
		queryDeposit(BOOL(HIWORD(wParam)), (char *)lParam);
		break;
	}
	return 0;
}

void CMapWnd::OnPaint() 
{
	CPaintDC dc(this); 
	CMemDC	memDC(&dc);

	CRect rc, rcWnd, trc, rcGuideBar;
	const int nRoundGap = 3;
	GetClientRect(&rc);
	rcWnd = rc;

	memDC.FillSolidRect(&rc, m_bkColor);
	trc = rc;
	trc.top = nRoundGap;
	trc.left = nRoundGap;
	trc.right = rc.right - nRoundGap;
	trc.bottom = trc.top + 28;
	memDC.FillSolidRect(&trc, m_clrGradient1);
	DrawRoundBitmap(memDC, trc, ROUND_PANEL);

	m_pJangoBtn->GetWindowRect(trc);
	ScreenToClient(trc);
	m_pPFGrid->GetWindowRect(rc);
	ScreenToClient(rc);
	trc.bottom = rc.bottom + 20;
	trc.right  = rc.right;
	trc.InflateRect(5, 5);
	memDC.FillSolidRect(&trc, GetAxColor(181));
	DrawRoundBitmap(memDC, trc, ROUND_PANEL);
	
	m_pPFGrid->GetWindowRect(trc);
	ScreenToClient(trc);
	rc = CRect(3, trc.bottom+32, trc.right+6, trc.bottom+35- GUIDEBAR_HEIGHT);
	memDC.FillSolidRect(&rc, getIndexColor(181));

	DrawExtraText(&memDC);
	
	rcGuideBar = rc;
	rcGuideBar.top = rcWnd.bottom - 1 - GUIDE_HEIGHT;
	rcGuideBar.bottom = rcWnd.bottom - 1;
	DrawGuideBar(memDC, rcGuideBar);

	if(m_pTab1->GetSafeHwnd())
		SetRoundWnd(memDC, m_pTab1->GetCurSel());

// 	if(m_pTab1->GetCurSel() == 4)
// 	{
// 		m_pSearchBtn->BringWindowToTop();
// 		m_pSearchBtn->SetFocus();
// 	}
}

void CMapWnd::DrawGuideBar(CDC* pDC, CRect rc)
{
	CRect rcGuidBar;
	
	CBrush brush(m_guidecolor);
	CBrush* pOldBrush = pDC->SelectObject(&brush); 
	CPen pen(PS_SOLID, 1, m_panelcolor[0] );
	CPen* pOldPen = pDC->SelectObject(&pen); 
	
	CFont* pOldFont; 
	pOldFont = pDC->SelectObject(m_pFont); 
	
	rcGuidBar = CRect(5,599,500,615);
	pDC->SetTextColor( m_guideTxtcolor);
	pDC->FillSolidRect(rc, m_guidecolor); 
	DrawRoundBitmap(pDC, rc, ROUND_PANEL);
	
	CRect alarmr = rcGuidBar;
	alarmr.left  = rcGuidBar.left + 5;
	alarmr.top  = rcGuidBar.top + 4;
	alarmr.right = alarmr.left + 14;
	alarmr.bottom = alarmr.top + 14;
	
	rcGuidBar.top = alarmr.top ;
	rcGuidBar.bottom = alarmr.bottom ;
	rcGuidBar.left = alarmr.right ;
	rcGuidBar.DeflateRect(3, 1, 1, 0);
	pDC->DrawText(m_strGuideMessage, rcGuidBar, DT_SINGLELINE|DT_LEFT|DT_VCENTER); 
	DrawBitmap(pDC, alarmr, "알림_IC.BMP" );
	
	pDC->SelectObject(pOldFont); 	
	pDC->SelectObject(pOldPen); 
	pDC->SelectObject(pOldBrush); 
	
	DeleteObject(pOldPen);
	DeleteObject(pOldBrush);
}

void CMapWnd::DrawGradientRect(CDC *pDC, CRect rc)
{
	CRect rcx;
	int rs{}, gs{}, bs{}, re{}, ge{}, be{};
	float rStep{}, gStep{}, bStep{};
	CBrush brush;

	rs = GetRValue(m_clrGradient1); gs = GetGValue(m_clrGradient1); bs = GetBValue(m_clrGradient1);
	re = GetRValue(m_clrGradient2); ge = GetGValue(m_clrGradient2); be = GetBValue(m_clrGradient2);

	rStep = float(rs - re)/float(rc.Height());
	gStep = float(gs - ge)/float(rc.Height());
	bStep = float(bs - be)/float(rc.Height());

	for (int ii = 0; ii < rc.Height(); ii++)
	{
		rcx.SetRect(rc.left, rc.top + ii, rc.right, rc.top + ii + 1);

		brush.CreateSolidBrush(RGB(rs - rStep*ii, gs - gStep*ii, bs - bStep*ii));
		pDC->FillRect(&rcx, &brush); brush.DeleteObject();
	}
}

void CMapWnd::DrawExtraText(CDC *pDC)
{
	CRect itemRC;
	CFont *pOldFont = nullptr;
	CBrush br(getIndexColor(65));
	const int nRoundGap = 8;

	pDC->SetBkMode(TRANSPARENT);
	pOldFont = (CFont *)pDC->SelectObject(m_pFont);

	m_pAccountCbo->GetWindowRect(&itemRC); ScreenToClient(&itemRC);
	itemRC.right = itemRC.left; 
	itemRC.left = nRoundGap;//itemRC.left - 30;
	pDC->DrawText("계좌", -1, itemRC, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

	m_pAccountCbo->GetWindowRect(&itemRC); ScreenToClient(&itemRC);
	itemRC.left = itemRC.right + 1; 
	itemRC.right = itemRC.left + nameWidth;
	pDC->FillSolidRect(itemRC, getIndexColor(96));
	pDC->FrameRect(itemRC, &br);
	itemRC.DeflateRect(2, 2);
	pDC->DrawText(m_AccnName, itemRC, DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	// 전체 합계
	m_pPFGrid->GetWindowRect(&itemRC); ScreenToClient(&itemRC);
	itemRC.top = itemRC.bottom -1; itemRC.bottom = itemRC.top + 20;
	DrawSum(&m_allSum, itemRC, pDC, getIndexColor(74));
	pDC->SelectObject(pOldFont);
}

void CMapWnd::DrawSum(struct _gdat *sumdat, CRect rc, CDC *pDC, COLORREF bkClr)
{
	CString val;
	CRect rcCell(rc), rcText;
	CBrush br(getIndexColor(65));
	pDC->FillSolidRect(rc, bkClr);
	pDC->FrameRect(rcCell, &br);

	val = sumdat->ccod;
	rcCell.right = rcCell.left + gridHdr[0].width + gridHdr[1].width + gridHdr[2].width;
	pDC->FrameRect(rcCell, &br);
	rcText = rcCell; rcText.DeflateRect(3, 2);
	pDC->SetTextColor(RGB(255, 255, 255)); // 김진아 대리 요청(090715) : 검은색=>흰색
	pDC->DrawText(val, -1, rcText, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX);
	
	val.Format(" 옵션 매도 : %4d        옵션 매수 : %4d", sumdat->jango, sumdat->gb);
	rcCell.left = rcCell.right -1;
	rcCell.right = rcCell.left + gridHdr[3].width + gridHdr[4].width + gridHdr[5].width 
		+ gridHdr[6].width + gridHdr[7].width + gridHdr[8].width + 1;
	pDC->FrameRect(rcCell, &br);
	rcText = rcCell; rcText.DeflateRect(3, 2);
	pDC->DrawText(val, -1, rcText, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX);

	val.Format("%.0f", sumdat->sonik);
	val.Format("%10s", m_pApp->CommaModify(val));
	rcCell.left = rcCell.right -1;
	rcCell.right = rcCell.left + gridHdr[9].width + 1;
	pDC->FrameRect(rcCell, &br);
	rcText = rcCell; rcText.DeflateRect(3, 2);
	pDC->DrawText(val, -1, rcText, DT_RIGHT | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX);

	// Space
	rcCell.left = rcCell.right -1;
	rcCell.right = rcCell.left + gridHdr[10].width + 1;
	pDC->FrameRect(rcCell, &br);

	val.Format("%.4f", sumdat->delta);
	rcCell.left = rcCell.right -1;
	rcCell.right = rcCell.left + gridHdr[11].width + 1;
	pDC->FrameRect(rcCell, &br);
	rcText = rcCell; rcText.DeflateRect(3, 2);
	pDC->DrawText(val, -1, rcText, DT_RIGHT | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX);

	val.Format("%.4f", sumdat->gamma);
	rcCell.left = rcCell.right -1;
	rcCell.right = rcCell.left + gridHdr[12].width + 1;
	pDC->FrameRect(rcCell, &br);
	rcText = rcCell; rcText.DeflateRect(3, 2);
	pDC->DrawText(val, -1, rcText, DT_RIGHT | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX);

	val.Format("%.4f", sumdat->theta);
	rcCell.left = rcCell.right -1;
	rcCell.right = rcCell.left + gridHdr[13].width + 1;
	pDC->FrameRect(rcCell, &br);
	rcText = rcCell; rcText.DeflateRect(3, 2);
	pDC->DrawText(val, -1, rcText, DT_RIGHT | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX);

	val.Format("%.4f", sumdat->vegga);
	rcCell.left = rcCell.right -1;
	rcCell.right = rcCell.left + gridHdr[14].width + 1;
	pDC->FrameRect(rcCell, &br);
	rcText = rcCell; rcText.DeflateRect(3, 2);
	pDC->DrawText(val, -1, rcText, DT_RIGHT | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX);
}

void CMapWnd::SetPalette()
{
	m_clrGradient1	= getIndexColor(66);
	m_clrGradient2	= getIndexColor(64);
	m_bkColor	= getIndexColor(64);
	m_fColor	= getIndexColor(69);
}

void CMapWnd::initGrid(CfxGrid* grid)
{
	const int count = sizeof(gridHdr) / sizeof(_gridHdr);
	grid->Initial(12, count, 1, 0);


	LOGFONT lf; m_pFont->GetLogFont(&lf); lf.lfHeight = 90;
	GVITEM	gvitem;
	CopyMemory(&gvitem.font, &lf, sizeof(LOGFONT));

 	// column header setting
	for (int ii = 0; ii < count; ii++)
	{
		gvitem.row   = 0;
		gvitem.col   = ii;
		gvitem.state = 0;
		gvitem.mask  = GVMK_TEXT|GVMK_FORMAT|GVMK_FONT|GVMK_ATTR|GVMK_FGCOLOR|GVMK_BKCOLOR|GVMK_MISC;
		gvitem.format= GVFM_HEADER;
		gvitem.fgcol = getIndexColor(76);
//		if (ii == gPrc || ii == gCurr)
//			gvitem.bkcol = getIndexColor(75);
//		else
			gvitem.bkcol = getIndexColor(74);

		gvitem.text  = gridHdr[ii].text;
		gvitem.attr  = gridHdr[ii].attr;
		grid->SetItem(&gvitem);
	};

	for (int ii = 0; ii < count; ii++)
		grid->SetColumnWidth(ii, gridHdr[ii].width);

	for (int ii = 0; ii < count; ii++)
	{
		gvitem.state = 0;
		gvitem.mask  = GVMK_FORMAT|GVMK_FONT|GVMK_ATTR|GVMK_FGCOLOR|GVMK_BKCOLOR|GVMK_SYMBOL;

		gvitem.format= gridHdr[ii].format;
		gvitem.symbol= gridHdr[ii].symbol; 
		gvitem.attr  = gridHdr[ii].attrx;
		gvitem.fgcol = getIndexColor(69); 
		gvitem.bkcol = getIndexColor(68);

		grid->SetColumnItems(ii, &gvitem);
	}

	for (int ii = 0; ii < grid->GetRowCount(); ii++)
		grid->SetRowHeight(ii, GRID_ROWH);

	grid->SetFocusCellSensitive(false);
	grid->SetStepColor(1, getIndexColor(68), getIndexColor(77));

	grid->SetGridLine(GVLN_BOTH);
	grid->SetGridScroll(GVSC_VERT);
	grid->SetRowColResize(FALSE, FALSE);
	grid->SetGridFocusLine(FALSE, TRUE);
	grid->SetGridColor(getIndexColor(65));
	grid->SetBKSelColor(getIndexColor(78));
	grid->Adjust();
}

void CMapWnd::initGridRow(CfxGrid *grid, int row)
{
	LOGFONT lf; m_pFont->GetLogFont(&lf); lf.lfHeight = 90;
	GVITEM	gvitem;

	gvitem.row   = row;
	gvitem.mask  = /*GVMK_TEXT|*/GVMK_FORMAT|GVMK_FONT|GVMK_FGCOLOR|GVMK_BKCOLOR|GVMK_MISC;
	gvitem.mask |= GVMK_ATTR;
	gvitem.font  = lf;
	gvitem.fgcol = getIndexColor(69);
	gvitem.bkcol = getIndexColor(68);
	CopyMemory(&gvitem.font, &lf, sizeof(LOGFONT));

	for (int ii = 0; ii < grid->GetColumnCount(); ii++)
	{
		if ((grid == m_pPFGrid.get() && ii == gCodeCtrl) || ii == gPrcCtrl)
		{
			gvitem.mask |= GVMK_IMAGE;
			if (ii == gCodeCtrl)
				gvitem.image = 0;
			else
				gvitem.image = 1;
		}
		else
		{
			gvitem.mask &= ~GVMK_IMAGE;
		}
		gvitem.state = 0;
		gvitem.col    = ii;
		gvitem.format= gridHdr[ii].format;
		if (grid == m_pPFGrid.get() && ii == gGB)
		{
			CString temp;
			temp = gridHdr[ii].defaultVal;
			gvitem.param = 1;
			gvitem.misc = temp;
			
			for (int jj = 0; jj <= gvitem.param; jj++) 
				gvitem.text = m_pApp->Parser(temp, "|");
		}
		else
		{
			gvitem.misc = "";
			gvitem.text = "";
		}

		if ((grid == m_pPFGrid.get() && (ii == gJango)) || ii == gPrc)
			gvitem.attr  = gridHdr[ii].attrx | GVAT_EDIT;
		else
			gvitem.attr  = gridHdr[ii].attrx;

		grid->SetItem(&gvitem);
	}
	grid->SetRowHeight(row, GRID_ROWH);
	if (grid == m_pPFGrid.get())
		grid->SetItemImage(row, gCodeCtrl, 0);
	grid->SetItemImage(row, gPrcCtrl, 1);
}

void CMapWnd::SetGridMode()
{
	m_pPFGrid->ShowWindow(SW_SHOW);
	m_pOrdBtn->SetWindowText("일괄주문");
	m_pOrdBtn->Invalidate();
	generateSum();
}

void CMapWnd::deleteGrid(class CfxGrid* pGrid, int idx, bool removeAll)
{
	CString code;
	struct _gdat *gdat=nullptr;
	CArray<struct _gdat*, struct _gdat*>* pCodes;
	pCodes = &m_PFCodes;

	if (removeAll)
	{
		for (int ii = pCodes->GetSize() -1; ii >= 0; ii--)
		{
			code = pGrid->GetItemText(ii+1, gCode);
			if (code.IsEmpty()) break;

			deleteMData(code);

			gdat = pCodes->GetAt(ii);
			pCodes->RemoveAt(ii);
			delete gdat;	gdat = NULL;

			m_pPFGrid->DeleteRow(ii+1);
		}

		initGrid(m_pPFGrid.get());
	}
	else
	{
		code = pGrid->GetItemText(idx, gCode);
		if (!code.IsEmpty())
		{
			deleteMData(code);

			gdat = pCodes->GetAt(idx-1);
			pCodes->RemoveAt(idx-1);
			delete gdat;	gdat = NULL;

			pGrid->DeleteRow(idx);
		}

		const int cnt = m_pPFGrid->GetRowCount();
		m_pPFGrid->SetRowCount((cnt+1 < 10) ? 10 : cnt+1);
		for (int ii = 0; ii < cnt; ii ++)
			initGridRow(m_pPFGrid.get(), ii+1);
	}


}

int CMapWnd::findGrid(class CfxGrid* pGrid, CString code)
{
	CString val;
	int idx = -1;
	if (code.IsEmpty()) return idx;

	for (int ii = 1; ii < pGrid->GetRowCount(); ii++)
	{
		val.Format("%s", pGrid->GetItemText(ii, gCode));
		if (!val.IsEmpty() && val.CompareNoCase(code) == 0)
		{
			idx = ii;
			break;
		}
	}
	return idx;
}

int CMapWnd::findMData(CString code)
{
	int idx = -1;
	struct _mdat* mDat = nullptr;
	for (int ii = 0; ii < m_mData.GetSize(); ii++)
	{
		mDat = (struct _mdat*)m_mData.GetAt(ii);
		if (code.CompareNoCase(mDat->ccod) == 0)
		{
			idx = ii;
			break;
		}
	}
	return idx;
}

int CMapWnd::findGData(CArray<struct _gdat*, struct _gdat*> *arr, CString code)
{
	int idx = -1;
	struct _gdat* gdat=nullptr;
	for (int ii = 0; ii < arr->GetSize(); ii++)
	{
		gdat = arr->GetAt(ii);
		if (gdat) {
			if (code.CompareNoCase(gdat->ccod) == 0)
			{
				idx = ii;
				break;
			}
		}
	}
	return idx;
}

void CMapWnd::deleteMData(CString code)
{
	for (int ii = m_mData.GetSize() -1; ii >= 0; ii--)
	{
		struct _mdat* mdat = (struct _mdat*)m_mData.GetAt(ii);
		if (code.CompareNoCase(mdat->ccod) == 0)
		{
			m_mData.RemoveAt(ii);

			mdat->ccod.Empty();
			mdat->iv.Empty();
			delete mdat;
			break;
		}
	}
}

void CMapWnd::addImageList(CString ImageName)
{
	CBitmap bitmap;
	HBITMAP	hBmp;

	CString	filename;
	filename.Format("%s\\image\\%s", m_sRoot, ImageName);

	hBmp = (HBITMAP)::LoadImage(AfxGetInstanceHandle(),
		filename, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	bitmap.Attach(hBmp);

	m_imageList->Add(&bitmap, RGB(255,255,255));
}

void CMapWnd::OnClickMingam()
{
	if (m_cfg->ShowDlg())
	{
		m_opsenscal->ReadCfg();
		readCfg();
		generateBasicPrice();
		generateFData();
		for (int ii = 0; ii < m_mData.GetSize(); ii++)
			generateMingam(ii);
		generateSum();

		if (IsWindow(m_pSheet1->m_pSonikPage->GetSafeHwnd()))
		{
			//TRACE("send[%.2f]\n", m_bData->basicprc);
			((CWnd*)m_pSheet1->m_pSonikPage->m_pGrpWnd.get())->SendMessage(WM_GRPBasicPrc, 0, (long)(m_bData->basicprc * 100.0));
		}
		redrawGraph();
	}
}

//2012.07.23 KSJ 주문버튼 추가
void CMapWnd::OnClickOrderBtn() 
{
	CString strData, strCode;
	double dPrc = 0.0;
	int nQty = 0;

	if(m_PFCodes.GetSize() > 0)
	{
		strCode = m_PFCodes.GetAt(0)->ccod;
		dPrc = m_PFCodes.GetAt(0)->curcalc;
		nQty = m_PFCodes.GetAt(0)->jango;
	}

	strData.Format("%s /S/t0/p99/d%s\t%s\n%s\t%lf\n%s\t%d\n", "IB301100", "ed_focod", strCode, "ed_jprc", dPrc, "ed_jqty", nQty);
// 	OutputDebugString("[KSJ]" + strData);
	m_pView->SendMessage(WM_USER, MAKEWPARAM(viewDLL, typeVIEW), (LPARAM)(LPCTSTR)strData);
}

//2012.07.23 KSJ 조회버튼 추가
void CMapWnd::OnClickSearch() 
{
	CfxGrid *pGrid = nullptr;
	int nChk{}, nCnt = 0;
	struct _gdat* gdat = nullptr;
	CString sDomino, sAccount, sPwd, sData, sLine, sCode, sGb, sPrice, sQty;
	CArray<struct _gdat*, struct _gdat*>* pCodes = nullptr;
	pGrid = m_pPFGrid.get();
	pCodes = &m_PFCodes;
	
	sAccount = m_pAccountCbo->GetAccNo(); sAccount.TrimRight();
	if (sAccount.IsEmpty())
	{
		AfxMessageBox("계좌를 선택해주세요.");
		setMessage("계좌를 선택해주세요.");
		Invalidate();
		return;
	}
	
	m_pPwdEdit->GetWindowText(sPwd); sPwd.TrimRight();
	if (sPwd.IsEmpty() && IsNumber(m_sUser))
	{
		AfxMessageBox("비밀번호를 입력해주세요.");
		setMessage("비밀번호를 입력해주세요.");
		Invalidate();
		return;
	}
	else if (sPwd.IsEmpty())
	{
		sPwd = "9999";
	}
	
	for (int ii = 0; ii < pCodes->GetSize(); ii++)
	{
		nChk = pGrid->GetItemParam(ii+1, gSelect);
		if (nChk != 1) continue;
		nCnt++;
	}
	
	if (nCnt == 0)
	{
		AfxMessageBox("선택된 종목이 없습니다.");
		setMessage("선택된 종목이 없습니다.");
		Invalidate();
		return;
	}
	
	//2014.12.15 KSJ 일방향암호관련 맵데이터에서 사용하는 암호화된 비번 추가해주어야함.
	sData.Format("%d\t%s\t%s\t", nCnt, sAccount, sPwd);	//총 갯수, 계좌번호, 비밀번호

	for (int ii = 0; ii < pCodes->GetSize(); ii++)
	{
		nChk = pGrid->GetItemParam(ii+1, gSelect);
		if (nChk != 1) continue;
		
		gdat = pCodes->GetAt(ii);
		sCode = gdat->ccod;
		sQty.Format("%d", gdat->jango);
		sPrice.Format("%012.2f", fabs(gdat->ordprc));
		sGb = (gdat->gb == -1) ? "1" : "2";
		sLine.Format("%s|00|%s|+%s|+%015s|\t", sCode, sGb, sPrice, sQty);
		sData += sLine;
	}
// 	sDomino.Format("/d%s\t%s\n%s\t%s\n%s\t%s\n", "zAcntNo", sAccount, "zInptPwd", sPwd, "domino_data", sData);

// 	m_pSearchPage->ChangeMap("IB410110", sDomino);
// 	m_pSearchPage->ChangeMap("IB410110", "");
	m_pSearchPage->ProcDll("SetData", sData);
}
//KSJ

void CMapWnd::OnClickConfigBtn() 
{
	CString sPath;
	sPath.Format("%s\\%s\\%s\\%s", m_sRoot, USRDIR, Variant(nameCC, ""), FILE3101);

	m_pConfigDlg = std::make_unique<class CConfigDlg>();
	m_pConfigDlg->SetPath(sPath);
	if(m_pConfigDlg->DoModal() == IDOK)
	{
		m_bMulti = m_pConfigDlg->GetConfig();
	}

	m_pConfigDlg = nullptr;
}

void CMapWnd::OnClickJango() 
{
	CString sAccn, sPwd;
	sAccn = m_pAccountCbo->GetAccNo(); sAccn.TrimRight();
	m_pPwdEdit->GetWindowText(sPwd); sPwd.TrimRight();

	if (sAccn.IsEmpty())
	{
		AfxMessageBox("계좌번호가 없습니다.");
		setMessage("계좌번호가 없습니다.");
		m_pAccountCbo->SetFocus();
		Invalidate();
		return;
	} 
 
	if (sPwd.IsEmpty() && IsNumber(m_sUser)) 
	{
		AfxMessageBox("비밀번호를 입력하세요.");
		setMessage("비밀번호를 입력하세요.");
		m_pPwdEdit->SetFocus();
		Invalidate();
		return; 
	}

	m_pNotify->Send(sAccn, sPwd);
}


void CMapWnd::OnAddItem()
{
	//2012.07.18 KSJ 종목다이얼로그가 떠 있을때 그래프를 그리면 에러남
	m_bOnDialog = true;

	CPoint pt(200, 0);
	ClientToScreen(&pt);

	if(m_bMulti)
	{
		CString code, gubn, qty, rcvdata, szUnit, dlgOK;
		int nRow=0, igubn=0, iqty=0;
		
		dlgOK = GetCtrlProperty("DoSelect");
		
// 		OutputDebugString("[KSJ] dlgOK[" + dlgOK + "]");

		nRow = 1;
		if(dlgOK == "-1")
		{
			m_bOnDialog = false;
			redrawGraph();
			
			rcvdata = GetCtrlProperty("GetProperties");
			
// 			OutputDebugString("[KSJ] [" + rcvdata + "]");

			while(!rcvdata.IsEmpty())
			{
				szUnit = Parser(rcvdata, "#");
				
				while(!szUnit.IsEmpty())
				{
					code = Parser(szUnit, "|");
					gubn = Parser(szUnit, "|");
					qty = Parser(szUnit, "|");
					
					if (findGData(&m_PFCodes, code) != -1) 
						continue;
					
// 					CIdCell idCell = m_pPFGrid->GetFocusCell();
// 					if (idCell.row > m_PFCodes.GetSize())
// 					{
// 						nRow = m_PFCodes.GetSize() + 1;
// 					} 
// 					else
// 					{
// 						nRow = idCell.row;
// 						
// 						if(nRow > 0)
// 							nRow++;
// 					}
// 					if (nRow < 1) 
// 						nRow = 1;
					
// 					CString strTemp;
// 					strTemp.Format("[KSJ] nRow[%d] idCell.row[%d]", nRow, idCell.row);
// 					OutputDebugString(strTemp);
					
					igubn = atoi(gubn);
					
					if(igubn == 1)
						igubn = -1;
					else if(igubn == 2)
						igubn = 1;

					//2013.07.24 KSJ 최대종목이 넘으면 메세지 보여준다. 100개까지는 안됨.
					if(m_PFCodes.GetSize() >= MAX_CODE - 30)
					{
						MessageBox("종목갯수는 최대 70개 입니다.", "전략주문", MB_OK|MB_ICONEXCLAMATION );
						return;
					}

					if(qty.Compare("") == 0)
					{
						setPF(nRow, code, igubn);
					}
					else
					{
						iqty = atoi(qty);
						setPF(nRow, code, igubn, iqty);
					}
				}	
			}			
			
			queryMingam();
			m_bAdd = true;
		}
		else if(dlgOK == "0")
		{
			m_bOnDialog = false;	//2014.12.22 KSJ 취소누를때도 false로 해줌
			redrawGraph();
		}
	}
	else
	{
		//2012.07.18 KSJ 종목다이얼로그가 떠 있을때 그래프를 그리면 에러남
		
		CString code;
		code = (char*)m_pView->SendMessage(WM_USER, 
			MAKEWPARAM(codeDLL, MAKEWORD(foptionCODE, 0)), MAKELPARAM(pt.x, pt.y));
		
		m_bOnDialog = false;
		
		redrawGraph();
		//2012.07.18 KSJ 종목다이얼로그를 종료하면 다시 그려줌.
		
		if (code.Find("\t") <= 0) return;
		
		// Portfolio에 종목중복은 허용하지 않는다
		code = code.Left(code.Find("\t"));
		if (findGData(&m_PFCodes, code) != -1) return;
		
		int nRow=0;
		const CIdCell idCell = m_pPFGrid->GetFocusCell();
		if (idCell.row > m_PFCodes.GetSize())
		{
			nRow = m_PFCodes.GetSize() + 1;
		} else
		{
			nRow = idCell.row;
		}
		if (nRow < 1) nRow = 1;
		
		CString strTemp;
// 		strTemp.Format("[KSJ] nRow[%d] idCell.row[%d]", nRow, idCell.row);
// 		OutputDebugString(strTemp);
		
		if (setPF(nRow, code))
			queryMingam();

		m_bAdd = true;
	}
}

void CMapWnd::OnDelItem()
{
	const int pos = -1;
	CString code;
	struct _gdat * gdat = nullptr;
	const CIdCell idCell = m_pPFGrid->GetFocusCell();
	const int row = idCell.row;
	if (row < 1 || row > m_PFCodes.GetSize())
		return;

	code = m_pPFGrid->GetItemText(row, gCode);
	if (code.IsEmpty()) return;

	gdat = m_PFCodes.GetAt(row-1);
	m_PFCodes.RemoveAt(row-1);
	delete gdat;	gdat = NULL;

	deleteMData(code);

	m_pPFGrid->DeleteRow(row);			
	m_pPFGrid->SetFocusCellEdit(row, gCode);
	if (IsWindow(m_pSheet1->m_pSonikPage->GetSafeHwnd()))
		(m_pSheet1->m_pSonikPage)->SendMessage(WM_POSInitData, (long)m_bData.get(), 0);
	queryMingam();
}

void CMapWnd::OnDelAllItem()
{
	deleteGrid(m_pPFGrid.get(), 0, true);
	queryMingam();
	if (IsWindow(m_pSheet1->m_pSonikPage->GetSafeHwnd()))
	{
		(m_pSheet1->m_pSonikPage)->SendMessage(WM_POSInitData, (long)m_bData.get(), 0);
		SetGraph();	// 2006.11.09 lsh
	}
}

void CMapWnd::OnExcel()
{
	RunExcel(true);
}

void CMapWnd::OnClickOrder()
{
	DoOrder();
}

void CMapWnd::DoOrder()
{
	CfxGrid *pGrid = nullptr;
	int nChk=0, nCnt = 0;
	struct _gdat* gdat = nullptr;
	CString sDomino, sAccount, sPwd, sCodes, sLine, sCode, sGb, sQty, sPrice;
	CArray<struct _gdat*, struct _gdat*>* pCodes = nullptr;
	pGrid = m_pPFGrid.get();
	pCodes = &m_PFCodes;

	sAccount = m_pAccountCbo->GetAccNo(); sAccount.TrimRight();
	if (sAccount.IsEmpty())
	{
		AfxMessageBox("계좌를 선택해주세요.");
		setMessage("계좌를 선택해주세요.");
		Invalidate();
		return;
	}

/*
	m_pPwdEdit->GetWindowText(sPwd); sPwd.TrimRight();
	if (sPwd.IsEmpty() && IsNumber(m_sUser))
	{
		AfxMessageBox("비밀번호를 입력해주세요.");
		setMessage("비밀번호를 입력해주세요.");
		Invalidate();
		return;
	}
*/

	for (int ii = 0; ii < pCodes->GetSize(); ii++)
	{
		nChk = pGrid->GetItemParam(ii+1, gSelect);
		if (nChk != 1) continue;
		nCnt++;
	}

	if (nCnt == 0)
	{
		AfxMessageBox("선택된 종목이 없습니다.");
		setMessage("선택된 종목이 없습니다.");
		Invalidate();
		return;
	}

	sCodes.Empty();
	for (int ii = 0; ii < pCodes->GetSize(); ii++)
	{
		nChk = pGrid->GetItemParam(ii+1, gSelect);
		if (nChk != 1) continue;

		gdat = pCodes->GetAt(ii);
		sCode = gdat->ccod;
		sQty.Format("%d", gdat->jango);
		sGb = (gdat->gb == -1) ? "1" : "2";
		sPrice.Format("%.2f", gdat->prc);
		sLine.Format("%s|%s|%s|%s|#", sCode, sGb, sQty, sPrice);
		sCodes += sLine;
	}
	sDomino.Format("%s /t /s /p5 /d%s\t%s\n%s\t%s\n", MAP_MULTIORDER, "AN20", sAccount, "ORDCODE", sCodes);	//2016.04.14 KSJ 일괄주문버튼 3004번으로 연동
	m_pView->SendMessage(WM_USER, MAKEWPARAM(viewDLL, typeVIEW), (long)(LPTSTR)(LPCTSTR)sDomino);
}

void CMapWnd::OnClickSaveStg()
{
	// 전략저장
	if (m_PFCodes.GetSize())
	{
		CString sPath;
		sPath.Format("%s\\%s\\%s", m_sRoot, USRDIR, Variant(nameCC, ""));

		CDlgSave Save(this);
		Save.m_sPath = sPath;
		Save.DoModal();

		// 나의전략 ReLoad
		if (IsWindow(m_pSheet1->m_pMyStgPage->GetSafeHwnd()))
			(m_pSheet1->m_pMyStgPage)->SendMessage(WM_MAPRefresh, UpdateUserList, 0);
	}
}

void CMapWnd::OnEditNotify(NMHDR * pHdr, LRESULT * result)
{
	switch (pHdr->idFrom)
	{
	case ID_EDIT_PASSWORD:
		if (pHdr->code == ENM_VKRETURN)
		{
			OnClickJango();			
		}
		break;
	}
}
 
void CMapWnd::OnGridLMouseUp(NMHDR * pHdr, LRESULT * result)
{
	DWORD nChk=0;
	NM_GVITEM *gItem = (NM_GVITEM *)pHdr;
	int ii=0;
	switch (pHdr->idFrom)
	{
	case ID_JANGOGRID:
	case ID_PFGRID:
		CfxGrid *pGrid = m_pPFGrid.get();
		switch (gItem->col)
		{
		case gSelect:
			if (gItem->row == 0)
			{
				// Grid Header CheckBox Select
				if (pGrid->GetItemText(1, gCode).IsEmpty()) 
				{
					pGrid->SetItemParam(gItem->row, gSelect, 0);
					break;
				}

				nChk = pGrid->GetItemParam(gItem->row, gItem->col);
				for (ii = 1; ii < pGrid->GetRowCount(); ii++)
				{
					if (pGrid->GetItemText(ii, gCode).IsEmpty()) break;
					pGrid->SetItemParam(ii, gSelect, nChk);
				}
			} 
			else
			{
				// Grid DataRow CheckBox Select
				if (pGrid->GetItemText(gItem->row, gCode).IsEmpty()) 
				{
					pGrid->SetItemParam(gItem->row, gSelect, 0);
					break;
				}
			}
			generateSum();
			if (IsWindow(m_pSheet1->m_pSonikPage->GetSafeHwnd()))
				(m_pSheet1->m_pSonikPage)->SendMessage(WM_POSInitData, (long)m_bData.get(), 0);
			SetGraph();
			break;
		case gPrc:
			if (gItem->row != 0) break;
			m_prcMode = (m_prcMode == pmBuyPrc) ? pmCurrPrc : pmBuyPrc;
			ChangePrcMode(m_prcMode);
			SetGraph();
			break;
		case gCurr:
			if (gItem->row != 0) break;
			if (m_currMode == cmExpire)
				m_currMode = cmCurr;
			else
				m_currMode = (GRID_CURRMODE)((int)m_currMode + 1);
			ChangeCurrMode(m_currMode);
			break;
		}
		break;
	}
}

void CMapWnd::ChangeColors()
{
	m_clrGradient1 = GetAxColor(TOOLINDEXCOLOR);
	m_bkColor = GetAxColor(BACKINDEXCOLOR);

	m_pTab1->LoadTabImage(m_sRoot, FALSE);
	m_pTab1->SetColor(m_pApp->GetIndexColor(m_pView, 70), m_pApp->GetIndexColor(m_pView, 3), 
		              m_pApp->GetIndexColor(m_pView, 70), m_pApp->GetIndexColor(m_pView, 181), m_pApp->GetIndexColor(m_pView, 181));

	m_pTab1->Invalidate();

	if(m_pSheet1)
	{
		if(m_pSheet1->m_pStgPage)
		{
			m_pSheet1->m_pStgPage->m_bkColor = m_clrGradient1;
			m_pSheet1->m_pStgPage->Invalidate();
		}

		if(m_pSheet1->m_pMyStgPage)
		{
			m_pSheet1->m_pMyStgPage->m_bkColor = m_clrGradient1;
			m_pSheet1->m_pMyStgPage->Invalidate();
		}

		if(m_pSheet1->m_pSonikPage)
		{
			m_pSheet1->m_pSonikPage->m_bkColor = m_clrGradient1;
			m_pSheet1->m_pSonikPage->Invalidate();
		}

		if(m_pSheet1->m_pHedgePage)
		{
			m_pSheet1->m_pHedgePage->m_bkColor = m_clrGradient1;
			m_pSheet1->m_pHedgePage->Invalidate();
		}
	}

	Invalidate();
}

void CMapWnd::ChangePrcMode(enum GRID_PRCMODE mode)
{
	int idx=0;
	CString code;
	struct _gdat* gdat = nullptr;
	struct _mdat* mdat = nullptr;

	CString strTemp;

	switch (mode)
	{
	case pmBuyPrc: // 매입단가
		m_pPFGrid->SetItemText(0, gPrc, "단가");
		break;
	case pmCurrPrc: // 현재가
		m_pPFGrid->SetItemText(0, gPrc, "단가(현)");
		break;
	}
	for (int ii = 0; ii < m_PFCodes.GetSize(); ii++)
	{
		code = m_pPFGrid->GetItemText(ii+1, gCode);
		if (code.IsEmpty()) break;
		gdat = m_PFCodes.GetAt(ii);

		if (mode == pmBuyPrc)
		{
// 			strTemp.Format("[KSJ]ChangePrcMode old[%lf] new[%lf]", gdat->prc, gdat->ordprc);
			gdat->ordprc = gdat->prc;
// 			gdat->prc = gdat->ordprc;
// 			OutputDebugString(strTemp);
			
			m_pPFGrid->SetItemAttr(ii, gPrc, m_pPFGrid->GetItemAttr(ii, gPrc)|GVAT_EDIT);
		}
		else // if (mode == pmCurrPrc)
		{
			idx = findMData(code);
			if (idx != -1)
			{
				mdat = m_mData.GetAt(idx);

// 				strTemp.Format("[KSJ]ChangePrcMode1 old[%lf] new[%lf]", gdat->prc, mdat->ccur);

				gdat->prc = gdat->ordprc;
				gdat->ordprc = mdat->ccur;
// 				OutputDebugString(strTemp);

				m_pPFGrid->SetItemAttr(ii, gPrc, m_pPFGrid->GetItemAttr(ii, gPrc) & ~GVAT_EDIT);
			}
		}
	}
	for (int ii = 0; ii < m_mData.GetSize(); ii++)
		generateMingam(ii);
	generateSum();
}

void CMapWnd::ChangeCurrMode(enum GRID_CURRMODE mode)
{
	int idx=0;
	CString code;
	struct _gdat* gdat = nullptr;
	struct _mdat* mdat = nullptr;

	switch (mode)
	{
	case cmCurr:
		m_pPFGrid->SetItemText(0, gCurr, "현재가");
		break;
	case cmOp1:
		m_pPFGrid->SetItemText(0, gCurr, "상대1호가");
		break;
	case cmOp2:
		m_pPFGrid->SetItemText(0, gCurr, "상대2호가");
		break;
	case cmExpire:
		m_pPFGrid->SetItemText(0, gCurr, "만기가");
		break;
	}
	for (int ii = 0; ii < m_PFCodes.GetSize(); ii++)
	{
		code = m_pPFGrid->GetItemText(ii+1, gCode);
		if (code.IsEmpty()) break;

		idx = findMData(code);
		if (idx == -1) continue;
		mdat = m_mData.GetAt(idx);
		gdat = m_PFCodes.GetAt(ii);

		switch (mode)
		{
		case cmCurr:
			gdat->curcalc = mdat->ccur;
			break;
		case cmOp1:
			if (gdat->gb == 1)
				gdat->curcalc = mdat->mdga[0];
			else
				gdat->curcalc = mdat->msga[0];
			break;
		case cmOp2:
			if (gdat->gb == 1)
				gdat->curcalc = mdat->mdga[1];
			else
				gdat->curcalc = mdat->msga[1];
			break;
		case cmExpire:
			if (gdat->ccod[0] == '2' || gdat->ccod[0] == 'B')  //파생상품 코드개편
			{
				gdat->curcalc = fabs(m_bData->k200cur) - m_pApp->getOptionStrike(gdat->ccod);
				if (gdat->curcalc < 0) gdat->curcalc = 0;	
			}
			else if (gdat->ccod[0] == '3' || gdat->ccod[0] == 'C')  //파생상품 코드개편
			{
				gdat->curcalc = m_pApp->getOptionStrike(gdat->ccod) - fabs(m_bData->k200cur);
				if (gdat->curcalc < 0) gdat->curcalc = 0;	
			}
			else // if (gdat->ccod[0] == '1')
			{
				gdat->curcalc = fabs(mdat->ccur);
			}
			break;
		}
	}
	for (int ii = 0; ii < m_mData.GetSize(); ii++)
		generateMingam(ii);
	generateSum();
}

void CMapWnd::OnGridLMouseDown(NMHDR * pHdr, LRESULT * result)
{
	NM_GVITEM *gItem = (NM_GVITEM *)pHdr;
	struct _gdat* gdat = nullptr;
	CArray<struct _gdat*, struct _gdat*> *pGArray = nullptr;//, *pOtherGArray;

	switch (pHdr->idFrom)
	{
	case ID_JANGOGRID:
	case ID_PFGRID:
		if (gItem->row == 0) return;
		CfxGrid *pGrid = m_pPFGrid.get();
		pGArray = &m_PFCodes;

		int idx=0;
		CRect rc;
		CString prvcode, code, name;
		code = pGrid->GetItemText(gItem->row, gCode); code.TrimRight();
		if (code.IsEmpty()) return;

		switch (gItem->col)
		{
		case gCodeCtrl:
			// 잔고 Grid는 종목변경 불가
			if (pHdr->idFrom == ID_JANGOGRID) return;

			pGrid->GetItemRect(gItem->row, gItem->col, &rc);
			pGrid->ClientToScreen(&rc);

			m_bOnDialog = true;

			prvcode = code;
			code = (char*) m_pView->SendMessage(WM_USER, 
				MAKEWPARAM(codeDLL, MAKEWORD(foptionCODE, 0)), 
				MAKELPARAM(rc.left, rc.bottom));

			m_bOnDialog = false;

			if (code.GetLength() == 0) return;

			if (code.Find("\t") != -1)
			{
				name = code.Mid(code.Find("\t") + 1);
				code = code.Left(code.Find("\t"));
			}
				
			if (findGData(&m_PFCodes, code) != -1)
			{
				AfxMessageBox("등록된 종목입니다.");
				setMessage("등록된 종목입니다.");
				Invalidate();
				return;
			}

			idx = findGData(pGArray, prvcode);
			if (idx == -1)
				return;
			gdat = pGArray->GetAt(idx);
			gdat->ccod = code;
			gdat->addnew = true;
			gdat->prc = 0;
			gdat->ordprc = 0;
			pGrid->SetItemText(gItem->row, gCode, code);
			pGrid->SetItemText(gItem->row, gName, name);

			queryMingam();
			break;
		case gGB:
			if (pHdr->idFrom == ID_PFGRID)
			{
				CString code, gb;
				code = m_pPFGrid->GetItemText(gItem->row, gCode);
				gb = m_pPFGrid->GetItemText(gItem->row, gItem->col);

				int idx=0;
				struct _gdat *gdat = nullptr;
				idx = findGData(&m_PFCodes, code);
				if (idx == -1)
					return;
				gdat = m_PFCodes.GetAt(idx);
				gdat->gb = (gb.Find("매도") == -1) ? -1 : 1;
				m_pPFGrid->SetItemParam(gItem->row, gGB, gdat->gb == -1 ? 0:1);

				struct _mdat *mdat = nullptr;
				idx = findMData(code);
				if(idx == -1)
				{
					return;
				}
				mdat = m_mData.GetAt(idx);
				setGridVal(m_pPFGrid.get(), mdat, false);
				generateSum();
				SetGraph();				
			}
			break;
		case gPrcCtrl:
			pGrid->GetItemRect(gItem->row, gItem->col, &rc);
			// 구분, RTS CODE, 현재가, 상한가, 하한가 

			const int idx = findMData(code);
			if (idx == -1) return;

			m_priceRow = gItem->row;
			m_priceGrid = pGrid;
			struct _mdat* mdat = m_mData.GetAt(idx);
			CString str;
			str.Format("%s%-12s%-8.2f%-8.2f%-8.2f", PriceGubun(code), "", mdat->ccur, mdat->shga, mdat->hhga);
			m_PriceData.Format("%s%-12s%-8.2f%-8.2f%-8.2f", PriceGubun(code), "", mdat->ccur, mdat->shga, mdat->hhga);
			m_PriceRect = rc;
			
			sendTR(TRN_JJGA, (LPTSTR)(LPCTSTR)code, code.GetLength(), KEY_JJGA, 0);
// 			m_price->SetValues(str);
// 			m_price->SetWindowPos(NULL, rc.right, rc.top, 0, 0, SWP_NOZORDER|SWP_NOSIZE);
// 			m_price->BtnClick();
			break;
		}
		break;
	}
}

void CMapWnd::OnGridEndEdit(NMHDR * pHdr, LRESULT * result)
{
	NM_GVITEM *gItem = (NM_GVITEM *)pHdr;

	switch (pHdr->idFrom)
	{
	case ID_JANGOGRID:
	case ID_PFGRID:
		{
			CArray<struct _gdat*, struct _gdat*> *pGArray = nullptr;
			pGArray = &m_PFCodes;
			CfxGrid *pGrid = m_pPFGrid.get();

			CString code;
			code = pGrid->GetItemText(gItem->row, gCode); code.TrimRight();
			if (code.IsEmpty()) return;

			CString val;
			struct _gdat *gdat = nullptr;
			struct _mdat *mdat = nullptr;
			gdat = pGArray->GetAt(gItem->row -1);
			val = pGrid->GetItemText(gItem->row, gItem->col);
			switch (gItem->col)
			{
			case gJango:
				gdat->jango = atoi(val);
				break;
			case gPrc:
				gdat->prc = atof(val);
				gdat->ordprc = atof(val);
				break;
			}
			
			const int idx = findMData(code);

			if(idx == -1)
			{
				return;
			}

			mdat = m_mData.GetAt(findMData(code));
			setGridVal(pGrid, mdat, false);
			generateSum();
			SetGraph();
		}
		break;
	}
}

void CMapWnd::OnGridEndSort(NMHDR * pHdr, LRESULT * result)
{
	NM_GVITEM *gItem = (NM_GVITEM *)pHdr;

	switch (pHdr->idFrom)
	{
	case ID_JANGOGRID:
	case ID_PFGRID:
		{
			CfxGrid	*pGrid;
			pGrid = m_pPFGrid.get();
			ReArrangeCodes(pGrid);
		}
		break;
	}
}

void CMapWnd::OnTab1Change(NMHDR * pHdr, LRESULT * result)
{
	if (pHdr->idFrom == ID_TAB1)
	{
		m_pSearchPage->ShowWindow(SW_HIDE);
// 		m_pSearchBtn->ShowWindow(SW_HIDE);

		int nTab;
		nTab = m_pTab1->GetCurSel();
		if (nTab >= 0 && nTab < m_pSheet1->GetPageCount())
		{
			(CWnd*)m_pSheet1->ShowWindow(SW_SHOW);

			m_pSheet1->SetActivePage(nTab);
			switch(nTab)
			{
			case 0: // 만기손익
				{

					(m_pSheet1->m_pSonikPage)->SendMessage(WM_POSInitData, (long)m_bData.get(), 0);

					struct _mdat *mdat = nullptr;
					for(int i = 0 ; i < m_mData.GetSize(); i++)
					{
						mdat = m_mData.GetAt(i);
						setGridVal(m_pPFGrid.get(), mdat, false);
						generateSum();
						SetGraph();				
					}

//				SetGraph();
				}
				break;
			case 1: // 전략선택
				break;
			case 2: // 나의전략
				(m_pSheet1->m_pMyStgPage)->SendMessage(WM_POSInitData, (long)m_bData.get(), 0);
				break;
			case 3: // 헤지
				break;
			}
		}

		if (nTab == 4)	//2012.07.16 KSJ 증거금조회 추가
		{	
			(CWnd*)m_pSheet1->ShowWindow(SW_HIDE);
			
			m_pSearchPage->ShowWindow(SW_SHOW);
// 			m_pSearchBtn->SetForegroundWindow();
// 			m_pSearchBtn->BringWindowToTop();
// 			m_pSearchBtn->SetFocus();
// 			m_pSearchBtn->ShowWindow(SW_SHOW);
		}
	}
	return;
}

void CMapWnd::SetRoundWnd(CDC* pDc, int nSelTab)
{
	CRect rc;
	CRect rcWnd;
	const int nSideGap = 3;
	GetWindowRect(rc);
	ScreenToClient(rc);

	rcWnd = rc;

	rcWnd.left = nSideGap;
	rcWnd.right = rc.right - nSideGap;
	rcWnd.bottom = rc.bottom - nSideGap - GUIDEBAR_HEIGHT;

	m_pTab1->GetWindowRect(rc);
	ScreenToClient(rc);
	rcWnd.top =  rc.bottom;

	pDc->FillSolidRect(&rcWnd, m_clrGradient1);
	m_pSheet1->SetBkColor(pDc, nSelTab, m_clrGradient1);

//	DrawRoundBitmap(pDc, rcWnd, ROUND_PANEL);
}

void CMapWnd::setJango(CString dat)
{
	int cnt{}, row{};
	CString str, line, code, name, gb, jango, availQty, prc, fullbuyprc, sonik, ordprc;
	deleteGrid(m_pPFGrid.get(), 0, true);
	struct _gdat* gdat{};
	
	for (int ii = 0; ii < m_PFCodes.GetSize(); ii++)
	{
		gdat = m_PFCodes.GetAt(ii);
		delete gdat;	gdat = NULL;
	}
	m_PFCodes.RemoveAll();

// 	OutputDebugString("[KSJ]" + dat);

	row = 1;
	cnt = atoi(Parser(dat, "\t"));
	m_pPFGrid->ShowWindow(SW_HIDE);
	m_pPFGrid->SetRowCount((cnt+1 < 10) ? 10 : cnt+1);

	for (int ii = 1; ii <= cnt; ii++)
	{
		line = Parser(dat, "\n");
		if (line.IsEmpty()) break;

		// 종목코드(00)|종목명(01)|구분(02)|잔고수량(03)|매도가능수량(04)|단가(05)|매입금액(11)|현재가(06)|거래승수
		code = Parser(line, "\t");	
		Parser(line, "\t");	
		str.Format("%s", code);
		name.Format("%s", (char*)m_pView->SendMessage(WM_USER, MAKEWPARAM(nameDLL, 0), (long)(LPSTR)(LPCTSTR)str));

		gb = Parser(line, "\t");
		jango = Parser(line, "\t");	
		availQty = Parser(line, "\t");	
		ordprc = Parser(line, "\t");
		fullbuyprc = Parser(line, "\t");	
		prc = Parser(line, "\t");
		sonik = Parser(line, "\t");

		if (code[2] == '3') continue;	// KOSTAR 종목 제외

		CString type = code.Mid(1,2);

		const int nType = atoi(type);

		if((nType >= 11 && nType <= 99) || (type >= "B0" && type <= "ZZ") || (type >= "0A" && type <= "0Z") || (type >= "1A" && type <= "1Z")
			|| (type >= "2A" && type <= "2Z") || (type >= "3A" && type <= "3Z") || (type >= "4A" && type <= "4Z") || (type >= "5A" && type <= "5Z")
			|| (type >= "6A" && type <= "6Z") || (type >= "7A" && type <= "7Z") || (type >= "8A" && type <= "8Z") || (type >= "9A" && type <= "9Z"))	//2024.10.31 주식선물
		{
			CString m_slog;
			m_slog.Format("[4101][SF][%s]<%d> code=[%s]", __FUNCTION__, __LINE__, code);
			OutputDebugString(m_slog);
			continue;
		}

		gdat = new _gdat;
		gdat->addnew = true;
		gdat->ccod = code;
		gdat->miga = atof(fullbuyprc);	// 매입가추가
// 		if (code[0] == '1')
// 			gdat->prc = atof(fullbuyprc) / atof(jango) / 500000;
// 		else
//			gdat->prc = atof(fullbuyprc) / atof(jango) / 500000;

		gdat->prc = atof(prc);			//2012.05.15 KSJ 현재가를 여기서 세팅한다.
		gdat->ordprc = atof(ordprc);	//2012.05.14 KSJ 여기서 단가를 세팅한다.
		gdat->sonik = atof(sonik);
		
		gdat->gdan = 250000;	//KSJ 초기값을 50만원으로 세팅
// 		if(!gdan.IsEmpty())
// 			gdat->gdan = atof(gdan);		//2012.05.15 KSJ 거래승수를 여기서 세팅한다.
// 		else
// 			gdat->gdan = 500000;	//2012.05.15 KSJ빈 값일때는 50만언으로 세팅

// 		CString strTemp;
// 		strTemp.Format("[KSJ][setJango]prc[%.4f]ordprc[%.4f]fullbuyprc[%s]gdat->miga[%.2f]", gdat->prc, gdat->ordprc, fullbuyprc, gdat->miga);
// 		OutputDebugString(strTemp);

		gdat->gb  = (gb.Find("매도") != -1) ? -1 : 1;
		gdat->jango = atoi(jango);
		m_PFCodes.Add(gdat);

		initGridRow(m_pPFGrid.get(), row);
		m_pPFGrid->SetItemParam(row, gGB, gdat->gb == -1 ? 0:1);
		m_pPFGrid->SetItemText(row, gCode, code);
		m_pPFGrid->SetItemText(row, gName, name);
		m_pPFGrid->SetItemText(row, gGB, gb);
		m_pPFGrid->SetItemText(row, gJango, jango);

		row++;
	}
#ifdef	_DEBUG
	m_pPFGrid->Sort(gCode, true);
#else
	m_pPFGrid->Sort(gName, true);
#endif
	m_pPFGrid->ShowWindow(SW_SHOW);
	ReArrangeCodes(m_pPFGrid.get());
	m_pPFGrid->SetRowCount(row < 10 ? 10 : row);

}

bool CMapWnd::setPF(int nRow, CString code, int gb, int jango, double prc)
{
	CString val, cname;
	val.Format("%s", code);
	cname.Format("%s", (char*)m_pView->SendMessage(WM_USER, MAKEWPARAM(nameDLL, 0), (long)(LPSTR)(LPCTSTR)val));

	cname.TrimRight();
	if (cname.IsEmpty()) return false;

	gsl::owner<struct _gdat *>gdat;


	gdat = new _gdat;
	gdat->ccod.Format("%s", code);
	gdat->gb = gb;
	gdat->jango = jango;
	gdat->prc = prc;
	gdat->ordprc = prc;
	gdat->sonik = 0;
	gdat->miga = 0;	//2012.06.22 KSJ 매입가 초기화
	gdat->addnew = true;
	m_PFCodes.InsertAt(nRow-1, gdat);
	m_pPFGrid->InsertRow(gdat->ccod, nRow);
	initGridRow(m_pPFGrid.get(), nRow);
	m_pPFGrid->SetFocusCellEdit(nRow, gCode);

	CString strTemp;
// 	strTemp.Format("[KSJ] nRow[%d] m_PFCodes.GetSize[%d]", nRow, m_PFCodes.GetSize());
// 	OutputDebugString(strTemp);

	m_pPFGrid->SetItemParam(nRow, gSelect, 1);
	val.Format("%s", gdat->gb == 1 ? "매수" : "매도");		
	{
		m_pPFGrid->SetItemParam(nRow, gGB, gdat->gb == -1 ? 0:1);
		m_pPFGrid->SetItemText(nRow, gGB, val);
	}
	val.Format("%d", gdat->jango);		m_pPFGrid->SetItemText(nRow, gJango, val);
	val.Format("%.2f", gdat->prc);		m_pPFGrid->SetItemText(nRow, gPrc, val);

	m_pPFGrid->SetItemText(nRow, gCode, code);
	m_pPFGrid->SetItemText(nRow, gName, cname);
	return true;
}

void CMapWnd::queryInitValue()
{
	CString code, sqry;
	code = 	(char*)m_pView->SendMessage(WM_USER, MAKEWPARAM(codeDLL, MAKEWORD(getCODEcall, 0)), 0);
	
	sqry.Format("%s%c%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t", 
		"40301", 0x7f, code, "40370", "40314", "40361", "40369", "40368", "40367", "40332", "40323");
		// 영업일, 달력일, 역사적변동성(90, 60, 30, 21), cd금리, 배당지수
	sendTR(TRN_OOP, (LPTSTR)(LPCTSTR)sqry, sqry.GetLength(), KEY_INITVALUE, US_OOP);

	code = "101";
	sqry.Format("%s%c%s\t%s\t%s\t", 
		"20301", 0x7f, code, "20034", "20023");
		// 체결시간, 업종현재가
	sendTR(TRN_OOP, (LPTSTR)(LPCTSTR)sqry, sqry.GetLength(), KEY_UPJONG, US_OOP);
}

void CMapWnd::queryMingam()
{
	m_bRts = false;
	struct _mid mid;
	FillMemory(&mid, sizeof(struct _mid), ' ');

	CString val, param;
	param.Format("%%.%dd", sizeof(mid.crec));
	val.Format(param, m_PFCodes.GetSize());	//m_JangoCodes.GetSize() + 
	CopyMemory(&mid.crec, val.GetBuffer(val.GetLength()), sizeof(mid.crec));
	val.ReleaseBuffer();

	bool bFound{};
	struct _gdat* gdat{};
	CStringArray qryCodes;
	for (int ii = 0; ii < m_PFCodes.GetSize(); ii++)
	{
		bFound = false;
		gdat = m_PFCodes.GetAt(ii);
		val = gdat->ccod;
		for (int jj = 0; jj < qryCodes.GetSize(); jj ++)
		{
			if (qryCodes.GetAt(jj).CompareNoCase(val) == 0)
			{
				bFound = true;
				break;
			}
		}
		if (bFound) continue;

		qryCodes.Add(val);
	}

	for (int ii = 0; ii < qryCodes.GetSize(); ii++)
	{
		val = qryCodes.GetAt(ii);
		CopyMemory(&mid.codes[ii], val.GetBuffer(val.GetLength()), sizeof(mid.codes[0]));
	}
	sendTR(TRN_MINGAM, (char*)&mid, 3 + 8 * qryCodes.GetSize()/*sizeof(struct _mid)*/, KEY_CODEMINGAM, 0);
}

void CMapWnd::parseInitValue(char* dat)
{
	// 영업일, 달력일, 역사적변동성(90, 60, 30, 21)
	CString str, sdat(dat);
	str = Parser(sdat);	m_bData->jjil[0] = atof(str);
	str = Parser(sdat);	m_bData->jjil[1] = atof(str);
	for (int ii =0; ii < 4; ii++)
	{
		str = Parser(sdat);
		m_bData->hv[ii] = atof(str);
	}
	str = Parser(sdat);	m_bData->cdgl = atof(str);// / 1000;
	str = Parser(sdat);	m_bData->bdjs = atof(str) / DBASE;
}

void CMapWnd::parseUpJong(char* dat)
{
		// 영업일, 달력일, 역사적변동성(90, 60, 30, 21)
	CString str, sdat(dat);
	str = Parser(sdat);	m_bData->time.Format("%s", str);
}

void CMapWnd::parseMingam(char *dat)
{
	struct _mod *mod;
	mod = (struct _mod *)dat;
	
	CString str;
	m_bData->frts = CString(mod->frts, sizeof(mod->frts));	m_bData->frts.TrimRight();
	m_bData->fnam = CString(mod->fnam, sizeof(mod->fnam));
	m_bData->fcur = atof(CString(mod->fcur, sizeof(mod->fcur)));
	m_bData->krts = CString(mod->krts, sizeof(mod->krts));	m_bData->krts.TrimRight();
	m_bData->kcur = atof(CString(mod->kcur, sizeof(mod->kcur)));
	m_bData->k200rts = CString(mod->k200rts, sizeof(mod->k200rts));	m_bData->k200rts.TrimRight();
	m_bData->k200cur = atof(CString(mod->k200cur, sizeof(mod->k200cur)));

	m_fcode = (char*)m_pView->SendMessage(WM_USER, MAKEWPARAM(codeDLL, MAKEWORD(getCODEfuture, 0)), 0);
	FindAtmOption(fabs(m_bData->k200cur));

	str.Format("%.2f", m_bData->cdgl);	m_opsenscal->SetCd(str);
	str.Format("%.6f", m_bData->bdjs);	m_opsenscal->SetBedangJisu(str);
	m_opsenscal->SetHv(mod->hv[0], mod->hv[1], mod->hv[2], mod->hv[3]);

	readCfg();
	generateBasicPrice();
	generateFData();

	int pos=0, idx=0;
	CString code, firstcode("");
	gsl::owner<struct _mdat*> mdat = nullptr;
	int nRec = atoi(CString(mod->grec, sizeof(mod->grec)));
	for (int ii = 0; ii < nRec; ii++)
	{
		struct  _gmod *gmod = &mod->gmod[ii];
		code.Format("%s", CString(gmod->ccod, sizeof(gmod->ccod))); code.TrimRight();

		//2012.04.06 KSJ X 붙어있음
		if(code.GetAt(0) == 'x' || code.GetAt(0) == 'X')
		{
			code = code.Mid(1);
// 			OutputDebugString(code);
		}
		// KSJ

		if (firstcode.IsEmpty()) firstcode = code;
		if (code.IsEmpty()) continue;

		pos = findMData(code);
		if (pos >= 0)
		{
			mdat = m_mData.GetAt(pos);
		}
		else
		{
			mdat = new _mdat;
			mdat->ccod.Format("%s", code);
		}

		idx = findGData(&m_PFCodes, code);
		if (idx >= 0 && m_PFCodes.GetAt(idx)->ordprc == 0)
		{
			m_PFCodes.GetAt(idx)->ordprc = fabs(atof(CString(gmod->ccur, sizeof(gmod->ccur))));
			m_PFCodes.GetAt(idx)->prc = m_PFCodes.GetAt(idx)->ordprc;
		}
		
		//2012.05.17 KSJ 거래승수 추가해줌.
		if(idx >= 0)
		{
			m_PFCodes.GetAt(idx)->gdan = fabs(atof(CString(gmod->gdan, sizeof(gmod->gdan))));
		}
		//KSJ

		mdat->ccur = fabs(atof(CString(gmod->ccur, sizeof(gmod->ccur))));
		mdat->shga = atof(CString(gmod->shga, sizeof(gmod->shga)));
		mdat->hhga = atof(CString(gmod->hhga, sizeof(gmod->hhga)));
		for (int jj = 0; jj < 5; jj++)
		{
			mdat->mdga[jj] = atof(CString(gmod->mdga[jj], sizeof(gmod->mdga[0])));
			mdat->msga[jj] = atof(CString(gmod->msga[jj], sizeof(gmod->msga[0])));
		}
		mdat->jjil[0] = atof(CString(gmod->jjil[0], sizeof(gmod->jjil[0])));
		mdat->jjil[1] = atof(CString(gmod->jjil[1], sizeof(gmod->jjil[1])));

		if (pos < 0) 
		{
			m_mData.Add(mdat);
			generateMingam(m_mData.GetSize() -1);
		}
		else
		{
			generateMingam(pos);
		}
	}
	generateSum();
	SetGraph();

	if (IsWindow(m_pSheet1->m_pStgPage->GetSafeHwnd()))
		(m_pSheet1->m_pStgPage)->SendMessage(WM_STGInitData, 0, 0);
	m_bRts = true;
}

void CMapWnd::parseAlert(char *pDat)
{
	if (!m_bRts) return;

	int idx{};
	CString dat, sym, val, code;
	enum alertType { ctK200, ctKospi, ctFuture, ctOption } codeType{};
	dat.Format("%s", pDat);

	code = Parser(dat); code.TrimRight();


	if (code.IsEmpty()) return;

	if (m_bData->frts.CompareNoCase(code) == 0)
	{
		codeType = ctFuture;
		idx = findMData(code);
	}
	else if (m_bData->krts.CompareNoCase(code) ==  0)
	{
		codeType = ctKospi;
	}
	else if (m_bData->k200rts.CompareNoCase(code) == 0)
	{
		codeType = ctK200;
	}
	else
	{
		if (code[0] != '2' && code[0] != '3' && code[0] != 'B' && code[0] != 'C') return;   //파생상품 코드개편
		codeType = ctOption;
		
		idx = findMData(code);
		if (idx == -1) return;
	}

	int pos=0;
	CMapStringToString mapRts;
	while (!dat.IsEmpty())
	{
		pos = dat.Find('\t');
		if (pos < 0)	break;

		sym = dat.Left(pos++);
		dat = dat.Mid(pos);

		pos = dat.Find('\t');
		if (pos < 0)	pos = dat.GetLength();
		val = dat.Left(pos++);

		mapRts.SetAt(sym, val);

		if (dat.GetLength() <= pos)	break;

		dat = dat.Mid(pos);
	}

	bool bChange = false;
	int ii = 0;
	switch (codeType)
	{
	case ctKospi:
		if (!mapRts.Lookup("023", val)) return;		// 현재가
		val.Replace("+", ""); val.Replace("-", "");
		if (m_bData->kcur != atof(val))
		{
			m_bData->kcur = atof(val);
			generateFData();
			bChange = true;
		}
		break;
	case ctK200:
		if (!mapRts.Lookup("023", val)) return;		
		val.Replace("+", ""); val.Replace("-", "");
		if (m_bData->k200cur != atof(val))
		{
			m_bData->k200cur = atof(val);
			generateBasicPrice();
			generateFData();
			bChange = true;
		}
		break;
	case ctFuture:
		{
			if (!mapRts.Lookup("023", val)) return;
			val.Replace("+", ""); val.Replace("-", "");
			m_bData->fcur = atof(val);
			generateBasicPrice();
			generateFData();

			idx = findMData(code);
			if (idx == -1) return;

			double dval=0;
			struct _mdat* mdat = (struct _mdat*)m_mData.GetAt(idx);
			for (ii = 0; ii < 5; ii++)
			{
				if (mapRts.Lookup(mdatsym->mdga[ii], val))
				{
					val.Replace("+", ""); val.Replace("-", "");
					mdat->mdga[ii] = atof(val);
				}

				if (mapRts.Lookup(mdatsym->msga[ii], val))
				{
					val.Replace("+", ""); val.Replace("-", "");
					mdat->msga[ii] = atof(val);
				}
			}

			if (mapRts.Lookup(mdatsym->ccur, val))
			{
				val.Replace("+", ""); val.Replace("-", "");
				dval = atof(val);
				if (dval != mdat->ccur)
				{
					mdat->ccur = dval;
					generateMingam(idx);
				}
			}

			setGridVal(m_pPFGrid.get(), mdat, false);
			for (ii = 0; ii < m_mData.GetSize(); ii++)
				generateMingam(ii);
			generateSum();
		}
		break;
	case ctOption:
		{
			double dval=0;
			struct _mdat* mdat = (struct _mdat*)m_mData.GetAt(idx);
			for (int ii = 0; ii < 5; ii++)
			{
				if (mapRts.Lookup(mdatsym->mdga[ii], val))
				{
					val.Replace("+", ""); val.Replace("-", "");
					mdat->mdga[ii] = atof(val);
				}

				if (mapRts.Lookup(mdatsym->msga[ii], val))
				{
					val.Replace("+", ""); val.Replace("-", "");
					mdat->msga[ii] = atof(val);
				}
			}

			if (mapRts.Lookup(mdatsym->ccur, val))
			{
				val.Replace("+", ""); val.Replace("-", "");
				dval = atof(val);
				if (dval != mdat->ccur)
				{
					mdat->ccur = dval;
					generateMingam(idx);
				}
			}

			setGridVal(m_pPFGrid.get(), mdat, false);
			generateSum();
		}
		break;
	}

	if (bChange)
	{
		for (ii = 0; ii < m_mData.GetSize(); ii++)
			generateMingam(ii);
		generateSum();
	}
}

void CMapWnd::parseAlert(struct _alertR* alertR)
{
	if (!m_bRts) return;

	int idx{};
	CString dat, sym, val, code;
	enum alertType { ctK200, ctKospi, ctFuture, ctOption } codeType{};

	if (alertR->code.IsEmpty()) return;

	//2012.04.06 KSJ X 붙어있음
	code = alertR->code;

	if(code.GetAt(0) == 'x' || code.GetAt(0) == 'X')
	{
		code = code.Mid(1);
// 		OutputDebugString(code);
	}
	// KSJ


// 	CString strTemp;
// 	strTemp.Format("[KSJ]code[%s] alert[%s] codeType[%d]", alertR->code, (char*)((DWORD*)alertR->ptr[0])[0], codeType);
// 	OutputDebugString(strTemp);

	//2017.01.16 KSJ 최근월물만 실시간 되는것 수정
	if (code[0] == '1' || code[0] == '4' || code[0] == 'A' || code[0] == 'D') //파생상품 코드개편
//	if (m_bData->frts.CompareNoCase(code) == 0)
	{
		codeType = ctFuture;
// 		idx = findMData(code);
	}
	else if (m_bData->krts.CompareNoCase(code) ==  0)
	{
		codeType = ctKospi;
	}
	else if (m_bData->k200rts.CompareNoCase(code) == 0)
	{
		codeType = ctK200;
	}
	else
	{
		if (code.GetAt(0) != '2' && code.GetAt(0) != '3' && code.GetAt(0) != 'B' && code.GetAt(0) != 'C') return; //파생상품 코드개편
		codeType = ctOption;
		
		idx = findMData(code);
		if (idx == -1) return;
	}

// 	CString strTemp;
// 	strTemp.Format("[KSJ]code[%s] alert[%s] codeType[%d]", alertR->code, (char*)((DWORD*)alertR->ptr[0])[0], codeType);
// 	OutputDebugString(strTemp);

	DWORD* data=nullptr;

	for (int i = alertR->size - 1; i > -1; --i)
	{
		data = (DWORD*)alertR->ptr[i];

		bool bChange = false;
		int ii = 0;
		switch (codeType)
		{
		case ctKospi:
			if (!data[23]) return;		// 현재가
			val = (char*)data[23];
			val.Replace("+", ""); val.Replace("-", "");
			if (m_bData->kcur != atof(val))
			{
				m_bData->kcur = atof(val);
				generateFData();
				bChange = false;
			}
			break;
		case ctK200:
			if (!data[23]) return;		// 현재가
			val = (char*)data[23];
			val.Replace("+", ""); val.Replace("-", "");
			if (m_bData->k200cur != atof(val))
			{
				m_bData->k200cur = atof(val);
				generateBasicPrice();
				generateFData();
				bChange = true;
			}
			break;
		case ctFuture:
			{
				if (!data[23]) return;		// 현재가
				val = (char*)data[23];
				val.Replace("+", ""); val.Replace("-", "");
				m_bData->fcur = atof(val);
				generateBasicPrice();
				generateFData();

				idx = findMData(code);
				if (idx == -1) return;

				double dval = 0;
				struct _mdat* mdat = (struct _mdat*)m_mData.GetAt(idx);
				for (ii = 0; ii < 5; ii++)
				{
					if (data[atoi(mdatsym->mdga[ii])])
					{
						val = (char*)data[atoi(mdatsym->mdga[ii])];
						val.Replace("+", ""); val.Replace("-", "");
						mdat->mdga[ii] = atof(val);
					}

					if (data[atoi(mdatsym->msga[ii])])
					{
						val = (char*)data[atoi(mdatsym->msga[ii])];
						val.Replace("+", ""); val.Replace("-", "");
						mdat->msga[ii] = atof(val);
					}
				}

				if (data[atoi(mdatsym->ccur)])
				{
					val = (char*)data[atoi(mdatsym->ccur)];
					val.Replace("+", ""); val.Replace("-", "");
					dval = atof(val);
					if (dval != mdat->ccur)
					{
						mdat->ccur = dval;
						generateMingam(idx);
					}
				}

				setGridVal(m_pPFGrid.get(), mdat, false);
				for (ii = 0; ii < m_mData.GetSize(); ii++)
					generateMingam(ii);
				generateSum();
			}
			break;
		case ctOption:
			{
				double dval = 0;
				struct _mdat* mdat = (struct _mdat*)m_mData.GetAt(idx);
				for (int ii = 0; ii < 5; ii++)
				{
					if (data[atoi(mdatsym->mdga[ii])])
					{
						val = (char*)data[atoi(mdatsym->mdga[ii])];
						val.Replace("+", ""); val.Replace("-", "");
						mdat->mdga[ii] = atof(val);
					}

					if (data[atoi(mdatsym->msga[ii])])
					{
						val = (char*)data[atoi(mdatsym->msga[ii])];
						val.Replace("+", ""); val.Replace("-", "");
						mdat->msga[ii] = atof(val);
					}
				}
				
				if (data[atoi(mdatsym->ccur)])
				{	
					val = (char*)data[atoi(mdatsym->ccur)];
					val.Replace("+", ""); val.Replace("-", "");
					dval = atof(val);
					if (dval != mdat->ccur)
					{
						mdat->ccur = dval;
						generateMingam(idx);
					}
				}

				setGridVal(m_pPFGrid.get(), mdat, false);
				generateSum();
			}
			break;
		}

		if (bChange)
		{
			for (ii = 0; ii < m_mData.GetSize(); ii++)
				generateMingam(ii);
			generateSum();
		}
	}
}

void CMapWnd::readCfg()
{
	m_jjIdx  = atoi(m_cfg->ReadCfg("RDType"));
	m_idxVar = atoi(m_cfg->ReadCfg("BPType"));
	m_timeOpt	= atoi(m_cfg->ReadCfg("TimeValue"));
	m_timeStart	= m_cfg->ReadCfg("TimeStart");
	m_timeEnd	= m_cfg->ReadCfg("TimeEnd");

	CString str;
	str.Format("HV(%d)", atoi(m_cfg->ReadCfg("HVDay")));
	m_static[6]->SetText(str);
}

void CMapWnd::generateBasicPrice()
{
	switch (atoi(m_cfg->ReadCfg("BPType")))
	{
	case 1:		// 선물최근월물
		m_bData->basicprc = fabs(m_bData->fcur);
		break;
	case 2:		// 가중
		{
			double krate = atof(m_cfg->ReadCfg("K200Rate"));
			m_bData->basicprc = ((fabs(m_bData->k200cur) - m_bData->bdjs) * krate + fabs(m_bData->fcur) * (100 - krate)) / 100;
		}
		break;
	case 3:		// 자동계산
		m_bData->basicprc = fabs(m_bData->fcur) * exp(-1 * (m_bData->cdgl / 100) * (m_bData->jjil[1] / 365));
		break;
	default:	// 배당지수
		m_bData->basicprc = fabs(m_bData->k200cur) - m_bData->bdjs;
		break;
	}
	
}

void CMapWnd::generateFData()
{
	CString val;
	for (int ii = 0; ii < sizeof(staticX) / sizeof(staticX[0]); ii++)
	{
		val.Empty();
		switch (staticX[ii].dataIdx)
		{
		case datBasicPrice:
			val.Format("%.2f", m_bData->basicprc);
			break;
		case datK200:
			val.Format("%.2f", m_bData->k200cur);
			break;
		case datFuture:
			val.Format("%.2f", m_bData->fcur);
			break;
		case datHV:
			switch(atoi(m_cfg->ReadCfg("HVDay")))
			{
			case 90:
				val.Format("%.2f", m_bData->hv[0]);
				break;
			case 60:
				val.Format("%.2f", m_bData->hv[1]);
				break;
			case 30:
				val.Format("%.2f", m_bData->hv[2]);
				break;
			case 21:
			default: 
				val.Format("%.2f", m_bData->hv[3]);
				break;
			}
			break;
		case datCdgl:
			val.Format("%.2f", m_bData->cdgl);
			break;
		case datBdjs:
			val.Format("%.4f", m_bData->bdjs);
			break;
		default:
			continue;
		}
		m_static[ii]->SetText(val);
	}
}

void CMapWnd::generateMingam(int idx)
{
	if (idx == -1) return;

	struct  _mdat *mdat = m_mData.GetAt(idx);

	bool bMini = false;

	if(mdat->ccod.Mid(1, 2) == "05")//2015.07.07 KSJ 미니 코스피200 선물
		bMini = true;


	// 민감도 계산(iv, delta, gamma, theta, vegga, rho)
	if (mdat->ccod[0] == '1' || mdat->ccod[0] == '4' || mdat->ccod[0] == 'A' || mdat->ccod[0] == 'D') //파생상품 코드개편
	{
		// 선물 민감 계산
		mdat->iv = "0";
		mdat->delta = bMini ? 0.2:1;	//2012.10.12 KSJ 선물과 옵션이 동일하게 50만원이므로.. *5해주지 않음 2015.07.07 KSJ 미니 선물은 /5의 가격
		mdat->gamma = 0;
		mdat->theta = 0;
		mdat->vegga = 0;
	}
	else
	{
		CString code, val, opt;
		code.Format("%s", mdat->ccod);
		const double strike = m_pApp->getOptionStrike(code);

		m_opsenscal->SetJCode(code);
		val.Format("%.2f", mdat->ccur);		m_opsenscal->SetCurrValue(val);
		val.Format("%.1f", strike);		m_opsenscal->SetStrike(val);
		val.Format("%.2f", m_bData->cdgl);	m_opsenscal->SetCd(val);
		val.Format("%.2f", fabs(m_bData->k200cur));	m_opsenscal->SetK200Cur(val);
		val.Format("%.2f", fabs(m_bData->fcur));	m_opsenscal->SetFutureCur(val); 

		val.Format("%.6f", m_bData->bdjs);	m_opsenscal->SetBedangJisu(val);
		val.Format("%.2f", mdat->jjil[0]);	m_opsenscal->SetBzRemainDays(val);
		val.Format("%.2f", mdat->jjil[1]);	m_opsenscal->SetCalRemainDays(val);
		m_opsenscal->SetTime(m_bData->time);
		m_opsenscal->Calc(0);

		mdat->iv = m_opsenscal->GetIv();
		//2015.07.10 KSJ 델타만 1/5해준다
		mdat->delta = bMini ? atof(m_opsenscal->GetDelta()) / 5.0:atof(m_opsenscal->GetDelta());
		mdat->gamma = atof(m_opsenscal->GetGamma());
		mdat->theta = atof(m_opsenscal->GetTheta());
		mdat->vegga = atof(m_opsenscal->GetVegga());
	}
	setGridVal(m_pPFGrid.get(), mdat, false);
}

void CMapWnd::generateSum()
{
	int nChk=0, idx=0;
	struct _gdat	*gdat = nullptr;
	struct _mdat	*mdat = nullptr;
	CfxGrid		*maingrid = nullptr;
	CArray<struct _gdat*, struct _gdat*> *mainArray = nullptr;
	maingrid = m_pPFGrid.get();
	mainArray = &m_PFCodes;

	m_allSum.prc = 0;	// 소요자금
	m_allSum.jango = 0;	// 옵션매도수량
	m_allSum.gb = 0;	// 옵션매수수량
	m_allSum.sonik	= 0;
	m_allSum.delta = 0;
	m_allSum.gamma = 0;
	m_allSum.theta = 0;
	m_allSum.vegga = 0;
	for (int ii = 0; ii < mainArray->GetSize(); ii++)
	{
		nChk = maingrid->GetItemParam(ii+1, gSelect);
		if (nChk != 1) continue;

		gdat = mainArray->GetAt(ii);
		idx = findMData(gdat->ccod);
		if (idx == -1) continue;
		mdat = m_mData.GetAt(idx);
		if (gdat->ccod[0] == '2' || gdat->ccod[0] == '3' || gdat->ccod[0] == 'B' || gdat->ccod[0] == 'C') //파생상품 코드개편
		{
			if (gdat->gb == -1)
				m_allSum.jango += gdat->jango;
			else
				m_allSum.gb += gdat->jango;

			if (((gdat->ccod[0] == '2' || gdat->ccod[0] == 'B') && gdat->gb == 1) || ((gdat->ccod[0] == '3' || gdat->ccod[0] == 'C') && gdat->gb == -1)) //파생상품 코드개편
				m_allSum.prc += mdat->ccur * gdat->jango * gdat->gdan;
			else
				m_allSum.prc += mdat->ccur * gdat->jango * gdat->gdan * -1;
		}
		else
		{
			m_allSum.prc += mdat->ccur * gdat->jango * gdat->gdan * GUARANTEE_RATE * gdat->gb;
		}
		
		m_allSum.sonik	+= gdat->sonik;
		m_allSum.delta += gdat->delta;
		m_allSum.gamma += gdat->gamma;
		m_allSum.theta += gdat->theta;
		m_allSum.vegga += gdat->vegga;
	}

	InvalidateSum();
}

void CMapWnd::setGridVal(class CfxGrid *grid, struct _mdat* mdat, bool init)
{
	int row;
	row = findGrid(grid, mdat->ccod);

	CArray<struct _gdat*, struct _gdat*> *gArray = &m_PFCodes;	//(grid == m_pJangoGrid) ? &m_JangoCodes : 
	if (row < 1 || gArray->GetSize() < row) return;

	if (init) initGridRow(grid, row);

	CString val;
	struct _gdat *gdat;
	gdat = gArray->GetAt(row -1);

	// "현재가란 표시용" [현재가]/[상대호가...]/[만기일계산현재가]등을 표시할 수 있다
	switch (m_currMode)
	{
	case cmCurr:
		gdat->curcalc = fabs(mdat->ccur);
		break;
	case cmOp1:
	case cmOp2:
		{
			const int idx = m_currMode - cmOp1;
			if (gdat->gb == 1)
				gdat->curcalc = fabs(mdat->mdga[idx]);
			else
				gdat->curcalc = fabs(mdat->msga[idx]);
		}
		break;
	case cmExpire:
		if (gdat->ccod[0] == '2' || gdat->ccod[0] == 'B')  //파생상품 코드개편
		{
			gdat->curcalc = fabs(m_bData->k200cur) - m_pApp->getOptionStrike(gdat->ccod);
			if (gdat->curcalc < 0) gdat->curcalc = 0;	
		}
		else if (gdat->ccod[0] == '3' || gdat->ccod[0] == 'C')  //파생상품 코드개편
		{
			gdat->curcalc = m_pApp->getOptionStrike(gdat->ccod) - fabs(m_bData->k200cur);
			if (gdat->curcalc < 0) gdat->curcalc = 0;	
		}
		else
		{
			gdat->curcalc = fabs(m_bData->k200cur);
		}
		break;
	}


// 	if (mdat->ccod[4] == '6' && mdat->ccod[0] != '1')	//2012.05.15 KSJ 6월물일경우에는 무조건 10만원 그러나 선물이나 스프레드는 50만원
// 		gdat->sonik = gdat->curcalc * gdat->jango * 100000 - gdat->ordprc * gdat->jango * 100000;
// 	else
// 		gdat->sonik = gdat->curcalc * gdat->jango * 500000 - gdat->ordprc * gdat->jango * 500000;
// 

	//2012.05.18 KSJ 평가손익 = 평가금액 - 매입금액
	double dCal=0;

	if(int(gdat->miga) > 0)
	{
		dCal = gdat->curcalc * gdat->gdan * (double)gdat->jango;
	 	gdat->sonik =  dCal - gdat->miga;
	}
	else
	{
		gdat->sonik = gdat->curcalc * gdat->jango * gdat->gdan - gdat->ordprc * gdat->jango * gdat->gdan;
	}
	//KSJ

// 	CString strTemp;
// 	strTemp.Format("[KSJ][setGridVal]miga[%f]cal[%f]sonik[%.0f]gdat->prc[%f]", gdat->miga, dCal, gdat->sonik, gdat->prc);
// 	OutputDebugString(strTemp);

	gdat->sonik = gdat->sonik * (gdat->gb == 1 ? 1 : -1);

	grid->SetItemText(row, gCode, gdat->ccod);
	val.Format("%s", gdat->gb == 1 ? "+매수" : "-매도");		
	grid->SetItemText(row, gGB, val);	grid->SetItemParam(row, gGB, gdat->gb == -1 ? 0:1);
	val.Format("%d", gdat->jango);		grid->SetItemText(row, gJango, val);

	//2012.05.14 KSJ 여기서 단가는 ordPrc임..
	val.Format("%.2f", gdat->ordprc);		grid->SetItemText(row, gPrc, val);
	if (gdat->sonik > 0)
		val.Format("+%.0f", gdat->sonik);
	else
		val.Format("-%.0f", gdat->sonik);
	
	grid->SetItemText(row, gSonik, val);

	val.Format("%.2f", gdat->curcalc);	grid->SetItemText(row, gCurr, val);

	gdat->delta = mdat->delta * gdat->jango * gdat->gb;
	gdat->gamma = mdat->gamma * gdat->jango * gdat->gb;
	gdat->theta = mdat->theta * gdat->jango * gdat->gb;
	gdat->vegga = mdat->vegga * gdat->jango * gdat->gb;
	grid->SetItemText(row, gIV, mdat->iv);
	val.Format("%.4f", gdat->delta);	grid->SetItemText(row, gDelta, val);
	val.Format("%.4f", gdat->gamma);	grid->SetItemText(row, gGamma, val);
	val.Format("%.4f", gdat->theta);	grid->SetItemText(row, gTheta, val);
	val.Format("%.4f", gdat->vegga);	grid->SetItemText(row, gVegga, val);

	if (gdat->addnew)
	{
		gdat->addnew = 0;
		grid->SetItemParam(row, gSelect, 1);
	}
}

CString CMapWnd::PriceGubun(CString str)
{
	CString rValue = "0";

	if (str.GetLength())
	{
		switch (str[0])
		{
		case '1':
			rValue = "F";
			break;
		case '2':
			rValue = "O";
			break;
		case '3':
			rValue = "O";
			break;
		}
	}

	return rValue;
}

long CMapWnd::OnMsgPrice(WPARAM wParam, LPARAM lParam)
{
// 	OutputDebugString("[KSJ]OnMsgPrice");

	CString price = (char*) wParam;
	Parser(price, '\t');
	CArray<struct _gdat*, struct _gdat*> *gArray = &m_PFCodes;	//(m_priceGrid == m_pJangoGrid) ? &m_JangoCodes : 

	if (m_priceRow > 0 && m_priceGrid != NULL)
	{
		struct _gdat* gdat = gArray->GetAt(m_priceRow - 1);
		gdat->prc = atof(price);

		if(m_prcMode == pmBuyPrc)	//단가(현)일때는 현재가로만 세팅되어 있어야하고 바뀌면 안된다.
			gdat->ordprc = atof(price);	//2014.12.15 KSJ 데이터 받아오면 매입단가쪽을 수정해준다.

		int idx=0;
		struct _mdat* mdat = nullptr;
		idx = findMData(gdat->ccod);
		if (idx < 0)
		{
			generateSum();
			return 0;
		}

		mdat = m_mData.GetAt(idx);
		setGridVal(m_priceGrid, mdat, false);
		generateSum();
		m_priceGrid = NULL;
		m_priceRow = -1;
	}

	return 0;
}

void CMapWnd::SetGraph()
{
	if (m_pSheet1 && IsWindow(m_pSheet1->m_pSonikPage->GetSafeHwnd()))
	{
		// Delete
		((CWnd*)m_pSheet1->m_pSonikPage->m_pGrpWnd.get())->SendMessage(WM_GRPRefresh, DeleteGrpData, 0);

		int nChk{};
		_GraphData GData{};
		struct _gdat* gdat = nullptr;
		struct _mdat* mdat = nullptr;
		CString fistcode("");

		CString strTemp;
// 		strTemp.Format("[KSJ] m_PFCodes.GetSize(%d) m_mData[%d]", m_PFCodes.GetSize(), m_mData.GetSize());
// 		OutputDebugString(strTemp);

		for (int ii = 0; ii < m_PFCodes.GetSize(); ii++)
		{
			if(m_mData.GetSize() <= 0)
				return;

			gdat = m_PFCodes.GetAt(ii);
			mdat = m_mData.GetAt(findMData(gdat->ccod));
			nChk = m_pPFGrid->GetItemParam(ii+1, gSelect);
			if (nChk != 1) continue;
			
			sprintf(GData.code, "%s", gdat->ccod.GetString());	// 종목코드
			GData.curr = mdat->ccur;		// 현재가
			GData.prc  = gdat->ordprc;			// 주문가
			GData.numb = gdat->jango;		// 주문수량
			GData.mmgb = gdat->gb;			// 매매구분(-1. 매도, 1. 매수)
			GData.jjis[0] = mdat->jjil[0];		// 잔존만기일(거래일수)
			GData.jjis[1] = mdat->jjil[1];		// 잔존만기일(달력일수)
			GData.hvbd = m_bData->hv[3];		// 역사적 변동성
//			GData.hvbd = m_bData->hv[4];		// 역사적 변동성
			GData.gdan = gdat->gdan/100000.0;		//2012.05.17 KSJ 거래승수 추가 1배 또는 5배

// 			CString strTemp;
// 			strTemp.Format("[KSJ] SetGraph code[%s] dan[%f]\n" , GData.code, GData.gdan);
// 			OutputDebugString(strTemp);

			if (fistcode.IsEmpty())
				fistcode = GData.code;

			((CWnd*)m_pSheet1->m_pSonikPage->m_pGrpWnd.get())->SendMessage(WM_GRPAddData, 0, (long)(LPTSTR)(LPCTSTR)&GData);
		}

		redrawGraph();

		// 손익챠트를 마우스로 이동/변경하는 기능을 실시간잔고종목이 있는 경우에는 변경할 수 없도록 제한한다.
	
		const bool bMoveGrp = true;
		((CWnd*)m_pSheet1->m_pSonikPage->m_pGrpWnd.get())->SendMessage(WM_GRPRefresh, SetStgMouseMove, (long)bMoveGrp);
	}
}

long CMapWnd::OnMsgLineMove(WPARAM wParam, LPARAM lParam)
{
	_userMsg* muMsg = (_userMsg*) wParam;

	switch (muMsg->kind)
	{
	case LineMovePoint: // POINT
		{
			int nChk=0, idx=0;
			struct _gdat	*pfdat = nullptr;
			CString	newCode, val, cname, code = (char*) lParam;

			newCode = CodePlusGab(code, muMsg->gab);
			if (newCode.GetLength() > 0)
			{
				idx = findGData(&m_PFCodes, code);
				if (idx != -1)
				{
					nChk = m_pPFGrid->GetItemParam(idx+1, gSelect);
					if (nChk != 1) break;

					pfdat = m_PFCodes.GetAt(idx);
					pfdat->ccod.Format("%s", newCode);
					pfdat->ordprc = 0;
					val.Format("%s", newCode);
					cname.Format("%s", (char*)m_pView->SendMessage(WM_USER, MAKEWPARAM(nameDLL, 0), (long)(LPSTR)(LPCTSTR)val));
					m_pPFGrid->SetItemText(idx+1, gCode, newCode);
					m_pPFGrid->SetItemText(idx+1, gName, cname);
				}
				queryMingam();
			}
		}
		break;
	case LineMoveAll: // ALL
		{
			CString newCode, val, cname;
			struct _gdat *gdat = nullptr;
			for (int ii = 0; ii < m_PFCodes.GetSize(); ii++)
			{
				gdat = m_PFCodes.GetAt(ii);
				newCode = CodePlusGab(gdat->ccod, muMsg->gab);
				if (newCode.GetLength())
				{
					gdat->ccod.Format("%s", newCode);				
					gdat->ordprc = 0;
					val.Format("%s", newCode);
					cname.Format("%s", (char*)m_pView->SendMessage(WM_USER, MAKEWPARAM(nameDLL, 0), (long)(LPSTR)(LPCTSTR)val));
					m_pPFGrid->SetItemText(ii+1, gCode, newCode);
					m_pPFGrid->SetItemText(ii+1, gName, cname);
				}
			}
			queryMingam();
		}
		break;
	}

	return 0;
}

long CMapWnd::OnMsgSave(WPARAM wParam, LPARAM lParam)
{
	SaveINI((int) lParam, (char*) wParam);
	UpdateSaveList(m_sSavePath);

	return 0;
}

void CMapWnd::SaveINI(int no, CString name)
{
	CString	strNo;
	CString	file;
	CString sPath;
	sPath.Format("%s\\%s\\%s", m_sRoot, USRDIR, Variant(nameCC, ""));

	strNo.Format("%02d", no);
	file.Format("%s\\%s%s", sPath, STRATEGY_FILE, strNo);

	CString path = sPath;
	CreateDirectory(path, NULL);

	WritePrivateProfileString (SN_NAME, SN_PROJ, name, file);

	CString code, mdms, jqty, jprc, strMsg, strKey;
	for (int ii = 0 ; ii < m_PFCodes.GetSize() ; ii++)
	{
		_gdat *gData = m_PFCodes.GetAt(ii);

		code = gData->ccod;
		mdms.Format("%d", gData->gb);
		jqty.Format("%d", gData->jango);
		jprc.Format("%.2f", gData->prc);
		
		strMsg.Format("%s;%s;%s;%s", code, mdms, jqty, jprc);
		strKey.Format("%02d", ii);

		WritePrivateProfileString (SN_CODE, strKey, strMsg, file);
	}
}

long CMapWnd::OnMsgAddStrategy(WPARAM wParam, LPARAM lParam)
{
	_ProjectCode*	pProjectCode = (_ProjectCode*) wParam;
	_userMsg*	userMsg = (_userMsg*) lParam;

	int nRow=0;
	for (int ii = 0; ii < userMsg->nSize; ii++)
	{
		if (ii != 0)	pProjectCode = pProjectCode + 1;

		// 종목중복제거
		if (findGData(&m_PFCodes, pProjectCode->Code) != -1) continue;

		nRow = m_PFCodes.GetSize() + 1;	
		setPF(nRow, pProjectCode->Code, pProjectCode->mmgb, pProjectCode->Num, pProjectCode->jprc);
	}
	queryMingam();

	
	SetGridMode();
	return 0;
}

long CMapWnd::OnMsgApplyStrategy(WPARAM wParam, LPARAM lParam)
{
	_ProjectCode*	pProjectCode = (_ProjectCode*) wParam;
	_userMsg*	userMsg = (_userMsg*) lParam;

	deleteGrid(m_pPFGrid.get(), 0, true);

	int nRow=0;
	for (int ii = 0; ii < userMsg->nSize; ii++)
	{
		if (ii != 0)	pProjectCode = pProjectCode + 1;

		nRow = m_PFCodes.GetSize() + 1;			
		setPF(nRow, pProjectCode->Code, pProjectCode->mmgb, pProjectCode->Num, pProjectCode->jprc);
	}
	queryMingam();

	SetGridMode();
	return 0;
}

long CMapWnd::OnMapHedgeData(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case IsHedgeable:
		{
			if (m_PFCodes.GetSize() > 0)
				return 1;
			else
				return 0;
		}
		break;
	case GetHedgeTarget:
		{
			m_rtnStr.Format("%f\t%f\t%f\t%f\t%f\t", m_allSum.prc, m_allSum.delta, m_allSum.gamma,
				m_allSum.theta, m_allSum.vegga);
			return (long)(LPCTSTR)m_rtnStr;
		}
		break;
	case GetHedgeData:
		{
			int nMonth=0;
			CString sMonth, ocode;
			struct _mido mid {};
			ZeroMemory(&mid, sizeof(struct _mido));
			mid.func = ' ';

			ocode.Format("%s", FindAtmOption(fabs(m_bData->k200cur)));
			if (ocode.IsEmpty()) return 0;
				
			if (ocode[4] >= 'A')
				nMonth = 10 + (ocode[4] - 'A');
			else
				nMonth = atoi(ocode.Mid(4,1));

			sMonth.Format("%.2d", nMonth);
			CopyMemory((&mid.mymd[0]+2), sMonth, sMonth.GetLength());
			sendTR(TRN_HEDGE, (char *)&mid, sizeof(struct _mido), KEY_HEDGEOPTION, 0);
			return 1;
		}
		break;
	case ReqOrder:
		{
			CString sCodes, sAccount, sPwd;
			sCodes.Format("%s", (char*)lParam);

			sAccount = m_pAccountCbo->GetAccNo(); sAccount.TrimRight();
			if (sAccount.IsEmpty())
			{
				AfxMessageBox("계좌를 선택해주세요.");
				setMessage("계좌를 선택해주세요.");
				Invalidate();
				break;
			}

			m_pPwdEdit->GetWindowText(sPwd); sPwd.TrimRight();
			if (sPwd.IsEmpty() && IsNumber(m_sUser))
			{
				AfxMessageBox("비밀번호를 입력해주세요.");
				setMessage("비밀번호를 입력해주세요.");
				Invalidate();
				break;
			}

			m_rtnStr.Format("%s /t /s /p5 /d%s\t%s\n%s\t%s\n%s\t%s\n", MAP_MULTIORDER, "AN21", sAccount, "PWD", sPwd, "ORDCODE", sCodes);
			m_pView->SendMessage(WM_USER, MAKEWPARAM(viewDLL, typeVIEW), (long)(LPTSTR)(LPCTSTR)m_rtnStr);
		}
		break;
	}
	return 0;
}

void CMapWnd::redrawGraph()
{
	if (IsWindow(m_pSheet1->m_pSonikPage->GetSafeHwnd()))
	{
		((CWnd*)m_pSheet1->m_pSonikPage->m_pGrpWnd.get())->SendMessage(WM_GRPRefresh, RedrawGraph, 0);
	}
}

void CMapWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	if (m_pSheet1->GetSafeHwnd())
	{
		CRect rc, rcClient;
		GetClientRect(&rcClient);
		rc = CRect(0, SHEETTAB_TOP + TAB_HEIGHT, rcClient.right, rcClient.bottom - GUIDEBAR_HEIGHT);
		m_pSheet1->MoveWindow(&rc, TRUE);
	}
}

void CMapWnd::hideWizardBtn()
{
	CWnd* wnd = m_pSheet1->GetTopWindow();
	if (!wnd) return;


	CRect rcWnd;
	CString strName, strClass;
	char	clsName[256]{};
	while (wnd)
	{
		wnd->GetWindowText(strName); strName.TrimRight();

		::GetClassName(wnd->m_hWnd, clsName, 256); 
		strClass.Format("%s", clsName);

		wnd->GetClientRect(&rcWnd);

		if (strName.CompareNoCase("< 뒤로(&B)") == 0)
		{
			wnd->ShowWindow(SW_HIDE);
		}
		else if (strName.CompareNoCase("다음(&N) >") == 0)
		{
			wnd->ShowWindow(SW_HIDE);
		}
		else if (strName.CompareNoCase("취소") == 0)
		{
			wnd->ShowWindow(SW_HIDE);
		}
		else
		{
			if (strClass.CompareNoCase("Static") == 0 && rcWnd.Height() < 5)
				wnd->ShowWindow(SW_HIDE);
		}

		wnd = wnd->GetNextWindow(GW_HWNDNEXT);
	}
}

void CMapWnd::ReArrangeCodes(CfxGrid *pGrid)
{
	m_csSort.Lock();

	int	idx = -1;
	CString code;
	struct _gdat *gdat = nullptr;
	CArray<struct _gdat*, struct _gdat*>* pCodes = nullptr;
	pCodes = &m_PFCodes;

	for (int ii = 1; ii < pGrid->GetRowCount(); ii++)
	{
		code = pGrid->GetItemText(ii, gCode); code.TrimRight();
		if (code.IsEmpty()) break;

		idx = findGData(pCodes, code);
		if (idx != -1 && idx != ii-1)
		{
			gdat = pCodes->GetAt(idx);
			pCodes->RemoveAt(idx);
			pCodes->InsertAt(ii-1, gdat);
		}
	}
	m_csSort.Unlock();
}
void CMapWnd::InvalidateSum()
{
	CRect itemRC;
	m_pPFGrid->GetWindowRect(&itemRC); ScreenToClient(&itemRC);
	itemRC.top = itemRC.bottom + 1; itemRC.bottom = itemRC.top + 40;
	itemRC.left = gridHdr[0].width + gridHdr[1].width + gridHdr[2].width;
	InvalidateRect(itemRC);
}

CString CMapWnd::CodePlusGab(CString code, CString gab)
{
	CString rValue = _T("");

	if (code.GetLength() != 8)
		return rValue;

	if (code[0] == '1' || code[0] == 'A') //파생상품 코드개편
	{
		rValue = code;
	}
	else
	{
		CString strhsga, tmpStr, strCode, name;
		double	fullhsga=0;

		if (code.Right(1) == '2' || code.Right(1) == '7')
			fullhsga = atof(code.Right(3)) + 0.5;
		else
			fullhsga = atof(code.Right(3));

		tmpStr.Format("%.1f", fullhsga + atof(gab));
		strhsga.Format("%03d", atoi(tmpStr.Left(tmpStr.FindOneOf("."))));
		strCode.Format("%s%s", code.Left(5), strhsga);

		tmpStr.Format("%s", strCode);
		name.Format("%s", (char*)m_pView->SendMessage(WM_USER, MAKEWPARAM(nameDLL, 0), (long)(LPSTR)(LPCTSTR)tmpStr));

		if (name.GetLength() > 0)
			rValue = strCode;
		else
			rValue = _T("");
	}

	return rValue;
}

void CMapWnd::UpdateSaveList(CString savePath)
{
	CString	strNo;
	CString	saveFile, readFile;
	saveFile.Format("%s\\%s", savePath, STRATEGY_LIST);

	CString path = savePath;
	CreateDirectory(path, NULL);

	// 삭제
	WritePrivateProfileSection(SN_PROJ, "", saveFile);

	CFileFind finder;
	CHAR inBuf[80]{};
	int ii = 0;
	bool bContinue = TRUE;
	while (bContinue)
	{
		strNo.Format("%02d", ii);
		readFile.Format("%s\\%s%s", savePath, STRATEGY_FILE, strNo);

		if (finder.FindFile(readFile))
		{
			ZeroMemory(inBuf, sizeof(inBuf));
			GetPrivateProfileString (SN_NAME, SN_PROJ, NULL, inBuf, 80, readFile);

			WritePrivateProfileString(SN_PROJ, strNo, inBuf, saveFile);
		}
		else
			bContinue = FALSE;

		ii++;
	}
}

bool CMapWnd::LoadOJCode()
{
	clearOPCodeList();

	char	cMonth;
	CString str;
	str = (char*)m_pView->SendMessage(WM_USER, MAKEWPARAM(codeDLL, MAKEWORD(getCODEcall, 0)), 0);
	cMonth = str[4];

	int	codeN{}, len{};
	CFile	file;
	struct  ojcodh  OJCodh;
	struct  ojcode  OJCode;
	CString path;
	path.Format("%s\\%s\\%s", m_sRoot, TABDIR, OJCODE);

	if (!file.Open(path, CFile::modeRead|CFile::typeBinary))
	{	
		AfxMessageBox("선물/옵션 파일을 읽을 수 없습니다.");
		setMessage("선물/옵션 파일을 읽을 수 없습니다.");
		Invalidate();
		return false;
	}

	len = file.Read(&OJCodh, sizeof(struct ojcodh));
	codeN = (int)((file.GetLength() - len) / sizeof(struct ojcode));

	for (int ii = 0; ii < 11; ii++)
	{
		CString date = CString(OJCodh.pjym[ii], sizeof(OJCodh.pjym[ii]));
		if (date.GetLength() > 5)
			date.Insert(4, '.');
		sprintf(m_pOPMonthInfo[ii].name, "%s", date.GetString());
	}

	CString	sPrice;
	gsl::owner <_codelist*> CodeList{};
	//_strikelist	StrikeList;		
	for (int ii = 0; ii < codeN; ii++)
	{
		file.Read(&OJCode, sizeof(struct ojcode));
		sPrice = CString(OJCode.price, sizeof(OJCode.price));
		for (int jj = 0; jj < 11; jj++)
		{
			if (OJCode.call[10-jj].yorn == '1')
			{
				if (OJCode.call[10-jj].cod2[4] == cMonth)
				{
					CodeList = new _codelist;
					CodeList->code = CString(OJCode.call[10-jj].cod2, OCodeLen);
					CodeList->name = CString(OJCode.call[10-jj].hnam, ONameLen);
					CodeList->mdMargin = 1;
					m_OPCodeList.Add(CodeList);
				}
			}
		}
	}

	file.Close();

	return true;
}

CString CMapWnd::FindAtmOption(double jisu)
{
	CString stmp;
	stmp.Format("IB410100 FindAtmOption [%f]", jisu);
OutputDebugString(stmp);
	char	cMonth;
	CString str;
	str = (char*)m_pView->SendMessage(WM_USER, MAKEWPARAM(codeDLL, MAKEWORD(getCODEcall, 0)), 0);
	cMonth = str[4];
OutputDebugString("IB410100   "+str);
	CString atm, ym;
	CString strike, opcode;

	m_atmCall.Empty();
	ym = m_fcode.Mid(3, 2);
	atm = m_pApp->JisuToStrike(jisu);
	atm.Format("%.0f", floor(atof(atm)));
	for (int ii = 0; ii < m_OPCodeList.GetSize(); ii++)
	{
		opcode = m_OPCodeList.GetAt(ii)->code;
		if (opcode.Left(1) == "2" && opcode[4] == cMonth && opcode.Mid(5, 3) == atm)
		{
			m_atmCall = opcode;
			break;
		}
	}
	return m_atmCall;
}

void CMapWnd::clearOPCodeList()
{
	const int	nSize = m_OPCodeList.GetSize();
	for (int ii = 0; ii < nSize; ii++)
	{
		_codelist *codelist = m_OPCodeList.GetAt(ii);
		delete codelist;
	}
	m_OPCodeList.RemoveAll();
}

void CMapWnd::GetHedgeCode(char month)
{
	CStringArray	CallList, PutList;
	m_HedgeCodeList.RemoveAll();	CallList.RemoveAll();	PutList.RemoveAll();
	for (int ii = 0; ii < m_OPCodeList.GetSize(); ii++)
	{
		_codelist	*codelist = m_OPCodeList.GetAt(ii);

		if (codelist->code.GetAt(4) == month)
		{
			if (codelist->code.GetAt(0) == '2' || codelist->code.GetAt(0) == 'B')  //파생상품 코드개편
				CallList.Add(codelist->code);
			else
				PutList.Add(codelist->code);
		}
	}

	m_HedgeCodeList.Add(m_fcode);
	for (int ii = 0; ii < CallList.GetSize(); ii++)
		m_HedgeCodeList.Add(CallList.GetAt(ii));
	for (int ii = 0; ii < PutList.GetSize(); ii++)
		m_HedgeCodeList.Add(PutList.GetAt(ii));
}


UINT RunExcelThread(LPVOID lpvoid)
{
	CMapWnd* pMapWnd = (CMapWnd *) lpvoid;

	pMapWnd->m_syncx.Lock();
	pMapWnd->RunExcel_();
	pMapWnd->m_syncx.Unlock();
	return 0;
}

void CMapWnd::RunExcel(bool visible)
{
	m_syncx.Lock();

	m_excelF = visible;
	m_excelS = _T("");

	CString	text = _T(""), temps;


	UINT attr=0;
	int row =0, col = 0;

	for (row = 0; row < m_pPFGrid->GetRowCount(); row++)
	{
		for (col = 0; col < m_pPFGrid->GetColumnCount(); col++)
		{
			text = m_pPFGrid->GetItemText(row, col);
			attr = m_pPFGrid->GetItemAttr(row, col);
			if (attr & GVAT_HIDDEN)
				continue;
			if (text.GetLength() > 0)
			{
				if (attr & GVAT_CONTRAST)
				{
					switch (text[0])
					{
					case '1':		temps = ARR_UP; break;
					case '2': case '+':	temps = TRI_UP; break;
					case '4':		temps = ARR_DN; break;
					case '5': case '-':	temps = TRI_DN; break;
					default:		temps = _T("  "); break;
					}
					text = text.Mid(1); text.Insert(0, temps);
				}

				if (col != 9 && attr & GVAT_CONDITIONx)
				{
					if(text.Find("매수") < 0 && text.Find("매도") < 0)
					{
						text.Remove('+'); text.Remove('-');
					}
					else
					{
						if(text.FindOneOf("+-") > -1)
						{
							text = text.Mid(1);
						}
					}
				}
				else
				{
					if(text.Find("--") > -1)
					{
						text = text.Mid(1);
					}
				}
			}
			m_excelS += text; m_excelS += "\t";
		}
		m_excelS += "\n";
	}

	CString val;
	// 잔고&포트폴리오합계
	text = m_allSum.ccod + "\t\t";
	text += "옵션 매도\t";
	val.Format("%4d", m_allSum.jango);	text += val + "\t";
	text += "옵션 매수\t";
	val.Format("%4d", m_allSum.gb);	text += val + "\t\t\t\t";
	val.Format("%.0f", m_allSum.sonik);	text += val + "\t\t";
	val.Format("%.4f", m_allSum.delta);	text += val + "\t";
	val.Format("%.4f", m_allSum.gamma);	text += val + "\t";
	val.Format("%.4f", m_allSum.theta);	text += val + "\t";
	val.Format("%.4f", m_allSum.vegga);	text += val + "\t";
	text += "\n";
	m_excelS += text;
	
	m_syncx.Unlock();

	m_excelH = AfxBeginThread(RunExcelThread, this, THREAD_PRIORITY_HIGHEST);
}

void CMapWnd::DrawRoundBitmap(CDC *pDC, CRect rc, int nKind)
{
	CRect bitRc;
	
	switch(nKind)
	{
	case ROUND_PANEL:
		//left + top
		bitRc.SetRect(rc.left, rc.top, rc.left + 5, rc.top + 5);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "0_lt.bmp");
		
		//right + top
		bitRc.SetRect(rc.right - 5, rc.top, rc.right, rc.top + 5);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "0_rt.bmp");
		
		//left + bottom
		bitRc.SetRect(rc.left, rc.bottom - 5, rc.left + 5, rc.bottom);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "0_lb.bmp");
		
		//right + bottom
		bitRc.SetRect(rc.right - 5, rc.bottom - 5, rc.right, rc.bottom);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "0_rb.bmp");
		break;
	case ROUND_CONTENTS:
		//left + top
		bitRc.SetRect(rc.left, rc.top, rc.left + 5, rc.top + 5);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "1_lt.bmp");
		
		//right + top
		bitRc.SetRect(rc.right - 5, rc.top, rc.right, rc.top + 5);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "1_rt.bmp");
		
		//left + bottom
		bitRc.SetRect(rc.left, rc.bottom - 5, rc.left + 5, rc.bottom);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "1_lb.bmp");
		
		//right + bottom
		bitRc.SetRect(rc.right - 5, rc.bottom - 5, rc.right, rc.bottom);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "1_rb.bmp");
		break;
	}
}

CString CMapWnd::GetPalette()
{
	char buff[10];
	CString strPal;
	CString file;
	file.Format("%s\\%s\\%s", m_sRoot, "tab", "palette.ini");
	
	GetPrivateProfileString("General", "Palette", "", buff, sizeof(buff), file);
	strPal = buff;
	
	return strPal;
}

void CMapWnd::DrawBitmap(CDC *pDC, CRect rc, CString strImage)
{
	CString strPath = Variant(homeCC, "");
	CString sImageDir = strPath + "\\image\\" + strImage;
	
	HBITMAP BitMap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), sImageDir, IMAGE_BITMAP,0,0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	CDC		memDC;
	
	memDC.CreateCompatibleDC(pDC);
	
	if (memDC.m_hDC != NULL) 
		memDC.SelectObject(BitMap);
	
	if (memDC.m_hDC != 0)
		::TransparentBlt(pDC->m_hDC, rc.left, rc.top, rc.Width(), rc.Height(), memDC.m_hDC, 0, 0, rc.Width(), rc.Height(), 0xFF00FF);
	
	DeleteObject(BitMap);
	memDC.DeleteDC();
}

bool CMapWnd::RunExcel_()
{
	if (m_excelS.IsEmpty())
		return false;

	::OleInitialize(NULL);

	_Application	excel;
	if (!excel.CreateDispatch("Excel.Application"))
	{
		TRACE("CreateDispatch error....\n");
		::OleUninitialize();
		return true;
	}

	COleVariant vOpt((long)DISP_E_PARAMNOTFOUND, VT_ERROR);

	Workbooks  Wbs; _Workbook  Wb;
	Worksheets Wss; _Worksheet Ws;
	Range	range;

	Wbs = excel.GetWorkbooks();
	Wb  = Wbs.Add(vOpt);

	Wss = Wb.GetWorksheets();
	Ws  = Wss.Add(vOpt, vOpt, vOpt, vOpt);

	Ws.SetName("Document");

	if (m_excelS.IsEmpty())
	{
		excel.ReleaseDispatch();
		::OleUninitialize(); m_excelH = NULL;
		return false;
	}

	int nfixedRows=0, nfixedCols=0, nRows=0, nCols=0;
	const	int alphaC = 24;
	CString	string;

	nfixedRows = m_pPFGrid->GetFixedRowCount();
	nfixedCols = m_pPFGrid->GetFixedColumnCount();
	nRows = m_pPFGrid->GetRowCount()+2;	// 2:하단2줄 합계
	nCols = m_pPFGrid->GetColumnCount();
	
	string.Format("A1:%c%d", (char)('A' + nCols), nfixedRows + nRows);
	if (nCols >= alphaC)
		string.Format("A1:%c%c%d", (char)('A' + ((nCols/alphaC) -1)),
					(char)('A' + nCols % alphaC), nfixedRows + nRows);

	range = Ws.GetRange(COleVariant((LPCTSTR)string), vOpt);
	range.Clear();

	if (m_excelS.IsEmpty())
	{
		excel.ReleaseDispatch();
		::OleUninitialize(); m_excelH = NULL;
		return false;
	}

	UINT attr=0;
	for (int jj = 0; nfixedRows && jj < nCols; jj++)
	{
		attr = m_pPFGrid->GetItemAttr(nfixedRows, jj);
		
		if (!(attr & GVAT_TEXT))
			continue;

		string.Format("%c%d:%c%d", (char)('A' + jj), nfixedRows, (char)('A' + jj), nRows);
		if (jj >= alphaC)
		{
			string.Format("%c%c%d:%c%c%d", (char)('A' + ((jj / alphaC)-1)),
							(char)('A' + jj % alphaC), nfixedRows,
							(char)('A' + ((jj / alphaC)-1)),
							(char)('A' + jj % alphaC), nRows);
		}

		Range rangex = Ws.GetRange(COleVariant((LPCTSTR)string), vOpt);
		string = _T("@"); rangex.SetNumberFormat(COleVariant(string));
	}

	long row{}, col{};
	bool last = false;
	int idx{}; 
	CString stringX;
	for (int ii = 0; !m_excelS.IsEmpty() && ii < nRows; ii++)
	{
		idx = m_excelS.Find("\n");
		if (idx != -1)
		{
			stringX = m_excelS.Left(idx);
			m_excelS = m_excelS.Mid(idx+1);
		}
		else
		{
			stringX = m_excelS; m_excelS = _T(""); last = true;
		}

		for (int jj = 0; !stringX.IsEmpty() && jj < nCols; jj++)
		{
			idx = stringX.Find("\t");
			if (idx != -1)
			{
				string  = stringX.Left(idx);
				stringX = stringX.Mid(idx+1);
			}
			else
			{
				string = stringX; stringX = _T("");
			}

			if (string.IsEmpty())
				continue;

			TRY
			{
				row = ii+1; col = jj+1;
				range.Set_Default(COleVariant(row), COleVariant(col), COleVariant((LPCTSTR)string));
			}
			CATCH(COleDispatchException, e)
			{
				TRACE("runexcel error [%s]\n", e->m_strDescription);
				break;
			}
			END_CATCH
		}

		if (!Ws.m_lpDispatch) break;
	}

	excel.SetVisible(m_excelF);
	if (!m_excelF)
	{
		Wbs.Close(); excel.Quit();
	}

	excel.ReleaseDispatch();
	::OleUninitialize();

	m_excelH = NULL;
	return true;
}

void CMapWnd::queryDeposit(BOOL bALL, CString k200Guess)
{
	if (!m_pAccountCbo || !m_pPwdEdit)	return;

	if (IsWindow(m_pSheet1->m_pSonikPage->GetSafeHwnd()))
		(m_pSheet1->m_pSonikPage)->SendMessage(WM_POSCLEAR, 0, 0);

	CString dataS = "", dataS2 = "";
	CString tmpS;
	// 계좌번호
	CString accS = m_pAccountCbo->GetAccNo(); 
	accS.TrimRight();
	if (accS.GetLength() < 8)		return;
	accS = accS.Left(8);
	dataS += accS + "\t";
	dataS2 += accS + "\t";

	// 상품구분
	dataS += "21\t";
	dataS2 += "21\t";

	// 비밀번호
	CString passS, passX;
	m_pPwdEdit->GetWindowText(passS);
	passS.TrimRight();
	if (passS.IsEmpty() && IsNumber(m_sUser))	
	{
		AfxMessageBox("비밀번호를 입력하세요.");	
		setMessage("비밀번호를 입력하세요.");
		Invalidate();
		return;
	}
	passX.Format("%s\t%s", passS, accS);
	passX = (char*)m_pView->SendMessage(WM_USER, MAKEWPARAM(encPASSx, modeACN), (long)passX.operator LPCTSTR());
	dataS += passX + "\t";
	dataS2 += passX + "\t";

	// 예상k200
	dataS += "0000000\t";				// 주문체결후 증거금은 k200없음

	//if (atof(k200Guess) == 0.0)
	//	k200Guess = m_bData->k200cur;
	tmpS.Format("%07d", int(atof(k200Guess) * 100));
	dataS2 += tmpS + "\t";				// 익일추정 증거금


	// 조회 code
	int nCnt = 0, nChk = 0;
	for (int ii = 0; ii < m_PFCodes.GetSize(); ii++)
	{
		nChk = m_pPFGrid->GetItemParam(ii+1, gSelect);
		if (nChk != 1) continue;
		nCnt++;
	}

	if (nCnt == 0)
	{
		AfxMessageBox("선택된 종목이 없습니다.");	
		setMessage("선택된 종목이 없습니다.");	
		Invalidate();
		return;
	}

	CString sCode, sGb, sQty, sPrice;
	CString rowS;
	struct _gdat* gdat = nullptr;

	for (int ii = 0; ii < m_PFCodes.GetSize(); ii++)
	{
		nChk = m_pPFGrid->GetItemParam(ii+1, gSelect);
		if (nChk != 1) continue;

		gdat = m_PFCodes.GetAt(ii);

		sCode = gdat->ccod;
		
		sPrice.Format("%.2f", gdat->prc);
		sGb = (gdat->gb == -1) ? "1" : "2";	// 1:매도,2:매수
		if (sGb == "1")
			sQty.Format("-%d", gdat->jango);
		else
			sQty.Format("%d", gdat->jango);

		rowS.Format("%s\t%s\t%s\t%s\t1\t%c", sCode, sGb, sQty, sPrice, 0x0A);	// 1:지정가
		dataS += rowS;
		dataS2 += rowS;
	}

	dataS += "\n";// 0x0D;
	dataS2 += "\n";// 0x0D;

	//KEY_DEPOSIT
	sendLedgerTR('1', "SC264202_Q", (char *)(LPCTSTR)dataS, dataS.GetLength(), KEY_DEPOSIT, US_ENC, "3101");

	if (bALL)
		sendLedgerTR('1', "SC264203_Q", (char *)(LPCTSTR)dataS2, dataS2.GetLength(), KEY_DEPGUESS, US_ENC, "3101");
	
}

COLORREF CMapWnd::GetAxColor(int iColor)
{
	if (iColor & 0x02000000)
		return iColor;
	return m_pView->SendMessage(WM_USER, MAKEWPARAM(getPALETTE, 0), (LPARAM)iColor);
}


void CMapWnd::OnTimer(UINT nIDEvent) 
{
	CWnd::OnTimer(nIDEvent);
}

HBRUSH CMapWnd::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
// TODO: Return a different brush if the default is not desired
	HBRUSH hbr = CWnd::OnCtlColor(pDC, pWnd, nCtlColor);
	pDC->SetBkMode(TRANSPARENT);
	
	return hbr;
}


void CMapWnd::initFOMultiSel(CRect rect)
{
	if (m_hInst) return;
	
	CString dllPath = CString(Variant(homeCC, "")) + CString("\\dev\\CX_FOMultiSel.DLL");
	
	m_hInst = LoadLibrary((LPCSTR)dllPath);
	if (!m_hInst)
	{
		MessageBox("종목컨트롤 생성 실패1");
		return;
	}
	
	CWnd* (APIENTRY *axCreate)(CWnd *, void *);
	axCreate = (CWnd*(APIENTRY*)(CWnd*,void*))GetProcAddress(m_hInst, "axCreate");
	if (!axCreate)
	{
		MessageBox("종목컨트롤 생성 실패2");
		return;
	}
	
	struct _param param;
	GetClientRect(&param.rect);
	param.rect = rect;
	
	param.name  = _T("SelDialog");
	param.fonts = "굴림체";
	param.point = 9;
	param.style = 0;
	param.tRGB  = 63;
	param.pRGB  = 130;
	param.options = "";
	param.key   = 9999;
	
	m_pwndFOMultiSel = axCreate(m_pView, (LPVOID)&param);
}

CString CMapWnd::GetCtrlProperty( LPCSTR prop_name )
{
	USES_CONVERSION_CONST;
	if (!m_pwndFOMultiSel) return "";
	if (!m_pwndFOMultiSel->GetSafeHwnd()) return "";
	
	IDispatch *pDisp = m_pwndFOMultiSel->GetIDispatch(FALSE);
	if (!pDisp) return "";
	
	_variant_t var;
	CComDispatchDriver driver(pDisp);
	driver.GetPropertyByName(_bstr_t(prop_name), &var);
	
	return (LPCSTR)(_bstr_t)var;
}
