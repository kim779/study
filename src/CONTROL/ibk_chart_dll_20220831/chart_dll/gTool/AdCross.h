// AdCross.h: interface for the CAdCross class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ADCROSS_H__E76174A7_D33A_4FC4_ABAF_71D1D50D272B__INCLUDED_)
#define AFX_ADCROSS_H__E76174A7_D33A_4FC4_ABAF_71D1D50D272B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ToolBase.h"
#define	maxHLINE	1

class CAdCross : public CToolBase  
{
public:
	CAdCross(CWnd *pView, CWnd *pParent, CRect toolRC, CRect drawRC, int bgColor, CIndcBase* pIndcBase, int tkname);
	virtual ~CAdCross();

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

private:
	bool	calculateDrawingPosition(CDC* pDC);

private:
	CPoint	m_hLine[2];
	CPoint	m_vLine[2];

	CPoint	m_ptRegion;
};

#endif // !defined(AFX_ADCROSS_H__E76174A7_D33A_4FC4_ABAF_71D1D50D272B__INCLUDED_)
