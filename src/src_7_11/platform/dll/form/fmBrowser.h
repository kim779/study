#if !defined(AFX_FMBROWSER_H__FF5AE266_8360_4FBF_9C5E_FC4135D640B9__INCLUDED_)
#define AFX_FMBROWSER_H__FF5AE266_8360_4FBF_9C5E_FC4135D640B9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// fmBrowser.h : header file
//

#include "fmbase.h"

/////////////////////////////////////////////////////////////////////////////
// CfmBrowser command target

class AFX_EXT_CLASS CfmBrowser : public CfmBase
{
	DECLARE_DYNCREATE(CfmBrowser)

public:
	CfmBrowser();           // protected constructor used by dynamic creation
	CfmBrowser(CAxisForm* axform, struct _formR* formR, int iform, bool drawOnly);
	virtual ~CfmBrowser();

// Attributes
protected:
	class	Cbrowser* m_browser;
//	class   CbrowserX* m_browserX;
	BOOL	m_runningX;
	bool	m_drawOnly;

	CRect	m_toolRc;
	int	m_hot;
	bool	m_file;
	bool	m_hastool;
	CImageList m_hImage;
	CImageList m_cImage;

// Operations
public:
	void	Adjust(CSize size);
	void	Adjust(CPoint pt, float hR, float vR);
	void	Resize(float hR, float vR, bool font = true, bool redraw = false);
	void	Reload();
	void	Draw(CDC* dc);

	void	OnLButton(bool down, CPoint pt, int& result);
	int	OnDrag(CPoint pt, int& result);

	void	WriteAll(CString str, bool redraw = true);
	void	WriteData(CString data, bool redraw = true, int col = -1, int row = -1);
	void	SetVisible(bool visible, int col = -1);

	bool	IsFocus(HWND hWnd);
	void	ShowControl(BOOL visible);

//	bool	isBrowser() { return (m_browserX || m_browser) ? true : false; }
//	CWnd*	getBrowser() { return (m_browserX) ? (CWnd*)m_browserX : (CWnd*)m_browser; }

	bool	isBrowser()  { return (CWnd*)m_browser != nullptr ? true : false; }
	CWnd*	getBrowser() { return (CWnd*)m_browser; }


	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CfmBrowser)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	void	loadBitmap();
	int	getIndex(CPoint pt);
	CRect	getRect(int index);

	// Generated message map functions
	//{{AFX_MSG(CfmBrowser)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CfmBrowser)
	afx_msg BSTR _getText();
	afx_msg void _setText(LPCTSTR lpszNewValue);
	afx_msg BOOL _getVisible();
	afx_msg void _setVisible(BOOL bNewValue);
	afx_msg void _Refresh();
	afx_msg void _Navigate(LPCTSTR url);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FMBROWSER_H__FF5AE266_8360_4FBF_9C5E_FC4135D640B9__INCLUDED_)
