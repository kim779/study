// EnvironmentData.h: interface for the CEnvironmentData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ENVIRONMENTDATA_H__5F0179F5_E392_4ED3_AECC_6BB2C9C05512__INCLUDED_)
#define AFX_ENVIRONMENTDATA_H__5F0179F5_E392_4ED3_AECC_6BB2C9C05512__INCLUDED_

#include <afxtempl.h>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

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
// (2006/12/2 - Seung-Won, Bae) Support Variable Count of Price and Trade MA Type.
class CStyleData  
{
public:
	CStyleData();
	CStyleData(const CStyleData& styleData);
	CStyleData &operator=( const CStyleData& styleData);

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
	void SetPriceMACount( const int p_nCount);
	void SetTradeMACount( const int p_nCount);
	void SetPriceMAColor( const int p_nIndex, const COLORREF& clrPrice);
	void SetTradeMAColor( const int p_nIndex, const COLORREF& clrTrade);
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
	int		 GetPriceMACount( void) const;
	COLORREF GetPriceMAColor( const int p_nIndex) const;
	int		 GetTradeMACount( void) const;
	COLORREF GetTradeMAColor( const int p_nIndex) const;
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


//KHD : 그리드타입시간구분 
	int				GetHScaleGridType_Day() const				{	return 	m_nHScaleGridType_Day;		}//그리드타입 실선, 점선, 없음.
	int				GetHScaleGridType_Week() const				{	return 	m_nHScaleGridType_Week;		}//그리드타입 실선, 점선, 없음.
	int				GetHScaleGridType_Mon() const				{	return 	m_nHScaleGridType_Mon;		}//그리드타입 실선, 점선, 없음.
	int				GetHScaleGridType_Year() const				{	return 	m_nHScaleGridType_Year;		}//그리드타입 실선, 점선, 없음.

	COLORREF		GetHScaleGridColor_Day() const				{	return m_clrHScaleGridColor_Day;	}
	COLORREF		GetHScaleGridColor_Week() const				{	return m_clrHScaleGridColor_Week;	}
	COLORREF		GetHScaleGridColor_Mon() const				{	return m_clrHScaleGridColor_Mon;	}
	COLORREF		GetHScaleGridColor_Year() const				{	return m_clrHScaleGridColor_Year;	}
	//굵기 
	int				GetScaleLineThickness_Day() const				{	return 	m_scaleLineThickness_Day;		}//.
	int				GetScaleLineThickness_Week() const				{	return 	m_scaleLineThickness_Week;		}//.
	int				GetScaleLineThickness_Mon() const				{	return 	m_scaleLineThickness_Mon;		}//.
	int				GetScaleLineThickness_Year() const				{	return 	m_scaleLineThickness_Year;		}//.
	//KHD : 시간구분
	void			SetHScaleGridType_TimeGubun( const int &nHScaleGridType_Day, int nType);
	void			SetHScaleGridColor_TimeGubun( const COLORREF& clrHScaleGridColor, int nType);
	void			SetScaleLineThickness_TimeGubun(int nType ,  int scaleLineThickness);

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
	CDWordArray	m_priceColor;
	CDWordArray	m_tradeColor;
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

	//KHD : 시간구분 
	int         m_nHScaleGridType_Day; // 분/틱에서 일구분
	int         m_nHScaleGridType_Week;
	int         m_nHScaleGridType_Mon;
	int         m_nHScaleGridType_Year;
	
	COLORREF	m_clrHScaleGridColor_Day;
	COLORREF	m_clrHScaleGridColor_Week;
	COLORREF	m_clrHScaleGridColor_Mon;
	COLORREF	m_clrHScaleGridColor_Year;

	//굵기
	int         m_scaleLineThickness_Day;
	int         m_scaleLineThickness_Week;
	int         m_scaleLineThickness_Mon;
	int         m_scaleLineThickness_Year;

// (2009/9/21 - Seung-Won, Bae) To save the user default style.
public:
	BOOL	IsSameWith( CStyleData &p_styleData);
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

#endif // !defined(AFX_ENVIRONMENTDATA_H__5F0179F5_E392_4ED3_AECC_6BB2C9C05512__INCLUDED_)
