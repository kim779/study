// BaseDialog.cpp: implementation of the CBaseDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartEnvSetupDlgAddIn.h"
#include "BaseDialog.h"

#include "GraphData.h"
#include "Conversion.h"

// 2008.01.25 by LYH >>
#include "../Include_Chart/Dll_Load/IMetaTable.h"		// for g_iMetaTable
// 2008.01.25 by LYH <<

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 18
// Return void : 
// Param  UINT nIDTemplate : 
// Param  CWnd* pParentWnd : 
// Param  IGraphInfo* p_pIGraphInfo/* = NULL*/ : : 
// Comments		: Construction
//-----------------------------------------------------------------------------
CBaseDialog::CBaseDialog( UINT nIDTemplate, CWnd *pParentWnd, IChartOCX *p_pIChartOCX, CWnd *pTopWnd):
	CDialog(nIDTemplate, pParentWnd)
{
	// 1. Member 변수들을 초기화한다.
	m_bDoCalculate = false;

	// (2009/9/29 - Seung-Won, Bae) for WorldOn
	m_eChartMode = CChartInfo::HTS;

	// (2004.10.15, 배승원) OCX Interface가 NULL인 경우를 대비하여, Member에 미리 NULL 초기화를 처리한다.
	m_pIPacketManager		= NULL;
	m_pIChartManager		= NULL;
	m_pIGraphInfoManager	= NULL;
	m_pIAddInToolMgr		= NULL;
	m_pIChartOCX			= p_pIChartOCX;
	if( m_pIChartOCX)
	{
		m_pIChartOCX->AddRef();
		m_pIPacketManager		= m_pIChartOCX->GetIDefaultPacketManager();
		m_pIChartManager		= m_pIChartOCX->GetIChartManager();
		m_pIGraphInfoManager	= m_pIChartOCX->GetIGraphInfoManager();
		m_pIAddInToolMgr		= m_pIChartOCX->GetIAddInToolMgr();

		// (2009/9/29 - Seung-Won, Bae) for WorldOn
		m_eChartMode = ( CChartInfo::CHART_MODE)m_pIChartOCX->GetChartMode();
	}

	m_bSaveDefault = FALSE;
	m_bOrgSaveDefault = FALSE;
	m_nSubGraphIdx = 0;
	m_pTopWnd = pTopWnd;

	// 2. 인자로 받은 p_pIGraphInfo를 이용하여 m_pIGraphInfo를 생성한다.
	m_pIGraphInfo = NULL;
	m_bUseMultiplePriceData = FALSE;

// 2008.01.25 by LYH >>
	m_hOcxWnd = m_pIChartOCX->GetOcxHwnd();
	m_strMsgLineThick.LoadString(IDS_MSG_LINE_THICK);
	m_strMsgLineShape.LoadString(IDS_MSG_LINE_SHAPE);
	m_strMsgExistMin.LoadString(IDS_MSG_EXIST_MIN);
	m_strUp.LoadString(IDS_STRING_UP);
	m_strDown.LoadString(IDS_STRING_DOWN);
// 2008.01.25 by LYH <<
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 18
// Modifier		: 
// Comments		: Destruction
//-----------------------------------------------------------------------------
CBaseDialog::~CBaseDialog()
{
	// 1. 생성시킨 m_pIGraphInfo의 메모리를 해지한다.
	if( m_pIGraphInfo)
	{
		m_pIGraphInfo->Release();
		m_pIGraphInfo = NULL;
	}

	// (2004.10.08, 배승원) Chart OCX Interface를 관리하여, 이후 타 Object도 조회할 수 있도록 한다.
	if( m_pIChartOCX) m_pIChartOCX->Release();

	// (2004.10.11, 배승원) OCX로부터 주요 Manager의 Interface를 구하여 관리한다.
	if( m_pIPacketManager)
	{
		m_pIPacketManager->Release();
		m_pIPacketManager = NULL;
	}
	if( m_pIChartManager)
	{
		m_pIChartManager->Release();
		m_pIChartManager = NULL;
	}
	if( m_pIGraphInfoManager)
	{
		m_pIGraphInfoManager->Release();
		m_pIGraphInfoManager = NULL;
	}
	if( m_pIAddInToolMgr)
	{
		m_pIAddInToolMgr->Release();
		m_pIAddInToolMgr = NULL;
	}
}

CString CBaseDialog::GetIndicatorName() const
{
	if(m_pIGraphInfo == NULL)
		return "";

	ILPCSTR szGraphName = m_pIGraphInfo->GetGraphNameString();
	CString strGraphName( szGraphName);
	return strGraphName;
}

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 18
// Return void : 
// Param  CMainBlock *pMainBlock : 
// Comments		: 인자로 받은 pMainBlock의 값을 m_pMainBlock에 저장한다.
//-----------------------------------------------------------------------------
//sy test
/*void CBaseDialog::SetMainBlock( CMainBlock *pMainBlock )
{
	m_pMainBlock = pMainBlock;
}*/

/*
//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 18
// Return double : 
// Param  const CString &strCondition : 
// Param  const int nIndex :  nIndex : 1부터 시작
// Comments		: strCondition에서 nIndex 번째의 내용을 찾아서 double 형태로 return 한다.
//-----------------------------------------------------------------------------
double CBaseDialog::GetCondition( const CString &szCondition, const int nIndex )
{
	CString szData = szCondition;
	if(szData.Right(1) != ",")
		szData += ",";

	// 1. strCondition에서 nIndex 번째의 내용을 ","를 기준으로 잘라낸다.
	for( int i = 1 ; i < nIndex ; i++ )
		CDataConversion::GetStringData( szData, _T(",") );

	// 2. 찾은 값을 dluble형태로 return 한다.
	return atof( CDataConversion::GetStringData( szData, _T(",") ) );	
}
*/


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 18
// Return CString : 
// Param  CEdit &Condi_Edit : 
// Comments		: IGraphInfo에서 값을 넣어주기전에 구분자로 사용하는 ","를  붙여준다.
//-----------------------------------------------------------------------------
CString CBaseDialog::OnApply_Condition( CEdit &Condi_Edit )
{
	CString szConditionData;
	Condi_Edit.GetWindowText( szConditionData );

	return szConditionData += ",";
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 18
// Return void : 
// Param  CNumSpinCtrl &pSpin : 
// Param  CNumEdit &pEdit : 
// Param  const double Value : 
// Comments		: 설정부분 초기화
//-----------------------------------------------------------------------------
void CBaseDialog::Initial_Spin( CNumSpinCtrl &pSpin, CNumEdit &pEdit, const double Value )
{
	CNumEdit *pNumEdit = pSpin.GetBuddy();
	
	// 1. 인자로 받은 pEdit으로 Period에 값을 설정한다.
	if( pNumEdit == NULL || *pNumEdit != pEdit )
		pSpin.SetBuddy( &pEdit ); 
	
	// 2. Spin을 초기화 한다.
	pSpin.SetRange( 0, 10000);
	pSpin.SetPos( float( Value ) );
	pSpin.SetDelta( 1 );	
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 21
// Return void : 
// Param  CNumSpinCtrl &pSpin : 
// Param  CNumEdit &pEdit : 
// Param  const double Value : 
// Param  const float StartRange : 
// Param  const float EndRange : 
// Param  const float delta : 
// Comments		: 설정부분 초기화
//-----------------------------------------------------------------------------
void CBaseDialog::Initial_Spin( CNumSpinCtrl &pSpin, CNumEdit &pEdit, const double Value, const float StartRange, const float EndRange, const float delta )
{
	CNumEdit *pNumEdit = pSpin.GetBuddy();

	// 1. 인자로 받은 pEdit으로 Period에 값을 설정한다.
	if( pNumEdit == NULL || *pNumEdit != pEdit )
		pSpin.SetBuddy( &pEdit );
	
	// 2. 인자로 받은 범위값으로 Spin을 초기화 한다.
	pSpin.SetRange( StartRange, EndRange );
	pSpin.SetPos( float( Value ) );

	// 20081014 박동관 >>
	//pSpin.SetDelta( delta );
	CString strType = GetPacketType();
	if(CDataConversion::IsBaseFormat(strType))
	{
		// 진법 변환일 경우... 
		pSpin.SetDelta( delta );
	}
	else
	{
//>> vntsorl(20090828)
		if(strType.Find('%') == -1)
		{
			pSpin.SetDelta( delta );
		}
		else
		{
			// 진법 변환이 아닐 경우...
			CString strValue = strType;
			strValue.Remove('%');
			
			float fDelta = 1.0;
			int nPos = strValue.Find('.');
			if(nPos >= 0)
			{
				CString strPoint = strValue.Mid(nPos-1);
				fDelta = ( float)atof(strPoint);
			}
			
			pSpin.SetDelta(fDelta);
		}
//<< vntsorl(20090828)
	}
	// 20081014 박동관 <<
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 21
// Return void : 
// Param  int p_nSubGraphIndex : 
// Param  CBitmapItemComboBox &Drawsytle_Combo : 
// Comments		: DrawStyleComboBox을 초기화 한다. 
//-----------------------------------------------------------------------------
void CBaseDialog::Initial_DrawStyleComboBox( int p_nSubGraphIndex, CBitmapItemComboBox &Drawsytle_Combo )
{
	if( p_nSubGraphIndex < 0) return;

	ML_SET_DEFAULT_RES();

	// 1. BitMap을 Load한다.
	LoadBitmapToDrawStyleCombo( p_nSubGraphIndex, Drawsytle_Combo );
	
	// 2. 값을 setting한다.
	for( int i = -1 ; i < Drawsytle_Combo.GetCount() ; i++ ) 
		Drawsytle_Combo.SetItemHeight( i, 14 );

	Drawsytle_Combo.SetCurSel( m_pIGraphInfo->GetSubGraphDrawStyle( p_nSubGraphIndex));
}

// 기준선 두께, 모양 설정 - ojtaso (20071213)
void CBaseDialog::Initial_DrawBaseLineStyleComboBox( int p_nSubGraphIndex, CBitmapItemComboBox &Drawsytle_Combo )
{
	if( p_nSubGraphIndex < 0) return;

	ML_SET_DEFAULT_RES();

	// 1. BitMap을 Load한다.
	// 기준선 두께, 모양 설정 - ojtaso (20071213)
	RealLoadBitmap( 5, IDB_LINE_STYLE, Drawsytle_Combo );
	
	// 2. 값을 setting한다.
	for( int i = -1 ; i < Drawsytle_Combo.GetCount() ; i++ ) 
		Drawsytle_Combo.SetItemHeight( i, 14 );

	// 기준선 두께, 모양 설정 - ojtaso (20071213)
	Drawsytle_Combo.SetCurSel( m_pIGraphInfo->GetSubGraphDrawBaseLineStyle( p_nSubGraphIndex));
}

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 21
// Return void : 
// Param  int p_nSubGraphIndex : 
// Param  int idStatic : 
// Param  CBitmapItemComboBox &Thickness_Combo : 
// Comments		: ThicknessComboBox를 초기화 한다.
//-----------------------------------------------------------------------------
void CBaseDialog::Initial_ThicknessComboBox( int p_nSubGraphIndex, int idStatic, CBitmapItemComboBox &Thickness_Combo )
{
	if( p_nSubGraphIndex < 0) return;

	ML_SET_DEFAULT_RES();	

	// 1. BitMap을 Load한다.
	if( !Thickness_Combo.m_bitmap.GetSafeHandle()) Thickness_Combo.m_bitmap.LoadBitmap(IDB_LINE_THICKNESS);
	AddLineType( &Thickness_Combo, 5 );

	// 2. 값을 setting한다.
	for( int i = -1 ; i < Thickness_Combo.GetCount() ; i++ ) 
		Thickness_Combo.SetItemHeight( i, 14 );

	Thickness_Combo.SetCurSel( m_pIGraphInfo->GetSubGraphPenThickness( p_nSubGraphIndex, 1) - 1);

	// 3. 선형이고, static 아이디가 들어오지 않았을때는 그냥 나간다.
	if( idStatic == 0 || m_pIGraphInfo->GetSubGraphType( p_nSubGraphIndex) == CGraphBaseData::Line_Type )
		return;

	// 4. 인자로 받은 Static과 Thickness_Combo를 숨겨둔다.
	CStatic *pStatic = ( CStatic * )GetDlgItem( idStatic );
	pStatic->ShowWindow( SW_HIDE );
	Thickness_Combo.ShowWindow( SW_HIDE );
}

// 기준선 두께, 모양 설정 - ojtaso (20071213)
void CBaseDialog::Initial_BaseLineThicknessComboBox( int p_nSubGraphIndex, int idStatic, CBitmapItemComboBox &Thickness_Combo )
{
	if( p_nSubGraphIndex < 0) return;

	ML_SET_DEFAULT_RES();

	// 1. BitMap을 Load한다.
	if( !Thickness_Combo.m_bitmap.GetSafeHandle()) Thickness_Combo.m_bitmap.LoadBitmap(IDB_LINE_THICKNESS);
	AddLineType( &Thickness_Combo, 5 );

	// 2. 값을 setting한다.
	for( int i = -1 ; i < Thickness_Combo.GetCount() ; i++ ) 
		Thickness_Combo.SetItemHeight( i, 14 );

	// 기준선 두께, 모양 설정 - ojtaso (20071213)
	Thickness_Combo.SetCurSel( m_pIGraphInfo->GetBaseLinePenThickness( p_nSubGraphIndex, 1) - 1);

	// 3. static 아이디가 들어오지 않았을때는 그냥 나간다.
	// 기준선 두께, 모양 설정 - ojtaso (20071213)
	if( idStatic == 0)
		return;

	// 4. 인자로 받은 Static과 Thickness_Combo를 숨겨둔다.
	CStatic *pStatic = ( CStatic * )GetDlgItem( idStatic );
	pStatic->ShowWindow( SW_HIDE );
	Thickness_Combo.ShowWindow( SW_HIDE );
}

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 21
// Return void : 
// Param  int p_nSubGraphIndex : 
// Param  CEdit &Title_Edit : 
// Comments		: Title을 setting한다.
//-----------------------------------------------------------------------------
void CBaseDialog::Initial_Title( int p_nSubGraphIndex, CEdit &Title_Edit )
{
	if( p_nSubGraphIndex < 0) return;

	// 1. 인자로 받은 Title_Edit에 Title을 setting한다. 
	ILPCSTR szTitle = m_pIGraphInfo->GetSubGraphTitleString( p_nSubGraphIndex);

// 2008.01.25 by LYH >>
	//Title_Edit.SetWindowText( szTitle);
	CString strUIText = _MTOL1_N( szTitle);
	if( strUIText.IsEmpty()) strUIText = _MTOL3( szTitle);

	Title_Edit.SetWindowText( strUIText);
// 2008.01.25 by LYH <<
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 21
// Return void : 
// Param  int p_nSubGraphIndex : 
// Param  CButton &SubName_Bt : 
// Comments		: SubGraph의 이름을 지정한다.
//-----------------------------------------------------------------------------
void CBaseDialog::Initial_SubGraphName( int p_nSubGraphIndex, CButton &SubName_Bt )
{
	if( p_nSubGraphIndex < 0) return;

	// 1. SubName_Bt에 p_nSubGraphIndex의 이름을 setting한다.
	ILPCSTR szName = m_pIGraphInfo->GetSubGraphNameString( p_nSubGraphIndex);
	SubName_Bt.SetWindowText( szName);
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 21
// Return void : 
// Param  int p_nSubGraphIndex : 
// Param  CButton &FillUp_Bt : 
// Comments		: 상승 채움 CheckButton의 값을 setting한다.
//-----------------------------------------------------------------------------
void CBaseDialog::Initial_FillUp( int p_nSubGraphIndex, CButton &FillUp_Bt )
{
	if( p_nSubGraphIndex < 0) return;

	// 1. FillUp_Bt의 상태값을 setting한다.
	FillUp_Bt.SetCheck( m_pIGraphInfo->GetSubGraphFillFlag( p_nSubGraphIndex, 1));
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 21
// Return void : 
// Param  int p_nSubGraphIndex : 
// Param  CButton &FillDown_Bt : 
// Comments		: 하락 채움 CheckButton의 값을 setting한다.
//-----------------------------------------------------------------------------
void CBaseDialog::Initial_FillDown( int p_nSubGraphIndex, CButton &FillDown_Bt )
{
	if( p_nSubGraphIndex < 0) return;

	// 1. FillDown_Bt의 상태값을 setting한다.
	FillDown_Bt.SetCheck( m_pIGraphInfo->GetSubGraphFillFlag( p_nSubGraphIndex, -1));
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 21
// Return void : 
// Param  int p_nSubGraphIndex : 
// Param  CBitmapItemComboBox &Drawsytle_Combo : 
// Comments		: Graph의 type에 따라 Drawsytle_Combo의 값을 setting한다.
//-----------------------------------------------------------------------------
void CBaseDialog::LoadBitmapToDrawStyleCombo( int p_nSubGraphIndex, CBitmapItemComboBox &Drawsytle_Combo )
{
	if( p_nSubGraphIndex < 0) return;

	// 1. Graph의 type에 따라 해당되는 bitMap을 load한다.
	switch( m_pIGraphInfo->GetSubGraphType( p_nSubGraphIndex))
	{
		// 봉형
		case CGraphBaseData::Bong_Type :		
		{
			RealLoadBitmap( 3, IDB_BONG_STYLE, Drawsytle_Combo );							// 일본식, 미국식.
		
			if( m_pIGraphInfo->GetSubGraphStyle( p_nSubGraphIndex) == CGraphBaseData::LineBong )	// 라인형
			{ 
				Drawsytle_Combo.SetCurSel( m_pIGraphInfo->GetSubGraphDrawStyle( p_nSubGraphIndex));
				Drawsytle_Combo.EnableWindow( FALSE );
			}
	
			break;
		}

		// 선형
		case CGraphBaseData::Line_Type :		
		{
			RealLoadBitmap( 5, IDB_LINE_STYLE, Drawsytle_Combo );
			break;
		}
		
		// 바형
		case CGraphBaseData::Bar_Type :			
		{
			RealLoadBitmap( 2, IDB_BAR_STYLE, Drawsytle_Combo );
			break;
		}
	
		// 파이형
		case CGraphBaseData::Pie_Type :			
		{
			RealLoadBitmap( 2, IDB_PIE_STYLE, Drawsytle_Combo );
			break;
		}

		// 특수형
		case CGraphBaseData::Special_Type :	
		{
			RealLoadBitmap( 3, IDB_SPECIAL_STYLE, Drawsytle_Combo );
			break;
		}
	
		// 해당 사항이 없으면 Drawsytle_Combo를 숨김
		default:
		{
			Drawsytle_Combo.EnableWindow( FALSE );
		}
	}
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 21
// Return void : 
// Param  CComboBox *pCombo : 
// Param  int Type : 
// Comments		: pCombo에 LineType을 등록한다.
//-----------------------------------------------------------------------------
void CBaseDialog::AddLineType( CComboBox *pCombo, int Type )
{
	ASSERT( pCombo != NULL );
	
	// 1. Combo를 초기화 시킨다.
	pCombo->ResetContent();

	// 2. LineType을 지정한다.
	for( int i = 0 ; i < Type ; i++ )
		pCombo->AddString( szType[ i ] );
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 21
// Return void : 
// Param  int p_nSubGraphIndex : 
// Param  CEdit &Title_Edit : 
// Comments		: 현재 설정한 Graph Title을 적용한다.
//-----------------------------------------------------------------------------
void CBaseDialog::OnApply_Title( int p_nSubGraphIndex, CEdit &Title_Edit )
{
	if( p_nSubGraphIndex < 0) return;
	
	// 1. 현재 설정한 Graph Title을 적용한다.
	CString str;
	Title_Edit.GetWindowText( str );
	m_pIGraphInfo->SetSubGraphTitle( p_nSubGraphIndex, str);
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 21
// Return void : 
// Param  int p_nSubGraphIndex : 
// Param  CBitmapItemComboBox &Drawsytle_Combo : 
// Comments		: Drawsytle_Combo의 설정 값을 적용한다.
//-----------------------------------------------------------------------------
void CBaseDialog::OnApply_DrawStyle( int p_nSubGraphIndex, CBitmapItemComboBox &Drawsytle_Combo )
{
	if( p_nSubGraphIndex < 0) return;

	// 1. Drawsytle_Combo의 값이 정상이 아니면 저장되어있던 값을 얻어와서 다시 저장해준다.
	if( Drawsytle_Combo.GetCurSel() < 0 )
	{
		Drawsytle_Combo.SetCurSel( m_pIGraphInfo->GetSubGraphDrawStyle( p_nSubGraphIndex));
		return;
	}

	// 2. 현재의 값을 저장한다.
	m_pIGraphInfo->SetSubGraphDrawStyle( p_nSubGraphIndex, Drawsytle_Combo.GetCurSel());
}

// 기준선 두께, 모양 설정 - ojtaso (20071213)
void CBaseDialog::OnApply_BaseLineDrawStyle( int p_nSubGraphIndex, CBitmapItemComboBox &Drawsytle_Combo )
{
	if( p_nSubGraphIndex < 0) return;

	// 1. Drawsytle_Combo의 값이 정상이 아니면 저장되어있던 값을 얻어와서 다시 저장해준다.
	if( Drawsytle_Combo.GetCurSel() < 0 )
	{
		Drawsytle_Combo.SetCurSel( m_pIGraphInfo->GetSubGraphDrawBaseLineStyle( p_nSubGraphIndex));
		return;
	}

	// 2. 현재의 값을 저장한다.
	m_pIGraphInfo->SetSubGraphDrawBaseLineStyle( p_nSubGraphIndex, Drawsytle_Combo.GetCurSel());
}

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 21
// Return void : 
// Param  int p_nSubGraphIndex : 
// Param  CBitmapItemComboBox &Drawsytle_Combo : 
// Comments		: Drawsytle_Combo의 두깨값을 저장한다.
//-----------------------------------------------------------------------------
void CBaseDialog::OnApply_Thickness( int p_nSubGraphIndex, CBitmapItemComboBox &Drawsytle_Combo )
{
	if( p_nSubGraphIndex < 0) return;

	// 1. 현재의 Drawsytle_Combo의 두깨값을 저장한다.
	m_pIGraphInfo->SetSubGraphPenThickness( p_nSubGraphIndex, Drawsytle_Combo.GetCurSel() + 1, Drawsytle_Combo.GetCurSel() + 1, Drawsytle_Combo.GetCurSel() + 1);
}

// 기준선 두께, 모양 설정 - ojtaso (20071213)
void CBaseDialog::OnApply_BaseLineThickness( int p_nSubGraphIndex, CBitmapItemComboBox &Drawsytle_Combo )
{
	if( p_nSubGraphIndex < 0) return;

	// 1. 현재의 Drawsytle_Combo의 두깨값을 저장한다.
	m_pIGraphInfo->SetBaseLinePenThickness( p_nSubGraphIndex, Drawsytle_Combo.GetCurSel() + 1);
}

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 21
// Return void : 
// Param  int p_nSubGraphIndex : 
// Param  CButton &FillUp_Bt : 
// Comments		: 상승 채움 CheckButton의 값을 저장한다.
//-----------------------------------------------------------------------------
void CBaseDialog::OnApply_FillUp( int p_nSubGraphIndex, CButton &FillUp_Bt )
{
	if( p_nSubGraphIndex < 0) return;

	// 1. 현재 FillUp_Bt의 Check상태의 값을 저장한다.
	m_pIGraphInfo->SetSubGraphFillFlag( p_nSubGraphIndex, 1, itob( FillUp_Bt.GetCheck()));
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 21
// Return void : 
// Param  int p_nSubGraphIndex : 
// Param  CButton &FillDown_Bt : 
// Comments		: 하락 채움 CheckButton의 값을 저장한다.
//-----------------------------------------------------------------------------
void CBaseDialog::OnApply_FillDown( int p_nSubGraphIndex, CButton &FillDown_Bt )
{
	if( p_nSubGraphIndex < 0) return;

	// 1. 현재 FillDown_Bt Check상태의 값을 저장한다.
	m_pIGraphInfo->SetSubGraphFillFlag( p_nSubGraphIndex, -1, itob( FillDown_Bt.GetCheck()));
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 21
// Return void : 
// Param  IGraphInfo *p_pIGraphInfo : 
// Param  int p_nSubGraphIndex : 
// Param  CListBoxColorPickerST *pColor_List : 
// Comments		: 
//-----------------------------------------------------------------------------
void CBaseDialog::Initial_ColorListBox( IGraphInfo *p_pIGraphInfo, int p_nSubGraphIndex,
									   CListBoxColorPickerST *pColor_List )
{
	if( p_nSubGraphIndex < 0) return;
	if( p_pIGraphInfo == NULL ) return;

	// 1. ColorList의 Color값을 얻어와서 저장한다.
	m_MgrColorList.SetItem( p_pIGraphInfo, p_nSubGraphIndex, pColor_List );
	
	pColor_List->SetCurSel( 0);
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 21
// Return void : 
// Param  const CString &data : 
// Param  CBaseListBox &pList : 
// Comments		:  IGraphInfo에서 설정값을 가져오려면 구분자를 ","로 해야한다.
//-----------------------------------------------------------------------------
void CBaseDialog::Initial_ListBox( IDoubleList *p_pIDblBase, CBaseListBox &pList)
{
	if( !p_pIDblBase) return;

	// 1. strData에서 ","의 개수를 계산한다.
	double dData = 0;
	CString strData;
	POSITION psnBase = p_pIDblBase->GetHeadPosition();

	// 20081014 박동관 패킷타입에 따라 값을 다르게 가져온다. >>
	CString strType = GetPacketType();

	if(CDataConversion::IsBaseFormat(strType))
	{
		// 진법변환일 경우...
		while(psnBase)
		{
			p_pIDblBase->GetNext( psnBase, dData);

			// 정수를 패킷타입에 맞게 진법변환 한다.
			CString strData = CDataConversion::GetStringThousandFormat(dData, strType);
			pList.AddString(strData);
		}
	}
	else
	{
		// 진법변환이 아닐 경우...
		CString strValue = strType;
		strValue.Remove('%');

		int nPoint = 0;
		int nPos = strValue.Find('.');
		if(nPos >= 0)
		{
			CString strPoint = strValue.Mid(nPos+1);
			nPoint = strPoint.GetLength();
		}

		while(psnBase)
		{
			p_pIDblBase->GetNext( psnBase, dData);
			strData.Format("%.*f", nPoint, dData);
			pList.AddString(strData);
		}
	}
	// 20081014 박동관 <<

//	while( psnBase)
//	{
//		p_pIDblBase->GetNext( psnBase, dData);
//		// 기준선 두께, 모양 설정 - ojtaso (20071213)
//		strData.Format( "%f", dData);
//		pList.AddString( TrimStringWhenDecimalDataIsZero(strData));
//	}

	pList.SetCurSel( 0 );
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 21
// Return void : 
// Param  CListBoxColorPickerST *pColor_List : 
// Param  IGraphInfo *p_pIGraphInfo : 
// Param  int p_nSubGraphIndex : 
// Comments		: 
//-----------------------------------------------------------------------------
void CBaseDialog::OnApply_ColorListBox( CListBoxColorPickerST *pColor_List, IGraphInfo *p_pIGraphInfo,
									   int p_nSubGraphIndex )
{
	m_MgrColorList.OnApply(pColor_List, p_pIGraphInfo, p_nSubGraphIndex);
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 18
// Return CString : 
// Param  const CString &strInputData : 
// Comments		:  Double의 소숫점 이하값이 0이면 그냥 정수만 리턴하고, 소숫점 이하에
//              : 값이 있으면 정수와 소수를 더해서 리턴한다.
//-----------------------------------------------------------------------------
CString CBaseDialog::TrimStringWhenDecimalDataIsZero( const CString &strInputData )
{
	// 1. 입력받은 문자열을 Double 형으로 바꾼다.
	double dData = atof(strInputData);

	// 2. 소수점 이하의 불필요한 0을 제거한다.
	CString strData = _T("");
	strData.Format("%g", dData);
	
	return strData;
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 21
// Return void : 
// Param  CListBoxColorPickerST &Color_List : 
// Comments		: 
//-----------------------------------------------------------------------------
void CBaseDialog::DoModal_ColorDialog( CListBoxColorPickerST &Color_List )
{
	CString strText;
	COLORREF crColor;
	int nIndex = Color_List.GetCurSel();
	
	// 1. ColorListBox에서 색상 정보를 얻어온다.
	Color_List.GetTextAndColor( nIndex, strText, &crColor );

	// 2. 색상을 설정하는 ColorDialog를 연다.
	CColorDialog dlg( crColor, CC_FULLOPEN | CC_RGBINIT );
	
	if( dlg.DoModal() == IDOK ) 
		Color_List.ReplaceString( nIndex, strText, dlg.GetColor() );
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 21
// Return CString : 
// Param  const CString &strIndicatorName : 
// Param  const int nIndex : 
// Comments		: 
//-----------------------------------------------------------------------------
CString CBaseDialog::GetConditionTitle( const char *p_szIndicatorName, const int nIndex )
{
	CString title = m_DataConditionTitle.GetConditionTitle( p_szIndicatorName);

	for(int i = 1 ; i < nIndex ; i++) 
		CDataConversion::GetStringData(title, ",");
	return CDataConversion::GetStringData(title, ",");	
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 3
// Return void : 
// Param  CStatic &condi_static : 
// Param  CNumEdit &Condi_Edit : 
// Param  CNumSpinCtrl &Condi_Spin : 
// Comments		: 
//-----------------------------------------------------------------------------
void CBaseDialog::MoveWindow(CStatic &condi_static, CNumEdit &Condi_Edit, CNumSpinCtrl &Condi_Spin)
{
	CRect rctitle, rcEdit, rcSpin;
	condi_static.GetWindowRect(&rctitle);
	ScreenToClient(&rctitle);
	Condi_Edit.GetClientRect(&rcEdit);
	Condi_Spin.GetClientRect(&rcSpin);

	condi_static.MoveWindow(rctitle.left, rctitle.top, rctitle.Width(), rctitle.Height());
	Condi_Edit.SetWindowPos(&wndTop, rctitle.right, rctitle.top, rcEdit.Width(), rcEdit.Height(), SWP_NOSIZE | SWP_SHOWWINDOW);
	Condi_Spin.SetWindowPos(&wndTop, rctitle.right + rcEdit.Width(), rctitle.top, rcSpin.Width(), rcSpin.Height(), SWP_NOSIZE | SWP_SHOWWINDOW);
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 3
// Return void : 
// Param  const int nIndex : 
// Param  const CString &strTitle : 
// Comments		: 
//-----------------------------------------------------------------------------
void CBaseDialog::SetTitle(const int nIndex, const CString &strTitle)
{

}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 3
// Return void : 
// Param  CListBoxColorPickerST& Color_List : 
// Comments		: 
//-----------------------------------------------------------------------------
void CBaseDialog::DeleteString_ColorListBox(CListBoxColorPickerST& Color_List)
{
	for(int i = 0 ; i< Color_List.GetCount() ; i++) {
		Color_List.DeleteString(i);
	}		
	Color_List.ResetContent();
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 3
// Return bool : 
// Param  const CString &strIndicatorName : 
// Comments		: 
//-----------------------------------------------------------------------------
bool CBaseDialog::IsSpecialIndicator(const CString &strIndicatorName)
{
	//2008.01.25 by LYH Not Use Method
	if(strIndicatorName.CompareNoCase("삼선전환도") == 0) 
		return true;
	
	else if(strIndicatorName.CompareNoCase("역시계곡선") == 0) 
		return true;
	
	else if(strIndicatorName.CompareNoCase("P&F") == 0) 
		return true;
	
	return false;
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 3
// Return bool : 
// Param  const int nValue : 
// Comments		: 
//-----------------------------------------------------------------------------
bool CBaseDialog::itob(const int nValue)
{
	return nValue == 1 ? true : false;
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 3
// Return void : 
// Param  const int nCount : 
// Param  const int nIDBitmap : 
// Param  CBitmapItemComboBox& Drawsytle_Combo : 
// Comments		: 
//-----------------------------------------------------------------------------
void CBaseDialog::RealLoadBitmap(const int nCount, const int nIDBitmap, CBitmapItemComboBox& Drawsytle_Combo)
{
	if( Drawsytle_Combo.m_bitmap.GetSafeHandle()) Drawsytle_Combo.m_bitmap.DeleteObject();
	
	if (!Drawsytle_Combo.m_bitmap.LoadBitmap(nIDBitmap))
		return;   // need this one image
	
	AddLineType(&Drawsytle_Combo, nCount);
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 3
// Return void : 
// Param  IGraphInfo* p_pIGraphInfo : 
// Param  CComboBox &packetcombo : 
// Param  CStatic &packetstatic : 
// Comments		: 한가지 가격데이타를 사용하면 packet설정을 해야한다.
//				: 그렇지 않으면 packet 설정할 필요가 없다.
//-----------------------------------------------------------------------------
BOOL CBaseDialog::SetStatePacketCombo(IGraphInfo* p_pIGraphInfo, CComboBox &packetcombo, CStatic &packetstatic)
{
	if(p_pIGraphInfo == NULL)
		return FALSE;
	
	//해당 index에 cursor set하기.
	if(m_bUseMultiplePriceData) 
	{
		packetcombo.ShowWindow(SW_HIDE);
		packetstatic.ShowWindow(SW_HIDE);
		return FALSE;
	}

	packetcombo.ShowWindow(SW_SHOW);
	packetstatic.ShowWindow(SW_SHOW);
	
	ILPCSTR szPacketData = p_pIGraphInfo->GetSubGraphPacketNameString( m_nSubGraphIdx);
	if( szPacketData.IsEmpty()) szPacketData = p_pIGraphInfo->GetPacketNameString();
		
	CString packetdata = szPacketData;
	if(packetdata.Find(",") >= 0) packetdata.TrimRight(",");

	if( 0 <= packetdata.Find( '('))
	{
		if( packetdata == CString( "(") + _MTEXT( C0_HIGH) + "+" + _MTEXT( C0_LOW) + ")/2")
		{
			packetcombo.SetCurSel(4);
			return TRUE;
		}
		else if( packetdata == CString( "(") + _MTEXT( C0_HIGH) + "+" + _MTEXT( C0_LOW) + "+" + _MTEXT( C0_CLOSE) + ")/3")
		{
			packetcombo.SetCurSel(5);
			return TRUE;
		}
	}
	else
	{
		packetdata = _MTOLS( packetdata, ",", 0);
		CString data;
		for(int i = 0 ; i < packetcombo.GetCount() ; i++)
		{
			packetcombo.GetLBText( i, data);	
			if( packetdata.Compare(data) == 0)
			{
				packetcombo.SetCurSel(i);
				return TRUE;
			}
		}
	}
	
	packetcombo.SetCurSel(4);
		return TRUE;
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 3
// Return void : 
// Param  CComboBox &packetcombo : 
// Param  IGraphInfo* p_pIGraphInfo : 
// Comments		: packet 설정할 필요가 없는 경우에는 data도 set하면 안된다.
//-----------------------------------------------------------------------------
void CBaseDialog::OnApply_PacketData(CComboBox &packetcombo, IGraphInfo* p_pIGraphInfo)
{
	if(p_pIGraphInfo == NULL)
		return;
	
	if(m_bUseMultiplePriceData)
		return;

	CString data;
	int nIndex = packetcombo.GetCurSel();
	// (고가+저가)/2
	if(nIndex == 4)			data = CString( "(") + _MTEXT( C0_HIGH) + "+" + _MTEXT( C0_LOW) + ")/2";
	// (고가+저가+종가)/3
	else if(nIndex == 5)	data = CString( "(") + _MTEXT( C0_HIGH) + "+" + _MTEXT( C0_LOW) + "+" + _MTEXT( C0_CLOSE) + ")/3";
	else
	{
		packetcombo.GetLBText( nIndex, data);
		data = _LTOM0( data);
	}
	if(data.IsEmpty()) return;
	data += ",";

	if(!m_bDoCalculate) m_bDoCalculate = !DoesEqualCommonPakcetNames( p_pIGraphInfo, data);

	p_pIGraphInfo->SetPacketName( data);
}	


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 3
// Return void : 
// Param  CComboBox &packetcombo : 
// Param  IGraphInfo* p_pIGraphInfo : 
// Comments		: packet 설정할 필요가 없는 경우에는 data도 set하면 안된다.
//-----------------------------------------------------------------------------
void CBaseDialog::OnApply_SubPacketData(CComboBox &packetcombo, IGraphInfo* p_pIGraphInfo, int nSubGraphIdx )
{
	if(p_pIGraphInfo == NULL)
		return;
	
	if(!m_bUseMultiplePriceData)
		return;

	CString data;
	int nIndex = packetcombo.GetCurSel();
	// (고가+저가)/2
	if(nIndex == 4)			data = CString( "(") + _MTEXT( C0_HIGH) + "+" + _MTEXT( C0_LOW) + ")/2";
	// (고가+저가+종가)/3
	else if(nIndex == 5)	data = CString( "(") + _MTEXT( C0_HIGH) + "+" + _MTEXT( C0_LOW) + "+" + _MTEXT( C0_CLOSE) + ")/3";
	else
	{
		packetcombo.GetLBText( nIndex, data);
		data = _LTOM0( data);
	}
	if(data.IsEmpty()) return;
	data += ",";
	
	if(!m_bDoCalculate) m_bDoCalculate = !DoesEqualPacketNames_SubGraphPart( p_pIGraphInfo, nSubGraphIdx, data);

	p_pIGraphInfo->SetSubGraphPacketName( nSubGraphIdx, data);
}	

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 3
// Return bool : 
// Param  const CString& strSubGraphName : 
// Comments		: 
//-----------------------------------------------------------------------------
bool CBaseDialog::IsExistence_CurSubGraph(const CString& strSubGraphName)
{
	int nCount = m_pIGraphInfo->GetSubGraphCount();
	ILPCSTR szSubGraphName = NULL;
	for( int i = 0; i < nCount; i++)
	{
		szSubGraphName = m_pIGraphInfo->GetSubGraphNameString( i);
		if( strSubGraphName.CompareNoCase( szSubGraphName) == 0) return true;
	}
	return false;
}

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 3
// Return bool : 
// Comments		: 
//-----------------------------------------------------------------------------
bool CBaseDialog::DoCalculate()
{
	return m_bDoCalculate;
}

//////////////////////////////////////////////////////////////////////
// (2006/8/11 - Seung-Won, Bae) Support Condition String Handling Routines
//////////////////////////////////////////////////////////////////////
/*
BOOL CBaseDialog::DoesEqualCommonCalConditionList( IGraphInfo *p_pIGraphInfo, const CString& strData, const CString& strCompart) const
{
	IDoubleList *pIConditionList = p_pIGraphInfo->GetCommonCalculateConditionList();
	if( !pIConditionList) return FALSE;

	CDoubleList sourceData;
	SetDoubleList( strData, strCompart, &sourceData);
	BOOL bResult = DoesEqualDoubleList( pIConditionList, &sourceData);

	pIConditionList->Release();
	return bResult;
}
*/
void CBaseDialog::SetCommonCalConditionData( IGraphInfo *p_pIGraphInfo, const CString& strData, const CString& strCompart)
{
	IDoubleList *pIConditionList = p_pIGraphInfo->GetCommonCalculateConditionList();
	if( !pIConditionList) return;

	SetDoubleList( strData, strCompart, pIConditionList);
	pIConditionList->Release();
}
BOOL CBaseDialog::DoesEqualCommonPakcetNames( IGraphInfo *p_pIGraphInfo, const CString& strData) const
{
	// packetName 뒤에 있는 ',' 생각해서 비교하기.
	ILPCSTR szPacketName = p_pIGraphInfo->GetPacketNameString();
	CString strPacketName( szPacketName);
	return DoesEqualStringData( strPacketName, strData);
}
BOOL CBaseDialog::DoesEqualPacketNames_SubGraphPart( IGraphInfo *p_pIGraphInfo, const int nSubGraphIndex, const CString& strData) const
{
	ILPCSTR szPacketName = p_pIGraphInfo->GetSubGraphPacketNameString( nSubGraphIndex);
	CString strPacketName( szPacketName);

	// packetName 뒤에 있는 ',' 생각해서 비교하기.
	return DoesEqualStringData( strPacketName, strData);
}
void CBaseDialog::SetDoubleList( const CString& strData, const CString& strCompart, IDoubleList *p_ItargetList) const
{
	p_ItargetList->RemoveAll();
	if(strData.IsEmpty())
		return;

	// data가 하나일 경우
	if(strCompart.IsEmpty())
	{
		double dData = atof(strData);
		p_ItargetList->AddTail(dData);
		return;
	}

	// data가 두개 이상일 경우
	CString strTemp = strData;
	if(strTemp.Right(strCompart.GetLength()) != strCompart)
		strTemp += strCompart;

	while(!strTemp.IsEmpty())
	{
		CString strRealData = CDataConversion::GetStringData(strTemp, strCompart);
		p_ItargetList->AddTail(atof(strRealData));
	}
}
BOOL CBaseDialog::DoesEqualDoubleList( IDoubleList *p_pIDList1, IDoubleList *p_pIDList2) const
{
	POSITION pos1 = p_pIDList1->GetHeadPosition();
	POSITION pos2 = p_pIDList2->GetHeadPosition();
	double dData_s, dData_t;
	while( pos1 && pos2)
	{
		p_pIDList1->GetNext( pos1, dData_s);
		p_pIDList2->GetNext( pos2, dData_t);
		if(dData_s != dData_t) return FALSE;
	}
	return !pos1 && !pos2;
}

BOOL CBaseDialog::DoesEqualStringData(const CString& strTargetData, const CString& strSourceData) const
{
	if(strTargetData == strSourceData)
		return true;
	
	// packetName 뒤에 있는 ',' 생각해서 비교하기.
	CString strTargetData2 = strTargetData;
	if(strSourceData.Right(1) == ',' && strTargetData2.Right(1) != ',')
	{
		strTargetData2 += ',';
	}
	else if(strSourceData.Right(1) != ',' && strTargetData2.Right(1) == ',')
	{
		strTargetData2.TrimRight(',');
	}

	return (strTargetData2 == strSourceData);
}

// 20081014 박동관 >>
CString CBaseDialog::GetPacketType()
{
	CString strResult = "";
	IPacket* pIPacket = NULL;

	if(m_pIGraphInfo)
	{
		ILPCSTR szGraphName = m_pIGraphInfo->GetGraphNameString();
		ILPCSTR szSubGraphName = m_pIGraphInfo->GetSubGraphNameString( 0);
		ILPCSTR szPacketName = m_pIChartManager->GetSubGraphPacketNameString( szGraphName, szSubGraphName, 0);
		if( !szPacketName.IsEmpty()) pIPacket = m_pIPacketManager->GetPacket((LPCTSTR)szPacketName);
	}
	if( !pIPacket) pIPacket = m_pIPacketManager->GetPacket(_MTEXT(C0_CLOSE));

	if( pIPacket)
	{
		ILPCSTR szType = pIPacket->GetType();
		strResult = (LPCTSTR)szType;
		pIPacket->Release();
	}

	return strResult;
}
// 20081014 박동관 <<
