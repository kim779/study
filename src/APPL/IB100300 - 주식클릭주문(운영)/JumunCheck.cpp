// JumunCheck.cpp : implementation file
//

#include "stdafx.h"
#include "ib100300.h"
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
	ON_WM_CHAR()
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

void CJumunCheck::SetMsg(CString msg)
{
	GetDlgItem(IDC_STATIC_MSG)->SetWindowText(msg);
}

void CJumunCheck::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnChar(nChar, nRepCnt, nFlags);
}

HBRUSH CJumunCheck::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	const UINT nID = pWnd->GetDlgCtrlID();
    if (nID == IDC_STATIC_MSG)	pDC->SetTextColor(RGB(255,0,0));  //
//	if (nID == IDC_STATIC_TUJA3)	pDC->SetTextColor(RGB(255,0,0)); 
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

BOOL CJumunCheck::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	((CWnd*)GetDlgItem(IDC_STATIC_TUJA))->ShowWindow(SW_HIDE);
	((CWnd*)GetDlgItem(IDC_STATIC_TUJA2))->ShowWindow(SW_HIDE);
//	((CWnd*)GetDlgItem(IDC_STATIC_TUJA3))->ShowWindow(SW_HIDE);
	((CWnd*)GetDlgItem(IDC_STATIC_TUJA4))->ShowWindow(SW_HIDE);
	if(m_msg == "투자위험고지")
	{
		SetWindowText("투자위험고지");
		((CWnd*)GetDlgItem(IDC_STATIC_TUJA))->ShowWindow(SW_SHOW);
		((CWnd*)GetDlgItem(IDC_STATIC_TUJA2))->ShowWindow(SW_SHOW);
//		((CWnd*)GetDlgItem(IDC_STATIC_TUJA3))->ShowWindow(SW_SHOW);
		((CWnd*)GetDlgItem(IDC_STATIC_TUJA4))->ShowWindow(SW_SHOW);
		((CWnd*)GetDlgItem(IDC_STATIC_MSG))->ShowWindow(SW_HIDE);
	
		((CWnd*)GetDlgItem(IDC_STATIC_ETFETN))->ShowWindow(SW_HIDE);
		((CWnd*)GetDlgItem(IDC_STATIC_1ROW))->ShowWindow(SW_HIDE);
		((CWnd*)GetDlgItem(IDC_STATIC_2ROW))->ShowWindow(SW_HIDE);
	//	((CWnd*)GetDlgItem(IDC_STATIC_MSG))->ShowWindow(SW_HIDE);	
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
