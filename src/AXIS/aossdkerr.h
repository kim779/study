/*
 * (C) Copyright 2005-2010. AhnLab, Inc.
 *
 * Any part of this source code can not be copied with
 * any method without prior written permission from
 * the author or authorized person.
 *
 * @version			$Revision: 926 $
 * 
 */

#ifndef AOSSDKERR_H
#define AOSSDKERR_H


//
//	성공
//
#define AOSSDK_ERROR_SUCCESS											0x00000000L
																		
//
//	MessageText	: 스킨 파일을 받아올수 없습니다.\n잠시 후에 다시 시도해주세요.
//
#define AOSSDK_ERROR_DOWNLOAD_SKIN										0x32A10000L

//
//	MessageText	: 인증파일을 읽어들이지 못했습니다.
//
#define AOSSDK_ERROR_OPEN_AUTHFILE										0x32A10001L

//
//	MessageText	: 제품 셋업 ahn.ui 파일의 분석이 실패했습니다.
//
#define AOSSDK_ERROR_VALID_AHNUI										0x32A10002L

//
//	MessageText	: 제품 셋업 ahn.ui 파일을 받아올수 없습니다.\n잠시 후에 다시 시도해주세요.
//
#define AOSSDK_ERROR_DOWNLOAD_AHNUI										0x32A10003L

//
//	MessageText	: 제품 셋업의 검증이 실패하였습니다.
//
#define AOSSDK_ERROR_CHECK_SETUP										0x32A10005L

//
//	MessageText	: AhnUpEx.dll 호출이 실패하였습니다.
//
#define AOSSDK_ERROR_LOAD_AHNUPEX										0x32A10006L

//
//	MessageText	: AspUp.dll 호출이 실패하였습니다.
//
#define AOSSDK_ERROR_LOAD_ASPUP											0x32A10007L

//
//	MessageText	: 스킨 파일을 받아올수 없습니다.\n잠시 후에 다시 시도해주세요.
//
#define AOSSDK_ERROR_DOWNLOAD_SKIN2										0x32A10008L

//
//	MessageText	: ASPSnc.dll 호출이 실패하였습니다.
//
#define AOSSDK_ERROR_LOAD_SUPDATEI										0x32A10009L

//
//	MessageText	: 인증파일의 정보를 얻어내지 못했습니다.
//
#define AOSSDK_ERROR_READ_AUTHFILE										0x32A1000aL

//
//	MessageText	: 인증파일을 받아올수 없습니다. \n잠시 후에 다시 시도해주세요.
//
#define AOSSDK_ERROR_DOWNLOAD_AUTHFILE									0x32A1000bL

//
//	MessageText	: 제품으로 인증파일을 저장할수 없습니다.
//
#define AOSSDK_ERROR_SAVE_AUTHFILE2										0x32A1000dL

//
//	MessageText	: 이미 업데이트가 진행중입니다. (SUiPre)
//
#define AOSSDK_ERROR_EXIST_UPDATE										0x32A1000eL

//
//	MessageText	: 지원하지 않는 OS이기 때문에 실행이 불가능합니다.
//
#define AOSSDK_ERROR_ENABLE_OS											0x32A10012L

//
//	MessageText	: Administrator 권한이 없는 계정으로 로그인했습니다.\r\n
//					Administrator 권한이 없으면 프로그램을 설치하거나 사용할 수 없습니다.
//					\r\nAdministrator 권한을 가진 사용자 계정으로 로그인하십시오.
//
#define AOSSDK_ERROR_ENABLE_PRIVILEGE									0x32A10013L

//
//	MessageText	: 파일을 받아올수 없습니다.\n잠시 후에 다시 시도해주세요.
//
#define AOSSDK_ERROR_DOWNLOAD_FILE										0x32A10014L

//
//	MessageText	: 업데이트 프로그램의 ahn.ui 파일의 분석이 실패했습니다.
//
#define AOSSDK_ERROR_VALID_AHNUI2										0x32A10015L

//
//	MessageText	: Helper.dll 호출이 실패하였습니다.
//
#define AOSSDK_ERROR_LOAD_HELPER										0x32A10017L

//
//	MessageText	: 업데이트 프로그램의 모듈을 받아올수 없습니다.
//
#define AOSSDK_ERROR_DOWNLOAD_SUPDATEI									0x32A10018L

//
//	MessageText	: 업데이트 프로그램 셋업을 받아올수 없습니다.
//
#define AOSSDK_ERROR_DOWNLOAD_SUPDATEISETUP								0x32A1001aL

//
//	MessageText	: 업데이트 프로그램 셋업의 검증이 실패하였습니다.
//
#define AOSSDK_ERROR_CHECK_SUPDATEISETUP								0x32A1001bL

//
//	MessageText	: 업데이트 프로그램 모듈의 검증이 실패하였습니다.
//
#define AOSSDK_ERROR_CHECK_SUPDATEIMODULE								0x32A1001dL

//
//	MessageText	: 제품정보에 오류가 있습니다.
//
#define AOSSDK_ERROR_PRODUCT_INFO										0x32A1001eL

//
//	MessageText	: %1 파일을 실행할 수가 없습니다.
//
#define AOSSDK_ERROR_EXECUTE											0x32A1001fL

//
//	MessageText	: 압축풀기를 실패하였습니다.
//
#define AOSSDK_ERROR_EXTRACT											0x32A10020L

//
//	MessageText	: 업데이트 프로그램의 모듈이동이 실패하였습니다.
//
#define AOSSDK_ERROR_MOVEFILE											0x32A10021L

//
//	MessageText	: 스레드를 시작하지 못했습니다.
//
#define AOSSDK_ERROR_BEGINTHREAD2										0x32A10022L

//
//	MessageText	: 인증파일의 유효기간이 지났습니다.
//
#define AOSSDK_ERROR_EXPIRED_AUTHFILE									0x32A10023L

//
//	MessageText	: Smart Update i 설치 프로그램에서 오류가 발생했습니다.
//
#define AOSSDK_ERROR_EXECUTE_SUPDATEISETUP2								0x32A10024L

//
//	MessageText	: 인증파일의 정보를 얻어내지 못했습니다.
//
#define AOSSDK_ERROR_READ_AUTHFILE2										0x32A10025L

//
//	MessageText	: 지원하지 않는 이용자 권한이기 때문에 실행이 불가능합니다.
//
#define AOSSDK_ERROR_ENABLE_PRIVILEGE2									0x32A10026L

//
//	MessageText	: 지원하지 않는 버전의 Microsoft Internet Explorer 입니다.
//
#define AOSSDK_ERROR_ENABLE_IE											0x32A10027L

//
//	MessageText	: 지원하지 않는 CPU 입니다.
//
#define AOSSDK_ERROR_ENABLE_CPU											0x32A10028L

//
//	MessageText	: 파일 쓰기 권한이 없습니다.
//
#define AOSSDK_ERROR_ENABLE_WRITE										0x32A10029L

//
//	MessageText	: 스레드를 시작하지 못했습니다.
//
#define AOSSDK_ERROR_BEGINTHREAD										0x32A10032L

//
//	MessageText	: suimain.exe 를 실행하지 못했습니다.
//
#define AOSSDK_ERROR_EXECUTE_SUIMAIN									0x32A10033L

//
//	MessageText	: Not Allowed URL
//
#define AOSSDK_ERROR_NOT_ALLOWED_URL									0x32A10035L

//
//	MessageText	: The operation was canceled by the user.
//
#define AOSSDK_ERROR_CANCELLED											0x32A10036L

//
//	MessageText	: Not Allowed Process
//
#define AOSSDK_ERROR_NOT_ALLOWED_PROCESS								0x32A10037L

//
//	MessageText	: 안전 모드에서 지원하지 않습니다.
//
#define AOSSDK_ERROR_SAFE_BOOT										 	0x32A10038L

//
//	MessageText	: 알 수 없는 오류가 발생
//
#define AOSSDK_ERROR_UNSUPPORT											0x32A10039L

//
//  초기화 해제도중 실패하였습니다.
//
#define AOSSDK_ERROR_UNINITIALIZE_FAILED								0x32A41001L

//
// 	SbSdk.dll, Mkd25.dll, Mkd25def.dll 모듈을 로드하는데 실패하였습니다.
//				
#define AOSSDK_ERROR_LOADLIBRARY_FAILED									0x32A41002L

//
// 	초기화되지 않았습니다.
//
#define AOSSDK_ERROR_NOT_INITIALIZED									0x32A41007L
				
//
// 	파라미터가 이상합니다.
//
#define AOSSDK_ERROR_BAD_PARAMETER										0x32A41008L

//
// 	초기화 도중 실패하였습니다.
//
#define AOSSDK_ERROR_INITIALIZE_FAILED									0x32A41009L

//
// 	AOS MKD 모듈이 로드되지 않았습니다.
//
#define AOSSDK_ERROR_NOT_LOADLIBRARY									0x32A4100AL

//
// 업데이트가 지원되지 않습니다.
//
#define AOSSDK_ERROR_UPDATE_NOT_SUPPORTED								0x32A4100BL

//
// 업데이트 도중 실패하였습니다.
//
#define AOSSDK_ERROR_UPDATE_FAILED										0x32A4100CL

//
// 업데이트 파라미터가 이상합니다.
//
#define AOSSDK_ERROR_UPDATE_BAD_PARAMETER								0x32A4100DL

//
// 업데이트 모듈을 로드하는데 실패하였습니다.
//
#define AOSSDK_ERROR_UPDATE_LOADLIBRARY_FAILED							0x32A4100EL

//
// AOS MKD가 설치되지 않았습니다.
//
#define AOSSDK_ERROR_NOT_INSTALLED										0x32A4100FL

//
// AOS MKD호환에 장애가 발생했습니다.
// 모든 MKD 연동제품을 종료하고 다시 실행하십시오.
//
#define AOSSDK_ERROR_DRIVER_RULE_NOT_VERIFYED							0x32A41010L

//
// 보호되지 않는 Edit Control 입니다.				
//
#define AOSSDK_ERROR_NOT_PROTECTED_HWND									0x32A41011L

//
// 속성이 일치하지 않습니다.
//
#define AOSSDK_ERROR_NOT_PROPERTY_HWND									0x32A41012L

//
// 	SbSdk.dll, Mkd25.dll, Mkd25def.dll 모듈에서 인터페이스를 찾을 수 없습니다.
//				
#define AOSSDK_ERROR_GETPROCADDRESS_FAILED								0x32A41003L

#endif