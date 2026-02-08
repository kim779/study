#if !defined(AFX_XDATETIME_H__AEB0209A_6B3E_4975_8A49_1DF1E9C2889F__INCLUDED_)
#define AFX_XDATETIME_H__AEB0209A_6B3E_4975_8A49_1DF1E9C2889F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// XDateTime.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CXDateTime window
extern UINT NEAR WM_XEDIT_VK_RETURN;
extern UINT NEAR WM_XEDIT_VK_ESCAPE;
class CXDateTime : public CDateTimeCtrl
{
// Construction
public:
	CXDateTime();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXDateTime)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CXDateTime();

	// Generated message map functions
protected:
	//{{AFX_MSG(CXDateTime)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XDATETIME_H__AEB0209A_6B3E_4975_8A49_1DF1E9C2889F__INCLUDED_)
