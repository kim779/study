#include "stdafx.h"
#include <wrl.h>
#include "wil/com.h"

using namespace Microsoft::WRL;

#include "browserX.h"
#include "WebView2.h"

#include <sstream>
#include <iomanip>

#include <shlwapi.h>
#pragma comment(lib,"shlwapi.lib")
#include "shlobj.h"

#pragma comment(lib,"Version.lib")

#include <string>

static constexpr UINT MSG_RUN_ASYNC_CALLBACK = WM_APP + 124;

#define CHECK_FAILURE_STRINGIFY(arg)         #arg
#define CHECK_FAILURE_FILE_LINE(file, line)  ([](HRESULT hr){ CheckFailure(hr, L"Failure at " CHECK_FAILURE_STRINGIFY(file) L"(" CHECK_FAILURE_STRINGIFY(line) L")"); })
#define CHECK_FAILURE                        CHECK_FAILURE_FILE_LINE(__FILE__, __LINE__)
#define CHECK_FAILURE_BOOL(value)            CHECK_FAILURE((value) ? S_OK : E_UNEXPECTED)

struct CbrowserXImpl
{
        wil::com_ptr<ICoreWebView2Environment>    m_webViewEnvironment;
        wil::com_ptr<ICoreWebView2Environment2>   m_webViewEnvironment2;
        wil::com_ptr<ICoreWebView2>               m_webView;
        wil::com_ptr<ICoreWebView2_2>             m_webView2;
        wil::com_ptr<ICoreWebView2Controller>     m_webController;
        wil::com_ptr<ICoreWebView2Settings>       m_webSettings;
};

void ShowFailure(HRESULT hr, CString const& message)
{
        CString text;
        text.Format(_T("%s (0x%08X)"), (LPCTSTR)message, hr);

        ::MessageBox(nullptr, static_cast<LPCTSTR>(text), _T("Failure"), MB_OK);
}

void CheckFailure(HRESULT hr, CString const& message)
{
        if (FAILED(hr))
        {
                CString text;
                text.Format(_T("%s : 0x%08X"), (LPCTSTR)message, hr);
                        // TODO: log text
                std::exit(hr);
        }
}

/////////////////////////////////////////////////////////////////////////////
// CbrowserX

IMPLEMENT_DYNCREATE(CbrowserX, CWnd)

/////////////////////////////////////////////////////////////////////////////
// CbrowserX properties
BEGIN_MESSAGE_MAP(CbrowserX, CWnd)
        ON_WM_DESTROY()
END_MESSAGE_MAP()

CbrowserX::CbrowserX() : m_pImpl(new CbrowserXImpl())
{
        m_callbacks[CallbackType::CreationCompleted] = nullptr;
        m_callbacks[CallbackType::NavigationCompleted] = nullptr;
}

CbrowserX::~CbrowserX()
{
        SetWindowLongPtr(m_hWnd, GWLP_USERDATA, 0);
        CloseWebView();
        delete m_pImpl;
}

void CbrowserX::OnDestroy()
{
        CWnd::OnDestroy();
        delete this;
}

BOOL CbrowserX::CreateHostWindow(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
        if (lpszClassName == nullptr)
                lpszClassName = GetWindowClass();

        if (!CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID))
                return FALSE;

        ::SetWindowLongPtr(m_hWnd, GWLP_USERDATA, (LONG_PTR)this);

        return TRUE;
}

BOOL CbrowserX::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext*)
{
        if (!CreateHostWindow(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID))
                return FALSE;

        if (!SUCCEEDED(InitializeWebView()))
                return FALSE;

        return TRUE;
}

BOOL CbrowserX::CreateAsync(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CallbackFunc onCreated)
{
        if (!CreateHostWindow(nullptr, nullptr, dwStyle, rect, pParentWnd, nID))
                return FALSE;

        m_callbacks[CallbackType::CreationCompleted] = onCreated;

        if (!SUCCEEDED(InitializeWebView()))
                return FALSE;
        
        return TRUE;
}

void CbrowserX::RegisterCallback(CallbackType const type, CallbackFunc callback)
{
        m_callbacks[type] = callback;
}

void CbrowserX::CloseWebView()
{
        if (m_pImpl->m_webView)
        {
                m_pImpl->m_webView->remove_NavigationCompleted(m_navigationCompletedToken);
                m_pImpl->m_webView->remove_NavigationStarting(m_navigationStartingToken);
                m_pImpl->m_webView->remove_DocumentTitleChanged(m_documentTitleChangedToken);

                m_pImpl->m_webController->Close();

                m_pImpl->m_webController = nullptr;
                m_pImpl->m_webView = nullptr;
                m_pImpl->m_webView2 = nullptr;
                m_pImpl->m_webSettings = nullptr;
        }

        m_pImpl->m_webViewEnvironment2 = nullptr;
        m_pImpl->m_webViewEnvironment = nullptr;
}

HRESULT CbrowserX::InitializeWebView()
{
        CloseWebView();

        CString subFolder = GetInstallPath();
        CString appData = GetUserDataFolder();
        ICoreWebView2EnvironmentOptions* options = nullptr;

        HRESULT hr = CreateCoreWebView2EnvironmentWithOptions(
                CStringW(subFolder),
                nullptr,
                options,
                Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>
                (this, &CbrowserX::OnCreateEnvironmentCompleted).Get());

        if (false && !SUCCEEDED(hr))
        {
                CString text;
                if (hr == HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND))
                        text = _T("Cannot found the WebView2 component.");
                else
                        text = _T("Cannot create the webview environment.");

 //             ShowFailure(hr, text);
        }
        return hr;
}

HRESULT CbrowserX::OnCreateEnvironmentCompleted(HRESULT result, ICoreWebView2Environment* environment)
{
        CHECK_FAILURE(result);

        if (!environment)
                return E_FAIL;

        CHECK_FAILURE(environment->QueryInterface(IID_PPV_ARGS(&m_pImpl->m_webViewEnvironment)));
        CHECK_FAILURE(environment->QueryInterface(IID_PPV_ARGS(&m_pImpl->m_webViewEnvironment2)));

        CHECK_FAILURE(m_pImpl->m_webViewEnvironment->CreateCoreWebView2Controller(
                m_hWnd,
                Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>
                (this, &CbrowserX::OnCreateWebViewControllerCompleted).Get()));

        return S_OK;
}

HRESULT CbrowserX::OnCreateWebViewControllerCompleted(HRESULT result, ICoreWebView2Controller* controller)
{
        if (result == S_OK)
        {
                if (controller != nullptr)
                {
                        m_pImpl->m_webController = controller;
                        CHECK_FAILURE(controller->get_CoreWebView2(&m_pImpl->m_webView));

                        if (!m_pImpl->m_webView)
                                return E_FAIL;

                        CHECK_FAILURE(m_pImpl->m_webView->QueryInterface(IID_PPV_ARGS(&m_pImpl->m_webView2)));
                        CHECK_FAILURE(m_pImpl->m_webView->get_Settings(&m_pImpl->m_webSettings));

                        RegisterEventHandlers();
                        ResizeToClientArea();
                }

                auto callback = m_callbacks[CallbackType::CreationCompleted];
                if (callback != nullptr)
                        RunAsync(callback);
        }
        else
        {
//              ShowFailure(result, _T("Cannot create webview environment."));
        }

        return S_OK;
}

void CbrowserX::RegisterEventHandlers()
{
        // NavigationCompleted handler
        CHECK_FAILURE(m_pImpl->m_webView->add_NavigationCompleted(
                Callback<ICoreWebView2NavigationCompletedEventHandler>(
                        [this](ICoreWebView2*, ICoreWebView2NavigationCompletedEventArgs* args) -> HRESULT
                        {
                                m_isNavigating = false;

                                BOOL success;
                                CHECK_FAILURE(args->get_IsSuccess(&success));

                                if (!success)
                                {
                                        COREWEBVIEW2_WEB_ERROR_STATUS webErrorStatus{};
                                        CHECK_FAILURE(args->get_WebErrorStatus(&webErrorStatus));

                                        if (webErrorStatus == COREWEBVIEW2_WEB_ERROR_STATUS_DISCONNECTED)
                                        {
                                                // Do something here if you want to handle a specific error case.
                                                // In most cases this isn't necessary, because the WebView will
                                                // display its own error page automatically.
                                        }
                                }

                                wil::unique_cotaskmem_string uri;
                                m_pImpl->m_webView->get_Source(&uri);

                                if (strcmp(LPCTSTR(uri.get()), _T("about:blank")) == 0)
                                {
                                        uri = wil::make_cotaskmem_string(L"");
                                }

                                auto callback = m_callbacks[CallbackType::NavigationCompleted];
                                if (callback != nullptr)
                                        RunAsync(callback);

                                return S_OK;
                        }).Get(), &m_navigationCompletedToken));

        // NavigationStarting handler
        CHECK_FAILURE(m_pImpl->m_webView->add_NavigationStarting(
                Callback<ICoreWebView2NavigationStartingEventHandler>(
                        [this](ICoreWebView2*, ICoreWebView2NavigationStartingEventArgs* args) -> HRESULT
                        {
                                wil::unique_cotaskmem_string uri;
                                CHECK_FAILURE(args->get_Uri(&uri));

                                m_isNavigating = true;

                                return S_OK;
                        }).Get(), &m_navigationStartingToken));

        // DocumentTitleChanged handler
        CHECK_FAILURE(m_pImpl->m_webView->add_DocumentTitleChanged(
                Callback<ICoreWebView2DocumentTitleChangedEventHandler>(
                        [this](ICoreWebView2* sender, IUnknown* args) -> HRESULT
                        {
                                wil::unique_cotaskmem_string title;
                                CHECK_FAILURE(sender->get_DocumentTitle(&title));

                                m_strTitle = title.get();

                                auto callback = m_callbacks[CallbackType::TitleChanged];
                                if (callback != nullptr)
                                        RunAsync(callback);

                                return S_OK;
                        }).Get(), &m_documentTitleChangedToken));
}

void CbrowserX::ResizeToClientArea()
{
        if (m_pImpl->m_webController)
        {
                RECT bounds;
                GetClientRect(&bounds);
                m_pImpl->m_webController->put_Bounds(bounds);
        }
}

RECT CbrowserX::GetBounds()
{
        RECT rc{ 0,0,0,0 };

        if (m_pImpl->m_webController)
        {
                m_pImpl->m_webController->get_Bounds(&rc);
        }
        return rc;
}

void CbrowserX::Resize(LONG const width, LONG const height)
{
        SetWindowPos(nullptr, 0, 0, width, height, SWP_NOMOVE | SWP_NOREPOSITION);
}

CString CbrowserX::GetLocationURL()
{
        CString url = _T("");
        if (m_pImpl->m_webView)
        {
                wil::unique_cotaskmem_string uri;
                m_pImpl->m_webView->get_Source(&uri);

                if (strcmp(LPCTSTR(uri.get()), _T("about:blank")) == 0)
                {
                        uri = wil::make_cotaskmem_string(L"");
                }

                url = uri.get();
        }

        return url;
}

CString CbrowserX::NormalizeUrl(CString url)
{
        if (url.Find(_T("://")) < 0)
        {
                if (url.GetLength() > 1 && url[1] == ':')
                        url = _T("file://") + url;
                else
                        url = _T("http://") + url;
        }

        return url;
}

void CbrowserX::NavigateTo(CString url)
{
        CString normalizedUrl{ NormalizeUrl(url) };

        m_pImpl->m_webView->Navigate((CStringW)normalizedUrl);
}

void CbrowserX::xNavigate(CString const& url, CallbackFunc onComplete)
{
        if (m_pImpl->m_webView)
        {
                m_callbacks[CallbackType::NavigationCompleted] = onComplete;
                NavigateTo(url);
        }
}

// The raw request header string delimited by CRLF(optional in last header).
void CbrowserX::xNavigatePost(CString const& url, CString const& content, CString const& headers, std::function<void()> onComplete)
{
        if (!m_pImpl->m_webView) return;

        CString normalizedUrl{ NormalizeUrl(url) };

        m_callbacks[CallbackType::NavigationCompleted] = onComplete;

        wil::com_ptr<ICoreWebView2WebResourceRequest> webResourceRequest;
        wil::com_ptr<IStream> postDataStream = SHCreateMemStream(reinterpret_cast<const BYTE*>(static_cast<LPCTSTR>(content)), content.GetLength() + 1);

        CHECK_FAILURE(m_pImpl->m_webViewEnvironment2->CreateWebResourceRequest(
                CT2W(normalizedUrl),
                LPCWSTR(L"POST"),
                postDataStream.get(),
                CT2W(headers),
                &webResourceRequest));

        CHECK_FAILURE(m_pImpl->m_webView2->NavigateWithWebResourceRequest(webResourceRequest.get()));
}

void CbrowserX::PrintDocument()
{
        if (m_pImpl->m_webView)
        {
                m_pImpl->m_webView->ExecuteScript(LPCWSTR(L"window.print();"), nullptr);
        }
}

void CbrowserX::Stop()
{
        if (m_pImpl->m_webView)
        {
                m_pImpl->m_webView->Stop();
        }
}

void CbrowserX::Reload()
{
        if (m_pImpl->m_webView)
        {
                m_pImpl->m_webView->Reload();
        }
}

void CbrowserX::GoBack()
{
        if (m_pImpl->m_webView)
        {
                BOOL possible = FALSE;
                m_pImpl->m_webView->get_CanGoBack(&possible);
                if (possible) m_pImpl->m_webView->GoBack();
        }
}

void CbrowserX::GoForward()
{
        if (m_pImpl->m_webView)
        {
                BOOL possible = FALSE;
                m_pImpl->m_webView->get_CanGoForward(&possible);
                if (possible) m_pImpl->m_webView->GoForward();
        }
}

void CbrowserX::DisablePopups()
{
        if (m_pImpl->m_webSettings)
        {
                m_pImpl->m_webSettings->put_AreDefaultScriptDialogsEnabled(FALSE);
        }
}

PCTSTR CbrowserX::GetWindowClass()
{
        static PCTSTR windowClass = []
        {
                static TCHAR const* className = _T("EdgeBrowserHost");

                WNDCLASSEX wcex;
                wcex.cbSize = sizeof(WNDCLASSEX);

                wcex.style = CS_HREDRAW | CS_VREDRAW;
                wcex.lpfnWndProc = WndProcStatic;
                wcex.cbClsExtra = 0;
                wcex.cbWndExtra = 0;
                wcex.hInstance = AfxGetInstanceHandle();
                wcex.hIcon = nullptr;
                wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
                wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
                wcex.lpszMenuName = nullptr;
                wcex.lpszClassName = className;
                wcex.hIconSm = nullptr;

                ATOM result = RegisterClassEx(&wcex);
                if (result == 0)
                {
                        DWORD lastError = ::GetLastError();
                }

                return className;
        }();

        return windowClass;
}

LRESULT CALLBACK CbrowserX::WndProcStatic(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
        if (auto app = (CbrowserX*)::GetWindowLongPtr(hWnd, GWLP_USERDATA))
        {
                LRESULT result = 0;
                if (app->HandleWindowMessage(hWnd, message, wParam, lParam, &result))
                {
                        return result;
                }
        }

        return ::DefWindowProc(hWnd, message, wParam, lParam);
}

bool CbrowserX::HandleWindowMessage(HWND, UINT message, WPARAM wParam, LPARAM lParam, LRESULT* result)
{
        *result = 0;

        switch (message)
        {
        case WM_SIZE:
        {
                if (lParam != 0)
                {
                        ResizeToClientArea();
                        return true;
                }
        }
        break;
        case MSG_RUN_ASYNC_CALLBACK:
        {
                auto* task = reinterpret_cast<CallbackFunc*>(wParam);
                (*task)();
                delete task;
                return true;
        }
        break;
        }

        return false;
}

void CbrowserX::RunAsync(CallbackFunc callback)
{
        auto* task = new CallbackFunc(callback);
        PostMessage(MSG_RUN_ASYNC_CALLBACK, reinterpret_cast<WPARAM>(task), 0);
}

bool CbrowserX::IsWebViewCreated() const
{
        return m_pImpl->m_webView != nullptr;
}

CString CbrowserX::GetInstallPath()
{
        static CString path = []
        {
                auto installPath = GetInstallPathFromRegistry();        // check registry for WebView2
                if (installPath.IsEmpty())
                        installPath = GetInstallPathFromDisk();         // check disk for WebView2
                if (installPath.IsEmpty())
                        installPath = GetInstallPathFromRegistry(false);// check registry for Edge
                if (installPath.IsEmpty())
                        installPath = GetInstallPathFromDisk(false);    // check disk for Edge

                return installPath;
        }();

        return path;
}

CString CbrowserX::GetInstallPathFromRegistry(bool const searchWebView)
{
        CString path, subkey;
        HKEY handle = nullptr;
        LSTATUS result = ERROR_FILE_NOT_FOUND;

        if (searchWebView)
        {
                subkey = LR"(SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\Microsoft EdgeWebView)";
                result = RegOpenKeyEx(HKEY_LOCAL_MACHINE, subkey, 0, KEY_READ, &handle);
                if (result != ERROR_SUCCESS)
                {
                        subkey = LR"(SOFTWARE\WOW6432Node\Microsoft\Windows\CurrentVersion\Uninstall\Microsoft EdgeWebView)";
                        result = RegOpenKeyEx(HKEY_LOCAL_MACHINE, subkey, 0, KEY_READ, &handle);
                }
        }
        else
        {
                subkey = LR"(SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\Microsoft Edge)";
                result = RegOpenKeyEx(HKEY_LOCAL_MACHINE, subkey, 0, KEY_READ, &handle);
                if (result != ERROR_SUCCESS)
                {
                        subkey = LR"(SOFTWARE\WOW6432Node\Microsoft\Windows\CurrentVersion\Uninstall\Microsoft Edge)";
                        result = RegOpenKeyEx(HKEY_LOCAL_MACHINE, subkey, 0, KEY_READ, &handle);
                }
        }

        if (result == ERROR_SUCCESS)
        {
                TCHAR buffer[MAX_PATH + 1]{ 0 };
                DWORD type = REG_SZ;
                DWORD size = MAX_PATH;
                result = RegQueryValueEx(handle, "InstallLocation", 0, &type, reinterpret_cast<LPBYTE>(buffer), &size);
                if (result == ERROR_SUCCESS)
                        path += CString{ buffer };

                TCHAR version[100]{ 0 };
                size = 100;
                result = RegQueryValueEx(handle, "Version", 0, &type, reinterpret_cast<LPBYTE>(version), &size);
                if (result == ERROR_SUCCESS)
                {
                        if (path.GetAt(path.GetLength() - 1) != '\\')
                                path += "\\";
                        path += CString{ version };
                }
                else
                {
                        path.Empty();
                }

                RegCloseKey(handle);
        }

        return path;
}

CString CbrowserX::GetInstallPathFromDisk(bool const searchWebView)
{
        CString path =
                searchWebView ? LR"(c:\Program Files (x86)\Microsoft\EdgeWebView\Application\)" : LR"(c:\Program Files (x86)\Microsoft\Edge\Application\)";
        CString pattern = path + L"*";

        WIN32_FIND_DATA ffd{ 0 };
        HANDLE hFind = FindFirstFile(pattern, &ffd);
        if (hFind == INVALID_HANDLE_VALUE)
        {
                DWORD error = ::GetLastError();
                return {};
        }

        do
        {
                if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                {
                        CString name{ ffd.cFileName };
                        int a, b, c, d;
                        if (4 == scanf_s(ffd.cFileName, L"%d.%d.%d.%d", &a, &b, &c, &d))
                        {
                                FindClose(hFind);
                                return path + name;
                        }
                }
        } while (FindNextFile(hFind, &ffd) != 0);

        FindClose(hFind);

        return {};
}

CString CbrowserX::GetUserDataFolder()
{
        CString dataPath;

        TCHAR szPath[MAX_PATH]{ 0 };
        ::SHGetFolderPath(nullptr, CSIDL_LOCAL_APPDATA, nullptr, 0, szPath);
        dataPath.Format("%s\\Edge\\", szPath);

        return dataPath;
}

