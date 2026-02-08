// setup.cpp : implementation file
//

#include "stdafx.h"
#include "setup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Csetup dialog

Csetup::Csetup(CWnd* pParent /*=nullptr*/, Cconfig* config /*= nullptr*/)
	: CDialog(Csetup::IDD, pParent)
{
	//{{AFX_DATA_INIT(Csetup)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_config = config;
}


void Csetup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Csetup)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(Csetup, CDialog)
	//{{AFX_MSG_MAP(Csetup)
	ON_BN_CLICKED(IDC_CPRC, OnPrice)
	ON_BN_CLICKED(IDC_CHECK_APPLY, OnCheckApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void Csetup::EnableControl(BOOL bEnable)
{
	GetDlgItem(IDC_Q1)->EnableWindow(bEnable);
	GetDlgItem(IDC_Q2)->EnableWindow(bEnable);
	GetDlgItem(IDC_Q3)->EnableWindow(bEnable);
	GetDlgItem(IDC_Q4)->EnableWindow(bEnable);
	GetDlgItem(IDC_Q5)->EnableWindow(bEnable);
	GetDlgItem(IDC_Q6)->EnableWindow(bEnable);
	GetDlgItem(IDC_Q7)->EnableWindow(bEnable);
	GetDlgItem(IDC_Q8)->EnableWindow(bEnable);
	GetDlgItem(IDC_Q9)->EnableWindow(bEnable);

	GetDlgItem(IDC_A1)->EnableWindow(bEnable);
	GetDlgItem(IDC_A2)->EnableWindow(bEnable);
	GetDlgItem(IDC_A3)->EnableWindow(bEnable);
	GetDlgItem(IDC_A4)->EnableWindow(bEnable);
	GetDlgItem(IDC_A5)->EnableWindow(bEnable);
	GetDlgItem(IDC_A6)->EnableWindow(bEnable);
	GetDlgItem(IDC_A7)->EnableWindow(bEnable);
	GetDlgItem(IDC_A8)->EnableWindow(bEnable);
	GetDlgItem(IDC_A9)->EnableWindow(bEnable);

	if ((!bEnable) || (bEnable && m_config->m_price))
	{
		GetDlgItem(IDC_SPRC)->EnableWindow(bEnable);
		GetDlgItem(IDC_EPRC)->EnableWindow(bEnable);
	}

	GetDlgItem(IDC_CPRC)->EnableWindow(bEnable);

	GetDlgItem(IDC_AND)->EnableWindow(bEnable);
	GetDlgItem(IDC_OR)->EnableWindow(bEnable);

	GetDlgItem(IDC_C1)->EnableWindow(bEnable);
	GetDlgItem(IDC_C2)->EnableWindow(bEnable);
	GetDlgItem(IDC_C3)->EnableWindow(bEnable);
	GetDlgItem(IDC_C4)->EnableWindow(bEnable);
	GetDlgItem(IDC_C5)->EnableWindow(bEnable);
}

/////////////////////////////////////////////////////////////////////////////
// Csetup message handlers

BOOL Csetup::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CString	text;

	if (m_config->m_bApply)
		((CButton *)GetDlgItem(IDC_CHECK_APPLY))->SetCheck(TRUE);
	else
		((CButton *)GetDlgItem(IDC_CHECK_APPLY))->SetCheck(FALSE);


	switch (m_config->m_vol)
	{
	case 1:	((CButton *)GetDlgItem(IDC_Q2))->SetCheck(1);	break;
	case 2:	((CButton *)GetDlgItem(IDC_Q3))->SetCheck(1);	break;
	case 3: ((CButton *)GetDlgItem(IDC_Q4))->SetCheck(1);	break;
	case 5:	((CButton *)GetDlgItem(IDC_Q5))->SetCheck(1);	break;
	case 7:	((CButton *)GetDlgItem(IDC_Q6))->SetCheck(1);	break;
	case 10:((CButton *)GetDlgItem(IDC_Q7))->SetCheck(1);	break;
	case 30:((CButton *)GetDlgItem(IDC_Q8))->SetCheck(1);	break;
	case 50:((CButton *)GetDlgItem(IDC_Q9))->SetCheck(1);	break;
	case 0:	
	default:((CButton *)GetDlgItem(IDC_Q1))->SetCheck(1);	break;
	}

	switch (m_config->m_amt)
	{
	case 1:	((CButton *)GetDlgItem(IDC_A2))->SetCheck(1);	break;
	case 2:	((CButton *)GetDlgItem(IDC_A3))->SetCheck(1);	break;
	case 3: ((CButton *)GetDlgItem(IDC_A4))->SetCheck(1);	break;
	case 5:	((CButton *)GetDlgItem(IDC_A5))->SetCheck(1);	break;
	case 7:	((CButton *)GetDlgItem(IDC_A6))->SetCheck(1);	break;
	case 10:((CButton *)GetDlgItem(IDC_A7))->SetCheck(1);	break;
	case 30:((CButton *)GetDlgItem(IDC_A8))->SetCheck(1);	break;
	case 50:((CButton *)GetDlgItem(IDC_A9))->SetCheck(1);	break;
	case 0:	
	default:((CButton *)GetDlgItem(IDC_A1))->SetCheck(1);	break;
	}

	((CEdit *)GetDlgItem(IDC_SPRC))->LimitText(7);
	((CEdit *)GetDlgItem(IDC_EPRC))->LimitText(7);
	if (m_config->m_price)
	{
		((CButton *)GetDlgItem(IDC_CPRC))->SetCheck(1);
		GetDlgItem(IDC_SPRC)->EnableWindow(TRUE);
		GetDlgItem(IDC_EPRC)->EnableWindow(TRUE);
		text.Format("%d", m_config->m_sprc);
		GetDlgItem(IDC_SPRC)->SetWindowText(text);
		text.Format("%d", m_config->m_eprc);
		GetDlgItem(IDC_EPRC)->SetWindowText(text);
	}
	else	((CButton *)GetDlgItem(IDC_CPRC))->SetCheck(0);

	if (m_config->m_and)
		((CButton *)GetDlgItem(IDC_AND))->SetCheck(1);
	else
		((CButton *)GetDlgItem(IDC_OR))->SetCheck(1);

	if (m_config->m_ulimit)
		((CButton *)GetDlgItem(IDC_C1))->SetCheck(1);
	if (m_config->m_up)
		((CButton *)GetDlgItem(IDC_C2))->SetCheck(1);
	if (m_config->m_flat)
		((CButton *)GetDlgItem(IDC_C3))->SetCheck(1);
	if (m_config->m_dlimit)
		((CButton *)GetDlgItem(IDC_C4))->SetCheck(1);
	if (m_config->m_down)
		((CButton *)GetDlgItem(IDC_C5))->SetCheck(1);

	EnableControl(m_config->m_bApply);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void Csetup::OnOK() 
{
	CString	text;

	m_config->m_bApply =  (((CButton *)GetDlgItem(IDC_CHECK_APPLY))->GetCheck() == 1) ? TRUE : FALSE;

	if (((CButton *)GetDlgItem(IDC_Q2))->GetCheck() == 1)
		m_config->m_vol = 1;
	else if (((CButton *)GetDlgItem(IDC_Q3))->GetCheck() == 1)
		m_config->m_vol = 2;
	else if (((CButton *)GetDlgItem(IDC_Q4))->GetCheck() == 1)
		m_config->m_vol = 3;
	else if (((CButton *)GetDlgItem(IDC_Q5))->GetCheck() == 1)
		m_config->m_vol = 5;
	else if (((CButton *)GetDlgItem(IDC_Q6))->GetCheck() == 1)
		m_config->m_vol = 7;
	else if (((CButton *)GetDlgItem(IDC_Q7))->GetCheck() == 1)
		m_config->m_vol = 10;
	else if (((CButton *)GetDlgItem(IDC_Q8))->GetCheck() == 1)
		m_config->m_vol = 30;
	else if (((CButton *)GetDlgItem(IDC_Q9))->GetCheck() == 1)
		m_config->m_vol = 50;
	else if (((CButton *)GetDlgItem(IDC_Q1))->GetCheck() == 1)
		m_config->m_vol = 0;

	if (((CButton *)GetDlgItem(IDC_A2))->GetCheck() == 1)
		m_config->m_amt = 1;
	else if (((CButton *)GetDlgItem(IDC_A3))->GetCheck() == 1)
		m_config->m_amt = 2;
	else if (((CButton *)GetDlgItem(IDC_A4))->GetCheck() == 1)
		m_config->m_amt = 3;
	else if (((CButton *)GetDlgItem(IDC_A5))->GetCheck() == 1)
		m_config->m_amt = 5;
	else if (((CButton *)GetDlgItem(IDC_A6))->GetCheck() == 1)
		m_config->m_amt = 7;
	else if (((CButton *)GetDlgItem(IDC_A7))->GetCheck() == 1)
		m_config->m_amt = 10;
	else if (((CButton *)GetDlgItem(IDC_A8))->GetCheck() == 1)
		m_config->m_amt = 30;
	else if (((CButton *)GetDlgItem(IDC_A9))->GetCheck() == 1)
		m_config->m_amt = 50;
	else if (((CButton *)GetDlgItem(IDC_A1))->GetCheck() == 1)
		m_config->m_vol = 0;

	m_config->m_price  = (((CButton *)GetDlgItem(IDC_CPRC))->GetCheck() == 1) ? true : false;
	if (m_config->m_price)
	{
		GetDlgItem(IDC_SPRC)->GetWindowText(text);
		m_config->m_sprc = atoi(text);
		GetDlgItem(IDC_EPRC)->GetWindowText(text);
		m_config->m_eprc = atoi(text);
	}
	else
	{
		m_config->m_sprc = 0;
		m_config->m_eprc = 0;
	}

	m_config->m_and    = (((CButton *)GetDlgItem(IDC_AND))->GetCheck() == 1) ? true : false;
	
	m_config->m_ulimit = (((CButton *)GetDlgItem(IDC_C1))->GetCheck() == 1) ? true : false;
	m_config->m_up     = (((CButton *)GetDlgItem(IDC_C2))->GetCheck() == 1) ? true : false;
	m_config->m_flat   = (((CButton *)GetDlgItem(IDC_C3))->GetCheck() == 1) ? true : false;
	m_config->m_dlimit = (((CButton *)GetDlgItem(IDC_C4))->GetCheck() == 1) ? true : false;
	m_config->m_down   = (((CButton *)GetDlgItem(IDC_C5))->GetCheck() == 1) ? true : false;

	EnableControl(m_config->m_bApply);

	CDialog::OnOK();
}

void Csetup::OnPrice() 
{
	if (((CButton *)GetDlgItem(IDC_CPRC))->GetCheck() == 1)
	{
		GetDlgItem(IDC_SPRC)->EnableWindow(TRUE);
		GetDlgItem(IDC_EPRC)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_SPRC)->EnableWindow(FALSE);
		GetDlgItem(IDC_EPRC)->EnableWindow(FALSE);
		GetDlgItem(IDC_SPRC)->SetWindowText(_T(""));
		GetDlgItem(IDC_EPRC)->SetWindowText(_T(""));
	}
}

void Csetup::GetConfig(Cconfig* config)
{
	config->m_bApply  = m_config->m_bApply;
	config->m_vol     = m_config->m_vol;
	config->m_amt     = m_config->m_amt;
	config->m_price   = m_config->m_price;
	config->m_sprc    = m_config->m_sprc;
	config->m_eprc    = m_config->m_eprc;
	config->m_and     = m_config->m_and;
	config->m_ulimit  = m_config->m_ulimit;
	config->m_up      = m_config->m_up;
	config->m_flat    = m_config->m_flat;
	config->m_dlimit  = m_config->m_dlimit;
	config->m_down    = m_config->m_down;
}

void Csetup::OnCheckApply() 
{
	EnableControl( ((CButton*)GetDlgItem(IDC_CHECK_APPLY))->GetCheck() );
}
