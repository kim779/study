// GwanWnd.cpp : implementation file
//

#include "stdafx.h"
#include "PcTrMng.h"
#include "GwanWnd.h"
#include "../../inc/IGateMng.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CGwanWnd

CGwanWnd::CGwanWnd()
{
//	m_pGwansimList = NULL;
	m_nAdviseGwan = 0;
}

CGwanWnd::~CGwanWnd()
{
}

#define		WM_EUGENE_GWANSIM		(WM_USER + 525)

BEGIN_MESSAGE_MAP(CGwanWnd, CWnd)
	//{{AFX_MSG_MAP(CGwanWnd)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_EUGENE_GWANSIM, OnEugeneGwansim)
	END_MESSAGE_MAP()


//	ON_WM_TIMER()
	//{{관심
//	ON_REGISTERED_MESSAGE(RMSG_CONCERNHWND, OnConcernHWnd)
//	ON_REGISTERED_MESSAGE(RMSG_INTREQUEST, OnIntRequest)
	//}}관심


/////////////////////////////////////////////////////////////////////////////
// CGwanWnd message handlers
//BOOL CGwanWnd::Create(CWnd* pParentWnd)
//{
//	if(!CWnd::Create(NULL,"GwansimManagerWindow",
//		WS_CHILD,
//		CRect(0,0,0,0), pParentWnd,500))
//		return FALSE;
//
//	// SetTimer(10, 3000, NULL);
//
//	m_this = m_hWnd;
//	return TRUE;
//}


BOOL CGwanWnd::OpenWindow()
{
	CString		szClassName;	
	
	szClassName =	AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS, 
		0, 0, 0);
//	int rc = CreateEx(0L,														//dwExStyle, 
//					(LPCSTR)szClassName,
//					"",
//					WS_VISIBLE,	// | WS_CHILD, 
//					0, 0, 0, 0, 
//					HWND_DESKTOP, 
//					NULL,
//					NULL);
//	if(rc) {
//		ShowWindow(SW_HIDE);
//		m_this = m_hWnd;
//	}
//	return rc;

	if (!CWnd::CreateEx(WS_EX_TOPMOST, szClassName, _T(""), 
		WS_POPUP, 
		0, 0, 0, 0, // size & position updated when needed
		NULL, 0, NULL))
	{
		return FALSE;
	}
	return TRUE;

//	CString ClassName;
//	ClassName =	AfxRegisterWndClass(CS_DBLCLKS, 0, 0, 0);
//
//	DWORD dwStyle = WS_POPUP|WS_DISABLED;
//	DWORD dwExStyle = WS_EX_TOOLWINDOW ;
//
//	int rc = CreateEx(	dwExStyle,			//dwExStyle, 
//						ClassName.operator const char *(),
//						_T("PctrGwanWnd"),
//						dwStyle, // | WS_CHILD, 
//						0,0,0,0, 
//						NULL,
//						NULL,
//						NULL);
//	if(rc) ShowWindow( SW_HIDE );     
//
//	return rc;
}


void CGwanWnd::GwanAdvise()
{
// 		m_nAdviseGwan++;
// 		if(m_nAdviseGwan==1)
// 		{
// 			int nType = 1;	// 0:해제, 1:등록
// 			theApp.m_GwanMng.SetRegGwansim(this, nType);
// 		}
}

void CGwanWnd::GwanUnAdvise(BOOL bClearAll)
{
// 		if(bClearAll)
// 		{
// 			ClearAllList();
// 			m_nAdviseGwan = 0;
// 			int nType = 0;	// 0:해제, 1:등록
// 			theApp.m_GwanMng.SetRegGwansim(this, nType);
// 			return;
// 		}
// 
// 		m_nAdviseGwan--;
// 		if(m_nAdviseGwan<0)
// 		{
// 			m_nAdviseGwan = 0;
// 			return;
// 		}
// 
// 		if(m_nAdviseGwan==0)
// 		{
// 			int nType = 0;	// 0:해제, 1:등록
// 			theApp.m_GwanMng.SetRegGwansim(this, nType);
// 		}
}

BOOL CGwanWnd::DestroyWindow() 
{
	if(m_nAdviseGwan>0)
	{
		TRACE("\nError! [PcTrMng CGwanWnd::DestroyWindow] UnAdvise가 다 불리지 않음.\n");
		GwanUnAdvise(TRUE);
	}
	
	return CWnd::DestroyWindow();
}


// LRESULT CGwanWnd::OnConcernHWnd(WPARAM wParam, LPARAM lParam)
// {
// 	NMHDR* mh = (NMHDR*)lParam;
// 
// 	if(wParam == 1 || wParam == 0)				// 관심 반영 핸들등록/삭제
// 	{
// 		// 메인에서는 이부분을 처리함.
// 	}
// 	// 관심 변경을 알림
// 	else
// 	{
// 		RequestIntAll();
// 	}
// 
// 	return 0L;
// }

// void CGwanWnd::RequestIntAll()
// {
// 	INT_TOTAL IntTotal;
// 	::memset(&IntTotal, 0x00, sizeof(INT_TOTAL));
// 	::memcpy(IntTotal.Igubun, REQUEST_INTTOTAL, sizeof(IntTotal.Igubun));
// 
// 	HWND hMainHwnd = theApp.m_hMainHwnd; // AfxGetPctrMainHandle();
// 
// 	::SendMessage(hMainHwnd, RMSG_INTREQUEST, (WPARAM)m_this, (LPARAM)&IntTotal);
// }


// LRESULT CGwanWnd::OnIntRequest(WPARAM wParam, LPARAM lParam)
// {
// 	if(lParam==NULL) return 0;
// 
// 	bool bNotify = true;
// //	if(m_pGwansimList==NULL) bNotify = false;
// //
// //	RemoveAllList(m_pGwansimList);
// //	SetGwansimToPctr(wParam, lParam);
// //
// //	if(bNotify==false) return 0;
// 	
// 	long dwSubKey = 0;
// 	long dwData = (long)lParam;
// 	//theApp.m_pDataMonMng->NotifyData(DataMon_SZGWANSIM, dwSubKey, (long)&dwData);
// 	theApp.m_pDataMonMng->NotifyData(DataMon_SZGWANSIM, dwSubKey, dwData);
// 
// 	return 0;
// }

// void CGwanWnd::SetGwansimToPctr(WPARAM wParam, LPARAM lParam)
// {
//	INT_TOTAL* pMngData = new INT_TOTAL;
//	INT_TOTAL* pData = (INT_TOTAL*)lParam;
//
//	ZeroMemory(pMngData, SIZE_INT_TOTAL);
//	CopyIntTotal(pData, pMngData);
//
//	m_pGwansimList = pMngData;
//	COleVariant varCode((long)pMngData);
//	theApp.m_MainExinfoMng.SetData(IMainExInfoManager_Gwansim, &varCode);
// }
// 
// void CGwanWnd::CopyIntTotal(void* _pFrom, void* _pTo)
// {
//	INT_TOTAL* pFrom = (INT_TOTAL*)_pFrom;
//	INT_TOTAL* pTo  = (INT_TOTAL*)_pTo;
//
//	memcpy(pTo, pFrom, SIZE_INT_TOTAL);
//
//	pTo->Idata = (void*)new CPtrArray;
//	CopyIntGroupSt((CPtrArray*)pFrom->Idata, (CPtrArray*)pTo->Idata);
// }
// 
// void CGwanWnd::CopyIntGroupSt(CPtrArray* pGroupFrom, CPtrArray* pGroupTo)
//{
//	CPtrArray* pFromGroupAll = (CPtrArray*)pGroupFrom;
//
//	if(pFromGroupAll) {
//		int nSize = pFromGroupAll->GetSize();
//		//pGroupTo->SetSize(nSize);
//
//		INT_GROUP_ST* pIntGroup = NULL;
//
//		for(int nIndx = 0; nIndx < nSize; ++nIndx)
//		{
//			INT_GROUP_ST* pNewToGroup = new INT_GROUP_ST;
//			ZeroMemory(pNewToGroup, SIZE_INT_GROUP_ST);
//
//			pIntGroup = (INT_GROUP_ST*)pFromGroupAll->GetAt(nIndx);
//			memcpy(pNewToGroup, pIntGroup, SIZE_INT_GROUP_ST);
//
//			pNewToGroup->Idata = (void*)new CPtrArray;
//			CopyIntItemSt((CPtrArray*)pIntGroup->Idata, (CPtrArray*)pNewToGroup->Idata);
//			//pGroupTo->SetAt(nIndx, pNewToGroup);
//			pGroupTo->Add(pNewToGroup);
//		}
//	}
//}

//void CGwanWnd::CopyIntItemSt(CPtrArray* pFrom, CPtrArray* pTo)
//{
//	int nSize = pFrom->GetSize();
//	// pTo->SetSize(nSize);
//
//	INT_ITEM_ST* pItem = NULL;
//	for(int nIndx = 0; nIndx < nSize; ++nIndx)
//	{
//		INT_ITEM_ST* pNewItem = new INT_ITEM_ST;
//		ZeroMemory(pNewItem, SIZE_INT_ITEM_ST);
//
//		pItem = (INT_ITEM_ST*)pFrom->GetAt(nIndx);
//		memcpy(pNewItem, pItem, SIZE_INT_ITEM_ST);
//		//pTo->SetAt(nIndx, pNewItem);
//		pTo->Add(pNewItem);
//	}
//}


// void CGwanWnd::RemoveAllList(void* _pData)
// {
//	if(!_pData) return;
//
//	INT_TOTAL* pData = (INT_TOTAL*)_pData;
//	CPtrArray* pArGroupAll = (CPtrArray*)pData->Idata;
//	if(pArGroupAll) {
//		int nSize = pArGroupAll->GetSize();
//		INT_GROUP_ST* pIntGroup = NULL;
//
//		for(int nIndx = 0; nIndx < nSize; ++nIndx)
//		{
//			pIntGroup = (INT_GROUP_ST*)pArGroupAll->GetAt(nIndx);
//
//			CPtrArray* pGroupItem = (CPtrArray*)pIntGroup->Idata;
//			int nSubSize = pGroupItem->GetSize();
//
//			INT_ITEM_ST* pItem = NULL;
//			for(int nSubIndx = 0; nSubIndx < nSubSize; ++nSubIndx)
//			{
//				if(pItem = (INT_ITEM_ST*)pGroupItem->GetAt(nSubIndx)) {
//					delete pItem;
//				}
//			}
//			pGroupItem->RemoveAll();
//			delete pGroupItem;
//			delete pIntGroup;
//		}
//		pArGroupAll->RemoveAll();
//		delete pArGroupAll;
//	}
//	delete pData;
//	_pData = NULL;


//	
//	INT_TOTAL* pIntAll = (INT_TOTAL*)pData;
//	char sBuff[10];
//
//	memcpy(sBuff,pIntAll->Icnt,sizeof(pIntAll->Icnt));
//	sBuff[sizeof(pIntAll->Icnt)]=NULL;
//	int nCnt = atoi(sBuff);
//	CPtrArray* pArr = (CPtrArray*)pIntAll->Idata;	
//	if(!pArr) return;
//	if(pArr->GetSize() != nCnt)
//		nCnt = pArr->GetSize();
//
//	for(int i=0;i<nCnt;i++)
//	{
//		INT_GROUP_ST* pIntGroup = (INT_GROUP_ST*)pArr->GetAt(i);
//		memset(sBuff,0x00,sizeof(sBuff));
//		memcpy(sBuff,pIntGroup->Icnt,sizeof(pIntGroup->Icnt));
//		sBuff[sizeof(pIntGroup->Icnt)]=NULL;
//		int nGCnt = atoi(sBuff);
//		CPtrArray* pGArr = (CPtrArray*)pIntGroup->Idata;
//		if(!pGArr) continue;
//		if(pGArr->GetSize() != nGCnt)
//			nGCnt = pGArr->GetSize();
//		for(int i=0;i<nGCnt;i++)
//			delete (INT_ITEM_ST*)pGArr->GetAt(i);
//		delete pGArr;
//		delete pIntGroup;
//	}
//	delete pArr;
//	delete pIntAll;

//	pData = NULL;
// }

// void CGwanWnd::OnTimer(UINT nIDEvent) 
// {
// 	KillTimer(nIDEvent);
// 	GwanAdvise();
// }


// LParam이 변경된 그룹코드
LRESULT CGwanWnd::OnEugeneGwansim(WPARAM wParam, LPARAM lParam)
{
	long dwSubKey = 0;
	long dwData = (long)lParam;
 //	theApp.m_pDataMonMng->NotifyData(DataMon_SZGWANSIM, dwSubKey, (long)&dwData);
	return 0L;
}

void CGwanWnd::ClearList(int nIndex)
{

}

void CGwanWnd::ClearAllList()
{

}
