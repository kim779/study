// ExitPass.cpp : implementation file
//

#include "pch.h"
#include "Certify.h"
#include "ExitPass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExitPass dialog


CExitPass::CExitPass(CWnd* pParent /*=NULL*/)
	: CDialog(CExitPass::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExitPass)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

}


void CExitPass::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExitPass)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExitPass, CDialog)
	//{{AFX_MSG_MAP(CExitPass)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExitPass message handlers

BOOL CExitPass::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
