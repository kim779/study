// ConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "c_volline1.h"
#include "ConfigDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfigDlg dialog


CConfigDlg::CConfigDlg(struct _tickinfo *tickinfo, CWnd* pParent /*=NULL*/)
	: CDialog(CConfigDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConfigDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_tickinfo = tickinfo;
}


void CConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigDlg)
	DDX_Control(pDX, IDC_REQUEST, m_Request);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConfigDlg, CDialog)
	//{{AFX_MSG_MAP(CConfigDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfigDlg message handlers

BOOL CConfigDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CString	dispStr;
	dispStr.Format("%d", atoi(m_tickinfo->request));
	m_Request.SetWindowText(dispStr);

	m_Request.LimitText(5);
	m_Request.SetFocus();	
	m_Request.SetSel(0, -1);
	
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CConfigDlg::OnOK() 
{
	CString	dispStr;
	m_Request.GetWindowText(dispStr);

	if (atoi(dispStr) > 0)
		sprintf(m_tickinfo->request, "%d", atoi(dispStr));
	
	CDialog::OnOK();
}
