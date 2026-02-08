// VertScaleGroup.cpp: implementation of the CVertScaleGroup class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VertScaleGroup.h"

#ifdef _TEST
#include <assert.h>
#include <math.h>											// for HUGE_VAL
#include "../Include_Chart/DLL_Load/IndicatorInfo.h"		// for CIndicatorInfo
#include "../Include_Chart/DLL_Load/PacketBase.h"			// for CPacket
#include "../Include_Chart/Dll_Load/PacketListManager.h"	// for CPacketListManager
#include "../Include_Chart/Dll_Load/IMetaTable.h"			// for _MTEXT()
#include "../Include_Chart/IChartCtrl.h"					// for IChartCtrl
#include "../Include_Chart/Conversion.h"
#include "BlockImp.h"
#include "BlocksColumn.h"									// for CBlocksColumn
#include "GraphImp.h"
#include "ScaleAndGraphDrawer.h"
#include "CalculatorData.h"
#include "MainBlockImp.h"					// for CMainBlockImp
#include "GraphSearchInfo.h"							// for CGraphSearchInfo
#include "../Include_Chart/Dll_Load/XScaleManager.h"		// for CXScaleManager
#include "PacketRQ.h"									// for CPacketRQ


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// (2006/11/26 - Seung-Won, Bae) Support Empty VScaleGroup
CVertScaleGroup::CVertScaleGroup(CMainBlockBaseData* pMainBlockBaseData, CVertScaleGroupList* pVertScaleGroupList):
	m_pMainBlockBaseData( pMainBlockBaseData),
	m_pVertScaleGroupList( pVertScaleGroupList),
	m_displayDataMinMax( 0, 0),
	m_lpSelectedGraph(NULL)		// Y Scale상에 종가 표시 수정 - ojtaso (20070807)
{
	// (2007/2/16 - Seung-Won, Bae) Protect the redestructing on Destructor.
	m_bOnDestructor = FALSE;

	// (2007/1/14 - Seung-Won, Bae) Get the Base Packet for Default Scale Min/Max
	m_pBaseGraph	= NULL;
	m_pBasePacket	= NULL;
}

CVertScaleGroup::CVertScaleGroup(CMainBlockBaseData* pMainBlockBaseData, CVertScaleGroupList* pVertScaleGroupList, char *p_szVertScaleGroup):
	m_pMainBlockBaseData(pMainBlockBaseData),
	m_pVertScaleGroupList(pVertScaleGroupList),
	m_displayDataMinMax( 0, 0),
	m_lpSelectedGraph(NULL)		// Y Scale상에 종가 표시 수정 - ojtaso (20070807)
{
	// (2007/2/16 - Seung-Won, Bae) Protect the redestructing on Destructor.
	m_bOnDestructor = FALSE;

	// (2007/1/14 - Seung-Won, Bae) Get the Base Packet for Default Scale Min/Max
	m_pBaseGraph	= NULL;
	m_pBasePacket	= NULL;

	MakeGraphs( p_szVertScaleGroup);
}

// RQ로 검색 : 복수종목 - ojtaso (20070118)
CVertScaleGroup::CVertScaleGroup(LPVOID pPacketRQ, CMainBlockBaseData* pMainBlockBaseData, CVertScaleGroupList* pVertScaleGroupList, char *p_szVertScaleGroup):
	m_pMainBlockBaseData(pMainBlockBaseData),
	m_pVertScaleGroupList(pVertScaleGroupList),
	m_displayDataMinMax( 0, 0),
	m_lpSelectedGraph(NULL)		// Y Scale상에 종가 표시 수정 - ojtaso (20070807)
{
	// (2007/2/16 - Seung-Won, Bae) Protect the redestructing on Destructor.
	m_bOnDestructor = FALSE;

	// (2007/1/14 - Seung-Won, Bae) Get the Base Packet for Default Scale Min/Max
	m_pBaseGraph	= NULL;
	m_pBasePacket	= NULL;

	MakeGraphs(pPacketRQ, p_szVertScaleGroup);
}


CVertScaleGroup::CVertScaleGroup(CMainBlockBaseData* pMainBlockBaseData, CVertScaleGroupList* pVertScaleGroupList, const CVerticalScale& vertScale):
	m_pMainBlockBaseData(pMainBlockBaseData),
	m_pVertScaleGroupList(pVertScaleGroupList),
	m_vertScale(vertScale),
	m_displayDataMinMax(-20.0, 20.0),
	m_lpSelectedGraph(NULL)		// Y Scale상에 종가 표시 수정 - ojtaso (20070807)
{
	// (2007/2/16 - Seung-Won, Bae) Protect the redestructing on Destructor.
	m_bOnDestructor = FALSE;

	// (2007/1/14 - Seung-Won, Bae) Get the Base Packet for Default Scale Min/Max
	m_pBaseGraph	= NULL;
	m_pBasePacket	= NULL;
}

CVertScaleGroup::~CVertScaleGroup()
{
	// (2007/2/16 - Seung-Won, Bae) Protect the redestructing on Destructor.
	m_bOnDestructor = TRUE;

	// (2007/1/14 - Seung-Won, Bae) Get the Base Packet for Default Scale Min/Max
	m_pBaseGraph	= NULL;
	m_pBasePacket	= NULL;

	// Y Scale상에 종가 표시 수정 - ojtaso (20070807)
	m_lpSelectedGraph = NULL;

	DeleteAll();
}

//////////////////////////////////////////////////////////////////////
// Delete
//////////////////////////////////////////////////////////////////////
BOOL CVertScaleGroup::DeleteAllIndicator( const char *p_szIndicatorName)
{
	if( !p_szIndicatorName)		return FALSE;
	if( !*p_szIndicatorName)	return FALSE;

	BOOL bResult = FALSE;
	CGraphImp *pGraph = NULL;
	POSITION posGraph = m_GraphList.GetHeadPosition();
	while( posGraph)
	{
		pGraph = m_GraphList.GetNext( posGraph);
		if( !pGraph) continue;
		if( pGraph->GetGraphData().GetIndicatorName() == p_szIndicatorName)
		{
			DeleteGraphWithNotify( pGraph, true);
		
			bResult = TRUE;
		}
	}

	return bResult;
}

//////////////////////////////////////////////////////////////////////
// Search Object
//////////////////////////////////////////////////////////////////////

// (2007/2/22 - Seung-Won, Bae) Support Next Search.
BOOL CVertScaleGroup::FindGraph( CGraphSearchInfo *p_pGraphSearchInfo) const
{
	if( !p_pGraphSearchInfo)	return FALSE;

	int nGraphIndex		= p_pGraphSearchInfo->m_nGraphIndex;
	POSITION posGraph	= ( p_pGraphSearchInfo->m_posGraph ? p_pGraphSearchInfo->m_posGraph : m_GraphList.FindIndex( p_pGraphSearchInfo->m_nGraphIndex));

	CGraphImp *pGraph = NULL;
	POSITION posCurrentGraph = NULL;
	while( posGraph)
	{
		posCurrentGraph = posGraph;
		pGraph = m_GraphList.GetNext( posGraph);
		if( !pGraph) continue;
		if( p_pGraphSearchInfo->FindGraph( pGraph))
		{
			p_pGraphSearchInfo->m_posGraph		= posGraph;
			p_pGraphSearchInfo->m_nGraphIndex	= nGraphIndex;
			p_pGraphSearchInfo->m_pGraphImp		= pGraph;
			return TRUE;
		}
		nGraphIndex++;
	}

	// (2007/4/11 - Seung-Won, Bae) Reset for Next Vertical Scale Group Search
	p_pGraphSearchInfo->m_posGraph		= NULL;
	p_pGraphSearchInfo->m_nGraphIndex	= 0;
	return FALSE;
}

// (2008/9/15 - Seung-Won, Bae) RQ Search
BOOL CVertScaleGroup::FindGraph( LPVOID pPacketRQ, CGraphSearchInfo *p_pGraphSearchInfo) const
{
	if( !p_pGraphSearchInfo)	return FALSE;

	int nGraphIndex		= p_pGraphSearchInfo->m_nGraphIndex;
	POSITION posGraph	= ( p_pGraphSearchInfo->m_posGraph ? p_pGraphSearchInfo->m_posGraph : m_GraphList.FindIndex( p_pGraphSearchInfo->m_nGraphIndex));

	CGraphImp *pGraph = NULL;
	CPacketRQ* pRQ = (CPacketRQ*)pPacketRQ;
	POSITION posCurrentGraph = NULL;
	while( posGraph)
	{
		posCurrentGraph = posGraph;
		pGraph = m_GraphList.GetNext( posGraph);
		if( !pGraph) continue;
		if( p_pGraphSearchInfo->FindGraph( pGraph))
		{
			if(!::lstrcmp( pGraph->GetGraphRQ(), pRQ->GetRQ()))
			{
				p_pGraphSearchInfo->m_posGraph		= posGraph;
				p_pGraphSearchInfo->m_nGraphIndex	= nGraphIndex;
				p_pGraphSearchInfo->m_pGraphImp		= pGraph;
				return TRUE;
			}
		}
		nGraphIndex++;
	}

	// (2007/4/11 - Seung-Won, Bae) Reset for Next Vertical Scale Group Search
	p_pGraphSearchInfo->m_posGraph		= NULL;
	p_pGraphSearchInfo->m_nGraphIndex	= 0;
	return FALSE;
}

// Sub Graph
CSubGraph *CVertScaleGroup::GetSubGraph( const int nGraphIndex, const int nSubGraphIndex) const
{
	CGraphImp *pGraph = GetGraph( nGraphIndex);
	if( !pGraph) return NULL;

	return pGraph->GetSubGraphList().FindSubGraph( nSubGraphIndex);
}

// (2007/2/20 - Seung-Won, Bae) Support Indicator Hiding
//		It can not be replaced with AddChartBlock(), because of Condition Reset.
void CVertScaleGroup::ShowIndicator( const char *p_szIndicatorName, BOOL p_bShow, BOOL p_bRecalculate)
{
	if( !p_szIndicatorName) return;
	if( !*p_szIndicatorName) return;

	POSITION pos = m_GraphList.GetHeadPosition();
	while(pos != NULL)
	{
		CGraphImp* pGraph = m_GraphList.GetNext(pos);
		assert(pGraph != NULL);
		if( pGraph->GetGraphData().GetIndicatorName() != p_szIndicatorName)
			continue;

		pGraph->SetGraphHide( !p_bShow);

		if( p_bRecalculate && p_bShow) pGraph->CalculateGraph( CCalculateBaseData::TRDATA_CAL);
	}
}

// (2006/11/26 - Seung-Won, Bae) Support Empty VScaleGroup
CVertScaleGroup* CVertScaleGroup::MakeVertScaleGroup(CMainBlockBaseData* pMainBlockBaseData, CVertScaleGroupList* pVertScaleGroupList)
{
	return new CVertScaleGroup(pMainBlockBaseData, pVertScaleGroupList);
}

// CVertScaleGroup를 생성& 삭제
CVertScaleGroup* CVertScaleGroup::MakeVertScaleGroup(CMainBlockBaseData* pMainBlockBaseData, CVertScaleGroupList* pVertScaleGroupList, char *p_szVertScaleGroup)
{
	return new CVertScaleGroup(pMainBlockBaseData, pVertScaleGroupList, p_szVertScaleGroup);
}

// RQ로 검색 : 복수종목 - ojtaso (20070118)
CVertScaleGroup* CVertScaleGroup::MakeVertScaleGroup(LPVOID pPacketRQ, CMainBlockBaseData* pMainBlockBaseData, CVertScaleGroupList* pVertScaleGroupList, char *p_szVertScaleGroup)
{
	return new CVertScaleGroup(pPacketRQ, pMainBlockBaseData, pVertScaleGroupList, p_szVertScaleGroup);
}

CVertScaleGroup* CVertScaleGroup::MakeVertScaleGroup(CMainBlockBaseData* pMainBlockBaseData, CVertScaleGroupList* pVertScaleGroupList, const CVerticalScale& vertScale)
{
	return new CVertScaleGroup(pMainBlockBaseData, pVertScaleGroupList, vertScale);
}

void CVertScaleGroup::Delete(CVertScaleGroup* pVertScaleGroup)
{
	if( !pVertScaleGroup->m_bOnDestructor) delete pVertScaleGroup;
}

/////////////////////////////////////////////////////////////////////////////////////////////


// public ================================================================================
// 기존의 Graph을 삭제하고 원하는 갯수로 Graph 만든다.
// "사용할 Data;위치;grid타입;unit타입;\r\n
//		Graph;\r\n
//		GraphData...\r\n
//		SubGraphData....\r\n
// GraphEnd;\r\n" 형식이여야함.
void CVertScaleGroup::MakeGraphs( char *p_szVertScaleGroup)
{
	DeleteAll();

	char *szGraph = strstr( p_szVertScaleGroup, "\r\n");
	if( !szGraph) return;
	*szGraph = '\0';

	m_vertScale.SetScaleData( p_szVertScaleGroup, m_pMainBlockBaseData->GetParent_Handle());
	*szGraph = '\r';

	char cGraphEnd = 0;
	char *szGraphEnd = NULL;
	szGraph = strstr( szGraph, "Graph;\r\n");
	while( szGraph)
	{
		szGraphEnd = strstr( szGraph, "GraphEnd;\r\n");
		if( szGraphEnd)
		{
			szGraphEnd += 11;									// "GraphEnd;\r\n"
			cGraphEnd = *szGraphEnd;
			*szGraphEnd = '\0';
		}

		MakeGraph( szGraph);
		if( szGraphEnd)
		{
			*szGraphEnd = cGraphEnd;
			szGraph = strstr( szGraphEnd, "Graph;\r\n");
		}
		else szGraph = NULL;
	}
}

// RQ로 검색 : 복수종목 - ojtaso (20070118)
void CVertScaleGroup::MakeGraphs(LPVOID pPacketRQ,  char *p_szVertScaleGroup)
{
	DeleteAll();

	char *szGraph = strstr( p_szVertScaleGroup, "\r\n");
	if( !szGraph) return;
	*szGraph = '\0';

	m_vertScale.SetScaleData( p_szVertScaleGroup, m_pMainBlockBaseData->GetParent_Handle());
	*szGraph = '\r';

	char cGraphEnd = 0;
	char *szGraphEnd = NULL;
	szGraph = strstr( szGraph, "Graph;\r\n");
	while( szGraph)
	{
		szGraphEnd = strstr( szGraph, "GraphEnd;\r\n");
		if( szGraphEnd)
		{
			szGraphEnd += 11;									// "GraphEnd;\r\n"
			cGraphEnd = *szGraphEnd;
			*szGraphEnd = '\0';
		}

		MakeGraph(pPacketRQ, szGraph);
		if( szGraphEnd)
		{
			*szGraphEnd = cGraphEnd;
			szGraph = strstr( szGraphEnd, "Graph;\r\n");
		}
		else szGraph = NULL;
	}
}

// graph 생성
bool CVertScaleGroup::MakeGraph( char *p_szGraphData, const bool bIsAddTail)
{
	if( !p_szGraphData) return false;
	if( !*p_szGraphData) return false;

	CGraphImp* pGraph = MakeNewGraph( p_szGraphData);
	if(pGraph == NULL) return false;

	AddGraph(pGraph, bIsAddTail);

	// (2006/11/17 - Seung-Won, Bae) Notify to OCX
	if( m_pMainBlockBaseData)
	{
		IChartCtrl *pChartCtrl = m_pMainBlockBaseData->GetMainAttributes().GetIChartCtrl();
		if( pChartCtrl)
		{
			CGraphData &graphData = pGraph->GetGraphData();
			pChartCtrl->OnGraphAdded( pGraph->GetGraphRQ(), graphData.GetGraphName(), graphData.GetIndicatorName());
		}
	}

	// (2007/1/19 - Seung-Won, Bae) Activate Vertical Scale Group.
	if( m_pVertScaleGroupList) m_pVertScaleGroupList->SetActivate( this);

	return true;
}

// RQ를 가지고 있는 Graph 생성 : 복수종목 - ojtaso (20061224)
// CPacketRQ*로 변경 : 복수종목 - ojtaso (20070109)
bool CVertScaleGroup::MakeGraph(LPVOID pPacketRQ, char *p_szGraphData, const bool bIsAddTail)
{
	if( !p_szGraphData) return false;
	if( !*p_szGraphData) return false;

	CGraphImp* pGraph = MakeNewGraph(pPacketRQ, p_szGraphData);
	if(pGraph == NULL) return false;

	AddGraph(pGraph, bIsAddTail);

	// (2006/11/17 - Seung-Won, Bae) Notify to OCX
	if( m_pMainBlockBaseData)
	{
		IChartCtrl *pChartCtrl = m_pMainBlockBaseData->GetMainAttributes().GetIChartCtrl();
		if( pChartCtrl)
		{
			CGraphData &graphData = pGraph->GetGraphData();
			pChartCtrl->OnGraphAdded( pGraph->GetGraphRQ(), graphData.GetGraphName(), graphData.GetIndicatorName());
		}
	}

	return true;
}

bool CVertScaleGroup::DeleteGraphWithNotify(CGraphImp* pGraph, const bool bIsOnlyNoTRData)
{
	if( !pGraph) return false;

	// (2006/11/17 - Seung-Won, Bae) Notify to OCX
	// (2007/1/2 - Seung-Won, Bae) Vertical Scale Group can be deleted in DeleteGraph()
	//		So. Do Everything before DeleteGraph().
	if( m_pMainBlockBaseData)
	{
		IChartCtrl *pChartCtrl = m_pMainBlockBaseData->GetMainAttributes().GetIChartCtrl();
		if( pChartCtrl)
		{
			CGraphData &graphData = pGraph->GetGraphData();
			// 복수종목 지원을 위해 수정 : 복수종목 - ojtaso (20070125)
			pChartCtrl->OnGraphDeleted(pGraph->GetGraphRQ(), graphData.GetGraphName(), graphData.GetIndicatorName());
		}
	}

	DeleteGraph( pGraph, bIsOnlyNoTRData);
	return true;
}

// 현재 선택된 객체를 삭제한다.
bool CVertScaleGroup::DeleteGraph(CGraphImp* pGraph, const bool bIsOnlyNoTRData)
{
	assert(pGraph != NULL);
	CString strGraphName(pGraph->GetGraphData().GetGraphName()),
			strIndicatorName(pGraph->GetGraphData().GetIndicatorName());	
	
	if(strGraphName.GetLength()>0 && strGraphName.GetAt(0)=='#')
	{
		if(strIndicatorName.Compare("_Price MA_") !=0 && 
			strIndicatorName.Compare("_Volume MA_") !=0)
			pGraph->DeletePacket( bIsOnlyNoTRData);
	}
	else
	{
		pGraph->DeletePacket( bIsOnlyNoTRData);
	}

	// Y Scale상에 종가 표시 수정 - ojtaso (20070807)
	if(m_lpSelectedGraph == pGraph)
		m_lpSelectedGraph = NULL;

	// (2007/2/16 - Seung-Won, Bae) Remove in list after Deleting the Object.
	//		In Destructor, can be access the List.
	CGraphImp::Delete(pGraph);

	POSITION pos = m_GraphList.Find(pGraph);
	if(pos == NULL) return false;
	m_GraphList.RemoveAt(pos);

	// (2006/12/10 - Seung-Won, Bae) Support Self Destruction.
	//		Graph has Deleted in VScaleGroup's Destructor.
	//		Return Value must be false for No-More Action in DeleteGraphWithNotify().
	//		OCX Notify was processed in another DeleteGraphWithNotify() of VScaleGroup's Destructor.
	DeleteEmptyGroup();

	return true;
}

// 리스트의 모든 내용 삭제
// graph 삭제는 마자막걸 먼저 지운다.
// 삭제 후에는 remove 시킨다.
void CVertScaleGroup::DeleteAll()
{
	// (2006/12/10 - Seung-Won, Bae) Support Notify for ST.
	while( !m_GraphList.IsEmpty())
	{
		CGraphImp *pGraph = m_GraphList.GetTail();
		DeleteGraphWithNotify( pGraph);
	}
}

// ----------------------------------------------------------------------------------------
// list에 추가한다.
void CVertScaleGroup::AddGraph(CGraphImp* pGraph, const bool bIsAddTail)
{
	assert(pGraph != NULL);
	pGraph->SetVertScaleGroup(this);

	if(bIsAddTail)
		m_GraphList.AddTail(pGraph);
	else
		m_GraphList.AddHead(pGraph);
	
	CalculateDisplayDataMinMax();
}

bool CVertScaleGroup::RemoveAt(CGraphImp* pGraph, const bool bIsDeleteBlock)
{
	if(pGraph == NULL)
		return false;

	POSITION pos = m_GraphList.Find(pGraph);
	if(pos == NULL)
		return false;

	m_GraphList.RemoveAt(pos);

	if(m_GraphList.GetCount() == 0 && m_pVertScaleGroupList != NULL)
		m_pVertScaleGroupList->DeleteVertScaleGroup(this, bIsDeleteBlock);
	
	// (2009/3/9 - Seung-Won, Bae) Clear Selected Graph
	if( m_lpSelectedGraph == pGraph) m_lpSelectedGraph = NULL;

	return true;
}

// ----------------------------------------------------------------------------------------
bool CVertScaleGroup::MoveAllGraphs(CVertScaleGroupList& groupList)
{
	POSITION pos = m_GraphList.GetHeadPosition();
	if(pos == NULL)
		return false;

	bool bResult = false;
	while(pos != NULL)
	{
		CGraphImp* pGraph = m_GraphList.GetNext(pos);
		if(groupList.MoveGraph(pGraph, -1, false))
			bResult = true;
	}

	return bResult;
}

bool CVertScaleGroup::MoveAllGraphs(CVertScaleGroup* pSourceGroup)
{
	if(pSourceGroup == NULL || pSourceGroup->GetGraphCount() <= 0)
		return false;

	while(pSourceGroup->GetGraphCount() > 0)
	{
		CGraphImp* pGraph = pSourceGroup->GetGraph( 0);
		assert(pGraph != NULL);
		pSourceGroup->RemoveAt(pGraph, true);
		AddGraph(pGraph);
	}

	return true;
}


// graph type 변경.
bool CVertScaleGroup::ChangeGraphType(const CString& targetGraphName, const CGraphBaseData::GRAPHTYPE& sourceType, 
		const CGraphStyle& sourceStyle, const CGraphDrawStyle& sourceDrawStyle, const CString& sourcePacketNames)
{
	POSITION pos = m_GraphList.GetHeadPosition();
	while(pos != NULL)
	{
		CGraphImp* pGraph = m_GraphList.GetNext(pos);
		assert(pGraph != NULL);
		if(pGraph->GetGraphData().GetGraphName() != targetGraphName)
			continue;

		pGraph->SetPacketNames(sourcePacketNames);
		pGraph->GetSubGraphList().SetType(0, sourceType, sourceStyle, sourceDrawStyle);
		return true;
	}

	return false;
}

// subGraph data 변경.
bool CVertScaleGroup::ChangeSubGraphData(const CString& targetGraphName, const CString& targetSubGraphName, const CSubGraphData& sourceSubGraphData)
{
	POSITION pos = m_GraphList.GetHeadPosition();
	while(pos != NULL){
		CGraphImp* pGraph = m_GraphList.GetNext(pos);
		assert(pGraph != NULL);
		if(pGraph->GetGraphData().GetGraphName() != targetGraphName)
			continue;

		if(pGraph->GetSubGraphList().ChangeSubGraphData(targetSubGraphName, sourceSubGraphData))
			return true;
	}

	return false;
}

// subGraph 라인 굵기 변경.
bool CVertScaleGroup::ChangeSubGraphPenThickness(const CString& targetGraphName, const int nSubGraphIndex, const CPenThickness& penThickness)
{
	POSITION pos = m_GraphList.GetHeadPosition();
	while(pos != NULL){
		CGraphImp* pGraph = m_GraphList.GetNext(pos);
		assert(pGraph != NULL);
		if(pGraph->GetGraphData().GetGraphName() != targetGraphName)
			continue;

		// 모든 subGraph를 바꾼다.
		if(nSubGraphIndex == -1)
			pGraph->GetSubGraphList().SetAllSubGraphPenThickness(penThickness);
		else
			pGraph->GetSubGraphList().SetPenThickness(nSubGraphIndex, penThickness);
		
		return true;
	}

	return false;
}

// subGraph title 변경.
bool CVertScaleGroup::ChangeSubGraphTitle(const CString& orgSubGraphTitle, const CString& changeSubGraphTitle)
{
	POSITION pos = m_GraphList.GetHeadPosition();

	while(pos != NULL){
		CGraphImp* pGraph = m_GraphList.GetNext(pos);
		assert(pGraph != NULL);
		if(pGraph->GetSubGraphList().SetTitle(orgSubGraphTitle, changeSubGraphTitle))
			return true;
	}
	return false;
}

// subGraph 색 변경.
bool CVertScaleGroup::ChangeAllHeadSubGraphColor(const CList<CGraphColor, CGraphColor&>& graphColorList, POSITION& colorPos) const
{
	if(colorPos == NULL)
		colorPos = graphColorList.GetHeadPosition();

	bool bResult = false;
	POSITION pos = m_GraphList.GetHeadPosition();
	while(pos != NULL)
	{
		CGraphImp* pGraph = m_GraphList.GetNext(pos);
		assert(pGraph != NULL);
		const CGraphColor& graphColor = graphColorList.GetNext(colorPos);
		if(pGraph->GetSubGraphList().SetSubGraphColor(0, graphColor))
			bResult = true;
	}

	return bResult;
}

// ----------------------------------------------------------------------------------------
// 이미 생성되어 있는 graph에 data를 setting.
void CVertScaleGroup::ModifyGraphDataInVertScaleGroup( char *p_szVertScaleGroup, const bool doesMaintainOldScale)
{
	if( !p_szVertScaleGroup) return;
	if( !*p_szVertScaleGroup) return;

	// 새로운 scale 적용.
	if( !doesMaintainOldScale)
	{
		char *szGraphData = strstr( p_szVertScaleGroup, "\r\n");
		if( szGraphData)
		{
			*szGraphData = '\0';
			m_vertScale.SetScaleData( p_szVertScaleGroup, m_pMainBlockBaseData->GetParent_Handle());
			*szGraphData = '\r';
		}
	}

	char cNextGraphData = 0;
	char *szGraphData = strstr( p_szVertScaleGroup, "Graph;\r\n");
	char *szNextGraphData = NULL;
	POSITION posGraph = m_GraphList.GetHeadPosition();
	while( szGraphData)
	{
		szNextGraphData = strstr( szGraphData, "GraphEnd;\r\n");
		if( szNextGraphData)
		{
			szNextGraphData += 11;		// "GraphEnd;\r\n"
			cNextGraphData = *szNextGraphData;
			*szNextGraphData = '\0';
		}

		CGraphImp *pGraph = NULL;
		if( posGraph) pGraph = m_GraphList.GetNext( posGraph);
		if( pGraph) ModifyGraph( pGraph, szGraphData);
		else
		{
			posGraph = NULL;
			MakeGraph( szGraphData);
		}

		if( szNextGraphData)
		{
			*szNextGraphData = cNextGraphData;
			szGraphData = strstr( szNextGraphData, "Graph;\r\n");
		}
		else szGraphData = NULL;
	}
}


// -> IndicatorInfo를 gaphData(subGraph포함)로 변경.
bool CVertScaleGroup::ChangeAllIndicatorInfoFromGraphData()
{
	bool bResult = false;
	POSITION pos = m_GraphList.GetHeadPosition();
	while(pos != NULL){
		CGraphImp* pGraph = m_GraphList.GetNext(pos);
		assert(pGraph != NULL);
		if(pGraph->ChangeIndicatorInfoFromGraphData())
			bResult = true;
	}
	return bResult;
}

BOOL CVertScaleGroup::ChangeAllGraphDataFromIndicatorInfo( BOOL p_bWithCalculate)
{
	BOOL bResult = FALSE;
	POSITION pos = m_GraphList.GetHeadPosition();
	while(pos != NULL){
		CGraphImp* pGraph = m_GraphList.GetNext(pos);
		assert(pGraph != NULL);
		if(pGraph->ChangeGraphDataFromIndicatorInfo( p_bWithCalculate ? true : false))
			bResult = TRUE;
	}
	return bResult;
}

// -> 해당 graph를 Map Default IndicatorInfo로 변경
bool CVertScaleGroup::ChangeAllMapDefaultIndicatorInfo( CIndicatorList* pIndicatorList)
{
	bool bResult = false;
	POSITION pos = m_GraphList.GetHeadPosition();
	while(pos != NULL)
	{
		CGraphImp* pGraph = m_GraphList.GetNext(pos);
		assert(pGraph != NULL);
		if(pGraph->ChangeMapDefaultIndicatorInfo( pIndicatorList))
			bResult = true;
	}
	return bResult;
}

// ----------------------------------------------------------------------------------------
// 상위 class 주소
void CVertScaleGroup::SetVertScaleGroupList(CVertScaleGroupList* pVertScaleGroupList)
{
	m_pVertScaleGroupList = pVertScaleGroupList;
}

void CVertScaleGroup::SetMainBlockBaseData(CMainBlockBaseData* pMainBlockBaseData)
{
	m_pMainBlockBaseData = pMainBlockBaseData;

	POSITION pos = m_GraphList.GetHeadPosition();
	while(pos != NULL){
		CGraphImp* pGraph = m_GraphList.GetNext(pos);
		assert(pGraph != NULL);
		pGraph->SetMainBlockBaseData(m_pMainBlockBaseData);
	}
}

// 세로 scale 값 바꾸기.
void CVertScaleGroup::SetVerticalScale(const CVerticalScale& vertScale)
{
	m_vertScale = vertScale;
}

// subGraph를 보이기 setting.
void CVertScaleGroup::SetAllGraphsHiding(const bool bIsHiding)
{
	bool bResult = false;
	POSITION pos = m_GraphList.GetHeadPosition();
	while(pos != NULL){
		CGraphImp* pGraph = m_GraphList.GetNext(pos);
		assert(pGraph != NULL);
		if(pGraph->GetSubGraphList().SetAllHiding(bIsHiding))
			bResult = true;
	}

	//숨길경우 Min/Max 를 다시 구한다.
	if(bResult)
		CalculateDisplayDataMinMax();
}

bool CVertScaleGroup::SetSubGraphHiding(const CString& subGraphTitle, const bool bIsHiding)
{
	bool bResult = false;
	POSITION pos = m_GraphList.GetHeadPosition();
	while(pos != NULL){
		CGraphImp* pGraph = m_GraphList.GetNext(pos);
		assert(pGraph != NULL);
		if(pGraph->GetSubGraphList().SetHiding(subGraphTitle, bIsHiding))
		{
			bResult = true;

			// (2007/3/2 - Seung-Won, Bae) Update Indicator Info.
			pGraph->ChangeIndicatorInfoFromGraphData();
		}
	}

	//숨길경우 Min/Max 를 다시 구한다.
	if(bResult)
		CalculateDisplayDataMinMax();
	return bResult;
}

bool CVertScaleGroup::SetSubGraphHiding(const CString& strGraphName, const int nSubGraphIndex, const bool bIsHiding)
{
	CGraphImp* pGraph = FindGraph(strGraphName);
	if(pGraph == NULL)
		return false;

	if(!pGraph->GetSubGraphList().SetHiding(nSubGraphIndex, bIsHiding))
		return false;

	//숨길/보임경우 Min/Max 를 다시 구한다.
	CalculateDisplayDataMinMax();
	return true;
}

// ----------------------------------------------------------------------------------------
// graph의 indicatorinfo를 초기화(NULL)
void CVertScaleGroup::Initialize_IndicatorInfo(const bool bIsNULL)
{
	POSITION pos = m_GraphList.GetHeadPosition();
	while(pos != NULL){
		CGraphImp* pGraph = m_GraphList.GetNext(pos);
		assert(pGraph != NULL);
		pGraph->Initialize_IndicatorInfo(bIsNULL);
	}
}

// graph(subGraph들)의 packet들을 초기화(NULL)
void CVertScaleGroup::Initialize_AllPackets(const bool bIsCalculate)
{
	POSITION pos = m_GraphList.GetHeadPosition();
	while(pos != NULL){
		CGraphImp* pGraph = m_GraphList.GetNext(pos);
		assert(pGraph != NULL);
		pGraph->Initialize_AllPackets(bIsCalculate);
	}
}

// strPacketName 가 포함된 모든 subGraph의 packet들을 초기화
bool CVertScaleGroup::Initialize_Packet(const CString& strPacketName, const bool bIsCalculate)
{
	bool bResult = false;
	POSITION pos = m_GraphList.GetHeadPosition();
	while(pos != NULL){
		CGraphImp* pGraph = m_GraphList.GetNext(pos);
		assert(pGraph != NULL);
		if(pGraph->IsExistencePacketName(strPacketName)){
			pGraph->Initialize_AllPackets(bIsCalculate);
			bResult = true;
		}
	}
	return bResult;
}

// 지표를 계산한다.
void CVertScaleGroup::CalculateAllGraphs(const CCalculateBaseData::CALCULATETYPE eCalType)
{
	POSITION pos = m_GraphList.GetHeadPosition();
	while(pos != NULL)
	{
		CGraphImp* pGraph = m_GraphList.GetNext(pos);
		assert(pGraph != NULL);
		pGraph->CalculateGraph(eCalType, false, false, true);
	}

	CalculateDisplayDataMinMax();
}

bool CVertScaleGroup::CalculateAllGraphs(const CList<CString, CString>& packetNameList, const CCalculateBaseData::CALCULATETYPE eCalType, const bool bIsHeadGraph)
{
	bool bResult = false;
	POSITION pos = m_GraphList.GetHeadPosition();
	while(pos != NULL)
	{
		CGraphImp* pGraph = m_GraphList.GetNext(pos);
		assert(pGraph != NULL);
		if(!pGraph->IsExistencePacketName(packetNameList))
			continue;

		pGraph->CalculateGraph(eCalType, true, false, false);

		bResult = true;
		if(bIsHeadGraph)
			return true;
	}

	return bResult;
}

// vert scale Min/Max 를 계산한다.
// 함수 호출 시점 => 상위에서 호출 되는 경우는 CVertScaleGroup
//					CGraphImp 이하에서 호출 되는 경우는 CGraphImp
// 계산 시점 =>
// 1. 지표 계산(TR, RealData) - CVertScaleGroup::CalculateAllGraphs()
//		- CVertScaleGroupList::CalculateGraph()
// 2. Scroll - CMainBlockImp::SetDataRegionInBlock()
// 3. 지표설정 dlg 관련 부분 - CGraphImp::CalculateGraph()
// 4. Graph 추가, 삭제, 숨김, 이동 - CVertScaleGroup::AddTail()
// 5. 세로 Scale Data 변경
void CVertScaleGroup::CalculateDisplayDataMinMax()
{
	// (2007/1/14 - Seung-Won, Bae) Get the Base Packet for Default Scale Min/Max
	m_pBaseGraph = FindBaseGraphForDrawingVertScaleData();
	if( m_pBaseGraph)	m_pBasePacket = m_pBaseGraph->GetSubGraphList().GetHeadPacket();
	else				m_pBasePacket = NULL;

	// (2006/11/11 - Seung-Won, Bae) Use Y Scale Min/Max Setting
	if( m_vertScale.GetVertScaleMinMaxType() == CScaleBaseData::VERT_MINMAX_OF_USER)
	{
		// (2006/11/12 - Seung-Won, Bae) Check Min/Max reverse setting.
		if( m_vertScale.GetVertScaleMax() < m_vertScale.GetVertScaleMin())
		{
			double dMin = m_vertScale.GetVertScaleMin();
			m_vertScale.SetVertScaleMin( m_vertScale.GetVertScaleMax());
			m_vertScale.SetVertScaleMax( dMin);
		}

		m_displayDataMinMax.SetDisplayData( m_vertScale.GetVertScaleMin(), m_vertScale.GetVertScaleMax());
	}
	else
	{
		// (2006/10/15 - Seung-Won, Bae) Hide Empty Scale Text (Make Default Scale Range to 0:0)
		//	m_displayDataMinMax.SetDisplayData(-20.0, 20.0);
		//20100515 이문수 최소값을 잘못 얻어와서 계산루프를 오래 돌게되는 오류 수정>>
		//m_displayDataMinMax.SetDisplayData( -HUGE_VAL, -HUGE_VAL);
		m_displayDataMinMax.SetDisplayData( HUGE_VAL, -HUGE_VAL);
		//20100515 이문수 <<

		// <가격차트> 가 있고 <가격차트의 min/max> 일 경우만..
		if(!GetDisplayDataMinMaxInOnlyPrice(m_displayDataMinMax))
			m_displayDataMinMax = GetDisplayDataMinMaxInAll();
	}

	// (2006/11/19 - Seung-Won, Bae) Deny Empty VScale
	double dMax = m_displayDataMinMax.GetDisplayMax();
	double dMin = m_displayDataMinMax.GetDisplayMin();
	if( dMax == dMin && m_displayDataMinMax.IsInitialized() && m_pBasePacket)
	{
		CString strType = m_pBasePacket->GetType();
		strType.Replace( "x", "");
		strType.Replace( "X", "");
		strType.Replace( '%', ' ');
		strType.Replace( '$', ' ');
		// (2010/4/17 - Seung-Won, Bae) Check with Graphic Character 'X'
		strType.Replace( "×", "");
		double dGap = atof( strType);
		dMin -= dGap;
		dMax += dGap;
		m_displayDataMinMax.SetDisplayData( dMin, dMax);
	}

	// (2007/7/16 - Seung-Won, Bae) Up/Down Space
	if( m_vertScale.GetVertScaleMinMaxType() != CScaleBaseData::VERT_MINMAX_OF_USER && dMax != dMin && m_displayDataMinMax.IsInitialized())
	{
		int nUpSpace = 0, nDownSpace = 0;
		m_vertScale.GetSpace( nUpSpace, nDownSpace);
		if( 0 != nUpSpace || 0 != nDownSpace)
		{
			double dHeight = dMax - dMin;

			// (2007/7/18 - Seung-Won, Bae) Do not Support Down Space for Vertical BarChart (0-Base BarChart)
			CGraphSearchInfo_VisibleTypeStyleDrawStyle gsi;
			gsi.m_eGraphType = CGraphBaseData::Bar_Type;
			gsi.m_nGraphStyle = CGraphBaseData::VerticalBar;
			gsi.m_nDrawStyle = CGraphBaseData::Bar_Draw;
			if( FindGraph( &gsi)) nDownSpace = 0;

			double nHeight = 100 - nUpSpace - nDownSpace;
			dMax += ( dHeight * nUpSpace) / nHeight;
			dMin -= ( dHeight * nDownSpace) / nHeight;

			m_displayDataMinMax.SetDisplayData( dMin, dMax);
		}
	}

	// (2006/11/12 - Seung-Won, Bae) Update Min/Max Scale Setting Value
	m_vertScale.SetVertScaleMin( dMin);
	m_vertScale.SetVertScaleMax( dMax);
}

// ----------------------------------------------------------------------------------------
// 상위 class 주소
CMainChartFormulate* CVertScaleGroup::GetMainChartFormulate() const
{
	if( !m_pMainBlockBaseData) return NULL;
	return m_pMainBlockBaseData->GetMainAttributes().GetMainChartFormulate();
}

CBlockImp* CVertScaleGroup::GetBlock() const
{
	if(m_pVertScaleGroupList == NULL)
		return NULL;
	return m_pVertScaleGroupList->GetBlock();
}

CVertScaleGroupList* CVertScaleGroup::GetVertScaleGroupList() const
{
	return m_pVertScaleGroupList;
}

// 세로 scale
CVerticalScale& CVertScaleGroup::GetVertScale()
{
	return m_vertScale;
}

// 세로 scale
const CVerticalScale& CVertScaleGroup::GetVertScale() const
{
	return m_vertScale;
}

// 모든 graph의 모든 data
void CVertScaleGroup::GetVertScaleGroupAllData( CStringList &p_slVertScaleGroupData) const
{
	//>> JS.Kim_20100929  solomon.. PAIR RQ는 저장하지 않도록 수정..
	/*
	p_slVertScaleGroupData.AddTail( "VertScaleGroup;\r\n");

	p_slVertScaleGroupData.AddTail( CString());
	CString &strVertScaleData = p_slVertScaleGroupData.GetTail();
	m_vertScale.GetScaleAllData( strVertScaleData);


	POSITION pos = m_GraphList.GetHeadPosition();
	while( pos)
	{
		CGraphImp* pGraph = m_GraphList.GetNext(pos);
		assert( pGraph != NULL);
		pGraph->GetStringGraphData( p_slVertScaleGroupData);
	}
	
	p_slVertScaleGroupData.AddTail( "VertScaleGroupEnd;\r\n");
	*/
	CStringList slVertScaleGroupData;
	POSITION pos = m_GraphList.GetHeadPosition();
	while( pos)
	{
		CGraphImp* pGraph = m_GraphList.GetNext(pos);
		assert( pGraph != NULL);
		pGraph->GetStringGraphData( slVertScaleGroupData);
	}
	if( slVertScaleGroupData.IsEmpty() )		return;

	p_slVertScaleGroupData.AddTail( "VertScaleGroup;\r\n");

	p_slVertScaleGroupData.AddTail( CString());
	CString &strVertScaleData = p_slVertScaleGroupData.GetTail();
	m_vertScale.GetScaleAllData( strVertScaleData);

	p_slVertScaleGroupData.AddTail(&slVertScaleGroupData);
	
	p_slVertScaleGroupData.AddTail( "VertScaleGroupEnd;\r\n");
	//<<
}

bool CVertScaleGroup::GetAllIndicatorName(CList<CString, CString>& strIndicatorNameList) const
{
	bool bResult = false;
	POSITION pos = m_GraphList.GetHeadPosition();
	while(pos != NULL){
		CGraphImp* pGraph = m_GraphList.GetNext(pos);
		assert(pGraph != NULL);
		CString strIndicatorName = pGraph->GetGraphData().GetIndicatorName();
		if( strIndicatorName.IsEmpty())
			continue;

		strIndicatorNameList.AddTail( strIndicatorName);
		bResult = true;
	}
	return bResult;
}

bool CVertScaleGroup::GetAllGraphName(CList<CString, CString>& strGraphNameList) const
{
	bool bResult = false;
	POSITION pos = m_GraphList.GetHeadPosition();
	while(pos != NULL){
		CGraphImp* pGraph = m_GraphList.GetNext(pos);
		assert(pGraph != NULL);
		CString strGraphName = pGraph->GetGraphData().GetGraphName();
		if( strGraphName.IsEmpty())
			continue;

		strGraphNameList.AddTail( strGraphName);
		bResult = true;
	}
	return bResult;
}

// ----------------------------------------------------------------------------------------
// graph 갯수
int CVertScaleGroup::GetGraphCount() const
{
	return m_GraphList.GetCount();
}

// 모든 graph 함수명
CString CVertScaleGroup::GetGraphsGraphName() const
{
	CString strData;
	POSITION pos = m_GraphList.GetHeadPosition();
	while(pos != NULL){
		CGraphImp* pGraph = m_GraphList.GetNext(pos);
		assert(pGraph != NULL);
		CString strGraphName = pGraph->GetGraphData().GetGraphName();
		if(!strGraphName.IsEmpty())
			strData += (strGraphName + ';');
	}
	return strData;
}

// graph 색
// 모든 block에 있는 모든 graph의 head subGraph color 가져오기.
bool CVertScaleGroup::GetAllHeadSubGraphColor(CList<CGraphColor, CGraphColor&>& graphColorList) const
{
	bool bResult = false;
	POSITION pos = m_GraphList.GetHeadPosition();
	while(pos != NULL)
	{
		CGraphImp* pGraph = m_GraphList.GetNext(pos);
		assert(pGraph != NULL);
		CGraphColor* pGraphColor = pGraph->GetSubGraphList().GetGraphColor(0);
		if(pGraphColor == NULL)
			continue;

		graphColorList.AddTail(*pGraphColor);
		bResult = true;
	}

	return bResult;
}

COLORREF CVertScaleGroup::GetGraphColor1(const int nGraphIndex, const int nSubGraphIndex) const
{
	CGraphImp* pGraph = GetGraph(nGraphIndex);
	if(pGraph == NULL)
		return RGB(255, 255, 255);

	return pGraph->GetSubGraphList().GetGraphColor1(nSubGraphIndex);
}

// subGraph title region
// 타이틀 여러줄로 표시 : 복수종목 - ojtaso (20070227)
CRect CVertScaleGroup::GetSubGraphTitleRegion(CDC* pDC, const CSubGraph* pSubGraph, const CRect& graphRegion, int& preTitleEndPosition, int& nPreLineCount) const
{
	POSITION pos = m_GraphList.GetHeadPosition();

	while(pos != NULL){
		CGraphImp* pGraph = m_GraphList.GetNext(pos);
		assert(pGraph != NULL);
		// 타이틀 여러줄로 표시 : 복수종목 - ojtaso (20070227)
		CRect region = pGraph->GetSubGraphList().GetSubGraphTitleRegion(pDC, pSubGraph, graphRegion, preTitleEndPosition, nPreLineCount);
		if(!region.IsRectEmpty())
			return region;
	}

	return CRect(0, 0, 0, 0);
}

// ----------------------------------------------------------------------------------------
// 세로 scale의 폭을 구한다.
// -> 같은 block안에 있는 graph들의 길이 scale의 길이중 최대길이로 한다.
// (2008/6/13 - Seung-Won, Bae) for Support BuySell Overhanging Supply.
void CVertScaleGroup::GetLeftVertScaleRegionWidth(CDC* pDC, int &p_nLeftWidth, BOOL p_bIsDualScale) const
{
	// 세로 scale의 width 구할지의 여부
	// scale 값이 없으면 scale 을 그리지 않는다.
	if( m_pMainBlockBaseData == NULL || m_vertScale.GetScaleData().IsEmpty()) return;

	//scale position = 왼쪽, 양쪽일 경우에만.
	if( m_vertScale.GetVertScalePosition() != CScaleBaseData::VERT_LEFT
		&& m_vertScale.GetVertScalePosition() != CScaleBaseData::VERT_BOTH) return;

	CVertScaleMaxWidthCalculatorData calData(false, m_pMainBlockBaseData->GetDisplayAttributes(), m_vertScale, p_bIsDualScale);
	POSITION pos = m_GraphList.GetHeadPosition();
	while(pos != NULL)
	{
		CGraphImp* pGraph = m_GraphList.GetNext(pos);
		assert(pGraph != NULL);
		int nTemp = pGraph->GetLeftVertScaleRegionWidth(pDC, &calData);
		if( nTemp > p_nLeftWidth) p_nLeftWidth = nTemp;
	}
}
void CVertScaleGroup::GetRightVertScaleRegionWidth( CDC* pDC, int &p_nRightWidth, BOOL p_bIsDualScale) const
{
	// scale 값이 없으면 scale 을 그리지 않는다.
	if( m_pMainBlockBaseData == NULL || m_vertScale.GetScaleData().IsEmpty()) return;

	//scale position = 오른쪽, 양쪽일 경우에만.
	if( m_vertScale.GetVertScalePosition() != CScaleBaseData::VERT_RIGHT 
		&& m_vertScale.GetVertScalePosition() != CScaleBaseData::VERT_BOTH) return;

	CVertScaleMaxWidthCalculatorData calData(true, m_pMainBlockBaseData->GetDisplayAttributes(), m_vertScale, p_bIsDualScale);
	POSITION pos = m_GraphList.GetHeadPosition();
	while(pos != NULL)
	{
		CGraphImp* pGraph = (CGraphImp*) m_GraphList.GetNext(pos);
		assert(pGraph != NULL);
		int nTemp = pGraph->GetRightVertScaleRegionWidth(pDC, &calData);
		if(nTemp > p_nRightWidth) p_nRightWidth = nTemp;
	}
}

// 해당 RQ만 작업 : 복수종목 - ojtaso (20070420)
void CVertScaleGroup::GetSubGraphViewDataList(LPCTSTR lpszRQ, CSubGraphViewDataList& subGraphViewDataList) const
{
	POSITION pos = m_GraphList.GetHeadPosition();
	while(pos != NULL){
		CGraphImp* pGraph = (CGraphImp*) m_GraphList.GetNext(pos);
		assert(pGraph != NULL);

		if( lpszRQ) if( *lpszRQ) if( ::lstrcmp( pGraph->GetGraphRQ(), lpszRQ))
			continue;
		pGraph->GetSubGraphViewDataList(subGraphViewDataList);
	}
}

// ----------------------------------------------------------------------------------------
// 현재 보이는 화면에서의 min, max값 구한다. ==> y축의 값
CDisplayDataMinMax CVertScaleGroup::GetDisplayDataMinMax() const
{
	return m_displayDataMinMax;
}

// ----------------------------------------------------------------------------------------
// graph title name이 모두 empty인지 구하기
bool CVertScaleGroup::IsViewableGraphTitleEmpty() const
{
	POSITION pos = m_GraphList.GetHeadPosition();

	while(pos != NULL){
		CGraphImp* pGraph = (CGraphImp*) m_GraphList.GetNext(pos);
		assert(pGraph != NULL);
		if(!pGraph->GetSubGraphList().IsViewableSubGraphTitleEmpty())
			return false;
	}
	return true;
}

// 이미 존재하는 graph인지 체크
bool CVertScaleGroup::IsExistenceGraph( const CString &strGraphName) const
{
	POSITION pos = m_GraphList.GetHeadPosition();
	while(pos != NULL){
		CGraphImp* pGraph = (CGraphImp*) m_GraphList.GetNext(pos);
		assert(pGraph != NULL);
		if( pGraph->GetGraphData().GetGraphName() == strGraphName) return true;
	}
	return false;
}

// RQ를 가지고 있는 Graph 체크 : 복수종목 - ojtaso (20061224)
bool CVertScaleGroup::IsExistenceGraph(LPCTSTR lpszRQ, const CString &strGraphName) const
{
	POSITION pos = m_GraphList.GetHeadPosition();
	while(pos != NULL){
		CGraphImp* pGraph = (CGraphImp*) m_GraphList.GetNext(pos);
		assert(pGraph != NULL);
		if(!::lstrcmp(lpszRQ, pGraph->GetGraphRQ()) && 
			pGraph->GetGraphData().GetGraphName() == strGraphName) return true;
	}
	return false;
}

// CPacketRQ*로 Graph 체크 : 복수종목 - ojtaso (20070109)
bool CVertScaleGroup::IsExistenceGraph(LPVOID pPacketRQ, const CString &strGraphName) const
{
	POSITION pos = m_GraphList.GetHeadPosition();
	while(pos != NULL){
		CGraphImp* pGraph = (CGraphImp*) m_GraphList.GetNext(pos);
		assert(pGraph != NULL);
		if(pGraph->ComparePacketRQ(pPacketRQ) && 
			pGraph->GetGraphData().GetGraphName() == strGraphName) return true;
	}
	return false;
}

// 이미 존재하는 graph인지 체크
bool CVertScaleGroup::IsExistenceIndicator( const CString &strIndicatorName) const
{
	POSITION pos = m_GraphList.GetHeadPosition();
	while(pos != NULL){
		CGraphImp* pGraph = (CGraphImp*) m_GraphList.GetNext(pos);
		assert(pGraph != NULL);
		if( pGraph->GetGraphData().GetIndicatorName() == strIndicatorName) return true;
	}
	return false;
}

// RQ를 가지고 있는 Graph 체크 : 복수종목 - ojtaso (20061224)
bool CVertScaleGroup::IsExistenceIndicator(LPCTSTR lpszRQ, const CString &strIndicatorName) const
{
	POSITION pos = m_GraphList.GetHeadPosition();
	while(pos != NULL){
		CGraphImp* pGraph = (CGraphImp*) m_GraphList.GetNext(pos);
		assert(pGraph != NULL);
		if(!::lstrcmp(lpszRQ, pGraph->GetGraphRQ()) && 
			pGraph->GetGraphData().GetIndicatorName() == strIndicatorName) return true;
	}
	return false;
}

// 해당 graph Type가 존재하는지 체크
bool CVertScaleGroup::IsViewableGraphType(const CGraphBaseData::GRAPHTYPE eGraphType) const
{
	POSITION pos = m_GraphList.GetHeadPosition();
	while(pos != NULL){
		CGraphImp* pGraph = (CGraphImp*) m_GraphList.GetNext(pos);
		assert(pGraph != NULL);
		if(pGraph->GetSubGraphList().IsViewableGraphType(eGraphType))
			return true;
	}
	return false;
}

// 해당 graph style가 존재하는지 체크
bool CVertScaleGroup::IsViewableGraphStyle(const CGraphBaseData::GRAPHTYPE eGraphType, const int nGraphStyle) const
{
	POSITION pos = m_GraphList.GetHeadPosition();
	while(pos != NULL)
	{
		CGraphImp* pGraph = (CGraphImp*) m_GraphList.GetNext(pos);
		assert(pGraph != NULL);
		if(pGraph->GetSubGraphList().IsViewableGraphStyle(eGraphType, nGraphStyle))
			return true;
	}
	return false;
}

// 해당 graph draw style가 존재하는지 체크
bool CVertScaleGroup::IsViewableGraphDrawStyle(const CGraphBaseData::GRAPHTYPE eGraphType, const int nGraphStyle, const int nDrawStyle) const
{
	POSITION pos = m_GraphList.GetHeadPosition();
	while(pos != NULL){
		CGraphImp* pGraph = (CGraphImp*) m_GraphList.GetNext(pos);
		assert(pGraph != NULL);
		if(pGraph->GetSubGraphList().IsViewableGraphDrawStyle(eGraphType, nGraphStyle, nDrawStyle))
			return true;
	}
	return false;
}

// 모든 graph 가 hide 인지 체크
bool CVertScaleGroup::DoesAreHidingAllGraphs() const
{
	POSITION pos = m_GraphList.GetHeadPosition();
	while(pos != NULL){
		CGraphImp* pGraph = m_GraphList.GetNext(pos);
		assert(pGraph != NULL);
		if(!pGraph->GetSubGraphList().AreHidingAllSubGraphs())
			return false;
	}
	return true;
}

// ----------------------------------------------------------------------------------------
// exceptionGraph 를 제외하고 같은 indicatorInfo를 사용하는지 유무
bool CVertScaleGroup::DoesUseEqualIndicatorInfo(const CIndicatorInfo* pIndiInfo, const CGraphImp* pExceptionGraph) const
{
	POSITION pos = m_GraphList.GetHeadPosition();
	while(pos != NULL){
		CGraphImp* pGraph = m_GraphList.GetNext(pos);
		assert(pGraph != NULL);
		if(pGraph == pExceptionGraph)
			continue;
		if(pGraph->GetIndicatorInfo() == pIndiInfo)
			return true;
	}
	return false;
}

// ---------------------------------------------------------------------------------------
// 세로 scale 그리기.
void CVertScaleGroup::DrawVertScale(CDC* pDC, CDrawingVertScaleData* pDrawingVertScaleData, const CDrawingRegion& drawingRegion)
{
	if(!IsVertScaleDrawing() || pDrawingVertScaleData == NULL) return;

	// (2008/12/19 - Seung-Won, Bae) Calc it in VScale Draw for Including BaseLine.
	//real data 처리때문에 Min/Max를 계산한다.
	CalculateDisplayDataMinMax();

	// (2006/11/19 - Seung-Won, Bae) Deny to draw of Not-Initialized Scale.
	if( !m_displayDataMinMax.IsInitialized()) return;

	SetDrawingVertScaleData(pDrawingVertScaleData);
	CBlockScaleDrawer().DrawVertScale(pDC, pDrawingVertScaleData, drawingRegion.GetVertScaleRegion(), m_vertScale, m_pMainBlockBaseData->GetParent_Handle(), m_pMainBlockBaseData->GetMainAttributes().GetChartMode());
	InitializeDrawingVertScaleData(pDrawingVertScaleData);
}

// graph 그리기.
// 타이틀 여러줄로 표시 : 복수종목 - ojtaso (20070227)
void CVertScaleGroup::DrawGraphs(CDC* pDC, CDrawingGraphData* pDrawingGraphData, int& preGraphTitleEndPosition, int& nEndLimit, int& nGraphTitleEndLine)
{
	if( pDrawingGraphData == NULL) return;

	int nPreGraphTitleEndLine = 1;

	// 선택된 차트가 없으면 가격차트를 다시 그림 - ojtaso (20080104)
	if(!m_lpSelectedGraph)
		m_lpSelectedGraph = FindGraph( _MTEXT( C2_PRICE_CHART));

	int nThread_Flag = 0;

	// (2009/2/20 - Seung-Won, Bae) check no graph region
	CMainBlockImp *pMainBlock = m_pMainBlockBaseData->GetMainAttributes().GetMainBlockImpl();
	CXScaleManager *pXScaleManager = pMainBlock->GetXScaleManager();
	BOOL bNoRegion = ( pXScaleManager->GetScreenWidth( GetVertScaleGroupList()->GetBlock()->GetBlocksColumn()->GetColumnIndex()) <= 0);

	//NH-alzioyes.100616 대기매물은 배경으로만 그려지도록.
	bool bExtraDraw = false;
	if( !bNoRegion) 
	{
		CString szIndiName;
		if(nThread_Flag != THREAD_DRAWING)   //2009.01.06 김성조 - 그래프 그릴시에만 동작. 두번 그리는 현상 방지.
		{
			if(m_lpSelectedGraph && m_lpSelectedGraph->GetIndicatorInfo())
			{
				szIndiName = m_lpSelectedGraph->GetIndicatorInfo()->GetIndicatorName();
				if(szIndiName.Compare(_MTEXT( C2_OVERHANGING_SUPPLY))==0)
				{
					pDrawingGraphData->SetDisplayDataVertMinMax(m_displayDataMinMax);
					m_lpSelectedGraph->DrawGraphAndSignal( pDC, pDrawingGraphData);
					bExtraDraw = true;
				}
			}
		}
	}

	POSITION pos = m_GraphList.GetHeadPosition();
	while(pos != NULL){
		CGraphImp* pGraph = m_GraphList.GetNext(pos);
		assert(pGraph != NULL);
		pDrawingGraphData->SetDisplayDataVertMinMax(m_displayDataMinMax);
		// 타이틀 여러줄로 표시 : 복수종목 - ojtaso (20070227)
		
		nThread_Flag = pMainBlock->GetXScaleManager()->GetThreadFlag();
		
		// (2009/1/7 - Seung-Won, Bae) Draw all graph title on drawing-thread.
		if( nThread_Flag == THREAD_DRAWING)
			pGraph->DrawGraphTitle( pDC, pDrawingGraphData, preGraphTitleEndPosition, nEndLimit, nGraphTitleEndLine);

		// (2009/1/7 - Seung-Won, Bae) Do not call the selected graph on drawing. it will be done at last.
		if( !bNoRegion) if( pGraph != m_lpSelectedGraph || nThread_Flag != THREAD_DRAWING)
		{
			// (2009/10/9 - Seung-Won, Bae) for Thread Draw.
			if( THREAD_CALCULATION != nThread_Flag || 1 != pXScaleManager->GetCalculationMode())
				pGraph->DrawGraphAndSignal( pDC, pDrawingGraphData);
			else
			{
				pMainBlock->m_nThreadCount++;
				pGraph->m_pDC = pDC;
				pGraph->m_pDrawingGraphData = pDrawingGraphData;
				//20100506 이문수 교착상태의심 쓰레드 사용중단 >>
				//AfxBeginThread( DrawGraphAndSignal, pGraph);
				DrawGraphAndSignal( pGraph );

			}
		}

//>> 타이틀 여러줄 표시시 스케일 영역에 가리지 않도록 처리 - vntsorl (20090106)
		if(nEndLimit <= preGraphTitleEndPosition + MARGIN * 2)
//<< 타이틀 여러줄 표시시 스케일 영역에 가리지 않도록 처리 - vntsorl (20090106)
		{
			preGraphTitleEndPosition = VSGL_TITLE_START_POSITION;
			nGraphTitleEndLine++;
		}

		if(nGraphTitleEndLine > nPreGraphTitleEndLine)
			m_pVertScaleGroupList->RecalculateTitleRegion(TRUE);

		nPreGraphTitleEndLine = nGraphTitleEndLine;
	}	

	// 선택된 그래프 다시 그림 (20071007)
	//NH-alzioyes.100616 대기매물은 배경으로만 그려지도록.
	if( !bNoRegion) 
	{
		if(bExtraDraw)
		{
			CString szIndiName;
			if(nThread_Flag == THREAD_DRAWING)
			{
				if(m_lpSelectedGraph && m_lpSelectedGraph->GetIndicatorInfo())
				{
					szIndiName = m_lpSelectedGraph->GetIndicatorInfo()->GetIndicatorName();
					if(szIndiName.Compare(_MTEXT( C2_OVERHANGING_SUPPLY))!=0)
						m_lpSelectedGraph->DrawGraphAndSignal( pDC, pDrawingGraphData);
				}
			}
		}
		else
		{
			if(nThread_Flag == THREAD_DRAWING)   //2009.01.06 김성조 - 그래프 그릴시에만 동작. 두번 그리는 현상 방지.
				if(m_lpSelectedGraph)
				m_lpSelectedGraph->DrawGraphAndSignal( pDC, pDrawingGraphData);
		}
	}
// 	if( !bNoRegion) 
// 		if(nThread_Flag == THREAD_DRAWING)   //2009.01.06 김성조 - 그래프 그릴시에만 동작. 두번 그리는 현상 방지.
// 			if(m_lpSelectedGraph)
// 				m_lpSelectedGraph->DrawGraphAndSignal( pDC, pDrawingGraphData);

// 	//NH-alzioyes.100616 대기매물은 배경으로만 그려지도록.
// 	if( !bNoRegion) 
// 	{
// 		CString szIndiName;
// 		if(nThread_Flag == THREAD_DRAWING)
// 		{
// 			if(m_lpSelectedGraph && m_lpSelectedGraph->GetIndicatorInfo())
// 			{
// 				szIndiName = m_lpSelectedGraph->GetIndicatorInfo()->GetIndicatorName();
// 				if(szIndiName.Compare(_MTEXT( C2_OVERHANGING_SUPPLY))!=0)
// 					m_lpSelectedGraph->DrawGraphAndSignal( pDC, pDrawingGraphData);
// 			}
// 		}
// 	}
}

// (2009/10/9 - Seung-Won, Bae) for Thread Draw.
UINT CVertScaleGroup::DrawGraphAndSignal( LPVOID pParam)
{
	CGraphImp *pGraph = ( CGraphImp *)pParam;
	pGraph->DrawGraphAndSignal( pGraph->m_pDC, pGraph->m_pDrawingGraphData);
	pGraph->GetMainBlock()->m_nThreadCount--;
	return 0;
}

// ---------------------------------------------------------------------------------------
// graph 찾기
CGraphImp* CVertScaleGroup::FindGraph(const CString& strGraphName) const
{
	POSITION pos = m_GraphList.GetHeadPosition();

	while(pos != NULL){
		CGraphImp* pGraph = m_GraphList.GetNext(pos);
		assert(pGraph != NULL);
		if(pGraph->GetGraphData().GetGraphName() == strGraphName)
			return pGraph;
	}
	return NULL;
}

// RQ를 가지고 있는 Graph 검색 : 복수종목 - ojtaso (20061224)
CGraphImp* CVertScaleGroup::FindGraph(LPCTSTR lpszRQ, const CString& strGraphName) const
{
	POSITION pos = m_GraphList.GetHeadPosition();

	while(pos != NULL){
		CGraphImp* pGraph = m_GraphList.GetNext(pos);
		assert(pGraph != NULL);
		if(!::lstrcmp(lpszRQ, pGraph->GetGraphRQ()) && 
			pGraph->GetGraphData().GetGraphName() == strGraphName)
			return pGraph;
	}
	return NULL;
}
	 
CGraphImp* CVertScaleGroup::GetGraph(const int nIndex) const
{
	POSITION pos = m_GraphList.FindIndex(nIndex);
	if(pos == NULL)
		return NULL;

	return m_GraphList.GetAt(pos);
}

CGraphImp* CVertScaleGroup::FindGraph(const CString& strTitleName, int& nSubGraphIndex) const
{
	POSITION pos = m_GraphList.GetHeadPosition();
	while(pos != NULL){
		CGraphImp* pGraph = m_GraphList.GetNext(pos);
		assert(pGraph != NULL);
		int nIndex = pGraph->GetSubGraphList().GetSameTitleSubgraphIndex(strTitleName);
		if(nIndex < 0)
			continue;

		nSubGraphIndex = nIndex;
		return pGraph;
	}

	return NULL;
}

CGraphImp* CVertScaleGroup::FindIndicator( const CString &strIndicatorName) const
{
	POSITION pos = m_GraphList.GetHeadPosition();

	while(pos != NULL){
		CGraphImp* pGraph = m_GraphList.GetNext(pos);
		assert( pGraph != NULL);
		if(pGraph->GetGraphData().GetIndicatorName() == strIndicatorName)
			return pGraph;
	}
	return NULL;
}

// RQ를 가지고 있는 Graph 검색 : 복수종목 - ojtaso (20061224)
CGraphImp* CVertScaleGroup::FindIndicator(LPCTSTR lpszRQ, const CString &strIndicatorName) const
{
	POSITION pos = m_GraphList.GetHeadPosition();

	while(pos != NULL){
		CGraphImp* pGraph = m_GraphList.GetNext(pos);
		assert( pGraph != NULL);
		if(!::lstrcmp(lpszRQ, pGraph->GetGraphRQ()) && 
			pGraph->GetGraphData().GetIndicatorName() == strIndicatorName)
			return pGraph;
	}
	return NULL;
}
	 
// subGraph Title을 이용하여 subGraph찾기.
// 타이틀 여러줄로 표시 : 복수종목 - ojtaso (20070227)
CSubGraph* CVertScaleGroup::FindSubGraphTitle(CDC* pDC, const CRect& allTitleRegion, const CPoint& point, CRect& titleRegion, int& preTitleEndPosition, int& nPreLineCount) const
{
	POSITION pos = m_GraphList.GetHeadPosition();

	while(pos != NULL){
		CGraphImp* pGraph = m_GraphList.GetNext(pos);
		assert(pGraph != NULL);
		// 타이틀 여러줄로 표시 : 복수종목 - ojtaso (20070227)
		CSubGraph* pSubGraph = pGraph->GetSubGraphList().FindSubGraphTitle(pDC, allTitleRegion, point, titleRegion, preTitleEndPosition, nPreLineCount);
		if(pSubGraph != NULL)
 		{
			// 그래프/타이틀 선택 때 RQ알려줌 : 복수종목 - ojtaso (20070106)
			if(GetKeyState(VK_LBUTTON) < 0)
			{
				IChartCtrl* pChartCtrl = m_pMainBlockBaseData->GetMainAttributes().GetIChartCtrl();

				LPCTSTR lpszRQ = pGraph->GetGraphRQ();
				pChartCtrl->SelectedGraph(lpszRQ);
				
				// Y Scale상에 종가 표시 수정 - ojtaso (20070807)
				const_cast<CGraphImp*>(m_lpSelectedGraph) = pGraph;
			}
   			return pSubGraph;
   		}
	}

	return NULL;
}

// subGraph 찾기.
bool CVertScaleGroup::FindSubGraph(const CPoint& point, CSelectedSubGraphDeque& selectedSubGraphDeque) const
{
	// (2007/2/1 - Seung-Won, Bae) 
	if( !m_displayDataMinMax.IsInitialized()) return false;

	POSITION pos = m_GraphList.GetHeadPosition();
	while(pos != NULL){
		CGraphImp* pGraph = (CGraphImp*) m_GraphList.GetNext(pos);
		assert(pGraph != NULL);
		if(pGraph->GetSubGraphList().FindSubGraph(point, selectedSubGraphDeque))
		{
 			// 그래프/타이틀 선택 때 RQ알려줌 : 복수종목 - ojtaso (20070106)
 			if(GetKeyState(VK_LBUTTON) < 0)
 			{
 				IChartCtrl* pChartCtrl = m_pMainBlockBaseData->GetMainAttributes().GetIChartCtrl();
 				pChartCtrl->SelectedGraph(pGraph->GetGraphRQ());

				// Y Scale상에 종가 표시 수정 - ojtaso (20070807)
				const_cast<CGraphImp*>(m_lpSelectedGraph) = pGraph;
 			}
 			return true;
		}
	}
	return false;
}

// private ===============================================================================
// CGraphImp를 만든다.
CGraphImp* CVertScaleGroup::MakeNewGraph( char *p_szGraphData)
{
	if( !p_szGraphData) return NULL;
	if( !*p_szGraphData) return NULL;

	char *szGraphData = strstr( p_szGraphData, "Graph;\r\n");
	if( !szGraphData) return NULL;
	szGraphData += 8;	// "Graph;\r\n"

	char *szGraphDataEnd = strstr( szGraphData, "GraphEnd;\r\n");
	if( szGraphDataEnd) *szGraphDataEnd = '\0';

	if( !*szGraphData)
	{
		if( szGraphDataEnd) *szGraphDataEnd = 'G';
		return NULL;
	}


	char *szSubGraphData = strstr( szGraphData, "\r\n");
	if( szSubGraphData) *szSubGraphData = '\0';

	// 복수종목 저장 : 복수종목 - ojtaso (20070507)
	char *szRQ = strstr( szGraphData, "RQ:");  

	CGraphData graphData( szGraphData, m_pMainBlockBaseData->GetParent_Handle());
	if( szSubGraphData)
	{
		*szSubGraphData = '\r';
		szSubGraphData += 2;		// "\r\n"
	}

	// (2006/12/10 - Seung-Won, Bae) Change Graph Name for Multiple Supporting of Same Indicator.
	//		Caution. It must be processed before creation of Graph.
	//				Because the Creation of Graph makes the Indicator Info and bind it with the Graph.
	if( m_pMainBlockBaseData)
	{
		CMainBlockImp *pMainBlock = m_pMainBlockBaseData->GetMainAttributes().GetMainBlockImpl();
		CString strGraphName = graphData.GetGraphName();
		CIndicatorList *pIndicatorList = m_pMainBlockBaseData->GetMainAttributes().GetIndicatorList();
		if( pIndicatorList)
		{
			// (2007/1/12 - Seung-Won, Bae) Make Unique Graph Name
			if( strGraphName.IsEmpty())
			{
				CIndicatorInfo *pIndicatorInfo = NULL;
				do
				{
					strGraphName.Format( "#%04X", rand());
					pIndicatorInfo = pIndicatorList->GetGraphIndicatorInfo( EILT_USER, strGraphName, NULL, EIIGT_WITHOUT_DEFAULT);
				} while( pIndicatorInfo);
				( ( CGraphDataBase *)&graphData)->SetGraphName( strGraphName);
			}
			else if( IsExistenceGraph( strGraphName)						// Check Current VScale Group (It may be not added in Block, because in creation.)
						|| pMainBlock->IsExistenceGraph( strGraphName))
			{
				CIndicatorInfo *pIndicatorInfo = pIndicatorList->CreateGraphIndicatorInfo( graphData.GetIndicatorName());
				if( pIndicatorInfo) ( ( CGraphDataBase *)&graphData)->SetGraphName( pIndicatorInfo->GetGraphName());
			}
		}
	}

	// 복수종목 저장 : 복수종목 - ojtaso (20070507)
	CGraphImp *pGraph = NULL;
	if(szRQ)
	{
		szRQ += 3;
		char *szRQEnd = strstr( szRQ, "\r\n");
		if(szRQEnd)
			*szRQEnd = '\0';

		LPVOID pPacketRQ = m_pMainBlockBaseData->GetMainAttributes().GetPacketListManager()->GetPacketRQ(szRQ);
		if(pPacketRQ)
		{
			pGraph = CGraphImp::MakeGraph(pPacketRQ, m_pMainBlockBaseData, this, graphData, szSubGraphData);
			m_pMainBlockBaseData->GetMainAttributes().GetIChartCtrl()->NotifyRQStatusToAddin( CCommonInterfaceEnum::PACKETLISTMANAGER_NEW_RQ, szRQ);
		}
		else
			pGraph = CGraphImp::MakeGraph(m_pMainBlockBaseData, this, graphData, szSubGraphData);
	}
	else
	{
		// Packet Name 다시 세팅 : 복수종목 - ojtaso (20070531)
		CPacketListManager* lpPacketListManager = m_pMainBlockBaseData->GetMainAttributes().GetPacketListManager();

		LPVOID pPacketRQ = lpPacketListManager->GetPacketRQ( m_pMainBlockBaseData->GetMainAttributes().GetIChartCtrl()->GetCurrentRQ());
		if( !pPacketRQ)
		{
			if( szGraphDataEnd) *szGraphDataEnd = 'G';
			return NULL;
		}
		pGraph = CGraphImp::MakeGraph(pPacketRQ, m_pMainBlockBaseData, this, graphData, szSubGraphData);
	}

	if( szGraphDataEnd) *szGraphDataEnd = 'G';

	return pGraph;
}

// RQ를 가지고 있는 Graph 생성 : 복수종목 - ojtaso (20061224)
// CPacketRQ*로 변경 : 복수종목 - ojtaso (20070109)
CGraphImp* CVertScaleGroup::MakeNewGraph(LPVOID pPacketRQ, char *p_szGraphData)
{
	if( !p_szGraphData) return NULL;
	if( !*p_szGraphData) return NULL;

	char *szGraphData = strstr( p_szGraphData, "Graph;\r\n");
	if( !szGraphData) return NULL;
	szGraphData += 8;	// "Graph;\r\n"

	char *szGraphDataEnd = strstr( szGraphData, "GraphEnd;\r\n");
	if( szGraphDataEnd) *szGraphDataEnd = '\0';

	if( !*szGraphData)
	{
		if( szGraphDataEnd) *szGraphDataEnd = 'G';
		return NULL;
	}

	char *szSubGraphData = strstr( szGraphData, "\r\n");
	if( szSubGraphData) *szSubGraphData = '\0';

	// 복수종목 저장 : 복수종목 - ojtaso (20070507)
	char *szRQ = strstr( szGraphData, "RQ:");  

	CGraphData graphData( szGraphData, m_pMainBlockBaseData->GetParent_Handle());
	if( szSubGraphData)
	{
		*szSubGraphData = '\r';
		szSubGraphData += 2;		// "\r\n"
	}


	// (2006/12/10 - Seung-Won, Bae) Change Graph Name for Multiple Supporting of Same Indicator.
	//		Caution. It must be processed before creation of Graph.
	//				Because the Creation of Graph makes the Indicator Info and bind it with the Graph.
	if( m_pMainBlockBaseData)
	{
		CMainBlockImp *pMainBlock = m_pMainBlockBaseData->GetMainAttributes().GetMainBlockImpl();
		CString strGrpahName = graphData.GetGraphName();
		if( IsExistenceGraph(pPacketRQ, strGrpahName)					// Check Current VScale Group (It may be not added in Block, because in creation.)
			|| pMainBlock->IsExistenceGraph( strGrpahName))
		{
			CIndicatorList *pIndicatorList = m_pMainBlockBaseData->GetMainAttributes().GetIndicatorList();
			if( pIndicatorList)
			{
				CIndicatorInfo *pIndicatorInfo = pIndicatorList->CreateGraphIndicatorInfo( graphData.GetIndicatorName());
				if( pIndicatorInfo) ( ( CGraphDataBase *)&graphData)->SetGraphName( pIndicatorInfo->GetGraphName());
			}
		}
	}

	// 복수종목 불러오기 : 복수종목 - ojtaso (20070507)
	CGraphImp *pGraph = NULL;
	if(szRQ)
	{
		szRQ += 3;
		char *szRQEnd = strstr( szRQ, "\r\n");
		if(szRQEnd)
		{
			*szRQEnd = '\0';
			
			pGraph = CGraphImp::MakeGraph(m_pMainBlockBaseData, this, graphData, szSubGraphData);
			pGraph->SetGraphRQ(szRQ);

			*szRQEnd = '\r';
		}
	}
	else
		pGraph = CGraphImp::MakeGraph(pPacketRQ, m_pMainBlockBaseData, this, graphData, szSubGraphData);

//	CGraphImp *pGraph = CGraphImp::MakeGraph(pPacketRQ, m_pMainBlockBaseData, this, graphData, szSubGraphData);
	if( szGraphDataEnd) *szGraphDataEnd = 'G';

	return pGraph;
}

// 마지막 존재하는 graph를 삭제할 경우 자신을 삭제.
bool CVertScaleGroup::DeleteEmptyGroup()
{
	if( !m_GraphList.IsEmpty() || m_pVertScaleGroupList == NULL || m_bOnDestructor) return false;
	return m_pVertScaleGroupList->DeleteVertScaleGroup( this, true);
}

// graph를 비교하여 packet도 삭제한다.
void CVertScaleGroup::DeleteGraph(CGraphImp* pDeleteGraph, CGraphImp* pSourceGraph)
{
	bool bSamePacket = false;
	if(pSourceGraph != NULL)
		bSamePacket = pSourceGraph->GetSubGraphList().IsExistenceSamePacketInGraph(pDeleteGraph);

	if(bSamePacket)
		DeleteGraphNoDeletePacket(pDeleteGraph);
	else
		DeleteGraphWithNotify(pDeleteGraph);
}

// 현재 선택된 객체를 삭제한다.
void CVertScaleGroup::DeleteGraphNoDeletePacket(CGraphImp* pGraph)
{
	assert(pGraph != NULL);
	POSITION pos = m_GraphList.Find(pGraph);

	// Y Scale상에 종가 표시 수정 - ojtaso (20070807)
	if(m_lpSelectedGraph == pGraph)
		m_lpSelectedGraph = NULL;

	// (2007/2/16 - Seung-Won, Bae) Remove in list after Deleting the Object.
	//		In Destructor, can be access the List.
	CGraphImp::Delete(pGraph);
	if(pos != NULL)
		m_GraphList.RemoveAt(pos);
}

// 두 graph를 바꾼다.
void CVertScaleGroup::ChangeGraph(CGraphImp* pSourceGraph, CGraphImp* pTargetGraph)
{
	assert(pSourceGraph != NULL && pTargetGraph != NULL);

	POSITION pos = m_GraphList.Find(pTargetGraph);
	m_GraphList.InsertBefore(pos, pSourceGraph);
	//sy test
	// 두개의 graph의 packet이 같은 경우.
	//DeleteGraph(pTargetGraph);
	DeleteGraph(pTargetGraph, pSourceGraph);
}


// ---------------------------------------------------------------------------------------
// 이미 생성되어 있는 graph에 data를 setting.
void CVertScaleGroup::ModifyGraph( CGraphImp* pGraph, char *p_szGraphData)
{
	CGraphImp* pNewGraph = MakeNewGraph( p_szGraphData);
	// 바꿀 내용이 없을 경우엔 graph를 list에서 삭제한다.
	if( !pNewGraph)
	{
		DeleteGraphWithNotify( pGraph);
		return;
	}

	// 현재 선택된 pGraph를 새로운 graph로 바꾼다.
	// -> pGraph는 삭제 된다.
	ChangeGraph(pNewGraph, pGraph);
}

// ----------------------------------------------------------------------------------------
// 같은 VertScaleGroup 안의 쉼김이 아닌 모든 Graph의 min/max 구하기
CDisplayDataMinMax CVertScaleGroup::GetDisplayDataMinMaxInAll() const
{
	//sy test
	CDisplayDataMinMax displayDataMinMax, gapData;
	POSITION pos = m_GraphList.GetHeadPosition();
	while(pos != NULL)
	{
		CGraphImp* pGraph = m_GraphList.GetNext(pos);
		assert(pGraph != NULL);

		// (2007/4/23 - Seung-Won, Bae) Check Hide Graph.
		if( pGraph->IsGraphHide()) continue;

		GetDisplayDataMinMax(pGraph, displayDataMinMax);
	}

	return displayDataMinMax;
}

// <가격차트> 만의 min/max 구하기
bool CVertScaleGroup::GetDisplayDataMinMaxInOnlyPrice(CDisplayDataMinMax& displayDataMinMax) const
{
	CGraphImp* pGraph = FindGraph( _MTEXT( C2_PRICE_CHART));
	if(pGraph == NULL) return false;

	bool bIsOnlyPriceVertScale = DoesOnlyPriceVertScale();
	if(!bIsOnlyPriceVertScale) return false;

	GetDisplayDataMinMax(pGraph, displayDataMinMax);
	return true;
}

// 해당 graph의 min, max
void CVertScaleGroup::GetDisplayDataMinMax(CGraphImp* pGraph, CDisplayDataMinMax& displayDataMinMax) const
{
	CDisplayDataMinMax temp;
	// (2006/11/11 - Seung-Won, Bae) Use Y Scale Min/Max Setting
	if(!pGraph->GetDisplayDataMinMax(m_vertScale.GetScaleData(), temp, m_vertScale.GetVertScaleMinMaxType() == CScaleBaseData::VERT_MINMAX_OF_FULL))
		return;

	// (2006/11/19 - Seung-Won, Bae) Support Initizling State.
	if( displayDataMinMax.IsInitialized())
	{
		double dMin = displayDataMinMax.GetDisplayMin();
		double dMax = displayDataMinMax.GetDisplayMax();
		if(temp.GetDisplayMin() < dMin) dMin = temp.GetDisplayMin();
		if(temp.GetDisplayMax() > dMax) dMax = temp.GetDisplayMax();
		displayDataMinMax.SetDisplayData( dMin, dMax);
	}
	else displayDataMinMax = temp;
}

// ----------------------------------------------------------------------------------------
// draw 하기 위한 값 추출
void CVertScaleGroup::SetDrawingVertScaleData(CDrawingVertScaleData* pDrawingVertScaleData)
{
	if(pDrawingVertScaleData == NULL || m_GraphList.GetCount() <= 0)
		return;

	// (2007/1/14 - Seung-Won, Bae) Get the Base Packet for Default Scale Min/Max
	if( !m_pBaseGraph && !m_pBasePacket) return;

	pDrawingVertScaleData->SetDisplayDataMinMax(	m_displayDataMinMax);
	pDrawingVertScaleData->SetIndicatorName(		m_pBaseGraph->GetGraphData().GetIndicatorName());
	pDrawingVertScaleData->SetSubGraphNames(		m_pBaseGraph->GetSubGraphList().GetViewableSubGraphNames());
	pDrawingVertScaleData->SetPacketNames(			m_pBaseGraph->GetGraphData().GetPacketNames());
	pDrawingVertScaleData->SetIndicatorInfo(		m_pBaseGraph->GetIndicatorInfo());
	pDrawingVertScaleData->SetPacket(				m_pBasePacket);

	if( pDrawingVertScaleData->GetScaleDrawerType() == CScaleBaseData::VERT_NOTYPE){
		// <바형>의 <고정비율빗형식> 인 경우 -> ScaleDrawerType : <+상하식>
		if( m_pBaseGraph->GetSubGraphList().IsViewableGraphStyle(CGraphBaseData::Bar_Type, CGraphBaseData::FixedRateCombBar, 0))
			pDrawingVertScaleData->SetScaleDrawerType(CScaleBaseData::VERT_TOPBUTTOM_PLUS);
	}
}

void CVertScaleGroup::InitializeDrawingVertScaleData(CDrawingVertScaleData* pDrawingVertScaleData)
{
	// VERT_LOG 가 아닌 경우만 초기화한다.
	if(pDrawingVertScaleData->GetScaleDrawerType() == CScaleBaseData::VERT_LOG)
		return;

	pDrawingVertScaleData->SetScaleDrawerType(CScaleBaseData::VERT_NOTYPE);
}

// ----------------------------------------------------------------------------------------
// scale을 그릴지의 여부
bool CVertScaleGroup::IsVertScaleDrawing() const
{
	if( m_vertScale.GetScaleData().IsEmpty() || m_GraphList.GetCount() <= 0)
		return false;

	return (m_vertScale.GetVertScalePosition() != CScaleBaseData::VERT_NONE && m_vertScale.GetActiveFlag());
}

// <가격차트> 만의 scale 인지 여부
bool CVertScaleGroup::DoesOnlyPriceVertScale() const
{
	if(m_pMainBlockBaseData == NULL)
		return false;

	return m_pMainBlockBaseData->GetScaleAttributes().DoesOnlyPriceVertScale();
}

// ----------------------------------------------------------------------------
// vertScale을 그리기 위해 필요한 data(CDrawingVertScaleData)를 
// 구하기 위한 기준이 되는 graph 찾기.
CGraphImp* CVertScaleGroup::FindBaseGraphForDrawingVertScaleData() const
{
	if( m_GraphList.IsEmpty()) return NULL;

	// 기본적으로 head에 있는 graph.
	// (2007/3/11 - Seung-Won, Bae) Retrieve the Visible Graph first.
	CGraphImp* pBaseGraph = NULL;
	CGraphSearchInfo_VisibleSubGraph gsi;
	if( FindGraph( &gsi)) pBaseGraph = gsi.m_pGraphImp;
	else pBaseGraph = m_GraphList.GetHead();
	if( pBaseGraph == NULL) return NULL;
	
	CString strGraphName = pBaseGraph->GetGraphData().GetIndicatorName();
	// "가격차트" group. -> "가격차트"
	// (단, "가격차트"와 동일한 vertScaleGroup 이어야 한다.!)
	if(strGraphName != _MTEXT( C2_PRICE_CHART) && 
		g_iMetaTable.IsScaleGroup_Price(strGraphName))
	{
		CGraphImp* pGraph = FindGraph( _MTEXT( C2_PRICE_CHART));
		if(pGraph != NULL)
			return pGraph;
	}
	// "거래량차트" group. -> "거래량 차트"
	else if(strGraphName != _MTEXT( C2_VOLUME_CHART) && 
		g_iMetaTable.IsScaleGroup_Volume(strGraphName))
	{
		CGraphImp* pGraph = FindGraph( _MTEXT( C2_VOLUME_CHART));
		if(pGraph != NULL)
			return pGraph;
	}

	return pBaseGraph;
}

//////////////////////////////////////////////////////////////////////
// (2006/6/25 - Seung-Won, Bae) Get Data Info of Graph
//////////////////////////////////////////////////////////////////////

// (2006/6/25 - Seung-Won, Bae) Search Index of Graph
BOOL CVertScaleGroup::GetIndexOfGraph( const char *p_szGraphName, int &p_nG)
{
	p_nG = -1;
	if( !p_szGraphName) return FALSE;
	if( !*p_szGraphName) return FALSE;

	p_nG = 0;
	POSITION pos = m_GraphList.GetHeadPosition();
	while(pos != NULL)
	{
		CGraphImp* pGraph = m_GraphList.GetNext(pos);
		assert(pGraph != NULL);
		if( pGraph->GetGraphData().GetGraphName() == p_szGraphName) return TRUE;
		p_nG++;
	}

	p_nG = -1;
	return FALSE;
}

// Change Input Packet Names and Calculate again.
BOOL CVertScaleGroup::ChangeIndicatorPacketNames( const char *p_szIndicatorName, const int p_nSubGraphIndex, const char *p_szPacketNames)
{
	if( !p_szIndicatorName || !p_szPacketNames) return FALSE;
	if( !*p_szIndicatorName || !*p_szPacketNames) return FALSE;

	BOOL bResult = FALSE;
	CGraphImp *pGraph = NULL;
	CIndicatorInfo *pIndicatorInfo = NULL;
	POSITION pos = m_GraphList.GetHeadPosition();
	while( pos)
	{
		pGraph = m_GraphList.GetNext( pos);
		if( !pGraph) continue;
		pIndicatorInfo = pGraph->GetIndicatorInfo();
		if( !pIndicatorInfo) continue;
		if( pIndicatorInfo->GetIndicatorName() != p_szIndicatorName) continue;

		if( p_nSubGraphIndex < 0)
		{
			pIndicatorInfo->SetCommonPacketNames( p_szPacketNames);
			pIndicatorInfo->SetAllPacketNames_SubGraphPart( p_szPacketNames, false);
		}
		else if( !pIndicatorInfo->SetPacketNames( p_nSubGraphIndex, p_szPacketNames)) continue;

		pGraph->ChangeGraphDataFromIndicatorInfo( true);
		bResult = TRUE;
	}
	return bResult;
}

// Y Scale상에 종가 표시 수정 - ojtaso (20070807)
BOOL CVertScaleGroup::IsScaleTextDrawing()
{
	return IsVertScaleDrawing();
}

// Y Scale상에 종가 표시 수정 - ojtaso (20070807)
void CVertScaleGroup::SetSelectedGraph(CGraphImp*& lpSelectedGraph)
{
	m_lpSelectedGraph = lpSelectedGraph;
}

// 지표 뒤집기 - ojtaso (20071023)
bool CVertScaleGroup::GetVertScaleInvertType() const
{
	return m_vertScale.GetVertScaleInvertType();
}

// 지표 뒤집기 - ojtaso (20071023)
void CVertScaleGroup::SetVertScaleInvertType(const bool bIsVertScaleInvertType)
{
	m_vertScale.SetVertScaleInvertType(bIsVertScaleInvertType);
}

#endif