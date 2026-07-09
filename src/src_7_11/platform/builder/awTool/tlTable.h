// tlTable.h: interface for the CtlTable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TLTABLE_H__707B6702_50C2_11D4_A024_00001CD7F9BE__INCLUDED_)
#define AFX_TLTABLE_H__707B6702_50C2_11D4_A024_00001CD7F9BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "tool.h"
#include "ArrowWnd.h"

class AFX_EXT_CLASS CtlTable : public CtlRect  
{
public:
protected:
	CArrowWnd	*m_pArrowWnd;
	class	Cpicture*	m_pic;
	int		m_nMaxrow;
public:
	CRect	getHeadRect(int col, int row, CRect rc, CString txt, int pos = 0, CRect unionRect = CRect(0, 0, 0, 0));
	void	DrawFocusRect(CDC *pDC, CRect rc);
	void	MouseDrag();
	CtlTable(CChildView* view, UINT id);
	virtual ~CtlTable();

	void	Draw(CDC* dc, int fN);
	int	Movable(CPoint point, CRect& rc);
	int	Selectable(CPoint point, CRect& rc, CPoint &pt, bool bSet = true);
	bool	OptimumSize(CPoint point);

protected:
	enum	{ tbNONE=0, tbGRID, tbTABLE }	m_type;
	int	m_column, m_selected;

	void	drawBegin(CPoint point);
	void	drawTo(CPoint point);
	void	drawEnd();
	void	drawGRID(CDC* dc);
	void	drawTABLE(CDC* dc);

	void	drawCOMBO(CDC* dc, CRect rc, CString text);
	void	drawCHECK(CDC* dc, CRect rc, CString text, BYTE alignC);
	void	drawHead(CDC* dc, int col, CString data, CRect rc, UINT format);
	void	drawPage(CDC* dc, CRect rc, int type);

	int	getColumnWidth(struct _repR* repR, int index);
};

#endif // !defined(AFX_TLTABLE_H__707B6702_50C2_11D4_A024_00001CD7F9BE__INCLUDED_)
