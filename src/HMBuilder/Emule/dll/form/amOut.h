#if !defined(AFX_AMOUT_H__7B7B73E0_B5F3_472B_AC4E_BB25402A1083__INCLUDED_)
#define AFX_AMOUT_H__7B7B73E0_B5F3_472B_AC4E_BB25402A1083__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// fmOut.h : header file
//

#include "ambase.h"
#include "image.h"

/////////////////////////////////////////////////////////////////////////////
// CamOut command target

class AFX_EXT_CLASS CamOut : public CamBase
{
	DECLARE_DYNCREATE(CamOut)

public:
	CamOut();	           // protected constructor used by dynamic creation
	CamOut(CAmForm* axform, struct _formR* formR);
	virtual ~CamOut();

// Attributes
protected:
	CRect		m_tRc;
	CString		m_edit;
	CString		m_data;

	Cimage*		m_pic;
// Operations
public:
	void	Draw(CDC* dc);
	void	ReadData(CString& data, bool edit = false, int col = -1, int row = -1);
	void	WriteData(CString data, bool redraw = true, int col = -1, int row = -1);
	void	ClearData(bool selectOnly = false);

	LPCTSTR	GetText();
	void	SetText(LPCTSTR lpszNewValue);
	LPCTSTR	GetData();
	void	SetData(LPCTSTR lpszNewValue);
	long	GetPRgb();
	void	SetPRgb(long nNewValue);
	long	GetTRgb();
	void	SetTRgb(long nNewValue);
	bool	GetVisible();
	void	SetVisible(bool bNewValue);
	bool	GetEnable();
	void	SetEnable(bool bNewValue);
	bool	GetSkip();
	void	SetSkip(bool bNewValue);
	
	void	_Refresh();
	void	_Blink(bool set, long color);
	void	_Push(LPCTSTR name);
	void	_Clear();

	void	Refresh();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CamOut)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	void	editData();

	// Generated message map functions
	//{{AFX_MSG(CamOut)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CamOut)	
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AMOUT_H__7B7B73E0_B5F3_472B_AC4E_BB25402A1083__INCLUDED_)
