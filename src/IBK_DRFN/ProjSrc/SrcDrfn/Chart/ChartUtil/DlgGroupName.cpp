// DlgGroupName.cpp : implementation file
//

#include "stdafx.h"
#include "ChartUtil.h"
#include "DlgGroupName.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgGroupName dialog


CDlgGroupName::CDlgGroupName(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgGroupName::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgGroupName)
	m_szName = _T("");
	//}}AFX_DATA_INIT
}


void CDlgGroupName::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgGroupName)
	DDX_Text(pDX, IDC_EDIT1, m_szName);
	DDV_MaxChars(pDX, m_szName, 20);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgGroupName, CDialog)
	//{{AFX_MSG_MAP(CDlgGroupName)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgGroupName message handlers

BOOL CDlgGroupName::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	GetDlgItem(IDC_EDIT1)->SetFocus();

	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}