#if !defined(AFX_DLGINPUTMONTH_H__AB738F19_14E4_4810_BEBA_FD5CA57A8E07__INCLUDED_)
#define AFX_DLGINPUTMONTH_H__AB738F19_14E4_4810_BEBA_FD5CA57A8E07__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgInputMonth.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgInputMonth dialog

class CDlgInputMonth : public CDialog
{
// Construction
public:
	CDlgInputMonth( HWND p_hOcxWnd, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgInputMonth)
	enum { IDD = IDD_DLG_INPUT_MONTH };
	CString	m_strMonth;
	CString	m_strYear;
	//}}AFX_DATA

	CString m_strCurrentDate;
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgInputMonth)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgInputMonth)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// (2008/1/22 - Seung-Won, Bae) for Multi-Language
protected:
	HWND				m_hOcxWnd;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGINPUTMONTH_H__AB738F19_14E4_4810_BEBA_FD5CA57A8E07__INCLUDED_)
