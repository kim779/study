#if !defined(AFX_INPUTIP_H__22395FE3_13EB_4640_B9D4_D3872BC9011B__INCLUDED_)
#define AFX_INPUTIP_H__22395FE3_13EB_4640_B9D4_D3872BC9011B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputIP.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInputIP dialog

class CInputIP : public CDialog
{
// Construction
public:
	CInputIP(CString home, CWnd* pParent = NULL);   // standard constructor

	CString	m_home;
// Dialog Data
	//{{AFX_DATA(CInputIP)
	enum { IDD = IDD_INPUT_IP };
	CString	m_strConnectName;
	CString	m_strIPNum;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputIP)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInputIP)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTIP_H__22395FE3_13EB_4640_B9D4_D3872BC9011B__INCLUDED_)
