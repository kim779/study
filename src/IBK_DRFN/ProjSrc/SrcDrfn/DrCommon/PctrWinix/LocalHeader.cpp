#include "stdafx.h"
//#include "PctrWinix.h"
#include "LocalHeader.h"


/////////////////////////////////////////////////////////////////////////////
// CWinixGwanGroupList
CWinixGwanGroupList::CWinixGwanGroupList()
{

}

CWinixGwanGroupList::~CWinixGwanGroupList()
{
	_DoClearList();	
}

void CWinixGwanGroupList::_DoClearList()
{
	for(POSITION pos=m_List.GetHeadPosition(); pos;)
		delete m_List.GetNext(pos);
	m_List.RemoveAll();
}


/////////////////////////////////////////////////////////////////////////////
// CWinixGwanItem
CWinixGwanItem::CWinixGwanItem(HWND hWinix, HWND hDrfnWrap, HWND hGwanReciever)
{
	m_hWinix	= hWinix;
	m_hDrfnWrap = hDrfnWrap;
	m_hGwanReciever = hGwanReciever;
	//m_nQueryState = wgi_ready;

	// 2012.2.7 박성경: 관심 솔로몬
	//m_pIM= new CFxInterestManager();
}

CWinixGwanItem::~CWinixGwanItem()
{
	// 2012.2.7 박성경: 관심 솔로몬
	//if(m_pIM) delete m_pIM; m_pIM=NULL;
}


void CWinixGwanItem::DoInitialize(BOOL bCreate)
{
	if(bCreate)
	{
		CWnd* pWizard = CWnd::FromHandle(m_hWinix);
		CWnd* pDRfnWrap = CWnd::FromHandle(m_hDrfnWrap);
		CWnd* pGwanReciever = CWnd::FromHandle(m_hGwanReciever);

		// 2012.2.7 박성경: 관심 솔로몬 >>
		//m_pIM = new CFxInterestManager();
		// 2012.2.7 박성경: 관심 솔로몬 <<
		//m_pIM->Initialize(pWizard, pDRfnWrap);
		// 2012.2.7 박성경: 관심 솔로몬 >>
		//m_pIM->Initialize(pWizard, pGwanReciever);	//나중에 메시지를 받기위해 pGwanReciever등록
		// 2012.2.7 박성경: 관심 솔로몬 <<

//		RequestGwansimGroup(this);
// 		{
// 			CString strTRData = "";
// 			strTRData.Format("%s\t%08X\t%d\t%d\t%s"	
// 				,"GwanSim"
// 				, &m_xTrCommSite
// 				, TRKEY_GWANSIMGROUP
// 				, 0			//Size
// 				, " ");		//Data
// 			pDRfnWrap->GetParent()->SendMessage(RMSG_SENDMESSAGE, MAKEWPARAM(1, 0), (LPARAM)(LPCSTR)strTRData);
// 		}
// 		//m_nQueryState = wgi_querygroup;
// 		m_pIM->QueryGroupList(TRKEY_GWANSIMGROUP);
	}
	// 2012.2.7 박성경: 관심 솔로몬 >>
// 	else
// 	{
// 		if(m_pIM) delete m_pIM;
// 		m_pIM=NULL;
// 	}
	// 2012.2.7 박성경: 관심 솔로몬 <<
}

void CWinixGwanItem::RequestGwansimGroup()
{
	int sendL = 0;
	char sendB[16 * 1024] = { 0, };
	int isz_updn = sz_uinfo;
	struct _updn* updn = (struct _updn*)&sendB[sendL];
	sendL += isz_updn;
	FillMemory((char*)updn, isz_updn, ' ');
	CopyMemory(updn->uinfo.gubn, "MY", sizeof(updn->uinfo.gubn));
	updn->uinfo.dirt[0] = 'U';
	updn->uinfo.cont[0] = 'g';
	CopyMemory(updn->uinfo.nblc, "00001", sizeof(updn->uinfo.nblc));
	updn->uinfo.retc[0] = 'U';
	sendL = strlen(sendB);

	CString strTRData = "";

	//strWinix.Format("%s%80s%s", "MYUg", " ", "00001U");
	strTRData.Format("%s\t%08X\t%d\t%d\t%s"
			, TRNO_FAVORITE
			, &m_xTrCommSite
			, TRKEY_GWANSIMGROUP
			, sendL
			, sendB);		//Data
		
		CWnd* pDRfnWrap = CWnd::FromHandle(m_hDrfnWrap);
		//pDRfnWrap->GetParent()->SendMessage(RMSG_SENDMESSAGE, MAKEWPARAM(1, 0), (LPARAM)(LPCSTR)strTRData);
		pDRfnWrap->SendMessage(RMSG_SENDMESSAGE, MAKEWPARAM(1, 0), (LPARAM)(LPCSTR)strTRData);
	
	// 2012.2.7 박성경: 관심 솔로몬
	//m_pIM->QueryGroupList(TRKEY_GWANSIMGROUP);
}


#define DR_MYMETHOD_PROLOGUE(theClass, localClass) \
	theClass* pThis = \
	((theClass*)((BYTE*)this - offsetof(theClass, m_x##localClass))); \
pThis;

// HRESULT CWinixGwanItem::CTrCommSite::ReceiveData(long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDateLen)
// {
// 	DR_MYMETHOD_PROLOGUE(CWinixGwanItem, TrCommSite)
// 
// 	int nQueryState = pThis->m_nQueryState;
// 	if(dwKey==TRKEY_GWANSIM)
// 	{
// 		pThis->m_nQueryState = wgi_ready;
// 		switch(nQueryState)
// 		{
// 			case wgi_querygroup:
// 				pThis->m_pIM->ReceiveGroupList((char*)aTRData, dwTRDateLen);
// 				break;
// 			case wgi_queryitem:
// 				pThis->m_pIM->ReceiveGroup((char*)aTRData, dwTRDateLen);
// 				break;
// 		}
// 		OutputDebugString("관심 OK\n");
// 		return S_OK;
// 	}
// 
// 	return  S_OK;
// }

HRESULT CWinixGwanItem::CTrCommSite::ReceiveData(long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDateLen)
{
	DR_MYMETHOD_PROLOGUE(CWinixGwanItem, TrCommSite)
		
	//int nQueryState = pThis->m_nQueryState;

	if(dwKey==TRKEY_GWANSIMGROUP)
	{
		// 2012.2.7 박성경: 관심 솔로몬
		//pThis->m_pIM->ReceiveGroupList((char*)aTRData, dwTRDateLen);
		{
			//관심그룹을 읽어가라고 알려준다.
			//::SendMessage(pThis->m_hGwanReciever, WM_NOTIFY_GROUP, 999, 999);
		//tour2k	::SendMessage(pThis->m_hGwanReciever, WM_NOTIFY_SAVE, 0, 0);
                        ::SendMessage(pThis->m_hGwanReciever, WM_NOTIFY_SAVE, 0, (LPARAM)(char*)aTRData);
			//GetGwansimGroupName(long dwRet, CStringArray &arrKey, CStringArray &arrName);
		}		
	}
	else if(dwKey==TRKEY_GWANSIMITEM)
	{
		// 2012.2.7 박성경: 관심 솔로몬
		//pThis->m_pIM->ReceiveGroup((char*)aTRData, dwTRDateLen);
	//	::SendMessage(pThis->m_hGwanReciever, WM_NOTIFY_GROUP, 0, 0);
                ::SendMessage(pThis->m_hGwanReciever, WM_NOTIFY_GROUP, 0, (LPARAM)(char*)aTRData);
	}
	else if(dwKey==TRKEY_GWANSAVE)
	{
		// 2012.2.7 박성경: 관심 솔로몬
		//pThis->m_pIM->ReceiveSaveCode((char*)aTRData, dwTRDateLen);
	}
	
	return  S_OK;
}

HRESULT CWinixGwanItem::CTrCommSite::StatusChange(int nState){return  S_OK;}
HRESULT CWinixGwanItem::CTrCommSite::ReceiveMessage(long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage){return  S_OK;}
HRESULT CWinixGwanItem::CTrCommSite::ReceiveRealData(WORD wFID, LPCSTR szMainKey, void* pRealData){	return S_OK;}

