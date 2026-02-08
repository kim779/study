// RowSet.cpp: implementation of the CRowSet class.
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RowSet.h"

#include "Element.h"
#include "ElementFactory.h"
#include "AnalysisData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////
// class CRowSet

CRowSet* CRowSet::Make()
{
	return new CRowSet;
}

void CRowSet::Delete(CRowSet* pRowSet)
{
	delete pRowSet;
}


// public =====================================================================
CRowSet::~CRowSet()
{
	DeleteAll();
}

// ----------------------------------------------------------------------------
void CRowSet::SetAutoPrice(const bool bIsAutoPrice)
{
	for(int nIndex = 0; nIndex < m_ElementList.GetCount(); nIndex++){
		CElement* pElement = m_ElementList.GetAt(nIndex);
		assert(pElement != NULL);
		pElement->GetCommonEnvironment().SetAutoPrice(bIsAutoPrice);
	}
}

// ----------------------------------------------------------------------------
// element 색 변경하기.
void CRowSet::ChangeAllElementsColor(const COLORREF& penColor, const COLORREF& textColor)
{
	for(int nIndex = 0; nIndex < m_ElementList.GetCount(); nIndex++)
	{
		CElement* pElement = m_ElementList.GetAt(nIndex);
		assert(pElement != NULL);
		pElement->ChangePenColor(penColor);
		pElement->ChangeTextColor(textColor);
	}
}

//[A00000552]alzioyes_20110628 element 선두께 변경하기.
void CRowSet::ChangeAllElementsThick(int nElementType, int nNewWidth)
{
	for(int nIndex = 0; nIndex < m_ElementList.GetCount(); nIndex++)
	{
		CElement* pElement = m_ElementList.GetAt(nIndex);
		assert(pElement != NULL);
		if(pElement->GetElementType()==nElementType)
			pElement->ChangePenThick(nNewWidth);
	}
}

// ----------------------------------------------------------------------------
// element count
int CRowSet::GetElementCount() const
{
	return m_ElementList.GetCount();
}

// 저장할 모든 element data.
CString CRowSet::GetSavedElementsData(const COrder& order) const
{
	CString savedElementsData("<Row>\r\n");
	for(int nIndex = 0; nIndex < m_ElementList.GetCount(); nIndex++){
		CElement* pElement = m_ElementList.GetAt(nIndex);
		assert(pElement != NULL);
		savedElementsData += pElement->GetSavedElementData(order);
	}

	return savedElementsData + "<\\Row>\r\n";
}

// 해당 elements 가져오기.
void CRowSet::GetElementList(CDequePointerList<CElement>& elementList)
{
	for(int nIndex = 0; nIndex < m_ElementList.GetCount(); nIndex++){
		CElement* pElement = m_ElementList.GetAt(nIndex);
		assert(pElement != NULL);
		elementList.AddTail(pElement);
	}
}

void CRowSet::GetElementList(const CToolEnumData::ElementType eElementTYPE, CDequePointerList<CElement>& elementList)
{
	for(int nIndex = 0; nIndex < m_ElementList.GetCount(); nIndex++){
		CElement* pElement = m_ElementList.GetAt(nIndex);
		assert(pElement != NULL);
		if(pElement->GetElementType() == eElementTYPE)
			elementList.AddTail(pElement);
	}
}

// ----------------------------------------------------------------------------
/// list에 element 추가.
void CRowSet::AddElement(CElement* pElement)
{
	assert(pElement != NULL);
	if(IsInSameElement(pElement))
		return;

	m_ElementList.AddTail(pElement);
}

// ----------------------------------------------------------------------------
// 모든 element 삭제
void CRowSet::DeleteAll()
{
	while(m_ElementList.GetCount() > 0){
		Delete(m_ElementList.GetTail(), false);
	}

	m_ElementList.RemoveAll();
}

// 특정 type인 element만 삭제
// 삭제된 경우에는 해당 element의 point를 list에 담는다.
// -> 객체를 생성한 순서를 저장되는 class에서도 remove 해야 하기때문.
bool CRowSet::Delete(const CToolEnumData::ElementType eElementTYPE, CDequePointerList<CElement>& deletedElementList)
{
	CDequePointerList<CElement> elementList;
	GetElementList(eElementTYPE, elementList);
	if(elementList.GetCount() <= 0)
		return false;

	bool bIsDeleted = false;
	for(int nIndex = 0; nIndex < elementList.GetCount(); nIndex++)
	{
		CElement* pElement = elementList.GetAt(nIndex);
		assert(pElement != NULL);
		if(pElement->GetElementType() != eElementTYPE)
			continue;

		deletedElementList.AddTail(pElement);
		Delete(pElement, false);
		bIsDeleted = true;
	}

	return bIsDeleted;
}

// element 삭제
// -> bCheckSameElement = true : pElement가 CRowSet의 객체인지 검사를 한다.
bool CRowSet::Delete(CElement* pElement, const bool bCheckSameElement)
{
	assert(pElement != NULL);
	if(bCheckSameElement && !IsInSameElement(pElement))
		return false;

	CElementFactory::Delete(pElement);
	m_ElementList.RemoveAt(pElement);
	return true;
}

bool CRowSet::Delete(const long lElementAddres,BOOL bListOnly)
{
	assert(lElementAddres != 0);	

	bool bFind = false;
	long lTempAdress = 0;
	CElement *pElement = NULL;
	for(int i=0;i<GetElementCount();i++)
	{
		pElement = m_ElementList.GetAt(i);
		lTempAdress = (long)pElement;
		if(lTempAdress == lElementAddres)
		{
			if(bListOnly == FALSE)
				CElementFactory::Delete(pElement);
			
			m_ElementList.RemoveAt(pElement);
			bFind = true;
			break;
		}
	}	
	return bFind;
}

// ----------------------------------------------------------------------------
// element 찾기
CElement* CRowSet::FindElement(CExtractingDataOfModifyType* pExtractingData, CToolEnumData::ModifyType& modifyType)
{
	for(int nIndex = 0; nIndex < m_ElementList.GetCount(); nIndex++){
		CElement* pElement = m_ElementList.GetAt(nIndex);
		assert(pElement != NULL);
		modifyType = pElement->GetModifyType(pExtractingData);
		if(modifyType != CToolEnumData::MODIFY_NONE)
			return pElement;
	}
	return NULL;
}

// ----------------------------------------------------------------------------
// element 그리기
void CRowSet::DrawElement(CDC* pDC, CElement* pSelectedElement, 
						  const CDrawingRegions& drawingRegions,const CCoordinateCalculatorData& calData)
{
	if(!WillDrawElement(drawingRegions))
		return;

	for(int nIndex = 0; nIndex < m_ElementList.GetCount(); nIndex++)
	{
		CElement* pElement = m_ElementList.GetAt(nIndex);
		assert(pElement != NULL);
		pElement->Draw(pDC,drawingRegions,calData);
		if(pElement == pSelectedElement)
			pElement->DrawShadowAndGrip(pDC, drawingRegions, calData);
	}
}

CElement *CRowSet::CopyElement(CElement* pSelectedElement)
{
	CElement *lAddress = NULL;
	for(int nIndex = 0; nIndex < m_ElementList.GetCount(); nIndex++)
	{
		CElement* pElement = m_ElementList.GetAt(nIndex);
		assert(pElement != NULL);
		if(pElement == pSelectedElement)
		{
			CElement *pNewElement = CElementFactory::Copy(pElement);
			if(pNewElement != NULL)
			{
				lAddress = pNewElement;
				m_ElementList.AddTail(pNewElement);
			}			
			break;
		}
	}

	return lAddress;
}

long CRowSet::MoveElement(CElement* pSelectedElement)
{
	long lAddress = 0;
	for(int nIndex = 0; nIndex < m_ElementList.GetCount(); nIndex++)
	{
		CElement* pElement = m_ElementList.GetAt(nIndex);
		assert(pElement != NULL);
		if(pElement == pSelectedElement)
		{
			lAddress = (long)pSelectedElement;
			break;
		}
	}

	if(lAddress == 0)
		m_ElementList.AddTail(pSelectedElement);

	return lAddress;
}

// list에 같은 element가 있으면 = true
bool CRowSet::IsInSameElement(const CElement* pElement) const
{
	for(int nIndex = 0; nIndex < m_ElementList.GetCount(); nIndex++){
		if(pElement == m_ElementList.GetAt(nIndex))
			return true;
	}
	return false;
}

// privata ====================================================================
// element를 그릴지 여부. (그리기 = true)
bool CRowSet::WillDrawElement(const CDrawingRegions& drawingRegions) const
{
	return (m_ElementList.GetCount() > 0 && drawingRegions.GetCount() > 0);
}

// ----------------------------------------------------------------------------
