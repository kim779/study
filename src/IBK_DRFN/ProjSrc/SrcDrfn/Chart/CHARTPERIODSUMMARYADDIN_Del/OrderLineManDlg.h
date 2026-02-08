#if !defined(AFX_ORDERLINEMANDLG_H__F26EFB08_A988_4359_8448_591B423E9B74__INCLUDED_)
#define AFX_ORDERLINEMANDLG_H__F26EFB08_A988_4359_8448_591B423E9B74__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OrderLineManDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COrderLineManDlg dialog

class COrderLineManDlg : public CDialog
{
// Construction
public:
	void RemoveLIneFromObList(CString strSerial);
	CObList* m_pLineList;
	CString m_strUserPath;
	int FindListItem(CString strSerial);
	CString TokenizeEx(CString strSrc/*IN*/, const char* pszTokens/*IN*/, int& iStart/*IN, OUT*/);

	CStringList *m_pSavedAllOrderLines;
	COrderLineManDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COrderLineManDlg)
	enum { IDD = IDD_DLG_OL_MAN };
	CListCtrl	m_ctrlOLList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COrderLineManDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COrderLineManDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnDel();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeydownList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ORDERLINEMANDLG_H__F26EFB08_A988_4359_8448_591B423E9B74__INCLUDED_)
