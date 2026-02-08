// IndicatorListImplementation.h: interface for the CIndicatorListImplementation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INDICATORLISTIMPLEMENTATION_H__E3430B0C_BD42_4D3A_8ED8_C7F9CD9CF209__INCLUDED_)
#define AFX_INDICATORLISTIMPLEMENTATION_H__E3430B0C_BD42_4D3A_8ED8_C7F9CD9CF209__INCLUDED_

#include "../Include_Chart/DLL_Load/IndicatorList.h"		// for EIndicatorListType
#include "UnchangeableIndicatorData.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CIndicatorInfo;
class CIndicatorListImplementation  
{
public:
	CIndicatorListImplementation( HWND p_hOcxWnd, CIndicatorList* pIndicatorList, const CString& strMainDefaultFilePathAndName, 
		const CString& strUserDefaultFilePathAndName, const CString& strUserFilePathAndName, CChartInfo::CHART_MODE	p_eChartMode);
	virtual ~CIndicatorListImplementation();

// 0. Member Indicator Info List
protected:
	static CUnchangeableData	m_unchangeableData;
protected:
	BOOL						m_bUseUserDefaultIndicator; //CUnchangeableDatab 에 있는 user default사용유무
	CIndicatorList *			m_pParent;
	CUserIndicatorList			m_mapDefaultIndiList;		//화면(map)에서 설정한 default값
	CUserIndicatorList			m_userIndiList;

// 1. Load
public:
	BOOL	LoadIndicatorInfo( const EIndicatorListType p_eIndicatorListType, const char *szFilePath);
	BOOL	LoadIndicatorInfoFromString( const EIndicatorListType p_eIndicatorListType, const CString &p_strIndicatorList);

// 2. Save
public:
	BOOL	SaveIndicatorInfo( const EIndicatorListType p_eIndicatorListType, const char *szFilePath);
	BOOL	GetSaveIndicatorInfoString( const EIndicatorListType p_eIndicatorListType, CString &p_strIndicatorList);

// 3. Basic Info Query
public:
	double							GetCfgFileVersion( const EIndicatorListType p_eIndicatorListType) const;
	BOOL							GetIndicatorNameList( const EIndicatorListType p_eIndicatorListType,
										const EIndicatorInfoType p_eIndicatorInfoType, CList<CString, CString>& IndicatorNameList) const;
	BOOL							GetGraphNameList( const EIndicatorListType p_eIndicatorListType,
										const EIndicatorInfoType p_eIndicatorInfoType, CList<CString, CString>& graphNameList) const;
	CIndicatorGroupDataList *		GetAllIndicatorNameWithGroupName( void);	// Indicator의 Group에 관련된 내용
	const CGraphTypeAndStyleData *	GetGraphTypeAndStyleData( void);

// 4. Indicator Info Query.
public:
	// Get Graph Info from User or MapDefault Indicator Info List.
	//	Get Graph Info in User Indicator Info (Option Order)
	//		1. Without MapDefault Get, With MapDefault Get, With MapDefault Get and Set		~ EIndicatorInfoGettingType
	//		2. Without Indicator Info, With Indicator Info									~ p_szIndicatorName
	//		3. in With Indicator Info : With SiteDefault Get, With SiteDefault Get and Set	~ EIndicatorInfoGettingType
	//	Get Graph Info in MapDefault Indicator Info (Option Order)
	//		1. Without Indicator Info, With Indicator Info									~ p_szIndicatorName
	//		2. in With Indicator Info : With SiteDefault Get, With SiteDefault Get and Set	~ EIndicatorInfoGettingType
	CIndicatorInfo *				GetGraphIndicatorInfo( const EIndicatorListType p_eIndicatorListType, const char *p_szGraphName,
											const char *p_szIndicatorName, EIndicatorInfoGettingType p_nDefaultOption);
	// Create New Graph Indicator Info
	CIndicatorInfo *				CreateGraphIndicatorInfo( const char *p_szIndicatorName);
	// Get First Graph Info with Indicator Name from User or MapDefault Indicator Info List without Default.
	CIndicatorInfo *				GetFirstIndicatorInfo( const EIndicatorListType p_eIndicatorListType, const char *p_szIndicatorName,
											POSITION *p_ppsnIndicator);
	// Get Next Graph Info with Indicator Name in Appointed List.
	void							GetNextIndicatorInfo( const EIndicatorListType p_eIndicatorListType, const char *p_szIndicatorName,
											POSITION *p_ppsnIndicator, CIndicatorInfo *&p_pPrevIndicatorInfo);
	// Get Indicator Info from UserDefault or SiteDefault Indicator Info List (Without Default Get, With Default Get, With Default Get and Set)
	CIndicatorInfo *				GetDefaultIndicatorInfo( const EIndicatorListType p_eIndicatorListType, const char *p_szIndicatorName,
											EIndicatorInfoGettingType p_nDefaultOption);

// 5. Remove
//	Deleting with appointment is not supported in Map, User and Site Default Indicator Info List.
public:
	BOOL	RemoveGraphIndicatorInfo( const char *p_szGraphName);						// Remove Indicator Info in User Indicator Info List
	BOOL	RemoveAllIndicatorInfo( const EIndicatorListType p_eIndicatorListType);
	BOOL	RemoveAllIndicatorInfo( const EIndicatorListType p_eIndicatorListType, const char *p_szIndicatorName);

// (2009/9/2 - Seung-Won, Bae) Support Chart Mode. (for HTS/FX/WorldOn)
protected:
	CChartInfo::CHART_MODE	m_eChartMode;
	CIndicatorGroupDataList m_indiGroupDataList;
};

#endif // !defined(AFX_INDICATORLISTIMPLEMENTATION_H__E3430B0C_BD42_4D3A_8ED8_C7F9CD9CF209__INCLUDED_)
