// VLine.h: interface for the CVLine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VLINE_H__51A11DD5_83E4_4EAB_8B9F_445BD923C837__INCLUDED_)
#define AFX_VLINE_H__51A11DD5_83E4_4EAB_8B9F_445BD923C837__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ToolBase.h"
#define	maxVLINE	1

class CVLine : public CToolBase  
{
public:
	CVLine(CWnd *pView, CWnd *pParent, CRect toolRC, CRect drawRC, int bgColor, CIndcBase* pIndcBase, int tkname);
	virtual ~CVLine();

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

#endif // !defined(AFX_VLINE_H__51A11DD5_83E4_4EAB_8B9F_445BD923C837__INCLUDED_)
