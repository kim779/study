// AutoScrollAddInImp.cpp: implementation of the CAutoScrollAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartCommonAddIn.h"
#include "AutoScrollAddInImp.h"

#include "BlockBaseEnum.h"										// for ERRORVALUE
#include "../Include_AddIn/_resource.h"							// for ID_ASA_AUTOSCROLL_TIMER
#include "../Include_AddIn/I000000/_IChartOCX.h"				// for IChartOCX

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAutoScrollAddInImp::CAutoScrollAddInImp(IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase)
	: CChartAddInBase( p_pIChartOCX, p_pIAddInDllBase)
{
	m_nCurrentMode = CToolOptionInfo::TOOLOPTION::T_ADDIN_AUTOSCROLL_STOP;
}

CAutoScrollAddInImp::~CAutoScrollAddInImp()
{
	// 소멸되지 않은 Timer를 소멸한다.
	m_pChartCWnd->KillTimer( ID_ASA_AUTOSCROLL_TIMER);
}


// ( 2006 / 11 / 21 - Sang-Woo, Cho )  Multiple Item in DLL
CString CAutoScrollAddInImp::m_strAddInItemName = "AUTO_SCROLL";


//////////////////////////////////////////////////////////////////////
// Define AddIn Event Map
//////////////////////////////////////////////////////////////////////
BEGIN_ADDIN_EVENT_MAP( CAutoScrollAddInImp)
	ONADDINEVENT( OnTimer)
	ONADDINEVENT( OnToolCommand)
END_ADDIN_EVENT_MAP()


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 11 / 21
// Return void	: 
// Param  const int p_nToolType : Button의 값
// Param  const int p_nCmdOption : 실행유무 ( 0 : 실행하지 않음.	1 : 실행 )
// Comments		: 자동 스크롤 버튼이 눌러지면 해당되는 동작을 실행한다.
//-----------------------------------------------------------------------------
void CAutoScrollAddInImp::OnToolCommand( const int p_nToolType, const int p_nCmdOption )
{
	// (2007/1/29 - Seung-Won, Bae) Re-Notify the State to Tool UI
	if( CToolOptionInfo::T_ALL_TOOL_TRUE_QUERY == p_nToolType)
	{
		m_pIChartOCX->OnToolStateChanged( CToolOptionInfo::T_ADDIN_AUTOSCROLL_PLAY_LEFT, m_nCurrentMode == CToolOptionInfo::T_ADDIN_AUTOSCROLL_PLAY_LEFT);
		m_pIChartOCX->OnToolStateChanged( CToolOptionInfo::T_ADDIN_AUTOSCROLL_PLAY_RIGHT, m_nCurrentMode == CToolOptionInfo::T_ADDIN_AUTOSCROLL_PLAY_RIGHT);
		return;
	}

	// 1. CmdOption이 1일 경우에만 처리한다.
	if( p_nCmdOption == 0 ) return;

	if( p_nToolType < CToolOptionInfo::T_ADDIN_AUTOSCROLL_PLAY_LEFT || CToolOptionInfo::T_ADDIN_AUTOSCROLL_STOP < p_nToolType)
		return;

	// 2. 현재 실행 Type을 저장한다.
	m_nCurrentMode = p_nToolType;

	// 3. 실행 Type에 맞는 작업을 한다.
	switch( m_nCurrentMode)
	{
		// Stop Button을 눌렀을 때 
		case CToolOptionInfo::TOOLOPTION::T_ADDIN_AUTOSCROLL_STOP :
		{
			m_pChartCWnd->KillTimer( ID_ASA_AUTOSCROLL_TIMER);
			// (2006/11/25 - Seung-Won, Bae) Reset UI
			m_pIChartOCX->OnToolStateChanged( CToolOptionInfo::T_ADDIN_AUTOSCROLL_PLAY_LEFT, FALSE);
			m_pIChartOCX->OnToolStateChanged( CToolOptionInfo::T_ADDIN_AUTOSCROLL_PLAY_RIGHT, FALSE);
			break;
		}
		
		// Left Scroll, Right Scroll Button을 눌렀을 때
		case CToolOptionInfo::TOOLOPTION::T_ADDIN_AUTOSCROLL_PLAY_LEFT  :				// Run Left
		case CToolOptionInfo::TOOLOPTION::T_ADDIN_AUTOSCROLL_PLAY_RIGHT :				// Run Right
		{
			// 자동 스크롤 속조 조절 - ojtaso (20080425)
			UINT nTime = (UINT)(1000. / p_nCmdOption);
			m_pChartCWnd->KillTimer( ID_ASA_AUTOSCROLL_TIMER);
			m_pChartCWnd->SetTimer( ID_ASA_AUTOSCROLL_TIMER, nTime, NULL );
			// (2006/11/25 - Seung-Won, Bae) Reset UI
			m_pIChartOCX->OnToolStateChanged( CToolOptionInfo::T_ADDIN_AUTOSCROLL_PLAY_LEFT, m_nCurrentMode == CToolOptionInfo::T_ADDIN_AUTOSCROLL_PLAY_LEFT);
			m_pIChartOCX->OnToolStateChanged( CToolOptionInfo::T_ADDIN_AUTOSCROLL_PLAY_RIGHT, m_nCurrentMode == CToolOptionInfo::T_ADDIN_AUTOSCROLL_PLAY_RIGHT);
			break;
		}
	}
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 11 / 21
// Return void	: 
// Param  UINT p_uTimerID : Timer ID가 ID_ASA_AUTOSCROLL_TIMER일때만 동작한다.
// Comments		: 
//-----------------------------------------------------------------------------
void CAutoScrollAddInImp::OnTimer( UINT p_uTimerID )
{
	// 1. Timer의 ID가 ID_ASA_AUTOSCROLL_TIMER인 경우에만 처리한다.
	if( p_uTimerID != ID_ASA_AUTOSCROLL_TIMER)
		return;

	int nStartIndex, nEndIndex;

	// 2. Scroll Left일 경우에는 Left로 1봉씩 이동한다.
	if( m_nCurrentMode == CToolOptionInfo::TOOLOPTION::T_ADDIN_AUTOSCROLL_PLAY_LEFT )
		m_pIChartOCX->ScrollTo( -1, TRUE );

	// 3. Scroll Right일 경우에는 Right로 1봉씩 이동한다.
	else if( m_nCurrentMode == CToolOptionInfo::TOOLOPTION::T_ADDIN_AUTOSCROLL_PLAY_RIGHT )
		m_pIChartOCX->ScrollTo( 1, TRUE );

	else
		return;
	
	// 4. Scroll을 정상적으로 하고 난 뒤 Timer를 계속할지 종료할지를 판단한다.
	m_pIChartOCX->GetViewStartEndIndex( nStartIndex, nEndIndex );

	int nMaxRange;
	m_pIChartOCX->GetMaxScrollRange( nMaxRange );

	if( nStartIndex == 0 || nMaxRange <= nEndIndex )
		OnToolCommand( CToolOptionInfo::TOOLOPTION::T_ADDIN_AUTOSCROLL_STOP, 1);
}