// JumunCheck.cpp : implementation file
//

#include "stdafx.h"
#include "IB300700.h"
#include "JumunCheck.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CJumunCheck dialog


CJumunCheck::CJumunCheck(CWnd* pParent /*=NULL*/)
	: CDialog(CJumunCheck::IDD, pParent)
{
	//{{AFX_DATA_INIT(CJumunCheck)
	m_msg = _T("");
	//}}AFX_DATA_INIT
}


void CJumunCheck::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CJumunCheck)
	DDX_Text(pDX, IDC_STATIC_MSG, m_msg);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CJumunCheck, CDialog)
	//{{AFX_MSG_MAP(CJumunCheck)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CJumunCheck message handlers

void CJumunCheck::OnOK() 
{
	// TODO: Add extra validation here
	if (GetAsyncKeyState(VK_RETURN)<0) return;
	CDialog::OnOK();
}

void CJumunCheck::OnCancel() 
{
	// TODO: Add extra cleanup here
	if (GetAsyncKeyState(VK_RETURN)<0) return;	
	CDialog::OnCancel();
}

HBRUSH CJumunCheck::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	UINT nID = pWnd->GetDlgCtrlID();
	if (nID == IDC_STATIC_MSG)	pDC->SetTextColor(RGB(255,0,0));
	// TODO: Return a different brush if the default is not desired
	return hbr;
}
