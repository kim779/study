// CoordinateCalculatorData.cpp: implementation of the CCoordinateCalculatorData class.
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CoordinateCalculatorData.h"

#include "DataMath.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////
// class CVertScaleType

CVertScaleType& CVertScaleType::operator=(const CVertScaleType& vertScaleType)
{
	if(this == &vertScaleType)
		return *this;

	m_bIsLog = vertScaleType.IsLog();
	m_bIsInvert = vertScaleType.IsInvert();
	return *this;
}

CVertScaleType::CVertScaleType():
	m_bIsLog(false),
	m_bIsInvert(false)
{
}

CVertScaleType::CVertScaleType(const CVertScaleType& vertScaleType):
	m_bIsLog(vertScaleType.IsLog()),
	m_bIsInvert(vertScaleType.IsInvert())
{
}

CVertScaleType::CVertScaleType(const bool bIsLog, const bool bIsInvert):
	m_bIsLog(bIsLog),
	m_bIsInvert(bIsInvert)
{
}

void CVertScaleType::SetScaleType(const bool bIsLog, const bool bIsInvert)
{
	m_bIsLog = bIsLog;
	m_bIsInvert = bIsInvert;
}

void CVertScaleType::SetScaleLogType(const bool bIsLog)
{
	m_bIsLog = bIsLog;
}

void CVertScaleType::SetScaleInverType(const bool bIsInvert)
{
	m_bIsInvert = bIsInvert;
}

// ----------------------------------------------------------------------------
bool CVertScaleType::IsLog() const
{
	return m_bIsLog;
}

bool CVertScaleType::IsInvert() const
{
	return m_bIsInvert;
}

// ----------------------------------------------------------------------------
double CVertScaleType::ApplyLog(const double& dData) const
{
	if(!m_bIsLog)
		return dData;
	
	return CMath::Log(dData);
}

double CVertScaleType::ApplyExp(const double& dData) const
{
	if(!m_bIsLog)
		return dData;

	return CMath::Exp(dData);
}


///////////////////////////////////////////////////////////////////////////////
// class CDisplayDataVertMinMax

CDisplayDataVertMinMax& CDisplayDataVertMinMax::operator=(const CDisplayDataVertMinMax& data)
{
	if(this == &data)
		return *this;

	m_dMin = data.GetMin();
	m_dMax = data.GetMax();
	m_vertScaleType = data.GetVertScaleType();
	return *this;
}

CDisplayDataVertMinMax::CDisplayDataVertMinMax():
	m_dMin(0.0),
	m_dMax(0.0)
{
}

CDisplayDataVertMinMax::CDisplayDataVertMinMax(const double& dMin, const double& dMax, const CVertScaleType& vertScaleType):
	m_dMin(dMin),
	m_dMax(dMax),
	m_vertScaleType(vertScaleType)
{
}

void CDisplayDataVertMinMax::SetMinMax(const double& dMin, const double& dMax)
{
	m_dMin = dMin;
	m_dMax = dMax;
}

void CDisplayDataVertMinMax::NormalizeMinMax()
{
	if(m_dMax >= m_dMin)
		return;

	double dTemp = m_dMax;
	m_dMax = m_dMin;
	m_dMin = dTemp;
}

// ----------------------------------------------------------------------------
double CDisplayDataVertMinMax::GetMin() const
{
	return m_dMin;
}

double CDisplayDataVertMinMax::GetMax() const
{
	return m_dMax;
}

CVertScaleType CDisplayDataVertMinMax::GetVertScaleType() const
{
	return m_vertScaleType;
}

CVertScaleType& CDisplayDataVertMinMax::GetVertScaleType()
{
	return m_vertScaleType;
}

double CDisplayDataVertMinMax::GetMin_ScaleType() const
{
	double min = m_dMin;
	if(m_vertScaleType.IsInvert())
		min = m_dMax;
	
	return m_vertScaleType.ApplyLog(min);
}

double CDisplayDataVertMinMax::GetMax_ScaleType() const
{
	double max = m_dMax;
	if(m_vertScaleType.IsInvert())
		max = m_dMin;

	return m_vertScaleType.ApplyLog(max);
}


///////////////////////////////////////////////////////////////////////////////
// class CCoordinateCalculatorData

CCoordinateCalculatorData& CCoordinateCalculatorData::operator=(const CCoordinateCalculatorData& data)
{
	if(this == &data)
		return *this;

	m_region = data.GetRegion();
	m_nDisplayDataStart = data.GetDisplayDataStart();
	m_nDisplayDataEnd = data.GetDisplayDataEnd();
	m_displayDataVertMinMax = data.GetDisplayDataVertMinMax();
	m_dVertDataType = data.GetVertDataType();
	m_pPacket_x = data.GetPacket_X();
	m_pPacket_y = data.GetPacket_Y();
	return *this;
}

CCoordinateCalculatorData::CCoordinateCalculatorData():
	m_region(0, 0, 0, 0),
	m_nDisplayDataStart(0),
	m_nDisplayDataEnd(0),
	m_dVertDataType(1.0),
	m_pPacket_x(NULL),
	m_pPacket_y(NULL)
{
}

void CCoordinateCalculatorData::SetRegion(const CRect& region)
{
	m_region = region;
}

void CCoordinateCalculatorData::SetDisplayDataStartEnd(const int start, const int end)
{
	m_nDisplayDataStart = start;
	m_nDisplayDataEnd = end;
}

void CCoordinateCalculatorData::SetVertDataType(const double& dDataType)
{
	m_dVertDataType = dDataType;
}

void CCoordinateCalculatorData::SetPacket_X(CPacket* pDataPacket)
{
	m_pPacket_x = pDataPacket;
}

void CCoordinateCalculatorData::SetPacket_Y(CPacket* pClosePacket)
{
	m_pPacket_y = pClosePacket;
}

// ----------------------------------------------------------------------------
CRect CCoordinateCalculatorData::GetRegion() const
{
	return m_region;
}

int CCoordinateCalculatorData::GetDisplayDataStart() const
{
	return m_nDisplayDataStart;
}

int CCoordinateCalculatorData::GetDisplayDataEnd() const
{
	return m_nDisplayDataEnd;
}

int CCoordinateCalculatorData::GetDisplayDataCount() const
{
	return (m_nDisplayDataEnd - m_nDisplayDataStart +1);
}

double CCoordinateCalculatorData::GetDisplayDataMin_ScaleType() const
{
	return m_displayDataVertMinMax.GetMin_ScaleType();
}

double CCoordinateCalculatorData::GetDisplayDataMax_ScaleType() const
{
	return m_displayDataVertMinMax.GetMax_ScaleType();
}

CDisplayDataVertMinMax CCoordinateCalculatorData::GetDisplayDataVertMinMax() const
{
	return m_displayDataVertMinMax;
}

CDisplayDataVertMinMax& CCoordinateCalculatorData::GetDisplayDataVertMinMax()
{
	return m_displayDataVertMinMax;
}

CVertScaleType CCoordinateCalculatorData::GetVertScaleType() const
{
	return m_displayDataVertMinMax.GetVertScaleType();
}

double CCoordinateCalculatorData::GetVertDataType() const
{
	return m_dVertDataType;
}

CPacket* CCoordinateCalculatorData::GetPacket_X() const
{
	return m_pPacket_x;
}

CPacket* CCoordinateCalculatorData::GetPacket_Y() const
{
	return m_pPacket_y;
}

// ----------------------------------------------------------------------------
bool CCoordinateCalculatorData::DoesCalculator(const CPoint& point) const
{
	if(m_pPacket_x == NULL || GetDisplayDataCount() <= 0)
		return false;
	return (m_region.left <= point.x && m_region.right >= point.x);
}

double CCoordinateCalculatorData::ApplyLog(const double& data) const
{
	return m_displayDataVertMinMax.GetVertScaleType().ApplyLog(data);
}

double CCoordinateCalculatorData::ApplyExp(const double& data) const
{
	return m_displayDataVertMinMax.GetVertScaleType().ApplyExp(data);
}
