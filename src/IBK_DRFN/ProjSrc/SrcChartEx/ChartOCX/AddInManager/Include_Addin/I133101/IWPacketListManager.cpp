// IWPacketListManager.cpp: implementation of the CIWPacketListManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IWPacketListManager.h"

#include "../../../Include_Chart/DLL_Load/PacketList.h"		// for CPacketList
#include "../../../Include_Chart/DLL_Load/PacketBase.h"		// for CPacket
#include "IWPacket.h"										// for CIWPacket
#include "IWStringList.h"									// for CIWStringList
#include "IWPacketManager.h"								// for CIWPacketManager


//////////////////////////////////////////////////////////////////////
// _IWRAPPER_IUNKNOWN_MANAGER_IMP
//////////////////////////////////////////////////////////////////////
_IWRAPPER_IUNKNOWN_MANAGER_IMP( CIWPacketListManager)


// (2008/10/14 - Seung-Won, Bae) Retrieve PacketManager
IPacketManager *CIWPacketListManager::GetPacketManager( const char *p_szRQ)
{
	if( !m_pPacketListManager) return NULL;
	
	CPacketList* pPacketList = m_pPacketListManager->GetPacketList( p_szRQ);
	if( !pPacketList) return NULL;

	return CIWPacketManager::CreateObject( pPacketList);
}


// RQ별 Packet관리 : 복수종목 - ojtaso (20070112)
IPacket *CIWPacketListManager::GetPacket( LPCTSTR lpszRQ, const char *p_szPacketName)
{
	if( !m_pPacketListManager) return NULL;
	
	CPacketList* pPacketList = m_pPacketListManager->GetPacketList( lpszRQ);
	if( !pPacketList) return NULL;

	CPacket *pPacket = pPacketList->GetBaseData( p_szPacketName);
	if( !pPacket) return NULL;

	return CIWPacket::CreateObject( pPacket);
}

IStringList *CIWPacketListManager::GetRQByItemCode( LPCTSTR lpszItemCode)
{
	if( !m_pPacketListManager) return NULL;

	CIWStringList::CStringTList *pStringTList = new CIWStringList::CStringTList;
	m_pPacketListManager->GetRQByItemCode( lpszItemCode, *pStringTList);

	if( pStringTList->IsEmpty())
	{
		delete pStringTList;
		return NULL;
	}

	return CIWStringList::CreateObject( pStringTList, TRUE);
}

// 2008.08.07 김진순 종목간 Ratio chart 모든 RQ의 이름을 구해온다		<<
IStringList *CIWPacketListManager::GetAllRQ( void)
{
	if( !m_pPacketListManager) return NULL;

	CIWStringList::CStringTList *pStringTList = new CIWStringList::CStringTList;
	m_pPacketListManager->GetAllRQ( *pStringTList);

	return CIWStringList::CreateObject( pStringTList, TRUE);
}

// 2008.08.07 김진순 종목간 Ratio chart 모든 RQ의 이름을 구해온다		<<
void CIWPacketListManager::GetItemCode(LPCTSTR lpszRQ, LPCTSTR& lpszItemCode, LPCTSTR& lpszItemName)
{
	if(m_pPacketListManager)
		m_pPacketListManager->GetItemCode(lpszRQ, lpszItemCode, lpszItemName);
}

// 현재가 등락률 - ojtaso (20080602)
void CIWPacketListManager::OffsetPacketData(LPCTSTR lpszRQ, double dData, CString strPacketName/* = _T("")*/)
{
	if( !m_pPacketListManager) return;

	if(dData == 0.)
		return;

	m_pPacketListManager->OffsetPacketData(lpszRQ, dData, strPacketName);
}

// (2009/2/16 - Seung-Won, Bae) Support Non-Time X Scale Manager.
BOOL CIWPacketListManager::IsBasePacketString( void)
{
	if( !m_pPacketListManager) return FALSE;
	return m_pPacketListManager->IsBasePacketString();
}

// 20081007 JS.Kim	사용자시간대 선택
// 일자시간 Packet 가져오기
IPacket *CIWPacketListManager::GetDatePacket(LPCTSTR lpszRQ)
{
	if( !m_pPacketListManager) return NULL;

	CPacket *pPacket = m_pPacketListManager->GetBasePacket( lpszRQ);
	if( !pPacket) return NULL;

	return CIWPacket::CreateObject( pPacket);
}

