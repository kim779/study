#if !defined(AFX_AXMCELLMERGECTRL_H__0179B653_36F9_4171_BD11_A57FD316ADD0__INCLUDED_)
#define AFX_AXMCELLMERGECTRL_H__0179B653_36F9_4171_BD11_A57FD316ADD0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AxMCellMergeCtrl.h : header file
//


typedef struct {
	int	col;
	int row;
} CELL, *LPCELL;

typedef struct {
	CELL from;
	CELL to;
} CELLRANGE, *LPCELLRANGE;

typedef struct {
	RECT rc;
	BOOL active;
} CELLINFO, *LPCELLINFO;

typedef CTypedPtrList<CPtrList, LPCELLRANGE> CCellRange;

/////////////////////////////////////////////////////////////////////////////
// CAxMCellMergeCtrl window

class AFX_EXT_CLASS CAxMCellMerge
{
public:
	CAxMCellMerge();
	virtual ~CAxMCellMerge();
protected:
	int			m_cols, m_rows;
	CRect		m_rc;
	CSize		CalcSize();

	CString		GetToken(CString& buffer, char delim = ';');
	void		GetCell(CString str, CELL& cell);
	void		GetMergeOwner(CELL& cell);
	void		ComplexFromTo(CELL& from, CELL& to);

	CCellRange  m_range;
	BOOL		IsMerged(CELL cell);
	BOOL		CellInRange(LPCELLRANGE range, LPCELL cell);
	LPCELLRANGE GetCellRange(CELL cell);
	LPCELLRANGE GetCellRange(int col, int row);
	void		DeleteRange(LPCELLRANGE range);
	void		NormalizeRange(LPCELLRANGE range, BOOL bDelete);

	CELLRANGE   m_virtualRange;
public:
	CELLINFO	m_cells[TMAX_COL][TMAX_ROW];

	void		SetSize(int cols, int rows);
	void		SetSize(CRect Rect);
	void		SetSize(CRect Rect, int cols, int rows);
	BOOL		HitTest(CPoint pt, CELL& cell);

	BOOL		AddMerge(CELL from, CELL to);
	BOOL		SetVirtualRange(CELL from, CELL to);
	void		ReleaseVirtualRange();
	void		SetMergeString(LPCTSTR mergeString);
	CString		GetMergeString();

	int			GetColCount() const { return m_cols; }
	int			GetRowCount() const { return m_rows; }
	LPCELLINFO  GetCell(int col, int row) {
		if (col >= 0 && col < m_cols && row >= 0 && row < m_rows) {
			return &m_cells[col][row];
		}
		else {
			return NULL;
		}
	}
	LPCELLINFO  GetCell(CELL cell) {
		return	GetCell(cell.col, cell.row);
	}
	CRect		GetRect(int col, int row);
	BOOL		IsMerged(int col, int row);
	BOOL		IsActive(int col, int row);
	void		ClearRange();
	BOOL		ExistVirtualRange();
	CRect		GetVirtualRangeRect();
};

class AFX_EXT_CLASS CAxMCellMergeCtrl : public CWnd
{
// Construction
public:
	CAxMCellMergeCtrl();
	BOOL Create(CWnd *parent, CRect rc);
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAxMCellMergeCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAxMCellMergeCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CAxMCellMergeCtrl)
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
protected:
	CWnd*		m_parent;
	CFont		m_font;
	CELL		m_mouseDown;
	HBITMAP		m_back;

	void		Modified();
	COLORREF	m_clrBack, m_clrMerged;
	COLORREF	m_clrSelected;

	void		DrawBack(CDC* dc);
public:
	CAxMCellMerge m_cell;
	void		SetSize(int cols, int rows) {
		m_cell.SetSize(cols,  rows);
		Modified();
	}

	void		SetMergeString(LPCTSTR lpszMergeString);
	CString		GetMergeString();

	void		Initialize();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AXMCELLMERGECTRL_H__0179B653_36F9_4171_BD11_A57FD316ADD0__INCLUDED_)
