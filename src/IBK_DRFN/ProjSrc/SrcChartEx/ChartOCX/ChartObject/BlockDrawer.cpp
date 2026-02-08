// BlockDrawer.cpp: implementation of the CBlockDrawer class.
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BlockDrawer.h"

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////
// class CBlockDrawer

CBlockDrawer::CBlockDrawer():
	HEIGHT(8)
{
}

CRect CBlockDrawer::GetBlockMoveRegion(const CPoint& topLeft) const
{
	CPoint pt = CPoint(topLeft.x + 1, topLeft.y + 1);
	return CRect(pt.x, pt.y, pt.x +(HEIGHT *3/2), pt.y +HEIGHT);
}

CRect CBlockDrawer::GetBlockDeleteRegion(const CPoint& topRight) const
{
	CPoint pt = CPoint(topRight.x - 1, topRight.y + 1);
	return CRect(pt.x -(HEIGHT *4/3), pt.y, pt.x, pt.y +(HEIGHT *4/3));
}

// 블럭 최대화 추가 - ojtaso (20080519)
CRect CBlockDrawer::GetBlockSizeRegion(const CPoint& topRight) const
{
	CPoint pt = CPoint(topRight.x - 1, topRight.y + 1);
	return CRect(pt.x -(HEIGHT *4/3) * 2, pt.y, pt.x-(HEIGHT *4/3), pt.y +(HEIGHT *4/3));
}

// ----------------------------------------------------------------------------
void CBlockDrawer::DrawFrameRegion(CDC* pDC, const CRect& region, const COLORREF& penColor, const int penThickness)
{
	if(region.IsRectEmpty())
		return;

	CBrush* pOldBrush = (CBrush*) pDC->SelectStockObject(NULL_BRUSH);
	CPen newPen(PS_INSIDEFRAME, penThickness, penColor);
	CPen* pOldPen = pDC->SelectObject(&newPen);

	CRect realRegion = region;
	realRegion.bottom += 1;
	pDC->Rectangle(realRegion);

	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
	newPen.DeleteObject();
}

// ----------------------------------------------------------------------------
void CBlockDrawer::DrawBlockMove2DButton(CDC* pDC, const CPoint& topLeft, const COLORREF& penColor, const COLORREF& brushColor)
{
	Draw2DButton(pDC, GetBlockMoveRegion(topLeft), penColor, brushColor);
}

void CBlockDrawer::DrawBlockDelete2DButton(CDC* pDC, const CPoint& topRight, const COLORREF& penColor, const COLORREF& brushColor)
{
	CRect region = GetBlockDeleteRegion(topRight);
	Draw2DButton(pDC, region, penColor, brushColor);
	region.DeflateRect(1, 1);
	DrawX(pDC, region, penColor);
}

// 블럭 최대화 추가 - ojtaso (20080519)
void CBlockDrawer::DrawBlockSize2DButton(CDC* pDC, const CPoint& topRight, const COLORREF& penColor, const COLORREF& brushColor)
{
	CRect region = GetBlockSizeRegion(topRight);
	Draw2DButton(pDC, region, penColor, brushColor);
	region.DeflateRect(1, 1);
	
/*
	if(bMaximized)
		DrawRestore(pDC, region, penColor);
	else
*/
		DrawMax(pDC, region, penColor);
}

// ----------------------------------------------------------------------------
void CBlockDrawer::DrawBlockMove3DButton(CDC* pDC, const CPoint& topLeft, const bool bIsFlat, const bool bIsSelection)
{
	CRect region = GetBlockMoveRegion(topLeft);
	Draw3DButton(pDC, region, bIsFlat, bIsSelection);
	DrawPoints(pDC, region, RGB(255, 255, 255), true);
	DrawPoints(pDC, region, RGB(0, 0, 0), false);
}

void CBlockDrawer::DrawBlockDelete3DButton(CDC* pDC, const CPoint& topRight, const bool bIsFlat, const bool bIsSelection)
{
	CRect region = GetBlockDeleteRegion(topRight);
	Draw3DButton(pDC, region, bIsFlat, !bIsSelection);
	region.DeflateRect(1, 1);
	DrawX(pDC, region, RGB(110, 110, 110));
}


// private ====================================================================
void CBlockDrawer::Draw2DButton(CDC* pDC, const CRect& region, const COLORREF& penColor, const COLORREF& brushColor)
{
	CBrush newBrush(brushColor);
	CBrush* pOldBrush = pDC->SelectObject(&newBrush);
	CPen newPen(PS_INSIDEFRAME, 1, penColor);
	CPen* pOldPen = pDC->SelectObject(&newPen);

	pDC->Rectangle(region);

	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
	newBrush.DeleteObject();
	newPen.DeleteObject();
}

void CBlockDrawer::Draw3DButton(CDC* pDC, const CRect& region, const bool bIsFlat, const bool bIsUp)
{
	if(bIsUp)
		Draw3DButton_Up(pDC, region, bIsFlat);
	else
		Draw3DButton_Down(pDC, region, bIsFlat);
}

void CBlockDrawer::Draw3DButton_Up(CDC* pDC, const CRect& region, const bool bIsFlat)
{
	DrawRegion(pDC, region, GetFrameRegionColor(bIsFlat), RGB(217, 217, 217));
	CRect frameRegion = region;
	frameRegion.DeflateRect(1, 1);

	if(bIsFlat)
		DrawRegionFrame(pDC, frameRegion, RGB(255, 255, 255), true, true, false, false);
	else
		DrawRegionFrame(pDC, frameRegion, RGB(195, 195, 195), false, false, true, true);

	DrawRegionFrame(pDC, region, RGB(0, 0, 0), false, false, true, true);
}

void CBlockDrawer::Draw3DButton_Down(CDC* pDC, const CRect& region, const bool bIsFlat)
{
	DrawRegion(pDC, region, RGB(0, 0, 0), RGB(217, 217, 217));
	CRect frameRegion = region;
	frameRegion.DeflateRect(1, 1);

	if(bIsFlat)
		DrawRegionFrame(pDC, frameRegion, RGB(255, 255, 255), false, false, true, true);
	else{
		frameRegion.BottomRight().Offset(-1, -1);
		DrawRegionFrame(pDC, frameRegion, RGB(195, 195, 195), true, true, false, false);
	}

	DrawRegionFrame(pDC, region, GetFrameRegionColor(bIsFlat), false, false, true, true);
}

COLORREF CBlockDrawer::GetFrameRegionColor(const bool bIsFlat) const
{
	return (bIsFlat ? RGB(110, 110, 110): RGB(255, 255, 255));
}

// ----------------------------------------------------------------------------
void CBlockDrawer::DrawPoints(CDC* pDC, const CRect& region, const COLORREF& penColor, const bool bIsLightPoints)
{
	CPoint startPoint = GetDrawPointPosition(region, bIsLightPoints);
	if(!region.PtInRect(startPoint))
		return;

	CPen newPen(PS_INSIDEFRAME, 1, penColor);
	CPen* pOldPen = pDC->SelectObject(&newPen);

	while(IsDrawPoint(region, startPoint)){
		DrawLine(pDC, startPoint, CPoint(startPoint.x +1, startPoint.y));
		startPoint.x += 3;
	}

	pDC->SelectObject(pOldPen);
	newPen.DeleteObject();
}

bool CBlockDrawer::IsDrawPoint(const CRect& region, const CPoint& point) const
{
	return (point.x >= (region.left +3) && point.x < (region.right -3));
}

CPoint CBlockDrawer::GetDrawPointPosition(const CRect& region, const bool bIsLightPoints) const
{
	if(bIsLightPoints)
		return CPoint(region.left +3, region.top +region.Height()/2 -1);

	return CPoint(region.left +4, region.top +region.Height()/2);
}

// ----------------------------------------------------------------------------
void CBlockDrawer::DrawX(CDC* pDC, const CRect& region, const COLORREF& penColor)
{
	CRect drawingRegion = region;
	drawingRegion.DeflateRect(2, 2);

	CPen newPen(PS_INSIDEFRAME, 1, penColor);
	CPen* pOldPen = pDC->SelectObject(&newPen);

	DrawLine(pDC, drawingRegion.TopLeft(), drawingRegion.BottomRight());
	DrawLine(pDC, CPoint(drawingRegion.left, drawingRegion.bottom -1), CPoint(drawingRegion.right, drawingRegion.top -1));

	pDC->SelectObject(pOldPen);
	newPen.DeleteObject();
}

// 블럭 최대화 추가 - ojtaso (20080519)
void CBlockDrawer::DrawMax(CDC* pDC, const CRect& region, const COLORREF& penColor)
{
	CRect drawingRegion = region;
	drawingRegion.DeflateRect(1, 1);

	CPen newPen(PS_INSIDEFRAME, 1, penColor);
	CPen* pOldPen = pDC->SelectObject(&newPen);

	pDC->Draw3dRect(drawingRegion, penColor, penColor);
	DrawLine(pDC, CPoint(drawingRegion.left, drawingRegion.top + 1), CPoint(drawingRegion.right, drawingRegion.top + 1));

	pDC->SelectObject(pOldPen);
	newPen.DeleteObject();
}

// 블럭 최대화 추가 - ojtaso (20080519)
void CBlockDrawer::DrawRestore(CDC* pDC, const CRect& region, const COLORREF& penColor)
{
	CRect drawingRegion = region;
	drawingRegion.DeflateRect(1, 1);

	CPen newPen(PS_INSIDEFRAME, 1, penColor);
	CPen* pOldPen = pDC->SelectObject(&newPen);

	pDC->Draw3dRect(drawingRegion.left, drawingRegion.top, HEIGHT - 3, HEIGHT - 3, penColor, penColor);
	pDC->Draw3dRect(drawingRegion.left + 2, drawingRegion.top + 2, HEIGHT - 4, HEIGHT - 4, penColor, penColor);
//	DrawLine(pDC, CPoint(drawingRegion.left, drawingRegion.top + 3), CPoint(drawingRegion.right, drawingRegion.top + 3));

	pDC->SelectObject(pOldPen);
	newPen.DeleteObject();
}

// ----------------------------------------------------------------------------
void CBlockDrawer::DrawRegion(CDC* pDC, const CRect& region, const COLORREF& penColor, const COLORREF& brushColor)
{
	CBrush newBrush(brushColor);
	CBrush* pOldBrush = pDC->SelectObject(&newBrush);
	CPen newPen(PS_INSIDEFRAME, 1, penColor);
	CPen* pOldPen = pDC->SelectObject(&newPen);

	pDC->Rectangle(region);

	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
	newBrush.DeleteObject();
	newPen.DeleteObject();
}

void CBlockDrawer::DrawRegionFrame(CDC* pDC, const CRect& region, const COLORREF& penColor, 
		const bool bIsLeft, const bool bIsTop, const bool bIsRight, const bool bIsBottom)
{
	CPen newPen(PS_INSIDEFRAME, 1, penColor);
	CPen* pOldPen = pDC->SelectObject(&newPen);

	if(bIsLeft)
		DrawLine(pDC, region.TopLeft(), CPoint(region.left, region.bottom));
	if(bIsTop)
		DrawLine(pDC, region.TopLeft(), CPoint(region.right, region.top));
	if(bIsRight)
		DrawLine(pDC, CPoint(region.right -1, region.top), CPoint(region.right -1, region.bottom));
	if(bIsBottom)
		DrawLine(pDC, CPoint(region.left, region.bottom -1), CPoint(region.right, region.bottom -1));

	pDC->SelectObject(pOldPen);
	newPen.DeleteObject();
}

void CBlockDrawer::DrawLine(CDC* pDC, const CPoint& pt1, const CPoint& pt2)
{
	pDC->MoveTo(pt1);
	pDC->LineTo(pt2);
}