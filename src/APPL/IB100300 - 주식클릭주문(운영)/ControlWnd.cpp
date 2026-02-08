// ControlWnd.cpp : implementation file
//

#include "stdafx.h"
#include "ControlWnd.h"
#include "3DEdit.h"
#include "SpinCtrl.h"
#include "CodeReg.h"
#include "MultiJumunDlg.h"
#include "ContainerWnd.h"
#include "Tool.h"
#include "UserDefine.h"
#include "../../control/fx_grid/memdc.h"
#include "../../control/fx_misc/fxImgButton.h"
#include "../../h/AxisVar.h"
#include "../../h/axisfire.h"
#include <math.h>
#include "MarketConfirm.h"
#include "ConfigDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ID_CTRL_ACCOUNT	0x01
#define ID_CTRL_CHART	0x02
#define ID_CTRL_MDCAN	0x03
#define ID_CTRL_MSCAN	0x04
#define ID_EDIT_JMCN	0x05
#define ID_SPIN_JMCN	0x06
#define ID_CTRL_JMCN	0x07
#define ID_EDIT_MONEY	0x08
#define ID_COMBO_MULTI	0x09
#define ID_EDIT_TICK	0x0a
#define ID_SPIN_TICK	0x0b
#define ID_WND_CODEGRID	0x0c
#define ID_EDIT_JMDG	0x0d
#define ID_CTRL_CODEREG	0x0e
#define ID_CTRL_MAXMS	0x0f
#define ID_CTRL_REVAL	0x10
#define IDC_CHK_MARKET	0x11
#define ID_CTRL_CONAMT	0x12	//2015.03.16 KSJ 매매금액
#define ID_COMBO_CRD	0x13

/////////////////////////////////////////////////////////////////////////////
// CControlWnd

//CControlWnd::CControlWnd(CWnd* pParent, CWnd* pView, class CTool* pTool)
CControlWnd::CControlWnd(CWnd* pwndView, class CTool* pTool)
{
//	m_pParent	= (CMapWnd*)pParent;
	m_pwndView	= pwndView;
	m_pTool		= pTool;

	m_pfont		= NULL;
	m_pfontB	= NULL;

	m_ppenLine	= NULL;

	m_pBtnAccount	= NULL;
	m_pBtnChart	= NULL;
	m_pBtnRevaluate	= NULL;
	m_pBtnMDCan	= NULL;
	m_pBtnMSCan	= NULL;
	m_pBtnMaxMS	= NULL;
	m_pBtnJmcn	= NULL;
	m_pBtnConAmt = NULL;

	m_pEditJmdg	= NULL;
	m_pEditJmcn	= NULL;
	m_pEditMoney	= NULL;
	m_pEditTick	= NULL;
	m_pSpinJmcn	= NULL;
	m_pSpinTick	= NULL;
	m_pcontwndAmount	= NULL;
	m_pCodeReg	= NULL;
	m_pAmount	= NULL;
	m_iJmdw		= 1;

	m_clrBG		= RGB(255, 255, 255);
	m_clrDT		= RGB(255, 255, 255);
	m_clrTFG	= RGB(0, 0, 0);

	m_strPgdg		= "";
	m_bMarketConfirm = TRUE;

	m_pTooltip	= NULL;
}	

CControlWnd::~CControlWnd()
{
/*
	if (m_pBtnAccount)
		delete m_pBtnAccount;
	if (m_pBtnChart)
		delete m_pBtnChart;
	if (m_pBtnRevaluate)
		delete m_pBtnRevaluate;
	if (m_pBtnJmcn)
		delete m_pBtnJmcn;
	if (m_pBtnMSCan)
		delete m_pBtnMSCan;
	if (m_pBtnMDCan)
		delete m_pBtnMDCan;
	if (m_pBtnConAmt)
		delete m_pBtnConAmt;

	if (m_pEditJmdg)
		delete m_pEditJmdg;
	if (m_pEditJmcn)
		delete m_pEditJmcn;
	if (m_pEditMoney)
		delete m_pEditMoney;
	if (m_pEditTick)
		delete m_pEditTick;

	if (m_pSpinJmcn)
		delete m_pSpinJmcn;
	if (m_pSpinTick)
		delete m_pSpinTick;

*/
	if (m_pCodeReg)
		delete m_pCodeReg;
	if (m_pAmount)
	{
		m_pAmount->DestroyWindow();
		delete m_pAmount;
	}
}


BEGIN_MESSAGE_MAP(CControlWnd, CWnd)
	//{{AFX_MSG_MAP(CControlWnd)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(ID_CTRL_ACCOUNT, OnButtonAccount)
	ON_BN_CLICKED(ID_CTRL_CHART, OnButtonChart)
	ON_BN_CLICKED(ID_CTRL_REVAL, OnButtonReval)
	ON_BN_CLICKED(ID_CTRL_MDCAN, OnButtonMDCan)
	ON_BN_CLICKED(ID_CTRL_MSCAN, OnButtonMSCan)
	ON_BN_CLICKED(ID_CTRL_JMCN, OnButtonJmcn)
	ON_BN_CLICKED(ID_CTRL_MAXMS, OnButtonMaxMS)
	ON_BN_CLICKED(ID_CTRL_CONAMT, OnButtonConAmt)
	ON_WM_CREATE()
	ON_WM_RBUTTONUP()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER + 200, OnContainerAmount)
	ON_EN_UPDATE(ID_EDIT_JMCN, OnChangeJmcn)
END_MESSAGE_MAP()



LRESULT CControlWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch (message)
	{
	case WM_LBUTTONDOWN:
	case WM_MOUSEMOVE:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONUP:
		{
			if (!m_pTooltip)
				break;
			
			MSG	msg{};
			POINT	point;
			
			point.x = LOWORD(lParam);
			point.y = HIWORD(lParam);
			
			msg.hwnd = m_hWnd;
			msg.message = message;
			msg.wParam = wParam;
			msg.lParam = lParam;
			msg.pt = point;
			
			if(m_tRc4.PtInRect(point))	
				m_pTooltip->RelayEvent(&msg);
		}
		break;
	default:
		break;
	}
	
	return CWnd::WindowProc(message, wParam, lParam);
}

/////////////////////////////////////////////////////////////////////////////
// CControlWnd message handlers
int CControlWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	m_sDept = m_pTool->Variant(deptCC);
	// TODO: Add your specialized creation code here
	Init();
	
	return 0;
}

void CControlWnd::Init()
{
	m_clrBG = m_pTool->GetIndexColor(CONTENTS_BACK_INDEX);
	m_clrDT	= m_pTool->GetIndexColor(COLOR_DATA);
	m_clrTFG = m_pTool->GetIndexColor(COLOR_TEXT);

	m_ppenLine = m_pTool->RequestPen(m_pTool->GetIndexColor(COLOR_LINE), 1, PS_SOLID);

	m_pfont = m_pTool->RequestFont("굴림체", 9, false, 0);
	m_pfontB = m_pTool->RequestFont("굴림체", 9, false, FW_BOLD);

	m_text1 = "매도가능";
	m_text2 = "매수가능";
	m_text3 = "주문가격";
	m_text4 = "매매금액";

	m_strJangoS = "잔    고";
	m_text5 = "평균단가";
	m_text6 = "평가손익";
	m_text7 = "수량 ";
	m_text8 = "예약틱";

	m_bStaff = false;

	CRect rc, cRc;
	GetWindowRect(&rc);

	if (!m_pTooltip)
	{
		// escdream 
		// CToolTipCtrl 스마트 포인터로 전환시 Destroy시 오류 있는 것 같음. 
		// 변경전 내용확인 후 처리 할것
		[[gsl::suppress(26409)]]
		m_pTooltip = new CToolTipCtrl;

		m_pTooltip->Create(GetDesktopWindow(), TTS_ALWAYSTIP);
		m_pTooltip->SetMaxTipWidth(1000);
		m_pTooltip->Activate(TRUE);
	}

// line 1 링크 버튼
	const int cx = INNER_FRAME_SPACE, cy = INNER_FRAME_SPACE;
	//COLORREF bkColor = RGB(0, 0, 0);
	//COLORREF fgColor = RGB(255, 255, 0);
	const int ibtn3w = 48;
	const int ibtngap = 1;

	cRc.SetRect(INNER_FRAME_SPACE, cy, INNER_FRAME_SPACE + ibtn3w, cy + CTRL_HEIGHT);
	m_pBtnAccount = std::make_unique<CfxImgButton>();
	m_pBtnAccount->Create("\n실시간 체결/미체결", cRc, this, ID_CTRL_ACCOUNT, FALSE, TRUE);
	m_pBtnAccount->SetWindowText("체결");
	m_pBtnAccount->SetFont(m_pfont);
	m_pBtnAccount->SetTextColor(m_pTool->GetIndexColor(69));


	cRc.SetRect(cRc.right + ibtngap, cRc.top, cRc.right + ibtngap + ibtn3w, cRc.bottom);
	m_pBtnChart = std::make_unique<CfxImgButton>();
	m_pBtnChart->Create("\n종합차트", cRc, this, ID_CTRL_CHART, FALSE, TRUE);
	m_pBtnChart->SetWindowText("차트");
	m_pBtnChart->SetFont(m_pfont);
	m_pBtnChart->SetTextColor(m_pTool->GetIndexColor(69));

	cRc.SetRect(cRc.right + ibtngap, cRc.top, cRc.right + ibtngap + ibtn3w, cRc.bottom);
	m_pBtnRevaluate = std::make_unique<CfxImgButton>();
	m_pBtnRevaluate->Create("\n보유잔고 및 평가손익", cRc, this, ID_CTRL_REVAL, FALSE, TRUE);
	m_pBtnRevaluate->SetWindowText("잔고");
	m_pBtnRevaluate->SetFont(m_pfont);
	m_pBtnRevaluate->SetTextColor(m_pTool->GetIndexColor(69));



// line 2 주문수량 조회
	m_tRc1.SetRect(cx, cRc.bottom + GAP * 2, cx + 70, cRc.bottom + GAP * 2 + CTRL_HEIGHT);
	m_tRc2 = m_tRc1;	m_tRc2.OffsetRect(0, CTRL_HEIGHT + GAP);
	m_tRc9 = m_tRc2;	m_tRc9.OffsetRect(0, CTRL_HEIGHT + GAP);
	m_tRc3 = m_tRc9;	m_tRc3.OffsetRect(0, CTRL_HEIGHT + GAP);
	m_tRc4 = m_tRc3;	m_tRc4.OffsetRect(0, CTRL_HEIGHT + GAP);	//2015.03.16 KSJ 매매금액

	m_rectMdCnt.SetRect(m_tRc1.right + GAP, m_tRc1.top, CTRL_WIDTH - GAP, m_tRc1.bottom);
	m_rectMsCnt = m_rectMdCnt;	m_rectMsCnt.OffsetRect(0, CTRL_HEIGHT + GAP);
	m_rectMaxMsCnt = m_rectMsCnt;	m_rectMaxMsCnt.OffsetRect(0, CTRL_HEIGHT + GAP);
	m_oRc3 = m_rectMaxMsCnt;
	m_oRc3.OffsetRect(0, CTRL_HEIGHT + GAP);
	m_pEditJmdg = std::make_unique<class C3DEdit>();
	m_pEditJmdg->Create(NULL, WS_CHILDWINDOW | WS_VISIBLE | WS_TABSTOP | ES_NUMBER | ES_RIGHT, m_oRc3, this, ID_EDIT_JMDG, m_pfont);

	m_rectConAmt = m_oRc3; m_rectConAmt.OffsetRect(0, CTRL_HEIGHT + GAP);	//2015.03.16 KSJ 매매금액

	m_pBtnMDCan = std::make_unique<CfxImgButton>(m_pfont);
	m_pBtnMDCan->Create("\n매도가능", m_tRc1, this, ID_CTRL_MDCAN, FALSE, TRUE);
	m_pBtnMDCan->SetWindowText("매도가능");
	m_pBtnMDCan->SetTextColor(m_pTool->GetIndexColor(69));


	m_pBtnMSCan = std::make_unique<CfxImgButton>(m_pfont);
	m_pBtnMSCan->Create("\n매수가능", m_tRc2, this, ID_CTRL_MSCAN, FALSE, TRUE);
	m_pBtnMSCan->SetWindowText("매수가능");
	m_pBtnMSCan->SetTextColor(m_pTool->GetIndexColor(69));


	m_pBtnMaxMS = std::make_unique<CfxImgButton>(m_pfont);
	m_pBtnMaxMS->Create("\n최대매수", m_tRc9, this, ID_CTRL_MAXMS, FALSE, TRUE);
	m_pBtnMaxMS->SetWindowText("최대매수");
	m_pBtnMaxMS->SetTextColor(m_pTool->GetIndexColor(69));

	//2015.03.16 KSJ 매매금액
	m_pBtnConAmt = std::make_unique<CfxImgButton>(m_pfont);
	m_pBtnConAmt = std::make_unique<CfxImgButton>(m_pfont);
	m_pBtnConAmt->Create("\n[매매금액= 수량*주문가격 / 수수료,세금 미반영]", m_tRc4, this, ID_CTRL_CONAMT, FALSE, TRUE);
	m_pBtnConAmt->SetWindowText("매매금액");
	m_pBtnConAmt->SetTextColor(m_pTool->GetIndexColor(69));
	m_pBtnConAmt->ShowWindow(SW_HIDE);

	cRc.SetRect(2, m_tRc1.top - GAP*1, CTRL_WIDTH, m_rectConAmt.bottom + GAP*1);
	m_sRc1 = cRc;

// line 3 잔고
	m_rectJangoS.SetRect(cx, cRc.bottom + GAP * 3, cx + 70, cRc.bottom + GAP * 2 + CTRL_HEIGHT);
	m_tRc5 = m_rectJangoS;	m_tRc5.OffsetRect(0, CTRL_HEIGHT + GAP);
	m_tRc6 = m_tRc5;	m_tRc6.OffsetRect(0, CTRL_HEIGHT + GAP);

	m_rectJango.SetRect(m_rectJangoS.right + GAP, m_rectJangoS.top, CTRL_WIDTH - GAP, m_rectJangoS.bottom);
	m_rectPgdg = m_rectJango;	m_rectPgdg.OffsetRect(0, CTRL_HEIGHT + GAP);
	m_rectPgsi = m_rectPgdg;	m_rectPgsi.OffsetRect(0, CTRL_HEIGHT + GAP);

	cRc.SetRect(2, m_rectJangoS.top - GAP * 1, CTRL_WIDTH, m_rectPgsi.bottom + GAP * 1);
	m_sRc2 = cRc;

// line 4 주문수량
	m_tRc7.SetRect(cx, cRc.bottom + GAP * 3, cx + 52, cRc.bottom + GAP * 3 + CTRL_HEIGHT);
	
//	cRc.SetRect(m_tRc7.right + GAP * 2, m_tRc7.top, m_tRc7.right + GAP * 2 + 68, m_tRc7.bottom + 2);
	cRc.SetRect(m_tRc7.right, m_tRc7.top, m_tRc7.right + 68, m_tRc7.bottom + 2);
	m_pEditJmcn = std::make_unique < class C3DEdit>();
	m_pEditJmcn->Create(NULL, WS_CHILDWINDOW | WS_VISIBLE | WS_TABSTOP | ES_NUMBER | ES_RIGHT, cRc, this, ID_EDIT_JMCN, m_pfont);
	m_pEditJmcn->SetToParent(true);

	m_pSpinJmcn = std::make_unique< CSpinCtrl >();
	m_pSpinJmcn->Create(NULL, cRc, this, ID_SPIN_JMCN, m_pfont);
	m_pSpinJmcn->SetBuddy(m_pEditJmcn.get());
	m_pSpinJmcn->SetRange32(0, 99999999);	
	m_pSpinJmcn->SetNaturalType(m_pEditJmcn.get(), 0, 9999999, 10);
	m_pEditJmcn->SetWindowText("0");

	cRc.SetRect(cRc.right + 2, cRc.top, cRc.right + 2 + 20, cRc.top + 20);
	m_pBtnJmcn = std::make_unique<CfxImgButton>(m_pfont);
	m_pBtnJmcn->Create("", cRc, this, ID_CTRL_JMCN, FALSE, TRUE);
	m_pBtnJmcn->SetTextColor(m_pTool->GetIndexColor(69));

	m_pAmount = (CContainerWnd*) new CContainerWnd;
	m_pAmount->SetParent(m_pwndView);
	m_pAmount->Create(NULL, NULL, WS_CHILD|WS_VISIBLE, CRect(0, 0, 1, 1), this, -1);
	m_pAmount->CreateControl(m_pTool->GetHome(), "cx_Amount", "", CRect(0, 0, 1, 1), "/k1");

	CString strPath;
	strPath.Format("%s\\%s\\%s\\%s.ini", m_pTool->GetHome(), USRDIR, m_pTool->GetName(), MAP_NAME);
	CConfigDlg	dlg(strPath);
	dlg.InitData();

	if(dlg.m_config.popupremain)
		m_pAmount->SetAlways(true);
	else
		m_pAmount->SetAlways(false);

// line 5 금액주문
	cRc.SetRect(cx, cRc.bottom + GAP * 3, cx + 70, cRc.bottom + GAP * 2 + CTRL_HEIGHT);
	m_money.rcChk.SetRect(cRc.left, cRc.top, cRc.left + 13, cRc.bottom);
	m_money.rcText.SetRect(m_money.rcChk.right, cRc.top, cRc.right, cRc.bottom);
	m_money.data = _T("금액주문");
	m_money.chk = false;

	cRc.SetRect(cRc.right + 2, cRc.top, CTRL_WIDTH - GAP, cRc.bottom);
	m_pEditMoney = std::make_unique<class C3DEdit>();
	m_pEditMoney->Create(NULL, WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_NUMBER | ES_RIGHT, cRc, this, ID_EDIT_MONEY, m_pfont);
	m_pEditMoney->SetWindowText("0");
	m_pEditMoney->SetLimitText(13);
	m_pEditMoney->EnableWindow(FALSE);	//2016.08.01 KSJ 초기에 비활성화

	CString	Path;
	Path.Format("%s\\tab\\ACCNTDEPT.INI", m_pTool->Variant(homeCC,""));
	
	char readB[1024];
	int readL;
	
	readL = GetPrivateProfileString("ACCNTDEPT","DEPT"," ",readB,sizeof(readB),Path);
	
	CString tDept(readB,readL);
	tDept.TrimLeft();tDept.TrimRight();

	//if(m_sDept == "813" || m_sDept == tDept)
	if(m_sDept == tDept)
	{
		cRc.SetRect(cx, cRc.bottom + GAP * 3, cx + 70, cRc.bottom + GAP * 2 + CTRL_HEIGHT);
		m_chaik.rcChk.SetRect(cRc.left, cRc.top, cRc.left + 13, cRc.bottom);
		m_chaik.rcText.SetRect(m_chaik.rcChk.right, cRc.top, cRc.right, cRc.bottom);
		m_chaik.data = _T("차익거래");
		m_chaik.chk = true;
	}

// line 6 다중매매 
	cRc.SetRect(cx, cRc.bottom + GAP * 3, cx + 70, cRc.bottom + GAP * 2 + CTRL_HEIGHT);
	m_multi.rcChk.SetRect(cRc.left, cRc.top, cRc.left + 13, cRc.bottom);
	m_multi.rcText.SetRect(m_multi.rcChk.right, cRc.top, cRc.right, cRc.bottom);
	m_multi.data = _T("다중매매");
	m_multi.chk = false;

	cRc.SetRect(cRc.right + 2, cRc.top, CTRL_WIDTH - GAP, cRc.bottom + 200);
	m_cbMulti.Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | CBS_DROPDOWNLIST, cRc, this, ID_COMBO_MULTI);
	m_cbMulti.SetFont(m_pfont);
	m_cbMulti.EnableWindow(FALSE);	//2016.08.01 KSJ 초기에 비활성화

	m_cbMulti.AddString("2");
	m_cbMulti.AddString("3");
	m_cbMulti.AddString("4");
	m_cbMulti.AddString("5");
	m_cbMulti.AddString("6");
	m_cbMulti.AddString("7");
	m_cbMulti.AddString("8");
	m_cbMulti.AddString("9");
	m_cbMulti.AddString("10");
	m_cbMulti.SetCurSel(0);

// line 7 주문조건
	cRc.SetRect(cx, cRc.top + CTRL_HEIGHT + GAP * 3, cx, cRc.top + CTRL_HEIGHT + GAP * 2 + CTRL_HEIGHT);
	m_condition[0].rcChk.SetRect(cRc.right, cRc.top, cRc.right + 13, cRc.bottom);
	m_condition[0].rcText.SetRect(m_condition[0].rcChk.right, m_condition[0].rcChk.top, m_condition[0].rcChk.right + 35, m_condition[0].rcChk.bottom);
	m_condition[0].data = _T("현금");
//	m_condition[0].chk = true;
	m_condition[0].chk = false;

	//m_condition[1].rcChk.SetRect(m_condition[0].rcText.right + GAP*1, m_condition[0].rcText.top, m_condition[0].rcText.right + 13 + GAP*1, m_condition[0].rcText.bottom);
	m_condition[1].rcChk.SetRect(cRc.right, m_condition[0].rcText.top + CTRL_HEIGHT, cRc.right + 13, m_condition[0].rcText.bottom + CTRL_HEIGHT);
	m_condition[1].rcText.SetRect(m_condition[1].rcChk.right, m_condition[1].rcChk.top, m_condition[1].rcChk.right + 35, m_condition[1].rcChk.bottom);
	m_condition[1].data = _T("신용");
	m_condition[1].chk = false;

	CRect cbCrd;
	cbCrd = m_condition[1].rcText;
	cbCrd.right += cbCrd.left + 45;
	cbCrd.OffsetRect(m_condition[1].rcText.Width(), -2);
	cbCrd.bottom += 200;
	m_cbCrType.Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | CBS_DROPDOWNLIST, cbCrd, this, ID_COMBO_CRD);
	m_cbCrType.SetFont(m_pfont);
//	m_cbCrType.EnableWindow(FALSE);	
	m_cbCrType.AddString("유통융자");
	m_cbCrType.AddString("자기융자");
//	m_cbCrType.AddString("유통대주");
	m_cbCrType.SetCurSel(0);

//	m_condition[2].rcChk.SetRect(m_condition[1].rcText.right + GAP*1, m_condition[1].rcText.top, m_condition[1].rcText.right + 13 + GAP*1, m_condition[1].rcText.bottom);
	m_condition[2].rcChk.SetRect(m_condition[0].rcText.right + GAP*1, m_condition[0].rcText.top, m_condition[0].rcText.right + 13 + GAP*1, m_condition[0].rcText.bottom);
	m_condition[2].rcText.SetRect(m_condition[2].rcChk.right, m_condition[2].rcChk.top, m_condition[2].rcChk.right + 35, m_condition[2].rcChk.bottom);
	m_condition[2].data = _T("대출");
	m_condition[2].chk = false;

	cRc.SetRect(2, m_tRc7.top - GAP * 1, CTRL_WIDTH, cRc.bottom);

	cRc.OffsetRect(0, GAP * 2 + CTRL_HEIGHT);

	m_sRc3 = cRc;

// line 8 예약매매 주문틱 설정
	m_tRc8.SetRect(cx, cRc.bottom + GAP * 3, cx + 70, cRc.bottom + GAP * 2 + CTRL_HEIGHT);
	
// 	cRc.SetRect(m_tRc8.right + GAP, m_tRc8.top, CTRL_WIDTH - GAP, m_tRc8.bottom);
	cRc.SetRect(m_tRc8.right + GAP -27 , m_tRc8.top, CTRL_WIDTH - GAP - 60, m_tRc8.bottom);	//2012.10.29 KSJ 예약틱 사이즈 조절
	m_pEditTick = std::make_unique<class C3DEdit>();
	m_pEditTick->Create(NULL, WS_CHILDWINDOW | WS_VISIBLE | WS_TABSTOP | ES_NUMBER | ES_RIGHT, cRc, this, ID_EDIT_TICK, m_pfont);
	m_pEditTick->SetWindowText("2");

	m_pSpinTick = std::make_unique<CSpinCtrl>();
	m_pSpinTick->Create(NULL, cRc, this, ID_SPIN_TICK, m_pfont);
	m_pSpinTick->SetBuddy(m_pEditTick.get());
	m_pSpinTick->SetRange32(-999, 999);

// line 8 시장가 2012.10.29 KSJ
	cRc.SetRect(cRc.right + GAP * 3 , cRc.top, cRc.right + 62 , cRc.bottom);
	m_sijang.rcChk.SetRect(cRc.left, cRc.top, cRc.left + 13, cRc.bottom);
	m_sijang.rcText.SetRect(m_sijang.rcChk.right, cRc.top, cRc.right, cRc.bottom);
	m_sijang.data = _T("시장가");
	m_sijang.chk = false;
	
// 원클릭주문 설정
	cRc.SetRect(cx, cRc.bottom + GAP * 3, cx + 88, cRc.bottom + GAP * 2 + CTRL_HEIGHT);
	m_click.rcChk.SetRect(cRc.left, cRc.top, cRc.left + 13, cRc.bottom);
	m_click.rcText.SetRect(cRc.left + 13, cRc.top, cRc.right, cRc.bottom);
	m_click.data = _T("원클릭 주문");
	m_click.chk = false;

	cRc.SetRect(2, m_tRc8.top - GAP * 1, CTRL_WIDTH, cRc.bottom + GAP * 1);
	m_sRc4 = cRc;

// line 9 종목등록
	cRc.SetRect(GAP, cRc.bottom + GAP * 3 - 1, CTRL_WIDTH - GAP, 560);
	//cRc.SetRect(GAP, cRc.bottom + GAP * 16 - 1, CTRL_WIDTH - GAP, 570);
	//m_pCodeReg = new CCodeReg(this, m_pMapWnd, m_pView);
	m_pCodeReg = new CCodeReg(m_pTool, true);
	m_pCodeReg->Create(NULL, "", WS_CHILD | WS_VISIBLE, cRc, this, ID_CTRL_CODEREG);
	//m_pCodeReg->Init(m_pTool->GetHome(), m_pTool->GetName(), true);

// etc
	SetButtonBitmaps();
	SetTimer(101, 250, NULL);

	CString strHome = m_pTool->Variant(homeCC);
	CString strImgDir = "";
	strImgDir.Format("%s\\Image\\", strHome);
	m_hAlertBitmap = m_pTool->RequestHBitmap(strImgDir+_T("알림_ic.bmp"));
}

void CControlWnd::SetButtonBitmaps()
{
	CString path = "";
	path.Format("%s\\%s\\", m_pTool->GetHome(), IMAGEDIR);

	HBITMAP hbmp	= m_pTool->RequestBitmap(path + "3btn.bmp")->operator HBITMAP();
	HBITMAP hbmp_dn	= m_pTool->RequestBitmap(path + "3btn_dn.bmp")->operator HBITMAP();
	HBITMAP hbmp_hv	= m_pTool->RequestBitmap(path + "3btn_en.bmp")->operator HBITMAP();
	m_pBtnAccount->SetImgBitmap(hbmp, hbmp_dn, hbmp_hv);
	m_pBtnChart->SetImgBitmap(hbmp, hbmp_dn, hbmp_hv);
	m_pBtnRevaluate->SetImgBitmap(hbmp, hbmp_dn, hbmp_hv);

	hbmp    = m_pTool->RequestBitmap(path + "6btn.bmp")->operator HBITMAP();
	hbmp_dn	= m_pTool->RequestBitmap(path + "6btn_dn.bmp")->operator HBITMAP();
	hbmp_hv	= m_pTool->RequestBitmap(path + "6btn_en.bmp")->operator HBITMAP();
	m_pBtnMDCan->SetImgBitmap(hbmp, hbmp_dn, hbmp_hv);
	m_pBtnMSCan->SetImgBitmap(hbmp, hbmp_dn, hbmp_hv);
	m_pBtnMaxMS->SetImgBitmap(hbmp, hbmp_dn, hbmp_hv);
	m_pBtnConAmt->SetImgBitmap(hbmp, hbmp_dn, hbmp_hv);

	hbmp	= m_pTool->RequestBitmap(path + "수량.bmp")->operator HBITMAP();
	hbmp_dn	= m_pTool->RequestBitmap(path + "수량_dn.bmp")->operator HBITMAP();
	hbmp_hv	= m_pTool->RequestBitmap(path + "수량_en.bmp")->operator HBITMAP();
	m_pBtnJmcn->SetImgBitmap(hbmp, hbmp_dn, hbmp_hv);
}

void CControlWnd::OnDestroy() 
{
	CString sval;
	m_pEditJmdg->GetWindowText(sval);
	((CMapWnd*)GetParent())->SaveBuffer(_T("JumunPrc"), sval);

	//수량
	m_pEditJmcn->GetWindowText(sval);
	((CMapWnd*)GetParent())->SaveBuffer(_T("JVol"), sval);

	//금액주문
	m_pEditMoney->GetWindowText(sval);
	((CMapWnd*)GetParent())->SaveBuffer(_T("JAmt"), sval);

	sval.Format("%d", m_money.chk);
	((CMapWnd*)GetParent())->SaveBuffer(_T("MoneyChk"), sval);
	

	//다중매매
	const int ival = m_cbMulti.GetCurSel();
	sval.Format("%d", ival);
	((CMapWnd*)GetParent())->SaveBuffer(_T("JMulti"), sval);

	
	sval.Format("%d", m_multi.chk);
	((CMapWnd*)GetParent())->SaveBuffer(_T("MultiChk"), sval);

	int ichk = 0;
	for(int i=0; i<3; i++)
	{
		if(m_condition[i].chk) ichk = i;
	}

//	sval.Format("%d", ichk);   //IOC FOK는 사용하지 않음
//	((CMapWnd*)GetParent())->SaveBuffer(_T("ConditionChk"), sval);

	if(m_pCodeReg)
	{
		m_pCodeReg->DestroyWindow();
		delete m_pCodeReg;
		m_pCodeReg = NULL;
	}

	if (m_pBtnJmcn)
	{
		::DestroyWindow(m_pBtnJmcn->m_hWnd);
//		delete m_pBtnJmcn;
		m_pBtnJmcn = NULL;
	}
	if (m_pBtnMaxMS)
	{
		::DestroyWindow(m_pBtnMaxMS->m_hWnd);
//		delete m_pBtnMaxMS;
		m_pBtnMaxMS = NULL;
	}
	if (m_pBtnMDCan)
	{
		::DestroyWindow(m_pBtnMDCan->m_hWnd);
//		delete m_pBtnMDCan;
		m_pBtnMDCan = NULL;
	}
	if (m_pBtnMSCan)
	{
		::DestroyWindow(m_pBtnMSCan->m_hWnd);
//		delete m_pBtnMSCan;
		m_pBtnMSCan = NULL;
	}
	if (m_pBtnChart)
	{
		::DestroyWindow(m_pBtnChart->m_hWnd);
//		delete m_pBtnChart;
		m_pBtnChart = NULL;
	}
	if (m_pBtnAccount)
	{
		::DestroyWindow(m_pBtnAccount->m_hWnd);
//		delete m_pBtnAccount;
		m_pBtnAccount = NULL;
	}
	if (m_pBtnConAmt)
	{
		::DestroyWindow(m_pBtnConAmt->m_hWnd);
//		delete m_pBtnConAmt;
		m_pBtnConAmt = NULL;
	}

	if (m_pTooltip && m_pTooltip->m_hWnd)
	{
		m_pTooltip->Activate(FALSE);
		m_pTooltip->DelTool(this);
//		m_pTooltip->RelayEvent(NULL);
		
		delete m_pTooltip;
	}

	CWnd::OnDestroy();
}

void CControlWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CMemDC memDC(&dc);	

	CRect rc;
	GetClientRect(&rc);

	const int bkMode = memDC.SetBkMode(TRANSPARENT);
	memDC.FillSolidRect(rc, m_clrBG);

	CPen *ppenOld = memDC.SelectObject(m_ppenLine);
	memDC.SelectStockObject(NULL_BRUSH);

	DrawSiseText(&memDC, m_text3, m_tRc3, DT_RIGHT);
	DrawSiseText(&memDC, m_text4, m_tRc4, DT_RIGHT);
	m_rcAlert = m_tRc4;
	
	//2015.03.26 KSJ 툴팁추가  //test
#ifndef _DEBUG
	if (m_pTooltip)
		m_pTooltip->AddTool(this, _T("[매매금액= 수량*주문가격 / 수수료,세금 미반영]"), m_tRc4);
#endif

	memDC.FillSolidRect(&m_rectMdCnt, m_clrDT);
	memDC.Rectangle(&m_rectMdCnt);
	memDC.FillSolidRect(&m_rectMsCnt, m_clrDT);
	memDC.Rectangle(&m_rectMsCnt);
	memDC.FillSolidRect(&m_rectMaxMsCnt, m_clrDT);
	memDC.Rectangle(&m_rectMaxMsCnt);
	memDC.FillSolidRect(&m_rectConAmt, m_clrDT);
	memDC.Rectangle(&m_rectConAmt);

	DrawSiseText(&memDC, m_strMdCnt, m_rectMdCnt, DT_RIGHT, ST_COMMA | ST_NORMAL);
	DrawSiseText(&memDC, m_strMsCnt, m_rectMsCnt, DT_RIGHT, ST_COMMA | ST_NORMAL);
	DrawSiseText(&memDC, m_strMaxMsCnt, m_rectMaxMsCnt, DT_RIGHT, ST_COMMA | ST_NORMAL);
	DrawSiseText(&memDC, m_strConAmt, m_rectConAmt, DT_RIGHT, ST_COMMA | ST_NORMAL);	//2015.03.16 KSJ 매매금액

	memDC.Rectangle(&m_sRc1);
	
	DrawSiseText(&memDC, m_strJangoS, m_rectJangoS, DT_RIGHT);
	DrawSiseText(&memDC, m_text5, m_tRc5, DT_RIGHT);
	DrawSiseText(&memDC, m_text6, m_tRc6, DT_RIGHT);

	memDC.FillSolidRect(&m_rectJango, m_clrDT);
	memDC.Rectangle(&m_rectJango);
	memDC.FillSolidRect(&m_rectPgdg, m_clrDT);
	memDC.Rectangle(&m_rectPgdg);
	memDC.FillSolidRect(&m_rectPgsi, m_clrDT);
	memDC.Rectangle(&m_rectPgsi);

	DrawSiseText(&memDC, m_strJango, m_rectJango, DT_RIGHT, ST_COMMA);
	DrawSiseText(&memDC, m_strPgdg, m_rectPgdg, DT_RIGHT, ST_COMMA);
	DrawSiseText(&memDC, m_strPgsi, m_rectPgsi, DT_RIGHT, ST_COMMA | ST_COLOR);

	memDC.Rectangle(&m_sRc2);
	DrawSiseText(&memDC, m_text7, m_tRc7, DT_RIGHT);

	memDC.DrawFrameControl(&m_money.rcChk, DFC_BUTTON, DFCS_BUTTONCHECK | (m_money.chk ? DFCS_CHECKED : 0));
	DrawSiseText(&memDC, m_money.data, m_money.rcText, DT_RIGHT);

	memDC.DrawFrameControl(&m_multi.rcChk, DFC_BUTTON, DFCS_BUTTONCHECK | (m_multi.chk ? DFCS_CHECKED : 0));
	DrawSiseText(&memDC, m_multi.data, m_multi.rcText, DT_RIGHT);

	memDC.DrawFrameControl(&m_chaik.rcChk, DFC_BUTTON, DFCS_BUTTONCHECK | (m_chaik.chk ? DFCS_CHECKED : 0));
	DrawSiseText(&memDC, m_chaik.data, m_chaik.rcText, DT_RIGHT);

	memDC.DrawFrameControl(&m_click.rcChk, DFC_BUTTON, DFCS_BUTTONCHECK | (m_click.chk ? DFCS_CHECKED : 0));
	DrawSiseText(&memDC, m_click.data, m_click.rcText, DT_RIGHT);

	memDC.DrawFrameControl(&m_sijang.rcChk, DFC_BUTTON, DFCS_BUTTONCHECK | (m_sijang.chk ? DFCS_CHECKED : 0));
	DrawSiseText(&memDC, m_sijang.data, m_sijang.rcText, DT_RIGHT);

	CFont* pOldFont = memDC.SelectObject( m_pfont);
	for (int ii = 0; ii < 3; ii++)
	{
		memDC.DrawFrameControl(&m_condition[ii].rcChk, DFC_BUTTON, DFCS_BUTTONRADIO | (m_condition[ii].chk ? DFCS_CHECKED : 0));

	//	memDC.Rectangle(m_condition[ii].rcText);
		memDC.DrawText(m_condition[ii].data, m_condition[ii].rcText, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
	//	DrawSiseText(&memDC, m_condition[ii].data, m_condition[ii].rcText, DT_LEFT);
	}

	memDC.SelectObject( pOldFont);
	DeleteObject(pOldFont);
	memDC.Rectangle(&m_sRc3);
	
	DrawSiseText(&memDC, m_text8, m_tRc8, DT_LEFT);
	memDC.Rectangle(&m_sRc4);
//	CRect rr = m_rectPgsi;
//	rr.InflateRect(2,2,2,2); 
//	memDC.Rectangle(&rr);	

//	tmRc.OffsetRect(-1 * GUIDE_ALARM_BITMAP, 0);
	m_rcAlert.right = m_rcAlert.left + GUIDE_ALARM_BITMAP;
	m_rcAlert.OffsetRect(0, 3);
	DrawBitmap(&memDC, m_rcAlert, m_hAlertBitmap);

	memDC.SelectObject(ppenOld);
	memDC.SetBkMode(bkMode);
}

void CControlWnd::DrawSiseText(CDC* pDC, CString data, CRect rc, UINT nFormat, UINT nType)
{
	if (data.IsEmpty())
		return;

	nFormat = DT_SINGLELINE | DT_VCENTER | nFormat;

	CFont* pOldFont{};
	if (nType & ST_BOLD)
		pOldFont = pDC->SelectObject(m_pfontB);
	else
		pOldFont = pDC->SelectObject(m_pfont);

	rc.DeflateRect(5, 0);
	if (nType & ST_TEXT)
	{
		pDC->DrawText(data, rc, nFormat);
		return;
	}

	CString strGiho;
	COLORREF clrText = m_clrTFG;

	int iPos=0;
	if (nType & ST_COLOR)
	{
		iPos = data.Find('+');
		if (iPos >= 0)
			clrText = m_pTool->GetIndexColor(94);

		iPos = data.Find('-');
		if (iPos >= 0)
			clrText = m_pTool->GetIndexColor(95);
	}

	CString strPercent;
	if (nType & ST_PERCENT)
		strPercent = " %";

	if (nType & ST_REMOVE)
	{
		data.Remove('+');
		data.Remove('-');
	}

	CString strValue;
	if (nType & ST_NORMAL)
	{
		CString pre = _T("");
		iPos = data.Find('+');
		if (iPos >= 0)
		{
			pre = "+";
			data.Remove('+');
		}

		iPos = data.Find('-');
		if (iPos >= 0)
		{
			pre = "-";
			data.Remove('-');
		}

//		data.Remove(' ');
//		iPos = data.Find('.');
//		if (iPos >= 0)
//			strValue.Format("%.2f", atof(data));
//		else
//			strValue.Format("%d", atoi(data));
//
//		if (atof(data) == 0.0)
//			data = "";
//		else
//			data = pre + strValue;
	}

	if (nType & ST_COMMA)
	{
		data = m_pTool->AddComma(data);
		
	}

	const COLORREF clrOldText = pDC->SetTextColor(clrText);
	pDC->DrawText(strGiho + data + strPercent, rc, nFormat);

	pDC->SelectObject(pOldFont);
	pDC->SetTextColor(clrOldText);
}

void CControlWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (m_money.rcChk.PtInRect(point) || m_money.rcText.PtInRect(point))
	{
		m_money.chk = !m_money.chk;
		m_pEditMoney->EnableWindow(m_money.chk);
		InvalidateRect(&m_money.rcChk);
	}
	
	if (m_multi.rcChk.PtInRect(point) || m_multi.rcText.PtInRect(point))
	{
		m_multi.chk = !m_multi.chk;

		if (m_multi.chk)
		{
			CMultiJumunDlg dlg;

			if (dlg.DoModal() == IDCANCEL)
				m_multi.chk = false;
		}

		m_cbMulti.EnableWindow(m_multi.chk);

		InvalidateRect(&m_multi.rcChk);
	}

	if (m_chaik.rcChk.PtInRect(point) || m_chaik.rcText.PtInRect(point))
	{
		m_chaik.chk = !m_chaik.chk;		
		InvalidateRect(&m_chaik.rcChk);
	}

	if (m_click.rcChk.PtInRect(point) || m_click.rcText.PtInRect(point))
	{
		m_click.chk = !m_click.chk;

		if(m_click.chk)
		{
			MessageBox("원클릭주문 설정 시 한번의 클릭으로 주문이 접수되므로 이용에 유의하시기 바랍니다." , "IBK투자증권", MB_OK);
		}

		InvalidateRect(&m_click.rcChk);
	}

	if (m_sijang.rcChk.PtInRect(point) || m_sijang.rcText.PtInRect(point))
	{
		if(!m_sijang.chk && m_bMarketConfirm)
		{
			CMarketConfirm dlg(this);
			
			dlg.DoModal();
		}

		m_sijang.chk = !m_sijang.chk;
		m_pEditTick->EnableWindow(!m_sijang.chk);
		InvalidateRect(&m_sijang.rcChk);
	}

	for (int ii = 0; ii < 3; ii++)
	{
		if (m_condition[ii].rcChk.PtInRect(point) || m_condition[ii].rcText.PtInRect(point))
		{
			for (int jj = 0; jj < 3; jj++)
				m_condition[jj].chk = false;
			
			m_condition[ii].chk = true;
			
			for (int jj = 0; jj < 3; jj++)
				InvalidateRect(&m_condition[jj].rcChk);
			
		
			if(ii == 1 || ii == 2)
				GetParent()->SendMessage(UM_MYMESSAGE, MAKEWPARAM(UP_SHOW_JANGO, 1), NULL);

			m_pBtnMDCan->EnableWindow(TRUE);  //매도가능
			m_pBtnMSCan->EnableWindow(TRUE);  //매수가능
			m_pBtnMaxMS->EnableWindow(TRUE);  //최대매수

			m_strMdCnt = "";
			InvalidateRect(&m_rectMdCnt);
			m_strMsCnt = "";
			InvalidateRect(&m_rectMsCnt);
			m_strMaxMsCnt = "";
			InvalidateRect(&m_rectMaxMsCnt);

			if(ii == 1)
				m_pBtnMaxMS->EnableWindow(FALSE);  //최대매수

			if(ii == 2)
			{
				m_pBtnMDCan->EnableWindow(FALSE);  //매도가능
				m_pBtnMSCan->EnableWindow(FALSE);  //매수가능
				m_pBtnMaxMS->EnableWindow(FALSE);  //최대매수
			}

			break;
		}
	}

	if (m_rectMdCnt.PtInRect(point))
	{
		if (atoi(m_strMdCnt) > 0)
		{
			CString jmcn;
			jmcn.Format("%d", atoi(m_strMdCnt));
			m_pEditJmcn->SetWindowText(jmcn);
			OnButtonConAmt();
		}
	}
	else if (m_rectMsCnt.PtInRect(point))
	{
		if (atoi(m_strMsCnt) > 0)
		{
			CString jmcn;
			jmcn.Format("%d", atoi(m_strMsCnt));
			m_pEditJmcn->SetWindowText(jmcn);
			OnButtonConAmt();
		}
	}
	else if (m_rectMaxMsCnt.PtInRect(point))
	{
		if (atoi(m_strMaxMsCnt) > 0)
		{
			CString jmcn;
			jmcn.Format("%d", atoi(m_strMaxMsCnt));
			m_pEditJmcn->SetWindowText(jmcn);
			OnButtonConAmt();
		}
	}
	else if (m_rectJango.PtInRect(point))
	{
		if (atoi(m_strJango) > 0)
		{
			CString jmcn;
			jmcn.Format("%d", atoi(m_strJango));
			ClearMoney();
			m_pEditJmcn->SetWindowText(jmcn);
			OnButtonConAmt();
		}
	}
	else if(m_rcAlert.PtInRect(point))
	{
		MessageBox("수량X단가로 계산된 단순 매매금액으로 신용/대출 융자금은 반영되지 않습니다" , "IBK투자증권", MB_OK);
	}
	
	CWnd::OnLButtonUp(nFlags, point);
}

void CControlWnd::ReceiveSise(char* pData, int len)
{
	CString strTemp;

	struct _sise_mod* pMod = (struct _sise_mod*)pData;

	if (pMod->jmgb[0] == '+')
		m_market = KOSPI;
	else
		m_market = KOSDAQ;

	m_curr = m_pTool->ToString(pMod->curr, sizeof(pMod->curr));

	m_strMdCnt = m_strMsCnt = m_strMaxMsCnt = m_strConAmt = _T("");
	m_pEditJmdg->SetWindowText("");

	// 주문단위수량
	m_iJmdw = atoi(m_pTool->ToString(pMod->jmdw, sizeof(pMod->jmdw)));
	m_pSpinJmcn->SetNaturalType(m_pEditJmcn.get(), 0, 9999999, m_iJmdw);

	//2015.03.16 KSJ 주문수량 설정유지 기능
	CString strPath;
	strPath.Format("%s\\%s\\%s\\%s.ini", m_pTool->GetHome(), USRDIR, m_pTool->GetName(), MAP_NAME);
	CConfigDlg	dlg(strPath);
	dlg.InitData();

	if(dlg.m_config.hold_amt != 1) 	m_pEditJmcn->SetWindowText("0");	//2013.07.24 KSJ 종목, 계좌를 바꿔도 주문수량 바뀌지 않도록

	// 체결탭
	CString time, curr, cvol;

	for(int n=0; n<MAX_FC; ++n)
	{
		if (pMod->cheg[n].time[0]<'0' || pMod->cheg[n].time[0]>'9') break;
		
		time = CString(&pMod->cheg[n].time[0], 6);
		curr = CString(&pMod->cheg[n].curr[0], 9);
		cvol = CString(&pMod->cheg[n].cvol[0], 6);
		cvol.TrimRight();
		m_pCodeReg->SetRealData(time,curr,cvol);
	}	

	Invalidate();
}

void CControlWnd::NoticeParse(CString dat, CMapStringToString& ary)
{
	int	pos{}, pos2{};
	CString	str, symbol, value;

	pos = dat.Find('\n');
	if (pos != -1)
	{
		str = dat.Left(pos++);
		dat = dat.Mid(pos);
	}

	while (!str.IsEmpty())
	{
		pos = str.Find('\t');
		if (pos == -1)	break;

		symbol = str.Left(pos++);
		str    = str.Mid(pos);

		pos = str.Find('\t');
		if (pos == -1)
		{
			value = str;
			str.Empty();
		}
		else
		{
			pos2 = pos;
			value = str.Left(pos2++);
			pos = value.Find('\r');
			if (pos != -1)
			{
				value = value.Left(pos++);
				str   = str.Mid(pos);
			}
			else
				str = str.Mid(pos2);
		}

		value.TrimRight();
		ary.SetAt(symbol, value);
	}

	while (!dat.IsEmpty())
	{
		pos = dat.Find('\t');
		if (pos == -1)	break;

		symbol = dat.Left(pos++);
		dat    = dat.Mid(pos);

		pos = dat.Find('\t');
		if (pos == -1)
		{
			value = dat;
			dat.Empty();
		}
		else
		{
			value = dat.Left(pos++);
			dat   = dat.Mid(pos);
		}

		value.TrimRight();
		ary.SetAt(symbol, value);
	}
}

int CControlWnd::GetJumunCount()
{
	CString data;
	m_pEditJmcn->GetWindowText(data);
	data.Remove(',');

	return atoi(data);
}	

CString CControlWnd::GetJumunCondition()  
{
	for (int ii = 0; ii < 3; ii++)
	{
		if (m_condition[ii].chk)
		{
			m_strCondition.Format("%d", ii);
		//	return m_strCondition;    //IOC FOK는 구분은 이제 없음으로만
			return "0";
		}
	}
	
	return "0";
}

int CControlWnd::GetTickCount()
{
	CString tick;
	m_pEditTick->GetWindowText(tick);
	return atoi(tick);
}

int CControlWnd::GetMultiCount()
{
	CString	count;
	m_cbMulti.GetWindowText(count);

	if (m_multi.chk)
		return atoi(count);
	else
		return 1;
}

int CControlWnd::GetPGDG()
{
	return atoi(m_strPgdg);
}

void CControlWnd::SetPrice(int mmgb, int price)
{
	if (price < 1)
		return;

	// 금액주문
	CString money;
	m_pEditMoney->GetWindowText(money);
	money.TrimLeft();	money.TrimRight();
	money.Remove(',');

	if (money.IsEmpty())
		return;

	const int iBatPrice = atoi(money);
	if (iBatPrice < 1)
		return;

	if (!m_money.chk)
		return;

	// 주문 수량
	int iOrderCount = iBatPrice / price;

	int iTotalPay = iOrderCount * price;

	// 세금 계산 (매도 일때만)
	int iTax = 0;
	if (mmgb == 1)
		iTax = (int)(iTotalPay * 0.0003);

	// 수수료 계산
	int iCommission = calculateCommission(iTotalPay);

	// 잔고
	const int iJango = m_pTool->AtoI(m_strJango);
	
	switch (mmgb)
	{
	case 1:	// 매도
		while (iBatPrice > iTotalPay - (iCommission + iTax))
		{
			// 주문수량 재조정
			iOrderCount++;

			// 총비용 재계산
			iTotalPay = iOrderCount * price;

			// 매도일경우 세금 재계산
			iTax = (int)(iTotalPay * 0.0003);

			// 수수료 재계산
			iCommission = calculateCommission(iTotalPay);
		}

		if (m_iJmdw == 10)
			iOrderCount = (iOrderCount / 10) * 10 + 10;

		if (iOrderCount > iJango)
			iOrderCount = iJango;
		break;

	case 2:	// 매수
		while (iBatPrice < iTotalPay + iCommission + iTax)
		{
			// 주문수량 재조정
			iOrderCount--;

			// 총비용 재계산
			iTotalPay = iOrderCount * price;

			// 수수료 재계산
			iCommission = calculateCommission(iTotalPay);
		}

		if (m_iJmdw != 1)
			iOrderCount = (iOrderCount / m_iJmdw) * m_iJmdw;
		break;
	}

	CString count;
	count.Format("%d", iOrderCount);
	m_pEditJmcn->SetWindowText(count);

	OnButtonConAmt();
}

int CControlWnd::calculateCommission(int money)
{
	return (int)(money * 0.00024);
}

void CControlWnd::OnButtonAccount()
{
	//m_pTool->CreateView(typeVIEW, "DH705100", "/S/s/p5");
	m_pTool->CreateView(typeVIEW, "IB110200", "/t");
}

void CControlWnd::OnButtonChart()
{
	CString param;
	param.Format("/t/S/d1301\t%s\n", GetJcod());
	m_pTool->CreateView(typeVIEW, "IB711200", param);	//2013.01.10 KSJ 신차트로 변경
}

void CControlWnd::OnButtonReval()
{
	//m_pTool->CreateView(typeVIEW, "IB120100", "/t");
	GetParent()->SendMessage(UM_MYMESSAGE, MAKEWPARAM(UP_SHOW_JANGO, NULL), NULL);
}

void CControlWnd::OnButtonMDCan()
{
	CString jmdg;
	m_pEditJmdg->GetWindowText(jmdg);

	jmdg.Remove(',');
	if (atoi(jmdg) == 0)
	{
		jmdg.Format("%d", abs(atoi(m_curr)));
		m_pEditJmdg->SetWindowText(jmdg);
	}
//	m_pParent->SendMaxJmcn(jmdg, 1);
	SendMaxJmcn(jmdg, 1);
}

void CControlWnd::OnButtonMSCan()
{
	CString jmdg;
	m_pEditJmdg->GetWindowText(jmdg);

	jmdg.Remove(',');
	if (atoi(jmdg) == 0)
	{
		jmdg.Format("%d", abs(atoi(m_curr)));
		m_pEditJmdg->SetWindowText(jmdg);
	}

//	m_pParent->SendMaxJmcn(jmdg, 2);
	SendMaxJmcn(jmdg, 2);
}
void CControlWnd::OnButtonMaxMS()
{
	CString jmdg;
	m_pEditJmdg->GetWindowText(jmdg);

	jmdg.Remove(',');
	if (atoi(jmdg) == 0)
	{
		jmdg.Format("%d", abs(atoi(m_curr)));
		m_pEditJmdg->SetWindowText(jmdg);
	}

//	m_pParent->SendMaxJmcn(jmdg, 3);
	SendMaxJmcn(jmdg, 3);
}

void CControlWnd::OnButtonConAmt()
{
	m_strConAmt = "";

	CString jmdg; m_pEditJmdg->GetWindowText(jmdg);//주문가격
	CString jmcn; m_pEditJmcn->GetWindowText(jmcn);//주문수량
	
	jmdg.Remove(',');
	jmcn.Remove(',');

	//둘중하나라도 0이면 계산 하지 않음.
	if (atoi(jmdg) == 0 || atoi(jmcn) == 0)		m_strConAmt = "";
	else										m_strConAmt.Format("%d", atoi(jmdg) * atoi(jmcn));

	InvalidateRect(m_rectConAmt, FALSE);
}
void CControlWnd::OnButtonJmcn()
{
	CRect rect;
	m_pBtnJmcn->GetWindowRect(&rect);
	ScreenToClient(&rect);
	
	m_pAmount->SetWindowPos(NULL, rect.right + 2, rect.top, 0, 0, SWP_NOZORDER|SWP_NOSIZE);
	m_pAmount->BtnClick("");
	m_pAmount->BringWindowToTop();
}

void CControlWnd::SetJmdg(CString jmdg)
{
	m_pEditJmdg->SetWindowText(jmdg);
// 	OnButtonMDCan();
// 	OnButtonMSCan();
// 	OnButtonMaxMS();
 	OnButtonConAmt();
}

void CControlWnd::SetMaxJmcn(CString strData, int iMmgb)
{
	struct _maxjmct_mod mod {};

	struct _maxCrjmct_mod Crmod {};

	switch(iMmgb)
	{
		case TK_MDJMCN:
		case TK_MSJMCN:
		case TK_MAXMSJMCN:
			{
				CopyMemory(&mod, (LPCTSTR)strData, min(strData.GetLength(), L_maxjmct_mod));
			}
			break;
		case TK_CRMDJMCN:
		case TK_CRMSJMCN:
		case TK_CRMAXMSJMCN:
			{
				CopyMemory(&Crmod, (LPCTSTR)strData, min(strData.GetLength(), L_maxCrjmct_mod));
			}
			break;
	}
//	CopyMemory(&mod, (LPCTSTR)strData, min(strData.GetLength(), L_maxjmct_mod));

	switch (iMmgb)
	{
	case TK_MDJMCN:
		m_strMdCnt = CString(mod.lOrdAbleQty + 1, sizeof(mod.lOrdAbleQty) - 1);
		m_strMdCnt.TrimLeft(_T("0"));
		if (m_strMdCnt.IsEmpty())
			m_strMdCnt = "0";
		InvalidateRect(&m_rectMdCnt);
		break;

	case TK_MSJMCN:
		m_strMsCnt = CString(mod.lMgnRat100OrdAbleQty + 1, sizeof(mod.lMgnRat100OrdAbleQty) - 1);
		m_strMsCnt.TrimLeft(_T("0"));
		if (m_strMsCnt.IsEmpty())
			m_strMsCnt = "0";
		InvalidateRect(&m_rectMsCnt);
		break;

	case TK_MAXMSJMCN:
		m_strMaxMsCnt = CString(mod.lOrdAbleQty + 1, sizeof(mod.lOrdAbleQty) - 1);
		m_strMaxMsCnt.TrimLeft(_T("0"));
		if (m_strMaxMsCnt.IsEmpty())
			m_strMaxMsCnt = "0";
		InvalidateRect(&m_rectMaxMsCnt);
		break;
	case TK_CRMDJMCN:
		m_strMdCnt = CString(Crmod.OrdAbleQty, sizeof(Crmod.OrdAbleQty));
		m_strMdCnt.Replace("+", "");
		m_strMdCnt.Replace("-", "");
		m_strMdCnt.TrimLeft(_T("0"));
		if (m_strMdCnt.IsEmpty())
			m_strMdCnt = "0";
		InvalidateRect(&m_rectMdCnt);
		break;
		
	case TK_CRMSJMCN:
		m_strMsCnt = CString(Crmod.OrdAbleQty, sizeof(Crmod.OrdAbleQty));
		m_strMsCnt.Replace("+", "");
		m_strMsCnt.Replace("-", "");
		m_strMsCnt.TrimLeft(_T("0"));
		if (m_strMsCnt.IsEmpty())
			m_strMsCnt = "0";
		InvalidateRect(&m_rectMsCnt);
		break;
		
	case TK_CRMAXMSJMCN:
		m_strMaxMsCnt = CString(Crmod.OrdAbleQty, sizeof(Crmod.OrdAbleQty));
		m_strMaxMsCnt.Replace("+", "");
		m_strMaxMsCnt.Replace("-", "");
		m_strMaxMsCnt.TrimLeft(_T("0"));
		if (m_strMaxMsCnt.IsEmpty())
			m_strMaxMsCnt = "0";
		InvalidateRect(&m_rectMaxMsCnt);
		break;
	}

TRACE("[!!!!!! 주문가능수량] %s\n", CString(mod.lOrdAbleQty, sizeof(mod.lOrdAbleQty)));
}

void CControlWnd::SetCurr(CString curr)
{
	m_curr = curr;
	
	//	m_strPgsi.Format("%.f", fabs(atof(m_curr)) * atoi(m_strJango) - atoi(m_cmsg));
	//	if (atof(m_strPgdg) < fabs(atof(m_curr)))
	//		m_strPgsi = "+" + m_strPgsi;
	//	InvalidateRect(&m_rectPgsi);
}

//2015.03.16 KSJ 매체수수료
void CControlWnd::SetFee(CString strData, int iMmgb)
{
	struct _pihocfee_mod mod {};
	
	CopyMemory(&mod, (LPCTSTR)strData, min(strData.GetLength(), L_pihocfee_mod));

	m_strHtsFee = CString(mod.zOutHts, sizeof(mod.zOutHts));
	m_strHtsSaveFee = CString(mod.zSaveHts, sizeof(mod.zSaveHts));

	m_strHtsFee.TrimLeft(); m_strHtsFee.TrimRight();
	m_strHtsSaveFee.TrimLeft(); m_strHtsSaveFee.TrimRight();

	CString strUser = m_pTool->Variant(userCC, "");

	if(atol(strUser) > 0)	m_bStaff = true;
}

//2015.03.16 KSJ 정산금액을 구하려면 매수매도를 알아야 하는데 현재로서는 구분할 수가 없어서 처리하지 않음.
void CControlWnd::CalcFee(int iMmgb)
{
	if(m_strHtsFee.IsEmpty())	return;

	CString jmdg; m_pEditJmdg->GetWindowText(jmdg);//주문가격
	CString jmcn; m_pEditJmcn->GetWindowText(jmcn);//주문수량

	jmdg.Remove(',');
	jmcn.Remove(',');

	const long lAmt = atol(jmdg) * atol(jmcn);
	long lTmpFee=0, lFeeAmt=0, lTmpTax=0, lTaxAmt=0, lJungsanAmt=0;

	const int nType = m_pTool->GetCodeType(GetJcod());

	//수수료계산
	//수수료는 원단위이하 절삭

	if(!m_bStaff)
	{
		if(!m_strHtsFee.Compare("+0.00012500") && !m_strHtsSaveFee.Compare("+000000000000500"))	//SAVE 수수료일때
		{
			if(lAmt >= 1000)
			{
				lTmpFee = (int)(lAmt * atof(m_strHtsFee));	//소수점이하 버림.
				lTmpFee = ((lTmpFee /10) * 10);				//수수료는 원단위 이하 절삭
				lFeeAmt = lTmpFee + atol(m_strHtsSaveFee);
			}
			else	//save수수료이면서 주문금액이 1000원이하이면 수수료는 0.1%에 +500원 제외
			{
				lTmpFee = (int)(lAmt * 0.001);				//소수점이하 버림.
				lTmpFee = ((lTmpFee /10) * 10);				//수수료는 원단위 이하 절삭
				lFeeAmt = lTmpFee;
			}
		}
		else
		{
			lTmpFee = (int)(lAmt * atof(m_strHtsFee));	//소수점이하 버림.
			lTmpFee = ((lTmpFee /10) * 10);				//수수료는 원단위 이하 절삭
			lFeeAmt = lTmpFee + atol(m_strHtsSaveFee);
		}
	}
	else	// 직원이면
	{
		if(nType == 3)	//프리보드는 수수료 0.4%
			lTmpFee = (int)(lAmt * 0.004);
		else
			lTmpFee = (int)(lAmt * 0.005);

		lTmpFee = ((lTmpFee /10) * 10);
		lFeeAmt = lTmpFee;
	}

	//거래세계산
	//거래세는 사사오입

	if(iMmgb == 1)	//매도
	{
		switch(nType)
		{
			case kospiType:		//거래소	: 농특세 0.15%, 거래세 0.15%, 거래세는 사사오입
				lTmpTax = (long)(((lAmt * 0.0015) *2) + 5);
				lTmpTax = ((lTmpTax /10) * 10);
				break;
			case kosdaqType:	//코스닥	: 거래세 0.3%, 거래세는 사사오입
				lTmpTax = (long)((lAmt * 0.003) + 5);
				lTmpTax = ((lTmpTax /10) * 10);
				break;
			case thirdType:		//프리보드 : 거래세 0.5%, 거래세는 사사오입
				lTmpTax = (long)((lAmt * 0.005) + 5);
				lTmpTax = ((lTmpTax /10) * 10);
				break;
			case etfType:		//ETF, ELW, ETN는 거래세 없음.
			case elwType:		
			case etnType:
				lTmpTax = 0;
				break;
		}
	}
	else	//매수
		lTmpTax = 0;

	lTaxAmt = lTmpTax;

	//정산금액 2015.03.19 KSJ 현재로는 매도매수인지 구분하기가 애매함. 나중에 사용하자
	if(iMmgb == 1)	//매도
	{
		lJungsanAmt = lAmt - lFeeAmt - lTaxAmt;
	}
	else			//매수
	{
		lJungsanAmt = lAmt + lFeeAmt;
	}
}

LRESULT CControlWnd::OnContainerAmount(WPARAM wParam, LPARAM lParam)
{
	CString strText;
	strText.Format("%s", (char*)lParam);

	if (atoi(strText) == 0)
		return 0;

	ClearMoney();
	m_pEditJmcn->SetWindowText(strText);

	OnButtonConAmt();
	return 0;
}

void CControlWnd::ClearMoney()
{
	m_money.chk = false;
	InvalidateRect(&m_money.rcChk);
}

void CControlWnd::SetSize(int depth)
{
	if (depth == 0)
		m_pCodeReg->ShowWindow(SW_SHOW);
	else
		m_pCodeReg->ShowWindow(SW_HIDE);
}

void CControlWnd::ClearJango()
{
	m_strMdCnt = "";
	m_strMsCnt = "";
//	m_output3 = "";
	m_strJango = "";
	m_strPgdg = "";
	m_strPgsi = "";
	m_strMaxMsCnt = "";
	m_strConAmt = "";

	m_maip	  = "";
	m_jango   = "";

	Invalidate();
}

void CControlWnd::ClearMaip()
{
	m_maip	  = "";
	m_jango   = "";
}

void CControlWnd::JangoClear(CString strJCod)
{
	if (strJCod != GetCodx())
		return;

	m_strJango = "";
	InvalidateRect(m_rectJango);
	m_jango = "";

	m_strPgdg = "";
	InvalidateRect(m_rectPgdg);
	m_maip = "";

	m_strPgsi = "";
	InvalidateRect(m_rectPgsi);

	GetParent()->SendMessage(UM_MYMESSAGE, MAKEWPARAM(UP_INVALIDATE_HOGA, NULL), NULL);
}

void CControlWnd::SetJango(CString strCodx, CString strJango, CString strPgdg, CString strPgsi)
{
	bool bHogaUpdate = false;

	if (strCodx != GetCodx())
	{
		return;
	}
	
	
	
	if (!strJango.IsEmpty())
	{
		m_strJango = strJango;
		InvalidateRect(m_rectJango);
		m_jango = strJango;
		bHogaUpdate = true;
	}
	if (!strPgdg.IsEmpty())
	{
		m_strPgdg = strPgdg;
		InvalidateRect(m_rectPgdg);
		m_maip = strPgdg;
		bHogaUpdate = true;
	}
	if (!strPgsi.IsEmpty())
	{
		if (strPgsi.GetAt(0) == _T('+'))
			strPgsi.Delete(0);

		m_strPgsi = strPgsi;
		InvalidateRect(m_rectPgsi);
	}

	if (bHogaUpdate)
	{
		GetParent()->SendMessage(UM_MYMESSAGE, MAKEWPARAM(UP_INVALIDATE_HOGA, NULL), NULL);
	}
}

CString CControlWnd::GetCodx()
{
	return CString((char*)GetParent()->SendMessage(UM_MYMESSAGE, MAKEWPARAM(UP_GET_CODX, NULL), NULL));
}

CString CControlWnd::GetJcod()
{
	return CString((char*)GetParent()->SendMessage(UM_MYMESSAGE, MAKEWPARAM(UP_GET_JCOD, NULL), NULL));
}

void CControlWnd::SendMaxJmcn(CString strJmdg, int iGubn)
{
	GetParent()->SendMessage(UM_MYMESSAGE, MAKEWPARAM(UP_SEND_MAXJMCN, iGubn), (LPARAM)strJmdg.operator LPCTSTR());
}

void CControlWnd::SendFee(CString strJmdg, int iGubn)
{
	GetParent()->SendMessage(UM_MYMESSAGE, MAKEWPARAM(UP_SEND_FEE, iGubn), (LPARAM)strJmdg.operator LPCTSTR());
}

void CControlWnd::AddJCod(CString strJCode, CString strJName)
{
	m_pCodeReg->AddJCod(strJCode, strJName);
}

void CControlWnd::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CPoint ptScreen = point;
	ClientToScreen(&ptScreen);
	m_pTool->PopupMenu(MAP_NAME, ptScreen.x, ptScreen.y);
	
	CWnd::OnRButtonUp(nFlags, point);
}

void CControlWnd::OnChangeJmcn()
{
// 	OnButtonMDCan();
// 	OnButtonMSCan();
// 	OnButtonMaxMS();
	OnButtonConAmt();
}

void CControlWnd::Palette()
{
	m_clrBG = m_pTool->GetIndexColor(COLOR_CONTENTS_BK);
	m_clrDT	= m_pTool->GetIndexColor(COLOR_DATA);
	m_clrTFG = m_pTool->GetIndexColor(COLOR_TEXT);

	m_ppenLine = m_pTool->RequestPen(m_pTool->GetIndexColor(COLOR_LINE), 1, PS_SOLID);

	m_pCodeReg->Palette();

	Invalidate();
}

BOOL CControlWnd::GetWaitMode() 
{
	CMapWnd* pMapWnd = (CMapWnd*)GetParent();

	if (pMapWnd)
		return pMapWnd->GetWaitMode();
	else
		return FALSE;
}

void CControlWnd::SetWaitMode(BOOL bWait) 
{
	CMapWnd* pMapWnd = (CMapWnd*)GetParent();

	if (pMapWnd)
		pMapWnd->SetWaitMode(bWait);
}


void CControlWnd::ChangeFont(double* lRate, CFont* pfont, CFont* boldFont)
{
	m_pfont = pfont;

	
	CRect wrc, rc;
	GetClientRect(&rc);
	int iw{}, ih{}, iright{}, ibottom{};
	int ileft = rc.left + GAP;
	int itop = rc.top + GAP;
	int igap2 = GAP;
	const int ibtngap = 1;

	//체결버튼
	m_pBtnAccount->GetWindowRect( &wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width() , ih = wrc.Height() ;
	iw = iw + (int)(lRate[0] * (float)iw);
	ih = ih + (int)(lRate[1] * (float)ih);

	
	wrc.SetRect(wrc.left, INNER_FRAME_SPACE, wrc.left + iw, INNER_FRAME_SPACE+ih);
	m_pBtnAccount->SetFont(m_pfont);
	m_pBtnAccount->MoveWindow(wrc); 
	iright = wrc.right;
	ibottom = wrc.top+ih + GAP;

	//차트버튼
	m_pBtnChart->GetWindowRect( &wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width() , ih = wrc.Height() ;
	iw = iw + (int)(lRate[0] * (float)iw);
	ih = ih + (int)(lRate[1] * (float)ih);
	wrc.SetRect(iright + ibtngap, wrc.top, iright + ibtngap + iw, wrc.top+ih);
	m_pBtnChart->SetFont(m_pfont);
	m_pBtnChart->MoveWindow(wrc); 
	iright = wrc.right;
	ibottom = wrc.top+ih + GAP;

	//잔고버튼
	m_pBtnRevaluate->GetWindowRect( &wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width() , ih = wrc.Height() ;
	iw = iw + (int)(lRate[0] * (float)iw);
	ih = ih + (int)(lRate[1] * (float)ih);
	wrc.SetRect(iright + ibtngap, wrc.top, iright + ibtngap + iw, wrc.top+ih);
	m_pBtnRevaluate->SetFont(m_pfont);
	m_pBtnRevaluate->MoveWindow(wrc); 

	iright = wrc.right;
	

	itop = ibottom;
	iw = m_sRc1.Width() , ih = m_sRc1.Height() ;
	iw = iw + (int)(lRate[0] * (float)iw);
	ih = ih + (int)(lRate[1] * (float)ih);

	m_sRc1.SetRect(ileft, itop, ileft + iw, itop+ih); //첫박스
	ibottom = itop + GAP;
	ileft = ileft + GAP;

	//매도가능버튼
	m_pBtnMDCan->GetWindowRect( &wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width() , ih = wrc.Height() ;
	iw = iw + (int)(lRate[0] * (float)iw);
	ih = ih + (int)(lRate[1] * (float)ih);
	wrc.SetRect(ileft, ibottom, wrc.left + iw, ibottom+ih);
	m_pBtnMDCan->SetFont(m_pfont);
	m_pBtnMDCan->MoveWindow(wrc); 

	iright = wrc.right+GAP;
	m_rectMdCnt.SetRect(iright, wrc.top, m_sRc1.right -GAP,  wrc.bottom );

	//매수가능버튼
	wrc.top = wrc.bottom+GAP;
	wrc.bottom = wrc.top+ih;
	m_pBtnMSCan->SetFont(m_pfont);
	m_pBtnMSCan->MoveWindow(wrc); 
	ibottom = wrc.bottom + GAP;
	m_rectMsCnt.SetRect(iright, wrc.top, m_sRc1.right -GAP,  wrc.bottom );

	//최대매수버튼
	wrc.top = ibottom;
	wrc.bottom = ibottom+ih;
	m_pBtnMaxMS->SetFont(m_pfont);
	m_pBtnMaxMS->MoveWindow(wrc); 
	m_rectMaxMsCnt.SetRect(iright, wrc.top, m_sRc1.right -GAP,  wrc.bottom );

	wrc = m_rectMaxMsCnt;
	wrc.top = m_rectMaxMsCnt.bottom + GAP;
	wrc.bottom = wrc.top+ih;

	m_tRc3 = wrc; //주문가격
	m_tRc3.left = ileft;
	m_tRc3.right = wrc.left;
	m_pEditJmdg->SetFont(m_pfont);
	m_pEditJmdg->MoveWindow(wrc); 

	wrc = m_tRc3;
	wrc.top = m_tRc3.bottom + GAP;
	wrc.bottom = wrc.top+ih;

	ibottom = wrc.bottom + GAP;
	//2015.03.16 KSJ 약정가격
	wrc.top = ibottom;
	wrc.bottom = ibottom+ih;

	m_tRc4 = wrc; //약정가격

// 	m_pBtnConAmt->SetFont(m_pfont);
// 	m_pBtnConAmt->MoveWindow(wrc); 
	m_rectConAmt.SetRect(iright, wrc.top, m_sRc1.right -GAP,  wrc.bottom );


//두번째 박스
	iw = m_tRc4.Width();
	m_sRc2 = m_sRc1; 
	ih = m_sRc2.Height() ;
	ih = ih + (int)(lRate[1] * (float)ih);
	m_sRc2.top = m_sRc1.bottom + GAP;
	m_sRc2.bottom = m_sRc2.top + ih;
	m_rectJango = m_rectPgdg = m_rectPgsi = wrc;

	ih = m_rectConAmt.Height(); 

	int itmp = m_sRc2.Height() - (ih*3);
	igap2 = min(itmp / 4, 6); 
	igap2 = max(igap2, GAP);

	//잔고 텍스트
	m_rectJango.top = m_sRc2.top + igap2;
	m_rectJango.bottom = m_rectJango.top + ih;

	//평균단가 텍스트
	m_rectJangoS = m_rectJango;
	m_rectJangoS.left = m_tRc4.left;
	m_rectJangoS.right = m_tRc4.right;
	
	//평가손익텍스트
	m_rectPgdg.top = m_rectJango.bottom + igap2;
	m_rectPgdg.bottom = m_rectPgdg.top + ih;
	m_tRc5 = m_rectPgdg;
	m_tRc5.left = m_tRc4.left;
	m_tRc5.right = m_tRc4.right;
	
	m_rectPgsi.top = m_rectPgdg.bottom + igap2;
	m_rectPgsi.bottom = m_rectPgsi.top + ih;
	m_tRc6 = m_rectPgsi;
	m_tRc6.left = m_tRc4.left;
	m_tRc6.right = m_tRc4.right;
	m_sRc2.bottom = m_rectPgsi.bottom + igap2; 
	
////세번째
	m_sRc3 = m_sRc2;
	ih = m_sRc3.Height() ;
	ih = ih + (int)(lRate[1] * (float)ih);
	m_sRc3.top = m_sRc2.bottom + igap2;
	m_sRc3.bottom = m_sRc3.top + ih;

	
	ih = m_rectConAmt.Height(); 
	itmp = m_sRc3.Height() - (ih*4);
	igap2 = min(itmp / 5, 6); 
	igap2 = max(igap2, GAP);

	iw = m_tRc7.Width()  ;
	iw = iw + (int)(lRate[0] * (float)iw);
	m_tRc7 = m_tRc4; //수량
	m_tRc7.right = m_tRc7.left + iw;
	m_tRc7.top =  m_sRc3.top + igap2;
	m_tRc7.bottom = m_tRc7.top + m_tRc3.Height();

	//수량 선택버튼
	
	itop = wrc.top;
	m_pBtnJmcn->GetWindowRect( &wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width();
	iw = iw + (int)(lRate[0] * (float)iw);

	ileft = m_rectPgsi.right - iw - GAP;
	wrc.SetRect(ileft, m_tRc7.top, ileft+iw, m_tRc7.top+ih);
	m_pBtnJmcn->SetFont(m_pfont);
	m_pBtnJmcn->MoveWindow(wrc); 


	itmp = wrc.left;
	m_pSpinJmcn->GetWindowRect( &wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width() , ih = wrc.Height() ;
	iw = iw + (int)(lRate[0] * (float)iw);
	ih = ih + (int)(lRate[1] * (float)ih);

	ileft = itmp-iw;
	wrc.SetRect(ileft, m_tRc7.top, ileft + iw, m_tRc7.top+ih);
	m_pSpinJmcn->SetFont(m_pfont);
	m_pSpinJmcn->MoveWindow(wrc); 

	itmp = wrc.left;
	m_pEditJmcn->GetWindowRect( &wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width() , ih = wrc.Height() ;
	iw = iw + (int)(lRate[0] * (float)iw);
	ih = ih + (int)(lRate[1] * (float)ih);

	ileft = itmp-iw;
	wrc.SetRect(ileft, m_tRc7.top, ileft+iw, m_tRc7.top+ih);

	m_pEditJmcn->SetFont(m_pfont);
	m_pEditJmcn->MoveWindow(wrc); 

	////
//수량에디트 박스
/*	ileft = m_tRc7.right + 1;
	m_pEditJmcn->GetWindowRect( &wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width() , ih = wrc.Height() ;
	iw = iw + (int)(lRate[0] * (float)iw);
	ih = ih + (int)(lRate[1] * (float)ih);
	wrc.SetRect(ileft, m_tRc7.top, ileft + iw, m_tRc7.top+ih);
	m_pEditJmcn->SetFont(m_pfont);
	m_pEditJmcn->MoveWindow(wrc); 

//수량 스핀컨트롤
	iright = wrc.right;
	m_pSpinJmcn->GetWindowRect( &wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width() , ih = wrc.Height() ;
	iw = iw + (int)(lRate[0] * (float)iw);
	ih = ih + (int)(lRate[1] * (float)ih);
	wrc.SetRect(iright, m_tRc7.top, iright + iw, m_tRc7.top+ih);
	m_pSpinJmcn->SetFont(m_pfont);
	m_pSpinJmcn->MoveWindow(wrc); 

	//수량 선택버튼
	ileft = wrc.right + GAP;
	itop = wrc.top;
	m_pBtnJmcn->GetWindowRect( &wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width();
	iw = iw + (int)(lRate[0] * (float)iw);

	wrc.SetRect(ileft, itop, ileft+iw, itop+ih);
	m_pBtnJmcn->SetFont(m_pfont);
	m_pBtnJmcn->MoveWindow(wrc); 
*/

	itop = wrc.top + igap2;
	wrc = m_rectJango;
	wrc.top = itop;
	wrc.bottom = itop + ih;
	

	int ichkw = 0;
	int ichktxtw = m_money.rcText.Width(); 
	iw = m_money.rcChk.Width(); 
	iw = iw + (int)(lRate[0] * (float)iw);
	ichkw = iw;
	ichkw = min(ichkw, 18);

	wrc.top = m_tRc7.bottom + igap2;
	wrc.bottom = wrc.top + m_tRc7.Height();
	wrc.left = m_tRc7.left;
	wrc.right = wrc.left + iw;
	m_money.rcChk = m_money.rcText = wrc; //금액주문
	m_money.rcChk.right = m_money.rcChk.left + ichkw;

	
	
	iw = ichktxtw + (int)(lRate[0] * (float)ichktxtw);
	iw = max(iw, 56);
	m_money.rcText.left = m_money.rcChk.right;
	m_money.rcText.right = m_money.rcText.left + iw;//금액주문 text rectangle

	wrc = m_rectPgsi;
	wrc.left = m_money.rcText.right+2;
	wrc.top = m_money.rcText.top;
	wrc.bottom = wrc.top + m_money.rcText.Height();
	m_pEditMoney->SetFont(m_pfont);//금액주문에디트
	m_pEditMoney->MoveWindow(wrc); 

	//다중매매
	m_multi.rcChk = m_money.rcChk;
	m_multi.rcText = m_money.rcText;
	m_multi.rcChk.top =  m_multi.rcText.top = m_money.rcText.bottom + igap2;
	m_multi.rcChk.bottom =  m_multi.rcText.bottom = m_multi.rcChk.top + m_money.rcText.Height();

	wrc = m_rectPgsi;
	wrc.left = m_money.rcText.right+2;
	wrc.top = m_multi.rcText.top;
	wrc.bottom = wrc.top + m_multi.rcText.Height();
	m_cbMulti.SetFont(m_pfont);//다중매매 
	m_cbMulti.MoveWindow(wrc); 

	iw = m_condition[0].rcText.Width(); 
	ichktxtw = iw + (int)(lRate[0] * (float)iw);

	//없음, IOC, FOK 체크 영역
	wrc.top = wrc.bottom + igap2;
	wrc.bottom = wrc.top + m_multi.rcText.Height();
	m_condition[0].rcChk = m_condition[0].rcText = wrc;
	m_condition[1].rcChk = m_condition[1].rcText = wrc;
	m_condition[2].rcChk = m_condition[2].rcText = wrc;


	m_condition[0].rcChk.left = m_money.rcChk.left;
	ileft = m_condition[0].rcChk.left;
	m_condition[0].rcText.left = ileft;
	m_condition[0].rcChk.right = ileft + ichkw+1;
	m_condition[0].rcText.left = ileft + ichkw;

	ichktxtw = GetTextWidth(m_condition[0].data);
	m_condition[0].rcText.right = m_condition[0].rcText.left + ichktxtw;

	ileft = m_condition[0].rcText.right + GAP;
	m_condition[1].rcChk.left = ileft;
	m_condition[1].rcChk.right = ileft + ichkw;
	m_condition[1].rcText.left = ileft + ichkw+1;
	ichktxtw = GetTextWidth(m_condition[1].data);
	m_condition[1].rcText.right = m_condition[1].rcText.left + ichktxtw;

	ileft = m_condition[1].rcText.right + 1;
	m_condition[2].rcChk.left = ileft;
	m_condition[2].rcChk.right = ileft + ichkw+1;
	m_condition[2].rcText.left = ileft + ichkw;

	ichktxtw = GetTextWidth(m_condition[2].data);
	m_condition[2].rcText.right = m_condition[2].rcText.left + ichktxtw;
	m_sRc3.bottom = m_condition[2].rcText.bottom + igap2; 

//예약틱
	ih = m_sRc4.Height() ;
	m_sRc4 = m_sRc3;
	ih = ih + (int)(lRate[1] * (float)ih);
	m_sRc4.top = m_sRc3.bottom + GAP;
	m_sRc4.bottom = m_sRc4.top + ih;

	m_tRc8 = m_tRc3;
	m_tRc8.top = m_sRc4.top + GAP;
	m_tRc8.bottom = m_tRc8.top + m_tRc3.Height();
	m_tRc8.right = m_tRc8.left + GetTextWidth(m_text8) + 4;
	
	
	m_pEditTick->GetWindowRect( &wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width();
	iw = iw + (int)(lRate[0] * (float)iw);
	wrc.left = (m_tRc8.right + 2)/2;//m_rectPgsi.left;

//	if( (m_rectPgsi.Width() - wrc.left) < iw) iw = m_rectPgsi.right - 24;
	itmp = m_rectPgsi.Width() - 18;
//	CString ss;
//	ss.Format("%d-%d-%d-%d", iw, m_rectPgsi.Width(), wrc.left, itmp);
//	AfxMessageBox(ss);

	if(itmp < iw) iw = itmp;
	wrc.right = (wrc.left + iw)/2;
	wrc.top = m_tRc8.top;
	wrc.bottom = m_tRc8.bottom;

	//틱입력에디트
	m_pEditTick->SetFont(m_pfont);
	m_pEditTick->MoveWindow(wrc); 
	
	wrc.left = wrc.right/2;
	wrc.right = m_rectPgsi.right/2;
	m_pSpinTick->SetFont(m_pfont);//틱입력에디트 스핀
	m_pSpinTick->MoveWindow(wrc); 
	m_sRc4.bottom = wrc.bottom + GAP; 

	m_click.rcChk = m_multi.rcChk;
	m_click.rcText = m_money.rcText;
	m_click.rcChk.top =  m_click.rcText.top = wrc.bottom + igap2;
	m_click.rcChk.bottom =  m_click.rcText.bottom = m_click.rcChk.top + m_click.rcText.Height();

	m_sijang.rcChk = m_multi.rcChk;
	m_sijang.rcText = m_money.rcText;
	m_sijang.rcChk.top =  m_click.rcText.top = wrc.bottom + igap2;
	m_sijang.rcChk.bottom =  m_click.rcText.bottom = m_click.rcChk.top + m_click.rcText.Height();


	itop = wrc.bottom + GAP + GAP;
	//종목추가 포함하단윈도우
	m_pCodeReg->GetWindowRect( &wrc);
	ScreenToClient(&wrc);
	ih = wrc.Height() ;
	ih = ih + (int)(lRate[1] * (float)ih);
	wrc.left = m_tRc3.left;
	wrc.right = m_rectPgsi.right;
	wrc.top = itop;
	wrc.bottom = itop + ih;
	m_pCodeReg->ChangeFont(lRate, m_pfont);
	m_pCodeReg->MoveWindow(wrc); 
}

void CControlWnd::LoadBuffer()
{
	CString sval;
	sval = ((CMapWnd*)GetParent())->GetBuffer(_T("JumunPrc"));
	m_pEditJmdg->SetWindowText(sval);

	//수량
	sval = ((CMapWnd*)GetParent())->GetBuffer(_T("JVol"));
	if(sval.IsEmpty()) sval = "0";
	m_pEditJmcn->SetWindowText(sval);

	//금액주문
	sval = ((CMapWnd*)GetParent())->GetBuffer(_T("JAmt"));
	if(sval.IsEmpty()) sval = "0";
	m_pEditMoney->SetWindowText(sval);

	sval = ((CMapWnd*)GetParent())->GetBuffer(_T("MoneyChk"));
	m_money.chk = atoi(sval);
	InvalidateRect(m_money.rcChk);


	//다중매매
	sval = ((CMapWnd*)GetParent())->GetBuffer(_T("JMulti"));
	if(!sval.IsEmpty())
	{
		const int ival = atoi(sval);
		m_cbMulti.SetCurSel(ival);
	}

	sval = ((CMapWnd*)GetParent())->GetBuffer(_T("MultiChk"));
	m_multi.chk = atoi(sval);
	InvalidateRect(m_multi.rcChk);

//	sval = ((CMapWnd*)GetParent())->GetBuffer(_T("ConditionChk"));
//	int ival = atoi(sval);
	for(int i=0; i<3; i++) m_condition[i].chk = 0;
	m_condition[0].chk = 1;
	InvalidateRect(m_condition[0].rcChk);

	OnButtonConAmt();

}

void CControlWnd::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == 101)
	{
		KillTimer(nIDEvent);
		LoadBuffer();
	}

	CWnd::OnTimer(nIDEvent);
}


int CControlWnd::GetTextWidth(CString sText)
{
	CClientDC	dc(this);
	if(sText.IsEmpty() ) sText = "....";
	
	CFont* pOldFont = dc.SelectObject( m_pfont);
	const CSize tsize = dc.GetTextExtent(sText); 

	dc.SelectObject( pOldFont);
	DeleteObject(pOldFont);
	return tsize.cx + 5;
}

BOOL CControlWnd::GetOneClickJumun()
{
	return m_click.chk;
}

BOOL CControlWnd::GetCheckMarket()
{
	return m_sijang.chk;
}

void	CControlWnd::ClearChegGrid()
{
	m_pCodeReg->ClearChegGrid();
}

void	CControlWnd::SetRealData(CString time, CString curr, CString cvol)
{
	m_pCodeReg->SetRealData(time, curr, cvol);
}

void CControlWnd::SetAmountRemain( bool flag )
{
	m_pAmount->SetAlways(flag);
}

void CControlWnd::DrawBitmap(CDC* pDC, CRect rc, HBITMAP hBitmap)
{
	
	HDC hDC = ::CreateCompatibleDC (NULL);
	SelectObject (hDC, hBitmap); //icon
	::BitBlt( pDC->m_hDC, rc.left, rc.top, rc.Width(), rc.Height(),	hDC, 0, 0, SRCCOPY);
//	::TransparentBlt(pDC->m_hDC, rc.left, rc.top, rc.Width(), rc.Height(), hDC, 0, 0, rc.Width(), rc.Height(), 0xFF00FF);
	
	::DeleteDC (hDC);
}

void CControlWnd::SetPrgbRadio(int index, CString sgubn)
{
	m_cbCrType.SetCurSel(0);
	for(int ii = 0 ; ii < 3 ; ii++)
	{
		if(ii == index)
			m_condition[ii].chk = 1;
		else
			m_condition[ii].chk = 0;

		InvalidateRect(m_condition[ii].rcChk);
	}

	int icheck = 0;
	for (int jj = 0; jj < 3; jj++)
	{
		if(m_condition[jj].chk)
			icheck  = jj;
	}
	
	m_pBtnMDCan->EnableWindow(TRUE);  //매도가능
	m_pBtnMSCan->EnableWindow(TRUE);  //매수가능
	m_pBtnMaxMS->EnableWindow(TRUE);  //최대매수

	m_strMdCnt = "";
	InvalidateRect(&m_rectMdCnt);
	m_strMsCnt = "";
	InvalidateRect(&m_rectMsCnt);
	m_strMaxMsCnt = "";
	InvalidateRect(&m_rectMaxMsCnt);
	
	if(icheck == 1)
		m_pBtnMaxMS->EnableWindow(FALSE);  //최대매수
	
	if(icheck == 2)
	{
		m_pBtnMDCan->EnableWindow(FALSE);  //매도가능
		m_pBtnMSCan->EnableWindow(FALSE);  //매수가능
		m_pBtnMaxMS->EnableWindow(FALSE);  //최대매수
	}

	if(index == 1)  //신용인경우
	{
		if(sgubn == "03")  //자기융자 
			m_cbCrType.SetCurSel(1);
		else if(sgubn == "01")  //유통융자
			m_cbCrType.SetCurSel(0);
	}
}

int CControlWnd::GetCondition()
{
	for(int ii = 0 ; ii < 3 ; ii++)
	{
		if(m_condition[ii].chk == 1)
			return ii;
	}
	return 0;
}

CString CControlWnd::GetCrdType()
{
	CString sResult;
	sResult = "";

	if(m_cbCrType.GetCurSel() == 0)   //유통융자
		sResult = "01";
	else if(m_cbCrType.GetCurSel() == 1)  //자기융자 
		sResult = "03";

	return sResult;
}