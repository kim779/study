#pragma once
// GroupHeaderCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGroupHeaderCtrl window

class CGroupHeaderCtrl : public CHeaderCtrl
{
// Construction
public:
	CGroupHeaderCtrl();
	virtual ~CGroupHeaderCtrl();

// Attributes
public:

protected:
	CFont	m_font;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGroupHeaderCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	CString GetItemText(int nCol);

	// Generated message map functions
protected:
	//{{AFX_MSG(CGroupHeaderCtrl)
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);	
	//}}AFX_MSG

	virtual void DrawItem(LPDRAWITEMSTRUCT lp);


	DECLARE_MESSAGE_MAP()
};