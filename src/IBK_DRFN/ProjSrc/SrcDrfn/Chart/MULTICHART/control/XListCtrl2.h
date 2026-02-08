// XListCtrl2.h  Version 1.3
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

#ifndef XLISTCTRL2_H
#define XLISTCTRL2_H

#include "XHeaderCtrl.h"
#ifndef DO_NOT_INCLUDE_XCOMBOLIST
#include "XComboList.h"
#endif
#include "XEdit.h"

extern UINT NEAR WM_XLISTCTRL2_COMBO_SELECTION;
extern UINT NEAR WM_XLISTCTRL2_CHECKBOX_CLICKED;


#define DONOT_SHOW  -1
#define UNCHECKED	 0
#define CHECKED		 1
///////////////////////////////////////////////////////////////////////////////
// Style bits for use with SetExtendedStyleX()

// none defined yet

///////////////////////////////////////////////////////////////////////////////
// CXListCtrl2 data

struct XLISTCTRLDATA
{
	// ctor
	XLISTCTRLDATA()
	{
		bEnabled             = TRUE;
		crText               = ::GetSysColor(COLOR_WINDOWTEXT);
		crBackground         = ::GetSysColor(COLOR_WINDOW);
		bShowProgress        = FALSE;
		nProgressPercent     = 0;
		strProgressMessage   = _T("");
		bShowProgressMessage = TRUE;
		nCheckedState        = -1;
		bBold                = FALSE;
		nImage               = -1;
#ifndef NO_XLISTCTRL_TOOL_TIPS
		strToolTip           = _T("");
#endif
		bCombo               = FALSE;
		bComboIsClicked      = FALSE;
		nComboListHeight     = 10;
		nInitialComboSel     = 0;
		bUseEditBox			 = FALSE;
		psa                  = NULL;
		dwItemData           = 0;
	}

	BOOL			bEnabled;				// TRUE = enabled, FALSE = disabled (gray text)
	BOOL			bBold;					// TRUE = display bold text
	int				nImage;					// index in image list, else -1
#ifndef NO_XLISTCTRL_TOOL_TIPS
	CString			strToolTip;				// tool tip text for cell
#endif

	// for combo
	BOOL			bCombo;					// TRUE = display combobox
	BOOL			bComboIsClicked;		// TRUE = downarrow is clicked
	CStringArray *	psa;					// pointer to string array for combo listbox
	int				nComboListHeight;		// combo listbox height (in rows)
	int				nInitialComboSel;		// initial combo listbox selection (0 = first)

	// for color
	COLORREF	crText;
	COLORREF	crBackground;

	// for progress
	BOOL		bShowProgress;				// true = show progress control
	int			nProgressPercent;			// 0 - 100
	CString		strProgressMessage;			// custom message for progress indicator -
											// MUST INCLUDE %d
	BOOL		bShowProgressMessage;		// TRUE = display % message, or custom message
											// if one is supplied
	// for checkbox
	
	int			nCheckedState;				// -1 = don't show, 0 = unchecked, 1 = checked
	
	// for editbox
	BOOL		bUseEditBox;

	DWORD		dwItemData;					// pointer to app's data
};


///////////////////////////////////////////////////////////////////////////////
// CXListCtrl2 class

//#include "../../../../DataMgr/DBMgr.h" //05.09.27
class CXListCtrl2 : public CListCtrl
{
// Construction
public:
	CXListCtrl2();
	virtual ~CXListCtrl2();

// Attributes
public:
	//05. 08. 22 added by sy, nam
	BOOL m_bAutoRunRecycle;
	void SetAutoRunRecycle(BOOL bAutoRun) { m_bAutoRunRecycle = bAutoRun; }

// Operations
private:
	BOOL	m_bShowEmptyMessage;
public:
//	void	ShowEmptyMessage() { m_bShowEmptyMessage = TRUE; Invalidate();}
	int		CountCheckedItems(int nSubItem);
	BOOL	DeleteAllItems();
	BOOL	DeleteItem(int nItem);
	void	DeleteProgress(int nItem, int nSubItem);
	BOOL	GetBold(int nItem, int nSubItem);
	int		GetCheckbox(int nItem, int nSubItem);
	int		GetColumns();
	CString	GetComboText(int iItem, int iSubItem);
	int		GetCurSel();
	BOOL	GetEnabled(int nItem);
	DWORD	GetExtendedStyleX() { return m_dwExtendedStyleX; }
	int		GetHeaderCheckedState(int nSubItem);
	DWORD	GetItemData(int nItem);
	BOOL	GetSubItemRect(int iItem, int iSubItem, int nArea, CRect& rect);
	int		InsertItem(int nItem, LPCTSTR lpszItem);
	int		InsertItem(int nItem, 
					   LPCTSTR lpszItem, 
					   COLORREF crText, 
					   COLORREF crBackground);
	int		InsertItem(const LVITEM* pItem);
	BOOL	SetBold(int nItem, int nSubItem, BOOL bBold);
	BOOL	SetComboBox(int nItem, 
						int nSubItem, 
						BOOL bEnableCombo, 
						CStringArray *psa,
						int nComboListHeight,
						int nInitialComboSel);
	BOOL	SetCheckbox(int nItem, int nSubItem, int nCheckedState);
	BOOL	SetCurSel(int nItem, BOOL bSelect = TRUE);
	BOOL	SetEnabled(int nItem, BOOL bEnable);
	DWORD	SetExtendedStyleX(DWORD dwNewStyle) 
	{
		DWORD dwOldStyle = m_dwExtendedStyleX;
		m_dwExtendedStyleX = dwNewStyle;
		return dwOldStyle;
	}

	BOOL	SetHeaderCheckedState(int nSubItem, int nCheckedState);
	int		SetItem(const LVITEM* pItem);
	BOOL	SetItemData(int nItem, DWORD dwData);
	BOOL	SetItemImage(int nItem, int nSubItem, int nImage);
	BOOL	SetItemText(int nItem, int nSubItem, LPCTSTR lpszText); 
	BOOL	SetItemText(int nItem, 
						int nSubItem, 
						LPCTSTR lpszText,
						COLORREF crText, 
						COLORREF crBackground);
	BOOL	SetProgress(int nItem, 
						int nSubItem, 
						BOOL bShowProgressText = TRUE, 
						LPCTSTR lpszProgressText = NULL);
	void	UpdateProgress(int nItem, int nSubItem, int nPercent);
	void	UpdateSubItem(int nItem, int nSubItem);
	BOOL    SetEditBox(int nItem, int nSubItem,	 BOOL bUseEditBox = TRUE);

	void	SetHilightColor(COLORREF crText,COLORREF crBackground) 
	{ 
		m_crHighLightText = m_crUsrHighLightText = crText;
		m_crHighLight = m_crUsrHighLight = crBackground;
	}
	void SetSortImage(int nCol,int nType );

#ifndef NO_XLISTCTRL_TOOL_TIPS
	void DeleteAllToolTips();
	BOOL SetItemToolTipText(int nItem, int nSubItem, LPCTSTR lpszToolTipText);
	CString GetItemToolTipText(int nItem, int nSubItem);
	virtual int OnToolHitTest(CPoint point, TOOLINFO * pTI) const;
#endif

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXListCtrl2)
public:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	CXHeaderCtrl	m_HeaderCtrl;
	CImageList		m_cImageList;	// Image list for the header control

protected:
	void DrawCheckbox(int nItem, 
					  int nSubItem, 
					  CDC *pDC, 
					  COLORREF crText,
					  COLORREF crBkgnd,
					  CRect& rect, 
					  XLISTCTRLDATA *pCLD);
#ifndef DO_NOT_INCLUDE_XCOMBOLIST
	void DrawComboBox(int nItem, 
					  int nSubItem, 
					  CDC *pDC, 
					  COLORREF crText,
					  COLORREF crBkgnd,
					  CRect& rect, 
					  XLISTCTRLDATA *pCLD);
	void UnpressComboButton();
#endif
	int DrawImage(int nItem, 
				  int nSubItem, 
				  CDC* pDC, 
				  COLORREF crText,
				  COLORREF crBkgnd,
				  CRect rect,
				  XLISTCTRLDATA *pXLCD);
	void DrawProgress(int nItem, 
					  int nSubItem, 
					  CDC *pDC, 
					  COLORREF crText,
					  COLORREF crBkgnd,
					  CRect& rect, 
					  XLISTCTRLDATA *pCLD);
	void DrawText(int nItem, 
				  int nSubItem, 
				  CDC *pDC, 
				  COLORREF crText,
				  COLORREF crBkgnd,
				  CRect& rect, 
				  XLISTCTRLDATA *pCLD);
	virtual void GetDrawColors(int nItem,
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

	COLORREF		m_crUsrHighLight;
	COLORREF		m_crUsrHighLightText;

#ifndef DO_NOT_INCLUDE_XCOMBOLIST
	BOOL			m_bComboIsClicked;
	int				m_nComboItem;
	int				m_nComboSubItem;
	CRect			m_rectComboButton;
	CRect			m_rectComboList;
	CXComboList *	m_pListBox;
	CFont			m_ListboxFont;
	BOOL			m_bFontIsCreated;
	CString			m_strInitialComboString;
#endif

	CXEdit*			m_pEdit;
	CString			m_strEdit;
	int				m_nEditItem;
	int				m_nEditSubItem;

	// Generated message map functions
protected:
	//{{AFX_MSG(CXListCtrl2)
	afx_msg BOOL OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnSysColorChange();
	afx_msg BOOL OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
#ifndef DO_NOT_INCLUDE_XCOMBOLIST
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg LRESULT OnComboEscape(WPARAM, LPARAM);
	afx_msg LRESULT OnComboReturn(WPARAM, LPARAM);
	afx_msg LRESULT OnComboKeydown(WPARAM, LPARAM);
	afx_msg LRESULT OnComboLButtonUp(WPARAM, LPARAM);
#endif
	
#ifndef NO_XLISTCTRL_TOOL_TIPS
	virtual afx_msg BOOL OnToolTipText(UINT id, NMHDR * pNMHDR, LRESULT * pResult);
#endif
	LRESULT OnEditReturn( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif //XLISTCTRL2_H
