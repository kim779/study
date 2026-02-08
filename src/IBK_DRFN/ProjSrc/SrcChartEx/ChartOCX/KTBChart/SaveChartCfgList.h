// SaveChartCfgList.h: interface for the CSaveChartCfgList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SAVECHARTCFGLIST_H__64DA4842_FF93_11D5_9371_0050FC28B229__INCLUDED_)
#define AFX_SAVECHARTCFGLIST_H__64DA4842_FF93_11D5_9371_0050FC28B229__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

class CChartCfg;
class CPropertyMap;
class CSaveChartCfgList  
{
public:
	CSaveChartCfgList();
	virtual ~CSaveChartCfgList();

	bool AddChartCfg(	CPropertyMap *p_pAddInEnvMap,
						const int nRowCount, const int nColumnCount, 
						const int nRowDoubleSizeBlock, const int nColumnDoubleSizeBlock,
						const int nOnePageDataCount, const int nBongCountMargin, 
						const CString& strChartList, const CString& strBlockRegion,
						const BOOL bWholeView, 
						const int nNumericalInquiryOption,
						const int nYScalePosition,
						const int nUseInquiryWithCrossline,	const BOOL bToolTipwithZoomIn,
						const BOOL bUseConditiontoTitle, const BOOL bUseDatatoTitle,
						const int nVertGap, const int nHorzGap,
						const BOOL bShowMinMaxbyPrice, /*const bool bUseFullMinMax,*/	//교보와 다름
						const BOOL bAllGraphTitleHiding, 
						const BOOL bShowTrendLineText, const BOOL bShowVerticalText, 
						const BOOL bShowHorizonTextLeft, const BOOL bShowHorizonTextRight,// 종찬(04/10/14) 가격Text를 왼쪽, 오른쪽 구분해서 보이기.
						const BOOL bShowThreeText, const BOOL bShowFourText, const BOOL bShowFiboText,
						const COLORREF clrBackground, const COLORREF clrBackBorder,
						const COLORREF clrBlock, const COLORREF clrBlockBorder, 
						const COLORREF clrScaleLine, const COLORREF clrScaleLine2, const COLORREF clrScaleText, 
						const COLORREF clrAnalysisTool, const COLORREF clrInquiryWindow, const COLORREF clrLattice,
						// (2006/10/23 - Seung-Won, Bae) Manage the Flag of Showing the Price Min/Max Data Text.
						BOOL p_bShowPriceMinMaxText,
						// (2006/12/10 - Seung-Won, Bae) Support Map Default Info.
						const char *p_szMapIndicatorInfo,
						BOOL bDrawBaseLine,				// 기준선 보임 환경설정과 Scale 자동포함 기능 - ojtaso (20071101)
						const int p_nVertScaleType,
						const long p_nRightMargin,	// (2009/5/13 - Seung-Won, Bae) for Right Margin
						const long p_nFutureTimeCount,		// (2009/5/17 - Seung-Won, Bae) for Future Time
						const int p_nMouseWheelIndex,
						const int p_nMinMaxRatio
						);

	void RemoveAll();
	bool RemoveTail();

	int GetCfgCount();
	POSITION GetHeadPosition() const;
	POSITION GetTailPosition() const;
	CChartCfg* GetChartCfg(const int nIndex) const;
	CChartCfg* GetChartCfg(const int nIndex, const bool bDelete);
	CChartCfg* GetNext(POSITION& pos) const;
	CChartCfg* GetPrev(POSITION& pos) const;
	bool GetChartCfg(const CString& strGraphName, CChartCfg*& pChartCfg, int& nIndex) const;

// (2004.05.20, 배승원) DLL Container에서 지정한 Packet 초기화 상태까지만 RollBack 시키는 기능을 추가한다.
//		nIndex번째 부터, 그 이후의 모든 설정을 Clear 시키는 Interface를 제공한다.
	bool RemoveFromIndexToEnd(const int nIndex);

private:
	int GetIndex(CChartCfg* pChartCfg) const;

private:
	CTypedPtrList<CObList, CChartCfg*> m_lstChartCfg;
};

#endif // !defined(AFX_SAVECHARTCFGLIST_H__64DA4842_FF93_11D5_9371_0050FC28B229__INCLUDED_)
