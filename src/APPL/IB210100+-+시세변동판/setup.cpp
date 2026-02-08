// setup.cpp : implementation file
//

#include "stdafx.h"
#include "IB210100.h"
#include "setup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Csetup dialog

Csetup::Csetup(CWnd* pParent /*=NULL*/, Cconfig* config /*= NULL*/)
	: CDialog(Csetup::IDD, pParent)
{
	//{{AFX_DATA_INIT(Csetup)
	//}}AFX_DATA_INIT

	m_config = config;
}


void Csetup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Csetup)
	DDX_Control(pDX, IDC_KRX100, m_chkKrx100);
	DDX_Control(pDX, IDC_KOSPI200, m_chkKospi200);
	DDX_Control(pDX, IDC_KOSPI, m_chkKospi);
	DDX_Control(pDX, IDC_KOSDAQ, m_chkKosdaq);
	DDX_Control(pDX, IDC_FREEBOARD, m_chkFreeboard);
	DDX_Control(pDX, IDC_ETF, m_chkEtf);
	DDX_Control(pDX, IDC_ELWBASE, m_chkElwbase);
	DDX_Control(pDX, IDC_ELW, m_chkElw);
	DDX_Control(pDX, IDC_ALL, m_chkAll);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(Csetup, CDialog)
	//{{AFX_MSG_MAP(Csetup)
	ON_BN_CLICKED(IDC_CPRC, OnPrice)
	ON_BN_CLICKED(IDC_ALL, OnAll)
	ON_BN_CLICKED(IDC_ELW, OnElw)
	ON_BN_CLICKED(IDC_ELWBASE, OnElwbase)
	ON_BN_CLICKED(IDC_ETF, OnEtf)
	ON_BN_CLICKED(IDC_FREEBOARD, OnFreeboard)
	ON_BN_CLICKED(IDC_KOSDAQ, OnKosdaq)
	ON_BN_CLICKED(IDC_KOSPI, OnKospi)
	ON_BN_CLICKED(IDC_KOSPI200, OnKospi200)
	ON_BN_CLICKED(IDC_KRX100, OnKrx100)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Csetup message handlers

BOOL Csetup::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CString	text;

	switch (m_config->m_vol)
	{
	case 1:	((CButton *)GetDlgItem(IDC_Q2))->SetCheck(1);	break;
	case 2:	((CButton *)GetDlgItem(IDC_Q3))->SetCheck(1);	break;
	case 5:	((CButton *)GetDlgItem(IDC_Q5))->SetCheck(1);	break;
	case 7:	((CButton *)GetDlgItem(IDC_Q6))->SetCheck(1);	break;
	case 10:((CButton *)GetDlgItem(IDC_Q7))->SetCheck(1);	break;
	case 30:((CButton *)GetDlgItem(IDC_Q8))->SetCheck(1);	break;
	case 50:((CButton *)GetDlgItem(IDC_Q9))->SetCheck(1);	break;
	case 3:
	default:((CButton *)GetDlgItem(IDC_Q4))->SetCheck(1);	break;
	}

	switch (m_config->m_amt)
	{
	case 1:	((CButton *)GetDlgItem(IDC_A2))->SetCheck(1);	break;
	case 2:	((CButton *)GetDlgItem(IDC_A3))->SetCheck(1);	break;
	case 5:	((CButton *)GetDlgItem(IDC_A5))->SetCheck(1);	break;
	case 7:	((CButton *)GetDlgItem(IDC_A6))->SetCheck(1);	break;
	case 10:((CButton *)GetDlgItem(IDC_A7))->SetCheck(1);	break;
	case 30:((CButton *)GetDlgItem(IDC_A8))->SetCheck(1);	break;
	case 50:((CButton *)GetDlgItem(IDC_A9))->SetCheck(1);	break;
	case 3:
	default:((CButton *)GetDlgItem(IDC_A4))->SetCheck(1);	break;
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

	if (m_config->m_color)
		((CButton *)GetDlgItem(IDC_CON))->SetCheck(1);

	if (m_config->m_field & F_CODE)
		((CButton *)GetDlgItem(IDC_F1))->SetCheck(1);
	if (m_config->m_field & F_RATE)
		((CButton *)GetDlgItem(IDC_F2))->SetCheck(1);
	if (m_config->m_field & F_VOL)
		((CButton *)GetDlgItem(IDC_F3))->SetCheck(1);
	if (m_config->m_field & F_AMT)
		((CButton *)GetDlgItem(IDC_F4))->SetCheck(1);

	//세팅(체크 설정)
	int idxChk = -1;

	for(int ii=0 ; ii<CNT_TREE ; ii++)
	{
		idxChk = atoi(m_config->m_tree[ii]);
		
		//인덱스가 체크개수보다 작을경우
		if(idxChk < CNT_CHECK)
		{
			SetProceedCheck(idxChk);
		}
	}
	
	Check_AllCheck();
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void Csetup::OnOK() 
{
	CString	text;

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

	m_config->m_color  = (((CButton *)GetDlgItem(IDC_CON))->GetCheck() == 1) ? true : false;

	m_config->m_field = 0;
	if (((CButton *)GetDlgItem(IDC_F1))->GetCheck() == 1)
		m_config->m_field |= F_CODE;
	if (((CButton *)GetDlgItem(IDC_F2))->GetCheck() == 1)
		m_config->m_field |= F_RATE;
	if (((CButton *)GetDlgItem(IDC_F3))->GetCheck() == 1)
		m_config->m_field |= F_VOL;
	if (((CButton *)GetDlgItem(IDC_F4))->GetCheck() == 1)
		m_config->m_field |= F_AMT;

//	m_config->m_tree = m_tree;
	int ii;
	for(ii = 0 ; ii< CNT_TREE ; ii++)
	{
		m_config->m_tree[ii] = "99";
	}

	ii = 0;
	if(m_chkAll.GetCheck())
	{
		m_config->m_tree[ii] = "00";
		ii++;
	}
	
	if(m_chkKospi.GetCheck())
	{
		m_config->m_tree[ii] = "01";
		ii++;
	}

	if(m_chkKosdaq.GetCheck())
	{
		m_config->m_tree[ii] = "02";
		ii++;
	}

	if(m_chkKospi200.GetCheck())
	{
		m_config->m_tree[ii] = "03";
		ii++;
	}

	if(m_chkKrx100.GetCheck())
	{
		m_config->m_tree[ii] = "04";
		ii++;
	}

	if(m_chkFreeboard.GetCheck())
	{
		m_config->m_tree[ii] = "05";
		ii++;
	}

	if(m_chkEtf.GetCheck())
	{
		m_config->m_tree[ii] = "06";
		ii++;
	}

	if(m_chkElw.GetCheck())
	{
		m_config->m_tree[ii] = "07";
		ii++;
	}

	if(m_chkElwbase.GetCheck())
	{
		m_config->m_tree[ii] = "08";
		ii++;
	}

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

void Csetup::OnAll()
{
	Check_AllCheck();
	UpdateData();
}

void Csetup::Check_AllCheck()
{
	if(m_chkAll.GetCheck() == TRUE)
	{
		m_chkKospi.SetCheck(0);
		m_chkKosdaq.SetCheck(0);
		m_chkKospi200.SetCheck(0);
		m_chkKrx100.SetCheck(0);
		m_chkFreeboard.SetCheck(0);
		m_chkEtf.SetCheck(0);
		m_chkElw.SetCheck(0);
		m_chkElwbase.SetCheck(0);
		
		m_chkKospi.EnableWindow(FALSE);
		m_chkKosdaq.EnableWindow(FALSE);
		m_chkKospi200.EnableWindow(FALSE);
		m_chkKrx100.EnableWindow(FALSE);
		m_chkFreeboard.EnableWindow(FALSE);
		m_chkEtf.EnableWindow(FALSE);
		m_chkElw.EnableWindow(FALSE);
		m_chkElwbase.EnableWindow(FALSE);
	}
	else
	{
		m_chkKospi.EnableWindow(TRUE);
		m_chkKosdaq.EnableWindow(TRUE);
		m_chkKospi200.EnableWindow(TRUE);
		m_chkKrx100.EnableWindow(TRUE);
		m_chkFreeboard.EnableWindow(TRUE);
		m_chkEtf.EnableWindow(TRUE);
		m_chkElw.EnableWindow(TRUE);
		m_chkElwbase.EnableWindow(TRUE);
	}	
}
void Csetup::GetConfig(Cconfig* config)
{
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
	config->m_color   = m_config->m_color;
	config->m_field   = m_config->m_field;
	CopyMemory(config->m_tree, m_config->m_tree, sizeof(config->m_tree));
}

void Csetup::SetProceedCheck(int index)
{
	switch(index)
	{
	case 0:
		m_chkAll.SetCheck(1);
		break;
	case 1:
		m_chkKospi.SetCheck(1);
		break;
	case 2:
		m_chkKosdaq.SetCheck(1);
		break;
	case 3:
		m_chkKospi200.SetCheck(1);
		break;
	case 4:
		m_chkKrx100.SetCheck(1);
		break;
	case 5:
		m_chkFreeboard.SetCheck(1);
		break;
	case 6:
		m_chkEtf.SetCheck(1);
		break;
	case 7:
		m_chkElw.SetCheck(1);
		break;
	case 8:
		m_chkElwbase.SetCheck(1);
		break;
	}
}
