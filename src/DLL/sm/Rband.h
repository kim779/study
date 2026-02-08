#if !defined(AFX_RBAND_H__FFEA5CAF_486D_4AF1_AD07_2B1CF3B09582__INCLUDED_)
#define AFX_RBAND_H__FFEA5CAF_486D_4AF1_AD07_2B1CF3B09582__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Rband.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRband window

class CRband : public CWnd
{
// Construction
public:
	CRband(bool (*axiscall)(int, WPARAM, LPARAM), CString home);
	virtual ~CRband();

// Attributes
public:
	CString	m_home;
	CBitmap	m_img;

	class CNButton*	m_btn1;
	class CNButton*	m_btn2;
	class CNButton*	m_btn3;
	class CNButton*	m_btn4;
	bool	(*m_axiscall)(int, WPARAM, LPARAM);

	bool	m_bCustomer;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRband)
	//}}AFX_VIRTUAL

// Implementation
public:
	CSize	GetBandSize();
	// Generated message map functions
protected:
	//{{AFX_MSG(CRband)
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	afx_msg	LONG OnXMSG(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RBAND_H__FFEA5CAF_486D_4AF1_AD07_2B1CF3B09582__INCLUDED_)
