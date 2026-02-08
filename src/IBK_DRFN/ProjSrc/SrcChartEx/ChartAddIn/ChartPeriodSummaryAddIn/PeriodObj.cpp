// PeriodObj.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ChartPeriodSummaryAddIn.h"
#include "PeriodObj.h"

#include <math.h>									// for fabs()

#include "./Include_Chart/DLL_Load/IMetaTable.h"	// for _MHWND()
#include "./Include_Chart/Conversion.h"				// for CDataConversion

#include "EnglishString.h"							// for _ENG_DOWN
#include "ChartPeriodSummaryAddInImp.h"				// for CChartPeriodSummaryAddInImp

// CPeriodObj

CPeriodObj::CPeriodObj()
: m_nUpDown(-1)				// UP/DOWN
, m_nLineThickness(0)		// 선 굵기
, m_nLineStyle(0)			// 선 스타일
, m_strSDate(_T(""))		// 시작일
, m_strEDate(_T(""))		// 종료 일
, m_nRowCnt(1)				// 행 수
, m_strPeriod(_T(""))		// 기간
, m_bSEPrice(FALSE)			// 시작일 종가
, m_bEEPrice(FALSE)			// 종료일 종가
, m_bHPrice(FALSE)			// 고가
, m_bLPrice(FALSE)			// 저가
, m_bMPrice(FALSE)			// 
, m_bHAmount(FALSE)			// 최대 거래량
, m_bLAmount(FALSE)			// 최소 거래량
, m_bMAmount(FALSE)			// 평균 거래량
, m_bPCNum(FALSE)			// 양봉 수
, m_bMCNum(FALSE)			// 음봉 수
, m_bPMRatio(FALSE)			// 양음봉 비율
, m_nTextAlignment(0)		// 텍스트 정렬
, m_bSaved(FALSE)			// 저장되었던 것 여부
, m_nTimeDiff(0)			// 사용자선택 시간대	// 20081007 JS.Kim	사용자시간대 선택
,m_strRQ( "DEFAULT")			// (2008/11/5 - Seung-Won, Bae) for Multiple Item.
{
	m_ptAStart = CPoint(0,0);
	m_ptAEnd = CPoint(0,0);
	m_size = CSize(0,0);
	m_bSelected = FALSE;
	m_CurClickedPosition = N_PT;
	m_rc = CRect(0,0,0,0);
	m_rcLine = CRect(0,0,0,0);
	m_rcText = CRect(0,0,0,0);
	m_ptALineOffset = CPoint(0,0);
	m_ptLTOffset = CPoint(OFFSET_LT, -OFFSET_LT);
	m_ptRLineOffset = CPoint(0,0);
	m_ptRTextOffset = CPoint(0,0);
	m_nLineThickness = 1;
	m_clrLineColor = RGB(255,0,0);

	// 20081009 박동관 >>
	m_pChartPeriodSummaryAddInImp = NULL;
	// 20081009 박동관 <<
}

CPeriodObj::~CPeriodObj()
{
}

// CPeriodObj 멤버 함수

// 20081009 박동관 >>
void CPeriodObj::SetChartPeriodSummaryAddInImp(CChartPeriodSummaryAddInImp* pChartPeriodSummaryAddInImp)
{
	if(!m_pChartPeriodSummaryAddInImp)
		m_pChartPeriodSummaryAddInImp = pChartPeriodSummaryAddInImp;
}

CChartPeriodSummaryAddInImp* CPeriodObj::GetChartPeriodSummaryAddInImp()
{
	return m_pChartPeriodSummaryAddInImp;
}

CString CPeriodObj::GetPacketType()
{
	if( !m_pChartPeriodSummaryAddInImp) return "";
	CString strResult = "";

	IPacketListManager *pIPacketListManager = m_pChartPeriodSummaryAddInImp->GetPacketListManager();
	if( pIPacketListManager)
	{
		IPacketManager *pIPacketManager = pIPacketListManager->GetPacketManager( m_strRQ);
		if( pIPacketManager)
		{
			IPacket *pIPacket = pIPacketManager->GetPacket( _MTEXT(C0_CLOSE));
			if( pIPacket)
			{
				ILPCSTR szType = pIPacket->GetType();
				strResult = szType;
				pIPacket->Release();
			}
			pIPacketManager->Release();
		}
		pIPacketListManager->Release();
	}
	return strResult;
}
// 20081009 박동관 <<

// 2011.01.24 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
// 현재 주기의 날짜시간 Type을 얻는다.
CString CPeriodObj::GetPacketDateType()
{
	if( !m_pChartPeriodSummaryAddInImp) return "";
	CString strResult = "";
	
	IPacketListManager *pIPacketListManager = m_pChartPeriodSummaryAddInImp->GetPacketListManager();
	if( pIPacketListManager)
	{
		IPacketManager *pIPacketManager = pIPacketListManager->GetPacketManager( m_strRQ);
		if( pIPacketManager)
		{
			IPacket *pIPacket = pIPacketManager->GetPacket( _MTEXT(C0_DATE_TIME));
			if( pIPacket)
			{
				ILPCSTR szType = pIPacket->GetType();
				strResult = szType;
				pIPacket->Release();
			}
			pIPacketManager->Release();
		}
		pIPacketListManager->Release();
	}
	return strResult;
}

// 주기정보를 얻는다.
int CPeriodObj::GetPacketDateUnit()
{
	if( !m_pChartPeriodSummaryAddInImp) 
		return 0;
	
	int nResult = 0;
	
	IPacketListManager *pIPacketListManager = m_pChartPeriodSummaryAddInImp->GetPacketListManager();
	if( pIPacketListManager)
	{
		IPacketManager *pIPacketManager = pIPacketListManager->GetPacketManager(m_strRQ);
		if( pIPacketManager)
		{
			IPacket *pIPacket = pIPacketManager->GetPacket( _MTEXT(C0_DATE_TIME));
			if( pIPacket)
			{
				nResult = pIPacket->GetDateUnitType();
				pIPacket->Release();
			}
			pIPacketManager->Release();
		}
		pIPacketListManager->Release();
	}
	return nResult;
}
// 2011.01.24 by SYS <<


int CPeriodObj::IsInRegion(CPoint pt)
{
	if(IsOnEndPoint(pt) || IsOnLine(pt))
		return TRUE;
	return FALSE;
}


int CPeriodObj::IsOnLine(CPoint pt)
{
	pt -= m_ptALineOffset;
	CRect rc = m_rcLine;
	rc.InflateRect(LINE_ON_SENSITIVITY,LINE_ON_SENSITIVITY,LINE_ON_SENSITIVITY,LINE_ON_SENSITIVITY);
	if(rc.PtInRect(pt) && !m_rcCut.PtInRect(pt) && !m_rcCutS.PtInRect(pt) && !m_rcCutE.PtInRect(pt))
	{
		m_CurClickedPosition = M_PT;
		return m_CurClickedPosition;
	}
	return N_PT;
}

BOOL CPeriodObj::IsOnEndPoint(CPoint pt)
{
	if(pt.x >= m_ptAStart.x-RADIUS_OF_END_RC && pt.x <= m_ptAStart.x+RADIUS_OF_END_RC && pt.y >= m_ptAStart.y-RADIUS_OF_END_RC && pt.y <= m_ptAStart.y+RADIUS_OF_END_RC)
	{
		m_CurClickedPosition = S_PT;
		m_ptAt = m_ptAStart - pt;
		return TRUE;
	}
	if(pt.x >= m_ptAEnd.x-RADIUS_OF_END_RC && pt.x <= m_ptAEnd.x+RADIUS_OF_END_RC && pt.y >= m_ptAEnd.y-RADIUS_OF_END_RC && pt.y <= m_ptAEnd.y+RADIUS_OF_END_RC)
	{
		m_CurClickedPosition = E_PT;
		m_ptAt = m_ptAEnd - pt;
		return TRUE;
	}

	m_CurClickedPosition = N_PT;
	return FALSE;
}

void CPeriodObj::Draw(CDC* pDC, HWND p_hOcxWnd)
{
	MakeData( p_hOcxWnd);
//	ReCalcRect();
	GetRect();
	// 임시 값
	CPen pen (m_nLineStyle, m_nLineThickness, m_clrLineColor);
	CPen* pOldPen = pDC->SelectObject(&pen);

	//>>20100112_JS.Kim 그래프영역을 벗어나는 경우 보정
	/*
	if(m_nUpDown == UP)	// Up 방향
	{
		pDC->MoveTo(m_ptAOffset + m_ptRLineOffset + CPoint(-OFFSET_RC_IN/2, -OFFSET_RC_IN/2) + CPoint(m_rcLine.left,m_rcLine.top)				);	// top-left로 이동
		pDC->LineTo(m_ptAOffset + m_ptRLineOffset + CPoint(-OFFSET_RC_IN/2, -OFFSET_RC_IN/2) + CPoint(m_rcLine.right,m_rcLine.top)				);	// top-right까지 가로 선
		pDC->MoveTo(m_ptAOffset + m_ptRLineOffset + CPoint(-OFFSET_RC_IN/2, -OFFSET_RC_IN/2)													);	// top-left로 이동
		pDC->LineTo(m_ptAOffset + m_ptRLineOffset + CPoint(-OFFSET_RC_IN/2, -OFFSET_RC_IN/2) + ((m_ptRStart.x<m_ptREnd.x)?m_ptRStart:m_ptREnd)	);	// 시작점까지 세로 선
		pDC->MoveTo(m_ptAOffset + m_ptRLineOffset + CPoint(-OFFSET_RC_IN/2, -OFFSET_RC_IN/2) + CPoint(m_rcLine.right,m_rcLine.top)				);	// top-right로 이동
		pDC->LineTo(m_ptAOffset + m_ptRLineOffset + CPoint(-OFFSET_RC_IN/2, -OFFSET_RC_IN/2) + ((m_ptRStart.x<m_ptREnd.x)?m_ptREnd:m_ptRStart)	);	// 끝점까지 세로 선
	}
	else					// Down 방향
	{
		pDC->MoveTo(m_ptAOffset + m_ptRLineOffset + CPoint(-OFFSET_RC_IN/2, -OFFSET_RC_IN/2) + CPoint(m_rcLine.left,m_rcLine.bottom)			);	// bottom-left로 이동
		pDC->LineTo(m_ptAOffset + m_ptRLineOffset + CPoint(-OFFSET_RC_IN/2, -OFFSET_RC_IN/2) + CPoint(m_rcLine.right,m_rcLine.bottom)			);	// bottom-right까지 가로 선
		pDC->MoveTo(m_ptAOffset + m_ptRLineOffset + CPoint(-OFFSET_RC_IN/2, -OFFSET_RC_IN/2) + CPoint(m_rcLine.left,m_rcLine.bottom)			);	// bottom-left로 이동
		pDC->LineTo(m_ptAOffset + m_ptRLineOffset + CPoint(-OFFSET_RC_IN/2, -OFFSET_RC_IN/2) + ((m_ptRStart.x<m_ptREnd.x)?m_ptRStart:m_ptREnd)	);	// 시작점까지 세로 선
		pDC->MoveTo(m_ptAOffset + m_ptRLineOffset + CPoint(-OFFSET_RC_IN/2, -OFFSET_RC_IN/2) + CPoint(m_rcLine.right,m_rcLine.bottom)			);	// bottom-left로 이동
		pDC->LineTo(m_ptAOffset + m_ptRLineOffset + CPoint(-OFFSET_RC_IN/2, -OFFSET_RC_IN/2) + ((m_ptRStart.x<m_ptREnd.x)?m_ptREnd:m_ptRStart)	);	// 끝점까지 세로 선
	}
	*/
	if(m_nUpDown == UP)	// Up 방향
	{
		CPoint pt = m_ptAOffset + m_ptRLineOffset + CPoint(-OFFSET_RC_IN/2, -OFFSET_RC_IN/2);
		if( pt.y < m_rctGraphRegion.top + 2)		pt.y = m_rctGraphRegion.top + 2;

		pDC->MoveTo(pt + CPoint(m_rcLine.left,m_rcLine.top)					);	// top-left로 이동
		m_pBaseLine = pt + CPoint(m_rcLine.left,m_rcLine.top);	//20110202_alzioyes: UP/DOWN에서 텍스트위치 조정.
		pDC->LineTo(pt + CPoint(m_rcLine.right,m_rcLine.top)				);	// top-right까지 가로 선
		pDC->MoveTo(pt														);	// top-left로 이동

		CPoint ptEnd = m_ptAOffset + m_ptRLineOffset + CPoint(-OFFSET_RC_IN/2, -OFFSET_RC_IN/2) + ((m_ptRStart.x<m_ptREnd.x)?m_ptRStart:m_ptREnd);
		if( ptEnd.y < m_rctGraphRegion.top + RADIUS_OF_END_RC + OFFSET_RC_IN )		
			ptEnd.y = m_rctGraphRegion.top + RADIUS_OF_END_RC + OFFSET_RC_IN;
		pDC->LineTo(ptEnd);	// 시작점까지 세로 선

		pDC->MoveTo(pt + CPoint(m_rcLine.right,m_rcLine.top)				);	// top-right로 이동

		ptEnd = m_ptAOffset + m_ptRLineOffset + CPoint(-OFFSET_RC_IN/2, -OFFSET_RC_IN/2) + ((m_ptRStart.x<m_ptREnd.x)?m_ptREnd:m_ptRStart);
		if( ptEnd.y < m_rctGraphRegion.top + RADIUS_OF_END_RC+OFFSET_RC_IN)		
			ptEnd.y = m_rctGraphRegion.top + RADIUS_OF_END_RC + OFFSET_RC_IN;
		pDC->LineTo(ptEnd);	// 끝점까지 세로 선
	}
	else					// Down 방향
	{
		CPoint pt = m_ptAOffset + m_ptRLineOffset + CPoint(-OFFSET_RC_IN/2, -OFFSET_RC_IN/2) + CPoint(m_rcLine.left,m_rcLine.bottom);
		if( pt.y > m_rctGraphRegion.bottom - 2)		pt.y = m_rctGraphRegion.bottom - 2;
		pDC->MoveTo(pt														);	// bottom-left로 이동
		m_pBaseLine = pt + CPoint(m_rcLine.left,m_rcLine.top);	//20110202_alzioyes: UP/DOWN에서 텍스트위치 조정.
		pDC->LineTo(pt + CPoint(m_rcLine.right-m_rcLine.left, 0)			);	// bottom-right까지 가로 선
		pDC->MoveTo(pt														);	// bottom-left로 이동

		CPoint ptEnd = m_ptAOffset + m_ptRLineOffset + CPoint(-OFFSET_RC_IN/2, -OFFSET_RC_IN/2) + ((m_ptRStart.x<m_ptREnd.x)?m_ptRStart:m_ptREnd);
		if( ptEnd.y > m_rctGraphRegion.bottom - RADIUS_OF_END_RC - OFFSET_RC_IN )		
			ptEnd.y = m_rctGraphRegion.bottom - RADIUS_OF_END_RC - OFFSET_RC_IN;
		pDC->LineTo(ptEnd);	// 시작점까지 세로 선

		pDC->MoveTo(pt + CPoint(m_rcLine.right-m_rcLine.left,0)				);	// bottom-right로 이동

		ptEnd = m_ptAOffset + m_ptRLineOffset + CPoint(-OFFSET_RC_IN/2, -OFFSET_RC_IN/2) + ((m_ptRStart.x<m_ptREnd.x)?m_ptREnd:m_ptRStart);
		if( ptEnd.y > m_rctGraphRegion.bottom - RADIUS_OF_END_RC-OFFSET_RC_IN)		
			ptEnd.y = m_rctGraphRegion.bottom - RADIUS_OF_END_RC - OFFSET_RC_IN;
		pDC->LineTo(ptEnd);	// 끝점까지 세로 선
	}
	//<<
	
	DrawEndPoints(pDC);
	DrawTable(pDC);

	pDC->SelectObject(pOldPen);
}

//////////////////////////////////////////////////////////////////////////
// 그리기 영역
// 마우스 클릭 시작/끝점(절대 좌표)으로 부터 상대 정보 얻는다
//
CRect CPeriodObj::GetRect()
{
//////////////////////////////////////////////////////////////////////////
// 그리기 영역 1
//////////////////////////////////////////////////////////////////////////
	// 라인
	m_rcLine.right = abs(m_ptAStart.x - m_ptAEnd.x);
	m_rcLine.bottom = abs(m_ptAStart.y - m_ptAEnd.y) + DEFAULT_HEIGHT;

	
//////////////////////////////////////////////////////////////////////////
// 절대 위치
//////////////////////////////////////////////////////////////////////////
	// 라인
	m_ptALineOffset.x = min(m_ptAStart.x, m_ptAEnd.x);

	switch(m_nUpDown)
	{
	case UP:
		m_ptALineOffset.y = min(m_ptAStart.y, m_ptAEnd.y) - DEFAULT_HEIGHT;
		break;
	case DOWN:
		m_ptALineOffset.y = min(m_ptAStart.y, m_ptAEnd.y);
		break;
	}
	// 테이블
	m_ptATableOffset =  m_ptALineOffset + m_ptLTOffset;
	// 텍스트
	m_ptATextOffset =  m_ptATableOffset + CPoint(OFFSET_TT_IN, OFFSET_TT_IN);
	// 전체
	m_ptAOffset.x = m_ptALineOffset.x - OFFSET_RC_IN;
	m_ptAOffset.y = min(m_ptALineOffset.y, m_ptATableOffset.y) - OFFSET_RC_IN;


//	TRACE3("\n\n***In object - table y : %d,line y : %d, start y : %d",m_ptATableOffset.y,m_ptALineOffset.y,m_ptAStart.y);
//	TRACE1(", end y : %d\n\n",m_ptAEnd.y);

//////////////////////////////////////////////////////////////////////////
// 상대 위치
//////////////////////////////////////////////////////////////////////////
	// 라인
	m_ptRLineOffset = CPoint(OFFSET_RC_IN, OFFSET_RC_IN - m_ptLTOffset.y);	// m_rc 기준
	m_ptRStart = m_ptAStart - m_ptAOffset - m_ptRLineOffset;				// m_rcLine 기준
	m_ptREnd = m_ptAEnd - m_ptAOffset - m_ptRLineOffset;					// m_rcLine 기준

	switch(m_nUpDown)
	{
	case UP:
		// 테이블
		m_ptRTableOffset = CPoint(m_ptLTOffset.x + OFFSET_RC_IN, OFFSET_RC_IN);	// m_rc 기준
		// 텍스트
		m_ptRTextOffset =  CPoint(m_ptLTOffset.x + OFFSET_RC_IN+OFFSET_TT_IN, OFFSET_RC_IN+OFFSET_TT_IN);	// m_rc 기준
		break;
	case DOWN:
		// 테이블
		m_ptRTableOffset = CPoint(m_ptLTOffset.x + OFFSET_RC_IN, m_rcLine.bottom);	// m_rc 기준
		// 텍스트
		m_ptRTextOffset =  CPoint(m_ptLTOffset.x + OFFSET_RC_IN+OFFSET_TT_IN, m_rcLine.bottom + (OFFSET_TT_IN));	// m_rc 기준
		break;
	}

//	TRACE3("\n\nUP/DOWN:%d, 절대:%d, 상대:%d\n\n", m_nUpDown, m_ptATableOffset, m_ptRTableOffset.y);

//////////////////////////////////////////////////////////////////////////
// 그리기 영역 2
//////////////////////////////////////////////////////////////////////////
	// 라인
//	m_rcLine.right = abs(m_ptAStart.x - m_ptAEnd.x);
//	m_rcLine.bottom = abs(m_ptAStart.y - m_ptAEnd.y) + DEFAULT_HEIGHT;
	m_rcCut = m_rcLine;
	switch(m_nUpDown)
	{
	case UP:
		m_rcCut.InflateRect(-LINE_ON_SENSITIVITY,-LINE_ON_SENSITIVITY,-LINE_ON_SENSITIVITY,LINE_ON_SENSITIVITY);	// 선을 따라 4 pixel 영역
		m_rcCutS = CRect(m_rcLine.left-LINE_ON_SENSITIVITY, m_ptRStart.y,m_rcCut.left, m_rcLine.bottom);
		m_rcCutE = CRect(m_rcCut.right, m_ptREnd.y,m_rcLine.right+LINE_ON_SENSITIVITY, m_rcLine.bottom);
		break;
	case DOWN:
		m_rcCut.InflateRect(-LINE_ON_SENSITIVITY,LINE_ON_SENSITIVITY,-LINE_ON_SENSITIVITY,-LINE_ON_SENSITIVITY);	// 선을 따라 4 pixel 영역
		m_rcCutS = CRect(m_rcLine.left-LINE_ON_SENSITIVITY, m_rcLine.top,m_rcCut.left, m_ptRStart.y);
		m_rcCutE = CRect(m_rcCut.right, m_rcLine.top,m_rcLine.right+LINE_ON_SENSITIVITY, m_ptREnd.y);
		break;
	}
	
	// 텍스트
	m_rcText.right = TEXT_WIDTH * max(m_strPeriod.GetLength(), (m_strBasicInfo1.GetLength()+m_strBasicInfo2.GetLength()+m_strBasicInfo3.GetLength()));
	m_rcText.bottom = m_nRowCnt*TEXT_HEIGHT;
	// 테이블
	m_rcTable = m_rcText;
	m_rcTable.InflateRect(0,0,0,20);

	// 전체
	m_rc.right = max(m_ptRLineOffset.x+m_rcLine.right, m_ptRTableOffset.x+m_rcTable.right) + OFFSET_RC_IN;
	m_rc.bottom = max(m_ptRLineOffset.y+m_rcLine.bottom, m_ptRTableOffset.y+m_rcTable.bottom) + OFFSET_RC_IN;

	m_size.cx = m_rc.Width();	// 전체 영역 폭
	m_size.cy = m_rc.Height();	// 전체 영역 높이

	return m_rc;
}


void CPeriodObj::DrawEndPoints(CDC *pDC)
{
	CBrush brush;
	brush.CreateSolidBrush(m_clrLineColor);
	CBrush* pOldBrush = pDC->SelectObject(&brush);
	CRect rcStart, rcEnd;
	rcStart.left = m_ptRStart.x - RADIUS_OF_END_RC - OFFSET_RC_IN/2;
	rcStart.top = m_ptRStart.y - RADIUS_OF_END_RC - OFFSET_RC_IN/2;
	rcStart.right = m_ptRStart.x + 2*RADIUS_OF_END_RC - OFFSET_RC_IN/2;
	rcStart.bottom = m_ptRStart.y + 2*RADIUS_OF_END_RC - OFFSET_RC_IN/2;
	rcStart.OffsetRect(m_ptAOffset + m_ptRLineOffset);
	//>>20100112_JS.Kim 그래프영역을 벗어나는 경우 보정
	if( rcStart.top < m_rctGraphRegion.top )
	{
		rcStart.top = m_rctGraphRegion.top;
		rcStart.bottom = m_rctGraphRegion.top + 3*RADIUS_OF_END_RC;
	}
	else if( rcStart.bottom > m_rctGraphRegion.bottom )
	{
		rcStart.bottom = m_rctGraphRegion.bottom;
		rcStart.top = m_rctGraphRegion.bottom - 3*RADIUS_OF_END_RC;
	}
	//<<
	pDC->Ellipse(rcStart);

	rcEnd.left = m_ptREnd.x - RADIUS_OF_END_RC - OFFSET_RC_IN/2;
	rcEnd.top = m_ptREnd.y - RADIUS_OF_END_RC - OFFSET_RC_IN/2;
	rcEnd.right = m_ptREnd.x + 2*RADIUS_OF_END_RC - OFFSET_RC_IN/2;
	rcEnd.bottom = m_ptREnd.y + 2*RADIUS_OF_END_RC - OFFSET_RC_IN/2;
	rcEnd.OffsetRect(m_ptAOffset + m_ptRLineOffset);
	//>>20100112_JS.Kim 그래프영역을 벗어나는 경우 보정
	if( rcEnd.top < m_rctGraphRegion.top )
	{
		rcEnd.top = m_rctGraphRegion.top;
		rcEnd.bottom = m_rctGraphRegion.top + 3*RADIUS_OF_END_RC;
	}
	else if( rcEnd.bottom > m_rctGraphRegion.bottom )
	{
		rcEnd.bottom = m_rctGraphRegion.bottom;
		rcEnd.top = m_rctGraphRegion.bottom - 3*RADIUS_OF_END_RC;
	}
	//<<
	pDC->Ellipse(rcEnd);
}

//////////////////////////////////////////////////////////////////////////
//	마우스 클릭 위치를 타입에 따라 필요한 포인터에 매핑한다.
// 주로 오브젝트 생성(새로 그리기)시 사용된다.
void CPeriodObj::SetPos(CPoint pt, int type)
{
	switch(type)
	{
	case S_PT:
		m_ptAStart = pt;
		break;
	case E_PT:
		m_ptAEnd = pt;
		break;
	case M_PT:		
		m_ptAStart += pt - m_ptAt;
		m_ptAEnd += pt - m_ptAt;
		m_ptAt = pt;
		break;
	}
}

//////////////////////////////////////////////////////////////////////////
// 타입에 따라 해당하는 위치 정보를 반환한다.
CPoint CPeriodObj::GetPos(int type)
{
	switch(type)
	{
	case S_PT:
		return m_ptAStart;
		break;
	case E_PT:
		return m_ptAEnd;
		break;
	default:
		return m_ptAt;
	}
}

//////////////////////////////////////////////////////////////////////////
//	마우스 클릭 위치를 현재 선택된 타입에 따라 필요한 포인터에 매핑한다.
//  주로 오브젝트 선택시 사용된다.
void CPeriodObj::ButtonDownAt(const CPoint &pt)
{	
	switch(m_CurClickedPosition)
	{
	case S_PT:
		m_ptAStart = pt+m_ptAt;
		break;
	case E_PT:
		m_ptAEnd = pt+m_ptAt;
		break;
	case M_PT:
		m_ptAt = pt;
		m_ptAStart += pt - m_ptAt;
		m_ptAEnd += pt - m_ptAt;
		break;
	default:
		m_ptAStart = m_ptAEnd = pt;		 
	}
}

//////////////////////////////////////////////////////////////////////////
//	마우스 클릭 업 위치를 현재 선택된 타입에 따라 필요한 포인터에 매핑한다.
//  
void CPeriodObj::ButtonUpAt(const CPoint &pt)
{	
	switch(m_CurClickedPosition)
	{
	case S_PT:
		m_ptAStart = pt;
		break;
	case E_PT:
		m_ptAEnd = pt;
		break;
	case M_PT:
		m_ptAStart += pt - m_ptAt;
		m_ptAEnd += pt - m_ptAt;
		m_ptAt = pt;
		break;
	case N_PT:
		m_ptAEnd = pt;
		break;
	}
}


// void CPeriodObj::ReCalcRect()
//{
//	m_oldrc = m_rc;
//	GetRect();
//}

//void CPeriodObj::Erase(CDC* pDC)
//{
//}

/**
	@history
		2011.01.05 alzioyes:솔로몬요청사항 반영.
			1. 네모박스 제거하고 뒷배경이 보이도록 투명하게 처리
			2. 등락폭, 등락률 색상 적용
		2011.01.12 alzioyes:[CSA00000070]글자표현하는 위치 변경요청 적용.
			글자표현 위치를 UP에서는 선위로, DOWN일때는 선아래로 변경.
*/
void CPeriodObj::DrawTable(CDC* pDC)
{
	// 테이블 표시 속성
	//CBrush brush;
	//CPen pen (PS_SOLID, (m_bSelected)?2:1, RGB(157, 157, 217));
	//brush.CreateSolidBrush(RGB(255,255,255));

	//CBrush* pOldBrush = pDC->SelectObject(&brush);
	//CPen* pOldPen = pDC->SelectObject(&pen);

	CString szValFormat("%.f");
	int nDecimalDownCount = 0;
	CString strType;
	GetPacketTypeInfo( nDecimalDownCount, strType);
	szValFormat.Format("%c.%df", '%', nDecimalDownCount);

	//[CSA00000070-1]글자표현 위치를 UP에서는 선위로, DOWN일때는 선아래로 변경.
	CPoint tmpOffset(m_ptAOffset);
	int nTmpOffset;
	if(m_nUpDown == UP)
	{
		nTmpOffset=14;
		tmpOffset.y -= nTmpOffset;
	}
	else
	{
		nTmpOffset=20;
		tmpOffset.y += nTmpOffset;
	}
	
	CRect rcTableDraw = m_ptAOffset + m_rcTable;
	//CRect rcTextDraw = m_ptAOffset + m_rcText;
	CRect rcTextDraw = tmpOffset + m_rcText;

 	CPoint ptTableOffset = m_ptRTableOffset;
 	CPoint ptTextOffset = m_ptRTextOffset;

	switch(m_nTextAlignment)
	{
	case 0:	//left
		break;
	case 1:	//right
		if(m_rcLine.right+m_ptRLineOffset.x > m_rcTable.right + m_ptRTableOffset.x)	// 선이 더 오른쪽에 있을때
		{
			ptTableOffset.x +=  m_rcLine.right - (m_ptLTOffset.x*2 + rcTableDraw.Width());
			ptTextOffset.x +=  m_rcLine.right - (m_ptLTOffset.x*2 + rcTableDraw.Width());
		}

		break;

	}

	rcTextDraw.OffsetRect(ptTextOffset);
	rcTableDraw.OffsetRect(ptTableOffset);

	// text 표시 속성
	CFont defFont, boldFont;

	CString strFont;
	strFont.LoadString(IDS_STRING_FONT);
	// defFont.CreatePointFont(90,_T("굴림"), NULL);
	defFont.CreatePointFont(90,strFont, NULL);
	
	LOGFONT BoldLog;
	defFont.GetLogFont(&BoldLog);
	//	BoldLog.lfWeight = FW_BOLD;
	boldFont.CreateFontIndirect(&BoldLog);
	
	CFont *poldFont = pDC->SelectObject(&defFont);
	
	pDC->SetBkColor(RGB(255,255,255));
	pDC->SetTextColor(RGB(0,0,0)); //RGB(128,128,128)
	pDC->SetBkMode(TRANSPARENT);

	// 그리기 영역 계산(폭)

	CString strCellValue;
	CRect rcCell = rcTextDraw;
	rcCell.left += rcTextDraw.Width()/2;

	//////////////////////////////////////////////////////////////////////////
	// 박스 크기를 구한다
 	if(m_bSEPrice)
	{
		strCellValue = CDataConversion::GetStringThousandFormat(m_Price.sPrice, strType);
		//strCellValue.Format("%ld",(long)m_Price.sPrice);
		//strCellValue.Format(":  %s", CommaStr(strCellValue));
		rcCell.right = rcCell.left + max(strCellValue.GetLength()*TEXT_WIDTH, rcCell.Width());
	}
 	if(m_bEEPrice)
	{
		strCellValue = CDataConversion::GetStringThousandFormat(m_Price.ePrice, strType);
		//strCellValue.Format("%ld",(long)m_Price.ePrice);
		//strCellValue.Format(":  %s", CommaStr(strCellValue));
		rcCell.right = rcCell.left + max(strCellValue.GetLength()*TEXT_WIDTH, rcCell.Width());
	}
 	if(m_bHPrice)
	{
		strCellValue = CDataConversion::GetStringThousandFormat(m_Price.max, strType);
		//strCellValue.Format("%d",(long)m_Price.max);
		//strCellValue.Format(":  %s", CommaStr(strCellValue));
		rcCell.right = rcCell.left + max(strCellValue.GetLength()*TEXT_WIDTH, rcCell.Width());
	}
 	if(m_bLPrice)
	{
		strCellValue = CDataConversion::GetStringThousandFormat(m_Price.min, strType);
		//strCellValue.Format("%d",(long)m_Price.min);
		//strCellValue.Format(":  %s", CommaStr(strCellValue));
		rcCell.right = rcCell.left + max(strCellValue.GetLength()*TEXT_WIDTH, rcCell.Width());
	}
 	if(m_bMPrice)
	{
		strCellValue = CDataConversion::GetStringThousandFormat(m_Price.avg, strType);
		//strCellValue.Format("%.2lf",m_Price.avg);
		//strCellValue.Format(":  %s", CommaStr(strCellValue));
		rcCell.right = rcCell.left + max(strCellValue.GetLength()*TEXT_WIDTH, rcCell.Width());
	}
 	if(m_bHAmount)
	{
		strCellValue.Format("%ld",(long)m_Amount.max);
		strCellValue.Format(":  %s", CommaStr(strCellValue));
		rcCell.right = rcCell.left + max(strCellValue.GetLength()*TEXT_WIDTH, rcCell.Width());
	}
 	if(m_bLAmount)
	{
		strCellValue.Format("%ld",(long)m_Amount.min);
		strCellValue.Format(":  %s", CommaStr(strCellValue));
		rcCell.right = rcCell.left + max(strCellValue.GetLength()*TEXT_WIDTH, rcCell.Width());
	}
 	if(m_bMAmount)
	{
		strCellValue.Format("%.2lf",m_Amount.avg);
		strCellValue.Format(":  %s", CommaStr(strCellValue));
		rcCell.right = rcCell.left + max(strCellValue.GetLength()*TEXT_WIDTH, rcCell.Width());
	}
 	if(m_bPCNum)
	{
		strCellValue.Format("%d",m_Candle.plus);
		strCellValue.Format(":  %s", CommaStr(strCellValue));
		rcCell.right = rcCell.left + max(strCellValue.GetLength()*TEXT_WIDTH, rcCell.Width());
	}
 	if(m_bMCNum)
	{
		strCellValue.Format("%d",m_Candle.minus);
		strCellValue.Format(":  %s", CommaStr(strCellValue));
		rcCell.right = rcCell.left + max(strCellValue.GetLength()*TEXT_WIDTH, rcCell.Width());
	}
 	if(m_bPMRatio)
	{
		strCellValue.Format("%.2lf%%", m_Candle.ratio);
		strCellValue.Format(":  %s", CommaStr(strCellValue));
		rcCell.right = rcCell.left + max(strCellValue.GetLength()*TEXT_WIDTH, rcCell.Width());
	}

	//[CSA00000070-2]글자표현 위치를 UP에서는 선위로, DOWN일때는 선아래로 변경.
	//>>20100112_JS.Kim 그래프영역을 벗어나는 경우 보정
	if( rcTableDraw.right > m_rctGraphRegion.right && rcTableDraw.left < m_rctGraphRegion.right )
	{
		CPoint ptMove(m_rctGraphRegion.right-rcTableDraw.right, 0);
		rcTableDraw.OffsetRect(ptMove);
		rcTextDraw.OffsetRect(ptMove);
	}
// 	nTmpOffset = 0;
// 	if( rcTableDraw.bottom+nTmpOffset > m_rctGraphRegion.bottom )
// 	{
// 		CPoint ptMove(0, m_rctGraphRegion.bottom-rcTableDraw.bottom-nTmpOffset);
// 		rcTableDraw.OffsetRect(ptMove);
// 		rcTextDraw.OffsetRect(ptMove);
// 	}
// 	if( rcTableDraw.top < m_rctGraphRegion.top+nTmpOffset )
// 	{
// 		CPoint ptMove(0, m_rctGraphRegion.top - rcTableDraw.top + nTmpOffset);
// 		rcTableDraw.OffsetRect(ptMove);
// 		rcTextDraw.OffsetRect(ptMove);
// 	}

	//>>20110202_alzioyes: UP/DOWN에서 텍스트위치 조정.
	//		UP일때   텍스트위치가 위로 넘어갈 때 선 밑으로 조정.
	//		DOWN일때 텍스트위치가 아래로 내려갈 때 선 위로 조정.
	int nH1, nH2;
	nH1 = rcTableDraw.Height();
	nH2 = rcTextDraw.Height();

	if(m_nUpDown == UP)
	{
		if( rcTableDraw.top < m_rctGraphRegion.top+10)
		{
			rcTableDraw.top = m_pBaseLine.y+2; 
			rcTextDraw.top = m_pBaseLine.y+2; 
			rcTableDraw.bottom = rcTableDraw.top + nH1;
			rcTextDraw.bottom = rcTextDraw.top + nH2;
		}
	}
	else
	{
		if( rcTableDraw.top+50> m_rctGraphRegion.bottom )
		{
			rcTableDraw.top = m_pBaseLine.y-27; 
			rcTextDraw.top = m_pBaseLine.y-27; 
			rcTableDraw.bottom = rcTableDraw.top + nH1;
			rcTextDraw.bottom = rcTextDraw.top + nH2;
		}
	}
	//<<20110202_alzioyes: UP/DOWN에서 텍스트위치 조정.

	CRect rcOrgTextDraw = rcTextDraw;
	//<<

	// 박스를 그린다.
		
	//@@@@@pDC->RoundRect(rcTableDraw,CPoint(0,0));
//	TRACE2("\n\n       (%d, %d)\n\n", rcTableDraw.top, ptTableOffset.y);

	m_rc.right += 100;

	// 기본 정보1 (캔들 수)를 표시한다.
	pDC->DrawText(m_strBasicInfo1,rcTextDraw,DT_LEFT| DT_VCENTER);

	// 업/다운 표식의 색을 결정한다.
	switch(m_nUpDownSymbol)
	{
	case -1:
		pDC->SetTextColor(RGB(0,0,255));
		break;
	case 1:
		pDC->SetTextColor(RGB(255,0,0));
		break;
	}
	rcTextDraw.OffsetRect(TEXT_WIDTH*m_strBasicInfo1.GetLength(),0);
	// 기본 정보2 (업/다운 표식)를 표시한다.
	pDC->DrawText(m_strBasicInfo2,rcTextDraw,DT_LEFT| DT_VCENTER);

	//SMalzio110105-001 등락폭, 등락률에도 같은 색상 적용.
	//pDC->SetTextColor(RGB(0,0,0));
	rcTextDraw.OffsetRect(TEXT_WIDTH*m_strBasicInfo2.GetLength(),0);
	// 기본 정보3 (가격 정보)를 표시한다.
	pDC->DrawText(m_strBasicInfo3,rcTextDraw,DT_LEFT| DT_VCENTER);

	// 다시 처음 위치로 변경(CRLF)
	//>>20100112_JS.Kim 그래프영역을 벗어나는 경우 보정
	//rcTextDraw = m_ptAOffset + m_rcText;
	//rcTextDraw.OffsetRect(ptTextOffset);
	rcTextDraw = rcOrgTextDraw;
	//<<
	pDC->SetTextColor(RGB(0,0,0));
	rcTextDraw.OffsetRect(0, TEXT_HEIGHT);
	// 기간 표시
	pDC->DrawText(m_strPeriod,rcTextDraw,DT_LEFT| DT_VCENTER);

	/////////////////////////////////
	// 공통으로 들어가는 것을 묶는다.
	

	/////////////////////////////////
	
	// 옵션에 따라 상세 정보 표시

	// 20081009 박동관 패킷타입을 얻어온다. >>
	CString strValue;
	//CString strType = GetPacketType();
	strType = GetPacketType();
	// 20081009 박동관 <<

	if(m_bSEPrice)
	{
		rcTextDraw.OffsetRect(0, TEXT_HEIGHT);
		rcCell = rcTextDraw;
		rcCell.left += rcTextDraw.Width()/2;

		CString strStartColseNumber;
		strStartColseNumber.LoadString(IDS_STARTING_CLOSE_NUMBERS);
		// pDC->DrawText("시작일종가",rcTextDraw,DT_LEFT| DT_VCENTER);
		pDC->DrawText(strStartColseNumber,rcTextDraw,DT_LEFT| DT_VCENTER);

		// 20081009 박동관 8진법/32진법 처리 >>
		if(CDataConversion::IsBaseFormat(strType) && strType != "")
		{
			// 진법변환일 경우...
			strValue = CDataConversion::GetStringThousandFormat(m_Price.sPrice, strType);
		}
		else
		{
			//strValue.Format("%ld",(long)m_Price.sPrice);
			strValue.Format(szValFormat, m_Price.sPrice);
			strValue = CommaStr(strValue);
		}
		strCellValue.Format(":  %s", strValue);
		// 20081009 박동관 <<

		rcCell.right = rcCell.left + max(strCellValue.GetLength()*TEXT_WIDTH, rcTextDraw.Width()/2);
		pDC->DrawText(strCellValue,rcCell,DT_LEFT| DT_VCENTER);
	}
 	if(m_bEEPrice)
	{
		rcTextDraw.OffsetRect(0, TEXT_HEIGHT);
		rcCell = rcTextDraw;
		rcCell.left += rcTextDraw.Width()/2;

		CString strEndingClosePrive;
		strEndingClosePrive.LoadString(IDS_ENDING_CLOSE_PRIVE);
		// pDC->DrawText("종료일종가",rcTextDraw,DT_LEFT| DT_VCENTER);
		pDC->DrawText(strEndingClosePrive,rcTextDraw,DT_LEFT| DT_VCENTER);

		// 20081009 박동관 8진법/32진법 처리 >>
		if(CDataConversion::IsBaseFormat(strType) && strType != "")
		{
			// 진법변환일 경우...
			strValue = CDataConversion::GetStringThousandFormat(m_Price.ePrice, strType);
		}
		else
		{
			//strValue.Format("%ld",(long)m_Price.ePrice);
			strValue.Format(szValFormat, m_Price.ePrice);
			strValue = CommaStr(strValue);
		}
		strCellValue.Format(":  %s", strValue);
		// 20081009 박동관 <<

		rcCell.right = rcCell.left + max(strCellValue.GetLength()*TEXT_WIDTH, rcTextDraw.Width()/2);
		pDC->DrawText(strCellValue,rcCell,DT_LEFT| DT_VCENTER);
	}
 	if(m_bHPrice)
	{
		rcTextDraw.OffsetRect(0, TEXT_HEIGHT);
		rcCell = rcTextDraw;
		rcCell.left += rcTextDraw.Width()/2;
		
		CString strHighPrice;
		strHighPrice.LoadString( IDS_HIGH_PRICE);
		//pDC->DrawText("고가",rcTextDraw,DT_LEFT| DT_VCENTER);
		pDC->DrawText(strHighPrice,rcTextDraw,DT_LEFT| DT_VCENTER);	

		// 20081009 박동관 8진법/32진법 처리 >>
		if(CDataConversion::IsBaseFormat(strType) && strType != "")
		{
			// 진법변환일 경우...
			strValue = CDataConversion::GetStringThousandFormat(m_Price.max, strType);
		}
		else
		{
			//strValue.Format("%ld",(long)m_Price.max);
			strValue.Format(szValFormat, m_Price.max);
			strValue = CommaStr(strValue);
		}
		strCellValue.Format(":  %s", strValue);
		// 20081009 박동관 <<

		rcCell.right = rcCell.left + max(strCellValue.GetLength()*TEXT_WIDTH, rcTextDraw.Width()/2);
		pDC->DrawText(strCellValue,rcCell,DT_LEFT| DT_VCENTER);
	}
 	if(m_bLPrice)
	{
		rcTextDraw.OffsetRect(0, TEXT_HEIGHT);
		rcCell = rcTextDraw;
		rcCell.left += rcTextDraw.Width()/2;

		CString strLowPrice;
		strLowPrice.LoadString(IDS_LOW_PRICE);		
		// pDC->DrawText("저가",rcTextDraw,DT_LEFT| DT_VCENTER);
		pDC->DrawText(strLowPrice,rcTextDraw,DT_LEFT| DT_VCENTER);

		// 20081009 박동관 8진법/32진법 처리 >>
		if(CDataConversion::IsBaseFormat(strType) && strType != "")
		{
			// 진법변환일 경우...
			strValue = CDataConversion::GetStringThousandFormat(m_Price.min, strType);
		}
		else
		{
			//strValue.Format("%ld",(long)m_Price.min);
			strValue.Format(szValFormat, m_Price.min);
			strValue = CommaStr(strValue);
		}
		strCellValue.Format(":  %s", strValue);
		// 20081009 박동관 <<

		rcCell.right = rcCell.left + max(strCellValue.GetLength()*TEXT_WIDTH, rcTextDraw.Width()/2);
		pDC->DrawText(strCellValue,rcCell,DT_LEFT| DT_VCENTER);
	}
 	if(m_bMPrice)
	{
		rcTextDraw.OffsetRect(0, TEXT_HEIGHT);
		rcCell = rcTextDraw;
		rcCell.left += rcTextDraw.Width()/2;

		CString strAverageVolume;
		strAverageVolume.LoadString( IDS_AVERAGE_PRICE);
		// pDC->DrawText("평균가",rcTextDraw,DT_LEFT| DT_VCENTER);
		pDC->DrawText(strAverageVolume,rcTextDraw,DT_LEFT| DT_VCENTER);

		// 20081009 박동관 8진법/32진법 처리 >>
		if(CDataConversion::IsBaseFormat(strType) && strType != "")
		{
			// 진법변환일 경우...
			strValue = CDataConversion::GetStringThousandFormat(m_Price.avg, strType);
		}
		else
		{
			//strValue.Format("%ld",(long)m_Price.avg);
			strValue.Format("%.2lf",m_Price.avg);
			strValue = CommaStr(strValue);
		}
		strCellValue.Format(":  %s", strValue);
		// 20081009 박동관 <<

		rcCell.right = rcCell.left + max(strCellValue.GetLength()*TEXT_WIDTH, rcTextDraw.Width()/2);
		pDC->DrawText(strCellValue,rcCell,DT_LEFT| DT_VCENTER);
	}
 	if(m_bHAmount)
	{
		rcTextDraw.OffsetRect(0, TEXT_HEIGHT);
		rcCell = rcTextDraw;
		rcCell.left += rcTextDraw.Width()/2;

		CString strMaxVolume;
		strMaxVolume.LoadString(IDS_MAX_VOLUME);
		// pDC->DrawText("최대거래량",rcTextDraw,DT_LEFT| DT_VCENTER);
		pDC->DrawText(strMaxVolume,rcTextDraw,DT_LEFT| DT_VCENTER);

		strCellValue.Format("%ld",(long)m_Amount.max);
		strCellValue.Format(":  %s", CommaStr(strCellValue));
		rcCell.right = rcCell.left + max(strCellValue.GetLength()*TEXT_WIDTH, rcTextDraw.Width()/2);
		pDC->DrawText(strCellValue,rcCell,DT_LEFT| DT_VCENTER);
	}
 	if(m_bLAmount)
	{
		rcTextDraw.OffsetRect(0, TEXT_HEIGHT);
		rcCell = rcTextDraw;
		rcCell.left += rcTextDraw.Width()/2;

		CString strMinVolume;
		strMinVolume.LoadString(IDS_MIN_VOLUME);
		// pDC->DrawText("최소거래량",rcTextDraw,DT_LEFT| DT_VCENTER);
		pDC->DrawText(strMinVolume,rcTextDraw,DT_LEFT| DT_VCENTER);

		strCellValue.Format("%ld",(long)m_Amount.min);
		strCellValue.Format(":  %s", CommaStr(strCellValue));
		rcCell.right = rcCell.left + max(strCellValue.GetLength()*TEXT_WIDTH, rcTextDraw.Width()/2);
		pDC->DrawText(strCellValue,rcCell,DT_LEFT| DT_VCENTER);
	}
 	if(m_bMAmount)
	{
		rcTextDraw.OffsetRect(0, TEXT_HEIGHT);
		rcCell = rcTextDraw;
		rcCell.left += rcTextDraw.Width()/2;

		CString strAverageVolume;
		strAverageVolume.LoadString(IDS_AVERAGE_VOLUME);
		// pDC->DrawText("평균거래량",rcTextDraw,DT_LEFT| DT_VCENTER);
		pDC->DrawText(strAverageVolume,rcTextDraw,DT_LEFT| DT_VCENTER);

		strCellValue.Format("%.2lf",m_Amount.avg);
		strCellValue.Format(":  %s", CommaStr(strCellValue));
		rcCell.right = rcCell.left + max(strCellValue.GetLength()*TEXT_WIDTH, rcTextDraw.Width()/2);
		pDC->DrawText(strCellValue,rcCell,DT_LEFT| DT_VCENTER);
	}
 	if(m_bPCNum)
	{
		rcTextDraw.OffsetRect(0, TEXT_HEIGHT);
		rcCell = rcTextDraw;
		rcCell.left += rcTextDraw.Width()/2;

		CString strPositiveBongNumber;
		strPositiveBongNumber.LoadString(IDS_POSITIVE_BONG_NUMBERS);
		// pDC->DrawText("양봉수",rcTextDraw,DT_LEFT| DT_VCENTER);
		pDC->DrawText(strPositiveBongNumber,rcTextDraw,DT_LEFT| DT_VCENTER);

		strCellValue.Format("%d",m_Candle.plus);
		strCellValue.Format(":  %s", CommaStr(strCellValue));
		rcCell.right = rcCell.left + max(strCellValue.GetLength()*TEXT_WIDTH, rcTextDraw.Width()/2);
		pDC->DrawText(strCellValue,rcCell,DT_LEFT| DT_VCENTER);
	}
 	if(m_bMCNum)
	{
		rcTextDraw.OffsetRect(0, TEXT_HEIGHT);
		rcCell = rcTextDraw;
		rcCell.left += rcTextDraw.Width()/2;

		CString strNegativeBongNumbers;
		strNegativeBongNumbers.LoadString(IDS_NEGATIVE_BONG_NUMBERS);
		// pDC->DrawText("음봉수",rcTextDraw,DT_LEFT| DT_VCENTER);
		pDC->DrawText(strNegativeBongNumbers,rcTextDraw,DT_LEFT| DT_VCENTER);

		strCellValue.Format("%d",m_Candle.minus);
		strCellValue.Format(":  %s", CommaStr(strCellValue));
		rcCell.right = rcCell.left + max(strCellValue.GetLength()*TEXT_WIDTH, rcTextDraw.Width()/2);
		pDC->DrawText(strCellValue,rcCell,DT_LEFT| DT_VCENTER);
	}
 	if(m_bPMRatio)
	{
		rcTextDraw.OffsetRect(0, TEXT_HEIGHT);
		rcCell = rcTextDraw;
		rcCell.left += rcTextDraw.Width()/2;

		CString strPositiveNegativePercentage;
		strPositiveNegativePercentage.LoadString(IDS_POSITIVE_NEGATIVE_PERCENTAGE);
		// pDC->DrawText("양음봉비율",rcTextDraw,DT_LEFT| DT_VCENTER);
		pDC->DrawText(strPositiveNegativePercentage,rcTextDraw,DT_LEFT| DT_VCENTER);

		strCellValue.Format("%.2lf%%", m_Candle.ratio);
		strCellValue.Format(":  %s", CommaStr(strCellValue));
		rcCell.right = rcCell.left + max(strCellValue.GetLength()*TEXT_HEIGHT, rcTextDraw.Width()/2);
		pDC->DrawText(strCellValue,rcCell,DT_LEFT| DT_VCENTER);
	}
	
	
	// 펜, 브러쉬 헤제
	//pDC->SelectObject(pOldBrush);
	//pDC->SelectObject(pOldPen);
	pDC->SelectObject(poldFont);

}
///////////////////////////////////////////////////////////////////////////
// 기본 데이터 생성
void CPeriodObj::MakeData( HWND p_hOcxWnd)
{
	CString szValFormat("%.f");
	int nDecimalDownCount = 0;
	CString strType;
	GetPacketTypeInfo( nDecimalDownCount, strType);
	szValFormat.Format("%c.%df", '%', nDecimalDownCount);

	// (2008/3/4 - Seung-Won, Bae) for Multi-Language
	IMetaTable::E_LANGUAGEID eLanguageID = g_iMetaTable.GetCurrentLanguageID( p_hOcxWnd);
	if(m_Price.ePrice < m_Price.sPrice)	// 하락
	{
		m_nUpDownSymbol = -1;
		switch( eLanguageID)
		{
			case IMetaTable::KOREAN:	m_strBasicInfo2 = "▼";
										break;
			default:					m_strBasicInfo2 = _ENG_DOWN;
										break;
		}
	}
	else if(m_Price.ePrice > m_Price.sPrice)	// 상승
	{
		m_nUpDownSymbol = 1;
		switch( eLanguageID)
		{
			case IMetaTable::KOREAN:	m_strBasicInfo2 = "▲";
										break;
			default:					m_strBasicInfo2 = _ENG_UP;
										break;
		}
	}
	else
	{
		m_nUpDownSymbol = 0;
		m_strBasicInfo2 = "-";
	}

	CString strCandle;
	strCandle.LoadString(IDS_CANDLE);
	// m_strBasicInfo1.Format("%d 캔들", m_Candle.count);
	m_strBasicInfo1.Format("%d %s"
						, m_Candle.count, strCandle);

//	CString strDiffPrice;
//	strDiffPrice.Format("%ld", abs( int( (m_Price.ePrice - m_Price.sPrice))));
//	m_strBasicInfo3.Format("%s (%3.2f%%)"
//						, CommaStr(strDiffPrice)
//						, 100* (m_Price.ePrice - m_Price.sPrice) / m_Price.sPrice );

	// 20081009 박동관 8진법/32진법 처리 >>
	CString strDiffPrice;
	//CString strType = GetPacketType();
	strType = GetPacketType();

	if(CDataConversion::IsBaseFormat(strType) && strType != "")
	{
		double dwDiffPrice = fabs(m_Price.ePrice - m_Price.sPrice);
		strDiffPrice = CDataConversion::GetStringThousandFormat(dwDiffPrice, strType);
	}
	else
	{
		//strDiffPrice.Format("%ld", abs( int( (m_Price.ePrice - m_Price.sPrice))));
		strDiffPrice.Format(szValFormat, fabs(m_Price.ePrice - m_Price.sPrice));
		strDiffPrice = CommaStr(strDiffPrice);
	}

	// 0, 0.00일때 -가 앞에 붙어서 수정함.
	if(strDiffPrice=="0" || strDiffPrice=="0.00") m_strBasicInfo2 = "";

	m_strBasicInfo3.Format("%s (%3.2f%%)", strDiffPrice,
			                               100 * (m_Price.ePrice - m_Price.sPrice) / m_Price.sPrice);
	// 20081009 박동관 <<

	// 2 raw (기간)
	// 2011.01.06 by SYS >> 분차트 YYYY(년) 추가
	//long lDate;
	__int64 lDate;
	// 2011.01.06 by SYS <<

	int a, b, c, d, e, f;	// 시간 데이터를 분리

	// 2011.01.24 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
	CString strDateType = GetPacketDateType();
	int nDateUnit		= GetPacketDateUnit();

	//
	if (strDateType == _T("YYYYMMDDHHMMSS"))
	{
		int nYear1, nMon1, nDay1, nHour1, nMin1, nSec1;	// 시간 데이터를 분리
		int nYear2, nMon2, nDay2, nHour2, nMin2, nSec2;	// 시간 데이터를 분리

		// 시작일
		nYear1= m_lSDate/10000000000;	// 년
		lDate = m_lSDate%10000000000;
		nMon1 = lDate/100000000;		// 월
		lDate = lDate%100000000;
		nDay1 = lDate/1000000;			// 일
		lDate = lDate%1000000;
		nHour1= lDate/10000;			// 시
		lDate = lDate%10000;	
		nMin1 = lDate/100;				// 분
		nSec1 = lDate%100;				// 초
		
		// 종료일
		nYear2= m_lEDate/10000000000;	// 년
		lDate = m_lEDate%10000000000;
		nMon2 = lDate/100000000;		// 월
		lDate = lDate%100000000;	
		nDay2 = lDate/1000000;			// 일
		lDate = lDate%1000000;
		nHour2= lDate/10000;			// 시
		lDate = lDate%10000;
		nMin2 = lDate/100;				// 분
		nSec2 = lDate%100;				// 초

		switch(m_nDateType)
		{
		case 0:	// YYYYMMDD
			m_strSDate.Format("%d/%02d/%02d", nYear1,nMon1,nDay1);
			m_strEDate.Format("%d/%02d/%02d", nYear2,nMon2,nDay2);
			break;
		case 1:	// MMDDHHMM
			m_strSDate.Format("%d/%02d/%02d:%02d", nMon1,nDay1,nHour1,nMin1);
			m_strEDate.Format("%d/%02d/%02d:%02d", nMon2,nDay2,nHour2,nMin2);
			break;
		case 2:	// HHMMSS
			m_strSDate.Format("%02d:%02d:%02d", nHour1,nMin1,nSec1);
			m_strEDate.Format("%02d:%02d:%02d", nHour2,nMin2,nSec2);
			break;
		case 3:	// DDHHMMSS
			m_strSDate.Format("%d/%02d:%02d:%02d", nDay1,nHour1,nMin1,nSec1);
			m_strEDate.Format("%d/%02d:%02d:%02d", nDay2,nHour2,nMin2,nSec2);
			break;
		case 4:	//YYYYMM
			m_strSDate.Format("%d/%02d", nYear1,nMon1);
			m_strEDate.Format("%d/%02d", nYear2,nMon2);
			break;
		case 5:	// YYYYMMDDHHMMSS
			m_strSDate.Format("%d/%02d/%02d:%02d", nMon1,nDay1,nHour1,nMin1);
			m_strEDate.Format("%d/%02d/%02d:%02d", nMon2,nDay2,nHour2,nMin2);
			break;
		}
	}
	else
	// 2011.01.24 by SYS <<
	{
		switch(m_nDateType)
		{
		case 0:	// YYYYMMDD
			// 시작일
			lDate = m_lSDate;
			a = lDate/10000;
			lDate = lDate%10000;
			b = lDate/100;
			lDate = lDate%100;
			c = lDate;

			// 종료일
			lDate = m_lEDate;
			d = lDate/10000;
			lDate = lDate%10000;
			e = lDate/100;
			lDate = lDate%100;
			f = lDate;

			m_strSDate.Format("%d/%02d/%02d", a,b,c);
			m_strEDate.Format("%d/%02d/%02d", d,e,f);
			break;
		case 1:	// MMDDHHMM
			// 시작일
			lDate = m_lSDate;
			a = lDate/1000000;
			lDate = lDate%1000000;
			b = lDate/10000;
			lDate = lDate%10000;
			c = lDate/100;
			lDate = lDate%100;
			d = lDate;
			m_strSDate.Format("%d/%02d/%02d:%02d", a,b,c,d);

			// 종료일
			lDate = m_lEDate;
			a = lDate/1000000;
			lDate = lDate%1000000;
			b = lDate/10000;
			lDate = lDate%10000;
			c = lDate/100;
			lDate = lDate%100;
			d = lDate;
			m_strEDate.Format("%d/%02d/%02d:%02d", a,b,c,d);
			break;
		case 2:	// HHMMSS
			// 시작일
			lDate = m_lSDate;
			a = lDate/10000;
			lDate = lDate%10000;
			b = lDate/100;
			lDate = lDate%100;
			c = lDate;

			// 종료일
			lDate = m_lEDate;
			d = lDate/10000;
			lDate = lDate%10000;
			e = lDate/100;
			lDate = lDate%100;
			f = lDate;

			m_strSDate.Format("%02d:%02d:%02d", a,b,c);
			m_strEDate.Format("%02d:%02d:%02d", d,e,f);
			break;
		case 3:	// DDHHMMSS
			// 시작일
			lDate = m_lSDate;
			a = lDate/1000000;
			lDate = lDate%1000000;
			b = lDate/10000;
			lDate = lDate%10000;
			c = lDate/100;
			lDate = lDate%100;
			d = lDate;
			m_strSDate.Format("%d/%02d:%02d:%02d", a,b,c,d);

			// 종료일
			lDate = m_lEDate;
			a = lDate/1000000;
			lDate = lDate%1000000;
			b = lDate/10000;
			lDate = lDate%10000;
			c = lDate/100;
			lDate = lDate%100;
			d = lDate;
			m_strEDate.Format("%d/%02d:%02d:%02d", a,b,c,d);
			break;
		case 4:	//YYYYMM
			// 시작일
			lDate = m_lSDate;
			a = lDate/100;
			lDate = lDate%100;
			b = lDate;

			// 종료일
			lDate = m_lEDate;
			d = lDate/100;
			lDate = lDate%100;
			e = lDate;
			lDate = lDate;

			m_strSDate.Format("%d/%02d", a,b);
			m_strEDate.Format("%d/%02d", d,e);
			break;

		}
	}
	
	m_strPeriod = m_strSDate + _T("~") + m_strEDate;
	
}

//////////////////////////////////////////////////////////////////////////
// 숫자현 텍스트에 콤마 넣기 123456 --> 123,456
CString CPeriodObj::CommaStr(CString in)
{
	int NumOfComma, nPoint, nUnderPoint = 0, nLength;
	CString out = in;

	nPoint = out.Find(".",0);	// 소수점 위치
	if(nPoint >= 0)	nUnderPoint = out.GetLength() - nPoint;
	nLength = out.GetLength() - nUnderPoint;

	NumOfComma = (nLength - ((in.Left(1) == "-")?1:0))/ 3;	// 삽입해야할 컴마 수 계산.음수 고려
	if((in.GetLength() - nUnderPoint - ((in.Left(1) == "-")?1:0))% 3 == 0)NumOfComma--;	// 소수점 이하 고려, 있을 경우 1 감소
	for(int i=0;i<NumOfComma;i++)
	{
		out.Insert(nLength - (3*i+3), ",");
	}
	return out;
}

bool CPeriodObj::GetPacketTypeInfo(int& nDecimalDownCount, CString& strType) const
{
	if( !m_pChartPeriodSummaryAddInImp) return false;
	CString strPacketType = "";
	
	IPacketListManager *pIPacketListManager = m_pChartPeriodSummaryAddInImp->GetPacketListManager();
	if( pIPacketListManager)
	{
		IPacketManager *pIPacketManager = pIPacketListManager->GetPacketManager( m_strRQ);
		if( pIPacketManager)
		{
			IPacket *pIPacket = pIPacketManager->GetPacket( _MTEXT(C0_CLOSE));
			if( pIPacket)
			{
				ILPCSTR szType = pIPacket->GetType(false);
				strPacketType = szType;
				pIPacket->Release();
			}
			pIPacketManager->Release();
		}
		pIPacketListManager->Release();
	}
	if( strPacketType.IsEmpty() || strPacketType.Find(".") < 0)	return false;
	
	// 20080910 JS.Kim		8진법/32진법 처리, 소숫점 이하 자리수 최대자리로 Return <<
	if( CDataConversion::IsBaseFormat(strPacketType) )
	{
		nDecimalDownCount = 0;
		strType = strPacketType;
		int nBase, nDecLen1, nDecLen2;
		if( CDataConversion::GetBaseFormatInfo(strPacketType, nBase, nDecLen1, nDecLen2) )
			nDecimalDownCount = nDecLen1 + nDecLen2;
		return true;
	}
	//>>
	
	CDataConversion::GetStringData(strPacketType, ".");
	CString strRight = strPacketType.Right(1);
	int nRight = atoi(strRight);
	if(nRight == 0 && strRight != "0")
		strType = strRight;
	
	strPacketType.TrimRight(strType);
	strPacketType.TrimRight("0");
	nDecimalDownCount = strPacketType.GetLength();
	return true;
}

CString CPeriodObj::GetText_Price( double dPrice, const CString &p_strPacketType, int nDecimalDownCount) const
{
	CString strCurPrice;
	
	if( CDataConversion::IsBaseFormat( p_strPacketType) )
	{
		strCurPrice = CDataConversion::DoubleToString( dPrice, p_strPacketType);
	}
	else
	{
		strCurPrice = CDataConversion::DoubleToString( dPrice, nDecimalDownCount);
		
		if( p_strPacketType.Find( '.') < 0)
		{
			CString strPacketType( p_strPacketType);
			int nFactor = strPacketType.Replace( '0', '_');
			strCurPrice = strCurPrice.Left( strCurPrice.GetLength() - nFactor);
		}
	}
	
	return CDataConversion::GetStringThousandFormat( strCurPrice);
	//>>
}