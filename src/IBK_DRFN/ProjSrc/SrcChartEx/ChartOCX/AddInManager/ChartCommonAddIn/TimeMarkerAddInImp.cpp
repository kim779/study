// TimeMarkerAddInImp.cpp: implementation of the CTimeMarkerAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartCommonAddIn.h"
#include "TimeMarkerAddInImp.h"

#include <Mmsystem.h>									// for timeGetTime

#include "../../include/SharedBase.h"					// for DE_SHARDSET
#include "../../Include_Chart/Dll_Load/IMetaTable.h"	// for _MTEXT()
#include "../Include_AddIn/AddInCommand.h"				// for EAI_CUSTOM_CLEAR_TIMEMARK
#include "../Include_Addin/I000000/_IPacketManager.h"	// for IPacketManager
#include "../Include_AddIn/I000000/_IPacket.h"			// for IPacket
#include "../Include_Addin/I000000/_IChartManager.h"	// for IChartManager
#include "../Include_AddIn/I000000/_IBlock.h"			// for IBlock
#include "../Include_AddIn/I000000/_IChartOCX.h"		// for IChartOCX
#include "../Include_AddIn/I000000/_IString.h"			// for ILPCSTR

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTimeMarkerAddInImp::CTimeMarkerAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase)
	: CChartAddInBase( p_pIChartOCX, p_pIAddInDllBase)
{
	// 1. 현재 Mark 처리중인 일시값을 관리한다.
	m_lMarkTime = 0;

	// (2004.09.14, 배승원) 녹색 팬만을 관리한다.
	m_pnPen.CreatePen(		PS_SOLID,	3, RGB( 0, 255, 0));
	// (2004.09.30, 배승원) 시간공유의 수신을 허용하는 Flag을 두어 제한할 수 있도록 한다.
	//		발신은 제한하지 않는다.
	m_bEnableReceivingTimeShare = TRUE;

	// (2006/11/16 - Seung-Won, Bae) Manage the Disable Flag of TimeMark
	m_bEnableFiringTimeShare = TRUE;

	// (2007/1/17 - Seung-Won, Bae) Manage Double Click Time Interval for LBtnDblClick with force.
	m_uDoubleClickTime = GetDoubleClickTime();
}

CTimeMarkerAddInImp::~CTimeMarkerAddInImp()
{
	// (2004.09.14, 배승원) 녹색 팬만을 관리한다.
	m_pnPen.DeleteObject();
}

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
CString CTimeMarkerAddInImp::m_strAddInItemName = "TIME_MARKER";

//////////////////////////////////////////////////////////////////////
// Define AddIn Event Map
//////////////////////////////////////////////////////////////////////
BEGIN_ADDIN_EVENT_MAP( CTimeMarkerAddInImp)
	ONADDINEVENT( OnGSharedDataChange)
	ONADDINEVENT( OnResetChart)
	ONADDINEVENT( OnDrawBeforeGraph)
	ONADDINEVENT( OnToolCommand)
	ONADDINEVENT( OnAddInToolCommand_EAICommandType)
	ONADDINEVENT( OnAddInToolCommand_EAICommandType_char_long)
	ONADDINEVENT( OnAddInToolCommand_EAICommandType_long)
	ONADDINEVENT( OnLButtonDown)
	ONADDINEVENT( OnLButtonDblClk)
END_ADDIN_EVENT_MAP()

//////////////////////////////////////////////////////////////////////
// (2004.11.22, 배승원, 정리) 시간공유를 전달받아, 시간공유표시를 출력하는 기능을 제공한다.
//////////////////////////////////////////////////////////////////////

// (2004.06.10, 배승원) 시간 공유를 받아서 TimeMark를 설정한다.	// YYYYMMDD or HHMMSS or YYYYMMDDHHMMSS
void CTimeMarkerAddInImp::SetTimeMarkString( const char *p_szMarkTime, BOOL p_bWithScroll)
{
	// (2006/2/13 - Seung-Won, Bae) Check Empty Chart!
	if( m_pIChartManager->IsEmpty()) return;

	// (2004.11.19, 배승원) 이미 설정된 시간공유가 같은 시간일 경우 해제하는 기능으로 처리한다.
	//		특히, Scroll도 진행되지 않도록 처리한다.
	//		또한, 이전 TimeMark 값과 틀리다면 새로 Drawing을 하도록 한다.
	//		이런 기능을 위하여 이전 TimeMark Value를 BackUp해 놓는다.

	// 2011.01.06 by SYS >> 분차트 YYYY(년) 추가
	//long lPreMarkTime = m_lMarkTime;
	__int64 lPreMarkTime = m_lMarkTime;
	// 2011.01.06 by SYS <<

	// 1.1.1 공유값의 오류의 경우를 대비하여 TimeMarkerAddInImp를 Clear시킨다.
	//	(무시해야 할 공유값에 의해 초기화되지 않도록 한다.)
	ClearTimeMark();

	// 5.3 TimeMark를 설정한다.	// YYYYMMDD or HHMMSS or YYYYMMDDHHMMSS
	int nTimeIndex = 0;
	m_lMarkTime = GetTimeValue( p_szMarkTime, nTimeIndex);

	if( m_lMarkTime)
	{
		// 6. Mark위치로 Scroll 처리한다.
		// (2004.07.05, 배승원) Time Mark가 현재 화면에 보일 수 있는 상태이면 Scroll 하지 않는다.
		// 6.0.1 현 Data View의 Start/End Index를 구한다.
		int nStartIndex, nEndIndex;
		if( m_pIChartOCX->GetViewStartEndIndex( nStartIndex, nEndIndex))
			// 6.0.2 현재의 View 안쪽인 경우 Scroll을 처리하지 않는다.
			if( nTimeIndex < nStartIndex || nEndIndex < nTimeIndex)
				// 6.1 Scrolling을 처리한다.
				// (2004.11.22, 배승원) Start Index로의 Scroll을 별도의 Interface를 통하여 처리토록 한다.
				if( p_bWithScroll) m_pIChartOCX->ScrollTo( nTimeIndex - ( nEndIndex - nStartIndex + 1) / 2, FALSE);
	}

	// (2004.09.14, 배승원) Time Mark가 설정된 경우, Repaint 되도록 설정한다.
	//		주의! 이 함수 끝에 호출하지 않는 이유는? (중간에 Return이 있으니까...)
	// (2004.11.19, 배승원) 이전 TimeMark 값과 틀리다면 새로 Drawing을 하도록 한다.
	if( lPreMarkTime != m_lMarkTime) m_pIChartOCX->InvalidateControl();
}

//////////////////////////////////////////////////////////////////////
// (2004.11.22, 배승원, 정리) 시간공유를 발생시키는 Interface와 Clear 시키는 Interface를 제공한다.
//////////////////////////////////////////////////////////////////////

// (2004.06.11, 배승원) Block의 Double Click시의 시간 공유를 발생시키면서 TimeMarkerAddInImp를 설정하는 Interface를 제공한다.
void CTimeMarkerAddInImp::SetTimeMarkPoint( CPoint p_ptPoint)
{
	// (2006/2/13 - Seung-Won, Bae) Check Empty Chart!
	if( m_pIChartManager->IsEmpty()) return;
	// 4.1 시간공유의 발생설정이 되어 있는지 확인한다.
	if( !m_pIChartOCX->CheckGSharedSetting( _MTEXT( C6_TIME_CHART), FALSE, FALSE, TRUE)) return;

	// (2004.11.22, 배승원) 시간 공유 값을 산출하는 Routine을 제공하여, 사전에 시간 공유값을 확인하는 기능을 제공한다.
	CString strMarkTime;
	if( !GetMarkTimeInString( p_ptPoint, strMarkTime)) return;

	// 5. 자체 Chart에도 TimeMark를 표시하도록 처리한다.
	// (2004.09.14, 배승원) 내부적으로 OCX를 Invalidate 시키도록 한다.
	SetTimeMarkString( strMarkTime, FALSE);

	// (2006/3/4 - Seung-Won, Bae) Support "NN" Format
	if( strMarkTime.GetLength() == 16) strMarkTime = strMarkTime.Left( 14);

	// 4. 구해진 시간 정보를 공유로 발생시킨다.
	// (2004.11.19, 배승원) 이미 설정된 시간공유가 같은 시간일 경우 해제하는 기능으로 처리한다.
	//		특히, 해제된(또는 설정에 실패한) 경우에는 공유를 발생시키지 않는다.
	if( m_lMarkTime) m_pIChartOCX->FireGSharedData( _MTEXT( C6_TIME_CHART), strMarkTime);
}

//////////////////////////////////////////////////////////////////////
// (2004.11.22, 배승원) Mouse Click을 통한 시간공유 출력 해제 기능을 지원한다.
//////////////////////////////////////////////////////////////////////

// (2004.11.22, 배승원) 시간 공유 값을 산출하는 Routine을 제공하여, 사전에 시간 공유값을 확인하는 기능을 제공한다.
BOOL CTimeMarkerAddInImp::GetMarkTimeInString( CPoint p_ptPoint, CString &p_strMarkTime)
{
	// 1. 먼저 Block으로부터 선택된 Data Index를 구한다.
	// 1.1 주어진 Point에 해당하는 Block의 Graph Region을 구한다.
	// (2004.06.14, 배승원) Block 영역의 Height에, Graph의 Width 영역을 Input 영역으로 확인한다.
	// (2006/6/16 - Seung-Won, Bae) Use Block Interface
	int nRow, nColumn;
	if( !m_pIChartManager->GetBlockIndex( p_ptPoint, nRow, nColumn)) return FALSE;
	IBlock *pIBlock = m_pIChartManager->GetBlock( nRow, nColumn);
	if( !pIBlock) return FALSE;
	CRect rctBlockRegion, rctNoFrameAndMargin;
	pIBlock->GetGraphRegions( rctBlockRegion, rctNoFrameAndMargin);
	CRect rctGraphRegion = pIBlock->GetGraphDrwingRegion();
	pIBlock->Release();
	pIBlock = NULL;
	rctBlockRegion.left = rctGraphRegion.left;
	rctBlockRegion.right = rctGraphRegion.right;
	if( !rctBlockRegion.PtInRect( p_ptPoint)) return FALSE;
	// 1.2 현재 화면에 보이는 Data의 Start/End Index를 구한다.
	int nStartIndex, nEndIndex;
	if( !m_pIChartOCX->GetViewStartEndIndex( nStartIndex, nEndIndex)) return FALSE;
	// 1.3 지정된 Mouse Point의 Data Index를 구한다.
	//{{ 2007.03.26 by LYH XScale 적용
	//int nDataIndex = nStartIndex + ( ( p_ptPoint.x - rctBlockRegion.left) * ( nEndIndex - nStartIndex + 1)) / rctBlockRegion.Width();
	int nDataIndex = m_pIChartManager->FindViewDataIndex_Pt("DEFAULT", p_ptPoint.x);
	//}}
	// 2. 해당 Data Index의 시간 정보를 구한다.
	// 2.1 자료일자 Packet을 구한다.
	IPacket *pIPacket = m_pIDefaultPacketManager->GetPacket( _MTEXT( C0_DATE_TIME));
	if( !pIPacket) return FALSE;
	try
	{
		// 2.2 Data를 구한다.
		// (2004.07.02, 배승원) 시사영어와 충돌나는 Logic을 개선한다.
		// (2004.09.24, 배승원) ScrollBar도 Zero Base로 변경되어 1를 줄이지 않는다.
		double dMarkTime;
		if( !pIPacket->GetData( nDataIndex, dMarkTime)) throw this;
		long lMarkTime = ( long)dMarkTime;

		// 3. 시간 정보를 "YYYYMMDDHHMMSS"의 Full Data로 맞춘다.
		// 3.1 먼저 Packet Type을 구한다.
		ILPCSTR szPacketType = pIPacket->GetType();
		CString strPacketType( szPacketType);

		// 3.2 Start Index를 구한다.
		int nStart;
		// 3.2.1 YYYY로 시작하는 Type의 경우를 처리한다.
		if( strPacketType.Left( 4) == "YYYY")	nStart = 0;
		// 3.2.2 MMDD로 시작하는 Type의 경우를 처리한다. (MM만 있는 경우 분의 MM과 혼동되어 지워하지 않는다.)
		else if( strPacketType.Left( 4) == "MMDD")	nStart = 4;
		// (2005/9/9 - Seung-Won, Bae) Process the case of Starting with 'DD'
		else if( strPacketType.Left( 2) == "DD")	nStart = 6;
		// 3.2.3 HH로 시작하는 Type의 경우를 처리한다.
		else if( strPacketType.Left( 2) == "HH")	nStart = 8;
		// 3.2.4 MMSS로 시작하는 Type의 경우를 처리한다.
		else if( strPacketType.Left( 2) == "MM")	nStart = 10;
		else throw this;
		// 3.3 Packet Type에 준하여, Mark Time을 조정한다.
		// 3.3.0 먼저 현재 일시을 구한다.
		SYSTEMTIME stTime;
		GetSystemTime( &stTime);
		// 3.3.1 시간을 설정할 필요가 있는지 확인한다.
		// (2006/3/4 - Seung-Won, Bae) Support "NN" Format
		char *szMarkTime = p_strMarkTime.GetBufferSetLength( 16);
		strcpy( szMarkTime, "                ");
		int nEnd = nStart + strPacketType.GetLength();
		if( 8 < nEnd)
		{
			// (2006/3/4 - Seung-Won, Bae) Support "NN" Format
			int nNSecond;
			if( nEnd == 16)
			{
				nNSecond = lMarkTime % 100;
				lMarkTime /= 100;
			}
			else nNSecond = stTime.wMilliseconds / 10;
			szMarkTime[ 14] = '0' + nNSecond / 10;
			szMarkTime[ 15] = '0' + nNSecond % 10;

			// 3.3.2 먼저 초를 등록한다.
			int nSecond;
			if( nStart <= 12 && 14 <= nEnd)
			{
				nSecond = lMarkTime % 100;
				lMarkTime /= 100;
			}
			else nSecond = stTime.wSecond;
			szMarkTime[ 12] = '0' + nSecond / 10;
			szMarkTime[ 13] = '0' + nSecond % 10;
			// 3.3.3 분을 설정한다.
			int nMinute;
			if( nStart <= 10 && 12 <= nEnd)
			{
				nMinute = lMarkTime % 100;
				lMarkTime /= 100;
			}
			else nMinute = stTime.wMinute;
			szMarkTime[ 10] = '0' + nMinute / 10;
			szMarkTime[ 11] = '0' + nMinute % 10;
			// 3.3.4 시간을 설정한다.
			int nHour;
			if( nStart <= 8 && 10 <= nEnd)
			{
				nHour = lMarkTime % 100;
				lMarkTime /= 100;
			}
			else nHour = stTime.wHour;
			szMarkTime[ 8] = '0' + nHour / 10;
			szMarkTime[ 9] = '0' + nHour % 10;
		}
		// 3.3.5 일자 설정이 필요한지 확인한다.
		if( nStart < 8)
		{
			// 3.3.6 일를 등록한다.
			int nDay;
			if( nStart <= 6 && 8 <= nEnd)
			{
				nDay = lMarkTime % 100;
				lMarkTime /= 100;
			}
			else nDay = stTime.wDay;
			szMarkTime[ 6] = '0' + nDay / 10;
			szMarkTime[ 7] = '0' + nDay % 10;
			// 3.3.7 월을 등록한다.
			int nMonth;
			if( nStart <= 4 && 6 <= nEnd)
			{
				nMonth = lMarkTime % 100;
				lMarkTime /= 100;
			}
			else nMonth = stTime.wMonth;
			szMarkTime[ 4] = '0' + nMonth / 10;
			szMarkTime[ 5] = '0' + nMonth % 10;
			// 3.3.8 년을 등록한다.
			int nYear;
			if( nStart == 0) nYear = lMarkTime;
			else nYear = stTime.wYear;
			szMarkTime[ 3] = '0' + nYear % 10;
			szMarkTime[ 2] = '0' + ( ( nYear / 10) % 10);
			szMarkTime[ 1] = '0' + ( ( nYear / 100) % 10);
			szMarkTime[ 0] = '0' + nYear / 1000;
		}
	}
	catch( CTimeMarkerAddInImp *)
	{
		pIPacket->Release();
		return FALSE;
	}
	pIPacket->Release();
	return TRUE;
}

// (2004.11.22, 배승원) 시간 공유 값을 현차트의 시간축에 맞춰 Data를 변환하는 Interface를 제공한다.
//		오류시 0을 Return한다.	// YYYYMMDD or HHMMSS or YYYYMMDDHHMMSS  or YYYYMMDDHHMMSSNN
// 2011.01.06 by SYS >> 분차트 YYYY(년) 추가 <<
//long CTimeMarkerAddInImp::GetTimeValue( const char *p_szMarkTime, int &p_nTimeIndex)
__int64 CTimeMarkerAddInImp::GetTimeValue( const char *p_szMarkTime, int &p_nTimeIndex)
{
	// 0. Return Value를 초기화한다.
	// 2011.01.06 by SYS >> 분차트 YYYY(년) 추가
	//long lMarkTime = 0;
	__int64 lMarkTime = 0;
	// 2011.01.06 by SYS <<

	// 1. 먼저 정확한 Data인지를 확인한다.
	// 1.1 Data Length가 6/8/14 중에 한 가지인지 확인한다.
	CString strMarkTime( p_szMarkTime);

	// (2006/3/4 - Seung-Won, Bae) Support "NN" Format (16 Byte)
	BOOL bNNData = ( strMarkTime.GetLength() == 16 && strMarkTime.Right( 2) != "  ");

	strMarkTime.TrimLeft();
	strMarkTime.TrimRight();
	int nLength = strMarkTime.GetLength();
	// (2006/3/4 - Seung-Won, Bae) Support "NN" Format (16 Byte)
	if( nLength != 6 && nLength != 8 && nLength != 14 && nLength != 16) return 0;

	// 1.2 현재 자료일시 단위와 무관한 경우를 제외한다.
	// 1.2.1 현재 Chart의 자료일자 Data Format을 구한다.
	IPacket *pIPacket = m_pIDefaultPacketManager->GetPacket( _MTEXT( C0_DATE_TIME));
	if( !pIPacket) return 0;
	try
	{
		ILPCSTR szPacketType = pIPacket->GetType();
		CString strPacketType( szPacketType);

		// 1.2.2 참조 Data Start Index를 구한다.
		int nStart;
		// 1.2.2.1 YYYY로 시작하는 Type의 경우를 처리한다.
		if( strPacketType.Left( 4) == "YYYY")	nStart = 0;
		// 1.2.2.2 MMDD로 시작하는 Type의 경우를 처리한다. (MM만 있는 경우 분의 MM과 혼동되어 지워하지 않는다.)
		else if( strPacketType.Left( 4) == "MMDD")	nStart = 4;
		// (2005/9/9 - Seung-Won, Bae) Process the case of Starting with 'DD'
		else if( strPacketType.Left( 2) == "DD")	nStart = 6;
		// 1.2.2.3 HH로 시작하는 Type의 경우를 처리한다.
		else if( strPacketType.Left( 2) == "HH")	nStart = 8;
		// 1.2.2.4 MMSS로 시작하는 Type의 경우를 처리한다.
		else if( strPacketType.Left( 2) == "MM")	nStart = 10;
		else throw this;
		// 1.2.3 현재 자료일시와 비교하여 무관한 경우를 제외한다.
		// 1.2.3.1 공유는 일자이면서, 차트는 시간인 경우를 제외한다.
		// (2006/3/4 - Seung-Won, Bae) Support "NN" Format (Can be 8 Byte)
		if( ( nLength == 8 && !bNNData) && 8 <= nStart) throw this;
		// 1.2.3.2 공유는 시간이면서, 차트는 일자인 경우를 제외한다.
		if( nLength == 6 && nStart + strPacketType.GetLength() <= 8) throw this;

		// 2. 수신된 Data의 Type에 따라, 표준의 Data Format으로 조정한다.
		// (2006/3/4 - Seung-Won, Bae) Support "NN" Format - Nomalize to YYYYMMDDHHmmSSNN
		if( nLength != 16)
		{
			// 2.1 현재 시각와 일자를 구한다.
			SYSTEMTIME st;
			GetLocalTime( &st);

			// 2.2 시간 공유 Data를 보완한다.
			CString strOldValue( strMarkTime);
			// (2006/3/4 - Seung-Won, Bae) Support "NN" Format (Can be 8 Byte)
			switch( nLength)
			{
				case 6:		strMarkTime.Format( "%04d%02d%02d%s%02d", st.wYear, st.wMonth, st.wDay, strOldValue, st.wMilliseconds);
							break;
				case 8:		if( bNNData)	strMarkTime.Format( "%04d%02d%02d%s", st.wYear, st.wMonth, st.wDay, strOldValue);
							else			strMarkTime.Format( "%s%02d%02d%02d%02d", strOldValue, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds / 10);
							break;
				case 14:	strMarkTime.Format( "%s%02d", strOldValue, st.wMilliseconds);
							break;
			}
		}

		// 4. Packet Data Type에 맞춰, GShare Data를 조정한다.
		// 4.2 Packet Type에 준하여, GShare Data를 조정한다.
		strMarkTime = strMarkTime.Mid( nStart, strPacketType.GetLength());

		// 5. TimeMarkerAddInImp Object에 Marking 일시를 설정한다.
		// 2011.01.06 by SYS >> 분차트 YYYY(년) 추가
		//lMarkTime = atol( strMarkTime);
		lMarkTime = _atoi64( strMarkTime);
		// 2011.01.06 by SYS <<


		// (2004.07.02, 배승원) 우측여백이 있으면서, 마지막 Data이후의 시간연동일 경우,
		//		이를 우측여백에의한 허봉 Scale에 적절히 출력하지 못하므로 생략한다.
		// 5.1 Mark Time의 Data Index를 구한다.
		// 5.1.2 Time Mark의 위치를 확인한다.
		// (2004.06.18, 배승원) 정확한 Data의 Index가 구해지지 않을 경우 그 이전 Data Index를 구한다.
		double dSelectedDate;
		if( !pIPacket->GetCurOrPrevIndexFromData( lMarkTime, p_nTimeIndex, dSelectedDate)) throw this;
		// 5.2 TimeMark가 마지막 Data이후의 Index를 갖는지 확인한다.
		// 5.2.1 TimeMark가 마지막 봉에 출력하여야 하는 경우인지 확인한다.
		int nLastDataIndex = m_pIChartOCX->GetLastRealDataIndex();
		if( nLastDataIndex == p_nTimeIndex)
		{
			// 5.2.2 근사 Index인지 확인한다.
			if( lMarkTime != ( long)dSelectedDate)
			{
				// 5.2.3 우측 여백이 있는 경우에는 Skip한다.
				int nScrollRange;
				if( !m_pIChartOCX->GetMaxScrollRange( nScrollRange)) throw this;
				if( nLastDataIndex < nScrollRange) throw this;
			}
		}

		// (2004.07.05, 배승원) Time Mark의 값이 다른 종목에서도 유효하도록, 
		//		Packet의 Type을 관리하여, 변경된 경우가 아니면 계속 Draw하도록 한다.
		m_strPacketType = strPacketType;
	}
	catch( CTimeMarkerAddInImp *)
	{
		pIPacket->Release();
		return 0;
	}
	pIPacket->Release();

	return lMarkTime;
}

// (2004.11.22, 배승원) Mouse 위치에 시간공유표시가 설정된 경우, 이를 최소시킨다.
BOOL CTimeMarkerAddInImp::ClearTimeMark( CPoint p_ptPoint)
{
	// 0. 먼저 설정된 시간 공유 표시가 있는지 확인한다.
	if( !m_lMarkTime) return FALSE;

	// 1. 먼저 Mouse 위치의 시간공유값을 산출한다.
	// 1.1 시간 공유 String을 구한다.
	CString strMarkTime;
	if( !GetMarkTimeInString( p_ptPoint, strMarkTime)) return FALSE;
	// 1.2 시간 공유 값을 구한다.
	int nTimeIndex = 0;
	// 2011.01.06 by SYS >> 분차트 YYYY(년) 추가
	//long lPointTime = GetTimeValue( strMarkTime, nTimeIndex);
	__int64 lPointTime = GetTimeValue( strMarkTime, nTimeIndex);
	// 2011.01.06 by SYS <<

	if( !lPointTime) return FALSE;

	// 2. 구해진 시간공유 값이 이미 설정된 시간공유값과 같으면,
	//		Clear시키고 종료한다.
	// 2.1 시간공유 위치인지 확인한다.
	// 2.1.1 이때 설정된 시간공유 시간도 오차시간일 수 있으므로 (해당시간에 봉이 없어서)
	//		해당 위치의 정확한 시간을 구하여 확인한다.
	long lMarkTime = 0;
	IPacket *pIPacket = NULL;
	pIPacket = m_pIDefaultPacketManager->GetPacket( _MTEXT( C0_DATE_TIME));
	if( pIPacket)
	{
		double dSelectedDate;
		if( !pIPacket->GetCurOrPrevIndexFromData( m_lMarkTime, nTimeIndex, dSelectedDate)) dSelectedDate = 0;
		pIPacket->Release();
		lMarkTime = ( long)dSelectedDate;
	}
	if( !lMarkTime) return FALSE;
	// 2.1.2 구해진 시간공유 시간과 같은지 확인한다.
	if( lPointTime != lMarkTime) return FALSE;
	// 2.2 설정된 시간공유값을 Clear 시킨다.
	ClearTimeMark();
	m_pIChartOCX->InvalidateControl();
	return TRUE;
}

// (2004.09.30, 배승원) 시간공유의 수신을 허용하는 Flag을 두어 제한할 수 있도록 한다.
//		발신은 제한하지 않는다.
void CTimeMarkerAddInImp::OnEnableReceivingTimeShare (BOOL p_bEnable)
{
	m_bEnableReceivingTimeShare = p_bEnable;
	// (2004.09.30, 배승원) 시간공유의 수신이 제한되는 시점에서는 기존의 시간공유선을 Clear시킨다.
	if( !m_bEnableReceivingTimeShare)
	{
		ClearTimeMark();
		// (2004.12.13, 배승원) NULL Pointer를 확인한다.
		m_pIChartOCX->InvalidateControl();
	}
}

// (2004.10.11, 배승원) TimeMark를 설정하는 Interface를 제공한다.
void CTimeMarkerAddInImp::OnSetTimeMark( const char *p_szMarkTime, BOOL p_bWithScroll)
{
	// 1. Null String인 경우에는 TimeMark을 Clear시킨다.
	if( !p_szMarkTime || !p_szMarkTime[ 0])
	{
		ClearTimeMark();
		return;
	}

	// 2. TimeMark String을 전달하여 설정을 시도한다. (내부에서 유효성도 검사한다.)
	// (2004.09.14, 배승원) 내부적으로 OCX를 Invalidate 시키도록 한다.
	SetTimeMarkString( p_szMarkTime, p_bWithScroll);
}

//////////////////////////////////////////////////////////////////////
// [05/01/19] AddIn Object가 비정형 Interface를 제공할 수 있도록 제공한다.
//////////////////////////////////////////////////////////////////////
BOOL CTimeMarkerAddInImp::InvokeAddIn( int p_nCommandType, long p_lData)
{
	switch( p_nCommandType)
	{
		case 0:	m_bEnableFiringTimeShare = ( p_lData != 0);
				break;
		case 1:	m_bEnableReceivingTimeShare = ( p_lData != 0);
				break;
		case 2:	m_bEnableFiringTimeShare = ( p_lData != 0);
				m_bEnableReceivingTimeShare = ( p_lData != 0);
				break;
	}

	if( 0 < p_nCommandType && !m_bEnableReceivingTimeShare)
	{
		ClearTimeMark();
		// (2004.12.13, 배승원) NULL Pointer를 확인한다.
		m_pIChartOCX->InvalidateControl();
	}

	// (2006/11/24 - Seung-Won, Bae) Notify Tool State.
	m_pIChartOCX->OnToolStateChanged( CToolOptionInfo::T_ENABLE_TIMEMARK, m_bEnableFiringTimeShare && m_bEnableReceivingTimeShare);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// (2004.10.11, 배승원) 기본적인 Interface를 Default로 제공한다. (Derived Class에서 재정의하지 않아도 오류가 나지 않도록)
//////////////////////////////////////////////////////////////////////

// [04/10/11] Global 공유를 전달하는 Interface를 제공한다.
// (2005/8/10 - Seung-Won, Bae) Not used in Hanwha
void CTimeMarkerAddInImp::OnGSharedDataChange( const char *p_szGShareKey, const char *p_szGShareValue, BOOL p_bStartShare)
{
	// (2004.09.30, 배승원) 시간공유의 수신을 허용하는 Flag을 두어 제한할 수 있도록 한다.
	//		발신은 제한하지 않는다.
	if( !m_bEnableReceivingTimeShare) return;

	// 0. 그리드의 시간 공유에 대한 처리만을 제공한다.
	if( strnicmp( p_szGShareKey, _MTEXT( C6_TIME_GRID), 11)) return;
	if( p_szGShareKey[ 11] != '\0' &&  !( p_szGShareKey[ 11] == ';' && p_szGShareKey[ 12] == '\0')) return;

	// 1. TimeMarker에게 시간 공유를 설정시킨다.
	// (2004.09.14, 배승원) 내부적으로 OCX를 Invalidate 시키도록 한다.
	SetTimeMarkString( p_szGShareValue);
}

// [04/10/11] AddIn DLL들에게 ResetChart 상황을 알려 처리되도록 한다.
void CTimeMarkerAddInImp::OnResetChart( void)
{
	// (2004.07.26, 배승원) 시간 공유인 TimeMark도 Clear시킨다.
	ClearTimeMark();
}

// (2004.06.10, 배승원) Drawing 시점에 수직선을 출력한다.
void CTimeMarkerAddInImp::OnDrawBeforeGraph( HDC p_hDC)
{
	CDC *pDC = CDC::FromHandle( p_hDC);
	if( !pDC) return;

	// (2004.09.15, 배승원) 특수차트인 경우에는 보이지 않도록 한다. (역시계곡선/삼선전환도/P&F)
	if( m_pIChartOCX->IsOnSpecialChart()) return;

	// 0. Drawing이 가능한 상황인지 확인한다.
	if( !m_lMarkTime) return;

	// (2004.07.05, 배승원) Time Mark의 값이 다른 종목에서도 유효하도록, 
	//		Packet의 Type을 관리하여, 변경된 경우가 아니면 계속 Draw하도록 한다.
	IPacket *pIPacket = m_pIDefaultPacketManager->GetPacket( _MTEXT( C0_DATE_TIME));
	if( !pIPacket) return;
	try
	{
		ILPCSTR szPacketType = pIPacket->GetType();
		if( m_strPacketType != szPacketType) throw this;

		// 1. Mark Time의 Data Index를 구한다.
		// 1.1 현 Data View의 Start/End Index를 구한다.
		int nStartIndex, nEndIndex;
		if( !m_pIChartOCX->GetViewStartEndIndex( nStartIndex, nEndIndex)) throw this;	
		// 1.2 Time Mark의 위치를 확인한다.
		// (2004.06.18, 배승원) 정확한 Data의 Index가 구해지지 않을 경우 그 이전 Data Index를 구한다.
		int nTimeIndex;
		double dSelectedDate;
		if( !pIPacket->GetCurOrPrevIndexFromData( m_lMarkTime, nTimeIndex, dSelectedDate)) throw this;
		// (2004.09.24, 배승원) ScrollBar도 ZeroBase여서 1을 증가시키지 않는다.
		// nTimeIndex++;

		// (2004.09.14, 배승원) 녹색 팬만을 관리한다.
		CPen *pOldPen = pDC->SelectObject( &m_pnPen);

		// 2. 해당 Data Index의 View Point를 구한다.
		// 2.1 Block Count를 확인한다.
		int nBlockRow		= m_pIChartManager->GetRowCount();
		int nBlockColumn	= m_pIChartManager->GetColumnCount();
		// 2.2 매 Block마다 TimeMark를 그린다.
		int			nX, nDataOnPage = nEndIndex - nStartIndex + 1;
		int			nLeftBorder, nRightBorder, nTimeOffset = nTimeIndex - nStartIndex;
		CRect		rctBlockRegion, rctNoFrameAndMargin, rctGraphRegion;
		double		dCandleWidth;
		for( int nR = 0; nR < nBlockRow; nR++) for( int nC = 0; nC < nBlockColumn; nC++)
		{
			// 2.2.1 Block과 Region을 구한다.
			// (2006/6/16 - Seung-Won, Bae) Use Block Interface
			IBlock *pIBlock = m_pIChartManager->GetBlock( nR, nC);
			if( !pIBlock) continue;
			pIBlock->GetGraphRegions( rctBlockRegion, rctNoFrameAndMargin);
			rctGraphRegion = pIBlock->GetGraphDrwingRegion();
			pIBlock->Release();
			pIBlock = NULL;

			// 2.2.2 TimeMark의 X Position을 구한다. (Graph의 X Position은 Graph Region을 기준으로 산출한다.)
			// (2004.09.14, 배승원) Graph의 정확한 중심을 구하여 Drawing하도록 한다.
			//		정확한 중심을 구하기 위해서는 봉의 좌측경계선과 우측경계선의 중심(버림)이다.
			//		따라서 해당 봉의 좌경계와 우경계의 위치를 산출하고, 이를 평균낸다.
			// 2.2.2.1 봉의 폭을 구한다.
			dCandleWidth = double( rctGraphRegion.Width()) / double( nDataOnPage);
			// 2.2.2.2 봉의 좌경계를 구한다. (실 봉의 영역보다 1 Pixel(여백) 이후이다.)
			nLeftBorder = rctGraphRegion.left + int( dCandleWidth * double( nTimeOffset) + 0.5) + 1;
			// 2.2.2.3 봉의 우경계를 구한다. (다음 실 봉의 좌경계보다 1 Pixel(여백) 이전이다.)
			nRightBorder = rctGraphRegion.left + int( dCandleWidth * double( nTimeOffset + 1) + 0.5) - 1;
			// 2.2.2.4 봉의 중심을 잡는다. (버림으로 계산한다.)
			//{{ 2007.03.26 by LYH XScale 적용
			//nX = ( nLeftBorder + nRightBorder) / 2;
			nX = m_pIChartManager->GetXPosition("DEFAULT", nTimeIndex, 0);
			//}}
			// (2004.08.25, 배승원) Mark Line이 Graph 영역 밖으로 나가는 경우를 확인한다.
			if( nX <= rctGraphRegion.left || rctGraphRegion.right <= nX) continue;
			// 2.2.3 Line을 그린다. (시간 공유 Line은 Block Region에 그린다. 특히 Block 경계까지 그려서 행간 시간 공유선이 연장되도록 한다.)
			// (2004.09.14, 배승원) Pen의 굵기가 3 Pixel이 되면서, 경계선까지 그리지 않아도 경계선까지 영향을 미친다.
			
			// 2011.01.06 by SYS >> 분차트 YYYY(년) 추가
			//if( m_lMarkTime == ( long)dSelectedDate)
			if( m_lMarkTime == ( __int64)dSelectedDate)
			// 2011.01.06 by SYS <<
			{
				pDC->MoveTo( nX, rctBlockRegion.top);
				pDC->LineTo( nX, rctBlockRegion.bottom);
			}
			else
			{
				const int nUnitLineLen = 10;
				const int nGapLen = 5;
				for( int nL = int( rctBlockRegion.top / nUnitLineLen) * nUnitLineLen; nL < rctBlockRegion.bottom; nL += nUnitLineLen)
				{
					pDC->MoveTo( nX, max( rctBlockRegion.top, nL));
					pDC->LineTo( nX, min( rctBlockRegion.bottom, nL + nUnitLineLen - nGapLen));
				}
			}
		}

		// (2004.09.14, 배승원) 녹색 팬만을 관리한다.
		pDC->SelectObject( pOldPen);
	}
	catch( CTimeMarkerAddInImp *)
	{
		pIPacket->Release();
		return;
	}
	pIPacket->Release();
}

// (2006/11/24 - Seung-Won, Bae) On Tool Command
void CTimeMarkerAddInImp::OnToolCommand( const int p_nToolType, const int p_nCmdOption)
{
	if( p_nToolType != CToolOptionInfo::T_ENABLE_TIMEMARK) return;
	InvokeAddIn( 2, p_nCmdOption);
}

//////////////////////////////////////////////////////////////////////
// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
//////////////////////////////////////////////////////////////////////

BOOL CTimeMarkerAddInImp::OnAddInToolCommand( const EAICommandType p_eCommandID)
{
	if( EAI_CUSTOM_CLEAR_TIMEMARK != p_eCommandID) return FALSE;
	ClearTimeMark();
	return TRUE;
}

BOOL CTimeMarkerAddInImp::OnAddInToolCommand( const EAICommandType p_eCommandID, const char *p_szParam, const long p_lParam)
{
	if( EAI_CUSTOM_SET_TIMEMARK != p_eCommandID) return FALSE;
	OnSetTimeMark( p_szParam, p_lParam);
	return TRUE;
}

BOOL CTimeMarkerAddInImp::OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam)
{
	if( EAI_CUSTOM_ENABLE_RECEIVING_TIMESHARE != p_eCommandID) return FALSE;
	OnEnableReceivingTimeShare( p_lParam);
	return TRUE;
}

// (2006/2/18 - Seung-Won, Bae) Do not support deleting with Analysis Tool.
//		Use EAI_CUSTOM_CLEAR_TIMEMARK with OCX Method for Item Changing
/*
BOOL CTimeMarkerAddInImp::OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam1, const long p_lParam2)
{
	// (2004.07.27, 배승원) 분석도구 모두 지우기에도 TimeMark를 지우도록 한다.
	// (2004.08.12, 배승원) 분석도구를 모두 지우는 것은 종목이 변경되는 경우에도 자동처리되어야 한다.
	//		그러나 그 시점에서는 TimeMark가 지워지지 않도록 분석도구 Message에서 따로 삭제하도록 한다.
	if( EAI_OOA_ON_TOOL_FUNCTION == p_eCommandID && CToolOptionInfo::T_DELETE_ALL == ( CToolOptionInfo::TOOLOPTION)p_lParam1)
		ClearTimeMark();

	return FALSE;	// CToolOptionInfo::T_DELETE_ALL is for All
}
*/

// [04/10/25] Left Mouse Button Down Event를 처리하는 Interface를 제공한다.
BOOL CTimeMarkerAddInImp::OnLButtonDown( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	DWORD dwTime = timeGetTime();
	if( dwTime - m_dwLButtonTime < m_uDoubleClickTime && m_ptPrevious.x - point.x < 3 && m_ptPrevious.y - point.y < 3)
		return OnLButtonDblClk( p_hDC, nFlags, point, p_dwMouseActionID);

	m_ptPrevious = point;
	m_dwLButtonTime = dwTime;
	// (2004.11.22, 배승원) Mouse 위치에 시간공유표시가 설정된 경우, 이를 최소시킨다.
	return ClearTimeMark( point);
}

// [04/10/11] Left Mouse Button Double Click Event를 처리하는 Interface를 제공한다.
BOOL CTimeMarkerAddInImp::OnLButtonDblClk( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	m_dwLButtonTime = 0;

	// (2006/11/16 - Seung-Won, Bae) Manage the Disable Flag of TimeMark
	if( !m_bEnableFiringTimeShare) return FALSE;

	// (2004.06.11, 배승원) 시간공유를 발생시킨다. (바닥을 Double Click한 경우여야 한다.)
	SetTimeMarkPoint( point);
	return TRUE;
}
