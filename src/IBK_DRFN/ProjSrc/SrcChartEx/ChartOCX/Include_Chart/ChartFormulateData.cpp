// ChartFormulateData.cpp: implementation of the CChartFormulateData class.
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartFormulateData.h"

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////
// class CChartFormulateData

CChartFormulateData::CChartFormulateData(const CCalculateBaseData::CALCULATETYPE eCalType):
	m_eCalType(eCalType),
	m_nDiaplayDataStartIndex(0),
	m_nDiaplayDataEndIndex(0),
	m_pPacketList(NULL),
	m_pIndicatorList(NULL)
{
	// (2009/9/8 - Seung-Won, Bae) Chart Running Mode for the brick size of Renko on HTS.
	m_eChartMode = CChartInfo::HTS;
}

CChartFormulateData& CChartFormulateData::operator=(const CChartFormulateData& data)
{
	if(this == &data)
		return *this;

	m_eCalType					= data.GetCalculateType();
	m_strGraphName				= data.GetGraphName();
	m_strIndicatorName			= data.m_strIndicatorName;			// (2006/12/5 - Seung-Won, Bae) Support Indicator Name
	SetDataInEndComma( m_strPacketNames, data.GetPacketNames());
	m_strSubGraphNames			= data.GetSubGraphNames();
	m_strViewableSubGraphNames	= data.GetViewableSubGraphNames();
	m_nDiaplayDataStartIndex	= data.GetDisplayDataStartIndex();
	m_nDiaplayDataEndIndex		= data.GetDisplayDataEndIndex();
	m_pPacketList				= data.GetPacketList();
	m_pIndicatorList			= data.GetIndicatorList();
	// (2009/9/8 - Seung-Won, Bae) Chart Running Mode for the brick size of Renko on HTS.
	m_eChartMode				= data.GetChartMode();
	return *this;
}

// private ====================================================================
void CChartFormulateData::SetDataInEndComma(CString& strOrgData, const CString& strNewData)
{
	if(!strNewData.IsEmpty() && strNewData.Right(1) != ',')
		strOrgData = strNewData + ',';
	else
		strOrgData = strNewData;
}
