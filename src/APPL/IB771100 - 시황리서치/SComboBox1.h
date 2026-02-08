#pragma once
// SComboBox1.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSComboBox window

#include "SEdit.h"
class CSComboBox : public CComboBox
{
// Construction
public:
	CSComboBox();
	void InitEdit();
	CString GetEditText(){
		CString str = "";
		if( m_pedSearch){
			m_pedSearch->GetWindowText(str);
		}
		return str;
	}
	inline void SetParentWnd(CMapDlg* mapdlg){m_pMapDlg = mapdlg;}
	
// Attributes
public:
	class CMapDlg* m_pMapDlg;
	std::unique_ptr<CSEdit>m_pedSearch;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSComboBox)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSComboBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSComboBox)
	afx_msg void OnSetfocus();
	afx_msg void OnKillfocus();
	afx_msg void OnDropdown();
	afx_msg void OnSelchange();
	afx_msg void OnSelendok();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

