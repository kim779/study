// Tendency.h: interface for the CTendency class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TENDENCY_H__C831C671_A75C_471B_8A7F_1972E0C32A35__INCLUDED_)
#define AFX_TENDENCY_H__C831C671_A75C_471B_8A7F_1972E0C32A35__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ToolBase.h"

class CTendency : public CToolBase  
{
public:
	CTendency(CWnd *pView, CWnd *pParent, CRect toolRC, CRect drawRC, int bgColor, CIndcBase* pIndcBase, int tkname);
	CTendency(const CTendency* rhs);
	virtual ~CTendency();

private:
	bool	m_bDrawPosiAngle;	// added by schbang 2006.03.30
	CPoint	m_pointExtStart;
	CPoint	m_pointExtEnd;

	CPoint	m_MaxPoint;
	CPoint	m_MinPoint;
	int	m_UpDn;

public:
	virtual void	DrawTool(CDC* pDCz);
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

	void	ChangeMemberValue();
	void	ChangeStartEnd(CDC *pDC);

private:
	bool	CalculateDrawingPositionStart(CPoint& rpointStart);
	bool	CalculateDrawingPositionEnd(CPoint& rpointEnd);

	void	DrawPosiAngle(CDC* pDC, BOOL bPosi, BOOL bAngle);
};

#endif // !defined(AFX_TENDENCY_H__C831C671_A75C_471B_8A7F_1972E0C32A35__INCLUDED_)
