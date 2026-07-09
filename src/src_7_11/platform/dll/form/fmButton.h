#if !defined(AFX_FMBUTTON_H__36C954F7_B2EF_4333_8691_0E4536ED4B07__INCLUDED_)
#define AFX_FMBUTTON_H__36C954F7_B2EF_4333_8691_0E4536ED4B07__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// fmButton.h : header file
//

#include "fmbase.h"

/////////////////////////////////////////////////////////////////////////////
// CfmButton command target

class AFX_EXT_CLASS CfmButton : public CfmBase
{
	DECLARE_DYNCREATE(CfmButton)

public:
	CfmButton();           // protected constructor used by dynamic creation
	CfmButton(CAxisForm* axform, struct _formR* formR, int iform);
	virtual ~CfmButton();

// Attributes
protected:
	class	Cimage*	m_pic;
	int	m_image;

// Operations
public:
	void	SetFont(int point, int style, CString fonts, bool resize);
	void	Draw(CDC* dc);
	void	OnLButton(bool down, CPoint pt, int& result);
	int	OnDrag(CPoint pt, int& result);

	void	WriteData(CString data, bool redraw = true, int col = -1, int row = -1);
	void	ReadData(CString& data, bool edit = false, int size = -1, int col = -1, int row = -1);
	void	SetFocus(bool focus);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CfmButton)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CfmButton)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CfmButton)
	afx_msg BSTR _getText();
	afx_msg void _setText(LPCTSTR lpszNewValue);
	afx_msg long _getPRgb();
	afx_msg void _setPRgb(long nNewValue);
	afx_msg long _getTRgb();
	afx_msg void _setTRgb(long nNewValue);
	afx_msg BOOL _getVisible();
	afx_msg void _setVisible(BOOL bNewValue);
	afx_msg BOOL _getEnable();
	afx_msg void _setEnable(BOOL bNewValue);
	afx_msg BOOL _getTabStop();
	afx_msg void _setTabStop(BOOL bNewValue);
	afx_msg void _SetFocus();
	afx_msg void _Refresh();
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FMBUTTON_H__36C954F7_B2EF_4333_8691_0E4536ED4B07__INCLUDED_)
