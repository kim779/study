// ElementTree.h: interface for the CElementTree class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ELEMENTTREE_H__A3A4BD06_2494_46E3_85DC_01B5249D09E4__INCLUDED_)
#define AFX_ELEMENTTREE_H__A3A4BD06_2494_46E3_85DC_01B5249D09E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Include_Analysis/Include_AddIn.h"				// for Ixxxxxx.h
#include "../Include_Analysis/ToolBaseData.h"

#include "../Include_Chart/DequeList.h"
#include "../Include_Chart/BlockBaseData.h"

class CAnalysisData;
class CElement;
class CColumnSet;
class CExtractingDataOfModifyType;

// element의 순서 저장
class COrder
{
public:
	int GetCount() const;
	// 해당 element의 순서 가져오기.
	int GetIndex(const CElement* pElement) const;

	// element 추가
	void AddTail(CElement* pElement);

	// list에 모두 지우기.
	void RemoveAll();
	// 마지막 지우기.
	void RemoveTail();
	// list에 지우기.
	void Remove(const CDequePointerList<CElement>& elementList);
	// list에 지우기.
	void Remove(CElement* pElement);

	// element 순서 바꾸기
	bool ChangeOrder(CElement* pElement);

	// list의 마지막 data 가져오기.
	CElement* GetTail();
	CElement* GetAt(int nIndex);

private:
	CDequePointerList<CElement> m_ElementOrderList;

	// element 순서를 바꿀지의 여부
	bool IsChangeOrder(CElement* pElement, const int nIndex) const;
};


// element의 각 영역별로 저장
class CTree
{
public:
	virtual ~CTree();

	void SetAutoPrice(const bool bIsAutoPrice);

	// element 색 변경하기.
	void ChangeAllElementsColor(const COLORREF& penColor, const COLORREF& textColor);

	//[A00000552]alzioyes_20110628 element 선두께 변경하기.
	void ChangeAllElementsThick(int nElementType, int nNewWidth);

	// 저장할 모든 element data.
	CString GetSavedElementsData(const COrder& order) const;
	// 해당 elements 가져오기.
	void GetElementList(const CBlockIndex& blockIndex, CDequePointerList<CElement>& elementList);

	// element 추가
	void Add(const CBlockIndex& blockIndex, CElement* pElement);
	void AddColumnRowSet(const CBlockIndex& blockIndex);

	// block 추가시 elementTree을 추가하기.
	bool InsertElementTree(const CBlockBaseData::BLOCKTYPE blockType, const CBlockIndex& blockIndex);

	// CColumnSet 삭제
	void DeleteAll();
	// CRowSet 삭제
	void DeleteRowSet(const CBlockIndex& blockIndex);
	// 특정 tyle의 element만 삭제
	bool DeleteElement(const CToolEnumData::ElementType eElementTYPE, CDequePointerList<CElement>& deletedElementList);
	// element 삭제
	void DeleteElement(CElement* pElement);
	// element 삭제
	bool DeleteElement(const long lElementAddress,BOOL bListOnly);

	// block 이동
	bool MoveElementTree(const CBlockIndex& sourceBlockIndex, const CBlockIndex& targetBlockIndex);
	// element 찾기
	CElement* FindElement(IChartManager* pMainBlock, CExtractingDataOfModifyType* pExtractingData, CToolEnumData::ModifyType& modifyType);

	// element 그리기
	void DrawElement(CDC* pDC, CElement* pSelectedElement, IChartManager* pMainBlock);
	CElement *CopyElement(CElement* pSelectedElement);
	long MoveElement(CElement* pSelectedElement);

	CBlockIndex GetBlockIndexInElement(const CElement* pSelectedElement) const;

private:
	CDequePointerList<CColumnSet> m_ColumnSetList;

	// columnSet을 만들어 list에 추가
	void AddColumnSet(const int count);
	// CColumSet를 새로 만들고 거기에 element 추가
	void AddElementInNewMakedColumnSet(const CBlockIndex& blockIndex, CElement* pElement);
	// CColumnSet에 element 추가
	void AddElement(const int rowIndex, CColumnSet* pColumnSet, CElement* pElement);

	// Set을 추가할지의 여부 검사
	bool IsInsertElementTree(const CBlockIndex& blockIndex) const;
	// columnSet을 추가.
	bool InsertColumnSet(const int insertColumnIndex);
	// rowSet을 추가.
	bool InsertRowSet(const CBlockIndex& insertBlockIndex);

	// Set을 이동할 것인지의 유무
	bool IsMoveElementTree(const CBlockIndex& sourceBlockIndex, const CBlockIndex& targetBlockIndex) const;
	// columnSet간의 이동
	bool MoveColumnSet(const int sourceColumnIndex, const int targetColumnIndex);
	bool MoveColumnSet_InColumnSetList(const int sourceColumnIndex, const int targetColumnIndex);
	// 같은 columnSet의 rowSet간의 이동
	bool MoveRowSet(CColumnSet* pColumnSet, const int sourceRowIndex, const int targetRowIndex);
	int GetMax(const int x1, const int x2) const;


	// element 찾을지의 유무
	bool IsFindElement(IChartManager* pMainBlock, CExtractingDataOfModifyType* pExtractingData) const;
	bool IsFindElement(const CBlockIndex& blockIndex) const;
};


// element와 관련된 tree (order / tree)
class CElementTree
{
public:
	CElementTree( HWND p_hOcxWnd, CChartInfo::CHART_MODE p_eChartMode)
		{	m_hOcxWnd = p_hOcxWnd;	m_eChartMode = p_eChartMode;	}

public:
	void SetAutoPrice(const bool bIsAutoPrice);

	// element 색 변경하기.
	void ChangeAllElementsColor(const COLORREF& penColor, const COLORREF& textColor);

	//[A00000552]alzioyes_20110628 element 선두께 변경하기.
	void ChangeAllElementsThick(int nElementType, int nNewWidth);

	// 현재 있는 element count
	int GetElementCount() const;
	// block에 있는 element count
	void GetElementListOfBlock(const CBlockIndex& blockIndex, CDequePointerList<CElement>& elementList);
	// 저장할 모든 element data.
	CString GetSavedElementsData() const;

	// element 만들기.
	void BuildElementsFromString(CAnalysisData& analysisData,const CString& strElementsData);
	// element 추가
	void Add(const CBlockIndex& blockIndex, CElement* pElement);
	void AddColumnRowSet(const CBlockIndex& blockIndex);
	// block 추가시 elementTree을 추가하기.
	bool InsertElementTree(const CBlockBaseData::BLOCKTYPE blockType, const CBlockIndex& blockIndex);

	// CColumnSet 삭제
	void DeleteAll();
	// CRowSet 삭제
	void DeleteRowSet(const CBlockIndex& blockIndex);
	// element 순서적으로 지우기.
	void DeleteTail();
	// 특정 element만 삭제
	bool DeleteElement(const CToolEnumData::ElementType eElementTYPE);
	// element 삭제
	void DeleteElement(CElement* pElement);
	// element 삭제
	bool DeleteElementPos(const long lAddress,BOOL bListOnly = FALSE);

	// block 이동
	bool MoveElementTree(const CBlockIndex& sourceBlockIndex, const CBlockIndex& targetBlockIndex);
	// element 순서(order) 바꾸기
	bool ChangeElementOrder(CElement* pElement);
	// element 찾기
	CElement* FindElement(IChartManager* pMainBlock, CExtractingDataOfModifyType* pExtractingData, CToolEnumData::ModifyType& modifyType);

	// element 그리기
	void DrawElement(CDC* pDC, CElement* pSelectedElement, IChartManager* pMainBlock);
	CElement *CopyElement(CElement* pSelectedElement);	
	long MoveElement(CElement* pSelectedElement);	
	
	CBlockIndex GetBlockIndexInElement(const CElement* pSelectedElement) const;
	CElement* GetTail();
	CTree& GetTree();
	COrder& GetOrder();

private:
	COrder m_order;
	CTree m_tree;

// (2008/5/14 - Seung-Won, Bae) for Multi-Language of Special Date or Price Unit Text
protected:
	HWND	m_hOcxWnd;

// (2009/9/15 - Seung-Won, Bae) Do not support special time for WorldOn.
protected:
	CChartInfo::CHART_MODE	m_eChartMode;
};

#endif // !defined(AFX_ELEMENTTREE_H__A3A4BD06_2494_46E3_85DC_01B5249D09E4__INCLUDED_)
