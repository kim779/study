// XListCtrl.h  Version 1.3
//
// Author:  Hans Dietrich
//          hdietrich2@hotmail.com
//
// This software is released into the public domain.
// You are free to use it in any way you like.
//
// This software is provided "as is" with no expressed
// or implied warranty.  I accept no liability for any
// damage or loss of business that this software may cause.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef XLISTCTRL_H
#define XLISTCTRL_H

#include "XHeaderCtrl.h"
//#ifndef DO_NOT_INCLUDE_XCOMBOLIST
//#include "XComboList.h"
//#endif

//extern UINT NEAR WM_XLISTCTRL_COMBO_SELECTION;
extern UINT NEAR WM_XLISTCTRL_CHECKBOX_CLICKED;

///////////////////////////////////////////////////////////////////////////////
// Style bits for use with SetExtendedStyleX()

// none defined yet

///////////////////////////////////////////////////////////////////////////////
// CXListCtrl data

struct XLISTCTRLDATA
{
	// ctor
	XLISTCTRLDATA()
	{
		bEnabled             = TRUE;
		crText               = ::GetSysColor(COLOR_WINDOWTEXT);
		crBackground         = ::GetSysColor(COLOR_WINDOW);
		nCheckedState        = -1;
		bBold                = FALSE;
		nImage               = -1;
		dwItemData           = 0;
	}

	BOOL			bEnabled;				// TRUE = enabled, FALSE = disabled (gray text)
	BOOL			bBold;					// TRUE = display bold text
	int			nImage;					// index in image list, else -1

	// for color
	COLORREF	crText;
	COLORREF	crBackground;

	// for checkbox
	int		nCheckedState;					// -1 = don't show, 0 = unchecked, 1 = checked
	DWORD		dwItemData;					// pointer to app's data
};


///////////////////////////////////////////////////////////////////////////////
// CXListCtrl class

class CXListCtrl : public CListCtrl
{
// Construction
public:
	CXListCtrl();
	virtual ~CXListCtrl();

// Attributes
public:

// Operations
public:
	int	CountCheckedItems(int nSubItem);
	BOOL	DeleteAllItems();
	BOOL	DeleteItem(int nItem);
	BOOL	GetBold(int nItem, int nSubItem);
	int	GetCheckbox(int nItem, int nSubItem);
	int	GetColumns();
	int	GetCurSel();
	BOOL	GetEnabled(int nItem);
	DWORD	GetExtendedStyleX() { return m_dwExtendedStyleX; }
	int	GetHeaderCheckedState(int nSubItem);
	DWORD	GetItemData(int nItem);
	BOOL	GetSubItemRect(int iItem, int iSubItem, int nArea, CRect& rect);
	int	InsertItem(int nItem, LPCTSTR lpszItem);
	int	InsertItem(int nItem, LPCTSTR lpszItem, COLORREF crText, COLORREF crBackground);
	int	InsertItem(const LVITEM* pItem);
	BOOL	SetBold(int nItem, int nSubItem, BOOL bBold);
	BOOL	SetCheckbox(int nItem, int nSubItem, int nCheckedState);
	BOOL	SetCurSel(int nItem);
	BOOL	SetEnabled(int nItem, BOOL bEnable);
	DWORD	SetExtendedStyleX(DWORD dwNewStyle) 
		{
			DWORD dwOldStyle = m_dwExtendedStyleX;
			m_dwExtendedStyleX = dwNewStyle;
			return dwOldStyle;
		}

	BOOL	SetHeaderCheckedState(int nSubItem, int nCheckedState);
	int	SetItem(const LVITEM* pItem);
	BOOL	SetItemData(int nItem, DWORD dwData);
	BOOL	SetItemImage(int nItem, int nSubItem, int nImage);
	BOOL	SetItemText(int nItem, int nSubItem, LPCTSTR lpszText); 
	BOOL	SetItemText(int nItem, int nSubItem, LPCTSTR lpszText, COLORREF crText, COLORREF crBackground);
	void	UpdateSubItem(int nItem, int nSubItem);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXListCtrl)
public:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	CXHeaderCtrl	m_HeaderCtrl;
	CImageList	m_cImageList;	// Image list for the header control

protected:
	void DrawCheckbox(int nItem, 
			  int nSubItem, 
			  CDC *pDC, 
			  COLORREF crText,
			  COLORREF crBkgnd,
			  CRect& rect, 
			  XLISTCTRLDATA *pCLD);
	int DrawImage(int nItem, 
			  int nSubItem, 
			  CDC* pDC, 
			  COLORREF crText,
			  COLORREF crBkgnd,
			  CRect rect,
			  XLISTCTRLDATA *pXLCD);
	void DrawText(int nItem, 
			  int nSubItem, 
			  CDC *pDC, 
			  COLORREF crText,
			  COLORREF crBkgnd,
			  CRect& rect, 
			  XLISTCTRLDATA *pCLD);
	void GetDrawColors(int nItem,
			   int nSubItem,
			   COLORREF& colorText,
			   COLORREF& colorBkgnd);
	void SubclassHeaderControl();

	BOOL			m_bHeaderIsSubclassed;
	DWORD			m_dwExtendedStyleX;

	COLORREF		m_cr3DFace;
	COLORREF		m_cr3DHighLight;
	COLORREF		m_cr3DShadow;
	COLORREF		m_crBtnFace;
	COLORREF		m_crBtnShadow;
	COLORREF		m_crBtnText;
	COLORREF		m_crGrayText;
	COLORREF		m_crHighLight;
	COLORREF		m_crHighLightText;
	COLORREF		m_crWindow;
	COLORREF		m_crWindowText;


	// Generated message map functions
protected:
	//{{AFX_MSG(CXListCtrl)
	afx_msg BOOL OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnSysColorChange();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif //XLISTCTRL_H
