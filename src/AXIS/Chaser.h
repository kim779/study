#if !defined(AFX_CHASER_H__38D32D68_74F7_4B03_92C6_B7DC6288A94D__INCLUDED_)
#define AFX_CHASER_H__38D32D68_74F7_4B03_92C6_B7DC6288A94D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Chaser.h : header file
//
#define WM_CHASER	WM_USER+22222

#define	x_RCVs		0x03		// RCV DATAs
#define x_CONs		0x09		// CONs DATAs
/////////////////////////////////////////////////////////////////////////////
// CChaser window
struct _exeCDSS
{
	DWORD	flag;
	DWORD	len;
};
#define L_cds	sizeof(struct _exeCDSS)


class CChaser : public CWnd
{
// Construction
public:
	CChaser(CWnd* parent);
	virtual ~CChaser();

// Attributes
protected:
	CWnd*	m_parent;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChaser)
	//}}AFX_VIRTUAL

// Implementation
public:

	// Generated message map functions
protected:
	//{{AFX_MSG(CChaser)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	afx_msg	LRESULT OnChaser(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHASER_H__38D32D68_74F7_4B03_92C6_B7DC6288A94D__INCLUDED_)
