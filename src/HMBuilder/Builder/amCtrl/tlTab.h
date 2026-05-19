#pragma once

#include "tlRect.h"

class AFX_EXT_CLASS CTlTab : public CTlRect
{
public:
	CTlTab(CChildView* view, CAmDraw* draw, CFormItem* formItem, UINT type);
	virtual ~CTlTab();

	void	Draw(CDC* dc, int fN);
	void	MouseDrag();
	int	Selectable(CPoint point, CRect& rc, CPoint &pt, bool bSet = true);

protected:
	enum	{ taNONE=0, taTAB }	m_type;
	int	m_selected;
	class	CMarkWnd* m_markWnd;
	CString	m_dat;
	CString	m_str;

	void	drawBegin(CPoint point);
	void	drawTo(CPoint point);
	void	drawEnd();

	void	drawTAB(CDC* dc);
	void	draw2Rect(CDC *dc, CRect rc, COLORREF color1, COLORREF color2);
	void	draw3dRoundRect(CDC *dc, CRect rc, int align, int depth, COLORREF borderColor);

	void	drawVText(CDC* dc, CString tabs, CRect rect, UINT style);
	void	drawPage(CDC* dc, CRect tabRc, CRect rect, bool left);
	void	drawScroll(CDC* dc, CRect scrRc);

	void	addPage(int nPage);
	void	deletePage(int nPage);

	bool	isScrolled(CDC* dc, int tabN, CString data, int& index, int& width);
	bool	isScrolled(CDC* dc, int tabN, CString data, CString info, int& index, int& width, bool vertical = false);
	int	getSize(CDC* dc, CString tabs, bool vertical = false);
	void	getString(CString string, CStringArray& arrStr);
	int	getLastVisibleIndex(bool last);
};


