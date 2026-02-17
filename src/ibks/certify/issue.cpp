// issue.cpp : implementation file
//

#include "pch.h"
#include "Certify.h"
#include "issue.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIssue dialog


CIssue::CIssue(CWnd* pParent /*=NULL*/)
	: CDialog(CIssue::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIssue)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CIssue::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIssue)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIssue, CDialog)
	//{{AFX_MSG_MAP(CIssue)
	ON_BN_CLICKED(IDC_ISSUE, OnIssue)
	ON_BN_CLICKED(IDC_OTHER, OnOther)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIssue message handlers

BOOL CIssue::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CIssue::OnIssue() 
{
	EndDialog(IDC_ISSUE);	
}

void CIssue::OnOther() 
{
	EndDialog(IDC_OTHER);	
}
