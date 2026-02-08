// Setting_Price.cpp : implementation file
//

#include "stdafx.h"
#include "ChartEnvSetupDlgAddIn.h"
#include "Setting_Price.h"

#include "GraphData.h"
#include "GraphSetting.h"
#include "DlgMainTabIndicatorSetUp.h"
#include "TabMainChartEnvSetUp.h"
#include "DlgEnvSetUpAll.h"
#include "Conversion.h"

// 2008.01.25 by LYH >>
#include "../Include_Chart/Dll_Load/IMetaTable.h"	// for _MTOL0()
// 2008.01.25 by LYH <<

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetting_Price dialog

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 22
// Modifier		: 
// Comments		: 
//-----------------------------------------------------------------------------
CSetting_Price::CSetting_Price( IChartOCX *p_pIChartOCX, CWnd* pParent /*=NULL*/, CWnd *pTopWnd)
	: CBaseDialog( CSetting_Price::IDD, pParent, p_pIChartOCX, pTopWnd)
{
	//{{AFX_DATA_INIT(CSetting_Price)
	m_nSteadyType = -1;
	//}}AFX_DATA_INIT

	m_bUseBasePrice = FALSE;
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 22
// Modifier		: 
// Comments		: 
//-----------------------------------------------------------------------------
CSetting_Price::~CSetting_Price()
{
	if( m_pIChartManager)
	{
		m_pIChartManager->Release();
		m_pIChartManager = NULL;
	}
}


void CSetting_Price::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetting_Price)
	DDX_Control(pDX, IDC_BTN_DOWN, m_btnDown);
	DDX_Control(pDX, IDC_BTN_STEADY, m_btnSteady);
	DDX_Control(pDX, IDC_BTN_UP, m_btnUp);
	DDX_Control(pDX, IDC_THICKNESS_STATIC, m_Thickness_Static);
	DDX_Control(pDX, IDC_DRAWSTYLE_STATIC, m_DrawStyle_Static);
	DDX_Control(pDX, IDC_FILL_DOWN, m_FillDown_Bt);
	DDX_Control(pDX, IDC_FILL_UP, m_FillUp_Bt);
	DDX_Control(pDX, IDC_INVERT, m_InvertChart_Bt);
	DDX_Control(pDX, IDC_LOG, m_Log_Bt);
	DDX_Control(pDX, IDC_THICKNESS, m_Thickness_Combo);
	DDX_Control(pDX, IDC_DRAWSTYLE, m_DrawStyle_Combo);
	DDX_Radio(pDX, IDC_STEADY_NORMAL, m_nSteadyType);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSetting_Price, CBaseDialog)
	//{{AFX_MSG_MAP(CSetting_Price)
	ON_BN_CLICKED(IDC_EQUIVOLUME_CHART, OnEquiVolumeChart)
	ON_BN_CLICKED(IDC_CANDLEVOLUME_CHART, OnCandleVolumeChart)
	ON_BN_CLICKED(IDC_FLOW_CHART, OnFlowChart)
	ON_BN_CLICKED(IDC_NEWBAR_CHART, OnNewBarChart)
	ON_BN_CLICKED(IDC_BAR_CHART, OnBarChart)
	ON_BN_CLICKED(IDC_BONG_CHART, OnBongChart)
	ON_BN_CLICKED(IDC_LINE_CHART, OnLineChart)
	ON_BN_CLICKED(IDC_STEADY_NORMAL, OnSteadyNormal)
	ON_BN_CLICKED(IDC_STEADY_BEFORECLOSE, OnSteadyBeforeclose)
	ON_BN_CLICKED(IDC_STEADY_SAMECLOSE, OnSteadySameclose)
	ON_BN_CLICKED(IDC_BTN_UP, OnBtnUp)
	ON_BN_CLICKED(IDC_BTN_DOWN, OnBtnDown)
	ON_BN_CLICKED(IDC_BTN_STEADY, OnBtnSteady)
	ON_CBN_SELCHANGE(IDC_THICKNESS, OnSelchangeThickness)
	ON_CBN_SELCHANGE(IDC_DRAWSTYLE, OnSelchangeDrawstyle)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetting_Price message handlers

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 22
// Modifier		: 
// Comments		: 
//-----------------------------------------------------------------------------
BOOL CSetting_Price::OnInitDialog() 
{
	CBaseDialog::OnInitDialog();

	if( !m_pIGraphInfo) return TRUE;
	Initial( NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 22
// Return void : 
// Param  IGraphInfo* p_pIGraphInfo : 
// Comments		: 
//-----------------------------------------------------------------------------
void CSetting_Price::Initial(IGraphInfo *p_pIGraphInfo)
{
	if( m_pIGraphInfo)
	{
		OnApply( m_pIGraphInfo);
		m_pIGraphInfo->Release();
	}
	m_pIGraphInfo = p_pIGraphInfo;
	if( m_pIGraphInfo) m_pIGraphInfo->AddRef();
	else return;

	// (2009/9/5 - Seung-Won, Bae) Hide Equi/Candle Volume type on FX Mode
	if( CChartInfo::FX == m_pIChartOCX->GetChartMode())
	{
		GetDlgItem( IDC_CANDLEVOLUME_CHART)->ShowWindow( SW_HIDE);
		GetDlgItem( IDC_EQUIVOLUME_CHART)->ShowWindow( SW_HIDE);
	}

	// (2007/12/17 - Seung-Won, Bae) Check Base Price for Up/Down Color in LineType
	ILPCSTR szGraphName = m_pIGraphInfo->GetGraphNameString();
	ILPCSTR szPacketNames = m_pIChartManager->GetPacketNameStringOfGraph( szGraphName);
	CString strPacketNames( szPacketNames);
	strPacketNames.TrimRight( ',');
	int nIndex = strPacketNames.ReverseFind( ',');
	if( 0 <= nIndex) strPacketNames = strPacketNames.Mid( nIndex + 1);
	m_bUseBasePrice = FALSE;
	IPacket *pPacket = m_pIPacketManager->GetPacket( strPacketNames);
	if( pPacket)
	{
		double dBasePrice = 0.0;
		m_bUseBasePrice = pPacket->GetStandardValue( dBasePrice);
		pPacket->Release();
	}

	int nR = -1, nC = -1, nV = -1, nG = -1;
	if( m_pIChartManager->GetIndexOfGraph( szGraphName, nR, nC, nV, nG))
	{
		IBlock *pIBlock = m_pIChartManager->GetBlock( nR, nC);
		if( pIBlock)
		{
			m_bOrgLog = pIBlock->IsLogScale();
			m_bOrgInvertChart = pIBlock->IsVScaleInvert( nV);
			pIBlock->Release();
		}
	}

	Initial_ThicknessComboBox( 0, 0, m_Thickness_Combo);	
	Initial_DrawStyleComboBox( 0, m_DrawStyle_Combo);

	IDoubleList *pIDrawCondition = m_pIGraphInfo->GetDrawConditionList();
	if( !pIDrawCondition) return;
	double dSteadyType = -1;
	if( 0 < pIDrawCondition->GetCount()) pIDrawCondition->GetHead( dSteadyType);
	pIDrawCondition->Release();
	if( dSteadyType < 1) return;	// 1:General, 2:BasePrice, 3:Same Close.
	m_nSteadyType = ( int)dSteadyType - 1;

	Set_DrawStyle();
	Initial_ColorButton();

	m_InvertChart_Bt.SetCheck( m_bOrgInvertChart);
	m_Log_Bt.SetCheck( m_bOrgLog);

	m_DrawStyle_Combo.SetCurSel( m_pIGraphInfo->GetSubGraphDrawStyle( 0));
	m_Thickness_Combo.SetCurSel( m_pIGraphInfo->GetSubGraphPenThickness( 0, 1) - 1);

	Initial_FillUp( 0, m_FillUp_Bt);
	Initial_FillDown( 0, m_FillDown_Bt);

	ShowHide_FillUpDownNCombo( m_pIGraphInfo->GetSubGraphStyle( 0));
	
	UpdateData(FALSE);
}



//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 22
// Return void : 
// Param  IGraphInfo *p_pIGraphInfo : 
// Param  CSubGraphData *pSubGraphData : 
// Comments		: 
//-----------------------------------------------------------------------------
void CSetting_Price::Initial_ColorButton( void)
{
	// (2006/8/23 - Seung-Won, Bae) Color setting alwaysly
	m_btnUp.SetColor( m_pIGraphInfo->GetSubGraphColor( 0, 1));
	m_btnUp.Invalidate();
	m_btnDown.SetColor( m_pIGraphInfo->GetSubGraphColor( 0, 3));
	m_btnDown.Invalidate();
	m_btnSteady.SetColor( m_pIGraphInfo->GetSubGraphColor( 0, 5));
	m_btnDown.Invalidate();

	ML_SET_LANGUAGE_RES();
	CString strColor, strUp;
	strColor.LoadString(IDS_STRING_COLOR);
	strUp.LoadString(IDS_STRING_UP);

	// (2007/12/17 - Seung-Won, Bae) for Not-Candle Type
	int nSubGraphType = m_pIGraphInfo->GetSubGraphType( 0);
	if( CGraphBaseData::Bong_Type != nSubGraphType)
	{
		int nSubGraphStyle = m_pIGraphInfo->GetSubGraphStyle( 0);

		m_btnUp.ShowWindow( SW_SHOW);
		GetDlgItem( IDC_STATIC_UP)->ShowWindow( SW_SHOW);
		if( ( CGraphBaseData::Line_Type == nSubGraphType && CGraphBaseData::ComparisonLine == nSubGraphStyle && m_bUseBasePrice) // 기준가 유무 확인
			|| ( CGraphBaseData::Bar_Type == nSubGraphType && CGraphBaseData::UpDownBar == nSubGraphStyle)
			// 고정비율상하식 추가 - ojtaso (20080603)
			|| ( CGraphBaseData::Bar_Type == nSubGraphType && CGraphBaseData::FixedRateUpDownBar == nSubGraphStyle))
		{
			GetDlgItem( IDC_STATIC_DOWN)->ShowWindow( SW_SHOW);
			m_btnDown.ShowWindow( SW_SHOW);
		}
		else
		{
// 2008.01.25 by LYH >>
			//GetDlgItem( IDC_STATIC_UP)->SetWindowText( "색상");
			GetDlgItem( IDC_STATIC_UP)->SetWindowText( strColor);
// 2008.01.25 by LYH <<
			GetDlgItem( IDC_STATIC_DOWN)->ShowWindow( SW_HIDE);
			m_btnDown.ShowWindow( SW_HIDE);
		}
		GetDlgItem( IDC_STATIC_STEADY)->ShowWindow( SW_HIDE);
		m_btnSteady.ShowWindow( SW_HIDE);

		GetDlgItem( IDC_STEADY_NORMAL)->EnableWindow( FALSE);
		GetDlgItem( IDC_STEADY_BEFORECLOSE)->EnableWindow( FALSE);
		GetDlgItem( IDC_STEADY_SAMECLOSE)->EnableWindow( FALSE);
		return;
	}

	// (2006/12/7 - Seung-Won, Bae) Check 1 Tick Chart (Disable All Selection)
	if( m_pIPacketManager->GetBaseTimeUnit() == 1 && CScaleBaseData::HORZ_TICK == m_pIPacketManager->GetBaseTimeDataType())
	{
		GetDlgItem( IDC_STATIC_UP)->ShowWindow( SW_SHOW);
		m_btnUp.ShowWindow( SW_SHOW);

		if( m_bUseBasePrice) // 기준가 유무 확인
		{
			GetDlgItem( IDC_STATIC_DOWN)->ShowWindow( SW_SHOW);
			m_btnDown.ShowWindow( SW_SHOW);
		}
		else
		{
// 2008.01.25 by LYH >>
			//GetDlgItem( IDC_STATIC_UP)->SetWindowText( "색상");
			GetDlgItem( IDC_STATIC_UP)->SetWindowText( strColor);
// 2008.01.25 by LYH <<
			GetDlgItem( IDC_STATIC_DOWN)->ShowWindow( SW_HIDE);
			m_btnDown.ShowWindow( SW_HIDE);
		}
		GetDlgItem( IDC_STATIC_STEADY)->ShowWindow( SW_HIDE);
		m_btnSteady.ShowWindow( SW_HIDE);

		GetDlgItem( IDC_STEADY_NORMAL)->EnableWindow( FALSE);
		GetDlgItem( IDC_STEADY_BEFORECLOSE)->EnableWindow( FALSE);
		GetDlgItem( IDC_STEADY_SAMECLOSE)->EnableWindow( FALSE);
		return;
	}

	// 선 Style이면
// 2008.01.25 by LYH >>
	//GetDlgItem(IDC_STATIC_UP)->SetWindowText( "상승");
	GetDlgItem(IDC_STATIC_UP)->SetWindowText( strUp);
// 2008.01.25 by LYH <<
	if( m_pIGraphInfo->GetSubGraphStyle( 0) == 2)
	{
		GetDlgItem(IDC_STATIC_STEADY)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_STEADY)->ShowWindow(SW_HIDE);

		if( m_bUseBasePrice)
		{
	// (2006.11.03 안승환) Flow차트 추가
			GetDlgItem(IDC_STATIC_UP)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_UP)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_DOWN)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_DOWN)->ShowWindow(SW_SHOW);
	// (2006.11.03 안승환) Flow차트 추가
		}
		else
		{
			GetDlgItem(IDC_STATIC_UP)->ShowWindow(		SW_SHOW);
// 2008.01.25 by LYH >>
			//GetDlgItem(IDC_STATIC_UP)->SetWindowText( "색상");
			GetDlgItem(IDC_STATIC_UP)->SetWindowText( strColor);
// 2008.01.25 by LYH <<
			GetDlgItem(IDC_BTN_UP)->ShowWindow(			SW_SHOW);
			GetDlgItem(IDC_STATIC_DOWN)->ShowWindow(	SW_HIDE);
			GetDlgItem(IDC_BTN_DOWN)->ShowWindow(		SW_HIDE);
		}
	}
// (2006.11.03 안승환) Flow차트 추가
	else if(m_pIGraphInfo->GetSubGraphStyle( 0) == 5)
	{
		GetDlgItem(IDC_STATIC_STEADY)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BTN_STEADY)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_UP)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_UP)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_DOWN)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_DOWN)->ShowWindow(SW_HIDE);
	}
// (2006.11.03 안승환) Flow차트 추가

	else
	{
// (2006.11.03 안승환) Flow차트 추가
		GetDlgItem(IDC_STATIC_UP)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BTN_UP)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_DOWN)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BTN_DOWN)->ShowWindow(SW_SHOW);
// (2006.11.03 안승환) Flow차트 추가
		// 보합기준에 따라 보합색상 버튼을 표시해준다.
		if( m_nSteadyType == 0 )
		{
			GetDlgItem(IDC_STATIC_STEADY)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_STEADY)->ShowWindow(SW_SHOW);
		}
			
		else
		{
			GetDlgItem(IDC_STATIC_STEADY)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_STEADY)->ShowWindow(SW_HIDE);
		}
	}
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 22
// Return void	: 
// Param  IGraphInfo *p_pIGraphInfo : 
// Comments		: 
//-----------------------------------------------------------------------------
// 20081014 박동관 >>
BOOL CSetting_Price::OnApply( IGraphInfo *p_pIGraphInfo)
//void CSetting_Price::OnApply( IGraphInfo *p_pIGraphInfo)
// 20081014 박동관 <<
{
	if(p_pIGraphInfo == NULL)
		return FALSE;

	UpdateData(TRUE);

	// draw condition
	IDoubleList *pIDrawCondition = m_pIGraphInfo->GetDrawConditionList();
	if( pIDrawCondition)
	{
		POSITION psnCondition = pIDrawCondition->GetHeadPosition();
		if( psnCondition) pIDrawCondition->SetAt( psnCondition, m_nSteadyType + 1);
		pIDrawCondition->Release();
	}

	OnApply_ColorButton();
	
	// 봉, 바차트에서
	OnApply_FillUp( 0, m_FillUp_Bt);
	OnApply_FillDown( 0, m_FillDown_Bt);

	// 라인차트에서 설정했던 값을 적용
	OnApply_DrawStyle( 0, m_DrawStyle_Combo);
	OnApply_Thickness( 0, m_Thickness_Combo);

	if( !strcmp( GetParent()->GetRuntimeClass()->m_lpszClassName, "CGraphSetting"))
	{
		// GraphSetting을 띄운 경우라면 CIndicatorConfigurationChangedData에 Setting을 하게 되므로 CMain에 엑세스한다.
		((CGraphSetting*)GetParent())->SetLogState(itob(m_Log_Bt.GetCheck()));
		((CGraphSetting*)GetParent())->SetInvertScaleType(itob(m_InvertChart_Bt.GetCheck()));
	}
	else
	{
		ILPCSTR szGraphName = m_pIGraphInfo->GetGraphNameString();
		int nR = -1, nC = -1, nV = -1, nG = -1;
		if( m_pIChartManager->GetIndexOfGraph( szGraphName, nR, nC, nV, nG))
		{
			IBlock *pIBlock = m_pIChartManager->GetBlock( nR, nC);
			if( pIBlock)
			{
				pIBlock->SetLogScale( m_Log_Bt.GetCheck());
				pIBlock->SetVScaleInvert( nV, m_InvertChart_Bt.GetCheck());
				pIBlock->Release();
			}
		}

		if( ((CDlgMainTabIndicatorSetUp *)GetParent())->m_bSave == TRUE 
			&& ((CDlgMainTabIndicatorSetUp *)GetParent())->m_bPreview == FALSE ) 
		{
			m_bOrgInvertChart = itob( m_InvertChart_Bt.GetCheck() );
			m_bOrgLog = itob( m_Log_Bt.GetCheck() );
		}
	}

	p_pIGraphInfo->GetCopyDataFrom(m_pIGraphInfo);

	return TRUE;
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 22
// Return void : 
// Param  IGraphInfo *p_pIGraphInfo : 
// Param  CSubGraphData *pSubGraphData : 
// Comments		: 
//-----------------------------------------------------------------------------
void CSetting_Price::OnApply_ColorButton()
{
	COLORREF crColor = m_btnUp.GetColor();
	m_pIGraphInfo->SetSubGraphColor( 0, 1, crColor);
	m_pIGraphInfo->SetSubGraphColor( 0, 2, crColor);

	crColor = m_btnDown.GetColor();
	m_pIGraphInfo->SetSubGraphColor( 0, 3, crColor);
	m_pIGraphInfo->SetSubGraphColor( 0, 4, crColor);

	if( m_pIGraphInfo->GetSubGraphStyle( 0) != 2 && m_pIGraphInfo->GetSubGraphStyle( 0) != 5)
	{
		IDoubleList *pIDrawCondition = m_pIGraphInfo->GetDrawConditionList();
		if( pIDrawCondition)
		{
			double dSteadyType = -1;
			if( 0 < pIDrawCondition->GetCount()) pIDrawCondition->GetHead( dSteadyType);
			pIDrawCondition->Release();
			if( ( int)dSteadyType == 1)		// 1:General, 2:BasePrice, 3:Same Close.
			{
				crColor = m_btnSteady.GetColor();
				m_pIGraphInfo->SetSubGraphColor( 0, 5, crColor);
				m_pIGraphInfo->SetSubGraphColor( 0, 6, crColor);
			}
		}
	}
// (2006.11.03 안승환) Flow차트 추가
	if( m_pIGraphInfo->GetSubGraphStyle( 0) == 5 )
	{
		crColor = m_btnSteady.GetColor();
		m_pIGraphInfo->SetSubGraphColor( 0, 5, crColor);
		m_pIGraphInfo->SetSubGraphColor( 0, 6, crColor);
	}
// (2006.11.03 안승환) Flow차트 추가
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 22
// Modifier		: 
// Comments		: 차트의 타입에 따라 색깔 버튼을 보여준다.
//-----------------------------------------------------------------------------
void CSetting_Price::Set_DrawStyle()
{
	// (2007/12/17 - Seung-Won, Bae) for Not-Candle Type
	int nSubGraphType = m_pIGraphInfo->GetSubGraphType( 0);
	int nSubGraphStyle = m_pIGraphInfo->GetSubGraphStyle( 0);
	if( nSubGraphStyle < 0) return;
	if( CGraphBaseData::Bong_Type != nSubGraphType)
	{
		GetDlgItem( IDC_BONG_CHART)->EnableWindow(		FALSE);	
		GetDlgItem( IDC_LINE_CHART)->EnableWindow(		FALSE);	
		GetDlgItem( IDC_BAR_CHART)->EnableWindow(		FALSE);
		GetDlgItem( IDC_NEWBAR_CHART)->EnableWindow(	FALSE);
		GetDlgItem( IDC_FLOW_CHART)->EnableWindow(		FALSE);
		GetDlgItem( IDC_CANDLEVOLUME_CHART)->EnableWindow(		FALSE);
		GetDlgItem( IDC_EQUIVOLUME_CHART)->EnableWindow(		FALSE);
		( ( CButton *)GetDlgItem( IDC_NOT_CANDLE))->SetCheck( 1);
		CString strGraphType;
		strGraphType.Format( "%d", nSubGraphType);
		GetDlgItem( IDC_NOT_CANDLE_TYPE)->SetWindowText( strGraphType);
		CString strGraphStyle;
		strGraphStyle.Format( "%d", nSubGraphStyle);
		GetDlgItem( IDC_NOT_CANDLE_STYLE)->SetWindowText( strGraphStyle);
		return;
	}

	// (2007/1/15 - Seung-Won, Bae) Reset Sub Graph Style in Line Graph Type.
	ILPCSTR szGraphName = m_pIGraphInfo->GetGraphNameString();
	ILPCSTR szPacketNames = m_pIChartManager->GetPacketNameStringOfGraph( szGraphName);
	CString strPacketNames( szPacketNames);
	if( strPacketNames.Replace( ',', ' ') < 4)
	{
		GetDlgItem( IDC_BONG_CHART)->EnableWindow(		FALSE);	
		GetDlgItem( IDC_LINE_CHART)->EnableWindow(		FALSE);	
		GetDlgItem( IDC_BAR_CHART)->EnableWindow(		FALSE);
		GetDlgItem( IDC_NEWBAR_CHART)->EnableWindow(	FALSE);
		GetDlgItem( IDC_FLOW_CHART)->EnableWindow(		FALSE);
		GetDlgItem( IDC_CANDLEVOLUME_CHART)->EnableWindow(		FALSE);
		GetDlgItem( IDC_EQUIVOLUME_CHART)->EnableWindow(		FALSE);
	}

	CButton *pBongButton = (CButton *)GetDlgItem( IDC_BONG_CHART );	
	CButton *pBarButton = (CButton *)GetDlgItem( IDC_BAR_CHART );
	CButton *pLineButton = (CButton *)GetDlgItem( IDC_LINE_CHART );
// (2006.10.30 안승환) 바차트(고'저'종)추가
	CButton *pNewBarButton = (CButton *)GetDlgItem( IDC_NEWBAR_CHART );
// (2006.10.30 안승환) 바차트(고'저'종)추가
// (2006.11.03 안승환) Flow차트 추가
	CButton *pFlowButton = (CButton *)GetDlgItem( IDC_FLOW_CHART );
// (2006.11.03 안승환) Flow차트 추가
	
//{{ (2007.05.29 LYH) CandleVolume, EquiVolume차트 추가
	CButton *pCandleVolumeButton = (CButton *)GetDlgItem( IDC_CANDLEVOLUME_CHART );
	CButton *pEquiVolumeButton = (CButton *)GetDlgItem( IDC_EQUIVOLUME_CHART );
//}} (2007.05.29 LYH) CandleVolume, EquiVolume차트 추가
	switch( nSubGraphStyle)
	{ 
		// 일본식
		case CGraphBaseData::CandlestickBong:
		{
			pBongButton->SetCheck(1);
			pBarButton->SetCheck(0);
			pLineButton->SetCheck(0);
			pNewBarButton->SetCheck(0);
			pFlowButton->SetCheck(0);
			pCandleVolumeButton->SetCheck(0);
			pEquiVolumeButton->SetCheck(0);
			// 보합기준에 따라 보합색상 버튼을 표시해준다.
			if( m_nSteadyType == 0 )
			{
				GetDlgItem(IDC_STATIC_STEADY)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_BTN_STEADY)->ShowWindow(SW_SHOW);
			}
				
			else
			{
				GetDlgItem(IDC_STATIC_STEADY)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_BTN_STEADY)->ShowWindow(SW_HIDE);
			}

			break;
		}

		// 미국식(시고저종)
		case CGraphBaseData::OpenHighLowCloseBong:
		{
			pBongButton->SetCheck(0);
			pBarButton->SetCheck(1);
			pLineButton->SetCheck(0);
			pNewBarButton->SetCheck(0);
			pFlowButton->SetCheck(0);
			pCandleVolumeButton->SetCheck(0);
			pEquiVolumeButton->SetCheck(0);
		
			// 보합기준에 따라 보합색상 버튼을 표시해준다.
			if( m_nSteadyType == 0 )
			{
				GetDlgItem(IDC_STATIC_STEADY)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_BTN_STEADY)->ShowWindow(SW_SHOW);
			}
				
			else
			{
				GetDlgItem(IDC_STATIC_STEADY)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_BTN_STEADY)->ShowWindow(SW_HIDE);
			}

			break;
		}

		// 라인식
		case CGraphBaseData::LineBong:			
		{
			pLineButton->SetCheck(1);
			pBongButton->SetCheck(0);
			pBarButton->SetCheck(0);
			pNewBarButton->SetCheck(0);
			pFlowButton->SetCheck(0);
			pCandleVolumeButton->SetCheck(0);
			pEquiVolumeButton->SetCheck(0);

			GetDlgItem(IDC_STATIC_STEADY)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_STEADY)->ShowWindow(SW_HIDE);
			break;
		}
// (2006.10.30 안승환) 바차트(고'저'종)추가
		// 미국식(고저종)
		case CGraphBaseData::NewHighLowCloseBong:
		{
			pNewBarButton->SetCheck(1);
			pBongButton->SetCheck(0);
			pBarButton->SetCheck(0);
			pLineButton->SetCheck(0);
			pFlowButton->SetCheck(0);
			pCandleVolumeButton->SetCheck(0);
			pEquiVolumeButton->SetCheck(0);
	
			// 보합기준에 따라 보합색상 버튼을 표시해준다.
			if( m_nSteadyType == 0 )
			{
				GetDlgItem(IDC_STATIC_STEADY)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_BTN_STEADY)->ShowWindow(SW_SHOW);
			}
			else
			{
				GetDlgItem(IDC_STATIC_STEADY)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_BTN_STEADY)->ShowWindow(SW_HIDE);
			}
			break;
		}
// (2006.11.03 안승환) Flow차트 추가
		case CGraphBaseData::FlowBong:
		{
			pFlowButton->SetCheck(1);
			pNewBarButton->SetCheck(0);
			pBongButton->SetCheck(0);
			pBarButton->SetCheck(0);
			pLineButton->SetCheck(0);
			pCandleVolumeButton->SetCheck(0);
			pEquiVolumeButton->SetCheck(0);
		
			// 보합기준에 따라 보합색상 버튼을 표시해준다.
			GetDlgItem(IDC_STATIC_STEADY)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_STEADY)->ShowWindow(SW_HIDE);
			break;
		}
// (2006.11.03 안승환) Flow차트 추가
//{{ (2007.05.29 LYH) CandleVolume차트 추가
		case CGraphBaseData::CandleVolume:
		{
			pBongButton->SetCheck(0);
			pBarButton->SetCheck(0);
			pLineButton->SetCheck(0);
			pNewBarButton->SetCheck(0);
			pFlowButton->SetCheck(0);
			pCandleVolumeButton->SetCheck(1);
			pEquiVolumeButton->SetCheck(0);

			// 보합기준에 따라 보합색상 버튼을 표시해준다.
			if( m_nSteadyType == 0 )
			{
				GetDlgItem(IDC_STATIC_STEADY)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_BTN_STEADY)->ShowWindow(SW_SHOW);
			}
				
			else
			{
				GetDlgItem(IDC_STATIC_STEADY)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_BTN_STEADY)->ShowWindow(SW_HIDE);
			}
			break;
		}
//{{ (2007.05.29 LYH) CandleVolume차트 추가
//{{ (2007.05.29 LYH) EquiVolume차트 추가
		case CGraphBaseData::EquiVolume:
		{
			pBongButton->SetCheck(0);
			pBarButton->SetCheck(0);
			pLineButton->SetCheck(0);
			pNewBarButton->SetCheck(0);
			pFlowButton->SetCheck(0);
			pCandleVolumeButton->SetCheck(0);
			pEquiVolumeButton->SetCheck(1);

			// 보합기준에 따라 보합색상 버튼을 표시해준다.
			if( m_nSteadyType == 0 )
			{
				GetDlgItem(IDC_STATIC_STEADY)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_BTN_STEADY)->ShowWindow(SW_SHOW);
			}
				
			else
			{
				GetDlgItem(IDC_STATIC_STEADY)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_BTN_STEADY)->ShowWindow(SW_HIDE);
			}
			break;
		}
//}} (2007.05.29 LYH) EquiVolume차트 추가

	}

	// (2006/12/7 - Seung-Won, Bae) Check 1 Tick Chart (Disable All Selection)
	if( m_pIPacketManager->GetBaseTimeUnit() == 1 && CScaleBaseData::HORZ_TICK == m_pIPacketManager->GetBaseTimeDataType())
	{
		pBongButton->EnableWindow(		FALSE);
		pBarButton->EnableWindow(		FALSE);
		pLineButton->EnableWindow(		FALSE);
		pNewBarButton->EnableWindow(	FALSE);
		pFlowButton->EnableWindow(		FALSE);
		pCandleVolumeButton->EnableWindow(		FALSE);
		pEquiVolumeButton->EnableWindow(		FALSE);
	}
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 22
// Modifier		: 
// Comments		: 
//-----------------------------------------------------------------------------
void CSetting_Price::OnBarChart() 
{
	// 라인차트에서 설정했던 값을 적용
	OnApply_DrawStyle( 0, m_DrawStyle_Combo);
	OnApply_Thickness( 0, m_Thickness_Combo);

	ShowHide_FillUpDownNCombo( 1);
	ShowHide_SteadyRadio( true);

	// 미국식설정
	m_pIGraphInfo->SetSubGraphStyle( 0, 1);
	m_pIGraphInfo->SetSubGraphDrawStyle( 0, 1);

	Initial_ColorBtn(3);
	Initial_DrawStyleComboBox( 0, m_DrawStyle_Combo);
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 22
// Return void : 
// Param  int nCount : 
// Comments		: 
//-----------------------------------------------------------------------------
void CSetting_Price::Initial_ColorBtn( int nCount )
{
	// 선타입
	UpdateData();

// 2008.01.25 by LYH >>
	//GetDlgItem(IDC_STATIC_UP)->SetWindowText( "상승");
	GetDlgItem(IDC_STATIC_UP)->SetWindowText( m_strUp);
// 2008.01.25 by LYH <<
	
// (2006.11.03 안승환) Flow차트 추가
	GetDlgItem(IDC_STATIC_UP)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_BTN_UP)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_DOWN)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_BTN_DOWN)->ShowWindow(SW_SHOW);
// (2006.11.03 안승환) Flow차트 추가

	if( nCount == 2 ) 
	{ 
		GetDlgItem(IDC_STATIC_STEADY)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_STEADY)->ShowWindow(SW_HIDE);

		if( m_bUseBasePrice)
		{
	// (2006.11.03 안승환) Flow차트 추가
			GetDlgItem(IDC_STATIC_UP)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_UP)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_DOWN)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_DOWN)->ShowWindow(SW_SHOW);
	// (2006.11.03 안승환) Flow차트 추가
		}
		else
		{
			GetDlgItem(IDC_STATIC_UP)->ShowWindow(		SW_SHOW);
// 2008.01.25 by LYH >>
			//GetDlgItem(IDC_STATIC_UP)->SetWindowText( "색상");
			CString strColor;
			strColor.LoadString(IDS_STRING_COLOR);
			GetDlgItem(IDC_STATIC_UP)->SetWindowText( strColor);
// 2008.01.25 by LYH <<
			GetDlgItem(IDC_BTN_UP)->ShowWindow(			SW_SHOW);
			GetDlgItem(IDC_STATIC_DOWN)->ShowWindow(	SW_HIDE);
			GetDlgItem(IDC_BTN_DOWN)->ShowWindow(		SW_HIDE);
		}
	}
// (2006.11.03 안승환) Flow차트 추가
	else if(nCount == 5)
	{
		GetDlgItem(IDC_STATIC_STEADY)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BTN_STEADY)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_UP)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_UP)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_DOWN)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_DOWN)->ShowWindow(SW_HIDE);
		return;
	}
// (2006.11.03 안승환) Flow차트 추가
	else
	{
		if( m_nSteadyType == 0 )
		{
			GetDlgItem(IDC_STATIC_STEADY)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_STEADY)->ShowWindow(SW_SHOW);
		}

		else
		{
			GetDlgItem(IDC_STATIC_STEADY)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_STEADY)->ShowWindow(SW_HIDE);
		}
	}
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 22
// Modifier		: 
// Comments		: 
//-----------------------------------------------------------------------------
void CSetting_Price::OnBongChart() 
{
	// 라인차트에서 설정했던 값을 적용
	OnApply_DrawStyle( 0, m_DrawStyle_Combo);
	OnApply_Thickness( 0, m_Thickness_Combo);

	ShowHide_FillUpDownNCombo(0);
	ShowHide_SteadyRadio(true);

	// 봉형설정: 일본식
	m_pIGraphInfo->SetSubGraphStyle( 0, 0);
	m_pIGraphInfo->SetSubGraphDrawStyle( 0, 0);

	Initial_ColorBtn(3);
	Initial_DrawStyleComboBox( 0, m_DrawStyle_Combo);
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 22
// Modifier		: 
// Comments		: 
//-----------------------------------------------------------------------------
void CSetting_Price::OnLineChart() 
{
	GetDlgItem(IDC_STATIC_STEADY)->ShowWindow(SW_HIDE);

	OnApply_FillUp( 0, m_FillUp_Bt);
	OnApply_FillDown( 0, m_FillDown_Bt);

	ShowHide_FillUpDownNCombo(2);
	ShowHide_SteadyRadio(false);

	//선형 설정 (라인식)
	//drawstyle은 이전에 미국식이냐 일본식이냐에 따라서 틀리다.
	//먼저 라인차트에서 설정했던 값은 무시 될 수 밖에 없다.
	m_pIGraphInfo->SetSubGraphStyle( 0, 2);
	m_pIGraphInfo->SetSubGraphDrawStyle( 0, 2);

	//Initial_ColorItem_FromThreeToOne();
	Initial_ColorBtn(2);
	Initial_DrawStyleComboBox( 0, m_DrawStyle_Combo);
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 22
// Return void : 
// Param  int nState : 
// Comments		: 
//-----------------------------------------------------------------------------
void CSetting_Price::ShowHide_FillUpDownNCombo(int nState)
{
	// (2007/12/17 - Seung-Won, Bae) for Not-Candle Type
	int nSubGraphType = m_pIGraphInfo->GetSubGraphType( 0);
	if( CGraphBaseData::Bong_Type != nSubGraphType)
	{
		int nSubGraphStyle = m_pIGraphInfo->GetSubGraphStyle( 0);

		// 고정비율상하식 추가 - ojtaso (20080603)
		if( (CGraphBaseData::Bar_Type == nSubGraphType && CGraphBaseData::UpDownBar == nSubGraphStyle)
		   || (CGraphBaseData::Bar_Type == nSubGraphType && CGraphBaseData::FixedRateUpDownBar == nSubGraphStyle))
		{
			m_FillUp_Bt.ShowWindow( SW_SHOW);
			m_FillDown_Bt.ShowWindow( SW_SHOW);
		}
		else if( CGraphBaseData::Bar_Type == nSubGraphType)
		{
			m_FillUp_Bt.ShowWindow( SW_SHOW);
// 2008.01.25 by LYH >>
			//m_FillUp_Bt.SetWindowText( "채움");
			CString strFill;
			strFill.LoadString(IDS_STRING_FILL);
			m_FillUp_Bt.SetWindowText( strFill);
// 2008.01.25 by LYH <<
			m_FillDown_Bt.ShowWindow( SW_HIDE);
		}
		else
		{
			m_FillUp_Bt.ShowWindow( SW_HIDE);
			m_FillDown_Bt.ShowWindow( SW_HIDE);
		}

		int nShowWindow = ( CGraphBaseData::Line_Type == nSubGraphType ? SW_SHOW : SW_HIDE);
		m_Thickness_Static.ShowWindow(	nShowWindow);
		m_Thickness_Combo.ShowWindow(	nShowWindow);
		m_DrawStyle_Static.ShowWindow(	nShowWindow);
		m_DrawStyle_Combo.ShowWindow(	nShowWindow);

		return;
	}
	
	if(nState == 0 || nState == 6 || nState == 7)
	{
		m_FillUp_Bt.ShowWindow(true);
		m_FillDown_Bt.ShowWindow(true);
	}
	else
	{
		m_FillUp_Bt.ShowWindow(false);
		m_FillDown_Bt.ShowWindow(false);
	}

	m_Thickness_Static.ShowWindow(	SW_SHOW);
	m_Thickness_Combo.ShowWindow(	SW_SHOW);
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 22
// Return void : 
// Param  bool bShow : 
// Comments		: 
//-----------------------------------------------------------------------------
void CSetting_Price::ShowHide_SteadyRadio( BOOL bShow)
{
	GetDlgItem(IDC_STEADY_NORMAL)->EnableWindow(bShow);
	GetDlgItem(IDC_STEADY_BEFORECLOSE)->EnableWindow(bShow);
	GetDlgItem( IDC_STEADY_SAMECLOSE)->EnableWindow(bShow);
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 22
// Return void : 
// Comments		: 
//-----------------------------------------------------------------------------
void CSetting_Price::OnSteadyNormal() 
{
	if( m_btnSteady.IsWindowVisible()) return;

	GetDlgItem(IDC_STATIC_STEADY)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_BTN_STEADY)->ShowWindow(SW_SHOW);
	m_btnSteady.SetColor( m_pIGraphInfo->GetSubGraphColor( 0, 5) );
	m_btnSteady.Invalidate();
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 22
// Return void : 
// Comments		: 
//-----------------------------------------------------------------------------
void CSetting_Price::OnSteadyBeforeclose() 
{
	GetDlgItem(IDC_STATIC_STEADY)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BTN_STEADY)->ShowWindow(SW_HIDE);
}

//-----------------------------------------------------------------------------
// Author		: 이상신		Date : 2013 / 04 / 01
// Return void : 
// Comments		: 
//-----------------------------------------------------------------------------
void CSetting_Price::OnSteadySameclose() 
{
	GetDlgItem(IDC_STATIC_STEADY)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BTN_STEADY)->ShowWindow(SW_HIDE);
}

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 22
// Modifier		: 
// Comments		: 
//-----------------------------------------------------------------------------
void CSetting_Price::OnBtnUp() 
{
	// TODO: Add your control notification handler code here
	COLORREF crColor = m_btnUp.GetColor();
	CColorDialog color(crColor, CC_FULLOPEN);
	if(color.DoModal() == IDOK) 
	{
		m_btnUp.SetColor( color.GetColor() );
		m_btnUp.Invalidate();
	}	
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 22
// Modifier		: 
// Comments		: 
//-----------------------------------------------------------------------------
void CSetting_Price::OnBtnDown() 
{
	// TODO: Add your control notification handler code here
	COLORREF crColor = m_btnDown.GetColor();
	CColorDialog color(crColor, CC_FULLOPEN);
	if(color.DoModal() == IDOK) 
	{
		m_btnDown.SetColor( color.GetColor() );
		m_btnDown.Invalidate();
	}	
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 22
// Modifier		: 
// Comments		: 
//-----------------------------------------------------------------------------
void CSetting_Price::OnBtnSteady() 
{
	// TODO: Add your control notification handler code here
	COLORREF crColor = m_btnSteady.GetColor();
	CColorDialog color(crColor, CC_FULLOPEN);
	if(color.DoModal() == IDOK) 
	{
		m_btnSteady.SetColor( color.GetColor() );
		m_btnSteady.Invalidate();
	}	
}

BOOL CSetting_Price::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN)
		{
			if( m_pParentWnd != NULL )
			{
				if( m_pTopWnd != NULL )
					( (CDlgEnvSetUpAll *)m_pTopWnd)->OK();

				else
					( (CGraphSetting *)m_pParentWnd)->OK();
			}
		}
		
		if( pMsg->wParam == VK_ESCAPE )
		{
			if( m_pParentWnd != NULL )
			{
				if( m_pTopWnd != NULL )
					( (CDlgEnvSetUpAll *)m_pTopWnd)->Cancel();

				else
					( (CGraphSetting *)m_pParentWnd)->Cancel();
			}
		}
	}
	
	return CBaseDialog::PreTranslateMessage(pMsg);
}

// (2006.10.30 안승환) 바차트(고'저'종)추가
void CSetting_Price::OnNewBarChart() 
{
	// 라인차트에서 설정했던 값을 적용
	OnApply_DrawStyle( 0, m_DrawStyle_Combo);
	OnApply_Thickness( 0, m_Thickness_Combo);

	ShowHide_FillUpDownNCombo( 1);
	ShowHide_SteadyRadio( true);

	// 미국식설정
	m_pIGraphInfo->SetSubGraphStyle( 0, 4);
	m_pIGraphInfo->SetSubGraphDrawStyle( 0, 4);

	Initial_ColorBtn(3);
	Initial_DrawStyleComboBox( 0, m_DrawStyle_Combo);
}
// (2006.10.30 안승환) 바차트(고'저'종)추가
// (2006.11.03 안승환) Flow차트 추가
void CSetting_Price::OnFlowChart() 
{
	// 라인차트에서 설정했던 값을 적용
	ShowHide_SteadyRadio(false);

	OnApply_DrawStyle( 0, m_DrawStyle_Combo);
	OnApply_Thickness( 0, m_Thickness_Combo);

	ShowHide_FillUpDownNCombo( 1);

	m_pIGraphInfo->SetSubGraphStyle( 0, 5);
	m_pIGraphInfo->SetSubGraphDrawStyle( 0, 5);

	Initial_ColorBtn(5);
	Initial_DrawStyleComboBox( 0, m_DrawStyle_Combo);

}// (2006.11.03 안승환) Flow차트 추가

//{{ (2007.05.29 LYH) CandleVolume차트 추가
void CSetting_Price::OnCandleVolumeChart() 
{
	// 라인차트에서 설정했던 값을 적용
	OnApply_DrawStyle( 0, m_DrawStyle_Combo);
	OnApply_Thickness( 0, m_Thickness_Combo);

	ShowHide_FillUpDownNCombo(0);
	ShowHide_SteadyRadio(true);

	// 봉형설정: CandleVolume
	m_pIGraphInfo->SetSubGraphStyle( 0, 6);
	m_pIGraphInfo->SetSubGraphDrawStyle( 0, 6);

	Initial_ColorBtn(3);
	Initial_DrawStyleComboBox( 0, m_DrawStyle_Combo);

}
//}} (2007.05.29 LYH) CandleVolume차트 추가

//{{ (2007.05.29 LYH) CandleVolume차트 추가
void CSetting_Price::OnEquiVolumeChart() 
{
	OnApply_DrawStyle( 0, m_DrawStyle_Combo);
	OnApply_Thickness( 0, m_Thickness_Combo);

	ShowHide_FillUpDownNCombo(0);
	ShowHide_SteadyRadio(true);

	// 봉형설정: CandleVolume
	m_pIGraphInfo->SetSubGraphStyle( 0, 7);
	m_pIGraphInfo->SetSubGraphDrawStyle( 0, 7);

	Initial_ColorBtn(3);
	Initial_DrawStyleComboBox( 0, m_DrawStyle_Combo);
}
//}} (2007.05.29 LYH) CandleVolume차트 추가

void CSetting_Price::OnSelchangeThickness() 
{
	// TODO: Add your control notification handler code here

	// (2007/12/17 - Seung-Won, Bae) for Not-Candle Type
	int nSubGraphType = m_pIGraphInfo->GetSubGraphType( 0);
	if( CGraphBaseData::Bong_Type != nSubGraphType
		&& m_Thickness_Combo.GetCurSel() != 0
		&& m_DrawStyle_Combo.GetCurSel() != 0 )
	{
		// Sang-Woo, Cho		2006 / 06 / 07
		// Box Chart일 경우에는 해당되지 않는다.
		// Begin
		ILPCSTR szGraphName = m_pIGraphInfo->GetGraphNameString();
// 2008.01.25 by LYH >>
		//if( strcmp( szGraphName, "그물차트") && strcmp( szGraphName, "Box Chart"))
			//AfxMessageBox("선굵기가 \"1pt\"인 경우만 선모양을 변경할 수 있습니다.");
		if( strcmp( szGraphName, _MTEXT( C2_RAINBOW_CHART )) && strcmp( szGraphName, _MTEXT( C2_BOX_CHART )))
			AfxMessageBox(m_strMsgLineShape);
// 2008.01.25 by LYH <<
		// End
		
		m_DrawStyle_Combo.SetCurSel(0);
		m_DrawStyle_Combo.EnableWindow(FALSE);
		return;
	}	

	else
		m_DrawStyle_Combo.EnableWindow(TRUE);	
}

// (2007/12/17 - Seung-Won, Bae) for Not-Candle Type
void CSetting_Price::OnSelchangeDrawstyle() 
{
	// TODO: Add your control notification handler code here
	if( m_Thickness_Combo.GetCurSel() != 0 && m_DrawStyle_Combo.GetCurSel() != 0 )
	{
		// Sang-Woo, Cho		2006 / 06 / 07
		// 그물차트 일 경우에는 해당되지 않는다.
		// Begin
		ILPCSTR szGraphName = m_pIGraphInfo->GetGraphNameString();
// 2008.01.25 by LYH >>
//		if( strcmp( szGraphName, "그물차트")) 
			//AfxMessageBox("선모양이 \"실선\"인 경우만 굵기를 변경할 수 있습니다.");
		if( strcmp( szGraphName, _MTEXT( C2_RAINBOW_CHART )))
			AfxMessageBox(m_strMsgLineThick);
// 2008.01.25 by LYH <<
		// End
		
		m_Thickness_Combo.SetCurSel(0);
		m_Thickness_Combo.EnableWindow(FALSE);
		return;
	}

	else
		m_Thickness_Combo.EnableWindow(TRUE);
}
