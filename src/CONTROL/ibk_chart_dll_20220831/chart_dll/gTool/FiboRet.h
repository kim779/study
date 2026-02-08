// FiboRet.h: interface for the CFiboRet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FIBORET_H__C7CBA594_F179_4E4B_845D_2438D23542C4__INCLUDED_)
#define AFX_FIBORET_H__C7CBA594_F179_4E4B_845D_2438D23542C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ToolBase.h"
#define	maxFIBORET	11

class CFiboRet : public CToolBase  
{
public:
	CFiboRet(CWnd *pView, CWnd *pParent, CRect toolRC, CRect drawRC, int bgColor, CIndcBase* pIndcBase, int tkname);
	virtual ~CFiboRet();

private:
	CPoint	m_Retracement1[maxFIBORET];
	CPoint	m_Retracement2[maxFIBORET];

	CPoint	m_RgnPoint[maxFIBORET + 1][5];
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
	void	SetPointArray(int idx, CPoint pt1, CPoint pt2);
};

#endif // !defined(AFX_FIBORET_H__C7CBA594_F179_4E4B_845D_2438D23542C4__INCLUDED_)
