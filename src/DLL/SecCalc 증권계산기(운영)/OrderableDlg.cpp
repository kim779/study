// OrderableDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SecCalc.h"
#include "OrderableDlg.h"
#include "FeeChangeDlg.h"
#include "misc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COrderableDlg dialog


COrderableDlg::COrderableDlg(class CSecCalcDlg *pDlg, CWnd* pParent /*=NULL*/)
	: CDialog(COrderableDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COrderableDlg)
	//}}AFX_DATA_INIT

	m_pApp = (CSecCalcApp *)AfxGetApp();
	m_pMainDlg = pDlg;
	m_CurMarket = mk_None;
}


void COrderableDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COrderableDlg)
	DDX_Control(pDX, IDC_RADIO_STOCK2, m_ctrlRadioStock2);
	DDX_Control(pDX, IDC_CALC_EDIT, m_CalcEdit);
	DDX_Control(pDX, IDC_CHECK_SINGLE, m_ctrlCheckSingle);
	DDX_Control(pDX, IDC_STATIC_UORDERABLE, m_ctrlStaticCountUnit);
	DDX_Control(pDX, IDC_STATIC_UPRICE, m_ctrlStaticPriceUnit);
	DDX_Control(pDX, IDC_STATIC_ORDERABLE, m_ctrlStaticOrderable);
	DDX_Control(pDX, IDC_STATIC_MONEY, m_ctrlStaticMoney);
	DDX_Control(pDX, IDC_STATIC_FEE, m_ctrlStaticFee);
	DDX_Control(pDX, IDC_STATIC_BALANCE, m_ctrlStaticBalance);
	DDX_Control(pDX, IDC_RADIO_STOCK, m_ctrlRadioStock);
	DDX_Control(pDX, IDC_RADIO_FUTURE, m_ctrlRadioFuture);
	DDX_Control(pDX, IDC_RADIO_OPTION, m_ctrlRadioOption);
	DDX_Control(pDX, IDC_RADIO_ELW, m_ctrlRadioElw);
	DDX_Control(pDX, IDC_FEERATE, m_ctrlFreeRate);
	DDX_Control(pDX, IDC_EDIT_PRICE, m_ctrlInputPrice);
	DDX_Control(pDX, IDC_EDIT_MONEY, m_ctrlInputMoney);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COrderableDlg, CDialog)
	//{{AFX_MSG_MAP(COrderableDlg)
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
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_RADIO_STOCK2, OnRadioStock2)
	ON_BN_CLICKED(IDC_STATIC_FSTOCK2, OnStaticFstock2)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_CHILD, OnChild)
	ON_COMMAND_RANGE(IDC_CALC_BACK, IDC_CALC_ENTER, OnCalc)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COrderableDlg message handlers

BOOL COrderableDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	InitControls();
	
	((CComboBox*)GetDlgItem(IDC_COMBO_MARGIN))->SetCurSel(0);

	OnChangeFee();
	m_ctrlRadioStock.SetCheck(BST_CHECKED);
	OnRadioStock();

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_MARGIN);
	pCombo->Clear();
	pCombo->InsertString(0,"100%");
	pCombo->InsertString(1,"50%");
	pCombo->InsertString(2,"40%");

	pCombo->SetCurSel(0);

	SetTimer(1001, 100, NULL);	// 초기 포커스를 탭에서 에디터로 하기위함
	m_ctrlCheckSingle.SetCheck(true);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COrderableDlg::OnDestroy() 
{
	CDialog::OnDestroy();
}

BOOL COrderableDlg::PreTranslateMessage(MSG* pMsg) 
{
	int	kind;
	switch (pMsg->message)
	{
	case WM_CHAR:
		if (kind = IsCalcChar(pMsg->wParam))
		{
			CWnd	*pWnd = GetFocus();
			int	id = pWnd->GetDlgCtrlID();
			// (id != IDC_EDIT_MONEY && id != IDC_EDIT_PRICE)
			if (id >= IDC_CALC_BACK && id <= IDC_CALC_ENTER){
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

long COrderableDlg::OnChild(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case EDIT_FOCUS:
		GetDlgItem(IDC_CALC_0)->SetFocus();
		break;
	}

	return 0;
}

void COrderableDlg::OnCalc(UINT nID)
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

void COrderableDlg::OnRadioStock() {	GetDlgItem(IDC_COMBO_MARGIN)->EnableWindow(FALSE); OnMkRadio(mk_Stock);	}
void COrderableDlg::OnRadioFuture() {	GetDlgItem(IDC_COMBO_MARGIN)->EnableWindow(FALSE); OnMkRadio(mk_Future);	}
void COrderableDlg::OnRadioOption() {	GetDlgItem(IDC_COMBO_MARGIN)->EnableWindow(FALSE); OnMkRadio(mk_Option);	}
void COrderableDlg::OnRadioElw() {	OnMkRadio(mk_Elw);	}

void COrderableDlg::OnStaticFstock() {	SetMkRadio(mk_Stock);	OnMkRadio(mk_Stock);	}
void COrderableDlg::OnStaticFfuture() {	SetMkRadio(mk_Future);	OnMkRadio(mk_Future);	}
void COrderableDlg::OnStaticFoption() {	SetMkRadio(mk_Option);	OnMkRadio(mk_Option);	}
void COrderableDlg::OnStaticFelw() {	SetMkRadio(mk_Elw);	OnMkRadio(mk_Elw);	}

void COrderableDlg::OnBtnCalc() 
{
	CString	str;
	double	dMoney, dPrice;

	m_pApp->CorrectString(&m_ctrlInputMoney);
	m_pApp->CorrectString(&m_ctrlInputPrice, m_CurMarket);

	m_ctrlInputMoney.GetWindowText(str);	str.Remove(',');	dMoney = fabs(atof(str));
	m_ctrlInputPrice.GetWindowText(str);	str.Remove(',');	dPrice = fabs(atof(str));

	if (dMoney <= 0.0 || dPrice <= 0.0 || dMoney < dPrice)	
		return;

	CalcOrderable(dMoney, dPrice);
}

void COrderableDlg::OnBtnClear() 
{
	m_ctrlInputMoney.SetWindowText("");
	m_ctrlInputPrice.SetWindowText("");
	m_ctrlStaticOrderable.SetWindowText("");
	m_ctrlStaticMoney.SetWindowText("");
	m_ctrlStaticFee.SetWindowText("");
	m_ctrlStaticBalance.SetWindowText("");
}

void COrderableDlg::OnBtnChange() 
{
	CFeeChangeDlg	dlg;
	if (dlg.DoModal() == IDOK)
		m_pMainDlg->ChangeFee();
}

void COrderableDlg::OnMkRadio(int kind, bool bForce)
{
	if (!bForce && m_CurMarket == kind)
		return;

	CString	strFeeRate;
	switch (kind)
	{
	case mk_Stock:
	case mk_Stock2:
		if(kind == mk_Stock)
			m_CurMarket = mk_Stock;
		else
			m_CurMarket = mk_Stock2;

		strFeeRate.Format("%s  %s%%+%d", SN_FeeRate, m_pApp->m_pFeeRate[mk_Stock], atoi(m_pApp->m_pCharFee[mk_Stock]));
		m_ctrlFreeRate.SetWindowText(strFeeRate);
	//	m_ctrlCheckSingle.ShowWindow(SW_SHOW);

		m_ctrlStaticPriceUnit.SetWindowText("(원)");
		m_ctrlStaticCountUnit.SetWindowText("(주)");
		break;
	case mk_Future:
	case mk_Option:
		m_CurMarket = kind;
		strFeeRate.Format("%s  %s%%+%d", SN_FeeRate, m_pApp->m_pFeeRate[kind], atoi(m_pApp->m_pCharFee[kind]));
		m_ctrlFreeRate.SetWindowText(strFeeRate);
		m_ctrlCheckSingle.ShowWindow(SW_HIDE);

		m_ctrlStaticPriceUnit.SetWindowText("(pt)");
		m_ctrlStaticCountUnit.SetWindowText("(계약)");
		break;
	case mk_Elw:
		m_CurMarket = kind;
		strFeeRate.Format("%s  %s%%+%d", SN_FeeRate, m_pApp->m_pFeeRate[kind], atoi(m_pApp->m_pCharFee[kind]));
		m_ctrlFreeRate.SetWindowText(strFeeRate);
		m_ctrlCheckSingle.ShowWindow(SW_HIDE);

		m_ctrlStaticPriceUnit.ShowWindow(SW_SHOW);
		m_ctrlStaticPriceUnit.SetWindowText("(원)");
		m_ctrlStaticCountUnit.SetWindowText("(주)");
		break;
	}
}

void COrderableDlg::SetMkRadio(int kind)
{
	int Radio[] = { IDC_RADIO_STOCK, IDC_RADIO_FUTURE, IDC_RADIO_OPTION, IDC_RADIO_ELW};
	if(kind != mk_Stock2)
	{
		for (int ii = mk_Stock; ii <= mk_Elw; ii++)
		{
			if (kind == ii)
				((CButton*)GetDlgItem(Radio[ii]))->SetCheck(BST_CHECKED);
			else
				((CButton*)GetDlgItem(Radio[ii]))->SetCheck(BST_UNCHECKED);
		}

		((CButton*)GetDlgItem(IDC_RADIO_STOCK2))->SetCheck(BST_UNCHECKED);
		GetDlgItem(IDC_COMBO_MARGIN)->EnableWindow(FALSE);
	}
	else
	{	
		((CButton*)GetDlgItem(IDC_RADIO_STOCK2))->SetCheck(BST_CHECKED);
		GetDlgItem(IDC_COMBO_MARGIN)->EnableWindow(TRUE);

		for (int ii = mk_Stock; ii <= mk_Elw; ii++)
		{
			((CButton*)GetDlgItem(Radio[ii]))->SetCheck(BST_UNCHECKED);
		}
	}
}

void COrderableDlg::OnChangeFee()
{
	CString	str;
	int FeeStatic[] = { IDC_STATIC_FSTOCK, IDC_STATIC_FFUTURE, IDC_STATIC_FOPTION, IDC_STATIC_FELW};
	for (int ii = mk_Stock; ii <= mk_Elw; ii++)
	{
		str.Format("(%s%%)+%d", m_pApp->m_pFeeRate[ii], atoi(m_pApp->m_pCharFee[ii]));
	
		if(ii == 0)
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

void COrderableDlg::CalcOrderable(double dMoney, double dPrice)
{
	CString	str;
	int	iUnit = 10;
	bool	bSingle = false;
	if ((m_CurMarket != mk_Stock && m_CurMarket != mk_Stock2) || 
//		(m_ctrlCheckSingle.IsWindowVisible() && m_ctrlCheckSingle.GetCheck() == BST_CHECKED))	// 단주까지 계산
		( m_ctrlCheckSingle.GetCheck() == BST_CHECKED))	
	{
		bSingle = true;
		iUnit = 1;
	}
	if (m_CurMarket == mk_Future)
		dPrice *= 500000;
	else if (m_CurMarket == mk_Option)
		dPrice *= 100000;
	else if (m_CurMarket == mk_Stock2)
	{
		CString strMargin;

		CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_MARGIN);

		if(pCombo->GetCurSel() == 2)
		{
			dMoney = dMoney * 2.5;
		}
		else if(pCombo->GetCurSel() == 1)
		{
			dMoney = dMoney * 2;
		}
	}
	
	CString strFeeRate;
	
	if(m_CurMarket == mk_Stock2)
	{
		strFeeRate = m_pApp->m_pFeeRate[mk_Stock];
	}
	else
	{
		strFeeRate = m_pApp->m_pFeeRate[m_CurMarket];
	}

	double	dFeeRate = atof(strFeeRate)*0.01;
	double	dMok = dMoney / dPrice;
	str.Format("%.f", dMok);
	int	iMok = atoi(str);

	iMok = (iMok/iUnit)*iUnit;
	double	dUsed = iMok*dPrice;
	double	dFee = dUsed * dFeeRate;
	while (dFee + dUsed > dMoney)
	{
		iMok -= iUnit;
		dUsed = iMok*dPrice;
		dFee = dUsed * dFeeRate;
	}
	dFee = int(dFee / 10)*10+atoi(strFeeRate);
	str.Format("%.f", dFee);
	str = m_pApp->editComma(atof(str));
	m_ctrlStaticFee.SetWindowText(str);
	str.Format("%d", iMok);
	str = m_pApp->editComma(atof(str));
	m_ctrlStaticOrderable.SetWindowText(str);
	str.Format("%.f", dUsed + dFee);
	str = m_pApp->editComma(atof(str));
	m_ctrlStaticMoney.SetWindowText(str);
	//double dBal = int((dMoney - dUsed - dFee)/10)*10;
	str.Format("%.f",dMoney - dUsed - dFee);
	str = m_pApp->editComma(atof(str));
	m_ctrlStaticBalance.SetWindowText(str);
}

void COrderableDlg::OnTimer(UINT nIDEvent) 
{
	KillTimer(nIDEvent);
	m_ctrlInputMoney.SetFocus();

	CDialog::OnTimer(nIDEvent);
}

int COrderableDlg::IsCalcChar(int key)
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

void COrderableDlg::InitControls()
{
	m_ctrlInputPrice.SetNumericPositiveFloat();
	m_ctrlInputMoney.SetNumericPositiveFloat();

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

void COrderableDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect cRc;
	GetClientRect(cRc);
	dc.FillSolidRect(cRc, clContents);
}

HBRUSH COrderableDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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

void COrderableDlg::OnRadioStock2() 
{
	GetDlgItem(IDC_COMBO_MARGIN)->EnableWindow(TRUE);

	OnMkRadio(mk_Stock2);
}

void COrderableDlg::OnStaticFstock2() 
{
	SetMkRadio(mk_Stock2);	
	OnMkRadio(mk_Stock2);
}
