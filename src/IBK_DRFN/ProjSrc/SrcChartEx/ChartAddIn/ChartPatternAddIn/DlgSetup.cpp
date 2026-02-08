// DlgSetup.cpp : implementation file
//

#include "stdafx.h"
#include "chartpatternaddin.h"
#include "DlgSetup.h"
#include "Include/ColorTable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSetup dialog


CDlgSetup::CDlgSetup(CWnd* pParent /*=NULL*/)
	: CRscDlg(CDlgSetup::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSetup)
		// NOTE: the ClassWizard will add member initialization here
	m_brBack.CreateSolidBrush( RGB(233,233,233));		// Dialog BG Color
	//}}AFX_DATA_INIT
}


void CDlgSetup::DoDataExchange(CDataExchange* pDX)
{
	CRscDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSetup)
	DDX_Control(pDX, IDC_TAB1, m_tabCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSetup, CRscDlg)
	//{{AFX_MSG_MAP(CDlgSetup)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSetup message handlers
void CDlgSetup::OnDestroy() 
{
	CRscDlg::OnDestroy();
	if( m_pEnvSetup1 != NULL )
		delete m_pEnvSetup1;
	if( m_pEnvSetup2 != NULL )
		delete m_pEnvSetup2;
	if( m_pEnvSetup3 != NULL )
		delete m_pEnvSetup3;
	if( m_pEnvSetup4 != NULL )
		delete m_pEnvSetup4;
	if ( m_brBack.GetSafeHandle())	m_brBack.DeleteObject();
}

HBRUSH CDlgSetup::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CRscDlg::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	switch(nCtlColor) 
	{
	case CTLCOLOR_EDIT:
	case CTLCOLOR_LISTBOX:
		return hbr;
		break;
	default:
		pDC->SetBkColor( RGB(233,233,233));
		hbr = m_brBack;
//		pDC->SetBkColor(m_crBk);
//		return m_brBkColor;		
		break;
	}
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CDlgSetup::SetPatternEnvData(CPatternEnvData *pPatternEnvData)
{
	if ( pPatternEnvData)
	{
		m_pPatternEnvData = pPatternEnvData;
	}
}

BOOL CDlgSetup::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	ModifyStyle(WS_CHILD, WS_POPUP);
//	m_tabCtrl.SetTopLeftCorner(CPoint(0,21));//14,32
	m_tabCtrl.SetTopLeftCorner(CPoint(10,28));//14,32
	m_tabCtrl.SetLeftOffset(0);	//9
	m_tabCtrl.SetUpperOffset(0);//9
	m_tabCtrl.SetSelectedColor(RGB(0, 0, 0),RGB(239,242,247));
	m_tabCtrl.SetNormalColor(RGB(0,0,0), RGB(255,255,255) , RGB(226,225,233));
	m_tabCtrl.SetTabBkColor(RGB(234,235,233));

// 2008.02.20 by LYH >>
	CString strDrawMethod, strAutoTrend, strTrendPattern, strCandlePattern;
	strDrawMethod.LoadString(IDS_DRAWING_METHOD);
	strAutoTrend.LoadString(IDS_AUTOTREND);
	strTrendPattern.LoadString(IDS_TREND_PATTERN);
	strCandlePattern.LoadString(IDS_CANDLE_PATTERN);
// 2008.02.20 by LYH <<

	m_pEnvSetup1 = new CDlgEnvSetup1(this);
	m_pEnvSetup1->SetPatternEnvData(m_pPatternEnvData);
	m_pEnvSetup1->Create(CDlgEnvSetup1::IDD,this);
// 2008.02.20 by LYH >>
	//m_tabCtrl.AddTab(m_pEnvSetup1,"작도법");
	m_tabCtrl.AddTab(m_pEnvSetup1,(LPTSTR)(LPCTSTR)strDrawMethod);
// 2008.02.20 by LYH <<

	m_pEnvSetup2 = new CDlgEnvSetup2(this);
	m_pEnvSetup2->SetPatternEnvData(m_pPatternEnvData);
	m_pEnvSetup2->Create(CDlgEnvSetup2::IDD,this);
// 2008.02.20 by LYH >>
	//m_tabCtrl.AddTab(m_pEnvSetup2,"자동추세선");
	m_tabCtrl.AddTab(m_pEnvSetup2,(LPTSTR)(LPCTSTR)strAutoTrend);
// 2008.02.20 by LYH <<

	m_pEnvSetup3 = new CDlgEnvSetup3(this);
	m_pEnvSetup3->SetPatternEnvData(m_pPatternEnvData);
	m_pEnvSetup3->Create(CDlgEnvSetup3::IDD,this);
// 2008.02.20 by LYH >>
	//m_tabCtrl.AddTab(m_pEnvSetup3,"추세패턴");
	m_tabCtrl.AddTab(m_pEnvSetup3,(LPTSTR)(LPCTSTR)strTrendPattern);
// 2008.02.20 by LYH <<

	m_pEnvSetup4 = new CDlgEnvSetup4(this);
	m_pEnvSetup4->SetPatternEnvData(m_pPatternEnvData);
	m_pEnvSetup4->Create(CDlgEnvSetup4::IDD,this);
// 2008.02.20 by LYH >>
	//m_tabCtrl.AddTab(m_pEnvSetup4,"캔들패턴");
	m_tabCtrl.AddTab(m_pEnvSetup4,(LPTSTR)(LPCTSTR)strCandlePattern);
// 2008.02.20 by LYH <<
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSetup::OnOK() 
{
	// TODO: Add extra validation here
	if ( m_pPatternEnvData)	
	{
		m_pEnvSetup1->OnOK();
		m_pEnvSetup2->OnOK();
		m_pEnvSetup3->OnOK();
		m_pEnvSetup4->OnOK();
	}

	CDialog::OnOK();
}