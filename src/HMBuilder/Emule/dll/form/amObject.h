#if !defined(AFX_FMOBJECT_H__CFEDCE3F_9DDA_46D1_AACA_F837B8F902D3__INCLUDED_)
#define AFX_FMOBJECT_H__CFEDCE3F_9DDA_46D1_AACA_F837B8F902D3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// fmObject.h : header file
//

#include "ambase.h"

/////////////////////////////////////////////////////////////////////////////
// CamObject command target

class AFX_EXT_CLASS CamObject : public CamBase
{
	DECLARE_DYNCREATE(CamObject)

public:
	CamObject();	           // protected constructor used by dynamic creation
	CamObject(CAmForm* axform, struct _formR* formR);
	virtual ~CamObject();

// Attributes
protected:
	enum	{ bkNone=0, bkBrush, bkBmp } m_type;
	class	Cimage*	m_pic;
	CString	m_mapN;

// Operations
public:
	void	Resize(float hRatio, float vRatio, bool font = true, bool redraw = false);
	void	Draw(CDC* dc);

	void	SetEnable(bool enable);
	void	SetBkColor(int rgb, int col = -1, int row = -1);
	void	WriteData(CString data, bool redraw = true, int col = -1, int row = -1);
	void	ClearData(bool selectOnly = false);

	void	Refresh(bool bReload = true);

	void	SetObject(CString maps);
	CString	GetObject();

	LPCTSTR _getText();
	void _setText(LPCTSTR lpszNewValue);
	long _getPRgb();
	void _setPRgb(long nNewValue);
	BOOL _getVisible();
	void _setVisible(BOOL bNewValue);
	BOOL _getEnable();
	void _setEnable(BOOL bNewValue);
	void _Clear();
	void _ClearAll(long type);
	void _Send(int target, const char* strTR);
	void _RSend();
	void _SetObject(LPCTSTR maps);
	LPCTSTR _GetObjectName();
	luabind::object _GetObject(LPCTSTR strObj);
	void _Refresh();
	void _Proc(LPCTSTR procs, LPCTSTR data);
	void _ProcEx(LPCTSTR procs, LPCTSTR data, long count);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CamObject)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CamObject)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CamObject)
	
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FMOBJECT_H__CFEDCE3F_9DDA_46D1_AACA_F837B8F902D3__INCLUDED_)
