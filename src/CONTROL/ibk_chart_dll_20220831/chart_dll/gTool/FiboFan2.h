// FiboFan2.h: interface for the CFiboFan2 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FIBOFAN2_H__D8C25735_2635_4987_865C_4CC5E72D01DE__INCLUDED_)
#define AFX_FIBOFAN2_H__D8C25735_2635_4987_865C_4CC5E72D01DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ToolBase.h"
#define	maxFIBOFAN2	4

class CFiboFan2 : public CToolBase  
{
public:
	CFiboFan2(CWnd *pView, CWnd *pParent, CRect toolRC, CRect drawRC, int bgColor, CIndcBase* pIndcBase, int tkname);
	virtual ~CFiboFan2();

private:
	CPoint		m_basePOINT;
	CPoint		m_orgSPoint;

	CPoint		m_BOXfibo[5];
	CPoint		m_Xfibo[maxFIBOFAN2];
	CPoint		m_Yfibo[maxFIBOFAN2];

	CPoint		m_XfiboEXT[maxFIBOFAN2];
	CPoint		m_YfiboEXT[maxFIBOFAN2];

	CPoint		m_RgnPoint[maxFIBOFAN2 * 5 +1][5];
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

#endif // !defined(AFX_FIBOFAN2_H__D8C25735_2635_4987_865C_4CC5E72D01DE__INCLUDED_)
