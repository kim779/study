// ToolBarManager.cpp: implementation of the ToolBarManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../Include_Chart/Dll_Load/ToolBarManager.h"

#include "../Include_Chart/Dll_Load/PacketList.h"
#include "../Include_Chart/Dll_Load/MainBlock.h"
#include "../Include_Chart/Dll_Load/IndicatorList.h"
#include "../Include_Chart/IChartCtrl.h"				// for IChartCtrl

#include "ToolBarManagerImplementation.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CToolBarManager::CToolBarManager( IChartCtrl *p_pIChartCtrl)
{
	m_pManagerImpl = new CToolBarManagerImplementation( p_pIChartCtrl);
}


CToolBarManager::~CToolBarManager()
{
	if(m_pManagerImpl != NULL)
	{
		delete m_pManagerImpl;
		m_pManagerImpl = NULL;
	}
}

/*-----------------------------------------------------------------------------------------
 - Function    :  OnDraw
 - Created at  :  2005-08-16   19:00
 - Author      :  최종찬
 - Description :  툴바를 그린다.
 -----------------------------------------------------------------------------------------*/
void CToolBarManager::OnDraw()
{
	m_pManagerImpl->ReDraw();
}

/*-----------------------------------------------------------------------------------------
 - Function    :  CreateToolBar
 - Created at  :  2004-10-20   15:47
 - Author      :  최종찬
 - Description :  툴바를 생성시킨다.
 -----------------------------------------------------------------------------------------*/
bool CToolBarManager::CreateToolBar(const CString& strToolBarOption, 
									const CString& strImageFile)
{
	return m_pManagerImpl->CreateToolBar(strToolBarOption, strImageFile);
}


/*-----------------------------------------------------------------------------------------
 - Function    :  ToolBarPosChanged
 - Created at  :  2004-10-20   15:48
 - Author      :  최종찬
 - Description :  ToolBar의 위치를 변경시킨다.
 -----------------------------------------------------------------------------------------*/
void CToolBarManager::ToolBarPosChanged(const CRect rectToolBarPos)
{
	m_pManagerImpl->ToolBarPosChanged(rectToolBarPos);
}


/*-----------------------------------------------------------------------------------------
 - Function    :  GetToolBarRect
 - Created at  :  2004-10-21   09:33
 - Author      :  최종찬
 - Description :  툴바의 사이즈를 리턴한다. 
				(cf. 툴바에 국한되지 않고 툴바 위에 붙는 수치조회창(Single Line, Multi Line)의
				싸이즈까지 더하여 리턴한다. 즉! 툴바 DLL이 사용하는 사이즈 전부를 리턴한다.)
 -----------------------------------------------------------------------------------------*/
CRect CToolBarManager::GetToolBarRect() const
{
	return m_pManagerImpl->GetToolBarRect();
}

/*-----------------------------------------------------------------------------------------
 - Function    :  SetSliderMin
 - Created at  :  2004-10-29   15:27
 - Author      :  최종찬
 - Description :  Slider의 최소값을 세팅한다.
 -----------------------------------------------------------------------------------------*/
void CToolBarManager::SetSliderMin(const int nSliderMin)
{
	m_pManagerImpl->SetSliderMin(nSliderMin);
}


/*-----------------------------------------------------------------------------------------
 - Function    :  SetControlData
 - Created at  :  2004-11-01   09:41
 - Author      :  최종찬
 - Description :  데이타클래스를 세팅한다.
 -----------------------------------------------------------------------------------------*/
void CToolBarManager::SetControlData(CPacketList* pPacketList, CPacketListManager* pPacketListManager, CXScaleManager* pxScaleManager, CMainBlock* pMainBlock, CIndicatorList* pIndicatorList)
{
	m_pManagerImpl->SetControlData(pPacketList, pPacketListManager, pxScaleManager, pMainBlock, pIndicatorList);
}


/*-----------------------------------------------------------------------------------------
 - Function    :  SetGraphType
 - Created at  :  2004-11-01   16:31
 - Author      :  최종찬
 - Description :  그래프타입을 콤보에 세팅한다.
 -----------------------------------------------------------------------------------------*/
void CToolBarManager::SetGraphType(CGraphBaseData::GRAPHTYPE graphType, int nIndex)
{
	m_pManagerImpl->SetGraphType(graphType, nIndex);
}


/*-----------------------------------------------------------------------------------------
 - Function    :  SetLineThickness
 - Created at  :  2004-11-03   11:27
 - Author      :  최종찬
 - Description :  OCX에서 선택한 라인두께설정값에 따라 콤보에 해당 스타일 설정값으로 바꾼다.
 -----------------------------------------------------------------------------------------*/
void CToolBarManager::SetLineThickness(int nThickness)
{
	m_pManagerImpl->SetLineThickness(nThickness);
}


/*-----------------------------------------------------------------------------------------
 - Function    :  SetOneChart
 - Created at  :  2004-11-05   11:09
 - Author      :  최종찬
 - Description :  단일지표차트여부를 세팅한다.
 -----------------------------------------------------------------------------------------*/
void CToolBarManager::SetOneChart(bool bOneChart, bool p_bIsNoZoomChart)
{
	m_pManagerImpl->SetOneChart(bOneChart, p_bIsNoZoomChart);
}

/*-----------------------------------------------------------------------------------------
 - Function    :  GetStartEndIndex
 - Created at  :  2004-11-01   10:56
 - Author      :  최종찬
 - Description :  데이타의 시작인덱스와 끝인덱스를 가져온다.
 -----------------------------------------------------------------------------------------*/
bool CToolBarManager::GetStartEndIndex(int& nStartIndex, int& nEndIndex)
{
	return m_pManagerImpl->GetStartEndIndex(nStartIndex, nEndIndex);
}


/*-----------------------------------------------------------------------------------------
 - Function    :  GetMaxRange
 - Created at  :  2004-11-17   15:33
 - Author      :  최종찬
 - Description :  MaxRange를 가져온다.
 -----------------------------------------------------------------------------------------*/
int CToolBarManager::GetMaxRange()
{
	return m_pManagerImpl->GetMaxRange();
}


/*-----------------------------------------------------------------------------------------
 - Function    :  IsToolScrollBar
 - Created at  :  2005-02-28   10:33
 - Author      :  최종찬
 - Description :  스크롤바 사용유무를 리턴한다.
 -----------------------------------------------------------------------------------------*/
bool CToolBarManager::IsToolScrollBar()
{
	return m_pManagerImpl->IsToolScrollBar();
}

/*-----------------------------------------------------------------------------------------
 - Function    :  GetToolBarHwnd
 - Created at  :  2005-09-10
 - Author      :  최종찬
 - Description :  ToolBar Dialog의 윈도우 핸들을 리턴한다.
 -----------------------------------------------------------------------------------------*/
HWND CToolBarManager::GetToolBarHwnd()
{
	return m_pManagerImpl->GetToolBarHwnd();
}

/*-----------------------------------------------------------------------------------------
 - Function    :  ReleaseOtherButtonUI
 - Created at  :  2005-03-02   14:44
 - Author      :  최종찬
 - Description :  눌러져 있는 툴버튼을 풀어준다.
 -----------------------------------------------------------------------------------------*/
void CToolBarManager::ReleaseOtherButtonUI(CToolOptionInfo::TOOLOPTION enumToolOption)
{
	m_pManagerImpl->ReleaseOtherButtonUI(enumToolOption);
}


/*-------------------------------------------------------------------------------
 - Function    :  UseToolOption
 - Created at  :  2005-03-03   09:28
 - Author      :  최종찬   
 - Description :  툴바에 해당 툴아이템을 사용하는지 확인한다.
 -------------------------------------------------------------------------------*/
BOOL CToolBarManager::UseToolOption(CToolOptionInfo::TOOLOPTION enumToolOption)
{
	return m_pManagerImpl->UseToolOption(enumToolOption);
}


/*-------------------------------------------------------------------------------
 - Function    :  GetToolOption
 - Created at  :  2005-03-03   09:48
 - Author      :  최종찬   
 - Description :  툴바에 해당 툴아이템이 있는지 확인한다.
 -------------------------------------------------------------------------------*/
BOOL CToolBarManager::GetToolOption(CToolOptionInfo::TOOLOPTION enumToolOption)
{
	return m_pManagerImpl->GetToolOption(enumToolOption);
}

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2006/3/4
// Modifier		: 
// Comments		: Notify Cursor Mode Change for Drag Cross Line & Numercial Inquiry DLG
//-----------------------------------------------------------------------------
void CToolBarManager::OnNumericalInquiryOptionChanged( const long p_lNumericalInquiryOption)
{
	m_pManagerImpl->OnNumericalInquiryOptionChanged( p_lNumericalInquiryOption);
}

/*-------------------------------------------------------------------------------
 - Function    :  SetGraphColor
 - Created at  :  2005-03-22   13:38
 - Author      :  최종찬
 - Description :  그래프 색상을 설정한다.
 -------------------------------------------------------------------------------*/
void CToolBarManager::SetGraphColor(COLORREF clrGraphColor)
{
	m_pManagerImpl->SetGraphColor(clrGraphColor);
}

// (2006/11/15 - Seung-Won, Bae) Tool On/Off State Changed.
BOOL CToolBarManager::OnToolStateChanged( const CToolOptionInfo::TOOLOPTION p_eToolID, const BOOL p_bTurnOn)
{
	return  m_pManagerImpl->OnToolStateChanged( p_eToolID, p_bTurnOn);
}

// (2009/1/5 - Seung-Won, Bae) Update ScrollBar and Zoom SliderBar from XScaleManager by Real.
void CToolBarManager::UpdateZoomAndScrollInfo( int p_nFullRange, int p_nStartPos, int p_nViewCount, BOOL p_bWholeView)
{
	m_pManagerImpl->UpdateZoomAndScrollInfo( p_nFullRange, p_nStartPos, p_nViewCount, p_bWholeView);
}
