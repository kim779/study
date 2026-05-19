#if !defined(AFX_FMMEMO_H__EBB4339D_3AF1_48DA_81BF_A13962E32209__INCLUDED_)
#define AFX_FMMEMO_H__EBB4339D_3AF1_48DA_81BF_A13962E32209__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// fmMemo.h : header file
//

#include "amEdit.h"
#include "memo.h"

/////////////////////////////////////////////////////////////////////////////
// CamMemo command target

class AFX_EXT_CLASS CamMemo : public CamEdit
{
	DECLARE_DYNCREATE(CamMemo)

public:
	CamMemo();	           // protected constructor used by dynamic creation
	CamMemo(CAmForm* axform, struct _formR* formR, bool drawOnly);
	virtual ~CamMemo();

// Attributes
public:
	Cmemo*	m_memo;

protected:
	CWnd*	m_pWnd;
	bool	m_drawOnly;
	CBrush*	m_brush;

// Operations
public:
	void	Draw(CDC* dc);
	void	SetFocus(bool focus);
	void	Adjust(CSize size);
	void	Adjust(CPoint pt, float hR, float vR);
	void	Resize(float hR, float vR, bool font = true, bool redraw = false);
	void	Reload();

	void	ReadAll(CString& data, bool edit = false, bool head = false);
	void	ReadData(CString& data, bool edit = false, int col = -1, int row = -1);
	void	WriteAll(CString str, bool redraw = true);
	void	WriteData(CString data, bool redraw = true, int col = -1, int row = -1);
	void	SetVisible(bool visible, int col = -1);
	void	SetEnable(bool enable);

	void	ClearData(bool selectOnly = false);
	int	GetLength();
	void	Refresh();

	bool	IsFocus(HWND hWnd);
	void	ShowControl(BOOL visible);
	void	DoListBox(bool apply, bool focus = false);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CamMemo)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	void	setFont();

	// Generated message map functions
	//{{AFX_MSG(CamMemo)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CamMemo)
	afx_msg BSTR _getText();
	afx_msg void _setText(LPCTSTR lpszNewValue);
	afx_msg BOOL _getVisible();
	afx_msg void _setVisible(BOOL bNewValue);
	afx_msg BOOL _getEnable();
	afx_msg void _setEnable(BOOL bNewValue);
	afx_msg void _SetFocus();
	afx_msg void _Clear();
	afx_msg void _Refresh();
	afx_msg void _Save();
	afx_msg void _Print();
	afx_msg void _LimitText(long nChar);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FMMEMO_H__EBB4339D_3AF1_48DA_81BF_A13962E32209__INCLUDED_)
