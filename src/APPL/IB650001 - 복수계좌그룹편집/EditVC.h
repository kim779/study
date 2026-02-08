#if !defined(AFX_EDITVC_H__FFA31E57_8E1C_4DAC_9CA4_751A16E8310F__INCLUDED_)
#define AFX_EDITVC_H__FFA31E57_8E1C_4DAC_9CA4_751A16E8310F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditVC.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEditVC window

class CEditVC : public CEdit
{
// Construction
public:
	CEditVC();

// Attributes
public:
	CRect m_rectNCBottom;
    CRect m_rectNCTop;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditVC)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CEditVC();

	// Generated message map functions
protected:
	//{{AFX_MSG(CEditVC)
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	afx_msg void OnNcPaint();
	afx_msg UINT OnGetDlgCode();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITVC_H__FFA31E57_8E1C_4DAC_9CA4_751A16E8310F__INCLUDED_)
