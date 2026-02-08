// TimeTypeConvert.cpp: implementation of the CTimeTypeConvert class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TimeTypeConvert.h"

#include "Conversion.h"				// for CDataConversion

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTimeTypeConvert::CTimeTypeConvert()
{
	SetPacketType( "HHMMSS");
}

CTimeTypeConvert::~CTimeTypeConvert()
{

}

void CTimeTypeConvert::SetPacketType( const char *p_szPacketType)
{
	m_strPacketType = p_szPacketType;
	m_bIsYearInPacketType	= ( 0 <= m_strPacketType.Find( "YYYY"));
	m_bIsMonthInPacketType	= ( ( 0 <= m_strPacketType.Find( "YYYYMM")) || ( 0 <= m_strPacketType.Find( "MMDD")));
	m_bIsDayInPacketType	= ( 0 <= m_strPacketType.Find( "DD"));
	m_bIsHourInPacketType	= ( 0 <= m_strPacketType.Find( "HH"));
	m_bIsMinuteInPacketType	= ( ( 0 <= m_strPacketType.Find( "HHMM")) || ( 0 <= m_strPacketType.Find( "MMSS")));
	m_bIsSecondInPacketType	= ( 0 <= m_strPacketType.Find( "SS"));

	__int64 nMod = 100000000000000;
	if( 100000000000000 == nMod && !IsYearInPacketType())	nMod = 10000000000;
	if(     10000000000 == nMod && !IsMonthInPacketType())	nMod =   100000000;
	if(       100000000 == nMod && !IsDayInPacketType())	nMod =     1000000;
	if(         1000000 == nMod && !IsHourInPacketType())	nMod =       10000;
	if(           10000 == nMod && !IsMinuteInPacketType())	nMod =         100;
	m_nRotationFullTime = nMod;

	nMod = 1;
	if(             1 == nMod && !IsSecondInPacketType())	nMod =           100;
	if(           100 == nMod && !IsMinuteInPacketType())	nMod =         10000;
	if(         10000 == nMod && !IsHourInPacketType())		nMod =       1000000;
	if(       1000000 == nMod && !IsDayInPacketType())		nMod =     100000000;
	if(     100000000 == nMod && !IsMonthInPacketType())	nMod =   10000000000;
	m_nMinimumUnitTime = nMod;
}

__int64 CTimeTypeConvert::GetPacketTypeTime( __int64 p_nTime, BOOL p_bCeil) const
{
	if( !IsYearInPacketType()) p_nTime = p_nTime % GetRotationFullTime();

	if( 1 == m_nMinimumUnitTime) return p_nTime;
	if( p_bCeil && p_nTime % m_nMinimumUnitTime)
		p_nTime = CDataConversion::CalcTimeSum( p_nTime, m_nMinimumUnitTime);
	return p_nTime / m_nMinimumUnitTime;
}
