#if !defined(AFX_AMRADIO_H__4876C137_F8B2_4BB8_BBF7_A814C5E1B5BB__INCLUDED_)
#define AFX_AMRADIO_H__4876C137_F8B2_4BB8_BBF7_A814C5E1B5BB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// fmRadio.h : header file
//

#include "ambase.h"

/////////////////////////////////////////////////////////////////////////////
// CamRadio command target

class AFX_EXT_CLASS CamRadio : public CamBase
{
	DECLARE_DYNCREATE(CamRadio)

public:
	CamRadio();	           // protected constructor used by dynamic creation
	CamRadio(CAmForm* axform, struct _formR* formR);
	virtual ~CamRadio();

	class	Cimage* m_pic;
	int		m_image;
// Attributes
protected :
	bool		m_mousedown;
	CRect		m_iRc;
	CRect		m_tRc;

// Operations
public:
	virtual	void	Draw(CDC* dc);
	virtual	void	OnLButton(bool down, CPoint pt, int& result);
	virtual	void	OnDblClick(CPoint pt, int& result);
	virtual	void	SetFocus(bool focus);

	void	SetChecked(bool checked);
	void	ReadData(CString& data, bool edit = false, int col = -1, int row = -1);
	void	WriteData(CString data, bool redraw = true, int col = -1, int row = -1);

	void	_Refresh();
	void	_SetFocus();

	CString GetText();
	void	SetText(LPCTSTR lpszNewValue);
	long	GetPRgb();
	void	SetPRgb(long nNewValue);
	long	GetTRgb();
	void	SetTRgb(long nNewValue);
	BOOL	GetVisible();
	void	SetVisible(BOOL bNewValue);
	BOOL	GetEnable();
	void	SetEnable(BOOL bNewValue);
	BOOL	_GetChecked();
	void	_SetChecked(BOOL bNewValue);
	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CamRadio)
public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CamRadio)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CamRadio)	
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AMRADIO_H__4876C137_F8B2_4BB8_BBF7_A814C5E1B5BB__INCLUDED_)
