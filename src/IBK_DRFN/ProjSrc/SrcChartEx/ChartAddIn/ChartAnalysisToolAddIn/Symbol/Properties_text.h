#if !defined(AFX_PROPERTIES_TEXT_H__A34D3BC5_78F6_4C8A_A76B_EDE910EB9048__INCLUDED_)
#define AFX_PROPERTIES_TEXT_H__A34D3BC5_78F6_4C8A_A76B_EDE910EB9048__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Properties_text.h : header file
//

#include "resource.h"
#include "SymbolBaseData.h"
#include "PreView.h"
/////////////////////////////////////////////////////////////////////////////
// CProperties_text dialog
class CPropertiesData_text;
class CProperties_text : public CDialog
{
// Construction
public:
	CString GetModifyText();
	CProperties_text(CPropertiesData_text* pProperties, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CProperties_text)
	enum { IDD = IDD_PROPERTIES_TEXT };
	CEdit	m_Text_Edit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProperties_text)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CProperties_text)
	afx_msg void OnFont();
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeText();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CPropertiesData_text* m_pProperties;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTIES_TEXT_H__A34D3BC5_78F6_4C8A_A76B_EDE910EB9048__INCLUDED_)
