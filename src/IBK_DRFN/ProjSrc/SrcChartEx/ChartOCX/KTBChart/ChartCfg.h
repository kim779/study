// ChartCfg.h: interface for the CChartCfg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARTCFG_H__64DA4844_FF93_11D5_9371_0050FC28B229__INCLUDED_)
#define AFX_CHARTCFG_H__64DA4844_FF93_11D5_9371_0050FC28B229__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPropertyMap;
class CChartCfg : public CObject  
{
public:
	CChartCfg(	CPropertyMap *p_pAddInEnvMap,
				int nRowCount, int nColumnCount, int nRowDoubleSizeBlock, int nColumnDoubleSizeBlock,
				int nOnePageDataCount, int nBongCountMargin, 
				CString strChartList, CString strBlockRegion,
				const bool bWholeView, 
				const int nNumericalInquiryOption, 
				const int nYScalePosition,
				const int nUseInquiryWithCrossline,
				const bool bToolTipwithZoomIn,
				const bool bUseConditiontoTitle, const bool bUseDatatoTitle,
				const int nVertGap, const int nHorzGap,
				const bool bShowMinMaxbyPrice, /*const bool bUseFullMinMax,*/	//교보와 다름 
				const bool bAllGraphTitleHiding, //sy 2004.11.29. 모든 graph title 숨기기 여부
				const bool bShowTrendLineText,
				const bool bShowVerticalText, 
				const bool bShowHorizonTextLeft, const bool bShowHorizonTextRight, // 종찬(04/10/14) 가격Text를 왼쪽, 오른쪽 구분해서 보이기.
				const bool bShowThreeText, const bool bShowFourText, const bool bShowFiboText,
				COLORREF clrBackground, COLORREF	clrBackBorder,
				COLORREF clrBlock, COLORREF clrBlockBorder, COLORREF clrScaleLine,
				COLORREF	clrScaleLine2, COLORREF clrScaleText, COLORREF clrAnalysisTool, 
				COLORREF	clrInquiryWindow, COLORREF clrLattice,
				// (2006/10/23 - Seung-Won, Bae) Manage the Flag of Showing the Price Min/Max Data Text.
				BOOL p_bShowPriceMinMaxText,
				// (2006/12/10 - Seung-Won, Bae) Support Map Default Info.
				const char *p_szMapIndicatorInfo,
				BOOL bDrawBaseLine,			// 기준선 보임 환경설정과 Scale 자동포함 기능 - ojtaso (20071101)
				const int p_nVertScaleType,
				const long p_nRightMargin,	// (2009/5/13 - Seung-Won, Bae) for Right Margin
				const long p_nFutureTimeCount,		// (2009/5/17 - Seung-Won, Bae) for Future Time
				const int p_nMouseWheelIndex,
				const int p_nMinMaxRatio);

	virtual ~CChartCfg();

	int GetRowCount()	{return m_nRowCount;}
	int GetColumnCount()	{return m_nColumnCount;}
	int GetRowDoublesizeBlock()	{return m_nRowDoubleSizeBlock;}
	int GetColumnDoublesizeBlock()	{return m_nColumnDoubleSizeBlock;}
	int GetOnePageDataCount()	{return m_nOnePageDataCount;}
	int GetBongCountMargin()	{return m_nBongCountMargin;}

	CString GetChartList()	{return m_strChartList;}
	// (2004.09.10, 배승원) DLL Container가 있는 경우의 전체 초기화는 모든 지표의 속성을 Default 속성의 것으로 재수정한다.
	//		초기화하기 이전에 변경된 Default 속성(User Default)으로 설정되도록 한다.
	//		이를 위해 Chart CFG의 Chart List만 재설정하는 기능을 추가한다.
	void	SetChartList( const char *p_szChartList)	{	m_strChartList = p_szChartList;	}

	CString GetBlockRegion()	{return m_strBlockRegion;}
	bool IsWholeView()	{return m_bWholeView;}
	COLORREF GetclrBackground()	{return m_clrBackground;}
	COLORREF GetclrBackBorder()	{return m_clrBackBorder;}
	COLORREF GetclrBlock()	{return m_clrBlock;}
	COLORREF GetclrBlockBorder()	{return m_clrBlockBorder;}
	COLORREF GetclrScaleLine()	{return m_clrScaleLine;}
	COLORREF GetclrScaleLine2()	{return m_clrScaleLine2;}
	COLORREF GetclrScaleText()	{return m_clrScaleText;}
	COLORREF GetclrAnalysisTool()	{return m_clrAnalysisTool;}
	COLORREF GetclrInquiryWindow()	{return m_clrInquiryWindow;}
	COLORREF GetclrLattice() {return m_clrLattice;}

	//nami 9.7
	bool UseConditiontoTitle()	{return m_bUseConditiontoTitle;}
	bool UseDatatoTitle()	{return m_bUseDatatoTitle;}
	//sy 2005.12.01 drag 관련 내용(수치조회/확대축소)
	int GetNumericalInquiryOption() const { return m_nNumericalInquiryOption; };
	//sy end
	int GetVertGap()	{return m_nVertGap;}
	int GetHorzGap()	{return m_nHorzGap;}
	int GetYScalePosition() {return m_nYScalePosition;}

	//nami 9.10
	int GetUseInquiryWithCrossline() {return m_nUseInquiryWithCrossline;}
	bool UseToolTipwithZoomIn()	{return m_bToolTipwithZoomIn;}

	bool showMinMaxbyPrice() {return m_bShowMinMaxbyPrice;}

	//sy 2004.11.29. 모든 graph title 숨기기 여부
	bool IsAllGraphTitleHiding() { return m_bAllGraphTitleHiding; }

	bool showTrendLineText() {return m_bShowTrendLineText;}
	bool showVerticalText() {return m_bShowVerticalText;}
	// 종찬(04/10/14) 가격Text를 왼쪽, 오른쪽 구분해서 보이기.
	bool showHorizonTextLeft() {return m_bShowHorizonTextLeft;}
	bool showHorizonTextRight() {return m_bShowHorizonTextRight;}
	bool showThreeText() {return m_bShowThreeText;}
	bool showFourText() {return m_bShowFourText;}
	bool showFiboText() {return m_bShowFiboText;}

private:
	int		m_nRowCount;
	int		m_nColumnCount;
	int		m_nRowDoubleSizeBlock;
	int		m_nColumnDoubleSizeBlock;
	int		m_nOnePageDataCount;
	int		m_nBongCountMargin;
	//nami 9.10
	CString m_strChartList;
	CString m_strBlockRegion;
	bool	m_bWholeView;
	COLORREF	m_clrBackground;
	COLORREF	m_clrBackBorder;
	COLORREF	m_clrBlock;
	COLORREF	m_clrBlockBorder;
	COLORREF	m_clrScaleLine;
	COLORREF	m_clrScaleLine2;
	COLORREF	m_clrScaleText;
	COLORREF	m_clrAnalysisTool;
	COLORREF	m_clrInquiryWindow;
	COLORREF    m_clrLattice;

	//sy 2005.12.01 drag 관련 내용(수치조회/확대축소)
	int m_nNumericalInquiryOption;
	//sy end
	bool m_bUseConditiontoTitle;
	bool m_bUseDatatoTitle;
	int m_nVertGap;
	int m_nHorzGap;
	int m_nYScalePosition;

	//nami 9.10
    int m_nUseInquiryWithCrossline;
	bool m_bToolTipwithZoomIn;

	bool m_bShowMinMaxbyPrice;
	bool m_bAllGraphTitleHiding;
	bool m_bShowTrendLineText;
	bool m_bShowVerticalText;
	// 종찬(04/10/14) 가격Text를 왼쪽, 오른쪽 구분해서 보이기.
	bool m_bShowHorizonTextLeft;
	bool m_bShowHorizonTextRight;
	bool m_bShowThreeText;
	bool m_bShowFourText;
	bool m_bShowFiboText;

// (2006/10/23 - Seung-Won, Bae) Manage the Flag of Showing the Price Min/Max Data Text.
protected:
	BOOL	m_bShowPriceMinMaxText;
public:
	BOOL	WithShowPriceMinMaxText( void)	{	return m_bShowPriceMinMaxText;	}

// (2006/11/12 - Seung-Won, Bae) Manage AddIn ENV Value Map
protected:
	CPropertyMap *	m_pAddInEnvMap;
public:
	CPropertyMap *	GetAddInEnvMap( void)	{	return m_pAddInEnvMap;	}

// (2006/12/10 - Seung-Won, Bae) Support Map Default Info.
protected:
	CString			m_strMapIndicatorInfo;
public:
	const char *	GetMapIndicatorInfo( void) const	{	return m_strMapIndicatorInfo;	}

// (2007/7/2 - Seung-Won, Bae) Do not Support Main Grid Type.
//	for Old Version Map and User File.
protected:
	int m_nVertScaleType;
public:
	int GetVertScaleType()	{return m_nVertScaleType;}

// 기준선 보임 환경설정과 Scale 자동포함 기능 - ojtaso (20071101)
protected:
	BOOL m_bDrawBaseLine;
public:
	BOOL GetDrawBaseLine()	{ return m_bDrawBaseLine; }

// (2009/5/13 - Seung-Won, Bae) for Right Margin
protected:
	long	m_nRightMarginWidth;
public:
	long	GetRightMarginWidth()		{	return m_nRightMarginWidth;	}

// (2009/5/17 - Seung-Won, Bae) for Future Time
protected:
	long	m_nFutureTimeCount;
public:
	long	GetFutureTimeCount()		{	return m_nFutureTimeCount;	}
protected:
	long	m_nMouseWheelIndex;
public:
	long	GetMouseWheelIndex()		{	return m_nMouseWheelIndex;	}
protected:
	long	m_nMinMaxRatio;
public:
	long	GetPriceMinMaxShowType()		{	return m_nMinMaxRatio;	}
};

#endif // !defined(AFX_CHARTCFG_H__64DA4844_FF93_11D5_9371_0050FC28B229__INCLUDED_)
