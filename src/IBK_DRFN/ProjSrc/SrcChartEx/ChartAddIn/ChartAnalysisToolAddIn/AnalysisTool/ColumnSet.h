// ColumnSet.h: interface for the CColumnSet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COLUMNSET_H__366BC96A_EF4D_4140_A632_CA142448DC0F__INCLUDED_)
#define AFX_COLUMNSET_H__366BC96A_EF4D_4140_A632_CA142448DC0F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Include_Analysis/Include_AddIn.h"				// for Ixxxxxx.h
#include "../Include_Analysis/ToolBaseData.h"
#include "DequeList.h"

class CRowSet;
class CElement;
class CBlockIndex;
class CExtractingDataOfModifyType;
class COrder;

class CColumnSet  
{
public:
	static CColumnSet* Make();
	static void Delete(CColumnSet* pColumnSet);

public:
	virtual ~CColumnSet();

	void SetAutoPrice(const bool bIsAutoPrice);

	// element 색 변경하기.
	void ChangeAllElementsColor(const COLORREF& penColor, const COLORREF& textColor);

	//[A00000552]alzioyes_20110628 element 선두께 변경하기.
	void ChangeAllElementsThick(int nElementType, int nNewWidth);

	// 저장할 모든 element data.
	CString GetSavedElementsData(const COrder& order) const;
	// 해당 elements 가져오기.
	void GetElementList(const int sourceRowIndex, CDequePointerList<CElement>& elementList);

	// list에 element 추가.
	bool AddRowSet();
	void AddElement(const int rowIndex, CElement* pElement);
	// block 추가시 elementTree을 추가하기.
	bool InsertRowSet(const int rowIndex);

	// CRowSet 삭제.
	void DeleteAllRowSet();
	void DeleteRowSet(const int rowNumber);
	// 특정 tyle의 element만 삭제
	bool DeleteElement(const CToolEnumData::ElementType eElementTYPE, CDequePointerList<CElement>& deletedElementList);
	// element 삭제
	bool DeleteElement(CElement* pElement);
	// element 삭제
	bool DeleteElement(const long lElementAddres,BOOL bListOnly);

	// rowSet간의 이동
	bool MoveRowSet(const int rowIndex, const int targetRowIndex);
	// element 찾기
	CElement* FindElement(IChartManager* pMainBlock, CExtractingDataOfModifyType* pExtractingData, const int columnIndex, CToolEnumData::ModifyType& modifyType);

	// element 그리기
	void DrawElement(CDC* pDC, CElement* pSelectedElement, IChartManager* pMainBlock,const int columnIndex);
	CElement *CopyElement(CElement* pSelectedElement);
	long MoveElement(CElement* pSelectedElement);

	CBlockIndex GetBlockIndexInElement(const CElement* pSelectedElement, const int columnIndex) const;
	
private:
	CDequePointerList<CRowSet> m_RowSetList;

	// RowSet을 입력되 수만큰 추가한다.
	void AddRowSet(const int count);
	// CRowSet 새로 만들고 거기에 element 추가
	void AddElementInNewMakedRowSet(const int rowIndex, CElement* pElement);
	// CRowSet에 element 추가
	void AddElement(CRowSet* pRowSet, CElement* pElement);
	// CRowSet 삭제.
	void DeleteRowSet(CRowSet* pRowSet);

	// list에 존재하는 RowSet을 Move는 경우.
	bool MoveRowSet_InRowSetList(const int sourceRowIndex, const int targetRowIndex);
	
	int GetMax(const int x1, const int x2) const;
};

#endif // !defined(AFX_COLUMNSET_H__366BC96A_EF4D_4140_A632_CA142448DC0F__INCLUDED_)
