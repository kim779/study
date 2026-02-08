// CMainWnd.cpp: 구현 파일
//

#include "pch.h"
//#include "cx_edge.h"
#include "framework.h"
#include "WINNERS.h"
#include "CMainWnd.h"

#include "ViewComponent.h"
#include <ShObjIdl_core.h>
#include <Shellapi.h>
#include <ShlObj_core.h>
// CMainWnd

IMPLEMENT_DYNAMIC(CMainWnd, CWnd)

static constexpr UINT s_runAsyncWindowMessage = WM_APP;


BEGIN_DISPATCH_MAP(CMainWnd, CWnd)
    //{{AFX_DISPATCH_MAP(CControlWnd)
    DISP_PROPERTY_NOTIFY(CMainWnd, "testprt", m_testprt, OntestprtChanged, VT_BSTR)
    DISP_FUNCTION(CMainWnd, "testmethod", testmethod, VT_BSTR, VTS_BSTR)
    //}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// {DE2D0B6D-4B0C-475D-A264-BA2B017E3ABA}
static const GUID IID_IControlWnd =
{ 0xde2d0b6d, 0x4b0c, 0x475d, { 0xa2, 0x64, 0xba, 0x2b, 0x1, 0x7e, 0x3a, 0xba } };

BEGIN_INTERFACE_MAP(CMainWnd, CWnd)
    INTERFACE_PART(CMainWnd, IID_IControlWnd, Dispatch)
END_INTERFACE_MAP()

CMainWnd::CMainWnd()
{
    g_hInstance = GetModuleHandle(NULL);
}

CMainWnd::~CMainWnd()
{
}


BEGIN_MESSAGE_MAP(CMainWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
    ON_WM_SIZE()
END_MESSAGE_MAP()



// CMainWnd 메시지 처리기

void CMainWnd::OntestprtChanged()
{
  
}

void  CMainWnd::testmethod()
{

}

void CMainWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CWnd::OnPaint()을(를) 호출하지 마십시오.
	CRect rec;
	GetClientRect(rec);
	dc.FillSolidRect(rec, RGB(255, 255, 255));
}


int CMainWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
    InitializeWebView();
	return 0;
}

void CMainWnd::InitializeWebView()
{
	
	CloseWebView();
    m_dcompDevice = nullptr;


    HRESULT hr2 = DCompositionCreateDevice2(nullptr, IID_PPV_ARGS(&m_dcompDevice));
    if (!SUCCEEDED(hr2))
    {
        AfxMessageBox(L"Attempting to create WebView using DComp Visual is not supported.\r\n"
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
	
	HRESULT hr = CreateCoreWebView2EnvironmentWithOptions(subFolder, nullptr, options.Get(), Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(this, &CMainWnd::OnCreateEnvironmentCompleted).Get());

    if (!SUCCEEDED(hr))
    {
        if (hr == HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND))
        {
            TRACE("Couldn't find Edge installation. Do you have a version installed that is compatible with this ");
        }
        else
        {
            AfxMessageBox(_T("Failed to create webview environment"));
        }
    }
}

HRESULT CMainWnd::OnCreateEnvironmentCompleted(HRESULT result, ICoreWebView2Environment* environment)
{
	m_webViewEnvironment = environment;
	m_webViewEnvironment->CreateCoreWebView2Controller(this->GetSafeHwnd(), Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(this, &CMainWnd::OnCreateCoreWebView2ControllerCompleted).Get());

	return S_OK;
}

HRESULT CMainWnd::DCompositionCreateDevice2(IUnknown* renderingDevice, REFIID riid, void** ppv)
{
    HRESULT hr = E_FAIL;
    static decltype(::DCompositionCreateDevice2)* fnCreateDCompDevice2 = nullptr;
    if (fnCreateDCompDevice2 == nullptr)
    {
        HMODULE hmod = ::LoadLibraryEx(L"dcomp.dll", nullptr, 0);
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

HRESULT CMainWnd::OnCreateCoreWebView2ControllerCompleted(HRESULT result, ICoreWebView2Controller* controller)
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
            
        m_strurl = _T("naver.com");
        HRESULT hresult = m_webView->Navigate(m_strurl);
        
        if (hresult == S_OK)
        {
            TRACE("Web Page Opened Successfully");
            ResizeEverything();
        }
       
    }
    else
    {
        TRACE("Failed to create webview");
    }
    return S_OK;
}

void CMainWnd::CloseWebView(bool cleanupUserDataFolder)
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

void CMainWnd::ResizeEverything()
{
    RECT availableBounds = { 0 };
    GetClientRect(&availableBounds);

    if (auto view = GetComponent<ViewComponent>())
    {
        view->SetBounds(availableBounds);
    }
}

void CMainWnd::RunAsync(std::function<void()> callback)
{
    auto* task = new std::function<void()>(callback);
    PostMessage(s_runAsyncWindowMessage, reinterpret_cast<WPARAM>(task), 0);
}

void CMainWnd::OnSize(UINT nType, int cx, int cy)
{
    ResizeEverything();

    // TODO: 여기에 메시지 처리기 코드를 추가합니다.
}
