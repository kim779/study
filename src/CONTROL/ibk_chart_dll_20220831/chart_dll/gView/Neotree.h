#if !defined(AFX_NEOTREE_H__77F0F5CE_3BA1_4E56_9810_6D181509824E__INCLUDED_)
#define AFX_NEOTREE_H__77F0F5CE_3BA1_4E56_9810_6D181509824E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Neotree.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNeotree window

#define	getBookMark	0
#define	addBookMark	1
#define	delBookMark	2
#define	configIndc	3

class CNeotree : public CTreeCtrl
{
// Construction
public:
	CNeotree();

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

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNeotree)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PreSubclassWindow();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CNeotree();

	// Generated message map functions
protected:
	CImageList	m_ilTree;

	//{{AFX_MSG(CNeotree)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);	
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);	
	afx_msg void OnItemexpanded(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_DYNAMIC(CNeotree)
	DECLARE_MESSAGE_MAP()

	HTREEITEM	m_hitemDrag;	// 최초 잡은 아이템
	HTREEITEM	m_hitemDrop;	// 놓은 아이템
	CImageList*	m_pDragImage;	
	bool		m_bDrag;
};

//Dialog Data exchange support
void DDX_TreeCheck(CDataExchange* pDX, int nIDC, HTREEITEM hItem, BOOL& bCheck);
void DDX_TreeRadio(CDataExchange* pDX, int nIDC, HTREEITEM hParent, int& nIndex);

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEOTREE_H__77F0F5CE_3BA1_4E56_9810_6D181509824E__INCLUDED_)
