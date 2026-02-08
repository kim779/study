// LineObj.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ChartPeriodSummaryAddIn.h"
//#include "LINE.H"
#include "LineObj.h"
#include <math.h>
// CLineObj

#include "./Include_Chart/DLL_Load/IMetaTable.h"		// for _ML_LANGUAGE_RES
#include "EnglishString.h"								// for _ENG_STAR

CLineObj::CLineObj( HWND p_hOcxWnd)
: m_nUpDown(-1)
, m_nLineThickness(0)
, m_nLineStyle(0)
, m_strSDate(_T(""))
, m_strEDate(_T(""))
, m_nRowCnt(1)
, m_strOrderFlag(_T(""))
, m_nTextAlignment(0)		// 텍스트 정렬
{
	m_ptAStart = CPoint(0,0);
	m_ptAEnd = CPoint(0,0);
	m_size = CSize(0,0);
	m_bSelected = FALSE;
	m_CurClickedPosition = N_PT;
//	m_nRowCnt = 2;
	m_rc = CRect(0,0,0,0);
	m_rcLine = CRect(0,0,0,0);
	m_rcText = CRect(0,0,0,0);
	m_ptLTOffset = CPoint(-(TEXT_WIDTH * 2), -TEXT_HEIGHT/3);
	m_ptRLineOffset = CPoint(0,0);
	m_ptRTextOffset = CPoint(0,0);
	m_nLineThickness = 1;
	m_clrLineColor = RGB(255,0,0);
	m_bUpSell = FALSE;
	m_bUpBuy = FALSE;
	m_bDownSell = FALSE;
	m_bDownBuy = FALSE;
	m_lSDate = 0;
	m_lEDate = 0;
	m_nState = STAT_NONE;
	m_dAlertPrice = 0;
	// 20081007 JS.Kim	사용자시간대 선택
	m_nTimeDiff = 0;

	// (2008/3/4 - Seung-Won, Bae) for Multi-Language
	m_hOcxWnd = p_hOcxWnd;
	IMetaTable::E_LANGUAGEID eLanguageID = g_iMetaTable.GetCurrentLanguageID( m_hOcxWnd);
	switch( eLanguageID)
	{
		case IMetaTable::KOREAN:	m_strStar = "★";
									break;
		default:					m_strStar = _ENG_STAR;
									break;
	}
}

CLineObj::~CLineObj()
{
}


// CLineObj 멤버 함수

void CLineObj::Initilize(void)
{
// 	CClientDC dc(NULL);
// 	m_dcMem.CreateCompatibleDC(&dc);
// 	m_dcMemText.CreateCompatibleDC(&dc);
}

int CLineObj::IsInRegion(CPoint pt)
{
	if(IsOnEndPoint(pt) || IsOnLine(pt) || m_rcTable.PtInRect(pt))
		return TRUE;
	return FALSE;
}


int CLineObj::IsOnLine(CPoint pt)
{
	m_CurClickedPosition = N_PT;

	double a=0, b=0, c=0;

	// 수직선 일때
 	if(m_ptAStart.x == m_ptAEnd.x)
 	{
 		CRect rc(m_ptAStart.x - LINE_ON_SENSITIVITY, min(m_ptAStart.y, m_ptAEnd.y), m_ptAStart.x + LINE_ON_SENSITIVITY, max(m_ptAStart.y, m_ptAEnd.y));
 		if(rc.PtInRect(pt))m_CurClickedPosition = M_PT;
 	}
 	// 수평선 일때
 	else if(m_ptAStart.y == m_ptAEnd.y)
 	{
 		CRect rc(min(m_ptAStart.x, m_ptAEnd.x), m_ptAStart.y - LINE_ON_SENSITIVITY , max(m_ptAStart.x, m_ptAEnd.x), m_ptAStart.y + LINE_ON_SENSITIVITY);
 		if(rc.PtInRect(pt))m_CurClickedPosition = M_PT;
 	}
 	// 나머지 경우
 	else
	{
		// a*x+b*y+c=0 과 (x1, y1) 사이의 거리(ABS(a*x1+b*y1+c)/SQRT(a^2+b^2) )가
		// 가 LINE_ON_SENSITIVITY보다 작을 때 선택한 것으로 한다.
		a = m_ptAStart.y - m_ptAEnd.y;
		b = m_ptAEnd.x - m_ptAStart.x;
		c = m_ptAStart.x * m_ptAEnd.y - m_ptAEnd.x * m_ptAStart.y;

		CPoint ptTemp =  pt - m_ptALineOffset;
		if( (m_rcLine.PtInRect(ptTemp)) && (1.0*abs( int( a*pt.x + b*pt.y + c))/(1.0*sqrt(a*a+b*b)) <= LINE_ON_SENSITIVITY)	) 	
	 		m_CurClickedPosition = M_PT;
 	}
	return m_CurClickedPosition;
}

BOOL CLineObj::IsOnEndPoint(CPoint pt)
{
	if(pt.x >= m_ptAStart.x-RADIUS_OF_END_RC*2 && pt.x <= m_ptAStart.x+RADIUS_OF_END_RC*2 && pt.y >= m_ptAStart.y-RADIUS_OF_END_RC*2 && pt.y <= m_ptAStart.y+RADIUS_OF_END_RC*2)
	{
		m_CurClickedPosition = S_PT;
		m_ptAt = m_ptAStart - pt;
		return TRUE;
	}
	if(pt.x >= m_ptAEnd.x-RADIUS_OF_END_RC*2 && pt.x <= m_ptAEnd.x+RADIUS_OF_END_RC*2 && pt.y >= m_ptAEnd.y-RADIUS_OF_END_RC*2 && pt.y <= m_ptAEnd.y+RADIUS_OF_END_RC*2)
	{
		m_CurClickedPosition = E_PT;
		m_ptAt = m_ptAEnd - pt;
		return TRUE;
	}

	m_CurClickedPosition = N_PT;
	return FALSE;
}

CRect CLineObj::Draw(CDC* pDC)
{
	MakeData();
	ReCalcRect();
	CPen* pOldPen;
	CPen Pen (m_nLineStyle, m_nLineThickness, m_clrLineColor);
	CPen ExpandPen (PS_DOT, 1, m_clrLineColor);
	pOldPen = pDC->SelectObject(&Pen);

#if 1
////////////////////////////////////
// 확장선을 구분해서 그릴 경우
	pDC->MoveTo(m_ptAIndexedStart.x /*- m_ptAOffset.x*/ , m_ptAIndexedStart.y/* - m_ptAOffset.y*/);
	pDC->LineTo(m_ptAIndexedEnd.x /*- m_ptAOffset.x*/ , m_ptAIndexedEnd.y/* - m_ptAOffset.y*/);

	// 확장선 그리기
	pDC->SelectObject(&ExpandPen);
	pDC->MoveTo(m_ptAIndexedEnd.x /*- m_ptAOffset.x*/ , m_ptAIndexedEnd.y/* - m_ptAOffset.y*/);
	pDC->LineTo(m_ptAExpand.x /*- m_ptAOffset.x*/ , m_ptAExpand.y/* - m_ptAOffset.y*/);
#else
//////////////////////////////////////
// 확장선을 구분하지 않고 그릴 경우
	pDC->MoveTo(m_ptAIndexedStart.x, m_ptAIndexedStart.y/* - m_ptAOffset.y*/);
	pDC->LineTo(m_ptAExpand.x, m_ptAExpand.y/* - m_ptAOffset.y*/);
#endif
	if(m_bSelected)
		DrawEndPoints(pDC);
	if(m_bUpBuy || m_bUpSell || m_bDownBuy || m_bDownSell)
		DrawTable(pDC);
//	pDC->BitBlt(m_ptAOffset.x, m_ptAOffset.y, m_size.cx, m_size.cy, &m_dcMem, 0, 0,SRCINVERT);
	pDC->SelectObject(pOldPen);

	return CRect( 0, 0, 0, 0);
}

//////////////////////////////////////////////////////////////////////////
// 그리기 영역
// 마우스 클릭 시작/끝점(절대 좌표)으로 부터 상대 정보 얻는다
//
CRect CLineObj::GetRect()
{

//////////////////////////////////////////////////////////////////////////
// 절대 위치
//////////////////////////////////////////////////////////////////////////
	// 라인
	m_ptALineOffset.x = min(m_ptAStart.x, m_ptAExpand.x);
	m_ptALineOffset.y = min(m_ptAStart.y, m_ptAExpand.y);// - DEFAULT_HEIGHT;

	// 테이블
//	m_ptATableOffset =  m_ptALineOffset - m_ptLTOffset;
	// 텍스트
	m_ptATextOffset =  m_ptAStart + m_ptLTOffset;// + CPoint(OFFSET_TT_IN, OFFSET_TT_IN);
	// 전체
	m_ptAOffset.x = m_ptATextOffset.x - OFFSET_RC_IN;
	m_ptAOffset.y = min(m_ptALineOffset.y, m_ptATextOffset.y) - OFFSET_RC_IN;

//////////////////////////////////////////////////////////////////////////
// 상대 위치
//////////////////////////////////////////////////////////////////////////
	// 라인
	m_ptRLineOffset = CPoint(OFFSET_RC_IN, OFFSET_RC_IN - m_ptLTOffset.y);	// m_rc 기준
	m_ptRStart = m_ptAStart - m_ptAOffset - m_ptRLineOffset;				// m_rcLine 기준
	m_ptREnd = m_ptAEnd - m_ptAOffset - m_ptRLineOffset;					// m_rcLine 기준

	// 테이블
//	m_ptRTableOffset = CPoint(m_ptLTOffset.x + OFFSET_RC_IN, OFFSET_RC_IN);	// m_rc 기준
	// 텍스트
	m_ptRTextOffset =  CPoint(m_ptLTOffset.x + OFFSET_RC_IN+OFFSET_TT_IN, OFFSET_RC_IN+OFFSET_TT_IN);	// m_rc 기준


//////////////////////////////////////////////////////////////////////////
// 그리기 영역
//////////////////////////////////////////////////////////////////////////
	// 라인
	m_rcLine.right = abs(m_ptAStart.x - m_ptAExpand.x);
	m_rcLine.bottom = abs(m_ptAStart.y - m_ptAExpand.y);// + DEFAULT_HEIGHT;
	
	// 텍스트
	m_rcText.right = TEXT_WIDTH * 2;//max(m_strPeriod.GetLength(), (m_strBasicInfo1.GetLength()+m_strBasicInfo2.GetLength()+m_strBasicInfo3.GetLength()));
	m_rcText.bottom = m_nRowCnt*TEXT_HEIGHT;
	// 테이블
// 	m_rcTable = m_rcText;
// 	m_rcTable.InflateRect(0,0,0,20);

	// 전체
	m_rc.right = max(m_ptRLineOffset.x+m_rcLine.right, m_ptRTextOffset.x+m_rcText.right) + OFFSET_RC_IN;
	m_rc.bottom = max(m_ptRLineOffset.y+m_rcLine.bottom, m_ptRTextOffset.y+m_rcText.bottom) + OFFSET_RC_IN;

	m_size.cx = m_rc.Width();	// 전체 영역 폭
	m_size.cy = m_rc.Height();	// 전체 영역 높이

	return m_rc; 
}


void CLineObj::DrawEndPoints(CDC *pDC)
{
	CBrush brush;
	brush.CreateSolidBrush(RGB(255,255,255));
	CBrush* pOldBrush = pDC->SelectObject(&brush);
	CPen pen (PS_SOLID, 1, RGB(128,128,128));
	CPen* pOldPen = pDC->SelectObject(&pen);

	CRect rcStart, rcEnd;
	rcStart.left = m_ptRStart.x - RADIUS_OF_END_RC - OFFSET_RC_IN/2+1;
	rcStart.top = m_ptRStart.y - RADIUS_OF_END_RC - OFFSET_RC_IN/2+1;
	rcStart.right = m_ptRStart.x + 2*RADIUS_OF_END_RC - OFFSET_RC_IN/2+1;
	rcStart.bottom = m_ptRStart.y + 2*RADIUS_OF_END_RC - OFFSET_RC_IN/2+1;
	rcStart.OffsetRect(m_ptAOffset + m_ptRLineOffset);
	pDC->Rectangle(rcStart);

	rcEnd.left = m_ptREnd.x - RADIUS_OF_END_RC - OFFSET_RC_IN/2+1;
	rcEnd.top = m_ptREnd.y - RADIUS_OF_END_RC - OFFSET_RC_IN/2+1;
	rcEnd.right = m_ptREnd.x + 2*RADIUS_OF_END_RC - OFFSET_RC_IN/2+1;
	rcEnd.bottom = m_ptREnd.y + 2*RADIUS_OF_END_RC - OFFSET_RC_IN/2+1;
	rcEnd.OffsetRect(m_ptAOffset + m_ptRLineOffset);
	pDC->Rectangle(rcEnd);
	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
}
void CLineObj::SetPos(CPoint pt, int type)
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

CPoint CLineObj::GetPos(int type)
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

void CLineObj::ButtonDownAt(const CPoint &pt)
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
		m_ptAStart = pt;
	}
}

void CLineObj::ButtonUpAt(const CPoint &pt)
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


void CLineObj::ReCalcRect()
{
	m_oldrc = m_rc;
	GetRect();
}

void CLineObj::Erase(CDC* pDC)
{
//	pDC->BitBlt(m_ptAOffset.x, m_ptAOffset.y, m_size.cx, m_size.cy, &m_dcMem, 0, 0, SRCINVERT);
//	pDC->BitBlt(m_ptAExpand.x, m_ptAExpand.y, m_rcTable.Width(), m_rcTable.Height(), &m_dcMem, 0, 0, SRCINVERT);
//	TRACE2("Erase : %d, %d",m_rc.Width(),m_rc.Height());

}

void CLineObj::DrawTable(CDC* pDC)
{
// 	MakeData();
// 	ReCalcRect();

	// text 표시 속성
	CFont defFont, boldFont;

	CString strFont;
	strFont.LoadString(IDS_STRING_FONT);
	// defFont.CreatePointFont(90,_T("굴림"), NULL);
	defFont.CreatePointFont(90,strFont, NULL);
	
	LOGFONT BoldLog;
	defFont.GetLogFont(&BoldLog);
	BoldLog.lfWeight = FW_BOLD;
	boldFont.CreateFontIndirect(&BoldLog);
	pDC->SetBkColor(RGB(255,255,255));
	pDC->SetTextColor(m_clrLineColor); //RGB(157, 157, 217)
	CFont *poldFont = pDC->SelectObject(&defFont);
	pDC->SetBkMode(TRANSPARENT);

	// 그리기 영역 계산(폭)
	CRect rcTextDraw = m_rcText;
	rcTextDraw.OffsetRect(m_ptATextOffset);
	pDC->DrawText( m_strStar,rcTextDraw,DT_LEFT);

	pDC->SelectObject(poldFont);

}


///////////////////////////////////////////////////////////////////////////
// 기본 데이터 생성
void CLineObj::MakeData(void)
{


}


CString CLineObj::CommaStr(CString in)
{
	int NumOfComma, nPoint, nUnderPoint = 0, nLength;
	CString out = in;

	nPoint = out.Find(".",0);	// 소수점 위치
	if(nPoint >= 0)	nUnderPoint = out.GetLength() - nPoint;
	nLength = out.GetLength() - nUnderPoint;
	NumOfComma = nLength/ 3;
	if(in.GetLength() % 3 == 0)NumOfComma--;
	for(int i=0;i<NumOfComma;i++)
	{
		out.Insert(nLength - (4*i+3), ",");
	}
	return out;
}

void CLineObj::DrawExpand(CDC* pDC)
{
	ReCalcRect();
	// 임시 값
	CPen pen (m_nLineStyle, m_nLineThickness, m_clrLineColor);
	CPen* pOldPen = pDC->SelectObject(&pen);

	pDC->MoveTo(m_ptAEnd.x - m_ptAOffset.x , m_ptAEnd.y - m_ptAOffset.y);
	pDC->LineTo(m_ptAEnd.x - m_ptAOffset.x , m_ptAEnd.y - m_ptAOffset.y);
	//if(m_bSelected)
	DrawEndPoints(pDC);
//	DrawTable();	// 불투명 시에는 삭제
//	pDC->BitBlt(m_ptAOffset.x, m_ptAOffset.y, m_size.cx, m_size.cy, &m_dcMem, 0, 0,SRCINVERT);
	pDC->SelectObject(pOldPen);
}

void CLineObj::SetInfo()
{
	
}

void CLineObj::GenerateSerial()
{
	CTime t;
	t = CTime::GetCurrentTime();
	CString strTime = t.Format("%Y%m%d%H%M%S");
	m_strSerial = m_strCode+m_strCandle+strTime;
}
