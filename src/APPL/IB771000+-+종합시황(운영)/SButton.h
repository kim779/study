#pragma once
// SGButton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSButton window


#define	MODE_3D		1
#define	MODE_FLAT	2
#define	COLOR_DEFAULT	RGB(212, 212, 212)

#define COLOR_LT	RGB(171, 168, 153)
#define	COLOR_RB	RGB(115, 113, 100)

class CSButton : public CButton
{
// Construction
public:
	CSButton(UINT id, COLORREF color = COLOR_DEFAULT, int mode = MODE_3D, bool defaultColor = true);
	CSButton();
// Attributes
protected:
	bool	m_imgMode;
	bool	m_over;
	bool	m_push;
	bool	m_expand;
	bool	m_defaultColor;
	int	m_mode;
	UINT	m_id;

	COLORREF	m_bk;
	COLORREF	m_overbk;
	//CToolTipCtrl*	m_ToolTip;
	std::unique_ptr<CToolTipCtrl> m_ToolTip;

	CBitmap		m_Icool;
	CBitmap		m_Iover;
	CBitmap		m_Idown;
	HRGN		m_hRgn{};
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNButton)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	CSize	get_ImgSize();
	void	set_Image(UINT, UINT, UINT);
	void	set_Image(CString, CString, CString);
	void	set_Bk(COLORREF color) { m_bk = color; }
	void	set_Expand(bool expand) { m_expand = expand; }
	void	set_Color(COLORREF over) { m_overbk = over; }
	void	set_ToolTipText(CString text, BOOL bActivate = TRUE);

	void	change_img(CString cool, CString over, CString down);
	virtual ~CSButton();

	// Generated message map functions
protected:
	void	InitToolTip();
	bool	DrawImg(CDC* pDC);
	//{{AFX_MSG(CNButton)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

