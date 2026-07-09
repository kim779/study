#if !defined(AFX_BATCH_H__89EA843B_4E42_4D7A_91C9_A5288FA3ECC5__INCLUDED_)
#define AFX_BATCH_H__89EA843B_4E42_4D7A_91C9_A5288FA3ECC5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// batch.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBatch dialog
#include "resource.h"
#include <afxtempl.h>
#include "../mapvar.h"
#include "../awWcc/libwcc.h"
#include "../workio.h"
#include "../mapform.h"
#include "../awCommon/FormItem.h"

class AFX_EXT_CLASS CBatch : public CDialog
{
// Construction
public:
	bool isValidTabOrder(CFormItem *pFormItem, int idx);
	bool isValidIOOrder(CFormItem *pFormItem, int idx);
	void sortDownArray();
	void sortTabOrder();
	void sortIoOrder();
	CString setMapPath(CString mapN);
	CBatch(CWnd* pParent = NULL)	{}	// standard constructor
	CBatch(CWnd* pParent, CString dir);
	virtual	~CBatch();

// Dialog Data
	//{{AFX_DATA(CBatch)
	enum { IDD = IDD_BATCH };
	CListCtrl	m_list;
	CTreeCtrl	m_tree;
	CString	m_status;
	BOOL	m_bExport;
	//}}AFX_DATA

// Attributes
protected:
	HTREEITEM	m_item;

	enum	{ sndINFO=0, sndDOWN, sndUP } m_proc;

	CArray <_mapTree, _mapTree>	m_treeArr;
	CStringArray m_downArray;
	//struct	_rsmH*	m_SRH;
	//struct	_rsmH*	m_RRH;
	//struct	_workIO		m_workIO;

	CString		m_dir;
	CString		m_current;
	CString		m_files;
	CFile		m_file;
	int		m_state, m_mapGroup;
	CString		m_mapDesc;

	bool		m_source;
	int		m_sendL;
	int		m_offset;
	HANDLE		m_hfile;
	class CDownLoad	*m_pDownLoad;
	class CUpLoad	*m_pUpLoad;
	struct	_mapH	m_mapH;

	CMapToken	m_token[MAX_TOKEN_N];
private:
	bool		m_bBatching;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBatch)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HTREEITEM GetItemByName(HTREEITEM hItem, LPCTSTR szItemName);
	void	insert(CString mapName);
	void	loadFile();
	void	goState(int state, CString msg = _T(""));
	BOOL	addColumn(LPCTSTR str, int item, int width);
	BOOL	addItem(int item, int subItem, CString& str);
	bool	open(CString fileN);
	bool	compile();


	// Generated message map functions
	//{{AFX_MSG(CBatch)
	virtual BOOL OnInitDialog();
	afx_msg void OnBatch();
	virtual void OnOK();
	afx_msg void OnTreeDblClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTreeSelChanged(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	afx_msg	LONG OnState(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BATCH_H__89EA843B_4E42_4D7A_91C9_A5288FA3ECC5__INCLUDED_)
