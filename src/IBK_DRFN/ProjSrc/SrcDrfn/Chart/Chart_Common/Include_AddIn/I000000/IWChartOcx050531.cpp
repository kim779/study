// (2005/8/10 - Seung-Won, Bae) Not used in Hanwha
// IWChartOcx050531.cpp: implementation of the CIWChartOcx050531 class.
//
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// *. 주의!	050531 Version의 AddIn에서 이용되는 OCX측 Interface의 Wrapper로 기능 추가만 가능합니다.
//		기능 삭제와 Type 변경은 허용하지 않습니다.
//		허용되지 않는 편집은 050531 Version의 모든 AddIn Module과 관련 Site OCX가
//			함께 수정되어 내려가는 것이 전제되어야 합니다.
// *. 050531 Version Info
//		AddIn Module				Site			Manager
//		ChartCustomAddIn			한화			배승원
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IWChartOcx050531.h"

#include MChartOcxInclude				// for MChartOcx
#include "IWPacketManager050531.h"		// for CIWPacketManager050531
#include "IWChartManager050531.h"		// for CIWChartManager050531
#include "IWGraphInfoManager050531.h"	// for CIWGraphInfoManager050531

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
CIWChartOcx050531::CIWChartOcx050531( MChartOcx *p_pChartOcx)
{
	m_pChartOcx = p_pChartOcx;
	if( !m_pChartOcx)
	{
		AfxMessageBox( "CIWChartOcx050531 Null Wrapping Error!");
		return;
	}
}

CIWChartOcx050531::~CIWChartOcx050531()
{

}

//////////////////////////////////////////////////////////////////////
// (2004.10.19, 배승원) Windows 관련 Interface를 제공한다.
//////////////////////////////////////////////////////////////////////

// [04.10.11] ReDrawing을 위한 Interface를 제공한다.
void CIWChartOcx050531::InvalidateControl( void)
{
	if( m_pChartOcx) m_pChartOcx->InvalidateControl();
}

// [04/10/11] ReDrawing을 위한 Interface를 제공한다.
HWND CIWChartOcx050531::GetOcxHwnd( void)
{
	if( m_pChartOcx) return m_pChartOcx->GetSafeHwnd();
	return NULL;
}

// [04/11/02] Counter Capturing Interface를 제공한다.
HWND CIWChartOcx050531::AddSetCapture( void)
{
	if( m_pChartOcx)
	{
		CWnd *pWnd = m_pChartOcx->AddSetCapture();
		if( pWnd) return pWnd->GetSafeHwnd();
	}
	return NULL;
}

// [04/11/02] Counter Capturing Interface를 제공한다.
BOOL CIWChartOcx050531::RemoveSetCapture( void)
{
	if( m_pChartOcx) return m_pChartOcx->RemoveSetCapture();
	return FALSE;
}

//////////////////////////////////////////////////////////////////////
// (2004.10.18, 배승원) 환경 정보를 조회하는 Interface를 제공한다.
//////////////////////////////////////////////////////////////////////

// [04.10.18] 사용자 Folder를 구하는 Interface를 제공한다.
BOOL CIWChartOcx050531::GetUserFilePath( CString &p_strUserFilePath)
{
	if( m_pChartOcx)
	{
		p_strUserFilePath = m_pChartOcx->GetUserFilePath();
		return !p_strUserFilePath.IsEmpty();
	}
	return FALSE;
}

// [04.10.25] Chart 전용 Font를 제공한다.
CFont *CIWChartOcx050531::GetChartFont( void)
{
	if( !m_pChartOcx) return NULL;
	return m_pChartOcx->GetChartFont();
}

//////////////////////////////////////////////////////////////////////
// (2004.10.11, 배승원) Chart의 현 상태를 확인하는 Interface를 제공한다.
//////////////////////////////////////////////////////////////////////

// [04.10.11] 특수차트의 경우인지 확인하는 Interface를 제공한다. (역시계곡선/삼선전환도/P&F)
BOOL CIWChartOcx050531::IsOnSpecialChart( void)
{
	if( m_pChartOcx) return m_pChartOcx->IsRunningOneChart();
	return FALSE;
}

//////////////////////////////////////////////////////////////////////
// (2005.01.20, 배승원) ToolBar에 다음 Item이 있는지 확인하는 Interface를 제공한다.
//////////////////////////////////////////////////////////////////////

BOOL CIWChartOcx050531::HasToolItem( CToolOptionInfo::TOOLOPTION p_eToolItem)
{
	if( m_pChartOcx) return m_pChartOcx->HasToolItem( p_eToolItem);
	return FALSE;
}

//////////////////////////////////////////////////////////////////////
// (2004.10.11, 배승원) Data Index 및 Position 관련 Interface
//////////////////////////////////////////////////////////////////////

// [04/10/11] 마지막 실 Data의 Index를 구하는 Interface를 제공한다.
int CIWChartOcx050531::GetLastRealDataIndex( void)
{
	if( m_pChartOcx) return m_pChartOcx->GetDataCountForToolBar();
	return -1;
}

// [04/10/11] ScrollBar상의 마지막 Scroll Index를 구하는 Interface를 제공한다. (여백포함)
BOOL CIWChartOcx050531::GetMaxScrollRange( int &p_nMaxScrollRange)
{
	if( m_pChartOcx) return m_pChartOcx->GetScrollRange( p_nMaxScrollRange);
	return FALSE;
}

// [04/10/11] Chart의 현재 보이는 View 영역의 첫 Data Index와 끝 Data Index를 구하거나, 설정하는 Interface를 제공한다.
BOOL CIWChartOcx050531::GetViewStartEndIndex( int &p_nStartIndex, int &p_nEndIndex)
{
	if( m_pChartOcx) return m_pChartOcx->GetDataStartEnd( p_nStartIndex, p_nEndIndex);
	return FALSE;
}
BOOL CIWChartOcx050531::SetViewStartEndIndex( int p_nStartIndex, int p_nEndIndex)
{
	if( m_pChartOcx) return m_pChartOcx->SetDataStartEnd( p_nStartIndex, p_nEndIndex);
	return FALSE;
}

// [04/10/27] Data 좌표와 Screen 좌표를 변환하는 Interface를 제공한다.
int CIWChartOcx050531::ConvertDataToYPosition( double p_dData, double p_dDataMax, double p_dDataMin, int p_nYPositionMax, int p_nYPositionMin, BOOL p_bLog, BOOL p_bReverse)
{
	if( m_pChartOcx) return m_pChartOcx->ConvertDataToYPosition( p_dData, p_dDataMax, p_dDataMin, p_nYPositionMax, p_nYPositionMin, p_bLog, p_bReverse);
	return 0;
}
double CIWChartOcx050531::ConvertYPositionToData( int p_nYPosition, int p_nYPositionMax, int p_nYPositionMin, double p_dDataMax, double p_dDataMin, BOOL p_bLog, BOOL p_bReverse)
{
	if( m_pChartOcx) return m_pChartOcx->ConvertYPositionToData( p_nYPosition, p_nYPositionMax, p_nYPositionMin, p_dDataMax, p_dDataMin, p_bLog, p_bReverse);
	return 0;
}

// [04/11/09] Scroll/Zoom을 쉽게 처리하는 Interface를 제공한다.
BOOL CIWChartOcx050531::ScrollTo( int p_nScrollValue, BOOL p_bRelative)
{
	if( m_pChartOcx) return m_pChartOcx->ScrollTo( p_nScrollValue, p_bRelative);
	return FALSE;
}
BOOL CIWChartOcx050531::ZoomTo( int p_nZoomValue, BOOL p_bRelative)
{
	if( m_pChartOcx) return m_pChartOcx->ZoomTo( p_nZoomValue, p_bRelative);
	return FALSE;
}

//////////////////////////////////////////////////////////////////////
// [04.10.11] 공유 관련 Interface
//////////////////////////////////////////////////////////////////////

// [04.10.11] 주어진 Global 공유가 설정되어 있는지 확인하는 Interface를 제공한다.
BOOL CIWChartOcx050531::CheckGSharedSetting( const char *p_szGSharedName, BOOL p_bStart, BOOL p_bGet, BOOL p_bSet)
{
	if( !m_pChartOcx) return FALSE;
	ST_GSHAREDFORMAT SDF = m_pChartOcx->FindGSharedDataInfo( p_szGSharedName);
	if( SDF.m_szSharedName.IsEmpty()) return FALSE;
	return SDF.m_bStart && p_bStart || SDF.m_bGet && p_bGet || SDF.m_bSet && p_bSet;
}

// [04.10.11] 주어진 Global 공유에 공유를 발생시키는 Interface를 제공한다.
BOOL CIWChartOcx050531::FireGSharedData( const char *p_szGSharedName, const char *p_szGSharedData)
{
	if( !m_pChartOcx) return FALSE;
	return m_pChartOcx->FireGSharedData( p_szGSharedName, p_szGSharedData);
}

// (2005/8/10 - Seung-Won, Bae) used in Only Hanwha
BOOL CIWChartOcx050531::GetShareInfoItem( const char *p_szShareName, int &p_nShareType, int &p_nShareArea, long &p_lCodeFilter)
{
	return FALSE;
}
BOOL CIWChartOcx050531::FireSharedData( const char *p_szShareName, const char *p_szSharedData)
{
	return FALSE;
}

//////////////////////////////////////////////////////////////////////
// (2004.10.11, 배승원) OCX로부터 주요 Manager의 Interface를 구하여 관리한다.
//////////////////////////////////////////////////////////////////////

// (2004.10.11, 배승원) OCX로부터 주요 Manager의 Interface를 구하여 관리한다.
IPacketManager050531 *CIWChartOcx050531::GetIPacketManager050531( void)
{
	if( !m_pChartOcx) return NULL;
	CPacketList *pPacketList = m_pChartOcx->GetPacketList();
	if( !pPacketList) return NULL;
	IPacketManager050531 *pIPacketManager050531 = new CIWPacketManager050531( pPacketList);
	pIPacketManager050531->AddRef();
	return pIPacketManager050531;
}
IChartManager050531 *CIWChartOcx050531::GetIChartManager050531( void)
{
	if( !m_pChartOcx) return NULL;
	CMainBlock *pMainBlock = m_pChartOcx->GetMainBlock();
	if( !pMainBlock) return NULL;
	IChartManager050531 *pIChartManager050531 = new CIWChartManager050531( pMainBlock);
	pIChartManager050531->AddRef();
	return pIChartManager050531;
}
IGraphInfoManager050531 *CIWChartOcx050531::GetIGraphInfoManager050531( void)
{
	if( !m_pChartOcx) return NULL;
	CIndicatorList *pIndicatorList = m_pChartOcx->GetIndicatorList();
	if( !pIndicatorList) return NULL;
	IGraphInfoManager050531 *pIGraphInfoManager050531 = new CIWGraphInfoManager050531( pIndicatorList);
	pIGraphInfoManager050531->AddRef();
	return pIGraphInfoManager050531;
}

//////////////////////////////////////////////////////////////////////
// (2004.10.19, 배승원) Image 저장 및 출력을 위한 Drawing Interface를 제공한다.
//////////////////////////////////////////////////////////////////////

// (2004.10.19, 배승원) Image 저장 및 출력을 위한 Drawing Interface를 제공한다.
void CIWChartOcx050531::DrawForImage( CDC *p_pDC, CRect p_rctSource, BOOL p_bUseScreenColor)
{
	if( !m_pChartOcx) return;
	CBitmap bufferBitmap;
	m_pChartOcx->OnDrawForPrint( *p_pDC, bufferBitmap, p_rctSource, p_bUseScreenColor);
}

//////////////////////////////////////////////////////////////////////
// (2004.10.25, 배승원) Cursor 관련 Interface를 제공한다.
//////////////////////////////////////////////////////////////////////

// [04/10/25] 새로운 Cursor를 설정하면서, 현재 설정된 Cursor를 확인한다.
HCURSOR CIWChartOcx050531::SetCursorHandle( HCURSOR p_hNewCursor)
{
	if( !m_pChartOcx) return NULL;
	return m_pChartOcx->SetCursorHandle( p_hNewCursor);
}

// [04/10/26] 현재 설정된 Cursor를 확인한다.
HCURSOR CIWChartOcx050531::GetCursorHandle( void)
{
	if( !m_pChartOcx) return NULL;
	return m_pChartOcx->GetCursorHandle();
}

//////////////////////////////////////////////////////////////////////
// (2005.01.13, 배승원) 차트의 전체보기 Mode 여부를 확인 및 설정하는 Interface를 제공한다.
//////////////////////////////////////////////////////////////////////

BOOL CIWChartOcx050531::CheckFullView( void) const
{
	if( !m_pChartOcx) return FALSE;
	return m_pChartOcx->IsWholeView();
}

void CIWChartOcx050531::SetFullView( BOOL p_bFullView)
{
	if( !m_pChartOcx) return;
	if( ( p_bFullView && m_pChartOcx->IsWholeView())
		|| ( !p_bFullView && !m_pChartOcx->IsWholeView())) return;

	m_pChartOcx->SetWholeView( p_bFullView);
	m_pChartOcx->InvalidateControl();
}

//////////////////////////////////////////////////////////////////////
// 종찬(05/06/01) 그래프의 추가,삭제,변경 관련 Interface를 제공한다.
//////////////////////////////////////////////////////////////////////
/*-----------------------------------------------------------------------------------------
 - Function    :  AddPacket
 - Created at  :  2005-06-01   10:12
 - Author      :  최종찬
 - Description :  패킷을 추가한다.
 -----------------------------------------------------------------------------------------*/
void CIWChartOcx050531::AddPacket(const char* pcszPacketOption)
{
	if(!m_pChartOcx)
		return;
	m_pChartOcx->AddPacketAdapter(pcszPacketOption);
}

/*-----------------------------------------------------------------------------------------
 - Function    :  AddGraph
 - Created at  :  2005-06-01   13:35
 - Author      :  최종찬
 - Description :  지표를 추가를 한다.
 -----------------------------------------------------------------------------------------*/
void CIWChartOcx050531::AddGraph(short nRow, short nColumn, short nVertScaleGroup, const char* szGraphOption, BOOL bCreateBlock, BOOL bCreateVertScaleGroup, BOOL bUseAutoScale)
{
	if(!m_pChartOcx)
		return;
	m_pChartOcx->AddGraphAdapter(nRow, nColumn, nVertScaleGroup, szGraphOption, bCreateBlock, bCreateVertScaleGroup, bUseAutoScale);
	return;
}

/*-----------------------------------------------------------------------------------------
 - Function    :  ChangeSubGraphData
 - Created at  :  2005-06-01   16:45
 - Author      :  최종찬
 - Description :  지표들의 그래프 속성(색, 두께, 유형)을 변경한다.
 -----------------------------------------------------------------------------------------*/
bool CIWChartOcx050531::ChangeSubGraphData(const char* pcszIndicatorName, const char* pcszSubGraphName, COLORREF clrLineColor, int nThickness, int nDrawStyle)
{
	if(!m_pChartOcx)
		return false;
	return m_pChartOcx->ChangeSubGraphData(pcszIndicatorName, pcszSubGraphName, clrLineColor, nThickness, nDrawStyle);
}


/*-----------------------------------------------------------------------------------------
 - Function    :  SetStartPeriodOfPacket
 - Created at  :  2005-06-07   15:55
 - Author      :  최종찬
 - Description :  Packet의 시작일자를 지정한다.
 -----------------------------------------------------------------------------------------*/
bool CIWChartOcx050531::SetStartPeriodOfPacket(const char* pcszPacketName, long lStartPeriod, BOOL bAddSpace)
{
	if(!m_pChartOcx)
		return false;
	return m_pChartOcx->SetStartPeriodOfPacketAdapter(pcszPacketName, lStartPeriod, bAddSpace);
}


/*-----------------------------------------------------------------------------------------
 - Function    :  SetStartIndexOfPacket
 - Created at  :  2005-08-27   17:44
 - Author      :  최종찬
 - Description :  Packet의 시작인덱스를 지정한다.
 -----------------------------------------------------------------------------------------*/
bool CIWChartOcx050531::SetStartIndexOfPacket(const char* pcszPacketName, int nStartDataIndex, BOOL bAddSpace)
{
	if(!m_pChartOcx)
		return false;
	return m_pChartOcx->SetStartIndexOfPacket(pcszPacketName, nStartDataIndex, bAddSpace);
}


/*-----------------------------------------------------------------------------------------
 - Function    :  HasGraph
 - Created at  :  2005-06-07   15:56
 - Author      :  최종찬
 - Description :  해당 그래프가 있는지 확인한다.
 -----------------------------------------------------------------------------------------*/
bool CIWChartOcx050531::HasGraph(const char* pcszGraphName)
{
	if(!m_pChartOcx)
		return false;
	return m_pChartOcx->HasGraphAdapter(pcszGraphName);
}

/*-----------------------------------------------------------------------------------------
 - Function    :  RemoveGraph
 - Created at  :  2005-11-07
 - Author      :  김선영
 - Description :  해당 그래프가 삭제하기.
 -----------------------------------------------------------------------------------------*/
bool CIWChartOcx050531::RemoveGraph(const char* pcszIndicatorName)
{
	if(m_pChartOcx == NULL)
		return false;
	return m_pChartOcx->RemoveChart(pcszIndicatorName);
}

/*-----------------------------------------------------------------------------------------
 - Function    :  RemovePacket
 - Created at  :  2005-11-07
 - Author      :  김선영
 - Description :  해당 packet 삭제하기.
					bIsOnlyDataClear = true : data 만 삭제
					bIsOnlyDataClear = false : CPacket 삭제
 -----------------------------------------------------------------------------------------*/
bool CIWChartOcx050531::RemovePacket(const char* pcszPacketName, const bool bIsOnlyDataClear)
{
	if(m_pChartOcx == NULL)
		return false;

	if(!bIsOnlyDataClear)
		return m_pChartOcx->RemovePacket(pcszPacketName);
	return m_pChartOcx->ClearPacketData(pcszPacketName, true);
}

/*-----------------------------------------------------------------------------------------
 - Function    :  GetHtsPath
 - Created at  :  2005-07-14   10:59
 - Author      :  최종찬
 - Description :  Hts의 Dir Path를 알려준다.
 -----------------------------------------------------------------------------------------*/
LPCTSTR CIWChartOcx050531::GetHtsPath(void)
{
	if(!m_pChartOcx)
		return _T("");
	return m_pChartOcx->GetBaseDeskPath();
}

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2005/8/18
// Modifier		: 
// Comments		: for Event from AddIn
//-----------------------------------------------------------------------------
void CIWChartOcx050531::FireAddInEvent( const char *p_szAddInName, short p_nEventID, const char *p_szData, double p_dData, long p_lData)
{
	if( !m_pChartOcx) return;
	m_pChartOcx->NeoAddInEvent( p_szAddInName, p_nEventID, p_szData, p_dData, p_lData);
}

/*-----------------------------------------------------------------------------------------
 - Function    :  SetRealData
 - Created at  :  2005-08-22   20:09
 - Author      :  최종찬
 - Parameters  :  pcszPacketName - 패킷명
 -                dData          - 리얼데이타
 -                nOption        - 1: Update, 2:Append
 - Description :  리얼데이타를 처리하는 Interface를 제공한다.
 -----------------------------------------------------------------------------------------*/
void CIWChartOcx050531::SetRealData(const char* pcszPacketName, double dData, short nOption)
{
	if(!m_pChartOcx)
		return;
	m_pChartOcx->SetRealDataAdapter(pcszPacketName, dData, nOption);
}

//////////////////////////////////////////////////////////////////////
// 050531 Version, 전용 Interface
//		AddIn Module				Site			Manager
//		ChartCustomAddIn			한화			배승원
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Temp Interface I : DLL Container 연계 Interface
//////////////////////////////////////////////////////////////////////

// [04.10.11] DLL Container에게 Message하는 Interface를 제공한다.
LRESULT CIWChartOcx050531::SendEventMsgToDLL( UINT message, WPARAM wParam, LPARAM lParam)
{
	if( m_pChartOcx) m_pChartOcx->SendEventMsgToDLL( message, wParam, lParam);
	return 0;
}
