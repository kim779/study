// KTBChartScroll.cpp: implementation of the Scroll & Zoom Code.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KTBChart.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"		// for _MTEXT()
#include "../Include_Chart/Conversion.h"				// for CDataConversion
#include "KTBChartCtl.h"								// for CKTBChartCtrl
#include "PacketList.h"									// for CPacketList
#include "ToolBarManager.h"								// for IToolBarManager
#include "MainBlock.h"									// for CMainBlock
#include "XScaleManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// (2004.11.09, 배승원) Scroll을 간단히 처리하는 Interface를 제공한다.
//		주의! 지정하는 Scroll Value가 잘못되어도 그 값에 최대로 근접하도록 조정되므로,
//			사전에 예외처리하지 않는다.
BOOL CKTBChartCtrl::ScrollTo( int p_nScrollValue, BOOL p_bRelative) 
{
	// 0. Check WholeView
	if( m_bWholeView) return FALSE;

	// 1. Get Start and End DRI
	// 1.0 Get Base DRI
	int nStartDRIOfView	= p_nScrollValue;
	int nEndDRIOfView	= p_nScrollValue + m_pxScaleManager->GetDrawCount_Cur() - 1;
	// 1.1 Update Relative
	if( p_bRelative)
	{
		nStartDRIOfView	+= m_pxScaleManager->GetStartDrawIdx_Cur();
		nEndDRIOfView	+= m_pxScaleManager->GetStartDrawIdx_Cur();
	}

	// 2. Do not update managed Info.

	// 3. Update X Scale Manager
	m_pxScaleManager->SetStartEndDrawIdx_Cur( nStartDRIOfView, nEndDRIOfView);

	// 5. Update View
	InvalidateControl();

	return TRUE;
}

// (2004.11.09, 배승원) Zoom을 간단히 처리하는 Interface를 제공한다.
//		주의! 지정하는 Zoom Value가 잘못되어도 그 값에 최대로 근접하도록 조정되므로,
//			사전에 예외처리하지 않는다.
BOOL CKTBChartCtrl::ZoomTo(long p_nZoomValue, BOOL p_bRelative) 
{
	if( !m_pxScaleManager) return FALSE;

	// TODO: Add your dispatch handler code here
	if( IsRunningNoZoomChart() && p_nZoomValue != m_pxScaleManager->GetCount()) return FALSE;

	// 1. Get Start and End DRI
	// 1.0 Get Base DRI
	int nStartDRIOfView	= m_pxScaleManager->GetStartDrawIdx_Cur();
	int nEndDRIOfView	= m_pxScaleManager->GetStartDrawIdx_Cur() + m_pxScaleManager->GetDrawCount_Cur() - 1;
	// 1.1 Modify DRI
	if( p_bRelative)	nStartDRIOfView	-= p_nZoomValue;
	else				nStartDRIOfView	-= ( p_nZoomValue - m_pxScaleManager->GetDrawCount_Cur());
	// 1.2 Check validation.
	int nZoomCount = ( nEndDRIOfView - nStartDRIOfView + 1);
	if( nZoomCount < m_nMinDataCountOnOnePage) nZoomCount = m_nMinDataCountOnOnePage;
	nStartDRIOfView = nEndDRIOfView - nZoomCount + 1;
	if( nStartDRIOfView < 0)
	{
		nStartDRIOfView = 0;
		nEndDRIOfView = nStartDRIOfView + nZoomCount - 1;
	}
	// (2009/2/19 - Seung-Won, Bae) If no-data, Set view range to (0:ZoomValue) for accepting of ZoomValue.
	if( 0 < m_pxScaleManager->GetCount() && m_pxScaleManager->GetCount() <= nEndDRIOfView)
	{
		nEndDRIOfView = m_pxScaleManager->GetCount() - 1;
		nStartDRIOfView = nEndDRIOfView - nZoomCount + 1;
	}
	if( nStartDRIOfView < 0) nStartDRIOfView = 0;
	if( nEndDRIOfView < 0) return FALSE;

	// 2. Update Info.
	// 2.1 Uncheck WholeView option.
	m_bWholeView = FALSE;

	// 2.2 One Page Data Count by User with fixed Item Width option.
	m_nOnePageDataCount = nZoomCount;

	// 3. Update X Scale Manager
	m_pxScaleManager->SetStartEndDrawIdx_Cur( nStartDRIOfView, nEndDRIOfView, p_bRelative);

	// (2009/6/2 - Seung-Won, Bae) Zoom to adjusted zoom value on relative.
	if( p_bRelative) m_nOnePageDataCount = m_pxScaleManager->GetDrawCount_Cur();

	// (2006/11/18 - Seung-Won, Bae) Notify the User Zoom State to MultiChart
	//>>JS.Kim_20101001. 사용자 스크롤 조작시에만 MultiChart로 이벤트 준다.
	if( p_bRelative )
	//<<
		OnUserZoomChanged( m_nOnePageDataCount);

	// 5. Update View
	InvalidateControl();

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// (2009/1/18 - Seung-Won, Bae) Zoom In/Out
//////////////////////////////////////////////////////////////////////
// (2006/2/6 - Seung-Won, Bae) DragZoomIn for DragZoom AddIn
void CKTBChartCtrl::DragZoomIn( const CPoint &p_ptStart, const CPoint &p_ptEnd)
{
	// 1. Get Start and End DRI
	int nStartDRIOfView = -1;
	int nEndDRIOfView = -1;
	// 1.1 Zoom out to Full View
	if( p_ptEnd.x < p_ptStart.x)
	{
		nStartDRIOfView		= 0;
		nEndDRIOfView		= m_pxScaleManager->GetCount() - 1;
	}
	// 1.3 X Scale Manager Version.
	else
	{
		nStartDRIOfView		= m_pxScaleManager->GetDRInFromPt( p_ptStart.x);
		nEndDRIOfView		= m_pxScaleManager->GetDRInFromPt( p_ptEnd.x);
	}
	// 1.4 Check reverse state.
	if( nEndDRIOfView < nStartDRIOfView)
	{
		int nDRI		= nStartDRIOfView;
		nStartDRIOfView	= nEndDRIOfView;
		nEndDRIOfView	= nDRI;
	}

	// 1.5 Check minimum data count on page.
	if( nEndDRIOfView - nStartDRIOfView + 1 < m_nMinDataCountOnOnePage)
	{
		// (2009/5/18 - Seung-Won, Bae) Zoom to min
		nEndDRIOfView += ( m_nMinDataCountOnOnePage - ( nEndDRIOfView - nStartDRIOfView + 1)) / 2;
		nStartDRIOfView = nEndDRIOfView - m_nMinDataCountOnOnePage + 1;
	}

	// 2. Update Info.
	// 2.1 Uncheck WholeView option.
	m_bWholeView = FALSE;
	// 2.2 One Page Data Count by User with fixed Item Width option.
	m_nOnePageDataCount = nEndDRIOfView - nStartDRIOfView + 1;

	// 3. Update X Scale Manager
	m_pxScaleManager->SetStartEndDrawIdx_Cur( nStartDRIOfView, nEndDRIOfView);

	// 5. Update View
	InvalidateControl();

	// (2006/11/18 - Seung-Won, Bae) Notify the User Zoom State to MultiChart
	OnUserZoomChanged( m_nOnePageDataCount);
}

/*-----------------------------------------------------------------------------------------
 - Function    :  OnMouseWheel
 - Created at  :  2005-02-23   15:07
 - Author      :  최종찬
 - Description :  마우스 휠을 움직일때 3개씩 봉데이타를 이동시킨다.
 -----------------------------------------------------------------------------------------*/
BOOL CKTBChartCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	int nScrollValue = 5;
	if( zDelta < 0 && !m_pxScaleManager->GetReverse()
		|| 0 < zDelta && m_pxScaleManager->GetReverse())
			nScrollValue = -5;

	if(m_nMouseWheelIndex == 0)
	{
		if( nFlags & MK_SHIFT)	ZoomTo( -nScrollValue, TRUE);
		else					ScrollTo( nScrollValue, TRUE);
	}
	else
	{
		if( nFlags & MK_SHIFT)	ScrollTo( nScrollValue, TRUE);
		else					ZoomTo( -nScrollValue, TRUE);
	}

	return COleControl::OnMouseWheel(nFlags, zDelta, pt);
}

void CKTBChartCtrl::SetOnePageDataCount(long nOnePageDataCount) 
{
	// (2009/5/18 - Seung-Won, Bae) Do not zoom on whole-view.
	if( IsWholeView())
	{
		if( nOnePageDataCount < m_nMinDataCountOnOnePage) nOnePageDataCount = m_nMinDataCountOnOnePage;
		if( m_pxScaleManager->GetCount() <= nOnePageDataCount)	nOnePageDataCount = m_pxScaleManager->GetCount();
		m_nOnePageDataCount = nOnePageDataCount;
	}
	else ZoomTo( nOnePageDataCount, FALSE);
}

void CKTBChartCtrl::SetWholeView(const BOOL bWholeView)
{
	if( m_bWholeView && bWholeView
		|| !m_bWholeView && !bWholeView) return;

	// 1. Get Start and End DRI
	int nStartDRIOfView = 0;
	int nEndDRIOfView = m_pxScaleManager->GetCount() - 1;
	if( !bWholeView) nStartDRIOfView = nEndDRIOfView - m_nOnePageDataCount + 1;

	// 2. Update Info.
	// 2.1 Update WholeView option.
	m_bWholeView = bWholeView;

	// 3. Update X Scale Manager
	m_pxScaleManager->SetStartEndDrawIdx_Cur( nStartDRIOfView, nEndDRIOfView);

	// 5. Update View
	InvalidateControl();

	// (2009/5/12 - Seung-Won, Bae) for Fixed Item Width
	OnUserZoomChanged( m_nOnePageDataCount);
}

//////////////////////////////////////////////////////////////////////
// (2004.11.09, 배승원, 분석) Zoom Value를 조회 및 설정하는 Property & Method이다.
//////////////////////////////////////////////////////////////////////
void CKTBChartCtrl::OnnOnePageDataCountChanged() 
{
	BoundPropertyChanged(dispidNOnePageDataCount);

	//sy 2005.11.17. 속성값만 바꾸는거지 화면에 반영을 하진 않는다!!
	//-> file 저장시 page count 를 바꿔서 저장하는 경우가 있기 때문에 스크롤에 반영하면 화면이 꿈뻑하고 움직임.
	//SetScrollData(SETSCROLL_TR, true, true);

	SetModifiedFlag();
}

//sy 2005.11.02.
void CKTBChartCtrl::SetOnePageDataCount2(long nOnePageDataCount, BOOL bIncludeBongCountMargin) 
{
	m_bIncludeBongCountMargin = (bIncludeBongCountMargin ? true: false);

	SetOnePageDataCount(nOnePageDataCount);
}
//sy end

// 한화면에 보이는 데이타수를 리턴한다
// 02.18.2002
long CKTBChartCtrl::GetOnePageDataCount() 
{
	return m_nOnePageDataCount;
}
// (2009/5/18 - Seung-Won, Bae) Do not use one page data count and total data count.
//		for MultiChart.
long CKTBChartCtrl::GetNCurrentDrawCount() 
{
	// TODO: Add your property handler here
	if( m_pxScaleManager) return m_pxScaleManager->GetDrawCount_Cur();
	return m_nOnePageDataCount;
}

//////////////////////////////////////////////////////////////////////
// (2004.11.09, 배승원, 분석) Print를 위한 Scrolling을 처리하는 Routine이다.
//		그러나 ToolBar가 있는 경우로 제한된 Old Version이어서 
//		ToolBar가 없는 화면의 경우 출력에 오류가 있을 수 있다.
//////////////////////////////////////////////////////////////////////
BOOL CKTBChartCtrl::SetScrollPosToNextPage( BOOL p_bBackward)
{
	int nScrollPos = m_pxScaleManager->GetStartDrawIdx_Cur();
	int nPrevScrollPos = nScrollPos;
	int nDrawCount = m_pxScaleManager->GetDrawCount_Cur();

	if( p_bBackward)	nScrollPos -= nDrawCount;
	else				nScrollPos += nDrawCount;
	ScrollTo( nScrollPos, FALSE);

	int nNextScrollPos = m_pxScaleManager->GetStartDrawIdx_Cur();
	return nNextScrollPos != nPrevScrollPos;
}
int CKTBChartCtrl::GetCurrentScrollPos()
{
	return m_pxScaleManager->GetStartDrawIdx_Cur();
}

//////////////////////////////////////////////////////////////////////
// (2004.11.09, 배승원, 분석) 일자 기준 Scroll Info 조회 Interface(Method)이다.
//////////////////////////////////////////////////////////////////////

// (2004.06.15, 배승원) Scrolling을 처리하는 Interface를 제공한다.
// 0. 자료일자로 Scrolling을 처리하는 Interface를 제공한다.
BOOL CKTBChartCtrl::GetStartEndDateOnView(double FAR* p_pStartDate, double FAR* p_pEndDate) 
{
	// TODO: Add your dispatch handler code here
	
	// 0. Pointer를 확인한다.
	if( !p_pStartDate || !p_pEndDate || !m_pPacketList) return FALSE;

	// 1. 먼저 Start/End Index를 구한다. (종찬 04/09/20: 0 Based)
	int nStartIndex, nEndIndex;
	if( !GetDataStartEnd( nStartIndex, nEndIndex)) return FALSE;

	// 2. 각 Index에 해당하는 Data를 추출한다.
	// 2.1 자료일자 Packet을 구한다.
	CPacket *pPacket = m_pPacketList->GetBaseData( _MTEXT( C0_DATE_TIME));
	if( !pPacket) return FALSE;
	// 2.2 '자료일자'로부터 해당 Index의 Data를 구한다. (0 Based)
	*p_pStartDate	= pPacket->GetData( nStartIndex);
    *p_pEndDate		= pPacket->GetData( nEndIndex);

	// (2004.07.06, 배승원) 검색된 자료일자가 유효하지 않는 경우,
	//		마지막 Data의 자료일자로 처리한다.
	if( *p_pEndDate == -DBL_MAX)	// 하나라도 유효하지 않는가?
	{
		// 3.0 먼저 마지막 Data로 Update하기 위한 마지막 일자를 확보한다.
		int nDataCount = pPacket->GetCount();
		double dLastDate = 0;
		if( 1 <= nDataCount) dLastDate = pPacket->GetData( nDataCount - 1);
		
		// 3.1 End Date값을 확보한다.
		*p_pEndDate = dLastDate;

		// 3.2 Start Date도 없는 경우를 처리한다.
		if( *p_pStartDate == -DBL_MAX) *p_pStartDate = dLastDate;
	}

	return TRUE;
}

// (2004.11.09, 배승원, 분석) 일자 기준 Scroll Info 설정 Interface이다.
BOOL CKTBChartCtrl::SetStartEndDateOfView(double p_dStartDate, double p_dEndDate) 
{
	// TODO: Add your dispatch handler code here

	// 0. Pointer를 확인한다.
	if( !m_pPacketList) return FALSE;

	// 1. 주어진 일자 정보를 이용하여 Data Index를 구한다.
	// 1.1 먼저 자료일자 Packet을 구한다.
	CPacket *pPacket = m_pPacketList->GetBaseData( _MTEXT( C0_DATE_TIME));
	if( !pPacket) return FALSE;
	// 1.2 각 자료일자에 대한 Data Index를 구한다. (0 Based Index)
	int nStartIndex	= pPacket->GetIndexFromData( p_dStartDate);
	int nEndIndex	= pPacket->GetIndexFromData( p_dEndDate);
	if( nStartIndex < 0 || nEndIndex < 0) return FALSE;

	// 2. 구해진 Data Index를 기준으로 Scrolling을 처리한다. (종찬 04/09/20: 0 Based Index)
	SetStartEndIndex( nStartIndex, nEndIndex);

	return TRUE;
}

// (2004.11.09, 배승원, 분석) 일자 기준 Scroll Info 설정 Interface이다.
// (2004.06.17, 배승원) 주어진 자료일자를 기준으로 Scroll 하는 Interface를 제공한다.
// (2004.06.28, 배승원) 주어진 자료일자는 View의 우측끝으로 지정되도록 수정한다.
BOOL CKTBChartCtrl::ScrollToDataOfEnd(double p_dData) 
{
	// TODO: Add your dispatch handler code here

	// 0. Pointer를 확인한다.
	if( !m_pPacketList) return FALSE;

	// 1. 먼저 현재의 한 화면에 보이는 Data 수를 구한다.
	// 1.1 현재 보이는 Data의 Start/End Index를 구한다. (종찬 04/09/20: 0 Based)
	int nStartIndex, nEndIndex;
	if( !GetDataStartEnd( nStartIndex, nEndIndex)) return FALSE;
	// 1.2 한 화면에 보이는 Data수를 구한다.
	int nPageDataCount = nEndIndex - nStartIndex + 1;

	// 2. 주어진 일자 정보를 이용하여 Data Index를 구한다.
	// 2.1 먼저 자료일자 Packet을 구한다.
	CPacket *pPacket = m_pPacketList->GetBaseData( _MTEXT( C0_DATE_TIME));
	if( !pPacket) return FALSE;
	// (2004.06.28, 배승원) 주어진 자료일자는 View의 우측끝으로 지정되도록 수정한다.
	// 2.2 끝 자료일자에 대한 Data Index를 구한다. (0 Based Index)
	nEndIndex = pPacket->GetIndexFromData( p_dData);
	if( nEndIndex < 0) return FALSE;
//	nEndIndex++; // 종찬(04/09/20) 0-Base
	// (2004.07.06, 배승원) 구해진 Data Index가 화면에 보이는 영역이 아닌 경우를 제외한다.
	// 2.2.5 전체 ScrollRange를 구한다.
	int nFullRange;
	if( !GetScrollRange( nFullRange)) return FALSE;
	// 2.2.6 전체 ScrollRange보다 우측의 경우, False로 return한다.
	if( nFullRange < nEndIndex) return FALSE;
	// 2.3 시작 자료일자를 구한다.
	nStartIndex	= nEndIndex - nPageDataCount + 1;

	// 3. 한 화면에 보이는 Data수를 감안하여 nStartIndex를 보정한다.
	// 3.2 새로운 StartIndex에 의한 EndIndex의 OverFlow를 확인하여 조정한다.
	if( nFullRange < nStartIndex + nPageDataCount - 1) nStartIndex = nFullRange - nPageDataCount + 1;
	// 3.3 새로운 StartIndex에 대한 EndIndex를 보정한다.
	nEndIndex = nStartIndex + nPageDataCount - 1;

	// 4. 구해진 Data Index를 기준으로 Scrolling을 처리한다. (종찬 04/09/20: 0 Based Index)
	SetStartEndIndex( nStartIndex, nEndIndex);

	return TRUE;
}

//sy 2005.11.17. -> 오른쪽 여백 봉수 포함하지 않은 경우. 
int	CKTBChartCtrl::GetOnePageDataCountForScroll() const
{
	//오른쪽 여백 봉수 포함하지 않은 경우.
//	if(!m_bIncludeBongCountMargin && m_nBongCountMargin > 0)
//	{
//		return (m_nOnePageDataCount + m_nBongCountMargin);
//	}

	return m_nOnePageDataCount;
}

//sy 2005.06.16. 특수차트 인 경우 x축 count가 틀림
int CKTBChartCtrl::GetFullDataCount(const bool bIsHorzScaleDataCheck) const
{
	if(m_pPacketList == NULL)
		return 0;

	if(!bIsHorzScaleDataCheck || !IsScrollInHorzScaleData())
		return m_pPacketList->GetMaxCount();

	// 특수형 차트일 경우만 x축 scale data로 scroll count 조절한다
	int nCount = m_pPacketList->GetCount(m_pMainBlock->GetHorzScaleData(CBlockIndex(0, 0)));
	if(nCount > 0)
		return nCount;

	return m_pPacketList->GetMaxCount();
}

bool CKTBChartCtrl::IsScrollInHorzScaleData() const
{
	if(m_pMainBlock == NULL)
		return false;

	CString strIndicatorName = m_pMainBlock->GetIndicatorName(CBlockIndex(0, 0), 0, 0);
	if(strIndicatorName.IsEmpty())
		return false;

	return g_iMetaTable.IsSpecialGroupGraph(strIndicatorName);
}
//sy end


//////////////////////////////////////////////////////////////////////
// (2004.11.09, 배승원, 분석) Index 기준 Scroll Info 조회 Interface이다.
//////////////////////////////////////////////////////////////////////

// (2004.07.02, 배승원) MaxRange도 조회하는 Interface를 제공한다. (마지막 Index의 값이된다.)
BOOL CKTBChartCtrl::GetScrollRange( int &p_nRange)
{
	// 0. 처리 환경을 확인한다.
	if( !m_pToolBarManager || !m_pMainBlock) return FALSE;

	// 1. 먼저 전체 보기인 경우를 Check 한다.
	if( IsWholeView())
	{
		int nStartIndex;
		m_pMainBlock->GetDisplayDataIndex( nStartIndex, p_nRange);
		// 1.1 GetDisplayDataIndex()는 0 Base로 조회하여 조정한다.
		// 종찬(04/09/20) 미니툴바의 Scroll부분도 0 Base이므로 그대로 보낸다.
//		p_nRange++;
		return TRUE;
	}

	// 2. ScrollBar를 통하여 Scrolling을 처리하는 경우를 확인한다.
	// 종찬(04/11/16) MaxRange 구해오는 함수를 제공하므로 그걸 사용함.
	p_nRange = GetMaxRange();
	// 종찬(04/09/20) 미니툴바의 Scroll부분도 0 Base이므로 한개를 빼서...
	p_nRange--;

	return TRUE;
}

// (2004.11.09, 배승원, 분석) Index 기준 Scroll Info 조회 Interface이다.
// 1. Data Index로 Scrolling을 지정하는 Routine을 제공한다. (종찬 04/09/20: 0 Based Index로 처리한다.)
BOOL CKTBChartCtrl::GetDataStartEnd( int &p_nStartIndex, int &p_nEndIndex)
{
	// 0. 처리 환경을 확인한다.
	if( !m_pToolBarManager || !m_pMainBlock) return FALSE;

	// 1. 먼저 전체 보기인 경우를 Check 한다.
	if( IsWholeView())
	{
		m_pMainBlock->GetDisplayDataIndex( p_nStartIndex, p_nEndIndex);
		// 1.1 GetDisplayDataIndex()는 0 Base로 조회하여 조정한다.
//		p_nStartIndex++;
//		p_nEndIndex++;
		return TRUE;
	}

	// 2. ScrollBar를 통하여 Scrolling을 처리하는 경우를 확인한다.
	// 종찬(04/11/16) 조건문 수정
	if(IsUseToolScrollBar())
	if( m_pToolBarManager->GetStartEndIndex(p_nStartIndex, p_nEndIndex))
	{
		// (2004.07.01, 배승원) Index값이 잘못 조회되어 나오는 경우를 대비한다.
		if( p_nStartIndex < 0 || p_nEndIndex < 0 || p_nEndIndex < p_nStartIndex || (p_nStartIndex == 0 && p_nEndIndex == 0))
		{
			//sy 2004.09.15.
			//-> Map에 스크롤이 없을 경우 start, end 를 0 로 넘기기 때문에
			//   올바른 start, end를 가져올수 없다.
			//   이때 MainBlock에서 정보를 가져온다.
			m_pMainBlock->GetDisplayDataIndex(p_nStartIndex, p_nEndIndex);
//			if(p_nStartIndex >= 0)
//				p_nStartIndex++;
//			if(p_nEndIndex >= 0)
//				p_nEndIndex++;
			return (p_nStartIndex < 0 || p_nEndIndex < 0 || p_nEndIndex < p_nStartIndex || (p_nStartIndex == 0 && p_nEndIndex == 0));
			//sy end
		}
		return TRUE;
	}

	// 3. ScrollBar 없이 없는 경우, 직접 Scrolling을 처리한다.
	m_pMainBlock->GetDisplayDataIndex( p_nStartIndex, p_nEndIndex);
	// 종찬(04/09/20) ScrollBar쪽도 0 Base로 바꿈.
//	if(p_nStartIndex >= 0)
//		p_nStartIndex++;
//	if(p_nEndIndex >= 0)
//		p_nEndIndex++;
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// (2004.11.09, 배승원, 분석) Index 기준 Zoom Value를 설정하는 Interface(Method)이다.
//////////////////////////////////////////////////////////////////////

// (2004.07.05, 배승원) 한 화면에 보이는 Data 수를 환경치가 아니라 Zoom Value로 조정하는 Method를 제공한다.
//	1. OnePageDataCount Property를 조정할 경우, 환경설정에서의 한화면에 보이는 Data수 조정으로,
//		여백을 생성하며, 또한 우측끝으로 자동 Scroll하게 되는데,
//		이런 기능이 아닌 순수 Zoom (우측기준)을 처리하는 Method를 제공한다.
//		주의!! : 현재 환경설정의 '한 화면에 보이는 Data 수'는 Zoom Value를 참조하여 보여준다.
//			그러나 그곳에서의 설정은 Zoom Value의 기능이 아님에 유의한다.
//	2. 특히 ZoomTo의 with Right Fix를 이용할 경우, Zoom Out시 좌측이 끝에 닿아도,
//		우측을 확장하지 않도록 한다. (시간연동의 경우 그런 경우를 배제한다.)
short CKTBChartCtrl::ZoomToWithRightFix(short p_nZoomValue, BOOL p_bWithRightFix) 
{
	// TODO: Add your dispatch handler code here

	// (2004.05.18, 배승원) 한 화면에 보이는 Data 수 (ZoomValue)의 조정을 반영한다.
	// (2004.07.02, 배승원) 한 화면에 보이는 수가 바뀌는 경우에도, ScrollBar를 중심으로 처리한다.
	// 1. 현재 View의 Start/End Index를 구한다.
	int nStartIndex, nEndIndex;
	if( !GetDataStartEnd( nStartIndex, nEndIndex)) return FALSE;
	// 2. 조정된 한 화면의 Data수에 맞춰 Start Index를 조정한다.
	nStartIndex = nEndIndex - p_nZoomValue + 1;
	// 3. StartIndex의 유효성을 확인하고, 문제시 End Index를 조정한다.
	if( nStartIndex < 0)	// 종찬(04/09/20): 0 Base
	{
		nStartIndex = 0;
		// (2004.07.05, 배승원) EndIndex의 조정은 p_bWithRightFix의 값이 False인 경우에만 처리한다.
		if( !p_bWithRightFix)
		{
			nEndIndex = nStartIndex + p_nZoomValue - 1;
			// 4. End Index가 전체 Index보다 넘어서는 문제가 발생시에는 한 화면의 Data 수를 보정한다.
			int nScrollRange;
			if( !GetScrollRange( nScrollRange)) return FALSE;
			if( nScrollRange < nEndIndex) nEndIndex = nScrollRange;
		}
		// 4.5 End Index의 조정이 되었건 그렇지 않건 간에, Zoom Value를 새로이 구하여 (전과 같을 수 있다.)
		//		변경되는 경우의 Zoom Value를 구한다.
		p_nZoomValue = nEndIndex - nStartIndex + 1;
	}
	// 5. 보정된 Start/End Index를 설정한다.
	SetStartEndIndex( nStartIndex, nEndIndex);

	return p_nZoomValue;
}

//////////////////////////////////////////////////////////////////////
// (2004.11.09, 배승원) Scroll 및 Zoom을 간단히 처리하는 Interface를 제공한다.
//////////////////////////////////////////////////////////////////////

// (2009/1/5 - Seung-Won, Bae) Update ScrollBar and Zoom SliderBar from XScaleManager by Real.
void CKTBChartCtrl::UpdateZoomAndScrollInfo( int p_nFullRange, int p_nStartPos, int p_nViewCount)
{
	// (2009/5/19 - Seung-Won, Bae) for Auto Resize of Print and Image Capture
	if( m_bLockScrollUIUpdate) return;

	if( m_pMainBlock) m_pMainBlock->SetDataRegionInBlock2( p_nStartPos, p_nStartPos + p_nViewCount - 1);
	if( m_pToolBarManager) m_pToolBarManager->UpdateZoomAndScrollInfo( p_nFullRange, p_nStartPos, p_nViewCount, m_bWholeView);

	// 복수종목의 Total Count를 멀티차트에 알려줌 : 복수종목 - ojtaso (20070704)
	SendEventMsgToDLL( UWM_MULTITOTALCOUNT, 0, p_nFullRange);

	// 4. Notify DLL to manage of the user set, the one page data count.
	// (2007/5/23 - Seung-Won, Bae) Notify Data Count with '자료일자' Priority for Special Chart.
	NeoChangeOnePageDataCount( p_nViewCount);
}


// (2009/5/11 - Seung-Won, Bae) Reset Scroll.
void CKTBChartCtrl::ResetScroll( BOOL p_bWithCalcDRI)
{
	if( !m_pxScaleManager) return;
	if( p_bWithCalcDRI) m_pxScaleManager->Calc_DRI();

	// assume that Calc_DRI() was finished.
	// 1. Get Start and End DRI
	int nEndDRIOfView = m_pxScaleManager->GetCount() - 1;
	int nStartDRIOfView = nEndDRIOfView - m_nOnePageDataCount + 1;
	if( m_bWholeView || nStartDRIOfView < 0) nStartDRIOfView = 0;
	if( 0 <= nEndDRIOfView) m_pxScaleManager->SetStartEndDrawIdx_Cur( nStartDRIOfView, nEndDRIOfView);
	
	InvalidateControl();
}
