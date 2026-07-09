#if !defined(AFX_PANE_H__5EF2F059_ABD7_4726_B178_62974E801CA4__INCLUDED_)
#define AFX_PANE_H__5EF2F059_ABD7_4726_B178_62974E801CA4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Pane.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPane window

class AFX_EXT_CLASS CPane : public CWnd
{
// Construction
public:
	CPane();

// Attributes
public:
protected:
private:
	CString		m_text;
	COLORREF	m_bkClr, m_frClr, m_bxClr;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPane)
	//}}AFX_VIRTUAL

// Implementation
public:
	void setColor(COLORREF bkClr, COLORREF frClr, COLORREF bxClr);
	virtual ~CPane();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPane)
	afx_msg void OnPaint();
	afx_msg long HandleGetText(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT HandleSetText(WPARAM wParam, LPARAM lParam);
	afx_msg long OnGetTextLength(WPARAM , LPARAM );
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PANE_H__5EF2F059_ABD7_4726_B178_62974E801CA4__INCLUDED_)
