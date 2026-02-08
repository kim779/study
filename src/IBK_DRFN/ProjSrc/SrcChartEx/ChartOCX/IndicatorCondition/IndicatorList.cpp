// IndicatorList.cpp: implementation of the CIndicatorList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../Include_Chart/Dll_Load/IndicatorList.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"			// for g_iMetaTable
#include "IndicatorListImplementation.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIndicatorList::CIndicatorList( HWND p_hOcxWnd, const CString& strMainDefaultFilePathAndName, const CString& strUserDefaultFilePathAndName, const CString& strUserFilePathAndName, CChartInfo::CHART_MODE p_eChartMode)
{
	// (2008/1/20 - Seung-Won, Bae) for MultiLanguage.
	g_iMetaTable.LoadMetaTable();

	m_pIndicatorListImpl = new CIndicatorListImplementation( p_hOcxWnd, this, strMainDefaultFilePathAndName, strUserDefaultFilePathAndName, strUserFilePathAndName, p_eChartMode);
}

CIndicatorList::~CIndicatorList()
{
	delete m_pIndicatorListImpl;
}

//////////////////////////////////////////////////////////////////////
// 1. Load
//////////////////////////////////////////////////////////////////////

BOOL CIndicatorList::LoadIndicatorInfo( const EIndicatorListType p_eIndicatorListType, const char *szFilePath)
{
	return m_pIndicatorListImpl->LoadIndicatorInfo( p_eIndicatorListType, szFilePath);
}
BOOL CIndicatorList::LoadIndicatorInfoFromString( const EIndicatorListType p_eIndicatorListType, const CString &p_strIndicatorList)
{
	return m_pIndicatorListImpl->LoadIndicatorInfoFromString( p_eIndicatorListType, p_strIndicatorList);
}

//////////////////////////////////////////////////////////////////////
// 2. Save
//////////////////////////////////////////////////////////////////////

BOOL CIndicatorList::SaveIndicatorInfo( const EIndicatorListType p_eIndicatorListType, const char *szFilePath)
{
	return m_pIndicatorListImpl->SaveIndicatorInfo( p_eIndicatorListType, szFilePath);
}
BOOL CIndicatorList::GetSaveIndicatorInfoString( const EIndicatorListType p_eIndicatorListType, CString &p_strIndicatorList)
{
	return m_pIndicatorListImpl->GetSaveIndicatorInfoString( p_eIndicatorListType, p_strIndicatorList);
}

//////////////////////////////////////////////////////////////////////
// 3. Basic Info Query
//////////////////////////////////////////////////////////////////////

double CIndicatorList::GetCfgFileVersion( const EIndicatorListType p_eIndicatorListType) const
{
	return m_pIndicatorListImpl->GetCfgFileVersion( p_eIndicatorListType);
}

BOOL CIndicatorList::GetIndicatorNameList( const EIndicatorListType p_eIndicatorListType,
		const EIndicatorInfoType p_eIndicatorInfoType, CList<CString, CString>& IndicatorNameList) const
{
	return m_pIndicatorListImpl->GetIndicatorNameList( p_eIndicatorListType, p_eIndicatorInfoType, IndicatorNameList);
}
BOOL CIndicatorList::GetGraphNameList( const EIndicatorListType p_eIndicatorListType,
		const EIndicatorInfoType p_eIndicatorInfoType, CList<CString, CString>& graphNameList) const
{
	return m_pIndicatorListImpl->GetGraphNameList( p_eIndicatorListType, p_eIndicatorInfoType, graphNameList);
}

// Indicator의 Group에 관련된 내용
CIndicatorGroupDataList *CIndicatorList::GetAllIndicatorNameWithGroupName( void)
{
	return m_pIndicatorListImpl->GetAllIndicatorNameWithGroupName();
}

const CGraphTypeAndStyleData *CIndicatorList::GetGraphTypeAndStyleData( void)
{
	return m_pIndicatorListImpl->GetGraphTypeAndStyleData();
}

//////////////////////////////////////////////////////////////////////
// 4. Indicator Info Query.
//////////////////////////////////////////////////////////////////////

// Get Graph Info from User or MapDefault Indicator Info List.
//	Get Graph Info in User Indicator Info (Option Order)
//		1. Without MapDefault Get, With MapDefault Get, With MapDefault Get and Set		~ EIndicatorInfoGettingType
//		2. Without Indicator Info, With Indicator Info									~ p_szIndicatorName
//		3. in With Indicator Info : With SiteDefault Get, With SiteDefault Get and Set	~ EIndicatorInfoGettingType
//	Get Graph Info in MapDefault Indicator Info (Option Order)
//		1. Without Indicator Info, With Indicator Info									~ p_szIndicatorName
//		2. in With Indicator Info : With SiteDefault Get, With SiteDefault Get and Set	~ EIndicatorInfoGettingType
CIndicatorInfo *CIndicatorList::GetGraphIndicatorInfo( const EIndicatorListType p_eIndicatorListType, const char *p_szGraphName,
										const char *p_szIndicatorName, EIndicatorInfoGettingType p_nDefaultOption)
{
	return m_pIndicatorListImpl->GetGraphIndicatorInfo( p_eIndicatorListType, p_szGraphName, p_szIndicatorName, p_nDefaultOption);
}

// Create New Graph Indicator Info
CIndicatorInfo *CIndicatorList::CreateGraphIndicatorInfo( const char *p_szIndicatorName)
{
	return m_pIndicatorListImpl->CreateGraphIndicatorInfo( p_szIndicatorName);
}

// Get First Graph Info with Indicator Name from User or MapDefault Indicator Info List without Default.
CIndicatorInfo *CIndicatorList::GetFirstIndicatorInfo( const EIndicatorListType p_eIndicatorListType, const char *p_szIndicatorName,
										POSITION *p_ppsnIndicator)
{
	return m_pIndicatorListImpl->GetFirstIndicatorInfo( p_eIndicatorListType, p_szIndicatorName, p_ppsnIndicator);
}

// Get Next Graph Info with Indicator Name in Appointed List.
void CIndicatorList::GetNextIndicatorInfo( const EIndicatorListType p_eIndicatorListType, const char *p_szIndicatorName,
											POSITION *p_ppsnIndicator, CIndicatorInfo *&p_pPrevIndicatorInfo)
{
	m_pIndicatorListImpl->GetNextIndicatorInfo( p_eIndicatorListType, p_szIndicatorName, p_ppsnIndicator, p_pPrevIndicatorInfo);
}

// Get Indicator Info from UserDefault or SiteDefault Indicator Info List (Without Default Get, With Default Get, With Default Get and Set)
CIndicatorInfo *CIndicatorList::GetDefaultIndicatorInfo( const EIndicatorListType p_eIndicatorListType, const char *p_szIndicatorName,
										EIndicatorInfoGettingType p_nDefaultOption)
{
	return m_pIndicatorListImpl->GetDefaultIndicatorInfo( p_eIndicatorListType, p_szIndicatorName, p_nDefaultOption);
}

//////////////////////////////////////////////////////////////////////
// 5. Remove
//	Deleting with appointment is not supported in Map, User and Site Default Indicator Info List.
//////////////////////////////////////////////////////////////////////

// Remove Indicator Info in User Indicator Info List
BOOL CIndicatorList::RemoveGraphIndicatorInfo( const char *p_szGraphName)
{
	return m_pIndicatorListImpl->RemoveGraphIndicatorInfo( p_szGraphName);
}

BOOL CIndicatorList::RemoveAllIndicatorInfo( const EIndicatorListType p_eIndicatorListType)
{
	return m_pIndicatorListImpl->RemoveAllIndicatorInfo( p_eIndicatorListType);
}

BOOL CIndicatorList::RemoveAllIndicatorInfo( const EIndicatorListType p_eIndicatorListType, const char *p_szIndicatorName)
{
	return m_pIndicatorListImpl->RemoveAllIndicatorInfo( p_eIndicatorListType, p_szIndicatorName);
}

