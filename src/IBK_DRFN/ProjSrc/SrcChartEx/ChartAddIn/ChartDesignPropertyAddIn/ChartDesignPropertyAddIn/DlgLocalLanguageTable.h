#if !defined(AFX_DLGLOCALLANGUAGETABLE_H__1C219629_D131_4401_B037_D2EC7742FD5B__INCLUDED_)
#define AFX_DLGLOCALLANGUAGETABLE_H__1C219629_D131_4401_B037_D2EC7742FD5B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgLocalLanguageTable.h : header file
//

#include "EditableListCtrl.h"		// for CEditableListCtrl

/////////////////////////////////////////////////////////////////////////////
// CDlgLocalLanguageTable dialog

interface IPropertyMap;
class CDlgLocalLanguageTable : public CDialog
{
// Construction
public:
	CDlgLocalLanguageTable(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgLocalLanguageTable)
	enum { IDD = IDD_LOCAL_LANGUAGE_TABLE };
	CEditableListCtrl	m_lstLanguageTable;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgLocalLanguageTable)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgLocalLanguageTable)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnBtnDelete();
	afx_msg void OnBtnAdd();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnBtnRedefineGlobal();
	afx_msg void OnSelendokCmbGlobal();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// (2006/7/5 - Seung-Won, Bae) Use IPropertyMap
protected:
	IPropertyMap *	m_pIPropertyMap;
	CString			m_strLocalLanguageTable;
public:
	void SetProperty( IPropertyMap *p_pIPropertyMap);

// (2008/1/17 - Seung-Won, Bae) for Static Local Meta Data
protected:
	CStringList		m_lstStaticMetaData;

// (2008/2/24 - Seung-Won, Bae) Limit Size
protected:
	CPoint			m_ptMinSize;

// (2008/2/24 - Seung-Won, Bae) for Current Language ID
protected:
	IMetaTable::E_LANGUAGEID	m_eLanguageID;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGLOCALLANGUAGETABLE_H__1C219629_D131_4401_B037_D2EC7742FD5B__INCLUDED_)
