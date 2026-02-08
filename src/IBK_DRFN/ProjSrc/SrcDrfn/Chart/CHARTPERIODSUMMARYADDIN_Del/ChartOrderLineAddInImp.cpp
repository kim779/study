// ChartOrderLineAddInImp.cpp: implementation of the CChartOrderLineAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartPeriodSummaryAddIn.h"
#include "ChartOrderLineAddInImp.h"
#include "OrderLineConfigDlg.h"
#include "OrderLineManDlg.h"
#include "./Include_AddIn/Include_AddIn.h"							// for Ixxxxxx.h
#include "./Include_Chart/MessageDefine.h"			// for UM_CURSOR_EXCHANGE
#include "DlgConfirm.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//#define IDC_HAND MAKEINTRESOURCE(32649)

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
CString CChartOrderLineAddInImp::m_strAddInItemName = "ORDER_LINE";
extern HINSTANCE g_hInstance;


UINT THREAD_PROCEEDSIGN(LPVOID lpVoid)
{
	CLineObj* pLine = (CLineObj*)lpVoid;
	if(pLine)
	{
		AFX_MANAGE_STATE( AfxGetStaticModuleState());
		CDlgConfirm	dlg;	
		if(pLine->m_nState == STAT_UP)
		{
			if(pLine->m_bUpBuy)
				dlg.SetData(pLine->m_strName, pLine->m_nState, 2);
			else if(pLine->m_bUpSell)
				dlg.SetData(pLine->m_strName, pLine->m_nState, 1);
		}
		else
		{
			if(pLine->m_bDownBuy)
				dlg.SetData(pLine->m_strName, pLine->m_nState, 2);
			else if(pLine->m_bDownSell)
				dlg.SetData(pLine->m_strName, pLine->m_nState, 1);
		}
			
		int nRet = dlg.DoModal();
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CChartOrderLineAddInImp::CChartOrderLineAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase)
	: CChartAddInBase( p_pIChartOCX, p_pIAddInDllBase)
{
	m_bCentered = TRUE;
	m_nCommandType = 0;
	m_pCurSelectedLine = NULL;
}

CChartOrderLineAddInImp::~CChartOrderLineAddInImp()
{
	// 주문선 메모리에서 삭제
	POSITION pos = m_LineList.GetHeadPosition();
	while (pos)
	{
		CLineObj* pLine = (CLineObj*)m_LineList.GetNext(pos);
		delete pLine;
	}
	m_LineList.RemoveAll();
}

//////////////////////////////////////////////////////////////////////
// Define AddIn Event Map
//////////////////////////////////////////////////////////////////////
BEGIN_ADDIN_EVENT_MAP( CChartOrderLineAddInImp)
	ONADDINEVENT(OnDrawAfterGraph)	
	ONADDINEVENT(OnPacketData)
	ONADDINEVENT(OnMouseMove)
	ONADDINEVENT(OnLButtonDown)
	ONADDINEVENT(OnLButtonUp)
	ONADDINEVENT(OnRButtonDown)
	ONADDINEVENT(OnRButtonUp)
	ONADDINEVENT(OnLButtonDblClk)
	ONADDINEVENT(OnCmdMsg)
	ONADDINEVENT(OnKeyDown)
	ONADDINEVENT(OnResetChart)
	ONADDINEVENT(DrawCursor)
	ONADDINEVENT(OnLoadEnvironment)
	ONADDINEVENT(OnSaveEnvironment)
	ONADDINEVENT(OnUpdatePacketData)
	ONADDINEVENT(OnAddInToolCommand_EAICommandType_long_long)
	ONADDINEVENT( OnAddInToolCommand_EAICommandType_char_long)
	ONADDINEVENT(OnRealData)
	// 복수종목별 Real반영 : 복수종목 - ojtaso (20070131)
	ONADDINEVENT( OnUpdatePacketDataMultiItem)
	ONADDINEVENT( OnPacketDataMultiItem)
END_ADDIN_EVENT_MAP()


const LPSTR szDateType[] =
{
	 "일","일","주","월","분","틱"
};

//////////////////////////////////////////////////////////////////////
// Overrides
//////////////////////////////////////////////////////////////////////
void CChartOrderLineAddInImp::OnRealData(const char *p_szPacketName, double p_dData)
{
// 	POSITION pos = m_LineList.GetHeadPosition();
// 	while (pos)
// 	{
// 		CLineObj* pLine = (CLineObj*)m_LineList.GetNext(pos);
// 		if(pLine->m_bUpBuy || pLine->m_bDownBuy)	// 메수 이벤트
// 		{
// 			m_pIChartOCX->FireAddInEvent("ChartPeriodSummaryAddIn.dll:ORDER_LINE", 1, "종목코드", 0, 1/*0:매도, 1:매수*/);
// 		}
// 		else if(pLine->m_bUpSell || pLine->m_bDownSell)	// 매도 이벤트
// 		{
// 			m_pIChartOCX->FireAddInEvent("ChartPeriodSummaryAddIn.dll:ORDER_LINE", 1, "종목코드", 0, 0/*0:매도, 1:매수*/);
// 		}
// 	}

}

long CChartOrderLineAddInImp::UPOrDown(CLineObj* pLine, double dData)
{
	long lSellOrBuy = 0;
	CPoint ptStand;
	CPoint ptReal;
	CPoint ptEnd;

	IPacket *pIPacket = m_pIPacketManager->GetPacket( "종가");
	if( !pIPacket) return R_ERROR;

	try
	{
		double dLastData;
		int nEndIndex = pIPacket->GetDataCount()-1;
		if( !pIPacket->GetData(nEndIndex , dLastData)) throw this;
		GetPointFromIndex(nEndIndex, nEndIndex, ptStand);
		ptStand.y = m_pIChartOCX->ConvertDataToYPosition(dLastData, m_dViewMax, m_dViewMin,m_rctGraphRegion.top, m_rctGraphRegion.bottom, m_bLog, m_bReverse);

		// 라인 상의 값 얻기
		ptEnd = ptStand;
		ptEnd.y = GetYPositionOnLine((int&)ptEnd.x, pLine->m_ptAStart,pLine->m_ptAEnd);

		// 리얼의 위치
		ptReal = ptStand;
		ptReal.y = m_pIChartOCX->ConvertDataToYPosition(dData, m_dViewMax, m_dViewMin,m_rctGraphRegion.top, m_rctGraphRegion.bottom, m_bLog, m_bReverse);
		


	}
	catch( CChartOrderLineAddInImp *)
	{
		pIPacket->Release();
		return 0;
	}

	pIPacket->Release();
	return lSellOrBuy;
}

void CChartOrderLineAddInImp::OnPacketData(const char *p_szPacketNames, BOOL p_bWithReset)
{
//	AFX_MANAGE_STATE( AfxGetStaticModuleState());
//
//	// 종목코드, 종목명 얻기
//	ILPCSTR p_pszMessage;
//	m_pIPacketManager->GetHelpMessageItem("CODEANDNAME",p_pszMessage);
//
//	CString strCodeInfo = p_pszMessage;
//	strCodeInfo += ":";
//	int iStart = 0;
//	m_strCode = TokenizeEx(strCodeInfo, ":",iStart);		// 종목코드
//	m_strCodeName = TokenizeEx(strCodeInfo, ":",iStart);	// 종목명
//
//	m_nDateType = m_pIPacketManager->GetBaseTimeDataType();
//	m_strDateType = szDateType[m_nDateType];
//
//
//	LoadOrderLine();	// 파일 저장된 주문선 로드	

}

void CChartOrderLineAddInImp::OnUpdatePacketData( BOOL p_bTrUpdate)
{
//	if(p_bTrUpdate) return;
//
//	IPacket *pIPacketForCur = m_pIPacketManager->GetPacket( "종가");
//	if( !pIPacketForCur) return;
//
//	try
//	{
//		int nUPDOWN = 0;
//		CPoint ptPrev, ptCur;
//		double dCur=0, dPrev = 0, dCurOnLine=0, dPrevOnLine = 0;
//		int nIndex = pIPacketForCur->GetDataCount()-1;
//
//		if( !pIPacketForCur->GetData( nIndex-1, dPrev)) throw this;	
//		if( !pIPacketForCur->GetData( nIndex, dCur)) throw this;	
//
//		POSITION pos = m_LineList.GetHeadPosition();
//		while (pos)
//		{
//			CLineObj* pLine = (CLineObj*)m_LineList.GetNext(pos);
//			int iDummy = 0;
//			// Previous
//			GetPointFromIndex(nIndex-1, iDummy, ptPrev);
//			ptPrev.y = GetYPositionOnLine((int&)ptPrev.x, pLine->m_ptAStart,pLine->m_ptAEnd);
//			dPrevOnLine = m_pIChartOCX->ConvertYPositionToData(ptPrev.y, m_rctGraphDrwingRegion.top, m_rctGraphDrwingRegion.bottom, m_dViewMax, m_dViewMin, m_bLog, m_bReverse);
//
//			// Current
//			GetPointFromIndex(nIndex, iDummy, ptCur);
//			ptCur.y = GetYPositionOnLine((int&)ptCur.x, pLine->m_ptAStart,pLine->m_ptAEnd);
//			dCurOnLine = m_pIChartOCX->ConvertYPositionToData(ptCur.y, m_rctGraphDrwingRegion.top, m_rctGraphDrwingRegion.bottom, m_dViewMax, m_dViewMin, m_bLog, m_bReverse);
////			if((dPrevOnLine < dPrev) && (dCurOnLine > dCur))	// 상향 돌파
////				nUPDOWN = UP;
////			else if((dPrevOnLine > dPrev) && (dCurOnLine < dCur))	// 하향 돌파
////				nUPDOWN = DOWN;
//
//			if((dPrevOnLine > dPrev) && (dCurOnLine < dCur))	// 상향 돌파
//				nUPDOWN = UP;
//			else if((dPrevOnLine < dPrev) && (dCurOnLine > dCur))	// 하향 돌파
//				nUPDOWN = DOWN;
//
//			if(pLine->m_nState == STAT_DOWN && nUPDOWN != DOWN && dCur > pLine->m_dAlertPrice)
//				pLine->m_nState = STAT_NONE;
//
//			if(pLine->m_nState == STAT_UP && nUPDOWN != UP && dCur < pLine->m_dAlertPrice)
//				pLine->m_nState = STAT_NONE;
//
//			//if(( (nUPDOWN == UP) && pLine->m_bUpBuy) || ((nUPDOWN == DOWN) && pLine->m_bDownBuy))	// 매수 이벤트
//			if(nUPDOWN == UP)	//상승시
//			{
//				//AfxMessageBox("매수");
//				if(pLine->m_nState == STAT_NONE)
//				{
//					pLine->m_dAlertPrice =  dCur;
//					pLine->m_nState = STAT_UP;
//					AfxBeginThread(THREAD_PROCEEDSIGN, pLine);
//					if(pLine->m_bUpBuy)
//						m_pIChartOCX->FireAddInEvent("ChartPeriodSummaryAddIn.dll:ORDER_LINE", 1, m_strCode, 0, 1/*0:매도, 1:매수*/);
//					else if(pLine->m_bUpSell)
//						m_pIChartOCX->FireAddInEvent("ChartPeriodSummaryAddIn.dll:ORDER_LINE", 1, m_strCode, 0, 0/*0:매도, 1:매수*/);
//				}
//			}
//			//else if(( (nUPDOWN == UP) && pLine->m_bUpSell) || ((nUPDOWN == DOWN) && pLine->m_bDownSell))	// 매도 이벤트
//			else if(nUPDOWN == DOWN)	//상승시
//			{
//				//AfxMessageBox("매도");
//				if(pLine->m_nState == STAT_NONE)
//				{
//					pLine->m_dAlertPrice =  dCur;
//					pLine->m_nState = STAT_DOWN;
//					AfxBeginThread(THREAD_PROCEEDSIGN, pLine);
//					if(pLine->m_bDownBuy)
//						m_pIChartOCX->FireAddInEvent("ChartPeriodSummaryAddIn.dll:ORDER_LINE", 1, m_strCode, 0, 1/*0:매도, 1:매수*/);
//					else if(pLine->m_bDownSell)
//						m_pIChartOCX->FireAddInEvent("ChartPeriodSummaryAddIn.dll:ORDER_LINE", 1, m_strCode, 0, 0/*0:매도, 1:매수*/);
//				}
//				
//			}
//			
//		}
//	}
//	catch( CChartOrderLineAddInImp *)
//	{
//		pIPacketForCur->Release();
//		return;
//	}
//	pIPacketForCur->Release();
	
	// 외부 이벤트 발생
	//m_pIChartOCX->FireAddInEvent("ChartPeriodSummaryAddIn.dll:PERIOD_SUMMARY", 1, 0,0,m_pCurNewsObj->m_lxData);
}

void CChartOrderLineAddInImp::OnPacketDataMultiItem(LPCTSTR lpszRQ, const char *p_szPacketNames, BOOL p_bWithReset)
{
	AFX_MANAGE_STATE( AfxGetStaticModuleState());

	if(strcmp(lpszRQ, "DEFAULT")) return;
	// 종목코드, 종목명 얻기
//	ILPCSTR p_pszMessage;
//	m_pIPacketManager->GetHelpMessageItem("CODEANDNAME",p_pszMessage);
//
//	CString strCodeInfo = p_pszMessage;
//	strCodeInfo += ":";
//	int iStart = 0;
//	m_strCode = TokenizeEx(strCodeInfo, ":",iStart);		// 종목코드
//	m_strCodeName = TokenizeEx(strCodeInfo, ":",iStart);	// 종목명
//
//	m_nDateType = m_pIPacketManager->GetBaseTimeDataType();
//	m_strDateType = szDateType[m_nDateType];


	LoadOrderLine();	// 파일 저장된 주문선 로드	
}

void CChartOrderLineAddInImp::OnUpdatePacketDataMultiItem(LPCTSTR lpszRQ, BOOL p_bTrUpdate)
{
	if(p_bTrUpdate) return;
	if(strcmp(lpszRQ, "DEFAULT")) return;

	IPacket *pIPacketForCur = m_pIPacketManager->GetPacket( "종가");
	if( !pIPacketForCur) return;

	try
	{
		int nUPDOWN = 0;
		CPoint ptPrev, ptCur;
		double dCur=0, dPrev = 0, dCurOnLine=0, dPrevOnLine = 0;
		int nIndex = pIPacketForCur->GetDataCount()-1;

		if( !pIPacketForCur->GetData( nIndex-1, dPrev)) throw this;	
		if( !pIPacketForCur->GetData( nIndex, dCur)) throw this;	

		POSITION pos = m_LineList.GetHeadPosition();
		while (pos)
		{
			CLineObj* pLine = (CLineObj*)m_LineList.GetNext(pos);
			int iDummy = 0;
			// Previous
			GetPointFromIndex(nIndex-1, iDummy, ptPrev);
			ptPrev.y = GetYPositionOnLine((int&)ptPrev.x, pLine->m_ptAStart,pLine->m_ptAEnd);
			dPrevOnLine = m_pIChartOCX->ConvertYPositionToData(ptPrev.y, m_rctGraphDrwingRegion.top, m_rctGraphDrwingRegion.bottom, m_dViewMax, m_dViewMin, m_bLog, m_bReverse);

			// Current
			GetPointFromIndex(nIndex, iDummy, ptCur);
			ptCur.y = GetYPositionOnLine((int&)ptCur.x, pLine->m_ptAStart,pLine->m_ptAEnd);
			dCurOnLine = m_pIChartOCX->ConvertYPositionToData(ptCur.y, m_rctGraphDrwingRegion.top, m_rctGraphDrwingRegion.bottom, m_dViewMax, m_dViewMin, m_bLog, m_bReverse);
//			if((dPrevOnLine < dPrev) && (dCurOnLine > dCur))	// 상향 돌파
//				nUPDOWN = UP;
//			else if((dPrevOnLine > dPrev) && (dCurOnLine < dCur))	// 하향 돌파
//				nUPDOWN = DOWN;

			if((dPrevOnLine > dPrev) && (dCurOnLine < dCur))	// 상향 돌파
				nUPDOWN = UP;
			else if((dPrevOnLine < dPrev) && (dCurOnLine > dCur))	// 하향 돌파
				nUPDOWN = DOWN;

			if(pLine->m_nState == STAT_DOWN && nUPDOWN != DOWN && dCur > pLine->m_dAlertPrice)
				pLine->m_nState = STAT_NONE;

			if(pLine->m_nState == STAT_UP && nUPDOWN != UP && dCur < pLine->m_dAlertPrice)
				pLine->m_nState = STAT_NONE;

			//if(( (nUPDOWN == UP) && pLine->m_bUpBuy) || ((nUPDOWN == DOWN) && pLine->m_bDownBuy))	// 매수 이벤트
			if(nUPDOWN == UP)	//상승시
			{
				//AfxMessageBox("매수");
				if(pLine->m_nState == STAT_NONE)
				{
					pLine->m_dAlertPrice =  dCur;
					pLine->m_nState = STAT_UP;
					if(pLine->m_bUpBuy || pLine->m_bUpSell)
						AfxBeginThread(THREAD_PROCEEDSIGN, pLine);
					if(pLine->m_bUpBuy)
						m_pIChartOCX->FireAddInEvent("ChartPeriodSummaryAddIn.dll:ORDER_LINE", 1, m_strCode, 0, 1/*0:매도, 1:매수*/);
					else if(pLine->m_bUpSell)
						m_pIChartOCX->FireAddInEvent("ChartPeriodSummaryAddIn.dll:ORDER_LINE", 1, m_strCode, 0, 0/*0:매도, 1:매수*/);
				}
			}
			//else if(( (nUPDOWN == UP) && pLine->m_bUpSell) || ((nUPDOWN == DOWN) && pLine->m_bDownSell))	// 매도 이벤트
			else if(nUPDOWN == DOWN)	//상승시
			{
				//AfxMessageBox("매도");
				if(pLine->m_nState == STAT_NONE)
				{
					pLine->m_dAlertPrice =  dCur;
					pLine->m_nState = STAT_DOWN;
					if(pLine->m_bDownBuy || pLine->m_bDownSell)
						AfxBeginThread(THREAD_PROCEEDSIGN, pLine);
					if(pLine->m_bDownBuy)
						m_pIChartOCX->FireAddInEvent("ChartPeriodSummaryAddIn.dll:ORDER_LINE", 1, m_strCode, 0, 1/*0:매도, 1:매수*/);
					else if(pLine->m_bDownSell)
						m_pIChartOCX->FireAddInEvent("ChartPeriodSummaryAddIn.dll:ORDER_LINE", 1, m_strCode, 0, 0/*0:매도, 1:매수*/);
				}
				
			}
			
		}
	}
	catch( CChartOrderLineAddInImp *)
	{
		pIPacketForCur->Release();
		return;
	}
	pIPacketForCur->Release();
	
	// 외부 이벤트 발생
	//m_pIChartOCX->FireAddInEvent("ChartPeriodSummaryAddIn.dll:PERIOD_SUMMARY", 1, 0,0,m_pCurNewsObj->m_lxData);
}

BOOL CChartOrderLineAddInImp::OnAddInToolCommand(const EAICommandType p_eCommandID, const long p_lParam1, const long p_lParam2)
{
	if(p_eCommandID != EAI_OOA_ON_TOOL_FUNCTION) return FALSE;

	CToolOptionInfo::TOOLOPTION eToolOption = (CToolOptionInfo::TOOLOPTION)p_lParam1;
	
	if( eToolOption == CToolOptionInfo::T_DELETE_ALL )
	{
		POSITION pos = m_LineList.GetHeadPosition();
//		POSITION prevpos, pos_save;
		while (pos)
		{
			//prevpos = pos;
			CLineObj* pLine = (CLineObj*)m_LineList.GetNext(pos);

			// 해당 주문선 찾기
			//BOOL bExistLine = FALSE;
			//pos_save = m_AllOrderLines.GetHeadPosition();
			//while (pos_save)
			//{
			//	if(m_AllOrderLines.GetNext(pos_save).Find(pLine->m_strSerial) >= 0)
			//	{
			//		bExistLine = TRUE;
			//		break;
			//	}
			//}
			//if(!bExistLine)
			//{
				delete pLine;
				pLine = NULL;
			//	m_LineList.RemoveAt(prevpos);
			//}
		}
		m_LineList.RemoveAll();
	}
	else if(eToolOption == CToolOptionInfo::T_ORDER_LINE)// 주문선
	{
		if(!p_lParam2) return FALSE;
		if(p_lParam2 != 3)
			m_nCommandType = p_lParam2;

		m_nDateType = m_pIPacketManager->GetBaseTimeDataType();
		m_strDateType = szDateType[m_nDateType];

		switch(m_nCommandType)
		{
		case 1:			// draw mode
			{
				m_nCommandType = p_lParam2;
				int		nR, nC ;
				if( !GetChartBasicData(m_nStartIndex, m_nEndIndex, nR, nC, m_rctBlockRegion, m_rctGraphRegion))
					return FALSE;

				m_pIChartOCX->OnToolStateChanged( CToolOptionInfo::T_ORDER_LINE, TRUE );	// T_ORDER_LINE 으로 대체 필요
			}
			break;
		case 2:			// modify mode
			break;
		}
		return TRUE;
	}
	else if(eToolOption == CToolOptionInfo::T_ORDER_LINE_MANAGER)// 주문선 관리
	{
//		m_nCommandType = 2;
		OnOLMan();
		return FALSE;
	}

	else if(eToolOption == CToolOptionInfo::T_TOOL_SELECTION)
	{
		if(p_lParam2 == CToolOptionInfo::T_ORDER_LINE) return FALSE;	// T_ORDER_LINE 으로 대체 필요
//>> [QC No]090728HMC016
//		m_nCommandType = 2;
		if(m_pCurSelectedLine)	m_nCommandType = 2;
//<< [QC No]090728HMC016
		m_pIChartOCX->OnToolStateChanged( CToolOptionInfo::T_ORDER_LINE, FALSE );// T_ORDER_LINE 으로 대체 필요
		if(m_pCurSelectedLine)
		{
			m_pCurSelectedLine->m_bSelected = FALSE;
			m_pCurSelectedLine = NULL;
			m_pIChartOCX->InvalidateControl();
		}
		return FALSE;
	}
	else if(eToolOption == CToolOptionInfo::T_ALL_TOOL_TRUE_QUERY)
	{
		if( (m_pIChartOCX != NULL) && (m_nCommandType == 1))
			m_pIChartOCX->OnToolStateChanged( CToolOptionInfo::T_ORDER_LINE, TRUE );	// T_ORDER_LINE 으로 대체 필요
	}
	
	else if(eToolOption == CToolOptionInfo::T_ALL_TOOL_RESET_STATE)
	{
		m_nCommandType = 2;
	}
	else
	{
		if(p_lParam2 == CToolOptionInfo::T_ORDER_LINE) return FALSE;	// T_ORDER_LINE 으로 대체 필요
//>> [QC No]090728HMC016
//		m_nCommandType = 2;
		if(m_pCurSelectedLine)	m_nCommandType = 2;
//<< [QC No]090728HMC016
		m_pIChartOCX->OnToolStateChanged( CToolOptionInfo::T_ORDER_LINE, FALSE );// T_ORDER_LINE 으로 대체 필요
		if(m_pCurSelectedLine)
		{
			m_pCurSelectedLine->m_bSelected = FALSE;
			m_pCurSelectedLine = NULL;
			m_pIChartOCX->InvalidateControl();
		}
		return FALSE;
	}

	return FALSE;
}

BOOL CChartOrderLineAddInImp::OnAddInToolCommand(	const EAICommandType p_eCommandID, const char *p_szParam, const long p_lParam)
{
	if(p_eCommandID == EAI_TOOLORDERMGR_DELETE_THIS_TOOL)
	{
		if(strcmp(p_szParam, "ORDER_LINE")) return FALSE;
		CLineObj * pLine = (CLineObj *)p_lParam;
		if(!pLine) return FALSE;
		POSITION pos = m_LineList.Find(pLine);
		m_LineList.RemoveAt(pos);
		delete pLine;
		pLine = NULL;
		m_pIChartOCX->InvalidateControl();
	}

	return FALSE;
}

void CChartOrderLineAddInImp::OnResetChart()
{
	AFX_MANAGE_STATE( AfxGetStaticModuleState());

	//////////////////////////////////////////////////////////////////////////
	// 기존 데이터 클리어
	POSITION pos = m_LineList.GetHeadPosition();
	while (pos)
	{
		CLineObj* pLine = (CLineObj*)m_LineList.GetNext(pos);
		delete pLine;
		pLine = NULL;
	}
	m_LineList.RemoveAll();
	//////////////////////////////////////////////////////////////////////////

	m_nCommandType = 0;

	if( m_pIChartOCX != NULL )
		m_pIChartOCX->OnToolStateChanged( CToolOptionInfo::T_ORDER_LINE, FALSE );	

	LoadOrderLine();	// 파일 저장된 주문선 로드

}

BOOL CChartOrderLineAddInImp::OnMouseMove(HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	AFX_MANAGE_STATE( AfxGetStaticModuleState());
	if(!m_nCommandType) return FALSE;

	CPoint pt = point;

	if(!m_pCurSelectedLine) return FALSE;
	if( p_dwMouseActionID == m_dwMouseActionID)
	{
		int ix, is, ie;
		if(m_rctGraphRegion.PtInRect(point))
		{
			m_pCurSelectedLine->SetPos(pt, m_pCurSelectedLine->m_CurClickedPosition);
			if(		(GetIndexFromPoint(pt, ix, ix) == R_OK) 
				&&	(GetIndexFromPoint(m_pCurSelectedLine->m_ptAStart, is, is) == R_OK)
				&&	(GetIndexFromPoint(m_pCurSelectedLine->m_ptAEnd, ie, ie) == R_OK) 
			  )
			{
				if(IsValidIndex(ix) && IsValidIndex(is) && IsValidIndex(ie))
					m_ptCursor = point;
			}
		}

		return TRUE;
	}
	else if(m_nCommandType != 1)
	{
		POSITION pos = m_LineList.GetHeadPosition();
		while(pos)
		{
			CLineObj* pLine = (CLineObj*)m_LineList.GetNext(pos);
			if( pLine->IsInRegion(point) )
			{
				return TRUE;
			}
		}
		return FALSE;
	}
	if( m_ptInit == point)	return FALSE;


	return FALSE;
}

BOOL CChartOrderLineAddInImp::OnLButtonDown(HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	AFX_MANAGE_STATE( AfxGetStaticModuleState());

	if(!m_nCommandType) return FALSE;

	if( !m_pIChartOCX->IsUseToolScrollBar() || m_pIChartOCX->IsOnSpecialChart() || m_pIChartManager->IsEmpty()) return FALSE;
	CRect rctRegion = m_pIChartManager->GetGraphRegionSumInColumn( point);
	if( rctRegion.IsRectEmpty()) return FALSE;
	
	m_ptInit = point;
	if(!m_rctBlockRegion.PtInRect(point)) return FALSE;

	m_dwMouseActionID = p_dwMouseActionID;
	BOOL bSpace=TRUE;

	POSITION pos = m_LineList.GetHeadPosition();
	while(pos)
	{
		CLineObj* pLine = (CLineObj*)m_LineList.GetNext(pos);
		if( pLine->IsInRegion(point)  && m_nCommandType == 2)	// modifying
		{			
			if( !m_pIAddInToolMgr->IsLButtonExclusive( m_pIAddInDllBase))
			{
				if( !m_pIAddInToolMgr->GetLButtonExclusive( m_pIAddInDllBase)) return FALSE;
				TRACE("\n\nOnButton Exclusive\n\n");
			}

			if(m_pCurSelectedLine)
				m_pCurSelectedLine->m_bSelected = FALSE;
			m_bNewDraw = FALSE;
			m_pCurSelectedLine = pLine;
			m_pCurSelectedLine->ButtonDownAt(point);
			POSITION pos2 = m_LineList.Find(m_pCurSelectedLine);
			m_LineList.RemoveAt(pos2);
			m_LineList.AddTail(m_pCurSelectedLine);
			m_pCurSelectedLine->m_bSelected = TRUE;
			m_ptCursor = point;
			TRACE("\n\nOnButton selected\n\n");
			if( m_pIChartOCX != NULL )
				m_pIChartOCX->InvalidateControl();	// DrawCursor()가 불리기 전에 선택한 선을 지우기 위해.... 
// Tool id가 정의 되면 주석 풀 것
// 			if( m_pIChartOCX != NULL )
// 			{
// 
// 				m_pIChartOCX->RunToolCommand( CToolOptionInfo::T_TOOL_SELECTION,	
// 													CToolOptionInfo::T_ORDER_LINE );	
// 			}

			bSpace = FALSE;
			return TRUE;
			break;
		}
		else
		{
			bSpace = TRUE;
		}
	}

	if(bSpace && m_nCommandType == 1)	// drawing
	{
		if( !m_pIAddInToolMgr->IsLButtonExclusive( m_pIAddInDllBase))
		{
			if( !m_pIAddInToolMgr->GetLButtonExclusive( m_pIAddInDllBase)) return FALSE;
		}
		m_ptCursor = point;
		m_bNewDraw = TRUE;
		CLineObj* pLine;
		pLine = new CLineObj;
 		pLine->m_Num = m_LineList.GetCount();
		pLine->m_nDateType = m_nDateType;

		// 초기화
		pLine->m_strCode = m_strCode;		// 종목 코드
		pLine->m_strName = m_strCodeName;	// 종목명
		pLine->m_strCandle = m_strDateType;	// 기준봉
		
// 		pLine->Initilize();

		pLine->GenerateSerial();
 		int ix, iy;
 		// 시작일 및 종가
 		CPoint ptAt = point;
 		if(GetIndexFromPoint(ptAt, ix, iy) == R_ERROR) return FALSE;
 		IPacket *pIPacketForCur = m_pIPacketManager->GetPacket( "종가");
 //		IPacket *pIPacketForRef = m_pIPacketManager->GetPacket( "시가");
		if(!pIPacketForCur) return FALSE;

		double dCur;
		if( !pIPacketForCur->GetData( iy, dCur))
		{
			pIPacketForCur->Release();
			return FALSE;
		}
		double dClickedValue = m_pIChartOCX->ConvertYPositionToData(ptAt.y, m_rctGraphDrwingRegion.top, m_rctGraphDrwingRegion.bottom, m_dViewMax, m_dViewMin, m_bLog, m_bReverse);
	//	pLine->SetUpDown(UP);

		pIPacketForCur->Release();

 		pLine->ButtonDownAt(ptAt);
		pLine->m_CurClickedPosition = E_PT;
 		pLine->SetPos(ptAt, E_PT);
		
		if(m_pCurSelectedLine)	m_pCurSelectedLine->m_bSelected = FALSE;	// 기존에 선택된 선이 있으면 선택 해제

		m_pCurSelectedLine = pLine;
		m_pCurSelectedLine->m_bSelected = TRUE;
		m_LineList.AddTail(pLine);
		RegPS((long)pLine);
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

BOOL CChartOrderLineAddInImp::OnLButtonUp(HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	AFX_MANAGE_STATE( AfxGetStaticModuleState());
	if(!m_nCommandType) return FALSE;
	
	if( !m_pIAddInToolMgr->ReleaseLButtonExclusive( m_pIAddInDllBase)) return FALSE;

	if(p_dwMouseActionID == m_dwMouseActionID)
	{
  		if(!m_pCurSelectedLine) return FALSE;
		if(m_bNewDraw)	// 새로 그리는 경우에는 끝점으로 설정
		{
			// 한 봉 간격에서의 주문선 셋팅은 무시처리
//			if(m_pCurSelectedLine->m_ptAStart != m_pCurSelectedLine->m_ptAEnd)
			if(m_pCurSelectedLine->m_nSIndex != m_pCurSelectedLine->m_nEIndex)
				OnOLConfig();
		}

		if(!m_pCurSelectedLine) return TRUE;
		if(m_pCurSelectedLine->m_nSIndex == m_pCurSelectedLine->m_nEIndex)
		{
			POSITION pos = m_LineList.Find(m_pCurSelectedLine);
			m_LineList.RemoveAt(pos);
			UnregPS((long)m_pCurSelectedLine);
			delete m_pCurSelectedLine;
			m_pCurSelectedLine = NULL;
		}

		// 화면 업데이트
		m_pIChartOCX->InvalidateControl();
		return TRUE;
	}	

	return FALSE;
}

BOOL CChartOrderLineAddInImp::OnRButtonDown(HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	AFX_MANAGE_STATE( AfxGetStaticModuleState());
	if(!m_nCommandType) return FALSE;

	switch(m_nCommandType)
	{
	case 0:	// stop
		return FALSE;
	case 1:	// draw
		return TRUE;
		break;
	case 2: // modify
		if(m_pCurSelectedLine)
			if(m_pCurSelectedLine->IsInRegion(point))
			{
				m_dwMouseActionID = p_dwMouseActionID;
				return TRUE;
			}
		POSITION tmpPos;
		POSITION pos = m_LineList.GetHeadPosition();
		while(pos)
		{
			tmpPos = pos;
			CLineObj* pLine = (CLineObj*)m_LineList.GetNext(pos);
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
		break;
	}
	return FALSE;
}

BOOL CChartOrderLineAddInImp::OnRButtonUp(HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	AFX_MANAGE_STATE( AfxGetStaticModuleState());
	if(!m_nCommandType) return FALSE;

	//////////////////////////////////////////////////////////////////////////
	// popup menu 생성
	CPoint clPoint(point);
	::ClientToScreen(m_pIChartOCX->GetOcxHwnd(),&clPoint);

	CMenu menu;
	VERIFY(menu.LoadMenu(IDR_MENUOL));

	switch(m_nCommandType)
	{
	case 1:	// draw mode
		m_hPopupMenu = GetSubMenu(menu.GetSafeHmenu(),0);
		break;
	case 2: // modify mode
		if(p_dwMouseActionID == m_dwMouseActionID)
		{
			m_hPopupMenu = GetSubMenu(menu.GetSafeHmenu(),1);
		}
		else
			return FALSE;
		break;
	}
	
	TrackPopupMenu(m_hPopupMenu, TPM_LEFTALIGN | TPM_RIGHTBUTTON, clPoint.x, clPoint.y,	0, m_pIChartOCX->GetOcxHwnd(), NULL);
	//////////////////////////////////////////////////////////////////////////
	return TRUE;
}


BOOL CChartOrderLineAddInImp::OnLButtonDblClk(HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	AFX_MANAGE_STATE( AfxGetStaticModuleState());
	if(!m_nCommandType) return FALSE;
	//OnOLMan();	// <-- 테스트용, 나중에 멀티차트 등과 연결되면 삭제
	POSITION pos = m_LineList.GetHeadPosition();
	while(pos)
	{
		CLineObj* pLine = (CLineObj*)m_LineList.GetNext(pos);
		if( pLine->IsInRegion(point) && m_nCommandType == 2)
		{
			//OnPSConfig(pLine);
			OnOLConfig();
			return TRUE;
		}
	}
	return FALSE;
	return FALSE;
}

BOOL CChartOrderLineAddInImp::OnCmdMsg(UINT nID, int nCode, ICmdUI *p_pICmdUI)
{
	AFX_MANAGE_STATE( AfxGetStaticModuleState());
	if(!m_nCommandType) return FALSE;
	if( CN_UPDATE_COMMAND_UI == nCode)
	{
		switch( nID)
		{
 		case ID_KSA_COMMON_RELEASE:
		case ID_KSA_ORDERLINE_CONFIG:
		case ID_KSA_ORDERLINE_DELETE:
		case ID_KSA_ORDERLINE_DELETEFROMFILE:
		case ID_KSA_ORDERLINE_EXECUTE:
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
		case ID_KSA_ORDERLINE_CONFIG://ID_KSA_PERIODSUMMARY_CONFIG:
			OnOLConfig();
			break;
		case ID_KSA_ORDERLINE_DELETE:	//화면에서 삭지
			OnDeleteLine2();
			break;
		case ID_KSA_ORDERLINE_DELETEFROMFILE:	// 파일&화면에서 삭제
			OnDeleteLine();
			break;
		case ID_KSA_ORDERLINE_EXECUTE://ID_KSA_PERIODSUMMARY_EXECUTE://
			OnAddInToolCommand( EAI_OOA_ON_TOOL_FUNCTION, CToolOptionInfo::T_ORDER_LINE, 1);
			break;
		case ID_KSA_ORDERLINE_RELEASE://ID_KSA_COMMON_RELEASE:
			OnReleaseDraw();
			break;
		default:
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CChartOrderLineAddInImp::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	AFX_MANAGE_STATE( AfxGetStaticModuleState());
	if(!m_nCommandType) return FALSE;
	
	if(nChar == 46)
	{
		if(!m_pCurSelectedLine)
			return FALSE;
		OnDeleteLine2();	// 메모리(화면)에서만 삭제
		return TRUE;
	}

	return FALSE;
}

void CChartOrderLineAddInImp::OnDrawAfterGraph(HDC p_hDC)
{
	AFX_MANAGE_STATE( AfxGetStaticModuleState());
	try
	{
		CDC * pDC = CDC::FromHandle(p_hDC);
		int		nR, nC ;
		CRect rcPrev = m_rctBlockRegion;
		if( !GetChartBasicData(m_nStartIndex, m_nEndIndex, nR, nC, m_rctBlockRegion, m_rctGraphRegion))
			throw this;

		CRgn rgn;
		rgn.CreateRectRgn((int)m_rctGraphRegion.left, (int)m_rctGraphRegion.top, (int)m_rctGraphRegion.right, (int)m_rctGraphRegion.bottom);
		pDC->SelectClipRgn(&rgn);

		POSITION pos = m_LineList.GetHeadPosition();
		while(pos)
		{
			CLineObj* pLine = (CLineObj*)m_LineList.GetNext(pos);

			int iDummy = 0;
			GetIndexFromValue(pLine->m_lSDate, pLine->m_dSData, pLine->m_nSIndex,iDummy);
			GetIndexFromValue(pLine->m_lEDate, pLine->m_dEData, pLine->m_nEIndex,iDummy);
			if((pLine == m_pCurSelectedLine) && (m_pIAddInToolMgr->IsLButtonExclusive( m_pIAddInDllBase))) 
			{
				TRACE("\n\n***************************Dawing.....\n\n");
				continue;
			}
			GetPointFromIndex(pLine->m_nSIndex,0,pLine->m_ptAIndexedStart);
			GetPointFromIndex(pLine->m_nEIndex,0,pLine->m_ptAIndexedEnd);

			pLine->m_ptAIndexedStart.y = m_pIChartOCX->ConvertDataToYPosition(pLine->m_dSData, m_dViewMax, m_dViewMin,m_rctGraphDrwingRegion.top, m_rctGraphDrwingRegion.bottom, m_bLog, m_bReverse);
			pLine->m_ptAIndexedEnd.y = m_pIChartOCX->ConvertDataToYPosition(pLine->m_dEData, m_dViewMax, m_dViewMin,m_rctGraphDrwingRegion.top, m_rctGraphDrwingRegion.bottom, m_bLog, m_bReverse);
			pLine->m_ptAStart = pLine->m_ptAIndexedStart;
			pLine->m_ptAEnd = pLine->m_ptAIndexedEnd;
			pLine->m_ptAExpand.x = m_rctBlockRegion.right;
			pLine->m_ptAExpand.y = GetYPositionOnLine((int&)pLine->m_ptAExpand.x, pLine->m_ptAIndexedStart, pLine->m_ptAIndexedEnd);
			pLine->Draw(pDC);
		}
		
		//pDC->SelectClipRgn(&rgn, NULLREGION);
		pDC->SelectClipRgn(NULL);
		rgn.DeleteObject();
	}
	catch (CChartOrderLineAddInImp* )
	{
		
	}		
}

BOOL CChartOrderLineAddInImp::InvokeAddInStr( LPCTSTR p_szCommandName, LPCTSTR p_szData)
{
	AFX_MANAGE_STATE( AfxGetStaticModuleState());
	
	int nCommandType = atoi(p_szCommandName);
	//m_nCommandType = atoi(p_szCommandName);
	if((!p_szData || p_szData == "") && nCommandType != 3) return FALSE;
	CString strData = p_szData;
	strData += ":";
	int iStart = 0;
	m_strCode = TokenizeEx(strData, ":",iStart);		// 종목코드
	m_strCodeName = TokenizeEx(strData, ":",iStart);	// 종목명
	m_nDateType = m_pIPacketManager->GetBaseTimeDataType();
	m_strDateType = szDateType[m_nDateType];

	switch(nCommandType)
	{
	case 0:	// stop
		break;
	case 1:	// draw
		{
			int		nR, nC ;
			if( !GetChartBasicData(m_nStartIndex, m_nEndIndex, nR, nC, m_rctBlockRegion, m_rctGraphRegion))
				return FALSE;
		}
		break;
	case 2:	// Config
		break;
	case 3:	// manage
		OnOLMan();	
		break;
	}

	return TRUE;
}


void CChartOrderLineAddInImp::OnLoadEnvironment( IPropertyMap *p_pIAddInEnvMap)
{
//	LoadOrderLine();
}
void CChartOrderLineAddInImp::OnSaveEnvironment( IPropertyMap *p_pIAddInEnvMap)
{

}

RECT CChartOrderLineAddInImp::DrawCursor( HDC p_hDC)
{
	CRect rct;
	rct.SetRectEmpty();

	AFX_MANAGE_STATE( AfxGetStaticModuleState());

	// 1. DC의 Handle을 구한다.
	CDC *pDC = CDC::FromHandle( p_hDC);
	if( !pDC) return rct;

	if( m_pIAddInToolMgr->IsLButtonExclusive( m_pIAddInDllBase))
	{
		CRgn rgn;
		rgn.CreateRectRgn((int)m_rctGraphRegion.left, (int)m_rctGraphRegion.top, (int)m_rctGraphRegion.right, (int)m_rctGraphRegion.bottom);
		pDC->SelectClipRgn(&rgn);


 		if(!m_pCurSelectedLine) return rct;

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
		m_pCurSelectedLine->m_nDateType = m_nDateType;
		// 시작일 및 종가
		m_pCurSelectedLine->m_ptAIndexedStart = m_pCurSelectedLine->m_ptAStart;
		m_pCurSelectedLine->m_ptAIndexedEnd = m_pCurSelectedLine->m_ptAEnd;

		// 시작일 및 종가
		if(GetIndexFromPoint(m_pCurSelectedLine->m_ptAStart, ix, iy) == R_ERROR) goto Drawing;
		if(IsValidIndex(ix))
			m_pCurSelectedLine->m_nSIndex = ix;
 		if(GetValueFromIndex(ix, iy, m_pCurSelectedLine->m_lSDate, dDummy) == R_ERROR) goto Drawing;
		// 끝일 및 종가
		if(GetIndexFromPoint(m_pCurSelectedLine->m_ptAEnd, ix, iy) == R_ERROR) goto Drawing;
		if(IsValidIndex(ix))
			m_pCurSelectedLine->m_nEIndex = ix;
 		if(GetValueFromIndex(ix, iy, m_pCurSelectedLine->m_lEDate, dDummy) == R_ERROR) goto Drawing;
		if(m_bNewDraw == FALSE)
		{
			// 중간점 위치
			if(GetIndexFromPoint(m_pCurSelectedLine->m_ptAt, ix, iy) == R_ERROR) goto Drawing;
			if(!IsValidIndex(ix)) goto Drawing;
		}
		// 클릭 포인트 값
		m_pCurSelectedLine->m_dSClicked = m_pIChartOCX->ConvertYPositionToData(m_pCurSelectedLine->m_ptAStart.y, m_rctGraphDrwingRegion.top, m_rctGraphDrwingRegion.bottom, m_dViewMax, m_dViewMin, m_bLog, m_bReverse);
		m_pCurSelectedLine->m_dEClicked = m_pIChartOCX->ConvertYPositionToData(m_pCurSelectedLine->m_ptAEnd.y, m_rctGraphDrwingRegion.top, m_rctGraphDrwingRegion.bottom, m_dViewMax, m_dViewMin, m_bLog, m_bReverse);

		if(m_pCurSelectedLine->m_ptAStart.x < m_pCurSelectedLine->m_ptAEnd.x)
		{
			m_pCurSelectedLine->m_ptAIndexedStart.y = GetYPositionOnLine((int&)m_pCurSelectedLine->m_ptAIndexedStart.x, m_pCurSelectedLine->m_ptAStart, m_pCurSelectedLine->m_ptAEnd);
			m_pCurSelectedLine->m_ptAIndexedEnd.y = GetYPositionOnLine((int&)m_pCurSelectedLine->m_ptAIndexedEnd.x, m_pCurSelectedLine->m_ptAStart, m_pCurSelectedLine->m_ptAEnd);
			m_pCurSelectedLine->m_ptAExpand.x = m_rctBlockRegion.right;
			m_pCurSelectedLine->m_ptAExpand.y = GetYPositionOnLine((int&)m_pCurSelectedLine->m_ptAExpand.x, m_pCurSelectedLine->m_ptAIndexedStart, m_pCurSelectedLine->m_ptAIndexedEnd);
			m_pCurSelectedLine->m_dSData = m_pIChartOCX->ConvertYPositionToData(m_pCurSelectedLine->m_ptAIndexedStart.y, m_rctGraphDrwingRegion.top, m_rctGraphDrwingRegion.bottom, m_dViewMax, m_dViewMin, m_bLog, m_bReverse);
			m_pCurSelectedLine->m_dEData = m_pIChartOCX->ConvertYPositionToData(m_pCurSelectedLine->m_ptAIndexedEnd.y, m_rctGraphDrwingRegion.top, m_rctGraphDrwingRegion.bottom, m_dViewMax, m_dViewMin, m_bLog, m_bReverse);
		}

Drawing:
		m_pCurSelectedLine->Draw(pDC);
		//pDC->SelectClipRgn(&rgn, NULLREGION);
		pDC->SelectClipRgn(NULL);
		rgn.DeleteObject();
	}
	return rct;

}

//////////////////////////////////////////////////////////////////////////
// Functions
//////////////////////////////////////////////////////////////////////////

BOOL CChartOrderLineAddInImp::GetChartBasicData(int &p_nStartIndex, int &p_nEndIndex, int &p_nR, int &p_nC, CRect &p_rctBlockRegion, CRect &p_rctGraphRegion)
{
	// 현 Data View의 Start/End Index를 구한다.
	if( !m_pIChartOCX->GetViewStartEndIndex( p_nStartIndex, p_nEndIndex)) return FALSE;
	int nV, nG;
	

	// 가격차트가 포함된 Block의 Index를 구한다.
	if( !m_pIChartManager->GetIndexOfGraph( "가격차트", p_nR, p_nC, nV, nG)) return FALSE;
	
	// Block의 Log/뒤집기 설정을 확인하여 정확하게 Data를 변환한다. (가격<->Y Pos)
	IBlock *pIBlock = m_pIChartManager->GetBlock( p_nR, p_nC);
	if( pIBlock)
	{
		pIBlock->GetLogAndReverseScaleFlag( m_bLog, m_bReverse);
		pIBlock->GetVScaleGroupMinMax(nV, m_dViewMin, m_dViewMax);
	   	pIBlock->GetGraphRegions(p_rctGraphRegion, p_rctBlockRegion );
		m_rctGraphDrwingRegion = pIBlock->GetGraphDrwingRegion();

		// Scale 축에 따른 offset 얻기//////////////////////////////////
 		m_ptOffSet.x = p_rctGraphRegion.left;
		m_ptOffSet.y = p_rctGraphRegion.top;
		//////////////////////////////////////////////////////

		pIBlock->Release();
		pIBlock = NULL;
	}

	return TRUE;
}

void CChartOrderLineAddInImp::OnOLConfig()
{
	COrderLineConfigDlg dlg;

	dlg.m_OLObj = (CObject*)m_pCurSelectedLine;
	ILPCSTR szUserPath = m_pIChartOCX->GetUserFilePathString();
	dlg.m_strUserPath = szUserPath;
	dlg.m_pAllOrderLines = &m_AllOrderLines;	// 저장된 모든 주문선
	if(dlg.DoModal() == IDOK)
	{
		m_pIChartOCX->InvalidateControl();
	}
	else
	{
		if(m_nCommandType == 1)
		{
			POSITION pos = m_LineList.Find(m_pCurSelectedLine);
			m_LineList.RemoveAt(pos);
			delete m_pCurSelectedLine;
			m_pCurSelectedLine = NULL;
		}
	}

}

void CChartOrderLineAddInImp::OnOLMan()
{
	AFX_MANAGE_STATE( AfxGetStaticModuleState());

	COrderLineManDlg dlg;
	ILPCSTR szUserPath = m_pIChartOCX->GetUserFilePathString();
	dlg.m_strUserPath = szUserPath;
	dlg.m_pLineList = &m_LineList;
	dlg.m_pSavedAllOrderLines = &m_AllOrderLines;	// 저장된 모든 주문선
	dlg.DoModal();
	m_pIChartOCX->InvalidateControl();
}

//////////////////////////////////////////////////////////////////////////
// 메모리 & 파일에서 주문선 삭제
void CChartOrderLineAddInImp::OnDeleteLine()
{
	//////////////////////////////////////////////////////////////////////////
	// 저장 리스트에서 삭제

	if(!m_pCurSelectedLine) return;

	// 해당 주문선 찾기
	BOOL bExistLine = FALSE;
	POSITION prevpos, pos = m_AllOrderLines.GetHeadPosition();
	while (pos)
	{
		prevpos = pos;
		if(m_AllOrderLines.GetNext(pos).Find(m_pCurSelectedLine->m_strSerial) >= 0)
		{
			bExistLine = TRUE;
			break;
		}
	}

	if(bExistLine)		// 존재하면 파일에서 삭제
	{
		m_AllOrderLines.RemoveAt(prevpos);

		// 파일에 결과 저장
		CFile f;
		CString strFileName;
		ILPCSTR szUserPath = m_pIChartOCX->GetUserFilePathString();

		strFileName = szUserPath;
		strFileName += OL_FILE;

		if( !f.Open( strFileName, CFile::modeCreate | CFile::modeWrite ) ) {
			  return;
		}
		CArchive ar( &f, CArchive::store);

		m_AllOrderLines.Serialize(ar);
	}

	//////////////////////////////////////////////////////////////////////////

	OnDeleteLine2();	//메모리에서 주문선 삭제
}

//////////////////////////////////////////////////////////////////////////
// 메모리에서만 주문선 삭제
void CChartOrderLineAddInImp::OnDeleteLine2()
{
	if(!m_pCurSelectedLine) return;

	POSITION pos = m_LineList.Find(m_pCurSelectedLine);
	m_LineList.RemoveAt(pos);
	UnregPS((long)m_pCurSelectedLine);
	delete m_pCurSelectedLine;
	m_pCurSelectedLine = NULL;

	m_pIChartOCX->InvalidateControl();
}

//////////////////////////////////////////////////////////////////////////
// 연속 그리기 해제
void CChartOrderLineAddInImp::OnReleaseDraw()
{
	m_nCommandType = 2;
	if( m_pIChartOCX != NULL )
		m_pIChartOCX->OnToolStateChanged( CToolOptionInfo::T_ORDER_LINE, FALSE );	
//	LoadOrderLine();	// 나중에 삭제

}

BOOL CChartOrderLineAddInImp::GetValueFromPoint(CPoint& ptAt/*IN, OUT*/, long &x/*OUT*/, double &y/*OUT*/)
{
	int ix, iy;
	if(GetIndexFromPoint(ptAt, ix, iy) == -1) return FALSE;
	if(GetValueFromIndex(ix, iy, x, y) == -1) return FALSE;
	return TRUE;
}

int CChartOrderLineAddInImp::GetIndexFromPoint(CPoint& pt/*IN, OUT*/, int &x/*IN*/, int &y/*IN*/)
{
	POINT ptTemp = pt;
	ptTemp.x += m_ptOffSet.x;
	ptTemp.y += m_ptOffSet.y;

	int nStartIndex, nEndIndex;
	if( !m_pIChartOCX->GetViewStartEndIndex( nStartIndex, nEndIndex)) return R_ERROR;
	// 1.3 지정된 Mouse Point의 Data Index를 구한다.
	x = y = nStartIndex + ( ( pt.x - m_rctBlockRegion.left) * ( nEndIndex - nStartIndex + 1)) / m_rctBlockRegion.Width();

	return R_OK;
}

int CChartOrderLineAddInImp::GetValueFromIndex(int ix/*IN*/, int iy/*사용하지 않음*/, long &lx/*OUT*/, double &dy/*사용하지 않음*/)
{
	// 2.1 자료일자 Packet을 구한다.
	IPacket *pIPacket = m_pIPacketManager->GetPacket( "자료일자");
	if( !pIPacket) return R_ERROR;

	try
		{
		double dTime;
		if( !pIPacket->GetData( ix, dTime)) throw this;
		lx = ( long)dTime;
	}
	catch( CChartOrderLineAddInImp *)
	{
		pIPacket->Release();
		return R_ERROR;
	}

	pIPacket->Release();
	return R_OK;
}

int CChartOrderLineAddInImp::GetPointFromIndex(int x/*IN*/, int y/*사용하지 않음*/, CPoint &ptIndex/*OUT*/)
{
	if(m_bCentered)	// 중심선에 정렬 
		ptIndex.x = (long)( (x*(m_rctBlockRegion.Width()+1) -m_nStartIndex*( m_rctBlockRegion.Width()+1) + (m_rctBlockRegion.Width())/2.)/
					( m_nEndIndex - m_nStartIndex + 1) + m_rctBlockRegion.left+2);
	else
		ptIndex.x = (long)((x*m_rctBlockRegion.Width() - m_nStartIndex*m_rctBlockRegion.Width())/
					( m_nEndIndex - m_nStartIndex + 1) + m_rctBlockRegion.left-1);

	return R_OK;
}

int CChartOrderLineAddInImp::GetIndexFromValue(long lx/*IN*/, double dy/*IN*/, int &ix/*OUT, 사용하지 않음*/, int &iy/*OUT, 사용하지 않음*/)
{
	IPacket *pIPacket = m_pIPacketManager->GetPacket( "자료일자");

	try
	{
//		double dTime;
//		for(;iy< m_nEndIndex - m_nStartIndex+1;iy++)
//		{
//			if( !pIPacket->GetData( iy, dTime)) throw this;
//			if(lx == ( long)dTime)	// 있으면...
//			{
//				ix = iy;
//				pIPacket->Release();
//				iy++;
//				return R_OK;
//			}
//			else if(lx < ( long)dTime)
//			{
//				//iy++;
//				pIPacket->Release();
//				return R_ERROR;
//			}
//		}

		double dTime;
		for(int n=m_nStartIndex; n<m_nEndIndex ;n++)
		{
			if( !pIPacket->GetData( n, dTime)) throw this;
			if(lx == ( long)dTime)	// 있으면...
			{
				//ix = iy;
				pIPacket->Release();
				//iy++;
				ix = n;
				return R_OK;
			}
			else if(lx < ( long)dTime)
			{
				//iy++;
				pIPacket->Release();
				return R_ERROR;
			}
		}
		
	}
	catch( CChartOrderLineAddInImp *)
	{
		pIPacket->Release();
		return R_ERROR;
	}

	pIPacket->Release();

	return R_ERROR; 
}

/////////////////////////////////////////////
// pt1, pt2를 포함하는 선의 연장선 상의 y pixel 값을 구한다.
int CChartOrderLineAddInImp::GetYPositionOnLine(int& x/*IN, OUT*/, CPoint pt1/*IN*/, CPoint pt2/*IN*/)
{
	int y;
	double dy, x1, x2, y1, y2;
	x1 = pt1.x;
	x2 = pt2.x;
	y1 = pt1.y;
	y2 = pt2.y;

	if(pt1.x < pt2.x)
	{
		y = (long)( (1.0*(pt1.y - pt2.y)*x
			+(pt1.x * pt2.y - pt2.x * pt1.y))
			/((pt1.x - pt2.x)*1.0));
	}
	if(x1 < x2)
	{
		dy = (long)(((y1 - y2)*x
			+(x1 * y2 - x2 * y1))
			/(x1 - x2));
	}

	if(y < 0)
	{
		y = 0;
		x = (int)((pt2.x * pt1.y - pt1.x * pt2.y)/(1.0*(pt1.y - pt2.y)));//(pt2.x * pt1.y - pt1.x * pt2.y)/((pt1.x - pt2.x)*1.0)/(1.0*(pt1.y - pt2.y);
	}

	return y;
}

//////////////////////////////////////////////////////////////////////////
// 주어진 index 가 유효한 것인지 여부 판단
BOOL CChartOrderLineAddInImp::IsValidIndex(int nIndex)
{
	IPacket *pIPacketForCur = m_pIPacketManager->GetPacket( "종가");
	try
	{
		double dCur=0;
		if( !pIPacketForCur->GetData( nIndex, dCur)) throw this;	// 시작일 종가
	}
	catch( CChartOrderLineAddInImp *)
	{
		pIPacketForCur->Release();
		return FALSE;
	}
	pIPacketForCur->Release();
	return TRUE;
}

void CChartOrderLineAddInImp::RegPS(long pLine)
{
	if( m_pIAddInToolMgr != NULL )
	{
		m_pIAddInToolMgr->OnAddInToolCommand( EAI_TOOLORDERMGR_ADD_TOOL_TO_ORDERMANAGER,	
											  m_strAddInItemName,		
											  pLine );	
	}
}

void CChartOrderLineAddInImp::UnregPS(long pLine)
{
	if( m_pIAddInToolMgr != NULL )
	{
		m_pIAddInToolMgr->OnAddInToolCommand( EAI_TOOLORDERMGR_DELETE_TOOL_FROM_ORDERMANAGER,	
											  m_strAddInItemName,		
											  pLine );	
	}
}

CString CChartOrderLineAddInImp::TokenizeEx(CString strSrc/*IN*/, const char* pszTokens/*IN*/, int& iStart/*IN, OUT*/)
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

//////////////////////////////////////////////////////////////////////////
// 저장된 주문선 로드
void CChartOrderLineAddInImp::LoadOrderLine()
{
	//////////////////////////////////////////////////////////////////////////
	// 초기화
	if(!m_AllOrderLines.IsEmpty())
		m_AllOrderLines.RemoveAll();
	//////////////////////////////////////////////////////////////////////////

	CString strLineInfo;
	int nCnt = 0;
	ILPCSTR szUserPath = m_pIChartOCX->GetUserFilePathString();
	CString strFileName = szUserPath + OL_FILE;

	//////////////////////////////////////////////////////////////////////////
	// ar 속성 설정
	CFile f;
	if( !f.Open( strFileName, CFile::modeRead ) ) {
		  return;
	}
	CArchive ar( &f, CArchive::load);
	//////////////////////////////////////////////////////////////////////////
	m_AllOrderLines.Serialize(ar);

	if(m_AllOrderLines.GetCount() && !m_nCommandType) m_nCommandType = 2;	// 저장된 주문선이 있으며 그리기 모드가 아닐 경우, 수정 모드로 세팅

	POSITION pos = m_AllOrderLines.GetHeadPosition();
	while (pos)
	{
		strLineInfo = m_AllOrderLines.GetNext(pos);
		strLineInfo += ",";
		CLineObj* pLine;
		pLine = new CLineObj;
		int iStart2 = 0;
		pLine->m_strSerial = TokenizeEx(strLineInfo, ",", iStart2);
		pLine->m_strCode = TokenizeEx(strLineInfo, ",", iStart2);
		pLine->m_strName = TokenizeEx(strLineInfo, ",", iStart2);
		pLine->m_strCandle = TokenizeEx(strLineInfo, ",", iStart2);
		pLine->m_lSDate = atol(TokenizeEx(strLineInfo, ",", iStart2));
		pLine->m_dSData = atof(TokenizeEx(strLineInfo, ",", iStart2));
		pLine->m_lEDate = atol(TokenizeEx(strLineInfo, ",", iStart2));
		pLine->m_dEData = atof(TokenizeEx(strLineInfo, ",", iStart2));
		pLine->m_bUpSell = atoi(TokenizeEx(strLineInfo, ",", iStart2));		
		pLine->m_bUpBuy = atoi(TokenizeEx(strLineInfo, ",", iStart2));		
		pLine->m_bDownSell = atoi(TokenizeEx(strLineInfo, ",", iStart2));	
		pLine->m_bDownBuy = atoi(TokenizeEx(strLineInfo, ",", iStart2));	
		pLine->m_nLineThickness = atoi(TokenizeEx(strLineInfo, ",", iStart2));
		pLine->m_nLineStyle = atoi(TokenizeEx(strLineInfo, ",", iStart2));	
		pLine->m_clrLineColor = (COLORREF) atol(TokenizeEx(strLineInfo, ",", iStart2));

		// 해당 종목 및 기준붕의 데이터만 추가
		if((pLine->m_strCode == m_strCode) && ((pLine->m_strCandle) == m_strDateType))
		{
			m_LineList.AddTail(pLine);
			RegPS((long)pLine);
			nCnt++;
		}
		else
		{
			delete pLine;
			pLine = NULL;
		}
	}
	m_pIChartOCX->InvalidateControl();
}

