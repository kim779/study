// AccelFan.h: interface for the CAccelFan class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACCELFAN_H__EA67E1DB_8A95_4439_BEED_C608EC0E344F__INCLUDED_)
#define AFX_ACCELFAN_H__EA67E1DB_8A95_4439_BEED_C608EC0E344F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ToolBase.h"

#define	maxACCELFAN	2

class CAccelFan : public CToolBase  
{
public:
	CAccelFan(CWnd *pView, CWnd *pParent, CRect toolRC, CRect drawRC, int bgColor, CIndcBase* pIndcBase, int tkname);
	virtual ~CAccelFan();

private:
	CPoint		m_basePOINT;

	CPoint		m_BOXaccel[5];
	CPoint		m_Xaccel[maxACCELFAN];
	CPoint		m_Yaccel[maxACCELFAN];

	CPoint		m_XaccelEXT[maxACCELFAN];
	CPoint		m_YaccelEXT[maxACCELFAN];

	CPoint		m_RgnPoint[13][5];

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
	bool	CalculateDrawingPosition();
	void	SetPointArray(int idx, CPoint pt1, CPoint pt2);
};

#endif // !defined(AFX_ACCELFAN_H__EA67E1DB_8A95_4439_BEED_C608EC0E344F__INCLUDED_)
