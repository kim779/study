#if !defined(AFX_AXTICKER_H__3F31FC41_1B37_11D6_8AC9_00001CD7F9BE__INCLUDED_)
#define AFX_AXTICKER_H__3F31FC41_1B37_11D6_8AC9_00001CD7F9BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// axTicker.h : header file
//

#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CAxTicker dialog

#define	TBM_SELECTION		1
#define	TBM_NOSELECTION		2

class CAxTicker : public CDialog
{
// Construction
public:
	CAxTicker(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAxTicker)
	enum { IDD = IDD_TICKER };
	CListBox	m_list;
	//}}AFX_DATA

protected:
	int	m_count;
	int	m_selection;
	CWnd*	m_parent;

public:
	BOOL	Create(CWnd* pParentWnd);
	bool	UpdateListBox(CStringArray& array);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAxTicker)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAxTicker)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnListDblClk();
	afx_msg void OnListSelChange();
	afx_msg void OnListKillFocus();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AXTICKER_H__3F31FC41_1B37_11D6_8AC9_00001CD7F9BE__INCLUDED_)
