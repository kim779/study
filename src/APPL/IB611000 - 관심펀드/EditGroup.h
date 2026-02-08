#pragma once

// EditGroup.h : header file
//
#include "Dlg.h"
#include "EditEx.h"
/////////////////////////////////////////////////////////////////////////////
// CEditGroup dialog

class CEditGroup : public CDialog
{
// Construction
public:
	CEditGroup(CWnd* pParent = NULL, CWnd *m_pWizard = NULL, CString root = _T(""), CString name = _T(""), _grouplist **grouplist=NULL, int grouplistsize = 0);   // standard constructor
	//~CEditGroup();
// Dialog Data
	//{{AFX_DATA(CEditGroup)
	enum { IDD = IDD_GROUPEDIT };
	CListCtrl	m_list;
	//}}AFX_DATA

public:
	
	CWnd* m_EditGroup{};
	CDlg* m_cDlg{};
	CWnd* m_main{};
	CString		m_userD;
	CString		m_tempN;
	
	int		m_selindex{};
	CString		m_clickItem;
	
	int		m_nItem{};
	CImageList* m_dragImage{};
	int m_groupsize{};
	//CArray <std::unique_ptr<_groupEditlist>, std::unique_ptr<_groupEditlist>> m_oldGroupLIst;
	CArray <_groupEditlist*, _groupEditlist*> m_oldGroupLIst;
	//vector<std::unique_ptr<_groupEditlist>, std::unique_ptr<_groupEditlist>> m_oldGroupLIst;
	CArray <_grouplist*, _grouplist*> m_newGroupLIst;
	int		m_delGroupcnt{};
	_grouplist** m_grouplist{};

	char	m_afcd[MYFD_MAXFUND][12]{};

protected:
	void	InitList();
	void	KillSelected(CListCtrl *ListCtrl);
	void	SetSelected(CListCtrl *ListCtrl, int nItem, BOOL scroll = TRUE);
	CImageList* CreateDragImageSymbol(LPPOINT lpPoint, CListCtrl *ListCtrl);
	void	endDrag(CPoint point);
	int	addItem(const CString& strItem, int nItem);
	BOOL	addColumn(LPCTSTR strItem, int nItem, int nWidth);
	CString	parseX(CString &srcstr, CString substr);
	std::unique_ptr<class CEditEx>	m_pEdit;
	void	SendDeleteGroup();
	void	SendUpdaeGroup(int ncnt);
	LRESULT SendTR(CString strCode, CString strData, int iKey, int iStat = NULL);

	void	QueryExcelSise();

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
	afx_msg void OnDelBtn();
	afx_msg void OnDownBtn();
	afx_msg void OnClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRenameBtn();
	afx_msg void OnUpBtn();
	afx_msg void OnOK();
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnBegindragList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBeginlabeleditList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndlabeleditList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	afx_msg long OnEditGroupMessage(WPARAM wParam, LPARAM lParam);
	afx_msg long OnEditGroupMsg(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

