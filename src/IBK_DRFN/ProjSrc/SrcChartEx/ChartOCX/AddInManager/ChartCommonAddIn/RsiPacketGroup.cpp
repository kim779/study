// RsiPacketGroup.cpp: implementation of the CRsiPacketGroup class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RsiPacketGroup.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRsiPacketGroup::CRsiPacketGroup( const char *p_szRealPacketName)
{
	// (2004.11.29, 배승원) 관리중인 Real Setting Info의 Packet 명을 관리한다.
	m_strRealPacketName = p_szRealPacketName;
}

CRsiPacketGroup::~CRsiPacketGroup()
{
	// (2004.11.29, 배승원) 등록된 모든 Real Setting Info를 제거한다.
	RemoveAllRealSettingInfo();
}

//////////////////////////////////////////////////////////////////////
// (2004.11.29, 배승원) Real Setting Info를 구성하는 Interface를 제공한다.
//////////////////////////////////////////////////////////////////////
// 1. 1개의 Real 설정 Data를 추가하는 Interface를 제공한다.
BOOL CRsiPacketGroup::AddRealSettingInfo( const char *p_szChartPacketName, const char *p_szRealFieldName, const char *p_szShareName)
{
	// 0. 이미 등록되어 있는지 확인한다.
	if( FindRealSettingInfo( p_szChartPacketName, p_szRealFieldName, p_szShareName)) return FALSE;

	// 1. 새로운 Real Setting Info를 구성하여 등록한다.
	AddTail( new CRealSettingInfo( p_szChartPacketName, p_szRealFieldName, p_szShareName));

	return TRUE;
}

// RQ별 Real관리 : 복수종목 - ojtaso (20070111)
BOOL CRsiPacketGroup::AddRealSettingInfo(LPCTSTR lpszRQ, const char *p_szChartPacketName, const char *p_szRealFieldName, const char *p_szShareName)
{
	// 0. 이미 등록되어 있는지 확인한다.
	if( FindRealSettingInfo(lpszRQ, p_szChartPacketName, p_szRealFieldName, p_szShareName)) return FALSE;

	// 1. 새로운 Real Setting Info를 구성하여 등록한다.
	AddTail( new CRealSettingInfo(lpszRQ, p_szChartPacketName, p_szRealFieldName, p_szShareName));

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// (2004.11.29, 배승원) 구성된 Real Setting Info를 제거하는 Interface를 제공한다.
//////////////////////////////////////////////////////////////////////

// 1. 일괄 제거하는 Interface를 제공한다.
void CRsiPacketGroup::RemoveAllRealSettingInfo( void)
{
	while( !IsEmpty()) delete RemoveHead();
}

// 2. Real Setting Info 값을 지정받아 Real Setting Info를 제거하는 Interface를 제공한다.
BOOL CRsiPacketGroup::RemoveRealSettingInfo( const char *p_szChartPacketName, const char *p_szRealFieldName, const char *p_szShareName)
{
	POSITION posRealSettingInfo = FindRealSettingInfo( p_szChartPacketName, p_szRealFieldName, p_szShareName);
	if( !posRealSettingInfo) return FALSE;

	delete GetAt( posRealSettingInfo);
	RemoveAt( posRealSettingInfo);
	return TRUE;
}

// RQ별 Real관리 : 복수종목 - ojtaso (20070111)
BOOL CRsiPacketGroup::RemoveRealSettingInfo(LPCTSTR lpszRQ, const char *p_szChartPacketName, const char *p_szRealFieldName, const char *p_szShareName)
{
	POSITION posRealSettingInfo = FindRealSettingInfo(lpszRQ, p_szChartPacketName, p_szRealFieldName, p_szShareName);
	if( !posRealSettingInfo) return FALSE;

	delete GetAt( posRealSettingInfo);
	RemoveAt( posRealSettingInfo);
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// (2004.11.29, 배승원) 구성된 Real Setting Info를 검색하는 Interface를 제공한다.
//////////////////////////////////////////////////////////////////////

// 1. Real Setting Info 값을 지정받아 검색하는 Inteface를 제공한다.
POSITION CRsiPacketGroup::FindRealSettingInfo( const char *p_szChartPacketName, const char *p_szRealFieldName, const char *p_szShareName)
{
	POSITION posPrevRSI;
	POSITION posRSI = GetHeadPosition();
	while( posRSI)
	{
		posPrevRSI = posRSI;
		CRealSettingInfo *pRSI = GetNext( posRSI);
		if( pRSI->m_strChartPacketName		== p_szChartPacketName
			&& pRSI->m_strRealFieldName		== p_szRealFieldName
			&& pRSI->m_strShareName	== p_szShareName)
				return posPrevRSI;
	}
	return NULL;
}

// RQ별 Real관리 : 복수종목 - ojtaso (20070111)
POSITION CRsiPacketGroup::FindRealSettingInfo(LPCTSTR lpszRQ, const char *p_szChartPacketName, const char *p_szRealFieldName, const char *p_szShareName)
{
	POSITION posPrevRSI;
	POSITION posRSI = GetHeadPosition();
	while( posRSI)
	{
		posPrevRSI = posRSI;
		CRealSettingInfo *pRSI = GetNext( posRSI);
		if( pRSI->m_strChartPacketName		== p_szChartPacketName
			&& pRSI->m_strRealFieldName		== p_szRealFieldName
//			&& pRSI->m_strShareName	== p_szShareName
			&& pRSI->m_strRQ	== lpszRQ)
				return posPrevRSI;
	}
	return NULL;
}

// 2. Local Share Name만을 지정받아 해당 공유명으로 등록된 Real Setting Info가 있는지 검색한다.
POSITION CRsiPacketGroup::FindShareName( const char *p_szShareName)
{
	POSITION posPrevRSI;
	POSITION posRSI = GetHeadPosition();
	while( posRSI)
	{
		posPrevRSI = posRSI;
		CRealSettingInfo *pRSI = GetNext( posRSI);
		if( pRSI->m_strShareName	== p_szShareName) return posPrevRSI;
	}
	return NULL;
}

