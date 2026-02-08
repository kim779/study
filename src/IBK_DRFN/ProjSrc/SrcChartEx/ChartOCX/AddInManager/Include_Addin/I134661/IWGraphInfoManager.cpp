// IWGraphInfoManager.cpp: implementation of the CIWGraphInfoManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IWGraphInfoManager.h"

#include "../../Include_Chart/Dll_Load/GraphTypeData.h"	// for CGraphTypeAndStyleData
#include "GraphData.h"									// for CSubGraphData
#include "IndicatorInfo.h"								// for CIndicatorInfo
#include "IWGraphInfo.h"

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


/*-----------------------------------------------------------------------------------------
 - Function    :  GetCommonConditionList
 - Created at  :  2005-07-14   10:19
 - Author      :  최종찬
 - Parameters  :  pcszGraphName - 지표명
 - Return Value:  CList<double, double>* - 설정값 리스트
 - Description :  해당 지표의 설정값을 가져온다.
 -----------------------------------------------------------------------------------------*/
CList<double, double>* CIWGraphInfoManager::GetCommonConditionList(const char* pcszGraphName)
{
	CIndicatorInfo* pIndicatorInfo = m_pIndicatorList->GetGraphIndicatorInfo( EILT_USER, pcszGraphName, pcszGraphName, EIIGT_WITH_DEFAULT_GET_AND_SET);
	if(!pIndicatorInfo)
		return NULL;

	return pIndicatorInfo->GetCommonCalConditionList();
}


/*-----------------------------------------------------------------------------------------
 - Function    :  GetConditionList
 - Created at  :  2005-07-14   10:24
 - Author      :  최종찬
 - Parameters  :  pcszGraphName  - 지표명
 -                nSubGraphIndex - 지표의 서브그래프 인덱스
 - Return Value:  CList<double, double>* - 설정값 리스트
 - Description :  해당 지표의 설정값을 가져온다.
 -----------------------------------------------------------------------------------------*/
CList<double, double>* CIWGraphInfoManager::GetConditionList(const char* pcszGraphName, const int nSubGraphIndex)
{
	CIndicatorInfo* pIndicatorInfo = m_pIndicatorList->GetGraphIndicatorInfo( EILT_USER, pcszGraphName, pcszGraphName, EIIGT_WITH_DEFAULT_GET_AND_SET);
	if(!pIndicatorInfo)
		return NULL;

	return pIndicatorInfo->GetCalConditionList(nSubGraphIndex);
}


/*-----------------------------------------------------------------------------------------
 - Function    :  GetConditionList
 - Created at  :  2005-07-14   10:25
 - Author      :  최종찬
 - Parameters  :  pcszGraphName    - 지표명
 -                pcszSubGraphName - 지표의 서브그래프 명
 - Return Value:  CList<double, double>* - 설정값 리스트
 - Description :  해당 지표의 설정값을 가져온다.
 -----------------------------------------------------------------------------------------*/
CList<double, double>* CIWGraphInfoManager::GetConditionList(const char* pcszGraphName, const char* pcszSubGraphName)
{
	CIndicatorInfo* pIndicatorInfo = m_pIndicatorList->GetGraphIndicatorInfo( EILT_USER, pcszGraphName, pcszGraphName, EIIGT_WITH_DEFAULT_GET_AND_SET);
	if(!pIndicatorInfo)
		return NULL;

	return pIndicatorInfo->GetCalConditionList(pcszSubGraphName);
}

//////////////////////////////////////////////////////////////////////
// [05/07/15] Getting Graph Info Interface
//////////////////////////////////////////////////////////////////////
IGraphInfo *CIWGraphInfoManager::GetGraphInfo( IEGraphInfoListType p_eGraphInfoType, const char *p_szGraphName, IEGraphInfoGettingType p_nDefaultOption)
{
	if( !m_pIndicatorList) return NULL;
	CIndicatorInfo *pIndicatorInfo = m_pIndicatorList->GetGraphIndicatorInfo( ( EIndicatorListType)p_eGraphInfoType, p_szGraphName, p_szGraphName, ( EIndicatorInfoGettingType)p_nDefaultOption);
	if( !pIndicatorInfo) return NULL;
	return CIWGraphInfo::CreateObject( pIndicatorInfo);
}


//////////////////////////////////////////////////////////////////////
// (2006/7/6 - Seung-Won, Bae) Retrieve the Graph Type and Style Infos.
//////////////////////////////////////////////////////////////////////
int CIWGraphInfoManager::GetAllGraphTypeNames( CList< CString, CString> &p_slGraphTypeNames)
{
	while( !p_slGraphTypeNames.IsEmpty()) p_slGraphTypeNames.RemoveHead();
	if( !m_pIndicatorList) return 0;

	const CGraphTypeAndStyleData *pGraphTypeAndStyleData = m_pIndicatorList->GetGraphTypeAndStyleData();
	if( !pGraphTypeAndStyleData) return 0;

	pGraphTypeAndStyleData->GetAllGraphTypeList( p_slGraphTypeNames);
	return p_slGraphTypeNames.GetCount();
}
BOOL CIWGraphInfoManager::GetGraphTypeAndStyle( int nType, int nStyle, CString& strType, CString& strStyle)
{
	strType.Empty();
	strStyle.Empty();
	if( !m_pIndicatorList) return FALSE;

	const CGraphTypeAndStyleData *pGraphTypeAndStyleData = m_pIndicatorList->GetGraphTypeAndStyleData();
	if( !pGraphTypeAndStyleData) return FALSE;

	return pGraphTypeAndStyleData->GetGraphTypeAndStyle( nType, nStyle, strType, strStyle);
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
CList< CString, CString> *CIWGraphInfoManager::GetAllGraphTypeStyleNameList( const char *p_szGraphTypeName)
{
	if( !m_pIndicatorList) return NULL;
	const CGraphTypeAndStyleData *pStyleData = m_pIndicatorList->GetGraphTypeAndStyleData();
	return pStyleData->GetAllGraphStyleList( CGraphTypeAndStyleData::TYPE_STYLE, p_szGraphTypeName);
}
CList< CString, CString> *CIWGraphInfoManager::GetAllGraphDrawStyleNameList( const char *p_szGraphTypeName)
{
	if( !m_pIndicatorList) return NULL;
	const CGraphTypeAndStyleData *pStyleData = m_pIndicatorList->GetGraphTypeAndStyleData();
	return pStyleData->GetAllGraphStyleList( CGraphTypeAndStyleData::DRAW_STYLE, p_szGraphTypeName);
}


//////////////////////////////////////////////////////////////////////
// (2006/7/6 - Seung-Won, Bae) Retrieve the Indicator Names
//////////////////////////////////////////////////////////////////////
BOOL CIWGraphInfoManager::GetIndicatorNameList( IEGraphInfoListType p_eGraphInfoListType, IEGraphInfoType p_eIndicatorInfoType, CList<CString, CString>& GraphNameList)
{
	while( !GraphNameList.IsEmpty()) GraphNameList.RemoveHead();
	if( !m_pIndicatorList) return FALSE;

	m_pIndicatorList->GetIndicatorNameList( ( EIndicatorListType)p_eGraphInfoListType, ( EIndicatorInfoType)p_eIndicatorInfoType, GraphNameList);
	return !GraphNameList.IsEmpty();
}
