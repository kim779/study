#if !defined(AFX_FNAME_H__ED215BB9_EF79_4811_B321_11D8B01EC70D__INCLUDED_)
#define AFX_FNAME_H__ED215BB9_EF79_4811_B321_11D8B01EC70D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// fname.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFname dialog

class CFname : public CDialog
{
// Construction
public:
	CFname(CWnd* pParent = NULL);   // standard constructor

	CString	GetMapName() { return m_mapN; }
// Dialog Data
	//{{AFX_DATA(CFname)
	enum { IDD = IDD_FULLNAME };
	CEdit	m_edit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFname)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	DWORD		m_Conversion, m_Sentence;
	CString	m_mapN;
	// Generated message map functions
	//{{AFX_MSG(CFname)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeName();
	virtual void OnOK();
	afx_msg void OnSetfocusName();
	afx_msg void OnKillfocusName();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FNAME_H__ED215BB9_EF79_4811_B321_11D8B01EC70D__INCLUDED_)
