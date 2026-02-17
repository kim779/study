#if !defined(AFX_COLUMNS_H__EC0022F6_86FE_4983_8350_A71D7012D1D1__INCLUDED_)
#define AFX_COLUMNS_H__EC0022F6_86FE_4983_8350_A71D7012D1D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// columns.h : header file
//
#include "fxImgBtn.h"
#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CColumns dialog

class CColumns : public CDialog
{
// Construction
public:
	CColumns(CWnd* pParent = NULL);   // standard constructor
	CColumns(CString sRoot, CString src, CString items, CString defs, CWnd* pParent);
	CString m_slog{};
// Dialog Data
	//{{AFX_DATA(CColumns)
	enum { IDD = IDD_COLUMN };
	CListCtrl	m_list;
	
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColumns)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	CfxImgBtn		*m_BtnInit;
	CfxImgBtn		*m_BtnOk;
	CfxImgBtn		*m_BtnCancel;
	CfxImgBtn		*m_BtnTop;
	CfxImgBtn		*m_BtnUp;
	CfxImgBtn		*m_BtnDown;
	CfxImgBtn		*m_BtnBottom;

	CStringArray		m_items;
	CStringArray		m_defs;
	CMapStringToString	m_columns;
	CString			m_sItemList;
	CImageList*		m_dragImage;
	CWnd*			m_dragWnd;
	CFont*			m_pFont;
	CString			m_sRoot;
	
	int			m_selindex;
	CString			m_sImageDir;
	CBitmap			*bmInit, *bmInit_dn, *bmInit_en, 
				*bmTop, *bmTop_dn, *bmTop_en, 
				*bmUp, *bmUp_dn, *bmUp_en, 
				*bmDown, *bmDown_dn, *bmDown_en, 
				*bmBottom, *bmBottom_dn, *bmBottom_en, 
				*bmOk, *bmOk_dn, *bmOk_en, 
				*bmCancel, *bmCancel_dn, *bmCancel_en;
	
	HBITMAP			HbmInit, HbmInit_dn, HbmInit_en, 
				HbmTop, HbmTop_dn, HbmTop_en, 
				HbmUp, HbmUp_dn, HbmUp_en, 
				HbmDown, HbmDown_dn, HbmDown_en, 
				HbmBottom, HbmBottom_dn, HbmBottom_en, 
				HbmOk, HbmOk_dn, HbmOk_en, 
				HbmCancel, HbmCancel_dn, HbmCancel_en;
// Implementation
public:
	CString		GetItems();

protected:
	void		parse(CString src, CString items, CString defs);
	BOOL		addColumn(CListCtrl& list, CString columns, int col, int width);
	int		addItem(CString items, CString keys, int item);
	void		begindrag(CListCtrl* list, NMHDR* pNMHDR);
	CImageList*	createDragImage(CListCtrl* list, CPoint& point);
	bool		IsExist(CStringArray & array, CString sKey);
	bool		checkDuplicate(CString keys);
	void		OnBeginDragSelectList(NMHDR* pNMHDR, LRESULT* pResult) ;
	void		KillSelected();
	void		SetSelected(int nItem, BOOL scroll = TRUE);
	void		InitControl();
	void		SetBitmaps();
	void		CreateFont();
	LOGFONT		GetLogFont(int fsize, DWORD weight);
	CBitmap*	CreateBmp( LPCTSTR sBMPFile);
	void		Del();
	void		Add();
	void		OnEndDrag(CPoint point);
	// Generated message map functions
	//{{AFX_MSG(CColumns)
	virtual BOOL OnInitDialog();
	afx_msg void OnTop();
	afx_msg void OnUp();
	afx_msg void OnDown();
	afx_msg void OnBottom();
	afx_msg void OnOk();
	afx_msg void OnCancel();
	afx_msg void OnBeginDragAllList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnInit();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCustomdrawList(NMHDR* pNMHDR, LRESULT* pResult);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLUMNS_H__EC0022F6_86FE_4983_8350_A71D7012D1D1__INCLUDED_)
