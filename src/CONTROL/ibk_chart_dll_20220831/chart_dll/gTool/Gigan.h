// Gigan.h: interface for the CGigan class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GIGAN_H__7AE3D0AF_B7B5_42FE_864F_08005D820893__INCLUDED_)
#define AFX_GIGAN_H__7AE3D0AF_B7B5_42FE_864F_08005D820893__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ToolBase.h"
#define	maxGigan	5
class CGigan : public CToolBase  
{
public:
	CGigan(CWnd *pView, CWnd *pParent, CRect toolRC, CRect drawRC, int bgColor, CIndcBase* pIndcBase, int tkname);
	virtual ~CGigan();

private:
	CPoint	m_ptGigan[6];

	CPoint	m_RgnPoint[maxGigan + 1][5];
	CPoint	m_ptRegion[2];
	CRect	m_cntRcHolder[2];
	CString	m_date1, m_date2;
	CString m_dispDiff;
	CString	m_dispStr1, m_dispStr2;
	CPoint	m_pt1, m_pt2;

public:
	void	DrawTool(CDC* pDC);
	void	DrawArrow(CDC *pDC, CPoint pt,  CBrush *pBrush, bool bUp);
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

#endif // !defined(AFX_GIGAN_H__7AE3D0AF_B7B5_42FE_864F_08005D820893__INCLUDED_)
