// FiboFan.h: interface for the CFiboFan class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FIBOFAN_H__FA7EE164_B5C3_48D5_B524_477F099109D9__INCLUDED_)
#define AFX_FIBOFAN_H__FA7EE164_B5C3_48D5_B524_477F099109D9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ToolBase.h"
#define	maxFIBOFAN	4

class CFiboFan : public CToolBase  
{
public:
	CFiboFan(CWnd *pView, CWnd *pParent, CRect toolRC, CRect drawRC, int bgColor, CIndcBase* pIndcBase, int tkname);
	virtual ~CFiboFan();

private:
	CPoint		m_basePOINT;
	CPoint		m_orgSPoint;

	CPoint		m_BOXfibo[5];
	CPoint		m_Xfibo[maxFIBOFAN];
	CPoint		m_Yfibo[maxFIBOFAN];

	CPoint		m_XfiboEXT[maxFIBOFAN];
	CPoint		m_YfiboEXT[maxFIBOFAN];

	CPoint		m_RgnPoint[maxFIBOFAN * 5 +1][5];
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

#endif // !defined(AFX_FIBOFAN_H__FA7EE164_B5C3_48D5_B524_477F099109D9__INCLUDED_)
