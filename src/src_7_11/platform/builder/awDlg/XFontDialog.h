// XFontDialog.h  Version 1.0
//
// Author:  Hans Dietrich
//          hdietrich2@hotmail.com
//
// This software is released into the public domain.  You are free to use it 
// in any way you like.
//
// This software is provided "as is" with no expressed or implied warranty.  
// I accept no liability for any damage or loss of business that this software 
// may cause.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef XFONTDIALOG_H
#define XFONTDIALOG_H

/////////////////////////////////////////////////////////////////////////////
// CXFontDialog dialog

// bit masks for m_dwRemoveFonts
#define XCF_SHOW_SYMBOL					0x0001		// show only symbol fonts 
#define XCF_SHOW_ALL_EXCEPT_SYMBOL		0x0002		// show all except symbol fonts 
#define XCF_SHOW_MONOSPACED				0x0004		// show only monospaced fonts
#define XCF_SHOW_ALL_EXCEPT_MONOSPACED	0x0008		// show all except monospaced fonts


class CXFontDialog : public CFontDialog
{
	DECLARE_DYNAMIC(CXFontDialog)

// Construction
public:
	CXFontDialog(LPLOGFONT lplfInitial = NULL,
		DWORD dwFlags = /*CF_EFFECTS |*/ CF_SCREENFONTS,
		CDC* pdcPrinter = NULL,
		CWnd* pParentWnd = NULL);
#ifndef _AFX_NO_RICHEDIT_SUPPORT
	CXFontDialog(const CHARFORMAT& charformat,
		DWORD dwFlags = CF_SCREENFONTS,
		CDC* pdcPrinter = NULL,
		CWnd* pParentWnd = NULL);
#endif

// Operations
public:
	virtual int DoModal();

// Attributes
public:
	CString	GetCaption() { return m_strCaption; }
	DWORD	GetFontFilter() { return m_dwFontFilter; }
	CString	GetInitialFontName() { return m_strInitialFont; }
	LONG	GetTmHeight() { return m_Height; }
	BOOL	IsMonoSpaced() { return (m_PitchAndFamily & TMPF_FIXED_PITCH) == 0; }
	BOOL	IsSymbol() { return (m_CharSet == SYMBOL_CHARSET); }
	void	SetCaption(LPCTSTR strCaption) { m_strCaption = strCaption; }
	void	SetCharSet(BYTE charset) { m_CharSet = charset; }
	void	SetFontFilter(DWORD dwFontFilter) { m_dwFontFilter = dwFontFilter; }
	void	SetPitchAndFamily(BYTE pitchandfamily) { m_PitchAndFamily = pitchandfamily; }
	void	SetTmHeight(LONG nHeight) { m_Height = nHeight; }

// Implementation
protected:
	CString	m_strInitialFont;
	DWORD	m_dwFontFilter;
	CString m_strCaption;
	BYTE	m_CharSet;
	BYTE	m_PitchAndFamily;
	LONG	m_Height;

	//{{AFX_MSG(CXFontDialog)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	LRESULT OnShowMonoSpaced(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif //XFONTDIALOG_H
