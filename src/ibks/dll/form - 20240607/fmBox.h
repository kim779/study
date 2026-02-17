#if !defined(AFX_FMBOX_H__8E45B359_C941_4C52_B35C_36DC68F7EA4F__INCLUDED_)
#define AFX_FMBOX_H__8E45B359_C941_4C52_B35C_36DC68F7EA4F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// fmBox.h : header file
//

#include "fmbase.h"

/////////////////////////////////////////////////////////////////////////////
// CfmBox command target

class AFX_EXT_CLASS CfmBox : public CfmBase
{
	DECLARE_DYNCREATE(CfmBox)

public:
	CfmBox();           // protected constructor used by dynamic creation
	CfmBox(CAxisForm* axform, struct _formR* formR, int iform);
	virtual ~CfmBox();

// Attributes
protected:
	int	m_width;
	int	m_height;
	int	m_ww;
	int	m_hh;
	int	m_key;
	int	m_type;

// Operations
public:
	void	Adjust(CPoint pt, float hR, float vR);
	void	Resize(float hR, float vR, bool font = true, bool redraw = false);
	void	Reload();
	void	Draw(CDC* dc);
	void	SetVisible(bool visible, int col = -1);
	void	Refresh();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CfmBox)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	void	draw3DBox(CDC* dc);

	// Generated message map functions
	//{{AFX_MSG(CfmBox)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CfmBox)
	afx_msg long _getPRgb();
	afx_msg void _setPRgb(long nNewValue);
	afx_msg BOOL _getVisible();
	afx_msg void _setVisible(BOOL bNewValue);
	afx_msg void _Refresh();
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FMBOX_H__8E45B359_C941_4C52_B35C_36DC68F7EA4F__INCLUDED_)
