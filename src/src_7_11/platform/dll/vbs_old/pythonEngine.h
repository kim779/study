#pragma once

#include <afxtempl.h>

// Forward declaration - Python.h is only needed in pythonEngine.cpp
struct _object;
typedef struct _object PyObject;

/////////////////////////////////////////////////////////////////////////////
// CPythonEngine - same public interface as CScriptEngine, Python C API inside

class AFX_EXT_CLASS CPythonEngine
{
public:
	CPythonEngine(CWnd* parent);
	virtual ~CPythonEngine();

// Attributes
public:
	bool	m_invoke;

protected:
	CWnd*		m_parent;
	PyObject*	m_module;		// script globals dict
	PyObject*	m_globals;		// global namespace dict
	CStringArray	m_errors;	// error message list

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
	PyObject*	getFunction(CString procs);
	void		fetchError();
	CString		m_maps;		// map name for error messages
};
