// overPass.cpp : implementation file
//

#include "pch.h"
#include "Certify.h"
#include "overPass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COverPass dialog


COverPass::COverPass(CWnd* pParent /*=NULL*/)
	: CDialog(COverPass::IDD, pParent)
{
	//{{AFX_DATA_INIT(COverPass)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void COverPass::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COverPass)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COverPass, CDialog)
	//{{AFX_MSG_MAP(COverPass)
	ON_BN_CLICKED(IDC_ISSUE, OnIssue)
	ON_BN_CLICKED(IDC_OTHER, OnOther)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COverPass message handlers

BOOL COverPass::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COverPass::OnIssue() 
{
	EndDialog(IDC_ISSUE);	
}

void COverPass::OnOther() 
{
	EndDialog(IDC_OTHER);	
}
