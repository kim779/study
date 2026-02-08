// MarketColorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CX_INTERGRID.h"
#include "MarketColorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMarketColorDlg dialog


CMarketColorDlg::CMarketColorDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(CMarketColorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMarketColorDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMarketColorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMarketColorDlg)
	DDX_Control(pDX, IDC_CHECK_APPLY, m_cbApply);
/*@@Zeta
	DDX_Control(pDX, IDC_KOSPI_COLOR, m_ctlKospiColor);
	DDX_Control(pDX, IDC_KOSDAQ_COLOR, m_ctlKosdaqColor);
*/
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMarketColorDlg, CDialog)
	//{{AFX_MSG_MAP(CMarketColorDlg)
	ON_BN_CLICKED(IDC_CHECK_APPLY, OnCheckApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAlertSoundDlg Operators

void CMarketColorDlg::EnableConrol(BOOL bEnable)
{
	GetDlgItem(IDC_KOSPI_COLOR)->EnableWindow(bEnable);
	GetDlgItem(IDC_KOSDAQ_COLOR)->EnableWindow(bEnable);
}

/////////////////////////////////////////////////////////////////////////////
// CMarketColorDlg message handlers

BOOL CMarketColorDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_cbApply.SetCheck(m_bMKClrApply);
	EnableConrol(m_bMKClrApply);
/*@@Zeta
	m_ctlKospiColor.SetColor(m_clrKospi);
	m_ctlKosdaqColor.SetColor(m_clrKosdaq);
*/	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMarketColorDlg::OnCheckApply() 
{
	m_bMKClrApply = !m_bMKClrApply;
	EnableConrol(m_bMKClrApply);
}

void CMarketColorDlg::OnOK() 
{
/*@@Zeta
	m_clrKospi  = m_ctlKospiColor.GetColor();
	m_clrKosdaq = m_ctlKosdaqColor.GetColor();
*/
	CDialog::OnOK();
}
