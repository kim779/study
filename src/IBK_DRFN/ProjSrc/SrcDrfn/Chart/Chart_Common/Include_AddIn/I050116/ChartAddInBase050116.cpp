// ChartAddInBase050116.cpp: implementation of the CChartAddInBase050116 class.
//
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// *. 주의!	050116 Version의 AddIn에서 이용되는 AddIn측 Interface Support Base Class입니다.
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
#include "ChartAddInBase050116.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChartAddInBase050116::CChartAddInBase050116( IChartOCX *p_pIChartOCX)
{
	// (2004.10.15, 배승원) OCX Interface가 NULL인 경우를 대비하여, Member에 미리 NULL 초기화를 처리한다.
	m_pIPacketManager		= NULL;
	m_pIChartManager		= NULL;
	m_pIGraphInfoManager	= NULL;
	m_pChartCWnd			= NULL;

	// (2004.10.08, 배승원) Chart Ocx Interface를 관리하여, 이후 타 Object도 조회할 수 있도록 한다.
	m_pIChartOCX = p_pIChartOCX;
	if( !m_pIChartOCX)
	{
		AfxMessageBox( "CChartAddInBase050116 Null Wrapping Error!");
		return;
	}
	m_pIChartOCX->AddRef();

	// (2004.10.11, 배승원) OCX로부터 주요 Manager의 Interface를 구하여 관리한다.
	m_pIPacketManager		= m_pIChartOCX->GetIPacketManager050116();
	m_pIChartManager		= m_pIChartOCX->GetIChartManager050116();
	m_pIGraphInfoManager	= m_pIChartOCX->GetIGraphInfoManager050116();

	// (2004.10.25, 배승원) Chart OCX Control Window Pointer를 관리하여 Windows Interface를 이용할 수 있도록 한다.
	m_pChartCWnd = CWnd::FromHandle( m_pIChartOCX->GetOcxHwnd());
}

CChartAddInBase050116::~CChartAddInBase050116()
{
	// (2004.10.08, 배승원) Chart Ocx Interface를 관리하여, 이후 타 Object도 조회할 수 있도록 한다.
	m_pIChartOCX->Release();

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
}

// (2004.10.13, 배승원) 기본 Manager가 등록되어 있는지 확인하는 Interface를 제공한다.
BOOL CChartAddInBase050116::HasFullManager( void)
{
	return m_pIChartOCX && m_pIPacketManager && m_pIChartManager && m_pIGraphInfoManager;
}
