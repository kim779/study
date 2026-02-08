// ChartStyleDataManagerAddInImp.cpp: implementation of the CChartStyleDataManagerAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartEnvSetupDlgAddIn.h"
#include "ChartStyleDataManagerAddInImp.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"	// for ML_LANGUAGE_RES
#include "EnvironmentData.h"						// for CStyleData

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChartStyleDataManagerAddInImp::CChartStyleDataManagerAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase)
	: CChartAddInBase( p_pIChartOCX, p_pIAddInDllBase)
{
// 2008.01.25 by LYH >>
	m_hOcxWnd = NULL;
	if( p_pIChartOCX) m_hOcxWnd = p_pIChartOCX->GetOcxHwnd();
// 2008.01.25 by LYH <<

	m_styleDataManager.Initialize( m_pIChartOCX, TRUE);
}

CChartStyleDataManagerAddInImp::~CChartStyleDataManagerAddInImp()
{

}

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
CString CChartStyleDataManagerAddInImp::m_strAddInItemName = "STYLE_DATA";

//////////////////////////////////////////////////////////////////////
// Define AddIn Event Map
//////////////////////////////////////////////////////////////////////
BEGIN_ADDIN_EVENT_MAP( CChartStyleDataManagerAddInImp)
	ONADDINEVENT( OnAddInToolCommand_EAICommandType_long)
END_ADDIN_EVENT_MAP()

//////////////////////////////////////////////////////////////////////
// [05/07/08] AddIn 전용 Interface를 제공한다.
//		*. 주의. 반듯이 전용 Interface Pointer로 Converting후 IUnknown의 Pointer로 변환하여 전달한다.
//		*. 또한 다음의 _IWRAPPER_IUNKNOWN_INTERFACE도 정의한다.
//////////////////////////////////////////////////////////////////////
IUnknown *CChartStyleDataManagerAddInImp::GetAddInCustomInterface( void)
{
	IUnknown *pIUnknown = ( IUnknown *)( IStyleManager *)this;
	pIUnknown->AddRef();
	return pIUnknown;
}


//////////////////////////////////////////////////////////////////////
// (2009/6/17 - Seung-Won, Bae) for Custom Interface
//////////////////////////////////////////////////////////////////////
int CChartStyleDataManagerAddInImp::GetStyleCount( void)
{
	m_styleDataManager.Initialize( m_pIChartOCX);
	return m_styleDataManager.GetDataCount();
}
const char *CChartStyleDataManagerAddInImp::GetStyleName( int p_nStyleIndex)
{
	static CString strResult;
	strResult.Empty();

	m_styleDataManager.Initialize( m_pIChartOCX);
	if( m_styleDataManager.GetDataCount() <= p_nStyleIndex) return NULL;
	CStyleData *pStyleData = m_styleDataManager.GetStyleData( p_nStyleIndex);
	if( !pStyleData) return strResult;

	strResult = pStyleData->GetStyleName();
	return strResult;
}
void CChartStyleDataManagerAddInImp::SetStyleToChart( int p_nStyleIndex)
{
	m_styleDataManager.Initialize( m_pIChartOCX);
	if( m_styleDataManager.GetDataCount() <= p_nStyleIndex) return;
	CStyleData *pStyleData = m_styleDataManager.GetStyleData( p_nStyleIndex);
	if( !pStyleData) return;
	m_styleDataManager.SetStyleDataToOCX( *pStyleData, m_pIChartOCX);
}


//////////////////////////////////////////////////////////////////////
// Overrides
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
//////////////////////////////////////////////////////////////////////

BOOL CChartStyleDataManagerAddInImp::OnAddInToolCommand( const EAICommandType p_eCommandID, const long p_lParam)
{
	if( EAI_STYLE_SET_STYLE != p_eCommandID || p_lParam < 0) return FALSE;

	if( m_styleDataManager.GetDataCount( TRUE) <= p_lParam) return FALSE;

	CStyleData *pStyleData = m_styleDataManager.GetStyleData( p_lParam, TRUE);
	if( !pStyleData) return FALSE;

	m_styleDataManager.SetStyleDataToOCX( *pStyleData, m_pIChartOCX);

	return TRUE;
}

// (2009/9/21 - Seung-Won, Bae) Show Style Type on Price, Volume and MAs.
void CChartStyleDataManagerAddInImp::LoadCurrentStyle( void)
{
	ML_SET_LANGUAGE_RES();
	m_styleDataManager.CreateDummyUserStyle( m_pIChartOCX);	
}
void CChartStyleDataManagerAddInImp::DrawSampleImage( HDC p_hDC, int p_nLeft, int p_nTop, int p_nRight, int p_nBottom, int p_nStyleIndex, HGDIOBJ p_hFont)
{
	CDC *pDC = CDC::FromHandle( p_hDC);
	CFont *pFont = CFont::FromHandle( ( HFONT)p_hFont);
	CStyleData *pStyleData = m_styleDataManager.GetStyleData( p_nStyleIndex);
	if( !pStyleData) return;
	CStyleData tempStyleData( *pStyleData);
	m_styleDataManager.GetStyleDataFromOCX( tempStyleData, m_pIChartOCX, FALSE);
	m_styleDataManager.DrawSampleImage( pDC, CRect( p_nLeft, p_nTop, p_nRight, p_nBottom), &tempStyleData, ( CChartInfo::CHART_MODE)m_pIChartOCX->GetChartMode(), pFont, m_hOcxWnd, 20);
}
void CChartStyleDataManagerAddInImp::UpdateIndexStyle( const char *p_szGraphName, int p_nStyleIndex)
{
	CStyleData *pStyleData = m_styleDataManager.GetStyleData( p_nStyleIndex);
	if( !pStyleData) return;
	m_styleDataManager.SetStyleDataToIndex( *pStyleData, m_pIChartOCX, p_szGraphName);
}
