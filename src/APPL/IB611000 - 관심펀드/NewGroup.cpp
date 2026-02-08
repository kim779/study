// NewGroup.cpp : implementation file
//

#include "stdafx.h"
#include "IB611000.h"
#include "NewGroup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewGroup dialog


CNewGroup::CNewGroup(CWnd* pParent, int dlgMode, CString defName)
	: CDialog(CNewGroup::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewGroup)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_dlgMode = dlgMode;
	m_defName = defName;
}


void CNewGroup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewGroup)
	DDX_Control(pDX, IDC_GROUPNAME, m_grpname);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewGroup, CDialog)
	//{{AFX_MSG_MAP(CNewGroup)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewGroup message handlers

BOOL CNewGroup::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_title.LoadString(IDD_NEWGROUP);
	if (m_dlgMode == 1) m_title.LoadString(IDD_NEWGROUP);
	SetWindowText(m_title);
	
	m_grpname.SetWindowText(m_defName);
	m_grpname.SetLimitText(18);
	m_grpname.SetSel(0, -1);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

}

void CNewGroup::OnOK() 
{
	CString	string; m_grpname.GetWindowText(string);
	string.TrimLeft(); string.TrimRight();
	
	if (string.GetLength() <= 0)
	{
		string.LoadString(IDS_ENOGROUP);
		MessageBox(string, m_title, MB_OK|MB_ICONEXCLAMATION);
		return;
	}
	
	
	m_defName = string; CDialog::OnOK();
}

void CNewGroup::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}
