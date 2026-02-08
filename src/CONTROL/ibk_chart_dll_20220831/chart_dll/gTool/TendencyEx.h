// TendencyEx.h: interface for the CTendencyEx class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TENDENCYEX_H__7ACA1F48_D60D_43B4_A19D_9FC7D64EC7A9__INCLUDED_)
#define AFX_TENDENCYEX_H__7ACA1F48_D60D_43B4_A19D_9FC7D64EC7A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ToolBase.h"

class CTendencyEx : public CToolBase  
{
public:
	CTendencyEx(CWnd *pView, CWnd *pParent, CRect toolRC, CRect drawRC, int bgColor, CIndcBase* pIndcBase, int tkname);
	CTendencyEx(const CTendencyEx* rhs);
	virtual ~CTendencyEx();

private:
	bool	m_bDrawPosiAngle;	// added by schbang 2006.03.30
	CPoint	m_pointExtStart;
	CPoint	m_pointExtEnd;
	

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
	bool	CalculateDrawingPositionStart(CPoint& rpointStart);
	bool	CalculateDrawingPositionEnd(CPoint& rpointEnd);

	void	ChangeMemberValue();

private:
	void	DrawPosiAngle(CDC* pDC, BOOL bPosi, BOOL bAngle);
};

#endif // !defined(AFX_TENDENCYEX_H__7ACA1F48_D60D_43B4_A19D_9FC7D64EC7A9__INCLUDED_)
