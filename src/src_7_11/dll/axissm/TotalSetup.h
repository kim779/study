#pragma once


// CTotalSetup dialog
#include "resource.h"
#include <afxtempl.h>
#include "../ctrl/fxTreeCtrl.h"
#include "SetupDialog.h"
#include "XListCtrl.h"



#define MAX_SETUP	8

class AFX_EXT_CLASS CTotalSetup : public CDialog
{
//	DECLARE_DYNAMIC(CTotalSetup)

public:
	CTotalSetup(CWnd* pParent = NULL);   // standard constructor
	CTotalSetup(class CTMenu* menu, CString home = _T(""), CString user = _T(""), int index = 0, int tick = 0, bool bpcenable = false, int nWhoi = 1, CWnd* pParent = NULL);   // standard constructor

	virtual ~CTotalSetup();

	BOOL		m_bTickView[2];
	BOOL		m_bScreenBar;
	BOOL		m_bSetupCreate[11];
	bool			m_bpcenable;
	int			m_nWhoi;

	CString		m_home;
	CString		m_user;

	CImageList *m_pImageList;
	CXListCtrl m_list;
	//CListCtrl m_list;
protected:
	int		m_index;
	int		m_tick;

	CImageList	m_imageList;

	class CTMenu*	m_menu;

	CArray<CSetupDialog*, CSetupDialog*> m_dlgs;
	CMap<UINT, UINT&, bool, bool> m_applys;
// Dialog Data
	enum { IDD = IDD_TOTALSETUP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CfxTreeCtrl m_tree;
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnApply();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSelchangedTreemenu(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedDefault();
	afx_msg void OnDestroy();
private:
	void DestroyAllDialogs();
public:
	bool IsApplied(UINT id);
	void InitList(CXListCtrl &list, const TCHAR**pszTitle);
	void ResizeListColumn(CXListCtrl &list, const int *arrWidth);
	afx_msg void OnItemchangedXlist(NMHDR *pNMHDR, LRESULT *pResult);
};
