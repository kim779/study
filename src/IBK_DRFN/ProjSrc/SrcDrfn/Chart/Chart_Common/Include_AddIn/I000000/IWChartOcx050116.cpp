// IWChartOcx050116.cpp: implementation of the CIWChartOcx050116 class.
//
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// *. 주의!	050116 Version의 AddIn에서 이용되는 OCX측 Interface의 Wrapper로 기능 추가만 가능합니다.
//		기능 삭제와 Type 변경은 허용하지 않습니다.
//		허용되지 않는 편집은 050116 Version의 모든 AddIn Module과 관련 Site OCX가
//			함께 수정되어 내려가는 것이 전제되어야 합니다.
// *. 050116 Version Info
//		AddIn Module				Site			Manager
//			ChartCustomAddIn			GZone			배승원
//			ChartPatternAddIn			GZone			정서정
//			ChartGuideLine				GZone			남상윤
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IWChartOcx050116.h"

#include MChartOcxInclude				// for MChartOcx
#include "IWPacketManager050116.h"		// for CIWPacketManager050116
#include "IWChartManager050116.h"		// for CIWChartManager050116
#include "IWGraphInfoManager050116.h"	// for CIWGraphInfoManager050116

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// (2004.10.08, 배승원)	Chart OCX의 Pointer를 관리한다.
//		실제로 각 OCX의 Ctrl Class이며, StdAfx.h에 Macro로 정의하여 이용한다.
CIWChartOcx050116::CIWChartOcx050116( MChartOcx *p_pChartOcx)
{
	m_pChartOcx = p_pChartOcx;
	if( !m_pChartOcx)
	{
		AfxMessageBox( "CIWChartOcx050116 Null Wrapping Error!");
		return;
	}
}

CIWChartOcx050116::~CIWChartOcx050116()
{

}

//////////////////////////////////////////////////////////////////////
// (2004.10.19, 배승원) Windows 관련 Interface를 제공한다.
//////////////////////////////////////////////////////////////////////

// [04.10.11] ReDrawing을 위한 Interface를 제공한다.
void CIWChartOcx050116::InvalidateControl( void)
{
	if( m_pChartOcx) m_pChartOcx->InvalidateControl();
}

// [04/10/11] ReDrawing을 위한 Interface를 제공한다.
HWND CIWChartOcx050116::GetOcxHwnd( void)
{
	if( m_pChartOcx) return m_pChartOcx->GetSafeHwnd();
	return NULL;
}

// [04/11/02] Counter Capturing Interface를 제공한다.
HWND CIWChartOcx050116::AddSetCapture( void)
{
	if( m_pChartOcx)
	{
		CWnd *pWnd = m_pChartOcx->AddSetCapture();
		if( pWnd) return pWnd->GetSafeHwnd();
	}
	return NULL;
}

// [04/11/02] Counter Capturing Interface를 제공한다.
BOOL CIWChartOcx050116::RemoveSetCapture( void)
{
	if( m_pChartOcx) return m_pChartOcx->RemoveSetCapture();
	return FALSE;
}

//////////////////////////////////////////////////////////////////////
// (2004.10.18, 배승원) 환경 정보를 조회하는 Interface를 제공한다.
//////////////////////////////////////////////////////////////////////

// [04.10.18] 사용자 Folder를 구하는 Interface를 제공한다.
BOOL CIWChartOcx050116::GetUserFilePath( CString &p_strUserFilePath)
{
	if( m_pChartOcx)
	{
		p_strUserFilePath = m_pChartOcx->GetUserFilePath();
		return !p_strUserFilePath.IsEmpty();
	}
	return FALSE;
}

// [04.10.25] Chart 전용 Font를 제공한다.
CFont *CIWChartOcx050116::GetChartFont( void)
{
	if( !m_pChartOcx) return NULL;
	return m_pChartOcx->GetChartFont();
}

//////////////////////////////////////////////////////////////////////
// (2004.10.11, 배승원) Chart의 현 상태를 확인하는 Interface를 제공한다.
//////////////////////////////////////////////////////////////////////

// [04.10.11] 특수차트의 경우인지 확인하는 Interface를 제공한다. (역시계곡선/삼선전환도/P&F)
BOOL CIWChartOcx050116::IsOnSpecialChart( void)
{
	if( m_pChartOcx) return m_pChartOcx->IsRunningOneChart();
	return FALSE;
}

//////////////////////////////////////////////////////////////////////
// (2005.01.20, 배승원) ToolBar에 다음 Item이 있는지 확인하는 Interface를 제공한다.
//////////////////////////////////////////////////////////////////////

BOOL CIWChartOcx050116::HasToolItem( CToolOptionInfo::TOOLOPTION p_eToolItem)
{
	if( m_pChartOcx) return m_pChartOcx->HasToolItem( p_eToolItem);
	return FALSE;
}

//////////////////////////////////////////////////////////////////////
// (2004.10.11, 배승원) Data Index 및 Position 관련 Interface
//////////////////////////////////////////////////////////////////////

// [04/10/11] 마지막 실 Data의 Index를 구하는 Interface를 제공한다.
int CIWChartOcx050116::GetLastRealDataIndex( void)
{
	if( m_pChartOcx) return m_pChartOcx->GetDataCountForToolBar();
	return -1;
}

// [04/10/11] ScrollBar상의 마지막 Scroll Index를 구하는 Interface를 제공한다. (여백포함)
BOOL CIWChartOcx050116::GetMaxScrollRange( int &p_nMaxScrollRange)
{
	if( m_pChartOcx) return m_pChartOcx->GetScrollRange( p_nMaxScrollRange);
	return FALSE;
}

// [04/10/11] Chart의 현재 보이는 View 영역의 첫 Data Index와 끝 Data Index를 구하거나, 설정하는 Interface를 제공한다.
BOOL CIWChartOcx050116::GetViewStartEndIndex( int &p_nStartIndex, int &p_nEndIndex)
{
	if( m_pChartOcx) return m_pChartOcx->GetDataStartEnd( p_nStartIndex, p_nEndIndex);
	return FALSE;
}
BOOL CIWChartOcx050116::SetViewStartEndIndex( int p_nStartIndex, int p_nEndIndex)
{
	if( m_pChartOcx) return m_pChartOcx->SetDataStartEnd( p_nStartIndex, p_nEndIndex);
	return FALSE;
}

// [04/10/27] Data 좌표와 Screen 좌표를 변환하는 Interface를 제공한다.
int CIWChartOcx050116::ConvertDataToYPosition( double p_dData, double p_dDataMax, double p_dDataMin, int p_nYPositionMax, int p_nYPositionMin, BOOL p_bLog, BOOL p_bReverse)
{
	if( m_pChartOcx) return m_pChartOcx->ConvertDataToYPosition( p_dData, p_dDataMax, p_dDataMin, p_nYPositionMax, p_nYPositionMin, p_bLog, p_bReverse);
	return 0;
}
double CIWChartOcx050116::ConvertYPositionToData( int p_nYPosition, int p_nYPositionMax, int p_nYPositionMin, double p_dDataMax, double p_dDataMin, BOOL p_bLog, BOOL p_bReverse)
{
	if( m_pChartOcx) return m_pChartOcx->ConvertYPositionToData( p_nYPosition, p_nYPositionMax, p_nYPositionMin, p_dDataMax, p_dDataMin, p_bLog, p_bReverse);
	return 0;
}

// [04/11/09] Scroll/Zoom을 쉽게 처리하는 Interface를 제공한다.
BOOL CIWChartOcx050116::ScrollTo( int p_nScrollValue, BOOL p_bRelative)
{
	if( m_pChartOcx) return m_pChartOcx->ScrollTo( p_nScrollValue, p_bRelative);
	return FALSE;
}
BOOL CIWChartOcx050116::ZoomTo( int p_nZoomValue, BOOL p_bRelative)
{
	if( m_pChartOcx) return m_pChartOcx->ZoomTo( p_nZoomValue, p_bRelative);
	return FALSE;
}

//////////////////////////////////////////////////////////////////////
// [04.10.11] 공유 관련 Interface
//////////////////////////////////////////////////////////////////////

// [04.10.11] 주어진 Global 공유가 설정되어 있는지 확인하는 Interface를 제공한다.
BOOL CIWChartOcx050116::CheckGSharedSetting( const char *p_szGSharedName, BOOL p_bStart, BOOL p_bGet, BOOL p_bSet)
{
	if( !m_pChartOcx) return FALSE;
	ST_GSHAREDFORMAT SDF = m_pChartOcx->FindGSharedDataInfo( p_szGSharedName);
	if( SDF.m_szSharedName.IsEmpty()) return FALSE;
	return SDF.m_bStart && p_bStart || SDF.m_bGet && p_bGet || SDF.m_bSet && p_bSet;
}

// [04.10.11] 주어진 Global 공유에 공유를 발생시키는 Interface를 제공한다.
BOOL CIWChartOcx050116::FireGSharedData( const char *p_szGSharedName, const char *p_szGSharedData)
{
	if( !m_pChartOcx) return FALSE;
	return m_pChartOcx->FireGSharedData( p_szGSharedName, p_szGSharedData);
}

//////////////////////////////////////////////////////////////////////
// (2004.10.11, 배승원) OCX로부터 주요 Manager의 Interface를 구하여 관리한다.
//////////////////////////////////////////////////////////////////////

// (2004.10.11, 배승원) OCX로부터 주요 Manager의 Interface를 구하여 관리한다.
IPacketManager050116 *CIWChartOcx050116::GetIPacketManager050116( void)
{
	if( !m_pChartOcx) return NULL;
	CPacketList *pPacketList = m_pChartOcx->GetPacketList();
	if( !pPacketList) return NULL;
	IPacketManager050116 *pIPacketManager050116 = new CIWPacketManager050116( pPacketList);
	pIPacketManager050116->AddRef();
	return pIPacketManager050116;
}
IChartManager050116 *CIWChartOcx050116::GetIChartManager050116( void)
{
	if( !m_pChartOcx) return NULL;
	CMainBlock *pMainBlock = m_pChartOcx->GetMainBlock();
	if( !pMainBlock) return NULL;
	IChartManager050116 *pIChartManager050116 = new CIWChartManager050116( pMainBlock);
	pIChartManager050116->AddRef();
	return pIChartManager050116;
}
IGraphInfoManager050116 *CIWChartOcx050116::GetIGraphInfoManager050116( void)
{
	if( !m_pChartOcx) return NULL;
	CIndicatorList *pIndicatorList = m_pChartOcx->GetIndicatorList();
	if( !pIndicatorList) return NULL;
	IGraphInfoManager050116 *pIGraphInfoManager050116 = new CIWGraphInfoManager050116( pIndicatorList);
	pIGraphInfoManager050116->AddRef();
	return pIGraphInfoManager050116;
}

//////////////////////////////////////////////////////////////////////
// (2004.10.19, 배승원) Image 저장 및 출력을 위한 Drawing Interface를 제공한다.
//////////////////////////////////////////////////////////////////////

// (2004.10.19, 배승원) Image 저장 및 출력을 위한 Drawing Interface를 제공한다.
void CIWChartOcx050116::DrawForImage( CDC *p_pDC, CRect p_rctSource, BOOL p_bUseScreenColor)
{
	if( !m_pChartOcx) return;
	CBitmap bufferBitmap;
	m_pChartOcx->OnDrawForPrint( *p_pDC, bufferBitmap, p_rctSource, p_bUseScreenColor);
}

//////////////////////////////////////////////////////////////////////
// (2004.10.25, 배승원) Cursor 관련 Interface를 제공한다.
//////////////////////////////////////////////////////////////////////

// [04/10/25] 새로운 Cursor를 설정하면서, 현재 설정된 Cursor를 확인한다.
HCURSOR CIWChartOcx050116::SetCursorHandle( HCURSOR p_hNewCursor)
{
	if( !m_pChartOcx) return NULL;
	return m_pChartOcx->SetCursorHandle( p_hNewCursor);
}

// [04/10/26] 현재 설정된 Cursor를 확인한다.
HCURSOR CIWChartOcx050116::GetCursorHandle( void)
{
	if( !m_pChartOcx) return NULL;
	return m_pChartOcx->GetCursorHandle();
}

//////////////////////////////////////////////////////////////////////
// (2005.01.13, 배승원) 차트의 전체보기 Mode 여부를 확인 및 설정하는 Interface를 제공한다.
//////////////////////////////////////////////////////////////////////

BOOL CIWChartOcx050116::CheckFullView( void) const
{
	if( !m_pChartOcx) return FALSE;
	return m_pChartOcx->IsWholeView();
}

void CIWChartOcx050116::SetFullView( BOOL p_bFullView)
{
	if( !m_pChartOcx) return;
	if( ( p_bFullView && m_pChartOcx->IsWholeView())
		|| ( !p_bFullView && !m_pChartOcx->IsWholeView())) return;

	m_pChartOcx->SetWholeView( p_bFullView);
	m_pChartOcx->InvalidateControl();
}

//////////////////////////////////////////////////////////////////////
// 050116 Version, 전용 Interface
//		AddIn Module				Site			Manager
//			ChartCustomAddIn			GZone			배승원
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Temp Interface I : DLL Container 연계 Interface
//////////////////////////////////////////////////////////////////////

// [04.10.11] DLL Container에게 Message하는 Interface를 제공한다.
LRESULT CIWChartOcx050116::SendEventMsgToDLL( UINT message, WPARAM wParam, LPARAM lParam)
{
	if( m_pChartOcx) m_pChartOcx->SendEventMsgToDLL( message, wParam, lParam);
	return 0;
}
