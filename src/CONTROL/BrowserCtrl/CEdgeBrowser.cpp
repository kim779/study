#include "stdafx.h"
#include "CEdgeBrowser.h"

CEdgeBrowser::CEdgeBrowser(CWnd* pWizard)
{
	m_pWizard = pWizard;
	m_pParent = nullptr;

	m_bCtrlVisible = false;
}


CEdgeBrowser::~CEdgeBrowser()
{
}
BEGIN_MESSAGE_MAP(CEdgeBrowser, CWnd)
//	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

void CEdgeBrowser::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	ResizeEverything();
}

BOOL CEdgeBrowser::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	m_pParent = pParentWnd;

	CWnd* pWnd = this;
	BOOL bResult = pWnd->Create(AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW), nullptr, dwStyle, rect, pParentWnd, nID);
	HRESULT hresult = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

	return TRUE;
}

HRESULT CEdgeBrowser::OnCreateEnvironmentCompleted(HRESULT result, ICoreWebView2Environment* environment)
{
	m_webViewEnvironment = environment;
	m_webViewEnvironment->CreateCoreWebView2Controller(this->GetSafeHwnd(), Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(this, &CEdgeBrowser::OnCreateCoreWebView2ControllerCompleted).Get());

	return S_OK;
}

void CEdgeBrowser::CloseWebView(bool cleanupUserDataFolder)
{
	if (m_controller)
	{
		m_controller->Close();
		m_controller = nullptr;
		m_webView = nullptr;
	}

	if (m_webViewEnvironment)
		m_webViewEnvironment = nullptr;

	if (cleanupUserDataFolder)
	{
		//Clean user data        
	}
}

bool CEdgeBrowser::InitializeWebView()
{
	CloseWebView();
	//m_dcompDevice = nullptr;

	/*HRESULT hr2 = DCompositionCreateDevice2(nullptr, IID_PPV_ARGS(&m_dcompDevice));
	if (!SUCCEEDED(hr2))
	{
		AfxMessageBox("Attempting to create WebView using DComp Visual is not supported.\r\n"
			"DComp device creation failed.\r\n"
			"Current OS may not support DComp.\r\n"
			"Create with Windowless DComp Visual Failed", MB_OK);
		return;
	}*/


#ifdef USE_WEBVIEW2_WIN10
	m_wincompCompositor = nullptr;
#endif
	LPCWSTR subFolder = nullptr;
	auto options = Microsoft::WRL::Make<CoreWebView2EnvironmentOptions>();
	options->put_AllowSingleSignOnUsingOSPrimaryAccount(FALSE);

	HRESULT hr = CreateCoreWebView2EnvironmentWithOptions(subFolder, nullptr, options.Get(), 
		Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>
		(this, &CEdgeBrowser::OnCreateEnvironmentCompleted).Get());

	if (!SUCCEEDED(hr))
	{
		if (hr == HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND))
		{
			TRACE("Couldn't find Edge installation. Do you have a version installed that is compatible with this ");
		}
		else
		{
			AfxMessageBox("Failed to create webview environment");
		}

		return false;
	}

	return true;
}

HRESULT CEdgeBrowser::OnCreateCoreWebView2ControllerCompleted(HRESULT result, ICoreWebView2Controller* controller)
{
	if (result == S_OK)
	{
		m_controller = controller;
		Microsoft::WRL::ComPtr<ICoreWebView2> coreWebView2;
		m_controller->get_CoreWebView2(&coreWebView2);

		if(m_bParentWnd)
			m_controller->put_ParentWindow(m_pWizard->GetSafeHwnd());

		m_webView = coreWebView2.Get();

		m_controller->add_AcceleratorKeyPressed(
			Microsoft::WRL::Callback<ICoreWebView2AcceleratorKeyPressedEventHandler>
			(this, &CEdgeBrowser::OnAcceleratorKeyPress).Get(), NULL);

		if (!m_webView)
			return S_FALSE;

		m_webView->add_NavigationStarting(
			Microsoft::WRL::Callback<ICoreWebView2NavigationStartingEventHandler>(
				this, &CEdgeBrowser::OnNavigationStarting).Get(), NULL);

		ResizeEverything();

		if (!m_bFirst)
		{ 
			if (m_webView)
			{
				m_bFirst = true;
				m_webView->Navigate(m_sUrl.AllocSysString());
			}
		}

		if(m_bCtrlVisible)
			m_controller->put_IsVisible(true);
	}
	else
	{
		TRACE("Failed to create webview");
	}
	return S_OK;
}

void CEdgeBrowser::ResizeEverything()
{
	if (!m_pWizard)
		return;

	RECT availableBounds = { 0 };
	
	CRect rcWizard, rcBrowser;
	m_pWizard->GetWindowRect(&rcWizard);

	GetClientRect(&availableBounds);
	rcBrowser = availableBounds;
	ClientToScreen(&rcBrowser);

	if (m_bParentWnd)
	{
		availableBounds.left = abs(rcWizard.left - rcBrowser.left);
		availableBounds.top = abs(rcWizard.top - rcBrowser.top);
		availableBounds.right += availableBounds.left;
		availableBounds.bottom += availableBounds.top;
	}
	
	SIZE webViewSize = {LONG((availableBounds.right - availableBounds.left) * 1 * 1), LONG((availableBounds.bottom - availableBounds.top) * 1 * 1)};

	RECT desiredBounds = availableBounds;

	desiredBounds.bottom = LONG(webViewSize.cy + availableBounds.top);
	desiredBounds.right = LONG(webViewSize.cx + availableBounds.left);

	if (m_controller)
		m_controller->put_Bounds(desiredBounds);
}

void CEdgeBrowser::Navigate(CString sUrl)
{
	if (!m_bFirst)
		m_sUrl = sUrl;
	else
	{
		if (m_webView)
			m_webView->Navigate(sUrl.AllocSysString());
	}
}

HRESULT CEdgeBrowser::OnAcceleratorKeyPress(ICoreWebView2Controller* pSender, ICoreWebView2AcceleratorKeyPressedEventArgs* pEventArgs)
{
	COREWEBVIEW2_KEY_EVENT_KIND eventKind;
	pEventArgs->get_KeyEventKind(&eventKind);
	if (eventKind == COREWEBVIEW2_KEY_EVENT_KIND_KEY_DOWN)
	{
		UINT uKey;
		pEventArgs->get_VirtualKey(&uKey);

		if (uKey == VK_ESCAPE)
			m_pParent->PostMessage(WM_KEYDOWN, (WPARAM)VK_ESCAPE, 0);
	}

	return S_OK;
}

HRESULT CEdgeBrowser::OnNavigationStarting(ICoreWebView2* pICoreWebView2, ICoreWebView2NavigationStartingEventArgs* pEventArgs)
{
	LPWSTR lpwUrl;
	pEventArgs->get_Uri(&lpwUrl);
	CString strData = lpwUrl;

	if (m_pParent)
	{
		LRESULT lResult = m_pParent->SendMessage(WM_USER + 2401, 0, (LPARAM)strData.GetString());
		if (lResult == S_OK)
		{
			pEventArgs->put_Cancel(TRUE);
			return S_OK;
		}
	}
	
	return S_FALSE;
}

void CEdgeBrowser::Print()
{
	if (!m_webView)
		return;

	LPCWSTR lpcwScript(L"window.print();");
	m_webView->ExecuteScript(lpcwScript, NULL);
}

void CEdgeBrowser::SetVisible(bool bVisible)
{
	if(m_controller)
		m_controller->put_IsVisible(bVisible);
	else
		m_bCtrlVisible = bVisible;
}

void CEdgeBrowser::SetParentWnd(bool bParentWnd)
{
	m_bParentWnd = bParentWnd;
}