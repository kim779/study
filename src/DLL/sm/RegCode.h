#if !defined(AFX_REGCODE_H__9684BBB1_32DE_4AF1_B1E9_E11A4499BF02__INCLUDED_)
#define AFX_REGCODE_H__9684BBB1_32DE_4AF1_B1E9_E11A4499BF02__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RegCode.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRegCode dialog
#include "resource.h"
#include <afxtempl.h>
#include "../ctrl/ExTreeCtrl.h"

#define CFCODE_BASE		1000
#define	GROUP_BASE		2000
#define THEME_BASE		3000
#define INGRE_BASE		4000

class _stockcode
{
public:
	CString	Name;		// code Name
	CString	Code;		// code Symbol
	int	Catagory{};	// catagory kind
	char	market{};		// market kind
};

struct _regCode
{
	char	code[12];
	char	name[40];
};

class CRegCode : public CDialog
{
// Construction
public:
	CRegCode(CWnd* pParent = NULL);   // standard constructor
	~CRegCode();

// Dialog Data
	//{{AFX_DATA(CRegCode)
	enum { IDD = IDD_REGISTER_CODE };
	CListCtrl	m_selectList;
	CListCtrl	m_codeList;
	CExTreeCtrl	m_tree;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRegCode)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	CMapStringToString	m_mapCodeToSelect;
	CMapStringToString	m_arCodeByName;
	CArray < class _stockcode*, class _stockcode* > m_arCode;

	void	Init();
	void	initList();
	void	loadInfo();
	void	saveInfo();
	void	LoadSCODE();
	void	LoadCFCODE();
	void	LoadOTHER(char kind);
	void	GetCodeAry(CStringArray& ary, CString dat);

	HTREEITEM addItem(CString itemS, DWORD dwData = 0, HTREEITEM hParent = NULL, HTREEITEM hInsertAfter = NULL);
	HTREEITEM addItemEx(CString itemS, CString string = _T(""), DWORD dwData = 0, HTREEITEM hParent = NULL, HTREEITEM hInsertAfter = NULL);
	BOOL	addColumn(CListCtrl& list, LPCTSTR columns, int col, int width = 0);
	BOOL	addListItem(CListCtrl& list, CString items, int item, int subi = 0);

	void	fit(CListCtrl& list);
	void	setCode(char* dat, DWORD dw);

	void	InsertItem();
	void	DeleteItem();
	void	InsertAll();
	void	DeleteAll();
	bool	IsDuplication(CString Code);
	// Generated message map functions
	//{{AFX_MSG(CRegCode)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangedCategorytree(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REGCODE_H__9684BBB1_32DE_4AF1_B1E9_E11A4499BF02__INCLUDED_)
