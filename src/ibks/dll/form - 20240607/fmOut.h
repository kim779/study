#if !defined(AFX_FMOUT_H__7B7B73E0_B5F3_472B_AC4E_BB25402A1083__INCLUDED_)
#define AFX_FMOUT_H__7B7B73E0_B5F3_472B_AC4E_BB25402A1083__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// fmOut.h : header file
//

#include "fmBase.h"

/////////////////////////////////////////////////////////////////////////////
// CfmOut command target

class AFX_EXT_CLASS CfmOut : public CfmBase
{
	DECLARE_DYNCREATE(CfmOut)

public:
	CfmOut();	           // protected constructor used by dynamic creation
	CfmOut(CAxisForm* axform, struct _formR* formR, int iform);
	virtual ~CfmOut();

// Attributes
protected:
	CRect		m_tRc;
	CString		m_edit;
	CString		m_data;

	class	Cimage*	m_pic;
	bool	m_pop;

// Operations
public:
	void	Draw(CDC* dc);
	void	OnLButton(bool down, CPoint pt, int& result);
	int	OnDrag(CPoint pt, int& result);

	void	ReadData(CString& data, bool edit = false, int size = -1, int col = -1, int row = -1);
	void	WriteData(CString data, bool redraw = true, int col = -1, int row = -1);
	void	ClearData(bool selectOnly = false);

	void	SetEnum(CString text, int value, int func = -1);
	void	Refresh();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CfmOut)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	void	editData();

	// Generated message map functions
	//{{AFX_MSG(CfmOut)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CfmOut)
	afx_msg BSTR _getText();
	afx_msg BSTR _getData();
	afx_msg void _setData(LPCTSTR lpszNewValue);
	afx_msg long _getPRgb();
	afx_msg void _setPRgb(long nNewValue);
	afx_msg long _getTRgb();
	afx_msg void _setTRgb(long nNewValue);
	afx_msg BOOL _getVisible();
	afx_msg void _setVisible(BOOL bNewValue);
	afx_msg BOOL _getSkip();
	afx_msg void _setSkip(BOOL bNewValue);
	afx_msg long _getFloatDigit();
	afx_msg void _setFloatDigit(long nNewValue);
	afx_msg void _Refresh();
	afx_msg void _Blink(BOOL set, long color);
	afx_msg void _Push(LPCTSTR name);
	afx_msg void _Clear();
	afx_msg long _GetDataKind();
	afx_msg long _GetMode();
	afx_msg long _GetLength();
	afx_msg BSTR _GetFormat();
	afx_msg void _SetText(LPCTSTR text);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FMOUT_H__7B7B73E0_B5F3_472B_AC4E_BB25402A1083__INCLUDED_)
