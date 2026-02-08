#if !defined(AFX_INFO_INTEREST_H__C014DE37_8DA5_44DE_8B53_650A1546A17E__INCLUDED_)
#define AFX_INFO_INTEREST_H__C014DE37_8DA5_44DE_8B53_650A1546A17E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Info_Interest.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInfo_Interest dialog

class CInfo_Interest : public CDialog
{
// Construction
public:
	void SetOK();
	CInfo_Interest(CWnd* pParent = NULL);   // standard constructor
	CBitmap* m_backImg;

// Dialog Data
	//{{AFX_DATA(CInfo_Interest)
	enum { IDD = IDD_INFO_INTEREST };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInfo_Interest)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInfo_Interest)
	virtual void OnCancel();
	virtual	void OnOK();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INFO_INTEREST_H__C014DE37_8DA5_44DE_8B53_650A1546A17E__INCLUDED_)
