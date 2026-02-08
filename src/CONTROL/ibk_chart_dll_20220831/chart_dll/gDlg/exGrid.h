#if !defined(AFX_XXGRID_H__4ED53BA1_7011_46A9_B21E_A260A65CB5E0__INCLUDED_)
#define AFX_XXGRID_H__4ED53BA1_7011_46A9_B21E_A260A65CB5E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif

#include "../../../fx_grid/fxgrid.h"

#define	GVAT_3DHEAD		0x00010000

class CExGrid : public CfxGrid  
{
public:
	CExGrid();
	virtual ~CExGrid();

	void	SetGridBkColor(COLORREF color)	{ m_bkCOL = color; }
	int	GetSelectRow()			{ return m_idSelect.row; }

protected:
	void	OnDraw(CDC* pDC);
	BOOL	DrawFixedCell(CDC* pDC, int nRow, int nCol, CRect rect, BOOL bEraseBk=FALSE);
	BOOL	RedrawCell(int nRow, int nCol, CDC* pDC = NULL);

	// Generated message map functions
};

#endif // !defined(AFX_XXGRID_H__4ED53BA1_7011_46A9_B21E_A260A65CB5E0__INCLUDED_)
