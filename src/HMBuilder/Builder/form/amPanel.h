#if !defined(AFX_AMPANEL_H__000DCAFE_0063_430C_BEA5_17E482ACC993__INCLUDED_)
#define AFX_AMPANEL_H__000DCAFE_0063_430C_BEA5_17E482ACC993__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// fmPanel.h : header file
//

#include "ambase.h"

/////////////////////////////////////////////////////////////////////////////
// CamPanel command target

class AFX_EXT_CLASS CamPanel : public CamBase
{
	DECLARE_DYNCREATE(CamPanel)

public:
	CamPanel();	           // protected constructor used by dynamic creation
	CamPanel(CAmForm* axform, struct _formR* formR);
	virtual ~CamPanel();

// Attributes
protected :
	class	Cimage*	m_pic;
	int	m_image;

// Operations
public:
	void	Draw(CDC* dc);
	void	OnLButton(bool down, CPoint pt, int& result);
	int	OnDrag(CPoint pt, int& result);

	void	WriteData(CString data, bool redraw = true, int col = -1, int row = -1);
	void	ReadData(CString& data, bool edit = false, int col = -1, int row = -1);

	LPCTSTR	GetText();
	void	SetText(LPCTSTR lpszNewValue);
	long	GetPRgb();
	void	SetPRgb(long nNewValue);
	long	GetTRgb();
	void	SetTRgb(long nNewValue);
	BOOL	GetVisible();
	void	SetVisible(BOOL bNewValue);
	
	void	_Refresh();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CamPanel)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CamPanel)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CamPanel)	
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AMPANEL_H__000DCAFE_0063_430C_BEA5_17E482ACC993__INCLUDED_)
