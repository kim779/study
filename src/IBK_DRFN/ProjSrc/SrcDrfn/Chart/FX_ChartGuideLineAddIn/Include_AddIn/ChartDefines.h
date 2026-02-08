// ChartDefines.h: Define for Chart & Addine Common Values
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARTDEFINES_H__70621873_4DC4_4F9F_AB31_11D47FACF3CC__INCLUDED_)
#define AFX_CHARTDEFINES_H__70621873_4DC4_4F9F_AB31_11D47FACF3CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BlockBaseEnum.h"

// (2004.10.11, 배승원) DLL Container와 Communication을 위한 Message를 정의한다.
// (2004.05.06, 배승원) Chart OCX에서 보내는 Event Message를 정의한다.
//		WPARAM	: ID
//		LPARAM	: MSG Structure Address
//			MSG msg;
//			msg.hwnd	= GetSafeHwnd();
//			msg.message	= message;
//			msg.wParam	= wParam;
//			msg.lParam	= lParam;
#define _CHARTMSG_EVENTMESSAGE	"CHARTMSG_EVENTMESSAGE"
extern const UINT RMSG_CHARTMSG_EVENTMESSAGE;

// (2004.05.18, 배승원) RMSG_CHARTMSG_EVENTMESSAGE로 전달되는 Event Message Structure에 다음의 Message로 전달된다.
// (2004.05.18, 배승원) DLL Container에게 한 화면의 Data 수 (ZoomValue)의 변화를 전달한다.
//		이때, LParam의 값이 ZoomValue이다.
#define UWM_ZOOMVALUE				( WM_USER + 0x2D36)
// (2004.06.04, 배승원) Scroll이 변경될 때마다 DLL Container에게 끝일시를 전달한다.
//		이때, LParam의 값이 현 화면의 마지막 Data의 일시를 Long Value로 전달하게 된다.
#define UWM_VIEWENDTIME				( WM_USER + 0x2D37)
// (2004.10.08, 배승원) 십자선 이용시의 수치조회창 이용여부 변경시 Multi-Chart에게 알려,
//		다른 분할차트에도 이 설정이 적용되도록 한다.
//		이때, WParam의 값이 새로이 설정된 수치조회창 이용여부 설정값이다.
#define UWM_USEINQUIRY_CROSSLINE	( WM_USER + 0x2D39)
// (2004.11.04, 배승원) 차트의 Global 공유 Data 발생을 DLL Container에게 알리는 Message이다.
//		이때 LParam으로 "공유명;공유Data"의 형태로 Data가 전달된다.
//		기본적으로 Chart는 시간공유, 일반호가공유, 특수호가공유(화면별제한불가)를 발생시킨다.
#define UWM_FIREGSHAREDATA			( WM_USER + 0x2D3B)
// (2005/2/17 - Seung-Won, Bae) Term Analysis Infomation Notify I (Drag Start Position in Time Serial)
//		LParam : Time Value (long) of Mouse Position
#define UWM_DRAGSTART				( WM_USER + 0x2D3C)
// (2005/2/17 - Seung-Won, Bae) Term Analysis Infomation Notify II (Drag End Position in Time Serial)
//		LParam : Time Value (long) of Mouse Position
#define UWM_DRAGEND					( WM_USER + 0x2D3D)
// (2005/10/25 - Sun-Young, Kim) 차트 전체초기화
#define UWM_ORGRETURN				( WM_USER + 0x2D40)

// (2004.05.21, 배승원) 켄들차트/바차트/선차트의 Chart Type을 제공한다.
//		'켄들차트/바차트/선차트'는 '가격차트'의 SubType이나 외부적으로는 기본 Type으로 인식되도록 한다.
#define _CHARTDLL_CHARTTYPE_PRICE			"가격차트"
#define _CHARTDLL_CHARTTYPE_CANDLE			"캔들차트"
#define _CHARTDLL_CHARTTYPE_BAR				"바차트"
#define _CHARTDLL_CHARTTYPE_LINE			"선차트"
#define _CHARTDLL_CHARTTYPE_INVERSELINE		"역시계곡선"
#define _CHARTDLL_CHARTTYPE_THREELINEBREAK	"삼선전환도"
#define _CHARTDLL_CHARTTYPE_PNF				"P&F"

#endif // !defined(AFX_CHARTDEFINES_H__70621873_4DC4_4F9F_AB31_11D47FACF3CC__INCLUDED_)
