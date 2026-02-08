// EdgeWnd.cpp : implementation file
//

#include "stdafx.h"
#include "CX_WebView.h"
#include "EdgeWnd.h"

#include "ViewComponent.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEdgeWnd
#define TM_IB2023 9999

static constexpr UINT s_runAsyncWindowMessage = WM_APP;

CEdgeWnd::CEdgeWnd()
{
	EnableAutomation();
}

CEdgeWnd::~CEdgeWnd()
{
}

void CEdgeWnd::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CWnd::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CEdgeWnd, CWnd)
	//{{AFX_MSG_MAP(CEdgeWnd)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_TIMER()
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CEdgeWnd, CWnd)
	//{{AFX_DISPATCH_MAP(CEdgeWnd)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
	DISP_FUNCTION_ID(CEdgeWnd, "Navigate", dispidNavigate, Navigate, VT_EMPTY, VTS_BSTR)
END_DISPATCH_MAP()

// Note: we add support for IID_IEdgeWnd to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {FA4AE7FB-92A9-4452-9C50-F08EDAB6E985}
static const IID IID_IEdgeWnd =
{ 0xfa4ae7fb, 0x92a9, 0x4452, { 0x9c, 0x50, 0xf0, 0x8e, 0xda, 0xb6, 0xe9, 0x85 } };

BEGIN_INTERFACE_MAP(CEdgeWnd, CWnd)
	INTERFACE_PART(CEdgeWnd, IID_IEdgeWnd, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEdgeWnd message handlers

void CEdgeWnd::SetParam(_param* pParam)
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

void CEdgeWnd::Navigate(BSTR sUrl)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	KillTimer(TM_IB2023);
	m_strurl.Format("%s", sUrl);
	if (m_webView == nullptr)
	{
		SetTimer(TM_IB2023, 1000, nullptr);
		return;
	}

	// TODO: 여기에 디스패치 처리기 코드를 추가합니다.
	wchar_t strUnicode[256] = { 0, };
	char    strMultibyte[256] = { 0, };
	strcpy_s(strMultibyte, 256, m_strurl);
	int nLen = MultiByteToWideChar(CP_ACP, 0, strMultibyte, strlen(strMultibyte), NULL, NULL);
	MultiByteToWideChar(CP_ACP, 0, strMultibyte, strlen(strMultibyte), strUnicode, nLen);

	HRESULT hresult = m_webView->Navigate(strUnicode);

	if (hresult == S_OK)
	{
		OutputDebugString("[cx_edge]Web Page Opened Successfully");
		ResizeEverything();
	}
}

void CEdgeWnd::Navigate_strUrl()
{
	KillTimer(TM_IB2023);
	if (m_webView == nullptr)
	{
		SetTimer(TM_IB2023, 1000, nullptr);
		return;
	}

	// TODO: 여기에 디스패치 처리기 코드를 추가합니다.
	wchar_t strUnicode[256] = { 0, };
	char    strMultibyte[256] = { 0, };
	strcpy_s(strMultibyte, 256, m_strurl);
	int nLen = MultiByteToWideChar(CP_ACP, 0, strMultibyte, strlen(strMultibyte), NULL, NULL);
	MultiByteToWideChar(CP_ACP, 0, strMultibyte, strlen(strMultibyte), strUnicode, nLen);

	HRESULT hresult = m_webView->Navigate(strUnicode);

	if (hresult == S_OK)
	{
		OutputDebugString("[cx_edge]Web Page Opened Successfully");
		ResizeEverything();
	}
	else
	{
		TRACE("Navigate_strUrl navigate fail!!!!!!!!!!!!!!!");
	}
}

int CEdgeWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	InitializeWebView();
	return 0;
}

void CEdgeWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	ResizeEverything();
}

void CEdgeWnd::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (nIDEvent)
	{
	case TM_IB2023:
	{
		KillTimer(nIDEvent);
		Navigate_strUrl();
	}
	break;
	}
	CWnd::OnTimer(nIDEvent);
}

void CEdgeWnd::InitializeWebView()
{
	CloseWebView();
	m_dcompDevice = nullptr;

	HRESULT hr2 = DCompositionCreateDevice2(nullptr, IID_PPV_ARGS(&m_dcompDevice));
	if (!SUCCEEDED(hr2))
	{
		AfxMessageBox("Attempting to create WebView using DComp Visual is not supported.\r\n"
			"DComp device creation failed.\r\n"
			"Current OS may not support DComp.\r\n"
			"Create with Windowless DComp Visual Failed", MB_OK);
		return;
	}

#ifdef USE_WEBVIEW2_WIN10
	m_wincompCompositor = nullptr;
#endif
	LPCWSTR subFolder = nullptr;
	auto options = Microsoft::WRL::Make<CoreWebView2EnvironmentOptions>();
	options->put_AllowSingleSignOnUsingOSPrimaryAccount(FALSE);

	HRESULT hr = CreateCoreWebView2EnvironmentWithOptions(subFolder, nullptr, options.Get(), Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(this, &CEdgeWnd::OnCreateEnvironmentCompleted).Get());
}

HRESULT CEdgeWnd::OnCreateEnvironmentCompleted(HRESULT result, ICoreWebView2Environment* environment)
{
	m_webViewEnvironment = environment;
	m_webViewEnvironment->CreateCoreWebView2Controller(this->GetSafeHwnd(), Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(this, &CEdgeWnd::OnCreateCoreWebView2ControllerCompleted).Get());

	return S_OK;
}

HRESULT CEdgeWnd::DCompositionCreateDevice2(IUnknown* renderingDevice, REFIID riid, void** ppv)
{
	HRESULT hr = E_FAIL;
	static decltype(::DCompositionCreateDevice2)* fnCreateDCompDevice2 = nullptr;
	if (fnCreateDCompDevice2 == nullptr)
	{
		HMODULE hmod = ::LoadLibraryEx("dcomp.dll", nullptr, 0);
		if (hmod != nullptr)
		{
			fnCreateDCompDevice2 = reinterpret_cast<decltype(::DCompositionCreateDevice2)*>(
				::GetProcAddress(hmod, "DCompositionCreateDevice2"));
		}
	}
	if (fnCreateDCompDevice2 != nullptr)
	{
		hr = fnCreateDCompDevice2(renderingDevice, riid, ppv);
	}
	return hr;
}

HRESULT CEdgeWnd::OnCreateCoreWebView2ControllerCompleted(HRESULT result, ICoreWebView2Controller* controller)
{
	if (result == S_OK)
	{
		m_controller = controller;
		wil::com_ptr<ICoreWebView2> coreWebView2;
		m_controller->get_CoreWebView2(&coreWebView2);
		coreWebView2.query_to(&m_webView);

		NewComponent<ViewComponent>(
			this, m_dcompDevice.get(),
#ifdef USE_WEBVIEW2_WIN10
			m_wincompCompositor,
#endif
			m_creationModeId == IDM_CREATION_MODE_TARGET_DCOMP);

		/*	wchar_t strUnicode[256] = { 0, };
			char    strMultibyte[256] = { 0, };
			strcpy_s(strMultibyte, 256, "https://ibks.com");
			int nLen = MultiByteToWideChar(CP_ACP, 0, strMultibyte, strlen(strMultibyte), NULL, NULL);
			MultiByteToWideChar(CP_ACP, 0, strMultibyte, strlen(strMultibyte), strUnicode, nLen);

			HRESULT hresult = m_webView->Navigate(strUnicode);

			if (hresult == S_OK)
			{
				OutputDebugString("[cx_edge]Web Page Opened Successfully");
				ResizeEverything();
			}*/

	}
	else
	{
		TRACE("Failed to create webview");
		AfxMessageBox("Failed to create webview");
	}

	return S_OK;
}

void CEdgeWnd::CloseWebView(bool cleanupUserDataFolder)
{
	if (m_controller)
	{
		m_controller->Close();
		m_controller = nullptr;
		m_webView = nullptr;
	}
	m_webViewEnvironment = nullptr;
	if (cleanupUserDataFolder)
	{
		//Clean user data        
	}
}

void CEdgeWnd::ResizeEverything()
{
	RECT availableBounds = { 0 };
	GetClientRect(&availableBounds);

	if (auto view = GetComponent<ViewComponent>())
	{
		view->SetBounds(availableBounds);
		wil::com_ptr<ICoreWebView2Controller> controller = view->getController();
		BOOL bvisiable;
		controller->get_IsVisible(&bvisiable);
		TRACE("!!!!!!!!!!");
		if (!bvisiable)
			controller->put_IsVisible(TRUE);
	}
}

void CEdgeWnd::RunAsync(std::function<void()> callback)
{
	auto* task = new std::function<void()>(callback);
	PostMessage(s_runAsyncWindowMessage, reinterpret_cast<WPARAM>(task), 0);
}