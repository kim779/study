#if !defined(AFX_TABPAGE_H__94A19820_59C3_11D4_A024_00001CD7F9BE__INCLUDED_)
#define AFX_TABPAGE_H__94A19820_59C3_11D4_A024_00001CD7F9BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabPage.h : header file
//

#include "resource.h"
#include "../mapvar.h"

/////////////////////////////////////////////////////////////////////////////
// CTabPage dialog

class CTabPage : public CDialog
{
// Construction
public:
	CTabPage(CWnd* pParent, struct _formR* formR);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTabPage)
	enum { IDD = IDD_TABITEM };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Attributes
protected:
	int		m_sel;
	struct	_formR*	m_formR;
	CEdit*		m_text[MAX_TAB];
	CEdit*		m_map[MAX_TAB];

// Implementation
protected:
	void	setTextIndex(int id);
	void	setMapIndex(int id);

	// Generated message map functions
	//{{AFX_MSG(CTabPage)
	afx_msg void OnInsert();
	afx_msg void OnDelete();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnSetFocusCaption1();
	afx_msg void OnSetFocusCaption2();
	afx_msg void OnSetFocusCaption3();
	afx_msg void OnSetFocusCaption4();
	afx_msg void OnSetFocusCaption5();
	afx_msg void OnSetFocusCaption6();
	afx_msg void OnSetFocusCaption7();
	afx_msg void OnSetFocusCaption8();
	afx_msg void OnSetFocusCaption9();
	afx_msg void OnSetFocusCaption10();
	afx_msg void OnSetFocusCaption11();
	afx_msg void OnSetFocusCaption12();
	afx_msg void OnSetFocusCaption13();
	afx_msg void OnSetFocusCaption14();
	afx_msg void OnSetFocusCaption15();
	afx_msg void OnSetFocusCaption16();
	afx_msg void OnSetFocusMap1();
	afx_msg void OnSetFocusMap2();
	afx_msg void OnSetFocusMap3();
	afx_msg void OnSetFocusMap4();
	afx_msg void OnSetFocusMap5();
	afx_msg void OnSetFocusMap6();
	afx_msg void OnSetFocusMap7();
	afx_msg void OnSetFocusMap8();
	afx_msg void OnSetFocusMap9();
	afx_msg void OnSetFocusMap10();
	afx_msg void OnSetFocusMap11();
	afx_msg void OnSetFocusMap12();
	afx_msg void OnSetFocusMap13();
	afx_msg void OnSetFocusMap14();
	afx_msg void OnSetFocusMap15();
	afx_msg void OnSetFocusMap16();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABPAGE_H__94A19820_59C3_11D4_A024_00001CD7F9BE__INCLUDED_)
