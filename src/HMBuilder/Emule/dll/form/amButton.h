#if !defined(AFX_AMBUTTON_H__36C954F7_B2EF_4333_8691_0E4536ED4B07__INCLUDED_)
#define AFX_AMBUTTON_H__36C954F7_B2EF_4333_8691_0E4536ED4B07__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// fmButton.h : header file
//

#include "ambase.h"

/////////////////////////////////////////////////////////////////////////////
// CAmButton command target

class AFX_EXT_CLASS CAmButton : public CamBase
{
	DECLARE_DYNCREATE(CAmButton)

public:
	CAmButton();           // protected constructor used by dynamic creation
	CAmButton(CAmForm* axform, struct _formR* formR);
	virtual ~CAmButton();

// Attributes
protected:
	class	Cimage*	m_pic;
	bool	m_mousedown;
	int		m_image;
	CRect		m_iRc;
	CRect		m_tRc;

// Operations
public:
	void	SetFont(int point, int style, CString fonts, bool resize);
	void	Draw(CDC* dc);
	void	OnLButton(bool down, CPoint pt, int& result);
	int		OnDrag(CPoint pt, int& result);

	void	WriteData(CString data, bool redraw = true, int col = -1, int row = -1);
	void	ReadData(CString& data, bool edit = false, int col = -1, int row = -1);
	void	SetFocus(bool focus);

	void	SetText(LPCTSTR lpszNewValue);
	LPCTSTR	GetText();
	long	GetPRgb();
	void	SetPRgb(long nNewValue);
	long	GetTRgb();
	void	SetTRgb(long nNewValue);
	bool	GetVisible();
	void	SetVisible(bool bNewValue);
	bool	GetEnable();
	void	SetEnable(bool bNewValue);
	
	void	_SetFocus();
	void	_Refresh();
	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAmButton)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CAmButton)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CAmButton)	
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AMBUTTON_H__36C954F7_B2EF_4333_8691_0E4536ED4B07__INCLUDED_)
