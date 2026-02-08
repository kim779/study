// GraphDrawer.cpp: implementation of the CGraphDrawer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GraphDrawer.h"

#include "GraphDrawerImplementation.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// (2009/1/7 - Seung-Won, Bae) Support graph title drawing routine.
bool CGraphDrawer::DrawGraphTitle(CDC* pDC,	// CDC
			const CDrawingGraphData& drawingGraphData,
            const CSubGraphData& subGraphData,  // subgraph data
			int& titlePosition,
			int& titleLine)				// 타이틀 여러줄로 표시 : 복수종목 - ojtaso (20070227)
{
	CGraphDrawerImplementation graphDrawerImplementation;
	// 타이틀 여러줄로 표시 : 복수종목 - ojtaso (20070227)
	return graphDrawerImplementation.DrawGraphTitle( pDC, drawingGraphData, subGraphData, titlePosition, titleLine);
}

// Y Scale상에 종가 표시 수정 - ojtaso (20070807)
bool CGraphDrawer::DrawGraph(CDC* pDC,	// CDC
			const CDrawingGraphData& drawingGraphData,
            const CSubGraphData& subGraphData,  // subgraph data
			const CString& strRQ,
			CXScaleManager* pxScaleManager) 
{
	CGraphDrawerImplementation graphDrawerImplementation;
	return graphDrawerImplementation.DrawGraph(pDC, drawingGraphData, subGraphData, strRQ, pxScaleManager);
}
// xScaleManager/복수종목 적용 - xScaleManager - ojtaso (20070404)
void CGraphDrawer::DrawSignal(CDC* pDC, 
			const CDrawingGraphData& drawingGraphData,
			const CList<double, double>* pSignalData,
			CSignalData &p_signalData,
			const CString& strRQ,
			CXScaleManager* pxScaleManager)
{
	CGraphDrawerImplementation graphDrawerImplementation;
	graphDrawerImplementation.DrawSignal(pDC, drawingGraphData, pSignalData, p_signalData, strRQ, pxScaleManager);
}

bool CGraphDrawer::DrawGrip(CDC* pDC, const int index, const CSelectSubGraphData& selectSubGraphData)
{
	CGraphDrawerImplementation graphDrawerImplementation;
	return graphDrawerImplementation.DrawGrip(pDC, index, selectSubGraphData);
}

int CGraphDrawer::PointInSubGraph(const CPoint& point, const CSelectSubGraphData& selectSubGraphData, CIndicatorList* pIndicatorList)
{
	CGraphDrawerImplementation graphDrawerImplementation;
	return graphDrawerImplementation.PointInSubGraph(point, selectSubGraphData, pIndicatorList);
}

// vntsorl_20110609:[A00000259] 가격눈금 위의 현재가 표시 수정
bool CGraphDrawer::GetTextInRightScaleRegionDrawer(const CTextInRightScaleRegionDrawer& textData, CString& strCurPrice, CString& strDiffPrice, CString& strPipPrice, CString& strRiseFallRate) const
{
	CGraphDrawerImplementation graphDrawerImplementation;
	return graphDrawerImplementation.GetTextInRightScaleRegionDrawer(textData, strCurPrice, strDiffPrice, strPipPrice, strRiseFallRate);
}


