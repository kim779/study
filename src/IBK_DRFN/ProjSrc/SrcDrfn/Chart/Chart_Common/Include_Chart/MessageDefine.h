#ifndef __MESSAGEDEFINE_H__
#define __MESSAGEDEFINE_H__

//Main <-> Chart Control
//const UINT RMS_ANALYSIS_TOOL = ::RegisterWindowMessage(_T("RMS_ANALYSIS_TOOL"));
const UINT RMS_ANALYSIS_FUNC = ::RegisterWindowMessage(_T("RMS_ANALYSIS_FUNC"));

//Initialize Properties
#define	UM_INIT_PROPERTIES					WM_USER + 501
//Property Page Properties	
#define	UM_PPG_PROPERTIES					WM_USER + 502
#define UM_PPG_FONT							WM_USER + 503
//#define UM_BUTTONCLICKED                    WM_USER + 503
//Scroll
#define	UM_SCROLL_STARTEND_MESSAGE			WM_USER + 506
#define UM_SCROLL_SAVE_INFO					WM_USER + 507
//Cursor exchange
#define UM_CURSOR_EXCHANGE					WM_USER + 521
//Analysis Tool
//#define UM_ANALYSIS_TOOL					WM_USER + 531
//#define UM_ANALYSIS_FUNCTION				WM_USER + 532
#define UM_TOOL_INNER						WM_USER + 533
#define UM_SYMBOL_TOOL						WM_USER + 534
//#define UM_RUN_ONECHART					WM_USER + 535	// (2006/1/31 - Seung-Won, Bae) Use RMS_ANALYSIS_FUNC with CToolOptionInfo::TOOLOPTION
//지표추가
#define UM_ADD_INDICATOR					WM_USER + 541
#define UM_ADD_INDICATOR_SYSTEM				WM_USER + 542
#define	UM_CHANGE_INDICATOR_TITLE			WM_USER + 543
#define UM_ADD_INDICATOR_NEWPACKET			WM_USER + 544
//환경설정
#define UM_ENVIRONMENT_CONFIG               WM_USER + 551
//주가예측 -> 패킷에 임의의 데이타를 추가했으므로 스크롤 재계산 및 ReDraw 시에 call
#define UM_STOCKPRICE_SCROLL_REDRAW			WM_USER + 561
//기타 메세지
#define UM_SET_ETC							WM_USER + 571
//margin
#define UM_GRAPH_MARGIN						WM_USER + 572
//그래프설정
#define UM_GRAPH_THICKNESS					WM_USER + 576
#define UM_GRAPH_TYPE_STYLE					WM_USER + 577
#define UM_GRAPH_SELECTEDDATAINDEX			WM_USER + 578
//MainBlock
#define UM_BLOCK_BEHAVIOR					WM_USER + 581 // block 추가/삭제/이동
#define UDM_GET_USERMAGIN                   WM_USER + 582

//ocx -> main
#define	UDM_CHART_RELEASE_ALL				WM_USER + 599 // 메인에 선택된 툴바 해제
#define	UDM_CHART_ADD						WM_USER + 600	// 메인 챠트 리스트에 챠트 OCX 핸들을 추가/삭제한다. 

#define UDM_GET_CHARTDATA					WM_USER+604	// 차트에 대한 DATA 가져오기	
#define UDM_GET_USERID						WM_USER+605	 //id 얻어오기
#define UDM_GET_USETOOLBAR					WM_USER+606	 //외부툴바 사용여부
#define UDM_SET_USETOOLBAR					WM_USER+607 //메인에서 외부툴바 보이기,숨기기 세팅하기.

#define	UDM_TOOL_SELECT     				WM_USER+608 // 메인에 선택된 툴바 선택

#define	UDM_STRATEGY				   		WM_USER+609

// cj 2003.5.29
#define	UDM_BUTTON_CLICK_FROM_CHART_OCX		WM_USER+610
#define UDM_SEND_DATALIST					WM_USER+611

// (2005/8/12 - Seung-Won, Bae) In Real Property Page, Retreive the Real Table Manager from OCX in Hanwha Platform
#define UDM_GET_REALTABLE_MANAGER			WM_USER+612
#define UDM_GET_HW_DESIGN_UI				WM_USER+613

#endif //__MESSAGEDEFINE_H__