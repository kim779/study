// TimeTypeConvert.h: interface for the CTimeTypeConvert class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TIMETYPECONVERT_H__89C667C7_4A71_40CE_BC45_528467DE610F__INCLUDED_)
#define AFX_TIMETYPECONVERT_H__89C667C7_4A71_40CE_BC45_528467DE610F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTimeTypeConvert  
{
public:
	CTimeTypeConvert();
	virtual ~CTimeTypeConvert();
protected:
	CString		m_strPacketType;
	BOOL		m_bIsYearInPacketType;
	BOOL		m_bIsMonthInPacketType;
	BOOL		m_bIsDayInPacketType;
	BOOL		m_bIsHourInPacketType;
	BOOL		m_bIsMinuteInPacketType;
	BOOL		m_bIsSecondInPacketType;
	__int64		m_nRotationFullTime;
	__int64		m_nMinimumUnitTime;
public:
	void			SetPacketType( const char *p_szPacketType);
	const CString &	GetPacketType( void) const						{	return m_strPacketType;			}
	BOOL			IsYearInPacketType( void) const					{	return m_bIsYearInPacketType;	}
	BOOL			IsMonthInPacketType( void) const				{	return m_bIsMonthInPacketType;	}
	BOOL			IsDayInPacketType( void) const					{	return m_bIsDayInPacketType;	}
	BOOL			IsHourInPacketType( void) const					{	return m_bIsHourInPacketType;	}
	BOOL			IsMinuteInPacketType( void) const				{	return m_bIsMinuteInPacketType;	}
	BOOL			IsSecondInPacketType( void) const				{	return m_bIsSecondInPacketType;	}
					// Caution!! Do not use m_nRotationFullTime on PacketType with Year.

	__int64			GetRotationFullTime( void)										const
	// 2011.01.06 by SYS >> 분차트 YYYY(년) 추가
	//				{	ASSERT( !IsYearInPacketType());		return m_nRotationFullTime;		}
					{										return m_nRotationFullTime;		}
	// 2011.01.06 by SYS <<

	__int64			GetMinimumUnitTime( void)										const
					{										return m_nMinimumUnitTime;		}
	__int64			GetFullTypeTime( __int64 p_nTime)								const
					{	
						return p_nTime *= m_nMinimumUnitTime;	
					}
	__int64			GetPacketTypeTime( __int64 p_nTime, BOOL p_bCeil = FALSE)		const;
};

#endif // !defined(AFX_TIMETYPECONVERT_H__89C667C7_4A71_40CE_BC45_528467DE610F__INCLUDED_)
