// GraphTypeData.cpp: implementation of the CGraphTypeData class.
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../Include_Chart/DLL_Load/GraphTypeData.h"

#include <assert.h>
#include "../Include_Chart/Dll_Load/IMetaTable.h"			// for _MTEXT()
#include "BlockBaseData.h"
#include "FileBuilder.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////
// class CGraphTypeData

CGraphTypeData* CGraphTypeData::Make()
{
	return new CGraphTypeData();
}

void CGraphTypeData::Delete(CGraphTypeData* pGraphTypeData)
{
	delete pGraphTypeData;
}

CGraphTypeData::CGraphTypeData() :
	m_strName("")
{
}

CGraphTypeData::~CGraphTypeData()
{
}


// public =====================================================================
CString CGraphTypeData::GetName() const
{
	return m_strName;
}

CStringDataList* CGraphTypeData::GetStyleDataList()
{
	return &m_styleDataList;
}

const CStringDataList* CGraphTypeData::GetStyleDataList() const
{
	return &m_styleDataList;
}

int CGraphTypeData::GetStyleCount() const
{
	return m_styleDataList.GetCount();
}

CString CGraphTypeData::GetStyle(const int nIndex) const
{
	POSITION pos = m_styleDataList.FindIndex(nIndex);
	if(pos == NULL)
		return "";

	return m_styleDataList.GetAt(pos);
}

int CGraphTypeData::GetStyleIndex(const CString& strStyle) const
{
	if(strStyle.IsEmpty())
		return -1;

	int nIndex = 0;
	POSITION pos = m_styleDataList.GetHeadPosition();
	while(pos != NULL){
        CString strData = m_styleDataList.GetNext(pos);;
		if(strStyle == strData)
			return nIndex;

		nIndex++;
	}
	
	return -1;
}

// ----------------------------------------------------------------------------
void CGraphTypeData::SetName(const CString& strName)
{
	m_strName = strName;
}


///////////////////////////////////////////////////////////////////////////////
// class CGraphTypeDataList

CGraphTypeDataList::~CGraphTypeDataList()
{
	DeleteAll();
}

void CGraphTypeDataList::AddTail(CGraphTypeData* pGraphTypeData)
{
	if(pGraphTypeData == NULL)
		return;

	m_graphTypeDataList.push_back(pGraphTypeData);
}

void CGraphTypeDataList::DeleteAll()
{
	if(m_graphTypeDataList.size() <= 0)
		return;

	CGraphTypeDataDeque::iterator i;
	for(i = m_graphTypeDataList.begin(); i != m_graphTypeDataList.end(); ++i){
        CGraphTypeData* pGraphTypeData = *i;
		CGraphTypeData::Delete(pGraphTypeData);
	}

	m_graphTypeDataList.clear();
}

// ----------------------------------------------------------------------------
bool CGraphTypeDataList::IsEmpty() const
{
	return (m_graphTypeDataList.size() <= 0);
}

int CGraphTypeDataList::GetCount() const
{
	return m_graphTypeDataList.size();
}

CGraphTypeData* CGraphTypeDataList::GetGraphTypeData(const CString& strName) const
{
	if(m_graphTypeDataList.size() <= 0)
		return NULL;

	CGraphTypeDataDeque::const_iterator i;
	for(i = m_graphTypeDataList.begin(); i != m_graphTypeDataList.end(); ++i){
        CGraphTypeData* pGraphTypeData = *i;
		assert(pGraphTypeData != NULL);
		if(pGraphTypeData->GetName() == strName){
			return pGraphTypeData;
		}
	}
	return NULL;
}

// ----------------------------------------------------------------------------
bool CGraphTypeDataList::GetDataIndex(const CString& strName, const CString& strStyle, 
		int& nNameIndex, int& nStyleIndex) const
{
	if(strName.IsEmpty())
		return false;

	// (2006/11/2 - Seung-Won, Bae) Support for Old Price Chart Type Manage
	CString strNewStyle( strStyle);
	if( strName == _MTEXT( C5_CANDLE_TYPE))
	{
		if(			strNewStyle == _MTEXT( C5_JAPANESE_STYLE))	strNewStyle = _MTEXT( C5_CANDLE_CHART);
		else if(	strNewStyle == _MTEXT( C5_AMERICAN_STYLE))	strNewStyle = _MTEXT( C5_BAR_CHART_OPEN_HIGH_LOW_CLOSE);
		else if(	strNewStyle == _MTEXT( C5_LINE_STYLE))		strNewStyle = _MTEXT( C5_LINE_CHART);
		else if(	strNewStyle == _MTEXT( C5_ANTENNA_STYLE))	strNewStyle = _MTEXT( C5_ANTENNA_CHART);
	}

	int nIndex = 0;
	CGraphTypeDataDeque::const_iterator i;
	for(i = m_graphTypeDataList.begin(); i != m_graphTypeDataList.end(); ++i){
        CGraphTypeData* pGraphTypeData = *i;
		assert(pGraphTypeData != NULL);
		if(pGraphTypeData->GetName() == strName){
			nNameIndex = nIndex;
			nStyleIndex = pGraphTypeData->GetStyleIndex( strNewStyle);
			return true;
		}
		nIndex++;
	}

	return false;
}

int CGraphTypeDataList::GetStyleDataIndex(const CString& strStyle) const
{
	if(strStyle.IsEmpty())
		return -1;

	// (2006/11/2 - Seung-Won, Bae) Support for Old Price Chart Type Manage
	CString strNewStyle( strStyle);
	if(			strNewStyle == _MTEXT( C5_JAPANESE_STYLE))	strNewStyle = _MTEXT( C5_CANDLE_CHART);
	else if(	strNewStyle == _MTEXT( C5_AMERICAN_STYLE))	strNewStyle = _MTEXT( C5_BAR_CHART_OPEN_HIGH_LOW_CLOSE);
	else if(	strNewStyle == _MTEXT( C5_LINE_STYLE))		strNewStyle = _MTEXT( C5_LINE_CHART);
	else if(	strNewStyle == _MTEXT( C5_ANTENNA_STYLE))	strNewStyle = _MTEXT( C5_ANTENNA_CHART);

	CGraphTypeDataDeque::const_iterator i;
	for(i = m_graphTypeDataList.begin(); i != m_graphTypeDataList.end(); ++i){
        CGraphTypeData* pGraphTypeData = *i;
		assert(pGraphTypeData != NULL);
		int nStyleIndex = pGraphTypeData->GetStyleIndex( strNewStyle);
		if(nStyleIndex >= 0){
			return nStyleIndex;
		}
	}
	return -1;
}

bool CGraphTypeDataList::GetData(const int nTypeIndex, const int nStyleIndex, 
		CString& strName, CString& strStyle) const
{
	if(!IsExisting(nTypeIndex))
		return false;

	CGraphTypeData* pGraphTypeData = m_graphTypeDataList[nTypeIndex];
	assert(pGraphTypeData != NULL);

	strName = pGraphTypeData->GetName();
	strStyle = pGraphTypeData->GetStyle(nStyleIndex);
	return (!strName.IsEmpty() && !strStyle.IsEmpty());
}

bool CGraphTypeDataList::GetAllNamesList(CStringDataList& namesList) const
{
	namesList.RemoveAll();

	CGraphTypeDataDeque::const_iterator i;
	for(i = m_graphTypeDataList.begin(); i != m_graphTypeDataList.end(); ++i){
        CGraphTypeData* pGraphTypeData = *i;
		assert(pGraphTypeData != NULL);
		namesList.AddTail(pGraphTypeData->GetName());
	}
	return (namesList.GetCount() > 0);
}

CStringDataList* CGraphTypeDataList::GetAllStyleList(const CString& strType) const
{
	CGraphTypeData* pGraphTypeData = GetGraphTypeData(strType);
	if(pGraphTypeData == NULL)
		return NULL;

	return pGraphTypeData->GetStyleDataList();
}

// private ====================================================================
bool CGraphTypeDataList::IsExisting(const int nIndex) const
{
	if(m_graphTypeDataList.size() <= 0)
		return false;

	return (nIndex >= 0 && nIndex < m_graphTypeDataList.size());
}


///////////////////////////////////////////////////////////////////////////////
// class CGraphTypeAndStyleData

CGraphTypeAndStyleData::~CGraphTypeAndStyleData()
{
	DeleteAll(ALL);
}


// public =====================================================================
bool CGraphTypeAndStyleData::FileDataBuild( char *&p_szDataString)
{
	DeleteAll(ALL);

	if( !p_szDataString) return false;
	if( !*p_szDataString) return false;

	CGraphTypeOrStyleDataBuilder graphTypeBuilder;
	// graphType 관련 부분
	graphTypeBuilder.FileDataBuild_GraphType( p_szDataString, m_graphTypeList);
	// graphStyle 관련 부분
	graphTypeBuilder.FileDataBuild_GraphStyle( p_szDataString, m_graphStyleList);

	return (m_graphTypeList.GetCount() > 0 && m_graphStyleList.GetCount() > 0);
}

// ----------------------------------------------------------------------------
void CGraphTypeAndStyleData::DeleteAll(const CGraphTypeAndStyleData::DELETETYPE eDeleteType)
{
	switch(eDeleteType){
	case ALL:
		m_graphTypeList.DeleteAll();
		m_graphStyleList.DeleteAll();
		break;
	case TYPE:
		m_graphTypeList.DeleteAll();
		break;
	case STYLE:
		m_graphStyleList.DeleteAll();
		break;
	}
}

// ----------------------------------------------------------------------------
CGraphTypeDataList* CGraphTypeAndStyleData::GetGraphTypeList()
{
	return &m_graphTypeList;
}

CGraphTypeDataList* CGraphTypeAndStyleData::GetStyleTypeList()
{
	return &m_graphStyleList;
}


// ----------------------------------------------------------------------------
bool CGraphTypeAndStyleData::IsEmpty() const
{
	return (m_graphTypeList.IsEmpty() && m_graphStyleList.IsEmpty());
}

// ----------------------------------------------------------------------------
bool CGraphTypeAndStyleData::GetGraphTypeAndStyle(const CString& strType, const CString& strStyle, 
		const CString& strDrawStyleName, const CString& strDrawStyle, 
		int& nType, int& nStyle, int& nDrawStyle) const
{
	if(!GetGraphTypeAndStyle(strType, strStyle, nType, nStyle)){
		return false;
	}

	// <봉형> 인 경우는 style 이 곧 drawStyle 이다!
	if(CGraphBaseData::GRAPHTYPE(nType) == CGraphBaseData::Bong_Type){
		nDrawStyle = nStyle;
		return true;
	}

	// style 이 없는 경우는 <기본값("0")>으로 한다.
	if(strDrawStyleName.IsEmpty() || strDrawStyle.IsEmpty()){
		nDrawStyle = 0;
		return true;
	}

	return GetGraphDrawStyle(strDrawStyleName, strDrawStyle, nDrawStyle);
}

bool CGraphTypeAndStyleData::GetGraphTypeAndStyle(const CString& strType, const CString& strStyle, const CString& strDrawStyle, 
		int& nType, int& nStyle, int& nDrawStyle) const
{
	if(GetGraphTypeAndStyle(strType, strStyle, 
		strType, strDrawStyle, nType, nStyle, nDrawStyle))
		return true;

	nDrawStyle = m_graphStyleList.GetStyleDataIndex(strStyle);
	return (nDrawStyle >= 0);
}

bool CGraphTypeAndStyleData::GetGraphTypeAndStyle(const CString& strType, const CString& strStyle, int& nType, int& nStyle) const
{
	return m_graphTypeList.GetDataIndex(strType, strStyle, nType, nStyle);
}

bool CGraphTypeAndStyleData::GetGraphTypeAndStyle(const int nType, const int nStyle, CString& strType, CString& strStyle) const
{
	return m_graphTypeList.GetData(nType, nStyle, strType, strStyle);
}

bool CGraphTypeAndStyleData::GetGraphDrawStyle(const CString& strName, const CString& strStyle, int& nDrawStyle) const
{
	int nTypeIndex = -1;
	return m_graphStyleList.GetDataIndex(strName, strStyle, nTypeIndex, nDrawStyle);
}

bool CGraphTypeAndStyleData::GetAllGraphTypeList(CStringDataList& graphTypeList) const
{
	return m_graphTypeList.GetAllNamesList(graphTypeList);
}

CStringDataList* CGraphTypeAndStyleData::GetAllGraphStyleList(const CGraphTypeAndStyleData::STYLETYPE eStyleType, const CString& strType) const
{
	if(eStyleType == TYPE_STYLE)
		return m_graphTypeList.GetAllStyleList(strType);
	else if(eStyleType == DRAW_STYLE)
		return m_graphStyleList.GetAllStyleList(strType);
	return NULL;
}
