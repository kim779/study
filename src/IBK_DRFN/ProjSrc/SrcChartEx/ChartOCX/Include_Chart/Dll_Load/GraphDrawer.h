// GraphDrawer.h: interface for the CGraphDrawer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRAPHDRAWER_H__DC43AE70_3209_446D_B0B4_7A8D3ECC1A59__INCLUDED_)
#define AFX_GRAPHDRAWER_H__DC43AE70_3209_446D_B0B4_7A8D3ECC1A59__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "../BlockBaseData.h"

// (2006/9/5 - Seung-Won, Bae) for Import in Extension DLL
#ifndef AFX_EXT_CLASS_GRAPHDRAW
	#ifdef _GRAPHDRAW_EXPORT									// in IndicatorCondition
		#define AFX_EXT_CLASS_GRAPHDRAW	AFX_CLASS_EXPORT
//	#elif defined _DD_NOT_IMPORT							// in Lib or Regular used in IndicatorCondition.
//		#define AFX_EXT_CLASS_DD			
	#else													// in Other, To Link
		#define AFX_EXT_CLASS_GRAPHDRAW	AFX_CLASS_IMPORT
	#endif
#endif

class CGraphColor;
class CPacket;
class CSubGraphData;
class CIndicatorList;
class CDrawingGraphData;
class CDisplayAttributes;
class CDisplayDataMinMax;
class CEtcGraphColor;
class CGraphRegions;
class CSelectSubGraphData;
class CTextInRightScaleRegionDrawer;
class CXScaleManager;
class CSignalData;

class AFX_EXT_CLASS_GRAPHDRAW CGraphDrawer
{	
public:
	// (2009/1/7 - Seung-Won, Bae) Support graph title drawing routine.
	bool DrawGraphTitle(CDC* pDC,	// CDC
			const CDrawingGraphData& drawingGraphData,
            const CSubGraphData& subGraphData,			// subgraph data
			int& titlePosition,
			int& titleLine);							// 타이틀 여러줄로 표시 : 복수종목 - ojtaso (20070227)

	// Y Scale상에 종가 표시 수정 - ojtaso (20070807)
	bool CGraphDrawer::DrawGraph(CDC* pDC,	// CDC
				const CDrawingGraphData& drawingGraphData,
				const CSubGraphData& subGraphData,  // subgraph data
				const CString& strRQ,
				CXScaleManager* pxScaleManager);

	// xScaleManager/복수종목 적용 - xScaleManager - ojtaso (20070404)
	void DrawSignal(CDC* pDC, 
			const CDrawingGraphData& drawingGraphData, 
			const CList<double, double>* pSignalData,
			CSignalData &p_signalData,
			const CString& strRQ, 
			CXScaleManager* pxScaleManager);

	bool DrawGrip(CDC* pDC, const int index, const CSelectSubGraphData& selectSubGraphData);

	int PointInSubGraph(const CPoint& point, const CSelectSubGraphData& selectSubGraphData, CIndicatorList* pIndicatorList);

	// vntsorl_20110609:[A00000259] 가격눈금 위의 현재가 표시 수정
	bool GetTextInRightScaleRegionDrawer(const CTextInRightScaleRegionDrawer& textData, CString& strCurPrice, CString& strDiffPrice, CString& strPipPrice, CString& strRiseFallRate) const;

};

#endif // !defined(AFX_GRAPHDRAWER_H__DC43AE70_3209_446D_B0B4_7A8D3ECC1A59__INCLUDED_)
