#pragma once
// setup.h : header file
//

#include "treectrl.h"
#include "listctrl.h"
#include <afxtempl.h>

#define	S_FILE		"u_config.ini"

/////////////////////////////////////////////////////////////////////////////
// Csetup dialog

class Csetup : public CDialog
{
// Construction
public:
	Csetup(CWnd* pParent = NULL, CString root = _T(""), CString user = _T(""), CWnd* pWnd = NULL);   // standard constructor
	virtual	~Csetup();

// Dialog Data
	//{{AFX_DATA(Csetup)
	enum { IDD = IDD_SETUP };
	CEdit	m_editVol;
	ClistCtrl	m_list;
	CtreeCtrl	m_tree;
	//}}AFX_DATA

private:
	CWnd* m_parent{};
	DWORD	m_flags[4]{};
	CString	m_state;
	CString	m_file;
	CString	m_sound;
	BOOL	m_chkVol{};		// 거래량
	CString	m_vol;
	CString	m_gubn;			// 거래소구분
	CMap	< DWORD, DWORD, class Citem*, Citem* > m_items;
	//std::map< DWORD, DWORD, class Citem*, Citem* > m_items;
	std::vector<std::unique_ptr<Citem>> _vData;

	CImageList	m_ilist;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Csetup)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:

protected:
	void	loadfile();
	int	hexs2int(CString hexs);
	BOOL	addColumn(CString heads, int col, int width);
	BOOL	addItem(CString items, int item, int subi);
	
	// Generated message map functions
	//{{AFX_MSG(Csetup)
	virtual BOOL OnInitDialog();
	afx_msg void OnAdd();
	afx_msg void OnAddAll();
	afx_msg void OnDel();
	afx_msg void OnDelAll();
	afx_msg void OnBeep();
	afx_msg void OnFile();
	virtual void OnOK();
	afx_msg void OnDestroy();
	afx_msg void OnDblclkTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChkvol();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

