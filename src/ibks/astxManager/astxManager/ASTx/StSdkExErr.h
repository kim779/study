/*
 * (C) Copyright 2005-2010. AhnLab, Inc.
 *
 * Any part of this source code can not be copied with
 * any method without prior written permission from
 * the author or authorized person.
 *
 * @version			$Revision 926 $
 * 
 */

#ifndef __STSDKEXERR_H_
#define __STSDKEXERR_H_


//	MessageText : 성공
#define STSDKEX_ERROR_SUCCESS											0x00000000L

//	MessageText : UNINITIALIZE에 실패하였습니다.
#define STSDKEX_ERROR_UNINITIALIZE_FAILED								0x32A41001L

//	MessageText : SDK 모듈을 로드하는데 실패하였습니다.
#define STSDKEX_ERROR_LOADLIBRARY_FAILED								0x32A41002L

//	MessageText : SDK 모듈에서 인터페이스를 찾을 수 없습니다.
#define STSDKEX_ERROR_GETPROCADDRESS_FAILED								0x32A41003L

//	MessageText : 기능을 시작시키는데 실패하였습니다.
#define STSDKEX_ERROR_FUNCTION_START_FAILED								0x32A41004L

//	MessageText : 기능을 종료시키는데 실패하였습니다.
#define STSDKEX_ERROR_FUNCTION_STOP_FAILED								0x32A41005L

//	MessageText : 제품이 이미 설치되어 있습니다.
#define STSDKEX_ERROR_ALREADY_INSTALLED									0x32A41006L

//	MessageText : 초기화 되지 않았습니다.
#define STSDKEX_ERROR_NOT_INITIALIZED									0x32A41007L

//	MessageText : 파라미터가 이상합니다.
#define STSDKEX_ERROR_BAD_PARAMETER										0x32A41008L

//	MessageText : 초기화에 실패하였습니다.
#define STSDKEX_ERROR_INITIALIZE_FAILED									0x32A41009L

//	MessageText : SDK 모듈이 로드되어 있지 않습니다.
#define STSDKEX_ERROR_NOT_LOADLIBRARY									0x32A4100AL

//	MessageText : 기능을 다른 곳에서 사용중이므로 종료할 수가 없습니다.
#define STSDKEX_ERROR_REMAIN_SHAREDCOUNT								0x32A4100BL

//	MessageText : 기능이 이미 종료된 상태입니다.
#define STSDKEX_ERROR_FUNCTION_ALREADY_STOPPED							0x32A4100CL

//	MessageText : 고객사 ID가 잘못 되었습니다.
#define STSDKEX_ERROR_INVALID_CUSTOMER_ID								0x32A4100DL

//	MessageText : 기능 코드가 잘못 되었습니다.
#define STSDKEX_ERROR_INVALID_FUNCTION_CODE								0x32A4100EL

//	MessageText : 제품이 설치되지 않았습니다.
#define STSDKEX_ERROR_PRODUCT_NOT_INSTALLED								0x32A4100FL

//	MessageText : 드라이버 룰이 유효하지 않습니다.
#define STSDKEX_ERROR_DRIVER_RULE_NOT_VERIFYED							0x32A41010L

//	MessageText : 보호하지 않는 Edit Control입니다.
#define STSDKEX_ERROR_NOT_PROTECTED_HWND								0x32A41011L

//	MessageText : 속성이 유효하지 않습니다.
#define STSDKEX_ERROR_NOT_PROPERTY_HWND									0x32A41012L

//	MessageText : 프로세스 ID가 잘못 지정되었습니다.
#define STSDKEX_ERROR_INVALID_PROCESS_ID								0x32A41013L

//	MessageText : 다운로드할 파일셋을 구성하는데 실패하였습니다.
#define STSDKEX_ERROR_DOWNLOAD_SETTING_FAILED							0x32A41014L

//	MessageText : 업데이트 프로그램 셋업을 받아올수 없습니다.
#define STSDKEX_ERROR_DOWNLOAD_UPDATESETUP								0x32A41015L

//	MessageText : 업데이트 프로그램 모듈 검증이 실패하였습니다.
#define STSDKEX_ERROR_CHECK_UPDATEMODULE								0x32A41016L

//	MessageText : 서명이 올바르게 되어 있지 않습니다.
#define STSDKEX_ERROR_SIGN_NOT_VERIFYED									0x32A41017L

//	MessageText : 함수를 수행하는데 실패하였습니다.
#define STSDKEX_ERROR_FUNCTION_FAILED									0x32A41018L

//	MessageText : 해킹보호 기능의 세부기능 코드가 잘못 되었습니다.
#define STSDKEX_ERROR_INVALID_SUB_FUNCTION_CODE							0x32A41019L

//	MessageText : 기능이 동작중이지 않습니다.
#define STSDKEX_ERROR_FUNCTION_NOT_RUNNING								0x32A4101AL

//	MessageText : 기능이 이미 동작중인 상태입니다.
#define STSDKEX_ERROR_FUNCTION_ALREADY_STARTED							0x32A4101BL

//	MessageText : 고객사 정책파일(기능)을 다운로드 하는데 실패하였습니다.
#define STSDKEX_ERROR_CUSTOMER_POLICY_DOWNLOAD_FAILED					0x32A4101CL

//	MessageText : 고객사 정책파일(옵션)을 다운로드 하는데 실패하였습니다.
#define STSDKEX_ERROR_OPTION_POLICY_DOWNLOAD_FAILED						0x32A4101DL

//	MessageText : 예외프로세스 리스트가 잘못 지정되었습니다.
#define STSDKEX_ERROR_INVALID_EXCEPTION_PROCESS_LIST					0x32A4101EL

//	MessageText : 우선순위(충돌 프로그램 존재, Super룰)에 따라 상위 룰이 존재하여 요청을 수행할 수 없습니다.
#define STSDKEX_ERROR_NOT_ABLE_BECAUSE_PRIORITY_RULE					0x32A4101FL

//	MessageText : 기능 연결에 실패하였습니다.
#define STSDKEX_ERROR_CONNECT_FAILED									0x32A41020L

//	MessageText : 시스템 함수 호출에 실패하였습니다.
#define STSDKEX_ERROR_FUNCTION_CALL_FAILED								0x32A41021L

//	MessageText : STSESS 실행에 실패하였습니다.
#define STSDKEX_ERROR_STSESS_EXEC_FAILED								0x32A41022L

//	MessageText : STSESS 실행이 Timeout 되었습니다.
#define STSDKEX_ERROR_STSESS_TIMEOUT_FAILED								0x32A41023L

//	MessageText : AsdSvc 가 실행중이지 않습니다.
#define STSDKEX_ERROR_ASDSVC_NOT_RUNNING								0x32A41024L

//	MessageText : ASTx 업데이트 중입니다.
#define STSDKEX_ERROR_ASTX_UPDATING										0x32A41025L

//	MessageText : CUSTOMER ID가 NULL입니다.
#define STSDKEX_ERROR_NULL_CUSTOMER_ID									0x32A41026L	

//	MessageText : ActiveConsoleSessionId를 얻지 못했습니다.
#define STSDKEX_ERROR_GET_ACTIVE_CONSOLE_SESSION_ID						0x32A41027L	

//  MessageText : STSDKEX_Initialize()보다 먼저 호출되어야 합니다.
#define STSDKEX_ERROR_CALL_AFTER_INIT									0x32A41028L	

//  MessageText : 지원하지 않는 함수입니다.
#define STSDKEX_ERROR_CALL_NOT_IMPLEMENTED								0x32A41029L	

//  MessageText : 콜백함수가 NULL입니다.
#define STSDKEX_ERROR_PB_NULL_CALLBACK									0x32A41030L	

//  MessageText : HSBCTL이 NULL입니다.
#define STSDKEX_ERROR_PB_NULL_HSBCTL									0x32A41031L	

//  MessageText : 호환성 모드가 설정되어 있습니다.
#define STSDKEX_ERROR_PB_COMPATIBLITY_MODE								0x32A41032L	

//  MessageText : CreateOptions이 실패하였습니다.
#define STSDKEX_ERROR_PB_CREATEOPTIONS_FAILED							0x32A41033L	

//  MessageText : 드라이버 복사에 실패하였습니다.
#define STSDKEX_ERROR_PB_COPYDRIVER_FAILED								0x32A41034L	

//  MessageText : 드라이버 초기화에 실패하였습니다.
#define STSDKEX_ERROR_PB_HSBDRV_INITIALIZE_FAILED						0x32A41035L	

//  MessageText : 드라이버 구동에 실패하였습니다.
#define STSDKEX_ERROR_PB_HSBDRV_START_FAILED							0x32A41036L	

//  MessageText : 드라이버 예외프로세스 설정에 실패하였습니다.
#define STSDKEX_ERROR_PB_HSBDRV_INSERTEXCEPTIONPROCESS_FAILED			0x32A41037L	

//  MessageText : 드라이버 버전확인에 실패하였습니다.
#define STSDKEX_ERROR_PB_HSBDRV_GETDRIVERVERSION_FAILED					0x32A41038L	

//  MessageText : 드라이버 로드에 실패하였습니다.
#define STSDKEX_ERROR_PB_HSBDRV_LOADDRIVER_FAILED						0x32A41039L	

//	MessageText : 업데이트 프로그램 구동에 Timeout 되었습니다
#define STSDKEX_ERROR_UPDATE_TIMEOUT									0x32A41040L


#endif	// __STSDKEXERR_H_
