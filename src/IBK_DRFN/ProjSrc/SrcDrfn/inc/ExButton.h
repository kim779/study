// ExButton.h: interface for the CExButton class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXBUTTON_H__4593D39A_9BA7_471A_A52D_72C73B318BF9__INCLUDED_)
#define AFX_EXBUTTON_H__4593D39A_9BA7_471A_A52D_72C73B318BF9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CExButton : public CButton  
{
// Construction
public:
	CExButton();

// Attributes
public:
	BOOL	m_bDrawBackground;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CExButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CExButton)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_EXBUTTON_H__4593D39A_9BA7_471A_A52D_72C73B318BF9__INCLUDED_)
