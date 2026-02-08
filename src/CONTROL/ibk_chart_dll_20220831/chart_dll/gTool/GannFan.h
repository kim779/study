// GannFan.h: interface for the CGannFan class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GANNFAN_H__2B8888F7_CC5F_4D02_95F3_4CB60B428903__INCLUDED_)
#define AFX_GANNFAN_H__2B8888F7_CC5F_4D02_95F3_4CB60B428903__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ToolBase.h"
#define	maxGANNFAN	4

class CGannFan : public CToolBase  
{
public:
	CGannFan(CWnd *pView, CWnd *pParent, CRect toolRC, CRect drawRC, int bgColor, CIndcBase* pIndcBase, int tkname);
	virtual ~CGannFan();

private:
	int		m_id;
	
	CPoint		m_basePOINT;

	CPoint		m_Xgann[maxGANNFAN];
	CPoint		m_Ygann[maxGANNFAN];

	CPoint		m_gannEXT;
	CPoint		m_XgannEXT[maxGANNFAN];
	CPoint		m_YgannEXT[maxGANNFAN];

	CPoint		m_RgnPoint[maxGANNFAN * 2 +1][5];
	int			m_iSXGab;
	int			m_iEXGab;
public:

	void	DrawTool(CDC* pDCz);
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

#endif // !defined(AFX_GANNFAN_H__2B8888F7_CC5F_4D02_95F3_4CB60B428903__INCLUDED_)
