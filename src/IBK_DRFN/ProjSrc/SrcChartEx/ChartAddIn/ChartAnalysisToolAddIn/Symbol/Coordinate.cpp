// Coordinate.cpp: implementation of the CCoordinate class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Coordinate.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////
// class CCoordinate

CCoordinate::CCoordinate():
	m_nX(0.00),
	m_nY(0.00),
	m_nTimeDiff(0)
{
}

CCoordinate::CCoordinate(const double& x, const double& y):
	m_nX(x),
	m_nY(y),
	m_nTimeDiff(0)
{
}

// 20081007 JS.Kim	사용자시간대 선택
CCoordinate::CCoordinate(const double& x, const double& y, const int nTimeDiff):
	m_nX(x),
	m_nY(y),
	m_nTimeDiff(nTimeDiff)
{
}

CCoordinate::CCoordinate(const CPoint& point):
	m_nX(point.x),
	m_nY(point.y),
	m_nTimeDiff(0)
{
}


double CCoordinate::GetX() const
{
	return m_nX;
}

double CCoordinate::GetY() const
{
	return m_nY;
}

// 20081007 JS.Kim	사용자시간대 선택. 
int CCoordinate::GetTimeDiff() const
{
	return m_nTimeDiff;
}

void CCoordinate::SetX(const double& x)
{
	m_nX = x;
}

void CCoordinate::SetY(const double& y)
{
	m_nY = y;
}

// 20081007 JS.Kim	사용자시간대 선택. 
void CCoordinate::SetTimeDiff(int nTimeDiff)
{
	m_nTimeDiff = nTimeDiff;
}

CPoint CCoordinate::GetPixel() const
{
	return CPoint((int)m_nX, (int)m_nY);
}
