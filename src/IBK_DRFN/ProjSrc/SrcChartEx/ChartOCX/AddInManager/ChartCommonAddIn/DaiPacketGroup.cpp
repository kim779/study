// DaiPacketGroup.cpp: implementation of the CDaiPacketGroup class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DaiPacketGroup.h"

#include "DaiCodeGroup.h"		// for CDaiCodeGroup

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDaiPacketGroup::CDaiPacketGroup( const char *p_szRealPacketName)
{
	// (2004.11.29, 배승원) 관리중인 DRDS Advised Info의 Packet 명을 관리한다.
	m_strRealPacketName = p_szRealPacketName;
}

CDaiPacketGroup::~CDaiPacketGroup()
{
	// (2004.11.29, 배승원) 등록된 모든 DRDS Advised Info를 제거한다.
	RemoveAllDrdsAdvisedInfo();
}

//////////////////////////////////////////////////////////////////////
// (2004.11.29, 배승원) DRDS Advised Info를 구성하는 Interface를 제공한다.
//////////////////////////////////////////////////////////////////////

// 1. 1개의 DRDS Advised Info Data를 추가하는 Interface를 제공한다.
BOOL CDaiPacketGroup::AddDrdsAdvisedInfo( IPacket *p_pIPacket, const char *p_szRealFieldName, const char *p_szRealKeyCode)
{
	// 1. Real Key Code Group이 등록되어 있지 않는 경우에는 새로 생성한다.
	POSITION posDaiCodeGroup = FindDaiCodeGroup( p_szRealKeyCode);
	if( !posDaiCodeGroup)
	{
		AddTail( new CDaiCodeGroup( p_szRealKeyCode));
		posDaiCodeGroup = GetTailPosition();
	}

	// 2. 검색된 Real Key Code Group에 새로운 DRDS Advised Info를 구성하여 등록한다.
	return GetAt( posDaiCodeGroup)->AddDrdsAdvisedInfo( p_pIPacket, p_szRealFieldName);
}

// RQ별 DRDS관리 : 복수종목 - ojtaso (20070111)
BOOL CDaiPacketGroup::AddDrdsAdvisedInfo(LPCTSTR lpszRQ, IPacket *p_pIPacket, const char *p_szRealFieldName, const char *p_szRealKeyCode)
{
	// 1. Real Key Code Group이 등록되어 있지 않는 경우에는 새로 생성한다.
	POSITION posDaiCodeGroup = FindDaiCodeGroup(p_szRealKeyCode);
	if( !posDaiCodeGroup)
	{
		AddTail( new CDaiCodeGroup( p_szRealKeyCode));
		posDaiCodeGroup = GetTailPosition();
	}

	// 2. 검색된 Real Key Code Group에 새로운 DRDS Advised Info를 구성하여 등록한다.
	return GetAt( posDaiCodeGroup)->AddDrdsAdvisedInfo(lpszRQ, p_pIPacket, p_szRealFieldName);
}

//////////////////////////////////////////////////////////////////////
// (2004.11.29, 배승원) 구성된 DRDS Advised Info를 제거하는 Interface를 제공한다.
//////////////////////////////////////////////////////////////////////

// 1. 일괄 제거하는 Interface를 제공한다.
void CDaiPacketGroup::RemoveAllDrdsAdvisedInfo( void)
{
	while( !IsEmpty()) delete RemoveHead();
}

//////////////////////////////////////////////////////////////////////
// (2004.11.29, 배승원) 구성된 DRDS Advised Info를 검색하는 Interface를 제공한다.
//////////////////////////////////////////////////////////////////////

// 1. Drds Advised Info 값을 지정받아 검색하는 Inteface를 제공한다.
POSITION CDaiPacketGroup::FindDaiCodeGroup( const char *p_szRealKeyCode)
{
	// (2006/4/20 - Seung-Won, Bae) RTrim for space.  After Advise with space, DRDS Send WM_DRDS_PACKET_END without space
	CString strAdviseKeyCode, strRealKeyCode( p_szRealKeyCode);
	strRealKeyCode.TrimRight( ' ');

	POSITION posPrevDaiCodeGroup;
	POSITION posDaiCodeGroup = GetHeadPosition();
	while( posDaiCodeGroup)
	{
		posPrevDaiCodeGroup = posDaiCodeGroup;
		strAdviseKeyCode = GetNext( posDaiCodeGroup)->m_strRealKeyCode;
		strAdviseKeyCode.TrimRight( ' ');
		if( strAdviseKeyCode == strRealKeyCode) return posPrevDaiCodeGroup;
	}
	return NULL;
}

//////////////////////////////////////////////////////////////////////
// (2004.12.01, 배승원) Real Packet의 수신여부를 관리하는 Interface를 제공한다.
//		(전 Packet의 수신시를 확인하여, Drawing의 별도 처리를 하기 위함이이다.)
//////////////////////////////////////////////////////////////////////

// 1. Real Packet의 수신여부를 설정하는 Interface를 제공한다.
//		(이때, 전 Packet의 수신여부를 확인하여 Return한다.)
BOOL CDaiPacketGroup::SetPacketReceivedFlag( const char *p_szRealKeyCode)
{
	// 1. Real Key Code Group이 등록되어 있지 않는 경우에는 새로 생성한다.
	POSITION posDaiCodeGroup = FindDaiCodeGroup( p_szRealKeyCode);
	if( !posDaiCodeGroup) return FALSE;

	// 2. 검색된 Real Key Code Group에 새로운 DRDS Advised Info를 구성하여 등록한다.
	GetAt( posDaiCodeGroup)->m_bPacketReceived = TRUE;
	return TRUE;
}

// 2. 모든 Real Packet이 수신되었는지를 확인하는 Interface를 제공한다.
BOOL CDaiPacketGroup::HasOneMoreReceiveForEachPacket( void)
{
	POSITION posDaiCodeGroup = GetHeadPosition();
	while( posDaiCodeGroup) if( !GetNext( posDaiCodeGroup)->m_bPacketReceived) return FALSE;
	return TRUE;
}

// 3. 모든 Real Packet의 수신 여부 Flag을 초기화 처리하는 Interface를 제공한다.
void CDaiPacketGroup::ResetPacketReceiveFlag( void)
{
	POSITION posDaiCodeGroup = GetHeadPosition();
	while( posDaiCodeGroup) GetNext( posDaiCodeGroup)->m_bPacketReceived = FALSE;
}
