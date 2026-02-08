// ElementTreeBuilder.h: interface for the CElementTreeBuilder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ELEMENTTREEBUILDER_H__8710710C_728D_4C73_AB31_BEAC7A452D6C__INCLUDED_)
#define AFX_ELEMENTTREEBUILDER_H__8710710C_728D_4C73_AB31_BEAC7A452D6C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Include_Analysis/ToolBaseData.h"
#include "../Include_Analysis/BlockBaseEnum2.h"		// for CAnalysis
#include "DequeList.h"
#include "BlockBaseData.h"

class CAnalysisData;
class CTree;
class COrder;
class CBlockIndex;
class CCoordinate;
class CElement;
class CTextPosition;
class CTextShowPosition;
class CTextHorzPosition;
class CTextVertPosition;
class CTextHorzVertPosition;
class CTextEnvironment;
class CElementCommonEnvironment;
class CPenEnvironment;
class CDataTypeEnvironment;
class CLineExtEnvironment;

// index를 구하기 위해 필요한 data.
class CElementAndOrder
{
public:
	CElementAndOrder(const int nIndex, CElement* pElement);
	CElementAndOrder& operator=(const CElementAndOrder& elementAndOrder);

	int GetIndex() const;
	CElement* GetElement() const;

private:
	int m_nIndex;
	CElement* m_pElement;
};


class CElementAndOrderSet
{
public:
	void AddTail(const CElementAndOrder& elementAndOrder);

	int GetCount() const;
	int GetMaxIndex() const;
	CElementAndOrder GetData(const int nIndex) const;
	CElement* GetElement(const int nIndex) const;

private:
	CDequeList<CElementAndOrder> m_ElementAndOrderSet;
};


// string --> element 로 만들기.
class CElementTreeBuilder  
{
public:
	CElementTreeBuilder( HWND p_hOcxWnd, CChartInfo::CHART_MODE p_eChartMode)
		{	m_hOcxWnd = p_hOcxWnd;	m_eChartMode = p_eChartMode;	}

public:
	// element 만들기.
	bool BuildElementsFromString(CAnalysisData& analysisData,const CString& strElementsData, CTree& elementTree, COrder& elementOrder);
	long BuildElementsFromString_ForUndoRedo(CAnalysisData& analysisData,const CBlockIndex& blockIndex, const CString& strElementData, 
		CTree& elementTree, COrder& elementOrder);

private:
	// element 만들기.
	// --> CTree
	bool BuildElementsFromString(CAnalysisData& analysisData,const CString& strElementsData, CTree& elementTree, CElementAndOrderSet& elementAndOrderSet);
	void BuildElementsFromString_Column(CAnalysisData& analysisData,const int nColumnIndex, const CString& strColumnData, 
		CTree& elementTree, CElementAndOrderSet& elementAndOrderSet);
	void BuildElementsFromString_Row(CAnalysisData& analysisData,const CBlockIndex& blockIndex, const CString& strRowData, 
		CTree& elementTree, CElementAndOrderSet& elementAndOrderSet);
	void BuildElementsFromString_Element(CAnalysisData& analysisData,const CBlockIndex& blockIndex, const CString& strElementData, 
		CTree& elementTree, CElementAndOrderSet& elementAndOrderSet);
	// --> COrder
	void BuildElementsFromString(CAnalysisData& analysisData,const CElementAndOrderSet& elementAndOrderSet, COrder& elementOrder);

	// element 추가.
	void AddElement(const CBlockIndex& blockIndex, const int nIndex, CElement* pElement, 
		CTree& elementTree, CElementAndOrderSet& elementAndOrderSet);
	// 20081007 JS.Kim	사용자시간대 선택
	CElement* MakeElement(const CString& strElementData, const int& nBlockTimeDiff);
	CElement* MakeElement(const CAnalysis::TOOLTYPE eToolType, const CDequeList<CCoordinate>& pointList, const CString& strTextPosition, const CElementCommonEnvironment& environment);
	CElement* MakeElement(const CAnalysis::TOOLTYPE eToolType, const CDequeList<CCoordinate>& pointList, const CTextPosition* pTextPosition, const CElementCommonEnvironment& environment);

	CString GetData(const CString& strOrgData, const CString& strTrimStart, const CString& strTrimEnd) const;
	int GetIndex(const CString& strData) const;
	CAnalysis::TOOLTYPE GetToolType(const CString& strData) const;
	// 20081007 JS.Kim	사용자시간대 선택
	void GetPoint(const CString& strData, const int& nBlockTimeDiff, CDequeList<CCoordinate>& pointList) const;
	// 20081007 JS.Kim	사용자시간대 선택
	CCoordinate GetPoint(const CString& strData, const int& nBlockTimeDiff) const;
	bool GetTextShowPosition(const CString& strData, CTextShowPosition& position) const;// 2008.08.20 김진순 피보나치
	bool GetTextHorzPosition(const CString& strData, CTextHorzPosition& position) const;
	bool GetTextVertPosition(const CString& strData, CTextVertPosition& position) const;
	bool GetTextHorzVertPosition(const CString& strData, CTextHorzVertPosition& position) const;
	CTextEnvironment GetTextEnvironment(const CString& strData) const;
	CElementCommonEnvironment GetEnvironment(const CString& strData) const;
	bool GetAutoPrice(const CString& strData) const;
	int GetEquiDivEnvironment(const CString& strData) const;
	int GetPriceRangeDivEnvironment(const CString& strData) const;	// (2008/8/15 - Seung-Won, Bae) Add option for using price range.
	CLineExtEnvironment GetLineExtEnvironment(const CString& strData) const;
	int GetLeftLineExt(const CString& strData) const;
	int GetRightLineExt(const CString& strData) const;
	CPenEnvironment GetPenEnvironment(const CString& strData) const;
	COLORREF GetColor(const CString& strData) const;
	int GetWidth(const CString& strData) const;
	CToolEnumData::PenStyle GetStyle(const CString& strData) const;
	CDataTypeEnvironment GetDataTypeEnvironment(const CString& strData) const;
	CAnalysis::COORDINATETYPE GetCoordinateType(const CString& strData) const;
	double GetVertDataType(const CString& strData) const;

	CString GetTrimData(CString& strData, const CString& strTrimData) const;

// (2008/5/14 - Seung-Won, Bae) for Multi-Language of Special Date or Price Unit Text
protected:
	HWND	m_hOcxWnd;

// (2009/9/15 - Seung-Won, Bae) Do not support special time for WorldOn.
protected:
	CChartInfo::CHART_MODE	m_eChartMode;
};

#endif // !defined(AFX_ELEMENTTREEBUILDER_H__8710710C_728D_4C73_AB31_BEAC7A452D6C__INCLUDED_)
