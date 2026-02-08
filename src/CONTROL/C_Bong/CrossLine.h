// CrossLine.h: interface for the CCrossLine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CROSSLINE_H__D6C56114_D945_453D_B00B_C1BF86385132__INCLUDED_)
#define AFX_CROSSLINE_H__D6C56114_D945_453D_B00B_C1BF86385132__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCrossLine  
{
public:
	CCrossLine(CWnd *pView, CWnd *pParent, COLORREF bg = RGB(255,255,255), COLORREF fg = RGB(255, 0, 0));
	virtual ~CCrossLine();

	void	Resize(CRect DrawRect);
	void	DrawCrossLine(CDC *pDC);
	void	ShowCrossLine(CPoint pt);
	void	HideCrossLine(CPoint pt = CPoint(0,0));

	void	SetColor(COLORREF bg, COLORREF fg);

private:
	CC_BongApp	*m_pApp;
	CWnd	*m_pView;
	CWnd	*m_pParent;
	CRect	m_DrawRect;
	CPoint	m_curPt;
	CPoint	m_prePt;

	COLORREF	m_bgColor;
	COLORREF	m_fgColor;
};

#endif // !defined(AFX_CROSSLINE_H__D6C56114_D945_453D_B00B_C1BF86385132__INCLUDED_)
