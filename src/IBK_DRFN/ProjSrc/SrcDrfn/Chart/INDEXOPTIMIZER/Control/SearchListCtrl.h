#if !defined(AFX_SEARCHLISTCTRL_H__F8BDBC1C_7387_4588_B71E_6C44DFDD7223__INCLUDED_)
#define AFX_SEARCHLISTCTRL_H__F8BDBC1C_7387_4588_B71E_6C44DFDD7223__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SearchListCtrl.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CSearchListCtrl window

class CSearchListCtrl : public CListCtrl
{
// Construction
public:
	CSearchListCtrl();

// Attributes
public:

// Operations
public:
	int m_nHighlight;
	int m_nCurFocus;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSearchListCtrl)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL
	void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
// Implementation
public:
	void FP_vSortItem();
	CString FP_csEditEnter();
	void FP_vEventUpDown(UINT nKey);
	virtual ~CSearchListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSearchListCtrl)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnKillfocus(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEARCHLISTCTRL_H__F8BDBC1C_7387_4588_B71E_6C44DFDD7223__INCLUDED_)
