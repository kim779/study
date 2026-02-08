// MainAutoTrendImplementation.cpp: implementation of the CMainAutoTrendImplementation class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MainAutoTrendImplementation.h"

//#include "DefineData.h"
#include "MessageDefine.h"
#include "NonChartFormulate.h"
#include "../Include_Chart/Dll_Load/IMetaTable.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMainAutoTrendImplementation::CMainAutoTrendImplementation( IChartManager *p_pIChartManager):
	m_pParent(NULL),
	m_pIChartManager(NULL),
	m_pPacketList(NULL),
	m_bProcessStarting(false),
	m_bPrevPointUsed(false),
	m_prevPoint(-1, -1),
	m_calData( p_pIChartManager)
{
}

CMainAutoTrendImplementation::CMainAutoTrendImplementation( IChartManager *p_pIChartManager, CWnd* pParent):
	m_pParent(pParent),
	m_pIChartManager(NULL),
	m_pPacketList(NULL),
	m_bProcessStarting(false),
	m_bPrevPointUsed(false),
	m_prevPoint(-1, -1),
	m_calData( p_pIChartManager)
{
}

CMainAutoTrendImplementation::~CMainAutoTrendImplementation()
{
	if( m_pIChartManager) m_pIChartManager->Release();
	if( m_pPacketList) m_pPacketList->Release();
}

// public =====================================================================
void CMainAutoTrendImplementation::SetProcessStarting( IChartManager *p_pIChartManager, const bool bStarting)
{
	m_bProcessStarting = bStarting;
	Initialize( p_pIChartManager, true);

	if(bStarting){
		Calculate( p_pIChartManager);
	}

	//SendMessageToParent(bStarting);
}

void CMainAutoTrendImplementation::Set(CWnd* pParent, IChartManager* pMainBlock, IPacketManager* pPacketList, const COLORREF& penColor, const int nPenThickness)
{
	m_pParent = pParent;

	if( m_pIChartManager) m_pIChartManager->Release();
	m_pIChartManager = pMainBlock;
	if( m_pIChartManager) m_pIChartManager->AddRef();

	if( m_pPacketList) m_pPacketList->Release();
	m_pPacketList = pPacketList;
	if( m_pPacketList) m_pPacketList->AddRef();

	m_drawer.SetPenColor(penColor);
	m_drawer.SetPenThickness(nPenThickness);
}

void CMainAutoTrendImplementation::SetParent(CWnd* pParent)
{
	m_pParent = pParent;
}

void CMainAutoTrendImplementation::SetMainBlock(IChartManager* pMainBlock)
{
	if( m_pIChartManager) m_pIChartManager->Release();
	m_pIChartManager = pMainBlock;
	if( m_pIChartManager) m_pIChartManager->AddRef();
}

void CMainAutoTrendImplementation::SetPacketList(IPacketManager* pPacketList)
{
	if( m_pPacketList) m_pPacketList->Release();
	m_pPacketList = pPacketList;
	if( m_pPacketList) m_pPacketList->AddRef();
}

void CMainAutoTrendImplementation::SetPenColor(const COLORREF& color)
{
	m_drawer.SetPenColor(color);
}

void CMainAutoTrendImplementation::SetPenThickness(const int nThickness)
{
	m_drawer.SetPenThickness(nThickness);
}

// ----------------------------------------------------------------------------
void CMainAutoTrendImplementation::Calculate( IChartManager *p_pIChartManager, const CCalculateBaseData::CALCULATETYPE eCalType)
{
	if(!DoesStarting())
		return;

	if(eCalType == CCalculateBaseData::TRDATA_CAL)
		Initialize( p_pIChartManager, false);

	CNonChartFormulate().GetAutoTrendData(eCalType, "AutoTrend", m_pPacketList, &m_calData.GetPacketList());
}

// ----------------------------------------------------------------------------
bool CMainAutoTrendImplementation::DoesProcessStarting() const
{
	return m_bProcessStarting;
}

// ----------------------------------------------------------------------------
bool CMainAutoTrendImplementation::OnRButtonUp(CDC* pDC, const CPoint& point)
{
	if(!DoesStarting())
		return false;

	SetProcessStarting( m_pIChartManager, false);
	m_pParent->Invalidate();
	return true;
}

bool CMainAutoTrendImplementation::OnMouseMove(CDC* pDC, const CPoint& point)
{
	if(!DoesStarting())
		return false;

	bool bResult = GetAutoTrendDrawingCalData(point, m_calData);
	if(bResult)
		DrawNewToolDrawing(pDC, m_calData, point);
	else
		EraseLastToolDrawing(pDC, m_calData.GetNoFrameGraphRegion(), point);

	return bResult;
}

void CMainAutoTrendImplementation::OnDraw(CDC* pDC)
{
	if(!DoesStarting() || !m_bPrevPointUsed)
		return;

	m_drawer.Drawing(pDC, m_calData, m_prevPoint, R2_NOT);
}


// private ====================================================================
void CMainAutoTrendImplementation::Initialize( IChartManager *p_pIChartManager, const bool bIsAllData)
{
	m_bPrevPointUsed = false;
	if(bIsAllData)
		m_calData.Initialize( p_pIChartManager);

	if(m_pParent != NULL)
		m_pParent->Invalidate();
}

// ----------------------------------------------------------------------------
bool CMainAutoTrendImplementation::DoesStarting() const
{
	return (m_bProcessStarting && m_pIChartManager != NULL && m_pPacketList != NULL);
}

bool CMainAutoTrendImplementation::GetAutoTrendDrawingCalData(const CPoint& point, CAutoTrendDrawingCalculatorData& calData) const
{
	if(m_pIChartManager == NULL)
		return false;

	int nRow, nColumn, nV, nG;
	ILPCSTR szGraphName = m_pIChartManager->FindFirstIndicator( _MTEXT( C2_PRICE_CHART), nRow, nColumn, nV, nG, "DEFAULT");
	if( szGraphName.IsEmpty()) return false;
	IBlock *pIBlock = m_pIChartManager->GetBlock( nRow, nColumn);
	if( !pIBlock) return false;
	CRect region, rctNoFrameAndMargin;
	pIBlock->GetGraphRegions( region, rctNoFrameAndMargin);
	pIBlock->Release();
	pIBlock = NULL;

	if(region.IsRectEmpty() || !region.PtInRect(point))
		return false;

	calData.SetNoFrameGraphRegion(region);
	calData.GetCoordinateCalculatorData().GetCoordinateCalculatorData( point);
	return true;
}

// ----------------------------------------------------------------------------
// 마지막에 그린 tool 그림을 지운다. (-> 그리는 영역에서 그리는 영역밖으로 나갈 경우)
void CMainAutoTrendImplementation::EraseLastToolDrawing(CDC* pDC, const CRect& noFrameGraphRegion, const CPoint& point)
{
	if(!m_bPrevPointUsed)
		return;

	if(noFrameGraphRegion.PtInRect(point) && noFrameGraphRegion.PtInRect(m_prevPoint))
		return;

	m_drawer.Drawing(pDC, m_calData, m_prevPoint, R2_NOT);
	Initialize( m_pIChartManager, false);
}

// 이전 그림을 지우며 새로 그리기.
void CMainAutoTrendImplementation::DrawNewToolDrawing(CDC* pDC, const CAutoTrendDrawingCalculatorData& calData, const CPoint& point)
{
	if(!m_bPrevPointUsed)
		m_bPrevPointUsed = true;
	else
		m_drawer.Drawing(pDC, calData, m_prevPoint, R2_NOT);

	m_drawer.Drawing(pDC, calData, point, R2_NOT);
	m_prevPoint = point;
}

// ----------------------------------------------------------------------------
void CMainAutoTrendImplementation::SendMessageToParent(const bool bIsProcessStarting)
{
	if(m_pParent == NULL)
		return;

	if(bIsProcessStarting)
		m_pParent->PostMessage(UM_CURSOR_EXCHANGE, CCursorShape::TOOL_AUTOTREND);
	else
		m_pParent->PostMessage(UM_CURSOR_EXCHANGE, CCursorShape::OBJECT_NONE);
}
