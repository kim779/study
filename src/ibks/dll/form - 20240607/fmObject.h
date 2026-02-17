#if !defined(AFX_FMOBJECT_H__CFEDCE3F_9DDA_46D1_AACA_F837B8F902D3__INCLUDED_)
#define AFX_FMOBJECT_H__CFEDCE3F_9DDA_46D1_AACA_F837B8F902D3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// fmObject.h : header file
//

#include "fmBase.h"

/////////////////////////////////////////////////////////////////////////////
// CfmObject command target

class AFX_EXT_CLASS CfmObject : public CfmBase
{
	DECLARE_DYNCREATE(CfmObject)

public:
	CfmObject();	           // protected constructor used by dynamic creation
	CfmObject(CAxisForm* axform, struct _formR* formR, int iform);
	virtual ~CfmObject();

// Attributes
protected:
	enum	{ bkNone=0, bkBrush, bkBmp } m_type;
	class	Cimage*	m_pic;
	CString	m_mapN;

// GO_FIX : Resizable
	int	m_bottom_gap;
	int	m_right_gap;

// FA_ENUM : fit to bottom
	int	m_height;

// Operations
public:
	void	Adjust(CPoint pt, float hR, float vR);
	void	Resize(float hRatio, float vRatio, bool font = true, bool redraw = false);
	void	Draw(CDC* dc);

	void	SetEnable(bool enable);
	void	SetBkColor(int rgb, int col = -1, int row = -1);
	void	WriteData(CString data, bool redraw = true, int col = -1, int row = -1);
	void	ClearData(bool selectOnly = false);

	void	Refresh();

	void	SetObject(CString maps);
	CString	GetObject();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CfmObject)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	void	setRect(int kind, long value);
	void	setRect(CRect rect);

	// Generated message map functions
	//{{AFX_MSG(CfmObject)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CfmObject)
	afx_msg BSTR _getText();
	afx_msg void _setText(LPCTSTR lpszNewValue);
	afx_msg long _getPRgb();
	afx_msg void _setPRgb(long nNewValue);
	afx_msg BOOL _getVisible();
	afx_msg void _setVisible(BOOL bNewValue);
	afx_msg BOOL _getEnable();
	afx_msg void _setEnable(BOOL bNewValue);
	afx_msg void _Clear();
	afx_msg void _ClearAll(long type);
	afx_msg void _Send();
	afx_msg void _RSend();
	afx_msg void _SetObject(LPCTSTR maps);
	afx_msg BSTR _GetObject();
	afx_msg void _Refresh();
	afx_msg void _Proc(LPCTSTR procs, LPCTSTR data);
	afx_msg void _Reload();
	afx_msg void _ProcEx(LPCTSTR procs, LPCTSTR data, long count);
	afx_msg void _SendTR(LPCTSTR trN);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FMOBJECT_H__CFEDCE3F_9DDA_46D1_AACA_F837B8F902D3__INCLUDED_)
