#if !defined(AFX_DEFAULTPAGE_H__EA488770_992B_42AA_87C7_7CA7DEA40725__INCLUDED_)
#define AFX_DEFAULTPAGE_H__EA488770_992B_42AA_87C7_7CA7DEA40725__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DefaultPage.h : header file
//
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CDefaultPage dialog

class CDefaultPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CDefaultPage)

// Construction
public:
	CDefaultPage();
	~CDefaultPage();

	CBrush	m_ctlBrushDn;
	CBrush	m_ctlBrushUp;

	COLORREF	m_ctlClrUp;
	COLORREF	m_ctlClrDn;

// Dialog Data
	//{{AFX_DATA(CDefaultPage)
	enum { IDD = IDD_PROPPAGE_DEFAULT };
	CSpinButtonCtrl	m_dnRGBSpin;
	CSpinButtonCtrl	m_upRGBSpin;
	int		m_dnRGB;
	int		m_upRGB;
	int		m_clrTabPos;
	CString	m_fontname;
	CString	m_fontpnt;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CDefaultPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDefaultPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnSetfontbtn();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEFAULTPAGE_H__EA488770_992B_42AA_87C7_7CA7DEA40725__INCLUDED_)
