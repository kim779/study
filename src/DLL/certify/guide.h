#if !defined(AFX_GUIDE_H__E9A93005_76CF_4696_BA44_42C6DECD2A88__INCLUDED_)
#define AFX_GUIDE_H__E9A93005_76CF_4696_BA44_42C6DECD2A88__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// guide.h : header file
//

#define	typeREVOKE	0
#define	typeSUSPEND	1
#define	typeEXPIRE	2

/////////////////////////////////////////////////////////////////////////////
// CGuide dialog

class CGuide : public CDialog
{
// Construction
public:
	CGuide(int type, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGuide)
	enum { IDD = IDD_GUIDE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

private:
	int	m_type;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGuide)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGuide)
	virtual BOOL OnInitDialog();
	afx_msg void OnIssue();
	afx_msg void OnOther();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GUIDE_H__E9A93005_76CF_4696_BA44_42C6DECD2A88__INCLUDED_)
