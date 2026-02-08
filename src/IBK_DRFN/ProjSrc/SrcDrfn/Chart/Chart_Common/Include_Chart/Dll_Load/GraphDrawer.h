// GraphDrawer.h: interface for the CGraphDrawer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRAPHDRAWER_H__DC43AE70_3209_446D_B0B4_7A8D3ECC1A59__INCLUDED_)
#define AFX_GRAPHDRAWER_H__DC43AE70_3209_446D_B0B4_7A8D3ECC1A59__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "BlockBaseData.h"

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


class AFX_EXT_CLASS CGraphDrawer
{
public:
	bool DrawGraph(CDC* pDC,	// CDC
			const CDrawingGraphData& drawingGraphData,
            const CSubGraphData& subGraphData,			// subgraph data
			int& titlePosition);
	
	void DrawSignal(CDC* pDC, 
			const CDrawingGraphData& drawingGraphData, 
			const CList<double, double>* pSignalData);

	bool DrawGrip(CDC* pDC, const int index, const CSelectSubGraphData& selectSubGraphData);

	int PointInSubGraph(const CPoint& point, const CSelectSubGraphData& selectSubGraphData, CIndicatorList* pIndicatorList);

	bool GetTextInRightScaleRegionDrawer(const CTextInRightScaleRegionDrawer& textData, CString& strCurPrice, CString& strRiseFallRate) const;
};

#endif // !defined(AFX_GRAPHDRAWER_H__DC43AE70_3209_446D_B0B4_7A8D3ECC1A59__INCLUDED_)
