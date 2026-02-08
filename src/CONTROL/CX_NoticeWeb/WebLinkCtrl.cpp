/************************************************************************/
/* FILENAME:	WebLinkCtrl.cpp                                         */
/* DESCRIPTION: IBK투자증권 MAC: 웹링크용 Control인 WebLinkCtrl 소스파일*/
/* WRITE:		Macho @2008-06-04                                       */
/* UPDATE:                                                              */
/*                                                                      */
/*                                                                      */
/************************************************************************/

#include "stdafx.h"
#include "CX_WebLink.h"
#include "WebLinkCtrl.h"
#include "mshtml.h"
#include <winsock2.h>
#include <comutil.h>
#include <Psapi.h>

#include "../../h/axisfire.h"

#pragma comment(lib, "psapi.lib")
#pragma comment (lib, "ws2_32.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//** 내부 생성 콘트롤용 ControlID 정의
const UINT constWebLinkCtrlID		= 20900;
const UINT constWebBrowserCtrlID	=  1000;
struct _web_mid
{
	char	gubn[1];	        // 구분
	char	scno[4];			// 화면번호
};
#define sz_WEBMID		sizeof(struct _web_mid) 

struct	_web_mod	{
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
//** Helper Class 
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


/////////////////////////////////////////////////////////////////////////////
// WebLinkCtrl
[[gsl::suppress(26409)]]
IMPLEMENT_DYNCREATE(WebLinkCtrl, CWnd)


//** register this control
void WebLinkCtrl::RegisterClass()
{
	m_className = AfxRegisterWndClass(0);
}

//** constructor of this class
WebLinkCtrl::WebLinkCtrl()
{
	//** set enable OLE automation
	EnableAutomation();

	//** initialization member variables
	m_baseURL = m_userID = m_itemCode = m_sMapName = _T("");
	m_ctrl = nullptr;
	m_baseURL = "http://www.ibks.com/LoadService.jsp";

	m_bNavigate2 = TRUE;
	m_hInst = nullptr;
	m_bBrowser = false;
	m_bCertLogin = FALSE;
	//** register this control
	RegisterClass();
}

//** destructor of this class
WebLinkCtrl::~WebLinkCtrl()
{
	if (m_hInst)
	{
		FreeLibrary(m_hInst);
	}
}

//** virtual function OnFinalRelease()
void WebLinkCtrl::OnFinalRelease()
{
	//** do not anything!
	CWnd::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(WebLinkCtrl, CWnd)
	//{{AFX_MSG_MAP(WebLinkCtrl)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnUser)
	ON_MESSAGE(WM_USER + 1234, OnMessage)
	ON_MESSAGE(WM_BROWSER, OnBrowser)
	ON_WM_CREATE()
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(WebLinkCtrl, CWnd)
	//{{AFX_DISPATCH_MAP(WebLinkCtrl)
	DISP_PROPERTY_EX(WebLinkCtrl, "BaseURL", GetBaseURL, SetBaseURL, VT_BSTR)
	DISP_PROPERTY_EX(WebLinkCtrl, "ItemCode", GetItemCode, SetItemCode, VT_BSTR)
	DISP_PROPERTY_EX(WebLinkCtrl, "UserID", GetUserID, SetUserID, VT_BSTR)
	DISP_PROPERTY_EX(WebLinkCtrl, "Home", GetHome, SetHome, VT_BSTR)
	DISP_PROPERTY_EX(WebLinkCtrl, "Menu", GetMenu, SetMenu, VT_BSTR)
	//DISP_PROPERTY_EX(WebLinkCtrl, "Width", GetWidth, SetNotSupported, VT_I2)
	//DISP_PROPERTY_EX(WebLinkCtrl, "Height", GetHeight, SetNotSupported, VT_I2)
	DISP_PROPERTY_EX(WebLinkCtrl, "Width", GetWidth, SetNotSupported, VT_I2)
	DISP_PROPERTY_EX(WebLinkCtrl, "Height", GetHeight, SetNotSupported, VT_I2)
	DISP_FUNCTION(WebLinkCtrl, "ResizeToFitWindow", ResizeToFitWindow, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(WebLinkCtrl, "Navigate", Navigate, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(WebLinkCtrl, "GetCaption", GetCaption, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(WebLinkCtrl, "Navigate2", Navigate2, VT_EMPTY, VTS_BSTR VTS_BOOL)
	DISP_FUNCTION(WebLinkCtrl, "NavigateFRmap",  NavigateFRmap, VT_EMPTY, VTS_BSTR VTS_I2)
	//}}AFX_DISPATCH_MAP
	
	DISP_FUNCTION_ID(WebLinkCtrl, "SetWidth", dispidSetWidth, SetWidth, VT_EMPTY, VTS_I4)
	DISP_FUNCTION_ID(WebLinkCtrl, "SetHeight", dispidSetHeight, SetHeight, VT_EMPTY, VTS_I4)
END_DISPATCH_MAP()

// {EC249151-7342-4934-830F-416CA10EB057}
static const IID IID_IWebLinkCtrl =
{ 0xec249151, 0x7342, 0x4934, { 0x83, 0xf, 0x41, 0x6c, 0xa1, 0xe, 0xb0, 0x57 } };

BEGIN_INTERFACE_MAP(WebLinkCtrl, CWnd)
	INTERFACE_PART(WebLinkCtrl, IID_IWebLinkCtrl, Dispatch)
END_INTERFACE_MAP()

// {6394EA04-8202-49C4-8CCD-0AFF3C97B8CA}
IMPLEMENT_OLECREATE(WebLinkCtrl, "CXWebLink.WebLinkCtrl", 
					0x6394EA04, 0x8202, 0x49C4, 0x8C, 
					0xCD, 0x0A, 0xFF, 0x3C, 0x97, 0xB8, 0xCA)


BEGIN_EVENTSINK_MAP(WebLinkCtrl, CWnd)
    //{{AFX_EVENTSINK_MAP(CWebTestDlg)
	ON_EVENT(WebLinkCtrl, constWebBrowserCtrlID, 250 /* BeforeNavigate2 */, OnBeforeNavigate2WebCtrl, VTS_DISPATCH VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PBOOL)
	ON_EVENT(WebLinkCtrl, constWebBrowserCtrlID, 259 /* DocumentComplete */, OnDocumentComplete, VTS_DISPATCH VTS_PVARIANT) 
	ON_EVENT(WebLinkCtrl, constWebBrowserCtrlID, 271 /* NavigateError */, OnNavigateError, VTS_DISPATCH VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PBOOL) 
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

/////////////////////////////////////////////////////////////////////////////
// WebLinkCtrl message handlers

//** resize 
void WebLinkCtrl::ResizeToFitWindow() 
{
	CWnd* pParent = GetParent();
	if (pParent == NULL)
		return;

	CRect rc;
	pParent->GetClientRect(rc);

	if (!m_bNavigate2)
		MoveWindow(rc, TRUE);

	CRect src;
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

BSTR WebLinkCtrl::GetBaseURL() 
{
	CString strResult;
 	strResult = m_baseURL;

	return strResult.AllocSysString();
}

void WebLinkCtrl::SetBaseURL(LPCTSTR lpszNewValue) 
{
	m_baseURL = lpszNewValue;
}

BSTR WebLinkCtrl::GetItemCode() 
{
	CString strResult;
	strResult = m_itemCode;

	return strResult.AllocSysString();
}

void WebLinkCtrl::SetItemCode(LPCTSTR lpszNewValue) 
{
	m_itemCode = lpszNewValue;
	if (m_itemCode.GetLength() > 6)
		m_itemCode = m_itemCode.Left(6);
}

BSTR WebLinkCtrl::GetUserID() 
{
	CString strResult;
	strResult = m_userID;

	return strResult.AllocSysString();
}

void WebLinkCtrl::SetUserID(LPCTSTR lpszNewValue) 
{
	m_userID = lpszNewValue;
	m_userID.TrimLeft();	
	m_userID.TrimRight();
}

BSTR WebLinkCtrl::GetHome() 
{
	CString strResult;
	strResult = m_home;

	return strResult.AllocSysString();
}

void WebLinkCtrl::SetHome(LPCTSTR lpszNewValue) 
{
	m_home = lpszNewValue;

	if (m_home[m_home.GetLength() - 1] != '\\')
		m_home += '\\';

	CString ini;

	ini.Format("%stab\\axis.ini", m_home);

	CString home(ProfileStr("WebLink", "home", "", ini).value);

	if (!home.IsEmpty())
		m_baseURL = home;
}

BSTR WebLinkCtrl::GetMenu() 
{
	CString strResult;
	strResult = m_menu;

	return strResult.AllocSysString();
}

void WebLinkCtrl::SetMenu(LPCTSTR lpszNewValue) 
{
	m_menu = lpszNewValue;
}

BOOL WebLinkCtrl::Create(CWnd* parent, _param* param)
{
	m_parent = parent;
	SetParam(param);
	m_param  = param;
	m_sRoot  = Variant(homeCC, "");
	m_sPath.Format("%s\\%s\\%s", m_sRoot, "TAB", "NOT_FOUND.HTM");
	m_sMapName = (LPCTSTR)m_parent->SendMessage(WM_USER, MAKEWPARAM(mapDLL, 1));

	const	BOOL bResult = CWnd::Create(m_className, _T(""), WS_VISIBLE | WS_CHILD, CRect(0, 0, 0, 0), 
		m_parent, constWebLinkCtrlID, NULL);

	if (bResult)
	{
		m_ctrl = std::make_unique<CWebBrowserCtrl>();
		if (!m_ctrl->Create(_T("WebBrowser2"), WS_VISIBLE | WS_CHILD, CRect(-2, -2, 1, 1), this, constWebBrowserCtrlID))
			m_ctrl.reset();
		ResizeToFitWindow();

		m_ctrl->SetSilent(TRUE);
	}

	if (SUCCEEDED(CoInitialize(NULL)))
	{
		const	HRESULT hrco = ::CoCreateInstance(CLSID_InternetExplorer, NULL, CLSCTX_SERVER, 
			IID_IWebBrowser2, (void**)&pBrowser2);
		
		if (FAILED(hrco))
			return FALSE;

		CString url = "www.ibks.com";
		if (pBrowser2)
		{
			const	HRESULT hr = pBrowser2->QueryInterface(IID_IWebBrowserApp,(void**)&m_pIEApp);

			m_pIEApp->put_MenuBar(false);
			m_pIEApp->put_ToolBar(false);
			m_pIEApp->put_StatusBar(false);
		}
	}

	GetLocalIP();	//2013.08.12 KSJ IP구하기
	GetMAC();

	return bResult;
}

void WebLinkCtrl::OnDestroy() 
{
	if (m_ctrl)
	{
		m_ctrl.reset();

		if (m_bBrowser)
		{
			HWND pWB;

			pBrowser2->get_HWND((long*)&pWB);
			::PostMessage(pWB, WM_CLOSE, 0, 0);
		}
	}
	
	CWnd::OnDestroy();	
}

void WebLinkCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	ResizeToFitWindow();
}


void WebLinkCtrl::GetInformation()
{
	const char* iniName = "WebInfo.ini";

	CString path;
	path.Format("%stab\\%s", m_home, iniName);

	m_width = ProfileInt(m_menu, "Width", 0, path).value;
	if (!m_width)
		m_width = ProfileInt("Common", "DefaultWidth", 0, path).value;

	m_height = ProfileInt(m_menu, "Height", 0, path).value;
	if (!m_height)
		m_height = ProfileInt("Common", "DefaultHeight", 0, path).value;

	m_type = ProfileStr(m_menu, "Type", "", path).value;
	//m_url  = ProfileStr(m_menu, "URL", "", path).value;
	m_description = ProfileStr(m_menu, "Description", "", path).value;
}

CString WebLinkCtrl::GetURL()
{
	//예외처리 (LoadService.jsp 안타도록 하려면 주석을 해제하세요.
	CString temp(m_url);
	temp.MakeUpper();

	// dhttp 방식으로 변경
	//if (temp.Find("KRX.CO.KR") > 0)	return m_url;

	// dhttp 로 시작하면 바로 띄워라!
	if (temp.GetLength()>5)
	{
		if (temp.Mid(0, 5)=="DHTTP")
			return m_url.Mid(1);
	}

	CString url(m_baseURL);

	url += "?";
	url += "url=";

	//**url += URLEncode(m_url);

	url += m_url;

	url += "&auth=";

	url += GetAuthParam();
	
	return url;
}

short WebLinkCtrl::GetWidth() 
{
	return m_width;
}

short WebLinkCtrl::GetHeight() 
{
	return m_height;
}

BSTR WebLinkCtrl::GetCaption() 
{
	CString strResult;

	strResult = m_description;

	return strResult.AllocSysString();
}

void WebLinkCtrl::OnBeforeNavigate2WebCtrl(LPDISPATCH pDisp, VARIANT FAR* URL, VARIANT FAR* Flags, VARIANT FAR* TargetFrameName, VARIANT FAR* PostData, VARIANT FAR* Headers, BOOL FAR* Cancel)
{
	CString url(URL->bstrVal);

	//const char* trCommand = "bestez:tr";
	const char* trCommand = "HTS:TR";
	const char* codCommand = "HTS:CD";

	url.MakeUpper();

	int	find = url.Find(trCommand);
	if (find > -1)
	{
		*Cancel = TRUE;

		CString string;                                                   
		CString map = url.Mid(find + lstrlen(trCommand), 4);

		//CString mapOriginal = FindMap(map);

		if (map.IsEmpty())
			return;

		if (map.GetLength() == 4 && atoi(map) > 0)
		{
			map.Insert(0, "IB");
			map += "00";
		}

		string = map;

		find = url.Find(codCommand);
		
		if (find > -1)
		{
			CString code = url.Mid(find + lstrlen(trCommand), 6);
			
			CString strCode;
			strCode.Format("1301\t%s",code);

			string.Format("%s/t0/S/d %s",map,strCode);
		}
		
		m_parent->SendMessage(WM_USER, MAKEWPARAM(viewDLL, typeVIEW), (LPARAM)(LPCTSTR)string);

		return;
	}

	find = url.Find(codCommand);
	
	if (find > -1)
	{
		*Cancel = TRUE;

		CString code = url.Mid(find + lstrlen(trCommand), 6);
		
		CString strCode;
		strCode.Format("1301\t%s",code);
		
		Variant(triggerCC,strCode);

		return;
	}
}

CString WebLinkCtrl::FindMap(LPCTSTR map)
{
	CFile	file;
	CString menuFilePath;

	menuFilePath.Format("%stab\\axis.menu", m_home);
	if (!file.Open(menuFilePath, CFile::modeRead | CFile::shareDenyNone))
		return "";

	int	ival = 0;
	const	char*	mapNamePrefix = "IB"; // only IBK MAC
	const	int	len = (int)file.GetLength();
	std::unique_ptr<char[]> buffer = std::make_unique<char[]>(len + 1);

	ival = file.Read(buffer.get(), len);
	file.Close();

	if (ival != len)
	{
		buffer.reset();
		return "";
	}

	CString s = CString(buffer.get());
	buffer.reset();

	ival = s.Find(CString(map) + '\t');
	if (ival < 0) 
		return "";

	for (int ii = ival; ii >= 0; ii--)
	{
		if (s[ii] == mapNamePrefix[0] && s[ii+1] == mapNamePrefix[1])
		{
			return s.Mid(ii, 8);
		}
	}

	return "";
}

void WebLinkCtrl::OnTimer(UINT nIDEvent) 
{
	KillTimer(nIDEvent);

	/*
	if (m_parent->GetParent())
		m_parent->GetParent()->SetWindowPos(NULL, 0, 0, m_width, m_height, SWP_NOMOVE | SWP_NOZORDER);
	*/

	ResizeToFitWindow();
	CWnd::OnTimer(nIDEvent);
}

void WebLinkCtrl::Navigate() 
{
	m_bNavigate2 = false;

	if (m_menu.IsEmpty()) 
		return;
	if (m_home.IsEmpty()) 
		return;

	GetInformation();
	m_parent->SetWindowPos(NULL, 0, 0, m_width, m_height, SWP_NOMOVE | SWP_NOZORDER);
	ResizeToFitWindow();
	struct	_web_mid Mid;
	FillMemory(&Mid, sz_WEBMID, ' ');

	CopyMemory(Mid.gubn, "Q", 1);
	CopyMemory(Mid.scno, m_menu, m_menu.GetLength());

	CString strData((char*)&Mid, sz_WEBMID);
	SendTR(_T("PMHOWURL"), NULL, strData, (BYTE)1);
}

//2012.10.30 KSJ 로그인서비스를 태우는 url이면 bLogin = TRUE
void WebLinkCtrl::Navigate2(LPCTSTR sUrl, bool bLogin) 
{
	// TODO: Add your dispatch handler code here
	CString stmp;
	stmp.Format("\r\n[CX_WEBLINK] Navigate2 bLogin=[%d]\r\n", bLogin);
	OutputDebugString(stmp);
	
	m_userID = Variant(userCC, "");
	m_bNavigate2 = true;
	m_bBrowser = true;

	CString url;

	if (bLogin)
		url.Format("http://www.ibks.com/LoadService.jsp?url=%s&auth=%s", sUrl, GetAuthParam());
	else
	{
		url = sUrl;
		if (url.Find("?") == -1)	//2014.07.21 KSJ 
			url += "?";
	}

	if (url.Find("ibks.com") != -1)	//2014.07.17 KSJ 우리쪽 url만 바꾸자..
		url += SetNoParam();	//2014.07.17 KSJ 방화벽,키보드,aos 해제

	if (IsDev())
	{
		//url.Replace("ibks.com","ibkis.net");
		//url.Replace("www.ibks.com","test.ibks.com");  //기존
	}

	if (url.Find("http://") > -1)
		url.Replace("http://","https://");
	// --------------------------------------------------------------------
	
	m_ctrl->Navigate(url, NULL, NULL, NULL, NULL);
	
	stmp.Format("\r\n[CX_WEBLINK] Navigate2=[%s]\r\n", url);
	OutputDebugString(stmp);
}

CString WebLinkCtrl::URLEncode(const char* lpszURL)
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

// String code_YN = request.getParameter("code_YN"); /* 종목코드 Script 삽입여부 */
// String keyB_YN = request.getParameter("keyB_YN"); /* 키보드보안 Script 삽입여부 */
// String sign_YN = request.getParameter("sign_YN"); /* SignKorea Script 삽입여부 */
// String nPro_YN = request.getParameter("nPro_YN"); /* 개인방화벽 Script 삽입여부 */
// String aos_YN = request.getParameter("aos_YN"); /* AOS 메모리보호 Script 삽입여부 */ 
CString WebLinkCtrl::SetNoParam()
{
	CString strReturn;
	strReturn.Format("&keyB_YN=Y&nPro_YN=N&aos_YN=N");

	//2017.01.02 KSJ 직원용이고 8712이면 ID를 넣어준다.
	if (!m_sMapName.Compare("IB871200") && IsNumber(m_userID))
	{
		strReturn += "&id=" + m_userID;
	}

	return strReturn;
}

BOOL WebLinkCtrl::IsNumber(CString str)
{
	BOOL	bRet = TRUE;
	const	int	nLen = str.GetLength();

	for (int ii = 0 ;ii < nLen ;ii++)
	{
		if ( isdigit(str.GetAt(ii)) == FALSE)
		{
			bRet = FALSE;
			break;
		}
	}

	return bRet;
}

CString WebLinkCtrl::GetAuthParam()
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
	//	if(m_time.IsEmpty())
	//		m_time.Format("%04d%02d%02d%02d%02d%02d", time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
		
		auth += URLEncode(m_ipAddr);
		auth += "&mac=" + URLEncode(m_MacAddr);
		auth += "&dt=" + URLEncode(strTime1);
	//	auth += "&dt=" + URLEncode(m_time);
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
	}
	CATCH (CMemoryException, e)
	{
		
	}
	END_CATCH
	
	return auth;
}

CString WebLinkCtrl::HTSEncode(const char* lpszSource, const char* key)
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

	AfxMessageBox(result);
	AfxMessageBox(buffer);
	**/

	buffer.reset();
	return result;
}
//////////////////////////////////////////////////////////////////////////
// IBK Main Program's WM_USER definition
// Warning! IBK only.
#define MMSG_GETPASSWORD		0x01
#define MMSG_GETCERTPASSWORD	0x02

CString WebLinkCtrl::GetUserPassword()
{
	//현재는 운용과 개발 두가지 모드만 존재하므로 해당 메인혹은 단위화면의 캡션으로 개발용인지 판단하도록 처리함
	//차후 모드가 어려가지로 분기되면 해당 로직도 변경 되어야함
	CWinApp* app = AfxGetApp();
	CString title;
	CString file;
	CString user;
	m_pMainFrame->GetWindowText(title);
	/*
	user = Variant(nameCC,"");

	file.Format("%s\\%s\\%s\\%s", m_sRoot, "user", user, "axisensetup.ini");

	const	int	titleChange = GetPrivateProfileInt("SCREEN", "TITLECHANGE", 0, file);
	CWnd*	wnd = nullptr;

	CString s;
	//LPCSTR lpszClassName = "AfxMDIFrame42";
	LPCSTR lpszClassName = NULL;

	if (titleChange)
	{
		CString userTitle,titleType1,titleType2,titleType3;
		char	buff[1024]{};

		GetPrivateProfileString("SCREEN","USERTITLE","",buff,sizeof(buff),file);
		userTitle = buff;

		titleType1.Format("%s(직원용)",userTitle);
		titleType2.Format("%s - 개발용",userTitle);
		titleType3.Format("%s(직원용) - 개발용",userTitle);

		if (title == userTitle)
			wnd = FindWindow(lpszClassName,userTitle);
		else if (title == titleType1)
			wnd = FindWindow(lpszClassName,titleType1);
		else if (title == titleType2)
			wnd = FindWindow(lpszClassName,titleType2);
		else if (title == titleType3)
			wnd = FindWindow(lpszClassName,titleType3);
		if (!wnd)  //SDI인경우 메인폼 접근이 안되서 편법이용
		{
			wnd = FindWindow(lpszClassName,userTitle);
			if (!wnd) wnd = FindWindow(NULL,titleType1);
			if (!wnd) wnd = FindWindow(NULL,titleType2);		
			if (!wnd) wnd = FindWindow(NULL,titleType3);		
		}
	}
	else
	{
		if (title == ("IBK hot Trading"))
			wnd = FindWindow(lpszClassName,"IBK hot Trading");
		else if (title == "[직원]IBK hot Trading")
			wnd = FindWindow(lpszClassName,"[직원]IBK hot Trading");
		else if (title == "IBK개발")
			wnd = FindWindow(lpszClassName,"IBK개발");
		else if (title == "[직원]IBK개발")
			wnd = FindWindow(lpszClassName,"[직원]IBK개발");
		if (!wnd)  //SDI인경우 메인폼 접근이 안되서 편법이용
		{
			wnd = FindWindow(lpszClassName,"IBK hot Trading");
			if (!wnd) wnd = FindWindow(NULL,"[직원]IBK hot Trading");
			if (!wnd) wnd = FindWindow(NULL,"IBK개발");		
			if (!wnd) wnd = FindWindow(NULL,"[직원]IBK개발");		
		}
	}
	*/

	if (m_pMainFrame)
		return CString((LPCTSTR)m_pMainFrame->SendMessage(WM_USER, MMSG_GETPASSWORD));

	return "";
}

CString WebLinkCtrl::GetCertPassword()
{
	CWinApp* app = AfxGetApp();
	CString title;
	m_pMainFrame->GetWindowText(title);
	/*
	CWnd*	wnd = nullptr;

	if (title == ("IBK hot Trading"))
		wnd = FindWindow(NULL,"IBK hot Trading");
	else if (title == "[직원]IBK hot Trading")
		wnd = FindWindow(NULL,"[직원]IBK hot Trading");
	else if (title == "IBK개발")
		wnd = FindWindow(NULL,"IBK개발");
	else if (title == "[직원]IBK개발")
		wnd = FindWindow(NULL,"[직원]IBK개발");

	if (!wnd)  //SDI인경우 메인폼 접근이 안되서 편법이용
	{
		wnd = FindWindow(NULL,"IBK hot Trading");
		if (!wnd) wnd = FindWindow(NULL,"[직원]IBK hot Trading");
		if (!wnd) wnd = FindWindow(NULL,"IBK개발");		
		if (!wnd) wnd = FindWindow(NULL,"[직원]IBK개발");	
	}
*/
	if (m_pMainFrame)
		return CString((LPCTSTR)m_pMainFrame->SendMessage(WM_USER, MMSG_GETCERTPASSWORD));
	else
		return "";
	
	//return CString((LPCTSTR)wnd->SendMessage(WM_USER, MMSG_GETCERTPASSWORD));
}


LRESULT WebLinkCtrl::OnMessage(WPARAM wParam, LPARAM lParam)
{
	if (m_ctrl)
	{
		LPUNKNOWN lpU = m_ctrl->GetControlUnknown();
		IOleInPlaceActiveObject *pIO;
		const	HRESULT hr = lpU->QueryInterface(IID_IOleInPlaceActiveObject, (void **)&pIO);
		if (SUCCEEDED(hr))  //영문키 입력이 안되서 수정
		//if (hr)
		{
			LPMSG pMsg = (MSG*)lParam;
			pIO->TranslateAccelerator(pMsg);
			pIO->Release();
			return TRUE;
		}
	}
	return FALSE;
}

LRESULT WebLinkCtrl::OnUser(WPARAM wParam, LPARAM lParam)
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

			const char* iniName = "OnlyBrowser.ini";
			CString path;

			path.Format("%s\\tab\\%s", m_home, iniName);

			const	int	nMode = ProfileInt("MODE", "BrowserMode", 0, path).value;
			if (m_type != "BROWSER" && nMode == 1)
			{
				m_type = "BROWSER";
				m_url = GetURL();
			}
			if (m_type.CompareNoCase("BROWSER") == 0)
			{
	// 			if (IsDev())
	// 			{
	// 				//url.Replace("ibks.com","ibkis.net");
	// 				m_url.Replace("www.ibks.com","na.ibks.com");
	// 			}

				// --------------------------------------------------------------------
				// BROWSER모드로 띄울때도 인증이 필요한 케이스가 발생하여
				// 아래와 같이 ${authval} 플래그가 존재하면 인증정보를 삽입하도록 수정
				//                                             - by warship 2010.04.26

				//m_url = GetURL();

				CString url = m_url;
				if (url.Find("${authval}") > 0)
					url.Replace("${authval}", (LPCSTR)GetAuthParam());
				// --------------------------------------------------------------------

				if (url.Find("ibks.com") != -1)	//2014.07.17 KSJ 우리쪽 url만 바꾸자..
				{
					if (url.Find("?") == -1)	//2014.07.21 KSJ 
						url += "?";

					url += SetNoParam();	//2014.07.17 KSJ 방화벽,키보드,aos 해제
				}

				if (url.Find("ibks.com") > -1)
					url.Replace("http://", "https://");

				m_finalurl = url;

			//	openhtml();
				if (GetOSversion() == 11)
				{
					openhtml();
				}
				else
					PostMessage(WM_BROWSER, 0, (LPARAM)url.GetString());

				m_parent->GetParent()->PostMessage(WM_CLOSE);
			}
			else
			{
				CString url = GetURL();
				if (IsDev())
				{
					//url.Replace("ibks.com","ibkis.net");
					url.Replace("www.ibks.com", "test.ibks.com");
				}

				if (url.Find("ibks.com") != -1)	//2014.07.17 KSJ 우리쪽 url만 바꾸자..
				{
					if (url.Find("?") == -1)	//2014.07.21 KSJ 
						url += "?";

					url += SetNoParam();	//2014.07.17 KSJ 방화벽,키보드,aos 해제
				}

				/*(
				m_parent->SetWindowPos(NULL, 0, 0, m_width, m_height, SWP_NOMOVE | SWP_NOZORDER);
				ResizeToFitWindow();
				*/
				m_ctrl->Navigate(url, NULL, NULL, NULL, NULL);

				//SetTimer(1, 10, NULL);
			}
		}
		break;

	case DLL_OUB:	// NOT USED. Use DLL_OUBx Only !!
		{
			key = HIBYTE(LOWORD(wParam));
			nLen = HIWORD(wParam);
			struct _web_mod* outweb = (_web_mod*)lParam;

			//m_width = atoi(CString(outweb->widt, sizeof(outweb->widt)));
			//m_height = atoi(CString(outweb->heig, sizeof(outweb->heig)));

			m_type = CString(outweb->type, sizeof(outweb->type));
			m_url = CString(outweb->url, sizeof(outweb->url));
			m_strUrl = m_url;
			m_sUrl = m_url;

			m_description = "";//CString(outweb->dumy, sizeof(outweb->dumy));
			m_url.TrimLeft(); m_url.TrimRight();
			m_url.Remove(0);

			/*
			CFileFind finder;
			CString filename;
			filename.Format("%s\\%s\\%s", m_sRoot, "exe", "ignoreWMAP");

			if (finder.FindFile(filename))
			{
				if (m_type.CompareNoCase("BROWSER") != 0)
				{
					m_type = "BROWSER";

					m_url = GetURL();
				}
			}
			*/

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

			const char* iniName = "OnlyBrowser.ini";

			CString path;
			path.Format("%s\\tab\\%s", m_home, iniName);

			const	int	nMode = ProfileInt("MODE", "BrowserMode", 0, path).value;

			if (m_type != "BROWSER" && nMode == 1)
			{
				m_type = "BROWSER";

				m_url = GetURL();
			}
			if (m_type.CompareNoCase("BROWSER") == 0)
			{
				// 			if (IsDev())
				// 			{
				// 				//url.Replace("ibks.com","ibkis.net");
				// 				m_url.Replace("www.ibks.com","na.ibks.com");
				// 			}

							// --------------------------------------------------------------------
							// BROWSER모드로 띄울때도 인증이 필요한 케이스가 발생하여
							// 아래와 같이 ${authval} 플래그가 존재하면 인증정보를 삽입하도록 수정
							//                                             - by warship 2010.04.26

							//m_url = GetURL();

				CString url = m_url;
				if (url.Find("${authval}") > 0)
					url.Replace("${authval}", (LPCSTR)GetAuthParam());
				// --------------------------------------------------------------------

				if (url.Find("ibks.com") != -1)	//2014.07.17 KSJ 우리쪽 url만 바꾸자..
				{
					if (url.Find("?") == -1)	//2014.07.21 KSJ 
						url += "?";

					url += SetNoParam();	//2014.07.17 KSJ 방화벽,키보드,aos 해제
				}

				if (url.Find("ibks.com") > -1)
					url.Replace("http://", "https://");

				m_finalurl = url;

				//	openhtml();
				if (GetOSversion() == 11)
				{
					openhtml();
				}
				else
					PostMessage(WM_BROWSER, 0, (LPARAM)url.operator LPCTSTR());

				m_parent->GetParent()->PostMessage(WM_CLOSE);
			}
			else
			{
				CString url = GetURL();
				if (IsDev())
				{
					//url.Replace("ibks.com","ibkis.net");
					url.Replace("www.ibks.com", "test.ibks.com");
				}

				if (url.Find("ibks.com") != -1)	//2014.07.17 KSJ 우리쪽 url만 바꾸자..
				{
					if (url.Find("?") == -1)	//2014.07.21 KSJ 
						url += "?";

					url += SetNoParam();	//2014.07.17 KSJ 방화벽,키보드,aos 해제
				}

				/*(
				m_parent->SetWindowPos(NULL, 0, 0, m_width, m_height, SWP_NOMOVE | SWP_NOZORDER);
				ResizeToFitWindow();
				*/
				
				m_ctrl->Navigate(url, NULL, NULL, NULL, NULL);

				//SetTimer(1, 10, NULL);
			}
		}
		break;
	}
	return FALSE;
}
#include <Lm.h>
#pragma comment(lib, "netapi32.lib")

int WebLinkCtrl::GetOSversion()
{
	HMODULE hDll = LoadLibrary(("Ntdll.dll"));
	if (hDll == nullptr)
		return 0;

	typedef NTSTATUS(CALLBACK* RTLGETVERSION) (PRTL_OSVERSIONINFOW lpVersionInformation);
	RTLGETVERSION pRtlGetVersion;
	pRtlGetVersion = (RTLGETVERSION)GetProcAddress(hDll, "RtlGetVersion");
	if (pRtlGetVersion)
	{
		RTL_OSVERSIONINFOW ovi = { 0 };
		ovi.dwOSVersionInfoSize = sizeof(ovi);
		const	NTSTATUS ntStatus = pRtlGetVersion(&ovi);
		if (ntStatus == 0)
		{
			if (ovi.dwBuildNumber >= 22000)
				return 11;
			else if (ovi.dwMajorVersion >= 10)
				return 10;
			else if (ovi.dwBuildNumber >= 9200 && ovi.dwBuildNumber <= 9600)
				return 8;
			return 7;
		}
    }
	
	return 0;
}

int WebLinkCtrl::GetProcssNameByPID(DWORD dwPID, char *buff, int size)
{
    int nBinNameLen = 0;
    HANDLE hProc = NULL;
	
    if ( (hProc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwPID)) != NULL)
    {
        nBinNameLen = ::GetModuleBaseName(hProc, NULL, buff, size);
        CloseHandle (hProc);
    }
	
    return nBinNameLen;
}


void WebLinkCtrl::OnNavigateError(LPDISPATCH lpDisp, VARIANT FAR* URL, VARIANT FAR* TargetFrameName, VARIANT FAR* StatusCode, BOOL FAR* Cancel)
{
	const	int	val = StatusCode->iVal;
	CString statuscode; statuscode.Format("%d", val);
	CString url;

	url = m_sPath;
	if (val!=200)
	{
		//AfxMessageBox("HTTP404");
		m_ctrl->Navigate(url, NULL, NULL, NULL, NULL);
	}
}

void WebLinkCtrl::OnDocumentComplete(LPDISPATCH lpDisp, VARIANT FAR* URL)
{
	//int status = m_ctrl->GetReadyState();
	
}

CString WebLinkCtrl::Variant(int comm, CString data)
{
	CString retvalue;
	char* dta = (char*)m_parent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, comm), (LPARAM)(LPCTSTR)data);

	if ((long)dta > 1)
		retvalue = dta;
	else
		return "";

	return retvalue;
}

BOOL WebLinkCtrl::IsDev()
{
	CString strdata, name;
	name = Variant(nameCC, "");

	int readL = 0;
	char readB[1024];
	CString userD;
	userD.Format("%s\\%s\\%s\\%s.ini", m_sRoot, "user", name, name);
	readL = GetPrivateProfileString("MODE", "DEV", "", readB, sizeof(readB), userD);

	if (readL <= 0)
		return 0;

	CString sDEV;
	sDEV.Format("%s", readB);
	if (sDEV == "1")
		return TRUE;
	else
		return FALSE;
	/*
	CWinApp* app = AfxGetApp();
	if (app==NULL) return FALSE;

	CString title;

	CWnd *wnd=NULL;
	if (m_pMainFrame ==NULL) return FALSE;

	m_pMainFrame->GetWindowText(title);

	if (title == ("IBK hot Trading"))
		wnd = FindWindow(NULL,"IBK hot Trading");
	else if (title == "[직원]IBK hot Trading")
	{
		wnd = FindWindow(NULL,"[직원]IBK hot Trading");
	}
	else if (title == "IBK개발")
		wnd = FindWindow(NULL,"IBK개발");
	else if (title == "[직원]IBK개발")
		wnd = FindWindow(NULL,"[직원]IBK개발");
	if (!wnd)  //SDI인경우 메인폼 접근이 안되서 편법이용
	{
		wnd = FindWindow(NULL,"IBK hot Trading");
		if (!wnd) wnd = FindWindow(NULL,"[직원]IBK hot Trading");
		if (!wnd) wnd = FindWindow(NULL,"IBK개발");		
		if (!wnd) wnd = FindWindow(NULL,"[직원]IBK개발");	
	}
	if (wnd)
	{
		wnd->GetWindowText(title);

		if (title.Find("개발")!=-1)
			return TRUE;
		else
			return FALSE;
	}

	return FALSE;
	*/
}

LRESULT WebLinkCtrl::SendTR(CString strName, BYTE type, CString strData, BYTE key)
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

	return m_parent->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, strData.GetLength()), (LPARAM)buff);
}

void WebLinkCtrl::SetParam(_param *pParam)
{
	m_Param.key = pParam->key;
	m_Param.name = CString(pParam->name, pParam->name.GetLength());
	m_Param.rect = CRect(pParam->rect.left, pParam->rect.top, pParam->rect.right, pParam->rect.bottom);
	m_Param.fonts = CString(pParam->fonts, pParam->fonts.GetLength());
	m_Param.point = pParam->point;
	m_Param.style = pParam->style;
	m_Param.options = CString(pParam->options, pParam->options.GetLength());
}

void WebLinkCtrl::GetLocalIP()
{
    //m_ipAddr = "172.17.2.74";
    //return;
	
    char szHostName[64] = {0};
	
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


void WebLinkCtrl::GetMAC()
{
	char	buff[20]{};
	CString file;
	file.Format("%s\\tab\\AXIS.INI", m_sRoot);
	GetPrivateProfileString("PCDATA", "sPhyMac", "", buff, sizeof(buff), file);
	m_MacAddr.Format("%s", buff);
	m_MacAddr.TrimRight();
	m_MacAddr.MakeLower();
	//CString file;
	//file.Format("%s\\exe\\CX_PCIdentity.dll", m_sRoot);

	//typedef char* (*GETMACADD)();
	//HMODULE hModule = LoadLibrary(file);
	//if (hModule)
	//{
	//	GETMACADD func = (GETMACADD)GetProcAddress(hModule, "axGetMac");
	//	if (func)
	//	{
	//		char* pdata = func();
	//		if (pdata)
	//		{
	//			m_MacAddr.Format("%s", pdata);
	//			m_MacAddr.MakeLower();
	//		}
	//	}
	//	else
	//	{
	//		OutputDebugString("[CX_WEBLINK] axGetMac load fail\r\n");
	//	}
	//}
	//else
	//{
	//	OutputDebugString("[CX_WEBLINK] LoadLibrary[cx_pcidentity]  load fail\r\n");
	//}

}

void WebLinkCtrl::openhtml()
{
	CString url;
	url = m_finalurl;
	
	BSTR bstrURL = url.AllocSysString();

	CString surl(m_baseURL);
	m_baseURL.Replace("http","https");

	bstrURL = m_baseURL.AllocSysString();

	CString sHeader = _T("Content-Type: application/x-www-form-urlencoded\r\n");
	CString stmp;

	CString spath;
//	spath = "test.html";
	spath.Format("%s\\%s\\%s", m_sRoot, "TAB", "edge.html");
	CFile fileB;

	if (!fileB.Open(spath, CFile::modeRead | CFile::shareDenyNone))
	{
		AfxMessageBox("no file path 1");
		return ;
	}
				
	const	int	len = (int)fileB.GetLength();
	std::unique_ptr<char[]> pdata = std::make_unique<char[]>(len+ 1);

	if (len != fileB.Read(pdata.get(), len))
	{
		fileB.Close();
		pdata.reset();
		return;
	}
	fileB.Close();

	CString strdata = CString(pdata.get());
	pdata.reset();

	//.Mid(0, 5)=="DHTTP"  ShellExecute(NULL, _T("open"), szFile, NULL,NULL, SW_SHOW);	
	if (m_bCertLogin)
		strdata.Replace("replace", m_sCert);
	else //if (m_strUrl.Mid(0, 5)=="DHTTP")
	{
		m_finalurl.Replace("dhttp", "http");
		ShellExecute(NULL, _T("open"), m_finalurl, NULL,NULL, SW_SHOW);	  //test
	//	ShellExecute(NULL, _T("open"), "msedge.exe", m_finalurl, NULL, SW_SHOW);
		return;
	}
//	else
//		strdata.Replace("replace", GetAuthParam());

	strdata.Replace("tmpurl", m_sUrl);

	CFile	file;
	CString	szFile;

	szFile.Format("%s\\%s\\%s", m_sRoot, "TAB", "edgeRP.html");
	//DeleteFile(szFile);

	if (!file.Open(szFile, CFile::modeWrite|CFile::modeCreate | CFile::shareDenyNone))
	{
		AfxMessageBox("no file path 3");
		return ;
	}					
	file.Write(strdata, strdata.GetLength());
	file.Close();

	ShellExecute(NULL, _T("open"), szFile, NULL,NULL, SW_SHOW);		
}

LRESULT WebLinkCtrl::OnBrowser( WPARAM wParam, LPARAM lParam )
{
	TRY
	{
		if (m_pIEApp)
		{
			CString url;
			//url = (char*)lParam;
			url = m_finalurl;


			HRESULT hr = S_OK;
			VARIANT vEmpty;
			VARIANT vHeader;
			VariantInit(&vEmpty);
			
			CString strHeader;
			strHeader = "IBK투자증권";
			vHeader.vt = VT_BSTR;
			vHeader.bstrVal = strHeader.AllocSysString();
			
			BSTR bstrURL = url.AllocSysString();

			pBrowser2->put_Resizable(TRUE);
			pBrowser2->put_Width(m_width);
			pBrowser2->put_Height(m_height);
		
			//hr = pBrowser2->Navigate(bstrURL, &vEmpty, &vEmpty, &vEmpty, &vHeader);
			CString sBlank = "about:blank";
			BSTR bstrBlank = sBlank.AllocSysString();
			hr = pBrowser2->Navigate(bstrBlank, &vEmpty, &vEmpty, &vEmpty, &vHeader);
			
			if (SUCCEEDED(hr))
			{
				m_pIEApp->put_MenuBar(false);
				m_pIEApp->put_ToolBar(false);
				m_pIEApp->put_StatusBar(false);
				
				
				CRect rc;

				GetParent()->GetParent()->GetClientRect(rc);

				ClientToScreen(rc);
				
				m_pIEApp->put_Top(100);
				m_pIEApp->put_Left(rc.left + 200);

				//hr = pBrowser2->Navigate(bstrURL, &vEmpty, &vEmpty, &vEmpty, &vHeader);
				
				HWND pWB;
				//m_pIEApp->get_HWND((long*)&pWB);
				
				pBrowser2->get_HWND((long*)&pWB);
				pBrowser2->put_Silent(VARIANT_TRUE);
				CWnd *pwnd = CWnd::FromHandle(pWB); 

				//hr = pBrowser2->Navigate(bstrBlank, &vEmpty, &vEmpty, &vEmpty, &vHeader);
				
				m_pIEApp->put_Visible(true);

				pwnd->BringWindowToTop();
				pwnd->CenterWindow();
				
				if ( m_bCertLogin)
				{
					CString surl(m_baseURL);
					m_baseURL.Replace("http","https");


					bstrURL = m_baseURL.AllocSysString();
					CString sHeader = _T("Content-Type: application/x-www-form-urlencoded\r\n");
	
					VARIANT vtHeader;

					vtHeader.vt = VT_BSTR;
					vtHeader.bstrVal = sHeader.AllocSysString();
					
					CString sParam;
					CString stmp;
					if (m_sMapName == "IB831300")  //온라인계좌개설
					{
						stmp = m_sUrl;
						const	int	ifind = stmp.Find("&auth");
						if (ifind > 0)
							stmp = m_sUrl.Left(ifind);
						
						sParam.Format("url=%s&cflag=1&auth=%s",stmp,m_sCert);
					}
					else
					{
						sParam.Format("url=%s&cflag=1&auth=%s",m_sUrl,m_sCert);
					}

					SAFEARRAY FAR *sfPost = NULL;
					SAFEARRAYBOUND bound;

					bound.cElements = sParam.GetLength();
					bound.lLbound = 0;
					sfPost = SafeArrayCreate(VT_UI1, 1, &bound);

					for (long lIndex = 0; lIndex < (signed)bound.cElements; lIndex++)
					{
						SafeArrayPutElement(sfPost, &lIndex, sParam.GetBuffer(bound.cElements)+lIndex);
					}
					sParam.ReleaseBuffer();
			
					VARIANT vtPost;
					vtPost.vt = VT_ARRAY|VT_UI1;
					vtPost.parray = sfPost;

					hr = pBrowser2->Navigate(bstrURL, &vEmpty, &vEmpty, &vtPost, &vtHeader);
				}
				else  //ID 로그인
				{
					hr = pBrowser2->Navigate(bstrURL, &vEmpty, &vEmpty, &vEmpty, &vHeader);
				}

				CString data;
				data.Format("$WEBHWND\t%u",(LONG)pwnd);
				
				Variant(triggerCC,data);
			}
			else
			{


				pBrowser2->Quit();
			}
			
			//SysFreeString(bstrURL);
		}
	}
	CATCH (CException, e)
	{
		char emsg[1024];
		e->GetErrorMessage(emsg, 1024);

		CString errMsg;
		errMsg.Format("%s",emsg);

		AfxMessageBox(errMsg);
	}
	END_CATCH;

	return 0;
}

BOOL WebLinkCtrl::CheckBrowserVersion()
{
	LONG	lResult = 0;
	HKEY	hKey{};
	TCHAR	keyValue[10]{};
	DWORD	dwType = REG_SZ; 
	DWORD	dwSize = sizeof(keyValue);
	
	lResult = RegOpenKeyEx(HKEY_LOCAL_MACHINE,_T("SOFTWARE\\Microsoft\\Internet Explorer"), 0, KEY_QUERY_VALUE, &hKey);
	
	if (lResult != ERROR_SUCCESS)
	{
		return FALSE;
	}
	
	lResult = RegQueryValueEx(hKey, _T("svcVersion"), 0, &dwType, (LPBYTE)keyValue, &dwSize);
	if (lResult != ERROR_SUCCESS)
	{
		lResult = RegQueryValueEx(hKey, _T("Version"), 0, &dwType, (LPBYTE)keyValue, &dwSize);

		if (lResult != ERROR_SUCCESS)
		{
			return FALSE;
		}
	}

	CString sValue;
	sValue = keyValue;

	if (sValue.GetAt(0) == '7')
		return TRUE;
	
	RegCloseKey(hKey);

	return FALSE;
}


int WebLinkCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CString strdata, name;
	name = Variant(nameCC, "");

	int readL = 0;
	char readB[1024];
	CString userD;
	userD.Format("%s\\%s\\%s\\%s.ini", m_sRoot, "user", name, name);
	readL = GetPrivateProfileString("MODE", "main", "", readB, sizeof(readB), userD);

	if (readL <= 0)
		return 0;

	CString sWnd;
	sWnd.Format("%s", readB);
	int ddata = atoi(sWnd);
	HWND hWnd = (HWND)ddata;
	m_pMainFrame = CWnd::FromHandle(hWnd);
	
	return 0;
}

void WebLinkCtrl::WriteLog(LPCSTR log, ...)
{
#if 1
	TRY
	{
		CString slog;
	slog.Format("[AXIS][WriteLog] [%s]\n", log);
	//OutputDebugString(slog);

		FILE* fp;
		fopen_s(&fp, m_sRoot + "\\exe\\axis.log", "a+");
		if (!fp) return;

		const CTime time = CTime::GetCurrentTime();
		fprintf(fp, (LPCSTR)time.Format("[%Y-%m-%d %H:%M:%S] "));

		va_list argptr;
		va_start(argptr, log);
		vfprintf(fp, log, argptr);
		va_end(argptr);
		fprintf(fp, "\n");

		fclose(fp);
	}
		CATCH(CMemoryException, e)
	{

	}
	END_CATCH
#endif
}

void WebLinkCtrl::NavigateFRmap(LPCTSTR sUrl, bool blogin)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_url.Format("%s", sUrl);
	m_url.Trim();
	m_strUrl = m_url;
	m_sUrl = m_url;
	WriteLog("NavigateFRmap 1 = " + m_url);
	m_description = "";
	m_url.TrimLeft(); m_url.TrimRight();
	m_url.Remove(0);


	CString path;
	m_type = "BROWSER";
	m_url = GetURL();
		
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
		WriteLog(m_finalurl.Left(100) + "......");
		if (GetOSversion() == 11)
		{
			openhtml();
		}
		else
			PostMessage(WM_BROWSER, 0, (LPARAM)url.GetString());

		m_parent->GetParent()->PostMessage(WM_CLOSE);
	}
}


void WebLinkCtrl::SetWidth(LONG width)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_width = (int)width;
	
}


void WebLinkCtrl::SetHeight(LONG height)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_height = (int)height;
}
