// CellItem.cpp: implementation of the CCellItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CellItem.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCellItem::CCellItem(struct _repR *repR)
{
	m_pRep = repR;
}

CCellItem::~CCellItem()
{

}

int CCellItem::getCellCount()
{
	struct	_cellR	*tmp = m_pRep->cellR;
	if (tmp == NULL)
		return 0;
	int cnt = 1;
	while(tmp->next != NULL)
	{
		tmp = tmp->next;
		cnt++;
	}
	return cnt;
}

struct _cellR* CCellItem::getCellPoint(int idx)
{
	struct	_cellR	*tmp = m_pRep->cellR;
	for (int ii = 0; ii < idx; ii++)
		tmp = (tmp->next == NULL?tmp:tmp->next);
	return tmp;
}


void CCellItem::removeLastCell(int lastIdx)
{
	struct	_cellR	*tmp = m_pRep->cellR, *tmp2;
	if (lastIdx > 0)
		tmp = getCellPoint(lastIdx - 1);
	if (tmp == NULL)
		return;
	
	tmp2 = tmp->next;
	tmp->next = NULL;
	tmp = tmp2;

	while(tmp2 != NULL)
	{
		tmp2 = tmp->next;
		free(tmp);
		tmp = tmp2;
	}
	if (lastIdx == 0)
	{
		free(m_pRep->cellR);
		m_pRep->cellR = NULL;
	}
}

void CCellItem::removeCell(int idx)
{
	struct	_cellR	*tmp = m_pRep->cellR, *tmp2;
	if (idx == 0)
	{
		tmp2 = tmp->next;
		m_pRep->cellR = tmp2;
		free(tmp);
		return;
	}
	tmp = getCellPoint(idx - 1);
	if (tmp->next)
	{
		tmp2 = tmp->next;
		tmp->next = tmp2->next;
		free(tmp2);
	}
}

int CCellItem::insertCell(int idx, _cellR *cellR)
{
	if (m_pRep->cellR == NULL)
	{
		addCell(cellR);
		return 0;
	}
	struct	_cellR	*tmp = m_pRep->cellR, *tmp2;

	if (cellR == NULL)
	{
		tmp2 = (struct _cellR *)malloc(sizeof(_cellR));
		ZeroMemory(tmp2, sizeof(_cellR));
	}
	else
		tmp2 = cellR;

	if (idx > 0)
	{
		tmp = getCellPoint(idx - 1);
		tmp2->next = tmp->next;
		tmp->next = tmp2;
		return idx;
	}
	else
	{
		tmp2->next = m_pRep->cellR;
		m_pRep->cellR = tmp2;
		return 0;
	}
	return -1;
}

int CCellItem::addCell(_cellR *cellR)
{
	struct	_cellR	*tmp = m_pRep->cellR, *tmp2;
	int cnt = getCellCount();

	if (cellR == NULL)
	{
		tmp2 = (struct _cellR *)malloc(sizeof(_cellR));
		ZeroMemory(tmp2, sizeof(_cellR));
	}
	else
		tmp2 = cellR;

	if (cnt == 0)
	{
		tmp = tmp2;
		tmp->next = NULL;
		m_pRep->cellR = tmp;
	}
	else
	{
		tmp = getCellPoint(cnt - 1);
		tmp->next = tmp2;
		tmp2->next = NULL;
	}

	return cnt;
}

void CCellItem::RemoveAllCell()
{
	removeLastCell(0);
}

int CCellItem::getIdxOfPtr(_cellR *ptr)
{
	struct _cellR *tmp = m_pRep->cellR;

	for (int ii = 1; ii < getCellCount(); ii++)
	{
		if (tmp == ptr)
			return ii;
		tmp = tmp->next;
	}
	return -1;
}

void CCellItem::moveCell(int from, int to)
{
	int allCnt = getCellCount();
	if (from >= allCnt || to > allCnt || from == to)
		return;
	struct _cellR *fromR, *toR;
	fromR = getCellPoint(from);
	
	if (to == 0)
		toR = NULL;
	else
		toR = getCellPoint(to - 1);
	
	if (from == 0)
		m_pRep->cellR = fromR->next;
	else
		getCellPoint(from - 1)->next = fromR->next;

	if (toR == NULL)
	{
		fromR->next = m_pRep->cellR;
		m_pRep->cellR = fromR;
	}
	else
	{
		fromR->next = toR->next;
		toR->next = fromR;
	}
}
