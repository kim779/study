
#pragma once

#include "../h/form_w.h"

// CCellItem 명령 대상입니다.

class AFX_EXT_CLASS CCellItem
{
public:
	CCellItem(struct _repR* repR);
	virtual ~CCellItem();

public:
	int	AddCell(struct _cellR* cellR = NULL);
	int	InsertCell(int index, struct _cellR* cellR = NULL);
	void	RemoveAll();
	void	RemoveCell(int index);
	void	RemoveLastCell(int index);

	struct	_cellR* GetCell(int index);
	int	GetCount();

	void	MoveCell(int from, int to);

protected:
	struct	_repR*	m_repR;
	void	freeCell(struct _cellR* cellR);
};


