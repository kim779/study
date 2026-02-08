// OrderWnd.cpp : implementation file
//

#include "stdafx.h"
#include "IB100600.h"
#include "OrderWnd.h"
#include "Define.h"
#include "MapWnd.h"
#include "ConfirmDlg.h"
#include "../../control/fx_misc/misctype.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define UM_PASSWORD	WM_USER+1111
/////////////////////////////////////////////////////////////////////////////
// COrderWnd

COrderWnd::COrderWnd(CWnd* pWizard)
	: m_pWizard(pWizard)
	//, m_pParent(NULL)
	//, m_pAccount(NULL)
	//, m_pAccName(NULL)
	//, m_pPassStatic(NULL)
	//, m_pPassword(NULL)
	//, m_pOrderConfirm(NULL)
	//, m_pClear(NULL)
	//, m_pRemain(NULL)
	//, m_pInterest(NULL)
	//, m_pOrderSend(NULL)
	//, m_pContrect(NULL)
	//, m_pBuy(NULL)
	//, m_pSell(NULL)
	//, m_pOrderTypeStatic(NULL)
	//, m_pOrderType(NULL)
	//, m_pConditionStatic(NULL)
	//, m_pCondition(NULL)
	//, m_pQuantityStatic(NULL)
	//, m_pQuantityShare(NULL)
	//, m_pQuantityPrice(NULL)
	//, m_pQuantityValue(NULL)
	//, m_pPriceUnitStatic(NULL)
	//, m_pPriceUnit(NULL)
	//, m_pPercent(NULL)
	//, m_pPercentStatic(NULL)
	//, m_pOrderGrid(NULL)
	//, m_bBlock(FALSE)
	//, m_bInit(FALSE)
{
	m_bFirst = FALSE;
	
}

COrderWnd::~COrderWnd()
{
	//_SAFEDESTROY(m_pAccount);
	//_SAFEDESTROY(m_pAccName);
	//_SAFEDESTROY(m_pPassStatic);
	//_SAFEDESTROY(m_pPassword);
	//_SAFEDESTROY(m_pOrderConfirm);
	//_SAFEDESTROY(m_pClear);
	//_SAFEDESTROY(m_pRemain);
	//_SAFEDESTROY(m_pInterest);
	//_SAFEDESTROY(m_pOrderSend);
	//_SAFEDESTROY(m_pContrect);
	//_SAFEDESTROY(m_pBuy);
	//_SAFEDESTROY(m_pSell);
	//_SAFEDESTROY(m_pOrderTypeStatic);
	//_SAFEDESTROY(m_pOrderType);
	//_SAFEDESTROY(m_pConditionStatic);
	//_SAFEDESTROY(m_pCondition);
	//_SAFEDESTROY(m_pQuantityStatic);
	//_SAFEDESTROY(m_pQuantityShare);
	//_SAFEDESTROY(m_pQuantityPrice);
	//_SAFEDESTROY(m_pQuantityValue);
	//_SAFEDESTROY(m_pPriceUnitStatic);
	//_SAFEDESTROY(m_pPriceUnit);

	//_SAFEDESTROY(m_pOrderGrid);
}


BEGIN_MESSAGE_MAP(COrderWnd, CWnd)
	//{{AFX_MSG_MAP(COrderWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CLEAR, OnClear)
	ON_BN_CLICKED(IDC_REMAIN, OnRemain)
	ON_BN_CLICKED(IDC_INTEREST, OnInterest)
	ON_BN_CLICKED(IDC_ORDERSEND, OnOrderSend)
	ON_BN_CLICKED(IDC_CONTRECT, OnContrect)
	ON_BN_CLICKED(IDC_BUY, OnBuy)
	ON_BN_CLICKED(IDC_SELL, OnSell)
	ON_CBN_SELCHANGE(IDC_ORDERTYPE, OnSelchangeOrderType)
	ON_CBN_SELCHANGE(IDC_CONDITION, OnSelchangeCondition)
	ON_CBN_SELCHANGE(IDC_PRICEUNIT, OnSelchangePriceUnit)
	ON_EN_CHANGE(IDC_QUANTITYVALUE, OnQuantityValue)
	ON_EN_CHANGE(IDC_PERCENT, OnPercent)
	ON_MESSAGE(WM_USER + 1, OnUser)
	ON_MESSAGE(UM_PASSWORD, OnMyPassword)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// COrderWnd message handlers

int COrderWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_pParent = (CMapWnd*)GetParent();

	CRect rcCtrl, rcClient;
	GetClientRect(&rcClient);
	rcCtrl.SetRect(SZ_TAB_HMARGIN, SZ_FORM_MARGIN, SZ_TAB_HMARGIN + SZ_CTRL_ACCN, SZ_FORM_MARGIN + SZ_CTRL_HEIGHT);

	m_pAccount = std::make_unique<CAccountCtrl>(m_pWizard, this, m_pParent->GetAxRoot());
	m_pAccount->CreateEx(WS_EX_TOPMOST, NULL, "ACCN", WS_VISIBLE | WS_CHILD | WS_TABSTOP, rcCtrl, this, IDC_ACCOUNT);
	m_pAccount->createAccountCtrl("AN10", KEY_ACCOUNT, m_pParent->GetIndexColor(CLR_PANNEL_BACK));
	
	rcCtrl.left = rcCtrl.right + 2;					// 계좌 컨트롤과 계좌명 컨트롤간의 간격은 1로 한다.
	rcCtrl.right = rcCtrl.left + SZ_CTRL_ACCNM;

	m_pAccName = std::make_unique <CfxStatic>();
	m_pAccName->Create(_T(""), WS_CHILD | WS_VISIBLE | SS_LEFT | WS_CLIPCHILDREN | 
							WS_CLIPSIBLINGS, rcCtrl, this, IDC_ACCOUNTNAME);

	rcCtrl.left = rcCtrl.right + SZ_CTRL_HGAP + 3;	// 컨트롤 그룹간 간격은 5로 한다. (SZ_CTRL_HGAP : 5px,  fxStatic내부 마진 2px)
	rcCtrl.right = rcCtrl.left + SZ_CTRL_4STATIC;
	
	m_pPassStatic = std::make_unique<CfxStatic>();
	m_pPassStatic->Create(_T("비밀번호"), WS_CHILD | WS_VISIBLE | SS_LEFT | WS_CLIPCHILDREN | 
							WS_CLIPSIBLINGS, rcCtrl, this, IDC_PASSSTATIC);

	rcCtrl.left = rcCtrl.right + 3;					// 캡션과 컨트롤간 간격은 5로 한다. (fxStatic 내부마진 2px)
	rcCtrl.right = rcCtrl.left + SZ_CTRL_4EDIT + 5;
	
	m_pPassword = std::make_unique<CfxEdit>();
	m_pPassword->Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_PASSWORD, rcCtrl, this, IDC_PASSWORD);
//	m_pPassword->Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP, rcCtrl, this, IDC_PASSWORD);
	

	rcCtrl.left = rcCtrl.right + SZ_CTRL_HGAP + 3;	// 컨트롤 그룹간 간격은 5로 한다. (SZ_CTRL_HGAP : 5px,  fxStatic내부 마진 2px)
	rcCtrl.right = rcCtrl.left + SZ_CHECK_5WIDTH;

	m_pOrderConfirm = std::make_unique<CButton>();
	m_pOrderConfirm->Create(_T("주문확인창"), WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | BS_AUTOCHECKBOX |
							BS_TOP|BS_BOTTOM, rcCtrl, this, IDC_ORDERCONFIRM);

///////////////// Temp 관심종목 버튼 제외
	rcCtrl.left = rcClient.right - (SZ_CTRL_4BUTTON * 4) - 3 - SZ_TAB_HMARGIN;
	rcCtrl.right = rcCtrl.left + SZ_CTRL_4BUTTON;

	m_pClear = std::make_unique<CfxImgButton>();
	m_pClear->Create("\nClear", rcCtrl, this, IDC_CLEAR, FALSE, TRUE);
	
	rcCtrl.left = rcCtrl.right + 1;
	rcCtrl.right = rcCtrl.left + SZ_CTRL_4BUTTON;

	m_pRemain = std::make_unique<CfxImgButton>();
	m_pRemain->Create("\n잔고수신", rcCtrl, this, IDC_REMAIN, FALSE, TRUE);

	rcCtrl.left = rcCtrl.right + 1;
	rcCtrl.right = rcCtrl.left + SZ_CTRL_4BUTTON;

	m_pOrderSend = std::make_unique<CfxImgButton>();
	m_pOrderSend->Create("\n일괄주문", rcCtrl, this, IDC_ORDERSEND, FALSE, TRUE);

	rcCtrl.left = rcCtrl.right + 1;
	rcCtrl.right = rcCtrl.left + SZ_CTRL_4BUTTON;

	m_pContrect = std::make_unique<CfxImgButton>();
	m_pContrect->Create("\n체결", rcCtrl, this, IDC_CONTRECT, FALSE, TRUE);

	rcCtrl.left = rcCtrl.right + 1;
	rcCtrl.right = rcCtrl.left + SZ_CTRL_1BUTTON;

///////////////////////////////////////
	rcCtrl.left = SZ_TAB_HMARGIN;
	rcCtrl.right = SZ_RADIO_2WIDTH;
	rcCtrl.top = rcCtrl.bottom + SZ_CTRL_VGAP + 1;
	rcCtrl.bottom = rcCtrl.top + SZ_CTRL_HEIGHT;

	m_pSell = std::make_unique<CButton>();
	m_pSell->Create(_T("매도"), WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | BS_AUTORADIOBUTTON | 
						WS_GROUP | BS_TOP|BS_BOTTOM , rcCtrl, this, IDC_SELL);

	rcCtrl.left = rcCtrl.right + SZ_CTRL_HGAP;
	rcCtrl.right = rcCtrl.left + SZ_RADIO_2WIDTH;

	m_pBuy = std::make_unique<CButton>();
	m_pBuy->Create(_T("매수"), WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | BS_AUTORADIOBUTTON | 
						BS_TOP|BS_BOTTOM , rcCtrl, this, IDC_BUY);

	rcCtrl.top -= 1;
	rcCtrl.bottom -= 1;
	rcCtrl.left = rcCtrl.right + SZ_CTRL_HGAP + 3;
	rcCtrl.right = rcCtrl.left + SZ_CTRL_4STATIC;
	
	m_pOrderTypeStatic = std::make_unique <CfxStatic>();
	m_pOrderTypeStatic->Create(_T("주문유형"), WS_CHILD | WS_VISIBLE | SS_LEFT | WS_CLIPCHILDREN | 
								WS_CLIPSIBLINGS, rcCtrl, this, IDC_ORDERTYPESTATIC);

	rcCtrl.left = rcCtrl.right + 3;					// 캡션과 컨트롤간 간격은 5로 한다. (fxStatic 내부마진 2px)
	rcCtrl.right = rcCtrl.left + SZ_COMBO_6WIDTH;

	m_pOrderType = std::make_unique<CComboBox>();
	m_pOrderType->Create(WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | CBS_DROPDOWNLIST, 
						CRect(rcCtrl.left, rcCtrl.top, rcCtrl.right, 200), this, IDC_ORDERTYPE);
	SetWindowTheme(m_pOrderType->GetSafeHwnd(), L"", L"");
	rcCtrl.left = rcCtrl.right + SZ_CTRL_HGAP + 3;
	rcCtrl.right = rcCtrl.left + SZ_CTRL_2STATIC;

	m_pConditionStatic = std::make_unique<CfxStatic>();
	m_pConditionStatic->Create(_T("조건"), WS_CHILD | WS_VISIBLE | SS_LEFT | WS_CLIPCHILDREN | 
								WS_CLIPSIBLINGS, rcCtrl, this, IDC_CONDITIONSTATIC);

	rcCtrl.left = rcCtrl.right + 3;
	rcCtrl.right = rcCtrl.left + SZ_COMBO_3WIDTH;
	
	m_pCondition = std::make_unique<CComboBox>();
	m_pCondition->Create(WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | CBS_DROPDOWNLIST, 
						CRect(rcCtrl.left, rcCtrl.top, rcCtrl.right, 200), this, IDC_CONDITION);
	SetWindowTheme(m_pCondition->GetSafeHwnd(), L"", L"");
	rcCtrl.left = rcCtrl.right + SZ_CTRL_HGAP + 3;
	rcCtrl.right = rcCtrl.left + SZ_CTRL_2STATIC;

	m_pQuantityStatic = std::make_unique<CfxStatic>();
	m_pQuantityStatic->Create(_T("수량"), WS_CHILD | WS_VISIBLE | SS_LEFT | WS_CLIPCHILDREN | 
								WS_CLIPSIBLINGS, rcCtrl, this, IDC_QUANTITYSTATIC);

	rcCtrl.top += 1;
	rcCtrl.bottom += 1;
	rcCtrl.left = rcCtrl.right + 3;
	rcCtrl.right = rcCtrl.left + SZ_RADIO_1WIDTH;

	m_pQuantityShare = std::make_unique<CButton>();
	m_pQuantityShare->Create(_T("주"), WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | BS_AUTORADIOBUTTON | 
						WS_GROUP | BS_TOP | BS_BOTTOM, rcCtrl, this, IDC_QUANTITYSHARE);
	
	rcCtrl.left = rcCtrl.right + 3;
	rcCtrl.right = rcCtrl.left + SZ_RADIO_2WIDTH;

	m_pQuantityPrice = std::make_unique <CButton>();
	m_pQuantityPrice->Create(_T("금액"), WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | BS_AUTORADIOBUTTON | 
						BS_TOP|BS_BOTTOM, rcCtrl, this, IDC_QUANTITYPRICE);

	rcCtrl.top -= 1;
	rcCtrl.bottom -= 1;
	rcCtrl.left = rcCtrl.right + 3;
	rcCtrl.right = rcCtrl.left + SZ_CTRL_6EDIT;
	m_pQuantityValue = std::make_unique <CfxEdit>();
	m_pQuantityValue->Create(WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | ES_RIGHT, rcCtrl, this, IDC_QUANTITYVALUE);
	m_pQuantityValue->SetPasswordChar(0);

	rcCtrl.left = rcCtrl.right + SZ_CTRL_HGAP + 3;
	rcCtrl.right = rcCtrl.left + SZ_CTRL_2STATIC;

	m_pPriceUnitStatic = std::make_unique <CfxStatic>();
	m_pPriceUnitStatic->Create(_T("단가"), WS_CHILD | WS_VISIBLE | SS_LEFT | WS_CLIPCHILDREN | 
								WS_CLIPSIBLINGS, rcCtrl, this, IDC_PRICEUNITSTATIC);

	rcCtrl.left = rcCtrl.right + 3;
	rcCtrl.right = rcCtrl.left + SZ_COMBO_7WIDTH;
	
	m_pPriceUnit = std::make_unique <CComboBox>();
	m_pPriceUnit->Create(WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | CBS_DROPDOWNLIST, 
						CRect(rcCtrl.left, rcCtrl.top, rcCtrl.right, 200), this, IDC_PRICEUNIT);

	SetWindowTheme(m_pPriceUnit->GetSafeHwnd(), L"", L"");
	rcCtrl.left = rcCtrl.right + 1;
	rcCtrl.right = rcCtrl.left + SZ_CTRL_3EDIT;

	m_pPercent = std::make_unique <CfxEdit>();
	m_pPercent->Create(WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | ES_RIGHT, rcCtrl, this, IDC_PERCENT);
	m_pPercent->SetPasswordChar(0);


	rcCtrl.left = rcCtrl.right;
	rcCtrl.right = rcCtrl.left + SZ_CTRL_1STATIC;

	m_pPercentStatic = std::make_unique <CfxStatic>();
	m_pPercentStatic->Create(_T("%"), WS_CHILD | WS_VISIBLE | SS_LEFT | WS_CLIPCHILDREN | 
								WS_CLIPSIBLINGS, rcCtrl, this, IDC_PERCENTSTATIC);
	m_iCmdH = rcCtrl.bottom + INNER_FRAME_SPACE;
	rcCtrl.left = 0;
	rcCtrl.right = rcClient.right;
	rcCtrl.top = rcCtrl.bottom + INNER_FRAME_SPACE+1;
	rcCtrl.bottom = rcClient.bottom - 1;

	m_pOrderGrid = std::make_unique<COrderGrid>();
	m_pOrderGrid->CreateGrid(WS_CHILD | WS_VISIBLE, rcCtrl, this, IDC_ORDERGRID);
	


	SetTimer(100, 50, NULL);
	return 0;
}

void COrderWnd::Init()
{
	CFont* pFont = m_pParent->GetNormalFont();

	m_pAccount->SetFont(pFont);

	m_pAccName->SetStyle(m_pAccName->GetStyle() | ssBORDER);
	m_pAccName->SetBkColor(m_pParent->GetIndexColor(CLR_SYMBOL_BACK));
	m_pAccName->SetFgColor(m_pParent->GetIndexColor(CLR_SYMBOL_FORE));
	m_pAccName->SetBorderColor(m_pParent->GetIndexColor(CLR_SYMBOL_LINE));
	m_pAccName->SetFont(pFont);
	m_pAccName->SetAlignment(alLEFT);

	m_pPassStatic->SetBkColor(m_pParent->GetIndexColor(CLR_PANNEL_BACK));
	m_pPassStatic->SetFgColor(m_pParent->GetIndexColor(CLR_STATIC_FORE));
	m_pPassStatic->SetFont(pFont);

	m_pPassword->LimitText(8);
	m_pPassword->SetFont(pFont);

	m_pOrderConfirm->SetFont(pFont);

	CString strPath = "";
	strPath.Format("%s\\Image\\", m_pParent->GetAxRoot());

	HBITMAP hbmp	= m_pParent->GetAxBitmap(strPath + "4btn.bmp")->operator HBITMAP();
	HBITMAP hbmp_dn	= m_pParent->GetAxBitmap(strPath + "4btn_dn.bmp")->operator HBITMAP();
	HBITMAP hbmp_hv = m_pParent->GetAxBitmap(strPath + "4btn_en.bmp")->operator HBITMAP();

	m_pClear->SetImgBitmap(hbmp, hbmp_dn, hbmp_hv);
	m_pClear->SetWindowText(_T("Clear"));
	m_pClear->SetFont(pFont);

	m_pRemain->SetImgBitmap(hbmp, hbmp_dn, hbmp_hv);
	m_pRemain->SetWindowText(_T("잔고조회"));
	m_pRemain->SetFont(pFont);

//	m_pInterest->SetImgBitmap(hbmp, hbmp_dn, hbmp_hv);
//	m_pInterest->SetWindowText(_T("관심종목"));
//	m_pInterest->SetFont(pFont);

	m_pOrderSend->SetImgBitmap(hbmp, hbmp_dn, hbmp_hv);
	m_pOrderSend->SetWindowText(_T("일괄주문"));
	m_pOrderSend->SetFont(pFont);

	m_pContrect->SetImgBitmap(hbmp, hbmp_dn, hbmp_hv);
	m_pContrect->SetWindowText(_T("체결"));
	m_pContrect->SetFont(pFont);

	m_pSell->SetFont(pFont);
	m_pSell->SetCheck(0);

	m_pBuy->SetFont(pFont);

	m_pOrderTypeStatic->SetBkColor(m_pParent->GetIndexColor(CLR_PANNEL_BACK));
	m_pOrderTypeStatic->SetFgColor(m_pParent->GetIndexColor(CLR_STATIC_FORE));
	m_pOrderTypeStatic->SetFont(pFont);

	m_pOrderType->SetFont(pFont);
	m_pOrderType->AddString(_T("지정가"));
	m_pOrderType->AddString(_T("시장가"));
	m_pOrderType->AddString(_T("조건부지정가"));
	m_pOrderType->AddString(_T("최유리지정가"));
	m_pOrderType->AddString(_T("최우선지정가"));
	m_pOrderType->AddString(_T("장개시전 시간외종가"));
	m_pOrderType->AddString(_T("시간외종가"));
	m_pOrderType->SetCurSel(0);

	m_pConditionStatic->SetBkColor(m_pParent->GetIndexColor(CLR_PANNEL_BACK));
	m_pConditionStatic->SetFgColor(m_pParent->GetIndexColor(CLR_STATIC_FORE));
	m_pConditionStatic->SetFont(pFont);

	m_pCondition->SetFont(pFont);
	m_pCondition->AddString(_T("없음"));
	m_pCondition->AddString(_T("FOK"));
	m_pCondition->AddString(_T("IOC"));
	m_pCondition->SetCurSel(0);

	m_pQuantityStatic->SetBkColor(m_pParent->GetIndexColor(CLR_PANNEL_BACK));
	m_pQuantityStatic->SetFgColor(m_pParent->GetIndexColor(CLR_STATIC_FORE));
	m_pQuantityStatic->SetFont(pFont);

	m_pQuantityShare->SetFont(pFont);
	m_pQuantityShare->SetCheck(0);

	m_pQuantityPrice->SetFont(pFont);
	
	m_pQuantityValue->SetFont(pFont);
	m_pQuantityValue->ModifyAttribute(0, attrNUMERIC);
	m_pQuantityValue->SetWindowText(_T("0"));

	m_pPriceUnitStatic->SetBkColor(m_pParent->GetIndexColor(CLR_PANNEL_BACK));
	m_pPriceUnitStatic->SetFgColor(m_pParent->GetIndexColor(CLR_STATIC_FORE));
	m_pPriceUnitStatic->SetFont(pFont);

	m_pPriceUnit->SetFont(pFont);
	m_pPriceUnit->AddString(_T("상한가"));
	m_pPriceUnit->AddString(_T("매도호가"));
	m_pPriceUnit->AddString(_T("현재가"));
	m_pPriceUnit->AddString(_T("매수호가"));
	m_pPriceUnit->AddString(_T("하한가"));
	m_pPriceUnit->AddString(_T("전일종가대비"));
	m_pPriceUnit->AddString(_T("예상체결가대비"));
	m_pPriceUnit->AddString(_T("기준가"));
	m_pPriceUnit->SetCurSel(2);
	
	m_pPercent->SetFont(pFont);
	m_pPercent->ModifyAttribute(0, attrNUMERIC);
	m_pPercent->SetWindowText(_T("100.00"));
	m_pPercent->ShowWindow(SW_HIDE);

	m_pPercentStatic->SetBkColor(m_pParent->GetIndexColor(CLR_PANNEL_BACK));
	m_pPercentStatic->SetFgColor(m_pParent->GetIndexColor(CLR_STATIC_FORE));
	m_pPercentStatic->SetFont(pFont);
	m_pPercentStatic->ShowWindow(SW_HIDE);

	m_bInit = TRUE;
//	SetTimer(100, 50, NULL);


}

void COrderWnd::SetInputFieldFocus()
{
	if (m_pAccount && m_pPassword && m_pRemain) 
	{
		if (m_pAccount->GetAccNo().IsEmpty())
			m_pAccount->SetFocus();
		else if (m_pPassword->GetText().IsEmpty())
			m_pPassword->SetFocus();
		else
			m_pRemain->SetFocus();
	}	
}

void COrderWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rcClient, rcbox,rc;
	GetClientRect(&rcClient);
	dc.FillSolidRect(&rcClient, m_pParent->GetIndexColor(CONTENTS_BACK_INDEX));

	m_pPriceUnitStatic->GetWindowRect(&rc);
	ScreenToClient(rc);
	rcbox = rcClient;
	m_iCmdH = rcbox.bottom = rc.bottom + INNER_FRAME_SPACE;
	dc.FillSolidRect(&rcbox, m_pParent->GetIndexColor(CLR_PANNEL_BACK));

//	rcClient.top = rcbox.bottom + INNER_FRAME_SPACE;
//	dc.FillSolidRect(&rcClient, m_pParent->GetIndexColor(CONTENTS_BACK_INDEX));
//	dc.FillSolidRect(&rcClient, 0xff0000);
}

HBRUSH COrderWnd::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CWnd::OnCtlColor(pDC, pWnd, nCtlColor);
	
    const UINT nID = pWnd->GetDlgCtrlID();

    if (nID == IDC_ORDERCONFIRM || nID == IDC_SELL || nID == IDC_BUY ||
		nID == IDC_QUANTITYSHARE || nID == IDC_QUANTITYPRICE)
    {    
		pDC->SetBkMode(TRANSPARENT);                              
		return (HBRUSH)::CreateSolidBrush(m_pParent->GetIndexColor(CLR_PANNEL_BACK)); //::GetStockObject(NULL_BRUSH);
    }

	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void COrderWnd::SetPallete()
{
	if (!m_bInit)
		return;

	m_pAccName->SetBkColor(m_pParent->GetIndexColor(CLR_SYMBOL_BACK));
	m_pAccName->SetFgColor(m_pParent->GetIndexColor(CLR_SYMBOL_FORE));
	m_pPassStatic->SetBkColor(m_pParent->GetIndexColor(CLR_PANNEL_BACK));
	m_pPassStatic->SetFgColor(m_pParent->GetIndexColor(CLR_STATIC_FORE));
	m_pOrderTypeStatic->SetBkColor(m_pParent->GetIndexColor(CLR_PANNEL_BACK));
	m_pOrderTypeStatic->SetFgColor(m_pParent->GetIndexColor(CLR_STATIC_FORE));
	m_pConditionStatic->SetBkColor(m_pParent->GetIndexColor(CLR_PANNEL_BACK));
	m_pConditionStatic->SetFgColor(m_pParent->GetIndexColor(CLR_STATIC_FORE));
	m_pQuantityStatic->SetBkColor(m_pParent->GetIndexColor(CLR_PANNEL_BACK));
	m_pQuantityStatic->SetFgColor(m_pParent->GetIndexColor(CLR_STATIC_FORE));
	m_pPriceUnitStatic->SetBkColor(m_pParent->GetIndexColor(CLR_PANNEL_BACK));
	m_pPriceUnitStatic->SetFgColor(m_pParent->GetIndexColor(CLR_STATIC_FORE));

	m_pOrderGrid->SetPallete();

	Invalidate();
}

void COrderWnd::SetHistory(CString strData)
{
	CString param;
	param.Format("1301\t%s", strData);

	m_pParent->Variant(codeCC, param);
}

CString COrderWnd::GetHistory(CString strSymbol)
{
	return m_pParent->Variant(historyCC, strSymbol);
}

void COrderWnd::OnClear()
{
	ClearOrderGrid();
}

void COrderWnd::ClearOrderGrid()
{
	CUGCell cell;
	m_pOrderGrid->SetNumberRows(DEF_ORDERGRIDROW);

	for (int i = 0; i < MAX_ORDERGRIDCOL; i++)
	{
		for (int j = 0; j < DEF_ORDERGRIDROW; j++)
		{
			m_pOrderGrid->GetCell(i, j, &cell);
			cell.ClearProperty(UGCELL_TEXT_SET);
			m_pOrderGrid->SetCell(i, j, &cell);
		}
	}

	m_pOrderGrid->GotoRow(0);
	m_pOrderGrid->RedrawAll();
}

void COrderWnd::OnRemain()
{
	CString strAccount(_T(""));
	CString strPassword(_T(""));
	CString strEncPass(_T(""));
	CString strUser(_T(""));
	CString strSendData(_T(""));
	struct _ledgerH ledger;
	struct s_mid mid {};

	m_pPassword->GetWindowText(strPassword);
	
	if (strPassword.IsEmpty())
	{
		AfxMessageBox("비밀번호를 입력하세요");
		return;
	}
	if (strPassword.GetLength()<4)
	{
		MessageBox("비밀번호를 확인하십시오.","IBK투자증권");
		return;
	}

	m_pParent->SaveBuffer(_T("PASSWORD1"), strPassword); 

	strAccount = m_pAccount->GetAccNo();
	strUser = m_pParent->Variant(userCC, "");
	strEncPass.Format("%s\t%s", strPassword, strAccount.Left(8));
	strPassword = (char *)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(encPASSx, modeACN), (LPARAM)(LPCTSTR)strEncPass);
	
	FillMemory(&ledger, L_ledgerH, ' ');

	m_pParent->SendMessage(WM_USER, ledgerDLL, (LPARAM)&ledger);
	CopyMemory(&ledger.svcd, _T("SONAQ052"), sizeof(ledger.svcd));
	CopyMemory(&ledger.usid, (LPCTSTR)strUser, strUser.GetLength());
	CopyMemory(&ledger.brno, strAccount.Left(3), 3);
	CopyMemory(&ledger.rcnt, _T("0000"), sizeof(ledger.rcnt));
	
	ledger.fkey[0] = 'C';
	ledger.mkty[0] = '2';
	ledger.odrf[0] = '1';

	strSendData = CString((char*)&ledger, L_ledgerH);

	FillMemory(&mid, L_smid, ' ');

	CopyMemory(&mid.accn, strAccount, sizeof(mid.accn));
	CopyMemory(&mid.pswd, strPassword, strPassword.GetLength());
	mid.allf[0] = '0';

	strSendData += CString((char*)&mid, L_smid);
	
	m_bBlock = TRUE;
	m_pParent->SendTR("PIBOSJGO", strSendData, KEY_SREMAIN, 0);
}

void COrderWnd::ParsingSRemain(CString strData, int nLen, CString strMsg)
{
	if (strData.GetLength() < 15 + L_srec)
		return;

	CString strCode(_T(""));
	CUGCell cell;
	CString strVal(_T(""));
	CString strAccnount = strData.Left(11);
	int nRec = atoi((LPCTSTR)strData.Mid(11, 4));

	strData.Delete(0, 15);
	struct s_rec rec {};

	ClearOrderGrid();
	
	if (nRec >= m_pOrderGrid->GetNumberRows())
		m_pOrderGrid->SetNumberRows(nRec + 1);

	const int nDeleteCount = 0;

	for (int i = 0; i < nRec; i++)
	{
		CopyMemory(&rec, (LPCTSTR)strData.Left(L_srec), L_srec);

		strVal = CString(rec.xqty, sizeof(rec.xqty));

		_TRIMDATA(strVal);

		if (atoi((LPCTSTR)strVal) == 0)	// 매도가능수량이 0인 경우 삭제
		{
			m_pOrderGrid->DeleteRow(i);
			strData.Delete(0, L_srec);
			nRec--;
			i--;
			continue;
		}
		m_pOrderGrid->QuickSetText(9, i, strVal);

		strVal = CString(rec.cod2, sizeof(rec.cod2));
		_TRIMDATA(strVal);
		strVal.Delete(0);
		m_pOrderGrid->QuickSetText(1, i, strVal);

		strVal = CString(rec.hnam, sizeof(rec.hnam));
		m_pOrderGrid->QuickSetText(3, i, strVal);
		m_pOrderGrid->QuickSetText(4, i, _T("매도"));

		strVal = CString(rec.jggb, sizeof(rec.jggb));
		_TRIMDATA(strVal);

		if (strVal == _T("00"))
			strVal = _T("현금");
		else if (strVal == _T("01"))
		{
			strVal = CString(rec.sycd, sizeof(rec.sycd));
			strVal = _T("신용");
		}
		else
		{
			m_pOrderGrid->DeleteRow(i);
			strData.Delete(0, L_srec);
			nRec--;
			i--;
			continue;
		}
		m_pOrderGrid->QuickSetText(5, i, strVal);

		strVal = CString(rec.sydt, sizeof(rec.sydt));
		_TRIMDATA(strVal);
		m_pOrderGrid->QuickSetText(6, i, strVal);

		m_pOrderGrid->QuickSetText(7, i, _T("지정가"));
		m_pOrderGrid->QuickSetText(8, i, _T("없음"));

		strVal = CString(rec.curr, sizeof(rec.curr));
		_TRIMDATA(strVal);
		m_pOrderGrid->QuickSetText(10, i, strVal);

		m_pOrderGrid->SetRowHeight(i, SZ_ROWHEIGHT);
		strData.Delete(0, L_srec);
		m_pOrderGrid->GotoRow(i);	
	}

	m_pOrderGrid->SetPallete();	//2014.01.21 KSJ 열구분을 위해서 

	m_pOrderGrid->RedrawAll();

	CString strProc;
	strVal = m_pOrderGrid->QuickGetText(1, nRec - 1);
	strProc.Format("change_code\t%s", strVal);
	m_pParent->Procedure(strProc);

	m_bBlock = FALSE;
}

void COrderWnd::OnInterest()
{
	MessageBox("관심종목 팝업창");
}

void COrderWnd::OrderSend()
{
	int nCount = 0;
	int nRet = 0;
	CStringArray arrData;
	CStringArray arrConfirm;
	CString strData(_T(""));
	CString strCode(_T(""));		// 종목코드
	CString strOrderState(_T(""));	// 매매유형
	CString strOrderCash(_T(""));	// 현금 신용 구분
	CString strOrderType(_T(""));	// 호가구분
	CString strQuantity(_T(""));	// 수량
	CString strOrderPrice(_T(""));	// 주문단가
	CString strOrderCondition(_T(""));	// 주문조건
	CString strCount(_T(""));
	CString strUser(_T(""));
	CString strPassword(_T(""));
	CString strEncPass(_T(""));
	CString strSendData(_T(""));
	CString strAccount = m_pAccount->GetAccNo();
	CString strErrMsg;
	m_pPassword->GetWindowText(strPassword);
/*
	if (strAccount.IsEmpty())
	{
		AfxMessageBox("계좌번호를 입력하세요");
		return;
	}

	m_pPassword->GetWindowText(strPassword);
	
	if (strPassword.IsEmpty())
	{
		AfxMessageBox("비밀번호를 입력하세요");
		return;
	}
	if (strPassword.GetLength()<4)
	{
		MessageBox("비밀번호를 확인하십시오.","IBK투자증권");
		return;
	}
//	m_pParent->CheckPassWord(strAccount, strPassword, this);
*/

	nRet = m_pOrderGrid->GetSendData(&arrData);
	
	switch (nRet)
	{
	case 1:
		strErrMsg = _T("주문수량을 입력하세요");
		break;
	case 2:
		strErrMsg = _T("주문수량을 잘못 입력하였습니다.");
		break;
	case 3:
		strErrMsg = _T("주문단가를 입력하세요");
		break;
	case 4:
		strErrMsg = _T("주문단가를 잘못 입력하였습니다.");
		break;
	}

	if (nRet != 0)
	{
		AfxMessageBox(strErrMsg);
		return;
	}

	nCount = arrData.GetSize();

	if (nCount == 0)
		return;

	strUser = m_pParent->Variant(userCC, "");
// 	strEncPass.Format("%s\t%s", strPassword, strAccount.Left(8));
// 	strPassword = (char *)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(encPASSx, modeACN), (LPARAM)(LPCTSTR)strEncPass);

	struct i_com mid;

	FillMemory(&mid, L_i_com, ' ');

	if (m_pOrderConfirm->GetCheck() == 1)
	{
		CConfirmDlg dlg(this, m_pParent, 1);
		CString strConfirm(_T(""));
		
		for (int i = 0; i < nCount; i++)
		{
			strConfirm = strAccount + _T("\t");
			strConfirm += m_pAccount->GetAccName();
			strConfirm += _T("\t");
			strConfirm += arrData.GetAt(i);
			arrConfirm.Add(strConfirm);
		}
		
		dlg.SetOrderData(&arrConfirm);
		
		if (dlg.DoModal() == IDCANCEL)
			return;
	}

	//strCount.Format("%04d", nCount);

	//CopyMemory(mid.nrec, (LPCTSTR)strCount, strCount.GetLength());
	int nCnt = 0;

	for (int i = 0; i < nCount; i++)
	{
		struct irec rec {};
		strData = arrData.GetAt(i);

		if (strData.IsEmpty())
			continue;
		
		strCode = Parser(strData, _T("\t"));
		Parser(strData, _T("\t"));
		strOrderState = Parser(strData, _T("\t"));
		strOrderCash = Parser(strData, _T("\t"));
		Parser(strData, _T("\t"));
		strOrderType = Parser(strData, _T("\t"));
		strOrderCondition = Parser(strData, _T("\t"));
		strQuantity = Parser(strData, _T("\t"));
		strOrderPrice = Parser(strData, _T("\t"));
		CString istrOrderPrice, istrQuantity;
		istrOrderPrice  = strOrderPrice;
		istrQuantity	= strQuantity;
		istrOrderPrice.Remove(_T(','));
		istrQuantity.Remove(_T(','));

		//sss.Format("%s | %s | %s",strOrderPrice,strQuantity,strCode);
		//MessageBox(sss);
		/*
		BOOL bRet = CheckupMistakeOrder(this, atoi((LPCTSTR)istrOrderPrice), atoi((LPCTSTR)istrQuantity), strCode, "0");
		
		if (!bRet)
		{
			//m_pOrderGrid->SetCheckState(nCount, false);
			continue;
		}
		*/
		FillMemory(&rec, L_irec, ' ');

		rec.odgb[0] = _T('1');
		rec.mmgb[0] = strOrderState == _T("매도") ? _T('1') : _T('2');
		CopyMemory(rec.accn, strAccount, strAccount.GetLength());
//		CopyMemory(rec.pswd, strPassword, strPassword.GetLength());
		CopyMemory(rec.pswd, "HEAD", 4);	//2013.12.23 KSJ 일방향암호화
		CopyMemory(rec.cod2, strCode, strCode.GetLength());
		CopyMemory(rec.jqty, strQuantity, strQuantity.GetLength());
		CopyMemory(rec.jprc, strOrderPrice, strOrderPrice.GetLength());
		
		_TRIMDATA(strOrderType);

		if (strOrderType == _T("지정가"))
			CopyMemory(rec.hogb, _T("00"), sizeof(rec.hogb));
		else if (strOrderType == _T("시장가"))
			CopyMemory(rec.hogb, _T("03"), sizeof(rec.hogb));
		else if (strOrderType == _T("조건부지정가"))
			CopyMemory(rec.hogb, _T("05"), sizeof(rec.hogb));
		else if (strOrderType == _T("최유리지정가"))
			CopyMemory(rec.hogb, _T("06"), sizeof(rec.hogb));
		else if (strOrderType == _T("최우선지정가"))
			CopyMemory(rec.hogb, _T("07"), sizeof(rec.hogb));
		else if (strOrderType == _T("장개시전 시간외종가"))
			CopyMemory(rec.hogb, _T("61"), sizeof(rec.hogb));
		else if (strOrderType == _T("시간외종가"))
			CopyMemory(rec.hogb, _T("81"), sizeof(rec.hogb));

		if (strOrderType != _T("지정가"))
			CopyMemory(rec.jprc, _T("0000000000"), sizeof(rec.jprc));

		_TRIMDATA(strOrderCondition);

		if (strOrderCondition == _T("IOC"))
			rec.cdgb[0] = _T('1');
		else if (strOrderCondition == _T("FOK"))
			rec.cdgb[0] = _T('2');
		else
			rec.cdgb[0] = _T('0');

		rec.mdgb[0] = _T('2');

		if (strOrderCash == _T("현금"))
			rec.prgb[0] = _T('X');
		else
		{
			rec.prgb[0] = _T('B');

			CString strDate;
			SYSTEMTIME s;
		    GetLocalTime(&s);

			strDate.Format("003%04d%02d%02d", s.wYear, s.wMonth, s.wDay);
			CopyMemory(rec.usrf, (LPCTSTR)strDate, strDate.GetLength());
		}

		//CopyMemory(&mid.rec[i], &rec, L_irec);
		CopyMemory(&mid.rec[nCnt], &rec, L_irec);
		nCnt++;
	}
	strCount.Format("%04d", nCnt);
	CopyMemory(mid.nrec, (LPCTSTR)strCount, strCount.GetLength());


	char* pData = (char*)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, userCC), 0L);
	if ((long)pData > 1)
		strUser = pData;

	struct _ledgerH ledger;

	FillMemory(&ledger, L_ledgerH, ' ');
	m_pWizard->SendMessage(WM_USER, ledgerDLL, (LPARAM)&ledger);

	CopyMemory(&ledger.usid, (LPCTSTR)strUser, strUser.GetLength());
	CopyMemory(&ledger.brno, strAccount.Left(3), 3);
	CopyMemory(&ledger.rcnt, _T("0000"), sizeof(ledger.rcnt));

	m_pPassword->GetWindowText(strPassword);

	//2013.12.23 KSJ 일방향암호화
	if(!strPassword.IsEmpty())
	{
		CString pswd;
		pswd = m_pParent->GetEncPassword(strPassword);
		CopyMemory(&ledger.hsiz, "44", sizeof(ledger.hsiz));
		CopyMemory(&ledger.epwd, pswd, pswd.GetLength());
	}

	ledger.fkey[0] = 'C';
	ledger.mkty[0] = '1';
	ledger.odrf[0] = '2';

	strSendData = CString((char*)&ledger, L_ledgerH);
	CString ddd;
	ddd.Format("%d",nCnt);
	//MessageBox(ddd);
	//strSendData += CString((char*)&mid, nCount * L_irec + 4);
	strSendData += CString((char*)&mid, nCnt * L_irec + 4);
	m_pParent->SendTR("PIBOSODR", strSendData, KEY_SORDER, 0);
}

void COrderWnd::OnOrderSend()
{
	//본사직원 주문 안나가도록 예외 처리 [ Start ]
	if (!(long)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, orderCC), 0L))
	{
		if (!(long)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, orderCCx), 0L))
		{
			MessageBox("본사직원은 주문이 불가합니다.","IBK투자증권");
			return;
		}	
	}
	//본사직원 주문 안나가도록 예외 처리 [ End ]

	CString strUser(_T(""));
	CString strPassword(_T(""));
	CString strAccount = m_pAccount->GetAccNo();
	CString strErrMsg;

	if (strAccount.IsEmpty())
	{
		AfxMessageBox("계좌번호를 입력하세요");
		return;
	}

	m_pPassword->GetWindowText(strPassword);
	
	if (strPassword.IsEmpty())
	{
		AfxMessageBox("비밀번호를 입력하세요");
		return;
	}

	if (strPassword.GetLength()<4)
	{
		MessageBox("비밀번호를 확인하십시오.","IBK투자증권");
		return;
	}

	m_pParent->CheckPassWord(strAccount, strPassword, this);
}

void COrderWnd::OnContrect()
{
	CMenu* pMenu{};
	CRect  rcMenu;
	const int nCount = 0;

	pMenu = new CMenu;
	pMenu->CreatePopupMenu();
	pMenu->AppendMenu(MF_STRING, 0, "체결조회");
	pMenu->AppendMenu(MF_STRING, 1, "미체결조회");

	m_pContrect->GetWindowRect(&rcMenu);

	const int nSelectGroup = pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON | TPM_RETURNCMD,
								rcMenu.left, rcMenu.bottom, this);
	delete pMenu;

	CString strData(_T(""));
	if (nSelectGroup == 1)
		strData = _T("IB110100 /S /t");
	else if (nSelectGroup == 0)
		strData = _T("IB110200 /S /t");

	m_pWizard->SendMessage(WM_USER, MAKEWPARAM(viewDLL, typeVIEW), (LPARAM)(LPCTSTR)strData);
}

CString COrderWnd::Parser(CString &srcstr, CString substr)
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

void COrderWnd::OnSelchangeOrderType()
{
	CString strVal(_T(""));
	m_pOrderType->GetWindowText(strVal);
	m_pOrderGrid->UpdateCheckedRowData(7, strVal);
}

void COrderWnd::OnSelchangeCondition()
{
	CString strVal(_T(""));
	m_pCondition->GetWindowText(strVal);
	m_pOrderGrid->UpdateCheckedRowData(8, strVal);
}

void COrderWnd::OnBuy()
{
	m_pOrderGrid->UpdateCheckedRowData(4, _T("매수"));
}

void COrderWnd::OnSell()
{
	m_pOrderGrid->UpdateCheckedRowData(4, _T("매도"));
}

void COrderWnd::OnQuantityValue()
{
	CString strVal(_T(""));
	m_pQuantityValue->GetWindowText(strVal);

	if (m_pQuantityPrice->GetCheck() == 1)
		m_pOrderGrid->UpdateCheckedRowData(9, strVal, TRUE);
	else
		m_pOrderGrid->UpdateCheckedRowData(9, strVal);
	
}

CString COrderWnd::GetOrderState()
{
	if (m_pBuy->GetCheck() == 1)
		return _T("매수");
	else if (m_pSell->GetCheck() == 1)
		return _T("매도");
	else
		return _T("");
}

CString COrderWnd::GetCreditType()
{
	return _T("현금");
}

CString COrderWnd::GetOrderType()
{
	CString strVal(_T(""));
	m_pOrderType->GetWindowText(strVal);
	
	return strVal;
}

CString COrderWnd::GetCondition()
{
	CString strVal(_T(""));
	m_pCondition->GetWindowText(strVal);
	
	return strVal;
}

CString COrderWnd::GetQuantity()
{
	CString strVal(_T(""));
	m_pQuantityValue->GetWindowText(strVal);

	return strVal;
}

CString COrderWnd::GetOrderPrice()
{
	return _T("");
}

//2015.06.12 KSJ 기준가 구하기 추가
void COrderWnd::QuerySCHoga(BOOL bHoga, CString strCode, BOOL bStdPrc)
{
	if (!m_bInit)
		return;

	CStringArray arrCode;
	int nCount = 0;

	if (bHoga)
	{
		m_pOrderGrid->GetCheckCode(&arrCode);
		nCount = arrCode.GetSize();
	}
	else
		nCount = 1;

	if (nCount < 0)
		return;
	
	struct shmid mid;
	CString strSendData(_T(""));
	CString strCount(_T(""));
	CString strVal;
	
	strCount.Format("%04d", nCount);
	
	FillMemory(&mid, L_shmid, ' ');
	
	//2015.06.12 KSJ 기준가를
	if(bStdPrc)
		mid.gubn[0] = m_pPriceUnit->GetCount() - 1 + 0x30;
	else
		mid.gubn[0] = m_pPriceUnit->GetCurSel() + 0x30;
	
	CopyMemory(&mid.nrec, (LPCTSTR)strCount, sizeof(mid.nrec));
	
	if (bHoga)
	{
		for (int i = 0; i < nCount; i++)
		{
			strVal = arrCode.GetAt(i);
			struct i_code code;
			FillMemory(&code, L_i_code, ' ');
			
			CopyMemory(&code.cod2, strVal.Left(6), sizeof(code.cod2));
			CopyMemory(&mid.rec[i], &code, L_i_code);
		}
	}
	else
	{
		struct i_code code;
		FillMemory(&code, L_i_code, ' ');

		CopyMemory(&code.cod2, strCode.Left(6), sizeof(code.cod2));
		CopyMemory(&mid.rec[0], &code, L_i_code);
	}
	
	strSendData = CString((char*)&mid, nCount * L_i_code + 6);

	if(bStdPrc)
		m_pParent->SendTR("PIBO1006", strSendData, bHoga ? KEY_SHOGA_STDPRC : KEY_SCURR_STDPRC, 0);
	else
		m_pParent->SendTR("PIBO1006", strSendData, bHoga ? KEY_SHOGA : KEY_SCURR, 0);
}

void COrderWnd::ParsingSCHoga(CString strData, BOOL bHoga, BOOL bStdPrc)	//2015.06.12 KSJ 기준가 구하기 추가
{
	if (strData.IsEmpty())
		return;
	const char chGubn = strData.GetAt(0);
	int nCount = atoi((LPCTSTR)strData.Mid(1, 4));

	CString strCode(_T(""));
	CString strPrice(_T(""));
	CString strDan(_T(""));
	CStringArray arrPrice;
	struct o_price price {};

	strData.Delete(0, 5);

	for (int i = 0; i < nCount; i++)
	{
		CString strVal;
		CopyMemory(&price, (LPCTSTR)strData.Left(L_o_price), L_o_price);
		strData.Delete(0, L_o_price);

		strCode = CString(price.cod2, sizeof(price.cod2));
		strPrice = CString(price.price, sizeof(price.price));

		_TRIMDATA(strCode);
		_TRIMDATA(strPrice);

		if (chGubn == _T('5') || chGubn == _T('6'))
		{
			CString strPercent;
			m_pPercent->GetWindowText(strPercent);

			double fPrice = atof((LPCTSTR)strPrice);
			const double fPercent = atof((LPCTSTR)strPercent);
			fPrice = fPrice * fPercent / 100;

		//	strPrice.Format("%d", (int)fPrice);
			strPrice = this->GetOrderPriceByMarket((int)fPrice, strCode);
		}

		strDan = CString(price.gdan, sizeof(price.gdan));
		_TRIMDATA(strDan);
		
		strVal.Format("%s\t%s\t%s", strCode, strPrice, strDan);
		arrPrice.Add(strVal);
	}

	m_pOrderGrid->SetSCData(&arrPrice, bHoga, bStdPrc);
}

void COrderWnd::OnSelchangePriceUnit()
{
	if (m_pPriceUnit->GetCurSel() == 5 || m_pPriceUnit->GetCurSel() == 6)
	{
		m_pPercent->ShowWindow(SW_SHOW);
		m_pPercentStatic->ShowWindow(SW_SHOW);
	}
	else
	{
		m_pPercent->ShowWindow(SW_HIDE);
		m_pPercentStatic->ShowWindow(SW_HIDE);
	}

	QuerySCHoga(TRUE);	
}

void COrderWnd::OnPercent()
{
	QuerySCHoga(TRUE);
}


void COrderWnd::ParsingSOrder(CString strData, int nLen, CString strMsg)
{
	if (strData.GetLength() < 4)
		return;

	int nCount = atoi((LPCTSTR)strData.Left(4));
	
	if (nCount == 0)
		return;

	strData.Delete(0, 4);

	CStringArray arrData;
	CString strVal(_T(""));

	for (int i = 0; i < nCount; i++)
	{
		if (strData.IsEmpty())
			break;

		strVal = strData.Left(L_orec);
		arrData.Add(strVal);

		strData.Delete(0, L_orec);
	}

	m_pOrderGrid->SetOrderResult(&arrData);
}

BOOL COrderWnd::IsQuantityPriceType()
{
	if (m_pQuantityPrice->GetCheck() == 1)
		return TRUE;
	
	return FALSE;
}

void COrderWnd::SetAccount(CString strAccount)
{
	if (m_pAccount)
		m_pAccount->TriggerAccount(strAccount);
}

long COrderWnd::OnUser(WPARAM wParam, LPARAM lParam)
{
	switch(LOWORD(wParam))
	{
	case 100:	// CAccountCtrl 계좌변경시 수신 Message
		{
			if (!m_pAccName)
				return 0;

			CString data = (char*)lParam;

			Parser(data, "\t");
			m_pAccName->SetText(Parser(data, "\t"));
			Parser(data, "\t");

			if(!m_bFirst)
			{
				CString sval = m_pParent->GetBuffer(_T("PASSWORD1") ); 
				if(sval.IsEmpty())
				{
//					if( m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, orderCC)) )
//						sval = "9999";
				}

				m_pPassword->SetWindowText(sval); 
				if(!sval.IsEmpty())
				{
					OnRemain();
				}
				m_bFirst = TRUE;
			}
			else m_pPassword->SetFocus();

		}

		break;

	case KEY_CHKPASSWORD:
		OrderSend();
		break;
	}
	return 0;
}

BOOL COrderWnd::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	if (wParam == IDC_PASSWORD)
		OnRemain();
	
	return CWnd::OnNotify(wParam, lParam, pResult);
}

LRESULT COrderWnd::OnMyPassword(WPARAM wParam, LPARAM lParam)
{
	//CString rcb = (char*)lParam;
	//MessageBox((char*)lParam);
	m_pPassword->SetWindowText((char*)lParam);
	//MessageBox("Received: "+rcb);
	if (m_pParent->m_pModCanWnd)
	{
		return 0;
	}else
	{
		return 1;
	}
}

bool COrderWnd::IsNumber(CString str)
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


void COrderWnd::ChangeSize(double* lRate, int ipoint, LPCTSTR lszname)
{
	CFont* pFont = m_pParent->GetNormalFont();

	CRect wrc, rc;
	GetClientRect(&rc);

	int iw=0, ih=0, itop=0, ileft=0;
	const int igap = 3;

	itop = igap;//max(3 + (ipoint - 10), 3);
	m_pAccount->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width(), ih = wrc.Height() ;
	iw = iw + (int)(lRate[0] * (float)iw );
	ih = ih + (int)(lRate[1] * (float)ih);
	m_pAccount->MoveWindow(wrc.left, itop, wrc.left + iw, ih );
	m_pAccount->ChangeFont(ipoint,  lszname);

	ileft = wrc.left + iw + igap;
//	itop = wrc.top;
	m_pAccName->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width(), ih = wrc.Height() ;
	iw = iw + (int)(lRate[0] * (float)iw );
	ih = ih + (int)(lRate[1] * (float)ih);
	m_pAccName->SetFont(pFont);
	m_pAccName->MoveWindow(ileft, itop, iw, ih );
	
	ileft +=  iw + 6;
	itop = wrc.top;
	m_pPassStatic->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width(), ih = wrc.Height() ;
	iw = iw + (int)(lRate[0] * (float)iw );
	ih = ih + (int)(lRate[1] * (float)ih);
	m_pPassStatic->SetFont(pFont);
	m_pPassStatic->MoveWindow(ileft, itop, iw, ih );
	const int ieditH = ih;

	ileft +=  iw + igap;
	m_pPassword->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width(), ih = wrc.Height() ;
	iw = iw + (int)(lRate[0] * (float)iw );
	ih = ih + (int)(lRate[1] * (float)ih);
	m_pPassword->SetFont(pFont);
	m_pPassword->MoveWindow(ileft, itop, iw, ih );

	ileft +=  iw + 6;
	m_pOrderConfirm->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width(), ih = wrc.Height() ;
	iw = iw + (int)(lRate[0] * (float)iw );
	ih = ih + (int)(lRate[1] * (float)ih);
	m_pOrderConfirm->SetFont(pFont);
	m_pOrderConfirm->MoveWindow(ileft, itop, iw, ih );

	const int ibtngap = 1;
	ileft +=  iw + igap;
	m_pClear->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width(), ih = wrc.Height() ;
	iw = iw + (int)(lRate[0] * (float)iw );
	ih = ih + (int)(lRate[1] * (float)ih);

	ileft = rc.right - (iw * 4) - (ibtngap*4) - igap;
	m_pClear->SetFont(pFont);
	m_pClear->MoveWindow(ileft, itop, iw, ih );
	ileft +=  iw + ibtngap;

	m_pRemain->SetFont(pFont);
	m_pRemain->MoveWindow(ileft, itop, iw, ih );
	ileft +=  iw + ibtngap;

	m_pOrderSend->SetFont(pFont);
	m_pOrderSend->MoveWindow(ileft, itop, iw, ih );
	ileft +=  iw + ibtngap;

	m_pContrect->SetFont(pFont);
	m_pContrect->MoveWindow(ileft, itop, iw, ih );

	itop = itop + ih + 2;

	//두번째 라인 컨트롤
	m_pSell->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width(), ih = wrc.Height() ;
	iw = iw + (int)(lRate[0] * (float)iw );
	ih = ih + (int)(lRate[1] * (float)ih);
	ileft = wrc.left ;
	
	m_pSell->SetFont(pFont);
	m_pSell->MoveWindow(ileft, itop, iw, ih );

	ileft +=  iw + igap;
	m_pBuy->SetFont(pFont);
	m_pBuy->MoveWindow(ileft, itop, iw, ih );

	ileft +=  iw + igap;
	m_pOrderTypeStatic->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width();
	iw = iw + (int)(lRate[0] * (float)iw );
	m_pOrderTypeStatic->SetFont(pFont);
	m_pOrderTypeStatic->MoveWindow(ileft, itop, iw, ih );

	ileft +=  iw + igap;
	m_pOrderType->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width();
	iw = iw + (int)(lRate[0] * (float)iw );
	m_pOrderType->SetFont(pFont);
	m_pOrderType->MoveWindow(ileft, itop, iw, ih );

	ileft +=  iw + igap;
	m_pConditionStatic->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width();
	iw = iw + (int)(lRate[0] * (float)iw );
	m_pConditionStatic->SetFont(pFont);
	m_pConditionStatic->MoveWindow(ileft, itop, iw, ih );

	ileft +=  iw + igap;
	m_pCondition->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width();
	iw = iw + (int)(lRate[0] * (float)iw );
	m_pCondition->SetFont(pFont);
	m_pCondition->MoveWindow(ileft, itop, iw, ih );

	ileft +=  iw + igap;
	m_pQuantityStatic->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width();
	iw = iw + (int)(lRate[0] * (float)iw );
	m_pQuantityStatic->SetFont(pFont);
	m_pQuantityStatic->MoveWindow(ileft, itop, iw, ih );
	
	ileft +=  iw + igap;
	m_pQuantityShare->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width();
	iw = iw + (int)(lRate[0] * (float)iw );
	m_pQuantityShare->SetFont(pFont);
	m_pQuantityShare->MoveWindow(ileft, itop, iw, ih );

	ileft +=  iw + igap;
	m_pQuantityPrice->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width();
	iw = iw + (int)(lRate[0] * (float)iw );
	m_pQuantityPrice->SetFont(pFont);
	m_pQuantityPrice->MoveWindow(ileft, itop, iw, ih );

	ileft +=  iw + igap;
	m_pQuantityValue->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width();
	iw = iw + (int)(lRate[0] * (float)iw );
	m_pQuantityValue->SetFont(pFont);
	m_pQuantityValue->MoveWindow(ileft, itop, iw, ih );

	ileft +=  iw + igap;
	m_pPriceUnitStatic->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width();
	iw = iw + (int)(lRate[0] * (float)iw );
	m_pPriceUnitStatic->SetFont(pFont);
	m_pPriceUnitStatic->MoveWindow(ileft, itop, iw, ih );


	ileft +=  iw + igap;
	m_pPriceUnit->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width();
	iw = iw + (int)(lRate[0] * (float)iw );
	m_pPriceUnit->SetFont(pFont);
	m_pPriceUnit->MoveWindow(ileft, itop, iw, ih );


	ileft +=  iw + igap;
	m_pPercent->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width();
	iw = iw + (int)(lRate[0] * (float)iw );
	m_pPercent->SetFont(pFont);
	m_pPercent->MoveWindow(ileft, itop, iw, ieditH );


	ileft +=  iw + igap;
	m_pPercentStatic->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width();
	iw = iw + (int)(lRate[0] * (float)iw );
	m_pPercentStatic->SetFont(pFont);
	m_pPercentStatic->MoveWindow(ileft, itop, iw, ih );


	itop += ih  +2;
	//세번째 라인 컨트롤
	ih = rc.Height()  - itop - 2;
	m_pOrderGrid->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	ileft = wrc.left ;
	iw = wrc.Width();
	iw = iw + (int)(lRate[0] * (float)iw );

	m_pOrderGrid->MoveWindow(ileft, itop, rc.Width(), ih);
	m_pOrderGrid->ChangeFont(pFont, lRate);
}

void COrderWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if(!m_pAccount) return;

}

void COrderWnd::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == 100)
	{
		KillTimer(nIDEvent);
		if(m_pPassword)
		{

			CString sval = m_pParent->GetBuffer(_T("PASSWORD1") ); 
	//		m_pPassword->SetWindowText(sval); 
			//주문확인창
			sval = m_pParent->GetBuffer(_T("QORDER1") ); 
			if(!sval.IsEmpty())
				m_pOrderConfirm->SetCheck( atoi(sval) );

			//매도?매수
			sval = m_pParent->GetBuffer(_T("JTYPERdo1"));
			int ival = atoi(sval);
			if(ival) m_pBuy->SetCheck(1); 
			else m_pSell->SetCheck(1);
	
			//주문유형 코보
			sval = m_pParent->GetBuffer(_T("JTypeCbo1"));
			if(!sval.IsEmpty())
			{
				ival = atoi(sval);
				m_pOrderType->SetCurSel(ival); 
			}
	
			//조건
			sval = m_pParent->GetBuffer(_T("JCaseCbo1"));
			if(!sval.IsEmpty())
			{
				ival = atoi(sval);
				m_pCondition->SetCurSel(ival); 
			}

			//수량: 주?금액
			sval = m_pParent->GetBuffer(_T("JVolRdo1"));
			ival = atoi(sval);
			if(ival) m_pQuantityPrice->SetCheck(1); 
			else m_pQuantityShare->SetCheck(1);


			//금액 에디트
			sval = m_pParent->GetBuffer(_T("JAmtEdt1"));
			if(!sval.IsEmpty() ) 
				m_pQuantityValue->SetWindowText(sval); 

			//단가
			sval = m_pParent->GetBuffer(_T("JPrcUnit1"));
			if(!sval.IsEmpty() ) 
			{
				ival = atoi(sval);
				m_pPriceUnit->SetCurSel(ival); 
			}

			m_pPassword->GetWindowText(sval); 
			if(!sval.IsEmpty())
			{
			}
		}
	}

	CWnd::OnTimer(nIDEvent);
}



void COrderWnd::OnDestroy() 
{
	CWnd::OnDestroy();
	
	// TODO: Add your message handler code here
	//매도?매수
	int ival = 0;
	CString sval;
	if( m_pSell->GetCheck()) ival = 0;
	else ival =1;
	sval.Format("%d", ival); 
	m_pParent->SaveBuffer(_T("JTYPERdo1"), sval);

	//주문확인창
	sval.Format("%d", m_pOrderConfirm->GetCheck() ); 
	m_pParent->SaveBuffer(_T("QORDER1"), sval ); 

	//주문유형 코보
	ival = m_pOrderType->GetCurSel(); 
	sval.Format("%d", ival); 
	m_pParent->SaveBuffer(_T("JTypeCbo1"), sval);
	
	//조건
	ival = m_pCondition->GetCurSel(); 
	sval.Format("%d", ival); 
	m_pParent->SaveBuffer(_T("JCaseCbo1"), sval);

	//수량: 주?금액
	if( m_pQuantityShare->GetCheck()) ival = 0;
	else ival = 1;
	sval.Format("%d", ival); 
	m_pParent->SaveBuffer(_T("JVolRdo1"), sval);
	
	//금액 에디트
	m_pQuantityValue->GetWindowText(sval); 
	m_pParent->SaveBuffer(_T("JAmtEdt1"), sval);

	//단가
	ival = m_pPriceUnit->GetCurSel(); 
	sval.Format("%d", ival); 
	m_pParent->SaveBuffer(_T("JPrcUnit1"), sval);

}
int COrderWnd::GetValueUpDownfloor(int ival, int idelta, bool bUP)
{
	//내림 
	int number = ival;
	int result_floor{};
	if (ival % idelta == 0)
		result_floor = ival;
	else if (bUP)
		result_floor = number + (idelta - (number % idelta));
	else
		result_floor = number - (number % idelta);

	return result_floor;
}

CString COrderWnd::GetOrderPriceByMarket(int iprice, CString strCode)
{
	CString slog;
	CString strPrice;
	const int code_type = GetWizard()->SendMessage(WM_USER, MAKEWPARAM(codeTYPE, 0), (LPARAM)(LPCSTR)strCode);
	//slog.Format("[1006][%d] strPercent =[%s] strPrice=[%s] code_type=[%d]", i, strPercent, strPrice, code_type);
	if (code_type == kospiType)
	{
		if (iprice < 1000)
			strPrice.Format("%d", iprice);
		else if (iprice >= 1000 && iprice < 5000)
			strPrice.Format("%d", GetValueUpDownfloor(iprice, 5));
		else if (iprice >= 5000 && iprice < 10000)
			strPrice.Format("%d", GetValueUpDownfloor(iprice, 10));
		else if (iprice >= 10000 && iprice < 50000)
			strPrice.Format("%d", GetValueUpDownfloor(iprice, 50));
		else if (iprice >= 50000 && iprice < 100000)
			strPrice.Format("%d", GetValueUpDownfloor(iprice, 100));
		else if (iprice >= 100000 && iprice < 500000)
			strPrice.Format("%d", GetValueUpDownfloor(iprice, 500));
		else
			strPrice.Format("%d", GetValueUpDownfloor(iprice, 1000));
	}
	else if (code_type == kosdaqType)
	{
		if (iprice < 1000)
			strPrice.Format("%d", iprice);
		else if (iprice >= 1000 && iprice < 5000)
			strPrice.Format("%d", GetValueUpDownfloor(iprice, 5));
		else if (iprice >= 5000 && iprice < 10000)
			strPrice.Format("%d", GetValueUpDownfloor(iprice, 10));
		else if (iprice >= 10000 && iprice < 50000)
			strPrice.Format("%d", GetValueUpDownfloor(iprice, 50));
		else
			strPrice.Format("%d", GetValueUpDownfloor(iprice, 100));
	}
	else if (code_type == thirdType)
	{
		if (iprice < 1000)
			strPrice.Format("%d", iprice);
		else if (iprice >= 1000 && iprice < 5000)
			strPrice.Format("%d", GetValueUpDownfloor(iprice, 5));
		else if (iprice >= 5000 && iprice < 10000)
			strPrice.Format("%d", GetValueUpDownfloor(iprice, 10));
		else if (iprice >= 10000 && iprice < 50000)
			strPrice.Format("%d", GetValueUpDownfloor(iprice, 50));
		else if (iprice >= 50000 && iprice < 100000)
			strPrice.Format("%d", GetValueUpDownfloor(iprice, 100));
		else if (iprice >= 100000 && iprice < 500000)
			strPrice.Format("%d", GetValueUpDownfloor(iprice, 500));
		else
			strPrice.Format("%d", GetValueUpDownfloor(iprice, 1000));
	}
	else if (code_type == etfType || code_type == etnType)
	{
		if (iprice >= 2000)
			strPrice.Format("%d", GetValueUpDownfloor(iprice, 5));
		else
			strPrice.Format("%d", iprice);
	}
	else if (code_type == elwType)
	{
		strPrice.Format("%d", GetValueUpDownfloor(iprice, 5));
	}
	else
	{
		if (iprice < 1000)
			strPrice.Format("%d", iprice);
		else if (iprice >= 1000 && iprice < 5000)
			strPrice.Format("%d", GetValueUpDownfloor(iprice, 5));
		else if (iprice >= 5000 && iprice < 10000)
			strPrice.Format("%d", GetValueUpDownfloor(iprice, 10));
		else if (iprice >= 10000 && iprice < 50000)
			strPrice.Format("%d", GetValueUpDownfloor(iprice, 50));
		else if (iprice >= 50000 && iprice < 100000)
			strPrice.Format("%d", GetValueUpDownfloor(iprice, 100));
		else if (iprice >= 100000 && iprice < 500000)
			strPrice.Format("%d", GetValueUpDownfloor(iprice, 500));
		else
			strPrice.Format("%d", GetValueUpDownfloor(iprice, 1000));
	}
	return strPrice;
}