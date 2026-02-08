#ifndef __MESSAGEDEFINE_H__
#define __MESSAGEDEFINE_H__

//Main <-> Chart Control
const UINT RMS_ANALYSIS_FUNC = ::RegisterWindowMessage(_T("RMS_ANALYSIS_FUNC"));

//Scroll
// #define	UM_SCROLL_STARTEND_MESSAGE			WM_USER + 506	// (2007/5/23 - Seung-Won, Bae) No more used.
// #define UM_SCROLL_SAVE_INFO					WM_USER + 507	// (2007/5/23 - Seung-Won, Bae) No more used.
//Cursor exchange
#define UM_CURSOR_EXCHANGE					WM_USER + 521

//주가예측 -> 패킷에 임의의 데이타를 추가했으므로 스크롤 재계산 및 ReDraw 시에 call
// #define UM_STOCKPRICE_SCROLL_REDRAW			WM_USER + 561	// (2007/5/23 - Seung-Won, Bae) No more used.

//기타 메세지
#define UM_SET_ETC							WM_USER + 571

//margin
//#define UM_GRAPH_MARGIN						WM_USER + 572

//그래프설정
#define UM_GRAPH_SELECTEDDATAINDEX			WM_USER + 578

//MainBlock
#define UDM_GET_USERMAGIN                   WM_USER + 582

// AddIn Message
#define UDM_ADDIN_MSG						WM_USER + 614	// wParam - AddIn Key String Pointer, lParam - AddIn Custom Value.

// (2007/1/6 - Seung-Won, Bae) Add Platform Menu
#define UDM_ADD_PLATFORM_MENU				WM_USER + 615	// wParam - 0, lParam - Chart Menu Handle.
#define UDM_CHECK_PLATFORM_MENU				WM_USER + 616	// wParam - Menu ID, lParam - 0

// (2007/2/21 - Seung-Won, Bae) Draw in MemDC for Print or Save
#define UDM_DRAW_FOR_PRINT					WM_USER + 617	// wParam - 0, lParam - HDC

// (2008/10/21 - JS, Kim) User Changes Time Difference
#define UDM_CHANGE_TIMEDIFF					WM_USER + 618	// wParam - Time Index, lParam - 0
#define UDM_NOTIFY_TIMEDIFF					WM_USER + 619	// wParam - Time Index, lParam - Not Used

// (2009/1/11 - Seung-Won, Bae) for X Scale Manager's Calc_DRI (Zoom)
#define UDM_RECALC_DRI						WM_USER + 620	// wParam - NULL, lParam - NULL.

#endif //__MESSAGEDEFINE_H__