#pragma once

#include "resource.h"
#include "editListCtrl.h"

// CEditOrder 대화 상자입니다.

class AFX_EXT_CLASS CEditOrder : public CDialogEx
{
	DECLARE_DYNAMIC(CEditOrder)

public:
	CEditOrder(CWnd* pParent, struct _mapH* mapH, int iKind = 0);   // 표준 생성자입니다.
	virtual ~CEditOrder();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_EDITORDER };
	CEditListCtrl	m_oList;

	class	CFormItem*	m_formItem;

protected:
	struct	_mapH*	m_mapH;
	class	CPopListCtrl*	m_popup;
	CImageList*	m_pimglstDragImage;

	void	adjustScroll(int nItem);
	CString	getCellIO(struct _formR *formR, int idx = -1);
	int	cellSize(struct _formR *formR, int idx = -1);
	CString	getIO(int iok);
	CString	getDataKind(int type);
	bool	insertItemOfIdx(int idx, bool isInsert = true);
	void	fillList();

	void	setSelected(CListCtrl* LC, int nItem, BOOL scroll = TRUE);
	void	killSelected(CListCtrl* LC);

	int	addItem(CListCtrl* LC, CString strItem, int nItem, int iSubItem = -1);
	BOOL	addColumn(CListCtrl* LC, LPCTSTR strItem, int nItem, int nWidth);

	void	saveItems();
	void	moveItems(int nItem);

	CImageList* createDragImageSymbol(LPPOINT lpPoint, CListCtrl *ListCtrl);
	void	endDrag(CPoint point);

	CString parse(CString& src, CString sub);

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

private:
	int	m_DlgKind;
	int	m_item;
	CStringArray	m_itemArr;
	class	CSepWnd*	m_sepWnd;

	bool	isValidItem(int idx);

public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDestroy();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnBegindragOldlist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDblclkOldlist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDownBtn();
	afx_msg void OnUpBtn();

protected:
	afx_msg LRESULT OnMessage(WPARAM wParam, LPARAM lParam);
};
