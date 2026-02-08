// ChartCfg.cpp: implementation of the CChartCfg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartCfg.h"

#include "../Include_Chart/PropertyMap.h"		// for CPropertyMap

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChartCfg::~CChartCfg()
{
	// (2006/11/12 - Seung-Won, Bae) Manage AddIn ENV Value Map
	if( m_pAddInEnvMap)
	{
		delete m_pAddInEnvMap;
		m_pAddInEnvMap = NULL;
	}
}

CChartCfg::CChartCfg(	CPropertyMap *p_pAddInEnvMap,
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
						const bool bAllGraphTitleHiding, // sy 2004.11.29. 모든 graph title 숨김여부
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
						BOOL bDrawBaseLine,		// 기준선 보임 환경설정과 Scale 자동포함 기능 - ojtaso (20071101)
						const int p_nVertScaleType,
						const long p_nRightMarginWidth,
						const long p_nFutureTimeCount,	// (2009/5/17 - Seung-Won, Bae) for Future Time
						const int p_nMouseWheelIndex,
						const int p_nMinMaxRatio)
{
	// (2009/5/17 - Seung-Won, Bae) for Future Time
	m_nFutureTimeCount = p_nFutureTimeCount;

	// (2009/5/13 - Seung-Won, Bae) for Right Margin
	m_nRightMarginWidth = p_nRightMarginWidth;

	// (2006/11/12 - Seung-Won, Bae) Manage AddIn ENV Value Map
	m_pAddInEnvMap = p_pAddInEnvMap;

	m_nRowCount = nRowCount;
	m_nColumnCount = nColumnCount;
	m_nRowDoubleSizeBlock = nRowDoubleSizeBlock;
	m_nColumnDoubleSizeBlock = nColumnDoubleSizeBlock;
	m_nOnePageDataCount = nOnePageDataCount;
	m_nBongCountMargin = nBongCountMargin;
	m_strChartList = strChartList;
	m_strBlockRegion = strBlockRegion;
	m_bWholeView = bWholeView;
	m_clrBackground = clrBackground;
	m_clrBackBorder = clrBackBorder;
	m_clrBlock = clrBlock;
	m_clrBlockBorder = clrBlockBorder;
	m_clrScaleLine = clrScaleLine;
	m_clrScaleLine2 = clrScaleLine2;
	m_clrScaleText = clrScaleText;
	m_clrAnalysisTool = clrAnalysisTool;
	m_clrInquiryWindow = clrInquiryWindow;
	m_clrLattice = clrLattice;

	//sy 2005.12.01 drag 관련 내용(수치조회/확대축소)
	m_nNumericalInquiryOption = nNumericalInquiryOption;
	//sy end
	m_bUseConditiontoTitle = bUseConditiontoTitle;
	m_bUseDatatoTitle = bUseDatatoTitle;
	m_nVertGap = nVertGap;
	m_nHorzGap = nHorzGap;
	m_nYScalePosition = nYScalePosition;

	//nami 9.10
    m_nUseInquiryWithCrossline = nUseInquiryWithCrossline;
	m_bToolTipwithZoomIn = bToolTipwithZoomIn;

	m_bShowMinMaxbyPrice = bShowMinMaxbyPrice;

	//sy 2004.11.29. 모든 graph title 숨기기 여부
	m_bAllGraphTitleHiding = bAllGraphTitleHiding;

	m_bShowTrendLineText = bShowTrendLineText;
	m_bShowVerticalText = bShowVerticalText;
	
	// 종찬(04/10/14) 가격Text를 왼쪽, 오른쪽 구분해서 보이기.
	m_bShowHorizonTextLeft = bShowHorizonTextLeft;
	m_bShowHorizonTextRight = bShowHorizonTextRight;

	m_bShowThreeText = bShowThreeText;
	m_bShowFourText = bShowFourText;
	m_bShowFiboText = bShowFiboText;

	// (2006/10/23 - Seung-Won, Bae) Manage the Flag of Showing the Price Min/Max Data Text.
	m_bShowPriceMinMaxText = p_bShowPriceMinMaxText;

	// (2006/12/10 - Seung-Won, Bae) Support Map Default Info.
	m_strMapIndicatorInfo = p_szMapIndicatorInfo;

	// (2007/7/2 - Seung-Won, Bae) Do not Support Main Grid Type.
	//	for Old Version Map and User File.
	m_nVertScaleType = p_nVertScaleType;

	// 기준선 보임 환경설정과 Scale 자동포함 기능 - ojtaso (20071101)
	m_bDrawBaseLine = bDrawBaseLine;

	m_nMouseWheelIndex = p_nMouseWheelIndex;
	m_nMinMaxRatio = p_nMinMaxRatio;
}
