// DlgTimeSelector.cpp : implementation file
//

#include "stdafx.h"
#include "DlgTimeSelector.h"
#include "StrategyConfigureWizardDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgTimeSelector dialog
#define CRBACKGROUND	RGB(211,223,241)


CDlgTimeSelector::CDlgTimeSelector(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTimeSelector::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgTimeSelector)
	m_nOption = 0;
	m_nMinute = 1;
	m_nTick = 1;
	//}}AFX_DATA_INIT
	m_crBk = CRBACKGROUND;
	m_pDlgStrategyList = NULL;
}

void CDlgTimeSelector::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgTimeSelector)
	DDX_Control(pDX, IDC_EDIT_TICK, m_editTick);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDC_EDIT_MINUTE, m_editMinute);
	DDX_Text(pDX, IDC_EDIT_MINUTE, m_nMinute);
	DDV_MinMaxInt(pDX, m_nMinute, 1, 99);
	DDX_Text(pDX, IDC_EDIT_TICK, m_nTick);
	DDV_MinMaxInt(pDX, m_nTick, 1, 99);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgTimeSelector, CDialog)
	//{{AFX_MSG_MAP(CDlgTimeSelector)
	ON_BN_CLICKED(IDC_RADIO_DAY, OnRadioDay)
	ON_BN_CLICKED(IDC_RADIO_MINUTE, OnRadioMinute)
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_RADIO_TICK, OnRadioTick)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgTimeSelector message handlers

void CDlgTimeSelector::OnRadioDay() 
{
	m_nOption = 0;
	UpdateData(FALSE);
}

void CDlgTimeSelector::OnRadioMinute() 
{
	m_nOption = 1;
	UpdateData(FALSE);
}

void CDlgTimeSelector::OnRadioTick() 
{
	m_nOption = 2;
	UpdateData(FALSE);
}


BOOL CDlgTimeSelector::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_brBkColor.CreateSolidBrush(m_crBk);
	//m_btnOK.SetSkin(IDB_BITMAP_OKNOR,IDB_BITMAP_OKSEL,IDB_BITMAP_OKNOR,IDB_BITMAP_OKDIS);

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CDlgTimeSelector::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	switch(nCtlColor) 
	{
	case CTLCOLOR_EDIT:
	case CTLCOLOR_LISTBOX:
		return hbr;
		break;
	default:
		pDC->SetBkColor(m_crBk);
		return m_brBkColor;		
		break;
	}
	
	return hbr;
}

void CDlgTimeSelector::OnDestroy() 
{
	CDialog::OnDestroy();
	
	m_brBkColor.DeleteObject();
}

void CDlgTimeSelector::OnOK() 
{
	UpdateData();
	if(m_pDlgStrategyList)
		m_pDlgStrategyList->SendMessage(UWM_SETTIME);

	CDialog::OnOK();
}

BOOL CDlgTimeSelector::UpdateData(BOOL bSaveAndValidate/*= TRUE*/)
{
	BOOL bRetValue = CDialog::UpdateData(bSaveAndValidate);
	if(bSaveAndValidate==FALSE)
	{
		if(m_nOption==1)	// minute
		{
			m_editMinute.EnableWindow();
			m_editTick.EnableWindow(FALSE);
			((CButton*)GetDlgItem(IDC_RADIO_DAY))->SetCheck(0);
			((CButton*)GetDlgItem(IDC_RADIO_MINUTE))->SetCheck(1);
			((CButton*)GetDlgItem(IDC_RADIO_TICK))->SetCheck(0);
		}
		else if(m_nOption==2)	// tick
		{
			m_editMinute.EnableWindow(FALSE);
			m_editTick.EnableWindow();
			((CButton*)GetDlgItem(IDC_RADIO_DAY))->SetCheck(0);
			((CButton*)GetDlgItem(IDC_RADIO_MINUTE))->SetCheck(0);
			((CButton*)GetDlgItem(IDC_RADIO_TICK))->SetCheck(1);
		}
		else 
		{
			m_editMinute.EnableWindow(FALSE);
			m_editTick.EnableWindow(FALSE);
			((CButton*)GetDlgItem(IDC_RADIO_DAY))->SetCheck(1);
			((CButton*)GetDlgItem(IDC_RADIO_MINUTE))->SetCheck(0);
			((CButton*)GetDlgItem(IDC_RADIO_TICK))->SetCheck(0);
		}
	}
	return bRetValue;
}
