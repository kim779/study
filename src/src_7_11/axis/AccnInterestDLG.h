#if !defined(AFX_ACCNINTERESTDLG_H__04A37B90_AF86_4906_BD39_A3ED3FF5979F__INCLUDED_)
#define AFX_ACCNINTERESTDLG_H__04A37B90_AF86_4906_BD39_A3ED3FF5979F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AccnInterestDLG.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAccnInterestDLG dialog

class CAccnInterestDLG : public CDialog
{
// Construction
public:
	CAccnInterestDLG(CWnd* pParent = NULL);   // standard constructor

	BOOL	SetTreeAccn(int vsn);

	void	RedrawCtrl(bool bResize);

	HTREEITEM	m_itemACCN01, m_itemACCN02;

	CString		m_TRSymbol, m_TRData;

	CImageList	m_image;

// Dialog Data
	//{{AFX_DATA(CAccnInterestDLG)
	enum { IDD = IDD_ACCN_INTEREST };
	CTreeCtrl	m_AccnTree;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAccnInterestDLG)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAccnInterestDLG)
	afx_msg void OnDblclkAccnTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangedAccnTree(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACCNINTERESTDLG_H__04A37B90_AF86_4906_BD39_A3ED3FF5979F__INCLUDED_)
