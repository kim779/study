// ProfitDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SecCalc.h"
#include "ProfitDlg.h"
#include "FeeChangeDlg.h"
#include "misc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProfitDlg dialog


CProfitDlg::CProfitDlg(class CSecCalcDlg *pDlg, CWnd* pParent /*=NULL*/)
	: CDialog(CProfitDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProfitDlg)
	//}}AFX_DATA_INIT

	m_pApp = (CSecCalcApp *)AfxGetApp();
	m_pMainDlg = pDlg;
	m_CurMarket = mk_None;
}


void CProfitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProfitDlg)
	DDX_Control(pDX, IDC_CALC_EDIT, m_CalcEdit);
	DDX_Control(pDX, IDC_EDIT_BUYPRICE, m_ctrlInputBuyPrc);
	DDX_Control(pDX, IDC_EDIT_SELLPRICE, m_ctrlInputSellPrc);
	DDX_Control(pDX, IDC_EDIT_CALCNT, m_ctrlInputSellCnt);	
	DDX_Control(pDX, IDC_STATIC_USELLCNT, m_ctrlStaticUSellCnt);
	DDX_Control(pDX, IDC_STATIC_USELLPRICE, m_ctrlStaticUSellPrc);
	DDX_Control(pDX, IDC_STATIC_UBUYPRICE, m_ctrlStaticUBuyPrc);
	DDX_Control(pDX, IDC_STATIC_PROFITRATE, m_ctrlStaticProfitRate);
	DDX_Control(pDX, IDC_STATIC_PROFIT, m_ctrlStaticProfit);
	DDX_Control(pDX, IDC_STATIC_TAX, m_ctrlStaticTax);
	DDX_Control(pDX, IDC_STATIC_FEE, m_ctrlStaticFee);
	DDX_Control(pDX, IDC_RADIO_STOCK, m_ctrlRadioStock);
	DDX_Control(pDX, IDC_RADIO_FUTURE, m_ctrlRadioFuture);
	DDX_Control(pDX, IDC_RADIO_OPTION, m_ctrlRadioOption);
	DDX_Control(pDX, IDC_RADIO_ELW, m_ctrlRadioElw);
	DDX_Control(pDX, IDC_FEERATE, m_ctrlFreeRate);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProfitDlg, CDialog)
	//{{AFX_MSG_MAP(CProfitDlg)
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
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_CHILD, OnChild)
	ON_COMMAND_RANGE(IDC_CALC_BACK, IDC_CALC_ENTER, OnCalc)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProfitDlg message handlers

BOOL CProfitDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	InitControls();
	
	OnChangeFee();

	m_ctrlRadioStock.SetCheck(BST_CHECKED);
	OnRadioStock();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CProfitDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

BOOL CProfitDlg::PreTranslateMessage(MSG* pMsg) 
{
	int	kind;
	switch (pMsg->message)
	{
	case WM_CHAR:
		if (kind = IsCalcChar(pMsg->wParam))
		{
			CWnd	*pWnd = GetFocus();
			int	id = pWnd->GetDlgCtrlID();
			//if (id != IDC_EDIT_BUYPRICE && id != IDC_EDIT_SELLPRICE && id != IDC_EDIT_CALCNT)
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

long CProfitDlg::OnChild(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case EDIT_FOCUS:
		GetDlgItem(IDC_CALC_0)->SetFocus();
		break;
	}

	return 0;
}

void CProfitDlg::OnCalc(UINT nID)
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

void CProfitDlg::OnRadioStock()	{	OnMkRadio(mk_Stock);	}
void CProfitDlg::OnRadioFuture() {	OnMkRadio(mk_Future);	}
void CProfitDlg::OnRadioOption() {	OnMkRadio(mk_Option);	}
void CProfitDlg::OnRadioElw()	{	OnMkRadio(mk_Elw);	}

void CProfitDlg::OnStaticFstock() {	SetMkRadio(mk_Stock);	OnMkRadio(mk_Stock);	}
void CProfitDlg::OnStaticFfuture() {	SetMkRadio(mk_Future);	OnMkRadio(mk_Future);	}
void CProfitDlg::OnStaticFoption() {	SetMkRadio(mk_Option);	OnMkRadio(mk_Option);	}
void CProfitDlg::OnStaticFelw()	{	SetMkRadio(mk_Elw);	OnMkRadio(mk_Elw);	}

void CProfitDlg::OnBtnCalc() 
{
	CString	str;
	double	dBuyPrice, dSellPrice, dCnt;

	m_pApp->CorrectString(&m_ctrlInputSellCnt);
	m_pApp->CorrectString(&m_ctrlInputBuyPrc, m_CurMarket);
	m_pApp->CorrectString(&m_ctrlInputSellPrc, m_CurMarket);

	m_ctrlInputBuyPrc.GetWindowText(str);	str.Remove(',');	dBuyPrice = fabs(atof(str));
	m_ctrlInputSellPrc.GetWindowText(str);	str.Remove(',');	dSellPrice = fabs(atof(str));
	m_ctrlInputSellCnt.GetWindowText(str);	str.Remove(',');	dCnt = fabs(atof(str));

	if (dBuyPrice <= 0.0 || dSellPrice <= 0.0 || dCnt <= 0.0)
		return;

	CalcProfit(dBuyPrice, dSellPrice, dCnt);
}

void CProfitDlg::OnBtnClear() 
{
	m_ctrlInputBuyPrc.SetWindowText("");
	m_ctrlInputSellPrc.SetWindowText("");
	m_ctrlInputSellCnt.SetWindowText("");
	m_ctrlStaticProfitRate.SetWindowText("");
	m_ctrlStaticProfit.SetWindowText("");
	m_ctrlStaticTax.SetWindowText("");
	m_ctrlStaticFee.SetWindowText("");
}

void CProfitDlg::OnBtnChange() 
{
	CFeeChangeDlg	dlg;
	if (dlg.DoModal() == IDOK)
		m_pMainDlg->ChangeFee();
}

void CProfitDlg::OnMkRadio(int kind, bool bForce)
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

		m_ctrlStaticUBuyPrc.SetWindowText("(원)");
		m_ctrlStaticUSellPrc.SetWindowText("(원)");
		m_ctrlStaticUSellCnt.SetWindowText("(주)");
		break;
	case mk_Future:
	case mk_Option:
		strFeeRate.Format("%s  %s%%+%d", SN_FeeRate, m_pApp->m_pFeeRate[kind], atoi(m_pApp->m_pCharFee[kind]));
		m_ctrlFreeRate.SetWindowText(strFeeRate);

		m_ctrlStaticUBuyPrc.SetWindowText("(pt)");
		m_ctrlStaticUSellPrc.SetWindowText("(pt)");
		m_ctrlStaticUSellCnt.SetWindowText("(계약)");
		break;
	case mk_Elw:
		strFeeRate.Format("%s  %s%%+%d", SN_FeeRate, m_pApp->m_pFeeRate[kind], atoi(m_pApp->m_pCharFee[kind]));
		m_ctrlFreeRate.SetWindowText(strFeeRate);

		m_ctrlStaticUBuyPrc.SetWindowText("(원)");
		m_ctrlStaticUSellPrc.SetWindowText("(원)");
		m_ctrlStaticUSellCnt.SetWindowText("(주)");
		break;
	}
}

void CProfitDlg::SetMkRadio(int kind)
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

void CProfitDlg::OnChangeFee()
{
	CString	str;
	int FeeStatic[] = { IDC_STATIC_FSTOCK, IDC_STATIC_FFUTURE, IDC_STATIC_FOPTION, IDC_STATIC_FELW};
	for (int ii = mk_Stock; ii <= mk_Elw; ii++)
	{
		str.Format("(%s%%)+%d", m_pApp->m_pFeeRate[ii], atoi(m_pApp->m_pCharFee[ii]));
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

void CProfitDlg::CalcProfit(double dBuyPrice, double dSellPrice, double dCnt)
{
	CString	str;
	double	dFee[2];
	double	dTax[2];
	double	dMoney[2];

	dMoney[0] = m_pApp->CalcBuyCharge(m_CurMarket, dCnt, dBuyPrice, dFee[0], dTax[0]);
	dMoney[1] = m_pApp->CalcSellCharge(m_CurMarket, dCnt, dSellPrice, dFee[1], dTax[1]);

	double	dProfit = dMoney[1] - dMoney[0];
	double	dProfitRate = (dProfit*100.0) / (dMoney[0]);
	//double dFees = int((dFee[0] + dFee[1]) / 10)*10;
	str.Format("%.f", dFee[0] + dFee[1]);
	
	str = m_pApp->editComma(atof(str));
	m_ctrlStaticFee.SetWindowText(str);

	str.Format("%.f", dTax[0] + dTax[1]);
	str = m_pApp->editComma(atof(str));
	m_ctrlStaticTax.SetWindowText(str);
	//dProfit = int((dProfit/10))*10;
	str.Format("%.f", dProfit);
	str = m_pApp->editComma(atof(str));
	m_ctrlStaticProfit.SetWindowText(str);

	str.Format("%.2f", dProfitRate);
	m_ctrlStaticProfitRate.SetWindowText(str);
}

int CProfitDlg::IsCalcChar(int key)
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

void CProfitDlg::InitControls()
{
	m_ctrlInputBuyPrc.SetNumericPositiveFloat();
	m_ctrlInputSellPrc.SetNumericPositiveFloat();
	m_ctrlInputSellCnt.SetNumericPositiveFloat();

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

void CProfitDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect cRc;
	GetClientRect(cRc);
	dc.FillSolidRect(cRc, clContents);
}

HBRUSH CProfitDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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
