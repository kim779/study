#if !defined(AFX_BASELISTBOX_H__E4530F75_C359_4B51_B643_C31036AF7A99__INCLUDED_)
#define AFX_BASELISTBOX_H__E4530F75_C359_4B51_B643_C31036AF7A99__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BaseListBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBaseListBox window


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 25
// Comments		: 
// Using way	: 
// See			: class CListBox
//-----------------------------------------------------------------------------
class CBaseListBox : public CListBox
{
// Construction
public:
	CBaseListBox();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBaseListBox)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBaseListBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CBaseListBox)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BASELISTBOX_H__E4530F75_C359_4B51_B643_C31036AF7A99__INCLUDED_)
