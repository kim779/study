#if !defined(AFX_AXMEDIT_H__F47610CF_B955_48FE_AEB2_E56078E27350__INCLUDED_)
#define AFX_AXMEDIT_H__F47610CF_B955_48FE_AEB2_E56078E27350__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AxMEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAxMEdit window

class AFX_EXT_CLASS CAxMEdit : public CEdit
{
// Construction
public:
	CAxMEdit();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAxMEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAxMEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CAxMEdit)
	afx_msg void OnChange();
	//}}AFX_MSG
	afx_msg LRESULT OnSetMessageHandler(WPARAM, LPARAM);
	afx_msg LRESULT OnGetMessageHandler(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()
protected:
	CWnd* m_messageHandler;
	LRESULT Notify(UINT msg, WPARAM wParam = 0, LPARAM lParam = 0);
public:
	inline void SetMessageHandler(CWnd* wnd) { m_messageHandler = wnd; }
	inline CWnd* GetMessageHandler() const { return m_messageHandler; }
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AXMEDIT_H__F47610CF_B955_48FE_AEB2_E56078E27350__INCLUDED_)
