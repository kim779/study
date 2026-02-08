// OrderLineAddinImp.cpp: implementation of the COrderLineAddinImp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OrderLineAddInImp.h"
#include "ChartOutsideCustomAddIn.h"
#include <math.h>


#include "../Include_Chart/Dll_Load/IMetaTable.h"		// for _MTEXT()
#include "../Include_Chart/MessageDefine.h"				// for UM_CURSOR_EXCHANGE
#include "../Include_AddIn/AddInCommand.h"					// for EAI_ALL_ON_CHANGE_CURSOR_MODE
#include "../Include_AddIn/I003531/_IAddInToolMgr.h"		// for IAddInToolMgr
#include "../Include_AddIn/I003531/_IChartManager.h"		// for IChartManager
#include "../Include_AddIn/I003531/_IChartOCX.h"			// for IChartOCX
#include "../Include_AddIn/I003531/_IBlock.h"				// for IBlock
#include "../Include_AddIn/I003531/_IPacket.h"			// for IPacket
#include "../Include_AddIn/I003531/_IPacketManager.h"	// for IPacketManager

#define CONCLUSION_YES 0   //체결
#define CONCLUSION_NO  1   //미체결

#define ORDERTYPE_MARKET		0 //시장가
#define ORDERTYPE_ENTRY			1 //지정가
#define ORDERTYPE_CHANGE		2 //정정
#define ORDERTYPE_CANCEL		3 //취소
#define ORDERTYPE_STOP			4 //Stop/Limit 적용
#define ORDERTYPE_LIMIT			5 //Stop/Limit 취소
#define ORDERTYPE_CLEAR			6 //주문청산
#define ORDERTYPE_CON_STOP		7 //미청산 Stop/Limit 적용
#define ORDERTYPE_CON_LIMIT		8 //미청산 Stop/Limit 취소

#define ORDERTYPE_STOP_ONLY		9	 //Stop 적용 미체결
#define ORDERTYPE_LIMIT_ONLY	10	//Limit 적용 미체결
#define ORDERTYPE_CON_STOP_ONLY	11	//Stop 적용 미청산
#define ORDERTYPE_CON_LIMIT_ONLY 12	//Limit 적용 미청산
#define ORDERTYPE_CROSS_ENTRY	 13	//역지정가 
 
#define ORDERTYPE_ENTRY2		14 //지정가(매수/매도 동시에 나오는 주문창을 띄우는 경우)

#define CHAR_TO_VALIDATE(xStr, nSize, oStr, VStr, strFormat) 	memset(xStr, 0x00, sizeof(xStr));	memcpy(xStr, oStr, nSize);\
	strFormat.Format("%%%ds", nSize);\
szTmp.Format(strFormat, tmpVal); VStr = szTmp;
//////////////////////////////////////////////////////
///COrderLine
///////////////////////////////////////////////////////
COrderLine::COrderLine()
{
	m_nIndex        = 0;
	m_eLinePosition = LINE_POS_IN;
	m_bShow			= TRUE;
	m_bMouseOver	= FALSE;
	m_bSelected		= FALSE;
	m_ptStart		= CPoint(0,0);
	m_ptEnd			= CPoint(0,0);
	m_ptTableOffset = CPoint(0,0);

	m_ptStartChgLine= CPoint(0,0);
	m_ptEndChgLine  = CPoint(0,0);
	m_ptBackupTalbeOffset	= CPoint(0,0);
	m_ptLineOffset			= CPoint(0,0);
	m_dOrderPrice	= 0.0;
	m_nPanOffset	= 0;

	m_nSymbol			= 0;
	m_nFlagSellorBuy	= 0;
	m_dOrgOrderAmount	= 0.0;
	m_dAmount			= 0.0;
	m_nOrderType		= 0;
	m_dRate				= 0.0;
	m_strOpenTime		= "";
	m_strExeTime		= "";
	m_nCurClickedPos	= CLICKEDPOS_NONE;

	m_nPenStyle			= PS_DOT;
	m_nPenThickness		= 1;
	m_nPenClr			= RGB(0, 0, 0);
	m_bCon			    = 0;   //미체결
	m_strOrderID		= "";
	m_strPositionID		= "";
	m_IsStop		    = FALSE;
	m_IsLimit		    = FALSE;
	m_clrCurBrush		= RGB(255, 255, 255);
	m_clrOpen			= RGB(145, 145, 145);
	m_clrPend			= RGB(255, 255, 255);
	m_clrStop			= RGB(253,86,68);
	m_clrLimit			= RGB(91, 155, 255);
	m_strStop			= "";
	m_strLimit			= "";
	m_strCustItem1		= "";
	m_strCustItem2		= "";
	m_strCustItem3		= "";
	m_strSide			= "";

	m_ptStartStop = CPoint(0, 0);
	m_ptEndStop = CPoint(0, 0);
	m_ptStartChgStopLine = CPoint(0, 0);
	m_ptEndChgStopLine = CPoint(0, 0);	
	m_ptStartLimit = CPoint(0, 0);
	m_ptEndLimit = CPoint(0, 0);
	m_ptStartChgLimitLine = CPoint(0, 0); 
	m_ptEndChgLimitLine = CPoint(0, 0);
	m_ptStopTableOffset = CPoint(0, 0);
	m_ptLimitTableOffset = CPoint(0, 0);

	m_rctStopLine = CRect(0,0, 0, 0);
	m_rctLimitLine = CRect(0,0, 0, 0);	
	m_rctStopTable = CRect(0,0, 0, 0);
	m_rctLimitTable = CRect(0,0, 0, 0);
	m_rctStopText = CRect(0,0, 0, 0); 
	m_rctLimitText = CRect(0,0, 0, 0);
	m_rct = CRect(0,0, 0, 0);
	m_rctLine = CRect(0,0, 0, 0);
	m_rctTable = CRect(0,0, 0, 0);
	m_rctText = CRect(0,0, 0, 0);
	m_rctBackup = CRect(0,0, 0, 0);
}

COrderLine::COrderLine(CPoint ptPrev, CPoint ptEnd, double dOrderPrice)
{
	m_bShow			= TRUE;
	m_bSelected		= FALSE;
	m_bMouseOver	= FALSE;
	m_ptStart		= ptPrev;
	m_ptEnd			= ptEnd;
	m_dOrderPrice	= dOrderPrice;

	m_nSymbol			= 0;
	m_nFlagSellorBuy	= 0;
	m_dOrgOrderAmount	= 0.;
	m_dAmount			= 0.;
	m_nOrderType		= 0;
	m_dRate				= 0.0;

	m_nPenStyle			= PS_DOT;
	m_nPenThickness     = 1;
	m_nPenClr			= RGB(0, 0, 0);
}

COrderLine::~COrderLine()
{

}


CPoint COrderLine::GetPrevPoint()
{
	return (CPoint)m_ptStart;
}
CPoint COrderLine::GetEndPoint()
{
	return (CPoint)m_ptEnd;
}

void COrderLine::Draw(CDC* pDC)
{
	GetRect();
//	if(!m_bShow)
//		return;

	CPen pen; //주문선
	CPen penEdge; //테두리선

	CBrush brush(RGB(255, 255, 255));
	m_clrCurBrush = RGB(255, 255, 255);

	if(m_bShow)
	{	
		if(m_IsStop)
			m_clrCurBrush = m_clrStop;
		else if(m_IsLimit)
			m_clrCurBrush = m_clrLimit;
		else if(m_bCon)
			m_clrCurBrush = m_clrOpen;
		else if(!m_bCon)
			m_clrCurBrush = m_clrPend;

		pen.CreatePen(m_nPenStyle, m_nPenThickness, RGB(0, 0, 0));
		penEdge.CreatePen(PS_SOLID, m_nPenThickness, RGB(0, 0, 0));
	}
	else
	{
		if(m_IsStop)
			m_clrCurBrush = COLOR_ORDERLINE_STOP2;
		else if(m_IsLimit)
			m_clrCurBrush = COLOR_ORDERLINE_LIMIT2;
		else if(m_bCon)
			m_clrCurBrush = COLOR_ORDERLINE_GRAY;
		else if(!m_bCon)
			m_clrCurBrush = m_clrPend;	

		pen.CreatePen(m_nPenStyle, m_nPenThickness, COLOR_ORDERLINE_GRAY);
		penEdge.CreatePen(PS_SOLID, m_nPenThickness, COLOR_ORDERLINE_GRAY);
	}

	if(brush.DeleteObject())
		brush.CreateSolidBrush(m_clrCurBrush);	
	CBrush* pOldBrush = pDC->SelectObject(&brush);
	CPen *pOldPen = pDC->SelectObject( &pen);	

	if(m_eLinePosition == LINE_POS_IN)
	{		
		if(m_bMouseOver)
		{
			DrawOrderLineOnMouseOver(pDC);
		}
		else
		{
			DrawOrderLine(pDC);
		}
//		pDC->SelectObject( pOldPen);

		CPen *pOldPenEdge = pDC->SelectObject( &penEdge);

		DrawChgLine(pDC);		
		DrawOrderTable(pDC);		
		DrawOrderText(pDC);
		pDC->SelectObject( pOldPenEdge);				
	}
	else
	{	
		COLORREF clrTriangle;
		if(m_IsStop)
			clrTriangle = m_clrStop;
		else if(m_IsLimit)
			clrTriangle = m_clrLimit;
		else if(m_bCon)
			clrTriangle = m_clrOpen;
		else if(!m_bCon)
			clrTriangle = m_clrPend;

 		CPen penTriangle(PS_SOLID, m_nPenThickness, COLOR_ORDERLINE);
 		CBrush brushTriangle(clrTriangle);

		CPen *pOldPen = pDC->SelectObject( &penTriangle);		
		CBrush* pOldBrush = pDC->SelectObject(&brushTriangle);
		
		DrawEscapeTriangle(pDC);
	
		pDC->SelectObject( penTriangle);
		pDC->SelectObject( brushTriangle);
	}
 	pDC->SelectObject( pOldPen);
	pDC->SelectObject( pOldBrush);	
}

void COrderLine::DrawOrderLine(CDC* pDC)
{
	pDC->MoveTo( m_ptStart);
	pDC->LineTo( m_ptEnd);	
}

void COrderLine::DrawOrderLineOnMouseOver(CDC* pDC)
{
//	CPen pen(m_nPenStyle, m_nPenThickness, m_nPenClr);
//	CPen *pOldPen = pDC->SelectObject( &pen);

	pDC->MoveTo( m_ptStart);
	pDC->LineTo( m_ptEnd);

	CPoint ptShadowPrev = m_ptStart;
	CPoint ptShadowEnd  = m_ptEnd;
	
	ptShadowPrev.y -= 1;
	ptShadowEnd.y -= 1;

	pDC->MoveTo( ptShadowPrev);
	pDC->LineTo( ptShadowEnd);

	ptShadowPrev.y += 2;
	ptShadowEnd.y += 2;

	pDC->MoveTo( ptShadowPrev);
	pDC->LineTo( ptShadowEnd);

//	pDC->SelectObject( pOldPen);
}

void COrderLine::DrawChgLine(CDC* pDC)
{
	pDC->MoveTo( m_ptStartChgLine);
	pDC->LineTo( m_ptEndChgLine);
}

void COrderLine::DrawOrderText(CDC* pDC)
{
	CString strText;
	CString strSide;
	if(m_strSide.CompareNoCase("079") == 0) strSide = "매수";
	else if(m_strSide.CompareNoCase("081") == 0) strSide = "매도";
	else if(m_strSide.CompareNoCase("080") == 0) strSide = "매수";//strSide = "환매";
	else if(m_strSide.CompareNoCase("082") == 0) strSide = "매도";//strSide = "전매";

	CString strAmount;
	strAmount.Format("%lf", ceil(m_dAmount));
	long lAmout = atol(strAmount);
	
	if(m_IsStop)
	{
		if(m_bCon)
			strText.Format("Stop<%s>, <%ld>, <%s>", strSide, lAmout, m_strStop);
		else
			strText.Format("Stop<%s>, <%ld>, <%s>", strSide, lAmout, m_strStop);
	}
	else if(m_IsLimit)
	{
		if(m_bCon)
			strText.Format("Limit<%s>, <%ld>, <%s>", strSide, lAmout, m_strLimit);
		else
			strText.Format("Limit<%s>, <%ld>, <%s>", strSide, lAmout, m_strLimit);
	}
	else
	{
		if(m_bCon)
			strText.Format("미청산<%s>, <%ld>, <%s>", strSide, lAmout, m_strRate);
		else
			strText.Format("미체결<%s>, <%ld>, <%s>", strSide, lAmout, m_strRate);
	}

	if(m_bShow)
		pDC->SetTextColor(RGB(0, 0, 0));
	else
		pDC->SetTextColor(RGB(125, 125, 125));
	pDC->SetBkColor(m_clrCurBrush);
	pDC->DrawText(strText,m_rctText,DT_LEFT| DT_VCENTER);
	pDC->SetBkColor(RGB(255, 255, 255));
}

CString COrderLine::GetToolTipText()
{
	CString strToolTip;
	CString strSide;
	if(m_strSide.CompareNoCase("079") == 0) strSide = "매수";
	else if(m_strSide.CompareNoCase("081") == 0) strSide = "매도";
	else if(m_strSide.CompareNoCase("080") == 0) strSide = "매수";//strSide = "환매";
	else if(m_strSide.CompareNoCase("082") == 0) strSide = "매도";//strSide = "전매";
	
	CString strAmount;
	strAmount.Format("%lf", ceil(m_dAmount));
	long lAmout = atol(strAmount);

	CString strOrderID = m_strOrderID;

	int nIndex = 0;
	int nLength = strOrderID.GetLength();
	for(; nIndex < nLength && strOrderID != ""; nIndex++)
	{
		if( strOrderID.Find("0") == 0)
			strOrderID = strOrderID.Mid(1, strOrderID.GetLength() - 1);
		else
			break;
	}
	
	if(m_IsStop)
	{			
		strToolTip.Format("<Stop> \r\n가격 : %s", m_strStop);	
	}
	else if(m_IsLimit)
	{	
		strToolTip.Format("<Limit> \r\n가격 : %s", m_strLimit);	
	}
	else
	{
		if(m_bCon)
			//<<20100407_JS.Kim 솔로몬
			//strToolTip.Format("체결번호 : <%s> \r\n수량       : %ld \r\n환율       : %s \r\n시간       : %s", strOrderID, lAmout, m_strRate, m_strExeTime);
			strToolTip.Format("포지션번호: <%s> \r\n수량         : %ld \r\n환율         : %s \r\n시간         : %s", m_strPositionID, lAmout, m_strRate, m_strExeTime);
			//>>
		else
			strToolTip.Format("주문번호 : <%s> \r\n수량       : %ld \r\n환율       : %s \r\n시간       : %s", strOrderID, lAmout, m_strRate, m_strOpenTime);
	}
	return strToolTip;
}

void COrderLine::DrawOrderTable(CDC* pDC)
{
	pDC->Rectangle(m_rctTable);

	CRect rt = m_rctTable;
	rt.left += 1;
	rt.right -= 1;
	rt.top += 1;
	rt.bottom -= 1;

	CBrush brush(m_clrCurBrush);
	pDC->FillRect(rt, &brush);
}

void COrderLine::DrawEscapeTriangle(CDC* pDC)
{
	CPoint ptTriangle[3];

	if(m_eLinePosition == LINE_POS_OUT_UP)
	{
		ptTriangle[0].x = m_ptEscape_Start.x;
		ptTriangle[0].y = m_ptEscape_Start.y + TRIANGLE_HEIGHT;

		ptTriangle[1].x = m_ptEscape_Start.x + TRIANGLE_SIDE / 2;
		ptTriangle[1].y = m_ptEscape_Start.y ;

		ptTriangle[2].x = m_ptEscape_Start.x + TRIANGLE_SIDE;
		ptTriangle[2].y = m_ptEscape_Start.y + TRIANGLE_HEIGHT;
	}
	else if(m_eLinePosition == LINE_POS_OUT_DOWN)
	{
		ptTriangle[0].x = m_ptEscape_Start.x;
		ptTriangle[0].y = m_ptEscape_Start.y - TRIANGLE_HEIGHT;

		ptTriangle[1].x = m_ptEscape_Start.x + TRIANGLE_SIDE / 2;
		ptTriangle[1].y = m_ptEscape_Start.y;

		ptTriangle[2].x = m_ptEscape_Start.x + TRIANGLE_SIDE;
		ptTriangle[2].y = m_ptEscape_Start.y - TRIANGLE_HEIGHT;
	}
	pDC->Polygon(ptTriangle, 3);
}

void COrderLine::ShowOrderLine(CDC* pDC)
{

}

void COrderLine::HideOrderLine(CDC* pDC)
{
}

BOOL COrderLine::IsInRegion(CPoint pt)  
{
	if(IsOnLine(pt) || IsOnTable(pt))
		return TRUE;
	else 
		return FALSE;
}
BOOL COrderLine::IsOnLine(CPoint pt)    
{
	if(m_rctLine.PtInRect(pt))
		return TRUE;
	else 
		return FALSE;
}
BOOL COrderLine::IsOnTable(CPoint pt)  
{
	if(m_rctTable.PtInRect(pt))
		return TRUE;
	else 
		return FALSE;
}

CRect COrderLine::GetRect()
{
	m_rctLine.left  = m_ptStart.x;
	m_rctLine.right = m_ptEnd.x;
	m_rctLine.top   = m_ptStart.y -1;
	m_rctLine.bottom   = m_ptStart.y + 2;

	m_ptStartChgLine.x  = m_ptStart.x + 10  +  m_ptTableOffset.x;
	m_ptStartChgLine.y  = m_ptStart.y;          
	m_ptEndChgLine.x    = m_ptStartChgLine.x;
	m_ptEndChgLine.y    = m_ptStartChgLine.y - 15 + m_ptTableOffset.y;

	
	m_rctTable.left   = m_ptEndChgLine.x;
	m_rctTable.right  = m_rctTable.left + 190;
	m_rctTable.bottom = m_ptEndChgLine.y + 1;
	m_rctTable.top    = m_rctTable.bottom - 15;

	m_rctText = m_rctTable;
	m_rctText.left += 2;
	m_rctText.right -= 2;
	m_rctText.top += 2;
	m_rctText.bottom -= 2;

	return m_rct;
}

CPoint COrderLine::GetPostion()
{
	CPoint pt(0, 0);
	return pt;
}

void COrderLine::SetPosition(CPoint pt, int nType)
{
	if(m_nCurClickedPos == CLICKEDPOS_ORDERLINE && !m_bCon)
	{
		m_ptStart.y = pt.y ;
		m_ptEnd.y = pt.y ;
	}
	else if(m_nCurClickedPos == CLICKEDPOS_TABLE)
	{
		m_ptTableOffset.x = pt.x - m_ptLBtnDown.x + m_ptBackupTalbeOffset.x;
		m_ptTableOffset.y = pt.y - m_ptLBtnDown.y + m_ptBackupTalbeOffset.y;

	}
}

void COrderLine::MouseLButtonDown(const CPoint &pt)
{
	m_ptLBtnDown = pt;

	if(	IsOnTable(pt) )
		m_nCurClickedPos = CLICKEDPOS_TABLE;
	else if( IsOnLine(pt) )
		m_nCurClickedPos = CLICKEDPOS_ORDERLINE;
	else
		m_nCurClickedPos = CLICKEDPOS_NONE;
}

void COrderLine::MouseLButtonUp(const CPoint &pt)
{
	m_ptBackupTalbeOffset = m_ptTableOffset;
	m_ptLineOffset  = CPoint(0,0);
}

void COrderLine::MouseRButtonUp(const CPoint &pt)
{
	if(	IsOnTable(pt) )
		m_nCurClickedPos = CLICKEDPOS_TABLE;
	else if( IsOnLine(pt) )
		m_nCurClickedPos = CLICKEDPOS_ORDERLINE;
	else
		m_nCurClickedPos = CLICKEDPOS_NONE;
}


//////////////////////////////////////////////////////
///COrderLineAddInImp
///////////////////////////////////////////////////////
COrderLineAddInImp::COrderLineAddInImp(IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase)
	:CChartAddInBase( p_pIChartOCX, p_pIAddInDllBase)
{
	m_bRun		= FALSE;
	m_bComment  = FALSE;
	m_hAPCursor = NULL;
	m_bIsLock = FALSE;
	m_bSplit	= FALSE;

	m_dViewMin = 0;
	m_dViewMax = 0;

	m_bOnSetCaprture = FALSE;
	m_pSelectedOrderLine = NULL;

	m_nCount_EscapeUp	 = 0;
	m_nCount_EscapeDown  = 0;


	//PAN
	m_dwMouseActionID = UINT_MAX;
	m_ptPanStart    = CPoint(-1, -1);
	m_ptPrevPoint   = CPoint(-1, -1);
	m_dPanStart    = 0;
	m_nStartIndex  = 0;
	m_nEndIndex    = 0;
	m_bLButtonDown = FALSE;
	m_dBarWidth    = 0.0;

	//YZOOM
	m_nRow    = -1;
	m_nColumn = -1;
	m_ptYZoomStart = CPoint( -1, -1);
	m_nVScaleIndex = -1;
	m_nScaleHeight = 0;
	m_rctGraphRegion = CRect(0, 0, 0, 0);
	m_ToolTip.m_hWnd = NULL;

	m_nMMGb = 1;		// 매매구분 ( 0:매수, 1:매도 )
}

COrderLineAddInImp::~COrderLineAddInImp()
{
/*	POSITION pos = m_OrderLineList.GetHeadPosition();
	while (pos)
	{
		COrderLine* pLineOrder = (COrderLine*)m_OrderLineList.GetNext(pos);
		delete pLineOrder;
		pLineOrder = NULL;
	}
	m_OrderLineList.RemoveAll();*/
	SetRemove_OrderLine();
	SetRemove_CommentData();
}

// Multiple Item in DLL
CString COrderLineAddInImp::m_strAddInItemName = "ORDER_LINE";

//////////////////////////////////////////////////////////////////////
// Define AddIn Event Map
//////////////////////////////////////////////////////////////////////
BEGIN_ADDIN_EVENT_MAP( COrderLineAddInImp)
	ONADDINEVENT( OnDrawAfterGraph)
	ONADDINEVENT( OnDrawBeforeGraph)
	ONADDINEVENT( DrawCursor)
	ONADDINEVENT( OnResetChart)
	ONADDINEVENT( OnMouseMove)
	ONADDINEVENT( OnLButtonDown)
	ONADDINEVENT( OnLButtonUp)
	ONADDINEVENT( OnLButtonDblClk)
	ONADDINEVENT( OnRButtonUp)
	ONADDINEVENT( OnToolCommand )
	ONADDINEVENT( OnResetChart )
	ONADDINEVENT( OnRealData)
//	ONADDINEVENT( OnPacketData)
	ONADDINEVENT( OnUpdatePacketData)	
END_ADDIN_EVENT_MAP()

void COrderLineAddInImp::OnDrawAfterGraph( HDC p_hDC )
{
	if( !m_bRun && !m_bComment )
		return;

	// 2. "특수차트"일 때에는 보이지 않게 한다.
	if( m_pIChartOCX->IsOnSpecialChart()) return;

	// 3. Mouse의 position으로 Block의 영역을 구한다.
	GetGraphVScaleRegion();

	CDC *pDC = CDC::FromHandle( p_hDC);
	if( !pDC) return;
	if( !m_pIChartOCX->IsUseToolScrollBar() || m_pIChartOCX->IsOnSpecialChart() || m_pIChartManager->IsEmpty())
		return;
	
	// 3. 가격차트의 정보를 얻어온다.
	int nR, nC, nV, nG;
	ILPCSTR szGraphName = m_pIChartManager->FindFirstIndicator( _MTEXT( C2_PRICE_CHART), nR, nC, nV, nG, "DEFAULT");
	if( szGraphName.IsEmpty()) return ;
	
	// 4. Block 정보를 얻어온다.
	IBlock *pIBlock = NULL;
	pIBlock = m_pIChartManager->GetBlock( nR, nC );
	if( !pIBlock )
		 return;
		
	// 5. Block에서 GraphResion을 얻어온다.
	CRect rctNoFrameAndMargin;
	pIBlock->GetGraphRegions(m_rctGraphRegion, rctNoFrameAndMargin);
	if( m_rctGraphRegion.IsRectEmpty() )
		return;

	if( m_bRun )
	{
		CheckAllTypeLineAndDoFunc(&m_OrderLineList, pDC);
		CheckAllTypeLineAndDoFunc(&m_OrderLineStopList, pDC);
		CheckAllTypeLineAndDoFunc(&m_OrderLineLimitList, pDC);

		CPoint pt;

		CBrush  brush(RGB(255, 255, 255));
		CBrush* pOldBrush = (CBrush*)pDC->SelectObject(&brush);
		GetCursorPos(&pt);	
		ScreenToClient(m_pChartCWnd->GetSafeHwnd(), &pt);
		DrawVertText(p_hDC, pt);
		DrawHorzText(p_hDC, pt);
		if(m_rctGraphRegion.PtInRect(pt))
			DrawCrossLine(p_hDC, pt);

		pDC->SelectObject(pOldBrush);
	}


	if( m_bComment )
	{
		if (m_arrCommentList.GetSize() <= 0)
			return;

		CFont *pOldFont = NULL;
		if (m_boldFont.GetSafeHandle() == NULL)
		{
			pOldFont = pDC->GetCurrentFont();
			LOGFONT lf;
			pOldFont->GetLogFont(&lf);
			lf.lfWeight = FW_BOLD;
			m_boldFont.CreateFontIndirect(&lf);
		}

		pOldFont = pDC->SelectObject(&m_boldFont);

 		IPacket *pIPacketForLow = m_pIDefaultPacketManager->GetPacket(_MTEXT( C0_LOW));
		IPacket *pIPacketForHigh = m_pIDefaultPacketManager->GetPacket(_MTEXT( C0_HIGH));

		if ((pIPacketForLow == NULL) || (pIPacketForHigh == NULL))
			return;

		try
		{
			CPoint point;
			CString strDate;
			double dPrice;

			CCommentItem* pObj = NULL;
			int nIndex=0, nBeforeIdx=0, nBuySum=0, nSellSum=0;
			int nSize = m_arrCommentList.GetSize();
			for (int i=0; i<nSize; i++)
			{
				pObj = m_arrCommentList.GetAt(i);
				if (pObj->m_nIndex < 0)
					continue;

				point.x = m_pIChartManager->GetXPosition( "DEFAULT", ( int)pObj->m_nIndex, 0);

				if (!pObj->m_bBuy)
				{
					if (!pIPacketForHigh->GetData(pObj->m_nIndex, dPrice))
						throw this;
				}
				else
				{
					if (!pIPacketForLow->GetData(pObj->m_nIndex, dPrice))
						throw this;
				}

				point.y = m_pIChartOCX->ConvertDataToYPosition(dPrice, m_dViewMax, m_dViewMin,
							m_rctGraphDrawingRegion.top, m_rctGraphDrawingRegion.bottom, m_bLog, m_bReverse);

				pObj->SetPosition(pDC, &point);
				pObj->Draw(pDC);
			}
		}
		catch (CException*)
		{
			pIPacketForLow->Release();
			pIPacketForHigh->Release();

			return;
		}

		pDC->SelectObject(pOldFont);
		pIPacketForLow->Release();
		pIPacketForHigh->Release();
	}
}

void COrderLineAddInImp::OnDrawBeforeGraph( HDC p_hDC)
{
	if( !m_bRun )	return;	
}

RECT COrderLineAddInImp::DrawCursor( HDC p_hDC)
{
	CRect rt;
	rt.SetRectEmpty();
	
	if( !m_bRun )	return rt;

	return rt;
}

BOOL COrderLineAddInImp::IsPointInLine(CObList* obOrderLineList, CPoint point)
{
	COrderLine* pOrderLine_Find = NULL;
	POSITION pos = obOrderLineList->GetHeadPosition();
	BOOL bOnLine = FALSE;
	while(pos)
	{
		pOrderLine_Find = (COrderLine*)obOrderLineList->GetNext(pos);
		if(pOrderLine_Find && pOrderLine_Find->IsInRegion(point))
		{
			bOnLine = TRUE;
			m_pSelectedOrderLine = pOrderLine_Find;
			m_pSelectedOrderLine->MouseLButtonDown(point);

			m_bOnSetCaprture = TRUE;
			SetMouseCursor(IDC_MOVE4WAY);

			if(m_pSelectedOrderLine)
				m_pSelectedOrderLine->m_bSelected = FALSE;
			POSITION posCur = m_OrderLineList.Find((CObject*)m_pSelectedOrderLine);
			if(!posCur) continue;

			obOrderLineList->RemoveAt(posCur);
			obOrderLineList->AddTail((CObject*)m_pSelectedOrderLine);
			m_pSelectedOrderLine->m_bSelected = TRUE;				
			break;
		}
	}	
	return bOnLine;
}

BOOL COrderLineAddInImp::OnLButtonDown( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	// 1. 현재 실행해야될지 확인하고 실행해야 될 경우에만 실행한다.
	if(!m_bRun)		return FALSE;

	// 2. Check Empty Chart!
	if( !m_pIChartOCX->IsUseToolScrollBar() || m_pIChartOCX->IsOnSpecialChart() || m_pIChartManager->IsEmpty())
		return FALSE;

	// 3. 가격차트의 정보를 얻어온다.
	int nR, nC, nV, nG;
	ILPCSTR szGraphName = m_pIChartManager->FindFirstIndicator( _MTEXT( C2_PRICE_CHART), nR, nC, nV, nG, "DEFAULT");
	if( szGraphName.IsEmpty()) return FALSE;

	// 4. Block 정보를 얻어온다.
	IBlock *pIBlock = NULL;
	pIBlock = m_pIChartManager->GetBlock( nR, nC );
	if( !pIBlock )
		return FALSE;
	
	// 5. Block에서 GraphResion을 얻어온다.
	CRect rctNoFrameAndMargin;
	pIBlock->GetGraphRegions(m_rctGraphRegion, rctNoFrameAndMargin);
	if( m_rctGraphRegion.IsRectEmpty() )
		return FALSE;

	// 6. Block에서의 Graph Resion을 구한다.
	m_rctGraphDrawingRegion = pIBlock->GetGraphDrwingRegion();
	if( m_rctGraphDrawingRegion.IsRectEmpty() )
		return FALSE;

	// 7. 가격차트의 Block에 Log/뒤집기 설정값을 확인한다.
	m_bLog = pIBlock->IsLogScale();
	m_bReverse = pIBlock->IsVScaleInvert( nV);
	

	CScaleBaseData::VERTMINMAXTYPE eMinMaxType;
	pIBlock->GetVScaleMinMaxSetting( nV, m_bIsLock, eMinMaxType, m_dViewMin, m_dViewMax);		
	
	m_ptPrevPoint = point;	
	if( m_rctGraphRegion.PtInRect( point) ) //그래프 영역안에서 동작
	{	
		BOOL bOnLine = TRUE;
// 		COrderLine* pOrderLine_Find = NULL;
// 		POSITION pos = m_OrderLineList.GetHeadPosition();
// 		BOOL bOnLine = FALSE;
// 		while(pos)
// 		{
// 			pOrderLine_Find = (COrderLine*)m_OrderLineList.GetNext(pos);
// 			if(pOrderLine_Find && pOrderLine_Find->IsInRegion(point))
// 			{
// 				bOnLine = TRUE;
// 				m_pSelectedOrderLine = pOrderLine_Find;
// 				m_pSelectedOrderLine->MouseLButtonDown(point);
// 				
// 				m_bOnSetCaprture = TRUE;
// 				SetMouseCursor(IDC_MOVE4WAY);
// 
// 				if(m_pSelectedOrderLine)
// 					m_pSelectedOrderLine->m_bSelected = FALSE;
// 				POSITION posCur = m_OrderLineList.Find((CObject*)m_pSelectedOrderLine);
// 				m_OrderLineList.RemoveAt(posCur);
// 				m_OrderLineList.AddTail((CObject*)m_pSelectedOrderLine);
// 				m_pSelectedOrderLine->m_bSelected = TRUE;				
// 				break;
// 			}
// 		}
		if(!IsPointInLine(&m_OrderLineList, point))
			if(!IsPointInLine(&m_OrderLineStopList, point))
				if(!IsPointInLine(&m_OrderLineLimitList, point))
					bOnLine = FALSE;

		if(bOnLine)
			return TRUE;
	}	
	double dYValue;
	dYValue = m_pIChartOCX->ConvertYPositionToData( point.y, m_rctGraphDrawingRegion.top, m_rctGraphDrawingRegion.bottom, m_dViewMax, m_dViewMin, m_bLog, m_bReverse);
	m_nSelecteYValue = point.y;

	
//	BOOL bDoOrder = FALSE;	
	
	if(m_nSelecteYValue < m_rctGraphRegion.top || m_nSelecteYValue > m_rctGraphRegion.bottom)
		return FALSE;

	if(/*bDoOrder &&*/ (m_rctLVScaleRegion.PtInRect( point) || m_rctRVScaleRegion.PtInRect( point)))//주문시 라인 드로우
	{

//신규 주문요청 Test용 클릭시 주문선 생성
//		m_pIChartOCX->OnOrderInfomationChanged(dYValue, 10, ORDERTYPE_ENTRY, "");

// 		COrderLine* pOrderLine = new COrderLine();
// 		pOrderLine->m_ptStart = CPoint(m_rctGraphRegion.left, m_nSelecteYValue);
// 		pOrderLine->m_ptEnd = CPoint(m_rctGraphRegion.right, m_nSelecteYValue);
// 		pOrderLine->m_dOrderPrice = dYValue;
// 
// 		if(m_pSelectedOrderLine)
// 			m_pSelectedOrderLine->m_bSelected = FALSE;
// 		m_pSelectedOrderLine = pOrderLine;
// 		m_OrderLineList.AddTail((CObject*)m_pSelectedOrderLine);
// 		m_pSelectedOrderLine->m_bSelected = TRUE;
// 	
//   		m_pIChartOCX->InvalidateControl();
	}

	pIBlock->Release();
	pIBlock = NULL;	


	//PAN//////////	
	m_bLButtonDown = TRUE;
	if( OnLButtonDownForYZoom( p_hDC, nFlags, point, p_dwMouseActionID))
		return TRUE;

	m_dwMouseActionID = p_dwMouseActionID;
	m_ptPanStart = point;
	// 1. 현재 화면에 보이는 Data의 Start/End Index를 구한다.
	if( !m_pIChartOCX->GetViewStartEndIndex( m_nStartIndex, m_nEndIndex ))
		return FALSE;

	int nDataOnPage = m_nEndIndex - m_nStartIndex + 1;
	m_dBarWidth = double( m_rctGraphDrawingRegion.Width() ) / double( nDataOnPage );

	m_dPanStart = m_pIChartOCX->ConvertYPositionToData( m_ptPanStart.y, m_rctGraphDrawingRegion.top, m_rctGraphDrawingRegion.bottom, m_dViewMax, m_dViewMin, m_bLog, m_bReverse);
	/////////////////
	return TRUE;
}

BOOL COrderLineAddInImp::OnLButtonUp( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	if(!m_bRun)		return FALSE;	

	if(m_bOnSetCaprture)
	{	
		m_hAPCursor = NULL;	
		if( m_pIChartOCX) m_pIChartOCX->SetCursorHandle( m_hAPCursor);
		if(m_pSelectedOrderLine)
			m_pSelectedOrderLine->MouseLButtonUp(point);
		
		if(m_ptPanStart == point)
			return 0;
		m_pIChartOCX->InvalidateControl();
		SetOrderInfomation(point);	
		m_bOnSetCaprture = FALSE;
	}
	
	// 5. Make Real Effect Gap 5 Pixel.
//	if( abs(m_ptPanStart.x - point.x) < m_dBarWidth )
//		return TRUE;

	// 6. Drag가 종료되었슴으로 Mouse Cursor를 변경한다.
//	m_pChartCWnd->SendMessage( UM_CURSOR_EXCHANGE, ( WPARAM)CCursorShape::OBJECT_NONE, NULL);

	m_ptPanStart = CPoint(-1, -1);	
	m_bLButtonDown = FALSE;

	if( OnLButtonUpForYZoom( p_hDC, nFlags, point, p_dwMouseActionID))
		return TRUE;

	return TRUE;
}

BOOL COrderLineAddInImp::OnLButtonDblClk( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	// 1. 현재 실행해야될지 확인하고 실행해야 될 경우에만 실행한다.
	if(!m_bRun)		return FALSE;

	// 2. Check Empty Chart!
	if( !m_pIChartOCX->IsUseToolScrollBar() || m_pIChartOCX->IsOnSpecialChart() || m_pIChartManager->IsEmpty())
		return FALSE;

	// 3. 가격차트의 정보를 얻어온다.
	int nR, nC, nV, nG;
	ILPCSTR szGraphName = m_pIChartManager->FindFirstIndicator( _MTEXT( C2_PRICE_CHART), nR, nC, nV, nG, "DEFAULT");
	if( szGraphName.IsEmpty()) return FALSE;

	// 4. Block 정보를 얻어온다.
	IBlock *pIBlock = NULL;
	pIBlock = m_pIChartManager->GetBlock( nR, nC );
	if( !pIBlock )
		return FALSE;
	
	// 5. Block에서 GraphResion을 얻어온다.
	CRect rctNoFrameAndMargin;
	pIBlock->GetGraphRegions(m_rctGraphRegion, rctNoFrameAndMargin);
	if( m_rctGraphRegion.IsRectEmpty() )
		return FALSE;

	// 6. Block에서의 Graph Resion을 구한다.
	m_rctGraphDrawingRegion = pIBlock->GetGraphDrwingRegion();
	if( m_rctGraphDrawingRegion.IsRectEmpty() )
		return FALSE;

	// 7. 가격차트의 Block에 Log/뒤집기 설정값을 확인한다.
	m_bLog = pIBlock->IsLogScale();
	m_bReverse = pIBlock->IsVScaleInvert( nV);
	

	CScaleBaseData::VERTMINMAXTYPE eMinMaxType;
	pIBlock->GetVScaleMinMaxSetting( nV, m_bIsLock, eMinMaxType, m_dViewMin, m_dViewMax);	
	
	
	double dYValue;
	dYValue = m_pIChartOCX->ConvertYPositionToData( point.y, m_rctGraphDrawingRegion.top, m_rctGraphDrawingRegion.bottom, m_dViewMax, m_dViewMin, m_bLog, m_bReverse);
	m_nSelecteYValue = point.y;

	
	if(m_nSelecteYValue < m_rctGraphRegion.top || m_nSelecteYValue > m_rctGraphRegion.bottom)
		return FALSE;

//	m_pIChartOCX->OnOrderInfomationChanged(0, 1, ORDERTYPE_MARKET, "", "", "", "");

	IPacket *pIPacket = m_pIDefaultPacketManager->GetPacket( _MTEXT( C0_CLOSE ));
	if( !pIPacket) return FALSE;
	
	CString strType;
	ILPCSTR szIStr = pIPacket->GetType();
	strType = (LPCTSTR)szIStr;
	
	int nFindIndex = strType.Find(".") + 1;
	strType = strType.Mid(nFindIndex, strType.GetLength() - nFindIndex + 1);
	int nTypeLength = strType.GetLength();				
	
	CString strValue;
	CString strFormat;
	strFormat.Format("%%.%dlf", nTypeLength);
	strValue.Format(strFormat, dYValue);
	double dEntry = atof(strValue);
//	dEntry = floor( (dEntry*pow(10, nTypeLength)+0.5) / pow(10, nTypeLength));
	m_pIChartOCX->OnOrderInfomationChanged(dEntry, 1, ORDERTYPE_ENTRY2, "", "", "", "");


	return TRUE;
}

BOOL COrderLineAddInImp::OnMouseMove( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID, const HBITMAP p_hbmpMainOSB)
{
	if(!m_bRun && !m_bComment)		return FALSE;

	if(m_bSplit)
	{
		m_pChartCWnd->SendMessage( UM_CURSOR_EXCHANGE, ( WPARAM)CCursorShape::BLOCKPOS_SPLITH, NULL);
	}

	if( OnMouseMoveForYZoom( p_hDC, nFlags, point, p_dwMouseActionID, p_hbmpMainOSB))
		return TRUE;	

	BOOL	 bLineOver = FALSE;
//1. 마우스 오버시
//2. 영역 밖일 경우 

	//가격차트가 포함된 Block의 Index를 구한다.
		int nR, nC, nV, nG;
		ILPCSTR szGraphName = m_pIChartManager->FindFirstIndicator( _MTEXT( C2_PRICE_CHART), nR, nC, nV, nG, "DEFAULT");
		if( szGraphName.IsEmpty()) return TRUE;

		IBlock *pIBlock = m_pIChartManager->GetBlock( nR, nC );
		if( !pIBlock) return TRUE;

		CRect rctNoFrameAndMargin;
		pIBlock->GetGraphRegions(m_rctGraphRegion, rctNoFrameAndMargin);
		if( m_rctGraphRegion.IsRectEmpty() )
			return FALSE;
		
		m_rctGraphDrawingRegion	= pIBlock->GetGraphDrwingRegion();
		if( m_rctGraphDrawingRegion.IsRectEmpty() )
			return FALSE;

		// 4. GraphRegion이 넘어서면 영역을 GraphRegion으로 제한한다.
		if( m_rctGraphRegion.top >= point.y )
			point.y = m_rctGraphRegion.top;

		if( m_rctGraphRegion.bottom <= point.y )
			point.y = m_rctGraphRegion.bottom;

		m_bLog = pIBlock->IsLogScale();
		m_bReverse = pIBlock->IsVScaleInvert( nV);
		

		CScaleBaseData::VERTMINMAXTYPE eMinMaxType;
		pIBlock->GetVScaleMinMaxSetting( nV, m_bIsLock, eMinMaxType, m_dViewMin, m_dViewMax);

	if(m_bOnSetCaprture)
	{	
		if(m_pSelectedOrderLine)
			m_pSelectedOrderLine->SetPosition(point);
	}
	else if(m_bLButtonDown) //PAN
	{	
		if( !m_pIAddInToolMgr->IsLButtonExclusive( m_pIAddInDllBase))
		{
			if( p_dwMouseActionID != m_dwMouseActionID)
				return FALSE;		
			if( !m_pIAddInToolMgr->GetLButtonExclusive( m_pIAddInDllBase))
				return FALSE;
		}
		//현재의 point가 Drag를 시작했던 point와 같으면 return한다.
		if( m_ptPanStart == point )
			return TRUE;

		// point와의 차이값으로 이동을 결정한다.
		int nXGap = m_ptPanStart.x - point.x;
		BOOL bInvalidate = FALSE;
		int nXMove = 0;
		if( abs(nXGap) > m_dBarWidth )
		{
			nXMove = (int)(nXGap / m_dBarWidth);

			int nLast;
			m_pIChartOCX->GetMaxScrollRange(nLast);

			if( (m_nStartIndex + nXMove >= 0) && (m_nEndIndex + nXMove <= nLast ) )
				m_pIChartOCX->SetViewStartEndIndex( m_nStartIndex + nXMove, m_nEndIndex + nXMove );
			bInvalidate = TRUE;
		}

		// Block에 설정값을 전달한다.
		if( !m_bIsLock )
		{
			double dDragCur = 0;
			dDragCur = m_pIChartOCX->ConvertYPositionToData( point.y, m_rctGraphDrawingRegion.top, m_rctGraphDrawingRegion.bottom, m_dViewMax, m_dViewMin, m_bLog, m_bReverse);

			if( ( m_dViewMin + ( m_dPanStart - dDragCur ) > 0 ) && ( m_dPanStart - dDragCur ) != 0 )
			{
				m_dViewMin = m_dViewMin + ( m_dPanStart - dDragCur );
				m_dViewMax = m_dViewMax + ( m_dPanStart - dDragCur );
				pIBlock->SetVScaleMinMaxSetting( nV, m_bIsLock,
												 CScaleBaseData::VERTMINMAXTYPE::VERT_MINMAX_OF_USER,
												 m_dViewMin, m_dViewMax);

				bInvalidate = TRUE;
			}
		}
		if( pIBlock )
		pIBlock->Release();
		pIBlock = NULL;
		m_pChartCWnd->SendMessage( UM_CURSOR_EXCHANGE, ( WPARAM)CCursorShape::OBJECT_MOVE, NULL);
		
	}
	else if( m_rctLVScaleRegion.PtInRect( point) || m_rctRVScaleRegion.PtInRect( point))
	{
		SetMouseCursor(IDC_AP_INDICATOR);		
	}
	else
	{	
		if(SetLineStatusOnMouseMove(point))
			SetToolTipText(m_strToolTipText, TRUE);
		else if(SetCommentStatusOnMouseMove(point))
			SetToolTipText(m_strToolTipText, TRUE);
		else
			SetToolTipText(" ", FALSE);

		m_hAPCursor = NULL;	
		if( m_pIChartOCX) m_pIChartOCX->SetCursorHandle( m_hAPCursor);
	}			
	
	m_pIChartOCX->InvalidateControl();


	if(m_strToolTipText.IsEmpty())
		return TRUE;
	
	if(m_ToolTip.m_hWnd == NULL)
		SetToolTipText(m_strToolTipText, true);
	
	MSG msg;
	msg.hwnd = m_pChartCWnd->GetSafeHwnd();
	msg.message = WM_MOUSEMOVE;
	msg.wParam = 0;
	msg.lParam = 0;
	m_ToolTip.RelayEvent(&msg);

	return TRUE;
}

BOOL COrderLineAddInImp::SetLineStatusOnMouseMove(CPoint point)
{
	BOOL bLineOver = FALSE;
	POSITION pos = m_OrderLineList.GetHeadPosition();
	while(pos)
	{
		COrderLine* pOrderLine = (COrderLine*)m_OrderLineList.GetNext(pos);
		if(pOrderLine) 
		{
			if(pOrderLine->IsInRegion(point))
			{
				bLineOver = TRUE;
				pOrderLine->m_bShow = TRUE;
				pOrderLine->m_bMouseOver = TRUE;
				m_strToolTipText = pOrderLine->GetToolTipText();
				break;
			}
		}
	}

	if(!bLineOver)
	{
		pos = m_OrderLineStopList.GetHeadPosition();
		while(pos)
		{
			COrderLine* pOrderLine = (COrderLine*)m_OrderLineStopList.GetNext(pos);
			if(pOrderLine) 
			{
				if(pOrderLine->IsInRegion(point))
				{
					bLineOver = TRUE;
					pOrderLine->m_bShow = TRUE;
					pOrderLine->m_bMouseOver = TRUE;
					m_strToolTipText = pOrderLine->GetToolTipText();
					break;
				}
			}
		}
	}

	if(!bLineOver)
	{		
		pos = m_OrderLineLimitList.GetHeadPosition();
		while(pos)
		{
			COrderLine* pOrderLine = (COrderLine*)m_OrderLineLimitList.GetNext(pos);
			if(pOrderLine) 
			{
				if(pOrderLine->IsInRegion(point))
				{
					bLineOver = TRUE;
					pOrderLine->m_bShow = TRUE;
					pOrderLine->m_bMouseOver = TRUE;
					m_strToolTipText = pOrderLine->GetToolTipText();
					break;
				}
			}
		}
	}


	if(bLineOver)
	{	
		SetOrderLineShowAndMouseOver(pos, &m_OrderLineList, TRUE);
		SetOrderLineShowAndMouseOver(pos, &m_OrderLineStopList, TRUE);
		SetOrderLineShowAndMouseOver(pos, &m_OrderLineLimitList, TRUE);
	}
	else if(!m_bOnSetCaprture)
	{
		SetOrderLineShowAndMouseOver(pos, &m_OrderLineList, FALSE);		
		SetOrderLineShowAndMouseOver(pos, &m_OrderLineStopList, FALSE);
		SetOrderLineShowAndMouseOver(pos, &m_OrderLineLimitList, FALSE);
	}
	
	
	return bLineOver;
}

void COrderLineAddInImp::SetOrderLineShowAndMouseOver(POSITION pos, CObList* obOrderLineList, BOOL bMouseOver)
{
	if(bMouseOver)
	{
		POSITION posOtherLine = obOrderLineList->GetHeadPosition();
		while(posOtherLine)
		{
			COrderLine* potherOrderLine = (COrderLine*)obOrderLineList->GetNext(posOtherLine);
			if(pos != posOtherLine && potherOrderLine ) 
			{
				potherOrderLine->m_bShow = FALSE;
				potherOrderLine->m_bMouseOver = FALSE;
			}			
		}
	}
	else
	{
		POSITION posOtherLine = obOrderLineList->GetHeadPosition();
		while(posOtherLine)
		{
			COrderLine* potherOrderLine = (COrderLine*)obOrderLineList->GetNext(posOtherLine);
			if(potherOrderLine ) 
			{
				potherOrderLine->m_bShow = TRUE;
				potherOrderLine->m_bMouseOver = FALSE;
			}
		}	
	}
}

BOOL COrderLineAddInImp::OnLButtonDownForYZoom( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	if( 0 < m_ptYZoomStart.x && 0 < m_ptYZoomStart.y) return FALSE;

	if( !m_pIChartManager->GetBlockIndex( point, m_nRow, m_nColumn)) return FALSE;
	IBlock *pIBlock = m_pIChartManager->GetBlock( m_nRow, m_nColumn);
	if( !pIBlock) return FALSE;

	CRect rctLeftScale, rctRightScale;
	pIBlock->GetVertScaleRegion( rctLeftScale, rctRightScale);

	m_nVScaleIndex = pIBlock->GetActiveVScaleIndex( rctLeftScale.PtInRect( point) ? CScaleBaseData::VERT_LEFT : CScaleBaseData::VERT_RIGHT);
	if( 0 <= m_nVScaleIndex)
	{
		CScaleBaseData::VERTMINMAXTYPE eMinMaxType;
		pIBlock->GetVScaleMinMaxSetting( m_nVScaleIndex, m_bIsLock, eMinMaxType, m_dViewMin, m_dViewMax);
	}
	pIBlock->Release();

	if( m_bIsLock || m_nVScaleIndex < 0 || ( !rctLeftScale.PtInRect( point) && !rctRightScale.PtInRect( point))) return FALSE;
	m_nScaleHeight = rctLeftScale.Height();
	m_ptYZoomStart = point;

	m_pIAddInToolMgr->GetLButtonExclusive( m_pIAddInDllBase);
	m_bSplit = TRUE;
//	m_pChartCWnd->SendMessage( UM_CURSOR_EXCHANGE, ( WPARAM)CCursorShape::BLOCKPOS_SPLITH, NULL);

	return TRUE;
	}	

BOOL COrderLineAddInImp::OnMouseMoveForYZoom( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID, const HBITMAP p_hbmpMainOSB)
{
	if( m_ptYZoomStart.x < 0 && m_ptYZoomStart.y < 0) return FALSE;
	if( m_nRow < 0 || m_nColumn < 0) return TRUE;

	IBlock *pIBlock = m_pIChartManager->GetBlock( m_nRow, m_nColumn);
	if( !pIBlock) return TRUE;

	double dViewMin = m_dViewMin;
	double dViewMax = m_dViewMax;
	if( m_dViewMin != m_dViewMax && point.y != m_ptYZoomStart.y)
	{
		double dViewRange = ( m_dViewMax - m_dViewMin) / 2;
		double dFactor = pow( 10, ( double( abs(point.y - m_ptYZoomStart.y)) / m_nScaleHeight) / ( 200. / 100.));
		if( m_ptYZoomStart.y < point.y) dViewRange /= dFactor;
		else							dViewRange *= dFactor;

		dViewMin = ( m_dViewMax + m_dViewMin) / 2  - dViewRange;
		dViewMax = ( m_dViewMax + m_dViewMin) / 2  + dViewRange;
	}

	pIBlock->SetVScaleMinMaxSetting( m_nVScaleIndex, m_bIsLock,
									 CScaleBaseData::VERTMINMAXTYPE::VERT_MINMAX_OF_USER,
									 dViewMin, dViewMax);
	pIBlock->Release();

	m_pIChartOCX->InvalidateControl();
	return TRUE;
}

BOOL COrderLineAddInImp::OnLButtonUpForYZoom( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	if( m_ptYZoomStart.x < 0 && m_ptYZoomStart.y < 0) return FALSE;

	m_pIAddInToolMgr->ReleaseLButtonExclusive( m_pIAddInDllBase);
//	m_pChartCWnd->SendMessage( UM_CURSOR_EXCHANGE, ( WPARAM)CCursorShape::OBJECT_NONE, NULL);

	m_ptYZoomStart.x = -1;
	m_ptYZoomStart.y = -1;

	m_bSplit = FALSE;
	return TRUE;
}

BOOL COrderLineAddInImp::OnRButtonUp( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
//	m_bRun = !m_bRun;
//	m_pIChartOCX->RunToolCommand( CToolOptionInfo::T_TOOL_SELECTION, 1);

//	m_pIChartOCX->OnOrderInfomationChanged(0.0, 0, 0, "");

	// 1. 현재 실행해야될지 확인하고 실행해야 될 경우에만 실행한다.
	if(!m_bRun)		return FALSE;

	// 2. Check Empty Chart!
	if( !m_pIChartOCX->IsUseToolScrollBar() || m_pIChartOCX->IsOnSpecialChart() || m_pIChartManager->IsEmpty())
		return FALSE;

	// 3. 가격차트의 정보를 얻어온다.
	int nR, nC, nV, nG;
	ILPCSTR szGraphName = m_pIChartManager->FindFirstIndicator( _MTEXT( C2_PRICE_CHART), nR, nC, nV, nG, "DEFAULT");
	if( szGraphName.IsEmpty()) return FALSE;

	// 4. Block 정보를 얻어온다.
	IBlock *pIBlock = NULL;
	pIBlock = m_pIChartManager->GetBlock( nR, nC );
	if( !pIBlock )
		return FALSE;
	
	// 5. Block에서 GraphResion을 얻어온다.
	CRect rctNoFrameAndMargin;
	pIBlock->GetGraphRegions(m_rctGraphRegion, rctNoFrameAndMargin);
	if( m_rctGraphRegion.IsRectEmpty() )
		return FALSE;

	// 6. Block에서의 Graph Resion을 구한다.
	m_rctGraphDrawingRegion = pIBlock->GetGraphDrwingRegion();
	if( m_rctGraphDrawingRegion.IsRectEmpty() )
		return FALSE;

	// 7. 가격차트의 Block에 Log/뒤집기 설정값을 확인한다.
	m_bLog = pIBlock->IsLogScale();
	m_bReverse = pIBlock->IsVScaleInvert( nV);
	

	CScaleBaseData::VERTMINMAXTYPE eMinMaxType;
	pIBlock->GetVScaleMinMaxSetting( nV, m_bIsLock, eMinMaxType, m_dViewMin, m_dViewMax);	
	
	
	double dYValue;
	dYValue = m_pIChartOCX->ConvertYPositionToData( point.y, m_rctGraphDrawingRegion.top, m_rctGraphDrawingRegion.bottom, m_dViewMax, m_dViewMin, m_bLog, m_bReverse);
	m_nSelecteYValue = point.y;

	
//	BOOL bDoOrder = FALSE;	
	
	if(m_nSelecteYValue < m_rctGraphRegion.top || m_nSelecteYValue > m_rctGraphRegion.bottom)
		return FALSE;

	//<<솔로몬
	m_pSelectedOrderLine = NULL;
	if(!FindLineAndSelect(&m_OrderLineList, point))
		if(!FindLineAndSelect(&m_OrderLineStopList, point))
			FindLineAndSelect(&m_OrderLineLimitList, point);
	//>>

	if(/*bDoOrder &&*/ (m_rctLVScaleRegion.PtInRect( point) || m_rctRVScaleRegion.PtInRect( point))
						|| (!m_pSelectedOrderLine && m_rctGraphRegion.PtInRect( point)) )//주문시 라인 드로우
	{
		CMenu	menu;
		menu.CreatePopupMenu();

		int nID=100;
		/*
		CString strMMGb;
		if( m_nMMGb == 0 )		strMMGb = "매수";
		else					strMMGb = "매도";
		menu.AppendMenu(MF_STRING, nID++, CString("시장가 ") + strMMGb); //menu.AppendMenu(MF_STRING, nID++, "시장가 주문"); 
		menu.AppendMenu(MF_STRING, nID++, CString("지정가 ") + strMMGb); 
		menu.AppendMenu(MF_STRING, nID++, CString("역지정가 ") + strMMGb); 
		*/
		menu.AppendMenu(MF_STRING, nID++, "지정가 매도");
		menu.AppendMenu(MF_STRING, nID++, "지정가 매수");
		menu.AppendMenu(MF_STRING, nID++, "역지정가 매도");
		menu.AppendMenu(MF_STRING, nID++, "역지정가 매수");
		menu.AppendMenu(MF_STRING, nID++, "시장가 매도");
		menu.AppendMenu(MF_STRING, nID++, "시장가 매수");

		UINT flags = TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD;	
		
		CPoint ptPop = point;
		ClientToScreen(m_pChartCWnd->GetSafeHwnd(), &ptPop);
//		int nRet = menu.TrackPopupMenu(flags, point.x + 115, point.y + 260, m_pChartCWnd);
		int nRet = menu.TrackPopupMenu(flags, ptPop.x, ptPop.y, m_pChartCWnd);

//		double d = floor( (dYValue*10)+0.5) / 10);
		menu.DestroyMenu();

		switch(nRet) 
		{		
		case 100:		// 지정가 매도
		case 101:		// 지정가 매수
			{
				IPacket *pIPacket = m_pIDefaultPacketManager->GetPacket( _MTEXT( C0_CLOSE ));
				if( !pIPacket) return FALSE;
				
				CString strType;
				ILPCSTR szIStr = pIPacket->GetType();
				strType = (LPCTSTR)szIStr;
				
				int nFindIndex = strType.Find(".") + 1;
				strType = strType.Mid(nFindIndex, strType.GetLength() - nFindIndex + 1);
				int nTypeLength = strType.GetLength();				
				
				CString strValue;
				CString strFormat;
				strFormat.Format("%%.%dlf", nTypeLength);
				strValue.Format(strFormat, dYValue);
				double dEntry = atof(strValue);
			//	dEntry = floor( (dEntry*pow(10, nTypeLength)+0.5) / pow(10, nTypeLength));
				m_pIChartOCX->OnOrderInfomationChanged(dEntry, 1, ORDERTYPE_ENTRY, "", nRet==100 ? "0" : "1", "", "");
				break;	
			}
		case 102:		// 역지정가 매도
		case 103:		// 역지정가 매수
			{
				IPacket *pIPacket = m_pIDefaultPacketManager->GetPacket( _MTEXT( C0_CLOSE ));
				if( !pIPacket) return FALSE;
				
				CString strType;
				ILPCSTR szIStr = pIPacket->GetType();
				strType = (LPCTSTR)szIStr;
				
				int nFindIndex = strType.Find(".") + 1;
				strType = strType.Mid(nFindIndex, strType.GetLength() - nFindIndex + 1);
				int nTypeLength = strType.GetLength();				
				
				CString strValue;
				CString strFormat;
				strFormat.Format("%%.%dlf", nTypeLength);
				strValue.Format(strFormat, dYValue);
				double dEntry = atof(strValue);
			//	dEntry = floor( (dEntry*pow(10, nTypeLength)+0.5) / pow(10, nTypeLength));
				m_pIChartOCX->OnOrderInfomationChanged(dEntry, 1, ORDERTYPE_CROSS_ENTRY, "", nRet==102 ? "0" : "1", "", "");
				break;
			}
		case 104:		// 시장가 매도
			{						
				m_pIChartOCX->OnOrderInfomationChanged(0, 1, ORDERTYPE_MARKET, "", "0", "", "");
				break;
			}
		case 105:		// 시장가 매수
			{						
				m_pIChartOCX->OnOrderInfomationChanged(0, 1, ORDERTYPE_MARKET, "", "1", "", "");
				break;
			}

		}		
	}
	else
	{
		if( m_rctGraphRegion.PtInRect( point) ) //그래프 영역안에서 동작
		{
			m_pSelectedOrderLine = NULL;
			if(!FindLineAndSelect(&m_OrderLineList, point))
				if(!FindLineAndSelect(&m_OrderLineStopList, point))
					FindLineAndSelect(&m_OrderLineLimitList, point);
// 			COrderLine* pOrderLine_Find = NULL;
// 			POSITION pos = m_OrderLineList.GetHeadPosition();
// 			BOOL bOnLine = FALSE;
// 			while(pos)
// 			{
// 				pOrderLine_Find = (COrderLine*)m_OrderLineList.GetNext(pos);
// 				if(pOrderLine_Find && pOrderLine_Find->IsInRegion(point))
// 				{
// 					bOnLine = TRUE;
// 					m_pSelectedOrderLine = pOrderLine_Find;			
// 
// 					if(m_pSelectedOrderLine)
// 						m_pSelectedOrderLine->m_bSelected = FALSE;
// 					POSITION posCur = m_OrderLineList.Find((CObject*)m_pSelectedOrderLine);
// 					m_OrderLineList.RemoveAt(posCur);
// 					m_OrderLineList.AddTail((CObject*)m_pSelectedOrderLine);
// 					m_pSelectedOrderLine->m_bSelected = TRUE;				
// 					break;
// 				}
// 			}			
			
			if(!m_pSelectedOrderLine)
				return TRUE;

			m_pSelectedOrderLine->MouseRButtonUp(point);
	
			if(m_pSelectedOrderLine->m_nCurClickedPos == CLICKEDPOS_TABLE || m_pSelectedOrderLine->m_nCurClickedPos == CLICKEDPOS_ORDERLINE)
			{			
				CMenu	menu;
				menu.CreatePopupMenu();

				
				int nID=102;
				if( m_pSelectedOrderLine->m_IsStop || m_pSelectedOrderLine->m_IsLimit)
				{
					if(!m_pSelectedOrderLine->m_bCon) //미체결
					{
						menu.AppendMenu(MF_STRING, nID, "주문 정정"); 
						menu.AppendMenu(MF_STRING, nID+1, "주문 취소"); 
//						menu.AppendMenu(MF_STRING, nID+2, "Stop/Limit 적용"); 	
//						menu.AppendMenu(MF_STRING, nID+3, "Stop/Limit 취소"); 
					}
					else                             //미청산
					{
						menu.AppendMenu(MF_STRING, nID+5, "Stop/Limit 적용"); 	
						menu.AppendMenu(MF_STRING, nID+6, "Stop/Limit 취소"); 
					}
				}
				else
				{								
					if(	!m_pSelectedOrderLine->m_bCon)  //미체결
					{
						menu.AppendMenu(MF_STRING, nID, "주문 정정"); 
						menu.AppendMenu(MF_STRING, nID+1, "주문 취소"); 
						//menu.AppendMenu(MF_STRING, nID+2, "Stop/Limit 적용");
						//menu.AppendMenu(MF_STRING, nID+3, "Stop/Limit 취소");	
					}
					else if(m_pSelectedOrderLine->m_bCon)    //미청산
					{
						menu.AppendMenu(MF_STRING, nID+4, "주문 청산");
						menu.AppendMenu(MF_STRING, nID+5, "Stop/Limit 적용");
						menu.AppendMenu(MF_STRING, nID+6, "Stop/Limit 취소");	
					}
									
				}

				UINT flags = TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD;
				CPoint ptPop = point;
				ClientToScreen(m_pChartCWnd->GetSafeHwnd(), &ptPop);
				int nRet = menu.TrackPopupMenu(flags, ptPop.x, ptPop.y, m_pChartCWnd);
				menu.DestroyMenu();				

				switch(nRet) 
				{
				case 102:
					{
						m_pIChartOCX->OnOrderInfomationChanged(m_pSelectedOrderLine->m_dOrderPrice, (int)m_pSelectedOrderLine->m_dAmount, ORDERTYPE_CHANGE, 
													m_pSelectedOrderLine->m_strOrderID,  m_pSelectedOrderLine->m_strCustItem1,
													m_pSelectedOrderLine->m_strCustItem2, m_pSelectedOrderLine->m_strCustItem3);
						break;
					}
				case 103:
					{					
						m_pIChartOCX->OnOrderInfomationChanged(m_pSelectedOrderLine->m_dOrderPrice, (int)m_pSelectedOrderLine->m_dAmount, ORDERTYPE_CANCEL, 
													m_pSelectedOrderLine->m_strOrderID,  m_pSelectedOrderLine->m_strCustItem1,
													m_pSelectedOrderLine->m_strCustItem2, m_pSelectedOrderLine->m_strCustItem3);
						break;
					}
				case 104:
					{
						m_pIChartOCX->OnOrderInfomationChanged(m_pSelectedOrderLine->m_dOrderPrice, (int)m_pSelectedOrderLine->m_dAmount, ORDERTYPE_STOP, 
													m_pSelectedOrderLine->m_strOrderID,  m_pSelectedOrderLine->m_strCustItem1,
													m_pSelectedOrderLine->m_strCustItem2, m_pSelectedOrderLine->m_strCustItem3);
						break;
					}
				case 105:
					{
						m_pIChartOCX->OnOrderInfomationChanged(m_pSelectedOrderLine->m_dOrderPrice, (int)m_pSelectedOrderLine->m_dAmount, ORDERTYPE_LIMIT, 
													m_pSelectedOrderLine->m_strOrderID, m_pSelectedOrderLine->m_strCustItem1,
													m_pSelectedOrderLine->m_strCustItem2, m_pSelectedOrderLine->m_strCustItem3);
						break;
					}
				case 106:	//청산
					{
						m_pIChartOCX->OnOrderInfomationChanged(m_pSelectedOrderLine->m_dOrderPrice, (int)m_pSelectedOrderLine->m_dAmount, ORDERTYPE_CLEAR, 
							m_pSelectedOrderLine->m_strOrderID, m_pSelectedOrderLine->m_strCustItem1,
							m_pSelectedOrderLine->m_strPositionID, m_pSelectedOrderLine->m_strCustItem3);
						break;
					}
				case 107:
					{
						m_pIChartOCX->OnOrderInfomationChanged(m_pSelectedOrderLine->m_dOrderPrice, (int)m_pSelectedOrderLine->m_dAmount, ORDERTYPE_CON_STOP, 
							m_pSelectedOrderLine->m_strOrderID, m_pSelectedOrderLine->m_strCustItem1,
							m_pSelectedOrderLine->m_strPositionID, m_pSelectedOrderLine->m_strCustItem3);
						break;
					}
				case 108:
					{
						m_pIChartOCX->OnOrderInfomationChanged(m_pSelectedOrderLine->m_dOrderPrice, (int)m_pSelectedOrderLine->m_dAmount, ORDERTYPE_CON_LIMIT, 
							m_pSelectedOrderLine->m_strOrderID, m_pSelectedOrderLine->m_strCustItem1,
							m_pSelectedOrderLine->m_strPositionID, m_pSelectedOrderLine->m_strCustItem3);
						break;
					}
				default:
					break;
				}
			}
		}

	}
	return FALSE;
}

BOOL COrderLineAddInImp::FindLineAndSelect(CObList* obOrderLineLiset, CPoint point)
{
	COrderLine* pOrderLine_Find = NULL;
	POSITION pos = obOrderLineLiset->GetHeadPosition();
	BOOL bOnLine = FALSE;
	while(pos)
	{
		pOrderLine_Find = (COrderLine*)obOrderLineLiset->GetNext(pos);
		if(pOrderLine_Find && pOrderLine_Find->IsInRegion(point))
		{
			bOnLine = TRUE;
			m_pSelectedOrderLine = pOrderLine_Find;			

			if(m_pSelectedOrderLine)
				m_pSelectedOrderLine->m_bSelected = FALSE;
			POSITION posCur = obOrderLineLiset->Find((CObject*)m_pSelectedOrderLine);
			obOrderLineLiset->RemoveAt(posCur);
			obOrderLineLiset->AddTail((CObject*)m_pSelectedOrderLine);
			m_pSelectedOrderLine->m_bSelected = TRUE;				
			break;
		}
	}	
	return bOnLine;
}

void COrderLineAddInImp::OnToolCommand( const int p_nToolType, const int p_nCmdOption )
{
	CToolOptionInfo::TOOLOPTION eToolOption = (CToolOptionInfo::TOOLOPTION)p_nToolType;


//	if( eToolOption == CToolOptionInfo::T_DRAG_X_ZOOM || eToolOption == CToolOptionInfo::T_DRAG_Y_ZOOM )
//		m_bRun = FALSE;
//	else if( eToolOption == CToolOptionInfo::T_TOOL_SELECTION)
//	{
//		m_bRun = FALSE;
//		return;
//	}
//	else
		return;

//	m_pIChartOCX->OnToolStateChanged( CToolOptionInfo::T_DRAG_Y_ZOOM, m_bRun);
	
}

BOOL COrderLineAddInImp::GetGraphVScaleRegion( void )
{
	// 1. "일목균형표"가 포함된 Block의 Index를 구한다.
	int nR, nC, nV, nG;
	ILPCSTR szGraphName = m_pIChartManager->FindFirstIndicator( _MTEXT( C2_PRICE_CHART), nR, nC, nV, nG, "DEFAULT");
	if( szGraphName.IsEmpty()) return FALSE;

	IBlock *pIBlock = m_pIChartManager->GetBlock( nR, nC );
	if( !pIBlock )
		return FALSE;
		
	// 3. Vertical Scale Region을 구한다.
	pIBlock->GetVertScaleRegion(m_rctLVScaleRegion, m_rctRVScaleRegion);
	pIBlock->Release();
	pIBlock = NULL;
	
	return TRUE;	
}

BOOL COrderLineAddInImp::GetGraphBlockRegion( void )
{
	// 1. "일목균형표"가 포함된 Block의 Index를 구한다.
	int nR, nC, nV, nG;
	ILPCSTR szGraphName = m_pIChartManager->FindFirstIndicator( _MTEXT( C2_PRICE_CHART), nR, nC, nV, nG, "DEFAULT");
	if( szGraphName.IsEmpty()) return FALSE;

	IBlock *pIBlock = m_pIChartManager->GetBlock( nR, nC );
	if( !pIBlock )
		return FALSE;
		
	// 3. Block의 Region을 구한다.
	CRect rctNoFrameAndMargin;
	pIBlock->GetGraphRegions( m_rctGraphRegion, rctNoFrameAndMargin);

	// 4. Block에서의 Graph Resion을 구한다.
	m_rctGraphDrawingRegion = pIBlock->GetGraphDrwingRegion();
	pIBlock->Release();
	pIBlock = NULL;

	// 5. Block Region을 조정한다.
//	m_rctGraphRegion.left = m_rctGraphDrawingRegion.left;
//	m_rctGraphRegion.right = m_rctGraphDrawingRegion.right;
	
	return TRUE;
}

void COrderLineAddInImp::OnResetChart()
{
//	OnAddInToolCommand( EAI_CUSTOM_ENABLE_BOUNDMARK, TRUE);
	SetRemove_OrderLine();
	SetRemove_CommentData();
}



void COrderLineAddInImp::SetRemove_OrderLine()
{
	//기본 주문선
	POSITION pos = m_OrderLineList.GetHeadPosition();
	while (pos)
	{
		COrderLine* pLineOrder = (COrderLine*)m_OrderLineList.GetNext(pos);
		delete pLineOrder;
		pLineOrder = NULL;
	}
	m_OrderLineList.RemoveAll();	

	// Stop 주문선
	pos = NULL;
	pos = m_OrderLineStopList.GetHeadPosition();
	while (pos)
	{
		COrderLine* pLineOrder = (COrderLine*)m_OrderLineStopList.GetNext(pos);
		delete pLineOrder;
		pLineOrder = NULL;
	}
	m_OrderLineStopList.RemoveAll();

	// Limit 주문선
	pos = NULL;
	pos = m_OrderLineLimitList.GetHeadPosition();
	while (pos)
	{
		COrderLine* pLineOrder = (COrderLine*)m_OrderLineLimitList.GetNext(pos);
		delete pLineOrder;
		pLineOrder = NULL;
	}
	m_OrderLineLimitList.RemoveAll();

	m_pSelectedOrderLine = NULL;

}

void COrderLineAddInImp::SetMouseCursor(int nType)
{
	ML_SET_DEFAULT_RES();
	m_hAPCursor = AfxGetApp()->LoadCursor( nType);
	ASSERT( m_hAPCursor);
	if( m_pIChartOCX) m_pIChartOCX->SetCursorHandle( m_hAPCursor);

}

// void COrderLineAddInImp::OnPacketData( const char *p_szPacketNames, BOOL p_bWithReset)
// {
// 
// }

void COrderLineAddInImp::OnUpdatePacketData( BOOL p_bTrUpdate)
{
}

void COrderLineAddInImp::OnRealData( const char *p_szPacketName, double p_dData, const char *p_szRQ)
{
	
}

BOOL COrderLineAddInImp::FindandSet(FX_CHE_STRUCT* pStOpenPosition, int nLineType, CString& strData)
{
	CString szTmp;
	CString strFormat;
	CString strKeyType(pStOpenPosition->szKeyType, sizeof(pStOpenPosition->szKeyType)); //미체결 - OR01, 체결 CH01


//if( strKeyType.CompareNoCase("CH01") == 0 && memcmp(pStOpenPosition->szTradeType, "12", 2) == 0 )	return TRUE;


	char tmpVal[512]={0,};

	CString strOrderID;
	CString strStatus(pStOpenPosition->szStatus, sizeof(pStOpenPosition->szStatus));
	int nStatus = atoi(strStatus);
	if(strKeyType.CompareNoCase("CH01") == 0)
	{
		CString strID(pStOpenPosition->szFXCMPosID, sizeof(pStOpenPosition->szFXCMPosID));
		strOrderID = strID;
	}
	else if(strKeyType.CompareNoCase("OR01") == 0)
	{
		CString strID(pStOpenPosition->szCustItem2, sizeof(pStOpenPosition->szCustItem2));
		strOrderID = strID;
	}
	else if(strKeyType.CompareNoCase("CLSL") == 0)
	{
		CString strID(pStOpenPosition->szFXCMPosID, sizeof(pStOpenPosition->szFXCMPosID));
		strOrderID = strID;
	}
	strOrderID.TrimLeft();
	strOrderID.TrimRight();

	CString strOrgOrderID(pStOpenPosition->szOrgCustItem2, sizeof(pStOpenPosition->szOrgCustItem2));
	strOrgOrderID.TrimLeft();		strOrgOrderID.TrimRight();

	CString strPositionID(pStOpenPosition->szPositionID, sizeof(pStOpenPosition->szPositionID));
	strPositionID.TrimLeft();	strPositionID.TrimRight();

	strData = "";
	CObList* obOrderList = NULL;
	if(nLineType == LINE_BASE)
	{
		obOrderList = &m_OrderLineList;
		CHAR_TO_VALIDATE(tmpVal, sizeof(pStOpenPosition->szRate), pStOpenPosition->szRate, szTmp, strFormat);
		strData = szTmp;	
	}
	else if(nLineType == LINE_STOP)
	{
		obOrderList = &m_OrderLineStopList;
		CHAR_TO_VALIDATE(tmpVal, sizeof(pStOpenPosition->szStop), pStOpenPosition->szStop, szTmp, strFormat);
		strData = szTmp;	
	}
	else if(nLineType == LINE_LIMIT)
	{
		obOrderList = &m_OrderLineLimitList;
		CHAR_TO_VALIDATE(tmpVal, sizeof(pStOpenPosition->szLimit), pStOpenPosition->szLimit, szTmp, strFormat);
		strData = szTmp;
		strData.TrimLeft();			strData.TrimRight();

		//<<솔로몬. 청산 STOP/LIMIT 주문낸 경우 Limit 가격이 szLimit이 아닌 주문가격에 들어온다......( 서버에서 못 고친다고 함 )
		if(pStOpenPosition->szOrdType[0] == '5' && (strData.IsEmpty() || atof(strData) == 0.0) )
		{
			memcpy(pStOpenPosition->szLimit, pStOpenPosition->szRate, min(sizeof(pStOpenPosition->szLimit), sizeof(pStOpenPosition->szRate))); 
			CHAR_TO_VALIDATE(tmpVal, sizeof(pStOpenPosition->szRate), pStOpenPosition->szRate, szTmp, strFormat);
			strData = szTmp;	
		}
	}
	strData.TrimLeft();
	strData.TrimRight();
	POSITION pos = obOrderList->GetHeadPosition();
	POSITION pos2;

	while(pos)
	{
		pos2 = pos;
		COrderLine* pOpenPosition = (COrderLine*)obOrderList->GetNext(pos);

		pOpenPosition->m_strOrderID.TrimLeft();			pOpenPosition->m_strOrderID.TrimRight();
		pOpenPosition->m_strPositionID.TrimLeft();		pOpenPosition->m_strPositionID.TrimRight();

		// 청산체결인 경우 해당 미청산 선 제거
		if( strKeyType.CompareNoCase("CH01") == 0 && memcmp(pStOpenPosition->szTradeType, "12", 2) == 0 )
		{
			if( strPositionID.CompareNoCase(pOpenPosition->m_strPositionID) == 0 )
			{
				pOpenPosition->m_dAmount -= pStOpenPosition->fAmount;
				if( pOpenPosition->m_dAmount <= 0 )
				{
					delete pOpenPosition;
					pOpenPosition = NULL;

					obOrderList->RemoveAt(pos2);
				}
				continue;
			}
		}
		if( (strOrderID != "" && strOrderID.CompareNoCase(pOpenPosition->m_strOrderID) == 0) ||
			(!strOrgOrderID.IsEmpty() && strOrgOrderID.CompareNoCase(pOpenPosition->m_strOrderID) == 0) ||
			(!strPositionID.IsEmpty() && strPositionID.CompareNoCase(pOpenPosition->m_strPositionID) == 0) )
		{   //값, 수량이 NULL or 0 인 경우 해당 주문선 제거
			/*
			if((nLineType == LINE_BASE && strKeyType.CompareNoCase("CH01") == 0 && pStOpenPosition->fAmount == 0.) ||
			   (nLineType == LINE_BASE && strKeyType.CompareNoCase("OR01") == 0 && pStOpenPosition->fOrderAmt == 0.) ||
			   ((nLineType == LINE_STOP && strData == "" || atof(strData) == 0.0) || (strKeyType.CompareNoCase("CH01") == 0 && pStOpenPosition->fAmount == 0.) ) ||
			   ((nLineType == LINE_LIMIT && strData == "" || atof(strData) == 0.0) || (strKeyType.CompareNoCase("CH01") == 0 && pStOpenPosition->fAmount == 0.) ) ||
			   ((nLineType == LINE_STOP && strData == "" || atof(strData) == 0.0)) || (strKeyType.CompareNoCase("OR01") == 0 && pStOpenPosition->fOrderAmt == 0.) ||
			   ((nLineType == LINE_LIMIT && strData == "" || atof(strData) == 0.0)) || (strKeyType.CompareNoCase("OR01") == 0 && pStOpenPosition->fOrderAmt == 0.) ||
			   ((nLineType == LINE_STOP && strData == "" || atof(strData) == 0.0) || (strKeyType.CompareNoCase("CLSL") == 0 && pStOpenPosition->fOrderAmt == 0.) ||
			   ((nLineType == LINE_LIMIT && strData == "" || atof(strData) == 0.0) || (strKeyType.CompareNoCase("CLSL") == 0 && pStOpenPosition->fOrderAmt == 0.) )))
			*/
			// 취소주문의 경우 주문선 삭제
			if( nStatus == 17 )
			{
				//{{ 청산주문의 STOP/LIMIT 주문 취소
				if(nLineType == LINE_BASE && strKeyType.CompareNoCase("CLSL") == 0 && atof(strData) == 0.0 )
				{
					return TRUE;
				}
				else
				{
					delete pOpenPosition;
					pOpenPosition = NULL;

					obOrderList->RemoveAt(pos2);
				}
				//}}

				return TRUE;
			}
			else if( (nLineType == LINE_BASE && strKeyType.CompareNoCase("CH01") == 0 && pStOpenPosition->fAmount == 0.) ||
				(nLineType == LINE_BASE && strKeyType.CompareNoCase("OR01") == 0 && pStOpenPosition->fOrderAmt == 0.) ||
				(nLineType == LINE_STOP &&  (strData == "" || atof(strData) == 0.0 || (strKeyType.CompareNoCase("CH01") == 0 && pStOpenPosition->fAmount == 0.)) ) ||
				(nLineType == LINE_LIMIT && (strData == "" || atof(strData) == 0.0 || (strKeyType.CompareNoCase("CH01") == 0 && pStOpenPosition->fAmount == 0.)) ) ||
				(nLineType == LINE_STOP  && (strData == "" || atof(strData) == 0.0 || (strKeyType.CompareNoCase("CLSL") == 0 && pStOpenPosition->fOrderAmt == 0.)) ) ||
				(nLineType == LINE_LIMIT && (strData == "" || atof(strData) == 0.0 || (strKeyType.CompareNoCase("CLSL") == 0 && pStOpenPosition->fOrderAmt == 0.)) ) ||
				(nLineType == LINE_STOP  && (strData == "" || atof(strData) == 0.0 || (strKeyType.CompareNoCase("OR01") == 0 && pStOpenPosition->fOrderAmt == 0.)) ) ||
				(nLineType == LINE_LIMIT && (strData == "" || atof(strData) == 0.0 || (strKeyType.CompareNoCase("OR01") == 0 && pStOpenPosition->fOrderAmt == 0.)) ) )
			{
				delete pOpenPosition;
				pOpenPosition = NULL;

				obOrderList->RemoveAt(pos2);
				return TRUE;
			}
			//{{ 청산에 대한 STOP/LIMIT 주문인 경우
			else if(nLineType == LINE_BASE && strKeyType.CompareNoCase("CLSL") == 0 && atof(strData) != 0.0 )
			{
				return TRUE;
			}
			else if(strKeyType.CompareNoCase("CLSL") == 0 && atof(strData) != 0.0 && nStatus != 15 )
			{
				return FALSE;
			}
			//}}
			// Stop 체결인 경우는 같은 가격의 Stop주문을 찾아서 삭제하고, 체결된 내역을 새로 추가한다.
			else if( strKeyType.CompareNoCase("CH01") == 0
				&& nLineType == LINE_STOP
				&& (pStOpenPosition->szOrdType[0] == '3' || pStOpenPosition->szOrdType[0] == '4' || pStOpenPosition->szOrdType[0] == '5') )
			{
				CString strStopPrice(pStOpenPosition->szStop, sizeof(pStOpenPosition->szStop));
				strStopPrice.TrimLeft();					strStopPrice.TrimRight();
				pOpenPosition->m_strStop.TrimLeft();		pOpenPosition->m_strStop.TrimRight();
				if( strStopPrice == pOpenPosition->m_strStop &&  pOpenPosition->m_dOrgOrderAmount == pStOpenPosition->fOrderAmt )
				{
					pOpenPosition->m_dAmount -= pStOpenPosition->fAmount;
					if( pOpenPosition->m_dAmount <= 0 )
					{
						delete pOpenPosition;
						pOpenPosition = NULL;

						obOrderList->RemoveAt(pos2);
						return TRUE;
					}
				}
			}	
			else if( strKeyType.CompareNoCase("CH01") == 0
				&& nLineType == LINE_LIMIT
				&& (pStOpenPosition->szOrdType[0] == '3' || pStOpenPosition->szOrdType[0] == '4' || pStOpenPosition->szOrdType[0] == '5') )
			{
				CString strLimitPrice(pStOpenPosition->szLimit, sizeof(pStOpenPosition->szLimit));
				strLimitPrice.TrimLeft();					strLimitPrice.TrimRight();
				pOpenPosition->m_strLimit.TrimLeft();		pOpenPosition->m_strLimit.TrimRight();
				if( strLimitPrice == pOpenPosition->m_strLimit &&  pOpenPosition->m_dOrgOrderAmount == pStOpenPosition->fOrderAmt )
				{
					pOpenPosition->m_dAmount -= pStOpenPosition->fAmount;
					if( pOpenPosition->m_dAmount <= 0 )
					{
						delete pOpenPosition;
						pOpenPosition = NULL;

						obOrderList->RemoveAt(pos2);
						return TRUE;
					}
				}
			}	
			else //주문선 갱신
			{
				double fPrice = atof(strData);
				if( fPrice != 0. )
					pOpenPosition->m_dOrderPrice = fPrice;

				CHAR_TO_VALIDATE(tmpVal, sizeof(pStOpenPosition->szCustItem1), pStOpenPosition->szCustItem1, szTmp, strFormat);
				pOpenPosition->m_strCustItem1 = szTmp;
				CHAR_TO_VALIDATE(tmpVal, sizeof(pStOpenPosition->szCustItem2), pStOpenPosition->szCustItem2, szTmp, strFormat);
				pOpenPosition->m_strCustItem2 = szTmp;
				CHAR_TO_VALIDATE(tmpVal, sizeof(pStOpenPosition->szCustItem3), pStOpenPosition->szCustItem3, szTmp, strFormat);
				pOpenPosition->m_strCustItem3 = szTmp;
				//CHAR_TO_VALIDATE(tmpVal, sizeof(pStOpenPosition->szPositionID), pStOpenPosition->szPositionID, szTmp, strFormat);
				CString strTmpPositionID(pStOpenPosition->szPositionID, sizeof(pStOpenPosition->szPositionID));
				strTmpPositionID.TrimLeft();			strTmpPositionID.TrimRight();
				if( !strTmpPositionID.IsEmpty() )
					pOpenPosition->m_strPositionID = strTmpPositionID;	

				int nYPoint = m_pIChartOCX->ConvertDataToYPosition( pOpenPosition->m_dOrderPrice, m_dViewMax, m_dViewMin, m_rctGraphDrawingRegion.top, m_rctGraphDrawingRegion.bottom, m_bLog, m_bReverse);
				if(nLineType == LINE_BASE)
				{
					pOpenPosition->m_strRate = strData;
				}
				if(nLineType == LINE_STOP)
				{	
					pOpenPosition->m_strStop = strData;
				}
				if(nLineType == LINE_LIMIT)
				{	
					pOpenPosition->m_strLimit = strData;
				}

				pOpenPosition->m_ptStart = CPoint(m_rctGraphRegion.left, nYPoint);
				pOpenPosition->m_ptEnd = CPoint(m_rctGraphRegion.right, nYPoint);

				CHAR_TO_VALIDATE(tmpVal, sizeof(pStOpenPosition->szOpenTime), pStOpenPosition->szOpenTime, szTmp, strFormat);
				szTmp.TrimRight();
				//<<20100407_JS.Kim 솔로몬
				//pOpenPosition->m_strOpenTime = szTmp;
				if( strlen(szTmp) >= 14 )
					pOpenPosition->m_strOpenTime.Format("%4s:%2s:%2s-%2s:%2s:%2s", szTmp.Left(4), szTmp.Mid(4,2), szTmp.Mid(6,2), szTmp.Mid(8,2), szTmp.Mid(10,2), szTmp.Mid(12,2));
				else if( strlen(szTmp) >= 6 )
					pOpenPosition->m_strOpenTime.Format("%2s:%2s:%2s", szTmp.Left(2), szTmp.Mid(2,2), szTmp.Mid(4,2));
				else
					pOpenPosition->m_strOpenTime = szTmp;
				//>>

				CHAR_TO_VALIDATE(tmpVal, sizeof(pStOpenPosition->szExeTime), pStOpenPosition->szExeTime, szTmp, strFormat);
				szTmp.TrimRight();	
				//<<20100407_JS.Kim 솔로몬
				//pOpenPosition->m_strExeTime = szTmp;	
				if( strlen(szTmp) >= 14 )
					pOpenPosition->m_strExeTime.Format("%4s:%2s:%2s-%2s:%2s:%2s", szTmp.Left(4), szTmp.Mid(4,2), szTmp.Mid(6,2), szTmp.Mid(8,2), szTmp.Mid(10,2), szTmp.Mid(12,2));
				else if( strlen(szTmp) >= 6 )
					pOpenPosition->m_strExeTime.Format("%2s:%2s:%2s", szTmp.Left(2), szTmp.Mid(2,2), szTmp.Mid(4,2));
				else
					pOpenPosition->m_strExeTime = szTmp;
				//>>
				
				CHAR_TO_VALIDATE(tmpVal, sizeof(pStOpenPosition->szSide), pStOpenPosition->szSide, szTmp, strFormat);
				szTmp.TrimRight();	
				pOpenPosition->m_strSide = szTmp;

				CHAR_TO_VALIDATE(tmpVal, sizeof(pStOpenPosition->szKeyType), pStOpenPosition->szKeyType, szTmp, strFormat);
				szTmp.TrimRight();	
				CString strKeyType = szTmp;
				if(strKeyType.CompareNoCase("CH01") == 0)
				{
					pOpenPosition->m_dAmount = pStOpenPosition->fAmount;
					pOpenPosition->m_bCon = TRUE;
				}
				else if(strKeyType.CompareNoCase("OR01") == 0)
				{
					pOpenPosition->m_dAmount = pStOpenPosition->fOrderAmt;
					pOpenPosition->m_bCon = FALSE;
				}
				else if(strKeyType.CompareNoCase("CLSL") == 0)
				{
					pOpenPosition->m_dAmount = pStOpenPosition->fOrderAmt;
					pOpenPosition->m_bCon = TRUE;
				}

				// 정정주문의 경우 주문번호도 수정
				if( nStatus == 5 || nStatus == 15 )
				{
					pOpenPosition->m_strOrderID = strOrderID;
				}

			}			
			return TRUE;
		}			
	}

	// 청산체결인 경우 주문선 ADD 하지 않는다.
	if( strKeyType.CompareNoCase("CH01") == 0 && memcmp(pStOpenPosition->szTradeType, "12", 2) == 0 )
		return TRUE;

	if( (nLineType == LINE_BASE && strKeyType.CompareNoCase("CH01") == 0 && pStOpenPosition->fAmount == 0.) ||
		(nLineType == LINE_BASE && strKeyType.CompareNoCase("OR01") == 0 && pStOpenPosition->fOrderAmt == 0.) ||
		(nLineType == LINE_STOP &&  (strData == "" || atof(strData) == 0.0 || (strKeyType.CompareNoCase("CH01") == 0 && pStOpenPosition->fAmount == 0.)) ) ||
		(nLineType == LINE_LIMIT && (strData == "" || atof(strData) == 0.0 || (strKeyType.CompareNoCase("CH01") == 0 && pStOpenPosition->fAmount == 0.)) ) ||
		(nLineType == LINE_STOP  && (strData == "" || atof(strData) == 0.0 || (strKeyType.CompareNoCase("CLSL") == 0 && pStOpenPosition->fOrderAmt == 0.)) ) ||
		(nLineType == LINE_LIMIT && (strData == "" || atof(strData) == 0.0 || (strKeyType.CompareNoCase("CLSL") == 0 && pStOpenPosition->fOrderAmt == 0.)) ) ||
		(nLineType == LINE_STOP  && (strData == "" || atof(strData) == 0.0 || (strKeyType.CompareNoCase("OR01") == 0 && pStOpenPosition->fOrderAmt == 0.)) ) ||
		(nLineType == LINE_LIMIT && (strData == "" || atof(strData) == 0.0 || (strKeyType.CompareNoCase("OR01") == 0 && pStOpenPosition->fOrderAmt == 0.)) ) )
	{
		//수량이 없을 경우 ADD하지 않기 위해
		return TRUE;
	}

	return FALSE;// 주문선 ADD
}

void COrderLineAddInImp::SetOpenPosition(FX_CHE_STRUCT* pStOpenPosition)
{
	// 호가감시주문 제외 - onlyojt (20100217)
	//if(!memcmp(pStOpenPosition->szOrdType + 1, "OT", 2))
	//	return;

	CString strStatus(pStOpenPosition->szStatus, sizeof(pStOpenPosition->szStatus));
	int nStatus = atoi(strStatus);
	if( nStatus == 5 || nStatus == 7 )
		return;

	CString strData;
	CString strOrderType(pStOpenPosition->szOrdType, sizeof(pStOpenPosition->szOrdType));
	strOrderType.TrimRight();

	for(int nIndex = LINE_BASE; nIndex <= LINE_LIMIT; nIndex++)
	{	
		BOOL bExist = FindandSet(pStOpenPosition, nIndex, strData);
		/*
		if(!bExist && strData != "" && atof(strData) != 0 && (strOrderType.CompareNoCase("DE") != 0))
			AddOpenPosition(pStOpenPosition, nIndex);
		*/
		//기존에 데이터가 존재하지 않고 && 가격이 존재하고 && 정정완료(원주문) && 취소완료(원주문) 이 아닐경우
		if(!bExist && strData != "" && (atof(strData) != 0.0) )
			AddOpenPosition(pStOpenPosition, nIndex);
	}


/*	POSITION pos = m_OrderLineList.GetHeadPosition();
	POSITION pos2;

	while(pos)
	{
		pos2 = pos;
		COrderLine* pOpenPosition = (COrderLine*)m_OrderLineList.GetNext(pos);
		if(!::memcmp(pStOpenPosition->szOrgCustItem2, pOpenPosition->m_strOrderID, sizeof(pStOpenPosition->szOrgCustItem2)))
		{
			if(pStOpenPosition->fAmount == 0.)
			{
				delete pOpenPosition;
				pOpenPosition = NULL;

				m_OrderLineList.RemoveAt(pos2);
			}
			else
			{
				CString strOrderPrice(pStOpenPosition->szRate, sizeof(pStOpenPosition->szRate));

				pOpenPosition->m_dOrderPrice = atof(strOrderPrice);
				int nYPoint = m_pIChartOCX->ConvertDataToYPosition( pOpenPosition->m_dOrderPrice, m_dViewMax, m_dViewMin, m_rctGraphDrawingRegion.top, m_rctGraphDrawingRegion.bottom, m_bLog, m_bReverse);

				pOpenPosition->m_strRate = strOrderPrice;
				pOpenPosition->m_ptStart = CPoint(m_rctGraphRegion.left, nYPoint);
				pOpenPosition->m_ptEnd = CPoint(m_rctGraphRegion.right, nYPoint);
			}			
			return;
		}	
	}
	AddOpenPosition(pStOpenPosition);
*/	
}
BOOL COrderLineAddInImp::CheckTypeAndSet(FX_CHE_STRUCT* pDataPosition, int nLineType)
{
	CString strData = "";
	if(nLineType == LINE_BASE)
		return TRUE;
	else if(nLineType == LINE_BASE)
		strData.Format("%s", pDataPosition->szStop);
	else if(nLineType == LINE_BASE)
		strData.Format("%s", pDataPosition->szLimit);
 
	if(strData == "")
		return FALSE;
	else
		return TRUE;
}

void COrderLineAddInImp::AddOpenPosition(FX_CHE_STRUCT* pStOpenPosition,  int nLineType)
{	
	COrderLine* pOrderLine = new COrderLine();

	SetData(pOrderLine, pStOpenPosition, CONCLUSION_YES, nLineType);

	if(pOrderLine->m_dOrderPrice == 0) return;
	if(nLineType == LINE_BASE)
		m_OrderLineList.AddTail((CObject*)pOrderLine);
	else if(nLineType == LINE_STOP)
		m_OrderLineStopList.AddTail((CObject*)pOrderLine);
	else if(nLineType == LINE_LIMIT)
		m_OrderLineLimitList.AddTail((CObject*)pOrderLine);
}

void COrderLineAddInImp::SetPendingPosition(FX_CHE_STRUCT*	pStPendingPosition)
{
	// 호가감시주문 제외 - onlyojt (20100217)
	//if(!memcmp(pStPendingPosition->szOrdType + 1, "OT", 2))
	//	return;

	/*[주문상태정보]
	01:주문접수,02:체결중,03:체결완료,
	04:정정접수,05:정정완료(원주문),06:취소접수,
	07:취소완료(원주문),08:거부,15:정정완료(정정주문),17:취소완료(취소주문)*/
	CString strStatus(pStPendingPosition->szStatus, sizeof(pStPendingPosition->szStatus));
	int nStatus = atoi(strStatus);
	if( nStatus == 5 || nStatus == 7 )
		return;

	CString strData;
	CString strOrderType(pStPendingPosition->szOrdType, sizeof(pStPendingPosition->szOrdType));
	strOrderType.TrimRight();

	//CString strType = strOrderType.Mid(1, strOrderType.GetLength() - 1);

	for(int nIndex = LINE_BASE; nIndex <= LINE_LIMIT; nIndex++)
	{	
		BOOL bExist = FindandSet(pStPendingPosition, nIndex, strData);
		double dData = atof(strData);		
		/*
		//기존에 데이터가 존재하지 않고 && 가격이 존재하고 && 취소주문이 아닐경우
		if(!bExist && strData != "" && (dData != 0.0) && (strOrderType.CompareNoCase("DE") != 0) &&
			(strType.CompareNoCase("CEL") != 0) && (strType.CompareNoCase("CES") != 0) && (strType.CompareNoCase("CET") != 0))
		*/
		//기존에 데이터가 존재하지 않고 && 가격이 존재하고 && 정정완료(원주문) && 취소완료(원주문) 이 아닐경우
		if(!bExist && strData != "" && (dData != 0.0) )
			AddPendingPosition(pStPendingPosition, nIndex);
	}

/*
	POSITION pos = m_OrderLineList.GetHeadPosition();
	POSITION pos2;

	while(pos)
	{
		pos2 = pos;
		COrderLine* pPendingPosition = (COrderLine*)m_OrderLineList.GetNext(pos);
		if(!::memcmp(pStPendingPosition->szOrgCustItem2, pPendingPosition->m_strOrderID, sizeof(pStPendingPosition->szOrgCustItem2)))
		{
			if(pStPendingPosition->fOrderAmt == 0.)
			{
				delete pPendingPosition;
				pPendingPosition = NULL;

				m_OrderLineList.RemoveAt(pos2);
			}
			else
			{
				CString strOrderPrice(pStPendingPosition->szRate, sizeof(pStPendingPosition->szRate));

				pPendingPosition->m_dOrderPrice = atof(strOrderPrice);
				int nYPoint = m_pIChartOCX->ConvertDataToYPosition( pPendingPosition->m_dOrderPrice, m_dViewMax, m_dViewMin, m_rctGraphDrawingRegion.top, m_rctGraphDrawingRegion.bottom, m_bLog, m_bReverse);

				pPendingPosition->m_strRate = strOrderPrice;
				pPendingPosition->m_ptStart = CPoint(m_rctGraphRegion.left, nYPoint);
				pPendingPosition->m_ptEnd = CPoint(m_rctGraphRegion.right, nYPoint);
			}			
			return;
		}
	}
	AddPendingPosition(pStPendingPosition);
*/
}

void COrderLineAddInImp::AddPendingPosition(FX_CHE_STRUCT* pStPendingPosition, int nLineType)
{
	COrderLine* pOrderLine = new COrderLine();

	SetData(pOrderLine, pStPendingPosition, CONCLUSION_NO, nLineType);

	if(nLineType == LINE_BASE)
		m_OrderLineList.AddTail((CObject*)pOrderLine);
	else if(nLineType == LINE_STOP)
		m_OrderLineStopList.AddTail((CObject*)pOrderLine);
	else if(nLineType == LINE_LIMIT)
		m_OrderLineLimitList.AddTail((CObject*)pOrderLine);


//	COrderLine* pOrderLine = new COrderLine();
//	SetData(pOrderLine, pStPendingPosition, CONCLUSION_NO);
//	m_OrderLineList.AddTail((CObject*)pOrderLine);
/*
	int nYPoint = m_pIChartOCX->ConvertDataToYPosition( stOpenPosition.m_dOpenPrice, m_dViewMax, m_dViewMin, m_rctGraphDrawingRegion.top, m_rctGraphDrawingRegion.bottom, m_bLog, m_bReverse);
	
	COrderLine* pOrderLine = new COrderLine();
	pOrderLine->m_ptStart = CPoint(m_rctGraphRegion.left, nYPoint);
	pOrderLine->m_ptEnd = CPoint(m_rctGraphRegion.right, nYPoint);
	pOrderLine->m_dOrderPrice = stOpenPosition.m_dOpenPrice;
	pOrderLine->m_dAmount = stOpenPosition.m_dAmount;
	m_OrderLineList.AddTail((CObject*)pOrderLine);
*/
}

void COrderLineAddInImp::CheckAllTypeLineAndDoFunc(CObList* pOrderLineList, CDC* pDC)
{
	POSITION pos;
	COrderLine* pOrderLine;
	for (pos = pOrderLineList->GetHeadPosition(); pos != NULL;)
	{
		pOrderLine = (COrderLine*)pOrderLineList->GetNext(pos);
		if(!pOrderLine) return;

		// 미체결일때 Stop/Limit은 그리지 않는다. 정보만 가지고 있음 - onlyojt (20100218)
//		if(!pOrderLine->m_bCon && (pOrderLine->m_IsStop || pOrderLine->m_IsLimit))
//			return;

		pOrderLine->m_ptEnd.x = m_rctGraphRegion.right;  //리사이즈처리
		CheckIsLineInScreen(pOrderLine);
	
		pOrderLine->Draw(pDC);		
	}	
}

void COrderLineAddInImp::CheckIsLineInScreen(COrderLine* pOrderLine)
{	
	CPoint ptLeftTop(m_rctGraphRegion.left, m_rctGraphRegion.top);       //화면 좌측상단 좌표
	CPoint ptLeftBottom(m_rctGraphRegion.left, m_rctGraphRegion.bottom); //화면 좌측하단 좌표

	int nR, nC, nV, nG;
	ILPCSTR szGraphName = m_pIChartManager->FindFirstIndicator( _MTEXT( C2_PRICE_CHART), nR, nC, nV, nG, "DEFAULT");

	IBlock *pIBlock = NULL;
	pIBlock = m_pIChartManager->GetBlock( nR, nC );
	if( !pIBlock )
		return;
	
	double dViewMin, dViewMax;
	CScaleBaseData::VERTMINMAXTYPE eMinMaxType;
	pIBlock->GetVScaleMinMaxSetting( nV, m_bIsLock, eMinMaxType, dViewMin, dViewMax);

	if(!m_bOnSetCaprture)
	{	
		int nYPoint = m_pIChartOCX->ConvertDataToYPosition(pOrderLine->m_dOrderPrice, dViewMax, dViewMin, m_rctGraphDrawingRegion.top, m_rctGraphDrawingRegion.bottom, m_bLog, m_bReverse);
		pOrderLine->m_ptStart.y = nYPoint;
		pOrderLine->m_ptEnd.y   = nYPoint;
	}
	
	if(pOrderLine->m_dOrderPrice > dViewMax)      //OrderLine이 화면 위 영역에 존재
	{		
		if(pOrderLine->m_eLinePosition == LINE_POS_OUT_UP)
			return;
		else if(pOrderLine->m_eLinePosition == LINE_POS_OUT_DOWN)
		{
			m_nCount_EscapeDown--;			
		}	
		pOrderLine->m_eLinePosition = LINE_POS_OUT_UP;
		m_nCount_EscapeUp++;	
		//화면 영역 벗어날시 삼각형 시작 좌표 설정
		pOrderLine->m_ptEscape_Start = CPoint(m_rctGraphRegion.left + TRIANGLE_SIDE * m_nCount_EscapeUp, m_rctGraphRegion.top + 20);
	}
	else if(pOrderLine->m_dOrderPrice < dViewMin)  //OrderLine이 화면 아래 영역에 존재
	{
		if(pOrderLine->m_eLinePosition == LINE_POS_OUT_UP)
		{
			m_nCount_EscapeUp--;
		}
		else if(pOrderLine->m_eLinePosition == LINE_POS_OUT_DOWN)
			return;
		
		pOrderLine->m_eLinePosition = LINE_POS_OUT_DOWN;
		m_nCount_EscapeDown++;
		//화면 영역 벗어날시 삼각형 시작 좌표 설정
		pOrderLine->m_ptEscape_Start = CPoint(m_rctGraphRegion.left + TRIANGLE_SIDE * m_nCount_EscapeDown, m_rctGraphRegion.bottom);
	}
	else									  ////OrderLine이 화면 영역에 존재
	{
		if(pOrderLine->m_eLinePosition == LINE_POS_OUT_UP)
			m_nCount_EscapeUp--;
		else if(pOrderLine->m_eLinePosition == LINE_POS_OUT_DOWN)
			m_nCount_EscapeDown--;
		pOrderLine->m_eLinePosition = LINE_POS_IN;
	}
}

BOOL COrderLineAddInImp::InvokeAddInStr( LPCTSTR p_szCommandName, LPCTSTR p_szData)
{
	// Load Chart Data Info I
	// 주문선 기능
	if( strcmp(p_szCommandName, "1") == 0 )
	{
		if( stricmp(p_szData, "ON") == 0 )
		{
			m_bRun = TRUE;
			// (2008/9/13 - Seung-Won, Bae) Set Mouse Exclusive
			m_pIAddInToolMgr->GetAllMouseExclusive( m_pIAddInDllBase);
		}
		else
		{
			m_bRun = FALSE;
			// (2008/9/13 - Seung-Won, Bae) Set Mouse Exclusive
			m_pIAddInToolMgr->ReleaseAllMouseExclusive( m_pIAddInDllBase);

			IBlock *pIBlock = m_pIChartManager->GetBlock( 0, 0);
			if( !pIBlock) return TRUE;

			pIBlock->SetVScaleMinMaxSetting( m_nVScaleIndex, FALSE,
											 CScaleBaseData::VERTMINMAXTYPE::VERT_MINMAX_OF_VIEW,
											 m_dViewMax, m_dViewMax);
			pIBlock->Release();
		}
	}
	// 거래내역 표시 기능 ㅎ
	else if( strcmp(p_szCommandName, "2") == 0 )
	{
		if( stricmp(p_szData, "ON") == 0 )
			m_bComment = TRUE;
		else
			m_bComment = FALSE;
	}
	// 매도/매수 구분값 셋팅
	else if( strcmp(p_szCommandName, "3") == 0 )
	{
		if( stricmp(p_szData, "BUY") == 0 )
			m_nMMGb = 0;
		else
			m_nMMGb = 1;
	}

	m_pIChartOCX->RunToolCommand( CToolOptionInfo::T_TOOL_SELECTION, 1);
	return TRUE;
}

BOOL COrderLineAddInImp::InvokeAddIn( int p_nCommandType, long p_lData)
{
	if(p_nCommandType == 5)
	{
	}
	else if(!p_lData) return FALSE;	

	if(p_nCommandType == 1)			// ADD OPEN_POSITION after Seach... 잔고내역
	{
	 	CArray<FX_CHE_STRUCT*, FX_CHE_STRUCT*>* arrOrderList = (CArray<FX_CHE_STRUCT*, FX_CHE_STRUCT*>*)p_lData;
		for(int nIndex = 0; nIndex < arrOrderList->GetSize(); nIndex++)
		{
			FX_CHE_STRUCT* pDataPosition = (FX_CHE_STRUCT*)arrOrderList->GetAt(nIndex);
			SetOpenPosition(pDataPosition);
			SetCommentData(pDataPosition);
		}
	}
	else if(p_nCommandType == 2)	// ADD PENDING_ORDER after Seach.... 미체결내역
	{
		CArray<FX_CHE_STRUCT*, FX_CHE_STRUCT*>* arrOrderList = (CArray<FX_CHE_STRUCT*, FX_CHE_STRUCT*>*)p_lData;
		for(int nIndex = 0; nIndex < arrOrderList->GetSize(); nIndex++)
		{
			FX_CHE_STRUCT* pDataPosition = (FX_CHE_STRUCT*)arrOrderList->GetAt(nIndex);		
			SetPendingPosition(pDataPosition);	
		}		
	}
	else if(p_nCommandType == 3)	// ADD OPEN_POSITION   체결실시간
	{
		FX_CHE_STRUCT* pDataPosition = (FX_CHE_STRUCT*)p_lData;
		if(pDataPosition)
		{
			SetOpenPosition(pDataPosition);
			SetCommentData(pDataPosition);
		}
//		for(int nIndex = LINE_BASE; nIndex <= LINE_LIMIT; nIndex++)
// 		{		
// 			if(CheckTypeAndSet(pDataPosition, nIndex))
// 				AddOpenPosition(pDataPosition, nIndex);	
// 		}
	}
	else if(p_nCommandType == 4)	// ADD PENDING_ORDER   미체결실시간
	{
		FX_CHE_STRUCT* pDataPosition = (FX_CHE_STRUCT*)p_lData;
		if(pDataPosition)
		{
			SetPendingPosition(pDataPosition);	
		}
//		for(int nIndex = LINE_BASE; nIndex <= LINE_LIMIT; nIndex++)
//		{
//			if(CheckTypeAndSet(pDataPosition, nIndex))
//				AddPendingPosition(pDataPosition, nIndex);	
//		}
	}
	else if(p_nCommandType == 5)	// reset
	{
		SetRemove_OrderLine();
		SetRemove_CommentData();
	}
	else if(p_nCommandType == 6)	// ADD PENDING_ORDER after Seach.... 체결내역
	{
		CArray<FX_CHE_STRUCT*, FX_CHE_STRUCT*>* arrOrderList = (CArray<FX_CHE_STRUCT*, FX_CHE_STRUCT*>*)p_lData;
		for(int nIndex = 0; nIndex < arrOrderList->GetSize(); nIndex++)
		{
			FX_CHE_STRUCT* pDataPosition = (FX_CHE_STRUCT*)arrOrderList->GetAt(nIndex);		
			//if( memcmp(pDataPosition->szTradeType, "11", 2) != 0 )
				SetCommentData(pDataPosition);
		}		
	}

	return TRUE;
}

void COrderLineAddInImp::SetData(COrderLine* pOrderLine, FX_CHE_STRUCT* pDataPosition, int nType, int nLineType)
{
	CString szTmp;
	CString strFormat;
	char tmpVal[512]={0,};

	CString strOrderPrice;
	if(nLineType == LINE_BASE)
	{
		CHAR_TO_VALIDATE(tmpVal, sizeof(pDataPosition->szRate), pDataPosition->szRate, szTmp, strFormat);
		strOrderPrice = szTmp;
		strOrderPrice.TrimLeft();
		pOrderLine->m_strRate = strOrderPrice;
	}
	else if(nLineType == LINE_STOP)
	{
		CHAR_TO_VALIDATE(tmpVal, sizeof(pDataPosition->szStop), pDataPosition->szStop, szTmp, strFormat);
		strOrderPrice = szTmp;
		strOrderPrice.TrimLeft();
		pOrderLine->m_strStop = strOrderPrice;
		pOrderLine->m_IsStop = TRUE;
	}
	else if(nLineType == LINE_LIMIT)
	{
		CHAR_TO_VALIDATE(tmpVal, sizeof(pDataPosition->szLimit), pDataPosition->szLimit, szTmp, strFormat);
		strOrderPrice = szTmp;
		strOrderPrice.TrimLeft();
		pOrderLine->m_strLimit = strOrderPrice;
		pOrderLine->m_IsLimit = TRUE;
	}	
	strOrderPrice.TrimRight();
	
	pOrderLine->m_dOrderPrice = atof(strOrderPrice) ;
	CHAR_TO_VALIDATE(tmpVal, sizeof(pDataPosition->szCustItem1), pDataPosition->szCustItem1, szTmp, strFormat);
	pOrderLine->m_strCustItem1 = szTmp;
	CHAR_TO_VALIDATE(tmpVal, sizeof(pDataPosition->szCustItem2), pDataPosition->szCustItem2, szTmp, strFormat);
	pOrderLine->m_strCustItem2 = szTmp;
	CHAR_TO_VALIDATE(tmpVal, sizeof(pDataPosition->szCustItem3), pDataPosition->szCustItem3, szTmp, strFormat);
	pOrderLine->m_strCustItem3 = szTmp;


	CHAR_TO_VALIDATE(tmpVal, sizeof(pDataPosition->szOpenTime), pDataPosition->szOpenTime, szTmp, strFormat);
	szTmp.TrimRight();
	//<<20100407_JS.Kim 솔로몬
	//pOrderLine->m_strOpenTime = szTmp;
	if( strlen(szTmp) >= 14 )
		pOrderLine->m_strOpenTime.Format("%4s:%2s:%2s-%2s:%2s:%2s", szTmp.Left(4), szTmp.Mid(4,2), szTmp.Mid(6,2), szTmp.Mid(8,2), szTmp.Mid(10,2), szTmp.Mid(12,2));
	else if( strlen(szTmp) >= 6 )
		pOrderLine->m_strOpenTime.Format("%2s:%2s:%2s", szTmp.Left(2), szTmp.Mid(2,2), szTmp.Mid(4,2));
	else
		pOrderLine->m_strOpenTime = szTmp;
	//>>

	CHAR_TO_VALIDATE(tmpVal, sizeof(pDataPosition->szExeTime), pDataPosition->szExeTime, szTmp, strFormat);
	szTmp.TrimRight();	
	//<<20100407_JS.Kim 솔로몬
	//pOrderLine->m_strExeTime = szTmp;
	if( strlen(szTmp) >= 14 )
		pOrderLine->m_strExeTime.Format("%4s:%2s:%2s-%2s:%2s:%2s", szTmp.Left(4), szTmp.Mid(4,2), szTmp.Mid(6,2), szTmp.Mid(8,2), szTmp.Mid(10,2), szTmp.Mid(12,2));
	else if( strlen(szTmp) >= 6 )
		pOrderLine->m_strExeTime.Format("%2s:%2s:%2s", szTmp.Left(2), szTmp.Mid(2,2), szTmp.Mid(4,2));
	else
		pOrderLine->m_strExeTime = szTmp;
	//>>

	pOrderLine->m_dOrgOrderAmount = pDataPosition->fOrderAmt;

	CHAR_TO_VALIDATE(tmpVal, sizeof(pDataPosition->szSide), pDataPosition->szSide, szTmp, strFormat);
	szTmp.TrimRight();	
	pOrderLine->m_strSide = szTmp;

	CHAR_TO_VALIDATE(tmpVal, sizeof(pDataPosition->szKeyType), pDataPosition->szKeyType, szTmp, strFormat);
	szTmp.TrimRight();	
	CString strKeyType = szTmp;

	if(nType == CONCLUSION_YES && strKeyType.CompareNoCase("CH01") == 0)
	{
		CString strCMPosID(pDataPosition->szFXCMPosID, sizeof(pDataPosition->szFXCMPosID));
		strCMPosID.TrimRight();
		CString strPosID(pDataPosition->szPositionID, sizeof(pDataPosition->szPositionID));
		strPosID.TrimRight();

		pOrderLine->m_dAmount = pDataPosition->fAmount;
		pOrderLine->m_bCon = TRUE;
		pOrderLine->m_strOrderID = strCMPosID;
		pOrderLine->m_strPositionID = strPosID;
	}
	else if(strKeyType.CompareNoCase("OR01") == 0)
	{
		CString strOrderID(pDataPosition->szCustItem2, sizeof(pDataPosition->szCustItem2));
		strOrderID.TrimRight();	

		pOrderLine->m_dAmount = pDataPosition->fOrderAmt;
		pOrderLine->m_bCon = FALSE;
		pOrderLine->m_strOrderID = strOrderID;
	}
	else if(strKeyType.CompareNoCase("CLSL") == 0)
	{
		CString strOrderID(pDataPosition->szFXCMPosID, sizeof(pDataPosition->szFXCMPosID));
		strOrderID.TrimRight();	
		CString strPosID(pDataPosition->szPositionID, sizeof(pDataPosition->szPositionID));
		strPosID.TrimRight();

		pOrderLine->m_dAmount = pDataPosition->fOrderAmt;
		pOrderLine->m_bCon = TRUE;
		pOrderLine->m_strOrderID = strOrderID;
		pOrderLine->m_strPositionID = strPosID;
	}

	int nYPoint = m_pIChartOCX->ConvertDataToYPosition( pOrderLine->m_dOrderPrice, m_dViewMax, m_dViewMin, m_rctGraphDrawingRegion.top, m_rctGraphDrawingRegion.bottom, m_bLog, m_bReverse);
	

	GetGraphBlockRegion();
	pOrderLine->m_ptStart = CPoint(m_rctGraphRegion.left, nYPoint);
	pOrderLine->m_ptEnd = CPoint(m_rctGraphRegion.right, nYPoint);

}


void COrderLineAddInImp::SetOrderInfomation(CPoint point)
{
	if(m_pSelectedOrderLine->m_nCurClickedPos == CLICKEDPOS_NONE)
		return;

	int nOrderType;
	if(	m_pSelectedOrderLine->m_nCurClickedPos == CLICKEDPOS_ORDERLINE)
	{
		//if(m_pSelectedOrderLine->m_ptStart.y != m_pSelectedOrderLine->m_ptStartChgLine.y
		//	&& m_pSelectedOrderLine->m_ptEnd.y != m_pSelectedOrderLine->m_ptEndChgLine.y)
		if(m_pSelectedOrderLine->m_ptEnd.y != m_ptPrevPoint.y)
			nOrderType = ORDERTYPE_CHANGE;
		else
		{	
			if(m_pSelectedOrderLine->m_bCon)
				nOrderType= ORDERTYPE_CLEAR;
			else
				nOrderType= ORDERTYPE_CANCEL;
		}
//Stop/Limit
		if(m_pSelectedOrderLine->m_IsStop)
		{
			if(m_pSelectedOrderLine->m_bCon)
				nOrderType = ORDERTYPE_CON_STOP_ONLY;
			else
				nOrderType = ORDERTYPE_STOP_ONLY;
		}
		else if(m_pSelectedOrderLine->m_IsLimit)
		{
			if(m_pSelectedOrderLine->m_bCon)
				nOrderType = ORDERTYPE_CON_LIMIT_ONLY;
			else
				nOrderType = ORDERTYPE_LIMIT_ONLY;	
		}
	}
	else if(m_pSelectedOrderLine->m_nCurClickedPos == CLICKEDPOS_TABLE)
	{
		if(m_pSelectedOrderLine->m_ptTableOffset == m_pSelectedOrderLine->m_ptBackupTalbeOffset &&
			m_ptPrevPoint == point)
			nOrderType = ORDERTYPE_CANCEL;
		else 
			return;

		if(m_pSelectedOrderLine->m_bCon)
			nOrderType = ORDERTYPE_CLEAR;

		if(m_pSelectedOrderLine->m_IsStop || m_pSelectedOrderLine->m_IsLimit)
		{
			if(m_pSelectedOrderLine->m_bCon)
				nOrderType = ORDERTYPE_CON_LIMIT;
			else
				nOrderType = ORDERTYPE_LIMIT;
		}	
	}	


	//정정할 가격
	double dYValue;
	dYValue = m_pIChartOCX->ConvertYPositionToData( point.y, m_rctGraphDrawingRegion.top, m_rctGraphDrawingRegion.bottom, m_dViewMax, m_dViewMin, m_bLog, m_bReverse);
	IPacket *pIPacket = m_pIDefaultPacketManager->GetPacket( _MTEXT( C0_CLOSE ));
	if( !pIPacket) return;
	
	CString strType;
	ILPCSTR szIStr = pIPacket->GetType();
	strType = (LPCTSTR)szIStr;
	
	int nFindIndex = strType.Find(".") + 1;
	strType = strType.Mid(nFindIndex, strType.GetLength() - nFindIndex + 1);
	int nTypeLength = strType.GetLength();
	
	
	CString strValue;
	CString strFormat;
	strFormat.Format("%%.%dlf", nTypeLength);
	strValue.Format(strFormat, dYValue);
	dYValue = atof(strValue);

	if( nOrderType == ORDERTYPE_CLEAR || nOrderType == ORDERTYPE_CON_LIMIT || nOrderType == ORDERTYPE_CON_STOP )	// 청산/미청산STOP-LIMIT 이면 주문번호 대신 포지션번호를 보낸다.
		m_pIChartOCX->OnOrderInfomationChanged(dYValue, (int)m_pSelectedOrderLine->m_dAmount, nOrderType,  
												m_pSelectedOrderLine->m_strOrderID,  m_pSelectedOrderLine->m_strCustItem1,
												m_pSelectedOrderLine->m_strPositionID, m_pSelectedOrderLine->m_strCustItem3);
	else
		m_pIChartOCX->OnOrderInfomationChanged(dYValue, (int)m_pSelectedOrderLine->m_dAmount, nOrderType,  
												m_pSelectedOrderLine->m_strOrderID,  m_pSelectedOrderLine->m_strCustItem1,
												m_pSelectedOrderLine->m_strCustItem2, m_pSelectedOrderLine->m_strCustItem3);

}

void COrderLineAddInImp::DrawVertText(HDC p_hDC, CPoint pt)
{
	CDC *pDC = CDC::FromHandle( p_hDC);
	if( !pDC) return;

	double dYValue;
	dYValue = m_pIChartOCX->ConvertYPositionToData( pt.y, m_rctGraphDrawingRegion.top, m_rctGraphDrawingRegion.bottom, m_dViewMax, m_dViewMin, m_bLog, m_bReverse);

	IPacket *pIPacket = m_pIDefaultPacketManager->GetPacket( _MTEXT( C0_CLOSE ));
	if( !pIPacket) return;

	CString strType;
	ILPCSTR szIStr = pIPacket->GetType();
	strType = (LPCTSTR)szIStr;

	int nFindIndex = strType.Find(".") + 1;
	strType = strType.Mid(nFindIndex, strType.GetLength() - nFindIndex + 1);
	int nTypeLength = strType.GetLength();


	CString strValue;
	CString strFormat;
	strFormat.Format("%%.%dlf", nTypeLength);
	strValue.Format(strFormat, dYValue);

	CRect rtTextBox;	
	CRect rtText;
//	if(m_rctRVScaleRegion.PtInRect(pt))
//	{
		TEXTMETRIC tmMetric;
		pDC->GetTextMetrics( &tmMetric);
		long lfontHeight = tmMetric.tmHeight;		
		rtTextBox.left	= m_rctRVScaleRegion.left;
		rtTextBox.right	= m_rctRVScaleRegion.right ;
		rtTextBox.top		= pt.y - lfontHeight / 2 - 2;
		rtTextBox.bottom  = pt.y + lfontHeight / 2 + 3;
		pDC->Rectangle(rtTextBox);		

		CRect rt = rtTextBox;
		rt.left  += 1;
		rt.right -= 1;
		rt.top   += 1;
		rt.bottom -= 1;
		
		pDC->FillSolidRect(rt, RGB(255, 255, 255));
		if(m_pSelectedOrderLine)
		{
			if(m_bOnSetCaprture)
				pDC->FillSolidRect(rt, m_pSelectedOrderLine->m_clrCurBrush);
		}

		rtText = rtTextBox;
		rtText.left += 6;
		rtText.right -= 2;
		rtText.top += 2;
		rtText.bottom += 2;
		pDC->DrawText(strValue, rtText, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
//	}

}

void COrderLineAddInImp::DrawHorzText(HDC p_hDC, CPoint pt)
{
	CDC *pDC = CDC::FromHandle( p_hDC);
	if( !pDC) return;

	IPacket *pIPacket = m_pIDefaultPacketManager->GetPacket( _MTEXT( C0_DATE_TIME ));
	if( !pIPacket) return;


	//x좌표 시간
	int nBlockColumn = 0;
	int nIndex;
	time_t timept = m_pIChartManager->GetTimeFromX(pt.x, &nBlockColumn, nIndex);	
	CTime time(timept);

	CString strType;
	ILPCSTR szIStr = pIPacket->GetType();
	strType = (LPCTSTR)szIStr;

//HHMMSS   //틱
//MMDDHHMM //분
//YYYYMM   //월
//YYYYMMDD //주
//YYYYMMDD //일
	CString strTime;
	int		nWidthText = 0;
	if(time == -1) return;
	if(strType.CompareNoCase("YYYYMMDD") == 0)
	{
		strTime.Format("%4d/%02d/%02d", time.GetYear(), time.GetMonth(), time.GetDay());
	}
	else if(strType.CompareNoCase("YYYYMM") == 0)
	{
		strTime.Format("%4d/%02d", time.GetYear(), time.GetMonth());
	}
	else if(strType.CompareNoCase("MMDDHHMM") == 0)
	{
		strTime.Format("%02d/%02d/%02d:%02d", time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute());
	}
	else if(strType.CompareNoCase("HHMMSS") == 0)
	{
		strTime.Format("%02d:%02d:%02d", time.GetHour(), time.GetMinute(), time.GetSecond());
	}
	
	CRect rtTextBox;	
	CRect rtText;

	TEXTMETRIC tmMetric;
	pDC->GetTextMetrics( &tmMetric);
	long lfontHeight = tmMetric.tmHeight;	
	long lfontWidth  = tmMetric.tmAveCharWidth;
	nWidthText = strTime.GetLength() * lfontWidth + 4;

	rtTextBox.left	= pt.x - nWidthText/2 - 2;
	rtTextBox.right	= pt.x + nWidthText/2 + 2;
	rtTextBox.top	  = m_rctGraphRegion.bottom;
	rtTextBox.bottom  = rtTextBox.top + lfontHeight + 6;
	pDC->Rectangle(rtTextBox);

	CRect rt = rtTextBox;
	rt.left  += 1;
	rt.right -= 1;
	rt.top   += 1;
	rt.bottom -= 1;
	pDC->FillSolidRect(rt, RGB(255, 255, 255));

	if(m_pSelectedOrderLine)
	{
		if(m_bOnSetCaprture)
			pDC->FillSolidRect(rt, m_pSelectedOrderLine->m_clrCurBrush);
	}


	rtText = rtTextBox;
	rtText.left += 2;
	rtText.right -= 2;
	rtText.top += 2;
	rtText.bottom -= 2;
	pDC->DrawText(strTime, rtText, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

}

void COrderLineAddInImp::DrawCrossLine(HDC p_hDC, CPoint pt)
{
	CDC *pDC = CDC::FromHandle( p_hDC);
	if( !pDC) return;

	IPacket *pIPacket = m_pIDefaultPacketManager->GetPacket( _MTEXT( C0_DATE_TIME ));
	if( !pIPacket) return;

	pDC->MoveTo(m_rctGraphRegion.left, pt.y);
	pDC->LineTo(m_rctGraphRegion.right, pt.y);

	pDC->MoveTo(pt.x, m_rctGraphRegion.top);
	pDC->LineTo(pt.x, m_rctGraphRegion.bottom);
}

void COrderLineAddInImp::InitToolTip()
{
	if(m_ToolTip.m_hWnd != NULL)
		return;
	
	m_ToolTip.Create(m_pChartCWnd);
	m_ToolTip.Activate(FALSE);
	m_ToolTip.SetMaxTipWidth(200);
}

void COrderLineAddInImp::AddToolInTooltip(const CString& strText, const bool bActivate)
{
	// tooltip의 text가 없을 경우에 추가 한다.
	if(m_ToolTip.GetToolCount() == 0)
	{
		CRect rectBtn; 
		GetClientRect(m_pChartCWnd->GetSafeHwnd(), rectBtn);
		m_ToolTip.AddTool(m_pChartCWnd, (LPCTSTR)strText, rectBtn, 1);
	}
	
	// ooltip text 를 set
	m_ToolTip.UpdateTipText((LPCTSTR)strText, m_pChartCWnd, 1);
	m_ToolTip.Activate(bActivate);
}

void COrderLineAddInImp::SetToolTipText(const CString& strText)
{
	m_strToolTipText = strText;
}

void COrderLineAddInImp::SetToolTipText(const CString& strText, const bool bActivate)
{
	if(strText.IsEmpty())
		return;
	
	InitToolTip();
	AddToolInTooltip(strText, bActivate);
}


///////////////////////////////////////////////////////////////////////////////////////////
BOOL COrderLineAddInImp::SetCommentStatusOnMouseMove(CPoint point)
{
	CCommentItem *pItem = NULL;

	m_strToolTipText.Empty();

	int nSize = m_arrCommentList.GetSize();
	for (int i=0; i<nSize; i++)
	{
		pItem = m_arrCommentList.GetAt(i);
		if( pItem && pItem->IsInRegion(point) )
		{
			if( !m_strToolTipText.IsEmpty() )
				m_strToolTipText += "\r\n\r\n";
			m_strToolTipText += pItem->GetToolTipText();
		}
	}

	if( !m_strToolTipText.IsEmpty() )
		return TRUE;
	return FALSE;
}

void COrderLineAddInImp::SetRemove_CommentData()
{
	CCommentItem *pItem = NULL;
	int nSize = m_arrCommentList.GetSize();
	for (int i=0; i<nSize; i++)
	{
		pItem = m_arrCommentList.GetAt(i);
		if (pItem != NULL)
		{
			delete pItem;
			pItem = NULL;
		}
	}
	m_arrCommentList.RemoveAll();

	if (m_boldFont.GetSafeHandle() == NULL)
		m_boldFont.DeleteObject();
}

void COrderLineAddInImp::SetCommentData(FX_CHE_STRUCT* pDataPosition)
{
	if( memcmp(pDataPosition->szKeyType, "CH01", 4) != 0 )
		return;



	int nStartIndex, nEndIndex;
	long lDate;

	if (!m_pIChartOCX->GetViewStartEndIndex(nStartIndex, nEndIndex))
		return;


	CString strDateTime(pDataPosition->szExeTime, sizeof(pDataPosition->szExeTime));
	CString strComment;
	CString strIsBuy;

	if( memcmp(pDataPosition->szSide, "082", 3) == 0 || memcmp(pDataPosition->szSide, "079", 3) == 0 )
		strIsBuy = "1";
	else
		strIsBuy = "0";

	if( memcmp(pDataPosition->szTradeType, "11", 2) == 0 )
		strComment = "진입";
	else if( memcmp(pDataPosition->szTradeType, "12", 2) == 0 )
		strComment = "청산";
	else if( memcmp(pDataPosition->szTradeType, "13", 2) == 0 )
		strComment = "마진콜";
	else if( strIsBuy == "1" )		// 해외선물 매매
		strComment = "매수";
	else if( strIsBuy == "0" )		// 해외선물 매매
		strComment = "매도";
	else
		return;
	

	CString strTradeType(pDataPosition->szTradeType, sizeof(pDataPosition->szTradeType));
	strTradeType.TrimLeft();			strTradeType.TrimRight();
	CString strPositionID(pDataPosition->szPositionID, sizeof(pDataPosition->szPositionID));
	strPositionID.TrimLeft();			strPositionID.TrimRight();
	CString strFXCMPosID(pDataPosition->szFXCMPosID, sizeof(pDataPosition->szFXCMPosID));
	strFXCMPosID.TrimLeft();			strFXCMPosID.TrimRight();

	CString strPrice(pDataPosition->szRate, sizeof(pDataPosition->szRate));
	strPrice.TrimLeft();		strPrice.TrimRight();

	// 진입하고 청산하지 않은 경우는 잔고내역과 체결내역 양쪽에 존재하므로 체크하여 중복등록하지 않도록 한다.
	if( memcmp(pDataPosition->szTradeType, "11", 2) == 0 )
	{
		CCommentItem *pCheckItem = NULL;
		int nSize = m_arrCommentList.GetSize();
		for (int i=0; i<nSize; i++)
		{
			pCheckItem = m_arrCommentList.GetAt(i);
			if (pCheckItem)
			{
				/*
				if( pCheckItem->m_strDateTime == strDateTime 
						&& pCheckItem->m_bBuy == atoi(strIsBuy)
						&& pCheckItem->m_strPrice == strPrice
						&& pCheckItem->m_fAmount	== pDataPosition->fAmount )
				*/
				if( strPositionID.GetLength() > 0 && strPositionID == pCheckItem->m_strPositionID )
					return;
			}
		}
	}

	strDateTime.TrimLeft();			strDateTime.TrimRight();
	strDateTime.Remove(':');
	if( strDateTime.GetLength() < 14 )		// YYYYMMDDHHMMSS 가 아니면 리턴tour2k.2022.7.27
		return;
	int nIndex = GetIndexFromValue(strDateTime, lDate);
	if( nIndex < 0 )
		return;

	CCommentItem* pItem	= new CCommentItem;


	pItem->m_nIndex			= nIndex;
	pItem->m_strDate.Format("%d", (int)lDate);
	pItem->m_bBuy			= atoi(strIsBuy);
	pItem->m_strDateTime	= strDateTime;
	pItem->m_strTradeType	= strTradeType;
	pItem->m_strComment		= strComment;
	pItem->m_strPrice		= strPrice;
	pItem->m_fAmount		= pDataPosition->fAmount;
	pItem->m_strPositionID	= strPositionID;
	pItem->strFXCMPosID		= strFXCMPosID;

	m_arrCommentList.Add(pItem);

	m_pIChartOCX->InvalidateControl();
}

//////////////////////////////////////////////////////////////////////////
// 데이터로부터 인넥스 얻기
int COrderLineAddInImp::GetIndexFromValue(CString strTime, long& lDate)
{
	IPacket *pIPacket = m_pIDefaultPacketManager->GetPacket( _MTEXT( C0_DATE_TIME));
	if( !pIPacket) return -1;

	CString strType;
	ILPCSTR szIStr = pIPacket->GetType();
	strType = (LPCTSTR)szIStr;


	CString strDateTime = "";
	if( strType.Find("YYYY") >= 0 )
		strDateTime += strTime.Left(4);
	if( strType.Find("YYYYMM") >= 0 || strType.Find("MMDD") >= 0)
		strDateTime += strTime.Mid(4, 2);
	if( strType.Find("DD") >= 0 || strType.Find("DD") >= 0 )
		strDateTime += strTime.Mid(6, 2);
	if( strType.Find("HH") >= 0 )
		strDateTime += strTime.Mid(8, 2);
	if( strType.Find("HHMM") >= 0 || strType.Find("MMSS") >= 0 )
		strDateTime += strTime.Mid(10, 2);
	if( strType.Find("SS") >= 0 )
		strDateTime += strTime.Mid(12, 2);

	int nIndex = pIPacket->GetIndexFromData( atof(strDateTime) );
	if( nIndex >= pIPacket->GetDataCount() )
		nIndex = -1;

	pIPacket->Release();

	return nIndex;
}	

