#pragma once


// NMButton
#define	MODE_3D		1
#define	MODE_FLAT	2
#define	COLOR_DEFAULT	RGB(212, 212, 212)

#define COLOR_LT	RGB(171, 168, 153)
#define	COLOR_RB	RGB(115, 113, 100)

class NMButton : public CWnd
{
public:
	bool	m_imgMode;
	bool	m_over;
	bool	m_push;
	bool	m_expand;
	bool	m_defaultColor;
	bool	m_bMasking;
	bool	m_bMaskDraw;
	int	m_mode;
	CRect m_rcMasking;
	UINT	m_id;

	COLORREF	m_bkMask;
	COLORREF	m_bk;
	COLORREF	m_overbk;
	CToolTipCtrl*	m_ToolTip;

	CBitmap		m_Icool;
	CBitmap		m_Iover;
	CBitmap		m_Idown;

	void	SetItemFont(CFont* pfont) {m_font = pfont;};
	CString		m_text;
	CFont*	m_font;
	COLORREF	m_fcolor;

	void	SetMaskDraw(bool bmask, COLORREF backColr);
	CSize	get_ImgSize();
	void  set_BkMaskSize(CRect size);
	void	set_Image(UINT, UINT, UINT);
	void	set_Image(CString, CString, CString);
	void	set_Bk(COLORREF color) { m_bk = color; }
	void	set_Expand(bool expand) { m_expand = expand; }
	void	set_Color(COLORREF over) { m_overbk = over; }
	void	set_ToolTipText(CString text, BOOL bActivate = TRUE);
	void	transparentBlt(CDC *pDC, CBitmap *pBitmap, int x, int y, int cx, int cy);
	void	change_img(CString cool, CString over, CString down);
	int		DrawBitmapByMask(CDC* pDC,int maskcolor);

	void	InitToolTip();
	bool	DrawImg(CDC* pDC);
public:
	NMButton();
	NMButton(UINT id, COLORREF color = COLOR_DEFAULT, int mode = MODE_3D, bool defaultColor = true);
	virtual ~NMButton();

protected:
	DECLARE_MESSAGE_MAP()
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	afx_msg void OnPaint();
};


