// ViewChkDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IB771100.h"
#include "ViewChkDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewChkDlg dialog


CViewChkDlg::CViewChkDlg(CWnd* pParent, CWnd* pWizard)
	: CDialog(CViewChkDlg::IDD, pParent)
{
	m_pParent = pParent;
	m_pWizard = pWizard;

	//{{AFX_DATA_INIT(CViewChkDlg)
	m_bChkAutoTrigger = FALSE;
	m_bChkOnlyjongmok = FALSE;
	m_bChkInitSize = FALSE;
	//}}AFX_DATA_INIT
}


void CViewChkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewChkDlg)
	DDX_Check(pDX, IDC_CHKTRIGGER, m_bChkAutoTrigger);
	DDX_Check(pDX, IDC_CHKONLYJONGMOK, m_bChkOnlyjongmok);

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewChkDlg, CDialog)
	//{{AFX_MSG_MAP(CViewChkDlg)
	ON_BN_CLICKED(IDC_BTN_INITSIZE, OnBtnInitsize)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewChkDlg message handlers

void CViewChkDlg::OnOK() 
{
	// TODO: Add extra validation here
	SaveScreenView();

	CDialog::OnOK();
}

BOOL CViewChkDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_szRootDir = GetAxVariant(homeCC);
	m_szUser = GetAxVariant(nameCC);

	// TODO: Add extra initialization here
	loadScreenView();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

CString CViewChkDlg::GetAxVariant(int cmd, CString data)
{
	if( !m_pWizard )
		return "";
	
	const char* dta = (char*)m_pWizard->SendMessage(WM_USER, 
		MAKEWPARAM(variantDLL, cmd), (LPARAM)(LPCTSTR)data);
	
	if ((long)dta > 1)	return dta;
	else			return "";
}

void CViewChkDlg::SaveScreenView()
{
	CString sztmp, sFile;
//	int nNode = 0;
	
	UpdateData(TRUE);
	sFile.Format("%s\\User\\%s\\u_config.ini", m_szRootDir, m_szUser);
	
	sztmp.Format("%d", m_bChkAutoTrigger);
	WritePrivateProfileString("IB771100", "AUTOTRIGGER", sztmp, sFile);

	sztmp.Format("%d", m_bChkOnlyjongmok);
	WritePrivateProfileString("IB771100", "ONLYJONGMOK", sztmp, sFile);
}

void CViewChkDlg::loadScreenView()
{
	CString sztmp, sFile;
	int readL = 0;
	sFile.Format("%s\\User\\%s\\u_config.ini", m_szRootDir, m_szUser);

	readL = (int)GetPrivateProfileInt("IB771100", "AUTOTRIGGER", 0, sFile);
	m_bChkAutoTrigger = readL;

	readL = (int)GetPrivateProfileInt("IB771100", "ONLYJONGMOK", 0, sFile);
	m_bChkOnlyjongmok = readL;

	UpdateData(FALSE);
}

void CViewChkDlg::OnBtnInitsize() 
{
	// TODO: Add your control notification handler code here
	((CMapDlg*)m_pParent)->ChangeInitSize();
}
