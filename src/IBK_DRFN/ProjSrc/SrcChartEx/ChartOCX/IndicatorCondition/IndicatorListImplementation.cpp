// IndicatorListImplementation.cpp: implementation of the CIndicatorListImplementation class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IndicatorListImplementation.h"

#include "../Include_Chart/Dll_Load/IndicatorInfo.h"
#include "../Include_Chart/Dll_Load/IMetaTable.h"			// for _MTEXT
#include "FileBuilder.h"
#include "Conversion.h"
#include "IndicatorInfoImplementation.h"					// for CIndicatorInfoImplementation

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUnchangeableData CIndicatorListImplementation::m_unchangeableData;

CIndicatorListImplementation::CIndicatorListImplementation( HWND p_hOcxWnd, CIndicatorList* pIndicatorList, const CString& strMainDefaultFilePathAndName, 
	const CString& strUserDefaultFilePathAndName, const CString& strUserFilePathAndName, CChartInfo::CHART_MODE	p_eChartMode):
		m_pParent(pIndicatorList),
		m_bUseUserDefaultIndicator( FALSE), 
		m_mapDefaultIndiList( m_unchangeableData.GetMainDefaultIndicatorList_Pointer(), EILT_MAP_DEFAULT, p_hOcxWnd),
		m_userIndiList( m_unchangeableData.GetMainDefaultIndicatorList_Pointer(), EILT_USER, p_hOcxWnd)
{
	// (2009/9/2 - Seung-Won, Bae) Support Chart Mode. (for HTS/FX/WorldOn)
	m_eChartMode = p_eChartMode;

	// 순서를 변경해서는 안된다! -> user default/user 는 Site default 값을 참조한다.
	m_unchangeableData.FileSUDefaultDataBuild( m_pParent, strMainDefaultFilePathAndName, strUserDefaultFilePathAndName, p_hOcxWnd);

	LoadIndicatorInfo( EILT_USER, strUserFilePathAndName);
}

CIndicatorListImplementation::~CIndicatorListImplementation()
{
}

//////////////////////////////////////////////////////////////////////
// 1. Load
//////////////////////////////////////////////////////////////////////

BOOL CIndicatorListImplementation::LoadIndicatorInfo( const EIndicatorListType p_eIndicatorListType, const char *szFilePath)
{
	if( EILT_USER != p_eIndicatorListType) return FALSE;

	if( EILT_USER == p_eIndicatorListType)
	{
		CString strFileData;
		char *szFileData = CFileBuilder().FileRead( szFilePath, strFileData);
		if( strFileData.IsEmpty()) return FALSE;

		// 지표관련 내용을 읽기
		return m_userIndiList.AddIndiInfoFromCfgString( m_pParent, szFileData, true);
	}

	return FALSE;
}
BOOL CIndicatorListImplementation::LoadIndicatorInfoFromString( const EIndicatorListType p_eIndicatorListType, const CString &p_strIndicatorList)
{
	if( p_strIndicatorList.IsEmpty()) return FALSE;

	CString strFileData;
	char *szFileData = strFileData.GetBufferSetLength( p_strIndicatorList.GetLength());
	strcpy( szFileData, p_strIndicatorList);

	switch( p_eIndicatorListType)
	{
		case EILT_USER:	return m_userIndiList.AddIndiInfoFromCfgString( m_pParent, szFileData, true);
		case EILT_MAP_DEFAULT:	return m_mapDefaultIndiList.AddIndiInfoFromCfgString( m_pParent, szFileData, true);
	}

	return FALSE;
}

//////////////////////////////////////////////////////////////////////
// 2. Save
//////////////////////////////////////////////////////////////////////

BOOL CIndicatorListImplementation::SaveIndicatorInfo( const EIndicatorListType p_eIndicatorListType, const char *szFilePath)
{
	if( !szFilePath) return FALSE;
	if( !*szFilePath) return FALSE;
	if( EILT_USER != p_eIndicatorListType && EILT_USER_DEFAULT != p_eIndicatorListType) return FALSE;

	if( EILT_USER == p_eIndicatorListType)
	{
		CStringList slDataString;
		m_userIndiList.GetWritingFileData( slDataString);
		return CFileBuilder().FileCreatWrite( szFilePath, slDataString);
	}

	if( EILT_USER_DEFAULT == p_eIndicatorListType)
	{
		CStringList slDataString;
		m_unchangeableData.GetWritingFileData( slDataString);
		return CFileBuilder().FileCreatWrite( szFilePath, slDataString);
	}

	return FALSE;
}

BOOL CIndicatorListImplementation::GetSaveIndicatorInfoString( const EIndicatorListType p_eIndicatorListType, CString &p_strIndicatorList)
{
	p_strIndicatorList.Empty();

	CStringList slDataString;

	switch( p_eIndicatorListType)
	{
		case EILT_USER:	m_userIndiList.GetWritingFileData( slDataString);
						break;
		case EILT_MAP_DEFAULT:	m_mapDefaultIndiList.GetWritingFileData( slDataString);
								break;
		default:	return FALSE;
	}

	// 1. Get Buffer Size
	int nBufferLength = 0;
	POSITION psn = slDataString.GetHeadPosition();
	while( psn)
	{
		CString &strData = slDataString.GetNext( psn);
		nBufferLength += strData.GetLength();
	}
	// 2. Set Buffer Size
	char *szBuffer = p_strIndicatorList.GetBufferSetLength( nBufferLength);
	psn = slDataString.GetHeadPosition();
	while( psn)
	{
		CString &strData = slDataString.GetNext( psn);
		strcpy( szBuffer, strData);
		szBuffer += strData.GetLength();
	}

	return nBufferLength > 0;
}

//////////////////////////////////////////////////////////////////////
// 3. Basic Info Query
//////////////////////////////////////////////////////////////////////

double CIndicatorListImplementation::GetCfgFileVersion( const EIndicatorListType p_eIndicatorListType) const
{
	if( EILT_USER != p_eIndicatorListType && EILT_SITE_DEFAULT != p_eIndicatorListType) return 0.;

	if( EILT_USER == p_eIndicatorListType)
		return m_userIndiList.GetVersion();

	if( EILT_SITE_DEFAULT == p_eIndicatorListType)
		return m_unchangeableData.GetDefaultVersion();

	return 0;
}

BOOL CIndicatorListImplementation::GetIndicatorNameList( const EIndicatorListType p_eIndicatorListType,
								const EIndicatorInfoType p_eIndicatorInfoType, CList<CString, CString>& IndicatorNameList) const
{
	if( EILT_USER != p_eIndicatorListType && EILT_SITE_DEFAULT != p_eIndicatorListType) return FALSE;

	if( EILT_USER == p_eIndicatorListType) switch( p_eIndicatorInfoType)
	{
		// (2009/9/2 - Seung-Won, Bae) Support Chart Mode. (for HTS/FX/WorldOn)
		case EIIT_GENERAL:	return m_userIndiList.GetIndicatorNameList( CBaseIndicatorList::GENERAL, IndicatorNameList, m_eChartMode);
		case EIIT_SPECIAL:	return m_userIndiList.GetIndicatorNameList( CBaseIndicatorList::SPECIAL, IndicatorNameList, m_eChartMode);
	}

	if( EILT_SITE_DEFAULT == p_eIndicatorListType) switch( p_eIndicatorInfoType)
	{
		// (2009/9/2 - Seung-Won, Bae) Support Chart Mode. (for HTS/FX/WorldOn)
		case EIIT_GENERAL:	return m_unchangeableData.GetMainDefaultIndicatorNameList_General(	IndicatorNameList, m_eChartMode);
		case EIIT_SPECIAL:	return m_unchangeableData.GetMainDefaultIndicatorNameList_Special(	IndicatorNameList, m_eChartMode);
		case EIIT_NONE:		return m_unchangeableData.GetMainDefaultIndicatorNameList_None(		IndicatorNameList, m_eChartMode);
	}

	return FALSE;
}

BOOL CIndicatorListImplementation::GetGraphNameList( const EIndicatorListType p_eIndicatorListType,
								const EIndicatorInfoType p_eIndicatorInfoType, CList<CString, CString>& graphNameList) const
{
	if( EILT_USER != p_eIndicatorListType && EILT_MAP_DEFAULT != p_eIndicatorListType) return FALSE;

	const CUserIndicatorList *pUserIndicatorList = &m_mapDefaultIndiList;
	if( EILT_USER == p_eIndicatorListType) pUserIndicatorList = &m_userIndiList;

	switch( p_eIndicatorInfoType)
	{
		// (2009/9/2 - Seung-Won, Bae) Support Chart Mode. (for HTS/FX/WorldOn)
		case EIIT_ALL:		return pUserIndicatorList->GetGraphNameList( CBaseIndicatorList::ALL,		graphNameList, m_eChartMode);
		case EIIT_GENERAL:	return pUserIndicatorList->GetGraphNameList( CBaseIndicatorList::GENERAL,	graphNameList, m_eChartMode);
		case EIIT_SPECIAL:	return pUserIndicatorList->GetGraphNameList( CBaseIndicatorList::SPECIAL,	graphNameList, m_eChartMode);
		case EIIT_NONE:		return pUserIndicatorList->GetGraphNameList( CBaseIndicatorList::NONE,		graphNameList, m_eChartMode);
	}

	return FALSE;
}

// Indicator의 Group에 관련된 내용
CIndicatorGroupDataList* CIndicatorListImplementation::GetAllIndicatorNameWithGroupName()
{
	if( m_indiGroupDataList.GetCount() > 0)
		return &m_indiGroupDataList;

	// (2009/9/2 - Seung-Won, Bae) Support Chart Mode. (for HTS/FX/WorldOn)
	m_unchangeableData.GetAllIndicatorNameWithGroupName( m_indiGroupDataList, m_eChartMode);

	return &m_indiGroupDataList;
}

const CGraphTypeAndStyleData *CIndicatorListImplementation::GetGraphTypeAndStyleData( void)
{
	return m_unchangeableData.GetGraphTypeAndStyleData();
}

//////////////////////////////////////////////////////////////////////
// 4. Indicator Info Query.
//////////////////////////////////////////////////////////////////////

// (2006/12/1 - Seung-Won, Bae) New Interface for Graph Indicator Info.
// Get Graph Info from User or MapDefault Indicator Info List.
//	Get Graph Info in User Indicator Info (Option Order)
//		1. Without MapDefault Get, With MapDefault Get, With MapDefault Get and Set		~ EIndicatorInfoGettingType
//		2. Without Indicator Info, With Indicator Info									~ p_szIndicatorName
//		3. in With Indicator Info : With SiteDefault Get, With SiteDefault Get and Set	~ EIndicatorInfoGettingType
//	Get Graph Info in MapDefault Indicator Info (Option Order)
//		1. Without Indicator Info, With Indicator Info									~ p_szIndicatorName
//		2. in With Indicator Info : With SiteDefault Get, With SiteDefault Get and Set	~ EIndicatorInfoGettingType
CIndicatorInfo *CIndicatorListImplementation::GetGraphIndicatorInfo( const EIndicatorListType p_eIndicatorListType, const char *p_szGraphName,
																	const char *p_szIndicatorName, EIndicatorInfoGettingType p_nDefaultOption)
{
	if( !p_szGraphName) return NULL;
	if( !*p_szGraphName) return NULL;
	if( EILT_USER != p_eIndicatorListType && EILT_MAP_DEFAULT != p_eIndicatorListType) return NULL;

	CBaseIndicatorList *pTargetIndicatorList = NULL;
	// 1. Select Target Indicator List
	if( EILT_USER == p_eIndicatorListType)	pTargetIndicatorList = &m_userIndiList;
	else									pTargetIndicatorList = &m_mapDefaultIndiList;
	// 2. Get Indicator Info from target or without defualt
	// (2009/9/2 - Seung-Won, Bae) Support Chart Mode. (for HTS/FX/WorldOn)
	CIndicatorInfo *pIndicatorInfo = pTargetIndicatorList->GetIndicatorInfo( p_szGraphName, m_eChartMode);
	if( pIndicatorInfo) return pIndicatorInfo;

	//	Get Graph Info in User Indicator Info (Option Order)
	//		1. Without MapDefault Get		~ EIndicatorInfoGettingType
	if( EIIGT_WITHOUT_DEFAULT == p_nDefaultOption) return NULL;

	// 3. Get Indicator Info with default
	CBaseIndicatorList *pDefaultIndicatorList = NULL;
	switch( p_eIndicatorListType)
	{
		//	Get Graph Info in User Indicator Info (Option Order)
		//		1. With MapDefault Get		~ EIndicatorInfoGettingType
		case EILT_USER:			pDefaultIndicatorList = &m_mapDefaultIndiList;
								// (2009/9/2 - Seung-Won, Bae) Support Chart Mode. (for HTS/FX/WorldOn)
								pIndicatorInfo = pDefaultIndicatorList->GetIndicatorInfo( p_szGraphName, m_eChartMode);
								if( pIndicatorInfo) break;

		//	Get Graph Info in MapDefault Indicator Info (Option Order)
		//		1. Without Indicator Info, With Indicator Info				~ p_szIndicatorName
		//		2. in With Indicator Info : With SiteDefault Get			~ EIndicatorInfoGettingType
		case EILT_MAP_DEFAULT:	if( !p_szIndicatorName) break;
								if( !*p_szIndicatorName) break;
								if( m_bUseUserDefaultIndicator)
								{
									pDefaultIndicatorList = m_unchangeableData.GetUserDefaultIndiList();
									// (2009/9/2 - Seung-Won, Bae) Support Chart Mode. (for HTS/FX/WorldOn)
									pIndicatorInfo = pDefaultIndicatorList->GetIndicatorInfo( p_szIndicatorName, m_eChartMode);
									if( pIndicatorInfo) break;
								}
								pDefaultIndicatorList = m_unchangeableData.GetMainDefaultIndicatorList_Pointer();
								// (2009/9/2 - Seung-Won, Bae) Support Chart Mode. (for HTS/FX/WorldOn)
								pIndicatorInfo = pDefaultIndicatorList->GetIndicatorInfo( p_szIndicatorName, m_eChartMode);
	}
	if( !pIndicatorInfo) return NULL;
	if( EIIGT_WITH_DEFAULT_GET_AND_SET != p_nDefaultOption) return pIndicatorInfo;

	// 4. Set Indicator Info with default
	//	With MapDefault Get and Set
	if( pTargetIndicatorList)
	{
		CIndicatorInfo *pIndicatorInfo2 = CIndicatorInfo::Make( pIndicatorInfo);
		if( pIndicatorInfo2)
		{
			CGraphDataBase *pGraphDataBase = pIndicatorInfo2->GetGraphData_PointerType();
			if( pGraphDataBase) pGraphDataBase->SetGraphName( p_szGraphName);
			if( !pTargetIndicatorList->AddIndicatorInfo( pIndicatorInfo2)) delete pIndicatorInfo2;
			else pIndicatorInfo = pIndicatorInfo2;
		}
	}

	return pIndicatorInfo;
}

// Create New Graph Indicator Info
CIndicatorInfo *CIndicatorListImplementation::CreateGraphIndicatorInfo( const char *p_szIndicatorName)
{
	if( !p_szIndicatorName) return NULL;
	if( !*p_szIndicatorName) return NULL;

	// 3. Get Indicator Info with default
	CIndicatorInfo *pIndicatorInfo = NULL;
	//	Get Graph Info in MapDefault Indicator Info (Option Order)
	//		1. Without Indicator Info, With Indicator Info				~ p_szIndicatorName
	if( m_bUseUserDefaultIndicator)
	{
		CBaseIndicatorList *pDefaultIndicatorList = m_unchangeableData.GetUserDefaultIndiList();
		// (2009/9/2 - Seung-Won, Bae) Support Chart Mode. (for HTS/FX/WorldOn)
		if( pDefaultIndicatorList) pIndicatorInfo = pDefaultIndicatorList->GetIndicatorInfo( p_szIndicatorName, m_eChartMode);
	}
	//		2. in With Indicator Info : With SiteDefault Get			~ EIndicatorInfoGettingType
	// (2009/9/2 - Seung-Won, Bae) Support Chart Mode. (for HTS/FX/WorldOn)
	if( !pIndicatorInfo) pIndicatorInfo = m_unchangeableData.GetMainDefaultIndicatorList().GetIndicatorInfo( p_szIndicatorName, m_eChartMode);

	// (2006/12/10 - Seung-Won, Bae) Make new Graph Name
	CString strGraphName( p_szIndicatorName);
	CIndicatorInfo *pIndicatorInfo2 = pIndicatorInfo;
	while( pIndicatorInfo2 )
	{
		// (2009/9/2 - Seung-Won, Bae) Support Chart Mode. (for HTS/FX/WorldOn)
		pIndicatorInfo2 = m_userIndiList.GetIndicatorInfo( strGraphName, m_eChartMode);
		if( pIndicatorInfo2 ) strGraphName.Format( "#%04X", rand());
	}

	// 4. Set Indicator Info with default
	//	With MapDefault Get and Set
	pIndicatorInfo = CIndicatorInfo::Make( pIndicatorInfo);
	if( pIndicatorInfo)
	{
		CGraphData *pGraphData = pIndicatorInfo->GetGraphData_PointerType();
		if( pGraphData) ( ( CGraphDataBase *)pGraphData)->SetGraphName( strGraphName);
		if( !m_userIndiList.AddIndicatorInfo( pIndicatorInfo))
		{
			delete pIndicatorInfo;
			pIndicatorInfo = NULL;
		}
		else
		{
			// (2009/9/12 - Seung-Won, Bae) for WorldOn's OHLC
			if( CChartInfo::WORLD_ON == m_eChartMode)
			{
				if( !strcmp( p_szIndicatorName, _MTEXT( C2_PRICE_CHART)))
				{
					CGraphDataDrawingData &graphDataDrawingData = pGraphData->GetGraphDataDrawingData();
					graphDataDrawingData.SetDoesCalcRiseFallRate_HighCur( false);
					graphDataDrawingData.SetDoesCalcRiseFallRate_LowCur( false);
					graphDataDrawingData.SetDoesCalcRiseFallRate_LowHigh( false);
					graphDataDrawingData.SetShowOHLC( TRUE);
				}
			}
		}
	}

	return pIndicatorInfo;
}

// Get First Graph Info with Indicator Name from User or MapDefault Indicator Info List without Default.
CIndicatorInfo *CIndicatorListImplementation::GetFirstIndicatorInfo( const EIndicatorListType p_eIndicatorListType, const char *p_szIndicatorName,
										POSITION *p_ppsnIndicator)
{
	if( p_ppsnIndicator) *p_ppsnIndicator = NULL;

	if( !p_szIndicatorName) return NULL;
	if( !*p_szIndicatorName) return NULL;
	if( EILT_USER != p_eIndicatorListType && EILT_MAP_DEFAULT != p_eIndicatorListType) return NULL;

	CBaseIndicatorList *pTargetIndicatorList = NULL;
	// 1. Select Target Indicator List
	if( EILT_USER == p_eIndicatorListType)	pTargetIndicatorList = &m_userIndiList;
	else									pTargetIndicatorList = &m_mapDefaultIndiList;
	// 2. Get Indicator Info from target or without defualt
	// (2009/9/2 - Seung-Won, Bae) Support Chart Mode. (for HTS/FX/WorldOn)
	return pTargetIndicatorList->GetFirstIndicatorInfo( p_szIndicatorName, p_ppsnIndicator, m_eChartMode);
}

// Get Next Graph Info with Indicator Name in Appointed List.
void CIndicatorListImplementation::GetNextIndicatorInfo( const EIndicatorListType p_eIndicatorListType, const char *p_szIndicatorName,
											POSITION *p_ppsnIndicator, CIndicatorInfo *&p_pPrevIndicatorInfo)
{
	if( !p_ppsnIndicator)			p_pPrevIndicatorInfo = NULL;
	if( !p_pPrevIndicatorInfo)
	{
		*p_ppsnIndicator = NULL;
		return;
	}

	EIndicatorListType eIndicatorListType = ( EIndicatorListType)-1;
	if( p_szIndicatorName) if( *p_szIndicatorName) eIndicatorListType = p_eIndicatorListType;
	if( EILT_USER != p_eIndicatorListType && EILT_MAP_DEFAULT != p_eIndicatorListType)
	{
		*p_ppsnIndicator = NULL;
		p_pPrevIndicatorInfo = NULL;
		return;
	}

	CBaseIndicatorList *pIndicatorList = NULL;
	// 1. Select Target Indicator List
	if( EILT_USER == p_eIndicatorListType)	pIndicatorList = &m_userIndiList;
	else									pIndicatorList = &m_mapDefaultIndiList;
	// 2. Get Indicator Info from target or without defualt
	pIndicatorList->GetNextIndicatorInfo( p_szIndicatorName, p_ppsnIndicator, p_pPrevIndicatorInfo);
}

// Get Indicator Info from UserDefault or SiteDefault Indicator Info List (Without Default Get, With Default Get, With Default Get and Set)
CIndicatorInfo *CIndicatorListImplementation::GetDefaultIndicatorInfo( const EIndicatorListType p_eIndicatorListType, const char *p_szIndicatorName,
											EIndicatorInfoGettingType p_nDefaultOption)
{
	if( !p_szIndicatorName) return NULL;
	if( !*p_szIndicatorName) return NULL;
	if( EILT_USER_DEFAULT != p_eIndicatorListType && EILT_SITE_DEFAULT != p_eIndicatorListType) return NULL;

	CBaseIndicatorList *pTargetIndicatorList = NULL;
	// 1. Select Target Indicator List
	if( EILT_USER_DEFAULT == p_eIndicatorListType)	pTargetIndicatorList = m_unchangeableData.GetUserDefaultIndiList();
	else											pTargetIndicatorList = m_unchangeableData.GetMainDefaultIndicatorList_Pointer();
	// 2. Get Indicator Info from target or without defualt
	// (2009/9/2 - Seung-Won, Bae) Support Chart Mode. (for HTS/FX/WorldOn)
	CIndicatorInfo *pIndicatorInfo = pTargetIndicatorList->GetIndicatorInfo( p_szIndicatorName, m_eChartMode);
	if( pIndicatorInfo) return pIndicatorInfo;

	// (Without Default Get)
	if( EIIGT_WITHOUT_DEFAULT == p_nDefaultOption || EILT_SITE_DEFAULT == p_eIndicatorListType) return NULL;

	// 3. Get Indicator Info with default
	// (With Default Get)
	CBaseIndicatorList *pDefaultIndicatorList = m_unchangeableData.GetMainDefaultIndicatorList_Pointer();
	// (2009/9/2 - Seung-Won, Bae) Support Chart Mode. (for HTS/FX/WorldOn)
	pIndicatorInfo = pDefaultIndicatorList->GetIndicatorInfo( p_szIndicatorName, m_eChartMode);
	if( !pIndicatorInfo) return NULL;
	if( EIIGT_WITH_DEFAULT_GET_AND_SET != p_nDefaultOption) return pIndicatorInfo;

	// 4. Set Indicator Info with default
	//	(With MapDefault Get and Set)
	if( pTargetIndicatorList)
	{
		CIndicatorInfo *pIndicatorInfo2 = CIndicatorInfo::Make( pIndicatorInfo);
		if( pIndicatorInfo2)
		{
			if( !pTargetIndicatorList->AddIndicatorInfo( pIndicatorInfo2)) delete pIndicatorInfo2;
			else pIndicatorInfo = pIndicatorInfo2;
		}
	}

	return pIndicatorInfo;
}

//////////////////////////////////////////////////////////////////////
// 5. Remove
//	Deleting with appointment is not supported in Map and Site Default Indicator Info List.
//////////////////////////////////////////////////////////////////////

// Remove Indicator Info in User Indicator Info List
BOOL CIndicatorListImplementation::RemoveGraphIndicatorInfo( const char *p_szGraphName)
{
	return m_userIndiList.Delete( GetGraphIndicatorInfo( EILT_USER, p_szGraphName, NULL, EIIGT_WITHOUT_DEFAULT));
}

BOOL CIndicatorListImplementation::RemoveAllIndicatorInfo( const EIndicatorListType p_eIndicatorListType)
{
	if( EILT_USER != p_eIndicatorListType && EILT_MAP_DEFAULT != p_eIndicatorListType) return FALSE;

	if( EILT_USER == p_eIndicatorListType)
		return m_userIndiList.DeleteAll(CBaseIndicatorList::ALL);

	if( EILT_MAP_DEFAULT == p_eIndicatorListType)
		return m_mapDefaultIndiList.DeleteAll(CBaseIndicatorList::ALL);

	return FALSE;
}

BOOL CIndicatorListImplementation::RemoveAllIndicatorInfo( const EIndicatorListType p_eIndicatorListType, const char *p_szIndicatorName)
{
	if( !p_szIndicatorName) return FALSE;
	if( !*p_szIndicatorName) return FALSE;

	if( EILT_USER != p_eIndicatorListType && EILT_MAP_DEFAULT != p_eIndicatorListType) return FALSE;

	if( EILT_USER == p_eIndicatorListType)
		return m_userIndiList.RemoveAllIndicator( CBaseIndicatorList::ALL, p_szIndicatorName);

	if( EILT_MAP_DEFAULT == p_eIndicatorListType)
		return m_mapDefaultIndiList.RemoveAllIndicator( CBaseIndicatorList::ALL, p_szIndicatorName);

	return FALSE;
}
