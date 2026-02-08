// GlanceBalanceCursorAddInImp.cpp: implementation of the CGlanceBalanceCursorAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartCommonAddIn.h"
#include "GlanceBalanceCursorAddInImp.h"

#include "../../Include_Chart/Dll_Load/IMetaTable.h"		// for _MTEXT()
#include "../../Include_Chart/MessageDefine.h"				// for UDM_ADDIN_MSG
#include "../../Include_Chart/IPropertyMap.h"				// for IPropertyMap
// 제공되지 않은 메뉴 제거 - ojtaso (20071025)
#include "../Include_AddIn/_resource.h"						// for ID_GLANCEBALANCE_CURSOR
#include "../Include_AddIn/AddInCommand.h"					// for EAI_ST_CALCULATE_INDICATOR
// 제공되지 않은 메뉴 제거 - ojtaso (20071025)
#include "../Include_AddIn/I000000/_ICmdUI.h"				// for ICmdUI
#include "../Include_Addin/I000000/_IPacketManager.h"		// for IPacketManager
#include "../Include_AddIn/I000000/_IPacket.h"				// for IPacket
#include "../Include_AddIn/I000000/_IChartOCX.h"			// for IChartOCX
#include "../Include_AddIn/I000000/_IChartManager.h"		// for IChartManager
#include "../Include_AddIn/I000000/_IBlock.h"				// for IBlock
#include "../Include_AddIn/I000000/_IGraphInfo.h"			// for IGraphInfo
#include "../Include_AddIn/I000000/_IGraphInfoManager.h"	// for IGraphInfoManager
#include "../Include_AddIn/I000000/_IString.h"				// for IDoubleList

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 09 / 06
// Return void	: 
// Param  IChartOCX *p_pIChartOCX : 
// Param  IAddInDllBase *p_pIAddInDllBase : 
// Comments		: 
//-----------------------------------------------------------------------------
CGlanceBalanceCursorAddInImp::CGlanceBalanceCursorAddInImp(IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase)
	: CChartAddInBase( p_pIChartOCX, p_pIAddInDllBase), m_ptCursor( 0, 0)
{
	// 1. 각각의 Flag의 값을 초기화 한다.
	// 처음에는 Span Mouse Cursor는 사용하는 것으로 설정한다.
	m_bGlanceBalanceState = FALSE;			

	m_strIndicatorName = _MTEXT( C2_OBVIOUSLY_BALANCE_CHART);
	
	// 2. View의 환경을 관리하는 변수를 초기화 한다.
	m_rctBlockRegion = CRect(0, 0, 0, 0);
	m_rctGraphDrawingRegion = CRect(0, 0, 0, 0);

	m_nStartIndex = 0;
	m_nEndIndex = 0;
	m_nPrevSpanIndex = 0;
	m_nAfterSpanIndex = 0;

	// (2006/10/17 - Seung-Won, Bae) Manage flag of Enable State.
	m_bEnableGlanceBalanceCursor = TRUE;
}

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 09 / 06
// Comments		: 
// Using way	: 
// See			: destruction
// (2006/11/13 - Seung-Won, Bae) Do not use MainOSB. It has collision with DragZoom Rectangle.
//-----------------------------------------------------------------------------
CGlanceBalanceCursorAddInImp::~CGlanceBalanceCursorAddInImp()
{
}

// Multiple Item in DLL
CString CGlanceBalanceCursorAddInImp::m_strAddInItemName = "GLANCEBALANCE_CURSOR";

//////////////////////////////////////////////////////////////////////
// Define AddIn Event Map
//////////////////////////////////////////////////////////////////////
BEGIN_ADDIN_EVENT_MAP( CGlanceBalanceCursorAddInImp )
	ONADDINEVENT( OnDrawAfterGraph)
	ONADDINEVENT( OnGraphAdded)
	ONADDINEVENT( OnGraphDeleted)
	ONADDINEVENT( OnLoadEnvironment)
	ONADDINEVENT( OnSaveEnvironment)
	ONADDINEVENT( DrawCursor)
	ONADDINEVENT( OnAddInMsg)
	ONADDINEVENT( OnAddInToolCommand_EAICommandType)
	ONADDINEVENT( OnAddInToolCommand_long_EAICommandType)
	ONADDINEVENT( OnMouseMove)
	ONADDINEVENT( OnCmdMsg)		// 제공되지 않은 메뉴 제거 - ojtaso (20071025)
END_ADDIN_EVENT_MAP()

//////////////////////////////////////////////////////////////////////
// (2006/11/12 - Seung-Won, Bae) Support AddIn ENV Value Map
//////////////////////////////////////////////////////////////////////

void CGlanceBalanceCursorAddInImp::OnLoadEnvironment( IPropertyMap *p_pIAddInEnvMap)
{
	const char *szEnvValue = NULL;
	if( !p_pIAddInEnvMap->Lookup( m_strAddInItemName, szEnvValue)) return;
	m_bEnableGlanceBalanceCursor = ( atoi( szEnvValue) == 1);
}
void CGlanceBalanceCursorAddInImp::OnSaveEnvironment( IPropertyMap *p_pIAddInEnvMap)
{
	const char *szEnvValue = NULL;
	if( p_pIAddInEnvMap->Lookup( m_strAddInItemName, szEnvValue)) return;

	CString strEnvValue;
	strEnvValue.Format( "%d", m_bEnableGlanceBalanceCursor ? 1 : 0);
	p_pIAddInEnvMap->SetAt( m_strAddInItemName, strEnvValue);
}

// (2007/1/7 - Seung-Won, Bae) Support AddIn Cursor Operation.
RECT CGlanceBalanceCursorAddInImp::DrawCursor( HDC p_hDC)
{
	CRect rct;
	rct.SetRectEmpty();

	if( !m_bGlanceBalanceState || !m_bEnableGlanceBalanceCursor) return rct;
	if( 0 == m_ptCursor.x && 0 == m_ptCursor.y) return rct;

	// 1. DC의 Handle을 구한다.
	CDC *pDC = CDC::FromHandle( p_hDC);
	if( !pDC) return rct;

	// 2. 현재의 Mouse Position으로 해당되는 자료일자를 얻어온다.
	CRect rcChartOcx;
	m_pChartCWnd->GetClientRect( &rcChartOcx);

	// 4. Span Data에 빈 문자가 있을경우 제거해준다.
	m_strPrevSpan.TrimLeft();	m_strPrevSpan.TrimRight();
	m_strAfterSpan.TrimLeft();	m_strAfterSpan.TrimRight();

	// 5. Span Data의 Size를 구한다.
	CSize szPrevSpan = pDC->GetTextExtent( m_strPrevSpan );
	CRect rctPrevSpan = CRect( m_ptPrevSpan.x - ( szPrevSpan.cx / 2 + 5 ) , m_ptPrevSpan.y - 20,
		m_ptPrevSpan.x + ( szPrevSpan.cx / 2 + 5 ), m_ptPrevSpan.y + szPrevSpan.cy + 3 );

	CSize szAfterSpan = pDC->GetTextExtent( m_strAfterSpan );
	CRect rctAfterSpan = CRect( m_ptAfterSpan.x - ( szAfterSpan.cx / 2 + 5 ) , m_ptAfterSpan.y - 20,
		m_ptAfterSpan.x + ( szAfterSpan.cx / 2 + 5 ), m_ptAfterSpan.y + szAfterSpan.cy + 3 );

	// 8. Memory DC를 구한다.
	CDC MemDC;
	MemDC.CreateCompatibleDC(pDC);
	pDC->SetBkMode(TRANSPARENT);

	// 9. Span Data의 출력에 사용될 이미지를 등록한다.
	ML_SET_DEFAULT_RES();
	CBitmap bitmap;
	bitmap.LoadBitmap(IDB_BMP_GLANCEBALANCE_ARROW);
	BITMAP bmpInfo;
	bitmap.GetBitmap(&bmpInfo);
	CBitmap *pOldBitmap = (CBitmap *)MemDC.SelectObject(&bitmap);	

	// 10. 선행 Span의 Index가 Graph영역의 Start Index보다 작으면 그려준다.
	CRect rcTemp;
	if( m_nStartIndex <= m_nPrevSpanIndex )
	{
		rcTemp = rctPrevSpan;
		rcTemp.DeflateRect( 5, 24, 0, 0 );
		pDC->DrawText( m_strPrevSpan, rcTemp, DT_NOCLIP);

		pDC->BitBlt(rctPrevSpan.left + ((rctPrevSpan.Width()-bmpInfo.bmWidth) /2), rctPrevSpan.top + 5,
			bmpInfo.bmWidth, bmpInfo.bmHeight, &MemDC, 0, 0, SRCAND );
	}

	// 11. 후행 Span의 Index가 Graph영역의 End Index보다 작으면 그려준다.
	//{{ 2007.03.26 by LYH XScale 적용
	//if( m_nAfterSpanIndex <= m_nEndIndex )
	if( m_nAfterSpanIndex <= (m_nEndIndex + m_nAfterSpan) )
	//}}
	{
		rcTemp = rctAfterSpan;
		rcTemp.DeflateRect( 5, 24, 0, 0 );
		pDC->DrawText( m_strAfterSpan, rcTemp, DT_NOCLIP);
	
		pDC->BitBlt(rctAfterSpan.left + ((rctPrevSpan.Width()-bmpInfo.bmWidth) /2), rctAfterSpan.top + 5,
			bmpInfo.bmWidth, bmpInfo.bmHeight, &MemDC, 0, 0, SRCAND );

	}

	// 12. 사용한 이미지를 반납한다.
	MemDC.SelectObject(pOldBitmap);
	MemDC.DeleteDC();
	
	rct.UnionRect( rctPrevSpan, rctAfterSpan);
	return rct;
}

//////////////////////////////////////////////////////////////////////
// Receive Command Message
//////////////////////////////////////////////////////////////////////

BOOL CGlanceBalanceCursorAddInImp::OnAddInToolCommand( const EAICommandType p_eCommandID)
{
	// 일목균형표 설정 커서 적용 - ojtaso (20071109)
	switch(p_eCommandID)
	{
	// (2006/10/17 - Seung-Won, Bae) Manage flag of Enable State.
	case EAI_GLANCEBALANCE_CURSOR_TOGGLE_STATE:
		m_bEnableGlanceBalanceCursor = !m_bEnableGlanceBalanceCursor;
		m_pIChartOCX->InvalidateControl();
		return TRUE;
	case EAI_GLANCEBALANCE_CURSOR_SET_STATE:
		GetSpanData();
		return TRUE;
	}

	return TRUE;
}

BOOL CGlanceBalanceCursorAddInImp::OnAddInToolCommand( long &p_lResult, const EAICommandType p_eCommandID)
{
	if( p_eCommandID != EAI_GLANCEBALANCE_CURSOR_GET_STATE) return FALSE;
	p_lResult = m_bEnableGlanceBalanceCursor;
	return TRUE;
}

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 09 / 19
// Return BOOL	: 
// Param  void  : 
// Comments		: 일목균형표의 IndicatorInfo에서 Span의 값을 얻어온다.
//-----------------------------------------------------------------------------
BOOL CGlanceBalanceCursorAddInImp::GetSpanData( void )
{
	// 1. "선행 Span"과 "후행 Span"에 설정된 값을 얻어온다.
	if( m_pIGraphInfoManager)
	{
		// 1.1 GraphData를 가져온다.

		IGraphInfo *pIGraphInfo = m_pIGraphInfoManager->GetGraphInfo( IGraphInfoManager::IEILT_USER, m_strIndicatorName);
		
		if( !pIGraphInfo)
			return FALSE;

		// 1.2 Graph의 조건값을 읽어온다.
		IDoubleList* pCalcConditionList = pIGraphInfo->GetCommonCalculateConditionList();
		if( pCalcConditionList)
		{
			POSITION pos = pCalcConditionList->GetHeadPosition();
			double dCalcCond = 0;
			
			// 1.2.1 선행 Span값을 가져온다.
			if( pos )
			{
				pCalcConditionList->GetNext( pos, dCalcCond );
				// 일목균형표 설정 커서 2차 적용 - ojtaso (20071113)
				m_nPrevSpan = (int)dCalcCond - 1;
			}

			// 1.2.2 후행 Span값을 가져온다.
			m_nAfterSpan = m_nPrevSpan;


			// 1.2.3 I로 시작되는 Indterface는 반드시 Release()를 해준다.
			pCalcConditionList->Release();
		}

		// 1.3 I로 시작되는 Indterface는 반드시 Release()를 해준다.
		pIGraphInfo->Release();
	}

	return TRUE;
}


//////////////////////////////////////////////////////////////////////
// Drawing Span Data
//////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 09 / 11
// Return void	: 
// Param  HDC p_hDC : 
// Comments		: Draw Event를 AddIn DLL에 알려 처리될 수 있도록 한다.
//				: "일목균형표"의 영역에 Mouse Cursor가 있으면 "선행Span"과
//				: "후행Span"을 표시해준다.
//-----------------------------------------------------------------------------
void CGlanceBalanceCursorAddInImp::OnDrawAfterGraph( HDC p_hDC )
{
	// 1. "Span Cursor"를 사용하지 않으면 return한다.
	if( !m_bGlanceBalanceState || !m_bEnableGlanceBalanceCursor) return;

	// 2. "특수차트"일 때에는 보이지 않게 한다.
	if( m_pIChartOCX->IsOnSpecialChart()) return;

	// 4. Mouse의 position으로 Block의 영역을 구한다.
	GetGraphBlockRegion();
}

//////////////////////////////////////////////////////////////////////
// Receive Mouse Event
//////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 09 / 04
// Return BOOL	: 
// Param  HDC p_hDC : 
// Param  UINT nFlags : 
// Param  POINT &point : 
// Param  const DWORD p_dwMouseActionID : 
// Comments		: Mouse cursor가 "일목균형표"의 위에 있을 경우에만 Mouse cursor와 같이 
//				: "선행Span", "후행Span"을 표시해준다.
//-----------------------------------------------------------------------------
BOOL CGlanceBalanceCursorAddInImp::OnMouseMove( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID, const HBITMAP p_hbmpMainOSB)	// (2006/11/5 - Seung-Won, Bae) Support MainOSB
{
	// 1. "Span Cursor"를 제거하면 처리하지 않고 return한다.
	if( !m_bGlanceBalanceState || !m_bEnableGlanceBalanceCursor) return FALSE;
	
	m_ptCursor.x = 0;
	m_ptCursor.y = 0;

	// 2. "일목균형표" 영역에 Mouse가 위치한 경우를 처리한다.
	if( m_rctBlockRegion.PtInRect( point))
	{
		GetSpanTime( point);
		m_ptCursor = point;

	}

	// return FALSE to pass the Event to a next AddIn Routine
	return FALSE;
}


//////////////////////////////////////////////////////////////////////
// Set Environment data & calculate condition
//////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 09 / 19
// Return BOOL	: 
// Param  CPoint ptPoint : 
// Comments		: 주어진 Point에 해당하는 Block의 Graph Region을 구한다.
//				: Block 영역의 Height에, Graph의 Width 영역을 Input 영역으로 확인한다.
//-----------------------------------------------------------------------------
BOOL CGlanceBalanceCursorAddInImp::GetGraphBlockRegion( void )
{
	// 1. "일목균형표"가 포함된 Block의 Index를 구한다.
	int nR, nC, nV, nG;
	ILPCSTR szGraphName = m_pIChartManager->FindFirstIndicator( m_strIndicatorName, nR, nC, nV, nG, "DEFAULT");
	if( szGraphName.IsEmpty()) return FALSE;

	IBlock *pIBlock = m_pIChartManager->GetBlock( nR, nC );
	if( !pIBlock )
		return FALSE;
		
	// 3. Block의 Region을 구한다.
	CRect rctNoFrameAndMargin;
	pIBlock->GetGraphRegions( m_rctBlockRegion, rctNoFrameAndMargin);

	// 4. Block에서의 Graph Resion을 구한다.
	m_rctGraphDrawingRegion = pIBlock->GetGraphDrwingRegion();
	pIBlock->Release();
	pIBlock = NULL;

	// 5. Block Region을 조정한다.
	m_rctBlockRegion.left = m_rctGraphDrawingRegion.left;
	m_rctBlockRegion.right = m_rctGraphDrawingRegion.right;
	
	return TRUE;
}



//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 09 / 15
// Return BOOL	: 
// Param  CPoint p_ptPoint : 
// Comments		: Mouse의 위치로 Span의 값을 얻어온다.
//-----------------------------------------------------------------------------
BOOL CGlanceBalanceCursorAddInImp::GetSpanTime( CPoint ptPoint)
{
//	// 1. 현재 화면에 보이는 Data의 Start/End Index를 구한다.
//	if( !m_pIChartOCX->GetViewStartEndIndex( m_nStartIndex, m_nEndIndex ))
//		return FALSE;
//
//	// 2. Mouse Point의 Data Index를 구한다.
//	int nDataOnPage = m_nEndIndex - m_nStartIndex + 1;
//	double dBarWidth = double( m_rctGraphDrawingRegion.Width() ) / double( nDataOnPage );
//
//
//	int nDataIndex = m_nStartIndex + int( double( ptPoint.x - m_rctBlockRegion.left ) / dBarWidth );
//
//	// 3. 현재 Mouse Point를 이용하여 각 Span의 Index를 구한다.
//	m_nPrevSpanIndex = nDataIndex - m_nPrevSpan;
//	m_nAfterSpanIndex = nDataIndex + m_nAfterSpan;
//
//	// 4. 각 Span의 기준 Point를 계산한다.
//	int nBarLeftEdge, nBarRightEdge, nX;
//	int nDataOffset = m_nPrevSpanIndex - m_nStartIndex;
//	nBarLeftEdge = m_rctGraphDrawingRegion.left + int( dBarWidth * double( nDataOffset) + 0.5) + 1;
//	nBarRightEdge = m_rctGraphDrawingRegion.left + int( dBarWidth * double( nDataOffset + 1) + 0.5) - 1;
//	nX = ( nBarLeftEdge + nBarRightEdge) / 2;
//
//	m_ptPrevSpan.x = nX;
//	m_ptPrevSpan.y = ptPoint.y;
//
//	nDataOffset = m_nAfterSpanIndex - m_nStartIndex;
//	nBarLeftEdge = m_rctGraphDrawingRegion.left + int( dBarWidth * double( nDataOffset) + 0.5) + 1;
//	nBarRightEdge = m_rctGraphDrawingRegion.left + int( dBarWidth * double( nDataOffset + 1) + 0.5) - 1;
//	nX = ( nBarLeftEdge + nBarRightEdge) / 2;
//
//	m_ptAfterSpan.x = nX;
//	m_ptAfterSpan.y = ptPoint.y;
//
//	// 5. Block에서 XScale 구분자를 얻어온다.
//	int nR, nC;
//	if( !m_pIChartManager->GetBlockIndex( ptPoint, nR, nC))
//		return FALSE;
//	IBlock *pIBlock = m_pIChartManager->GetBlock( nR, nC);
//	if( !pIBlock)
//		return FALSE;
//
//	ILPCSTR szDateCompart = pIBlock->GetDateCompart();
//	m_strDateCompartment = szDateCompart;
//
//	ILPCSTR szTimeCompart = pIBlock->GetTimeCompart();
//	m_strTimeCompartment = szTimeCompart;
//
//
//	// 6. Span Data에 해당되는 문자열을 구한다.
//	GetStringfromDataIdx( m_nPrevSpanIndex, m_strPrevSpan );
//	GetStringfromDataIdx( m_nAfterSpanIndex, m_strAfterSpan );
//
//	pIBlock->Release();
//	return TRUE;

//{{ 2007.03.26 by LYH XScale 적용
	// 1. 현재 화면에 보이는 Data의 Start/End Index를 구한다.
	if( !m_pIChartManager->GetStartEndDrawIdx_Cur("DEFAULT", m_nStartIndex, m_nEndIndex ))
		return FALSE;

	// 2. Mouse Point의 Data Index를 구한다.
	int nDataIndex = m_pIChartManager->FindViewDataIndex_Pt("DEFAULT", ptPoint.x);
	// 3. 현재 Mouse Point를 이용하여 각 Span의 Index를 구한다.
	if(m_pIChartManager->GetGraphXArea_Start() > ptPoint.x)	//Reverse
	{
		m_nPrevSpanIndex = nDataIndex + m_nPrevSpan;
		m_nAfterSpanIndex = nDataIndex - m_nAfterSpan;
	}
	else
	{
		m_nPrevSpanIndex = nDataIndex - m_nPrevSpan;
		m_nAfterSpanIndex = nDataIndex + m_nAfterSpan;
	}
	// 4. Block에서 XScale 구분자를 얻어온다.
	int nR, nC;
	if( !m_pIChartManager->GetBlockIndex( ptPoint, nR, nC))
		return FALSE;
	IBlock *pIBlock = m_pIChartManager->GetBlock( nR, nC);
	if( !pIBlock)
		return FALSE;

	// 5. 각 Span의 기준 Point를 계산한다.
	m_ptPrevSpan.x = m_pIChartManager->GetXPosition("DEFAULT", m_nPrevSpanIndex, nC);
	m_ptPrevSpan.y = ptPoint.y;

	m_ptAfterSpan.x = m_pIChartManager->GetXPosition("DEFAULT", m_nAfterSpanIndex, nC);
	m_ptAfterSpan.y = ptPoint.y;

	ILPCSTR szDateCompart = pIBlock->GetDateCompart();
	m_strDateCompartment = szDateCompart;

	ILPCSTR szTimeCompart = pIBlock->GetTimeCompart();
	m_strTimeCompartment = szTimeCompart;


	// 6. Span Data에 해당되는 문자열을 구한다.
	GetStringfromDataIdx( m_nPrevSpanIndex, m_strPrevSpan );
	GetStringfromDataIdx( m_nAfterSpanIndex, m_strAfterSpan );

	pIBlock->Release();
	return TRUE;
//}}
}



//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 09 / 15
// Return BOOL	: 
// Param  int nDataIdx : 
// Param  CString &p_strSpan : 
// Comments		: 해당 Data Index의 시간 정보를 구한다.
//-----------------------------------------------------------------------------
BOOL CGlanceBalanceCursorAddInImp::GetStringfromDataIdx( int nDataIdx, CString &strSpan )
{
	// 1. 자료일자 Packet을 구한다.
	IPacket *pIPacket = m_pIDefaultPacketManager->GetPacket( _MTEXT( C0_DATE_TIME));
	if( !pIPacket)
	{
		strSpan = "00/00/00";
		return FALSE;
	}

	// 2. Data를 구한다.
	double dMarkTime;
	if( !pIPacket->GetData( nDataIdx, dMarkTime ))
		pIPacket->GetDataPastandFuture( nDataIdx, dMarkTime );
	
	long lMarkTime = (long)dMarkTime;
	CString szMarkTime;
	szMarkTime.Format("%d", lMarkTime);

	// 3. Packet Type을 구한다.
	ILPCSTR szPacketType = pIPacket->GetType();
	CString strPacketType( szPacketType);
	
	int nPacketTypeLength = strPacketType.GetLength();

	// 4. PacketType의 길이와 Data의 길이가 다르면 '0'으로 채워준다.
	//SK에 적용한 루틴을 한화에도 적용함.
	//>>[KSJ000000023]일목 균형표 죽는 현상 수정 sj_kim(20110404)
	int nGap = nPacketTypeLength - szMarkTime.GetLength();
	if(nGap >= 0)
	{
		for( int i = nGap ; i ; i-- )
			szMarkTime = "0" + szMarkTime;
	}
	else
		szMarkTime = szMarkTime.Left(szMarkTime.GetLength() + nGap);
	//<<[KSJ000000023]일목 균형표 죽는 현상 수정 sj_kim(20110404)

	char strChar = strPacketType.GetAt(0);
	strSpan = szMarkTime.GetAt(0);

	// 5. XScale에 사용되는 구분자 의 Type을 구분한다.
	CString strCompartment = m_strDateCompartment;
	BOOL bUseOneCharDateCompartment = ( m_strDateCompartment.GetLength() > 1 ) ? FALSE : TRUE;
	BOOL bUseOneCharTimeCompartment = ( m_strTimeCompartment.GetLength() > 1 ) ? FALSE : TRUE;

	
	// "월"과 "분"의 PacketType이 같이 'M'을 사용하기 때문에 구분해준다.
	BOOL bIsMonth = FALSE;				
	CString strPacketChar = "";
	
	// 6. PacketType의 길이만큼 문자열을 parsing한다.
	ML_SET_LANGUAGE_RES();
	for( int nIdx = 1 ; nPacketTypeLength > nIdx ; nIdx++ )
	{
		strPacketChar = strPacketType.GetAt(nIdx);

		// 6.1 PacketType내에서 Data의 Type이 바뀌면 해당되는 구분자를 문자열에 추가해준다.
		if( strChar != strPacketChar )
		{
			// 6.1.1 Date의 구분자가 "년 월 일"을 사용할 경우 
			if( !bUseOneCharDateCompartment )
			{
				if( strChar == 'Y' && strPacketChar == 'M' )
				{
					strCompartment.LoadString( IDS_YEAR);
					bIsMonth = TRUE;
				}
				
				else if( strChar == 'M' && strPacketChar == 'D' )		strCompartment.LoadString( IDS_MONTH);
				else if( strChar == 'D' && strPacketChar == 'H' )		strCompartment.LoadString( IDS_DAY);
			}

			// 6.1.2 Time의 구분자가 "시 분 초"를 사용할 경우 
			if( !bUseOneCharTimeCompartment )
			{
				if( strChar == 'H' && strPacketChar == 'M' )
				{
					strCompartment.LoadString( IDS_HOUR);
					bIsMonth = FALSE;
				}
				
				else if( strChar == 'M' && strPacketChar == 'S' )		strCompartment.LoadString( IDS_MINUTE);
				else if( strChar == 'S' && strPacketChar == 'N' )		strCompartment.LoadString( IDS_SECOND);
			}


			// 6.1.3 현재 구분자가 일반 문자일경우 날짜에서 시간으로 변경될 때 구분 문자를 변경해준다.
			if( strChar == 'H' && strPacketChar == 'M' && bUseOneCharTimeCompartment )
				strCompartment = m_strTimeCompartment;

			strSpan = strSpan + strCompartment + szMarkTime.GetAt(nIdx);

			strChar = strPacketType.GetAt(nIdx);
		}

		// 6.2 PacketType이 변경되지 않으면 문자를 
		else
		{
			// 6.2.1 PacketType의 변경이 없으면 Span문자를 더해준다.
			strSpan = strSpan + szMarkTime.GetAt(nIdx);
			
			// 6.2.2 szMarkTime을 모두 parsing하고 나서 마지막으로 구분자를 붙여야 될 경우를 확인하고 붙여준다.
			CString strComp;
			if( nPacketTypeLength == ( nIdx + 1 ) )
			{
				if( strChar == 'Y' && !bUseOneCharDateCompartment )
				{
					strComp.LoadString( IDS_YEAR);
					strSpan = strSpan + strComp;
				}
				else if( strChar == 'M' && !bUseOneCharDateCompartment && bIsMonth )
				{
					strComp.LoadString( IDS_MONTH);
					strSpan = strSpan + strComp;
				}
				else if( strChar == 'D' && !bUseOneCharDateCompartment )
				{
					strComp.LoadString( IDS_DAY);
					strSpan = strSpan + strComp;
				}				
				else if( strChar == 'H' && !bUseOneCharTimeCompartment )
				{
					strComp.LoadString( IDS_HOUR);
					strSpan = strSpan + strComp;
				}
				else if( strChar == 'M' && !bUseOneCharTimeCompartment && !bIsMonth)
				{
					strComp.LoadString( IDS_MINUTE);
					strSpan = strSpan + strComp;
				}
				else if( strChar == 'S' && !bUseOneCharTimeCompartment )
				{
					strComp.LoadString( IDS_SECOND);
					strSpan = strSpan + strComp;
				}
			}
		}
	}

	pIPacket->Release();
	return TRUE;
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 09 / 13
// Return BOOL	: 
// Param  long &p_lResult : 
// Param  const EAICommandType p_eCommandID : 입력명령어
// Param  const char *p_szParam : 추가되는 지표명
// Param  CObject *p_pParam : 
// Comments		: 지표추가시 호출됨. 일목균형표를 추가할 경우를 확인하고 해당되는
//				: 작업을 해준다.
//-----------------------------------------------------------------------------
// (2006/11/17 - Seung-Won, Bae) On Graph Added
void CGlanceBalanceCursorAddInImp::OnGraphAdded( const char *p_szGraphName, const char *p_szIndicatorName)
{
	// 2. 현재 추가되는 지표가 "일목균형표"일때 Span값을 나타내는 Mouse Cursor를
	// 사용할 것인지를 확인한다.
	if( m_strIndicatorName != p_szIndicatorName) return;

	// 2.1 Span을 나타내는 Mouse Cursor를 사용하는것으로 설정.
	m_bGlanceBalanceState = TRUE;
	// 2.2 지표를 추가하고 나면 Span 값을 가지고 온다.
	GetSpanData();
}

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 09 / 13
// Return BOOL	: 
// Param  const EAICommandType p_eCommandID : 
// Param  const char *p_szParam : 
// Comments		: 지표제거시 호출됨.
//-----------------------------------------------------------------------------
// (2006/9/27 - Seung-Won, Bae) On Graph Deleted.
void CGlanceBalanceCursorAddInImp::OnGraphDeleted( const char *p_szGraphName, const char *p_szIndicatorName)
{
	// 2. 현재 제거하는 지표가 "일목균형표"일때 Span값을 나타내는 Mouse Cursor를
	// 사용할 것인지를 확인한다.
	if( p_szIndicatorName == m_strIndicatorName)
	{
		// Span을 나타내는 Mouse Cursor를 사용하는것으로 설정.
		m_bGlanceBalanceState = FALSE;
	}
}

// 제공되지 않은 메뉴 제거 - ojtaso (20071025)
BOOL CGlanceBalanceCursorAddInImp::OnCmdMsg( UINT nID, int nCode, ICmdUI *p_pICmdUI)
{
	if( ID_GLANCEBALANCE_CURSOR != nID) return FALSE;
	if( CN_COMMAND == nCode) return OnAddInToolCommand( EAI_GLANCEBALANCE_CURSOR_TOGGLE_STATE);
	else if( CN_UPDATE_COMMAND_UI == nCode)
	{
		if( p_pICmdUI) 
		{
			p_pICmdUI->Enable( TRUE);

			long lResult = 0;
			OnAddInToolCommand( lResult, EAI_GLANCEBALANCE_CURSOR_GET_STATE);
			p_pICmdUI->SetCheck( lResult);
		}
		return TRUE;
	}
	return FALSE;
}
