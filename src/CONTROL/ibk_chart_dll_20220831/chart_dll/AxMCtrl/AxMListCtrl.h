#if !defined(AFX_AXMLISTCTRL_H__5D762227_7F37_4A3E_81EF_A2B6BCFB1C08__INCLUDED_)
#define AFX_AXMLISTCTRL_H__5D762227_7F37_4A3E_81EF_A2B6BCFB1C08__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AxMListCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAxMListCtrl window

class AFX_EXT_CLASS CAxMListCtrl : public CListCtrl
{
// Construction
public:
	CAxMListCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAxMListCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAxMListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CAxMListCtrl)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg LRESULT OnSetMessageHandler(WPARAM, LPARAM);
	afx_msg LRESULT OnGetMessageHandler(WPARAM, LPARAM);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

protected:
	COleDataSource m_COleDataSource;
	CWnd*		m_messageHandler;
	LRESULT		Notify(UINT msg, WPARAM wParam, LPARAM lParam);
	void		OnDrag(CPoint point, int ListIndex);

public:
	void		SetMessageHandler(CWnd* wnd);
	CWnd*		GetMessageHandler() const { return m_messageHandler; }
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AXMLISTCTRL_H__5D762227_7F37_4A3E_81EF_A2B6BCFB1C08__INCLUDED_)
