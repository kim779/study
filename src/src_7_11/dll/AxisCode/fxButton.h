#if !defined(AFX_FXBUTTON_H__80B57896_1CAC_4B2A_839A_08642ABEF7F4__INCLUDED_)
#define AFX_FXBUTTON_H__80B57896_1CAC_4B2A_839A_08642ABEF7F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AxBitmap.h"

#define BMP_DF 0
#define BMP_EN 1
#define BMP_DN 2
#define BMP_CN 3
#define BMP_MAX 4

class CfxButton : public CButton
{
public:
	CfxButton(CFont* pFont = NULL);
	virtual ~CfxButton();
	
	//{{AFX_VIRTUAL(CfxButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

public:
	BOOL	Create(LPCTSTR lpszCaption, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	void	Init(DWORD style, CString text = _T(""), CString images = _T(""),
				COLORREF clrBACK = GetSysColor(COLOR_3DFACE), 
				COLORREF clrFORE = GetSysColor(COLOR_BTNTEXT),
				COLORREF clrTOGGLE = GetSysColor(COLOR_BTNTEXT));
	void	SetStyle(DWORD style, bool redraw = true);
	void	SetText(CString text, bool redraw = true);
	void	SetBitmap(CString path, bool redraw = true);
	void	SetBkColor(COLORREF clrBACK, bool redraw = true);
	void	SetFgColor(COLORREF clrFORE, bool redraw = true);
	void	SetDisableBkColor(COLORREF clrDISBACK, bool redraw = true);
	void	SetDisableFgColor(COLORREF clrDISFORE, bool redraw = true);
	void	SetToggleColor(COLORREF clrTOGGLE, bool redraw = true);
	void	SetFont(CFont* pFont, bool redraw = true);
	void	SetToggleStatus(bool bPress, bool redraw = true);
	bool	GetToggleStatus() { return m_bPress;	}
	void	ModifyBtnStyle(DWORD remove, DWORD add, bool redraw = true);
	CString GetText(){ return m_text;}
	void	SetUser();
	void	LoadImg(CString path);
	void	SetSelected(bool bFlag);
	void	SetBold(bool bFlag);
	DWORD SetColor(BYTE byColorIndex, COLORREF crColor, BOOL bRepaint = TRUE);
	DWORD SetBtnCursor(int nCursorId = NULL, BOOL bRepaint = TRUE);
private:
	void	drawGradRect(CDC *pDC, CRect drawRC, COLORREF sColor, COLORREF eColor);

protected:
	CBitmap* m_bitmap;
	COLORREF m_clrBACK;
	COLORREF m_clrFORE;
	COLORREF m_clrTOGGLE;
	COLORREF m_clrDISBACK;
	COLORREF m_clrDISFORE;
	DWORD	m_style;
	CString	m_text;
	CFont*	m_pFont;

	bool	m_bPushed;
	CWnd*	m_parent;
	bool	m_bPress;	// use toggle
	bool    m_bUser;
	bool		m_bImgBtn;
	HBITMAP		m_hBmp[7];
	HBITMAP		m_hBmpLoad;
	CAxBitmap	m_AxBmp[BMP_MAX];
	BOOL		m_bTracking;
	bool		m_bToogleSel;
	HCURSOR		m_hCursor;			// Handle to cursor

protected:
	//{{AFX_MSG(CfxButton)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMouseLeave();
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	LOGFONT getLogFont(int fsize, DWORD weight);
	afx_msg void OnDestroy();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FXBUTTON_H__80B57896_1CAC_4B2A_839A_08642ABEF7F4__INCLUDED_)
