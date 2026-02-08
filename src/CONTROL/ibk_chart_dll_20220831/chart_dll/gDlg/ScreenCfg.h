#if !defined(AFX_SCREENCFG_H__F76B4300_6116_4AAC_BC6A_E1217B5B1418__INCLUDED_)
#define AFX_SCREENCFG_H__F76B4300_6116_4AAC_BC6A_E1217B5B1418__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "resource.h"
#include "controlex.h"

class CScreenCfg : public CDialog
{
public:
	CScreenCfg(CWnd* pMain, COLORREF tkCOLOR, char* envinfo, char* ginfo, char* scrinfo, char** pOutput, CWnd* pParent = NULL);
	void SaveInfo();	
	char*	GetData() { return *m_pOutput; }	
	
	//{{AFX_DATA(CScreenCfg)
	enum { IDD = IDD_SCREEN_CONFIG };
	CAxListCtrl	m_listPrice;	// 가격지표
	CAxListCtrl	m_listAid;	// 보조지표
	CScrTreeCtrl	m_tree;	
	
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CScreenCfg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CScreenCfg)
	afx_msg void OnBtRgnAdd();
	afx_msg void OnBtRgnDel();
	afx_msg void OnBtLeft();
	afx_msg void OnBtRight();
	afx_msg void OnBtUp();
	afx_msg void OnBtDown();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnSetfocusList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSetfocusList2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkList2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBegindragTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);	
	afx_msg void OnBegindragList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBegindragList2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT OnManage(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CImageList* createDragImageEx(CListCtrl *pList, LPPOINT lpPoint);
	void AddToRight(HTREEITEM hItem);
	void DeleteItem(HTREEITEM hItem);
	void DeleteRgn(HTREEITEM hItem);
	HTREEITEM GetMainItem();

	char**		m_pOutput;
	CWnd*		m_pMain;	
	HTREEITEM	m_hitemDrag;
	HTREEITEM	m_hitemDrop;
	int		m_nCurDrag;	
	char*		m_pScrInfo;
	char*		m_pEnv;
	char*		m_pIndi;
	COLORREF	m_tkCOLOR;
	
	CMap<int, int, struct _graph*, struct _graph*>	m_mapInfo;
	CArray<int, int>				m_arKind;

	CImageList*	m_pDragImage;
	CImageList	m_tmpImage;
	CImageList*	m_pCompleteImageList;
	struct _graph*	m_pgraphUnion;	// 합성차트 저장위해
};

//{{AFX_INSERT_LOCATION}}
#endif // !defined(AFX_SCREENCFG_H__F76B4300_6116_4AAC_BC6A_E1217B5B1418__INCLUDED_)
