// InputKeyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "InputKeyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInputKeyDlg dialog


CInputKeyDlg::CInputKeyDlg(CWnd* pParent /*=NULL*/, CRect voidRect/*=CRect(0)*/)
	: CDialog(CInputKeyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputKeyDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_voidRect = voidRect;
}


void CInputKeyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputKeyDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInputKeyDlg, CDialog)
	//{{AFX_MSG_MAP(CInputKeyDlg)
	ON_BN_CLICKED(IDC_NONEBTN, OnNonebtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputKeyDlg message handlers

BOOL CInputKeyDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam != 16)
		{
			m_keyVal = pMsg->wParam;
			OnOK();
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CInputKeyDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CRect	cRc, wRc;
	CPoint Pos;
	
	GetClientRect(&cRc);
	GetWindowRect(&wRc);
	int dW = wRc.Width() - cRc.Width();
	int dH = wRc.Height() - cRc.Height();
	dW += 170;
	dH += 50;

	Pos.x = m_voidRect.left;
	Pos.y = m_voidRect.bottom;
	if (dW + m_voidRect.left > GetSystemMetrics(SM_CXFULLSCREEN))
		Pos.x = GetSystemMetrics(SM_CXSCREEN) - dW;
	if (dH + m_voidRect.bottom > GetSystemMetrics(SM_CYFULLSCREEN))
		Pos.y = m_voidRect.top - dH - 20;
	SetWindowPos(NULL, Pos.x, Pos.y, dW + 10, dH + 20, NULL);	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CInputKeyDlg::OnNonebtn() 
{
	m_keyVal = 0;
	OnOK();
}
