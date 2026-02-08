#if !defined(AFX_EXTOOLTIP_H__63BC8A46_1E05_4FDD_B11B_352A322F1B31__INCLUDED_)
#define AFX_EXTOOLTIP_H__63BC8A46_1E05_4FDD_B11B_352A322F1B31__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExToolTip.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CExToolTip window

class CExToolTip : public CToolTipCtrl
{
// Construction
public:
	CExToolTip();

// Attributes
public:

// Operations
public:
	BOOL AddRectTool(CWnd *pWnd, LPCTSTR pszText, LPRECT lpRect, UINT nIDTool);
	BOOL AddWindowTool(CWnd *pWnd, LPCTSTR pszText);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExToolTip)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CExToolTip();

	// Generated message map functions
protected:
	//{{AFX_MSG(CExToolTip)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXTOOLTIP_H__63BC8A46_1E05_4FDD_B11B_352A322F1B31__INCLUDED_)
