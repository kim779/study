#pragma once
// ContainerWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CContainerWnd window
#define WM_MANAGE	(WM_USER + 16)
#define MK_MARKETEVENT		1

#include <atlcom.h>
#include <comdef.h>
#define USES_CONVERSION_CONST  const int _convert = 0; (_convert); const UINT _acp = ATL::_AtlGetConversionACP() /*CP_THREAD_ACP*/; (_acp); LPCWSTR _lpw = NULL; (_lpw); LPCSTR _lpa = NULL; (_lpa)
#define USES_CONVERSION_CONST2       int _convert = 0; (_convert); const UINT _acp = ATL::_AtlGetConversionACP() /*CP_THREAD_ACP*/; (_acp); LPCWSTR _lpw = NULL; (_lpw); LPCSTR _lpa = NULL; (_lpa)

#include <comutil.h>

class CContainerWnd : public CWnd
{
// Construction
public:
	CContainerWnd();

// Attributes
public:

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

