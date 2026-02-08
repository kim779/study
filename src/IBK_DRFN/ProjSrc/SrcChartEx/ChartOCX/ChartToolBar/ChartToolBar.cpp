// ChartToolBar.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "ChartToolBar.h"

#include "../Include_Chart/Dll_Load/ToolBarManager.h"	// for CToolBarManager
#include "../Include_Chart/Dll_Load/IMetaTable.h"		// for _MTEXT()
#include "../Include_Chart/BlockBaseEnum.h"				// for CToolOptionInfo
#include "Definition.h"									// for TB_BUTTONPARAM_LEFTSCROLL

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CChartToolBarApp

BEGIN_MESSAGE_MAP(CChartToolBarApp, CWinApp)
	//{{AFX_MSG_MAP(CChartToolBarApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChartToolBarApp construction

CChartToolBarApp::CChartToolBarApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CChartToolBarApp object

CChartToolBarApp theApp;


/*-----------------------------------------------------------------------------------------
 - Function    :  IsToggleButton
 - Created at  :  2004-11-09   09:46
 - Author      :  최종찬
 - Description :  토글버튼인지 안토글버튼인지를 파악한다.
 - Update	   :  종찬(05/04/14) 자동스크롤버튼도 토글버튼으로 추가한다.
 -----------------------------------------------------------------------------------------*/
bool IsToggleButton(CToolOptionInfo::TOOLOPTION enumToolOption, const CString &p_strParam)
{
	switch(enumToolOption) 
	{
		case CToolOptionInfo::T_WHOLEVIEW:					// 전체보기
		case CToolOptionInfo::T_NIDLG_DRAG:					// 수치조회창	// (2006/6/28 - Seung-Won, Bae) Rename for Actual Function with Old Map Usage.
		case CToolOptionInfo::T_CROSSHAIRS_DRAG:			// 십자선
		case CToolOptionInfo::T_RADAR:						// 레이더 차트
		case CToolOptionInfo::T_PATTERN:					// 패턴분석
		case CToolOptionInfo::T_MAGNIFYINGGLASS:			// 돋보기
		case CToolOptionInfo::T_SHOW_OUTERTOOLBAR:			// 외부툴바 보이기
		case CToolOptionInfo::T_MINMAX_FULLDATA:			// 전체 최대 최소 보기
		case CToolOptionInfo::T_CHART_RAINBOW:				// 레인보우
		case CToolOptionInfo::T_CHART_BALANCE:				// 일목균형표
		case CToolOptionInfo::T_CHART_PANDF:				// P&F
		case CToolOptionInfo::T_CHART_THREELINEBREAK:		// 삼선전환도
		case CToolOptionInfo::T_CHART_REVERSE:				// 역시계곡선
		case CToolOptionInfo::T_CHART_BARFORSALE:			// 대기매물
		case CToolOptionInfo::T_SYMBOL_BULET:				// 기호Dlg 
		case CToolOptionInfo::T_SYMBOL_SQUARE:				// 사각도형
		case CToolOptionInfo::T_SYMBOL_CIRCLE:				// 원형도형
		case CToolOptionInfo::T_SYMBOL_TRIANGLE:			// 삼각형
		case CToolOptionInfo::T_SYMBOL_TEXT:				// 문자
		case CToolOptionInfo::T_TOOL_LINE_PROFIT:			// 추세선 수익률
		case CToolOptionInfo::T_TOOL_LINE:					// 추세선
		case CToolOptionInfo::T_TOOL_LINE_CROSS:			// 십자선
		case CToolOptionInfo::T_TOOL_LINE_VERT:				// 수직선
		case CToolOptionInfo::T_TOOL_LINE_HORZ:				// 수평선
		case CToolOptionInfo::T_TOOL_LINE_TRISECT:			// 삼등분선
		case CToolOptionInfo::T_TOOL_LINE_QUARTER:			// 사등분선
		case CToolOptionInfo::T_TOOL_LINE_RESISTANCE:		// 지지저항선
		case CToolOptionInfo::T_TOOL_FIBONACCI_ARC:			// 피보나치 반원
		case CToolOptionInfo::T_TOOL_FIBO_CIRCLE:			// 피보나치 원
		case CToolOptionInfo::T_TOOL_FIBONACCI_FAN:			// 피보나치 팬라인
		case CToolOptionInfo::T_TOOL_FIBO_RETRACE:			// 피보나치 되돌림(조정대)
		case CToolOptionInfo::T_TOOL_FIBONACCI_TIME:		// 피보나치 시간대
		case CToolOptionInfo::T_TOOL_GANN_LINE45:			// 갠라인(우상45도)
		case CToolOptionInfo::T_TOOL_GANN_LINE315:			// 갠라인(우하45도)
		case CToolOptionInfo::T_TOOL_GANN_FAN45:			// 갠펜(우상45도)
		case CToolOptionInfo::T_TOOL_GANN_FAN135:			// 갠펜(좌상45도)
		case CToolOptionInfo::T_TOOL_GANN_FAN225:			// 갠펜(좌하45도)
		case CToolOptionInfo::T_TOOL_GANN_FAN315:			// 갠펜(우하45도)
		case CToolOptionInfo::T_TOOL_GANN_GRID:				// 갠그리드
		case CToolOptionInfo::T_TOOL_ANDREWS_PITCHFORK:		// 엔드류 피치포크
		case CToolOptionInfo::T_TOOL_ELLIOT_WAVE_LINE:		// 엘리오드 파동선
		case CToolOptionInfo::T_TOOL_CYCLE_LINES:			// 사이클 구간
		case CToolOptionInfo::T_TOOL_CUT_LINE:				// 꺽은선 분석라인
		case CToolOptionInfo::T_TOOL_SPEED_LINE:			// 스피드 라인
		case CToolOptionInfo::T_TOOL_LINEAR_REGRESSION:		// 선형 회귀선(회귀선)
		case CToolOptionInfo::T_TOOL_RAFF_REGRESSION:		// Raff 채널선(회귀채널)
		case CToolOptionInfo::T_INQUIRY_STATUS:				// 수치조회데이타보기
		case CToolOptionInfo::T_NIDLG_OVER:					// NIDLG Cursor
		case CToolOptionInfo::T_CROSSHAIRS_OVER:			// CrossLine Cursor
		case CToolOptionInfo::T_TOOL_LINE_EXT1:				// Single Direction Trend Line
		case CToolOptionInfo::T_TOOL_LINE_EXT2:				// Double Direction Trend Line
		case CToolOptionInfo::T_TOOL_LINE_CROSS_WITH_NIDLG:	// CorssLine Tool with NIDLG
		case CToolOptionInfo::T_NIDLG_TOOL:					// NIDLG Tool
		case CToolOptionInfo::T_CROSSHAIRS_TOOL:			// CrossLine Cursor Tool
		case CToolOptionInfo::T_CROSSHAIRS_TOOL_WITH_NIDLG:	// CrossLine Cursor with NIDLG Tool
		case CToolOptionInfo::T_ENABLE_TIMEMARK:			// TimeMark Enabling Tool
															return true;
		// (2006/11/25 - Seung-Won, Bae) Support AutoScroll Tool Button with Left & Right Scroller
		case CToolOptionInfo::T_AUTOSCROLL:					return p_strParam == _MTEXT( C4_LEFT_AUTO_SCROLL) || p_strParam == _MTEXT( C4_RIGHT_AUTO_SCROLL);
	}
	return false;
}

// (2006/2/8 - Seung-Won, Bae) Create from Regular DLL
class CCommonInterface;
interface IToolBarManager;
IToolBarManager * WINAPI CreateToolBarManager( IChartCtrl *p_pIChartCtrl)
{
	return new CToolBarManager( p_pIChartCtrl);
}