#ifndef __MESSAGEDEFINE_H__
#define __MESSAGEDEFINE_H__

//Main <-> Chart Control
const UINT RMS_ANALYSIS_FUNC = ::RegisterWindowMessage(_T("RMS_ANALYSIS_FUNC"));

//Scroll
#define	UM_SCROLL_STARTEND_MESSAGE			WM_USER + 506
#define UM_SCROLL_SAVE_INFO					WM_USER + 507
//Cursor exchange
#define UM_CURSOR_EXCHANGE					WM_USER + 521

//주가예측 -> 패킷에 임의의 데이타를 추가했으므로 스크롤 재계산 및 ReDraw 시에 call
#define UM_STOCKPRICE_SCROLL_REDRAW			WM_USER + 561

//기타 메세지
#define UM_SET_ETC							WM_USER + 571

//margin
//#define UM_GRAPH_MARGIN						WM_USER + 572

//그래프설정
#define UM_GRAPH_SELECTEDDATAINDEX			WM_USER + 578

//MainBlock
#define UDM_GET_USERMAGIN                   WM_USER + 582

//ocx -> main
#define	UDM_CHART_RELEASE_ALL				WM_USER + 599 // 메인에 선택된 툴바 해제
#define	UDM_CHART_ADD						WM_USER + 600	// 메인 챠트 리스트에 챠트 OCX 핸들을 추가/삭제한다. 
#define UDM_GET_USERID						WM_USER+605	 //id 얻어오기
#define UDM_GET_USETOOLBAR					WM_USER+606	 //외부툴바 사용여부
#define	UDM_TOOL_SELECT     				WM_USER+608 // 메인에 선택된 툴바 선택

// AddIn Message
#define UDM_ADDIN_MSG						WM_USER + 614	// wParam - AddIn Key String Pointer, lParam - AddIn Custom Value.

// (2007/1/6 - Seung-Won, Bae) Add Platform Menu
#define UDM_ADD_PLATFORM_MENU				WM_USER + 615	// wParam - 0, lParam - Chart Menu Handle.
#define UDM_CHECK_PLATFORM_MENU				WM_USER + 616	// wParam - Menu ID, lParam - 0

#endif //__MESSAGEDEFINE_H__