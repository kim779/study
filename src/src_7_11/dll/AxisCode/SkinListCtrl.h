#if !defined(AFX_SKINLISTCTRL_H__D65A645A_8C29_4A93_B453_ED9C92807426__INCLUDED_)
#define AFX_SKINLISTCTRL_H__D65A645A_8C29_4A93_B453_ED9C92807426__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
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
	void SetCustomDrawList(BOOL bDraw = TRUE){m_bCustomDrawList = bDraw;	};
	
	void AddCustomItem(DWORD dwData);
	void SortCustomItem(LPARAM lParamSort, BOOL bAscending = FALSE);
	void QuickSortRe(int low, int high, LPARAM lParamSort, BOOL bAscending = FALSE);
	void RemoveAllCustomItem();
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
	void SetAtmPutIndex(int nIdx);
	void SetAtmCallIndex(int nIdx);
	virtual ~CSkinListCtrl();


protected:
	int		m_nAtmCallIdx;
	int		m_nAtmPutIdx;

	COLORREF	m_cFGColor;
	COLORREF	m_cBGColor;
	BOOL		m_bUse;
	BOOL		m_bCustomDrawList;

	CSkinHeaderCtrl m_SkinHeaderCtrl;
	CArray<DWORD, DWORD> m_arrayCustomItem;
	// Generated message map functions

protected:
	//{{AFX_MSG(CSkinListCtrl)
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	afx_msg void OnGetdispinfo(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	afx_msg void OnCustomDrawList ( NMHDR* pNMHDR, LRESULT* pResult );
//	afx_msg LRESULT OnSetFont(WPARAM wParam, LPARAM);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SKINLISTCTRL_H__D65A645A_8C29_4A93_B453_ED9C92807426__INCLUDED_)
