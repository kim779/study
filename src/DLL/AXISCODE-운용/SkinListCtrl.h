#pragma once
// SkinListCtrl.h : header file
//
#include "SkinHeaderCtrl.h"

/*****************************************************************
** credits for the custom highlight color bar
** using custom draw technique goes to Michael Dunn
**
** Neat Stuff to do in List Controls Using Custom Draw
** By Michael Dunn 
** http://www.codeproject.com/listctrl/lvcustomdraw.asp?print=true
******************************************************************/

/////////////////////////////////////////////////////////////////////////////
// CSkinListCtrl window

class CSkinListCtrl : public CListCtrl
{
// Construction
public:
	CSkinListCtrl();
	
	void EnableHighlighting(HWND hWnd, int row, bool bHighlight);
	bool IsRowSelected(HWND hWnd, int row);
	bool IsRowHighlighted(HWND hWnd, int row);
	void Selected(int nItem, BOOL scroll = TRUE);				// selected item position
	void SetHeaderUse(BOOL use = TRUE){ m_bUse = use; };


// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSkinListCtrl)
	protected:
	virtual void PreSubclassWindow();
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSkinListCtrl();

protected:
	COLORREF m_cFGColor;
	COLORREF m_cBGColor;
	BOOL	 m_bUse;

	CSkinHeaderCtrl m_SkinHeaderCtrl;

	// Generated message map functions

protected:
	//{{AFX_MSG(CSkinListCtrl)
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	//}}AFX_MSG
	afx_msg void OnCustomDrawList ( NMHDR* pNMHDR, LRESULT* pResult );
//	afx_msg LRESULT OnSetFont(WPARAM wParam, LPARAM);
	DECLARE_MESSAGE_MAP()
};

