// DlgMainTabAnalysisSetUp.cpp : implementation file
//

#include "stdafx.h"
#include "ChartEnvSetupDlgAddIn.h"
#include "DlgMainTabAnalysisSetUp.h"

#include "../Include_Chart/OutsideCommonInterfaceFunction.h"	// for COutsideCommonInterfaceFunction
#include "DlgEnvSetUpAll.h"	// 부모
#include "Conversion.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgMainTabAnalysisSetUp dialog


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 2
// Return void : 
// Param  CAnalysisToolData *pAnalysisToolData : 
// Param  CWnd* pParent : 
// Comments		: 
//-----------------------------------------------------------------------------
CDlgMainTabAnalysisSetUp::CDlgMainTabAnalysisSetUp( IChartOCX *p_pIChartOCX, CWnd* pParent /*=NULL*/)
	: CBaseTabDlg(CDlgMainTabAnalysisSetUp::IDD, pParent)
//	: CDialog(CDlgMainTabAnalysisSetUp::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgMainTabAnalysisSetUp)
	m_bShowTrendLine = FALSE;
	m_bShowVertical = FALSE;
	m_bShowThree = FALSE;
	m_bShowHorizonLeft = FALSE;
	m_bShowHorizonRight = FALSE;
	m_bShowFour = FALSE;
	m_bShowFibo = FALSE;
	//}}AFX_DATA_INIT
	
	m_pIChartOCX = p_pIChartOCX;
	if( m_pIChartOCX) m_pIChartOCX->AddRef();

	GetAnalysisToolData( m_OrgAnalysisToolData );
	GetAnalysisToolData( m_AnalysisToolData );

	m_bPreview = FALSE;
}

IMPLEMENT_DYNAMIC( CDlgMainTabAnalysisSetUp, CBaseTabDlg)

CDlgMainTabAnalysisSetUp::~CDlgMainTabAnalysisSetUp()   // standard destructor
{
	if( m_pIChartOCX)
	{
		m_pIChartOCX->Release();
		m_pIChartOCX = NULL;
	}
}

void CDlgMainTabAnalysisSetUp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMainTabAnalysisSetUp)
	DDX_Control(pDX, IDC_TRENDLINE_COLOR, m_btnTrendLineColor);
	DDX_Check(pDX, IDC_SHOW_TREND, m_bShowTrendLine);
	DDX_Check(pDX, IDC_SHOW_VERTICAL, m_bShowVertical);
	DDX_Check(pDX, IDC_SHOW_THREE, m_bShowThree);
	DDX_Check(pDX, IDC_SHOW_HORIZON, m_bShowHorizonLeft);
	DDX_Check(pDX, IDC_SHOW_HORIZON_RIGHT, m_bShowHorizonRight);
	DDX_Check(pDX, IDC_SHOW_FOUR, m_bShowFour);
	DDX_Check(pDX, IDC_SHOW_FIBO, m_bShowFibo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMainTabAnalysisSetUp, CDialog)
	//{{AFX_MSG_MAP(CDlgMainTabAnalysisSetUp)
	ON_BN_CLICKED(IDC_SHOW_TREND, OnShowTrend)
	ON_BN_CLICKED(IDC_SHOW_VERTICAL, OnShowVertical)
	ON_BN_CLICKED(IDC_SHOW_HORIZON, OnShowHorizon)
	ON_BN_CLICKED(IDC_SHOW_THREE, OnShowThree)
	ON_BN_CLICKED(IDC_SHOW_FOUR, OnShowFour)
	ON_BN_CLICKED(IDC_SHOW_FIBO, OnShowFibo)
	ON_BN_CLICKED(IDC_SHOW_HORIZON_RIGHT, OnShowHorizonRight)
	ON_BN_CLICKED(IDC_BTN_SAVE, OnBtnSave)
	ON_BN_CLICKED(IDC_BTN_RECOVER, OnBtnRecover)
	ON_BN_CLICKED(IDC_BTN_PREVIEW, OnBtnPreview)
	ON_BN_CLICKED(IDC_TRENDLINE_COLOR, OnTrendlineColor)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMainTabAnalysisSetUp message handlers

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 2
// Modifier		: 
// Comments		: 
//-----------------------------------------------------------------------------
BOOL CDlgMainTabAnalysisSetUp::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	Initial();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 2
// Modifier		: 
// Comments		: 각각의 값의 초기화
//-----------------------------------------------------------------------------
void CDlgMainTabAnalysisSetUp::Initial()
{
	if( m_pIChartOCX == NULL )
		return;

	// 추세선 색상
	m_clrTrendLineColor = m_pIChartOCX->GetStyleValueAnalysisToolColor();	
	m_clrOrgTrendLineColor = m_clrTrendLineColor;
	m_btnTrendLineColor.SetColor( m_clrTrendLineColor );
	m_btnTrendLineColor.Invalidate();
	
	// 분석도구
	GetAnalysisToolData( m_AnalysisToolData );

	m_bShowTrendLine	= m_AnalysisToolData.GetShowTrendLine();
	m_bShowVertical		= m_AnalysisToolData.GetShowVertical();
	m_bShowHorizonLeft	= m_AnalysisToolData.GetShowHorizonLeft();
	m_bShowHorizonRight = m_AnalysisToolData.GetShowHorizonRight();
	m_bShowThree		= m_AnalysisToolData.GetShowThree();
	m_bShowFour			= m_AnalysisToolData.GetShowFour();
	m_bShowFibo			= m_AnalysisToolData.GetShowFibo();

	// 원래값을 저장한다.
	m_OrgAnalysisToolData.SetSavedData( m_AnalysisToolData );

	UpdateData(FALSE);
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 2
// Modifier		: 
// Comments		: 키보드 처리
//-----------------------------------------------------------------------------
BOOL CDlgMainTabAnalysisSetUp::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN)
		{
			//OnBtnSave();
			if( m_pParentWnd != NULL )
				( (CDlgEnvSetUpAll *)m_pParentWnd)->OK();
		}
		
		if( pMsg->wParam == VK_ESCAPE )
		{
			if( m_pParentWnd != NULL )
				( (CDlgEnvSetUpAll *)m_pParentWnd)->Cancel();
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 2
// Modifier		: 
// Comments		: 추세선 text보이기
//-----------------------------------------------------------------------------
void CDlgMainTabAnalysisSetUp::OnShowTrend() 
{
	UpdateData(TRUE);
	m_AnalysisToolData.SetShowTrendLine(m_bShowTrendLine ? true:false);	
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 2
// Modifier		: 
// Comments		: 
//-----------------------------------------------------------------------------
void CDlgMainTabAnalysisSetUp::OnShowVertical() 
{
	UpdateData(TRUE);
	m_AnalysisToolData.SetShowVertical(m_bShowVertical ? true:false);
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 2
// Modifier		: 
// Comments		: 
//-----------------------------------------------------------------------------
void CDlgMainTabAnalysisSetUp::OnShowHorizon() 
{
	UpdateData(TRUE);
	m_AnalysisToolData.SetShowHorizonLeft(m_bShowHorizonLeft ? true:false);
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 2
// Modifier		: 
// Comments		: 
//-----------------------------------------------------------------------------
void CDlgMainTabAnalysisSetUp::OnShowHorizonRight() 
{
	UpdateData(TRUE);
	m_AnalysisToolData.SetShowHorizonRight(m_bShowHorizonRight ? true:false);
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 2
// Modifier		: 
// Comments		: 
//-----------------------------------------------------------------------------
void CDlgMainTabAnalysisSetUp::OnShowThree() 
{
	UpdateData(TRUE);
	m_AnalysisToolData.SetShowThree(m_bShowThree ? true:false);
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 2
// Modifier		: 
// Comments		: 
//-----------------------------------------------------------------------------
void CDlgMainTabAnalysisSetUp::OnShowFour() 
{
	UpdateData(TRUE);
	m_AnalysisToolData.SetShowFour(m_bShowFour ? true:false);
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 2
// Modifier		: 
// Comments		: 
//-----------------------------------------------------------------------------
void CDlgMainTabAnalysisSetUp::OnShowFibo() 
{
	UpdateData(TRUE);
	m_AnalysisToolData.SetShowFibo(m_bShowFibo ? true:false);
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 13
// Return void : 
// Comments		: 
//-----------------------------------------------------------------------------
void CDlgMainTabAnalysisSetUp::OnBtnSave() 
{
	// TODO: Add your control notification handler code here
	m_bPreview = FALSE;
	
//	m_clrTrendLineColor = m_btnTrendLineColor.GetColor();
	SetAnalysisToolColor( m_clrTrendLineColor);
	m_clrOrgTrendLineColor = m_clrTrendLineColor;

	m_pIChartOCX->SetAnalysisToolTextFlag( m_AnalysisToolData.GetShowTrendLine(), m_AnalysisToolData.GetShowHorizonLeft(),
		m_AnalysisToolData.GetShowHorizonRight(), m_AnalysisToolData.GetShowVertical(), m_AnalysisToolData.GetShowThree(),
		m_AnalysisToolData.GetShowFour(), m_AnalysisToolData.GetShowFibo());

	m_OrgAnalysisToolData.SetSavedData( m_AnalysisToolData );
	
	m_pIChartOCX->InvalidateControl();
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 13
// Return void : 
// Comments		: 
//-----------------------------------------------------------------------------
void CDlgMainTabAnalysisSetUp::OnBtnRecover() 
{
	// TODO: Add your control notification handler code here
	// 원래값을 저장한다.
	m_bPreview = FALSE;

	SetAnalysisToolColor( m_clrOrgTrendLineColor );
	m_clrTrendLineColor = m_clrOrgTrendLineColor;
	m_btnTrendLineColor.SetColor( m_clrOrgTrendLineColor );
	m_btnTrendLineColor.Invalidate();

	m_AnalysisToolData.SetSavedData( m_OrgAnalysisToolData );
	m_bShowTrendLine	= m_OrgAnalysisToolData.GetShowTrendLine();
	m_bShowVertical		= m_OrgAnalysisToolData.GetShowVertical();
	m_bShowHorizonLeft	= m_OrgAnalysisToolData.GetShowHorizonLeft();
	m_bShowHorizonRight = m_OrgAnalysisToolData.GetShowHorizonRight();
	m_bShowThree		= m_OrgAnalysisToolData.GetShowThree();
	m_bShowFour			= m_OrgAnalysisToolData.GetShowFour();
	m_bShowFibo			= m_OrgAnalysisToolData.GetShowFibo();

	UpdateData(FALSE);

	m_pIChartOCX->SetAnalysisToolTextFlag( m_OrgAnalysisToolData .GetShowTrendLine(), m_OrgAnalysisToolData .GetShowHorizonLeft(),
		m_OrgAnalysisToolData .GetShowHorizonRight(), m_OrgAnalysisToolData .GetShowVertical(), m_OrgAnalysisToolData .GetShowThree(),
		m_OrgAnalysisToolData .GetShowFour(), m_OrgAnalysisToolData .GetShowFibo());
	m_pIChartOCX->InvalidateControl();
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 23
// Modifier		: 
// Comments		: 
//-----------------------------------------------------------------------------
void CDlgMainTabAnalysisSetUp::OnBtnPreview() 
{
	// TODO: Add your control notification handler code here
	m_bPreview = TRUE;
	SetAnalysisToolColor( m_clrTrendLineColor);
	m_pIChartOCX->SetAnalysisToolTextFlag( m_AnalysisToolData.GetShowTrendLine(), m_AnalysisToolData.GetShowHorizonLeft(),
		m_AnalysisToolData.GetShowHorizonRight(), m_AnalysisToolData.GetShowVertical(), m_AnalysisToolData.GetShowThree(),
		m_AnalysisToolData.GetShowFour(), m_AnalysisToolData.GetShowFibo());
	m_pIChartOCX->InvalidateControl();
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 23
// Modifier		: 
// Comments		: 
//-----------------------------------------------------------------------------
void CDlgMainTabAnalysisSetUp::Save( void )
{
	OnBtnSave();
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 23
// Modifier		: 
// Comments		: 
//-----------------------------------------------------------------------------
void CDlgMainTabAnalysisSetUp::Recover()
{
	OnBtnRecover();
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 23
// Modifier		: 
// Comments		: 
//-----------------------------------------------------------------------------
void CDlgMainTabAnalysisSetUp::OnTrendlineColor() 
{
	// TODO: Add your control notification handler code here
	m_clrTrendLineColor = m_btnTrendLineColor.GetColor();
	
	CColorDialog color( m_clrTrendLineColor, CC_FULLOPEN);

	if(color.DoModal() != IDOK) 
		return;
	
	m_clrTrendLineColor = color.GetColor();

	m_btnTrendLineColor.SetColor( m_clrTrendLineColor );
	m_btnTrendLineColor.Invalidate();

	// (2006/8/22 - Seung-Won, Bae) Shere the Analysis Tool Color within Analysis and Style Setting.
	( ( CTabMainChartEnvSetUp *)GetParent())->UpdateAnalysisColorInStyle( m_clrTrendLineColor);
}

// (2006/8/22 - Seung-Won, Bae) Shere the Analysis Tool Color within Analysis and Style Setting.
void CDlgMainTabAnalysisSetUp::UpdateAnalysisColor( COLORREF p_clrAnalysis)
{
	m_clrTrendLineColor = p_clrAnalysis;
	m_btnTrendLineColor.SetColor( m_clrTrendLineColor);
}


//sy 2005.10.12
CBaseTabDlg::DLG_TYPE CDlgMainTabAnalysisSetUp::GetDialogType() const
{
	return CBaseTabDlg::ANALYSIS_SET;
}
//sy end


/*-----------------------------------------------------------------------------------------
 - Function    :  GetAnalysisToolData
 - Created at  :  2005-08-02
 - Author      :  김선영
 - Description :  환경설정 관련 함수 - 분석도구에 관련된 내용
 -----------------------------------------------------------------------------------------*/
void CDlgMainTabAnalysisSetUp::GetAnalysisToolData(CAnalysisToolData& analysisToolData)
{
	//추세선
	analysisToolData.SetShowTrendLine( m_pIChartOCX->IsShowTrendLineText() ? true : false);

	//수직선
	analysisToolData.SetShowVertical( m_pIChartOCX->IsShowVerticalText() ? true : false);

	//수평선(왼쪽)
	analysisToolData.SetShowHorizonLeft( m_pIChartOCX->IsShowHorizonTextLeft() ? true : false);

	//수평선(오른쪽)
	analysisToolData.SetShowHorizonRight( m_pIChartOCX->IsShowHorizonTextRight() ? true : false);

	//삼등분선
	analysisToolData.SetShowThree( m_pIChartOCX->IsShowThreeText() ? true : false);

	//사등분선
	analysisToolData.SetShowFour( m_pIChartOCX->IsShowFourText() ? true : false);

	//피보나치되돌림
	analysisToolData.SetShowFibo( m_pIChartOCX->IsShowFiboText() ? true : false);
}

void CDlgMainTabAnalysisSetUp::SetAnalysisToolColor( COLORREF p_clrColor)
{
	m_pIChartOCX->SetStyleValueAnalysisToolColor( p_clrColor);

	// (2009/9/3 - Seung-Won, Bae) for WorldOn MChartInterface
	COutsideCommonInterfaceFunction outsideIF;
	ILPCSTR szDataPath = m_pIChartOCX->GetDefaultPathString();
	outsideIF.SetOutsideCommonInterface( m_pIChartOCX->GetMChartInterface(), CString( szDataPath));
	outsideIF.MChart_SetTrendLineColorToOutside( p_clrColor);;
}