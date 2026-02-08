#if !defined(AFX_EXXMLTREECTRL_H__AAAFA8CA_8049_4EBA_8F38_1788C4950280__INCLUDED_)
#define AFX_EXXMLTREECTRL_H__AAAFA8CA_8049_4EBA_8F38_1788C4950280__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExXmlTreeCtrl.h : header file
//

#include <afxtempl.h>   //<= CList
#include "../DefineFiles/client_typedef.h"

/////////////////////////////////////////////////////////////////////////////
// CExXmlTreeCtrl window

class CExXmlTreeCtrl : public CTreeCtrl
{
// Construction
public:
	CExXmlTreeCtrl();
	virtual ~CExXmlTreeCtrl();

private:
	int			m_bAllItemView;
	HTREEITEM	m_htiRoot;
	CImageList	m_imglistTree;

public:
	BOOL Create(CWnd* pParentWnd, UINT nID);

	void SetIconLoad();
	int GetTreeIconIndex_File(BOOL bAlertReg, BOOL bUnitCan);
	
	BOOL SetInitTreeList(ArrChartSectionInfo *pUserData);
	void SetSelectItem();
	BOOL SetSelectItem(CString strFolderPath, CString strFileName);


	void SetControlState(BOOL bState);
	HTREEITEM SetTreeItem(HTREEITEM htiParent, CString strItem, int nImgIndex, int nImgSelIndex, int lParam);

	void SetItemAllView(BOOL bAllItemView);
	void SetItemChildView(HTREEITEM hItem, BOOL bAllItemView);
	BOOL GetFindTreeItem(HTREEITEM htItemNode, CString strFindText, HTREEITEM &hItem);

	BOOL SetDeleteItem(HTREEITEM hSelItem);
	void GetSelectItemData(HTREEITEM hSelItem);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExXmlTreeCtrl)
	public:
	
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CExXmlTreeCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXXMLTREECTRL_H__AAAFA8CA_8049_4EBA_8F38_1788C4950280__INCLUDED_)
