// ControlWnd.cpp: 구현 파일
//

//#include "UrlEnc.h"
#include "StdAfx.h"
#include "CX_WEBEDGE.h"
#include "ControlWnd.h"
#include "AxBrowser.h"
#include "CURLEnc.h"



// ControlWnd

IMPLEMENT_DYNAMIC(ControlWnd, CWnd)


// {8F1E8118-D3F2-4D77-BAC0-2109371E8AA7}
static const IID IID_IControlWnd =
{ 0x8f1e8118, 0xd3f2, 0x4d77, { 0xba, 0xc0, 0x21, 0x9, 0x37, 0x1e, 0x8a, 0xa7 } };

BEGIN_DISPATCH_MAP(ControlWnd, CWnd)
	DISP_FUNCTION(ControlWnd, "Seturl2", Seturl2, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION_ID(ControlWnd, "Navigate", dispidNavigate, Navigate, VT_EMPTY, VTS_BSTR)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(ControlWnd, CWnd)
	INTERFACE_PART(ControlWnd, IID_IControlWnd, Dispatch)
END_INTERFACE_MAP()

ControlWnd::ControlWnd()
{
	EnableAutomation();
}

ControlWnd::~ControlWnd()
{
}

void ControlWnd::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CWnd::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(ControlWnd, CWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()



// ControlWnd 메시지 처리기




int ControlWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	if (m_bScroll)
	{
		m_Htmlrc.SetRect(-2, -2, m_Param.rect.Width() + 2, m_Param.rect.Height() + 2);
		m_pHtml = new CAxBrowser;
		m_pHtml->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, m_Htmlrc, this, 0);
	}
	else
	{
		std::unique_ptr<CWnd> pRefView = std::make_unique<CWnd>();
		pRefView->Create(NULL, NULL, WS_CHILD | WS_VSCROLL, CRect(0, 0, 50, 50), this, 0);

		CRect	crc;
		pRefView->GetClientRect(&crc);
		pRefView.reset();

		m_scrGap = 50 - crc.Width();

		m_Htmlrc.SetRect(-2, -2, m_Param.rect.Width() + 2 + m_scrGap, m_Param.rect.Height() + 2 + m_scrGap);
		m_pHtml = new CAxBrowser;
		m_pHtml->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, m_Htmlrc, this, 0);
		m_pHtml->SetParentView(m_pParent);
		m_pHtml->m_scrGap = m_scrGap;
	}

	return 0;
}

void ControlWnd::SetParam(_param* pParam)
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

	if (m_Param.options.Find("k1") >= 0)
		m_mode = BROWSER_POPUP;
	else if(m_Param.options.Find("k1") >= 0)
		m_mode = EMBEDE_IE;
	else if(m_Param.options.Find("k2") >= 0)
		m_mode = INSTALL_EDGE;
	else
		m_mode = BROWSER_POPUP;
}

void ControlWnd::Seturl2(LPCTSTR url)
{
	if (m_pHtml)
	{
		//m_pHtml->SetSilent(TRUE);
		//	SetBrowserReg();
		//m_pHtml->Navigate2(url);
	//	PostMessage(WM_CLOSE, 0, 0);
		
	}
}

void ControlWnd::Browserpopup(CString strurl)
{
	CURLEnc _clEnc;
	m_strurl = strurl;
	m_strurl.Trim();
	BOOL bGoogleDoc{};
	if (m_strurl.Find(TEXT_GOOGLEDOC) >= 0)
	{
		bGoogleDoc = TRUE;
		m_strurl.Replace("https:\\\\docs.google.com\\viewer?url=", "");
	}

	CString strkor = _clEnc.ExtractHangulCharacters(m_strurl);

m_slog.Format("[edge] 1. strkor = [%s] ", strkor);
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

	ShellExecute(NULL, _T("open"), m_strurl, NULL, NULL, SW_SHOWNORMAL);
	m_pParent->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_Param.key, evOnClick)), (LPARAM)m_Param.name.GetString());
}

void ControlWnd::Navigate(BSTR sUrl)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString strUrl;
	strUrl.Format("%s", sUrl);

	CString slog;
	slog.Format("[edge][%s]m_mode [%d] strUrl=[%s]", __FUNCTION__,  m_mode, strUrl);
	OutputDebugString(slog);

	if (m_mode == EMBEDE_IE)
	{
		if (m_pHtml)
			m_pHtml->Navigate2(strUrl);
	}
	else if (m_mode == BROWSER_POPUP)
	{
		CString m_strurl;
		m_strurl.Format("%s", sUrl);
		Browserpopup(m_strurl);
	}
	else if (m_mode == INSTALL_EDGE)
	{

	}

	//// TODO: 여기에 디스패치 처리기 코드를 추가합니다.
}
