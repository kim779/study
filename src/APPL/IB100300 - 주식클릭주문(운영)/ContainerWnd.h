#pragma once
// ContainerWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CContainerWnd window

class CContainerWnd : public CWnd
{
// Construction
public:
	CContainerWnd();

// Attributes
public:
	void CContainerWnd::SetKind(int iKind);


protected:
	CWnd*		m_pwndParent;

	HINSTANCE	m_hinstanceDll;
	CWnd*		m_pwndControl;

	CString		m_strPreText;

	CString		GetCtrlProperty( LPCSTR prop_name );
	void		SetCtrlProperty( _bstr_t prop_name, _variant_t &var );

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

	void	BtnClick(CString strPreText);

	void	SetParent(CWnd* pwndParent);

	void	SetValues(CString strValue);
	void	SetAlways(bool bflag){ SetCtrlProperty("Always",_variant_t(bflag)); }

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
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	LRESULT	OnMessage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

