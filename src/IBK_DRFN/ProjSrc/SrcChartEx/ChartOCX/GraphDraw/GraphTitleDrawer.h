// GraphTitleDrawer.h: interface for the CGraphTitleDrawer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRAPHTITLEDRAWER_H__B57B34DA_5AF6_45E5_BB5E_0FF79ABC32BB__INCLUDED_)
#define AFX_GRAPHTITLEDRAWER_H__B57B34DA_5AF6_45E5_BB5E_0FF79ABC32BB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDrawingGraphData;
class CSubGraphData;
class CGraphRegions;
class CGraphColor;
class CIndicatorList;
class CIndicatorInfo;
class CPacket;

class CGraphTitleDrawer  
{
public:
	CGraphTitleDrawer();
	// 타이틀 여러줄로 표시 : 복수종목 - ojtaso (20070227)
	bool DrawGraphTitle(CDC* pDC, const CDrawingGraphData& drawingGraphData, 
			const CSubGraphData& subGraphData, int& nTitleEndPosition, int& nTitleEndLine);

private:
	const CString NODRAW_TITLEREGION;
	const CString TITLE_COMPART;

private:
	// 타이틀 여러줄로 표시 : 복수종목 - ojtaso (20070227)
	// 지표에 조건값 서브그래프별 표시 - ojtaso (20071116)
	bool DrawGraphTitle(CDC* pDC, const CDrawingGraphData& drawingGraphData, 
		const CSubGraphData& subGraphData, const CString& strTitle, const int nTitleIndex, int& nTitleEndPosition, int& nTitleEndLine, const int nSubGraphIndex);
	void DrawGraphTitle(CDC* pDC, const CRect& titleRegion, const CString& strTitle, const COLORREF& textColor1, const COLORREF& textColor2);
	void DrawGraphTitleRegion(CDC* pDC, const CRect& titleRegion, const COLORREF& color);
	void DrawGraphTitleText(CDC* pDC, const CPoint& titlePoint, const CString& strTitle, const COLORREF& color1, const COLORREF& color2);

	bool IsGraphTitleDraw(const CString& strTitle, const CGraphRegions& graphRegions) const;
	bool IsGraphTitleRegionDraw(const CString& strTitle) const;
	bool IsTitleTextColor_StandardValue(CPacket* pPacket, const CSubGraphData& subGraphData) const;
	bool IsTitleTextColor_Five(const CSubGraphData& subGraphData) const;

	CString GetGraphTitle(const CSubGraphData& subGraphData, HWND p_hOcxWnd) const;
	// 지표에 조건값 서브그래프별 표시 - ojtaso (20071116)
	CString GetGraphTitle(const CDrawingGraphData& drawingGraphData, const CString& strGraphTitle, const int nIndex, const int nSubGraphIndex) const;
	// 지표에 조건값 서브그래프별 표시 - ojtaso (20071116)
	// 20080908 김진순	8진법/32진법 표시 적용
	//CString GetGraphTitle_Condition(CIndicatorInfo* pIndicatorInfo, const int nSubGraphIndex) const;
	CString GetGraphTitle_Condition(const CDrawingGraphData& drawingGraphData, const int nSubGraphIndex) const;
	CString GetGraphTitle_Price(const CDrawingGraphData& drawingGraphData, const int nIndex) const;
	COLORREF GetTitleTextColor(const CDrawingGraphData& drawingGraphData, const CSubGraphData& subGraphData, const int nIndex) const;
	COLORREF GetTitleTextColor_General(const CGraphColor& graphColor, const int nIndex) const;
	COLORREF GetTitleTextColor_PriceChart(const CDrawingGraphData& drawingGraphData, const CSubGraphData& subGraphData) const;
	COLORREF GetTitleTextColor_VolumeChart(const CDrawingGraphData& drawingGraphData, const CSubGraphData& subGraphData) const;
	COLORREF GetTitleTextColor_Five(const CSubGraphData& subGraphData) const;
	// 타이틀 여러줄로 표시 : 복수종목 - ojtaso (20070227)
	CRect GetGraphTitleRegion(CDC* pDC, const CPoint& orgPoint, const CString& strTitle, int& nTitleEndPosition, int& nTitleEndLine) const;
	CPacket* GetPacket(const CDrawingGraphData& drawingGraphData, const int nIndex) const;
	bool GetDrawConditionData(const CIndicatorInfo* pIndicatorInfo, const int nIndex, int& nDrawCond) const;
//	bool GetGraphColor( CIndicatorList* pIndicatorList, const CString& strGraphName, const int nSubGraphIndex, CGraphColor& graphColor) const;
};

#endif // !defined(AFX_GRAPHTITLEDRAWER_H__B57B34DA_5AF6_45E5_BB5E_0FF79ABC32BB__INCLUDED_)
