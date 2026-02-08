// XXGrid.h: interface for the CXXGrid class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "../../control/fx_grid/fxgrid.h"

class CXXGrid : public CfxGrid  
{
public:
	CXXGrid();
	virtual ~CXXGrid();
	virtual void  CreateInPlaceEdit(CRect& rect, DWORD dwStyle, UINT nID,
				int nRow, int nCol, LPCTSTR szText, int limit, int nChar);

	void	SetBkColor(COLORREF bkCOL)	{ m_bkCOL = bkCOL; };
	void	SetPass(CString pass) { m_temp = pass; };
	CString GetPass();

	
	class CInPlaceEdit*	m_pEdit;
	int			m_nPassCol;
	CString			m_temp;
	bool			m_bEdit;

	//{{AFX_MSG(CXXGrid)
	//}}AFX_MSG
	afx_msg LONG OnEditEnd(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

