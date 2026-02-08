// CWebDlg.cpp: 구현 파일
//

#include "pch.h"
#include "WINNERS.h"
#include "CWebDlg.h"
#include "afxdialogex.h"
#include "winuser.h"
#include "ViewComponent.h"
//#include "pch.h"
#include <ShObjIdl_core.h>
#include <Shellapi.h>
#include <ShlObj_core.h>

// CWebDlg 대화 상자

IMPLEMENT_DYNAMIC(CWebDlg, CDialog)

static constexpr UINT s_runAsyncWindowMessage = WM_APP;

CWebDlg::CWebDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_WEBDLG, pParent)
{
	g_hInstance = GetModuleHandle(NULL);
}

CWebDlg::~CWebDlg()
{
}

void CWebDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CWebDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_SYSCOMMAND()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CWebDlg 메시지 처리기


BOOL CWebDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetWindowPos(nullptr, 0, 0, m_iwidth, m_iheight, SWP_NOMOVE);
	InitializeWebView();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CWebDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CDialog::OnPaint()을(를) 호출하지 마십시오.
 //   CRect rec;
 //   GetClientRect(rec);
//    dc.FillSolidRect(rec, RGB(255, 0, 0));
}


void CWebDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialog::OnSysCommand(nID, lParam);
}


void CWebDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
    ResizeEverything();
}

void CWebDlg::ResizeEverything()
{
    RECT availableBounds = { 0 };
    GetClientRect(&availableBounds);
    // ClientToScreen(&availableBounds);

    if (auto view = GetComponent<ViewComponent>())
    {
        view->SetBounds(availableBounds);
    }
}

void CWebDlg::RunAsync(std::function<void()> callback)
{
    auto* task = new std::function<void()>(callback);
    PostMessage(s_runAsyncWindowMessage, reinterpret_cast<WPARAM>(task), 0);
}

void CWebDlg::InitializeWebView()
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


    HRESULT hr = CreateCoreWebView2EnvironmentWithOptions(subFolder, nullptr, options.Get(), Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(this, &CWebDlg::OnCreateEnvironmentCompleted).Get());
    
    if (!SUCCEEDED(hr))
    {
        if (hr == HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND))
        {
            TRACE("Couldn't find Edge installation. Do you have a version installed that is compatible with this ");
        }
        else
        {
            AfxMessageBox(L"Failed to create webview environment");
        }
    }
}

HRESULT CWebDlg::DCompositionCreateDevice2(IUnknown* renderingDevice, REFIID riid, void** ppv)
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

void CWebDlg::CloseWebView(bool cleanupUserDataFolder)
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

HRESULT CWebDlg::OnCreateEnvironmentCompleted(HRESULT result, ICoreWebView2Environment* environment)
{
    m_webViewEnvironment = environment;
    m_webViewEnvironment->CreateCoreWebView2Controller(this->GetSafeHwnd(), Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(this, &CWebDlg::OnCreateCoreWebView2ControllerCompleted).Get());

    return S_OK;
}

HRESULT CWebDlg::OnCreateCoreWebView2ControllerCompleted(HRESULT result, ICoreWebView2Controller* controller)
{
    m_slog.Format(_T("[WINNERS]  OnCreateCoreWebView2ControllerCompleted [% d]\n"), result);
    OutputDebugString(m_slog);
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

        HRESULT hresult = m_webView->Navigate(m_strurl);

        if (hresult == S_OK)
        {
            TRACE("Web Page Opened Successfully");
m_slog.Format(_T("[WINNERS]Web Page Opened Successfull[% d]\n"), hresult );
OutputDebugString(m_slog);
            ResizeEverything();
        }

    }
    else
    {
        TRACE("Failed to create webview");
m_slog.Format(_T("[WINNERS]Failed to create webviewl err=[%d]\n"), GetLastError());
OutputDebugString(m_slog);
    }
    return S_OK;
}


BOOL CWebDlg::PreTranslateMessage(MSG* pMsg)
{
    // TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
    if (pMsg->message == WM_KEYDOWN)
    {
        if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
        {
            return TRUE;
        }
    }

    return CDialog::PreTranslateMessage(pMsg);
}
