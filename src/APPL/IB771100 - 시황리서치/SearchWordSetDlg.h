#pragma once
// SearchWordSetDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSearchWordSetDlg dialog
#include "BkUtil.h"
#include "XPButton.h"
class CSearchWordSetDlg : public CDialog, public CBkUtil
{
// Construction
public:
	CSearchWordSetDlg(CMapDlg *mapdlg);   // standard constructor

	void SetWordArray(CStringArray *array);
	CStringArray * m_pSearchWordArray;
	class CMapDlg* m_pMapDlg;
// Dialog Data
	//{{AFX_DATA(CSearchWordSetDlg)
	enum { IDD = IDD_SET_DLG };
	CListBox	m_listWord;
	CString	m_edWord;
	CXPButton m_Btn[4];
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSearchWordSetDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSearchWordSetDlg)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonDel();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

