// IndicatorGroupData.cpp: implementation of the CIndicatorGroupData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../Include_Chart/Dll_Load/IndicatorGroupData.h"

#include <assert.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// class CIndicatorGroupData

// 생성&삭제
CIndicatorGroupData* CIndicatorGroupData::Make(const CString& strGroupName)
{
	return new CIndicatorGroupData(strGroupName);
}

void CIndicatorGroupData::Delete(CIndicatorGroupData* pData)
{
	delete pData;
}

// ---------------------------------------------------------------------------------
CIndicatorGroupData& CIndicatorGroupData::operator=(const CIndicatorGroupData& data)
{
	if(this == &data)
		return *this;

	m_strGroupName = data.GetGroupName();
	CopySubGraphDataList(*data.GetIndicatorNameList());
	return *this;
}

CIndicatorGroupData::CIndicatorGroupData(const CString& strGroupName) :
	m_strGroupName(strGroupName)
{
}

CIndicatorGroupData::~CIndicatorGroupData()
{
}

// public ==========================================================================
void CIndicatorGroupData::Initialize()
{
	m_strGroupName.Empty();
	RemoveAll();
}

void CIndicatorGroupData::RemoveAll()
{
	m_strIndicatorNameList.RemoveAll();
}

void CIndicatorGroupData::AddTail(const CString& strIndicatorName)
{
	m_strIndicatorNameList.AddTail(strIndicatorName);
}

// ---------------------------------------------------------------------------------
void CIndicatorGroupData::SetGroupName(const CString& strGroupName)
{
	m_strGroupName = strGroupName;
}

// ---------------------------------------------------------------------------------
CString CIndicatorGroupData::GetGroupName() const
{
	return m_strGroupName;
}

int CIndicatorGroupData::GetIndicatorNameCount() const
{
	return m_strIndicatorNameList.GetCount();
}

const CList<CString, CString>* CIndicatorGroupData::GetIndicatorNameList() const
{
	return &m_strIndicatorNameList;
}

CList<CString, CString>* CIndicatorGroupData::GetIndicatorNameList()
{
	return &m_strIndicatorNameList;
}


// private =========================================================================
void CIndicatorGroupData::CopySubGraphDataList(const CList<CString, CString>& strSourceIndiNameList)
{
	m_strIndicatorNameList.RemoveAll();

	POSITION pos = strSourceIndiNameList.GetHeadPosition();
	while(pos != NULL)
	{
		CString strIndicatorName = strSourceIndiNameList.GetNext(pos);
		m_strIndicatorNameList.AddTail(strIndicatorName);
	}
}


////////////////////////////////////////////////////////////////////////////////////
// class CIndicatorGroupDataList

CIndicatorGroupDataList::~CIndicatorGroupDataList()
{
	DeleteAll();
}


// public ==========================================================================
void CIndicatorGroupDataList::DeleteAll()
{
	POSITION pos = m_indicatorGroupDataList.GetHeadPosition();
	while(pos != NULL)
	{
		CIndicatorGroupData* pIndiGroupData = m_indicatorGroupDataList.GetNext(pos);
		assert(pIndiGroupData != NULL);
		CIndicatorGroupData::Delete(pIndiGroupData);
	}

	m_indicatorGroupDataList.RemoveAll();
}

void CIndicatorGroupDataList::AddTail(CIndicatorGroupData* pIndiGroupData)
{
	assert(pIndiGroupData != NULL);
	m_indicatorGroupDataList.AddTail(pIndiGroupData);
}

// 해당 CIndicatorGroupData 에 IndicatorName을 adding.
// 단, 해당 GroupData 가 없을 경우엔 group를 만들어 adding. (bIsAutoMake == true)
bool CIndicatorGroupDataList::AddTail_IndiName(const CString& strGroupName, const CString& strIndiName, const bool bIsAutoMake)
{
	CIndicatorGroupData* pIndiGroupData = FindIndicatorGroupData(strGroupName);

	// 해당 group가 없을 경우 새로운 group를 만든다.
	if(pIndiGroupData == NULL && bIsAutoMake)
	{
		pIndiGroupData = CIndicatorGroupData::Make(strGroupName);
		AddTail(pIndiGroupData);
	}

	if(pIndiGroupData == NULL)
		return false;

	pIndiGroupData->AddTail(strIndiName);
	return true;
}

// ---------------------------------------------------------------------------------
int CIndicatorGroupDataList::GetCount() const
{
	return m_indicatorGroupDataList.GetCount();
}

POSITION CIndicatorGroupDataList::GetHeadPosition() const
{
	return m_indicatorGroupDataList.GetHeadPosition();
}

POSITION CIndicatorGroupDataList::GetHeadPosition()
{
	return m_indicatorGroupDataList.GetHeadPosition();
}

POSITION CIndicatorGroupDataList::GetTailPosition() const
{
	return m_indicatorGroupDataList.GetTailPosition();
}

POSITION CIndicatorGroupDataList::GetTailPosition()
{
	return m_indicatorGroupDataList.GetTailPosition();
}

const CIndicatorGroupData* CIndicatorGroupDataList::GetPrev(POSITION& pos) const
{
	if(pos == NULL)
		return NULL;

	return m_indicatorGroupDataList.GetPrev(pos);
}

CIndicatorGroupData* CIndicatorGroupDataList::GetPrev(POSITION& pos)
{
	if(pos == NULL)
		return NULL;

	return m_indicatorGroupDataList.GetPrev(pos);
}

const CIndicatorGroupData* CIndicatorGroupDataList::GetNext(POSITION& pos) const
{
	if(pos == NULL)
		return NULL;

	return m_indicatorGroupDataList.GetNext(pos);
}

CIndicatorGroupData* CIndicatorGroupDataList::GetNext(POSITION& pos)
{
	if(pos == NULL)
		return NULL;

	return m_indicatorGroupDataList.GetNext(pos);
}

// ---------------------------------------------------------------------------------
const CList<CIndicatorGroupData*, CIndicatorGroupData*>* CIndicatorGroupDataList::GetIndicatorGroupDataList() const
{
	return &m_indicatorGroupDataList;
}

CList<CIndicatorGroupData*, CIndicatorGroupData*>* CIndicatorGroupDataList::GetIndicatorGroupDataList()
{
	return &m_indicatorGroupDataList;
}

// ---------------------------------------------------------------------------------
// 해당 groupName인 CIndicatorGroupData 를 찾는다.
CIndicatorGroupData* CIndicatorGroupDataList::FindIndicatorGroupData(const CString& strIndiGroupName)
{
	POSITION pos = m_indicatorGroupDataList.GetHeadPosition();
	while(pos != NULL)
	{
		CIndicatorGroupData* pIndiGroupData = m_indicatorGroupDataList.GetNext(pos);
		assert(pIndiGroupData != NULL);
		if(pIndiGroupData->GetGroupName() == strIndiGroupName)
			return pIndiGroupData;
	}

	return NULL;
}
