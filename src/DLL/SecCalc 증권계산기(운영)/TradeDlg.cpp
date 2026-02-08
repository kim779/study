// TradeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SecCalc.h"
#include "TradeDlg.h"
#include "FeeChangeDlg.h"
#include "misc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTradeDlg dialog


CTradeDlg::CTradeDlg(class CSecCalcDlg *pDlg, CWnd* pParent /*=NULL*/)
	: CDialog(CTradeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTradeDlg)
	//}}AFX_DATA_INIT
	m_pApp = (CSecCalcApp *)AfxGetApp();
	m_pMainDlg = pDlg;
	m_CurMarket = mk_None;
}


void CTradeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTradeDlg)
	DDX_Control(pDX, IDC_CALC_EDIT, m_CalcEdit);
	DDX_Control(pDX, IDC_EDIT_IVOL, m_ctrlInputVol);
	DDX_Control(pDX, IDC_EDIT_IPRICE, m_ctrlInputPrice);
	DDX_Control(pDX, IDC_STATIC_IUVOL, m_ctrlStaticIUVol);
	DDX_Control(pDX, IDC_STATIC_IUPRICE, m_ctrlStaticIUPrice);
	DDX_Control(pDX, IDC_STATIC_IVOL, m_ctrlStaticIVol);
	DDX_Control(pDX, IDC_STATIC_IPRICE, m_ctrlStaticIPrice);
	DDX_Control(pDX, IDC_STATIC_MONEY, m_ctrlStaticMoney);
	DDX_Control(pDX, IDC_STATIC_TAX, m_ctrlStaticTax);
	DDX_Control(pDX, IDC_STATIC_FEE, m_ctrlStaticFee);
	DDX_Control(pDX, IDC_RADIO_SELL, m_ctrlRadioSell);
	DDX_Control(pDX, IDC_RADIO_BUY, m_ctrlRadioBuy);
	DDX_Control(pDX, IDC_RADIO_STOCK, m_ctrlRadioStock);
	DDX_Control(pDX, IDC_RADIO_FUTURE, m_ctrlRadioFuture);
	DDX_Control(pDX, IDC_RADIO_OPTION, m_ctrlRadioOption);
	DDX_Control(pDX, IDC_RADIO_ELW, m_ctrlRadioElw);
	DDX_Control(pDX, IDC_FEERATE, m_ctrlFreeRate);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTradeDlg, CDialog)
	//{{AFX_MSG_MAP(CTradeDlg)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_RADIO_STOCK, OnRadioStock)
	ON_BN_CLICKED(IDC_RADIO_FUTURE, OnRadioFuture)
	ON_BN_CLICKED(IDC_RADIO_OPTION, OnRadioOption)
	ON_BN_CLICKED(IDC_RADIO_ELW, OnRadioElw)
	ON_BN_CLICKED(IDC_BTN_CALC, OnBtnCalc)
	ON_BN_CLICKED(IDC_BTN_CLEAR, OnBtnClear)
	ON_BN_CLICKED(IDC_BTN_CHANGE, OnBtnChange)
	ON_BN_CLICKED(IDC_STATIC_FSTOCK, OnStaticFstock)
	ON_BN_CLICKED(IDC_STATIC_FFUTURE, OnStaticFfuture)
	ON_BN_CLICKED(IDC_STATIC_FOPTION, OnStaticFoption)
	ON_BN_CLICKED(IDC_STATIC_FELW, OnStaticFelw)
	ON_BN_CLICKED(IDC_RADIO_BUY, OnRadioBuy)
	ON_BN_CLICKED(IDC_RADIO_SELL, OnRadioSell)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_CHILD, OnChild)
	ON_COMMAND_RANGE(IDC_CALC_BACK, IDC_CALC_ENTER, OnCalc)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTradeDlg message handlers

BOOL CTradeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	InitControls();
	
	OnChangeFee();

	m_ctrlRadioBuy.SetCheck(BST_CHECKED);
	OnRadioBuy();

	m_ctrlRadioStock.SetCheck(BST_CHECKED);
	OnRadioStock();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTradeDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

BOOL CTradeDlg::PreTranslateMessage(MSG* pMsg) 
{
	int	kind;
	switch (pMsg->message)
	{
	case WM_CHAR:
		if (kind = IsCalcChar(pMsg->wParam))
		{
			CWnd	*pWnd = GetFocus();
			int	id = pWnd->GetDlgCtrlID();
			//if (id != IDC_EDIT_MONEY && id != IDC_EDIT_PRICE)
			if (id >= IDC_CALC_BACK && id <= IDC_CALC_ENTER)
			{
				m_Calc.Calc_Key(pMsg->wParam, kind);
			}
		}
		break;
	case WM_KEYDOWN:
		if (pMsg->wParam == VK_RETURN)
		{
			CWnd	*pWnd = GetFocus();
			int	id = pWnd->GetDlgCtrlID();

			if (id >= IDC_CALC_BACK && id <= IDC_CALC_ENTER)
			{
				m_Calc.Calc_Enter();
				pMsg->wParam = 0;
			}
			else
			{
				switch(id)
				{
				case IDC_BTN_CHANGE:
				case IDC_BTN_CALC:
				case IDC_BTN_CLEAR:
					return TRUE;
				default:
					pMsg->wParam = VK_TAB;
					break;
				}
			}
		}
		else if (pMsg->wParam == VK_ESCAPE)
		{
			m_Calc.Calc_C();
			return TRUE;
		}

		break;
	case WM_KEYUP:	
		if (pMsg->wParam == VK_RETURN)
		{
			CWnd	*pWnd = GetFocus();
			int	id = pWnd->GetDlgCtrlID();
			switch(id)
			{
			case IDC_BTN_CHANGE:
				OnBtnChange();
				break;
			case IDC_BTN_CALC:
				OnBtnCalc();
				break;
			case IDC_BTN_CLEAR:
				OnBtnClear();
				break;
			}
		}		
		break;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

long CTradeDlg::OnChild(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case EDIT_FOCUS:
		GetDlgItem(IDC_CALC_0)->SetFocus();
		break;
	}

	return 0;
}

void CTradeDlg::OnCalc(UINT nID)
{
	switch (nID)
	{
	case IDC_CALC_0:	m_Calc.Calc_Char("0");	break;
	case IDC_CALC_1:	m_Calc.Calc_Char("1");	break;
	case IDC_CALC_2:	m_Calc.Calc_Char("2");	break;
	case IDC_CALC_3:	m_Calc.Calc_Char("3");	break;
	case IDC_CALC_4:	m_Calc.Calc_Char("4");	break;
	case IDC_CALC_5:	m_Calc.Calc_Char("5");	break;
	case IDC_CALC_6:	m_Calc.Calc_Char("6");	break;
	case IDC_CALC_7:	m_Calc.Calc_Char("7");	break;
	case IDC_CALC_8:	m_Calc.Calc_Char("8");	break;
	case IDC_CALC_9:	m_Calc.Calc_Char("9");	break;
	case IDC_CALC_DOT:	m_Calc.Calc_Char(".");	break;
	case IDC_CALC_SIGN:	m_Calc.Calc_Sign();	break;

	case IDC_CALC_BACK:	m_Calc.Calc_Back();	break;
	case IDC_CALC_CE:	m_Calc.Calc_CE();	break;
	case IDC_CALC_C:	m_Calc.Calc_C();	break;
	case IDC_CALC_ENTER:	m_Calc.Calc_Enter();	break;

	case IDC_CALC_DIV:	m_Calc.Calc_Oper(ck_Div);	break;
	case IDC_CALC_MUL:	m_Calc.Calc_Oper(ck_Mul);	break;
	case IDC_CALC_MINUS:	m_Calc.Calc_Oper(ck_Minus);	break;
	case IDC_CALC_PLUS:	m_Calc.Calc_Oper(ck_Plus);	break;
	}
}

void CTradeDlg::OnRadioBuy()	{	OnOdrRadio(ok_Buy);	}
void CTradeDlg::OnRadioSell()	{	OnOdrRadio(ok_Sell);	}

void CTradeDlg::OnRadioStock()	{	OnMkRadio(mk_Stock);	}
void CTradeDlg::OnRadioFuture() {	OnMkRadio(mk_Future);	}
void CTradeDlg::OnRadioOption() {	OnMkRadio(mk_Option);	}
void CTradeDlg::OnRadioElw()	{	OnMkRadio(mk_Elw);	}

void CTradeDlg::OnStaticFstock() {	SetMkRadio(mk_Stock);	OnMkRadio(mk_Stock);	}
void CTradeDlg::OnStaticFfuture() {	SetMkRadio(mk_Future);	OnMkRadio(mk_Future);	}
void CTradeDlg::OnStaticFoption() {	SetMkRadio(mk_Option);	OnMkRadio(mk_Option);	}
void CTradeDlg::OnStaticFelw()	{	SetMkRadio(mk_Elw);	OnMkRadio(mk_Elw);	}

void CTradeDlg::OnBtnCalc() 
{
	CString	str;
	double	dVol, dPrice;

	m_pApp->CorrectString(&m_ctrlInputVol);
	m_pApp->CorrectString(&m_ctrlInputPrice, m_CurMarket);

	m_ctrlInputVol.GetWindowText(str);	str.Remove(',');	dVol = fabs(atof(str));
	m_ctrlInputPrice.GetWindowText(str);	str.Remove(',');	dPrice = fabs(atof(str));

	if (dVol <= 0.0 || dPrice <= 0.0)
		return;
	
	double	dFee, dTax, dMoney;
	if (m_ctrlRadioBuy.GetCheck() == BST_CHECKED)
		dMoney = m_pApp->CalcBuyCharge(m_CurMarket, dVol, dPrice, dFee, dTax);
	else
		dMoney =  m_pApp->CalcSellCharge(m_CurMarket, dVol, dPrice, dFee, dTax);
	
	str.Format("%.f", dFee);
	str = m_pApp->editComma(atof(str));
	m_ctrlStaticFee.SetWindowText(str);

//	str.Format("%f", dTax);
	str.Format("%f", floor(dTax));
	str = m_pApp->editComma(atof(str));
	m_ctrlStaticTax.SetWindowText(str);
	//dMoney = int(dMoney/10)*10;
	str.Format("%.f", dMoney/* - dFee*/);
	str = m_pApp->editComma(atof(str));
	m_ctrlStaticMoney.SetWindowText(str);
}

void CTradeDlg::OnBtnClear() 
{
	m_ctrlInputVol.SetWindowText("");
	m_ctrlInputPrice.SetWindowText("");
	m_ctrlStaticMoney.SetWindowText("");
	m_ctrlStaticTax.SetWindowText("");
	m_ctrlStaticFee.SetWindowText("");
}

void CTradeDlg::OnBtnChange() 
{
	CFeeChangeDlg	dlg;
	if (dlg.DoModal() == IDOK)
		m_pMainDlg->ChangeFee();
}

void CTradeDlg::OnMkRadio(int kind, bool bForce)
{
	if (!bForce && m_CurMarket == kind)
		return;

	m_CurMarket = kind;
	CString	strFeeRate;
	switch (kind)
	{
	case mk_Stock:
		strFeeRate.Format("%s  %s%%+%d", SN_FeeRate, m_pApp->m_pFeeRate[kind], atoi(m_pApp->m_pCharFee[kind]));
		m_ctrlFreeRate.SetWindowText(strFeeRate);

		m_ctrlStaticIUPrice.SetWindowText("(원)");
		m_ctrlStaticIUVol.SetWindowText("(주)");
		break;
	case mk_Future:
	case mk_Option:
		strFeeRate.Format("%s  %s%%+%d", SN_FeeRate, m_pApp->m_pFeeRate[kind], atoi(m_pApp->m_pCharFee[kind]));
		m_ctrlFreeRate.SetWindowText(strFeeRate);

		m_ctrlStaticIUPrice.SetWindowText("(pt)");
		m_ctrlStaticIUVol.SetWindowText("(계약)");
		break;
	case mk_Elw:
		strFeeRate.Format("%s  %s%%+%d", SN_FeeRate, m_pApp->m_pFeeRate[kind], atoi(m_pApp->m_pCharFee[kind]));
		m_ctrlFreeRate.SetWindowText(strFeeRate);

		m_ctrlStaticIUPrice.SetWindowText("(원)");
		m_ctrlStaticIUVol.SetWindowText("(주)");
		break;
	}
}

void CTradeDlg::OnOdrRadio(int kind)
{
	switch (kind)
	{
	case ok_Buy:
		m_ctrlStaticIVol.SetWindowText("매수수량");
		m_ctrlStaticIPrice.SetWindowText("매수단가");
		break;
	case ok_Sell:
		m_ctrlStaticIVol.SetWindowText("매도수량");
		m_ctrlStaticIPrice.SetWindowText("매도단가");
		break;
	}
}

void CTradeDlg::SetMkRadio(int kind)
{
	int Radio[] = { IDC_RADIO_STOCK, IDC_RADIO_FUTURE, IDC_RADIO_OPTION, IDC_RADIO_ELW};
	for (int ii = mk_Stock; ii <= mk_Elw; ii++)
	{
		if (kind == ii)
			((CButton*)GetDlgItem(Radio[ii]))->SetCheck(BST_CHECKED);
		else
			((CButton*)GetDlgItem(Radio[ii]))->SetCheck(BST_UNCHECKED);
	}
}

void CTradeDlg::OnChangeFee()
{
	CString	str;
	int FeeStatic[] = { IDC_STATIC_FSTOCK, IDC_STATIC_FFUTURE, IDC_STATIC_FOPTION, IDC_STATIC_FELW};
	for (int ii = mk_Stock; ii <= mk_Elw; ii++)
	{
		str.Format("(%s%%)+%d", m_pApp->m_pFeeRate[ii],atoi(m_pApp->m_pCharFee[ii]));
		if(mk_Stock == 0)
		{
			GetDlgItem(FeeStatic[ii])->SetWindowText(str);
			GetDlgItem(IDC_STATIC_FSTOCK2)->SetWindowText(str);
		}
		else
		{
			GetDlgItem(FeeStatic[ii])->SetWindowText(str);	
		}
	}
	OnMkRadio(m_CurMarket, true);
}

int CTradeDlg::IsCalcChar(int key)
{
	if ((key >= '0' && key <= '9') || (key == '.'))
		return ck_Char;
	else if (key == '+')
		return ck_Plus;
	else if (key == '-')
		return ck_Minus;
	else if (key == '/')
		return ck_Div;
	else if (key == '*')
		return ck_Mul;
	return ck_None;
}

void CTradeDlg::InitControls()
{
	m_ctrlInputPrice.SetNumericPositiveFloat();
	m_ctrlInputVol.SetNumericPositiveFloat();

	m_CalcEdit.SetTextColor(RGB(0,0,0));
	m_CalcEdit.SetBackColor(RGB(255,255,255));
	m_Calc.Init(&m_CalcEdit);

	int	pColorID[] = 
	{ 
		IDC_BTN_CHANGE, IDC_BTN_CALC, IDC_BTN_CLEAR,
		IDC_CALC_BACK,	IDC_CALC_CE,
		IDC_CALC_7,	IDC_CALC_8,	IDC_CALC_9,     
		IDC_CALC_4,	IDC_CALC_5,	IDC_CALC_6,
		IDC_CALC_1,	IDC_CALC_2,	IDC_CALC_3,
		IDC_CALC_0,	IDC_CALC_DOT,	IDC_CALC_SIGN,
		IDC_CALC_DIV,	IDC_CALC_MUL,	IDC_CALC_MINUS,
		IDC_CALC_PLUS,	IDC_CALC_C,	IDC_CALC_ENTER
	};
	for (int ii = 0; ii < 23; ii++)
	{
		m_pColorButton[ii].Attach(pColorID[ii], this, RGB(123,123,123), RGB(255,255,255));
	}
}

void CTradeDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect cRc;
	GetClientRect(cRc);
	dc.FillSolidRect(cRc, clContents);
}

HBRUSH CTradeDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	switch(nCtlColor)
	{
	case 6:
		pDC->SetBkMode(TRANSPARENT);
		return CreateSolidBrush(clContents);
	break;
	}

	return hbr;
}
