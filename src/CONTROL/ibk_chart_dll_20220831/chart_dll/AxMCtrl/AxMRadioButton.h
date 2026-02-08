#if !defined(AFX_AXMRADIOBUTTON_H__C97A5BDC_A9D1_4E50_BDE2_C8AD974BF2AC__INCLUDED_)
#define AFX_AXMRADIOBUTTON_H__C97A5BDC_A9D1_4E50_BDE2_C8AD974BF2AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AxMRadioButton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAxMRadioButton window

class AFX_EXT_CLASS CAxMRadioButton : public CButton
{
// Construction
public:
	CAxMRadioButton();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAxMRadioButton)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAxMRadioButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CAxMRadioButton)
	afx_msg LRESULT OnSetMessageHandler(WPARAM, LPARAM);
	afx_msg LRESULT OnGetMessageHandler(WPARAM, LPARAM);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg LRESULT OnSetCheck(WPARAM, LPARAM);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
protected:
	CWnd* m_messageHandler;
	int m_check;
	void ResetOtherRadioButton();
	LRESULT Notify(UINT msg, WPARAM wParam = 0, LPARAM lParam = 0);
	void DrawButton(CDC* dc);
public:
	BOOL	Create(LPCTSTR szText, LPRECT rc, CWnd* parent, UINT id);
	virtual void SetCheck(int nCheck, BOOL bRepaint = TRUE);
	virtual int  GetCheck();

	void	SetMessageHandler(CWnd* wnd) { m_messageHandler = wnd; }
	CWnd*	GetMessageHandler() const { return m_messageHandler; }
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AXMRADIOBUTTON_H__C97A5BDC_A9D1_4E50_BDE2_C8AD974BF2AC__INCLUDED_)
