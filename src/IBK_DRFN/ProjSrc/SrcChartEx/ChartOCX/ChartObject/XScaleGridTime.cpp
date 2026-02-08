// XScaleGridTime.cpp: implementation of the CXScaleGridTime class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XScaleGridTime.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXScaleGridTime::CXScaleGridTime()
{
}

CXScaleGridTime::~CXScaleGridTime()
{
	Clear();
}

//////////////////////////////////////////////////////////////////////
// Operations
//////////////////////////////////////////////////////////////////////

void CXScaleGridTime::PopLastScaleDataTime( void)
{
	for( int i = 0; i < 6; i++) if( 1 < m_blaScaleDataDRI[ i].GetItemCount())
	{
		m_blaScaleDataDRI[ i].DeleteBack( 1);
		m_blaScaleDataTime[ i].DeleteBack( 1);
		int nDRICount = m_blaScaleDataDRI[ i].GetItemCount();
		if( 0 < nDRICount)
		{
			m_pMReturn = m_blaScaleDataTime[ i].GetAt( nDRICount - 1);
			if( m_pMReturn) m_tmLastScale[ i] = *m_pMReturn;
		}
		else memset( &m_tmLastScale[ i], 0, sizeof( tm));
	}
}

void CXScaleGridTime::PushScaleDataTime( int p_nTimeDiff, int p_nDRI, tm tmDRI, BOOL p_bEndDRI)
{
	tmDRI.tm_hour += p_nTimeDiff;
	mktime( &tmDRI);

	BOOL bNewTime = p_bEndDRI;
	for( int i = 0; i < 6; i++)
	{
		if( !bNewTime) switch( i)
		{
			case 0:	bNewTime = ( tmDRI.tm_year	!= m_tmLastScale[ 0].tm_year);
					break;
			case 1: bNewTime = ( tmDRI.tm_mon	!= m_tmLastScale[ 1].tm_mon);
					break;
			case 2: bNewTime = ( tmDRI.tm_mday	!= m_tmLastScale[ 2].tm_mday);
					break;
			case 3: bNewTime = ( tmDRI.tm_hour	!= m_tmLastScale[ 3].tm_hour);
					break;
			case 4: bNewTime = ( tmDRI.tm_min	!= m_tmLastScale[ 4].tm_min);
					break;
			case 5: bNewTime = ( tmDRI.tm_sec	!= m_tmLastScale[ 5].tm_sec);
					break;
		}
		if( bNewTime)
		{
			m_tmLastScale[ i] = tmDRI;
			m_blaScaleDataTime[ i].PushBack( tmDRI);
			m_blaScaleDataDRI[ i].PushBack( p_nDRI);
		}
	}
}

void CXScaleGridTime::Clear( void)
{
	for( int i = 0; i < 6; i++) m_blaScaleDataTime[ i].Clear();
	for( int i = 0; i < 6; i++)	m_blaScaleDataDRI[ i].Clear();
	memset( m_tmLastScale, 0, sizeof( tm) * 6);
}

int CXScaleGridTime::RecalculateScaleType( const CString &p_strPacketType, const CScaleBaseData::HORZSCALEDRAWERTYPE p_eScaleDrawType, const int p_nScaleRegionWidth, int &p_nSkipFactor, int p_nTimeInterval, const CChartInfo::CHART_MODE p_eChartMode)
{
	// 1. Get Scale Type
	int nMaxDRICount = m_blaScaleDataDRI[ 5].GetItemCount();
	int nDRICount = 0;
	int nUpDRICount = 0;
	int nScaleType = 0;
	for( nScaleType = 0; nScaleType < 6; nScaleType++)
	{
		// (2009/4/30 - Seung-Won, Bae) Check packet type for Auto Scale Type.
		if( 0 == nScaleType && p_strPacketType.Find( 'Y') < 0) continue;
		if( 1 == nScaleType && p_strPacketType.Find( "YM") < 0 && p_strPacketType.Find( "MD") < 0) continue;
		if( 2 == nScaleType && p_strPacketType.Find( 'D') < 0) continue;
		if( 3 == nScaleType && p_strPacketType.Find( 'H') < 0) continue;
		if( 4 == nScaleType && p_strPacketType.Find( "HM") < 0 && p_strPacketType.Find( "MS") < 0) continue;
		if( 5 == nScaleType && p_strPacketType.Find( 'S') < 0) continue;
		nUpDRICount = nDRICount;

		nDRICount = m_blaScaleDataDRI[ nScaleType].GetItemCount();

		// (2009/2/6 - Seung-Won, Bae) check maximum scale type
		if( ( CScaleBaseData::HORZ_TIME_ONE == p_eScaleDrawType
				|| CScaleBaseData::HORZ_TIME_TWO == p_eScaleDrawType)
			&& nScaleType < 1)
			continue;
		if( ( CScaleBaseData::HORZ_TICK == p_eScaleDrawType
				|| CScaleBaseData::HORZ_TIME_SECOND == p_eScaleDrawType)
			&& nScaleType < 2)
			continue;

		// (2009/1/22 - Seung-Won, Bae) check minimum scale type.
		if( 1 == nScaleType && CScaleBaseData::HORZ_MONTHLY == p_eScaleDrawType)
			break;
		if( 2 == nScaleType
			&& ( CScaleBaseData::HORZ_DAILY == p_eScaleDrawType
				|| CScaleBaseData::HORZ_WEEKLEY == p_eScaleDrawType))
				break;
		if( 4 == nScaleType
			&& ( CScaleBaseData::HORZ_TIME_ONE == p_eScaleDrawType
				|| CScaleBaseData::HORZ_TIME_TWO == p_eScaleDrawType
				|| CScaleBaseData::HORZ_TICK == p_eScaleDrawType))
				break;

		// (2009/2/17 - Seung-Won, Bae) Check minimum scale.
		if( nDRICount < 3) continue;
		
		if( p_nScaleRegionWidth / nDRICount <= 110)						// Maximum Scale width is 100 pixel.
			break;
	}
	if( 6 <= nScaleType) nScaleType = 5;

	double dOverlapFactor = p_nScaleRegionWidth;
	if( 0 < nDRICount) dOverlapFactor /= nDRICount;		// the factor for closing of scale
	TRACE( "\r\nScale Closing : %f\r\n", dOverlapFactor);


	// Adjust Skip Factor
	switch( nScaleType)
	{
		// Year
		case 0: if(			150./10 >= dOverlapFactor)	p_nSkipFactor = 10;
				else if(	150./ 5 >= dOverlapFactor)	p_nSkipFactor = 5;
				else if(	150./ 2 >= dOverlapFactor)	p_nSkipFactor = 2;
				break;
		// Month
		case 1: if(			150./12 >= dOverlapFactor)	p_nSkipFactor = 12;
				else if(	150./ 6 >= dOverlapFactor)	p_nSkipFactor = 6;
				else if(	150./ 4 >= dOverlapFactor)	p_nSkipFactor = 4;
				else if(	150./ 3 >= dOverlapFactor)	p_nSkipFactor = 3;
				else if(	150./ 2 >= dOverlapFactor || CScaleBaseData::HORZ_MONTHLY == p_eScaleDrawType)
														p_nSkipFactor = 2;
				break;
		// Day
		case 2:	switch( p_eScaleDrawType)
				{
					case CScaleBaseData::HORZ_WEEKLEY:	
						if(			150. * 7 / 28 >= dOverlapFactor)	p_nSkipFactor = 28;
						else											p_nSkipFactor = 14;
						break;
					case CScaleBaseData::HORZ_TIME_ONE:
					case CScaleBaseData::HORZ_TIME_TWO:
						if(			150. / 20 >= dOverlapFactor)	p_nSkipFactor = 31;
						else if(	150. / 10 >= dOverlapFactor)	p_nSkipFactor = 14;
						else if(	150. /  5 >= dOverlapFactor)	p_nSkipFactor = 7;
						break;
					case CScaleBaseData::HORZ_DAILY:
						if( p_nTimeInterval <= 1)
						{
							if(			150. / 20 >= dOverlapFactor)	p_nSkipFactor = 31;
							else if(	150. / 10 >= dOverlapFactor)	p_nSkipFactor = 14;
							else										p_nSkipFactor = 7;
						}
						else
						{
							dOverlapFactor /= p_nTimeInterval;
							if(			150. / 15 >= dOverlapFactor)	p_nSkipFactor = 15;
							else if(	150. / 10 >= dOverlapFactor)	p_nSkipFactor = 10;
							else if(	150. / 8  >= dOverlapFactor)	p_nSkipFactor = 8;
							else if(	150. / 5  >= dOverlapFactor)	p_nSkipFactor = 5;
							else										p_nSkipFactor = 2 * p_nTimeInterval;
						}
						break;
					case CScaleBaseData::HORZ_TICK:
						if(			150. / 20 >= dOverlapFactor)	p_nSkipFactor = 31;
						else if(	150. / 10 >= dOverlapFactor)	p_nSkipFactor = 14;
						else if(	150. / 5 >= dOverlapFactor)		p_nSkipFactor = 7;
						break;
				}
				break;
		// Hour
		case 3: // (2009/4/30 - Seung-Won, Bae) Check packet type for Auto Scale Type.
				if( CChartInfo::HTS != p_eChartMode)
				{
					if(			150./24 >= dOverlapFactor && 2 < nUpDRICount)
														p_nSkipFactor = 24;
					else if(	150./12 >= dOverlapFactor && 2 < nUpDRICount)
														p_nSkipFactor = 12;
					else if(	150./6 >= dOverlapFactor && 2 < nUpDRICount)
														p_nSkipFactor = 6;
				}
				else if(	150./7 >= dOverlapFactor && 2 < nUpDRICount)
														p_nSkipFactor = 7;
				if( 3 < p_nSkipFactor);
				else if(	150./3 >= dOverlapFactor)	p_nSkipFactor = 3;
				else									p_nSkipFactor = 2;
				break;
		// Minute
		case 4:	// (2009/5/7 - Seung-Won, Bae) for HHMMSS Type.
				if( CScaleBaseData::HORZ_TIME_ONE == p_eScaleDrawType || CScaleBaseData::HORZ_TIME_TWO == p_eScaleDrawType)
				{
					if( 0 <= p_strPacketType.Find( 'S')) p_nTimeInterval /= 100;
					if( 1 < p_nTimeInterval) dOverlapFactor /= ( p_nTimeInterval / 100 * 60 + p_nTimeInterval % 100);
				}
				if(			150./60 >= dOverlapFactor)	p_nSkipFactor = 60;
				else if(	150./30 >= dOverlapFactor)	p_nSkipFactor = 30;
				else if(	150./20 >= dOverlapFactor)	p_nSkipFactor = 20;
				else if(	150./10 >= dOverlapFactor)	p_nSkipFactor = 10;
				else if(	150./ 5 >= dOverlapFactor)	p_nSkipFactor = 5;
				else if(	150./ 2 >= dOverlapFactor || CScaleBaseData::HORZ_TIME_ONE == p_eScaleDrawType || CScaleBaseData::HORZ_TIME_TWO == p_eScaleDrawType)
														p_nSkipFactor = 2;	
				if( CScaleBaseData::HORZ_TIME_ONE == p_eScaleDrawType || CScaleBaseData::HORZ_TIME_TWO == p_eScaleDrawType)
					if( 1 < p_nTimeInterval && p_nSkipFactor <= p_nTimeInterval) p_nSkipFactor = ( p_nTimeInterval / 100 * 60 + p_nTimeInterval % 100) * 2;
				break;
		// Second
		case 5: if(			150./30 >= dOverlapFactor)	p_nSkipFactor = 30;
				else if(	150./20 >= dOverlapFactor)	p_nSkipFactor = 20;
				else if(	150./10 >= dOverlapFactor)	p_nSkipFactor = 10;
				else if(	150./ 5 >= dOverlapFactor)	p_nSkipFactor = 5;
				else if(	150./ 2 >= dOverlapFactor)	p_nSkipFactor = 2;	
				break;
	}

	return nScaleType;
}

int CXScaleGridTime::GetLastDRI( void)
{
	int nDRIScaleCount = m_blaScaleDataDRI[ 5].GetItemCount();
	if( nDRIScaleCount <= 0) return 0;

	m_pIReturn = m_blaScaleDataDRI[ 5].GetAt( nDRIScaleCount - 1);
	if( !m_pIReturn) return 0;
	int nDRI_Start_Cur = *m_pIReturn - 1;
	if( nDRI_Start_Cur < 0) nDRI_Start_Cur = 0;
	return nDRI_Start_Cur;
}

void CXScaleGridTime::MakeScaleData( const int p_nScaleType, CDataBlockList <tm> &p_blScaleTime, CDataBlockList <int> &p_blScaleDRI, const int p_nSkipFactor, const CChartInfo::CHART_MODE p_eChartMode)
{
	const int *pIReturn = NULL;
	const tm *pMReturn = NULL;

	time_t timeNext = 0;
	int nNextTime = -1;
	int nDRI = -1;
	tm tmDRI, tmNext;
	memset( &tmDRI, 0, sizeof( tm));
	memset( &tmNext, 0, sizeof( tm));
	m_blaScaleDataDRI[ p_nScaleType].SetIndex( 0);
	m_blaScaleDataTime[ p_nScaleType].SetIndex( 0);

	int nDRICount = m_blaScaleDataDRI[ p_nScaleType].GetItemCount();
	for( int i = 0; i < nDRICount; i++)
	{
		pIReturn = m_blaScaleDataDRI[ p_nScaleType].GetNext();
		if( !pIReturn) break;
		nDRI = *pIReturn;

		pMReturn = m_blaScaleDataTime[ p_nScaleType].GetNext();
		if( !pMReturn) break;
		tmDRI = *pMReturn;

		if( 0 == p_blScaleTime.GetItemCount())
		{
			p_blScaleTime.PushBack( tmDRI);
			p_blScaleDRI.PushBack( nDRI);
			switch( p_nScaleType)
			{
				case 0:	tmNext.tm_year = tmDRI.tm_year + p_nSkipFactor - tmDRI.tm_year % p_nSkipFactor;
						tmNext.tm_mday = 1;
						break;
				case 1:	tmNext.tm_year = tmDRI.tm_year;
						tmNext.tm_mon = tmDRI.tm_mon + p_nSkipFactor - tmDRI.tm_mon % p_nSkipFactor;
						tmNext.tm_mday = 1;
						break;
				case 2:	tmNext.tm_year = tmDRI.tm_year;
						tmNext.tm_mon = tmDRI.tm_mon;
						if( 1 == p_nSkipFactor)		tmNext.tm_mday = tmDRI.tm_mday + 1;
						else if( 7 == p_nSkipFactor)	tmNext.tm_mday = tmDRI.tm_mday + p_nSkipFactor - tmDRI.tm_wday + 1;
						else if( 14 == p_nSkipFactor || 28 == p_nSkipFactor)
						{
							tmNext.tm_mday = tmDRI.tm_mday + 7 - tmDRI.tm_wday + 1;
							timeNext = mktime( &tmNext);
							if( tmNext.tm_mon == tmDRI.tm_mon) tmNext.tm_mday += 7;
							if( 28 == p_nSkipFactor)
							{
								timeNext = mktime( &tmNext);
								if( tmNext.tm_mon == tmDRI.tm_mon) tmNext.tm_mday += 7;
								timeNext = mktime( &tmNext);
								if( tmNext.tm_mon == tmDRI.tm_mon) tmNext.tm_mday += 7;
							}
						}
						else if( 31 == p_nSkipFactor)
						{
							tmNext.tm_mon++;
							tmNext.tm_mday = 1;
						}
						else // if( 2 == p_nSkipFactor) 2 and others
						{
							tmNext.tm_mday = tmDRI.tm_mday + p_nSkipFactor;
							timeNext = mktime( &tmNext);
							if( tmNext.tm_mon != tmDRI.tm_mon) tmNext.tm_mday = 0;
						}
						break;
				case 3:	tmNext.tm_year = tmDRI.tm_year;
						tmNext.tm_mon = tmDRI.tm_mon;
						tmNext.tm_mday = tmDRI.tm_mday;
						if( 1 == p_nSkipFactor)		tmNext.tm_hour = tmDRI.tm_hour + 1;
						else if( 2 <= p_nSkipFactor)
						{
							tmNext.tm_hour = tmDRI.tm_hour + p_nSkipFactor;
							if( CChartInfo::HTS != p_eChartMode) tmNext.tm_hour -= tmDRI.tm_hour % p_nSkipFactor;
							timeNext = mktime( &tmNext);
							if( tmNext.tm_mday != tmDRI.tm_mday) tmNext.tm_hour = 0;
						}
						break;
				case 4:	tmNext.tm_year = tmDRI.tm_year;
						tmNext.tm_mon = tmDRI.tm_mon;
						tmNext.tm_mday = tmDRI.tm_mday;
						tmNext.tm_hour = tmDRI.tm_hour;
						if( 1 == p_nSkipFactor)		tmNext.tm_min = tmDRI.tm_min + 1;
						else if( 2 <= p_nSkipFactor)
						{
							tmNext.tm_min = tmDRI.tm_min + p_nSkipFactor - tmDRI.tm_min % p_nSkipFactor;
							timeNext = mktime( &tmNext);
							if( tmNext.tm_mday != tmDRI.tm_mday) tmNext.tm_hour = 0;
							if( tmNext.tm_hour != tmDRI.tm_hour) tmNext.tm_min = 0;
						}
						break;
				case 5:	tmNext.tm_year = tmDRI.tm_year;
						tmNext.tm_mon = tmDRI.tm_mon;
						tmNext.tm_mday = tmDRI.tm_mday;
						tmNext.tm_hour = tmDRI.tm_hour;
						tmNext.tm_min = tmDRI.tm_min;
						if( 1 == p_nSkipFactor)		tmNext.tm_sec = tmDRI.tm_sec + 1;
						else if( 2 <= p_nSkipFactor)
						{
							tmNext.tm_sec = tmDRI.tm_sec + p_nSkipFactor - tmDRI.tm_sec % p_nSkipFactor;
							timeNext = mktime( &tmNext);
							if( tmNext.tm_min != tmDRI.tm_min) tmNext.tm_sec = 0;
						}
						break;
			}
			timeNext = mktime( &tmNext);
			continue;
		}

		BOOL bNext = FALSE;
		if( nDRICount - 1 == i) bNext = TRUE;
		else switch( p_nScaleType)
		{
			case 0:	if( timeNext <= mktime( &tmDRI))
					{
						bNext = TRUE;
						tmNext.tm_year += p_nSkipFactor;
						timeNext = mktime( &tmNext);
					}
					break;
			case 1:	if( timeNext <= mktime( &tmDRI))
					{
						tmNext.tm_year = tmDRI.tm_year;
						if( p_nSkipFactor == 1 || 11 != tmDRI.tm_mon)
						{
							bNext = TRUE;		// Do not show Descember in skip mode.
							tmNext.tm_mon += p_nSkipFactor;
						}
						else tmNext.tm_mon += 1;
						timeNext = mktime( &tmNext);
					}
					break;
			case 2:	if( timeNext <= mktime( &tmDRI))
					{
						bNext = TRUE;
						tmNext.tm_year = tmDRI.tm_year;
						tmNext.tm_mon = tmDRI.tm_mon;
						if( 1 == p_nSkipFactor)		tmNext.tm_mday = tmDRI.tm_mday + 1;
						else if( 7 == p_nSkipFactor)	tmNext.tm_mday = tmDRI.tm_mday + p_nSkipFactor - tmDRI.tm_wday + 1;
						else if( 14 == p_nSkipFactor || 28 == p_nSkipFactor)
						{
							tmNext.tm_mday = tmDRI.tm_mday + 7 - tmDRI.tm_wday + 1;
							timeNext = mktime( &tmNext);
							if( tmNext.tm_mon == tmDRI.tm_mon) tmNext.tm_mday += 7;
							else bNext = FALSE;
							if( 28 == p_nSkipFactor)
							{
								timeNext = mktime( &tmNext);
								if( tmNext.tm_mon == tmDRI.tm_mon) tmNext.tm_mday += 7;
								else bNext = FALSE;
								timeNext = mktime( &tmNext);
								if( tmNext.tm_mon == tmDRI.tm_mon) tmNext.tm_mday += 7;
								else bNext = FALSE;
							}
						}
						else if( 31 == p_nSkipFactor)
						{
							tmNext.tm_mon++;
							tmNext.tm_mday = 1;
						}
						else // if( 2 == p_nSkipFactor) 2 and others
						{
							tmNext.tm_mday = tmDRI.tm_mday + p_nSkipFactor;
							timeNext = mktime( &tmNext);
							if( tmNext.tm_mon != tmDRI.tm_mon) tmNext.tm_mday = 0;
						}
						timeNext = mktime( &tmNext);
					}
					break;
			case 3:	if( timeNext <= mktime( &tmDRI))
					{
						bNext = TRUE;
						tmNext.tm_year = tmDRI.tm_year;
						tmNext.tm_mon = tmDRI.tm_mon;
						tmNext.tm_mday = tmDRI.tm_mday;
						if( 1 == p_nSkipFactor)		tmNext.tm_hour= tmDRI.tm_hour + 1;
						else if( 2 <= p_nSkipFactor)
						{
							tmNext.tm_hour = tmDRI.tm_hour + p_nSkipFactor;
							if( CChartInfo::HTS != p_eChartMode) tmNext.tm_hour -= tmDRI.tm_hour % p_nSkipFactor;
							timeNext = mktime( &tmNext);
							if( tmNext.tm_mday != tmDRI.tm_mday) tmNext.tm_hour = 0;
						}
						timeNext = mktime( &tmNext);
					}
					break;
			case 4:	if( timeNext <= mktime( &tmDRI))
					{
						bNext = TRUE;
						tmNext.tm_year = tmDRI.tm_year;
						tmNext.tm_mon = tmDRI.tm_mon;
						tmNext.tm_mday = tmDRI.tm_mday;
						tmNext.tm_hour = tmDRI.tm_hour;
						if( 1 == p_nSkipFactor)		tmNext.tm_min = tmDRI.tm_min + 1;
						else if( 2 <= p_nSkipFactor)
						{
							tmNext.tm_min = tmDRI.tm_min + p_nSkipFactor - tmDRI.tm_min % p_nSkipFactor;
							timeNext = mktime( &tmNext);
							if( tmNext.tm_mday != tmDRI.tm_mday) tmNext.tm_hour = 0;
							if( tmNext.tm_hour != tmDRI.tm_hour) tmNext.tm_min = 0;
						}
						timeNext = mktime( &tmNext);
					}
					break;
			case 5:	if( timeNext <= mktime( &tmDRI))
					{
						bNext = TRUE;
						tmNext.tm_year = tmDRI.tm_year;
						tmNext.tm_mon = tmDRI.tm_mon;
						tmNext.tm_mday = tmDRI.tm_mday;
						tmNext.tm_min = tmDRI.tm_min;
						if( 1 == p_nSkipFactor)		tmNext.tm_sec = tmDRI.tm_sec + 1;
						else if( 2 <= p_nSkipFactor)
						{
							tmNext.tm_sec = tmDRI.tm_sec + p_nSkipFactor - tmDRI.tm_sec % p_nSkipFactor;
							timeNext = mktime( &tmNext);
							if( tmNext.tm_min != tmDRI.tm_min) tmNext.tm_sec = 0;
						}
						timeNext = mktime( &tmNext);
					}
					break;
		}

		if( !bNext) continue;

		p_blScaleTime.PushBack( tmDRI);
		p_blScaleDRI.PushBack( nDRI);
	}
}