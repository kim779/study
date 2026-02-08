// ChartPeriodSummaryAddInImp.cpp: implementation of the CChartPeriodSummaryAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartPeriodSummaryAddIn.h"
#include "ChartPeriodSummaryAddInImp.h"

#include "./Include_Chart/MessageDefine.h"				// for UM_CURSOR_EXCHANGE
#include "./Include_Chart/Conversion.h"					// for CDataConversion
#include "./Include_Chart/Dll_Load/IMetaTable.h"		// for g_iMetaTable

#include "resource.h"
#include "PSConfigDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define IDC_HAND MAKEINTRESOURCE(32649)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChartPeriodSummaryAddInImp::CChartPeriodSummaryAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase)
	: CChartAddInBase( p_pIChartOCX, p_pIAddInDllBase)
,	m_nMouseClickStatus(0)	// No click
,	m_bCentered(TRUE)
,	m_nCommandType(0)
,	m_bAttachData(FALSE)
,	m_bMouseCaptured(FALSE)
,	m_pCurSelectedLine(NULL)
,	m_bDrag(FALSE)
,	m_bLoadData(FALSE)
{
	//{{2008.01.30 by LYH
	m_hOcxWnd = NULL;
	if( p_pIChartOCX) m_hOcxWnd = p_pIChartOCX->GetOcxHwnd();
	//}}
}

CChartPeriodSummaryAddInImp::~CChartPeriodSummaryAddInImp()
{
	POSITION pos = m_LineList.GetHeadPosition();
	while (pos)
	{
		CPeriodObj* pLine = (CPeriodObj*)m_LineList.GetNext(pos);
		delete pLine;
		pLine = NULL;
	}
	m_LineList.RemoveAll();
//	delete m_pBmp;
}

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
CString CChartPeriodSummaryAddInImp::m_strAddInItemName = "PERIOD_SUMMARY";

//////////////////////////////////////////////////////////////////////
// Define AddIn Event Map
//////////////////////////////////////////////////////////////////////
BEGIN_ADDIN_EVENT_MAP( CChartPeriodSummaryAddInImp)
	ONADDINEVENT( OnDrawAfterGraph)
	ONADDINEVENT( OnDrawBeforeGraph)
	ONADDINEVENT( OnPacketDataMultiItem)
	ONADDINEVENT( OnResetChart)
	ONADDINEVENT( OnToolCommand)
	ONADDINEVENT( OnLoadEnvironment)
	ONADDINEVENT( OnSaveEnvironment)
	ONADDINEVENT( DrawCursor)
	ONADDINEVENT( OnCaptureChanged)
	ONADDINEVENT( OnCmdMsg)
	ONADDINEVENT( OnKeyDown)
	ONADDINEVENT( OnMouseMove)
	ONADDINEVENT( OnLButtonDown)
	ONADDINEVENT( OnLButtonUp)
	ONADDINEVENT( OnLButtonDblClk)
	ONADDINEVENT( OnRButtonDown)
	ONADDINEVENT( OnRButtonUp)
	ONADDINEVENT( OnAddInToolCommand_EAICommandType_char_long)
END_ADDIN_EVENT_MAP()

//////////////////////////////////////////////////////////////////////
// Overrides
//////////////////////////////////////////////////////////////////////


// (2006/11/24 - Seung-Won, Bae) On Tool Command
void CChartPeriodSummaryAddInImp::OnToolCommand( const int p_nToolType, const int p_nCmdOption)
{
	CToolOptionInfo::TOOLOPTION eToolOption = (CToolOptionInfo::TOOLOPTION)p_nToolType;
	
	if( eToolOption == CToolOptionInfo::T_DELETE_ALL )
	{
		POSITION pos = m_LineList.GetHeadPosition();
		while (pos)
		{
			CPeriodObj* pLine = (CPeriodObj*)m_LineList.GetNext(pos);
			delete pLine;
			pLine = NULL;
		}
		m_LineList.RemoveAll();
		return;
	}
	
	if(eToolOption == CToolOptionInfo::T_TOOL_PERIOD_SUMMARY)
	{
		if(p_nCmdOption != 3)
			m_nCommandType = p_nCmdOption;
		switch(m_nCommandType)
		{
		case 1:			// draw mode
			{
				m_nCommandType = p_nCmdOption;
//				int		nR, nC ;
//				if( !GetChartBasicData(m_nStartIndex, m_nEndIndex, nR, nC, m_rctBlockRegion, m_rctGraphRegion))
//					return FALSE;

				m_pIChartOCX->OnToolStateChanged( CToolOptionInfo::T_TOOL_PERIOD_SUMMARY, TRUE );	
			}
			break;
		case 2:			// modify mode
			break;
		}
		return;
	}

	if(eToolOption == CToolOptionInfo::T_TOOL_SELECTION)
	{
		if( p_nCmdOption == CToolOptionInfo::T_TOOL_PERIOD_SUMMARY || p_nCmdOption == CToolOptionInfo::T_DELETE_INORDER) return;
		m_nCommandType = 2;
		m_pIChartOCX->OnToolStateChanged( CToolOptionInfo::T_TOOL_PERIOD_SUMMARY, FALSE );
		if(m_pCurSelectedLine)
		{
			m_pCurSelectedLine->m_bSelected = FALSE;
			m_pCurSelectedLine = NULL;
			m_pIChartOCX->InvalidateControl();
		}
		return;
	}

	if(eToolOption == CToolOptionInfo::T_ALL_TOOL_TRUE_QUERY)
	{
		if( (m_pIChartOCX != NULL) && (m_nCommandType == 1))
			m_pIChartOCX->OnToolStateChanged( CToolOptionInfo::T_TOOL_PERIOD_SUMMARY, TRUE );	
		return;
	}
	
	if(eToolOption == CToolOptionInfo::T_ALL_TOOL_RESET_STATE)
	{
		m_nCommandType = 2;
		return;
	}
}

BOOL CChartPeriodSummaryAddInImp::OnAddInToolCommand(	const EAICommandType p_eCommandID, const char *p_szParam, const long p_lParam)
{
	if(p_eCommandID == EAI_TOOLORDERMGR_DELETE_THIS_TOOL)
	{
		if(strcmp(p_szParam, "PERIOD_SUMMARY")) return FALSE;
		CPeriodObj * pLine = (CPeriodObj *)p_lParam;
		if(!pLine) return FALSE;
		POSITION pos = m_LineList.Find(pLine);
		m_LineList.RemoveAt(pos);
		delete pLine;
		pLine = NULL;
		m_pIChartOCX->InvalidateControl();
	}
	// 20081007 JS.Kim	사용자시간대 선택
	else if( p_eCommandID == EAI_USER_CHANGE_TIMEDIFF )
	{
		RecalcLineXPoint(p_szParam, (int)p_lParam);
		return FALSE;		// 다른 AddIn에서도 처리할 수 있도록 FALSE를 return한다.
	}

	return FALSE;
}


void CChartPeriodSummaryAddInImp::OnResetChart()
{
	ML_SET_LANGUAGE_RES();

	//////////////////////////////////////////////////////////////////////////
	// 기존 데이터 클리어
	POSITION pos = m_LineList.GetHeadPosition();
	while (pos)
	{
		CPeriodObj* pLine = (CPeriodObj*)m_LineList.GetNext(pos);
		delete pLine;
		pLine = NULL;
	}
	m_LineList.RemoveAll();

	m_nCommandType = 0;

	if( m_pIChartOCX != NULL )
		m_pIChartOCX->OnToolStateChanged( CToolOptionInfo::T_TOOL_PERIOD_SUMMARY, FALSE );	

	//////////////////////////////////////////////////////////////////////////
}

// RQ별 Packet : 복수종목 - ojtaso (20070111)
void CChartPeriodSummaryAddInImp::OnPacketDataMultiItem( LPCTSTR lpszRQ, const char *p_szPacketNames, BOOL p_bWithReset)
{
	ML_SET_LANGUAGE_RES();

	//////////////////////////////////////////////////////////////////////////
	// 오브젝트 데이터 클리어
	POSITION pos = m_LineList.GetHeadPosition();
	while (pos)
	{
		CPeriodObj* pLine = (CPeriodObj*)m_LineList.GetNext(pos);
		delete pLine;
		pLine = NULL;
	}
	m_LineList.RemoveAll();
}

BOOL CChartPeriodSummaryAddInImp::OnCmdMsg(UINT nID, int nCode, ICmdUI *p_pICmdUI)
{
	ML_SET_LANGUAGE_RES();
	if(!m_nCommandType) return FALSE;

	if( CN_UPDATE_COMMAND_UI == nCode)
	{
		switch( nID)
		{
		case ID_KSA_PERIODSUMMARY_CONFIG:
		case ID_KSA_PERIODSUMMARY_DELETE:
		case ID_KSA_COMMON_RELEASE:
		case ID_KSA_PERIODSUMMARY_EXECUTE:
			if( p_pICmdUI)
			{
				p_pICmdUI->Enable(TRUE);
			}
			break;
		default:
			return FALSE;
		}
	}

	if( CN_COMMAND == nCode)
	{
		switch(nID)
		{
		case ID_KSA_PERIODSUMMARY_CONFIG:
			OnPSConfig(m_pCurSelectedLine);
			break;
		case ID_KSA_PERIODSUMMARY_DELETE:
			OnDeleteLine();
			break;
		case ID_KSA_COMMON_RELEASE:
			OnReleaseDraw();
			break;
		case ID_KSA_PERIODSUMMARY_EXECUTE:
			OnToolCommand( CToolOptionInfo::T_TOOL_PERIOD_SUMMARY, 1);
			break;
		default:
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CChartPeriodSummaryAddInImp::OnMouseMove( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID, const HBITMAP p_hbmpMainOSB)
{
	ML_SET_LANGUAGE_RES();
	if(!m_nCommandType) return FALSE;
	POINT ptTemp = point;

	CPoint pt = point;

	if(!m_pCurSelectedLine) return FALSE;
	if( p_dwMouseActionID == m_dwMouseActionID)
	{
		m_bDrag = TRUE;

		int ix, is, ie;
		if( m_pCurSelectedLine->m_rctGraphRegion.PtInRect(ptTemp))
		{
			m_pCurSelectedLine->SetPos(pt, m_pCurSelectedLine->m_CurClickedPosition);
			if(		(GetIndexFromPoint( m_pCurSelectedLine->m_strRQ, pt, ix, ix) == R_OK) 
				&&	(GetIndexFromPoint( m_pCurSelectedLine->m_strRQ, m_pCurSelectedLine->m_ptAStart, is, is) == R_OK)
				&&	(GetIndexFromPoint( m_pCurSelectedLine->m_strRQ, m_pCurSelectedLine->m_ptAEnd, ie, ie) == R_OK) 
			  )
			{
				if(IsValidIndex( m_pCurSelectedLine->m_strRQ, ix) && IsValidIndex( m_pCurSelectedLine->m_strRQ, is) && IsValidIndex( m_pCurSelectedLine->m_strRQ, ie))
					m_ptCursor = point;
 				else
 				{
 					//point = m_ptCursor;
 					TRACE1("\nTEST:%d\n",point.x);
 				}
			}
		}

		return TRUE;
	}
	else if(m_nCommandType != 1)
	{
	//	SetCursor(::LoadCursor(NULL, IDC_ARROW));

		POSITION pos = m_LineList.GetHeadPosition();
		while(pos)
		{
			CPeriodObj* pLine = (CPeriodObj*)m_LineList.GetNext(pos);
			if( pLine->IsInRegion(point) )
			{
			//	m_pIChartOCX->SetCursorHandle(::LoadCursor(NULL, IDC_HAND));
				return TRUE;
			}
		}
		return FALSE;
	}
	if( m_ptInit == point)	return FALSE;


	return FALSE;
	
}

BOOL CChartPeriodSummaryAddInImp::OnLButtonDown(HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	ML_SET_LANGUAGE_RES();

	if(!m_nCommandType) return FALSE;
	POINT ptTemp = point;

	// (2006/2/13 - Seung-Won, Bae) Check Empty Chart!
	if( !m_pIChartOCX->IsUseToolScrollBar() || m_pIChartOCX->IsOnSpecialChart() || m_pIChartManager->IsEmpty()) return FALSE;
	CRect rctRegion = m_pIChartManager->GetGraphRegionSumInColumn( ptTemp);
	if( rctRegion.IsRectEmpty()) return FALSE;

	m_ptInit = point;
	if(!rctRegion.PtInRect(ptTemp)) return FALSE;

	CDC * pDC = CDC::FromHandle(p_hDC);
//	m_pIChartOCX->SetCursorHandle(::LoadCursor(NULL, IDC_HAND));
	
	m_dwMouseActionID = p_dwMouseActionID;
	
	BOOL bSpace=TRUE;
	
	POSITION pos = m_LineList.GetHeadPosition();
	while(pos)
	{
		CPeriodObj* pLine = (CPeriodObj*)m_LineList.GetNext(pos);
		if( pLine->IsInRegion(point) && m_nCommandType == 2)
		{
			if( !m_pIAddInToolMgr->IsLButtonExclusive( m_pIAddInDllBase))
			{
				if( !m_pIAddInToolMgr->GetLButtonExclusive( m_pIAddInDllBase)) return FALSE;
			}

			m_bNewDraw = FALSE;
			if(m_pCurSelectedLine)
				m_pCurSelectedLine->m_bSelected = FALSE;
			m_pCurSelectedLine = pLine;
			m_pCurSelectedLine->ButtonDownAt(point);
			POSITION pos2 = m_LineList.Find(m_pCurSelectedLine);
			m_LineList.RemoveAt(pos2);
			m_LineList.AddTail(m_pCurSelectedLine);
			m_pCurSelectedLine->m_bSelected = TRUE;
			m_ptCursor = point;
//			TRACE2("\n%d, %d\n", m_ptCursor.x, m_ptCursor.y);
			if( m_pIChartOCX != NULL )
			{

				m_pIChartOCX->RunToolCommand( CToolOptionInfo::T_TOOL_SELECTION,	
													CToolOptionInfo::T_TOOL_PERIOD_SUMMARY );	
			}

			bSpace = FALSE;
			return TRUE;
			break;
		}
		else
		{
			bSpace = TRUE;
		}
	}

	if(bSpace && (m_nCommandType == 1))
	{
		if( !m_pIAddInToolMgr->IsLButtonExclusive( m_pIAddInDllBase))
		{
			if( !m_pIAddInToolMgr->GetLButtonExclusive( m_pIAddInDllBase)) return FALSE;
		}
		m_bMouseCaptured = TRUE;
		m_ptCursor = point;
		m_bNewDraw = TRUE;
 		int ix, iy;

		// Get Main RQ.
 		CPoint ptAt = point;
		int nRow = -1, nCol = -1;
		if( !m_pIChartManager->GetBlockIndex( ptAt, nRow, nCol)) return FALSE;
		
		CString strRQ;
		ILPCSTR szMainRQ = m_pIChartOCX->GetCurrentRQ();
		int nR = nRow - 1, nC = nCol, nV = -1, nG = -1;
		ILPCSTR szGraphName = m_pIChartManager->FindFirstIndicator( _MTEXT( C2_PRICE_CHART), nR, nC, nV, nG, szMainRQ);
		if( nR != nRow || nC != nCol)
		{
			IBlock *pIBlock = m_pIChartManager->GetBlock( nRow, nCol);
			if( pIBlock)
			{
				szMainRQ = pIBlock->GetMainRQ();
				pIBlock->Release();
			}
		}
		if( szMainRQ.IsEmpty()) strRQ = "DEFAULT";
		else strRQ = szMainRQ;

		// 시작일 및 종가
 		if( GetIndexFromPoint( strRQ, ptAt, ix, iy) == R_ERROR)
			return FALSE;

		IPacketManager *pIPacketManager = m_pIPacketListManager->GetPacketManager( strRQ);
		if( !pIPacketManager) return FALSE;

		IPacket *pIPacketForCur = pIPacketManager->GetPacket( _MTEXT(C0_CLOSE));
		IPacket *pIPacketForRef = pIPacketManager->GetPacket( _MTEXT(C0_OPEN));
		IPacket *pIPacketTime = pIPacketManager->GetPacket( _MTEXT( C0_DATE_TIME));
		if( !pIPacketForCur || !pIPacketForRef || !pIPacketTime)
		{
			if( pIPacketForCur) pIPacketForCur->Release();
			if( pIPacketForRef) pIPacketForRef->Release();
			if( pIPacketTime) pIPacketTime->Release();
			pIPacketManager->Release();
			return FALSE;
		}

		double dCur, dRef;
		if( !pIPacketForCur->GetData( iy, dCur) || !pIPacketForRef->GetData( iy, dRef))
		{
			pIPacketForCur->Release();
			pIPacketForRef->Release();
			pIPacketTime->Release();
			pIPacketManager->Release();
			return FALSE;
		}

		CPeriodObj* pLine = new CPeriodObj;
		pLine->SetChartPeriodSummaryAddInImp(this);
 		pLine->m_Num = m_LineList.GetCount();
//		pLine->m_nDateType = m_nDateType;

		// (2008/11/5 - Seung-Won, Bae) for Multiple Item.
		pLine->m_strRQ = strRQ;
		GetDateType( pLine);

		pLine->m_nRow = nRow;
		pLine->m_nCol = nCol;
		if( !GetChartBasicData( pLine))
		{
			delete pLine;
			return FALSE;
		}

		double dClickedValue = m_pIChartOCX->ConvertYPositionToData(ptAt.y, pLine->m_rctGraphRegion.top, pLine->m_rctGraphRegion.bottom, pLine->m_dViewMax, pLine->m_dViewMin, pLine->m_bLog, pLine->m_bReverse);
		if( dClickedValue >= max(dRef, dCur)) pLine->SetUpDown(UP);
		else if(dClickedValue <= min(dRef, dCur)) pLine->SetUpDown(DOWN);
		else pLine->SetUpDown(UP);

 		pLine->ButtonDownAt(ptAt);
		pLine->m_CurClickedPosition = E_PT;
		
		if(m_pCurSelectedLine)	m_pCurSelectedLine->m_bSelected = FALSE;	// 기존에 선택된 선이 있으면 선택 해제

		m_pCurSelectedLine = pLine;
		m_pCurSelectedLine->m_bSelected = TRUE;

		// 20081007 JS.Kim	사용자시간대 선택 <<
		pLine->m_nTimeDiff = pIPacketManager->GetBaseTimeDifference();

		m_LineList.AddTail(pLine);
		RegPS((long)pLine);
		
		pIPacketForCur->Release();
		pIPacketForRef->Release();
		pIPacketTime->Release();
		pIPacketManager->Release();

		return TRUE;
	}
	else if(bSpace && (m_nCommandType != 1))	// no action
 	{
		if(m_pCurSelectedLine)
			m_pCurSelectedLine->m_bSelected = FALSE;
		m_pCurSelectedLine = NULL;
		return FALSE;
 	}

	return FALSE;
}

BOOL CChartPeriodSummaryAddInImp::OnLButtonUp(HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	ML_SET_LANGUAGE_RES();
	if(!m_nCommandType) return FALSE;

	CDC * pDC = CDC::FromHandle(p_hDC);
	if( !pDC) return FALSE;

	// (2006/2/6 - Seung-Won, Bae) Manage Mouse Action ID for Serial Action.
	if( !m_pIAddInToolMgr->ReleaseLButtonExclusive( m_pIAddInDllBase)) return FALSE;


	if(p_dwMouseActionID == m_dwMouseActionID)
	{
		m_bDrag = FALSE;
 		int ix = 0, iy = 0;
// 		double dDummy;
 		if(!m_pCurSelectedLine) return FALSE;
		if(m_pCurSelectedLine->m_nStartIndex == m_pCurSelectedLine->m_nEndIndex)
		{
		//	m_pCurSelectedLine->Erase(&m_dcMem);
			POSITION pos = m_LineList.Find(m_pCurSelectedLine);
			m_LineList.RemoveAt(pos);
			UnregPS((long)m_pCurSelectedLine);
			delete m_pCurSelectedLine;
			m_pCurSelectedLine = NULL;
	//		return TRUE;
		}
		m_pIChartOCX->InvalidateControl();
	//	m_pIChartOCX->SetCursorHandle(::LoadCursor(NULL, IDC_ARROW));
	}

	return TRUE;


}

BOOL CChartPeriodSummaryAddInImp::OnRButtonDown(HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	ML_SET_LANGUAGE_RES();

	switch(m_nCommandType)
	{
	case 0:	// stop
		return FALSE;
	case 1:	// draw
		return TRUE;
		break;
	case 2: // modify
		POSITION tmpPos;
		POSITION pos = m_LineList.GetHeadPosition();
		while(pos)
		{
			tmpPos = pos;
			CPeriodObj* pLine = (CPeriodObj*)m_LineList.GetNext(pos);
			if( pLine->IsInRegion(point) )
			{
				m_PosForRemove = tmpPos;
				m_dwMouseActionID = p_dwMouseActionID;
				m_bNewDraw = FALSE;
				m_pCurSelectedLine = pLine;
				m_pCurSelectedLine->ButtonDownAt(point);			
				return TRUE;
			}
		}	
		return FALSE;
		break;
	}
	return FALSE;
}

BOOL CChartPeriodSummaryAddInImp::OnRButtonUp(HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	ML_SET_LANGUAGE_RES();
	if(!m_nCommandType) return FALSE;

	CPoint clPoint(point);

		//////////////////////////////////////////////////////////////////////////
		// popup menu 생성

		CMenu menu;
		VERIFY(menu.LoadMenu(IDR_MENUPS));

		switch(m_nCommandType)
		{
		case 1:	// draw mode
			::ClientToScreen(m_pIChartOCX->GetOcxHwnd(),&clPoint);
			m_hPopupMenu = GetSubMenu(menu.GetSafeHmenu(),0);
			break;
		case 2: // modify mode
			{
				POSITION tmpPos;
				POSITION pos = m_LineList.GetHeadPosition();
				while(pos)
				{
					tmpPos = pos;
					CPeriodObj* pLine = (CPeriodObj*)m_LineList.GetNext(pos);
					if( pLine->IsInRegion(clPoint) )
					{
						m_PosForRemove = tmpPos;
						m_dwMouseActionID = p_dwMouseActionID;
						m_bNewDraw = FALSE;
						m_pCurSelectedLine = pLine;
						m_pCurSelectedLine->ButtonDownAt(clPoint);			
						break;
					}
				}	

				if(p_dwMouseActionID == m_dwMouseActionID)
				{
					::ClientToScreen(m_pIChartOCX->GetOcxHwnd(),&clPoint);
					m_hPopupMenu = GetSubMenu(menu.GetSafeHmenu(),1);
				}
				else
					return FALSE;
				
			}
			break;
		default:
			return FALSE;
		}
		
		TrackPopupMenu(m_hPopupMenu, TPM_LEFTALIGN | TPM_RIGHTBUTTON, clPoint.x, clPoint.y,	0, m_pIChartOCX->GetOcxHwnd(), NULL);
//		DestroyMenu(m_hPopupMenu);
		//////////////////////////////////////////////////////////////////////////
//		TRACE1("\nthis:%ld",this);
		return TRUE;
//	}
//	return FALSE;
}


BOOL CChartPeriodSummaryAddInImp::OnLButtonDblClk(HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	ML_SET_LANGUAGE_RES();
	if(!m_nCommandType) return FALSE;

	POSITION pos = m_LineList.GetHeadPosition();
	while(pos)
	{
		CPeriodObj* pLine = (CPeriodObj*)m_LineList.GetNext(pos);
		if( pLine->IsInRegion(point) && m_nCommandType == 2)
		{
			OnPSConfig(pLine);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CChartPeriodSummaryAddInImp::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	ML_SET_LANGUAGE_RES();
	if(!m_nCommandType) return FALSE;
	
	if(nChar == 46)
	{
		if(!m_pCurSelectedLine) return FALSE;
//		m_pCurSelectedLine->Erase(&m_dcMem);
		POSITION pos = m_LineList.Find(m_pCurSelectedLine);
		m_LineList.RemoveAt(pos);
		UnregPS((long)m_pCurSelectedLine);
		delete m_pCurSelectedLine;
		m_pCurSelectedLine = NULL;
		m_pIChartOCX->InvalidateControl();
		return TRUE;
	}

	return FALSE;
}

void CChartPeriodSummaryAddInImp::OnDrawBeforeGraph(HDC p_hDC)
{
	ML_SET_LANGUAGE_RES();
}

void CChartPeriodSummaryAddInImp::OnDrawAfterGraph(HDC p_hDC)
{
	ML_SET_LANGUAGE_RES();
	try
	{
		CDC * pDC = CDC::FromHandle(p_hDC);

		// 20081007 JS.Kim	사용자시간대 선택
		POSITION pos = m_LineList.GetHeadPosition();
		while(pos)
		{
			CPeriodObj* pLine = (CPeriodObj*)m_LineList.GetNext(pos);
			if( !pLine) continue;

			CRgn rgn;
			rgn.CreateRectRgn( ( int)pLine->m_rctGraphRegion.left,
				( int)pLine->m_rctGraphRegion.top,
				( int)pLine->m_rctGraphRegion.right,
				( int)pLine->m_rctGraphRegion.bottom);
			pDC->SelectClipRgn(&rgn);

			CRect rcPrev = pLine->m_rctBlockRegion;
			if( !GetChartBasicData( pLine))
				continue;

			if(pLine->m_bSaved)
			{
				int iDummy=0;
				pLine->m_bSaved = FALSE;
				// 20081007 JS.Kim	사용자시간대 선택<<
				IPacketManager *pIPacketManager = m_pIPacketListManager->GetPacketManager( pLine->m_strRQ);
				if( pIPacketManager)
				{
					IPacket *pIPacket = pIPacketManager->GetPacket(  _MTEXT( C0_DATE_TIME));
					if( pIPacket)
					{
						int nPckTimeDiff = pIPacketManager->GetBaseTimeDifference();
						if( nPckTimeDiff != pLine->m_nTimeDiff )
						{
							pLine->m_lSDate = (__int64)pIPacket->CalcTimeDiff((double)pLine->m_lSDate, nPckTimeDiff - pLine->m_nTimeDiff);
							pLine->m_lEDate = (__int64)pIPacket->CalcTimeDiff((double)pLine->m_lEDate, nPckTimeDiff - pLine->m_nTimeDiff);
							pLine->m_nTimeDiff = nPckTimeDiff;
						}
						pIPacket->Release();
					}
					pIPacketManager->Release();
				}
				//>>
				GetIndexFromValue( pLine->m_strRQ, pLine->m_nEndIndexOfView, pLine->m_lSDate, 0, pLine->m_nStartIndex, iDummy);
				GetIndexFromValue( pLine->m_strRQ, pLine->m_nEndIndexOfView, pLine->m_lEDate, 0, pLine->m_nEndIndex, iDummy);
				RegPS((long)pLine);
			}
	
			if((pLine == m_pCurSelectedLine) && (m_pIAddInToolMgr->IsLButtonExclusive( m_pIAddInDllBase))) 
			{
				continue;
			}
 			GetPointFromIndex( pLine->m_strRQ, pLine->m_nStartIndex,0,pLine->m_ptAStart);
 			GetPointFromIndex( pLine->m_strRQ, pLine->m_nEndIndex,0,pLine->m_ptAEnd);
 			pLine->m_ptAStart.y = m_pIChartOCX->ConvertDataToYPosition( pLine->m_dSClicked, pLine->m_dViewMax, pLine->m_dViewMin, pLine->m_rctGraphRegion.top, pLine->m_rctGraphRegion.bottom, pLine->m_bLog, pLine->m_bReverse);
 			pLine->m_ptAEnd.y = m_pIChartOCX->ConvertDataToYPosition( pLine->m_dEClicked, pLine->m_dViewMax, pLine->m_dViewMin, pLine->m_rctGraphRegion.top, pLine->m_rctGraphRegion.bottom, pLine->m_bLog, pLine->m_bReverse);
	
			if( GetPriceInfo( pLine->m_strRQ, &pLine->m_Price, pLine->m_nStartIndex, pLine->m_nEndIndex) == R_ERROR) goto Drawing;
			if( GetCandleInfo( pLine->m_strRQ, &pLine->m_Candle, pLine->m_nStartIndex, pLine->m_nEndIndex) == R_ERROR) goto Drawing;
			if( GetAmountInfo( pLine->m_strRQ, &pLine->m_Amount, pLine->m_nStartIndex, pLine->m_nEndIndex) == R_ERROR) goto Drawing;
			
//			TRACE2("\n\nsidx : %d, eidx : %d\n\n",pLine->m_nStartIndex,pLine->m_nEndIndex);
Drawing:
			pLine->Draw(pDC, m_hOcxWnd);

			rgn.DeleteObject();
		}
		
		//pDC->SelectClipRgn(&rgn, NULLREGION);
		pDC->SelectClipRgn(NULL);
	}
	catch (CChartPeriodSummaryAddInImp* )
		{
		
	}
}

BOOL CChartPeriodSummaryAddInImp::InvokeAddIn(int p_nCommandType, long p_lData)
{
	ML_SET_LANGUAGE_RES();
	m_nCommandType = p_nCommandType;
	switch(p_nCommandType)
	{
	case 0:			// stop mode
		break;
	case 1:			// draw mode
		{
//			int		nR, nC ;
//			if( !GetChartBasicData(m_nStartIndex, m_nEndIndex, nR, nC, m_rctBlockRegion, m_rctGraphRegion))
//				return FALSE;
		}
		break;
	case 2:			// modify mode
		break;
	}
	return TRUE;
}

BOOL CChartPeriodSummaryAddInImp::GetChartBasicData( CPeriodObj* p_pLine)
{
	// 가격차트가 포함된 Block의 Index를 구한다.
	int nR = p_pLine->m_nRow - 1, nV, nG;
	ILPCSTR szGraphName = m_pIChartManager->FindFirstIndicator( _MTEXT( C2_PRICE_CHART), nR, p_pLine->m_nCol, nV, nG, p_pLine->m_strRQ);
	if( szGraphName.IsEmpty()) return FALSE;

	// 현 Data View의 Start/End Index를 구한다.
	if( !m_pIChartManager->GetStartEndDrawIdx_Cur( p_pLine->m_strRQ, p_pLine->m_nStartIndexOfView, p_pLine->m_nEndIndexOfView)) return FALSE;
	
	// Block의 Log/뒤집기 설정을 확인하여 정확하게 Data를 변환한다. (가격<->Y Pos)
	IBlock *pIBlock = m_pIChartManager->GetBlock( p_pLine->m_nRow, p_pLine->m_nCol);
	if( pIBlock)
	{
		p_pLine->m_bLog = pIBlock->IsLogScale();
		p_pLine->m_bReverse = pIBlock->IsVScaleInvert( nV);
		pIBlock->GetVScaleGroupMinMax( nV, p_pLine->m_dViewMin, p_pLine->m_dViewMax);
	   	pIBlock->GetGraphRegions( p_pLine->m_rctGraphRegion, p_pLine->m_rctBlockRegion);
		p_pLine->m_rctGraphRegion = pIBlock->GetGraphDrwingRegion();
 		//m_ptOffSet.x = p_rctGraphRegion.left;
		//m_ptOffSet.y = p_rctGraphRegion.top;

		pIBlock->Release();
		pIBlock = NULL;

	}

	return TRUE;
}

//BOOL CChartPeriodSummaryAddInImp::GetValueFromPoint(CPoint& ptAt/*IN, OUT*/, long &x/*OUT*/, double &y/*OUT*/)
//{
//	int ix, iy;
//	if(GetIndexFromPoint(ptAt, ix, iy) == -1) return FALSE;
//	if(GetValueFromIndex(ix, iy, x, y) == -1) return FALSE;
//	return TRUE;
//}

int CChartPeriodSummaryAddInImp::GetIndexFromPoint( const char *p_szRQ, CPoint& pt/*IN, OUT*/, int &x/*IN*/, int &y/*IN*/)
{
//	POINT ptTemp = pt;
//	ptTemp.x += m_ptOffSet.x;
//	ptTemp.y += m_ptOffSet.y;
//
//	int nStartIndex, nEndIndex;
//	if( !m_pIChartOCX->GetViewStartEndIndex( nStartIndex, nEndIndex)) return R_ERROR;
//	// 1.3 지정된 Mouse Point의 Data Index를 구한다.
//	x = y = nStartIndex + ( ( pt.x - m_rctBlockRegion.left) * ( nEndIndex - nStartIndex + 1)) / m_rctBlockRegion.Width();
//
//	if(m_bAttachData)
//	{
//		if(m_bCentered)	// 중심선에 정렬
//			ptTemp.x = (x*m_rctBlockRegion.Width() -m_nStartIndex*m_rctBlockRegion.Width() + m_rctBlockRegion.Width()/2.)/
//						( m_nEndIndex - m_nStartIndex + 1) + m_rctBlockRegion.left;
//		else
//			ptTemp.x = (x*m_rctBlockRegion.Width() - m_nStartIndex*m_rctBlockRegion.Width())/
//						( m_nEndIndex - m_nStartIndex + 1) + m_rctBlockRegion.left;
//	}
//
//	POINT ptAt;
//	ptAt.x = pt.x;
//	ptAt.y = pt.y;

	int nStartIndex, nEndIndex;
	if( !m_pIChartManager->GetStartEndDrawIdx_Cur( p_szRQ, nStartIndex, nEndIndex)) return R_ERROR;
	// 1.3 지정된 Mouse Point의 Data Index를 구한다.
	//x = y = nStartIndex + ( ( pt.x - m_rctBlockRegion.left) * ( nEndIndex - nStartIndex + 1)) / m_rctBlockRegion.Width();
	x = y = m_pIChartManager->FindViewDataIndex_Pt( p_szRQ, pt.x, 0, TRUE);

//	if(m_bAttachData)
//	{
//		if(m_bCentered)	// 중심선에 정렬
//			ptTemp.x = (x*m_rctBlockRegion.Width() -m_nStartIndex*m_rctBlockRegion.Width() + m_rctBlockRegion.Width()/2.)/
//						( m_nEndIndex - m_nStartIndex + 1) + m_rctBlockRegion.left;
//		else
//			ptTemp.x = (x*m_rctBlockRegion.Width() - m_nStartIndex*m_rctBlockRegion.Width())/
//						( m_nEndIndex - m_nStartIndex + 1) + m_rctBlockRegion.left;
//	}

//	POINT ptAt;
//	ptAt.x = pt.x;
//	ptAt.y = pt.y;


	return R_OK;
}

/*
int CChartPeriodSummaryAddInImp::GetValueFromIndex(int ix, int iy, long &lx, double &dy)
{
	// 2.1 자료일자 Packet을 구한다.
	IPacket *pIPacket = m_pIDefaultPacketManager->GetPacket( _MTEXT(C0_DATE_TIME));
	if( !pIPacket) return R_ERROR;

	try
	{
		double dTime;
		ILPCSTR is = pIPacket->GetType();
		CString s = is;
		if(s == "YYYYMMDD")
		{
			m_nDateType = 0;
		}
		else if(s == "HHMMSS")
		{
			m_nDateType = 1;
		}

		if( !pIPacket->GetData( ix, dTime)) throw this;
		lx = ( long)dTime;
	}
	catch( CChartPeriodSummaryAddInImp *)
	{
		pIPacket->Release();
		return R_ERROR;
	}

	pIPacket->Release();

	return R_OK;
}
*/

int CChartPeriodSummaryAddInImp::GetPointFromIndex( const char *p_szRQ, int x, int y, CPoint &ptIndex)
{
//	if(m_bCentered)	// 중심선에 정렬 
//		ptIndex.x = (x*(m_rctBlockRegion.Width()+1) -m_nStartIndex*( m_rctBlockRegion.Width()+1) + (m_rctBlockRegion.Width())/2.)/
//					( m_nEndIndex - m_nStartIndex + 1) + m_rctBlockRegion.left+2;
//	else
//		ptIndex.x = (x*m_rctBlockRegion.Width() - m_nStartIndex*m_rctBlockRegion.Width())/
//					( m_nEndIndex - m_nStartIndex + 1) + m_rctBlockRegion.left-1;

	ptIndex.x = m_pIChartManager->GetXPosition( p_szRQ, x, y);
	return R_OK;
}

// 2011.01.06 by SYS >> 분차트 YYYY(년) 추가 <<
//int CChartPeriodSummaryAddInImp::GetIndexFromValue( const char *p_szRQ, int p_nEndIndex, long lx, double dy, int &ix, int &iy)
int CChartPeriodSummaryAddInImp::GetIndexFromValue( const char *p_szRQ, int p_nEndIndex, __int64 lx, double dy, int &ix, int &iy)
{
	IPacketManager *pIPacketManager = m_pIPacketListManager->GetPacketManager( p_szRQ);
	if( !pIPacketManager) return R_ERROR;

	//IPacket *pIPacket = m_pIDefaultPacketManager->GetPacket( "자료일자");
	IPacket *pIPacket = pIPacketManager->GetPacket( _MTEXT(C0_DATE_TIME));
	pIPacketManager->Release();

	try
	{
		double dTime;
		for( iy = 0; iy <= p_nEndIndex; iy++)
		{
			if( !pIPacket->GetData( iy, dTime)) throw this;
			
			// 2011.01.06 by SYS >> 분차트 YYYY(년) 추가
			//if(lx == ( long)dTime)	// 있으면...
			if(lx == ( __int64)dTime)	// 있으면...
			// 2011.01.06 by SYS <<
			{
				ix = iy;
				pIPacket->Release();
				iy++;
				return R_OK;
			}
			// 2011.01.06 by SYS >> 분차트 YYYY(년) 추가
			//else if(lx < ( long)dTime)
			else if(lx < ( __int64)dTime)
			// 2011.01.06 by SYS <<
			{
				//iy++;
				pIPacket->Release();
				return R_ERROR;
			}
		}
		
	}
	catch( CChartPeriodSummaryAddInImp *)
	{
		pIPacket->Release();
		return R_ERROR;
	}

	pIPacket->Release();
	return R_ERROR; 
}

//////////////////////////////////////////////////////////////////////////
// 가격 얻기, 계산
int CChartPeriodSummaryAddInImp::GetPriceInfo( const char *p_szRQ, CPeriodObj::CPrice *pPriceInfo, int nSIndex, int nEIndex)
{
	IPacketManager *pIPacketManager = m_pIPacketListManager->GetPacketManager( p_szRQ);
	if( !pIPacketManager) return R_ERROR;

	// 정보 얻기
	IPacket *pIPacketForMax = pIPacketManager->GetPacket( _MTEXT(C0_HIGH));
	IPacket *pIPacketForMin = pIPacketManager->GetPacket( _MTEXT(C0_LOW));
	IPacket *pIPacketForCur = pIPacketManager->GetPacket( _MTEXT(C0_CLOSE));
	pIPacketManager->Release();
	
	if( !pIPacketForMax || !pIPacketForMin || !pIPacketForCur)
	{
		if( pIPacketForMax) pIPacketForMax->Release();
		if( pIPacketForMin) pIPacketForMin->Release();
		if( pIPacketForCur) pIPacketForCur->Release();
		return R_ERROR;
	}

	pPriceInfo->avg = 0;
	
	try
	{
		double dMax=0, dMin=0, dCur=0;

		// 초기값 설정
		if( !pIPacketForCur->GetData( nSIndex, pPriceInfo->sPrice)) throw this;	// 시작일 종가
		if( !pIPacketForCur->GetData( nEIndex, pPriceInfo->ePrice)) throw this;	// 종료일 종가
		if( !pIPacketForMax->GetData( nSIndex, pPriceInfo->max)) throw this;	// 고가
		if( !pIPacketForMin->GetData( nSIndex, pPriceInfo->min)) throw this;	// 저가

		if(nSIndex>nEIndex)
		{
			int nTemp = nEIndex;
			nEIndex = nSIndex;
			nSIndex = nTemp;
		}
		for(int i=nSIndex; i<=nEIndex;i++)
		{
			// 최대가 계산
			if( !pIPacketForMax->GetData( i, dMax) || !pIPacketForMin->GetData( i, dMin) || !pIPacketForCur->GetData( i, dCur))
			{
				pIPacketForMax->Release();
				pIPacketForMin->Release();
				pIPacketForCur->Release();
				return R_ERROR;
			}
			pPriceInfo->max = max(pPriceInfo->max, dMax);
			// 최소가 계산
			pPriceInfo->min = min(pPriceInfo->min, dMin);
			// 평균가 계산
			pPriceInfo->avg += dCur/(nEIndex-nSIndex+1);
		}		
	}
	catch( CChartPeriodSummaryAddInImp *)
	{
		pIPacketForMax->Release();
		pIPacketForMin->Release();
		pIPacketForCur->Release();
		return R_ERROR;
	}

	pIPacketForMax->Release();
	pIPacketForMin->Release();
	pIPacketForCur->Release();
	return R_OK;

}

//////////////////////////////////////////////////////////////////////////
// 봉 정보 얻기, 계산
int CChartPeriodSummaryAddInImp::GetCandleInfo( const char *p_szRQ, CPeriodObj::CCandle *pCandleInfo, int nSIndex, int nEIndex)
{
	if(nSIndex>nEIndex)
	{
		int nTemp = nEIndex;
		nEIndex = nSIndex;
		nSIndex = nTemp;
	}

	IPacketManager *pIPacketManager = m_pIPacketListManager->GetPacketManager( p_szRQ);
	if( !pIPacketManager) return R_ERROR;
	// 정보 얻기
	IPacket *pIPacketForRef = pIPacketManager->GetPacket( _MTEXT(C0_OPEN));
	IPacket *pIPacketForCur = pIPacketManager->GetPacket( _MTEXT(C0_CLOSE));
	pIPacketManager->Release();

	if( !pIPacketForRef || !pIPacketForCur)
	{
		if( pIPacketForRef) pIPacketForRef->Release();
		if( pIPacketForCur) pIPacketForCur->Release();
		return R_ERROR;
	}

	try
	{
		double dRef=0, dCur=0;
		pCandleInfo->minus = 0;
		pCandleInfo->plus = 0;
		pCandleInfo->count = abs(nEIndex - nSIndex) + 1;
		for(int i=nSIndex; i<=nEIndex;i++)
		{
			// 봉수 계산
			if( !pIPacketForRef->GetData( i, dRef) || !pIPacketForCur->GetData( i, dCur))
			{
				pIPacketForRef->Release();
				pIPacketForCur->Release();
				return R_ERROR;
			}
			if(dCur < dRef)	// 음봉
				pCandleInfo->minus++;
			else
				pCandleInfo->plus++;				
		}

		pCandleInfo->CalcRatio();
		
	}
	catch( CChartPeriodSummaryAddInImp *)
	{
		pIPacketForRef->Release();
		pIPacketForCur->Release();
		return R_ERROR;
	}


	pIPacketForRef->Release();
	pIPacketForCur->Release();
	return R_OK;
}

//////////////////////////////////////////////////////////////////////////
// 거래량 얻기, 계산
int CChartPeriodSummaryAddInImp::GetAmountInfo( const char *p_szRQ, CPeriodObj::CAmount *pAmountInfo, int nSIndex, int nEIndex)
{
	if(nSIndex>nEIndex)
	{
		int nTemp = nEIndex;
		nEIndex = nSIndex;
		nSIndex = nTemp;
	}

	IPacketManager *pIPacketManager = m_pIPacketListManager->GetPacketManager( p_szRQ);
	if( !pIPacketManager) return R_ERROR;

	LPCSTR szItemCode = NULL, szItemName = NULL;
	m_pIPacketListManager->GetItemCode( p_szRQ, szItemCode, szItemName);
	if( szItemCode) if( '|' == *( const char *)szItemCode)
	{
		pIPacketManager->Release();
		return R_OK;
	}

	// 정보 얻기
	IPacket *pIPacketForAmount = pIPacketManager->GetPacket( _MTEXT(C0_VOLUME));
	if( !pIPacketForAmount)
		pIPacketForAmount = pIPacketManager->GetPacket( _MTEXT(C0_ACCUMULATED_VOLUME));

	pIPacketManager->Release();
	if( !pIPacketForAmount) return R_ERROR;

	pAmountInfo->avg = 0;

	try
	{
		double dAmount=0;
		if( !pIPacketForAmount->GetData( nSIndex, pAmountInfo->max) || !pIPacketForAmount->GetData( nSIndex, pAmountInfo->min))
		{
			pIPacketForAmount->Release();
			return R_ERROR;
		}

		for(int i=nSIndex; i<=nEIndex;i++)
		{
			// min max 계산
			if( !pIPacketForAmount->GetData( i, dAmount))
			{
				pIPacketForAmount->Release();
				return R_ERROR;
			}
			pAmountInfo->max = max(pAmountInfo->max, dAmount);
			pAmountInfo->min = min(pAmountInfo->min, dAmount);
			pAmountInfo->avg += dAmount/(nEIndex-nSIndex+1);
		}
	}
	catch( CChartPeriodSummaryAddInImp *)
	{
		pIPacketForAmount->Release();
		return R_ERROR;
	}

	pIPacketForAmount->Release();
	return R_OK;
}

void CChartPeriodSummaryAddInImp::OnPSConfig(CPeriodObj* pLine)
{
	//100125-alzioyes.
	if(!m_pCurSelectedLine) return;

	CPSConfigDlg dlg;

	m_pCurSelectedLine = pLine;

	//데이터 표시 플래그
	dlg.m_SPrice = m_pCurSelectedLine->m_Price.sPrice;
	dlg.m_EPrice = m_pCurSelectedLine->m_Price.ePrice;
	dlg.m_bSEPrice = m_pCurSelectedLine->m_bSEPrice;
	dlg.m_bEEPrice = m_pCurSelectedLine->m_bEEPrice;
	dlg.m_bHPrice = m_pCurSelectedLine->m_bHPrice;
	dlg.m_bLPrice = m_pCurSelectedLine->m_bLPrice;
	dlg.m_bMPrice = m_pCurSelectedLine->m_bMPrice;
	dlg.m_bHAmount = m_pCurSelectedLine->m_bHAmount;
	dlg.m_bLAmount = m_pCurSelectedLine->m_bLAmount;
	dlg.m_bMAmount = m_pCurSelectedLine->m_bMAmount;
	dlg.m_bPCandle = m_pCurSelectedLine->m_bPCNum;
	dlg.m_bMCandle = m_pCurSelectedLine->m_bMCNum;
	dlg.m_bPMRatio = m_pCurSelectedLine->m_bPMRatio;

	// 일자
	dlg.m_lSDate = m_pCurSelectedLine->m_lSDate;
	dlg.m_lEDate = m_pCurSelectedLine->m_lEDate;

	// 클릭 포인트 값
	dlg.m_SPrice = m_pIChartOCX->ConvertYPositionToData(m_pCurSelectedLine->m_ptAStart.y, m_pCurSelectedLine->m_rctGraphRegion.top, m_pCurSelectedLine->m_rctGraphRegion.bottom, m_pCurSelectedLine->m_dViewMax, m_pCurSelectedLine->m_dViewMin, m_pCurSelectedLine->m_bLog, m_pCurSelectedLine->m_bReverse);
	dlg.m_EPrice = m_pIChartOCX->ConvertYPositionToData(m_pCurSelectedLine->m_ptAEnd.y, m_pCurSelectedLine->m_rctGraphRegion.top, m_pCurSelectedLine->m_rctGraphRegion.bottom, m_pCurSelectedLine->m_dViewMax, m_pCurSelectedLine->m_dViewMin, m_pCurSelectedLine->m_bLog, m_pCurSelectedLine->m_bReverse);

	// 선 색
	dlg.m_clrLineColor = m_pCurSelectedLine->m_clrLineColor;
	// 선 굵기
	dlg.m_nLineThickness = m_pCurSelectedLine->m_nLineThickness-1;
	dlg.m_nLineStyle = m_pCurSelectedLine->m_nLineStyle;
	dlg.m_nDateType = m_pCurSelectedLine->m_nDateType;

	dlg.m_nTextAlignment = m_pCurSelectedLine->m_nTextAlignment;
	if(dlg.DoModal() == IDOK)
	{
		if(dlg.m_bAllChange)
		{
			POSITION pos = m_LineList.GetHeadPosition();
			while (pos)
			{
				CPeriodObj* pLine = (CPeriodObj*)m_LineList.GetNext(pos);
				pLine->m_nRowCnt		= dlg.m_nCnt + 1;
				pLine->m_bSEPrice		= dlg.m_bSEPrice;
				pLine->m_bEEPrice		= dlg.m_bEEPrice;
				pLine->m_bHPrice		= dlg.m_bHPrice;
				pLine->m_bLPrice		= dlg.m_bLPrice;
				pLine->m_bMPrice		= dlg.m_bMPrice;
				pLine->m_bHAmount		= dlg.m_bHAmount;
				pLine->m_bLAmount		= dlg.m_bLAmount;
				pLine->m_bMAmount		= dlg.m_bMAmount;
				pLine->m_bPCNum			= dlg.m_bPCandle;
				pLine->m_bMCNum			= dlg.m_bMCandle;
				pLine->m_bPMRatio		= dlg.m_bPMRatio;
				pLine->m_nLineThickness	= dlg.m_nLineThickness+1;
				pLine->m_nLineStyle		= dlg.m_nLineStyle;
				pLine->m_lSDate			= dlg.m_lSDate;
				pLine->m_lEDate			= dlg.m_lEDate;

				pLine->m_clrLineColor	= dlg.m_clrLineColor;
				pLine->m_nTextAlignment	= dlg.m_nTextAlignment;
			}
		}
		else
		{
			m_pCurSelectedLine->m_nRowCnt			= dlg.m_nCnt + 1;
			m_pCurSelectedLine->m_bSEPrice			= dlg.m_bSEPrice;
			m_pCurSelectedLine->m_bEEPrice			= dlg.m_bEEPrice;
			m_pCurSelectedLine->m_bHPrice			= dlg.m_bHPrice;
			m_pCurSelectedLine->m_bLPrice			= dlg.m_bLPrice;
			m_pCurSelectedLine->m_bMPrice			= dlg.m_bMPrice;
			m_pCurSelectedLine->m_bHAmount			= dlg.m_bHAmount;
			m_pCurSelectedLine->m_bLAmount			= dlg.m_bLAmount;
			m_pCurSelectedLine->m_bMAmount			= dlg.m_bMAmount;
			m_pCurSelectedLine->m_bPCNum			= dlg.m_bPCandle;
			m_pCurSelectedLine->m_bMCNum			= dlg.m_bMCandle;
			m_pCurSelectedLine->m_bPMRatio			= dlg.m_bPMRatio;
			m_pCurSelectedLine->m_nLineThickness	= dlg.m_nLineThickness+1;
			m_pCurSelectedLine->m_nLineStyle		= dlg.m_nLineStyle;
			m_pCurSelectedLine->m_lSDate			= dlg.m_lSDate;
			m_pCurSelectedLine->m_lEDate			= dlg.m_lEDate;
			m_pCurSelectedLine->m_clrLineColor		= dlg.m_clrLineColor;
			m_pCurSelectedLine->m_nTextAlignment	= dlg.m_nTextAlignment;
		}

		// 위치 변경 (X)
		int iDummy=0;
		GetIndexFromValue( m_pCurSelectedLine->m_strRQ, m_pCurSelectedLine->m_nEndIndexOfView, dlg.m_lSDate, 0, m_pCurSelectedLine->m_nStartIndex, iDummy);
		GetPointFromIndex( m_pCurSelectedLine->m_strRQ, m_pCurSelectedLine->m_nStartIndex, 0,m_pCurSelectedLine->m_ptAStart);
		GetIndexFromValue( m_pCurSelectedLine->m_strRQ, m_pCurSelectedLine->m_nEndIndexOfView, dlg.m_lEDate, 0, m_pCurSelectedLine->m_nEndIndex, iDummy);
		GetPointFromIndex( m_pCurSelectedLine->m_strRQ, m_pCurSelectedLine->m_nEndIndex, 0,m_pCurSelectedLine->m_ptAEnd);
		// 위치 변경(Y)
		m_pCurSelectedLine->m_ptAStart.y = m_pIChartOCX->ConvertDataToYPosition(dlg.m_SPrice, m_pCurSelectedLine->m_dViewMax, m_pCurSelectedLine->m_dViewMin, m_pCurSelectedLine->m_rctGraphRegion.top, m_pCurSelectedLine->m_rctGraphRegion.bottom, m_pCurSelectedLine->m_bLog, m_pCurSelectedLine->m_bReverse);
		m_pCurSelectedLine->m_ptAEnd.y = m_pIChartOCX->ConvertDataToYPosition(dlg.m_EPrice, m_pCurSelectedLine->m_dViewMax, m_pCurSelectedLine->m_dViewMin, m_pCurSelectedLine->m_rctGraphRegion.top, m_pCurSelectedLine->m_rctGraphRegion.bottom, m_pCurSelectedLine->m_bLog, m_pCurSelectedLine->m_bReverse);
		m_pIChartOCX->InvalidateControl();
	}
}

void CChartPeriodSummaryAddInImp::OnDeleteLine()
{
		UnregPS((long)m_pCurSelectedLine);

		POSITION psn = m_LineList.Find( m_pCurSelectedLine);
		if( psn) m_LineList.RemoveAt( psn);
		
		delete m_pCurSelectedLine;
		m_pCurSelectedLine = NULL;

		m_pIChartOCX->InvalidateControl();
}

void CChartPeriodSummaryAddInImp::OnReleaseDraw()
{
	m_nCommandType = 2;
	if( m_pIChartOCX != NULL )
		m_pIChartOCX->OnToolStateChanged( CToolOptionInfo::T_TOOL_PERIOD_SUMMARY, FALSE );	
}

//void CChartPeriodSummaryAddInImp::MakeInfo()
//{
//
//}

int CChartPeriodSummaryAddInImp::GetDateType( CPeriodObj *p_pLine)
{
	if( !p_pLine) return FALSE;

	IPacketManager *pIPacketManager = m_pIPacketListManager->GetPacketManager( p_pLine->m_strRQ);
	if( !pIPacketManager) return FALSE;
	IPacket *pIPacket = pIPacketManager->GetPacket( _MTEXT(C0_DATE_TIME));
	pIPacketManager->Release();
	if( !pIPacket) return FALSE;

	try
	{
//		double dTime;
		ILPCSTR is = pIPacket->GetType();
		CString s = is;

		if(s == "YYYYMMDD")
		{
			p_pLine->m_nDateType = 0;
		}
		else if(s == "MMDDHHMM")
		{
			p_pLine->m_nDateType = 1;
		}
		else if(s == "HHMMSS")
		{
			p_pLine->m_nDateType = 2;
		}
		else if(s == "DDHHMMSS")
		{
			p_pLine->m_nDateType = 3;
		}
		else if(s == "YYYYMM")
		{
			p_pLine->m_nDateType = 4;
		}
		// 2011.01.24 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
		else if(s == "YYYYMMDDHHMMSS")
		{
			// nDataUnit (0:nontype, 1:일, 2:주, 3:월, 4:분, 5:틱, 6:Text)
			int nDateUnit = pIPacket->GetDateUnitType();
			switch(nDateUnit)
			{
			case 1:	// 일
			case 2:	// 주 
					// _T("YYYYMMDD"); 타입
					p_pLine->m_nDateType = 0;
					break;
			case 3:	// 월 
					// _T("YYYYMM"); 타입
					p_pLine->m_nDateType = 4;
					break;
			case 4:	// 분
			case 6:
					// _T("MMDDHHMM"); 타입
					p_pLine->m_nDateType = 1;
					break;
			case 5:	// 틱 
					// _T("DDHHMMSS"); 타입
					p_pLine->m_nDateType = 3;
					break;
			}
		}
		// 2011.01.24 by SYS <<
	}
	catch( CChartPeriodSummaryAddInImp *)
	{
		pIPacket->Release();
		return R_ERROR;
	}

	pIPacket->Release();

	return R_OK;
}

RECT CChartPeriodSummaryAddInImp::DrawCursor( HDC p_hDC)
{
	CRect rct;
	rct.SetRectEmpty();

	ML_SET_LANGUAGE_RES();

	// 1. DC의 Handle을 구한다.
	CDC *pDC = CDC::FromHandle( p_hDC);
	if( !pDC) return rct;

	if( m_pIAddInToolMgr->IsLButtonExclusive( m_pIAddInDllBase))
	{
		if(!m_pCurSelectedLine) return rct;
		CRgn rgn;
		rgn.CreateRectRgn( ( int)m_pCurSelectedLine->m_rctGraphRegion.left,
							( int)m_pCurSelectedLine->m_rctGraphRegion.top,
							( int)m_pCurSelectedLine->m_rctGraphRegion.right,
							( int)m_pCurSelectedLine->m_rctGraphRegion.bottom);
		pDC->SelectClipRgn(&rgn);

		m_pCurSelectedLine->SetPos(m_ptCursor, m_pCurSelectedLine->m_CurClickedPosition);

		//////////////////////////////////////////////////////////////////////////
		// 시작점과 종료점이 교차할 때
		if(m_pCurSelectedLine->m_ptAStart.x > m_pCurSelectedLine->m_ptAEnd.x)	// 위치 역전
		{
			CPoint ptTemp;
			if(m_pCurSelectedLine->m_CurClickedPosition == E_PT)
			{
				m_pCurSelectedLine->m_CurClickedPosition = S_PT;
			}
			else if(m_pCurSelectedLine->m_CurClickedPosition == S_PT)
			{
				m_pCurSelectedLine->m_CurClickedPosition = E_PT;
			}
			ptTemp = m_pCurSelectedLine->m_ptAStart;
			m_pCurSelectedLine->m_ptAStart = m_pCurSelectedLine->m_ptAEnd;
			m_pCurSelectedLine->m_ptAEnd = ptTemp;

		}
	
		int ix, iy;
		double dDummy = 0;
//		m_pCurSelectedLine->m_nDateType = m_nDateType;
		// 시작일 및 종가
		if(GetIndexFromPoint( m_pCurSelectedLine->m_strRQ, m_pCurSelectedLine->m_ptAStart, ix, iy) == R_ERROR) goto Drawing;
		if( IsValidIndex( m_pCurSelectedLine->m_strRQ, ix))
			m_pCurSelectedLine->m_nStartIndex = ix;
 		//if(GetValueFromIndex(ix, iy, m_pCurSelectedLine->m_lSDate, dDummy) == R_ERROR) goto Drawing;
		if(GetData_X(m_pCurSelectedLine->m_ptAStart, m_pCurSelectedLine->m_lSDate) == R_ERROR) goto Drawing;
		// 끝일 및 종가
		if(GetIndexFromPoint( m_pCurSelectedLine->m_strRQ, m_pCurSelectedLine->m_ptAEnd, ix, iy) == R_ERROR) goto Drawing;
		if( IsValidIndex( m_pCurSelectedLine->m_strRQ, ix))
			m_pCurSelectedLine->m_nEndIndex = ix;
 		//if(GetValueFromIndex(ix, iy, m_pCurSelectedLine->m_lEDate, dDummy) == R_ERROR) goto Drawing;
		//m_pCurSelectedLine->m_lEDate = GetData_X(m_pCurSelectedLine->m_ptAEnd);
		if(GetData_X(m_pCurSelectedLine->m_ptAEnd, m_pCurSelectedLine->m_lEDate) == R_ERROR) goto Drawing;
		if(m_bNewDraw == FALSE)
		{
			// 중간점 위치
			if(GetIndexFromPoint( m_pCurSelectedLine->m_strRQ, m_pCurSelectedLine->m_ptAt, ix, iy) == R_ERROR) goto Drawing;
			if( !IsValidIndex( m_pCurSelectedLine->m_strRQ, ix)) goto Drawing;
		}
		// 가격, 봉, 거래량 정보
		if(GetPriceInfo( m_pCurSelectedLine->m_strRQ, &m_pCurSelectedLine->m_Price, m_pCurSelectedLine->m_nStartIndex, m_pCurSelectedLine->m_nEndIndex) == R_ERROR) return rct;
		if(GetCandleInfo( m_pCurSelectedLine->m_strRQ, &m_pCurSelectedLine->m_Candle, m_pCurSelectedLine->m_nStartIndex, m_pCurSelectedLine->m_nEndIndex) == R_ERROR) return rct;
		if(GetAmountInfo( m_pCurSelectedLine->m_strRQ, &m_pCurSelectedLine->m_Amount, m_pCurSelectedLine->m_nStartIndex, m_pCurSelectedLine->m_nEndIndex) == R_ERROR) return rct;

		// 클릭 포인트 값
		m_pCurSelectedLine->m_dSClicked = m_pIChartOCX->ConvertYPositionToData(m_pCurSelectedLine->m_ptAStart.y, m_pCurSelectedLine->m_rctGraphRegion.top, m_pCurSelectedLine->m_rctGraphRegion.bottom, m_pCurSelectedLine->m_dViewMax, m_pCurSelectedLine->m_dViewMin, m_pCurSelectedLine->m_bLog, m_pCurSelectedLine->m_bReverse);
		m_pCurSelectedLine->m_dEClicked = m_pIChartOCX->ConvertYPositionToData(m_pCurSelectedLine->m_ptAEnd.y, m_pCurSelectedLine->m_rctGraphRegion.top, m_pCurSelectedLine->m_rctGraphRegion.bottom, m_pCurSelectedLine->m_dViewMax, m_pCurSelectedLine->m_dViewMin, m_pCurSelectedLine->m_bLog, m_pCurSelectedLine->m_bReverse);

Drawing:
		m_pCurSelectedLine->Draw(pDC, m_hOcxWnd);
		//pDC->SelectClipRgn(&rgn, NULLREGION);
		pDC->SelectClipRgn(NULL);
		rgn.DeleteObject();
	}
	
///////////////////////////////////////////////////
	return rct;
}

/*
BOOL CChartPeriodSummaryAddInImp::GetSpanTime( CPoint ptPoint )
{
	// 1. 현재 화면에 보이는 Data의 Start/End Index를 구한다.
	if( !m_pIChartOCX->GetViewStartEndIndex( m_nStartIndex, m_nEndIndex ))
		return FALSE;

	// 2. Mouse Point의 Data Index를 구한다.
	int nDataOnPage = m_nEndIndex - m_nStartIndex + 1;
	double dBarWidth = double( m_rctGraphRegion.Width() ) / double( nDataOnPage );
	int nDataIndex = m_nStartIndex + int( double( ptPoint.x - m_rctBlockRegion.left ) / dBarWidth );

	// 3. 현재 Mouse Point를 이용하여 각 Span의 Index를 구한다.
	m_nPrevSpanIndex = nDataIndex - m_nPrevSpan;
	m_nAfterSpanIndex = nDataIndex + m_nAfterSpan;

	// 4. 각 Span의 기준 Point를 계산한다.
	int nBarLeftEdge, nBarRightEdge, nX;
	int nDataOffset = m_nPrevSpanIndex - m_nStartIndex;
	nBarLeftEdge = m_rctGraphRegion.left + int( dBarWidth * double( nDataOffset) + 0.5) + 1;
	nBarRightEdge = m_rctGraphRegion.left + int( dBarWidth * double( nDataOffset + 1) + 0.5) - 1;
	nX = ( nBarLeftEdge + nBarRightEdge) / 2;

	m_ptPrevSpan.x = nX;
	m_ptPrevSpan.y = ptPoint.y;

	nDataOffset = m_nAfterSpanIndex - m_nStartIndex;
	nBarLeftEdge = m_rctGraphRegion.left + int( dBarWidth * double( nDataOffset) + 0.5) + 1;
	nBarRightEdge = m_rctGraphRegion.left + int( dBarWidth * double( nDataOffset + 1) + 0.5) - 1;
	nX = ( nBarLeftEdge + nBarRightEdge) / 2;

	m_ptAfterSpan.x = nX;
	m_ptAfterSpan.y = ptPoint.y;

	// 5. Block에서 XScale 구분자를 얻어온다.
	int nR, nC;
	if( !m_pIChartManager->GetBlockIndex( ptPoint, nR, nC))
		return FALSE;
	IBlock *pIBlock = m_pIChartManager->GetBlock( nR, nC);
	if( !pIBlock)
		return FALSE;

	pIBlock->Release();
	return TRUE;
}
*/

void CChartPeriodSummaryAddInImp::OnCaptureChanged( HWND p_hWnd, const HBITMAP p_hbmpMainOSB)
{
	m_bMouseCaptured = FALSE;
}

/*
BOOL CChartPeriodSummaryAddInImp::GetStringfromDataIdx( const char *p_szRQ, int nDataIdx, CString &strSpan )
{
	strSpan = "00/00/00";
	IPacketManager *pIPacketManager = m_pIPacketListManager->GetPacketManager( p_szRQ);
	if( !pIPacketManager) return FALSE;
	// 1. 자료일자 Packet을 구한다.
	IPacket *pIPacket = pIPacketManager->GetPacket( _MTEXT(C0_DATE_TIME));
	pIPacketManager->Release();
	if( !pIPacket) return FALSE;

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
	for( int i = nPacketTypeLength - szMarkTime.GetLength() ; i ; i-- )
		szMarkTime = "0" + szMarkTime;

	char strChar = strPacketType.GetAt(0);
	strSpan = szMarkTime.GetAt(0);

	// 5. XScale에 사용되는 구분자 의 Type을 구분한다.
	CString strCompartment = m_strDateCompartment;
	BOOL bUseOneCharDateCompartment = ( m_strDateCompartment.GetLength() > 1 ) ? FALSE : TRUE;
	BOOL bUseOneCharTimeCompartment = ( m_strTimeCompartment.GetLength() > 1 ) ? FALSE : TRUE;
	
	
	// "월"과 "분"의 PacketType이 같이 'M'을 사용하기 때문에 구분해준다.
	BOOL bIsMonth = FALSE;				
	CString strPacketChar = "";
	
	CString strYear, strMonth, strDay, strHour, strMinute, strSecond;
	strYear.LoadString(IDS_STRING_YEAR);			// 년
	strMonth.LoadString(IDS_STRING_MONTH);			// 월
	strDay.LoadString(IDS_STRING_DAY);				// 일
	strHour.LoadString(IDS_STRING_HOUR);			// 시
	strMinute.LoadString(IDS_STRING_MINUTE);		// 분
	strSecond.LoadString(IDS_STRING_SECOND);		// 초
	
	// 6. PacketType의 길이만큼 문자열을 parsing한다.
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
					strCompartment = strYear;
					bIsMonth = TRUE;
				}
				
				else if( strChar == 'M' && strPacketChar == 'D' )		strCompartment = strMonth;
				else if( strChar == 'D' && strPacketChar == 'H' )		strCompartment = strDay;
			}

			// 6.1.2 Time의 구분자가 "시 분 초"를 사용할 경우 
			if( !bUseOneCharTimeCompartment )
			{
				if( strChar == 'H' && strPacketChar == 'M' )
				{
					strCompartment = strHour;
					bIsMonth = FALSE;
				}
				
				else if( strChar == 'M' && strPacketChar == 'S' )		strCompartment = strMinute;
				else if( strChar == 'S' && strPacketChar == 'N' )		strCompartment = strSecond;
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
			if( nPacketTypeLength == ( nIdx + 1 ) )
			{
				if( strChar == 'Y' && !bUseOneCharDateCompartment )
					strSpan = strSpan + strYear;
				
				else if( strChar == 'M' && !bUseOneCharDateCompartment && bIsMonth )
					strSpan = strSpan + strMonth;
				
				else if( strChar == 'D' && !bUseOneCharDateCompartment )
					strSpan = strSpan + strDay;
				
				else if( strChar == 'H' && !bUseOneCharTimeCompartment )
					strSpan = strSpan + strHour;
				
				else if( strChar == 'M' && !bUseOneCharTimeCompartment && !bIsMonth)
					strSpan = strSpan + strMinute;
				
				else if( strChar == 'S' && !bUseOneCharTimeCompartment )
					strSpan = strSpan + strSecond;
			}
		}
	}

	pIPacket->Release();
	return TRUE;
}
*/

void CChartPeriodSummaryAddInImp::RegPS(long pLine)
{
	if( m_pIAddInToolMgr != NULL )
	{
		m_pIAddInToolMgr->OnAddInToolCommand( EAI_TOOLORDERMGR_ADD_TOOL_TO_ORDERMANAGER,	
											  m_strAddInItemName,		
											  pLine );	
	}
}

void CChartPeriodSummaryAddInImp::UnregPS(long pLine)
{
	if( m_pIAddInToolMgr != NULL )
	{
		m_pIAddInToolMgr->OnAddInToolCommand( EAI_TOOLORDERMGR_DELETE_TOOL_FROM_ORDERMANAGER,	
											  m_strAddInItemName,		
											  pLine );	
	}
}

void CChartPeriodSummaryAddInImp::OnLoadEnvironment( IPropertyMap *p_pIAddInEnvMap)	
{
	if(p_pIAddInEnvMap->GetCount() <= 0)
		return;

	const char *chToolLoading;
	p_pIAddInEnvMap->Lookup(m_strAddInItemName,chToolLoading);
	m_strToolLoading.Format("%s",chToolLoading);
	if(m_strToolLoading.IsEmpty() == TRUE)
		return;

	POSITION pos = m_LineList.GetHeadPosition();
	while (pos)
	{
		CPeriodObj* pLine = (CPeriodObj*)m_LineList.GetNext(pos);
		delete pLine;
		pLine = NULL;
	}
	m_LineList.RemoveAll();

	// 연속그리기 해제
	CString strLineInfo, strLineInfoPrev;
	int iStart = 0;

	while(iStart >= 0)
	{
		strLineInfo = TokenizeEx(m_strToolLoading, ";", iStart);
		if(iStart == -1)	// No data
			break;
		strLineInfo += ",";

		if( strLineInfoPrev == strLineInfo )
			continue;

		CPeriodObj* pLine;
		pLine = new CPeriodObj;
		int iStart2 = 0;
		pLine->m_bSaved = TRUE;
		// 20081009 박동관 >>
		pLine->SetChartPeriodSummaryAddInImp(this);
		// 20081009 박동관 <<
		pLine->m_nDateType = atoi(TokenizeEx(strLineInfo, ",", iStart2));
		pLine->m_nUpDown = atoi(TokenizeEx(strLineInfo, ",", iStart2));
		pLine->m_lSDate = _atoi64(TokenizeEx(strLineInfo, ",", iStart2));
		pLine->m_lEDate = _atoi64(TokenizeEx(strLineInfo, ",", iStart2));
		pLine->m_Price.sPrice = atof(TokenizeEx(strLineInfo, ",", iStart2));
		pLine->m_Price.ePrice = atof(TokenizeEx(strLineInfo, ",", iStart2));
		pLine->m_nLineThickness = atoi(TokenizeEx(strLineInfo, ",", iStart2));
		pLine->m_nLineStyle = atoi(TokenizeEx(strLineInfo, ",", iStart2));
		pLine->m_bSEPrice = atoi(TokenizeEx(strLineInfo, ",", iStart2));			// 시작일 종가
		pLine->m_bEEPrice = atoi(TokenizeEx(strLineInfo, ",", iStart2));			// 종료일 종가
		pLine->m_bHPrice = atoi(TokenizeEx(strLineInfo, ",", iStart2));				// 고가
		pLine->m_bLPrice = atoi(TokenizeEx(strLineInfo, ",", iStart2));				// 저가
		pLine->m_bMPrice = atoi(TokenizeEx(strLineInfo, ",", iStart2));				// 평균가
		pLine->m_bHAmount = atoi(TokenizeEx(strLineInfo, ",", iStart2));			//최대 거래량
		pLine->m_bLAmount = atoi(TokenizeEx(strLineInfo, ",", iStart2));			// 최소 거래량
		pLine->m_bMAmount = atoi(TokenizeEx(strLineInfo, ",", iStart2));			// 평균 거래량
		pLine->m_bPCNum = atoi(TokenizeEx(strLineInfo, ",", iStart2));				//양봉 수
		pLine->m_bMCNum = atoi(TokenizeEx(strLineInfo, ",", iStart2));				// 음봉 수
		pLine->m_bPMRatio = atoi(TokenizeEx(strLineInfo, ",", iStart2));			//양음봉 비율
		pLine->m_dSClicked = atof(TokenizeEx(strLineInfo, ",", iStart2));
		pLine->m_dEClicked = atof(TokenizeEx(strLineInfo, ",", iStart2));
		pLine->m_clrLineColor = atol(TokenizeEx(strLineInfo, ",", iStart2));
		pLine->m_nRowCnt = atoi(TokenizeEx(strLineInfo, ",", iStart2)); 
		pLine->m_nTextAlignment = atoi(TokenizeEx(strLineInfo, ",", iStart2)); 
		// 20081007 JS.Kim	사용자시간대 선택
		pLine->m_nTimeDiff = atoi(TokenizeEx(strLineInfo, ",", iStart2));	
		// (2009/10/15 - Seung-Won, Bae) Save and Load the Block Position.
		pLine->m_nRow = atoi(TokenizeEx(strLineInfo, ",", iStart2));
		pLine->m_nCol = atoi(TokenizeEx(strLineInfo, ",", iStart2));
	
		m_LineList.AddTail(pLine);

		strLineInfoPrev = strLineInfo;
	}

	//m_pIChartOCX->InvalidateControl();
	if(m_nCommandType == 0)
		m_nCommandType = 2;
//	m_pIChartOCX->OnToolStateChanged( CToolOptionInfo::T_TOOL_PERIOD_SUMMARY, FALSE );
}

void CChartPeriodSummaryAddInImp::OnSaveEnvironment( IPropertyMap *p_pIAddInEnvMap)	
{
	CString strSaveToolList = "";// = m_pAnalysisTool->GetSavedElementsData();
	CString strTemp;

	POSITION pos = m_LineList.GetHeadPosition();
	while(pos)
	{
		CPeriodObj* pLine = (CPeriodObj*)m_LineList.GetNext(pos);


		strTemp.Format(_T("%d,%d,%s,%s,%f,%f,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%f,%f,%ld,%d,%d,%d,%d,%d;")
						, pLine->m_nDateType
						, pLine->m_nUpDown				
						, DoubleToString((double)pLine->m_lSDate)
						, DoubleToString((double)pLine->m_lEDate)
						, pLine->m_Price.sPrice
						, pLine->m_Price.ePrice
						, pLine->m_nLineThickness
						, pLine->m_nLineStyle
						, pLine->m_bSEPrice			// 시작일 종가
						, pLine->m_bEEPrice			// 종료일 종가
						, pLine->m_bHPrice				// 고가
						, pLine->m_bLPrice				// 저가
						, pLine->m_bMPrice				// 평균가
						, pLine->m_bHAmount			//최대 거래량
						, pLine->m_bLAmount			// 최소 거래량
						, pLine->m_bMAmount			// 평균 거래량
						, pLine->m_bPCNum				//양봉 수
						, pLine->m_bMCNum				// 음봉 수
						, pLine->m_bPMRatio			//양음봉 비율
						, pLine->m_dSClicked
						, pLine->m_dEClicked
						, pLine->m_clrLineColor
						, pLine->m_nRowCnt
						, pLine->m_nTextAlignment
						, pLine->m_nTimeDiff		// 20081007 JS.Kim	사용자시간대 선택
						, pLine->m_nRow				// (2009/10/15 - Seung-Won, Bae) Save and Load the Block Position.
						, pLine->m_nCol);
	strSaveToolList += strTemp;

	}
	p_pIAddInEnvMap->SetAt(m_strAddInItemName,strSaveToolList);
}

CString CChartPeriodSummaryAddInImp::TokenizeEx(CString strSrc/*IN*/, const char* pszTokens/*IN*/, int& iStart/*IN, OUT*/)
{
	CString strRes = _T("");
	int iCur = iStart;
	iStart = strSrc.Find(pszTokens, iCur);
	if(iStart != -1)
	{
		strRes = strSrc.Mid(iCur, iStart-iCur);
		iStart++;
		return strRes;
	}
	return strRes;
}

BOOL CChartPeriodSummaryAddInImp::IsValidIndex( const char *p_szRQ, int nIndex)
{
	IPacketManager *pIPacketManager = m_pIPacketListManager->GetPacketManager( p_szRQ);
	if( !pIPacketManager) return FALSE;
	IPacket *pIPacketForCur = pIPacketManager->GetPacket( _MTEXT(C0_CLOSE));
	pIPacketManager->Release();
	if( !pIPacketForCur) return FALSE;
	try
	{
		double dCur=0;
		if( !pIPacketForCur->GetData( nIndex, dCur)) throw this;	// 시작일 종가
	}
	catch( CChartPeriodSummaryAddInImp *)
	{
		pIPacketForCur->Release();
		return FALSE;
	}
	pIPacketForCur->Release();
	return TRUE;
}

// protected ==================================================================
// XScale : 관련 Interface로 사용 - vntsorl(20070521)
// 2011.01.06 by SYS >> 분차트 YYYY(년) 추가 <<
//BOOL CChartPeriodSummaryAddInImp::GetData_X(const CPoint& point, long& lx)
BOOL CChartPeriodSummaryAddInImp::GetData_X(const CPoint& point, __int64& lx)
{
	int nBlockColumn, nCenter_x;
	int nRow, nCol;
	m_pIChartManager->GetBlockIndex(point, nRow, nCol);
	if(nRow<0 || nCol<0)
		return R_ERROR;
	else
		// 2011.01.06 by LYH >> 분차트 YYYY(년) 추가
		//lx = ( long)m_pIChartManager->GetDoubleFromX(nRow, nCol, point.x, &nBlockColumn, nCenter_x);
		lx = ( __int64)m_pIChartManager->GetDoubleFromX(nRow, nCol, point.x, &nBlockColumn, nCenter_x);
		// 2011.01.06 by LYH <<
	return R_OK;
}

// 20081007 JS.Kim	사용자시간대 선택
// Graph의 시간대가 변경되면 기간요약선의 시간도 바꿔준다.
bool CChartPeriodSummaryAddInImp::RecalcLineXPoint(const char* szName, int nTimeDiff)
{
//	CString strRQ = m_pIChartManager->GetCurrentRQ();
//	IPacket* pPacket= m_pIDefaultPacketManager->GetDatePacket(strRQ);
	IPacket *pPacket = m_pIDefaultPacketManager->GetPacket( _MTEXT( C0_DATE_TIME));
	if( !pPacket )		return false;

	ILPCSTR is = pPacket->GetType();
	CString strType = is;

	int	nBaseTimeDiff;
	if( strlen(szName) == 0 )	nBaseTimeDiff = pPacket->GetOrgBaseTimeDifference();		// 원래시간으로 되돌린 경우
	else						nBaseTimeDiff = nTimeDiff;

	double dXDate, oldXDate;
	int oldTimeDiff;

	CPeriodObj *pLine = NULL;
	POSITION pos = m_LineList.GetHeadPosition();
	while( pos)
	{
		if( !(pLine = (CPeriodObj *)m_LineList.GetNext( pos)) )	break;

		oldTimeDiff = pLine->m_nTimeDiff;
		if( oldTimeDiff != nBaseTimeDiff )
		{

			for( int i=0; i<2; i++ )
			{
				if( i == 0 )		oldXDate = (double)pLine->m_lSDate;
				else				oldXDate = (double)pLine->m_lEDate;

				dXDate = CDataConversion::CalcTimeDiff(oldXDate, nBaseTimeDiff - oldTimeDiff, pPacket->GetOrgBaseTimeDifference(), strType);

				if( i == 0 )		pLine->m_lSDate = (long)dXDate;
				else				pLine->m_lEDate = (long)dXDate;
			}
			pLine->m_nTimeDiff = nBaseTimeDiff;
		}
	}
	if( pPacket) pPacket->Release();

	return true;
}

// 2011.01.06 by SYS >> 분차트 YYYY(년) 추가
CString CChartPeriodSummaryAddInImp::DoubleToString(const double& Num)
{
	//assert(Num != DBL_MAX);
	//assert(Num != -DBL_MAX);
	
	CString strData;
	strData.Format(_T("%f"), Num);
	strData.TrimRight("0");
	if(strData.Right(1) == '.')
		strData.TrimRight(".");
	
	return strData;
}
// 2011.01.06 by SYS <<
