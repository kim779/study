#if !defined(AFX_DOWNDLG_H__B7181561_4694_11D4_A024_00001CD7F9BE__INCLUDED_)
#define AFX_DOWNDLG_H__B7181561_4694_11D4_A024_00001CD7F9BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// downDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDownDlg dialog
//
#include "resource.h"
#include <afxtempl.h>
#include "xtreectrl.h"

class	CfileSET
{
public:
	CString		m_name;
	CStringArray	m_arFILE;
};

class	CvirtualSET
{
public:
	CString		m_name;
	CArray <CfileSET *, CfileSET *> m_arVIR;
};

class	CsubSET
{
public:
	CString		m_name;
	CArray <CvirtualSET *, CvirtualSET *> m_arSUB;
};

class AFX_EXT_CLASS CDownDlg : public CDialog
{
// Construction
public:
	CDownDlg(CWnd* pParent = NULL)	{}	// standard constructor
	CDownDlg(class CMainFrame* frame, CString dir, CString path, int child, bool request = false);
	~CDownDlg();

// Dialog Data
	//{{AFX_DATA(CDownDlg)
	enum { IDD = IDD_DOWN };
	CString		m_status;
	CString		m_name;
	//}}AFX_DATA

	int		m_count;
	int		m_type;
	CString		m_path;
	CXTreeCtrl	m_tree;
	CStringArray	m_files;
	CByteArray	m_types;	
	int		m_state;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDownDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CMainFrame*	m_mainFRM;
	class	CwSock*	m_wsock;
	bool		m_enable;
	bool		m_finish;
	struct	_rsmH*	m_SRH;
	struct	_rsmH*	m_RRH;

	int		m_child;
	int		m_index;
	bool		m_request;
	CString		m_dir;
	CFile		m_file;
	CButton*	m_kind[2];

	CArray <CsubSET *, CsubSET *> m_arMAP;
	CArray <CsubSET *, CsubSET *> m_arUOB;
	HTREEITEM	m_item;

protected:
	HTREEITEM getSelectedItem(int index);
	bool	sendData(bool req = true);
	void	finish(bool add = true);
	void	removeAll();
	void	loadFile();
	void	load(int type, CString dir);
	void	parse(char* buf, int bufL, CStringArray& arr, CString section = _T(""), char ch = NULL);
	void	insert(int type, HTREEITEM hitem);
	void	goState(int state, CString msg = _T(""));
	bool	checkDuplicate(CString fileN, int type);

	// Generated message map functions
	//{{AFX_MSG(CDownDlg)
	virtual void OnCancel();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnDblClkTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelChangedTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSetFocusName();
	afx_msg void OnReq();
	afx_msg void OnMAP();
	afx_msg void OnUSER();
	//}}AFX_MSG
	afx_msg	LONG OnState(WPARAM wParam, LPARAM lParam);
	afx_msg	LONG OnReadData(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DOWNDLG_H__B7181561_4694_11D4_A024_00001CD7F9BE__INCLUDED_)
