#if !defined(AFX_DRLABEL_H__A4EABEC5_2E8C_11D1_B79F_00805F9ECE10__INCLUDED_)
#define AFX_DRLABEL_H__A4EABEC5_2E8C_11D1_B79F_00805F9ECE10__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// Label.h : header file
//

#define	NM_LINKCLICK	WM_APP + 0x200

/////////////////////////////////////////////////////////////////////////////
// CDrLabel window

class CDrLabel : public CStatic
{
// Construction
public:


	static enum LinkStyle { LinkNone, HyperLink, MailLink };
	static enum FlashType {None, Text, Background };
	static enum Type3D { Raised, Sunken};
	static enum BackFillMode { Normal, Gradient };
	static enum AlignType {_LEFT,_CENTER,_RIGHT};

	CDrLabel();
	virtual CDrLabel& SetBkColor(COLORREF crBkgnd, COLORREF crBkgndHigh = 0, BackFillMode mode = Normal);
	virtual CDrLabel& SetTextColor(COLORREF crText);
	virtual CDrLabel& SetText(const CString& strText);
	virtual CDrLabel& SetFontBold(BOOL bBold);
	virtual CDrLabel& SetFontName(const CString& strFont, BYTE byCharSet = ANSI_CHARSET);
	virtual CDrLabel& SetFontUnderline(BOOL bSet);
	virtual CDrLabel& SetFontItalic(BOOL bSet);
	virtual CDrLabel& SetFontSize(int nSize);
	virtual CDrLabel& SetSunken(BOOL bSet);
	virtual CDrLabel& SetBorder(BOOL bSet);
	virtual CDrLabel& SetTransparent(BOOL bSet);
	virtual CDrLabel& FlashText(BOOL bActivate);
	virtual CDrLabel& FlashBackground(BOOL bActivate);
	virtual CDrLabel& SetLink(BOOL bLink,BOOL bNotifyParent);
	virtual CDrLabel& SetLinkCursor(HCURSOR hCursor);
	virtual CDrLabel& SetFont3D(BOOL bSet,Type3D type=Raised);
	virtual CDrLabel& SetRotationAngle(UINT nAngle,BOOL bRotation);
	virtual CDrLabel& SetText3DHiliteColor(COLORREF cr3DHiliteColor);
	virtual CDrLabel& SetFont(LOGFONT lf);
	virtual CDrLabel& SetMailLink(BOOL bEnable, BOOL bNotifyParent);
	virtual CDrLabel& SetHyperLink(const CString& sLink);
	virtual CDrLabel& SetAlign(AlignType AlignTypes = _LEFT);

// Attributes
public:
	BOOL	m_bStandAlone;
protected:
	void UpdateSurface();
	void ReconstructFont();
	void DrawGradientFill(CDC* pDC, CRect* pRect, COLORREF crStart, COLORREF crEnd, int nSegments);
	COLORREF		m_crText;
	COLORREF		m_cr3DHiliteColor;
	HBRUSH			m_hwndBrush;
	HBRUSH			m_hBackBrush;
	LOGFONT			m_lf;
	CFont			m_font;
	BOOL			m_bState;
	BOOL			m_bTimer;
	LinkStyle		m_Link;
	BOOL			m_bTransparent;
	BOOL			m_bFont3d;
	BOOL			m_bToolTips;
	BOOL			m_bNotifyParent;
	BOOL			m_bRotation;
	FlashType		m_Type;
	HCURSOR			m_hCursor;
	Type3D			m_3dType;
	BackFillMode	m_fillmode;
	COLORREF		m_crHiColor;
	COLORREF		m_crLoColor;
	CString			m_sLink;
    AlignType	    m_nAlignType;
	// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDrLabel)
	protected:
	virtual void PreSubclassWindow();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDrLabel();

	// Generated message map functions
protected:
	//{{AFX_MSG(CDrLabel)
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnSysColorChange();
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRLABEL_H__A4EABEC5_2E8C_11D1_B79F_00805F9ECE10__INCLUDED_)
