#pragma once
// ColorPickerWnd.h : header file
//
#include "ColorButton.h"

class CParam
{
public:
	int		key=0;			// screen key
	CString		name;			// control symbol name
	CRect		rect;			// control rect
	CString		fonts;			// font name
	int		point=0;		// font point
	int		style=0;		// font style
	DWORD		tRGB=0;			// text color index
	DWORD		pRGB=0;			// paint color index
	CString		options;		// properties
};


/////////////////////////////////////////////////////////////////////////////
// CColorPickerWnd window
class CColorPickerWnd : public CWnd
{
// Construction
public:
	CColorPickerWnd();

// Attributes
public:
	CWnd*		m_pParent;
	
	CString		m_sRoot, m_sUser;
	CParam		m_Param;
	CString		m_sFile;		// File Name
	CString		m_sSession;		// ini File contact Session
	CString		m_sKey;			// ini File contact Key
	COLORREF	m_clrSelColor;

	std::unique_ptr <CColorButton>	m_ctlLogo;

// Operations
public:
	CFont		*getAxFont(CString fName, int point, int style);
	CBitmap		*getBitmap(CString path);

	CString		OptionParser(CString sOption, CString sKey);
	CString		Variant(int comm, CString data);
	CString		Parser(CString &srcstr, CString substr);

	void		SetParam(struct _param *pParam);
	LOGFONT		setFont(CString fName, int fsize, int style);
	COLORREF	GetIndexColor(int index);
	CPen		*GetAxPen(COLORREF clr, int width, int style);
	CBrush		*GetAxBrush(COLORREF clr);
	COLORREF	GetColorData();			// ini File Read
	void		SetColorData(COLORREF color);	// ini File Write
	BOOL		CheckSession();			// ini File, Session-Value Check
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorPickerWnd)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CColorPickerWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CColorPickerWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	afx_msg LONG OnSelEndOK(UINT lParam, LONG wParam);
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CColorPickerWnd)
	afx_msg void SetName(LPCTSTR sSession, LPCTSTR sKey);
	afx_msg void refresh();
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};