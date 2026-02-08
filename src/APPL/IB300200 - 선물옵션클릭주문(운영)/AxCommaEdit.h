#pragma once
// CommaEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAxCommaEdit window

class CAxCommaEdit : public CEdit
{
// Construction
public:
	CAxCommaEdit();

// Attributes
public:
	int		m_nChar{};
	CString		m_beforeStr;
protected:
	bool		m_bOnce{};
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAxCommaEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	CString GetText(bool bTrim = false);
	CString SetComma(CString src);
	virtual ~CAxCommaEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CAxCommaEdit)
	afx_msg void OnChange();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

