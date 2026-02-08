// IndicatorInfoMap.cpp: implementation of the CIndicatorInfoMap class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IndicatorInfoMap.h"

#include "../Include_Chart/DLL_Load/IndicatorInfo.h"		// for CIndicatorInfo
#include "../Include_Chart/DLL_Load/IndicatorGroupData.h"	// for CIndicatorGroupDataList
#include "../Include_Chart/DLL_Load/IMetaTable.h"			// for g_iMetaTable

//////////////////////////////////////////////////////////////////////
// CIndicatorInfoMap
//////////////////////////////////////////////////////////////////////

CIndicatorInfoMap::CIndicatorInfoMap( EIndicatorInfoType p_eIndicatorInfoType)
{
	m_eIndicatorInfoType = p_eIndicatorInfoType;

	InitHashTable( 200);
}

CIndicatorInfoMap::~CIndicatorInfoMap()
{

}

BOOL CIndicatorInfoMap::Delete( CIndicatorInfo *p_pIndicatorInfo)
{
	CString strKey;
	CIndicatorInfo *pIndicatorInfo2 = NULL;
	POSITION psn = GetStartPosition();
	while( psn)
	{
		GetNextAssoc( psn, strKey, pIndicatorInfo2);
		if( pIndicatorInfo2 == p_pIndicatorInfo)
		{
			RemoveKey( strKey);
			delete pIndicatorInfo2;
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CIndicatorInfoMap::DeleteAll( void)
{
	CString strKey;
	CIndicatorInfo *pIndicatorInfo2 = NULL;
	POSITION psn = GetStartPosition();
	while( psn)
	{
		GetNextAssoc( psn, strKey, pIndicatorInfo2);
		delete pIndicatorInfo2;
	}
	RemoveAll();

	return TRUE;
}

// (2009/9/2 - Seung-Won, Bae) Support Chart Mode. (for HTS/FX/WorldOn)
BOOL CIndicatorInfoMap::GetIndicatorNameList( CStringDataList& dataList, CChartInfo::CHART_MODE p_eChartMode) const
{
	if( GetCount() <= 0) return FALSE;

	CString strKey, strIndicatorName;
	CIndicatorInfo *pIndicatorInfo = NULL;
	POSITION psnIndicator = GetStartPosition();
	while( psnIndicator)
	{
		GetNextAssoc( psnIndicator, strKey, pIndicatorInfo);
		strIndicatorName = pIndicatorInfo->GetIndicatorName();
		if( g_iMetaTable.IsIndicatorInThisMode( strIndicatorName, p_eChartMode))
			dataList.AddTail( strIndicatorName);
	}
	return TRUE;
}

// (2009/9/2 - Seung-Won, Bae) Support Chart Mode. (for HTS/FX/WorldOn)
CIndicatorInfo *CIndicatorInfoMap::GetIndicatorInfo( const char *p_szIndicatorKeyName, CChartInfo::CHART_MODE p_eChartMode)
{
	if( !p_szIndicatorKeyName) return NULL;
	if( !*p_szIndicatorKeyName) return NULL;
	if( !g_iMetaTable.IsIndicatorInThisMode( p_szIndicatorKeyName, p_eChartMode)) return NULL;
	CIndicatorInfo *pIndicatorInfo = NULL;
	if( !Lookup( p_szIndicatorKeyName, pIndicatorInfo)) return NULL;
	return pIndicatorInfo;
}


//////////////////////////////////////////////////////////////////////
// CDefaultIndicatorInfoMap
//////////////////////////////////////////////////////////////////////

CDefaultIndicatorInfoMap::CDefaultIndicatorInfoMap( EIndicatorInfoType p_eIndicatorInfoType) : CIndicatorInfoMap( p_eIndicatorInfoType)
{

}

CDefaultIndicatorInfoMap::~CDefaultIndicatorInfoMap()
{

}

BOOL CDefaultIndicatorInfoMap::AddIndicatorInfo( CIndicatorInfo *p_pIndicatorInfo)
{
	if( !p_pIndicatorInfo) return FALSE;
	const char *szIndicatorName = p_pIndicatorInfo->GetIndicatorName();

	CIndicatorInfo *pIndicatorInfo = NULL;
	if( Lookup( szIndicatorName, pIndicatorInfo)) return FALSE;

	SetAt( szIndicatorName, p_pIndicatorInfo);
	m_slIndicatorNameList.AddTail( szIndicatorName);
	return TRUE;
}

// (2009/9/2 - Seung-Won, Bae) Support Chart Mode. (for HTS/FX/WorldOn)
BOOL CDefaultIndicatorInfoMap::GetAllIndicatorNameWithGroupName( CIndicatorGroupDataList& indiGroupDataList, CChartInfo::CHART_MODE p_eChartMode) const
{
	if( GetCount() <= 0) return FALSE;

	CIndicatorInfo *pIndicatorInfo = NULL;
	POSITION psnIndicator = m_slIndicatorNameList.GetHeadPosition();
	while( psnIndicator)
	{
		const CString &strIndicatorName = m_slIndicatorNameList.GetNext( psnIndicator);
		if( strIndicatorName.IsEmpty()) continue;
		if( !g_iMetaTable.IsIndicatorInThisMode( strIndicatorName, p_eChartMode)) continue;

		if( !Lookup( strIndicatorName, pIndicatorInfo)) continue;

		indiGroupDataList.AddTail_IndiName( pIndicatorInfo->GetGroupName(), strIndicatorName);
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// CGraphIndicatorInfoMap
//////////////////////////////////////////////////////////////////////

CGraphIndicatorInfoMap::CGraphIndicatorInfoMap( EIndicatorInfoType p_eIndicatorInfoType) : CIndicatorInfoMap( p_eIndicatorInfoType)
{

}

CGraphIndicatorInfoMap::~CGraphIndicatorInfoMap()
{

}

BOOL CGraphIndicatorInfoMap::AddIndicatorInfo( CIndicatorInfo *p_pIndicatorInfo)
{
	if( !p_pIndicatorInfo) return FALSE;
	const char *szGraphName = p_pIndicatorInfo->GetGraphName();

	CIndicatorInfo *pIndicatorInfo = NULL;
	if( Lookup( szGraphName, pIndicatorInfo)) return FALSE;

	SetAt( szGraphName, p_pIndicatorInfo);
	return TRUE;
}

BOOL CGraphIndicatorInfoMap::RemoveAllIndicator( const char *p_szIndicatorName)
{
	if( GetCount() <= 0 || !p_szIndicatorName) return NULL;
	if( !*p_szIndicatorName) return NULL;

	BOOL bResult = FALSE;
	CString strKey;
	CIndicatorInfo *pIndicatorInfo = NULL;
	POSITION psnIndicator = GetStartPosition();
	while( psnIndicator)
	{
		GetNextAssoc( psnIndicator, strKey, pIndicatorInfo);
		if( pIndicatorInfo->GetIndicatorName() == p_szIndicatorName)
		{
			RemoveKey( strKey);
			delete pIndicatorInfo;
			bResult = TRUE;
		}
	}

	return bResult;
}

CIndicatorInfo *CGraphIndicatorInfoMap::GetFirstIndicatorInfo( const char *p_szIndicatorName, POSITION *p_ppsnIndicator) const
{
	if( GetCount() <= 0 || !p_szIndicatorName) return NULL;
	if( !*p_szIndicatorName) return NULL;

	CString strKey;
	CIndicatorInfo *pIndicatorInfo = NULL;
	POSITION psnIndicatorOrg = NULL;
	POSITION &psnIndicator = ( p_ppsnIndicator ? *p_ppsnIndicator : psnIndicatorOrg);
	psnIndicator = GetStartPosition();
	while( psnIndicator)
	{
		GetNextAssoc( psnIndicator, strKey, pIndicatorInfo);
		if( pIndicatorInfo->GetIndicatorName() == p_szIndicatorName) return pIndicatorInfo;
	}

	return NULL;
}

CIndicatorInfo *CGraphIndicatorInfoMap::GetNextIndicatorInfo( const char *p_szIndicatorName, POSITION *p_ppsnIndicator) const
{
	if( !p_ppsnIndicator) return NULL;
	if( GetCount() <= 0 || !p_szIndicatorName)		*p_ppsnIndicator = NULL;
	if( !*p_szIndicatorName)						*p_ppsnIndicator = NULL;
	if( !*p_ppsnIndicator) return NULL;

	CString strKey;
	CIndicatorInfo *pIndicatorInfo = NULL;
	POSITION &psnIndicator = *p_ppsnIndicator;
	while( psnIndicator)
	{
		GetNextAssoc( psnIndicator, strKey, pIndicatorInfo);
		if( pIndicatorInfo->GetIndicatorName() == p_szIndicatorName) return pIndicatorInfo;
	}

	return NULL;
}

// (2009/9/2 - Seung-Won, Bae) Support Chart Mode. (for HTS/FX/WorldOn)
BOOL CGraphIndicatorInfoMap::GetGraphNameList( CStringDataList& dataList, CChartInfo::CHART_MODE p_eChartMode) const
{
	if( GetCount() <= 0) return FALSE;

	CString strKey;
	CIndicatorInfo *pIndicatorInfo = NULL;
	POSITION psnIndicator = GetStartPosition();
	while( psnIndicator)
	{
		GetNextAssoc( psnIndicator, strKey, pIndicatorInfo);
		if( g_iMetaTable.IsIndicatorInThisMode( pIndicatorInfo->GetIndicatorName(), p_eChartMode))
			dataList.AddTail( strKey);
	}
	return TRUE;
}
