#if !defined(AFX_AMBOX_H__8E45B359_C941_4C52_B35C_36DC68F7EA4F__INCLUDED_)
#define AFX_AMBOX_H__8E45B359_C941_4C52_B35C_36DC68F7EA4F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// fmBox.h : header file
//

#include "ambase.h"

/////////////////////////////////////////////////////////////////////////////
// CamBox command target

class AFX_EXT_CLASS CamBox : public CamBase
{
	DECLARE_DYNCREATE(CamBox)

public:
	CamBox();           // protected constructor used by dynamic creation
	CamBox(CAmForm* axform, struct _formR* formR);
	virtual ~CamBox();

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
	void	_SetVisible(bool visible, int col = -1);
	void	Refresh();

	long	GetPRgb();
	void	SetPRgb(long nNewValue);
	BOOL	GetVisible();
	void	SetVisible(BOOL bNewValue);

	void	_Refresh();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CamBox)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	void	draw3DBox(Graphics& g);

	// Generated message map functions
	//{{AFX_MSG(CamBox)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CamBox)
	
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AMBOX_H__8E45B359_C941_4C52_B35C_36DC68F7EA4F__INCLUDED_)
