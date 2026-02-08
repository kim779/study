#if !defined(AFX_POWLABEL_H__0C57797A_C92B_4FC1_AE01_D32417A12B95__INCLUDED_)
#define AFX_POWLABEL_H__0C57797A_C92B_4FC1_AE01_D32417A12B95__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PowLabel.h : header file
//

#define	NM_LINKCLICK	WM_APP + 0x200
#define	WM_CLICKLABEL	WM_USER+4343

/////////////////////////////////////////////////////////////////////////////
// CPowLabel window

class CPowLabel : public CStatic
{
// Construction
public:
	BOOL	m_bSignCheck;
	int		m_nSign;
	CWnd*	m_pWndSign;
	static enum FlashType {None, Text, Background };
	static enum Type3D { Raised, Sunken};
	static enum BackFillMode { Normal, Gradient };

	CPowLabel();
	virtual ~CPowLabel();

	virtual CPowLabel& SetBorderColor(COLORREF crBorderParm, int nWidth);
	virtual CPowLabel& SetBkColor(COLORREF crBkgnd, COLORREF crBkgndHigh = 0, BackFillMode mode = Normal);
	virtual CPowLabel& SetTextColor(COLORREF crText);
	virtual CPowLabel& SetText(const CString& strText);
	virtual CPowLabel& SetFontBold(BOOL bBold);
	virtual CPowLabel& SetFontName(const CString& strFont, BYTE byCharSet = ANSI_CHARSET);
	virtual CPowLabel& SetFontUnderline(BOOL bSet);
	virtual CPowLabel& SetFontItalic(BOOL bSet);
	virtual CPowLabel& SetFontSize(int nSize);
	virtual CPowLabel& SetSunken(BOOL bSet);
	virtual CPowLabel& SetBorder(BOOL bSet);
	virtual CPowLabel& SetTransparent(BOOL bSet);
	virtual CPowLabel& FlashText(BOOL bActivate);
	virtual CPowLabel& FlashBackground(BOOL bActivate);
	virtual CPowLabel& SetLink(BOOL bLink,BOOL bNotifyParent);
	virtual CPowLabel& SetLinkCursor(HCURSOR hCursor);
	virtual CPowLabel& SetFont3D(BOOL bSet,Type3D type=Raised);
	virtual CPowLabel& SetRotationAngle(UINT nAngle,BOOL bRotation);
	virtual CPowLabel& SetText3DHiliteColor(COLORREF cr3DHiliteColor);


	HBRUSH GetBrush()					{ return m_hwndBrush; }

	void OnDraw(CDC* pDC);							// Separate from OnPaint Func. - SeoJeong - 2005/07/10
	void RedrawLabel(CDC* pDC=NULL);
	virtual void GetText(CString& strText) const;	// Do Not Use GetWindowText Func. Use this GetText Func. - SeoJeong - 2005/07/10
	

protected:
	BOOL		m_bUseHandCursor;		// Hand Cursor Use(default false)
	HCURSOR		m_hHandCursor;			// Hand Cursor Handle
public:
	void		SetUseHandCursor(BOOL bUse)	{ m_bUseHandCursor = bUse;}
	void		LoadLibHandCursor();

// Attributes
protected:
	CString			m_strText;			// Text for this PowLabel
	BOOL			m_bFirstDraw;
	
	COLORREF		m_crText;
	COLORREF		m_cr3DHiliteColor;
	COLORREF		m_crBorder;
	HBRUSH			m_hwndBrush;
	HBRUSH			m_hBackBrush;
	LOGFONT			m_lf;
	CFont			m_font;
	BOOL			m_bState;
	BOOL			m_bTimer;
	BOOL			m_bLink;
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

	BOOL			m_bBorderColor;
	int				m_iBorderWidth;

// Operations
protected:
	void			UpdateSurface();
	void			ReconstructFont();
	void			DrawGradientFill(CDC* pDC, CRect* pRect, COLORREF crStart, COLORREF crEnd, int nSegments);


// Etc
public:
	int				m_nStyle;
	BOOL			SignDraw(CWnd *pWnd, int nStyle);

	
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPowLabel)
	protected:
	virtual void PreSubclassWindow();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

	
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPowLabel)
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnSysColorChange();
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_POWLABEL_H__0C57797A_C92B_4FC1_AE01_D32417A12B95__INCLUDED_)
