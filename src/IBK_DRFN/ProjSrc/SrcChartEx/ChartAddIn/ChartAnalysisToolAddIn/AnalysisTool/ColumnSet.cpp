// ColumnSet.cpp: implementation of the CColumnSet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ColumnSet.h"

#include <Math.h>				// for HUGE_VAL

#include "RowSet.h"
#include "AnalysisData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////
// class CColumnSet

CColumnSet* CColumnSet::Make()
{
	return new CColumnSet;
}

void CColumnSet::Delete(CColumnSet* pColumnSet)
{
	delete pColumnSet;
}


// public =====================================================================
CColumnSet::~CColumnSet()
{
	DeleteAllRowSet();
}

// ----------------------------------------------------------------------------
void CColumnSet::SetAutoPrice(const bool bIsAutoPrice)
{
	for(int nIndex = 0; nIndex < m_RowSetList.GetCount(); nIndex++){
		CRowSet* pRowSet = m_RowSetList.GetAt(nIndex);
		assert(pRowSet != NULL);
		pRowSet->SetAutoPrice(bIsAutoPrice);
	}
}

// ----------------------------------------------------------------------------
// element 색 변경하기.
void CColumnSet::ChangeAllElementsColor(const COLORREF& penColor, const COLORREF& textColor)
{
	for(int rowIndex = 0; rowIndex < m_RowSetList.GetCount(); rowIndex++)
	{
		CRowSet* pRowSet = m_RowSetList.GetAt(rowIndex);
		assert(pRowSet != NULL);
		pRowSet->ChangeAllElementsColor(penColor, textColor);
	}
}

//[A00000552]alzioyes_20110628 element 선두께 변경하기.
void CColumnSet::ChangeAllElementsThick(int nElementType, int nNewWidth)
{
	for(int rowIndex = 0; rowIndex < m_RowSetList.GetCount(); rowIndex++)
	{
		CRowSet* pRowSet = m_RowSetList.GetAt(rowIndex);
		assert(pRowSet != NULL);
		pRowSet->ChangeAllElementsThick(nElementType, nNewWidth);
	}
}
// ----------------------------------------------------------------------------
// 저장할 모든 element data.
CString CColumnSet::GetSavedElementsData(const COrder& order) const
{
	CString savedElementsData("<Column>\r\n");
	for(int nIndex = 0; nIndex < m_RowSetList.GetCount(); nIndex++){
		CRowSet* pRowSet = m_RowSetList.GetAt(nIndex);
		assert(pRowSet != NULL);
		savedElementsData += pRowSet->GetSavedElementsData(order);
	}

	return savedElementsData + "<\\Column>\r\n";
}

// 해당 elements 가져오기.
void CColumnSet::GetElementList(const int rowIndex, CDequePointerList<CElement>& elementList)
{
	if(rowIndex < 0 || rowIndex >= m_RowSetList.GetCount())
		return;

	CRowSet* pRowSet = m_RowSetList.GetAt(rowIndex);
	assert(pRowSet != NULL);
	pRowSet->GetElementList(elementList);
}

// ----------------------------------------------------------------------------
// list에 element 추가.
void CColumnSet::AddElement(const int rowIndex, CElement* pElement)
{
	assert(rowIndex >= 0);

	if(rowIndex < m_RowSetList.GetCount()){
		AddElement(m_RowSetList.GetAt(rowIndex), pElement);
		return;
	}

	AddElementInNewMakedRowSet(rowIndex, pElement);
}

// ----------------------------------------------------------------------------
// block 추가시 elementTree을 추가하기.
bool CColumnSet::AddRowSet()
{	
	CRowSet* pRowSet = CRowSet::Make();
	assert(pRowSet != NULL);
	m_RowSetList.AddTail(pRowSet);
	return true;
}

bool CColumnSet::InsertRowSet(const int rowIndex)
{
	// 추가된 block이 columnSet이 있는 list의 범위를 벗어날 경우.
	if(m_RowSetList.GetCount() <= 0 || m_RowSetList.GetCount() <= rowIndex)
		return false;

	CRowSet* pRowSet = CRowSet::Make();
	assert(pRowSet != NULL);
	m_RowSetList.InsertItem(rowIndex, pRowSet);
	return true;
}

// ----------------------------------------------------------------------------
// 모든 CRowSet 삭제
void CColumnSet::DeleteAllRowSet()
{
	while(m_RowSetList.GetCount() > 0){
		DeleteRowSet(0);
	}

	m_RowSetList.RemoveAll();
}

// CRowSet 삭제.
void CColumnSet::DeleteRowSet(const int rowNumber)
{
	if(rowNumber < 0 || rowNumber >= m_RowSetList.GetCount())
		return;
	
	CRowSet* pRowSet = m_RowSetList.GetAt(rowNumber);
	DeleteRowSet(pRowSet);
}

// 특정 type인 element만 삭제
// 삭제된 경우에는 해당 element의 point를 list에 담는다.
// -> 객체를 생성한 순서를 저장되는 class에서도 remove 해야 하기때문.
bool CColumnSet::DeleteElement(const CToolEnumData::ElementType eElementTYPE, CDequePointerList<CElement>& deletedElementList)
{
	bool bIsDeleted = false;

	for(int nIndex = 0; nIndex < m_RowSetList.GetCount(); nIndex++)
	{
		CRowSet* pRowSet = m_RowSetList.GetAt(nIndex);
		assert(pRowSet != NULL);
		if(pRowSet->Delete(eElementTYPE, deletedElementList))
			bIsDeleted = true;
	}

	return bIsDeleted;
}

// element 삭제
bool CColumnSet::DeleteElement(CElement* pElement)
{
	for(int nIndex = 0; nIndex < m_RowSetList.GetCount(); nIndex++)
	{
		CRowSet* pRowSet = m_RowSetList.GetAt(nIndex);
		assert(pRowSet != NULL);
		if(pRowSet->Delete(pElement))
			return true;
	}
	return false;
}

bool CColumnSet::DeleteElement(const long lElementAddres,BOOL bListOnly)
{
	for(int nIndex = 0; nIndex < m_RowSetList.GetCount(); nIndex++)
	{
		CRowSet* pRowSet = m_RowSetList.GetAt(nIndex);
		assert(pRowSet != NULL);
		if(pRowSet->Delete(lElementAddres,bListOnly))
			return true;
	}
	return false;
}

// ----------------------------------------------------------------------------
// rowSet간의 이동
bool CColumnSet::MoveRowSet(const int sourceRowIndex, const int targetRowIndex)
{
	if(sourceRowIndex >= m_RowSetList.GetCount() && targetRowIndex >= m_RowSetList.GetCount())
		return false;

	// sourceRowIndex가 m_RowSetList의 내용이 아닐 경우
	// -> 새로운 RowSet을 insert한다.
	if(sourceRowIndex > (m_RowSetList.GetCount() -1)){
		m_RowSetList.InsertItem(targetRowIndex, CRowSet::Make());
		return true;
	}

	// list에 존재하는 RowSet을 Move하는 경우.
	return MoveRowSet_InRowSetList(sourceRowIndex, targetRowIndex);
}

// ----------------------------------------------------------------------------
// element 찾기
CElement* CColumnSet::FindElement( IChartManager* pMainBlock, CExtractingDataOfModifyType* pExtractingData, 
								  const int columnIndex, CToolEnumData::ModifyType& modifyType)
{
	for(int rowIndex = 0; rowIndex < m_RowSetList.GetCount(); rowIndex++){
		CRowSet* pRowSet = m_RowSetList.GetAt(rowIndex);
		assert(pRowSet != NULL);

		pExtractingData->SetRowSetIndex(rowIndex);
		pExtractingData->GetCoordinateCalculatorData().GetCoordinateCalculatorData( CBlockIndex(rowIndex, columnIndex));

		CElement* pElement = pRowSet->FindElement(pExtractingData, modifyType);
		if(pElement != NULL)
			return pElement;
	}

	return NULL;
}

// ----------------------------------------------------------------------------
// element 그리기
void CColumnSet::DrawElement(CDC* pDC, CElement* pSelectedElement, IChartManager* pMainBlock,const int columnIndex)
{
	CDrawingRegions drawingRegions;
	drawingRegions.SetDrawingRegions(pMainBlock, columnIndex);

	for(int rowIndex = 0; rowIndex < m_RowSetList.GetCount(); rowIndex++){
		CRowSet* pRowSet = m_RowSetList.GetAt(rowIndex);
		assert(pRowSet != NULL);
		drawingRegions.GetBlockRegionList().SetCurrentNumber(rowIndex);

		if(pRowSet->GetElementCount() <= 0)
			continue;
		CCoordinateCalculatorData calData( pMainBlock);
		calData.GetCoordinateCalculatorData( CBlockIndex( rowIndex, columnIndex));

		// (2007/5/10 - Seung-Won, Bae) Check No Y Scale Min/Max.
		if( -HUGE_VAL == calData.GetDisplayDataMax_ScaleType() || -HUGE_VAL == calData.GetDisplayDataMin_ScaleType()) continue;

		pRowSet->DrawElement(pDC,pSelectedElement,drawingRegions,calData);
	}
}

CElement *CColumnSet::CopyElement(CElement* pSelectedElement)
{
	CElement *lAddress = NULL;
	CElement *lAddressTemp = NULL;
	for(int rowIndex = 0; rowIndex < m_RowSetList.GetCount(); rowIndex++){
		CRowSet* pRowSet = m_RowSetList.GetAt(rowIndex);
		assert(pRowSet != NULL);
		
		if(pRowSet->GetElementCount() <= 0)
			continue;
		
		lAddressTemp = pRowSet->CopyElement(pSelectedElement);
		if(lAddressTemp != 0)
			lAddress = lAddressTemp;
	}

	return lAddress;
}

long CColumnSet::MoveElement(CElement* pSelectedElement)
{
	long lAddress = 0;
	for(int rowIndex = 0; rowIndex < m_RowSetList.GetCount(); rowIndex++){
		CRowSet* pRowSet = m_RowSetList.GetAt(rowIndex);
		assert(pRowSet != NULL);
		
		lAddress = pRowSet->MoveElement(pSelectedElement);
		if(lAddress == 0)
			break;
	}

	return lAddress;
}

CBlockIndex CColumnSet::GetBlockIndexInElement(const CElement* pSelectedElement, const int columnIndex) const
{
	if(pSelectedElement == NULL)
		return CBlockIndex();

	CBlockIndex blockIndex;
	for(int rowIndex = 0; rowIndex < m_RowSetList.GetCount(); ++rowIndex)
	{
		CRowSet* pRowSet = m_RowSetList.GetAt(rowIndex);
		assert(pRowSet != NULL);
		if(pRowSet->IsInSameElement(pSelectedElement))
			return CBlockIndex( rowIndex, columnIndex);
	}
	return CBlockIndex();
}

// private ====================================================================
// RowSet을 입력되 수만큼 추가한다.
void CColumnSet::AddRowSet(const int count)
{
	for(int i = 0; i < count; i++){
		CRowSet* pRowSet = CRowSet::Make();
		m_RowSetList.AddTail(pRowSet);
	}
}

// CRowSet 새로 만들고 거기에 element 추가
void CColumnSet::AddElementInNewMakedRowSet(const int rowIndex, CElement* pElement)
{
	AddRowSet(rowIndex - m_RowSetList.GetCount() +1);
	CRowSet* pRowSet = m_RowSetList.GetTail();
	assert(pRowSet != NULL);
	AddElement(pRowSet, pElement);
}

// CRowSet에 element 추가
void CColumnSet::AddElement(CRowSet* pRowSet, CElement* pElement)
{
	assert(pRowSet != NULL);
	pRowSet->AddElement(pElement);
}

// ----------------------------------------------------------------------------
// CRowSet 삭제.
void CColumnSet::DeleteRowSet(CRowSet* pRowSet)
{
	assert(pRowSet != NULL);
	CRowSet::Delete(pRowSet);
	m_RowSetList.RemoveAt(pRowSet);
}

// ----------------------------------------------------------------------------
// list에 존재하는 RowSet을 Move하는 경우.
bool CColumnSet::MoveRowSet_InRowSetList(const int sourceRowIndex, const int targetRowIndex)
{
	if(sourceRowIndex >= m_RowSetList.GetCount())
		return false;

	int nMaxIndex = GetMax(sourceRowIndex, targetRowIndex);
	// 새로 rowSet 만들기
	if(nMaxIndex >= m_RowSetList.GetCount()){
		AddRowSet(nMaxIndex - m_RowSetList.GetCount() +1);
	}
	// 이동
	int count = m_RowSetList.GetCount();
	CRowSet* s = m_RowSetList.GetAt(sourceRowIndex);
	CRowSet* t = m_RowSetList.GetAt(targetRowIndex);

	m_RowSetList.MoveItem(sourceRowIndex, targetRowIndex);
	s = m_RowSetList.GetAt(sourceRowIndex);
	t = m_RowSetList.GetAt(targetRowIndex);
	return true;
}

// ----------------------------------------------------------------------------
int CColumnSet::GetMax(const int x1, const int x2) const
{
	return (x1 > x2) ? x1: x2;
}
