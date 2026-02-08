// Daedeung.h: interface for the CDaedeung class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DAEDEUNG_H__A676D52F_4060_4C46_A33E_E98EB6652249__INCLUDED_)
#define AFX_DAEDEUNG_H__A676D52F_4060_4C46_A33E_E98EB6652249__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ToolBase.h"
#define	maxDaedeung	3

class CDaedeung : public CToolBase  
{
public:
	CDaedeung(CWnd *pView, CWnd *pParent, CRect toolRC, CRect drawRC, int bgColor, CIndcBase* pIndcBase, int tkname);
	virtual ~CDaedeung();

	void	InitValue(int lGap, int rGap);
private:
	CPoint	m_ptDaedeung[3];

	CPoint	m_RgnPoint[maxDaedeung + 1][5];
	CPoint	m_ptRegion[2];
	CRect	m_cntRcHolder[2];

	int	m_lGap, m_rGap;

public:
	void	DrawTool(CDC* pDCz);
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

#endif // !defined(AFX_DAEDEUNG_H__A676D52F_4060_4C46_A33E_E98EB6652249__INCLUDED_)
