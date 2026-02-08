// TimeMeterLoader.cpp: implementation of the CTimeMeterLoader class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

// (2003.12.28, 배승원) TimeMeter DLL Loader의 정의를 Macro에 따라 취소할 수 있도록,
//		TimeMeterLoader.h가 아닌 TimeMeterTarget.h를 Include한다.
//#include "TimeMeterLoader.h"
#include "TimeMeterTarget.h"

#if _TIMEMETER_USE

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2003.12.28
//
//	목적	:	TimeMeter DLL을 기본으로 Loading할 수 있도록 Global로 TimeMeter Loader를 정의한다.
///////////////////////////////////////////////////////////////////////////////
CTimeMeterLoader	g_dllTimeMeter;

#endif