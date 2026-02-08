// Order.cpp: implementation of the COrder class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "symbol.h"
#include "Order.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "Conversion.h"
#include <assert.h>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
COrder::~COrder()
{
	RemoveAll();
}

void COrder::RemoveAllInRow(const CList<CDrawingSymbol* , CDrawingSymbol*>& symbolsInRowList)
{
	for(int nIndex = 0; nIndex < symbolsInRowList.GetCount(); nIndex++){
		CDrawingSymbol* pDrawingSymbol = symbolsInRowList.GetAt(symbolsInRowList.FindIndex(nIndex));
		assert(pDrawingSymbol != NULL);
		RemoveAt(pDrawingSymbol);
	}	
}

void COrder::RemoveAll()
{
	m_OrderList.RemoveAll();
}

bool COrder::RemoveAt(const CDrawingSymbol *pDrawingSymbol)
{
	POSITION pos = m_OrderList.GetHeadPosition();
	while(pos) {
		POSITION tmp = pos;
		CDrawingSymbol* pSymbol = m_OrderList.GetNext(pos);
		if(pSymbol == pDrawingSymbol) {
			m_OrderList.RemoveAt(tmp);
			return true;
		}
	}
	return false;
}

int COrder::GetCount() const
{
	return m_OrderList.GetCount();
}

void COrder::Add(CDrawingSymbol *pDrawingSymbol)
{
	assert(pDrawingSymbol != NULL);
	m_OrderList.AddTail(pDrawingSymbol);
}

// 해당 element의 순서 가져오기.
int COrder::GetIndex(const CDrawingSymbol *pDrawingSymbol) const
{
	for(int nIndex = 0; nIndex < m_OrderList.GetCount(); nIndex++) {
		CDrawingSymbol* pSymbol = m_OrderList.GetAt(m_OrderList.FindIndex(nIndex));
		if(pSymbol == pDrawingSymbol)
			return nIndex;
	}
	return -1;
}

CDrawingSymbol* COrder::GetTail()
{
	if(m_OrderList.GetCount() <= 0)
		return NULL;

	return m_OrderList.GetTail();
}
