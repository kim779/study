// DataHorzScaleDrawer.h: interface for the CDataHorzScaleDrawer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATAHORZSCALEDRAWER_H__71364DDB_0517_404D_8E1D_3516E7F28193__INCLUDED_)
#define AFX_DATAHORZSCALEDRAWER_H__71364DDB_0517_404D_8E1D_3516E7F28193__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HorzScaleDrawer.h"

// <Data>
class CDataHorzScaleDrawer: public CHorzScaleDrawer
{
public:
	CDataHorzScaleDrawer( HWND p_hOcxWnd, CChartInfo::CHART_MODE p_eChartMode);

protected:
	void DrawTextAndCompartLine(CDC* pDC, const CRect& scaleRegion, const CPoint& nowPos, const double& textRegionWidth,
		const bool bIsTopPos, const CString& data, const CSize& textSize, const COLORREF textColor, const COLORREF gridColor, int& textEndPos);

	int GetTextEndPosition_x(const CPoint& nowPos, const CSize& textSize) const;
	int GetTextPosition_x(const int org_x, const int textWidth, const bool isCenter = true) const;
	
	bool IsOverlap(const int nTextWidth, const int nNowPos_x, const int nPrevPos_x) const;
};


// <행사가>
class CPriceZoneHorzScaleDrawer : public CDataHorzScaleDrawer
{
public:
	CPriceZoneHorzScaleDrawer( HWND p_hOcxWnd, CChartInfo::CHART_MODE p_eChartMode);

public:
	virtual void DrawTextOrGrid(CDC* pDC, const CDrawingHorzScaleData& drawingHorzScaleData, const CRect& scaleRegion, const CHorizontalScale& horzScale);
	virtual void DrawTextOrGrid_Both(CDC* pDC, const CDrawingHorzScaleData& drawingHorzScaleData, const CHorzScaleRegion& horzScaleRegion, const CHorizontalScale& horzScale);
	virtual void DrawGrid(CDC* pDC, const CDrawingHorzScaleData& drawingHorzScaleData, const CHorizontalScale& horzScale);
};


// <거래량>, <기타 data>
class CEtcDataHorzScaleDrawer : public CDataHorzScaleDrawer  
{
public:
	CEtcDataHorzScaleDrawer( HWND p_hOcxWnd, CChartInfo::CHART_MODE p_eChartMode);

public:
	virtual void DrawTextOrGrid(CDC* pDC, const CDrawingHorzScaleData& drawingHorzScaleData, const CRect& scaleRegion, const CHorizontalScale& horzScale);
	virtual void DrawTextOrGrid_Both(CDC* pDC, const CDrawingHorzScaleData& drawingHorzScaleData, const CHorzScaleRegion& horzScaleRegion, const CHorizontalScale& horzScale);
	virtual void DrawGrid(CDC* pDC, const CDrawingHorzScaleData& drawingHorzScaleData, const CHorizontalScale& horzScale);
	
private:
	// 세워 쓰기 : 일반 세로쓰기가 아니다 
	void DrawTextAndCompartLine_stand(CDC* pDC, const CRect& scaleRegion, const CPoint& nowPos, const double& textRegionWidth, 
		const CString& data, const CSize& textSize, const COLORREF textColor, const COLORREF gridColor, int& textEndPos);

	void GetFont(CFont& fontVertCaption);
};


// <좌우식>
class CLeftRightScaleDrawer : public CDataHorzScaleDrawer
{
public:
	CLeftRightScaleDrawer( HWND p_hOcxWnd, CChartInfo::CHART_MODE p_eChartMode);

public:
	virtual void DrawTextOrGrid(CDC* pDC, const CDrawingHorzScaleData& drawingHorzScaleData, const CRect& scaleRegion, const CHorizontalScale& horzScale);
	virtual void DrawTextOrGrid_Both(CDC* pDC, const CDrawingHorzScaleData& drawingHorzScaleData, const CHorzScaleRegion& horzScaleRegion, const CHorizontalScale& horzScale);
	virtual void DrawGrid(CDC* pDC, const CDrawingHorzScaleData& drawingHorzScaleData, const CHorizontalScale& horzScale);
	
private:
	bool GetDrawingEnvironment(const CDrawingHorzScaleData& drawingHorzScaleData, const CRect& scaleRegion, const int nTextHeight, 
		CList<double, double>& dataList, double& dMin, double& dMax, double& dOnePtWidth, int& nPosPt_y) const;
	bool GetDataListAndMinMax(const CDrawingHorzScaleData& drawingHorzScaleData, CList<double, double>& dataList, double& dMin, double& dMax) const;
	void GetRealMinMax(const CDrawingHorzScaleData& drawingHorzScaleData, const CString& strPacketType, double& dMiddle_Min, double& dMiddle_Max, double& dMin, double& dMax) const;
	void GetRealMinMax_FixedRate(const CDisplayDataMinMax& displayDataMinMax, double& dMin, double& dMax) const;
	CString GetData(const double& dData, double& dPreData) const;
};


// <MarketProfile>
class CMPPacket;
class CMProfile;

class CMarketProfileHorzScaleDrawer : public CHorzScaleDrawer
{
public:
	CMarketProfileHorzScaleDrawer( HWND p_hOcxWnd, CChartInfo::CHART_MODE p_eChartMode);

public:
	virtual void DrawTextOrGrid(CDC* pDC, const CDrawingHorzScaleData& drawingHorzScaleData, const CRect& scaleRegion, const CHorizontalScale& horzScale);
	virtual void DrawTextOrGrid_Both(CDC* pDC, const CDrawingHorzScaleData& drawingHorzScaleData, const CHorzScaleRegion& horzScaleRegion, const CHorizontalScale& horzScale);
	virtual void DrawGrid(CDC* pDC, const CDrawingHorzScaleData& drawingHorzScaleData, const CHorizontalScale& horzScale);

private:
	void DrawScale(CDC* pDC, const CDrawingHorzScaleData& drawingHorzScaleData, 
		const CRect& scaleRegion_Top, const CRect& scaleRegion_Bottom, const CHorizontalScale& horzScale);
	void DrawTextAndCompartLine(CDC* pDC, const CRect& scaleRegion, const CPoint& nowPos, const bool bIsTopPos, 
		const CString& data, const COLORREF textColor, const COLORREF gridColor, int& textEndPos);
	bool GetEnvironmentData(CMPPacket* pMPPacket, const CRect& scaleRegion1, const CRect& scaleRegion2, const int nSize_Y, 
		CList<CMProfile*, CMProfile*>*& pMProfileList, double& dOnePtWidth, int& nPosPt_y1, int& nPosPt_y2) const;

	bool IsDrawText(const CScaleBaseData::HORZSCALEPOSITION eHorzScalePos, const int nPre_textEndPos, const int nPosPt_x) const;
	bool IsTopPostiont(const CScaleBaseData::HORZSCALEPOSITION eHorzScalePos, const bool bIsScaleRegion1) const;
};

#endif // !defined(AFX_DATAHORZSCALEDRAWER_H__71364DDB_0517_404D_8E1D_3516E7F28193__INCLUDED_)
