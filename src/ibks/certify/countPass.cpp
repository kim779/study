// countPass.cpp : implementation file
//

#include "pch.h"
#include "Certify.h"
#include "countPass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCountPass dialog

CCountPass::CCountPass(int count, bool retry, CWnd* pParent /*=NULL*/)
	: CDialog(CCountPass::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCountPass)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_count = count;
	m_retry = retry;
}


void CCountPass::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCountPass)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCountPass, CDialog)
	//{{AFX_MSG_MAP(CCountPass)
	ON_BN_CLICKED(IDC_ISSUE, OnIssue)
	ON_BN_CLICKED(IDC_OTHER, OnOther)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCountPass message handlers

BOOL CCountPass::OnInitDialog() 
{
	CString	string;

	CDialog::OnInitDialog();

	string.Format(_T("%d"), m_count);
	GetDlgItem(IDC_COUNT)->SetWindowText(string);
	GetDlgItem(IDOK)->EnableWindow(m_retry ? TRUE : FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCountPass::OnIssue() 
{
	EndDialog(IDC_ISSUE);
}

void CCountPass::OnOther() 
{
	EndDialog(IDC_OTHER);
}
