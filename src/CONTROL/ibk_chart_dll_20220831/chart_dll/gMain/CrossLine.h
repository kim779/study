// CrossLine.h: interface for the CCrossLine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CROSSLINE_H__3B78669F_066C_4ADB_9CC8_6AFA3D0A85F5__INCLUDED_)
#define AFX_CROSSLINE_H__3B78669F_066C_4ADB_9CC8_6AFA3D0A85F5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCrossLine  
{
public:
	CCrossLine(CWnd *pView, class CObjMgr* pObjMgr, bool bVisible);
	virtual ~CCrossLine();

private:
	CAxisGMainApp	*m_pApp;
	CWnd		*m_pView;
	CObjMgr	*m_pObjMgr;
	CPoint	m_curPt;
	CPoint	m_prePt;

	CRect	m_DrawRect;
	bool	m_bVisable;

public:
	void	ResizeCrossLine(CRect rect);
	void	DrawCrossLine(CDC* pDC);
	void	ShowCrossLine(CPoint pt);
	void	HideCrossLine(CPoint pt = CPoint(0,0));

	bool	GetVisable()	{ return m_bVisable; }
	void	SetVisable(bool bVisible)	{ m_bVisable = bVisible; }
};

#endif // !defined(AFX_CROSSLINE_H__3B78669F_066C_4ADB_9CC8_6AFA3D0A85F5__INCLUDED_)
