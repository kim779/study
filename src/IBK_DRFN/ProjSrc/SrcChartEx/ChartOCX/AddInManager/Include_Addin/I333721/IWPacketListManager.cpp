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

// RQ별로 RealData 처리 : 복수종목 - ojtaso (20070110)
BOOL CIWPacketListManager::SetRealData( LPCTSTR lpszRQ, BOOL& bUpdate, IPacket *p_pIPacket, double p_dData, int &p_nRemovePacketDataCount)
{
	ASSERT( FALSE);
	return FALSE;		// (2009/10/19 - Seung-Won, Bae) Make CIWPacketListManager::SetRealData( LPCTSTR lpszRQ, BOOL& bUpdate, const char *p_szPacketName, double p_dData, int &p_nRemovePacketDataCount)
}

// RQ별로 RealData 처리 : 복수종목 - ojtaso (20070110)
BOOL CIWPacketListManager::SetRealData( LPCTSTR lpszRQ, BOOL& bUpdate, IPacket *p_pIPacket, const char *p_szField, const char *p_szKey, double p_dData, int &p_nRemovePacketDataCount)
{
	ASSERT( FALSE);
	return FALSE;		// (2009/10/19 - Seung-Won, Bae) Use CIWPacketListManager::SetRealData( LPCTSTR lpszRQ, BOOL& bUpdate, IPacket *p_pIPacket, double p_dData, int &p_nRemovePacketDataCount)
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
void CIWPacketListManager::GetAllRQ(CStringArray& arRQ)
{
	if( !m_pPacketListManager) return;

	CList < CString, CString> slStringTList;
	m_pPacketListManager->GetAllRQ( slStringTList);
	POSITION psn = slStringTList.GetHeadPosition();
	while( psn) arRQ.Add( slStringTList.GetNext( psn));
}
void CIWPacketListManager::GetItemCode(LPCTSTR lpszRQ, LPCTSTR& lpszItemCode, LPCTSTR& lpszItemName)
{
	if(m_pPacketListManager)
		m_pPacketListManager->GetItemCode(lpszRQ, lpszItemCode, lpszItemName);
}

// 20081024 JS.Kim	허수봉을 ocx 및 AddIn 으로 주기위해 허수봉의 시간을 실봉시간으로 환산한다.
bool CIWPacketListManager::CalcNMTimeToRealTime( LPCTSTR lpszRQ, IPacket *p_pIPacket, double p_dData, double& dRealTime, double& dCloseData)
{
	if( !m_pPacketListManager) return false;

	CPacketList* pPacketList = m_pPacketListManager->GetPacketList( lpszRQ);
//	if( pPacketList )
//		return pPacketList->CalcNMTimeToRealTime( ( ( CIWPacket *)p_pIPacket)->GetPacketPtr(), p_dData, dRealTime, dCloseData);

	return false;
}

// 20081027 JS.Kim	RealData 받고 안받고의 처리를 종목별로 한다
bool CIWPacketListManager::IsInUseRealData( LPCTSTR lpszRQ)
{
	if( !m_pPacketListManager) return false;

	CPacketList* pPacketList = m_pPacketListManager->GetPacketList( lpszRQ);
	if( pPacketList )		return pPacketList->UseRealData();

	return false;
}

