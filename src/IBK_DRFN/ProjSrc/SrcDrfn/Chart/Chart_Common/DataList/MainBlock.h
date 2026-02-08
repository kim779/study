// MainBlock.h: interface for the CMainBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINBLOCK_H__405226D5_F3E1_4335_BC7A_76FA68A1E0A7__INCLUDED_)
#define AFX_MAINBLOCK_H__405226D5_F3E1_4335_BC7A_76FA68A1E0A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "BlockBaseData.h"

class CCommonInterface;
class CGraphRegionColor;
class CMainBlockImplementation;
class CPacketList;
class CPacket;
class CIndicatorList;
class CBlock;
class CSubGraph;
class CScaleColor;
class CSelectedSubGraphDeque;
class CViewData;
class CCoordinateCalculatorData;
class CIndicatorConfigurationChangedData;
class CBlockRegionList;
class CGraphColor;
class CSubGraphData;
class CSubGraphNameAndColorList;


class AFX_EXT_CLASS CMainBlock
{
public:
	CMainBlock();
	CMainBlock(CCommonInterface* pParentIF);
	virtual ~CMainBlock();

	// block을 삭제
	bool DeleteBlock(const CPoint& point, const bool bAutoHorzScaleDelete = true);
	bool DeleteBlock(const CBlockIndex& blockIndex, const bool bAutoHorzScaleDelete = true);
	bool DeleteAllBlock();
	bool DeleteBlockHavingNoGraphs();
	// graph 삭제
	bool DeleteGraph(const CString& strFunctionName);
	bool DeleteGraph_TitleName(const CString& strTitleName, const bool bIsOnlyNoTRData = true);
	bool DeleteGraph_TitleName(const CBlockIndex& blockIndex, const CString& strTitleName, const bool bIsOnlyNoTRData = true);
	bool DeleteGraph_PacketNames(const CString& strGraphPacketNames);
	// graph 추가
	bool InsertGraph(const CString& graphData, const CGraphBaseData::ObjectChange blockType, const bool doesMaintainOldScale);
	bool InsertGraph(const CBlockIndex& blockIndex, const int nVertScaleGroupIndex, const CString& graphData, 
		const bool doesInsertInNewBlock = false, const bool doesInsertInNewVertScaleGroup = false, const bool doesMaintainOldScale = true);
	bool InsertGraph(const CPoint& insertPosition, const CString& graphData, const CGraphBaseData::ObjectChange blockType, const bool doesMaintainOldScale);
	// vertScaleGroup 해당 block에 이동
	bool MoveAllVertScaleGroups(const CBlockIndex& sourceBlockIndex, const CBlockIndex& targetBlockIndex);
	bool MoveVertScaleGroup(const CBlockIndex& sourceBlockIndex, const int nSourceGroupIndex, const CBlockIndex& targetBlockIndex);
	// vertScaleGroup의 join
	bool JoinAllVertScaleGroups(const CBlockIndex& blockIndex, const bool bIsJoin = true);
	// graph 이동.
	bool MoveAllGraphs(const CBlockIndex& sourceBlockIndex, const int nSourceGroupIndex, const CBlockIndex& targetBlockIndex, const int nTargetGroupIndex = -1);
	bool MoveGraph(const CBlockIndex& sourceBlockIndex, const int nSourceGroupIndex, const int nSourceGraphIndex, const CBlockIndex& targetBlockIndex, const int nTargetGroupIndex = -1);
	// graph 변경.
	// -> GraphTitle은 target 값으로 유지한다.
	bool ChangeGraph(const CBlockIndex& blockIndex, const CString& targetGraphTitle, const CString& sourceGraphData);
	bool ChangeGraph(const CString& targetFunctionName, const CString& sourceGraphData);
	bool ChangeGraph(const CString& targetFunctionName, const CIndicatorConfigurationChangedData& changedData);
	// graph functionName/subGraphName 의 변경. (해당 graph는 모두 적용, subGraphCount는 1개에 한함.)
	bool ChangeAllGraphFunctionNameAndSubGraphName(const CString& targetFunctionName, 
		const CString& sourceFunctionName, const CString& sourceSubGraphName);
	// graph functionName/subGraphName 의 변경. -> cfg의 함수명이 바뀌였을 경우 사용함
	bool ChangeFunctionNameAndSubGraphNames(const CString& targetFunctionName, const CString& sourceFunctionName, const bool bIsSubGraphCheck = true);
	// graph packetName 변경.
	bool ChangePacketNames(const CString& targetFunctionName, const CString& sourcePacketNames);
	// graph Data 변경.
	bool ChangeGraphData(const CBlockIndex& blockIndex, const int nVertScaleGroupIndex, const int nGraphIndex, 
		const CString& sourceFunctionName, const CString& sourcePacketNames, const CString& sourceSubGraphTitle, 
		const CGraphBaseData::GRAPHTYPE& sourceType, const CGraphStyle& sourceStyle, const CGraphDrawStyle& sourceDrawStyle, 
		const int nPenThickness1, const int nPenThickness2, const int nPenThickness3);
	bool ChangeGraphData(const CString& targetFunctionName, const int nSubGraphIndex, const CString& sourcePacketNames, const CString& sourceSubGraphTitle, 
		const CGraphBaseData::GRAPHTYPE& sourceType, const CGraphStyle& sourceStyle, const CGraphDrawStyle& sourceDrawStyle, 
		const int nPenThickness1, const int nPenThickness2, const int nPenThickness3);
	// GraphDataDrawingData 변경.
	bool ChangeGraphDataDrawingData(const CString& targetFunctionName, 
		const bool bLowHigh, const bool bLowCur, const bool bHighCur, const bool bPrevCur, const bool bPrevCurRate);
	// graph type 변경.
	bool ChangeGraphType(const CString& targetFunctionName, const CGraphBaseData::GRAPHTYPE& sourceType, 
		const CGraphStyle& sourceStyle, const CGraphDrawStyle& sourceDrawStyle, const CString& sourcePacketNames);
	bool ChangeGraphType(const CString& targetFunctionName, const int nSubGraphIndex, const CGraphBaseData::GRAPHTYPE eGraphType, const CGraphStyle& graphStyle, const CGraphDrawStyle& graphDrawStyle);
	bool ChangeGraphType(const CBlockIndex& blockIndex, const int nVertScaleGroupIndex, const int nGraphIndex, const int nSubGraphIndex, 
		const CGraphBaseData::GRAPHTYPE eGraphType, const CGraphStyle& graphStyle, const CGraphDrawStyle& graphDrawStyle);
	// subGraph data 변경.
	bool ChangeSubGraphData(const CString& targetFunctionName, const CString& targetSubGraphName, const CSubGraphData& sourceSubGraphData);
	// subGraph 라인 굵기 변경.
	// -> nSubGraphIndex : 바꿀 subGraph index (-1 부터 적용됨. -1 = 모든 subGraph에 해당)
	bool ChangeSubGraphPenThickness(const CString& targetFunctionName, const int nSubGraphIndex, const int nPenThickness1, const int nPenThickness2, const int nPenThickness3);
	// subGraph title 변경.
	bool ChangeSubGraphTitle(const CString& orgSubGraphTitle, const CString& changeSubGraphTitle);
	bool ChangeSubGraphTitle(const CBlockIndex& blockIndex, const CString& orgSubGraphTitle, const CString& changeSubGraphTitle);
	bool ChangeSubGraphTitle(const CBlockIndex& blockIndex, const int nScaleGroupIndex, const int nGraphIndex, const int nSubGraphIndex, const CString& changeSubGraphTitle);
	bool ChangeSubGraphTitle(const CString& strFunctionName, const int nSubGraphIndex, const CString& changeSubGraphTitle);
	// subGraph color 변경.
	bool ChangeGraphColor(const CString& strFunctionName, const CSubGraphNameAndColorList& graphNameAndColorList);
	bool ChangeGraphColor(const CString& strFunctionName, const int nSubGraphIndex, const CGraphColor& graphColor);
	bool ChangeGraphColor1(const CBlockIndex& blockIndex, const int nVertScaleGroupIndex, const int nGraphIndex, const int nSubGraphIndex, const COLORREF& newGraphColor1);
	// 해당 block의 graph값을 변경.
	bool ModifyGraphDataInBlock(const CString& graphsData, const CBlockIndex& blockIndex, const bool doesMaintainOldScale = true);
	// 해당 graph와 indicatorInfo 간의 변경.
	// -> gaphData(subGraph포함)를 IndicatorInfo로 변경.
	//    (지표계산과 관련된 내용이 변경된 경우 지표계산 유무)
	bool ChangeAllGraphDataFromIndicatorInfo(const bool bIsCalculate);
	bool ChangeGraphDataFromIndicatorInfo(const CString& strFunctionName, const bool bIsCalculate);
	// -> IndicatorInfo를 gaphData(subGraph포함)로 변경.
	bool ChangeAllIndicatorInfoFromGraphData();
	bool ChangeIndicatorInfoFromGraphData(const CString& strFunctionName);


	// object의 기본값을 set. -- block, graph, scale, data Set.

	// block의 영역은 모두 같게 만든다 --> 처음으로 block을 만든 경우
	bool SetBlockEnvironment(CDC* pDC, const CRect& allBlockRegion, const CBlockBaseData::BLOCKTYPE blockType, const CBlockBaseData::BLOCKCOMPART blockCompart, 
			const CBlockIndex& blockCount, const int graphRegionFrameThickness, const int interBlockHalfGap, const CGraphRegionColor& graphRegionColor);
	bool SetAllBlockRegion(const CRect& allBlockRegion, const bool bInitialize = false);
	bool SetBlockType(const CBlockBaseData::BLOCKTYPE blockType, const CBlockIndex& blockCount);
	void SetBlockCompart(const CBlockBaseData::BLOCKCOMPART blockCompart);
	void SetDrawingOrderType(const CDrawingOption::DRAWINGORDER eDrawingOrderType);
	void SetBlockCount(const CBlockIndex& blockCount);
	void SetGraphRegionFrameThickness(const int graphRegionFrameThickness);
	void SetInterBlockHalfGap(const int interBlockHalfGap);
	void SetGraphTitleDraw(const bool bIsCondition, const bool bIsData);
	void SetBackgroundColor(const COLORREF& color);
	void SetGraphRegionColor(const CGraphRegionColor& graphRegionColor);
	void SetEtcGraphColor(const COLORREF& graphColor1, const COLORREF& graphColor2, const COLORREF& graphColor3, const COLORREF& graphColor4, const COLORREF& graphColor5, const COLORREF& graphColor6);

	// block의 영역(순수block + gap + 보더 굵기)이 원하는 값으로 생성 
	// --> block이 이미 만들어진 후
	void SetBlocksPosition(const CString& blocksPosition);
	// 다른 block size보다 2배인 block 설정
	void SetDoubleSizeBlockNumber(const CBlockIndex& doubleSizeBlockIndex);
	// Tool과 관련된 내용 설정 (block이동, block삭제)

	// block에 Graph들을 생성한다.
	void BuildGraphsAndScales(const CString& data);

	// scale의 data 설정
	void SetScaleEnvironment(const CScaleBaseData::SCALEGRIDTYPE gridType, const COLORREF& horzGridColor, const COLORREF& vertGridColor, const COLORREF& textColor);
	void SetBaseScaleGridType(const CScaleBaseData::SCALEGRIDTYPE gridType);
	void SetAllScaleGridType(const CScaleBaseData::SCALEGRIDTYPE gridType);
	void SetAllHorzScaleGridType(const CScaleBaseData::SCALEGRIDTYPE gridType);
	void SetAllVertScaleGridType(const CScaleBaseData::SCALEGRIDTYPE gridType);
	void SetScaleColor(const COLORREF& horzGridColor, const COLORREF& vertGridColor, const COLORREF& textColor);
	bool SetHorzScaleData(const CBlockIndex& blockIndex, const CString& strScaleData);
	bool SetVertScaleData(const CBlockIndex& blockIndex, const int nVertScaleGroupIndex, const CString& strScaleData);
	void SetHorzScaleCompart(const CBlockIndex& blockIndex, const CString& dateCompart, const CString& timeCompart, const CScaleBaseData::TEXTCOMPARTTYPE eTextCompart = CScaleBaseData::HORZ_TEXT_COMPART);
	bool SetHorzScalePosition(const CBlockIndex& blockIndex, const CScaleBaseData::HORZSCALEPOSITION ePosition);
	bool SetHorzScalePosition(const int nBlockRowIndex, const CScaleBaseData::HORZSCALEPOSITION ePosition);
	void SetAllHorzScalePosition(const CScaleBaseData::HORZSCALEPOSITION ePosition);
	bool SetVertScalePosition(const CBlockIndex& blockIndex, const int nVertScaleGroupIndex, const CScaleBaseData::VERTSCALEPOSITION ePosition);
	void SetAllVertScalePosition(const CScaleBaseData::VERTSCALEPOSITION ePosition);
	void SetOnlyPriceVertScale(CDC* pDC, const bool bIsOnlyPrice, const bool bIsCalVertMinMax = true);
	void SetDisplayDataVertScale(CDC* pDC, const bool bIsDisplayData, const bool bIsCalVertMinMax = true);

	// 화면에 보일 data의 범위.
	void SetDataRegionInBlock(const int start, const int end);
	// 화면이 현재 design mode인지 설정.
	void SetDesignMode(const bool designMode = true);
	// 화면에 추가할 수 있는 총 block 수
	void SetMaxBlocksCountInView(const int maxBlockCount);
	// messagebox 띄우기 여부
	void SetMessageBoxShow(const bool bIsShow);
	// graph background gap 간격
	void SetGraphBackgroundGap(const int nGap_x, const int nGap_y);

	// ocx 주소
	void SetParentInterface(CCommonInterface* pParentIF);
	// packet에 대한 설정
	void SetPacketList(CPacketList* pPacketList);
	// prject 경로
	void SetProjectPath(const CString& projectPath);
	// 속성에 대한 설정
	void SetIndicatorList(CIndicatorList* pIndicatorList);
	// graph 삭제시 해당 graph의 indicatorInfo도 삭제한다.
	void SetAutoDeleteIndicatorInfo(const bool bIsAutoDelete);
	
	// graph의 indicatorInfo 초기화
	void Initialize_IndicatorInfo(const bool bIsNULL);
	// graph(subGraph들)의 packet들을 초기화(bIsCalculate = true 다시 계산함)
	void Initialize_AllPackets(const bool bIsCalculate);
	bool Initialize_AllPackets(const CString& strFunctionName, const bool bIsCalculate);
	bool Initialize_Packet(const CString& strPacketName, const bool bIsCalculate);
	// 지표를 계산한다.
	// 계산해야 하는 경우 : TR, 실시간, 조건값, 함수명, packetname, 스크롤, 보임
	void CalculateAllGraphs(const CCalculateBaseData::CALCULATETYPE eCalType = CCalculateBaseData::TRDATA_CAL);
	bool CalculateGraph(const CString& strFunctionName, const CCalculateBaseData::CALCULATETYPE eCalType = CCalculateBaseData::TRDATA_CAL);
	bool CalculateGraph(const CString& strFunctionName, const CCalculateBaseData::CALCULATETYPE eCalType, const bool bIsClearPacketData);


	// block에 대한 정보 바꾸기.
	bool SetBlockTypeNotChangeBlockData(const CBlockBaseData::BLOCKTYPE blockType, const CBlockIndex& blockCount);

	// subGraph를 보이기 여부 setting.
	void SetAllGraphsHiding(const bool bIsHiding = true);
	void SetAllGraphsHiding(const CBlockIndex& blockIndex, const bool bIsHiding = true);
	bool SetSubGraphHiding(const CBlockIndex& blockIndex, const CString& subGraphTitle, const bool bIsHiding = true);
	bool SetSubGraphHiding(const CBlockIndex& blockIndex, const int nVertScaleGroupIndex, const int nGraphIndex, const int nSubGraphIndex, const bool bIsHiding = true);
	// subGraph title 숨기기 여무 setting.
	void SetAllGraphsTitleHiding(const bool bIsAllHiding = true);
	// block의 세로 scale의 log/invert type setting.
	bool SetVertScaleLogAndInvertType(const CBlockIndex& blockIndex, const bool bIsLogType, const bool bIsInvertType);
	bool SetVertScaleLogAndInvertType(const CString& strFunctionName, const bool bIsLogType, const bool bIsInvertType);
	// block의 세로 scale의 log type setting.
	bool SetAllVertScaleLogType(const bool bIsLogType);
	bool SetVertScaleLogType(const CBlockIndex& blockIndex, const bool bIsLogType);
	bool SetVertScaleLogType(const CString& strFunctionName, const bool bIsLogType);
	// block의 세로 scale의 invert type setting.
	bool SetVertScaleInvertType(const CBlockIndex& blockIndex, const bool bIsInvertType);
	bool SetVertScaleInvertType(const CString& strFunctionName, const bool bIsInvertType);


	// block에 대한 data 가져오기
	// 현재 main block에 data가 있는지를 가져온다.
	bool IsEmpty() const;
	// 현재 화면에 대한 Mode를 가져온다.
	bool GetDesignMode() const;
	// region
	CRect GetAllBlockRegion() const;
	CRect GetBlockRegion(const CBlockIndex& blockIndex) const;
	CRect GetNoFrameBlockRegion(const CBlockIndex& blockIndex) const;
	CRect GetGraphRegionSumInColumn(const int blockColumnIndex = 0) const;
	CRect GetGraphRegionSumInColumn(const CPoint& point) const;
	CRect GetNoFrameGraphRegion(const CString& strFunctionName) const;
	CRect GetNoFrameGraphRegion(const CBlockIndex& blockIndex) const;	
	CRect GetNoFrameGraphRegion(const CPoint& point) const;
	CRect GetDrawingGraphRegion(const CString& strFunctionName) const;
	CRect GetDrawingGraphRegion(const CBlockIndex& blockIndex) const;
	CRect GetDrawingGraphRegion(const CPoint& point) const;
	bool GetBlockRegionList(const int blockColumnIndex, CBlockRegionList& blockRegionList) const;
	bool GetBlockRegionList(const CPoint& point, CBlockRegionList& blockRegionList) const;
	// type
	CBlockBaseData::BLOCKTYPE GetBlockType() const;
	CBlockBaseData::BLOCKCOMPART GetBlockCompartType() const;
	CDrawingOption::DRAWINGORDER& GetDrawingOrderType();
	// block의 갯수
	int GetRowCount() const;
	int GetColumnCount() const;
	// graph의 갯수
	int GetTotalGraphCount() const;
	int GetGraphCount(const CBlockIndex& blockIndex) const;
	int GetGraphCount(const CBlockIndex& blockIndex, const int nVertScaleGroupIndex) const;
	// 화면에 보이는 data 시작/끝
	void GetDisplayDataIndex(int& nDataStartInBlock, int& nDataEndInBlock) const;
	// graph 영역 테두리 두께
	int GetGraphRegionFrameThickness() const;
	// block 간의 gap/2
	int GetInterBlockHalfGap() const;
	// 한 화면에 보일 최대 block 갯수
	int GetMaxBlocksCountInView() const;
	// block의 영역 가져오기
	CString GetBlocksPosition() const;
	// 모든 block의 data
	CString GetAllBlocksData() const;
	// 가지고 있는 graph functionName
	CString GetGraphFunctionName(const CBlockIndex& blockIndex, const int nVertScaleGroupIndex = 0, const int nGraphIndex = 0) const;
	CString GetAllGraphFunctionName(const CString& strCompart = ',') const;
	bool GetAllGraphFunctionName(CList<CString, CString>& strFunctionNameList) const;
	// 해당 graph 가 있는 block의 모든 지표의 함수명 가져오기 (함수명 구분은 ";" 로 한다.)
	CString GetGraphsFunctionName_InBlock(const CString& strFunctionName) const;
	// 2배 크기 block
	CBlockIndex GetDoubleSizeBlockNumber() const;
	// 해당 graph functionName이 있는 block index
	CBlockIndex GetBlockIndex(const CString& strFunctionName) const;
	// block data가 같은 block index
	CBlockIndex GetBlockIndex_SameBlockData(const CString& graphData, const bool doesMaintainOldScale = true) const;

	
	// vert scale type
	bool GetVertScaleLogAndInvertType(const CBlockIndex& blockIndex, bool& bIsLog, bool& bIsInvert) const;
	bool GetVertScaleLogAndInvertType(const CString& strFunctionName, bool& bIsLog, bool& bIsInvert) const;
	// scale grid type
	CScaleBaseData::SCALEGRIDTYPE GetBaseScaleGridType() const;
	// scale position
	CScaleBaseData::VERTSCALEPOSITION GetVertScalePosition(const CBlockIndex& blockIndex = CBlockIndex(0, 0), const int nVertScaleGroupIndex = 0) const;
	// scale 색
	CScaleColor& GetScaleColor();
	void GetScaleColor(COLORREF& horzGridColor, COLORREF& vertGridColor, COLORREF& textColor) const;
	// scale data
	CString GetHorzScaleData(const CBlockIndex& blockIndex) const;
	// graph region에 관한 색을 가져온다.
	COLORREF GetBackgroundColor() const;
	CGraphRegionColor GetGraphRegionColor() const;
	void GetGraphRegionColor(COLORREF& backgroundColor, COLORREF& lineColor, COLORREF& frameColor) const;
	// graph 색
	bool GetGraphColor(const CString& strFunctionName, CSubGraphNameAndColorList& graphNameAndColorList) const;
	bool GetGraphColor(const CString& strFunctionName, const int nSubGraphIndex, CGraphColor& graphColor) const;
	CGraphColor* GetGraphColor(const CString& strFunctionName, const int nSubGraphIndex = 0) const;
	COLORREF GetGraphColor1(const CBlockIndex& blockIndex, const int nVertScaleGroupIndex, const int nGraphIndex, const int nSubGraphIndex) const;
	void GetEtcGraphColor(COLORREF& graphColor1, COLORREF& graphColor2, COLORREF& graphColor3, COLORREF& graphColor4, COLORREF& graphColor5, COLORREF& graphColor6) const;
	// graph packetName
	CString GetGraphPacketName(const CString& strFunctionName) const;
	CString GetGraphPacketName_TitleName(const CString& strSubGraphTitle) const;
	CString GetGraphPacketName_TitleName(const CBlockIndex& blockIndex, const CString& strSubGraphTitle) const;
	// packet
	bool GetGraphPacketList(const CString& strFunctionName, CList<CPacket*, CPacket*>& packetList) const;
	// graph type
	bool GetGraphType(const CString& strFunctionName, const int nSubGraphIndex, CGraphBaseData::GRAPHTYPE& eGraphType, CGraphStyle& graphStyle, CGraphDrawStyle& graphDrawStyle) const;
	// graph style
	bool GetGraphStyle(const CString& strFunctionName, const int nSubGraphIndex, CGraphStyle& graphStyle, CGraphDrawStyle& graphDrawStyle) const;
	// subGraphData
	CSubGraphData* GetSubGraphData(const CString& strFunctionName, const CString& strSubGraphName) const;
	CSubGraphData* GetSubGraphData(const CString& strFunctionName, const int nSubGraphIndex) const;
	// subGraph names (bIsViewable = true : 현재 보이고 있는 subGraph)
	CString GetSubGraphNames(const CString& strFunctionName, const bool bIsViewable = true) const;
	// subGraph의 title
	bool GetAllSubGraphTitle(const CString& strFunctionName, CList<CString, CString>& titleList) const;
	CString GetSubGraphTitle(const CString& strFunctionName, const int nSubGraphIndex = 0) const;
	// GraphDataDrawingData
	bool GetGraphDataDrawingData(const CString& strFunctionName, 
		bool& bLowHigh, bool& bLowCur, bool& bHighCur, bool& bPrevCur, bool& bPrevCurRate);
	// ocx 주소
	CWnd* GetParentWnd() const;
	// packet dll 주소
	CPacketList* GetPacketList() const;
	// 속성dll 주소
	CIndicatorList* GetIndicatorList() const;
	// project 경로
	CString GetProjectPath() const;

	// 해당 column의 graph 폭
	int GetGraphRegionWidthOfBlocksColumn(CDC* pDC, const int blockColumnIndex = 0) const;
	// view data
	bool GetViewDataListInAllBlock(CViewData& viewData) const;
	// data min/max
	bool GetDisplayDataMinMax(const CString& functionName, double& dMin, double& dMax) const;
	bool GetDisplayDataMinMax(const CString& functionName, const CBlockIndex& blockIndex, double& dMin, double& dMax) const;
	bool GetDisplayDataMinMax(const CBlockIndex& blockIndex, const int nVertScaleGroupIndex, double& dMin, double& dMax) const;
	// AnalysisTool에서 좌표 계산을 위한 data.
	void GetCoordinateCalculatorData(const CPoint& point, CCoordinateCalculatorData& calData) const;
	void GetCoordinateCalculatorData(const CBlockIndex& blockIndex, CCoordinateCalculatorData& calData) const;


	// 해당 지표가 있는지의 유무
	bool IsExistenceGraph(const CString& functionName) const;
	bool IsExistenceGraph(const CString& functionName, const CGraphBaseData::GRAPHTYPE eGraphType) const;
	bool IsExistenceGraph(const CString& functionName, const CBlockIndex& blockIndex, const int nVertScaleGroupIndex, const int nGraphIndex) const;
	bool IsExistenceGraphInBlock(const CString& functionName, const CBlockIndex& blockIndex) const;
	bool IsExistenceGraphInBlock(const CString& functionName, const CPoint& point) const;

	// 해당 graph Type가 존재하는지 체크
	bool IsViewableGraphType(const CGraphBaseData::GRAPHTYPE eGraphType) const;


	// block들 그리기
	void DrawBlocks(CDC* pDC);
	void DrawBlocks(CDC* pDC, const CDrawingOption::DRAWINGTYPE eDrawingType, const bool bIsCalDataForDrawing);

	// 해당 block 찾기&위치 변경.
	CBlockIndex FindCompartLine(const CPoint& point) const;
	bool MoveBlocksPosition(const CPoint& point, const CBlockIndex& compartLineIndex);

	// block region을 찾는다.
	CBlockIndex FindBlockMovingRegion(const CPoint& point) const;
	bool MoveBlock(const CBlockIndex& sourceBlockIndex, const CBlockIndex& targetBlockIndex);
	CBlockIndex FindBlockDeletingRegion(const CPoint& point) const;

	// block 찾기.
	CBlockIndex FindBlockNumber(const CPoint& point) const;
	CBlock* FindBlock(const CPoint& point) const;

	// subGraph 을 찾기.
	CSubGraph* FindSubGraphTitle(CDC* pDC, const CPoint& point, CRect& titleRegion) const;
	bool FindSubGraph(const CPoint& point, CSelectedSubGraphDeque& selectedSubGraphDeque) const;
	bool FindSubGraph(const CPoint& point, CBlockIndex& blockIndex, CString& strSubGraphData, int& nSubGraphIndex, int& nDataIndex) const;

private:
	CMainBlockImplementation* m_pMainBlockImpl;
};

///////////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_MAINBLOCK_H__405226D5_F3E1_4335_BC7A_76FA68A1E0A7__INCLUDED_)
