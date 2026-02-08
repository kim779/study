// MarketConfirm.cpp : implementation file
//

#include "stdafx.h"
#include "ib300200.h"
#include "MarketConfirm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMarketConfirm dialog


CMarketConfirm::CMarketConfirm(CControlWnd* pParent /*=NULL*/)
	: CDialog(CMarketConfirm::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMarketConfirm)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pCtlWnd = pParent;
}


void CMarketConfirm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMarketConfirm)
	DDX_Control(pDX, IDC_CHECK_AGAIN, m_chkAgain);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMarketConfirm, CDialog)
	//{{AFX_MSG_MAP(CMarketConfirm)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_CHECK_AGAIN, OnCheckAgain)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMarketConfirm message handlers

HBRUSH CMarketConfirm::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	return hbr;
}

void CMarketConfirm::OnCheckAgain() 
{	
}

void CMarketConfirm::OnOK() 
{
	if(m_pCtlWnd)
	{
		if(m_chkAgain.GetCheck())
		{
			m_pCtlWnd->m_bMarketConfirm = FALSE;
		}
	}
	
	CDialog::OnOK();
}

BOOL CMarketConfirm::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CFont* pFont = GetDlgItem(IDC_STATIC_CONF)->GetFont();
	
	std::shared_ptr<CFont> newFont = std::make_shared<CFont>();
	LOGFONT lf{};

//	newFont = new CFont();
	memset(&lf,0,sizeof(LOGFONT));
	
	pFont->GetLogFont(&lf);
	
	lf.lfWeight = FW_BOLD;
	
	newFont->CreateFontIndirect(&lf);
	
	GetDlgItem(IDC_STATIC_CONF)->SetFont(newFont.get());
	
	newFont->Detach();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
