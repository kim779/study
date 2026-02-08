// MainWnd.cpp : implementation file
//


#include "stdafx.h"
#include "CX_WEBEDGE.h"
#include "MainWnd.h"
#include "../../H/axisfire.h"
#include "CURLEnc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TM_IB2023 9999

CMainWnd::CMainWnd()
{
	EnableAutomation();
	HINSTANCE hins;
	BrowserWindowEdge::InitInstance(hins);
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
	ON_WM_TIMER()
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CMainWnd, CWnd)
	//{{AFX_DISPATCH_MAP(CMainWnd)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
	DISP_FUNCTION_ID(CMainWnd, "Navigate", dispidNavigate, Navigate, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION_ID(CMainWnd, "Navigatesrc", dispidNavigatesrc, Navigatesrc, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION_ID(CMainWnd, "closeEdge", dispidcloseEdge, closeEdge, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CMainWnd, "GoBack", dispidGoBack, GoBack, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CMainWnd, "GoForward", dispidGoForward, GoForward, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CMainWnd, "SetScreenZoomRate", dispidSetScreenZoomRate, SetScreenZoomRate, VT_EMPTY, VTS_R8)
	DISP_FUNCTION_ID(CMainWnd, "BrowserPopup", dispidBrowserPopup, BrowserPopup, VT_EMPTY, VTS_BSTR)
END_DISPATCH_MAP()

// Note: we add support for IID_IMainWnd to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {1F53C596-F91C-43E7-A3FA-98E92E2B137D}
static const IID IID_IMainWnd =
{ 0x1f53c596, 0xf91c, 0x43e7, { 0xa3, 0xfa, 0x98, 0xe9, 0x2e, 0x2b, 0x13, 0x7d } };

BEGIN_INTERFACE_MAP(CMainWnd, CWnd)
	INTERFACE_PART(CMainWnd, IID_IMainWnd, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainWnd message handlers


int CMainWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	
	browserEdge = std::make_unique<BrowserWindowEdge>();
	CRect rect;
	GetClientRect(rect);

	if (!browserEdge->Create(nullptr, "BrowserWindowEdge", WS_CHILD | WS_VISIBLE, rect, this, 0)) {
		m_slog.Format("[edge] [%s] create failure", __FUNCTION__);
		OutputDebugString(m_slog);
		AfxMessageBox("edge create fail");
	}
	
	CString strEmptyPage = _T("about:blank");
	std::string message_a = strEmptyPage;
	std::wstring wurl;
	wurl.assign(message_a.begin(), message_a.end());

	browserEdge->Init(wurl);
	m_slog.Format("[edge] [%s] create success", __FUNCTION__);
	OutputDebugString(m_slog);


	return 0;
}



void CMainWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

}



void CMainWnd::Navigate(BSTR url)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	KillTimer(TM_IB2023);

	CURLEnc _clEnc;

	m_strurl.Format("%s", url);


m_slog.Format("[edge][%s]  m_strurl=[%s]", __FUNCTION__, m_strurl);
OutputDebugString(m_slog);

	BOOL bGoogleDoc{};
	if (m_strurl.Find(TEXT_GOOGLEDOC) >= 0)
	{
		bGoogleDoc = TRUE;
		m_strurl.Replace("https:\\\\docs.google.com\\viewer?url=", "");
	}

	CString strkor = _clEnc.ExtractHangulCharacters(m_strurl);

m_slog.Format("[edge] 1. bGoogleDoc=[%d] strkor = [%s] ", bGoogleDoc,  strkor);
OutputDebugString(m_slog);

	std::wstring strUni = CA2W(strkor); // std::wstring에 어떤 값이 들어가 있는지에 따라 선언
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	std::string strUTF8 = converter.to_bytes(strUni);
	std::string stdres = _clEnc.ToUpperCase(_clEnc.URLEncodeUTF8(strUTF8));
	CString strEnCode;
	strEnCode = stdres.c_str();
	m_strurl.Replace(strkor, strEnCode);

m_slog.Format("[edge] 2. m_strurl = [%s] ", m_strurl);
OutputDebugString(m_slog);
		
	if (bGoogleDoc)
	{
		m_strurl.Replace("\\", "/");
		std::wstring strUni1 = CA2W(m_strurl); // std::wstring에 어떤 값이 들어가 있는지에 따라 선언
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converteruq;
		std::string struqUTF8 = converteruq.to_bytes(strUni1);
		std::string strres = _clEnc.ToUpperCase(_clEnc.URLEncodeUniqueUTF8(struqUTF8));
		m_strurl = strres.c_str();
		m_strurl = "https:\\\\docs.google.com\\viewer?url=" + m_strurl;

m_slog.Format("[edge] 3. m_strurl = [%s] ", m_strurl);
OutputDebugString(m_slog);
	}
	
	std::string message_a = m_strurl;
	std::wstring wurl;
	wurl.assign(message_a.begin(), message_a.end());
	browserEdge->Init(wurl);
}

void CMainWnd::Navigate_strUrl()
{
	//KillTimer(TM_IB2023);
	//if (m_webView == nullptr)
	//{
	//	SetTimer(TM_IB2023, 1000, nullptr);
	//	return;
	//}

	//// TODO: 여기에 디스패치 처리기 코드를 추가합니다.
	//wchar_t strUnicode[256] = { 0, };
	//char    strMultibyte[256] = { 0, };
	//strcpy_s(strMultibyte, 256, m_strurl);
	//int nLen = MultiByteToWideChar(CP_ACP, 0, strMultibyte, strlen(strMultibyte), NULL, NULL);
	//MultiByteToWideChar(CP_ACP, 0, strMultibyte, strlen(strMultibyte), strUnicode, nLen);

	//HRESULT hresult = m_webView->Navigate(strUnicode);

	//if (hresult == S_OK)
	//{
	//	OutputDebugString("[cx_edge]Web Page Opened Successfully");
	//	ResizeEverything();
	//}
	//else
	//{
	//	TRACE("Navigate_strUrl navigate fail!!!!!!!!!!!!!!!");
	//}
}

void CMainWnd::Navigatesrc(BSTR src)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 여기에 디스패치 처리기 코드를 추가합니다.
	
}

void CMainWnd::closeEdge()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	
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


void CMainWnd::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (nIDEvent)
	{
		case TM_IB2023:
		{
			KillTimer(nIDEvent);

		}
			break;
	}
	CWnd::OnTimer(nIDEvent);
}


void CMainWnd::GoBack()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 여기에 디스패치 처리기 코드를 추가합니다.
}


void CMainWnd::GoForward()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	//m_webView->GoForward();
	// TODO: 여기에 디스패치 처리기 코드를 추가합니다.
}


void CMainWnd::SetScreenZoomRate(DOUBLE dRate)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (browserEdge)
		browserEdge->m_dZoomFactor = dRate;
	// TODO: 여기에 디스패치 처리기 코드를 추가합니다.
}


CString CMainWnd::URLEncode(const char* lpszURL)
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

	return dest;
}


#include <iostream>
#include <string>
#include <cctype>
#include <sstream>
#include <iomanip>

std::string CMainWnd::UrlEncode(const std::string& value) {
	std::ostringstream escaped;
	escaped.fill('0');
	escaped << std::hex;

	for (auto&& ch : value) {
		// 안전한 문자는 그대로 유지, 특수문자는 URL 인코딩
		if (std::isalnum(ch) || ch == '-' || ch == '_' || ch == '.' || ch == '~') {
			escaped << ch;
		}
		else if (ch == ' ') {
			escaped << '+';
		}
		else {
			escaped << '%' << std::setw(2) << static_cast<unsigned int>(static_cast<unsigned char>(ch));
		}
	}

	return escaped.str();
}

void CMainWnd::BrowserPopup(BSTR sUrl)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString strUrl;
	strUrl.Format("%s", sUrl);
	ShellExecute(NULL, _T("open"), strUrl, NULL, NULL, SW_SHOWNORMAL);
	m_pParent->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_Param.key, evOnClick)), (LPARAM)m_Param.name.GetString());
	// TODO: 여기에 디스패치 처리기 코드를 추가합니다.
}
