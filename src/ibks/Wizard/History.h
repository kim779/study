#if !defined(AFX_HISTORY_H__F506606B_316C_4B9B_82D0_7FE452637846__INCLUDED_)
#define AFX_HISTORY_H__F506606B_316C_4B9B_82D0_7FE452637846__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// History.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHistory window

class CHistory : public CListBox
{
// Construction
public:
	CHistory(class CClient* client, CString items);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHistory)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	CFont		m_font;

private:
	class CGuard*	m_guard;
	class CClient*	m_client;

	CString		m_items;

	COLORREF	m_tRGB;
	COLORREF	m_pRGB;
	int		m_height;	// cell height

public:
	bool	GetItem(CString& item);
	void	FindItem(CString item, bool self);
	bool	OnKey(WPARAM wParam);
	void	OnWheel(int delta);

	void	MeasureItemX(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	void	DrawItemX(LPDRAWITEMSTRUCT lpDrawItemStruct);

public:
	virtual ~CHistory();

	// Generated message map functions
protected:
	//{{AFX_MSG(CHistory)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HISTORY_H__F506606B_316C_4B9B_82D0_7FE452637846__INCLUDED_)
