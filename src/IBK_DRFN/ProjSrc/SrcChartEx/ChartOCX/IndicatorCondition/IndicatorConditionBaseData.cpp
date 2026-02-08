// IndicatorConditionBaseData.cpp: implementation of the CIndicatorConditionBaseData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IndicatorConditionBaseData.h"

#include <assert.h>
#include <algorithm>

#include "../Include_Chart/Dll_Load/IndicatorInfo.h"
#include "../Include_Chart/Dll_Load/IndicatorGroupData.h"
#include "../Include_Chart/Dll_Load/IMetaTable.h"			// for g_iMetaTable
#include "FileBuilder.h"
#include "Conversion.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////
// class CSubGraphPartData -> CIndicatorInfo 에 저장되는 data 중 subGraph 관련 부분

CSubGraphPartData& CSubGraphPartData::operator=(const CSubGraphPartData& data)
{
	if(this == &data)
		return *this;

	m_strCalPacketNames = data.GetCalPacketNames();
	m_subGraphData = data.GetSubGraphData();
	CopyConditionList(data.GetConditionList());
	return *this;
}

CSubGraphPartData::CSubGraphPartData()
{
}

CSubGraphPartData::CSubGraphPartData(const CSubGraphPartData& data):
	m_strCalPacketNames(data.GetCalPacketNames()), 
	m_subGraphData(data.GetSubGraphData())
{
	CopyConditionList(data.GetConditionList());
}

CSubGraphPartData::CSubGraphPartData(const CSubGraphData& subGraphData):
	m_subGraphData(subGraphData)
{
}

CSubGraphPartData::~CSubGraphPartData()
{
	m_calConditionList.RemoveAll();
}


// public =====================================================================
const CDoubleList& CSubGraphPartData::GetConditionList() const
{
	return m_calConditionList;
}

CDoubleList& CSubGraphPartData::GetConditionList()
{
	return m_calConditionList;
}

const CSubGraphData& CSubGraphPartData::GetSubGraphData() const
{
	return m_subGraphData;
}

CSubGraphData& CSubGraphPartData::GetSubGraphData()
{
	return m_subGraphData;
}

CSubGraphData* CSubGraphPartData::GetSubGraphData_PointeType()
{
	return &m_subGraphData;
}

// ----------------------------------------------------------------------------
bool CSubGraphPartData::GetConditionData(const int nIndex, double& dData) const
{
	if(nIndex < 0)
		return false;

	POSITION pos = m_calConditionList.FindIndex(nIndex);
	if(pos == NULL)
		return false;

	dData = m_calConditionList.GetAt(pos);
	return true;
}

bool CSubGraphPartData::GetConditionData(const int nIndex, CString& strData) const
{
	double dData = 0.0;
	if(!GetConditionData(nIndex, dData))
		return false;

	strData = CDataConversion::DoubleToString(dData);
	return true;
}

CString CSubGraphPartData::GetCalPacketNames() const
{
	return m_strCalPacketNames;
}

CString CSubGraphPartData::GetSubGraphName() const
{
	return m_subGraphData.GetName();
}

void CSubGraphPartData::GetSubGraphStringData( CString &p_strSubGraphData) const
{
	m_subGraphData.GetStringData( p_strSubGraphData);
}

void CSubGraphPartData::GetSubGraphStringData( CString &p_strSubGraphData, const int nCondIndexForTitle, const bool bIncludeOrgTitle)
{
	GetSubGraphStringData( p_strSubGraphData, m_subGraphData, nCondIndexForTitle, bIncludeOrgTitle);
}

/*
void CSubGraphPartData::GetSubGraphStringData( const char *p_szTitle, CString &p_strSubGraphData) const
{
	CSubGraphData tempSubGraphData = m_subGraphData;
	tempSubGraphData.SetTitle( p_szTitle);
	tempSubGraphData.GetStringData( p_strSubGraphData);
}
*/

/*
void CSubGraphPartData::GetSubGraphStringData(const CGraphColor& graphColor, CString &p_strSubGraphData) const
{
	CSubGraphData tempSubGraphData = m_subGraphData;
	tempSubGraphData.SetColor(graphColor);
	tempSubGraphData.GetStringData( p_strSubGraphData);
}
*/

/*
CString CSubGraphPartData::GetSubGraphStringData(const int nCondIndexForTitle, const bool bIncludeOrgTitle, const CGraphColor& graphColor) const
{
	CSubGraphData tempSubGraphData = m_subGraphData;
	tempSubGraphData.SetColor(graphColor);

	return GetSubGraphStringData(tempSubGraphData, nCondIndexForTitle, bIncludeOrgTitle);
}
*/

// ----------------------------------------------------------------------------
// default 와 비교하여 user data를 검증 후 적용한다.
// -> SubGraphName, subGraphType 변경 안됨!!
bool CSubGraphPartData::CopyDataWithVerificationFrom(const CSubGraphPartData* pSourceData)
{
	if(pSourceData == NULL)
		return false;

	//subGraph가 변경된 뒤에 doubleList도 변경한다.
	if(!CopyDataWithVerificationFrom(pSourceData->GetSubGraphData()))
		return false;

	if(!pSourceData->GetCalPacketNames().IsEmpty())
		SetCalPacketNames(pSourceData->GetCalPacketNames());

	CopyDataWithVerificationFrom(pSourceData->GetConditionList());
	return true;
}

// -> SubGraphName, subGraphType 변경 안됨!!
bool CSubGraphPartData::CopyDataWithVerificationFrom(const CSubGraphData& sourceData)
{
	if(m_subGraphData.GetName() != sourceData.GetName())
		return false;

	// type, style, drawStyle
	if(m_subGraphData.GetType() == sourceData.GetType())
	{
		m_subGraphData.SetStyle(sourceData.GetStyle());
		m_subGraphData.SetDrawStyle(sourceData.GetDrawStyle());
		// (2008/9/14 - Seung-Won, Bae) for Pair Tick
		m_subGraphData.SetItemName( sourceData.GetItemName());
	}
	// title
	m_subGraphData.SetTitle(sourceData.GetTitle());
	// hiding
	m_subGraphData.SetHiding(sourceData.GetHiding());
	// graphColor
	m_subGraphData.SetColor(sourceData.GetColor());
	// penThickness
	m_subGraphData.SetPenThickness(sourceData.GetPenThickness());
	return true;
}

bool CSubGraphPartData::SetCalPacketNames( const char *p_szCalPacketNames)
{
	if(m_strCalPacketNames == p_szCalPacketNames) return false;

	m_strCalPacketNames = p_szCalPacketNames;
	return true;
}

bool CSubGraphPartData::SetCalPacketNames( const char *p_szCalPacketNames, const bool bIsAutoCheck)
{
	if( bIsAutoCheck && m_strCalPacketNames.IsEmpty()) return false;
	return SetCalPacketNames( p_szCalPacketNames);
}

void CSubGraphPartData::SetConditionList( const char *p_szData, const CString& strCompart)
{
	m_calConditionList.RemoveAll();
	if( !p_szData) return;
	if( !*p_szData) return;

	// data가 하나일 경우
	if(strCompart.IsEmpty())
	{
		double dData = atof( p_szData);
		m_calConditionList.AddTail(dData);
		return;
	}

	// data가 두개 이상일 경우
	CString strTemp = p_szData;
	if(strTemp.Right(strCompart.GetLength()) != strCompart)
		strTemp += strCompart;

	while(!strTemp.IsEmpty())
	{
		CString strRealData = CDataConversion::GetStringData(strTemp, strCompart);
		m_calConditionList.AddTail(atof(strRealData));
	}
}

bool CSubGraphPartData::SetCalConditionData(const int nCondIndex, const double& dData)
{
	POSITION pos = m_calConditionList.FindIndex(nCondIndex);
	if(pos == NULL)
		return false;

	m_calConditionList.SetAt(pos, dData);
	return true;
}

// ----------------------------------------------------------------------------
// subGraph 에 있는 계산 조건값을 title로 바꾼다.
bool CSubGraphPartData::ChangeCalConditionDataFromSubGraphTitle()
{
	if(m_calConditionList.GetCount() <= 0)
		return false;

	CString strTitle = m_subGraphData.GetTitle();
	strTitle.TrimLeft(" ");
	strTitle.TrimRight(" ");
	if(strTitle.IsEmpty())
		return false;

	double dData = atof(strTitle);
	if(dData == 0.0 && strTitle != "0")
		return false;

	return SetCalConditionData(0, dData);
}


// private ====================================================================
void CSubGraphPartData::CopyConditionList(const CDoubleList& sourceData)
{
	m_calConditionList.RemoveAll();

	POSITION pos = sourceData.GetHeadPosition();
	while(pos != NULL)
	{
		m_calConditionList.AddTail(sourceData.GetNext(pos));
	}
}

// -> 기본의 count를 변경하진 않는다.
void CSubGraphPartData::CopyDataWithVerificationFrom(const CDoubleList& sourceData)
{
	// count 가 같지 않을 경우는 어떤 조건값이 추가됬을지 모르기 때문에 default 값 사용한다.
	if(m_calConditionList.GetCount() != sourceData.GetCount())
		return;

	// count 가 같을 경우에는 default 와 같은 형태로 본다.
	CopyConditionList(sourceData);
}

/*void CSubGraphPartData::CopyDataWithVerificationFrom(const CDoubleList& sourceData)
{
	// count 가 같을 경우에는 default 와 같은 형태로 본다.
	if(m_calConditionList.GetCount() == sourceData.GetCount())
	{
		CopyConditionList(sourceData);
		return;
	}

	POSITION pos_source = sourceData.GetHeadPosition();
	POSITION pos_target = m_calConditionList.GetHeadPosition();
	while(pos_source != NULL && pos_target != NULL)
	{
		m_calConditionList.SetAt(pos_target, sourceData.GetNext(pos_source));
		m_calConditionList.GetNext(pos_target);
	}
}*/

void CSubGraphPartData::GetSubGraphStringData( CString &p_SubGraphData, CSubGraphData &subGraphData, const int nCondIndexForTitle, const bool bIncludeOrgTitle) const
{
	double dCondition = 0.0;
	if( !GetConditionData( nCondIndexForTitle, dCondition))
	{
		subGraphData.GetStringData( p_SubGraphData);
		return;
	}

	CString strTitle;
	if( bIncludeOrgTitle)	strTitle.Format( "%s_%0.f", subGraphData.GetTitle(), dCondition);
	else					strTitle.Format( "%0.f",	dCondition);

	CString OldTitle = subGraphData.GetTitle();
	subGraphData.SetTitle( strTitle);
	subGraphData.GetStringData( p_SubGraphData);
	subGraphData.SetTitle( OldTitle);
}


///////////////////////////////////////////////////////////////////////////////
// class CGraphPartData -> CIndicatorInfo 에 저장되는 data 중 graph 관련 부분

CGraphPartData& CGraphPartData::operator=(const CGraphPartData& data)
{
	if(this == &data)
		return *this;

	m_strGroupName = data.GetGroupName();
	m_graphData = data.GetGraphData();
	CopySubGraphPartDataList(const_cast<CSubGraphPartDataList*>(data.GetSubGraphPartDataList()));
	return *this;
}

CGraphPartData::CGraphPartData()
{
}

CGraphPartData::CGraphPartData(const CGraphPartData& graphPartData):
	m_strGroupName(graphPartData.GetGroupName()),
	m_graphData(graphPartData.GetGraphData())
{
	CopySubGraphPartDataList(const_cast<CSubGraphPartDataList*>(graphPartData.GetSubGraphPartDataList()));
}

CGraphPartData::~CGraphPartData()
{
	DeleteAllSubGraph();
}


// public =====================================================================
void CGraphPartData::Initialize()
{
	m_strGroupName.Empty();
	Initialize_GraphData();
	DeleteAllSubGraph();
}

void CGraphPartData::Initialize_GraphData()
{
	m_graphData.ClearData();
}

void CGraphPartData::AddTail(CSubGraphPartData* pSubGraphPartData)
{
	if(pSubGraphPartData == NULL)
		return;

	m_subGraphPartDataList.push_back(pSubGraphPartData);
}

bool CGraphPartData::DeleteAllSubGraph()
{
	if(m_subGraphPartDataList.size() <= 0)
		return false;

	CSubGraphPartDataList::iterator i;
	for(i = m_subGraphPartDataList.begin(); i != m_subGraphPartDataList.end(); ++i)
	{
        CSubGraphPartData* pSubGraphPartData = *i;
		delete pSubGraphPartData;
	}

	m_subGraphPartDataList.clear();
	return true;
}

// ----------------------------------------------------------------------------
CString CGraphPartData::GetGroupName() const
{
	return m_strGroupName;
}

CGraphData& CGraphPartData::GetGraphData()
{
	return m_graphData;
}

CSubGraphPartDataList* CGraphPartData::GetSubGraphPartDataList()
{
	return &m_subGraphPartDataList;
}

const CGraphData& CGraphPartData::GetGraphData() const
{
	return m_graphData;
}

const CSubGraphPartDataList* CGraphPartData::GetSubGraphPartDataList() const
{
	return &m_subGraphPartDataList;
}

int CGraphPartData::GetSubGraphPartDataCount() const
{
	return m_subGraphPartDataList.size();
}

int CGraphPartData::GetSubGraphPartDataIndex(const CString& strSubGraphName) const
{
	if(strSubGraphName.IsEmpty() || m_subGraphPartDataList.size() <= 0)
		return -1;

	int nIndex = 0;
	CSubGraphPartDataList::const_iterator i;
	for(i = m_subGraphPartDataList.begin(); i != m_subGraphPartDataList.end(); ++i)
	{
        CSubGraphPartData* pSubGraphPartData = *i;
		assert(pSubGraphPartData != NULL);
		if(pSubGraphPartData->GetSubGraphName() == strSubGraphName)
			return nIndex;
		nIndex++;
	}
	return -1;
}

bool CGraphPartData::GetAllSubGraphDataList(CSubGraphDataList& subGraphDataList) const
{
	CSubGraphPartDataList::const_iterator i;
	for(i = m_subGraphPartDataList.begin(); i != m_subGraphPartDataList.end(); ++i)
	{
        CSubGraphPartData* pSubGraphPartData = *i;
		assert(pSubGraphPartData != NULL);
		subGraphDataList.AddTail(pSubGraphPartData->GetSubGraphData_PointeType());
	}

	return (subGraphDataList.GetCount() > 0);
}

CSubGraphData* CGraphPartData::GetSubGraphData(const CString& strSubGraphName) const
{
	CSubGraphPartData* pSubGraphPartData = GetSubGraphPartData(strSubGraphName);
	if(pSubGraphPartData == NULL)
		return NULL;

	return &pSubGraphPartData->GetSubGraphData();
}

CSubGraphData* CGraphPartData::GetSubGraphData(const int nIndex) const
{
	CSubGraphPartData* pSubGraphPartData = GetSubGraphPartData(nIndex);
	if(pSubGraphPartData == NULL)
		return NULL;

	return &pSubGraphPartData->GetSubGraphData();
}

CDoubleList* CGraphPartData::GetConditionList(const CString& strSubGraphName) const
{
	CSubGraphPartData* pSubGraphPartData = GetSubGraphPartData(strSubGraphName);
	if(pSubGraphPartData == NULL)
		return NULL;

	return &pSubGraphPartData->GetConditionList();
}

CDoubleList* CGraphPartData::GetConditionList(const int nSubGraphIndex) const
{
	CSubGraphPartData* pSubGraphPartData = GetSubGraphPartData(nSubGraphIndex);
	if(pSubGraphPartData == NULL)
		return NULL;

	return &pSubGraphPartData->GetConditionList();
}

CString CGraphPartData::GetPacketNames(const int nSubGraphIndex) const
{
	CSubGraphPartData* pSubGraphPartData = GetSubGraphPartData(nSubGraphIndex);
	if(pSubGraphPartData == NULL)
		return "";

	return pSubGraphPartData->GetCalPacketNames();
}

CString CGraphPartData::GetSubGraphTitle(const int nSubGraphIndex) const
{
	CSubGraphPartData* pSubGraphPartData = GetSubGraphPartData(nSubGraphIndex);
	if(pSubGraphPartData == NULL)
		return "";

	CIndiBaseData::TITLETYPE eTitleType = GetConditionTitle();
	if(eTitleType == CIndiBaseData::COND_TITLE) // 조건값
	{
		CString strCond;
		if(pSubGraphPartData->GetConditionData(0, strCond))
			return strCond;
	}
	else if(eTitleType == CIndiBaseData::BASE_COND_TITLE) // title + 조건값
	{
		CString strCond;
		if(pSubGraphPartData->GetConditionData(0, strCond))
			return (pSubGraphPartData->GetSubGraphData().GetTitle() + "_" + strCond);
	}

	return pSubGraphPartData->GetSubGraphData().GetTitle();
}

// ----------------------------------------------------------------------------
// graph를 만들기 위한 data.
// Graph;\r\n
// index;가격차트;시가,고가,저자,종가;signal사용여부:signal보임;
//		최저가최고가등락률;최저가현재가등락률;최고가현재가등락률;전일가현재가등락률;전일비;
//		최고값보기;최저값보기;부호표시;색표현방법(0:봉색,1:빨/파,2:양(빨),음(파));\r\n
// subGraphtype;subGraphStyle;subGraphDrawStyle;SubGraphName;title;숨김;색1;색2;색3;색4;색5;색6;
//		상승채움;하락채움;선굵기1;선굵기2;선굵기3;강조;\r\n
// .... subGraph 갯수만큼..
// GraphEnd;\r\n
void CGraphPartData::GetGraphDataForMaking( CString &p_strGraphData) const
{
	CString strGraphData;
	m_graphData.GetData( strGraphData);

	CString strAllSubGraphData;
	GetAllSubGraphDataForMaking( strAllSubGraphData);

	int nGraphDataLen = 0;
	nGraphDataLen += 8;									// "Graph;\r\n"
	nGraphDataLen += strGraphData.GetLength();
	nGraphDataLen += strAllSubGraphData.GetLength();
	nGraphDataLen += 11;								// "GraphEnd;\r\n"

	char *szGraphData = p_strGraphData.GetBufferSetLength( nGraphDataLen);

	strcpy( szGraphData, "Graph;\r\n");
	szGraphData += 8;

	strcpy( szGraphData, strGraphData);
	szGraphData += strGraphData.GetLength();

	strcpy( szGraphData, strAllSubGraphData);
	szGraphData += strAllSubGraphData.GetLength();

	strcpy( szGraphData, "GraphEnd;\r\n");
}

void CGraphPartData::GetGraphDataForMaking( CString &p_strGraphData, const int nCondIndexForTitle, const bool bIncludeOrgTitle) const
{
	CString strGraphData;
	m_graphData.GetData( strGraphData);

	CString strAllSubGraphData;
	GetAllSubGraphDataForMaking( strAllSubGraphData, nCondIndexForTitle, bIncludeOrgTitle);

	int nGraphDataLen = 0;
	nGraphDataLen += 8;									// "Graph;\r\n"
	nGraphDataLen += strGraphData.GetLength();
	nGraphDataLen += strAllSubGraphData.GetLength();
	nGraphDataLen += 11;								// "GraphEnd;\r\n"

	char *szGraphData = p_strGraphData.GetBufferSetLength( nGraphDataLen);

	strcpy( szGraphData, "Graph;\r\n");
	szGraphData += 8;

	strcpy( szGraphData, strGraphData);
	szGraphData += strGraphData.GetLength();

	strcpy( szGraphData, strAllSubGraphData);
	szGraphData += strAllSubGraphData.GetLength();

	strcpy( szGraphData, "GraphEnd;\r\n");
}

// subGraph를 만들기 위한 data.
// subGraphtype;subGraphStyle;subGraphDrawStyle;SubGraphName;title;숨김;색1;색2;색3;색4;색5;색6;
//		상승채움;하락채움;선굵기1;선굵기2;선굵기3;강조;\r\n
void CGraphPartData::GetAllSubGraphDataForMaking( CString &p_strAllSubGraphData) const
{
	p_strAllSubGraphData.Empty();

	if( m_subGraphPartDataList.size() <= 0) return;

	int nAllSubGraphDataLen = 0;
	CStringList slSubGraphData;
	CSubGraphPartDataList::const_iterator i;
	for(i = m_subGraphPartDataList.begin(); i != m_subGraphPartDataList.end(); ++i)
	{
        CSubGraphPartData* pSubGraphPartData = *i;
		assert(pSubGraphPartData != NULL);

		slSubGraphData.AddTail( CString());
		CString &strSubGraphData = slSubGraphData.GetTail();
		pSubGraphPartData->GetSubGraphStringData( strSubGraphData);
		nAllSubGraphDataLen += strSubGraphData.GetLength();
	}

	char *szAllSubGraphData = p_strAllSubGraphData.GetBufferSetLength( nAllSubGraphDataLen);
	POSITION psn = slSubGraphData.GetHeadPosition();
	while( psn)
	{
		CString &strSubGraphData = slSubGraphData.GetNext( psn);
		strcpy( szAllSubGraphData, strSubGraphData);
		szAllSubGraphData += strSubGraphData.GetLength();
	}
}

void CGraphPartData::GetAllSubGraphDataForMaking( CString &p_strAllSubGraphData, const int nCondIndexForTitle, const bool bIncludeOrgTitle) const
{
	p_strAllSubGraphData.Empty();

	if( m_subGraphPartDataList.size() <= 0) return;

	int nAllSubGraphDataLen = 0;
	CStringList slSubGraphData;
	CSubGraphPartDataList::const_iterator i;
	for(i = m_subGraphPartDataList.begin(); i != m_subGraphPartDataList.end(); ++i)
	{
        CSubGraphPartData* pSubGraphPartData = *i;
		assert(pSubGraphPartData != NULL);

		slSubGraphData.AddTail( CString());
		CString &strSubGraphData = slSubGraphData.GetTail();
		pSubGraphPartData->GetSubGraphStringData( strSubGraphData, nCondIndexForTitle, bIncludeOrgTitle);
		nAllSubGraphDataLen += strSubGraphData.GetLength();
	}

	char *szAllSubGraphData = p_strAllSubGraphData.GetBufferSetLength( nAllSubGraphDataLen);
	POSITION psn = slSubGraphData.GetHeadPosition();
	while( psn)
	{
		CString &strSubGraphData = slSubGraphData.GetNext( psn);
		strcpy( szAllSubGraphData, strSubGraphData);
		szAllSubGraphData += strSubGraphData.GetLength();
	}
}

/*
CString CGraphPartData::GetAllSubGraphDataForMaking(const CGraphColorPointerList& graphColorList) const
{
	if(m_subGraphPartDataList.size() <= 0)
		return "";

	CString strAllData, strSubGraphData;
	POSITION colorPos = graphColorList.GetHeadPosition();
	CSubGraphPartDataList::const_iterator i;
	for(i = m_subGraphPartDataList.begin(); i != m_subGraphPartDataList.end(); ++i)
	{
        CSubGraphPartData* pSubGraphPartData = *i;
		assert(pSubGraphPartData != NULL);
		if(colorPos != NULL)
		{
			CGraphColor* pNewColor = graphColorList.GetNext(colorPos);
			if(pNewColor != NULL)
			{
				pSubGraphPartData->GetSubGraphStringData(*pNewColor, strSubGraphData);
				strAllData += strSubGraphData;
				continue;
			}
		}
		pSubGraphPartData->GetSubGraphStringData( strSubGraphData);
		strAllData += strSubGraphData;
	}

	return strAllData;
}
*/

/*
CString CGraphPartData::GetAllSubGraphDataForMaking(const CGraphColorPointerList& graphColorList, const int nCondIndexForTitle, const bool bIncludeOrgTitle) const
{
	if(m_subGraphPartDataList.size() <= 0)
		return "";

	CString strAllData;
	POSITION colorPos = graphColorList.GetHeadPosition();
	CSubGraphPartDataList::const_iterator i;
	for(i = m_subGraphPartDataList.begin(); i != m_subGraphPartDataList.end(); ++i)
	{
        CSubGraphPartData* pSubGraphPartData = *i;
		assert(pSubGraphPartData != NULL);
		if(colorPos != NULL)
		{
			CGraphColor* pNewColor = graphColorList.GetNext(colorPos);
			if(pNewColor != NULL)
			{
				strAllData += pSubGraphPartData->GetSubGraphStringData(nCondIndexForTitle, bIncludeOrgTitle, *pNewColor);
				continue;
			}
		}
		strAllData += pSubGraphPartData->GetSubGraphStringData(nCondIndexForTitle, bIncludeOrgTitle);
	}

	return strAllData;
}
*/

// 모든 subGraph의 name를 가져오기.
bool CGraphPartData::GetSubGraphNameList(CStringDataList& SubGraphNameList) const
{
	SubGraphNameList.RemoveAll();

	CSubGraphPartDataList::const_iterator i;
	for(i = m_subGraphPartDataList.begin(); i != m_subGraphPartDataList.end(); ++i)
	{
        CSubGraphPartData* pSubGraphPartData = *i;
		assert(pSubGraphPartData != NULL);
		SubGraphNameList.AddTail(pSubGraphPartData->GetSubGraphName());
	}
	return (SubGraphNameList.GetCount() > 0);
}

// (2006/5/31 - Seung-Won, Bae) To compare with Main Block Graph Objects
BOOL CGraphPartData::GetSubGraphNames( CString &p_strSubGraphNames) const
{
	p_strSubGraphNames.Empty();

	CSubGraphPartDataList::const_iterator i;
	for(i = m_subGraphPartDataList.begin(); i != m_subGraphPartDataList.end(); ++i)
	{
        CSubGraphPartData* pSubGraphPartData = *i;
		assert(pSubGraphPartData != NULL);
		p_strSubGraphNames += pSubGraphPartData->GetSubGraphName() + ",";
	}
	return !p_strSubGraphNames.IsEmpty();
}

// 모든 subGraph의 title를 가져오기
bool CGraphPartData::GetSubGraphTitleList(CStringDataList& subGraphTitleList) const
{
	subGraphTitleList.RemoveAll();

	CSubGraphPartDataList::const_iterator i;
	for(i = m_subGraphPartDataList.begin(); i != m_subGraphPartDataList.end(); ++i)
	{
        CSubGraphPartData* pSubGraphPartData = *i;
		assert(pSubGraphPartData != NULL);
		subGraphTitleList.AddTail(pSubGraphPartData->GetSubGraphData().GetTitle());
	}
	return (subGraphTitleList.GetCount() > 0);
}

// 모든 subGraph의 color를 가져오기.
bool CGraphPartData::GetSubGraphColorList(CGraphColorPointerList& subGraphColorList) const
{
	subGraphColorList.RemoveAll();

	CSubGraphPartDataList::const_iterator i;
	for(i = m_subGraphPartDataList.begin(); i != m_subGraphPartDataList.end(); ++i)
	{
        CSubGraphPartData* pSubGraphPartData = *i;
		assert(pSubGraphPartData != NULL);
		subGraphColorList.AddTail(pSubGraphPartData->GetSubGraphData().GetColor_PointerType());
	}

	return (subGraphColorList.GetCount() > 0);
}

CIndiBaseData::TITLETYPE CGraphPartData::GetConditionTitle() const
{
	CString strIndicatorName = m_graphData.GetIndicatorName();
	if(strIndicatorName.IsEmpty())
		return CIndiBaseData::BASE_TITLE;

	if( g_iMetaTable.IsTitleOnlyCondition(strIndicatorName))
		return CIndiBaseData::COND_TITLE;
	else if( g_iMetaTable.IsTitleOrCondition(strIndicatorName))
		return CIndiBaseData::BASE_COND_TITLE;

	return CIndiBaseData::BASE_TITLE;
}

// ----------------------------------------------------------------------------
void CGraphPartData::SetGroupName(const CString& strGroupName)
{
	m_strGroupName = strGroupName;
}

// default 와 비교하여 user data를 검증 후 적용한다.
// -> groupName, IndicatorName(IndicatorName), packetName, signal사용유무, 
//	  SubGraphName, subGraphType 변경 안됨!!
void CGraphPartData::CopyDataWithVerificationFrom(const CGraphPartData& sourceData)
{
	SetGraphDataInVerifiable(sourceData.GetGraphData());
	SetSubGraphPartDataListInVerifiable(sourceData.GetSubGraphPartDataList());
}

bool CGraphPartData::SetGraphData(const CGraphData& graphData, const bool bIsAutoCheck)
{
	//함수명 비교.
	if(bIsAutoCheck)
	{
		if(m_graphData.GetIndicatorName() != graphData.GetIndicatorName())
			return false;
	}

	m_graphData = graphData;
	return true;
}

bool CGraphPartData::SetAllSubGraphPartDataList(const CSubGraphPartDataList* pSubGraphPartDataList)
{
	if(pSubGraphPartDataList == NULL || pSubGraphPartDataList->size() <= 0)
		return false;

	// 해당 순서대로 subGraphData를 수정한다.
	bool bResult = false;
	CSubGraphPartDataList::const_iterator t_i, s_i;
	for(t_i = m_subGraphPartDataList.begin(), s_i = pSubGraphPartDataList->begin();
		t_i != m_subGraphPartDataList.end() && s_i != pSubGraphPartDataList->end(); 
		++t_i, ++s_i)
	{
        CSubGraphPartData* pTarget = *t_i;
        CSubGraphPartData* pSource = *s_i;
		assert(pTarget != NULL && pSource != NULL);
		if(pTarget->CopyDataWithVerificationFrom(pSource))
			bResult = true;
	}

	return bResult;
}

bool CGraphPartData::SetAllSubGraphDataList(const CSubGraphDataList* pSubGraphDataList)
{
	// subGraph count == 0 : 새로 subGraphPartData 생성
	if(m_subGraphPartDataList.size() <= 0)
		return MakeSubGraphPartDataList(pSubGraphDataList);

	// subGraph count != 0 : 기존의 subGraphPartData 변경
	return SetAllSubGraphDataList(pSubGraphDataList, m_subGraphPartDataList);
}

bool CGraphPartData::SetAllPacketNames_SubGraphPart(const CString& strPacketNames, const bool bIsAutoCheck)
{
	bool bResult = false;
	CSubGraphPartDataList::const_iterator i;
	for(i = m_subGraphPartDataList.begin(); i != m_subGraphPartDataList.end(); ++i)
	{
        CSubGraphPartData* pSubGraphPartData = *i;
		assert(pSubGraphPartData != NULL);
		if(pSubGraphPartData->SetCalPacketNames(strPacketNames, bIsAutoCheck))
			bResult = true;
	}
	return bResult;
}

bool CGraphPartData::SetPacketNames(const int nSubGraphIndex, const CString& strPacketNames)
{
	CSubGraphPartData* pSubGraphPartData = GetSubGraphPartData(nSubGraphIndex);
	if(pSubGraphPartData == NULL)
		return false;

	return pSubGraphPartData->SetCalPacketNames(strPacketNames);
}

bool CGraphPartData::SetAllCalConditionData(const int nCondIndex, const double& dData)
{
	bool bResult = false;
	CSubGraphPartDataList::const_iterator i;
	for(i = m_subGraphPartDataList.begin(); i != m_subGraphPartDataList.end(); ++i)
	{
        CSubGraphPartData* pSubGraphPartData = *i;
		assert(pSubGraphPartData != NULL);
		if(pSubGraphPartData->SetCalConditionData(nCondIndex, dData))
			bResult = true;
	}

	return bResult;
}

bool CGraphPartData::SetSubGraphData(const CSubGraphData* pSourceData)
{
	if(pSourceData == NULL)
		return false;

	CSubGraphData* pTargetData = GetSubGraphData(pSourceData->GetName());
	if(pTargetData == NULL)
		return false;

	*pTargetData = *pSourceData;
	return true;
}

bool CGraphPartData::SetSubGraphTitle(const int nIndex, const CString& strTitle)
{
	CSubGraphData* pSubGraphData = GetSubGraphData(nIndex);
	if(pSubGraphData == NULL)
		return false;

	pSubGraphData->SetTitle(strTitle);
	return true;
}

bool CGraphPartData::SetSubGraphColor(const int nIndex, const CGraphColor& graphColor)
{
	CSubGraphData* pSubGraphData = GetSubGraphData(nIndex);
	if(pSubGraphData == NULL)
		return false;

	pSubGraphData->SetColor(graphColor);
	return true;
}

bool CGraphPartData::SetSubGraphPenThickness(const int nIndex, const CPenThickness& penThickness)
{
	CSubGraphData* pSubGraphData = GetSubGraphData(nIndex);
	if(pSubGraphData == NULL)
		return false;

	pSubGraphData->SetPenThickness(penThickness);
	return true;
}


// ----------------------------------------------------------------------------
// subGraph 에 있는 계산 조건값을 title로 바꾼다.
bool CGraphPartData::ChangeCalConditionDataFromSubGraphTitle()
{
	bool bResult = false;
	CSubGraphPartDataList::const_iterator i;
	for(i = m_subGraphPartDataList.begin();	i != m_subGraphPartDataList.end(); ++i)
	{
        CSubGraphPartData* pSubGraphPartData = *i;
		assert(pSubGraphPartData != NULL);
		if(pSubGraphPartData->ChangeCalConditionDataFromSubGraphTitle())
			bResult = true;
	}

	return bResult;
}


// private ====================================================================
bool CGraphPartData::MakeSubGraphPartDataList(const CSubGraphDataList* pSubGraphDataList)
{
	if(pSubGraphDataList == NULL || pSubGraphDataList->GetCount() <= 0)
		return false;

	DeleteAllSubGraph();

	POSITION pos = pSubGraphDataList->GetHeadPosition();
	{
        CSubGraphData* pSourceSubGraphData = pSubGraphDataList->GetNext(pos);
		assert(pSourceSubGraphData != NULL);
        CSubGraphPartData* pNewSubGraphPartData = new CSubGraphPartData(*pSourceSubGraphData);
		m_subGraphPartDataList.push_back(pNewSubGraphPartData);
	}

	return (m_subGraphPartDataList.size() > 0);
}

void CGraphPartData::CopySubGraphPartDataList(const CSubGraphPartDataList* pSourceData)
{
	DeleteAllSubGraph();

	CSubGraphPartDataList::const_iterator i;
	for(i = pSourceData->begin(); i != pSourceData->end(); ++i)
	{
        CSubGraphPartData* pNewSubGraphPartData = new CSubGraphPartData(*(*i));
		m_subGraphPartDataList.push_back(pNewSubGraphPartData);
	}
}

// ----------------------------------------------------------------------------
// default 와 비교하여 user data를 검증 후 적용한다.
// -> IndicatorName(IndicatorName), packetName, signal사용유무, 변경 안됨!!
void CGraphPartData::SetGraphDataInVerifiable(const CGraphData& sourceData)
{
//	m_graphData.SetFunctionIndex( sourceData.GetFunctionIndex());

	if(m_graphData.IsSignalUse()) m_graphData.SetSignalData( true, sourceData.IsSignalShow());

	m_graphData.SetGraphDataDrawingData( sourceData.GetGraphDataDrawingData());

	// (2006/5/28 - Seung-Won, Bae) with minmaxdatadrawing
//	m_graphData.SetViewMinMaxDrawingData( sourceData.GetViewMinMaxDrawingData());
}

// -> count, SubGraphName, subGraphType 변경 안됨!!
void CGraphPartData::SetSubGraphPartDataListInVerifiable(const CSubGraphPartDataList* pSourceData)
{
	// (2006/5/30 - Seung-Won, Bae) Do not check Sub Graph Count. (Update possible SubGraph.)
	if(pSourceData == NULL) return;

	// 해당 SubGraphName에 해당되는 subGraphData로 변경.
	CSubGraphPartDataList::const_iterator i;
	for(i = m_subGraphPartDataList.begin(); i != m_subGraphPartDataList.end(); ++i)
	{
        CSubGraphPartData* pTarget = *i;
		assert(pTarget != NULL);
		CSubGraphPartData* pSource = GetSubGraphPartData(pSourceData, pTarget->GetSubGraphName());
		if( pSource) pTarget->CopyDataWithVerificationFrom(pSource);
	}
}

// 모든 data 변경 (subGraphPartData)
bool CGraphPartData::SetAllSubGraphDataList(const CSubGraphDataList* pSubGraphDataList, CSubGraphPartDataList& targetData)
{
	if(pSubGraphDataList == NULL || pSubGraphDataList->GetCount() <= 0)
		return false;

	// 해당 순서대로 subGraphData를 수정한다.
	bool bResult = false;
	POSITION pos = pSubGraphDataList->GetHeadPosition();
	CSubGraphPartDataList::const_iterator t_i;
	for(t_i = targetData.begin(); t_i != targetData.end() && pos != NULL; ++t_i)
	{
        CSubGraphPartData* pTarget = *t_i;
        CSubGraphData* pSourceSubGraphData = pSubGraphDataList->GetNext(pos);
		assert(pTarget != NULL && pSourceSubGraphData != NULL);
		if(pTarget->CopyDataWithVerificationFrom(*pSourceSubGraphData))
			bResult = true;
	}

	return bResult;
}

// ----------------------------------------------------------------------------

// (2006/5/28 - Seung-Won, Bae) for Old User Indicator Info. Add New SubGraph.
BOOL CGraphPartData::AddSubGraphPartDataWithCopy(const CSubGraphPartData* pSubGraphPartData)
{
	m_subGraphPartDataList.push_back( new CSubGraphPartData( *pSubGraphPartData));
	return true;
}
CSubGraphPartData* CGraphPartData::GetSubGraphPartData(const CString& strSubGraphName) const
{
	return GetSubGraphPartData(&m_subGraphPartDataList, strSubGraphName);
}
CSubGraphPartData* CGraphPartData::GetSubGraphPartData(const int nSubGraphIndex) const
{
	if(nSubGraphIndex < 0 || nSubGraphIndex >= m_subGraphPartDataList.size())
		return NULL;

	int nCount = 0;
	CSubGraphPartDataList::const_iterator i;
	for(i = m_subGraphPartDataList.begin(); i != m_subGraphPartDataList.end(); ++i)
	{
        CSubGraphPartData* pSubGraphPartData = *i;
		assert(pSubGraphPartData != NULL);
		if(nSubGraphIndex == nCount)
			return pSubGraphPartData;
		nCount++;
	}

	return NULL;
}
CSubGraphPartData* CGraphPartData::GetSubGraphPartData(const CSubGraphPartDataList* pSubGraphPartDataList, const CString& strSubGraphName) const
{
	if(pSubGraphPartDataList->size() <= 0)
		return NULL;

	CSubGraphPartDataList::const_iterator i;
	for(i = pSubGraphPartDataList->begin(); i != pSubGraphPartDataList->end(); ++i)
	{
        CSubGraphPartData* pSubGraphPartData = *i;
		assert(pSubGraphPartData != NULL);
		if(pSubGraphPartData->GetSubGraphName() == strSubGraphName)
			return pSubGraphPartData;
	}

	return NULL;
}

// (2006/5/29 - Seung-Won, Bae) Update CGraphDataDrawingData ('graphdatadrawing')
void CGraphPartData::SetGraphDataDrawingDataWithCopy( const CGraphDataDrawingData *p_pGraphDataDrawingData)
{
	m_graphData.SetGraphDataDrawingData( *p_pGraphDataDrawingData);
}
CGraphDataDrawingData *CGraphPartData::GetGraphDataDrawingData( void)
{
	return &m_graphData.GetGraphDataDrawingData();
}

// (2007/2/28 - Seung-Won, Bae) Retrieve Sub Graph Hiding
BOOL CGraphPartData::GetSubGraphHiding( const char *p_szSubGraphName)
{
	CSubGraphData *pSubGraphData = GetSubGraphData( p_szSubGraphName);
	if( !pSubGraphData) return FALSE;
	return pSubGraphData->GetHiding();
}
BOOL CGraphPartData::GetSubGraphHiding( const int p_nSubGraphIndex)
{
	CSubGraphData *pSubGraphData = GetSubGraphData( p_nSubGraphIndex);
	if( !pSubGraphData) return FALSE;
	return pSubGraphData->GetHiding();
}

///////////////////////////////////////////////////////////////////////////////
// class CBaseIndicatorList
///////////////////////////////////////////////////////////////////////////////
CBaseIndicatorList::CBaseIndicatorList( EIndicatorListType p_eIndicatorListType) :
	m_dVersion(0.0)
{
	// (2006/11/30 - Seung-Won, Bae) Support Indicator List Type Manage.
	m_eIndicatorListType = p_eIndicatorListType;

	switch( m_eIndicatorListType)
	{
		case EILT_SITE_DEFAULT:
		case EILT_USER_DEFAULT:
								m_pGeneralMap	= new CDefaultIndicatorInfoMap( EIIT_GENERAL);	// 일반지표(default)
								m_pSpecialMap	= new CDefaultIndicatorInfoMap( EIIT_SPECIAL);	// 특수지표(default)
								m_pNoneIndiMap	= new CDefaultIndicatorInfoMap( EIIT_NONE);		// none지표(default)
								break;
		case EILT_MAP_DEFAULT:
		case EILT_USER:
		default:
								m_pGeneralMap	= new CGraphIndicatorInfoMap( EIIT_GENERAL);	// 일반지표(default)
								m_pSpecialMap	= new CGraphIndicatorInfoMap( EIIT_SPECIAL);	// 특수지표(default)
								m_pNoneIndiMap	= new CGraphIndicatorInfoMap( EIIT_NONE);		// none지표(default)
								break;
	}
}
CBaseIndicatorList::~CBaseIndicatorList()
{
	m_pGeneralMap->DeleteAll();
	m_pSpecialMap->DeleteAll();
	m_pNoneIndiMap->DeleteAll();

	delete m_pGeneralMap;
	delete m_pSpecialMap;
	delete m_pNoneIndiMap; // none지표(default)
}

// Version Info
double CBaseIndicatorList::GetVersion() const
{
	return m_dVersion;
}
bool CBaseIndicatorList::SetVersion( char *&p_szData)
{
	double dVersion = GetVersion( p_szData);
	if(dVersion <= 0.0) return false;

	m_dVersion = dVersion;
	return true;
}
void CBaseIndicatorList::SetVersion(const double& dVersion)
{
	m_dVersion = dVersion;
}
double CBaseIndicatorList::GetVersion( char *&p_szData) const
{
	if( !p_szData) return 0.0;
	if( !*p_szData) return 0.0;

	char *szVersion = strstr( p_szData, "Ver");
	if( !szVersion) return 0.0;
	szVersion += 3;	// skip 'Ver'
	while( *szVersion == ' ') szVersion++;		// skip Space.

	char *szAsterisk = strchr( szVersion, '*');
	if( !szAsterisk) return 0.0;
	while( *( szAsterisk - 1) == ' ') szAsterisk--;		// skip Space.

	*szAsterisk = '\0';
	p_szData = szAsterisk + 1;

	return atof( szVersion);
}


// Info Map
CIndicatorInfoMap *CBaseIndicatorList::GetGeneralIndiMap()
{
	return m_pGeneralMap;
}
CIndicatorInfoMap *CBaseIndicatorList::GetSpecialIndiMap()
{
	return m_pSpecialMap;
}
CIndicatorInfoMap *CBaseIndicatorList::GetNoneIndiMap()
{
	return m_pNoneIndiMap;
}
const CIndicatorInfoMap *CBaseIndicatorList::GetGeneralIndiMap() const
{
	return m_pGeneralMap;
}
const CIndicatorInfoMap *CBaseIndicatorList::GetSpecialIndiMap() const
{
	return m_pSpecialMap;
}
const CIndicatorInfoMap *CBaseIndicatorList::GetNoneIndiMap() const
{
	return m_pNoneIndiMap;
}


// Add and Delete
bool CBaseIndicatorList::AddIndicatorInfo( CIndicatorInfo* pIndicatorInfo)
{
	if( pIndicatorInfo == NULL) return false;

	switch( pIndicatorInfo->GetIndicatorInfoType())
	{
		case EIIT_GENERAL:	return m_pGeneralMap->AddIndicatorInfo( pIndicatorInfo);
		case EIIT_SPECIAL:	return m_pSpecialMap->AddIndicatorInfo( pIndicatorInfo);
		case EIIT_NONE:		return m_pNoneIndiMap->AddIndicatorInfo( pIndicatorInfo);
	}
	return false;
}
bool CBaseIndicatorList::Delete(CIndicatorInfo* pIndicatorInfo)
{
	if( pIndicatorInfo == NULL) return false;
	
	if( m_pGeneralMap->Delete( pIndicatorInfo)) return true;
	if( m_pSpecialMap->Delete( pIndicatorInfo)) return true;
	return m_pNoneIndiMap->Delete( pIndicatorInfo);
}
bool CBaseIndicatorList::DeleteAll( const CBaseIndicatorList::INDICATORTYPE eDeleteType)
{
	switch(eDeleteType)
	{
	case ALL:
		m_pGeneralMap->DeleteAll();
		m_pSpecialMap->DeleteAll();
		m_pNoneIndiMap->DeleteAll();
		return true;
	case GENERAL:
		return m_pGeneralMap->DeleteAll();
	case SPECIAL:
		return m_pSpecialMap->DeleteAll();
	}

	return false;
}
bool CBaseIndicatorList::RemoveAllIndicator( const CBaseIndicatorList::INDICATORTYPE eDeleteType, const char *p_szIndicatorName)
{
	switch(eDeleteType)
	{
	case ALL:
		m_pGeneralMap->RemoveAllIndicator( p_szIndicatorName);
		m_pSpecialMap->RemoveAllIndicator( p_szIndicatorName);
		m_pNoneIndiMap->RemoveAllIndicator( p_szIndicatorName);
		return true;
	case GENERAL:
		return m_pGeneralMap->RemoveAllIndicator( p_szIndicatorName);
	case SPECIAL:
		return m_pSpecialMap->RemoveAllIndicator( p_szIndicatorName);
	}

	return false;
}

// List Info
bool CBaseIndicatorList::IsEmpty() const
{
	return 0 < m_pGeneralMap->GetCount() || 0 < m_pSpecialMap->GetCount() || 0 < m_pNoneIndiMap->GetCount();
}

// (2009/9/2 - Seung-Won, Bae) Support Chart Mode. (for HTS/FX/WorldOn)
bool CBaseIndicatorList::GetIndicatorNameList(CBaseIndicatorList::INDICATORTYPE eType, CStringDataList& dataList, CChartInfo::CHART_MODE p_eChartMode) const
{
	dataList.RemoveAll();

	switch(eType)
	{
	case ALL:
		m_pGeneralMap->GetIndicatorNameList( dataList, p_eChartMode);
		m_pSpecialMap->GetIndicatorNameList( dataList, p_eChartMode);
		m_pNoneIndiMap->GetIndicatorNameList( dataList, p_eChartMode);
		break;
	case GENERAL:
		m_pGeneralMap->GetIndicatorNameList( dataList, p_eChartMode);
		break;
	case SPECIAL:
		m_pSpecialMap->GetIndicatorNameList( dataList, p_eChartMode);
		break;
	case NONE:
		m_pNoneIndiMap->GetIndicatorNameList( dataList, p_eChartMode);
	default:
		return false;
	}

	return (dataList.GetCount() > 0);
}

// (2009/9/2 - Seung-Won, Bae) Support Chart Mode. (for HTS/FX/WorldOn)
bool CBaseIndicatorList::GetGraphNameList( CBaseIndicatorList::INDICATORTYPE eType, CStringDataList& dataList, CChartInfo::CHART_MODE p_eChartMode) const
{
	dataList.RemoveAll();

	switch(eType)
	{
	case ALL:
		m_pGeneralMap->GetGraphNameList( dataList, p_eChartMode);
		m_pSpecialMap->GetGraphNameList( dataList, p_eChartMode);
		m_pNoneIndiMap->GetGraphNameList( dataList, p_eChartMode);
		break;
	case GENERAL:
		m_pGeneralMap->GetGraphNameList( dataList, p_eChartMode);
		break;
	case SPECIAL:
		m_pSpecialMap->GetGraphNameList( dataList, p_eChartMode);
		break;
	case NONE:
		m_pNoneIndiMap->GetGraphNameList( dataList, p_eChartMode);
	default:
		return false;
	}

	return (dataList.GetCount() > 0);
}

// (2009/9/2 - Seung-Won, Bae) Support Chart Mode. (for HTS/FX/WorldOn)
bool CBaseIndicatorList::GetAllIndicatorNameWithGroupName( CIndicatorGroupDataList& indiGroupDataList, CChartInfo::CHART_MODE p_eChartMode) const
{
	indiGroupDataList.DeleteAll();

	m_pGeneralMap->GetAllIndicatorNameWithGroupName( indiGroupDataList, p_eChartMode);
	m_pSpecialMap->GetAllIndicatorNameWithGroupName( indiGroupDataList, p_eChartMode);
	m_pNoneIndiMap->GetAllIndicatorNameWithGroupName( indiGroupDataList, p_eChartMode);

	return (indiGroupDataList.GetCount() > 0);
}

// Get Indicator Info
// (2009/9/2 - Seung-Won, Bae) Support Chart Mode. (for HTS/FX/WorldOn)
CIndicatorInfo *CBaseIndicatorList::GetIndicatorInfo( const char *p_szIndicatorKeyName, CChartInfo::CHART_MODE p_eChartMode)
{
	if( !p_szIndicatorKeyName) return NULL;
	if( !*p_szIndicatorKeyName) return NULL;

	CIndicatorInfo *pIndicatorInfo = m_pGeneralMap->GetIndicatorInfo( p_szIndicatorKeyName, p_eChartMode);
	if( pIndicatorInfo) return pIndicatorInfo;

	pIndicatorInfo = m_pSpecialMap->GetIndicatorInfo( p_szIndicatorKeyName, p_eChartMode);
	if( pIndicatorInfo) return pIndicatorInfo;

	return m_pNoneIndiMap->GetIndicatorInfo( p_szIndicatorKeyName, p_eChartMode);
}

// (2009/9/2 - Seung-Won, Bae) Support Chart Mode. (for HTS/FX/WorldOn)
CIndicatorInfo *CBaseIndicatorList::GetFirstIndicatorInfo( const char *p_szIndicatorName, POSITION *p_ppsnIndicator, CChartInfo::CHART_MODE p_eChartMode) const
{
	if( !p_szIndicatorName) return NULL;
	if( !*p_szIndicatorName) return NULL;

	CIndicatorInfo *pIndicatorInfo = m_pGeneralMap->GetFirstIndicatorInfo( p_szIndicatorName, p_ppsnIndicator, p_eChartMode);
	if( pIndicatorInfo) return pIndicatorInfo;

	pIndicatorInfo = m_pSpecialMap->GetFirstIndicatorInfo( p_szIndicatorName, p_ppsnIndicator, p_eChartMode);
	if( pIndicatorInfo) return pIndicatorInfo;

	return m_pNoneIndiMap->GetFirstIndicatorInfo( p_szIndicatorName, p_ppsnIndicator, p_eChartMode);
}

void CBaseIndicatorList::GetNextIndicatorInfo( const char *p_szIndicatorName, POSITION *p_ppsnIndicator, CIndicatorInfo *&p_pPrevIndicatorInfo) const
{
	if( !p_ppsnIndicator)			p_pPrevIndicatorInfo = NULL;
	if( !p_pPrevIndicatorInfo)
	{
		*p_ppsnIndicator = NULL;
		return;
	}

	EIndicatorInfoType eIndicatorInfoType = ( EIndicatorInfoType)-1;
	if( p_szIndicatorName) if( *p_szIndicatorName) eIndicatorInfoType = p_pPrevIndicatorInfo->GetIndicatorInfoType();
	switch( eIndicatorInfoType)
	{
		case EIIT_GENERAL:	p_pPrevIndicatorInfo = m_pGeneralMap->GetNextIndicatorInfo( p_szIndicatorName, p_ppsnIndicator);
							break;
		case EIIT_SPECIAL:	p_pPrevIndicatorInfo = m_pGeneralMap->GetNextIndicatorInfo( p_szIndicatorName, p_ppsnIndicator);
							break;
		case EIIT_NONE:		p_pPrevIndicatorInfo = m_pGeneralMap->GetNextIndicatorInfo( p_szIndicatorName, p_ppsnIndicator);
							break;
		default:			*p_ppsnIndicator = NULL;
							p_pPrevIndicatorInfo = NULL;
							break;
	}
}


///////////////////////////////////////////////////////////////////////////////
// class CDefaultIndicatorList

// (2006/11/30 - Seung-Won, Bae) Support Indicator List Type Manage.
CDefaultIndicatorList::CDefaultIndicatorList( EIndicatorListType p_eIndicatorListType) :
	CBaseIndicatorList( p_eIndicatorListType)
{
}

CDefaultIndicatorList::~CDefaultIndicatorList()
{
}

// public =====================================================================
// file data를 객체로 만들기
bool CDefaultIndicatorList::AddIndiInfoFromCfgString( CIndicatorList* pIndicatorList, CGraphTypeAndStyleData* pGraphTypeAndStyleData, char *&p_szDataString, HWND p_hOcxWnd)
{
	if( !CBaseIndicatorList::SetVersion( p_szDataString)) return false;

	DeleteAll(CBaseIndicatorList::ALL);

	CDefaultTypeIndiListBuilder fileBuilder( pGraphTypeAndStyleData, p_hOcxWnd);
	return fileBuilder.FileDataBuild( this, p_szDataString);
}

///////////////////////////////////////////////////////////////////////////////
// class CUserIndicatorList

CUserIndicatorList::CUserIndicatorList( CDefaultIndicatorList* pDefaultIndiList, EIndicatorListType p_eIndicatorListType, HWND p_hOcxWnd) :
	CBaseIndicatorList( p_eIndicatorListType)
{
	m_pDefaultIndiList = pDefaultIndiList;

	// (2008/1/14 - Seung-Won, Bae) for MultiLanguage.
	m_hOcxWnd = p_hOcxWnd;
}

// public =====================================================================
// file data를 객체로 만들기
bool CUserIndicatorList::AddIndiInfoFromCfgString(CIndicatorList* pIndicatorList, char *&p_szDataString, const bool dDoesDeleteOldData)
{
	if( !CBaseIndicatorList::SetVersion( p_szDataString)) return false;

	bool bResult = false;
	// 기존의 data는 삭제한다.
	if(dDoesDeleteOldData)
	{
		bResult = CBaseIndicatorList::DeleteAll(CBaseIndicatorList::ALL);
	}

	// file 읽기
	double dUserVersion = CBaseIndicatorList::GetVersion();
	if(dUserVersion <= 0.0 || !p_szDataString) return false;

	// 현재 버전과 같은 경우
	if( dUserVersion == GetDefaultVersion())
		return CDefaultUserTypeIndiListBuilder( m_hOcxWnd).FileDataBuild( this, p_szDataString);

	// 사용자 버전이 3.21 인 경우
	if( dUserVersion > 3.2 && dUserVersion < 3.3)
	{
		// (2006/6/1 - Seung-Won, Bae) Manage static for Operation Map Init.
		static CUserVer3Dot21TypeIndiListBuilder dilBuilder( m_pDefaultIndiList, m_hOcxWnd);
		return dilBuilder.FileDataBuild( this, p_szDataString);
	}
	else if( dUserVersion > 4.9 && dUserVersion < 5.1)
	{
		static CUserVer5Dot00TypeIndiListBuilder dilBuilder( m_pDefaultIndiList, m_hOcxWnd);
		return dilBuilder.FileDataBuild(  this ,p_szDataString);
	}

	else if( dUserVersion > 5.0 && dUserVersion < 5.2)
	{
		static CUserVer5Dot10TypeIndiListBuilder dilBuilder( m_pDefaultIndiList, m_hOcxWnd);
		return dilBuilder.FileDataBuild(  this ,p_szDataString);
	}

	// (2008/6/13 - Seung-Won, Bae) for BuySellOverhaningSupply's Draw Option
	else if( dUserVersion > 5.199 && dUserVersion < 5.210)
	{
		static CUserVer5Dot20TypeIndiListBuilder dilBuilder( m_pDefaultIndiList, m_hOcxWnd);
		return dilBuilder.FileDataBuild(  this ,p_szDataString);
	}

	// 기타 현재 버전과 틀린 경우
	return CNotDefaultUserTypeIndiListBuilder( m_pDefaultIndiList, m_hOcxWnd).FileDataBuild( this, p_szDataString);
}

// private ====================================================================
double CUserIndicatorList::GetDefaultVersion() const
{
	if(m_pDefaultIndiList == NULL)
		return 0.0;

	return (m_pDefaultIndiList->GetVersion());
}

// user 관련 지표들의 data (저장할 data) -> default 형태로 저장한다.
void CUserIndicatorList::GetWritingFileData( CStringList &p_slDataString) const
{
	p_slDataString.AddTail( "***** Indicator List : Ver ");

	p_slDataString.AddTail( CString());
	CString &strVer = p_slDataString.GetTail();
	strVer.Format( "%.2f", GetWritingFileVersion());

	p_slDataString.AddTail( " *****\r\n");
	CDefaultUserTypeIndiListBuilder( m_hOcxWnd).GetWritingFileData( *this, p_slDataString);
	p_slDataString.AddTail( "\r\n***** File End *****\r\n");
}


double CUserIndicatorList::GetWritingFileVersion() const
{
	if(m_pDefaultIndiList == NULL)
		return CBaseIndicatorList::GetVersion();

	return (m_pDefaultIndiList->GetVersion());
}
