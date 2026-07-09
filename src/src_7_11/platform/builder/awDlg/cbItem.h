#if !defined(AFX_CBITEM_H__8BD62424_5C90_11D4_A024_00001CD7F9BE__INCLUDED_)
#define AFX_CBITEM_H__8BD62424_5C90_11D4_A024_00001CD7F9BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// cbItem.h : header file
//

#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CComboItem dialog

class AFX_EXT_CLASS CComboItem : public CDialog
{
// Construction
public:
	CComboItem(CWnd* pParent, int count, int size);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CComboItem)
	enum { IDD = IDD_COMBOITEM };
	CListBox	m_data;
	CListBox	m_display;
	CString	m_edata;
	CString	m_edisplay;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CComboItem)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Attributes
public:
	CString Parser(CString &srcstr, CString substr);
	int	m_count;
	CString	m_datB, m_disB;

protected:
	int	m_sel;
	int	m_size;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CComboItem)
	virtual BOOL OnInitDialog();
	afx_msg void OnInsert();
	afx_msg void OnModify();
	afx_msg void OnAdd();
	afx_msg void OnDelete();
	afx_msg void OnSelChangeDisplay();
	afx_msg void OnDblClkDisplay();
	afx_msg void OnSelChangeData();
	afx_msg void OnDblClkData();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnIdup();
	afx_msg void OnIddown();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CBITEM_H__8BD62424_5C90_11D4_A024_00001CD7F9BE__INCLUDED_)
