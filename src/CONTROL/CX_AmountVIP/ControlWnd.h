#pragma once
// ControlWnd.h : header file
//
#include "AmountPopup.h"

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
/////////////////////////////////////////////////////////////////////////////
// CControlWnd window

class CControlWnd : public CWnd
{
// Construction
public:
	CControlWnd();

// Attributes
public:
	DWORD		m_dwState;
	bool		m_capture, m_bChecked, m_bCheck;
	BOOL		m_bHover, m_bTracking;
	HBITMAP		m_hBitmap, m_hBitmap_dn, m_hBitmap_hv;
	CWnd*		m_pParent;
	CAmountPopup*	m_pPopup;
	CParam		m_Param;
	CString		m_rtnStr;
	COLORREF	m_HoverColor;
	CString		m_sRoot, m_sUserID;

	int		m_kind, m_type;
	CString		m_sImagePath;
	CString		m_sText, m_sSymbol;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CControlWnd)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
public:
	CRect adjustRect(CRect rc);
	CFont* getAxFont(CString fName, int point, int style);
	CBitmap* getBitmap(CString path);
	CString OptionParser(CString sOption, CString sKey);
	void	SetButtonImages();
	CString Variant(int comm, CString data);
	void	WriteAlert(CString str);
	CString	Parser(CString &srcstr, CString substr);
	void	setCheck(bool bCheck = true);
	bool	IsChecked();
	CSize	BitmapSize(HBITMAP hBitmap);
	void	SetParam(struct _param *pParam);
	LOGFONT	setFont(CString fName, int fsize, int style);
	COLORREF GetIndexColor(int index);
	virtual ~CControlWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CControlWnd)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnMouseHover(WPARAM wparam, LPARAM lparam);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnEnable(BOOL bEnable);
	afx_msg	void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	afx_msg long OnMessage(WPARAM wParam, LPARAM lParam);
	afx_msg long OnMessage1(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CControlWnd)
	afx_msg short GetType();
	afx_msg void SetType(short nNewValue);
	afx_msg BOOL GetEnable();
	afx_msg void SetEnable(BOOL bNewValue);
	afx_msg BOOL GetVisible();
	afx_msg void SetVisible(BOOL bNewValue);
	afx_msg BSTR GetProperties();
	afx_msg void SetProperties(LPCTSTR sParam);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};
