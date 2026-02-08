// CX_WebView.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "CX_WebView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CCX_WebViewApp

BEGIN_MESSAGE_MAP(CCX_WebViewApp, CWinApp)
	//{{AFX_MSG_MAP(CCX_WebViewApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCX_WebViewApp construction

CCX_WebViewApp::CCX_WebViewApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CCX_WebViewApp object

CCX_WebViewApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CCX_WebViewApp initialization

BOOL CCX_WebViewApp::InitInstance()
{
	// Register all OLE server (factories) as running.  This enables the
	//  OLE libraries to create objects from other applications.
	COleObjectFactory::RegisterAll();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Special entry points required for inproc servers

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AfxDllGetClassObject(rclsid, riid, ppv);
}

STDAPI DllCanUnloadNow(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AfxDllCanUnloadNow();
}

// by exporting DllRegisterServer, you can use regsvr.exe
STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	COleObjectFactory::UpdateRegistryAll();
	return S_OK;
}

#include "EdgeWnd.h"
#include "IEWnd.h"
__declspec(dllexport) CWnd* WINAPI axCreate(CWnd* parent, void* pParam)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	//컴퓨터\HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Edge\IEToEdge
	CString stmp;
	LONG lResult{};
	HKEY hKey{};

	//lResult = RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Edge\\IEToEdge"), 0, KEY_QUERY_VALUE, &hKey);

	wil::unique_cotaskmem_string version_info;
	HRESULT hr = GetAvailableCoreWebView2BrowserVersionString(nullptr, &version_info);
	stmp.Format("\r\n [edge][szSubKey]  version_info=[%s]", CString(version_info.get()));
	OutputDebugString(stmp);

	BOOL binstalled{};
	if (CString(version_info.get()).IsEmpty())
		lResult = TRUE;
	else
		lResult = ERROR_SUCCESS;

	CString _sroot{};
	_sroot.Format("%s\\tab\\WEBINFO.INI", (char*)parent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, homeCC), 0));
	char	buf[32]{};
	GetPrivateProfileString("cx_webEdge", "browser", _T(""), buf, sizeof(buf), _sroot);
	CString sBrowser;
	sBrowser.Format("%s", buf); sBrowser.TrimRight();

	CString strTarget{};

	char sbuf[1024]{};
	GetModuleFileName(nullptr, sbuf, 260);
	strTarget.Format("%s", sbuf);
	strTarget.TrimRight();

	if (strTarget.Find("86") >= 0 || strTarget.Find("Program") >= 0)
	//if(strTarget.Find("86") >= 0 || strTarget.Find("HTS") >= 0)
		sBrowser = "IE";

	if (lResult == ERROR_SUCCESS && sBrowser != "IE")  //edge가 있으면..  설정파일도 IE가 아니면
	{
		CEdgeWnd* pControlWnd = new CEdgeWnd();
		pControlWnd->m_pParent = parent;

		pControlWnd->SetParam((struct _param*)pParam);
		pControlWnd->Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, pControlWnd->m_Param.rect, parent, 100);

		return pControlWnd;
	}
	else
	{
		CIEWnd* pControlWnd = new CIEWnd();
		pControlWnd->m_pParent = parent;

		pControlWnd->SetParam((struct _param*)pParam);
		pControlWnd->Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, pControlWnd->m_Param.rect, parent, 100);

		return pControlWnd;
	}

	//DWORD keyValue[20];
	//memset(keyValue, 0, sizeof(keyValue));
	//DWORD dwType = REG_DWORD;
	//DWORD dwSize = sizeof(keyValue);
	//lResult = RegQueryValueEx(hKey, _T("SiteListUrlBucket"), 0, &dwType, (LPBYTE)keyValue, &dwSize);
	//int ivalue = keyValue[0];



}