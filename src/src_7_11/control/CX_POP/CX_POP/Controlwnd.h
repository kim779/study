#pragma once

#include "../../../h/axisfire.h"
// CControlwnd

class CParam
{
public:
	int		key;			// screen key
	CString		name;			// control symbol name
	CRect		rect;			// control rect
	CString		fonts;			// font name
	int		point;			// font point
	int		style;			// font style
	DWORD		tRGB;			// text color index
	DWORD		pRGB;			// paint color index
	CString		options;		// properties
};

class CControlwnd : public CWnd
{
	DECLARE_DYNAMIC(CControlwnd)

public:
	CControlwnd();
	virtual ~CControlwnd();

	virtual void OnFinalRelease();
	BOOL		m_bDown, m_bHover, m_bTracking;
	enum	    _MouseSate {dispidSetVisible = 2L,
		dispidSetdata = 1L,
		MsNo = 0, MsOver, MsDown} m_MouseState;
	
	CRect		m_btnRect;
	CWnd*		m_pWizard;
	CParam		m_Param;
	CFont		m_Font;
	CBitmap		*m_pBitmap[3];
	CString		m_strHome;
	CString		m_data;

	enum _wndPOS {WLEFT = 0, WCENTER, WRIGHT } m_wndPOS;

	HCURSOR mh_ncursor, mh_qcursor;
	class CPopInfo*	m_dlgpop;
	class CTriDlg*  m_dlgTri;

	CBitmap* GetBitmap(CString path);
	void	ResizeControls();
	void	SetParam(struct _param *pParam);
	void	DrawBitmap(CDC *pDC, CRect rc, CBitmap *pBitmap);
	CString Parser(CString &srcstr, CString substr);

protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg LRESULT OnWndMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMouseHover(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
protected:
	void Setdata(LPCTSTR data);
public:
	afx_msg void OnDestroy();
protected:
	void SetVisible(LONG bVisible);
};


