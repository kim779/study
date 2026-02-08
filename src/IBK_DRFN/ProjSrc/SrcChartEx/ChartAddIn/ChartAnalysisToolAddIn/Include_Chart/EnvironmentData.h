// EnvironmentData.h: interface for the CEnvironmentData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ENVIRONMENTDATA_H__5F0179F5_E392_4ED3_AECC_6BB2C9C05512__INCLUDED_)
#define AFX_ENVIRONMENTDATA_H__5F0179F5_E392_4ED3_AECC_6BB2C9C05512__INCLUDED_

#include <afxtempl.h>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//수정주가
class CModifyPriceData  
{
public:
	CModifyPriceData();
	CModifyPriceData(const bool bModifyPrice, const bool bDevidedlak);
	void SetSavedData(const CModifyPriceData& modifypricedata);

	void SetDevidedlak(const bool bDevidedlak);
	void SetModifyPrice(const bool bmodifyprice);

	bool IsDevidedlak() const;
	bool IsModifyPrice() const;

private:
	bool m_bModifyPrice;
	bool m_bDevidedlak;
};

//수치조회, 데이타보기
class CInquiryViewData  
{
public:
	CInquiryViewData();
	CInquiryViewData(const int nNumericalINquiryDlgType,	// 종찬(05/06/08) 수치조회창 타입.
					 const int nInquiryDataView, const bool bViewTotalData, const int nDataCountPerOnePage, const int nViewTool, const bool bShowInquiry, const int nDataCount = 500, const int nMinDataCount = 20,
					 const bool bViewLowHigh = false, const bool bViewLow = false, const bool bViewHigh = false,
					 const bool bViewPrice = false, const bool bViewPercent = false,
					 // 0310 추세선추가
					 const bool bShowTrendLine = true);

	void SetSavedData(const CInquiryViewData& viewdata);

	void SetShowInquiry(const bool bShowInquiry);
	void SetViewTool(const int nViewTool);
	void SetDataCountPreOnePage(const int nDataCount);
	void SetViewTotalData(const bool bViewTotalData);
	void SetInquiryDataView(const int ninquirydataview);
	void SetNumericalInquiryDlgType(const int nNumericalInquiryDlgType);	// 종찬(05/06/08) 수치조회창 타입.
	// modified
	void SetDataCount(const int nDataCount);
	void SetMinDataCount(const int nMinDataCount);
	// 1212
	void SetViewLowHigh(const bool bViewLowHigh);
	void SetViewLow(const bool bViewLow);
	void SetViewHigh(const bool bViewHigh);
	void SetViewPrice(const bool bViewPrice);
	void SetViewPercent(const bool bViewPercent);
	// 0310 추세선추가
	void SetShowTrendLine(const bool bShowTrendLine);
	
	bool GetShowInquiry() const;
	int GetNumericalInquiryDlgType() const;	// 종찬(05/06/08) 수치조회창 타입.
	int GetViewTool() const;
	int GetDataCountPerOnePage() const;
	bool IsViewTotalData() const;
	int GetInquiryDataView() const;
	// modified
	int GetDataCount() const;
	int GetMinDataCount() const;
	// 1212
	bool GetViewLowHigh() const;
	bool GetViewLow() const;
	bool GetViewHigh() const;
	bool GetViewPrice() const;
	bool GetViewPercent() const;
	// 0310 추세선추가
	bool GetShowTrendLine() const;
	
private:
	//데이타보기
	bool m_bViewTotalData;
	int	m_nDataCountPerOnePage;

	// 종찬(05/06/08) 수치조회창 타입
	int m_nNumericalInquiryDlgType;
	
	//수치조회
	int m_nInquiryDataView;

	// 수치조회도구 (없음, 다이얼로그창, 툴팁)
	int	m_nViewTool;

	// 수치조회 띄움/안띄움
	bool m_bShowInquiry;

	// modified 데이터조회 개수
	int	m_nDataCount;
	// 최소 데이터 갯수
	int m_nMinDataCount;

	// 1212
	bool m_bViewLowHigh;
	bool m_bViewLow;
	bool m_bViewHigh;
	bool m_bViewPrice;
	bool m_bViewPercent;

	// 0310 추세선추가
	bool m_bShowTrendLine;
};

//scale
class CScaleData
{
public:
	CScaleData();
	CScaleData(const int nRightMargin, const int nScalePosition);

	void SetSavedData(const CScaleData& scaledata);

	void SetScalePosition(const int nScalePosition);
	void SetRightMargin(const int nRightSpace);
	void SetXScaleExistence(const bool bInsertXScale);

	int GetScalePosition() const;
	int GetRightMargin() const;
	bool DoXScaleInsert() const;

private:
	//여백
	int	m_nRightMargin;
	//scale
	int m_nScalePosition;
};

//color
class CColorData  
{
public:
	CColorData();
	CColorData(const COLORREF& BackGroundColor, const COLORREF& BackBorderColor,
			   const COLORREF& HorizonScaleLineColor, const COLORREF& VerticalScaleLineColor, const COLORREF& ScaleTextColor,
			   const COLORREF& BlockGroundColor, const COLORREF& BlockBorderColor, const COLORREF& LatticeColor,
			   const COLORREF& AnalyzeToolColor, const COLORREF& NumericalColor);

	void SetSavedData(const CColorData& colordata);

	void SetLatticeColor(const COLORREF& clrLattice);
	void SetNumericalViewColor(const COLORREF& clrNumericalView);
	void SetAnalyzeToolColor(const COLORREF& clrAnalyzeTool);
	void SetScaleTextColor(const COLORREF& clrScaleText);
	void SetVerticalScaleLineColor(const COLORREF& clrVerticalScaleLine);
	void SetHorizonScaleLineColor(const COLORREF& clrHorizonScaleLine);
	void SetBlockBorderColor(const COLORREF& clrBorderColor);
	void SetBlockGroundColor(const COLORREF& clrBlockGround);
	void SetBackBorderColor(const COLORREF& clrBackBorder);
	void SetBackGroundColor(const COLORREF& clrbackground);

	COLORREF GetLatticeColor() const;
	COLORREF GetNumericalViewColor() const;
	COLORREF GetAnalyzeToolColor() const;
	COLORREF GetScaleTextColor() const;
	COLORREF GetVerticalScaleLineColor() const;
	COLORREF GetHorizonScaleLineColor() const;
	COLORREF GetBlockBorderColor() const;
	COLORREF GetBlockGroundColor() const;
	COLORREF GetBackBorderColor() const;
	COLORREF GetBackGroundColor() const;

private:
	COLORREF m_BackGroundColor;
	COLORREF m_BackBorderColor;
	COLORREF m_BlockGroundColor;
	COLORREF m_BlockBorderColor;
	COLORREF m_HorizonScaleLineColor;
	COLORREF m_VerticalScaleLineColor;
	COLORREF m_ScaleTextColor;
	COLORREF m_AnalyzeToolColor;
	COLORREF m_NumericalColor;
	COLORREF m_LatticeColor;
};


///mouse DragDrop시
class CMouseEventData  
{
public:
	CMouseEventData();
	CMouseEventData(const int nMouseEvent);
	virtual ~CMouseEventData();

	void SetSavedData(const CMouseEventData& mouseeventdata);

	void SetMouseEvent(const int bMouseEvent);
	int GetMouseEvent() const;

private:
	int m_nMouseEvent;
};


////////////////////
///title 설정
class CTitleData
{
public:
	CTitleData();
	CTitleData(const bool bUseCondition, const bool bUseRecentData);
	virtual ~CTitleData();

	bool IsDisplayCondition() const;		
	bool IsDisplayRecentData() const;

	void SetYesorNotDisplayCondition(const int bUseCondition);
	void SetYesorNotDisplayRecentData(const int bUseRecentData);
	void SetSavedData(const CTitleData& titledata);

private:
	bool m_bUseConditiontoTitle;
	bool m_bUseRecentDatatoTitle;
};


//sy 2005.04.19.
class CMATitleListData
{
public:
	const CList<CString, CString>& GetPriceMATitleList() const;
	const CList<CString, CString>& GetTradeMATitleList() const;
	CList<CString, CString>& GetPriceMATitleList();
	CList<CString, CString>& GetTradeMATitleList();
	CList<CString, CString>* GetPriceMATitleList_Pointer();
	CList<CString, CString>* GetTradeMATitleList_Pointer();

	void SetMATitleListData(const CMATitleListData& sourceMATitleListData);

	void AddTail_PriceMA(const CString& strTitle);
	void AddTail_TradeMA(const CString& strTitle);
	void RemoveAll();
	void RemovePriceMA();
	void RemoveTradeMA();

private:
	void SetTitleListData(const CList<CString, CString>& sourceData, CList<CString, CString>& targetData);

private:
	CList<CString, CString> m_priceMATitleList;
	CList<CString, CString> m_tradeMATitleList;
};
//sy end

//sy 2005.04.19.
//style
class CStyleData  
{
public:
	CStyleData();
	CStyleData(const CStyleData& styleData);
	CStyleData(const CString& styleName,
			   const COLORREF& priceUpColor, const COLORREF& priceDownColor, const COLORREF& priceEqualColor,
			   const bool& priceUpFill, const bool& priceDownFill,
			   const COLORREF& tradeUpColor, const COLORREF& tradeDownColor, const COLORREF& tradeEqualColor,
			   const bool& tradeUpFill, const bool& tradeDownFill,
			   const COLORREF& priceColor1, const COLORREF& priceColor2, const COLORREF& priceColor3, const COLORREF& priceColor4, const COLORREF& priceColor5, 
			   const COLORREF& priceColor6, const COLORREF& priceColor7, const COLORREF& priceColor8, const COLORREF& priceColor9, const COLORREF& priceColor10, 
			   const COLORREF& tradeColor1, const COLORREF& tradeColor2, const COLORREF& tradeColor3, const COLORREF& tradeColor4, 
			   const COLORREF& graphBGColor, const COLORREF& graphBorderColor, const COLORREF& graphGridColor,
			   const COLORREF& scaleBGColor, const COLORREF& scaleBorderColor, const COLORREF& scaleTextColor, const COLORREF& scaleGridColor,
			   const int& gridWidth, const int& gridHeight,
			   const int& divideWidth, const int& divideHeight,
			   const COLORREF& analyzeColor = RGB(0,0,255), const COLORREF& numericalColor = RGB(255,255,255));

	void SetSavedData(const CStyleData& styleData);

	void SetStyleName(const CString& name);
	void SetPriceUpColor(const COLORREF& clrPriceUp);
	void SetPriceDownColor(const COLORREF& clrPriceDown);
	void SetPriceEqualColor(const COLORREF& clrPriceEqual);
	void SetPriceUpFill(const bool& priceUpFill);
	void SetPriceDownFill(const bool& priceDownFill);
	void SetTradeUpColor(const COLORREF& clrTradeUp);
	void SetTradeDownColor(const COLORREF& clrTradeDown);
	void SetTradeEqualColor(const COLORREF& clrTradeEqual);
	void SetTradeUpFill(const bool& tradeUpFill);
	void SetTradeDownFill(const bool& tradeDwonFill);
	void SetPriceColor1(const COLORREF& clrPrice);
	void SetPriceColor2(const COLORREF& clrPrice);
	void SetPriceColor3(const COLORREF& clrPrice);
	void SetPriceColor4(const COLORREF& clrPrice);
	void SetPriceColor5(const COLORREF& clrPrice);
	void SetPriceColor6(const COLORREF& clrPrice);
	void SetPriceColor7(const COLORREF& clrPrice);
	void SetPriceColor8(const COLORREF& clrPrice);
	void SetPriceColor9(const COLORREF& clrPrice);
	void SetPriceColor10(const COLORREF& clrPrice);
	void SetTradeColor1(const COLORREF& clrTrade);
	void SetTradeColor2(const COLORREF& clrTrade);
	void SetTradeColor3(const COLORREF& clrTrade);
	void SetTradeColor4(const COLORREF& clrTrade);
	void SetGraphBGColor(const COLORREF& clrGraphBG);
	void SetGraphBorderColor(const COLORREF& clrGraphBorder);
	void SetGraphGridColor(const COLORREF& clrGraphGrid);
	void SetScaleBGColor(const COLORREF& clrScaleBG);
	void SetScaleBorderColor(const COLORREF& clrScaleBorder);
	void SetScaleTextColor(const COLORREF& clrScaleText);
	void SetScaleGridColor(const COLORREF& clrScaleGrid);
	void SetGridWidth(const int& gridWidth);
	void SetGridHeight(const int& gridHeight);
	void SetDivideWidth(const int& divideWidth);
	void SetDivideHeight(const int& divideHeight);
	void SetAnalyzeColor(COLORREF clrAnalyze);
	void SetNumericalColor(COLORREF clrNumerical);

	CString	 GetStyleName() const;
	COLORREF GetPriceUpColor() const;
	COLORREF GetPriceDownColor() const;
	COLORREF GetPriceEqualColor() const;
	bool	 GetPriceUpFill() const;
	bool	 GetPriceDownFill() const;
	COLORREF GetTradeUpColor() const;
	COLORREF GetTradeDownColor() const;
	COLORREF GetTradeEqualColor() const;
	bool	 GetTradeUpFill() const;
	bool	 GetTradeDownFill() const;
	COLORREF GetPriceColor1() const;
	COLORREF GetPriceColor2() const;
	COLORREF GetPriceColor3() const;
	COLORREF GetPriceColor4() const;
	COLORREF GetPriceColor5() const;
	COLORREF GetPriceColor6() const;
	COLORREF GetPriceColor7() const;
	COLORREF GetPriceColor8() const;
	COLORREF GetPriceColor9() const;
	COLORREF GetPriceColor10() const;
	COLORREF GetTradeColor1() const;
	COLORREF GetTradeColor2() const;
	COLORREF GetTradeColor3() const;
	COLORREF GetTradeColor4() const;
	COLORREF GetGraphBGColor() const;
	COLORREF GetGraphBorderColor() const;
	COLORREF GetGraphGridColor() const;
	COLORREF GetScaleBGColor() const;
	COLORREF GetScaleBorderColor() const;
	COLORREF GetScaleTextColor() const;
	COLORREF GetScaleGridColor() const;
	int		 GetGridWidth() const;
	int		 GetGridHeight() const;
	int		 GetDivideWidth() const;
	int		 GetDivideHeight() const;
	COLORREF GetAnalyzeColor() const;
	COLORREF GetNumericalColor() const;

	CMATitleListData& GetMATitleListData();
	const CMATitleListData& GetMATitleListData() const;
	void SetMATitleListData(const CMATitleListData& sourceTitleListData);

private:
	CString		m_styleName;
	COLORREF	m_priceUpColor;
	COLORREF	m_priceDownColor;
	COLORREF	m_priceEqualColor;
	bool		m_priceUpFill;
	bool		m_priceDownFill;
	COLORREF	m_tradeUpColor;
	COLORREF	m_tradeDownColor;
	COLORREF	m_tradeEqualColor;
	bool		m_tradeUpFill;
	bool		m_tradeDownFill;
	COLORREF	m_priceColor1;
	COLORREF	m_priceColor2;
	COLORREF	m_priceColor3;
	COLORREF	m_priceColor4;
	COLORREF	m_priceColor5;
	COLORREF	m_priceColor6;
	COLORREF	m_priceColor7;
	COLORREF	m_priceColor8;
	COLORREF	m_priceColor9;
	COLORREF	m_priceColor10;
	COLORREF	m_tradeColor1;
	COLORREF	m_tradeColor2;
	COLORREF	m_tradeColor3;
	COLORREF	m_tradeColor4;
	COLORREF	m_graphBGColor;
	COLORREF	m_graphBorderColor;
	COLORREF	m_graphGridColor;
	COLORREF	m_scaleBGColor;
	COLORREF	m_scaleBorderColor;
	COLORREF	m_scaleTextColor;
	COLORREF	m_scaleGridColor;
	int			m_gridWidth;
	int			m_gridHeight;
	int			m_divideWidth;
	int			m_divideHeight;
	COLORREF	m_analyzeColor;
	COLORREF	m_numericalColor;

	CMATitleListData m_titleListData;
};

// 분석도구 설정시
// 0114
class CAnalysisToolData  
{
public:
	CAnalysisToolData();
	CAnalysisToolData(const bool bShowTrendLine,
					const bool bShowVertical = true,
					const bool bShowHorizonLeft = true,
					const bool bShowHorizonRight = true,
					const bool bShowThree = true,
					const bool bShowFour = true,
					const bool bShowFibo = true);
	virtual ~CAnalysisToolData();

	void SetSavedData(const CAnalysisToolData& analysistooldata);

	void SetShowTrendLine(const bool bShowTrendLine);
	int GetShowTrendLine() const;
	void SetShowVertical(const bool bShowVertical);
	int GetShowVertical() const;
	void SetShowHorizonLeft(const bool bShowHorizonLeft);
	int GetShowHorizonLeft() const;
	void SetShowHorizonRight(const bool bShowHorizonRight);
	int GetShowHorizonRight() const;
	void SetShowThree(const bool bShowThree);
	int GetShowThree() const;
	void SetShowFour(const bool bShowFour);
	int GetShowFour() const;
	void SetShowFibo(const bool bShowFibo);
	int GetShowFibo() const;

private:
	int m_bShowTrendLine;
	int m_bShowVertical;
	// 종찬(04/10/14) 가격Text를 왼쪽, 오른쪽 구분해서 보이기.
	int m_bShowHorizonLeft;
	int m_bShowHorizonRight;
	int m_bShowThree;
	int m_bShowFour;
	int m_bShowFibo;
};

class CEnvironmentData  
{
public:
	CEnvironmentData();
	CEnvironmentData(CWnd* pParent,
					 const int defaultTab,
					 const bool bUseOneChart,
					 const CModifyPriceData& modifypricedata,
					 const CInquiryViewData& inquiryviewdata,
					 const CScaleData& scaledata,
					 const CColorData& colordata,
					 const CMouseEventData& mouseeventdata,
					 const CTitleData& titledata,
					 const CStyleData& styledata,
					 // 0114
					 const CAnalysisToolData& analysistooldata,
					 //sy 2005.04.19.
					 const CMATitleListData& maTitleListData, 
					 const CString defaultPath,
					 const CString userPath);

	CColorData* GetColorData();
	CScaleData* GetScaleData();
	CInquiryViewData* GetInquiryViewData();
	CModifyPriceData* GetModifyPriceData();
	CMouseEventData* GetMouseEventData();
	CTitleData* GetTitleData();
	CStyleData*	GetStyleData();
	// 0114
	CAnalysisToolData* GetAnalysisToolData();

	//sy 2005.04.19.
	CMATitleListData& GetMATitleListData();
	const CMATitleListData& GetMATitleListData() const;
	void SetMATitleListData(const CMATitleListData& sourceTitleListData);
	//sy end

	CColorData GetColorData() const;
	CScaleData GetScaleData() const;
	CInquiryViewData GetInquiryViewData() const;
	CModifyPriceData GetModifyPriceData() const;
	CMouseEventData GetMouseEventData() const;
	CTitleData GetTitleData() const;
	CStyleData GetStyleData() const;
	// 0114
	CAnalysisToolData GetAnalysisToolData() const;

	CString	GetDefaultPath() const;
	CString	GetUserPath() const;

	int GetDefaultTab() const;
	CWnd* GetParent() const;
	bool DoUseOneChart() const;

	void SetSavedData(const CEnvironmentData& environmentdata);

private:
	int m_defaultTab;
	CWnd* m_pParent;
	bool m_bUseOneChart;
	
	CModifyPriceData m_modifypricedata;
	CInquiryViewData m_inquiryviewdata;
	CScaleData m_scaledata;
	CColorData m_colordata;
	CMouseEventData m_mouseevetdata;
	CTitleData m_titledata;
	CStyleData m_styledata;
	CString	m_defaultPath;
	CString	m_userPath;
	// 0114
	CAnalysisToolData m_analysistooldata;
	//sy 2005.04.19.
	//CMATitleListData m_titleListData;
};


#endif // !defined(AFX_ENVIRONMENTDATA_H__5F0179F5_E392_4ED3_AECC_6BB2C9C05512__INCLUDED_)
