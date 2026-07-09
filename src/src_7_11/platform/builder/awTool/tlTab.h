// tlTab.h: interface for the CtlTab class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TLTAB_H__63FB2C61_50F2_11D4_A024_00001CD7F9BE__INCLUDED_)
#define AFX_TLTAB_H__63FB2C61_50F2_11D4_A024_00001CD7F9BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "tool.h"
#include "ArrowWnd.h"

class AFX_EXT_CLASS CtlTab : public CtlRect  
{
public:
protected:
	CArrowWnd	*m_pArrowWnd;
	CString		m_dat, m_str;
public:
	void	draw2Rect(CDC *dc, CRect rc, COLORREF color1, COLORREF color2);
	void	draw3dRoundRect(CDC *dc, CRect rc, int align, int depth, COLORREF borderColor);
	void	addPage(int nPage);
	void	deletePage(int nPage);
	void	MouseDrag();
	int	SelectableUTab(CPoint point, CRect &rc, CPoint &pt, bool bSet = true);
	int	Selectable(CPoint point, CRect& rc, CPoint &pt, bool bSet = true);
	CString Parser(CString &srcstr, CString substr);
	CtlTab(CChildView* view, UINT id);
	virtual ~CtlTab();

	void	Draw(CDC* dc, int fN);

protected:
	enum	{ taNONE=0, taTAB, taUTAB }	m_type;
	void	drawBegin(CPoint point);
	void	drawTo(CPoint point);
	void	drawEnd();
	void	drawTAB(CDC* dc);
	void	drawUserTAB(CDC* dc);

	void	drawVText(CDC* dc, CString tabs, CRect rect, UINT style);
	void	drawPage(CDC* dc, CRect tabRc, CRect rect, bool left);
	void	drawScroll(CDC* dc, CRect scrRc);
	bool	isScrolled(CDC* dc, int tabN, CString data, int& index, int& width);
	bool	isScrolled(CDC* dc, int tabN, CString data, CString info, int& index, int& width, bool vertical = false);
	int	getSize(CDC* dc, CString tabs, bool vertical = false);
	void	getString(CString string, CStringArray& arrStr);
	int	getLastVisibleIndex();
protected:
	int	m_selected;
};

#endif // !defined(AFX_TLTAB_H__63FB2C61_50F2_11D4_A024_00001CD7F9BE__INCLUDED_)
