#if !defined(AFX_FMAVI_H__D838F8AF_3723_4573_8A73_AA56EDCC2A99__INCLUDED_)
#define AFX_FMAVI_H__D838F8AF_3723_4573_8A73_AA56EDCC2A99__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// fmAvi.h : header file
//

#include "fmbase.h"
#include "mmsystem.h"

/////////////////////////////////////////////////////////////////////////////
// CfmAvi command target

class AFX_EXT_CLASS CfmAvi : public CfmBase
{
	DECLARE_DYNCREATE(CfmAvi)

public:
	CfmAvi();		// protected constructor used by dynamic creation
	CfmAvi(CAxisForm* axform, struct _formR* formR, int iform);
	virtual ~CfmAvi();

// Attributes
protected:
	MCI_ANIM_OPEN_PARMS	m_mciOpen;
	MCI_ANIM_RECT_PARMS	m_mciRect;
	MCI_ANIM_WINDOW_PARMS	m_mciWindow;
	MCI_PLAY_PARMS		m_mciPlay;
	UINT			m_aviDev;
	bool			m_valid;

// Operations
public:
	void	Resize(float hRatio, float vRatio, bool font = true, bool redraw = false);
	bool	Play();
	bool	Replay();
	void	Stop();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CfmAvi)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CfmAvi)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CfmAvi)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FMAVI_H__D838F8AF_3723_4573_8A73_AA56EDCC2A99__INCLUDED_)
