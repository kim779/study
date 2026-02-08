// IWChartOCX.cpp: implementation of the CIWChartOCX class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IWChartOCX.h"

#include "../../Include/GlobalSharedData.h"	// for ST_GSHAREDFORMAT
#include "../IAddInDll.h"					// for IAddInDll
#include "IChartCtrl.h"						// for IChartCtrl
#include "IWAddInToolMgr.h"					// for CIWAddInToolMgr
#include "IWPacketManager.h"				// for CIWPacketManager
#include "IWChartManager.h"					// for CIWChartManager
#include "IWGraphInfoManager.h"				// for CIWGraphInfoManager
#include "IWString.h"								// for CIWString
// PacketListManager : 복수종목 - ojtaso (20070110)
#include "PacketListManager.h"

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
CIWChartOCX::CIWChartOCX( IChartCtrl *p_pIChartCtrl, CAddInManagerImp *p_pAddInManagerImp)
{
	m_pIChartCtrl		= p_pIChartCtrl;
	m_pAddInManagerImp	= p_pAddInManagerImp;
	if( !m_pIChartCtrl)
	{
		AfxMessageBox( "CIWChartOCX Null Wrapping Error!");
		return;
	}
}

CIWChartOCX::~CIWChartOCX()
{
}


//////////////////////////////////////////////////////////////////////
// (2006/1/17 - Seung-Won, Bae) Get ChartCtrl Address for Special Usage
//////////////////////////////////////////////////////////////////////
CCommonInterface *CIWChartOCX::GetCommonInterface( void)
{
	if( !m_pIChartCtrl) return NULL;
	return m_pIChartCtrl->GetCommonInterface();
}


//////////////////////////////////////////////////////////////////////
// (2004.10.11, 배승원) OCX로부터 주요 Manager의 Interface를 구하여 관리한다.
//////////////////////////////////////////////////////////////////////

// (2004.10.11, 배승원) OCX로부터 주요 Manager의 Interface를 구하여 관리한다.
IPacketManager *CIWChartOCX::GetIPacketManager( void)
{
	if( !m_pIChartCtrl) return NULL;
	CPacketList *pPacketList = m_pIChartCtrl->GetPacketList();
	if( !pPacketList) return NULL;

	// PacketListManager Setting : 복수종목 - ojtaso (20070307)
	IPacketManager* pIPacketList = CIWPacketManager::CreateObject( pPacketList);
	pIPacketList->SetPacketListManager(m_pIChartCtrl->GetPacketListManager());
	return pIPacketList;

//	return CIWPacketManager::CreateObject( pPacketList);
}
IChartManager *CIWChartOCX::GetIChartManager( void)
{
	if( !m_pIChartCtrl) return NULL;
	CMainBlock *pMainBlock = m_pIChartCtrl->GetMainBlock();
	if( !pMainBlock) return NULL;
		
	//{{ XScaleManager Setting : XScale - LYH (20070326)
	IChartManager* pChartManager = CIWChartManager::CreateObject( pMainBlock);
	pChartManager->SetXScaleManager(m_pIChartCtrl->GetXScaleManager());
	return pChartManager;
	//return CIWChartManager::CreateObject( pMainBlock);
	//}}
}
IGraphInfoManager *CIWChartOCX::GetIGraphInfoManager( void)
{
	if( !m_pIChartCtrl) return NULL;
	CIndicatorList *pIndicatorList = m_pIChartCtrl->GetIndicatorList();
	if( !pIndicatorList) return NULL;
	return CIWGraphInfoManager::CreateObject( pIndicatorList);
}
IAddInToolMgr *CIWChartOCX::GetIAddInToolMgr( void)
{
	if( !m_pAddInManagerImp) return NULL;
	return CIWAddInToolMgr::CreateObject( m_pAddInManagerImp);
}

//////////////////////////////////////////////////////////////////////
// (2004.10.19, 배승원) Windows 관련 Interface를 제공한다.
//////////////////////////////////////////////////////////////////////

// [04.10.11] ReDrawing을 위한 Interface를 제공한다.
// 	void CIWChartOCX::InvalidateControl( void)
// 	{
// 		if( m_pIChartCtrl) m_pIChartCtrl->InvalidateControl();
// 	}
long CIWChartOCX::InvalidateControl( void)
{
	if( !m_pIChartCtrl) return 0;

	return m_pIChartCtrl->InvalidateControl();
}

// [04/10/11] ReDrawing을 위한 Interface를 제공한다.
HWND CIWChartOCX::GetOcxHwnd( void)
{
	if( m_pIChartCtrl) return m_pIChartCtrl->GetSafeHwnd();
	return NULL;
}

// [04/11/02] Counter Capturing Interface를 제공한다.
HWND CIWChartOCX::AddSetCapture( void)
{
	if( m_pIChartCtrl)
	{
		CWnd *pWnd = m_pIChartCtrl->AddSetCapture();
		if( pWnd) return pWnd->GetSafeHwnd();
	}
	return NULL;
}

// [04/11/02] Counter Capturing Interface를 제공한다.
BOOL CIWChartOCX::RemoveSetCapture( void)
{
	if( m_pIChartCtrl) return m_pIChartCtrl->RemoveSetCapture();
	return FALSE;
}

// [04/10/25] 새로운 Cursor를 설정하면서, 현재 설정된 Cursor를 확인한다.
HCURSOR CIWChartOCX::SetCursorHandle( HCURSOR p_hNewCursor)
{
	if( !m_pIChartCtrl) return NULL;
	return m_pIChartCtrl->SetCursorHandle( p_hNewCursor);
}

// [04/10/26] 현재 설정된 Cursor를 확인한다.
HCURSOR CIWChartOCX::GetCursorHandle( void)
{
	if( !m_pIChartCtrl) return NULL;
	return m_pIChartCtrl->GetCursorHandle();
}

// [04.10.25] Chart 전용 Font를 제공한다.
CFont *CIWChartOCX::GetChartFont( void)
{
	if( !m_pIChartCtrl) return NULL;
	return m_pIChartCtrl->GetChartFont();
}

//////////////////////////////////////////////////////////////////////
// (2004.10.18, 배승원) 환경 정보를 조회하는 Interface를 제공한다.
//////////////////////////////////////////////////////////////////////

// [04.10.18] 사용자 Folder를 구하는 Interface를 제공한다.
const char *CIWChartOCX::GetUserFilePath( void)
{
	if( !m_pIChartCtrl) return NULL;
	return m_pIChartCtrl->GetUserFilePath();
}

/*-----------------------------------------------------------------------------------------
 - Function    :  GetHtsPath
 - Created at  :  2005-07-14   10:59
 - Author      :  최종찬
 - Description :  Hts의 Dir Path를 알려준다.
 -----------------------------------------------------------------------------------------*/
const char *CIWChartOCX::GetHtsPath( void)
{
	if(!m_pIChartCtrl) return NULL;
	return m_pIChartCtrl->GetBaseDeskPath();
}

//sy 2006.02.21. OCX Path
const char *CIWChartOCX::GetOCXPath( void)
{
	if(!m_pIChartCtrl) return NULL;
	return  m_pIChartCtrl->GetOCXPath();
}

// (2006/1/18 - Seung-Won, Bae) Set the Design Info
void CIWChartOCX::SetStrBlockRegion( const char *p_szBlockRegion)
{
	if( m_pIChartCtrl) m_pIChartCtrl->SetStrBlockRegion( p_szBlockRegion);
}

//////////////////////////////////////////////////////////////////////
// (2004.10.11, 배승원) Chart의 현 상태를 확인하는 Interface를 제공한다.
//////////////////////////////////////////////////////////////////////

// [04.10.11] 특수차트의 경우인지 확인하는 Interface를 제공한다. (역시계곡선/삼선전환도/P&F)
BOOL CIWChartOCX::IsOnSpecialChart( void)
{
	if( m_pIChartCtrl) return m_pIChartCtrl->IsRunningOneChart();
	return FALSE;
}

// (2005.01.20, 배승원) ToolBar에 다음 Item이 있는지 확인하는 Interface를 제공한다.
BOOL CIWChartOCX::HasToolItem( CToolOptionInfo::TOOLOPTION p_eToolItem)
{
	if( m_pIChartCtrl) return m_pIChartCtrl->HasToolItem( p_eToolItem);
	return FALSE;
}

// (2006/1/20 - Seung-Won, Bae) Get Numerical Inquiry Option Setting
UINT CIWChartOCX::GetNumericalInquiryOption( void) const
{
	if( m_pIChartCtrl) return m_pIChartCtrl->GetNumericalInquiryOption();
	return 0;
}

// (2006/1/17 - Seung-Won, Bae) Check RunTimeMode
BOOL CIWChartOCX::IsRuntimeMode( void) const
{
	if( !m_pIChartCtrl) return FALSE;
	return m_pIChartCtrl->IsRunTimeMode();
}

// (2006/1/21 - Seung-Won, Bae) Check Graph Menu Usable
BOOL CIWChartOCX::IsGraphMenuShow( void) const
{
	if( !m_pIChartCtrl) return FALSE;
	return m_pIChartCtrl->IsGraphMenuShow();
}

// (2006/1/22 - Seung-Won, Bae) Retrieve Bong Count Margin.
short CIWChartOCX::GetBongCountMargin( void) const
{
	if( !m_pIChartCtrl) return 0;
	return m_pIChartCtrl->GetBongCountMargin();
}

// (2006/1/22 - Seung-Won, Bae) Retrieve Graph RButton Type.
short CIWChartOCX::GetGraphRButtonType( void) const
{
	if( !m_pIChartCtrl) return 0;
	return m_pIChartCtrl->GetGraphRButtonType();
}

// (2006/2/6 - Seung-Won, Bae) Get OCX Region except ToolBar Region.
CRect CIWChartOCX::GetOcxRegion( void) const
{
	if( !m_pIChartCtrl) return CRect( 0, 0, 0, 0);
	return m_pIChartCtrl->GetOcxRegion();
}

// (2006/2/6 - Seung-Won, Bae) Check ScrollBar
BOOL CIWChartOCX::IsUseToolScrollBar( void) const
{
	if( !m_pIChartCtrl) return FALSE;
	return m_pIChartCtrl->IsUseToolScrollBar();
}

//////////////////////////////////////////////////////////////////////
// (2006/1/24 - Seung-Won, Bae) Interface for Analysis Tool Option
//////////////////////////////////////////////////////////////////////
BOOL CIWChartOCX::IsShowTrendLineText( void) const
{
	if( m_pIChartCtrl) return m_pIChartCtrl->IsShowTrendLineText();
	return FALSE;
}
BOOL CIWChartOCX::IsShowHorizonTextLeft( void) const
{
	if( m_pIChartCtrl) return m_pIChartCtrl->IsShowHorizonTextLeft();
	return FALSE                                                       ;
}
BOOL CIWChartOCX::IsShowHorizonTextRight( void) const
{
	if( m_pIChartCtrl) return m_pIChartCtrl->IsShowHorizonTextRight();
	return FALSE;
}
BOOL CIWChartOCX::IsShowVerticalText( void) const
{
	if( m_pIChartCtrl) return m_pIChartCtrl->IsShowVerticalText();
	return FALSE;
}
BOOL CIWChartOCX::IsShowThreeText( void) const
{
	if( m_pIChartCtrl) return m_pIChartCtrl->IsShowThreeText();
	return FALSE;
}
BOOL CIWChartOCX::IsShowFourText( void) const
{
	if( m_pIChartCtrl) return m_pIChartCtrl->IsShowFourText();
	return FALSE;
}
BOOL CIWChartOCX::IsShowFiboText( void) const
{
	if( m_pIChartCtrl) return m_pIChartCtrl->IsShowFiboText();
	return FALSE;
}
COLORREF CIWChartOCX::GetAnalysisToolColor( void) const
{
	if( m_pIChartCtrl) return m_pIChartCtrl->GetStyleValueAnalysisToolColor();
	return FALSE;
}

//////////////////////////////////////////////////////////////////////
// (2004.10.11, 배승원) Data Index 및 Position 관련 Interface                                                                                                                                                                                             
//////////////////////////////////////////////////////////////////////

// [04/10/11] 마지막 실 Data의 Index를 구하는 Interface를 제공한다.
int CIWChartOCX::GetLastRealDataIndex( void)
{
	if( m_pIChartCtrl) return m_pIChartCtrl->GetDataCountForToolBar();
	return -1;
}

// [04/10/11] ScrollBar상의 마지막 Scroll Index를 구하는 Interface를 제공한다. (여백포함)
BOOL CIWChartOCX::GetMaxScrollRange( int &p_nMaxScrollRange)
{
	if( m_pIChartCtrl) return m_pIChartCtrl->GetScrollRange( p_nMaxScrollRange);
	return FALSE;
}

// [04/10/11] Chart의 현재 보이는 View 영역의 첫 Data Index와 끝 Data Index를 구하거나, 설정하는 Interface를 제공한다.
BOOL CIWChartOCX::GetViewStartEndIndex( int &p_nStartIndex, int &p_nEndIndex)
{
	if( m_pIChartCtrl) return m_pIChartCtrl->GetDataStartEnd( p_nStartIndex, p_nEndIndex);
	return FALSE;
}
BOOL CIWChartOCX::SetViewStartEndIndex( int p_nStartIndex, int p_nEndIndex)
{
	if( m_pIChartCtrl) return m_pIChartCtrl->SetStartEndIndex( p_nStartIndex, p_nEndIndex);
	return FALSE;
}

// [04/10/27] Data 좌표와 Screen 좌표를 변환하는 Interface를 제공한다.
int CIWChartOCX::ConvertDataToYPosition( double p_dData, double p_dDataMax, double p_dDataMin, int p_nYPositionMax, int p_nYPositionMin, BOOL p_bLog, BOOL p_bReverse)
{
	if( m_pIChartCtrl) return m_pIChartCtrl->ConvertDataToYPosition( p_dData, p_dDataMax, p_dDataMin, p_nYPositionMax, p_nYPositionMin, p_bLog, p_bReverse);
	return 0;
}
double CIWChartOCX::ConvertYPositionToData( int p_nYPosition, int p_nYPositionMax, int p_nYPositionMin, double p_dDataMax, double p_dDataMin, BOOL p_bLog, BOOL p_bReverse)
{
	if( m_pIChartCtrl) return m_pIChartCtrl->ConvertYPositionToData( p_nYPosition, p_nYPositionMax, p_nYPositionMin, p_dDataMax, p_dDataMin, (p_bLog ? true: false), (p_bReverse ? true: false));
	return 0;
}

// [04/11/09] Scroll/Zoom을 쉽게 처리하는 Interface를 제공한다.
BOOL CIWChartOCX::ScrollTo( int p_nScrollValue, BOOL p_bRelative)
{
	if( m_pIChartCtrl) return m_pIChartCtrl->ScrollTo( p_nScrollValue, p_bRelative);
	return FALSE;
}
BOOL CIWChartOCX::ZoomTo( int p_nZoomValue, BOOL p_bRelative)
{
	if( m_pIChartCtrl) return m_pIChartCtrl->ZoomTo( p_nZoomValue, p_bRelative);
	return FALSE;
}

// (2005.01.13, 배승원) 차트의 전체보기 Mode 여부를 확인 및 설정하는 Interface를 제공한다.
BOOL CIWChartOCX::CheckFullView( void) const
{
	if( !m_pIChartCtrl) return FALSE;
	return m_pIChartCtrl->IsWholeView();
}
void CIWChartOCX::SetFullView( BOOL p_bFullView)
{
	if( !m_pIChartCtrl) return;
	if( ( p_bFullView && m_pIChartCtrl->IsWholeView())
		|| ( !p_bFullView && !m_pIChartCtrl->IsWholeView())) return;

	m_pIChartCtrl->SetWholeView( p_bFullView);
	m_pIChartCtrl->InvalidateControl();
}

// (2006/2/6 - Seung-Won, Bae) DragZoomIn for DragZoom AddIn
void CIWChartOCX::DragZoomIn( const CPoint &p_ptStart, const CPoint &p_ptEnd)
{
	if( m_pIChartCtrl) m_pIChartCtrl->DragZoomIn( p_ptStart, p_ptEnd);
}

//////////////////////////////////////////////////////////////////////
// [04.10.11] 공유 관련 Interface
//////////////////////////////////////////////////////////////////////

// [04.10.11] 주어진 Global 공유가 설정되어 있는지 확인하는 Interface를 제공한다.
BOOL CIWChartOCX::CheckGSharedSetting( const char *p_szGSharedName, BOOL p_bStart, BOOL p_bGet, BOOL p_bSet)
{
	if( !m_pIChartCtrl) return FALSE;
	return m_pIChartCtrl->CheckGSharedSetting( p_szGSharedName, p_bStart, p_bGet, p_bSet);
}

// [04.10.11] 주어진 Global 공유에 공유를 발생시키는 Interface를 제공한다.
BOOL CIWChartOCX::FireGSharedData( const char *p_szGSharedName, const char *p_szGSharedData)
{
	if( !m_pIChartCtrl) return FALSE;
	return m_pIChartCtrl->FireGSharedData( p_szGSharedName, p_szGSharedData);
}

// (2005/8/10 - Seung-Won, Bae) used in Only Hanwha
// (2005/8/10 - Seung-Won, Bae) Retrieve the Share Info Item Setting
BOOL CIWChartOCX::GetShareInfoItem( const char *p_szShareName, int &p_nShareType, int &p_nShareArea, long &p_lCodeFilter)
{
	if( !m_pIChartCtrl) return FALSE;
	m_pIChartCtrl->GetShareInfoItem( p_szShareName, p_nShareType, p_nShareArea, p_lCodeFilter);
	return TRUE;
}

// (2005/8/10 - Seung-Won, Bae) Fire Shared Data
BOOL CIWChartOCX::FireSharedData( const char *p_szShareName, const char *p_szSharedData)
{
	if( !m_pIChartCtrl) return FALSE;
	return m_pIChartCtrl->FireSharedData( p_szShareName, p_szSharedData);
}

//////////////////////////////////////////////////////////////////////
// (2004.10.19, 배승원) Image 저장 및 출력을 위한 Drawing Interface를 제공한다.
//////////////////////////////////////////////////////////////////////

// (2004.10.19, 배승원) Image 저장 및 출력을 위한 Drawing Interface를 제공한다.
void CIWChartOCX::DrawForImage( CDC *p_pDC, CRect p_rctSource, BOOL p_bUseScreenColor)
{
	if( !m_pIChartCtrl) return;
	CBitmap bufferBitmap;
	m_pIChartCtrl->OnDrawForPrint( *p_pDC, bufferBitmap, p_rctSource, p_bUseScreenColor);
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
void CIWChartOCX::AddPacket(const char* pcszPacketOption)
{
	if(!m_pIChartCtrl)
		return;
	m_pIChartCtrl->AddPacketAdapter(pcszPacketOption);
}

/*-----------------------------------------------------------------------------------------
 - Function    :  AddGraph
 - Created at  :  2005-06-01   13:35
 - Author      :  최종찬
 - Description :  지표를 추가를 한다.
 -----------------------------------------------------------------------------------------*/
void CIWChartOCX::AddGraph(short nRow, short nColumn, short nVertScaleGroup, const char* szGraphOption, BOOL bCreateBlock, BOOL bCreateVertScaleGroup, BOOL bUseAutoScale)
{
	if(!m_pIChartCtrl)
		return;
	m_pIChartCtrl->AddGraphAdapter(nRow, nColumn, nVertScaleGroup, szGraphOption, bCreateBlock, bCreateVertScaleGroup, bUseAutoScale);
	return;
}

/*-----------------------------------------------------------------------------------------
 - Function    :  ChangeSubGraphData
 - Created at  :  2005-06-01   16:45
 - Author      :  최종찬
 - Description :  지표들의 그래프 속성(색, 두께, 유형)을 변경한다.
 -----------------------------------------------------------------------------------------*/
bool CIWChartOCX::ChangeSubGraphData(const char* pcszGraphName, const char* pcszSubGraphName, COLORREF clrLineColor, int nThickness, int nDrawStyle)
{
	if(!m_pIChartCtrl)
		return false;
	return m_pIChartCtrl->ChangeSubGraphData(pcszGraphName, pcszSubGraphName, clrLineColor, nThickness, nDrawStyle);
}


/*-----------------------------------------------------------------------------------------
 - Function    :  SetStartPeriodOfPacket
 - Created at  :  2005-06-07   15:55
 - Author      :  최종찬
 - Description :  Packet의 시작일자를 지정한다.
 -----------------------------------------------------------------------------------------*/
bool CIWChartOCX::SetStartPeriodOfPacket(const char* pcszPacketName, long lStartPeriod, BOOL bAddSpace)
{
	if(!m_pIChartCtrl)
		return false;
	return m_pIChartCtrl->SetStartPeriodOfPacketAdapter(pcszPacketName, lStartPeriod, bAddSpace);
}

/*-----------------------------------------------------------------------------------------
 - Function    :  SetStartIndexOfPacket
 - Created at  :  2005-08-27   17:44
 - Author      :  최종찬
 - Description :  Packet의 시작인덱스를 지정한다.
 -----------------------------------------------------------------------------------------*/
bool CIWChartOCX::SetStartIndexOfPacket(const char* pcszPacketName, int nStartDataIndex, BOOL bAddSpace)
{
	if(!m_pIChartCtrl)
		return false;
	return m_pIChartCtrl->SetStartIndexOfPacket(NULL, pcszPacketName, nStartDataIndex, bAddSpace);
}


/*-----------------------------------------------------------------------------------------
 - Function    :  HasGraph
 - Created at  :  2005-06-07   15:56
 - Author      :  최종찬
 - Description :  해당 그래프가 있는지 확인한다.
 -----------------------------------------------------------------------------------------*/
bool CIWChartOCX::HasGraph(const char* pcszGraphName)
{
	if(!m_pIChartCtrl)
		return false;
	return m_pIChartCtrl->HasGraphAdapter(pcszGraphName);
}

/*-----------------------------------------------------------------------------------------
 - Function    :  RemoveGraph
 - Created at  :  2005-11-07
 - Author      :  김선영
 - Description :  해당 그래프가 삭제하기.
 -----------------------------------------------------------------------------------------*/
bool CIWChartOCX::RemoveGraph(const char* pcszGraphName)
{
	if(m_pIChartCtrl == NULL)
		return false;
	return (m_pIChartCtrl->RemoveGraph( pcszGraphName) ? true: false);
}

/*-----------------------------------------------------------------------------------------
 - Function    :  RemovePacket
 - Created at  :  2005-11-07
 - Author      :  김선영
 - Description :  해당 packet 삭제하기.
					bIsOnlyDataClear = true : data 만 삭제
					bIsOnlyDataClear = false : CPacket 삭제
 -----------------------------------------------------------------------------------------*/
bool CIWChartOCX::RemovePacket(const char* pcszPacketName, const bool bIsOnlyDataClear)
{
	if(m_pIChartCtrl == NULL)
		return false;

	if(!bIsOnlyDataClear)
		return (m_pIChartCtrl->RemovePacket(pcszPacketName) ? true: false);
	return (m_pIChartCtrl->ClearPacketData(pcszPacketName, true) ? true: false);
}

//////////////////////////////////////////////////////////////////////
// 종찬(05/08/22) 리얼을 처리할수 있는 Interface를 제공한다.
//////////////////////////////////////////////////////////////////////

/*-----------------------------------------------------------------------------------------
 - Function    :  SetRealData
 - Created at  :  2005-08-22   20:09
 - Author      :  최종찬
 - Parameters  :  pcszPacketName - 패킷명
 -                dData          - 리얼데이타
 -                nOption        - 1: Update, 2:Append
 - Description :  리얼데이타를 처리하는 Interface를 제공한다.
 -----------------------------------------------------------------------------------------*/
void CIWChartOCX::SetRealData(const char* pcszPacketName, double dData, short nOption)
{
	if(!m_pIChartCtrl)
		return;
	m_pIChartCtrl->SetRealData(pcszPacketName, dData, nOption);
}

// (2006/1/20 - Seung-Won, Bae) Show Tooltip
void CIWChartOCX::ShowTooltip( const CPoint &p_ptPoint)
{
//	if( m_pIChartCtrl) m_pIChartCtrl->ShowTooltip( p_ptPoint);
}
void CIWChartOCX::HideTooltip( void)
{
//	if( m_pIChartCtrl) m_pIChartCtrl->HideTooltip();
}

// (2006/1/21 - Seung-Won, Bae) Save Current Graph State
BOOL CIWChartOCX::SaveChartCfg( void)
{
	if( m_pIChartCtrl) return m_pIChartCtrl->SaveChartCfg();
	return FALSE;
}
// (2006/1/23 - Seung-Won, Bae) Set Logging Flag
void CIWChartOCX::SetLoggingFlag( const BOOL p_bInLogging)
{
	if( m_pIChartCtrl) m_pIChartCtrl->SetLoggingFlag( p_bInLogging);
}

// (2006/1/28 - Seung-Won, Bae) Reset Tool UI
void CIWChartOCX::ReleaseToolUI( const CToolOptionInfo::TOOLOPTION p_eToolOption)
{
	if( m_pIChartCtrl) m_pIChartCtrl->ReleaseOtherToolUI( p_eToolOption);
}

// (2006/2/7 - Seung-Won, Bae) Show and Hide Numerical Inquiry Dialog
void CIWChartOCX::ShowNumericalInquiryDlg( const BOOL p_bShow, const BOOL p_bAutoMove)
{
}
void CIWChartOCX::ShowNumericalInquiryDlg( CDC *p_pDC, const CPoint &p_ptPoint)
{
}

//////////////////////////////////////////////////////////////////////
// (2006/1/17 - Seung-Won, Bae) Notify from AddIn
//////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2005/8/18
// Modifier		: 
// Comments		: for Event from AddIn
//-----------------------------------------------------------------------------
void CIWChartOCX::FireAddInEvent( const char *p_szAddInName, short p_nEventID, const char *p_szData, double p_dData, long p_lData)
{
	if( !m_pIChartCtrl) return;
	m_pIChartCtrl->NeoAddInEvent( p_szAddInName, p_nEventID, p_szData, p_dData, p_lData);
}

// [04.10.11] DLL Container에게 Message하는 Interface를 제공한다.
LRESULT CIWChartOCX::SendEventMsgToDLL( UINT message, WPARAM wParam, LPARAM lParam)
{
	if( m_pIChartCtrl) m_pIChartCtrl->SendEventMsgToDLL( message, wParam, lParam);
	return 0;
}

// (2006/11/15 - Seung-Won, Bae) Notify Tool State Changing to MChart an ToolBar
BOOL CIWChartOCX::OnToolStateChanged( const CToolOptionInfo::TOOLOPTION p_eToolID, const BOOL p_bTurnOn)
{
	if( m_pIChartCtrl) return m_pIChartCtrl->OnToolStateChanged( p_eToolID, p_bTurnOn);
	return FALSE;
}

// (2007/1/5 - Seung-Won, Bae) Excute Tool Command
BOOL CIWChartOCX::RunToolCommand( const CToolOptionInfo::TOOLOPTION p_eToolOption, const int p_nParam)
{
	if( m_pIChartCtrl) return m_pIChartCtrl->RunToolFunction( p_eToolOption, p_nParam);
	return FALSE;
}

// [07.05.18] 복수종목
IString *CIWChartOCX::GetSelectedRQ( void)
{
	if( !m_pIChartCtrl) return NULL;

	IString *pIString = CIWString::CreateObject( m_pIChartCtrl->GetCurrentRQ());
	return pIString;
}
