// PassInput.cpp : implementation file
//

#include "pch.h"
#include "Certify.h"
#include "PassInput.h"
#include "ExitPass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	TM_MSG	10000
#define szPASS	8	// minimum size of passwd

/////////////////////////////////////////////////////////////////////////////
// CPassInput dialog


CPassInput::CPassInput(CWnd* pParent /*=NULL*/)
	: CDialog(CPassInput::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPassInput)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_parent = pParent;
	m_msg.Empty();
	m_pass.Empty();
}


void CPassInput::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPassInput)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPassInput, CDialog)
	//{{AFX_MSG_MAP(CPassInput)
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	ON_EN_CHANGE(IDC_PASS, OnChangePass)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPassInput message handlers

void CPassInput::SetMessage(CString msg)
{
	m_msg = msg;
	SetTimer(TM_MSG, 200, NULL);
}

void CPassInput::OnOK() 
{
	CString	text;

	GetDlgItem(IDC_PASS)->GetWindowText(text);
	if (m_parent->SendMessage(WM_USER, MAKEWPARAM(checkPass, 0), (LPARAM)(char *)text.operator LPCTSTR()))
	{
		m_pass = text;
		CDialog::OnOK();
		return;
	}
	GetDlgItem(IDOK)->EnableWindow(FALSE);
}

void CPassInput::OnTimer(UINT nIDEvent)
{
	CDialog::OnTimer(nIDEvent);
	KillTimer(nIDEvent);

	struct	_pwdR* pwdR;
	CString	string;

	GetDlgItem(IDOK)->EnableWindow(TRUE);
	pwdR  = (struct _pwdR *)(char *)m_msg.operator LPCTSTR();
	switch (pwdR->ret[0])
	{
	case 'X':
		{
			CExitPass exitpass;
			exitpass.DoModal();
			EndDialog(IDCANCEL);

			m_parent->SendMessage(WM_USER, MAKEWPARAM(checkPass, 1));
			m_parent->SendMessage(WM_USER, MAKEWPARAM(rebootAxis, true));
		}
		return;
	case '0':
		string.Format(_T("전자서명 비밀번호 %c회 오류입니다"), pwdR->pwdn[0]);
		break;
	default:
		string = CString(pwdR->msg, sizeof(pwdR->msg)); string.Trim();
		break;
	}
	MessageBox(string, _T("IBK투자증권"), MB_ICONINFORMATION|MB_OK);
}

HBRUSH CPassInput::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	if (nCtlColor != CTLCOLOR_STATIC)
		return hbr;

	switch (pWnd->GetDlgCtrlID())
	{
	case IDC_STMSG:
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(255, 0, 0));
		break;
	}
	return hbr;
}

void CPassInput::OnChangePass() 
{
	CString pass;
	GetDlgItem(IDC_PASS)->GetWindowText(pass);

	GetDlgItem(IDOK)->EnableWindow(pass.GetLength() < szPASS ? FALSE : TRUE);
}
