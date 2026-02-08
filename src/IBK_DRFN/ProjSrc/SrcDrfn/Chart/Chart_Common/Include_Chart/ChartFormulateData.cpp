// ChartFormulateData.cpp: implementation of the CChartFormulateData class.
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartFormulateData.h"

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////
// class CChartFormulateData

CChartFormulateData& CChartFormulateData::operator=(const CChartFormulateData& data)
{
	if(this == &data)
		return *this;

	m_eCalType = data.GetCalculateType();
	m_strFunctionName = data.GetFunctionName();
	SetDataInEndComma(m_strPacketNames, data.GetPacketNames());
	m_strSubGraphNames = data.GetSubGraphNames();
	m_strViewableSubGraphNames = data.GetViewableSubGraphNames();
	m_nDiaplayDataStartIndex = data.GetDisplayDataStartIndex();
	m_nDiaplayDataEndIndex = data.GetDisplayDataEndIndex();
	m_pPacketList = data.GetPacketList();
	m_pIndicatorList = data.GetIndicatorList();
	return *this;
}

CChartFormulateData::CChartFormulateData(const CCalculateBaseData::CALCULATETYPE eCalType):
	m_eCalType(eCalType),
	m_nDiaplayDataStartIndex(0),
	m_nDiaplayDataEndIndex(0),
	m_pPacketList(NULL),
	m_pIndicatorList(NULL)
{
}

CChartFormulateData::CChartFormulateData(CPacketList* pPacketList, CIndicatorList* pIndicatorList):
	m_eCalType(CCalculateBaseData::TRDATA_CAL),
	m_pPacketList(pPacketList),
	m_pIndicatorList(pIndicatorList)
{
}

CChartFormulateData::CChartFormulateData(const CCalculateBaseData::CALCULATETYPE eCalType, const CString& strFunctionName, 
		const CString& strPacketNames, const CString& strSubGraphNames, const CString& strViewableSubGraphNames, 
		const int nDisplayDataStartIndex, const int nDisplayDataEndIndex, CPacketList* pPacketList, CIndicatorList* pIndicatorList):
	m_eCalType(eCalType),
	m_strFunctionName(strFunctionName),
	m_strSubGraphNames(strSubGraphNames),
	m_strViewableSubGraphNames(strViewableSubGraphNames),
	m_nDiaplayDataStartIndex(nDisplayDataStartIndex),
	m_nDiaplayDataEndIndex(nDisplayDataEndIndex),
	m_pPacketList(pPacketList),
	m_pIndicatorList(pIndicatorList)
{
	SetDataInEndComma(m_strPacketNames, strPacketNames);
}

// public =====================================================================
void CChartFormulateData::SetCalculateType(const CCalculateBaseData::CALCULATETYPE eCalType)
{
	m_eCalType = eCalType;
}

void CChartFormulateData::SetFunctionName(const CString& strFunctionName)
{
	m_strFunctionName = strFunctionName;
}

void CChartFormulateData::SetPacketNames(const CString& strPacketNames)
{
	m_strPacketNames = strPacketNames;
}

void CChartFormulateData::SetSubGraphNames(const CString& strSubGraphNames)
{
	m_strSubGraphNames = strSubGraphNames;
}

void CChartFormulateData::SetViewableSubGraphNames(const CString& strViewableSubGraphNames)
{
	m_strViewableSubGraphNames = strViewableSubGraphNames;
}

void CChartFormulateData::SetDisplayDataStartIndex(const int nDisplayDataStartIndex)
{
	m_nDiaplayDataStartIndex = nDisplayDataStartIndex;
}

void CChartFormulateData::SetDisplayDataEndIndex(const int nDisplayDataEndIndex)
{
	m_nDiaplayDataEndIndex = nDisplayDataEndIndex;
}

void CChartFormulateData::SetPacketList(CPacketList* pPacketList)
{
	m_pPacketList = pPacketList;
}

void CChartFormulateData::SetIndicatorList(CIndicatorList* pIndicatorList)
{
	m_pIndicatorList = pIndicatorList;
}

// ----------------------------------------------------------------------------
CString CChartFormulateData::GetFunctionName() const
{
	return m_strFunctionName;
}

CString CChartFormulateData::GetPacketNames() const
{
	return m_strPacketNames;
}

CString CChartFormulateData::GetSubGraphNames() const
{
	return m_strSubGraphNames;
}

CString CChartFormulateData::GetViewableSubGraphNames() const
{
	return m_strViewableSubGraphNames;
}

int CChartFormulateData::GetDisplayDataStartIndex() const
{
	return m_nDiaplayDataStartIndex;
}

int CChartFormulateData::GetDisplayDataEndIndex() const
{
	return m_nDiaplayDataEndIndex;
}

CPacketList* CChartFormulateData::GetPacketList() const
{
	return m_pPacketList;
}

CIndicatorList* CChartFormulateData::GetIndicatorList() const
{
	return m_pIndicatorList;
}

// ----------------------------------------------------------------------------
CCalculateBaseData::CALCULATETYPE CChartFormulateData::GetCalculateType() const
{
	return m_eCalType;
}

bool CChartFormulateData::DoesProcessStarting() const
{
	if(m_pPacketList == NULL || m_pIndicatorList == NULL)
		return false;

	return (m_nDiaplayDataStartIndex >= 0 && m_nDiaplayDataEndIndex >= 0);
}


// private ====================================================================
void CChartFormulateData::SetDataInEndComma(CString& strOrgData, const CString& strNewData)
{
	if(!strNewData.IsEmpty() && strNewData.Right(1) != ',')
		strOrgData = strNewData + ',';
	else
		strOrgData = strNewData;
}
