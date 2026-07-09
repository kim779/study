#if !defined(AFX_FMCTRL_H__693E8615_EB4D_48A3_9740_B52A9DBA1713__INCLUDED_)
#define AFX_FMCTRL_H__693E8615_EB4D_48A3_9740_B52A9DBA1713__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// fmCtrl.h : header file
//

#include "fmBase.h"
/////////////////////////////////////////////////////////////////////////////
// CfmCtrl command target

class AFX_EXT_CLASS CfmCtrl : public CfmBase
{
	DECLARE_DYNCREATE(CfmCtrl)

public:
	CfmCtrl();           // protected constructor used by dynamic creation
	CfmCtrl(CAxisForm* axform, struct _formR* formR, int iform, bool drawOnly);
	virtual ~CfmCtrl();

// Attributes
public:
	FARPROC	m_callproc;
	CWnd*	m_ctrl;
	int	m_visible;

protected:
	HINSTANCE m_instance;
	bool	m_drawOnly;

// GO_FIX : Resizable
	int	m_bottom_gap;
	int	m_right_gap;

// Operations
public:
	void	SetRect(CRect rect, CRect pRc);

	void	SetFont(int point, int style, CString fonts, bool resize);
	void	Draw(CDC* dc);
	void	Adjust(CSize size);
	void	Adjust(CPoint pt, float hR, float vR);
	void	Resize(float hR, float vR, bool font = true, bool redraw = false);
	void	Reload();

	void	SetFocus(bool focus);
	void	SetBkColor(int rgb, int col = -1, int row = -1);

	void	ReadAll(CString& data, bool edit = false, bool head = false);
	void	ReadData(CString& data, bool edit = false, int size = -1, int col = -1, int row = -1);
	void	WriteAll(CString str, bool redraw = true);
	void	WriteData(CString data, bool redraw = true, int col = -1, int row = -1);
	void	SetVisible(bool visible, int col = -1);
	bool	IsVisible(int col = -1);
	void	SetEnable(bool enable);
	bool	IsEnable();
	void	ClearData(bool selectOnly = false);

	bool	IsChanged(bool reset = true)			{ return false; }
	void	SetFCB(bool blink, int flash);
	void	SetEnum(CString text, int value, int func = -1);

	bool	IsFocus(HWND hWnd);
	void	ShowControl(BOOL visible);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CfmCtrl)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CfmCtrl)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CfmCtrl)
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FMCTRL_H__693E8615_EB4D_48A3_9740_B52A9DBA1713__INCLUDED_)
