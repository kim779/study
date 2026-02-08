// QuardLine.h: interface for the CQuardLine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUARDLINE_H__4F2D91A9_9B72_4AA0_B02B_3FC879951426__INCLUDED_)
#define AFX_QUARDLINE_H__4F2D91A9_9B72_4AA0_B02B_3FC879951426__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ToolBase.h"
#define	maxQuardLine	7

class CQuardLine : public CToolBase  
{
public:
	CQuardLine(CWnd *pView, CWnd *pParent, CRect toolRC, CRect drawRC, int bgColor, CIndcBase* pIndcBase, int tkname);
	virtual ~CQuardLine();

private:
	CPoint	m_ptQuardLine[10];

	CPoint	m_RgnPoint[maxQuardLine + 1][5];
	CPoint	m_ptRegion[2];
	CRect	m_cntRcHolder[2];

public:
	void	ResizeTool(float hRatio, float vRatio, bool bforce);
	void	DrawTool(CDC* pDC);
	void	HideTool();

	void	InflateHolder();

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
	void	SetPointArray(int idx, CPoint pt1, CPoint pt2);
	bool	CalculateDrawingPosition();
};

#endif // !defined(AFX_QUARDLINE_H__4F2D91A9_9B72_4AA0_B02B_3FC879951426__INCLUDED_)
