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

CCoordinate& CCoordinate::operator=(const CCoordinate& point)
{
	if(this == &point)
		return *this;

	m_dX = point.GetX();
	m_dY = point.GetY();
	//m_nIndex = 0;
	m_nIndex = point.GetIndex();
	// 분차트 근접시간 적용 - ojtaso (20080811)
	m_strType = point.GetDateType();
	// 20081007 JS.Kim	사용자시간대 선택. 
	m_nTimeDiff = point.GetTimeDiff();
	return *this;
}

bool CCoordinate::operator==(const CCoordinate& point)
{
	if(this == &point)
		return true;

	return (m_dX == point.GetX() && m_dY == point.GetY() && m_nIndex == point.GetIndex() && m_nTimeDiff == point.GetTimeDiff());
}

bool CCoordinate::operator!=(const CCoordinate& point)
{
	return !(*this == point);
}

// ----------------------------------------------------------------------------
CCoordinate::CCoordinate():
	m_dX(0.0),
	m_dY(0.0),
	m_nIndex(0),
	// 20081007 JS.Kim	사용자시간대 선택. 
	m_nTimeDiff(0)
{
	// 분차트 근접시간 적용 - ojtaso (20080811)
	m_strType.Empty();
}

// 분차트 근접시간 적용 - ojtaso (20080811)
// 20081007 JS.Kim	사용자시간대 선택. 
/*
CCoordinate::CCoordinate(const double& x, const double& y, const CString& strType, const int nIndex):
	m_dX(x),
	m_dY(y),
	m_nIndex(nIndex),
	// 분차트 근접시간 적용 - ojtaso (20080811)
	m_strType(strType),
	m_nTimeDiff( 0)
{
}
*/
// 20081007 JS.Kim	사용자시간대 선택
CCoordinate::CCoordinate(const double& x, const double& y, const int nTimeDiff, const CString& strType, const int nIndex):
	m_dX(x),
	m_dY(y),
	m_nIndex(nIndex),
	m_strType(strType),
	m_nTimeDiff(nTimeDiff)
{
}

CCoordinate::CCoordinate(const CPoint& point):
	m_dX((double)point.x),
	m_dY((double)point.y),
	// 20081007 JS.Kim	사용자시간대 선택. 
	m_nTimeDiff(0)
{
}

CCoordinate::CCoordinate(const CCoordinate& point):
	m_dX(point.GetX()),
	m_dY(point.GetY()),
	m_nIndex(point.GetIndex()),
	// 분차트 근접시간 적용 - ojtaso (20080811)
	m_strType(point.GetDateType()),
	// 20081007 JS.Kim	사용자시간대 선택. 
	m_nTimeDiff(point.GetTimeDiff())
{
}

// public =====================================================================
double CCoordinate::GetX() const
{
	return m_dX;
}

double CCoordinate::GetY() const
{
	return m_dY;
}

int CCoordinate::GetIndex() const
{
	return m_nIndex;
}

CPoint CCoordinate::GetPixel() const
{
	return CPoint((int)m_dX, (int)m_dY);
}

// 분차트 근접시간 적용 - ojtaso (20080811)
CString CCoordinate::GetDateType() const
{
	return m_strType;
}

// 20081007 JS.Kim	사용자시간대 선택. 
int CCoordinate::GetTimeDiff() const
{
	return m_nTimeDiff;
}

void CCoordinate::SetX(const double& x, const int nIndex)
{
	m_dX = x;
	m_nIndex = nIndex;
}

void CCoordinate::SetIndex(int nIndex)
{
	m_nIndex = nIndex;
}

void CCoordinate::SetY(const double& y)
{
	m_dY = y;
}

// 분차트 근접시간 적용 - ojtaso (20080811)
void CCoordinate::SetDateType(CString& strType)
{
	m_strType = strType;
}

// 20081007 JS.Kim	사용자시간대 선택. 
void CCoordinate::SetTimeDiff(int nTimeDiff)
{
	m_nTimeDiff = nTimeDiff;
}

///////////////////////////////////////////////////////////////////////////////
// class CPoints

CPoints& CPoints::operator=(const CPoints& points)
{
	if(this == &points)
		return *this;

	m_savingPoint = points.GetSavingPoint();
	m_drawingPoint = points.GetDrawingPoint();
	return *this;
}

bool CPoints::operator==(const CPoints& points)
{
	if(this == &points)
		return true;

	return (m_savingPoint == points.GetSavingPoint() && m_drawingPoint == points.GetDrawingPoint());
}

bool CPoints::operator!=(const CPoints& points)
{
	return !(*this == points);
}

// ----------------------------------------------------------------------------
CPoints::CPoints():
	m_drawingPoint(0, 0)
{
}

CPoints::CPoints(const CCoordinate& savingPoint, const CPoint& drawingPoint):
	m_savingPoint(savingPoint),
	m_drawingPoint(drawingPoint)
{
}

void CPoints::SetPoints(const CCoordinate& savingPoint, const CPoint& drawingPoint)
{
	m_savingPoint = savingPoint;
	m_drawingPoint = drawingPoint;
}

void CPoints::SetDrawingPoints(const CPoint& drawingPoint)
{
	m_drawingPoint = drawingPoint;
}

void CPoints::SetPoints_Point_Y(const int& nDrawingPointY)
{
	m_drawingPoint.y = nDrawingPointY;
}

void CPoints::SetPoints_Value_Y(const double& dSavingPointY)
{
	m_savingPoint.SetY(dSavingPointY);
}

CCoordinate CPoints::GetSavingPoint() const
{
	return m_savingPoint;
}

CPoint CPoints::GetDrawingPoint() const
{
	return m_drawingPoint;
}

///////////////////////////////////////////////////////////////////////////////
// class CPointsList

CPointsList& CPointsList::operator=(const CPointsList& pointsList)
{
	if(this == &pointsList)
		return *this;
	
	SetPointsList(pointsList);
	return *this;
}

bool CPointsList::operator==(const CPointsList& pointsList)
{
	if(this == &pointsList)
		return true;

	if(m_PointsList.GetCount() != pointsList.GetCount())
		return false;

	for(int nIndex = 0; nIndex < m_PointsList.GetCount(); nIndex++){
		if(m_PointsList.GetAt(nIndex) != pointsList.GetAt(nIndex))
			return false;
	}
	return true;
}

bool CPointsList::operator!=(const CPointsList& pointsList)
{
	return !(*this == pointsList);
}


CPointsList::CPointsList(const CPointsList& pointsList)
{
	SetPointsList(pointsList);
}

CPointsList::CPointsList()
{
}

// public =====================================================================
// list에 points 추가/삭제.
void CPointsList::AddPoints(const CPoints& points)
{
	m_PointsList.AddTail(points);
}

void CPointsList::RemoveAll()
{
	if(m_PointsList.GetCount() <= 0)
		return;

	m_PointsList.RemoveAll();
}

bool CPointsList::RemoveAt(const int nIndex)
{
	if(!HavingIndex(nIndex))
		return false;

	m_PointsList.RemoveAt(nIndex);
	return true;
}

// ----------------------------------------------------------------------------
void CPointsList::SetPointsList(const CPointsList& pointsList)
{
	for(int nIndex = 0; nIndex < pointsList.GetCount(); nIndex++){		
		m_PointsList.AddTail(pointsList.GetAt(nIndex));
	}
}

int CPointsList::GetCount() const
{
	return m_PointsList.GetCount();
}

CPoints CPointsList::GetAt(const int nIndex) const
{
	assert(HavingIndex(nIndex));
	return m_PointsList.GetAt(nIndex);
}

CPoints& CPointsList::GetAt(const int nIndex)
{
	assert(HavingIndex(nIndex));
	return m_PointsList.GetAt(nIndex);
}


// private ====================================================================
bool CPointsList::HavingIndex(const int nIndex) const
{
	return (nIndex >= 0 && nIndex < m_PointsList.GetCount());
}
