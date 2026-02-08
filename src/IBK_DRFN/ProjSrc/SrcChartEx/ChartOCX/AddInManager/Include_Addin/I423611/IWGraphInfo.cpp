// IWGraphInfo.cpp: implementation of the CIWGraphInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IWGraphInfo.h"

#include "../../../Include_Chart/Graph.h"		// for CGraph
#include "../../../Include_Chart/GraphData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// _IWRAPPER_IUNKNOWN_MANAGER_IMP
//////////////////////////////////////////////////////////////////////
_IWRAPPER_IUNKNOWN_MANAGER_IMP( CIWGraphInfo)

//////////////////////////////////////////////////////////////////////
// // (2006/7/13 - Seung-Won, Bae) Operation.
//////////////////////////////////////////////////////////////////////

/*-----------------------------------------------------------------------------------------
 - Function    :  GetCommonCalculateVarList
 - Created at  :  2005-07-15   13:31
 - Author      :  최종찬
 - Return Value:  CList<double, double>* - 계산조건값 리스트
 - Description :  지표계산에 필요한 조건값을 리스트로 제공한다.
 -----------------------------------------------------------------------------------------*/
CList<double, double>* CIWGraphInfo::GetCommonCalculateVarList()
{
	if(!m_pIndicatorInfo)
		return NULL;
	return m_pIndicatorInfo->GetCommonCalConditionList();
}


/*-----------------------------------------------------------------------------------------
 - Function    :  GetCalculateVarList
 - Created at  :  2005-07-15   13:32
 - Author      :  최종찬
 - Parameters  :  nSubGraphIndex - 서브그래프 인덱스
 - Return Value:  CList<double, double>* - 계산조건값 리스트
 - Description :  지표계산에 필요한 조건값을 서브그래프별로 제공한다.
 -----------------------------------------------------------------------------------------*/
CList<double, double>* CIWGraphInfo::GetCalculateVarList(const int nSubGraphIndex)
{
	if(!m_pIndicatorInfo)
		return NULL;
	return m_pIndicatorInfo->GetCalConditionList(nSubGraphIndex);
}


/*-----------------------------------------------------------------------------------------
 - Function    :  GetCalculateVarList
 - Created at  :  2005-07-15   13:33
 - Author      :  최종찬
 - Parameters  :  pcszSubGraphName - 서브그래프 네임
 - Return Value:  CList<double, double>* - 계산조건값 리스트
 - Description :  지표계산에 필요한 조건값을 서브그래프별로 제공한다.
 -----------------------------------------------------------------------------------------*/
CList<double, double>* CIWGraphInfo::GetCalculateVarList(const char* pcszSubGraphName)
{
	if(!m_pIndicatorInfo)
		return NULL;

	return m_pIndicatorInfo->GetCalConditionList(pcszSubGraphName);
}


/*-----------------------------------------------------------------------------------------
 - Function    :  GetCommonCalculatePacketNames
 - Created at  :  2005-07-15   13:49
 - Author      :  최종찬
 - Return Value:  LPCTSTR - 계산할 패킷명
 - Description :  지표 계산에 필요한 패킷명을 제공한다.
 -----------------------------------------------------------------------------------------*/
LPCTSTR CIWGraphInfo::GetCommonCalculatePacketNames()
{
	if(!m_pIndicatorInfo)
		return _T("");

	return m_pIndicatorInfo->GetCommonPacketNames();
}


/*-----------------------------------------------------------------------------------------
 - Function    :  GetCalculatePacketNames
 - Created at  :  2005-07-15   13:51
 - Author      :  최종찬
 - Parameters  :  nSubGraphIndex - 서브그래프 인덱스
 - Return Value:  LPCTSTR - 계산에 필요한 패킷명
 - Description :  지표 계산에 필요한 패킷명을 제공한다. (서브그래프별로 제공)
 -----------------------------------------------------------------------------------------*/
LPCTSTR CIWGraphInfo::GetCalculatePacketNames(const int nSubGraphIndex)
{
	if(!m_pIndicatorInfo)
		return _T("");

	return m_pIndicatorInfo->GetPacketNames(nSubGraphIndex);
}


/*-----------------------------------------------------------------------------------------
 - Function    :  GetSubGraphIndex
 - Created at  :  2005-07-15   13:51
 - Author      :  최종찬
 - Parameters  :  pcszSubGraphName - 서브그래프명.
 - Return Value:  int - 서브그래프 인덱스
 - Description :  서브그래프의 인덱스를 리턴한다.
 -----------------------------------------------------------------------------------------*/
int CIWGraphInfo::GetSubGraphIndex(const char* pcszSubGraphName)
{
	if(!m_pIndicatorInfo)
		return -1;

	return m_pIndicatorInfo->GetSubGraphIndex(pcszSubGraphName);
}


/*-----------------------------------------------------------------------------------------
 - Function    :  IsShowSubGraph
 - Created at  :  2005-07-29   10:11
 - Author      :  최종찬
 - Parameters  :  pcszSubGraphName - [in] 서브그래프명.
 -                bSubGraphShow    - [out] Show/Hide여부를 리턴한다.
 - Return Value:  bool - true: 성공적 호출, false: 실패(SubGraph가 없을경우가 가장 큰 이유겠지)
 - Description :  서브그래프의 Show/Hide여부를 제공한다.
 -----------------------------------------------------------------------------------------*/
bool CIWGraphInfo::IsShowSubGraph(const char* pcszSubGraphName, bool& bSubGraphShow)
{
	bSubGraphShow = true;
	if( m_pIndicatorInfo)
	{
		// (2007/3/9 - Seung-Won, Bae) Check Graph's Hide
		CGraph *pGraph = m_pIndicatorInfo->GetGraph();
		if( pGraph) if( pGraph->IsGraphHide()) bSubGraphShow = false;

		if( bSubGraphShow) bSubGraphShow = !m_pIndicatorInfo->GetSubGraphHiding( pcszSubGraphName);
	}
	return true;
}


/*-----------------------------------------------------------------------------------------
 - Function    :  IsShowSubGraph
 - Created at  :  2005-07-29   10:14
 - Author      :  최종찬
 - Parameters  :  nSubGraphIndex - [in] SubGraph Index
 -                bSubGraphShow  - [out] Show/Hide여부를 리턴한다.
 - Return Value:  bool - true: 성공적 호출, false: 실패(SubGraph가 없을경우가 가장 큰 이유겠지)
 - Description :  서브그래프의 Show/Hide여부를 제공한다.
 -----------------------------------------------------------------------------------------*/
bool CIWGraphInfo::IsShowSubGraph(const int nSubGraphIndex, bool& bSubGraphShow)
{
	bSubGraphShow = true;
	if( m_pIndicatorInfo)
	{
		// (2007/3/9 - Seung-Won, Bae) Check Graph's Hide
		CGraph *pGraph = m_pIndicatorInfo->GetGraph();
		if( pGraph) if( pGraph->IsGraphHide()) bSubGraphShow = false;

		if( bSubGraphShow) bSubGraphShow = !m_pIndicatorInfo->GetSubGraphHiding( nSubGraphIndex);
	}
	return true;
}