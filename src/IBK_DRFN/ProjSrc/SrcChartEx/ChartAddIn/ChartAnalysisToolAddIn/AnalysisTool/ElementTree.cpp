// ElementTree.cpp: implementation of the CElementTree class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ElementTree.h"

#include "Element.h"
#include "AnalysisData.h"
#include "ColumnSet.h"
#include "ElementTreeBuilder.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////
// class COrder

int COrder::GetCount() const
{
	return m_ElementOrderList.GetCount();
}

// 해당 element의 순서 가져오기.
int COrder::GetIndex(const CElement* pElement) const
{
	return m_ElementOrderList.GetIndex(pElement);
}

// ----------------------------------------------------------------------------
// element 추가
void COrder::AddTail(CElement* pElement)
{
	assert(pElement != NULL);
	m_ElementOrderList.AddTail(pElement);
}

// ----------------------------------------------------------------------------
// list에 모두 지우기
void COrder::RemoveAll()
{
	m_ElementOrderList.RemoveAll();
}

// list에 지우기
void COrder::Remove(const CDequePointerList<CElement>& elementList)
{
	for(int nIndex = 0; nIndex < elementList.GetCount(); nIndex++){
		CElement* pElement = elementList.GetAt(nIndex);
		assert(pElement != NULL);
		Remove(pElement);
	}
}

// 마지막 지우기.
void COrder::RemoveTail()
{
	if(m_ElementOrderList.GetCount() <= 0)
		return;

	m_ElementOrderList.RemoveTail();
}

// list에 지우기
void COrder::Remove(CElement* pElement)
{
	assert(pElement != NULL);
	m_ElementOrderList.RemoveAt(pElement);
}

// ----------------------------------------------------------------------------
// element 순서 바꾸기
bool COrder::ChangeOrder(CElement* pElement)
{
	assert(pElement != NULL);
	for(int nIndex = 0; nIndex < m_ElementOrderList.GetCount(); nIndex++){
		if(!IsChangeOrder(pElement, nIndex))
			continue;
		
		m_ElementOrderList.RemoveAt(nIndex);
		m_ElementOrderList.AddTail(pElement);
		return true;
	}
	return false;
}

// ----------------------------------------------------------------------------
// list의 마지막 data 가져오기.
CElement* COrder::GetTail()
{
	return m_ElementOrderList.GetTail();
}

CElement* COrder::GetAt(int nIndex)
{
	return m_ElementOrderList.GetAt(nIndex);
}

// private ====================================================================
// element 순서를 바꿀지의 여부
bool COrder::IsChangeOrder(CElement* pElement, const int nIndex) const
{
	return (pElement == m_ElementOrderList.GetAt(nIndex) && nIndex < m_ElementOrderList.GetCount());
}


///////////////////////////////////////////////////////////////////////////////
// class CTree

CTree::~CTree()
{
	DeleteAll();
}

// ----------------------------------------------------------------------------
void CTree::SetAutoPrice(const bool bIsAutoPrice)
{
	for(int nIndex = 0; nIndex < m_ColumnSetList.GetCount(); nIndex++){
		CColumnSet* pColumnSet = m_ColumnSetList.GetAt(nIndex);
		assert(pColumnSet != NULL);
		pColumnSet->SetAutoPrice(bIsAutoPrice);
	}
}

// ----------------------------------------------------------------------------
// element 색 변경하기.
void CTree::ChangeAllElementsColor(const COLORREF& penColor, const COLORREF& textColor)
{
	for(int nIndex = 0; nIndex < m_ColumnSetList.GetCount(); nIndex++)
	{
		CColumnSet* pColumnSet = m_ColumnSetList.GetAt(nIndex);
		assert(pColumnSet != NULL);
		pColumnSet->ChangeAllElementsColor(penColor, textColor);
	}
}

//[A00000552]alzioyes_20110628 element 선두께 변경하기.
void CTree::ChangeAllElementsThick(int nElementType, int nNewWidth)
{
	for(int nIndex = 0; nIndex < m_ColumnSetList.GetCount(); nIndex++)
	{
		CColumnSet* pColumnSet = m_ColumnSetList.GetAt(nIndex);
		assert(pColumnSet != NULL);
		pColumnSet->ChangeAllElementsThick(nElementType, nNewWidth);
	}
}
// ----------------------------------------------------------------------------
// 저장할 모든 element data.
CString CTree::GetSavedElementsData(const COrder& order) const
{
	CString savedElementsData;
	for(int nIndex = 0; nIndex < m_ColumnSetList.GetCount(); nIndex++){
		CColumnSet* pColumnSet = m_ColumnSetList.GetAt(nIndex);
		assert(pColumnSet != NULL);
		CString savedColumnData = pColumnSet->GetSavedElementsData(order);
		savedElementsData += pColumnSet->GetSavedElementsData(order);
	}

	return savedElementsData;
}

// 해당 elements 가져오기.
void CTree::GetElementList(const CBlockIndex& blockIndex, CDequePointerList<CElement>& elementList)
{
	elementList.RemoveAll();

	if(blockIndex.GetColumn() < 0 || blockIndex.GetColumn() >= m_ColumnSetList.GetCount())
		return;

	CColumnSet* pColumnSet = m_ColumnSetList.GetAt(blockIndex.GetColumn());
	assert(pColumnSet != NULL);
	pColumnSet->GetElementList(blockIndex.GetRow(), elementList);
}

// ----------------------------------------------------------------------------
// list에 element 추가.
void CTree::Add(const CBlockIndex& blockIndex, CElement* pElement)
{
	if(blockIndex.GetColumn() < m_ColumnSetList.GetCount()){
		AddElement(blockIndex.GetRow(), m_ColumnSetList.GetAt(blockIndex.GetColumn()), pElement);
		return;
	}

	AddElementInNewMakedColumnSet(blockIndex, pElement);
}

void CTree::AddColumnRowSet(const CBlockIndex& blockIndex)
{
	AddColumnSet(blockIndex.GetColumn() - m_ColumnSetList.GetCount() +1);
	CColumnSet* pColumnSet = m_ColumnSetList.GetTail();
	assert(pColumnSet != NULL);
	pColumnSet->AddRowSet();	
}
// ----------------------------------------------------------------------------
// block 추가시 elementTree을 추가하기.
bool CTree::InsertElementTree(const CBlockBaseData::BLOCKTYPE blockType, const CBlockIndex& blockIndex)
{
	if(!IsInsertElementTree(blockIndex))
		return false;

	// column 형의 block 구조일 경우엔 columnSet을 만든다.
	// row 형의 block 구조일 경우엔 기존의 columnSet에 rowSet을 추가한다.
	if(blockType == CBlockBaseData::COLUMN)
		return InsertColumnSet(blockIndex.GetColumn());
	else if(blockType == CBlockBaseData::ROW)
		return InsertRowSet(blockIndex);

	return false;
}

// ----------------------------------------------------------------------------
// 모든 CColumnSet 삭제
void CTree::DeleteAll()
{
	for(int nIndex = 0; nIndex < m_ColumnSetList.GetCount(); nIndex++){
		CColumnSet::Delete(m_ColumnSetList.GetAt(nIndex));
	}

	m_ColumnSetList.RemoveAll();
}

// CRowSet 삭제
void CTree::DeleteRowSet(const CBlockIndex& blockIndex)
{
	if(blockIndex.GetColumn() < 0 || blockIndex.GetColumn() >= m_ColumnSetList.GetCount())
		return;

	CColumnSet* pColumnSet = m_ColumnSetList.GetAt(blockIndex.GetColumn());
	assert(pColumnSet != NULL);
	pColumnSet->DeleteRowSet(blockIndex.GetRow());
}

// 특정 tyle의 element만 삭제
// 삭제된 경우에는 해당 element의 point를 list에 담는다.
// -> 객체를 생성한 순서를 저장되는 class에서도 remove 해야 하기때문.
bool CTree::DeleteElement(const CToolEnumData::ElementType eElementTYPE, CDequePointerList<CElement>& deletedElementList)
{
	deletedElementList.RemoveAll();

	for(int nIndex = 0; nIndex < m_ColumnSetList.GetCount(); nIndex++)
	{
		CColumnSet* pColumnSet = m_ColumnSetList.GetAt(nIndex);
		assert(pColumnSet != NULL);
		pColumnSet->DeleteElement(eElementTYPE, deletedElementList);
	}

	return (deletedElementList.GetCount() > 0);
}

// element 삭제
void CTree::DeleteElement(CElement* pElement)
{
	for(int nIndex = 0; nIndex < m_ColumnSetList.GetCount(); nIndex++)
	{
		CColumnSet* pColumnSet = m_ColumnSetList.GetAt(nIndex);
		assert(pColumnSet != NULL);
		if(pColumnSet->DeleteElement(pElement))
			return;
	}
}

// element 삭제
bool CTree::DeleteElement(const long lElementAddress,BOOL bListOnly)
{
	bool bFind = false;
	assert(lElementAddress != 0);
	for(int nIndex = 0; nIndex < m_ColumnSetList.GetCount(); nIndex++)
	{
		CColumnSet* pColumnSet = m_ColumnSetList.GetAt(nIndex);		
		bFind = pColumnSet->DeleteElement(lElementAddress,bListOnly);
		if(bFind == true)
			return true;
	}

	return false;
}
// ----------------------------------------------------------------------------
// block 이동
bool CTree::MoveElementTree(const CBlockIndex& sourceBlockIndex, const CBlockIndex& targetBlockIndex)
{
	if(!IsMoveElementTree(sourceBlockIndex, targetBlockIndex))
		return false;

	// column 간의 이동
	if(sourceBlockIndex.GetColumn() != targetBlockIndex.GetColumn())
		return MoveColumnSet(sourceBlockIndex.GetColumn(), targetBlockIndex.GetColumn());

	// 같은 column 안에서 row간의 이동
	if(sourceBlockIndex.GetColumn() < m_ColumnSetList.GetCount())
		return MoveRowSet(m_ColumnSetList.GetAt(sourceBlockIndex.GetColumn()), sourceBlockIndex.GetRow(), targetBlockIndex.GetRow());

	return false;
}

// ----------------------------------------------------------------------------
// element 찾기
CElement* CTree::FindElement(IChartManager* pMainBlock, CExtractingDataOfModifyType* pExtractingData, CToolEnumData::ModifyType& modifyType)
{
	if( !pMainBlock || !IsFindElement(pMainBlock, pExtractingData)) return NULL;

	int nRow, nColumn;
	if(!pMainBlock->GetBlockIndex( pExtractingData->GetMousePoint(), nRow, nColumn)) return NULL;
	CBlockIndex blockIndexInPoint( nRow, nColumn);
	if(!IsFindElement(blockIndexInPoint))
		return NULL;

	CColumnSet* pColumnSet = m_ColumnSetList.GetAt(blockIndexInPoint.GetColumn());
	assert(pColumnSet != NULL);
	return pColumnSet->FindElement(pMainBlock, pExtractingData, blockIndexInPoint.GetColumn(), modifyType);
}

// ----------------------------------------------------------------------------
// element 그리기.
void CTree::DrawElement(CDC* pDC, CElement* pSelectedElement, IChartManager* pMainBlock)
{
	if(pMainBlock == NULL)
		return;

	for(int nIndex = 0; nIndex < m_ColumnSetList.GetCount(); nIndex++){
		CColumnSet* pColumnSet = m_ColumnSetList.GetAt(nIndex);
		assert(pColumnSet != NULL);
		pColumnSet->DrawElement(pDC, pSelectedElement, pMainBlock, nIndex);
	}
}

CElement *CTree::CopyElement(CElement* pSelectedElement)
{
	CElement *lAddress = NULL;
	CElement*lAddressTemp = NULL;
	for(int nIndex = 0; nIndex < m_ColumnSetList.GetCount(); nIndex++){
		CColumnSet* pColumnSet = m_ColumnSetList.GetAt(nIndex);
		assert(pColumnSet != NULL);
		lAddressTemp = pColumnSet->CopyElement(pSelectedElement);
		if(lAddressTemp != 0)
			lAddress = lAddressTemp;
	}
	return lAddress;
}

long CTree::MoveElement(CElement* pSelectedElement)
{
	long lAddress = 0;
	for(int nIndex = 0; nIndex < m_ColumnSetList.GetCount(); nIndex++){
		CColumnSet* pColumnSet = m_ColumnSetList.GetAt(nIndex);
		assert(pColumnSet != NULL);
		lAddress = pColumnSet->MoveElement(pSelectedElement);
		if(lAddress == 0)
			break;
	}
	return lAddress;
}

CBlockIndex CTree::GetBlockIndexInElement(const CElement* pSelectedElement) const
{
	if(pSelectedElement == NULL)
		return CBlockIndex();

	CBlockIndex blockIndex;
	for(int columnIndex = 0; columnIndex < m_ColumnSetList.GetCount(); ++columnIndex)
	{
		CColumnSet* pColumnSet = m_ColumnSetList.GetAt(columnIndex);
		assert(pColumnSet != NULL);
		blockIndex = pColumnSet->GetBlockIndexInElement(pSelectedElement, columnIndex);
		if(blockIndex.IsAllValueBiggerThanZero())
			break;
	}
	return blockIndex;
}

// private ====================================================================
// columnSet을 만들어 list에 추가
void CTree::AddColumnSet(const int count)
{
	for(int i = 0; i < count; i++){
		CColumnSet* pColumnSet = CColumnSet::Make();
		m_ColumnSetList.AddTail(pColumnSet);
	}
}

// CColumSet를 새로 만들고 거기에 element 추가
void CTree::AddElementInNewMakedColumnSet(const CBlockIndex& blockIndex, CElement* pElement)
{
	AddColumnSet(blockIndex.GetColumn() - m_ColumnSetList.GetCount() +1);
	CColumnSet* pColumnSet = m_ColumnSetList.GetTail();
	assert(pColumnSet != NULL);
	AddElement(blockIndex.GetRow(), pColumnSet, pElement);
}

// CColumnSet에 element 추가
void CTree::AddElement(const int rowIndex, CColumnSet* pColumnSet, CElement* pElement)
{
	assert(pColumnSet != NULL);
	pColumnSet->AddElement(rowIndex, pElement);
}

// ----------------------------------------------------------------------------
// Set을 추가할지의 여부 검사
bool CTree::IsInsertElementTree(const CBlockIndex& blockIndex) const
{
	// 추가된 block이 없을 경우.
	if(!blockIndex.IsAllValueBiggerThanZero())
		return false;

	// 추가된 block이 columnSet이 있는 list의 범위를 벗어날 경우.
	return (m_ColumnSetList.GetCount() > 0 && blockIndex.GetColumn() < m_ColumnSetList.GetCount());
}

// columnSet을 추가.
bool CTree::InsertColumnSet(const int insertColumnIndex)
{
	CColumnSet* pColumnSet = CColumnSet::Make();
	assert(pColumnSet != NULL);
	m_ColumnSetList.InsertItem(insertColumnIndex, pColumnSet);
	return true;
}

// rowSet을 추가.
bool CTree::InsertRowSet(const CBlockIndex& insertBlockIndex)
{
	CColumnSet* pColumnSet = m_ColumnSetList.GetAt(insertBlockIndex.GetColumn());
	assert(pColumnSet != NULL);
	return pColumnSet->InsertRowSet(insertBlockIndex.GetRow());
}

// ----------------------------------------------------------------------------
// Set을 이동할 것인지의 유무
bool CTree::IsMoveElementTree(const CBlockIndex& sourceBlockIndex, const CBlockIndex& targetBlockIndex) const
{
	if(!sourceBlockIndex.IsAllValueBiggerThanZero() || !targetBlockIndex.IsAllValueBiggerThanZero())
		return false;
	if(sourceBlockIndex.GetColumn() >= m_ColumnSetList.GetCount() && 
		sourceBlockIndex.GetColumn() >= m_ColumnSetList.GetCount())
		return false;

	return true;
}

// columnSet간의 이동
bool CTree::MoveColumnSet(const int sourceColumnIndex, const int targetColumnIndex)
{
	// sourceColumnIndex가 m_ColumnSetList 내용이 아닐 경우.
	// -> 새로운 ColumnSet을 insert한다.
	if(sourceColumnIndex > (m_ColumnSetList.GetCount() -1)){
		m_ColumnSetList.InsertItem(targetColumnIndex, CColumnSet::Make());
		return true;
	}
	
	// list에 존재하는 ColumnSet을 Move하는 경우.
	return MoveColumnSet_InColumnSetList(sourceColumnIndex, targetColumnIndex);
}

// list에 존재하는 ColumnSet을 Move하는 경우.
bool CTree::MoveColumnSet_InColumnSetList(const int sourceColumnIndex, const int targetColumnIndex)
{
	if(sourceColumnIndex >= m_ColumnSetList.GetCount())
		return false;

	int nMaxIndex = GetMax(sourceColumnIndex, targetColumnIndex);

	// 새로 ColumnSet 만들기
	if(nMaxIndex > m_ColumnSetList.GetCount())
		AddColumnSet(nMaxIndex - m_ColumnSetList.GetCount());

	// 이동
	m_ColumnSetList.MoveItem(sourceColumnIndex, targetColumnIndex);
	return true;
}

// 같은 columnSet의 rowSet간의 이동
bool CTree::MoveRowSet(CColumnSet* pColumnSet, const int sourceRowIndex, const int targetRowIndex)
{
	assert(pColumnSet != NULL);
	if(sourceRowIndex == targetRowIndex)
		return false;

	return pColumnSet->MoveRowSet(sourceRowIndex, targetRowIndex);
}

int CTree::GetMax(const int x1, const int x2) const
{
	return (x1 > x2) ? x1: x2;
}

// ----------------------------------------------------------------------------
// element 찾을지의 유무
bool CTree::IsFindElement(IChartManager* pMainBlock, CExtractingDataOfModifyType* pExtractingData) const
{
	return (pMainBlock == NULL || pExtractingData == NULL || pExtractingData->IsFindElement());
}

bool CTree::IsFindElement(const CBlockIndex& blockIndex) const
{
	return (blockIndex.IsAllValueBiggerThanZero() && blockIndex.GetColumn() < m_ColumnSetList.GetCount());
}


///////////////////////////////////////////////////////////////////////////////
// class CElementTree

void CElementTree::SetAutoPrice(const bool bIsAutoPrice)
{
	m_tree.SetAutoPrice(bIsAutoPrice);
}

// element 찾기
void CElementTree::ChangeAllElementsColor(const COLORREF& penColor, const COLORREF& textColor)
{
	m_tree.ChangeAllElementsColor(penColor, textColor);
}

//[A00000552]alzioyes_20110628 element 선두께 변경하기.
void CElementTree::ChangeAllElementsThick(int nElementType, int nNewWidth)
{
	m_tree.ChangeAllElementsThick(nElementType, nNewWidth);
}

// ----------------------------------------------------------------------------
// 현재 있는 element count
int CElementTree::GetElementCount() const
{
	return m_order.GetCount();
}

void CElementTree::GetElementListOfBlock(const CBlockIndex& blockIndex, CDequePointerList<CElement>& elementList)
{
	elementList.RemoveAll();
	m_tree.GetElementList(blockIndex, elementList);
}

// 저장할 모든 element data.
CString CElementTree::GetSavedElementsData() const
{
	return m_tree.GetSavedElementsData(m_order);
}

// ----------------------------------------------------------------------------
// element 만들기.
void CElementTree::BuildElementsFromString(CAnalysisData& analysisData,const CString& strElementsData)
{
	DeleteAll();

	if(strElementsData.IsEmpty())
		return;

	CElementTreeBuilder builder( m_hOcxWnd, m_eChartMode);
	builder.BuildElementsFromString(analysisData,strElementsData, m_tree, m_order);
}

// list에 element 추가.
void CElementTree::Add(const CBlockIndex& blockIndex, CElement* pElement)
{
	assert(blockIndex.IsAllValueBiggerThanZero() && pElement != NULL);

	m_order.AddTail(pElement);
	m_tree.Add(blockIndex, pElement);
}

void CElementTree::AddColumnRowSet(const CBlockIndex& blockIndex)
{
	assert(blockIndex.IsAllValueBiggerThanZero());
	m_tree.AddColumnRowSet(blockIndex);
}

// ----------------------------------------------------------------------------
// block 추가시 elementTree을 추가하기.
bool CElementTree::InsertElementTree(const CBlockBaseData::BLOCKTYPE blockType, const CBlockIndex& blockIndex)
{
	return m_tree.InsertElementTree(blockType, blockIndex);
}

// ----------------------------------------------------------------------------
// 모든 CColumnSet 삭제
void CElementTree::DeleteAll()
{
	if(m_order.GetCount() <= 0)
		return;

	m_order.RemoveAll();
	m_tree.DeleteAll();
}

// CRowSet 삭제
void CElementTree::DeleteRowSet(const CBlockIndex& blockIndex)
{
	CDequePointerList<CElement> elementList;
	m_tree.GetElementList(blockIndex, elementList);

	if(elementList.GetCount() > 0)
		m_order.Remove(elementList);

	m_tree.DeleteRowSet(blockIndex);
}

// element 순서적으로 지우기.
void CElementTree::DeleteTail()
{
	CElement* pElement = m_order.GetTail();
	if(pElement == NULL)
		return;

	DeleteElement(pElement);
}

// 특정 element만 삭제
bool CElementTree::DeleteElement(const CToolEnumData::ElementType eElementTYPE)
{
	// 해당 type인 element를 삭제하고 그 point를 구한다.
	CDequePointerList<CElement> deletedElementList;
	if(!m_tree.DeleteElement(eElementTYPE, deletedElementList))
		return false;

	m_order.Remove(deletedElementList);
	return true;
}

// element 삭제
void CElementTree::DeleteElement(CElement* pElement)
{
	assert(pElement != NULL);

	m_order.Remove(pElement);
	m_tree.DeleteElement(pElement);
}

// element 삭제
bool CElementTree::DeleteElementPos(const long lAddress,BOOL bListOnly)
{
	if(GetElementCount() <= 0 || lAddress == 0)
		return FALSE;

	//Order데 대한 관리가 안된다
	bool bDeleted = m_tree.DeleteElement(lAddress,bListOnly);		
	if(bDeleted == true)
	{
		CElement *pElement = (CElement *)lAddress;
		m_order.Remove(pElement);
	}
	return bDeleted;
}

// ----------------------------------------------------------------------------
// block 이동
bool CElementTree::MoveElementTree(const CBlockIndex& sourceBlockIndex, const CBlockIndex& targetBlockIndex)
{
	return m_tree.MoveElementTree(sourceBlockIndex, targetBlockIndex);
}

// element 순서(order) 바꾸기
bool CElementTree::ChangeElementOrder(CElement* pElement)
{
	return m_order.ChangeOrder(pElement);
}

// ----------------------------------------------------------------------------
// element 찾기
CElement* CElementTree::FindElement(IChartManager* pMainBlock, CExtractingDataOfModifyType* pExtractingData, CToolEnumData::ModifyType& modifyType)
{
	return m_tree.FindElement(pMainBlock, pExtractingData, modifyType);
}

// ----------------------------------------------------------------------------
// element 그리기.
void CElementTree::DrawElement(CDC* pDC, CElement* pSelectedElement, IChartManager* pMainBlock)
{
	m_tree.DrawElement(pDC, pSelectedElement, pMainBlock);
}

CElement *CElementTree::CopyElement(CElement* pSelectedElement)
{
	CElement *lReturn = m_tree.CopyElement(pSelectedElement);
	if(lReturn != 0)
		m_order.AddTail(pSelectedElement);
	return lReturn;
}

long CElementTree::MoveElement(CElement* pSelectedElement)
{	
	long lReturn = m_tree.MoveElement(pSelectedElement);
	if(lReturn == 0)
		m_order.AddTail(pSelectedElement);

	return lReturn;
}

CBlockIndex CElementTree::GetBlockIndexInElement(const CElement* pSelectedElement) const
{
	assert(pSelectedElement != NULL);
	
	return m_tree.GetBlockIndexInElement(pSelectedElement);
}

CElement* CElementTree::GetTail()
{
	return m_order.GetTail();
}

CTree& CElementTree::GetTree()
{
	return m_tree;
}

COrder& CElementTree::GetOrder()
{
	return m_order;
}

// private ====================================================================
