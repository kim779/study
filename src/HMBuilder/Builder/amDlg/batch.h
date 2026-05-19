#pragma once

#include <afxtempl.h>
#include "resource.h"

#include "../h/workio.h"
#include "../h/mapvar.h"
#include "../h/mapform.H"
#include "../amCc/LibBuild.h"

// CBatch 대화 상자입니다.

class AFX_EXT_CLASS CBatch : public CDialogEx
{
	DECLARE_DYNAMIC(CBatch)

public:
	CBatch(CWnd* pParent = NULL)	{}   // 표준 생성자입니다.
	CBatch(CWnd* pParent, CString dir);
	virtual ~CBatch();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_BATCH };
	CListCtrl	m_list;
	CTreeCtrl	 m_tree;
	BOOL	m_export;
	CString	m_status;

protected:
	HTREEITEM	m_item;

	CArray <_mapTree, _mapTree>	m_treeArr;
	CStringArray	m_downArray;
	CString		m_dir;
	struct	_mapH	m_mapH;
	bool		m_batching;

	//class CDownload* m_download;
	//class CUpload*	m_upload;
	//CMapToken	m_token[MAX_TOKEN_N];

	bool isValidTabOrder(class CFormItem* formItem, int idx);
	bool isValidIOOrder(class CFormItem* formItem, int idx);
	void sortDownArray();
	void sortTabOrder();
	void sortIoOrder();

	HTREEITEM getItemByName(HTREEITEM hItem, LPCTSTR szItemName);
	void	insert(CString mapName);
	void	loadFile();
	void	goState(int state, CString msg = _T(""));
	BOOL	addColumn(LPCTSTR str, int item, int width);
	BOOL	addItem(int item, int subItem, CString& str);
	bool	open(CString fileN);
	bool	compile(CString fileN);

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnBatch();
	afx_msg void OnTreeSelChanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTreeDblClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg LRESULT OnState(WPARAM wParam, LPARAM lParam);
};
