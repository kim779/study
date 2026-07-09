// tlPointer.h: interface for the CtlPointer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TLPOINTER_H__6FB9C5EE_4E6B_11D4_A024_00001CD7F9BE__INCLUDED_)
#define AFX_TLPOINTER_H__6FB9C5EE_4E6B_11D4_A024_00001CD7F9BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "tool.h"
#include "SelFrameWnd.h"
#include <afxtempl.h>

class AFX_EXT_CLASS CtlPointer : public CTOOL  
{
public:
	CtlPointer(CChildView* view);
	virtual ~CtlPointer();

protected :
	void	drawBegin(CPoint point);
	void 	drawTo(CPoint point);
	void	drawEnd();
	void	formBegin(CPoint point);
	void	blockBegin(CPoint point);
	void	formTo(CPoint point);
	void	blockTo(CPoint point);
	bool	IsHit(int idx, CPoint point);
	void	setBlockGroup(int nGroup, int mx, int my);
	void	setBlockTab(struct _formR *tabN, int mx, int my);
	CSelFrameWnd*	m_pSelFrame;
public:
	
	int	m_nGap;
protected :
	struct	_block*	m_block;

	CRect	m_pRc;
	int	m_where;
	int	m_x;
	int	m_y;
	CPoint	m_firstCurPos;
};

#endif // !defined(AFX_TLPOINTER_H__6FB9C5EE_4E6B_11D4_A024_00001CD7F9BE__INCLUDED_)
