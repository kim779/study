#pragma once
// KeywordConDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CKeywordConDlg dialog
#include "ColorButton.h"

class CMarketDlg;

class CKeywordConDlg : public CDialog
{
// Construction
public:
	CKeywordConDlg(CWnd* pParent = NULL);   // standard constructor

	CAListArray			m_arKeyList;
	CUIntArray			m_arStepColor;

	CMarketDlg* m_pMarketDlg;
// Dialog Data
	//{{AFX_DATA(CKeywordConDlg)
	enum { IDD = IDD_KEYWORDSDIALOG };
	CComboBox	m_cboAdd;
	CComboBox	m_cboStp;
	CListBox	m_stepList;
	CListBox	m_KeyList;
	CColorButton	m_btColor;
	CString	m_strKey;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKeywordConDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CKeywordConDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonDel();
	afx_msg void OnSelchangeStepcombo();
	afx_msg void OnOkbutton();
	afx_msg void OnCancelbutton();
	afx_msg void OnButtonEdit();
	afx_msg void OnSelchangeKeylist();
	afx_msg void OnSelchangeSteplist();
	//}}AFX_MSG
	afx_msg LONG OnChangeColorPicker(UINT lParam, LONG wParam);
	DECLARE_MESSAGE_MAP()
};

