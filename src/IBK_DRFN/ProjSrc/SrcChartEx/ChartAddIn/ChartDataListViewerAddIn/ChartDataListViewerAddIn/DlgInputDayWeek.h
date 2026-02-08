#if !defined(AFX_DLGINPUTDAYWEEK_H__35A99921_2800_4CCB_89DE_6A0E33719608__INCLUDED_)
#define AFX_DLGINPUTDAYWEEK_H__35A99921_2800_4CCB_89DE_6A0E33719608__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgInputDayWeek.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgInputDayWeek dialog

class CDlgInputDayWeek : public CDialog
{
// Construction
public:
	CDlgInputDayWeek(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgInputDayWeek)
	enum { IDD = IDD_DLG_INPUT_DAY_WEEK };
	CString	m_strDay;
	CString	m_strMonth;
	CString	m_strYear;
	//}}AFX_DATA

	CString	m_strCurrentDate;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgInputDayWeek)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgInputDayWeek)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGINPUTDAYWEEK_H__35A99921_2800_4CCB_89DE_6A0E33719608__INCLUDED_)
