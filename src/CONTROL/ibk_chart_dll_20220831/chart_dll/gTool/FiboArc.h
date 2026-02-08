// FiboArc.h: interface for the CFiboArc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FIBOARC_H__1BD392BB_258F_4FA2_B874_DFED28BD1F8C__INCLUDED_)
#define AFX_FIBOARC_H__1BD392BB_258F_4FA2_B874_DFED28BD1F8C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ToolBase.h"
#define	maxFIBOARC	3

class CFiboArc : public CToolBase  
{
public:
	CFiboArc(CWnd *pView, CWnd *pParent, CRect toolRC, CRect drawRC, int bgColor, CIndcBase* pIndcBase, int tkname);
	virtual ~CFiboArc();

private:
	bool	m_bUPPER;
	CRect	m_ArcRECT[maxFIBOARC];

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
	void	CalculateDrawingPosition();
};

#endif // !defined(AFX_FIBOARC_H__1BD392BB_258F_4FA2_B874_DFED28BD1F8C__INCLUDED_)
