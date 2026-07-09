#if !defined(AFX_CORGB_H__EE007FC0_7D9B_11D4_A024_00001CD7F9BE__INCLUDED_)
#define AFX_CORGB_H__EE007FC0_7D9B_11D4_A024_00001CD7F9BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// coRGB.h : header file
//

#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CCoRGB dialog

class AFX_EXT_CLASS CCoRGB : public CDialog
{
// Construction
public:
	CCoRGB(CWnd* pParent, CString name, BYTE isEdit, int upRGB, int dnRGB);   // standard constructor

protected:
	CBrush		m_ctlBrushUp, m_ctlBrushDn;
	COLORREF	m_ctlClrUp, m_ctlClrDn;

// Dialog Data
	//{{AFX_DATA(CCoRGB)
	enum { IDD = IDD_CORGB };
	CString	m_downE;
	CString	m_upE;
	CString	m_name;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCoRGB)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Attributes
public:
	int	m_upRGB;
	int	m_dnRGB;
	BYTE	m_isEdit;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCoRGB)
	virtual BOOL OnInitDialog();
	afx_msg void OnUp();
	afx_msg void OnDown();
	virtual void OnOK();
	afx_msg void OnNoEdit();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CORGB_H__EE007FC0_7D9B_11D4_A024_00001CD7F9BE__INCLUDED_)
