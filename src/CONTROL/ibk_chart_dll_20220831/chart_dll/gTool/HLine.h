// HLine.h: interface for the CHLine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HLINE_H__84DC7C4E_D18E_40E2_AF11_75AE1EDF362D__INCLUDED_)
#define AFX_HLINE_H__84DC7C4E_D18E_40E2_AF11_75AE1EDF362D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ToolBase.h"
#define	maxHLINE	1

class CHLine : public CToolBase  
{
public:
	CHLine(CWnd *pView, CWnd *pParent, CRect toolRC, CRect drawRC, int bgColor, CIndcBase* pIndcBase, int tkname);
	virtual ~CHLine();

private:
	CRect	m_cntRcHolder;

public:

	void	DrawTool(CDC* pDC);
	void	HideTool();

	void	DrawHolder(CDC *pDC, bool showHolder);
	void	HideHolder();
	void	InflateHolder();

	void	CreateToolLBDN(CPoint pt, bool &bMouseCapture);
	void	CreateToolLBMV(CPoint pt, bool &bMouseCapture);
	void	CreateToolLBUP(CPoint pt, bool &bMouseCapture);

	void	MoveToolLBDN(CPoint pt, bool &bMouseCapture);
	void	MoveToolLBMV(CPoint pt, bool &bMouseCapture);
	void	MoveToolLBUP(CPoint pt, bool &bMouseCapture);

	bool	PtInToolHolderRect(CPoint pt);
	bool	PtInToolRegion(CPoint pt);

	CPoint	m_ptRegion;
};

#endif // !defined(AFX_HLINE_H__84DC7C4E_D18E_40E2_AF11_75AE1EDF362D__INCLUDED_)
