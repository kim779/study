#pragma once
// XXGrid.h : header file
//

#include "../../control/fx_grid/fxgrid.h"

/////////////////////////////////////////////////////////////////////////////
// CXXGrid window

class CXXGrid : public CfxGrid
{
// Construction
public:
	CXXGrid();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXXGrid)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CXXGrid();

	// Generated message map functions
protected:
	//{{AFX_MSG(CXXGrid)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	afx_msg LONG OnEditEnd(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
public:
	virtual void  CreateInPlaceEdit(CRect& rect, DWORD dwStyle, UINT nID,
				int nRow, int nCol, LPCTSTR szText, int limit, int nChar);

	void	SetBkColor(COLORREF bkCOL)	{ m_bkCOL = bkCOL; };
	void	SetPass(CString pass) { m_temp = pass; };
	CString GetPass();

	
	class CInPlaceEdit*	m_pEdit;
	int			m_nPassCol;
	CString			m_temp;
	bool			m_bEdit;

};
