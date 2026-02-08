#if !defined(AFX_CONDITIONTREECTRL_H__AAAFA8CA_8049_4EBA_8F38_1788C4950280__INCLUDED_)
#define AFX_CONDITIONTREECTRL_H__AAAFA8CA_8049_4EBA_8F38_1788C4950280__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExTreeCtrl.h : header file
//

#include <afxtempl.h>   //<= CList
/////////////////////////////////////////////////////////////////////////////
// CConditionTreeCtrl window
class CConditionTreeCtrl : public CTreeCtrl
{
// Construction
public:
	CConditionTreeCtrl();
	virtual ~CConditionTreeCtrl();

private:
	int									m_bAllItemView;
	HTREEITEM						m_htiRoot;
	
	HTREEITEM						m_hSelItem;
	BOOL								m_bSelState;
	ConditionData				m_stSelectData;

	CImageList m_imglistTree;
	CList <ConditionDataGroup, ConditionDataGroup&> *m_plistGroup;

  int m_nConditionCnt;

public :
	BOOL Create(CWnd* pParentWnd, UINT nID);
	BOOL SetInit(CList <ConditionDataGroup, ConditionDataGroup&> *plistGroup);
	
	BOOL GetData_SelItem(ConditionData	&stConditionData);
  
  int GetDataCnt() { return m_nConditionCnt;};

private:
	void SetIconLoad();
	HTREEITEM SetTreeItem(HTREEITEM htiParent, CString strItem, 
												int nImgIndex, int nImgSelIndex, int lParam);

	void SetItemAllView(BOOL bAllItemView);
	void SetItemChildView(HTREEITEM hItem, BOOL bAllItemView);

	BOOL GetData_Item(ConditionData	&stConditionData);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConditionTreeCtrl)
	public:
	
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CConditionTreeCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROFFERTREECTRL_H__AAAFA8CA_8049_4EBA_8F38_1788C4950280__INCLUDED_)
