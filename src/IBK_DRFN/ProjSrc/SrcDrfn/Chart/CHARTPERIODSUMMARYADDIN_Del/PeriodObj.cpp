// PeriodObj.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ChartPeriodSummaryAddIn.h"
#include "PeriodObj.h"

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

}

CPeriodObj::~CPeriodObj()
{
}

// CPeriodObj 멤버 함수


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

void CPeriodObj::Draw(CDC* pDC)
{
	MakeData();
//	ReCalcRect();
	GetRect();
	// 임시 값
	CPen pen (m_nLineStyle, m_nLineThickness, m_clrLineColor);
	CPen* pOldPen = pDC->SelectObject(&pen);

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
	pDC->Ellipse(rcStart);

	rcEnd.left = m_ptREnd.x - RADIUS_OF_END_RC - OFFSET_RC_IN/2;
	rcEnd.top = m_ptREnd.y - RADIUS_OF_END_RC - OFFSET_RC_IN/2;
	rcEnd.right = m_ptREnd.x + 2*RADIUS_OF_END_RC - OFFSET_RC_IN/2;
	rcEnd.bottom = m_ptREnd.y + 2*RADIUS_OF_END_RC - OFFSET_RC_IN/2;
	rcEnd.OffsetRect(m_ptAOffset + m_ptRLineOffset);
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

void CPeriodObj::DrawTable(CDC* pDC)
{
	// 테이블 표시 속성

	CBrush brush;
	CPen pen (PS_SOLID, (m_bSelected)?2:1, RGB(157, 157, 217));
	brush.CreateSolidBrush(RGB(255,255,255));

	CBrush* pOldBrush = pDC->SelectObject(&brush);
	CPen* pOldPen = pDC->SelectObject(&pen);

	CRect rcTableDraw = m_ptAOffset + m_rcTable;
 	CRect rcTextDraw = m_ptAOffset + m_rcText;

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
	defFont.CreatePointFont(90,_T("굴림"), NULL);

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
		strCellValue.Format("%ld",(long)m_Price.sPrice);
		strCellValue.Format(":  %s", CommaStr(strCellValue));
		rcCell.right = rcCell.left + max(strCellValue.GetLength()*TEXT_WIDTH, rcCell.Width());
	}
 	if(m_bEEPrice)
	{
		strCellValue.Format("%ld",(long)m_Price.ePrice);
		strCellValue.Format(":  %s", CommaStr(strCellValue));
		rcCell.right = rcCell.left + max(strCellValue.GetLength()*TEXT_WIDTH, rcCell.Width());
	}
 	if(m_bHPrice)
	{
		strCellValue.Format("%d",(long)m_Price.max);
		strCellValue.Format(":  %s", CommaStr(strCellValue));
		rcCell.right = rcCell.left + max(strCellValue.GetLength()*TEXT_WIDTH, rcCell.Width());
	}
 	if(m_bLPrice)
	{
		strCellValue.Format("%d",(long)m_Price.min);
		strCellValue.Format(":  %s", CommaStr(strCellValue));
		rcCell.right = rcCell.left + max(strCellValue.GetLength()*TEXT_WIDTH, rcCell.Width());
	}
 	if(m_bMPrice)
	{
		strCellValue.Format("%.2lf",m_Price.avg);
		strCellValue.Format(":  %s", CommaStr(strCellValue));
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

	// 박스를 그린다.
	pDC->RoundRect(rcTableDraw,CPoint(0,0));
//	TRACE2("\n\n       (%d, %d)\n\n", rcTableDraw.top, ptTableOffset.y);

	m_rc.right += 20;

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

	pDC->SetTextColor(RGB(0,0,0));
	rcTextDraw.OffsetRect(TEXT_WIDTH*m_strBasicInfo2.GetLength(),0);
	// 기본 정보3 (가격 정보)를 표시한다.
	pDC->DrawText(m_strBasicInfo3,rcTextDraw,DT_LEFT| DT_VCENTER);

	// 다시 처음 위치로 변경(CRLF)
	rcTextDraw = m_ptAOffset + m_rcText;
	rcTextDraw.OffsetRect(ptTextOffset);
	rcTextDraw.OffsetRect(0, TEXT_HEIGHT);
	// 기간 표시
	pDC->DrawText(m_strPeriod,rcTextDraw,DT_LEFT| DT_VCENTER);
	// 옵션에 따라 상세 정보 표시
	if(m_bSEPrice)
	{
		rcTextDraw.OffsetRect(0, TEXT_HEIGHT);
		rcCell = rcTextDraw;
		rcCell.left += rcTextDraw.Width()/2;

		pDC->DrawText("시작일종가",rcTextDraw,DT_LEFT| DT_VCENTER);
		strCellValue.Format("%ld",(long)m_Price.sPrice);
		strCellValue.Format(":  %s", CommaStr(strCellValue));
		rcCell.right = rcCell.left + max(strCellValue.GetLength()*TEXT_WIDTH, rcTextDraw.Width()/2);
		pDC->DrawText(strCellValue,rcCell,DT_LEFT| DT_VCENTER);
	}
 	if(m_bEEPrice)
	{
		rcTextDraw.OffsetRect(0, TEXT_HEIGHT);
		rcCell = rcTextDraw;
		rcCell.left += rcTextDraw.Width()/2;

		pDC->DrawText("종료일종가",rcTextDraw,DT_LEFT| DT_VCENTER);
		strCellValue.Format("%ld",(long)m_Price.ePrice);
		strCellValue.Format(":  %s", CommaStr(strCellValue));
		rcCell.right = rcCell.left + max(strCellValue.GetLength()*TEXT_WIDTH, rcTextDraw.Width()/2);
		pDC->DrawText(strCellValue,rcCell,DT_LEFT| DT_VCENTER);
	}
 	if(m_bHPrice)
	{
		rcTextDraw.OffsetRect(0, TEXT_HEIGHT);
		rcCell = rcTextDraw;
		rcCell.left += rcTextDraw.Width()/2;

		pDC->DrawText("고가",rcTextDraw,DT_LEFT| DT_VCENTER);
		strCellValue.Format("%d",(long)m_Price.max);
		strCellValue.Format(":  %s", CommaStr(strCellValue));
		rcCell.right = rcCell.left + max(strCellValue.GetLength()*TEXT_WIDTH, rcTextDraw.Width()/2);
		pDC->DrawText(strCellValue,rcCell,DT_LEFT| DT_VCENTER);
	}
 	if(m_bLPrice)
	{
		rcTextDraw.OffsetRect(0, TEXT_HEIGHT);
		rcCell = rcTextDraw;
		rcCell.left += rcTextDraw.Width()/2;

		pDC->DrawText("저가",rcTextDraw,DT_LEFT| DT_VCENTER);
		strCellValue.Format("%d",(long)m_Price.min);
		strCellValue.Format(":  %s", CommaStr(strCellValue));
		rcCell.right = rcCell.left + max(strCellValue.GetLength()*TEXT_WIDTH, rcTextDraw.Width()/2);
		pDC->DrawText(strCellValue,rcCell,DT_LEFT| DT_VCENTER);
	}
 	if(m_bMPrice)
	{
		rcTextDraw.OffsetRect(0, TEXT_HEIGHT);
		rcCell = rcTextDraw;
		rcCell.left += rcTextDraw.Width()/2;

		pDC->DrawText("평균가",rcTextDraw,DT_LEFT| DT_VCENTER);
		strCellValue.Format("%.2lf",m_Price.avg);
		strCellValue.Format(":  %s", CommaStr(strCellValue));
		rcCell.right = rcCell.left + max(strCellValue.GetLength()*TEXT_WIDTH, rcTextDraw.Width()/2);
		pDC->DrawText(strCellValue,rcCell,DT_LEFT| DT_VCENTER);
	}
 	if(m_bHAmount)
	{
		rcTextDraw.OffsetRect(0, TEXT_HEIGHT);
		rcCell = rcTextDraw;
		rcCell.left += rcTextDraw.Width()/2;

		pDC->DrawText("최대거래량",rcTextDraw,DT_LEFT| DT_VCENTER);
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

		pDC->DrawText("최소거래량",rcTextDraw,DT_LEFT| DT_VCENTER);
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

		pDC->DrawText("평균거래량",rcTextDraw,DT_LEFT| DT_VCENTER);
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

		pDC->DrawText("양봉수",rcTextDraw,DT_LEFT| DT_VCENTER);
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

		pDC->DrawText("음봉수",rcTextDraw,DT_LEFT| DT_VCENTER);
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

		pDC->DrawText("양음봉비율",rcTextDraw,DT_LEFT| DT_VCENTER);
		strCellValue.Format("%.2lf%%", m_Candle.ratio);
		strCellValue.Format(":  %s", CommaStr(strCellValue));
		rcCell.right = rcCell.left + max(strCellValue.GetLength()*TEXT_WIDTH, rcTextDraw.Width()/2);
		pDC->DrawText(strCellValue,rcCell,DT_LEFT| DT_VCENTER);
	}
	// 펜, 브러쉬 헤제
	pDC->SelectObject(pOldPen);	
	pDC->SelectObject(poldFont);

}
///////////////////////////////////////////////////////////////////////////
// 기본 데이터 생성
void CPeriodObj::MakeData(void)
{
	// 1 raw(기본 정보)
	char szUpdown[3] = {0};

	if(m_Price.ePrice < m_Price.sPrice)	// 하락
	{
		m_nUpDownSymbol = -1;
		strcpy(szUpdown, "▼");
	}
	else if(m_Price.ePrice > m_Price.sPrice)	// 상승
	{
		m_nUpDownSymbol = 1;
		strcpy(szUpdown, "▲");
	}
	else
	{
		m_nUpDownSymbol = 0;
		strcpy(szUpdown, "-");
	}

	m_strBasicInfo1.Format("%d 캔들"
						, m_Candle.count);
	m_strBasicInfo2.Format("%s"
						, szUpdown);
	CString strDiffPrice;
	strDiffPrice.Format("%ld", abs((int)(m_Price.ePrice - m_Price.sPrice)));
	m_strBasicInfo3.Format("%s (%3.2f%%)"
						, CommaStr(strDiffPrice)
						, 100* (m_Price.ePrice - m_Price.sPrice) / m_Price.sPrice );

	// 2 raw (기간)
	long lDate;
	int a, b, c, d, e, f;	// 시간 데이터를 분리


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
		d = lDate/100;
		m_strSDate.Format("%d/%02d/%02d:%02d", a,b,c,d);

		// 종료일
		lDate = m_lEDate;
		a = lDate/1000000;
		lDate = lDate%1000000;
		b = lDate/10000;
		lDate = lDate%10000;
		c = lDate/100;
		lDate = lDate%100;
		d = lDate/100;
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
