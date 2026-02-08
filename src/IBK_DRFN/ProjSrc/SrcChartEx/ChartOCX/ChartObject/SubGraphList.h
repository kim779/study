// SubGraphList.h: interface for the CSubGraphList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SUBGRAPHLIST_H__2255E3BA_4922_4512_9B33_39A02570ABCA__INCLUDED_)
#define AFX_SUBGRAPHLIST_H__2255E3BA_4922_4512_9B33_39A02570ABCA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "../Include_Chart/BlockBaseData.h"

class CGraphImp;
class CPacket;
class CSubGraph;
class CGraphColor;
class CPenThickness;
class CSubGraphData;
class CGraphSearchInfo;
class CDrawingGraphData;
class CDisplayDataMinMax;
class CMainBlockBaseData;
class CGraphCalculateData;
class CSubGraphPacketList;
class CSubGraphViewDataList;
class CSelectedSubGraphDeque;
class CVertScaleMaxWidthCalculatorData;
class CVertDisplayDataMinMaxInputData;
class CSubGraphList  
{
public:
	CSubGraphList();
	virtual ~CSubGraphList();

// Search
public:
	// Graph
	CSubGraph *	GetSubGraph( int p_nIndex);
	BOOL		FindGraph( CGraphSearchInfo *p_pGraphSearchInfo) const;








	// subGraph을 생성&삭제
	void MakeSubGraphs( char *p_szSubGraphsData, const char *p_szIndicatorName);
	// 타이틀에 종목명 표시 : 복수종목 - ojtaso (20070322)
	void MakeSubGraphs( char *p_szSubGraphsData, const char *p_szIndicatorName, LPCTSTR lpszItemName);
	void Delete(CSubGraph* pSubGraph);
	void DeleteAll();
	void DeleteAllPackets(const bool bIsOnlyNoTRData = true);
	void ClearAllPackets(const bool bIsOnlyNoTRData = true);
	// subGraphPacketList 초기화
	void Initialize_AllPackets();

	// 부모 주소
	void SetMainBlockBaseData(CMainBlockBaseData* pMainBlockBaseData);
	void SetGraph(CGraphImp* pGraph);

	// 이미 만들어진 SubGraph에 data를 setting.
//	void SetSubGraphs(const CString& strData);

	// subGraph를 보이기 setting.
	bool SetAllHiding(const bool bIsHiding);
	bool SetHiding(const CString& targetTitle, const bool bIsHiding);
	bool SetHiding(const int nIndex, const bool bIsHiding);
	// subGraph data setting.
	bool SetData(const int nIndex, const CString& changeTitle, const CGraphBaseData::GRAPHTYPE& sourceType, 
		const CGraphStyle& sourceStyle, const CGraphDrawStyle& sourceDrawStyle, const CPenThickness& penThickness);
	// subGraph name setting.
	bool SetSubGraphName(const int nIndex, const CString& strSubGraphName);
	// subGraph title setting.
	bool SetTitle(const CString& orgTitle, const CString& changeTitle);
	bool SetTitle(const int nIndex, const CString& changeTitle);
	// subGraph type 변경.
	bool SetType(const int nIndex, const CGraphBaseData::GRAPHTYPE& sourceType, 
		const CGraphStyle& sourceStyle, const CGraphDrawStyle& sourceDrawStyle);
	// subGraph 라인 굵기 변경.
	bool SetAllSubGraphPenThickness(const CPenThickness& penThickness);
	bool SetPenThickness(const int nIndex, const CPenThickness& penThickness);
	// subGraph 색 변경.
	bool SetSubGraphColor(const int nIndex, const CGraphColor& graphColor);

	// 모든 subGraphData 변경.
	bool ChangeAllSubGraphData(const CList<CSubGraphData*, CSubGraphData*>& sourceSubGraphDataList, const bool bIsAllChange = true);
	bool ChangeSubGraphData(const CString& targetSubGraphName, const CSubGraphData& sourceSubGraphData);


	// subGraphs 모두가 숨김인지 체크
	bool AreHidingAllSubGraphs() const;
	// subGraph title name이 모두 empty인지 체크
	bool IsViewableSubGraphTitleEmpty() const;
	// subGraph인지 존재하는지 여부
	bool IsExistenceSubGrape(const CString& subGraphName) const;
	// 해당 graph type이 존재하는지 체크
	bool IsViewableGraphType(const CGraphBaseData::GRAPHTYPE eGraphType) const;
	bool IsViewableGraphType(const CGraphBaseData::GRAPHTYPE eGraphType, const int nSubGraphIndex) const;
	// 해당 graph style가 존재하는지 체크
	bool IsViewableGraphStyle(const CGraphBaseData::GRAPHTYPE eGraphType, const int nGraphStyle) const;
	bool IsViewableGraphStyle(const CGraphBaseData::GRAPHTYPE eGraphType, const int nGraphStyle, const int nSubGraphIndex) const;
	// 해당 graph draw style가 존재하는지 체크
	bool IsViewableGraphDrawStyle(const CGraphBaseData::GRAPHTYPE eGraphType, const int nGraphStyle, const int nGraphDrawStyle) const;
	// 같은 packet이 존재하는지 확인한다.
	bool IsExistenceSamePacketInGraph(CGraphImp* pTargetGraph) const;
	bool IsExistenceSamePacket(CPacket* pPacket) const;


	// 부모 주소
	CGraphImp* GetGraph() const;
	int GetSubGraphCount() const;
	// 모든 subGraphData
	bool GetAllSubGraphDataList(CList<CSubGraphData*, CSubGraphData*>& subGraphDataList) const;
	CSubGraphData* GetSubGraphData(const CString& strSubGraphName) const;
	CSubGraphData* GetSubGraphData(const int nSubGraphIndex) const;
	// 화면에 보이는 subGraph의 갯수
	int GetViewableSubGraphCount() const;
	// subGraphs name.
	CString GetSubGraphNames() const;
	CString GetSubGraphName(const int nIndex = 0) const;
	CString GetViewableSubGraphNames() const;
	// subGraphs title.
	bool GetAllSubGraphTitle(CList<CString, CString>& titleList) const;
	CString GetSubGraphTitle(const int nIndex = 0) const;
	CString GetViewableSubGraphTitles() const;
	// subGraph title이 같은 subGraph index.
	int GetSameTitleSubgraphIndex(const CString& subGraphTitle) const;
	// subGraphs들의 data
	void GetSubGraphsData( CStringList &p_slSubGraphData) const;
	// subGraph type, style
	bool GetSubGraphType(const int nIndex, CGraphBaseData::GRAPHTYPE& eGraphType, CGraphStyle& graphStyle, CGraphDrawStyle& graphDrawStyle) const;
	CGraphBaseData::GRAPHTYPE GetSubGraphType(const int nIndex = 0) const;
	CGraphStyle GetSubGraphStyle(const int nIndex = 0) const;
	CGraphDrawStyle GetSubGraphDrawStyle(const int nIndex = 0) const;
	// graph 색
	bool GetSubGraphsColor(CList<CGraphColor, CGraphColor&>& graphColorList) const;
	CGraphColor* GetGraphColor(const int nSubGraphIndex) const;
	COLORREF GetGraphColor1(const int nSubGraphIndex) const;
	// subGraph title region
	CRect GetSubGraphTitleRegion(CDC* pDC, const CSubGraph* pSubGraph, const CRect& graphRegion, int& preTitleEndPosition) const;
	// 타이틀 여러줄로 표시 : 복수종목 - ojtaso (20070227)
	CRect GetSubGraphTitleRegion(CDC* pDC, const CSubGraph* pSubGraph, const CRect& graphRegion, int& preTitleEndPosition, int& nPreLineCount) const;
	// view data setting
	void GetSubGraphViewDataList(const CString& GraphName, const CString& packetName, CSubGraphViewDataList& subGraphViewDataList) const;
	// packet과 관련된 내용
	bool GetAllSubGraphCalculateData(CGraphCalculateData& graphCalculateData) const;
	bool GetSubGraphPacketList(CList<CPacket*, CPacket*>& packetList) const;
	CSubGraphPacketList* GetSubGraphPacketList(const int nSubGraphIndex = 0) const;
	CPacket* GetHeadPacket() const;
	bool GetHeadPacketAndDataType(CPacket*& pPacket, double& dDataType) const;
	
	// 세로 scale width 구하기.
	int GetVertScaleRegionWidth(CDC* pDC, CVertScaleMaxWidthCalculatorData* pCalData) const;
	// 현재 보이는 화면에서의 min, max값 구한다. ==> y축의 값
	bool GetDisplayDataMinMax(CVertDisplayDataMinMaxInputData* pInputData, CDisplayDataMinMax& displayDataMinMax, BOOL p_bWithFullRange) const;

	// graph 그리기
	// (2009/1/7 - Seung-Won, Bae) Support graph title drawing routine.
	bool DrawSubGraphTitles(CDC* pDC, CDrawingGraphData* pDrawingGraphData, int& graphTitlePosition, int& graphTitleLine, int& nGraphTitleEndLine);
	bool DrawSubGraphs(CDC* pDC, CDrawingGraphData* pDrawingGraphData);

	// subGraph Title을 이용하여 subGraph찾기.
	CSubGraph* FindSubGraphTitle(CDC* pDC, const CRect& allTitleRegion, const CPoint& point, CRect& titleRegion, int& preGraphTitleEndPosition) const;
	// 타이틀 여러줄로 표시 : 복수종목 - ojtaso (20070227)
	CSubGraph* FindSubGraphTitle(CDC* pDC, const CRect& allTitleRegion, const CPoint& point, CRect& titleRegion, int& preGraphTitleEndPosition, int& nPreLineCount) const;
	// 타이틀로 찾기 : 복수종목 - ojtaso (20070322)
	CSubGraph* FindSubGraphTitle(const CString& strSubGraphTitle) const;
	// subGraph 찾기.
	CSubGraph* FindSubGraph(const int nIndex) const;
	CSubGraph* FindSubGraph(const CString& strSubGraphName) const;
	bool FindSubGraph(const CPoint& point, CSelectedSubGraphDeque& selectedSubGraphDeque) const;
//	bool FindSubGraph(const CPoint& point, CString& strSubGraphData, int& nSubGraphIndex, int& nDataIndex) const;

	void ChangeSubGraphTitle(LPCTSTR lpszPreName, LPCTSTR lpszNewName);

	// (2009/5/26 - Seung-Won, Bae) Clear SubGraph Drawing Cahe.
	void ClearCalcDataForDraw( void);

private:
	CMainBlockBaseData* m_pMainBlockBaseData;
	CGraphImp* m_pGraph;
	CTypedPtrList<CObList, CSubGraph*> m_SubGraphList; // CSubGraph들을 list로..

	bool MakeSubGraph( const char *p_szSubGraphData, const char *p_szIndicatorName);
	// 타이틀에 종목명 표시 : 복수종목 - ojtaso (20070322)
	bool MakeSubGraph( const char *p_szSubGraphData, const char *p_szIndicatorName, LPCTSTR lpszItemName);
	// subGraph를 list에 추가&삭제한다.
	void Add(CSubGraph* pSubGraph);
	// subGraph가 list에 하나도 없을 경우 graph 삭제.
	bool DeleteEmptyGraph();
	// 모든 subGraph가 숨김 이면 graph를 삭제한다.
	bool DeleteGraphInAllSubGraphHiding();
	bool DeleteGraph();
	void DeleteSubGraphs(const int nStartIndex = 0);
	void DeleteSubGraph(CSubGraph* pSubGraph);

//	int CopySubGraphs(const CString& strData);
//	bool CopySubGraph(CSubGraph* pSubGraph, const CString& strSubGraphData);
	// CSubGraph에 모든 data를 셋팅한다.
//	bool SetSubGraphData(CSubGraph* pSubGraph, const CString& strSubGraphData);
	// CDrawingGraphData clear
	// 지표에 조건값 서브그래프별 표시 - ojtaso (20071116)
	void ClearDrawingGraphData(CDrawingGraphData* pDrawingGraphData, BOOL bDrawConditionEachTitle);

	// list에 subGraph가 이미 있는지를 찾는다. true = 이미 존재함.
	bool IsSelectedSubGraphInDeque(CSubGraph* pSubGraph, CSelectedSubGraphDeque& selectedSubGraphDeque) const;
};

#endif // !defined(AFX_SUBGRAPHLIST_H__2255E3BA_4922_4512_9B33_39A02570ABCA__INCLUDED_)
