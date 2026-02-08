// Order.h: interface for the COrder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ORDER_H__1B512349_93CD_4CF7_B989_ADED37F4E53B__INCLUDED_)
#define AFX_ORDER_H__1B512349_93CD_4CF7_B989_ADED37F4E53B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "BlockBaseData.h"

class CDrawingSymbol;
class COrder  
{
public:
	CDrawingSymbol* GetTail();
	virtual ~COrder();

	int GetCount() const;
	
	//기호가 추가되었을때 add한다.
	void Add(CDrawingSymbol *pDrawingSymbol);

	int GetIndex(const CDrawingSymbol *pDrawingSymbol) const;

	//해당 Symbol을 지운다.
	void RemoveAll();
	void RemoveAllInRow(const CList<CDrawingSymbol* , CDrawingSymbol*>& symbolsInRowList);
	bool RemoveAt(const CDrawingSymbol *pDrawingSymbol);

private:
	//symbol 순서 리스트.
	CList<CDrawingSymbol*, CDrawingSymbol* > m_OrderList; 
};

#endif // !defined(AFX_ORDER_H__1B512349_93CD_4CF7_B989_ADED37F4E53B__INCLUDED_)
