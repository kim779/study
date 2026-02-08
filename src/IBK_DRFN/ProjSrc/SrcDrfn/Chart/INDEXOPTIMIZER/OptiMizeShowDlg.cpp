// OptiMizeShowDlg.cpp : implementation file
//

#include "stdafx.h"

#include "OptiMizeShowDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptiMizeShowDlg dialog

#define CRBACKGROUND	RGB(211,223,241)
COptiMizeShowDlg::COptiMizeShowDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COptiMizeShowDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COptiMizeShowDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void COptiMizeShowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptiMizeShowDlg)
	DDX_Control(pDX, IDC_TAB1, m_CtlTab);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptiMizeShowDlg, CDialog)
	//{{AFX_MSG_MAP(COptiMizeShowDlg)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptiMizeShowDlg message handlers

BOOL COptiMizeShowDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitTab();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void COptiMizeShowDlg::InitTab()
{
	CRect rc;
	m_CtlTab.GetClientRect(&rc);
//	m_CtlTab.GetWindowRect(&rc);
//	rc.left = 185;
//	rc.top  = 248;

	m_CtlTab.SetTopLeftCorner(CPoint(rc.left,rc.top));
	m_CtlTab.SetLeftOffset(0);
	m_CtlTab.SetUpperOffset(0);
	m_CtlTab.SetSelectedColor(RGB(0, 0, 0),CRBACKGROUND);
	m_CtlTab.SetNormalColor(RGB(0,0,0), RGB(255,255,255) , RGB(226,225,233));
	m_CtlTab.SetTabBkColor(RGB(234,235,233));
	
	m_DlgResult = new CDlgResult;
	m_DlgResult->Create(CDlgResult::IDD,this);
	m_arrayTab.Add(m_DlgResult);

	m_DlgSooik  = new CDlgSooikChart;
	m_DlgSooik->Create(CDlgSooikChart::IDD,this);
	m_arrayTab.Add(m_DlgSooik);

	m_DlgSonik  = new CDlgSoIkChart;
	m_DlgSonik->Create(CDlgSoIkChart::IDD,this);
	m_arrayTab.Add(m_DlgSonik);

	m_DlgTrade  = new CDlgTrade;
	m_DlgTrade->Create(CDlgTrade::IDD,this);
	m_arrayTab.Add(m_DlgTrade);

	m_CtlTab.AddTab(m_DlgResult,"성과분석");
	m_CtlTab.AddTab(m_DlgTrade,"매매내역");
	m_CtlTab.AddTab(m_DlgSooik,"수익률차트");
	m_CtlTab.AddTab(m_DlgSonik,"손익분포차트");
	
}

void COptiMizeShowDlg::OnButton2() 
{
	// TODO: Add your control notification handler code here

	InitTab();
}
