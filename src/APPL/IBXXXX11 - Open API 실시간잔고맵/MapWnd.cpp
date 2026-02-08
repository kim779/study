// MapWnd.cpp : implementation file
//

#include "stdafx.h"
#include "IBXXXX11.h"
#include "MapWnd.h"

#include "../../h/fxCommonx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMapWnd

CMapWnd::CMapWnd(CWnd *pParent)
: CAxisExt(pParent)
{
	m_pCNotify = nullptr;
	m_pSNotify = nullptr;
	m_bType = F_NOTIFY;
	m_MainWnd = nullptr;
	m_pParent = nullptr;
}

CMapWnd::~CMapWnd()
{
//	DELWND(m_pCNotify);
//	DELWND(m_pSNotify);
}


BEGIN_MESSAGE_MAP(CMapWnd, CWnd)
	//{{AFX_MSG_MAP(CMapWnd)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnMessage)
	ON_MESSAGE(WM_ACCOUNT, OnAccount)
	ON_MESSAGE(WM_APP_SIG, OnAppSignal)

END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMapWnd message handlers

BOOL CMapWnd::CreateMap(CWnd *pParent)
{
	m_pParent = pParent;

	const BOOL bCreate = Create(NULL, NULL, WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS, CRect(0, 0, 300, 200), m_pParent, 100);
	CString path = Variant(homeCC, "");
	
	Variant(titleCC, "Untitled");
	path.Replace("\\", "_");
	
	return bCreate;
}

void CMapWnd::JangoSend(LPCTSTR acno, LPCTSTR pswd)
{
	if(m_bType)	// 선물
		m_pCNotify->Send(acno, pswd, "");
	else		// 주식
		m_pSNotify->Send(acno, pswd, "");
}

long CMapWnd::OnMessage(WPARAM wParam, LPARAM lParam)
{
	switch(LOBYTE(LOWORD(wParam)))
	{
		case DLL_OUB:
			break;

		case DLL_ALERT:
			break;

		case DLL_NOTICE:
			break;

		case DLL_GUIDE:
			return true;
			break;
	}
	return 0;
}

long CMapWnd::OnAccount(WPARAM wParam, LPARAM lParam)
{
	CString strData = (char*)lParam;
	int nIndex;
	nIndex = strData.Find("\t");

	m_MainWnd = (HWND)LOWORD(wParam);

	if(HIWORD(wParam) == F_NOTIFY)
	{
		if(m_pCNotify == NULL)
		{
			m_pCNotify = std::make_unique<CNotifyCtrl>(m_pParent, Variant(homeCC, ""), F_NOTIFY);
			m_pCNotify->Create(NULL, "FNOTIFY", WS_CHILD, CRect(0, 0, 0, 0), this, 5002);
		}
		m_bType = F_NOTIFY;
	}
	else if(HIWORD(wParam) == S_NOTIFY)
	{
		if(m_pSNotify == NULL)
		{
			m_pSNotify = std::make_unique<CNotifyCtrl>(m_pParent, Variant(homeCC, ""), S_NOTIFY);
			m_pSNotify->Create(NULL, "SNOTIFY", WS_CHILD, CRect(0, 0, 0, 0), this, 5003);
		}
		m_bType = S_NOTIFY;
	}
	
	JangoSend(strData.Left(nIndex), strData.Right(strData.GetLength() - nIndex - 1));
	return 0;
}

long CMapWnd::OnAppSignal(WPARAM wParam, LPARAM lParam)
{
	CString strData;
	switch(wParam)
	{
		case WP_F_JNGO_CHANGE:
			// 		OutputDebugString("[KSJ]IBXXXX11 WP_JNGO_CHANGE \n");
			::SendMessage(m_MainWnd, WM_USER, WP_F_JNGO_CHANGE, lParam);
			break;
	
		case WP_S_JNGO_CHANGE:
			// 		OutputDebugString("[KSJ]IBXXXX11 WP_JNGO_CHANGE \n");
			::SendMessage(m_MainWnd, WM_USER, WP_S_JNGO_CHANGE, lParam);
			break;
	}

	return 0;
}
