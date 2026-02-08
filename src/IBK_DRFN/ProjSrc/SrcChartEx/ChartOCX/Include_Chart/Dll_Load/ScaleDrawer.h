// ScaleDrawer.h: interface for the CScaleDrawer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCALEDRAWER_H__D71B1615_8C0D_485B_811B_AD7EDF85A125__INCLUDED_)
#define AFX_SCALEDRAWER_H__D71B1615_8C0D_485B_811B_AD7EDF85A125__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../BlockBaseData.h"

class CScaleDrawerImplementation;
class CDrawingHorzScaleData;
class CDrawingVertScaleData;
class CHorizontalScale;
class CHorzScaleRegion;
class CVerticalScale;
class CVertScaleRegion;
class CXScaleManager;

class AFX_EXT_CLASS CScaleDrawer  
{
public:
	CScaleDrawer( HWND p_hOcxWnd, CChartInfo::CHART_MODE p_eChartMode);
	virtual ~CScaleDrawer();

	// 가로 scale 그린다.
	void DrawHorzTextOrGrid(CDC* pDC, 
				const CDrawingHorzScaleData& drawingHorzScaleData,
				const CHorzScaleRegion& horzScaleRegion, // 가로 scale의 영역
				const CHorizontalScale& horzScale,		 // 가로 scale에 대한 값
				CXScaleManager* pxScaleManager,
				const char *p_szRQ);		 

	// 세로 scale 그린다.
	void DrawVertTextOrGrid(CDC* pDC, 
				const CDrawingVertScaleData& drawingVertScaleData,
				const CVertScaleRegion& vertScaleRegion, // 세로 scale의 영역
				const CVerticalScale& vertScale);		 // 세로 scale에 대한 값

private:
	CScaleDrawerImplementation* m_pScaleDrawerImpl;
};

#endif // !defined(AFX_SCALEDRAWER_H__D71B1615_8C0D_485B_811B_AD7EDF85A125__INCLUDED_)
