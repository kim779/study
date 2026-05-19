// ScriptSite.cpp : implementation file
//

#include "stdafx.h"
#include "scriptSite.h"
#include "vbGuide.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScriptSite

CScriptSite::CScriptSite()
{
	m_objects.RemoveAll();
	m_errors.RemoveAll();
	m_hWnd = NULL;

	m_dwRef = 1;
}

CScriptSite::~CScriptSite()
{
	m_objects.RemoveAll();
	m_errors.RemoveAll();
}


HRESULT CScriptSite::QueryInterface(REFIID riid, void** ppvObject)
{
	HRESULT	hr = S_OK;

	if (riid == IID_IActiveScriptSiteWindow)
		*ppvObject = (IActiveScriptSiteWindow *)this;
	else
	{
		*ppvObject = NULL;
		hr = E_NOTIMPL;
	}

	return hr;
}

ULONG CScriptSite::AddRef()
{
	return ++m_dwRef;
}

ULONG CScriptSite::Release()
{
	if (m_dwRef == 0)
		return 0;

	return --m_dwRef;
}

HRESULT CScriptSite::GetLCID(LCID* plcid)
{
	return S_OK;
}

HRESULT CScriptSite::GetItemInfo(LPCOLESTR pstrName, DWORD dwReturnMask, IUnknown** ppunkItem, ITypeInfo** ppti)
{
	USES_CONVERSION;

	if (ppti)
	{
		*ppti = NULL;
		if (dwReturnMask & SCRIPTINFO_ITYPEINFO)
			return TYPE_E_ELEMENTNOTFOUND;
	}

	if (ppunkItem)
	{
		*ppunkItem = NULL;
		if (dwReturnMask & SCRIPTINFO_IUNKNOWN)
		{
			CCmdTarget* target;
			if (m_objects.Lookup(OLE2T(pstrName), (CObject*&)target) && target != NULL)
			{
				*ppunkItem = target->GetIDispatch(TRUE);
			//	*ppunkItem = target->GetInterface(&IID_IUnknown);
				if (*ppunkItem)
				{
					(*ppunkItem)->AddRef();
					return S_OK;
				}
			}
		}
	}

	return TYPE_E_ELEMENTNOTFOUND;
/*
	if (dwReturnMask & SCRIPTINFO_ITYPEINFO)
	{
		if (!ppti)
			return E_INVALIDARG;
		*ppti = NULL;
	}

	if (dwReturnMask & SCRIPTINFO_IUNKNOWN)
	{
		if (!ppiunkItem)
			return E_INVALIDARG;
		*ppiunkItem = NULL;
	}

	// Global object
	if (!_wcsicmp(L"Screen", pstrName))
	{
		if (dwReturnMask & SCRIPTINFO_ITYPEINFO)
		{
			pThis->GetTypeInfoOfGuid(0 // lcid unknown!
						, CLSID_MfcaxscrvbDlg, ppti);
			(*ppti)->AddRef();      // because returning
		}
		
		if (dwReturnMask & SCRIPTINFO_IUNKNOWN)
		{
			*ppiunkItem = pThis->GetIDispatch(TRUE);
			(*ppiunkItem)->AddRef();    // because returning
		}
		return S_OK;
	}

	// Check map for named item
	CCmdTarget* pdispimpl = NULL;
	if (!(pThis->mapNamedItems.Lookup(OLE2CT(pstrName), (CObject*&) pdispimpl)))
	{
		return TYPE_E_ELEMENTNOTFOUND;
	}

	LPUNKNOWN pUnk = NULL;
	ASSERT(NULL != pdispimpl);
	if (pdispimpl->IsKindOf(RUNTIME_CLASS(CCmdTargetPlus)))
		// Automation class
		pUnk = pdispimpl->GetIDispatch(TRUE);
	else
		// Wrapper Class probably
		pUnk = ((CWnd*)pdispimpl)->GetControlUnknown();

	if (NULL == pUnk)
	{
		TRACE("No IUnknown for item!\n");
		// This shouldn't ever happen, since we're providing the items
		return E_FAIL;
	}

	if (dwReturnMask & SCRIPTINFO_ITYPEINFO)
	{
		// Use IProvideClassInfo to get ITypeInfo of coclass!
		IProvideClassInfo *pci = NULL;
		hr = pUnk->QueryInterface(IID_IProvideClassInfo, (void**)&pci);
		if (SUCCEEDED(hr) && pci)
			hr = pci->GetClassInfo(ppti);
		if (FAILED(hr))
			return E_FAIL;
	}
	
	if (dwReturnMask & SCRIPTINFO_IUNKNOWN)
	{
		*ppiunkItem = pUnk;
		(*ppiunkItem)->AddRef();    // because returning
	}

	return S_OK;
	*/
}

HRESULT CScriptSite::GetDocVersionString(BSTR* pbstrVersion)
{
	return E_NOTIMPL;
}

HRESULT CScriptSite::OnScriptTerminate(const VARIANT* pvarResult, const EXCEPINFO* pexcepInfo)
{
	return S_OK;
}

HRESULT CScriptSite::OnStateChange(SCRIPTSTATE ssScriptState)
{
	return S_OK;
}

HRESULT CScriptSite::OnScriptError(IActiveScriptError* pScriptError)
{
	EXCEPINFO ei;
	BSTR	lines;
	DWORD	context;
	ULONG	line;
	LONG	charPosition;
	CString	message, src, desc;

	pScriptError->GetExceptionInfo(&ei);
	pScriptError->GetSourceLineText(&lines);
	pScriptError->GetSourcePosition(&context, &line, &charPosition);

	desc = (LPCWSTR)ei.bstrDescription;
	src  = (LPCWSTR)ei.bstrSource;
	message.Format("\nLine:%d  Error:%x\n\n%s\n", line, (ei.wCode > 0) ? (int)ei.wCode : ei.scode, desc);
	m_errors.Add(message);
 
	CVbGuide* guide = new CVbGuide(src, message);
	guide->CreateThread();

	return S_OK;
}

HRESULT CScriptSite::OnEnterScript(void)
{
	return S_OK;
}

HRESULT CScriptSite::OnLeaveScript(void)
{
	return S_OK;
}

HRESULT CScriptSite::GetWindow(HWND* phWnd)
{
	HWND	activeWindow = NULL;
	CWnd*	activeWnd    = CWnd::GetActiveWindow();

	if (activeWnd)
		activeWindow = activeWnd->m_hWnd;

	if (!activeWindow)
	{
		activeWnd = AfxGetMainWnd();
		if (activeWnd)
			activeWindow = activeWnd->m_hWnd;
	}

	m_hWnd = activeWindow;
	*phWnd = m_hWnd;
	return S_OK;
}

HRESULT CScriptSite::EnableModeless(BOOL fEnable)
{
	return S_OK;
}

