
/////////////////////////////// Defines ///////////////////////////////////////
#pragma once


//The actual tree options control class
class CTreeOptionsCtrl : public CTreeCtrl
{
public:
	//Constructors / Destructors
	CTreeOptionsCtrl();
	
	CArray<HTREEITEM,HTREEITEM>  m_selItem;

	//Misc
	void SetImageListToUse(UINT nResourceID) { m_nilID = nResourceID; };
	
	//Inserting items into the control
	HTREEITEM	InsertGroup(LPCTSTR lpszItem, int nImage, int nSelImage, HTREEITEM hParent = TVI_ROOT);
	HTREEITEM	InsertChild(LPCTSTR lpszItem, int nImage, int nSelImage, HTREEITEM hParent);
	BOOL		IsGroup(HTREEITEM hItem);
	void		setImage(CBitmap* pBitmap);
	void		SetChildCheck(HTREEITEM parentItem);
	void		AppendChecked(HTREEITEM hItem);

	void		UnselectCurrentItem( void );

protected:
	CImageList m_ilTree;
	UINT m_nilID;
	CWnd	*m_pParent;
	HTREEITEM m_hPrev;
	bool	m_bFirst;
	
	void DestroyOldChildControl();
	
	//{{AFX_VIRTUAL(CTreeOptionsCtrl)
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL
	
	//{{AFX_MSG(CTreeOptionsCtrl)
	afx_msg void OnDestroy();
	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	
	DECLARE_DYNAMIC(CTreeOptionsCtrl)
		
	DECLARE_MESSAGE_MAP()
};

//#endif //__TREEOPTIONSCTRL_H__


