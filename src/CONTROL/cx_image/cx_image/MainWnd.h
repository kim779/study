#if !defined(AFX_MAINWND_H__62292936_215D_4027_BCE6_A7172FF169DA__INCLUDED_)
#define AFX_MAINWND_H__62292936_215D_4027_BCE6_A7172FF169DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MainWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMainWnd window
#include "../H/axisfire.h"
class CMainWnd : public CWnd
{
// Construction
public:
	CMainWnd(CWnd* pParent, void* pParam);

// Attributes
	CWnd* m_pWizard{};
	_param m_Param{};
	CString m_slog{};

	CBitmap* m_pStrechBmp{};
	HBITMAP m_hBmp{};
	Gdiplus::Bitmap* m_pGdiBitmap{};
	CToolTipCtrl* m_pTooltip{};
	CString m_strToolText{};

	COLORREF m_clrBack{};
	CString Variant(int nComm, CString strData = _T(""));
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	void DrawBitmapWithGDIPlus(Graphics* pGraphics, HBITMAP hBmp, int x, int y, int width, int height);
	CString Parser(CString& srcstr, CString substr);
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainWnd)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMainWnd)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CMainWnd)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
protected:
	void SetImageFile(BSTR sImage);

	enum
	{
		dispidSetToolTipText = 3L,
		dispidSetRect = 2L,
		dispidSetImageFile = 1L
	};
	void SetRect(SHORT left, SHORT top, SHORT right, SHORT bottom);
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
protected:
	void SetToolTipText(BSTR sText);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINWND_H__62292936_215D_4027_BCE6_A7172FF169DA__INCLUDED_)
