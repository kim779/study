#if !defined(AFX_FMSYSM_H__9BBE83D3_9ADD_4332_A053_5DF86BBCCED0__INCLUDED_)
#define AFX_FMSYSM_H__9BBE83D3_9ADD_4332_A053_5DF86BBCCED0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// fmSysm.h : header file
//

#include "fmLabel.h"

/////////////////////////////////////////////////////////////////////////////
// CfmSysm command target

class AFX_EXT_CLASS CfmSysm : public CfmLabel
{
	DECLARE_DYNCREATE(CfmSysm)

public:
	CfmSysm();	           // protected constructor used by dynamic creation
	CfmSysm(CAxisForm* axform, struct _formR* formR, int iform);
	virtual ~CfmSysm();

// Operations
public:
	void	WriteData(CString data, bool redraw = true, int col = -1, int row = -1);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CfmSysm)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	void	getSystemData();

	// Generated message map functions
	//{{AFX_MSG(CfmSysm)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CfmSysm)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FMSYSM_H__9BBE83D3_9ADD_4332_A053_5DF86BBCCED0__INCLUDED_)
