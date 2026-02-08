// Radar.cpp : implementation file
//

#include "stdafx.h"
#include "Radar.h"

#include "../Include_Chart/Dll_Load/PacketList.h"
#include "../Include_Chart/Dll_Load/IMetaTable.h"			// for _MTEXT()
#include "../Include_Chart/IChartCtrl.h"					// for IChartCtrl

#include "Conversion.h"
#include "ToolBarDlg.h"

/*
#include "../ChartObject/PacketRQ.h"
#include "../Include_Chart/DLL_Load/PacketListManager.h"
#include "../ChartObject/PacketListManagerImplementation.h"
#include "../Include_Chart/DLL_Load/XScaleManager.h"
*/

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRadar

CRadar::CRadar( IChartCtrl *p_pIChartCtrl)
{
	m_strDataName = "";
	m_pPacketList = NULL;

	// (2003.12.03, 배승원) Zoom & Scroll 관련 기능을 정리한다.
	m_dXScale			= 1;
	m_dYScale			= 1;
	m_nDataStartIndex	= 0;
	m_nOnePageDataCount	= 1;
	m_nRadarDataLeft	= 1;
	m_nRadarDataRight	= -1;
	m_dRadarDataMin		= 0;
	m_dRadarDataMax		= 0;

	// (2003.12.03) Zoom Full Mode Flag을 관리하여,
	//		해당 Mode에서는 Mouse Tracing외의 작업을 제한한다.
	m_bZoomFullMode		= FALSE;

	// (2003.12.04, 배승원) 구간 선택 기능을 추가한다. Drag Select 영역을 초기화한다.
	//		(Dragging 중임을 나타내는 좌우 영역만 초기화한다.)
	m_nDragSelectStart	= 0;
	m_nDragSelectEnd	= 0;

	// (2006/11/18 - Seung-Won, Bae) Notify the User Zoom State to MultiChart
	m_pIChartCtrl = p_pIChartCtrl;
}


BEGIN_MESSAGE_MAP(CRadar, CWnd)
	//{{AFX_MSG_MAP(CRadar)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRadar message handlers

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2003.12.02
//
//	분석	:	Radar Chart를 출력한다.
//
//	수정	:	(2003.12.03) 선물,옵션 Data도 문안히 Mapping할 수 있도록 100배된 Data로 처리한다.
//				(2003.12.03) 불필요한 Drawing을 줄이기 위하여, Index를 건너뛴다.
//				(2003.12.04, 배승원, 수정) LButtonDown시나 Dragging에 의한 Client 벗어사는 경우마다,
//					Drawing이 다시 발생하는 것을 제한하기 위하여 CButton이 아니라 CWnd에서 계승받는다.
//					Drawing도 DrawItem()이 아니라, OnPaint에서 처리한다.
///////////////////////////////////////////////////////////////////////////////
void CRadar::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CWnd::OnPaint() for painting messages

	// 0. 먼저 설정된 Data Packet Name과 Packet List Pointer를 확인한다.
	if( m_strDataName.IsEmpty() || m_pPacketList == NULL)	return;

	// 1. 출력할 Packet Object를 구한다.	
	CPacket* pPacket = m_pPacketList->GetBaseData( m_strDataName);
	if( !pPacket) return;

	// 2. 선택 영역까지 Graph의 배경을 처리한다.
	// 2.1 기본 배경을 처리한다.
	dc.FillSolidRect( m_rctRadarClient, RGB( 255, 255, 255));
	dc.DrawEdge( ( LPRECT)m_rctRadarClient, EDGE_SUNKEN, BF_RECT);

	// 3. Graph를 그릴 Data가 없는 경우 처리를 제외한다.
	CList<double, double>* datalist = pPacket->GetnumericDataList();
	POSITION pos = datalist->GetHeadPosition();
	if( !pos) return;

	// 4. 선택 영역의 배경을 처리한다. (Data가 없는 곳으로 가면 역전된다.)
	if( m_rctRadarSelect.left < m_rctRadarSelect.right)
		dc.FillSolidRect( m_rctRadarSelect, RGB(239, 231, 222));

	// 5. Graph를 처리한다.
	SIZE sizOldViewportExt;
	POINT ptOldWindowOrg, ptOldViewportOrg;
	// 5.0 Graph를 그릴 Pen을 생성/설정한다.
	CPen pen( PS_SOLID, 1, RGB( 218, 46, 46));
	CPen* pOldPen = dc.SelectObject(&pen);
	// 5.1 Mapping Mode를 설정한다.
	HDC hDC = dc.GetSafeHdc();
	int nPreMapMode = SetMapMode( hDC, MM_ANISOTROPIC);
	SetWindowOrgEx( hDC, m_nRadarDataLeft, ( int)m_dRadarDataMin, &ptOldWindowOrg);
	SetViewportOrgEx( hDC, m_rgnRadarGraph.left, m_rgnRadarGraph.bottom, &ptOldViewportOrg);
	SetViewportExtEx( hDC, 1, -1, &sizOldViewportExt);

// (2003.12.03, 배승원, Debug) Mapping 좌표로 정확한 Full Rect의 Drawing에 성공하여야 한다.
// (2003.12.03, 배승원) X Scale의 마지막 Index도 다른 것과 동등한 폭을 갖도록 한다.
//		Right를 구할때 왜 m_nRadarDataRight+1의 변환에 다시 1을 빼었는지를 확실히 이해하여야 한다.
//	int nLeft	= m_nRadarDataLeft;
//	int nTop	= int( m_dRadarDataMin + 0.5);
//	int nRight	= int( ( m_nRadarDataRight + 1 - m_nRadarDataLeft) * m_dXScale + 0.5) + m_nRadarDataLeft - 1;
//	int nBottom	= int( ( m_dRadarDataMax - m_dRadarDataMin) * m_dYScale + 0.5 + m_dRadarDataMin);
// 0. 주의 이 Test시에는 반듯이 각각의 Scale Factor가 0이 아니어야 정상적인 그림이 나타난다. (m_dXScale, m_dYScale)
// 1. 주의 Rectangle과 아래의 LineTo가 무엇이 다른지 정확히 이해하여야 한다.
//	dc.Rectangle( nLeft, nTop, nRight, nBottom);
// 2. Rectangle의 Width(), Height() 값이 무엇을 의미하는지 알아야 하고,
//		함부로 이용하지 말아야 한다. (Rectangle이 아닌 개념을 Region으로 명시했다.)
//		또한 LineTo의 경우 지정된 Point에는 칠하지 않는 것을 주의한다.
//	dc.MoveTo( nLeft, nTop);
//	dc.LineTo( nLeft, nBottom);
//	dc.LineTo( nRight, nBottom);
//	dc.LineTo( nRight, nTop);
//	dc.LineTo( nLeft, nTop);

	// 5.2 Graph를 출력한다.
	// 5.2.1 Data가 한 개 이하의 경우를 처리한다.
	//	(Scale Factor 값이 0인 경우로 그 한 값을 중심에 선으로 그린다.)
	if( m_nRadarDataRight <= m_nRadarDataLeft)
	{
		// 5.2.1.1 첫 Point 위치로 이동한다.
		// (2003.12.03) 선물,옵션 Data도 문안히 Mapping할 수 있도록 100배된 Data로 처리한다.
		double data = datalist->GetAt(pos) * 100;
		int nOneValue = int( ( m_rgnRadarGraph.bottom - m_rgnRadarGraph.top + 1) / 2 + m_dRadarDataMin);
		dc.MoveTo( m_rgnRadarGraph.left, nOneValue);
		// 5.2.1.2 끝 Point 위치로 선을 그린다. 마지막 Line이 Graph영역 끝에 닿도록 다음 X 좌표로 Line을 좀더 그린다.
		//	(LineTo가 마지막 Point를 칠하지 않는다. Reference 참조)
		dc.LineTo( m_rgnRadarGraph.right + 1, nOneValue);
	}
	// 5.2.2 Data가 두 개 이상의 경우를 처리한다.
	else 
	{
		// 5.2.2.1 첫 Point 위치로 이동한다.
		int i = m_nRadarDataLeft;
		int nXOldValue = i;
		// (2003.12.03) 선물,옵션 Data도 문안히 Mapping할 수 있도록 100배된 Data로 처리한다.
		dc.MoveTo( int( ( i++ - m_nRadarDataLeft) * m_dXScale + 0.5) + m_nRadarDataLeft,
			int( ( datalist->GetNext(pos) * 100 - m_dRadarDataMin) * m_dYScale + 0.5 + m_dRadarDataMin));
		// 5.2.2.2 Data 부분을 이동하면서 Line을 그린다.
		double dPacketValue;
		int nYValue, nXValue; 
		while( i <= m_nRadarDataRight)
		{
			if(pos == NULL) break;

			dPacketValue = datalist->GetNext(pos);

			// (2003.12.03) 불필요한 Drawing을 줄이기 위하여, Index를 건너뛴다.
			nXValue = int( ( i - m_nRadarDataLeft) * m_dXScale + 0.5) + m_nRadarDataLeft;
			if( nXOldValue != nXValue)
			{
				nXOldValue = nXValue;

				// (2003.12.03) 선물,옵션 Data도 문안히 Mapping할 수 있도록 100배된 Data로 처리한다.
				nYValue = int( ( dPacketValue * 100 - m_dRadarDataMin) * m_dYScale + 0.5 + m_dRadarDataMin);
				dc.LineTo( nXValue, nYValue);
			}

			i++;
		}
		// 5.2.2.3 마지막 Line이 Graph영역 끝에 닿도록 다음 X 좌표로 Line을 좀더 그린다.
		//	(LineTo가 마지막 Point를 칠하지 않는다. Reference 참조)
		// (2003.12.03, 배승원) X Scale의 마지막 Index도 다른 것과 동등한 폭을 갖도록 한다.
		//		(1 Pxcel이 아니라 1 Data 단위의 위치까지 그린다. 물론 LineTo니까 다음 Data의 것은 그려지지 않는다.)
		dc.LineTo( int( ( m_nRadarDataRight + 1 - m_nRadarDataLeft) * m_dXScale + 0.5) + m_nRadarDataLeft, 
			nYValue);
	}

	// 6. 구간선택중이 아닌 경우를 제외한다.
	if( m_nDragSelectStart == 0 && m_nDragSelectEnd == 0)
	{
		// 6.1 마무리 작업으로 초기의 Pen으로 복원한다.
		dc.SelectObject(pOldPen);
		return;
	}

	// (2003.12.04, 배승원) 구간 선택 기능을 추가한다.
	//		구간선택 처리중에는 Real이 못오게 제한하지만, 특별한 경우의 RePaint를 대비하여 복원기능을 제공한다.
	// 6.1 Mapping Mode를 복원한다.
	SetMapMode( hDC, nPreMapMode);
	SetWindowOrgEx( hDC, ptOldWindowOrg.x, ptOldWindowOrg.y, NULL);
	SetViewportOrgEx( hDC, ptOldViewportOrg.x, ptOldViewportOrg.y, NULL);
	SetViewportExtEx( hDC, sizOldViewportExt.cx, sizOldViewportExt.cy, NULL);
	// 6.2 Dragging 영역을 출력한다.
	DrawDragSelectRectangle( &dc, m_nDragSelectStart, m_nDragSelectEnd);

	// 7. 초기의 Pen으로 복원한다.
	dc.SelectObject(pOldPen);
}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2003.12.03
//
//	목적	:	Data View 정보를 전달받아 Radar Chart에 반영하는 Interface를 제공한다.
//
//	내용	:	1. Data View의 정보 뿐만아니라 Drawing에 필요한 기본 Data도 산출한다.
//
//	수정	:	(2003.12.03) X Scale의 마지막 Index도 다른 것과 동등한 폭을 갖도록 한다.
//				(2003.12.03) 선물,옵션 Data도 문안히 Mapping할 수 있도록 100배된 Data로 처리한다.
//				(2003.12.03) Zoom Full Mode 여부와 조정된 Data View 정보를 함께 전달받아 처리한다.
//					(Radar의 경우 Zoom Full Mode여도 Data를 지속적으로 반영한다.)
//				(2003.12.04) 구간 선택 중인 경우에는 Real을 반영하지 않는다.
//					선택 구간을 ReDraw를 하여 보여주면되지만, OnPaint의 DC에서 XOR_PEN 처리가 잘 안되는 것같고,
//					또한 사용자의 구간 선택 정확성을 위하여 제한한다.
///////////////////////////////////////////////////////////////////////////////
void CRadar::SetDataViewInfo( int nDataSize, int nDataStartIndex, int nOnePageDataCount, BOOL p_bZoomFullMode)
{
	// (2003.12.04) 구간 선택 중인 경우에는 Real을 반영하지 않는다.
	if( m_nDragSelectStart != 0 || m_nDragSelectEnd != 0) return;

	// (2003.12.03) Zoom Full Mode 여부와 조정된 Data View 정보를 함께 전달받아 처리한다.
	//		(Radar의 경우 Zoom Full Mode여도 Data를 지속적으로 반영한다.)
	if( p_bZoomFullMode)
	{
		nOnePageDataCount = nDataSize;
		// 종찬(04/11/05) Zero Base로 수정한다.
		nDataStartIndex = 0;
	}

	// 0. Data View의 설정을 관리한다.
	m_nDataStartIndex = nDataStartIndex;
	m_nOnePageDataCount = nOnePageDataCount;
	// 2. Graph Data의 Min/Max 및 Index 구간도 관리한다.
	m_nRadarDataLeft	= ( nDataSize == 0 ? 0 : 1);
	m_nRadarDataRight	= nDataSize;
	m_dRadarDataMin		= m_dRadarDataMax	= 0;

	if( m_pPacketList && nDataSize)
	{
		CPacket *pPacket = m_pPacketList->GetBaseData(m_strDataName);
		// (주의, GetMinMax는 0 Base Index를 이용한다.)
		// (2003.12.03) 선물,옵션 Data도 문안히 Mapping할 수 있도록 100배된 Data로 처리한다.
		if( pPacket)
		{
			pPacket->GetMinMax( 0, nDataSize - 1, m_dRadarDataMin, m_dRadarDataMax);
			m_dRadarDataMin *= 100;
			m_dRadarDataMax *= 100;
		}
	}
/*
	if(m_pPacketListManager && nDataSize)
	{
		int nRQ, nCountOfRQ;
		CPacketList* ppacketList;
		double dRadarDataMin, dRadarDataMax;

		MAP_RQ_PACKETLIST* pmapPacketList = (MAP_RQ_PACKETLIST*)m_ppacketListManager->GetMapPacketList();
		nCountOfRQ = pmapPacketList->size();

		MAP_RQ_PACKETLIST::iterator itRQ = pmapPacketList->begin();

		while(itRQ != pmapPacketList->end())
		{
			ppacketList = itRQ->second->GetPacketList();
			ppacketList->GetMinMax(0, nDataSize - 1, dRadarDataMin, dRadarDataMax);

			if(m_dRadarDataMin > dRadarDataMin) m_dRadarDataMin = dRadarDataMin;
			if(m_dRadarDataMax < dRadarDataMax) m_dRadarDataMax = dRadarDataMax;
		}

		m_dRadarDataMin *= 100;
		m_dRadarDataMax *= 100;
	}
*/
	// (2004.04.16, 배승원) Radar의 Resize시에도, Resize된 Graph를 보이도록,
	//		Radar Graph를 재구성하는 Routine을 별도로 제공한다.
	DrawNewRadarView();

	// (2003.12.03) Zoom Full Mode Flag을 관리하여,
	//		해당 Mode에서는 Mouse Tracing외의 작업을 제한한다.
	m_bZoomFullMode = p_bZoomFullMode;

	// (2003.12.03) Data View의 정보가 재설정될 경우 Radar Status Static에게,
	//		중심 Index의 값을 전달하도록 처리한다.
	OnMouseMove( NULL, m_rctRadarSelect.CenterPoint());
}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2003.12.03
//
//	분석	:	1. Radar Chart의 초기설정 Interface이다.
//				2. Packet List Pointer와 출력할 Packet Name 전달받아 관리한다.
//				3. Indicator Info는 선색을 처리하는데 이용되었으나, 배경색이 조절되지 않는 관계로 기능을 제거하였다.
//				4. 실 Data는 On Draw시에 직접 Access하므로 종목이 바뀌는등 Packet에 변화가 있어도 문제되지 않는다.
//
//	수정	:	(2003.12.04, 배승원) 구간 선택을 통지할 Main OCX의 Pointer도 함께 관리한다.
///////////////////////////////////////////////////////////////////////////////
void CRadar::SetData(CPacketList *pPacketList, CPacketListManager* pPacketListManager, CXScaleManager* pxScaleManager, CString& strDataName)
{
	if(pPacketList == NULL || strDataName.IsEmpty())	return;

	m_strDataName	= strDataName;
	m_pPacketList	= pPacketList;
	m_pPacketListManager = pPacketListManager;
	m_pxScaleManager = pxScaleManager;
}

void CRadar::OnMouseMove(UINT nFlags, CPoint point) 
{
	// 0. 먼저 자료가 없는 경우를 처리한다.
	if( m_nRadarDataLeft == 0 && m_nRadarDataRight == 0)
	{
		CWnd::OnMouseMove(nFlags, point);
		return;
	}

	// 1. Mouse의 위치를 확인하여 Radar Status Static에 출력시킨다.
	// 1.1 Data Index를 산출해 낸다.
	int nDataIndex = GetDataIndex( point.x);
	// 1.3 해당 Data Index의 자료 일자를 구하여 Radar Status Static에 알려 출력토록 한다.
	// 1.3.1 일자 Data를 구한다.
	CPacket *pDateTimePacket = m_pPacketList->GetBaseData( _MTEXT( C0_DATE_TIME));
	
	if(pDateTimePacket == NULL) return;
	if(pDateTimePacket->GetCount() == 0) return;

	CString strDataTime = CDataConversion::DoubleToString( pDateTimePacket->GetData( nDataIndex - 1));	// 주의 0 Base
	strDataTime = CDataConversion( m_pIChartCtrl->GetSafeHwnd(), m_pIChartCtrl->GetChartMode()).GetStringFormatTrans( strDataTime, pDateTimePacket->GetType(), "");
	// 1.3.2 가격 Data를 구한다.
	CPacket *pValuePacket = m_pPacketList->GetBaseData(m_strDataName);
	CString strValue = CDataConversion::DoubleToString( pValuePacket->GetData( nDataIndex - 1));	// 주의 0 Base
	strValue = CDataConversion( m_pIChartCtrl->GetSafeHwnd(), m_pIChartCtrl->GetChartMode()).GetStringFormatTrans( strValue, pValuePacket->GetType());
	// 1.3.3 Radar의 Index 정보를 Radar Status Static에게 전달한다.
	if( m_pstRadarStatus) m_pstRadarStatus->SetWindowText( strDataTime + "\n" + strValue);

	// (2003.12.03) Zoom Full Mode Flag을 관리하여, 해당 Mode에서는 Mouse Tracing외의 작업을 제한한다.
	// (2003.12.04) 구간 설정을 하는 중이 아닌 경우도 제외한다.
	if( m_bZoomFullMode || m_nDragSelectStart == 0)
	{
		CWnd::OnMouseMove(nFlags, point);
		return;
	}

	// (2003.12.04) 구간 설정을 처리를 계속 진행한다.
	// 1. Dragging 영역을 Boxing한다.
	//		(DrawDragSelectRectangle내에서 m_nDragSelectEnd가 최신의 것으로 Upate된다.)
	CDC *pDC = GetDC();
	DrawDragSelectRectangle( pDC, m_nDragSelectStart, point.x, &m_nDragSelectEnd);
	ReleaseDC( pDC);

	CWnd::OnMouseMove(nFlags, point);
}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2003.12.04
//
//	목적	:	Dragging 영역을 출력하는 Routine을 별도로 제공한다.
//
//	주의	:	1. 호출 Routine에서 Start와 End가 역전되는 것을 바로잡아 주지 않는다.
//					그렇다고 여기서 바로잡는 것도 좋은 방법은 아니다.
//					Rectangle이 어느 순간부터 역전되는데 그것을 XOR로 Clear하지 못하는 경우가 발생하기 때문이다.
//					따라서 항상 역정될 가능성에 대해 고려하여 Coding한다.
///////////////////////////////////////////////////////////////////////////////
void CRadar::DrawDragSelectRectangle( CDC *pDC, long p_nDragSelectStart, long p_nDragSelectEnd, long *p_nPreDragSelectEnd)
{
	// 0. 먼저 이전과 이후의 Dragging End가 같은 경우를 제외한다.
	if( p_nPreDragSelectEnd && p_nDragSelectEnd == *p_nPreDragSelectEnd) return;

	// 0.5 주의, 왜 이렇게 할까? (Pen Size가 2인 경우라면 선이 m_rgnRadarGraph영역 밖으로 나가겠지?
	//		물론 여기서 Data Index를 조정하지 않으니까 실제적으로 그 선택영역을 조절하는 것은 아니다.
	if( p_nDragSelectStart	== m_rgnRadarGraph.left) p_nDragSelectStart++;
	if( p_nDragSelectEnd	== m_rgnRadarGraph.left) p_nDragSelectEnd++;

	// 1. Hatch Pen, XOR Mode로 설정한다.
	//		(참고, Pen Style중에 PS_ALTERNATE이 Win9x를 지원하지 않아 Pettern Pen으로 처리한다.
	//			또한 일반 PS_DOT나 BS_HATCHED는 그 점단위가 3 Pixel이어서 맞지않는다.)
	WORD waHatchBits[ 2] = { 0xAA, 0x55};
	CBitmap bmHatchBitmap;
	bmHatchBitmap.CreateBitmap( 8, 2, 1, 1, waHatchBits);
	LOGBRUSH lbrBrushLog = { BS_PATTERN, RGB( 0, 0, 0), ( long)( HBITMAP)bmHatchBitmap};
	// 2. 왜 크기 1의 Pen을 구하는지는 3.6 참조
	CPen pnDragPen( PS_GEOMETRIC, 1, &lbrBrushLog);
	CPen *pOldPen = pDC->SelectObject( &pnDragPen);
	pDC->SetROP2( R2_XORPEN);
	pDC->SetBrushOrg( 0, 1);
	// 3. Dragging 영역을 Boxing한다.
	//		(주의, Pen의 크기가 2인 경우 지정된 좌표보다 좌측과 윗쪽으로 선 굵기가 굵어지고,
	//			또한 Reference와 달리 마지막 Point도 칠하는데, 의외로 수평선의 경우 첫 출발점만 굵기가 1이다.
	//			다음의 예를 잘 이해하여야 한다.)
	// 3.1 Demo 1 (빈틈없이 다 찬다. Reference와 달리 마지막 Point도 칠한다.)
	//	pdc->MoveTo( p_nDragSelectStart + 1, m_rgnRadarGraph.top);
	//	pdc->LineTo( p_nDragSelectStart + 1, m_rgnRadarGraph.bottom);
	// 3.2 Demo 2 (Reference와 달리 마지막 Point도 칠하는데, 출발점의 선굵기만 1이다.)
	//	pdc->MoveTo( p_nDragSelectStart, m_rgnRadarGraph.bottom);
	//	pdc->LineTo( p_nDragSelectEnd, m_rgnRadarGraph.bottom);
	// 3.3 Demo 3 (빈틈없이 다 찬다. Reference와 달리 마지막 Point도 칠한다.)
	//	pdc->MoveTo( p_nDragSelectEnd, m_rgnRadarGraph.bottom);
	//	pdc->LineTo( p_nDragSelectEnd, m_rgnRadarGraph.top);
	// 3.4 Demo 4 (Reference와 달리 마지막 Point도 칠하는데, 출발점의 선굵기만 1이다.)
	//	pdc->MoveTo( p_nDragSelectEnd, m_rgnRadarGraph.top + 1);
	//	pdc->LineTo( p_nDragSelectStart, m_rgnRadarGraph.top + 1);
	// 3.5 Boxing by LineTo Demo
	//		LineTo의 반복 사용에 대한 비효율성과 XOR ROP에 의한 겹칩시의 오류의 추가 관리가 필요하여
	//		참고안으로만 정리하여 놓는다.
	//	int nDragSelectLeft = p_nDragSelectStart + 1;
	//	int nDragSelectTop = m_rgnRadarGraph.top + 1;
	//	pdc->MoveTo( nDragSelectLeft, m_rgnRadarGraph.top);
	//	pdc->LineTo( nDragSelectLeft, m_rgnRadarGraph.bottom);
	//	pdc->LineTo( p_nDragSelectEnd, m_rgnRadarGraph.bottom);
	//	pdc->LineTo( p_nDragSelectEnd, m_rgnRadarGraph.top);
	//	pdc->MoveTo( p_nDragSelectEnd, m_rgnRadarGraph.top + 1);
	//	pdc->LineTo( p_nDragSelectStart, nDragSelectTop);
	// 3.6 Boxing by Rectangle
	//		왜 OffsetRect를 하는지 정확히 이해해야 한다. Region이 아니라 Rectangle인 점과 Pen 굵기가 2인 점을 감안한 것이다.
	//		Rectangle을 이용하여도 위의 문제로 양측 맨끝 상단의 1 Pixel이 처리되지 않는다.
	//		이를 해결하기 위하여 크기 1의 Pen으로 2차에 걸쳐 Drawing한다.
	HGDIOBJ hbrOldBrush = pDC->SelectObject( GetStockObject( NULL_BRUSH));
	// 3.6.1 먼저 이전의 Box를 Clear한다.
	if( p_nPreDragSelectEnd) DrawRectangleInRegion( pDC, p_nDragSelectStart, *p_nPreDragSelectEnd);
	// 3.6.2 새로운 영역을 Boxing한다.
	DrawRectangleInRegion( pDC, p_nDragSelectStart, p_nDragSelectEnd);

	pDC->SelectObject( hbrOldBrush);

	// 4. 초기의 Pen으로 복원한다.
	pDC->SelectObject( pOldPen);

	// 5. 끝으로 Dragging End를 Backup한다.
	if( p_nPreDragSelectEnd) *p_nPreDragSelectEnd = p_nDragSelectEnd;
}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2003.12.04
//
//	목적	:	Radar의 구간을 선택하는 UI를 제공한다.
//
//	내용	:	(2003.12.04) 자료가 없는 경우도 제외한다.
///////////////////////////////////////////////////////////////////////////////
void CRadar::OnLButtonDown(UINT nFlags, CPoint point) 
{	
	// (2003.12.03) Zoom Full Mode Flag을 관리하여, 해당 Mode에서는 Mouse Tracing외의 작업을 제한한다.
	// (2003.12.04) 자료가 없는 경우도 함께 제외한다.
	if( m_bZoomFullMode || m_nRadarDataLeft == 0 && m_nRadarDataRight == 0)
	{
		CWnd::OnLButtonDown(nFlags, point);
		return;
	}

	// 1. Mouse Message를 Capture하고 Dragging Flag을 설정한다.
	SetCapture();
	// 2. Dragging 영역의 시작과 끝을 초기화한다.
	// 2.1 먼저 해당 Data Index를 확인하여, 유효하도록 보정한다.
	GetDataIndex( point.x);
	// 2.2 Dragging 영역의 시작과 끝을 초기화한다.
	m_nDragSelectStart = point.x;
	m_nDragSelectEnd = point.x;
	// 3. 첫 Dragging 영역을 출력한다.
	CDC *pDC = GetDC();
	DrawDragSelectRectangle( pDC, m_nDragSelectStart, m_nDragSelectEnd);
	ReleaseDC( pDC);

	CWnd::OnLButtonDown(nFlags, point);
}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2003.12.04
//
//	목적	:	Radar의 구간을 선택하는 UI를 제공한다.
//
//	내용	:	(2003.12.04) 자료가 없는 경우도 제외한다.
//				(2003.12.05) Radar 밖에서 LButton을 누르고 들어오는 경우를 제외한다.
///////////////////////////////////////////////////////////////////////////////
void CRadar::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// (2003.12.03) Zoom Full Mode Flag을 관리하여, 해당 Mode에서는 Mouse Tracing외의 작업을 제한한다.
	// (2003.12.04) 자료가 없는 경우도 함께 제외한다.
	// (2003.12.05) Radar 밖에서 LButton을 누르고 들어오는 경우를 제외한다.
	if( m_bZoomFullMode || ( m_nRadarDataLeft == 0 && m_nRadarDataRight == 0) || m_nDragSelectStart == 0)
	{
		CWnd::OnLButtonUp(nFlags, point);
		return;
	}

	// 1. Mouse Message Capture를 해제하고, Dragging Flag을 해제한다.
	ReleaseCapture();
	// 2. Dragging 영역을 Boxing한다.
	//		(DrawDragSelectRectangle내에서 m_nDragSelectEnd가 최신의 것으로 Upate된다.)
	// 2.1 먼저 해당 Mouse 위치를 확인하여, 유효하도록 보정한다.
	//		(주의, 왜 m_nDragSelectEnd를 DrawDragSelectRectangle()를 통하여 설정받는지 아는가?
	//			GetDataIndex()의 주의사항 참조)
	GetDataIndex( point.x);
	// 2.2 Dragging Rectangle을 Drawing한다.
	CDC *pDC = GetDC();
	DrawDragSelectRectangle( pDC, m_nDragSelectStart, point.x, &m_nDragSelectEnd);
	ReleaseDC( pDC);
	// 3. 선택한 구간의 시작과 끝의 순서를 보정한다.
	if( m_nDragSelectEnd < m_nDragSelectStart)
	{
		int nDragSelectEnd	= m_nDragSelectStart;
		m_nDragSelectStart	= m_nDragSelectEnd;
		m_nDragSelectEnd	= nDragSelectEnd;
	}
	// 4. 선택한 구간을 기본으로 Data View의 Start Index와 Width를 산출한다.
	//		(왜 보정을 요청하지 않지? 그것은 이미 보정되어 있기 때문이다.)
	int nDataViewStart = GetDataIndex( m_nDragSelectStart, FALSE);
	int nDataViewWidth = GetDataIndex( m_nDragSelectEnd, FALSE) - nDataViewStart + 1;
	// 5. Dragging이 끝났음을 먼저 설정한다.
	m_nDragSelectStart = 0;
	m_nDragSelectEnd = 0;
	// 6. 산출한 Data View의 정보를 내부 ToolBar에게 전달하여 조정토록 한다.
	// 6.1 먼저 한 화면 최소 Data수를 확인하여 보정한다.
	int nMinDataCountOnOnePage = ((CToolBarDlg*)GetParent())->GetCountMinPerPage();	// 종찬(04/11/04) OCX에서 가져오는 루틴 수정.
	if( nDataViewWidth < nMinDataCountOnOnePage)
	{
		nDataViewStart -= ( nMinDataCountOnOnePage - nDataViewWidth) / 2;
		nDataViewWidth = nMinDataCountOnOnePage;
	}
	// 6.2 End Index를 확인하여 보정한다.
	// (2004.09.02, 배승원) 마지막 Data 다음의 Index를 구하는 것이 아니므로, 1을 뺀다.
	int nEndNextIndex = nDataViewStart + nDataViewWidth - 1;
	if( m_nRadarDataRight < nEndNextIndex)
	{
		// (2004.09.02, 배승원) 보정 오류를 개선한다. (이전에 '-='로 구성하였었다.)
		nDataViewStart	+= m_nRadarDataRight - nEndNextIndex;
		nEndNextIndex	+= m_nRadarDataRight - nEndNextIndex;
	}
	// 6.3 Start Index를 확인하여 보정한다.
	if( nDataViewStart < 1)
	{
		nEndNextIndex	+= 1 - nDataViewStart;		// 순서를 왜 바꿨는지에 주의...
		nDataViewStart	+= 1 - nDataViewStart;
	}
	// 6.4 Zero Based로 변환한다.
	nDataViewStart--;
	nEndNextIndex--;

	// 6.5 ScrollBar에게 전달하여 UI를 Update토록 한다.
	( ( CToolBarDlg *)GetParent())->SetStartEndIndexToOcx( nDataViewStart, nEndNextIndex);

	// (2006/11/18 - Seung-Won, Bae) Notify the User Zoom State to MultiChart
	m_pIChartCtrl->OnUserZoomChanged( nEndNextIndex - nDataViewStart + 1);

	CWnd::OnLButtonUp(nFlags, point);
}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2003.12.04
//
//	목적	:	Mouse의 Point에 해당하는 Data Index를, Mouse Point를 보정하며 산출하는 Routine을 별도 제공한다.
//
//	내용	:	1. 조정을 요청하지 않은 경우의 오류는 -1이 Return된다.
//				2. 0이 Return되는 경우는 없다.
//
//	주의	:	1. Index 조정에 있어서 최좌측의 경우 m_rgnRadarGraph.left + 1로 조정한다.
//					그것은 Drawing시에 Pen의 굵기가 2인 것과 연관되어 조정된 것이다.
//				2. 따라서, 이렇게 조정한 PointX를 가지고 다시 DataIndex를 산출하면 오동작할 수 있으니,
//					가능하면 1회에 처리한다.
///////////////////////////////////////////////////////////////////////////////
int CRadar::GetDataIndex( long &p_nPointX, BOOL p_bWithCorrection) const
{
	// 1. Point가 Graph 구간에 들어왔는지로 유효성을 판단한다.
	//		(Graph 영역밖이면서도 오차에 의해 유효한 Data Index가 나올 수 있기 때문이다.)
	//		(구간 설정을 감안하여 Data가 유효하지 않은 경우, point값을 한계값으로 조정하며,
	//		요청하지 않은 경우에는 -1로 설정한다.)
	int nDataIndex = 0;
	if( p_nPointX < m_rgnRadarGraph.left)
	{
		nDataIndex = ( p_bWithCorrection ? m_nRadarDataLeft : -1);
		if( p_bWithCorrection) p_nPointX = m_rgnRadarGraph.left;
	}
	else if( m_rgnRadarGraph.right < p_nPointX)
	{
		nDataIndex = ( p_bWithCorrection ? m_nRadarDataRight : -1);
		if( p_bWithCorrection) p_nPointX = m_rgnRadarGraph.right;
	}
	// 2. Data Index를 산출하여 Return한다.
	//  (주의, 어느 선까지의 연산을 INT화 시키느냐가 값을 좌우한다.)
	if( nDataIndex) return nDataIndex;
	return int( ( p_nPointX - m_rgnRadarGraph.left) / m_dXScale) + m_nRadarDataLeft;
}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2003.12.04
//
//	목적	:	시작과 끝 영역에 해당하는 선택영영에 Rectangle을 출력하는 Routine을 제공한다.
///////////////////////////////////////////////////////////////////////////////
void CRadar::DrawRectangleInRegion( CDC *p_pDC, long p_nDragSelectStart, long p_nDragSelectEnd) const
{
	// 1. Rectangle을 구한다.
	//		(주어지는 것은 Region이므로 right와 bottom에 +1 처리한다.)
	// 1.1 진짜 right는 누구일까?
	//		그렇다고 역전된 것을 바로잡지는 않는다.
	//		Rectangle이 어느 순간부터 역전되는데 그것을 XOR로 Clear하지 못하는 경우가 발생하기 때문이다.
	//		따라서 항상 역정될 가능성에 대해 고려하여 Coding한다.
	if( p_nDragSelectStart <= p_nDragSelectEnd) p_nDragSelectEnd++;
	else p_nDragSelectStart++;
	// 1.2 Rectangle을 구한다.
	CRect rctDragSelect( p_nDragSelectStart, m_rgnRadarGraph.top, p_nDragSelectEnd, m_rgnRadarGraph.bottom + 1);
	// 2. 겉의 Rectangle을 처리한다.
	p_pDC->Rectangle( rctDragSelect);
	// 3.6.1.5 Width가 2이하인 경우, DeflateRect()는 되려 영역을 확장시키거나, 바로 앞의 것을 지우게 된다.
	//		왜 2인가? Region이 아니라 Rectangle이라는 것을 감안한 것이다.
	//		abs()는 왜? Reverse되는 경우의 처리이다.
	if( abs( rctDragSelect.Width()) > 2)
	{
		rctDragSelect.DeflateRect( 1, 1);
		p_pDC->Rectangle( rctDragSelect);
	}
}

// (2004.04.16, 배승원) Radar의 Resize시에도, Resize된 Graph를 보이도록,
//		Radar Graph를 재구성하는 Routine을 별도로 제공한다.
void CRadar::DrawNewRadarView( void)
{
	// 1. Radar Chart의 Client 영역을 구하여 놓는다.
	GetClientRect( &m_rctRadarClient);
	// 3. Graph가 그려지는 영역을 확보한다. 이때는 Region으로 Right/Bottom 좌표를 포함시키도록 한다.
	m_rgnRadarGraph = m_rctRadarClient;
	m_rgnRadarGraph.DeflateRect( 2, 2);
	m_rgnRadarGraph.right--;
	m_rgnRadarGraph.bottom--;
	// 4. Graph의 Scale 변환 수치를 미리 구하여 놓는다.
	m_dXScale = 0;
	// (2003.12.03, 배승원) X Scale의 마지막 Index도 다른 것과 동등한 폭을 갖도록 한다.
	//		if( nDataSize > 1) m_dXScale = ( m_rgnRadarGraph.right - m_rgnRadarGraph.left) / ( nDataSize - 1.);
	//		기존의 위 Code는 전체 폭을 Data 개수보다 1적개 분할 하고 있으며,
	//		nDataSize 번째 Index 위치 (nDataSize-1분할의 끝경계)에서나 m_rgnRadarGraph.right값이 완성되도록 되어 있다.
	//		따라서, nDataSize으로 분할하고, nDataSize+1 번째 Index 위치 (nDataSize분할의 끝경계)에는 nDataSize번째 Index값으로 한번 더 연장처리한다.
	//		이때, Graph Region의 영역에 딱맞게 m_rgnRadarGraph.right+1까지의 폭을 nDataSize분할 하여야 한다.
	// (2004.04.16, 배승원) 이전의 nDataSize를 이용하여도 처리가 가능하게 m_nRadarDataRight로 변경한다.
	if( m_nRadarDataRight > 0)
		m_dXScale = ( m_rgnRadarGraph.right + 1 - m_rgnRadarGraph.left) / ( double)m_nRadarDataRight;
	m_dYScale = 0;
	if( m_dRadarDataMax > m_dRadarDataMin)
		m_dYScale = ( m_rgnRadarGraph.bottom - m_rgnRadarGraph.top)
					/ ( m_dRadarDataMax - m_dRadarDataMin);
	// 5. Radar Chart의 선택된 영역을 Rect로 구하여 놓는다.
	// 5.1 Radar Chart의 선택 영역의 높이를 확정한다. (Radar Chart의 Border는 2 Pixel 이다.)
	m_rctRadarSelect = m_rctRadarClient;
	m_rctRadarSelect.DeflateRect( 0, 2);
	// 5.2 Radar Chart 선택 영역의 위치와 폭을 확정한다.
	m_rctRadarSelect.left = 0;
	m_rctRadarSelect.right = -2;
	// (2003.12.03, 배승원) X Scale의 마지막 Index도 다른 것과 동등한 폭을 갖도록 한다.
	// 5.2.1 먼저 Data가 1개 이상인 경우를 처리한다.
	if( m_nRadarDataRight > 0)
	{
		m_rctRadarSelect.left	= int( ( m_nDataStartIndex - m_nRadarDataLeft)								* m_dXScale + m_rgnRadarGraph.left + 0.5);
		// (2004.09.02, 배승원) 한 Data가 1 Pixel 이상을 차지할 경우, Right(우경계)의 위치는 m_nOnePageDataCount + 1 이후에 있어야
		//		마지막 Data의 영역을 포함하게 된다.
		m_rctRadarSelect.right	= int( ( m_nDataStartIndex - m_nRadarDataLeft + m_nOnePageDataCount + 1)	* m_dXScale + m_rgnRadarGraph.left + 0.5);
	}
	// 5.2.2 Data가 없는 경우 전체 선택으로 설정한다.
	else
	{
		m_rctRadarSelect.left = m_rgnRadarGraph.left;
		m_rctRadarSelect.right = m_rgnRadarGraph.right + 1;		// Graph와 Region의 차이때문에 + 1
	}

	// 5.3 설정된 Data View Width에 따라 Radar Chart의 선택영역 오른쪽을 보정한다.
	//		(Radar Chart에는 우측여백이나 최소 X Scale이 없기 때문이다. Rectangle과 Region의 차이를 이해해야 한다.)
	if( m_rgnRadarGraph.right < m_rctRadarSelect.right) m_rctRadarSelect.right = m_rgnRadarGraph.right + 1;

	// 6. 설정된 내용을 바로 반영하도록 Invalidate시킨다.
//	Invalidate( FALSE);
	RedrawWindow();
}

// (2004.04.16, 배승원) ToolBar Resize시 Radar의 크기 조정과 Graph를 재구성을 처리토록 한다.
void CRadar::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	DrawNewRadarView();
}


/*-----------------------------------------------------------------------------------------
 - Function    :  OnSetFocus
 - Created at  :  2005-09-10   15:49
 - Author      :  최종찬
 - Description :  Focus가 들어오면 OCX에 다시 포커스를 토스!!!
 -----------------------------------------------------------------------------------------*/
void CRadar::OnSetFocus(CWnd* pOldWnd) 
{
//	CWnd::OnSetFocus(pOldWnd);
	
	CWnd *pParent = GetParent();			// Getting ToolBar DLG
	if( pParent)
	{
		pParent = pParent->GetParent();
		if( pParent) pParent->SetFocus();	// Getting Chart OCX
	}
	
}
