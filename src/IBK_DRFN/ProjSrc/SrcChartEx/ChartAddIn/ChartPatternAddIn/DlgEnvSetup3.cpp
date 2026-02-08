// DlgEnvSetup3.cpp : implementation file
//

#include "stdafx.h"
#include "DlgEnvSetup3.h"

#include "PatternDefine.h"
#include "Include/ColorTable.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgEnvSetup3 dialog


CDlgEnvSetup3::CDlgEnvSetup3(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgEnvSetup3::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgEnvSetup3)
	//}}AFX_DATA_INIT

	m_pPatternEnvData = NULL;


	m_brBack.CreateSolidBrush( COLOR_BG_ANLYS_DLG);		// Dialog BG Color
	m_brWhite.CreateSolidBrush(COLOR_WHITE);
}


void CDlgEnvSetup3::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgEnvSetup3)
	DDX_Control(pDX, IDC_CHK_AUTOPATTERN, m_chkAutoPattern);
	DDX_Control(pDX, IDC_CB_WAVE_THICK, m_cbWaveThick);
	DDX_Control(pDX, IDC_BTN_WAVE_CLR, m_btnWaveClr);
	DDX_Control(pDX, IDC_CB_LOW_THICK, m_cbLowThick);
	DDX_Control(pDX, IDC_BTN_LOW_CLR, m_btnLowClr);
	DDX_Control(pDX, IDC_CB_HIGH_THICK, m_cbHighThick);
	DDX_Control(pDX, IDC_BTN_HIGH_CLR, m_btnHighClr);
	DDX_Control(pDX, IDC_CHK_HIGHLOWPTLINE, m_chkHighLowPtLine);
	DDX_Control(pDX, IDC_CHK_MMA, m_chkMMa);
	DDX_Control(pDX, IDC_CHK_HIGHPTHLINE, m_chkHighPtHLine);
	DDX_Control(pDX, IDC_CHK_LOWPTHLINE, m_chkLowPtHLine);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgEnvSetup3, CDialog)
	//{{AFX_MSG_MAP(CDlgEnvSetup3)
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_WAVE_CLR, OnBtnWaveClr)
	ON_BN_CLICKED(IDC_BTN_HIGH_CLR, OnBtnHighClr)
	ON_BN_CLICKED(IDC_BTN_LOW_CLR, OnBtnLowClr)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgEnvSetup3 message handlers


BOOL CDlgEnvSetup3::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetPatternEnvDataToControl();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgEnvSetup3::SetPatternEnvData(CPatternEnvData *pPatternEnvData)
{
	if ( pPatternEnvData)
	{
		m_pPatternEnvData = pPatternEnvData;
	}
}

void CDlgEnvSetup3::SetPatternEnvDataToControl()
{
	if ( m_pPatternEnvData)
	{
		CString strTemp;

		// 추세패턴
		m_chkAutoPattern.SetCheck(m_pPatternEnvData->m_bDrawAutoPatternLine);
		// 파동선
		m_chkHighLowPtLine.SetCheck(m_pPatternEnvData->m_bDrawHighLowPtLine);
		
		// 조정이동평균선
		//m_chkMMa.SetCheck(m_pPatternEnvData->m_bDrawMMa);

		// 고점, 저점 수평선
		m_chkHighPtHLine.SetCheck(m_pPatternEnvData->m_bDrawHighPtHLine);
		m_chkLowPtHLine.SetCheck(m_pPatternEnvData->m_bDrawLowPtHLine);
		UpdateData(FALSE);

		//라인 속성
		m_btnWaveClr.SetColor(m_pPatternEnvData->m_lineDataHighLowPt.m_color);		// 파동선
		m_cbWaveThick.m_bitmap.LoadBitmap(IDB_LINE_THICKNESS);

		m_btnHighClr.SetColor(m_pPatternEnvData->m_lineDataHighPtH.m_color);	// 고점 수평라인
		m_cbHighThick.m_bitmap.LoadBitmap(IDB_LINE_THICKNESS);
		
		m_btnLowClr.SetColor(m_pPatternEnvData->m_lineDataLowPtH.m_color);		// 저점 수평라인
		m_cbLowThick.m_bitmap.LoadBitmap(IDB_LINE_THICKNESS);

		for(int iIdx=0; iIdx<5; iIdx++)
		{
			m_cbWaveThick.AddString("");
			m_cbHighThick.AddString("");
			m_cbLowThick.AddString("");
		}
		for (int i= -1 ;i < m_cbHighThick.GetCount();i++) 
		{
			m_cbWaveThick.SetItemHeight(i, 14);
			m_cbHighThick.SetItemHeight(i, 14);
			m_cbLowThick.SetItemHeight(i, 14);
		}
		m_cbWaveThick.SetCurSel(m_pPatternEnvData->m_lineDataHighLowPt.m_nWeight -1);
		m_cbHighThick.SetCurSel(m_pPatternEnvData->m_lineDataHighPtH.m_nWeight -1);
		m_cbLowThick.SetCurSel(m_pPatternEnvData->m_lineDataLowPtH.m_nWeight -1);

	}
}

void CDlgEnvSetup3::OnOK() 
{
	// TODO: Add extra validation here
	if ( m_pPatternEnvData)	
	{
		UpdateData(TRUE);
		
		CString strTemp;

		// 추세패턴
		m_pPatternEnvData->m_bDrawAutoPatternLine = m_chkAutoPattern.GetCheck();

		// 파동선
		m_pPatternEnvData->m_bDrawHighLowPtLine = m_chkHighLowPtLine.GetCheck();

		// 조정이동평균선
		m_pPatternEnvData->m_bDrawMMa = m_chkMMa.GetCheck();

		// 고점, 저점 수평선
		m_pPatternEnvData->m_bDrawHighPtHLine = m_chkHighPtHLine.GetCheck();
		m_pPatternEnvData->m_bDrawLowPtHLine = m_chkLowPtHLine.GetCheck();

		//선색상, 두께
		m_pPatternEnvData->m_lineDataHighPtH.m_color = m_btnHighClr.GetColor();
		m_pPatternEnvData->m_lineDataLowPtH.m_color = m_btnLowClr.GetColor();
		m_pPatternEnvData->m_lineDataHighLowPt.m_color = m_btnWaveClr.GetColor();
	
		m_pPatternEnvData->m_lineDataHighPtH.m_nWeight = m_cbHighThick.GetCurSel()+1;
		m_pPatternEnvData->m_lineDataLowPtH.m_nWeight = m_cbLowThick.GetCurSel()+1;
		m_pPatternEnvData->m_lineDataHighLowPt.m_nWeight = m_cbWaveThick.GetCurSel()+1;
	}

	//CDialog::OnOK();
}


HBRUSH CDlgEnvSetup3::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
//	pDC->SetBkMode( TRANSPARENT);

	if ( nCtlColor == CTLCOLOR_EDIT)
	{
		pDC->SetBkColor( COLOR_WHITE);

		hbr = m_brWhite;
	}
	else
	{
		pDC->SetBkColor( COLOR_BG_ANLYS_DLG);
		hbr = m_brBack;
	}
		
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CDlgEnvSetup3::OnDestroy() 
{
	// Delete Brush
	if ( m_brBack.GetSafeHandle())	m_brBack.DeleteObject();
	if ( m_brWhite.GetSafeHandle())	m_brWhite.DeleteObject();


	CDialog::OnDestroy();

}

void CDlgEnvSetup3::OnBtnWaveClr() 
{
	// TODO: Add your control notification handler code here
	COLORREF crColor = m_btnWaveClr.GetColor();
	CColorDialog color(crColor, CC_FULLOPEN);
	if(color.DoModal() == IDOK) 
	{
		m_btnWaveClr.SetColor(color.GetColor());				
		m_btnWaveClr.Invalidate();
	}
}

void CDlgEnvSetup3::OnBtnHighClr() 
{
	// TODO: Add your control notification handler code here
	COLORREF crColor = m_btnHighClr.GetColor();
	CColorDialog color(crColor, CC_FULLOPEN);
	if(color.DoModal() == IDOK) 
	{
		m_btnHighClr.SetColor(color.GetColor());				
		m_btnHighClr.Invalidate();
	}
}

void CDlgEnvSetup3::OnBtnLowClr() 
{
	// TODO: Add your control notification handler code here
	COLORREF crColor = m_btnLowClr.GetColor();
	CColorDialog color(crColor, CC_FULLOPEN);
	if(color.DoModal() == IDOK) 
	{
		m_btnLowClr.SetColor(color.GetColor());				
		m_btnLowClr.Invalidate();
	}
}
