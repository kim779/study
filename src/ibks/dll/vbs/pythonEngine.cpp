// pythonEngine.cpp

#include "stdafx.h"
#include <Python.h>
#include "pythonEngine.h"
#include "VbGuide.h"

#include <vector>
#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ============================================================
// Forward declarations
// ============================================================

struct AxisObject;
struct AxisMethod;

static PyTypeObject AxisObjectType;
static PyTypeObject AxisMethodType;

// ============================================================
// Type structs
// ============================================================

struct AxisObject {
    PyObject_HEAD
    IDispatch* m_dispatch;
};

struct AxisMethod {
    PyObject_HEAD
    IDispatch* m_dispatch;
    DISPID     m_dispid;
};

// ============================================================
// ANSI string helpers (project is MultiByte)
// ============================================================

static PyObject* ansiToPy(const char* s, int len = -1)
{
    if (!s) { Py_RETURN_NONE; }
    if (len < 0) len = (int)strlen(s);
    if (len == 0) return PyUnicode_FromString("");
    int wlen = MultiByteToWideChar(CP_ACP, 0, s, len, NULL, 0);
    if (wlen <= 0) return PyUnicode_FromString("");
    std::vector<wchar_t> wbuf((size_t)wlen);
    MultiByteToWideChar(CP_ACP, 0, s, len, wbuf.data(), wlen);
    return PyUnicode_FromWideChar(wbuf.data(), wlen);
}

static void ansiToUtf8(const char* s, int len, std::vector<char>& out)
{
    int wlen = MultiByteToWideChar(CP_ACP, 0, s, len, NULL, 0);
    std::vector<wchar_t> wbuf((size_t)((std::max)(wlen, 0)) + 1);
    if (wlen > 0)
        MultiByteToWideChar(CP_ACP, 0, s, len, wbuf.data(), wlen);
    wbuf[((std::max)(wlen, 0))] = 0;

    int u8len = WideCharToMultiByte(CP_UTF8, 0, wbuf.data(), wlen, NULL, 0, NULL, NULL);
    out.resize((size_t)((std::max)(u8len, 0)) + 1);
    if (u8len > 0)
        WideCharToMultiByte(CP_UTF8, 0, wbuf.data(), wlen, out.data(), u8len, NULL, NULL);
    out[((std::max)(u8len, 0))] = 0;
}

static PyObject* cstrToPy(const CString& s)
{
    return ansiToPy(s.GetString(), s.GetLength());
}

// ============================================================
// VARIANT <-> Python conversion
// ============================================================

static bool pyToVariant(PyObject* obj, VARIANT& var)
{
    VariantInit(&var);
    if (obj == Py_None) {
        var.vt = VT_NULL;
    } else if (PyBool_Check(obj)) {
        var.vt      = VT_BOOL;
        var.boolVal = PyObject_IsTrue(obj) ? VARIANT_TRUE : VARIANT_FALSE;
    } else if (PyLong_Check(obj)) {
        var.vt   = VT_I4;
        var.lVal = PyLong_AsLong(obj);
    } else if (PyFloat_Check(obj)) {
        var.vt     = VT_R8;
        var.dblVal = PyFloat_AsDouble(obj);
    } else if (PyUnicode_Check(obj)) {
        Py_ssize_t wlen;
        wchar_t* ws = PyUnicode_AsWideCharString(obj, &wlen);
        if (!ws) return false;
        var.vt      = VT_BSTR;
        var.bstrVal = SysAllocString(ws);
        PyMem_Free(ws);
    } else if (PyBytes_Check(obj)) {
        CString s(PyBytes_AS_STRING(obj), (int)PyBytes_GET_SIZE(obj));
        var.vt      = VT_BSTR;
        var.bstrVal = s.AllocSysString();
    } else {
        PyObject* str = PyObject_Str(obj);
        if (!str) return false;
        Py_ssize_t wlen;
        wchar_t* ws = PyUnicode_AsWideCharString(str, &wlen);
        Py_DECREF(str);
        if (!ws) return false;
        var.vt      = VT_BSTR;
        var.bstrVal = SysAllocString(ws);
        PyMem_Free(ws);
    }
    return true;
}

static PyObject* variantToPy(const VARIANT& var)
{
    switch (var.vt) {
    case VT_EMPTY:
    case VT_NULL:
        Py_RETURN_NONE;
    case VT_BOOL:
        return PyBool_FromLong(var.boolVal != VARIANT_FALSE);
    case VT_I2:  return PyLong_FromLong(var.iVal);
    case VT_I4:  return PyLong_FromLong(var.lVal);
    case VT_I8:  return PyLong_FromLongLong(var.llVal);
    case VT_UI2: return PyLong_FromUnsignedLong(var.uiVal);
    case VT_UI4: return PyLong_FromUnsignedLong(var.ulVal);
    case VT_R4:  return PyFloat_FromDouble(var.fltVal);
    case VT_R8:  return PyFloat_FromDouble(var.dblVal);
    case VT_BSTR:
        if (var.bstrVal)
            return PyUnicode_FromWideChar(var.bstrVal, SysStringLen(var.bstrVal));
        Py_RETURN_NONE;
    case VT_DISPATCH:
        if (var.pdispVal) {
            AxisObject* obj = PyObject_New(AxisObject, &AxisObjectType);
            if (!obj) return NULL;
            var.pdispVal->AddRef();
            obj->m_dispatch = var.pdispVal;
            return (PyObject*)obj;
        }
        Py_RETURN_NONE;
    default:
        Py_RETURN_NONE;
    }
}

// ============================================================
// AxisObject implementation
// ============================================================

static void AxisObject_dealloc(AxisObject* self)
{
    if (self->m_dispatch)
        self->m_dispatch->Release();
    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject* AxisObject_getattro(AxisObject* self, PyObject* nameObj)
{
    if (!PyUnicode_Check(nameObj)) {
        PyErr_SetString(PyExc_AttributeError, "Attribute name must be a string");
        return NULL;
    }

    Py_ssize_t wlen;
    wchar_t* wname = PyUnicode_AsWideCharString(nameObj, &wlen);
    if (!wname) return NULL;

    DISPID dispid;
    HRESULT hr = self->m_dispatch->GetIDsOfNames(
        IID_NULL, &wname, 1, LOCALE_SYSTEM_DEFAULT, &dispid);
    PyMem_Free(wname);

    if (FAILED(hr)) {
        PyErr_Format(PyExc_AttributeError, "Object has no attribute '%U'", nameObj);
        return NULL;
    }

    AxisMethod* method = PyObject_New(AxisMethod, &AxisMethodType);
    if (!method) return NULL;
    self->m_dispatch->AddRef();
    method->m_dispatch = self->m_dispatch;
    method->m_dispid   = dispid;
    return (PyObject*)method;
}

static int AxisObject_setattro(AxisObject* self, PyObject* nameObj, PyObject* valueObj)
{
    if (!PyUnicode_Check(nameObj)) {
        PyErr_SetString(PyExc_AttributeError, "Attribute name must be a string");
        return -1;
    }

    Py_ssize_t wlen;
    wchar_t* wname = PyUnicode_AsWideCharString(nameObj, &wlen);
    if (!wname) return -1;

    DISPID dispid;
    HRESULT hr = self->m_dispatch->GetIDsOfNames(
        IID_NULL, &wname, 1, LOCALE_SYSTEM_DEFAULT, &dispid);
    PyMem_Free(wname);

    if (FAILED(hr)) {
        PyErr_Format(PyExc_AttributeError, "Object has no attribute '%U'", nameObj);
        return -1;
    }

    VARIANT var;
    if (!pyToVariant(valueObj, var)) {
        PyErr_SetString(PyExc_TypeError, "Unsupported value type for property set");
        return -1;
    }

    DISPID putID = DISPID_PROPERTYPUT;
    DISPPARAMS dp = { &var, &putID, 1, 1 };
    EXCEPINFO ei  = {};
    UINT argErr   = 0;
    hr = self->m_dispatch->Invoke(
        dispid, IID_NULL, LOCALE_SYSTEM_DEFAULT,
        DISPATCH_PROPERTYPUT, &dp, NULL, &ei, &argErr);
    VariantClear(&var);

    if (FAILED(hr)) {
        PyErr_Format(PyExc_AttributeError, "Failed to set property '%U'", nameObj);
        return -1;
    }
    return 0;
}

// ============================================================
// AxisMethod implementation
// ============================================================

static void AxisMethod_dealloc(AxisMethod* self)
{
    if (self->m_dispatch)
        self->m_dispatch->Release();
    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject* AxisMethod_call(AxisMethod* self, PyObject* args, PyObject* /*kwargs*/)
{
    Py_ssize_t nArgs = PyTuple_Size(args);

    std::vector<VARIANT> vars((size_t)nArgs);
    for (Py_ssize_t i = 0; i < nArgs; i++) {
        VariantInit(&vars[i]);
        if (!pyToVariant(PyTuple_GET_ITEM(args, i), vars[i])) {
            for (Py_ssize_t j = 0; j < i; j++) VariantClear(&vars[j]);
            PyErr_SetString(PyExc_TypeError, "Unsupported argument type");
            return NULL;
        }
    }

    DISPPARAMS dp = { NULL, NULL, 0, 0 };
    if (nArgs > 0) {
        std::reverse(vars.begin(), vars.end());
        dp.rgvarg = vars.data();
        dp.cArgs  = (UINT)nArgs;
    }

    VARIANT result;
    VariantInit(&result);
    EXCEPINFO ei = {};
    UINT argErr  = 0;

    HRESULT hr = self->m_dispatch->Invoke(
        self->m_dispid, IID_NULL, LOCALE_SYSTEM_DEFAULT,
        DISPATCH_METHOD | DISPATCH_PROPERTYGET,
        &dp, &result, &ei, &argErr);

    for (auto& v : vars) VariantClear(&v);

    if (FAILED(hr)) {
        VariantClear(&result);
        Py_RETURN_NONE;
    }

    PyObject* ret = variantToPy(result);
    VariantClear(&result);
    return ret;
}

// ============================================================
// Type registration
// ============================================================

static bool s_typesReady = false;

static void initAxisTypes()
{
    if (s_typesReady) return;

    memset(&AxisObjectType, 0, sizeof(AxisObjectType));
    Py_SET_REFCNT(&AxisObjectType, 1);
    AxisObjectType.tp_name         = "axisvbs.AxisObject";
    AxisObjectType.tp_basicsize    = sizeof(AxisObject);
    AxisObjectType.tp_dealloc      = (destructor)AxisObject_dealloc;
    AxisObjectType.tp_getattro     = (getattrofunc)AxisObject_getattro;
    AxisObjectType.tp_setattro     = (setattrofunc)AxisObject_setattro;
    AxisObjectType.tp_flags        = Py_TPFLAGS_DEFAULT;
    PyType_Ready(&AxisObjectType);

    memset(&AxisMethodType, 0, sizeof(AxisMethodType));
    Py_SET_REFCNT(&AxisMethodType, 1);
    AxisMethodType.tp_name         = "axisvbs.AxisMethod";
    AxisMethodType.tp_basicsize    = sizeof(AxisMethod);
    AxisMethodType.tp_dealloc      = (destructor)AxisMethod_dealloc;
    AxisMethodType.tp_call         = (ternaryfunc)AxisMethod_call;
    AxisMethodType.tp_flags        = Py_TPFLAGS_DEFAULT;
    PyType_Ready(&AxisMethodType);

    s_typesReady = true;
}

// ============================================================
// CPythonEngine
// ============================================================

CPythonEngine::CPythonEngine(CWnd* parent)
    : m_parent(parent)
    , m_module(NULL)
    , m_globals(NULL)
    , m_invoke(false)
{
}

CPythonEngine::~CPythonEngine()
{
    if (m_globals) {
        Py_DECREF(m_globals);
        m_globals = NULL;
    }
}

void CPythonEngine::Initialize(CString maps)
{
    m_maps = maps;
    m_errors.RemoveAll();

    if (!Py_IsInitialized()) {
        PyConfig config;
        PyConfig_InitPythonConfig(&config);
        PyConfig_SetString(&config, &config.home,
            L"C:\\Users\\IBKS\\AppData\\Local\\Programs\\Python\\Python311-32");
        Py_InitializeFromConfig(&config);
        PyConfig_Clear(&config);
    }

    initAxisTypes();

    if (m_globals) {
        Py_DECREF(m_globals);
        // Module globals form a reference cycle with the def'd functions
        // (each function keeps __globals__ alive), so a plain DECREF never
        // drops the refcount to zero - it just becomes GC-only garbage.
        // Collect now, while the COM objects AddObject() wrapped (Screen,
        // System, ...) are still valid, instead of leaving it to a later,
        // unrelated GC pass that may run after those objects are destroyed.
        PyGC_Collect();
    }

    m_globals = PyDict_New();
    PyDict_SetItemString(m_globals, "__builtins__", PyEval_GetBuiltins());
    m_module  = NULL;
}

void CPythonEngine::AddObject(CString names, CCmdTarget* object, DWORD /*flag*/)
{
    if (!m_globals || !object) return;

    if (::isdigit(static_cast<unsigned char>(names.GetAt(0))))
        names.Insert(0, 'S');

    IDispatch* pDisp = object->GetIDispatch(TRUE);
    if (!pDisp) return;

    AxisObject* axObj = PyObject_New(AxisObject, &AxisObjectType);
    if (!axObj) { pDisp->Release(); return; }
    axObj->m_dispatch = pDisp;

    PyObject* key = cstrToPy(names);
    if (key) {
        PyDict_SetItem(m_globals, key, (PyObject*)axObj);
        Py_DECREF(key);
    }
    Py_DECREF((PyObject*)axObj);
}

bool CPythonEngine::LoadScript(CString scripts)
{
    if (!m_globals) return false;

    // Strip binary section markers: 0x0E [kind] 0x0C (added by BinaryMngr::AddMapScript)
    const char* p = scripts.GetString();
    int slen = scripts.GetLength();
    std::vector<char> buf;
    buf.reserve((size_t)slen);
    for (int i = 0; i < slen; ) {
        if (i + 2 < slen && (unsigned char)p[i] == 0x0E && (unsigned char)p[i+2] == 0x0C) {
            i += 3;
        } else {
            buf.push_back(p[i++]);
        }
    }
    buf.push_back('\0');

    // Source is stored ANSI(CP949); PyRun_String assumes UTF-8, so transcode first
    // (Korean literals/comments would otherwise fail to parse as invalid UTF-8).
    std::vector<char> u8buf;
    ansiToUtf8(buf.data(), (int)buf.size() - 1, u8buf);

    PyObject* result = PyRun_String(
        u8buf.data(), Py_file_input, m_globals, m_globals);

    if (!result) {
        fetchError();
        return false;
    }
    Py_DECREF(result);
    m_module = m_globals;
    return true;
}

bool CPythonEngine::UnloadScript()
{
    if (m_globals) {
        Py_DECREF(m_globals);
        m_globals = NULL;
        // See comment in Initialize(): break the globals/functions
        // reference cycle immediately, before the caller (CScreen dtor)
        // deletes the COM objects these globals wrap (Screen, System, ...).
        PyGC_Collect();
    }
    m_module = NULL;
    return true;
}

bool CPythonEngine::IsAvailable(CString procs)
{
    PyObject* func = getFunction(procs);
    if (func) { Py_DECREF(func); return true; }
    return false;
}

bool CPythonEngine::DoProcedure(CString procs, WPARAM wParam, LPARAM lParam, int key)
{
    PyObject* func = getFunction(procs);
    if (!func) return false;

    m_invoke = true;
    PyObject* args = NULL;

    if (!procs.CompareNoCase("AX_SUB_OnService_AX_")) {
        PyObject* pData = ansiToPy((const char*)lParam, (int)wParam);
        args = PyTuple_Pack(2, pData, PyLong_FromLong((long)wParam));
        Py_DECREF(pData);
    }
    else if (!procs.CompareNoCase("AX_SUB_OnApprove_AX_")) {
        PyObject* pData = ansiToPy((const char*)lParam, (int)wParam);
        args = PyTuple_Pack(3, PyLong_FromLong(key), pData, PyLong_FromLong((long)wParam));
        Py_DECREF(pData);
    }
    else if (!procs.CompareNoCase("AX_SUB_OnSelect_AX_")) {
        PyObject* pData = ansiToPy((const char*)lParam);
        args = PyTuple_Pack(1, pData);
        Py_DECREF(pData);
    }
    else if (!procs.CompareNoCase("AX_SUB_OnFile_AX_")) {
        PyObject* pUpload = key ? Py_True : Py_False;
        PyObject* pData   = ansiToPy((const char*)lParam, (int)wParam);
        Py_INCREF(pUpload);
        args = PyTuple_Pack(3, pUpload, pData, PyLong_FromLong((long)wParam));
        Py_DECREF(pUpload);
        Py_DECREF(pData);
    }
    else if (!procs.CompareNoCase("AX_SUB_OnAlert_AX_")) {
        PyObject* pCode = ansiToPy((const char*)wParam);
        PyObject* pData = ansiToPy((const char*)lParam);
        args = PyTuple_Pack(3, pCode, pData, PyLong_FromLong(key));
        Py_DECREF(pCode);
        Py_DECREF(pData);
    }
    else if (!procs.CompareNoCase("AX_SUB_OnTimerX_AX_")) {
        args = PyTuple_Pack(1, PyLong_FromLong((long)wParam));
    }
    else {
        args = PyTuple_New(0);
    }

    PyObject* result = NULL;
    if (args) {
        result = PyObject_Call(func, args, NULL);
        Py_DECREF(args);
    }
    Py_DECREF(func);

    if (!result) {
        fetchError();
        m_invoke = false;
        return false;
    }
    Py_DECREF(result);
    m_invoke = false;
    return true;
}

bool CPythonEngine::DoProcedure(CString procs, CString data, int count)
{
    PyObject* func = getFunction(procs);
    if (!func) return true;  // match VBS behavior: true even if not found

    m_invoke = true;

    CStringArray arrs;
    CString tmp = data;
    for ( ; !tmp.IsEmpty(); ) {
        int idx = tmp.Find(',');
        if (idx != -1) {
            arrs.Add(tmp.Left(idx));
            tmp = tmp.Mid(idx + 1);
        } else {
            arrs.Add(tmp);
            tmp.Empty();
        }
    }

    int nArgs = (count < 0) ? (int)arrs.GetSize() : count;
    PyObject* args = PyTuple_New(nArgs);
    for (int i = 0; i < nArgs; i++) {
        CString s;
        if (i < (int)arrs.GetSize()) {
            s = arrs.GetAt(i);
            s.TrimLeft(); s.TrimRight();
        }
        PyTuple_SET_ITEM(args, i, cstrToPy(s));
    }

    PyObject* result = PyObject_Call(func, args, NULL);
    Py_DECREF(args);
    Py_DECREF(func);

    if (!result) {
        fetchError();
        m_invoke = false;
        return false;
    }
    Py_DECREF(result);
    m_invoke = false;
    return true;
}

CStringArray* CPythonEngine::GetErrorMessages()
{
    return &m_errors;
}

void CPythonEngine::Close()
{
    UnloadScript();
}

// ============================================================
// Private helpers
// ============================================================

PyObject* CPythonEngine::getFunction(CString procs)
{
    if (!m_globals) return NULL;

    PyObject* key  = cstrToPy(procs);
    if (!key) return NULL;
    PyObject* func = PyDict_GetItem(m_globals, key);
    Py_DECREF(key);

    if (!func || !PyCallable_Check(func)) return NULL;
    Py_INCREF(func);
    return func;
}

void CPythonEngine::fetchError()
{
    PyObject *ptype = NULL, *pvalue = NULL, *ptb = NULL;
    PyErr_Fetch(&ptype, &pvalue, &ptb);
    PyErr_NormalizeException(&ptype, &pvalue, &ptb);

    CString msg;
    if (pvalue) {
        PyObject* str = PyObject_Str(pvalue);
        if (str) {
            Py_ssize_t wlen;
            wchar_t* ws = PyUnicode_AsWideCharString(str, &wlen);
            if (ws) {
                msg.Format("Python Error [%s]: %S", m_maps.GetString(), ws);
                PyMem_Free(ws);
            }
            Py_DECREF(str);
        }

        // SyntaxError/IndentationError carry the offending source line in .text
        if (PyObject_HasAttrString(pvalue, "text")) {
            PyObject* textObj = PyObject_GetAttrString(pvalue, "text");
            if (textObj && textObj != Py_None && PyUnicode_Check(textObj)) {
                Py_ssize_t tlen;
                wchar_t* tws = PyUnicode_AsWideCharString(textObj, &tlen);
                if (tws) {
                    CString line;
                    line.Format("%S", tws);
                    line.TrimRight("\r\n");
                    msg += "\r\n\r\n>> " + line;
                    PyMem_Free(tws);
                }
            }
            Py_XDECREF(textObj);
        }

        // Runtime errors (AttributeError, TypeError, ...) don't have .text, but
        // do have a traceback with line numbers - format it via the traceback module.
        if (ptb) {
            PyObject* tbModule = PyImport_ImportModule("traceback");
            if (tbModule) {
                PyObject* formatFunc = PyObject_GetAttrString(tbModule, "format_exception");
                if (formatFunc) {
                    PyObject* list = PyObject_CallFunctionObjArgs(formatFunc, ptype, pvalue, ptb, NULL);
                    if (list) {
                        PyObject* sep = PyUnicode_FromString("");
                        PyObject* joined = PyUnicode_Join(sep, list);
                        if (joined) {
                            Py_ssize_t jlen;
                            wchar_t* jws = PyUnicode_AsWideCharString(joined, &jlen);
                            if (jws) {
                                CString tbText;
                                tbText.Format("%S", jws);
                                msg += "\r\n\r\n" + tbText;
                                PyMem_Free(jws);
                            }
                            Py_DECREF(joined);
                        }
                        Py_DECREF(sep);
                        Py_DECREF(list);
                    }
                    Py_DECREF(formatFunc);
                }
                Py_DECREF(tbModule);
            }
        }
    }
    if (msg.IsEmpty())
        msg.Format("Python Error [%s]: Unknown error", m_maps.GetString());

    m_errors.Add(msg);

    CVbGuide* guide = new CVbGuide(CString("Python Script Error"), msg);
    guide->CreateThread();

    Py_XDECREF(ptype);
    Py_XDECREF(pvalue);
    Py_XDECREF(ptb);
}
