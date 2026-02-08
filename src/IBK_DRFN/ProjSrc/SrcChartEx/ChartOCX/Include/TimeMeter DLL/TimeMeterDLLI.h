// TimeMeterDLLI.h: interface for the TimeMeter_...()
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TIMEMETERDLLI_H__BD96C933_9DC2_4F99_BBB6_34AC4B0F4085__INCLUDED_)
#define AFX_TIMEMETERDLLI_H__BD96C933_9DC2_4F99_BBB6_34AC4B0F4085__INCLUDED_

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2002.10.18
//
//	목적	:	TimeMeter.DLL의 Interface 정의이다.
//
//	주의	:	DLL의 Interface이므로 2002년 11월 21일 이후, 이 File을 수정하지 않습니다.
//
//	MT-Sync	:	두 개 이상의 Thread가 이 Object에 동시 접근하여, 
//					Data 공유에 문제가 발생할 수 있는가?
//					매 Code마다 참조한 Member가 유효하다고 할 수 있는가?
//					또, Routine이 공유되는 경우는 정상동작 할 수 있는가?
//					이 Object가 공유되어 삭제시 자유로운가?
//				Data		: DLL Instance, Manager Object Pointer
//				Action Flow	: Manager Auto Creation/Deletion, StartTimeMeter/EndTimeMeter, ResetAll
//				Result		: 이 DLL은 기본적으로 Manager Object를 생성하고,
//					모든 기능에 대해 Multi-Thread를 지원한다.
//
//	한계	:	1. 이 Library의 측정은 약 100분의 1에서 2정도의 오차가 발생할 수 있다.
///////////////////////////////////////////////////////////////////////////////

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef TIMEMETERDLL_EXPORTS
#define TIMEMETERDLL_API extern "C" __declspec(dllexport)
#else
#define TIMEMETERDLL_API extern "C" __declspec(dllimport)
#endif

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2002.11.13
//
//	목적	:	중복측정의 비합산 방식을 열거형으로 정의한다.
//
//	수정	:	(2003.12.27) TimeMeter 호출시 Stack/Queue Mode만 선택하고,
//					Report DLG에서 Total 여부를 설정할 수 있도록 Total Mode를 제거한다.
///////////////////////////////////////////////////////////////////////////////
typedef enum
{
	TIMEMETER_STACKMODE,
	TIMEMETER_QUEUEMODE
} TimeMeter_SumMode;

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2002.10.18
//
//	목적	:	TimeMeter.DLL의 Interface를 선언한다.
//
//	수정	:	(2002.10.22) Metering의 Enable/Disable이 손쉽게 처리되도록 Target 기능을 추가한다.
//				(2002.11.12) Title별로 합산하지 않는 기능을 추가한다.
//				(2002.11.13) Title별로 합산하지 않는 기능을 추가한다.
//				(2003.12.27) Report Dialog를 중복생성하지 않도록 Report Dialog를 Global하게 관리하며,
//					외부에서 Dialog를 접근하지 못하도록 제한한다.
//				(2003.12.27) TimeMeter 호출시 Stack/Queue Mode만 선택하고,
//					Report DLG에서 Total 여부를 설정할 수 있도록 Total Mode를 제거한다.
//					따라서, 명시하지 않는 경우 Stack Mode로 제공한다.
//				(2003.12.27) Report Dialog를 호출 Routine에서 숨기길 요청할 수 있도록 처리한다.
//				(2003.12.27) Meter Target는 Bit Value로 지정되도록 DWORD64으로 변경한다.
//				(2003.12.27) 외부에서 호출에 의한 Meter Target 설정을 지원하지 않는다.
//
//	참고	:	1. Title별 합산을 하지 않기 위해서, 자동으로 Title 뒤에 ' XXX' 일련번호를 붙인다.
///////////////////////////////////////////////////////////////////////////////
TIMEMETERDLL_API void		TimeMeter_GetMeterTarget( DWORD64 &p_dwMeterTarget);
TIMEMETERDLL_API void		TimeMeter_StartTimeMeter( DWORD64 p_dwMeterTarget, const char *p_szTitle, TimeMeter_SumMode p_eSumMode = TIMEMETER_STACKMODE);
TIMEMETERDLL_API void		TimeMeter_EndTimeMeter( DWORD64 p_dwMeterTarget, const char *p_szTitle);
TIMEMETERDLL_API void		TimeMeter_ResetAll( void);
TIMEMETERDLL_API void		TimeMeter_ShowReportDialog( BOOL p_bShowWindow = TRUE);

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2002.10.28
//
//	목적	:	자동으로 Title을 부여하면서 연속적으로 처리하는 Interface를 제공한다.
//
//	참고	:	1. 자동으로 Title을 생성하면서 Metering을 처리한다.
//				2. Title은 "Default AutoTimeMeter XX"의 형태로 생성된다.
//				3. Title이 지정되는 경우는 Default AutoTimeMeter Index가 증가하지 않는다.
//				4. 100번째 Default AutoTimeMeter Index는 다시 0으로 초기화된다.
//
//	한계	:	1. AutoTimeMeter는 시간 Flow상에서 Nesting(중첩)될 수 없다.
//
//	수정	:	(2003.12.27) Meter Target는 Bit Value로 지정되도록 Long으로 변경한다.
///////////////////////////////////////////////////////////////////////////////
TIMEMETERDLL_API void		TimeMeter_StartAutoTimeMeter( DWORD64 p_dwMeterTarget);
TIMEMETERDLL_API void		TimeMeter_NextAutoTimeMeter( DWORD64 p_dwMeterTarget, const char *p_szTitle = NULL);
TIMEMETERDLL_API void		TimeMeter_EndAutoTimeMeter( DWORD64 p_dwMeterTarget);

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2002.11.21
//
//	목적	:	소요 시간이 아니라, 측정 시각을 확인하기 위한 간단한 Interface를 제공한다.
//
//	내용	:	내부적으로 TimeMeter_StartTimeMeter()후, 곧 TimeMeter_EndTimeMeter()를 처리하며,
//				특히 TIMEMETER_QUEUEMODE로 처리된다.
//
//	수정	:	(2003.01.06) TIMEMETER_QUEUEMODE에서 TIMEMETER_TOTALMODE로 변경한다. (Name에 자동 번호 붙는 것 방지용)
//				(2003.01.16) TIMEMETER_TOTALMODE에서 TIMEMETER_QUEUEMODE로 변경하고, '000'번은 Clear 시킨다.
//					(1회성 Time Check에 일련번호 등록 방지)
//				(2003.12.27) Meter Target는 Bit Value로 지정되도록 DWORD64으로 변경한다.
///////////////////////////////////////////////////////////////////////////////
TIMEMETERDLL_API void		TimeMeter_CheckTimeMeter( DWORD64 p_dwMeterTarget, const char *p_szTitle);

#endif // !defined(AFX_TIMEMETERDLLI_H__BD96C933_9DC2_4F99_BBB6_34AC4B0F4085__INCLUDED_)
