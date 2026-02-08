#pragma once
// SkinListCtrl2.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSkinListCtrl2 window

#define WM_BLCLICK		WM_USER+999
#define WM_CLICK		WM_USER+1000

class CSkinListCtrl2 : public CListCtrl
{
// Construction
public:
	CSkinListCtrl2();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSkinListCtrl2)
	protected:
	virtual void PreSubclassWindow();
	virtual int OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSkinListCtrl2();
	void DrawItem(LPDRAWITEMSTRUCT lp );
	void DrawSubItem( CDC* pDC, int nItem, int nSubItem );

	CPoint GetSelect( void ) { return m_ptCurrent; }

	BOOL DeleteAllItems();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSkinListCtrl2)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);	
	afx_msg void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	afx_msg BOOL OnToolTipText(UINT nID, NMHDR* pNMHDR, LRESULT* pResult);

	BOOL IsSelected( int nItem, int nSub );
	BOOL IsSelectedRow( int nItem );
	int	 CellRectFromPoint(const CPoint & point, RECT * cellrect, int * col) const;

protected:

	CBrush m_brFrame;
	CBrush m_brColumn;
	CBrush m_brBack;
	CBrush m_brSelected;
	CBrush m_brColumnSelected;

	CPoint m_ptCurrent;

	COLORREF m_clrSelect;
	COLORREF m_clrNormal;

public:
	CMapStringToString m_mapYear;	//2016.06.22 KSJ ∏ ø° ¿˙¿Â

	DECLARE_MESSAGE_MAP()
};

