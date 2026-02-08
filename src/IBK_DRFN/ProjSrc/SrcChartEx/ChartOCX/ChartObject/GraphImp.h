// GraphImp.h: interface for the CGraphImp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRAPHIMP_H__6C371F97_C2BC_4CBA_8AB9_E44AD1E3B2A0__INCLUDED_)
#define AFX_GRAPHIMP_H__6C371F97_C2BC_4CBA_8AB9_E44AD1E3B2A0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Include_Chart/Graph.h"
#include "../Include_Chart/GraphData.h"

#include "SubGraphList.h"
#include "ChartObjectBaseData.h"

class CMainChartFormulate;
class CBlockImp;
class CVertScaleGroupList;
class CVertScaleGroup;
class CChartFormulateData;
class CIndicator;
class CPacketRQ;

class CGraphImp : public CGraph
{
public:
	// graph를 생성& 삭제
	static CGraphImp* MakeGraph(CMainBlockBaseData* pMainBlockBaseData, CVertScaleGroup* pVertScaleGroup, const CGraphData& graphData, char *p_szSubGraphData);
	// RQ를 가지고 있는 Graph 생성 : 복수종목 - ojtaso (20061224)
	// CPacketRQ*로 변경 : 복수종목 - ojtaso (20070109)
	static CGraphImp* MakeGraph(LPVOID pPacketRQ, CMainBlockBaseData* pMainBlockBaseData, CVertScaleGroup* pVertScaleGroup, const CGraphData& graphData, char *p_szSubGraphData);
	static void Delete(CGraphImp* pGraph);

protected:
	CGraphImp(CMainBlockBaseData* pMainBlockBaseData, CVertScaleGroup* pVertScaleGroup, const CGraphData& graphData, char *p_szSubGraphData);
	// RQ를 가지고 있는 Graph 생성 : 복수종목 - ojtaso (20061224)
	// CPacketRQ*로 변경 : 복수종목 - ojtaso (20070109)
	CGraphImp(LPVOID pPacketRQ, CMainBlockBaseData* pMainBlockBaseData, CVertScaleGroup* pVertScaleGroup, const CGraphData& graphData, char *p_szSubGraphData);
	~CGraphImp();

public:
	void DeleteGraph();
	void DeletePacket(const bool bIsOnlyNoTRData = true);

	// 지표를 계산한다.
	bool CalculateGraph(const CCalculateBaseData::CALCULATETYPE eCalType, const bool bIsCalMinMax = true, const bool bIsClearPacketData = false, const bool bIsRequestTR = false);
	
	// 초기화 관련 내용.
	void Initialize_MakeGraph();
//	void Initialize_PacketAndIndicatorInfo(const bool bIsCalculate);
	void Initialize_AllPackets(const bool bIsCalculate);
	void Initialize_IndicatorInfo(const bool bIsNULL);

	// 해당 graph와 indicatorInfo 간의 변경.
	// -> gaphData(subGraph포함)를 IndicatorInfo로 변경.
	//    (지표계산과 관련된 내용이 변경된 경우 지표계산 유무)
	bool ChangeGraphDataFromIndicatorInfo(const bool bIsCalculate);
	BOOL ChangeGraphDataFromIndicatorInfo( CIndicatorInfo& indicatorInfo, const bool bIsCalculate);
	// -> IndicatorInfo를 gaphData(subGraph포함)로 변경.
	bool ChangeIndicatorInfoFromGraphData();
	bool ChangeIndicatorInfoFromGraphData( CIndicatorInfo& indicatorInfo);
	// -> graphData 와 indicatorInfo를 모두 변경.
	bool ChangeIndicatorInfoAndGraphData(const CIndicatorInfo* pSourceIndicatorInfo, const bool bIsCalculate);
	// -> 해당 sub graph와 indicatorInfo 간의 변경.
	bool ChangeAllSubGraphTitleFromIndicator();
	bool ChangeSubGraphTitleFromIndicator(const int nSubGraphIndex);
	// -> 해당 graph를 Map Default IndicatorInfo로 변경
	bool ChangeMapDefaultIndicatorInfo( CIndicatorList* pIndicatorList);


	void SetVertScaleGroup(CVertScaleGroup* pVertScaleGroup);
	// graph의 data setting.
//	bool SetGraph(const CGraphData& graphData, const CString& strSubGraphData);
	// 함수명과 packetname을 변경한다.
//	bool SetGraphNameAndPacketName(const CString& strGraphName, const CString& strPacketName);
//	bool SetGraphNameAndSubGraphNames(const CString& strGraphName, const bool bIsSubGraphCheck, const bool bIsCalculate);
//	bool SetGraphName(const CString& strGraphName, const bool bIsCalculate = true);
	bool SetPacketNames(const CString& strPacketName, const bool bIsChangeIndicatorInfo = true, const bool bIsCalculate = true);
	bool SetGraphDataDrawingData(const CGraphDataDrawingData& sourceGraphDrawingData);
	// GraphDataDraw 여부 추가 - ojtaso (20080609)
	bool ShowGraphDataDrawingData(const bool bShow, const int nShowType);

	// 상위 class 주소
	void SetMainBlockBaseData(CMainBlockBaseData* pMainBlockBaseData);
	CMainBlockImp* GetMainBlock() const;
	CBlockImp* GetBlock() const;
	CVertScaleGroupList* GetVertScaleGroupList() const;
	CVertScaleGroup* GetVertScaleGroup() const;

	// graph data
	CGraphData& GetGraphData();
	const CGraphData& GetGraphData() const;
	// graph가 가지고 있는 subGraph들의 있는 list.
	CSubGraphList& GetSubGraphList();
	const CSubGraphList& GetSubGraphList() const;
	CIndicatorInfo* GetIndicatorInfo();
	const CIndicatorInfo* GetIndicatorInfo() const	{	return m_pIndiInfo;	}

	// graph data 가져오기
	void GetStringGraphData( CStringList &p_slGraphData) const;

	// graph title region
	CRect GetGraphTitleRegion(CDC* pDC, const CRect& graphRegion, int& preGraphTitleEndPosition) const;
	// view data setting
	void GetSubGraphViewDataList(CSubGraphViewDataList& subGraphViewDataList) const;

	// 세로 scale의 폭을 구한다.
	int GetLeftVertScaleRegionWidth(CDC* pDC, CVertScaleMaxWidthCalculatorData* pCalData) const;
	int GetRightVertScaleRegionWidth(CDC* pDC, CVertScaleMaxWidthCalculatorData* pCalData) const;
	// 현재 보이는 화면에서의 min, max값 구한다. ==> y축의 값
	bool GetDisplayDataMinMax(const CString& strScaleData, CDisplayDataMinMax& displayDataMinMax, BOOL p_bWithFullRange) const;

	bool IsExistencePacketName(const CList<CString, CString>& packetNameList) const;
	bool IsExistencePacketName(const CString& strPacketName) const;

	// (2009/1/7 - Seung-Won, Bae) Support graph title drawing routine.
	void DrawGraphTitle( CDC* pDC, CDrawingGraphData* pDrawingGraphData, int& graphTitlePosition, int& graphTitleLine, int& nGraphTitleEndLine);
	void DrawGraphAndSignal( CDC* pDC, CDrawingGraphData* pDrawingGraphData);

	LPCTSTR GetGraphRQ() const;						// Graph의 RQ : 복수종목 - ojtaso (20061224)
	void	SetGraphRQ(LPCTSTR lpszRQ);				// RQ 세팅 : 복수종목 - ojtaso (200705015)
	BOOL	ComparePacketRQ(LPVOID pPacketRQ);		// CPacketRQ*로 Graph 체크 : 복수종목 - ojtaso (20070109)
	 // CPacketRQ를 알려줌 : 복수종목 - ojtaso (20070531)
	CPacketRQ*	GetPacketRQ();

	// 타이틀에 종목명 표시 : 복수종목 - ojtaso (20070322)
	void ChangeSubGraphTitle(LPCTSTR lpszPreName, LPCTSTR lpszNewName);

private:
	// Info의 data로 SubGraph를 만든다.
//	bool MakeAllSubGraphFromIndicatorInfo(const bool bIsCalculate);
	// IndicatorInfo 삭제하기
	bool DeleteIndicatorInfo();
	// Packet 삭제 하기
	void DeletePacket_SubGraph(const bool bIsOnlyNoTRData = true);
	void DeletePacket_Signal(const bool bIsOnlyNoTRData = true);
	// Packet Clear 하기
	void ClearPacket(const bool bIsOnlyNoTRData = true);
	void ClearPacket_Signal(const bool bIsOnlyNoTRData = true);
	// Packet 초기화
	void Initialize_AllPackets();
	// Indicator 계산하기
	bool CalculateIndicator(const CCalculateBaseData::CALCULATETYPE eCalType, const bool bIsClearPacketData = false, const bool bIsRequestTR = false);
	bool CalculateIndicator_TR(const CCalculateBaseData::CALCULATETYPE eCalType);
	bool CalculateIndicator_Indicator(const CCalculateBaseData::CALCULATETYPE eCalType);
	void CalculateIndicator_OCX(const CCalculateBaseData::CALCULATETYPE eCalType);
	// TR 요청
	bool SendMessage_TRData();

	bool IsDisplayDataMinMax() const;
	bool IsEqualPacketNames(const CString& strPacketNames) const;
	bool IsDeleteIndicatorInfo() const;
	bool IsSendMessage_TRData() const;
	bool IsSendMessage_NewTRData(const CPacket* pPacket) const;
	bool IsSendMessage_InsertTRData(const CPacket* pPacket) const;
	bool IsSendMessage_InsertTRData(const CPacketList* pPacketList, const int nDataCount) const;

	CPacketList* GetPacketList() const;
	CIndicatorList* GetIndicatorList() const;
	CMainChartFormulate* GetMainChartFormulate() const;
	bool GetChartFormulateData(const CCalculateBaseData::CALCULATETYPE eCalType, CChartFormulateData& chartFormulateData) const;
	CScaleBaseData::VERTSCALEPOSITION GetVertScalePosition(const CString& strVertScalePosition) const;
	// 세로 scale width 구하기.
	int GetVertScaleRegionWidth(CDC* pDC, CVertScaleMaxWidthCalculatorData* pCalData) const;
	// <가격차트>의 color 구하기
	CGraphColor* GetPriceChartColor() const;
	// 양끝에 " "와 ","가 없는 string.
	CString GetNoSpaceCommaString(const CString& strData) const;
	// TR 보낼 count 와 type
	bool GetRequestTRCountAndType(int& nRequestTRCount, bool& bIsNew) const;
	bool GetRequestTRCountAndType_New(int& nRequestTRCount, bool& bIsNew) const;
	bool GetRequestTRCountAndType_Insert(const CPacket* pPacket, int& nRequestTRCount, bool& bIsNew) const;

//	void ChangeGraphData(const CGraphData& sourceGraphData, const bool bIsAutoCalCheck);
//	bool ChangeAllSubGraphData(const bool bIsCheckSubGraphCount, const bool bIsAllChangeSubGraphData);
	bool ChangeAllSubGraphDataFromIndicatorInfo(const CIndicatorInfo* pIndicatorInfo, const bool bIsAllChangeSubGraphData = true);
	bool ChangeIndicatorInfoFromAllSubGraphData(CIndicatorInfo& indicatorInfo);
	void ChangeTitleDraw(CDrawingGraphData* pDrawingGraphData, bool& bOldConditionTitleDraw, bool& bOldDataTitleDraw);
	bool ChangePacketNameInIndicatorInfo(const CString& strPacketName);
	bool ChangeIndicatorInfo(const CIndicatorInfo* pSourceIndicatorInfo);

	// graph를 그리기.
	// 타이틀 여러줄로 표시 : 복수종목 - ojtaso (20070227)
//	void DrawGraph(CDC* pDC, CDrawingGraphData* pDrawingGraphData, int& graphTitlePosition, int& graphTitleLine, int& nGraphTitleEndLine);
	// signal을 그린다.
//	void DrawSignal(CDC* pDC, const CDrawingGraphData& drawingGraphData);
	void CheckWeightedVolume();
	// 특정한 위치에 박스형태를 그림 (유진 2512 화면 요청) - ojtaso (20080709)
	void DrawVerticalBox(CDC* pDC, CDrawingGraphData* pDrawingGraphData);

private:
	CMainBlockBaseData* m_pMainBlockBaseData;
	CVertScaleGroup* m_pVertScaleGroup;
	CIndicatorInfo* m_pIndiInfo;
	CPacket* m_pSignalPacket;
	CGraphData m_graphData;
	CSubGraphList m_subGraphList;	// subGraph들이 list형태로 있는 객체

	//	CString		m_strRQ;		// RQ를 가지고 있는 Graph : 복수종목 - ojtaso (20061224)
	CPacketRQ*	m_pPacketRQ;		// 사용중인 CPacketRQ* : 복수종목 - ojtaso (20070109)
 
// (2006/9/8 - Seung-Won, Bae) the Temp Variables for Calculating 
protected:
	CIndicator *m_pIndicator;

// (2007/2/28 - Seung-Won, Bae) Support Graph Hiding
protected:
	BOOL	m_bHide;
public:
	virtual void	SetGraphHide( BOOL p_bHide)		{	m_bHide = p_bHide;	}
	virtual BOOL	IsGraphHide( void)				{	return m_bHide;		}

	// 한번에 그래프 속성 변경 가능한 메쏘드 추가 - ojtaso (20080602)
	void ChangeGraphPropertyAtOnce(short nSubGraphIndex, short nGraphType, short nGraphStyle, short nDrawStyle, short nThickness1, short nThickness2, short nThickness3);

// 20081007 JS.Kim	사용자시간대 선택
public:
	LPCTSTR			GetPacketTimeDiffName();
	int				GetPacketOrgBaseTimeDiff();
	void			SetPacketTimeDiff(CString strTimeName, int nTimeDiff);

	// KTB 체결잔고표시 - ojtaso (20091019)
	long	AddHorizontalLine(LPCTSTR lpszTitle, OLE_COLOR clrLine, short nLine, double dPrice);
	void	DeleteHorizontalLine(long nIndex);
	void	DeleteAllHorizontalLine();
	void	UpdateHorizontalLine(long nIndex, LPCTSTR lpszTitle, OLE_COLOR clrLine, short nLine, double dPrice);

// (2009/10/9 - Seung-Won, Bae) for Thread Draw.
public:
	CDC *				m_pDC;
	CDrawingGraphData *	m_pDrawingGraphData;

	// 한번에 그래프 속성 얻어올 수 메쏘드 추가 - (@Solomon-20100120)
	bool GetGraphPropertyAtOnce( short nSubGraphIndex, short &nGraphType, short &nGraphStyle, short &nDrawStyle, short &nThickness1, short &nThickness2, short &nThickness3);
};

#endif // !defined(AFX_GRAPHIMP_H__6C371F97_C2BC_4CBA_8AB9_E44AD1E3B2A0__INCLUDED_)
