// RowSet.h: interface for the CRowSet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ROWSET_H__B2CFF04A_0FC8_405C_A314_C4C6F208D148__INCLUDED_)
#define AFX_ROWSET_H__B2CFF04A_0FC8_405C_A314_C4C6F208D148__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Include_Analysis/ToolBaseData.h"
#include "DequeList.h"

class CElement;
class CDrawingRegions;
class CCoordinateCalculatorData;
class CExtractingDataOfModifyType;
class COrder;

class CRowSet  
{
public:
	static CRowSet* Make();
	static void Delete(CRowSet* pRowSet);

public:
	virtual ~CRowSet();

	void SetAutoPrice(const bool bIsAutoPrice);

	// element 색 변경하기.
	void ChangeAllElementsColor(const COLORREF& penColor, const COLORREF& textColor);

	//[A00000552]alzioyes_20110628 element 선두께 변경하기.
	void ChangeAllElementsThick(int nElementType, int nNewWidth);

	// element count
	int GetElementCount() const;
	// 저장할 모든 element data.
	CString GetSavedElementsData(const COrder& order) const;
	// 해당 elements 가져오기.
	void GetElementList(CDequePointerList<CElement>& elementList);
	void GetElementList(const CToolEnumData::ElementType eElementTYPE, CDequePointerList<CElement>& elementList);


	// list에 element 추가.
	void AddElement(CElement* pElement);

	// element 삭제.
	void DeleteAll();
	bool Delete(const long lElementAddres,BOOL bListOnly);
	bool Delete(const CToolEnumData::ElementType eElementTYPE, CDequePointerList<CElement>& deletedElementList);
	bool Delete(CElement* pElement, const bool bCheckSameElement = true);

	// element 찾기
	CElement* FindElement(CExtractingDataOfModifyType* pExtractingData, CToolEnumData::ModifyType& modifyType);

	// element Copy
	CElement *CopyElement(CElement* pSelectedElement);
	long MoveElement(CElement* pSelectedElement);
	// element 그리기.
	void DrawElement(CDC* pDC, CElement* pSelectedElement,
					const CDrawingRegions& drawingRegions, const CCoordinateCalculatorData& calData);
	// list에 같은 element가 있으면 = true
	bool IsInSameElement(const CElement* pElement) const;

private:
	CDequePointerList<CElement> m_ElementList;

	// element를 그릴지 여부. (그리기 = true)
	bool WillDrawElement(const CDrawingRegions& drawingRegions) const;
};

#endif // !defined(AFX_ROWSET_H__B2CFF04A_0FC8_405C_A314_C4C6F208D148__INCLUDED_)
