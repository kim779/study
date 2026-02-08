// FullTimeScaleDrawer.h: interface for the CFullTimeScaleDrawer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FULLTIMESCALEDRAWER_H__E3000E43_66D6_4178_BFCA_404426C2DD64__INCLUDED_)
#define AFX_FULLTIMESCALEDRAWER_H__E3000E43_66D6_4178_BFCA_404426C2DD64__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HorzScaleDrawer.h"

class CFullTimeScaleDrawer : public CHorzScaleDrawer  
{
public:
	CFullTimeScaleDrawer( HWND p_hOcxWnd, CChartInfo::CHART_MODE p_eChartMode);
	virtual ~CFullTimeScaleDrawer();

// Not Used
protected:
	virtual void DrawTextOrGrid(CDC* pDC, const CDrawingHorzScaleData& drawingHorzScaleData, const CRect& scaleRegion, const CHorizontalScale& horzScale)						{}
	virtual void DrawTextOrGrid_Both(CDC* pDC, const CDrawingHorzScaleData& drawingHorzScaleData, const CHorzScaleRegion& horzScaleRegion, const CHorizontalScale& horzScale)	{}
	virtual void DrawGrid(CDC* pDC, const CDrawingHorzScaleData& drawingHorzScaleData, const CHorizontalScale& horzScale)														{}

// Used
protected:
	virtual bool DrawTextOrGrid(CDC* pDC, const CDrawingHorzScaleData& drawingHorzScaleData, const CRect& scaleRegion, const CHorizontalScale& horzScale, CXScaleManager* pxScaleManager, const char *p_szRQ);
	virtual bool DrawTextOrGrid_Both(CDC* pDC, const CDrawingHorzScaleData& drawingHorzScaleData, const CHorzScaleRegion& horzScaleRegion, const CHorizontalScale& horzScale, CXScaleManager* pxScaleManager, const char *p_szRQ);
	virtual bool DrawGrid(CDC* pDC, const CDrawingHorzScaleData& drawingHorzScaleData, const CHorzScaleRegion& horzScaleRegion, const CHorizontalScale& horzScale, CXScaleManager* pxScaleManager, const char *p_szRQ);
protected:
	int			GetScaleTextType( int p_nScaleType, CScaleBaseData::HORZSCALEDRAWERTYPE p_eScaleDrawType, const tm &p_tmPrev, const tm &p_tmDRI, int p_nSkipFactor);
	CString		GetScaleText( int &p_nTextType, tm &p_tmDRI, CStringArray &p_astrTimeUnit, int p_nTimeDiff, int p_nScaleType, CScaleBaseData::HORZSCALEDRAWERTYPE p_eScaleDrawType, const tm &p_tmPrev, int p_nSkipFactor, int p_nScaleTextTypeMask, CXScaleManager *p_pxScaleManager, const CString &p_strBlockRQ);
	// (2009/4/30 - Seung-Won, Bae) Check packet type for Auto Scale Type.
	int			GetScaleTextTypeMask( CString p_strPacketType);
};

#endif // !defined(AFX_FULLTIMESCALEDRAWER_H__E3000E43_66D6_4178_BFCA_404426C2DD64__INCLUDED_)
