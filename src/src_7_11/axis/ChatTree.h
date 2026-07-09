#pragma once


// CChatTree


struct _mapTree{
	char	mgub[4];	/* map category ±¸ºÐÀÚ			*/
	char	fil1;		/* filler				*/
	char	mapd[20];	/* map desc				*/
	char	fil2;		/* filler				*/
	char	mapn[8];	/* map name				*/
	char	fil3;		/* filler				*/
};

class CUserTree 
{
public:
	CString m_mgub;
	CString m_desc;
	CString m_name;
	CString m_ckgb;
	CString m_ckid;
};

#define	R_NONE		-1
class CChatTree : public CTreeCtrl
{
	DECLARE_DYNAMIC(CChatTree)

public:
	CChatTree();
	virtual ~CChatTree();

public:
	CArray      <CUserTree*, CUserTree*> m_userTree;
	CArray	< HTREEITEM, HTREEITEM & > m_arItem;
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
	CCriticalSection	m_section;
private:
	TCHAR* m_pchTip ;
	WCHAR* m_pwchTip;
	CMapStringToPtr	m_itemArr1, m_itemArr2;
	CMapStringToPtr	m_itemMap1;
	CMapStringToPtr	m_itemMap2;

public:
	void  InitTree();
	int	GetSelectedItemCount();
	void	ClearSelection();

protected:
	DECLARE_MESSAGE_MAP()
	virtual void PreSubclassWindow();

public:
	void Initialize();

	void OpenMap(CString mapName);
	CString getNameofIdx(int idx);
	void		loadTemplate(CPoint pnt);
	void		AddFiles(CString name);
	void		OpenMap(int idx = -1);
	void		setFileName(int idx);
	//BOOL		addItem(CToolTipListCtrl *LC, int item, int subItem, CString str, int data = 0, CString tipText = "");
	//void		fillList(CToolTipListCtrl *LC, int idx);
	void		fillCombo(CComboBox *CBO);
	BOOL		OnBeginDrag(NMHDR* pNMHDR, LRESULT* pResult);
	void		insert(CString strgubn, CString strname, int idx);
	CString		setMapPath(CString mapN);
	bool		openList(CComboBox* CBO);
	HTREEITEM	getSelectedItem(int index);
	void		removeAll();
	void		LoadFile(CString path = _T(""));
	CString	Parser(CString &srcstr, CString substr);

	// Generated message map functions
protected:
	bool	selectItems(HTREEITEM hitemFROM, HTREEITEM hitemTO);
	HTREEITEM GetItemByName(HTREEITEM hItem, LPCTSTR szItemName);
	HTREEITEM getFirstSelectedItem();
	HTREEITEM getNextSelectedItem(HTREEITEM hitem);
	HTREEITEM getPrevSelectedItem(HTREEITEM hitem);

public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnTvnItemexpanded(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnKeydown(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);
};


