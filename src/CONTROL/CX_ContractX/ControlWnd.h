#pragma once
// ControlWnd.h : header file
//

#include "../../h/axisfire.h"
#include "../../h/axisvar.h"
#include "../../h/grid.h"

class CParam
{
public:
	int		key=0;		// screen key
	CString		name;		// control CX_Symbol name
	CRect		rect;		// control rect
	CString		fonts;		// font name
	int		point=0;	// font point
	int		style=0;	// font style
	DWORD		tRGB=0;		// text color index
	DWORD		pRGB=0;		// paint color index
	CString		options;	// properties
};

/////////////////////////////////////////////////////////////////////////////
// CControlWnd window

class CControlWnd : public CWnd
{
// Construction
public:
	CControlWnd(CWnd *pParent, struct _param *pParam);

// Attributes
public:
	CWnd*	m_pParent;
	CParam	m_param;

	int	m_gubn;
	int	m_col1;
	int	m_col2;
	int	m_rows;
	int	m_max;
	int	m_display;

	CString	m_sRTSCode;

// Operations
public:
	void	Init();
	void	setParam(struct _param *param);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CControlWnd)
public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CControlWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CControlWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	afx_msg long OnMessage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CControlWnd)
	CString m_jCode;
	afx_msg void OnJCodeChanged();
	BOOL m_visible;
	afx_msg void OnVisibleChanged();
	afx_msg void Clear();
	afx_msg void SendData();
	afx_msg void _SetRect(short left, short top, short right, short bottom);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

public:
	int	GetMax()	{ return m_max; }

private:
	CSize	m_BasicSize;
	CFont	*m_pBasicFont, *m_pFont;
	std::unique_ptr<class CGridWnd>	m_grid;

	bool	m_bWait;
	bool	m_bAlert;

	void	OnAlertx(struct _alertR* alertR);
	void	OnAlert(const char* param);
	void	WriteAlert(CString str);
	void	SendTR(CString name, CString data, BYTE type, int key);
	CString Parser(CString &srcstr, CString substr);
	bool	IsWait();
	void	DoWait(bool bWait = true);

	CString	CommaModify(CString string);
	void	ResizeToFit();
};