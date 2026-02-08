// MainBlock.h: interface for the CMainBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINBLOCK_H__405226D5_F3E1_4335_BC7A_76FA68A1E0A7__INCLUDED_)
#define AFX_MAINBLOCK_H__405226D5_F3E1_4335_BC7A_76FA68A1E0A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "../BlockBaseData.h"

class CGraphRegionColor;
class CPacketList;
class CPacket;
class CIndicatorList;
class CBlock;
class CSubGraph;
class CScaleColor;
class CSelectedSubGraphDeque;
class CViewData;
class CCoordinateCalculatorData;
class CSignalData;
class CGraphColor;
class CSubGraphData;
class CXScaleManager;
class CPacketListManager;	// 복수종목 관리 : 복수종목 - ojtaso (20070111)
class _ICHARTCTRL_CTRL;
class CHorizontalScale;
interface IToolBarManager;
interface IChartCtrl;

class CMainBlock
{
public:
	virtual ~CMainBlock()	{};

public:
	virtual CBlockIndex FindBlockIndex(const CPoint& point) const	= 0;
	// block 찾기.
	virtual CBlock* GetBlock( const CBlockIndex &blockIndex) const	= 0;
	// block을 삭제
	virtual bool DeleteBlock(const CBlockIndex& blockIndex, const bool bAutoHorzScaleDelete = true)	= 0;
	virtual bool DeleteAllBlock()	= 0;
	// 블럭 최대화 추가 - ojtaso (20080519)
	virtual void		SetMaximizedBlock( const CBlockIndex& maxBlockIndex) = 0;
	virtual CBlockIndex	GetMaximizedBlock( void) const = 0;
	// graph 삭제
	virtual bool DeleteGraph(const CString& strGraphName)	= 0;
	virtual BOOL DeleteAllIndicator( const CString& strIndicatorName)	= 0;
	virtual bool DeleteGraphWithSubGraphTitle( const CBlockIndex &blockIndex, const CString& strTitleName, const bool bIsOnlyNoTRData = true)	= 0;
	virtual BOOL RemoveAllGraphOfRQ( LPVOID lpPacketRQ) = 0;	// (2008/9/14 - Seung-Won, Bae) for Pair Tick
	// RQ 지정 그래프 삭제 메소드 추가 - ojtaso (20080604)
	virtual bool DeleteGraphWithSubGraphTitleRQ(LPCTSTR lpszRQ, const CBlockIndex &blockIndex, const CString& strTitleName, const bool bIsOnlyNoTRData = true)	= 0;


//	bool DeleteGraph_PacketNames(const CString& strGraphPacketNames)	= 0;
	// graph 추가
	virtual BOOL InsertGraph(const CString& graphData, const CGraphBaseData::ObjectChange blockType, const bool doesMaintainOldScale)	= 0;
	// RQ로 검색 : 복수종목 - ojtaso (20070118)
	virtual BOOL InsertGraph(LPVOID pPacketRQ, const CString& graphData, const CGraphBaseData::ObjectChange blockType, const bool doesMaintainOldScale)	= 0;
	virtual BOOL InsertGraphInBlock(const CBlockIndex& blockIndex, const int nVertScaleGroupIndex, const CString& graphData, 
		const bool doesInsertInNewBlock = false, const bool doesInsertInNewVertScaleGroup = false, const bool doesMaintainOldScale = true)	= 0;
	// RQ를 가지고 있는 Graph 생성 : 복수종목 - ojtaso (20061224)
	// CPacketRQ*로 변경 : 복수종목 - ojtaso (20070109)
	virtual BOOL InsertGraphInBlock(LPVOID pPacketRQ, const CBlockIndex& blockIndex, const int nVertScaleGroupIndex, const CString& graphData, 
		const bool doesInsertInNewBlock = false, const bool doesInsertInNewVertScaleGroup = false, const bool doesMaintainOldScale = true)	= 0;
//	bool InsertGraph(const CPoint& insertPosition, const CString& graphData, const CGraphBaseData::ObjectChange blockType, const bool doesMaintainOldScale)	= 0;
	// vertScaleGroup 해당 block에 이동
	virtual bool MoveAllVertScaleGroupsToHeadBlock(const bool bDoesMaintainOldScale = true)	= 0;
	virtual bool MoveAllVertScaleGroupsToEachRowBlock(const CBlockIndex& targetBlockIndex, const bool bDoesMaintainOldScale = true)	= 0;
	virtual bool MoveAllVertScaleGroups(const CBlockIndex& sourceBlockIndex, const CBlockIndex& targetBlockIndex)	= 0;
//	bool MoveVertScaleGroup(const CBlockIndex& sourceBlockIndex, const int nSourceGroupIndex, const CBlockIndex& targetBlockIndex)	= 0;
	// vertScaleGroup의 join
	virtual bool JoinAllVertScaleGroups(const CBlockIndex& blockIndex, const bool bIsJoin = true)	= 0;
	// graph 이동.
	virtual bool MoveAllGraphs(const CBlockIndex& sourceBlockIndex, const int nSourceGroupIndex, const CBlockIndex& targetBlockIndex, const int nTargetGroupIndex = -1)	= 0;
	virtual bool MoveGraph(const CBlockIndex& sourceBlockIndex, const int nSourceGroupIndex, const int nSourceGraphIndex, const CBlockIndex& targetBlockIndex, const int nTargetGroupIndex = -1)	= 0;
	virtual bool MoveGraph2(const CBlockIndex& sourceBlockIndex, const int nSourceGroupIndex, const int nSourceGraphIndex, const CBlockIndex& targetBlockIndex, const int nTargetGroupIndex = -1, const BOOL bIsAddTail = TRUE)	= 0;
	// graph 변경.
	// -> GraphTitle은 target 값으로 유지한다.
//	bool ChangeGraph(const CBlockIndex& blockIndex, const CString& targetGraphTitle, const CString& sourceGraphData)	= 0;
//	bool ChangeGraph(const CString& targetGraphName, const CString& sourceGraphData)	= 0;
//	bool ChangeGraph(const CString& targetGraphName, const CIndicatorConfigurationChangedData& changedData)	= 0;
	// graph GraphName/subGraphName 의 변경. (해당 graph는 모두 적용, subGraphCount는 1개에 한함.)
//	bool ChangeAllGraphNameAndSubGraphName(const CString& targetGraphName, 
//		const CString& sourceGraphName, const CString& sourceSubGraphName)	= 0;
	// graph GraphName/subGraphName 의 변경. -> cfg의 함수명이 바뀌였을 경우 사용함
//	bool ChangeAllGraphNameAndSubGraphNames(const CString& targetGraphName, const CString& sourceGraphName, const bool bIsSubGraphCheck = true)	= 0;
//	bool ChangeGraphNameAndSubGraphNames(const CString& targetGraphName, const CString& sourceGraphName, const bool bIsSubGraphCheck = true)	= 0;
//	bool ChangeGraphNameAndSubGraphNames(const CBlockIndex& blockIndex, const CString& targetGraphName, const CString& sourceGraphName, const bool bIsSubGraphCheck = true)	= 0;
	// Change Input Packet Names and Calculate again.
	virtual BOOL	ChangeIndicatorPacketNames( const char *p_szIndicatorName, const int p_nSubGraphIndex, const char *p_szPacketNames)	= 0;
	// graph Data 변경.
//	bool ChangeGraphData(const CBlockIndex& blockIndex, const int nVertScaleGroupIndex, const int nGraphIndex, 
//		const CString& sourceGraphName, const CString& sourcePacketNames, const CString& sourceSubGraphTitle, 
//		const CGraphBaseData::GRAPHTYPE& sourceType, const CGraphStyle& sourceStyle, const CGraphDrawStyle& sourceDrawStyle, 
//		const int nPenThickness1, const int nPenThickness2, const int nPenThickness3)	= 0;
	virtual bool ChangeGraphData(const CString& targetGraphName, const int nSubGraphIndex, const CString& sourcePacketNames, const CString& sourceSubGraphTitle, 
		const CGraphBaseData::GRAPHTYPE& sourceType, const CGraphStyle& sourceStyle, const CGraphDrawStyle& sourceDrawStyle, 
		const int nPenThickness1, const int nPenThickness2, const int nPenThickness3)	= 0;
	// GraphDataDrawingData 변경.
	virtual bool ChangeGraphDataDrawingData(const CString& p_strIndicatorName, const bool bLowHigh, const bool bLowCur, const bool bHighCur, const bool bPrevCur, const int nCurData)	= 0;
	// GraphDataDraw 여부 추가 - ojtaso (20080609)
	virtual bool ShowGraphDataDrawingData(const CString& strRQ, const CString& p_strIndicatorName, const bool bShow, const int nShowType) = 0;
	// ViewMinMaxDrawingData 변경.
//	virtual bool ChangeViewMinMaxDrawingData(const CString& targetGraphName, 
//		const bool bIsMinDrawing, const bool bIsMaxDrawing, const bool bIsSignDrawing, const int nTextColorType)	= 0;
	// graph type 변경.
	virtual bool ChangeGraphType(const CString& targetGraphName, const CGraphBaseData::GRAPHTYPE& sourceType, 
		const CGraphStyle& sourceStyle, const CGraphDrawStyle& sourceDrawStyle, const CString& sourcePacketNames)	= 0;
	// subGraph data 변경.
	virtual bool ChangeSubGraphData(const CString& targetGraphName, const CString& targetSubGraphName, const CSubGraphData& sourceSubGraphData)	= 0;
	// subGraph title 변경.
	virtual bool ChangeSubGraphTitle(const CString& orgSubGraphTitle, const CString& changeSubGraphTitle)	= 0;
	virtual bool ChangeSubGraphTitle(const CBlockIndex& blockIndex, const int nScaleGroupIndex, const int nGraphIndex, const int nSubGraphIndex, const CString& changeSubGraphTitle)	= 0;
	virtual bool ChangeSubGraphTitle(const CString& strGraphName, const int nSubGraphIndex, const CString& changeSubGraphTitle)	= 0;
	virtual bool ChangeSubGraphTitleFromIndicator(const CString& strGraphName, const int nSubGraphIndex)	= 0;
	// subGraph color 변경.
	virtual bool ChangeGraphColor1(const CBlockIndex& blockIndex, const int nVertScaleGroupIndex, const int nGraphIndex, const int nSubGraphIndex, const COLORREF& newGraphColor1)	= 0;
	// 해당 block의 graph값을 변경.
//	bool ModifyGraphDataInBlock(const CString& graphsData, const CBlockIndex& blockIndex, const bool doesMaintainOldScale = true)	= 0;
	// 해당 graph와 indicatorInfo 간의 변경.
	// -> gaphData(subGraph포함)를 IndicatorInfo로 변경.
	//    (지표계산과 관련된 내용이 변경된 경우 지표계산 유무)
//	bool ChangeAllGraphDataFromIndicatorInfo(const bool bIsCalculate)	= 0;
	virtual bool ChangeGraphDataFromIndicatorInfo(const CString& strGraphName, const bool bIsCalculate)	= 0;
	// -> IndicatorInfo를 gaphData(subGraph포함)로 변경.
	virtual bool ChangeAllIndicatorInfoFromGraphData()	= 0;
	virtual BOOL ChangeAllGraphDataFromIndicatorInfo( BOOL p_bWithCalculate)	= 0;
	virtual bool ChangeIndicatorInfoFromGraphData(const CString& strGraphName)	= 0;
	// -> 해당 graph를 Map Default IndicatorInfo로 변경
	virtual bool ChangeAllMapDefaultIndicatorInfo( void)	= 0;
//	bool ChangeMapDefaultIndicatorInfo(const CString& strGraphName, const bool bIsAutoMake = true)	= 0;

	// object의 기본값을 set. -- block, graph, scale, data Set.
	// block의 영역은 모두 같게 만든다 --> 처음으로 block을 만든 경우
	virtual bool SetBlockEnvironment(CDC* pDC, const CRect& allBlockRegion, const CBlockBaseData::BLOCKTYPE blockType, const CBlockBaseData::BLOCKCOMPART blockCompart, 
			const CBlockIndex& blockCount, const int graphRegionFrameThickness, const int interBlockHalfGap, const CGraphRegionColor& graphRegionColor)	= 0;
	virtual bool SetAllBlockRegion(const CRect& allBlockRegion)	= 0;
	virtual void SetBlockCount(const CBlockIndex& blockCount)	= 0;
	virtual void SetGraphTitleDraw(const bool bIsCondition, const bool bIsData)	= 0;
	virtual void SetBackgroundColor(const COLORREF& color)	= 0;
	virtual void SetGraphRegionColor(const CGraphRegionColor& graphRegionColor)	= 0;
	virtual void SetEtcGraphColor(const COLORREF& graphColor1, const COLORREF& graphColor2, const COLORREF& graphColor3, const COLORREF& graphColor4, const COLORREF& graphColor5, const COLORREF& graphColor6)	= 0;

	// block의 영역(순수block + gap + 보더 굵기)이 원하는 값으로 생성 
	// --> block이 이미 만들어진 후
	virtual void SetBlocksPosition(const CString& blocksPosition)	= 0;
	// 다른 block size보다 2배인 block 설정
	virtual void SetDoubleSizeBlockNumber(const CBlockIndex& doubleSizeBlockIndex)	= 0;
	// Tool과 관련된 내용 설정 (block이동, block삭제)

	// block에 Graph들을 생성한다.
	// 그래프 생성후 계산 여부 : xScaleManager - ojtaso (20070701)
	virtual BOOL BuildGraphsAndScales(const CString& data, BOOL p_bWithIndicatorInfoUpdate = TRUE)	= 0;

	// scale의 data 설정
	virtual void SetAllHorzScaleGridType(const CScaleBaseData::SCALEGRIDTYPE gridType)	= 0;
	virtual void SetAllVertScaleGridType(const CScaleBaseData::SCALEGRIDTYPE gridType)	= 0;
	virtual void SetScaleColor(const COLORREF& horzGridColor, const COLORREF& vertGridColor, const COLORREF& textColor)	= 0;
	virtual bool SetHorzScaleData(const CBlockIndex& blockIndex, const CString& strScaleData)	= 0;
	// (2009/9/6 - Seung-Won, Bae) Do not support CHorizontalScale::SetHorzScalePosition() as public.
	//		To rearrange the row positions. Use CMainBlockImp::SetHorzScalePosition().
	virtual void SetHorzScalePosition( CHorizontalScale &p_horizontalScale, const CScaleBaseData::HORZSCALEPOSITION ePosition) = 0;
	virtual bool SetVertScaleData(const CBlockIndex& blockIndex, const int nVertScaleGroupIndex, const CString& strScaleData)	= 0;
	// 호가단위, 기준가 사용 - ojtaso (20070917)
	virtual void SetVertScaleType(const CBlockIndex& blockIndex, const int nVertScaleGroupIndex, const int nScaleType, const double dTypeValue) = 0;
	virtual void SetHorzScaleCompart(const CBlockIndex& blockIndex, const CString& dateCompart, const CString& timeCompart, const CScaleBaseData::TEXTCOMPARTTYPE eTextCompart = CScaleBaseData::HORZ_TEXT_COMPART)	= 0;
	virtual bool SetHorzScalePosition(const CBlockIndex& blockIndex, const CScaleBaseData::HORZSCALEPOSITION ePosition)	= 0;
	virtual void SetAllHorzScalePosition(const CScaleBaseData::HORZSCALEPOSITION ePosition)	= 0;
	virtual void SetBaseHorzScalePosition()	= 0;
	virtual bool SetVertScalePosition(const CBlockIndex& blockIndex, const int nVertScaleGroupIndex, const CScaleBaseData::VERTSCALEPOSITION ePosition)	= 0;
	virtual void SetAllVertScalePosition(const CScaleBaseData::VERTSCALEPOSITION ePosition)	= 0;
	virtual void SetOnlyPriceVertScale(CDC* pDC, const bool bIsOnlyPrice)	= 0;
	// 기준선 보임 환경설정과 Scale 자동포함 기능 - ojtaso (20071101)
	virtual BOOL SetDrawBaseLine(BOOL bDrawBaseLine) = 0;
	virtual bool GetDrawBaseLine() = 0;

	// 화면이 현재 design mode인지 설정.
	virtual void SetDesignMode(const bool designMode = true)	= 0;
	// 화면에 추가할 수 있는 총 block 수
	virtual void SetMaxBlocksCountInView(const int maxBlockCount)	= 0;
	// messagebox 띄우기 여부
	virtual void SetMessageBoxShow(const bool bIsShow)	= 0;
	// graph background gap 간격
	virtual void SetGraphBackgroundGap(const int nGap_x, const int nGap_y)	= 0;

	// ocx 주소
	virtual void			SetIChartCtrl( IChartCtrl* p_pICharetCtrl)	= 0;
	virtual IChartCtrl *	GetIChartCtrl( void)	= 0;

	// packet에 대한 설정
	virtual void SetPacketList( CPacketList* pPacketList, const char *p_szRQ)	= 0;
	// OCX 경로
	virtual void SetOCXPath(const CString& strOCXPath)	= 0;
	// 속성에 대한 설정
	virtual void SetIndicatorList(CIndicatorList* pIndicatorList)	= 0;
	// X-Scale Manager
	virtual void SetXScaleManager(CXScaleManager* pxScaleManager)	= 0;
	// 복수종목 관리 : 복수종목 - ojtaso (20070111)
	virtual void SetPacketListManager(CPacketListManager* pPacketListManager)	= 0;
	virtual void SetToolBarManager(IToolBarManager* pToolManager)	= 0;

	// graph 삭제시 해당 graph의 indicatorInfo도 삭제한다.
	virtual void SetAutoDeleteIndicatorInfo(const bool bIsAutoDelete)	= 0;
	
	// graph의 indicatorInfo 초기화
	virtual void Initialize_IndicatorInfo(const bool bIsNULL)	= 0;
	// graph(subGraph들)의 packet들을 초기화(bIsCalculate = true 다시 계산함)
	virtual bool Initialize_AllPackets(const CString& strGraphName, const bool bIsCalculate)	= 0;
	virtual bool Initialize_Packet(const CString& strPacketName, const bool bIsCalculate)	= 0;
	// 지표를 계산한다.
	// 계산해야 하는 경우 : TR, 실시간, 조건값, 함수명, packetname, 스크롤, 보임
	virtual void CalculateAllGraphs(const CCalculateBaseData::CALCULATETYPE eCalType = CCalculateBaseData::TRDATA_CAL)	= 0;
	virtual bool CalculateAllGraphs(const CList<CString, CString>& packetNameList, const CCalculateBaseData::CALCULATETYPE eCalType, const bool bIsHeadGraph)	= 0;
	virtual bool CalculateGraph(const CString& strGraphName, const CCalculateBaseData::CALCULATETYPE eCalType = CCalculateBaseData::TRDATA_CAL, const bool bIsClearPacketData = false)	= 0;

	// block에 대한 정보 바꾸기.
//	virtual bool SetBlockTypeNotChangeBlockData(const CBlockBaseData::BLOCKTYPE blockType, const CBlockIndex& blockCount)	= 0;

	// subGraph를 보이기 여부 setting.
	virtual void SetAllGraphsHiding(const CBlockIndex& blockIndex, const bool bIsHiding = true)	= 0;
	virtual bool SetSubGraphHiding(const CString& strGraphName, const int nSubGraphIndex, const bool bIsHiding = true)	= 0;
	virtual bool SetSubGraphHiding(const CBlockIndex& blockIndex, const CString& subGraphTitle, const bool bIsHiding = true)	= 0;
	virtual bool SetSubGraphHiding(const CBlockIndex& blockIndex, const int nVertScaleGroupIndex, const int nGraphIndex, const int nSubGraphIndex, const bool bIsHiding = true)	= 0;
	// subGraph title 숨기기 여무 setting.
	virtual void SetAllGraphsTitleHiding(const bool bIsAllHiding = true)	= 0;
	// block의 세로 scale의 log/invert type setting.
	virtual bool SetVertScaleLogAndInvertType(const CString& strGraphName, const bool bIsLogType, const bool bIsInvertType)	= 0;
	// block의 세로 scale의 log type setting.
	virtual bool SetAllVertScaleLogType(const bool bIsLogType)	= 0;
	virtual bool SetVertScaleLogType(const CBlockIndex& blockIndex, const bool bIsLogType)	= 0;
	virtual bool SetVertScaleLogType(const CString& strGraphName, const bool bIsLogType)	= 0;
	// block의 세로 scale의 invert type setting.
	virtual bool SetAllVertScaleInvertType(const CBlockIndex& blockIndex, const bool bIsInvertType)	= 0;


	// block에 대한 data 가져오기
	// 현재 main block에 data가 있는지를 가져온다.
	virtual bool IsEmpty() const	= 0;
	// 현재 화면에 대한 Mode를 가져온다.
	virtual bool GetDesignMode() const	= 0;
	// region
	virtual CRect GetAllBlockRegion() const	= 0;
	virtual CRect GetBlockRegion(const CBlockIndex& blockIndex) const	= 0;
	virtual CRect GetGraphRegionSumInColumn(const CPoint& point) const	= 0;
	virtual CRect GetNoFrameGraphRegion(const CBlockIndex& blockIndex) const	= 0;	
	virtual CRect GetNoFrameGraphRegion(const CPoint& point) const	= 0;
	// Graph 분리/삽입 - ojtaso (20070731)
	virtual CRect GetNoFrameGraphRegion(const CPoint& point, int& nInsertCol, int& nInsertRow) const	= 0;
	virtual CRect GetDrawingGraphRegion(const CBlockIndex& blockIndex) const	= 0;
	// type
	virtual CBlockBaseData::BLOCKTYPE GetBlockType() const	= 0;
	virtual CBlockBaseData::BLOCKCOMPART GetBlockCompartType() const	= 0;
	// block의 갯수
	virtual int GetRowCount() const	= 0;
	virtual int GetRowCountInColumn( int p_nColumnIndex) const	= 0;
	virtual int GetColumnCount() const	= 0;
	// graph의 갯수
	// 화면에 보이는 data 시작/끝
	virtual void GetDisplayDataIndex(int& nDataStartInBlock, int& nDataEndInBlock) const	= 0;
	// graph 영역 테두리 두께
	virtual int GetGraphRegionFrameThickness() const	= 0;
	// block 간의 gap/2
	virtual int GetInterBlockHalfGap() const	= 0;
	// 한 화면에 보일 최대 block 갯수
	virtual int GetMaxBlocksCountInView() const	= 0;
	// block의 영역 가져오기
	virtual CString GetBlocksPosition() const	= 0;
	// 모든 block의 data
	virtual void GetAllBlocksData( CString &p_strAllBlockData) const	= 0;
	// 가지고 있는 graph GraphName
	virtual CString GetIndicatorName(const CBlockIndex& blockIndex, const int nVertScaleGroupIndex = 0, const int nGraphIndex = 0) const	= 0;
	virtual bool GetAllIndicatorName(CList<CString, CString>& strIndicatorNameList) const	= 0;
	// (2008/9/14 - Seung-Won, Bae) for Pair Tick
	virtual bool GetAllIndicatorAndGraphNameWithRQ(CList<CString, CString>& strIndicatorNameList, CList<CString, CString>& strGraphNameList, const char *p_szRQ) = 0;
	virtual bool GetAllGraphName(CList<CString, CString>& strGraphNameList) const	= 0;
	// 해당 graph 가 있는 block의 모든 지표의 함수명 가져오기 (함수명 구분은 "	= 0;" 로 한다.)
	// 2배 크기 block
	virtual CBlockIndex GetDoubleSizeBlockNumber() const	= 0;
	// 해당 graph GraphName이 있는 block index
	// block data가 같은 block index
//	CBlockIndex GetBlockIndex_SameBlockData(const CString& graphData, const bool doesMaintainOldScale = true) const	= 0;

	
	// vert scale type
	virtual CScaleBaseData::SCALEGRIDTYPE	GetFirstBlockHGridLineType( void)	= 0;
	virtual CScaleBaseData::SCALEGRIDTYPE	GetFirstBlockVGridLineType( void)	= 0;
	// scale position
	virtual CScaleBaseData::VERTSCALEPOSITION GetVertScalePosition(const CBlockIndex& blockIndex = CBlockIndex(0, 0), const int nVertScaleGroupIndex = 0) const	= 0;
	virtual CScaleBaseData::HORZSCALEPOSITION GetHorzScalePosition(const CBlockIndex& blockIndex = CBlockIndex(0, 0)) const	= 0;
	// scale 색
	virtual CScaleColor& GetScaleColor()	= 0;
	virtual void GetScaleColor(COLORREF& horzGridColor, COLORREF& vertGridColor, COLORREF& textColor) const	= 0;
	// scale data
	virtual CString GetHorzScaleData(const CBlockIndex& blockIndex) const	= 0;
	// 수치조회창의 자료일자 Format, X Scale 설정값에 따라 처리 - ojtaso (20070827)
	virtual void GetHorzScaleCompart( const CBlockIndex& blockIndex, CString& strCompartDate, CString& strCompartTime) = 0;
	// graph region에 관한 색을 가져온다.
	virtual COLORREF GetBackgroundColor() const	= 0;
	virtual CGraphRegionColor GetGraphRegionColor() const	= 0;
	virtual void GetGraphRegionColor(COLORREF& backgroundColor, COLORREF& lineColor, COLORREF& frameColor) const	= 0;
	// graph 색
	virtual CGraphColor* GetGraphColor(const CString& strGraphName, const int nSubGraphIndex = 0) const	= 0;
	virtual void GetEtcGraphColor(COLORREF& graphColor1, COLORREF& graphColor2, COLORREF& graphColor3, COLORREF& graphColor4, COLORREF& graphColor5, COLORREF& graphColor6) const	= 0;
	// 미래영역 스케일을 그릴것인지 : ojtaso (20070724)
	virtual BOOL GetDrawFutureScale() const = 0;
	virtual void SetDrawFutureScale(const BOOL bDrawFutureScale) = 0;
	// 차트DLL에서 복수종목을 사용할지 : 복수종목 - ojtaso (20070904)
	virtual void SetMultiItemMode(const BOOL bMultiItem) = 0;
	virtual BOOL GetMultiItemMode() const = 0;
	// graph packetName
	virtual CString GetGraphPacketName(const CString& strGraphName) const	= 0;
	virtual CString GetGraphPacketName_TitleName(const CString& strSubGraphTitle) const	= 0;
	virtual CString GetGraphPacketName_TitleName(const CBlockIndex& blockIndex, const CString& strSubGraphTitle) const	= 0;
	// packet
	// graph type
	virtual bool GetGraphType(const CString& strGraphName, const int nSubGraphIndex, CGraphBaseData::GRAPHTYPE& eGraphType, CGraphStyle& graphStyle, CGraphDrawStyle& graphDrawStyle) const	= 0;
	// graph style
	// subGraphData
	virtual CSubGraphData* GetSubGraphData(const CString& strGraphName, const CString& strSubGraphName) const	= 0;
	// subGraph names (bIsViewable = true : 현재 보이고 있는 subGraph)
	virtual CString GetSubGraphNames(const CString& strGraphName, const bool bIsViewable = true) const	= 0;
	// subGraph의 title
	virtual bool GetAllSubGraphTitle(const CString& strGraphName, CList<CString, CString>& titleList) const	= 0;
	// ViewMinMaxDrawingData 변경.
//	bool GetViewMinMaxDrawingData(const CString& strGraphName, bool& bIsMinDrawing, bool& bIsMaxDrawing, bool& bIsSignDrawing, bool& nTextColorType)	= 0;

	// packet dll 주소
	virtual CPacketList* GetPacketList() const	= 0;
	// 속성dll 주소
	virtual CIndicatorList* GetIndicatorList() const	= 0;
	// X-Scale Manager
	virtual CXScaleManager* GetXScaleManager() const	= 0;
	// 복수종목 관리 : 복수종목 - ojtaso (20070111)
	virtual CPacketListManager* GetPacketListManager() const	= 0;
	// IToolBarManager
	virtual IToolBarManager* GetToolBarManager() const	= 0;

	// OCX 경로
	virtual CString GetOCXPath() const	= 0;
	// 해당 RQ만 작업 : 복수종목 - ojtaso (20070420)
	virtual bool GetViewDataListInAllBlock(LPCTSTR lpszRQ, CViewData& viewData) const	= 0;
	// data min/max
	virtual bool GetDisplayDataMinMax(const CString& GraphName, double& dMin, double& dMax) const	= 0;

	// 해당 지표가 있는지의 유무
	virtual BOOL IsExistenceGraph( const CString &strGraphName) const	= 0;
	virtual BOOL IsExistenceGraphInBlock( const CString& GraphName, const CBlockIndex& blockIndex) const	= 0;
	virtual BOOL IsExistenceIndicator( const CString &strIndicatorName) const	= 0;
	// 복수종목별 검사 : 복수종목 - ojtaso (20070502)
	virtual BOOL IsExistenceIndicator(LPVOID pPacketRQ, const CString &strIndicatorName) const	= 0;
	virtual BOOL IsExistenceIndicatorInBlock( const CString &strIndicatorName, const CBlockIndex& blockIndex) const	= 0;
	// RQ로 지표검색 : 복수종목 - ojtaso (20070109)
	virtual BOOL IsExistenceIndicatorInBlock(LPCTSTR lpszRQ, const CString &strIndicatorName, const CBlockIndex& blockIndex) const	= 0;
	virtual BOOL IsExistenceIndicatorInBlock( const CString &strIndicatorName, const CPoint& point) const	= 0;

	// block들 그리기
	virtual void CalculateDataForDrawing( CDC* pDC) = 0;
	virtual void DrawBlocks(CDC* pDC, const CDrawingOption::DRAWINGTYPE eDrawingType, const bool bIsCalDataForDrawing)	= 0;
	virtual void ChangeDrawStyle(int nDrawStyle) = 0;
	// 해당 block 찾기&위치 변경.
	virtual CBlockIndex FindCompartLine(const CPoint& point) const	= 0;
	virtual bool MoveBlocksPosition(const CPoint& point, const CBlockIndex& compartLineIndex)	= 0;

	// block region을 찾는다.
	virtual CBlockIndex FindBlockMovingRegion(const CPoint& point) const	= 0;
	virtual bool MoveBlock(const CBlockIndex& sourceBlockIndex, const CBlockIndex& targetBlockIndex)	= 0;
	virtual CBlockIndex FindBlockDeletingRegion(const CPoint& point) const	= 0;
	// 블럭 최대화 추가 - ojtaso (20080519)
	virtual CBlockIndex FindBlockSizingRegion(const CPoint& point) const	= 0;

	// subGraph 을 찾기.
	virtual CSubGraph *	FindSubGraphTitle(CDC* pDC, const CPoint& point, CRect& titleRegion) const	= 0;
	virtual bool		FindSubGraph(const CPoint& point, CSelectedSubGraphDeque& selectedSubGraphDeque) const	= 0;
	virtual CString		FindGraph( const CPoint &p_pt) const	= 0;
	// (2006/6/25 - Seung-Won, Bae) Get Index of Graph
	virtual BOOL	GetIndexOfGraph( const char *p_szGraphName, int &p_nR, int &p_nC, int &p_nV, int &p_nG)	= 0;

	// ( 2006 / 09 / 26 - Sang-Woo, Cho )
	// 가격차트에서 최고, 최저가 Text가 보이게 할 것인지 말 것인지에 대한 값을 설정한다.
	virtual BOOL	GetShowPriceMinMaxText( void )	= 0;					
	virtual void	SetShowPriceMinMaxText( BOOL bShowPriceMinMaxText )	= 0;

	// (2006/10/13 - Seung-Won, Bae) Manage Block Move & Delete Tool Flag for Position of Graph Drawing
	virtual void	SetBlockMoveDeleteFlag( BOOL p_bBlockMoveDelete)	= 0;

	// (2006/11/12 - Seung-Won, Bae) Support VScale Min/Max Type Change for All VSCale
	virtual void	SetAllVScaleMinMaxType( CScaleBaseData::VERTMINMAXTYPE p_eVScaleMinMaxType, CDC* pDC)	= 0;
	// (2006/11/14 - Seung-Won, Bae) Support VScale Min/Max Type Release in TR
	virtual void	ReleaseAllVScaleSettingInFixedMinMax( CDC *pDC)	= 0;

	// VertScaleGroup의 가장 처음 Graph의 타입 : 복수종목 - ojtaso (20070504)
	virtual CScaleBaseData::HORZSCALEDRAWERTYPE GetMasterValueType(const int nRow, const int nCol)	= 0;
	virtual LPCTSTR GetMasterValueRQ(const int nRow, const int nCol)	= 0;
	// 20081007 JS.Kim	사용자시간대 선택
	virtual CString		GetCurrentRQ( void) = 0;

// (2006/12/12 - Seung-Won, Bae) Support Indicator Calculation Enabling Flag
public:
	virtual void	EnableIndicatorCalculation( BOOL p_bEnableIndicatorCalculation)	= 0;
	virtual BOOL	IsEnableIndicatorCalculation( void) const	= 0;

// (2007/1/20 - Seung-Won, Bae) Search Indicator
public:
	virtual BOOL	FindFirstIndicator( const char *p_szIndicatorName, int &p_nR, int &p_nC, int &p_nV, int &p_nG, CString &p_strGraphName, const char *p_szRQ)	= 0;
	virtual BOOL	FindNextIndicator( const char *p_szIndicatorName, int &p_nR, int &p_nC, int &p_nV, int &p_nG, CString &p_strGraphName, const char *p_szRQ)	= 0;

// (2007/2/20 - Seung-Won, Bae) Support Indicator Hiding
//		It can not be replaced with AddChartBlock(), because of Condition Reset.
public:
	virtual void	ShowIndicator( const char *p_szIndicatorName, BOOL p_bShow, BOOL p_bRecalculate)	= 0;

// (2007/2/22 - Seung-Won, Bae) Support Base Text Option
public:
	virtual void	ShowBaseText( BOOL p_bShow)	= 0;

// (2007/2/22 - Seung-Won, Bae) Support Graph Color Changing with Graph Title
public:
	virtual BOOL	ChangeGraphColor( const char *p_szGraphTitle, const char *p_szColors)	= 0;

// (2007/3/28 - Seung-Won, Bae) Retrieve Packet Name of SubGraph
public:
	virtual CString		GetSubGraphPacketName( const char *p_szGraphName, const char *p_szSubGraphName, const int p_nPacketIndex)	= 0;

// (2007/4/30 - Seung-Won, Bae) Get Data Index of Inverse Line Graph
public:
	virtual int			GetDataIndexOfInverseLine( CPoint p_pt)	= 0;

// (2007/5/23 - Seung-Won, Bae) Support for Only IChartCtrl::SetStartEndIndex in OCX.
//		Use IChartCtrl::SetStartEndIndex() for Invalidation and Notify Zoom Value.
protected:
friend class _ICHARTCTRL_CTRL;
	// 화면에 보일 data의 범위.
	virtual void	SetDataRegionInBlock2( const int start, const int end)	= 0;

// (2007/7/18 - Seung-Won, Bae) Set Up/Down Space for Spacial Chart.
public:
	virtual BOOL	GetVertScaleSpace( const int p_nR, const int p_nC, const int p_nV, int &p_nUpSpace, int &p_nDownSpace)	= 0;
	virtual BOOL	SetVertScaleSpace( const int p_nR, const int p_nC, const int p_nV, const int p_nUpSpace, const int p_nDownSpace)	= 0;

	// 블럭의 VertScaleGroupCount를 얻음 - ojtaso (20080618)
	virtual long	GetVertScaleGroupCount(const CBlockIndex& blockIndex) = 0;
	// 한번에 그래프 속성 변경 가능한 메쏘드 추가 - ojtaso (20080602)
	virtual void	ChangeGraphPropertyAtOnce(LPCTSTR lpszRQ, LPCTSTR lpszTargetTitle, LPCTSTR lpszNewTitle, short nGraphType, short nGraphStyle, short nDrawStyle, LPCTSTR lpszColors, short nThickness1, short nThickness2, short nThickness3) = 0;

// (2008/1/22 - Seung-Won, Bae) for Multi-Language
public:
	virtual HWND	GetParent_Handle( void)								= 0;

// (2009/9/12 - Seung-Won, Bae) for Recalculation of Block Position.
public:
	virtual void	SetHorizontalScaleHeight( short nBlockRow, short nBlockCol, long nHeight) = 0;

// (2009/9/27 - Seung-Won, Bae) Get RQ of Graph.
public:
	virtual const char *	GetRQofGraph( const char *p_szGraphName)	= 0;

	// KTB 체결잔고표시 - ojtaso (20091019)
	virtual long	AddHorizontalLine(LPCSTR lpszRQ, LPCTSTR lpszTitle, OLE_COLOR clrLine, short nLine, double dPrice) = 0;
	virtual void	DeleteHorizontalLine(LPCSTR lpszRQ, long nIndex) = 0;
	virtual void	DeleteAllHorizontalLine(LPCSTR lpszRQ) = 0;
	virtual void	UpdateHorizontalLine(LPCSTR lpszRQ, long nIndex, LPCTSTR lpszTitle, OLE_COLOR clrLine, short nLine, double dPrice) = 0;

	// 한번에 그래프 속성 얻어올 수 메쏘드 추가 - (@Solomon-20100120)
	virtual bool	GetGraphPropertyAtOnce(LPCTSTR lpszRQ, LPCTSTR lpszTargetTitle, short &nGraphType, short &nGraphStyle, short &nDrawStyle, COLORREF &crColors, short &nThickness1, short &nThickness2, short &nThickness3) = 0;

//20100518_솔로몬  해당 지표의 SubGraphIndex의 Hide 속성을 Get
	virtual BOOL GetSubGraphHiding( const CString& strGraphName, const int nSubGraphIndex) = 0;

	//	시간구분선
	virtual CScaleBaseData::SCALEGRIDTYPE GetFirstDailyScaleGridType( CScaleBaseData::SCALEGRIDTIMEGUBUN nType) = 0;
	virtual BOOL GetFirstDailyScaleTime( CScaleBaseData::SCALEGRIDTIMEGUBUN nType) = 0;
	virtual const unsigned short GetFirstBlockGridLineWidth_TimeGubun( int) = 0;

	virtual void SetAllDailyScaleGridType(const CScaleBaseData::SCALEGRIDTYPE gridType, const CScaleBaseData::SCALEGRIDTIMEGUBUN eTimeType, BOOL p_bCheckTime) = 0;
	virtual void SetAllScaleGridWidth_TimeGubun( CScaleBaseData::SCALEGRIDTIMEGUBUN nType, const unsigned short nGridWidth) = 0;

	virtual const unsigned long GetFirstBlockGridLineColor_TimeGubun( int) = 0;
	virtual void SetAllScaleGridColor_TimeGubun( CScaleBaseData::SCALEGRIDTIMEGUBUN nType, const unsigned long nGridColor) = 0;
};

typedef CMainBlock *(*FN_CreateMainBlock)( void); 

///////////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_MAINBLOCK_H__405226D5_F3E1_4335_BC7A_76FA68A1E0A7__INCLUDED_)
