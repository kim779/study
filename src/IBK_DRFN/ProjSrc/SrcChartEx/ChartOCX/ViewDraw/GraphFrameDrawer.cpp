// GraphFrameDrawer.cpp: implementation of the CGraphFrameDrawer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GraphFrameDrawer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "Color.h"
#include "GraphFrameDrawerImplementation.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// class CGraphFrameDrawer

CGraphFrameDrawer::CGraphFrameDrawer():
	m_pGraphFrameDrawerImpl(NULL)
{
}

CGraphFrameDrawer::~CGraphFrameDrawer()
{
	delete m_pGraphFrameDrawerImpl;
}

void CGraphFrameDrawer::DrawGraphFrame(CDC* pDC, 
			const CRect& graphRegion,		// 그릴 graph 영역
			const CBlockBaseData::BLOCKCOMPART eBlockCompart, // block 구분타입
			const int interBlockHalfGap,		 // block간의 Gap / 2
			const int graphRegionFrameThickness, // block frame 굵기
			const int nGraphBackgroundGapX,	// graph 바탕의 간격(가로)
			const int nGraphBackgroundGapY,	// graph 바탕의 간격(세로)
			const CGraphRegionColor& graphRegionColor)	// Graph Region 색
{
	m_pGraphFrameDrawerImpl->DrawGraphFrame(pDC, graphRegion, eBlockCompart, interBlockHalfGap, graphRegionFrameThickness, nGraphBackgroundGapX, nGraphBackgroundGapY, graphRegionColor);
}

void CGraphFrameDrawer::DrawBlockCompartLine(CDC* pDC, const CPoint& startPt, const CPoint& endPt, const COLORREF& blockCompartLineColor)
{
	m_pGraphFrameDrawerImpl->DrawBlockCompartLine(pDC, startPt, endPt, blockCompartLineColor);
}
