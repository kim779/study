#include "stdafx.h"
#include "BalanceBasicDrawer.h"
#include "DrawingData.h"
#include "Conversion.h"
#include "DataMath.h"

// CBalanceBasicDrawer
//=============================================================================================================================
// Balance 의 좌표를 구하기.
bool CBalanceBasicDrawer::IsDrawing(const CRect& region, const CPoint& point) const
{
	return true;
}

void CBalanceBasicDrawer::DrawShape(CDC* pDC, const COnePointDrawingData& drawingData, const CDisplayData& displayData, const CPoint& point, const bool /*isCurrentRegion*/)
{
	CString strCoefficient = drawingData.GetDataType().GetExtData();
	if( strCoefficient.IsEmpty() )
		strCoefficient = "9,17,26,33,42,65,76,129,172,257,0,0";
	
	// 이전 값과 다른 경우에는 새로 Coefficient를 구한다.
	if( strCoefficient != m_strCoefficient )	
	{
		m_strCoefficient = strCoefficient;
		m_arrCoefficient.RemoveAll();

		CString strBuf = strCoefficient;
		CString strR("");
		while( !strBuf.IsEmpty() )
		{
			strR = CDataConversion::GetStringData(strBuf, ",");
			if( strR.IsEmpty() )
			{
				strR = strBuf;
				strBuf.Empty();
			}
			if( strR.IsEmpty() )
				break;

			m_arrCoefficient.Add(atoi(strR));
		}
	}

	BOOL bLeftLineExt = (BOOL)drawingData.GetLineExtEnvironment().GetLeftExt();
	BOOL bRightLineExt = (BOOL)drawingData.GetLineExtEnvironment().GetRightExt();

	// 클릭 POINT를 기준으로 그린다.
	DrawFigure_Data(pDC, displayData, point, bLeftLineExt, bRightLineExt);
}

// data 형태의 그리기.
void CBalanceBasicDrawer::DrawFigure_Data(CDC* pDC, const CDisplayData& displayData, const CPoint& point, BOOL bLeftExt, BOOL bRightExt)
{
	CRegion coordinateCalRegion( displayData.GetCoordinateCalRegion(), displayData.GetDisplayAttributes());
	int nCurrentDataIndex = CDrawer::GetCurrentDataIndex( coordinateCalRegion, point);
	
	int		drawPt_X	= 0;
	int		nBalanceCoefficient;
	BOOL	bCenterDraw	= FALSE;
	for( int i = 0; i < _GB_VALUE_COUNT; i++)
	{
		nBalanceCoefficient = GetCoefficient(i);

		if( 1 < nBalanceCoefficient )
		{
			if( bRightExt )
			{
				drawPt_X = CDrawer::GetDrawingPoint_X( coordinateCalRegion, nCurrentDataIndex + nBalanceCoefficient - 1);
				CDrawer::DrawLine( pDC, CPoint(drawPt_X, displayData.GetDisplayRegion().top), CPoint(drawPt_X, displayData.GetDisplayRegion().bottom));
			}

			if( bLeftExt )
			{
				drawPt_X = CDrawer::GetDrawingPoint_X( coordinateCalRegion, nCurrentDataIndex - nBalanceCoefficient + 1);
				CDrawer::DrawLine( pDC, CPoint(drawPt_X, displayData.GetDisplayRegion().top), CPoint(drawPt_X, displayData.GetDisplayRegion().bottom));
			}
		}
		else if( 1 == nBalanceCoefficient && !bCenterDraw)
		{
			int drawPt_X = CDrawer::GetDrawingPoint_X( coordinateCalRegion, nCurrentDataIndex);
			CDrawer::DrawLine( pDC, CPoint(drawPt_X, displayData.GetDisplayRegion().top), CPoint(drawPt_X, displayData.GetDisplayRegion().bottom));
			bCenterDraw = TRUE;
		}
	}
	
	if( !bCenterDraw)
	{
		drawPt_X = CDrawer::GetDrawingPoint_X( coordinateCalRegion, nCurrentDataIndex );
		CDrawer::DrawStyleLine( pDC, 
										CPoint(drawPt_X, displayData.GetDisplayRegion().top),
										CPoint(drawPt_X, displayData.GetDisplayRegion().bottom),
										PS_DASHDOT );
	}
}

// ----------------------------------------------------------------------------
// 일목수치 계수를 가져오기.
int CBalanceBasicDrawer::GetCoefficient(const int nIndex) const
{
	if( nIndex < 0 || nIndex >= m_arrCoefficient.GetSize() )
		return -1;

	return (int)m_arrCoefficient.GetAt(nIndex);
}

void CBalanceBasicDrawer::DrawText(CDC* pDC, const COnePointDrawingData& drawingData)
{
	if(!CVertLineDrawer::IsDrawing(drawingData.GetCurrentRegion(), drawingData.GetPoints().GetDrawingPoint()))
		return;

	CTextShowPosition* pTextPosition = (CTextShowPosition *) drawingData.GetTextPosition();
	if(pTextPosition == NULL)
		return;
	if(!pTextPosition->IsShow())
		return;

	CString strIndex;
	int nBalanceCoefficient;
	for( int i = 0; i < _GB_VALUE_COUNT; i++)
	{
		nBalanceCoefficient = GetCoefficient(i);

		if( 0 < nBalanceCoefficient )
		{
			strIndex.Format( _T("%d"), nBalanceCoefficient );
			if( drawingData.GetLineExtEnvironment().GetRightExt())	DrawText( pDC, drawingData, strIndex, i + 1);
			if( drawingData.GetLineExtEnvironment().GetLeftExt())	DrawText( pDC, drawingData, strIndex, -( i + 1));
		}
	}

	return;
}

void CBalanceBasicDrawer::DrawText( CDC* pDC, const COnePointDrawingData& drawingData, CString strIndex, int nIndex)
{
	CDrawingRegions *pDrawingRegions = drawingData.GetDrawingRegions();
	if( !pDrawingRegions) return;
	CRect rctGraphRegion = pDrawingRegions->GetCurrentUnmixedGraphRegion();
	
	CBlockRegion blockRegion = drawingData.GetHeadBlockRegion();
	CPoint ptLine;
	if( !GetPoint( drawingData.IsRelativePointType(), CRegion( rctGraphRegion, drawingData.GetDisplayAttributes()), drawingData.GetPoints().GetDrawingPoint(), nIndex, ptLine))
		return;
	
	CRect textRegion = GetTextRegion( blockRegion, pDC->GetTextExtent( strIndex), ptLine.x, false);
	textRegion.OffsetRect( ptLine.x - textRegion.left, 0);
	
	if( textRegion.left < rctGraphRegion.left || rctGraphRegion.right < textRegion.right) return;
	
	CDrawer::DrawTextOut( pDC, strIndex, textRegion);
}

bool CBalanceBasicDrawer::GetPoint( const bool p_bIsRelativePointType, const CRegion &region, const CPoint& startPoint, const int nIndex, CPoint& gbLinePoint) const
{
	if( nIndex < -_GB_VALUE_COUNT || _GB_VALUE_COUNT < nIndex || 0 == nIndex ) return false;
	
	int nDirection = nIndex / abs( nIndex);
	int nI = nIndex * nDirection - 1;
	int nBalanceCoefficient = GetCoefficient(nI);
	if( nBalanceCoefficient < 1) return false;
	
	CPoint point = startPoint;
	
	if( p_bIsRelativePointType)
	{
		int drawPt_X = CDrawer::GetDrawingPoint_X( region, CDrawer::GetCurrentDataIndex( region, startPoint) + ( nBalanceCoefficient - 1) * nDirection);
		gbLinePoint = CPoint(drawPt_X, startPoint.y);
	}
	else
	{
		point.x = point.x + ( nBalanceCoefficient - 1) * nDirection;
		gbLinePoint = point;
	}

	return true;
}
