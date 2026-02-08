
#pragma once
// OHeaderCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COHeaderCtrl window

#define HEADER_CALL			0
#define HEADER_PUT			1
#define HEADER_NORMAL		2

class COHeaderCtrl : public CHeaderCtrl
{
// Construction
public:
	COHeaderCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COHeaderCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COHeaderCtrl();

	void DrawItem(LPDRAWITEMSTRUCT lp );
	void DrawColumn( int iCol, CDC* pDC );
	void DrawUpColumn( int iStart, int iCol, int type, CDC* pDC );

	BOOL Layout(HDLAYOUT* pHeaderLayout);

	// Generated message map functions
protected:

	CFont m_Font;

	int GetColumnType( int iCol );

	//{{AFX_MSG(COHeaderCtrl)
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

