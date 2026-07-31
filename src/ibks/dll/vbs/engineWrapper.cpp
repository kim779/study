// engineWrapper.cpp

#include "stdafx.h"
#include "engineWrapper.h"
#include "../../h/axlog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Stack of map names for the screen(s) currently running script, innermost
// last. A stack (not a single variable) because DoProcedure can re-enter
// itself on the same thread (e.g. Screen.Send(0) inside OnClick synchronously
// reaching OnSend, or Screen.Proc() jumping into another screen's script).
static std::vector<CString> s_mapStack;

CString CEngineWrapper::GetCurrentMap()
{
    return s_mapStack.empty() ? CString() : s_mapStack.back();
}

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

bool CEngineWrapper::LoadScript(CString scripts, int scpKind)
{
    bool usePython = (scpKind == -1) ? isPythonScript(scripts) : (scpKind != 0);

    axlog(LOG_SCRIPT, "CEngineWrapper::LoadScript scpKind=%d -> engine=%s (source=%s)",
        scpKind, usePython ? "Python" : "VBScript",
        (scpKind == -1) ? "auto-detect(legacy text scan)" : "explicit pythonMode");

    ensureEngine(usePython);

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
    // Nested calls (e.g. Screen.Send(0) inside OnClick synchronously reaching
    // OnSend) are safe in CPython on the same thread, so no re-entrancy guard
    // is needed here - matches VBScript's behavior below.
    if (m_usePython && m_py)
        return m_py->IsAvailable(procs);
    if (m_vbs)
        return m_vbs->IsAvailable(procs);
    return false;
}

bool CEngineWrapper::DoProcedure(CString procs, WPARAM wParam, LPARAM lParam, int key)
{
    axlog(LOG_EVENT, "DoProcedure(1) procs=%s engine=%s",
        procs.GetString(), (m_usePython && m_py) ? "Python" : (m_vbs ? "VBS" : "none"));

    s_mapStack.push_back(m_pendingMaps);
    bool result;
    if (m_usePython && m_py) {
        result   = m_py->DoProcedure(procs, wParam, lParam, key);
        m_invoke = m_py->m_invoke;
    } else if (m_vbs) {
        result   = m_vbs->DoProcedure(procs, wParam, lParam, key);
        m_invoke = m_vbs->m_invoke;
    } else {
        result = false;
    }
    s_mapStack.pop_back();
    return result;
}

bool CEngineWrapper::DoProcedure(CString procs, CString data, int count)
{
    axlog(LOG_EVENT, "DoProcedure(2) procs=%s engine=%s",
        procs.GetString(), (m_usePython && m_py) ? "Python" : (m_vbs ? "VBS" : "none"));

    s_mapStack.push_back(m_pendingMaps);
    bool result;
    if (m_usePython && m_py) {
        result   = m_py->DoProcedure(procs, data, count);
        m_invoke = m_py->m_invoke;
    } else if (m_vbs) {
        result   = m_vbs->DoProcedure(procs, data, count);
        m_invoke = m_vbs->m_invoke;
    } else {
        result = true;
    }
    s_mapStack.pop_back();
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
    CString text = scripts;
    CString line;
    int idx;

    for (; !text.IsEmpty(); )
    {
        idx = text.Find('\n');
        if (idx != -1)
        {
            line = text.Left(idx);
            text = text.Mid(idx + 1);
        }
        else
        {
            line = text;
            text.Empty();
        }

        line.TrimLeft();
        if (line.Find("def ") == 0 || line.Find("import ") == 0)
            return true;
    }

    return false;
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
