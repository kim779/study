/*****************************************************************************
 
    Copyright   AhnLab, Inc. 1998-2006, All rights reserved.
 
    Any part of this source code can not be copied with
    any method without prior written permission from
    the author or authorized person.
	   
*****************************************************************************/

#ifndef __SUIPREDEF_H_
#define __SUIPREDEF_H_


#include "SUiPreCommon.h"

// 업데이트와 실행
// return : 제품설치 여부(TRUE:설치가 되었으므로 실행이 가능한 상태, FALSE:실행불가 상태)
extern "C" __declspec(dllimport) BOOL SUiPre_StartA(	DWORD dwPdCode,				// 제품코드
														LPCTSTR szUniqueKey,		// 인증파일 유니크키
														HWND Hwnd,					// 현재 윈도우의 HWND
														PFN_SUI_STATUS pfnStatus,	// 콜백함수(NULL 가능)
														void* pParam,				// 콜백 포인터(NULL 가능)
														DWORD* pdwErrorCode			// 에러코드
													);

// 업데이트와 실행(기존의 배포된 SDK를 사용처에 맞추기 위해서만 사용한다)
// return : 제품설치 여부(TRUE:설치가 되었으므로 실행이 가능한 상태, FALSE:실행불가 상태)
extern "C" __declspec(dllimport) BOOL SUiPre_Start(	DWORD dwPdCode,				// 제품코드
													LPCTSTR szUniqueKey,		// 인증파일 유니크키
													long lParam,				// 콜백 포인터(NULL 가능)
													PFN_SUI_STATUS pfnStatus,	// 콜백함수(NULL 가능)
													DWORD* pdwErrorCode			// 에러코드
													);

// UI 표시모드 설정(0:표시안함, 1:표시함)
extern "C" __declspec(dllimport) void SUiPre_SetModeUI(int nModeUI);

// 비동기모드 설정(TRUE:비동기모드, FALSE:동기모드)
extern "C" __declspec(dllimport) void SUiPre_SetModeAsync(BOOL bAsync);

// 업데이트 실행여부 결정, 제품코드가 설정되어 있으면 해당 제품 업데이트하지 않고 패스
extern "C" __declspec(dllexport) void SUiPre_SetIfOnThenPass(DWORD dwIfOnThenPass);

// 제품실행 종료
extern "C" __declspec(dllimport) BOOL SUiPre_TerminatePd(DWORD dwServiceCode, LPCTSTR szItem);

#endif // __SUIPREDEF_H_