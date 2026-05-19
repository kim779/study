#pragma once


// CFileTree

class CFileTree : public CMFCShellTreeCtrl
{
	DECLARE_DYNAMIC(CFileTree)

public:
	CFileTree();
	virtual ~CFileTree();

	bool	m_mapOnly;

	virtual HRESULT EnumObjects(HTREEITEM hParentItem, LPSHELLFOLDER pParentFolder, LPITEMIDLIST pidlParent);
	BOOL	GetName(LPSHELLFOLDER lpsf, LPITEMIDLIST lpi, DWORD dwFlags, CString& strName);
	void	SetMapOnly();
	BOOL	GetItemPathName(CString& strPath, HTREEITEM htreeItem);
	CString	GetPath(LPAFX_SHELLITEMINFO pItem);

protected:
	bool	checkMapOnly(CString path);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
};


