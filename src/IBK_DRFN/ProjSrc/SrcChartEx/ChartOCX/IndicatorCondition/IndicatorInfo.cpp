// IndicatorInfo.cpp: implementation of the CIndicatorInfo class.
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../Include_Chart/Dll_Load/IndicatorInfo.h"

#include <assert.h>										// for assert()
#include "IndicatorInfoImplementation.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////
// class CIndicatorInfo

CIndicatorInfo* CIndicatorInfo::Make(const CIndicatorInfo* pDefaultIndicatorInfo)
{
	if( !pDefaultIndicatorInfo) return NULL;
	return new CIndicatorInfo( pDefaultIndicatorInfo);
}

CIndicatorInfo::CIndicatorInfo( const EIndicatorInfoType p_eIndicatorInfoType)
{
	m_pIndiInfoImpl = new CIndicatorInfoImplementation( p_eIndicatorInfoType);
}

CIndicatorInfo::CIndicatorInfo(const CIndicatorInfo* pDefaultIndicatorInfo)
{
	assert( pDefaultIndicatorInfo);
	m_pIndiInfoImpl = new CIndicatorInfoImplementation( pDefaultIndicatorInfo->m_pIndiInfoImpl);
}

CIndicatorInfo::~CIndicatorInfo()
{
	TRACE( "IndicatorInfo Deleted : [%s](0x%08X)\r\n", GetIndicatorName(), ( long)this);
	delete m_pIndiInfoImpl;
}


EIndicatorInfoType CIndicatorInfo::GetIndicatorInfoType( void) const
{
	return m_pIndiInfoImpl->GetIndicatorInfoType();
}


// public =====================================================================

bool CIndicatorInfo::CopyDataWithVerificationFrom(const CIndicatorInfo& sourceData)
{
	if(sourceData.m_pIndiInfoImpl == NULL)
		return false;

	return m_pIndiInfoImpl->CopyDataWithVerificationFrom(*sourceData.m_pIndiInfoImpl);
}

// ----------------------------------------------------------------------------
CString CIndicatorInfo::GetGroupName() const
{
	return m_pIndiInfoImpl->GetGroupName();
}

CString CIndicatorInfo::GetIndicatorName() const
{
	return m_pIndiInfoImpl->GetIndicatorName();
}

CString CIndicatorInfo::GetGraphName() const
{
	return m_pIndiInfoImpl->GetGraphName();
}

CGraphPartData* CIndicatorInfo::GetGraphPartData() const
{
	return m_pIndiInfoImpl->GetGraphPartData();
}

CGraphPartData* CIndicatorInfo::GetGraphPartData()
{
	return m_pIndiInfoImpl->GetGraphPartData();
}

int CIndicatorInfo::GetCommonCalConditionCount() const
{
	return m_pIndiInfoImpl->GetCommonCalConditionCount();
}

int CIndicatorInfo::GetDrawConditionCount() const
{
	return m_pIndiInfoImpl->GetDrawConditionCount();
}

int CIndicatorInfo::GetBaseLineCount() const
{
	return m_pIndiInfoImpl->GetBaseLineCount();
}

int CIndicatorInfo::GetCalConditionCount(const int nSubGraphIndex) const
{
	return m_pIndiInfoImpl->GetCalConditionCount(nSubGraphIndex);
}

int CIndicatorInfo::GetCalConditionCount(const CString& strSubGraphName) const
{
	return m_pIndiInfoImpl->GetCalConditionCount(strSubGraphName);
}

CList<double, double>* CIndicatorInfo::GetCommonCalConditionList() const
{
	return m_pIndiInfoImpl->GetCommonCalConditionList();
}

CList<double, double>* CIndicatorInfo::GetCommonCalConditionList()
{
	return m_pIndiInfoImpl->GetCommonCalConditionList();
}

CList<double, double>* CIndicatorInfo::GetDrawConditionList() const
{
	return m_pIndiInfoImpl->GetDrawConditionList();
}

CList<double, double>* CIndicatorInfo::GetDrawConditionList()
{
	return m_pIndiInfoImpl->GetDrawConditionList();
}

CList<double, double>* CIndicatorInfo::GetBaseLineList() const
{
	return m_pIndiInfoImpl->GetBaseLineList();
}

CList<double, double>* CIndicatorInfo::GetBaseLineList()
{
	return m_pIndiInfoImpl->GetBaseLineList();
}

CList<double, double>* CIndicatorInfo::GetCalConditionList(const int nSubGraphIndex) const
{
	return m_pIndiInfoImpl->GetCalConditionList(nSubGraphIndex);
}

CList<double, double>* CIndicatorInfo::GetCalConditionList(const int nSubGraphIndex)
{
	return m_pIndiInfoImpl->GetCalConditionList(nSubGraphIndex);
}

CList<double, double>* CIndicatorInfo::GetCalConditionList(const CString& strSubGraphName) const
{
	return m_pIndiInfoImpl->GetCalConditionList(strSubGraphName);
}

CList<double, double>*CIndicatorInfo::GetCalConditionList(const CString& strSubGraphName)
{
	return m_pIndiInfoImpl->GetCalConditionList(strSubGraphName);
}

// ----------------------------------------------------------------------------
void CIndicatorInfo::GetGraphDataForMaking( CString &p_strGraphData) const
{
	m_pIndiInfoImpl->GetGraphDataForMaking( p_strGraphData);
}

void CIndicatorInfo::GetAllSubGraphDataForMaking( CString &p_strAllSubGraphData) const
{
	m_pIndiInfoImpl->GetAllSubGraphDataForMaking( p_strAllSubGraphData);
}

/*
CString CIndicatorInfo::GetAllSubGraphDataForMaking(const CList<CGraphColor*, CGraphColor*>& graphColorList) const
{
	return m_pIndiInfoImpl->GetAllSubGraphDataForMaking(graphColorList);
}
*/

CString CIndicatorInfo::GetStringCommonCalConditionList(const CString& strCompart) const
{
	return m_pIndiInfoImpl->GetStringCommonCalConditionList(strCompart);
}

CString CIndicatorInfo::GetStringDrawConditionList(const CString& strCompart) const
{
	return m_pIndiInfoImpl->GetStringDrawConditionList(strCompart);
}

CString CIndicatorInfo::GetStringBaseLineList(const CString& strCompart) const
{
	return m_pIndiInfoImpl->GetStringBaseLineList(strCompart);
}

CGraphData& CIndicatorInfo::GetGraphData()
{
	return m_pIndiInfoImpl->GetGraphData();
}

CGraphData* CIndicatorInfo::GetGraphData_PointerType()
{
	return m_pIndiInfoImpl->GetGraphData_PointerType();
}

void CIndicatorInfo::GetGraphDataDrawingData(bool& bCalcRiseFallRate_LowHigh, bool& bCalcRiseFallRate_LowCur, 
		bool& bCalcRiseFallRate_HighCur, bool& bCalcRate_PrevCur, int& nCalcRiseFallRate_CurData, BOOL &p_bShowOHLC) const
{
	m_pIndiInfoImpl->GetGraphDataDrawingData(bCalcRiseFallRate_LowHigh, bCalcRiseFallRate_LowCur, 
		bCalcRiseFallRate_HighCur, bCalcRate_PrevCur, nCalcRiseFallRate_CurData, p_bShowOHLC);
}

CString CIndicatorInfo::GetCommonPacketNames() const
{
	return m_pIndiInfoImpl->GetCommonPacketNames();
}

CString CIndicatorInfo::GetPacketNames(const int nSubGraphIndex) const
{
	return m_pIndiInfoImpl->GetPacketNames(nSubGraphIndex);
}

CSignalData CIndicatorInfo::GetSignalData() const
{
	return m_pIndiInfoImpl->GetSignalData();
}

CSignalData& CIndicatorInfo::GetSignalData()
{
	return m_pIndiInfoImpl->GetSignalData();
}

CSignalData* CIndicatorInfo::GetSignalData_PointerType()
{
	return m_pIndiInfoImpl->GetSignalData_PointerType();
}

bool CIndicatorInfo::IsSignalUse() const
{
	return m_pIndiInfoImpl->IsSignalUse();
}

bool CIndicatorInfo::IsSignalShow() const
{
	return m_pIndiInfoImpl->IsSignalShow();
}

int CIndicatorInfo::GetSubGraphCount() const
{
	return m_pIndiInfoImpl->GetSubGraphCount();
}

bool CIndicatorInfo::GetAllSubGraphDataList(CList<CSubGraphData*, CSubGraphData*>& subGraphDataList) const
{
	return m_pIndiInfoImpl->GetAllSubGraphDataList(subGraphDataList);
}

CSubGraphData* CIndicatorInfo::GetSubGraphData(const CString& strSubGraphName) const
{
	return m_pIndiInfoImpl->GetSubGraphData(strSubGraphName);
}

CSubGraphData* CIndicatorInfo::GetSubGraphData(const int nSubGraphIndex) const
{
	return m_pIndiInfoImpl->GetSubGraphData(nSubGraphIndex);
}

int CIndicatorInfo::GetSubGraphIndex(const CString& strSubGraphName) const
{
	return m_pIndiInfoImpl->GetSubGraphIndex(strSubGraphName);
}

CString CIndicatorInfo::GetSubGraphName(const int nIndex) const
{
	return m_pIndiInfoImpl->GetSubGraphName(nIndex);
}

CString CIndicatorInfo::GetSubGraphTitle(const int nIndex) const
{
	return m_pIndiInfoImpl->GetSubGraphTitle(nIndex);
}

bool CIndicatorInfo::GetSubGraphNameList(CList<CString, CString>& SubGraphNameList) const
{
	return m_pIndiInfoImpl->GetSubGraphNameList(SubGraphNameList);
}

// (2006/5/31 - Seung-Won, Bae) To compare with Main Block Graph Objects
BOOL CIndicatorInfo::GetSubGraphNames( CString &p_strSubGraphNames) const
{
	return m_pIndiInfoImpl->GetSubGraphNames( p_strSubGraphNames);
}

CGraphColor CIndicatorInfo::GetSubGraphColor(const int nIndex) const
{
	return m_pIndiInfoImpl->GetSubGraphColor(nIndex);
}

CGraphColor* CIndicatorInfo::GetSubGraphColor_PointerType(const int nIndex) const
{
	return m_pIndiInfoImpl->GetSubGraphColor_PointerType(nIndex);
}

bool CIndicatorInfo::GetSubGraphColorList(CList<CGraphColor*, CGraphColor*>& subGraphColorList) const
{
	return m_pIndiInfoImpl->GetSubGraphColorList(subGraphColorList);
}

// ----------------------------------------------------------------------------
bool CIndicatorInfo::GetCopyDataFrom(const CIndicatorInfo* pIndicatorInfo, BOOL p_bSameGraph)
{
	if(pIndicatorInfo == NULL)
		return false;

	return m_pIndiInfoImpl->GetCopyDataFrom(pIndicatorInfo->m_pIndiInfoImpl, p_bSameGraph);
}

int CIndicatorInfo::SetCommonCalConditionData(const int nIndex, const double& dData)
{
	return m_pIndiInfoImpl->SetCommonCalConditionData(nIndex, dData);
}

int CIndicatorInfo::SetDrawConditionData(const int nIndex, const double& dData)
{
	return m_pIndiInfoImpl->SetDrawConditionData(nIndex, dData);
}

int CIndicatorInfo::SetBaseLineData(const int nIndex, const double& dData)
{
	return m_pIndiInfoImpl->SetBaseLineData(nIndex, dData);
}

bool CIndicatorInfo::SetAllCalConditionData_SubGraphPart(const int nCondIndex, const double& dData)
{
	return m_pIndiInfoImpl->SetAllCalConditionData_SubGraphPart(nCondIndex, dData);
}

int CIndicatorInfo::SetCalConditionData(const int nSubGraphIndex, const int nCondIndex, const double& dData)
{
	return m_pIndiInfoImpl->SetCalConditionData(nSubGraphIndex, nCondIndex, dData);
}

// ----------------------------------------------------------------------------
bool CIndicatorInfo::SetGraphData(const CGraphData& graphData, const bool bIsAutoCheck)
{
	return m_pIndiInfoImpl->SetGraphData(graphData, bIsAutoCheck);
}

bool CIndicatorInfo::SetCommonPacketNames(const CString& strPacketNames)
{
	return m_pIndiInfoImpl->SetCommonPacketNames(strPacketNames);
}

bool CIndicatorInfo::SetAllPacketNames_SubGraphPart(const CString& strPacketNames, const bool bIsAutoCheck)
{
	return m_pIndiInfoImpl->SetAllPacketNames_SubGraphPart(strPacketNames, bIsAutoCheck);
}

bool CIndicatorInfo::SetPacketNames(const int nSubGraphIndex, const CString& strPacketNames)
{
	return m_pIndiInfoImpl->SetPacketNames(nSubGraphIndex, strPacketNames);
}

void CIndicatorInfo::SetSignalData(const CSignalData& signalData)
{
	m_pIndiInfoImpl->SetSignalData(signalData);
}

// ----------------------------------------------------------------------------
bool CIndicatorInfo::SetAllSubGraphDataList(const CList<CSubGraphData*, CSubGraphData*>* pSubGraphDataList, const bool bAutoCalConditionData)
{
	return m_pIndiInfoImpl->SetAllSubGraphDataList((CSubGraphDataList*)(pSubGraphDataList), bAutoCalConditionData);
}

bool CIndicatorInfo::SetSubGraphData(const CSubGraphData* pSourceData)
{
	return m_pIndiInfoImpl->SetSubGraphData(pSourceData);
}

bool CIndicatorInfo::SetSubGraphTitle(const int nIndex, const CString& strTitle)
{
	return m_pIndiInfoImpl->SetSubGraphTitle(nIndex, strTitle);
}

bool CIndicatorInfo::SetSubGraphColor(const int nIndex, const CGraphColor& graphColor)
{
	return m_pIndiInfoImpl->SetSubGraphColor(nIndex, graphColor);
}

bool CIndicatorInfo::SetSubGraphColor(const int nIndex, const COLORREF& lineColor)
{
	return m_pIndiInfoImpl->SetSubGraphColor(nIndex, lineColor);
}

bool CIndicatorInfo::SetSubGraphPenThickness(const int nIndex, const CPenThickness& penThickness)
{
	return m_pIndiInfoImpl->SetSubGraphPenThickness(nIndex, penThickness);
}

bool CIndicatorInfo::SetSubGraphPenThickness(const int nIndex, const int nPenThickness)
{
	return m_pIndiInfoImpl->SetSubGraphPenThickness(nIndex, nPenThickness);
}

// ----------------------------------------------------------------------------
bool CIndicatorInfo::ChangeAllSubGraphTitleFromCalConditionData()
{
	return m_pIndiInfoImpl->ChangeAllSubGraphTitleFromCalConditionData();
}


// (2006/5/28 - Seung-Won, Bae) for Old User Indicator Info. Add New SubGraph.
BOOL CIndicatorInfo::AddSubGraphPartDataWithCopy( const CSubGraphPartData* pSubGraphPartData)
{
	return m_pIndiInfoImpl->AddSubGraphPartDataWithCopy( pSubGraphPartData);
}
CSubGraphPartData *CIndicatorInfo::GetSubGraphPartData( int p_nIndex)
{
	return m_pIndiInfoImpl->GetSubGraphPartData( p_nIndex);
}


// (2006/5/29 - Seung-Won, Bae) Update CGraphDataDrawingData ('graphdatadrawing')
void CIndicatorInfo::SetGraphDataDrawingDataWithCopy( const CGraphDataDrawingData * p_pGraphDataDrawingData)
{
	m_pIndiInfoImpl->SetGraphDataDrawingDataWithCopy( p_pGraphDataDrawingData);
}
CGraphDataDrawingData * CIndicatorInfo::GetGraphDataDrawingData( void)
{
	return m_pIndiInfoImpl->GetGraphDataDrawingData();
}

// (2007/2/28 - Seung-Won, Bae) Retrieve Sub Graph Hiding
BOOL CIndicatorInfo::GetSubGraphHiding( const char *p_szSubGraphName)
{
	return m_pIndiInfoImpl->GetSubGraphHiding( p_szSubGraphName);
}
BOOL CIndicatorInfo::GetSubGraphHiding( const int p_nSubGraphIndex)
{
	return m_pIndiInfoImpl->GetSubGraphHiding( p_nSubGraphIndex);
}

// CGraph : 복수종목 - ojtaso (20070308)
void CIndicatorInfo::SetGraph(CGraph* pGraph)
{
	m_pIndiInfoImpl->SetGraph(pGraph);
}

// CGraph : 복수종목 - ojtaso (20070308)
CGraph*	CIndicatorInfo::GetGraph() const
{
	return m_pIndiInfoImpl->GetGraph();
}

// CGraph : 복수종목 - ojtaso (20070308)
CGraph*	CIndicatorInfo::GetGraph()
{
	return m_pIndiInfoImpl->GetGraph();
}

// 복수종목 불러오기 : 복수종목 - ojtaso (20070514)
void CIndicatorInfo::SetLoadedRQ(LPCTSTR lpszRQ)
{
	m_pIndiInfoImpl->SetLoadedRQ(lpszRQ);
}