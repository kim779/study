// LineThicknessSelDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LineThicknessSelDlg.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLineThicknessSelDlg dialog

#define NUM_OF_LINE		5

CLineThicknessSelDlg::CLineThicknessSelDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLineThicknessSelDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLineThicknessSelDlg)
	//}}AFX_DATA_INIT
}


void CLineThicknessSelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLineThicknessSelDlg)
	DDX_Control(pDX, IDC_LIST_LINE, m_lineListbox);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLineThicknessSelDlg, CDialog)
	//{{AFX_MSG_MAP(CLineThicknessSelDlg)
	ON_WM_KILLFOCUS()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLineThicknessSelDlg message handlers


BOOL CLineThicknessSelDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	for(int i = 0; i < NUM_OF_LINE; i++)
		m_lineListbox.SetItemData(m_lineListbox.InsertString(i, ""), i + 1);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLineThicknessSelDlg::OnKillFocus(CWnd* pNewWnd) 
{
	CDialog::OnKillFocus(pNewWnd);
	
	// TODO: Add your message handler code here
	
}

void CLineThicknessSelDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if(m_lineListbox.GetSafeHwnd())
		m_lineListbox.MoveWindow(0, 0, cx, cy);
}

void CLineThicknessSelDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	m_lineListbox.SetCurSel(0);
}
