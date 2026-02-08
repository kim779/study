 // ViewWrap.cpp : implementation file
//

#include "stdafx.h"
#include "ViewWrap.h"

#include <afxpriv.h>
#include "TrMChartView.h"
#include "../../inc/RMsgUserInfo.h"
//#include "../../../SrcSite/include/InterestInfo.h"
#include "../../../SrcSite/include/define.h"

#include "SI050000.h"

#include "../../chart/COMMONTR/TRDefine.h"
#include "../../inc/IAUTrCommMng.h"
#include "../../inc/IMainInfoMng.h"	

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define		WM_DLLDATA		(WM_USER+9111)	//위닉스 메인에서 저장관련해서 보내는 메시지.

//--> 0027629: [0000] 서버로 부터 조회되는 차트모듈 관한 확인 및 수정요청
#define TRKEY_MEMO_POPUPOOP		118
#define TRKEY_MEMO_CHECK		124
//<--

CString		Glb_SubTrCode;
//HWND		Glb_hFormMngCtrl = NULL;
//IDispatch*	Glb_ScrFormDisp = NULL;

// extern "C"
// {
// 	__declspec(dllexport) int OnCodeDragAndDropProc(CWnd* pViewWnd, LPCTSTR strCode ); 
// 	// M*N화면 조회정보 저장 및 로드 처리
// 	__declspec(dllexport) void GetLastData(CWnd* pViewWnd, LPCTSTR strPageScrNo, LPCTSTR strPageIndex, CString& strData); 
// 	__declspec(dllexport) void SetLastData(CWnd* pViewWnd, LPCTSTR strPageScrNo, LPCTSTR strPageIndex, LPCTSTR strData); 
// };
// 
// int OnCodeDragAndDropProc(CWnd* pViewWnd, LPCTSTR strCode )
// {
// 	((CTrMChartView*)pViewWnd)->SetCodeDragAndDrop(strCode);
// 	return FALSE;
// }
// 
// void GetLastData(CWnd* pViewWnd, LPCTSTR strPageScrNo, LPCTSTR strPageIndex, CString& strData)
// {
// 	if(pViewWnd)
// 	{
// 		((CTrMChartView*)pViewWnd)->GetLastData(strPageScrNo, strPageIndex, strData);
// 	}
// }
// 
// void SetLastData(CWnd* pViewWnd, LPCTSTR strPageScrNo, LPCTSTR strPageIndex, LPCTSTR strData)
// {
// 	if(pViewWnd)
// 	{
// 		((CTrMChartView*)pViewWnd)->SetLastData(strPageScrNo, strPageIndex, strData);
// 	}
// }

// 	STDAPI CreateScreen( CWnd** ppWrapWnd, CWnd** ppScrWnd, CWnd* pParent,
// 						 LPCTSTR strSubTRCode, IDispatch * FormDisp, LPCTSTR strMapFileName, HWND hFormMng, CMapStringToOb * ArrCtrlName, long lBkColor, int nAddView)			 					 
// 	{
// 		Glb_SubTrCode = strSubTRCode;
// 		//Glb_MapFileName = strMapFileName;
// 		Glb_hFormMngCtrl = hFormMng;
// 		Glb_ScrFormDisp = FormDisp;
// 		
// 		CViewWrap* pWrapWnd = new CViewWrap();
// 		
// 		CRect rect;
// 		pParent->GetWindowRect( &rect );
// 		pParent->ScreenToClient( &rect );
// 
// 		if( !pWrapWnd->Create( NULL, NULL, WS_CHILD|WS_VISIBLE, rect, pParent, 999 ) )
// 			return E_FAIL;
// 
// 		// 추가 한다.
// 		if(nAddView == 1)
// 			pWrapWnd->AddViewForSingle();
// 
// 		*ppWrapWnd = pWrapWnd;
// 		*ppScrWnd = pWrapWnd->m_pView;
// 
// 		((CTrMChartView*)pWrapWnd->m_pView)->m_hFormMng = hFormMng;
// 		((CTrMChartView*)pWrapWnd->m_pView)->m_lBkColor = lBkColor;
// 		((CTrMChartView*)pWrapWnd->m_pView)->m_MapControlName = ArrCtrlName;
// 		((CTrMChartView*)pWrapWnd->m_pView)->m_strScreenNo = strSubTRCode;
// 		((CTrMChartView*)pWrapWnd->m_pView)->m_ScrFormDisp = FormDisp;
// 		((CTrMChartView*)pWrapWnd->m_pView)->m_strMapFileName = strMapFileName;
// 		((CTrMChartView*)pWrapWnd->m_pView)->m_pFormWnd = pParent;
// 
// 		return NOERROR;
// 	}


// 	STDAPI DestroyScreen( CWnd* pWrapWnd )
// 	{
// 		CViewWrap* pDelWnd = (CViewWrap*)pWrapWnd;
// 		pDelWnd->DestroyWindow();
// 		delete pDelWnd;
// 
// 		return NOERROR;
// 	}

const UINT RMSG_SENDMESSAGE = ::RegisterWindowMessage(_T("RMSG_SENDMESSAGE"));


/////////////////////////////////////////////////////////////////////////////
// CViewWrap

CViewWrap::CViewWrap()
{
	m_pView = NULL;

	m_pPtSvrMng = NULL;
	m_pPtMng = NULL;
}

CViewWrap::~CViewWrap()
{
	_DoClearSendItem();
}


BEGIN_MESSAGE_MAP(CViewWrap, CWnd)
	//{{AFX_MSG_MAP(CViewWrap)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_MESSAGE(WM_USER, OnUser)
	ON_MESSAGE(WM_DLLDATA, OnWinixDllData)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(RMSG_SENDMESSAGE, OnRmsgSendMessage)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CViewWrap message handlers

int CViewWrap::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	_DoGetPartnerManger();

	if (theApp.m_szDllName.Find(_T("0510")) >= 0)
	{
		/*
		struct	시스템트레이딩 동의여부 TR Name : pidosyst
		struct  mid     {
				char    mode[1];        // 1:조회 2:저장                
				char    syst[1];        // 동의여부 1:동의, 0:미동의    
		};
		struct  mod     {
				char    syst[1];        // 동의여부 1:동의, 0:미동의    
				char    sflg[1];        // OK '0' else ERROR            
				char    emsg[99];
		}; 
		*/
		_DoDirectSendTR(TRNO_SYSTEMAGREE, "11", 2, TRKEY_SYSTEMAGREE);

		SetTimer(510, 5000, NULL);
	}
	else
	{
		// TODO: Add your specialized creation code here
		if( !CreateView( RUNTIME_CLASS( CTrMChartView ) ) )
			return -1;
	}

	return 0;
}

void CViewWrap::OnDestroy() 
{
	_DoGetPartnerManger(FALSE);
	CWnd::OnDestroy();
}

void CViewWrap::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	if( m_pView )
	{
		m_pView->SetWindowPos( NULL, 0, 0, cx, cy, SWP_NOZORDER );
	}
}

typedef	CWnd * (CreateTradingAup)(UINT nDldresId, CWnd* pParent);

BOOL CViewWrap::CreateView( CRuntimeClass * pViewClass )
{
    //CRect rect, client;
    ASSERT(pViewClass );

    CWnd *pWnd=(CWnd *)pViewClass->CreateObject();
    if (!pWnd) return FALSE;	

	CTrMChartView *pTRView = (CTrMChartView*)pWnd;

	//100530:동일화면 다르게 저장/읽기
	pTRView->m_szFileKey = m_szFileKey;

	int dwStyle = AFX_WS_DEFAULT_VIEW;
    dwStyle&=~WS_BORDER;
	dwStyle&=~WS_VISIBLE;

    if( !pWnd->Create(NULL, NULL, dwStyle, CRect(0,0,0,0), this, 999, NULL ) )
    {
        TRACE0("Warning: couldn't create client area for tab view\n");
        // pWnd will be cleaned up by PostNcDestroy
		return FALSE;
    }

	WPARAM wp = MAKEWPARAM(USERINFO_SET, 0);
	if(pTRView->m_pChartScreen && IsWindow(pTRView->m_pChartScreen->GetSafeHwnd()))
		pTRView->m_pChartScreen->SendMessage(RMSG_USERINFO, wp, (LPARAM)m_pPtMng);

	//pWnd->ModifyStyleEx( 0, WS_EX_CLIENTEDGE);

	CRect rect;
	GetWindowRect( &rect );
	ScreenToClient( &rect );	
	
	pWnd->MoveWindow( rect );
	
	m_pView = (CView*)pWnd;
	pWnd->ShowWindow( SW_SHOW );
//}

    return TRUE;
}

// void CViewWrap::AddViewForSingle( )
// {
// 	CDocument* pDoc = ((CView*)GetParent()->GetParent())->GetDocument();
// 	pDoc->AddView((CView*)m_pView);
// }

BOOL CViewWrap::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
//	return CWnd::OnEraseBkgnd(pDC);
	return TRUE;
}

void CViewWrap::_DoGetPartnerManger(BOOL _bAdvise)
{
	if(_bAdvise)
	{
		m_pPtSvrMng = (IPartnerSvrManager*)AfxGetPctrInterface(UUID_IPartnerSvrManager);
		if(m_pPtSvrMng)
		{
			m_pPtMng = m_pPtSvrMng->AdviseManager(GetParent()->GetSafeHwnd(), m_hWnd);
//			{
//				CString szDebug;
//				szDebug.Format("ViewWrap [%08X], [%08X]\n", m_hWnd, m_pPtMng);
//				OutputDebugString(szDebug);
//			}
		}
	}
	else
	{
		if(m_pPtSvrMng)
		{
			m_pPtSvrMng->UnAdviseManager(GetParent()->GetSafeHwnd(), m_hWnd);
			m_pPtSvrMng = NULL;
			m_pPtMng = NULL;
		}
	}
}

int CViewWrap::_DoParsingSendData(CString &szData, CSendItem &Info)
{
	int nReceiveLen = szData.GetLength();
	if(nReceiveLen==0) return 0;

	int nPos1 = 0, nOffset=0;
	CString szTmp, szCode, szCodeName;

	CSendItem SendItem;
	int nDataLen = szData.GetLength();
	//while(nPos1>=0 && nOffset<=nDataLen)
	{
		nPos1 = szData.Find('\t', nOffset);
		if(nPos1<0) return 0;
		SendItem.m_szTR = szData.Left(nPos1);

		//-----------------------------------
		nOffset = nPos1+1;
		nPos1 = szData.Find('\t', nOffset);
		if(nPos1<0) return 0;
		szTmp = szData.Mid(nOffset, nPos1-nOffset);
		sscanf(szTmp, "%08X", &SendItem.m_pCallback);

		//-----------------------------------
		nOffset = nPos1+1;
		nPos1 = szData.Find('\t', nOffset);
		if(nPos1<0) return 0;
		szTmp = szData.Mid(nOffset, nPos1-nOffset);
		SendItem.m_nKey = atoi(szTmp);

		//-----------------------------------
		nOffset = nPos1+1;
		nPos1 = szData.Find('\t', nOffset);
		if(nPos1<0) return 0;
		szTmp = szData.Mid(nOffset, nPos1-nOffset);
		SendItem.m_nLen = atoi(szTmp);

		//-----------------------------------
		nOffset = nPos1+1;
		//SendItem.m_pData = (LPBYTE)(LPCSTR)szData.Mid(nOffset);

		_DoListSendItem(TRUE, &SendItem);
		Info.Copy(&SendItem);
	}
	return nOffset;
}

int CViewWrap::_DoListSendItem(BOOL bAdd, CSendItem* _pItem)
{
	if(bAdd)
	{
		for(POSITION pos=m_SIList.GetHeadPosition(); pos;)
		{
			CSendItem* pItem = m_SIList.GetNext(pos);
			if(pItem->m_nKey==_pItem->m_nKey)
			{
				pItem->Copy(_pItem);
				return 0;
			}
		}
		CSendItem *pItem = new CSendItem;
		pItem->Copy(_pItem);
		m_SIList.AddTail(pItem);
		return 1;
	}
	else
	{
		POSITION tpos;
		for(POSITION pos=m_SIList.GetHeadPosition(); pos;)
		{
			tpos = pos;
			CSendItem* pItem = m_SIList.GetNext(pos);
			if(pItem->m_nKey==_pItem->m_nKey)
			{
				delete pItem;
				m_SIList.RemoveAt(tpos);
				return -1;
			}
		}
		return 0;	//리스트에 없음..변동없음.
	}
	return 0;
}

void CViewWrap::_DoClearSendItem()
{
	for(POSITION pos=m_SIList.GetHeadPosition(); pos;)
	{
		CSendItem* pItem = m_SIList.GetNext(pos);
		delete pItem;
	}
	m_SIList.RemoveAll();
}

CSendItem* CViewWrap::FindSendItem(int nKey)
{
	for(POSITION pos=m_SIList.GetHeadPosition(); pos;)
	{
		CSendItem* pItem = m_SIList.GetNext(pos);
		if(pItem->m_nKey==nKey) return pItem;
	}
	return NULL;
}

BOOL CViewWrap::_DoDirectSendTR(CString strTR, char *szData, int nLen, int nKey)
{
	char *pszBuff = NULL;

	struct _userTH user;
	strncpy(&user.trc[0], strTR, 8);

	//<<20100319_JS.Kim FX TR은 secondary session으로 요청한다.
	//user.path = ssMASTER;
//KHD
//	if( strTR.Left(2) == TRMODE_FX )
//		user.path = ssSEC1;
//	else
//		user.path = ssMASTER;
//END
	//>>
	user.key  = nKey;
	user.stat = US_KEY;
//KHD	user.size = nLen;

	//@100118-alzioyes
	int nSize = sizeof(_userTH) + nLen;
	pszBuff = new char[nSize+1];

	memcpy(pszBuff, (char*)&user, sizeof(_userTH));
	memcpy(pszBuff+sizeof(_userTH), szData, nLen);
//	strTRData = CString((char*)&user, sizeof(_userTH));
//	strTRData += CString(szData, nLen);

	BOOL bRet = ::SendMessage(GetParent()->GetSafeHwnd(), WM_USER, MAKEWPARAM(invokeTRx, nLen), (LPARAM)pszBuff);

	delete []pszBuff;
	pszBuff = NULL;

	return bRet;
}

LONG CViewWrap::OnRmsgSendMessage(WPARAM wParam, LPARAM lParam)
{
	BYTE bt = LOBYTE(LOWORD(wParam));
	switch(bt)
	{
		case 1:
			{
				CString szData = (LPCSTR)lParam;
				CSendItem SendInfo;
				int nRet = _DoParsingSendData(szData, SendInfo);
				if(nRet>0 && SendInfo.m_nLen>0)
				{
//					_DoDirectSendTR(SendInfo.m_szTR, (LPSTR)(LPCSTR)szData.Mid(nRet), SendInfo.m_nLen, SendInfo.m_nKey);
					_DoDirectSendTR(SendInfo.m_szTR, (char*)(lParam+nRet), SendInfo.m_nLen, SendInfo.m_nKey);
				}
			}
			break;
	}
	return 0L;
}

#include "../../inc/IDebugMng.h"
struct _trkey
{
	UINT kind:5;
	UINT group:3;
};
IDebugManager* g_pDebugMng = NULL;
LONG CViewWrap::OnUser(WPARAM wParam, LPARAM lParam)
{
	int nKey = 0;
	int nLen = 0;
	LONG	ret = 0;
	BYTE bt = LOBYTE(LOWORD(wParam));
	switch(bt)
	{
	case DLL_OUBx:
		{
#if(0)
			char* pBytes = (char *)lParam;
			struct _extTH* extTH = (struct _extTH *)pBytes;
			pBytes += L_extTH;

			int nKey = extTH->key;
			int nLen = extTH->size;

			wParam = MAKEWPARAM(MAKEWORD(DLL_OUB, nKey), nLen);
			lParam = (LPARAM)pBytes;
#else
			_extTHx* info = (_extTHx*)lParam;
			const int nLen = info->size;
			char* pBytes = info->data;
			int nKey = info->key;

			wParam = MAKEWPARAM(MAKEWORD(DLL_OUB, nKey), nLen);
			lParam = (LPARAM)pBytes;
#endif
		}

	case DLL_OUB:			// Tr수신		
		{
			WORD kind = HIBYTE(LOWORD(wParam));
			struct _trkey* trkey = (struct _trkey*)&kind;
			nKey = kind;
			nLen = (long)HIWORD(wParam);

			//--> 0027629: [0000] 서버로 부터 조회되는 차트모듈 관한 확인 및 수정요청
			if ((nKey >= TRKEY_MEMO_POPUPOOP && nKey <= TRKEY_MEMO_CHECK))
			{
				if (m_pPtMng && IsWindow(m_hWnd))
				{
					OutputDebugString("[ViewWrap]-[DLL_OUB]");
					IPartnerSite* pSite = m_pPtMng->GetSite();
					if (pSite) pSite->ReceiveOnUser(wParam, lParam);
				}
				break;
			}
			//<--
			
			if(nLen==1)
			{
				wParam = MAKEWPARAM(LOWORD(wParam), HIWORD(wParam) - SZ_ZCOMHEAD);
				nLen = HIWORD(wParam);
			}
			if(!g_pDebugMng)
				g_pDebugMng = (IDebugManager*)AfxGetPctrInterface(UUID_IDebugManager);
			if(g_pDebugMng)
				g_pDebugMng->TraceTran('O', "OnUser", nLen, (LPBYTE)(lParam+sizeof(_zcom_head)));
			
			LPBYTE pRBuff = new BYTE[nLen+1];
			if( nKey == 2 )
			{
				// MSTCode일때는 패킷 사이즈가 커서 위닉스 서버에서 분할해서 전송한다.
				// 그러면 메인에서 패킷을 받은후 화면에 전송하는데 이때 패킷 사이즈는 마지막에 받은 패킷사이즈로
				// 전송해서 실제 받은 패킷사이즈와 차이가 발생한다. 그래서 실제 받아서 처리하는 부분에서
				// 사이즈를 다시 계산하후 처리를 한다.
				if(m_pPtMng) m_pPtMng->ReceiveTRData("", (LPBYTE)(lParam), nLen, nKey);
				delete [] pRBuff;
				((CTrMChartView*)m_pView)->m_IsLoading = FALSE;

				return 1L;
			}
			else if(nKey ==50 || nKey == 24/* 심볼코드(POOP) */ || (nKey > 70 &&nKey <80))// KHD : 호가는 PooPPoop이기때문에 zComHeader를 쓰지 않는다. 71~ 79 시스템 트레이딩 Key값 
			{
				memcpy(pRBuff, (char*)(lParam), nLen);
				pRBuff[nLen] = 0;
			}
			else
			{
				int zcomSize = sizeof(_zcom_head);
				
				if(zcomSize < nLen)
				{
					memset(pRBuff,0x00, sizeof(pRBuff));
					memcpy(pRBuff, (char*)lParam + SZ_ZCOMHEAD, nLen - zcomSize); //KHD : _zcom_head 쓰지도 않는데 더해줘야 한다. 
					pRBuff[nLen- zcomSize] = 0;
				}
				else 
				{
					delete []pRBuff;
					break;
				}
			}
			
				
			if (nKey == TRKEY_SYSTEMAGREE)
			{
				KillTimer(510);

				if (nLen <= 0)
					CloseScreen();

				char *pszData = (char*)(lParam + SZ_ZCOMHEAD);

				if ((pszData[1] == '0') && (pszData[0] != '1'))
				{
					if (m_pPtSvrMng)
					{
						IPartnerUtilManager* pPtUtilMng = (IPartnerUtilManager*)m_pPtSvrMng->GetInerInterface(LUID_IPartnerUtilManager);
						if (pPtUtilMng)
							pPtUtilMng->OpenScreen(GetParent()->GetSafeHwnd(), _T("SI051090"));
					}
					CloseScreen();
					delete [] pRBuff;
					break;
				}

				if (!CreateView(RUNTIME_CLASS(CTrMChartView)))
					CloseScreen();

				delete [] pRBuff;
				break;
			}

			//TreeListMng.dll 에서의 빠른종목 예외처리
			if(nKey==TRKEY_CONDITION || (TRKEY_GWANSIMGROUP<=nKey && nKey<=TRKEY_GWANSAVE) || nKey == TRKEY_ACCOUNTINFO)
			{
				CString strTR;
				switch(nKey)
				{
					case TRKEY_CONDITION:	strTR = TRNO_CONDIONECLK_RESULT;	break;
				}
				CSendItem* pItem = FindSendItem(nKey);
				if(pItem && pItem->m_pCallback)
				{
					ITrCommSite* pSite = (ITrCommSite*)pItem->m_pCallback;
					if(pSite)
						pSite->ReceiveData(nKey, strTR, "", "", (char*)lParam, nLen);
					delete [] pRBuff;
					return 1L;
				}
			}
			else if( nKey == ACC_REV_KEY1 || nKey == ACC_REV_KEY2 )	// 계좌콘트롤용
			{
				const UINT RMSG_ACCOUNT_INIT = ::RegisterWindowMessage(_T("RMSG_ACCOUNT_INIT"));
				CTrMChartView *pTRView = (CTrMChartView*)m_pView;
				if(pTRView->m_pChartScreen && IsWindow(pTRView->m_pChartScreen->GetSafeHwnd()))
					pTRView->m_pChartScreen->SendMessage(RMSG_ACCOUNT_INIT, wParam, lParam);
			}

			if(m_pPtMng) m_pPtMng->ReceiveTRData("", pRBuff, nLen, nKey);
			delete [] pRBuff;
			((CTrMChartView*)m_pView)->m_IsLoading = FALSE;
			
			return 1L;
		}
		
		break;

//-->100309 리얼처리방식의 변경.
//	DLL_ALERT 대신에 DLL_ALERTx 사용하는 것으로 루틴 변경함.
//	case DLL_ALERT:			// 리얼 수신
//		{
//	//			{
//	//				CString szDebug;
//	//				szDebug.Format("ViewWrap [%08X], [%08X]\n", m_hWnd, m_pPtMng);
//	//				OutputDebugString(szDebug);
//	//			}
//
//@100309			if(m_pPtMng && IsWindow(m_hWnd)) m_pPtMng->ReceiveRealData(wParam, lParam);
//		}
//		break;
	case DLL_ALERTx:
		if (m_pPtMng)
		{
			// 요청한 종목코드의 실시간인지 체크.
			if (!((CTrMChartView*)m_pView)->m_IsLoading)
			{
				struct _alertR* alertR = (struct _alertR*)lParam;

				if (_DoCheckAllertCode(alertR->code))
				{
//	지우지말것,DRFN Platform 로그기능추가, 필요시엔만 사용하고 배포할 때는 막을 것]
/*
					if (!g_pDebugMng)
						g_pDebugMng = (IDebugManager*)AfxGetPctrInterface(UUID_IDebugManager);
					if (g_pDebugMng)
						g_pDebugMng->TraceTran('I', "DLL_ALERTx RealCode", alertR->code.GetLength(), (LPBYTE)(LPCSTR)(LPCTSTR)(alertR->code));
*/
					if (IsWindow(m_hWnd))
						m_pPtMng->ReceiveRealDataEx(wParam, lParam);
				}
			}
		}

		break;
//<--100309 리얼처리방식의 변경.

	case DLL_SETPAL:
		{
			const UINT RMSG_CHANGE_SKINCOLOR = ::RegisterWindowMessage(_T("RMSG_CHANGE_SKINCOLOR"));
			CTrMChartView *pTRView = (CTrMChartView*)m_pView;
			pTRView->m_pChartScreen->PostMessage(RMSG_CHANGE_SKINCOLOR, 0, 0);
		}
		break;
	case DLL_DOMINO:		// 화면 열리면서 데이터 공유 받을 때
		if (m_pPtMng && IsWindow(m_hWnd))
		{
			IPartnerSite* pSite = m_pPtMng->GetSite();
			if (pSite) pSite->ReceiveOnUser(wParam, lParam);
		}

// 		{
// 			CString szData((char*)lParam), strTemp, strTemp2, strKey, strValue;
// 			CMapStringToString strMap;
// 			strTemp = szData;
// 
// 			int nFind = strTemp.Find(_T("\n"));
// 			while (nFind>=0)
// 			{
// 				strTemp2 = strTemp.Left(nFind);
// 				strTemp = strTemp.Mid(nFind+1);
// 				nFind = strTemp2.Find(_T("\t"));
// 				if (nFind >= 0)
// 				{
// 					strKey = strTemp2.Left(nFind);
// 					strValue = strTemp2.Mid(nFind+1);
// 					strMap.SetAt(strKey, strValue);
// 
// 					if (strKey[0]>='1' && strKey[0]<='9')
// 					{
// 						int nKey = IPartnerSite::IPS_RK_shareCode;
// 						if (m_pPtMng && IsWindow(m_hWnd))
// 						{
// 							if (strKey.Find(_T("301")) > 0)
// 							{
// 								IPartnerSite* pSite = m_pPtMng->GetSite();
// 								if(pSite) pSite->RecieveKeyData(nKey, strTemp2);
// 							}
// 						}
// 					}
// 				}
// 
// 				nFind = strTemp.Find(_T("\n"));
// 			}
// 		}

		break;
	case DLL_GUIDE:
		return 1;
	case DLL_SETFONTx:
		break;
	case DLL_TRIGGER:
		{
			CString szData((char*)lParam);
			int nPos = szData.Find('\t');
			if(nPos<1) return 0L;

			CString szSymbol = (LPSTR)(LPCSTR)szData.Left(nPos);
			CString szValue  = (LPSTR)(LPCSTR)szData.Mid(nPos+1);

			if( strcmp(szSymbol, "DRFNGWAN")==0) //관심관련 메시지이면
			{
				UINT nMsg = atoi((LPCSTR)szValue);
				switch (nMsg)
				{
				case WM_NOTIFY_LOAD:					// 관심그룹 Load (전체)
					break;
				case WM_NOTIFY_SAVE:					// 관심그룹 Save (전체 or 1그룹)
					{
						int nKey = IPartnerSite::IPS_RK_gwansim;
						if(m_pPtMng && IsWindow(m_hWnd)) 
						{
							IPartnerSite* pSite = m_pPtMng->GetSite();
							if(pSite) pSite->RecieveKeyData(nKey, szData);
						}
					}
					break;
				case WM_NOTIFY_GROUP:					// 관심그룹 Load (1그룹)
					break;
				case WM_NOTIFY_GROUPLIST:				// 관심그룹 Load (그룹리스트 - 종목 제외)
					break;
				case WM_NOTIFY_SAVECODE:				// 관심그룹에 종목 추가 (1종목)
					break;
				}
			}
			else if(szSymbol[0]>='1' && szSymbol[0]<='9')
			{
				int nKey = IPartnerSite::IPS_RK_shareCode;
				if(m_pPtMng && IsWindow(m_hWnd)) 
				{
					if( strstr(szData, "1301\t")==0  ||
						strstr(szData, "20301\t")==0 ||
						strstr(szData, "30301\t")==0 ||
						strstr(szData, "50301\t")==0)
					{
						IPartnerSite* pSite = m_pPtMng->GetSite();
						if(pSite) pSite->RecieveKeyData(nKey, szData);
					}
				}
			}
			else if(szSymbol[0]>='c' && szSymbol[0]<='o')
			{
				int nKey = IPartnerSite::IPS_RK_shareCode;
				if(m_pPtMng && IsWindow(m_hWnd)) 
				{
					if( strstr(szData, "focod\t")==0 ||
						strstr(szData, "fcod\t")==0 ||
						strstr(szData, "cfcod\t")==0 ||
						strstr(szData, "ocod\t")==0 ||
						strstr(szData, "gscode\t")==0)
					{
						IPartnerSite* pSite = m_pPtMng->GetSite();
						if(pSite) 
							pSite->RecieveKeyData(nKey, szData);
					}
				}
			}

		}
		break;
	case DLL_CODE: // 종목다이얼로그에서 선택한 코드
		if(m_pPtMng && IsWindow(m_hWnd)) 
		{
			int nKey = IPartnerSite::IPS_RK_codeDlg;
			IPartnerSite* pSite = m_pPtMng->GetSite();
			if(pSite) pSite->RecieveKeyData(nKey, (char*)lParam);
		}
		break;

	case DLL_NOTICE:	// 체결데이타. 해외선물(8041)과 FX(9041)에서만 처리한다.
		{
		CTrMChartView *pTRView = (CTrMChartView*)m_pView;
		if( pTRView )
		{
			CString strFormCode = pTRView->m_strFormCode;
			if( strFormCode == "8041" || strFormCode == "9041" )
			{
				if(m_pPtMng && IsWindow(m_hWnd)) m_pPtMng->ReceiveNoticeData((WPARAM)strFormCode.operator LPCTSTR(), lParam);
			}
			}
		}
		break;
	case DLL_INIT :		// Init 시점.. 계좌콘트롤 Init해준다.
		{
		const UINT RMSG_ACCOUNT_INIT = ::RegisterWindowMessage(_T("RMSG_ACCOUNT_INIT"));
		CTrMChartView *pTRView = (CTrMChartView*)m_pView;
		if(pTRView && IsWindow(pTRView->GetSafeHwnd()) && pTRView->m_pChartScreen && IsWindow(pTRView->m_pChartScreen->GetSafeHwnd()))
			pTRView->m_pChartScreen->SendMessage(RMSG_ACCOUNT_INIT, 0, 0);
		}
		break;

	}

	return 0L;
}

//100530:동일화면 다르게 저장/읽기-->
// --------------------------------------------------------------------------------------
// [100512수 winix의 정학석과장에게 받은 내용.]
// [주의] 본 통보 메세지 및 트리거는 DLL화면인 경우에만 발생합니다.
// 
//   1. 최종화면 또는, 사용자 화면 저장 시, 아래 메세지 발생
//   
// 	#define WM_DLLDATA    WM_USER + 9111
// 	SendMessage(WM_DLLDATA, 0, lParam);
// 	
// 	  lParam = USERDEFDATA\t구분,가상화면번호,개별화면/독립화면구분,일련번호
// 	  (1) 구분(LASTSTAT : 최종 저장화면, USERSCREEN : 사용자 화면)
// 	  (2) 가상화면번호
// 	  (3) 개별화면/독립화면 구분(0 : 개별화면, 1 : 독립화면)
// 	  (4) 일련번호
// 	  
// 		2. "WM_DLLDATA" 메세지 Event Handler에서 환경 파일 생성 저장
// 		(1) $UID 생성
// 		lParam을 조합하여 해당 화면별로 식별가능한 Unique한 파일명을 만든다.
// 		
// 		  (2) 생성 파일명을 저장한다.
// 		  - 최종화면 : $HOME/user/LASTSTAT/$UID.INI
// 		  - 사용자화면 : $HOME/user/USERSCREEN/$UID.INI
// 		  
// 			(3) 환경 관련 내용을 INI 파일에 저장한다.
// 			
// 			  3. 화면 오픈 시, 도미노 데이터로 [2번] lParam과 동일 포멧으로 전달 됨.
// 			  (1) 도미노 데이터 : USERDEFDATA\t구분,가상화면번호,개별화면/독립화면구분,일련번호
// 			  (2) 데이터 조합해서 저장 시, 사용한 Unique한 파일명을 만든다.
// 			  (3) 저장 위치에서 해당 환경 파일을 읽어 환경설정을 한다.
// 			  
// 				4. 저장 폴더의 메인에서의 관리
// 				(1) 최종 화면이 없는 경우, $HOME/user/LASTSTAT 폴더를 삭제 한다.
//  (2) 사용자 화면이 삭제 되는 경우, $HOME/user/USERSCREEN 폴더를 삭제 한다.
// --------------------------------------------------------------------------------------
void CViewWrap::SetStartInfo(void* pData)
{
// 	{
// 		CString szDebug;
// 		szDebug.Format("[DRCS][LOAD](%8d) CViewWrap::SetStartInfo: Begin\n", __LINE__);
// 		OutputDebugString(szDebug);
// 	}

// 	  (1) 구분(LASTSTAT : 최종 저장화면, USERSCREEN : 사용자 화면)
// 	  (2) 가상화면번호
// 	  (3) 개별화면/독립화면 구분(0 : 개별화면, 1 : 독립화면)
// 	  (4) 일련번호
	CString strIs; 
	strIs = (char*)pData;
	//OutputDebugString("KHD : " + strIs);
	CString szParam = (char*)pData;
	if(strIs.Find("USERSCREEN") < 0 && strIs.Find("LASTSTAT") < 0)
	{
		if(strIs.Find("1301") < 0  &&														 //주식			
			strIs.Find("gicode")   && strIs.Find("20301") < 0 &&								 //업종
			strIs.Find("ed_fcode") && strIs.Find("30301") < 0 && strIs.Find("40301") < 0  && //선물옵션
			strIs.Find("gscode")   &&														 //해외
			strIs.Find("ed_fcod") < 0 && strIs.Find("100301") < 0 &&					  	 //상품선물
			 strIs.Find("ed_focod") ) //??
		{
			//return;
			szParam = "";
		}
		else
		{
			if(strIs.Find("\n") > -1)
			{
				int nIndex = strIs.Find("\n");
				
				CString strDomino = strIs.Mid(0,nIndex);
				
				szParam = strDomino;
			}
			else if(strIs.Find("\t") > -1)
			{				
				CString strDomino = strIs;
				
				szParam = strDomino;
			}
		}
	//	szParam = "";
	}

	m_szStartInfo = (char*)pData;
//	OutputDebugString("KHD : End : " + m_szStartInfo );
	
//	int nPos = szInfo.Find('\t');
//	if(nPos<0) return;
//	szInfo = szInfo.Mid(nPos+1);
//	char aParam1[20+1], aParam2[3+1], aParam3[3+1], aParam4[3+1];
//	sscanf((LPCSTR)szInfo, "%s,%s,%s,%s", aParam1, aParam2, aParam3, aParam4);
	int nPos = szParam.Find("\t");
	if(nPos<0) return;

	szParam = szParam.Mid(nPos+1);
	nPos = szParam.Find('X');
	if(nPos == 0)
		szParam.Delete(0);

	//OutputDebugString("KHD : End2222 : " + szParam );
	//nPos = szParam.Find(",");
	//CString szSaveKey1;
	//szSaveKey1 = szParam.Left(nPos);
	m_szFileKey = szParam;
	m_szFileKey.Remove(',');

// 	{
// 		CString szDebug;
// 		szDebug.Format("[DRCS][LOAD](%8d) CViewWrap::SetStartInfo: End(%s)\n", __LINE__);
// 		OutputDebugString(szDebug);
// 	}
}

#include <DIRECT.H>
LONG CViewWrap::OnWinixDllData(WPARAM wParam, LPARAM lParam)
{
//return 0L;	//위닉스 플랫폼 수정중이라 끝난 후 작업할 것임.

	if(!lParam) return 0L;

	CString szParam = (char*)lParam;
	int nPos = szParam.Find(",");
	CString szSaveKey1;
	szSaveKey1 = szParam.Left(nPos);
	szParam.Remove(',');
	
	IPartnerSite* pSite = m_pPtMng->GetSite();
	if(pSite) pSite->RecieveKeyData(IPartnerSite::IPS_RK_SaveKeyEnd, (LPCSTR)szParam);

// 		IMainInfoManager* pMainMng = (IMainInfoManager*)AfxGetPctrInterface(UUID_IMainInfoManager);
// 		CString szUserDir;
// 		if(!pMainMng) return 0L;
// 
// 		szUserDir.Format("%s\\%s", pMainMng->GetEncUserDir(), szSaveKey1);
// 		::mkdir(szUserDir);
// 
// 		CString szUserIni;
// 		szUserIni.Format("%s\\%s.INI", szUserDir, szParam);
// 
// 		//::WritePrivateProfileString("A", "B", "C", szUserIni);

	return 0L;
}
//100530:동일화면 다르게 저장/읽기 <--

void CViewWrap::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == 510)
	{
		KillTimer(nIDEvent);

		CloseScreen();
	}

	CWnd::OnTimer(nIDEvent);
}

void CViewWrap::CloseScreen()
{
	::PostMessage(GetParent()->GetSafeHwnd(), WM_USER, MAKEWPARAM(closeDLL, 0), (LPARAM)0);
}

// Real데이터에서 사용하지 않는 호가는 체크해서 리턴처리하도록 함.
// 호가사용조건 : 주식, FO
// 해외는 필요한지 몰라 일단 기존처럼 전부전달.
// IMasterDataManagerLast*	g_pMasterDataMng = NULL;
// BOOL CViewWrap::_DoCheckHogaReal(WPARAM wParam, LPARAM lParam)
// {
// 	m_pPtMng->ReceiveRealDataEx(wParam, lParam);
// 	return TRUE;
// 	struct _alertR* alertR = (struct _alertR*)lParam;
// 	CString szCode = alertR->code;
// 	if( szCode.IsEmpty() )
// 	{
// 		return FALSE;
// 	}
// 
// 	if(!g_pMasterDataMng) g_pMasterDataMng = (IMasterDataManagerLast*)AfxGetPctrInterface(UUID_IMasterDataManager);
// 	int nMarketType = (int)g_pMasterDataMng->GetMarketTypeFromCode(szCode);
// 
// 	switch(nMarketType)
// 	{
// 		case CMasterTypeInfo::STOCK_CHART:
// 		case CMasterTypeInfo::FUOPT_JIPYO_CHART:
// 		case CMasterTypeInfo::FOREIGN_CHART:
// 		case CMasterTypeInfo::FO_FOREIGN_CHART:
// 		case CMasterTypeInfo::FX_CHART:
// 			m_pPtMng->ReceiveRealDataEx(wParam, lParam);
// 			return TRUE;
// 	}
// 
// 	CTrMChartView *pTRView = (CTrMChartView*)m_pView;
// 	return TRUE;
// }

// 위닉스의 리얼체계는 내가 요청하지 않은 종목에 대해서도 실시간 데이터를 무조건 준다고 함.
// 그래서 개별 화면에서 각자 걸러야 함.
BOOL CViewWrap::_DoCheckAllertCode(CString szCode)
{
	if (szCode.Left(1) == "X" && szCode.Find('@') == -1)	//IBK에서 쓸데없는 실시간 필터링 하는 루틴!!
		return FALSE;

	if (m_pView && (((CTrMChartView*)m_pView)->m_nEnumScreenType == ST_FOREIGNCHART ||
					((CTrMChartView*)m_pView)->m_nEnumScreenType == ST_FXCHART))
	{
		return TRUE;
	}
	
	//종목은 앞의 A제거하고 확인, 업종은 리얼에 별로 영향이 없으므로 바로 리턴.
	if (szCode.Left(1) == "A" || szCode.Left(1) == "Q")
		szCode = szCode.Mid(1);

//	if (szCode.Left(1) == "K")	//KHD ??
//		return TRUE;

	if (m_pPtMng->IsRegisterCode(szCode) == FALSE)
		return FALSE;

	return TRUE;
}


