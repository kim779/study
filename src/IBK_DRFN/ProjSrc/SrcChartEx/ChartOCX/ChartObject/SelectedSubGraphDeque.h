// SelectedSubGraphDeque.h: interface for the CSelectedSubGraphDeque class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SELECTEDSUBGRAPHDEQUE_H__412B6F99_3DB9_4A7C_B9A1_6592D2D792E9__INCLUDED_)
#define AFX_SELECTEDSUBGRAPHDEQUE_H__412B6F99_3DB9_4A7C_B9A1_6592D2D792E9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Include_Chart/DequeList.h"
#include "../Include_Chart/BlockBaseData.h"

class CVertScaleGroup;
class CGraphImp;
class CSubGraph;

class CSelectedSubGraphData
{
public:
	CSelectedSubGraphData& operator=(const CSelectedSubGraphData& selectedSubGraphData);

public:
	CSelectedSubGraphData(CSubGraph* pSubGraph, const int nDataIndex);
	CSelectedSubGraphData(const CSelectedSubGraphData& selectedSubGraphData);

	// list의 data 가져오기
	CSubGraph*& GetSubGraph();
	CSubGraph* GetSubGraph() const;
	CGraphImp* GetGraph() const;
	CVertScaleGroup* GetVertScaleGroup() const;
	int GetDataIndex() const;

	CMenuType::MENUTYPE GetMenuType() const;
	bool OnMenu(const CMenuType::ELEMENTMENU menuType, const WPARAM wParam);

	bool SetSubGraphColor(const COLORREF& color);
	bool SetSubGraphThickness(const int nThickness);
	bool SetSubGraphHighlight();
	bool SetSubGraphHighlight(const bool bIsHightlight);
	bool SetSubGraphDrawStyle(const CGraphDrawStyle& drawStyle);

	bool AreAllSubGraphHighlighted() const;
	bool AreExistenceSubGraph(const CString& strSubGraphName) const;
	bool DeleteOrHideSubGraph();
	bool DrawGrip(CDC* pDC);

private:
	CSubGraph* m_pSubGraph;
	int m_nDataIndex;

	bool IsDeleteOrHideSubGraph() const;
	bool IsDeleteGraph() const;

	void DeleteGraph();
};

class CSelectedSubGraphDeque
{
public:
	CSelectedSubGraphDeque& operator=(const CSelectedSubGraphDeque& selectedSubGraphDeque);

public:
	void AddTail(const CSelectedSubGraphData& selectedSubGraphData);
	void RemoveAll();

	// list에 있는 data 갯수
	int GetCount() const;
	// list의 data 가져오기
	CSelectedSubGraphData GetAt(const int nIndex) const;
	CSelectedSubGraphData GetTail() const;
	CSubGraph* GetSubGraphTail() const;
	CVertScaleGroup* GetVertScaleGroupTail() const;

	CMenuType::MENUTYPE GetMenuType() const;
	bool OnMenu(const CMenuType::ELEMENTMENU menuType, const WPARAM wParam);

	// 모든 subGraph 가 강조인지의 유무
	bool AreAllSubGraphHighlighted() const;
	bool AreExistenceSubGraph(const CString& strSubGraphName) const;
	bool DrawGrip(CDC* pDC);

private:
	CDequeList<CSelectedSubGraphData> m_selectedSubGraphDeque;

	// subGraph 숨김
	bool DeleteOrHideSubGraph(const int nIndex);

	// subGraph 숨김
	bool OnMenu_SubGraphHide(const bool bIsAction);
	// subGraph color
	bool OnMenu_SubGraphColor(const COLORREF& color);
	// subGraph thickness
	bool OnMenu_SubGraphThickness(const int nThickness);
	// 강조
	bool OnMenu_SubGraphHighlight(const bool bIsHightlight);
	// subGraph draw style	
	bool OnMenu_SubGraphDrawStyle(const CGraphDrawStyle& drawStyle);

	// subGraph 중 GraphType이 있는 경우 = true
	bool AreSubGraphType(const CGraphBaseData::GRAPHTYPE eGraphType) const;
};


// 이동할 graph 들.
class CMovingGraphDeque
{
public:
	CMovingGraphDeque& operator=(const CMovingGraphDeque& movingGraphDeque);

public:
	void AddTail(CGraphImp* pGraph);
	void RemoveAll();

	int GetCount() const;
	CGraphImp* GetAt(const int nIndex) const;
	CGraphImp* GetAt(const int nIndex);

private:
	CDequePointerList<CGraphImp> m_movingGraphDeque;
};

#endif // !defined(AFX_SELECTEDSUBGRAPHDEQUE_H__412B6F99_3DB9_4A7C_B9A1_6592D2D792E9__INCLUDED_)
