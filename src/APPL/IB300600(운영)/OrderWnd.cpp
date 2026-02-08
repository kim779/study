// OrderWnd.cpp : implementation file
//

#include "stdafx.h"
#include "IB300600.h"
#include "OrderWnd.h"
#include "MapWnd.h"
#include "Define.h"
#include "../../control/fx_misc/misctype.h"
#include "ConfirmDlg.h"

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
	, m_pAccount(NULL)
	, m_pAccName(NULL)
	, m_pPassStatic(NULL)
	, m_pPassword(NULL)
	, m_pClear(NULL)
	, m_pRemain(NULL)
	, m_pOrderSend(NULL)
	, m_pContract(NULL)
	, m_pSell(NULL)
	, m_pBuy(NULL)
	, m_pOrderTypeStatic(NULL)
	, m_pOrderPriceType(NULL)
	, m_pOrderPrice(NULL)
	, m_pOrderSpin(NULL)
	, m_pQuantityStatic(NULL)
	, m_pQuantity(NULL)
	, m_pQuantitySpin(NULL)
	, m_pOrderGrid(NULL)
	, m_strCurPrice("100.00")
	, m_bBlock(FALSE)
{
}

COrderWnd::~COrderWnd()
{
	_SAFEDESTROY(m_pAccount);
	_SAFEDESTROY(m_pAccName);
	_SAFEDESTROY(m_pPassStatic);
	_SAFEDESTROY(m_pPassword);
	_SAFEDESTROY(m_pClear);
	_SAFEDESTROY(m_pOrderSend);
	_SAFEDESTROY(m_pContract);
	_SAFEDESTROY(m_pSell);
	_SAFEDESTROY(m_pBuy);
	_SAFEDESTROY(m_pOrderTypeStatic);
	_SAFEDESTROY(m_pOrderType);
	_SAFEDESTROY(m_pOrderPriceStatic);
	_SAFEDESTROY(m_pOrderPriceType);
	_SAFEDESTROY(m_pOrderPrice);
	_SAFEDESTROY(m_pOrderSpin);
	_SAFEDESTROY(m_pQuantityStatic);
	_SAFEDESTROY(m_pQuantity);
	_SAFEDESTROY(m_pQuantitySpin);
	_SAFEDESTROY(m_pOrderGrid);
}


BEGIN_MESSAGE_MAP(COrderWnd, CWnd)
	//{{AFX_MSG_MAP(COrderWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_CTLCOLOR()
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CLEAR2, OnClear)
	ON_BN_CLICKED(IDC_REMAIN, OnRemain)
	ON_BN_CLICKED(IDC_ORDERSEND, OnOrderSend)
	ON_BN_CLICKED(IDC_BUY, OnBuy)
	ON_BN_CLICKED(IDC_SELL, OnSell)
	ON_BN_CLICKED(IDC_CONTRACT, OnContract)
	ON_CBN_SELCHANGE(IDC_ORDERTYPE, OnSelchangeOrderType)
	ON_CBN_SELCHANGE(IDC_ORDERPRICETYPE, OnSelchangeOrderPriceType)
	ON_EN_CHANGE(IDC_QUANTITY, OnQuantity)
	ON_EN_CHANGE(IDC_ORDERPRICE, OnOrderPrice)
	ON_MESSAGE(WM_USER + 1, OnUser)
	ON_MESSAGE(UM_PASSWORD, OnMyPassword)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// COrderWnd message handlers

void COrderWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CMapWnd* pWnd = (CMapWnd*)GetParent();
	
	CRect rcClient;
	GetClientRect(&rcClient);

	dc.FillSolidRect(&rcClient, pWnd->GetIndexColor(CLR_PANNEL_BACK));
}

int COrderWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_pParent = (CMapWnd*)GetParent();

	CRect rcCtrl, rcClient;
	GetClientRect(&rcClient);

	rcCtrl.SetRect(SZ_TAB_HMARGIN, SZ_FORM_MARGIN, SZ_TAB_HMARGIN + SZ_CTRL_ACCN, SZ_FORM_MARGIN + SZ_CTRL_HEIGHT);

	m_pAccount = new CAccountCtrl(m_pWizard, this, m_pParent->GetAxRoot());
	m_pAccount->CreateEx(WS_EX_TOPMOST, NULL, "ACCN", WS_VISIBLE|WS_CHILD|WS_TABSTOP, rcCtrl, this, IDC_ACCOUNT);
	m_pAccount->createAccountCtrl("AN20", KEY_ACCOUNT, m_pParent->GetIndexColor(CLR_PANNEL_BACK));
	
	rcCtrl.left = rcCtrl.right + 2;					// 계좌 컨트롤과 계좌명 컨트롤간의 간격은 1로 한다.
	rcCtrl.right = rcCtrl.left + SZ_CTRL_ACCNM;

	m_pAccName = new CfxStatic;
	m_pAccName->Create(_T(""), WS_CHILD | WS_VISIBLE | SS_LEFT | WS_CLIPCHILDREN | 
							WS_CLIPSIBLINGS, rcCtrl, this, IDC_ACCOUNTNAME);

	rcCtrl.left = rcCtrl.right + SZ_CTRL_HGAP + 3;	// 컨트롤 그룹간 간격은 5로 한다. (SZ_CTRL_HGAP : 5px,  fxStatic내부 마진 2px)
	rcCtrl.right = rcCtrl.left + SZ_CTRL_4STATIC;
	
	m_pPassStatic = new CfxStatic;
	m_pPassStatic->Create(_T("비밀번호"), WS_CHILD | WS_VISIBLE | SS_LEFT | WS_CLIPCHILDREN | 
							WS_CLIPSIBLINGS, rcCtrl, this, IDC_PASSSTATIC);

	rcCtrl.left = rcCtrl.right + 3;					// 캡션과 컨트롤간 간격은 5로 한다. (fxStatic 내부마진 2px)
	rcCtrl.right = rcCtrl.left + SZ_CTRL_3EDIT;
	
	m_pPassword = new CfxEdit;
	m_pPassword->Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_PASSWORD, rcCtrl, this, IDC_PASSWORD);

	rcCtrl.left = rcClient.right - (SZ_CTRL_4BUTTON * 4) - 4 - SZ_TAB_HMARGIN;
	rcCtrl.right = rcCtrl.left + SZ_CTRL_4BUTTON;

	m_pClear = new CfxImgButton;
	m_pClear->Create("\nClear", rcCtrl, this, IDC_CLEAR2);
	
	rcCtrl.left = rcCtrl.right + 1;
	rcCtrl.right = rcCtrl.left + SZ_CTRL_4BUTTON;

	m_pRemain = new CfxImgButton;
	m_pRemain->Create("\n잔고조회", rcCtrl, this, IDC_REMAIN);

	rcCtrl.left = rcCtrl.right + 1;
	rcCtrl.right = rcCtrl.left + SZ_CTRL_4BUTTON;

	m_pOrderSend = new CfxImgButton;
	m_pOrderSend->Create("\n주문전송", rcCtrl, this, IDC_ORDERSEND);

	rcCtrl.left = rcCtrl.right + 1;
	rcCtrl.right = rcCtrl.left + SZ_CTRL_4BUTTON;

	m_pContract = new CfxImgButton;
	m_pContract->Create("\n체결내역", rcCtrl, this, IDC_CONTRACT);

	rcCtrl.left = SZ_TAB_HMARGIN;
	rcCtrl.right = SZ_RADIO_2WIDTH;
	rcCtrl.top = rcCtrl.bottom + SZ_CTRL_VGAP + 1;
	rcCtrl.bottom = rcCtrl.top + SZ_CTRL_HEIGHT;

	m_pSell = new CButton;
	m_pSell->Create(_T("매도"), WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | BS_AUTORADIOBUTTON | 
						WS_GROUP | BS_TOP | BS_BOTTOM, rcCtrl, this, IDC_SELL);

	rcCtrl.left = rcCtrl.right + SZ_CTRL_HGAP;
	rcCtrl.right = rcCtrl.left + SZ_RADIO_2WIDTH;

	m_pBuy = new CButton;
	m_pBuy->Create(_T("매수"), WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | BS_AUTORADIOBUTTON | 
						BS_TOP | BS_BOTTOM, rcCtrl, this, IDC_BUY);

	rcCtrl.top -= 1;
	rcCtrl.bottom -= 1;
	rcCtrl.left = rcCtrl.right + SZ_CTRL_HGAP + 3;
	rcCtrl.right = rcCtrl.left + SZ_CTRL_4STATIC;
	
	m_pOrderTypeStatic = new CfxStatic;
	m_pOrderTypeStatic->Create(_T("주문유형"), WS_CHILD | WS_VISIBLE | SS_LEFT | WS_CLIPCHILDREN | 
								WS_CLIPSIBLINGS, rcCtrl, this, IDC_ORDERTYPESTATIC);

	rcCtrl.left = rcCtrl.right + 3;					// 캡션과 컨트롤간 간격은 5로 한다. (fxStatic 내부마진 2px)
	rcCtrl.right = rcCtrl.left + SZ_COMBO_3WIDTH;

	m_pOrderType = new CComboBox;
	m_pOrderType->Create(WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | CBS_DROPDOWNLIST, 
						CRect(rcCtrl.left, rcCtrl.top, rcCtrl.right, 200), this, IDC_ORDERTYPE);

	rcCtrl.left = rcCtrl.right + SZ_CTRL_HGAP + 3;
	rcCtrl.right = rcCtrl.left + SZ_CTRL_4STATIC;
	
	m_pOrderPriceStatic = new CfxStatic;
	m_pOrderPriceStatic->Create(_T("주문가격"), WS_CHILD | WS_VISIBLE | SS_LEFT | WS_CLIPCHILDREN | 
								WS_CLIPSIBLINGS, rcCtrl, this, IDC_ORDERTYPESTATIC);

	rcCtrl.left = rcCtrl.right + 3;					// 캡션과 컨트롤간 간격은 5로 한다. (fxStatic 내부마진 2px)
	rcCtrl.right = rcCtrl.left + SZ_COMBO_5WIDTH;

	m_pOrderPriceType = new CComboBox;
	m_pOrderPriceType->Create(WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | CBS_DROPDOWNLIST, 
						CRect(rcCtrl.left, rcCtrl.top, rcCtrl.right, 200), this, IDC_ORDERPRICETYPE);

	rcCtrl.left = rcCtrl.right + 2;
	rcCtrl.right = rcCtrl.left + SZ_COMBO_3WIDTH;

	m_pOrderPrice = new CfxEdit;
	m_pOrderPrice->Create(WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | ES_RIGHT, rcCtrl, this, IDC_ORDERPRICE);
/*	m_pOrderPrice->CreateEx(WS_EX_CLIENTEDGE, _T("EDIT"), "", 
							WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER, 
							rcCtrl.left, rcCtrl.top, rcCtrl.Width(), rcCtrl.Height(),
							this->GetSafeHwnd(), (HMENU)IDC_ORDERPRICE);
*/

	rcCtrl.left = rcCtrl.right - 20;
	rcCtrl.right = rcCtrl.left + 20;
	m_pOrderSpin = new CNumSpinCtrl;
	m_pOrderSpin->Create(WS_CHILD | WS_VISIBLE | UDS_ALIGNRIGHT | UDS_AUTOBUDDY | UDS_ARROWKEYS, rcCtrl, this, IDC_ORDERSPIN);

	rcCtrl.left = rcCtrl.right + SZ_CTRL_HGAP + 3;
	rcCtrl.right = rcCtrl.left + SZ_CTRL_2STATIC;
	m_pQuantityStatic = new CfxStatic;
	m_pQuantityStatic->Create(_T("수량"), WS_CHILD | WS_VISIBLE | SS_LEFT | WS_CLIPCHILDREN | 
								WS_CLIPSIBLINGS, rcCtrl, this, IDC_QUANTITYSTATIC);

	rcCtrl.left = rcCtrl.right + 3;
	rcCtrl.right = rcCtrl.left + SZ_COMBO_3WIDTH;

	m_pQuantity = new CfxEdit;
	m_pQuantity->Create(WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | ES_RIGHT, rcCtrl, this, IDC_QUANTITY);

	rcCtrl.left = rcCtrl.right - 20;
	rcCtrl.right = rcCtrl.left + 20;
	m_pQuantitySpin = new CNumSpinCtrl;
	m_pQuantitySpin->Create(WS_CHILD | WS_VISIBLE | UDS_ALIGNRIGHT | UDS_AUTOBUDDY | UDS_ARROWKEYS, rcCtrl, this, IDC_QUANTITYSPIN);

	rcCtrl.left = SZ_TAB_HMARGIN;
	rcCtrl.top = rcCtrl.bottom + SZ_CTRL_VGAP;
	rcCtrl.right = rcClient.Width() - SZ_CTRL_VGAP;
	rcCtrl.bottom = rcClient.Height() - SZ_FORM_MARGIN;
	
	m_pOrderGrid = new COrderGrid;
	m_pOrderGrid->CreateGrid(WS_CHILD | WS_VISIBLE, rcCtrl, this, IDC_ORDERGRID);

	return 0;
}

void COrderWnd::Init()
{
	CFont* pFont = m_pParent->GetNormalFont();

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

	m_pOrderSend->SetImgBitmap(hbmp, hbmp_dn, hbmp_hv);
	m_pOrderSend->SetWindowText(_T("주문전송"));
	m_pOrderSend->SetFont(pFont);

	m_pContract->SetImgBitmap(hbmp, hbmp_dn, hbmp_hv);
	m_pContract->SetWindowText(_T("체결"));
	m_pContract->SetFont(pFont);

	m_pSell->SetFont(pFont);
	m_pSell->SetCheck(1);

	m_pBuy->SetFont(pFont);

	m_pOrderTypeStatic->SetBkColor(m_pParent->GetIndexColor(CLR_PANNEL_BACK));
	m_pOrderTypeStatic->SetFgColor(m_pParent->GetIndexColor(CLR_STATIC_FORE));
	m_pOrderTypeStatic->SetFont(pFont);

	m_pOrderType->SetFont(pFont);
	m_pOrderType->AddString(_T("지정가"));
	m_pOrderType->AddString(_T("시장가"));
	m_pOrderType->AddString(_T("조건부"));
	m_pOrderType->AddString(_T("최유리"));
	m_pOrderType->AddString(_T("IOC"));
	m_pOrderType->AddString(_T("FOK"));
	m_pOrderType->SetCurSel(0);

	m_pOrderPriceStatic->SetBkColor(m_pParent->GetIndexColor(CLR_PANNEL_BACK));
	m_pOrderPriceStatic->SetFgColor(m_pParent->GetIndexColor(CLR_STATIC_FORE));
	m_pOrderPriceStatic->SetFont(pFont);

	m_pOrderPriceType->SetFont(pFont);
	m_pOrderPriceType->AddString(_T("현재가"));
	m_pOrderPriceType->AddString(_T("상대1호가"));
	m_pOrderPriceType->AddString(_T("상대2호가"));
	m_pOrderPriceType->AddString(_T("상대3호가"));
	m_pOrderPriceType->AddString(_T("우선1호가"));
	m_pOrderPriceType->AddString(_T("우선2호가"));
	m_pOrderPriceType->AddString(_T("우선3호가"));
	m_pOrderPriceType->AddString(_T("지정가"));
	m_pOrderPriceType->SetCurSel(0);

	m_pOrderPrice->SetFont(pFont);
	m_pOrderPrice->ModifyAttribute(0, attrNUMERIC);
	m_pOrderPrice->EnableWindow(FALSE);

	m_pOrderSpin->SetDecimalPlaces(2);
	m_pOrderSpin->SetTrimTrailingZeros(FALSE);
	m_pOrderSpin->SetRangeAndDelta(0.00, 10000.00, 0.05);
	m_pOrderSpin->SetPos(100.00);

	m_pQuantityStatic->SetBkColor(m_pParent->GetIndexColor(CLR_PANNEL_BACK));
	m_pQuantityStatic->SetFgColor(m_pParent->GetIndexColor(CLR_STATIC_FORE));
	m_pQuantityStatic->SetFont(pFont);

	m_pQuantity->SetFont(pFont);
	m_pQuantity->ModifyAttribute(0, attrNUMERIC);

	m_pQuantitySpin->SetDecimalPlaces(0);
	m_pQuantitySpin->SetTrimTrailingZeros(FALSE);
	m_pQuantitySpin->SetRangeAndDelta(0, 100000000, 1);
	m_pQuantitySpin->SetPos(0);

	m_pOrderGrid->OnSetup();

	SetInputFieldFocus();
}

HBRUSH COrderWnd::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CWnd::OnCtlColor(pDC, pWnd, nCtlColor);
	
    UINT nID = pWnd->GetDlgCtrlID();

    if (nID == IDC_SELL || nID == IDC_BUY)
    {    
		pDC->SetBkMode(TRANSPARENT);                              
		return (HBRUSH)::GetStockObject(NULL_BRUSH);                      
    }

	// TODO: Return a different brush if the default is not desired
	return hbr;
}

CString COrderWnd::GetHistory(CString strSymbol)
{
	return m_pParent->Variant(historyCC, strSymbol);
}

CWnd* COrderWnd::GetWizard()
{	
	return m_pWizard;
}

void COrderWnd::SetHistory(CString strData)
{
	CString param;
	param.Format("ed_focod\t%s", strData);

	m_pParent->Variant(codeCC, param);
}

CString COrderWnd::GetOrderType()
{
	CString strVal(_T(""));
	m_pOrderType->GetWindowText(strVal);
	
	return strVal;
}

CString COrderWnd::GetOrderPrice()
{
	CString strVal(_T(""));
	m_pOrderPrice->GetWindowText(strVal);

	return strVal;
}

CString COrderWnd::GetQuantity()
{
	CString strVal(_T(""));
	m_pQuantity->GetWindowText(strVal);

	return strVal;
}

CString COrderWnd::GetOrderPriceType()
{
	CString strVal(_T(""));
	m_pOrderPriceType->GetWindowText(strVal);

	return strVal;
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

void COrderWnd::OnClear()
{
	ClearOrderGrid();
}

void COrderWnd::OnRemain()
{
	ClearOrderGrid();

	CString strAccount(_T(""));
	CString strPassword(_T(""));
	CString strEncPass(_T(""));
	CString strUser(_T(""));
	CString strSendData(_T(""));
	struct _ledgerH ledger;
	struct f_mid mid;

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

	strAccount = m_pAccount->GetAccNo();
	strUser = m_pParent->Variant(userCC, "");
	strEncPass.Format("%s\t%s", strPassword, strAccount.Left(8));
	strPassword = (char *)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(encPASSx, modeACN), (LPARAM)(LPCTSTR)strEncPass);

	FillMemory(&ledger, L_ledgerH, ' ');

	m_pParent->SendMessage(WM_USER, ledgerDLL, (LPARAM)&ledger);
	CopyMemory(&ledger.svcd, _T("SONBQ021"), sizeof(ledger.svcd));
	CopyMemory(&ledger.usid, (LPCTSTR)strUser, strUser.GetLength());
	CopyMemory(&ledger.brno, strAccount.Left(3), 3);
	CopyMemory(&ledger.rcnt, _T("0000"), sizeof(ledger.rcnt));
	
	ledger.fkey[0] = 'C';
	ledger.mkty[0] = '2';
	ledger.odrf[0] = '1';

	strSendData = CString((char*)&ledger, L_ledgerH);

	FillMemory(&mid, L_fmid, ' ');

	CopyMemory(&mid.accn, strAccount, sizeof(mid.accn));
	CopyMemory(&mid.pswd, strPassword, strPassword.GetLength());
	CopyMemory(&mid.date, "", sizeof(mid.date));
	strSendData += CString((char*)&mid, L_fmid);
	
	m_bBlock = TRUE;
	m_pParent->SendTR("PIBOFJGO", strSendData, KEY_FREMAIN, 0);
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
	
	int nCount = 0;
	CStringArray arrData;
	CStringArray arrConfirm;
	CString strData(_T(""));
	CString strCode(_T(""));		// 종목코드
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

	m_pOrderGrid->GetSendData(&arrData);
	nCount = arrData.GetSize();

	if (nCount == 0)
		return;

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

	strUser = m_pParent->Variant(userCC, "");
	strEncPass.Format("%s\t%s", strPassword, strAccount.Left(8));
	strPassword = (char *)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(encPASSx, modeACN), (LPARAM)(LPCTSTR)strEncPass);

	struct i_com mid;

	FillMemory(&mid, L_i_com, ' ');

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

	strCount.Format("%04d", nCount);

	CopyMemory(mid.nrec, (LPCTSTR)strCount, strCount.GetLength());

	for (i = 0; i < nCount; i++)
	{
		struct irec rec;
		strData = arrData.GetAt(i);

		if (strData.IsEmpty())
			continue;
		
		FillMemory(&rec, L_irec, ' ');

		strCode = Parser(strData, _T("\t"));
		Parser(strData, _T("\t"));
		strOrderState = Parser(strData, _T("\t"));
		strOrderType = Parser(strData, _T("\t"));
		strQuantity = Parser(strData, _T("\t"));
		strOrderPrice = Parser(strData, _T("\t"));

		int nPrice = (int)(atof((LPCTSTR)strOrderPrice) * 100);
		strOrderPrice.Format("%d", nPrice);

		rec.odgb[0] = _T('2');
		rec.mmgb[0] = strOrderState == _T("매도") ? _T('1') : _T('2');
		CopyMemory(rec.accn, strAccount, strAccount.GetLength());
		CopyMemory(rec.pswd, strPassword, strPassword.GetLength());
		CopyMemory(rec.cod2, strCode, strCode.GetLength());
		CopyMemory(rec.jqty, strQuantity, strQuantity.GetLength());
		CopyMemory(rec.jprc, strOrderPrice, strOrderPrice.GetLength());

		_TRIMDATA(strOrderType);

		if (strOrderType == _T("지정가") || strOrderType == _T("IOC") || strOrderType == _T("FOK") )
			CopyMemory(rec.hogb, _T("00"), sizeof(rec.hogb));
		else if (strOrderType == _T("시장가"))
		{
			CopyMemory(rec.hogb, _T("03"), sizeof(rec.hogb));
			CopyMemory(rec.jprc, _T("0000000000"), sizeof(rec.jprc));
		}
		else if (strOrderType == _T("조건부"))
			CopyMemory(rec.hogb, _T("05"), sizeof(rec.hogb));
		else if (strOrderType == _T("최유리"))
			CopyMemory(rec.hogb, _T("06"), sizeof(rec.hogb));

		if (strOrderType == _T("IOC"))
			rec.cdgb[0] = _T('1');
		else if (strOrderType == _T("FOK"))
			rec.cdgb[0] = _T('2');
		else
			rec.cdgb[0] = _T('0');

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

	ledger.fkey[0] = 'C';
	ledger.mkty[0] = '1';
	ledger.odrf[0] = '2';

	strSendData = CString((char*)&ledger, L_ledgerH);


	strSendData += CString((char*)&mid, nCount * L_irec + 4);
	m_pParent->SendTR("PIBOFODR", strSendData, KEY_FORDER, 0);
}

void COrderWnd::OnBuy()
{
	m_pOrderGrid->SetOrderState(_T("매수"));
}

void COrderWnd::OnSell()
{
	m_pOrderGrid->SetOrderState(_T("매도"));
}

void COrderWnd::OnSelchangeOrderType()
{
	CString strVal(_T(""));
	m_pOrderType->GetWindowText(strVal);
	m_pOrderGrid->SetOrderType(strVal);
}

void COrderWnd::OnSelchangeOrderPriceType()
{
	int nCurSel = m_pOrderPriceType->GetCurSel();
	if (nCurSel == 7)
	{
		m_pOrderPrice->EnableWindow(TRUE);
		CString strVal(_T(""));
		m_pOrderPrice->GetWindowText(strVal);
		m_pOrderGrid->SetOrderPrice(strVal);
	}
	else
	{
		m_pOrderPrice->EnableWindow(FALSE);
		QueryFOCHoga(TRUE);
	}
}

void COrderWnd::OnQuantity()
{
	CString strVal(_T(""));
	m_pQuantity->GetWindowText(strVal);
	m_pOrderGrid->SetQuantity(strVal);
}

void COrderWnd::OnOrderPrice()
{
	CString strVal(_T(""));
	m_pOrderPrice->GetWindowText(strVal);
	m_pOrderGrid->SetOrderPrice(strVal);
}

void COrderWnd::SetHogaData(CString strData)
{
	if (strData.IsEmpty())
		return;

	CString strSymbol = Parser(strData, _T("\t"));

	if (strSymbol != SYMBOL_HOGA)
		return;

	CString strCode = Parser(strData, _T("\t")); 
	CString strVal = Parser(strData, _T("\t"));

	m_pOrderGrid->SetTriggerData(strCode, strVal);
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

//DEL void COrderWnd::SetFiveHoga(CString strData)
//DEL {
//DEL 	if (strData.IsEmpty())
//DEL 		return;
//DEL 
//DEL 	m_arrBuyFive.RemoveAll();
//DEL 	m_arrBuyFive.Add(Parser(strData, _T("\t")));
//DEL 	m_arrBuyFive.Add(Parser(strData, _T("\t")));
//DEL 	m_arrBuyFive.Add(Parser(strData, _T("\t")));
//DEL 	m_arrBuyFive.Add(Parser(strData, _T("\t")));
//DEL 	m_arrBuyFive.Add(Parser(strData, _T("\t")));
//DEL 	
//DEL 	m_arrSellFive.RemoveAll();
//DEL 	m_arrSellFive.Add(Parser(strData, _T("\t")));
//DEL 	m_arrSellFive.Add(Parser(strData, _T("\t")));
//DEL 	m_arrSellFive.Add(Parser(strData, _T("\t")));
//DEL 	m_arrSellFive.Add(Parser(strData, _T("\t")));
//DEL 	m_arrSellFive.Add(Parser(strData, _T("\t")));
//DEL }


void COrderWnd::ParsingFRemain(CString strData, int nLen, CString strMsg)
{
	if (strData.GetLength() < 15 + L_frec)
		return;

	CString strCode(_T(""));
	CUGCell cell;
	CString strVal(_T(""));
	CString strAccnount = strData.Left(11);
	int nRec = atoi((LPCTSTR)strData.Mid(11, 4));

	strData.Delete(0, 15);
	struct f_rec rec;

	ClearOrderGrid();
	
	if (nRec >= m_pOrderGrid->GetNumberRows())
		m_pOrderGrid->SetNumberRows(nRec + 1);

	for (int i = 0; i < nRec; i++)
	{
		CopyMemory(&rec, (LPCTSTR)strData.Left(L_frec), L_frec);

		strVal = CString(rec.cod2, sizeof(rec.cod2));
		_TRIMDATA(strVal);
		m_pOrderGrid->QuickSetText(1, i, strVal);

		strVal = CString(rec.hnam, sizeof(rec.hnam));
		_TRIMDATA(strVal);

		strVal.Insert(2, _T("20"));

		if (strVal.GetAt(0) == _T('F'))
			strVal = _T("K200 ") + strVal;
		
		m_pOrderGrid->QuickSetText(3, i, strVal);

		strVal = CString(rec.dsgb, sizeof(rec.dsgb));
		_TRIMDATA(strVal);

		if (strVal.GetAt(0) == _T('+') || strVal.GetAt(0) == _T('-'))
			strVal.Delete(0);

		m_pOrderGrid->QuickSetText(4, i, strVal);

		strVal = CString(rec.curr, sizeof(rec.curr));
		_TRIMDATA(strVal);
		m_pOrderGrid->QuickSetText(8, i, strVal);

		m_pOrderGrid->QuickSetText(5, i, (LPCTSTR)GetOrderType());
		m_pOrderGrid->QuickSetText(6, i, (LPCTSTR)CString(rec.cqty, sizeof(rec.cqty)));

		if (strVal.GetAt(0) == _T('+') || strVal.GetAt(0) == _T('-'))
			strVal.Delete(0);

		m_pOrderGrid->QuickSetText(7, i, strVal);
		m_pOrderGrid->SetRowHeight(i, SZ_ROWHEIGHT);
		strData.Delete(0, L_frec);
		m_pOrderGrid->GotoRow(i);
	}

	m_pOrderGrid->RedrawAll();

	CString strProc;
	strVal = m_pOrderGrid->QuickGetText(1, nRec - 1);
	strProc.Format("change_code\t%s", strVal);
	m_pParent->Procedure(strProc);

	m_bBlock = FALSE;
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

void COrderWnd::ParsingFOCHoga(CString strData, BOOL bHoga)
{
	if (strData.IsEmpty())
		return;

	int nCount = atoi((LPCTSTR)strData.Left(4));

	CString strCurPrice(_T(""));
	CString strCode(_T(""));
	CString strPrice(_T(""));
	CStringArray arrHoga;
	struct o_hoga hoga;

	strData.Delete(0, 4);

	for (int i = 0; i < nCount; i++)
	{
		CString strVal;
		CopyMemory(&hoga, (LPCTSTR)strData.Left(L_o_hoga), L_o_hoga);
		strData.Delete(0, L_o_hoga);

		strCode = CString(hoga.cod2, sizeof(hoga.cod2));
		strCurPrice = CString(hoga.curr, sizeof(hoga.curr));
		strPrice = CString(hoga.hoga, sizeof(hoga.hoga));

		_TRIMDATA(strCode);
		_TRIMDATA(strCurPrice);
		_TRIMDATA(strPrice);
		
		strVal.Format("%s\t%s\t%s", strCode, strCurPrice, strPrice);
		arrHoga.Add(strVal);
	}

	m_pOrderGrid->SetFOCData(&arrHoga, bHoga);
}

void COrderWnd::QueryFOCHoga(BOOL bHoga, CString strCode)
{
	CStringArray arrHoga;
	int nCount = 0;

	if (bHoga)
	{
		m_pOrderGrid->GetCheckCode(&arrHoga);
		nCount = arrHoga.GetSize();
	}
	else
		nCount = 1;

	int nCurSel = m_pOrderPriceType->GetCurSel();

	if (nCount < 0)
		return;
	
	struct fhmid mid;
	CString strSendData(_T(""));
	CString strCount(_T(""));
	CString strVal;
	
	strCount.Format("%04d", nCount);
	
	FillMemory(&mid, L_fhmid, ' ');
	
	switch (nCurSel)
	{
	case 0:
		CopyMemory(&mid.gubn, _T("00"), sizeof(mid.gubn));
		break;
	case 1:
		CopyMemory(&mid.gubn, _T("20"), sizeof(mid.gubn));
		break;
	case 2:
		CopyMemory(&mid.gubn, _T("21"), sizeof(mid.gubn));
		break;
	case 3:
		CopyMemory(&mid.gubn, _T("22"), sizeof(mid.gubn));
		break;
	case 4:
		CopyMemory(&mid.gubn, _T("10"), sizeof(mid.gubn));
		break;
	case 5:
		CopyMemory(&mid.gubn, _T("11"), sizeof(mid.gubn));
		break;
	case 6:
		CopyMemory(&mid.gubn, _T("12"), sizeof(mid.gubn));
		break;
	}
	
	CopyMemory(&mid.nrec, (LPCTSTR)strCount, sizeof(mid.nrec));
	
	if (bHoga)
	{
		for (int i = 0; i < nCount; i++)
		{
			strVal = arrHoga.GetAt(i);
			struct i_hoga hoga;
			FillMemory(&hoga, L_i_hoga, ' ');
			
			hoga.dsgb[0] = strVal.GetAt(0);
			
			CopyMemory(&hoga.cod2, strVal.Right(8), sizeof(hoga.cod2));
			CopyMemory(&mid.rec[i], &hoga, L_i_hoga);
		}
	}
	else
	{
		struct i_hoga hoga;
		FillMemory(&hoga, L_i_hoga, ' ');

		hoga.dsgb[0] = m_pSell->GetCheck() == 1 ? _T('1') : _T('2');

		CopyMemory(&hoga.cod2, strCode, sizeof(hoga.cod2));
		CopyMemory(&mid.rec[0], &hoga, L_i_hoga);
	}
	
	strSendData = CString((char*)&mid, nCount * L_i_hoga + 6);
	
	m_pParent->SendTR("PIBO3006", strSendData, bHoga ? KEY_FOHOGA : KEY_FOCURR, 0);
}

void COrderWnd::ParsingFOrder(CString strData, int nLen, CString strMsg)
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


long COrderWnd::OnUser(WPARAM wParam, LPARAM lParam)
{
	switch(LOWORD(wParam))
	{
	case 100:	// CAccountCtrl 계좌변경시 수신 Message
		{
			if (!m_pAccName)
				return 0;

			CString data = (char*)lParam;

			/*
			Parser(data, "\t");
			m_pAccName->SetText(Parser(data, "\t"));
			Parser(data, "\t");

			m_pPassword->SetFocus();
			*/

			CString accno = Parser(data, "\t");
			CString accnm = Parser(data, "\t");
			CString accpw = Parser(data, "\t");
			m_pAccName->SetText(accnm);
			m_pPassword->SetText(accpw);
			m_pPassword->SetFocus();
			if (accpw.GetLength()>0) this->OnRemain();
		}
		break;
	}
	return 0;
}

void COrderWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CWnd::OnShowWindow(bShow, nStatus);
	
	if (bShow)
		SetInputFieldFocus();
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

void COrderWnd::SetPallete()
{
	m_pAccName->SetBkColor(m_pParent->GetIndexColor(CLR_PANNEL_BACK));
	m_pAccName->SetFgColor(m_pParent->GetIndexColor(CLR_STATIC_FORE));
	m_pPassStatic->SetBkColor(m_pParent->GetIndexColor(CLR_PANNEL_BACK));
	m_pPassStatic->SetFgColor(m_pParent->GetIndexColor(CLR_STATIC_FORE));
	m_pOrderTypeStatic->SetBkColor(m_pParent->GetIndexColor(CLR_PANNEL_BACK));
	m_pOrderTypeStatic->SetFgColor(m_pParent->GetIndexColor(CLR_STATIC_FORE));
	m_pOrderPriceStatic->SetBkColor(m_pParent->GetIndexColor(CLR_PANNEL_BACK));
	m_pOrderPriceStatic->SetFgColor(m_pParent->GetIndexColor(CLR_STATIC_FORE));
	m_pQuantityStatic->SetBkColor(m_pParent->GetIndexColor(CLR_PANNEL_BACK));
	m_pQuantityStatic->SetFgColor(m_pParent->GetIndexColor(CLR_STATIC_FORE));

	m_pOrderGrid->SetPallete();
	Invalidate();
}


BOOL COrderWnd::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	if (wParam == IDC_PASSWORD)
		OnRemain();
	
	return CWnd::OnNotify(wParam, lParam, pResult);
}

void COrderWnd::OnContract()
{
	CMenu* pMenu;
	CRect  rcMenu;
	int nCount = 0;

	pMenu = new CMenu;
	pMenu->CreatePopupMenu();
	pMenu->AppendMenu(MF_STRING, 0, "체결조회");
	pMenu->AppendMenu(MF_STRING, 1, "미체결조회");

	m_pContract->GetWindowRect(&rcMenu);

	int nSelectGroup = pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON | TPM_RETURNCMD,
								rcMenu.left, rcMenu.bottom, this);
	delete pMenu;

	CString strData(_T(""));
	if (nSelectGroup == 1)
		strData = _T("IB310100 /S /t");
	else if (nSelectGroup == 0)
		strData = _T("IB310200 /S /t");

	m_pWizard->SendMessage(WM_USER, MAKEWPARAM(viewDLL, typeVIEW), (LPARAM)(LPCTSTR)strData);
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
	}else{
		return 1;
	}
}

bool COrderWnd::IsNumber(CString str)
{
    BOOL bRet = TRUE;
    int nLen = str.GetLength();
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

void COrderWnd::OnAlert(CString altmsg)
{
	int st, ed;
	CString key = Parser(altmsg, "\t");
	long rows = m_pOrderGrid->GetNumberRows();
	for(long i=0; i<rows; ++i)
	{
		CString code = m_pOrderGrid->QuickGetText(1, i);
		if (code==key) {
			CString currSymb = "\t023\t";	/* 현재가 심볼을 찾는다 */
			if ((st=altmsg.Find(currSymb))>-1) {	
				/* 현재가값을 추출한다 */
				st += currSymb.GetLength();
				ed = altmsg.Find('\t', st);

				char giho = altmsg[st];
				int col;
				switch(giho)
				{
				case '+': col = 94; break;
				case '-': col = 95; break;
				default : col = 69; break;
				}
				
				m_pOrderGrid->QuickSetText(8, i, (LPCTSTR)altmsg.Mid(st, ed-st));
				/*
				CUGCell cell;
				m_pOrderGrid->GetCell(8, i, &cell);
				cell.SetTextColor(m_pParent->GetAxColor(col));
				cell.SetText((LPCTSTR)altmsg.Mid(st+1, ed-st-1));
				m_pOrderGrid->SetCell(8, i, &cell);
				*/
				m_pOrderGrid->RedrawCell(8,i);

				break;
			}
			break;
		}
	}
}
