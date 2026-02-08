// DlgHelp.cpp : implementation file
//

#include "stdafx.h"
#include "DlgHelp.h"
#include "MainFrmWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DlgHelp dialog


DlgHelp::DlgHelp(CWnd* pParent /*=NULL*/, CString strHelpText)
	: CDialog(DlgHelp::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlgHelp)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pWndMainFrm = pParent;
	m_strHelpText = "\r\n" + strHelpText + "\r\n";
}

void DlgHelp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgHelp)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_EDIT1, m_ctrlHelpText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlgHelp, CDialog)
	//{{AFX_MSG_MAP(DlgHelp)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnBtnOk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgHelp message handlers

BOOL DlgHelp::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CConditionMakeWnd* pMakeWnd = (CConditionMakeWnd*)m_pWndMainFrm;
	CMainFrmWnd* pMainWnd = (CMainFrmWnd*)pMakeWnd->m_pwndMain;
	m_clrMainSkin = pMainWnd->GetAxColor(66);

	m_btnOk.Create(_T("´Ý±â"), CRect(0, 0, 0, 0), this, IDOK);
	pMakeWnd->SetBtnImg(&m_btnOk, 4);

	m_ctrlHelpText.SetWindowText(m_strHelpText);

	OnSize();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void DlgHelp::OnBtnOk()
{
	OnOK();
}

void DlgHelp::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect reClient;
	GetClientRect(reClient);
	dc.FillSolidRect(reClient, m_clrMainSkin);
}

void DlgHelp::OnSize() 
{
	CRect reClient, reLeft;
	GetClientRect(reClient);
	
	reLeft = reClient;
	
	reLeft.top = reClient.bottom - 25;
	reLeft.bottom = reLeft.top + 20;
	
	reLeft.right = reClient.right - 5;
	reLeft.left = reLeft.right - 60;
	m_btnOk.MoveWindow(&reLeft);
	m_btnOk.ShowWindow(SW_HIDE);
	
	Invalidate();
}
