// DaiCodeGroup.cpp: implementation of the CDaiCodeGroup class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DaiCodeGroup.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDaiCodeGroup::CDaiCodeGroup( const char *p_szRealKeyCode)
{
	// (2004.11.29, 배승원) 관리중인 DRDS Advised Info의 Real Key Code 값을 관리한다.
	m_strRealKeyCode = p_szRealKeyCode;

	// (2004.12.01, 배승원) Real Packet의 수신여부를 관리하는 Interface를 제공한다.
	//		(전 Packet의 수신시를 확인하여, Drawing의 별도 처리를 하기 위함이이다.)
	m_bPacketReceived = FALSE;
}

CDaiCodeGroup::~CDaiCodeGroup()
{
	// (2004.11.29, 배승원) 등록된 모든 DRDS Advised Info를 제거한다.
	RemoveAllDrdsAdvisedInfo();
}

//////////////////////////////////////////////////////////////////////
// (2004.11.29, 배승원) DRDS Advised Info를 구성하는 Interface를 제공한다.
//////////////////////////////////////////////////////////////////////

// 1. 1개의 DRDS Advised Info Data를 추가하는 Interface를 제공한다.
BOOL CDaiCodeGroup::AddDrdsAdvisedInfo( IPacket *p_pIPacket, const char *p_szRealFieldName)
{
	// 0. 이미 등록되어 있는지 확인한다.
	if( FindDrdsAdvisedInfo( p_pIPacket, p_szRealFieldName)) return FALSE;

	// 1. 새로운 DRDS Advised Info를 구성하여 등록한다.
	AddTail( new CDrdsAdvisedInfo( p_pIPacket, p_szRealFieldName));

	return TRUE;
}

// RQ별 DRDS관리 : 복수종목 - ojtaso (20070111)
BOOL CDaiCodeGroup::AddDrdsAdvisedInfo(LPCTSTR lpszRQ, IPacket *p_pIPacket, const char *p_szRealFieldName)
{
	// 0. 이미 등록되어 있는지 확인한다.
	if( FindDrdsAdvisedInfo(lpszRQ, p_pIPacket, p_szRealFieldName)) return FALSE;

	// 1. 새로운 DRDS Advised Info를 구성하여 등록한다.
	AddTail( new CDrdsAdvisedInfo(lpszRQ, p_pIPacket, p_szRealFieldName));

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// (2004.11.29, 배승원) 구성된 DRDS Advised Info를 제거하는 Interface를 제공한다.
//////////////////////////////////////////////////////////////////////

// 1. 일괄 제거하는 Interface를 제공한다.
void CDaiCodeGroup::RemoveAllDrdsAdvisedInfo( void)
{
	while( !IsEmpty()) delete RemoveHead();
}

//////////////////////////////////////////////////////////////////////
// (2004.11.29, 배승원) 구성된 DRDS Advised Info를 검색하는 Interface를 제공한다.
//////////////////////////////////////////////////////////////////////

// 1. Drds Advised Info 값을 지정받아 검색하는 Inteface를 제공한다.
POSITION CDaiCodeGroup::FindDrdsAdvisedInfo( IPacket *p_pIPacket, const char *p_szRealFieldName)
{
	POSITION posPrevDAI;
	POSITION posDAI = GetHeadPosition();
	while( posDAI)
	{
		posPrevDAI = posDAI;
		CDrdsAdvisedInfo *pDAI = GetNext( posDAI);
		if( pDAI->m_pIPacket == p_pIPacket && pDAI->m_strRealFieldName == p_szRealFieldName)
			return posPrevDAI;
	}
	return NULL;
}

// RQ별 DRDS관리 : 복수종목 - ojtaso (20070111)
POSITION CDaiCodeGroup::FindDrdsAdvisedInfo(LPCTSTR lpszRQ, IPacket *p_pIPacket, const char *p_szRealFieldName)
{
	POSITION posPrevDAI;
	POSITION posDAI = GetHeadPosition();
	while( posDAI)
	{
		posPrevDAI = posDAI;
		CDrdsAdvisedInfo *pDAI = GetNext( posDAI);
		if(pDAI && pDAI->m_lpszRQ && !::lstrcmp(pDAI->m_lpszRQ, lpszRQ) &&
			pDAI->m_pIPacket == p_pIPacket && pDAI->m_strRealFieldName == p_szRealFieldName)
			return posPrevDAI;
	}
	return NULL;
}

