// IndicatorInfoImplementation.cpp: implementation of the CIndicatorInfoImplementation class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IndicatorInfoImplementation.h"

#include "FileBuilder.h"
#include "Conversion.h"
#include "Graph.h"		// 복수종목 불러오기 : 복수종목 - ojtaso (20070514)

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////

CIndicatorInfoImplementation::CIndicatorInfoImplementation( const EIndicatorInfoType p_eIndicatorInfoType)
{
	m_eIndicatorInfoType = p_eIndicatorInfoType;

	Initialize();

	// 복수종목 불러오기 : 복수종목 - ojtaso (20070514)
	m_lpszLoadedRQ = NULL;
}

CIndicatorInfoImplementation::CIndicatorInfoImplementation(const CIndicatorInfoImplementation* pDefaultIndiInfoImpl)
{
	CopyAllData( pDefaultIndiInfoImpl);
	
	// 복수종목 불러오기 : 복수종목 - ojtaso (20070514)
	m_lpszLoadedRQ = NULL;
}

CIndicatorInfoImplementation::~CIndicatorInfoImplementation()
{
	// 복수종목 불러오기 : 복수종목 - ojtaso (20070514)
	if(m_lpszLoadedRQ)
		::free(m_lpszLoadedRQ);
}


// 해당 부분만을 셋팅한다.
// -> IndicatorName(IndicatorName), packetName, signal사용유무, SubGraphName, subGraphType은 변경하지 않는다!!
// -> 조건값들의 갯수는 변경되지 않는다.
bool CIndicatorInfoImplementation::CopyDataWithVerificationFrom(const CIndicatorInfoImplementation& sourceData)
{
	if(GetIndicatorName() != sourceData.GetIndicatorName())
		return false;

	m_graphPartData.CopyDataWithVerificationFrom(sourceData.GetGraphPartData());
	SetDoubleListInVerifiable(sourceData.GetCommonCalConditionList(), m_calConditionList);
	SetDoubleListInVerifiable(sourceData.GetDrawConditionList(), m_drawConditionList);

	// (2006/5/28 - Seung-Won, Bae) Do not check count.
	CopyDoubleList(sourceData.GetBaseLineList(), m_baseLineList);
	return true;
}

// ----------------------------------------------------------------------------
void CIndicatorInfoImplementation::Initialize( void)
{
	m_graphPartData.Initialize();
	ClearCommonCalConditionList();
	ClearDrawConditionList();
	ClearBaseLineList();

	// CGraph : 복수종목 - ojtaso (20070308)
	m_pGraph = NULL;
}

bool CIndicatorInfoImplementation::ClearCommonCalConditionList()
{
	if(m_calConditionList.GetCount() <= 0)
		return false;

	m_calConditionList.RemoveAll();
	return true;
}

bool CIndicatorInfoImplementation::ClearDrawConditionList()
{
	if(m_drawConditionList.GetCount() <= 0)
		return false;

	m_drawConditionList.RemoveAll();
	return true;
}

bool CIndicatorInfoImplementation::ClearBaseLineList()
{
	if(m_baseLineList.GetCount() <= 0)
		return false;

	m_baseLineList.RemoveAll();
	return true;
}

// ----------------------------------------------------------------------------
CString CIndicatorInfoImplementation::GetGroupName() const
{
	return m_graphPartData.GetGroupName();
}

CString CIndicatorInfoImplementation::GetIndicatorName() const
{
	return m_graphPartData.GetGraphData().GetIndicatorName();
}

CString CIndicatorInfoImplementation::GetGraphName() const
{
	return m_graphPartData.GetGraphData().GetGraphName();
}

const CGraphPartData& CIndicatorInfoImplementation::GetGraphPartData() const
{
	return const_cast<CGraphPartData&>(m_graphPartData);
}

CGraphPartData* CIndicatorInfoImplementation::GetGraphPartData()
{
	return &m_graphPartData;
}

int CIndicatorInfoImplementation::GetCommonCalConditionCount() const
{
	return m_calConditionList.GetCount();
}

int CIndicatorInfoImplementation::GetDrawConditionCount() const
{
	return m_drawConditionList.GetCount();
}

int CIndicatorInfoImplementation::GetBaseLineCount() const
{
	return m_baseLineList.GetCount();
}

int CIndicatorInfoImplementation::GetCalConditionCount(const int nSubGraphIndex) const
{
	CDoubleList* pDoubleList = GetCalConditionList(nSubGraphIndex);
	if(pDoubleList == NULL)
		return 0;

	return pDoubleList->GetCount();
}

int CIndicatorInfoImplementation::GetCalConditionCount(const CString& strSubGraphName) const
{
	CDoubleList* pDoubleList = GetCalConditionList(strSubGraphName);
	if(pDoubleList == NULL)
		return 0;

	return pDoubleList->GetCount();
}

const CDoubleList& CIndicatorInfoImplementation::GetCommonCalConditionList() const
{
	return const_cast<CDoubleList&>(m_calConditionList);
}

CDoubleList* CIndicatorInfoImplementation::GetCommonCalConditionList()
{
	return &m_calConditionList;
}

const CDoubleList& CIndicatorInfoImplementation::GetDrawConditionList() const
{
	return const_cast<CDoubleList&>(m_drawConditionList);
}

CDoubleList* CIndicatorInfoImplementation::GetDrawConditionList()
{
	return &m_drawConditionList;
}

const CDoubleList& CIndicatorInfoImplementation::GetBaseLineList() const
{
	return const_cast<CDoubleList&>(m_baseLineList);
}

CDoubleList* CIndicatorInfoImplementation::GetBaseLineList()
{
	return &m_baseLineList;
}

CDoubleList* CIndicatorInfoImplementation::GetCalConditionList(const CString& strSubGraphName) const
{
	return m_graphPartData.GetConditionList(strSubGraphName);
}

CDoubleList* CIndicatorInfoImplementation::GetCalConditionList(const CString& strSubGraphName)
{
	return m_graphPartData.GetConditionList(strSubGraphName);
}

CDoubleList* CIndicatorInfoImplementation::GetCalConditionList(const int nSubGraphIndex) const
{
	return m_graphPartData.GetConditionList(nSubGraphIndex);
}

CDoubleList* CIndicatorInfoImplementation::GetCalConditionList(const int nSubGraphIndex)
{
	return m_graphPartData.GetConditionList(nSubGraphIndex);
}

// ----------------------------------------------------------------------------
// graph를 만들기 위한 data.
// Graph;\r\n
// index;가격차트;시가,고가,저자,종가;signal사용여부:signal보임;
//		최저가최고가등락률;최저가현재가등락률;최고가현재가등락률;전일가현재가등락률;전일비;\r\n
// subGraphtype;subGraphStyle;subGraphDrawStyle;SubGraphName;title;숨김;색1;색2;색3;색4;색5;색6;
//		상승채움;하락채움;선굵기1;선굵기2;선굵기3;강조;\r\n
// .... subGraph 갯수만큼..
// GraphEnd;\r\n
void CIndicatorInfoImplementation::GetGraphDataForMaking( CString &p_strGraphData) const
{
	switch( GetConditionTitle())
	{
		case CIndiBaseData::COND_TITLE:			m_graphPartData.GetGraphDataForMaking( p_strGraphData, 0, false);
												break;
		case CIndiBaseData::BASE_COND_TITLE:	m_graphPartData.GetGraphDataForMaking( p_strGraphData, 0, true);
												break;
		default:								m_graphPartData.GetGraphDataForMaking( p_strGraphData);	// title가 조건값이 아닌 경우.
												break;
	}
}

// subGraphtype;subGraphStyle;subGraphDrawStyle;SubGraphName;title;숨김;색1;색2;색3;색4;색5;색6;
//		상승채움;하락채움;선굵기1;선굵기2;선굵기3;강조;\r\n
// .... subGraph 갯수만큼..
void CIndicatorInfoImplementation::GetAllSubGraphDataForMaking( CString &p_strGraphData) const
{
	switch( GetConditionTitle())
	{
		case CIndiBaseData::COND_TITLE:			m_graphPartData.GetAllSubGraphDataForMaking( p_strGraphData, 0, false);
												break;
		case CIndiBaseData::BASE_COND_TITLE:	m_graphPartData.GetAllSubGraphDataForMaking( p_strGraphData, 0, true);
												break;
		default:								m_graphPartData.GetAllSubGraphDataForMaking( p_strGraphData);	// title가 조건값이 아닌 경우.
												break;
	}
}

/*
CString CIndicatorInfoImplementation::GetAllSubGraphDataForMaking(const CGraphColorPointerList& graphColorList) const
{
	CIndiBaseData::TITLETYPE eTitleType = GetConditionTitle();
	if(eTitleType == CIndiBaseData::COND_TITLE)
		return m_graphPartData.GetAllSubGraphDataForMaking(graphColorList, 0, false);
	else if(eTitleType == CIndiBaseData::BASE_COND_TITLE)
		return m_graphPartData.GetAllSubGraphDataForMaking(graphColorList, 0, true);

	// title가 조건값이 아닌 경우.
	return m_graphPartData.GetAllSubGraphDataForMaking(graphColorList);
}
*/

// 계산하기 필요한 조건값들을 string 형태로 가지오기.
CString CIndicatorInfoImplementation::GetStringCommonCalConditionList(const CString& strCompart) const
{
	return GetStringDoubleList(m_calConditionList, strCompart);
}

CString CIndicatorInfoImplementation::GetStringDrawConditionList(const CString& strCompart) const
{
	return GetStringDoubleList(m_drawConditionList, strCompart);
}

CString CIndicatorInfoImplementation::GetStringBaseLineList(const CString& strCompart) const
{
	return GetStringDoubleList(m_baseLineList, strCompart);
}

CGraphData& CIndicatorInfoImplementation::GetGraphData()
{
	return m_graphPartData.GetGraphData();
}

CGraphData* CIndicatorInfoImplementation::GetGraphData_PointerType()
{
	return &m_graphPartData.GetGraphData();
}

void CIndicatorInfoImplementation::GetGraphDataDrawingData(bool& bCalcRiseFallRate_LowHigh, bool& bCalcRiseFallRate_LowCur, 
		bool& bCalcRiseFallRate_HighCur, bool& bCalcRate_PrevCur, int& nCalcRiseFallRate_CurData, BOOL &p_bShowOHLC) const
{
	CGraphDataDrawingData graphDrawingData = m_graphPartData.GetGraphData().GetGraphDataDrawingData();
	
	bCalcRiseFallRate_LowHigh = graphDrawingData.DoesCalcRiseFallRate_LowHigh();
	bCalcRiseFallRate_LowCur = graphDrawingData.DoesCalcRiseFallRate_LowCur();
	bCalcRiseFallRate_HighCur = graphDrawingData.DoesCalcRiseFallRate_HighCur();
	bCalcRate_PrevCur = graphDrawingData.DoesCalcRate_PrevCur();
	nCalcRiseFallRate_CurData = graphDrawingData.GetCalcRiseFallRate_CurData();
	p_bShowOHLC = graphDrawingData.GetShowOHLC();
}

// signal data
CSignalData CIndicatorInfoImplementation::GetSignalData() const
{
	return m_graphPartData.GetGraphData().GetSignalData();
}

CSignalData& CIndicatorInfoImplementation::GetSignalData()
{
	return m_graphPartData.GetGraphData().GetSignalData();
}

CSignalData* CIndicatorInfoImplementation::GetSignalData_PointerType()
{
	return &m_graphPartData.GetGraphData().GetSignalData();
}

bool CIndicatorInfoImplementation::IsSignalUse() const
{
	return m_graphPartData.GetGraphData().GetSignalData().IsUse();
}

bool CIndicatorInfoImplementation::IsSignalShow() const
{
	return m_graphPartData.GetGraphData().GetSignalData().IsShow();
}

// packet name
CString CIndicatorInfoImplementation::GetCommonPacketNames() const
{
	return m_graphPartData.GetGraphData().GetPacketNames();
}

CString CIndicatorInfoImplementation::GetPacketNames(const int nSubGraphIndex) const
{
	return m_graphPartData.GetPacketNames(nSubGraphIndex);
}

// subGraph count.
int CIndicatorInfoImplementation::GetSubGraphCount() const
{
	return m_graphPartData.GetSubGraphPartDataCount();
}

// 모든 subGraphData
bool CIndicatorInfoImplementation::GetAllSubGraphDataList(CSubGraphDataList& subGraphDataList) const
{
	return m_graphPartData.GetAllSubGraphDataList(subGraphDataList);
}

// 해당 subGraphData
CSubGraphData* CIndicatorInfoImplementation::GetSubGraphData(const CString& strSubGraphName) const
{
	return m_graphPartData.GetSubGraphData(strSubGraphName);
}

CSubGraphData* CIndicatorInfoImplementation::GetSubGraphData(const int nSubGraphIndex) const
{
	return m_graphPartData.GetSubGraphData(nSubGraphIndex);
}

// 해당 SubGraphName의 index.
int CIndicatorInfoImplementation::GetSubGraphIndex(const CString& strSubGraphName) const
{
	return m_graphPartData.GetSubGraphPartDataIndex(strSubGraphName);
}

// 해당 index의 SubGraphName.
CString CIndicatorInfoImplementation::GetSubGraphName(const int nIndex) const
{
	CSubGraphData* pSubGraphData = m_graphPartData.GetSubGraphData(nIndex);
	if(pSubGraphData == NULL)
		return "";

	return pSubGraphData->GetName();
}

CString CIndicatorInfoImplementation::GetSubGraphTitle(const int nIndex) const
{
	return m_graphPartData.GetSubGraphTitle(nIndex);
}

// 모든 subGraph의 name를 가져오기.
bool CIndicatorInfoImplementation::GetSubGraphNameList(CStringDataList& SubGraphNameList) const
{
	return m_graphPartData.GetSubGraphNameList(SubGraphNameList);
}

// (2006/5/31 - Seung-Won, Bae) To compare with Main Block Graph Objects
BOOL CIndicatorInfoImplementation::GetSubGraphNames( CString &p_strSubGraphNames) const
{
	return m_graphPartData.GetSubGraphNames( p_strSubGraphNames);
}

CGraphColor CIndicatorInfoImplementation::GetSubGraphColor(const int nIndex) const
{
	CSubGraphData* pSubGraphData = m_graphPartData.GetSubGraphData(nIndex);
	if(pSubGraphData == NULL)
		return CGraphColor();

	return pSubGraphData->GetColor();
}

CGraphColor* CIndicatorInfoImplementation::GetSubGraphColor_PointerType(const int nIndex) const
{
	CSubGraphData* pSubGraphData = m_graphPartData.GetSubGraphData(nIndex);
	if(pSubGraphData == NULL)
		return NULL;

	return pSubGraphData->GetColor_PointerType();
}

bool CIndicatorInfoImplementation::GetSubGraphColorList(CGraphColorPointerList& subGraphColorList) const
{
	return m_graphPartData.GetSubGraphColorList(subGraphColorList);
}

// ----------------------------------------------------------------------------
bool CIndicatorInfoImplementation::GetCopyDataFrom(const CIndicatorInfoImplementation* pIndiInfoImpl, BOOL p_bSameGraph)
{
	if( pIndiInfoImpl == NULL) return false;
	if( pIndiInfoImpl == this) return true;

	if(GetIndicatorName() != pIndiInfoImpl->GetIndicatorName())
		return false;

	CopyAllData(pIndiInfoImpl, p_bSameGraph);
	return true;
}

int CIndicatorInfoImplementation::SetCommonCalConditionData(const int nIndex, const double& dData)
{
	return SetDoubleListData(nIndex, dData, m_calConditionList);
}

int CIndicatorInfoImplementation::SetDrawConditionData(const int nIndex, const double& dData)
{
	return SetDoubleListData(nIndex, dData, m_drawConditionList);
}

int CIndicatorInfoImplementation::SetBaseLineData(const int nIndex, const double& dData)
{
	return SetDoubleListData(nIndex, dData, m_baseLineList);
}

bool CIndicatorInfoImplementation::SetAllCalConditionData_SubGraphPart(const int nCondIndex, const double& dData)
{
	return m_graphPartData.SetAllCalConditionData(nCondIndex, dData);
}

int CIndicatorInfoImplementation::SetCalConditionData(const int nSubGraphIndex, const int nCondIndex, const double& dData)
{
	CSubGraphPartData* pSubGraphPartData = m_graphPartData.GetSubGraphPartData(nSubGraphIndex);
	if(pSubGraphPartData == NULL)
		return -1;

	return SetDoubleListData(nCondIndex, dData, pSubGraphPartData->GetConditionList());
}

// ----------------------------------------------------------------------------
bool CIndicatorInfoImplementation::SetGraphData(const CGraphData& graphData, const bool bIsAutoCheck)
{
	return m_graphPartData.SetGraphData(graphData, bIsAutoCheck);
}

bool CIndicatorInfoImplementation::SetCommonPacketNames(const CString& strPacketNames)
{
	if(m_graphPartData.GetGraphData().GetPacketNames() == strPacketNames)
		return false;

	m_graphPartData.GetGraphData().SetPacketNames(strPacketNames);
	return true;
}

bool CIndicatorInfoImplementation::SetAllPacketNames_SubGraphPart(const CString& strPacketNames, const bool bIsAutoCheck)
{
	return m_graphPartData.SetAllPacketNames_SubGraphPart(strPacketNames, bIsAutoCheck);
}

bool CIndicatorInfoImplementation::SetPacketNames(const int nSubGraphIndex, const CString& strPacketNames)
{
	return m_graphPartData.SetPacketNames(nSubGraphIndex, strPacketNames);
}

void CIndicatorInfoImplementation::SetSignalData(const CSignalData& signalData)
{
	m_graphPartData.GetGraphData().SetSignalData(signalData);
}

// ----------------------------------------------------------------------------
bool CIndicatorInfoImplementation::SetAllSubGraphDataList(const CSubGraphDataList* pSubGraphDataList, const bool bAutoCalConditionData)
{
	bool bResult = m_graphPartData.SetAllSubGraphDataList(pSubGraphDataList);

	// <가격 이동평균>, <거래량 이동평균> 과 같은 지표는 title이 계산 조건값이기 때문에
	// 계산 조건값을 title 값으로 변경해줘야 한다.
	// -> subGraphData를 먼저 바뀐뒤 조건값을 변경해야 한다!
	if(bAutoCalConditionData)
	{
		ChangeCalConditionDataFromSubGraphTitle();
		m_graphPartData.ChangeCalConditionDataFromSubGraphTitle();
	}
	
	return bResult;
}

bool CIndicatorInfoImplementation::SetSubGraphData(const CSubGraphData* pSourceData)
{
	return m_graphPartData.SetSubGraphData(pSourceData);
}

bool CIndicatorInfoImplementation::SetSubGraphTitle(const int nIndex, const CString& strTitle)
{
	return m_graphPartData.SetSubGraphTitle(nIndex, strTitle);
}

bool CIndicatorInfoImplementation::SetSubGraphColor(const int nIndex, const CGraphColor& graphColor)
{
	return m_graphPartData.SetSubGraphColor(nIndex, graphColor);
}

bool CIndicatorInfoImplementation::SetSubGraphColor(const int nIndex, const COLORREF& lineColor)
{
	return m_graphPartData.SetSubGraphColor(nIndex, CGraphColor(lineColor, lineColor, lineColor, lineColor, lineColor, lineColor));
}

bool CIndicatorInfoImplementation::SetSubGraphPenThickness(const int nIndex, const CPenThickness& penThickness)
{
	return m_graphPartData.SetSubGraphPenThickness(nIndex, penThickness);
}

bool CIndicatorInfoImplementation::SetSubGraphPenThickness(const int nIndex, const int nPenThickness)
{
	// 기준선 두께, 모양 설정 - ojtaso (20071213)
	return m_graphPartData.SetSubGraphPenThickness(nIndex, CPenThickness(nPenThickness, nPenThickness, nPenThickness, nPenThickness));
}

// ----------------------------------------------------------------------------
bool CIndicatorInfoImplementation::ChangeAllSubGraphTitleFromCalConditionData()
{
	// 조건값을 title로 변경
	if(GetConditionTitle() == CIndiBaseData::BASE_TITLE)
		return false;

	bool bResult = false;
	int nCount = GetSubGraphCount();
	for(int nIndex = 0; nIndex < nCount; nIndex++)
	{
		CString strTitle = GetSubGraphTitle(nIndex);
		if(strTitle.IsEmpty())
			continue;

		if(SetSubGraphTitle(nIndex, strTitle))
			bResult = true;
	}

	return bResult;
}

// public =====================================================================
// file data를 셋팅한다.
 
// private ====================================================================
// file data를 셋팅한다.

// ----------------------------------------------------------------------------
void CIndicatorInfoImplementation::CopyAllData(const CIndicatorInfoImplementation* pSourceData, BOOL p_bSameGraph)
{
	CGraph *pGraph = NULL;
	CString strGraphName;
	CString strItemName;
	if( !p_bSameGraph)
	{
		pGraph = m_pGraph;
		strGraphName = m_graphPartData.GetGraphData().GetGraphName();
		if( 0 < m_graphPartData.GetSubGraphPartDataCount())
			strItemName = m_graphPartData.GetSubGraphData( 0)->GetItemName();
	}

	Initialize();
	if(pSourceData == NULL) return;
	m_eIndicatorInfoType	= pSourceData->m_eIndicatorInfoType;
	m_graphPartData			= pSourceData->GetGraphPartData();
 	CopyDoubleList(pSourceData->GetCommonCalConditionList(), m_calConditionList);
	CopyDoubleList(pSourceData->GetDrawConditionList(), m_drawConditionList);
	CopyDoubleList(pSourceData->GetBaseLineList(), m_baseLineList);
	
	// CGraph : 복수종목 - ojtaso (20070308)
	m_pGraph = const_cast<CIndicatorInfoImplementation*>(pSourceData)->GetGraph();

	if( !p_bSameGraph)
	{
		m_pGraph = pGraph;
		( ( CGraphDataBase *)( &m_graphPartData.GetGraphData()))->SetGraphName( strGraphName);
		if( !strItemName.IsEmpty())
			m_graphPartData.GetSubGraphData( 0)->SetItemName( strItemName);
	}
}

void CIndicatorInfoImplementation::CopyDoubleList(const CDoubleList& sourceData, CDoubleList& targetData)
{
	targetData.RemoveAll();

	POSITION pos = sourceData.GetHeadPosition();
	while(pos != NULL)
	{
		targetData.AddTail(sourceData.GetNext(pos));
	}
}

// default 값과 user 값과 비교하여 data를 검증후 user 내용으로 수정한다.
void CIndicatorInfoImplementation::SetDoubleListInVerifiable(const CDoubleList& sourceData, CDoubleList& targetData)
{
	// count 가 같지 않을 경우는 어떤 조건값이 추가됬을지 모르기 때문에 default 값 사용한다.
	if(sourceData.GetCount() != targetData.GetCount())
		return;

	// count 가 같을 경우에는 default 와 같은 형태로 본다.
	CopyDoubleList(sourceData, targetData);
}

// ----------------------------------------------------------------------------
CString CIndicatorInfoImplementation::GetStringDoubleList(const CDoubleList& doubleList, const CString& strCompart) const
{
	if(doubleList.GetCount() <= 0)
		return "";

	CString strData;
	POSITION pos = doubleList.GetHeadPosition();
	while(pos != NULL)
	{
		double dData = doubleList.GetNext(pos);
		strData += (CDataConversion::DoubleToString(dData) + strCompart);
	}
	return strData;
}

// ----------------------------------------------------------------------------
// 조건값이 title 인지의 여부.
// -> [가격 이동평균], [거래량 이동평균] : 조건값
// -> [이격도] : title + 조건값
CIndiBaseData::TITLETYPE CIndicatorInfoImplementation::GetConditionTitle() const
{
	// "가격 이동평균", "거래량 이동평균" 은 subCalCondition 이다.
	// -> 조건값이 존재하는지 확인
	if(GetCommonCalConditionCount() <= 0)
	{
		if(GetCalConditionCount(0) <= 0)
			return CIndiBaseData::BASE_TITLE;
	}

	return m_graphPartData.GetConditionTitle();
}

// ----------------------------------------------------------------------------

// nIndex는 0부터..
// return : -1(해당 nIndex 없음) 0(해당 nIndex있으나 값이 같음) 1(변경함)
int CIndicatorInfoImplementation::SetDoubleListData(const int nIndex, const double& dData, CDoubleList& targetList)
{
	POSITION pos = targetList.FindIndex(nIndex);
	if(pos == NULL)
		return -1;

	double dOrgData = targetList.GetAt(pos);
	if(dData == dOrgData)
		return 0;

	targetList.SetAt(pos, dData);
	return 1;
}

// ----------------------------------------------------------------------------
// subGraph Title을 계산 조건값으로 변경해준다.
// -> <가격 이동평균>, <거래량 이동평균>
bool CIndicatorInfoImplementation::ChangeCalConditionDataFromSubGraphTitle()
{
	// title가 조건값이 아닌 경우.
	if(GetConditionTitle() == CIndiBaseData::BASE_TITLE)
		return false;

	CStringDataList subGraphTitleList;
	m_graphPartData.GetSubGraphTitleList(subGraphTitleList);

	bool bResult = false;
	int nIndex = 0;
	POSITION pos = subGraphTitleList.GetHeadPosition();
	while(pos != NULL)
	{
		CString strTitle = subGraphTitleList.GetNext(pos);
		double dTitle = 0.0;
		if(!GetCalConditionDataFromSubGraphTitle(strTitle, dTitle))
			continue;
		if(SetCommonCalConditionData(nIndex, dTitle) == 1)
			bResult = true;
		if(SetCalConditionData(nIndex, 0, dTitle) == 1)
			bResult = true;
		nIndex++;
	}

	return bResult;
}

bool CIndicatorInfoImplementation::GetCalConditionDataFromSubGraphTitle(const CString& strOrgTitle, double& dCalData)
{
	// (2007/2/20 - Seung-Won, Bae) Use Title Header and Footer
	const char *szTitle = strOrgTitle;
	int nStartIndex = 0;
	while( szTitle[ nStartIndex] && ( '9' < szTitle[ nStartIndex] || szTitle[ nStartIndex] < '0')) nStartIndex++;
	int nEndIndex = nStartIndex;
	while( szTitle[ nEndIndex] && '0' <= szTitle[ nEndIndex] && szTitle[ nEndIndex] <= '9') nEndIndex++;
	if( nStartIndex == nEndIndex) return false;
	dCalData = atof( strOrgTitle.Mid( nStartIndex, nEndIndex - nStartIndex));
	return true;
}


// (2006/5/28 - Seung-Won, Bae) for Old User Indicator Info. Add New SubGraph.
BOOL CIndicatorInfoImplementation::AddSubGraphPartDataWithCopy( const CSubGraphPartData* pSubGraphPartData)
{
	return m_graphPartData.AddSubGraphPartDataWithCopy( pSubGraphPartData);
}
CSubGraphPartData *CIndicatorInfoImplementation::GetSubGraphPartData( int p_nIndex)
{
	return m_graphPartData.GetSubGraphPartData( p_nIndex);
}


// (2006/5/29 - Seung-Won, Bae) Update CGraphDataDrawingData ('graphdatadrawing')
void CIndicatorInfoImplementation::SetGraphDataDrawingDataWithCopy( const CGraphDataDrawingData * p_pGraphDataDrawingData)
{
	m_graphPartData.SetGraphDataDrawingDataWithCopy( p_pGraphDataDrawingData);
}
CGraphDataDrawingData * CIndicatorInfoImplementation::GetGraphDataDrawingData( void)
{
	return m_graphPartData.GetGraphDataDrawingData();
}


// (2007/2/28 - Seung-Won, Bae) Retrieve Sub Graph Hiding
BOOL CIndicatorInfoImplementation::GetSubGraphHiding( const char *p_szSubGraphName)
{
	return m_graphPartData.GetSubGraphHiding( p_szSubGraphName);
}
BOOL CIndicatorInfoImplementation::GetSubGraphHiding( const int p_nSubGraphIndex)
{
	return m_graphPartData.GetSubGraphHiding( p_nSubGraphIndex);
}

// CGraph : 복수종목 - ojtaso (20070308)
void CIndicatorInfoImplementation::SetGraph(CGraph* pGraph)
{
	m_pGraph = pGraph;

	// 복수종목 불러오기 : 복수종목 - ojtaso (20070514)
	if(m_lpszLoadedRQ)
	{
		m_pGraph->SetGraphRQ(m_lpszLoadedRQ);
		
		::free(m_lpszLoadedRQ);
		m_lpszLoadedRQ = NULL;
	}
}

// CGraph : 복수종목 - ojtaso (20070308)
CGraph*	CIndicatorInfoImplementation::GetGraph()
{
	return m_pGraph;
}

// 복수종목 불러오기 : 복수종목 - ojtaso (20070514)
void CIndicatorInfoImplementation::SetLoadedRQ(LPCTSTR lpszRQ)
{
	if(m_lpszLoadedRQ)
		::free(m_lpszLoadedRQ);

	m_lpszLoadedRQ = ::strdup(lpszRQ);
}