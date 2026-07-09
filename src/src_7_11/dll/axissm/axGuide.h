#if !defined(AFX_AXGUIDE_H__26F01F18_0E9A_4DA5_B2E0_744C63AAA5F1__INCLUDED_)
#define AFX_AXGUIDE_H__26F01F18_0E9A_4DA5_B2E0_744C63AAA5F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// axGuide.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAxGuide window

class AFX_EXT_CLASS CAxGuide : public CWnd
{
// Construction
public:
	CAxGuide(int wndPOS = 0, CString rootDir = _T(""));

// Attributes
private:
	CWnd*		m_pWnd;
	CFont		m_font;

	CString		m_text;

	COLORREF	m_textRGB;
	COLORREF	m_bkRGB;
	COLORREF	m_frameRGB;

	int		m_height;
	CSize		m_fontSize;
	CString		m_fontName;

	int		m_wndPOS;

	//enum _wndPOS {wndCENTER = 0, wndTOP, wndBOTTOM } m_wndPOS;

	CBitmap		m_bitmap;
	CSize		m_szBitmap;
// Operations
public:
	BOOL	Create(CWnd *pWnd);
	void	SetGuide(CString guide, CWnd* owner = NULL);
	void	SetPos(int pos);

	void	SetTime(int interval);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAxGuide)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAxGuide();

	// Generated message map functions
protected:
	//{{AFX_MSG(CAxGuide)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AXGUIDE_H__26F01F18_0E9A_4DA5_B2E0_744C63AAA5F1__INCLUDED_)
