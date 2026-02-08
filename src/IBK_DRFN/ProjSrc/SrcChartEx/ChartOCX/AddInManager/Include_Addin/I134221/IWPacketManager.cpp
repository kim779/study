// IWPacketManager.cpp: implementation of the CIWPacketManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IWPacketManager.h"

#include "_IPacketList.h"			// for IPacketList
#include "IWPacket.h"				// for CIWPacket
#include "IWHelpMessageMap.h"		// for CIWHelpMessageMap
#include "IWString.h"				// for CIWString
#include "Packetbase.h"				// for CPacket
// PacketListManager : 복수종목 - ojtaso (20070110)
#include "PacketListManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// _IWRAPPER_IUNKNOWN_MANAGER_IMP
//////////////////////////////////////////////////////////////////////
_IWRAPPER_IUNKNOWN_MANAGER_IMP( CIWPacketManager)

//////////////////////////////////////////////////////////////////////
// [04/10/11] Packet Object의 Interface를 구하는 Interface를 제공한다.
//////////////////////////////////////////////////////////////////////

// [04/10/11] Packet Name으로 Packet Object를 구하는 Interface를 제공한다.
IPacket *CIWPacketManager::GetPacket( const char *p_szPacketName)
{
	if( !m_pPacketList) return NULL;
	CPacket *pPacket = m_pPacketList->GetBaseData( p_szPacketName);
	if( !pPacket) return NULL;
	return CIWPacket::CreateObject( pPacket);
}

// RQ별 Packet관리 : 복수종목 - ojtaso (20070112)
IPacket *CIWPacketManager::GetPacket(LPCTSTR lpszRQ, const char *p_szPacketName)
{
	if(!m_pPacketListManager)
		return GetPacket(p_szPacketName);

	CPacketList* pPacketList = m_pPacketListManager->GetPacketList(lpszRQ);
	if( !pPacketList) return NULL;

	CPacket *pPacket = pPacketList->GetBaseData( p_szPacketName);
	if( !pPacket) return NULL;
	return CIWPacket::CreateObject(lpszRQ, pPacket);
}

// (2005/9/2 - Seung-Won, Bae) Get Packet Interface List for Packet Names
BOOL CIWPacketManager::GetPacketList( const char *p_szPacketNames, IPacketList *p_lIPacketList)
{
	if( !m_pPacketList || !p_lIPacketList) return FALSE;

	CList<CPacket*, CPacket*> lPackets;
	if( !m_pPacketList->GetPacketList( p_szPacketNames, ";", lPackets)) return FALSE;

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

// [04/11/30] Packet Object의 Memory Address으로 Packet Object를 구하는 Interface를 제공한다.
IPacket *CIWPacketManager::GetPacketInterface( unsigned long p_ulPacketAddress)
{
	if( !m_pPacketList) return NULL;
	if( !p_ulPacketAddress) return NULL;
	return CIWPacket::CreateObject( ( CPacket *)p_ulPacketAddress);
}

// (2006/1/9 - Seung-Won, Bae) Get Packet Interface List from CPacket List
//					 ~ CObject * <= const CList<CPacket*, CPacket*> *
BOOL CIWPacketManager::GetIPacketListFromCPacketList( CObject *p_plCPacket, IPacketList *p_lIPacketList)
{
	const CList<CPacket*, CPacket*> *plCPacketList = ( const CList<CPacket*, CPacket*> *)p_plCPacket;
	if( !plCPacketList || !p_lIPacketList) return FALSE;
	POSITION pos = plCPacketList->GetHeadPosition();
	if( !pos) return FALSE;
	CPacket *pPacket = NULL;
	while( pos)
	{
		pPacket = plCPacketList->GetNext(pos);
		p_lIPacketList->AddTail( CIWPacket::CreateObject( pPacket));
	}
	return TRUE;
}

// RQ별 Packet관리 : 복수종목 - ojtaso (20070112)
BOOL CIWPacketManager::GetIPacketListFromCPacketList(LPCTSTR lpszRQ, CObject *p_plCPacket, IPacketList *p_lIPacketList)
{
	const CList<CPacket*, CPacket*> *plCPacketList = ( const CList<CPacket*, CPacket*> *)p_plCPacket;
	if( !plCPacketList || !p_lIPacketList) return FALSE;
	POSITION pos = plCPacketList->GetHeadPosition();
	if( !pos) return FALSE;
	CPacket *pPacket = NULL;
	IPacket *pIPacket = NULL;
	while( pos)
	{
		pPacket = plCPacketList->GetNext(pos);
		p_lIPacketList->AddTail( CIWPacket::CreateObject(lpszRQ, pPacket));
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// [04/10/13] 보조 Message를 검색하는 Interface를 제공한다. (최근에 수신된 TR에 제한된다.)
//////////////////////////////////////////////////////////////////////
BOOL CIWPacketManager::GetHelpMessageItem( const char *p_szMsgKey, ILPCSTR &p_pszMessage)
{
	p_pszMessage = NULL;
	if( !m_pPacketList) return FALSE;
	CString strMessage;
	BOOL bResult = m_pPacketList->GetHelpMessageItem( p_szMsgKey, strMessage);
	p_pszMessage = CIWString::CreateObject( strMessage);
	return bResult;
}

// [05/09/29] Get Help Message List
IHelpMessageMap *CIWPacketManager::GetHelpMessageMap( void)
{
	if( !m_pPacketList) return NULL;
	const CMapStringToString *pHelpMessageMap = m_pPacketList->GetHelpMessageMap();
	if( !pHelpMessageMap) return NULL;
	return CIWHelpMessageMap::CreateObject( pHelpMessageMap);
}

//////////////////////////////////////////////////////////////////////
// (2006/6/22 - Seung-Won, Bae) Get Maximum Data Count
//////////////////////////////////////////////////////////////////////

// (2006/6/22 - Seung-Won, Bae) Get RealUpdated Data Index
int	CIWPacketManager::GetLastestRealIndex( void)
{
	if( !m_pPacketList) return -1;
	return m_pPacketList->GetLastestRealIndex();
}

// (2006/6/22 - Seung-Won, Bae) Get Maximum Data Count
long CIWPacketManager::GetMaximumDataCount( void)
{
	if( !m_pPacketList) return -1;
	return m_pPacketList->GetMaxCount();
}
long CIWPacketManager::GetMaximumDataCount( const char *p_szPacketNames, const char *p_szCompartString)
{
	if( !m_pPacketList) return -1;
	return m_pPacketList->GetMaxCount( p_szPacketNames, p_szCompartString);
}

// (2006/6/26 - Seung-Won, Bae) Get the type of BaseTime
CScaleBaseData::HORZSCALEDRAWERTYPE	CIWPacketManager::GetBaseTimeDataType( void)
{
	if( !m_pPacketList) return CScaleBaseData::HORZ_NOTYPE;
	return m_pPacketList->GetDateUnitType();
}
int CIWPacketManager::GetBaseTimeUnit( void)
{
	if( !m_pPacketList) return 0;
	return m_pPacketList->GetTimeInterval();
}
// (2006/7/26 - Seung-Won, Bae) Retrieve the flag of Real Usage
BOOL CIWPacketManager::IsInUseRealData( void)
{
	if( !m_pPacketList) return FALSE;
	return m_pPacketList->UseRealData();
}
// (2006/8/15 - Seung-Won, Bae) Retrieve Main Item Type
IPacketManager::ICOMMODITY_BOND_NAME CIWPacketManager::GetCommodityBondNameInBase( void)
{
	if( !m_pPacketList) return INONAME;
	return ( ICOMMODITY_BOND_NAME)m_pPacketList->GetCommodityBondNameInBase();
}

//>> Gap Revision - vntsorl(20090213)
ILPCSTR CIWPacketManager::GetTRPacketNames( const char *p_szCompart)
{
	if( !m_pPacketList) return NULL;
	CString strTRPacketNames;
	strTRPacketNames = m_pPacketList->GetTRPacketNames( p_szCompart);
	return CIWString::CreateObject( strTRPacketNames);
}

// (2008/11/3 - Seung-Won, Bae) Support User TimeScale Setting.
int CIWPacketManager::GetCalcDataTimeDifference( void)
{
	if( !m_pPacketList) return FALSE;
	return m_pPacketList->GetCalcDataTimeDifference();
}

// PacketListManager Setting : 복수종목 - ojtaso (20070307)
void CIWPacketManager::SetPacketListManager(LPVOID lpPacketListManager)
{
	m_pPacketListManager = (CPacketListManager*)lpPacketListManager;
}
