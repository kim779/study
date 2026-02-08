// DlgSooikChart.cpp : implementation file
//

#include "stdafx.h"

#include "DlgSooikChart.h"
#include <math.h>							// for fabs()
#include <float.h>							// for DBL_MAX, ...

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSooikChart dialog


CDlgSooikChart::CDlgSooikChart(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSooikChart::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSooikChart)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pStrategyItem= NULL;
	m_brBackground.CreateSolidBrush( RGB(255,255,255));//CLR_BACKGROUND);
}


void CDlgSooikChart::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSooikChart)
	DDX_Control(pDX, IDC_TCHART1, m_ctlBarTChart);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSooikChart, CDialog)
	//{{AFX_MSG_MAP(CDlgSooikChart)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_RTYPE1, OnRType1)
	ON_BN_CLICKED(IDC_RTYPE2, OnRType2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSooikChart message handlers


BOOL CDlgSooikChart::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	// 1. Get Client Area
	//CRect rctClient;
	//GetClientRect( &rctClient);
	//// 2. Get Chart Area
	//rctClient.right -= 6;	// why ? Border Style ? 
	//rctClient.bottom -= 6;	// why ? Border Style ? 
	//// 3. Create Section Chart
	//CRect rctSectionChart( rctClient);
	//rctSectionChart.top += 25;
	//if( !m_chartSection.CreateChart( this, rctSectionChart, 922)) return FALSE;
	//// 4. Create Accumulation Chart
	//CRect rctAccumulation( rctClient);
	//rctAccumulation.bottom -= rctAccumulation.Height() * 2 / 5 + 3;
	//if( !m_chartAccumulation.CreateChart( this, rctAccumulation, 927)) return FALSE;
	//m_chartAccumulation.ShowControl( FALSE);
	//// 5. Create Case Chart
	//CRect rctCase( rctClient);
	//rctCase.top += rctCase.Height() * 3 / 5 - 3;
	//if( !m_chartCase.CreateChart( this, rctCase, 928)) return FALSE;
	//m_chartCase.ShowControl( FALSE);

	CheckRadioButton(IDC_RTYPE1, IDC_RTYPE2, IDC_RTYPE1);
	OnRType1();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgSooikChart::OnRType1() 
{
	// TODO: Add your control notification handler code here

	if( 0 != m_nQuerySection) return;

	m_chartSection.ShowPartialChart(m_pDlgOptimizer );

	// 4. Set Timer for Zero Line. (the Timer must be used for getting the Axis's Min/Max
	SetTimer( 926, 100, NULL);
}

void CDlgSooikChart::OnRType2() 
{
	// TODO: Add your control notification handler code here

	if( 0 != m_nQuerySection) return;

	m_chartSection.ShowFullChart( m_pDlgOptimizer);

	// 4. Set Timer for Zero Line. (the Timer must be used for getting the Axis's Min/Max
	SetTimer( 926, 100, NULL);
}

void CDlgSooikChart::OnOK() 
{
	// TODO: Add extra validation here
//	CDialog::OnOK();
}
void CDlgSooikChart::OnCancel() 
{
	// TODO: Add extra cleanup here
//	CDialog::OnCancel();
}


/////////////////////////////////////////////////////////////////////////////
// CEarningsRatioChartCase Chart
/////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2005/9/27
// Modifier		: 
// Comments		: Constructor
//-----------------------------------------------------------------------------
CDlgSooikChart::CEarningsRatioChartCase::CEarningsRatioChartCase()
{
	// (2005/9/23 - Seung-Won, Bae) Manage Axis
	m_pTAxisLeft	= NULL;	
	m_pTAxisRight	= NULL;	
	m_pTAxisTop		= NULL;	
	m_pTAxisBottom	= NULL;	

	// (2005/9/26 - Seung-Won, Bae) Manage Zero Line for Adding before Other Data Line
	m_pTGraphZero = NULL;
}

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2005/9/27
// Modifier		: 
// Comments		: Create Chart Control
//-----------------------------------------------------------------------------
BOOL CDlgSooikChart::CEarningsRatioChartCase::CreateChart( CWnd *p_pParent, CRect p_rect, UINT p_uID)
{
	//if( !p_pParent) return FALSE;

	//p_rect.DeflateRect( 5, 5);
	//m_chartEzp.CreateControl( p_rect, p_pParent, p_uID, "°Çº°¼öÀÍ·ü");
	//m_chartEzp.SetBackgroundColor( CLR_BACKGROUND);
	//// 4. Show Side Scale Area
	//m_chartEzp.ShowOuterAxis( TRUE, FALSE, FALSE, TRUE);
	//// 5. Manage Top, Right Axis for Horizontal Axis's Centering
	//m_pTAxisLeft	= m_chartEzp.GetAxis( CTAxis::m_eATypeLeft);
	//m_pTAxisTop		= m_chartEzp.GetAxis( CTAxis::m_eATypeTop);
	//m_pTAxisRight	= m_chartEzp.GetAxis( CTAxis::m_eATypeRight);
	//m_pTAxisBottom	= m_chartEzp.GetAxis( CTAxis::m_eATypeBottom);
	//if( !m_pTAxisLeft || !m_pTAxisTop || !m_pTAxisRight	|| !m_pTAxisBottom) return FALSE;
	//// 6. Set Graph Indent
	//m_pTAxisLeft->SetPosition(		0, 5, 95, 0);
	//m_pTAxisRight->SetPosition(		0, 5, 95, 0);
	return TRUE;
}
//-----------------------------------------------------------------------------
// Author		: Han-Doo, Kim	Date :2005/10/14
// Modifier		: 
// Comments		: Remove All Graph
//-----------------------------------------------------------------------------
void CDlgSooikChart::CEarningsRatioChartCase::RemoveGraph()
{
	/*m_chartEzp.RemoveAllGraph();
	if( m_pTAxisLeft && m_pTAxisTop && m_pTAxisRight && m_pTAxisBottom) 
	{
		m_pTAxisBottom->SetAutomatic(TRUE, TRUE);
		m_pTAxisRight->SetAutomatic(TRUE, TRUE);
		m_pTAxisLeft->SetAutomatic(TRUE,TRUE);
		m_pTAxisTop->SetAutomatic(TRUE,TRUE);
	}*/
}
//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2005/9/27
// Modifier		: 
// Comments		: Show Full Chart
//-----------------------------------------------------------------------------
void CDlgSooikChart::CEarningsRatioChartCase::ShowChart( int p_nSection,CDlgOptimizer* pOptmizer)
{
//	// 0. Init Chart
//	// 0.1 Remove All Graph of previous Chart Type
//	m_chartEzp.RemoveAllGraph();
//	// 0.2 Add Zero Line
//	m_pTGraphZero = m_chartEzp.AddLineGraph( 1);
//	if( !m_pTGraphZero) return;
//	// 0.3 Set Left Axis Auto Range for getting Min/Max
//	if( !m_pTAxisLeft) return;
//	m_pTAxisLeft->SetAutomatic( TRUE, TRUE);
//
//	// 1. Get the Graph Count
//
////	if( nGraphCount <= p_nSection) return;
//
//	// 2. Add Graphs
//	// 2.1 Get Data Count
//
//	CIStrategyItem *pStrategyItem =pOptmizer->GetStrategyItem();
//	if( !pStrategyItem) return;
//	long lDataCount = pStrategyItem->GetCntStrategyResult();
//	if( lDataCount <= 0) return;
//		
//	// 2.2 Add One Graph for Each Section
//	CTGraph *pGraph = m_chartEzp.AddBarGraph();
//	if( !pGraph) return;
//
//	// 3. Set Graph Data
//	double dX = 0;
//	COLORREF clrBar;
//	CStrategyResult *pStrategyResult = NULL;
//	for( long i = 0; i < lDataCount; i++)
//	{
//		pStrategyResult = pStrategyItem->GetStrategyResult( i);
//		if( !pStrategyResult) continue;
//		if( CStrategyResult::EXITSHORT != pStrategyResult->m_DealType && CStrategyResult::EXITLONG != pStrategyResult->m_DealType) continue;
//
//		// (2005/9/30 - Seung-Won, Bae) Set Color for Ratio
//		if(			pStrategyResult->m_dProfitRate < 0)	clrBar = RGB(   0, 0, 255);
//		else if(	pStrategyResult->m_dProfitRate > 0)	clrBar = RGB( 255, 0,   0);
//		else											clrBar = RGB(   0, 0,   0);
//
//		pGraph->AddData( ++dX, pStrategyResult->m_dProfitRate, clrBar);
//	}
//
//	// (2005/9/30 - Seung-Won, Bae) Show Zero Line
//	ShowZeroLine();
}

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2005/9/27
// Modifier		: 
// Comments		: Show Zero Line
//-----------------------------------------------------------------------------
BOOL CDlgSooikChart::CEarningsRatioChartCase::ShowZeroLine( void)
{
	//// 0. Set Right & Top Axis Min/Max
	//// 0.1 Check NULL Pointer
	//if( !m_pTAxisLeft || !m_pTAxisBottom) return TRUE;										// Don't retry.
	//// 0.2 Set Sync Zero Line Axis with Data Line Axis
	//double dMin = 0, dMax = 0;
	//m_pTAxisLeft->GetMinMax(	dMin,	dMax);
	//if( fabs( dMin) < FLT_EPSILON * 10 && fabs( dMax) < FLT_EPSILON * 10) return FALSE;	// Retry.
	//m_pTAxisRight->SetMinMax(	dMin,	dMax);
	//// 0.2.5 Sync Horizontal Axis
	//m_pTAxisBottom->GetMinMax(	dMin,	dMax);
	//m_pTAxisTop->SetMinMax(		dMin,	dMax);
	//// 0.3 Set Left Axis to No-Automatic for Perect Sync with Right Axis (Auto & Not-Auto is not Same.)
	//m_pTAxisLeft->SetAutomatic( FALSE, FALSE);

	//// 1. Show Horizontal Axis Line in Zero Y with Series
	//if( !m_pTGraphZero) return TRUE;														// Don't retry.
	//m_pTGraphZero->AddData( -FLT_MAX, 0);
	//m_pTGraphZero->AddData( FLT_MAX, 0);
	//m_pTGraphZero->SetAxis( m_pTAxisTop);
	//m_pTGraphZero->SetAxis( m_pTAxisRight);
	return TRUE;																			// Don't retry.
}


/////////////////////////////////////////////////////////////////////////////
// CEarningsRatioChartAccumulation Chart
/////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2005/9/27
// Modifier		: 
// Comments		: Constructor
//-----------------------------------------------------------------------------
CDlgSooikChart::CEarningsRatioChartAccumulation::CEarningsRatioChartAccumulation()
{
	// (2005/9/23 - Seung-Won, Bae) Manage Axis
	m_pTAxisLeft	= NULL;	
	m_pTAxisRight	= NULL;	
	m_pTAxisTop		= NULL;	
	m_pTAxisBottom	= NULL;	

	// (2005/9/26 - Seung-Won, Bae) Manage Zero Line for Adding before Other Data Line
	m_pTGraphZero = NULL;
}

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2005/9/27
// Modifier		: 
// Comments		: Create Chart Control
//-----------------------------------------------------------------------------
BOOL CDlgSooikChart::CEarningsRatioChartAccumulation::CreateChart( CWnd *p_pParent, CRect p_rect, UINT p_uID)
{
	//if( !p_pParent) return FALSE;

	//p_rect.DeflateRect( 5, 5);
	//m_chartEzp.CreateControl( p_rect, p_pParent, p_uID, "´©Àû¼öÀÍ·ü");
	//m_chartEzp.SetBackgroundColor( CLR_BACKGROUND);
	//// 4. Show Side Scale Area
	//m_chartEzp.ShowOuterAxis( TRUE, FALSE, FALSE, TRUE);
	//// 5. Manage Top, Right Axis for Horizontal Axis's Centering
	//m_pTAxisLeft	= m_chartEzp.GetAxis( CTAxis::m_eATypeLeft);
	//m_pTAxisTop		= m_chartEzp.GetAxis( CTAxis::m_eATypeTop);
	//m_pTAxisRight	= m_chartEzp.GetAxis( CTAxis::m_eATypeRight);
	//m_pTAxisBottom	= m_chartEzp.GetAxis( CTAxis::m_eATypeBottom);
	//if( !m_pTAxisLeft || !m_pTAxisTop || !m_pTAxisRight	|| !m_pTAxisBottom) return FALSE;
	//// 6. Set Graph Indent
	//m_pTAxisLeft->SetPosition(		0, 5, 95, 0);
	//m_pTAxisRight->SetPosition(		0, 5, 95, 0);
	return TRUE;
}
//-----------------------------------------------------------------------------
// Author		: Han-Doo, Kim	Date :2005/10/14
// Modifier		: 
// Comments		: Remove All Graph
//-----------------------------------------------------------------------------
void CDlgSooikChart::CEarningsRatioChartAccumulation::RemoveGraph()
{
	/*m_chartEzp.RemoveAllGraph();
	if( m_pTAxisLeft && m_pTAxisTop && m_pTAxisRight && m_pTAxisBottom) 
	{
		m_pTAxisBottom->SetAutomatic(TRUE, TRUE);
		m_pTAxisRight->SetAutomatic(TRUE, TRUE);
		m_pTAxisLeft->SetAutomatic(TRUE,TRUE);
		m_pTAxisTop->SetAutomatic(TRUE,TRUE);
	}*/
}
//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2005/9/27
// Modifier		: 
// Comments		: Show Full Chart
//-----------------------------------------------------------------------------
void CDlgSooikChart::CEarningsRatioChartAccumulation::ShowChart( int p_nSection, CDlgOptimizer *pOptmizer)
{
//	// 0. Init Chart
//	// 0.1 Remove All Graph of previous Chart Type
//	m_chartEzp.RemoveAllGraph();
//	// 0.2 Add Zero Line
//	m_pTGraphZero = m_chartEzp.AddLineGraph( 1);
//	if( !m_pTGraphZero) return;
//	// 0.3 Set Left Axis Auto Range for getting Min/Max
//	if( !m_pTAxisLeft) return;
//	m_pTAxisLeft->SetAutomatic( TRUE, TRUE);
//
//	// 1. Get the Graph Count
////	if( !p_pInput) return;
////	int nGraphCount = p_pInput->m_stDataIndex.GetSize();
////	if( nGraphCount <= p_nSection) return;
//
//	// 2. Add Graphs
//	// 2.1 Get Data Count
////	p_pInput->SetCalculateData( p_nSection);
//	CIStrategyItem *pStrategyItem =pOptmizer->GetStrategyItem();
//	if( !pStrategyItem) return;
//	long lDataCount = pStrategyItem->GetCntStrategyResult();
//	if( lDataCount <= 0) return;
//		
//	// 2.2 Add One Graph for Each Section
//	CTGraph *pGraph = m_chartEzp.AddLineGraph( 2);
//	if( !pGraph) return;
//		
//	// (2005/9/27 - Seung-Won, Bae) Add Bar Graph for X Scale Sync with Case Cahrt
//	CTGraph *pGraphBar = m_chartEzp.AddBarGraph();
//	if( !pGraphBar) return;
//
//	// 3. Set Graph Data
//	double dX = 0;
//	CStrategyResult *pStrategyResult = NULL;
//	for( long i = 0; i < lDataCount; i++)
//	{
//		pStrategyResult = pStrategyItem->GetStrategyResult( i);
//		if( !pStrategyResult) continue;
//		if( CStrategyResult::EXITSHORT != pStrategyResult->m_DealType && CStrategyResult::EXITLONG != pStrategyResult->m_DealType) continue;
//
//		pGraph->AddData( ++dX, pStrategyResult->m_dProfitRateAccmulated);
//
//		// (2005/9/27 - Seung-Won, Bae) Add Bar Graph for X Scale Sync with Case Cahrt
//		pGraphBar->AddData( dX, 0);
//	}
//
//	// (2005/9/30 - Seung-Won, Bae) Show Zero Line
//	ShowZeroLine();
}

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2005/9/27
// Modifier		: 
// Comments		: Show Zero Line
//-----------------------------------------------------------------------------
BOOL CDlgSooikChart::CEarningsRatioChartAccumulation::ShowZeroLine( void)
{
	//// 0. Set Right & Top Axis Min/Max
	//// 0.1 Check NULL Pointer
	//if( !m_pTAxisLeft || !m_pTAxisBottom) return TRUE;										// Don't retry.
	//// 0.2 Set Sync Zero Line Axis with Data Line Axis
	//double dMin = 0, dMax = 0;
	//m_pTAxisLeft->GetMinMax(	dMin,	dMax);
	//if( fabs( dMin) < FLT_EPSILON * 10 && fabs( dMax) < FLT_EPSILON * 10) return FALSE;	// Retry.
	//m_pTAxisRight->SetMinMax(	dMin,	dMax);
	//// 0.2.5 Sync Horizontal Axis
	//m_pTAxisBottom->GetMinMax(	dMin,	dMax);
	//m_pTAxisTop->SetMinMax(		dMin,	dMax);
	//// 0.3 Set Left Axis to No-Automatic for Perect Sync with Right Axis (Auto & Not-Auto is not Same.)
	//m_pTAxisLeft->SetAutomatic( FALSE, FALSE);

	//// 1. Show Horizontal Axis Line in Zero Y with Series
	//if( !m_pTGraphZero) return TRUE;														// Don't retry.
	//m_pTGraphZero->AddData( -FLT_MAX, 0);
	//m_pTGraphZero->AddData( FLT_MAX, 0);
	//m_pTGraphZero->SetAxis( m_pTAxisTop);
	//m_pTGraphZero->SetAxis( m_pTAxisRight);
	return TRUE;																			// Don't retry.
}


/////////////////////////////////////////////////////////////////////////////
// CEarningsRatioChartSection Chart
/////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2005/9/27
// Modifier		: 
// Comments		: Constructor
//-----------------------------------------------------------------------------
CDlgSooikChart::CEarningsRatioChartSection::CEarningsRatioChartSection()
{
	// (2005/9/23 - Seung-Won, Bae) Manage Axis
	m_pTAxisLeft	= NULL;	
	m_pTAxisRight	= NULL;	
	m_pTAxisTop		= NULL;	
	m_pTAxisBottom	= NULL;	

	// (2005/9/26 - Seung-Won, Bae) Manage Zero Line for Adding before Other Data Line
	m_pTGraphZero = NULL;
}

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2005/9/27
// Modifier		: 
// Comments		: Create Chart Control
//-----------------------------------------------------------------------------
BOOL CDlgSooikChart::CEarningsRatioChartSection::CreateChart( CWnd *p_pParent, CRect p_rect, UINT p_uID)
{
	//if( !p_pParent) return FALSE;

	//p_rect.DeflateRect( 5, 5);
	//m_chartEzp.CreateControl( p_rect, p_pParent, p_uID);
	//m_chartEzp.SetBackgroundColor( CLR_BACKGROUND);
	//// 4. Show Side Scale Area
	//m_chartEzp.ShowOuterAxis( TRUE, FALSE, FALSE, TRUE);
	//// 5. Manage Top, Right Axis for Horizontal Axis's Centering
	//m_pTAxisLeft	= m_chartEzp.GetAxis( CTAxis::m_eATypeLeft);
	//m_pTAxisTop		= m_chartEzp.GetAxis( CTAxis::m_eATypeTop);
	//m_pTAxisRight	= m_chartEzp.GetAxis( CTAxis::m_eATypeRight);
	//m_pTAxisBottom	= m_chartEzp.GetAxis( CTAxis::m_eATypeBottom);
	//if( !m_pTAxisLeft || !m_pTAxisTop || !m_pTAxisRight	|| !m_pTAxisBottom) return FALSE;
	//// 6. Set Graph Indent
	//m_pTAxisLeft->SetPosition(		0, 5, 95, 0);
	//m_pTAxisTop->SetPosition(		0, 5, 95, 0);
	//m_pTAxisRight->SetPosition(		0, 5, 95, 0);
	//m_pTAxisBottom->SetPosition(	0, 5, 95, 0);
	return TRUE;
}
//-----------------------------------------------------------------------------
// Author		: Han-Doo, Kim	Date :2005/10/14
// Modifier		: 
// Comments		: Remove All Graph
//-----------------------------------------------------------------------------
void CDlgSooikChart::CEarningsRatioChartSection::RemoveGraph()
{
	//m_chartEzp.RemoveAllGraph();
	//
	//if( m_pTAxisLeft && m_pTAxisTop && m_pTAxisRight && m_pTAxisBottom) 
	//{
	//	m_pTAxisBottom->SetAutomatic(TRUE, TRUE);
	//	m_pTAxisRight->SetAutomatic(TRUE, TRUE);
	//	m_pTAxisLeft->SetAutomatic(TRUE,TRUE);
	//	m_pTAxisTop->SetAutomatic(TRUE,TRUE);
	//}
}
//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2005/9/27
// Modifier		: 
// Comments		: Show Full Chart
//-----------------------------------------------------------------------------
void CDlgSooikChart::CEarningsRatioChartSection::ShowFullChart(CDlgOptimizer *pOptmizer)
{
	//// 0. Init Chart
	//// 0.1 Remove All Graph of previous Chart Type
	//m_chartEzp.RemoveAllGraph();
	//// 0.2 Add Zero Line
	//m_pTGraphZero = m_chartEzp.AddLineGraph( 1);
	//if( !m_pTGraphZero) return;
	//// 0.3 Set Left Axis Auto Range for getting Min/Max
	//if( !m_pTAxisLeft) return;
	//m_pTAxisLeft->SetAutomatic( TRUE, TRUE);

	//// 1. Get Data Count

	//CIStrategyItem *pStrategyItem = pOptmizer->GetStrategyItem();
	//if( !pStrategyItem) return;
	//long lDataCount = pStrategyItem->GetCntStrategyResult();
	//if( lDataCount <= 0) return;
	//
	//// 2. Add One Graph for Full Section
	//CTGraph *pGraph = m_chartEzp.AddLineGraph( 2);
	//if( !pGraph) return;

	//// 3. Set Graph Data
	//double dX = 0;
	//CStrategyResult *pStrategyResult = NULL;
	//for( long i = 0; i < lDataCount; i++)
	//{
	//	pStrategyResult = pStrategyItem->GetStrategyResult( i);
	//	if( !pStrategyResult) continue;
	//	if( CStrategyResult::EXITSHORT != pStrategyResult->m_DealType && CStrategyResult::EXITLONG != pStrategyResult->m_DealType) continue;

	//	pGraph->AddData( ++dX, pStrategyResult->m_dProfitRateAccmulated);
	//}

	//// (2005/9/30 - Seung-Won, Bae) Show Zero Line
	//ShowZeroLine();
}

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2005/9/27
// Modifier		: 
// Comments		: Show Partial Chart
//-----------------------------------------------------------------------------
void CDlgSooikChart::CEarningsRatioChartSection::ShowPartialChart(CDlgOptimizer* pOptmizer)
{
//	// 0. Init Chart
//	// 0.1 Remove All Graph of previous Chart Type
//	m_chartEzp.RemoveAllGraph();
//	// 0.2 Add Zero Line
//	m_pTGraphZero = m_chartEzp.AddLineGraph( 1);
//	if( !m_pTGraphZero) return;
//	// 0.3 Set Left Axis Auto Range for getting Min/Max
//	if( !m_pTAxisLeft) return;
//	m_pTAxisLeft->SetAutomatic( TRUE, TRUE);
//	CIStrategyItem *pStrategyItem = pOptmizer->GetStrategyItem(); 
//	// 1. Get the Graph Count
////	if( !p_pInput) return;
////	int nGraphCount =6;// p_pInput->m_stDataIndex.GetSize();
////	if( nGraphCount <= 1) return;
//		
//	// 1.5 Add Time Graph
//	CTGraph *pGraphTime = m_chartEzp.AddLineGraph( 1, 0, FALSE);
//	if( !pGraphTime) return;
//
//	// 2. Add Graphs
//	double dX = 0, dXNext = 0;
////	for( int j = 1; j < nGraphCount; j++)		// Skip Full Section (index 0)
////	{
//		// 2.1 Get Data Count
//	//	p_pInput->SetCalculateData( j);
//
//	
//	long lDataCount = pStrategyItem->GetCntStrategyResult();
////	if( lDataCount <= 0) continue;
//	
//	// 2.2 Add One Graph for Each Section
//	CTGraph *pGraph = m_chartEzp.AddLineGraph( 2, CLR_BACKGROUND);
//	if( !pGraph) return;
//
//	// 3. Set Graph Data
//	BOOL bAfterFirstData = FALSE;
//	CStrategyResult *pStrategyResult = NULL;
//	for( long i = 0; i < lDataCount; i++)
//	{
//		pStrategyResult = pStrategyItem->GetStrategyResult( i);
//		if( !pStrategyResult) continue;
//		if( CStrategyResult::EXITSHORT != pStrategyResult->m_DealType && CStrategyResult::EXITLONG != pStrategyResult->m_DealType) continue;
//
//		// (2005/9/26 - Seung-Won, Bae) Get X Position.
//		//		In first Data of CIStrategyItem, It will be found with Time.
//		//		In Other Case, Increase 1.
//		if( bAfterFirstData || !pGraphTime->GetXValueWithY( dXNext, pStrategyResult->m_dTime)) dXNext = 0;
//		if( dXNext) dX = dXNext;
//		else dX++;
//		bAfterFirstData = TRUE;
//		pGraphTime->AddData( dX, pStrategyResult->m_dTime);
//
//		pGraph->AddData( dX, pStrategyResult->m_dProfitRateAccmulated);
//	}
////	}
//
//	// (2005/9/30 - Seung-Won, Bae) Show Zero Line
//	ShowZeroLine();
}

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2005/9/27
// Modifier		: 
// Comments		: Show Zero Line
//-----------------------------------------------------------------------------
BOOL CDlgSooikChart::CEarningsRatioChartSection::ShowZeroLine( void)
{
	return FALSE; 
	// 0. Set Right & Top Axis Min/Max
	// 0.1 Check NULL Pointer
	if( !m_pTAxisLeft || !m_pTAxisBottom) return TRUE;										// Don't retry.
	// 0.2 Set Sync Zero Line Axis with Data Line Axis
	double dMin = 0, dMax = 0;
	m_pTAxisLeft->GetMinMax(	dMin,	dMax);
	if( fabs( dMin) < FLT_EPSILON * 10 && fabs( dMax) < FLT_EPSILON * 10) return FALSE;	// Retry.
	m_pTAxisRight->SetMinMax(	dMin,	dMax);
	// 0.2.5 Sync Horizontal Axis
	m_pTAxisBottom->GetMinMax(	dMin,	dMax);
	m_pTAxisTop->SetMinMax(		dMin,	dMax);
	// 0.3 Set Left Axis to No-Automatic for Perect Sync with Right Axis (Auto & Not-Auto is not Same.)
	m_pTAxisLeft->SetAutomatic( FALSE, FALSE);

	// 1. Show Horizontal Axis Line in Zero Y with Series
	if( !m_pTGraphZero) return TRUE;														// Don't retry.
	m_pTGraphZero->AddData( -FLT_MAX, 0);
	m_pTGraphZero->AddData( FLT_MAX, 0);
	m_pTGraphZero->SetAxis( m_pTAxisTop);
	m_pTGraphZero->SetAxis( m_pTAxisRight);
	return TRUE;																			// Don't retry.
}
/////////////////////////////
void CDlgSooikChart::SetStrategyItem(CIStrategyItem* pStrat)
{
	m_pStrategyItem = pStrat;
}
void CDlgSooikChart::SetOptimizer(CDlgOptimizer* Opt)
{
	m_pDlgOptimizer = Opt;
}

void CDlgSooikChart::RemoveAllGraph()
{
	return;
	m_chartSection.RemoveGraph();
	m_chartAccumulation.RemoveGraph();
	m_chartCase.RemoveGraph();
}


//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2005/9/23
// Modifier		: 
// Comments		: Update New Result Data
//-----------------------------------------------------------------------------
void CDlgSooikChart::GuganBtnUpdate( void)
{
	return;
	// 0. Get Query Section Index
//	if( !m_pInput) return;
//	CInfoRight *pInfoRight = m_pInput->m_pInfoRight;
//	if( !pInfoRight) return;
//	CTabResult *pTabResult = pInfoRight->m_Result;
//	if( !pTabResult) return;
	m_nQuerySection = 0;//pTabResult->m_iDateIndex;
	if( m_nQuerySection < 0) return;

	// 1. Show/Hide Control with Query Type
	BOOL bFullQuery = ( 0 == m_nQuerySection);
	CButton *pRdoFull		= ( CButton *)GetDlgItem( IDC_RTYPE2);
	CButton *pRdoPartial	= ( CButton *)GetDlgItem( IDC_RTYPE1);
	if( !pRdoFull || !pRdoPartial) return;
	pRdoFull->ShowWindow(				bFullQuery ? SW_SHOW : SW_HIDE);
	pRdoPartial->ShowWindow(			bFullQuery ? SW_SHOW : SW_HIDE);
	m_chartSection.ShowControl(			bFullQuery);
	m_chartAccumulation.ShowControl(	!bFullQuery);
	m_chartCase.ShowControl(			!bFullQuery);

	// 2. Show Query Data
	if( bFullQuery)
	{
		if( pRdoFull->GetCheck()) OnRType2();
		else
		{
			if( !pRdoPartial->GetCheck()) pRdoPartial->SetCheck( 1);
			OnRType1();
		}
	}
	else
	{
		// 2.5 Show Accumulation and Case Chart Data
		m_chartAccumulation.ShowChart( m_nQuerySection,m_pDlgOptimizer);
		m_chartCase.ShowChart( m_nQuerySection,m_pDlgOptimizer);

		// 4. Set Timer for Zero Line. (the Timer must be used for getting the Axis's Min/Max
		SetTimer( 926, 100, NULL);
	}
}
HBRUSH CDlgSooikChart::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	
	// TODO: Return a different brush if the default is not desired
	pDC->SetBkColor( RGB(255,255,255));//CLR_BACKGROUND);
	return m_brBackground;
}

void CDlgSooikChart::ShowData1Chart(long dwValue)
{
	CArray<double , double> * dValue = (CArray<double,double>*)dwValue;
	COLORREF nRed, nBlue, clr;
	nRed = RGB(255, 000, 000);
	nBlue = RGB(000, 000, 255);

	double dwValue1;
	for (int AX = 0; AX < dValue->GetSize(); AX++){
		dwValue1 = dValue->GetAt(AX);
		clr = (dwValue1 < 0) ? nBlue : nRed;
		m_ctlBarTChart.Series(0).AddXY(AX, dwValue1, "", clr);
	}
}

void CDlgSooikChart::ShowData2Chart(CStringArray &stNujukArray, int nShowRow)
{
}
