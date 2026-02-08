#if !defined(AFX_AXMSTATICCTRL_H__2C9F5E6F_6E91_4C90_BC9E_977B0E3EDB40__INCLUDED_)
#define AFX_AXMSTATICCTRL_H__2C9F5E6F_6E91_4C90_BC9E_977B0E3EDB40__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AxMStaticCtrl.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CAxMStaticCtrl window

class AFX_EXT_CLASS CAxMStaticCtrl : public CStatic
{
// Construction
public:
	CAxMStaticCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAxMStaticCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAxMStaticCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CAxMStaticCtrl)
	afx_msg LRESULT OnSetMessageHandler(WPARAM, LPARAM);
	afx_msg LRESULT OnGetMessageHandler(WPARAM, LPARAM);
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	CBrush	 m_brush;
	BOOL	 m_transparent;

	COLORREF m_textColor, m_textColorHover;
	COLORREF m_bgColor;
	BOOL	 m_mouseOverControl;
	UINT	 m_bgColorPercent;
	UINT	 m_bgShadeID;
	void	 Refresh() { Invalidate(); }
protected:
	CWnd*	 m_messageHandler;
	LRESULT  Notify(UINT msg, WPARAM wParam = 0, LPARAM lParam = 0);
public:
	COLORREF GetTextColor() const { return m_textColor; }
	void	 SetTextColor(COLORREF color) { if (m_textColor != color) { m_textColor = color; Refresh();} }
	COLORREF GetTextColorHover() const { return m_textColorHover; }
	void	 SetTextColorHover(COLORREF color) { if (m_textColorHover != color) { m_textColorHover = color; Refresh();} }
	COLORREF GetBgColor() const { return m_bgColor; }
	void	 SetBgColor(COLORREF color) { if (m_bgColor != color) { m_bgColor = color; Refresh();} }

	UINT	 GetBgColorPercent() const { return m_bgColorPercent; }
	void	 SetBgColorPercent(UINT percent) { if (m_bgColorPercent != percent) { m_bgColorPercent = percent; Refresh();} }
	UINT	 GetBgShade() const { return m_bgShadeID; }
	void	 SetBgShade(UINT shade) { if (m_bgShadeID != shade) { m_bgShadeID = shade; Refresh();} }

	void	 SetMessageHandler(CWnd* wnd) { m_messageHandler = wnd; }
	CWnd*    GetMessageHandler() const { return m_messageHandler; }
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AXMSTATICCTRL_H__2C9F5E6F_6E91_4C90_BC9E_977B0E3EDB40__INCLUDED_)
