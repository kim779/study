#include "stdafx.h"
#include "IB0000A2.h"
#include "dlgGrNew.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDlgGrNew::CDlgGrNew(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgGrNew::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgGrNew)
	m_EditGrName = _T("");
	//}}AFX_DATA_INIT
	m_titleS = _T("");
}


void CDlgGrNew::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgGrNew)
	DDX_Text(pDX, IDC_EDITGRNAME, m_EditGrName);
	DDV_MaxChars(pDX, m_EditGrName, 20);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgGrNew, CDialog)
	//{{AFX_MSG_MAP(CDlgGrNew)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CDlgGrNew::OnOK() 
{
	GetDlgItem(IDC_EDITGRNAME)->GetWindowText(m_grName);
	m_grName.TrimLeft();
	m_grName.TrimRight();
	if (m_grName.IsEmpty())
	{
		GetDlgItem(IDC_EDITGRNAME)->SetFocus();
		return;
	}
	CDialog::OnOK();
}

BOOL CDlgGrNew::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetWindowText(m_titleS);
	return TRUE; 
}
