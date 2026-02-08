// FiboLine.h: interface for the CFiboLine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FIBOLINE_H__90B7740B_E28F_40F5_B461_8EBF72345296__INCLUDED_)
#define AFX_FIBOLINE_H__90B7740B_E28F_40F5_B461_8EBF72345296__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ToolBase.h"
#define	maxFIBOLINE	12

class CFiboLine : public CToolBase  
{
public:
	CFiboLine(CWnd *pView, CWnd *pParent, CRect toolRC, CRect drawRC, int bgColor, CIndcBase* pIndcBase, int tkname);
	virtual ~CFiboLine();

private:
	CRect	m_cntRcHolder[maxFIBOLINE];
	double	m_szOneDay;
public:
	void ResizeTool(CRect drawRect, CRect toolRect);

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

	CPoint	CenterPointAdjust(CPoint pt);
	CPoint	m_ptRegion[maxFIBOLINE];
};

#endif // !defined(AFX_FIBOLINE_H__90B7740B_E28F_40F5_B461_8EBF72345296__INCLUDED_)
