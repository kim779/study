#pragma once
// ContainerWnd.h : header file
//
#include "stdafx.h"
/////////////////////////////////////////////////////////////////////////////
// CContainerWnd window
#include <comutil.h>
class CContainerWnd : public CWnd
{
// Construction
public:
	CContainerWnd();

// Attributes
public:
	CString m_slog{};
protected:
	CWnd*		m_pwndParent;

	HINSTANCE	m_hinstanceDll;
	CWnd*		m_pwndControl;

public:
	BOOL	CreateControl(	CString strRoot,
				CString strDllName,
				CString strSymbolName,
				CRect rectControl,
				CString strOption,
				CString strFontName = "±¼¸²Ã¼",
				int iFontPoint = 9,
				int iStyle = 0,
				int iKey = 0,
				COLORREF rgbFG = 63,
				COLORREF rgbBG = 62);


	void	SetParent(CWnd* pwndParent);
	CString		GetCtrlProperty(LPCSTR prop_name);
	void		SetCtrlProperty(_bstr_t prop_name, _variant_t& var);
	short IsNxtAble(LPCSTR code);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CContainerWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CContainerWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CContainerWnd)
	//}}AFX_MSG
	LRESULT	OnMessage(WPARAM wParam, LPARAM lParam);
	LRESULT	OnMaketPickerMessage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMove(int x, int y);
};

