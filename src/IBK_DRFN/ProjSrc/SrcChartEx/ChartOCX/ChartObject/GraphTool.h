// GraphTool.h: interface for the CGraphTool class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRAPHTOOL_H__D219A3D9_B1ED_41D4_BC93_1EF641BE86B7__INCLUDED_)
#define AFX_GRAPHTOOL_H__D219A3D9_B1ED_41D4_BC93_1EF641BE86B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Include_Chart/BaseData.h"	// 20081007 JS.Kim	사용자시간대 선택

#include "Tool.h"
#include "GraphMove.h"
#include "SelectedSubGraphDeque.h"

class CBlockImp;
interface IChartCtrl;

class CGraphTool : public CBaseTool
{
public:
	CGraphTool();

	void SetIChartCtrl(IChartCtrl* p_pICharetCtrl);
	void SetGraphMove(const bool bIsGraphMove);
	void SetGraphDelete(const bool bIsGraphDelete);
	// 블럭 최대화 추가 - ojtaso (20080519)
	void SetGraphSize(const bool bIsGraphSize);

	// Tool의 모든 내용을 초기화
	virtual void InitializeAllData();
	virtual bool OnLButtonDown(CDC* pDC, CMainBlock* pMainBlock, const UINT nFlags, const CPoint& point);
	virtual bool OnLButtonUp(CDC* pDC, CMainBlock* pMainBlock, const CPoint& point);
	virtual void OnMouseMove(CDC* pDC, CMainBlock* pMainBlock, const UINT nFlags, const CPoint& point);
	virtual bool OnMenu(CMainBlock* pMainBlock, const CMenuType::ELEMENTMENU menuType, const WPARAM wParam);
	// 지표 뒤집기 - ojtaso (20071023)
	virtual bool GetMenuCheck(CMainBlock* pMainBlock, const CMenuType::ELEMENTMENU menuType, const WPARAM wParam);

	// 지표 뒤집기 - ojtaso (20071023)
	virtual bool OnMenu_SubGraphInvert(CMainBlock* pMainBlock, CSubGraph* pSubGraph);
	// 지표 뒤집기 - ojtaso (20071023)
	virtual CGraphImp* IsGeneralGraph(CGraphImp* pGraph, CVertScaleGroup* pGroup);
	// 지표 뒤집기 - ojtaso (20071023)
	virtual bool GetMenuCheck_SubGraphInvert(CMainBlock* pMainBlock, CSubGraph* pSubGraph);

	// 20081007 JS.Kim	사용자시간대 선택
	virtual bool OnMenu_SubGraphSelectTimeDiff(CMainBlock* pMainBlock, CSubGraph* pSubGraph, CTimeDiffInfo* pTimeInfo);
	virtual bool GetMenuCheck_SubGraphSelectTimeDiff(CMainBlock* pMainBlock, CSubGraph* pSubGraph, CTimeDiffInfo* pTimeInfo);

protected:
	virtual void OnLButtonDown_Real(CDC* pDC, CMainBlock* pMainBlock, const UINT nFlags, const CPoint& point) = 0;
	virtual bool OnLButtonUp() = 0;
	virtual void OnMouseMove(CDC* pDC, CMainBlock* pMainBlock) = 0;
	virtual bool OnMenu_SubGraph(CMainBlock* pMainBlock, const CMenuType::ELEMENTMENU menuType, const WPARAM wParam) = 0;
	// 지표 뒤집기 - ojtaso (20071023)
	virtual bool GetMenuCheck_SubGraph(CMainBlock* pMainBlock, const CMenuType::ELEMENTMENU menuType, const WPARAM wParam) = 0;
	virtual bool IsSelectedSubGraph() const = 0;
	virtual bool IsSendMessage_SubGraph() const = 0;
	virtual CGraphImp* GetGraphMoving() = 0;
	virtual void GraphMoving_Ended(const bool bGraphMovingResult) = 0;

	// graph 가져오기.
	CGraphImp* GetGraph(CSubGraph* pSubGraph) const;
	
	// <지표설정> 에 맞게 값을 적용시킨다. - graph 수정 및 삭제.
	bool LoadIndicatorConfiguration(CMainBlock* pMainBlock, CSubGraph* pSubGraph);
	// data index를 ocx에 주기
	bool OnMenu_DataIndex(const CSelectedSubGraphData& selectedSubGraphData);
	// 세로 scale 바꾸기
	void ChangeAllVertScaleGridType(CVertScaleGroup* pVertScaleGroup);

	// ocx에 메시지로 보낸다.
	void SendMessage_GraphType(CSubGraph* pSubGraph);

private:
	bool IsCursorType_NoneMode(const CPoint& point) const;

	// block 가져오기.
	CBlockImp* GetBlock(CGraphImp* pGraph) const;
	// cursor type 가져오기.
	CBaseToolEnumData::CURSORTYPE GetCursorType(CMainBlock* pMainBlock, const CPoint& point) const;

	// graph move
	void SetGraphMoving(const bool bIsGraphMoving);
	bool GraphMoving(CMainBlock* pMainBlock, const CPoint& targetPoint);

private:
	bool m_bIsGraphMove;
	bool m_bIsGraphDelete;
	CPoint m_lBtDownPt;
	CGraphMove m_graphMove;
};

// subGraph title 을 찾는 Tool
class CSubGraphTitleTool : public CGraphTool
{
public:
	CSubGraphTitleTool();

	// Tool의 모든 내용을 초기화
	virtual void InitializeAllData();
	virtual CBlockBaseData::TOOLTYPE GetToolType() const;

	// mouse
	virtual bool OnLButtonDblClk(CDC* pDC, CMainBlock* pMainBlock, const CPoint& point);
	virtual CMenuType::MENUTYPE OnRButtonUp(CDC* pDC, CMainBlock* pMainBlock, const CPoint& point);
	virtual void OnDraw(CDC* pDC, CMainBlock* pMainBlock);

	// 해당 graph을 찾는다.
	bool Find(CDC* pDC, CMainBlock* pMainBlock, const CPoint& point);
	// <지표설정> 에 맞게 값을 적용시킨다. - graph 수정 및 삭제.
	bool LoadIndicatorConfiguration(CMainBlock* pMainBlock);

protected:
	virtual void OnLButtonDown_Real(CDC* pDC, CMainBlock* pMainBlock, const UINT nFlags, const CPoint& point);
	virtual bool OnLButtonUp();
	virtual void OnMouseMove(CDC* pDC, CMainBlock* pMainBlock);
	virtual bool OnMenu_SubGraph(CMainBlock* pMainBlock, const CMenuType::ELEMENTMENU menuType, const WPARAM wParam);
	// 지표 뒤집기 - ojtaso (20071023)
	virtual bool GetMenuCheck_SubGraph(CMainBlock* pMainBlock, const CMenuType::ELEMENTMENU menuType, const WPARAM wParam);
	virtual bool IsSelectedSubGraph() const;
	virtual bool IsSendMessage_SubGraph() const;
	virtual CGraphImp* GetGraphMoving();
	virtual void GraphMoving_Ended(const bool bGraphMovingResult);

private:
	bool FindSubGraphTitle(CDC* pDC, CMainBlock* pMainBlock, const CPoint& point, CSubGraph*& pSubGraph);

	// 현재 선택된 subGraph의 vert scale을 보이게 한다.
	void ChangeAllVertScaleGridType();

	// subgraph title가 선택된 경우
	void DrawSelectedSubGraphTitle(CDC* pDC);
	bool DrawSelectedSubGraphGrip(CDC* pDC);

	bool IsLoadIndicatorConfiguration(const CPoint& point) const;
	CSubGraph* GetSubGraph() const;
	// subgraph title의 선택 영역
	CRect GetSelectedSubGraphTitleRegion(const CRect& subGraphTitleRegion) const;
	
	void InitializeTitleRegion();

	// OnMenu
	bool OnMenu_SubGraphHide(const bool bIsAction);
	bool OnMenu_SubGraphSet(CMainBlock* pMainBlock);

private:
	CSelectedSubGraphData m_selectedSubGraphData;
	CRect m_selectedSubGraphTitleRegion;
	CRect m_oldSelectedSubGraphTitleRegion;
};


class CSubGraphTool : public CGraphTool
{
public:
	// Tool의 모든 내용을 초기화
	virtual void InitializeAllData();
	virtual CBlockBaseData::TOOLTYPE GetToolType() const;

	// mouse
	virtual bool OnLButtonDblClk(CDC* pDC, CMainBlock* pMainBlock, const CPoint& point);
	virtual CMenuType::MENUTYPE OnRButtonUp(CDC* pDC, CMainBlock* pMainBlock, const CPoint& point);
	virtual void OnDraw(CDC* pDC, CMainBlock* pMainBlock);

	// 해당 graph을 찾는다.
	bool Find(CDC* pDC, CMainBlock* pMainBlock, const CPoint& point);

protected:
	virtual void OnLButtonDown_Real(CDC* pDC, CMainBlock* pMainBlock, const UINT nFlags, const CPoint& point);
	virtual bool OnLButtonUp();
	virtual void OnMouseMove(CDC* pDC, CMainBlock* pMainBlock);
	virtual bool OnMenu_SubGraph(CMainBlock* pMainBlock, const CMenuType::ELEMENTMENU menuType, const WPARAM wParam);
	// 지표 뒤집기 - ojtaso (20071023)
	virtual bool GetMenuCheck_SubGraph(CMainBlock* pMainBlock, const CMenuType::ELEMENTMENU menuType, const WPARAM wParam);
	virtual bool IsSelectedSubGraph() const;
	virtual bool IsSendMessage_SubGraph() const;
	virtual CGraphImp* GetGraphMoving();
	virtual void GraphMoving_Ended(const bool bGraphMovingResult);

private:
	// subGraph 찾기.
	void FindSubGraph(CMainBlock* pMainBlock, const UINT nFlags, const CPoint& point);
	bool FindSubGraph(CMainBlock* pMainBlock, const CPoint& point, CSelectedSubGraphDeque& selectedSubGraphDeque);

//	void SetSelectedCursor(const bool bIsSelectedCursor);

	// 현재 선택된 subGraph의 vert scale을 보이게 한다.
	void ChangeAllVertScaleGridType();

	// subGraph 강조
	bool ReversedSubGraphHighlight(CMainBlock* pMainBlock, const CPoint& point);
	// data index를 ocx에 주기
	bool SelectedGraphDataIndex();
	// OnMenu
	bool OnMenu_SubGraphSet(CMainBlock* pMainBlock);
	bool OnMenu_Etc(const CMenuType::ELEMENTMENU menuType, const WPARAM wParam);

private:
	CSelectedSubGraphDeque m_selectedSubGraphDeque;
};

#endif // !defined(AFX_GRAPHTOOL_H__D219A3D9_B1ED_41D4_BC93_1EF641BE86B7__INCLUDED_)
