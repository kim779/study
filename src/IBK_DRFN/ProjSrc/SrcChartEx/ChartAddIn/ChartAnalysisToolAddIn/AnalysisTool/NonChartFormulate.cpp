// NonChartFormulate.cpp: implementation of the CNonChartFormulate class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NonChartFormulate.h"

#include "NonChartImplementation.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNonChartFormulate::CNonChartFormulate()
{
	m_pNonChartImplementation = new CNonChartImplementation();
}

CNonChartFormulate::~CNonChartFormulate()
{
	delete m_pNonChartImplementation;
}

bool CNonChartFormulate::GetAutoTrendData(const CCalculateBaseData::CALCULATETYPE eCalType, const CString& strPacketName, IPacketManager* pPacketList,
					  CList<IPacket*, IPacket*>* pCalculateForPacketList) const
{
	return m_pNonChartImplementation->GetAutoTrendData(eCalType, 
		strPacketName, pPacketList, pCalculateForPacketList);
}
