// GraphMove.h: interface for the CGraphMove class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRAPHMOVE_H__D86E1907_2963_4F33_8B6A_95D5DD3C2D37__INCLUDED_)
#define AFX_GRAPHMOVE_H__D86E1907_2963_4F33_8B6A_95D5DD3C2D37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCommonInterface;
class CMainBlock;
class CGraphImp;
class CMovingGraphDeque;
class CMovingGraphVertScaleData;
class CVertScaleGroupList;
class CVerticalScaleGroupDataList;


class CGraphMove  
{
public:
	CGraphMove(CWnd* pParent);

	void InitializeAllData(CWnd* pParent);

	void SetGraphMoving(const bool bGraphMoving);

	// graph move 실행 유무.
	bool IsGraphMoving() const;
	
	// graph move
	bool GraphMoving(CMainBlock* pMainBlock, CGraphImp* pSourceGraph, const CPoint& targetPoint);

private:
	CWnd* m_pParent;
	bool m_bGraphMoving; // graph를 이동할지의 여부.

	// Graph 분리/삽입 - ojtaso (20070731)
	bool IsGraphInsert(CMainBlock* pMainBlock, CGraphImp* pSourceGraph, const CPoint& targetPoint, const CRect &targetGraphRegion, const int& nInsertCol, const int& nInsertRow) const;
	// graph move 실행 유무.
	bool IsGraphMoving(CMainBlock* pMainBlock, CGraphImp* pSourceGraph, const CPoint& targetPoint, const CRect &targetGraphRegion) const;
	bool IsPriceChartScaleGroup(CGraphImp* pGraph) const;
	void MessageBox_NoGraphMoving(const CString& GraphName) const;

	// block의 vertScale group list.
//	CVertScaleGroupList* GetVertScaleGroupList(CMainBlock* pMainBlock, const CPoint& point) const;
	// block에 있는 vertScale들 가져오기.
	CVerticalScaleGroupDataList GetVertScaleGroupDataList(CGraphImp* pSourceGraph, const CVertScaleGroupList& targetVertScaleGroupList) const;
	// 이동할 graph들 가져오기.
	CMovingGraphDeque GetMovingGraphDeque(CGraphImp* pGraph);
	void AddMovingGraph(CVertScaleGroupList* pVertScaleGroupList, const CString& addGraphName, CMovingGraphDeque& movingGraphDeque);

	// graph 이동을 위한 dlg load.
	bool LoadDialog_GraphMoving(CMainBlock* pMainBlock, CGraphImp* pSourceGraph, const CPoint& targetPoint);
	bool LoadDialog_GraphMoving( CMainBlock* pMainBlock, CGraphImp* pSourceGraph, const CVerticalScaleGroupDataList& vertScaleGroupDataList, const CString& strOCXPath, CMovingGraphVertScaleData& movingVertScaleData);
	// graph move.
	bool GraphMoving(CGraphImp* pSourceGraph, CVertScaleGroupList* pTargetScaleGroupList, const CMovingGraphVertScaleData& movingVertScaleData);
	bool GraphMoving(CVertScaleGroupList* pTargetScaleGroupList, const CMovingGraphVertScaleData& movingVertScaleData, const CMovingGraphDeque& movingGraphDeque);
	void SetVertScaleGridType(CVertScaleGroupList* pVertScaleGroupList);

// (2008/1/22 - Seung-Won, Bae) for Multi-Language
protected:
	HWND				m_hOcxWnd;
};

#endif // !defined(AFX_GRAPHMOVE_H__D86E1907_2963_4F33_8B6A_95D5DD3C2D37__INCLUDED_)
