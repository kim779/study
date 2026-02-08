// DrdsAdvisedInfoList.cpp: implementation of the CDrdsAdvisedInfoList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DrdsAdvisedInfoList.h"	// for CDrdsAdvisedInfoList

#include "DaiPacketGroup.h"		// for CDaiPacketGroup

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDrdsAdvisedInfoList::CDrdsAdvisedInfoList()
{

}

CDrdsAdvisedInfoList::~CDrdsAdvisedInfoList()
{
	// (2004.11.29, 배승원) 등록된 모든 DRDS Advised Info를 제거한다.
	RemoveAllDrdsAdvisedInfo();
}

//////////////////////////////////////////////////////////////////////
// (2004.11.29, 배승원) DRDS Advised Info를 구성하는 Interface를 제공한다.
//////////////////////////////////////////////////////////////////////

// 1. DRDS Advised Info Packet Group을 추가하는 Interface를 제공한다.
BOOL CDrdsAdvisedInfoList::AddDaiPacketGroup( const char *p_szRealPacketName)
{
	// 0. 이미 등록된 경우를 제외한다.
	if( FindDaiPacketGroup( p_szRealPacketName)) return FALSE;

	// 1. 새로운 DRDS Advised Info Packet Group을 등록시킨다.
	AddTail( new CDaiPacketGroup( p_szRealPacketName));
	return TRUE;
}

// 2. DRDS Advised Info를 추가하는 Interface를 제공한다.
BOOL CDrdsAdvisedInfoList::AddDrdsAdvisedInfo( IPacket *p_pIPacket, const char *p_szRealPacketName, const char *p_szRealFieldName, const char *p_szRealKeyCode)
{
	// 0. 아직 DRDS Advised Info Packet Group이 등록되지 않은 경우를 제외한다.
	POSITION posDaiPacketGroup = FindDaiPacketGroup( p_szRealPacketName);
	if( !posDaiPacketGroup) return FALSE;

	// 1. DRDS Advised Info Packet Group을 구하여 DRDS Advised Info를 등록한다.
	return GetAt( posDaiPacketGroup)->AddDrdsAdvisedInfo( p_pIPacket, p_szRealFieldName, p_szRealKeyCode);
}

// RQ별 DRDS관리 : 복수종목 - ojtaso (20070111)
BOOL CDrdsAdvisedInfoList::AddDrdsAdvisedInfo(LPCTSTR lpszRQ, IPacket *p_pIPacket, const char *p_szRealPacketName, const char *p_szRealFieldName, const char *p_szRealKeyCode)
{
	if(!lpszRQ)
		return AddDrdsAdvisedInfo(p_pIPacket, p_szRealPacketName, p_szRealFieldName, p_szRealKeyCode);

	// 0. 아직 DRDS Advised Info Packet Group이 등록되지 않은 경우를 제외한다.
	POSITION posDaiPacketGroup = FindDaiPacketGroup( p_szRealPacketName);
	if( !posDaiPacketGroup) return FALSE;

	// 1. DRDS Advised Info Packet Group을 구하여 DRDS Advised Info를 등록한다.
	return GetAt( posDaiPacketGroup)->AddDrdsAdvisedInfo(lpszRQ, p_pIPacket, p_szRealFieldName, p_szRealKeyCode);
}

//////////////////////////////////////////////////////////////////////
// (2004.11.29, 배승원) 구성된 DRDS Advised Info를 제거하는 Interface를 제공한다.
//////////////////////////////////////////////////////////////////////

// 1. 일괄 제거하는 Interface를 제공한다.
void CDrdsAdvisedInfoList::RemoveAllDrdsAdvisedInfo( void)
{
	while( !IsEmpty())
	{
		CDaiPacketGroup *pDaiPacketGroup = RemoveHead();
		pDaiPacketGroup->RemoveAllDrdsAdvisedInfo();
		delete pDaiPacketGroup;
	}
}

//////////////////////////////////////////////////////////////////////
// (2004.11.29, 배승원) 구성된 DRDS Advised Info를 검색하는 Interface를 제공한다.
//////////////////////////////////////////////////////////////////////

// 1. DRDS Advised Info Packet Group을 검색하는 Inteface를 제공한다.
//		(중복 등록을 방지하기 위하여 이용된다.)
POSITION CDrdsAdvisedInfoList::FindDaiPacketGroup( const char *p_szRealPacketName)
{
	POSITION posPrevDaiPacketGroup;
	POSITION posDaiPacketGroup = GetHeadPosition();
	while( posDaiPacketGroup)
	{
		posPrevDaiPacketGroup = posDaiPacketGroup;
		CDaiPacketGroup *pDaiPacketGroup = GetNext( posDaiPacketGroup);
		if( pDaiPacketGroup->m_strRealPacketName == p_szRealPacketName) return posPrevDaiPacketGroup;
	}
	return NULL;
}

//////////////////////////////////////////////////////////////////////
// (2004.12.01, 배승원) Real Packet의 수신여부를 관리하는 Interface를 제공한다.
//		(전 Packet의 수신시를 확인하여, Drawing의 별도 처리를 하기 위함이이다.)
//////////////////////////////////////////////////////////////////////

// 1. Real Packet의 수신여부를 설정하는 Interface를 제공한다.
//		(이때, 전 Packet의 수신여부를 확인하여 Return한다.)
BOOL CDrdsAdvisedInfoList::SetPacketReceivedFlag( const char *p_szRealPacketName, const char *p_szRealKeyCode)
{
	// 1. 먼저 DRDS Advised Info Packet Group을 검색한다.
	POSITION posDaiPacketGroup = FindDaiPacketGroup( p_szRealPacketName);
	if( !posDaiPacketGroup) return FALSE;

	// 2. DRDS Advised Info Packet Group을 구하여 해당 Real Packet의 수신 여부를 설정한다.
	CDaiPacketGroup *pDaiPacketGroup = GetAt( posDaiPacketGroup);
	if( !pDaiPacketGroup->SetPacketReceivedFlag( p_szRealKeyCode)) return FALSE;

	return HasOneMoreReceiveForEachPacket();
}

// 2. 모든 Real Packet이 수신되었는지를 확인하는 Interface를 제공한다.
BOOL CDrdsAdvisedInfoList::HasOneMoreReceiveForEachPacket( void)
{
	POSITION posDaiPacketGroup = GetHeadPosition();
	while( posDaiPacketGroup) if( !GetNext( posDaiPacketGroup)->HasOneMoreReceiveForEachPacket()) return FALSE;
	return TRUE;
}

// 3. 모든 Real Packet의 수신 여부 Flag을 초기화 처리하는 Interface를 제공한다.
void CDrdsAdvisedInfoList::ResetPacketReceiveFlag( void)
{
	POSITION posDaiPacketGroup = GetHeadPosition();
	while( posDaiPacketGroup) GetNext( posDaiPacketGroup)->ResetPacketReceiveFlag();
}
