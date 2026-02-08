// MakeChildDlg.cpp: implementation of the CMakeChildDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartEnvSetupDlgAddIn.h"
#include "MakeChildDlg.h"

#include "FindType.h"

// 부모
#include "GraphSetting.h"
#include "DlgMainTabIndicatorSetUp.h"

// SubDlg
#include "BaseDialog.h"
#include "Setting_VolumeMA.h"
#include "Setting_PriceMA.h"
#include "Setting_Price.h"
#include "Setting_Volume.h"
#include "Setting_Special_Type_A.h"
#include "Setting_Special_Type_B.h"
#include "Setting_Special_Type_C.h"
#include "Setting_Special_Type_D.h"

// Sang-Woo, Cho		2006 / 04 / 19
// 대기매물 설정 Dlg
// Begin
#include "Setting_Special_Type_E.h"
// End


#include "Setting_Common_Type_A.h"
#include "Setting_Common_Type_B.h"			// Sang-Woo, Cho	2006 / 05 / 02 - BinaryWave 때문에 추가	
#include "Setting_GlanceBalance.h"
//#include "Setting_Common_Composition.h"
//#include "Setting_Common_Variables.h"
//#include "Setting_Common_BaseLine.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 26
// Return void : 
// Param  IGraphInfo *p_pIGraphInfo: 
// Param  CWnd *pParent : 
// Param  CBaseDialog * > *pChildDlg : 
// Param  IChartManager *p_pIChartManager : 
// Comments		: 각 Dialog Type에 맞게 지표설정Sub Dialog를 생성하고 ChildDlg List에
//				:   넣어둔다.
//-----------------------------------------------------------------------------
void CMakeChildDlg::Initial( IChartOCX *p_pIChartOCX, IGraphInfo* p_pIGraphInfo, CWnd *pParent, CBaseDialog *&pChildDlg, CWnd *pTopWnd)
{
	ILPCSTR szIndicatorName = p_pIGraphInfo->GetIndicatorNameString();
	CBaseData::DLGTYPE dlgtype = CFindType::FindDlgType( szIndicatorName);
	CBaseData::TABITEMTYPE	texttype = CFindType::FindTabItemText( szIndicatorName);

	if( pTopWnd != NULL )	m_pTopWnd = pTopWnd;
	else					m_pTopWnd = NULL;

	switch( dlgtype )
	{
		// price
		case CBaseData::Price :
			SetDlg_Price(texttype, p_pIChartOCX, pParent, pChildDlg);
			return;

		// volume
		case CBaseData::Volume :
			SetDlg_Volume(texttype, p_pIChartOCX, pParent, pChildDlg);
			return;

		// MovingAverage
		case CBaseData::MovingAverage : 
			SetDlg_VolumeMA(texttype, p_pIChartOCX, pParent, pChildDlg);
			break;

		// [가격이동평균] 설정창 변경
		case CBaseData::PriceMA: //PriceMA
			SetDlg_PriceMA(texttype, p_pIChartOCX, pParent, pChildDlg);
			return;

		// Sang-Woo, Cho		2005 / 11 / 10
		// 일목 균형표
		// Begin
		case CBaseData::GLANCEBALANCE:
			SetDlg_GlanceBalance( texttype, p_pIChartOCX, pParent, pChildDlg);
			return;
		// End

		// 삼선전환도, P&F
		case CBaseData::DT_SPECIAL_TYPE_A:
			SetDlg_Special_Type_A( texttype, p_pIChartOCX, pParent, pChildDlg );
			return;

		// Swing, Kagi
		case CBaseData::DT_SPECIAL_TYPE_B:
			SetDlg_Special_Type_B( texttype, p_pIChartOCX, pParent, pChildDlg );
			return;

		// Renko, 분산챠트
		case CBaseData::DT_SPECIAL_TYPE_C:
			SetDlg_Special_Type_C( texttype, p_pIChartOCX, pParent, pChildDlg );
			return;

		// 역시계 곡선
		case CBaseData::DT_SPECIAL_TYPE_D:
			SetDlg_Special_Type_D( texttype, p_pIChartOCX, pParent, pChildDlg );
			return;

		// Sang-Woo, Cho		2006 / 04 / 19
		// 대기매물 설정
		case CBaseData::DT_SPECIAL_TYPE_E:
			SetDlg_Special_Type_E( texttype, p_pIChartOCX, pParent, pChildDlg);
			return;

		// Binary Wave
		case CBaseData::DT_COMMON_TYPE_B:
			SetDlg_Common_Type_B( texttype, p_pIChartOCX, pParent, pChildDlg);
			return;

		default:
			SetDlg_Common_Type_A( texttype, p_pIChartOCX, pParent, pChildDlg);
			return;
	}
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 27
// Return void : 
// Param  const CString &strIndicatorName : 
// Param  CButton &First : 
// Param  CButton &second : 
// Comments		: 해당 지표의 Child Dlg타입을 가져온다. 그 Child Dlg타입에 따라 
//				: Child Dlg의 그룹박스에 타입에 맞는 텍스트를 입력한다.
//-----------------------------------------------------------------------------
void CMakeChildDlg::SetItemText( const CString &strIndicatorName, CButton &First, CButton &second )
{
	CBaseData::TABITEMTYPE texttype = CFindType::FindTabItemText( strIndicatorName );

// 2008.01.25 by LYH >>
	CString strSetting;
	strSetting.LoadString(IDS_STRING_SETTING);
// 2008.01.25 by LYH <<
	if( texttype == CBaseData::CONFIGURATION )
	{
// 2008.01.25 by LYH >>
		//First.SetWindowText( _T( "설정" ) );
		First.SetWindowText( strSetting );
// 2008.01.25 by LYH <<
		second.SetWindowText( _T("") );
	}

	// 설정, 기준선
	else if( texttype == CBaseData::CONFIGU_BASELINE )
	{
// 2008.01.25 by LYH >>
		//First.SetWindowText( _T("설정") );
		First.SetWindowText( strSetting );
		//second.SetWindowText( _T("기준선") );
		CString strBaseLine;
		strBaseLine.LoadString(IDS_BASE_LINE);
		second.SetWindowText( strBaseLine );
// 2008.01.25 by LYH <<
	}
	
	// 설정, 수평선
	else if( texttype == CBaseData::CONFIGU_HORIZONLINE )
	{
// 2008.01.25 by LYH >>
		//First.SetWindowText( _T("설정") );
		First.SetWindowText( strSetting );
		//second.SetWindowText( _T("수평선") );
		CString strHorzLine;
		strHorzLine.LoadString(IDS_HORIZONTAL_LINE);
		second.SetWindowText( strHorzLine );
// 2008.01.25 by LYH <<
	}

	// 설정, signal
	else if( texttype == CBaseData::CONFIGU_SIGNAL )
	{
// 2008.01.25 by LYH >>
		//First.SetWindowText( _T("설정") );
		First.SetWindowText( strSetting );
// 2008.01.25 by LYH <<
		second.SetWindowText( _T("Signal") );
	}
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 27
// Return void : 
// Param  CBaseData::TABITEMTYPE texttype : 
// Param  IGraphInfo *p_pIGraphInfo: 
// Param  CWnd *pParent : 
// Param  IChartManager *p_pIChartManager : 
// Comments		: 
//-----------------------------------------------------------------------------
void CMakeChildDlg::SetDlg_Price( CBaseData::TABITEMTYPE texttype, IChartOCX* p_pIChartOCX, CWnd *pParent, CBaseDialog *&pChildDlg)
{
	// 설정
	pChildDlg = new CSetting_Price( p_pIChartOCX, pParent, m_pTopWnd);
	
	// 가격차트에서 필요.
	pChildDlg->Create(IDD_PRICE, pParent);
}

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 27
// Return void : 
// Param  CBaseData::TABITEMTYPE texttype : 
// Param  IGraphInfo *p_pIGraphInfo: 
// Param  CWnd *pParent : 
// Comments		: 
//-----------------------------------------------------------------------------
void CMakeChildDlg::SetDlg_Volume( CBaseData::TABITEMTYPE texttype, IChartOCX* p_pIChartOCX, CWnd *pParent, CBaseDialog *&pChildDlg)
{
	// 설정
	pChildDlg =  new CSetting_Volume( p_pIChartOCX, pParent);
	pChildDlg->Create( IDD_VOLUME, pParent );	
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 27
// Return void : 
// Param  CBaseData::TABITEMTYPE texttype : 
// Param  IGraphInfo *p_pIGraphInfo: 
// Param  CWnd *pParent : 
// Comments		: 
//-----------------------------------------------------------------------------
void CMakeChildDlg::SetDlg_VolumeMA( CBaseData::TABITEMTYPE texttype, IChartOCX* p_pIChartOCX, CWnd *pParent, CBaseDialog *&pChildDlg)
{
	// 설정
	pChildDlg =  new CSetting_VolumeMA( p_pIChartOCX, pParent, m_pTopWnd);
	pChildDlg->Create( IDD_VOLUMEMA, pParent );	
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 27
// Return void : 
// Param  CBaseData::TABITEMTYPE texttype : 
// Param  IGraphInfo *p_pIGraphInfo: 
// Param  CWnd *pParent : 
// Comments		: 
//-----------------------------------------------------------------------------
void CMakeChildDlg::SetDlg_PriceMA( CBaseData::TABITEMTYPE texttype, IChartOCX* p_pIChartOCX, CWnd *pParent, CBaseDialog *&pChildDlg )
{
	// 설정
	pChildDlg =  new CSetting_PriceMA( p_pIChartOCX, pParent, m_pTopWnd);
	pChildDlg->Create( IDD_PRICEMA, pParent);	
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 11 / 10
// Return void : 
// Param  CBaseData::TABITEMTYPE texttype : 
// Param  IChartOCX* p_pIChartOCX : 
// Param  IGraphInfo *p_pIGraphInfo: 
// Param  CWnd *pParent : 
// Param  CBaseDialog*>* pChildDlg : 
// Param  IChartManager *p_pIChartManager ; : 
// Comments		: 
//-----------------------------------------------------------------------------
void CMakeChildDlg::SetDlg_GlanceBalance( CBaseData::TABITEMTYPE texttype, IChartOCX* p_pIChartOCX, CWnd *pParent, CBaseDialog *&pChildDlg)
{
	pChildDlg =  new CSetting_GlanceBalance( p_pIChartOCX, pParent, m_pTopWnd);
	pChildDlg->Create( IDD_GLANCEBALANCE, pParent );
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 9
// Return void : 
// Param  CBaseData::TABITEMTYPE texttype : 
// Param  IGraphInfo *p_pIGraphInfo: 
// Param  CWnd *pParent : 
// Param  CBaseDialog*>* pChildDlg : 
// Comments		: 
//-----------------------------------------------------------------------------
void CMakeChildDlg::SetDlg_Special_Type_A( CBaseData::TABITEMTYPE texttype, IChartOCX* p_pIChartOCX, CWnd *pParent, CBaseDialog *&pChildDlg)
{
	pChildDlg =  new CSetting_Special_Type_A( p_pIChartOCX, pParent, m_pTopWnd);
	pChildDlg->Create( IDD_SPECIAL_TYPE_A, pParent );
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 9
// Return void : 
// Param  CBaseData::TABITEMTYPE texttype : 
// Param  IGraphInfo *p_pIGraphInfo: 
// Param  CWnd *pParent : 
// Param  CBaseDialog*>* pChildDlg : 
// Comments		: 
//-----------------------------------------------------------------------------
void CMakeChildDlg::SetDlg_Special_Type_B( CBaseData::TABITEMTYPE texttype, IChartOCX* p_pIChartOCX, CWnd *pParent, CBaseDialog *&pChildDlg)
{
	pChildDlg =  new CSetting_Special_Type_B( p_pIChartOCX, pParent, m_pTopWnd);
	pChildDlg->Create( IDD_SPECIAL_TYPE_B, pParent );
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 23
// Return void : 
// Param  CBaseData::TABITEMTYPE texttype : 
// Param  IGraphInfo *p_pIGraphInfo: 
// Param  CWnd *pParent : 
// Param  CBaseDialog*>* pChildDlg : 
// Comments		: 
//-----------------------------------------------------------------------------
void CMakeChildDlg::SetDlg_Special_Type_C( CBaseData::TABITEMTYPE texttype, IChartOCX* p_pIChartOCX, CWnd *pParent, CBaseDialog *&pChildDlg)
{
	pChildDlg =  new CSetting_Special_Type_C( p_pIChartOCX, pParent, m_pTopWnd);
	pChildDlg->Create( IDD_SPECIAL_TYPE_C, pParent );
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 23
// Return void : 
// Param  CBaseData::TABITEMTYPE texttype : 
// Param  IGraphInfo *p_pIGraphInfo: 
// Param  CWnd *pParent : 
// Param  CBaseDialog*>* pChildDlg : 
// Comments		: 
//-----------------------------------------------------------------------------
void CMakeChildDlg::SetDlg_Special_Type_D( CBaseData::TABITEMTYPE texttype, IChartOCX* p_pIChartOCX, CWnd *pParent, CBaseDialog *&pChildDlg)
{
	pChildDlg =  new CSetting_Special_Type_D( p_pIChartOCX, pParent, m_pTopWnd);
	pChildDlg->Create( IDD_SPECIAL_TYPE_D, pParent );
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 4 / 19
// Return void : 
// Param  CBaseData::TABITEMTYPE texttype : 
// Param  IChartOCX* p_pIChartOCX : 
// Param  IGraphInfo *p_pIGraphInfo: 
// Param  CWnd *pParent : 
// Param  CBaseDialog*>* pChildDlg : 
// Comments		: 대기매물 지표의 설정 Dlg를 호출한다.
//-----------------------------------------------------------------------------
void CMakeChildDlg::SetDlg_Special_Type_E( CBaseData::TABITEMTYPE texttype, IChartOCX* p_pIChartOCX, CWnd *pParent, CBaseDialog *&pChildDlg)
{
	pChildDlg =  new CSetting_Special_Type_E( p_pIChartOCX, pParent, m_pTopWnd);
	pChildDlg->Create( IDD_SPECIAL_TYPE_E, pParent );
}


// Sang-Woo, Cho		2006 / 05 / 02
// BinaryWave 설정창을 분리함.
// Begin
void CMakeChildDlg::SetDlg_Common_Type_B( CBaseData::TABITEMTYPE texttype, IChartOCX* p_pIChartOCX, CWnd *pParent, CBaseDialog *&pChildDlg)
{
	//sy test
	pChildDlg =  new CSetting_Common_Type_B( p_pIChartOCX, pParent, m_pTopWnd);
	pChildDlg->Create( IDD_COMMON_TYPE_B, pParent );
}
// End

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 23
// Return void : 
// Param  CBaseData::TABITEMTYPE texttype : 
// Param  IGraphInfo *p_pIGraphInfo: 
// Param  CWnd *pParent : 
// Param  CBaseDialog*>* pChildDlg : 
// Comments		: 
//-----------------------------------------------------------------------------
void CMakeChildDlg::SetDlg_Common_Type_A( CBaseData::TABITEMTYPE texttype, IChartOCX *p_pIChartOCX, CWnd *pParent, CBaseDialog *&pChildDlg)
{
	//sy test
	pChildDlg =  new CSetting_Common_Type_A( p_pIChartOCX, pParent, m_pTopWnd);
	pChildDlg->Create( IDD_COMMON_TYPE_A, pParent );
}

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 27
// Return void : 
// Param  CBaseData::TABITEMTYPE texttype : 
// Param  IGraphInfo *p_pIGraphInfo: 
// Param  CWnd *pParent : 
// Comments		: 
//-----------------------------------------------------------------------------
void CMakeChildDlg::SetDlg_Variables( CBaseData::TABITEMTYPE texttype, CWnd *pParent, CBaseDialog *&pChildDlg )
{
	// 설정, 기준선, 수평선
//	pChildDlg =  new CSetting_Common_Variables( p_pIGraphInfo) );
//	pChildDlg->Create( IDD_COMMON_VARIABLES, pParent );
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 27
// Return void : 
// Param  CBaseData::TABITEMTYPE texttype : 
// Param  IGraphInfo *p_pIGraphInfo: 
// Param  CWnd *pParent : 
// Comments		: 
//-----------------------------------------------------------------------------
void CMakeChildDlg::SetDlg_BaseLine( CBaseData::TABITEMTYPE texttype, CWnd *pParent, CBaseDialog *&pChildDlg )
{
	// 설정, 기준선, 수평선
//	pChildDlg =  new CSetting_Common_BaseLine( p_pIGraphInfo) );
//	pChildDlg->Create( IDD_COMMON_BASELINE, pParent );
}