// CellItem.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "CellItem.h"


// CCellItem

CCellItem::CCellItem(struct _repR* repR)
{
	m_repR = repR;
}

CCellItem::~CCellItem()
{
}

int CCellItem::AddCell(struct _cellR* cellR)
{
	struct	_cellR	*tmp = m_repR->cellR, *tmp2;
	int	cnt = GetCount();

	if (cellR == NULL)
	{
		tmp2 = (struct _cellR *)malloc(sizeof(_cellR));
		ZeroMemory(tmp2, sizeof(struct _cellR));
	}
	else
		tmp2 = cellR;

	if (cnt == 0)
	{
		tmp = tmp2;
		tmp->next = NULL;
		m_repR->cellR = tmp;
	}
	else
	{
		tmp = GetCell(cnt - 1);
		tmp->next = tmp2;
		tmp2->next = NULL;
	}

	return cnt;
}

int CCellItem::InsertCell(int index, struct _cellR* cellR)
{
	if (m_repR->cellR == NULL)
	{
		AddCell(cellR);
		return 0;
	}

	struct	_cellR	*tmp = m_repR->cellR, *tmp2;
	if (cellR == NULL)
	{
		tmp2 = (struct _cellR *)malloc(sizeof(_cellR));
		ZeroMemory(tmp2, sizeof(_cellR));
	}
	else
		tmp2 = cellR;

	if (index > 0)
	{
		tmp = GetCell(index - 1);
		tmp2->next = tmp->next;
		tmp->next = tmp2;
		return index;
	}
	else
	{
		tmp2->next = m_repR->cellR;
		m_repR->cellR = tmp2;
		return 0;
	}
	return -1;
}

void CCellItem::RemoveAll()
{
	RemoveLastCell(0);
}

void CCellItem::RemoveCell(int index)
{
	struct	_cellR	*tmp = m_repR->cellR, *tmp2;

	if (index == 0)
	{
		tmp2 = tmp->next;
		m_repR->cellR = tmp2;
		freeCell(tmp);
		free(tmp);
		return;
	}

	tmp = GetCell(index - 1);
	if (tmp->next)
	{
		tmp2 = tmp->next;
		tmp->next = tmp2->next;
		freeCell(tmp2);
		free(tmp2);
	}
}

void CCellItem::RemoveLastCell(int index)
{
	struct	_cellR	*tmp = m_repR->cellR, *tmp2;

	if (index > 0)
		tmp = GetCell(index - 1);
	if (tmp == NULL)
		return;
	
	tmp2 = tmp->next;
	tmp->next = NULL;
	tmp = tmp2;

	while (tmp2 != NULL)
	{
		tmp2 = tmp->next;
		freeCell(tmp);
		free(tmp);
		tmp = tmp2;
	}

	if (index == 0)
	{
		freeCell(m_repR->cellR);
		free(m_repR->cellR);
		m_repR->cellR = NULL;
	}
}

struct _cellR* CCellItem::GetCell(int index)
{
	struct	_cellR	*tmp = m_repR->cellR;

	for (int ii = 0; ii < index; ii++)
		tmp = (tmp->next == NULL ? tmp : tmp->next);
	return tmp;
}

int CCellItem::GetCount()
{
	struct	_cellR	*tmp = m_repR->cellR;

	if (tmp == NULL)
		return 0;

	int	count = 1;
	while (tmp->next != NULL)
	{
		tmp = tmp->next;
		count++;
	}
	return count;
}

void CCellItem::MoveCell(int from, int to)
{
	int	count = GetCount();

	if (from >= count || to > count || from == to)
		return;

	struct _cellR *fromR, *toR;
	fromR = GetCell(from);
	
	if (to == 0)
		toR = NULL;
	else
		toR = GetCell(to - 1);
	
	if (from == 0)
		m_repR->cellR = fromR->next;
	else
		GetCell(from - 1)->next = fromR->next;

	if (toR == NULL)
	{
		fromR->next = m_repR->cellR;
		m_repR->cellR = fromR;
	}
	else
	{
		fromR->next = toR->next;
		toR->next = fromR;
	}
}

void CCellItem::freeCell(struct _cellR* cellR)
{
	if (cellR->fName)
		delete[] cellR->fName;
	if (cellR->margin)
	{
		cellR->margin->RemoveAll();
		delete cellR->margin;
	}
}
