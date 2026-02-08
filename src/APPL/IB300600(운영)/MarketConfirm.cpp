// MarketConfirm.cpp : implementation file
//

#include "stdafx.h"
#include "IB300600.h"
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
	DDX_Control(pDX, IDC_CHECK_MARKET, m_chkMarket);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMarketConfirm, CDialog)
	//{{AFX_MSG_MAP(CMarketConfirm)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMarketConfirm message handlers

BOOL CMarketConfirm::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CFont* pFont = GetDlgItem(IDC_STATIC_CONF)->GetFont();
	CFont* newFont;
	
	LOGFONT lf;
	
	newFont = new CFont();
	memset(&lf,0,sizeof(LOGFONT));
	
	pFont->GetLogFont(&lf);
	
	lf.lfWeight = FW_BOLD;
	
	newFont->CreateFontIndirect(&lf);
	
	GetDlgItem(IDC_STATIC_CONF)->SetFont(newFont);
	
	newFont->Detach();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMarketConfirm::OnOK() 
{
	if(m_pCtlWnd)
	{
		if(m_chkMarket.GetCheck())
		{
			m_pCtlWnd->m_bMarketConfirm = FALSE;
		}
	}

	CDialog::OnOK();
}
