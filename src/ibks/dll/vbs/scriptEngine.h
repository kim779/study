#if !defined(AFX_SCRIPTENGINE_H__817F6AD4_EE82_4306_BCE7_1D37D2CE01CD__INCLUDED_)
#define AFX_SCRIPTENGINE_H__817F6AD4_EE82_4306_BCE7_1D37D2CE01CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// scriptEngine.h : header file
//

#include "scriptsite.h"

/////////////////////////////////////////////////////////////////////////////
// CScriptEngine command target

class AFX_EXT_CLASS CScriptEngine : public COleDispatchDriver
{
public:
	CScriptEngine(CWnd* parent);           // protected constructor used by dynamic creation
	virtual ~CScriptEngine();

// Attributes
public:
	bool	m_invoke;

protected:
	CWnd*	m_parent;
	CScriptSite	m_ssite;
	IDispatch*	m_dispatch;
	IActiveScript*	m_as;
	IActiveScriptParse* m_asp;

// Operations
public:
	void	Initialize(CString maps);
	void	AddObject(CString names, CCmdTarget* object, DWORD flag = 0);

	bool	LoadScript(CString scripts);
	bool	UnloadScript();

	bool	IsAvailable(CString procs);
	bool	DoProcedure(CString procs, WPARAM wParam = 0, LPARAM lParam = 0, int key = 0);
	bool	DoProcedure(CString procs, CString data, int count = -1);
	CStringArray* GetErrorMessages();
	
	void	Close();

// Implementation
private:
	bool	getIDOfProcedure(CString procs, DISPID* pID);
	bool	verify(HRESULT hr, CString msg);
	DWORD	getParameter(CString data);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCRIPTENGINE_H__817F6AD4_EE82_4306_BCE7_1D37D2CE01CD__INCLUDED_)
