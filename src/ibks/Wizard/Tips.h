#if !defined(AFX_TIPS_H__2A2869AE_1A67_4EB6_A546_18EF8264B1FE__INCLUDED_)
#define AFX_TIPS_H__2A2869AE_1A67_4EB6_A546_18EF8264B1FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Tips.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTips window

class CTips : public CWnd
{
// Construction
public:
	CTips(CGuard* guard);

// Attributes
private:
	int		m_key;
	CFont		m_font;

	CString		m_text;
	bool		m_mouse;

	COLORREF	m_textRGB;
	CBrush		m_bkBrush;
	CBrush		m_frBrush;

	int		m_width;
	int		m_height;

	CPoint		m_point;
	int		m_works;
	CGuard*		m_guard;

// Operations
public:
	BOOL	Create(CWnd* pWnd);
	void	RelayEvent(CClient* client, UINT msg, LPARAM lParam);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTips)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTips();

	// Generated message map functions
protected:
	void	setTips();

	//{{AFX_MSG(CTips)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TIPS_H__2A2869AE_1A67_4EB6_A546_18EF8264B1FE__INCLUDED_)
