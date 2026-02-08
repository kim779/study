// DirectInputDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IB0000A2.h"
#include "DirectInputDlg.h"

#define	WM_SUBMITACC	WM_USER + 103

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDirectInputDlg dialog


CDirectInputDlg::CDirectInputDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDirectInputDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDirectInputDlg)
	m_sAccName = _T("");
	//}}AFX_DATA_INIT
}


void CDirectInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDirectInputDlg)
	DDX_Control(pDX, IDC_ACCNUM_EDIT, m_accEdit);
	DDX_Text(pDX, IDC_ACCNAME_EDIT, m_sAccName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDirectInputDlg, CDialog)
	//{{AFX_MSG_MAP(CDirectInputDlg)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER + 109, OnAccMaxMessage)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDirectInputDlg message handlers

void CDirectInputDlg::OnOK() 
{
	m_pParentWnd->m_sInputAccNum = m_accEdit.GetAccount();
	GetDlgItem(IDC_PASSWORD_EDIT)->GetWindowText(m_pParentWnd->m_sInputPassword);
	
	CDialog::OnOK();
}

void CDirectInputDlg::InputAccName(CString sAccName)
{
	m_sAccName = sAccName;
	UpdateData(FALSE);
}

LRESULT CDirectInputDlg::OnAccMaxMessage(WPARAM, LPARAM)
{
	m_pParentWnd->SubmitAcc(this, m_accEdit.GetAccount());
	return 0;
}
