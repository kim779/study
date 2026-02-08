// IWGraphInfoManager.cpp: implementation of the CIWGraphInfoManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IWGraphInfoManager.h"

#include "../../Include_Chart/Dll_Load/GraphTypeData.h"			// for CGraphTypeAndStyleData
#include "../../Include_Chart/Dll_Load/IndicatorGroupData.h"	// for CIndicatorGroupDataList
#include "../../Include_Chart/DLL_Load/IndicatorList.h"			// for EIIGT_WITHOUT_DEFAULT
#include "GraphData.h"											// for CSubGraphData
#include "IndicatorInfo.h"										// for CIndicatorInfo
#include "IWGraphInfo.h"										// for CIWGraphInfo
#include "_IStringList.h"										// for IStringList
#include "IWStringList.h"										// for CIWStringList
#include "IWString.h"											// for CIWString

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// _IWRAPPER_IUNKNOWN_MANAGER_IMP
//////////////////////////////////////////////////////////////////////
_IWRAPPER_IUNKNOWN_MANAGER_IMP( CIWGraphInfoManager)

//////////////////////////////////////////////////////////////////////
// Overrides
//////////////////////////////////////////////////////////////////////

// [04/10/14] Graph의 6색상을 조회하는 Interface를 제공한다.
BOOL CIWGraphInfoManager::GetGraph6Color( BOOL p_bDefaultInfo, const char *p_szGraphName, int p_nSubIndex, COLORREF p_aclrGraphColor6[ 6])
{
	// 1. Indicator Info를 구한다.
	CIndicatorInfo *pIndicatorInfo = NULL;
	if( p_bDefaultInfo) pIndicatorInfo = m_pIndicatorList->GetDefaultIndicatorInfo( EILT_USER_DEFAULT, p_szGraphName, EIIGT_WITH_DEFAULT_GET);
	else pIndicatorInfo = m_pIndicatorList->GetGraphIndicatorInfo( EILT_USER, p_szGraphName, p_szGraphName, EIIGT_WITH_DEFAULT_GET_AND_SET);
	if( !pIndicatorInfo) return FALSE;

	// 2. SubGraph를 구한다.
	CSubGraphData *pSubGraphData = pIndicatorInfo->GetSubGraphData( p_nSubIndex);
	if( !pSubGraphData) return FALSE;

	// 3. Graph Color를 구한다.
	CGraphColor *pGraphColor = pSubGraphData->GetColor_PointerType();
	if( !pGraphColor) return FALSE;
	p_aclrGraphColor6[ 0] = pGraphColor->GetColor1();
	p_aclrGraphColor6[ 1] = pGraphColor->GetColor2();
	p_aclrGraphColor6[ 2] = pGraphColor->GetColor3();
	p_aclrGraphColor6[ 3] = pGraphColor->GetColor4();
	p_aclrGraphColor6[ 4] = pGraphColor->GetColor5();
	p_aclrGraphColor6[ 5] = pGraphColor->GetColor6();
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// [05/07/15] Getting Graph Info Interface
//////////////////////////////////////////////////////////////////////
IGraphInfo *CIWGraphInfoManager::GetGraphInfo( IEGraphInfoListType p_eGraphInfoType, const char *p_szGraphName)
{
	if( !m_pIndicatorList) return NULL;
	CIndicatorInfo *pIndicatorInfo = m_pIndicatorList->GetGraphIndicatorInfo( ( EIndicatorListType)p_eGraphInfoType, p_szGraphName, NULL, EIIGT_WITHOUT_DEFAULT);
	if( !pIndicatorInfo) return NULL;
	return CIWGraphInfo::CreateObject( pIndicatorInfo);
}

//////////////////////////////////////////////////////////////////////
// (2006/7/6 - Seung-Won, Bae) Retrieve the Graph Type and Style Infos.
//////////////////////////////////////////////////////////////////////
IStringList *CIWGraphInfoManager::GetAllGraphTypeNames( void)
{
	if( !m_pIndicatorList) return NULL;

	const CGraphTypeAndStyleData *pGraphTypeAndStyleData = m_pIndicatorList->GetGraphTypeAndStyleData();
	if( !pGraphTypeAndStyleData) return 0;

	CStringDataList *pslGraphTypeNames = new CStringDataList;
	pGraphTypeAndStyleData->GetAllGraphTypeList( *pslGraphTypeNames);

	return CIWStringList::CreateObject( pslGraphTypeNames, TRUE);
}
BOOL CIWGraphInfoManager::GetGraphTypeAndStyle( int nType, int nStyle, ILPCSTR &p_pszType, ILPCSTR &p_pszStyle)
{
	p_pszType	= NULL;
	p_pszStyle	= NULL;
	if( !m_pIndicatorList) return FALSE;

	const CGraphTypeAndStyleData *pGraphTypeAndStyleData = m_pIndicatorList->GetGraphTypeAndStyleData();
	if( !pGraphTypeAndStyleData) return FALSE;

	CString strType, strStyle;
	BOOL bResult = pGraphTypeAndStyleData->GetGraphTypeAndStyle( nType, nStyle, strType, strStyle);
	p_pszType	= CIWString::CreateObject( strType);
	p_pszStyle	= CIWString::CreateObject( strStyle);
	return bResult;
}
BOOL CIWGraphInfoManager::GetGraphTypeAndStyle( const char *p_szTypeName, const char *p_szStyleName, int &p_nTypeIndex, int &p_nStyleIndex)
{
	p_nTypeIndex	= -1;
	p_nStyleIndex	= -1;
	if( !m_pIndicatorList) return FALSE;

	const CGraphTypeAndStyleData *pGraphTypeAndStyleData = m_pIndicatorList->GetGraphTypeAndStyleData();
	if( !pGraphTypeAndStyleData) return FALSE;

	return pGraphTypeAndStyleData->GetGraphTypeAndStyle( p_szTypeName, p_szStyleName, p_nTypeIndex, p_nStyleIndex);
}
BOOL CIWGraphInfoManager::GetGraphTypeAndStyle( const char *p_szTypeName, const char *p_szStyleName, const char *p_szDrawStyleName, int &p_nTypeIndex, int &p_nStyleIndex, int &p_nDrawStyleIndex)
{
	p_nTypeIndex		= -1;
	p_nStyleIndex		= -1;
	p_nDrawStyleIndex	= -1;
	if( !m_pIndicatorList) return FALSE;

	const CGraphTypeAndStyleData *pGraphTypeAndStyleData = m_pIndicatorList->GetGraphTypeAndStyleData();
	if( !pGraphTypeAndStyleData) return FALSE;

	return pGraphTypeAndStyleData->GetGraphTypeAndStyle( p_szTypeName, p_szStyleName, p_szDrawStyleName, p_nTypeIndex, p_nStyleIndex, p_nDrawStyleIndex);
}
IStringList *CIWGraphInfoManager::GetAllGraphTypeStyleNameList( const char *p_szGraphTypeName)
{
	if( !m_pIndicatorList) return NULL;
	const CGraphTypeAndStyleData *pStyleData = m_pIndicatorList->GetGraphTypeAndStyleData();
	CList< CString, CString> *plStringList = pStyleData->GetAllGraphStyleList( CGraphTypeAndStyleData::TYPE_STYLE, p_szGraphTypeName);
	if( !plStringList) return NULL;
	return CIWStringList::CreateObject( plStringList);
}
IStringList *CIWGraphInfoManager::GetAllGraphDrawStyleNameList( const char *p_szGraphTypeName)
{
	if( !m_pIndicatorList) return NULL;
	const CGraphTypeAndStyleData *pStyleData = m_pIndicatorList->GetGraphTypeAndStyleData();
	CList< CString, CString> *plStringList = pStyleData->GetAllGraphStyleList( CGraphTypeAndStyleData::DRAW_STYLE, p_szGraphTypeName);
	if( !plStringList) return NULL;
	return CIWStringList::CreateObject( plStringList);
}


//////////////////////////////////////////////////////////////////////
// (2006/7/6 - Seung-Won, Bae) Retrieve the Indicator Names
//////////////////////////////////////////////////////////////////////
IStringList *CIWGraphInfoManager::GetIndicatorGroupNameStringList( void)
{
	if( !m_pIndicatorList) return NULL;

	// 1. Indicator Group List를 구한다.
	CIndicatorGroupDataList *pIndicatorGroupList = m_pIndicatorList->GetAllIndicatorNameWithGroupName();
	if( !pIndicatorGroupList) return NULL;

	CStringDataList *pslGraphNameList = new CStringDataList;
	// 2. 각 Indicator Group에 대해서, Indicator Name을 구한다.
	POSITION posIndicatorGroup = pIndicatorGroupList->GetHeadPosition();
	while( posIndicatorGroup)
	{
		// 2.1 먼저 Indicator Group Name을 구한다.
		CIndicatorGroupData *pIndicatorGroup = pIndicatorGroupList->GetNext( posIndicatorGroup);
		pslGraphNameList->AddTail( pIndicatorGroup->GetGroupName());
	}

	return CIWStringList::CreateObject( pslGraphNameList, TRUE);
}
IStringList *CIWGraphInfoManager::GetIndicatorNameStringList( const char *p_szGroupName)
{
	if( !m_pIndicatorList || !p_szGroupName) return NULL;

	// 1. Indicator Group List를 구한다.
	CIndicatorGroupDataList *pIndicatorGroupList = m_pIndicatorList->GetAllIndicatorNameWithGroupName();
	if( !pIndicatorGroupList) return NULL;

	CStringDataList *pslGraphNameList = NULL;
	// 2. 각 Indicator Group에 대해서, Indicator Name을 구한다.
	POSITION posIndicatorGroup = pIndicatorGroupList->GetHeadPosition();
	while( posIndicatorGroup)
	{
		// 2.1 먼저 Indicator Group Name을 구한다.
		CIndicatorGroupData *pIndicatorGroup = pIndicatorGroupList->GetNext( posIndicatorGroup);
		if( pIndicatorGroup->GetGroupName() == p_szGroupName)
		{
			pslGraphNameList = pIndicatorGroup->GetIndicatorNameList();
			break;
		}
	}
	if( !pslGraphNameList) return NULL;
	return CIWStringList::CreateObject( pslGraphNameList);
}
IStringList *CIWGraphInfoManager::GetIndicatorNameStringList( IEGraphInfoListType p_eGraphInfoListType, IEGraphInfoType p_eIndicatorInfoType)
{
	if( !m_pIndicatorList) return NULL;

	CStringDataList *pslGraphNameList = new CStringDataList;
	m_pIndicatorList->GetIndicatorNameList( ( EIndicatorListType)p_eGraphInfoListType, ( EIndicatorInfoType)p_eIndicatorInfoType, *pslGraphNameList);

	return CIWStringList::CreateObject( pslGraphNameList, TRUE);
}


//////////////////////////////////////////////////////////////////////
// (2006/8/15 - Seung-Won, Bae) Control GraphInfo
//////////////////////////////////////////////////////////////////////
BOOL CIWGraphInfoManager::RemoveAllIndicatorInfo( IEGraphInfoListType p_eGraphInfoListType)
{
	if( !m_pIndicatorList) return FALSE;
	return m_pIndicatorList->RemoveAllIndicatorInfo( ( EIndicatorListType)p_eGraphInfoListType);
}
