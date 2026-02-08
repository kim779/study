// ModCanWnd.cpp : implementation file
//

#include "stdafx.h"
#include "IB100600.h"
#include "ModCanWnd.h"
#include "MapWnd.h"
#include "Define.h"
#include "ConfirmDlg.h"
#include "../../control/fx_misc/misctype.h"
#define UM_PASSWORD	WM_USER+1111

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CModCanWnd

CModCanWnd::CModCanWnd(CWnd* pWizard)
	: m_pWizard(pWizard)
	//, m_pParent(NULL)
	//, m_pAccount(NULL)
	//, m_pAccName(NULL)
	//, m_pPassStatic(NULL)
	//, m_pPassword(NULL)
	//, m_pOrderConfirm(NULL)
	//, m_pClear(NULL)
	//, m_pNonContract(NULL)
	//, m_pModify(NULL)
	//, m_pCancel(NULL)
	//, m_pContrect(NULL)
	//, m_pAll(NULL)
	//, m_pBuy(NULL)
	//, m_pSell(NULL)
	//, m_pCodeCtrl(NULL)
	//, m_pCodeSearch(NULL)
	//, m_pSymbol(NULL)
	//, m_pQuantityStatic(NULL)
	//, m_pQuantityRemain(NULL)
	//, m_pQuantityPart(NULL)
	//, m_pPriceUnitStatic(NULL)
	//, m_pPriceUnit(NULL)
	//, m_pPercent(NULL)
	//, m_pPercentStatic(NULL)
	//, m_bBlock(FALSE)
{
	m_bFirst = FALSE;
	m_bOrderType = FALSE;
	m_pCodeCtrlStatic = nullptr;
	m_pModCanGrid = nullptr;
	m_pExpand = nullptr;
}

CModCanWnd::~CModCanWnd()
{
//	_SAFEDESTROY(m_pAccount);
	//_SAFEDESTROY(m_pAccName);
	//_SAFEDESTROY(m_pPassStatic);
	//_SAFEDESTROY(m_pPassword);
	//_SAFEDESTROY(m_pOrderConfirm);
	//_SAFEDESTROY(m_pClear);
	//_SAFEDESTROY(m_pNonContract);
	//_SAFEDESTROY(m_pModify);
	//_SAFEDESTROY(m_pCancel);
	//_SAFEDESTROY(m_pContrect);
	//_SAFEDESTROY(m_pAll);
	//_SAFEDESTROY(m_pBuy);
	//_SAFEDESTROY(m_pSell);
	//_SAFEDESTROY(m_pCodeCtrl);
	//_SAFEDESTROY(m_pCodeSearch);

	if (m_pSymbol)
	{
		if (IsWindow(m_pSymbol->GetSafeHwnd()))
			m_pSymbol->SendMessage(WM_CLOSE);
		m_pSymbol->Detach();
		delete m_pSymbol;
		m_pSymbol = NULL;
	}

	if (m_hiSymbol)
	{
		AfxFreeLibrary(m_hiSymbol);
		m_hiSymbol = NULL;
	}

	//_SAFEDESTROY(m_pQuantityStatic);
	//_SAFEDESTROY(m_pQuantityRemain);
	//_SAFEDESTROY(m_pQuantityPart);
}


BEGIN_MESSAGE_MAP(CModCanWnd, CWnd)
	//{{AFX_MSG_MAP(CModCanWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_CTLCOLOR()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_CBN_SELENDOK(IDC_CODECTRL, OnCodeSelEndOk)
	ON_CBN_SELCHANGE(IDC_PRICEUNIT, OnSelchangePriceUnit)
	ON_BN_CLICKED(IDC_NONCONTRACT, OnNonContract)
	ON_BN_CLICKED(IDC_BUY, OnBuy)
	ON_BN_CLICKED(IDC_SELL, OnSell)
	ON_BN_CLICKED(IDC_ALL, OnAll)
	ON_BN_CLICKED(IDC_MODIFY, OnModify)
	ON_BN_CLICKED(IDC_CANCEL, OnCancel)
	ON_BN_CLICKED(IDC_CODESEARCH, OnCodeSearch)
	ON_CBN_SELCHANGE(IDC_PRICEUNIT, OnSelchangePriceUnit)
	ON_EN_CHANGE(IDC_PERCENT, OnPercent)
	ON_MESSAGE(WM_USER + 1, OnUser)
	ON_MESSAGE(UM_PASSWORD, OnMyPassword)
	ON_BN_CLICKED(IDC_CONTRECT, OnContrect)
	ON_BN_CLICKED(IDC_CLEAR, OnClear)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CModCanWnd message handlers

void CModCanWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CMapWnd* pWnd = (CMapWnd*)GetParent();
	
	CRect rcClient,rcbox, rc;
	GetClientRect(&rcClient);

	dc.FillSolidRect(&rcClient, pWnd->GetIndexColor(CONTENTS_BACK_INDEX));

	m_pPriceUnitStatic->GetWindowRect(&rc);
	ScreenToClient(rc);
	rcbox = rcClient;
	rcbox.bottom = rc.bottom + INNER_FRAME_SPACE;
	dc.FillSolidRect(&rcbox, m_pParent->GetIndexColor(CLR_PANNEL_BACK));


//	dc.FillSolidRect(&rcClient, 0xff0000); 
}

void CModCanWnd::SetPallete()
{
	m_pAccName->SetBkColor(m_pParent->GetIndexColor(CLR_SYMBOL_BACK));
	m_pAccName->SetFgColor(m_pParent->GetIndexColor(CLR_SYMBOL_FORE));
	m_pPassStatic->SetBkColor(m_pParent->GetIndexColor(CLR_PANNEL_BACK));
	m_pPassStatic->SetFgColor(m_pParent->GetIndexColor(CLR_STATIC_FORE));
	m_pCodeCtrlStatic->SetBkColor(m_pParent->GetIndexColor(CLR_PANNEL_BACK));
	m_pCodeCtrlStatic->SetFgColor(m_pParent->GetIndexColor(CLR_STATIC_FORE));
	m_pQuantityStatic->SetBkColor(m_pParent->GetIndexColor(CLR_PANNEL_BACK));
	m_pQuantityStatic->SetFgColor(m_pParent->GetIndexColor(CLR_STATIC_FORE));
	m_pPriceUnitStatic->SetBkColor(m_pParent->GetIndexColor(CLR_PANNEL_BACK));
	m_pPriceUnitStatic->SetFgColor(m_pParent->GetIndexColor(CLR_STATIC_FORE));
	m_pPercentStatic->SetBkColor(m_pParent->GetIndexColor(CLR_PANNEL_BACK));
	m_pPercentStatic->SetFgColor(m_pParent->GetIndexColor(CLR_STATIC_FORE));

	m_pModCanGrid->SetPallete();
}

int CModCanWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
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

	m_pAccName = std::make_unique < CfxStatic>();
	m_pAccName->Create(_T(""), WS_CHILD | WS_VISIBLE | SS_LEFT | WS_CLIPCHILDREN | 
							WS_CLIPSIBLINGS, rcCtrl, this, IDC_ACCOUNTNAME);

	rcCtrl.left = rcCtrl.right + SZ_CTRL_HGAP + 3;	// 컨트롤 그룹간 간격은 5로 한다. (SZ_CTRL_HGAP : 5px,  fxStatic내부 마진 2px)
	rcCtrl.right = rcCtrl.left + SZ_CTRL_4STATIC;
	
	m_pPassStatic = std::make_unique <CfxStatic>();
	m_pPassStatic->Create(_T("비밀번호"), WS_CHILD | WS_VISIBLE | SS_LEFT | WS_CLIPCHILDREN | 
							WS_CLIPSIBLINGS, rcCtrl, this, IDC_PASSSTATIC);

	rcCtrl.left = rcCtrl.right + 3;					// 캡션과 컨트롤간 간격은 5로 한다. (fxStatic 내부마진 2px)
	rcCtrl.right = rcCtrl.left + SZ_CTRL_4EDIT + 5;
	
	m_pPassword = std::make_unique <CfxEdit>();
	m_pPassword->Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_PASSWORD, rcCtrl, this, IDC_PASSWORD);

	rcCtrl.left = rcCtrl.right + SZ_CTRL_HGAP + 3;	// 컨트롤 그룹간 간격은 5로 한다. (SZ_CTRL_HGAP : 5px,  fxStatic내부 마진 2px)
	rcCtrl.right = rcCtrl.left + SZ_CHECK_5WIDTH;

	m_pOrderConfirm = std::make_unique <CButton>();
	m_pOrderConfirm->Create(_T("주문확인창"), WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | BS_AUTOCHECKBOX |
							BS_TOP | BS_BOTTOM, rcCtrl, this, IDC_ORDERCONFIRM);

	rcCtrl.left = rcClient.right - (SZ_CTRL_4BUTTON * 5) - 4 - SZ_TAB_HMARGIN;
	rcCtrl.right = rcCtrl.left + SZ_CTRL_4BUTTON;

	m_pClear = std::make_unique <CfxImgButton>();
	m_pClear->Create("\nClear", rcCtrl, this, IDC_CLEAR, FALSE, TRUE);
	
	rcCtrl.left = rcCtrl.right + 1;
	rcCtrl.right = rcCtrl.left + SZ_CTRL_4BUTTON;

	m_pNonContract = std::make_unique <CfxImgButton>();
	m_pNonContract->Create("\n미체결", rcCtrl, this, IDC_NONCONTRACT, FALSE, TRUE);

	rcCtrl.left = rcCtrl.right + 1;
	rcCtrl.right = rcCtrl.left + SZ_CTRL_4BUTTON;

	m_pModify = std::make_unique <CfxImgButton>();
	m_pModify->Create("\n일괄정정", rcCtrl, this, IDC_MODIFY, FALSE, TRUE);

	rcCtrl.left = rcCtrl.right + 1;
	rcCtrl.right = rcCtrl.left + SZ_CTRL_4BUTTON;

	m_pCancel = std::make_unique <CfxImgButton>();
	m_pCancel->Create("\n일괄취소", rcCtrl, this, IDC_CANCEL, FALSE, TRUE);
	
	rcCtrl.left = rcCtrl.right + 1;
	rcCtrl.right = rcCtrl.left + SZ_CTRL_4BUTTON;

	m_pContrect = std::make_unique <CfxImgButton>();
	m_pContrect->Create("\n체결", rcCtrl, this, IDC_CONTRECT, FALSE, TRUE);

	rcCtrl.left = SZ_TAB_HMARGIN;
	rcCtrl.right = SZ_RADIO_2WIDTH;
	rcCtrl.top = rcCtrl.bottom + SZ_CTRL_VGAP + 1;
	rcCtrl.bottom = rcCtrl.top + SZ_CTRL_HEIGHT;

	m_pAll = std::make_unique <CButton>();
	m_pAll->Create(_T("전체"), WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | BS_AUTORADIOBUTTON | 
						WS_GROUP | BS_TOP | BS_BOTTOM, rcCtrl, this, IDC_ALL);

	rcCtrl.left = rcCtrl.right + SZ_CTRL_HGAP;
	rcCtrl.right = rcCtrl.left + SZ_RADIO_2WIDTH;

	m_pSell = std::make_unique <CButton>();
	m_pSell->Create(_T("매도"), WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | BS_AUTORADIOBUTTON | 
						BS_TOP | BS_BOTTOM, rcCtrl, this, IDC_SELL);

	rcCtrl.left = rcCtrl.right + SZ_CTRL_HGAP;
	rcCtrl.right = rcCtrl.left + SZ_RADIO_2WIDTH;

	m_pBuy = std::make_unique <CButton>();
	m_pBuy->Create(_T("매수"), WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | BS_AUTORADIOBUTTON | 
						BS_TOP | BS_BOTTOM, rcCtrl, this, IDC_BUY);

	rcCtrl.top -= 1;
	rcCtrl.bottom -= 1;
	rcCtrl.left = rcCtrl.right + SZ_CTRL_HGAP + 3;	// 컨트롤 그룹간 간격은 5로 한다. (SZ_CTRL_HGAP : 5px,  fxStatic내부 마진 2px)
	rcCtrl.right = rcCtrl.left + SZ_CTRL_2STATIC;
	
	m_pCodeCtrlStatic = std::make_unique <CfxStatic>();
	m_pCodeCtrlStatic->Create(_T("종목"), WS_CHILD | WS_VISIBLE | SS_LEFT | WS_CLIPCHILDREN | 
							WS_CLIPSIBLINGS, rcCtrl, this, IDC_CODECTRLSTATIC);

	rcCtrl.left = rcCtrl.right + 3;
	rcCtrl.right = rcCtrl.left + SZ_COMBO_3WIDTH;
	m_pCodeCtrl = std::make_unique <CfxCodeCtrl>(this, CString(m_pParent->GetAxRoot() + "\\tab"));
	m_pCodeCtrl->Create(this, rcCtrl, IDC_CODECTRL);

	rcCtrl.left = rcCtrl.right + 2;
	rcCtrl.right = rcCtrl.left + SZ_CTRL_1BUTTON;
	m_pCodeSearch = std::make_unique <CfxImgButton>();
	m_pCodeSearch->Create("\n호가", rcCtrl, this, IDC_CODESEARCH, FALSE, TRUE);
	
	rcCtrl.left = rcCtrl.right + SZ_CTRL_HGAP;
	rcCtrl.right = rcCtrl.left + SZ_SYMBOL;

	InitSymbol(rcCtrl);

	rcCtrl.left = rcCtrl.right + SZ_CTRL_HGAP + 3;
	rcCtrl.right = rcCtrl.left + SZ_CTRL_2STATIC;

	m_pQuantityStatic = std::make_unique <CfxStatic>();
	m_pQuantityStatic->Create(_T("수량"), WS_CHILD | WS_VISIBLE | SS_LEFT | WS_CLIPCHILDREN | 
								WS_CLIPSIBLINGS, rcCtrl, this, IDC_QUANTITYSTATIC);

	rcCtrl.top += 1;
	rcCtrl.bottom += 1;
	rcCtrl.left = rcCtrl.right + 3;
	rcCtrl.right = rcCtrl.left + SZ_RADIO_2WIDTH;

	m_pQuantityRemain = std::make_unique <CButton>();
	m_pQuantityRemain->Create(_T("잔량"), WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | BS_AUTORADIOBUTTON | 
						WS_GROUP | BS_TOP | BS_BOTTOM, rcCtrl, this, IDC_QUANTITYREMAIN);
	
	rcCtrl.left = rcCtrl.right + 3;
	rcCtrl.right = rcCtrl.left + SZ_RADIO_2WIDTH;

	m_pQuantityPart = std::make_unique <CButton>();
	m_pQuantityPart->Create(_T("일부"), WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | BS_AUTORADIOBUTTON | 
						BS_TOP | BS_BOTTOM, rcCtrl, this, IDC_QUANTITYPART);

	rcCtrl.top -= 1;
	rcCtrl.bottom -= 1;
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

	rcCtrl.left = rcCtrl.right + 1;
	rcCtrl.right = rcCtrl.left + SZ_CTRL_3EDIT;

	m_pPercent = std::make_unique <CfxEdit>();
	m_pPercent->Create(WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | ES_RIGHT, rcCtrl, this, IDC_PERCENT);

	rcCtrl.left = rcCtrl.right;
	rcCtrl.right = rcCtrl.left + SZ_CTRL_1STATIC;

	m_pPercentStatic = std::make_unique <CfxStatic>();
	m_pPercentStatic->Create(_T("%"), WS_CHILD | WS_VISIBLE | SS_LEFT | WS_CLIPCHILDREN | 
								WS_CLIPSIBLINGS, rcCtrl, this, IDC_PERCENTSTATIC);

	rcCtrl.left = 0;
	rcCtrl.right = rcClient.right - SZ_TAB_HMARGIN;
	rcCtrl.top = rcCtrl.bottom + SZ_CTRL_VGAP + 1;
	rcCtrl.bottom = rcClient.bottom - 1;

	m_pModCanGrid = std::make_unique <CModCanGrid>();
	m_pModCanGrid->CreateGrid(WS_CHILD | WS_VISIBLE, rcCtrl, this, IDC_MODCANGRID);

	
	return 0;
}

void CModCanWnd::Init()
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

	m_pNonContract->SetImgBitmap(hbmp, hbmp_dn, hbmp_hv);
	m_pNonContract->SetWindowText(_T("미체결"));
	m_pNonContract->SetFont(pFont);

	m_pModify->SetImgBitmap(hbmp, hbmp_dn, hbmp_hv);
	m_pModify->SetWindowText(_T("일괄정정"));
	m_pModify->SetFont(pFont);

	m_pCancel->SetImgBitmap(hbmp, hbmp_dn, hbmp_hv);
	m_pCancel->SetWindowText(_T("일괄취소"));
	m_pCancel->SetFont(pFont);

	m_pContrect->SetImgBitmap(hbmp, hbmp_dn, hbmp_hv);
	m_pContrect->SetWindowText(_T("체결"));
	m_pContrect->SetFont(pFont);

	m_pAll->SetFont(pFont);
	m_pAll->SetCheck(1);

	m_pSell->SetFont(pFont);
	m_pBuy->SetFont(pFont);

	m_pCodeCtrlStatic->SetBkColor(m_pParent->GetIndexColor(CLR_PANNEL_BACK));
	m_pCodeCtrlStatic->SetFgColor(m_pParent->GetIndexColor(CLR_STATIC_FORE));
	m_pCodeCtrlStatic->SetFont(pFont);

	m_pCodeCtrl->SetUnit(GU_CODE);
	m_pCodeCtrl->SetFont(pFont);
	m_pCodeCtrl->SetHistory(m_pParent->Variant(historyCC, _T("1301")));

	hbmp	= m_pParent->GetAxBitmap(strPath + "s검색.bmp")->operator HBITMAP();
	hbmp_dn	= m_pParent->GetAxBitmap(strPath + "s검색_dn.bmp")->operator HBITMAP();
	hbmp_hv = m_pParent->GetAxBitmap(strPath + "s검색_en.bmp")->operator HBITMAP();

	m_pCodeSearch->SetImgBitmap(hbmp, hbmp_dn, hbmp_hv);
	m_pCodeSearch->SetFont(pFont);

	m_pQuantityStatic->SetBkColor(m_pParent->GetIndexColor(CLR_PANNEL_BACK));
	m_pQuantityStatic->SetFgColor(m_pParent->GetIndexColor(CLR_STATIC_FORE));
	m_pQuantityStatic->SetFont(pFont);

	m_pQuantityRemain->SetFont(pFont);
	m_pQuantityRemain->SetCheck(1);

	m_pQuantityPart->SetFont(pFont);

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

//	SetTimer(100, 10, NULL);

	CString sval = m_pParent->GetBuffer(_T("PASSWORD1") ); 
	//주문확인창
	sval = m_pParent->GetBuffer(_T("QORDER2") ); 
	if(!sval.IsEmpty())
		m_pOrderConfirm->SetCheck(atoi(sval)); 

	//전체?매도?매수
	sval = m_pParent->GetBuffer(_T("JTYPERdo2"));
	int ival = 0;
	if(!sval.IsEmpty())
	{
		ival = atoi(sval);
		if(ival > 0)
		{
			m_pAll->SetCheck(0); 
			if(ival == 1) m_pSell->SetCheck(1); 
			else m_pBuy->SetCheck(1);
		}
	}


	//수량: 잔량?일부
	sval = m_pParent->GetBuffer(_T("JVolRdo2"));
	if(!sval.IsEmpty())
	{
		ival = atoi(sval);
		if(ival)
		{
			m_pQuantityRemain->SetCheck(0); 
			m_pQuantityPart->SetCheck(1); 
		}
	}

	//단가
	sval = m_pParent->GetBuffer(_T("JPRCUNT2"));
	if(!sval.IsEmpty())
	{
		ival = atoi(sval);
		m_pPriceUnit->SetCurSel(ival); 
	}
}

HBRUSH CModCanWnd::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CWnd::OnCtlColor(pDC, pWnd, nCtlColor);
	
    const UINT nID = pWnd->GetDlgCtrlID();

    if (nID == IDC_ORDERCONFIRM || nID == IDC_ALL || nID == IDC_SELL || 
		nID == IDC_BUY || nID == IDC_REMAINQUANTITY || nID == IDC_PARTQUANTITY ||
		nID == IDC_QUANTITYREMAIN || nID == IDC_QUANTITYPART)
    {    
		pDC->SetBkMode(TRANSPARENT);                              
		return (HBRUSH)::CreateSolidBrush(m_pParent->GetIndexColor(CLR_PANNEL_BACK)); //::GetStockObject(NULL_BRUSH);
    }

	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CModCanWnd::InitSymbol(CRect rcCtrl)
{
	CString	text;
	CWnd*	(APIENTRY* axCreate)(CWnd*, void*) = nullptr;

	text.Format("%s/dev/cx_symbol.dll", m_pParent->GetAxRoot());
	m_hiSymbol = AfxLoadLibrary(text);

	rcCtrl.OffsetRect(0, SZ_CTRL_HEIGHT + SZ_FORM_MARGIN);
	if (m_hiSymbol != NULL)
	{
		axCreate = (CWnd* (APIENTRY*)(CWnd*, void*))GetProcAddress(m_hiSymbol, _T("axCreate"));
		if (axCreate != NULL)
		{
			struct	_param	param;

			m_pWizard->ClientToScreen(&rcCtrl);
			ScreenToClient(&rcCtrl);

			param.key   = 0;
			param.name  = _T("17413");
			param.rect  = rcCtrl;
			param.fonts = "굴림체";
			param.point = 9;
			param.style = 1;
			param.tRGB  = 69;
			param.pRGB  = 90;
			param.options = _T("/a89/b91/c92/d69/i99/s1006");

			m_pSymbol = (*axCreate)(m_pWizard, &param);
			m_pSymbol->SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE);

			m_pSymbol->SetParent(this);

			if (m_pSymbol == NULL)
			{
				AfxFreeLibrary(m_hiSymbol);
				m_hiSymbol = NULL;
			}
			else
			{
				CRect wrc;
				m_pCodeSearch->GetWindowRect(&wrc);
				ScreenToClient(&wrc);

				const int itop = wrc.top;
				const int ibottom = wrc.Height();
				m_pSymbol->GetWindowRect(&wrc);
				ScreenToClient(&wrc);
				const int ileft = wrc.left;
				const int iright = wrc.Width(); 
				m_pSymbol->MoveWindow(ileft, itop, iright, ibottom );

			}
		}
	}
}

void CModCanWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CWnd::OnShowWindow(bShow, nStatus);
		
	if (m_pCodeCtrl || m_pSymbol || m_pPassword)
	{
		if (m_pSymbol) {
			m_pSymbol->ShowWindow(bShow);

			if (bShow)
			{
				CString strCode = m_pCodeCtrl->GetEditData();
				QuerySymbol(strCode);
				SetInputFieldFocus();
			}

			m_pSymbol->SendMessage(WM_MOUSEHOVER, 0L, MAKELPARAM(0, 0));
		}
	}
}

void CModCanWnd::SetAccount(CString strAccount)
{
	if (m_pAccount)
		m_pAccount->TriggerAccount(strAccount);
}

void CModCanWnd::SetCode(CString strCode)
{
	if (GetCodeName(strCode).IsEmpty())
		return;

	if (m_pCodeCtrl)
		m_pCodeCtrl->SetEditData(strCode);

	QuerySymbol(strCode);
}

CString CModCanWnd::GetCodeName(CString strCode)
{
	CString strInput = CString(strCode.operator LPCTSTR());	// buffer가 깨짐을 방지
	CString strName;
	strName.Empty();
	if (strCode.GetLength() > 0)
	{
		strName = (char*) m_pWizard->SendMessage(WM_USER, MAKEWPARAM(nameDLL, 0), (LPARAM)strInput.operator LPCTSTR());
		strName.TrimLeft();
		strName.TrimRight();
	}

	return strName;
}

void CModCanWnd::ParsingSymbol(WPARAM wParam, LPARAM lParam)
{
	if (m_pSymbol)
		m_pSymbol->SendMessage(WM_USER, wParam, lParam);
}

void CModCanWnd::QuerySymbol(CString strCode)
{
	CString strText;

	strText.Format("1301%c%s\t17413\t", 0x7f, strCode.Left(6));
	m_pParent->SendTR(_T("POOPPOOP"), strText, KEY_SYMBOL, 0);
}

void CModCanWnd::OnCodeSelEndOk()
{
	CString strCode = m_pCodeCtrl->GetEditData();
	QuerySymbol(strCode);

	if (IsMandatoryField() == 0)
		QueryNonContract(strCode, GetOrderType());
}

void CModCanWnd::OnSelchangePriceUnit()
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

void CModCanWnd::OnNonContract()
{
	CString strCode = m_pCodeCtrl->GetEditData();
	
	if (strCode.IsEmpty())
		QuerySymbol(_T(""));

	QueryNonContract(m_pCodeCtrl->GetEditData(), GetOrderType());
}

void CModCanWnd::ParsingNonCont(CString strData, int nLen, CString strMsg)
{
	ClearModCanGrid();

	if (strData.GetLength() < 15 + L_s_crec)
	{
		strMsg.Delete(0, 4);
		m_pParent->SetGuide(strMsg);
		return;
	}

	CUGCell cell;
	CString strVal(_T(""));
	CString strAccnount = strData.Left(11);
	int nRec = atoi((LPCTSTR)strData.Mid(11, 4));

	strData.Delete(0, 15);
	struct s_crec rec {};

	ClearModCanGrid();

	if (nRec >= m_pModCanGrid->GetNumberRows())
		m_pModCanGrid->SetNumberRows(nRec + 1);

	for (int i = 0; i < nRec; i++)
	{
		CopyMemory(&rec, (LPCTSTR)strData.Left(L_s_crec), L_s_crec);

		strVal = CString(rec.cod2, sizeof(rec.cod2));
		_TRIMDATA(strVal);
		m_pModCanGrid->QuickSetText(1, i, strVal);

		QuerySCHoga(FALSE, strVal);

		strVal = CString(rec.juno, sizeof(rec.juno));
		_TRIMDATA(strVal);
		m_pModCanGrid->QuickSetText(2, i, strVal);

		strVal = CString(rec.hnam, sizeof(rec.hnam));
		_TRIMDATA(strVal);
		m_pModCanGrid->QuickSetText(3, i, strVal);

		strVal = CString(rec.odgb, sizeof(rec.odgb));
		_TRIMDATA(strVal);
		m_pModCanGrid->QuickSetText(4, i, strVal.Right(4));

		strVal = CString(rec.jqty, sizeof(rec.jqty));
		_TRIMDATA(strVal);
		m_pModCanGrid->QuickSetText(5, i, strVal);

		strVal = CString(rec.jprc, sizeof(rec.jprc));
		_TRIMDATA(strVal);
		m_pModCanGrid->QuickSetText(6, i, strVal);

		strVal = CString(rec.wqty, sizeof(rec.wqty));
		_TRIMDATA(strVal);
		m_pModCanGrid->QuickSetText(7, i, strVal);

		strData.Delete(0, L_s_crec);
		m_pModCanGrid->GotoRow(i);
	}

	m_pModCanGrid->SetPallete();	//2014.01.21 KSJ 열구분을 위해서 

	m_pModCanGrid->RedrawAll();
	m_bBlock = FALSE;

	CString strCode = m_pModCanGrid->QuickGetText(1, m_pModCanGrid->GetCurrentRow());

	if (!strCode.IsEmpty())
	{
		CString strProc;
		strProc.Format("change_code\t%s", strCode);
		m_pParent->Procedure(strProc);
	}
}

void CModCanWnd::ClearModCanGrid()
{
	CUGCell cell;
	m_pModCanGrid->SetNumberRows(DEF_MODCANGRIDROW);

	for (int i = 0; i < MAX_MODCANGRIDCOL; i++)
	{
		for (int j = 0; j < DEF_MODCANGRIDROW; j++)
		{
			m_pModCanGrid->GetCell(i, j, &cell);
			cell.ClearProperty(UGCELL_TEXT_SET);
			m_pModCanGrid->SetCell(i, j, &cell);
		}
	}

	m_pModCanGrid->GotoRow(0);
	m_pModCanGrid->RedrawAll();
}

void CModCanWnd::QuerySCHoga(BOOL bHoga, CString strCode, BOOL bStdPrc)
{
	CStringArray arrCode;
	int nCount = 0;

	if (bHoga)
	{
		m_pModCanGrid->GetCheckCode(&arrCode);
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
	if(bStdPrc)
		mid.gubn[0] = m_pPriceUnit->GetCount() -1 + 0x30;
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
		m_pParent->SendTR("PIBO1006", strSendData, bHoga ? KEY_MODCANSHOGA_STDPRC : KEY_MODCANSCURR_STDPRC, 0);
	else
		m_pParent->SendTR("PIBO1006", strSendData, bHoga ? KEY_MODCANSHOGA : KEY_MODCANSCURR, 0);
}

void CModCanWnd::ParsingSCHoga(CString strData, BOOL bHoga, BOOL bStdPrc)	//2015.06.12 KSJ 기준가 구하기 추가
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

			strPrice.Format("%d", (int)fPrice);
		}

		strDan = CString(price.gdan, sizeof(price.gdan));
		_TRIMDATA(strDan);
		
		strVal.Format("%s\t%s\t%s", strCode, strPrice, strDan);
		arrPrice.Add(strVal);
	}

	m_pModCanGrid->SetSCData(&arrPrice, bHoga, bStdPrc);
}

void CModCanWnd::OnAll()
{
	if (IsMandatoryField() == 0)
		QueryNonContract(m_pCodeCtrl->GetEditData(), GetOrderType());
}

void CModCanWnd::OnBuy()
{
	if (IsMandatoryField() == 0)
		QueryNonContract(m_pCodeCtrl->GetEditData(), GetOrderType());
}

void CModCanWnd::OnSell()
{
	if (IsMandatoryField() == 0)
		QueryNonContract(m_pCodeCtrl->GetEditData(), GetOrderType());
}

void CModCanWnd::QueryNonContract(CString strCode, int nType)
{
	CString strAccount = m_pAccount->GetAccNo();
	CString strPassword(_T(""));
	CString strUser(_T(""));
	CString strData(_T(""));

	switch (IsMandatoryField())
	{
	case 1:
		AfxMessageBox("계좌번호를 입력하세요");
		return;

	case 2:
		AfxMessageBox("비밀번호를 입력하세요");
		return;
	}	

	m_pPassword->GetWindowText(strPassword);
	if (strPassword.GetLength()<4)
	{
		MessageBox("비밀번호를 확인하십시오.","IBK투자증권");
		return;
	}

	m_pParent->SaveBuffer(_T("PASSWORD2"), strPassword);
	strUser = m_pParent->Variant(userCC, "");

	struct _ledgerH ledger;

	FillMemory(&ledger, L_ledgerH, ' ');
	m_pWizard->SendMessage(WM_USER, ledgerDLL, (LPARAM)&ledger);

	CopyMemory(&ledger.usid, (LPCTSTR)strUser, strUser.GetLength());
	CopyMemory(&ledger.brno, strAccount.Left(3), 3);
	CopyMemory(&ledger.rcnt, _T("0000"), sizeof(ledger.rcnt));

	ledger.fkey[0] = 'C';
	ledger.mkty[0] = '1';
	ledger.odrf[0] = '1';

	strData = CString((char*)&ledger, L_ledgerH);

	struct s_cmid cmid;
	FillMemory(&cmid, L_s_cmid, ' ');
	cmid.gubn[0] = _T('>');	//2015.05.28 KSJ 구분값 추가 
	CopyMemory(&cmid.accn, strAccount, sizeof(cmid.accn));

	CString passX;
	passX.Format("%s\t%s", strPassword, strAccount.Left(8));
	passX = (char *)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(encPASSx, modeACN), (LPARAM)(LPCTSTR)passX);
	CopyMemory(&cmid.pswd, passX, passX.GetLength());

	cmid.dsgb[0] = nType + 0x30;
	cmid.otim[0] = _T('0');
	cmid.dlgb[0] = _T('2');
	cmid.sort[0] = _T('1');

	CopyMemory(cmid.cod2, (LPCTSTR)strCode, min(sizeof(cmid.cod2), strCode.GetLength()));
	CopyMemory(cmid.tcod, _T("00"), sizeof(cmid.tcod));

	strData += CString((char*)&cmid, L_s_cmid);

	m_bBlock = TRUE;

	m_pParent->SendTR("PIHOSCHG", strData, KEY_NONCONT, 0);
}

int CModCanWnd::GetOrderType()
{
	int nType=0;

	if (m_pAll->GetCheck() == 1)
		nType = 0;
	else if (m_pSell->GetCheck() == 1)
		nType = 1;
	if (m_pBuy->GetCheck() == 1)
		nType = 2;

	return nType;
}

int CModCanWnd::IsMandatoryField()
{
	CString strAccount = m_pAccount->GetAccNo();
	CString strPassword(_T(""));

	if (strAccount.IsEmpty())
		return 1;

	m_pPassword->GetWindowText(strPassword);
	
	if (strPassword.IsEmpty())
		return 2;

	return 0;
}


void CModCanWnd::ChkPassword()
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

	CStringArray arrData;
	m_pModCanGrid->GetSendData(&arrData);
	if (arrData.GetSize() <= 0) return;

	//본사직원 주문 안나가도록 예외 처리 [ End ]
	CString strPassword(_T(""));
	CString strAccount = m_pAccount->GetAccNo();

	m_pPassword->GetWindowText(strPassword);
	
	switch (IsMandatoryField())
	{
	case 1:
		AfxMessageBox("계좌번호를 입력하세요");
		return;

	case 2:
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
void CModCanWnd::OnModify()
{
	m_bOrderType = TRUE;
	ChkPassword();
//	SendOrder(TRUE);
}

void CModCanWnd::OnCancel()
{
	m_bOrderType = FALSE;
	ChkPassword();
//	SendOrder(FALSE);
}

void CModCanWnd::OnClear()
{
	ClearModCanGrid();
}

void CModCanWnd::SendOrder(BOOL bModify)
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
	
	int nCount = 0;
	CStringArray arrData;
	CStringArray arrConfirm;
	CString strData(_T(""));
	CString strCode(_T(""));		// 종목코드
	CString strOrderNo(_T(""));		// 원주문번호
	CString strOrderState(_T(""));	// 매매유형
	CString strOrderType(_T(""));	// 호가구분
	CString strQuantity(_T(""));	// 수량
	CString strOrderPrice(_T(""));	// 주문단가
	CString strCount(_T(""));;
	CString strUser(_T(""));
	CString strPassword(_T(""));
	CString strEncPass(_T(""));
	CString strSendData(_T(""));
	CString strAccount = m_pAccount->GetAccNo();

	m_pModCanGrid->GetSendData(&arrData);
	nCount = arrData.GetSize();

	if (nCount <= 0)
		return;

	m_pPassword->GetWindowText(strPassword);
	
	switch (IsMandatoryField())
	{
	case 1:
		AfxMessageBox("계좌번호를 입력하세요");
		return;

	case 2:
		AfxMessageBox("비밀번호를 입력하세요");
		return;
	}

	if (strPassword.GetLength()<4)
	{
		MessageBox("비밀번호를 확인하십시오.","IBK투자증권");
		return;
	}

	strUser = m_pParent->Variant(userCC, "");
// 	strEncPass.Format("%s\t%s", strPassword, strAccount.Left(8));
// 	strPassword = (char *)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(encPASSx, modeACN), (LPARAM)(LPCTSTR)strEncPass);
	
	if (m_pOrderConfirm->GetCheck() == 1)
	{
		CConfirmDlg dlg(this, m_pParent, bModify ? 3 : 4);
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
	struct i_com mid;
	FillMemory(&mid, L_i_com, ' ');

	strCount.Format("%04d", nCount);

	CopyMemory(mid.nrec, (LPCTSTR)strCount, strCount.GetLength());

	for (int i = 0; i < nCount; i++)
	{
		struct irec rec {};
		strData = arrData.GetAt(i);

		if (strData.IsEmpty())
			continue;
		
		FillMemory(&rec, L_irec, ' ');

		strCode = Parser(strData, _T("\t"));
		strOrderNo = Parser(strData, _T("\t"));
		Parser(strData, _T("\t"));				// 종목명
		strOrderState = Parser(strData, _T("\t"));
		Parser(strData, _T("\t"));
		Parser(strData, _T("\t"));
		strQuantity = Parser(strData, _T("\t"));
		strOrderPrice = Parser(strData, _T("\t"));

		strQuantity.Remove(_T(','));
		strOrderPrice.Remove(_T(','));

		if (bModify || m_pQuantityRemain->GetCheck() == 1)
			strQuantity = _T("0");

		rec.odgb[0] = _T('1');
		rec.mmgb[0] = bModify ? _T('3') : _T('4');
		CopyMemory(rec.accn, strAccount, strAccount.GetLength());
// 		CopyMemory(rec.pswd, strPassword, strPassword.GetLength());
		CopyMemory(rec.pswd, "HEAD", 4);	//2013.12.23 KSJ 일방향암호화
		CopyMemory(rec.cod2, strCode, strCode.GetLength());
		CopyMemory(rec.ojno, strOrderNo, strOrderNo.GetLength());
		CopyMemory(rec.jqty, strQuantity, strQuantity.GetLength());
		CopyMemory(rec.jprc, strOrderPrice, strOrderPrice.GetLength());
		CopyMemory(rec.hogb, _T("00"), sizeof(rec.hogb));
		
		if (strOrderType == _T("IOC"))
			rec.cdgb[0] = _T('1');
		else if (strOrderType == _T("FOK"))
			rec.cdgb[0] = _T('2');
		else
			rec.cdgb[0] = _T('0');

		if (bModify || m_pQuantityPart->GetCheck() == 1)
			rec.mdgb[0] = _T('1');
		else
			rec.mdgb[0] = _T('2');

		rec.prgb[0] = _T('X');

		CopyMemory(&mid.rec[i], &rec, L_irec);
	}

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

	strSendData += CString((char*)&mid, nCount * L_irec + 4);
	m_pParent->SendTR("PIBOSODR", strSendData, KEY_MODCANSORDER, 0);
}

CString CModCanWnd::Parser(CString &srcstr, CString substr)
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

void CModCanWnd::ParsingSOrder(CString strData, int nLen, CString strMsg)
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

	m_pModCanGrid->SetOrderResult(&arrData);
}

void CModCanWnd::OnPercent()
{
	QuerySCHoga(TRUE);
}

void CModCanWnd::SetInputFieldFocus()
{
	if (m_pAccount && m_pPassword && m_pNonContract)
	{
		if (m_pAccount->GetAccNo().IsEmpty())
			m_pAccount->SetFocus();
		else if (m_pPassword->GetText().IsEmpty())
			m_pPassword->SetFocus();
		else
			m_pNonContract->SetFocus();
	}
}

long CModCanWnd::OnUser(WPARAM wParam, LPARAM lParam)
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
				CString sval = m_pParent->GetBuffer(_T("PASSWORD2") ); 
				if(sval.IsEmpty())
				{
//					if( m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, orderCC)) )
//						sval = "9999";
				}

				m_pPassword->SetWindowText(sval); 
				if(!sval.IsEmpty())
					OnNonContract();
				m_bFirst = TRUE;
			}

		}
		break;

	case KEY_CHKPASSWORD: 
		SendOrder(m_bOrderType);
		break;

	//2016.02.16 KSJ 종목명컨트롤에 실시간 전달
	case DLL_ALERTx:
		m_pSymbol->SendMessage(WM_USER, wParam, lParam);
		break;
	}
	return 0;
}

void CModCanWnd::OnCodeSearch()
{
	CRect rcCtrl;
	m_pCodeSearch->GetClientRect(rcCtrl);
	m_pCodeSearch->ClientToScreen(&rcCtrl);
	CString strCodeData = (char*) m_pWizard->SendMessage(WM_USER, MAKEWPARAM(codeDLL, MAKEWORD(allCODE, NULL)), MAKELPARAM(rcCtrl.left, rcCtrl.bottom));
	
	if (!strCodeData.IsEmpty() && strCodeData.GetLength() > 6)
	{
		CString strCode = strCodeData.Left(6);
		if (m_pCodeCtrl)
			m_pCodeCtrl->SetEditData(strCode);

		QuerySymbol(strCode);
	}
}

void CModCanWnd::OnContrect()
{
	std::shared_ptr<CMenu> pMenu{};
	CRect  rcMenu;
	//int nCount = 0;

	//pMenu = new CMenu;
	pMenu = std::make_shared<CMenu>();
	pMenu->CreatePopupMenu();
	pMenu->AppendMenu(MF_STRING, 0, "체결조회");
	pMenu->AppendMenu(MF_STRING, 1, "미체결조회");

	m_pContrect->GetWindowRect(&rcMenu);

	const int nSelectGroup = pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON | TPM_RETURNCMD,
								rcMenu.left, rcMenu.bottom, this);
	
	//delete pMenu;
	pMenu.reset();

	CString strData(_T(""));

	if (nSelectGroup == 1)
		strData = _T("IB110100 /S /t");
	else if (nSelectGroup == 0)
		strData = _T("IB110200 /S /t");

	m_pWizard->SendMessage(WM_USER, MAKEWPARAM(viewDLL, typeVIEW), (LPARAM)(LPCTSTR)strData);
}

BOOL CModCanWnd::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	if (wParam == IDC_PASSWORD)
		OnNonContract();
	
	return CWnd::OnNotify(wParam, lParam, pResult);
}

LRESULT CModCanWnd::OnMyPassword(WPARAM wParam, LPARAM lParam)
{
	//CString rcb = (char*)lParam;
	//MessageBox((char*)lParam);
	m_pPassword->SetWindowText((char*)lParam);
	//MessageBox("Received: "+rcb);
	return 1;
}

bool CModCanWnd::IsNumber(CString str)
{
    bool bRet = true;
    const int nLen = str.GetLength();
    for( int i=0 ; i < nLen ; ++i)
    {
        if( isdigit(str.GetAt(i)) == FALSE)
        {
            bRet = false;
            break;
        }
    }
    return bRet;
}


void CModCanWnd::ChangeSize(double* lRate, int ipoint, LPCTSTR lszname)
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

	ileft +=  iw + 1;
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

	ileft +=  iw + igap;
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

	ileft = rc.right - (iw * 5) - (ibtngap*5) - igap;
	m_pClear->SetFont(pFont);
	m_pClear->MoveWindow(ileft, itop, iw, ih );
	ileft +=  iw + ibtngap;

	m_pNonContract->SetFont(pFont);
	m_pNonContract->MoveWindow(ileft, itop, iw, ih );
	ileft +=  iw + ibtngap;

	m_pModify->SetFont(pFont);
	m_pModify->MoveWindow(ileft, itop, iw, ih );
	ileft +=  iw + ibtngap;

	m_pCancel->SetFont(pFont);
	m_pCancel->MoveWindow(ileft, itop, iw, ih );
	ileft +=  iw + ibtngap;

	m_pContrect->SetFont(pFont);
	m_pContrect->MoveWindow(ileft, itop, iw, ih );
	
	itop = itop + ih + 2;

	//두번째 라인 컨트롤
	m_pAll->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width(), ih = wrc.Height() ;
	iw = iw + (int)(lRate[0] * (float)iw );
	ih = ih + (int)(lRate[1] * (float)ih);
	ileft = wrc.left ;
	m_pAll->SetFont(pFont);
	m_pAll->MoveWindow(ileft, itop, iw, ih );

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
	m_pCodeCtrlStatic->GetWindowRect(&wrc);
	ScreenToClient(&wrc); 
	iw = wrc.Width();
	iw = iw + (int)(lRate[0] * (float)iw );
	m_pCodeCtrlStatic->SetFont(pFont);
	m_pCodeCtrlStatic->MoveWindow(ileft, itop, iw, ih );

	ileft +=  iw + igap;
	m_pCodeCtrl->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width();
	iw = iw + (int)(lRate[0] * (float)iw );
	m_pCodeCtrl->SetFont(pFont);
	m_pCodeCtrl->MoveWindow(ileft, itop, iw, ih );
//

	ileft +=  iw + 1;
	m_pCodeSearch->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width();
	iw = iw + (int)(lRate[0] * (float)iw );
	m_pCodeSearch->SetFont(pFont);
	m_pCodeSearch->MoveWindow(ileft, itop, iw, ih );
	
	ileft +=  iw + 1;
	m_pSymbol->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width();
	iw = iw + (int)(lRate[0] * (float)iw );

	m_pSymbol->MoveWindow(ileft, itop, iw, ih );
	m_pSymbol->SendMessage(WM_USER, MAKEWPARAM(DLL_SETFONT, ipoint), (LPARAM)lszname);

	ileft +=  iw + igap;
	m_pQuantityStatic->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width();
	iw = iw + (int)(lRate[0] * (float)iw );
	m_pQuantityStatic->SetFont(pFont);
	m_pQuantityStatic->MoveWindow(ileft, itop, iw, ih );


	ileft +=  iw + igap;
	m_pQuantityRemain->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width();
	iw = iw + (int)(lRate[0] * (float)iw );
	m_pQuantityRemain->SetFont(pFont);
	m_pQuantityRemain->MoveWindow(ileft, itop, iw, ih );
	
	ileft +=  iw + igap;
	m_pQuantityPart->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width();
	iw = iw + (int)(lRate[0] * (float)iw );
	m_pQuantityPart->SetFont(pFont);
	m_pQuantityPart->MoveWindow(ileft, itop, iw, ih );

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
	m_pPercentStatic->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width();
	iw = iw + (int)(lRate[0] * (float)iw );
	m_pPercentStatic->SetFont(pFont);
	m_pPercentStatic->MoveWindow(ileft, itop, iw, ih );

	//세번째 라인 컨트롤
	itop += ih  +2;
	ih = rc.Height()  - itop - 2;
	m_pModCanGrid->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	ileft = wrc.left ;
	iw = wrc.Width();
	iw = iw + (int)(lRate[0] * (float)iw );

	iw = rc.right - ileft - 3;
	m_pModCanGrid->MoveWindow(ileft, itop, rc.Width() , ih);
	m_pModCanGrid->ChangeFont(pFont, lRate);

}

void CModCanWnd::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == 100)
	{
		KillTimer(100);
		if(m_pPassword)
		{

			CString sval = m_pParent->GetBuffer(_T("PASSWORD1") ); 
			//주문확인창
			sval = m_pParent->GetBuffer(_T("QORDER2") ); 
			if(!sval.IsEmpty())
				m_pOrderConfirm->SetCheck(atoi(sval)); 

			//전체?매도?매수
			sval = m_pParent->GetBuffer(_T("JTYPERdo2"));
			int ival = 0;
			if(!sval.IsEmpty())
			{
				ival = atoi(sval);
				if(ival > 0)
				{
					m_pAll->SetCheck(0); 
					if(ival == 1) m_pSell->SetCheck(1); 
					else m_pBuy->SetCheck(1);
				}
			}


			//수량: 잔량?일부
			sval = m_pParent->GetBuffer(_T("JVolRdo2"));
			if(!sval.IsEmpty())
			{
				ival = atoi(sval);
				if(ival)
				{
					m_pQuantityRemain->SetCheck(0); 
					m_pQuantityPart->SetCheck(1); 
				}
			}

			//단가
			sval = m_pParent->GetBuffer(_T("JPRCUNT2"));
			if(!sval.IsEmpty())
			{
				ival = atoi(sval);
				m_pPriceUnit->SetCurSel(ival); 
			}
		}
	}
	
	CWnd::OnTimer(nIDEvent);
}

void CModCanWnd::OnDestroy() 
{
	CWnd::OnDestroy();
	
	// TODO: Add your message handler code here
	CString sval;
	sval.Format("%d", m_pOrderConfirm->GetCheck() );
	m_pParent->SaveBuffer(_T("QORDER2"), sval);
	
	//전체?매도?매수
	if(m_pAll->GetCheck()) sval = "0";
	else if(m_pSell->GetCheck()) sval = "1";
	else sval = "2";
	m_pParent->SaveBuffer(_T("JTYPERdo2"), sval);

	sval = "0";
	if(m_pQuantityPart->GetCheck()) sval = "1";
	m_pParent->SaveBuffer(_T("JVolRdo2"), sval);

	sval.Format("%d", m_pPriceUnit->GetCurSel() );
	m_pParent->SaveBuffer(_T("JPRCUNT2"), sval);
}
