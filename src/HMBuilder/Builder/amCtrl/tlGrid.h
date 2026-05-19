#pragma once

#include "tlRect.h"

class AFX_EXT_CLASS CTlGrid : public CTlRect
{
public:
	CTlGrid(CChildView* view, CAmDraw* draw, CFormItem* formItem, UINT type);
	virtual ~CTlGrid();

	void	Draw(CDC* dc, int fN);
	void	MouseDrag();
	bool	OptimumSize(CPoint point);

protected:
	enum	{ tbNONE=0, tbGRID, tbGRIDEX, tbTABLE }	m_type;
	int	m_column;
	int	m_selected;
	class	CMarkWnd* m_markWnd;
	class	CTlImage* m_image;
	int	m_maxRow;

	CRect	getHeadRect(int col, int row, CRect rc, CString txt, int pos = 0, CRect unionRect = CRect(0, 0, 0, 0));
	void	drawFocusRect(CDC *pDC, CRect rc);
	int	movable(CPoint point, CRect& rc);
	int	selectable(CPoint point, CRect& rc, CPoint &pt, bool bSet = true);

	void	drawBegin(CPoint point);
	void	drawTo(CPoint point);
	void	drawEnd();

	void	drawGRID(CDC* dc, int fN);
	void	drawTABLE(CDC* dc, int fN);
	void	drawCOMBO(CDC* dc, CRect rc, CString text);
	void	drawCHECK(CDC* dc, CRect rc, CString text, CString imgPath, BYTE alignC);
	void	drawHead(CDC* dc, int col, CString data, CRect rc, UINT format);
	void	drawPage(CDC* dc, CRect rc, int type);

	int	getColumnWidth(struct _repR* repR, int index);
};