#if !defined(AFX_DLGINPUTMIN_H__9C083C03_2ED4_4B8A_B983_5B38D93E6701__INCLUDED_)
#define AFX_DLGINPUTMIN_H__9C083C03_2ED4_4B8A_B983_5B38D93E6701__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgInputMin.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgInputMin dialog

class CDlgInputMin : public CDialog
{
// Construction
public:
	CDlgInputMin( HWND p_hOcxWnd, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgInputMin)
	enum { IDD = IDD_DLG_INPUT_MIN };
	CString	m_strDay;
	CString	m_strHour;
	CString	m_strMonth;
	CString	m_strMin;
	//}}AFX_DATA

	CString	m_strCurrentDate;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgInputMin)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgInputMin)
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

#endif // !defined(AFX_DLGINPUTMIN_H__9C083C03_2ED4_4B8A_B983_5B38D93E6701__INCLUDED_)
