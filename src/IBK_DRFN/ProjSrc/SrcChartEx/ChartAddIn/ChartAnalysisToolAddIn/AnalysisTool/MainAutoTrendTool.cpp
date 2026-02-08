// MainAutoTrendTool.cpp: implementation of the CMainAutoTrendTool class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MainAutoTrendTool.h"

#include "MainAutoTrendImplementation.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMainAutoTrendTool::CMainAutoTrendTool():
	m_pMainAutoTrendImpl(NULL)
{
	m_pMainAutoTrendImpl = new CMainAutoTrendImplementation(NULL);
}

CMainAutoTrendTool::CMainAutoTrendTool(CWnd* pParent)
{
	m_pMainAutoTrendImpl = new CMainAutoTrendImplementation(pParent);
}

CMainAutoTrendTool::~CMainAutoTrendTool()
{
	delete m_pMainAutoTrendImpl;
}


// public =====================================================================
void CMainAutoTrendTool::SetProcessStarting(const bool bStarting)
{
	m_pMainAutoTrendImpl->SetProcessStarting(bStarting);
}

void CMainAutoTrendTool::Set(CWnd* pParent, IChartManager* pMainBlock, IPacketManager* pPacketList, const COLORREF& penColor, const int nPenThickness)
{
	m_pMainAutoTrendImpl->Set(pParent, pMainBlock, pPacketList, penColor, nPenThickness);
}

void CMainAutoTrendTool::SetParent(CWnd* pParent)
{
	m_pMainAutoTrendImpl->SetParent(pParent);
}

void CMainAutoTrendTool::SetMainBlock(IChartManager* pMainBlock)
{
	m_pMainAutoTrendImpl->SetMainBlock(pMainBlock);
}

void CMainAutoTrendTool::SetPacketList(IPacketManager* pPacketList)
{
	m_pMainAutoTrendImpl->SetPacketList(pPacketList);
}

void CMainAutoTrendTool::SetPenColor(const COLORREF& color)
{
	m_pMainAutoTrendImpl->SetPenColor(color);
}

void CMainAutoTrendTool::SetPenThickness(const int nThickness)
{
	m_pMainAutoTrendImpl->SetPenThickness(nThickness);
}

// ----------------------------------------------------------------------------
void CMainAutoTrendTool::Calculate(const CCalculateBaseData::CALCULATETYPE eCalType)
{
	m_pMainAutoTrendImpl->Calculate(eCalType);
}

// ----------------------------------------------------------------------------
bool CMainAutoTrendTool::DoesProcessStarting() const
{
	return m_pMainAutoTrendImpl->DoesProcessStarting();
}

// ----------------------------------------------------------------------------
bool CMainAutoTrendTool::OnRButtonUp(CDC* pDC, const CPoint& point)
{
	return m_pMainAutoTrendImpl->OnRButtonUp(pDC, point);
}

bool CMainAutoTrendTool::OnMouseMove(CDC* pDC, const CPoint& point)
{
	return m_pMainAutoTrendImpl->OnMouseMove(pDC, point);
}

// ±×¸®±â
void CMainAutoTrendTool::OnDraw(CDC* pDC)
{
	m_pMainAutoTrendImpl->OnDraw(pDC);
}

