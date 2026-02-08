// ToolTipAddInImp.cpp: implementation of the CToolTipAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ToolTipAddInImp.h"

#include <Mmsystem.h>									// for timeGetTime

// 수치조회, 툴팁에 등락률 추가 표시 - ojtaso (20080710)
#include <math.h>
														// XScale Text 툴팁표시 - ojtaso (20080429)
#include <locale.h>										// for setlocale()

#include "../Include_Addin_133101/I133101/_IString.h"	// for ILPCSTR
#include "../Include_Chart/Conversion.h"				// for CDataConversion
#include "../Include_Chart/BlockBaseEnum.h"
#include "../Include_Chart/Dll_Load/IMetaTable.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CToolTipAddInImp::CToolTipAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase)
	: CChartAddInBase( p_pIChartOCX, p_pIAddInDllBase),
	  m_niDataQueryer( p_pIChartOCX)
{

	// (2006/6/24 - Seung-Won, Bae) Manage the flag of Tooltip Using
	m_bToolTipCreate = FALSE;
	m_bMouseToolTip = FALSE;
	m_bTracingToolTip = FALSE;
	m_bMouseToolTipAll = TRUE;	//[A00000441]풍선도움말 보기시 전체 조회 옵션 추가
	// (2007/2/9 - Seung-Won, Bae) Set Mouse Move Delay.
	m_toolTip.SetDelay( 100);

	m_ptOldMousePos = CPoint(-1,-1);

	// (2007/2/9 - Seung-Won, Bae) Delay in TracingToolTip.
	m_dwDrawStartTime = 0;
//	m_hOcxWnd = ( m_pIChartOCX ? m_pIChartOCX->GetOcxHwnd() : NULL);
}

CToolTipAddInImp::~CToolTipAddInImp()
{
}

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
CString CToolTipAddInImp::m_strAddInItemName = "TOOL_TIP";

//////////////////////////////////////////////////////////////////////
// Define AddIn Event Map
//////////////////////////////////////////////////////////////////////
BEGIN_ADDIN_EVENT_MAP( CToolTipAddInImp)
	ONADDINEVENT( OnResetChart)
	ONADDINEVENT( OnToolCommand)
	ONADDINEVENT( OnMouseMove)
	ONADDINEVENT( OnAddInToolCommand_EAICommandType_long)
	ONADDINEVENT( OnAddInToolCommand_EAICommandType_HDC_UINT_POINT_DWORD)
END_ADDIN_EVENT_MAP()

// (2006/6/24 - Seung-Won, Bae) Manage the static Tooltip Object
TFXDataTip CToolTipAddInImp::m_toolTip;

//////////////////////////////////////////////////////////////////////
// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
//////////////////////////////////////////////////////////////////////

void CToolTipAddInImp::OnResetChart(void)
{
	m_pIChartOCX->OnToolStateChanged(CToolOptionInfo::T_TOOL_TRACEING_TT,FALSE);
	
	m_bTracingToolTip = FALSE;
	// (2007/2/9 - Seung-Won, Bae) Set Mouse Move Delay.
	m_toolTip.SetDelay( 100);

	if( m_bToolTipCreate && m_toolTip.m_hWnd != NULL) m_toolTip.Hide();			
}

BOOL CToolTipAddInImp::OnAddInToolCommand(	const EAICommandType p_eCommandID, HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	if( EAI_TOOLTIP_ONLBUTTONDOWN != p_eCommandID && EAI_TOOLTIP_ONLBUTTONUP != p_eCommandID) return FALSE;
	
	if( EAI_TOOLTIP_ONLBUTTONDOWN == p_eCommandID)	OnLButtonDown(	p_hDC, nFlags, point, p_dwMouseActionID);
	else											OnLButtonUp(	p_hDC, nFlags, point, p_dwMouseActionID);
	return TRUE;
}

BOOL CToolTipAddInImp::OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam)
{
	if( EAI_ALL_ON_CHANGE_CURSOR_MODE == p_eCommandID)
	{
		m_bMouseToolTip = p_lParam & TOOLTIPVIEW;

		//[A00000441]풍선도움말 보기시 전체 조회 옵션 추가
		m_bMouseToolTipAll	= p_lParam & TOOLTIPVIEW_ALL;

		// (2006/6/24 - Seung-Won, Bae) Create Static Tooltip
		if( m_bMouseToolTip && m_toolTip.m_hWnd == NULL) 
		{
			m_toolTip.Initialise( m_pIChartOCX->GetToolTipBackColor());	// 색깔 셋팅
			m_toolTip.Create();
			m_toolTip.SetBackgrandColor( RGB( 0, 0, 0));	// Not Used ?
			m_toolTip.SetSurrounding(1, 1);
			m_bToolTipCreate = TRUE;
		} 
	}
	return FALSE;
}

// (2006/11/24 - Seung-Won, Bae) On Tool Command
void CToolTipAddInImp::OnToolCommand( const int p_nToolType, const int p_nCmdOption)
{
	CToolOptionInfo::TOOLOPTION eToolOption = ( CToolOptionInfo::TOOLOPTION)p_nToolType;
	if(eToolOption == CToolOptionInfo::T_ALL_TOOL_RESET_STATE)
	{
		m_pIChartOCX->OnToolStateChanged(CToolOptionInfo::T_TOOL_TRACEING_TT,FALSE);

		m_bTracingToolTip = FALSE;
		// (2007/2/9 - Seung-Won, Bae) Set Mouse Move Delay.
		m_toolTip.SetDelay( 100);
		if( m_bToolTipCreate && m_toolTip.m_hWnd != NULL) m_toolTip.Hide();			
		return;
	}
	
	if(eToolOption == CToolOptionInfo::T_ALL_TOOL_TRUE_QUERY)
	{
		if(m_bTracingToolTip == TRUE) m_pIChartOCX->OnToolStateChanged(CToolOptionInfo::T_TOOL_TRACEING_TT,TRUE);
		return;
	}

	if(eToolOption == CToolOptionInfo::T_TOOL_TRACEING_TT)		
	{
		m_bTracingToolTip = TRUE;
		if(m_toolTip.m_hWnd == NULL)
		{
			m_toolTip.Initialise( m_pIChartOCX->GetToolTipBackColor());	// 색깔 셋팅
			m_toolTip.Create();
			m_toolTip.SetBackgrandColor( RGB( 0, 0, 0));	// Not Used ?
			m_toolTip.SetSurrounding(1, 1);
			m_bToolTipCreate = TRUE;
		}			
		m_toolTip.SetDelay( 0);							// ToolTip 이 보이는 시간 셋팅
	}
	else if(eToolOption == CToolOptionInfo::T_TOOL_SELECTION)
	{
		//TracingToolTip중에 환경설정등을 누른다면 ToolTip처리 없애준다
		if(m_bTracingToolTip == TRUE) m_pIChartOCX->OnToolStateChanged(CToolOptionInfo::T_TOOL_TRACEING_TT,FALSE);

		m_bTracingToolTip = FALSE;
		// (2007/2/9 - Seung-Won, Bae) Set Mouse Move Delay.
		m_toolTip.SetDelay( 100);
		m_toolTip.Hide();	
	}
}

BOOL CToolTipAddInImp::OnMouseMove( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID, const HBITMAP p_hbmpMainOSB)
{
	if( !m_bTracingToolTip && !m_bMouseToolTip || nFlags & 0x1F) return FALSE;

	// (2010/4/17 - Seung-Won, Bae) Do not show tooltip on backgroud window.
	//		I don't know why the mouse move message was delivered here with foreground other window.
	CPoint ptWindow( point);
	m_pChartCWnd->ClientToScreen( &ptWindow);
	if( WindowFromPoint( ptWindow) != m_hOcxWnd) return FALSE;

	if( m_ptOldMousePos == point) return FALSE;
	m_ptOldMousePos = point;

	// (2006/3/21 - Seung-Won, Bae) Check Graph Area
	int nRow, nColumn;
	if( !m_pIChartManager->GetBlockIndex( point, nRow, nColumn)) return FALSE;
	IBlock *pIBlock = m_pIChartManager->GetBlock( nRow, nColumn);
	if( !pIBlock) return FALSE;
	CRect rctGraph, rctNoFrameAndMargin;
	pIBlock->GetGraphRegions( rctGraph, rctNoFrameAndMargin);
	// (2008/11/8 - Seung-Won, Bae) Do selected RQ first. if no-graph, next MainRQ.
	ILPCSTR szMainRQ = m_pIChartOCX->GetCurrentRQ();
	int nV = -1, nG = -1;
	ILPCSTR szGraphName = pIBlock->FindFirstIndicator( NULL, nV, nG, szMainRQ);
	if( szGraphName.IsEmpty()) szMainRQ = pIBlock->GetMainRQ();
	if( !rctGraph.PtInRect( point))
	{
		// XScale Text 툴팁표시 - ojtaso (20080429)
		// ToolTip 이상현상 수정 - ojtaso (20080612)
		if( pIBlock->GetScaleTextType() != 1)
		{
			pIBlock->Release();
			m_toolTip.Hide();	
			return FALSE;
		}

		CRect rctTopScale, rctBottomScale;
		pIBlock->GetHorzScaleRegion(rctTopScale, rctBottomScale);
		if(!rctBottomScale.PtInRect(point) && !rctTopScale.PtInRect(point))
		{
			pIBlock->Release();
			m_toolTip.Hide();	
			return FALSE;
		}
			
		int nHieght = pIBlock->GetHorizontalScaleHeight();
		pIBlock->Release();
		if(nHieght < 0) return FALSE;
		
		CDC *pDC = CDC::FromHandle( p_hDC);
		CSize szText(0,0);
		if( pDC) pDC->GetTextExtent(_T(" "));

		ILPCSTR szGraphRQ = NULL;
		ILPCSTR szGraphName = m_pIChartManager->FindGraph( point);
		if( !szGraphName.IsEmpty()) szGraphRQ = m_pIChartManager->GetRQofGraph( szGraphName);
		if( !szGraphRQ.IsEmpty()) 
		{
			CString strRQ( szGraphRQ);
			DrawTooltipHorzScaleText( strRQ, point, szText.cy, nHieght);
		}
		return FALSE;
	}

	pIBlock->Release();
	pIBlock = NULL;

	CString strRQ;
	if( m_bTracingToolTip)
	{
		// (2007/2/9 - Seung-Won, Bae) Delay in TracingToolTip.
		DWORD dwDrawStartTime = timeGetTime();
		if( dwDrawStartTime - m_dwDrawStartTime < 100) return FALSE;
		m_dwDrawStartTime = dwDrawStartTime;

		strRQ = szMainRQ;
	}
	else
	{
		// (2007/2/9 - Seung-Won, Bae) Check Region for TracingToolTip.
		ILPCSTR szGraphRQ = NULL;
		szGraphName = m_pIChartManager->FindGraph( point);
		// (2007/2/9 - Seung-Won, Bae) Check Graph for Mouse Tooltip.
		if( szGraphName.IsEmpty())
		{
			m_toolTip.Hide();
			return FALSE;
		}
			
		szGraphRQ = m_pIChartManager->GetRQofGraph( szGraphName);
		strRQ = szGraphRQ;
	}

	// ToolTip 이상현상 수정 - ojtaso (20080612)
	// (2008/10/19 - Seung-Won, Bae) Do tooltip with one RQ and Block.
	//	CString szRQ = m_pIChartManager->FindGraphRQ(point);
	//	if(m_bTracingToolTip)
	//		DrawTooltipText("", point);
	//	else
	//		DrawTooltipText(szRQ, point);
	TRACE( "\r\nRQ:[%s]", strRQ);
	DrawTooltipText( strRQ, point);

	return FALSE;
}

// XScale Text 툴팁표시 - ojtaso (20080429)
BOOL CToolTipAddInImp::DrawTooltipHorzScaleText(CString szRQ, CPoint point, int nTextSize, int nHeight)
{
	m_toolTip.ResetText();
	// 데이타 가져오기.

	CString strScaleText;
	if(!m_niDataQueryer.ParsingInquiryHorzScaleTextData(szRQ, point, strScaleText)) return FALSE;

	if(strScaleText.IsEmpty())
		return FALSE;

	strScaleText.TrimRight();

	setlocale( LC_CTYPE, "Korean" );	// 지역을 한국으로 설정
	int nTextLength = _mbstrlen(strScaleText);

	int nAllTextSize = nTextLength * nTextSize;
	if(nAllTextSize < nHeight)
	{
		m_toolTip.Hide();
		return FALSE;
	}

	m_toolTip.AddText(strScaleText);
	m_pChartCWnd->ClientToScreen( &point);
	m_toolTip.SetNewTip( m_pChartCWnd, point);
	return TRUE;
}

/*
BOOL CToolTipAddInImp::DrawTooltipText(CString szRQ, CPoint point)
{
	m_toolTip.ResetText();

	// 데이타 가져오기.
	// (2008/9/14 - Seung-Won, Bae) for Pair Tick
	if( 5 < szRQ.GetLength()) if( szRQ.Right( 5) == "_PAIR") szRQ = szRQ.Left( szRQ.GetLength() - 5);
	if( !m_niDataQueryer.ParsingInquiryData(szRQ, point, true)) return FALSE;
	szRQ = szRQ + "_PAIR";
	m_niDataQueryer.ParsingInquiryData( szRQ, point, true, TRUE);

	CInquiryItem *listData = m_niDataQueryer.GetHead();
	if( !listData) return FALSE;

	// 수치조회, 툴팁에 등락률 추가 표시 - ojtaso (20080710)
	CInquiryItem *listPrevData = m_niDataQueryer.GetPrevHead();
	if( !listPrevData) return FALSE;

	// 각 컬럼의 최대길이 구해오기. 데이타의 줄맞춤을 위해.
	int nFirstColumnMaxLength = 0;
	int nSecondColumnMaxLenght = 0;
	m_niDataQueryer.GetInquiryDataMaxLength( listData, nFirstColumnMaxLength, nSecondColumnMaxLenght);
	
	// 수치조회, 툴팁에 등락률 추가 표시 - ojtaso (20080710)
	POSITION posPrevRow = listPrevData->GetHeadPosition();
	CRowData *pRowPrevData = NULL;
	CString strTempData;
	double dPrevItemData = 0.0, dCurItemData = 0.0;// 20100804 강부수 이동평균비율 현재가로 비교 >>
	double dPreveVolData = 0.0;		//110116alzioyes:툴팁 거래량에 등락추가.
	while( posPrevRow)
	{
		pRowPrevData = listPrevData->GetNext( posPrevRow);
		
		strTempData = pRowPrevData->m_strData;
		strTempData.Replace(",","");

		//110116alzioyes:거래량도 전일거래량과 비교하도록 수정.
// 		if(pRowPrevData != NULL && 
// 			(pRowPrevData->m_strName == "종가" ||
// 			 pRowPrevData->m_strName == "가격"))
// 		{
// 			strTempData = pRowPrevData->m_strData;
// 			strTempData.Replace(",","");
// 			dPrevItemData = atof(strTempData);
// 		}
		if(pRowPrevData != NULL)
		{
			if(pRowPrevData->m_strName == "종가" ||
			 pRowPrevData->m_strName == "가격")
			{
				strTempData = pRowPrevData->m_strData;
				strTempData.Replace(",","");
				dPrevItemData = atof(strTempData);
			}
			//110116alzioyes:툴팁 거래량에 등락추가.
			else if(pRowPrevData->m_strName == "거래량")
			{
				strTempData = pRowPrevData->m_strData;
				//strTempData.Replace(",","");
				strTempData.Remove(',');
				dPreveVolData = atof(strTempData);
			}
		}
	}

	nSecondColumnMaxLenght += 5;

	// 수치조회, 툴팁에 등락률 추가 표시 - ojtaso (20080710)
	CString strItemData;
	double dItemData = 0.0;
	double dItemRatio = 0.0;
	BOOL bFindPrice = FALSE, bIsNumeric;// 20100804 강부수 이동평균비율 현재가로 비교 >>

	// 툴팁에 데이타 세팅.
	CRowData *pRowData = NULL;
	POSITION posRow = listData->GetHeadPosition();

	BOOL bPreFound = FALSE;
	CString szPreData;
	//--> 선물전일대비일때 전일날자 셋팅을 위한 부분.
	while( posRow)
	{
		pRowData = listData->GetNext( posRow);
		if( pRowData->m_strName.GetLength() == 0)
		{
			continue;
		}

		CString strSecondData = CDataConversion::InsertString( pRowData->m_strData, _T(" "), nSecondColumnMaxLenght -  pRowData->m_strData.GetLength(), 1);
		if(strSecondData.GetLength()>5)
		{
			CString szTmp = strSecondData.Left(5);
			if(szTmp.Compare("!Pre!")==0)
			{
				szPreData = strSecondData.Mid(5, 8);
				bPreFound = TRUE;
				break;
			}
		}		
	}
	//<--

	// 비어있는 툴팁 제거 - ojtaso (20080625)
	posRow = listData->GetHeadPosition();
	if(posRow)
	{
		m_toolTip.AddText(_T(" "));

		// 20100804 강부수 이동평균비율 현재가로 비교 >>
		// 툴팁에 데이타 세팅.
		CRowData *pRowData = NULL;
		while( posRow)
		{
			pRowData = listData->GetNext( posRow);

			// 타이틀이 없는 문자열은 빈줄만 삽입.
			if( pRowData->m_strName.GetLength() == 0)
			{
				m_toolTip.AddText(_T(" "));
				continue;
			}

			if(atoi(pRowData->m_strName) > 0) bIsNumeric = TRUE;
			else bIsNumeric = FALSE;
			
			if(pRowData->m_strName == "종가") {
				strTempData = pRowData->m_strData;
				strTempData.Replace(",","");
				
				dCurItemData = atof(strTempData);
				bFindPrice = TRUE;
			}
			else if(bFindPrice && bIsNumeric)
				;
			else {
				bFindPrice = FALSE;
			}

			CString strFirstData;
			CString strSecondData;

			if( pRowData->m_strName == "시가" ||
				 pRowData->m_strName == "고가" ||
				 pRowData->m_strName == "저가" ||
				 pRowData->m_strName == "종가" ||
				 pRowData->m_strName == "가격" ||
				 (bFindPrice && bIsNumeric) )
			{
				strTempData = pRowData->m_strData;
				strTempData.Replace(",","");
				
				dItemData = atof(strTempData);
				
				if(strTempData.Right(1) == _T("%"))
					strItemData = pRowData->m_strData;
				else
				{
					if(bFindPrice && bIsNumeric)
						dItemRatio = (dItemData-dCurItemData)/fabs(dCurItemData)*100;
					else
						dItemRatio = (dItemData-dPrevItemData)/fabs(dPrevItemData)*100;
					strTempData.Format(" (%.2f%%)",dItemRatio);
					
					strItemData = pRowData->m_strData + strTempData;
				}				

				// 툴팁이 이쁘게 보이기 위해 콤마(:)줄을 맞춘다.
				strFirstData = CDataConversion::InsertString( pRowData->m_strName, _T(" "), nFirstColumnMaxLength -  pRowData->m_strName.GetLength(), 1);
				strSecondData = CDataConversion::InsertString( strItemData, _T(" "), nSecondColumnMaxLenght -  strItemData.GetLength(), 1);
				strFirstData = CDataConversion::InsertString(strFirstData, _T(" "), 1, 0);
				
				//m_toolTip.AddText(strFirstData + _T(" : ") + strSecondData);
			}
			//110116alzioyes:툴팁 거래량에 등락추가. -->
			else if(pRowData->m_strName == "거래량")
			{
				if(dPreveVolData==0.0)
				{
					strItemData = pRowData->m_strData;
				}
				else
				{
					strTempData = pRowData->m_strData;
					strTempData.Remove(',');
					dItemData = atof(strTempData);

					dItemRatio = ((dItemData-dPreveVolData)/fabs(dPreveVolData))*100;
					strTempData.Format(" (%.2f%%)",dItemRatio);
					strItemData = pRowData->m_strData + strTempData;
				}

				// 툴팁이 이쁘게 보이기 위해 콤마(:)줄을 맞춘다.
				strFirstData = CDataConversion::InsertString( pRowData->m_strName, _T(" "), nFirstColumnMaxLength -  pRowData->m_strName.GetLength(), 1);
				strSecondData = CDataConversion::InsertString( strItemData, _T(" "), nSecondColumnMaxLenght -  pRowData->m_strData.GetLength(), 1);
				strFirstData = CDataConversion::InsertString(strFirstData, _T(" "), 1, 0);
			}
			//110116alzioyes:툴팁 거래량에 등락추가.<--
			// 20100804 강부수 <<	
			else
			{
				// 툴팁이 이쁘게 보이기 위해 콤마(:)줄을 맞춘다.
				strFirstData = CDataConversion::InsertString( pRowData->m_strName, _T(" "), nFirstColumnMaxLength -  pRowData->m_strName.GetLength(), 1);
				strSecondData = CDataConversion::InsertString( pRowData->m_strData, _T(" "), nSecondColumnMaxLenght -  pRowData->m_strData.GetLength(), 1);
				strFirstData = CDataConversion::InsertString(strFirstData, _T(" "), 1, 0);
				
				//m_toolTip.AddText(strFirstData + _T(" : ") + strSecondData);
			}

//@Solomon-->
// "시간"이란 타이틀로 설정시에는 시간툴팁이 이상하므로 처리함.
// 현재는 "일자"라야 하는데, "시간"이란 이름으로 표시하길 원하기 때문임.
			{ 
				CString szTmp1(strFirstData);
				szTmp1.TrimLeft(); szTmp1.TrimRight();
				if(szTmp1.Compare("시간")==0)
				{
					strSecondData.TrimRight();

					if(bPreFound)
					{
						strSecondData = strSecondData.Mid(5);
						strSecondData.Insert(0, szPreData.Mid(4));
						strSecondData.Insert(2, "/");
					}

					if(strSecondData.GetLength()==5)
					{
						strSecondData.Insert(1, ":");
						strSecondData.Insert(4, ":");
					}
					else if(strSecondData.GetLength()==6)
					{
						strSecondData.Insert(2, ":");
						strSecondData.Insert(5, ":");
					}
					else if(strSecondData.GetLength()==8)
					{
						strSecondData.Insert(2, "/");
						strSecondData.Insert(5, ":");
						strSecondData.Insert(8, ":");
					}
				}
			}
//@Solomon<--

			m_toolTip.AddText(strFirstData + _T(" : ") + strSecondData);

		}
		
		m_toolTip.AddText(_T(" "));
		m_pChartCWnd->ClientToScreen( &point);
		m_toolTip.SetNewTip( m_pChartCWnd, point);
	}
	return TRUE;
}
*/

//>>[A00000441]풍선도움말 보기시 전체 조회 옵션 추가
BOOL CToolTipAddInImp::DrawTooltipText(CString szRQ, CPoint point)
{
	m_toolTip.ResetText();

	// 데이타 가져오기.
	// (2008/9/14 - Seung-Won, Bae) for Pair Tick
	if( 5 < szRQ.GetLength()) if( szRQ.Right( 5) == "_PAIR") szRQ = szRQ.Left( szRQ.GetLength() - 5);
	//<<[A00000196]110407풍선도움말 보기시 전체 조회 옵션 추가
	//if( !m_niDataQueryer.ParsingInquiryData(szRQ, point, true)) return FALSE;
	if( !m_niDataQueryer.ParsingInquiryData(szRQ, point, true, FALSE, m_bMouseToolTipAll)) return FALSE;
	//>>
	szRQ = szRQ + "_PAIR";
	//<<[A00000196]110407 풍선도움말 보기시 전체 조회 옵션 추가
	//m_niDataQueryer.ParsingInquiryData( szRQ, point, true, TRUE);
	m_niDataQueryer.ParsingInquiryData( szRQ, point, TRUE, TRUE, m_bMouseToolTipAll);
	//>>

	CInquiryItem *listData = m_niDataQueryer.GetHead();
	if( !listData) return FALSE;

	// 수치조회, 툴팁에 등락률 추가 표시 - ojtaso (20080710)
	CInquiryItem *listPrevData = m_niDataQueryer.GetPrevHead();
	if( !listPrevData) return FALSE;

	// 각 컬럼의 최대길이 구해오기. 데이타의 줄맞춤을 위해.
	int nFirstColumnMaxLength = 0;
	int nSecondColumnMaxLenght = 0;
	m_niDataQueryer.GetInquiryDataMaxLength( listData, nFirstColumnMaxLength, nSecondColumnMaxLenght);
	
	// 수치조회, 툴팁에 등락률 추가 표시 - ojtaso (20080710)
	POSITION posPrevRow = listPrevData->GetHeadPosition();
	CRowData *pRowPrevData = NULL;
	CString strTempData;
	double dPrevItemData = 0.0, dCurItemData = 0.0;// 20100804 강부수 이동평균비율 현재가로 비교 >>
	double dPreveVolData = 0.0;		//110116alzioyes:툴팁 거래량에 등락추가.
	while( posPrevRow)
	{
		pRowPrevData = listPrevData->GetNext( posPrevRow);
		
		strTempData = pRowPrevData->m_strData;
		strTempData.Replace(",","");

		//110116alzioyes:거래량도 전일거래량과 비교하도록 수정.
// 		if(pRowPrevData != NULL && 
// 			(pRowPrevData->m_strName == "종가" ||
// 			 pRowPrevData->m_strName == "가격"))
// 		{
// 			strTempData = pRowPrevData->m_strData;
// 			strTempData.Replace(",","");
// 			dPrevItemData = atof(strTempData);
// 		}
		if(pRowPrevData != NULL)
		{
			if(pRowPrevData->m_strName == "종가" ||
			 pRowPrevData->m_strName == "가격")
			{
				strTempData = pRowPrevData->m_strData;
				strTempData.Replace(",","");
				dPrevItemData = atof(strTempData);
			}
			//110116alzioyes:툴팁 거래량에 등락추가.
			else if(pRowPrevData->m_strName == "거래량")
			{
				strTempData = pRowPrevData->m_strData;
				//strTempData.Replace(",","");
				strTempData.Remove(',');
				dPreveVolData = atof(strTempData);
			}
		}
	}

	nSecondColumnMaxLenght += 5;

	// 수치조회, 툴팁에 등락률 추가 표시 - ojtaso (20080710)
	CString strItemData;
	double dItemData = 0.0;
	double dItemRatio = 0.0;
	double dItemContrast = 0.0;
	CString strConSymbol;
	BOOL bFindPrice = FALSE, bIsNumeric;// 20100804 강부수 이동평균비율 현재가로 비교 >>

	// 툴팁에 데이타 세팅.
	CRowData *pRowData = NULL;
	POSITION posRow = listData->GetHeadPosition();

	BOOL bPreFound = FALSE;
	CString szPreData;
	//--> 선물전일대비일때 전일날자 셋팅을 위한 부분.
	while( posRow)
	{
		pRowData = listData->GetNext( posRow);
		if( pRowData->m_strName.GetLength() == 0)
		{
			continue;
		}

		CString strSecondData = CDataConversion::InsertString( pRowData->m_strData, _T(" "), nSecondColumnMaxLenght -  pRowData->m_strData.GetLength(), 1);
		if(strSecondData.GetLength()>5)
		{
			CString szTmp = strSecondData.Left(5);
			if(szTmp.Compare("!Pre!")==0)
			{
				szPreData = strSecondData.Mid(5, 8);
				bPreFound = TRUE;
				break;
			}
		}		
	}
	//<--

	// 비어있는 툴팁 제거 - ojtaso (20080625)
	posRow = listData->GetHeadPosition();
	if(posRow)
	{
		m_toolTip.AddText(_T(" "));

		// 20100804 강부수 이동평균비율 현재가로 비교 >>
		// 툴팁에 데이타 세팅.
		CRowData *pRowData = NULL;
		while( posRow)
		{
			pRowData = listData->GetNext( posRow);

			// 타이틀이 없는 문자열은 빈줄만 삽입.
			if( pRowData->m_strName.GetLength() == 0)
			{
				m_toolTip.AddText(_T(" "));
				continue;
			}

			if(atoi(pRowData->m_strName) > 0) bIsNumeric = TRUE;
			else bIsNumeric = FALSE;
			
			if(pRowData->m_strName == "종가") {
				strTempData = pRowData->m_strData;
				strTempData.Replace(",","");
				
				dCurItemData = atof(strTempData);
				bFindPrice = TRUE;
			}
			else if(bFindPrice && bIsNumeric)
				;
			else {
				bFindPrice = FALSE;
			}

			CString strFirstData;
			CString strSecondData;

			if( pRowData->m_strName == "시가" ||
				 pRowData->m_strName == "고가" ||
				 pRowData->m_strName == "저가" ||
				 pRowData->m_strName == "종가" ||
				 pRowData->m_strName == "가격" ||
				 (bFindPrice && bIsNumeric) )
			{
				strTempData = pRowData->m_strData;
				strTempData.Replace(",","");
				
				dItemData = atof(strTempData);
				
				if(strTempData.Right(1) == _T("%"))
					strItemData = pRowData->m_strData;
				else
				{
					if(bFindPrice && bIsNumeric)
					{
                                                if (dCurItemData != 0) //tour2k
						dItemRatio = (dItemData-dCurItemData)/fabs(dCurItemData)*100;
						dItemContrast = dItemData-dCurItemData;
					}
					else
					{
                                                if (dPrevItemData != 0) //tour2k
						dItemRatio = (dItemData-dPrevItemData)/fabs(dPrevItemData)*100;
						dItemContrast = dItemData-dPrevItemData;
					}
					
					if( dItemContrast == 0 )
						strConSymbol = _T("   ");
					else if( dItemContrast > 0 )
						strConSymbol = _T("▲");
					else
						strConSymbol = _T("▼");
					dItemContrast = fabs( dItemContrast );					

					strTempData.Format(" (%.2f%%)",dItemRatio);

					if(bFindPrice && bIsNumeric)
						strItemData = pRowData->m_strData + strTempData;							
					else
					{
						if( pRowData->m_strData.Find( "." ) > - 1 )
							strItemData.Format("%s %s%.2f%s", pRowData->m_strData, strConSymbol, dItemContrast, strTempData);
						else
							strItemData.Format("%s %s%.0f%s", pRowData->m_strData, strConSymbol, dItemContrast, strTempData);
					}
				}				

				// 툴팁이 이쁘게 보이기 위해 콤마(:)줄을 맞춘다.
				strFirstData = CDataConversion::InsertString( pRowData->m_strName, _T(" "), nFirstColumnMaxLength -  pRowData->m_strName.GetLength(), 1);
				strSecondData = CDataConversion::InsertString( strItemData, _T(" "), nSecondColumnMaxLenght -  strItemData.GetLength(), 1);
				strFirstData = CDataConversion::InsertString(strFirstData, _T(" "), 1, 0);
				
				//m_toolTip.AddText(strFirstData + _T(" : ") + strSecondData);
			}
			//110116alzioyes:툴팁 거래량에 등락추가. -->
			else if(pRowData->m_strName == "거래량")
			{
				if(dPreveVolData==0.0)
				{
					strItemData = pRowData->m_strData;
				}
				else
				{
					strTempData = pRowData->m_strData;
					strTempData.Remove(',');
					dItemData = atof(strTempData);

                                        if (dPreveVolData != 0 )//tour2k
					dItemRatio = ((dItemData-dPreveVolData)/fabs(dPreveVolData))*100;
					strTempData.Format(" (%.2f%%)",dItemRatio);
					strItemData = pRowData->m_strData + strTempData;
				}

				// 툴팁이 이쁘게 보이기 위해 콤마(:)줄을 맞춘다.
				strFirstData = CDataConversion::InsertString( pRowData->m_strName, _T(" "), nFirstColumnMaxLength -  pRowData->m_strName.GetLength(), 1);
				strSecondData = CDataConversion::InsertString( strItemData, _T(" "), nSecondColumnMaxLenght -  pRowData->m_strData.GetLength(), 1);
				strFirstData = CDataConversion::InsertString(strFirstData, _T(" "), 1, 0);
			}
			//110116alzioyes:툴팁 거래량에 등락추가.<--
			// 20100804 강부수 <<	
			else
			{
				// 툴팁이 이쁘게 보이기 위해 콤마(:)줄을 맞춘다.
				strFirstData = CDataConversion::InsertString( pRowData->m_strName, _T(" "), nFirstColumnMaxLength -  pRowData->m_strName.GetLength(), 1);
				strSecondData = CDataConversion::InsertString( pRowData->m_strData, _T(" "), nSecondColumnMaxLenght -  pRowData->m_strData.GetLength(), 1);
				strFirstData = CDataConversion::InsertString(strFirstData, _T(" "), 1, 0);
				
				//m_toolTip.AddText(strFirstData + _T(" : ") + strSecondData);
			}

//@Solomon-->
// "시간"이란 타이틀로 설정시에는 시간툴팁이 이상하므로 처리함.
// 현재는 "일자"라야 하는데, "시간"이란 이름으로 표시하길 원하기 때문임.
			{ 
				CString szTmp1(strFirstData);
				szTmp1.TrimLeft(); szTmp1.TrimRight();
				if(szTmp1.Compare("시간")==0)
				{
					strSecondData.TrimRight();

					if(bPreFound)
					{
						strSecondData = strSecondData.Mid(5);
						strSecondData.Insert(0, szPreData.Mid(4));
						strSecondData.Insert(2, "/");
					}

					if(strSecondData.GetLength()==5)
					{
						strSecondData.Insert(1, ":");
						strSecondData.Insert(4, ":");
					}
					else if(strSecondData.GetLength()==6)
					{
						strSecondData.Insert(2, ":");
						strSecondData.Insert(5, ":");
					}
					else if(strSecondData.GetLength()==8)
					{
						strSecondData.Insert(2, "/");
						strSecondData.Insert(5, ":");
						strSecondData.Insert(8, ":");
					}

					if(m_pIDefaultPacketManager->GetBaseTimeDataType() == CScaleBaseData::HORZ_TICK)
					{
						CString strTemp;
						int nFind = strSecondData.Find('/');
						if(nFind > -1)
						{
							strTemp = strSecondData.Mid(nFind + 1);
							strSecondData = strTemp;
						}
					}
				}
			}
//@Solomon<--

			m_toolTip.AddText(strFirstData + _T(" : ") + strSecondData);

		}
		
		m_toolTip.AddText(_T(" "));
		m_pChartCWnd->ClientToScreen( &point);
		m_toolTip.SetNewTip( m_pChartCWnd, point);
	}
	return TRUE;
}
//<<[A00000441]