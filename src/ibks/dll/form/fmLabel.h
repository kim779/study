#if !defined(AFX_FMLABEL_H__2277C107_A164_438A_B537_4502912D0013__INCLUDED_)
#define AFX_FMLABEL_H__2277C107_A164_438A_B537_4502912D0013__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// fmLabel.h : header file
//

#include "fmbase.h"

/////////////////////////////////////////////////////////////////////////////
// CfmLabel command target

class AFX_EXT_CLASS CfmLabel : public CfmBase
{
	DECLARE_DYNCREATE(CfmLabel)

public:
	CfmLabel();	           // protected constructor used by dynamic creation
	CfmLabel(CAxisForm* axform, struct _formR* formR, int iform);
	virtual ~CfmLabel();

// Operations
public:
	void	Draw(CDC* dc);
	void	OnLButton(bool down, CPoint pt, int& result);
	int	OnDrag(CPoint pt, int& result);

protected:
	bool	m_pop;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CfmLabel)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CfmLabel)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CfmLabel)
	afx_msg BSTR _getText();
	afx_msg void _setText(LPCTSTR lpszNewValue);
	afx_msg long _getPRgb();
	afx_msg void _setPRgb(long nNewValue);
	afx_msg long _getTRgb();
	afx_msg void _setTRgb(long nNewValue);
	afx_msg BOOL _getVisible();
	afx_msg void _setVisible(BOOL bNewValue);
	afx_msg BSTR _getData();
	afx_msg void _setData(LPCTSTR lpszNewValue);
	afx_msg void _Refresh();
	afx_msg void _Blink(BOOL set, long color);
	afx_msg void _Push(LPCTSTR name);
	afx_msg void _Pop(LPCTSTR name);
	afx_msg void _Clear();
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FMLABEL_H__2277C107_A164_438A_B537_4502912D0013__INCLUDED_)
