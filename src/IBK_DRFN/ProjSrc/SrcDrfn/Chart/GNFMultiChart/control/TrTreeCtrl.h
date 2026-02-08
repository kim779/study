#if !defined(AFX_TRTREECTRL_H__60B05D0C_0948_4C5E_9370_11931912D4FB__INCLUDED_)
#define AFX_TRTREECTRL_H__60B05D0C_0948_4C5E_9370_11931912D4FB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TrTreeCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTrTreeCtrl window

class CTrTreeCtrl : public CTreeCtrl
{
// Construction
public:
	CTrTreeCtrl();

// Attributes
public:
	HTREEITEM m_hSelTreeItem;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTrTreeCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTrTreeCtrl();
	
	CString GetTrString(CString szText);
	BOOL Create(DWORD dwStyle, const RECT& rect, CWnd * pParentWnd, UINT nID);

	//Drag Ãß°¡
	BOOL m_bLBtnDown;
	BOOL m_bDragNDrop;
	// Generated message map functions
protected:
	//{{AFX_MSG(CTrTreeCtrl)
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRTREECTRL_H__60B05D0C_0948_4C5E_9370_11931912D4FB__INCLUDED_)
