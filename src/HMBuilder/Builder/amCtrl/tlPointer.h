#pragma once

#include "tlRect.h"

class AFX_EXT_CLASS CTlPointer : public CTlRect
{
public:
	CTlPointer(CChildView* view, CAmDraw* draw, CFormItem* formItem);
	virtual ~CTlPointer();
	void	SetGap(int gap)	{ m_gap = gap; }
protected:
	int	m_gap;
	struct	_block*	m_block;
	bool	m_bMoveLine;
	CRect	m_RcLine;

	enum Pos { none = 0, left, top, right, bottom };
	Pos	m_type;

	CRect	m_pRc;
	int	m_where;
	int	m_x;
	int	m_y;
	CPoint	m_firstCurPos;
	class	CPointWnd* m_ptWnd;

	void	drawBegin(CPoint point);
	void 	drawTo(CPoint point);
	void	drawEnd();
	void	formBegin(CPoint point);
	void	blockBegin(CPoint point);
	void	formTo(CPoint point);
	void	blockTo(CPoint point);
	bool	isHit(int idx, CPoint point);
	void	setBlockGroup(int nGroup, int mx, int my);
	void	DrawPositionLine();
	void	fileLog(CString dir, CRect rect);

};