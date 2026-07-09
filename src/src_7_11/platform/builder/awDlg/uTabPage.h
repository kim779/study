#if !defined(AFX_UTABPAGE_H__8BD62420_5C90_11D4_A024_00001CD7F9BE__INCLUDED_)
#define AFX_UTABPAGE_H__8BD62420_5C90_11D4_A024_00001CD7F9BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// uTabPage.h : header file
//

#include "resource.h"
#include "../mapvar.h"

/////////////////////////////////////////////////////////////////////////////
// CUTabPage dialog

class AFX_EXT_CLASS CUTabPage : public CDialog
{
// Construction
public:
	CString Parser(CString &srcstr, CString substr);
	CUTabPage(CWnd* pParent, struct _mapH* mapH, int index);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CUTabPage)
	enum { IDD = IDD_UTABITEM };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUTabPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Attributes
protected:
	int		m_sel, m_index;
	struct _mapH*	m_mapH;
	CEdit*		m_text[MAX_TAB];
	CEdit*		m_ID[MAX_TAB];
	CButton*	m_visible[MAX_TAB];

// Implementation
protected:
	void	setTextIndex(int id);
	void	setIDIndex(int id);
	void	setVisibleIndex(int id);

	// Generated message map functions
	//{{AFX_MSG(CUTabPage)
	virtual BOOL OnInitDialog();
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
	afx_msg void OnSetFocusID1();
	afx_msg void OnSetFocusID2();
	afx_msg void OnSetFocusID3();
	afx_msg void OnSetFocusID4();
	afx_msg void OnSetFocusID5();
	afx_msg void OnSetFocusID6();
	afx_msg void OnSetFocusID7();
	afx_msg void OnSetFocusID8();
	afx_msg void OnSetFocusID9();
	afx_msg void OnSetFocusID10();
	afx_msg void OnSetFocusID11();
	afx_msg void OnSetFocusID12();
	afx_msg void OnSetFocusID13();
	afx_msg void OnSetFocusID14();
	afx_msg void OnSetFocusID15();
	afx_msg void OnSetFocusID16();
	afx_msg void OnInsert();
	afx_msg void OnDelete();
	afx_msg void OnCheck01();
	afx_msg void OnCheck02();
	afx_msg void OnCheck03();
	afx_msg void OnCheck04();
	afx_msg void OnCheck05();
	afx_msg void OnCheck06();
	afx_msg void OnCheck07();
	afx_msg void OnCheck08();
	afx_msg void OnCheck09();
	afx_msg void OnCheck10();
	afx_msg void OnCheck11();
	afx_msg void OnCheck12();
	afx_msg void OnCheck13();
	afx_msg void OnCheck14();
	afx_msg void OnCheck15();
	afx_msg void OnCheck16();
	virtual void OnCancel();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UTABPAGE_H__8BD62420_5C90_11D4_A024_00001CD7F9BE__INCLUDED_)
