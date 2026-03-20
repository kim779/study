#pragma once
// NButton.h : header file
//
// mode
#define	MODE_3D		1
#define	MODE_FLAT	2
#define	COLOR_DEFAULT	RGB(212, 212, 212)

#define COLOR_LT	RGB(171, 168, 153)
#define	COLOR_RB	RGB(115, 113, 100)
//#define WM_XMSG			WM_USER+9999

/////////////////////////////////////////////////////////////////////////////
// CNButton window

class CNButton : public CWnd
{
// Construction
public:
	CNButton(UINT id, COLORREF color = COLOR_DEFAULT, int mode = MODE_3D, bool defaultColor = true, BOOL bCheck = FALSE, BOOL bToggle = FALSE, BOOL bRound = FALSE, bool bMask = false);
	virtual ~CNButton();

// Attributes
protected:
	BOOL	m_bText;
	CString	m_sText;
	
	bool	m_imgMode;
	bool	m_over;
	bool	m_push;
	bool	m_expand;
	bool	m_defaultColor;
	bool	m_bMask;
	int	m_mode;
	UINT	m_id;

	COLORREF	m_bk;
	COLORREF	m_overbk;
	std::unique_ptr<CToolTipCtrl>	m_ToolTip;

	CBitmap	m_Icool;
	CBitmap	m_Iover;
	CBitmap	m_Idown;
	HRGN	m_hRgn;

	CBitmap	m_IFull;
	BOOL	m_bFullImage;

	BOOL	m_bToggle;	// check 속성을 가지며, check시에 image를 toggle한다.
	BOOL	m_bRound;	// 버튼을 round처리한다.
	BOOL	m_bCheck;	// check 형태로 한다.
	BOOL	m_bChecked;	// 내부 reserved

	BOOL	m_bCBtnMsg;

	CFont	*m_pFont;

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
	CFont*	GetFont();
	void	SetFont(CFont *pFont);
	void	SetText(CString sText);
	void	SetCButtomMsg(BOOL bTF);
	void	Setcheck(BOOL bCheck = TRUE);
	BOOL	GetCheck();
	bool	DrawFullImg(CDC *pDC);
	void	set_Image(CString fullImage);
	CSize	get_ImgSize();
	void	set_Image(UINT, UINT, UINT);
	void	set_Image(CString, CString, CString);
	void	set_Bk(COLORREF color) { m_bk = color; }
	void	set_Expand(bool expand) { m_expand = expand; }
	void	set_Color(COLORREF over) { m_overbk = over; }
	void	set_ToolTipText(CString text, BOOL bActivate = TRUE);
	void	DrawBitmapByMask(CDC *pDC, CBitmap *pBitmap, CPoint pos, COLORREF maskcolor, CRect bmpRc = CRect(0, 0, 0, 0));
	void	change_img(CString cool, CString over, CString down);

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