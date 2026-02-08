// TimeMeterTarget.h: define for the TimeMeter DLL Loader
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TIMEMETERTARGET_H__B39EB2BC_8907_4783_B312_24F73E115A9A__INCLUDED_)
#define AFX_TIMEMETERTARGET_H__B39EB2BC_8907_4783_B312_24F73E115A9A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2003.12.28
//
//	목적	:	TimeMeter 측정을 무효화시키는 Macro를 정의한다.
//
//	내용	:	1. Define 값을 0으로 하거나 Define을 하지 않도록 수석처리하면 모든 TimeMeter 호출이
//					자동으로 제거된다.
//				2. 단, TimeMeter 호출을 위하여 전후 작업이 추가된 경우에는 #if _TIMEMETER_USE
//					~ #endif로 감싸 함께 제거되도록 하여야 한다.
///////////////////////////////////////////////////////////////////////////////
#define	_TIMEMETER_USE	1

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2003.12.28
//
//	목적	:	TimeMeter 호출을 위한 Interface 선언을 Include한다.
//
//	내용	:
//				void _TimeMeter_LoadTimeMeter( void)
//						1. TimeMeter Module을 Loading한다.
//						2. 반복 실행하여도 중복 Loading하지 않는다.
//
//				void _TimeMeter_GetMeterTarget( DWORD64 &p_dwMeterTarget)
//						1. 현재 측정중인 Target을 확인한다. Bit Mask로 조합되어 있다.
//
//				void _TimeMeter_StartTimeMeter( DWORD64 p_dwMeterTarget, const char *p_szTitle)
//						1. p_szTitle의 이름으로 측정을 시작시간을 설정한다.
//						2. 현재 설정된 Target에 해당되지 않으면 바로 Return한다.
//						3. p_szTitle의 이름으로 측정이 반복될 경우 Stack 방식으로 측정한다.
//							즉 마지막으로 시작한 측정을 먼저 끝낸다. (자세한 것은 Document 참조)
//
//				void _TimeMeter_StartTimeMeterQMode( DWORD64 p_dwMeterTarget, const char *p_szTitle)
//						1. Queue 방식으로 _TimeMeter_StartTimeMeter()를 처리한다.
//							먼저 시작한 측정을 먼저 끝낸다. (자세한 것은 Document 참조)
//
//				void _TimeMeter_EndTimeMeter( DWORD64 p_dwMeterTarget, const char *p_szTitle)
//						1. p_szTitle의 이름으로 시작된 측정의 마감시간을 설정하여 측정을 끝낸다.
//						2. 현재 설정된 Target에 해당되지 않으면 바로 Return한다.
//
//				void _TimeMeter_ResetAll( void)
//						1. 지금까지의 측정결과를 Reset 시킨다.
//
//				void _TimeMeter_ShowReportDialog( void)
//						1. 측정 결과 대화상자를 출력시킨다.
//
//				void _TimeMeter_HideReportDialog( void)
//						1. 측정 결과 대화상자를 숨긴다.
//
//				void _TimeMeter_StartAutoTimeMeter( DWORD64 p_dwMeterTarget)
//						1. 자동 측정을 시작한다.
//						2. 현재 설정된 Target에 해당되지 않으면 바로 Return한다.
//
//				void _TimeMeter_NextAutoTimeMeter( DWORD64 p_dwMeterTarget)
//						1. 이전 자동 측정을 마무리하고, 새로운 자동측정을 시작한다.
//						2. 현재 설정된 Target에 해당되지 않으면 바로 Return한다.
//						3. Title이 자동으로 생성되므로, 측정 결과를 정확히 판단하기 위해서는
//							정확한 Code의 실행 순서를 파악하여야 한다.
//
//				void _TimeMeter_NextAutoTimeMeterWithTitle( DWORD64 p_dwMeterTarget, const char *p_szTitle)
//						1. 새로운 자동측정에 Title을 지정하며 _TimeMeter_NextAutoTimeMeter()을 처리한다.
//
//				void _TimeMeter_EndAutoTimeMeter( DWORD64 p_dwMeterTarget)
//						1. 자동 측정을 마무리한다.
//						2. 현재 설정된 Target에 해당되지 않으면 바로 Return한다.
//
//				void _TimeMeter_CheckTimeMeter( DWORD64 p_dwMeterTarget, const char *p_szTitle)
//						1. p_szTitle의 이름으로 현재 시각을 측정한다.
//						2. 현재 설정된 Target에 해당되지 않으면 바로 Return한다.
//						3. 내부적으로 측정을 시작하자마자 바로 끝낸다.
//
///////////////////////////////////////////////////////////////////////////////
#include "TimeMeterLoader.h"

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2003.12.27
//
//	목적	:	TimeMeter의 Start/End/Check/Auto 처리시에 이용되는 Target를 사전에 Define하여 이용토록한다.
//
//	내용	:	1. TimeMeter Target의 사전 정의를 통하여 Report Dialog에서 Runtime시에 조작이 가능토록한다.
//				2. Target값의 정의는 Bit Value로 정의되며, Report Dialog에서 Runtime시 설정되는 Target와
//					Bit AND를 통하여, 측정을 의뢰하는 Target Code의 포함여부로 측정의 진행을 결정한다.
//				3. Target값의 DWORD64의 64Bit로 총 64가지 측정이 가능하다.
//				4. 그러나 측정의 Category 개념을 이용할 경우 그 이하가 된다.
//
//	주의	:	1. Target값이 0이면 측정되지 않는다.
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2003.12.29
//
//	목적	:	SK 맞춤형 HTS용, 측정 Topic을 정의한다.
//
//	내용	:	1. TimeMeter.cfg에 등록되는 Target List이다.
//				2. Target명의 첫 '+/-/.'은 Total Sum Mode의 '설정/취소/유지'를 의미한다.
///////////////////////////////////////////////////////////////////////////////
//																	= 0x123456789ABCDEFG
//	+화면 Loading 측정												= 0xFFFFFFFFFFFF0010
//	-화면 Loading 측정 (Main)										= 0x0000000000010010
//	+화면 Loading 측정 (Desk)										= 0x0000000000040010
//	+화면 Loading 측정 [Control별 CreateWindowlessControl()시간]	= 0x0000000000040020
//	+화면 Loading 측정 [Control별 SetMapInfoToCtrl()시간]			= 0x0000000000040040
//	+화면 Loading 측정 [Control별 생성 및 초기화 시간]				= 0x0000000000040060
//	-TR 처리 측정													= 0x0000000000040080
//	-화면 Loading과 TR 처리											= 0xFFFFFFFFFFFF0090
//	.Script 처리 측정												= 0x0000000000040100
//	-TR과 Script 처리 측정											= 0x0000000000040180
//	-화면 Loading과 Script 측정										= 0xFFFFFFFFFFFF0110
//	-화면 Loading과 Script 측정 (Desk)								= 0x0000000000040110
//	-화면 Loading과 TR 및 Script 측정								= 0xFFFFFFFFFFFF0190
//	+GetAllProperties2 비제공 Component								= 0x0000000000040200
//	-화면 Loading 전체 약식 측정									= 0xFFFFFFFFFFFF0400
//	+Real 처리 속도 측정											= 0xFFFFFFFFFFFF0800
//	+Drawing 처리 속도 측정											= 0xFFFFFFFFFFFF1000
//	-TEMP															= 0x0000000000000001

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2003.12.29
//
//	목적	:	SK 맞춤형 HTS용 Target을 정의한다.
///////////////////////////////////////////////////////////////////////////////
//									0x123456789ABCDEFG
// 1. Module 별 Target을 정의한다. (0x0000000000FF0000)
#define	_TMT_M_MAIN					0x0000000000010000
#define _TMT_M_PRIMLIB				0x0000000000020000
#define _TMT_M_DESK					0x0000000000040000
#define _TMT_M_SOCKET				0x0000000000080000
#define _TMT_M_TICKER				0x0000000000100000
//									0x123456789ABCDEFG
// 2. Control별 Target을 정의한다. (0xFFFFFFFFFF000000)
#define _TMT_M_ACCTINPUT			0x0000000001000000
#define _TMT_M_BUFFER				0x0000000002000000
#define _TMT_M_CALENDAR				0x0000000004000000
#define _TMT_M_CHART				0x0000000008000000
#define _TMT_M_CHECKBUTTON			0x0000000010000000
#define _TMT_M_CODE					0x0000000020000000
#define _TMT_M_CODEINPUT			0x0000000040000000
#define _TMT_M_COMBO				0x0000000080000000
#define _TMT_M_GRID					0x0000000100000000
#define _TMT_M_GROUP				0x0000000200000000
#define _TMT_M_IMAGEBUTTON			0x0000000400000000
#define _TMT_M_IMAGEVIEWER			0x0000000800000000
#define _TMT_M_INPUT				0x0000001000000000
#define _TMT_M_LABEL				0x0000002000000000
#define _TMT_M_MGRID				0x0000004000000000
#define _TMT_M_OUTPUT				0x0000008000000000
#define _TMT_M_PASS					0x0000010000000000
#define _TMT_M_PROGRESS				0x0000020000000000
#define _TMT_M_RADIO				0x0000040000000000
#define _TMT_M_RICHTX				0x0000080000000000
#define _TMT_M_SHAPE				0x0000100000000000
#define _TMT_M_SPININPUT			0x0000200000000000
#define _TMT_M_TABLE				0x0000400000000000
#define _TMT_M_TIMER				0x0000800000000000
#define _TMT_M_TREE					0x0001000000000000
#define _TMT_M_WEB					0x0002000000000000
#define _TMT_M_YOUNSOK				0x0004000000000000
#define _TMT_M_SLIMCHART			0x0008000000000000
#define _TMT_M_SLIMCODEINPUT		0x0010000000000000
#define _TMT_M_SLIMCOMBO			0x0020000000000000
#define _TMT_M_SLIMGRID				0x0040000000000000
#define _TMT_M_SLIMIMAGEBUTTON		0x0080000000000000
#define _TMT_M_SLIMSPININPUT		0x0100000000000000
#define _TMT_M_SLIMTABLE			0x0200000000000000
#define _TMT_M_FOCHART				0x0400000000000000
#define _TMT_M_DLLOCX				0x0800000000000000
//									0x123456789ABCDEFG
// 3. 기능별 Target을 정의한다.	   (0x000000000000FFF0)
#define _TMT_F_MAP_OPEN				0x0000000000000010		// 화면 Loading 측정
#define _TMT_F_CONTROL_CREATE		0x0000000000000020		// 화면 Loading 측정 [Control별 CreateWindowlessControl()시간]
#define _TMT_F_CONTROL_SETMAPINFO	0x0000000000000040		// 화면 Loading 측정 [Control별 SetMapInfoToCtrl()시간]
#define _TMT_F_TR					0x0000000000000080		// TR 처리 측정
#define _TMT_F_SCRIPT				0x0000000000000100		// Script 처리 측정
#define _TMT_F_NOT_GETBASEPROPIF	0x0000000000000200		// Control의 GetAllProperties2 제공 여부 확인
#define _TMT_F_S_MAP_T_TR			0x0000000000000400		// 화면 Loading 전체 약식 측정
#define _TMT_F_REAL					0x0000000000000800		// Real 처리 속도 측정
#define _TMT_F_DRAW					0x0000000000001000		// Drawing 처리 속도 측정
#define _TMT_F_SHARE				0x0000000000002000		// 공유 처리 속도 측정
//									0x123456789ABCDEFG
// 4. 임시 Target을 정의한다.	   (0x000000000000000F)
#define _TMT_TEMP					0x0000000000000001
#define _TMT_TEMP2					0x0000000000000002
#define _TMT_TEMP3					0x0000000000000004
#define _TMT_TEMP4					0x0000000000000008

#endif // !defined(AFX_TIMEMETERTARGET_H__B39EB2BC_8907_4783_B312_24F73E115A9A__INCLUDED_)
