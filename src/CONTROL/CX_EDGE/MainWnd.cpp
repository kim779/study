// MainWnd.cpp : implementation file
//

#include "stdafx.h"
#include "CX_EDGE.h"
#include "MainWnd.h"
#include "axisfire.h"
#include <string>


__declspec(dllexport) CWnd* WINAPI axCreate(CWnd* parent, void* pParam)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CMainWnd* pControlWnd = new CMainWnd();
	pControlWnd->m_pParent = parent;

	pControlWnd->SetParam((struct _param*)pParam);
	pControlWnd->Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, pControlWnd->m_Param.rect, parent, 100);

	return pControlWnd;
}


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainWnd

CMainWnd::CMainWnd()
{
	EnableAutomation();
}

CMainWnd::~CMainWnd()
{
}

void CMainWnd::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CWnd::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CMainWnd, CWnd)
	//{{AFX_MSG_MAP(CMainWnd)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CMainWnd, CWnd)
	//{{AFX_DISPATCH_MAP(CMainWnd)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
	DISP_FUNCTION_ID(CMainWnd, "Navigate", dispidNavigate, Navigate, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION_ID(CMainWnd, "Navigatesrc", dispidNavigatesrc, Navigatesrc, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION_ID(CMainWnd, "closeEdge", dispidcloseEdge, closeEdge, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()

// Note: we add support for IID_IMainWnd to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {5593AE2B-B6A6-41FF-AFCA-55DECFE9B24D}
static const IID IID_IMainWnd =
{ 0x5593ae2b, 0xb6a6, 0x41ff, { 0xaf, 0xca, 0x55, 0xde, 0xcf, 0xe9, 0xb2, 0x4d } };

BEGIN_INTERFACE_MAP(CMainWnd, CWnd)
	INTERFACE_PART(CMainWnd, IID_IMainWnd, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainWnd message handlers
void CMainWnd::InitEdge()
{
	// TODO: 여기에 구현 코드 추가.
	// <-- WebView2 sample code starts here -->
	// Step 3 - Create a single WebView within the parent window
	// Locate the browser and set up the environment for WebView
	CreateCoreWebView2EnvironmentWithOptions(nullptr, nullptr, nullptr,
		Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
			[&](HRESULT result, ICoreWebView2Environment* env) -> HRESULT {
				// Create a CoreWebView2Controller and get the associated CoreWebView2 whose parent is the main window hWnd
				env->CreateCoreWebView2Controller(GetSafeHwnd(), Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
					[&](HRESULT result, ICoreWebView2Controller* controller) -> HRESULT {
						if (controller != nullptr) {
							_edgeControl = controller;
							_edgeControl->get_CoreWebView2(&_edgeWnd);
						}

						// Add a few settings for the webview
						// The demo step is redundant since the values are the default settings
						ICoreWebView2Settings* Settings;
						_edgeWnd->get_Settings(&Settings);
						Settings->put_IsScriptEnabled(TRUE);
						Settings->put_AreDefaultScriptDialogsEnabled(TRUE);
						Settings->put_IsWebMessageEnabled(TRUE);

						// Resize WebView to fit the bounds of the parent window
						RECT bounds;
						GetClientRect(&bounds);
						_edgeControl->put_Bounds(bounds);

						// Schedule an async task to navigate to Bing
						_edgeWnd->Navigate(_sURL.AllocSysString());
						//_edgeWnd->Navigate(L"http://172.17.9.151:999/git/Home/LogOn?ReturnUrl=%2Fgit%2F");

						// Step 4 - Navigation events
						// register an ICoreWebView2NavigationStartingEventHandler to cancel any non-https navigation
						EventRegistrationToken token;
						_edgeWnd->add_NavigationStarting(Microsoft::WRL::Callback<ICoreWebView2NavigationStartingEventHandler>(
							[](ICoreWebView2* webview, ICoreWebView2NavigationStartingEventArgs* args) -> HRESULT {
								PWSTR uri;
								args->get_Uri(&uri);
								std::wstring source(uri);
								//if (source.substr(0, 5) != L"https") {
								//	args->put_Cancel(true);
								//}
								CoTaskMemFree(uri);
								return S_OK;
							}).Get(), &token);

						// Step 5 - Scripting
						// Schedule an async task to add initialization script that freezes the Object object
						_edgeWnd->AddScriptToExecuteOnDocumentCreated(L"Object.freeze(Object);", nullptr);
						// Schedule an async task to get the document URL
						_edgeWnd->ExecuteScript(L"window.document.URL;", Microsoft::WRL::Callback<ICoreWebView2ExecuteScriptCompletedHandler>(
							[](HRESULT errorCode, LPCWSTR resultObjectAsJson) -> HRESULT {
								LPCWSTR URL = resultObjectAsJson;
								//doSomethingWithURL(URL);
								return S_OK;
							}).Get());

						// Step 6 - Communication between host and web content
						// Set an event handler for the host to return received message back to the web content
						//webviewWindow->add_WebMessageReceived(Callback<ICoreWebView2WebMessageReceivedEventHandler>(
						//	[](ICoreWebView2* webview, ICoreWebView2WebMessageReceivedEventArgs* args) -> HRESULT {
						//		PWSTR message;
						//		args->TryGetWebMessageAsString(&message);
						//		// processMessage(&message);
						//		webview->PostWebMessageAsString(message);
						//		CoTaskMemFree(message);
						//		return S_OK;
						//	}).Get(), &token);

						// Schedule an async task to add initialization script that
						// 1) Add an listener to print message from the host
						// 2) Post document URL to the host
						_edgeWnd->AddScriptToExecuteOnDocumentCreated(
							L"window.chrome.webview.addEventListener(\'message\', event => alert(event.data));" \
							L"window.chrome.webview.postMessage(window.document.URL);",
							nullptr);

						return S_OK;
					}).Get());
				return S_OK;
			}).Get());
	// <-- WebView2 sample code ends here -->
}

void CMainWnd::Navigate(BSTR url)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 여기에 디스패치 처리기 코드를 추가합니다.
	if (_edgeWnd)
		_edgeWnd->Navigate(url);
}


void CMainWnd::Navigatesrc(BSTR src)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 여기에 디스패치 처리기 코드를 추가합니다.
	if (_edgeWnd)
		_edgeWnd->NavigateToString(src);
}


void CMainWnd::closeEdge()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 여기에 디스패치 처리기 코드를 추가합니다.
	if (_edgeControl)
	{
		_edgeControl->Close();
		_edgeControl->Release();
	}
	if (_edgeWnd)
		_edgeWnd->Release();
}

void CMainWnd::SetParam(_param* pParam)
{
	m_Param.key = pParam->key;
	m_Param.name = CString(pParam->name, pParam->name.GetLength());
	m_Param.rect = CRect(pParam->rect.left, pParam->rect.top, pParam->rect.right, pParam->rect.bottom);
	m_Param.fonts = CString(pParam->fonts, pParam->fonts.GetLength());
	m_Param.point = pParam->point;
	m_Param.style = pParam->style;
	m_Param.tRGB = pParam->tRGB;
	m_Param.pRGB = pParam->pRGB;
	m_Param.options = CString(pParam->options, pParam->options.GetLength());

	CString	sToken;
	sToken = _T("/wurl");
	int nPos = m_Param.options.Find(sToken, 0);
	if (nPos >= 0)
	{
		CString sTmp = m_Param.options.Mid(nPos + sToken.GetLength());
		nPos = sTmp.Find("/");
		if (nPos < 0)
			_sURL = sTmp;
		else
			_sURL = sTmp.Left(nPos);
	}

	sToken = _T("/s");
	nPos = m_Param.options.Find(sToken, 0);
	if (nPos >= 0)
	{
		_bScroll = true;
	}
	else
		_bScroll = false;

}

int CMainWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	InitEdge();
	return 0;
}


void CMainWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	RECT bounds;
	GetClientRect(&bounds);
	if (_edgeControl)
		_edgeControl->put_Bounds(bounds);
		
}
