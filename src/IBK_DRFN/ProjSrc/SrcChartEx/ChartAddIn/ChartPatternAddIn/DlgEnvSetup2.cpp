// DlgPatternEnvSetup.cpp : implementation file
//

#include "stdafx.h"
#include "DlgEnvSetup2.h"

#include "PatternDefine.h"
#include "Include/ColorTable.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgEnvSetup2 dialog


CDlgEnvSetup2::CDlgEnvSetup2(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgEnvSetup2::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgEnvSetup2)
	//}}AFX_DATA_INIT

	m_pPatternEnvData = NULL;


	m_brBack.CreateSolidBrush( COLOR_BG_ANLYS_DLG);		// Dialog BG Color
	m_brWhite.CreateSolidBrush(COLOR_WHITE);
}


void CDlgEnvSetup2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgEnvSetup2)
	DDX_Control(pDX, IDC_CB_LOW_THICK, m_cbLowThick);
	DDX_Control(pDX, IDC_BTN_LOW_CLR, m_btnLowClr);
	DDX_Control(pDX, IDC_CB_HIGH_THICK, m_cbHighThick);
	DDX_Control(pDX, IDC_BTN_HIGH_CLR, m_btnHighClr);
	DDX_Control(pDX, IDC_CHK_HIGHPTREGISTLINE, m_chkHighPtRegistLine);
	DDX_Control(pDX, IDC_CHK_LOWPTSUPPORTLINE, m_chkLowPtSupportLine);
	DDX_Control(pDX, IDC_EDIT_NEXTPT_ALLOWABLE, m_edtNextPtAllowable);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgEnvSetup2, CDialog)
	//{{AFX_MSG_MAP(CDlgEnvSetup2)
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_HIGH_CLR, OnBtnHighClr)
	ON_BN_CLICKED(IDC_BTN_LOW_CLR, OnBtnLowClr)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgEnvSetup2 message handlers


BOOL CDlgEnvSetup2::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetPatternEnvDataToControl();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgEnvSetup2::SetPatternEnvData(CPatternEnvData *pPatternEnvData)
{
	if ( pPatternEnvData)
	{
		m_pPatternEnvData = pPatternEnvData;
	}
}

void CDlgEnvSetup2::SetPatternEnvDataToControl()
{
	if ( m_pPatternEnvData)
	{
		CString strTemp;

		// 고점저항선, 저점 지지선
		m_chkHighPtRegistLine.SetCheck(m_pPatternEnvData->m_bDrawHighRegistLine);
		m_chkLowPtSupportLine.SetCheck(m_pPatternEnvData->m_bDrawLowSupportLine);
		
		// 다음주요점과 허용오차%(기울기를 이용한 다음 포인트와의 차이)
		strTemp.Format("%.1f", m_pPatternEnvData->m_dNextPtAllowableError);
		m_edtNextPtAllowable.SetWindowText(strTemp);

		//라인 속성
		m_btnHighClr.SetColor(m_pPatternEnvData->m_lineDataHighRegist.m_color);
		m_cbHighThick.m_bitmap.LoadBitmap(IDB_LINE_THICKNESS);
		
		m_btnLowClr.SetColor(m_pPatternEnvData->m_lineDataLowSupport.m_color);
		m_cbLowThick.m_bitmap.LoadBitmap(IDB_LINE_THICKNESS);
		for(int iIdx=0; iIdx<5; iIdx++)
		{
			m_cbHighThick.AddString("");
			m_cbLowThick.AddString("");
		}
		for (int i= -1 ;i < m_cbHighThick.GetCount();i++) 
		{
			m_cbHighThick.SetItemHeight(i, 14);
			m_cbLowThick.SetItemHeight(i, 14);
		}
		m_cbHighThick.SetCurSel(m_pPatternEnvData->m_lineDataHighRegist.m_nWeight -1);
		m_cbLowThick.SetCurSel(m_pPatternEnvData->m_lineDataLowSupport.m_nWeight -1);

		UpdateData(FALSE);
	}
}

void CDlgEnvSetup2::OnOK() 
{
	// TODO: Add extra validation here
	if ( m_pPatternEnvData)	
	{
		UpdateData(TRUE);
		

		CString strTemp;
		// 고점저항선, 저점 지지선
		m_pPatternEnvData->m_bDrawHighRegistLine = m_chkHighPtRegistLine.GetCheck();
		m_pPatternEnvData->m_bDrawLowSupportLine = m_chkLowPtSupportLine.GetCheck();
		
		m_pPatternEnvData->m_lineDataHighRegist.m_color = m_btnHighClr.GetColor();
		m_pPatternEnvData->m_lineDataLowSupport.m_color = m_btnLowClr.GetColor();
	
		m_pPatternEnvData->m_lineDataHighRegist.m_nWeight = m_cbHighThick.GetCurSel()+1;
		m_pPatternEnvData->m_lineDataLowSupport.m_nWeight = m_cbLowThick.GetCurSel()+1;
		
		// 다음주요점과 허용오차%(기울기를 이용한 다음 포인트와의 차이)
		m_edtNextPtAllowable.GetWindowText(strTemp);
		strTemp.Format("%.1f", atof(strTemp));
		m_pPatternEnvData->m_dNextPtAllowableError = atof(strTemp);
	}

	//CDialog::OnOK();
}


HBRUSH CDlgEnvSetup2::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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

void CDlgEnvSetup2::OnDestroy() 
{
	// Delete Brush
	if ( m_brBack.GetSafeHandle())	m_brBack.DeleteObject();
	if ( m_brWhite.GetSafeHandle())	m_brWhite.DeleteObject();


	CDialog::OnDestroy();

}

void CDlgEnvSetup2::OnBtnHighClr() 
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

void CDlgEnvSetup2::OnBtnLowClr() 
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
