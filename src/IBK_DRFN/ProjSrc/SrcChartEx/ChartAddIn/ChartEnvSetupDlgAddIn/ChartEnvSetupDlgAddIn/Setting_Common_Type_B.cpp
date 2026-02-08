// Setting_Common_Type_B.cpp : implementation file
//

#include "stdafx.h"
#include "ChartEnvSetupDlgAddIn.h"
#include "Setting_Common_Type_B.h"

#include "GraphData.h"
#include "BlockBaseEnum.h"
#include "Conversion.h"
#include "TabMainChartEnvSetUp.h"
#include "DlgEnvSetUpAll.h"
#include "GraphSetting.h"

// 2008.01.25 by LYH >>
#include "../Include_Chart/Dll_Load/IMetaTable.h"	// for _MTOL0()
// 2008.01.25 by LYH <<

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetting_Common_Type_B dialog


CSetting_Common_Type_B::CSetting_Common_Type_B( IChartOCX *p_pIChartOCX, CWnd* pParent, CWnd *pTopWnd)
	: CBaseDialog( CSetting_Common_Type_B::IDD, pParent, p_pIChartOCX, pTopWnd)
{
	//{{AFX_DATA_INIT(CSetting_Common_Type_B)
	m_nCompositionLineType = -1;
	m_dHorizonValue = 0.0;
	//}}AFX_DATA_INIT

	m_nSubGraphIndex = -1;

	m_bUseComposition = FALSE;
	m_bUseBaseLine = FALSE;
	m_bInitComplite = FALSE;
	m_bUsePacket = FALSE;
}


CSetting_Common_Type_B::~CSetting_Common_Type_B()
{
}


void CSetting_Common_Type_B::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetting_Common_Type_B)
	DDX_Control(pDX, IDC_SPIN, m_Horizon_Spin);
	DDX_Control(pDX, IDC_SIGNAL, m_Signal_Bt);
	DDX_Control(pDX, IDC_HORIZONTAL_LIST, m_HorizonLine_ListBox);
	DDX_Control(pDX, IDC_HORIZONTAL, m_Horizon_Edit);
	DDX_Control(pDX, IDC_COLOR_BUTTON, m_Color_Bt);
	DDX_Control(pDX, IDC_STATIC_VALUE7, m_stcValue7);
	DDX_Control(pDX, IDC_STATIC_VALUE6, m_stcValue6);
	DDX_Control(pDX, IDC_STATIC_VALUE5, m_stcValue5);
	DDX_Control(pDX, IDC_STATIC_VALUE4, m_stcValue4);
	DDX_Control(pDX, IDC_STATIC_VALUE3, m_stcValue3);
	DDX_Control(pDX, IDC_STATIC_VALUE2, m_stcValue2);
	DDX_Control(pDX, IDC_STATIC_VALUE1, m_stcValue1);
	DDX_Control(pDX, IDC_SPIN_VALUE7, m_spnValue7);
	DDX_Control(pDX, IDC_SPIN_VALUE6, m_spnValue6);
	DDX_Control(pDX, IDC_SPIN_VALUE5, m_spnValue5);
	DDX_Control(pDX, IDC_SPIN_VALUE4, m_spnValue4);
	DDX_Control(pDX, IDC_SPIN_VALUE3, m_spnValue3);
	DDX_Control(pDX, IDC_SPIN_VALUE2, m_spnValue2);
	DDX_Control(pDX, IDC_SPIN_VALUE1, m_spnValue1);
	DDX_Control(pDX, IDC_EDIT_VALUE7, m_edtValue7);
	DDX_Control(pDX, IDC_EDIT_VALUE6, m_edtValue6);
	DDX_Control(pDX, IDC_EDIT_VALUE5, m_edtValue5);
	DDX_Control(pDX, IDC_EDIT_VALUE4, m_edtValue4);
	DDX_Control(pDX, IDC_EDIT_VALUE3, m_edtValue3);
	DDX_Control(pDX, IDC_EDIT_VALUE2, m_edtValue2);
	DDX_Control(pDX, IDC_EDIT_VALUE1, m_edtValue1);
	DDX_Control(pDX, IDC_EDIT_COMPOSITION_TITLEDATA, m_edtCompositionTitleData);
	DDX_Control(pDX, IDC_CMB_THICKNESS, m_cmbThickness);
	DDX_Control(pDX, IDC_CMB_LINETYPE, m_cmbLineType);
	DDX_Control(pDX, IDC_BTN_COMPOSITION_COLOR, m_btnComositionColor);
	DDX_Control(pDX, IDC_LIST_COMPOSITION, m_lstComposition);
	DDX_Text(pDX, IDC_HORIZONTAL, m_dHorizonValue);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetting_Common_Type_B, CDialog)
	//{{AFX_MSG_MAP(CSetting_Common_Type_B)
	ON_BN_CLICKED(IDC_BTN_COMPOSITION_COLOR, OnBtnCompositionColor)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_COMPOSITION, OnItemchangedListComposition)
	ON_NOTIFY(NM_CLICK, IDC_LIST_COMPOSITION, OnClickListComposition)
	ON_CBN_SELCHANGE(IDC_CMB_LINETYPE, OnSelchangeCmbLinetype)
	ON_CBN_SELCHANGE(IDC_CMB_THICKNESS, OnSelchangeCmbThickness)
	ON_BN_CLICKED(IDC_INSERT, OnInsert)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_BN_CLICKED(IDC_ALLDELETE, OnAlldelete)
	ON_BN_CLICKED(IDC_COLOR_BUTTON, OnColorButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetting_Common_Type_B message handlers

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 10
// Modifier		: 
// Comments		: 
//-----------------------------------------------------------------------------
BOOL CSetting_Common_Type_B::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// 1. 기본적인 Column 생성
	m_lstComposition.InsertColumn( 0, "", LVCFMT_LEFT, 100 );
	m_lstComposition.SetColumnWidth( 0,  LVSCW_AUTOSIZE_USEHEADER  );
	m_lstComposition.SetExtendedStyle( LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES );

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
void CSetting_Common_Type_B::Initial(IGraphInfo* p_pIGraphInfo)
{
	if( m_pIGraphInfo)
	{
		OnApply( m_pIGraphInfo);
		m_pIGraphInfo->Release();
	}
	m_pIGraphInfo = p_pIGraphInfo;
	if( m_pIGraphInfo) m_pIGraphInfo->AddRef();
	else return;


	
	// 1. 구성설정 Dialog 부분 Setting
	UseCompositionGroup( m_pIGraphInfo->GetSubGraphCount());

	// 2. 변수 설정 Group
	IDoubleList *pIDblCnd = m_pIGraphInfo->GetCommonCalculateConditionList();
	if( pIDblCnd)
	{
		int nCalCndCount = pIDblCnd->GetCount();
		pIDblCnd->Release();

		if( 0 < nCalCndCount)
		{
			ILPCSTR szIndicatorName = m_pIGraphInfo->GetIndicatorNameString();
			for( int i = 0 ; i < nCalCndCount ; i++ )
			{
				switch( i )
				{
					case 0:	m_stcValue1.SetWindowText( GetConditionTitle( szIndicatorName, i+1 ) );
							break;
					case 1:	m_stcValue2.SetWindowText( GetConditionTitle( szIndicatorName, i+1 ) );
							break;
					case 2:	m_stcValue3.SetWindowText( GetConditionTitle( szIndicatorName, i+1 ) );
							break;
					case 3:	m_stcValue4.SetWindowText( GetConditionTitle( szIndicatorName, i+1 ) );
							break;
					case 4:	m_stcValue5.SetWindowText( GetConditionTitle( szIndicatorName, i+1 ) );
							break;
					case 5:	m_stcValue6.SetWindowText( GetConditionTitle( szIndicatorName, i+1 ) );
							break;
					case 6:	m_stcValue7.SetWindowText( GetConditionTitle( szIndicatorName, i+1 ) );
							break;
				}
			}
		}
	}

	//sy 2006.04.17.
	// 4. 매매신호 show/hide
	BOOL bValidFalg = FALSE, bShowSignal = FALSE;
	m_pIGraphInfo->GetSignalShowFlag( bValidFalg, bShowSignal);
	//signal 체크 표시 보이기, 안보이기
	m_Signal_Bt.ShowWindow( bValidFalg ? SW_SHOW : SW_HIDE);





	m_nSubGraphIndex = 0;

	// 1. 변수 설정 Group
	pIDblCnd = m_pIGraphInfo->GetCommonCalculateConditionList();
	if( pIDblCnd)
	{
		double dData = 0;
		POSITION psnCnd = pIDblCnd->GetHeadPosition();

		int i = 0;
		CNumSpinCtrl *	pspnValue[ 7] = {	&m_spnValue1, &m_spnValue2, &m_spnValue3, &m_spnValue4, &m_spnValue5, &m_spnValue6, &m_spnValue7	};
		CNumEdit *		pedtValue[ 7] = {	&m_edtValue1, &m_edtValue2, &m_edtValue3, &m_edtValue4, &m_edtValue5, &m_edtValue6, &m_edtValue7	};
		while( psnCnd && i < 7)
		{
			pIDblCnd->GetNext( psnCnd, dData);
			Initial_Spin( *pspnValue[ i],  *pedtValue[ i], dData);
			i++;
		}
		pIDblCnd->Release();
	}

	// 2. 구성설정 Dialog 부분 Setting
	Init_Composition_Group_SubGraphItem();
	Init_Composition_Group_SubGraphTitle(m_nSubGraphIndex);
	Init_Composition_Group_SubGraphLine(m_nSubGraphIndex);
	Init_Composition_Group_SubGraphColor(m_nSubGraphIndex);


	// 3. 기준선 설정 Dialog 부분 Setting
	UseBaseLineGroup( IsShowBaseLineList());
	Init_BaseLine_Group_Title();

	UpdateData(FALSE);
}

//sy 2006.04.17.
void CSetting_Common_Type_B::OnApplySignal(IGraphInfo* p_pIGraphInfo)
{
	if(p_pIGraphInfo == NULL || !m_Signal_Bt.IsWindowVisible()) return;

	// 속성이 보일때만 signal 저장한다.
	BOOL bValidFalg = FALSE, bShowSignal = FALSE;
	m_pIGraphInfo->GetSignalShowFlag( bValidFalg, bShowSignal);
	if( !bValidFalg) return;

	// signal 보임
	m_pIGraphInfo->SetSignalShowFlag( bValidFalg, m_Signal_Bt.GetCheck());
}

void CSetting_Common_Type_B::Initial_Signal(IGraphInfo* p_pIGraphInfo)
{
	if(p_pIGraphInfo == NULL) return;
	BOOL bValidFalg = FALSE, bShowSignal = FALSE;
	m_pIGraphInfo->GetSignalShowFlag( bValidFalg, bShowSignal);
	if( !bValidFalg) return;
	
	//signal 사용
	m_Signal_Bt.SetCheck( bShowSignal);
}
//sy end


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 10
// Return void : 
// Param  IGraphInfo* p_pIGraphInfo : 
// Comments		: 
//-----------------------------------------------------------------------------	
// 20081014 박동관 >>
BOOL CSetting_Common_Type_B::OnApply(IGraphInfo* p_pIGraphInfo)
//void CSetting_Common_Type_B::OnApply(IGraphInfo* p_pIGraphInfo)
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
void CSetting_Common_Type_B::OnApplyBaseLine()
{
	if(m_pIGraphInfo == NULL) return;
	
	IDoubleList *pIDblList = m_pIGraphInfo->GetBaseLineList();
	if( pIDblList)
	{
		pIDblList->RemoveAll();
		CString data;
		for(int nIndex = 0 ; nIndex < m_HorizonLine_ListBox.GetCount(); nIndex++) 
		{
			m_HorizonLine_ListBox.GetText(nIndex, data);
			pIDblList->AddTail( atof( data));
		}
		pIDblList->Release();
	}

	//sy 2006.04.17.
	OnApplySignal(m_pIGraphInfo);
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 10
// Return void : 
// Param  BOOL bUseComposition : 
// Comments		: 
//-----------------------------------------------------------------------------
void CSetting_Common_Type_B::UseCompositionGroup( BOOL bUseComposition )
{
	m_bUseComposition = bUseComposition;
	
	if( bUseComposition == TRUE )
	{
		GetDlgItem( IDC_COMPOSITION )->ShowWindow( SW_SHOW );			// GroupBox
		GetDlgItem( IDC_LIST_COMPOSITION )->ShowWindow( SW_SHOW );		// List
		GetDlgItem( IDC_COMPOSITION_EACH )->ShowWindow( SW_SHOW );		// sub GroupBox
		GetDlgItem( IDC_COMPOSITION_TITLE )->ShowWindow( SW_SHOW );
		GetDlgItem( IDC_EDIT_COMPOSITION_TITLEDATA )->ShowWindow( SW_SHOW );
		GetDlgItem( IDC_COMPOSITION_LINETITLE )->ShowWindow( SW_SHOW );
		GetDlgItem( IDC_BTN_COMPOSITION_COLOR )->ShowWindow( SW_SHOW );
		GetDlgItem( IDC_CMB_THICKNESS )->ShowWindow( SW_SHOW );
		GetDlgItem( IDC_CMB_LINETYPE )->ShowWindow( SW_SHOW );
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
void CSetting_Common_Type_B::UseBaseLineGroup( BOOL bUseBaseLine )
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
	}
}


bool CSetting_Common_Type_B::IsShowBaseLineList() const
{
	IDoubleList *pIDblBase = m_pIGraphInfo->GetBaseLineList();
	int nCount = 0;
	if( pIDblBase)
	{
		nCount = pIDblBase->GetCount();
		pIDblBase->Release();
		if( nCount) return TRUE;
	}

	ILPCSTR szGraphName = m_pIGraphInfo->GetGraphNameString();
	IGraphInfo* pDefaultInfo = m_pIGraphInfoManager->GetGraphInfo( IGraphInfoManager::IEILT_SITE_DEFAULT, szGraphName);
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
// Param  CList< int* : 
// Param  int* > *lstSubGraphData : 
// Comments		: 
//-----------------------------------------------------------------------------
//sy 2005.11.02.
void CSetting_Common_Type_B::Init_Composition_Group_SubGraphItem( void)
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
		m_lstComposition.InsertItem( nItem, strUIText);
// 2008.01.25 by LYH <<

		m_pIGraphInfo->GetSubGraphShowFlag( nItem, bShowFlag);
		m_lstComposition.SetCheck( nItem, bShowFlag);
	}
}


void CSetting_Common_Type_B::Init_Composition_Group_SubGraphTitle( int nSubGraphIndex)
{
	if( nSubGraphIndex < 0) return;

	// Title
	ILPCSTR szSubGraphTitle = m_pIGraphInfo->GetSubGraphTitleString( nSubGraphIndex);
// 2008.01.25 by LYH >>
	//m_edtCompositionTitleData.SetWindowText( szSubGraphTitle);

	CString strUIText = _MTOL1_N( szSubGraphTitle); 
	if( strUIText.IsEmpty()) strUIText = _MTOL3( szSubGraphTitle);

	m_edtCompositionTitleData.SetWindowText( strUIText);
// 2008.01.25 by LYH <<
}

void CSetting_Common_Type_B::Init_Composition_Group_SubGraphLine( int nSubGraphIndex )
{
	if( nSubGraphIndex < 0) return;

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


void CSetting_Common_Type_B::Init_Composition_Group_SubGraphColor( int nSubGraphIndex )
{
	if( nSubGraphIndex < 0) return;

	SetButtonColor(m_btnComositionColor, m_pIGraphInfo->GetSubGraphColor( nSubGraphIndex, 1));
}



void CSetting_Common_Type_B::SetButtonColor(CColorButton& colorButton, const COLORREF& color)
{
	colorButton.SetColor(color);
	colorButton.Invalidate();
}
//sy end


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 10
// Return void : 
// Param  CList< double : 
// Param  double >* pBaseLineList : 
// Comments		: 
//-----------------------------------------------------------------------------
void CSetting_Common_Type_B::Init_BaseLine_Group_Title( void)
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

	ILPCSTR szIndicatorName = m_pIGraphInfo->GetIndicatorNameString();
	CBaseData::LIMITHORIZONTALLINE type = CFindType::FindLimitValueofHorizontalLine( szIndicatorName);
	if(type == CBaseData::ZeroToOneZeroZero)			Initial_Spin(m_Horizon_Spin, m_Horizon_Edit, dFirst, 0, 100, 1);
	else if(type == CBaseData::MinusZeroToOneZeroZero)	Initial_Spin(m_Horizon_Spin, m_Horizon_Edit, dFirst, -100, -1, 1);
	else												Initial_Spin(m_Horizon_Spin, m_Horizon_Edit, dFirst, -999999, 999999, 1);
	
	// 맨 마지막 선의 색깔이 기준선 색이된다.
	m_Color_Bt.SetColor( m_pIGraphInfo->GetSubGraphColor( m_pIGraphInfo->GetSubGraphCount() - 1, 6));

	//sy 2006.04.17.
	Initial_Signal(m_pIGraphInfo);

	UpdateData(FALSE);
}

void CSetting_Common_Type_B::DeleteString_ListBox()
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
void CSetting_Common_Type_B::OnBtnCompositionColor() 
{
	// TODO: Add your control notification handler code here
	if( m_nSubGraphIndex < 0) return;
	
	COLORREF crColor = m_btnComositionColor.GetColor();
	CColorDialog color(crColor, CC_FULLOPEN);
	if(color.DoModal() == IDOK) 
	{
		m_pIGraphInfo->SetSubGraphColor( m_nSubGraphIndex, 1, color.GetColor());
		m_btnComositionColor.SetColor( m_pIGraphInfo->GetSubGraphColor( m_nSubGraphIndex, 1));
		
		m_btnComositionColor.Invalidate();
	}
}


void CSetting_Common_Type_B::OnClickListComposition(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
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
void CSetting_Common_Type_B::OnApplySubGraphData( void )
{
	// 선색상은 OnCompositioncolor에서 바로 적용
	m_pIGraphInfo->SetSubGraphShowFlag( m_nSubGraphIndex, m_lstComposition.GetCheck( m_nSubGraphIndex));

	OnApply_Title( m_nSubGraphIndex, m_edtCompositionTitleData);
	OnApply_DrawStyle( m_nSubGraphIndex, m_cmbLineType);
	OnApply_Thickness( m_nSubGraphIndex, m_cmbThickness);

	IDoubleList *pIDblCnd = m_pIGraphInfo->GetCommonCalculateConditionList();
	if( pIDblCnd)
	{
		CString strData;
		double dData = 0, dNewData = 0;
		int i = 0;
		CNumEdit *pNEdit[ 7] = { &m_edtValue1, &m_edtValue2, &m_edtValue3, &m_edtValue4, &m_edtValue5, &m_edtValue6, &m_edtValue7	};
		POSITION psnCnd = pIDblCnd->GetHeadPosition(), psnBack = NULL;
		while( psnCnd && i < 7)
		{
			psnBack = psnCnd;
			pIDblCnd->GetNext( psnCnd, dData);

			if( i < 7) pNEdit[ i]->GetWindowText( strData);
			else strData.Empty();
			i++;

			dNewData = atof( strData);
			if(!CBaseDialog::m_bDoCalculate) CBaseDialog::m_bDoCalculate = ( dData != dNewData);
			pIDblCnd->SetAt( psnBack, dNewData);
		}

		pIDblCnd->Release();
	}

	// 기준선 관련 저장
	OnApplyBaseLine();
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 11
// Return void : 
// Param  const int nIndex : 
// Comments		: 
//-----------------------------------------------------------------------------
void CSetting_Common_Type_B::Initial_SubGraphData( const int nIndex )
{
	if( m_pIGraphInfo == NULL ) return;

	int nOldSubGraphIndex = m_nSubGraphIndex;
	if( nIndex > m_pIGraphInfo->GetSubGraphCount() - 1 ) m_nSubGraphIndex = m_pIGraphInfo->GetSubGraphCount() - 1;
	else m_nSubGraphIndex = ( nIndex < 0 ? 0 : nIndex);

	if( 0 <= nOldSubGraphIndex && nOldSubGraphIndex == m_nSubGraphIndex) return;
	
	Initial_Title( m_nSubGraphIndex, m_edtCompositionTitleData);
	Initial_DrawStyleComboBox(m_nSubGraphIndex, m_cmbLineType);
	
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
void CSetting_Common_Type_B::OnItemchangedListComposition(NMHDR* pNMHDR, LRESULT* pResult) 
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
void CSetting_Common_Type_B::OnSelchangeCmbLinetype() 
{
	// TODO: Add your control notification handler code here
	if( m_cmbThickness.GetCurSel() != 0 && m_cmbLineType.GetCurSel() != 0 )
	{
// 2008.01.25 by LYH >>
//		AfxMessageBox("선모양이 \"실선\"인 경우만 굵기를 변경할 수 있습니다.");
		AfxMessageBox(m_strMsgLineThick);		
// 2008.01.25 by LYH <<
		m_cmbThickness.SetCurSel(0);
		m_cmbThickness.EnableWindow(FALSE);
		return;
	}

	else
		m_cmbThickness.EnableWindow(TRUE);
}

void CSetting_Common_Type_B::OnSelchangeCmbThickness() 
{
	// TODO: Add your control notification handler code here
	if( m_cmbThickness.GetCurSel() != 0 && m_cmbLineType.GetCurSel() != 0 )
	{
// 2008.01.25 by LYH >>
		//AfxMessageBox("선굵기가 \"1pt\"인 경우만 선모양을 변경할 수 있습니다.");
		AfxMessageBox(m_strMsgLineShape);
// 2008.01.25 by LYH <<
		m_cmbLineType.SetCurSel(0);
		m_cmbLineType.EnableWindow(FALSE);
		return;
	}	

	else
		m_cmbLineType.EnableWindow(TRUE);	
}


void CSetting_Common_Type_B::OnInsert() 
{
	// TODO: Add your control notification handler code here
	CString temp;
	UpdateData(TRUE);
	temp.Format("%d",(int)m_dHorizonValue);
	
	int nCount = m_HorizonLine_ListBox.GetCount();
	
	CString szText = "";
	for( int i = 0 ; i < nCount ; i++ )
	{
		m_HorizonLine_ListBox.GetText( i, szText );
		if( szText == temp )
			return;
	}

	m_HorizonLine_ListBox.AddString(TrimStringWhenDecimalDataIsZero(temp));
}


void CSetting_Common_Type_B::OnDelete() 
{
	// TODO: Add your control notification handler code here
	m_HorizonLine_ListBox.DeleteString(m_HorizonLine_ListBox.GetCurSel());	
}


void CSetting_Common_Type_B::OnAlldelete() 
{
	// TODO: Add your control notification handler code here
	DeleteString_ListBox();
	IDoubleList *pIDblBase = m_pIGraphInfo->GetBaseLineList();
	if( pIDblBase)
	{
		pIDblBase->RemoveAll();
		pIDblBase->Release();
	}

	m_Horizon_Edit.SetValue( (float)0);	
}


void CSetting_Common_Type_B::OnColorButton() 
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


BOOL CSetting_Common_Type_B::PreTranslateMessage(MSG* pMsg) 
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
