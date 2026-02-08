// DlgConfirm.cpp : implementation file
//

#include "stdafx.h"
#include "chartperiodsummaryaddin.h"
#include "DlgConfirm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgConfirm dialog


CDlgConfirm::CDlgConfirm(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgConfirm::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgConfirm)
		// NOTE: the ClassWizard will add member initialization here
	m_strName = "";
	m_nUpDown = 0;
	m_nSellBuy = 0;
	//}}AFX_DATA_INIT
}


void CDlgConfirm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgConfirm)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgConfirm, CDialog)
	//{{AFX_MSG_MAP(CDlgConfirm)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgConfirm message handlers

BOOL CDlgConfirm::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CString strDebug;
	strDebug.Format("#######OnInitDialog::%s###\r\n", m_strName);
	OutputDebugString(strDebug);
	// TODO: Add extra initialization here
	if(m_nUpDown == 1)
		GetDlgItem(IDC_STATIC_TYPE)->SetWindowText("상승");
	else if(m_nUpDown == 2)
		GetDlgItem(IDC_STATIC_TYPE)->SetWindowText("하락");

	GetDlgItem(IDC_STATIC_NAME)->SetWindowText(m_strName);

	if(m_nSellBuy == 1)
		GetDlgItem(IDC_STATIC_PRICE)->SetWindowText("매도");
	else if(m_nSellBuy == 2)
		GetDlgItem(IDC_STATIC_PRICE)->SetWindowText("매수");
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgConfirm::SetData(CString strName, int nUpDown, int nSellBuy)
{
	m_strName = strName;
	m_nUpDown = nUpDown;
	m_nSellBuy = nSellBuy;

	CString strDebug;
	strDebug.Format("#######SetData::%s###\r\n", m_strName);
	OutputDebugString(strDebug);
}
