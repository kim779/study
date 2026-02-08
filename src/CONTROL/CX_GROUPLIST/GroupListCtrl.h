#pragma once
// GroupListCtrl.h : header file
//

#include "GroupHeaderCtrl.h"

#define WM_ITEM_CLICK			WM_USER+938

#define GROUP_STATE_EXPAND		111
#define GROUP_STATE_SHRINK		112
#define ITEM_STATE_VISIBLE		113
#define ITEM_STATE_HIDE			114

#define TYPE_ITEM			133
#define TYPE_GROUP			134

typedef struct _group_list_item_
{
	int	type=0;			// Group or Item
	COLORREF clrText=0;	
	int	state=0;		// Group 일 때 펼쳐진 상태 인지 
	CString id;			// 속 한 group id, group 이면 gid
	CString gid;
	CString data;			// data
	CStringArray text;		// column text
} sGLI;

/////////////////////////////////////////////////////////////////////////////
// CGroupListCtrl window

#include <afxtempl.h>

class CGroupListCtrl : public CListCtrl
{
// Construction
public:
	CGroupListCtrl();

// Attributes
protected:
	BOOL	m_bFont;
	CFont	m_fontNormal;
	CFont   m_fontBold;

	CString m_strSelectCode;
	CGroupHeaderCtrl	m_header;
	CImageList		m_imageList;
	CList <sGLI*, sGLI*>	m_list;

// Operations
public:
	int	InsertColumn(int nCol, LPCTSTR lpszColumnHeading, int nFormat = LVCFMT_LEFT, int nWidth = -1, int nSubItem = -1);
	int	InsertGroup(LPCTSTR lszItem, CString id, CString strData = "");
	int	InsertItem(LPCTSTR lszItem, CString gid, CString id, COLORREF clr, CString strData = "");
	void	DeleteItem(sGLI* p);

	sGLI*	FindGroup(CString gid);
	sGLI*	FindItem(CString gid, CString id, CString jucode);
	int	GetItemIdx(sGLI* p);

	void	GetGroupGLI(sGLI* pg, CList<sGLI*,sGLI*>& list);
	sGLI*	GetGroupGLI(CString gid);
	int	GetGroupItemCount(CString gid);

	BOOL	SetItemText(int nItem, int nSubItem, LPCTSTR lpszText);	
	void	MakeImageList(void);	

	BOOL	DeleteAllItems();
	void	ExpandAllItems(BOOL bExpand);

	void	SortGroup(sGLI* pg);
	CString GetSelectedCode(void)	{ return m_strSelectCode; }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGroupListCtrl)
protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGroupListCtrl();

	// Generated message map functions
protected:
	sGLI*	GetItemGLI(int index);
	void	DoExpand(sGLI* p);
	void	DoShrink(sGLI* p);
	void	OnDoGroupDbclk(sGLI* p, int nItem);
	void	ReleaseData(void);

	//{{AFX_MSG(CGroupListCtrl)
	afx_msg void OnCustomDrawList (NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);	
	afx_msg void OnDestroy();
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetdispinfo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSetdispinfo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT OnSetFont(WPARAM wParam, LPARAM);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};