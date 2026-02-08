// CertErrDialog.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "CertErrDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCertErrDialog dialog


CCertErrDialog::CCertErrDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CCertErrDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCertErrDialog)
	m_strCnt = _T("");
	//}}AFX_DATA_INIT
}


void CCertErrDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCertErrDialog)
	DDX_Text(pDX, IDC_CERTCNT, m_strCnt);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCertErrDialog, CDialog)
	//{{AFX_MSG_MAP(CCertErrDialog)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCertErrDialog message handlers

void CCertErrDialog::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void CCertErrDialog::SetErrCount( CString cnt )
{
	int nCnt = atoi(cnt);

	nCnt = 5 - nCnt;

	CString tmp;
	tmp.Format("%d",nCnt);

	m_strCnt = tmp;
}

void CCertErrDialog::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnTimer(nIDEvent);
}
