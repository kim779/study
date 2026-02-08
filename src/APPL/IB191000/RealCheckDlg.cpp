// RealCheckDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IB191000.h"
#include "RealCheckDlg.h"

#include "GridWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRealCheckDlg dialog


CRealCheckDlg::CRealCheckDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRealCheckDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRealCheckDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pGridWnd = NULL;
}


void CRealCheckDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRealCheckDlg)
	DDX_Control(pDX, IDC_DATA, m_edData);
	DDX_Control(pDX, IDC_SYMBOL, m_edSymbol);
	DDX_Control(pDX, IDC_LIST1, m_listGrid);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_stcTitle);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRealCheckDlg, CDialog)
	//{{AFX_MSG_MAP(CRealCheckDlg)
	ON_BN_CLICKED(IDC_CLOSE, OnClose)
	ON_BN_CLICKED(IDC_START, OnStart)
	ON_BN_CLICKED(IDC_STOP, OnStop)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRealCheckDlg message handlers

void CRealCheckDlg::OnClose() 
{
	ShowWindow(SW_HIDE);
	m_pGridWnd->m_bMonitor = FALSE;
}

void CRealCheckDlg::OnStart() 
{
	m_edData.SetWindowText("Monitoring Start!!");

	m_pGridWnd->m_bMonitor = TRUE;

	m_strSymbol = "";
	m_edSymbol.GetWindowText(m_strSymbol);

	if (!m_strSymbol.IsEmpty())
	{
		m_pGridWnd->m_bMonSym = TRUE;
	}
}

void CRealCheckDlg::OnStop() 
{
	m_pGridWnd->m_bMonitor = FALSE;
	m_pGridWnd->m_bMonSym  = FALSE;

	CString strTemp;
	m_edData.GetWindowText(strTemp);
	strTemp += "\r\nMonitoring Stop!!";

	m_edData.SetWindowText(strTemp);
}

void CRealCheckDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
/*
	if (m_edData.GetSafeHwnd())
	{
		int nX = cx - m_rcWnd.Width();
		int nY = cy - m_rcWnd.Height();
		
		CRect rcData;
		m_edData.GetWindowRect(rcData);

		rcData.right  += nX;
		rcData.bottom += nY;

		m_edData.SetWindowPos(NULL, 0, 0, rcData.Width(), rcData.Height(), SWP_NOMOVE);

		rcData;
		m_listGrid.GetWindowRect(rcData);

		rcData.bottom += nY;

		m_listGrid.SetWindowPos(NULL, 0, 0, rcData.Width(), rcData.Height(), SWP_NOMOVE);

		m_rcWnd.right  = cx;
		m_rcWnd.bottom = cy;
	}
*/
}

BOOL CRealCheckDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	GetWindowRect(m_rcWnd);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
