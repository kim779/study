// DlgPrintCfg.cpp : implementation file
//

#include "stdafx.h"
#include "KTBChart.h"
#include "DlgPrintCfg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgPrintCfg dialog


CDlgPrintCfg::CDlgPrintCfg(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPrintCfg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgPrintCfg)
	m_nPrintAllbyOnePageDataCount = FALSE;
	//}}AFX_DATA_INIT
	m_nPrintColor = 0;
	m_nPrintRange = 1;
	m_HDC = NULL;
}


void CDlgPrintCfg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPrintCfg)
	DDX_Control(pDX, IDC_SEVERALPAGE, m_PrintAllByOnePageCount_Bt);
	DDX_Radio(pDX, IDC_RADIO_OPTIMUM, m_nPrintColor);
	DDX_Radio(pDX, IDC_ALL, m_nPrintRange);
	DDX_Check(pDX, IDC_SEVERALPAGE, m_nPrintAllbyOnePageDataCount);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPrintCfg, CDialog)
	//{{AFX_MSG_MAP(CDlgPrintCfg)
	ON_BN_CLICKED(IDC_BUTTON_PRINTSETTING, OnButtonPrintsetting)
	ON_BN_CLICKED(IDC_ALL, OnAll)
	ON_BN_CLICKED(IDC_SELECTIONRANGE, OnSelectionrange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPrintCfg message handlers

void CDlgPrintCfg::OnOK() 
{
	UpdateData(TRUE);
	CDialog::OnOK();
}

void CDlgPrintCfg::OnButtonPrintsetting() 
{
	UpdateData(TRUE);

	DWORD dwStyle;
	if(m_nPrintRange == 0) //모두
		dwStyle = PD_ALLPAGES | PD_USEDEVMODECOPIES;
	else if(m_nPrintRange == 1) //선택영역
		dwStyle = PD_SELECTION | PD_USEDEVMODECOPIES;
	
	CPrintDialog dlg(FALSE, dwStyle);
	if(dlg.DoModal() != IDOK)
		return;
	 
	// is a default printer set up?
	m_HDC = dlg.GetPrinterDC();
	if(dlg.PrintAll())
		m_nPrintRange = 0;
	else if(dlg.PrintSelection()) 
		m_nPrintRange = 1;

	EnableCheckButton();
	UpdateData(FALSE);
}

BOOL CDlgPrintCfg::GetPrintRange()
{
	return m_nPrintRange ? TRUE : FALSE;
}

BOOL CDlgPrintCfg::GetPrintColor()
{
	return m_nPrintColor ? TRUE : FALSE;
}

BOOL CDlgPrintCfg::PrintAllByOnePageDataCount()
{
	return m_nPrintAllbyOnePageDataCount ? TRUE : FALSE;
}

HDC CDlgPrintCfg::GetHDC()
{
	if(m_HDC == NULL) {
		// get the default printer
		CPrintDialog dlg(FALSE);
		dlg.GetDefaults();

		// is a default printer set up?
		return dlg.GetPrinterDC();
	}
	return m_HDC;
}

void CDlgPrintCfg::OnAll() 
{
	UpdateData(TRUE);
	EnableCheckButton();
}

void CDlgPrintCfg::EnableCheckButton()
{
	if(m_nPrintRange == 0) //all
		m_PrintAllByOnePageCount_Bt.EnableWindow(FALSE);
	else //선택영역
		m_PrintAllByOnePageCount_Bt.EnableWindow(TRUE);
}

void CDlgPrintCfg::OnSelectionrange() 
{
	UpdateData(TRUE);
	EnableCheckButton();
}
