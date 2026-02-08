// XScaleGridTime.h: interface for the CXScaleGridTime class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XSCALEGRIDTIME_H__B2091179_10EF_415F_80C2_1E4455E098D1__INCLUDED_)
#define AFX_XSCALEGRIDTIME_H__B2091179_10EF_415F_80C2_1E4455E098D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Include_Chart/DataBlockList.h"						// for CDataBlockList
#include "../Include_Chart/BlockBaseEnum.h"						// for CScaleBaseData

class CXScaleGridTime
{
public:
	CXScaleGridTime();
	virtual ~CXScaleGridTime();

// (2008/12/28 - Seung-Won, Bae) Manage temp pointer variable to check NULL from the CDataBlockList's NULL return.
protected:
	const int *			m_pIReturn;
	const tm *			m_pMReturn;

// (2009/1/19 - Seung-Won, Bae) for Scale Time List
protected:
	CDataBlockList< tm>		m_blaScaleDataTime[ 6];			// Time value for scale of YYYY/MM/DD/HH/mm/SS
	CDataBlockList< int>	m_blaScaleDataDRI[ 6];			// DRI value for scale of YYYY/MM/DD/HH/mm/SS
	tm						m_tmLastScale[ 6];
public:
	void					PopLastScaleDataTime( void);
	void					PushScaleDataTime( int p_nTimeDiff, int p_nDRI, tm tmDRI, BOOL p_bEndDRI);
	void					Clear( void);
	int						RecalculateScaleType( const CString &p_strPacketType, const CScaleBaseData::HORZSCALEDRAWERTYPE p_eScaleDrawType, const int p_nScaleRegionWidth, int &p_nSkipFactor, int p_nTimeInterval, const CChartInfo::CHART_MODE p_eChartMode);
	int						GetLastDRI( void);
	void					MakeScaleData( const int p_nScaleType, CDataBlockList <tm> &p_blScaleTime, CDataBlockList <int> &p_blScaleDRI, const int p_nSkipFactor, const CChartInfo::CHART_MODE p_eChartMode);
};

#endif // !defined(AFX_XSCALEGRIDTIME_H__B2091179_10EF_415F_80C2_1E4455E098D1__INCLUDED_)
