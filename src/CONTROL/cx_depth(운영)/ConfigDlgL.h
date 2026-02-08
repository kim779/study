#if !defined(AFX_CONFIGDLGL_H__FF1CF4C9_602E_49FC_B4E9_5E7EB2811701__INCLUDED_)
#define AFX_CONFIGDLGL_H__FF1CF4C9_602E_49FC_B4E9_5E7EB2811701__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConfigDlgL.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConfigDlgL dialog

class CConfigDlgL : public CDialog
{
// Construction
public:
	CConfigDlgL(struct _config *pConfig, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CConfigDlgL)
	enum { IDD = IDD_CONFIGL };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfigDlgL)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CConfigDlgL)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	afx_msg void OnRadio3();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	struct _config	*m_pConfig;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFIGDLGL_H__FF1CF4C9_602E_49FC_B4E9_5E7EB2811701__INCLUDED_)
