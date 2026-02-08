//
//	Class:		CButtonST
//
//	Compiler:	Visual C++
//	Tested on:	Visual C++ 5.0
//				Visual C++ 6.0
//
//	Version:	See GetVersionC() or GetVersionI()
//
//	Created:	xx/xxxx/1998
//	Updated:	03/March/2003
//
//	Author:		Davide Calabro'		davide_calabro@yahoo.com
//									http://www.softechsoftware.it
//
//	Note:		Code for the PreSubclassWindow and OnSetStyle functions
//				has been taken from the COddButton class
//				published by Paolo Messina and Jerzy Kaczorowski
//
//	Disclaimer
//	----------
//	THIS SOFTWARE AND THE ACCOMPANYING FILES ARE DISTRIBUTED "AS IS" AND WITHOUT
//	ANY WARRANTIES WHETHER EXPRESSED OR IMPLIED. NO REPONSIBILITIES FOR POSSIBLE
//	DAMAGES OR EVEN FUNCTIONALITY CAN BE TAKEN. THE USER MUST ASSUME THE ENTIRE
//	RISK OF USING THIS SOFTWARE.
//
//	Terms of use
//	------------
//	THIS SOFTWARE IS FREE FOR PERSONAL USE OR FREEWARE APPLICATIONS.
//	IF YOU USE THIS SOFTWARE IN COMMERCIAL OR SHAREWARE APPLICATIONS YOU
//	ARE GENTLY ASKED TO DONATE 5$ (FIVE U.S. DOLLARS) TO THE AUTHOR:
//
//		Davide Calabro'
//		P.O. Box 65
//		21019 Somma Lombardo (VA)
//		Italy
//
#ifndef _BTNST_H
#define _BTNST_H

#pragma once

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

class CButtonST : public CButton
{
public:
	CButtonST();
	~CButtonST();

	enum	{	ST_ALIGN_HORIZ = 0,			// Icon/bitmap on the left, text on the right
			ST_ALIGN_VERT,				// Icon/bitmap on the top, text on the bottom
			ST_ALIGN_HORIZ_RIGHT,			// Icon/bitmap on the right, text on the left
			ST_ALIGN_OVERLAP			// Icon/bitmap on the same space as text
		};

	enum	{	BTNST_COLOR_BK_IN = 0,			// Background color when mouse is INside
			BTNST_COLOR_FG_IN,			// Text color when mouse is INside
			BTNST_COLOR_BK_OUT,			// Background color when mouse is OUTside
			BTNST_COLOR_FG_OUT,			// Text color when mouse is OUTside
			BTNST_COLOR_BK_FOCUS,			// Background color when the button is focused
			BTNST_COLOR_FG_FOCUS,			// Text color when the button is focused
			BTNST_MAX_COLORS
		};

	 enum	{	BTNST_PRESSED_LEFTRIGHT = 0,		// Pressed style from left to right (as usual)
			BTNST_PRESSED_TOPBOTTOM			// Pressed style from top to bottom
		};

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CButtonST)
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

public:
	void	SetSelected(BOOL bselect = FALSE){	m_bSelected = bselect;	}
	DWORD	SetDefaultColors(BOOL bRepaint = TRUE);
	DWORD	SetColor(BYTE byColorIndex, COLORREF crColor, BOOL bRepaint = TRUE);

	DWORD	SetFlat(BOOL bFlat = TRUE, BOOL bRepaint = TRUE);
	DWORD	SetPressedStyle(BYTE byStyle, BOOL bRepaint = TRUE);

	static short GetVersionI()		{ return 39;}
	static LPCTSTR GetVersionC()		{ return (LPCTSTR)_T("3.9");}

	POINT	m_ptPressedOffset;
	BOOL	m_bNoFocus;

protected:
    //{{AFX_MSG(CButtonST)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSysColorChange();
	afx_msg BOOL OnClicked();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnEnable(BOOL bEnable);
	afx_msg void OnCancelMode();
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);

	virtual DWORD OnDrawBackground(CDC* pDC, CRect* pRect);

	BOOL		m_bIsFlat;		// Is a flat button?
	BOOL		m_bMouseOnButton;	// Is mouse over the button?
	BOOL		m_bIsPressed;		// Is button pressed?
	BOOL		m_bIsFocused;		// Is button focused?
	BOOL		m_bIsDisabled;		// Is button disabled?
	BOOL		m_bIsDefault;		// Is default button?
	BOOL		m_bIsCheckBox;		// Is the button a checkbox?
	COLORREF	m_crColors[BTNST_MAX_COLORS];	// Colors to be used
	BOOL		m_bSelected;
	
private:
	LRESULT	OnSetCheck(WPARAM wParam, LPARAM lParam);
	LRESULT	OnGetCheck(WPARAM wParam, LPARAM lParam);
	LRESULT	OnSetStyle(WPARAM wParam, LPARAM lParam);

	void	CancelHover();
	virtual void DrawTheText(CDC* pDC, LPCTSTR lpszText, RECT* rpItem, CRect* rpCaption, BOOL bIsPressed, BOOL bIsDisabled);

	int		m_nCheck;		// Current value for checkbox
	UINT		m_nTypeStyle;		// Button style

	CFont		m_Font;
	CFont		m_boldFont;

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif
