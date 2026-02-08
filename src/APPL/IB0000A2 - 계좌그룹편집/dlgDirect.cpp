#include "stdafx.h"
#include "IB0000A2.h"
#include "dlgDirect.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDlgDirect::CDlgDirect(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDirect::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgDirect)
	//}}AFX_DATA_INIT
	m_pParent = pParent;
}

void CDlgDirect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgDirect)
	DDX_Control(pDX, IDC_ACCNUM_EDIT, m_EditAccNum);
	DDX_Control(pDX, IDC_ACCNAME_EDIT, m_EditAccName);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgDirect, CDialog)
	//{{AFX_MSG_MAP(CDlgDirect)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_ACCSEARCH, OnAccNameSearch)
END_MESSAGE_MAP()

LRESULT CDlgDirect::OnAccNameSearch(WPARAM wParam, LPARAM lParam)
{
	CString accNum = m_EditAccNum.GetAccount();
	m_pParent->SendMessage(WM_ACCSEARCH, 0, (LPARAM)(LPCTSTR)accNum);
	return 0;
}

void CDlgDirect::SetAccName(CString accName)
{
	m_EditAccName.SetWindowText(accName);
}

CString CDlgDirect::GetAccNum()
{
	return m_accNum;
}

void CDlgDirect::GetAccInfo(CString &accNum, CString &accName, CString &accPass)
{
	accName = m_accName;
	accNum = m_accNum;
	accPass = m_accPass;
}

void CDlgDirect::OnOK() 
{
	CString accName, accNum, accPass;
	GetDlgItem(IDC_ACCNAME_EDIT)->GetWindowText(m_accName);
	GetDlgItem(IDC_ACCNUM_EDIT)->GetWindowText(m_accNum);
	m_accNum.Remove('-');
	GetDlgItem(IDC_PASSWORD_EDIT)->GetWindowText(m_accPass);

	CDialog::OnOK();
}
