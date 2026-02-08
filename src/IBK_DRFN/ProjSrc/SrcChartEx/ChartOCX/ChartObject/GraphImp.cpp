// GraphImp.cpp: implementation of the CGraphImp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GraphImp.h"

#include <assert.h>

#include "../Include_Chart/Dll_Load/PacketList.h"				// PacketData.dll
#include "../Include_Chart/Dll_Load/PacketBase.h"				// PacketData.dll
#include "../Include_Chart/Dll_Load/PacketListManager.h"		// for CPacketListManager
#include "../Include_Chart/Dll_Load/MainChartFormulate.h"		// ChartFormulate.dll
#include "../Include_Chart/Dll_Load/IndicatorList.h"			// IndicatorCondition.dll
#include "../Include_Chart/Dll_Load/IndicatorInfo.h"			// IndicatorCondition.dll
#include "../Include_Chart/Dll_Load/GraphDrawer.h"				// GraphDrawer.dll
#include "../Include_Chart/Dll_Load/IMetaTable.h"				// for _MTEXT()
#include "../Include_Chart/Dll_Load/XScaleManager.h"			// for CXScaleManager
#include "../Include_Chart/ChartFormulateData.h"
#include "../Include_Chart/IChartCtrl.h"						// for IChartCtrl
#include "../Include_Chart/Indicator.h"							// for CIndicator
#include "../Include_Chart/Conversion.h"
#include "../Include_Chart/PacketPointerList.h"
#include "../Include_Chart/MessageDefine.h"						// for UDM_RECALC_DRI
#include "MainBlockImp.h"
#include "BlockImp.h"
#include "VertScaleGroup.h"
#include "CalculatorData.h"
#include "GraphSearchInfo.h"									// for CGraphSearchInfo_IndicatorObject
#include "SubGraph.h"											// for CSubGraph
#include "PacketRQ.h"											// for CPacketRQ

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// graph를 객체로 만든다.
CGraphImp* CGraphImp::MakeGraph(CMainBlockBaseData* pMainBlockBaseData, CVertScaleGroup* pVertScaleGroup, const CGraphData& graphData, char *p_szSubGraphData)
{
	return new CGraphImp(pMainBlockBaseData, pVertScaleGroup, graphData, p_szSubGraphData);
}

// RQ를 가지고 있는 Graph 생성 : 복수종목 - ojtaso (20061224)
// CPacketRQ*로 변경 : 복수종목 - ojtaso (20070109)
CGraphImp* CGraphImp::MakeGraph(LPVOID pPacketRQ, CMainBlockBaseData* pMainBlockBaseData, CVertScaleGroup* pVertScaleGroup, const CGraphData& graphData, char *p_szSubGraphData)
{
	return new CGraphImp(pPacketRQ, pMainBlockBaseData, pVertScaleGroup, graphData, p_szSubGraphData);
}
 
// 객체를 삭제한다. Make에서 new를 했으므로...
void CGraphImp::Delete(CGraphImp* pGraph)
{
	if( !pGraph) return;

	delete pGraph;
}

/////////////////////////////////////////////////////////////////////////////////////////////
CGraphImp::CGraphImp(CMainBlockBaseData* pMainBlockBaseData, CVertScaleGroup* pVertScaleGroup, const CGraphData& graphData, char *p_szSubGraphData):
	m_pMainBlockBaseData(pMainBlockBaseData),
	m_pVertScaleGroup(pVertScaleGroup),
	m_pIndiInfo(NULL),
	m_pSignalPacket(NULL),
	m_graphData(graphData),
	m_pPacketRQ(NULL)	// 기본 RQ를 가지고 있는 Graph : 복수종목 - ojtaso (20061224)
{
	// (2007/2/28 - Seung-Won, Bae) Support Graph Hiding
	// (2007/3/5 - Seung-Won, Bae) Init before Initialize_MakeGraph() for ST Calculation.
	m_bHide = FALSE;

	// (2006/9/8 - Seung-Won, Bae) the Temp Variables for Calculating 
	m_pIndicator = NULL;

	m_subGraphList.SetMainBlockBaseData(pMainBlockBaseData);
	m_subGraphList.SetGraph(this);

	m_subGraphList.MakeSubGraphs( p_szSubGraphData, m_graphData.GetIndicatorName());

	Initialize_MakeGraph();
}

// RQ를 가지고 있는 Graph 생성 : 복수종목 - ojtaso (20061224)
// CPacketRQ*로 변경 : 복수종목 - ojtaso (20070109)
CGraphImp::CGraphImp(LPVOID pPacketRQ, CMainBlockBaseData* pMainBlockBaseData, CVertScaleGroup* pVertScaleGroup, const CGraphData& graphData, char *p_szSubGraphData):
	m_pMainBlockBaseData(pMainBlockBaseData),
	m_pVertScaleGroup(pVertScaleGroup),
	m_pIndiInfo(NULL),
	m_pSignalPacket(NULL),
	m_graphData(graphData),
	m_pPacketRQ((CPacketRQ*)pPacketRQ)	// 기본 RQ를 가지고 있는 Graph : 복수종목 - ojtaso (20061224)
{
	// (2007/2/28 - Seung-Won, Bae) Support Graph Hiding
	// (2007/3/5 - Seung-Won, Bae) Init before Initialize_MakeGraph() for ST Calculation.
	m_bHide = FALSE;

	m_pPacketRQ->AddRQGraph(this);

	m_pIndicator = NULL;

	m_subGraphList.SetMainBlockBaseData(pMainBlockBaseData);
	m_subGraphList.SetGraph(this);

	// 차트DLL에서 복수종목을 사용할지 : 복수종목 - ojtaso (20070904)
	if(pMainBlockBaseData->GetMainAttributes().GetMainBlockImpl()->GetMultiItemMode())
	{
		// 타이틀에 종목명 표시 : 복수종목 - ojtaso (20070322)
		// 종목명 표시 : 복수종목 - ojtaso (20070625)
		LPCTSTR lpszItemName = m_pPacketRQ->GetItemName();
		m_subGraphList.MakeSubGraphs( p_szSubGraphData, m_graphData.GetIndicatorName(), lpszItemName);
	}
	else
		m_subGraphList.MakeSubGraphs( p_szSubGraphData, m_graphData.GetIndicatorName());

	Initialize_MakeGraph();
}

// 타이틀에 종목명 표시 : 복수종목 - ojtaso (20070322)
void CGraphImp::ChangeSubGraphTitle(LPCTSTR lpszPreName, LPCTSTR lpszNewName)
{
	// 종목명 표시 : 복수종목 - ojtaso (20070625)
	if(!lpszPreName || !lpszNewName/* || m_graphData.GetIndicatorName2() != _MTEXT( C2_PRICE_CHART)*/)
		return;

	m_subGraphList.ChangeSubGraphTitle(lpszPreName, lpszNewName);
}

CGraphImp::~CGraphImp()
{
	//indicatorList에서 info를 삭제한다. (옵션에 따라..)
	DeleteIndicatorInfo();

	// (2006/9/8 - Seung-Won, Bae) the Temp Variables for Calculating 
	if( m_pIndicator) 
	{
		delete m_pIndicator;
		m_pIndicator = NULL;
	}

 	if( m_pPacketRQ)
 	{
		// (2009/9/20 - Seung-Won, Bae) Unlink first to avoid the access it on next processing.
		CPacketRQ *pPacketRQ = m_pPacketRQ;
 		m_pPacketRQ = NULL;

 		pPacketRQ->RemoveRQGraph(this);
 		if( pPacketRQ->IsEmptyRQGraph() && m_pMainBlockBaseData)
		{
		//KHD	CString szRQ = pPacketRQ->GetRQ();
		//KHD	if(szRQ.IsEmpty()==FALSE)
				m_pMainBlockBaseData->GetPacketListManager()->DeleteChartItem( pPacketRQ->GetRQ());
		}
 	}
}


// public ==================================================================================
void CGraphImp::DeleteGraph()
{
	if(m_pVertScaleGroup == NULL)
		return;

	m_pVertScaleGroup->DeleteGraphWithNotify( this);
}

// bIsOnlyNoTRData = true : TR Data가 아닌 경우만 삭제.
void CGraphImp::DeletePacket(const bool bIsOnlyNoTRData)
{
	DeletePacket_SubGraph(bIsOnlyNoTRData);
	DeletePacket_Signal(bIsOnlyNoTRData);
}

// 지표를 계산한다.
// 지표설정Dlg 에서 <확인>을 누른 경우
bool CGraphImp::CalculateGraph(const CCalculateBaseData::CALCULATETYPE eCalType, const bool bIsCalMinMax, const bool bIsClearPacketData, const bool bIsRequestTR)
{
	// (2006/12/12 - Seung-Won, Bae) Skip Calculation for Initializing Speed.
	if( !m_pMainBlockBaseData) return false;
	if( !m_pMainBlockBaseData->IsEnableIndicatorCalculation()) return false;

	//Initialize_IndicatorInfo(false);
	if(!CalculateIndicator(eCalType, bIsClearPacketData, bIsRequestTR))
		return false;

	if(bIsCalMinMax)
		m_pVertScaleGroup->CalculateDisplayDataMinMax();

	return true;
}

// ----------------------------------------------------------------------------------------
// graph 생성시 초기화할 내용
void CGraphImp::Initialize_MakeGraph()
{
	Initialize_IndicatorInfo( false);
	CalculateGraph( CCalculateBaseData::TRDATA_CAL, true, false, true);
}

/*
// packet* 와 indicatorInfo*를 수정한다.
void CGraphImp::Initialize_PacketAndIndicatorInfo(const bool bIsCalculate)
{
	Initialize_IndicatorInfo(false);
	Initialize_AllPackets(bIsCalculate);
}
*/

// packet*만 수정한다.
void CGraphImp::Initialize_AllPackets(const bool bIsCalculate)
{
	Initialize_AllPackets();

	if(bIsCalculate)
		CalculateGraph(CCalculateBaseData::TRDATA_CAL);
}

// indicatorInfo*만 수정한다.
void CGraphImp::Initialize_IndicatorInfo(const bool bIsNULL)
{
	m_pIndiInfo = NULL;

	if(bIsNULL)
		return;

	CIndicatorList* pIndicatorList = GetIndicatorList();
	// (2006/12/6 - Seung-Won, Bae) Support Multi-Indicator Info.
	if(pIndicatorList != NULL)
	{
		m_pIndiInfo = pIndicatorList->GetGraphIndicatorInfo( EILT_USER, m_graphData.GetGraphName2(), m_graphData.GetIndicatorName2(), EIIGT_WITH_DEFAULT_GET_AND_SET);
		TRACE( "Graph IndicatorInfo Binding : [%s](%s) ~ 0x%08X\r\n", m_graphData.GetIndicatorName2(), m_graphData.GetGraphName2(), m_pIndiInfo);
		
		// CGraphImp : 복수종목 - ojtaso (20070308)
		if( m_pIndiInfo)
		{
			m_pIndiInfo->SetGraph(this);

			int nCount = m_subGraphList.GetSubGraphCount();
			for(int nIndex = 0; nIndex < nCount; nIndex++)
			{
				CSubGraphData *pIndiData = m_pIndiInfo->GetSubGraphData( nIndex);
				CSubGraphData *pGraphData = m_subGraphList.GetSubGraphData( nIndex);
				if( !pIndiData || !pGraphData) continue;

				pIndiData->SetItemName( pGraphData->GetItemName());

				// (2007/12/17 - Seung-Won, Bae) Update Graph Type and Style of IndiCond with Graph Data of Design Time.
				pIndiData->SetType( pGraphData->GetType());
				pIndiData->SetStyle( pGraphData->GetStyle());
				pIndiData->SetDrawStyle( pGraphData->GetDrawStyle());
			}
		}
	}
}

// ----------------------------------------------------------------------------------------
// 해당 graph와 indicatorInfo 간의 변경.
// -> gaphData(subGraph포함)를 IndicatorInfo로 변경.
//	  (title 은 조건값에 해당되는지 확인하여 변경하기)
//    (지표계산과 관련된 내용이 변경된 경우 지표계산 유무)
// (2006/11/27 - Seung-Won, Bae) Do not use SetGraphName() & SetIndicatorName()
bool CGraphImp::ChangeGraphDataFromIndicatorInfo(const bool bIsCalculate)
{
	if( !m_pIndiInfo) return false;
	if( m_subGraphList.GetSubGraphCount() != m_pIndiInfo->GetSubGraphCount()) return false;

	// subGraph count가 같을 경우 -> subGraphData 만 수정
	CList<CSubGraphData*, CSubGraphData*> sourceSubGraphDataList;
	if( !m_pIndiInfo->GetAllSubGraphDataList( sourceSubGraphDataList)) return false;
	if( !m_subGraphList.ChangeAllSubGraphData( sourceSubGraphDataList, true)) return false;

	// graphData 변경
	m_graphData = m_pIndiInfo->GetGraphData();

	//지표 계산 -> subGraph 개수가 틀려질 경우을 대비한다.
	if(bIsCalculate) Initialize_AllPackets(true);
	
	//{{ (2007/5/28 - LYH) For Weighted Volume Chart
	CheckWeightedVolume();
	//}}

	// (2009/1/11 - Seung-Won, Bae) for X Scale Manager's Calc_DRI
	CString strIndicatorName = m_pIndiInfo->GetIndicatorName();
	if( g_iMetaTable.IsSpecialGroupGraph( strIndicatorName) || strIndicatorName == _MTEXT( C2_OBVIOUSLY_BALANCE_CHART))
	{
		if( strIndicatorName == _MTEXT( C2_OBVIOUSLY_BALANCE_CHART))
		{
			// xScaleManager/복수종목 적용 - xScaleManager - ojtaso (20070404)
			CString strRQ = GetGraphRQ();
			CXScaleManager* pxScaleManager = GetMainBlock()->GetXScaleManager();
			if( pxScaleManager)
			{
				pxScaleManager->DelOEBTRQ( strRQ);
				pxScaleManager->AddOEBTRQ( strRQ, m_graphData.GetGraphName(), m_graphData.GetIndicatorName());
			}
		}
		::PostMessage( m_pMainBlockBaseData->GetMainAttributes().GetParent_Handle(), UDM_RECALC_DRI, NULL, NULL);
	}

	return true;
}

BOOL CGraphImp::ChangeGraphDataFromIndicatorInfo( CIndicatorInfo& indicatorInfo, const bool bIsCalculate)
{
	if( m_subGraphList.GetSubGraphCount() != indicatorInfo.GetSubGraphCount()) return false;

	// subGraph count가 같을 경우 -> subGraphData 만 수정
	CList<CSubGraphData*, CSubGraphData*> sourceSubGraphDataList;
	if( !indicatorInfo.GetAllSubGraphDataList( sourceSubGraphDataList)) return false;
	if( !m_subGraphList.ChangeAllSubGraphData( sourceSubGraphDataList, true)) return false;

	// graphData 변경
	m_graphData = indicatorInfo.GetGraphData();

	//지표 계산 -> subGraph 개수가 틀려질 경우을 대비한다.
	if(bIsCalculate) Initialize_AllPackets(true);
	return true;
}

// -> IndicatorInfo를 gaphData(subGraph포함)로 변경.
bool CGraphImp::ChangeIndicatorInfoFromGraphData()
{
	if(m_pIndiInfo == NULL)
		Initialize_IndicatorInfo(false);

	if(m_pIndiInfo == NULL)
		return false;

	return ChangeIndicatorInfoFromGraphData(*m_pIndiInfo);
}

bool CGraphImp::ChangeIndicatorInfoFromGraphData(CIndicatorInfo& indicatorInfo)
{
	if(!ChangeIndicatorInfoFromAllSubGraphData(indicatorInfo))
		return false;

	indicatorInfo.GetGraphData() = m_graphData;

	//{{ (2007/5/28 - LYH) For Weighted Volume Chart
	CheckWeightedVolume();
	//}}
	return true;
}

/*
// -> graphData 와 indicatorInfo를 모두 변경.
bool CGraphImp::ChangeIndicatorInfoAndGraphData(const CIndicatorInfo* pSourceIndicatorInfo, const bool bIsCalculate)
{
	if(pSourceIndicatorInfo == NULL)
		return false;

	//indicatorInfo 바꾸기
	bool bResult = ChangeIndicatorInfo(pSourceIndicatorInfo);

	//지표가 다시 계산 될지 모르므로 subGraph 부분을 먼저 수정한다.
	if(!ChangeAllSubGraphDataFromIndicatorInfo(pSourceIndicatorInfo))
		return bResult;

	ChangeGraphData(const_cast<CIndicatorInfo*>(pSourceIndicatorInfo)->GetGraphData(), false);
	if(bIsCalculate)
		CalculateIndicator(CCalculateBaseData::TRDATA_CAL, true);
	
	return true;
}
*/

// -> 해당 sub graph와 indicatorInfo 간의 변경.
bool CGraphImp::ChangeAllSubGraphTitleFromIndicator()
{
	if(m_pIndiInfo == NULL)
		return false;

	bool bResult = false;
	int nCount = m_subGraphList.GetSubGraphCount();
	for(int nIndex = 0; nIndex < nCount; nIndex++)
	{
		if(ChangeSubGraphTitleFromIndicator(nIndex))
			bResult = true;
	}
	return bResult;
}

bool CGraphImp::ChangeSubGraphTitleFromIndicator(const int nSubGraphIndex)
{
	if(m_pIndiInfo == NULL)
		return false;

	return m_subGraphList.SetTitle(nSubGraphIndex, m_pIndiInfo->GetSubGraphTitle(nSubGraphIndex));
}

// -> 해당 graph를 Map Default IndicatorInfo로 변경
bool CGraphImp::ChangeMapDefaultIndicatorInfo( CIndicatorList* pIndicatorList)
{
	if(pIndicatorList == NULL) return false;

	// (2006/12/6 - Seung-Won, Bae) Support Multiple-Indicator.
	CIndicatorInfo* pMapDefaultInfo = pIndicatorList->GetGraphIndicatorInfo( EILT_MAP_DEFAULT, m_graphData.GetGraphName2(), m_graphData.GetIndicatorName2(), EIIGT_WITH_DEFAULT_GET_AND_SET);
	if( pMapDefaultInfo == NULL) return false;

	// CGraphImp : 복수종목 - ojtaso (20070308)
//	pMapDefaultInfo->SetGraph(this);

	return ChangeIndicatorInfoFromGraphData( *pMapDefaultInfo);
}

// ----------------------------------------------------------------------------------------
void CGraphImp::SetVertScaleGroup(CVertScaleGroup* pVertScaleGroup)
{
	m_pVertScaleGroup = pVertScaleGroup;
}

/*
// graph data setting.
// 이미 생성되어 있는 graph에 data를 setting.
bool CGraphImp::SetGraph(const CGraphData& graphData, const CString& strSubGraphData)
{
	//testing ->functionIndex 도 copy??
	if(strSubGraphData.IsEmpty()) // || graphData.GetFunctionIndex() < 0
		return false;

	// subGraph를 먼저 setting 해야 한다.
	// ChangeGraphData() 에서 지표를 다시 계산하는 경우가 있으므로..
	m_subGraphList.SetSubGraphs(strSubGraphData);

	// graph data 부분 setting.
	ChangeGraphData(graphData, true);
	return true;
}
*/

/*
// 함수명과 packetname을 변경한다.
bool CGraphImp::SetGraphNameAndPacketName(const CString& strGraphName, const CString& strPacketName)
{
	bool bResult = false;
	// 함수명 변경
	if(SetGraphName(strGraphName, false))
		bResult = true;
	// packet명 변경
	if(SetPacketNames(strPacketName, true, false))
		bResult = true;

	// 함수명, packet 명이 바뀔 경우는 계산한다.
	if(bResult)
		Initialize_PacketAndIndicatorInfo(true);

	return bResult;
}
*/

/*
// 함수명과 subGraphName 변경한다.
// bIsSubGraphCheck == true 일때만 subGraphName 비교한다.
// 함수명 비교 -> subGraph 비교 -> 지표 계산
// 함수명, subGraph 변경 -> 지표 계산
// subGraphC const 변경되면 Info를 통해 graph 다시 생성.
bool CGraphImp::SetGraphNameAndSubGraphNames(const CString& strGraphName, const bool bIsSubGraphCheck, const bool bIsCalculate)
{
	// 함수명 변경 -> 지표계산은 subGraphName check후에 한다.
	// 함수명이 변경되지 않은 경우 : subGraph를 체크하지 않은 경우만 return 함.
	if(!SetGraphName(strGraphName, false) && !bIsSubGraphCheck)
		return false;

	// subGraphName 변경 -> subGraph를 새로 만든다.
	if(bIsSubGraphCheck)
	{
		ChangeAllSubGraphData(true, false);
	}

	// 지표 계산 -> 함수명이나 subGraph 를 새로 만들어도 계산을 제일 나중에 한다.
	//				계산의 중복성 피하기 위함.
	Initialize_PacketAndIndicatorInfo(bIsCalculate);
	return true;
}
*/

/*
bool CGraphImp::SetGraphName(const CString& strGraphName, const bool bIsCalculate)
{
	if(m_graphData.GetGraphName() == strGraphName)
		return false;

	m_graphData.SetGraphName(strGraphName);
	Initialize_PacketAndIndicatorInfo(bIsCalculate);
	return true;
}
*/

bool CGraphImp::SetPacketNames(const CString& strPacketName, const bool bIsChangeIndicatorInfo, const bool bIsCalculate)
{
	if(IsEqualPacketNames(strPacketName))
		return false;

	m_graphData.SetPacketNames(strPacketName);
	if(bIsChangeIndicatorInfo)
		ChangePacketNameInIndicatorInfo(strPacketName);

	Initialize_AllPackets(bIsCalculate);
	return true;
}

bool CGraphImp::SetGraphDataDrawingData(const CGraphDataDrawingData& sourceGraphDrawingData)
{
	// (2007/2/22 - Seung-Won, Bae) Do not support the Up/Down Info, But only Candle Graph
	CGraphDataDrawingData graphDrawingData( sourceGraphDrawingData);
	CSubGraph *pSubGraph = m_subGraphList.GetSubGraph( 0);
	if( pSubGraph) if( pSubGraph->GetSubGraphData().GetType() != CGraphBaseData::Bong_Type)
		if( graphDrawingData.GetCalcRiseFallRate_CurData() < 3)
			graphDrawingData.SetCalcRiseFallRate_CurData( graphDrawingData.GetCalcRiseFallRate_CurData() + 2);
	
	m_graphData.SetGraphDataDrawingData( graphDrawingData);
	if(m_pIndiInfo != NULL)
		m_pIndiInfo->GetGraphData().SetGraphDataDrawingData( graphDrawingData);

	return true;
}

// GraphDataDraw 여부 추가 - ojtaso (20080609)
bool CGraphImp::ShowGraphDataDrawingData(const bool bShow, const int nShowType)
{
	CGraphDataDrawingData graphDrawingData = m_graphData.GetGraphDataDrawingData();
	
	graphDrawingData.SetDoesCalcRiseFallRate_HighCur(bShow);
	graphDrawingData.SetDoesCalcRiseFallRate_LowCur(bShow);
	graphDrawingData.SetDoesCalcRiseFallRate_LowHigh(bShow);

	graphDrawingData.SetCalcRiseFallRate_CurData(nShowType);

	m_graphData.SetGraphDataDrawingData( graphDrawingData);
	if(m_pIndiInfo != NULL)
		m_pIndiInfo->GetGraphData().SetGraphDataDrawingData( graphDrawingData);

	return true;
}

// ----------------------------------------------------------------------------------------
// 상위 class 주소
void CGraphImp::SetMainBlockBaseData(CMainBlockBaseData* pMainBlockBaseData)
{
	m_pMainBlockBaseData = pMainBlockBaseData;

	m_subGraphList.SetMainBlockBaseData(m_pMainBlockBaseData);
}

CMainBlockImp* CGraphImp::GetMainBlock() const
{
	if(m_pMainBlockBaseData == NULL)
		return NULL;

	return m_pMainBlockBaseData->GetMainAttributes().GetMainBlockImpl();
}

CBlockImp* CGraphImp::GetBlock() const
{
	if(m_pVertScaleGroup == NULL)
		return NULL;
	return m_pVertScaleGroup->GetBlock();
}

CVertScaleGroupList* CGraphImp::GetVertScaleGroupList() const
{
	if(m_pVertScaleGroup == NULL)
		return NULL;
	return m_pVertScaleGroup->GetVertScaleGroupList();
}

CVertScaleGroup* CGraphImp::GetVertScaleGroup() const
{
	return m_pVertScaleGroup;
}

// graph data
CGraphData& CGraphImp::GetGraphData()
{
	return m_graphData;
}

const CGraphData& CGraphImp::GetGraphData() const
{
	return m_graphData;
}

// graph가 가지고 있는 subGraph들의 있는 list.
CSubGraphList& CGraphImp::GetSubGraphList()
{
	return m_subGraphList;
}

const CSubGraphList& CGraphImp::GetSubGraphList() const
{
	return m_subGraphList;
}

CIndicatorInfo* CGraphImp::GetIndicatorInfo()
{
	return m_pIndiInfo;
}

// ----------------------------------------------------------------------------------------
// graph data 가져오기
void CGraphImp::GetStringGraphData( CStringList &p_slGraphData) const
{
	//>> JS.Kim_20100929  solomon.. PAIR RQ는 저장하지 않도록 수정..
	if(m_pPacketRQ)
	{
		CString strRQ = m_pPacketRQ->GetRQ();
		if( strRQ.GetLength() > 5 && strRQ.Right(5) == "_PAIR" )
		{
			return;
		}
	}
	//<<
	p_slGraphData.AddTail( "Graph;\r\n");

	p_slGraphData.AddTail( CString());
	CString &strGraphData = p_slGraphData.GetTail();

	// 복수종목 저장 : 복수종목 - ojtaso (20070507)
	if(m_pPacketRQ)
		m_graphData.GetData( strGraphData, m_pPacketRQ->GetRQ());
	else
		m_graphData.GetData( strGraphData);

	m_subGraphList.GetSubGraphsData( p_slGraphData);


	p_slGraphData.AddTail( "GraphEnd;\r\n");
}

// ----------------------------------------------------------------------------------------
// graph title region
CRect CGraphImp::GetGraphTitleRegion(CDC* pDC, const CRect& graphRegion, int& preGraphTitleEndPosition) const
{
	if(m_subGraphList.AreHidingAllSubGraphs())
		return CRect(0, 0, 0, 0);

	CString subGraphTitle = m_subGraphList.GetViewableSubGraphTitles();
	if(subGraphTitle.IsEmpty())
		return CRect(0, 0, 0, 0);

	CSize textSize = pDC->GetTextExtent(subGraphTitle);

	CRect titleRegion;
	titleRegion.left = graphRegion.left + preGraphTitleEndPosition + MARGIN;
	titleRegion.top = graphRegion.top + MARGIN;
	int totalMargin = (m_subGraphList.GetViewableSubGraphCount()-1)*MARGIN*3;
	titleRegion.right = titleRegion.left + textSize.cx + totalMargin;
	titleRegion.bottom = titleRegion.top + textSize.cy;

	preGraphTitleEndPosition = titleRegion.right - graphRegion.left + MARGIN*2;

	return titleRegion;
}

// ----------------------------------------------------------------------------------------
// view data setting
void CGraphImp::GetSubGraphViewDataList(CSubGraphViewDataList& subGraphViewDataList) const
{
	// (2010/6/24 - Seung-Won, Bae) Skip overhanging
	if( m_graphData.GetIndicatorName() == _MTEXT( C2_OVERHANGING_SUPPLY)) return;
	
	m_subGraphList.GetSubGraphViewDataList(m_graphData.GetGraphName2(), m_graphData.GetPacketNames(), subGraphViewDataList);
}

// ----------------------------------------------------------------------------------------
// 세로 scale의 폭을 구한다.
int CGraphImp::GetLeftVertScaleRegionWidth(CDC* pDC, CVertScaleMaxWidthCalculatorData* pCalData) const
{
	// graph 숨김, <Data 표현형> 일 경우엔 구하지 않는다.
	if(m_subGraphList.AreHidingAllSubGraphs() || m_subGraphList.IsViewableGraphType(CGraphBaseData::DataView_Type, 0))
		return 0;

	return GetVertScaleRegionWidth(pDC, pCalData);
}

int CGraphImp::GetRightVertScaleRegionWidth(CDC* pDC, CVertScaleMaxWidthCalculatorData* pCalData) const
{
	// graph 숨김, <Data 표현형> 일 경우엔 구하지 않는다.
	if(m_subGraphList.AreHidingAllSubGraphs() || m_subGraphList.IsViewableGraphType(CGraphBaseData::DataView_Type, 0))
		return 0;

	return GetVertScaleRegionWidth(pDC, pCalData);
}

// ----------------------------------------------------------------------------------------
// 현재 보이는 화면에서의 min, max값 구한다. ==> y축의 값
bool CGraphImp::GetDisplayDataMinMax(const CString& strScaleData, CDisplayDataMinMax& displayDataMinMax, BOOL p_bWithFullRange) const
{
	if(m_pMainBlockBaseData == NULL || !IsDisplayDataMinMax())
		return false;

	CVertDisplayDataMinMaxInputData inputData( m_graphData.GetIndicatorName(), m_graphData.GetGraphName2(), strScaleData, m_pMainBlockBaseData->GetDisplayAttributes());

	return m_subGraphList.GetDisplayDataMinMax(&inputData, displayDataMinMax, p_bWithFullRange);
}

// packet명의 존재여부
bool CGraphImp::IsExistencePacketName(const CList<CString, CString>& packetNameList) const
{
	if(packetNameList.GetCount() <= 0)
		return false;

	POSITION pos = packetNameList.GetHeadPosition();
	while(pos != NULL)
	{
		if(IsExistencePacketName(packetNameList.GetNext(pos)))
			return true;
	}
	return false;
}

bool CGraphImp::IsExistencePacketName(const CString& strPacketName) const
{
	if(strPacketName.IsEmpty())
		return false;

	// Find 로 할 경우 잘못된 결과를 return 할 수 있다.
	// ex) strPacketName == "시가", graphPacketNames = "PF_시가" 인경우 -> return : true
	// 이런 경우는 잘못된 것임. 이걸 막기 위해 "," 와 같이 비교한다.

	CString strGraphPacketNames = m_graphData.GetPacketNames();
	if(strGraphPacketNames.IsEmpty() || strGraphPacketNames.Find(strPacketName) < 0)
		return false;

	// graphPacket 이 하나일 경우
	if(strGraphPacketNames.Find(",") < 0){
		return (strGraphPacketNames == strPacketName);
	}
	
	// graphPacket 이 두개이상일 경우
	if(strGraphPacketNames.Right(1) != ",")
		strGraphPacketNames += ",";

	while(!strGraphPacketNames.IsEmpty()){
		CString strTemp = CDataConversion::GetStringData(strGraphPacketNames, ",");
		if(strTemp == strPacketName)
			return true;
	}

	return false;
}

// (2009/1/7 - Seung-Won, Bae) Support graph title drawing routine.
void CGraphImp::DrawGraphTitle( CDC* pDC, CDrawingGraphData* pDrawingGraphData, int& graphTitlePosition, int& graphTitleLine, int& nGraphTitleEndLine)
{
	if(pDrawingGraphData == NULL || m_subGraphList.AreHidingAllSubGraphs())
		return;

	CDataConversion::SelectClipRegion( pDC, pDrawingGraphData->GetGraphRegions().GetNoFrameRegion());
	pDrawingGraphData->SetGraphName( m_graphData.GetGraphName2());
	pDrawingGraphData->SetPacketNames( m_graphData.GetPacketNames());
	pDrawingGraphData->SetGraphDataDrawingData(m_graphData.GetGraphDataDrawingData());

	// 타이틀 여러줄로 표시 : 복수종목 - ojtaso (20070227)
	bool bOldConditionTitleDraw = false, bOldDataTitleDraw = false;
	ChangeTitleDraw( pDrawingGraphData, bOldConditionTitleDraw, bOldDataTitleDraw);
	pDrawingGraphData->SetIndicatorInfo(m_pIndiInfo);
	pDrawingGraphData->SetPriceChartColor(GetPriceChartColor());
	
	// PacketList 적용 : 복수종목 - ojtaso (20070214)
	if( m_pPacketRQ) pDrawingGraphData->SetPacketList(m_pPacketRQ->GetPacketList());

	m_subGraphList.DrawSubGraphTitles( pDC, pDrawingGraphData, graphTitlePosition, graphTitleLine, nGraphTitleEndLine);

	pDrawingGraphData->SetTitleDraw( bOldConditionTitleDraw, bOldDataTitleDraw);

	CDataConversion::SelectClipRegion(pDC);
}

// 그래프 그림 (20071007)
void CGraphImp::DrawGraphAndSignal( CDC* pDC, CDrawingGraphData* pDrawingGraphData)
{
	if(pDrawingGraphData == NULL || m_subGraphList.AreHidingAllSubGraphs())
		return;

	CDataConversion::SelectClipRegion(pDC, pDrawingGraphData->GetGraphRegions().GetNoFrameRegion());
	pDrawingGraphData->SetGraphName( m_graphData.GetGraphName2());
	pDrawingGraphData->SetPacketNames(m_graphData.GetPacketNames());
	pDrawingGraphData->SetGraphDataDrawingData(m_graphData.GetGraphDataDrawingData());

	pDrawingGraphData->SetIndicatorInfo(m_pIndiInfo);
	pDrawingGraphData->SetPriceChartColor(GetPriceChartColor());

	// PacketList 적용 : 복수종목 - ojtaso (20070214)
	if(m_pPacketRQ) pDrawingGraphData->SetPacketList(m_pPacketRQ->GetPacketList());

	m_subGraphList.DrawSubGraphs(pDC, pDrawingGraphData);

	// 특정한 위치에 박스형태를 그림 (유진 2512 화면 요청) - ojtaso (20080709)
	DrawVerticalBox(pDC, pDrawingGraphData);

	pDrawingGraphData->SetSubGraphPacketList(m_subGraphList.GetSubGraphPacketList());

	if( m_pSignalPacket && !m_graphData.GetIndicatorName2().IsEmpty() && m_graphData.GetSignalData().IsDraw())
	{
		// xScaleManager/복수종목 적용 - xScaleManager - ojtaso (20070404)
		CString strRQ = GetGraphRQ();
		CXScaleManager* pxScaleManager = GetMainBlock()->GetXScaleManager();

		CGraphDrawer().DrawSignal( pDC, *pDrawingGraphData, m_pSignalPacket->GetnumericDataList(), m_graphData.GetSignalData(), strRQ, pxScaleManager);
	}

	CDataConversion::SelectClipRegion(pDC);
}

// 특정한 위치에 박스형태를 그림 (유진 2512 화면 요청) - ojtaso (20080709)
void CGraphImp::DrawVerticalBox(CDC* pDC, CDrawingGraphData* pDrawingGraphData)
{
	CXScaleManager* pxScaleManager = GetMainBlock()->GetXScaleManager();

	COLORREF clrBox;
	BOOL bFill = -1;
	CString strName = GetBlock()->GetBlockData().GetHorzScale().GetScaleData();
	CRect rect = pDrawingGraphData->GetGraphRegions().GetNoFrameRegion();

	pxScaleManager->GetVerticalBoxInfo(GetGraphRQ(), strName, rect, clrBox, bFill);
	if(bFill != -1)
	{
		rect.NormalizeRect();
		rect.top -= 3;
		rect.bottom += 4;
		pDC->Draw3dRect(rect, clrBox, clrBox);
	}
}
// private ================================================================================
// Info의 data로 SubGraph를 만든다.
/*
bool CGraphImp::MakeAllSubGraphFromIndicatorInfo(const bool bIsCalculate)
{
	if(m_pIndiInfo == NULL)
		return false;

	CString strAllSubGraphData = m_pIndiInfo->GetAllSubGraphDataForMaking();
	if(strAllSubGraphData.IsEmpty())
		return false;

	m_subGraphList.MakeSubGraphs(strAllSubGraphData);
	if(bIsCalculate)
		CalculateGraph(CCalculateBaseData::TRDATA_CAL, false, true, false);

	return true;
}
*/

// IndicatorInfo 삭제하기
bool CGraphImp::DeleteIndicatorInfo()
{
	// 지표의 Graph를 NULL로 세팅 : 특수차트 - ojtaso (20070619)
	if(m_pIndiInfo)
		m_pIndiInfo->SetGraph(NULL);

	if(!IsDeleteIndicatorInfo())
		return false;

	TRACE( "Graph Deleted : [%s](%s) ~ 0x%08X\r\n", m_graphData.GetIndicatorName(), m_graphData.GetGraphName(), long( this));
	const char *szGraphName = m_pIndiInfo->GetGraphName();
	bool bResult = ( GetIndicatorList()->RemoveGraphIndicatorInfo( szGraphName) ? true : false);
	m_pIndiInfo = NULL;

	return bResult;
}

// Packet 삭제 하기
void CGraphImp::DeletePacket_SubGraph(const bool bIsOnlyNoTRData)
{
	m_subGraphList.DeleteAllPackets(bIsOnlyNoTRData);
}

void CGraphImp::DeletePacket_Signal(const bool bIsOnlyNoTRData)
{
	CPacketList* pPacketList = GetPacketList();
	if(pPacketList == NULL)
		return;

	pPacketList->DeleteOrClearPacket(m_pSignalPacket, true, bIsOnlyNoTRData);
	m_pSignalPacket = NULL;
}

// Packet Clear 하기
void CGraphImp::ClearPacket(const bool bIsOnlyNoTRData)
{
	ClearPacket_Signal(bIsOnlyNoTRData);
	m_subGraphList.ClearAllPackets(bIsOnlyNoTRData);
}

void CGraphImp::ClearPacket_Signal(const bool bIsOnlyNoTRData)
{
	CPacketList* pPacketList = GetPacketList();
	if(pPacketList == NULL)
		return;

	pPacketList->DeleteOrClearPacket(m_pSignalPacket, false, bIsOnlyNoTRData);
	m_pSignalPacket = NULL;
}

// ---------------------------------------------------------------------------------------
void CGraphImp::Initialize_AllPackets()
{
	m_pSignalPacket = NULL;
	m_subGraphList.Initialize_AllPackets();

//	CMainChartFormulate* pMainChartFormulate = GetMainChartFormulate();
//	if(pMainChartFormulate != NULL)
//		pMainChartFormulate->Initialize_AllPackets();
}

// ---------------------------------------------------------------------------------------
bool CGraphImp::CalculateIndicator(const CCalculateBaseData::CALCULATETYPE eCalType, const bool bIsClearPacketData, const bool bIsRequestTR)
{
	if(bIsClearPacketData)
	{
		ClearPacket();
	}

	bool bResult = CalculateIndicator_Indicator(eCalType);
	if(bIsRequestTR)
		CalculateIndicator_TR(eCalType);

	// (2009/5/26 - Seung-Won, Bae) Clear SubGraph Drawing Cahe.
	if( eCalType != CCalculateBaseData::REALDATA_CAL)
	{
		m_subGraphList.ClearCalcDataForDraw();
		m_graphData.GetSignalData().ClearCalcDataForDraw();
	}

	// (2010/4/18 - Seung-Won, Bae) for Proce Steady Color of Volume Chart.
	CPacketList* pPacketList = m_pMainBlockBaseData->GetPacketList();
	if( CCalculateBaseData::TRDATA_CAL == eCalType && m_graphData.GetIndicatorName() == _MTEXT( C2_PRICE_CHART) && pPacketList)
		m_pMainBlockBaseData->GetXScaleManager()->SetNeedCalcAreaFlag( 2);	// for price chart color retrieving

	return bResult;
}

bool CGraphImp::CalculateIndicator_TR(const CCalculateBaseData::CALCULATETYPE eCalType)
{
	if(eCalType != CCalculateBaseData::TRDATA_CAL)
		return false;

	return SendMessage_TRData();
}

bool CGraphImp::CalculateIndicator_Indicator(const CCalculateBaseData::CALCULATETYPE eCalType)
{
	CMainChartFormulate* pMainChartFormulate = GetMainChartFormulate();
	if(pMainChartFormulate == NULL)
		return false;

	CGraphCalculateData graphCalculateData( m_pSignalPacket, m_pIndicator);
	if(!m_subGraphList.GetAllSubGraphCalculateData(graphCalculateData))
		return false;

	CChartFormulateData chartFormulateData;
	if(!GetChartFormulateData(eCalType, chartFormulateData))
		return false;

	bool bResult = pMainChartFormulate->CalculateIndicator(chartFormulateData, &graphCalculateData, m_pIndiInfo);
	// (2006/11/19 - Seung-Won, Bae) Call ST Indiator Process.
	if( graphCalculateData.GetIndicator())
		if( g_iMetaTable.IsSTIndexIndicator( graphCalculateData.GetIndicator()->GetID()))
			CalculateIndicator_OCX(eCalType);

	return bResult;
}

// OCX에 지표 계산하라고 알려준다 -> TR 계산일 경우만..
void CGraphImp::CalculateIndicator_OCX(const CCalculateBaseData::CALCULATETYPE eCalType)
{
	if(m_pMainBlockBaseData == NULL || eCalType != CCalculateBaseData::TRDATA_CAL)
		return;

	CList<CPacket*, CPacket*> packetList;
	if(!m_subGraphList.GetSubGraphPacketList(packetList))
		return;

	// signal(신호)을 사용하는 지표인 경우 signal packet을 첨부.
	if(m_pSignalPacket != NULL)
		packetList.AddTail(m_pSignalPacket);

	// (2007/2/27 - Seung-Won, Bae) Skip if All SubGraph Hide.
	//	Do not skip calculating for ST Notify.
	//	if( m_subGraphList.AreHidingAllSubGraphs()) return;

	m_pMainBlockBaseData->SendMessageToParent_CalculateIndicatorInTRData(m_pIndiInfo, packetList);
}

// TR 요청
// -> "시장지표"
bool CGraphImp::SendMessage_TRData()
{
	if(!IsSendMessage_TRData())
		return false;

	int nRequestTRCount = 0;
	bool bIsNew = false;
	if(!GetRequestTRCountAndType(nRequestTRCount, bIsNew))
		return false;

	m_pMainBlockBaseData->SendMessageToParent_TRData( m_graphData.GetGraphName2(), m_graphData.GetPacketNames(), nRequestTRCount, bIsNew);
	return true;
}

// ---------------------------------------------------------------------------------------
// "BoxChart", "대기매물"일때는 계산 안함
// (참조: GraphType 은 subGraph 에서 체크한다.)
// 2004.05.12. -> "대기매물"이 특수형에서 첨부형으로 변경(가격차트의 scaleGroup 사용)
bool CGraphImp::IsDisplayDataMinMax() const
{
	CString strIndicatorName = m_graphData.GetIndicatorName2();
	return (strIndicatorName != _MTEXT( C2_OVERHANGING_SUPPLY));
}

bool CGraphImp::IsEqualPacketNames(const CString& strPacketNames) const
{
	if(m_graphData.GetPacketNames() == strPacketNames)
		return true;

	// "," 및 " "를 제외하고 비교한다.
	return (GetNoSpaceCommaString(m_graphData.GetPacketNames()) == GetNoSpaceCommaString(strPacketNames));
}

bool CGraphImp::IsDeleteIndicatorInfo() const
{
	if(m_pIndiInfo == NULL || m_pMainBlockBaseData == NULL)
		return false;

	CIndicatorList* pIndicatorList = GetIndicatorList();
	if(pIndicatorList == NULL)
		return false;

	// indicatorInfo 자동 삭제일 경우만 삭제한다!
	if(!m_pMainBlockBaseData->DoesAutoDeleteIndicatorInfo())
		return false;

	// (2007/2/16 - Seung-Won, Bae) Check for Previous Version of Multiple and Same Name Indicator.
	// 현재 다른 graph에서 같은 IndicatorInfo 를 쓰고 있는지 체크!!	
	CMainBlockImp* pMainBlock = GetMainBlock();
	if(pMainBlock == NULL) return false;
	CGraphSearchInfo_IndicatorObject gsi;
	gsi.m_pIndicatorInfo	= m_pIndiInfo;
	gsi.m_pExceptGraph		= this;
	return !pMainBlock->FindGraph( &gsi);
}

bool CGraphImp::IsSendMessage_TRData() const
{
	if(m_pIndiInfo == NULL || m_pMainBlockBaseData == NULL)
		return false;

	// TR data type : <일간> 일 경우만 처리
	CPacketList* pPacketList = GetPacketList();
	if(pPacketList == NULL || pPacketList->GetDateUnitType() != CScaleBaseData::HORZ_DAILY)
		return false;

	// group 명 체크
	// -> 나중에 공식쪽에서 지표와 TR 부분을 분류한뒤 처리.
//	IChartCtrl *pChartCtrl = m_pMainBlockBaseData->GetMainAttributes().GetIChartCtrl();
//	if( !pChartCtrl) return false;
//	return pChartCtrl->IsIndicatorGroup_TR( m_pIndiInfo->GetGroupName(), m_pIndiInfo->GetIndicatorName());
	return g_iMetaTable.IsMarketOrServerIndex( m_pIndiInfo->GetIndicatorName());
}

bool CGraphImp::IsSendMessage_NewTRData(const CPacket* pPacket) const
{
	return (pPacket == NULL || !pPacket->IsAcceptFromServer());
}

bool CGraphImp::IsSendMessage_InsertTRData(const CPacket* pPacket) const
{
	//기본 TR 이 안들어온 경우는 Insert TR 이 아님!
	if(pPacket == NULL || !pPacket->IsAcceptFromServer())
		return false;

	CPacketList* pPacketList = GetPacketList();
	if(pPacketList == NULL)
		return false;

	CPacketDataInfo::DATAINSERTTYPE eDataInsertType = pPacketList->GetDataInsertType();
	return (eDataInsertType == CPacketDataInfo::INSERT);
}

//현재 data count 가 처음 TR data count 보다 작은 경우(예탁금, 금리 경우)
//전체 TR data count와 같은 경우는 TR을 요청하지 않아도 된다.
bool CGraphImp::IsSendMessage_InsertTRData(const CPacketList* pPacketList, const int nDataCount) const
{
	if(pPacketList == NULL)
		return false;

	int nTotalTRIndex = pPacketList->GetTotalTRIndex();
	// 조회된 갯수가 없을 경우 - TR 요청 안함
	if(nTotalTRIndex <= 0)
		return false;
	// 조회된 갯수가 1개일 경우 - dataCount 가 있는 경우는 요청 안함
	else if(nTotalTRIndex == 1)
		return (nDataCount <= 0);
	// 조회된 갯수가 2개 이상일 경우 - 전체 data count와 비교
	else
		return (nDataCount < pPacketList->GetTotalTRCount());

	return false;
}

// ---------------------------------------------------------------------------------------
CPacketList* CGraphImp::GetPacketList() const
{
	if(m_pMainBlockBaseData == NULL)
		return NULL;

	// 복수종목 불러오기 : 복수종목 - ojtaso (20070514)
	if(m_pPacketRQ)
		return m_pPacketRQ->GetPacketList();
	else
		return m_pMainBlockBaseData->GetPacketList();
}

CIndicatorList* CGraphImp::GetIndicatorList() const
{
	if(m_pMainBlockBaseData == NULL)
		return NULL;

	return m_pMainBlockBaseData->GetIndicatorList();
}

CMainChartFormulate* CGraphImp::GetMainChartFormulate() const
{
	if(m_pMainBlockBaseData == NULL)
		return NULL;

	return m_pMainBlockBaseData->GetMainAttributes().GetMainChartFormulate();
}

bool CGraphImp::GetChartFormulateData(const CCalculateBaseData::CALCULATETYPE eCalType, CChartFormulateData& chartFormulateData) const
{
	CPacketList* pPacketList = GetPacketList();
	CIndicatorList* pIndicatorList = GetIndicatorList();
	if(pPacketList == NULL || pIndicatorList == NULL)
		return false;

	chartFormulateData.SetCalculateType(eCalType);
	chartFormulateData.SetGraphName(		m_graphData.GetGraphName2());
	chartFormulateData.SetIndicatorName(	m_graphData.GetIndicatorName2());	// (2006/12/5 - Seung-Won, Bae) Support Indicator Name
	chartFormulateData.SetPacketNames(m_graphData.GetPacketNames());
	chartFormulateData.SetSubGraphNames(m_subGraphList.GetSubGraphNames());
	chartFormulateData.SetViewableSubGraphNames(m_subGraphList.GetViewableSubGraphNames());
	chartFormulateData.SetDisplayDataStartIndex(m_pMainBlockBaseData->GetDisplayAttributes().GetDataStartInBlock());
	chartFormulateData.SetDisplayDataEndIndex(m_pMainBlockBaseData->GetDisplayAttributes().GetDataEndInBlock());
	chartFormulateData.SetPacketList(pPacketList);
	chartFormulateData.SetIndicatorList(pIndicatorList);
	chartFormulateData.SetOcxHwnd( m_pMainBlockBaseData->GetMainAttributes().GetParent_Handle());
	// (2009/9/8 - Seung-Won, Bae) Chart Running Mode for the brick size of Renko on HTS.
	chartFormulateData.SetChartMode( m_pMainBlockBaseData->GetMainAttributes().GetChartMode());
	return true;
}

// graph scale type 가져온다.
CScaleBaseData::VERTSCALEPOSITION CGraphImp::GetVertScalePosition(const CString& strVertScalePosition) const
{
	int vertScalePosition = atoi(strVertScalePosition);
	if(vertScalePosition < 0 || vertScalePosition > 3)
		return CScaleBaseData::VERT_NONE;
	return (CScaleBaseData::VERTSCALEPOSITION) vertScalePosition;
}

// ---------------------------------------------------------------------------------------
// 세로 scale width 구하기.
int CGraphImp::GetVertScaleRegionWidth(CDC* pDC, CVertScaleMaxWidthCalculatorData* pCalData) const
{
	assert(pCalData != NULL);

	// min/max를 구하는 지표만 width도 구한다.
	if(!IsDisplayDataMinMax())
		return 0;

	pCalData->SetCalcRiseFallRate_CurData(m_graphData.GetGraphDataDrawingData().GetCalcRiseFallRate_CurData());
	pCalData->SetGraphName( m_graphData.GetGraphName2());
	pCalData->SetIndicatorInfo( m_pIndiInfo);
	return m_subGraphList.GetVertScaleRegionWidth( pDC, pCalData);
}

// <가격차트>의 color 구하기
// <거래량 차트>, <거래대금>일 경우만 필요하다.
CGraphColor* CGraphImp::GetPriceChartColor() const
{
	if(m_pMainBlockBaseData == NULL)
		return NULL;

	if( !g_iMetaTable.IsDrawGroup_VolumeType( m_graphData.GetIndicatorName2())) return NULL;

	CMainBlockImp* pMainBlock = GetMainBlock();
	if(pMainBlock == NULL)
		return NULL;

	return pMainBlock->GetGraphColor( _MTEXT( C2_PRICE_CHART), 0);
}

// 양끝에 " "와 ","가 없는 string.
CString CGraphImp::GetNoSpaceCommaString(const CString& strData) const
{
	CString strNewData = strData;
	strNewData.TrimLeft(" ");
	strNewData.TrimRight(" ");
	strNewData.TrimRight(",");

	return strNewData;
}

// ---------------------------------------------------------------------------------------
// TR 보낼 count 와 type
bool CGraphImp::GetRequestTRCountAndType(int& nRequestTRCount, bool& bIsNew) const
{
	CPacket* pPacket = m_subGraphList.GetHeadPacket();
	if(IsSendMessage_NewTRData(pPacket))
	{
		return GetRequestTRCountAndType_New(nRequestTRCount, bIsNew);
	}
	
	if(IsSendMessage_InsertTRData(pPacket))
	{
		return GetRequestTRCountAndType_Insert(pPacket, nRequestTRCount, bIsNew);
	}

	return false;
}

bool CGraphImp::GetRequestTRCountAndType_New(int& nRequestTRCount, bool& bIsNew) const
{
	CPacketList* pPacketList = GetPacketList();
	if(pPacketList == NULL)
		return false;

	bIsNew = true;
	nRequestTRCount = pPacketList->GetTotalTRCount();
	return true;
}

bool CGraphImp::GetRequestTRCountAndType_Insert(const CPacket* pPacket, int& nRequestTRCount, bool& bIsNew) const
{
	if(pPacket == NULL)
		return false;

	bIsNew = false; //(nDataCount > 0 ? false: true);
	nRequestTRCount = 0;

	//count 구하기
	CPacketList* pPacketList = GetPacketList();
	int nDataCount = pPacket->GetCount();
	if(!IsSendMessage_InsertTRData(pPacketList, nDataCount))
		return false;

	nRequestTRCount = pPacketList->GetRequestTRCount(nDataCount);
	return (nRequestTRCount > 0);
}

/*
// ---------------------------------------------------------------------------------------
void CGraphImp::ChangeGraphData(const CGraphData& sourceGraphData, const bool bIsAutoCalCheck)
{
	// 함수명, packet 명이 바뀌였는지를 확인하여 바뀔 경우 지표계산 유무 체크.
	bool bIsCalculate = false;
	if(bIsAutoCalCheck)
		bIsCalculate = !IsEqualDataForCalculate(sourceGraphData);

	// 함수명이 바뀔 경우는 CIndicatorInfo* 를 변경해줘야 함
	SetGraphNameAndSubGraphNames(sourceGraphData.GetGraphName(), false, false);

	// graphData 변경
	m_graphData = sourceGraphData;

	// 지표 계산
	if(bIsCalculate)
		CalculateGraph(CCalculateBaseData::TRDATA_CAL);
}
*/

/*
// subGraphData를 모두 바꾸기.
// -> bIsCheckSubGraphCount = true : Info에 있는 subGraph 수와 비교.
//							  false : Info에 있는 내용으로 새로 Make.
bool CGraphImp::ChangeAllSubGraphData(const bool bIsCheckSubGraphCount, const bool bIsAllChangeSubGraphData)
{
	if(m_pIndiInfo == NULL)
		return false;

	if(bIsCheckSubGraphCount)
	{
		// subGraph count가 같을 경우 -> subGraphData 만 수정
		if(m_subGraphList.GetSubGraphCount() == m_pIndiInfo->GetSubGraphCount())
			return ChangeAllSubGraphDataFromIndicatorInfo(m_pIndiInfo, bIsAllChangeSubGraphData);
	}

	return MakeAllSubGraphFromIndicatorInfo(false);
}
*/

// IndicatorInfo에 있는 subGraphData로 바꾸기.
bool CGraphImp::ChangeAllSubGraphDataFromIndicatorInfo(const CIndicatorInfo* pIndicatorInfo, const bool bIsAllChangeSubGraphData)
{
	if(pIndicatorInfo == NULL)
		return false;

	CList<CSubGraphData*, CSubGraphData*> sourceSubGraphDataList;
	if(!pIndicatorInfo->GetAllSubGraphDataList(sourceSubGraphDataList))
		return false;

	return m_subGraphList.ChangeAllSubGraphData(sourceSubGraphDataList, bIsAllChangeSubGraphData);
}

// subGraph에 있는 subGraphData를 IndicatorInfo에 적용.
bool CGraphImp::ChangeIndicatorInfoFromAllSubGraphData(CIndicatorInfo& indicatorInfo)
{
	CList<CSubGraphData*, CSubGraphData*> sourceSubGraphDataList;
	if(!m_subGraphList.GetAllSubGraphDataList(sourceSubGraphDataList))
		return false;

	return indicatorInfo.SetAllSubGraphDataList(&sourceSubGraphDataList);
}

void CGraphImp::ChangeTitleDraw(CDrawingGraphData* pDrawingGraphData, bool& bOldConditionTitleDraw, bool& bOldDataTitleDraw)
{
	bOldConditionTitleDraw = pDrawingGraphData->DoesConditionTitleDraw();
	bOldDataTitleDraw = pDrawingGraphData->DoesDataTitleDraw();

	if(bOldConditionTitleDraw)
		pDrawingGraphData->SetConditionTitleDraw( g_iMetaTable.IsConditionTitle( m_graphData.GetIndicatorName2()));

	// data 안쓴다.
	// "가격차트", "거래량차트", "그물차트", "Box Chart", "꺽은선차트", "Zig Zag", "대기매물", 
	// GRAPHTYPE == DataView_Type(데이타표현형) 일 경우
	if(bOldDataTitleDraw)
	{
		if( !g_iMetaTable.IsDataTitle( m_graphData.GetIndicatorName2())
			|| m_subGraphList.IsViewableGraphType( CGraphBaseData::DataView_Type, 0))
				pDrawingGraphData->SetDataTitleDraw(false);
	}
}

// IndicatorInfo의 packetName 변경.
bool CGraphImp::ChangePacketNameInIndicatorInfo(const CString& strPacketName)
{
	if(m_pIndiInfo == NULL)
		return false;

	// common 부분 수정
	bool bResult = m_pIndiInfo->SetCommonPacketNames(strPacketName);
	// subGraph 부분 모두 수정
	if(m_pIndiInfo->SetAllPacketNames_SubGraphPart(strPacketName, true))
		bResult = true;

	return bResult;
}

bool CGraphImp::ChangeIndicatorInfo(const CIndicatorInfo* pSourceIndicatorInfo)
{
	if(m_pIndiInfo == NULL || pSourceIndicatorInfo == NULL || pSourceIndicatorInfo == m_pIndiInfo)
		return false;

	return m_pIndiInfo->GetCopyDataFrom(pSourceIndicatorInfo);
}

// CPacketRQ*로 Graph 체크 : 복수종목 - ojtaso (20070109)
BOOL CGraphImp::ComparePacketRQ(LPVOID pPacketRQ)
{
	if(!m_pPacketRQ)
		return FALSE;

	return !::lstrcmp(m_pPacketRQ->GetRQ(), ((CPacketRQ*)pPacketRQ)->GetRQ());
}

// CPacketRQ*로 RQ : 복수종목 - ojtaso (20070109)
LPCTSTR CGraphImp::GetGraphRQ() const
{
	if(!m_pPacketRQ)
		return _T("DEFAULT");
	
	return m_pPacketRQ->GetRQ();
}

// RQ 세팅 : 복수종목 - ojtaso (200705015)
void CGraphImp::SetGraphRQ(LPCTSTR lpszRQ)
{
	if(!m_pPacketRQ)
	{
		m_pPacketRQ = (CPacketRQ*)m_pMainBlockBaseData->GetMainAttributes().GetPacketListManager()->GetPacketRQ(lpszRQ);
		if(m_pPacketRQ)
		{
			m_pPacketRQ->AddRQGraph(this);
	//		m_pMainBlockBaseData->GetMainAttributes().GetIChartCtrl()->SelectedGraph(lpszRQ);
	
	//		m_pMainBlockBaseData->GetMainAttributes().GetMainBlockImpl()->CalculateAllGraphs(CCalculateBaseData::CALCULATETYPE::TRDATA_CAL);
	//		CalculateGraph(CCalculateBaseData::CALCULATETYPE::TRDATA_CAL, true, false, true);
		}
	}
}

//{{ (2007/5/28 - LYH) For Weighted Volume Chart
void CGraphImp::CheckWeightedVolume()
{
	if( 0 < m_subGraphList.GetSubGraphCount())
	{
		CSubGraph *pSubGraph = m_subGraphList.GetSubGraph( 0);
		if( pSubGraph)
		{
			CSubGraphData &subGraphData = pSubGraph->GetSubGraphData();
			if( subGraphData.GetType() == CGraphBaseData::Bong_Type)
				m_pMainBlockBaseData->GetMainAttributes().GetMainBlockImpl()->SetWeightedVolume( 
					subGraphData.GetStyle().GetGraphBongStyle());
		}
	}
}
//}}

// CPacketRQ를 알려줌 : 복수종목 - ojtaso (20070531)
CPacketRQ* CGraphImp::GetPacketRQ()
{
	return m_pPacketRQ;
}

// 한번에 그래프 속성 변경 가능한 메쏘드 추가 - ojtaso (20080602)
void CGraphImp::ChangeGraphPropertyAtOnce( short nSubGraphIndex, short nGraphType, short nGraphStyle, short nDrawStyle, short nThickness1, short nThickness2, short nThickness3) 
{
	if(!m_pIndiInfo)
		return;

	CSubGraphData *pIndiData = m_pIndiInfo->GetSubGraphData( nSubGraphIndex);
	CSubGraphData *pGraphData = m_subGraphList.GetSubGraphData( nSubGraphIndex);

	if(pIndiData)
	{
		if(nGraphType >= 0)
		{
			pIndiData->SetType((CGraphBaseData::GRAPHTYPE)nGraphType);
			pGraphData->SetType((CGraphBaseData::GRAPHTYPE)nGraphType);
		}

		if(nGraphStyle >= 0)
		{
			pIndiData->SetStyle(nGraphStyle);
			pGraphData->SetStyle(nGraphStyle);
		}
	
		if(nDrawStyle >= 0)
		{
			pIndiData->SetDrawStyle(nDrawStyle);
			pGraphData->SetDrawStyle(nDrawStyle);
		}

		CPenThickness PenThickness;
		PenThickness = pIndiData->GetPenThickness();

		if(nThickness1 >= 0)
			PenThickness.SetThickness1(nThickness1);
		if(nThickness2 >= 0)
			PenThickness.SetThickness2(nThickness2);
		if(nThickness3 >= 0)
			PenThickness.SetThickness3(nThickness3);

		pIndiData->SetPenThickness(PenThickness);
		pGraphData->SetPenThickness(PenThickness);
	}
}

// 한번에 그래프 속성 얻어올 수 메쏘드 추가 - (@Solomon-20100120)
bool CGraphImp::GetGraphPropertyAtOnce( short nSubGraphIndex, short &nGraphType, short &nGraphStyle, short &nDrawStyle, short &nThickness1, short &nThickness2, short &nThickness3) 
{
	if(!m_pIndiInfo)
		return false;
	
	CSubGraphData *pIndiData = m_pIndiInfo->GetSubGraphData( nSubGraphIndex);
	CSubGraphData *pGraphData = m_subGraphList.GetSubGraphData( nSubGraphIndex);
	
	if(pIndiData)
	{
		nGraphType = pIndiData->GetType();
// 		if(nGraphType >= 0)
// 		{
// 			pIndiData->SetType((CGraphBaseData::GRAPHTYPE)nGraphType);
// 			pGraphData->SetType((CGraphBaseData::GRAPHTYPE)nGraphType);
// 		}

		nGraphStyle = pIndiData->GetStyle().GetGraphStyle();
// 		if(nGraphStyle >= 0)
// 		{
// 			pIndiData->SetStyle(nGraphStyle);
// 			pGraphData->SetStyle(nGraphStyle);
// 		}

		nDrawStyle = pIndiData->GetDrawStyle().GetDrawStyle();
// 		if(nDrawStyle >= 0)
// 		{
// 			pIndiData->SetDrawStyle(nDrawStyle);
// 			pGraphData->SetDrawStyle(nDrawStyle);
// 		}
		
		CPenThickness PenThickness;
		PenThickness = pIndiData->GetPenThickness();
		
// 		if(nThickness1 >= 0)
// 			PenThickness.SetThickness1(nThickness1);
// 		if(nThickness2 >= 0)
// 			PenThickness.SetThickness2(nThickness2);
// 		if(nThickness3 >= 0)
// 			PenThickness.SetThickness3(nThickness3);
// 		
// 		pIndiData->SetPenThickness(PenThickness);
// 		pGraphData->SetPenThickness(PenThickness);
		nThickness1 = PenThickness.GetThickness1();
		nThickness2 = PenThickness.GetThickness2();
		nThickness3 = PenThickness.GetThickness3();

		return true;
	}

	return false;
}

// 20081007 JS.Kim	사용자시간대 선택
LPCTSTR CGraphImp::GetPacketTimeDiffName()
{
	CPacketList* ppacketList = GetPacketList();
	return ppacketList->GetBaseTimeName();
}

int CGraphImp::GetPacketOrgBaseTimeDiff()
{
	CPacketList* ppacketList = GetPacketList();
	return ppacketList->GetOrgBaseTimeDifference();
}

void CGraphImp::SetPacketTimeDiff(CString strTimeName, int nTimeDiff)
{
	CPacketList* pPacketList = GetPacketList();

	if( strTimeName.IsEmpty() )
		pPacketList->SetOrgBaseTime();		// 원래 시간대로...
	else
		pPacketList->SetBaseTime(strTimeName, nTimeDiff);

	if( pPacketList && CScaleBaseData::HORZ_TICK == pPacketList->GetDateUnitType() )
	{
		CString strRQ = m_pPacketRQ->GetRQ();
		if( strRQ.Right( 5) == "_PAIR")	strRQ = strRQ.Left( strRQ.GetLength() - 5);
		else							strRQ = strRQ + "_PAIR";

		CPacketList* pPairPacketList = m_pMainBlockBaseData->GetPacketListManager()->GetPacketList(strRQ);
		if( pPairPacketList )
		{
			if( strTimeName.IsEmpty() )		pPairPacketList->SetOrgBaseTime();		// 원래 시간대로...
			else							pPairPacketList->SetBaseTime(strTimeName, nTimeDiff);
		}
	}

	m_pMainBlockBaseData->GetXScaleManager()->Calc_DRI();
	CalculateGraph( CCalculateBaseData::TRDATA_CAL);
}

// KTB 체결잔고표시 - ojtaso (20091019)
long CGraphImp::AddHorizontalLine(LPCTSTR lpszTitle, OLE_COLOR clrLine, short nLine, double dPrice)
{
	// 가격차트 SubGraph는 한개
	CSubGraphData *pGraphData = m_subGraphList.GetSubGraphData(0);
	if(!pGraphData) return -1;

	return pGraphData->AddHorizontalLine(lpszTitle, clrLine, nLine, dPrice);
}

// KTB 체결잔고표시 - ojtaso (20091019)
void CGraphImp::DeleteHorizontalLine(long nIndex)
{
	// 가격차트 SubGraph는 한개
	CSubGraphData *pGraphData = m_subGraphList.GetSubGraphData(0);
	if(!pGraphData) return;

	pGraphData->DeleteHorizontalLine(nIndex);
}

// KTB 체결잔고표시 - ojtaso (20091019)
void CGraphImp::DeleteAllHorizontalLine()
{
	// 가격차트 SubGraph는 한개
	CSubGraphData *pGraphData = m_subGraphList.GetSubGraphData(0);
	if(!pGraphData) return;

	pGraphData->DeleteAllHorizontalLine();
}

// KTB 체결잔고표시 - ojtaso (20091019)
void CGraphImp::UpdateHorizontalLine(long nIndex, LPCTSTR lpszTitle, OLE_COLOR clrLine, short nLine, double dPrice)
{
	// 가격차트 SubGraph는 한개
	CSubGraphData *pGraphData = m_subGraphList.GetSubGraphData(0);
	if(!pGraphData) return;

	pGraphData->UpdateHorizontalLine(nIndex, lpszTitle, clrLine, nLine, dPrice);
}
