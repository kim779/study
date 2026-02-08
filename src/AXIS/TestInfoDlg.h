#if !defined(AFX_TESTINFODLG_H__4E05B5DD_5FDB_43D5_B67E_8F6B51AE11E6__INCLUDED_)
#define AFX_TESTINFODLG_H__4E05B5DD_5FDB_43D5_B67E_8F6B51AE11E6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TestInfoDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTestInfoDlg dialog

class CTestInfoDlg : public CDialog
{
// Construction
public:
	CTestInfoDlg(CWnd* pParent = NULL);   // standard constructor
	CString m_strurl, m_strbitmap;
	CBitmap* m_bmpBg;
	CBitmap* LoadFileBitmap(const char* bmpName );
	bool m_bRscMode;
// Dialog Data
	//{{AFX_DATA(CTestInfoDlg)
	enum { IDD = IDD_TESTDLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestInfoDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTestInfoDlg)
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTINFODLG_H__4E05B5DD_5FDB_43D5_B67E_8F6B51AE11E6__INCLUDED_)
