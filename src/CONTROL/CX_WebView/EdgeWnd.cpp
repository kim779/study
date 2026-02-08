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



#define MMSG_GETPASSWORD		0x01
#define MMSG_GETCERTPASSWORD	0x02

class ProfileStr
{
public:
	CString value;
	ProfileStr(const char* section, const char* item, const char* defaultValue, const char* iniFile)
	{
		char buffer[MAX_PATH];
		GetPrivateProfileString(section, item, defaultValue, buffer, MAX_PATH, iniFile);
		value = buffer;
	}
};

class ProfileInt
{
public:
	int value;
	ProfileInt(const char* section, const char* item, int defaultValue, const char* iniFile)
	{
		CString s;
		s.Format("%d", defaultValue);
		value = atoi(ProfileStr(section, item, s, iniFile).value);
	}
};

struct _web_mid
{
	char	gubn[1];	        // 구분
	char	scno[4];			// 화면번호
};
#define sz_WEBMID		sizeof(struct _web_mid) 

struct	_web_mod {
	char	gmsg[32];		//메세지
	char	grid_o[103];
	char	nrec[4];
	char	scno[4];		// 화면번호
	char	dumy[80];		// Dummy
	char	type[16];		// Type	
	//char	widt[5];		// width	
	//char	heig[5];		// height	
	char	url[600];		// URL	
};

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
	ON_MESSAGE(WM_USER, OnMessage)
	ON_WM_PAINT()
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CEdgeWnd, CWnd)
	//{{AFX_DISPATCH_MAP(CEdgeWnd)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
	DISP_FUNCTION_ID(CEdgeWnd, "Navigate", dispidNavigate, Navigate, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION_ID(CEdgeWnd, "Navigate2", dispidNavigate2, Navigate2, VT_EMPTY, VTS_I2 VTS_BSTR)
	DISP_FUNCTION_ID(CEdgeWnd, "GoBack", dispidGoBack, GoBack, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CEdgeWnd, "GoForward", dispidGoForward, GoForward, VT_EMPTY, VTS_NONE)
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

	m_slog.Format("[cx_WebeEdge] not certificate nLen=[%d] m_strurl=[%s]", nLen, m_strurl.Left(30) );
	OutputDebugString(m_slog);


	if (hresult == S_OK)
	{
		OutputDebugString("[cx_edge]Web Page Opened Successfully");
		ResizeEverything();
	}
}

void CEdgeWnd::Navigate2(SHORT igubn, BSTR sUrl)
{
	/*wchar_t strUnicode[256] = { 0, };
	char    strMultibyte[256] = { 0, };
	strcpy_s(strMultibyte, 256, "about:blank");
	int nLen = MultiByteToWideChar(CP_ACP, 0, strMultibyte, strlen(strMultibyte), NULL, NULL);
	MultiByteToWideChar(CP_ACP, 0, strMultibyte, strlen(strMultibyte), strUnicode, nLen);
	HRESULT hresult = m_webView->Navigate(strUnicode);*/


	m_slog.Format("%d  %s", igubn, sUrl);
	m_menu.Format("%s", sUrl);
	m_menu.TrimRight();
	SearchURL();
}

void CEdgeWnd::Navigate_strUrl()
{
	KillTimer(TM_IB2023);
	if (m_webView == nullptr)
	{
		//SetTimer(TM_IB2023, 1000, nullptr);
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
	m_sMapName = (LPCTSTR)m_pParent->SendMessage(WM_USER, MAKEWPARAM(mapDLL, 1));
	m_baseURL = "http://www.ibks.com/LoadService.jsp";
	m_sRoot.Format("%s", (char*)m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, homeCC), 0));
	m_sRoot.TrimRight();

	CString sname;
	sname.Format("%s", (char*)m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, nameCC), 0));

	int readL = 0;
	char readB[1024];
	CString userD;
	userD.Format("%s\\%s\\%s\\%s.ini", m_sRoot, "user", sname, sname);
	readL = GetPrivateProfileString("MODE", "main", "", readB, sizeof(readB), userD);

	if (readL <= 0)
		return 0;

	CString sWnd;
	sWnd.Format("%s", readB);
	int ddata = atoi(sWnd);
	HWND hWnd = (HWND)ddata;
	m_pMainFrame = CWnd::FromHandle(hWnd);

	//m_pMainFrame = GetTopLevelParent();

	GetLocalIP();	//2013.08.12 KSJ IP구하기
	GetMAC();
	m_userID.Format("%s", (char*)m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, userCC), 0));

	InitializeWebView();
	return 0;
}

void CEdgeWnd::GetMAC()
{
	char	buff[20]{};
	CString file;
	file.Format("%s\\tab\\AXIS.INI", m_sRoot);
	GetPrivateProfileString("PCDATA", "sPhyMac", "", buff, sizeof(buff), file);
	m_MacAddr.Format("%s", buff);
	m_MacAddr.TrimRight();
	m_MacAddr.MakeLower();
}

void CEdgeWnd::GetLocalIP()
{
	char szHostName[64] = { 0 };

	::gethostname(szHostName, sizeof(szHostName));

	if (lstrcmp(szHostName, "") != 0)
	{
		HOSTENT FAR* lphostent = ::gethostbyname(szHostName);

		for (int ii = 0; lphostent; ii++)
		{
			if (!lphostent->h_addr_list[ii])
				break;

			sprintf(szHostName, "%u.%u.%u.%u",
				0xff & lphostent->h_addr_list[ii][0],
				0xff & lphostent->h_addr_list[ii][1],
				0xff & lphostent->h_addr_list[ii][2],
				0xff & lphostent->h_addr_list[ii][3]);
			m_ipAddr = szHostName;
		}
	}
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

	m_webViewEnvironment->add_NewBrowserVersionAvailable(
		Microsoft::WRL::Callback<ICoreWebView2NewBrowserVersionAvailableEventHandler>(
			[this](ICoreWebView2Environment* sender, IUnknown* args) -> HRESULT
			{
				AfxMessageBox("A new version of the browser engine is available. Please update to the latest version.");
				return S_OK;
			}).Get(), nullptr);

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

		//testcode
		wil::com_ptr<ICoreWebView2> webView = m_webView;
		webView->add_NavigationStarting(
			Microsoft::WRL::Callback<ICoreWebView2NavigationStartingEventHandler>(
				[this](ICoreWebView2* sender, ICoreWebView2NavigationStartingEventArgs* args) -> HRESULT {
					m_slog.Format("[EDGE] add_NavigationStarting   ");
					OutputDebugString(m_slog);
					return S_OK;
				}).Get(), nullptr);


		webView->add_NavigationCompleted(
			Microsoft::WRL::Callback<ICoreWebView2NavigationCompletedEventHandler>(
				[this](ICoreWebView2* sender, ICoreWebView2NavigationCompletedEventArgs* args) -> HRESULT {
					m_slog.Format("[EDGE] add_NavigationCompleted   ");
					OutputDebugString(m_slog);
					return S_OK;
				}).Get(), nullptr);




		webView->add_WebMessageReceived(Microsoft::WRL::Callback<ICoreWebView2WebMessageReceivedEventHandler>(
			[this](ICoreWebView2* sender, ICoreWebView2WebMessageReceivedEventArgs* args) -> HRESULT {
				wil::unique_bstr message;
				args->TryGetWebMessageAsString(&message);
				CString strMsg(message.get());
				//AxStd::_Msg("WebMessageReceived: %s", strMsg);
				m_slog.Format("[EDGE] add_WebMessageReceived  strMsg =[%s] ", strMsg);
				OutputDebugString(m_slog);

				// TODO: 응답 처리 로직 추가


				return S_OK;
			}).Get(), nullptr);




		m_webView->add_FrameNavigationCompleted(Microsoft::WRL::Callback<ICoreWebView2NavigationCompletedEventHandler>(
				[this](ICoreWebView2* sender, ICoreWebView2NavigationCompletedEventArgs* args) -> HRESULT
				{
					BOOL success = FALSE;
					if (SUCCEEDED(args->get_IsSuccess(&success)) && success)
					{
						m_slog.Format("[EDGE] FrameNavigationCompleted   ");
						OutputDebugString(m_slog);

						// 웹페이지 로드 완료 후 메시지 전송
						/*LPCWSTR msg = L"{ \"type\": \"notify\", \"data\": \"Hello from native!\" }";
						HRESULT hr = m_webView->PostWebMessageAsString(msg);
						if (FAILED(hr))
						{
							TRACE(_T("PostWebMessageAsString 실패! hr=0x%08X\n"), hr);
						}*/
					}
					else
					{
						COREWEBVIEW2_WEB_ERROR_STATUS status;
						args->get_WebErrorStatus(&status);
						TRACE(_T("FrameNavigation 실패, status=%d\n"), status);
					}
					return S_OK;
				}).Get(),
					nullptr);



		m_webView->add_NavigationStarting(
			Microsoft::WRL::Callback<ICoreWebView2NavigationStartingEventHandler>(
				[this](ICoreWebView2* sender, ICoreWebView2NavigationStartingEventArgs* args) -> HRESULT {
					wil::unique_cotaskmem_string uri;
					args->get_Uri(&uri);

					CString strUrl(uri.get());
			
					m_slog.Format("[EDGE] FrameNavigationCompleted strUrl=[%s]  ", strUrl);
					OutputDebugString(m_slog);

					// 특정 URL 차단 가능
					if (strUrl.Find(_T("blockme.com")) >= 0)
						args->put_Cancel(TRUE);

					return S_OK;
				}).Get(),
					nullptr);

		
		//testcode

		NewComponent<ViewComponent>(
			this, m_dcompDevice.get(),
#ifdef USE_WEBVIEW2_WIN10
			m_wincompCompositor,
#endif
			m_creationModeId == IDM_CREATION_MODE_TARGET_DCOMP);

	}
	else
	{
		TRACE("Failed to create webview");
	/*	char	buf[32]{};
		GetPrivateProfileString("cx_webEdge", "browser", _T(""), buf, sizeof(buf), m_sRoot);
		WritePrivateProfileString("cx_webEdge", "browser", "IE", m_sRoot);*/
		AfxMessageBox("브라우져 실행중 오류 발생 다시 화면을 열어주십시요");
		
		//ShellExecute(NULL, _T("open"), m_strurl, NULL, NULL, SW_SHOW);
		//m_pParent->GetParent()->PostMessage(WM_CLOSE);
	}


	m_pParent->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_Param.key, evOnDblClk)), (LPARAM)m_Param.name.GetString());
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

void CEdgeWnd::GoBack()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_webView->GoBack();
//
}


void CEdgeWnd::GoForward()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 여기에 디스패치 처리기 코드를 추가합니다.
	m_webView->GoForward();
}

#define df_test1
LRESULT CEdgeWnd::OnMessage(WPARAM wParam, LPARAM lParam)
{
	int	key = 0, nLen = 0;
	CString StrData(_T(""));

	switch (LOBYTE(LOWORD(wParam)))
	{
		case DLL_OUBx:
		{
			struct	_extTHx* exth = (struct _extTHx*)lParam;
			struct _web_mod* outweb = (_web_mod*)exth->data;

			m_type = CString(outweb->type, sizeof(outweb->type));
			m_url = CString(outweb->url, sizeof(outweb->url));
			m_strUrl = m_url;
			m_sUrl = m_url;

			m_description = "";//CString(outweb->dumy, sizeof(outweb->dumy));
			m_url.TrimLeft(); m_url.TrimRight();
			m_url.Remove(0);

			CString filename;
			filename.Format("%s\\%s\\%s", m_sRoot, "exe", "ignoreWMAP");
			FILE* fp = fopen(filename, "rb");
			if (fp)
			{
				if (m_type.CompareNoCase("BROWSER") != 0)
				{
					m_type = "BROWSER";
					m_url = GetURL();
				}

				fclose(fp);
			}

			const char* iniName = "ONLYBROWSER.INI";
			CString path;
			path.Format("%s\\tab\\%s", m_sRoot, iniName);
			int	nMode = ProfileInt("MODE", "BrowserMode", 0, path).value;

			if (m_type != "BROWSER" && nMode == 1)
			{
				m_type = "BROWSER";
				m_url = GetURL();
			}

			if (m_type.CompareNoCase("BROWSER") == 0)
			{
				CString url = m_url;
				if (url.Find("${authval}") > 0)
					url.Replace("${authval}", (LPCSTR)GetAuthParam());

				if (url.Find("ibks.com") != -1)	//2014.07.17 KSJ 우리쪽 url만 바꾸자..
				{
					if (url.Find("?") == -1)	//2014.07.21 KSJ 
						url += "?";

					url += SetNoParam();	//2014.07.17 KSJ 방화벽,키보드,aos 해제
				}

				if (url.Find("ibks.com") > -1)
					url.Replace("http://", "https://");

				m_finalurl = url;

				if (m_bCertLogin)
				{
					OutputDebugString("webctrl------------------3test-------------");
					CString surl(m_baseURL);
					m_baseURL.Replace("http", "https");


					CString sParam;
					CString stmp;
					BOOL bCloude{};
					bCloude = CheckCloude();
					if (m_sMapName == "IB831300")  //온라인계좌개설
					{
						stmp = m_sUrl;
						const	int	ifind = stmp.Find("&auth");
						if (ifind > 0)
							stmp = m_sUrl.Left(ifind);

						if (bCloude)
							sParam.Format("url=%s&cflag=1&certType=%s&auth=%s", stmp, "cloud", m_sCert);
						else
							sParam.Format("url=%s&cflag=1&certType=%s&auth=%s", stmp, "", m_sCert);
					}
					else
					{
						if (bCloude)
							sParam.Format("url=%s&cflag=1&certType=%s&auth=%s", m_sUrl, "cloud", m_sCert);
						else
							sParam.Format("url=%s&cflag=1&certType=%s&auth=%s", m_sUrl, "", m_sCert);

						sParam += GetFSDValue();
					}

				
					//webview 에 poastmessage 하는 코드
					auto webView2 = m_webView.query<ICoreWebView2_2>();
					CStringA ansiBody(sParam);  // ANSI 로 변환
					ULONG bodySize = (ULONG)ansiBody.GetLength();

					CComPtr<IStream> postStream;
					if (bodySize > 0) {
						HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, bodySize);
						if (hGlobal) {
							void* pData = GlobalLock(hGlobal);
							memcpy(pData, (LPCSTR)ansiBody, bodySize);
							GlobalUnlock(hGlobal);

							HRESULT hr = CreateStreamOnHGlobal(hGlobal, TRUE, &postStream);
							if (FAILED(hr)) {
								GlobalFree(hGlobal);
								return 0;
							}
						}
					}

					wchar_t strUnicode[256] = { 0, };
					char    strMultibyte[256] = { 0, };
					strcpy_s(strMultibyte, 256, m_baseURL);
					int nLen = MultiByteToWideChar(CP_ACP, 0, strMultibyte, strlen(strMultibyte), NULL, NULL);
					MultiByteToWideChar(CP_ACP, 0, strMultibyte, strlen(strMultibyte), strUnicode, nLen);

					IStream* bodyStream = (bodySize > 0 && postStream) ? postStream.p : nullptr;
					wil::com_ptr<ICoreWebView2Environment2> env2 = m_webViewEnvironment.query<ICoreWebView2Environment2>();
					if (env2) {

m_slog.Format("[cx_WebeEdge] m_webViewEnvironment.query [111]");
OutputDebugString(m_slog);

						wil::com_ptr<ICoreWebView2WebResourceRequest> request;
						HRESULT hr = env2->CreateWebResourceRequest(
							strUnicode,
							L"POST",
							bodyStream,
							L"Content-Type: application/x-www-form-urlencoded\r\n",
							&request);

						if (SUCCEEDED(hr)) {
							auto webView2 = m_webView.query<ICoreWebView2_2>();

m_slog.Format("[cx_WebeEdge] m_webView.query [111.222] ");
OutputDebugString(m_slog);

							if (webView2) {
								webView2->NavigateWithWebResourceRequest(request.get());
							}
						}
					}







					wil::com_ptr<ICoreWebView2> webView = m_webView;
					webView->add_NavigationStarting(
						Microsoft::WRL::Callback<ICoreWebView2NavigationStartingEventHandler>(
							[this](ICoreWebView2* sender, ICoreWebView2NavigationStartingEventArgs* args) -> HRESULT {

m_slog.Format("[cx_WebeEdge] webView->add_NavigationStarting [222] ");
OutputDebugString(m_slog);

							

								return S_OK;
							}).Get(), nullptr);

				}
				else  //공동인증 아닐때 
				{

					if (0)
					{
						BSTR bstrURL = m_finalurl.AllocSysString();
						wchar_t strUnicode[1024] = { 0, };
						char    strMultibyte[1024] = { 0, };
						strcpy_s(strMultibyte, 256, m_finalurl);
						int nLen = MultiByteToWideChar(CP_ACP, 0, strMultibyte, strlen(strMultibyte), NULL, NULL);
						MultiByteToWideChar(CP_ACP, 0, strMultibyte, strlen(strMultibyte), strUnicode, nLen);
						HRESULT hresult = m_webView->Navigate(strUnicode);
					}
					else
					{
						m_finalurl += GetFSDValue();
						wchar_t strUnicode[1024] = { 0, };
						char    strMultibyte[1024] = { 0, };
						memcpy(strMultibyte, m_finalurl.GetBuffer(), m_finalurl.GetLength());
						int nLen = MultiByteToWideChar(CP_ACP, 0, strMultibyte, strlen(strMultibyte), NULL, NULL);
						MultiByteToWideChar(CP_ACP, 0, strMultibyte, strlen(strMultibyte), strUnicode, nLen);
						HRESULT hresult = m_webView->Navigate(strUnicode);
					}
				}
			}
			else  
			{
				AfxMessageBox("확인필요!!");
			}
		}
		break;
	}
	return 0;
}

CString CEdgeWnd::SetNoParam()
{
	CString strReturn;
	strReturn.Format("&keyB_YN=Y&nPro_YN=N&aos_YN=N");

	//2017.01.02 KSJ 직원용이고 8712이면 ID를 넣어준다.
	//if (!m_menu.Compare("IB871200") && IsNumber(m_userID))
	//{
	//	strReturn += "&id=" + m_userID;
	//}

	return strReturn;
}

CString CEdgeWnd::GetURL()
{
	//예외처리 (LoadService.jsp 안타도록 하려면 주석을 해제하세요.
	CString temp(m_url);
	temp.MakeUpper();

	// dhttp 로 시작하면 바로 띄워라!
	if (temp.GetLength() > 5)
	{
		if (temp.Mid(0, 5) == "DHTTP")
			return m_url.Mid(1);
	}

	CString url(m_baseURL);
	url += "?";
	url += "url=";
	url += m_url;
	url += "&auth=";
	url += GetAuthParam();

	return url;
}

CString CEdgeWnd::GetAuthParam()
{
	CString auth = "ip=";

	SYSTEMTIME st, st1;
	CString strTime, strTime1;

	TRY
	{
		GetLocalTime(&st);
		strTime.Format("%04d%02d%02d%02d%02d%02d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

		//2013.08.08 KSJ 외국에서 로그인했을 경우 우리나라와 시간체계가 다르다.
		//그래서 GMT 시간에서 +9를 해준다.
		GetSystemTime(&st1);

		CTime time(st1);
		const	CTimeSpan span(0,9,0,0);
		time = time + span;

		strTime1.Format("%04d%02d%02d%02d%02d%02d", time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());

		auth += URLEncode(m_ipAddr);
		auth += "&mac=" + URLEncode(m_MacAddr);
		auth += "&dt=" + URLEncode(strTime1);
		auth += "&id=" + URLEncode(m_userID);

		CString sPswd = GetUserPassword();

		if (sPswd == "" || sPswd.Find("CERTLOGIN") > -1)
		{
			m_bCertLogin = TRUE;
			auth += "&certpw=" + URLEncode(GetCertPassword());
			m_sCert = URLEncode(HTSEncode(auth, "ibkis"));
		}
		else
		{
			auth += "&pw=" + URLEncode(GetUserPassword());
		}

		// 	auth += "&certpw=" + URLEncode(m_certpw);

		auth = URLEncode(HTSEncode(auth, "ibkis"));

		m_slog.Format("[cx_WebeEdge][weblink][%s]<%d> 최종 [%d][%s]", __FUNCTION__, __LINE__, auth.GetLength(), auth.Left(100));
		OutputDebugString(m_slog);
	}
		CATCH(CMemoryException, e)
	{

	}
	END_CATCH

		return auth;
}

CString CEdgeWnd::URLEncode(const char* lpszURL)
{
	CString dest("");

	struct Encode
	{
	private:
		BOOL isexcept(char ch)
		{
			const char excepts[] = {
				'_', '.', 0x00
			};

			for (int i = 0; excepts[i]; i++)
				if (ch == excepts[i])
					return TRUE;
			return FALSE;
		}
	public:
		CString value;

		Encode(char ch)
		{
			if (isalpha(ch) || (ch >= '0' && ch <= '9') || isexcept(ch))
				value = ch;
			else
				value.Format("%%%02X", ch);
		}
	};

	for (int i = 0; i < (int)strlen(lpszURL); i++)
	{
		dest += Encode(lpszURL[i]).value;
	}

	CString slog;
	slog.Format("[weblink] 인코딩까지 한 서명값 dest len =[%d]", dest.GetLength());
	OutputDebugString(slog);

	return dest;
}

CString CEdgeWnd::GetUserPassword()
{
	//현재는 운용과 개발 두가지 모드만 존재하므로 해당 메인혹은 단위화면의 캡션으로 개발용인지 판단하도록 처리함
	//차후 모드가 어려가지로 분기되면 해당 로직도 변경 되어야함
	CWinApp* app = AfxGetApp();
	CString title;
	CString file;
	CString user;
	m_pMainFrame->GetWindowText(title);

	if (m_pMainFrame)
		return CString((LPCTSTR)m_pMainFrame->SendMessage(WM_USER, MMSG_GETPASSWORD));

	return "";
}

CString CEdgeWnd::GetCertPassword()
{
	CWinApp* app = AfxGetApp();
	CString title;
	m_pMainFrame->GetWindowText(title);
	
	if (m_pMainFrame)
		return CString((LPCTSTR)m_pMainFrame->SendMessage(WM_USER, MMSG_GETCERTPASSWORD));
	else
		return "";
}

CString CEdgeWnd::HTSEncode(const char* lpszSource, const char* key)
{
	const	int	lenKey = strlen(key);
	const	int	lenSource = strlen(lpszSource);

	if (lenSource == 0 || lenKey == 0)
		return "";

	std::unique_ptr<char[]> buffer = std::make_unique<char[]>(lenSource + 1);

	for (int i = 0; i < lenSource; i++)
	{
		buffer[i] = (lpszSource[i] ^ (0x0f & key[i % lenKey])) - 1;
	}
	buffer[lenSource] = 0x00;

	CString result = CString(buffer.get());

	/** Decoding test

	for (i = 0; i < lenSource; i++)
	{
		buffer[i] = (result[i] + 1) ^ (0x0f & key[i % lenKey]);
	}
	**/

	buffer.reset();
	return result;
}

void CEdgeWnd::GetInformation()
{
	const char* iniName = "WebInfo.ini";

	CString path;
	path.Format("%s\\tab\\%s", m_sRoot, iniName);

	m_width = ProfileInt(m_menu, "Width", 0, path).value;
	if (!m_width)
		m_width = ProfileInt("Common", "DefaultWidth", 0, path).value;

	m_height = ProfileInt(m_menu, "Height", 0, path).value;
	if (!m_height)
		m_height = ProfileInt("Common", "DefaultHeight", 0, path).value;

	m_type = ProfileStr(m_menu, "Type", "", path).value;
	m_description = ProfileStr(m_menu, "Description", "", path).value;
}

void CEdgeWnd::SearchURL()
{
	m_bNavigate2 = false;

	if (m_menu.IsEmpty())
		return;
	if (m_sRoot.IsEmpty())
		return;

	GetInformation();
	//m_pWizard->SetWindowPos(NULL, 0, 0, m_width, m_height, SWP_NOMOVE | SWP_NOZORDER);  //!!!!
	MoveWindow(CRect(0,0, m_width, m_height), TRUE);
	//ResizeToFitWindow();
	struct	_web_mid Mid;
	FillMemory(&Mid, sz_WEBMID, ' ');

	CopyMemory(Mid.gubn, "Q", 1);
	CopyMemory(Mid.scno, m_menu, m_menu.GetLength());

	CString strData((char*)&Mid, sz_WEBMID);
	SendTR(_T("PMHOWURL"), NULL, strData, (BYTE)1);
}

void CEdgeWnd::ResizeToFitWindow()
{
	CWnd* pParent = GetParent();
	if (pParent == NULL)
		return;

	CRect rc;
	pParent->GetClientRect(rc);

    MoveWindow(rc, TRUE); 
	 
	CRect src;
	this->GetClientRect(src);
	rc.bottom = rc.bottom - (src.Height() - rc.Height());

	//2013.10.31 KSJ 위,왼쪽에 border가 보여서 사이즈 조절해줌.
	src.top -= 2;
	src.left -= 2;
	src.bottom += 2;
	src.right += 2;

	//if (m_webView)   //!!!!!!????? 사이즈 조절 필요
	//	m_webView->MoveWindow(src, TRUE);
}

LRESULT CEdgeWnd::SendTR(CString strName, BYTE type, CString strData, BYTE key)
{
	char	buff[256]{};
	struct _userTH udat {};
	int	idx = m_Param.name.GetLength();

	// control name
	CopyMemory(buff, m_Param.name, idx);
	buff[idx++] = '\t';

	// userTH
	CopyMemory(udat.trc, strName, strName.GetLength());
	udat.stat = type;
	udat.key = m_Param.key;
	CopyMemory(&buff[idx], &udat, L_userTH);
	idx += L_userTH;

	// data	
	CopyMemory(&buff[idx], strData, strData.GetLength());

	return m_pParent->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, strData.GetLength()), (LPARAM)buff);
}

BOOL CEdgeWnd::CheckCloude()
{
	OutputDebugString("webctrl------------------CheckCloude-------------");
	char chfile[500]{};
	CString stmp, spath;
	spath = m_sRoot;
	spath += "tab\\axis.ini";

	int readL;
	memset(chfile, 0x00, 500);

	GetPrivateProfileString("CLOUDELOGIN", "USE", "0", chfile, sizeof(chfile), spath);
	stmp.Format("%s", chfile);
	stmp.TrimRight();

	if (atoi(stmp) == 1)
		return TRUE;
	else
		return FALSE;
}

CString CEdgeWnd::GetFSDValue()
{
	if (m_description.Find("NOFDS") >= 0)
		return "";

	//URLEncode
	CString sFDS{}, sRes{};
	CString filename{}, sPath{}, stmp{};
	filename.Format("%s", m_sRoot);
	filename.Format("%s\\%s\\%s", m_sRoot, "tab", "AXISENC.INI");
	CFileFind   finder;

	int readL = 0;
	char readB[8]{};
	readL = GetPrivateProfileString("FDS", "weblink", "0", readB, sizeof(readB), filename);
	stmp.Format("%s", readB);
	stmp.TrimRight();

	if (stmp == "1")
	{
		int	nRet = 0;
		sPath.Format("%s\\dev\\I3GManager.dll", m_sRoot);

		HMODULE	hInfoDll = LoadLibrary(sPath);
		if (hInfoDll == nullptr)
		{
			return sRes;
		}

		int nThreadTimeOut = 20;	// thread timeout
		char szWData[2048] = { 0, };
		typedef bool (*NPGetHtsWData_Func) (__in int nThreadTimeOut, __out char* szWData);
		// 데이터 수집
		if (hInfoDll) {
			FARPROC pF = GetProcAddress(hInfoDll, "NPGetHtsWData");
			if (pF)
			{

				std::unique_ptr<char[]> desB = std::make_unique<char[]>(2048);

				memset(desB.get(), ' ', 2048);
				NPGetHtsWData_Func npFunc = (NPGetHtsWData_Func)pF;

				bool bResult = npFunc(nThreadTimeOut, szWData);
				if (bResult) {
					CString sval;
					sval.Format("%s", szWData);
					sval.TrimRight();
					nRet = sval.GetLength();

					if (0)
					{
						int ilen{};
						if (nRet < 1024)
						{
							ilen = 1024;
							stmp.Format("FDS  version2 nRet[%d] is lower than 1024", nRet);
							OutputDebugString(stmp);
						}
						else
							ilen = nRet;

						FillMemory(desB.get(), ilen, ' ');
						desB[ilen] = 0x00;
						CopyMemory(desB.get(), szWData, nRet);
						nRet = ilen;
					}
					else
					{
						FillMemory(desB.get(), nRet, ' ');
						desB[nRet] = 0x00;
						CopyMemory(desB.get(), szWData, nRet);
					}

					sFDS.Format("%s", desB.get());

					stmp.Format("[FDS][web] sFDS[%d] [%s]", sFDS.GetLength(), sFDS.Left(50));
					OutputDebugString(stmp);

					sFDS = URLEncode(sFDS);  //
					sFDS = "&FDS=" + sFDS;
					sRes += sFDS;

					if (hInfoDll)
					{
						FreeLibrary(hInfoDll);
						hInfoDll = NULL;
					}
				}
			}
		}
	}
	return sRes;
}


void CEdgeWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CWnd::OnPaint()을(를) 호출하지 마십시오.
	CRect rc;
	GetClientRect(rc);
	dc.FillSolidRect(rc, RGB(255, 255, 255));
}
