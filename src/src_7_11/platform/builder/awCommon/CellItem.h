// CellItem.h: interface for the CCellItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CELLITEM_H__BC36AD9A_C652_409E_B7DD_148133077AFB__INCLUDED_)
#define AFX_CELLITEM_H__BC36AD9A_C652_409E_B7DD_148133077AFB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"
#include "../form_w.h"

class AFX_EXT_CLASS CCellItem  
{
public:
	CCellItem(struct _repR *repR);
	virtual ~CCellItem();
public:
	void RemoveAllCell();
	int addCell(struct _cellR *cellR = NULL);
	int insertCell(int idx, struct _cellR* cellR = NULL);
	void removeCell(int idx);
	void removeLastCell(int lastIdx);
	struct _cellR* getCellPoint(int idx);
	int getCellCount();
public:
	void moveCell(int from, int to);
protected:
	int getIdxOfPtr(_cellR *ptr);
	struct _repR *m_pRep;

};

#endif // !defined(AFX_CELLITEM_H__BC36AD9A_C652_409E_B7DD_148133077AFB__INCLUDED_)
