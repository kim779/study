// engineWrapper.cpp

#include "stdafx.h"
#include "engineWrapper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

CEngineWrapper::CEngineWrapper(CWnd* parent)
    : m_parent(parent)
    , m_invoke(false)
    , m_usePython(false)
    , m_vbs(NULL)
    , m_py(NULL)
{
}

CEngineWrapper::~CEngineWrapper()
{
    delete m_vbs;
    delete m_py;
}

void CEngineWrapper::Initialize(CString maps)
{
    m_pendingMaps = maps;
    m_pendingObjects.clear();
    m_invoke     = false;
    m_usePython  = false;

    delete m_vbs; m_vbs = NULL;
    delete m_py;  m_py  = NULL;
}

void CEngineWrapper::AddObject(CString names, CCmdTarget* object, DWORD flag)
{
    if (m_usePython && m_py) {
        m_py->AddObject(names, object, flag);
    } else if (!m_usePython && m_vbs) {
        m_vbs->AddObject(names, object, flag);
    } else {
        PendingObject po;
        po.name   = names;
        po.object = object;
        po.flag   = flag;
        m_pendingObjects.push_back(po);
    }
}

bool CEngineWrapper::LoadScript(CString scripts)
{
    ensureEngine(isPythonScript(scripts));

    if (m_usePython)
        return m_py->LoadScript(scripts);
    return m_vbs->LoadScript(scripts);
}

bool CEngineWrapper::UnloadScript()
{
    if (m_usePython && m_py)
        return m_py->UnloadScript();
    if (m_vbs)
        return m_vbs->UnloadScript();
    return true;
}

bool CEngineWrapper::IsAvailable(CString procs)
{
    if (m_usePython && m_py) {
        if (m_py->m_invoke) return false;  // re-entrancy guard: Python is busy
        return m_py->IsAvailable(procs);
    }
    if (m_vbs)
        return m_vbs->IsAvailable(procs);
    return false;
}

bool CEngineWrapper::DoProcedure(CString procs, WPARAM wParam, LPARAM lParam, int key)
{
    bool result;
    if (m_usePython && m_py) {
        result   = m_py->DoProcedure(procs, wParam, lParam, key);
        m_invoke = m_py->m_invoke;
    } else if (m_vbs) {
        result   = m_vbs->DoProcedure(procs, wParam, lParam, key);
        m_invoke = m_vbs->m_invoke;
    } else {
        return false;
    }
    return result;
}

bool CEngineWrapper::DoProcedure(CString procs, CString data, int count)
{
    bool result;
    if (m_usePython && m_py) {
        result   = m_py->DoProcedure(procs, data, count);
        m_invoke = m_py->m_invoke;
    } else if (m_vbs) {
        result   = m_vbs->DoProcedure(procs, data, count);
        m_invoke = m_vbs->m_invoke;
    } else {
        return true;
    }
    return result;
}

CStringArray* CEngineWrapper::GetErrorMessages()
{
    if (m_usePython && m_py)
        return m_py->GetErrorMessages();
    if (m_vbs)
        return m_vbs->GetErrorMessages();
    return NULL;
}

void CEngineWrapper::Close()
{
    if (m_usePython && m_py)
        m_py->Close();
    else if (m_vbs)
        m_vbs->Close();
}

/////////////////////////////////////////////////////////////////////////////
// Private helpers

bool CEngineWrapper::isPythonScript(const CString& scripts)
{
    // Python scripts contain 'def ' or 'import ' at the start of a line.
    // VBScript uses 'Sub ' or 'Function '.
    return (scripts.Find("def ") >= 0 || scripts.Find("import ") >= 0);
}

void CEngineWrapper::ensureEngine(bool usePython)
{
    m_usePython = usePython;

    if (usePython) {
        if (!m_py) {
            m_py = new CPythonEngine(m_parent);
            m_py->Initialize(m_pendingMaps);
            for (size_t i = 0; i < m_pendingObjects.size(); i++)
                m_py->AddObject(m_pendingObjects[i].name,
                                m_pendingObjects[i].object,
                                m_pendingObjects[i].flag);
        }
    } else {
        if (!m_vbs) {
            m_vbs = new CScriptEngine(m_parent);
            m_vbs->Initialize(m_pendingMaps);
            for (size_t i = 0; i < m_pendingObjects.size(); i++)
                m_vbs->AddObject(m_pendingObjects[i].name,
                                 m_pendingObjects[i].object,
                                 m_pendingObjects[i].flag);
        }
    }

    m_pendingObjects.clear();
}
