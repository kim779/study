/////////////////////////////////////////////////////////////////////////////
// SimpleBrowser.cpp: Web browser control
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "comdef.h"
//#include "comutil.h"
//#include "mshtml.h"
//#include "mshtmcid.h"
//#include "mshtmhst.h"
#include "exdispid.h"

#include "SimpleBrowser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define Unused(parameter) parameter					// avoid compile warnings
// about unused parameters

#define TAG_CODEVALIDATIONKEY "cate=item&key="
#define TAG_TERMVALIDATIONKEY "cate=term&key="
#define TAG_HELPVALIDATIONKEY "cate=help&key=" 

//#import "C:\Windows\System32\mshtml.tlb"

/////////////////////////////////////////////////////////////////////////////
// Construction and creation
/////////////////////////////////////////////////////////////////////////////

SimpleBrowser::SimpleBrowser(CWnd *pWizard, CString map1, CString map2)
	: _Browser(nullptr),
	_BrowserDispatch(nullptr),
	m_bDocumentComplete(false),
	m_strHTML(_T(""))
{
	m_pWizard = pWizard;
	m_strMap1 = map1;
	m_strMap2 = map2;
}

SimpleBrowser::~SimpleBrowser()
{
	// release browser interfaces	
	if (_Browser != nullptr)
	{
		_Browser->Release();
		_Browser = nullptr;
	}
	
	if (_BrowserDispatch != nullptr)
	{
		_BrowserDispatch->Release();
		_BrowserDispatch = nullptr;
	}
}

// Standard create

BOOL SimpleBrowser::Create(DWORD dwStyle, 
                           const RECT& rect, 
                           CWnd* pParentWnd, 
			   UINT nID)
{
	BOOL results = TRUE;	
	_Browser     = nullptr;
	
	// create this window	
	CWnd *window = this;
	results = window->Create(AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW),
		nullptr,
		dwStyle,
		rect,
		pParentWnd,
		nID);
	if (!results) return FALSE;
	
	// create browser control window as child of this window; 
	// this window sinks events from control
	
	const CRect browser_window_rect(0,0,(rect.right - rect.left),(rect.bottom - rect.top));
	
	results = _BrowserWindow.CreateControl(CLSID_WebBrowser,
		nullptr,
		(WS_VISIBLE | WS_TABSTOP),
		browser_window_rect,
		this,
		AFX_IDW_PANE_FIRST);
	if (!results) 
	{
		DestroyWindow();
		return FALSE;
	}
	
	// get control interfaces	
	LPUNKNOWN unknown = _BrowserWindow.GetControlUnknown();	
	HRESULT hr = unknown->QueryInterface(IID_IWebBrowser2,(void **)&_Browser);
	if (SUCCEEDED(hr)) 
	{
		hr = unknown->QueryInterface(IID_IDispatch,(void **)&_BrowserDispatch);
	}
	
	if (!SUCCEEDED(hr)) 
	{
		_BrowserWindow.DestroyWindow();
		DestroyWindow();
		return FALSE;        
	}
	
	// navigate to blank page; wait for document creation
	
	if (_Browser != nullptr)
	{
		// 스크립트 오류 표시 안함
		_Browser->put_Silent(VARIANT_TRUE);

		Navigate(_T("about:blank"));
		
		IHTMLDocument2 *document = nullptr;
		HRESULT	hr = S_OK;
		
		while ((document == nullptr) && (hr == S_OK))
		{			
			Sleep(0);
			
			IDispatch *document_dispatch = nullptr;
			hr = _Browser->get_Document(&document_dispatch);
			
			// if dispatch interface available, retrieve document interface			
			if (SUCCEEDED(hr) && (document_dispatch != nullptr))
			{				
				// retrieve document interface				
				hr = document_dispatch->QueryInterface(IID_IHTMLDocument2,(void **)&document);				
				document_dispatch->Release();				
			}
			
		}
		
		if (document != nullptr)
		{
			document->Release();	
		}		
	}
	
	return TRUE;
}

// Create in place of dialog control

BOOL SimpleBrowser::CreateFromControl(CWnd *pParentWnd,UINT nID)
{
	BOOL result = FALSE;	
	ASSERT(pParentWnd != nullptr);
	
	if (pParentWnd == nullptr)
		return FALSE;

	CWnd *control = pParentWnd->GetDlgItem(nID);	
	if (control != nullptr)
	{		
		// get control location
		
		CRect		rect;
		
		control->GetWindowRect(&rect);
		pParentWnd->ScreenToClient(&rect);
		
		// destroy control, since the browser will take its place		
		control->DestroyWindow();
		
		// create browser		
		result = Create((WS_CHILD | WS_VISIBLE),
			rect,
			pParentWnd,
			nID);		
	}
	
	return result;
}

// Destruction

void SimpleBrowser::PostNcDestroy()
{
	// release browser interfaces	
	if (_Browser != nullptr)
	{
		_Browser->Release();
		_Browser = nullptr;
	}
	
	if (_BrowserDispatch != nullptr)
	{
		_BrowserDispatch->Release();
		_BrowserDispatch = nullptr;
	}
}

/////////////////////////////////////////////////////////////////////////////
// Controls
/////////////////////////////////////////////////////////////////////////////

// Navigate to URL

void SimpleBrowser::Navigate(LPCTSTR URL)
{
	if (_Browser != nullptr)
	{		
		CString		url(URL);
		
		_variant_t	flags(0L,VT_I4);
		_variant_t	target_frame_name("");
		_variant_t	post_data("");
		_variant_t	headers("");
		
		BSTR bstr = url.AllocSysString();
		
		_Browser->Navigate(bstr,
			&flags,
			&target_frame_name,
			&post_data,
			&headers);
		::SysFreeString(bstr);
	}
}

void SimpleBrowser::NavigateEx(CString strHTML)
{
	if (false == m_bDocumentComplete)
	{
		m_strHTML = strHTML;
		return;
	}

	HRESULT hr{};
	
	IStream* pStream = nullptr;
	IDispatch* pHtmlDoc = nullptr;
	IPersistStreamInit* pPersistStreamInit = nullptr;
	HGLOBAL hHTMLText;
	
	hHTMLText = GlobalAlloc(GPTR, strHTML.GetLength() + 1);
	if(hHTMLText)
	{
		lstrcpy((char *)hHTMLText, strHTML);
		hr = CreateStreamOnHGlobal(hHTMLText, FALSE, &pStream);
		if(SUCCEEDED(hr))
		{
			// Retrieve the document object.
			pHtmlDoc = GetHtmlDocument();
			if ( pHtmlDoc != nullptr)
			{
				// Query for IPersistStreamInit.
				hr = pHtmlDoc->QueryInterface( IID_IPersistStreamInit,  (void**)&pPersistStreamInit );
				if (SUCCEEDED(hr))
				{
					// Initialize the document.
					hr = pPersistStreamInit->InitNew();
					if(SUCCEEDED(hr))
					{
						// Load the contents of the stream.
						hr = pPersistStreamInit->Load(pStream);
					}
					pPersistStreamInit->Release();
				}
				pHtmlDoc->Release();
			}
			pStream->Release();
		}

		GlobalFree(hHTMLText);
	}
}

LPDISPATCH SimpleBrowser::GetHtmlDocument() const
{
	ASSERT(_Browser != nullptr);

	LPDISPATCH result;
	_Browser->get_Document(&result);

	return result;
}

// Navigate to HTML document in resource
void SimpleBrowser::NavigateResource(int resource_ID)
{
	if (_Browser != nullptr)
	{		
		CString resource_string;
		
		// load HTML document from resource		
		HRSRC resource_handle = FindResource(AfxGetResourceHandle(), MAKEINTRESOURCE(resource_ID), RT_HTML);		
		if (resource_handle != nullptr)
		{			
			HGLOBAL resource = LoadResource(AfxGetResourceHandle(), resource_handle);			
			if (resource != nullptr)
			{				
				LPVOID resource_memory = LockResource(resource);				
				if (resource_memory != nullptr)
				{					
					const DWORD resource_size = SizeofResource(AfxGetResourceHandle(),
						resource_handle);
					
					// identify the resource document as MBCS (e.g. ANSI)
					// or UNICODE
					
					bool     UNICODE_document = false;					
					wchar_t *UNICODE_memory   = (wchar_t *)resource_memory;
					int      UNICODE_size     = resource_size / sizeof(wchar_t);
					
					if (UNICODE_size >= 1) 
					{						
						// check for UNICODE byte order mark
						
						if (*UNICODE_memory == L'\xFEFF') 
						{
							UNICODE_document = true;
							UNICODE_memory  += 1;
							UNICODE_size    -= 1;
						}
						
						// otherwise, check for UNICODE leading tag
						
						else if (UNICODE_size >= 5) 
						{							
							if ((UNICODE_memory[0]           == L'<') &&
								(towupper(UNICODE_memory[1]) == L'H') &&
								(towupper(UNICODE_memory[2]) == L'T') &&
								(towupper(UNICODE_memory[3]) == L'M') &&
								(towupper(UNICODE_memory[4]) == L'L')) {
								UNICODE_document = true;
							}							
						}
						
						// Note: This logic assumes that the UNICODE resource document is 
						//       in little-endian byte order, which would be typical for 
						//       any HTML document used as a resource in a Windows application.
						
					}
					
					// convert resource document if required
					
#if !defined(UNICODE)
					
					if (UNICODE_document) 
					{						
						char *MBCS_buffer = resource_string.GetBufferSetLength(resource_size + 1);						
						const int MBCS_length = ::WideCharToMultiByte(CP_ACP,
							0,
							UNICODE_memory,
							UNICODE_size,
							MBCS_buffer,
							resource_size + 1,
							nullptr,
							nullptr);
						
						resource_string.ReleaseBuffer(MBCS_length);						
					}					
					else 
					{						
						resource_string = CString((char *)resource_memory,resource_size);						
					}
					
#else
					
					if (UNICODE_document) 
					{						
						resource_string = CString(UNICODE_memory,UNICODE_size);						
					}					
					else 
					{						
						wchar_t *UNICODE_buffer = resource_string.GetBufferSetLength(resource_size + 1);						
						int UNICODE_length = ::MultiByteToWideChar(CP_ACP,
							0,
							(const char *)resource_memory,
							resource_size,
							UNICODE_buffer,
							(resource_size + 1));
						
						resource_string.ReleaseBuffer(UNICODE_length);						
					}
					
#endif					
					
				}
				
			}			
		}
		
		Clear();
		Write(resource_string);		
	}
}

// Append string to current document

void SimpleBrowser::Write(LPCTSTR string)
{
	if (_Browser != nullptr)
	{		
		// get document interface		
		IHTMLDocument2 *document = GetDocument();		
		if (document != nullptr)
		{			
			// construct text to be written to browser as SAFEARRAY
			
			SAFEARRAY *safe_array = SafeArrayCreateVector(VT_VARIANT,0,1);
			
			VARIANT	*variant;
			
			SafeArrayAccessData(safe_array,(LPVOID *)&variant);
			
			variant->vt      = VT_BSTR;
			variant->bstrVal = CString(string).AllocSysString();
			
			SafeArrayUnaccessData(safe_array);
			
			// write SAFEARRAY to browser document
			
			document->write(safe_array);
			
			::SysFreeString(variant->bstrVal);
			
			document->Release();
			document = nullptr;
		}		
	}
}

void SimpleBrowser::Clear()
{
	if (_Browser != nullptr) 
	{		
		// if document interface available, close/re-open document to clear display
		
		IHTMLDocument2	*document = GetDocument();
		HRESULT		hr = S_OK;
		
		if (document != nullptr)
		{			
			// close and re-open document to empty contents
			
			document->close();
			
			VARIANT		open_name{};
			VARIANT		open_features{};
			VARIANT		open_replace{};
			IDispatch	*open_window = nullptr;
			
			::VariantInit(&open_name);
			
			open_name.vt      = VT_BSTR;
			open_name.bstrVal = ::SysAllocString(L"_self");
			
			::VariantInit(&open_features);
			::VariantInit(&open_replace);
			
			BSTR bstr = ::SysAllocString(L"text/html");
			
			hr = document->open(bstr,
				open_name,
				open_features,
				open_replace,
				&open_window);
			
			::SysFreeString(bstr);
			::SysFreeString(open_name.bstrVal);
			
			if (hr == S_OK) 
			{
				Refresh();
			}
			
			if (open_window != nullptr)
			{
				open_window->Release();
			}			
		}		
		else	// otherwise, navigate to about:blank and wait for document ready		
		{			
			Navigate(_T("about:blank"));
			
			IHTMLDocument2 *document = nullptr;
			HRESULT		hr = S_OK;			
			while ((document == nullptr) && (hr == S_OK))
			{				
				Sleep(0);
				
				IDispatch *document_dispatch = nullptr;
				hr = _Browser->get_Document(&document_dispatch);
				
				// if dispatch interface available, retrieve document interface
				
				if (SUCCEEDED(hr) && (document_dispatch != nullptr))
				{					
					// retrieve document interface					
					hr = document_dispatch->QueryInterface(IID_IHTMLDocument2,(void **)&document);					
					document_dispatch->Release();					
				}				
			}
			
			if (document != nullptr)
			{
				document->Release();	
			}			
		}		
	}
}

// Navigation operations

void SimpleBrowser::GoBack()
{
	if (_Browser != nullptr) {
		_Browser->GoBack();
	}
}

void SimpleBrowser::GoForward()
{
	if (_Browser != nullptr) {
		_Browser->GoForward();
	}
}

void SimpleBrowser::GoHome()
{
	if (_Browser != nullptr) {
		_Browser->GoHome();
	}
}

void SimpleBrowser::Refresh()
{
	if (_Browser != nullptr) {
		_Browser->Refresh();
	}
}

void SimpleBrowser::Stop()
{
	if (_Browser != nullptr) {
		_Browser->Stop();
	}
}

// Print contents

void SimpleBrowser::Print(LPCTSTR header,LPCTSTR footer)
{
	if (_Browser != nullptr)
	{		
		// construct two element SAFEARRAY;
		// first element is header string, second element is footer string
		
		HRESULT hr{};
		
		VARIANT		header_variant;
		VariantInit(&header_variant);
		V_VT(&header_variant)   = VT_BSTR;
		V_BSTR(&header_variant) = CString(header).AllocSysString();
		
		VARIANT		footer_variant;
		VariantInit(&footer_variant);
		V_VT(&footer_variant)   = VT_BSTR;
		V_BSTR(&footer_variant) = CString(footer).AllocSysString();
		
		long index = 0;
		
		SAFEARRAYBOUND	parameter_array_bound[1] = {};
		SAFEARRAY	*parameter_array = nullptr;
		
		parameter_array_bound[0].cElements = 2;
		parameter_array_bound[0].lLbound   = 0;
		
		parameter_array = SafeArrayCreate(VT_VARIANT,1,parameter_array_bound);
		
		index = 0;
		hr    = SafeArrayPutElement(parameter_array,&index,&header_variant);
		
		index = 1;
		hr    = SafeArrayPutElement(parameter_array,&index,&footer_variant);
		
		VARIANT	parameter;
		VariantInit(&parameter);
		V_VT(&parameter)    = VT_ARRAY | VT_BYREF;
		V_ARRAY(&parameter) = parameter_array;
		
		// start printing browser contents
		
		hr = _Browser->ExecWB(OLECMDID_PRINT,OLECMDEXECOPT_DODEFAULT,&parameter, nullptr);
		
		// Note: There is no simple way to determine that printing has completed. 
		//       In fact, if the browser is destroyed while printing is in progress, 
		//       only part of the contents will be printed.
		
		// release SAFEARRAY
		
		if (!SUCCEEDED(hr)) 
		{
			VariantClear(&header_variant);
			VariantClear(&footer_variant);
			if (parameter_array != nullptr)
			{
				SafeArrayDestroy(parameter_array);
			}
		}		
	}
}

// Miscellaneous

bool SimpleBrowser::GetBusy()
{
	bool busy = false;
	
	if (_Browser != nullptr)
	{		
		VARIANT_BOOL    variant_bool;		
		const HRESULT hr = _Browser->get_Busy(&variant_bool);
		if (SUCCEEDED(hr)) 
		{
			busy = (variant_bool == VARIANT_TRUE);    
		}		
	}
	
	return busy;
}

CString SimpleBrowser::GetLocationName()
{
	CString location_name = _T("");	
	if (_Browser != nullptr)
	{		
		BSTR location_name_BSTR = nullptr;
		const HRESULT hr = _Browser->get_LocationName(&location_name_BSTR);
		if (SUCCEEDED(hr)) 
		{
			location_name = location_name_BSTR;
		}
		
		::SysFreeString(location_name_BSTR);		
	}
	
	return location_name;
}

CString SimpleBrowser::GetLocationURL()
{
	CString location_URL = _T("");	
	if (_Browser != nullptr)
	{		
		BSTR location_URL_BSTR = nullptr;
		const HRESULT hr = _Browser->get_LocationURL(&location_URL_BSTR);
		if (SUCCEEDED(hr)) 
		{
			location_URL = location_URL_BSTR;
		}
		
		::SysFreeString(location_URL_BSTR);		
	}
	
	return location_URL;
}

READYSTATE SimpleBrowser::GetReadyState()
{
	READYSTATE readystate = READYSTATE_UNINITIALIZED;
	
	if (_Browser != nullptr)
	{
		_Browser->get_ReadyState(&readystate);
	}
	
	return readystate;
}

bool SimpleBrowser::GetSilent()
{
	bool silent = false;
	
	if (_Browser != nullptr)
	{		
		VARIANT_BOOL silent_variant{};
		const HRESULT hr = _Browser->get_Silent(&silent_variant);

		if (SUCCEEDED(hr)) 
		{
			silent = (silent_variant == VARIANT_TRUE);
		}		
	}
	
	return silent;
}

void SimpleBrowser::PutSilent(bool silent)
{
	if (_Browser != nullptr)
	{		
		VARIANT_BOOL silent_variant{};
		if (silent) silent_variant = VARIANT_TRUE;
		else        silent_variant = VARIANT_FALSE;
		
		_Browser->put_Silent(silent_variant);
	}
}

IHTMLDocument2 *SimpleBrowser::GetDocument()
{
	IHTMLDocument2 *document = nullptr;
	if (_Browser != nullptr)
	{		
		// get browser document's dispatch interface		
		IDispatch *document_dispatch = nullptr;
		HRESULT hr = _Browser->get_Document(&document_dispatch);		
		if (SUCCEEDED(hr) && (document_dispatch != nullptr))
		{			
			// get the actual document interface			
			hr = document_dispatch->QueryInterface(IID_IHTMLDocument2, (void **)&document);
			
			// release dispatch interface			
			document_dispatch->Release();			
		}		
	}
	
	return document;
}

/////////////////////////////////////////////////////////////////////////////
// Message handlers
/////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(SimpleBrowser,CWnd)
//{{AFX_MSG_MAP(SimpleBrowser)
ON_WM_SIZE()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// Resize control window as this window is resized

void SimpleBrowser::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	
	if (_Browser != nullptr)
	{
		const CRect rect(0,0,cx,cy);
		_BrowserWindow.MoveWindow(&rect);
	}
}

/////////////////////////////////////////////////////////////////////////////
// Event handlers
/////////////////////////////////////////////////////////////////////////////

BEGIN_EVENTSINK_MAP(SimpleBrowser,CWnd)
ON_EVENT(SimpleBrowser,AFX_IDW_PANE_FIRST,
	 DISPID_BEFORENAVIGATE2,
	 _OnBeforeNavigate2, 
	 VTS_DISPATCH VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PBOOL)
	 ON_EVENT(SimpleBrowser,AFX_IDW_PANE_FIRST,
	 DISPID_DOCUMENTCOMPLETE,
	 _OnDocumentComplete, 
	 VTS_DISPATCH VTS_PVARIANT)
	 ON_EVENT(SimpleBrowser,AFX_IDW_PANE_FIRST,
	 DISPID_DOWNLOADBEGIN,
	 _OnDownloadBegin, 
	 VTS_NONE)
	 ON_EVENT(SimpleBrowser,AFX_IDW_PANE_FIRST,
	 DISPID_PROGRESSCHANGE,
	 _OnProgressChange, 
	 VTS_I4 VTS_I4)
	 ON_EVENT(SimpleBrowser,AFX_IDW_PANE_FIRST,
	 DISPID_DOWNLOADCOMPLETE,
	 _OnDownloadComplete, 
	 VTS_NONE)
	 ON_EVENT(SimpleBrowser,AFX_IDW_PANE_FIRST,
	 DISPID_NAVIGATECOMPLETE2,
	 _OnNavigateComplete2, 
	 VTS_DISPATCH VTS_PVARIANT)
	 ON_EVENT(SimpleBrowser,AFX_IDW_PANE_FIRST,
	 DISPID_STATUSTEXTCHANGE,
	 _OnStatusTextChange, 
	 VTS_BSTR)
	 ON_EVENT(SimpleBrowser,AFX_IDW_PANE_FIRST,
	 DISPID_TITLECHANGE,
	 _OnTitleChange, 
	 VTS_BSTR)
	 END_EVENTSINK_MAP()
	 
	 SimpleBrowser::Notification::Notification(HWND hwnd,UINT ID,NotificationType type)
{
	hdr.hwndFrom	= hwnd;
	hdr.idFrom	= ID;
	hdr.code	= type;
	URL		= _T("");
	frame		= _T("");
	post_data	= nullptr;
	post_data_size	= 0;
	headers		= _T("");
	progress	= 0;
	progress_max	= 0;
	text		= _T("");
}

// Called before navigation begins; application may cancel if required

void SimpleBrowser::_OnBeforeNavigate2(LPDISPATCH lpDisp,
                                       VARIANT FAR *URL,
                                       VARIANT FAR *Flags,
                                       VARIANT FAR *TargetFrameName,
                                       VARIANT FAR *PostData,
                                       VARIANT FAR *Headers,
                                       VARIANT_BOOL *Cancel)
{
	Unused(Flags);	// Note: flags value is reserved
	
	if (lpDisp == _BrowserDispatch) 
	{		
		CString		URL_string;
		CString		frame;
		std::unique_ptr<unsigned char[]> post_data = nullptr;
		int		post_data_size	= 0;
		CString		headers;
		
		if ((URL != nullptr) && (V_VT(URL) == VT_BSTR))
		{
			URL_string = V_BSTR(URL);
		}
		
		if ((TargetFrameName != nullptr) && (V_VT(TargetFrameName) == VT_BSTR))
		{
			frame = V_BSTR(TargetFrameName);
		}
		
		if ((PostData != nullptr) && (V_VT(PostData) == (VT_VARIANT | VT_BYREF)))
		{			
			VARIANT *PostData_variant = V_VARIANTREF(PostData);			
			if ((PostData_variant != nullptr) && (V_VT(PostData_variant) != VT_EMPTY))
			{				
				SAFEARRAY *PostData_safearray = V_ARRAY(PostData_variant);				
				if (PostData_safearray != nullptr)
				{					
					char *post_data_array = nullptr;					
					SafeArrayAccessData(PostData_safearray,(void HUGEP **)&post_data_array);
					
					long		lower_bound = 1;
					long		upper_bound = 1;
					
					SafeArrayGetLBound(PostData_safearray,1,&lower_bound);
					SafeArrayGetUBound(PostData_safearray,1,&upper_bound);
					
					post_data_size = (int)(upper_bound - lower_bound + 1);					
					post_data = std::make_unique<unsigned char[]>(post_data_size);
					
					memcpy(post_data.get(),post_data_array,post_data_size);					
					SafeArrayUnaccessData(PostData_safearray);					
				}				
			}			
		}
		
		const bool cancel = OnBeforeNavigate2(URL_string, frame, post_data.get(), post_data_size, headers);

		if (Cancel != nullptr)
		{
			if (cancel) *Cancel = VARIANT_TRUE;
			else        *Cancel = VARIANT_FALSE;
		}
		
//		delete[] post_data;		
	}    
}

bool SimpleBrowser::OnBeforeNavigate2(CString URL,
                                      CString frame,
				      void    *post_data,int post_data_size,
				      CString headers)
{
	bool cancel = false;	
	const CWnd *parent = GetParent();	
	if (parent != nullptr)
	{		
		Notification	notification(m_hWnd,GetDlgCtrlID(),BeforeNavigate2);
		
		notification.URL		= URL;
		notification.frame		= frame;
		notification.post_data		= post_data;
		notification.post_data_size	= post_data_size;
		notification.headers		= headers;

//////////////////////////////////////////////뉴스검색 
		bool	bFind = FALSE;
		bool	bflag = FALSE;				// 2009/09/01 KJH ADD
		bool	bfpop = FALSE;
		CString	strVal, strParam;
		if (URL.Find(TAG_CODEVALIDATIONKEY) != -1)
		{
			strVal = URL.Mid(URL.Find(TAG_CODEVALIDATIONKEY) + strlen(TAG_CODEVALIDATIONKEY) + 1);
			strParam.Format("%s/S/p5/d 1301\t%s", m_strMap1, strVal);
			bFind = TRUE;
			bfpop = TRUE;
		}
		else if (URL.Find(TAG_TERMVALIDATIONKEY) != -1)
		{
//			strVal = URL.Mid(URL.Find(TAG_TERMVALIDATIONKEY) + strlen(TAG_TERMVALIDATIONKEY)+ 1); 
			strVal = URL.Mid(URL.Find(TAG_TERMVALIDATIONKEY) + strlen(TAG_TERMVALIDATIONKEY));		// 2009/09/01 KJH CHANGE
			strParam.Format("%s/S/p5/d GUBN\t91\nSEQN\t%s", m_strMap2, strVal);
			bFind = TRUE;
			bflag = TRUE;
		}
		else if (URL.Find(TAG_HELPVALIDATIONKEY) != -1)
		{
//			strVal = URL.Mid(URL.Find(TAG_HELPVALIDATIONKEY) + strlen(TAG_HELPVALIDATIONKEY) + 1);
			strVal = URL.Mid(URL.Find(TAG_HELPVALIDATIONKEY) + strlen(TAG_HELPVALIDATIONKEY));		// 2009/09/01 KJH CHANGE
			strParam.Format("%s/S/p5/d GUBN\t92\nSEQN\t%s", m_strMap2, strVal);
			bFind = TRUE;
		}
		
		if (bFind)
		{
			if (!bflag)
			{
				if (!bfpop)
					m_pWizard->SendMessage(WM_USER, MAKEWPARAM(viewDLL, typeVIEW), (LPARAM)(LPCTSTR)strParam);
				else
					m_pWizard->SendMessage(WM_USER, MAKEWPARAM(viewDLL, typePOPUP), (LPARAM)(LPCTSTR)strParam);

			}
			else 
				m_pWizard->SendMessage(WM_USER, MAKEWPARAM(viewDLL, typePOPUPX), (LPARAM)(LPCTSTR)strParam);	// 2009/09/01 KJH ADD
			
			
			return TRUE;
		} 
//////////////////////////////////////////////	
		
		const  LRESULT result = parent->SendMessage(WM_NOTIFY, notification.hdr.idFrom, (LPARAM)&notification);
		if (result) 
		{
			cancel = true;
		}		
	}
	
	return cancel;
}

// Document loaded and initialized

void SimpleBrowser::_OnDocumentComplete(LPDISPATCH lpDisp,VARIANT *URL)
{
	if (lpDisp == _BrowserDispatch) 
	{
		CString URL_string;		
		if ((URL != nullptr) && (V_VT(URL) == VT_BSTR))
		{
			URL_string = V_BSTR(URL);
		}		
		OnDocumentComplete(URL_string);
	}    
}

void SimpleBrowser::OnDocumentComplete(CString URL)
{
	const  CWnd *parent = GetParent();
	if (parent != nullptr)
	{		
		Notification	notification(m_hWnd,GetDlgCtrlID(),DocumentComplete);		
		notification.URL = URL;		
		const LRESULT result = parent->SendMessage(WM_NOTIFY,	notification.hdr.idFrom, (LPARAM)&notification);
	}

	if (false == m_bDocumentComplete)
	{
		m_bDocumentComplete = true;
		if (!m_strHTML.IsEmpty())
		{
			NavigateEx(m_strHTML);
			m_strHTML.Empty();
		}
	}
}

// Navigation/download progress

void SimpleBrowser::_OnDownloadBegin()
{
	OnDownloadBegin();
}

void SimpleBrowser::OnDownloadBegin()
{
	const CWnd *parent = GetParent();
	if (parent != nullptr)
	{		
		Notification	notification(m_hWnd,GetDlgCtrlID(),DownloadBegin);		
		const LRESULT result = parent->SendMessage(WM_NOTIFY, notification.hdr.idFrom, (LPARAM)&notification);
	}
}

void SimpleBrowser::_OnProgressChange(long progress,long progress_max)
{
	OnProgressChange((int)progress,(int)progress_max);
}

void SimpleBrowser::OnProgressChange(int progress,int progress_max)
{
	const CWnd *parent = GetParent();
	if (parent != nullptr)
	{		
		Notification	notification(m_hWnd,GetDlgCtrlID(),ProgressChange);
		
		notification.progress     = progress;
		notification.progress_max = progress_max;
		
		const LRESULT result = parent->SendMessage(WM_NOTIFY, notification.hdr.idFrom, (LPARAM)&notification);
	}
}

void SimpleBrowser::_OnDownloadComplete()
{
	OnDownloadComplete();
}

void SimpleBrowser::OnDownloadComplete()
{
	const CWnd *parent = GetParent();
	if (parent != nullptr)
	{		
		Notification	notification(m_hWnd,GetDlgCtrlID(),DownloadComplete);		
		const LRESULT result = parent->SendMessage(WM_NOTIFY, notification.hdr.idFrom, (LPARAM)&notification);
	}
}

// Navigation to a link has completed

void SimpleBrowser::_OnNavigateComplete2(LPDISPATCH lpDisp,VARIANT *URL)
{
	if (lpDisp == _BrowserDispatch) 
	{		
		CString URL_string;		
		if ((URL != nullptr) && (V_VT(URL) == VT_BSTR))
		{
			URL_string = V_BSTR(URL);
		}
		
		OnNavigateComplete2(URL_string);		
	}    
}

void SimpleBrowser::OnNavigateComplete2(CString URL)
{
	const CWnd *parent = GetParent();
	if (parent != nullptr)
	{		
		Notification	notification(m_hWnd,GetDlgCtrlID(),NavigateComplete2);		
		notification.URL = URL;		
		const LRESULT result = parent->SendMessage(WM_NOTIFY, notification.hdr.idFrom, (LPARAM)&notification);
	}
}

// Status text has changed

void SimpleBrowser::_OnStatusTextChange(BSTR bstrText)
{
	CString text;	
	if (bstrText != nullptr)
	{
		text = (LPCTSTR)bstrText;
	}
	
	OnStatusTextChange(text);
}

void SimpleBrowser::OnStatusTextChange(CString text)
{
	const CWnd *parent = GetParent();
	if (parent != nullptr)
	{		
		Notification	notification(m_hWnd,GetDlgCtrlID(),StatusTextChange);		
		notification.text = text;		
		const LRESULT result = parent->SendMessage(WM_NOTIFY, notification.hdr.idFrom, (LPARAM)&notification);
	}
}

// Title text has changed

void SimpleBrowser::_OnTitleChange(BSTR bstrText)
{
	CString text;
	if (bstrText != nullptr)
	{
		text = (LPCTSTR)bstrText;
	}
	
	OnTitleChange(text);
}

void SimpleBrowser::OnTitleChange(CString text)
{
	const CWnd *parent = GetParent();
	if (parent != nullptr)
	{		
		Notification	notification(m_hWnd,GetDlgCtrlID(),TitleChange);		
		notification.text = text;		
		const LRESULT result = parent->SendMessage(WM_NOTIFY, notification.hdr.idFrom, (LPARAM)&notification);
	}
}

int SimpleBrowser::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	Navigate("about:black");
	
	return 0;
}
