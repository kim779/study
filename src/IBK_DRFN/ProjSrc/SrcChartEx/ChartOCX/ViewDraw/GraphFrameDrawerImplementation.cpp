// GraphFrameDrawerImplementation.cpp: implementation of the CGraphFrameDrawerImplementation class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GraphFrameDrawerImplementation.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "Color.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// class CGraphFrameDrawerImplementation

// public =================================================================================
// Graph Frame 그린다.
void CGraphFrameDrawerImplementation::DrawGraphFrame(CDC* pDC, 
			const CRect& graphRegion,		// 그릴 graph 영역
			const CBlockBaseData::BLOCKCOMPART eBlockCompart, // block 구분타입
			const int interBlockHalfGap,		 // block간의 Gap / 2
			const int graphRegionFrameThickness, // block frame 굵기
			const int nGraphBackgroundGapX,	// graph 바탕의 간격(가로)
			const int nGraphBackgroundGapY,	// graph 바탕의 간격(세로)
			const CGraphRegionColor& graphRegionColor)	// Graph Region 색
{
	DrawGraphBackground(pDC, graphRegion, graphRegionColor.GetBackgroundColor());
	if(eBlockCompart != CBlockBaseData::BCNONE)
	{
		DrawGraphLine(pDC, graphRegion, nGraphBackgroundGapX, nGraphBackgroundGapY, graphRegionColor.GetLineColor(), graphRegionFrameThickness);
		DrawGraphFrame(pDC, graphRegion, graphRegionFrameThickness, graphRegionColor.GetFrameColor());
	}
}

// 구분선을 그린다.
void CGraphFrameDrawerImplementation::DrawBlockCompartLine(CDC* pDC, const CPoint& startPt, const CPoint& endPt, const COLORREF& blockCompartLineColor)
{
	CPen newPen(PS_SOLID, 1, blockCompartLineColor);
	CPen* pOldPen = pDC->SelectObject(&newPen);

	DrawLine(pDC, startPt, endPt);
	
	pDC->SelectObject(pOldPen);
	newPen.DeleteObject();
}


// private ==============================================================================
// 영역 안 그리기.
void CGraphFrameDrawerImplementation::DrawGraphBackground(CDC* pDC, const CRect& region, const COLORREF& color)
{
	CBrush newBrush(color);
	CBrush* pOldBrush = pDC->SelectObject(&newBrush);

	// (2006/10/7 - Seung-Won, Bae) Do not use NULL_PEN to fill all without NULL Space.
	//		Caution! NULL_PEN makes the NULL Space in the right and bottom 1 Pixel.
	CPen newPen( PS_SOLID, 1, color);
	CPen* pOldPen = pDC->SelectObject( &newPen);
	pDC->Rectangle( region);
	pDC->SelectObject(pOldPen);
	newPen.DeleteObject();

	pDC->SelectObject(pOldBrush);
	newBrush.DeleteObject();
}

// 바둑판를 그린다.
void CGraphFrameDrawerImplementation::DrawGraphLine(CDC* pDC, CRect region, const int nGapX, const int nGapY, const COLORREF color, const int graphRegionFrameThickness)
{
	if(nGapX <= 0 && nGapY <= 0)
		return;

	if(graphRegionFrameThickness > 2) region.DeflateRect(graphRegionFrameThickness - 2, graphRegionFrameThickness - 2);


	CPen newPen(PS_SOLID|PS_INSIDEFRAME, 1, color);
	CPen* pOldPen = pDC->SelectObject(&newPen);

	if(nGapX > 0){
		for(int nStart = region.left +nGapX; nStart < region.right; nStart += nGapX)
			DrawLine(pDC, CPoint(nStart, region.top), CPoint(nStart, region.bottom));
	}
	if(nGapY > 0){
		for(int nStart = region.top +nGapY; nStart < region.bottom; nStart += nGapY)
			DrawLine(pDC, CPoint(region.left, nStart), CPoint(region.right, nStart));
	}

	pDC->SelectObject(pOldPen);
	newPen.DeleteObject();
}

// 테두리를 그린다.
void CGraphFrameDrawerImplementation::DrawGraphFrame(CDC* pDC, const CRect& region, const int nFrameThickness, const COLORREF color)
{
	if(nFrameThickness <= 0)
		return;

	CPen newPen( PS_INSIDEFRAME, nFrameThickness, color);
	CPen* pOldPen = pDC->SelectObject(&newPen);

	// (2006/10/8 - Seung-Won, Bae) Use Rectangle with NULL Brush instead of DrawRectangle().
	CBrush *pOldBrush = ( CBrush *)pDC->SelectStockObject( NULL_BRUSH);
	pDC->Rectangle( region);	//	DrawRectangle(pDC, region);
	pDC->SelectObject( pOldBrush);


	pDC->SelectObject(pOldPen);
	newPen.DeleteObject();
}

void CGraphFrameDrawerImplementation::DrawLine(CDC* pDC, const CPoint& startPt, const CPoint& endPt)
{
	pDC->MoveTo(startPt);
	pDC->LineTo(endPt);
}