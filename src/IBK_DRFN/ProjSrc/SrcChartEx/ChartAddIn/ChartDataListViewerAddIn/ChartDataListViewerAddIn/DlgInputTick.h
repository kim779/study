#if !defined(AFX_DLGINPUTTick_H__ACEB2D85_6AC8_4507_A553_9005C647A6F0__INCLUDED_)
#define AFX_DLGINPUTTick_H__ACEB2D85_6AC8_4507_A553_9005C647A6F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgInputTick.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgInputTick dialog

class CDlgInputTick : public CDialog
{
// Construction
public:
	CDlgInputTick(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgInputTick)
	enum { IDD = IDD_DLG_INPUT_TICK };
	CString	m_strSecond;
	CString	m_strDay;
	CString	m_strHour;
	CString	m_strMin;
	//}}AFX_DATA

	CString	m_strCurrentDate;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgInputTick)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgInputTick)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGINPUTTick_H__ACEB2D85_6AC8_4507_A553_9005C647A6F0__INCLUDED_)
