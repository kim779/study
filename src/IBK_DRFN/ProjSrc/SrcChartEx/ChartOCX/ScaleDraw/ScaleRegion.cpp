// ScaleRegion.cpp: implementation of the CScaleDrawingRegion class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ScaleRegion.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CScaleDrawingRegion::CScaleDrawingRegion():
	m_scaleTextRegion(CRect(0, 0, 0, 0)),
	m_scaleGridRegion(CRect(0, 0, 0, 0))
{
}

CScaleDrawingRegion::CScaleDrawingRegion(const CRect& scaleTextRegion, const CRect& scaleGridRegion):
	m_scaleTextRegion(scaleTextRegion),
	m_scaleGridRegion(scaleGridRegion)
{
}

CRect CScaleDrawingRegion::GetScaleTextRegion() const
{
	return m_scaleTextRegion;
}

CRect CScaleDrawingRegion::GetScaleGridRegion() const
{
	return m_scaleGridRegion;
}

