/*
 * (C) Copyright 2005-2010. AhnLab, Inc.
 *
 * Any part of this source code can not be copied with
 * any method without prior written permission from
 * the author or authorized person.
 *
 * @version			$Revision: 1705 $
 * 
 */

#ifndef __AOSSDKDEF_H_
#define __AOSSDKDEF_H_

#include "aossdkerr.h"
#include "aossdkcom.h"
#include "aossdksb.h"

#ifdef AOSSDK_EXPORTS
#define AOSSDK_API __declspec(dllexport)
#else
#define AOSSDK_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C"
{
#endif

// --------------------------------------------------------------------------------------
// AOS 초기화 : Anti KeyLogger 초기화/종료
AOSSDK_API void Aossdk_Initialize();
AOSSDK_API void Aossdk_Uninitialize();


// --------------------------------------------------------------------------------------
// AOS 업데이트 :  업데이트 실행
AOSSDK_API BOOL Aossdk_StartAosSDKA(								 
								LPCSTR szPidList,			// 제품 코드 리스트  ex) e5|e0
								LPCSTR szUniqueKey,		// 인증파일 유니크키
								HWND Hwnd,					// 현재 윈도우의 HWND
								DWORD* pdwErrorCode			// 에러코드
								);
								
AOSSDK_API BOOL Aossdk_StartAosSDKW(								 
								LPCWSTR wszPidList,			// 제품 코드 리스트  ex) e5|e0
								LPCWSTR wszUniqueKey,		// 인증파일 유니크키
								HWND Hwnd,					// 현재 윈도우의 HWND
								DWORD* pdwErrorCode			// 에러코드
								);								

// --------------------------------------------------------------------------------------
// AOS 업데이트 :  MKD, MF40을 위한 업데이트 실행
AOSSDK_API BOOL Aossdk_StartExAosSDKA( LPCSTR szUniqueKey, HWND Hwnd, DWORD* pdwErrorCode);
AOSSDK_API BOOL Aossdk_StartExAosSDKW( LPCWSTR wszUniqueKey, HWND Hwnd, DWORD* pdwErrorCode);

// --------------------------------------------------------------------------------------
// AOS 업데이트 : 인증서버경로 설정
AOSSDK_API void Aossdk_SetAuthServerA(LPCSTR szAuthServer);
AOSSDK_API void Aossdk_SetAuthServerW(LPCWSTR wszAuthServer);

// --------------------------------------------------------------------------------------
// AOS 업데이트 : 인증파일 루트경로 설정
AOSSDK_API void Aossdk_SetAuthRootUrlA(LPCSTR szAuthRootUrl);
AOSSDK_API void Aossdk_SetAuthRootUrlW(LPCWSTR wszAuthRootUrl);

// --------------------------------------------------------------------------------------
// AOS 업데이트 : UI 표시모드 설정(0:표시안함, 1:표시함)
AOSSDK_API void Aossdk_SetModeUI(int nModeUI);

// --------------------------------------------------------------------------------------
// AOS 업데이트 : 비동기모드 설정(TRUE:비동기모드, FALSE:동기모드)
AOSSDK_API void Aossdk_SetModeAsync(BOOL bAsync);

// --------------------------------------------------------------------------------------
// AOS 업데이트 : Anti KeyLogger를 제외한 제품실행 종료
AOSSDK_API BOOL Aossdk_TerminatePdA(DWORD dwServiceCode, LPCSTR szItem);
AOSSDK_API BOOL Aossdk_TerminatePdW(DWORD dwServiceCode, LPCWSTR wszItem);

AOSSDK_API void Aossdk_TerminateSDKA( LPCSTR szItem );
AOSSDK_API void Aossdk_TerminateSDKW( LPCWSTR szItem );

// --------------------------------------------------------------------------------------
// AOS 업데이트 : 업데이트 없이 키보드 보안 로드 가능하도록 설정
AOSSDK_API void Aossdk_SkipUpdate();

// --------------------------------------------------------------------------------------
// AOS Verify Interface
AOSSDK_API IAosVerify* Aossdk_GetVerifyObject(DWORD* pdwErrorCode);

// --------------------------------------------------------------------------------------
// AOS 키보드 보안 : Interface
AOSSDK_API IAosMkdA* Aossdk_GetMkdObjectA(DWORD* pdwErrorCode);
AOSSDK_API IAosMkdW* Aossdk_GetMkdObjectW(DWORD* pdwErrorCode);
AOSSDK_API IAosMkdS4* Aossdk_GetMkdS4Object(DWORD* pdwErrorCode);
AOSSDK_API void Aossdk_MkdUnload();

// --------------------------------------------------------------------------------------
// AOS Secure Browser : Interface
AOSSDK_API IAosSB*	Aossdk_GetSBObject(DWORD* pdwErrorCode);
AOSSDK_API BOOL	Aossdk_GetSBStatus(DWORD dwOption);

#ifdef __cplusplus
}
#endif

#endif // __AOSSDKDEF_H_
