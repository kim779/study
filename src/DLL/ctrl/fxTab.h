#if !defined(AFX_FXTAB_H__838AE6F9_DCCE_4A84_8493_F3E21C36EF32__INCLUDED_)
#define AFX_FXTAB_H__838AE6F9_DCCE_4A84_8493_F3E21C36EF32__INCLUDED_

#include <afxtempl.h>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// fxTab.h : header file
//

//
//	style
//
#define	tsNORMAL	0x00000000
#define	tsIMAGE		0x00000001

/////////////////////////////////////////////////////////////////////////////
// CfxTab window

class AFX_EXT_CLASS CfxTab : public CTabCtrl
{
// Construction
public:
	CfxTab();

// Attributes
public:
	CFont		*m_pFont;
	COLORREF	m_bkColor, m_frColor, m_selectedbColor, m_selectedfColor, m_backColor;
	CBitmap		*m_bmpSelL, *m_bmpSelM, *m_bmpSelR, *m_bmpL, *m_bmpM, *m_bmpR, *m_bmpSpace;
	DWORD		m_style{};
	HBITMAP		m_pics[6];
	bool		m_selfLoad;
	int		m_textUpDown;

// Operations
public:
	BOOL	Create(CRect rect, CWnd* parent, UINT nID);

	void	SetFont(CFont* pFont, bool redraw = true);

	void	SetStyle(DWORD style, bool redraw);
	void	SetColor(COLORREF fcolor, COLORREF bcolor, COLORREF sfcolor, COLORREF sbcolor, COLORREF backColor);
	void	SetImage(HBITMAP hselectL, HBITMAP hSelectM, HBITMAP hSelectR, HBITMAP hL, HBITMAP hM, HBITMAP hR, HBITMAP hSpace);
	void	SetImage(CString imageDir, CString selectL, CString selectM, CString selectR, CString left, CString middle, CString right, CString space);
	void	LoadTabImage(CString rootDir, bool bTopTab = true);

protected:
	LOGFONT getLogFont(int fsize, DWORD weight);

	void	OnDraw(CDC *pDC);
	void	drawNormal(CDC *pDC);
	void	drawImage(CDC* pDC);
	void	draw2Rect(CDC *dc, CRect rc, COLORREF color1, COLORREF color2);
	void	DrawItem(LPDRAWITEMSTRUCT lpdis);
	LOGFONT setFont(int fsize, DWORD weight);
	void	draw3dRoundRect(CDC *dc, CRect rc, int align, int depth, COLORREF borderColor);
	int	bmpCopy(CDC *dc, CDC *memDC, int bmpidx, int posX, int posTo = -1, int height = -1);
	int	getBmpWidth(int bmpidx);
	void	ClearImage();
	CBitmap* CreateBmp( LPCTSTR sBMPFile/*, CPalette *pPal */);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CfxTab)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CfxTab();

	// Generated message map functions
protected:
	//{{AFX_MSG(CfxTab)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FXTAB_H__838AE6F9_DCCE_4A84_8493_F3E21C36EF32__INCLUDED_)
