// VertScaleGroup.h: interface for the CVertScaleGroup class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VERTSCALEGROUP_H__D483D8C3_9A0B_4A61_AEE3_C8B9F50F1E8B__INCLUDED_)
#define AFX_VERTSCALEGROUP_H__D483D8C3_9A0B_4A61_AEE3_C8B9F50F1E8B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define _TEST
#ifdef _TEST
class CGraphImp;
class CSubGraph;
class CBlockImp;
class CGraphSearchInfo;
class CMainBlockBaseData;
class CMainChartFormulate;
class CVertScaleGroupList;
class CSubGraphViewDataList;
class CSelectedSubGraphDeque;

#include <afxtempl.h>
#include "../Include_Chart/BaseData.h"
#endif

class CVertScaleGroup : public CObject 
{
#ifdef _TEST
public:
	CVertScaleGroup(CMainBlockBaseData* pMainBlockBaseData, CVertScaleGroupList* pVertScaleGroupList);	// (2006/11/26 - Seung-Won, Bae) Support Empty VScaleGroup
	CVertScaleGroup(CMainBlockBaseData* pMainBlockBaseData, CVertScaleGroupList* pVertScaleGroupList, char *p_szVertScaleGroup);
	// RQ로 검색 : 복수종목 - ojtaso (20070118)
	CVertScaleGroup(LPVOID pPacketRQ, CMainBlockBaseData* pMainBlockBaseData, CVertScaleGroupList* pVertScaleGroupList, char *p_szVertScaleGroup);
	CVertScaleGroup(CMainBlockBaseData* pMainBlockBaseData, CVertScaleGroupList* pVertScaleGroupList, const CVerticalScale& vertScale);

// (2007/2/16 - Seung-Won, Bae) Protect the redestructing on Destructor.
protected:
	virtual ~CVertScaleGroup();
	BOOL	m_bOnDestructor;

// Delete
public:
	// Graph
	BOOL	DeleteAllIndicator( const char *p_szIndicatorName);

// Search
public:
	// Graph
	BOOL	FindGraph( CGraphSearchInfo *p_pGraphSearchInfo) const;
	// (2008/9/15 - Seung-Won, Bae) RQ Search
	BOOL	FindGraph( LPVOID pPacketRQ, CGraphSearchInfo *p_pGraphSearchInfo) const;

	// Sub Graph
	CSubGraph *	GetSubGraph( const int nGraphIndex, const int nSubGraphIndex) const;

	// (2007/2/20 - Seung-Won, Bae) Support Indicator Hiding
	//		It can not be replaced with AddChartBlock(), because of Condition Reset.
	void	ShowIndicator( const char *p_szIndicatorName, BOOL p_bShow, BOOL p_bRecalculate);

public:
	static CVertScaleGroup* MakeVertScaleGroup(CMainBlockBaseData* pMainBlockBaseData, CVertScaleGroupList* pVertScaleGroupList); // (2006/11/26 - Seung-Won, Bae) Support Empty VScaleGroup
	static CVertScaleGroup* MakeVertScaleGroup(CMainBlockBaseData* pMainBlockBaseData, CVertScaleGroupList* pVertScaleGroupList, char *p_szVertScaleGroup);
	// RQ로 검색 : 복수종목 - ojtaso (20070118)
	static CVertScaleGroup* MakeVertScaleGroup(LPVOID pPacketRQ, CMainBlockBaseData* pMainBlockBaseData, CVertScaleGroupList* pVertScaleGroupList, char *p_szVertScaleGroup);
	static CVertScaleGroup* MakeVertScaleGroup(CMainBlockBaseData* pMainBlockBaseData, CVertScaleGroupList* pVertScaleGroupList, const CVerticalScale& vertScale);
	static void Delete(CVertScaleGroup* pVertScaleGroup);
	CGraphImp* FindBaseGraphForDrawingVertScaleData() const;

private:
	// CGraphImp를 만든다.
	CGraphImp* MakeNewGraph( char *p_szGraphData);
	// RQ를 가지고 있는 Graph 생성 : 복수종목 - ojtaso (20061224)
	// CPacketRQ*로 변경 : 복수종목 - ojtaso (20070109)
	CGraphImp* MakeNewGraph(LPVOID pPacketRQ, char *p_szGraphData);	
	bool DeleteEmptyGroup();
	// graph를 비교하여 packet도 삭제한다.
	void DeleteGraph(CGraphImp* pDeleteGraph, CGraphImp* pSourceGraph);
	// packet은 삭제하지 않고 graph만 삭제한다.
	void DeleteGraphNoDeletePacket(CGraphImp* pGraph);
	// 두 graph를 바꾼다.
	void ChangeGraph(CGraphImp* pSourceGraph, CGraphImp* pTargetGraph);
//	bool CopyGraphData(CGraphImp* pTargetGraph, const CString& sourceGraphData);

	// 이미 생성되어 있는 graph에 data를 setting.
	void ModifyGraph( CGraphImp* pGraph, char *p_szGraphData);

	// 현재 보이는 화면에서의 min, max값 구한다. ==> y축의 값
	CDisplayDataMinMax GetDisplayDataMinMaxInAll() const;
	bool GetDisplayDataMinMaxInOnlyPrice(CDisplayDataMinMax& displayDataMinMax) const;
	void GetDisplayDataMinMax(CGraphImp* pGraph, CDisplayDataMinMax& displayDataMinMax) const;

	// draw 하기 위한 값 추출
	void SetDrawingVertScaleData(CDrawingVertScaleData* pDrawingVertScaleData);
	// 세로 scale의 data 초기화
	void InitializeDrawingVertScaleData(CDrawingVertScaleData* pDrawingVertScaleData);

	// scale을 그릴지의 여부
	bool IsVertScaleDrawing() const;
	// <가격차트> 만의 scale 인지 여부
	bool DoesOnlyPriceVertScale() const;
	
private:
	CMainBlockBaseData* m_pMainBlockBaseData;
	CVertScaleGroupList* m_pVertScaleGroupList; // 상위 class 주소
	CVerticalScale m_vertScale;	// 세로 scale
	CDisplayDataMinMax m_displayDataMinMax;	// 화면의 Min/Max
	CTypedPtrList<CObList, CGraphImp*> m_GraphList; // CGraphImp들을 list로..
	// Y Scale상에 종가 표시 수정 - ojtaso (20070807)
	CGraphImp*	m_lpSelectedGraph;

public:
	// graph을 생성&삭제
	void MakeGraphs( char *p_szVertScaleGroup);
	// RQ로 검색 : 복수종목 - ojtaso (20070118)
	void MakeGraphs(LPVOID pPacketRQ,  char *p_szVertScaleGroup);
	bool MakeGraph( char *p_szGraphData, const bool bIsAddTail = true);
	// RQ를 가지고 있는 Graph 생성 : 복수종목 - ojtaso (20061224)
	// CPacketRQ*로 변경 : 복수종목 - ojtaso (20070109)
	bool MakeGraph(LPVOID pPacketRQ, char *p_szGraphData, const bool bIsAddTail = true);
	bool DeleteGraphWithNotify( CGraphImp* pGraph, const bool bIsOnlyNoTRData = true);
	bool DeleteGraph(CGraphImp* pGraph, const bool bIsOnlyNoTRData = true);
	void DeleteAll();
	void AddGraph(CGraphImp* pGraph, const bool bIsAddTail = true);
	bool RemoveAt(CGraphImp* pGraph, const bool bIsDeleteBlock);

	bool MoveAllGraphs(CVertScaleGroupList& groupList);
	bool MoveAllGraphs(CVertScaleGroup* pSourceGroup);
	// graph 변경
//	bool ChangeGraph(const CString& targetGraphTitle, const CString& sourceGraphData);
	// graph GraphName/subGraphName 의 변경. (해당 graph는 모두 적용, subGraphCount는 1개에 한함.)
//	bool ChangeAllGraphNameAndSubGraphName(const CString& targetGraphName, 
//		const CString& sourceGraphName, const CString& sourceSubGraphName);
	// graph GraphName/subGraphName 의 변경. -> cfg의 함수명이 바뀌였을 경우 사용함
//	bool ChangeAllGraphNameAndSubGraphNames(const CString& targetGraphName, const CString& sourceGraphName, const bool bIsSubGraphCheck);
	// graph type 변경.
	bool ChangeGraphType(const CString& targetGraphName, const CGraphBaseData::GRAPHTYPE& sourceType, 
		const CGraphStyle& sourceStyle, const CGraphDrawStyle& sourceDrawStyle, const CString& sourcePacketNames);
	// subGraph data 변경.
	bool ChangeSubGraphData(const CString& targetGraphName, const CString& targetSubGraphName, const CSubGraphData& sourceSubGraphData);
	// subGraph 라인 굵기 변경.
	bool ChangeSubGraphPenThickness(const CString& targetGraphName, const int nSubGraphIndex, const CPenThickness& penThickness);
	// subGraph title 변경.
	bool ChangeSubGraphTitle(const CString& orgSubGraphTitle, const CString& changeSubGraphTitle);
	// subGraph 색 변경.
	bool ChangeAllHeadSubGraphColor(const CList<CGraphColor, CGraphColor&>& graphColorList, POSITION& colorPos) const;
	// 이미 생성되어 있는 graph에 data를 setting.
	void ModifyGraphDataInVertScaleGroup( char *p_szVertScaleGroup, const bool doesMaintainOldScale);
	// 해당 graph와 indicatorInfo 간의 변경.
	// -> gaphData(subGraph포함)를 IndicatorInfo로 변경.
	//    (지표계산과 관련된 내용이 변경된 경우 지표계산 유무)
//	bool ChangeAllGraphDataFromIndicatorInfo(const bool bIsCalculate);
	// -> IndicatorInfo를 gaphData(subGraph포함)로 변경.
	bool ChangeAllIndicatorInfoFromGraphData();
	BOOL	ChangeAllGraphDataFromIndicatorInfo( BOOL p_bWithCalculate);
	// -> 해당 graph를 Map Default IndicatorInfo로 변경
	bool ChangeAllMapDefaultIndicatorInfo( CIndicatorList* pIndicatorList);


	// 상위 class 주소
	void SetVertScaleGroupList(CVertScaleGroupList* pVertScaleGroupList);
	void SetMainBlockBaseData(CMainBlockBaseData* pMainBlockBaseData);
	// 세로 scale 값 바꾸기.
	void SetVerticalScale(const CVerticalScale& vertScale);
	// subGraph를 보이기 setting.
	void SetAllGraphsHiding(const bool bIsHiding);
	bool SetSubGraphHiding(const CString& subGraphTitle, const bool bIsHiding);
	bool SetSubGraphHiding(const CString& strGraphName, const int nSubGraphIndex, const bool bIsHiding);
	// graph의 indicatorinfo를 초기화(NULL)
	void Initialize_IndicatorInfo(const bool bIsNULL);
	// graph(subGraph들)의 packet들을 초기화
	void Initialize_AllPackets(const bool bIsCalculate);
	bool Initialize_Packet(const CString& strPacketName, const bool bIsCalculate);

	// 지표를 계산한다.
	void CalculateAllGraphs(const CCalculateBaseData::CALCULATETYPE eCalType);
	bool CalculateAllGraphs(const CList<CString, CString>& packetNameList, const CCalculateBaseData::CALCULATETYPE eCalType, const bool bIsHeadGraph);
	// vert scale Min/Max 를 계산한다.
	void CalculateDisplayDataMinMax();

	// 상위 class 주소
	CMainChartFormulate* GetMainChartFormulate() const;
	CBlockImp* GetBlock() const;
	CVertScaleGroupList* GetVertScaleGroupList() const;
	// 세로 scale 값
	CVerticalScale& GetVertScale();
	const CVerticalScale& GetVertScale() const;
	// 모든 VertScaleGroup의 모든 data
	void GetVertScaleGroupAllData( CStringList &p_slVertScaleGroupData) const;
	// 가지고 있는 모든 graph GraphName
	bool GetAllIndicatorName(CList<CString, CString>& strIndicatorNameList) const;
	bool GetAllGraphName(CList<CString, CString>& strGraphNameList) const;

	// graph
	int GetGraphCount() const;
	CString GetGraphsGraphName() const;
	// graph 색
	// 모든 block에 있는 모든 graph의 head subGraph color 가져오기.
	bool GetAllHeadSubGraphColor(CList<CGraphColor, CGraphColor&>& graphColorList) const;
	COLORREF GetGraphColor1(const int nGraphIndex, const int nSubGraphIndex) const;
	// subGraph title region
	// 타이틀 여러줄로 표시 : 복수종목 - ojtaso (20070227)
	CRect GetSubGraphTitleRegion(CDC* pDC, const CSubGraph* pSubGraph, const CRect& graphRegion, int& preTitleEndPosition, int& nPreLineCount) const;

	// graph의 세로 scale의 폭을 구한다.
	// (2008/6/13 - Seung-Won, Bae) for Support BuySell Overhanging Supply.
	void	GetLeftVertScaleRegionWidth(	CDC* pDC, int &p_nLeftWidth,	BOOL p_bIsDualScale) const;
	void	GetRightVertScaleRegionWidth(	CDC* pDC, int &p_nRightWidth,	BOOL p_bIsDualScale) const;
	// 해당 RQ만 작업 : 복수종목 - ojtaso (20070420)
	void GetSubGraphViewDataList(LPCTSTR lpszRQ, CSubGraphViewDataList& subGraphViewDataList) const;
	// 현재 보이는 화면에서의 min, max값 구한다. ==> y축의 값
	CDisplayDataMinMax GetDisplayDataMinMax() const;


	// graph title name이 모두 empty인지 구하기
	bool IsViewableGraphTitleEmpty() const;
	// 이미 존재하는 graph인지 체크
	bool IsExistenceGraph( const CString &strGraphName) const;
	// RQ를 가지고 있는 Graph 체크 : 복수종목 - ojtaso (20061224)
	bool IsExistenceGraph(LPCTSTR lpszRQ, const CString &strGraphName) const;
	// CPacketRQ*로 Graph 체크 : 복수종목 - ojtaso (20070109)
	bool IsExistenceGraph(LPVOID pPacketRQ, const CString &strGraphName) const;
	bool IsExistenceIndicator( const CString &strIndicatorName) const;
	// RQ를 가지고 있는 Graph 체크 : 복수종목 - ojtaso (20061224)
	bool IsExistenceIndicator(LPCTSTR lpszRQ, const CString &strIndicatorName) const;
	// 해당 graph Type가 존재하는지 체크
	bool IsViewableGraphType(const CGraphBaseData::GRAPHTYPE eGraphType) const;
	// 해당 graph style가 존재하는지 체크
	bool IsViewableGraphStyle(const CGraphBaseData::GRAPHTYPE eGraphType, const int nGraphStyle) const;
	// 해당 graph draw style가 존재하는지 체크
	bool IsViewableGraphDrawStyle(const CGraphBaseData::GRAPHTYPE eGraphType, const int nGraphStyle, const int nDrawStyle) const;
	// 모든 graph 가 hide 인지 체크
	bool DoesAreHidingAllGraphs() const;
	// exceptionGraph 를 제외하고 같은 indicatorInfo를 사용하는지 유무
	bool DoesUseEqualIndicatorInfo(const CIndicatorInfo* pIndiInfo, const CGraphImp* pExceptionGraph) const;

	// 세로 scale 그리기.
	void DrawVertScale(CDC* pDC, CDrawingVertScaleData* pDrawingVertScaleData, const CDrawingRegion& drawingRegion);
	// graph 그리기
	// 타이틀 여러줄로 표시 : 복수종목 - ojtaso (20070227)
	void DrawGraphs(CDC* pDC, CDrawingGraphData* pDrawingGraphData, int& preGraphTitleEndPosition, int& nEndLimit, int& nGraphTitleEndLine);
	// (2009/10/9 - Seung-Won, Bae) for Thread Draw.
	static UINT DrawGraphAndSignal( LPVOID pParam);
	
	// graph 찾기
	CGraphImp* FindGraph(const CString& strGraphName) const;
	// RQ를 가지고 있는 Graph 검색 : 복수종목 - ojtaso (20061224)
	CGraphImp* FindGraph(LPCTSTR lpszRQ, const CString& strGraphName) const;
	CGraphImp* GetGraph(const int nIndex) const;
	CGraphImp* FindGraph(const CString& strTitleName, int& nSubGraphIndex) const;
	CGraphImp* FindIndicator( const CString& strIndicatorName) const;
	// RQ를 가지고 있는 Graph 검색 : 복수종목 - ojtaso (20061224)
	CGraphImp* FindIndicator(LPCTSTR lpszRQ, const CString& strIndicatorName) const;

	// subGraph Title을 이용하여 subGraph찾기.
	// 타이틀 여러줄로 표시 : 복수종목 - ojtaso (20070227)
	CSubGraph* FindSubGraphTitle(CDC* pDC, const CRect& allTitleRegion, const CPoint& point, 
		CRect& titleRegion, int& preTitleEndPosition, int& nPreLineCount) const;
	// subGraph 찾기.
	bool FindSubGraph(const CPoint& point, CSelectedSubGraphDeque& selectedSubGraphDeque) const;
//	bool FindSubGraph(const CPoint& point, CString& strSubGraphData, int& nSubGraphIndex, int& nDataIndex) const;

	// (2006/6/25 - Seung-Won, Bae) Search Index of Graph
	BOOL	GetIndexOfGraph( const char *p_szGraphName, int &p_nG);

	// Change Input Packet Names and Calculate again.
	BOOL ChangeIndicatorPacketNames( const char *p_szIndicatorName, const int p_nSubGraphIndex, const char *p_szPacketNames);

	// Y Scale상에 종가 표시 수정 - ojtaso (20070807)
	BOOL IsScaleTextDrawing();
	// Y Scale상에 종가 표시 수정 - ojtaso (20070807)
	void SetSelectedGraph(CGraphImp*& lpSelectedGraph);

	// 지표 뒤집기 - ojtaso (20071023)
	bool	GetVertScaleInvertType() const;
	// 지표 뒤집기 - ojtaso (20071023)
	void	SetVertScaleInvertType(const bool bIsVertScaleInvertType);

// (2007/1/14 - Seung-Won, Bae) Get the Base Packet for Default Scale Min/Max
protected:
	CGraphImp *	m_pBaseGraph;
	CPacket *	m_pBasePacket;

// (2009/10/15 - Seung-Won, Bae) Retrieve the selected graph.
public:
	CGraphImp *		GetSelectedGraph( void)		{	return m_lpSelectedGraph;	}

#endif

};

#endif // !defined(AFX_VERTSCALEGROUP_H__D483D8C3_9A0B_4A61_AEE3_C8B9F50F1E8B__INCLUDED_)
