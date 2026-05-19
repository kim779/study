#pragma once
#include "../object/fmObject.h"

// CfmMyButton

class CfmMyButton : public CWnd, CfmObject
{
	DECLARE_DYNAMIC(CfmMyButton)

public:
	CfmMyButton();
	CfmMyButton(class CfmProperties* formOb, class CiForm* piObject, CWnd* parent);
	virtual ~CfmMyButton();

	virtual void OnFinalRelease();

// Attributes
protected:
	class	Cimage*	m_pic;
	int	m_image;
	bool	m_mousedown;

// Operations
public:
	void	Draw(CDC* dc);
	void	OnLMyButton(bool down, CPoint pt);
	int	OnDrag(CPoint pt);



public:
	// Object Interface
	LPDISPATCH  GetDispatch(){ return GetIDispatch(TRUE); }
	CWnd*	    GetWnd(){ return this; }
	CString   GetProperties();
	CString   GetMethod();
	CString	  GetEvent();
	
	void	Reload();
	void	Refresh();
	void	WriteData(CString data, bool redraw = true, int col = -1, int row = -1);
	void	SetFocus(bool focus);
	void	SetEnable(bool enable);
	void	drawObject(CRect rect, BOOL erase);
	void	SetFont(int point, int style, CString fonts, bool resize);
	void	SetRect(CRect rect, CRect pRc);
	void	SetVisible(bool visible, int col = -1);
protected:
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CfmMyButton)

	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnLMyButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLMyButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	afx_msg void OnLMyButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnDestroy();

protected:

	enum
	{
		dispidText = 1,
		dispidPRgb = 2,
		dispidTRgb = 3,
		dispidVisible = 4,
		dispidEnable = 5,
		dispidTabStop = 6,
		dispidLeft = 7,
		dispidTop = 8,
		dispidRight = 9,
		dispidBottom = 10,
		dispidRefresh = 11,
		dispidSetFocus = 12,
		dispidSetRect = 13
	};


	BSTR _getText();
	void _setText(LPCTSTR lpszNewValue);
	long _getPRgb();
	void _setPRgb(long nNewValue);
	long _getTRgb();
	void _setTRgb(long nNewValue);
	BOOL _getVisible();
	void _setVisible(BOOL bNewValue);
	BOOL _getEnable();
	void _setEnable(BOOL bNewValue);
	BOOL _getTabStop();
	void _setTabStop(BOOL bNewValue);
	void _SetFocus();
	void _Refresh();
	LONG _getLeft(void);
	void _setLeft(LONG newVal);
	LONG _getTop(void);
	void _setTop(LONG newVal);
	LONG _getRight(void);
	void _setRight(LONG newVal);
	LONG _getBottom(void);
	void _setBottom(LONG newVal);
	void _SetRect(LONG left, LONG top, LONG right, LONG bottom);
public:
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
};


