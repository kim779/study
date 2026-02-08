// AutoTrendDrawer.cpp: implementation of the CAutoTrendDrawer class.
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AutoTrendDrawer.h"

#include "AutoTrendData.h"
#include "AnalysisData.h."

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////
// class CAutoTrendDrawer

CAutoTrendDrawer::CAutoTrendDrawer():
	m_penColor(RGB(0, 0, 0)),
	m_nPenThickness(1)
{
}

void CAutoTrendDrawer::SetPenColor(const COLORREF& color)
{
	m_penColor = color;
}

void CAutoTrendDrawer::SetPenThickness(const int nThickness)
{
	m_nPenThickness = nThickness;
}

COLORREF CAutoTrendDrawer::GetPenColor() const
{
	return m_penColor;
}

int CAutoTrendDrawer::GetPenThickness() const
{
	return m_nPenThickness;
}

// ----------------------------------------------------------------------------
bool CAutoTrendDrawer::Drawing(CDC* pDC, const CAutoTrendDrawingCalculatorData& calData, const CPoint& point, const int nDrawMode)
{
	if(!calData.DoesCalculator(point))
		return false;

	CPen newPen(PS_INSIDEFRAME, m_nPenThickness, m_penColor);
	CPen* pOldPen = pDC->SelectObject(&newPen);
	int nOldDrawMode = pDC->SetROP2(nDrawMode);
	int oldBkMode = pDC->SetBkMode(TRANSPARENT);

	CDrawer::SelectClipRegion(pDC, calData.GetNoFrameGraphRegion());
	DrawShape(pDC, calData, point);
	CDrawer::SelectClipRegion(pDC);

	pDC->SelectObject(pOldPen);
	pDC->SetROP2(nOldDrawMode);
	pDC->SetBkMode(oldBkMode);
	newPen.DeleteObject();
	return true;
}


// private ====================================================================
// leftPt  : 현재 mouse point 의 가장 최근의 index
// rightPt : 현재 mouse point 의 가장 최근 이전의 index
// -> 왼쪽 부터 "leftPt - rightPt - mouse point" 순
void CAutoTrendDrawer::DrawShape(CDC* pDC, const CAutoTrendDrawingCalculatorData& calData, const CPoint& point)
{
	CRegion region = GetRegion(calData.GetCoordinateCalculatorData());
	int nCurrentDataIndex = CDrawer::GetCurrentDataIndex(region, point);
	if(nCurrentDataIndex < 0)
		return;

	CAutoTrendDrawingCalculator calculator;
	CPoint high_leftPt, high_rightPt, low_leftPt, low_rightPt;
	if(!calculator.GetDrawingPoints(calData, nCurrentDataIndex, high_leftPt, high_rightPt, low_leftPt, low_rightPt))
		return;

	CPoint realHigh_rightPt(high_rightPt), realLow_righPt(low_rightPt);
	DrawLine(pDC, region, nCurrentDataIndex, high_leftPt, high_rightPt, realHigh_rightPt);
	DrawLine(pDC, region, nCurrentDataIndex, low_leftPt, low_rightPt, realLow_righPt);
	DrawText(pDC, calData, nCurrentDataIndex, high_rightPt, low_rightPt, realHigh_rightPt, realLow_righPt);
}

void CAutoTrendDrawer::DrawLine(CDC* pDC, const CRegion& region, const int nCurrentIndex, const CPoint& leftPt, const CPoint& rightPt, CPoint& realRightPt)
{
	// 현재 mouse point 에 관한 교점
	realRightPt = GetPoint(region, nCurrentIndex, leftPt, rightPt);

	// 가장 과거 data 의 -3 번째에 관한 교점
	int nDataIndex = CDrawer::GetCurrentDataIndex(region, leftPt);
	CPoint realLeftPt = GetPoint(region, nDataIndex -3, leftPt, rightPt);
	
	CDrawer::DrawLine(pDC, realLeftPt, realRightPt);
}

void CAutoTrendDrawer::DrawText(CDC* pDC, const CAutoTrendDrawingCalculatorData& calData, const int nCurrentDataIndex, 
		const CPoint& highPt, const CPoint& lowPt, const CPoint& realHighPt, const CPoint& realLowPt)
{
	CRect textRegion = GetTextRegion(pDC->GetTextExtent("L"), calData.GetNoFrameGraphRegion());
	if(textRegion.IsRectEmpty() || !calData.GetNoFrameGraphRegion().PtInRect(textRegion.TopLeft()))
		return;

	int nOldDrawMode = pDC->SetROP2(R2_COPYPEN);
	CDrawer::DrawRectangle(pDC, textRegion, RGB(255, 0, 0), RGB(255, 255, 255), true);
	CDrawer::DrawTextOut(pDC, GetText(calData, nCurrentDataIndex, highPt, lowPt), textRegion);
	pDC->SetROP2(nOldDrawMode);
}

// ----------------------------------------------------------------------------
CRegion CAutoTrendDrawer::GetRegion(const CCoordinateCalculatorData& calData) const
{
	return CRegion(calData.GetRegion(), CDisplayAttributes(calData.GetDisplayDataStart(), calData.GetDisplayDataEnd()));
}

CPoint CAutoTrendDrawer::GetPoint(const CRegion& region, const int nDataIndex, const CPoint& line1_pt1, const CPoint& line1_pt2) const
{
	int nX = CDrawer::GetDrawingPoint_X(region, nDataIndex);
	CPoint line2_pt1(nX, region.GetRect().top), line2_pt2(nX, region.GetRect().bottom);
	return CDrawer::GetLineEndPoint(line1_pt1, line1_pt2, line2_pt1, line2_pt2);
}

CRect CAutoTrendDrawer::GetTextRegion(const CSize& size, const CRect& noFrameGraphRegion) const
{
	CRect region(noFrameGraphRegion.BottomRight(), CSize(-MARGIN, -MARGIN));
	region.NormalizeRect();
	region.InflateRect((size.cx * 18), size.cy, 0, 0);
	return region;
}

CString CAutoTrendDrawer::GetText(const CAutoTrendDrawingCalculatorData& calData, 
		const int nCurrentDataIndex, const CPoint& highPt, const CPoint& lowPt) const
{
	if(calData.GetCoordinateCalculatorData().GetDisplayDataStart() > nCurrentDataIndex)
		return "";

	CPoint point(highPt);
	if(highPt.x < lowPt.x)
		point.x = lowPt.x;

	int nStartIndex = CDrawer::GetCurrentDataIndex(GetRegion(calData.GetCoordinateCalculatorData()), point);
	CAutoTrendDrawingCalculator calculator;
	IPacket *pIPacket = calData.GetPacket( 0);
	CString strResult = calculator.GetText( pIPacket, nStartIndex, nCurrentDataIndex);
	if( pIPacket) pIPacket->Release();
	return strResult;
}

/*void CAutoTrendDrawer::GetTextFont(CDC* pDC, CFont& font) const
{
	LOGFONT lFont;
	CFont* pCurFont = pDC->GetCurrentFont();
	if(pCurFont == NULL)
		GetObject(GetStockObject(ANSI_VAR_FONT), sizeof lFont, &lFont);
	else
		pCurFont->GetLogFont(&lFont);

	lFont.lfHeight = 18;
	//strcpy(lFont.lfFaceName, "Arial"); // 궁서체로 font setting
	font.CreateFontIndirect(&lFont);
}*/
