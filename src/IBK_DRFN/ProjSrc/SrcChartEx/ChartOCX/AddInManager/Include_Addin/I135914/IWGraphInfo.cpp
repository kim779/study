// IWGraphInfo.cpp: implementation of the CIWGraphInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IWGraphInfo.h"

#include "../../../Include_Chart/Graph.h"		// for CGraph
#include "../../../Include_Chart/GraphData.h"

#include "GraphData.h"
#include "IWDoubleList.h"		// for CIWDoubleList
#include "IWString.h"			// for CIWString

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// _IWRAPPER_IUNKNOWN_MANAGER_IMP
//////////////////////////////////////////////////////////////////////
_IWRAPPER_IUNKNOWN_MANAGER_IMP( CIWGraphInfo)

//////////////////////////////////////////////////////////////////////
// // (2006/7/13 - Seung-Won, Bae) Operation.
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// (2006/8/11 - Seung-Won, Bae) Sub Graph Data Pointer Cache.
//////////////////////////////////////////////////////////////////////
CSubGraphData *CIWGraphInfo::GetSubGraphData( const int p_nSubGraphIndex)
{
//	if( m_nSubGraphIndex == p_nSubGraphIndex) return m_pSubGraphData;

	m_nSubGraphIndex = p_nSubGraphIndex;
	m_pSubGraphData = m_pIndicatorInfo->GetSubGraphData( m_nSubGraphIndex);
	return m_pSubGraphData;
}

/*-----------------------------------------------------------------------------------------
 - Function    :  GetCommonCalculateConditionList
 - Created at  :  2005-07-15   13:31
 - Author      :  최종찬
 - Return Value:  CList<double, double>* - 계산조건값 리스트
 - Description :  지표계산에 필요한 조건값을 리스트로 제공한다.
 -----------------------------------------------------------------------------------------*/
IDoubleList *CIWGraphInfo::GetCommonCalculateConditionList( void)
{
	if( !m_pIndicatorInfo) return NULL;
	CList<double, double> *plDoubleList = m_pIndicatorInfo->GetCommonCalConditionList();
	if( !plDoubleList) return NULL;
	return CIWDoubleList::CreateObject( plDoubleList);
}

IDoubleList *CIWGraphInfo::GetDrawConditionList( void)
{
	if( !m_pIndicatorInfo) return NULL;
	CList<double, double> *plDoubleList = m_pIndicatorInfo->GetDrawConditionList();
	if( !plDoubleList) return NULL;
	return CIWDoubleList::CreateObject( plDoubleList);
}

/*-----------------------------------------------------------------------------------------
 - Function    :  GetCalculateConditionList
 - Created at  :  2005-07-15   13:32
 - Author      :  최종찬
 - Parameters  :  nSubGraphIndex - 서브그래프 인덱스
 - Return Value:  CList<double, double>* - 계산조건값 리스트
 - Description :  지표계산에 필요한 조건값을 서브그래프별로 제공한다.
 -----------------------------------------------------------------------------------------*/
IDoubleList *CIWGraphInfo::GetCalculateConditionList(const int nSubGraphIndex)
{
	if( !m_pIndicatorInfo) return NULL;
	CList<double, double> *plDoubleList = m_pIndicatorInfo->GetCalConditionList( nSubGraphIndex);
	if( !plDoubleList) return NULL;
	return CIWDoubleList::CreateObject( plDoubleList);
}


/*-----------------------------------------------------------------------------------------
 - Function    :  GetCalculateConditionList
 - Created at  :  2005-07-15   13:33
 - Author      :  최종찬
 - Parameters  :  pcszSubGraphName - 서브그래프 네임
 - Return Value:  CList<double, double>* - 계산조건값 리스트
 - Description :  지표계산에 필요한 조건값을 서브그래프별로 제공한다.
 -----------------------------------------------------------------------------------------*/
IDoubleList *CIWGraphInfo::GetCalculateConditionList(const char* pcszSubGraphName)
{
	if( !m_pIndicatorInfo) return NULL;
	CList<double, double> *plDoubleList = m_pIndicatorInfo->GetCalConditionList( pcszSubGraphName);
	if( !plDoubleList) return NULL;
	return CIWDoubleList::CreateObject( plDoubleList);
}


/*-----------------------------------------------------------------------------------------
 - Function    :  GetCommonCalculatePacketNames
 - Created at  :  2005-07-15   13:49
 - Author      :  최종찬
 - Return Value:  LPCTSTR - 계산할 패킷명
 - Description :  지표 계산에 필요한 패킷명을 제공한다.
 -----------------------------------------------------------------------------------------*/
IString *CIWGraphInfo::GetPacketNameString( void)
{
	if( !m_pIndicatorInfo) return NULL;

	return CIWString::CreateObject( m_pIndicatorInfo->GetCommonPacketNames());
}
BOOL CIWGraphInfo::SetPacketName( const char *p_szPacketName)
{
	if( !m_pIndicatorInfo) return FALSE;
	m_pIndicatorInfo->SetCommonPacketNames( p_szPacketName);
	return TRUE;
}


// (2006/8/14 - Seung-Won, Bae) Support BaseLine Data List
IDoubleList *CIWGraphInfo::GetBaseLineList( void)
{
	if( !m_pIndicatorInfo) return NULL;
	return CIWDoubleList::CreateObject( m_pIndicatorInfo->GetBaseLineList());
}

//////////////////////////////////////////////////////////////////////
// (2006/7/7 - Seung-Won, Bae) SubGraph Info
//////////////////////////////////////////////////////////////////////

// (2006/7/7 - Seung-Won, Bae) Get Sub Graph Count.
int CIWGraphInfo::GetSubGraphCount( void)
{
	if( !m_pIndicatorInfo) return 0;
	return m_pIndicatorInfo->GetSubGraphCount();
}

/*-----------------------------------------------------------------------------------------
 - Function    :  GetSubGraphIndex
 - Created at  :  2005-07-15   13:51
 - Author      :  최종찬
 - Parameters  :  pcszSubGraphName - 서브그래프명.
 - Return Value:  int - 서브그래프 인덱스
 - Description :  서브그래프의 인덱스를 리턴한다.
 -----------------------------------------------------------------------------------------*/
int CIWGraphInfo::GetSubGraphIndex(const char* pcszSubGraphName)
{
	if(!m_pIndicatorInfo)
		return -1;

	return m_pIndicatorInfo->GetSubGraphIndex(pcszSubGraphName);
}


/*-----------------------------------------------------------------------------------------
 - Function    :  IsShowSubGraph
 - Created at  :  2005-07-29   10:14
 - Author      :  최종찬
 - Parameters  :  nSubGraphIndex - [in] SubGraph Index
 -                bSubGraphShow  - [out] Show/Hide여부를 리턴한다.
 - Return Value:  bool - true: 성공적 호출, false: 실패(SubGraph가 없을경우가 가장 큰 이유겠지)
 - Description :  서브그래프의 Show/Hide여부를 제공한다.
 -----------------------------------------------------------------------------------------*/
BOOL CIWGraphInfo::GetSubGraphShowFlag( const int p_nSubGraphIndex, BOOL &p_bShow)
{
	// (2007/3/9 - Seung-Won, Bae) Do not Support the Graph Hide.
	p_bShow = FALSE;
	if( m_pIndicatorInfo) p_bShow = !m_pIndicatorInfo->GetSubGraphHiding( p_nSubGraphIndex);
	return TRUE;
}
BOOL CIWGraphInfo::SetSubGraphShowFlag( const int p_nSubGraphIndex, BOOL p_bShow)
{
	CSubGraphData* pSubGraphData = GetSubGraphData( p_nSubGraphIndex);
	if(pSubGraphData == NULL) return FALSE;

	pSubGraphData->SetHiding( !p_bShow);
	return TRUE;
}

// (2006/8/11 - Seung-Won, Bae) Graph Name
IString *CIWGraphInfo::GetSubGraphNameString( const int p_nSubGraphIndex)
{
	CSubGraphData* pSubGraphData = GetSubGraphData( p_nSubGraphIndex);
	if( pSubGraphData == NULL) return NULL;

	return CIWString::CreateObject( pSubGraphData->GetName());
}

/*-----------------------------------------------------------------------------------------
 - Function    :  GetCalculatePacketNames
 - Created at  :  2005-07-15   13:51
 - Author      :  최종찬
 - Parameters  :  nSubGraphIndex - 서브그래프 인덱스
 - Return Value:  LPCTSTR - 계산에 필요한 패킷명
 - Description :  지표 계산에 필요한 패킷명을 제공한다. (서브그래프별로 제공)
 -----------------------------------------------------------------------------------------*/
IString *CIWGraphInfo::GetSubGraphPacketNameString( const int p_nSubGraphIndex)
{
	if( !m_pIndicatorInfo) return NULL;

	return CIWString::CreateObject( m_pIndicatorInfo->GetPacketNames( p_nSubGraphIndex));
}
BOOL CIWGraphInfo::SetSubGraphPacketName( const int p_nSubGraphIndex, const char *p_szPacketName)
{
	if( !m_pIndicatorInfo) return FALSE;
	return m_pIndicatorInfo-> SetPacketNames( p_nSubGraphIndex, p_szPacketName);
}

// (2006/8/11 - Seung-Won, Bae) Graph Title
IString *CIWGraphInfo::GetSubGraphTitleString( const int p_nSubGraphIndex)
{
	CSubGraphData* pSubGraphData = GetSubGraphData( p_nSubGraphIndex);
	if( pSubGraphData == NULL) return NULL;

	return CIWString::CreateObject( pSubGraphData->GetTitle());
}
BOOL CIWGraphInfo::SetSubGraphTitle( const int p_nSubGraphIndex, const char *p_szTitle)
{
	CSubGraphData* pSubGraphData = GetSubGraphData( p_nSubGraphIndex);
	if( pSubGraphData == NULL) return FALSE;

	pSubGraphData->SetTitle( p_szTitle);
	return TRUE;
}

// (2006/8/10 - Seung-Won, Bae) Graph Type
int CIWGraphInfo::GetSubGraphType( const int p_nSubGraphIndex)
{
	CSubGraphData* pSubGraphData = GetSubGraphData( p_nSubGraphIndex);
	if( pSubGraphData == NULL) return -1;

	return pSubGraphData->GetType();
}

// (2006/8/10 - Seung-Won, Bae) Graph Style
int CIWGraphInfo::GetSubGraphStyle( const int p_nSubGraphIndex)
{
	CSubGraphData* pSubGraphData = GetSubGraphData( p_nSubGraphIndex);
	if(pSubGraphData == NULL) return -1;

	return pSubGraphData->GetStyle().GetGraphStyle();
}
BOOL CIWGraphInfo::SetSubGraphStyle( const int p_nSubGraphIndex, const int p_nGraphStyle)
{
	CSubGraphData* pSubGraphData = GetSubGraphData( p_nSubGraphIndex);
	if(pSubGraphData == NULL) return FALSE;

	pSubGraphData->SetStyle( p_nGraphStyle);
	return TRUE;
}

// (2006/8/10 - Seung-Won, Bae) Draw Style
int CIWGraphInfo::GetSubGraphDrawStyle( const int p_nSubGraphIndex)
{
	CSubGraphData* pSubGraphData = GetSubGraphData( p_nSubGraphIndex);
	if(pSubGraphData == NULL) return -1;

	return pSubGraphData->GetDrawStyle().GetDrawStyle();
}
BOOL CIWGraphInfo::SetSubGraphDrawStyle( const int p_nSubGraphIndex, const int p_nGraphDrawStyle)
{
	CSubGraphData* pSubGraphData = GetSubGraphData( p_nSubGraphIndex);
	if(pSubGraphData == NULL) return FALSE;

	pSubGraphData->SetDrawStyle( p_nGraphDrawStyle);
	return TRUE;
}

// 기준선 두께, 모양 설정 - ojtaso (20071213)
int CIWGraphInfo::GetSubGraphDrawBaseLineStyle( const int p_nSubGraphIndex)
{
	CSubGraphData* pSubGraphData = GetSubGraphData( p_nSubGraphIndex);
	if(pSubGraphData == NULL) return -1;

	return pSubGraphData->GetBaseLineDrawStyle().GetBaseLineStyle();
}

// 기준선 두께, 모양 설정 - ojtaso (20071213)
BOOL CIWGraphInfo::SetSubGraphDrawBaseLineStyle( const int p_nSubGraphIndex, const int p_nGraphDrawStyle)
{
	CSubGraphData* pSubGraphData = GetSubGraphData( p_nSubGraphIndex);
	if(pSubGraphData == NULL) return FALSE;

	pSubGraphData->SetBaseLineStyle( p_nGraphDrawStyle);
	return TRUE;
}

// Singbar Graph 색상 비교기준 추가 - ojtaso (20071121)
void CIWGraphInfo::SetSubGraphPaintStyle(const int p_nSubGraphIndex, const int p_nPaintStyle)
{
	CSubGraphData* pSubGraphData = GetSubGraphData( p_nSubGraphIndex);
	if(pSubGraphData == NULL) return;

	pSubGraphData->SetPaintStyle(p_nPaintStyle);
}

// (2006/8/10 - Seung-Won, Bae) Color
COLORREF CIWGraphInfo::GetSubGraphColor( const int p_nSubGraphIndex, const int p_nColorIndex)
{
	CSubGraphData* pSubGraphData = GetSubGraphData( p_nSubGraphIndex);
	if(pSubGraphData == NULL) return RGB( 0, 0, 0);

	CGraphColor &clrGraph = pSubGraphData->GetColor();
	switch( p_nColorIndex)
	{
		case 1: return clrGraph.GetColor1();
		case 2: return clrGraph.GetColor2();
		case 3: return clrGraph.GetColor3();
		case 4: return clrGraph.GetColor4();
		case 5: return clrGraph.GetColor5();
		case 6: return clrGraph.GetColor6();
	}
	return RGB( 0, 0, 0);
}

// Singbar Graph 색상 비교기준 추가 - ojtaso (20071121)
int CIWGraphInfo::GetSubGraphPaintStyle(const int p_nSubGraphIndex)
{
	CSubGraphData* pSubGraphData = GetSubGraphData( p_nSubGraphIndex);
	if(pSubGraphData == NULL) return -1;

	return pSubGraphData->GetPaintStyle().GetPaintStyle();
}

BOOL CIWGraphInfo::SetSubGraphColor( const int p_nSubGraphIndex, const int p_nColorIndex, const COLORREF p_clrColor)
{
	CSubGraphData* pSubGraphData = GetSubGraphData( p_nSubGraphIndex);
	if(pSubGraphData == NULL) return FALSE;

	CGraphColor &clrGraph = pSubGraphData->GetColor();
	switch( p_nColorIndex)
	{
		case 1: clrGraph.SetColor1( p_clrColor);
				break;
		case 2: clrGraph.SetColor2( p_clrColor);
				break;
		case 3: clrGraph.SetColor3( p_clrColor);
				break;
		case 4: clrGraph.SetColor4( p_clrColor);
				break;
		case 5: clrGraph.SetColor5( p_clrColor);
				break;
		case 6: clrGraph.SetColor6( p_clrColor);
				break;
		default: return FALSE;
	}
	return TRUE;
}
BOOL CIWGraphInfo::SetSubGraphColor( const int p_nSubGraphIndex, const COLORREF p_clrColor1, const COLORREF p_clrColor2, const COLORREF p_clrColor3, const COLORREF p_clrColor4, const COLORREF p_clrColor5, const COLORREF p_clrColor6)
{
	CSubGraphData* pSubGraphData = GetSubGraphData( p_nSubGraphIndex);
	if(pSubGraphData == NULL) return FALSE;

	pSubGraphData->GetColor().SetColor( p_clrColor1, p_clrColor2, p_clrColor3, p_clrColor4, p_clrColor5, p_clrColor6);
	return TRUE;
}
BOOL CIWGraphInfo::GetSubGraphColorAndFillFlag( const int p_nSubGraphIndex, COLORREF &p_clrColor1, COLORREF &p_clrColor2, COLORREF &p_clrColor3, COLORREF &p_clrColor4, COLORREF &p_clrColor5, COLORREF &p_clrColor6, BOOL &p_bFill1, BOOL &p_bFill2)
{
	CSubGraphData* pSubGraphData = GetSubGraphData( p_nSubGraphIndex);
	if(pSubGraphData == NULL) return FALSE;

	CGraphColor &clrGraph = pSubGraphData->GetColor();
	p_clrColor1 = clrGraph.GetColor1();
	p_clrColor2 = clrGraph.GetColor2();
	p_clrColor3 = clrGraph.GetColor3();
	p_clrColor4 = clrGraph.GetColor4();
	p_clrColor5 = clrGraph.GetColor5();
	p_clrColor6 = clrGraph.GetColor6();
	p_bFill1 = clrGraph.GetUpGraphFill();
	p_bFill2 = clrGraph.GetDownGraphFill();
	return TRUE;
}
BOOL CIWGraphInfo::SetSubGraphColorAndFillFlag( const int p_nSubGraphIndex, const COLORREF p_clrColor1, const COLORREF p_clrColor2, const COLORREF p_clrColor3, const COLORREF p_clrColor4, const COLORREF p_clrColor5, const COLORREF p_clrColor6, const BOOL p_bFill1, const BOOL p_bFill2)
{
	CSubGraphData* pSubGraphData = GetSubGraphData( p_nSubGraphIndex);
	if(pSubGraphData == NULL) return FALSE;

	CGraphColor &clrGraph = pSubGraphData->GetColor();
	clrGraph.SetColor( p_clrColor1, p_clrColor2, p_clrColor3, p_clrColor4, p_clrColor5, p_clrColor6);
	clrGraph.SetUpGraphFill(	p_bFill1 ? true : false);
	clrGraph.SetDownGraphFill(	p_bFill2 ? true : false);
	return TRUE;
}
BOOL CIWGraphInfo::GetSubGraphFillFlag( const int p_nSubGraphIndex, const int p_nFillIndex)
{
	CSubGraphData* pSubGraphData = GetSubGraphData( p_nSubGraphIndex);
	if(pSubGraphData == NULL) return FALSE;

	CGraphColor &clrGraph = pSubGraphData->GetColor();
	switch( p_nFillIndex)
	{
		case -1: return clrGraph.GetDownGraphFill();
		case 1: return clrGraph.GetUpGraphFill();
	}
	return FALSE;
}
BOOL CIWGraphInfo::SetSubGraphFillFlag( const int p_nSubGraphIndex, const int p_nFillIndex, const BOOL p_bFill)
{
	CSubGraphData* pSubGraphData = GetSubGraphData( p_nSubGraphIndex);
	if(pSubGraphData == NULL) return FALSE;

	switch( p_nFillIndex)
	{
		case -1:	pSubGraphData->GetColor().SetDownGraphFill( p_bFill ? true : false);
					break;
		case 1:	pSubGraphData->GetColor().SetUpGraphFill( p_bFill ? true : false);
				break;
		default:	return FALSE;
	}
	return TRUE;
}

// (2006/8/11 - Seung-Won, Bae) Pen
int CIWGraphInfo::GetSubGraphPenThickness( const int p_nSubGraphIndex, const int p_nPenIndex)
{
	CSubGraphData* pSubGraphData = GetSubGraphData( p_nSubGraphIndex);
	if(pSubGraphData == NULL) return -1;

	CPenThickness &penThickness = pSubGraphData->GetPenThickness();
	switch( p_nPenIndex)
	{
		case 1: return penThickness.GetThickness1();
		case 2: return penThickness.GetThickness2();
		case 3: return penThickness.GetThickness3();
	}
	return -1;
}
BOOL CIWGraphInfo::SetSubGraphPenThickness( const int p_nSubGraphIndex, const int p_nThickness1, const int p_nThickness2, const int p_nThickness3)
{
	CSubGraphData* pSubGraphData = GetSubGraphData( p_nSubGraphIndex);
	if(pSubGraphData == NULL) return FALSE;

	pSubGraphData->GetPenThickness().SetThickness( p_nThickness1, p_nThickness2, p_nThickness3);
	return TRUE;
}

// 기준선 두께, 모양 설정 - ojtaso (20071213)
int CIWGraphInfo::GetBaseLinePenThickness( const int p_nSubGraphIndex, const int p_nPenIndex)
{
	CSubGraphData* pSubGraphData = GetSubGraphData( p_nSubGraphIndex);
	if(pSubGraphData == NULL) return -1;

	CPenThickness &penThickness = pSubGraphData->GetPenThickness();
	return penThickness.GetBaseLineThickness();
}

// 기준선 두께, 모양 설정 - ojtaso (20071213)
BOOL CIWGraphInfo::SetBaseLinePenThickness( const int p_nSubGraphIndex, const int p_nThickness)
{
	CSubGraphData* pSubGraphData = GetSubGraphData( p_nSubGraphIndex);
	if(pSubGraphData == NULL) return FALSE;

	pSubGraphData->GetPenThickness().SetBaseLineThickness( p_nThickness);
	return TRUE;
}
//////////////////////////////////////////////////////////////////////
// (2006/7/6 - Seung-Won, Bae) Retrieve Graph Info by string
//////////////////////////////////////////////////////////////////////
IString *CIWGraphInfo::GetGraphDataStringForMaking( void)
{
	if( !m_pIndicatorInfo) return NULL;
	CIWString *pIWString = CIWString::CreateObject( NULL);
	m_pIndicatorInfo->GetGraphDataForMaking( *pIWString);
	return pIWString;
}
IString *CIWGraphInfo::GetAllSubGraphDataStringForMaking( void)
{
	if( !m_pIndicatorInfo) return NULL;
	CIWString *pIWString = CIWString::CreateObject( NULL);
	m_pIndicatorInfo->GetAllSubGraphDataForMaking( *pIWString);
	return pIWString;
}
IString *CIWGraphInfo::GetGraphNameString( void)
{
	if( !m_pIndicatorInfo) return NULL;
	return CIWString::CreateObject( m_pIndicatorInfo->GetGraphName());
}
IString *CIWGraphInfo::GetIndicatorNameString( void)
{
	if( !m_pIndicatorInfo) return NULL;
	return CIWString::CreateObject( m_pIndicatorInfo->GetIndicatorName());
}
IString * CIWGraphInfo::GetIndicatorGroupNameString( void)
{
	if( !m_pIndicatorInfo) return NULL;
	return CIWString::CreateObject( m_pIndicatorInfo->GetGroupName());
}

//////////////////////////////////////////////////////////////////////
// (2006/7/6 - Seung-Won, Bae) Retrieve the Graph Data
//////////////////////////////////////////////////////////////////////
void CIWGraphInfo::GetGraphDataDrawingData( BOOL &p_bLowHigh, BOOL &p_bLowCur, BOOL &p_bHighCur, BOOL &p_bPrevCur, int &p_nCurData, BOOL &p_bShowOHLC)
{
	bool bLowHigh	= ( p_bLowHigh	? true : false);
	bool bLowCur	= ( p_bLowCur	? true : false);
	bool bHighCur	= ( p_bHighCur	? true : false);
	bool bPrevCur	= ( p_bPrevCur	? true : false);
	if( m_pIndicatorInfo) m_pIndicatorInfo->GetGraphDataDrawingData( bLowHigh, bLowCur, bHighCur, bPrevCur, p_nCurData, p_bShowOHLC);
	p_bLowHigh	= bLowHigh;
	p_bLowCur	= bLowCur;
	p_bHighCur	= bHighCur;
	p_bPrevCur	= bPrevCur;
}
void CIWGraphInfo::SetGraphDataDrawingData( BOOL p_bLowHigh, BOOL p_bLowCur, BOOL p_bHighCur, BOOL p_bPrevCur, int p_nCurData, BOOL p_bShowOHLC)
{
	if( m_pIndicatorInfo)
	{
		CGraphDataDrawingData graphDataDrawingData( p_bLowHigh ? true : false, p_bLowCur ? true : false, p_bHighCur ? true : false, p_bPrevCur ? true : false, p_nCurData, p_bShowOHLC);
		m_pIndicatorInfo->SetGraphDataDrawingDataWithCopy( &graphDataDrawingData);
	}
}

//////////////////////////////////////////////////////////////////////
// (2006/7/6 - Seung-Won, Bae) Retrieve Signal Info
//////////////////////////////////////////////////////////////////////
BOOL CIWGraphInfo::GetSignalShowFlag( BOOL &p_bValidFlag, BOOL &p_bShowSignal)
{
	if( !m_pIndicatorInfo) return FALSE;
	p_bValidFlag	= m_pIndicatorInfo->IsSignalUse();
	p_bShowSignal	= m_pIndicatorInfo->IsSignalShow();
	return TRUE;
}
BOOL CIWGraphInfo::SetSignalShowFlag( BOOL p_bValidFlag, BOOL p_bShowSignal)
{
	if( !m_pIndicatorInfo) return FALSE;
	m_pIndicatorInfo->SetSignalData(CSignalData( p_bValidFlag ? true : false, p_bShowSignal ? true : false));
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// (2006/8/8 - Seung-Won, Bae) Graph Info Handling
//////////////////////////////////////////////////////////////////////
IGraphInfo *CIWGraphInfo::CreateCopyObject( void)
{
	CIndicatorInfo *pIndicatorInfo = CIndicatorInfo::Make( GetObjectPtr());
	if( !pIndicatorInfo) return NULL;
	return CIWGraphInfo::CreateObject( pIndicatorInfo, TRUE);
}
BOOL CIWGraphInfo::GetCopyDataFrom( IGraphInfo *p_pIGrpahInfo)
{
	if( !m_pIndicatorInfo || !p_pIGrpahInfo) return FALSE;

	// (2006/8/12 - Seung-Won, Bae) Clear SubGraph Cache
	m_nSubGraphIndex	= -1;
	m_pSubGraphData		= NULL;

	CIWGraphInfo *pIWGraphInfo = ( CIWGraphInfo *)p_pIGrpahInfo;
	return m_pIndicatorInfo->GetCopyDataFrom( pIWGraphInfo->GetObjectPtr());
}

BOOL CIWGraphInfo::ChangeAllSubGraphTitleFromCalConditionData( void)
{
	if( !m_pIndicatorInfo) return FALSE;
	return m_pIndicatorInfo->ChangeAllSubGraphTitleFromCalConditionData();
}
