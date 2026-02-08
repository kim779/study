// OrderSet.cpp: implementation of the COrderSet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "symbol.h"
#include "OrderSet.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "OrderAndElement.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//indexµµ 0ºÎÅÍ

COrderSet::~COrderSet()
{
	POSITION pos = m_orderList.GetHeadPosition();
	while(pos) {
		COrderAndElement* porder = m_orderList.GetNext(pos);
		if(porder != NULL)
			delete porder;
	}
	m_orderList.RemoveAll();
}

void COrderSet::Add(const int nIndex, CDrawingSymbol *pDrawingSymbol)
{
	if(pDrawingSymbol != NULL)
		m_orderList.AddTail(new COrderAndElement(nIndex, pDrawingSymbol));
}

int COrderSet::GetCount() const
{
	return m_orderList.GetCount();
}

CDrawingSymbol* COrderSet::GetElement(const int nIndex) const
{
	POSITION pos = m_orderList.GetHeadPosition();
	while(pos) {
		COrderAndElement* porder = m_orderList.GetNext(pos);
		if(porder->GetIndex() == nIndex)
			return porder->GetElement();
	}
	return NULL;
}

POSITION COrderSet::GetHeadPosition() const
{
	return m_orderList.GetHeadPosition();
}

COrderAndElement* COrderSet::GetNext(POSITION& rPosition) const
{
	return m_orderList.GetNext(rPosition);
}

COrderAndElement*& COrderSet::GetNext(POSITION& rPosition)
{
	return m_orderList.GetNext(rPosition);
}