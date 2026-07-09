// LockDlg.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "LockDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLockDlg dialog


CLockDlg::CLockDlg(CWnd* pParent, CString user, CString pass, CString certify)
	: CDialog(CLockDlg::IDD, pParent)
{
	m_pParent = pParent;
	m_user = user;
	m_pass = pass;
	m_certify = certify;
	//{{AFX_DATA_INIT(CLockDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CLockDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLockDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLockDlg, CDialog)
	//{{AFX_MSG_MAP(CLockDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLockDlg message handlers

BOOL CLockDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	/*
	if (m_certify.IsEmpty())
		GetDlgItem(IDC_STATIC_CERT)->SetWindowText("로그인 비밀번호를 입력하시면");
	else
		GetDlgItem(IDC_STATIC_CERT)->SetWindowText("공인인증비밀번호를 입력하시면");
	*/
	if (!m_user.IsEmpty())
	{
		CString tmp;
		int nLength;
		nLength = m_user.GetLength();
		tmp.Format("%s%s", m_user.Left(2), CString('*', nLength - 2));
		GetDlgItem(IDC_USER)->SetWindowText(tmp);
		NextDlgCtrl();
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLockDlg::OnOK() 
{
	CString	user, pass;
	
	GetDlgItem(IDC_USER)->GetWindowText(user);
	GetDlgItem(IDC_LPASS)->GetWindowText(pass);
	
	 if (m_pass.Compare(pass))
	 {
		 MessageBox("비밀번호가 올바르지 않습니다.", "AXIS", MB_OK);
		GotoDlgCtrl(GetDlgItem(IDC_LPASS));
	 }
	else
		CDialog::OnOK();
}

void CLockDlg::OnCancel() 
{
	return;
}

BOOL CLockDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN))
	{
		if (GetDlgItem(IDC_LPASS)->m_hWnd == pMsg->hwnd)
			OnOK();
		else if (GetDlgItem(IDC_USER)->m_hWnd == pMsg->hwnd)
		{
			pMsg->wParam = VK_TAB;
			return CDialog::PreTranslateMessage(pMsg);
		}
		::TranslateMessage(pMsg);
		::DispatchMessage(pMsg);
		return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}
