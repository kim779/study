// DlgSonikChart.cpp : implementation file
//

#include "stdafx.h"
#include "DlgSonikChart.h"

#include <math.h>				// for floor() / ceil()
#include <float.h>				// for FLT_EPSILON, ...

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSonikChart dialog


CDlgSonikChart::CDlgSonikChart(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSonikChart::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSonikChart)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
		//{{AFX_DATA_INIT(CTabSonikChart)
	m_nRangeUnitWidth = 1;				// (2005/9/29 - Seung-Won, Bae) Set Default Width
	//}}AFX_DATA_INIT

	// (2005/9/21 - Seung-Won, Bae) Manage Brush for Background Color
	m_brBackground.CreateSolidBrush( RGB(255,255,255));//CLR_BACKGROUND);

	// (2005/9/23 - Seung-Won, Bae) Manage Right Axis to set a Graph.
	m_pTAxisRight	= NULL;
	// (2005/9/29 - Seung-Won, Bae) Manage Bottom Axis to reset scale.
	m_pTAxisBottom	= NULL;

	// (2005/9/29 - Seung-Won, Bae) Manage Bar Graph for Fill Zero Bar.
	m_pTGraphBar = NULL;
}

CDlgSonikChart::~CDlgSonikChart()
{
		m_brBackground.DeleteObject();
}
void CDlgSonikChart::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSonikChart)
	DDX_Control(pDX, IDC_STC_TOTAL_COUNT, m_stcTotalCount);
	DDX_Control(pDX, IDC_STC_LOSS_COUNT, m_stcLossCount);
	DDX_Control(pDX, IDC_STC_EARNING_RATIO, m_stcEarningRatio);
	DDX_Control(pDX, IDC_STC_EARNING_COUNT, m_stcEarningCount);
	DDX_Text(pDX, IDC_EDT_WIDTH, m_nRangeUnitWidth);
	DDV_MinMaxInt(pDX, m_nRangeUnitWidth, 1, 100);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSonikChart, CDialog)
	//{{AFX_MSG_MAP(CDlgSonikChart)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSonikChart message handlers

BOOL CDlgSonikChart::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
//		GetDlgItem( IDC_BTN_FOCUS_CATCHER)->MoveWindow( -100, -100, 1, 1);

	// (2005/9/22 - Seung-Won, Bae) Create Control
	//// 1. Get Client Area
	//CRect rctClient;
	//GetClientRect( &rctClient);
	//// 2. Get Chart Area
	//rctClient.right -= 6;	// why ? Border Style ? 
	//rctClient.bottom -= 6;	// why ? Border Style ? 
	//// 3. Create Section Chart
	//rctClient.top += 25;
	//rctClient.DeflateRect( 5, 5);
	//m_chartEzp.CreateControl( rctClient, this, 928);
	//m_chartEzp.SetBackgroundColor( CLR_BACKGROUND);
	//// 4. Show Side Scale Area
	//m_chartEzp.ShowOuterAxis( FALSE, FALSE, TRUE, TRUE);
	//// (2005/9/29 - Seung-Won, Bae) Manage Bottom Axis to reset scale.
	//m_pTAxisBottom	= m_chartEzp.GetAxis( CTAxis::m_eATypeBottom);
	//m_pTAxisBottom->SetScaleUnit( m_nRangeUnitWidth);
	//// (2005/9/23 - Seung-Won, Bae) Manage Right Axis to set a Graph.
	//m_pTAxisRight	= m_chartEzp.GetAxis( CTAxis::m_eATypeRight);
	//if( !m_pTAxisRight) return FALSE;
	//m_pTAxisRight->SetPosition(		0, 5, 100, 0);
	//m_pTAxisRight->SetScaleUnit( 1);
	//// (2005/9/28 - Seung-Won, Bae) Set Axis Title with Sub Title
	//m_chartEzp.SetTitle( "Á¾¸ñ¼ö",	CEzpTChartCtrl::m_eTTypeSubHeader,	CEzpTChartCtrl::m_eTAlignRight);
	//m_chartEzp.SetTitle( "%",		CEzpTChartCtrl::m_eTTypeSubFooter,	CEzpTChartCtrl::m_eTAlignCustom,	CPoint( 0, rctClient.bottom - 64));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CDlgSonikChart::GuganBtnUpdate()
{
	//CIStrategyItem *pStrategyItem = m_pStrategyItem;
	//if( !pStrategyItem) return;
	//long lDataCount = pStrategyItem->GetCntStrategyResult();
	//if( lDataCount <= 0) return;
	//	
	//// 2.2 Add One Graph for Each Section
	//m_pTGraphBar = m_chartEzp.AddBarGraph();
	//if( !m_pTGraphBar) return;
	//// (2005/9/28 - Seung-Won, Bae) Set Right Axis
	//m_pTGraphBar->SetAxis( m_pTAxisRight);

	//// (2005/9/30 - Seung-Won, Bae) Show Statistic
	//int nLossCount = 0;
	//int nEarningCount = 0;
	//int nZeroCount = 0;

	//// 3. Set Graph Data
	//COLORREF clrBar;
	//long nDataIndex;
	//double dX = 0, dY = 0;
	//CStrategyResult *pStrategyResult = NULL;
	//for( long i = 0; i < lDataCount; i++)
	//{
	//	pStrategyResult = pStrategyItem->GetStrategyResult( i);
	//	if( !pStrategyResult) continue;
	//	if( CStrategyResult::EXITSHORT != pStrategyResult->m_DealType && CStrategyResult::EXITLONG != pStrategyResult->m_DealType) continue;

	//	// (2005/9/29 - Seung-Won, Bae) Make Ratio Unit.
	//	dX = pStrategyResult->m_dProfitRate / m_nRangeUnitWidth;
	//	if( dX < 0) dX = floor( dX);
	//	else dX = ceil( dX);
	//	dX = dX * m_nRangeUnitWidth;

	//	// (2005/9/30 - Seung-Won, Bae) Show Statistic
	//	if(			dX < 0)	nLossCount++;
	//	else if(	dX > 0)	nEarningCount++;
	//	else				nZeroCount++;

	//	nDataIndex = m_pTGraphBar->GetYValueWithX( dY, dX);
	//	if( nDataIndex < 0)
	//	{
	//		// (2005/9/30 - Seung-Won, Bae) Set Color for Ratio
	//		if(			dX < 0)	clrBar = RGB(   0, 0, 255);
	//		else if(	dX > 0)	clrBar = RGB( 255, 0,   0);
	//		else				clrBar = RGB(   0, 0,   0);

	//		m_pTGraphBar->AddData( dX, 1, clrBar);
	//	}
	//	else m_pTGraphBar->SetData( nDataIndex, dX, dY + 1);
	//}

	//// 0.2 Set Sync Zero Line Axis with Data Line Axis
	//double dMin = 0, dMax = 0;
	//m_pTAxisBottom->GetMinMax(	dMin,	dMax);
	//dMax = max( -dMin, dMax);
	//dMin = -dMax;
	//m_pTAxisBottom->SetMinMax(	dMin - m_nRangeUnitWidth,	dMax + m_nRangeUnitWidth);

	//// (2005/9/29 - Seung-Won, Bae) Add Zero Value for the gab between bars.
	//// 1. Get Min Data
	//for( dX = dMin - m_nRangeUnitWidth; dX < dMax + m_nRangeUnitWidth; dX += m_nRangeUnitWidth)
	//	if( m_pTGraphBar->GetYValueWithX( dY, dX) < 0) m_pTGraphBar->AddData( dX, 0);

	//// (2005/9/29 - Seung-Won, Bae) Set Max of Right Axis to 10 (Default View)
	//m_pTAxisRight->GetMinMax( dMin, dMax);
	//if( dMax < 10) m_pTAxisRight->SetMinMax( dMin, 10);

	//// (2005/9/30 - Seung-Won, Bae) Show Statistic
	//CString strData;
	//strData.Format( "%d", nLossCount + nEarningCount + nZeroCount);
	//m_stcTotalCount.SetWindowText( strData);
	//strData.Format( "%d", nEarningCount);
	//m_stcEarningCount.SetWindowText( strData);
	//strData.Format( "%d", nLossCount);
	//m_stcLossCount.SetWindowText( strData);
	//strData.Format( "%1.2f %%", nEarningCount * 100. / ( nLossCount + nEarningCount + nZeroCount));
	//m_stcEarningRatio.SetWindowText( strData);
}

HBRUSH CDlgSonikChart::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
		pDC->SetBkColor( RGB(255,255,255));//CLR_BACKGROUND);
	// TODO: Return a different brush if the default is not desired
	return m_brBackground;
}

void CDlgSonikChart::SetStrategyItem(CIStrategyItem* pStrat)
{
	m_pStrategyItem = pStrat;
}
void CDlgSonikChart::SetOptimizer(CDlgOptimizer* Opt)
{
	m_pDlgOptimizer = Opt;
}
void CDlgSonikChart::RemoveAllGraph()
{
	/*m_chartEzp.RemoveAllGraph();
	if( !m_pTAxisBottom || !m_pTAxisRight) return;
	m_pTAxisBottom->SetAutomatic(	TRUE, TRUE);
	m_pTAxisRight->SetAutomatic(	TRUE, TRUE);

	m_stcTotalCount.SetWindowText( "");
	m_stcEarningCount.SetWindowText( "");
	m_stcLossCount.SetWindowText( "");
	m_stcEarningRatio.SetWindowText( "");*/
}
