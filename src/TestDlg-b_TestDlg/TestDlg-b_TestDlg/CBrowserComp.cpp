// CBrowserComp.cpp: 구현 파일
//

#include "pch.h"
#include "TestDlg.h"
#include "CBrowserComp.h"
#include "CheckFailure.h"
#include "CompositionHost.h"
#include <WinUser.h>
#include "AppWindow.h"
// CBrowserComp



IMPLEMENT_DYNAMIC(CBrowserComp, CWnd)

#ifndef DF_USEAPP
static constexpr LPCWSTR s_subFolder = nullptr;
static constexpr char c_samplePath[] = _T("WebView2SamplePage.html");
CBrowserComp::CBrowserComp() : m_winComp(std::make_unique<CompositionHost>())
{
    char szTitle[MAX_LOADSTRING] = "TITLE_test";
	CHECK_FAILURE(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED));
	//LoadStringW(hInst, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);

	m_sampleUri = L"http://naver.com";
	
}
#else

CBrowserComp::CBrowserComp()
{

}
#endif

CBrowserComp::~CBrowserComp()
{
}

BEGIN_MESSAGE_MAP(CBrowserComp, CWnd)
    ON_WM_CREATE()
    ON_WM_SIZE()
END_MESSAGE_MAP()



// CBrowserComp 메시지 처리기
#ifndef DF_USEAPP
void CBrowserComp::InitializeWebView()
{
	auto options = Microsoft::WRL::Make<CoreWebView2EnvironmentOptions>();
    HRESULT hr = CreateCoreWebView2EnvironmentWithOptions(
        s_subFolder, nullptr, options.Get(),
        Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
            [this](HRESULT result, ICoreWebView2Environment* environment) -> HRESULT {
                m_webViewEnvironment = environment;
                wil::com_ptr<ICoreWebView2Environment3>
                    webViewEnvironment3 =
                    m_webViewEnvironment.try_query<ICoreWebView2Environment3>();

                if (webViewEnvironment3)
                {
                    CHECK_FAILURE(
                        webViewEnvironment3->CreateCoreWebView2CompositionController(
                            m_mainWindow,
                            Callback<
                            ICoreWebView2CreateCoreWebView2CompositionControllerCompletedHandler>(
                                this, &CBrowserComp::OnCreateCoreWebView2ControllerCompleted)
                            .Get()));
                }
                return S_OK;
            })
        .Get());
    assert(SUCCEEDED(hr));
}

HRESULT CBrowserComp::OnCreateCoreWebView2ControllerCompleted(
    HRESULT result, ICoreWebView2CompositionController* compositionController)
{
    if (result == S_OK)
    {
        m_compositionController = compositionController;
        CHECK_FAILURE(m_compositionController->QueryInterface(IID_PPV_ARGS(&m_controller)));
        CHECK_FAILURE(m_controller->get_CoreWebView2(&m_webView));

        CRect rect;
        GetClientRect(&rect);
        // SendMessage를 사용하여 WM_SIZE 메시지를 보냄
        SendMessage(WM_SIZE, SIZE_RESTORED, MAKELPARAM(rect.Width(), rect.Height()));

        m_controller->put_IsVisible(true);


        RegisterEventHandlers();

        HRESULT hres = m_webView->Navigate(m_sampleUri.c_str());
        if (hres == S_OK)
        {
            OutputDebugString("\r\n navigate ok");
        }
        else
        {
            OutputDebugString("\r\n navigate fail");
        }
      
    }
    else
    {
        ShowFailure(result, L"Failed to create webview");
    }

    m_winComp->Initialize(this);
    return S_OK;
}
#endif

void CBrowserComp::RegisterEventHandlers()
{
  /* 
     m_webView->add_MouseDown(
        Callback<ICoreWebView2MouseEventArgs*>(
            this, &CBrowserComp::OnMouseDown).Get(), nullptr);
     m_webView->add_MouseUp(
        Callback<ICoreWebView2MouseEventArgs*>(
            this, &CBrowserComp::OnMouseUp).Get(), nullptr);
    m_webView->add_MouseMove(
        Callback<ICoreWebView2MouseEventArgs*>(
            this, &CBrowserComp::OnMouseMove).Get(), nullptr);
    m_webView->add_MouseWheel(
        Callback<ICoreWebView2MouseEventArgs*>(
            this, &CBrowserComp::OnMouseWheel).Get(), nullptr);
    */
}

int CBrowserComp::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

#ifdef DF_USEAPP
    AppWindow appWindow;
#else
    m_mainWindow = this->m_hWnd;
   InitializeWebView();
#endif
    return 0;
}


void CBrowserComp::OnSize(UINT nType, int cx, int cy)
{
    CWnd::OnSize(nType, cx, cy);
#ifndef DF_USEAPP
    if (m_webView)
    {
        RECT availableBounds = { 0 };
        GetClientRect(&availableBounds);
        m_winComp->SetBounds(availableBounds);
    }
#endif
}
