// ScrollBarEx.cpp : implementation file
//

#include "stdafx.h"
#include "ScrollBarEx.h"

#include "../Include_Chart/Dll_Load/XScaleManager.h"
#include "../Include_Chart/IChartCtrl.h"				// for IChartCtrl
#include "ToolBarDlg.h"
#include "DataMath.h"
#include "Conversion.h"
#include "SliderCtrlEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScrollBarEx

CScrollBarEx::CScrollBarEx( IChartCtrl *p_pIChartCtrl, int nScrollWidth)
{
	// (2009/1/18 - Seung-Won, Bae) Manage scroll state for next operation calculating.
	m_pIChartCtrl	= p_pIChartCtrl; 
	m_nPreScrollPos = 0;
	
	m_bFullPage = false;
	m_nTotalDataCount = 0;
	m_nDataEnd = 0;
	m_nScrollWidth = nScrollWidth;
	m_nMinDataCountOnOnePage = 5;
	m_pParent = NULL;
}

CScrollBarEx::~CScrollBarEx()
{
}


BEGIN_MESSAGE_MAP(CScrollBarEx, CScrollBar)
	//{{AFX_MSG_MAP(CScrollBarEx)
	ON_WM_HSCROLL()
	ON_WM_LBUTTONDOWN()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScrollBarEx message handlers

void CScrollBarEx::SetScrollData_TR(const int nTotalDataCount, const int nPageDataCount, const bool bFullPage, CPacketDataInfo::DATAINSERTTYPE dataType, const int nGlanceBalanceMargin)
{
	//sy 2005.09.15.
	//장전 TR 조회시엔 기본셋팅을 안하게 되므로 주석처리해야 한다.
	//if(nTotalDataCount == 0)
	//	return;
	//sy end
	//int reverseDataTotalCount = m_nTotalDataCount;
	
	m_nGlanceBalanceMargin = nGlanceBalanceMargin;
	m_nTotalDataCount = nTotalDataCount; 
	m_bFullPage = bFullPage;
	
	if(bFullPage == true)
		m_nPageDataCount = m_nTotalDataCount;
	//한화면에 보인는 데이터 수 때문에 변경.
	else if(m_nTotalDataCount < nPageDataCount)
	{
		m_nPageDataCount = nPageDataCount;
		m_nTotalDataCount = m_nPageDataCount;

	}
	else
	{
		m_nPageDataCount = nPageDataCount;
		if(dataType == CPacketDataInfo::INSERT && !m_bFullPage)
		{
			//sy 2005.12.13. 이전조회시 기존소스 잘못됨.
			m_nDataEnd = m_nTotalDataCount - 1;
			m_nPreScrollPos = m_nDataEnd - m_nPageDataCount + 1;
			if(m_nPreScrollPos < 0)
				m_nPreScrollPos = 0;
			// 종찬(04/07/16)
			// '이전'버튼을 클릭하여 옛날데이타를 가져 왔을 경우 가져온데이타만큼 index에 뿌라쓰시켜준다.
			//m_nPreScrollPos += (m_nTotalDataCount - reverseDataTotalCount);
			//m_nDataEnd += (m_nTotalDataCount - reverseDataTotalCount);
			//sy end
		}
	}

	if(dataType == CPacketDataInfo::INSERT && !m_bFullPage)
	{
		SetScrollPos();
		Invalidate();
		m_pParent->SetSliderPos(m_nPageDataCount);
		m_bFullPage = false;
	}
	else
	{
		//MODIFY: 종찬(04/06/30)
		m_nDataEnd = m_nTotalDataCount - 1;
		m_nPreScrollPos = m_nDataEnd - m_nPageDataCount + 1;
		if(m_nPreScrollPos < 0)
			m_nPreScrollPos = 0;
		CScrollBarEx::SetScrollInfo();
	}
}


/*-----------------------------------------------------------------------------------------
 - Function    :  SetScrollPos
 - Created at  :  2004-10-27   18:17
 - Author      :  최종찬
 - Description :  Data의 StartIndex에 따른 ScrollBox의 위치를 지정한다.
 -----------------------------------------------------------------------------------------*/
void CScrollBarEx::SetScrollPos()
{
	// (2005/9/10 - Seung-Won, Bae) 
	if( !m_nTotalDataCount) return;
	if(m_nPageDataCount >= 1 && m_nTotalDataCount >= 1)
		m_ScrollInfo.nPage = CMath::Round(((double)m_nScrollWidth * (double)m_nPageDataCount) / (double)m_nTotalDataCount);
	else
		m_ScrollInfo.nPage = m_nScrollWidth;
	CScrollBarNew::SetScrollInfo(&m_ScrollInfo, TRUE, m_pParent->GetReverseToOcx());

	CScrollBarNew::SetScrollPos(CMath::Round(double(m_nPreScrollPos * m_nScrollWidth)/(double)m_nTotalDataCount), TRUE, m_pParent->GetReverseToOcx());
}


void CScrollBarEx::SetScrollInfo()
{
	 int scrollMax, scrollPage, scrollPos, scrollTrackPos;
 
	 if(m_bFullPage == true)
	 {
		  scrollPage = m_nScrollWidth;
	 }
	 else
	 {
	  if(m_nPageDataCount >= 1 && m_nTotalDataCount >= 1)
		  scrollPage = int((m_nScrollWidth * m_nPageDataCount) / m_nTotalDataCount);
	  else
		  scrollPage = m_nScrollWidth;
	 }
    
	 scrollMax = m_nScrollWidth;

	// (2009/5/29 - Seung-Won, Bae) Do not reset scroll position.
	//	 if(m_nPageDataCount == m_nTotalDataCount)
	//		 scrollPos = 0;
	//	 else
	//		 scrollPos = m_nScrollWidth;

	// 20100520 박동관 m_nTotalDataCount가 0으로 오거나 m_nPreScrollPos가 0으로 올때 죽는 현상 수정 >>
	//scrollPos = CMath::Round( double( m_nPreScrollPos * m_nScrollWidth)/(double)m_nTotalDataCount);
	if(m_nPreScrollPos == 0 || m_nScrollWidth == 0 || m_nTotalDataCount == 0) scrollPos = 0;
	else scrollPos = CMath::Round( double( m_nPreScrollPos * m_nScrollWidth)/(double)m_nTotalDataCount);
	// 20100520 박동관 <<

	 if(m_nTotalDataCount <= 0)
		scrollTrackPos = 1;
	 else
		scrollTrackPos = CMath::Round((double)m_nScrollWidth / (double)m_nTotalDataCount);

  	 m_ScrollInfo.cbSize = sizeof(m_ScrollInfo);		 // size of this structure
	 m_ScrollInfo.fMask = SIF_ALL;						 // parameters to set
	 m_ScrollInfo.nMin = 0;							     // minimum scrolling position
	 m_ScrollInfo.nMax = scrollMax;			  		     // maximum scrolling position
	 m_ScrollInfo.nPage = scrollPage;				     // the page size of the scroll box
	 m_ScrollInfo.nPos = scrollPos;						 // initial position of the scroll box
	 m_ScrollInfo.nTrackPos = scrollTrackPos;			 // immediate position of a scroll box that the user is dragging
	 CScrollBarNew::SetScrollInfo(&m_ScrollInfo, TRUE, m_pParent->GetReverseToOcx());
	 CScrollBarNew::SetScrollPos(m_ScrollInfo.nPos, TRUE, m_pParent->GetReverseToOcx());
	 if(this->m_hWnd != NULL) 
		 Invalidate();
}

//tour2k pScrollbar 인자 추가
void CScrollBarEx::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{	
	int nScrollValue = 0;
	switch( nSBCode)
	{
	case SB_LINELEFT:	nScrollValue = -1;
						if( m_pParent->GetReverseToOcx()) nScrollValue = -nScrollValue;
						break;
	case SB_LINERIGHT:	nScrollValue = +1;
						if( m_pParent->GetReverseToOcx()) nScrollValue = -nScrollValue;
						break;
	case SB_PAGELEFT:	nScrollValue = -m_nPageDataCount;
						if( m_pParent->GetReverseToOcx()) nScrollValue = -nScrollValue;
						break;
	case SB_PAGERIGHT:	nScrollValue = +m_nPageDataCount;
						if( m_pParent->GetReverseToOcx()) nScrollValue = -nScrollValue;
						break;
	case SB_THUMBPOSITION:	nScrollValue = CMath::Round( ( double( CScrollBarNew::GetScrollPos( FALSE)) * double( m_nTotalDataCount)) / double( m_nScrollWidth));
							if( m_pParent->GetReverseToOcx()) nScrollValue = m_nTotalDataCount - nScrollValue - m_nPageDataCount;
							nScrollValue -= m_nPreScrollPos;
							break;
	case SB_THUMBTRACK:	nScrollValue = CMath::Round( ( ( double)( nPos) *  double( m_nTotalDataCount)) / double( m_nScrollWidth));
						if( m_pParent->GetReverseToOcx()) nScrollValue = ( m_nTotalDataCount - m_nPageDataCount) - nScrollValue;
						nScrollValue -= m_nPreScrollPos;
						break;
	// (2009/1/8 - Seung-Won, Bae) Do not response to SB_ENDSCROLL. (It cause the repaint.)
	default:
		return;
	}//end of switch
	m_pIChartCtrl->ScrollTo( nScrollValue, TRUE);
}

void CScrollBarEx::GetDataStartEnd(int& nDataStart, int& nDataEnd)
{
	nDataStart = m_nPreScrollPos;
	nDataEnd = m_nDataEnd;
}

void CScrollBarEx::SetScrollData_Real(const int nTotalDataCount)
{
	// MODIFY: 종찬(04/06/01) TR이 없는 틱차트에서 초기 틱이 들어와도 그려지지 않는 버그 수정
	if(nTotalDataCount >= m_nTotalDataCount)
	{
		int nTemp = nTotalDataCount - m_nTotalDataCount;

		if(!m_bFullPage) // 종찬(04/07/28): 전체보기가 아닐경우는 데이타가 흘러야 하므로...
			m_nPreScrollPos += nTemp;

		m_nDataEnd+= nTemp;
		// MODIFY: 종찬(04/06/29) SliderCtrl의 MaxRange를 가장 큰값으로 유지한다.
		m_nTotalDataCount = nTotalDataCount;
	}

	bool bMoveEndPage = false;
	if(m_nDataEnd == (m_nTotalDataCount-1))
		bMoveEndPage = true;

	
	if(m_bFullPage == true)
		m_nPageDataCount = m_nTotalDataCount;
	else if(m_nTotalDataCount < m_nPageDataCount)
		m_nTotalDataCount = m_nPageDataCount;

	SetScrollPos();

	if(bMoveEndPage == true){
		if(m_nTotalDataCount == m_nPageDataCount)
			m_ScrollInfo.nPos = 0;
		else
			m_ScrollInfo.nPos = m_nScrollWidth;
		CScrollBarNew::SetScrollPos(m_ScrollInfo.nPos, TRUE, m_pParent->GetReverseToOcx());
	}
}

/*-----------------------------------------------------------------------------------------
 - Function    :  SetScrollData_RemovePacket
 - Created at  :  2004-07-22   16:37
 - Author      :  최종찬(mono@dooriic.co.
 - Parameters  :  nTotalDataCount   - 표현할 데이타의 총갯수(여백포함)
 -				  bFullPage		   - '전체보기' 상태 여부
 -                nRemoveDataCount - 지워진 패킷데이타 갯수
 - Description :  데이타의 갯수가 해당PacketList가 담을수 있는 임계치를 넘어서면 오래된 데이타를
				  삭제하게 된다. 데이타 삭제가 발생하는 시점은 PacketList에서 알려주며 Real발생시
				  가 된다.
 -----------------------------------------------------------------------------------------*/
void CScrollBarEx::SetScrollData_RemovePacket(const int nTotalDataCount, const int nRemoveDataCount)
{
	// 오른쪽 여백 구하기
	int nRightMargin = m_nTotalDataCount - nTotalDataCount - nRemoveDataCount;
	if(nRightMargin < 0)
		nRightMargin = 0;

	// MODIFY: 종찬(04/07/30)
	m_nTotalDataCount = nTotalDataCount + nRightMargin;

	// 한페이지에 보여줄 데이타 갯수가 TotalDataCount를 넘어서면안된다.
	if(m_nPageDataCount > m_nTotalDataCount)
		m_nPageDataCount = m_nTotalDataCount;

	m_nPreScrollPos = m_nPreScrollPos - nRemoveDataCount + 1;
	m_nDataEnd = m_nDataEnd - nRemoveDataCount + 1;

	if(m_nPreScrollPos < 0)
	{
		m_nPreScrollPos = 0;
		m_nDataEnd = m_nPageDataCount - 1;
	}

	SetScrollPos();
	m_pParent->SetSliderPos(m_nPageDataCount);
}

int CScrollBarEx::GetTotalDataCount()
{
	return m_nTotalDataCount;
}


void CScrollBarEx::SetScrollBarWidth(const int nWidth)
{
	m_nScrollWidth = nWidth;
	SetScrollInfo();
}


/*-----------------------------------------------------------------------------------------
 - Function    :  OnSetFocus
 - Created at  :  2005-09-10   15:50
 - Author      :  최종찬
 - Description :  ScrollBar에 포커스가 들어오면 OCX에 포커스를 준다.
 // (2005/9/10 - Seung-Won, Bae) This ScrollBar did not catch focus. So, Make it to catch the focus in OnLButtonDown.
  -----------------------------------------------------------------------------------------*/
void CScrollBarEx::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CWnd *pParent = GetParent();			// Getting ToolBar DLG
	if( pParent)
	{
		pParent = pParent->GetParent();
		if( pParent) pParent->SetFocus();	// Getting Chart OCX
	}

	CScrollBarNew::OnLButtonDown(nFlags, point);
}
// (2005/9/10 - Seung-Won, Bae) For Focus Testing of OCX
void CScrollBarEx::OnSetFocus(CWnd* pOldWnd) 
{
	CScrollBarNew::OnSetFocus(pOldWnd);
	
	// TODO: Add your message handler code here

	// (2005/9/10 - Seung-Won, Bae) Caution! don't call OnSetFocus() in OnLButtonDown().
	//	So. Copy from OnLButtonDown.
	CWnd *pParent = GetParent();			// Getting ToolBar DLG
	if( pParent)
	{
		pParent = pParent->GetParent();
		if( pParent) pParent->SetFocus();	// Getting Chart OCX
	}
}

// (2009/1/5 - Seung-Won, Bae) Update ScrollBar and Zoom SliderBar from XScaleManager by Real.
void CScrollBarEx::UpdateZoomAndScrollInfo( int p_nFullRange, int p_nStartPos, int p_nViewCount)
{
	m_nPageDataCount = p_nViewCount;
	m_nTotalDataCount = p_nFullRange;
	m_nPreScrollPos = p_nStartPos;
	m_nDataEnd = p_nStartPos + p_nViewCount - 1;
	if( 0 < m_nTotalDataCount)
	{
		m_ScrollInfo.nPage = CMath::Round(((double)m_nScrollWidth * (double)m_nPageDataCount) / (double)m_nTotalDataCount);					// the page size of the scroll box
		m_ScrollInfo.nPos = CMath::Round(((double)m_nScrollWidth * (double)m_nPreScrollPos) / (double)m_nTotalDataCount);					// initial position of the scroll box
		m_ScrollInfo.nTrackPos = CMath::Round((double)m_nScrollWidth / (double)m_nTotalDataCount);				// immediate position of a scroll box that the user is dragging
		CScrollBarNew::SetScrollInfo( &m_ScrollInfo, TRUE, m_pParent->GetReverseToOcx());
		CScrollBarNew::SetScrollPos( CMath::Round( double( m_nPreScrollPos * m_nScrollWidth)/(double)m_nTotalDataCount), TRUE, m_pParent->GetReverseToOcx());
	}
	else
	{
		m_ScrollInfo.nPage = 1;					// the page size of the scroll box
		m_ScrollInfo.nPos = 0;					// initial position of the scroll box
		m_ScrollInfo.nTrackPos = 0;				// immediate position of a scroll box that the user is dragging
		CScrollBarNew::SetScrollInfo( &m_ScrollInfo, TRUE, m_pParent->GetReverseToOcx());
		CScrollBarNew::SetScrollPos( 0, TRUE, m_pParent->GetReverseToOcx());
	}
}
