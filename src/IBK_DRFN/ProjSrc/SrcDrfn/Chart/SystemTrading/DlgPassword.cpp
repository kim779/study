// DlgPassword.cpp : implementation file
//

#include "stdafx.h"
#include "systemtrading.h"
#include "DlgPassword.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgPassword dialog


CDlgPassword::CDlgPassword(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPassword::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgPassword)
	m_strPassword = _T("");
	//}}AFX_DATA_INIT
}


void CDlgPassword::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPassword)
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_strPassword);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPassword, CDialog)
	//{{AFX_MSG_MAP(CDlgPassword)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPassword message handlers

BOOL CDlgPassword::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	GetDlgItem(IDC_EDIT_PASSWORD)->SetFocus();
	
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
