#if !defined(AFX_TOOLSTATIC_H__60F863B4_BD92_41CD_BDEC_4A99B2AA07AD__INCLUDED_)
#define AFX_TOOLSTATIC_H__60F863B4_BD92_41CD_BDEC_4A99B2AA07AD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ToolStatic.h : header file
//

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2006/4/14
// Comments		: Static Control with Returning the Focus to OCX
// Using way	: 
// See			: 
//-----------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
// CToolStatic window

class CToolStatic : public CStatic
{
// Construction
public:
	CToolStatic();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolStatic)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CToolStatic();

	// Generated message map functions
protected:
	//{{AFX_MSG(CToolStatic)
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLSTATIC_H__60F863B4_BD92_41CD_BDEC_4A99B2AA07AD__INCLUDED_)
