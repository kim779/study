#if !defined(AFX_PCTRMAINWND_H__80EE97FF_650D_41AE_B4E7_1474C0ECD953__INCLUDED_)
#define AFX_PCTRMAINWND_H__80EE97FF_650D_41AE_B4E7_1474C0ECD953__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PcTrMainWnd.h : header file
//

class CPCtrMainWnd : public CWnd
{
	// Construction
public:
	CPCtrMainWnd();

	// Attributes
public:

	// Operations
public:

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPCtrMainWnd)
	public:
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

	// Implementation
public:
	BOOL	OpenWindow();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPCtrMainWnd)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif //AFX_PCTRMAINWND_H__80EE97FF_650D_41AE_B4E7_1474C0ECD953__INCLUDED_

