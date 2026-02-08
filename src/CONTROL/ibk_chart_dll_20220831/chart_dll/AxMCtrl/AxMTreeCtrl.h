#ifndef __AXMTREECTRL_H__
#define __AXMTREECTRL_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CAxMTreeCtrl window

#define	getBookMark	0
#define	addBookMark	1
#define	delBookMark	2
#define	configIndc	3


class AFX_EXT_CLASS CAxMTreeCtrl : public CTreeCtrl
{
// Construction
public:
	CAxMTreeCtrl();

	void	setImageList(CString path, int cx, int cy);

	//Inserting items into the control
	HTREEITEM InsertGroup(LPCTSTR lpszItem, int nImage, HTREEITEM hParent = TVI_ROOT);
	HTREEITEM InsertCheckBox(LPCTSTR lpszItem, HTREEITEM hParent, BOOL bCheck = TRUE);
	HTREEITEM InsertRadioButton(LPCTSTR lpszItem, HTREEITEM hParent, BOOL bCheck = TRUE);

	//Validation methods
	BOOL IsGroup(HTREEITEM hItem);
	BOOL IsCheckBox(HTREEITEM hItem);
	BOOL IsRadioButton(HTREEITEM hItem);

	//Setting / Getting combo states
	BOOL SetCheckBox(HTREEITEM hItem, BOOL bCheck);
	BOOL GetCheckBox(HTREEITEM hItem, BOOL& bCheck);
	
	//Setting / Getting radio states
	BOOL SetRadioButton(HTREEITEM hParent, int nIndex);
	BOOL SetRadioButton(HTREEITEM hItem);
	BOOL GetRadioButton(HTREEITEM hParent, int& nIndex, HTREEITEM& hCheckItem);
	BOOL GetRadioButton(HTREEITEM hItem, BOOL& bCheck);
	
	//Enable / Disbale items
	BOOL SetGroupEnable(HTREEITEM hItem, BOOL bEnable);
	BOOL SetCheckBoxEnable(HTREEITEM hItem, BOOL bEnable);
	BOOL SetRadioButtonEnable(HTREEITEM hItem, BOOL bEnable);  
	BOOL GetRadioButtonEnable(HTREEITEM hItem, BOOL& bEnable);
	BOOL GetCheckBoxEnable(HTREEITEM hItem, BOOL& bEnable);
	
	void	OnDrag(CPoint point, HTREEITEM hItem);

	// Attributes
public:
	
	// Operations
public:
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAxMTreeCtrl)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PreSubclassWindow();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL
	
	// Implementation
public:
	virtual ~CAxMTreeCtrl();
	
	// Generated message map functions
protected:
	CImageList	m_ilTree;
	
	//{{AFX_MSG(CAxMTreeCtrl)
	afx_msg LRESULT OnSetMessageHandler(WPARAM, LPARAM);
	afx_msg LRESULT OnGetMessageHandler(WPARAM, LPARAM);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);	
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);	
	afx_msg void OnItemexpanded(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	
	DECLARE_DYNAMIC(CAxMTreeCtrl)
	DECLARE_MESSAGE_MAP()
	
	HTREEITEM	m_hitemDrag;	// 최초 잡은 아이템
	HTREEITEM	m_hitemDrop;	// 놓은 아이템
	CImageList*	m_pDragImage;	
	bool		m_bDrag;

	COleDataSource m_COleDataSource;

	CWnd*		m_messageHandler;
	LRESULT		Notify(UINT msg, WPARAM wParam = 0, LPARAM lParam = 0);
public:
	void	SetMessageHandler(CWnd* wnd) { m_messageHandler = wnd; }
	CWnd*	GetMessageHandler() const    { return m_messageHandler; }

	// 2009.11.04 : ADD : 지표 검색 기능 추가
	bool	GetTreeItemTextList(CMapStringToPtr *pList, CString strItemText);
	void    SelectTreeItem(HTREEITEM hItem);
};

//Dialog Data exchange support
void DDX_TreeCheck(CDataExchange* pDX, int nIDC, HTREEITEM hItem, BOOL& bCheck);
void DDX_TreeRadio(CDataExchange* pDX, int nIDC, HTREEITEM hParent, int& nIndex);

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // __AXMTREECTRL_H__
