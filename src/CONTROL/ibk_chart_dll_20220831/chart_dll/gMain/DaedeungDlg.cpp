// DaedeungDlg.cpp : implementation file
//

#include "stdafx.h"
#include "axisgmain.h"
#include "DaedeungDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDaedeungDlg dialog


CDaedeungDlg::CDaedeungDlg(CString lgap, CString rgap)
	: CDialog(CDaedeungDlg::IDD, NULL)
{
	m_leftGap = lgap;
	m_rightGap = rgap;
}

void CDaedeungDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDaedeungDlg)
	DDX_Control(pDX, IDC_LEFTGAP, m_leftGapCtrl);
	DDX_Text(pDX, IDC_LEFTGAP, m_leftGap);
	DDX_Text(pDX, IDC_RIGHTGAP, m_rightGap);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDaedeungDlg, CDialog)
	//{{AFX_MSG_MAP(CDaedeungDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDaedeungDlg message handlers

BOOL CDaedeungDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_leftGapCtrl.SetFocus();
	m_leftGapCtrl.SetSel(0, -1);

	return FALSE;
}
