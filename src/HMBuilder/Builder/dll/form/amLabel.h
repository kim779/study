#if !defined(AFX_AMLABEL_H__2277C107_A164_438A_B537_4502912D0013__INCLUDED_)
#define AFX_AMLABEL_H__2277C107_A164_438A_B537_4502912D0013__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// fmLabel.h : header file
//

#include "ambase.h"

/////////////////////////////////////////////////////////////////////////////
// CamLabel command target

class AFX_EXT_CLASS CamLabel : public CamBase
{
	DECLARE_DYNCREATE(CamLabel)

public:
	CamLabel();	           // protected constructor used by dynamic creation
	CamLabel(CAmForm* axform, struct _formR* formR);
	virtual ~CamLabel();

// Operations
public:
	void	Draw(CDC* dc);

	LPCTSTR	GetText();
	void	SetText(LPCTSTR lpszNewValue);
	long	GetPRgb();
	void	SetPRgb(long nNewValue);
	long	GetTRgb();
	void	SetTRgb(long nNewValue);
	BOOL	GetVisible();
	void	SetVisible(BOOL bNewValue);
	void	_Refresh();
	void	_Blink(BOOL set, long color);
	void	_Push(LPCTSTR name);
	void	_Pop(LPCTSTR name);

protected:
	class	Cimage*	m_pic;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CamLabel)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CamLabel)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CamLabel)	
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AMLABEL_H__2277C107_A164_438A_B537_4502912D0013__INCLUDED_)
