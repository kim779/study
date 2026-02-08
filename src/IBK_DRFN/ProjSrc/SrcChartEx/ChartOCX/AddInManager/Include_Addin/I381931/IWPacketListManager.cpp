// IWPacketListManager.cpp: implementation of the CIWPacketListManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IWPacketListManager.h"

#include "../../../Include_Chart/DLL_Load/PacketList.h"		// for CPacketList
#include "../../../Include_Chart/DLL_Load/PacketBase.h"		// for CPacket
#include "IWPacket.h"										// for CIWPacket
#include "IWStringList.h"									// for CIWStringList


//////////////////////////////////////////////////////////////////////
// _IWRAPPER_IUNKNOWN_MANAGER_IMP
//////////////////////////////////////////////////////////////////////
_IWRAPPER_IUNKNOWN_MANAGER_IMP( CIWPacketListManager)

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


/*


// RQ별 Packet : 복수종목 - ojtaso (20070111)
BOOL CIWPacketManager::GetPacketList(LPCTSTR lpszRQ, const char *p_szPacketNames, IPacketList *p_lIPacketList)
{
	if( !m_pPacketListManager)
		return GetPacketList(p_szPacketNames, p_lIPacketList);

	CPacketList* pPacketList = m_pPacketListManager->GetPacketList(lpszRQ);

	CList<CPacket*, CPacket*> lPackets;
	if( !pPacketList->GetPacketList( p_szPacketNames, ";", lPackets)) return FALSE;

	CPacket *pPacket = NULL;
	IPacket *pIPacket = NULL;
	POSITION posPacket = lPackets.GetHeadPosition();
	while( posPacket)
	{
		pPacket = lPackets.GetNext( posPacket);
		p_lIPacketList->AddTail( CIWPacket::CreateObject( pPacket));
	}

	return TRUE;
}


// RQ별 Packet : 복수종목 - ojtaso (20070111)
BOOL CIWPacketManager::GetHelpMessageItem(LPCTSTR lpszRQ, const char *p_szMsgKey, ILPCSTR &p_pszMessage)
{
	if( !m_pPacketListManager)
		return GetHelpMessageItem(p_szMsgKey, p_pszMessage);

	p_pszMessage = NULL;
	CPacketList* pPacketList = m_pPacketListManager->GetPacketList(lpszRQ);

	CString strMessage;
	BOOL bResult = pPacketList->GetHelpMessageItem( p_szMsgKey, strMessage);
	p_pszMessage = CIWString::CreateObject( strMessage);
	return bResult;
}





*/