// GraphFrameDrawer.h: interface for the CGraphFrameDrawer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRAPHFRAMEDRAWER_H__CF6747E7_2382_4251_A6F7_B79BFB086C53__INCLUDED_)
#define AFX_GRAPHFRAMEDRAWER_H__CF6747E7_2382_4251_A6F7_B79BFB086C53__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../BlockBaseData.h"

class CGraphRegionColor;
class CGraphFrameDrawerImplementation;

class AFX_EXT_CLASS CGraphFrameDrawer  
{
public:
	CGraphFrameDrawer();
	virtual ~CGraphFrameDrawer();

	void DrawGraphFrame(CDC* pDC, 
			const CRect& graphRegion,		// 그릴 graph 영역
			const CBlockBaseData::BLOCKCOMPART eBlockCompart, // block 구분타입
			const int interBlockHalfGap,		 // block간의 Gap / 2
			const int graphRegionFrameThickness, // block frame 굵기
			const int nGraphBackgroundGapX,	// graph 바탕의 간격(가로)
			const int nGraphBackgroundGapY,	// graph 바탕의 간격(세로)
			const CGraphRegionColor& graphRegionColor);	// Graph Region 색

	void DrawBlockCompartLine(CDC* pDC, const CPoint& startPt, const CPoint& endPt, const COLORREF& blockCompartLineColor);


private:
	CGraphFrameDrawerImplementation* m_pGraphFrameDrawerImpl;
};

#endif // !defined(AFX_GRAPHFRAMEDRAWER_H__CF6747E7_2382_4251_A6F7_B79BFB086C53__INCLUDED_)
