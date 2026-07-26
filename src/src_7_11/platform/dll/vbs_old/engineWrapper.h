#pragma once

#include "scriptEngine.h"
#include "pythonEngine.h"
#include <vector>

/////////////////////////////////////////////////////////////////////////////
// CEngineWrapper
// Auto-selects VBS or Python engine per screen based on script content.
// Python is used when the script contains Python syntax (def/import).

struct PendingObject {
    CString      name;
    CCmdTarget*  object;
    DWORD        flag;
};

class AFX_EXT_CLASS CEngineWrapper
{
public:
    CEngineWrapper(CWnd* parent);
    virtual ~CEngineWrapper();

// Attributes
public:
    bool    m_invoke;

// Operations
public:
    void    Initialize(CString maps);
    void    AddObject(CString names, CCmdTarget* object, DWORD flag = 0);

    bool    LoadScript(CString scripts, int scpKind = -1);   // scpKind: -1=auto-detect(legacy def/import scan), 0=VBScript, 1=Python
    bool    UnloadScript();

    bool    IsAvailable(CString procs);
    bool    DoProcedure(CString procs, WPARAM wParam = 0, LPARAM lParam = 0, int key = 0);
    bool    DoProcedure(CString procs, CString data, int count = -1);
    CStringArray* GetErrorMessages();

    void    Close();

// Implementation
private:
    CWnd*           m_parent;
    bool            m_usePython;
    CScriptEngine*  m_vbs;
    CPythonEngine*  m_py;

    CString                  m_pendingMaps;
    std::vector<PendingObject> m_pendingObjects;

    bool    isPythonScript(const CString& scripts);
    void    ensureEngine(bool usePython);
};
