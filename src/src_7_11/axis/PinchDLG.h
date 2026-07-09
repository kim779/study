#if !defined(AFX_PINCHDLG_H__4C59C910_47F9_47EC_BC99_51A9BD3AC3C8__INCLUDED_)
#define AFX_PINCHDLG_H__4C59C910_47F9_47EC_BC99_51A9BD3AC3C8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PinchDLG.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPinchDLG dialog

class CPinchDLG : public CDialog
{
// Construction
public:
	CPinchDLG(CWnd* pParent = NULL);   // standard constructor
	void	SetData();
	CWnd*	m_pParent;

	CString	m_User[4];

// Dialog Data
	//{{AFX_DATA(CPinchDLG)
	enum { IDD = IDD_PINCHSET };
	CComboBox	m_VirSet4;
	CComboBox	m_VirSet3;
	CComboBox	m_VirSet2;
	CComboBox	m_VirSet1;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPinchDLG)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPinchDLG)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PINCHDLG_H__4C59C910_47F9_47EC_BC99_51A9BD3AC3C8__INCLUDED_)
