// IndicatorList.h: interface for the CIndicatorList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INDICATORLIST_H__B19E87F5_4EF5_4C17_938D_7B243D75730F__INCLUDED_)
#define AFX_INDICATORLIST_H__B19E87F5_4EF5_4C17_938D_7B243D75730F__INCLUDED_

#include <afxtempl.h>
#include <list>
using namespace std;

#include "../BlockBaseEnum.h"		// for CChartInfo

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// (2006/5/13 - Seung-Won, Bae) 4 Target Define for CIndicatorList's Interface
typedef enum
{
	EILT_SITE_DEFAULT,		// 4
	EILT_MAP_DEFAULT,		// 2	// (2006/12/1 - Seung-Won, Bae) Order Changed with Same Enum Value
	EILT_USER_DEFAULT,		// 3
	EILT_USER				// 1
} EIndicatorListType;

// (2006/5/13 - Seung-Won, Bae) 3 Type Define for CIndicatorList's Interface
typedef enum
{
	EIIT_ALL = -1,
	EIIT_GENERAL,
	EIIT_SPECIAL,
	EIIT_NONE
} EIndicatorInfoType;

// (2006/5/31 - Seung-Won, Bae) 3 Type Defint for GetIndicatorInfo() Operation.
typedef enum
{
	EIIGT_WITHOUT_DEFAULT,
	EIIGT_WITH_DEFAULT_GET,
	EIIGT_WITH_DEFAULT_GET_AND_SET
} EIndicatorInfoGettingType;


class CIndicatorInfo;
class CGraphTypeAndStyleData;
class CIndicatorGroupDataList;
class CIndicatorListImplementation;

class AFX_EXT_CLASS CIndicatorList  
{
public:
	CIndicatorList( HWND p_hOcxWnd, const CString& strMainDefaultFilePathAndName, const CString& strUserDefaultFilePathAndName, const CString& strUserFilePathAndName, CChartInfo::CHART_MODE p_eChartMode);
	virtual ~CIndicatorList();

// 0. Manage Implementation Class
private:
	CIndicatorListImplementation* m_pIndicatorListImpl;

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
											POSITION *p_ppsnIndicator = NULL);
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
};

#endif // !defined(AFX_INDICATORLIST_H__B19E87F5_4EF5_4C17_938D_7B243D75730F__INCLUDED_)
