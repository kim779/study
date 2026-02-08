#if !defined(AFX_DLGITEMMAXCOUNT_H__E9C4120D_98B8_4B8E_9860_C029D1A974F2__INCLUDED_)
#define AFX_DLGITEMMAXCOUNT_H__E9C4120D_98B8_4B8E_9860_C029D1A974F2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgItemMaxCount.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgItemMaxCount dialog

class CDlgItemMaxCount : public CDialog
{
// Construction
public:
	CDlgItemMaxCount(int nMaxItemCount, int& nReturn, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgItemMaxCount)
	enum { IDD = IDD_DLG_MAXITEMCOUNT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgItemMaxCount)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int	m_nMaxItemCount;
	int& m_nReturn;

	// Generated message map functions
	//{{AFX_MSG(CDlgItemMaxCount)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnBtnModify();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGITEMMAXCOUNT_H__E9C4120D_98B8_4B8E_9860_C029D1A974F2__INCLUDED_)
