/************************************************************************/
/* FILENAME:    AxMTabCtrl.h                                            */
/* DESCRIPTION: CAxMTabCtrl class definetion file.                      */
/* WRITTEN:     macho@2006.11.06                                        */
/*                                                                      */
/************************************************************************/


#ifndef __AXISMTAB_H__
#define __AXISMTAB_H__

enum
{
	BTC_NONE = 0,
	BTC_TABS = 1,
	BTC_ALL  = 2,
};

enum
{
	ETC_FLAT = 1, 
	ETC_COLOR = 2, // draw tabs with color
	ETC_SELECTION = 4, // highlight the selected tab
	ETC_GRADIENT = 8, // draw colors with a gradient
	ETC_BACKTABS = 16,
};


class AFX_EXT_CLASS CBaseTabCtrl : public CTabCtrl
{
// Construction
public:
	CBaseTabCtrl(int nType = BTC_NONE);
	BOOL EnableDraw(int nType = BTC_ALL);
	void SetBkgndColor(COLORREF color);

protected:
	COLORREF m_crBack;
	int m_nDrawType;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBaseTabCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBaseTabCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CBaseTabCtrl)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	virtual void DrawMainBorder(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void DrawItemBorder(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual COLORREF GetTabColor(BOOL bSelected = FALSE);
	virtual COLORREF GetTabTextColor(BOOL bSelected = FALSE);
	virtual void PreSubclassWindow();
public:
};

class AFX_EXT_CLASS CAxMTabCtrl: public CTabCtrl
{
public:
	CAxMTabCtrl(CWnd* parent);
	
	static void EnableCustomLook(BOOL bEnable = TRUE, DWORD dwStyle = ETC_FLAT | ETC_COLOR);
protected:
	static DWORD s_dwCustomLook;
public:
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAxMTabCtrl)
protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL
	// Implementation
public:
		
	virtual ~CAxMTabCtrl();
	
	// Generated message map functions
protected:
	// Generated message map functions
	//{{AFX_MSG(CAxMTabCtrl)
	afx_msg void OnPaint();
	afx_msg LRESULT OnSetMessageHandler(WPARAM, LPARAM);
	afx_msg LRESULT OnGetMessageHandler(WPARAM, LPARAM);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CWnd* m_messageHandler;
	CFont m_font;
	CWnd* m_parent;
	int	  m_oldTabIndex;

	int	 GetItemFromPoint(CPoint point, CRect& rc);
	LRESULT Notify(UINT msg, WPARAM wParam, LPARAM lParam);

public:
	void	SetFont(int pt, LPCTSTR lpszFontName);
	void	SetToolTipText(int index, LPCTSTR text);

	void	SetMessageHandler(CWnd* wnd) { m_messageHandler = wnd; }
	CWnd*	GetMessageHandler() const { return m_messageHandler; }

protected:
	int	m_bkColor;
	int	m_drawMode;
	HBITMAP	m_TabBMP[2];

	BOOL	IsImageReady();
	void	ClearImageObject();
	int	getBmpWidth(HBITMAP hBitmap);
	int	drawBmp(CDC *dc, CDC *memDC, CRect rc, HBITMAP hBitmap, int posX, int posTo = -1);
	void	drawImage(CDC* pDC);
public:
	enum { DM_DRAW, DM_IMAGE } ;
	void	SetTabImage(LPCTSTR sel, LPCTSTR back);
	void	SetDrawMode(int mode = DM_DRAW);
};

#endif // __XTAB_H__
