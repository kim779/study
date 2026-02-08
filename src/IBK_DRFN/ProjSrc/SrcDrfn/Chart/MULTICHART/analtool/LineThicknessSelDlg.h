#if !defined(AFX_LINETHICKNESSSELDLG_H__BA398077_A4B3_45FE_9B29_5026E065583A__INCLUDED_)
#define AFX_LINETHICKNESSSELDLG_H__BA398077_A4B3_45FE_9B29_5026E065583A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LineThicknessSelDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLineThicknessSelDlg dialog

#define IDD_DIALOG_LINE_THICKNESS       1039

#include "LineSelListBox.h"

class CLineThicknessSelDlg : public CDialog
{
// Construction
public:
	CLineThicknessSelDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLineThicknessSelDlg)
	enum { IDD = IDD_DIALOG_LINE_THICKNESS };
	CLineSelListBox	m_lineListbox;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLineThicknessSelDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLineThicknessSelDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LINETHICKNESSSELDLG_H__BA398077_A4B3_45FE_9B29_5026E065583A__INCLUDED_)
