#if !defined(AFX_AMBROWSER_H__FF5AE266_8360_4FBF_9C5E_FC4135D640B9__INCLUDED_)
#define AFX_AMBROWSER_H__FF5AE266_8360_4FBF_9C5E_FC4135D640B9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// fmBrowser.h : header file
//

#include "ambase.h"

/////////////////////////////////////////////////////////////////////////////
// CamBrowser command target

class AFX_EXT_CLASS CamBrowser : public CamBase
{
	DECLARE_DYNCREATE(CamBrowser)

public:
	CamBrowser();           // protected constructor used by dynamic creation
	CamBrowser(CAmForm* axform, struct _formR* formR, bool drawOnly);
	virtual ~CamBrowser();

// Attributes
protected:
	class	Cbrowser* m_browser;
	bool	m_drawOnly;

	CRect	m_toolRc;
	int	m_hot;
	bool	m_down;
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
	void	_SetVisible(bool visible, int col = -1);

	bool	IsFocus(HWND hWnd);
	void	ShowControl(BOOL visible);

	LPCTSTR	GetText();
	void	SetText(LPCTSTR lpszNewValue);
	BOOL	GetVisible();
	void	SetVisible(BOOL bNewValue);

	void	_Refresh();
	void	_Navigate(LPCTSTR url);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CamBrowser)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	void	loadBitmap();
	int	getIndex(CPoint pt);
	CRect	getRect(int index);

	// Generated message map functions
	//{{AFX_MSG(CamBrowser)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CamBrowser)	
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AMBROWSER_H__FF5AE266_8360_4FBF_9C5E_FC4135D640B9__INCLUDED_)
