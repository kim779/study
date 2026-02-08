// KeyboardScrollAddInImp.cpp: implementation of the CKeyboardScrollAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartCommonAddIn.h"
#include "KeyboardScrollAddInImp.h"

#include "../Include_AddIn/I000000/_IChartOCX.h"				// for IChartOCX

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKeyboardScrollAddInImp::CKeyboardScrollAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase)
	: CChartAddInBase( p_pIChartOCX, p_pIAddInDllBase)
{
}

CKeyboardScrollAddInImp::~CKeyboardScrollAddInImp()
{

}

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
CString CKeyboardScrollAddInImp::m_strAddInItemName = "KBD_SCROLL";

//////////////////////////////////////////////////////////////////////
// Define AddIn Event Map
//////////////////////////////////////////////////////////////////////
BEGIN_ADDIN_EVENT_MAP( CKeyboardScrollAddInImp)
	ONADDINEVENT( OnKeyDown)
	ONADDINEVENT( OnKeyUp)
END_ADDIN_EVENT_MAP()


//////////////////////////////////////////////////////////////////////
// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
//////////////////////////////////////////////////////////////////////
BOOL CKeyboardScrollAddInImp::OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// (2004.11.09, 배승원) Keyboard에 의한 ZoomIn/Out 및 Scrolling을 처리한다.
	// 1. Handling을 하지 않을 KeyDown을 처리한다.
	if( nChar != VK_LEFT && nChar != VK_RIGHT && nChar != VK_UP && nChar != VK_DOWN) return FALSE;
	// 2. 먼저 Shift Key를 확인한다.
	int nWithShift = 0;
	if( GetKeyState( VK_SHIFT) & 0x8000) nWithShift = 1;	// Shift Key Down Checking
	// 3. Key Code에 따라 Scroll/Zoom을 처리한다.
	// (2005.01.20, 배승원) VK_LEFT/VK_RIGHT는 ScrollBar가 VK_UP/VK_DOWN은 SlideBar가 있는지 확인하여 처리한다.
	if( VK_LEFT == nChar || VK_RIGHT == nChar)
	{
		// 3.1 좌로 1 이동한다. (Shift Key 연동시 5이동한다.)
		if( VK_LEFT == nChar)
		{
			if( !m_pIChartOCX->ScrollTo( ( 1 + nWithShift * ( 5 - 1)) * ( -1), TRUE)) return FALSE;
		}
		// 3.2 우로 1 이동한다. (Shift Key 연동시 5이동한다.)
		if( VK_RIGHT == nChar)
		{
			if( !m_pIChartOCX->ScrollTo( ( 1 + nWithShift * ( 5 - 1)) * ( +1), TRUE)) return FALSE;
		}
	}
	if( VK_UP == nChar || VK_DOWN == nChar)
	{
		// 3.3 화면의 봉수를 1 줄인다. (Shift Key 연동시 10을 줄인다.)
		if( VK_UP == nChar)
		{
			if( !m_pIChartOCX->ZoomTo( ( 1 + nWithShift * ( 10 - 1)) * ( -1), TRUE)) return FALSE;
		}
		// 3.3 화면의 봉수를 1 늘인다. (Shift Key 연동시 10을 늘인다.)
		if( VK_DOWN == nChar)
		{
			if( !m_pIChartOCX->ZoomTo( ( 1 + nWithShift * ( 10 - 1)) * ( +1), TRUE)) return FALSE;
		}
		// (2006/11/18 - Seung-Won, Bae) Notify the User Zoom State to MultiChart
		int nStartIndex = 0, nEndIndex = 0;
		m_pIChartOCX->GetViewStartEndIndex( nStartIndex, nEndIndex);
		m_pIChartOCX->OnUserZoomChanged( nEndIndex - nStartIndex + 1);
	}

	return TRUE;
}

BOOL CKeyboardScrollAddInImp::OnKeyUp( UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// (2005.01.13, 배승원) 전체보기 Mode로 즉시 전환하는 Key UI를 제공한다.
	// 1. Page Up / Down Key Input을 제외하고는 모두 통과시킨다.
	if( nChar != VK_PRIOR && nChar != VK_NEXT) return FALSE;
	// (2005.01.20, 배승원) VK_PRIOR/VK_NEXT는 SlideBar가 있는지 확인하여 처리한다.
	if( !m_pIChartOCX->HasToolItem( CToolOptionInfo::T_DISPLAY_SLIDERBAR)) return FALSE;
	// 2. 현재의 전체보기 여부에 따라 설정처리한다.
	//		VK_PRIOR	: 전체보기 설정
	//		VK_NEXT		: 전체보기 해제
	if( !m_pIChartOCX) return FALSE;
	if(			m_pIChartOCX->CheckFullView()	&& nChar == VK_NEXT)	m_pIChartOCX->SetFullView( FALSE);
	else if(	!m_pIChartOCX->CheckFullView()	&& nChar == VK_PRIOR)	m_pIChartOCX->SetFullView( TRUE);
	else return FALSE;

	return TRUE;
}
