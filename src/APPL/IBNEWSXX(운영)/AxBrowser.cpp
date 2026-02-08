#include "stdafx.h"
#include "ibnewsxx.h"
#include "AxBrowser.h"
#include "Strsafe.h"
#include <stdio.h>
#include "common.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

[[gsl::suppress(26409)]]
IMPLEMENT_DYNCREATE(CAxBrowser, CHtmlView)

CAxBrowser::CAxBrowser()
{
	//{{AFX_DATA_INIT(CAxBrowser)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CAxBrowser::~CAxBrowser()
{
}

void CAxBrowser::DoDataExchange(CDataExchange* pDX)
{
	CHtmlView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAxBrowser)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAxBrowser, CHtmlView)
	//{{AFX_MSG_MAP(CAxBrowser)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

#ifdef _DEBUG
void CAxBrowser::AssertValid() const
{
//	CHtmlView::AssertValid();
}

void CAxBrowser::Dump(CDumpContext& dc) const
{
//	CHtmlView::Dump(dc);
}

#endif //_DEBUG

void CAxBrowser::NavigateFromMemory( LPCSTR dat, int len )
{
	if (dat==NULL) return;

	HRESULT hr = S_OK;
	HGLOBAL hGlobal = NULL;
	IStream *pStream = NULL;
	ULONG lWritten{};

	do {
		HGLOBAL hGlobal = GlobalAlloc(GHND, len);
		if (hGlobal==NULL) break;

		hr = CreateStreamOnHGlobal(hGlobal, TRUE, &pStream);
		if (FAILED(hr)) break;

		if (pStream != NULL) {
			hr = pStream->Write(dat, len, &lWritten);
			if (SUCCEEDED(hr))
			{
				LARGE_INTEGER pos;
				pos.QuadPart = 0;
				pStream->Seek((LARGE_INTEGER)pos, STREAM_SEEK_SET, NULL);
			}
			else
				break;
		}
		else break;

		NavigateFromStream(pStream);

	} while(FALSE);

	if (pStream) pStream->Release();
	if (hGlobal) GlobalFree(hGlobal);
}

void CAxBrowser::NavigateFromStream( IStream *pStream )
{
	if (pStream==NULL) return;

	HRESULT hr{};
	IDispatch* pHtmlDoc = NULL;
	IPersistStreamInit* pPersistStreamInit = NULL;

	do {
		
		hr = m_pBrowserApp->get_Document(&pHtmlDoc);
		if (FAILED(hr) || pHtmlDoc==NULL) break;

		hr = pHtmlDoc->QueryInterface(IID_IPersistStreamInit, (void**)&pPersistStreamInit);
		if (FAILED(hr) || pPersistStreamInit==NULL) break;

		hr = pPersistStreamInit->InitNew();
		if (FAILED(hr)) break;
		
		hr = pPersistStreamInit->Load( pStream );
		
	} while(FALSE);
	
	if (pPersistStreamInit) pPersistStreamInit->Release();
	if (pHtmlDoc) pHtmlDoc->Release();
}


void CAxBrowser::OnBeforeNavigate2(LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName, CByteArray& baPostedData, LPCTSTR lpszHeaders, BOOL* pbCancel)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	const CWnd* pWnd = GetParent();
	LPCSTR stURL = "axis://menu/";
	CString mapn, url = lpszURL;

	if (url.Find(stURL) != -1 && pWnd)
	{
		url.Replace(stURL, "");
		if (url.GetLength() == 7)
			url = url.Right(6);
		mapn.Format("IB200100 /t/S/d1301\t%s\n", (LPCSTR)url);
		pWnd->SendMessage(WX_EVT_MAPN, NULL, (LPARAM)(LPCSTR)mapn);
		*pbCancel = TRUE;
	}
//	CHtmlView::OnBeforeNavigate2(lpszURL, nFlags, lpszTargetFrameName, baPostedData, lpszHeaders, pbCancel);
}
