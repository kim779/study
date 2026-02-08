// Figure.h: interface for the CFigure class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FIGURE_H__B71C5B3D_CCF7_47DB_8AAE_E7D46B0BBCFA__INCLUDED_)
#define AFX_FIGURE_H__B71C5B3D_CCF7_47DB_8AAE_E7D46B0BBCFA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ToolBase.h"

class CFigure : public CToolBase  
{
public:
	CFigure(CWnd *pView, CWnd *pParent, CRect toolRC, CRect drawRC, int bgColor, CIndcBase* pIndcBase, int tkname);
	virtual ~CFigure();

private:
	bool	m_bDrawPosi;	// added by schbang 2006.04.04

public:
	void	DrawTool(CDC* pDC);
	void	HideTool();

	void	CreateToolLBDN(CPoint pt, bool &bMouseCapture);
	void	CreateToolLBMV(CPoint pt, bool &bMouseCapture);
	void	CreateToolLBUP(CPoint pt, bool &bMouseCapture);

	void	ChangeToolLBDN(CPoint pt, bool &bMouseCapture);
	void	ChangeToolLBMV(CPoint pt, bool &bMouseCapture);
	void	ChangeToolLBUP(CPoint pt, bool &bMouseCapture);

	void	MoveToolLBDN(CPoint pt, bool &bMouseCapture);
	void	MoveToolLBMV(CPoint pt, bool &bMouseCapture);
	void	MoveToolLBUP(CPoint pt, bool &bMouseCapture);

	bool	PtInToolRegion(CPoint pt);

private:
	void	CalculateDrawingPosition();

};

#endif // !defined(AFX_FIGURE_H__B71C5B3D_CCF7_47DB_8AAE_E7D46B0BBCFA__INCLUDED_)
