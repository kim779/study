#pragma once
// SkinHeaderCtrl.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CSkinHeaderCtrl window

class CAxHeaderCtrl : public CHeaderCtrl
{
// Construction
public:
	CAxHeaderCtrl();

// Attributes
public:
	CFont _font;
// Operations
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSkinHeaderCtrl)
	//}}AFX_VIRTUAL
	void SetHeaderColor(COLORREF txtclr, COLORREF bgclr)
	{
		_bgclr = bgclr;
		_Txtclr = txtclr;
	}

	void SetHeight(int height)
	{
		_height = height;
		RedrawWindow();
	}
	void SetSortArrow(int nCol, int Ascending);

// Implementation
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual ~CAxHeaderCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CAxHeaderCtrl)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	afx_msg LRESULT OnLayOut(WPARAM wParam, LPARAM lParam);
//	CMyBitmap m_bitmapSpan;

	DECLARE_MESSAGE_MAP()
public:
	virtual void PreSubclassWindow();

private:
	COLORREF _bgclr  = RGB(200,200,200);
	COLORREF _Txtclr = RGB(255,255,255);
	int _height = 20;
	int _sortColumn = -1;
	int _bAscending = 0;
	
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
};

