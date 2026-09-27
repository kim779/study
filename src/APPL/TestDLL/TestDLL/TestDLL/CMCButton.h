#pragma once
#include <afxwin.h>

// Return values
#ifndef	BTNST_OK
#define	BTNST_OK					0
#endif
#ifndef	BTNST_INVALIDRESOURCE
#define	BTNST_INVALIDRESOURCE				1
#endif
#ifndef	BTNST_FAILEDMASK
#define	BTNST_FAILEDMASK				2
#endif
#ifndef	BTNST_INVALIDINDEX
#define	BTNST_INVALIDINDEX				3
#endif
#ifndef	BTNST_INVALIDALIGN
#define	BTNST_INVALIDALIGN				4
#endif
#ifndef	BTNST_BADPARAM
#define	BTNST_BADPARAM					5
#endif
#ifndef	BTNST_INVALIDPRESSEDSTYLE
#define	BTNST_INVALIDPRESSEDSTYLE			6
#endif

// Dummy identifier for grayscale icon
#ifndef	BTNST_AUTO_GRAY
#define	BTNST_AUTO_GRAY					(HICON)(0xffffffff - 1L)
#endif
// Dummy identifier for 15% darker icon
#ifndef	BTNST_AUTO_DARKER
#define	BTNST_AUTO_DARKER				(HICON)(0xffffffff - 2L)
#endif

class CMCButton :
    public CButton
{
public:
	CMCButton();

	CString m_slog{};

public:
	enum {
		ST_ALIGN_HORIZ = 0,			// Icon/bitmap on the left, text on the right
		ST_ALIGN_VERT,				// Icon/bitmap on the top, text on the bottom
		ST_ALIGN_HORIZ_RIGHT,			// Icon/bitmap on the right, text on the left
		ST_ALIGN_OVERLAP			// Icon/bitmap on the same space as text
	};

	enum {
		BTNST_COLOR_BK_IN = 0,		// Background color when mouse is INside
		BTNST_COLOR_FG_IN,			// Text color when mouse is INside
		BTNST_COLOR_BK_OUT,			// Background color when mouse is OUTside
		BTNST_COLOR_FG_OUT,			// Text color when mouse is OUTside
		BTNST_COLOR_BK_FOCUS,			// Background color when the button is focused
		BTNST_COLOR_FG_FOCUS,			// Text color when the button is focused

		BTNST_MAX_COLORS
	};

	enum {
		BTNST_PRESSED_LEFTRIGHT = 0,		// Pressed style from left to right (as usual)
		BTNST_PRESSED_TOPBOTTOM			// Pressed style from top to bottom
	};

	DWORD SetDefaultColors(BOOL bRepaint = TRUE);
	DWORD SetColor(BYTE byColorIndex, COLORREF crColor, BOOL bRepaint = TRUE);
	DWORD GetColor(BYTE byColorIndex, COLORREF* crpColor);
	DWORD OffsetColor(BYTE byColorIndex, short shOffset, BOOL bRepaint = TRUE);
	
	DWORD SetCheck(int nCheck, BOOL bRepaint = TRUE);
	 int GetCheck();
	
	DWORD SetURL(LPCTSTR lpszURL = NULL);
	void DrawTransparent(BOOL bRepaint = FALSE);
	DWORD SetBk(CDC* pDC);

	BOOL GetDefault();
	DWORD SetAlwaysTrack(BOOL bAlwaysTrack = TRUE);

	void SetTooltipText(int nText, BOOL bActivate = TRUE);
	void SetTooltipText(LPCTSTR lpszText, BOOL bActivate = TRUE);
	void ActivateTooltip(BOOL bEnable = TRUE);
	DWORD EnableBalloonTooltip();

	DWORD SetBtnCursor(int nCursorId = NULL, BOOL bRepaint = TRUE);

	DWORD SetFlat(BOOL bFlat = TRUE, BOOL bRepaint = TRUE);
	DWORD SetAlign(BYTE byAlign, BOOL bRepaint = TRUE);
	DWORD SetPressedStyle(BYTE byStyle, BOOL bRepaint = TRUE);

	DWORD DrawBorder(BOOL bDrawBorder = TRUE, BOOL bRepaint = TRUE);
	DWORD DrawFlatFocus(BOOL bDrawFlatFocus, BOOL bRepaint = TRUE);

	DWORD SetIcon(int nIconIn, int nCxDesiredIn, int nCyDesiredIn, int nIconOut = NULL, int nCxDesiredOut = 0, int nCyDesiredOut = 0);
	DWORD SetIcon(int nIconIn, int nIconOut = NULL);
	DWORD SetIcon(HICON hIconIn, HICON hIconOut = NULL);

	DWORD SetBitmaps(int nBitmapIn, COLORREF crTransColorIn, int nBitmapOut = NULL, COLORREF crTransColorOut = 0);
	DWORD SetBitmaps(HBITMAP hBitmapIn, COLORREF crTransColorIn, HBITMAP hBitmapOut = NULL, COLORREF crTransColorOut = 0);
	DWORD SetBitmaps(CString sBitmapIn, COLORREF crTransColorIn, CString sBitmapOut, COLORREF crTransColorOut);

	void SizeToContent();

	//testlogin
	COLORREF m_clrEnableCheck;
	BOOL	m_bEnableRGBColor;
	void SetEnableCheckRGBColor(COLORREF clr)
	{
		m_clrEnableCheck = clr;
		m_bEnableRGBColor = true;
	}

	static short GetVersionI() { return 39; }
	static LPCTSTR GetVersionC() { return (LPCTSTR)_T("3.9"); }

	BOOL	m_bShowDisabledBitmap;
	POINT	m_ptImageOrg;
	POINT	m_ptPressedOffset;
    
    virtual void PreSubclassWindow();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	DECLARE_MESSAGE_MAP()
	//afx_msg void OnPaint();
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	virtual BOOL PreTranslateMessage(MSG* pMsg);


	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSysColorChange();
	afx_msg void OnBnClicked();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnEnable(BOOL bEnable);
	afx_msg void OnCancelMode();
	afx_msg UINT OnGetDlgCode();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg HBRUSH CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);

#ifdef	BTNST_USE_BCMENU
	afx_msg LRESULT OnMenuChar(UINT nChar, UINT nFlags, CMenu* pMenu);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
#endif

	HICON CreateGrayscaleIcon(HICON hIcon);
	HICON CreateDarkerIcon(HICON hIcon);
	HBITMAP CreateGrayscaleBitmap(HBITMAP hBitmap, DWORD dwWidth, DWORD dwHeight, COLORREF crTrans);
	HBITMAP CreateDarkerBitmap(HBITMAP hBitmap, DWORD dwWidth, DWORD dwHeight, COLORREF crTrans);
	COLORREF DarkenColor(COLORREF crColor, double dFactor);
	virtual DWORD OnDrawBackground(CDC* pDC, CRect* pRect);
	virtual DWORD OnDrawBorder(CDC* pDC, CRect* pRect);

	BOOL		m_bIsFlat;		// Is a flat button?
	BOOL		m_bMouseOnButton;	// Is mouse over the button?
	BOOL		m_bDrawTransparent;	// Draw transparent?
	BOOL		m_bIsPressed;		// Is button pressed?
	BOOL		m_bIsFocused;		// Is button focused?
	BOOL		m_bIsDisabled;		// Is button disabled?
	BOOL		m_bIsDefault;		// Is default button?
	BOOL		m_bIsCheckBox;		// Is the button a checkbox?
	BYTE		m_byAlign;		// Align mode
	BOOL		m_bDrawBorder;		// Draw border?
	BOOL		m_bDrawFlatFocus;	// Draw focus rectangle for flat button?
	COLORREF	m_crColors[BTNST_MAX_COLORS];	// Colors to be used
	HWND		m_hParentWndMenu;	// Handle to window for menu selection
	BOOL		m_bMenuDisplayed;	// Is menu displayed ?

private:
	LRESULT OnSetCheck(WPARAM wParam, LPARAM lParam);
	LRESULT OnGetCheck(WPARAM wParam, LPARAM lParam);
	LRESULT OnSetStyle(WPARAM wParam, LPARAM lParam);
	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);

	void CancelHover();
	void FreeResources(BOOL bCheckForNULL = TRUE);
	void PrepareImageRect(BOOL bHasTitle, RECT* rpItem, CRect* rpTitle, BOOL bIsPressed, DWORD dwWidth, DWORD dwHeight, CRect* rpImage);
	HBITMAP CreateBitmapMask(HBITMAP hSourceBitmap, DWORD dwWidth, DWORD dwHeight, COLORREF crTransColor);
	virtual void DrawTheIcon(CDC* pDC, BOOL bHasTitle, RECT* rpItem, CRect* rpCaption, BOOL bIsPressed, BOOL bIsDisabled);
	virtual void DrawTheBitmap(CDC* pDC, BOOL bHasTitle, RECT* rpItem, CRect* rpCaption, BOOL bIsPressed, BOOL bIsDisabled);
	virtual void DrawTheText(CDC* pDC, LPCTSTR lpszText, RECT* rpItem, CRect* rpCaption, BOOL bIsPressed, BOOL bIsDisabled);
	void PaintBk(CDC* pDC);

	void InitToolTip();
	void AdjustFontSize(CWnd* pWnd, CFont& font, LPCTSTR fontName, int weight = FW_BOLD);

	HCURSOR		m_hCursor;			// Handle to cursor
	CToolTipCtrl m_ToolTip;			// Tooltip

	CDC			m_dcBk;
	CBitmap		m_bmpBk;
	CBitmap* m_pbmpOldBk;
	UINT		m_nTypeStyle;		// Button style
	int			m_nCheck{};			// Current value for checkbox
	BOOL		m_bAlwaysTrack;		// Always hilight button?
	DWORD		m_dwToolTipStyle;	// Style of tooltip control

	TCHAR		m_szURL[_MAX_PATH];	// URL to open when clicked

	CFont		m_Bfont;		// hover font setting for hbg
	CFont		m_sfont;		

#pragma pack(1)
	typedef struct _STRUCT_ICONS
	{
		HICON		hIcon;			// Handle to icon
		DWORD		dwWidth;		// Width of icon
		DWORD		dwHeight;		// Height of icon
	} STRUCT_ICONS;
#pragma pack()

#pragma pack(1)
	typedef struct _STRUCT_BITMAPS
	{
		HBITMAP		hBitmap;		// Handle to bitmap
		DWORD		dwWidth;		// Width of bitmap
		DWORD		dwHeight;		// Height of bitmap
		HBITMAP		hMask;			// Handle to mask bitmap
		COLORREF	crTransparent;	// Transparent color
	} STRUCT_BITMAPS;
#pragma pack()

#pragma pack(1)
	typedef struct _STRUCT_CALLBACK
	{
		HWND		hWnd;			// Handle to window
		UINT		nMessage;		// Message identifier
		WPARAM		wParam;
		LPARAM		lParam;
	} STRUCT_CALLBACK;
#pragma pack()

	STRUCT_ICONS	m_csIcons[2];
	STRUCT_BITMAPS	m_csBitmaps[2];

	STRUCT_CALLBACK	m_csCallbacks;

#ifdef	BTNST_USE_SOUND
#pragma pack(1)
	typedef struct _STRUCT_SOUND
	{
		TCHAR		szSound[_MAX_PATH];
		LPCTSTR		lpszSound;
		HMODULE		hMod;
		DWORD		dwFlags;
	} STRUCT_SOUND;
#pragma pack()

	STRUCT_SOUND	m_csSounds[2];	// Index 0 = Over	1 = Clicked
#endif

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

