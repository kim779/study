// Draw.h: interface for the CDraw class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAW_H__72586866_9926_4EDD_92DA_F0D6CAD746F6__INCLUDED_)
#define AFX_DRAW_H__72586866_9926_4EDD_92DA_F0D6CAD746F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "BlockBaseData.h"

#define POLYLINE_LIMIT_WIDTH_1		16000
#define POLYLINE_LIMIT_WIDTH_GT_1	1300

class CDisplayAttributes;
class CDrawingGraphData;
class CSubGraphData;

class CEtcGraphColor;
class CIndicatorList;
class CGraphColor;
class CPenThickness;
class CPacket;
class CXScaleManager;

class CDraw  
{
public:
	CDraw();
	virtual ~CDraw();

	virtual bool DrawGraph(CDC* pDC,
			const CRect& realRegion, 
			const CDisplayAttributes& displayAttributes, 
			const CDrawingGraphData& drawingGraphData,
			const CSubGraphData& subGraphData);

	virtual bool DrawGraph(CDC* pDC,
			const CRect& realRegion, 
			const CDisplayAttributes& displayAttributes, 
			const CDrawingGraphData& drawingGraphData,
			const CSubGraphData& subGraphData,
			const CString& strRQ,
			CXScaleManager* pxScaleManager);

protected:
	const double PI;
	CGraphBaseData::GRAPHTYPE	m_eGraphType;
	CGraphStyle					m_graphStyle;
	CGraphDrawStyle				m_graphDrawStyle;

friend class CDrawSingleton;
// (2008/5/14 - Seung-Won, Bae) for Multi-Language of Special Date or Price Unit Text
protected:
	HWND						m_hOcxWnd;
// (2009/9/15 - Seung-Won, Bae) Do not support special time for WorldOn.
protected:
	CChartInfo::CHART_MODE		m_eChartMode;

protected:
	void DrawLine(CDC* pDC, POINT startPt, POINT endPt);
	void DrawLine(CDC* pDC, POINT startPt, long endX, long endY);
	void DrawLine(CDC* pDC, CPen* pPen, const CPoint& startPt, const CPoint& endPt);
	void DrawLine(CDC* pDC, const CPoint& startPt, const CPoint& endPt, COLORREF penColor, int nLineThickness);
	void DrawRectangle(CDC* pDC, CRect rect);
	void DrawRectangle(CDC* pDC, CRect rect, const COLORREF& penColor, const int nPenWidth = 1, const int nPenStyle = PS_SOLID, const bool bIsNullBrush = true);
	void DrawPolyLine(CDC* pDC, POINT *lppt, int cPoints, int nLineWidth);
//	void DrawPolyPolyLine(CDC* pDC, POINT *lppt, DWORD *lpdwPolyPoints, DWORD cCount, int nLineWidth);

	void DrawArrow(CDC* pDC, CPoint point, BOOL bSell, BOOL upDown, SIZE strDataSize, const COLORREF color, const int nArrowLength = 11, const int nArrowHight = 8);
	void DrawArrow(CDC* pDC, CPoint point, BOOL bSell, BOOL upDown, SIZE strDataSize, const int nArrowLength = 11, const int nArrowHight = 8);
	void DrawArrow(CDC* pDC, CPoint point, BOOL upDown);
	void DrawTriangle(CDC* pDC, CPoint point, COLORREF color);

	void TextOut(CDC* pDC, CRect textRegion, const CString& strText, 
		const bool bIsFullText = false, const bool bIsVertCenter = true, const bool bIsHorzCenter = true);
	void TextOut(CDC* pDC, const CPoint& point, const CString& strText, const COLORREF& textColor = RGB(0, 0, 0), const int nBkMode = TRANSPARENT);
	void TextOut_MultiLine(CDC* pDC, const CPoint& textStartPt, const int nLineGap, 
		const CList<CString, CString>& strTextList, const COLORREF& textColor, const int nBkMode = TRANSPARENT);

	int CalUpLineThickness(const CPenThickness& penThickness, int barWidth);
	int CalDownLineThickness(const CPenThickness& penThickness, int barWidth);
	int CalStationaryLineThickness(const CPenThickness& penThickness, int barWidth);
	
	double CalRealWidth(const CRect& graphDrawingRegion, const CDisplayAttributes& displayAttributes);		
	double CalRealWidth(const CRect& graphDrawingRegion, const int dataStart, const int dataEnd);		
	double CalInclination(CPoint startPoint, CPoint endPoint);

	double CalLogValue(bool checkLog, double data);
	
	void DrawSemiTransparentBitmap(CDC *pDC, CPoint* ptArr, COLORREF rgb);	

	void InsertFourPointToArray(CPoint* point, CPoint point0, CPoint point1, CPoint point2, CPoint point3);
	void InsertThreePointToArray(CPoint* point, CPoint point0, CPoint point1, CPoint point2);

	void CreatePen(CPen& pen, const CGraphDrawStyle& drawStyle, int penThickness, COLORREF color);
	// 기준선 두께, 모양 설정 - ojtaso (20071213)
	void CreatePen(CPen& pen, const CGraphBaseLineStyle& drawStyle, int penThickness, COLORREF color);

	POSITION FindPosition(int nIndex,  CList<int, int>& dataIndex);

	// (2007/1/14 - Seung-Won, Bae) Draw BaseLine and Texts
	// (2007/2/22 - Seung-Won, Bae) Support Text Hiding Option
	int		DrawBaseLineAndTexts( const CRect& graphRegion, CPacket *p_pBasePacket,
									const double& dViewMax, const double& dViewMin, const double& dLoggedViewMax, const double& dLoggedViewMin,
									bool bLog, bool bInvert);	// 차트 성능 개선 김성조 - 그리는 부분과 분리하기 위해 DC사용 부분 제거

	int		DrawBaseLineAndTexts( CDC *p_pDC, const CRect& graphRegion, const CGraphColor& graphColor, CPacket *p_pBasePacket,
		const double& dViewMax, const double& dViewMin, const double& dLoggedViewMax, const double& dLoggedViewMin,
 									bool bLog, bool bInvert, BOOL p_bWithText, bool bBaseLine);	// 기준선 보임 환경설정과 Scale 자동포함 기능 - ojtaso (20071101)

	// (2007/1/15 - Seung-Won, Bae) Draw Price Min/Max Text
	void	GetPriceMinMaxTime( CString &strTime, CPacket *pDataPacket, const int p_nIndex);
	void	DrawPriceMaxText( CDC *pDC, const CRect& graphRegion, const CGraphColor& graphColor, const CString &strData, const int nViewX, const int nViewY, const bool &bInvert);
	void	DrawPriceMinText( CDC *pDC, const CRect& graphRegion, const CGraphColor& graphColor, const CString &strData, const int nViewX, const int nViewY, const bool &bInvert);

	// (2007/1/15 - Seung-Won, Bae) for Line Candle Chart.
	const int CalcMidPoint( const int &nBase, const CPoint& lPt, const CPoint& rPt);
	int GetThickness(const int nThickness, const double& dWidth) const;
protected:
				// (2010/2/21 - Seung-Won, Bae) for Element on one line mode.
	inline void	DrawElement_AddLineForOneLineMode( const bool &p_bInvert, POINT *ptPolyPoints, DWORD *dwPointCounts, int &nCountOfPoly, int &nFullCountOfPoint,
								const int &nX, int nFrom, int nTo);
				// (2010/2/25 - Seung-Won, Bae) for Line Drawing.
	inline void	DrawLine_AddPointToMergedLine( const bool &p_bInvert, POINT *ptPolyPoints, DWORD *dwPointCounts, int &nCountOfPoly, int &nFullCountOfPoint,
								int &nFullCountOfLastPolyLine, const int &nX, const int &nY, const BOOL &bNewPolyLine = FALSE);

protected:
	// (2010/2/24 - Seung-Won, Bae) for Element Drawing.
	void	DrawElement_DrawPolygon( CDC* pDC, const int &nOneLineMode, const int &nCountOfPoly, POINT *ptPolyPoints,
								const DWORD *dwPointCounts, int *nPointCounts, const int &nPenThick, const COLORREF &clrColor, const BOOL &bFill,
								const BOOL &bLineBar = FALSE, const CGraphBaseData::LINEDRAWSTYLE &eLineStyle = CGraphBaseData::Solid_Draw);

protected:
	// (2010/4/5 - Seung-Won, Bae) for Only One Area Polygon.
	inline void		DrawArea_AddPointToOnePolygon( const bool &p_bInvert, POINT *ptPolyPoints, int &nCountOfPoint, const int &nX, const int &nY);

protected:
	inline void	DrawElement_AddShapePolygon( POINT *ptPolyPoints, int *pnPointCounts, int &nCountOfElement, int &nFullPointCount,
								const short &nX, const long &dValue, const POINT *ptTemplatePoints, const POINT &ptTemplateCenter,
								const int &nPointCountOfOneElement);
};

//////////////////////////////////////////////////////////////////////
// Caution!! Do not move to .cpp. it must be inline function for performance.
//////////////////////////////////////////////////////////////////////

// (2010/2/21 - Seung-Won, Bae) for Element on one line mode.
inline void	CDraw::DrawElement_AddLineForOneLineMode( const bool &p_bInvert, POINT *ptPolyPoints, DWORD *dwPointCounts, int &nCountOfPoly, int &nFullCountOfPoint,
													 const int &nX, int nFrom, int nTo)
{
	// Sort to check for merge.
	if( nTo < nFrom)
	{
		int nTemp = nTo;
		nTo = nFrom;
		nFrom = nTemp;
	}
	nTo++;	// for DC Coordination.
	
	// Check for Merge
	if( 2 <= nFullCountOfPoint)
	{
		POINT *ptPrePolyPoints = ptPolyPoints + nFullCountOfPoint - 2;
		if( ptPrePolyPoints->x == nX)
		{
			// Skip one Situation.
			if( nTo < ptPrePolyPoints->y || ptPrePolyPoints[ 1].y < nFrom)
			{
				int nLengthPre = ptPrePolyPoints[ 1].y - ptPrePolyPoints->y;
				int nLengthNew = nTo - nFrom;
				int nGap = ( nTo < ptPrePolyPoints->y ? ptPrePolyPoints->y - nTo : nFrom - ptPrePolyPoints[ 1].y);
				if( nLengthPre < nGap && nLengthNew < nGap)
				{
					// Skip New
					if( nLengthNew < nLengthPre)
					{
						nFrom = ptPrePolyPoints->y;
						nTo = ptPrePolyPoints[ 1].y;
					}
					// Skip Pre
					else
					{
						ptPrePolyPoints->y = nFrom;
						ptPrePolyPoints[ 1].y = nTo;
					}
				}
			}
			// Merge It.
			if( ptPrePolyPoints->y < nFrom)		nFrom = ptPrePolyPoints->y;
			if( nTo < ptPrePolyPoints[ 1].y)	nTo = ptPrePolyPoints[ 1].y;
			nFullCountOfPoint -= 2;
			nCountOfPoly--;
		}
	}
	
	ptPolyPoints += nFullCountOfPoint;
	ptPolyPoints->x = nX;
	ptPolyPoints->y = nFrom;
	
	ptPolyPoints++;
	ptPolyPoints->x = nX;
	ptPolyPoints->y = nTo;
	
	dwPointCounts[ nCountOfPoly] = 2;
	nFullCountOfPoint += dwPointCounts[ nCountOfPoly];
	
	nCountOfPoly++;
}

inline void	CDraw::DrawLine_AddPointToMergedLine( const bool &p_bInvert, POINT *ptPolyPoints, DWORD *dwPointCounts,
												 int &nCountOfPoly, int &nFullCountOfPoint, int &nFullCountOfLastPolyLine, const int &nX, const int &nY, const BOOL &bNewPolyLine)
{
	// Check for Merge
	if( 4 <= nFullCountOfLastPolyLine)
	{
		POINT *ptPrePolyPoints = ptPolyPoints + nFullCountOfPoint - 4;
		if( ptPrePolyPoints->x == nX)
		{
			if( ptPrePolyPoints[ 1].y < nY) ptPrePolyPoints[ 1].y = nY;
			if( nY < ptPrePolyPoints[ 2].y) ptPrePolyPoints[ 2].y = nY;
			ptPrePolyPoints[ 3].y = nY;
			return;
		}
	}
	
	ptPolyPoints += nFullCountOfPoint;
	ptPolyPoints->x = nX;
	ptPolyPoints->y = nY;
	
	ptPolyPoints++;
	ptPolyPoints->x = nX;
	ptPolyPoints->y = nY;
	
	ptPolyPoints++;
	ptPolyPoints->x = nX;
	ptPolyPoints->y = nY;
	
	ptPolyPoints++;
	ptPolyPoints->x = nX;
	ptPolyPoints->y = nY;
	
	// (2010/2/26 - Seung-Won, Bae) for PolyPolyline.
	if( bNewPolyLine || 0 == nCountOfPoly)
	{
		nCountOfPoly++;
		nFullCountOfLastPolyLine = 0;
	}
	
	nFullCountOfLastPolyLine += 4;
	dwPointCounts[ nCountOfPoly - 1] = nFullCountOfLastPolyLine;
	nFullCountOfPoint += 4;
}

// (2010/4/5 - Seung-Won, Bae) for Only One Area Polygon.
inline void	CDraw::DrawArea_AddPointToOnePolygon( const bool &p_bInvert, POINT *ptPolyPoints, int &nCountOfPoint, const int &nX, const int &nY)
{
	// Check for Merge
	if( 2 <= nCountOfPoint)
	{
		POINT *ptPrePolyPoints = ptPolyPoints + nCountOfPoint - 1;
		if( ptPrePolyPoints->x == nX)
		{
			ptPrePolyPoints->y = nY;
			return;
		}
	}
	
	ptPolyPoints += nCountOfPoint;
	ptPolyPoints->x = nX;
	ptPolyPoints->y = nY;
	
	ptPolyPoints++;
	ptPolyPoints->x = nX;
	ptPolyPoints->y = nY;
	
	nCountOfPoint += 2;
}

// (2010/6/28 - Seung-Won, Bae) for shape polygon.
inline void	CDraw::DrawElement_AddShapePolygon( POINT *ptPolyPoints, int *pnPointCounts, int &nCountOfElement, int &nFullPointCount,
								const short &nX, const long &dValue, const POINT *ptTemplatePoints, const POINT &ptTemplateCenter,
								const int &nPointCountOfOneElement)
{
	// to skip of overlapped element.
	if( nPointCountOfOneElement <= nFullPointCount)
	{
		// Get New Start Point
		POINT ptStart = ptTemplatePoints[ 0];
		ptStart.x = nX - ptTemplateCenter.x;
		ptStart.y = dValue - ptTemplateCenter.y;

		POINT &ptPreStart = ptPolyPoints[ nFullPointCount - nPointCountOfOneElement];
		int nXGap = ptPreStart.x - ptStart.x;
		int nYGap = ptPreStart.y - ptStart.y;
		if( -2 < nXGap && nXGap < 2 && -2 < nYGap && nYGap < 2)
			return;
	}

	ptPolyPoints += nFullPointCount;
	for( int i = 0; i < nPointCountOfOneElement; i++)
	{
		ptPolyPoints->x = ptTemplatePoints[ i].x + nX - ptTemplateCenter.x;
		ptPolyPoints->y = ptTemplatePoints[ i].y + dValue - ptTemplateCenter.y;
		ptPolyPoints++;
	}

	pnPointCounts[ nCountOfElement] = nPointCountOfOneElement;
	nFullPointCount += pnPointCounts[ nCountOfElement];

	nCountOfElement++;
}

#endif // !defined(AFX_DRAW_H__72586866_9926_4EDD_92DA_F0D6CAD746F6__INCLUDED_)
