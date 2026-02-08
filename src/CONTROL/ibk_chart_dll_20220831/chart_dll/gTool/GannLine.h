// GannLine.h: interface for the CGannLine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GANNLINE_H__36E3AE49_3FE7_4A6D_806C_6329F151E96B__INCLUDED_)
#define AFX_GANNLINE_H__36E3AE49_3FE7_4A6D_806C_6329F151E96B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ToolBase.h"

class CGannLine : public CToolBase  
{
public:
	CGannLine(CWnd *pView, CWnd *pParent, CRect toolRC, CRect drawRC, int bgColor, CIndcBase* pIndcBase, int tkname);
	virtual ~CGannLine();

private:
	CPoint		m_basePOINT;
	CPoint		m_gannEXT;

public:

	void	DrawTool(CDC* pDCz);
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
	bool	CalculateDrawingPosition();
};

#endif // !defined(AFX_GANNLINE_H__36E3AE49_3FE7_4A6D_806C_6329F151E96B__INCLUDED_)
