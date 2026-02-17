#if !defined(AFX_SCRIPTSITE_H__32E77F7A_83BA_40FB_8A09_39064541B867__INCLUDED_)
#define AFX_SCRIPTSITE_H__32E77F7A_83BA_40FB_8A09_39064541B867__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ScriptSite.h : header file
//

#include <afxtempl.h>
#include "activscp.h"

/////////////////////////////////////////////////////////////////////////////
// CScriptSite

class CScriptSite : public IActiveScriptSite, public IActiveScriptSiteWindow
{
public:
	CScriptSite();
	virtual ~CScriptSite();

// Attributes
public:
	CMapStringToOb	m_objects;
	CStringArray	m_errors;
	HWND		m_hWnd;
	CString		m_maps;

protected:
	CString		m_scripts;

private:
	ULONG		m_dwRef;

// Operations
public:
	void	SetScript(CString scripts)	{ m_scripts = scripts; }

	// IUnknown Methods
	virtual	HRESULT	_stdcall QueryInterface(REFIID riid, void** ppvObject);
	virtual	ULONG	_stdcall AddRef();
	virtual	ULONG	_stdcall Release();

	// IActiveScriptSite methods
	virtual	HRESULT	_stdcall GetLCID(LCID* plcid);
	virtual	HRESULT	_stdcall GetItemInfo(LPCOLESTR pstrName, DWORD dwReturnMask, IUnknown** ppunkItem, ITypeInfo** ppti);
	virtual	HRESULT	_stdcall GetDocVersionString(BSTR* pbstrVersion);
	virtual	HRESULT	_stdcall OnScriptTerminate(const VARIANT* pvarResult, const EXCEPINFO* pexcepInfo);
	virtual	HRESULT	_stdcall OnStateChange(SCRIPTSTATE ssScriptState);
	virtual	HRESULT	_stdcall OnScriptError(IActiveScriptError* pScriptError);
	virtual	HRESULT	_stdcall OnEnterScript(void);
	virtual	HRESULT	_stdcall OnLeaveScript(void);

	// IActiveScriptSiteWindow methods
	virtual	HRESULT	_stdcall GetWindow(HWND* phWnd);
	virtual	HRESULT	_stdcall EnableModeless(BOOL fEnable);

protected:
	CString	getSource(int line, CString& subs);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCRIPTSITE_H__32E77F7A_83BA_40FB_8A09_39064541B867__INCLUDED_)
