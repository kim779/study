// GraphDataDrawer.h: interface for the CGraphDataDrawer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRAPHDATADRAWER_H__07EEA2C9_974A_4705_96FD_2FDF4E216EC0__INCLUDED_)
#define AFX_GRAPHDATADRAWER_H__07EEA2C9_974A_4705_96FD_2FDF4E216EC0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

class CPacket;
class CGraphStyle;
class CGraphColor;
class CSubGraphData;
class CDrawingGraphData;
class CGraphDataDrawingData;
class CDisplayAttributes;
class CDisplayDataMinMax;

class CPrices
{
public:
	CPrices& operator=(const CPrices& prices);

public:
	CPrices();
// vntsorl_20110609:[A00000259] 가격눈금 위의 현재가 표시 수정
	CPrices(const double& dPrev, const double& dCur, const double& dPipSize);

	void SetPrev(const double& dPrice);
	void SetCur(const double& dPrice);
	void SetDecimalDownCount(const int nDecimalDownCount);
// vntsorl_20110609:[A00000259] 가격눈금 위의 현재가 표시 수정
	void SetPipSize(const double& dPipSize);
	void SetDataUnit(const CString& strDataUnit);
	void SetPacketType(const CString& strPacketType);

	double GetPrev() const;
	double GetCur() const;
	double GetDifference() const;
	int GetDecimalDownCount() const;
// vntsorl_20110609:[A00000259] 가격눈금 위의 현재가 표시 수정
	double GetPipSize() const;
	CString GetDataUnit() const;
	CString GetPacketType() const;

	bool IsEmpty() const;
	bool IsRose() const;
	bool IsFall() const;

private:
	double m_dPrevPrice;
	double m_dCurPrice;
	int m_nDecimalDownCount;
	// vntsorl_20110609:[A00000259] 가격눈금 위의 현재가 표시 수정
	double m_dPipSize;
	CString m_strDataUnit;
	CString m_strPacketType;
};


class CTextData
{
public:
	CTextData& operator=(const CTextData& data);

public:
	CTextData();
	CTextData(const CString& strTitle, const CString& strData, const COLORREF& color);

	void Set(const CString& strTitle, const CString& strData, const COLORREF& color);
	void SetTitle(const CString& strTitle);
	void SetData(const CString& strData);
	void SetColor(const COLORREF& color);

	CString GetTitle() const;
	CString GetData() const;
	COLORREF GetColor() const;

	bool IsEmpty() const;

private:
	CString m_strTitle;
	CString m_strData;
	COLORREF m_color;
};


// ----------------------------------------------------------------------------
// 기본적인 그리기
class CRateDrawer
{
protected:
	void DrawTextOut(CDC* pDC, const CPoint& point, const CString& strData, int& nPosition_Y);
	void DrawTextOut(CDC* pDC, const COLORREF& textColor, const CPoint& point, const CString& strData, int& nPosition_Y);
	void DrawRegion(CDC* pDC, const CRect& region, const COLORREF& penColor, const COLORREF& brushColor);

	CString GetRateText(const double& dBaseData, const double& dData) const;

	bool GetPrices(CPacket* pPacket, const int nDataEnd, CPrices& prices) const;
	bool GetDecimalDownCountAndDataType(CPacket* pPacket, CPrices& prices) const;
	int GetRealDataEnd(CPacket* pPacket, const int nDataEnd) const;

private:
	bool GetDecimalDownCount(CPacket* pPacket, int& nDecimalDownCount, CString& strType) const;
};

// graph 영역 안의 data 그리기
// -> 최저가 대비 최고가 등락률, 최저가 대비 현재가 등락률
//	  최고가 대비 현재가 등락률, 전일비
class CDataInGraphRegionDrawer : public CRateDrawer
{
// (2008/1/16 - Seung-Won, Bae) for MultiLanguage
protected:
	HWND	m_hOcxWnd;
public:
	HWND	GetOcxHwnd( void) const		{	return m_hOcxWnd;	}

public:
	void DrawData(CDC* pDC, const CRect& region, const CDisplayAttributes& displayAttributes, 
		const CDrawingGraphData& drawingGraphData, const CSubGraphData& subGraphData, CPrices& prices);

private:
	void DrawData(CDC* pDC, const CRect& region, const CDrawingGraphData& drawingGraphData, 
		const CDisplayDataMinMax& curMinMax, const CPrices& prices, const CGraphColor& graphColor);
	void DrawData(CDC* pDC, const CRect& region, const CDrawingGraphData& drawingGraphData, 
		const CPrices& prices, const CList<CTextData, CTextData&>& textDataList);

	bool IsGraphDataDraw(const CRect& region, const CDisplayAttributes& displayAttributes) const;

	bool GetDrawingDatas(const CString& strIndicatorName, const CGraphStyle& graphStyle, const CList<CPacket*, CPacket*>& packetList, 
		const CDisplayAttributes& displayAttributes, CDisplayDataMinMax& curMinMax, CPrices& prices) const;
	bool GetHighLow(const CString& strIndicatorName, const CGraphStyle& graphStyle, 
		const CList<CPacket*, CPacket*>& packetList, const CDisplayAttributes& displayAttributes, CDisplayDataMinMax& curMinMax) const;

	bool GetDataTexts(const CGraphDataDrawingData& dataDrawing, const CDisplayDataMinMax& curMinMax, 
		const CPrices& prices, const CGraphColor& graphColor, CList<CTextData, CTextData&>& textDataList) const;
	bool GetDataTexts(const CGraphDataDrawingData& dataDrawing, const CDisplayDataMinMax& curMinMax, 
		const CPrices& prices, const CGraphColor& graphColor, int& nMaxLength_Title, int& nMaxLength_Data, CList<CTextData, CTextData&>& textDataList) const;

	int GetPosition_Y(CDC* pDC, const int nDataCount, const int nRegionHeight, const bool bIsInvert, const CDisplayDataMinMax& curMinMax, const double& dCur) const;
	CString GetDrawingText(const CTextData& textData) const;

	bool AddTailAndGetMaxLength(const CString& strTitle, const CString& strData, const COLORREF& color, 
		int& nMaxLength_Title, int& nMaxLength_Data, CList<CTextData, CTextData&>& textDataList) const;
	void ChangeData(const int nMaxLength_Title, const int nMaxLength_Data, CList<CTextData, CTextData&>& textDataList) const;
	void ChangeMaxLength(const CString& strData, int& nMaxLength) const;
	bool GetChangingData(const CString& strData, const int nMaxLength, const bool bIsTail, CString& strChangeData) const;

	CString GetRiseFallRateText_LowHigh(const CDisplayDataMinMax& curMinMax) const;
	CString GetRiseFallRateText_LowCur(const double& dLow, const double& dCur) const;
	CString GetRiseFallRateText_HighCur(const double& dHigh, const double& dCur) const;
	CString GetRateText_PrevCur(const CPrices& prices) const;

};


// right scale 영역 안의 data 그리기
// -> 전일가 대비 현재가 등락률 (현재가)
class CTextInRightScaleRegionDrawer;
class CDataInRightScaleRegionDrawer : public CRateDrawer
{
public:
	void DrawData(CDC* pDC, const CRect& region, const CDisplayAttributes& displayAttributes, 
		const CDrawingGraphData& drawingGraphData, const CGraphColor& graphColor, const CPrices& pricesInView);

// vntsorl_20110609:[A00000259] 가격눈금 위의 현재가 표시 수정
	bool GetText(const CTextInRightScaleRegionDrawer& textData, CString& strCurPrice, CString& strDiffPrice, CString& strPipPrice, CString& strRiseFallRate) const;

	CPacket* GetPacket(const CDrawingGraphData& drawingGraphData) const;
private:
	void DrawData(CDC* pDC, const CRect& region, 
		const CDrawingGraphData& drawingGraphData, const CGraphColor& graphColor, const CPrices& prices, CString &p_strPacketType);
	// Y Scale상에 종가 표시 수정 - ojtaso (20070807)
	void DrawData(CDC* pDC, const CRect& region, 
		const CDrawingGraphData& drawingGraphData, const CGraphColor& graphColor, const CPrices& prices, CString &p_strPacketType,
		CRect& rcScaleText);
	void DrawData(CDC* pDC, const CRect& textRegion, const CString& strText, 
		const COLORREF& bkColor, const COLORREF& textColor, int& nPrevGap_Y);

	bool GetDrawingDatas(const CDisplayAttributes& displayAttributes,
			const CDrawingGraphData& drawingGraphData, const CPrices& pricesInView, CPrices& prices, CString &p_strPacketType) const;
	bool GetDrawingDatas(CPacket* pPacket, const int nDataEnd, const int nCurDataType, 
		const CPrices& pricesInView, CPrices& prices) const;
	bool GetPricesInFull(CPacket* pPacket, CPrices& prices) const;
	bool GetPricesInView(CPacket* pPacket, const int nDataEnd, const CPrices& pricesInView, CPrices& prices) const;

	// Y Scale상에 종가 표시 수정 - ojtaso (20070807)
	int GetPosition_Y(const CSize& oneTextSize, const CDrawingGraphData& drawingGraphData, const double& dCur, const CRect& rcScaleText) const;
	CRect GetGripRegion(const CDrawingGraphData& drawingGraphData, const CRect& rcScaleText) const;
// vntsorl_20110609:[A00000259] 가격눈금 위의 현재가 표시 수정
	CRect GetTextRegion(CDC* pDC, const CDrawingGraphData& drawingGraphData, const CPrices& prices, const CString& strCurPrice, const CString& strDiffPrice, const CString& strPipPrice, const CString& strRiseFallRate, const CRect& rcScaleText) const;
	CRect GetTextRegion(CDC* pDC, const CPoint& rightTopPt, const CString& strCurPrice, const CString& strDiffPrice, const CString& strPipPrice, const CString& strRiseFallRate) const;
	CRect GetDrawingTextRegion(const CRect& textRegion, const int nPrevGap_Y) const;
	CPoint GetTextPosition(const int nOrg_x, const int nOrg_y, const CSize& textSize, const int textPos = DT_CENTER) const;
	COLORREF GetCurrentPriceTextColor(const CGraphColor& graphColor, const CPrices& prices) const;
	COLORREF GetTextColor(const COLORREF& bkColor) const;

// vntsorl_20110609:[A00000259] 가격눈금 위의 현재가 표시 수정
	bool GetText(const CRect& region, const CPrices& prices, int nCurDataType, CString& strCurPrice, CString& strDiffPrice, CString& strPipPrice, CString& strRiseFallRate) const;
	CString GetText_Price( double dPrice, const CString &p_strPacketType, const CString &p_strUnit, int nDecimalDownCount) const;
	CString GetText_CurrentPrice(const CPrices& prices, const CString &p_strPacketName) const;

	bool IsGraphDataDraw(const CRect& region) const;
	bool IsGraphDataDraw_Rate(const CRect& region, const CPrices& prices) const;

	bool GetPacketAndDataEnd(const CTextInRightScaleRegionDrawer& textData, CPacket*& pPacket, int& nRealDataEnd) const;
};


// ----------------------------------------------------------------------------
class CGraphDataDrawer  
{
public:
	void DrawGraphDatas(CDC* pDC, const CRect& realRegion, const CDisplayAttributes& displayAttributes, 
		const CDrawingGraphData& drawingGraphData, const CSubGraphData& subGraphData);

private:
	CDataInGraphRegionDrawer m_dataInGraphRegionDrawer;
	CDataInRightScaleRegionDrawer m_dataInRightScaleRegionDrawer;
};

#endif // !defined(AFX_GRAPHDATADRAWER_H__07EEA2C9_974A_4705_96FD_2FDF4E216EC0__INCLUDED_)
