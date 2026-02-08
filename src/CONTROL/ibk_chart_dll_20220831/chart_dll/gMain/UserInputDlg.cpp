// UserInputDlg.cpp : implementation file
//

#include "stdafx.h"
#include "axisgmain.h"
#include "UserInputDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUserInputDlg dialog

CUserInputDlg::CUserInputDlg(CString str, CString title)
	: CDialog(CUserInputDlg::IDD, NULL)
{
	m_Input = str;
	m_title = title;
}

void CUserInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUserInputDlg)
	DDX_Text(pDX, IDC_INPUT, m_Input);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUserInputDlg, CDialog)
	//{{AFX_MSG_MAP(CUserInputDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUserInputDlg message handlers

BOOL CUserInputDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	UpdateData(TRUE);
	if (!m_title.IsEmpty())
		SetWindowText(m_title.operator LPCTSTR());
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CUserInputDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void CUserInputDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}
