#if !defined(AFX_FXCOMBOBOX_H__CC9CC1FA_D03A_4AF1_8436_CAD8B4707EEF__INCLUDED_)
#define AFX_FXCOMBOBOX_H__CC9CC1FA_D03A_4AF1_8436_CAD8B4707EEF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// fxComboBox1.h : header file
//
#define BTN_DF 0
#define BTN_DN 1
#define BTN_EN 2
#define BTN_XN 3
/////////////////////////////////////////////////////////////////////////////
// CfxComboBox window

class CfxComboBox : public CComboBox
{
// Construction
public:
	CfxComboBox();
// Attributes
public:
	CFont		*m_pFont;
	CWnd		*m_pParent;

	COLORREF	m_cbRGB;
	COLORREF	m_csRGB;
	
	BOOL		m_bEnable;
	HBITMAP		m_hBmp[4];
	HBITMAP		m_hBmpLoad;
	bool		m_bImgBtn;
	CRect		m_rc, m_rcBtn, m_rcText;
	bool		m_bCursorOnButton;
	bool		m_bDropdownClick;
// Operations
public:
	BOOL		Create(int dwStyle, CRect rect, CWnd* parent, UINT nID);
	void		SetFont(CFont* pFont, bool redraw = true);
	void		SetPalette(COLORREF clrFg, COLORREF clrBg);
	void		SetParent(CWnd * parent) {m_pParent = parent;}
	void		LoadImg(CString path);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CfxComboBox)
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CfxComboBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CfxComboBox)
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg BOOL OnDropdown();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnEnable(BOOL bEnable);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg BOOL OnCbnCloseup();
	afx_msg void OnSize(UINT nType, int cx, int cy);
//	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
};
typedef struct _IconComboBox {
	HICON	m_hIcon;		// Icon Handle
	int		m_nIconId;	// Icon Resource Id
	CString m_strItem;
} _IconComboBoxData;
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FXCOMBOBOX_H__CC9CC1FA_D03A_4AF1_8436_CAD8B4707EEF__INCLUDED_)
