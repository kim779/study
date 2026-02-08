// ArrowTool.h: interface for the CArrowTool class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARROWTOOL_H__B6EE5082_FF40_454C_9891_84691CF04D53__INCLUDED_)
#define AFX_ARROWTOOL_H__B6EE5082_FF40_454C_9891_84691CF04D53__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ToolBase.h"

class CArrowTool : public CToolBase  
{
public:
	CArrowTool(CWnd *pView, CWnd *pParent, CRect toolRC, CRect drawRC, int bgColor, CIndcBase* pIndcBase, int tkname);
	virtual ~CArrowTool();

public:
	void	DrawTool(CDC* pDC);
	void	HideTool();

	void	CreateToolLBDN(CPoint pt, bool &bMouseCapture);
	void	CreateToolLBMV(CPoint pt, bool &bMouseCapture);
	void	CreateToolLBUP(CPoint pt, bool &bMouseCapture);

	void	MoveToolLBDN(CPoint pt, bool &bMouseCapture);
	void	MoveToolLBMV(CPoint pt, bool &bMouseCapture);
	void	MoveToolLBUP(CPoint pt, bool &bMouseCapture);

	bool	PtInToolRegion(CPoint pt);

	bool	calculateDrawingPosition();
	void	DrawHolder(CDC *pDC, bool showHolder);

private:
	CPoint	m_pt[8];
	COLORREF m_arrowRGB;
};

#endif // !defined(AFX_ARROWTOOL_H__B6EE5082_FF40_454C_9891_84691CF04D53__INCLUDED_)
