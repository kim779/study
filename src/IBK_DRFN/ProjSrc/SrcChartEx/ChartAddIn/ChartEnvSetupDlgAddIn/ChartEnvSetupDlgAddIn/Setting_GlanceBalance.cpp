// Setting_GlanceBalance.cpp : implementation file
//

#include "stdafx.h"
#include "ChartEnvSetupDlgAddIn.h"
#include "Setting_GlanceBalance.h"

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
// CSetting_GlanceBalance dialog


CSetting_GlanceBalance::CSetting_GlanceBalance( IChartOCX *p_pIChartOCX, CWnd* pParent, CWnd *pTopWnd)
	: CBaseDialog(CSetting_GlanceBalance::IDD, pParent, p_pIChartOCX, pTopWnd)
{
	//{{AFX_DATA_INIT(CSetting_GlanceBalance)
	m_nCompositionLineType = -1;
	m_nFillType = 0;
	//}}AFX_DATA_INIT
	
	m_nSubGraphIndex = -1;
}

CSetting_GlanceBalance::~CSetting_GlanceBalance()
{

}



void CSetting_GlanceBalance::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetting_GlanceBalance)
	DDX_Control(pDX, IDC_STATIC_VALUE5, m_stcValue5);
	DDX_Control(pDX, IDC_STATIC_VALUE4, m_stcValue4);
	DDX_Control(pDX, IDC_STATIC_VALUE3, m_stcValue3);
	DDX_Control(pDX, IDC_STATIC_VALUE2, m_stcValue2);
	DDX_Control(pDX, IDC_STATIC_VALUE1, m_stcValue1);
	DDX_Control(pDX, IDC_EDIT_VALUE5, m_edtValue5);
	DDX_Control(pDX, IDC_EDIT_VALUE4, m_edtValue4);
	DDX_Control(pDX, IDC_EDIT_VALUE3, m_edtValue3);
	DDX_Control(pDX, IDC_EDIT_VALUE2, m_edtValue2);
	DDX_Control(pDX, IDC_EDIT_VALUE1, m_edtValue1);
	DDX_Control(pDX, IDC_EDIT_COMPOSITION_TITLEDATA, m_edtCompositionTitleData);
	DDX_Control(pDX, IDC_CMB_THICKNESS, m_cmbThickness);
	DDX_Control(pDX, IDC_CMB_LINETYPE, m_cmbLineType);
	DDX_Control(pDX, IDC_CMB_DATA, m_cmbData);
	DDX_Control(pDX, IDC_BTN_UP1, m_btnUp1Color);
	DDX_Control(pDX, IDC_BTN_UP_COLOR, m_btnUpColor);
	DDX_Control(pDX, IDC_BTN_STEADY_COLOR, m_btnSteadyColor);
	DDX_Control(pDX, IDC_BTN_DOWN1, m_btnDown1Color);
	DDX_Control(pDX, IDC_BTN_DOWN_COLOR, m_btnDownColor);
	DDX_Control(pDX, IDC_BTN_COMPOSITION_COLOR, m_btnCompositionColor);
	DDX_Control(pDX, IDC_STATIC_DATA_TITLE, m_stcDataTitle);
	DDX_Control(pDX, IDC_SPIN_VALUE5, m_spnValue5);
	DDX_Control(pDX, IDC_SPIN_VALUE4, m_spnValue4);
	DDX_Control(pDX, IDC_SPIN_VALUE3, m_spnValue3);
	DDX_Control(pDX, IDC_SPIN_VALUE2, m_spnValue2);
	DDX_Control(pDX, IDC_SPIN_VALUE1, m_spnValue1);
	DDX_Control(pDX, IDC_LIST_COMPOSITION, m_lstComposition);
	DDX_Radio(pDX, IDC_FILL_LINE, m_nFillType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetting_GlanceBalance, CDialog)
	//{{AFX_MSG_MAP(CSetting_GlanceBalance)
	ON_BN_CLICKED(IDC_BTN_UP_COLOR, OnBtnUpColor)
	ON_BN_CLICKED(IDC_BTN_DOWN_COLOR, OnBtnDownColor)
	ON_BN_CLICKED(IDC_BTN_STEADY_COLOR, OnBtnSteadyColor)
	ON_BN_CLICKED(IDC_BTN_UP1, OnBtnUp1)
	ON_BN_CLICKED(IDC_BTN_DOWN1, OnBtnDown1)
	ON_BN_CLICKED(IDC_BTN_COMPOSITION_COLOR, OnBtnCompositionColor)
	ON_NOTIFY(NM_CLICK, IDC_LIST_COMPOSITION, OnClickListComposition)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_COMPOSITION, OnItemchangedListComposition)
	ON_CBN_SELCHANGE(IDC_CMB_LINETYPE, OnSelchangeCmbLinetype)
	ON_CBN_SELCHANGE(IDC_CMB_THICKNESS, OnSelchangeCmbThickness)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetting_GlanceBalance message handlers


BOOL CSetting_GlanceBalance::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	// 1. 기본적인 Column 생성
	m_lstComposition.InsertColumn( 0, "", LVCFMT_LEFT, 100 );
	m_lstComposition.SetColumnWidth( 0,  LVSCW_AUTOSIZE_USEHEADER  );
	m_lstComposition.SetExtendedStyle( LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES );

	if( !m_pIGraphInfo) return TRUE;
	Initial( NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CSetting_GlanceBalance::Initial( IGraphInfo *p_pIGraphInfo )
{
	if( m_pIGraphInfo)
	{
		OnApply( m_pIGraphInfo);
		m_pIGraphInfo->Release();
	}
	m_pIGraphInfo = p_pIGraphInfo;
	if( m_pIGraphInfo) m_pIGraphInfo->AddRef();
	else return;
	
	// 2. 변수 설정 Group
	ILPCSTR szGraphName = m_pIGraphInfo->GetGraphNameString();
	m_stcValue1.SetWindowText( GetConditionTitle( szGraphName, 1));
	m_stcValue2.SetWindowText( GetConditionTitle( szGraphName, 2));
	m_stcValue3.SetWindowText( GetConditionTitle( szGraphName, 3));
	m_stcValue4.SetWindowText( GetConditionTitle( szGraphName, 4));
	m_stcValue5.SetWindowText( GetConditionTitle( szGraphName, 5));

	m_nSubGraphIndex = 0;

	// 1. 변수 설정 Group
	IDoubleList *pICalCnd = m_pIGraphInfo->GetCommonCalculateConditionList();
	if( !pICalCnd) return;
	
	POSITION psnCnd = pICalCnd->GetHeadPosition();
	double dData = 0;
	if( psnCnd)
	{
		pICalCnd->GetNext( psnCnd, dData);
		Initial_Spin( m_spnValue1, m_edtValue1, dData);
	}
	if( psnCnd)
	{
		pICalCnd->GetNext( psnCnd, dData);
		Initial_Spin( m_spnValue2, m_edtValue2, dData);
	}
	if( psnCnd)
	{
		pICalCnd->GetNext( psnCnd, dData);
		Initial_Spin( m_spnValue3, m_edtValue3, dData);
	}
	if (psnCnd == NULL)
	{
		CString str1, str2, str3;
		m_edtValue1.GetWindowText(str1);
		m_edtValue3.GetWindowText(str3);

		Initial_Spin( m_spnValue3, m_edtValue3, atof(str1));
		Initial_Spin( m_spnValue4, m_edtValue4, atof(str3));
		Initial_Spin( m_spnValue5, m_edtValue5, atof(str1));
	}
	else
	{
		if( psnCnd)
		{
			pICalCnd->GetNext( psnCnd, dData);
			Initial_Spin( m_spnValue4, m_edtValue4, dData);
		}
		if( psnCnd)
		{
			pICalCnd->GetNext( psnCnd, dData);
			Initial_Spin( m_spnValue5, m_edtValue5, dData);
		}
	}
	pICalCnd->Release();

	// 2. 구성설정 Dialog 부분 Setting
	Init_Composition_Group( m_pIGraphInfo);

	UpdateData(FALSE);
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 11 / 10
// Return void  : 
// Param  IGraphInfo *p_pIGraphInfo : 
// Comments		: 
//-----------------------------------------------------------------------------
// 20081014 박동관 >>
BOOL CSetting_GlanceBalance::OnApply( IGraphInfo *p_pIGraphInfo )
//void CSetting_GlanceBalance::OnApply( IGraphInfo *p_pIGraphInfo )
// 20081014 박동관 <<
{
	if(p_pIGraphInfo == NULL)
		return FALSE;
	
	UpdateData();
	
	OnApplySubGraphData();

	IDoubleList *pICalCnd = m_pIGraphInfo->GetCommonCalculateConditionList();
	if( pICalCnd)
	{
		double dData = 0;
		CString strCnd;
		POSITION psnCnd = pICalCnd->GetHeadPosition();
		m_edtValue1.GetWindowText( strCnd);
		if( psnCnd)
		{
			pICalCnd->SetAt( psnCnd, atof( strCnd));
			pICalCnd->GetNext( psnCnd, dData);
		}
		else
			pICalCnd->AddHead(atof(strCnd));

		m_edtValue2.GetWindowText( strCnd);
		if( psnCnd)
		{
			pICalCnd->SetAt( psnCnd, atof( strCnd));
			pICalCnd->GetNext( psnCnd, dData);
		}
		else
			pICalCnd->AddTail(atof(strCnd));
		
		m_edtValue3.GetWindowText( strCnd);
		if( psnCnd)
		{
			pICalCnd->SetAt( psnCnd, atof( strCnd));
			pICalCnd->GetNext( psnCnd, dData);
		}
		else
			pICalCnd->AddTail(atof(strCnd));

		m_edtValue4.GetWindowText( strCnd);
		if( psnCnd)
		{
			pICalCnd->SetAt( psnCnd, atof( strCnd));
			pICalCnd->GetNext( psnCnd, dData);
		}
		else
			pICalCnd->AddTail(atof(strCnd));

		m_edtValue5.GetWindowText( strCnd);
		if( psnCnd)
		{
			pICalCnd->SetAt( psnCnd, atof( strCnd));
			pICalCnd->GetNext( psnCnd, dData);
		}
		else
			pICalCnd->AddTail(atof(strCnd));

		if( !CBaseDialog::m_bDoCalculate)
		{
			IDoubleList *pICalCndOrg = p_pIGraphInfo->GetCommonCalculateConditionList();
			if( pICalCndOrg)
			{
				CBaseDialog::m_bDoCalculate = !DoesEqualDoubleList( pICalCndOrg, pICalCnd);
				pICalCndOrg->Release();
			}
		}

		pICalCnd->Release();
	}

	p_pIGraphInfo->GetCopyDataFrom( m_pIGraphInfo);

	// 일목균형표 설정 커서 적용 - ojtaso (20071109)
	OnApply_SpanCursor();

	return TRUE;
}

//sy 2005.11.02.
void CSetting_GlanceBalance::Init_Composition_Group( IGraphInfo *p_pIGraphInfo)
{
	Init_Composition_Group_SubGraphItem( p_pIGraphInfo);
	Init_Composition_Group_SubGraphTitle(m_nSubGraphIndex);
	Init_Composition_Group_SubGraphLine(m_nSubGraphIndex);
	Init_Composition_Group_SubGraphColor(m_nSubGraphIndex);
}

void CSetting_GlanceBalance::Init_Composition_Group_SubGraphItem( IGraphInfo *p_pIGraphInfo)
{
	m_lstComposition.DeleteAllItems();

	// SubGraphName list check
	BOOL bShow = FALSE;
	ILPCSTR szSubGraphName = NULL;
// 2008.01.25 by LYH >>
	CString strUIText;
// 2008.01.25 by LYH <<
	
	for( int nItem = 0; nItem < p_pIGraphInfo->GetSubGraphCount(); nItem++)
	{
		szSubGraphName = p_pIGraphInfo->GetSubGraphNameString( nItem);
// 2008.01.25 by LYH >>
		//m_lstComposition.InsertItem( nItem, szSubGraphName);
		CString strUIText = _MTOL3_N(szSubGraphName);
		m_lstComposition.InsertItem( nItem, strUIText);
// 2008.01.25 by LYH <<

		p_pIGraphInfo->GetSubGraphShowFlag( nItem, bShow);
		m_lstComposition.SetCheck( nItem, bShow);
	}
}

void CSetting_GlanceBalance::Init_Composition_Group_SubGraphTitle( int p_nSubGraphIndex)
{
	if( p_nSubGraphIndex < 0) return;

	// Title
	ILPCSTR szSubGraphTitle = m_pIGraphInfo->GetSubGraphTitleString( p_nSubGraphIndex);
// 2008.01.25 by LYH >>
	//m_edtCompositionTitleData.SetWindowText( szSubGraphTitle);
	CString strUIText = _MTOL1_N( szSubGraphTitle);
	if( strUIText.IsEmpty()) strUIText = _MTOL3( szSubGraphTitle);
	m_edtCompositionTitleData.SetWindowText( strUIText);
// 2008.01.25 by LYH <<
}


void CSetting_GlanceBalance::Init_Composition_Group_SubGraphLine( int p_nSubGraphIndex)
{
	if(p_nSubGraphIndex < 0) return;

	// 선굵기
	if( m_cmbThickness.GetCount() == 0 )
		Initial_ThicknessComboBox( p_nSubGraphIndex, 0, m_cmbThickness );
	else
		m_cmbThickness.SetCurSel( m_pIGraphInfo->GetSubGraphPenThickness( p_nSubGraphIndex, 1) - 1);

	// 선모양
	if( m_cmbLineType.GetCount() == 0 )
		Initial_DrawStyleComboBox( p_nSubGraphIndex, m_cmbLineType );
	else
		m_cmbLineType.SetCurSel( m_pIGraphInfo->GetSubGraphDrawStyle( p_nSubGraphIndex));
}


void CSetting_GlanceBalance::Init_Composition_Group_SubGraphColor( int p_nSubGraphIndex)
{
	if( p_nSubGraphIndex < 0) return;

	// "선형"
	if(Init_Composition_Group_SubGraphColor_LineType( p_nSubGraphIndex ))
		return;
	
	// "바형"
	if(Init_Composition_Group_SubGraphColor_BoxType( p_nSubGraphIndex ))
		return;

	UseSubColorBtn( FALSE );
	SetBottonColor(m_btnCompositionColor, m_pIGraphInfo->GetSubGraphColor( p_nSubGraphIndex, 1));
}


bool CSetting_GlanceBalance::Init_Composition_Group_SubGraphColor_LineType( const int p_nSubGraphIndex )
{
	if( m_pIGraphInfo->GetSubGraphType( p_nSubGraphIndex) != CGraphBaseData::Line_Type) return false;

	CGraphBaseData::LINESTYLE eLineStyle = ( CGraphBaseData::LINESTYLE)m_pIGraphInfo->GetSubGraphStyle( p_nSubGraphIndex);

	// 지표 스타일이 "채움식"일 때에는 지표선의 선 색상이 다르게 적용된다.
	if( eLineStyle ==  CGraphBaseData::FillLine)
	{
// 2008.01.25 by LYH >>
		//UseSubColorBtn( TRUE, "선색" );
		CString strColor;
		strColor.LoadString(IDS_LINE_COLOR);
		UseSubColorBtn( TRUE, strColor );
// 2008.01.25 by LYH <<

		SetBottonColor(m_btnUpColor, m_pIGraphInfo->GetSubGraphColor( p_nSubGraphIndex, 1));
		SetBottonColor(m_btnDownColor, m_pIGraphInfo->GetSubGraphColor( p_nSubGraphIndex, 3));
		SetBottonColor(m_btnSteadyColor, m_pIGraphInfo->GetSubGraphColor( p_nSubGraphIndex, 5));
		UseFillType( FALSE );
	}

	// 지표 스타일이 "선형구름식", "구름식" 일 때에는 지표선의 선 색상이 다르게 적용된다.
	else if( eLineStyle == CGraphBaseData::LineCloudLine || eLineStyle == CGraphBaseData::CloudLine || eLineStyle == CGraphBaseData::NonCloudLine)
	{
		UseSubColorBtn( TRUE, "", FALSE );
		SetBottonColor(m_btnUp1Color, m_pIGraphInfo->GetSubGraphColor( p_nSubGraphIndex, 1));
		SetBottonColor(m_btnDown1Color, m_pIGraphInfo->GetSubGraphColor( p_nSubGraphIndex, 3));
		SetGraphStyle_PreSpen( p_nSubGraphIndex);
		UseFillType( TRUE );
	}

	else
	{
		UseSubColorBtn( FALSE );
		UseFillType( FALSE );
		SetBottonColor(m_btnCompositionColor, m_pIGraphInfo->GetSubGraphColor( p_nSubGraphIndex, 1));
	}

	return true;
}

void CSetting_GlanceBalance::UseFillType( BOOL bUse )
{
	if( bUse )
	{
		GetDlgItem( IDC_FILL_LINE )->ShowWindow( SW_SHOW );
		GetDlgItem( IDC_FILL_COLOR )->ShowWindow( SW_SHOW );
		GetDlgItem( IDC_FILL_NONE )->ShowWindow( SW_SHOW );
	}

	else
	{
		GetDlgItem( IDC_FILL_LINE )->ShowWindow( SW_HIDE );
		GetDlgItem( IDC_FILL_COLOR )->ShowWindow( SW_HIDE );
		GetDlgItem( IDC_FILL_NONE )->ShowWindow( SW_HIDE );
	}
}


//sy 2005.11.11
void CSetting_GlanceBalance::SetGraphStyle_PreSpen( const int p_nSubGraphIndex)
{
	if( p_nSubGraphIndex < 0 || m_pIGraphInfo->GetSubGraphType( p_nSubGraphIndex) != CGraphBaseData::Line_Type) return;

	switch( m_pIGraphInfo->GetSubGraphStyle( p_nSubGraphIndex))
	{
		case CGraphBaseData::NonCloudLine:	//비움
			m_nFillType = 2;
			break;
		case CGraphBaseData::CloudLine:		//구름식
			m_nFillType = 1;
			break;
		case CGraphBaseData::LineCloudLine: //선형 구름식
		default:
			m_nFillType = 0;
	}

	UpdateData(FALSE);
}

void CSetting_GlanceBalance::OnApply_GraphStyle(int p_nSubGraphIndex)
{
	if(p_nSubGraphIndex < 0 || m_pIGraphInfo->GetSubGraphType( p_nSubGraphIndex) != CGraphBaseData::Line_Type) return;

	switch( m_pIGraphInfo->GetSubGraphStyle( p_nSubGraphIndex))
	{
		case CGraphBaseData::NonCloudLine:	//비움
		case CGraphBaseData::CloudLine:		//구름식
		case CGraphBaseData::LineCloudLine: //선형 구름식
			SetGraphStyle(p_nSubGraphIndex);
			break;
		default:
			break;
	}
}

void CSetting_GlanceBalance::SetGraphStyle(int p_nSubGraphIndex)
{
	UpdateData(TRUE);
	switch(m_nFillType)
	{
		case 2:	 //비움
			m_pIGraphInfo->SetSubGraphStyle( p_nSubGraphIndex, CGraphBaseData::NonCloudLine);
			break;
		case 1:	 //구름식
			m_pIGraphInfo->SetSubGraphStyle( p_nSubGraphIndex, CGraphBaseData::CloudLine);
			break;
		default: //선형 구름식
			m_pIGraphInfo->SetSubGraphStyle( p_nSubGraphIndex, CGraphBaseData::LineCloudLine);
	}
}
//sy end


bool CSetting_GlanceBalance::Init_Composition_Group_SubGraphColor_BoxType( const int p_nSubGraphIndex)
{
	if(m_pIGraphInfo->GetSubGraphType( p_nSubGraphIndex) != CGraphBaseData::Bar_Type)
		return false;

	CGraphBaseData::BARSTYLE eBarStyle = ( CGraphBaseData::BARSTYLE)m_pIGraphInfo->GetSubGraphStyle( p_nSubGraphIndex);
	
	// 지표 스타일이 "박스형"일 때에는 지표선의 선 색상이 다르게 적용된다.
	if( eBarStyle ==  CGraphBaseData::BoxBar )
	{
// 2008.01.25 by LYH >>
		//UseSubColorBtn( TRUE, "보합" );
		CString strSteady;
		strSteady.LoadString(IDS_STRING_STEADY);
		UseSubColorBtn( TRUE, strSteady );
// 2008.01.25 by LYH <<

		SetBottonColor(m_btnUpColor, m_pIGraphInfo->GetSubGraphColor( p_nSubGraphIndex, 1));
		SetBottonColor(m_btnDownColor, m_pIGraphInfo->GetSubGraphColor( p_nSubGraphIndex, 3));
		SetBottonColor(m_btnSteadyColor, m_pIGraphInfo->GetSubGraphColor( p_nSubGraphIndex, 5));
	}
	// 지표 스타일이 "Dot형"일 때에는 지표선의 선 색상이 다르게 적용된다.
	else if( eBarStyle ==  CGraphBaseData::DotBar )
	{
		UseSubColorBtn( TRUE, "", FALSE );
		SetBottonColor(m_btnUp1Color, m_pIGraphInfo->GetSubGraphColor( p_nSubGraphIndex, 1));
		SetBottonColor(m_btnDown1Color, m_pIGraphInfo->GetSubGraphColor( p_nSubGraphIndex, 3));
	}
	else
	{
		UseSubColorBtn( FALSE );
		SetBottonColor(m_btnCompositionColor, m_pIGraphInfo->GetSubGraphColor( p_nSubGraphIndex, 1));
	}

	return true;
}


void CSetting_GlanceBalance::SetBottonColor(CColorButton& colorButton, const COLORREF& color)
{
	colorButton.SetColor(color);
	colorButton.Invalidate();
}


void CSetting_GlanceBalance::UseSubColorBtn( BOOL bUseSubColorBtn, CString szBtnText, BOOL bUseThirdBtn )
{
// 2008.01.25 by LYH >>
	//m_btnUpColor.SetText( "상승" );
	//m_btnDownColor.SetText( "하락" );

	//m_btnUp1Color.SetText( "상승" );
	//m_btnDown1Color.SetText( "하락" );

	m_btnUpColor.SetText( m_strUp );
	m_btnDownColor.SetText( m_strDown );

	m_btnUp1Color.SetText( m_strUp );
	m_btnDown1Color.SetText( m_strDown );
// 2008.01.25 by LYH <<

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

void CSetting_GlanceBalance::OnBtnCompositionColor() 
{
	// TODO: Add your control notification handler code here
	if(m_nSubGraphIndex < 0) return;
	
	COLORREF crColor = m_btnCompositionColor.GetColor();
	CColorDialog color(crColor, CC_FULLOPEN);
	if(color.DoModal() == IDOK) 
	{
		// 지표 스타일이 "채움식" 일 때에는 지표선의 선 색상이 다르게 적용된다.
		if( m_pIGraphInfo->GetSubGraphStyle( m_nSubGraphIndex) ==  CGraphBaseData::FillLine )
		{
			m_pIGraphInfo->SetSubGraphColor( m_nSubGraphIndex, 5, color.GetColor());
			m_btnCompositionColor.SetColor( m_pIGraphInfo->GetSubGraphColor( m_nSubGraphIndex, 5));
		}
		
		else
		{
			m_pIGraphInfo->SetSubGraphColor( m_nSubGraphIndex, 1, color.GetColor());
			m_btnCompositionColor.SetColor( m_pIGraphInfo->GetSubGraphColor( m_nSubGraphIndex, 1));
		}
		
		m_btnCompositionColor.Invalidate();
	}
}


void CSetting_GlanceBalance::OnBtnUpColor() 
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

void CSetting_GlanceBalance::OnBtnDownColor() 
{
	// TODO: Add your control notification handler code here
	if( m_nSubGraphIndex < 0) return;
	
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

void CSetting_GlanceBalance::OnBtnSteadyColor() 
{
	// TODO: Add your control notification handler code here
	if( m_nSubGraphIndex < 0) return;
	
	COLORREF crColor = m_btnSteadyColor.GetColor();
	CColorDialog color(crColor, CC_FULLOPEN);
	if(color.DoModal() == IDOK) 
	{
		// 지표 스타일이 "채움식" 일 때에는 지표선의 선 색상이 다르게 적용된다.
		if( m_pIGraphInfo->GetSubGraphStyle( m_nSubGraphIndex) ==  CGraphBaseData::FillLine )
		{
			m_pIGraphInfo->SetSubGraphColor( m_nSubGraphIndex, 5, color.GetColor());
			
			ILPCSTR szSubGraphName = m_pIGraphInfo->GetSubGraphNameString( m_nSubGraphIndex);
			if( strcmp( szSubGraphName, "Psychological Line"))
				m_pIGraphInfo->SetSubGraphColor( m_nSubGraphIndex, 6, color.GetColor());
			
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

void CSetting_GlanceBalance::OnBtnUp1() 
{
	// TODO: Add your control notification handler code here
	if(m_nSubGraphIndex < 0) return;
	
	COLORREF crColor = m_btnUp1Color.GetColor();
	CColorDialog color(crColor, CC_FULLOPEN);
	if(color.DoModal() == IDOK) 
	{
		m_pIGraphInfo->SetSubGraphColor( m_nSubGraphIndex, 1, color.GetColor());
		m_pIGraphInfo->SetSubGraphColor( m_nSubGraphIndex, 2, color.GetColor());
		m_btnUp1Color.SetColor( m_pIGraphInfo->GetSubGraphColor( m_nSubGraphIndex, 1));
	}	
	
	m_btnUp1Color.Invalidate();		
}

void CSetting_GlanceBalance::OnBtnDown1() 
{
	// TODO: Add your control notification handler code here
	if(m_nSubGraphIndex < 0) return;
	
	COLORREF crColor = m_btnDown1Color.GetColor();
	CColorDialog color(crColor, CC_FULLOPEN);
	if(color.DoModal() == IDOK) 
	{
		m_pIGraphInfo->SetSubGraphColor( m_nSubGraphIndex, 3, color.GetColor());
		m_pIGraphInfo->SetSubGraphColor( m_nSubGraphIndex, 4, color.GetColor());
		m_btnDown1Color.SetColor( m_pIGraphInfo->GetSubGraphColor( m_nSubGraphIndex, 3));
	}
	
	m_btnDown1Color.Invalidate();		
}


void CSetting_GlanceBalance::OnClickListComposition(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	// 1. 최소 하나 이상의 구성 설정이 체크되어 있어야 한다.
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	OnApplySubGraphData();
	Initial_SubGraphData( pNMListView->iItem );
	
	*pResult = 0;
}


void CSetting_GlanceBalance::OnItemchangedListComposition(NMHDR* pNMHDR, LRESULT* pResult) 
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

	if(bChecked)	// 그래프 추가
		m_pIGraphInfo->SetSubGraphShowFlag( pNMListView->iItem, TRUE);
	
	else			// 그래프 제거
		m_pIGraphInfo->SetSubGraphShowFlag( pNMListView->iItem, FALSE);

	if( bReCheck) m_pIGraphInfo->SetSubGraphShowFlag( pNMListView->iItem, TRUE);
}


void CSetting_GlanceBalance::OnApplySubGraphData( void )
{
	// 선색상은 OnCompositioncolor에서 바로 적용
	ILPCSTR szSubGraphName = m_pIGraphInfo->GetSubGraphNameString( m_nSubGraphIndex);
	for( int idx = 0 ; idx < m_lstComposition.GetItemCount() ; idx++ )
	{
		if( !strcmp( m_lstComposition.GetItemText( idx, 0 ), szSubGraphName))
			m_pIGraphInfo->SetSubGraphShowFlag( m_nSubGraphIndex, m_lstComposition.GetCheck( idx));
	}

	OnApply_Title( m_nSubGraphIndex, m_edtCompositionTitleData);
	OnApply_DrawStyle( m_nSubGraphIndex, m_cmbLineType);
	OnApply_Thickness( m_nSubGraphIndex, m_cmbThickness);
	//sy 2005.11.11
	OnApply_GraphStyle(m_nSubGraphIndex);
}


void CSetting_GlanceBalance::Initial_SubGraphData( const int nIndex )
{
	if( m_pIGraphInfo == NULL) return;

	int nOldSubGraphIndex = m_nSubGraphIndex;
	if( nIndex > m_pIGraphInfo->GetSubGraphCount() - 1 ) m_nSubGraphIndex = m_pIGraphInfo->GetSubGraphCount() - 1;
	else m_nSubGraphIndex = ( nIndex < 0 ? 0 : nIndex);

	if( nOldSubGraphIndex == m_nSubGraphIndex) return;

	Initial_Title(m_nSubGraphIndex, m_edtCompositionTitleData);
	Initial_DrawStyleComboBox(m_nSubGraphIndex, m_cmbLineType);
	SetStatePacketCombo( m_pIGraphInfo, m_cmbData, m_stcDataTitle);
	m_cmbThickness.SetCurSel( m_pIGraphInfo->GetSubGraphPenThickness( m_nSubGraphIndex, 1) - 1);

	//sy 2005.11.02.
	Init_Composition_Group_SubGraphColor(m_nSubGraphIndex);
}

void CSetting_GlanceBalance::OnSelchangeCmbLinetype() 
{
	// TODO: Add your control notification handler code here
	if( m_cmbThickness.GetCurSel() != 0 && m_cmbLineType.GetCurSel() != 0 )
	{
// 2008.01.25 by LYH >>
		//AfxMessageBox("선모양이 \"실선\"인 경우만 굵기를 변경할 수 있습니다.");
		AfxMessageBox(m_strMsgLineThick);
// 2008.01.25 by LYH >>
		m_cmbThickness.SetCurSel(0);
		m_cmbThickness.EnableWindow(FALSE);
		return;
	}

	else
		m_cmbThickness.EnableWindow(TRUE);	
}

void CSetting_GlanceBalance::OnSelchangeCmbThickness() 
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

BOOL CSetting_GlanceBalance::PreTranslateMessage(MSG* pMsg) 
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

// 일목균형표 설정 커서 적용 - ojtaso (20071109)
void CSetting_GlanceBalance::OnApply_SpanCursor()
{
	if(m_pIAddInToolMgr)
		m_pIAddInToolMgr->OnAddInToolCommand( EAI_GLANCEBALANCE_CURSOR_SET_STATE);
}