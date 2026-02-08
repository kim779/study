// LowHighMakerAddInImp.cpp: implementation of the CLowHighMakerAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "chartcommonaddin.h"
#include "LowHighMakerAddInImp.h"

#include "../CommonTR/Data_Common.h"

#include <math.h>										// for HUGE_VAL
#include "../Include_Chart/Dll_Load/IMetaTable.h"	// for _MTEXT()
#include "../Include_Chart/IPropertyMap.h"

#ifndef __COMMON_DEF_H
	#include "./Client/commondef.h"
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLowHighMakerAddInImp::CLowHighMakerAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase)
	: CChartAddInBase( p_pIChartOCX, p_pIAddInDllBase)
{
	m_bEnableMark = FALSE;
//	m_bEnableMark = TRUE;

	m_bLoaded = FALSE;

	m_strCode = "";
/*
	ML_SET_LANGUAGE_RES();
	m_pAnalysisTool = new CMainAnalysisTool( m_pChartCWnd, p_pIChartOCX->GetOcxHwnd(), ( CChartInfo::CHART_MODE)p_pIChartOCX->GetChartMode());
	// update ver 1.2 -> 11.5.2001
	m_pAnalysisTool->SetMainBlock( m_pIChartManager, p_pIChartOCX);
*/
}

CLowHighMakerAddInImp::~CLowHighMakerAddInImp()
{
/*
	if( m_pAnalysisTool)
	{
		delete m_pAnalysisTool;
		m_pAnalysisTool = NULL;
	}
*/
}

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
CString CLowHighMakerAddInImp::m_strAddInItemName = "LOWHIGH_MARKER";

//////////////////////////////////////////////////////////////////////
// Define AddIn Event Map
//////////////////////////////////////////////////////////////////////
BEGIN_ADDIN_EVENT_MAP( CLowHighMakerAddInImp)
	ONADDINEVENT( OnDrawAfterGraph)
	ONADDINEVENT( OnAddInToolCommand_EAICommandType_long)
	ONADDINEVENT( OnAddInToolCommand_long_EAICommandType)
	ONADDINEVENT( OnPacketDataMultiItem)
//	ONADDINEVENT( OnToolCommand)
//	ONADDINEVENT( OnAddInToolCommand_EAICommandType)
//	ONADDINEVENT( OnAddInToolCommand_EAICommandType_char_long)
//	ONADDINEVENT( OnAddInToolCommand_EAICommandType_long)
	// 기준선 Line값 표시 기능 추가 및 개별적용 기능 추가 - onlyojt
	ONADDINEVENT( OnLoadEnvironment)
	// 기준선 Line값 표시 기능 추가 및 개별적용 기능 추가 - onlyojt
	ONADDINEVENT( OnSaveEnvironment)	
END_ADDIN_EVENT_MAP()


void CLowHighMakerAddInImp::OnDrawAfterGraph( HDC p_hDC)
{
	if( m_bEnableMark == FALSE )		return;

	IPacket* pIPacket, *pLowPacket, *pHighPacket, *pDatePacket;
	pIPacket = m_pIPacketManager->GetPacket( _MTEXT( C0_UPDOWN_SIGN));
	if( !pIPacket )		return;

	CDC *pDC = CDC::FromHandle( p_hDC);
	if( !pDC)
	{
		if( pIPacket) pIPacket->Release();
		return;
	}

	// (2004.09.15, 배승원) 특수차트인 경우에는 보이지 않도록 한다. (역시계곡선/삼선전환도/P&F)
	if( m_pIChartOCX->IsOnSpecialChart())
	{
		if( pIPacket) pIPacket->Release();
		return;
	}

	// 3. 가격차트의 정보를 얻어온다.
	int nR, nC, nV, nG;
	if (m_pIChartManager->GetIndexOfGraph("DEFAULT", _MTEXT( C2_PRICE_CHART), nR, nC, nV, nG) == FALSE)
	{
		if (pIPacket) pIPacket->Release();
		return;
	}

	// 4. Block 정보를 얻어온다.
	IBlock *pIBlock = NULL;
	pIBlock = m_pIChartManager->GetBlock( nR, nC );
	if( !pIBlock)
	{
		if( pIPacket) pIPacket->Release();
		return;
	}
	
	// 5. Block에서 GraphResion을 얻어온다.
	CRect rctNoFrameAndMargin, m_rcGraphRegion, m_rctGraphDrawingRegion;
	pIBlock->GetGraphRegions(m_rcGraphRegion, rctNoFrameAndMargin);
	if( m_rcGraphRegion.IsRectEmpty())
	{
		if( pIPacket) pIPacket->Release();
		return;
	}

	m_rctGraphDrawingRegion	= pIBlock->GetGraphDrwingRegion();
	if( m_rctGraphDrawingRegion.IsRectEmpty())
	{
		if( pIPacket) pIPacket->Release();
		return;
	}

	double m_dViewMin, m_dViewMax;
	// 2.2.0 View의 Min/Max 값을 구한다.
	pIBlock->GetVScaleGroupMinMax( nV, m_dViewMin, m_dViewMax);
	// (2007/1/15 - Seung-Won, Bae) Check No Data in Block
	if( m_dViewMin == m_dViewMax && m_dViewMax == -HUGE_VAL)
	{
		if( pIPacket) pIPacket->Release();
		return;
	}

	// 10. 가격차트의 Block에 Log/뒤집기 설정값을 확인한다.
	BOOL m_bLog, m_bReverse;
	pIBlock->GetLogAndReverseScaleFlag(m_bLog, m_bReverse);
	pIBlock->Release();
	pIBlock = NULL;

	int m_nStartIndex, m_nEndIndex;
	// 1. 현재 화면에 보이는 Data의 Start/End Index를 구한다.
	if( !m_pIChartOCX->GetViewStartEndIndex( m_nStartIndex, m_nEndIndex ))
	{
		if( pIPacket) pIPacket->Release();
		return;
	}

	double dData, dPrice, dDate;
	int top, left;
	pLowPacket = m_pIPacketManager->GetPacket( _MTEXT( C0_LOW));
	pHighPacket = m_pIPacketManager->GetPacket( _MTEXT( C0_HIGH));
	pDatePacket = m_pIPacketManager->GetPacket( _MTEXT( C0_DATE_TIME));

	dData=.0f;
	if( pLowPacket != NULL && pHighPacket != NULL )
	{
		if( m_nStartIndex<=m_nEndIndex )
		{
			pIPacket->GetData( m_nEndIndex, dData);
		}
	}

	int nLastXPos = m_pIChartManager->GetXPosition("DEFAULT", m_nEndIndex, 0);
	if( m_dcdCash.IsChanged(m_rctGraphDrawingRegion, nLastXPos, m_dViewMin, m_dViewMax, m_bLog, m_bReverse, m_nStartIndex, m_nEndIndex, dData ) ){
		if( pLowPacket != NULL && pHighPacket != NULL )
		{
			for( int i= m_nStartIndex; i<=m_nEndIndex; i++ )
			{
				pIPacket->GetData( i, dData);

				if( dData == 1/*상한*/ || dData == 4/*하한*/ )
				{
					if( dData == 1 )	{	pHighPacket->GetData( i, dPrice);	m_dcdCash.SetDirUp(); }
					else				{	pLowPacket->GetData( i, dPrice);	m_dcdCash.SetDirDn(); }

					pDatePacket->GetData(i, dDate);

					top		= m_pIChartOCX->ConvertDataToYPosition( dPrice, m_dViewMax, m_dViewMin, m_rctGraphDrawingRegion.top, m_rctGraphDrawingRegion.bottom, m_bLog, m_bReverse);
					left	= m_pIChartManager->GetXPosition("DEFAULT", i, 0);

					m_dcdCash.SetPoint( top, left );
					m_dcdCash.incEffectCount();
				}
			}
		}
	}

	bool bUseClip = true;
	bool bDbgShow = false;
	CRgn rgnClip;

	if( pLowPacket != NULL && pHighPacket != NULL )
	{
		int nSaveDC = pDC->SaveDC();

		//{{ 영역확인을 위한 디버그코드
		if( bDbgShow ){
			RECT rClip = rctNoFrameAndMargin; //m_rctGraphDrawingRegion;
			CBrush brshDbg(RGB(192, 20,20));
			pDC->FrameRect( &rClip, &brshDbg );
			brshDbg.DeleteObject();
		}
		//}}

		//{{ clipping 을 위한 코드추가
		if( bUseClip ){
//			RECT rClip = rctNoFrameAndMargin; //m_rctGraphDrawingRegion;
			RECT rClip = m_rcGraphRegion;
			//rgnClip.CreateEllipticRgnIndirect(&rClip);
			rgnClip.CreateRectRgn( rClip.left, rClip.top, rClip.right, rClip.bottom );
			pDC->SelectClipRgn(&rgnClip);
		}
		//}}

		// text 표시 속성
		CFont defFont, boldFont;

		//CString strFont;
		//strFont.LoadString(IDS_STRING_FONT);
		//defFont.CreatePointFont(90,strFont, NULL);
		defFont.CreatePointFont(90, _T("굴림"), NULL);
		CFont *poldFont = pDC->SelectObject(&defFont);
		pDC->SetBkMode(TRANSPARENT);

		int		nDataOnePage = m_nEndIndex - m_nStartIndex + 1;								// 현 Data View의 Data 갯수
		double	dCandleWidth = double( m_rcGraphRegion.Width()) / double( nDataOnePage) - 2;	// 봉의 폭을 구한다
		//if( dCandleWidth > 19. )		dCandleWidth = 19;
		//if( dCandleWidth < 9. )			dCandleWidth = 9;
		//if( dCandleWidth < 12. )			
			dCandleWidth = 12;

		CRect rect;
		CPoint polygon[7];
		CRgn UpLimit, DnLimit;
		CBrush brUp, brDn;
		brUp.CreateSolidBrush(RGB(255, 25, 25));
		brDn.CreateSolidBrush(RGB(25, 25, 255));

		ILPCSTR dlmt;

		for( int i= 0; i<m_dcdCash.nEffectCount; i++ )
		{
			//pIPacket->GetData( i, dData);
			m_dcdCash.GetDataSet( i, top, left, dData );

			if( dData == 1/*상한*/ || dData == 4/*하한*/ )
			{
				//if( dData == 1 )		pHighPacket->GetData( i, dPrice);
				//else					pLowPacket->GetData( i, dPrice);

				//top	= m_pIChartOCX->ConvertDataToYPosition( dPrice, m_dViewMax, m_dViewMin, m_rctGraphDrawingRegion.top, m_rctGraphDrawingRegion.bottom, m_bLog, m_bReverse);
				//left	= m_pIChartManager->GetXPosition("DEFAULT", i, 0);

				if( dData == 1 )				
				{
					//int l = left-6;
					//int t = top-22;
					//int r = l + 20;
					//int b = min( t+20, rctNoFrameAndMargin.bottom );
					//CRect rcDrawText( l, t, r, b);

					pDC->SetTextColor(RGB(255, 25, 25));
					pDC->TextOut(left-6, top-(22+4), "상");
					//DrawTextA( p_hDC, "상", 2, &rcDrawText, DT_LEFT|DT_TOP );

					left -= (int)(dCandleWidth/2.);

					//rect.SetRect(left, top-10, left + (int)dCandleWidth, top - 1 );
					rect.SetRect(left, top-14, left + (int)dCandleWidth, top - 1 );
					int quard = rect.Height() / 4;
	
					polygon[0].x = rect.left + rect.Width() / 2;
					polygon[0].y = rect.bottom -1;
					
					polygon[1].x = rect.left+1;
					polygon[1].y = rect.top + rect.Height() / 2;
					
					polygon[2].x = polygon[0].x - (quard-1); //2;
					polygon[2].y = polygon[1].y;
					
					polygon[3].x = polygon[2].x;
					polygon[3].y = rect.top + 1;
					
					polygon[4].x = polygon[0].x + (quard); //2; 
					polygon[4].y = polygon[3].y;
					
					polygon[5].x = polygon[4].x;
					polygon[5].y = polygon[1].y;
					
					polygon[6].x = rect.right;
					polygon[6].y = polygon[1].y;
					
					UpLimit.CreatePolygonRgn(polygon, 7, WINDING);
					pDC->FillRgn(&UpLimit, &brUp);
					UpLimit.DeleteObject();

				}
				else
				{
					//int l = left-6;
					//int t = top+12;
					//int r = l + 20;
					//int b = min( t+50, rctNoFrameAndMargin.bottom );
					//CRect rcDrawText( l, t, r, b);

					pDC->SetTextColor(RGB(25, 25, 255));
					pDC->TextOut(left-6, top+12+4, "하");
					//DrawTextA( p_hDC, "하", 2, &rcDrawText, DT_LEFT|DT_TOP );

					left -= (int)(dCandleWidth/2.);

					//rect.SetRect(left, top+2, left + (int)dCandleWidth, top+11 );
					rect.SetRect(left, top+2, left + (int)dCandleWidth, top+18 );
					int quard = rect.Height() / 4;

					polygon[0].x = rect.left + rect.Width() / 2 ; //+1;
					polygon[0].y = rect.top+1;

					polygon[1].x = rect.right+1;
					polygon[1].y = rect.top + rect.Height() / 2 ; //+1;

					polygon[2].x = polygon[0].x + (quard-1); //2;
					polygon[2].y = polygon[1].y;

					polygon[3].x = polygon[2].x;
					polygon[3].y = rect.bottom - (1+2);

					polygon[4].x = left+quard; //polygon[0].x - (quard-1); //2; 
					polygon[4].y = polygon[3].y;
					
					polygon[5].x = polygon[4].x;
					polygon[5].y = polygon[1].y;
					
					polygon[6].x = rect.left;
					polygon[6].y = polygon[1].y;
					
					DnLimit.CreatePolygonRgn(polygon, 7, WINDING);
					pDC->FillRgn(&DnLimit, &brDn);
					DnLimit.DeleteObject();
				}
			}
		}

		pDC->SelectObject(poldFont);
		defFont.DeleteObject();
		brUp.DeleteObject();
		brDn.DeleteObject();
			
		if( bUseClip )
		{
			pDC->SelectClipRgn(NULL);
			rgnClip.DeleteObject();
		}

		pDC->RestoreDC(nSaveDC);
	}


	if( pIPacket )			pIPacket->Release();
	if( pLowPacket )		pLowPacket->Release();
	if( pHighPacket	)		pHighPacket->Release();
	if( pDatePacket)		pDatePacket->Release();
}


//////////////////////////////////////////////////////////////////////
// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
//////////////////////////////////////////////////////////////////////

BOOL CLowHighMakerAddInImp::OnAddInToolCommand( const EAICommandType p_eCommandID, const long p_lParam)
{
	if( EAI_CUSTOM_ENABLE_LOWHIGHMARK != p_eCommandID) return FALSE;

	// (2004.10.22, 배승원) 상/하한가 표시기능에 호가연동 기능을 보강한다.
	EnableLowHighMark( p_lParam);	

	// (2004.10.06, 배승원) 수정시 바로 화면에 반영되도록 Redraw시킨다.
	// (2004.12.13, 배승원) NULL Pointer를 확인한다.
	m_pIChartOCX->InvalidateControl();

	
	return TRUE;
}

BOOL CLowHighMakerAddInImp::OnAddInToolCommand( long &p_lResult, const EAICommandType p_eCommandID)
{
	switch( p_eCommandID)
	{
		case EAI_CUSTOM_GET_LOWHIGHMARK_FLAG:		p_lResult = GetLowHighMarkFlag();
											return TRUE;
		case EAI_CUSTOM_GET_EXISTENCE_LOWHIGHMARK:	p_lResult = IsExistenceLowHighMark();
											return TRUE;
		default:	return FALSE;
	}
	return FALSE;
}

void CLowHighMakerAddInImp::OnPacketDataMultiItem(LPCTSTR lpszRQ, const char *p_szPacketNames, BOOL p_bWithReset)
{

	// place holder function
	int ii = 0 ;

}

void CLowHighMakerAddInImp::OnUpdatePacketDataMultiItem(LPCTSTR lpszRQ, BOOL p_bTrUpdate)
{
	//// 해당 RQ의 AssistIndexManager : 복수종목 - ojtaso (20070305)
	//CPatternMaker* pPatternMaker;
	//GetPatternMaker(pPatternMaker, lpszRQ);

	//if(!pPatternMaker)
	//	return;

	//if(p_bTrUpdate==0&&pPatternMaker)
	//{
	//	if ( m_bRecvRealData_S31 )
	//	{
	//		m_bRecvRealData_S31 = FALSE;


	//		pPatternMaker->DeleteAllData();					// 패턴관련 데이터 모두 삭제
	//		pPatternMaker->InitSelectedPatternData(TRUE);	// Init Selected Pattern Data

	//		// Get All WavePattern Data
	//		pPatternMaker->GetAllData_AboutPattern();

	//		// 라인을 그리는 경우
	//		if ( !pPatternMaker->m_PatternEnvData.m_bNotDrawAll)
	//		{
	//			// 실시간에서 그려줄 선택된 패턴이 있는지 체크
	//			pPatternMaker->CheckSelectedPatternData_ForReal();
	//		}
	//	}
	//}
}

void CLowHighMakerAddInImp::OnLoadEnvironment( IPropertyMap *p_pIAddInEnvMap)	
{
	if(p_pIAddInEnvMap->GetCount() <= 0)
		return;

	m_bLoaded = TRUE;

	const char *chToolLoading;
	CString strToolLoading, strEnableMark;	
	p_pIAddInEnvMap->Lookup(m_strAddInItemName,chToolLoading);
	strToolLoading.Format("%s",chToolLoading);
	if(strToolLoading.IsEmpty() == TRUE)
		return;

	// (2009/8/20 - Seung-Won, Bae) To load previous setting of same time period.
	if( strToolLoading.Left( 11) == "EnableMark:")
	{
		int nItemInfoLen = strToolLoading.Find( "\r\n");
		strEnableMark = strToolLoading.Mid(11);
		m_bEnableMark = ( atoi(strEnableMark) == 0 ? FALSE : TRUE );
	}

	//m_pAnalysisTool->BuildElementsFromString(strToolLoading);	
}

void CLowHighMakerAddInImp::OnSaveEnvironment( IPropertyMap *p_pIAddInEnvMap)	
{
	if( !m_bLoaded ) 
		return;

	//CString strSaveToolList = m_pAnalysisTool->GetSavedElementsData();

	// (2009/8/20 - Seung-Won, Bae) To load previous setting of same time period.
	//if( strSaveToolList.IsEmpty())
//	{
		CString strItemInfo;
		strItemInfo.Format( "EnableMark:%d\r\n", m_bEnableMark);
		CString strSaveToolList = strItemInfo;// + strSaveToolList;
//	}

	p_pIAddInEnvMap->SetAt(m_strAddInItemName,strSaveToolList);
}

BOOL CLowHighMakerAddInImp::InvokeAddIn(int p_nCommandType, long p_lData)
{
	if(p_nCommandType == INVOKEMSG_RECEIVEREALDATA) // RealData수신
	{
		STREALTYPEDATA *prData = (STREALTYPEDATA *)p_lData;

		if (m_strCode != prData->szCode)
			return TRUE;

		int nSign = atoi(prData->sign);

		IPacket* pIPacket;
		pIPacket = m_pIPacketManager->GetPacket( _MTEXT( C0_UPDOWN_SIGN));
		if (!pIPacket)
			return TRUE;

		int nCount = pIPacket->GetDataCount();
		if (nCount > 0)
		{
			int nLastDay = nCount - 1;
			double dData;
			pIPacket->GetData(nLastDay, dData);

			if (dData != nSign)
			{
				dData = nSign;
				pIPacket->SetData(nLastDay, dData);
			}
		}

		pIPacket->Release();
		m_pIChartOCX->InvalidateControl();

		return TRUE;
	}
	else if (p_nCommandType == INVOKEMSG_CHANGECODE) //종목 코드가 바뀌었음.
	{
		stMSGToInvoke* pMSGToInvoke = NULL;
		pMSGToInvoke = (stMSGToInvoke*)p_lData;
		if (pMSGToInvoke)
		{			
			CString strThisChart;
			strThisChart.Format("%d", pMSGToInvoke->m_pChartItem);
			CString strCode;
			strCode.Format("%s", pMSGToInvoke->m_pBuffer);
			if (strCode != m_strCode)				// 기존의 종목코드가 다른 경우엔 
			{
				m_strCode = strCode;
				m_pIChartOCX->InvalidateControl();
			}

			return TRUE;
		}
	}

	return FALSE;
}

