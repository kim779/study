// SetGapDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SetGapDlg.h"
#include "../../h/axisvar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetGapDlg dialog


CSetGapDlg::CSetGapDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetGapDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetGapDlg)
	m_nGap = 0;
	//}}AFX_DATA_INIT
}


void CSetGapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetGapDlg)
	DDX_Text(pDX, IDC_EDITGAP, m_nGap);
	DDV_MinMaxInt(pDX, m_nGap, 1, 20);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetGapDlg, CDialog)
	//{{AFX_MSG_MAP(CSetGapDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetGapDlg message handlers

BOOL CSetGapDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_nGap = AfxGetApp()->GetProfileInt(ENVIRONMENT, "gap", 5);
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
