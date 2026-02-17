// invalid.cpp : implementation file
//

#include "pch.h"
#include "Certify.h"
#include "invalid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInvalid dialog


CInvalid::CInvalid(CWnd* pParent /*=NULL*/)
	: CDialog(CInvalid::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInvalid)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CInvalid::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInvalid)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInvalid, CDialog)
	//{{AFX_MSG_MAP(CInvalid)
	ON_BN_CLICKED(IDC_ISSUE, OnIssue)
	ON_BN_CLICKED(IDC_OTHER, OnOther)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInvalid message handlers

BOOL CInvalid::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CInvalid::OnIssue() 
{
	EndDialog(IDC_ISSUE);	
}

void CInvalid::OnOther() 
{
	EndDialog(IDC_OTHER);	
}
