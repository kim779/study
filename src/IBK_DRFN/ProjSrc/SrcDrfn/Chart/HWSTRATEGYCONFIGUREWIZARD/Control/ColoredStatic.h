#if !defined(AFX_COLOREDSTATIC_H__DD053FCC_A0CA_422C_AEAF_C99ED10C2213__INCLUDED_)
#define AFX_COLOREDSTATIC_H__DD053FCC_A0CA_422C_AEAF_C99ED10C2213__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColoredStatic.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CColoredStatic window

class CColoredStatic : public CStatic
{
// Construction
public:
	CColoredStatic();
	void SetBorder(BOOL bUse, COLORREF clrBorder = RGB(0,0,0));
	void SetBkColor(BOOL bUse, COLORREF clrBk = RGB(255,255,255));
	void SetCenterText(BOOL bCenter = TRUE) { m_bCenter = bCenter; };
	void SetWindowText(LPCSTR lpszText);
	CString GetWindowText();


// Attributes
private:
	BOOL m_bCenter;
	BOOL m_bHasBorder;
	BOOL m_bHasBkColor;
	COLORREF m_clrBorder;
	COLORREF m_clrBackbround;
	CString m_strText;
	CBrush m_brBorder;
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColoredStatic)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CColoredStatic();

	// Generated message map functions
protected:
	//{{AFX_MSG(CColoredStatic)
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLOREDSTATIC_H__DD053FCC_A0CA_422C_AEAF_C99ED10C2213__INCLUDED_)
