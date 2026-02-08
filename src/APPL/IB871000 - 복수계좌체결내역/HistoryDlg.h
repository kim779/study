#pragma once

#include "Common.h"
#include "AxisExt.h"

class CHistoryDlg : public CDialog, public CAxisExt
{
public:
	CHistoryDlg(CWnd* pWizard, CWnd* pParent = NULL);   // standard constructor
	void AddData(LPCSTR text, COLORREF color);

public:
	CStringArray m_arList;
	CArray<COLORREF, COLORREF> m_arColor;


// Dialog Data
	//{{AFX_DATA(CHistoryDlg)
	enum { IDD = IDD_HISTORY };
	CListBox	m_list;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHistoryDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CHistoryDlg)
	afx_msg void OnKillfocusHistlist();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

