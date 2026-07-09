#if !defined(AFX_GRIDCOLUMNDLG_H__CFD347B8_A530_4E00_8D4B_7B3EE3F72BA8__INCLUDED_)
#define AFX_GRIDCOLUMNDLG_H__CFD347B8_A530_4E00_8D4B_7B3EE3F72BA8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GridColumnDlg.h : header file
//
#include "resource.h"
#include "../form_w.h"
#include "../mapvar.h"
#include "../ListGrid.h"

/////////////////////////////////////////////////////////////////////////////
// CGridColumnDlg dialog

class AFX_EXT_CLASS CGridColumnDlg : public CDialog
{
// Construction
public:
	CGridColumnDlg(CWnd* pParent = NULL, struct _mapH* mapH = NULL, int index = -1);   // standard constructor

public:
	struct	_repR	*m_repR;
	struct	_mapH	*m_mapH;
protected:
	CListGrid	m_listGrid;
	CString		m_sCellGrid, m_sCellTable;
	int		m_index;
	bool		m_bGrid;

// Dialog Data
	//{{AFX_DATA(CGridColumnDlg)
	enum { IDD = IDD_GRIDCOLUMN };
	CButton	m_okBtn;
	BOOL	m_bMultiLine;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGridColumnDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGridColumnDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnAddbtn();
	afx_msg void OnDelbtn();
	afx_msg void OnMultiline();
	virtual void OnOK();
	afx_msg void OnClose();
	afx_msg void OnInsertbtn();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRIDCOLUMNDLG_H__CFD347B8_A530_4E00_8D4B_7B3EE3F72BA8__INCLUDED_)
