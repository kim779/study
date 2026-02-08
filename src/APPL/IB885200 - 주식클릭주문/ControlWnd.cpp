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
}	

CControlWnd::~CControlWnd()
{
/*	if (m_pBtnAccount)
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
	if (m_pCodeReg)
		delete m_pCodeReg;
	if (m_pAmount)
	{
		m_pAmount->DestroyWindow();
		delete m_pAmount;
	}
*/
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
	ON_WM_CREATE()
	ON_WM_RBUTTONUP()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER + 200, OnContainerAmount)
//	ON_EN_UPDATE(ID_EDIT_JMCN, OnChangeJmcn)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CControlWnd message handlers
int CControlWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
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

	m_strJangoS = "잔    고";
	m_text5 = "평균단가";
	m_text6 = "평가손익";
	m_text7 = "수량 ";
	m_text8 = "예약주문틱";

	CRect rc, cRc;
	GetWindowRect(&rc);


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
	m_pBtnChart = std::make_unique <CfxImgButton>();
	m_pBtnChart->Create("\n종합차트", cRc, this, ID_CTRL_CHART, FALSE, TRUE);
	m_pBtnChart->SetWindowText("차트");
	m_pBtnChart->SetFont(m_pfont);
	m_pBtnChart->SetTextColor(m_pTool->GetIndexColor(69));

	cRc.SetRect(cRc.right + ibtngap, cRc.top, cRc.right + ibtngap + ibtn3w, cRc.bottom);
	m_pBtnRevaluate = std::make_unique <CfxImgButton>();
	m_pBtnRevaluate->Create("\n보유잔고 및 평가손익", cRc, this, ID_CTRL_REVAL, FALSE, TRUE);
	m_pBtnRevaluate->SetWindowText("잔고");
	m_pBtnRevaluate->SetFont(m_pfont);
	m_pBtnRevaluate->SetTextColor(m_pTool->GetIndexColor(69));



// line 2 주문수량 조회
	m_tRc1.SetRect(cx, cRc.bottom + GAP * 2, cx + 70, cRc.bottom + GAP * 2 + CTRL_HEIGHT);
	m_tRc2 = m_tRc1;	m_tRc2.OffsetRect(0, CTRL_HEIGHT + GAP);
	m_tRc9 = m_tRc2;	m_tRc9.OffsetRect(0, CTRL_HEIGHT + GAP);
	m_tRc3 = m_tRc9;	m_tRc3.OffsetRect(0, CTRL_HEIGHT + GAP);

	m_rectMdCnt.SetRect(m_tRc1.right + GAP, m_tRc1.top, CTRL_WIDTH - GAP, m_tRc1.bottom);
	m_rectMsCnt = m_rectMdCnt;	m_rectMsCnt.OffsetRect(0, CTRL_HEIGHT + GAP);
	m_rectMaxMsCnt = m_rectMsCnt;	m_rectMaxMsCnt.OffsetRect(0, CTRL_HEIGHT + GAP);
	m_oRc3 = m_rectMaxMsCnt;
	m_oRc3.OffsetRect(0, CTRL_HEIGHT + GAP);
	m_pEditJmdg = std::make_unique<class C3DEdit>();
	m_pEditJmdg->Create(NULL, WS_CHILDWINDOW | WS_VISIBLE | WS_TABSTOP | ES_NUMBER | ES_RIGHT, m_oRc3, this, ID_EDIT_JMDG, m_pfont);

	m_pBtnMDCan = std::make_unique <CfxImgButton>(m_pfont);
	m_pBtnMDCan->Create("\n매도가능", m_tRc1, this, ID_CTRL_MDCAN, FALSE, TRUE);
	m_pBtnMDCan->SetWindowText("매도가능");
	m_pBtnMDCan->SetTextColor(m_pTool->GetIndexColor(69));
	m_pBtnMDCan->EnableWindow(FALSE);


	m_pBtnMSCan = std::make_unique <CfxImgButton>(m_pfont);
	m_pBtnMSCan->Create("\n매수가능", m_tRc2, this, ID_CTRL_MSCAN, FALSE, TRUE);
	m_pBtnMSCan->SetWindowText("매수가능");
	m_pBtnMSCan->SetTextColor(m_pTool->GetIndexColor(69));
	m_pBtnMSCan->EnableWindow(FALSE);


	m_pBtnMaxMS = std::make_unique <CfxImgButton>(m_pfont);
	m_pBtnMaxMS->Create("\n최대매수", m_tRc9, this, ID_CTRL_MAXMS, FALSE, TRUE);
	m_pBtnMaxMS->SetWindowText("최대매수");
	m_pBtnMaxMS->SetTextColor(m_pTool->GetIndexColor(69));
	m_pBtnMaxMS->EnableWindow(FALSE);


	cRc.SetRect(2, m_tRc1.top - GAP*1, CTRL_WIDTH, m_oRc3.bottom + GAP*1);
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
	m_pEditJmcn = std::make_unique<class C3DEdit>();
	m_pEditJmcn->Create(NULL, WS_CHILDWINDOW | WS_VISIBLE | WS_TABSTOP | ES_NUMBER | ES_RIGHT, cRc, this, ID_EDIT_JMCN, m_pfont);
	m_pEditJmcn->SetToParent(true);


	m_pSpinJmcn = std::make_unique<class CSpinCtrl>();
	m_pSpinJmcn->Create(NULL, cRc, this, ID_SPIN_JMCN, m_pfont);
	m_pSpinJmcn->SetBuddy(m_pEditJmcn.get());
	m_pSpinJmcn->SetRange32(0, 99999999);	
	m_pSpinJmcn->SetNaturalType(m_pEditJmcn.get(), 0, 9999999, 10);
	m_pEditJmcn->SetWindowText("0");

	cRc.SetRect(cRc.right + 2, cRc.top, cRc.right + 2 + 20, cRc.top + 20);
	m_pBtnJmcn = std::make_unique <CfxImgButton>(m_pfont);
	m_pBtnJmcn->Create("", cRc, this, ID_CTRL_JMCN, FALSE, TRUE);
	m_pBtnJmcn->SetTextColor(m_pTool->GetIndexColor(69));



	m_pAmount = std::make_unique <CContainerWnd>();
	m_pAmount->SetParent(m_pwndView);
	m_pAmount->Create(NULL, NULL, WS_CHILD|WS_VISIBLE, CRect(0, 0, 1, 1), this, -1);
	m_pAmount->CreateControl(m_pTool->GetHome(), "cx_Amount", "", CRect(0, 0, 1, 1), "/k1");




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

// line 6 다중매매 
	cRc.SetRect(cx, cRc.bottom + GAP * 3, cx + 70, cRc.bottom + GAP * 2 + CTRL_HEIGHT);
	m_multi.rcChk.SetRect(cRc.left, cRc.top, cRc.left + 13, cRc.bottom);
	m_multi.rcText.SetRect(m_multi.rcChk.right, cRc.top, cRc.right, cRc.bottom);
	m_multi.data = _T("다중매매");
	m_multi.chk = false;

	cRc.SetRect(cRc.right + 2, cRc.top, CTRL_WIDTH - GAP, cRc.bottom + 200);
	m_cbMulti.Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | CBS_DROPDOWNLIST, cRc, this, ID_COMBO_MULTI);
	m_cbMulti.SetFont(m_pfont);

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
	m_condition[0].data = _T("없음");
//	m_condition[0].chk = true;
	m_condition[0].chk = false;

	m_condition[1].rcChk.SetRect(m_condition[0].rcText.right + GAP*1, m_condition[0].rcText.top, m_condition[0].rcText.right + 13 + GAP*1, m_condition[0].rcText.bottom);
	m_condition[1].rcText.SetRect(m_condition[1].rcChk.right, m_condition[1].rcChk.top, m_condition[1].rcChk.right + 35, m_condition[1].rcChk.bottom);
	m_condition[1].data = _T("IOC");
	m_condition[1].chk = false;

	m_condition[2].rcChk.SetRect(m_condition[1].rcText.right + GAP*1, m_condition[1].rcText.top, m_condition[1].rcText.right + 13 + GAP*1, m_condition[1].rcText.bottom);
	m_condition[2].rcText.SetRect(m_condition[2].rcChk.right, m_condition[2].rcChk.top, m_condition[2].rcChk.right + 35, m_condition[2].rcChk.bottom);
	m_condition[2].data = _T("FOK");
	m_condition[2].chk = false;

	cRc.SetRect(2, m_tRc7.top - GAP * 1, CTRL_WIDTH, cRc.bottom);
	m_sRc3 = cRc;

// line 8 예약매매 주문틱 설정
	m_tRc8.SetRect(cx, cRc.bottom + GAP * 3, cx + 70, cRc.bottom + GAP * 2 + CTRL_HEIGHT);
	
	cRc.SetRect(m_tRc8.right + GAP, m_tRc8.top, CTRL_WIDTH - GAP, m_tRc8.bottom);
	m_pEditTick = std::make_unique<class C3DEdit>();
	m_pEditTick->Create(NULL, WS_CHILDWINDOW | WS_VISIBLE | WS_TABSTOP | ES_NUMBER | ES_RIGHT, cRc, this, ID_EDIT_TICK, m_pfont);
	m_pEditTick->SetWindowText("2");

	m_pSpinTick = std::make_unique<class CSpinCtrl>();
	m_pSpinTick->Create(NULL, cRc, this, ID_SPIN_TICK, m_pfont);
	m_pSpinTick->SetBuddy(m_pEditTick.get());
	m_pSpinTick->SetRange32(-999, 999);

	cRc.SetRect(2, m_tRc8.top - GAP * 1, CTRL_WIDTH, cRc.bottom + GAP * 1);
	m_sRc4 = cRc;

// line 9 종목등록
	cRc.SetRect(GAP, cRc.bottom + GAP * 3 - 1, CTRL_WIDTH - GAP, 519);
	//m_pCodeReg = new CCodeReg(this, m_pMapWnd, m_pView);
	m_pCodeReg = std::make_unique <CCodeReg>(m_pTool, true);
	m_pCodeReg->Create(NULL, "", WS_CHILD | WS_VISIBLE, cRc, this, ID_CTRL_CODEREG);
	//m_pCodeReg->Init(m_pTool->GetHome(), m_pTool->GetName(), true);

// etc
	SetButtonBitmaps();
	SetTimer(101, 250, NULL);
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

	sval.Format("%d", ichk);
	((CMapWnd*)GetParent())->SaveBuffer(_T("ConditionChk"), sval);

	if(m_pCodeReg)
	{
		m_pCodeReg->DestroyWindow();
		//delete m_pCodeReg;
		m_pCodeReg = nullptr;
	}

	if (m_pBtnJmcn)
	{
		::DestroyWindow(m_pBtnJmcn->m_hWnd);
		//delete m_pBtnJmcn;
		m_pBtnJmcn = nullptr;
	}
	if (m_pBtnMaxMS)
	{
		::DestroyWindow(m_pBtnMaxMS->m_hWnd);
		//delete m_pBtnMaxMS;
		m_pBtnMaxMS = nullptr;
	}
	if (m_pBtnMDCan)
	{
		::DestroyWindow(m_pBtnMDCan->m_hWnd);
		//delete m_pBtnMDCan;
		m_pBtnMDCan = nullptr;
	}
	if (m_pBtnMSCan)
	{
		::DestroyWindow(m_pBtnMSCan->m_hWnd);
		//delete m_pBtnMSCan;
		m_pBtnMSCan = nullptr;
	}
	if (m_pBtnChart)
	{
		::DestroyWindow(m_pBtnChart->m_hWnd);
		//delete m_pBtnChart;
		m_pBtnChart = nullptr;
	}
	if (m_pBtnAccount)
	{
		::DestroyWindow(m_pBtnAccount->m_hWnd);
		//delete m_pBtnAccount;
		m_pBtnAccount = nullptr;
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

	memDC.FillSolidRect(&m_rectMdCnt, m_clrDT);
	memDC.Rectangle(&m_rectMdCnt);
	memDC.FillSolidRect(&m_rectMsCnt, m_clrDT);
	memDC.Rectangle(&m_rectMsCnt);
	memDC.FillSolidRect(&m_rectMaxMsCnt, m_clrDT);
	memDC.Rectangle(&m_rectMaxMsCnt);

	DrawSiseText(&memDC, m_strMdCnt, m_rectMdCnt, DT_RIGHT, ST_COMMA | ST_NORMAL);
	DrawSiseText(&memDC, m_strMsCnt, m_rectMsCnt, DT_RIGHT, ST_COMMA | ST_NORMAL);
	DrawSiseText(&memDC, m_strMaxMsCnt, m_rectMaxMsCnt, DT_RIGHT, ST_COMMA | ST_NORMAL);

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
	
	DrawSiseText(&memDC, m_text8, m_tRc8, DT_RIGHT);
	memDC.Rectangle(&m_sRc4);
//	CRect rr = m_rectPgsi;
//	rr.InflateRect(2,2,2,2); 
//	memDC.Rectangle(&rr);

	

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

	int iPos{};
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

	for (int ii = 0; ii < 3; ii++)
	{
		if (m_condition[ii].rcChk.PtInRect(point) || m_condition[ii].rcText.PtInRect(point))
		{
			for (int jj = 0; jj < 3; jj++)
				m_condition[jj].chk = false;
			
			m_condition[ii].chk = true;
			
			for (int jj = 0; jj < 3; jj++)
				InvalidateRect(&m_condition[jj].rcChk);
			
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
		}
	}
	else if (m_rectMsCnt.PtInRect(point))
	{
		if (atoi(m_strMsCnt) > 0)
		{
			CString jmcn;
			jmcn.Format("%d", atoi(m_strMsCnt));
			m_pEditJmcn->SetWindowText(jmcn);
		}
	}
	else if (m_rectMaxMsCnt.PtInRect(point))
	{
		if (atoi(m_strMaxMsCnt) > 0)
		{
			CString jmcn;
			jmcn.Format("%d", atoi(m_strMaxMsCnt));
			m_pEditJmcn->SetWindowText(jmcn);
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
		}
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

	m_strMdCnt = m_strMsCnt = m_strMaxMsCnt = _T("");
	m_pEditJmdg->SetWindowText("");

	// 주문단위수량
	m_iJmdw = atoi(m_pTool->ToString(pMod->jmdw, sizeof(pMod->jmdw)));
	m_pSpinJmcn->SetNaturalType(m_pEditJmcn.get(), 0, 9999999, m_iJmdw);
	m_pEditJmcn->SetWindowText("0");

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
			return m_strCondition;
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
}

int CControlWnd::calculateCommission(int money)
{
	const int iTax = (int)(money * 0.00024);
	return iTax;
}

void CControlWnd::OnButtonAccount()
{
	CString param;
	//m_pTool->CreateView(typeVIEW, "DH705100", "/S/s/p5");
	m_pTool->CreateView(typeVIEW, "IB110200", "/t");
}

void CControlWnd::OnButtonChart()
{
	m_pTool->CreateView(typeVIEW, "IB700000", "/t");
}

void CControlWnd::OnButtonReval()
{
	m_pTool->CreateView(typeVIEW, "IB885100", "/t");
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

void CControlWnd::OnButtonJmcn()
{
	CRect rect;
	m_pBtnJmcn->GetWindowRect(&rect);
	ScreenToClient(&rect);
	
	m_pAmount->SetWindowPos(NULL, rect.right + 2, rect.top, 0, 0, SWP_NOZORDER|SWP_NOSIZE);
	m_pAmount->BtnClick("");

}

void CControlWnd::SetJmdg(CString jmdg)
{
	m_pEditJmdg->SetWindowText(jmdg);
}

void CControlWnd::SetMaxJmcn(CString strData, int iMmgb)
{
	struct _maxjmct_mod mod {};

	CopyMemory(&mod, (LPCTSTR)strData, min(strData.GetLength(), L_maxjmct_mod));

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

	}

TRACE("[!!!!!! 주문가능수량] %s\n", CString(mod.lOrdAbleQty, sizeof(mod.lOrdAbleQty)));
/*
	for (int ii = 0; ii < 9; ii++)
	{
		m_pTool->Parser(strData, '\t');
	}

	switch (iMmgb)
	{
	case TK_MDJMCN:
		m_strMdCnt = m_pTool->Parser(strData, '\t');
		if (m_strMdCnt.IsEmpty())
			m_strMdCnt = "0";
		InvalidateRect(&m_rectMdCnt);
		break;

	case TK_MSJMCN:
		m_pTool->Parser(strData, '\t');
		m_strMsCnt = m_pTool->Parser(strData, '\t');
		if (m_strMsCnt.IsEmpty())
			m_strMsCnt = "0";
		InvalidateRect(&m_rectMsCnt);
		break;

	case TK_MAXMSJMCN:
		m_strMaxMsCnt = m_pTool->Parser(strData, '\t');
		if (m_strMaxMsCnt.IsEmpty())
			m_strMaxMsCnt = "0";
		InvalidateRect(&m_rectMaxMsCnt);
		break;

	}
*/
}

void CControlWnd::SetCurr(CString curr)
{
	m_curr = curr;

//	m_strPgsi.Format("%.f", fabs(atof(m_curr)) * atoi(m_strJango) - atoi(m_cmsg));
//	if (atof(m_strPgdg) < fabs(atof(m_curr)))
//		m_strPgsi = "+" + m_strPgsi;
//	InvalidateRect(&m_rectPgsi);
}

LRESULT CControlWnd::OnContainerAmount(WPARAM wParam, LPARAM lParam)
{
	CString strText;
	strText.Format("%s", (char*)lParam);

	if (atoi(strText) == 0)
		return 0;

	ClearMoney();
	m_pEditJmcn->SetWindowText(strText);

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

void CControlWnd::SendMaxJmcn(CString strJmdg, int iGubn)
{
	GetParent()->SendMessage(UM_MYMESSAGE, MAKEWPARAM(UP_SEND_MAXJMCN, iGubn), (LPARAM)strJmdg.operator LPCTSTR());
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
/*
void CControlWnd::OnChangeJmcn()
{
	ClearMoney();
}
*/

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

//두번째 박스
	iw = m_tRc3.Width();
	m_sRc2 = m_sRc1; 
	ih = m_sRc2.Height() ;
	ih = ih + (int)(lRate[1] * (float)ih);
	m_sRc2.top = m_sRc1.bottom + GAP;
	m_sRc2.bottom = m_sRc2.top + ih;
	m_rectJango = m_rectPgdg = m_rectPgsi = wrc;

	ih = m_rectMaxMsCnt.Height(); 

	int itmp = m_sRc2.Height() - (ih*3);
	igap2 = min(itmp / 4, 6); 
	igap2 = max(igap2, GAP);

	//잔고 텍스트
	m_rectJango.top = m_sRc2.top + igap2;
	m_rectJango.bottom = m_rectJango.top + ih;

	//평균단가 텍스트
	m_rectJangoS = m_rectJango;
	m_rectJangoS.left = m_tRc3.left;
	m_rectJangoS.right = m_tRc3.right;
	
	//평가손익텍스트
	m_rectPgdg.top = m_rectJango.bottom + igap2;
	m_rectPgdg.bottom = m_rectPgdg.top + ih;
	m_tRc5 = m_rectPgdg;
	m_tRc5.left = m_tRc3.left;
	m_tRc5.right = m_tRc3.right;
	
	m_rectPgsi.top = m_rectPgdg.bottom + igap2;
	m_rectPgsi.bottom = m_rectPgsi.top + ih;
	m_tRc6 = m_rectPgsi;
	m_tRc6.left = m_tRc3.left;
	m_tRc6.right = m_tRc3.right;
	m_sRc2.bottom = m_rectPgsi.bottom + igap2; 
	
////세번째
	m_sRc3 = m_sRc2;
	ih = m_sRc3.Height() ;
	ih = ih + (int)(lRate[1] * (float)ih);
	m_sRc3.top = m_sRc2.bottom + igap2;
	m_sRc3.bottom = m_sRc3.top + ih;

	
	ih = m_rectMaxMsCnt.Height(); 
	itmp = m_sRc3.Height() - (ih*4);
	igap2 = min(itmp / 5, 6); 
	igap2 = max(igap2, GAP);

	iw = m_tRc7.Width()  ;
	iw = iw + (int)(lRate[0] * (float)iw);
	m_tRc7 = m_tRc3; //수량
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

//예약주문틱
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
	wrc.left = m_tRc8.right + 2;//m_rectPgsi.left;

//	if( (m_rectPgsi.Width() - wrc.left) < iw) iw = m_rectPgsi.right - 24;
	itmp = m_rectPgsi.Width() - 18;
//	CString ss;
//	ss.Format("%d-%d-%d-%d", iw, m_rectPgsi.Width(), wrc.left, itmp);
//	AfxMessageBox(ss);

	if(itmp < iw) iw = itmp;
	wrc.right = wrc.left + iw;
	wrc.top = m_tRc8.top;
	wrc.bottom = m_tRc8.bottom;

	//틱입력에디트
	m_pEditTick->SetFont(m_pfont);
	m_pEditTick->MoveWindow(wrc); 
	
	wrc.left = wrc.right;
	wrc.right = m_rectPgsi.right;
	m_pSpinTick->SetFont(m_pfont);//틱입력에디트 스핀
	m_pSpinTick->MoveWindow(wrc); 
	m_sRc4.bottom = wrc.bottom + GAP; 


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

	sval = ((CMapWnd*)GetParent())->GetBuffer(_T("ConditionChk"));
	int ival = atoi(sval);
	for(int i=0; i<3; i++) m_condition[i].chk = 0;
	m_condition[ival].chk = 1;
	InvalidateRect(m_condition[ival].rcChk);

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