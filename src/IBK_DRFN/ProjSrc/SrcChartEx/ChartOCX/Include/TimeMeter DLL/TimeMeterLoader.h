// TimeMeterLoader.h: interface for the CTimeMeterLoader class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TIMEMETERLOADER_H__B028E205_E689_4401_ABFD_25E6A65046A6__INCLUDED_)
#define AFX_TIMEMETERLOADER_H__B028E205_E689_4401_ABFD_25E6A65046A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TimeMeter_PDll.h"		// for PDLL
#include "TimeMeterDLLI.h"		// for TimeMeter_SumMode

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2003.12.28
//
//	목적	:	TimeMeter DLL을 동적으로 Loading하도록 Loading Helper를 제공한다.
///////////////////////////////////////////////////////////////////////////////
class CTimeMeterLoader : public PDLL  
{
// (2004.09.22, 배승원) 예전의 Contructor에서 AutoLoading하는 Code를 지위기 위해,
//		Constructor 자체를 지웠으나, VC Project에 이 Header가 포함되어 있지 않아서인지 모르지만,
//		Constructor(Default)에서 계속 Loading되는 오류가 확인되어, 명시적으로 AutoLoading을 제거한다.
//		(*. 다시 컴파일후 이 Contructor를 지우니 AutoLoading이되는 오류는 다시 나타나지 않았지만,
//			다른 Module들의 Loading시에도 쉽게 등록할 수 있도록 명시적으로 나타낸다.)
public:
	CTimeMeterLoader()	{}

// (2003.03.09) OCX의 Register시에는 Loading되지 않고 별도의 Init을 거치도록 수정한다.
public:
	void	LoadTimeMeter( void)	{	if( !m_dllHandle) LoadDll( "TimeMeter.dll");	}

// 1. 기본 측정 Interface를 제공한다.
public:
	// void TimeMeter_GetMeterTarget( DWORD64 &p_dwMeterTarget);
	DECLARE_FUNCTION1( TimeMeter_GetMeterTarget, DWORD64 &)
	// void TimeMeter_StartTimeMeter( DWORD64 p_dwMeterTarget, const char *p_szTitle, TimeMeter_SumMode p_eSumMode = TIMEMETER_STACKMODE);
	DECLARE_FUNCTION3_V3( TimeMeter_StartTimeMeter, DWORD64, const char *, TimeMeter_SumMode, TIMEMETER_STACKMODE)
	// void TimeMeter_EndTimeMeter( DWORD64 p_dwMeterTarget, const char *p_szTitle);
	DECLARE_FUNCTION2( TimeMeter_EndTimeMeter, DWORD64, const char *)
	// void TimeMeter_ResetAll( void);
	DECLARE_FUNCTION0( TimeMeter_ResetAll)
	// void TimeMeter_ShowReportDialog( BOOL p_bShowWindow = TRUE);
	DECLARE_FUNCTION1_V1( TimeMeter_ShowReportDialog, BOOL, TRUE)

// 2. AutoMeter Interface를 제공한다.
public:
	// void TimeMeter_StartAutoTimeMeter( DWORD64 p_dwMeterTarget);
	DECLARE_FUNCTION1( TimeMeter_StartAutoTimeMeter, DWORD64)
	// void TimeMeter_NextAutoTimeMeter( DWORD64 p_dwMeterTarget, const char *p_szTitle = NULL);
	DECLARE_FUNCTION2_V2( TimeMeter_NextAutoTimeMeter, DWORD64, const char *, NULL)
	// void TimeMeter_EndAutoTimeMeter( DWORD64 p_dwMeterTarget);
	DECLARE_FUNCTION1( TimeMeter_EndAutoTimeMeter, DWORD64)

// 3. CheckTime Interface를 제공한다.
public:
	// void TimeMeter_CheckTimeMeter( DWORD64 p_dwMeterTarget, const char *p_szTitle);
	DECLARE_FUNCTION2( TimeMeter_CheckTimeMeter, DWORD64, const char *)
};

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2003.12.28
//
//	목적	:	TimeMeter DLL을 Loading하는 Global LoadHelper를 Access할 수 있도록
//					선언한다.
///////////////////////////////////////////////////////////////////////////////
extern CTimeMeterLoader	g_dllTimeMeter;

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2003.12.28
//
//	목적	:	Macro Define에 따라 호출이 무효화될 수 있도록, 호출 Interface를 Macro로 정의한다.
//
//	수정	:	(2003.03.09) OCX의 Register시에는 Loading되지 않고 별도의 Init을 거치도록 수정한다.
///////////////////////////////////////////////////////////////////////////////
#if _TIMEMETER_USE
	#define _TimeMeter_LoadTimeMeter()								g_dllTimeMeter.LoadTimeMeter()
	#define _TimeMeter_GetMeterTarget( Param1)						g_dllTimeMeter.TimeMeter_GetMeterTarget( Param1)
	#define _TimeMeter_StartTimeMeter( Param1, Param2)				g_dllTimeMeter.TimeMeter_StartTimeMeter( Param1, Param2)
	#define _TimeMeter_StartTimeMeterQMode( Param1, Param2)			g_dllTimeMeter.TimeMeter_StartTimeMeter( Param1, Param2, TIMEMETER_QUEUEMODE)
	#define _TimeMeter_EndTimeMeter( Param1, Param2)				g_dllTimeMeter.TimeMeter_EndTimeMeter( Param1, Param2)
	#define _TimeMeter_ResetAll()									g_dllTimeMeter.TimeMeter_ResetAll()
	#define _TimeMeter_ShowReportDialog()							g_dllTimeMeter.TimeMeter_ShowReportDialog()
	#define _TimeMeter_HideReportDialog()							g_dllTimeMeter.TimeMeter_ShowReportDialog( FALSE)
	#define _TimeMeter_StartAutoTimeMeter( Param1)					g_dllTimeMeter.TimeMeter_StartAutoTimeMeter( Param1)
	#define _TimeMeter_NextAutoTimeMeter( Param1)					g_dllTimeMeter.TimeMeter_NextAutoTimeMeter( Param1)
	#define _TimeMeter_NextAutoTimeMeterWithTitle( Param1, Param2)	g_dllTimeMeter.TimeMeter_NextAutoTimeMeter( Param1, Param2)
	#define _TimeMeter_EndAutoTimeMeter( Param1)					g_dllTimeMeter.TimeMeter_EndAutoTimeMeter( Param1)
	#define _TimeMeter_CheckTimeMeter( Param1, Param2)				g_dllTimeMeter.TimeMeter_CheckTimeMeter( Param1, Param2)
#else
	#define _TimeMeter_LoadTimeMeter()
	#define _TimeMeter_GetMeterTarget( Param1)
	#define _TimeMeter_StartTimeMeter( Param1, Param2)
	#define _TimeMeter_StartTimeMeterQMode( Param1, Param2)
	#define _TimeMeter_EndTimeMeter( Param1, Param2)
	#define _TimeMeter_ResetAll()
	#define _TimeMeter_ShowReportDialog()
	#define _TimeMeter_HideReportDialog()
	#define _TimeMeter_StartAutoTimeMeter( Param1)
	#define _TimeMeter_NextAutoTimeMeter( Param1)
	#define _TimeMeter_NextAutoTimeMeterWithTitle( Param1, Param2)
	#define _TimeMeter_EndAutoTimeMeter( Param1)
	#define _TimeMeter_CheckTimeMeter( Param1, Param2)
#endif

#endif // !defined(AFX_TIMEMETERLOADER_H__B028E205_E689_4401_ABFD_25E6A65046A6__INCLUDED_)
