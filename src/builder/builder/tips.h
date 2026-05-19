#if !defined(AFX_TIPS_H__E748F401_98EF_11D3_83EE_00105A779B5A__INCLUDED_)
#define AFX_TIPS_H__E748F401_98EF_11D3_83EE_00105A779B5A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Tips.h : header file
//
#include <afxtempl.h>
/////////////////////////////////////////////////////////////////////////////
// CTips window

class CTips : public CWnd
{
// Construction
public:
	CTips();

// Attributes
public:
	CRect		m_currentRect;
private:
	int		m_key;
	LPCTSTR		m_ClassName;
	CFont		m_font;

	CString		m_text;
	bool		m_mouse;

	COLORREF	m_textRGB;
	COLORREF	m_bkRGB;
	COLORREF	m_frameRGB;

	CMap <int, int, CString, CString &> m_tips;

	int		m_width;
	int		m_height;
	CWnd		*m_client;

// Operations
public:
	void	RelayEvent(UINT msg, LPARAM lParam);
	void	RemoveTips();
	void	AddTips(int key, CString text)	{ m_tips.SetAt(key, text); }
	BOOL	Create(CWnd* client);
	CString Parser(CString &srcstr, CString substr);
	void	killMouse(int key);
	void	ShowTip(int key, CPoint mousePos);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTips)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTips();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTips)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TIPS_H__E748F401_98EF_11D3_83EE_00105A779B5A__INCLUDED_)
