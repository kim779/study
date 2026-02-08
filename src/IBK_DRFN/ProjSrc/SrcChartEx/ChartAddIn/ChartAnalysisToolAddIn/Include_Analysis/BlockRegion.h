// BlockRegion.h: interface for the CBlockRegion class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BLOCKREGION_H__53730167_84B6_443B_B7E6_71DEDFA7C77D__INCLUDED_)
#define AFX_BLOCKREGION_H__53730167_84B6_443B_B7E6_71DEDFA7C77D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DequeList.h"
#include "ScaleBaseData.h"

class CGraphPartRegion
{
public:
	CGraphPartRegion& operator=(const CGraphPartRegion& data);

public:
	CGraphPartRegion();
	CGraphPartRegion(const CRect& full, const CRect& unmixed);
	CGraphPartRegion(const CGraphPartRegion& data);

	void SetFull(const CRect& region);
	void SetUnmixed(const CRect& region);

	CRect GetFull() const;
	CRect GetUnmixed() const;

	bool IsDrawing() const;

private:
	CRect m_full; // graph의 frame만 제외한 영역(title, text, margin영역포함)
	CRect m_unmixed; // graph만 그리는 영역(좌표계산을 위한 영역)(title, text, margin영역포함)
};


class CBlockRegion
{
public:
	CBlockRegion& operator=(const CBlockRegion& data);

public:
	CBlockRegion();
	CBlockRegion(const CGraphPartRegion& graphPartRegion, 
		const CHorzScaleRegion& horzScale, const CVertScaleRegion& vertScale);
	CBlockRegion(const CBlockRegion& data);

	CGraphPartRegion GetGraphPartRegion() const;
	CHorzScaleRegion GetHorzScaleRegion() const;
	CVertScaleRegion GetVertScaleRegion() const;

	CRect GetBlockRegion() const;
	CRect GetFullGraphRegion() const;
	CRect GetUnmixedGraphRegion() const;
	CPoint GetFullGraphRegion_TopLeft() const;
	CPoint GetFullGraphRegion_TopRight() const;
	CPoint GetFullGraphRegion_BottomLeft() const;

private:
	//sy test - ok
	CGraphPartRegion m_graphPartRegion;
	CHorzScaleRegion m_horzScaleRegion;
	CVertScaleRegion m_vertScaleRegion;
};

#include "../Include_Analysis/Include_AddIn.h"				// for Ixxxxxx.h
class CBlockRegionList
{
public:
	CBlockRegionList& operator=(const CBlockRegionList& data);

public:
	CBlockRegionList();
	CBlockRegionList(const CBlockRegionList& data);

	void AddTail(const CBlockRegion& blockRegion);
	void RemoveAll();

	void SetCurrentNumber(const int nNumber);
	void SetCurrentNumber(const CPoint& point);

	int GetCount() const;
	CBlockRegion GetHead() const;
	CBlockRegion GetTail() const;
	CBlockRegion GetCurrentAt() const;
	CBlockRegion GetAt(const int nIndex) const;
	CRect GetFullGraphRegionSumInList() const;

	int GetCurrentNumber() const;
	int GetNumber(const CPoint& point) const;

private:
	int m_nCurrentNumber;
	CDequeList<CBlockRegion> m_regionList;

public:
	BOOL	GetBlockRegionList( const CPoint& point, IChartManager* pMainBlock);
	BOOL	GetBlockRegionList( const int nColumn, IChartManager* pMainBlock);
};

#endif // !defined(AFX_BLOCKREGION_H__53730167_84B6_443B_B7E6_71DEDFA7C77D__INCLUDED_)
