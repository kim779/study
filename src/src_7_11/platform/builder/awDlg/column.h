#if !defined(AFX_COLUMN_H__8BD62425_5C90_11D4_A024_00001CD7F9BE__INCLUDED_)
#define AFX_COLUMN_H__8BD62425_5C90_11D4_A024_00001CD7F9BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// column.h : header file
//

#include "resource.h"
#include "../form_w.h"
#include "../mapvar.h"
#include "../awCommon/xbutton.h"

/////////////////////////////////////////////////////////////////////////////
// CColumn dialog

class CColumn : public CDialog
{
// Construction
public:
	CColumn(CWnd* pParent, struct _repR* repR, class CAxisPalette* pal, DWORD attr, bool common = false);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CColumn)
	enum { IDD = IDD_CELL };
	CString	m_domino;
	CString	m_float;
	CString	m_hpRGB;
	CString	m_htRGB;
	CString	m_len;
	BOOL	m_multi;
	CString	m_pRGB;
	CString	m_width;
	CString	m_head;
	CString	m_symbol;
	CString	m_tRGB;
	BOOL	m_visible;
	CString	m_reference;
	BOOL	m_sort;
	CString	m_check;
	BOOL	m_cut;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColumn)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Attributes
public:
	DWORD	m_attr;
	struct	_repR	m_repR;

protected:
	CWnd*	m_parent;
	struct	_repR*	m_pRepR;
	CAxisPalette*	m_palette;
	CXButton	m_column[MAX_COLUMN];
	int	m_sel;
	bool	m_first;
	bool	m_common;

// Implementation
protected:
	void	setColumn(int id, bool save = true);
	void	setColumn();
	void	saveColumn();
	void	applyCommonColumn(int prop);

	// Generated message map functions
	//{{AFX_MSG(CColumn)
	virtual BOOL OnInitDialog();
	afx_msg void OnInsert();
	afx_msg void OnDelete();
	afx_msg void OnChangeColor();
	afx_msg void OnDominoCheck();
	afx_msg void OnReferenceCheck();
	afx_msg void OnEdit();
	afx_msg void OnAttr();
	afx_msg void OnHeadPRGB();
	afx_msg void OnHeadTRGB();
	afx_msg void OnPRGB();
	afx_msg void OnTRGB();
	afx_msg void OnSelChangeDataType();
	afx_msg void OnSelChangeKind();
	afx_msg void OnSelChangeMode();
	afx_msg void OnColumn01();
	afx_msg void OnColumn02();
	afx_msg void OnColumn03();
	afx_msg void OnColumn04();
	afx_msg void OnColumn05();
	afx_msg void OnColumn06();
	afx_msg void OnColumn07();
	afx_msg void OnColumn08();
	afx_msg void OnColumn09();
	afx_msg void OnColumn10();
	afx_msg void OnColumn11();
	afx_msg void OnColumn12();
	afx_msg void OnColumn13();
	afx_msg void OnColumn14();
	afx_msg void OnColumn15();
	afx_msg void OnColumn16();
	afx_msg void OnColumn17();
	afx_msg void OnColumn18();
	afx_msg void OnColumn19();
	afx_msg void OnColumn20();
	afx_msg void OnColumn21();
	afx_msg void OnColumn22();
	afx_msg void OnColumn23();
	afx_msg void OnColumn24();
	afx_msg void OnColumn25();
	afx_msg void OnColumn26();
	afx_msg void OnColumn27();
	afx_msg void OnColumn28();
	afx_msg void OnColumn29();
	afx_msg void OnColumn30();
	afx_msg void OnColumn31();
	afx_msg void OnColumn32();
	virtual void OnOK();
	afx_msg void OnData();
	afx_msg void OnApply();
	afx_msg void OnShiftLeft();
	afx_msg void OnShiftRight();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLUMN_H__8BD62425_5C90_11D4_A024_00001CD7F9BE__INCLUDED_)
