#ifndef _DEFINITION_H
#define _DEFINITION_H

// (2006/2/17 - Seung-Won, Bae) Share ToolBar's Height with OCX
#include "../Include_Chart/DLL_Load/ToolBarManager.h"		// for _OCX_INNERTOOLBAR_HEIGHT

// 싸이즈. 버튼의 가로 세로 싸이즈 및 슬라이더바의 너비. 레이더차트의 높이등을 지정한다.
#define TB_BUTTON_WIDTH			18							// 버튼의 가로 길이
#define TB_BUTTON_HEIGHT		_OCX_INNERTOOLBAR_HEIGHT	// 버튼의 세로 길이		// (2006/2/17 - Seung-Won, Bae) Share ToolBar's Height with OCX
#define TB_BUTTON_SLIDERBAR		70					// 슬라이더바 너비.
#define TB_RADAR_HEIGHT			30					// Radar Chart Height
#define TB_RADAR_STATIC_WIDTH	120					// Radar Chart Static Width

// 수치조회창(툴바위에 붙는넘의 한줄 높이)
#define TB_NUMERICALINQIURYDLG_HEIGHT	_OCX_INNERTOOLBAR_HEIGHT	// (2006/2/17 - Seung-Won, Bae) Share ToolBar's Height with OCX

// 버튼 인자값. 하나의 ToolOptionInfo값에 두개이상의 버튼을 생성해야 하는 경우
// ToolOptionInfo에 추가적으로 각각의 버튼을 구별해야하는 파라미터가 필요하다.
#define TB_BUTTONPARAM_LEFT			_T("Left")			// 왼쪽
#define TB_BUTTONPARAM_CENTER		_T("Center")		// 중앙
#define TB_BUTTONPARAM_RIGHT		_T("Right")			// 오른쪽

// 버튼 초기ID값. 버튼컨트롤의 ID값은 동적으로 생성시킨다.
#define TB_CTRL_INITIAL_ID		18000				// 툴바 버튼 컨트롤 초기 아이디

// 자동스크롤 타이머 아이디
#define AUTOSCROLL_LEFT_TIMER_ID	200				// 왼쪽자동스크롤 타이머 아이디
#define AUTOSCROLL_RIGHT_TIMER_ID	201				// 오른쪽 자동스크롤 타이머 아이디

#endif