// AutoSaveSetupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IB191000.h"
#include "AutoSaveSetupDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAutoSaveSetupDlg dialog


CAutoSaveSetupDlg::CAutoSaveSetupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAutoSaveSetupDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAutoSaveSetupDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_bAddCnd = FALSE;
	m_bChgCnd = FALSE;
	m_bAllApply = FALSE;
}


void CAutoSaveSetupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAutoSaveSetupDlg)
	DDX_Control(pDX, IDC_CHECK_CHG, m_cbChg);
	DDX_Control(pDX, IDC_CHECK_ALERT, m_cbAllApply);
	DDX_Control(pDX, IDC_CHECK_ADD, m_cbAdd);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAutoSaveSetupDlg, CDialog)
	//{{AFX_MSG_MAP(CAutoSaveSetupDlg)
	ON_BN_CLICKED(IDC_STATIC_CHG, OnStaticChg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAutoSaveSetupDlg message handlers

BOOL CAutoSaveSetupDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_cbAdd.SetCheck(m_bAddCnd);
	m_cbChg.SetCheck(m_bChgCnd);
	m_cbAllApply.SetCheck(m_bAllApply);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAutoSaveSetupDlg::OnOK() 
{
	m_bAddCnd = m_cbAdd.GetCheck();
	m_bChgCnd = m_cbChg.GetCheck();
	m_bAllApply = m_cbAllApply.GetCheck();
	
	CDialog::OnOK();
}

void CAutoSaveSetupDlg::OnStaticChg() 
{
	m_bChgCnd = m_cbChg.GetCheck();
	m_cbChg.SetCheck(!m_bChgCnd);
}
