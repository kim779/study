
// WINNERSDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "WINNERS.h"
#include "WINNERSDlg.h"
#include "afxdialogex.h"
//#include "IEView.h"
#define DF_WIDTH 1380
#define DF_HEIGHT 760



//#define DF_TEST 1

//#define DF_HTMLVIEW 1
#define DF_CREATEIE 1

#ifdef DF_TEST
#define DF_URL  _T("http://daum.net")
#define DF_FINDURL  _T("daum.net") 
#else 
#define DF_URL  _T("http://172.16.210.61:8000/login.do?htsUserId=")
#define DF_FINDURL _T("http://172.16.210.61:8000")
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CWINNERSDlg 대화 상자



CWINNERSDlg::CWINNERSDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_WINNERS_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);


	//m_pMain = nullptr;
}

void CWINNERSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CWINNERSDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CWINNERSDlg 메시지 처리기

BOOL CWINNERSDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
#ifdef DF_CREATEIE
	ShowWindow(SW_SHOWMINIMIZED);//! 최소화후 숨겨야 화면에 나타나지 않음
	PostMessage(WM_SHOWWINDOW, FALSE, SW_OTHERUNZOOM);
#else	
	SetWindowPos(nullptr, 0, 0, DF_WIDTH, DF_HEIGHT,  SWP_NOMOVE);
#endif
	PopWinner();
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CWINNERSDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CWINNERSDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CWINNERSDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CWINNERSDlg::PopWinner()
{
	CString plainID, strRoot, slog;

	LONG lResult;
	HKEY hRKey;
	DWORD dwType;
	DWORD dwBytes = 100;
	char buffer[100];

	char chfile[500]{};
	GetModuleFileName(nullptr, chfile, 260);

	CString spath, stmp;
	spath.Format("%s", chfile);
	spath.TrimRight();

	int iFind = spath.Find("exe");
	spath = spath.Left(iFind);
	m_root = spath;
	spath += "tab\\axis.ini";

	int readL;
	memset(chfile, 0x00, 500);

	GetPrivateProfileString("WINNERS", "ID", "", chfile, sizeof(chfile), spath);
	stmp.Format("%s", chfile);
	stmp.TrimRight();

	CString surl;
	surl = DF_URL;
	surl += stmp;

	ShowWindow(SW_HIDE);
	CreateShellExcute(strRoot, surl);

	ShowWindow(SW_SHOWMINIMIZED);//! 최소화후 숨겨야 화면에 나타나지 않음
	PostMessage(WM_SHOWWINDOW, FALSE, SW_OTHERUNZOOM);

	return;


	lResult = RegOpenKeyEx(HKEY_CURRENT_USER,
#ifdef DF_TEST
		_T("Software\\IBK_WORK\\AXIS Workstation V03.00\\Workstation"),
#else
		_T("Software\\IBKMAC_STAFF\\AXIS Workstation V03.00\\Workstation"),
#endif
		0, KEY_ALL_ACCESS, &hRKey);
	if (lResult != ERROR_SUCCESS)
	{
		lResult = RegOpenKeyEx(HKEY_CURRENT_USER, _T("Software\\IBK_NEW_STAFF\\AXIS Workstation V04.00\\Workstation"), 0, KEY_ALL_ACCESS, &hRKey);
		if (lResult != ERROR_SUCCESS)
		{
			lResult = RegOpenKeyEx(HKEY_CURRENT_USER, _T("Software\\IBK_MAC_STAFF\\AXIS Workstation V04.00\\Workstation"), 0, KEY_ALL_ACCESS, &hRKey);
			if (lResult != ERROR_SUCCESS)
			{
				slog.Format(_T("[winners]   Register Open Error"));
				OutputDebugString(slog);
				AfxMessageBox(slog);
				SetTimer(9898, 8000, NULL);
				return;
			}
		}
	}

	// Read Regstry Key 
	lResult = RegQueryValueEx(hRKey, _T("SignOnID"), 0, &dwType, (LPBYTE)buffer, &dwBytes);
	if (lResult == ERROR_SUCCESS)
	{
		slog.Format(_T("[winners]   Register Open RegQueryValueEx  [%s]"), buffer);
		OutputDebugString(slog);
	}
	else
	{
		slog.Format(_T("[winners]   Register Open RegQueryValueEx fail"));
		OutputDebugString(slog);
		AfxMessageBox(slog);
		SetTimer(9898, 8000, NULL);
		return;
	}

	plainID.Format(_T("%s"), buffer);
	plainID = Parser(plainID, _T("|"));

	slog.Format(_T("[winners]  ID = [%s]\n"), plainID);
	OutputDebugString(slog);




	CString strurl;
	strurl = DF_URL;
#ifndef DF_TEST
	strurl += plainID;
#endif

	SetWindowText(strurl);
	
	//CreateIE(strurl);
	

#ifdef DF_HTMLVIEW
	CRect rc;
	GetClientRect(&rc);
	IEView* pIEview = new IEView();
	pIEview->Create(NULL, _T(""), WS_CHILD | WS_VISIBLE, rc, this, 20008);
	pIEview->OnInitialUpdate(strurl);
#else

	#ifdef  DF_CREATEIE
	ShowWindow(SW_HIDE);
	CreateShellExcute(strRoot, strurl);
	
	ShowWindow(SW_SHOWMINIMIZED);//! 최소화후 숨겨야 화면에 나타나지 않음
	PostMessage(WM_SHOWWINDOW, FALSE, SW_OTHERUNZOOM);
	//CreateIEPorcess(strRoot, strurl);
	#else
		
		m_pdlg = new CWebDlg;
		m_pdlg->m_strurl = strurl;
		m_pdlg->m_iwidth = DF_WIDTH;
		m_pdlg->m_iheight = DF_HEIGHT;
		//m_pdlg->DoModal();
		m_pdlg->Create(IDD_WEBDLG, this);
		m_pdlg->ShowWindow(SW_SHOW);
	#endif
	/*
	m_pMain = new CMainWnd;

	if (!m_pMain->Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, rc, this, (UINT)20008))
	{
		//AfxMessageBox("window create fail");
		OutputDebugString(_T("[edge] create fail"));
		delete m_pMain;
	}
	m_pMain->m_strurl = strurl;
	*/
	/*
	m_pIEView = new CWebBrowserCtrl;
	if (!m_pIEView->Create("WebBrowser2", WS_VISIBLE | WS_CHILD, rc, this, 20900, NULL))
	{
		m_slog.Format("[WINNERS] create fail");
		return;
	}
	m_pIEView->Navigate(strurl, nullptr, nullptr, nullptr, nullptr);
	*/
#endif
}

CString CWINNERSDlg::Parser(CString& srcstr, CString substr)
{
	if (srcstr.Find(substr) == -1)
	{
		CString temp = srcstr;
		srcstr.Empty();
		return temp;
	}
	else
	{
		CString  temp = srcstr.Left(srcstr.Find(substr));
		srcstr = srcstr.Mid(srcstr.Find(substr) + substr.GetLength());
		return temp;
	}
	return _T("");
}

void CWINNERSDlg::CreateIEPorcess(CString strurl, CString strRoot)
{
	//STARTUPINFO si;
	//PROCESS_INFORMATION pi;
	//WCHAR cmd[256] = { 0, };
	//CString  starget;
	//starget = _T("C:\\Program Files\\internet explorer\\iexplore.exe");

	//memset(&si, 0, sizeof(STARTUPINFO));
	//wsprintf(cmd, _T("%s %s"), starget, strurl);

	//si.cb = sizeof(STARTUPINFO);

	//si.dwFlags = STARTF_USESIZE | STARTF_USESHOWWINDOW | STARTF_USEPOSITION | STARTF_USESTDHANDLES;

	//si.dwX = 0;
	//si.dwY = 0;
	//si.dwXSize = DF_WIDTH;
	//si.dwYSize = DF_HEIGHT;
	//CreateProcess(NULL, cmd, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi);
	//SetTimer(9898, 8000, NULL);
}

void CWINNERSDlg::CreateShellExcute(CString strRoot , CString  strurl)
{
	ShellExecute(NULL, _T("open"), _T("C:\\Program Files\\internet explorer\\iexplore.exe"), strurl, NULL, SW_SHOW );
	//ShellExecute(NULL, _T("open"), _T("C:\\Chrome\\GoogleChromePortable.exe"), strurl, NULL, SW_SHOW | SW_SHOWMAXIMIZED);
	SetTimer(9898, 8000, NULL);
}

BOOL CWINNERSDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
		{
			return TRUE;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CWINNERSDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

}

void CWINNERSDlg::CreateIE(CString strurl)
{
	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	CString stemp, stmp;
	HWND hWnd = NULL;
	clock_t start = clock();

	for (;;) { //CreateIE
		clock_t end = clock();
		double time = double(end - start) / CLOCKS_PER_SEC; //초단위 변환

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		Sleep(500);

		m_slog.Format(_T("[WINNER]"));
		OutputDebugString(m_slog);

		//if (pi.dwProcessId)
		{
			IShellWindows* pShellWindows = NULL;
			IWebBrowser2* pWebBrowser = NULL;
			IDispatch* pDisp = NULL;
			HWND hWnd = NULL;
			long i, lCount = 0;
			VARIANT varIndex;
			varIndex.vt = VT_I4; // 4바이트 롱형
			DWORD pid;
			BOOL bSuc = FALSE;
			BOOL bExistWinners = FALSE;
			//	

			if (S_OK == CoCreateInstance(CLSID_ShellWindows, NULL, CLSCTX_ALL, IID_IShellWindows, (void**)&pShellWindows))
			{
				if (S_OK == pShellWindows->get_Count(&lCount))
				{
					for (i = 0; i < lCount; i++)
					{
						varIndex.lVal = i;
						if (S_OK == pShellWindows->Item(varIndex, &pDisp))
						{
							if (S_OK == pDisp->QueryInterface(IID_IWebBrowser2, (void**)&pWebBrowser))
							{
								if (S_OK == pWebBrowser->get_HWND((SHANDLE_PTR*)&hWnd))
								{
									if (GetWindowThreadProcessId(hWnd, &pid))
									{
										CWnd* pcwnd;
										pcwnd = CWnd::FromHandle(hWnd);
										pcwnd->GetWindowText(stemp);

										m_slog.Format(_T("[WINNER]  iweb title=[%s]  strurl=[%s]\n"), stemp, strurl);
										OutputDebugString(m_slog);
										//delete[] pdata;

										if (stemp.Find(_T("빈")) >= 0 && stemp.Find(_T("페이지")) >= 0)
										{
											bSuc = TRUE;
											bExistWinners = FALSE;
											m_slog.Format(_T("[WINNER]!!!!!!!!!!!!!!!!!!!!!!  [%s]를 찾았다 FIND handle by iweb"), stemp);
											OutputDebugString(m_slog);
											pDisp->Release();

											// 메뉴바를 출력할지 설정한다 
											pWebBrowser->put_MenuBar(VARIANT_FALSE);
											// 툴바를 출력할지 설정한다 
											pWebBrowser->put_ToolBar(VARIANT_FALSE);
											// 주소창을 출력할지 설정한다 
											pWebBrowser->put_AddressBar(VARIANT_FALSE);
											// 상태바를 출력할지 설정한다 
											pWebBrowser->put_StatusBar(VARIANT_FALSE);
											// 크기 조절을 가능하게 할지 설정한다 
											pWebBrowser->put_Resizable(VARIANT_TRUE);

											VARIANT vtFlags, vtTarget, vtPostData, vtHeader;
											::VariantInit(&vtFlags);
											::VariantInit(&vtTarget);
											::VariantInit(&vtPostData);
											::VariantInit(&vtHeader);

											pWebBrowser->put_Width(DF_WIDTH);
											pWebBrowser->put_Height(DF_HEIGHT);
											//	pWebBrowser->put_Visible(VARIANT_TRUE);

											HRESULT hr = pWebBrowser->Navigate(strurl.AllocSysString(), &vtFlags, &vtTarget,
												&vtPostData, &vtHeader);
											m_slog.Format(_T("[WINNER] web navigate=[%d]\n"), hr);
											OutputDebugString(m_slog);
											/*
											LONG_PTR lwnd = NULL;
											pWebBrowser->get_HWND(&lwnd);
											HWND hwnd = reinterpret_cast<HWND>(lwnd);
											pcwnd = CWnd::FromHandle(hwnd);
											pcwnd->SendMessage( WM_SYSCOMMAND, SC_RESTORE, 0);
											*/
											CoUninitialize();
											PostMessage(WM_CLOSE, 0, 0);
											return;
											break;
										}
									}
								}
								pDisp->Release();
								pWebBrowser->Release();
							}
							pDisp->Release();
						}
					}
				}
				pShellWindows->Release();
			}

			if (time > 4)
				break;
		}
	}
	PostMessage(WM_CLOSE, 0, 0);
	CoUninitialize();
}


void CWINNERSDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (nIDEvent)
	{
		case 9898:
		{
			KillTimer(nIDEvent);
			PostMessage(WM_CLOSE, 0, 0);
		}
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}
