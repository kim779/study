// CrossLine.h: interface for the CCrossLine class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

class CCrossLine  
{
public:
	CCrossLine(CWnd* pView, CWnd *pParent, COLORREF bg = RGB(255,255,255), COLORREF fg = RGB(255, 0, 0));
	virtual ~CCrossLine();

	void	Resize(CRect DrawRect);
	void	DrawCrossLine(CDC *pDC);
	void	ShowCrossLine(CPoint pt);
	void	HideCrossLine(CPoint pt = CPoint(0,0));

	void	SetColor(COLORREF bg, COLORREF fg);

private:
	CC_MultiLineExApp	*m_pApp;
	CWnd	*m_pParent;
	CWnd	*m_pView;
	CRect	m_DrawRect;
	CPoint	m_curPt;
	CPoint	m_prePt;

	COLORREF	m_bgColor;
	COLORREF	m_fgColor;
};
