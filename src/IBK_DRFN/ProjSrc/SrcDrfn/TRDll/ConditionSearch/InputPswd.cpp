// InputPswd.cpp : implementation file
//

#include "stdafx.h"
#include "InputPswd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInputPswd dialog


CInputPswd::CInputPswd(CWnd* pParent, CString account)
	: CDialog(CInputPswd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputPswd)
	m_strPassword = _T("");
	m_strAccount = account;
	//}}AFX_DATA_INIT
}


void CInputPswd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputPswd)
	DDX_Control(pDX, IDC_EDIT1, m_editPassword);
	DDX_Text(pDX, IDC_EDIT1, m_strPassword);
	DDX_Text(pDX, IDC_STATIC_ACCT2, m_strAccount);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInputPswd, CDialog)
	//{{AFX_MSG_MAP(CInputPswd)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputPswd message handlers

BOOL CInputPswd::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	UpdateData(TRUE);
	
	
	SetTimer(1000, 300, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CInputPswd::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void CInputPswd::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	m_editPassword.SetFocus();

	CDialog::OnTimer(nIDEvent);
}
