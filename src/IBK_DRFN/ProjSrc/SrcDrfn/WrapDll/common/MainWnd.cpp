// MainWnd.cpp : implementation file
//

#include "stdafx.h"
//#include "si035000.h"
#include "MainWnd.h"

// #include "../common/BaseNetworkModule.h"
// #include "../common/HistoricalVolateWnd.h"
// #include "../common/KeyManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainWnd
#define SAFE_DELETE(p) if(p){p->DestroyWindow(); delete p; p = NULL;}

CMainWnd::CMainWnd(CWnd* pWnd) :m_pIDebugMng(NULL)
{
 	m_pParent = pWnd;
// 	m_pwndBaseNetwork = NULL;
// 
// 	m_pHistoricalWnd  = NULL;
}

CMainWnd::~CMainWnd()
{
}

BEGIN_MESSAGE_MAP(CMainWnd, CWnd)
	//{{AFX_MSG_MAP(CMainWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_COPYDATA()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnUser)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainWnd message handlers
int CMainWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_pIDebugMng = (IDebugManager*)AfxGetPctrInterface(UUID_IDebugManager);

// 	m_pwndBaseNetwork = new CBaseNetworkModule(m_pParent);
// 	BOOL bret = m_pwndBaseNetwork->CreateBaseNetworkWnd(10000, this);
// 
// 	m_pHistoricalWnd = new CHistoricalVolateWnd(m_pParent);
// 	bret = m_pHistoricalWnd->CreateWnd(10000, this);

	if(m_pIDebugMng)
	{
		LPCSTR pTranName = "p0602";
		STIDM2Info_SR_A srA;
		WORD dwOnCopyDataKey =  10;
		memset(&srA, 0x20, sizeof(STIDM2Info_SR_A));

		m_pIDebugMng->TraceSR('A', 'I', pTranName, m_hWnd, dwOnCopyDataKey);
	}
	return 0;
}


LONG CMainWnd::OnUser(WPARAM wParam, LPARAM lParam)
{
	LONG	ret = 0;
	BYTE bt = LOBYTE(LOWORD(wParam));
	switch(LOBYTE(LOWORD(wParam)))
	{
// 	case DLL_OUB:			// Tr수신
// 		{
// 			int nKind = (HIBYTE(LOWORD(wParam)));			
// 			if(nKind == TR_HIS_IV_TOT_IV_DATE)
// 				m_pHistoricalWnd->SendMessage(WM_USER, wParam, lParam);
// 			else
// 				m_pwndBaseNetwork->SendMessage(WM_USER, wParam, lParam);
// 		}
// 		break;
// 	case DLL_ALERT:			// 리얼 수신
// 		m_pwndBaseNetwork->SendMessage(WM_USER, wParam, lParam);
// 		break;
// 	case DLL_SETPAL:
// 		break;
// 	case DLL_DOMINO:		// 화면 열리면서 데이터 공유 받을 때
// 		break;
// 	case DLL_GUIDE:
// 		return 1;
// 	case DLL_SETFONTx:
// 		break;
 	case DLL_OUB:			// Tr수신
	default: break;
	}

	return ret;
}

void CMainWnd::OnDestroy() 
{
	CWnd::OnDestroy();

// 	if(m_pwndBaseNetwork)
// 	{
// 		m_pwndBaseNetwork->DestroyWindow();
// 		delete m_pwndBaseNetwork;
// 		m_pwndBaseNetwork = NULL;
// 	}
// 
// 	if(m_pHistoricalWnd)
// 	{
// 		if(IsWindow(m_pHistoricalWnd->GetSafeHwnd()))
// 			m_pHistoricalWnd->DestroyWindow();
// 		delete m_pHistoricalWnd;
// 		m_pHistoricalWnd = NULL;
// 	}
}

BOOL CMainWnd::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct) 
{
	if(m_pIDebugMng)
	{
		m_pIDebugMng->Trace("CMainWnd::OnCopyData[cbData=%d] [dwData=%d]", 
					pCopyDataStruct->cbData,
					pCopyDataStruct->dwData);
	}

	return CWnd::OnCopyData(pWnd, pCopyDataStruct);
}
