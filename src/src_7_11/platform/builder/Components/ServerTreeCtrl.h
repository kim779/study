#if !defined(AFX_SERVERTREECTRL_H__65F47667_3617_4925_90F4_35FEBDDF7F17__INCLUDED_)
#define AFX_SERVERTREECTRL_H__65F47667_3617_4925_90F4_35FEBDDF7F17__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ServerTreeCtrl.h : header file
//

#include "../workio.h"
#include "ToolTipListCtrl.h"
#include "../awUser/DownLoad.h"
/////////////////////////////////////////////////////////////////////////////
// CServerTreeCtrl window
#define	R_NONE		-1

class CServerTreeCtrl : public CTreeCtrl
{
// Construction
public:
	CServerTreeCtrl();

// Attributes
public:
	CArray	< HTREEITEM, HTREEITEM & > m_arItem;

public:
	HTREEITEM	m_firstItem;
	CString		m_name, m_status, m_path, m_dir, m_errMsg;
	int		m_state, m_type, m_index, m_count, m_child;
	bool		m_finish, m_request, m_enable, m_bOpen;
	CFile		m_file;
	int		m_nTwoDepth;

	CStringArray	m_files;
	CByteArray	m_types;


	CArray <_mapTree, _mapTree>	m_treeArr;

	HTREEITEM	m_item;
	CImageList*	m_pilDrag;
	HTREEITEM	m_hItemDrag, m_hItemDrop;
	CComboBox	*m_pCombo;

	HCURSOR		m_DropCopyCursor;
	HCURSOR		m_NoDropCopyCursor;
	HCURSOR		m_DropMoveCursor;
	HCURSOR		m_NoDropMoveCursor;
protected:
	CPoint		m_dropPos, m_MousePoint;
	bool		m_bTemplate;
	CDownLoad	*m_pDownLoad;
	CCriticalSection	m_section;
private:
	TCHAR* m_pchTip ;
	WCHAR* m_pwchTip;
	CMapStringToPtr	m_itemArr1, m_itemArr2;

// Operations
public:
	int	GetSelectedItemCount();
	void	ClearSelection();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CServerTreeCtrl)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	void Initialize(CString dir,  CString path, int child, bool request, bool bTemplate = false);
	virtual ~CServerTreeCtrl();

public:
	void InitTree();
	void		OpenMap(CString mapName);
	CString		getNameOfIdx(int idx);
	void		loadTemplate(CPoint pnt);
	void		AddFiles(CString name);
	void		OpenMap(int idx = -1);
	void		setFileName(int idx);
	BOOL		addItem(CToolTipListCtrl *LC, int item, int subItem, CString str, int data = 0, CString tipText = "");
	void		fillList(CToolTipListCtrl *LC, int idx);
	void		fillCombo(CComboBox *CBO);
	BOOL		OnBeginDrag(NMHDR* pNMHDR, LRESULT* pResult);
	void		insert(CString mapName, CString mapDesc, int idx);
	CString		setMapPath(CString mapN);
	bool		openList(CComboBox* CBO);
	HTREEITEM	getSelectedItem(int index);
	void		removeAll();
	void		loadFile();

	// Generated message map functions
protected:
	bool	selectItems(HTREEITEM hitemFROM, HTREEITEM hitemTO);
	HTREEITEM GetItemByName(HTREEITEM hItem, LPCTSTR szItemName);
	HTREEITEM getFirstSelectedItem();
	HTREEITEM getNextSelectedItem(HTREEITEM hitem);
	HTREEITEM getPrevSelectedItem(HTREEITEM hitem);
	//{{AFX_MSG(CServerTreeCtrl)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnKeydown(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnItemexpanded(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	virtual int OnToolHitTest(CPoint point, TOOLINFO * pTI) const;
	afx_msg BOOL OnToolTipText( UINT id, NMHDR * pNMHDR, LRESULT * pResult );
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVERTREECTRL_H__65F47667_3617_4925_90F4_35FEBDDF7F17__INCLUDED_)
