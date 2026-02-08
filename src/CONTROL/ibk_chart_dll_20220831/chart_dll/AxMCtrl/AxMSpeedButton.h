#if !defined(AFX_AXMSPEEDBUTTON_H__55C5C0E0_0536_409A_9BF4_50B291799826__INCLUDED_)
#define AFX_AXMSPEEDBUTTON_H__55C5C0E0_0536_409A_9BF4_50B291799826__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AxMSpeedButton.h : header file
//

class CAxMToolTip;
/////////////////////////////////////////////////////////////////////////////
// CAxMSpeedButton window

class AFX_EXT_CLASS CAxMSpeedButton : public CWnd
{
// Construction
public:
	CAxMSpeedButton();
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAxMSpeedButton)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAxMSpeedButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CAxMSpeedButton)
	afx_msg LRESULT OnSetMessageHandler(WPARAM, LPARAM);
	afx_msg LRESULT OnGetMessageHandler(WPARAM, LPARAM);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	CString m_bitmapFileName;
	CString m_hoverBitmapPostfix, m_downBitmapPostfix;
	BOOL m_down, m_downState;
	BOOL m_check;
	BOOL m_hover;
	HBITMAP m_bmp, m_bmpHover, m_bmpDown;
	BOOL m_stretchDraw, m_textDraw;
	UINT m_bmWidth, m_bmHeight;
	CFont* m_font;
	CAxMToolTip* m_toolTip;

	void	ClearBitmaps();
	CString	GetEtcFileName(CString bmpFile, CString postfix);
protected:
	CWnd* m_messageHandler;
	virtual void DrawButton(CDC* dc);
	virtual LRESULT Notify(UINT msg, WPARAM wParam = 0, LPARAM lParam = 0);

	void ShowToolTip();
	void HideToolTip();
public:
	BOOL	Create(LPCTSTR szText, LPRECT lpRect, CWnd* parent, UINT id);
	void	MakeTooltip(CString sTip = "");
	void	SetHoverBitmapPostfix(LPCTSTR postfix);
	void	SetDownBitmapPostfix(LPCTSTR postfix);
	void	LoadBitmap(LPCTSTR bmpFile);

	void	SetCheckStyle(BOOL bCheckStyle = TRUE);
	void	StretchMode(BOOL bStretch);

	CWnd*	GetMessageHandler() const { return m_messageHandler; }
	void	SetMessageHandler(CWnd* wnd) { m_messageHandler = wnd; }

	BOOL	GetDown();
	void	SetDown(BOOL bDown);

	void	SetFont(CFont* pFont) { 
		m_font = pFont;
	}

	void	SetTextDraw(BOOL bDrawText = FALSE);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AXMSPEEDBUTTON_H__55C5C0E0_0536_409A_9BF4_50B291799826__INCLUDED_)
