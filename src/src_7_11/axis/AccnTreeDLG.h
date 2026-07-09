


#pragma once

// AccnTreeDLG.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAccnTreeDLG dialog

class CAccnTreeDLG : public CDialog
{
// Construction
public:
	CAccnTreeDLG(CWnd* pParent = NULL);   // standard constructor
	BOOL	SetTreeAccn(int vsn);

	void	RedrawCtrl(bool bResize);

	HTREEITEM	m_itemACCN01, m_itemACCN02, m_itemACCN03, m_itemACCN04;

	CString m_TRSymbol[2], m_TRData;

	CImageList m_image;

public:

// Dialog Data
	//{{AFX_DATA(CAccnTreeDLG)
	enum { IDD = IDD_ACCN_TREE };
	CTreeCtrl	m_AccnTree;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAccnTreeDLG)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAccnTreeDLG)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
	afx_msg void OnHIDE();
	afx_msg void OnDblclkAccnTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangedAccnTree(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

