// CalculateData.cpp: implementation of the CCalculateData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CalculateData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCalculateData::CCalculateData(const CString& m_PacketNameForCal, CIndicatorInfo* pIndicatorInfo):
	m_pIndicatorInfo(pIndicatorInfo)
{
}

CCalculateData::~CCalculateData()
{
}


// private ====================================================================
CString CCalculateData::GetPacketNamesForSave() const
{
	return m_PacketNamesForSave;
}

CIndicatorInfo* CCalculateData::GetIndicatorInfo()
{
	return m_pIndicatorInfo;
}

CPoint CCalculateData::GetPoint() const
{
	return m_point;
}
// ----------------------------------------------------------------------------
void CCalculateData::SetPacketNamesForSave(const CString& strData)
{
	m_PacketNamesForSave = strData;
}

void CCalculateData::SetIndicatorInfo(CIndicatorInfo *pIndicatorInfo)
{
	m_pIndicatorInfo = pIndicatorInfo;
}

void CCalculateData::SetPoint(const CPoint& point)
{
	m_point = point;
}
