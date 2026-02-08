#if !defined(AFX_TOOLANDRBUTTONGRID_LISTCTRL_H__6E1C5731_9579_4E68_A798_F9433CCCC117__INCLUDED_)
#define AFX_TOOLANDRBUTTONGRID_LISTCTRL_H__6E1C5731_9579_4E68_A798_F9433CCCC117__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ToolandRButtonGrid_ListCtrl.h : header file
//
#include "SuperGridCtrl.h"
#include "ToolandRButtonBase_ListCtrl.h"
#include "NumericalInquiryOptionDlg.h"	// ADD: 종찬(04/03/17) 수치조회관련
/////////////////////////////////////////////////////////////////////////////
// CToolandRButtonGrid_ListCtrl window

class CToolandRButtonItemMgr;
class CToolandRButtonGrid_ListCtrl : public CSuperGridCtrl
{
// Construction
public:
	CToolandRButtonGrid_ListCtrl();

// Attributes
public:
	//HOWTO:
	void InitializeGrid(void);
	CImageList *CreateDragImageEx(int nItem);
	BOOL m_bDrag;
	// Overrides
	void OnControlLButtonDown(UINT nFlags, CPoint point, LVHITTESTINFO& ht);	
	void OnUpdateListViewItem(CTreeItem* lpItem, LV_ITEM *plvItem);
	CItemInfo* CopyData(CItemInfo* lpSrc);
	int GetIcon(const CTreeItem* pItem);
	COLORREF GetCellRGB(void);
	BOOL OnItemExpanding(CTreeItem *pItem, int iItem);
	BOOL OnItemExpanded(CTreeItem* pItem, int iItem);
	BOOL OnCollapsing(CTreeItem *pItem);
	BOOL OnItemCollapsed(CTreeItem *pItem);
	BOOL OnDeleteItem(CTreeItem* pItem, int nIndex);
	BOOL OnVkReturn(void);
	BOOL OnItemLButtonDown(LVHITTESTINFO& ht);

	//list에 설정되어 있는 값을 얻는다.
	CString GetListItemInfo();
// Operations
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolandRButtonGrid_ListCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CToolandRButtonGrid_ListCtrl();

	//이전에 저장되어 있는 Item의 정보를 다시 listctrl에 적용시칸다.
	void InsertSavedItemInfo(const CString &strItemName, const CString &strLimit1, const CString &strLimit2);
	//모든 Item의 정보를 가지고 컨트롤하는 객체의 포인터를 Setting한다.
	void SetListItemInfoMgrObject(CToolandRButtonItemMgr *pItemInfoMgr, LPCTSTR pcszTabName, CNumericalInquiryOptionDlg* pNumericalInquiryOptionDlg);	// MODIFY: 종찬(04/03/17) 수치조회관련 옵션.
	//draganddrop을 지원하기 위해서 호출한다.
	void InitDropTarget();

protected:
	CImageList m_image;//Must provide an imagelist
	//positions and creates/initalize a combobox control
	CComboBox* ShowList(int nItem, int nCol, CStringList *lstItems);
	//helper function called from ShowList...calcs the lists max horz extent
	int CalcHorzExtent(CWnd* pWnd, CStringList *pList);

	// Generated message map functions
protected:
	//{{AFX_MSG(CToolandRButtonGrid_ListCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeydown(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	LONG OnDragChartData(UINT wParam, LPARAM lParam);
	LONG OnDropChartData(UINT wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
private:
	void SetComboListWhen_Extension_Recuction_Item(const int nItem, const int nColumn, CStringList *lstItems);
	//ListCtrl의 맨 윗부분.
	CTreeItem* m_pRootItem;
	//모든 Item의 정보를 가지고 컨트롤하는 객체의 포인터이다.
	CToolandRButtonItemMgr* m_pItemInfoManager;

	//listctrl의 맨 윗뿌리를 Setting한다.
	void InsertRoot();
	//칼럼헤더를 Insert한다.
	void InsertColumnHeader();
	//CItemInfo에 정보를 저장한다.
	void SetRootItem(const CString& szItemName, CItemInfo *lpItemInfo);

	//DragandDrop을 위한 멤버변수이다.
	COleListCtrlDropTarget m_dropTarget;

	// ADD: 종찬(04/03/17) 수치조회관련. 내부툴바 탭페이지와 수치조회탭페이지의 옵션값 연동을 위해서
	CString m_strTabName;
	CNumericalInquiryOptionDlg* m_pNumericalInquiryOptionDlg;

// (2008/1/14 - Seung-Won, Bae) for MultiLanguage.
protected:
	HWND	m_hOcxWnd;
public:
	void	SetOcxHwnd( HWND p_hOcxWnd)		{	m_hOcxWnd = p_hOcxWnd;	}
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLANDRBUTTONGRID_LISTCTRL_H__6E1C5731_9579_4E68_A798_F9433CCCC117__INCLUDED_)
