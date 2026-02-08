// AddQueryDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wdrchartdgndlg.h"
#include "AddQueryDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAddQueryDlg dialog


CAddQueryDlg::CAddQueryDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAddQueryDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAddQueryDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_bRename = FALSE;
}

void CAddQueryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddQueryDlg)
	DDX_Control(pDX, IDC_EDIT_QUERYNAME, m_editQryName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAddQueryDlg, CDialog)
	//{{AFX_MSG_MAP(CAddQueryDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAddQueryDlg message handlers

void CAddQueryDlg::OnOK() 
{
	m_editQryName.GetWindowText(m_strQueryName);

	CDialog::OnOK();
}

BOOL CAddQueryDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if (m_bRename)	SetWindowText(_T("Modify Query Name"));
	else			SetWindowText(_T("Add Query"));

	m_editQryName.SetWindowText(m_strQueryName);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
