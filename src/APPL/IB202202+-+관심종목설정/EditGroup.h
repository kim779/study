#pragma once
// EditGroup.h : header file
//
#include "Page1.h"
#include "EditEx.h"

/////////////////////////////////////////////////////////////////////////////
// CEditGroup dialog
class CPage1;

class CEditGroup : public CDialog
{
// Construction
public:
	CEditGroup(CWnd* pParent = nullptr, CString root = _T(""), CString name = _T(""));   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEditGroup)
	enum { IDD = IDD_EDITGROUP };
	CListCtrl	m_list;
	//}}AFX_DATA

public:
	char m_pdata[1024 * 2];
	CString		m_userD;
	CString		m_tempN;

	int		m_selindex{};
	CString		m_clickItem;

	int		m_nItem{};
	std::shared_ptr<CImageList>  m_dragImage;
	std::unique_ptr<class CEditEx> m_pEdit;
	CWnd* m_pParent{};
public:

protected:

	void	InitList();
	void	KillSelected(CListCtrl *ListCtrl);
	void	SetSelected(CListCtrl *ListCtrl, int nItem, BOOL scroll = TRUE);
	std::shared_ptr<CImageList> CreateDragImageSymbol(LPPOINT lpPoint, CListCtrl *ListCtrl);
	void	endDrag(CPoint point);

protected:
	int	addItem(CString& strItem, int nItem);
	BOOL	addColumn(LPCTSTR strItem, int nItem, int nWidth);
	CString	parseX(CString &srcstr, CString substr);
	BOOL ExistFile(CString fullfile);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditGroup)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEditGroup)
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnUpBtn();
	afx_msg void OnDownBtn();
	afx_msg void OnRenameBtn();
	afx_msg void OnDelBtn();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnBegindragList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndlabeleditList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickList(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	afx_msg void OnBeginlabeleditList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

