// WebLinkCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "CX_Browser.h"
#include "WebLinkCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// WebLinkCtrl

WebLinkCtrl::WebLinkCtrl()
{
	EnableAutomation();
	m_parent = NULL;

	m_className = AfxRegisterWndClass(0);
}

WebLinkCtrl::~WebLinkCtrl()
{
}

void WebLinkCtrl::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CWnd::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(WebLinkCtrl, CWnd)
	//{{AFX_MSG_MAP(WebLinkCtrl)
	ON_WM_CREATE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_BROWSER, OnBrowser)
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(WebLinkCtrl, CWnd)
	//{{AFX_DISPATCH_MAP(WebLinkCtrl)
	DISP_FUNCTION(WebLinkCtrl, "Navigate", Navigate, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(WebLinkCtrl, "PopIE", PopIE, VT_I2, VTS_BSTR)
	DISP_FUNCTION(WebLinkCtrl, "PopUrl", PopUrl, VT_I2, VTS_BSTR)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IWebLinkCtrl to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {878CA46C-2C1A-4BF3-97DB-E6BE28A91E8E}
static const IID IID_IWebLinkCtrl =
{ 0x878ca46c, 0x2c1a, 0x4bf3, { 0x97, 0xdb, 0xe6, 0xbe, 0x28, 0xa9, 0x1e, 0x8e } };

BEGIN_INTERFACE_MAP(WebLinkCtrl, CWnd)
	INTERFACE_PART(WebLinkCtrl, IID_IWebLinkCtrl, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// WebLinkCtrl message handlers

void WebLinkCtrl::Navigate(LPCTSTR url) 
{
	// TODO: Add your dispatch handler code here
}

BOOL WebLinkCtrl::SetParam(CWnd* parent, _param* param)
{
	m_parent = parent;
	SetParam(param);
	m_param = param;

//	
	return true;
}

void WebLinkCtrl::SetParam(_param *pParam)
{
	CString stmp;

	m_Param.key = pParam->key;
	m_Param.name = CString(pParam->name, pParam->name.GetLength());
	m_Param.rect = CRect(pParam->rect.left, pParam->rect.top, pParam->rect.right, pParam->rect.bottom);
	m_Param.fonts = CString(pParam->fonts, pParam->fonts.GetLength());
	m_Param.point = pParam->point;
	m_Param.style = pParam->style;
	m_Param.options = CString(pParam->options, pParam->options.GetLength());

	int	ipos = m_Param.options.Find("/k");
	m_igubn = atoi(m_Param.options.Mid(ipos + 2, 1));
	ipos = m_Param.options.Find("/t");
	stmp = m_Param.options.Mid(ipos + 2);
	
	m_width = atoi(Parser(stmp, "^"));
	m_height = atoi(stmp);
}	

CString WebLinkCtrl::Parser(CString &srcstr, CString substr)
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
	return "";
}

void WebLinkCtrl::ResizeToFitWindow() 
{
	const	CWnd	*pParent = GetParent();

	if (pParent == NULL)
	{
		TRACE("[cx_weblink] resizetofitwindow error");
		return;
	}
	
	CRect rc;
	pParent->GetClientRect(rc);
	
//	if(!m_bNavigate2)
		MoveWindow(rc, TRUE);
	
	CRect	src;
	this->GetClientRect(src);
	rc.bottom = rc.bottom - (src.Height()-rc.Height());
	
	//2013.10.31 KSJ 위,왼쪽에 border가 보여서 사이즈 조절해줌.
	src.top -= 2;
	src.left -= 2;
	src.bottom +=2;
	src.right +=2;
	
	if (m_ctrl)
		m_ctrl->MoveWindow(src, TRUE);
}

const UINT constWebLinkCtrlID		= 20900;
const UINT constWebBrowserCtrlID	=  1000;
BOOL WebLinkCtrl::create(CWnd* parent, _param* param)
{

	CString slog;
	const	BOOL	bResult = CWnd::Create(m_className, _T(""), WS_VISIBLE | WS_CHILD, CRect(0, 0, 0, 0), 
					m_parent, constWebLinkCtrlID, NULL);
	
	if (bResult)
	{
		m_ctrl = std::make_unique<CWebBrowserCtrl>();
		if (!m_ctrl->Create(_T("WebBrowser2"), WS_VISIBLE | WS_CHILD, CRect(-2, -2, 1, 1), this, constWebBrowserCtrlID))
		{
			m_ctrl.reset();
		}
		ResizeToFitWindow();
	}
	
	if (SUCCEEDED(CoInitialize(NULL)))
	{
		const HRESULT hrco = ::CoCreateInstance(CLSID_InternetExplorer, NULL, CLSCTX_SERVER, 
			IID_IWebBrowser2, (void**)&m_pBrowser2);
		
		if (FAILED(hrco))
		{
			return 0;
		}
		CString url = "www.ibks.com";

		m_pBrowser2->put_MenuBar(false);
		m_pBrowser2->put_ToolBar(false);
		m_pBrowser2->put_StatusBar(false);

		
		if (m_pBrowser2)
		{
			const HRESULT hr = m_pBrowser2->QueryInterface(IID_IWebBrowserApp,(void**)&m_pIEApp);
			
			m_pIEApp->put_MenuBar(false);
			m_pIEApp->put_ToolBar(false);
			m_pIEApp->put_StatusBar(false);
		}
		
	
	}
		
	return bResult;
}

short WebLinkCtrl::PopIE(LPCTSTR surl) 
{
	// TODO: Add your dispatch handler code here
	CString strurl;
	strurl.Format("%s", surl);
	ShellExecute(NULL, "open", _T("C:\\Program Files\\Internet Explorer\\iexplore.exe"), strurl, NULL,SW_SHOW|SW_SHOWMAXIMIZED);
	/*                     C:\\Program Files\\Internet Explorer
	CoInitialize(NULL);

	IWebBrowser2* pWeBrowser;
	HRESULT hr;
	//hr = ::CoCreateInstance(CLSID_InternetExplorer, NULL, CLSCTX_LOCAL_SERVER, IID_IWebBrowser2, (void**)&pWeBrowser); 
	hr = ::CoCreateInstance(CLSID_InternetExplorer, NULL, CLSCTX_SERVER, IID_IWebBrowser2, (void**)&pWeBrowser); 

	pWeBrowser->put_MenuBar(VARIANT_FALSE);
	pWeBrowser->put_AddressBar(VARIANT_FALSE);
	pWeBrowser->put_ToolBar(VARIANT_FALSE);
	pWeBrowser->put_StatusBar(VARIANT_FALSE);
	pWeBrowser->put_Resizable(VARIANT_TRUE);

	pWeBrowser->put_Left(100);
	pWeBrowser->put_Top(100);
	pWeBrowser->put_Width(1380);
	pWeBrowser->put_Height(780);

	VARIANT vtFlag, vtTarget, vtPostData, vtHeader;
	::VariantInit(&vtFlag);
	::VariantInit(&vtTarget);
	::VariantInit(&vtPostData);
	::VariantInit(&vtHeader);

	pWeBrowser->put_Visible(VARIANT_TRUE);

	CString strurl;
	strurl.Format("%s", surl);
	hr = pWeBrowser->Navigate(strurl.AllocSysString(), &vtFlag, &vtTarget, &vtPostData, &vtHeader);

	pWeBrowser->Release();
	CoUninitialize();
	*/

	return 0;
}


LRESULT WebLinkCtrl::OnBrowser( WPARAM wParam, LPARAM lParam )
{
	TRY
	{
	//	if(m_pIEApp)
		{
			CString url, slog;
			
			url = m_url;
			HRESULT hr = 0;
			VARIANT vEmpty;
			VARIANT vHeader;
			VariantInit(&vEmpty);
			
			CString strHeader;
			strHeader = "IBK투자증권";
			vHeader.vt = VT_BSTR;
			vHeader.bstrVal = strHeader.AllocSysString();
			
			BSTR bstrURL = url.AllocSysString();
			
			m_pBrowser2->put_Resizable(TRUE);
			
			m_pBrowser2->put_Width(m_width);
			m_pBrowser2->put_Height(m_height);
			
			
			CString sBlank = "about:blank";
			BSTR bstrBlank = sBlank.AllocSysString();
			hr = m_pBrowser2->Navigate(bstrBlank, &vEmpty, &vEmpty, &vEmpty, &vHeader);

		//	if (SUCCEEDED(hr))
			{
				m_pIEApp->put_MenuBar(false);
				m_pIEApp->put_ToolBar(false);
				m_pIEApp->put_StatusBar(false);
				
				CRect rc;
				
				GetParent()->GetParent()->GetClientRect(rc);
				
				ClientToScreen(rc);
				
				m_pIEApp->put_Top(100);
				m_pIEApp->put_Left(rc.left + 200);
				
				m_pIEApp->put_Width(1380);
				m_pIEApp->put_Height(680);
				
				HWND pWB;
				m_pBrowser2->get_HWND((long*)&pWB);
				CWnd *pwnd = CWnd::FromHandle(pWB); 
				
				m_pIEApp->put_Visible(true);
				
				pwnd->BringWindowToTop();
				hr = m_pBrowser2->Navigate(bstrURL, &vEmpty, &vEmpty, &vEmpty, &vHeader);
			}
			
		}
	}
	CATCH (CException, e)
	{
		CString errMsg;
		errMsg.Format("%ld", GetLastError());
	}
	END_CATCH;	
	return 0;
}

short WebLinkCtrl::PopUrl(LPCTSTR surl) 
{
	// TODO: Add your dispatch handler code here
	CString url;

	m_url.Format("%s", surl);
	m_parent->SetWindowPos(NULL, 0, 0, m_width, m_height, SWP_NOMOVE | SWP_NOZORDER);
//	ResizeToFitWindow();
//	PostMessage(WM_BROWSER,0,0);
	SetTimer(9898, 1000, NULL);
	return 0;
}

void WebLinkCtrl::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	KillTimer(nIDEvent);
	PostMessage(WM_BROWSER,0,0);	
	m_parent->GetParent()->PostMessage(WM_CLOSE);
	CWnd::OnTimer(nIDEvent);
}
