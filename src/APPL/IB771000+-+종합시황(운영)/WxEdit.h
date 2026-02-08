#pragma once
// WxEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWxEdit window

class CWxEdit : public CEdit
{
// Construction
public:
	CWxEdit();

// Attributes
public:
	CWnd*	m_pParent;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWxEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CWxEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CWxEdit)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

