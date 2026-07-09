#if !defined(AFX_RESEDITOR_H__0E76B500_5739_11D4_A024_00001CD7F9BE__INCLUDED_)
#define AFX_RESEDITOR_H__0E76B500_5739_11D4_A024_00001CD7F9BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// resEditor.h : header file
//

#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CResEditor dialog

class AFX_EXT_CLASS CResEditor : public CDialog
{
// Construction
public:
	CResEditor(CWnd* pParent, struct _mapH* mapH, struct _formR* formR, CString curdir);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CResEditor)
	enum { IDD = IDD_RESOURCE };
	CComboBox	m_event;
	CString		m_script;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CResEditor)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Attributes
	int	m_prev;
	struct	_mapH*	m_mapH;
	struct	_formR*	m_formR;
	CString	m_curdir;

	CString	m_click;
	CString	m_dblclk;
	CString	m_change;
	CString	m_char;

// Implementation
protected:
	void	insertToEdit(CString str);
	// Generated message map functions
	//{{AFX_MSG(CResEditor)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelChangeEvent();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RESEDITOR_H__0E76B500_5739_11D4_A024_00001CD7F9BE__INCLUDED_)
