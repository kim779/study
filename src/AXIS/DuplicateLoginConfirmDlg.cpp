// DuplicateLoginConfirmDlg.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "DuplicateLoginConfirmDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDuplicateLoginConfirmDlg dialog


CDuplicateLoginConfirmDlg::CDuplicateLoginConfirmDlg(CWnd* pParent, const char* lpszMsg)
	: CDialog(CDuplicateLoginConfirmDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDuplicateLoginConfirmDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_msg = lpszMsg;

	const int find = m_msg.Find(' ');
	if (find > 0)
	{
		m_msg = m_msg.Right(m_msg.GetLength() - (find + 1));
		Trim(m_msg);
	}
	m_result = IDCANCEL;
}


void CDuplicateLoginConfirmDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDuplicateLoginConfirmDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDuplicateLoginConfirmDlg, CDialog)
	//{{AFX_MSG_MAP(CDuplicateLoginConfirmDlg)
	ON_BN_CLICKED(IDC_FORCELOGIN, OnForcelogin)
	ON_BN_CLICKED(IDC_SISEONLY, OnSiseonly)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDuplicateLoginConfirmDlg message handlers
BOOL CDuplicateLoginConfirmDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CString format;
	GetDlgItemText(IDC_MSG, format);
	SetDlgItemText(IDC_MSG, Format(format, m_msg));
	SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);

	return TRUE;  
}

void CDuplicateLoginConfirmDlg::OnForcelogin() 
{
	CDialog::OnOK();
	m_result = IDIGNORE;
}

void CDuplicateLoginConfirmDlg::OnSiseonly() 
{
	m_result = IDOK;
	CDialog::OnOK();
}

void CDuplicateLoginConfirmDlg::OnCancel() 
{
	m_result = IDCANCEL;
	CDialog::OnCancel();
}

UINT CDuplicateLoginConfirmDlg::GetResult()
{
	return m_result;
}



