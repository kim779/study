// Draw.cpp: implementation of the CDraw class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Draw.h"

#include "DataMath.h"
#include "Conversion.h"

#include "Draw.h"
#include "BaseData.h"

#include "PacketList.h"
#include "PacketBase.h"
#include "IndicatorList.h"
#include "IndicatorInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDraw::CDraw() :
	PI(3.1415926535897932384626433832795)
{
	// (2008/5/14 - Seung-Won, Bae) for Multi-Language of Special Date or Price Unit Text
	m_hOcxWnd = NULL;
}

CDraw::~CDraw()
{
}

//public ==========================================================================================
bool CDraw::DrawGraph(CDC* pDC,
			const CRect& realRegion, 
			const CDisplayAttributes& displayAttributes, 
			const CDrawingGraphData& drawingGraphData,
			const CSubGraphData& subGraphData)
{
	return false;
}

bool CDraw::DrawGraph(CDC* pDC,
		const CRect& realRegion, 
		const CDisplayAttributes& displayAttributes, 
		const CDrawingGraphData& drawingGraphData,
		const CSubGraphData& subGraphData,
		const CString& strRQ, 
		CXScaleManager* pxScaleManager)
{
	return false;
}

//protected =======================================================================================
void CDraw::DrawLine(CDC* pDC, POINT startPt, POINT endPt)
{
	pDC->MoveTo(startPt);
	pDC->LineTo(endPt);
}

void CDraw::DrawLine(CDC* pDC, POINT startPt, long endX, long endY)
{
	pDC->MoveTo(startPt);
	pDC->LineTo(endX, endY);
}

void CDraw::DrawLine(CDC* pDC, CPen* pPen, const CPoint& startPt, const CPoint& endPt)
{
	CPen* pOldPen = NULL;
	if(pPen != NULL)
		pOldPen = (CPen*)pDC->SelectObject(pPen);

	DrawLine(pDC, startPt, endPt);

	if(pOldPen != NULL)
		pDC->SelectObject(pOldPen);
}

void CDraw::DrawLine(CDC* pDC, const CPoint& startPt, const CPoint& endPt, COLORREF penColor, int nLineThickness)
{
	CPen newPen(PS_SOLID, nLineThickness, penColor);
	CPen* pOldPen = (CPen*)pDC->SelectObject(&newPen);
	
	DrawLine(pDC, startPt, endPt);

	pDC->SelectObject(pOldPen);
	newPen.DeleteObject();
}

void CDraw::DrawRectangle(CDC* pDC, CRect rect)
{
	rect.NormalizeRect();	//sy
	rect.bottom += 1;
	pDC->Rectangle(rect);
	rect.bottom -= 1;
}

void CDraw::DrawRectangle(CDC* pDC, CRect rect, const COLORREF& penColor, const int nPenWidth, const int nPenStyle, const bool bIsNullBrush)
{
	CBrush* pOldBrush = NULL;
	if(bIsNullBrush)
		pOldBrush = (CBrush*) pDC->SelectStockObject(NULL_BRUSH);

	CPen newPen(nPenStyle, nPenWidth, penColor);
	CPen* pOldPen = (CPen*) pDC->SelectObject(&newPen);
	
	DrawRectangle(pDC, rect);
	
	if(pOldBrush != NULL)
		pDC->SelectObject(pOldBrush);

	pDC->SelectObject(pOldPen);
	newPen.DeleteObject();
}

void CDraw::DrawPolyLine(CDC* pDC, POINT *lppt, int cPoints, int nLineWidth)
{
	if( !pDC || !lppt) return;

	// (2007/1/18 - Seung-Won, Bae) Check Dot Process
	if( 1 == cPoints)
	{
		pDC->MoveTo( lppt[ 0]);
		pDC->LineTo( lppt[ 0].x + 1, lppt[0].y);
		return;
	}

	/* example 
		cPoints : 8
		nLimit	: 2

				nCurPos		nDrawCount
		Step 1		1			2
		Step 2		3			2
		Step 3		5			2
		Step 4		7			1
	*/

	// (2007/1/18 - Seung-Won, Bae) Draw Line with Zero Base for avoiding the drop of First Line.
	int nLimit = 0;
	if( nLineWidth == 1)	nLimit = POLYLINE_LIMIT_WIDTH_1;
	else					nLimit = POLYLINE_LIMIT_WIDTH_GT_1;

	int nCurPos = 0;
	int nAllCount = cPoints;
	int nDrawCount = nLimit;
	while( 1 < nAllCount)
	{
		if( nAllCount < nDrawCount) nDrawCount = nAllCount;

		pDC->Polyline( &lppt[ nCurPos], nDrawCount);

		nCurPos += nDrawCount - 1;
		nAllCount -= nDrawCount - 1;
	}
}

// Windows 95/98/Me 에서는 lppt 의 갯수의 제한이 있습니다.
// 그에 대한 처리를 위해서 작업을 하였으나 현재 해당 OS를 사용하는 경우가 드물어
// 보다 빨리 처리하기 위해서 해당 함수를 바로 호출합니다.
//void CDraw::DrawPolyPolyLine(CDC* pDC, POINT *lppt, DWORD *lpdwPolyPoints, DWORD cCount, int nLineWidth)
//{
//	pDC->PolyPolyline(lppt, lpdwPolyPoints, cCount);
//}

void CDraw::TextOut(CDC* pDC, CRect textRegion, const CString& strText, 
		const bool bIsFullText, const bool bIsVertCenter, const bool bIsHorzCenter)
{
	if(strText.IsEmpty())
		return;

	CSize size = pDC->GetTextExtent(strText);
	if(bIsFullText)
	{
		if(textRegion.Width() < size.cx)
		{
			int nOffset_x = (size.cx - textRegion.Width())/2 + 1;
			if(nOffset_x > 0)
				textRegion.InflateRect(nOffset_x, 0);
		}
	}

	if(bIsVertCenter)
	{
		if(textRegion.Height() > size.cy)
		{
			int nOffset_y = (textRegion.Height() - size.cy)/2;
			// ',' 은 1픽셀을 더 소요한다.
			if(strText.Find(',') < 0)
				nOffset_y += 1;

			if(nOffset_y > 1)
				textRegion.DeflateRect(0, nOffset_y);
		}
	}

	if(bIsHorzCenter)
		pDC->DrawText(strText, textRegion, DT_VCENTER | DT_CENTER);
	else
		pDC->DrawText(strText, textRegion, DT_VCENTER);
}

void CDraw::TextOut(CDC* pDC, const CPoint& point, const CString& strText, const COLORREF& textColor, const int nBkMode)
{
	if(strText.IsEmpty())
		return;

	int nOldBkMode = pDC->SetBkMode(nBkMode);
	COLORREF oldTextColor = pDC->SetTextColor(textColor);

	pDC->TextOut(point.x, point.y, strText);

	pDC->SetBkMode(nOldBkMode);
	pDC->SetTextColor(oldTextColor);
}

void CDraw::TextOut_MultiLine(CDC* pDC, const CPoint& textStartPt, const int nLineGap, 
		const CList<CString, CString>& strTextList, const COLORREF& textColor, const int nBkMode)
{
	if(strTextList.GetCount() <= 0)
		return;

	int nOldBkMode = pDC->SetBkMode(nBkMode);
	COLORREF oldTextColor = pDC->SetTextColor(textColor);

	int nTextHeight = pDC->GetTextExtent("0").cy;
	int nPrevTextStart_y = textStartPt.y;
	POSITION pos = strTextList.GetHeadPosition();
	while(pos != NULL){
		CString strText = strTextList.GetNext(pos);
		pDC->TextOut(textStartPt.x, nPrevTextStart_y, strText);
		nPrevTextStart_y += (nTextHeight + nLineGap);
	}

	pDC->SetBkMode(nOldBkMode);
	pDC->SetTextColor(oldTextColor);
}

int CDraw::CalUpLineThickness(const CPenThickness& penThickness, int barWidth)
{
	if(barWidth > penThickness.GetDrawingThickness1() * 2)  
		return penThickness.GetDrawingThickness1();
	return 1;
}

int CDraw::CalDownLineThickness(const CPenThickness& penThickness, int barWidth)
{
	int lineThickness = 1;
	
	if(barWidth > penThickness.GetDrawingThickness2() * 2)  
		lineThickness = penThickness.GetDrawingThickness2();
	return lineThickness;

}
int CDraw::CalStationaryLineThickness(const CPenThickness& penThickness, int barWidth)
{
	int lineThickness = 1;
	
	if(barWidth > penThickness.GetDrawingThickness3() * 2)  
		lineThickness = penThickness.GetDrawingThickness3();
	return lineThickness;
}

double CDraw::CalRealWidth(const CRect& graphDrawingRegion, const CDisplayAttributes& displayAttributes)
{
	// (2004.07.19, 배승원) 0으로 나누는 경우를 예외처리한다. (0으로 Return한다.)
	double dDataCount = displayAttributes.GetDataCountInBlock();
	if( dDataCount <= 0) return 0;
	return (double)graphDrawingRegion.Width()/dDataCount;
}

double CDraw::CalRealWidth(const CRect& graphDrawingRegion, const int dataStart, const int dataEnd)
{
	// (2004.07.19, 배승원) 0으로 나누는 경우를 예외처리한다. (0으로 Return한다.)
	double dDataCount = dataEnd - dataStart + 1;
	if( dDataCount <= 0) return 0;
	return (double)graphDrawingRegion.Width()/dDataCount;
}

double CDraw::CalInclination(CPoint startPoint, CPoint endPoint)
{
	if(endPoint.x - startPoint.x == 0)
		return 10000.0;
	else
		return double(endPoint.y - startPoint.y) / double(endPoint.x - startPoint.x);
}

double CDraw::CalLogValue(bool checkLog, double data)
{
	if(checkLog)
		return CMath::Log(data);

	return double(data);
}

void CDraw::DrawSemiTransparentBitmap(CDC *pDC, CPoint* ptArr, COLORREF rgb)
{
	CBrush brush(rgb), *pOldBrush=NULL;
	CRgn rgn;
	rgn.CreatePolygonRgn(ptArr, 4, WINDING);
	CDC CompatibleDC;
	CompatibleDC.CreateCompatibleDC(pDC);
	CRect rgnRect, OriginalRect;
	rgn.GetRgnBox(&rgnRect);
	OriginalRect = rgnRect;
	rgn.OffsetRgn(-OriginalRect.left, -OriginalRect.top);
	rgnRect.bottom -= rgnRect.top;
	rgnRect.right -= rgnRect.left;
	rgnRect.top = 0;
	rgnRect.left = 0;
	CBitmap bm, *pOldBitmap=NULL;
	bm.CreateCompatibleBitmap(pDC, rgnRect.Width(), rgnRect.Height());
	pOldBrush = CompatibleDC.SelectObject(&brush);
	pOldBitmap = CompatibleDC.SelectObject(&bm);

	CompatibleDC.FillSolidRect(CRect(0,0, rgnRect.Width(), rgnRect.Height()), RGB(255, 255, 255));
	CompatibleDC.PaintRgn(&rgn);
	pDC->BitBlt(OriginalRect.left, OriginalRect.top, OriginalRect.Width(), OriginalRect.Height(), &CompatibleDC, 0, 0, SRCAND);
	
	CompatibleDC.SelectObject(pOldBitmap);
	bm.DeleteObject();
	CompatibleDC.SelectObject(pOldBrush);
	brush.DeleteObject();
	CompatibleDC.DeleteDC();
	rgn.DeleteObject();
}

void CDraw::InsertFourPointToArray(CPoint* point, CPoint point0, CPoint point1, CPoint point2, CPoint point3)
{
	point[0] = point0;
	point[1] = point1;
	point[2] = point2;
	point[3] = point3;
}

void CDraw::InsertThreePointToArray(CPoint* point, CPoint point0, CPoint point1, CPoint point2)
{
	point[0] = point0;
	point[1] = point1;
	point[2] = point2;
}

void CDraw::CreatePen(CPen& pen, const CGraphDrawStyle& drawStyle, int penThickness, COLORREF color)
{
	if(drawStyle.GetGraphLineDrawStyle() == CGraphBaseData::Dot_Draw)			  // 좌표점형
		pen.CreatePen(PS_DOT, penThickness, color);
	else if(drawStyle.GetGraphLineDrawStyle() == CGraphBaseData::DottedLine_Draw) // 선형 점선
		pen.CreatePen(PS_DASH, penThickness, color);
	else if(drawStyle.GetGraphLineDrawStyle() == CGraphBaseData::DashDot_Draw)	  // 선점형
		pen.CreatePen(PS_DASHDOT, penThickness, color);
	else if(drawStyle.GetGraphLineDrawStyle() == CGraphBaseData::DashDotDot_Draw) // 선점점형
		pen.CreatePen(PS_DASHDOTDOT, penThickness, color);
	else
		pen.CreatePen(PS_SOLID, penThickness, color);
}

// 기준선 두께, 모양 설정 - ojtaso (20071213)
void CDraw::CreatePen(CPen& pen, const CGraphBaseLineStyle& drawStyle, int penThickness, COLORREF color)
{
	if(drawStyle.GetBaseLineDrawStyle() == CGraphBaseData::Dot_Draw)			  // 좌표점형
		pen.CreatePen(PS_DOT, penThickness, color);
	else if(drawStyle.GetBaseLineDrawStyle() == CGraphBaseData::DottedLine_Draw) // 선형 점선
		pen.CreatePen(PS_DASH, penThickness, color);
	else if(drawStyle.GetBaseLineDrawStyle() == CGraphBaseData::DashDot_Draw)	  // 선점형
		pen.CreatePen(PS_DASHDOT, penThickness, color);
	else if(drawStyle.GetBaseLineDrawStyle() == CGraphBaseData::DashDotDot_Draw) // 선점점형
		pen.CreatePen(PS_DASHDOTDOT, penThickness, color);
	else
		pen.CreatePen(PS_SOLID, penThickness, color);
}

//sy 2005.09.12.
void CDraw::DrawArrow(CDC* pDC, CPoint point, BOOL bSell, BOOL upDown, SIZE strDataSize, const COLORREF color, const int nArrowLength, const int nArrowHight)
{
	CBrush newBrush(color);
	CPen newPen(PS_SOLID, 1, color);
	CBrush* pOldBrush = pDC->SelectObject(&newBrush);
	CPen* pOldPen = pDC->SelectObject(&newPen);

	DrawArrow(pDC, point, bSell, upDown, strDataSize, nArrowLength, nArrowHight);
	
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
	newPen.DeleteObject();
	newBrush.DeleteObject();
}

//sy 2004.04.14. -> 화살표의 사이즈를 조절할 수 있게 한다.
void CDraw::DrawArrow(CDC* pDC, CPoint point, BOOL bSell, BOOL upDown, SIZE strDataSize, const int nArrowLength, const int nArrowHight)
{
	int nMargin = 2; // 화살표의 gap
	int nArrowTriangle = 2; // 화살표의 삼각 부분의 사이즈.

	CPoint pPoint[9];
	pPoint[0] = point;
	if(upDown){
		pPoint[0].y -= nMargin;
		pPoint[7].y = pPoint[2].y = pPoint[8].y = pPoint[1].y = pPoint[0].y - nArrowTriangle;
		if(!bSell){
			pPoint[4].y = pPoint[3].y = pPoint[0].y - nArrowHight;	
			pPoint[5].y = pPoint[6].y = pPoint[0].y - nArrowHight;
			pPoint[4].x = pPoint[5].x = pPoint[0].x + nArrowLength;
		}
		else{
			pPoint[4].y = pPoint[3].y = pPoint[0].y - nArrowHight;	
			pPoint[5].y = pPoint[6].y = pPoint[0].y - nArrowHight;
			pPoint[4].x = pPoint[5].x = pPoint[0].x - nArrowLength;
		}
	}
	else{
		pPoint[0].y += nMargin;
		pPoint[7].y = pPoint[2].y = pPoint[8].y = pPoint[1].y = pPoint[0].y + (nMargin + nArrowTriangle + 1);
		if(!bSell){
			pPoint[4].y = pPoint[3].y = pPoint[0].y + nArrowHight;	
			pPoint[5].y = pPoint[6].y = pPoint[0].y + nArrowHight;
			pPoint[4].x = pPoint[5].x = pPoint[0].x + nArrowLength;
		}
		else{
			pPoint[4].y = pPoint[3].y = pPoint[0].y + nArrowHight;	
			pPoint[5].y = pPoint[6].y = pPoint[0].y + nArrowHight;
			pPoint[4].x = pPoint[5].x = pPoint[0].x - nArrowLength;
		}
	}

	pPoint[1].x = pPoint[0].x - nArrowTriangle;
	pPoint[2].x = pPoint[3].x = pPoint[0].x;
	pPoint[6].x = pPoint[7].x = pPoint[0].x;
	pPoint[8].x = pPoint[0].x + nArrowTriangle;

	pDC->Polygon(pPoint, 9);
}

void CDraw::DrawArrow(CDC* pDC, CPoint point, BOOL upDown)
{
	CPoint pPoint[7];
	pPoint[0] = point;

	if(!upDown){
		pPoint[5].y = pPoint[2].y = pPoint[6].y = pPoint[1].y = pPoint[0].y - 5;
		pPoint[4].y = pPoint[3].y = pPoint[0].y - 11;	
	}
	else{
		pPoint[1].y = pPoint[2].y = pPoint[5].y = pPoint[6].y = pPoint[0].y + 5;
		pPoint[3].y = pPoint[4].y = pPoint[0].y + 11;
	}

	pPoint[1].x = pPoint[0].x - 3;
	pPoint[2].x = pPoint[3].x = pPoint[0].x - 1;
	pPoint[4].x = pPoint[5].x = pPoint[0].x + 1;
	pPoint[6].x = pPoint[0].x + 3;

	pDC->Polygon(pPoint, 7);
}

// 2002.8.24.
void CDraw::DrawTriangle(CDC* pDC, CPoint point, COLORREF color)
{
	int nHeight = 7;

	CBrush brush(color);
	CBrush* pOldBrush = pDC->SelectObject(&brush);

	CPoint pt[3];
	int i = 0;
	pt[i].x 	= (int)(point.x);
	pt[i++].y 	= (int)(point.y - nHeight / 2);
	pt[i].x 	= (int)(point.x - nHeight / 2);
	pt[i++].y 	= (int)(point.y + nHeight / 2);
	pt[i].x 	= (int)(point.x + nHeight / 2);
	pt[i++].y 	= (int)(point.y + nHeight / 2);
	
	pDC->Polygon(pt, 3);

	pDC->SelectObject(pOldBrush);

	///김찬중...12월20일
	brush.DeleteObject();
}
//end

POSITION CDraw::FindPosition(int nIndex, CList<int, int>& dataIndex)
{
	int nTailIndex = dataIndex.GetCount() -1;
	if(nIndex < 0 || nIndex > nTailIndex)
		return NULL;

	if(nIndex > nTailIndex/2){
		int nCount = nTailIndex;
		POSITION pos = dataIndex.GetTailPosition();
		while(pos != NULL){
			if(nCount-- == nIndex)
				return pos;
			dataIndex.GetPrev(pos);
		}
	}
	else{
		int nCount = 0;
		POSITION pos = dataIndex.GetHeadPosition();
		while(pos != NULL){
			if(nCount++ == nIndex)
				return pos;
			dataIndex.GetNext(pos);
		}
	}

	return NULL;
}

int CDraw::DrawBaseLineAndTexts( const CRect& graphRegion, CPacket *p_pBasePacket,
								 const double& dViewMax, const double& dViewMin, const double& dLoggedViewMax, const double& dLoggedViewMin,
								 bool bLog, bool bInvert)	// 기준선 보임 환경설정과 Scale 자동포함 기능 - ojtaso (20071101)
{
	if( !p_pBasePacket) return INT_MAX;

	double dBase	= p_pBasePacket->GetStandardValue( 0);
	double dUBase	= p_pBasePacket->GetStandardValue( 1);
	double dLBase	= p_pBasePacket->GetStandardValue( 2);

	int nBase = 0;
	CString strPacketType = p_pBasePacket->GetType();
	CString strBase, strUBase, strLBase;
	if( dBase == -HUGE_VAL) return INT_MAX;

	strBase.Format( "%f", dBase);
	strBase = CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans( strBase, strPacketType);
	nBase = CDataConversion::GetChangingPosition( graphRegion.bottom, graphRegion.Height(), dLoggedViewMin, dLoggedViewMax - dLoggedViewMin, CDraw::CalLogValue( bLog, dBase));
	if( bInvert) nBase = CDataConversion::InvertGraphDataPt( graphRegion, nBase);
	return nBase;
}

// (2007/1/14 - Seung-Won, Bae) Draw BaseLine and Texts
// (2007/2/22 - Seung-Won, Bae) Support Text Hiding Option
int CDraw::DrawBaseLineAndTexts( CDC *p_pDC, const CRect& graphRegion, const CGraphColor& graphColor, CPacket *p_pBasePacket,
								const double& dViewMax, const double& dViewMin, const double& dLoggedViewMax, const double& dLoggedViewMin,
								bool bLog, bool bInvert, BOOL p_bWithText, bool bBaseLine)	// 기준선 보임 환경설정과 Scale 자동포함 기능 - ojtaso (20071101)
{
	if( !p_pDC || !p_pBasePacket) return 0;
	
	int	nBkMode = p_pDC->SetBkMode( TRANSPARENT);
	
	double dBase	= p_pBasePacket->GetStandardValue( 0);
	double dUBase	= p_pBasePacket->GetStandardValue( 1);
	double dLBase	= p_pBasePacket->GetStandardValue( 2);
	
	int nBase = 0;
	CString strPacketType = p_pBasePacket->GetType();
	CString strBase, strUBase, strLBase;
	if( dBase != -HUGE_VAL && dViewMin <= dBase && dBase <= dViewMax)
	{
		strBase.Format( "%f", dBase);
		strBase = CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans( strBase, strPacketType);
	}
	else if( dBase < dViewMin) nBase = graphRegion.bottom;
	else if( dViewMax < dBase) nBase = graphRegion.top;
	
	if( dUBase != -HUGE_VAL)
	{
		if(dUBase == 0)
			strUBase = "";
		else
			strUBase.Format( "%f", dUBase);
		strUBase = CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans( strUBase, strPacketType);
	}
	if( dLBase != -HUGE_VAL)
	{
		strLBase.Format( "%f", dLBase);
		strLBase = CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans( strLBase, strPacketType);
	}
	
	CSize sizUBase = p_pDC->GetTextExtent( strUBase);
	CSize sizLBase = p_pDC->GetTextExtent( strLBase);
	COLORREF clrOldText = p_pDC->GetTextColor();
	
	if( !strBase.IsEmpty())
	{
		nBase = CDataConversion::GetChangingPosition( graphRegion.bottom, graphRegion.Height(), dLoggedViewMin, dLoggedViewMax - dLoggedViewMin, CDraw::CalLogValue( bLog, dBase));
		if( bInvert) nBase = CDataConversion::InvertGraphDataPt( graphRegion, nBase);
		
		// 기준선 보임 환경설정과 Scale 자동포함 기능 - ojtaso (20071101)
		if(bBaseLine)
		{
			CDraw::DrawLine( p_pDC, CPoint( graphRegion.left, nBase), CPoint( graphRegion.right, nBase));
			
			if( p_bWithText)
			{
				CSize sizBase = p_pDC->GetTextExtent( strBase);
				p_pDC->SetTextColor( graphColor.GetColor6());
				p_pDC->TextOut( graphRegion.left + ( sizUBase.cx < sizLBase.cx ? sizLBase.cx : sizUBase.cx) + 5, ( nBase - sizBase.cy < graphRegion.top ? nBase + 2 : nBase - sizBase.cy), strBase);
			}
		}
	}
	
	if( p_bWithText)
	{
		if( !strUBase.IsEmpty())
		{
			p_pDC->SetTextColor( graphColor.GetColor1());
			p_pDC->TextOut( graphRegion.left + ( sizUBase.cx < sizLBase.cx ? sizLBase.cx - sizUBase.cx : 0), graphRegion.top + 1, strUBase);
		}
		
		if( !strLBase.IsEmpty())
		{
			p_pDC->SetTextColor( graphColor.GetColor3());
			p_pDC->TextOut( graphRegion.left + ( sizLBase.cx < sizUBase.cx ? sizUBase.cx - sizLBase.cx : 0), graphRegion.bottom  - sizUBase.cy, strLBase);
		}
	}
	
	p_pDC->SetTextColor( clrOldText);
	p_pDC->SetBkMode( nBkMode);
	
	return nBase;
}

// (2007/1/15 - Seung-Won, Bae) Draw Price Min/Max Text
const int g_nArrowLength = 11;
void CDraw::GetPriceMinMaxTime( CString &strTime, CPacket *pDataPacket, const int p_nIndex)
{
	strTime.Empty();
	// (2007/1/17 - Seung-Won, Bae) Do not support the Time Info in Line Graph.
	if( !pDataPacket || m_eGraphType == CGraphBaseData::Line_Type
		|| ( m_eGraphType == CGraphBaseData::Bong_Type && m_graphDrawStyle.GetDrawStyle() == CGraphBaseData::Line_Draw)) return;

	CPacket * pDatePacket = pDataPacket->GetDatePacket();
	if( !pDatePacket) return;

	POSITION datePos = pDatePacket->GetnumericDataList()->FindIndex( p_nIndex);
	if( !datePos) return;

	CList<double, double>* pDateList = pDatePacket->GetnumericDataList();
	if( !pDateList || datePos == NULL) return;

	double date = pDateList->GetNext( datePos);
	CString datePacketType = pDatePacket->GetType();

	// 2011.02.08 by onlyojt >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
	// 추세선 공유 기능 수정 - onlyojt
	CDataConversion dataConversion( m_hOcxWnd, m_eChartMode);
	if (datePacketType == _T("YYYYMMDDHHMMSS"))
	{
		int nDataUnit = pDatePacket->GetDateUnitType();
		strTime = " (" + dataConversion.GetStringFormatTrans2( CDataConversion::DoubleToString( date), nDataUnit) + ")";
	}
	else
	// 2011.02.08 by onlyojt <<
	{
		if( pDatePacket->GetDateUnitType() == CScaleBaseData::HORZ_MONTHLY)
		{
			if( CDataConversion::DoubleToString( date).GetLength() >= 8.0) date = date/100.0;
			datePacketType = datePacketType.Left(6);
		}
		strTime = " (" + dataConversion.GetStringFormatTrans( CDataConversion::DoubleToString( date), datePacketType) + ")";
	}
}
void CDraw::DrawPriceMaxText( CDC *pDC, const CRect& graphRegion, const CGraphColor& graphColor, const CString &strData, const int nViewX, const int nViewY, const bool &bInvert)
{
	COLORREF textClr = pDC->SetTextColor(graphColor.GetColor1());
	CSize size = pDC->GetTextExtent( strData);

	bool bToLeft = ( ( graphRegion.left + graphRegion.right) / 2 < nViewX);
	if( bToLeft)
	{
		if( bInvert)
			pDC->TextOut(nViewX - g_nArrowLength - 2 - size.cx, nViewY + 5, strData);
		else
			pDC->TextOut(nViewX - g_nArrowLength - 2 - size.cx, nViewY - size.cy -3, strData);
	}
	else
	{
		if( bInvert)
			pDC->TextOut(nViewX + g_nArrowLength + 2, nViewY +5, strData);
		else
			pDC->TextOut(nViewX + g_nArrowLength + 2, nViewY - size.cy -3, strData);
	}

	//pDC->SelectObject(&brushArray[UP_BRUSH]);
	//pDC->SelectObject(&penArray[UP_PEN]);
	DrawArrow( pDC, CPoint( nViewX, nViewY), bToLeft, !bInvert, size, graphColor.GetColor1());

	pDC->SetTextColor(textClr);
}
void CDraw::DrawPriceMinText( CDC *pDC, const CRect& graphRegion, const CGraphColor& graphColor, const CString &strData, const int nViewX, const int nViewY, const bool &bInvert)
{
	COLORREF textClr = pDC->SetTextColor(graphColor.GetColor3());
	CSize size = pDC->GetTextExtent(strData);

	bool bToLeft = ( ( graphRegion.left + graphRegion.right) / 2 < nViewX);
	if( bToLeft)
	{
		if(bInvert)
			pDC->TextOut(nViewX - g_nArrowLength - 2 - size.cx, nViewY - size.cy -2, strData);
		else
			pDC->TextOut(nViewX - g_nArrowLength - 2 - size.cx, nViewY +2, strData);
	}
	else
	{
		if(bInvert)
			pDC->TextOut(nViewX + g_nArrowLength + 2, nViewY - size.cy -3, strData);
		else
			pDC->TextOut(nViewX + g_nArrowLength + 2, nViewY +5, strData);
	}

	//pDC->SelectObject(brushArray[DOWN_BRUSH]);
	//pDC->SelectObject(penArray[DOWN_PEN]);
	DrawArrow(pDC, CPoint(nViewX, nViewY), bToLeft, bInvert, size, graphColor.GetColor3());

	pDC->SetTextColor(textClr);
}

// ----------------------------------------------------------------------------
///	수평선(대비선)과 임의의 선분의 교차점을 구한다 : 비례식을 이용하면 쉽게 계산 가능하다.
const int CDraw::CalcMidPoint( const int &nBase, const CPoint& lPt, const CPoint& rPt)
{
	double X1=lPt.x, Y1=lPt.y, Y2=nBase, X3=rPt.x, Y3=rPt.y;
	return CMath::Round( ( Y2 -Y1) / ( Y3 - Y1) * ( X3 - X1)) + X1;
}

int CDraw::GetThickness(const int nThickness, const double& dWidth) const
{
	if(dWidth <= nThickness * 3)
		return 1;

	return nThickness;
}

// (2010/2/24 - Seung-Won, Bae) for Element Drawing.
void CDraw::DrawElement_DrawPolygon( CDC* pDC, const int &nOneLineMode, const int &nCountOfPoly,
									POINT *ptPolyPoints, const DWORD *dwPointCounts, int *nPointCounts, const int &nPenThick,
									const COLORREF &clrColor, const BOOL &bFill, const BOOL &bLineBar, const CGraphBaseData::LINEDRAWSTYLE &eLineStyle)
{
	CPen pnPen;
	if( bLineBar)
	{
		LOGBRUSH logBrush;
		logBrush.lbColor = clrColor;
		logBrush.lbStyle = BS_SOLID;
		pnPen.CreatePen( PS_SOLID | PS_GEOMETRIC | PS_ENDCAP_FLAT, nPenThick, &logBrush);
	}
	else
	{
		int nPenStyle = PS_SOLID;
		switch( eLineStyle)
		{
		case CGraphBaseData::Dot_Draw:			nPenStyle = PS_DOT;			// 좌표점형
			break;
		case CGraphBaseData::DottedLine_Draw:	nPenStyle = PS_DASH;		// 선형 점선
			break;
		case CGraphBaseData::DashDot_Draw:		nPenStyle = PS_DASHDOT;		// 선점형
			break;
		case CGraphBaseData::DashDotDot_Draw:	nPenStyle = PS_DASHDOTDOT;	// 선점점형
			break;
		}
		pnPen.CreatePen( nPenStyle, nPenThick, clrColor);
	}
	CPen *pOldPen = pDC->SelectObject( &pnPen);
	
	CBrush *pOldBrush = NULL;
	CBrush brBrush;
	HGDIOBJ hBrush = NULL;
	if( bFill && 1 != nOneLineMode)
	{
		brBrush.CreateSolidBrush( clrColor);
		pOldBrush = pDC->SelectObject( &brBrush);
	}
	else hBrush = pDC->SelectObject( GetStockObject( NULL_BRUSH));
	
	if( 1 < nCountOfPoly)
	{
		if( nOneLineMode)	pDC->PolyPolyline( ptPolyPoints, dwPointCounts, nCountOfPoly);
		else				pDC->PolyPolygon( ptPolyPoints, nPointCounts, nCountOfPoly);
	}
	else if( 1 == nCountOfPoly)
	{
		if( nOneLineMode)
		{
			if( 1 < dwPointCounts[ 0]) pDC->Polyline( ptPolyPoints, dwPointCounts[ 0]);
		}
		else
		{
			if( 2 < nPointCounts[ 0]) pDC->Polygon( ptPolyPoints, nPointCounts[ 0]);
		}
	}
	
	pDC->SelectObject( pOldPen);
	pnPen.DeleteObject();
	if( bFill && 1 != nOneLineMode)
	{
		pDC->SelectObject( pOldBrush);
		brBrush.DeleteObject();
	}
	else pDC->SelectObject( hBrush);
}