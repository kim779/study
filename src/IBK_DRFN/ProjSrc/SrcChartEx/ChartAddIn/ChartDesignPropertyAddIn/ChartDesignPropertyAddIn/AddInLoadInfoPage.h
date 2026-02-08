#if !defined(AFX_ADDINLOADINFOPAGE_H__1DEC0487_67F0_4879_87B5_7115077EE541__INCLUDED_)
#define AFX_ADDINLOADINFOPAGE_H__1DEC0487_67F0_4879_87B5_7115077EE541__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AddInLoadInfoPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAddInLoadInfoPage dialog

interface IPropertyMap;
class CAddInLoadInfoPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CAddInLoadInfoPage)

// Construction
public:
	CAddInLoadInfoPage();
	~CAddInLoadInfoPage();

// Dialog Data
	//{{AFX_DATA(CAddInLoadInfoPage)
	enum { IDD = IDD_PROPPAGE_ADDINLOAD };
	CStatic	m_stcComment;
	CListCtrl	m_lstAddInItem;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CAddInLoadInfoPage)
	public:
	virtual BOOL OnApply();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CAddInLoadInfoPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnSelectAll();
	afx_msg void OnBtnClearAll();
	afx_msg void OnBtnResetUndo();
	afx_msg void OnBtnResetDefault();
	afx_msg void OnItemchangedLstAddin(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


// (2006/7/5 - Seung-Won, Bae) Manage Property Map for Update
protected:
	IPropertyMap *	m_pIPropertyMap;
public:
	void SetProperty( IPropertyMap *p_pIPropertyMap);

// (2006/8/24 - Seung-Won, Bae) Manage Property Value for Initializing
protected:
	CString				m_strAddInNameList;
	CMapStringToString	m_mapAddInLoadString;
protected:
	void		SetAddInNameListWith( const char *p_szAddInNameList);
	BOOL		AddInNameListParse( char *&szItem, char *&szItemNext, int *pnItemLength = NULL, int *pnItemOver = NULL);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADDINLOADINFOPAGE_H__1DEC0487_67F0_4879_87B5_7115077EE541__INCLUDED_)
