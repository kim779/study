// scriptEngine.cpp : implementation file
//

#include "stdafx.h"
#include "scriptEngine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScriptEngine

CScriptEngine::CScriptEngine(CWnd* parent)
{
	m_parent   = parent;

	m_as       = NULL; 
	m_asp      = NULL;
	m_dispatch = NULL;
	m_invoke   = false;
}

CScriptEngine::~CScriptEngine()
{
}

/////////////////////////////////////////////////////////////////////////////
// CScriptEngine message handlers

void CScriptEngine::Initialize(CString maps)
{
	HRESULT	hr;
	CLSID	clsid;

	m_ssite.m_errors.RemoveAll();

	// Find CLSID
	hr = CLSIDFromProgID(L"VBScript", &clsid);
	if (!verify(hr, "Scripting Engine (VBScript.dll) not registered"))
		return;

	// Start VBScript.dll (inproc server)
	hr = CoCreateInstance(clsid, NULL, CLSCTX_INPROC_SERVER, IID_IActiveScript, (void**)&m_as);
	if (!verify(hr, "Scripting Engine (VBScript.dll) not started"))
		return;

	// QueryInterface for IActiveScriptParse
	hr = m_as->QueryInterface(IID_IActiveScriptParse, (void**)&m_asp);
	if (!verify(hr, "Scripting Engine Parser not available"))
	{
		m_as->Release();
		m_as       = NULL;
		m_asp      = NULL;
		m_dispatch = NULL;
		return;
	}

	// Setup our ScriptSite
	hr = m_as->SetScriptSite(&m_ssite);
	if (!verify(hr, "Could not host scripting site"))
	{
		m_as->Release();
		m_asp->Release();
		m_as       = NULL;
		m_asp      = NULL;
		m_dispatch = NULL;
		return;
	}

	hr = m_asp->InitNew();
	if (!verify(hr, "Could not initialize scripting parser"))
	{
		m_as->Release();
		m_asp->Release();
		m_as       = NULL;
		m_asp      = NULL;
		m_dispatch = NULL;
		return;
	}

	// Initialize m_pScriptDispatch to NULL
	m_dispatch = NULL;
	m_ssite.m_objects.RemoveAll();

	// Set the application main window as ScriptWindow parent
	m_ssite.m_hWnd = m_parent->m_hWnd;
	m_ssite.m_maps = maps;
}

void CScriptEngine::AddObject(CString names, CCmdTarget* object, DWORD flag)
{
	USES_CONVERSION;

	HRESULT	hr;
	CString	string;
	CCmdTarget* target;
// updateXXX__20220X
	if (::isdigit(static_cast<unsigned char>(names.GetAt(0))))
		names.Insert(0, 'S');
	if (!m_ssite.m_objects.Lookup(names, (CObject*&)target))
	{
		hr = m_as->AddNamedItem(T2OLE(names), SCRIPTITEM_ISVISIBLE|SCRIPTITEM_ISSOURCE|flag);
		string.Format(_T("Could not add Exposed-Script-Object [%s]"), names.GetString());
		if (!verify(hr, string))
			return;
	}
	m_ssite.m_objects.SetAt(names, object);
}

bool CScriptEngine::LoadScript(CString scripts)
{
	HRESULT	hr;
	BSTR	strParse;
	EXCEPINFO ei;

	strParse = scripts.AllocSysString();
	// This loads the script and keeps it around for the user
	// to allow to call specific methods in the script
	// Add script code to the engine
	m_ssite.SetScript(scripts);
	hr = m_asp->ParseScriptText(strParse, nullptr, nullptr, nullptr, 0, 0, 0L, nullptr, &ei);
	::SysFreeString(strParse);
	if (!verify(hr, "Error parsing script text"))
	{
		m_as->Release();
		m_asp->Release();
		m_as       = NULL;
		m_asp      = NULL;
		m_dispatch = NULL;
		return false;
	}

	// This will actually run the script
	hr = m_as->SetScriptState(SCRIPTSTATE_CONNECTED);
	if (!verify(hr, "Error running the script"))
	{
		m_as->Release();
		m_asp->Release();
		m_as       = NULL;
		m_asp      = NULL;
		m_dispatch = NULL;
		return false;
	}

	// We need the IDispatch for the script itself
	// to allow to use InvokeHelper
	hr = m_as->GetScriptDispatch(0, &m_dispatch);
	if (!verify(hr, "Error getting script dispatch"))
	{
		m_as->Release();
		m_asp->Release();
		m_as       = NULL;
		m_asp      = NULL;
		m_dispatch = NULL;
		return false;
	}

	// Attach for easy access to InvokeHelper
	// ScriptEngineFactory is actually also derived from COleDispatchDriver,
	// So we can call AttachDispatch to use it's InvokeHelper method
	AttachDispatch(m_dispatch);
	return true;
}

bool CScriptEngine::UnloadScript()
{
	// Release script dispatch pointer from this
	DetachDispatch();

	// Release all interfaces
	if (m_dispatch)
		m_dispatch->Release();
	if (m_asp)
		m_asp->Release();
	if (m_as)
	{
		m_as->Close();
		m_as->Release();
	}

	m_as       = NULL;
	m_asp      = NULL;
	m_dispatch = NULL;

	return true;
}

bool CScriptEngine::IsAvailable(CString procs)
{
	DISPID	dispid;

	return getIDOfProcedure(procs, &dispid);
}

bool CScriptEngine::DoProcedure(CString procs, WPARAM wParam, LPARAM lParam, int key)
{
	long	result;
	DISPID	dispid;
	
	if (getIDOfProcedure(procs, &dispid))
	{
		m_invoke = true;
		TRY
		{
			if (!procs.CompareNoCase("AX_SUB_OnService_AX_"))
			{
				InvokeHelper(dispid, DISPATCH_METHOD, VT_EMPTY, (void *)&result,
					(BYTE *)(VTS_BSTR VTS_I4), (char *)lParam, (int)wParam);
			}
			else if (!procs.CompareNoCase("AX_SUB_OnApprove_AX_"))
			{
				InvokeHelper(dispid, DISPATCH_METHOD, VT_EMPTY, (void *)&result,
					(BYTE *)(VTS_I4 VTS_BSTR VTS_I4), key, (char *)lParam, (int)wParam);
			}
			else if (!procs.CompareNoCase("AX_SUB_OnSelect_AX_"))
			{
				InvokeHelper(dispid, DISPATCH_METHOD, VT_EMPTY, (void *)&result,
					(BYTE *)(VTS_BSTR), (char *)lParam);
			}
			else if (!procs.CompareNoCase("AX_SUB_OnFile_AX_"))
			{
				InvokeHelper(dispid, DISPATCH_METHOD, VT_EMPTY, (void *)&result,
					(BYTE *)(VTS_BOOL VTS_BSTR VTS_I4), key, (char *)lParam, (int)wParam);
			}
			else if (!procs.CompareNoCase("AX_SUB_OnAlert_AX_"))
			{
				InvokeHelper(dispid, DISPATCH_METHOD, VT_EMPTY, (void *)&result,
					(BYTE *)(VTS_BSTR VTS_BSTR VTS_I4), (char *)wParam, (char *)lParam, key);
			}
			else if (!procs.CompareNoCase("AX_SUB_OnTimerX_AX_"))
			{
				InvokeHelper(dispid, DISPATCH_METHOD, VT_EMPTY, (void *)&result,
					(BYTE *)(VTS_I4), (int)wParam);
			}
		//	else if (!procs.CompareNoCase("AX_SUB_OnReceive_AX_"))
		//	{
		//		InvokeHelper(dispid, DISPATCH_METHOS, VT_EMPTY, (void *)&result,
		//			(BYTE *)(VTS_BSTR), (char *)lParam);
		//	}
			else
				InvokeHelper(dispid, DISPATCH_METHOD, VT_EMPTY, (void *)&result, NULL);
		}
		CATCH (COleException, e)
		{
			m_invoke = false;
			return false;
		}
		CATCH (COleDispatchException, e1)
		{
			m_invoke = false;
			return false;
		}
		END_CATCH

		m_invoke = false;
		return true;
	}

	return false;
}

bool CScriptEngine::DoProcedure(CString procs, CString data, int count)
{
	long	result;
	DISPID	dispid;

	if (getIDOfProcedure(procs, &dispid))
	{
		m_invoke = true;
		TRY
		{
			int	value;
			CString	text;
			CStringArray arrs;
			BYTE*	paramInfo = NULL;
			DWORD*	paramPtr  = NULL;
			const	int	max_param = 10;

			arrs.RemoveAll();
		//	data.TrimLeft(); data.TrimRight();
			for ( ; !data.IsEmpty(); )
			{
				value = data.Find(',');
				if (value != -1)
				{
					arrs.Add(data.Left(value));
					data = data.Mid(value+1);
				}
				else
				{
					arrs.Add(data);
					data = _T("");
				}
			}

			paramPtr = new DWORD[max_param];
			ZeroMemory(paramPtr, max_param*sizeof(DWORD));
			paramInfo = new BYTE[max_param+1];
			ZeroMemory(paramInfo, max_param+1);
			count = (count < 0) ? arrs.GetSize() : count;
			for (int ii = 0; ii < count; ii++)
			{
				if (ii < arrs.GetSize())
				{
					text = arrs.GetAt(ii);
					text.TrimLeft();
					text.TrimRight();
				}
				else
					text.Empty();

				paramInfo[ii] = 0x0e;		// VTS_BSTR
				paramPtr[ii]  = getParameter(text);
			}

			InvokeHelper(dispid, DISPATCH_METHOD, VT_EMPTY, (void *)&result,
						paramInfo, (void *)paramPtr[0], (void *)paramPtr[1], (void *)paramPtr[2],
						(void *)paramPtr[3], (void *)paramPtr[4], (void *)paramPtr[5],
						(void *)paramPtr[6], (void *)paramPtr[7], (void *)paramPtr[8], (void *)paramPtr[9]);

			for (int ii = 0; ii < max_param; ii++)
			{
				if (paramPtr[ii] && paramInfo[ii])
					delete[] (void *)paramPtr[ii];
			}
			delete[] paramPtr;

			if (paramInfo)
				delete[] paramInfo;
		}
		CATCH (COleException, e)
		{
			m_invoke = false;
			return false;
		}
		END_CATCH

		m_invoke = false;
	}

	return true;
}

CStringArray* CScriptEngine::GetErrorMessages()
{
	return &m_ssite.m_errors;
}

bool CScriptEngine::getIDOfProcedure(CString procs, DISPID* pID)
{
	if (!m_dispatch)
		return false;

	BSTR	strProc = procs.AllocSysString();
	HRESULT hr= m_dispatch->GetIDsOfNames(IID_NULL, &strProc, 1, LOCALE_SYSTEM_DEFAULT, pID);
	::SysFreeString(strProc);
	return (hr == S_OK) ? true : false;
}

bool CScriptEngine::verify(HRESULT hr, CString msg)
{
	if (FAILED(hr))
	{
		CString	string;

		string.Format("COM Error: 0x%08lx\n", hr);
		string += msg;
		m_ssite.m_errors.Add(string);
		return false;
	}

	return true;
}

DWORD CScriptEngine::getParameter(CString data)
{
	char*	sparam;
	int	len = data.GetLength();

	sparam = new char[len+1];
	CopyMemory(sparam, data, len);
	sparam[len] = '\0';
	return (DWORD)sparam;
}

void CScriptEngine::Close()
{
	m_as->Close();
}
