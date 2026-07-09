#if !defined(AFX_FMCHECK_H__CD3A9690_948C_47D1_A3D9_3DC24265C577__INCLUDED_)
#define AFX_FMCHECK_H__CD3A9690_948C_47D1_A3D9_3DC24265C577__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// fmCheck.h : header file
//

#include "fmRadio.h"

/////////////////////////////////////////////////////////////////////////////
// CfmCheck command target

class AFX_EXT_CLASS CfmCheck : public CfmRadio
{
	DECLARE_DYNCREATE(CfmCheck)

public:
	CfmCheck();           // protected constructor used by dynamic creation
	CfmCheck(CAxisForm* axform, struct _formR* formR, int iform);
	virtual ~CfmCheck();

// Operations
public:
	void	Draw(CDC* dc);
	void	OnLButton(bool down, CPoint pt, int& result);
	void	SetFocus(bool focus);

	void	UpdateData(int key, bool moving, int& result);
	void	InsertData(int key, bool moving, int& result);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CfmCheck)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CfmCheck)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CfmCheck)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FMCHECK_H__CD3A9690_948C_47D1_A3D9_3DC24265C577__INCLUDED_)
