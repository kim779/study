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
#include "../../../h/axisfire.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//** 내부 생성 콘트롤용 ControlID 정의
const UINT constWebLinkCtrlID		= 20900;
const UINT constWebBrowserCtrlID	=  1000;






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
	m_baseURL = m_userID = m_itemCode = _T("");
	m_ctrl = NULL;
	m_baseURL = "http://www.ibks.com/LoadService.jsp";

	//** register this control
	RegisterClass();
}

//** destructor of this class
WebLinkCtrl::~WebLinkCtrl()
{
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
	ON_MESSAGE(WM_USER + 1234, OnMessage)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(WebLinkCtrl, CWnd)
	//{{AFX_DISPATCH_MAP(WebLinkCtrl)
	DISP_PROPERTY_EX(WebLinkCtrl, "BaseURL", GetBaseURL, SetBaseURL, VT_BSTR)
	DISP_PROPERTY_EX(WebLinkCtrl, "ItemCode", GetItemCode, SetItemCode, VT_BSTR)
	DISP_PROPERTY_EX(WebLinkCtrl, "UserID", GetUserID, SetUserID, VT_BSTR)
	DISP_PROPERTY_EX(WebLinkCtrl, "Home", GetHome, SetHome, VT_BSTR)
	DISP_PROPERTY_EX(WebLinkCtrl, "Menu", GetMenu, SetMenu, VT_BSTR)
	DISP_PROPERTY_EX(WebLinkCtrl, "Width", GetWidth, SetNotSupported, VT_I2)
	DISP_PROPERTY_EX(WebLinkCtrl, "Height", GetHeight, SetNotSupported, VT_I2)
	DISP_FUNCTION(WebLinkCtrl, "ResizeToFitWindow", ResizeToFitWindow, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(WebLinkCtrl, "Navigate", Navigate, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(WebLinkCtrl, "GetCaption", GetCaption, VT_BSTR, VTS_NONE)
	//}}AFX_DISPATCH_MAP
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

	/*
	CScrollBar* sc = m_ctrl->GetScrollBarCtrl();
	CRect src;
	sc->GetClientRect(&src);
	int sc_height = src.Width();
	MoveWindow(rc, TRUE);

	rc.bottom = rc.bottom - 20;
	*/

	CRect src, src2;
	pParent->GetParent()->GetWindowRect(src2);
	pParent->GetWindowRect(src);
	
	CString tmp;
	tmp.Format("%d, %d: src top : %d , src2 top: %d \n",src.top, src2.top, src.Height(), src2.Height());
	TRACE(tmp);
	if ((src.top-src2.top)>40)  //탭이 존재하는 경우 총 42 차이가 남
	  rc.bottom = rc.bottom - (src2.Height() - src.Height());

	rc.top -= 2;
	rc.left -= 2;
	rc.bottom +=2;
	rc.right +=2;

	MoveWindow(rc, TRUE);

	if (m_ctrl)
		m_ctrl->MoveWindow(rc, TRUE);
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

	char buffer[MAX_PATH];
	CString ini;
	ini.Format("%stab\\axis.ini", m_home);

	CString home(ProfileStr("WebLink", "home", "", ini).value);

	if (!home.IsEmpty())
		m_baseURL = buffer;
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

BOOL WebLinkCtrl::Create(CWnd* parent, void* param)
{
	m_parent = parent;
	m_param  = param;

	BOOL bResult = CWnd::Create(m_className, _T(""), WS_VISIBLE | WS_CHILD, CRect(0, 0, 0, 0), 
		m_parent, constWebLinkCtrlID, NULL);

	if (bResult) {
		m_ctrl = new CWebBrowserCtrl;

		if (!m_ctrl->Create(_T("WebBrowser2"), WS_BORDER | WS_VISIBLE | WS_CHILD, CRect(0, 0, 1, 1), this, constWebBrowserCtrlID))
		{
			delete m_ctrl;
			m_ctrl = NULL;
		}
		
		ResizeToFitWindow();
	}

	//m_ctrl->ShowScrollBar(SB_BOTH);

	
	/*
	if (m_ctrl)	
	{
		m_ctrl->SetSilent(false);
	}
	*/
	// Define the name and type of the client scripts on the page.
	

	


	return bResult;
}

void WebLinkCtrl::OnDestroy() 
{
	if (m_ctrl) {
		m_ctrl->DestroyWindow();
		delete m_ctrl;
	}
	KillTimer(2);
	
	CWnd::OnDestroy();	
}

void WebLinkCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	ResizeToFitWindow();
	SetTimer(3,100,NULL);  // 탭이 존재하는경우 리사이징시 수평스크롤 문제 때문에
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
	m_url  = ProfileStr(m_menu, "URL", "", path).value;
	m_description = ProfileStr(m_menu, "Description", "", path).value;
}

CString WebLinkCtrl::GetURL()
{
	//예외처리 (LoadService.jsp 안타도록 하려면 주석을 해제하세요.
	CString temp(m_url);
	temp.MakeUpper();
	if (temp.Find("KRX.CO.KR") > 0)
		return m_url;
	
	if ((temp[0]!='/'))
	{
		if (temp.Find("CONTENTS=TRUE")>0)
		{
			return m_url.Mid(0,temp.Find("CONTENTS=TRUE")-1);
		}
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

	const char* trCommand = "bestez:tr";

	int find = url.Find(trCommand);

	if (find > 0)
	{
		*Cancel = TRUE;

		CString map = url.Mid(find + lstrlen(trCommand) + 1, 4);

		CString mapOriginal = FindMap(map);
		if (mapOriginal.IsEmpty())
			return;
		
		m_parent->SendMessage(WM_USER, MAKEWPARAM(viewDLL, typeVIEW), (LPARAM)(LPCTSTR)mapOriginal);
	}
}

CString WebLinkCtrl::FindMap(LPCTSTR map)
{
	const char* mapNamePrefix = "IB"; // only IBK MAC

	CString menuFilePath;
	menuFilePath.Format("%stab\\axis.menu", m_home);

	CFile* pFile;

	TRY
	{
		pFile = new CFile(menuFilePath, CFile::modeRead);
	}
	CATCH (CFileException, e)
	{
		return "";
	}
	END_CATCH;
	
	LPSTR buffer = new char[pFile->GetLength() + 1];

	pFile->Read(buffer, pFile->GetLength());
	buffer[pFile->GetLength()] = 0x00;

	CString s(buffer);

	delete pFile;
	delete [] buffer;

	int nFind = s.Find(CString(map) + '\t');

	if (nFind < 0) 
		return "";

	int index = -1;
	for (int i = nFind; i >= 0; i--)
	{
		if (s[i] == mapNamePrefix[0] && s[i+1] == mapNamePrefix[1])
		{
			index = i;
			break;
		}
	}

	if (index < 0)
		return "";

	return s.Mid(index, 8);
}

void WebLinkCtrl::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == 1)
	{
		KillTimer(nIDEvent);
		if (m_parent->GetParent())
			m_parent->GetParent()->SetWindowPos(NULL, 0, 0, m_width, m_height, SWP_NOMOVE | SWP_NOZORDER);
		
		ResizeToFitWindow();
	}
	else if (nIDEvent == 2)
	{
		if (kill_count > 1000)
		{
			KillTimer(nIDEvent);
			kill_count = 0;
		}

		CWnd* hwd = FindWindow(NULL, "오류");
		if (!hwd) hwd = FindWindow(NULL, "Internet Explorer 스크립트 오류");
		if (!hwd) hwd = FindWindow(NULL, "Error");
		if (!hwd) hwd = FindWindow(NULL, "ERROR");

		if (hwd)
		{
			//hwd->ShowWindow(SW_HIDE);
			hwd->PostMessage(WM_COMMAND, MAKELONG(IDNO, BN_CLICKED), 0);		
		}

		
		kill_count++;
	}
	else if (nIDEvent == 3)
	{
		KillTimer(nIDEvent);
		ResizeToFitWindow();
	}
	


	CWnd::OnTimer(nIDEvent);
}

void WebLinkCtrl::Navigate() 
{
	if (m_menu.IsEmpty()) 
		return;
	if (m_home.IsEmpty()) 
		return;

	GetInformation();

	if (m_type.CompareNoCase("BROWSER") == 0)
	{
		//** 액티브 되어 있는 익스플로러를 사용하여 열고자 할 때는 아래 코드 사용
		//HINSTANCE hInst = ShellExecuteEx(NULL, _T("open"), m_url, _T(""), NULL, SW_SHOWNORMAL);
		//** 항상 새 창으로 열고자 할 때는 아래 코드 사용
		HINSTANCE hInst = ShellExecute(NULL, "open", "iexplore.exe", m_url, NULL, SW_SHOWNORMAL);

		if (hInst < (HINSTANCE)32)
		{
			// error execute
		}

		m_parent->GetParent()->PostMessage(WM_CLOSE);
	}
	else 
	{
		CString url = GetURL();

		/**
		CFile* pFile;
		TRY
		{
			pFile = new CFile("c:\\test.txt", CFile::modeWrite);
		}
		CATCH (CFileException, e)
		{

		}
		END_CATCH;

		pFile->SeekToEnd();
		pFile->Write((LPCTSTR)url, url.GetLength());
		pFile->Write("\r\n", 2);
		delete pFile;
		**/

		//AfxMessageBox(url);

		m_parent->SetWindowPos(NULL, 0, 0, m_width, m_height, SWP_NOMOVE | SWP_NOZORDER);
		ResizeToFitWindow();
		
		

		m_ctrl->Navigate(url, NULL, NULL, NULL, NULL);

		
		SetTimer(1, 10, NULL);
		kill_count = 0;
		SetTimer(2, 25, NULL);
	}
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


CString WebLinkCtrl::GetAuthParam()
{
	CString auth = "id=" + URLEncode(m_userID);
	
	auth += "&pw=";
	auth += URLEncode(GetUserPassword());

	auth += "&certpw=";
	auth += URLEncode(GetCertPassword());

	auth = URLEncode(HTSEncode(auth, "ibkis"));

	/**
	//** test
	CString auth = "id=" + URLEncode("ibkit");
	
	auth += "&pw=";
	auth += URLEncode("123456");

	auth += "&certpw=";
	auth += URLEncode("654321");

	auth = URLEncode(HTSEncode(auth, "ibkis"));
	**/

	return auth;
}

CString WebLinkCtrl::HTSEncode(const char* lpszSource, const char* key)
{
	int lenKey = strlen(key);
	int lenSource = strlen(lpszSource);

	if (!lenSource || !lenKey)
		return "";

	LPSTR buffer = new char[lenSource + 1];

	for (int i = 0; i < lenSource; i++)
	{
		buffer[i] = (lpszSource[i] ^ (0x0f & key[i % lenKey])) - 1;
	}
	buffer[lenSource] = 0x00;

	CString result;
	result = buffer;

	/** Decoding test 

	for (i = 0; i < lenSource; i++)
	{
		buffer[i] = (result[i] + 1) ^ (0x0f & key[i % lenKey]);
	}

	AfxMessageBox(result);
	AfxMessageBox(buffer);
	**/


	delete [] buffer;

	return result;
}
//////////////////////////////////////////////////////////////////////////
// IBK Main Program's WM_USER definition
// Warning! IBK only.
#define MMSG_GETPASSWORD		0x01
#define MMSG_GETCERTPASSWORD	0x02

CString WebLinkCtrl::GetUserPassword()
{
	CWnd* wnd = AfxGetMainWnd();
	ASSERT(wnd);

	return CString((LPCTSTR)wnd->SendMessage(WM_USER, MMSG_GETPASSWORD));
}

CString WebLinkCtrl::GetCertPassword()
{
	CWnd* wnd = AfxGetMainWnd();
	ASSERT(wnd);

	return CString((LPCTSTR)wnd->SendMessage(WM_USER, MMSG_GETCERTPASSWORD));
}


LRESULT WebLinkCtrl::OnMessage(WPARAM wParam, LPARAM lParam)
{
	if (m_ctrl)
	{
		LPUNKNOWN lpU = m_ctrl->GetControlUnknown();
		IOleInPlaceActiveObject *pIO;
		HRESULT hr = lpU->QueryInterface(IID_IOleInPlaceActiveObject, (void **)&pIO);
		if(SUCCEEDED(hr))  //영문키 입력이 안되서 수정
		//if(hr)
		{
			LPMSG pMsg = (MSG*)lParam;
			pIO->TranslateAccelerator(pMsg);
			pIO->Release();
			return TRUE;
		}
	}
	return FALSE;
}

void WebLinkCtrl::OnDocumentComplete(LPDISPATCH lpDisp, VARIANT FAR* URL)
{
	//ResizeToFitWindow();


/*
	IUnknown*  pUnk;
	LPDISPATCH lpWBDisp;
	HRESULT    hr;
	IHTMLDocument3 *doc		= NULL;
	IHTMLElement *elem			= NULL;
	
	pUnk = m_ctrl->GetControlUnknown();
	ASSERT(pUnk);
	
	hr = pUnk->QueryInterface(IID_IDispatch, (void**)&lpWBDisp);
	ASSERT(SUCCEEDED(hr));
	lpWBDisp->Set
	if (lpDisp == lpWBDisp )
	{
		// Top-level Window object, so document has been loaded
		TRACE("Web document is finished downloading\n");
	}

	lpWBDisp->Release();
*/
	//=====================================================================
	
	
/*	
	
	IHTMLDocument2* pHTMLDocument;
    LPDISPATCH lpDispatch;    
    
    lpDispatch = m_ctrl->GetDocument();
	CString m_Temp;
    m_Temp ="";
		
		
		
		if (lpDispatch)
		{
			HRESULT hr;
			hr = lpDispatch->QueryInterface(IID_IHTMLDocument2,
				(LPVOID*)
				
				&pHTMLDocument);
			lpDispatch->Release();
			ASSERT(SUCCEEDED(hr));
			
			
			IHTMLElement*  pBody;
			
			HRESULT nhr = pHTMLDocument -> get_body(&pBody);
			
			if (FAILED(hr)) return;
			
			CString strTexTToBeAdded("");
			
			
			strTexTToBeAdded += m_Temp; 
			
			
			BSTR BstrTexTToBeAdded;    
			
			BstrTexTToBeAdded =     CString
				(strTexTToBeAdded).AllocSysString();
            
			//pBody->insertAdjacentHTML(L"afterBegin", L"&#xa0;<SCRIPT For='window' Event='onerror'>var noOp = null;</SCRIPT>");
			pBody->insertAdjacentHTML(L"afterBegin", L"<script> alert(테스트); </script>");
			
			SysFreeString(BstrTexTToBeAdded);
			
			pBody->Release();
		}
        
		
    pHTMLDocument->Release();
	
	
	
	
*/	
	//=====================================================================

/*

	IWebBrowser2 *thisBrowser		= NULL;
	IDispatch *docDisp			= NULL;
	IHTMLDocument2 *doc		= NULL;
	IHTMLElement *elem			= NULL;
	HRESULT hr = S_OK;
	
	hr = m_ctrl->QueryInterface(IID_IWebLinkCtrl, reinterpret_cast<void **>(&thisBrowser));
	CHECK_HR(hr, cleanup);
	hr = thisBrowser->get_Document(&docDisp);
	CHECK_HR(hr, cleanup);
	hr = docDisp->QueryInterface(IID_IHTMLDocument2, reinterpret_cast<void**>(&doc));
	CHECK_HR(hr, cleanup);
	RRELEASE(docDisp);
	
	// Get the documentElement for the document object, and then call 
	// IHTMLElement::insertAdjacentHTML() to add the onerror SCRIPT 
	// handler tag.
	hr = doc->get_documentElement(&elem);
	CHECK_HR(hr, cleanup);
	// NOTE: The &nbsp, or some other visible HTML, is required. Internet Explorer will not
	// parse and recognize the script block without some visual HTML to
	// accompany it.
	hr = elem->insertAdjacentHTML(L"afterBegin", L"&#xa0;<SCRIPT For='window' Event='onerror'>var noOp = null;</SCRIPT>");
	
	// Cleanup.
cleanup:
	RRELEASE(elem);
	RRELEASE(thisBrowser);
	RRELEASE(doc);
*/
}
