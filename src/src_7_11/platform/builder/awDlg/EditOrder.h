#if !defined(AFX_EDITORDER_H__DCA860CF_2249_4A8B_914B_53C45133BD6E__INCLUDED_)
#define AFX_EDITORDER_H__DCA860CF_2249_4A8B_914B_53C45133BD6E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditOrder.h : header file
//
#include "../awCommon/FormItem.h"
#include "../Components/EditList.h"
#include "resource.h"
#include "../Components/PopListCtrl.h"
#include "SepWnd.h"
/////////////////////////////////////////////////////////////////////////////
// CEditOrder dialog

class AFX_EXT_CLASS CEditOrder : public CDialog
{
// Construction
public:
	CEditOrder(CWnd* pParent, struct _mapH* mapH, int nKind = 0);   // standard constructor
public:
	struct	_mapH*	m_mapH;
	CFormItem	*m_pFormItem;
	int		m_nKind;
	CPopListCtrl	*m_pPopup;
protected:
	CImageList*	m_pimglstDragImage;
private:
	bool isValidItem(int idx);
	int		m_nItem;
	CStringArray	m_itemArr;
	CSepWnd		*m_pSepWnd;
public:
	void	adjustScroll(int nItem);
	CString	getCellIO(_formR *formR, int idx = -1);
	void	SaveItems();
	CString Parser(CString &srcstr, CString substr);
	void	MoveItems(int nItem);
	int	cellSize(struct _formR *formR, int idx = -1);
	CString	getIO(int idx);
	CString	getDataKind(int type);
	bool	insertItemOfIdx(int idx, bool isInsert = true);
	void	fillList();
	void	setSelected(CListCtrl *ListCtrl, int nItem, BOOL scroll = TRUE);
	void	killSelected(CListCtrl *ListCtrl);
	int	addItem(CListCtrl *LC, CString strItem, int nItem, int iSubItem = -1);
	BOOL	addColumn(CListCtrl *LC, LPCTSTR strItem, int nItem, int nWidth);

	// drag
	CImageList* CreateDragImageSymbol(LPPOINT lpPoint, CListCtrl *ListCtrl);
	void	endDrag(CPoint point);
// Dialog Data
	//{{AFX_DATA(CEditOrder)
	enum { IDD = IDD_EDITORDER };
	CEditList	m_oList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditOrder)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEditOrder)
	virtual BOOL OnInitDialog();
	afx_msg void OnBegindragOldlist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClose();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnUpBtn();
	afx_msg void OnDownBtn();
	virtual void OnOK();
	afx_msg void OnDblclkOldlist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	afx_msg long OnMessage1(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITORDER_H__DCA860CF_2249_4A8B_914B_53C45133BD6E__INCLUDED_)
