#if !defined(AFX_SHAREDDATAPAGE_H__943FE8EC_9C65_467A_805E_C3289FD944E0__INCLUDED_)
#define AFX_SHAREDDATAPAGE_H__943FE8EC_9C65_467A_805E_C3289FD944E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SharedDataPage.h : header file
//

#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CSharedDataPage dialog

interface IPropertyMap;
class CSharedDataPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CSharedDataPage)

// Construction
public:
	void SetProperty( IPropertyMap *p_pIPropertyMap);
	CSharedDataPage();
	~CSharedDataPage();

	// (2004.06.08, 배승원) Global 공유 Property를 추가한다.
	//		Local/Global의 겸용 Routine으로 개선한다.
	void	InitList(CListCtrl* pList, LPCSTR _szData);
	void	MakeShareList(CListCtrl* pList, UINT nID);
	int		UpdateItemData(CListCtrl* pList, CString _szShareName, BOOL _bStart, BOOL _bSet, BOOL _bGet, int _nIndex);
	
	int		AddData(CListCtrl* pList, CString _szData);
	int		AddData(CListCtrl* pList, CString _szShareName, BOOL _bStart, BOOL _bSet, BOOL _bGet);
	void	Refresh(CListCtrl* pList);
	int		GetSelectItem(CListCtrl* pList);


// Dialog Data
	//{{AFX_DATA(CSharedDataPage)
	enum { IDD = IDD_PROPPAGE_SHARED };
	CListCtrl	m_ctrlGShareList;
	CListCtrl	m_ctrlLShareList;
	CString	m_sLocalSharedList;
	CString	m_sGlobalSharedList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CSharedDataPage)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnApply();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CSharedDataPage)
	afx_msg void OnBtnadd();
	afx_msg void OnBtndel();
	afx_msg void OnBtnmodify();
	virtual BOOL OnInitDialog();
	afx_msg void OnGBtnAdd();
	afx_msg void OnGBtnDel();
	afx_msg void OnGBtnModify();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// (2004.06.08, 배승원) Global 공유 Property를 추가한다.
//		Local/Global의 겸용 Routine을 제공한다.
protected:
	void LFnBtnDel(CListCtrl* pList, UINT nID);
	void LFnBtnAdd(CListCtrl* pList, UINT nID);
	void LFnBtnModify(CListCtrl* pList, UINT nID);

// (2006/7/5 - Seung-Won, Bae) Manage Property Map for Update
protected:
	IPropertyMap *	m_pIPropertyMap;

// (2008/1/14 - Seung-Won, Bae) for MultiLanguage.
protected:
	HWND	m_hOcxWnd;
public:
	void	SetOcxHwnd( HWND p_hOcxWnd)		{	m_hOcxWnd = p_hOcxWnd;	}
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHAREDDATAPAGE_H__943FE8EC_9C65_467A_805E_C3289FD944E0__INCLUDED_)
