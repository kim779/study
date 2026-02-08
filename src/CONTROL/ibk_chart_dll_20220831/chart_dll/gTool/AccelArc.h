// AccelArc.h: interface for the CAccelArc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACCELARC_H__AF4C1C5D_D8DE_40AB_ACB4_7F38A1580ABA__INCLUDED_)
#define AFX_ACCELARC_H__AF4C1C5D_D8DE_40AB_ACB4_7F38A1580ABA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ToolBase.h"

#define	maxACCELARC	2

class CAccelArc : public CToolBase  
{
public:
	CAccelArc(CWnd *pView, CWnd *pParent, CRect toolRC, CRect drawRC, int bgColor, CIndcBase* pIndcBase, int tkname);
	virtual ~CAccelArc();
private:
	bool	m_bUPPER;
	CRect	m_ArcRECT[maxACCELARC];

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

#endif // !defined(AFX_ACCELARC_H__AF4C1C5D_D8DE_40AB_ACB4_7F38A1580ABA__INCLUDED_)
