#pragma once

#include <Python.h>
#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////
// CPythonEngine
// CScriptEngine 과 동일한 public 인터페이스 - 내부만 CPython C API 사용

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
	PyObject*	m_module;		// 스크립트 실행 결과 모듈 (globals dict)
	PyObject*	m_globals;		// 전역 네임스페이스 dict
	CStringArray	m_errors;	// 에러 메시지 목록

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
	CString		m_maps;		// 현재 맵 이름 (에러 표시용)
};
