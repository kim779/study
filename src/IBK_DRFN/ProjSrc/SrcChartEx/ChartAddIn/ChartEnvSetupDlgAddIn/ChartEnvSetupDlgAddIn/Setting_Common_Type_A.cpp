// Setting_Common_Type_A.cpp : implementation file
//

#include "stdafx.h"
#include "ChartEnvSetupDlgAddIn.h"
#include "Setting_Common_Type_A.h"

#include <Math.h>									// 20081014 박동관

#include "../Include_Chart/Dll_Load/IMetaTable.h"	// for _MTOL0()
#include "GraphData.h"
#include "BlockBaseEnum.h"
#include "Conversion.h"
#include "TabMainChartEnvSetUp.h"
#include "DlgEnvSetUpAll.h"
#include "GraphSetting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetting_Common_Type_A dialog


CSetting_Common_Type_A::CSetting_Common_Type_A( IChartOCX *p_pIChartOCX, CWnd* pParent, CWnd *pTopWnd)
	: CBaseDialog( CSetting_Common_Type_A::IDD, pParent, p_pIChartOCX, pTopWnd)
{
	//{{AFX_DATA_INIT(CSetting_Common_Type_A)
	m_nCompositionLineType = -1;
	m_strHorizonValue = _T("");
	//}}AFX_DATA_INIT

	m_nSubGraphIndex = -1;

	m_bUseComposition = FALSE;
	m_bUseBaseLine = FALSE;
	m_bInitComplite = FALSE;
	m_bUsePacket = FALSE;
}


CSetting_Common_Type_A::~CSetting_Common_Type_A()
{
}


void CSetting_Common_Type_A::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetting_Common_Type_A)
	DDX_Control(pDX, IDC_CMB_COMPCOLOR, m_cmbCompColor);
	DDX_Control(pDX, IDC_CHK_FILL_COLOR, m_btnFillColor);
	DDX_Control(pDX, IDC_BTN_DOWN1, m_btnDown1Color);
	DDX_Control(pDX, IDC_BTN_UP1, m_btnUp1Color);
	DDX_Control(pDX, IDC_BTN_UP_COLOR, m_btnUpColor);
	DDX_Control(pDX, IDC_BTN_STEADY_COLOR, m_btnSteadyColor);
	DDX_Control(pDX, IDC_BTN_DOWN_COLOR, m_btnDownColor);
	DDX_Control(pDX, IDC_SPIN, m_Horizon_Spin);
	DDX_Control(pDX, IDC_SIGNAL, m_Signal_Bt);
	DDX_Control(pDX, IDC_HORIZONTAL_LIST, m_HorizonLine_ListBox);
	DDX_Control(pDX, IDC_HORIZONTAL, m_Horizon_Edit);
	DDX_Control(pDX, IDC_COLOR_BUTTON, m_Color_Bt);
	DDX_Control(pDX, IDC_STATIC_VALUE5, m_stcValue5);
	DDX_Control(pDX, IDC_STATIC_VALUE4, m_stcValue4);
	DDX_Control(pDX, IDC_STATIC_VALUE3, m_stcValue3);
	DDX_Control(pDX, IDC_STATIC_VALUE2, m_stcValue2);
	DDX_Control(pDX, IDC_STATIC_VALUE1, m_stcValue1);
	DDX_Control(pDX, IDC_SPIN_VALUE5, m_spnValue5);
	DDX_Control(pDX, IDC_SPIN_VALUE4, m_spnValue4);
	DDX_Control(pDX, IDC_SPIN_VALUE3, m_spnValue3);
	DDX_Control(pDX, IDC_SPIN_VALUE2, m_spnValue2);
	DDX_Control(pDX, IDC_SPIN_VALUE1, m_spnValue1);
	DDX_Control(pDX, IDC_EDIT_VALUE5, m_edtValue5);
	DDX_Control(pDX, IDC_EDIT_VALUE4, m_edtValue4);
	DDX_Control(pDX, IDC_EDIT_VALUE3, m_edtValue3);
	DDX_Control(pDX, IDC_EDIT_VALUE2, m_edtValue2);
	DDX_Control(pDX, IDC_EDIT_VALUE1, m_edtValue1);
	DDX_Control(pDX, IDC_STATIC_DATA_TITLE, m_stcDataTitle);
	DDX_Control(pDX, IDC_EDIT_COMPOSITION_TITLEDATA, m_edtCompositionTitleData);
	DDX_Control(pDX, IDC_CMB_THICKNESS, m_cmbThickness);
	DDX_Control(pDX, IDC_CMB_LINETYPE, m_cmbLineType);
	DDX_Control(pDX, IDC_CMB_DATA, m_cmbData);
	DDX_Control(pDX, IDC_BTN_COMPOSITION_COLOR, m_btnComositionColor);
	DDX_Control(pDX, IDC_LIST_COMPOSITION, m_lstComposition);
	DDX_Control(pDX, IDC_CMB_THICKNESS2, m_cmbThickness2);
	DDX_Control(pDX, IDC_CMB_LINETYPE2, m_cmbLineType2);
	DDX_Text(pDX, IDC_HORIZONTAL, m_strHorizonValue);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetting_Common_Type_A, CDialog)
	//{{AFX_MSG_MAP(CSetting_Common_Type_A)
	ON_BN_CLICKED(IDC_BTN_COMPOSITION_COLOR, OnBtnCompositionColor)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_COMPOSITION, OnItemchangedListComposition)
	ON_NOTIFY(NM_CLICK, IDC_LIST_COMPOSITION, OnClickListComposition)
	ON_CBN_SELCHANGE(IDC_CMB_LINETYPE, OnSelchangeCmbLinetype)
	ON_CBN_SELCHANGE(IDC_CMB_THICKNESS, OnSelchangeCmbThickness)
	ON_BN_CLICKED(IDC_INSERT, OnInsert)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_BN_CLICKED(IDC_ALLDELETE, OnAlldelete)
	ON_BN_CLICKED(IDC_COLOR_BUTTON, OnColorButton)
	ON_BN_CLICKED(IDC_BTN_UP_COLOR, OnBtnUpColor)
	ON_BN_CLICKED(IDC_BTN_DOWN_COLOR, OnBtnDownColor)
	ON_BN_CLICKED(IDC_BTN_STEADY_COLOR, OnBtnSteadyColor)
	ON_BN_CLICKED(IDC_BTN_UP1, OnBtnUp1)
	ON_BN_CLICKED(IDC_BTN_DOWN1, OnBtnDown1)
	ON_BN_CLICKED(IDC_CHK_FILL_COLOR, OnChkFillColor)
	ON_CBN_SELCHANGE(IDC_CMB_THICKNESS2, OnSelchangeCmbThickness2)
	ON_CBN_SELCHANGE(IDC_CMB_LINETYPE2, OnSelchangeCmbLinetype2)
	ON_LBN_SELCHANGE(IDC_HORIZONTAL_LIST, OnSelchangeHorizontalList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetting_Common_Type_A message handlers

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 10
// Modifier		: 
// Comments		: 
//-----------------------------------------------------------------------------
BOOL CSetting_Common_Type_A::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// 1. 기본적인 Column 생성
	m_lstComposition.InsertColumn( 0, "", LVCFMT_LEFT, 100 );
	m_lstComposition.SetColumnWidth( 0,  LVSCW_AUTOSIZE_USEHEADER  );
	m_lstComposition.SetExtendedStyle( LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES );

	m_cmbData.ResetContent();
	m_cmbData.AddString( _LTEXT0( C0_OPEN));
	m_cmbData.AddString( _LTEXT0( C0_HIGH));
	m_cmbData.AddString( _LTEXT0( C0_LOW));
	m_cmbData.AddString( _LTEXT0( C0_CLOSE));
	CString strStdHL, strStdHLC;
	strStdHL.LoadString( IDS_STANDART_HL);
	strStdHLC.LoadString( IDS_STANDART_HLC);
	m_cmbData.AddString( strStdHL);
	m_cmbData.AddString( strStdHLC);

	//20081014 박동관 >>
	CString strType = GetPacketType();

	// 패킷타입을 셋팅한다.
	m_Horizon_Edit.SetPacketType(strType);

	// 처음 초기화될때 한번만 최소 진법의 수를 셋팅한다.
	CString strResult = CDataConversion::GetMinimumValue(strType);

	m_Horizon_Edit.SetDeltaMinimum(strResult);
	//20081014 박동관 <<

	if( !m_pIGraphInfo) return TRUE;
	Initial( NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 10
// Return void : 
// Param  IGraphInfo* p_pIGraphInfo : 
// Comments		: 
//-----------------------------------------------------------------------------
void CSetting_Common_Type_A::Initial(IGraphInfo* p_pIGraphInfo)
{
	if( m_pIGraphInfo)
	{
		OnApply( m_pIGraphInfo);
		m_pIGraphInfo->Release();
	}
	m_pIGraphInfo = p_pIGraphInfo;
	if( m_pIGraphInfo) m_pIGraphInfo->AddRef();
	else return;

	//20081014 박동관 >>
	CString strType = GetPacketType();

	// 패킷타입을 셋팅한다.
	m_Horizon_Edit.SetPacketType(strType);

	// 처음 초기화될때 한번만 최소 진법의 수를 셋팅한다.
	CString strResult = CDataConversion::GetMinimumValue(strType);

	m_Horizon_Edit.SetDeltaMinimum(strResult);
	//20081014 박동관 <<

	// (2007/4/23 - Seung-Won, Bae) Init SubGraph Index before Init_Composition_Group()
	m_nSubGraphIndex = 0;

	// 1. 구성설정 Dialog 부분 Setting
	UseCompositionGroup( m_pIGraphInfo->GetSubGraphCount());
	Init_Composition_Group( );

	// 2. 변수 설정 Group
	int nCount = 0;
	IDoubleList *pIDblCnd = m_pIGraphInfo->GetCommonCalculateConditionList();
	if( pIDblCnd)
	{
		nCount = pIDblCnd->GetCount();
		UseValueGroup( 0 < nCount && nCount <= 5);
		pIDblCnd->Release();
	}
	Init_Variable_Group_Title( nCount);
	
	//sy 2006.04.17.
	// 4. 매매신호 show/hide
	BOOL bValidFalg = FALSE, bShowSignal = FALSE;
	m_pIGraphInfo->GetSignalShowFlag( bValidFalg, bShowSignal);
	//signal 체크 표시 보이기, 안보이기
	m_Signal_Bt.ShowWindow( bValidFalg ? SW_SHOW : SW_HIDE);

	// 1. 변수 설정 Group
	pIDblCnd = m_pIGraphInfo->GetCommonCalculateConditionList();
	if( pIDblCnd)
	{
		double dData = 0;
		POSITION psnCnd = pIDblCnd->GetHeadPosition();

		int i = 0;
		CNumSpinCtrl *	pspnValue[ 5] = {	&m_spnValue1, &m_spnValue2, &m_spnValue3, &m_spnValue4, &m_spnValue5	};
		CNumEdit *		pedtValue[ 7] = {	&m_edtValue1, &m_edtValue2, &m_edtValue3, &m_edtValue4, &m_edtValue5	};
		// (2008/10/8 - Seung-Won, Bae) Support double condition of Envelope.
		ILPCSTR szIndicatorName = m_pIGraphInfo->GetIndicatorNameString();
//>> vntsorl(20090828)
		if( !strcmp( szIndicatorName, _MTEXT( C2_ENVELOPE))) 
		{
			if( psnCnd) pIDblCnd->GetNext( psnCnd, dData);
			Initial_Spin( *pspnValue[ 0],  *pedtValue[ 0], dData);
			if( psnCnd) pIDblCnd->GetNext( psnCnd, dData);
			Initial_Spin( *pspnValue[ 1],  *pedtValue[ 1], dData, 0, (float)100, (float)0.1);
			if( psnCnd) pIDblCnd->GetNext( psnCnd, dData);
			Initial_Spin( *pspnValue[ 2],  *pedtValue[ 2], dData, 0, (float)100, (float)0.1);
		}
		else if( !strcmp( szIndicatorName, _MTEXT( C2_PARABOLIC_SAR)))
		{
			if( psnCnd) pIDblCnd->GetNext( psnCnd, dData);
			Initial_Spin( *pspnValue[ 0],  *pedtValue[ 0], dData, 0, (float)100, (float)0.02);
			if( psnCnd) pIDblCnd->GetNext( psnCnd, dData);
			Initial_Spin( *pspnValue[ 1],  *pedtValue[ 1], dData, 0, (float)100, (float)1);
		}
		else
		{
			while( psnCnd && i < 5)
			{
				pIDblCnd->GetNext( psnCnd, dData);
				Initial_Spin( *pspnValue[ i],  *pedtValue[ i], dData);
				i++;
			}
		}
//<< vntsorl(20090828)
		pIDblCnd->Release();
	}

	SetStatePacketCombo( m_pIGraphInfo, m_cmbData, m_stcDataTitle);

	// 2. 구성설정 Dialog 부분 Setting
	Init_Composition_Group();

	ILPCSTR szIndicatorName = m_pIGraphInfo->GetIndicatorNameString();
	if( CheckSpecialCond( szIndicatorName))
	{
		UpdateData(FALSE);
		return;
	}

	// 3. 기준선 설정 Dialog 부분 Setting
	Init_BaseLine_Group_Title();
	UseBaseLineGroup( IsShowBaseLineList());

	// Singbar Graph 색상 비교기준 추가 - ojtaso (20071121)
	Init_BarGraphCompareColor();

	ILPCSTR szGraphName = m_pIGraphInfo->GetGraphNameString();
	if(!strcmp( szGraphName,  _MTEXT( C2_LINE_PRICE_MA )))
		m_cmbLineType.EnableWindow(TRUE);

	UpdateData(FALSE);
}



//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 10
// Return void : 
// Param  CString szTitle : 
// Param  CList<double, double>* pDataList : 
// Comments		: 
//-----------------------------------------------------------------------------
void CSetting_Common_Type_A::Init_Variable_Group_Title( int p_nCndCount)
{
	UseValue1(FALSE);
	UseValue2(FALSE);
	UseValue3(FALSE);
	UseValue4(FALSE);
	UseValue5(FALSE);
	
	if( p_nCndCount == 0 )
	{
		GetDlgItem( IDC_STATIC_VALUE )->ShowWindow( SW_HIDE);
		return;
	}

	if( 0 < p_nCndCount && p_nCndCount < 6 )
	{
		ILPCSTR szGraphName = m_pIGraphInfo->GetGraphNameString();
		for( int i = 0 ; i < p_nCndCount ; i++ )
		{
			switch( i )
			{
				case 0:
					UseValue1( TRUE );
					m_stcValue1.SetWindowText( GetConditionTitle( szGraphName, i+1 ) );
					break;
				
				case 1:
					UseValue2( TRUE );
					m_stcValue2.SetWindowText( GetConditionTitle( szGraphName, i+1 ) );
					break;
						
				case 2:
					UseValue3( TRUE );
					m_stcValue3.SetWindowText( GetConditionTitle( szGraphName, i+1 ) );
					break;
							
				case 3:
					UseValue4( TRUE );
					m_stcValue4.SetWindowText( GetConditionTitle( szGraphName, i+1 ) );
					break;
					
				case 4:
					UseValue5( TRUE );
					m_stcValue5.SetWindowText( GetConditionTitle( szGraphName, i+1 ) );
					break;
			}
		}
			
		SetStatePacketCombo( m_pIGraphInfo, m_cmbData, m_stcDataTitle);
	}
}

//sy 2006.04.17.
void CSetting_Common_Type_A::OnApplySignal(IGraphInfo* p_pIGraphInfo)
{
	if(p_pIGraphInfo == NULL || !m_Signal_Bt.IsWindowVisible()) return;

	// 속성이 보일때만 signal 저장한다.
	BOOL bValidFalg = FALSE, bShowSignal = FALSE;
	m_pIGraphInfo->GetSignalShowFlag( bValidFalg, bShowSignal);
	if( !bValidFalg) return;

	// signal 보임
	m_pIGraphInfo->SetSignalShowFlag( bValidFalg, m_Signal_Bt.GetCheck());
}

void CSetting_Common_Type_A::Initial_Signal(IGraphInfo* p_pIGraphInfo)
{
	if(p_pIGraphInfo == NULL) return;
	BOOL bValidFalg = FALSE, bShowSignal = FALSE;
	m_pIGraphInfo->GetSignalShowFlag( bValidFalg, bShowSignal);
	if( !bValidFalg) return;
	
	//signal 사용
	m_Signal_Bt.SetCheck( bShowSignal);
}
//sy end

BOOL CSetting_Common_Type_A::CheckSpecialCond( const char *p_szIndicatorName)
{
// 2008.01.25 by LYH >>
//	if( !strcmp( p_szIndicatorName, "상대비교차트"))
	if( !strcmp( p_szIndicatorName, _MTEXT(C2_COMPARISON_CHART)))
// 2008.01.25 by LYH <<
	{
		GetDlgItem( IDC_EDIT_COMPOSITION_TITLEDATA )->EnableWindow( FALSE );

		UseValueGroup( FALSE );
		UseValue1(FALSE);
		UseValue2(FALSE);
		UseValue3(FALSE);
		UseValue4(FALSE);
		UseValue5(FALSE);
		
		UsePacketData( true );
		
		GetDlgItem( IDC_STATIC_VALUE )->ShowWindow(SW_HIDE);
		
		UseBaseLineGroup(FALSE);

		return TRUE;
	}

	return FALSE;
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 10
// Return void : 
// Param  IGraphInfo* p_pIGraphInfo : 
// Comments		: 
//-----------------------------------------------------------------------------	
// 20081014 박동관 >>
BOOL CSetting_Common_Type_A::OnApply(IGraphInfo* p_pIGraphInfo)
//void CSetting_Common_Type_A::OnApply(IGraphInfo* p_pIGraphInfo)
// 20081014 박동관 <<
{
	if(p_pIGraphInfo == NULL)
		return FALSE;
	
	UpdateData();
	
	OnApplySubGraphData();

	p_pIGraphInfo->GetCopyDataFrom(m_pIGraphInfo);

	return TRUE;
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 11
// Return void : 
// Comments		: 
//-----------------------------------------------------------------------------
void CSetting_Common_Type_A::OnApplyBaseLine()
{
	if(m_pIGraphInfo == NULL) return;
	
	IDoubleList *pIDblList = m_pIGraphInfo->GetBaseLineList();
	if( pIDblList)
	{
		pIDblList->RemoveAll();
		CString data;

		// 20081014 박동관 패킷타입에 따라 데이터를 저장한다. >>
		CString strType = GetPacketType();
		
		if(CDataConversion::IsBaseFormat(strType))
		{
			// 진법변환일 경우...
			for(int nIndex = 0 ; nIndex < m_HorizonLine_ListBox.GetCount(); nIndex++) 
			{
				m_HorizonLine_ListBox.GetText(nIndex, data);

				// 진법을 10진수로 변환한다.
				CString strOutput;
				BOOL bResult = CDataConversion::GetDataToDec(data, strType, strOutput);

				if(bResult)
					pIDblList->AddTail( atof(strOutput));
				else
					pIDblList->AddTail( atof(data));
			}
		}
		else
		{
			// 진법변환이 아닐경우...
			for(int nIndex = 0 ; nIndex < m_HorizonLine_ListBox.GetCount(); nIndex++) 
			{
				m_HorizonLine_ListBox.GetText(nIndex, data);
				pIDblList->AddTail( atof( data));
			}
		}
		// 20081014 박동관 <<

		pIDblList->Release();
	}

	// 기준선 두께, 모양 설정 - ojtaso (20071213)
	for(int i = 0; i < m_pIGraphInfo->GetSubGraphCount(); i++)
	{
		OnApply_BaseLineThickness( i, m_cmbThickness2);
		OnApply_BaseLineDrawStyle( i, m_cmbLineType2);
	}

	//sy 2006.04.17.
	OnApplySignal(m_pIGraphInfo);
}

// Singbar Graph 색상 비교기준 추가 - ojtaso (20071121)
void CSetting_Common_Type_A::OnApplyPaintStyle()
{
	if( m_pIGraphInfo->GetSubGraphType( m_nSubGraphIndex) != CGraphBaseData::Bar_Type) return;

	int nPaintStyle = m_cmbCompColor.GetCurSel();
	m_pIGraphInfo->SetSubGraphPaintStyle(m_nSubGraphIndex, nPaintStyle);
}

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 10
// Return void : 
// Param  BOOL bUseValue : 
// Comments		: 
//-----------------------------------------------------------------------------
void CSetting_Common_Type_A::UseValueGroup( BOOL bUseValue)
{
	IDoubleList *pIDblCnd = m_pIGraphInfo->GetCommonCalculateConditionList();
	if( !pIDblCnd) return;
	
	int nValueCount = pIDblCnd->GetCount();
	pIDblCnd->Release();

	switch( nValueCount)
	{
		case 5 :	UseValue5(bUseValue);
		case 4 :	UseValue4(bUseValue);
		case 3 :	UseValue3(bUseValue);
		case 2 :	UseValue2(bUseValue);
		case 1 :	UseValue1(bUseValue);
	}

	// (2007/3/12 - Seung-Won, Bae) Set Multiple Packet List
	ILPCSTR szGroupName = m_pIGraphInfo->GetIndicatorGroupNameString();
	ILPCSTR szIndicatorName = m_pIGraphInfo->GetIndicatorNameString();
	if( szIndicatorName.IsEmpty() || m_pIChartOCX->IsTRIndicator( szGroupName, szIndicatorName))	m_bUseMultiplePriceData = TRUE;
	else
	{
		ILPCSTR szPacketName = m_pIGraphInfo->GetPacketNameString();
		CString strPacketName( szPacketName);
		m_bUseMultiplePriceData = ( 1 < strPacketName.Replace( ',', ' '));
	}

	UsePacketData( m_bUseMultiplePriceData);
}


void CSetting_Common_Type_A::UsePacketData( bool bUsePacket )
{
	GetDlgItem( IDC_STATIC_DATA_TITLE )->ShowWindow( ( bUsePacket ? FALSE : TRUE ) );
	GetDlgItem( IDC_CMB_DATA )->ShowWindow( ( bUsePacket ? FALSE : TRUE ) );

	// Singbar Graph 색상 비교기준 추가 - ojtaso (20071121)
	UseCompColorCtrl(bUsePacket ? FALSE : TRUE);
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 11
// Return void : 
// Param  BOOL bUseValue1 : 
// Comments		: 
//-----------------------------------------------------------------------------
void CSetting_Common_Type_A::UseValue1( BOOL bUseValue1 )
{
	if( bUseValue1 == TRUE )
	{
		GetDlgItem( IDC_STATIC_VALUE1 )->ShowWindow( SW_SHOW );
		GetDlgItem( IDC_EDIT_VALUE1 )->ShowWindow( SW_SHOW );
		GetDlgItem( IDC_SPIN_VALUE1 )->ShowWindow( SW_SHOW );
	}

	else
	{
		GetDlgItem( IDC_STATIC_VALUE1 )->ShowWindow( SW_HIDE );
		GetDlgItem( IDC_EDIT_VALUE1 )->ShowWindow( SW_HIDE );
		GetDlgItem( IDC_SPIN_VALUE1 )->ShowWindow( SW_HIDE );
	}
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 11
// Return void : 
// Param  BOOL bUseValue2 : 
// Comments		: 
//-----------------------------------------------------------------------------
void CSetting_Common_Type_A::UseValue2( BOOL bUseValue2 )
{
	if( bUseValue2 == TRUE )
	{
		GetDlgItem( IDC_STATIC_VALUE2 )->ShowWindow( SW_SHOW );
		GetDlgItem( IDC_EDIT_VALUE2 )->ShowWindow( SW_SHOW );
		GetDlgItem( IDC_SPIN_VALUE2 )->ShowWindow( SW_SHOW );
	}

	else
	{
		GetDlgItem( IDC_STATIC_VALUE2 )->ShowWindow( SW_HIDE );
		GetDlgItem( IDC_EDIT_VALUE2 )->ShowWindow( SW_HIDE );
		GetDlgItem( IDC_SPIN_VALUE2 )->ShowWindow( SW_HIDE );
	}
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 11
// Return void : 
// Param  BOOL bUseValue3 : 
// Comments		: 
//-----------------------------------------------------------------------------
void CSetting_Common_Type_A::UseValue3( BOOL bUseValue3 )
{
	if( bUseValue3 == TRUE )
	{
		GetDlgItem( IDC_STATIC_VALUE3 )->ShowWindow( SW_SHOW );
		GetDlgItem( IDC_EDIT_VALUE3 )->ShowWindow( SW_SHOW );
		GetDlgItem( IDC_SPIN_VALUE3 )->ShowWindow( SW_SHOW );
	}

	else
	{
		GetDlgItem( IDC_STATIC_VALUE3 )->ShowWindow( SW_HIDE );
		GetDlgItem( IDC_EDIT_VALUE3 )->ShowWindow( SW_HIDE );
		GetDlgItem( IDC_SPIN_VALUE3 )->ShowWindow( SW_HIDE );
	}
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 11
// Return void : 
// Param  BOOL bUseValue4 : 
// Comments		: 
//-----------------------------------------------------------------------------
void CSetting_Common_Type_A::UseValue4( BOOL bUseValue4 )
{
	if( bUseValue4 == TRUE )
	{
		GetDlgItem( IDC_STATIC_VALUE4 )->ShowWindow( SW_SHOW );
		GetDlgItem( IDC_EDIT_VALUE4 )->ShowWindow( SW_SHOW );
		GetDlgItem( IDC_SPIN_VALUE4 )->ShowWindow( SW_SHOW );
	}

	else
	{
		GetDlgItem( IDC_STATIC_VALUE4 )->ShowWindow( SW_HIDE );
		GetDlgItem( IDC_EDIT_VALUE4 )->ShowWindow( SW_HIDE );
		GetDlgItem( IDC_SPIN_VALUE4 )->ShowWindow( SW_HIDE );
	}
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 11
// Return void : 
// Param  BOOL bUseValue5 : 
// Comments		: 
//-----------------------------------------------------------------------------
void CSetting_Common_Type_A::UseValue5( BOOL bUseValue5 )
{
	if( bUseValue5 == TRUE )
	{
		GetDlgItem( IDC_STATIC_VALUE5 )->ShowWindow( SW_SHOW );
		GetDlgItem( IDC_EDIT_VALUE5 )->ShowWindow( SW_SHOW );
		GetDlgItem( IDC_SPIN_VALUE5 )->ShowWindow( SW_SHOW );
	}

	else
	{
		GetDlgItem( IDC_STATIC_VALUE5 )->ShowWindow( SW_HIDE );
		GetDlgItem( IDC_EDIT_VALUE5 )->ShowWindow( SW_HIDE );
		GetDlgItem( IDC_SPIN_VALUE5 )->ShowWindow( SW_HIDE );
	}
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 10
// Return void : 
// Param  BOOL bUseComposition : 
// Comments		: 
//-----------------------------------------------------------------------------
void CSetting_Common_Type_A::UseCompositionGroup( BOOL bUseComposition )
{
	m_bUseComposition = bUseComposition;
	
	if( bUseComposition)
	{
		GetDlgItem( IDC_COMPOSITION )->ShowWindow( SW_SHOW );			// GroupBox
		GetDlgItem( IDC_LIST_COMPOSITION )->ShowWindow( SW_SHOW );		// List
		GetDlgItem( IDC_COMPOSITION_EACH )->ShowWindow( SW_SHOW );		// sub GroupBox
		GetDlgItem( IDC_COMPOSITION_TITLE )->ShowWindow( SW_SHOW );
		GetDlgItem( IDC_EDIT_COMPOSITION_TITLEDATA )->ShowWindow( SW_SHOW );
		GetDlgItem( IDC_COMPOSITION_LINETITLE )->ShowWindow( SW_SHOW );
		GetDlgItem( IDC_BTN_COMPOSITION_COLOR )->ShowWindow( SW_SHOW );
		
		// Sang-Woo, Cho		2006 / 06 / 07
		// 향후 IndiCond.cfg에 인자로 추가하는 방향으로 수정하는게 바람직할 것 같음.
		// Begin
		ILPCSTR szGraphName = m_pIGraphInfo->GetIndicatorNameString();
		// "그물차트"일때에는 선 두께 설정을 숨김.
		// (2006/8/14 - Seung-Won, Bae) Do not support Pen Thickness and Draw Style in Parabolic SAR
		if( strcmp( szGraphName, _MTEXT( C2_RAINBOW_CHART )) && strcmp( szGraphName, _MTEXT( C2_PARABOLIC_SAR) ))
			GetDlgItem( IDC_CMB_THICKNESS )->ShowWindow( SW_SHOW);
		else GetDlgItem( IDC_CMB_THICKNESS )->ShowWindow( SW_HIDE);
		
		// "Box Chart"일때에는 선 Type이 Box형만 지원하기 때문에 Line Type을 숨김.
		if( strcmp( szGraphName, _MTEXT( C2_BOX_CHART )) && strcmp( szGraphName, _MTEXT( C2_PARABOLIC_SAR )))
			GetDlgItem( IDC_CMB_LINETYPE )->ShowWindow( SW_SHOW);
		else GetDlgItem( IDC_CMB_LINETYPE )->ShowWindow( SW_HIDE);
		// End
	}

	else
	{
		GetDlgItem( IDC_COMPOSITION )->ShowWindow( SW_HIDE );
		GetDlgItem( IDC_LIST_COMPOSITION )->ShowWindow( SW_HIDE );
		GetDlgItem( IDC_COMPOSITION_EACH )->ShowWindow( SW_HIDE );
		GetDlgItem( IDC_COMPOSITION_TITLE )->ShowWindow( SW_HIDE );
		GetDlgItem( IDC_EDIT_COMPOSITION_TITLEDATA )->ShowWindow( SW_HIDE );
		GetDlgItem( IDC_COMPOSITION_LINETITLE )->ShowWindow( SW_HIDE );
		GetDlgItem( IDC_BTN_COMPOSITION_COLOR )->ShowWindow( SW_HIDE );
		GetDlgItem( IDC_CMB_THICKNESS )->ShowWindow( SW_HIDE );
		GetDlgItem( IDC_CMB_LINETYPE )->ShowWindow( SW_HIDE );
	}
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 10
// Return void : 
// Param  BOOL bUseBaseLine : 
// Comments		: 
//-----------------------------------------------------------------------------
void CSetting_Common_Type_A::UseBaseLineGroup( BOOL bUseBaseLine )
{
	m_bUseBaseLine = bUseBaseLine;

	if( bUseBaseLine == TRUE )
	{
		GetDlgItem( IDC_HORIZONTAL )->ShowWindow( SW_SHOW );
		GetDlgItem( IDC_HORIZONTAL_LIST )->ShowWindow( SW_SHOW );
		GetDlgItem( IDC_SPIN )->ShowWindow( SW_SHOW );
		GetDlgItem( IDC_INSERT )->ShowWindow( SW_SHOW );
		GetDlgItem( IDC_DELETE )->ShowWindow( SW_SHOW );
		GetDlgItem( IDC_ALLDELETE )->ShowWindow( SW_SHOW );
		GetDlgItem( IDC_COLOR_BUTTON )->ShowWindow( SW_SHOW );
		GetDlgItem( IDC_BASELINE )->ShowWindow( SW_SHOW );
		// 기준선 두께, 모양 설정 - ojtaso (20071213)
		GetDlgItem( IDC_CMB_THICKNESS2 )->ShowWindow( SW_SHOW );
		GetDlgItem( IDC_CMB_LINETYPE2 )->ShowWindow( SW_SHOW );
	}

	else
	{
		GetDlgItem( IDC_HORIZONTAL )->ShowWindow( SW_HIDE );
		GetDlgItem( IDC_HORIZONTAL_LIST )->ShowWindow( SW_HIDE );
		GetDlgItem( IDC_SPIN )->ShowWindow( SW_HIDE );
		GetDlgItem( IDC_INSERT )->ShowWindow( SW_HIDE );
		GetDlgItem( IDC_DELETE )->ShowWindow( SW_HIDE );
		GetDlgItem( IDC_ALLDELETE )->ShowWindow( SW_HIDE );
		GetDlgItem( IDC_COLOR_BUTTON )->ShowWindow( SW_HIDE );
		GetDlgItem( IDC_BASELINE )->ShowWindow( SW_HIDE );
		// 기준선 두께, 모양 설정 - ojtaso (20071213)
		GetDlgItem( IDC_CMB_THICKNESS2 )->ShowWindow( SW_HIDE );
		GetDlgItem( IDC_CMB_LINETYPE2 )->ShowWindow( SW_HIDE );
	}
}


bool CSetting_Common_Type_A::IsShowBaseLineList() const
{
	IDoubleList *pIDblBase = m_pIGraphInfo->GetBaseLineList();
	int nCount = 0;
	if( pIDblBase)
	{
		nCount = pIDblBase->GetCount();
		pIDblBase->Release();
		if( nCount) return TRUE;
	}

	// 20081014 배승원 리스트에 카운트 없을때 리스트 박스 안보이는 버그 수정 >>
//	ILPCSTR szGraphName = m_pIGraphInfo->GetGraphNameString();
//	IGraphInfo* pDefaultInfo = m_pIGraphInfoManager->GetGraphInfo( IGraphInfoManager::IEILT_SITE_DEFAULT, szGraphName);
	ILPCSTR szIndicatorName = m_pIGraphInfo->GetIndicatorNameString();
	IGraphInfo* pDefaultInfo = m_pIGraphInfoManager->GetIndicatorInfo( IGraphInfoManager::IEILT_SITE_DEFAULT, szIndicatorName, IGraphInfoManager::IEIIGT_WITH_DEFAULT_GET);
	// 20081014 배승원 <<
	if( !pDefaultInfo) return FALSE;
	pIDblBase = pDefaultInfo->GetBaseLineList();
	nCount = 0;
	if( pIDblBase)
	{
		nCount = pIDblBase->GetCount();
		pIDblBase->Release();
	}
	pDefaultInfo->Release();
	return 0 < nCount;
}



//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 10
// Return void : 
// Param  CList< int : 
// Param  int > *lstSubGraphData : 
// Comments		: 
//-----------------------------------------------------------------------------
//sy 2005.11.02.
void CSetting_Common_Type_A::Init_Composition_Group( void)
{
	Init_Composition_Group_SubGraphItem();
	Init_Composition_Group_SubGraphTitle(m_nSubGraphIndex);
	Init_Composition_Group_SubGraphLine(m_nSubGraphIndex);
	Init_Composition_Group_SubGraphColor(m_nSubGraphIndex);
	// 기준선 두께, 모양 설정 - ojtaso (20071213)
	Init_Composition_Group_BaseLine(m_nSubGraphIndex);
}

void CSetting_Common_Type_A::Init_Composition_Group_SubGraphItem( void)
{
	m_lstComposition.DeleteAllItems();

	// SubGraphName list check
	BOOL bShowFlag = FALSE;
	ILPCSTR szSubGraphName = NULL;
	int nCount = m_pIGraphInfo->GetSubGraphCount();
// 2008.01.25 by LYH >>
	CString strUIText;
// 2008.01.25 by LYH <<
	
	for( int nItem = 0 ; nItem < nCount; nItem++ )
	{
		szSubGraphName = m_pIGraphInfo->GetSubGraphNameString( nItem);
// 2008.01.25 by LYH >>
		//m_lstComposition.InsertItem( nItem, szSubGraphName);
		strUIText = _MTOL3( szSubGraphName);

		m_lstComposition.InsertItem( nItem, strUIText );
// 2008.01.25 by LYH <<
		m_pIGraphInfo->GetSubGraphShowFlag( nItem, bShowFlag);
		m_lstComposition.SetCheck( nItem, bShowFlag);
	}
}

void CSetting_Common_Type_A::Init_Composition_Group_SubGraphTitle( int nSubGraphIndex )
{
	if(nSubGraphIndex < 0) return;

	// Title
	ILPCSTR szSubGraphTitle = m_pIGraphInfo->GetSubGraphTitleString( nSubGraphIndex);
// 2008.01.25 by LYH >>
	//m_edtCompositionTitleData.SetWindowText( szSubGraphTitle );
	CString strUIText = _MTOL1_N( szSubGraphTitle); 
	if( strUIText.IsEmpty()) strUIText = _MTOL3( szSubGraphTitle);

	m_edtCompositionTitleData.SetWindowText( strUIText );
// 2008.01.25 by LYH <<
}

void CSetting_Common_Type_A::Init_Composition_Group_SubGraphLine( int nSubGraphIndex )
{
	if(nSubGraphIndex < 0) return;

	// 선굵기
	if( m_cmbThickness.GetCount() == 0 )
		Initial_ThicknessComboBox( nSubGraphIndex, 0, m_cmbThickness );
	else
		m_cmbThickness.SetCurSel( m_pIGraphInfo->GetSubGraphPenThickness( nSubGraphIndex, 1) - 1);

	// 선모양
	if( m_cmbLineType.GetCount() == 0 )
		Initial_DrawStyleComboBox( nSubGraphIndex, m_cmbLineType );
	else
		m_cmbLineType.SetCurSel( m_pIGraphInfo->GetSubGraphDrawStyle( nSubGraphIndex));
}

// 기준선 두께, 모양 설정 - ojtaso (20071213)
void CSetting_Common_Type_A::Init_Composition_Group_BaseLine( int nSubGraphIndex )
{
	if(nSubGraphIndex < 0) return;

	// 선굵기
	if( m_cmbThickness2.GetCount() == 0 )
		Initial_BaseLineThicknessComboBox( nSubGraphIndex, 0, m_cmbThickness2 );
	else
		m_cmbThickness2.SetCurSel( m_pIGraphInfo->GetBaseLinePenThickness( nSubGraphIndex, 1) - 1);

	// 선모양
	if( m_cmbLineType2.GetCount() == 0 )
		Initial_DrawBaseLineStyleComboBox( nSubGraphIndex, m_cmbLineType2 );
	else
		m_cmbLineType2.SetCurSel( m_pIGraphInfo->GetSubGraphDrawBaseLineStyle( nSubGraphIndex));
}

void CSetting_Common_Type_A::Init_Composition_Group_SubGraphColor( int nSubGraphIndex )
{
	if(nSubGraphIndex < 0) return;

	// "선형"
	if(Init_Composition_Group_SubGraphColor_LineType( nSubGraphIndex ))
		return;

	// "바형"
	if(Init_Composition_Group_SubGraphColor_BoxType( nSubGraphIndex ))
		return;

	UseSubColorBtn( FALSE );
	SetBottonColor(m_btnComositionColor, m_pIGraphInfo->GetSubGraphColor( nSubGraphIndex, 1));
}


bool CSetting_Common_Type_A::Init_Composition_Group_SubGraphColor_LineType( const int nSubGraphIndex )
{
	if( m_pIGraphInfo->GetSubGraphType( nSubGraphIndex) != CGraphBaseData::Line_Type) return false;

	CGraphBaseData::LINESTYLE eLineStyle = ( CGraphBaseData::LINESTYLE)m_pIGraphInfo->GetSubGraphStyle( nSubGraphIndex);
	
	// 지표 스타일이 "채움식"일 때에는 지표선의 선 색상이 다르게 적용된다.
	if( eLineStyle ==  CGraphBaseData::FillLine)
	{
// 2008.01.25 by LYH >>
		//UseSubColorBtn( TRUE, "선색" );
		CString strColor;
		strColor.LoadString(IDS_LINE_COLOR);
		UseSubColorBtn( TRUE, strColor );
// 2008.01.25 by LYH <<

		UseFillColorBtn( TRUE );
		SetBottonColor(m_btnUpColor, m_pIGraphInfo->GetSubGraphColor( nSubGraphIndex, 1));
		SetBottonColor(m_btnDownColor, m_pIGraphInfo->GetSubGraphColor( nSubGraphIndex, 3));
		SetBottonColor(m_btnSteadyColor, m_pIGraphInfo->GetSubGraphColor( nSubGraphIndex, 5));
		
		int SubGraphData( nSubGraphIndex );
		Initial_FillUp( nSubGraphIndex, m_btnFillColor );
	}

	// 지표 스타일이 "선형구름식", "구름식" 일 때에는 지표선의 선 색상이 다르게 적용된다.
	else if( eLineStyle == CGraphBaseData::LineCloudLine || eLineStyle == CGraphBaseData::CloudLine )
	{
		UseSubColorBtn( TRUE, "", FALSE );
		UseFillColorBtn( FALSE );
		SetBottonColor(m_btnUp1Color, m_pIGraphInfo->GetSubGraphColor( nSubGraphIndex, 1));
		SetBottonColor(m_btnDown1Color, m_pIGraphInfo->GetSubGraphColor( nSubGraphIndex, 3));
	}

	// Sang-Woo, Cho		2006 / 06 / 07
	// "그물차트" 일때 다르게 처리하게 수정.
	// Begin
	else if( eLineStyle == CGraphBaseData::RainbowLine )
	{
		UseSubColorBtn( TRUE, "", FALSE );
		UseFillColorBtn( FALSE );
		SetBottonColor(m_btnUp1Color, m_pIGraphInfo->GetSubGraphColor( nSubGraphIndex, 1));
		SetBottonColor(m_btnDown1Color, m_pIGraphInfo->GetSubGraphColor( nSubGraphIndex, 2));
	}
	// End
	
	else
	{
		UseSubColorBtn( FALSE );
		UseFillColorBtn( FALSE );
		SetBottonColor(m_btnComositionColor, m_pIGraphInfo->GetSubGraphColor( nSubGraphIndex, 1));
	}

	return true;
}

bool CSetting_Common_Type_A::Init_Composition_Group_SubGraphColor_BoxType( const int nSubGraphIndex )
{
	if( m_pIGraphInfo->GetSubGraphType( nSubGraphIndex) != CGraphBaseData::Bar_Type) return false;

	CGraphBaseData::BARSTYLE eBarStyle = ( CGraphBaseData::BARSTYLE)m_pIGraphInfo->GetSubGraphStyle( nSubGraphIndex);
	
	// 지표 스타일이 "박스형"일 때에는 지표선의 선 색상이 다르게 적용된다.
	if( eBarStyle ==  CGraphBaseData::BoxBar)
	{
// 2008.01.25 by LYH >>
		//UseSubColorBtn( TRUE, "보합" );
		CString strSteady;
		strSteady.LoadString(IDS_STRING_STEADY);
		UseSubColorBtn( TRUE, strSteady );
// 2008.01.25 by LYH <<
		SetBottonColor(m_btnUpColor, m_pIGraphInfo->GetSubGraphColor( nSubGraphIndex, 1));
		SetBottonColor(m_btnDownColor, m_pIGraphInfo->GetSubGraphColor( nSubGraphIndex, 3));
		SetBottonColor(m_btnSteadyColor, m_pIGraphInfo->GetSubGraphColor( nSubGraphIndex, 5));
	}

	// 지표 스타일이 "Dot형"일 때에는 지표선의 선 색상이 다르게 적용된다.
	// 고정비율상하식 추가 - ojtaso (20080603)
	else if( eBarStyle ==  CGraphBaseData::DotBar || eBarStyle ==  CGraphBaseData::UpDownBar || eBarStyle ==  CGraphBaseData::FixedRateUpDownBar)
	{
		UseSubColorBtn( TRUE, "", FALSE );
		SetBottonColor(m_btnUp1Color, m_pIGraphInfo->GetSubGraphColor( nSubGraphIndex, 1));
		SetBottonColor(m_btnDown1Color, m_pIGraphInfo->GetSubGraphColor( nSubGraphIndex, 3));
	}
	else
	{
		UseSubColorBtn( FALSE );
		SetBottonColor(m_btnComositionColor, m_pIGraphInfo->GetSubGraphColor( nSubGraphIndex, 1));
	}

	return true;
}

void CSetting_Common_Type_A::SetBottonColor(CColorButton& colorButton, const COLORREF& color)
{
	colorButton.SetColor(color);
	colorButton.Invalidate();
}
//sy end


//-----------------------------------------------------------------------------
// Writer		: Sang-Woo, Cho		Date : 2005 / 10 / 11
// Return  void : 
// Param  BOOL bUseSubColorBtn : 
// Param  CString szBtnText   : 
// Explain		: 채움형일때와 박스형일때 색상설정 버튼을 달리한다.
//-----------------------------------------------------------------------------
void CSetting_Common_Type_A::UseSubColorBtn( BOOL bUseSubColorBtn, CString szBtnText, BOOL bUseThirdBtn )
{
// 2008.01.25 by LYH >>
	//m_btnUpColor.SetText( "상승" );
	//m_btnDownColor.SetText( "하락" );
	m_btnUpColor.SetText( m_strUp );
	m_btnDownColor.SetText( m_strDown );
// 2008.01.25 by LYH <<

	// Sang-Woo, Cho		2006 / 06 / 07
	// "그물차트" 일때 수정
	// Begin
	ILPCSTR szGraphName = m_pIGraphInfo->GetIndicatorNameString();
// 2008.01.25 by LYH >>
	//if( !strcmp( szGraphName, "그물차트"))
	if( !strcmp( szGraphName, _MTEXT( C2_RAINBOW_CHART )))
	{
//		m_btnUp1Color.SetText( "시작색" );
//		m_btnDown1Color.SetText( "끝색" );
		CString strStart, strEnd;
		strStart.LoadString(IDS_COLOR_START);
		strEnd.LoadString(IDS_COLOR_END);
		m_btnUp1Color.SetText( strStart );
		m_btnDown1Color.SetText( strEnd );
	}
	else
	{
		//m_btnUp1Color.SetText( "상승" );
		//m_btnDown1Color.SetText( "하락" );
		m_btnUp1Color.SetText( m_strUp );
		m_btnDown1Color.SetText( m_strDown );
	}
// 2008.01.25 by LYH <<
	// End

	m_btnSteadyColor.SetText( szBtnText );

	if( bUseSubColorBtn )
	{
		GetDlgItem(IDC_COMPOSITION_LINETITLE)->ShowWindow( SW_HIDE );
		GetDlgItem(IDC_BTN_COMPOSITION_COLOR)->ShowWindow( SW_HIDE );
		
		if( bUseThirdBtn )
		{
			GetDlgItem(IDC_BTN_UP1)->ShowWindow( SW_HIDE );
			GetDlgItem(IDC_BTN_DOWN1)->ShowWindow( SW_HIDE );

			GetDlgItem(IDC_BTN_UP_COLOR)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_DOWN_COLOR)->ShowWindow(SW_SHOW);
			GetDlgItem( IDC_BTN_STEADY_COLOR)->ShowWindow(SW_SHOW);
		}
					
		else
		{
			GetDlgItem(IDC_BTN_UP_COLOR)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_DOWN_COLOR)->ShowWindow(SW_HIDE);
			GetDlgItem( IDC_BTN_STEADY_COLOR)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_BTN_UP1)->ShowWindow( SW_SHOW );
			GetDlgItem(IDC_BTN_DOWN1)->ShowWindow( SW_SHOW );
		}
	}

	else
	{
		GetDlgItem(IDC_COMPOSITION_LINETITLE)->ShowWindow( SW_SHOW );
		GetDlgItem(IDC_BTN_COMPOSITION_COLOR)->ShowWindow( SW_SHOW );

		GetDlgItem(IDC_BTN_UP_COLOR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_DOWN_COLOR)->ShowWindow(SW_HIDE);
		GetDlgItem( IDC_BTN_STEADY_COLOR)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_BTN_UP1)->ShowWindow( SW_HIDE );
		GetDlgItem(IDC_BTN_DOWN1)->ShowWindow( SW_HIDE );
	}
}


void CSetting_Common_Type_A::UseFillColorBtn( BOOL bUseFillColorBtn )
{
	if( bUseFillColorBtn == TRUE )
		GetDlgItem( IDC_CHK_FILL_COLOR )->ShowWindow( SW_SHOW );

	else
		GetDlgItem( IDC_CHK_FILL_COLOR )->ShowWindow( SW_HIDE );
}

// Singbar Graph 색상 비교기준 추가 - ojtaso (20071121)
void CSetting_Common_Type_A::UseCompColorCtrl(BOOL bUseCompColorCtrl)
{
	if( bUseCompColorCtrl == TRUE )
	{
		GetDlgItem( IDC_STATIC_COMPCOLOR )->ShowWindow( SW_SHOW );
		m_cmbCompColor.ShowWindow( SW_SHOW );
	}
	else
	{
		GetDlgItem( IDC_STATIC_COMPCOLOR )->ShowWindow( SW_HIDE );
		m_cmbCompColor.ShowWindow( SW_HIDE );
	}
}

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 10
// Return void : 
// Param  CList< double : 
// Param  double >* pBaseLineList : 
// Comments		: 
//-----------------------------------------------------------------------------
void CSetting_Common_Type_A::Init_BaseLine_Group_Title( void)
{
	for(int i = 0 ; i< m_HorizonLine_ListBox.GetCount() ; i++)
		m_HorizonLine_ListBox.DeleteString(i);
	m_HorizonLine_ListBox.ResetContent();

	double dFirst = 0;
	IDoubleList *pIDblBase = m_pIGraphInfo->GetBaseLineList();
	if( pIDblBase)
	{
		Initial_ListBox(  pIDblBase, m_HorizonLine_ListBox);

		if( !pIDblBase->IsEmpty()) pIDblBase->GetHead( dFirst);
		pIDblBase->Release();
	}

	ILPCSTR szIndicatorName = m_pIGraphInfo->GetGraphNameString();
	CBaseData::LIMITHORIZONTALLINE type = CFindType::FindLimitValueofHorizontalLine( szIndicatorName);
	if(type == CBaseData::ZeroToOneZeroZero)			Initial_Spin(m_Horizon_Spin, m_Horizon_Edit, dFirst, 0, 100, 1);
	else if(type == CBaseData::MinusZeroToOneZeroZero)	Initial_Spin(m_Horizon_Spin, m_Horizon_Edit, dFirst, -100, -1, 1);
	else												Initial_Spin(m_Horizon_Spin, m_Horizon_Edit, dFirst, -999999, 999999, 1);
	
	// 맨 마지막 선의 색깔이 기준선 색이된다.
	m_Color_Bt.SetColor( m_pIGraphInfo->GetSubGraphColor( m_pIGraphInfo->GetSubGraphCount() - 1, 6));

	//sy 2006.04.17.
	Initial_Signal(m_pIGraphInfo);
}

// Singbar Graph 색상 비교기준 추가 - ojtaso (20071121)
void CSetting_Common_Type_A::Init_BarGraphCompareColor()
{
	if( m_pIGraphInfo->GetSubGraphType( m_nSubGraphIndex) != CGraphBaseData::Bar_Type) 
	{
		UseCompColorCtrl(FALSE);
		return;
	}

	int nPaintStyle = m_pIGraphInfo->GetSubGraphPaintStyle(m_nSubGraphIndex);
	if(m_bUseMultiplePriceData || nPaintStyle < 0)
		nPaintStyle = 1;		// 초기값은 기준선

	m_cmbCompColor.SetCurSel(nPaintStyle);
}

double CSetting_Common_Type_A::GetFirstValue(const CString &data)
{
	CString horizon = data;
	return atof(CDataConversion::GetStringData(horizon, ","));
}

void CSetting_Common_Type_A::DeleteString_ListBox()
{
	for(int i = 0 ; i< m_HorizonLine_ListBox.GetCount() ; i++)
		m_HorizonLine_ListBox.DeleteString(i);
	m_HorizonLine_ListBox.ResetContent();
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 10
// Return void : 
// Comments		: 
//-----------------------------------------------------------------------------
void CSetting_Common_Type_A::OnBtnCompositionColor() 
{
	// TODO: Add your control notification handler code here
	if(m_nSubGraphIndex < 0) return;
	
	COLORREF crColor = m_btnComositionColor.GetColor();
	CColorDialog color(crColor, CC_FULLOPEN);
	if(color.DoModal() == IDOK) 
	{
		// 지표 스타일이 "채움식" 일 때에는 지표선의 선 색상이 다르게 적용된다.
		if( m_pIGraphInfo->GetSubGraphStyle( m_nSubGraphIndex) ==  CGraphBaseData::FillLine )
		{
			m_pIGraphInfo->SetSubGraphColor( m_nSubGraphIndex, 5, color.GetColor());
			m_btnComositionColor.SetColor( color.GetColor());
		}		
		else
		{
			m_pIGraphInfo->SetSubGraphColor( m_nSubGraphIndex, 1, color.GetColor());

			//바차트의 Fill처리.
			m_pIGraphInfo->SetSubGraphColor( m_nSubGraphIndex, 2, color.GetColor());

			ILPCSTR szGraphName = m_pIGraphInfo->GetGraphNameString();
			if(!strcmp( szGraphName,  _MTEXT( C2_LINE_PRICE_MA )))
			{
				m_pIGraphInfo->SetSubGraphColor( m_nSubGraphIndex, 3, color.GetColor());
				m_pIGraphInfo->SetSubGraphColor( m_nSubGraphIndex, 5, color.GetColor());
				m_pIGraphInfo->SetSubGraphColor( m_nSubGraphIndex, 6, color.GetColor());
			}
			m_btnComositionColor.SetColor( color.GetColor());
		}
		
		m_btnComositionColor.Invalidate();
	}
}


void CSetting_Common_Type_A::OnClickListComposition(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here

//	m_lstComposition.SetItemState(pNMListView->iItem, LVIS_SELECTED, LVIF_STATE); 
	
	// 1. 최소 하나 이상의 구성 설정이 체크되어 있어야 한다.
	OnApplySubGraphData();
	Initial_SubGraphData( pNMListView->iItem );
	
	*pResult = 0;
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 12
// Return void : 
// Param  void : 
// Comments		: 
//-----------------------------------------------------------------------------
void CSetting_Common_Type_A::OnApplySubGraphData( void )
{
	// 선색상은 OnCompositioncolor에서 바로 적용
	m_pIGraphInfo->SetSubGraphShowFlag( m_nSubGraphIndex, m_lstComposition.GetCheck( m_nSubGraphIndex));

	OnApply_Title( m_nSubGraphIndex, m_edtCompositionTitleData);
	OnApply_DrawStyle( m_nSubGraphIndex, m_cmbLineType);
	OnApply_Thickness( m_nSubGraphIndex, m_cmbThickness);

	if( m_btnFillColor.IsWindowVisible() == TRUE )
	{
		OnApply_FillUp( m_nSubGraphIndex, m_btnFillColor );
		OnApply_FillDown( m_nSubGraphIndex, m_btnFillColor );
	}

	ILPCSTR szIndicatorName = m_pIGraphInfo->GetIndicatorNameString();
// 2008.01.25 by LYH >>
//	if( !strcmp( szIndicatorName, "상대비교차트")) return;
	if( !strcmp( szIndicatorName, _MTEXT(C2_COMPARISON_CHART))) return;
// 2008.01.25 by LYH <<

	IDoubleList *pIDblCnd = m_pIGraphInfo->GetCommonCalculateConditionList();
	if( pIDblCnd)
	{
		CString strData;
		double dData = 0, dNewData = 0;
		int i = 0;
		CNumEdit *pNEdit[ 5] = { &m_edtValue1, &m_edtValue2, &m_edtValue3, &m_edtValue4, &m_edtValue5	};
		POSITION psnCnd = pIDblCnd->GetHeadPosition(), psnBack = NULL;
		while( psnCnd && i < 5)
		{
			psnBack = psnCnd;
			pIDblCnd->GetNext( psnCnd, dData);

			pNEdit[ i]->GetWindowText( strData);
			i++;

			dNewData = atof( strData);
			if(!CBaseDialog::m_bDoCalculate) CBaseDialog::m_bDoCalculate = ( dData != dNewData);
			pIDblCnd->SetAt( psnBack, dNewData);
		}

		pIDblCnd->Release();
	}

	if( !m_bUseMultiplePriceData)
	{
		ILPCSTR szSubPacketName = m_pIGraphInfo->GetSubGraphPacketNameString( 0);
		if( szSubPacketName.IsEmpty()) OnApply_PacketData( m_cmbData, m_pIGraphInfo );
		else  OnApply_SubPacketData(m_cmbData, m_pIGraphInfo, m_nSubGraphIndex);
	}

	// 기준선 관련 저장
	OnApplyBaseLine();

	// Singbar Graph 색상 비교기준 추가 - ojtaso (20071121)
	OnApplyPaintStyle();
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 11
// Return void : 
// Param  const int nIndex : 
// Comments		: 
//-----------------------------------------------------------------------------
void CSetting_Common_Type_A::Initial_SubGraphData( const int nIndex )
{
	if( m_pIGraphInfo == NULL ) return;

	int nOldSubGraphIndex = m_nSubGraphIndex;
	if( nIndex > m_pIGraphInfo->GetSubGraphCount() - 1 ) m_nSubGraphIndex = m_pIGraphInfo->GetSubGraphCount() - 1;
	else m_nSubGraphIndex = ( nIndex < 0 ? 0 : nIndex);

	if( 0 <= nOldSubGraphIndex && nOldSubGraphIndex == m_nSubGraphIndex) return ;
	
	Initial_Title( m_nSubGraphIndex, m_edtCompositionTitleData);
	Initial_DrawStyleComboBox(m_nSubGraphIndex, m_cmbLineType);
	SetStatePacketCombo( m_pIGraphInfo, m_cmbData, m_stcDataTitle);
	m_cmbThickness.SetCurSel( m_pIGraphInfo->GetSubGraphPenThickness( m_nSubGraphIndex, 1) - 1);

	//sy 2005.11.02.
	Init_Composition_Group_SubGraphColor(m_nSubGraphIndex);
}



//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 11
// Return void : 
// Param  NMHDR* pNMHDR : 
// Param  LRESULT* pResult : 
// Comments		: 구성 설정을 Click하면 
//-----------------------------------------------------------------------------
void CSetting_Common_Type_A::OnItemchangedListComposition(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	*pResult = 0;

	if(pNMListView->uOldState == 0 && pNMListView->uNewState == 0)
		return;	// No change

	// Old check box state
	BOOL bPrevState = (BOOL)(((pNMListView->uOldState & LVIS_STATEIMAGEMASK)>>12)-1);   // Old check box state
	if (bPrevState < 0)			// On startup there's no previous state 
		bPrevState = 0;			// so assign as false (unchecked)

	// New check box state
	BOOL bChecked=(BOOL)(((pNMListView->uNewState & LVIS_STATEIMAGEMASK)>>12)-1);   
	if (bChecked < 0)		// On non-checkbox notifications assume false
		bChecked = 0; 

	if (bPrevState == bChecked) // No change in check box
		return;

	if( pNMListView->iItem < 0 )
		pNMListView->iItem = 0;

	BOOL bReCheck = FALSE;
	// 마지막으로 선택된 Item이 해지되는것을 막아준다.
	if( bPrevState == 1 && bChecked == 0 )
	{
		int nCount = m_lstComposition.GetItemCount();	
		int nChecked = 0;
		for( int nItem = 0 ; nItem < nCount ; nItem++ )
		{
			if( m_lstComposition.GetCheck( nItem ) == TRUE )
				nChecked++;
		}

		m_pIGraphInfo->SetSubGraphShowFlag( pNMListView->iItem, FALSE);
		if( nChecked < 1 )
		{
// 2008.01.25 by LYH >>
			//AfxMessageBox("최소 하나 이상의 구성설정이 존재해야 됩니다.");
			AfxMessageBox(m_strMsgExistMin);
// 2008.01.25 by LYH <<
			m_lstComposition.SetCheck( pNMListView->iItem );
			m_pIGraphInfo->SetSubGraphShowFlag( pNMListView->iItem, TRUE);
			m_lstComposition.SetHotItem( pNMListView->iItem );

			OnApplySubGraphData();
			Initial_SubGraphData( pNMListView->iItem );
			bReCheck = TRUE;
		}
	}

	if(!m_bInitComplite)
	{
		// 그래프 추가
		if(bChecked) m_pIGraphInfo->SetSubGraphShowFlag( pNMListView->iItem, TRUE);
		// 그래프 제거
		else m_pIGraphInfo->SetSubGraphShowFlag( pNMListView->iItem, FALSE);

		if( bReCheck) m_pIGraphInfo->SetSubGraphShowFlag( pNMListView->iItem, TRUE);
	}
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 12
// Return void : 
// Param  int nItem : 
// Comments		: 
//-----------------------------------------------------------------------------
void CSetting_Common_Type_A::OnSelchangeCmbLinetype() 
{
	// TODO: Add your control notification handler code here
	if( m_cmbThickness.GetCurSel() != 0 && m_cmbLineType.GetCurSel() != 0 )
	{
		// Sang-Woo, Cho		2006 / 06 / 07
		// 그물차트 일 경우에는 해당되지 않는다.
		// Begin
		ILPCSTR szGraphName = m_pIGraphInfo->GetGraphNameString();
// 2008.01.25 by LYH >>
//		if( strcmp( szGraphName, "그물차트")) 
		if(!strcmp( szGraphName,  _MTEXT( C2_LINE_PRICE_MA )))
		{
			m_cmbThickness.EnableWindow(TRUE);
			return;
		}
		else if( strcmp( szGraphName,  _MTEXT( C2_RAINBOW_CHART )))
			//AfxMessageBox("선모양이 \"실선\"인 경우만 굵기를 변경할 수 있습니다.");
			AfxMessageBox(m_strMsgLineThick);
// 2008.01.25 by LYH <<
		// End
		
		m_cmbThickness.SetCurSel(0);
		m_cmbThickness.EnableWindow(FALSE);
		return;
	}

	else
		m_cmbThickness.EnableWindow(TRUE);
}

void CSetting_Common_Type_A::OnSelchangeCmbThickness() 
{
	// TODO: Add your control notification handler code here
	if( m_cmbThickness.GetCurSel() != 0 && m_cmbLineType.GetCurSel() != 0 )
	{
		// Sang-Woo, Cho		2006 / 06 / 07
		// Box Chart일 경우에는 해당되지 않는다.
		// Begin
		ILPCSTR szGraphName = m_pIGraphInfo->GetGraphNameString();
// 2008.01.25 by LYH >>
		//if( strcmp( szGraphName, "그물차트") && strcmp( szGraphName, "Box Chart"))

		if(!strcmp( szGraphName,  _MTEXT( C2_LINE_PRICE_MA )))
		{
			m_cmbLineType.EnableWindow(TRUE);
			return;
		}
		else if( strcmp( szGraphName,  _MTEXT( C2_RAINBOW_CHART )) && strcmp( szGraphName,  _MTEXT( C2_BOX_CHART )))
			//AfxMessageBox("선굵기가 \"1pt\"인 경우만 선모양을 변경할 수 있습니다.");
			AfxMessageBox(m_strMsgLineShape);
// 2008.01.25 by LYH <<
		// End
		
		m_cmbLineType.SetCurSel(0);
		m_cmbLineType.EnableWindow(FALSE);
		return;
	}	

	else
		m_cmbLineType.EnableWindow(TRUE);	
}


void CSetting_Common_Type_A::OnInsert() 
{
	// TODO: Add your control notification handler code here
	// 20081014 박동관 >>
	CString temp;
	UpdateData(TRUE);

	CString strType = GetPacketType();

	// 기준선 두께, 모양 설정 - ojtaso (20071213)
	//temp.Format("%f",m_dHorizonValue);

	temp = m_strHorizonValue;
	
	// 아무것도 입력이 안되면 그냥 리턴한다.
	temp.TrimLeft(); temp.TrimRight();
	if(temp == "")
		return ;
	// 20081014 박동관 <<
	
	int nCount = m_HorizonLine_ListBox.GetCount();

	// 20081017 이문수 >>
	if(CDataConversion::IsBaseFormat(strType)) 
	{								
		CString strOutput;
		if(!CDataConversion::GetDataToDec(temp, strType, strOutput)) 
			temp = "0";
		else
			temp = strOutput;
	}
	temp = CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans(temp, strType);
	// 20081017 이문수 <<
	
	// 20081030 박동관 >>
	temp.Remove('%');
	// 20081030 박동관 <<

	CString szText = "";
	for( int i = 0 ; i < nCount ; i++ )
	{
		m_HorizonLine_ListBox.GetText( i, szText );
		if( szText == temp )
			return;
	}

	// 20081014 박동관 >>
	//m_HorizonLine_ListBox.AddString(TrimStringWhenDecimalDataIsZero(temp));

	if(CDataConversion::IsBaseFormat(strType))
	{
		// 입력된 문자열이 알맞은 진법 입력인지 판별한다.
		BOOL bResult = CDataConversion::IsValid8_32(temp);

		// 만약 문자열에 알맞은 문자열이 아니면 메세지박스를 띄운다.
		if(!bResult)
		{
			AfxMessageBox("입력된 문자열이 알맞은 진법이 아닙니다.");
			m_Horizon_Edit.SetFocus();
			m_Horizon_Edit.SetSel(0, -1);
			return ;
		}
	}
	m_HorizonLine_ListBox.AddString(temp);		// 진법변환이 아닐 경우...
	// 20081014 박동관 <<

	// 20081017 이문수 >>
	m_strHorizonValue = temp;
	UpdateData(FALSE);
	// 20081017 이문수 <<
}


void CSetting_Common_Type_A::OnDelete() 
{
	// TODO: Add your control notification handler code here
	m_HorizonLine_ListBox.DeleteString(m_HorizonLine_ListBox.GetCurSel());	
}


void CSetting_Common_Type_A::OnAlldelete() 
{
	// TODO: Add your control notification handler code here
	DeleteString_ListBox();
	IDoubleList *pIDblBase = m_pIGraphInfo->GetBaseLineList();
	if( pIDblBase)
	{
		pIDblBase->RemoveAll();
		pIDblBase->Release();
	}

	m_Horizon_Edit.SetValue( 0);	
}


void CSetting_Common_Type_A::OnColorButton() 
{
	// TODO: Add your control notification handler code here
	COLORREF crColor = m_Color_Bt.GetColor();
	CColorDialog color(crColor, CC_FULLOPEN);
	if(color.DoModal() != IDOK)
		return;

	for(int i = 0; i < m_pIGraphInfo->GetSubGraphCount(); i++)
		m_pIGraphInfo->SetSubGraphColor( i, 6, color.GetColor());
	
	m_Color_Bt.SetColor( m_pIGraphInfo->GetSubGraphColor( 0, 6));
	m_Color_Bt.Invalidate();		
}


BOOL CSetting_Common_Type_A::PreTranslateMessage(MSG* pMsg) 
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
	

	return CDialog::PreTranslateMessage(pMsg);
}


//-----------------------------------------------------------------------------
// Writer		: Sang-Woo, Cho		Date : 2005 / 10 / 11
// Return  void : 
// Param   : 
// Explain		: "채움식"이나 "박스형"일때 "상승" 색상을 설정한다. 
//-----------------------------------------------------------------------------
void CSetting_Common_Type_A::OnBtnUpColor() 
{
	// TODO: Add your control notification handler code here
	if(m_nSubGraphIndex < 0) return;
	
	COLORREF crColor = m_btnUpColor.GetColor();
	CColorDialog color(crColor, CC_FULLOPEN);
	if(color.DoModal() == IDOK) 
	{
		m_pIGraphInfo->SetSubGraphColor( m_nSubGraphIndex, 1, color.GetColor());
		m_pIGraphInfo->SetSubGraphColor( m_nSubGraphIndex, 2, color.GetColor());
		m_btnUpColor.SetColor( m_pIGraphInfo->GetSubGraphColor( m_nSubGraphIndex, 1));
	}	
	
	m_btnUpColor.Invalidate();
}


//-----------------------------------------------------------------------------
// Writer		: Sang-Woo, Cho		Date : 2005 / 10 / 11
// Return  void : 
// Param   : 
// Explain		: "채움식"이나 "박스형"일때 "하락" 색상을 설정한다. 
//-----------------------------------------------------------------------------
void CSetting_Common_Type_A::OnBtnDownColor() 
{
	// TODO: Add your control notification handler code here
	if(m_nSubGraphIndex < 0) return;
	
	COLORREF crColor = m_btnDownColor.GetColor();
	CColorDialog color(crColor, CC_FULLOPEN);
	if(color.DoModal() == IDOK) 
	{
		m_pIGraphInfo->SetSubGraphColor( m_nSubGraphIndex, 3, color.GetColor());
		m_pIGraphInfo->SetSubGraphColor( m_nSubGraphIndex, 4, color.GetColor());
		m_btnDownColor.SetColor( m_pIGraphInfo->GetSubGraphColor( m_nSubGraphIndex, 3));
	}
	
	m_btnDownColor.Invalidate();
}


//-----------------------------------------------------------------------------
// Writer		: Sang-Woo, Cho		Date : 2005 / 10 / 11
// Return  void : 
// Param   : 
// Explain		: "채움식"이나 "박스형"일때 "보합" 색상을 설정한다. 
//-----------------------------------------------------------------------------
void CSetting_Common_Type_A::OnBtnSteadyColor() 
{
	// TODO: Add your control notification handler code here
	if(m_nSubGraphIndex < 0) return;
	
	COLORREF crColor = m_btnSteadyColor.GetColor();
	CColorDialog color(crColor, CC_FULLOPEN);
	if(color.DoModal() == IDOK) 
	{
		// 지표 스타일이 "채움식" 일 때에는 지표선의 선 색상이 다르게 적용된다.
		if( m_pIGraphInfo->GetSubGraphStyle( m_nSubGraphIndex) ==  CGraphBaseData::FillLine )
		{
			m_pIGraphInfo->SetSubGraphColor( m_nSubGraphIndex, 5, color.GetColor());

// (2009/10/8 - Seung-Won, Bae) why? only psychological line? how about rsi?
//			ILPCSTR szGraphName = m_pIGraphInfo->GetGraphNameString();
//			if( strcmp( szGraphName, "Psychological Line"))
//				m_pIGraphInfo->SetSubGraphColor( m_nSubGraphIndex, 6, color.GetColor());
			
			m_btnSteadyColor.SetColor( m_pIGraphInfo->GetSubGraphColor( m_nSubGraphIndex, 5));
		}

		else if( m_pIGraphInfo->GetSubGraphStyle( m_nSubGraphIndex) ==  CGraphBaseData::BoxBar )
		{
			m_pIGraphInfo->SetSubGraphColor( m_nSubGraphIndex, 5, color.GetColor());
			m_btnSteadyColor.SetColor( m_pIGraphInfo->GetSubGraphColor( m_nSubGraphIndex, 5));
		}
	}	
	
	m_btnSteadyColor.Invalidate();
}


//-----------------------------------------------------------------------------
// Writer		: Sang-Woo, Cho		Date : 2005 / 10 / 12
// Return  void : 
// Param   : 
// Explain		: 
//-----------------------------------------------------------------------------
void CSetting_Common_Type_A::OnBtnUp1() 
{
	// TODO: Add your control notification handler code here
	if(m_nSubGraphIndex < 0) return;
	
	COLORREF crColor = m_btnUp1Color.GetColor();
	CColorDialog color(crColor, CC_FULLOPEN);
	if(color.DoModal() == IDOK) 
	{
		// Sang-Woo, Cho		2006 / 06 / 07
		// "그물차트" 일때의 처리를 달리함.
		// Begin
		ILPCSTR szGraphName = m_pIGraphInfo->GetGraphNameString();
// 2008.01.25 by LYH >>
		//if( !strcmp( szGraphName, "그물차트"))
		if( !strcmp( szGraphName,  _MTEXT( C2_RAINBOW_CHART )))
// 2008.01.25 by LYH <<
			m_pIGraphInfo->SetSubGraphColor( m_nSubGraphIndex, 1, color.GetColor());
		else
		{
			m_pIGraphInfo->SetSubGraphColor( m_nSubGraphIndex, 1, color.GetColor());
			m_pIGraphInfo->SetSubGraphColor( m_nSubGraphIndex, 2, color.GetColor());
		}

		m_btnUp1Color.SetColor( m_pIGraphInfo->GetSubGraphColor( m_nSubGraphIndex, 1));
		// End
	}	
	
	m_btnUp1Color.Invalidate();	
}


//-----------------------------------------------------------------------------
// Writer		: Sang-Woo, Cho		Date : 2005 / 10 / 12
// Return  void : 
// Param   : 
// Explain		: 
//-----------------------------------------------------------------------------
void CSetting_Common_Type_A::OnBtnDown1() 
{
	// TODO: Add your control notification handler code here
	if(m_nSubGraphIndex < 0) return;
	
	COLORREF crColor = m_btnDown1Color.GetColor();
	CColorDialog color(crColor, CC_FULLOPEN);
	if(color.DoModal() == IDOK) 
	{
		// Sang-Woo, Cho		2006 / 06 / 07
		// "그물차트" 일때의 처리를 달리함.
		// Begin
		ILPCSTR szGraphName = m_pIGraphInfo->GetGraphNameString();
// 2008.01.25 by LYH >>
//		if( !strcmp( szGraphName, "그물차트"))
		if( !strcmp( szGraphName,  _MTEXT( C2_RAINBOW_CHART )))
// 2008.01.25 by LYH <<
		{
			m_pIGraphInfo->SetSubGraphColor( m_nSubGraphIndex, 2, color.GetColor());
			m_btnDown1Color.SetColor( m_pIGraphInfo->GetSubGraphColor( m_nSubGraphIndex, 2));
		}

		else
		{
			m_pIGraphInfo->SetSubGraphColor( m_nSubGraphIndex, 3, color.GetColor());
			m_pIGraphInfo->SetSubGraphColor( m_nSubGraphIndex, 4, color.GetColor());
			m_btnDown1Color.SetColor( m_pIGraphInfo->GetSubGraphColor( m_nSubGraphIndex, 3));
		}
		// End
	}
	
	m_btnDown1Color.Invalidate();	
}

void CSetting_Common_Type_A::OnChkFillColor() 
{
	// TODO: Add your control notification handler code here
// 	m_btnFillColor.SetCheck( )
}

void CSetting_Common_Type_A::OnSelchangeCmbThickness2() 
{
	if(m_cmbLineType2.GetCurSel() != 0 && m_cmbThickness2.GetCurSel() != 0 )
	{
// 2008.01.25 by LYH >>
		//AfxMessageBox("선모양이 \"실선\"인 경우만 굵기를 변경할 수 있습니다.");
		AfxMessageBox(m_strMsgLineThick);
// 2008.01.25 by LYH <<
		m_cmbThickness2.SetCurSel(0);
	}
}

void CSetting_Common_Type_A::OnSelchangeCmbLinetype2() 
{
	if(m_cmbLineType2.GetCurSel() != 0 && m_cmbThickness2.GetCurSel() != 0 )
	{
// 2008.01.25 by LYH >>
		//AfxMessageBox("선모양이 \"실선\"인 경우만 굵기를 변경할 수 있습니다.");
		AfxMessageBox(m_strMsgLineThick);
// 2008.01.25 by LYH <<
		m_cmbLineType2.SetCurSel(0);
	}
}

// 20081014 박동관 >>
void CSetting_Common_Type_A::OnSelchangeHorizontalList() 
{
	// TODO: Add your control notification handler code here
	CString strSelection;
	m_HorizonLine_ListBox.GetText(m_HorizonLine_ListBox.GetCurSel(), strSelection);
	m_Horizon_Edit.SetWindowText(strSelection);
}
// 20081014 박동관 <<
