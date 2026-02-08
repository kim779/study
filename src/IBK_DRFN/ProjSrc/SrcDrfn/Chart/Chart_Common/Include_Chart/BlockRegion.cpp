// BlockRegion.cpp: implementation of the CBlockRegion class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BlockRegion.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////
// class CGraphPartRegion

CGraphPartRegion& CGraphPartRegion::operator=(const CGraphPartRegion& data)
{
	if(this == &data)
		return *this;

	m_full = data.GetFull();
	m_unmixed = data.GetUnmixed();

	return *this;
}

CGraphPartRegion::CGraphPartRegion() :
	m_full(CRect(0, 0, 0, 0)), 
	m_unmixed(CRect(0, 0, 0, 0))
{
}

CGraphPartRegion::CGraphPartRegion(const CRect& full, const CRect& unmixed) :
	m_full(full),
	m_unmixed(unmixed)
{
}

CGraphPartRegion::CGraphPartRegion(const CGraphPartRegion& data) :
	m_full(data.GetFull()),
	m_unmixed(data.GetUnmixed())
{
}


// public =====================================================================
void CGraphPartRegion::SetFull(const CRect& region)
{
	m_full = region;
}

void CGraphPartRegion::SetUnmixed(const CRect& region)
{
	m_unmixed = region;
}

CRect CGraphPartRegion::GetFull() const
{
	return m_full;
}

CRect CGraphPartRegion::GetUnmixed() const
{
	return m_unmixed;
}

bool CGraphPartRegion::IsDrawing() const
{
	return (!m_full.IsRectEmpty() && !m_unmixed.IsRectEmpty());
}


///////////////////////////////////////////////////////////////////////////////
// class CBlockRegion

CBlockRegion& CBlockRegion::operator=(const CBlockRegion& data)
{
	if(this == &data)
		return *this;

	m_graphPartRegion = data.GetGraphPartRegion();
	m_horzScaleRegion = data.GetHorzScaleRegion();
	m_vertScaleRegion = data.GetVertScaleRegion();

	return *this;
}

CBlockRegion::CBlockRegion()
{
}

CBlockRegion::CBlockRegion(const CGraphPartRegion& graphPartRegion, 
		   const CHorzScaleRegion& horzScale, const CVertScaleRegion& vertScale) :
	m_graphPartRegion(graphPartRegion),
	m_horzScaleRegion(horzScale),
	m_vertScaleRegion(vertScale)
{
}

CBlockRegion::CBlockRegion(const CBlockRegion& data) :
	m_graphPartRegion(data.GetGraphPartRegion()),
	m_horzScaleRegion(data.GetHorzScaleRegion()),
	m_vertScaleRegion(data.GetVertScaleRegion())
{
}


// public =====================================================================
CGraphPartRegion CBlockRegion::GetGraphPartRegion() const
{
	return m_graphPartRegion;
}

CHorzScaleRegion CBlockRegion::GetHorzScaleRegion() const
{
	return m_horzScaleRegion;
}

CVertScaleRegion CBlockRegion::GetVertScaleRegion() const
{
	return m_vertScaleRegion;
}

// ----------------------------------------------------------------------------
CRect CBlockRegion::GetBlockRegion() const
{
	CRect region = m_graphPartRegion.GetFull();
	if(!m_horzScaleRegion.GetTopScaleRegion().IsRectEmpty())
		region.top = m_horzScaleRegion.GetTopScaleRegion().top;
	if(!m_horzScaleRegion.GetBottomScaleRegion().IsRectEmpty())
		region.bottom = m_horzScaleRegion.GetBottomScaleRegion().bottom;
	if(!m_vertScaleRegion.GetLeftScaleRegion().IsRectEmpty())
		region.left = m_vertScaleRegion.GetLeftScaleRegion().left;
	if(!m_vertScaleRegion.GetRightScaleRegion().IsRectEmpty())
		region.right = m_vertScaleRegion.GetRightScaleRegion().right;

	return region;
}

CRect CBlockRegion::GetFullGraphRegion() const
{
	return m_graphPartRegion.GetFull();
}

CRect CBlockRegion::GetUnmixedGraphRegion() const
{
	return m_graphPartRegion.GetUnmixed();
}

CPoint CBlockRegion::GetFullGraphRegion_TopLeft() const
{
	return GetFullGraphRegion().TopLeft();
}

CPoint CBlockRegion::GetFullGraphRegion_TopRight() const
{
	CRect rect = GetFullGraphRegion();
	return CPoint(rect.right, rect.top);
}

CPoint CBlockRegion::GetFullGraphRegion_BottomLeft() const
{
	CRect rect = GetFullGraphRegion();
	return CPoint(rect.left, rect.bottom);
}


///////////////////////////////////////////////////////////////////////////////
// class CBlockRegionList

CBlockRegionList& CBlockRegionList::operator=(const CBlockRegionList& data)
{
	if(this == &data)
		return *this;

	m_nCurrentNumber = data.GetCurrentNumber();
	for(int nIndex = 0; nIndex < data.GetCount(); nIndex++)
	{
		m_regionList.AddTail(data.GetAt(nIndex));
	}

	return *this;
}

CBlockRegionList::CBlockRegionList():
	m_nCurrentNumber(-1)
{
}

CBlockRegionList::CBlockRegionList(const CBlockRegionList& data):
	m_nCurrentNumber(data.GetCurrentNumber())
{
	for(int nIndex = 0; nIndex < data.GetCount(); nIndex++)
	{
		m_regionList.AddTail(data.GetAt(nIndex));
	}
}


// public =====================================================================
void CBlockRegionList::AddTail(const CBlockRegion& blockRegion)
{
	m_regionList.AddTail(blockRegion);
}

void CBlockRegionList::RemoveAll()
{
	m_regionList.RemoveAll();
	m_nCurrentNumber = -1;
}

// ----------------------------------------------------------------------------
void CBlockRegionList::SetCurrentNumber(const int nNumber)
{
	m_nCurrentNumber = nNumber;
}

void CBlockRegionList::SetCurrentNumber(const CPoint& point)
{
	if(m_regionList.GetCount() <= 0)
		return;

	for(int nIndex = 0; nIndex < m_regionList.GetCount(); nIndex++)
	{
		CRect region = m_regionList.GetAt(nIndex).GetFullGraphRegion();
		if(region.PtInRect(point))
		{
			m_nCurrentNumber = nIndex;
			return;
		}
	}
}

// ----------------------------------------------------------------------------
int CBlockRegionList::GetCount() const
{
	return m_regionList.GetCount();
}

CBlockRegion CBlockRegionList::GetHead() const
{
	return GetAt(0);
}

CBlockRegion CBlockRegionList::GetTail() const
{
	return GetAt(m_regionList.GetCount() -1);
}

CBlockRegion CBlockRegionList::GetCurrentAt() const
{
	return GetAt(m_nCurrentNumber);
}

CBlockRegion CBlockRegionList::GetAt(const int nIndex) const
{
	if(nIndex < 0 || nIndex >= GetCount())
		return CBlockRegion();

	return m_regionList.GetAt(nIndex);
}

CRect CBlockRegionList::GetFullGraphRegionSumInList() const
{
	if(m_regionList.GetCount() <= 0)
		return CRect(0, 0, 0, 0);

	if(m_regionList.GetCount() == 1)
		return m_regionList.GetHead().GetFullGraphRegion();

	CPoint topLeft = m_regionList.GetHead().GetFullGraphRegion().TopLeft();
	CPoint bottomRight = m_regionList.GetTail().GetFullGraphRegion().BottomRight();
	return CRect(topLeft, bottomRight);
}

// ----------------------------------------------------------------------------
int CBlockRegionList::GetCurrentNumber() const
{
	return m_nCurrentNumber;
}

int CBlockRegionList::GetNumber(const CPoint& point) const
{
	for(int nIndex = 0; nIndex < GetCount(); nIndex++)
	{
		CBlockRegion blockRegion = m_regionList.GetAt(nIndex);
		if(blockRegion.GetFullGraphRegion().PtInRect(point))
			return nIndex;
	}
	return -1;
}